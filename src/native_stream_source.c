#include "game_functions.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

typedef struct NATIVE_STREAM_ASSET
{
    const char *path;
    const char *name;
    uint32 first_sector;
    uint32 sectors;
    uint32 logical_bytes;
    CdlFILE metadata;
    FILE *file;
} NATIVE_STREAM_ASSET;

/* Original extents and logical lengths from startup-extraction.json */
static NATIVE_STREAM_ASSET native_stream_assets[] = {
    {"DATA/1/DATABASE.PAK", "DATABASE.PAK", 6239, 55350, 113356800},
    {"DATA/1/MISSIONS.PAK", "MISSIONS.PAK", 61589, 83491, 170989568}
};
static sint32 native_stream_mounted;

static void native_stream_close(void *context)
{
    uint32 index;
    for (index = 0; index < 2; ++index) {
        if (native_stream_assets[index].file)
            fclose(native_stream_assets[index].file);
        native_stream_assets[index].file = NULL;
    }
    native_stream_mounted = 0;
}

static sint32 native_stream_read_sector(void *context, uint32 sector, uint8 buffer[2352], uint32 *sector_bytes)
{
    uint32 index;
    for (index = 0; index < 2; ++index) {
        NATIVE_STREAM_ASSET *asset = &native_stream_assets[index];
        uint32 relative = sector - asset->first_sector;
        if (sector < asset->first_sector || relative >= asset->sectors)
            continue;
        if (!asset->file)
            asset->file = xport_fopen(asset->path, "rb");
        if (!asset->file || fseek(asset->file, (long)(relative * 2352u), SEEK_SET) != 0)
            return 0;
        if (xport_fread(buffer, 1, 2352, asset->file) != 2352)
            return 0;
        *sector_bytes = 2352;
        return 1;
    }
    errno = ERANGE;
    return 0;
}

static CdlFILE *native_stream_search_file(void *context, CdlFILE *result, const char *path)
{
    uint32 index;
    size_t length;
    if (!result || !path)
        return NULL;
    while (*path == '/' || *path == '\\')
        ++path;
    length = strcspn(path, ";");
    if (path[length] && strcmp(path + length, ";1") != 0)
        return NULL;
    for (index = 0; index < 2; ++index) {
        NATIVE_STREAM_ASSET *asset = &native_stream_assets[index];
        size_t bytes;
        if (strlen(asset->name) != length || strncmp(path, asset->name, length) != 0)
            continue;
        if (!xport_file_size(asset->path, &bytes))
            return NULL;
        if (bytes != (size_t)asset->sectors * 2352u) {
            errno = EINVAL;
            return NULL;
        }
        *result = asset->metadata;
        return result;
    }
    errno = ENOENT;
    return NULL;
}

sint32 game_mount_stream_source(void)
{
    XPORT_CD_SOURCE source;
    uint32 index;
    uint8 mode = 0xC8;
    if (native_stream_mounted)
        return CdControl(CdlSetmode, &mode, NULL);
    for (index = 0; index < 2; ++index) {
        NATIVE_STREAM_ASSET *asset = &native_stream_assets[index];
        memset(&asset->metadata, 0, sizeof(asset->metadata));
        CdIntToPos((sint32)asset->first_sector, &asset->metadata.pos);
        asset->metadata.size = asset->logical_bytes;
        strcpy(asset->metadata.name, asset->name);
        strcat(asset->metadata.name, ";1");
    }
    memset(&source, 0, sizeof(source));
    source.sectors = 145080;
    source.read_sector = native_stream_read_sector;
    source.search_file = native_stream_search_file;
    source.close = native_stream_close;
    if (!xport_cd_mount_source(&source))
        return 0;
    native_stream_mounted = 1;
    return CdControl(CdlSetmode, &mode, NULL);
}

sint32 game_validate_data_disc(void)
{
    size_t bytes;
    CdlFILE metadata;
    if (!xport_file_size("DATA/1/GAME.RSC", &bytes))
        return 0;
    if (bytes != 11498392u) {
        errno = EINVAL;
        return 0;
    }
    memset(&metadata, 0, sizeof(metadata));
    CdIntToPos(548, &metadata.pos);
    metadata.size = (uint32)bytes;
    strcpy(metadata.name, "GAME.RSC;1");
    memcpy(psx_addr(0x80040FC0, sizeof(metadata)), &metadata, sizeof(metadata));
    if (r_u32(0x80040FE8) != 12) {
        w_u32(0x80040FE8, 12);
        w_u32(0x80040FE4, 4);
    }
    CdControl(CdlPause, NULL, NULL);
    return 1;
}
