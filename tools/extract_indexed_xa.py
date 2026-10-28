"""Extract Colony Wars indexed XA sectors and classify their storage usage"""

import argparse
import bisect
import collections
import csv
import hashlib
import json
import mmap
from pathlib import Path
import struct


SECTOR_BYTES = 2352
SYNC = b'\x00' + b'\xff' * 10 + b'\x00'
PACKAGES = ('MISSIONS', 'DATABASE')


def digest_file(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def archive_indexes(path):
    with path.open('rb') as stream:
        header = stream.read(4)
        if len(header) != 4:
            raise ValueError(f'Truncated resource archive: {path}')
        count = struct.unpack('<I', header)[0] + 1
        if not 2 <= count <= 511:
            raise ValueError(f'Invalid resource directory count: {count}')
        directory = stream.read(count * 32)
        if len(directory) != count * 32:
            raise ValueError('Truncated resource directory')
        entries = []
        size = path.stat().st_size
        previous = 4 + count * 32
        for index in range(count):
            raw = directory[index * 32:(index + 1) * 32]
            name = raw[:16].split(b'\0', 1)[0].decode('ascii')
            offset = struct.unpack_from('<I', raw, 16)[0]
            if not previous <= offset <= size:
                raise ValueError(f'Invalid archive offset for {name!r}')
            entries.append((name.upper(), offset))
            previous = offset
        if entries[-1][0]:
            raise ValueError('Missing resource directory sentinel')
        result = {}
        for (name, start), (_, end) in zip(entries, entries[1:]):
            if name in (kind + '.INF' for kind in PACKAGES):
                if name in result:
                    raise ValueError(f'Duplicate index: {name}')
                stream.seek(start)
                result[name] = stream.read(end - start)
        if set(result) != {kind + '.INF' for kind in PACKAGES}:
            raise ValueError('Missing DATABASE.INF or MISSIONS.INF')
        return result


def index_records(data):
    if len(data) < 8 or data[:4] != b'STDT':
        raise ValueError('Invalid XA index signature')
    count = struct.unpack_from('<I', data, 4)[0]
    if len(data) != 8 + count * 24:
        raise ValueError('XA index size does not match its record count')
    return [struct.unpack_from('<6I', data, 8 + index * 24)
            for index in range(count)]


def audio_format(coding):
    if coding & 0xAA or (coding & 3) > 1 or (coding >> 2 & 3) > 1 or (coding >> 4 & 3) > 1:
        raise ValueError(f'Unsupported XA coding: {coding:02X}')
    channels = 2 if coding & 1 else 1
    rate = 18900 if coding & 4 else 37800
    bits = 8 if coding & 16 else 4
    frames = 18 * (4 if bits == 8 else 8) * 28 // channels
    return channels, rate, bits, frames


def classify(package, formats):
    if formats == {1} and package == 'MISSIONS':
        return 'music', 'Probable music: stereo 37.8 kHz; includes game background-music IDs'
    if formats == {4}:
        if package == 'DATABASE':
            return 'database_narration', 'Probable database narration: mono 18.9 kHz and database UI callers'
        return 'mission_speech', 'Probable mission speech: mono 18.9 kHz and radio/briefing callers'
    return 'unclassified', 'Encoding or package use does not establish a semantic type'


def extract_package(disc_root, package, index_data, disc, verify_only=False):
    source = disc_root / (package + '.PAK')
    records = index_records(index_data)
    size = source.stat().st_size
    if not size or size % SECTOR_BYTES:
        raise ValueError(f'Invalid raw-sector file size: {source}')
    sector_count = size // SECTOR_BYTES
    channels = collections.defaultdict(list)
    tracks = []
    ownership = set()
    output = disc_root / package
    with source.open('rb') as stream, mmap.mmap(stream.fileno(), 0, access=mmap.ACCESS_READ) as data:
        for sector in range(sector_count):
            offset = sector * SECTOR_BYTES
            if data[offset:offset + 12] != SYNC or data[offset + 15] != 2:
                raise ValueError(f'Invalid Mode 2 sector: {package}:{sector}')
            if data[offset + 16:offset + 20] != data[offset + 20:offset + 24]:
                raise ValueError(f'Mismatched XA subheaders: {package}:{sector}')
            file_number, channel, submode, coding = data[offset + 16:offset + 20]
            if submode & 4:
                channels[(file_number, channel)].append(sector)
        for track_id, record in enumerate(records):
            start, end, placeholder, channel, expected_count, duration_ticks = record
            if start >= sector_count or channel > 255 or not expected_count:
                raise ValueError(f'Invalid track record: {package}:{track_id}')
            warnings = []
            if end == 0xFFFFFFFF:
                limit = sector_count - 1
                warnings.append('Index has no finite end; scan matching file/channel only to the bounded PAK end')
            elif not start <= end < sector_count:
                raise ValueError(f'Invalid track bounds: {package}:{track_id}')
            else:
                limit = end
            positions = channels[(1, channel)]
            selected = positions[bisect.bisect_left(positions, start):bisect.bisect_right(positions, limit)]
            if not selected:
                raise ValueError(f'No matching audio sectors: {package}:{track_id}')
            if len(selected) != expected_count:
                warnings.append(f'Index declares {expected_count} sectors; found {len(selected)}; no sectors synthesized')
            overlap = ownership.intersection(selected)
            if overlap:
                raise ValueError(f'Overlapping indexed tracks: {package}:{track_id}')
            ownership.update(selected)
            formats = {data[sector * SECTOR_BYTES + 19] for sector in selected}
            category, basis = classify(package, formats)
            duration = 0.0
            pcm_bytes = 0
            for sector in selected:
                coding = data[sector * SECTOR_BYTES + 19]
                channel_count, rate, bits, frames = audio_format(coding)
                duration += frames / rate
                pcm_bytes += frames * channel_count * 2
            target = output / f'{track_id:04d}.xa'
            raw_hash = hashlib.sha256()
            payload_hash = hashlib.sha256()
            for sector in selected:
                offset = sector * SECTOR_BYTES
                raw_hash.update(data[offset:offset + SECTOR_BYTES])
                payload_hash.update(data[offset + 24:offset + 2328])
            expected_hash = raw_hash.hexdigest()
            if target.exists():
                if not target.is_file() or digest_file(target) != expected_hash:
                    raise FileExistsError(f'Refusing to overwrite different existing output: {target}')
            elif verify_only:
                raise FileNotFoundError(target)
            else:
                output.mkdir(parents=True, exist_ok=True)
                temporary = target.with_suffix('.xa.partial')
                with temporary.open('xb') as destination:
                    for sector in selected:
                        offset = sector * SECTOR_BYTES
                        destination.write(data[offset:offset + SECTOR_BYTES])
                if digest_file(temporary) != expected_hash:
                    raise ValueError(f'Output verification failed: {target}')
                temporary.replace(target)
            track = dict(disc=disc, package=package, track_id=track_id,
                         path=f'bin/DATA/{disc}/{package}/{target.name}',
                         category=category, classification_basis=basis,
                         classification_confidence='Inferred from encoding and game use; not individually auditioned',
                         start_sector=start, index_end_sector=end,
                         actual_first_sector=selected[0], actual_last_sector=selected[-1],
                         file_number=1, channel=channel, coding_bytes=sorted(formats),
                         indexed_sectors=expected_count, extracted_sectors=len(selected),
                         xa_bytes=len(selected) * SECTOR_BYTES,
                         adpcm_payload_bytes=len(selected) * 2304,
                         pcm_s16_bytes=pcm_bytes, duration_seconds=round(duration, 6),
                         index_duration_ticks=duration_ticks,
                         sha256=expected_hash, adpcm_sha256=payload_hash.hexdigest(), warnings=warnings)
            tracks.append(track)
        unindexed_headers = collections.Counter(
            tuple(data[sector * SECTOR_BYTES + 16:sector * SECTOR_BYTES + 20])
            for sector in range(sector_count) if sector not in ownership)
    return dict(disc=disc, package=package, source=str(source), source_bytes=size,
                source_sha256=digest_file(source), index_sha256=hashlib.sha256(index_data).hexdigest(),
                source_sectors=sector_count, indexed_unique_sectors=len(ownership),
                unindexed_sectors=sector_count - len(ownership),
                unindexed_bytes=(sector_count - len(ownership)) * SECTOR_BYTES,
                unindexed_sector_types=[dict(file_number=key[0], channel=key[1], submode=key[2],
                                             coding=key[3], sectors=count,
                                             category='unused_channel_255' if key[1] == 255 else 'unclassified')
                                        for key, count in sorted(unindexed_headers.items())],
                tracks=tracks)


def summarize(tracks):
    groups = collections.defaultdict(list)
    for track in tracks:
        groups[track['category']].append(track)
    result = {}
    for category, rows in sorted(groups.items()):
        unique = {row['sha256']: row for row in rows}
        result[category] = dict(tracks=len(rows), xa_bytes=sum(row['xa_bytes'] for row in rows),
                                adpcm_payload_bytes=sum(row['adpcm_payload_bytes'] for row in rows),
                                duration_seconds=round(sum(row['duration_seconds'] for row in rows), 6),
                                unique_raw_tracks=len(unique),
                                unique_xa_bytes=sum(row['xa_bytes'] for row in unique.values()))
    return result


def run(project, discs, report_directory, verify_only=False):
    project = project.resolve()
    if not (project / 'xport-project.json').is_file():
        raise ValueError('Project must contain xport-project.json')
    packages = []
    inputs = {}
    for disc in discs:
        root = project / 'bin/DATA' / str(disc)
        archive = root / 'GAME.RSC'
        inputs[str(archive.relative_to(project))] = digest_file(archive)
        indexes = archive_indexes(archive)
        for package in PACKAGES:
            result = extract_package(root, package, indexes[package + '.INF'], disc, verify_only)
            inputs[str((root / (package + '.PAK')).relative_to(project))] = result['source_sha256']
            packages.append(result)
            print(f'Disc {disc} {package}: {len(result["tracks"])} tracks, '
                  f'{sum(track["xa_bytes"] for track in result["tracks"])} XA bytes', flush=True)
    for path, expected in inputs.items():
        if digest_file(project / path) != expected:
            raise ValueError(f'Original input changed during extraction: {path}')
    tracks = [track for package in packages for track in package['tracks']]
    report = dict(schema='cw-indexed-xa-extraction-v1',
                  extraction='Original 2352-byte sectors matching file1/channel in inclusive INF bounds, gaps removed; headers, ADPCM and tails preserved byte-for-byte',
                  names='Four-digit numeric original table IDs, not recovered filenames',
                  classification='Semantic categories are inferred from coding and game callers, not exhaustive listening or transcript analysis',
                  byte_accounting='XA file bytes include 2352-byte sectors; ADPCM is2304bytes/sector; PCM is estimated decoded signed16 at source rate, not an exported WAV',
                  runtime='Game still reads PAK files; no playback migration or Web packaging change',
                  originals_unchanged=True, inputs=inputs,
                  per_disc={str(disc): summarize([row for row in tracks if row['disc'] == disc]) for disc in discs},
                  all_discs=summarize(tracks),
                  warnings=[dict(disc=t['disc'], package=t['package'], track_id=t['track_id'], warnings=t['warnings']) for t in tracks if t['warnings']],
                  packages=packages)
    report_directory.mkdir(parents=True, exist_ok=True)
    (report_directory / 'catalog.json').write_text(json.dumps(report, indent=2), encoding='utf-8')
    fields = ['disc', 'package', 'track_id', 'category', 'path', 'channel', 'coding_bytes',
              'indexed_sectors', 'extracted_sectors', 'xa_bytes', 'adpcm_payload_bytes',
              'pcm_s16_bytes', 'duration_seconds', 'start_sector', 'index_end_sector',
              'actual_last_sector', 'sha256', 'warnings']
    with (report_directory / 'tracks.csv').open('w', newline='', encoding='utf-8-sig') as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, extrasaction='ignore')
        writer.writeheader()
        writer.writerows(tracks)
    summary = {key: value for key, value in report.items() if key != 'packages'}
    summary['package_storage'] = [{key: value for key, value in package.items() if key != 'tracks'} for package in packages]
    (report_directory / 'summary.json').write_text(json.dumps(summary, indent=2), encoding='utf-8')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--discs', type=int, choices=(1, 2), nargs='+', default=[1, 2])
    parser.add_argument('--report-directory', type=Path)
    parser.add_argument('--verify-only', action='store_true')
    args = parser.parse_args()
    discs = list(dict.fromkeys(args.discs))
    report_directory = args.report_directory or args.project / 'status/xa-extraction'
    report = run(args.project, discs, report_directory, args.verify_only)
    print(json.dumps(report['all_discs'], indent=2))
    print(f'Reports: {report_directory}')


if __name__ == '__main__':
    main()
