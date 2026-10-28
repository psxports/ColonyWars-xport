#include "game_functions.h"
#include <stdio.h>
#include <string.h>

#ifdef CW_EXTRACTED_XA
typedef struct GAME_STREAM_TRACK
{
    uint32 sector;
    uint32 frames;
    uint16 id;
    uint8 channel;
    uint8 package;
    uint8 music;
} GAME_STREAM_TRACK;

#include "native_stream_tracks.inc"

static const GAME_STREAM_TRACK *active_track;
static uint32 last_file_ticks;

sint32 game_stream_package_base(const char *name, uint32 *sector)
{
    if (strcmp(name, "MISSIONS.PAK") == 0)
        *sector = 61589;
    else if (strcmp(name, "DATABASE.PAK") == 0)
        *sector = 6239;
    else
        return 0;
    return 1;
}

sint32 game_stream_files_reset(void)
{
    sint32 owned = active_track != NULL;
    active_track = NULL;
    last_file_ticks = 0;
    return owned ? CdControl(CdlStop, NULL, NULL) : 1;
}

sint32 game_stream_file_start(uint32 record)
{
    const GAME_STREAM_TRACK *track = NULL;
    uint32 index;
    uint32 sector = r_u32(record + 28);
    uint32 channel = r_u32(record + 12);
    char path[64];
    for (index = 0; index < sizeof(game_stream_tracks) / sizeof(game_stream_tracks[0]); ++index)
        if (game_stream_tracks[index].sector == sector && game_stream_tracks[index].channel == channel) {
            track = &game_stream_tracks[index];
            break;
        }
    if (track == NULL)
        return 0;
    if (track->music)
        snprintf(path, sizeof(path), "MUSIC/CW_MISSIONS_%04u.XA", (uint32)track->id);
    else
#ifdef AP_WEB
        snprintf(path, sizeof(path), "MUSIC/CW_VOICE_%s_%04u.XA", track->package ? "DATABASE" : "MISSIONS", (uint32)track->id);
#else
        snprintf(path, sizeof(path), "DATA/1/%s/%04u.xa", track->package ? "DATABASE" : "MISSIONS", (uint32)track->id);
#endif
    if (!xport_xa_play_file_at(path, 0, (sint32)(sector + (uint32)(r_s32(record + 36) / 2)))) {
        game_stream_files_reset();
        return 0;
    }
    active_track = track;
    last_file_ticks = 0;
    return 1;
}

sint32 game_stream_file_ready(void)
{
    return active_track != NULL && xport_xa_file_position(NULL, NULL);
}

uint32 game_stream_file_remaining(void)
{
    uint32 frames;
    if (active_track == NULL || !xport_xa_file_position(NULL, &frames))
        return 0;
    last_file_ticks = (uint32)((uint64)frames * 300 / 44100);
    if (frames >= active_track->frames)
        return 0;
    return (uint32)(((uint64)(active_track->frames - frames) * 300 + 44099) / 44100);
}

void game_stream_file_advance(uint32 record)
{
    uint32 frames, ticks, remaining;
    if (active_track == NULL || !xport_xa_file_position(NULL, &frames))
        return;
    ticks = (uint32)((uint64)frames * 300 / 44100);
    remaining = r_u32(record + 40) - (ticks - last_file_ticks);
    last_file_ticks = ticks;
    if (frames >= active_track->frames)
        remaining = 0xFFFFFFFF;
    w_u32(record + 40, remaining);
}
#endif
