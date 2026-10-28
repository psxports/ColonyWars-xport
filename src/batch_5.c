#include "game_functions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "psx_gpu.h"

uint32 stream_request_segments(uint32 position, uint32 sectors, uint32 asset, uint32 segments, sint32 priority, uint32 value, const char *text)
{
    uint32 buffer;
    uint32 current;
    uint32 result;
    uint32 word;
    sint32 accept;
    FUNCTION_MARKER(0x800280C4, "SLUS_005.43");
    buffer = r_u32(0x80041008);
    result = r_u32(buffer);
    accept = 0;
    if (result)
    {
        result = priority < r_s32(buffer + 4);
        if (!result)
            accept = 1;
    }
    else
    {
        result = r_u32(r_u32(0x80041004));
        if (!result)
            accept = 1;
    }
    if (!accept)
    {
        current = r_u32(0x80041004);
        result = r_u32(current);
        if (result)
        {
            result = priority < r_s32(current + 4);
            if (!result)
                accept = 1;
        }
    }
    if (!accept)
        return result;
    if (!r_s16(segments))
        return stream_request(asset, priority, value, text);
    w_u32(r_u32(0x80041008) + 4, (uint32)priority);
    buffer = r_u32(0x80041008);
    word = r_u32(position);
    w_u32(buffer + 8, word);
    result = (uint32)CdPosToInt((CdlLOC *)psx_addr(position, sizeof(CdlLOC)));
    w_u32(r_u32(0x80041008) + 28, result);
    w_u32(r_u32(0x80041008) + 12, sectors);
    w_u32(r_u32(0x80041008) + 20, segments);
    w_u32(r_u32(0x80041008) + 24, 75);
    w_u32(r_u32(0x80041008) + 32, asset);
    w_u32(r_u32(0x80041008) + 36, 0);
    w_u32(r_u32(0x80041008) + 40, 0);
    buffer = r_u32(0x80041008);
    if (text)
        strcpy((char *)psx_addr(buffer + 48, strlen(text) + 1), text);
    else
        w_u8(r_u32(0x80041008) + 48, 0);
    w_u32(r_u32(0x80041008) + 44, value);
    buffer = r_u32(0x80041008);
    w_u32(0x80041EE4, 0);
    w_u32(buffer, 2);
    current = r_u32(0x80040FF8);
    if (current)
    {
        result = r_u32(current);
        if (result)
            return result;
    }
    w_u32(0x80040FE4, 4);
    return 4;
}

sint32 disc_draw_notice(uint32 image)
{
    static struct
    {
        uint32 ordering_table;
        POLY_FT4 polygon;
    } dma;
    PSX_RECT rectangle;
    TEX_INFO info;
    uint32 format;
    uint32 width;
    uint32 height;
    uint32 left;
    uint32 right;
    uint16 page;
    uint16 clut;
    FUNCTION_MARKER(0x8002848C, "SLUS_005.43");
    VSync(0);
    rectangle.x = 128;
    rectangle.y = 96;
    rectangle.w = 64;
    rectangle.h = 64;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    texture_read_info(image + 4, &info);
    format = info.mode & 3;
    texture_upload_at(image, 0, 256, 0, 320);
    gpu_register_packet_range(&dma, sizeof(dma));
    ClearOTag(&dma.ordering_table, 1);
    SetPolyFT4(&dma.polygon);
    width = (uint32)(uint16)info.pixel.w << ((2 - format) & 31);
    height = (uint16)info.pixel.h;
    left = 160u - (uint32)((sint32)width / 2);
    right = left + width;
    dma.polygon.rgbc.r = 128;
    dma.polygon.rgbc.g = 128;
    dma.polygon.rgbc.b = 128;
    dma.polygon.p[0].xy.y = 96;
    dma.polygon.p[1].xy.y = 96;
    dma.polygon.p[0].uv.u = 0;
    dma.polygon.p[0].uv.v = 0;
    dma.polygon.p[1].uv.v = 0;
    dma.polygon.p[2].uv.u = 0;
    dma.polygon.p[0].xy.x = (sint16)left;
    dma.polygon.p[1].xy.x = (sint16)right;
    dma.polygon.p[2].xy.x = (sint16)left;
    dma.polygon.p[2].xy.y = (sint16)(height + 96);
    dma.polygon.p[3].xy.x = (sint16)right;
    dma.polygon.p[3].xy.y = (sint16)(height + 96);
    dma.polygon.p[1].uv.u = (uint8)width;
    dma.polygon.p[2].uv.v = (uint8)height;
    dma.polygon.p[3].uv.u = (uint8)width;
    dma.polygon.p[3].uv.v = (uint8)height;
    page = GetTPage((sint32)format, 1, 0, 256);
    dma.polygon.p[1].attr = page;
    clut = GetClut(0, 320);
    dma.polygon.p[0].attr = clut;
    AddPrim(&dma.ordering_table, &dma.polygon);
    DrawOTag(&dma.ordering_table);
    DrawSync(0);
    return VSync(0);
}

sint32 disc_prompt(void)
{
    sint32 was_ready;
    sint32 result;
    FUNCTION_MARKER(0x80028628, "SLUS_005.43");
    was_ready = r_u32(0x80040FE8) == 12;
    scene_suspend_loading();
    CdDataCallback(NULL);
    CdReadyCallback(NULL);
    result = game_mount_stream_source() ? game_validate_data_disc() : 0;
    scene_resume_loading();
    return result && !was_ready ? result : 0;
}

sint32 disc_set_filter(uint32 channel)
{
    uint8 filter[2];
    sint32 result;
    FUNCTION_MARKER(0x80029A94, "SLUS_005.43");
    filter[0] = 1;
    filter[1] = (uint8)channel;
    do
        result = CdControl(13, filter, NULL);
    while (!result);
    return result;
}

sint32 disc_start_stream(uint32 position, sint32 mode)
{
    sint32 result;
    FUNCTION_MARKER(0x80029AD0, "SLUS_005.43");
    CdControl(2, (uint8 *)psx_addr(position, 3), NULL);
    do
        result = CdRead2(mode);
    while (!result);
    return result;
}

sint32 pad_initialize(void)
{
    uint32 index;
    sint32 result;
    FUNCTION_MARKER(0x8002A340, "SLUS_005.43");
    for (index = 0; index < 16; ++index)
        w_u8(0x8004EAE0 + index, 0);
    w_u8(0x8004EAE0, 0);
    w_u8(0x8004EAE4, 0);
    w_u8(0x8004EAE8, 0);
    w_u8(0x8004EAEC, 0);
    ExitCriticalSection();
    ChangeClearPAD(0);
    InitPAD(0x8004EA68, 32, 0x8004EA88, 32);
    for (index = 0; index < 21; ++index)
        w_u8(0x80040F20 + index, 0);
    for (index = 0; index < 21; ++index)
        w_u8(0x80040F35 + index, 0);
    w_u8(0x8004EAE1, 115);
    w_u8(0x8004EAE2, 0);
    w_u8(0x8004EAE3, 0);
    w_u8(0x8004EAE0, 1);
    w_u32(0x8004EAE4, r_u32(0x8004EAE0));
    w_u32(0x8004EAE8, r_u32(0x8004EAE0));
    w_u32(0x8004EAEC, r_u32(0x8004EAE0));
    StartPAD();
    ChangeClearPAD(0);
    VSync(0);
    result = pad_update(0);
    w_u8(0x800313D4, 0);
    return result;
}

void object_check_model_address(uint32 object)
{
    uint32 relative;
    FUNCTION_MARKER(0x8002B8AC, "SLUS_005.43");
    relative = r_u32(object + 100) + 0x7FFF0000u;
    if (relative > 0x001EF000u)
        diagnostic_report(210, 0x80011EAC, (intptr)0x80011EC0);
}

sint32 angle_cosine(uint32 angle)
{
    FUNCTION_MARKER(0x8002E1F8, "SLUS_005.43");
    return rcos((sint32)(angle & 0xFFF));
}

sint32 angle_sine(uint32 angle)
{
    FUNCTION_MARKER(0x8002E218, "SLUS_005.43");
    return rsin((sint32)(angle & 0xFFF));
}

sint32 matrix_from_trigonometry(uint32 matrix, uint32 sx, uint32 cx, uint32 sy, uint32 cy, uint32 sz, uint32 cz)
{
    uint32 xy;
    uint32 yy;
    sint32 first;
    sint32 second;
    sint32 third;
    sint32 width;
    uint32 mode;
    uint32 scale;
    sint32 result;
    FUNCTION_MARKER(0x8002BA04, "SLUS_005.43");
    xy = (uint32)((sint32)(sy * sx) >> 15);
    yy = (uint32)((sint32)(sy * cx) >> 15);
    w_u16(matrix + 2, (uint16)(0u - (uint32)((sint32)(sz * cy) >> 15)));
    w_u16(matrix + 8, (uint16)((sint32)(cz * cy) >> 15));
    w_u16(matrix + 12, (uint16)(0u - (uint32)((sint32)(cy * sx) >> 15)));
    w_u16(matrix + 16, (uint16)((sint32)(cy * cx) >> 15));
    w_u16(matrix, (uint16)((uint32)((sint32)(cz * cx) >> 15) - (uint32)((sint32)(sz * xy) >> 15)));
    w_u16(matrix + 4, (uint16)((uint32)((sint32)(cz * sx) >> 15) + (uint32)((sint32)(sz * yy) >> 15)));
    w_u16(matrix + 6, (uint16)((uint32)((sint32)(sz * cx) >> 15) + (uint32)((sint32)(cz * xy) >> 15)));
    result = (sint32)((uint32)((sint32)(sz * sx) >> 15) - (uint32)((sint32)(cz * yy) >> 15));
    width = r_s32(0x8003119C);
    w_u16(matrix + 14, (uint16)sy);
    w_u16(matrix + 10, (uint16)result);
    if (width == 640)
    {
        first = r_u16(matrix);
        third = r_u16(matrix + 4);
        w_u16(matrix, (uint16)((uint32)first << 1));
        second = r_u16(matrix + 2);
        w_u16(matrix + 4, (uint16)((uint32)third << 1));
        w_u16(matrix + 2, (uint16)((uint32)second << 1));
    }
    else if (width == 256 || width == 512)
    {
        scale = width == 256 ? 4 : 8;
        first = r_s16(matrix) * (sint32)scale;
        second = r_s16(matrix + 2) * (sint32)scale;
        third = r_s16(matrix + 4) * (sint32)scale;
        w_u16(matrix, (uint16)(first / 5));
        w_u16(matrix + 2, (uint16)(second / 5));
        w_u16(matrix + 4, (uint16)(third / 5));
    }
    mode = r_u32(0x800311C0);
    result = (sint32)mode;
    if (mode)
    {
        first = r_s16(matrix + 6);
        scale = r_u32(0x800311C4);
        first = (sint32)((uint32)first * scale);
        second = (sint32)((uint32)r_s16(matrix + 8) * scale);
        third = (sint32)((uint32)r_s16(matrix + 10) * scale);
        w_u16(matrix + 6, (uint16)(first / 240));
        w_u16(matrix + 8, (uint16)(second / 240));
        result = third / 240;
        w_u16(matrix + 10, (uint16)result);
    }
    return result;
}

sint32 matrix_from_angles(uint32 matrix, uint32 transform)
{
    uint32 cx;
    uint32 sx;
    uint32 cy;
    uint32 sy;
    uint32 cz;
    uint32 sz;
    FUNCTION_MARKER(0x8002B948, "SLUS_005.43");
    cx = (uint32)angle_cosine(r_u32(transform + 12));
    sx = 0u - (uint32)angle_sine(r_u32(transform + 12));
    cy = (uint32)angle_cosine(r_u32(transform + 16));
    sy = 0u - (uint32)angle_sine(r_u32(transform + 16));
    cz = (uint32)angle_cosine(r_u32(transform + 20));
    sz = 0u - (uint32)angle_sine(r_u32(transform + 20));
    return matrix_from_trigonometry(matrix, sx, cx, sy, cy, sz, cz);
}

static sint32 matrix_shift_body(uint32 source, uint32 destination, const MATRIX *native_source, MATRIX *native_destination, uint32 shift)
{
    uint32 offset;
    sint32 result;
    shift &= 31;
    for (offset = 0; offset < 18; offset += 2)
    {
        result = native_source != NULL ? native_source->m[offset / 6u][(offset / 2u) % 3u] : r_s16(source + offset);
        result >>= shift;
        if (native_destination != NULL)
            native_destination->m[offset / 6u][(offset / 2u) % 3u] = (sint16)result;
        else
            w_u16(destination + offset, (uint16)result);
    }
    return result;
}

sint32 matrix_shift(uint32 source, uint32 destination, uint32 shift)
{
    FUNCTION_MARKER(0x8002BDDC, "SLUS_005.43");
    return matrix_shift_body(source, destination, NULL, NULL, shift);
}

sint32 matrix_shift_native(const MATRIX *source, MATRIX *destination, uint32 shift)
{
    return matrix_shift_body(0u, 0u, source, destination, shift);
}

uint32 matrix_rotate_x_before(uint32 destination, uint32 source, uint32 angle)
{
    FUNCTION_MARKER(0x8002BD50, "SLUS_005.43");
    matrix_rotate_x_before_native((MATRIX *)psx_addr(destination, 20), (MATRIX *)psx_addr(source, 20), angle);
    return destination;
}

void matrix_rotate_x_before_native(MATRIX *destination, MATRIX *source, uint32 angle)
{
    MATRIX rotation = {0};
    sint16 c;
    sint16 s;
    angle &= 0xFFF;
    c = (sint16)rcos((sint32)angle);
    s = (sint16)rsin((sint32)angle);
    rotation.m[0][0] = 4096;
    rotation.m[1][1] = c;
    rotation.m[1][2] = -s;
    rotation.m[2][1] = s;
    rotation.m[2][2] = c;
    MulMatrix0(&rotation, source, destination);
}

uint32 matrix_rotate_y_before(uint32 destination, uint32 source, uint32 angle)
{
    FUNCTION_MARKER(0x8002BEFC, "SLUS_005.43");
    matrix_rotate_y_before_native((MATRIX *)psx_addr(destination, 20), (MATRIX *)psx_addr(source, 20), angle);
    return destination;
}

void matrix_rotate_y_before_native(MATRIX *destination, MATRIX *source, uint32 angle)
{
    MATRIX rotation = {0};
    sint16 c;
    sint16 s;
    angle &= 0xFFF;
    c = (sint16)rcos((sint32)angle);
    s = (sint16)rsin((sint32)angle);
    rotation.m[0][2] = -s;
    rotation.m[0][0] = c;
    rotation.m[1][1] = 4096;
    rotation.m[2][0] = s;
    rotation.m[2][2] = c;
    MulMatrix0(&rotation, source, destination);
}

uint32 matrix_rotate_z_before(uint32 destination, uint32 source, uint32 angle)
{
    FUNCTION_MARKER(0x8002C014, "SLUS_005.43");
    matrix_rotate_z_before_native((MATRIX *)psx_addr(destination, 20), (MATRIX *)psx_addr(source, 20), angle);
    return destination;
}

void matrix_rotate_z_before_native(MATRIX *destination, MATRIX *source, uint32 angle)
{
    MATRIX rotation = {0};
    sint16 c;
    sint16 s;
    angle &= 0xFFF;
    c = (sint16)rcos((sint32)angle);
    s = (sint16)rsin((sint32)angle);
    rotation.m[0][1] = s;
    rotation.m[1][0] = -s;
    rotation.m[0][0] = c;
    rotation.m[1][1] = c;
    rotation.m[2][2] = 4096;
    MulMatrix0(&rotation, source, destination);
}

uint32 matrix_apply_vector(uint32 destination, uint32 matrix, uint32 vector)
{
    FUNCTION_MARKER(0x8002C0A0, "SLUS_005.43");
    ApplyMatrixLV((MATRIX *)psx_addr(matrix, 20), (VECTOR *)psx_addr(vector, 12), (VECTOR *)psx_addr(destination, 12));
    return destination;
}

uint32 camera_from_transform(uint32 transform)
{
    uint32 camera;
    FUNCTION_MARKER(0x8002C0F8, "SLUS_005.43");
    matrix_from_angles(0x80035060, transform + 8);
    camera = r_u32(0x80040C94);
    w_u32(camera + 8, r_u32(transform + 8));
    w_u32(camera + 12, r_u32(transform + 12));
    w_u32(camera + 16, r_u32(transform + 16));
    return camera_apply_vertical_scale();
}

uint32 camera_from_direction(uint32 object)
{
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 square_x;
    uint32 square_z;
    uint32 camera;
    uint32 sx;
    uint32 cx;
    uint32 sy;
    uint32 cy;
    uint32 sz;
    uint32 cz;
    sint32 horizontal;
    sint32 length;
    FUNCTION_MARKER(0x8002C158, "SLUS_005.43");
    x = r_u32(object + 144);
    square_x = x * x;
    z = r_u32(object + 152);
    square_z = z * z;
    camera = r_u32(0x80040C94);
    y = r_u32(object + 148);
    w_u32(camera + 8, r_u32(object + 8));
    w_u32(camera + 12, r_u32(object + 12));
    w_u32(camera + 16, r_u32(object + 16));
    horizontal = (sint32)SquareRoot0((sint32)(square_x + square_z));
    length = (sint32)SquareRoot0((sint32)(square_x + y * y + square_z));
    if (horizontal <= 0)
        horizontal = 4096;
    if (length <= 0)
        length = 4096;
    sx = (uint32)((sint32)(x - (x << 12)) / horizontal);
    cx = (uint32)((sint32)((z << 12) - z) / horizontal);
    cy = (uint32)((sint32)(((uint32)horizontal << 12) - (uint32)horizontal) / length);
    sy = (uint32)((sint32)(y - (y << 12)) / length);
    sz = 0u - (uint32)angle_sine(r_u32(object + 40));
    cz = (uint32)angle_cosine(r_u32(object + 40));
    matrix_from_trigonometry(0x80035060, sx, cx, sy, cy, sz, cz);
    return camera_apply_vertical_scale();
}

uint32 camera_look_at_object_point(uint32 object)
{
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 position_x;
    uint32 position_y;
    uint32 target_y;
    uint32 square_x;
    uint32 square_z;
    uint32 camera;
    uint32 sx;
    uint32 cx;
    uint32 sy;
    uint32 cy;
    uint32 sz;
    uint32 cz;
    sint32 horizontal;
    sint32 length;
    FUNCTION_MARKER(0x8002C348, "SLUS_005.43");
    x = r_u32(object + 144);
    position_x = r_u32(object + 8);
    x -= position_x;
    square_x = x * x;
    z = r_u32(object + 152) - r_u32(object + 16);
    square_z = z * z;
    target_y = r_u32(object + 148);
    position_y = r_u32(object + 12);
    camera = r_u32(0x80040C94);
    w_u32(camera + 8, position_x);
    w_u32(camera + 12, r_u32(object + 12));
    y = target_y - position_y;
    w_u32(camera + 16, r_u32(object + 16));
    horizontal = (sint32)SquareRoot0((sint32)(square_x + square_z));
    length = (sint32)SquareRoot0((sint32)(square_x + y * y + square_z));
    if (horizontal <= 0)
        horizontal = 4096;
    if (length <= 0)
        length = 4096;
    sx = (uint32)((sint32)(x - (x << 12)) / horizontal);
    cx = (uint32)((sint32)((z << 12) - z) / horizontal);
    cy = (uint32)((sint32)(((uint32)horizontal << 12) - (uint32)horizontal) / length);
    sy = (uint32)((sint32)(y - (y << 12)) / length);
    sz = 0u - (uint32)angle_sine(r_u32(object + 40));
    cz = (uint32)angle_cosine(r_u32(object + 40));
    matrix_from_trigonometry(0x80035060, sx, cx, sy, cy, sz, cz);
    return camera_apply_vertical_scale();
}

static uint32 matrix_copy_rotation_body(uint32 destination, MATRIX *native_destination, const MATRIX *source)
{
    uint32 offset;
    uint32 result;
    for (offset = 0; offset < 18; offset += 2)
    {
        result = (uint16)source->m[offset / 6u][(offset / 2u) % 3u];
        if (native_destination != NULL)
            native_destination->m[offset / 6u][(offset / 2u) % 3u] = (sint16)result;
        else
            w_u16(destination + offset, (uint16)result);
    }
    return result;
}

uint32 matrix_copy_rotation(uint32 destination, uint32 source)
{
    FUNCTION_MARKER(0x8002C7AC, "SLUS_005.43");
    return matrix_copy_rotation_native(destination, (const MATRIX *)psx_addr(source, 18u));
}

uint32 matrix_copy_rotation_native(uint32 destination, const MATRIX *source)
{
    return matrix_copy_rotation_body(destination, NULL, source);
}

uint32 matrix_copy_rotation_to_native(MATRIX *destination, uint32 source)
{
    return matrix_copy_rotation_body(0u, destination, (const MATRIX *)psx_addr(source, 18u));
}

sint32 vector_subtract_native(VECTOR *first, VECTOR *second, VECTOR *output)
{
    sint32 result;
    FUNCTION_MARKER(0x8002C910, "SLUS_005.43");
    output->vx = (sint32)((uint32)first->vx - (uint32)second->vx);
    output->vy = (sint32)((uint32)first->vy - (uint32)second->vy);
    result = (sint32)((uint32)first->vz - (uint32)second->vz);
    output->vz = result;
    return result;
}

sint32 vector_dot_native(VECTOR *first, VECTOR *second)
{
    uint32 x;
    uint32 y;
    uint32 z;
    FUNCTION_MARKER(0x8002C9A4, "SLUS_005.43");
    x = (uint32)first->vx * (uint32)second->vx;
    y = (uint32)first->vy * (uint32)second->vy;
    z = (uint32)first->vz * (uint32)second->vz;
    return (sint32)(x + y + z);
}

sint32 vector_magnitude(uint32 vector)
{
    uint32 value;
    uint32 squared;
    FUNCTION_MARKER(0x8002CA34, "SLUS_005.43");
    value = r_u32(vector);
    squared = value * value;
    value = r_u32(vector + 4);
    squared += value * value;
    value = r_u32(vector + 8);
    squared += value * value;
    return (sint32)SquareRoot0((sint32)squared);
}

sint32 vector_make_unit(uint32 source, uint32 destination)
{
    sint32 length;
    sint32 result;
    FUNCTION_MARKER(0x8002CA88, "SLUS_005.43");
    length = vector_magnitude(source);
    if (!length)
        length = 1;
    result = (sint32)(r_u32(source) << 12) / length;
    w_u32(destination, (uint32)result);
    result = (sint32)(r_u32(source + 4) << 12) / length;
    w_u32(destination + 4, (uint32)result);
    result = (sint32)(r_u32(source + 8) << 12) / length;
    w_u32(destination + 8, (uint32)result);
    return result;
}

sint32 vector_project_native(VECTOR *first, VECTOR *axis, VECTOR *output)
{
    uint32 factor;
    sint32 result;
    FUNCTION_MARKER(0x8002CB8C, "SLUS_005.43");
    factor = (uint32)(vector_dot_native(first, axis) >> 12);
    output->vx = (sint32)((uint32)axis->vx * factor) >> 12;
    output->vy = (sint32)((uint32)axis->vy * factor) >> 12;
    result = (sint32)((uint32)axis->vz * factor) >> 12;
    output->vz = result;
    return result;
}

sint32 matrix_orthonormalize(uint32 matrix)
{
    VECTOR first;
    VECTOR second;
    VECTOR third;
    VECTOR first_unit;
    VECTOR second_unit;
    VECTOR third_unit;
    VECTOR projection_first;
    VECTOR projection_second;
    VECTOR difference;
    FUNCTION_MARKER(0x8002CC08, "SLUS_005.43");
    first.vx = r_s16(matrix);
    first.vy = r_s16(matrix + 2);
    first.vz = r_s16(matrix + 4);
    second.vx = r_s16(matrix + 6);
    second.vy = r_s16(matrix + 8);
    second.vz = r_s16(matrix + 10);
    third.vx = r_s16(matrix + 12);
    third.vy = r_s16(matrix + 14);
    third.vz = r_s16(matrix + 16);
    VectorNormal(&first, &first_unit);
    vector_project_native(&second, &first_unit, &projection_first);
    vector_subtract_native(&second, &projection_first, &projection_second);
    VectorNormal(&projection_second, &second_unit);
    vector_project_native(&third, &first_unit, &projection_first);
    vector_project_native(&third, &second_unit, &projection_second);
    vector_subtract_native(&third, &projection_first, &difference);
    vector_subtract_native(&difference, &projection_second, &projection_first);
    VectorNormal(&projection_first, &third_unit);
    w_u16(matrix, (uint16)first_unit.vx);
    w_u16(matrix + 2, (uint16)first_unit.vy);
    w_u16(matrix + 4, (uint16)first_unit.vz);
    w_u16(matrix + 6, (uint16)second_unit.vx);
    w_u16(matrix + 8, (uint16)second_unit.vy);
    w_u16(matrix + 10, (uint16)second_unit.vz);
    w_u16(matrix + 12, (uint16)third_unit.vx);
    w_u16(matrix + 14, (uint16)third_unit.vy);
    w_u16(matrix + 16, (uint16)third_unit.vz);
    return third_unit.vz;
}

void matrix_normalize(uint32 matrix, uint32 technique)
{
    FUNCTION_MARKER(0x8002CDC0, "SLUS_005.43");
    if (!technique)
    {
        matrix_orthonormalize(matrix);
        return;
    }
    if (technique != 1)
    {
        diagnostic_report(210, 0x80011EDC, (intptr)0x80011EC0);
        return;
    }
    w_u32(0x80052188, (uint32)r_s16(matrix));
    w_u32(0x8005218C, (uint32)r_s16(matrix + 2));
    w_u32(0x80052190, (uint32)r_s16(matrix + 4));
    w_u32(0x80052198, (uint32)r_s16(matrix + 6));
    w_u32(0x8005219C, (uint32)r_s16(matrix + 8));
    w_u32(0x800521A0, (uint32)r_s16(matrix + 10));
    w_u32(0x80052828, (uint32)r_s16(matrix + 12));
    w_u32(0x8005282C, (uint32)r_s16(matrix + 14));
    w_u32(0x80052830, (uint32)r_s16(matrix + 16));
    VectorNormal((VECTOR *)psx_addr(0x80052188, 12), (VECTOR *)psx_addr(0x80052840, 12));
    VectorNormal((VECTOR *)psx_addr(0x80052198, 12), (VECTOR *)psx_addr(0x80052850, 12));
    OuterProduct12((VECTOR *)psx_addr(0x80052840, 12), (VECTOR *)psx_addr(0x80052850, 12), (VECTOR *)psx_addr(0x80052868, 12));
    VectorNormal((VECTOR *)psx_addr(0x80052868, 12), (VECTOR *)psx_addr(0x80052828, 12));
    OuterProduct12((VECTOR *)psx_addr(0x80052840, 12), (VECTOR *)psx_addr(0x80052828, 12), (VECTOR *)psx_addr(0x80052850, 12));
    VectorNormal((VECTOR *)psx_addr(0x80052850, 12), (VECTOR *)psx_addr(0x80052198, 12));
    w_u16(matrix, (uint16)r_u32(0x80052840));
    w_u16(matrix + 2, (uint16)r_u32(0x80052844));
    w_u16(matrix + 4, (uint16)r_u32(0x80052848));
    w_u16(matrix + 6, (uint16)(0u - r_u32(0x80052198)));
    w_u16(matrix + 8, (uint16)(0u - r_u32(0x8005219C)));
    w_u16(matrix + 10, (uint16)(0u - r_u32(0x800521A0)));
    w_u16(matrix + 12, (uint16)r_u32(0x80052828));
    w_u16(matrix + 14, (uint16)r_u32(0x8005282C));
    w_u16(matrix + 16, (uint16)r_u32(0x80052830));
}

sint32 angle_atan2(sint32 y, sint32 x)
{
    FUNCTION_MARKER(0x8002E238, "SLUS_005.43");
    return ratan2(y, x);
}

sint32 object_angles_from_basis(uint32 object, uint32 basis)
{
    uint32 component;
    sint32 square;
    sint32 root;
    sint32 result;
    FUNCTION_MARKER(0x8002D0E4, "SLUS_005.43");
    result = angle_atan2(r_s32(basis + 24), r_s32(basis + 32));
    w_u32(object + 32, (uint32)result);
    component = r_u32(basis + 28);
    square = (sint32)(0x00FFE001u - component * component);
    if (square < 0)
        square = 0;
    root = (sint32)SquareRoot0(square);
    result = angle_atan2((sint32)(0u - r_u32(basis + 28)), root);
    w_u32(object + 36, (uint32)result);
    result = angle_atan2(r_s32(basis + 4), r_s32(basis + 16));
    w_u32(object + 40, (uint32)result);
    return result;
}

uint32 object_camera_relative(uint32 object, uint32 destination, uint32 unused, uint32 reference)
{
    FUNCTION_MARKER(0x8002D188, "SLUS_005.43");
    w_u32(0x8004EAA8, r_u32(object + 8) - r_u32(reference + 8));
    w_u32(0x8004EAAC, r_u32(object + 12) - r_u32(reference + 12));
    w_u32(0x8004EAB0, r_u32(object + 16) - r_u32(reference + 16));
    return matrix_apply_vector(destination, 0x80035060, 0x8004EAA8);
}

sint32 object_select_model(uint32 object, sint32 distance)
{
    uint32 entries;
    uint32 offset = 0;
    uint32 table;
    uint32 index;
    uint32 second_index;
    uint32 attributes;
    FUNCTION_MARKER(0x8002D1F8, "SLUS_005.43");
    entries = r_u32(object + 108);
    if (!entries)
        return 1;
    w_u8(object + 628, 0);
    if (r_u32(entries) == 0xFFFFFFFFu)
        return 0;
    for (;;)
    {
        if (r_s32(entries) >= distance)
        {
            index = r_u32(object + 116) + offset;
            table = r_u32(0x80046178);
            second_index = r_u32(object + 116) + offset;
            attributes = r_u32(table + index * 132 + 116);
            w_u32(object + 104, attributes);
            w_u32(object + 100, r_u32(table + second_index * 132));
            return 1;
        }
        entries += 8;
        w_u8(object + 628, (uint8)(r_u8(object + 628) + 1));
        ++offset;
        if (r_u32(entries) == 0xFFFFFFFFu)
            return 0;
    }
}

sint32 object_draw_transformed(uint32 object)
{
    uint32 flags;
    uint32 camera;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 shift;
    uint32 count;
    uint32 scale;
    sint32 check;
    sint32 check_y;
    sint32 check_z;
    FUNCTION_MARKER(0x8002DA68, "SLUS_005.43");
    scale = r_u32(object + 164);
    flags = r_u32(object + 112);
    w_u32(0x80031400, 0);
    w_u32(object + 128, (uint32)-10000);
    w_u32(0x800313D8, scale);
    w_u32(0x80052838, (flags & 4) ? 0x01000000 : 0);
    if (r_u32(0x8004EB2C))
        w_u32(0x80052838, r_u32(0x80052838) | 0x02000000);
    flags = r_u32(object + 112);
    camera = r_u32(0x80040C94);
    x = r_u32(object + 8) - r_u32(camera + 8);
    w_u32(0x8004EB18, flags & 2);
    flags = r_u32(object + 112);
    w_u32(0x8004EAA8, x);
    y = r_u32(object + 12) - r_u32(camera + 12);
    w_u32(0x8004EB2C, (flags >> 12) & 2);
    w_u32(0x8004EAAC, y);
    z = r_u32(object + 16) - r_u32(camera + 16);
    w_u32(0x8004EAB0, z);
    scale = r_u32(object + 132);
    if ((sint32)scale > 0)
    {
        shift = r_u32(object + 132) & 31;
        w_u32(0x8004EAA8, (uint32)((sint32)x >> shift));
        shift = r_u32(object + 132) & 31;
        w_u32(0x8004EAAC, (uint32)((sint32)y >> shift));
        shift = r_u32(object + 132) & 31;
        w_u32(0x80053670, scale);
        w_u32(0x8004EAB0, (uint32)((sint32)z >> shift));
    }
    else
        w_u32(0x80053670, 0);
    check = r_s32(0x8004EAA8);
    if (check < 0)
        check = (sint32)(0u - (uint32)check);
    if (check >= 16385)
        goto reduce;
    check = r_s32(0x8004EAAC);
    if (check < 0)
        check = (sint32)(0u - (uint32)check);
    if (check >= 16385)
        goto reduce;
    check = r_s32(0x8004EAB0);
    if (check < 0)
        check = (sint32)(0u - (uint32)check);
    if (check < 16385)
        goto reduced;
reduce:
    count = r_u32(0x80053670);
    check = r_s32(0x8004EAA8) >> 1;
    w_u32(0x8004EAA8, (uint32)check);
    if (check < 0)
        check = (sint32)(0u - (uint32)check);
    w_u32(0x80053670, count + 1);
    count = r_u32(0x80031400);
    check_z = r_s32(0x8004EAB0);
    w_u32(0x80031400, count + 1);
    check_y = r_s32(0x8004EAAC);
    check_z >>= 1;
    w_u32(0x8004EAB0, (uint32)check_z);
    check_y >>= 1;
    w_u32(0x8004EAAC, (uint32)check_y);
    if (check >= 16385)
        goto reduce;
    if (check_y < 0)
        check_y = (sint32)(0u - (uint32)check_y);
    if (check_y >= 16385)
        goto reduce;
    if (check_z < 0)
        check_z = (sint32)(0u - (uint32)check_z);
    if (check_z >= 16385)
        goto reduce;
reduced:
    scale = r_u32(object + 132);
    if ((sint32)scale < 0)
        w_u32(0x80031400, r_u32(0x80031400) + (0u - scale));
    ApplyMatrixLV((MATRIX *)psx_addr(0x80035060, 20), (VECTOR *)psx_addr(0x8004EAA8, 12), (VECTOR *)psx_addr(0x8004EAD0, 12));
    x = r_u32(0x8004EAD0);
    shift = r_u32(0x80053670) & 31;
    w_u32(object + 120, x << shift);
    w_u32(object + 124, r_u32(0x8004EAD4) << shift);
    w_u32(object + 128, r_u32(0x8004EAD8) << shift);
    matrix_copy_rotation(0x8004EAF8, object + 284);
    matrix_shift(0x8004EAF8, 0x80034DA0, r_u32(0x80031400));
    MulMatrix0((MATRIX *)psx_addr(0x80035060, 20), (MATRIX *)psx_addr(0x80034DA0, 20), (MATRIX *)psx_addr(0x800420F0, 20));
    MulMatrix0((MATRIX *)psx_addr(0x80041F28, 20), (MATRIX *)psx_addr(object + 284, 20), (MATRIX *)psx_addr(0x80034D80, 20));
    SetRotMatrix((MATRIX *)psx_addr(0x800420F0, 20));
    SetLightMatrix((MATRIX *)psx_addr(0x80034D80, 20));
    x = r_u32(0x8004EAD0);
    y = r_u32(0x8004EAD4);
    z = r_u32(0x8004EAD8);
    w_u32(0x80034D6C, x);
    w_u32(0x80034D70, y);
    w_u32(0x80034D74, z);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    mesh_render_fixed(r_u32(object + 100));
    return 0;
}

sint32 object_basis_from_vectors(VECTOR *forward, VECTOR *up, uint32 object)
{
    VECTOR forward_unit;
    VECTOR up_unit;
    VECTOR side;
    VECTOR vertical;
    FUNCTION_MARKER(0x8002DE60, "SLUS_005.43");
    vector_normalize(forward, &forward_unit);
    vector_normalize(up, &up_unit);
    OuterProduct12(&forward_unit, &up_unit, &side);
    vector_normalize(&side, &side);
    OuterProduct12(&forward_unit, &side, &vertical);
    vector_normalize(&vertical, &vertical);
    w_u16(object + 284, (uint16)side.vx);
    w_u16(object + 290, (uint16)side.vy);
    w_u16(object + 296, (uint16)side.vz);
    w_u16(object + 286, (uint16)vertical.vx);
    w_u16(object + 292, (uint16)vertical.vy);
    w_u16(object + 298, (uint16)vertical.vz);
    w_u16(object + 288, (uint16)forward_unit.vx);
    w_u16(object + 294, (uint16)forward_unit.vy);
    w_u16(object + 300, (uint16)forward_unit.vz);
    return forward_unit.vz;
}

sint32 matrix_basis_from_vectors(VECTOR *forward, VECTOR *up, uint32 matrix)
{
    VECTOR forward_unit;
    VECTOR up_unit;
    VECTOR side;
    VECTOR vertical;
    FUNCTION_MARKER(0x8002DF54, "SLUS_005.43");
    vector_normalize(forward, &forward_unit);
    vector_normalize(up, &up_unit);
    OuterProduct12(&forward_unit, &up_unit, &side);
    vector_normalize(&side, &side);
    OuterProduct12(&forward_unit, &side, &vertical);
    vector_normalize(&vertical, &vertical);
    w_u16(matrix, (uint16)side.vx);
    w_u16(matrix + 6, (uint16)side.vy);
    w_u16(matrix + 12, (uint16)side.vz);
    w_u16(matrix + 2, (uint16)vertical.vx);
    w_u16(matrix + 8, (uint16)vertical.vy);
    w_u16(matrix + 14, (uint16)vertical.vz);
    w_u16(matrix + 4, (uint16)forward_unit.vx);
    w_u16(matrix + 10, (uint16)forward_unit.vy);
    w_u16(matrix + 16, (uint16)forward_unit.vz);
    return forward_unit.vz;
}

sint32 integer_sign(sint32 value)
{
    FUNCTION_MARKER(0x8002E09C, "SLUS_005.43");
    return value > 0 ? 1 : value >> 31;
}

sint32 integer_shift_away(sint32 value, uint32 shift)
{
    sint32 sign;
    uint32 magnitude;
    sint32 shifted;
    FUNCTION_MARKER(0x8002E048, "SLUS_005.43");
    sign = integer_sign(value);
    magnitude = value < 0 ? 0u - (uint32)value : (uint32)value;
    shifted = (sint32)(0u - magnitude) >> (shift & 31);
    return (sint32)((0u - (uint32)sign) * (uint32)shifted);
}

sint32 random_between(sint32 first, sint32 second)
{
    sint32 lower;
    sint32 upper;
    sint32 value;
    sint32 range;
    FUNCTION_MARKER(0x8002E0C8, "SLUS_005.43");
    if (second < first)
    {
        lower = second;
        upper = first;
    }
    else
    {
        lower = first;
        upper = second;
    }
    value = randPSX();
    range = (sint32)((uint32)upper - (uint32)lower + 1);
    if (!range)
        trap(7);
    if (range == -1 && value == (sint32)0x80000000u)
        trap(0x5D);
    return (sint32)((uint32)(value % range) + (uint32)lower);
}

sint32 square_root_refined(sint32 value)
{
    sint32 root;
    sint32 remainder;
    sint32 correction;
    FUNCTION_MARKER(0x8002E258, "SLUS_005.43");
    root = (sint32)SquareRoot0(value);
    remainder = (sint32)((uint32)value - (uint32)root * (uint32)root);
    if (!root)
        trap(7);
    if (root == -1 && remainder == (sint32)0x80000000u)
        trap(0x5D);
    correction = remainder / root;
    return (sint32)((uint32)root + (uint32)(correction >> 1));
}

sint32 random_signed_between(sint32 first, sint32 second)
{
    sint32 choice;
    FUNCTION_MARKER(0x8002E390, "SLUS_005.43");
    choice = random_between(0, 1);
    if (choice)
        return random_between((sint32)(0u - (uint32)second), (sint32)(0u - (uint32)first));
    return random_between(first, second);
}

void font_emit_packets(uint32 ordering_table, const char *text, uint32 position, uint32 palette)
{
    uint32 color;
    uint32 index;
    uint32 font;
    uint32 packet;
    uint32 glyph;
    uint32 shift;
    uint32 character;
    sint32 mapping;
    sint32 format;
    sint32 width;
    sint32 height;
    uint32 u;
    uint32 v;
    FUNCTION_MARKER(0x80030D68, "SLUS_005.43");
    color = 0x66000000u | r_u8(0x8004617C);
    color |= (uint32)r_u8(0x80033D38) << 8;
    color |= (uint32)r_u8(0x80053680) << 16;
    index = r_u32(0x80031410);
    font = r_u32(0x80033D2C);
    packet = 0x800429D8 + index * 40 + (r_u32(0x800311B8) & 1) * 20;
    for (;;)
    {
        character = (uint8)*text;
        format = r_s16(0x8004EB7C);
        if (!character)
            break;
        mapping = r_s16(0x8003FC68 + character * 2);
        w_u32(packet + 8, position);
        if (mapping < 0)
            position += 4;
        else
        {
            glyph = 0x8004EB70 + ((uint32)mapping + font) * 16;
            shift = (2u - (uint32)format) & 31;
            if (font == 84)
                ++position;
            position += (uint32)r_s16(glyph + 14);
            u = r_u16(glyph);
            v = r_u16(glyph + 2);
            w_u8(packet + 12, (uint8)((u & 0x3F) << shift));
            width = r_s16(glyph + 8);
            w_u8(packet + 13, (uint8)v);
            width = (sint32)((uint32)width << shift);
            height = r_s16(glyph + 10);
            w_u16(packet + 16, (uint16)width);
            w_u16(packet + 18, (uint16)height);
            w_u16(packet + 14, (uint16)palette);
            w_u32(packet + 4, color);
            AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 4));
            packet = (packet & 0xFFFFFF) + 40;
            ++index;
            if ((sint32)(index - 350) >= 0)
            {
                index = 0;
                packet = 0x800429D8 + (r_u32(0x800311B8) & 1) * 20;
            }
        }
        ++text;
    }
    w_u32(0x8003142C, position & 0xFFFF);
    w_u32(0x80031410, index);
}

uint32 font_draw(const char *text, uint32 x, uint32 y)
{
    uint32 font_record;
    uint32 initial;
    uint32 packet;
    uint32 owner;
    uint32 result;
    uint32 position;
    uint16 palette;
    uint16 page;
    FUNCTION_MARKER(0x8002E5D8, "SLUS_005.43");
    font_record = 0x8004EB70 + r_u32(0x80033D2C) * 16;
    initial = r_u32(0x80031410);
    position = (y << 16) + x;
    packet = 0x800429D8 + initial * 40 + (r_u32(0x800311B8) & 1) * 20;
    palette = GetClut(r_s16(font_record + 4), r_s16(font_record + 6));
    owner = r_u32(0x8004608C);
    w_u16(packet + 14, palette);
    font_emit_packets(owner + 112, text, position, palette);
    result = r_u32(0x80031410);
    if ((uint32)(sint32)(sint16)initial != result)
    {
        page = GetTPage(r_s16(font_record + 12), 1, r_u16(font_record) & 0xFFC0, r_u16(font_record + 2) & 0xFF00);
        packet = r_u32(0x80040690);
        SetDrawMode((DR_MODE *)psx_addr(packet, sizeof(DR_MODE)), 0, 1, (sint16)page, NULL);
        owner = r_u32(0x8004608C);
        AddPrim(psx_addr(owner + 112, 4), psx_addr(packet, 4));
        result = r_u32(owner + 112);
        w_u32(0x80040690, packet + 12);
    }
    return result;
}

sint32 font_center_x(const char *text)
{
    uint32 width = 0;
    uint32 character;
    uint32 font;
    uint32 glyph;
    sint32 mapping;
    FUNCTION_MARKER(0x8002EE70, "SLUS_005.43");
    while ((character = (uint8)*text++) != 0)
    {
        mapping = r_s16(0x8003FC68 + character * 2);
        font = r_u32(0x80033D2C);
        glyph = mapping < 0 ? 0xFFFFFFFFu : (uint32)mapping + font;
        if (font == 84)
            ++width;
        width += (uint32)r_s16(0x8004EB7E + (glyph << 4));
    }
    return (sint32)((uint32)(r_s32(0x8003119C) >> 1) - (uint32)((sint32)width >> 1) - 14);
}

uint32 font_draw_current(const char *text)
{
    uint32 x;
    uint32 y;
    FUNCTION_MARKER(0x8002ED70, "SLUS_005.43");
    x = r_u32(0x80053688);
    y = r_u32(0x8005368C);
    return font_draw(text, x, y);
}

uint32 font_draw_centered(const char *text, uint32 y)
{
    uint32 x;
    FUNCTION_MARKER(0x8002EDA0, "SLUS_005.43");
    x = (uint32)font_center_x(text);
    w_u32(0x80053688, x);
    w_u32(0x8005368C, y);
    return font_draw(text, x, y);
}

uint32 font_draw_integer(sint32 value)
{
    char text[24];
    FUNCTION_MARKER(0x8002F064, "SLUS_005.43");
    if (value < 0)
        sprintf(text, "-%d", (sint32)(0u - (uint32)value));
    else
        sprintf(text, "%d", value);
    return font_draw_current(text);
}

uint32 font_draw_outline(const char *text, uint32 x, uint32 y, uint32 radius)
{
    uint32 initial;
    uint32 character;
    uint32 glyph;
    uint32 shift;
    uint32 angle;
    uint32 packet;
    uint32 owner;
    uint32 draw_x;
    uint32 draw_y;
    uint32 index;
    uint32 result;
    uint32 base_x = 0;
    uint32 base_y = 0;
    sint32 format = 0;
    sint32 mapping;
    uint16 palette;
    uint16 page;
    FUNCTION_MARKER(0x8002E718, "SLUS_005.43");
    initial = r_u16(0x80031410);
    while ((character = (uint8)*text) != 0)
    {
        mapping = r_s16(0x8003FC68 + character * 2);
        if (mapping < 0)
            mapping = -1;
        else
            mapping = (sint32)((uint32)mapping + r_u32(0x80033D2C));
        if (mapping < 0)
            x += 4;
        else
        {
            glyph = 0x8004EB70 + ((uint32)mapping << 4);
            format = r_s16(glyph + 12);
            shift = (2u - (uint32)format) & 31;
            base_x = (uint32)(r_s16(glyph) >> 6) << 6;
            base_y = (uint32)(r_s16(glyph + 2) >> 8) << 8;
            for (angle = 0; angle < 4096; angle += 256)
            {
                draw_x = x + (uint32)((sint32)(radius * (uint32)rsin((sint32)angle)) >> 9);
                draw_y = (uint32)rcos((sint32)angle);
                packet = r_u32(0x80040690);
                draw_y = y + (uint32)((sint32)(radius * draw_y) >> 12);
                SetSprt((SPRT *)psx_addr(packet, sizeof(SPRT)));
                SetSemiTrans(psx_addr(packet, 8), 1);
                w_u16(packet + 8, (uint16)draw_x);
                w_u16(packet + 10, (uint16)draw_y);
                w_u8(packet + 12, (uint8)(((uint32)r_s16(glyph) - base_x) << shift));
                w_u8(packet + 13, (uint8)((uint32)r_u8(glyph + 2) - base_y));
                w_u16(packet + 16, (uint16)((uint32)r_s16(glyph + 8) << shift));
                w_u16(packet + 18, r_u16(glyph + 10));
                palette = GetClut(r_s16(glyph + 4), r_s16(glyph + 6));
                w_u8(packet + 4, r_u8(0x8004617C));
                w_u8(packet + 5, r_u8(0x80033D38));
                character = r_u8(0x80053680);
                w_u16(packet + 14, palette);
                w_u8(packet + 6, (uint8)character);
                owner = r_u32(0x8004608C);
                AddPrim(psx_addr(owner + 112, 4), psx_addr(packet, 4));
                w_u32(0x80040690, packet + 20);
            }
            if (r_u32(0x80033D2C) == 84)
                ++x;
            x += (uint32)r_s16(glyph + 14);
            index = r_u32(0x80031410) + 1;
            w_u32(0x80031410, index);
            if ((sint32)index >= 350)
                w_u32(0x80031410, 0);
        }
        ++text;
    }
    index = r_u32(0x80031410);
    w_u32(0x8003142C, x);
    result = (uint32)(sint32)(sint16)initial;
    if (result != index)
    {
        page = GetTPage(format, 1, (sint32)base_x, (sint32)base_y);
        packet = r_u32(0x80040690);
        SetDrawMode((DR_MODE *)psx_addr(packet, sizeof(DR_MODE)), 0, 1, (sint16)page, NULL);
        owner = r_u32(0x8004608C);
        AddPrim(psx_addr(owner + 112, 4), psx_addr(packet, 4));
        result = r_u32(owner + 112);
        w_u32(0x80040690, packet + 12);
    }
    return result;
}

uint32 font_draw_centered_outline(const char *text, uint32 y, sint32 radius)
{
    uint32 x;
    FUNCTION_MARKER(0x8002EDF4, "SLUS_005.43");
    x = (uint32)font_center_x(text);
    w_u32(0x80053688, x);
    w_u32(0x8005368C, y);
    if (radius > 0)
        return font_draw_outline(text, x, y, (uint32)radius);
    return font_draw(text, x, y);
}

static sint32 font_draw_justified_body(const char *text, uint32 layout, uint32 *native_layout)
{
    uint32 remaining;
    uint32 x;
    uint32 y;
    uint32 spaces;
    uint32 step;
    uint32 accumulated = 0;
    uint32 previous = 0;
    uint32 base_x = 0;
    uint32 base_y = 0;
    uint32 extra;
    uint32 character;
    uint32 glyph;
    uint32 shift;
    uint32 packet;
    uint32 owner;
    uint32 blue;
    sint32 format = 0;
    sint32 mapping;
    uint16 palette;
    uint16 page;
    remaining = (native_layout != NULL ? native_layout[5] : r_u32(layout + 20));
    x = (native_layout != NULL ? native_layout[0] : r_u32(layout));
    spaces = (native_layout != NULL ? native_layout[4] : r_u32(layout + 16));
    y = (native_layout != NULL ? native_layout[1] : r_u32(layout + 4));
    if ((sint32)spaces > 0)
        step = (uint32)((sint32)((native_layout != NULL ? native_layout[3] : r_u32(layout + 12)) << 16) / (sint32)spaces);
    else
        step = 0;
    while ((character = (uint8)*text) != 0)
    {
        if ((sint32)remaining <= 0)
            break;
        if (character != '[' && character != ']')
        {
            mapping = r_s16(0x8003FC68 + (uint32)(uint8)*text * 2);
            if (mapping < 0)
                mapping = -1;
            else
                mapping = (sint32)((uint32)mapping + r_u32(0x80033D2C));
            if (mapping >= 0)
            {
                glyph = 0x8004EB70 + ((uint32)mapping << 4);
                base_x = (uint32)(r_s16(glyph) >> 6) << 6;
                format = r_s16(glyph + 12);
                packet = r_u32(0x80040690);
                shift = (2u - (uint32)format) & 31;
                base_y = (uint32)(r_s16(glyph + 2) >> 8) << 8;
                SetSprt((SPRT *)psx_addr(packet, sizeof(SPRT)));
                w_u16(packet + 8, (uint16)x);
                w_u16(packet + 10, (uint16)y);
                w_u8(packet + 12, (uint8)(((uint32)r_s16(glyph) - base_x) << shift));
                w_u8(packet + 13, (uint8)((uint32)r_u8(glyph + 2) - base_y));
                w_u16(packet + 16, (uint16)((uint32)r_s16(glyph + 8) << shift));
                w_u16(packet + 18, r_u16(glyph + 10));
                palette = GetClut(r_s16(glyph + 4), r_s16(glyph + 6));
                w_u8(packet + 4, r_u8(0x8004617C));
                w_u8(packet + 5, r_u8(0x80033D38));
                blue = r_u8(0x80053680);
                w_u16(packet + 14, palette);
                w_u8(packet + 6, (uint8)blue);
                SetSemiTrans(psx_addr(packet, 8), 0);
                owner = r_u32(0x8004608C);
                AddPrim(psx_addr(owner + 112, 4), psx_addr(packet, 4));
                x += (uint32)r_s16(glyph + 14);
                w_u32(0x80040690, r_u32(0x80040690) + 20);
            }
            else if (mapping == -1)
            {
                --spaces;
                if (spaces)
                {
                    accumulated += step;
                    x += 4 + (uint32)((sint32)accumulated >> 16) - (uint32)((sint32)previous >> 16);
                    previous = accumulated;
                }
                else
                {
                    extra = (native_layout != NULL ? native_layout[3] : r_u32(layout + 12));
                    x += 4 + extra - (uint32)((sint32)accumulated >> 16);
                    accumulated = extra << 16;
                }
            }
            else
                x += 4;
            --remaining;
        }
        ++text;
    }
    if (native_layout != NULL)
        native_layout[0] = x;
    else
        w_u32(layout, x);
    w_u32(0x8003142C, x);
    page = GetTPage(format, 1, (sint32)base_x, (sint32)base_y);
    packet = r_u32(0x80040690);
    SetDrawMode((DR_MODE *)psx_addr(packet, sizeof(DR_MODE)), 0, 1, (sint16)page, NULL);
    owner = r_u32(0x8004608C);
    AddPrim(psx_addr(owner + 112, 4), psx_addr(packet, 4));
    w_u32(0x80040690, packet + 12);
    return (sint32)remaining;
}

sint32 font_draw_justified(const char *text, uint32 layout)
{
    FUNCTION_MARKER(0x8002EA2C, "SLUS_005.43");
    return font_draw_justified_body(text, layout, NULL);
}

sint32 font_draw_justified_native(const char *text, uint32 layout[6])
{
    return font_draw_justified_body(text, 0, layout);
}

void mesh_project_vertices(uint32 vertices, uint32 screen, uint32 depth, sint32 count)
{
    uint32 index;
    uint32 first;
    uint32 second;
    uint32 third;
    FUNCTION_MARKER(0x8002F180, "SLUS_005.43");
    do
    {
        for (index = 0; index < 6; ++index)
            xport_gte_write_data(index, r_u32(vertices + index * 4));
        xport_gte_execute(0x280030);
        vertices += 24;
        screen += 12;
        depth += 6;
        count = (sint32)((uint32)count - 3);
        w_u32(screen - 12, xport_gte_read_data(12));
        w_u32(screen - 8, xport_gte_read_data(13));
        w_u32(screen - 4, xport_gte_read_data(14));
        first = xport_gte_read_data(17);
        second = xport_gte_read_data(18);
        third = xport_gte_read_data(19);
        w_u16(depth - 6, (uint16)first);
        w_u16(depth - 4, (uint16)second);
        w_u16(depth - 2, (uint16)third);
    }
    while (count > 0);
}

sint32 render_test_textured_triangle(uint32 output, uint32 depths, uint32 screen, uint32 face, uint32 short_layout)
{
    uint32 first_index;
    uint32 second_index;
    uint32 third_index;
    uint32 backface;
    uint32 result;
    sint32 near_plane;
    sint32 first_depth;
    sint32 second_depth;
    sint32 third_depth;
    sint32 visible;
    sint32 limit;
    sint32 winding;
    if (short_layout)
    {
        FUNCTION_MARKER(0x80030120, "SLUS_005.43");
        first_index = r_u16(face + 10);
        second_index = r_u16(face + 14);
        third_index = r_u16(face + 18);
    }
    else
    {
        FUNCTION_MARKER(0x80030110, "SLUS_005.43");
        first_index = r_u16(face + 18);
        second_index = r_u16(face + 22);
        third_index = r_u16(face + 26);
    }
    near_plane = r_s32(0x800313E0);
    first_depth = r_s16(depths + first_index * 2);
    near_plane >>= 1;
    second_depth = r_s16(depths + second_index * 2);
    third_depth = r_s16(depths + third_index * 2);
    visible = (near_plane < first_depth) + (near_plane < second_depth) + (near_plane < third_depth);
    if (!visible)
        return 0;
    xport_gte_write_data(12, r_u32(screen + first_index * 4));
    xport_gte_write_data(13, r_u32(screen + second_index * 4));
    xport_gte_write_data(14, r_u32(screen + third_index * 4));
    backface = r_u32(0x8004EB18);
    xport_gte_execute(0x1400006);
    if (backface)
    {
        xport_gte_read_data(24);
        winding = (sint32)xport_gte_read_data(24);
        if (winding <= 0 || visible != 3)
            return 0;
    }
    xport_gte_write_data(17, (uint32)first_depth);
    xport_gte_write_data(18, (uint32)second_depth);
    xport_gte_write_data(19, (uint32)third_depth);
    xport_gte_execute(0x158002D);
    xport_gte_read_data(7);
    result = xport_gte_read_data(7);
    limit = r_s32(0x800313E4);
    result <<= 2;
    w_u32(output, result);
    return (sint32)result < limit;
}

uint32 render_emit_gouraud_triangle(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena)
{
    uint32 index;
    uint32 normal;
    uint32 packet;
    uint32 tag;
    uint32 next;
    uint32 flags;
    uint32 color;
    FUNCTION_MARKER(0x80030244, "SLUS_005.43");
    xport_gte_write_data(6, r_u32(face + 4));
    for (index = 0; index < 3; ++index)
    {
        normal = normals + r_u16(face + 8 + index * 4) * 8;
        xport_gte_write_data(index * 2, r_u32(normal));
        xport_gte_write_data(index * 2 + 1, r_u32(normal + 4));
    }
    packet = r_u32(arena);
    tag = r_u32(ordering_table);
    xport_gte_execute(0x118043F);
    w_u32(packet, (tag & 0xFFFFFF) | 0x06000000);
    w_u32(ordering_table, packet & 0xFFFFFF);
    next = packet + 28;
    w_u32(arena, next);
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 16, xport_gte_read_data(13));
    w_u32(packet + 24, xport_gte_read_data(14));
    flags = r_u32(0x80052838);
    color = xport_gte_read_data(20);
    w_u32(packet + 4, color | flags);
    w_u32(packet + 12, xport_gte_read_data(21));
    w_u32(packet + 20, xport_gte_read_data(22));
    return next;
}

sint32 string_table_find(const char *text, uint32 table)
{
    uint32 index = 0;
    uint32 candidate;
    uint32 length;
    FUNCTION_MARKER(0x80030578, "SLUS_005.43");
    if (!r_u32(table))
        return -1;
    for (;;)
    {
        candidate = r_u32(table);
        length = 0;
        while (r_u8(candidate + length))
            ++length;
        if (!strcmp(text, (const char *)psx_addr(candidate, (size_t)length + 1)))
            return (sint32)index;
        table += 4;
        candidate = r_u32(table);
        ++index;
        if (!candidate)
            return -1;
    }
}

sint32 string_to_integer(const char *text)
{
    FUNCTION_MARKER(0x800305F0, "SLUS_005.43");
    return atoi(text);
}

sint32 video_clear_vram(uint32 left, uint32 right)
{
    PSX_RECT rect;
    FUNCTION_MARKER(0x80030C64, "SLUS_005.43");
    if (left)
    {
        rect.x = 0;
        rect.y = 0;
        rect.w = 512;
        rect.h = 256;
        ClearImage(&rect, 0, 0, 0);
        DrawSync(0);
        rect.x = 0;
        rect.y = 256;
        rect.w = 512;
        rect.h = 256;
        ClearImage(&rect, 0, 0, 0);
        DrawSync(0);
    }
    if (right)
    {
        rect.x = 512;
        rect.y = 0;
        rect.w = 512;
        rect.h = 256;
        ClearImage(&rect, 0, 0, 0);
        DrawSync(0);
        rect.x = 512;
        rect.y = 256;
        rect.w = 512;
        rect.h = 256;
        ClearImage(&rect, 0, 0, 0);
        DrawSync(0);
    }
    SetDispMask(1);
    return 3;
}

sint32 system_enter_critical(void)
{
    FUNCTION_MARKER(0x8005A280, "SLUS_005.43");
    return EnterCriticalSectionPSX();
}

void system_exit_critical(void)
{
    FUNCTION_MARKER(0x8005A290, "SLUS_005.43");
    ExitCriticalSection();
}

uint32 system_pad_init(uint32 first, uint32 first_size, uint32 second, uint32 second_size)
{
    FUNCTION_MARKER(0x8005A638, "SLUS_005.43");
    return InitPAD(first, first_size, second, second_size);
}

uint32 system_pad_start(void)
{
    FUNCTION_MARKER(0x8005A648, "SLUS_005.43");
    return StartPAD();
}

uint32 system_pad_stop(void)
{
    FUNCTION_MARKER(0x8005A658, "SLUS_005.43");
    return xport_bios_stop_pad();
}

uint32 system_pad_init_legacy(uint32 mode, uint32 destination)
{
    FUNCTION_MARKER(0x8005A668, "SLUS_005.43");
    return PAD_init2(mode, destination);
}

uint32 system_card_init(uint32 mode)
{
    FUNCTION_MARKER(0x8005B5C8, "SLUS_005.43");
    return xport_bios_init_card(mode);
}

uint32 system_card_start_bios(void)
{
    FUNCTION_MARKER(0x8005B5D8, "SLUS_005.43");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 system_card_stop(void)
{
    FUNCTION_MARKER(0x8005B5E8, "SLUS_005.43");
    return xport_bios_stop_card();
}

void system_run_initializers(void)
{
    FUNCTION_MARKER(0x8005B960, "SLUS_005.43");
    /* The original initializer count is zero */
}

sint32 system_cd_mix(uint32 volume)
{
    FUNCTION_MARKER(0x8005C7E4, "SLUS_005.43");
    return CdMix((CdlATV *)psx_addr(volume, sizeof(CdlATV)));
}

sint32 system_cd_get_sector(void *destination, sint32 words)
{
    FUNCTION_MARKER(0x8005C804, "SLUS_005.43");
    return CdGetSector(destination, words);
}

sint32 system_cd_position_to_sector(CdlLOC *position)
{
    FUNCTION_MARKER(0x8005C96C, "SLUS_005.43");
    return CdPosToInt(position);
}

sint32 system_stream_clear_disabled(void)
{
    FUNCTION_MARKER(0x8005F5F8, "SLUS_005.43");
    /* The original entry bypasses the ring update */
    return 1;
}

sint32 system_read_counter_one(void)
{
    FUNCTION_MARKER(0x80062C30, "SLUS_005.43");
    return GetRCnt(0xF2000001);
}

sint32 system_reset_counter_one(void)
{
    FUNCTION_MARKER(0x80062C54, "SLUS_005.43");
    return ResetRCnt(0xF2000001);
}

void system_set_projection_offset(sint32 x, sint32 y)
{
    FUNCTION_MARKER(0x80063BE0, "SLUS_005.43");
    SetGeomOffset(x, y);
}

void system_set_projection_distance(sint32 distance)
{
    FUNCTION_MARKER(0x80063C00, "SLUS_005.43");
    SetGeomScreen(distance);
}

void system_spu_init(void)
{
    FUNCTION_MARKER(0x8006F398, "SLUS_005.43");
    SpuInit();
}

void system_spu_quit(void)
{
    FUNCTION_MARKER(0x800700FC, "SLUS_005.43");
    SpuQuit();
}

uint32 system_spu_write(const void *source, uint32 bytes)
{
    FUNCTION_MARKER(0x80070AA8, "SLUS_005.43");
    if (bytes > 0x7EFF0)
        bytes = 0x7EFF0;
    return SpuWrite(source, bytes);
}

sint32 system_set_video_mode(sint32 mode)
{
    FUNCTION_MARKER(0x80073374, "SLUS_005.43");
    return SetVideoMode(mode);
}

sint32 system_get_video_mode(void)
{
    FUNCTION_MARKER(0x8007338C, "SLUS_005.43");
    return GetVideoMode();
}

uint32 render_emit_textured_gouraud_triangle(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 shade_flags, uint32 primitive_code)
{
    uint32 first_index;
    uint32 second_index;
    uint32 third_index;
    uint32 normal;
    uint32 packet;
    uint32 tag;
    uint32 uv_offset;
    uint32 first_uv;
    uint32 second_uv;
    uint32 third_uv;
    FUNCTION_MARKER(0x8002FE10, "SLUS_005.43");
    xport_gte_write_data(6, shade_flags | primitive_code);
    first_index = r_u16(face + 16);
    second_index = r_u16(face + 20);
    third_index = r_u16(face + 24);
    normal = normals + first_index * 8;
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    normal = normals + second_index * 8;
    xport_gte_write_data(2, r_u32(normal));
    xport_gte_write_data(3, r_u32(normal + 4));
    normal = normals + third_index * 8;
    xport_gte_write_data(4, r_u32(normal));
    xport_gte_write_data(5, r_u32(normal + 4));
    packet = r_u32(arena);
    tag = r_u32(ordering_table);
    xport_gte_execute(0x118043F);
    w_u32(packet, (tag & 0xFFFFFF) | 0x09000000);
    w_u32(ordering_table, packet & 0xFFFFFF);
    w_u32(arena, packet + 40);
    uv_offset = r_u32(0x800313EC);
    first_uv = r_u32(face + 4);
    second_uv = r_u32(face + 8);
    third_uv = r_u32(face + 12);
    w_u32(packet + 12, first_uv + uv_offset);
    w_u32(packet + 24, second_uv);
    w_u32(packet + 36, third_uv);
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 20, xport_gte_read_data(13));
    w_u32(packet + 32, xport_gte_read_data(14));
    w_u32(packet + 4, xport_gte_read_data(20));
    w_u32(packet + 16, xport_gte_read_data(21));
    w_u32(packet + 28, xport_gte_read_data(22));
    return uv_offset;
}

sint32 render_test_textured_quad(uint32 output, uint32 depths, uint32 screen, uint32 face, uint32 *fourth_xy, uint32 short_layout)
{
    uint32 first_index;
    uint32 second_index;
    uint32 third_index;
    uint32 fourth_index;
    uint32 backface;
    uint32 average;
    sint32 near_plane;
    sint32 first_depth;
    sint32 second_depth;
    sint32 third_depth;
    sint32 fourth_depth;
    sint32 visible;
    sint32 limit;
    sint32 winding;
    if (short_layout)
    {
        FUNCTION_MARKER(0x8002FFE0, "SLUS_005.43");
        first_index = r_u16(face + 10);
        second_index = r_u16(face + 14);
        third_index = r_u16(face + 18);
        fourth_index = r_u16(face + 22);
    }
    else
    {
        FUNCTION_MARKER(0x8002FFCC, "SLUS_005.43");
        first_index = r_u16(face + 22);
        second_index = r_u16(face + 26);
        third_index = r_u16(face + 30);
        fourth_index = r_u16(face + 34);
    }
    near_plane = r_s32(0x800313E0);
    first_depth = r_s16(depths + first_index * 2);
    near_plane >>= 1;
    second_depth = r_s16(depths + second_index * 2);
    third_depth = r_s16(depths + third_index * 2);
    fourth_depth = r_s16(depths + fourth_index * 2);
    near_plane = (sint32)((uint32)near_plane >> 1);
    visible = (near_plane < first_depth) + (near_plane < second_depth) + (near_plane < third_depth) + (near_plane < fourth_depth);
    if (!visible)
        return 0;
    xport_gte_write_data(12, r_u32(screen + first_index * 4));
    xport_gte_write_data(13, r_u32(screen + second_index * 4));
    xport_gte_write_data(14, r_u32(screen + third_index * 4));
    backface = r_u32(0x8004EB18);
    xport_gte_execute(0x1400006);
    if (backface)
    {
        xport_gte_read_data(24);
        winding = (sint32)xport_gte_read_data(24);
        if (winding <= 0 || visible != 4)
            return 0;
    }
    *fourth_xy = r_u32(screen + fourth_index * 4);
    average = ((uint32)first_depth + (uint32)second_depth + (uint32)third_depth + (uint32)fourth_depth) >> 2;
    limit = r_s32(0x800313E4);
    w_u32(output, average);
    return (sint32)average < limit;
}

uint32 render_emit_textured_gouraud_quad(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 shade_flags, uint32 primitive_code, uint32 fourth_xy)
{
    uint32 first_index;
    uint32 second_index;
    uint32 third_index;
    uint32 normal;
    uint32 packet;
    uint32 tag;
    uint32 uv_offset;
    uint32 first_uv;
    uint32 second_uv;
    uint32 third_uv;
    uint32 fourth_uv;
    FUNCTION_MARKER(0x8002FED4, "SLUS_005.43");
    xport_gte_write_data(6, shade_flags | primitive_code);
    first_index = r_u16(face + 20);
    second_index = r_u16(face + 24);
    third_index = r_u16(face + 28);
    normal = normals + first_index * 8;
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    normal = normals + second_index * 8;
    xport_gte_write_data(2, r_u32(normal));
    xport_gte_write_data(3, r_u32(normal + 4));
    normal = normals + third_index * 8;
    xport_gte_write_data(4, r_u32(normal));
    xport_gte_write_data(5, r_u32(normal + 4));
    packet = r_u32(arena);
    tag = r_u32(ordering_table);
    xport_gte_execute(0x118043F);
    w_u32(packet, (tag & 0xFFFFFF) | 0x0C000000);
    w_u32(ordering_table, packet & 0xFFFFFF);
    w_u32(arena, packet + 52);
    w_u32(packet + 4, xport_gte_read_data(20));
    w_u32(packet + 16, xport_gte_read_data(21));
    w_u32(packet + 28, xport_gte_read_data(22));
    normal = normals + r_u16(face + 32) * 8;
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    xport_gte_execute(0x108041B);
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 20, xport_gte_read_data(13));
    w_u32(packet + 32, xport_gte_read_data(14));
    w_u32(packet + 44, fourth_xy);
    uv_offset = r_u32(0x800313EC);
    first_uv = r_u32(face + 4);
    second_uv = r_u32(face + 8);
    third_uv = r_u32(face + 12);
    fourth_uv = r_u32(face + 16);
    w_u32(packet + 12, first_uv + uv_offset);
    w_u32(packet + 24, second_uv);
    w_u32(packet + 36, third_uv);
    w_u32(packet + 48, fourth_uv);
    w_u32(packet + 40, xport_gte_read_data(22));
    return uv_offset;
}

uint32 render_emit_gouraud_quad(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 fourth_xy)
{
    uint32 packet;
    uint32 tag;
    uint32 next;
    uint32 flags;
    uint32 index;
    uint32 normal;
    uint32 color;
    FUNCTION_MARKER(0x80030300, "SLUS_005.43");
    packet = r_u32(arena);
    tag = r_u32(ordering_table);
    w_u32(packet, (tag & 0xFFFFFF) | 0x08000000);
    w_u32(ordering_table, packet & 0xFFFFFF);
    next = packet + 36;
    w_u32(arena, next);
    flags = r_u32(0x80052838);
    for (index = 0; index < 4; ++index)
    {
        normal = normals + r_u16(face + 8 + index * 4) * 8;
        xport_gte_write_data(6, r_u32(face + 4));
        xport_gte_write_data(0, r_u32(normal));
        xport_gte_write_data(1, r_u32(normal + 4));
        xport_gte_execute(0x108041B);
        xport_gte_read_data(22);
        color = xport_gte_read_data(22);
        if (!index)
            color |= flags;
        w_u32(packet + 4 + index * 8, color);
    }
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 16, xport_gte_read_data(13));
    w_u32(packet + 24, xport_gte_read_data(14));
    w_u32(packet + 32, fourth_xy);
    return next;
}

sint32 render_test_flat_quad(uint32 output, uint32 depths, uint32 screen, uint32 face, uint32 *fourth_xy, uint32 short_layout)
{
    uint32 packed_first;
    uint32 packed_second;
    uint32 fourth_index;
    uint32 first_index;
    uint32 second_index;
    uint32 third_index;
    uint32 backface;
    uint32 average;
    sint32 first_depth;
    sint32 second_depth;
    sint32 third_depth;
    sint32 fourth_depth;
    sint32 near_plane;
    sint32 visible;
    sint32 winding;
    sint32 limit;
    if (short_layout)
    {
        FUNCTION_MARKER(0x8002F43C, "SLUS_005.43");
        packed_first = r_u32(face + 8);
        packed_second = r_u32(face + 12);
        fourth_index = (uint32)r_s16(face + 16);
    }
    else
    {
        FUNCTION_MARKER(0x8002F42C, "SLUS_005.43");
        packed_first = r_u32(face + 20);
        packed_second = r_u32(face + 24);
        fourth_index = (uint32)r_s16(face + 28);
    }
    first_index = packed_first >> 16;
    second_index = packed_second & 0xFFFF;
    third_index = packed_second >> 16;
    near_plane = r_s32(0x800313E0);
    first_depth = r_s16(depths + first_index * 2);
    near_plane >>= 1;
    second_depth = r_s16(depths + second_index * 2);
    third_depth = r_s16(depths + third_index * 2);
    fourth_depth = r_s16(depths + fourth_index * 2);
    near_plane = (sint32)((uint32)near_plane >> 1);
    visible = (near_plane < first_depth) + (near_plane < second_depth) + (near_plane < third_depth) + (near_plane < fourth_depth);
    if (!visible)
        return 0;
    xport_gte_write_data(12, r_u32(screen + first_index * 4));
    xport_gte_write_data(13, r_u32(screen + second_index * 4));
    xport_gte_write_data(14, r_u32(screen + third_index * 4));
    backface = r_u32(0x8004EB18);
    xport_gte_execute(0x1400006);
    if (backface)
    {
        xport_gte_read_data(24);
        winding = (sint32)xport_gte_read_data(24);
        if (winding <= 0 || visible != 4)
            return 0;
    }
    *fourth_xy = r_u32(screen + fourth_index * 4);
    average = ((uint32)first_depth + (uint32)second_depth + (uint32)third_depth + (uint32)fourth_depth) >> 2;
    limit = r_s32(0x800313E4);
    w_u32(output, average);
    return (sint32)average < limit;
}
