#include "game_functions.h"
#include "psx_gpu.h"
#include "psx_stream.h"

#include <string.h>

sint32 render_test_flat_triangle(uint32 output, uint32 depths, uint32 screen, uint32 face, uint32 short_layout)
{
    uint32 packed_first;
    uint32 packed_second;
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
    sint32 winding;
    sint32 limit;
    if (short_layout)
    {
        FUNCTION_MARKER(0x8002F580, "SLUS_005.43");
        packed_first = r_u32(face + 8);
        packed_second = r_u32(face + 12);
    }
    else
    {
        FUNCTION_MARKER(0x8002F574, "SLUS_005.43");
        packed_first = r_u32(face + 16);
        packed_second = r_u32(face + 20);
    }
    first_index = packed_first >> 16;
    second_index = packed_second & 0xFFFF;
    third_index = packed_second >> 16;
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

uint32 render_emit_textured_flat_triangle(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 shade_flags, uint32 primitive_code)
{
    uint32 normal;
    uint32 packet;
    uint32 tag;
    uint32 uv_offset;
    uint32 first_uv;
    uint32 second_uv;
    uint32 third_uv;
    FUNCTION_MARKER(0x8002F2F0, "SLUS_005.43");
    xport_gte_write_data(6, shade_flags | primitive_code);
    normal = normals + r_u16(face + 16) * 8;
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    packet = r_u32(arena);
    tag = r_u32(ordering_table);
    xport_gte_execute(0x108041B);
    w_u32(packet, (tag & 0xFFFFFF) | 0x07000000);
    w_u32(ordering_table, packet & 0xFFFFFF);
    w_u32(arena, packet + 32);
    uv_offset = r_u32(0x800313EC);
    first_uv = r_u32(face + 4);
    second_uv = r_u32(face + 8);
    third_uv = r_u32(face + 12);
    w_u32(packet + 12, first_uv + uv_offset);
    w_u32(packet + 20, second_uv);
    w_u32(packet + 28, third_uv);
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 16, xport_gte_read_data(13));
    w_u32(packet + 24, xport_gte_read_data(14));
    w_u32(packet + 4, xport_gte_read_data(22));
    return uv_offset;
}

uint32 render_emit_textured_flat_quad(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 shade_flags, uint32 primitive_code, uint32 fourth_xy)
{
    uint32 normal;
    uint32 packet;
    uint32 tag;
    uint32 uv_offset;
    uint32 first_uv;
    uint32 second_uv;
    uint32 third_uv;
    uint32 fourth_uv;
    FUNCTION_MARKER(0x8002F388, "SLUS_005.43");
    xport_gte_write_data(6, shade_flags | primitive_code);
    normal = normals + r_u16(face + 20) * 8;
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    packet = r_u32(arena);
    tag = r_u32(ordering_table);
    xport_gte_execute(0x108041B);
    w_u32(packet, (tag & 0xFFFFFF) | 0x09000000);
    w_u32(ordering_table, packet & 0xFFFFFF);
    w_u32(arena, packet + 40);
    uv_offset = r_u32(0x800313EC);
    first_uv = r_u32(face + 4);
    second_uv = r_u32(face + 8);
    third_uv = r_u32(face + 12);
    fourth_uv = r_u32(face + 16);
    w_u32(packet + 12, first_uv + uv_offset);
    w_u32(packet + 20, second_uv);
    w_u32(packet + 28, third_uv);
    w_u32(packet + 36, fourth_uv);
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 16, xport_gte_read_data(13));
    w_u32(packet + 24, xport_gte_read_data(14));
    w_u32(packet + 32, fourth_xy);
    w_u32(packet + 4, xport_gte_read_data(22));
    return uv_offset;
}

uint32 render_emit_flat_triangle(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena)
{
    uint32 normal;
    uint32 packet;
    uint32 tag;
    uint32 next;
    uint32 flags;
    uint32 color;
    FUNCTION_MARKER(0x8002F6AC, "SLUS_005.43");
    normal = normals + (r_u32(face + 8) & 0xFFFF) * 8;
    xport_gte_write_data(6, r_u32(face + 4));
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    packet = r_u32(arena);
    tag = r_u32(ordering_table);
    xport_gte_execute(0x108041B);
    w_u32(packet, (tag & 0xFFFFFF) | 0x04000000);
    w_u32(ordering_table, packet & 0xFFFFFF);
    next = packet + 20;
    flags = r_u32(0x80052838);
    xport_gte_read_data(22);
    color = xport_gte_read_data(22);
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 12, xport_gte_read_data(13));
    w_u32(packet + 16, xport_gte_read_data(14));
    w_u32(packet + 4, color | flags);
    w_u32(arena, next);
    return next;
}

uint32 render_emit_flat_quad(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 fourth_xy)
{
    uint32 normal;
    uint32 packet;
    uint32 tag;
    uint32 next;
    uint32 color;
    FUNCTION_MARKER(0x8002F738, "SLUS_005.43");
    normal = normals + (r_u32(face + 8) & 0xFFFF) * 8;
    xport_gte_write_data(6, r_u32(face + 4));
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    packet = r_u32(arena);
    tag = r_u32(ordering_table);
    xport_gte_execute(0x108041B);
    w_u32(packet, (tag & 0xFFFFFF) | 0x05000000);
    w_u32(ordering_table, packet & 0xFFFFFF);
    next = packet + 24;
    (void)r_u32(0x80052838);
    xport_gte_read_data(22);
    color = xport_gte_read_data(22);
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 12, xport_gte_read_data(13));
    w_u32(packet + 16, xport_gte_read_data(14));
    w_u32(packet + 20, fourth_xy);
    w_u32(packet + 4, color);
    w_u32(arena, next);
    return next;
}

void scene_clear_transition(void)
{
    DISPENV previous_display;
    DISPENV display;
    DRAWENV previous_draw;
    DRAWENV draw;
    PSX_RECT clear;
    sint32 height;
    FUNCTION_MARKER(0x8001FAD4, "SLUS_005.43");
    gpu_get_display_env(&previous_display);
    GetDrawEnv(&previous_draw);
    height = r_u32(0x800311C0) ? 256 : 240;
    SetDefDispEnv(&display, 0, 0, 320, height);
    SetDefDrawEnv(&draw, 0, 0, 320, height);
    display.pad0 = r_u8(0x800311C0);
    display.isinter = 0;
    display.screen.x = (sint16)r_u16(0x80010564);
    display.screen.y = (sint16)r_u16(0x80010568);
    draw.isbg = 1;
    draw.dfe = 0;
    draw.dtd = 0;
    draw.r0 = 0;
    draw.g0 = 0;
    draw.b0 = 0;
    DrawSync(0);
    VSync(0);
    SetDispMask(0);
    PutDispEnv(&display);
    PutDrawEnv(&draw);
    clear.x = 0;
    clear.y = 0;
    clear.w = previous_display.disp.w;
    clear.h = display.disp.h;
    ClearImage(&clear, 0, 0, 0);
    DrawSync(0);
    clear.y = 256;
    ClearImage(&clear, 0, 0, 0);
    DrawSync(0);
    disc_draw_notice(0x800328AC);
    DrawSync(0);
    VSync(0);
    SetDispMask(1);
}

void system_card_start(void)
{
    FUNCTION_MARKER(0x8005B560, "SLUS_005.43");
    /* Memory card disabled pending xport_save/load */
}

sint32 sound_set_voice_left(uint32 voice, uint32 level)
{
    uint32 enabled;
    FUNCTION_MARKER(0x800259D8, "SLUS_005.43");
    enabled = r_u8(0x80031258);
    FUNCTION_MARKER(0x800259E0, "SLUS_005.43");
    if (!enabled || voice == 0xFFFFFFFF)
        return 0;
    if (voice >= 24)
        return (sint32)(voice << 3);
    if (!(r_u32(0x80033994 + voice * 28) & 2))
        return 0;
    w_u8(0x800339A7 + voice * 28, (uint8)level);
    return sound_update_voice(voice);
}

sint32 sound_set_voice_right(uint32 voice, uint32 level)
{
    uint32 enabled;
    FUNCTION_MARKER(0x80025A50, "SLUS_005.43");
    enabled = r_u8(0x80031258);
    FUNCTION_MARKER(0x80025A58, "SLUS_005.43");
    if (!enabled || voice == 0xFFFFFFFF)
        return 0;
    if (voice >= 24)
        return (sint32)(voice << 3);
    if (!(r_u32(0x80033994 + voice * 28) & 2))
        return 0;
    w_u8(0x800339A8 + voice * 28, (uint8)level);
    return sound_update_voice(voice);
}

sint32 sound_select_mode(uint32 mode)
{
    sint32 result;
    FUNCTION_MARKER(0x80024C14, "SLUS_005.43");
    result = r_u8(0x80031258);
    FUNCTION_MARKER(0x80024C1C, "SLUS_005.43");
    if (result)
    {
        result = sound_set_mode((sint32)(mode & 255));
        w_u8(0x80033C30, (uint8)mode);
    }
    return result;
}

uint32 texture_place_info(TEX_INFO *info, uint32 mode)
{
    uint32 reset;
    uint32 cursor;
    uint32 result;
    uint32 packed;
    sint32 row;
    sint32 x;
    sint32 y;
    FUNCTION_MARKER(0x80030610, "SLUS_005.43");
    reset = r_u32(0x80031454);
    FUNCTION_MARKER(0x80030618, "SLUS_005.43");
    if (reset)
        tex_reset_regions();
    if (mode == 0 || mode == 2)
    {
        cursor = 0x8004294E;
        for (row = 31; row >= 0; --row, cursor -= 2)
        {
            x = r_s16(cursor);
            if (x + (uint16)info->clut.w < 1024)
            {
                info->clut.x = (sint16)x;
                info->clut.y = (sint16)(row + 480);
                w_u16(cursor, (uint16)(r_u16(cursor) + (uint16)info->clut.w));
                break;
            }
        }
        if (row == -1)
            diagnostic_report(3, 0x80011F08, 0x80011F28);
    }
    result = mode - 1 < 2;
    if (result)
    {
        packed = tex_alloc_region((uint16)info->pixel.w, (uint16)info->pixel.h);
        w_u32(0x80031458, packed);
        if (packed == 0xFFFFFFFF)
            diagnostic_report(4, 0x80011F2C, 0x80011F28);
        packed = r_u32(0x80031458);
        y = (sint32)packed >> 16;
        result = packed & 0xFFFF;
        w_u32(0x8003145C, (uint32)y);
        w_u32(0x80031458, result);
        info->pixel.x = (sint16)result;
        info->pixel.y = (sint16)y;
    }
    return result;
}

uint32 camera_dispatch(void)
{
    uint32 mode;
    FUNCTION_MARKER(0x8002C6E4, "SLUS_005.43");
    mode = r_u32(0x800313DC);
    FUNCTION_MARKER(0x8002C6EC, "SLUS_005.43");
    switch (mode)
    {
    case 1:
        return camera_look_at_object_point(r_u32(0x80040C94));
    case 2:
        return camera_from_direction(r_u32(0x80040C94));
    case 3:
        return camera_apply_vertical_scale();
    case 4:
        (void)r_u32(0x80040C94);
        return camera_follow_target();
    case 5:
        (void)r_u32(0x80040C94);
        return camera_update_modes();
    default:
        return camera_from_transform(r_u32(0x80040C94));
    }
}

uint32 stream_request_position(uint32 position, uint32 sectors, uint32 asset)
{
    uint32 buffer;
    uint32 result;
    uint32 word;
    uint32 shift;
    uint32 mask;
    uint32 destination;
    FUNCTION_MARKER(0x800282F4, "SLUS_005.43");
    buffer = r_u32(0x80041000);
    FUNCTION_MARKER(0x800282FC, "SLUS_005.43");
    w_u32(buffer, 0);
    w_u32(r_u32(0x80041000) + 4, 0);
    buffer = r_u32(0x80041000);
    shift = ((position + 3) & 3) * 8;
    word = r_u32((position + 3) & 0xFFFFFFFC) << (24 - shift);
    shift = (position & 3) * 8;
    mask = 0xFFFFFFFF;
    mask >>= shift;
    word = (word & ~mask) | (r_u32(position & 0xFFFFFFFC) >> shift);
    destination = buffer + 8;
    shift = ((destination + 3) & 3) * 8;
    mask = 0xFFFFFFFF;
    mask >>= 24 - shift;
    w_u32((destination + 3) & 0xFFFFFFFC,
        (r_u32((destination + 3) & 0xFFFFFFFC) & ~mask) | (word >> (24 - shift)));
    shift = (destination & 3) * 8;
    mask = 0xFFFFFFFF;
    mask <<= shift;
    w_u32(destination & 0xFFFFFFFC,
        (r_u32(destination & 0xFFFFFFFC) & ~mask) | (word << shift));
    result = (uint32)CdPosToInt((CdlLOC *)psx_addr(position, sizeof(CdlLOC)));
    w_u32(r_u32(0x80041000) + 28, result);
    w_u32(r_u32(0x80041000) + 12, sectors);
    w_u32(r_u32(0x80041000) + 20, 0x800118FA);
    w_u32(r_u32(0x80041000) + 24, 75);
    w_u32(r_u32(0x80041000) + 32, asset);
    w_u32(r_u32(0x80041000) + 36, 0);
    w_u32(r_u32(0x80041000) + 40, 0);
    w_u8(r_u32(0x80041000) + 48, 0);
    w_u32(r_u32(0x80041000) + 44, 0);
    buffer = r_u32(0x80041000);
    w_u32(0x80041EE8, 0);
    w_u32(buffer, 3);
    result = r_u32(0x80040FF8);
    if (result)
    {
        result = r_u32(result);
        if (result)
            return result;
    }
    w_u32(0x80040FE4, 4);
    return 4;
}

sint32 stream_enqueue_segments(uint32 position, uint32 sectors, uint32 asset, uint32 segments, uint32 value, const char *text)
{
    uint32 initial;
    uint32 consumer;
    uint32 producer;
    uint32 record;
    uint32 word;
    uint32 shift;
    uint32 mask;
    uint32 destination;
    uint32 result;
    FUNCTION_MARKER(0x80027E2C, "SLUS_005.43");
    initial = r_u32(0x80041EE0);
    FUNCTION_MARKER(0x80027E34, "SLUS_005.43");
    consumer = r_u32(0x80041EDC);
    if (((initial + 1) & 7) == consumer)
        return 0;
    if (!r_s16(segments))
    {
        stream_enqueue(asset, value, text);
        return 1;
    }
    if (!r_u32(r_u32(0x80041004)) && !r_u32(r_u32(0x80041008)))
    {
        stream_request_segments(position, sectors, asset, segments, 20000, value, text);
        return 1;
    }
    producer = r_u32(0x80041EE0);
    record = 0x800414FC + producer * 316;
    w_u32(record + 4, 20000);
    w_u32(record, 0);
    shift = ((position + 3) & 3) * 8;
    word = r_u32((position + 3) & 0xFFFFFFFC) << (24 - shift);
    shift = (position & 3) * 8;
    mask = 0xFFFFFFFF;
    mask >>= shift;
    word = (word & ~mask) | (r_u32(position & 0xFFFFFFFC) >> shift);
    destination = record + 8;
    shift = ((destination + 3) & 3) * 8;
    mask = 0xFFFFFFFF;
    mask >>= 24 - shift;
    w_u32((destination + 3) & 0xFFFFFFFC,
        (r_u32((destination + 3) & 0xFFFFFFFC) & ~mask) | (word >> (24 - shift)));
    shift = (destination & 3) * 8;
    mask = 0xFFFFFFFF;
    mask <<= shift;
    w_u32(destination & 0xFFFFFFFC,
        (r_u32(destination & 0xFFFFFFFC) & ~mask) | (word << shift));
    result = (uint32)CdPosToInt((CdlLOC *)psx_addr(position, sizeof(CdlLOC)));
    w_u32(record + 28, result);
    w_u32(record + 12, sectors);
    w_u32(record + 20, segments);
    w_u32(record + 24, 75);
    w_u32(record + 32, asset);
    w_u32(record + 36, 0);
    w_u32(record + 40, 0);
    if (text)
        strcpy((char *)psx_addr(record + 48, strlen(text) + 1), text);
    else
        w_u8(record + 48, 0);
    w_u32(record + 44, value);
    w_u32(record, 2);
    result = r_u32(0x80040FF8);
    if (!result || !r_u32(result))
        w_u32(0x80040FE4, 4);
    w_u32(0x80041EE0, (producer + 1) & 7);
    return 1;
}

uint32 system_get_odd_field(void)
{
    FUNCTION_MARKER(0x8006B8EC, "SLUS_005.43");
    FUNCTION_MARKER(0x8006B8F4, "SLUS_005.43");
    return (uint32)GetODE();
}

uint32 render_ellipse_fan(uint32 x, uint32 y, uint32 radius, uint32 color, uint32 depth)
{
    uint32 angle = 0;
    uint32 first_x;
    uint32 first_y;
    uint32 second_x;
    uint32 second_y;
    uint32 packet;
    uint32 base;
    uint32 slot;
    uint32 bucket_depth;
    uint32 tag;
    uint32 previous;
    uint32 result;
    sint32 sine;
    sint32 cosine;
    FUNCTION_MARKER(0x8001EAAC, "SLUS_005.43");
    FUNCTION_MARKER(0x8001EAB0, "SLUS_005.43");
    do
    {
        sine = r_s16(0x8003DC68 + (angle & 0xFFF) * 2);
        cosine = r_s16(0x8003DC68 + ((angle + 1024) & 0xFFF) * 2);
        first_x = x + (uint32)((sint32)((uint32)sine * radius) >> 12);
        first_y = y + (uint32)((sint32)((uint32)cosine * radius) >> 13);
        angle += 256;
        sine = r_s16(0x8003DC68 + (angle & 0xFFF) * 2);
        cosine = r_s16(0x8003DC68 + ((angle + 1024) & 0xFFF) * 2);
        second_x = x + (uint32)((sint32)((uint32)sine * radius) >> 12);
        second_y = y + (uint32)((sint32)((uint32)cosine * radius) >> 13);
        packet = r_u32(0x80040690);
        w_u32(packet + 4, color);
        w_u32(packet + 12, 0);
        w_u32(packet + 20, 0);
        w_u8(packet + 7, 0x32);
        w_u8(packet + 3, 6);
        w_u16(packet + 8, (uint16)x);
        w_u16(packet + 10, (uint16)y);
        w_u16(packet + 16, (uint16)first_x);
        w_u16(packet + 18, (uint16)first_y);
        w_u16(packet + 24, (uint16)second_x);
        w_u16(packet + 26, (uint16)second_y);
        base = r_u32(0x800421AC);
        bucket_depth = depth;
        if ((sint32)bucket_depth < 0x4000)
        {
            if ((sint32)bucket_depth < 0)
                bucket_depth = 0;
            slot = base + ((bucket_depth >> 6) + 64) * 4;
        }
        else
        {
            if ((sint32)(bucket_depth - 0x3FFFFE) > 0)
                bucket_depth = 0x3FFFFE;
            slot = base + ((bucket_depth >> 11) + 320) * 4;
        }
        tag = r_u32(packet);
        previous = r_u32(slot);
        w_u32(packet, (tag & 0xFF000000) | (previous & 0xFFFFFF));
        previous = r_u32(slot);
        result = (previous & 0xFF000000) | (packet & 0xFFFFFF);
        w_u32(slot, result);
        w_u32(0x80040690, r_u32(0x80040690) + 28);
    } while ((sint32)(angle - 4096) < 0);
    return result;
}

uint32 render_set_full_height(uint32 width)
{
    uint32 active;
    uint32 standard;
    uint32 height;
    FUNCTION_MARKER(0x80022AB8, "SLUS_005.43");
    active = r_u32(0x800310A4);
    FUNCTION_MARKER(0x80022AC0, "SLUS_005.43");
    if (active)
        return active;
    standard = r_u32(0x800311C0);
    w_u32(0x8003119C, width);
    height = standard ? 256 : 240;
    w_u32(0x800311B0, height);
    width = r_u32(0x8003119C);
    height = r_u32(0x800311B0);
    return render_init_view(width, height, 200);
}

uint32 render_set_reduced_height(uint32 width)
{
    uint32 active;
    uint32 standard;
    uint32 height;
    FUNCTION_MARKER(0x80022B78, "SLUS_005.43");
    active = r_u32(0x800310A4);
    FUNCTION_MARKER(0x80022B80, "SLUS_005.43");
    if (active)
        return active;
    standard = r_u32(0x800311C0);
    w_u32(0x8003119C, width);
    height = standard ? 180 : 160;
    w_u32(0x800311B0, height);
    width = r_u32(0x8003119C);
    height = r_u32(0x800311B0);
    return render_init_view(width, height, 200);
}

uint32 light_scale_scene(uint32 scale)
{
    uint32 red;
    uint32 green;
    uint32 blue;
    uint32 light;
    FUNCTION_MARKER(0x80023288, "SLUS_005.43");
    red = r_u8(0x80042900);
    FUNCTION_MARKER(0x80023290, "SLUS_005.43");
    red *= scale;
    green = r_u8(0x80042901) * scale;
    blue = r_u8(0x80042902) * scale;
    SetBackColor((sint32)red >> 8, (sint32)green >> 8, (sint32)blue >> 8);
    for (light = 0; light < 3; ++light)
    {
        if (r_u32(0x800311D0 + light * 4))
        {
            red = r_u8(0x8003FE68 + light * 4) * scale;
            green = r_u8(0x8003FE69 + light * 4) * scale;
            blue = r_u8(0x8003FE6A + light * 4) * scale;
            light_set_scaled_color(light, (uint32)((sint32)red >> 8),
                (uint32)((sint32)green >> 8), (uint32)((sint32)blue >> 8));
        }
    }
    return 0;
}

uint32 object_extract_angles(uint32 destination, uint32 object)
{
    uint32 reference;
    uint32 pitch_offset;
    uint32 result;
    uint32 square;
    sint32 component;
    sint32 root;
    sint32 last;
    FUNCTION_MARKER(0x8002CFC4, "SLUS_005.43");
    reference = r_u32(0x80040FDC);
    FUNCTION_MARKER(0x8002CFCC, "SLUS_005.43");
    if (object == reference)
    {
        result = (uint32)ratan2(r_s16(object + 296), r_s16(object + 300));
        pitch_offset = 298;
    }
    else
    {
        result = (uint32)ratan2(r_s16(object + 288), r_s16(object + 300));
        pitch_offset = 294;
    }
    w_u32(destination + 32, result);
    w_u32(destination + 44, result);
    component = r_s16(object + pitch_offset);
    square = (uint32)component * (uint32)component;
    root = (sint32)(0xFFE001 - square);
    if (root < 0)
        root = 0;
    root = SquareRoot0(root);
    component = r_s16(object + pitch_offset);
    result = (uint32)ratan2((sint32)(0 - (uint32)component), root);
    w_u32(destination + 36, result);
    w_u32(destination + 48, result);
    last = r_s16(object + (object == reference ? 286 : 290));
    result = (uint32)ratan2(last, r_s16(object + 292));
    w_u32(destination + 40, result);
    w_u32(destination + 52, result);
    return result;
}

uint32 render_submit_frame(void)
{
    uint32 result;
    uint32 owner;
    uint32 packet;
    uint32 slot;
    uint16 page;
    FUNCTION_MARKER(0x80021BBC, "SLUS_005.43");
    result = r_u32(0x800311CC);
    FUNCTION_MARKER(0x80021BC4, "SLUS_005.43");
    if (result)
    {
        w_u32(0x800311CC, 0);
        return result;
    }
    render_check_lists();
    sound_tick();
    w_u32(0x80042178, (uint32)VSync(1));
    DrawSync(0);
    VSync(2);
    owner = r_u32(0x8004608C);
    PutDrawEnv((DRAWENV *)psx_addr(owner, sizeof(DRAWENV)));
    owner = r_u32(0x8004608C);
    PutDispEnv((DISPENV *)psx_addr(owner + 92, sizeof(DISPENV)));
    packet = r_u32(0x80040690);
    page = GetTPage(0, 1, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(packet, sizeof(DR_MODE)), 1, 1, page, NULL);
    owner = r_u32(0x8004608C);
    AddPrim(psx_addr(owner + 0x45A8, 4), psx_addr(packet, 4));
    slot = r_u32(0x80034D78);
    owner = r_u32(0x8004608C);
    w_u32(0x80040690, packet + 12);
    DrawOTag((uint32 *)psx_addr(owner + (slot << 2) + 112, 4));
    result = (uint32)VSync(1);
    w_u32(0x8004EAF0, result);
    DrawSyncCallback(render_draw_complete);
    result = r_u32(0x800311B8) + 1;
    w_u32(0x800311B8, result);
    return result;
}

uint32 stream_update(void)
{
    CdlLOC position;
    uint8 filter[2];
    uint32 standard;
    uint32 pause;
    uint32 state;
    uint32 current;
    uint32 ready;
    uint32 active;
    uint32 background;
    uint32 previous;
    uint32 source;
    uint32 destination;
    uint32 offset;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    uint32 count;
    uint32 result;
    uint32 target;
    sint32 remaining;
    sint32 divisor;
    sint32 quotient;
    sint32 numerator;
    sint32 increment;
    sint32 volume;
    sint32 duration;
    uint8 status;
    FUNCTION_MARKER(0x80028C8C, "SLUS_005.43");
    standard = r_u32(0x800311C0);
    FUNCTION_MARKER(0x80028C94, "SLUS_005.43");
    w_u32(0x80040FE0, standard ? 6 : 5);
    pause = r_u32(0x80040FF0);
    if (pause == 1 && r_u32(0x80040FE4) == 8)
        return 8;
    state = r_u32(0x80040FE4);
    if (state == 21)
        return 8;
    if (state == 8)
        w_u32(0x80040FE4, 4);
    else if (!pause && r_u32(0x80040FF4) == 1)
        w_u32(0x80040FE4, 6);
    pause = r_u32(0x80040FF0);
    w_u32(0x80040FF4, pause == 1 ? pause : 0);
    remaining = r_s32(0x80041EEC);
    if (remaining > 0)
    {
        result = (uint32)remaining - r_u32(0x80040FE0);
        w_u32(0x80041EEC, result);
        if ((sint32)result < 0)
            w_u32(0x80041EEC, 0);
        current = r_u32(0x80040FFC);
        if (r_u32(0x80040FF8) != current)
            w_u32(0x80040FE4, 4);
        goto select_current;
    }
    ready = r_u32(0x80041008);
    w_u32(0x80041EEC, 0xFFFFFFFF);
    state = r_u32(ready);
    if (state)
    {
        current = ready;
        if (state == 1 && !r_u32(r_u32(0x80041004)))
        {
            if (r_u8(current + 48))
            {
                result = r_u32(current + 32) - r_u32(current + 36);
                w_u32(0x80041EEC, result);
                message_add((char *)psx_addr(current + 48, 268), 0, (uint32)((sint32)result / 300));
                w_u8(current + 48, 0);
            }
            w_u32(current, 0);
            if (r_u32(0x80041EDC) == r_u32(0x80041EE0))
                goto select_background;
            goto consume_pending;
        }
        previous = r_u32(0x80040FF8);
        if (previous)
        {
            state = r_u32(0x80040FE4);
            if (state - 6 < 2 || state - 2 < 2)
            {
                current = previous;
                w_u32(0x80040FE4, 7);
                goto select_current;
            }
        }
        goto swap_ready;
    }
    active = r_u32(0x80041004);
    state = r_u32(active);
    current = active;
    if (state)
    {
        if (state == 1)
        {
            if (r_u8(current + 48))
            {
                result = r_u32(current + 32) - r_u32(current + 36);
                w_u32(0x80041EEC, result);
                message_add((char *)psx_addr(current + 48, 268), 0, (uint32)((sint32)result / 300));
                w_u8(current + 48, 0);
            }
            w_u32(current, 0);
            if (r_u32(0x80041EDC) == r_u32(0x80041EE0))
                goto select_background;
            goto consume_pending;
        }
        previous = r_u32(0x80040FF8);
        if (previous == current || !previous)
            goto select_current;
        state = r_u32(0x80040FE4);
        if (state - 6 >= 2 && state - 2 >= 2)
            goto select_current;
        current = previous;
        w_u32(0x80040FE4, 7);
        goto select_current;
    }
    background = r_u32(0x80041000);
    if (!r_u32(background))
        goto select_background;
    previous = r_u32(0x80040FF8);
    if (previous)
    {
        state = r_u32(0x80040FE4);
        if (state - 6 < 2 || state - 2 < 2)
        {
            current = previous;
            w_u32(0x80040FE4, 7);
            goto select_current;
        }
    }
    previous = r_u32(0x80040FFC);
    current = background;
    w_u32(0x80040FFC, current);
    w_u32(0x80041000, previous);
    w_u32(previous, 0);
    w_u32(0x80040FE4, 4);
    goto select_current;

consume_pending:
    destination = r_u32(0x80041008);
    source = 0x800414FC + r_u32(0x80041EDC) * 316;
    for (offset = 0; offset < 304; offset += 16)
    {
        first = r_u32(source + offset);
        second = r_u32(source + offset + 4);
        third = r_u32(source + offset + 8);
        fourth = r_u32(source + offset + 12);
        w_u32(destination + offset, first);
        w_u32(destination + offset + 4, second);
        w_u32(destination + offset + 8, third);
        w_u32(destination + offset + 12, fourth);
    }
    first = r_u32(source + 304);
    second = r_u32(source + 308);
    third = r_u32(source + 312);
    w_u32(destination + 304, first);
    w_u32(destination + 308, second);
    w_u32(destination + 312, third);
    w_u32(0x800414FC + r_u32(0x80041EDC) * 316, 0);
    count = r_u32(0x80041EDC);
    current = r_u32(0x80041008);
    active = r_u32(0x80041004);
    w_u32(0x80041EDC, (count + 1) & 7);
    w_u32(0x80041008, active);
    w_u32(0x80041004, current);
    w_u32(active, 0);
    goto select_background;

swap_ready:
    active = r_u32(0x80041004);
    current = r_u32(0x80041008);
    w_u32(0x80041008, active);
    previous = r_u32(0x80041008);
    w_u32(0x80041004, current);
    w_u32(previous, 0);
    w_u32(0x80040FE4, 4);
    goto select_current;

select_background:
    current = r_u32(0x80040FFC);
select_current:
    pause = r_u32(0x80040FF0);
    w_u32(0x80040FF8, current);
    if (pause == 1)
    {
        state = r_u32(0x80040FE4);
        if (state - 6 < 2 || state - 2 < 2)
            w_u32(0x80040FE4, 7);
        else
        {
            SsSetSerialVol(0, 0, 0);
            CdControlF(9, NULL);
            w_u32(0x80040FE4, 8);
            return 8;
        }
    }
    if (!r_u32(current))
    {
        w_u32(0x80040FF8, 0);
        SsSetSerialVol(0, 0, 0);
        if (r_u32(0x80041EF0) != 1)
            return 1;
        CdControlF(1, NULL);
        if (!(CdStatus() & 0x10))
            return 21;
        w_u32(0x80040FE4, 21);
        w_u32(0x80040FE8, 11);
        return 11;
    }
    state = r_u32(0x80040FE4);
    switch (state)
    {
    case 4:
        SsSetSerialVol(0, 0, 0);
        result = r_u32(current + 36);
        if (result)
        {
            result = r_u32(current + 28) + (uint32)((sint32)result / 2);
            CdIntToPos((sint32)result, &position);
            CdControlF(2, (uint8 *)&position);
        }
        else
            CdControlF(2, (uint8 *)psx_addr(current + 8, sizeof(CdlLOC)));
        w_u32(0x80040FE4, 9);
        return 9;
    case 9:
        filter[0] = 1;
        filter[1] = (uint8)r_u32(current + 12);
        CdControlF(13, filter);
        w_u32(0x80040FE4, 10);
        return 10;
    case 10:
        CdControlF(27, NULL);
        w_u32(current + 40, 5);
        w_u32(0x80040FE4, 5);
        w_u32(0x80033D10, 3000);
        return 3000;
    case 5:
        CdControlF(1, NULL);
        status = CdStatus();
        if ((status & 0x60) == 0x20)
        {
            result = r_u32(current + 40) - 1;
            w_u32(current + 40, result);
            if ((sint32)result > 0)
                return result;
            w_u32(0x80040FE4, 6);
            previous = r_u32(current + 36);
            result = r_u32(current + 32);
            status = r_u8(current + 48);
            w_u32(current + 16, 0);
            w_u32(current + 36, 0);
            result -= previous;
            w_u32(current + 40, result);
            if (status)
            {
                result = (uint32)message_add((char *)psx_addr(current + 48, 268), 0,
                    (uint32)(r_s32(current + 40) / 300));
                w_u8(current + 48, 0);
            }
            return result;
        }
        if (status & 0x10)
        {
            w_u32(0x80040FE4, 21);
            w_u32(0x80040FE8, 11);
            return 11;
        }
        result = r_u32(0x80033D10) - r_u32(0x80040FE0);
        w_u32(0x80033D10, result);
        if ((sint32)result > 0)
            return result;
        result = r_u32(0x80041EF4) + 1;
        w_u32(0x80041EF4, result);
        if ((sint32)result < 16)
            w_u32(0x80040FE4, 4);
        else
        {
            w_u32(0x80040FE4, 21);
            w_u32(0x80040FE8, 21);
            w_u32(0x80041EF4, 0);
        }
        return 21;
    default:
        break;
    }
    if (r_u32(0x80041EF0) == 1)
    {
        CdControlF(1, NULL);
        if (CdStatus() & 0x10)
        {
            w_u32(0x80040FE4, 7);
            w_u32(0x80040FE8, 11);
            w_u32(current + 16, 0);
        }
    }
    w_u32(current + 40, r_u32(current + 40) - r_u32(0x80040FE0));
    state = r_u32(0x80040FE4);
    if (state == 6 || state == 7)
    {
        duration = r_s32(current + 24);
        divisor = r_s32(0x80040FE0);
        if (divisor)
            quotient = (sint32)((sint64)duration / divisor);
        else
            quotient = duration < 0 ? 1 : -1;
        if (!divisor)
            trap(7);
        if (duration == (sint32)0x80000000 && divisor == -1)
            trap(0x5D);
        numerator = r_s16(r_u32(current + 20));
        if (quotient)
            increment = (sint32)((sint64)numerator / quotient);
        else
            increment = numerator < 0 ? 1 : -1;
        if (!quotient)
            trap(7);
        increment >>= 7;
        if (!increment)
            increment = 1;
        if (state == 6)
        {
            result = r_u32(current + 16);
            target = r_u32(current + 20);
            result += (uint32)increment;
            w_u32(current + 16, result);
            volume = (sint32)((uint32)r_u16(target) << 16) >> 23;
            if ((sint32)result >= volume)
            {
                state = r_u32(current);
                w_u32(current + 16, (uint32)volume);
                w_u32(0x80040FE4, state);
            }
            volume = r_s16(current + 16);
            SsSetSerialVol(0, (sint16)volume, (sint16)volume);
            /* Original serial mode zero leaves v0 as flags & 0x2000 */
            return 0;
        }
        result = r_u32(current + 16) - (uint32)increment;
        w_u32(current + 16, result);
        if ((sint32)result < 0)
            w_u32(current + 16, 0);
        volume = r_s16(current + 16);
        SsSetSerialVol(0, (sint16)volume, (sint16)volume);
        if (r_s32(current + 40) < 0 || !r_u32(current + 16))
        {
            if (r_u32(0x80040FF0) == 1)
                w_u32(0x80040FE4, 8);
            first = r_u32(current + 24) << 1;
            second = r_u32(current + 40);
            result = r_u32(current + 32);
            if ((sint32)first < (sint32)second)
                result -= second;
            w_u32(current + 36, result);
            if (r_u32(current) == 3)
            {
                if (!r_u32(0x80040FF0))
                {
                    w_u32(current + 36, 0);
                    w_u32(0x80040FE4, 4);
                }
                if (r_u32(0x80041EE4) == 1)
                {
                    w_u32(r_u32(0x80041004), 0);
                    w_u32(0x80041EE4, 0);
                }
                if (r_u32(0x80041EE8) == 1)
                {
                    w_u32(r_u32(0x80040FFC), 0);
                    w_u32(0x80041EE8, 0);
                }
            }
            else if (r_u32(current + 36) == r_u32(current + 32) ||
                (!r_u32(0x80040FF0) && r_u32(0x80040FE8) != 11))
            {
                w_u32(current, 0);
                w_u32(0x80040FE4, 4);
            }
            ready = r_u32(0x80041008);
            if (r_u32(ready))
            {
                active = r_u32(0x80041004);
                current = r_u32(0x80041008);
                w_u32(0x80041008, active);
                previous = r_u32(0x80041008);
                w_u32(0x80041004, current);
                w_u32(previous, 0);
            }
            else if (r_u32(0x80041EDC) != r_u32(0x80041EE0) &&
                r_s32(0x80041EEC) < 0 && !r_u32(0x80040FF0))
            {
                destination = ready;
                source = 0x800414FC + r_u32(0x80041EDC) * 316;
                for (offset = 0; offset < 304; offset += 16)
                {
                    first = r_u32(source + offset);
                    second = r_u32(source + offset + 4);
                    third = r_u32(source + offset + 8);
                    fourth = r_u32(source + offset + 12);
                    w_u32(destination + offset, first);
                    w_u32(destination + offset + 4, second);
                    w_u32(destination + offset + 8, third);
                    w_u32(destination + offset + 12, fourth);
                }
                first = r_u32(source + 304);
                second = r_u32(source + 308);
                third = r_u32(source + 312);
                w_u32(destination + 304, first);
                w_u32(destination + 308, second);
                w_u32(destination + 312, third);
                w_u32(0x800414FC + r_u32(0x80041EDC) * 316, 0);
                count = r_u32(0x80041EDC);
                current = r_u32(0x80041008);
                active = r_u32(0x80041004);
                w_u32(0x80041EDC, (count + 1) & 7);
                w_u32(0x80041008, active);
                w_u32(0x80041004, current);
                w_u32(active, 0);
            }
            if (r_u32(r_u32(0x80041000)))
            {
                background = r_u32(0x80040FFC);
                current = r_u32(0x80041000);
                w_u32(0x80041000, background);
                previous = r_u32(0x80041000);
                w_u32(0x80040FFC, current);
                w_u32(previous, 0);
            }
            if (!r_u32(r_u32(0x80041004)) && !r_u32(r_u32(0x80040FFC)) &&
                r_u32(0x80040FE4) != 8)
            {
                if (r_u32(0x80040FE8) == 11)
                {
                    w_u32(0x80040FE4, 21);
                    return 21;
                }
                CdControlF(9, NULL);
            }
        }
        if (r_u32(0x80040FE8) != 11)
            return 11;
        w_u32(0x80040FE4, 21);
        return 21;
    }
    volume = (sint32)((uint32)r_u16(r_u32(current + 20)) << 16) >> 23;
    if ((uint32)volume != r_u32(current + 16))
    {
        w_u32(current + 16, (uint32)volume);
        SsSetSerialVol(0, (sint16)volume, (sint16)volume);
    }
    if (r_s32(current + 40) < r_s32(current + 24))
    {
        w_u32(0x80040FE4, 7);
        return 1;
    }
    if (!r_s16(r_u32(current + 20)) && r_u32(current) != 3)
        w_u32(0x80040FE4, 7);
    return 3;
}

uint32 object_update_all(void)
{
    uint32 object;
    uint32 callback;
    FUNCTION_MARKER(0x80024A08, "SLUS_005.43");
    object = r_u32(0x8005367C);
    while (object)
    {
        callback = r_u32(object + 72);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
        object = r_u32(object + 468);
    }
    return object_flush_removed();
}

uint32 object_draw_visible(uint32 object)
{
    uint32 flags;
    uint32 camera;
    uint32 base;
    uint32 index;
    uint32 model;
    uint32 value;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 absolute_x;
    uint32 absolute_y;
    uint32 absolute_z;
    uint32 distance;
    uint32 radius;
    uint32 step;
    uint32 count;
    uint32 axis;
    uint32 callback;
    sint32 scale;
    sint32 initial_scale;
    sint32 shift;
    FUNCTION_MARKER(0x8002D2BC, "SLUS_005.43");
    flags = r_u32(object + 112);
    w_u8(object + 629, 0);
    w_u32(0x80031400, 0);
    if (flags & 0x40)
        return 10;
    value = r_u32(object + 164);
    flags = r_u32(object + 112);
    w_u32(object + 128, 0xFFFFD8F0);
    w_u32(0x800313D8, value);
    w_u32(0x80052838, flags & 4 ? 0x01000000 : 0);
    if (r_u32(0x8004EB2C))
        w_u32(0x80052838, r_u32(0x80052838) | 0x02000000);
    flags = r_u32(object + 112);
    camera = r_u32(0x80040C94);
    x = r_u32(object + 8);
    value = r_u32(camera + 8);
    w_u32(0x8004EB18, flags & 2);
    flags = r_u32(object + 112);
    x -= value;
    w_u32(0x8004EAA8, x);
    y = r_u32(object + 12);
    value = r_u32(camera + 12);
    absolute_x = (sint32)x < 0 ? 0 - x : x;
    w_u32(0x8004EB2C, (flags >> 12) & 2);
    y -= value;
    w_u32(0x8004EAAC, y);
    z = r_u32(object + 16);
    value = r_u32(camera + 16);
    absolute_y = (sint32)y < 0 ? 0 - y : y;
    z -= value;
    absolute_z = (sint32)z < 0 ? 0 - z : z;
    w_u32(0x8004EAB0, z);
    distance = (sint32)absolute_x < (sint32)absolute_y ? absolute_y : absolute_x;
    if ((sint32)distance < (sint32)absolute_z)
        distance = absolute_z;
    flags = r_u32(object + 276);
    if (flags & 0x20)
    {
        index = r_u32(object + 116);
        base = r_u32(0x80046178);
        value = r_u32(base + index * 132 + 116);
        index = r_u32(object + 116);
        w_u32(object + 104, value);
        value = r_u32(base + index * 132);
        w_u32(object + 100, value);
    }
    else if (r_u32(object + 108))
    {
        if (!object_select_model(object, (sint32)distance))
            return 11;
    }
    else
    {
        w_u32(object + 104, (flags & 0x10) != 0);
        if (r_s32(object + 216) < (sint32)distance)
            return 11;
    }
    initial_scale = r_s32(object + 132);
    if (initial_scale > 0)
    {
        x = r_u32(0x8004EAA8);
        value = r_u32(object + 132);
        w_u32(0x8004EAA8, (uint32)((sint32)x >> (value & 31)));
        y = r_u32(0x8004EAAC);
        value = r_u32(object + 132);
        w_u32(0x8004EAAC, (uint32)((sint32)y >> (value & 31)));
        z = r_u32(0x8004EAB0);
        value = r_u32(object + 132);
        w_u32(0x80053670, (uint32)initial_scale);
        w_u32(0x8004EAB0, (uint32)((sint32)z >> (value & 31)));
    }
    else
        w_u32(0x80053670, 0);
    for (axis = 0; axis < 3; ++axis)
    {
        value = r_u32(0x8004EAA8 + axis * 4);
        if ((sint32)value < 0)
            value = 0 - value;
        if ((sint32)value >= 0x4001)
            break;
    }
    if (axis != 3)
    {
        for (;;)
        {
            count = r_u32(0x80053670) + 1;
            x = (uint32)(r_s32(0x8004EAA8) >> 1);
            w_u32(0x8004EAA8, x);
            absolute_x = (sint32)x < 0 ? 0 - x : x;
            w_u32(0x80053670, count);
            count = r_u32(0x80031400);
            z = r_u32(0x8004EAB0);
            w_u32(0x80031400, count + 1);
            y = r_u32(0x8004EAAC);
            z = (uint32)((sint32)z >> 1);
            w_u32(0x8004EAB0, z);
            y = (uint32)((sint32)y >> 1);
            w_u32(0x8004EAAC, y);
            if ((sint32)absolute_x >= 0x4001)
                continue;
            absolute_y = (sint32)y < 0 ? 0 - y : y;
            if ((sint32)absolute_y >= 0x4001)
                continue;
            absolute_z = (sint32)z < 0 ? 0 - z : z;
            if ((sint32)absolute_z < 0x4001)
                break;
        }
    }
    ApplyMatrixLV((MATRIX *)psx_addr(0x80035060, sizeof(MATRIX)),
        (VECTOR *)psx_addr(0x8004EAA8, sizeof(VECTOR)),
        (VECTOR *)psx_addr(0x8004EAD0, sizeof(VECTOR)));
    x = r_u32(0x8004EAD0);
    value = r_u32(0x80053670);
    w_u32(object + 120, x << (value & 31));
    y = r_u32(0x8004EAD4);
    w_u32(object + 124, y << (value & 31));
    z = r_u32(0x8004EAD8);
    scale = r_s32(object + 132);
    w_u32(object + 128, z << (value & 31));
    if (scale >= 0)
    {
        index = r_u32(object + 116);
        base = r_u32(0x80046178);
        radius = r_u32(base + index * 132 + 108) << ((uint32)scale & 31);
    }
    else
    {
        value = 0 - (uint32)scale;
        count = r_u32(0x80031400);
        index = r_u32(object + 116);
        w_u32(0x80031400, count + value);
        base = r_u32(0x80046178);
        radius = (uint32)(r_s32(base + index * 132 + 108) >> (value & 31));
    }
    scale = r_s32(object + 132);
    if (scale > 0)
    {
        flags = r_u32(object + 112);
        radius = (uint32)((sint32)radius >> 12);
        if (flags == 10 && r_u32(object + 176))
            radius = (radius << 1) + (uint32)((sint32)radius >> 1);
        x = r_u32(object + 120);
        absolute_x = (sint32)x < 0 ? 0 - x : x;
        y = r_u32(object + 124);
        z = r_u32(object + 128);
        absolute_y = (sint32)y < 0 ? 0 - y : y;
        if ((sint32)z < 0)
            z = 0 - (uint32)((sint32)(0 - z) >> 12);
        else
            z = (uint32)((sint32)z >> 12);
        if ((sint32)z < (sint32)(0 - radius))
            return 11;
        if ((sint32)(radius * 8192 + z * 7094) < (sint32)absolute_x)
            return 11;
        if ((sint32)(radius * 5793 + z * 4096) < (sint32)absolute_y)
            return 11;
    }
    else
    {
        x = r_u32(object + 120);
        y = r_u32(object + 124);
        z = r_u32(object + 128);
        absolute_x = (sint32)x < 0 ? 0 - x : x;
        absolute_y = (sint32)y < 0 ? 0 - y : y;
        if ((sint32)z < (sint32)(0 - radius))
            return 11;
        if ((sint32)(radius * 8192 + z * 7094) < (sint32)(absolute_x << 12))
            return 11;
        if ((sint32)(radius * 5793 + z * 4096) < (sint32)(absolute_y << 12))
            return 11;
    }
    if (r_u32(object + 112) & 8)
    {
        matrix_copy_rotation(0x8004EAF8, object + 284);
        matrix_shift(0x8004EAF8, 0x80034DA0, r_u32(0x80031400));
        MulMatrix0((MATRIX *)psx_addr(0x80035060, sizeof(MATRIX)),
            (MATRIX *)psx_addr(0x80034DA0, sizeof(MATRIX)),
            (MATRIX *)psx_addr(0x800420F0, sizeof(MATRIX)));
        MulMatrix0((MATRIX *)psx_addr(0x80041F28, sizeof(MATRIX)),
            (MATRIX *)psx_addr(object + 284, sizeof(MATRIX)),
            (MATRIX *)psx_addr(0x80034D80, sizeof(MATRIX)));
        SetRotMatrix((MATRIX *)psx_addr(0x800420F0, sizeof(MATRIX)));
        SetLightMatrix((MATRIX *)psx_addr(0x80034D80, sizeof(MATRIX)));
    }
    else
    {
        SetRotMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
        SetLightMatrix((MATRIX *)psx_addr(0x80041F00, sizeof(MATRIX)));
    }
    x = r_u32(0x8004EAD0);
    y = r_u32(0x8004EAD4);
    z = r_u32(0x8004EAD8);
    w_u32(0x80034D6C, x);
    w_u32(0x80034D70, y);
    w_u32(0x80034D74, z);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    index = r_u32(object + 116);
    w_u8(object + 629, 1);
    base = r_u32(0x80046178);
    model = r_u32(base + index * 132 + 116);
    w_u32(0x80052E60, object);
    w_u32(object + 104, model);
    if (model)
    {
        index = r_u32(object + 116);
        scale = r_s32(object + 132);
        radius = r_u32(base + index * 132 + 108);
        step = 4096;
        if (scale > 0)
        {
            radius <<= (uint32)scale & 31;
            step <<= (uint32)scale & 31;
        }
        else
        {
            shift = (sint32)(0 - (uint32)scale);
            radius = (uint32)((sint32)radius >> ((uint32)shift & 31));
            step = (uint32)((sint32)step >> ((uint32)shift & 31));
        }
        value = r_u32(object + 128) - radius;
        if ((sint32)value < (sint32)step)
            mesh_render_fixed(r_u32(object + 100));
        else
            mesh_render_adaptive_single(r_u32(object + 100));
    }
    else
    {
        callback = r_u32(object + 56);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
    }
    w_u32(0x80052E60, 0);
    return 0;
}

void object_draw_primitives(uint32 object)
{
    uint32 color;
    uint32 flags;
    uint32 mesh;
    FUNCTION_MARKER(0x8002B8FC, "SLUS_005.43");
    if (r_u32(object + 104))
        return;
    color = r_u32(object + 212);
    flags = r_u32(0x80052838);
    mesh = r_u32(object + 100);
    w_u32(0x80052E64, color | flags);
    render_emit_mesh_primitives(mesh);
}

sint32 system_set_graph_reverse(sint32 reverse)
{
    FUNCTION_MARKER(0x8006A804, "SLUS_005.43");
    FUNCTION_MARKER(0x8006A80C, "SLUS_005.43");
    return SetGraphReverse(reverse);
}

sint32 system_set_graph_queue(sint32 enabled)
{
    FUNCTION_MARKER(0x8006A97C, "SLUS_005.43");
    FUNCTION_MARKER(0x8006A984, "SLUS_005.43");
    return SetGraphQue(enabled);
}

uint32 system_stop_callbacks(void)
{
    PsxCallbackDriver *previous;
    FUNCTION_MARKER(0x80072988, "SLUS_005.43");
    FUNCTION_MARKER(0x80072990, "SLUS_005.43");
    previous = stopIntr();
    return previous ? 0x800733B4 : 0;
}

intptr system_vsync_callbacks(sint32 index, PsxInterruptCallback callback)
{
    FUNCTION_MARKER(0x80072958, "SLUS_005.43");
    FUNCTION_MARKER(0x80072960, "SLUS_005.43");
    return (intptr)setIntrVSync((uint32)index, callback);
}

uint32 system_restart_callbacks(void)
{
    PsxCallbackDriver *changed;
    FUNCTION_MARKER(0x800729B8, "SLUS_005.43");
    FUNCTION_MARKER(0x800729C0, "SLUS_005.43");
    changed = restartIntr();
    return changed ? 0x800733B4 : 0;
}

sint32 system_cd_read_break(void)
{
    FUNCTION_MARKER(0x8005EE74, "SLUS_005.43");
    FUNCTION_MARKER(0x8005EE84, "SLUS_005.43");
    return CdControl(CdlPause, NULL, NULL);
}

uint32 system_pad_disable(void)
{
    FUNCTION_MARKER(0x8005A6BC, "SLUS_005.43");
    return xport_guest_call0(r_u32(0x8005A990));
}

sint32 system_stream_back_location(CdlLOC *location)
{
    FUNCTION_MARKER(0x8005F4B8, "SLUS_005.43");
    FUNCTION_MARKER(0x8005F4C0, "SLUS_005.43");
    return StGetBackloc(location);
}

void system_stream_set_mask(uint32 mask, uint32 start, uint32 end)
{
    FUNCTION_MARKER(0x8005F758, "SLUS_005.43");
    StSetMask(mask, start, end);
}

uint32 game_entry(void)
{
    FUNCTION_MARKER(0x8001F490, "SLUS_005.43");
    /* The native CRT owns compiler initialization */
    /* Native C uses the host call stack */
    game_init_services();
    return game_main_loop();
}

uint32 game_main_loop(void)
{
    uint32 allocation;
    uint32 quit;
    sint16 mode;
    uint16 saved_first;
    uint16 saved_second;
    sint16 saved_camera;
    uint16 saved_option;
    FUNCTION_MARKER(0x8001EE8C, "SLUS_005.43");
    pad_init_config();
    if (r_s16(0x8003115C) == 2)
    {
        allocation = overlay_load("FRONT", 0);
        video_init_front();
        xport_guest_call0(0x8008DFE4);
    }
    resource_init_index();
    sound_init_level(0, 255);
    system_enter_critical();
    pad_initialize();
    system_exit_critical();
    font_init();
    game_init_options();
    for (;;)
    {
        quit = r_u32(0x80031160);
        if (quit)
            return quit;
        CdDataCallback(0);
        CdReadyCallback(0);
        if (!r_u32(0x800311C0))
            CdSyncCallback(0);
        mode = r_s16(0x8003115C);
        switch (mode)
        {
        case 1:
            render_alloc_buffers();
            allocation = overlay_load_resource("GAME", 1);
            w_u16(0x8003115C, overlay_run(game_run_scene, allocation));
            render_free_buffers();
            break;
        case 2:
            w_u32(0x80031138, 0);
            w_u16(0x8003115C, overlay_run(front_run, allocation));
            break;
        case 3:
            w_u32(0x80031138, 0);
            allocation = overlay_load_resource("FRONT", 0);
            w_u16(0x8003115C, overlay_run(front_run, allocation));
            break;
        case 5:
            render_alloc_buffers();
            allocation = overlay_load_resource("GAME", 1);
            w_u32(0x80031138, 1);
            saved_first = r_u16(0x800595F8);
            saved_second = r_u16(0x800595FA);
            saved_camera = r_s16(0x800313DC);
            w_u16(0x8003115C, overlay_run(game_run_scene, allocation));
            w_u32(0x800313DC, (uint32)(sint32)saved_camera);
            w_u16(0x80059610, 7);
            w_u16(0x800595F8, saved_first);
            w_u16(0x800595FA, saved_second);
            render_free_buffers();
            break;
        case 6:
            saved_option = r_u16(0x800118F4);
            render_alloc_buffers();
            allocation = overlay_load_resource("GAME", 1);
            w_u32(0x80031138, 2);
            saved_first = r_u16(0x800595F8);
            saved_second = r_u16(0x800595FA);
            saved_camera = r_s16(0x800313DC);
            w_u16(0x8003115C, overlay_run(game_run_scene, allocation));
            w_u32(0x800313DC, (uint32)(sint32)saved_camera);
            w_u16(0x80059610, 7);
            w_u16(0x800595F8, saved_first);
            w_u16(0x800595FA, saved_second);
            render_free_buffers();
            w_u16(0x800118F4, saved_option);
            break;
        case 7:
            render_alloc_buffers();
            allocation = overlay_load_resource("GAME", 1);
            saved_first = r_u16(0x800595F8);
            saved_second = r_u16(0x800595FA);
            w_u16(0x800595F8, 1);
            w_u16(0x800595FA, 5);
            w_u32(0x80031134, 1);
            w_u16(0x8003115C, overlay_run(game_run_scene, allocation));
            w_u16(0x80059610, 7);
            w_u16(0x800595F8, saved_first);
            w_u16(0x800595FA, saved_second);
            render_free_buffers();
            w_u32(0x80031134, 0);
            break;
        default:
            break;
        }
        disc_wait_valid();
        scene_clear_transition();
    }
}

sint32 disc_file_open(char *name)
{
    uint32 occupied;
    uint32 slot;
    uint32 entry;
    uint32 sector;
    uint32 opened;
    FUNCTION_MARKER(0x80020A1C, "SLUS_005.43");
    FUNCTION_MARKER(0x80020A24, "SLUS_005.43");
    occupied = r_u32(0x80040698);
    slot = 0;
    if (occupied)
    {
        slot = 1;
        while (slot != 16)
        {
            occupied = r_u32(0x80040698 + slot * 16);
            if (!occupied)
                break;
            ++slot;
        }
        if (slot == 16)
            return -1;
    }
    path_uppercase_ascii(name);
    entry = 0x80040698 + slot * 16;
    if (!disc_find_file(name, (uint32 *)psx_addr(entry + 4, 8)))
        return -1;
    sector = r_u32(entry + 4);
    opened = r_u32(0x80031174);
    w_u32(entry, slot + 1);
    w_u32(entry + 12, sector);
    w_u32(0x80031174, opened + 1);
    return (sint32)(slot + 1);
}

sint32 sound_free_sample(uint32 sample)
{
    uint32 record;
    uint32 address;
    FUNCTION_MARKER(0x80025108, "SLUS_005.43");
    FUNCTION_MARKER(0x80025110, "SLUS_005.43");
    if (!r_u8(0x80031258))
        return -1;
    record = 0x80032CF0 + sample * 20;
    if (!r_u32(record))
        return -1;
    address = r_u32(record + 8);
    if (address)
    {
        SpuFree(address);
        w_u32(record + 12, 0);
        w_u32(record + 16, 0);
    }
    SpuFree(r_u32(record));
    w_u32(record + 4, 0);
    w_u32(record + 16, 0);
    return 1;
}

void sound_shutdown(void)
{
    FUNCTION_MARKER(0x80024C54, "SLUS_005.43");
    FUNCTION_MARKER(0x80024C5C, "SLUS_005.43");
    if (r_u8(0x80031258))
    {
        SpuSetKey(0, 0xFFFFFF);
        SpuQuit();
        w_u8(0x80031258, 0);
    }
}

uint32 system_spu_read_blocks(void *destination, uint32 address_units, uint32 blocks)
{
    FUNCTION_MARKER(0x8006FA5C, "SLUS_005.43");
    FUNCTION_MARKER(0x8006FA64, "SLUS_005.43");
    return spu_read_blocks(destination, (uint16)address_units, (uint16)blocks);
}

void card_initialize(uint32 mode)
{
    FUNCTION_MARKER(0x8005B50C, "SLUS_005.43");
    /* Memory card disabled pending xport_save/load */
}

void card_stop(void)
{
    FUNCTION_MARKER(0x8005B598, "SLUS_005.43");
    /* Memory card disabled pending xport_save/load */
}

void render_emit_mesh_primitives(uint32 mesh)
{
    uint32 screen;
    uint32 depths;
    uint32 vertices;
    sint32 vertex_count;
    uint32 face;
    uint32 count;
    uint32 normals;
    uint32 ordering_table;
    uint32 shade_flags;
    uint32 depth_mode;
    uint32 diagnostic;
    uint32 primitive_code;
    uint32 code;
    uint32 target;
    uint32 depth;
    uint32 bucket;
    uint32 scaled;
    uint32 slot;
    uint32 fourth_xy;
    sint32 visible;
    FUNCTION_MARKER(0x8002F1E4, "SLUS_005.43");
    screen = 0x1F800000;
    depths = 0x1F8002A0;
    vertices = r_u32(mesh);
    vertex_count = r_s32(mesh + 4);
    if (vertex_count >= 169)
    {
        depths = 0x800105E8;
        if (vertex_count >= 257)
        {
            screen = 0x80010F68;
            depths = 0x1F800000;
        }
    }
    mesh_project_vertices(vertices, screen, depths, vertex_count);
    face = r_u32(mesh + 16);
    count = r_u32(mesh + 20);
    normals = r_u32(mesh + 8);
    ordering_table = r_u32(0x800421AC);
    shade_flags = r_u32(0x80052E64);
    depth_mode = r_u32(0x800313D8);
    diagnostic = r_u32(0x800313E8);
    if (!count)
        return;
    if (diagnostic)
    {
        trap(0x407);
        return;
    }
    do
    {
        code = r_u8(face + 3);
        primitive_code = code << 24;
        code &= 0xFC;
        if (code < 0x20)
            goto next_face;
        target = r_u32(0x80031434 + code - 0x20);
        switch (target)
        {
        case 0x8002F974:
            visible = render_test_flat_triangle(0x800428FC, depths, screen, face, 1);
            break;
        case 0x8002F804:
            visible = render_test_flat_triangle(0x800428FC, depths, screen, face, 0);
            break;
        case 0x8002FA2C:
            visible = render_test_flat_quad(0x800428FC, depths, screen, face, &fourth_xy, 1);
            break;
        case 0x8002F8BC:
            visible = render_test_flat_quad(0x800428FC, depths, screen, face, &fourth_xy, 0);
            break;
        case 0x8002FCA0:
            FUNCTION_MARKER(0x8002FCA0, "SLUS_005.43");
            visible = render_test_textured_triangle(0x800428FC, depths, screen, face, 1);
            break;
        case 0x8002FB30:
            FUNCTION_MARKER(0x8002FB30, "SLUS_005.43");
            visible = render_test_textured_triangle(0x800428FC, depths, screen, face, 0);
            break;
        case 0x8002FD58:
            FUNCTION_MARKER(0x8002FD58, "SLUS_005.43");
            visible = render_test_textured_quad(0x800428FC, depths, screen, face, &fourth_xy, 1);
            break;
        case 0x8002FBE8:
            FUNCTION_MARKER(0x8002FBE8, "SLUS_005.43");
            visible = render_test_textured_quad(0x800428FC, depths, screen, face, &fourth_xy, 0);
            break;
        default:
            /* Original TMDS polygon format and immutable table define eight targets */
#if defined(_MSC_VER)
            __assume(0);
#else
            __builtin_unreachable();
#endif
        }
        depth = r_u32(0x800428FC);
        if (visible == 1)
        {
            if (depth_mode == 0xFFFFFFFF)
                bucket = depth >> 8;
            else if ((sint32)depth_mode < 0x1140)
            {
                scaled = depth << (r_u32(0x80053670) & 31);
                if ((sint32)scaled < 0x3FFF)
                    bucket = 64 + (scaled >> 6);
                else
                    bucket = 320 + (scaled >> 11);
            }
            else
                bucket = depth_mode;
            if ((sint32)bucket >= 0x114F)
                bucket = 0x114F;
            slot = ordering_table + (bucket << 2);
            switch (target)
            {
            case 0x8002F974:
                render_emit_flat_triangle(face, normals, slot, 0x80040690);
                break;
            case 0x8002F804:
                render_emit_textured_flat_triangle(face, normals, slot, 0x80040690, shade_flags, primitive_code);
                break;
            case 0x8002FA2C:
                render_emit_flat_quad(face, normals, slot, 0x80040690, fourth_xy);
                break;
            case 0x8002F8BC:
                render_emit_textured_flat_quad(face, normals, slot, 0x80040690, shade_flags, primitive_code, fourth_xy);
                break;
            case 0x8002FCA0:
                render_emit_gouraud_triangle(face, normals, slot, 0x80040690);
                break;
            case 0x8002FB30:
                render_emit_textured_gouraud_triangle(face, normals, slot, 0x80040690, shade_flags, primitive_code);
                break;
            case 0x8002FD58:
                render_emit_gouraud_quad(face, normals, slot, 0x80040690, fourth_xy);
                break;
            case 0x8002FBE8:
                render_emit_textured_gouraud_quad(face, normals, slot, 0x80040690, shade_flags, primitive_code, fourth_xy);
                break;
            }
        }
next_face:
        code = ((uint32)r_u8(face + 1) + 1) << 2;
        --count;
        face += code;
    } while (count);
}

void system_card_restore_patch(void)
{
    FUNCTION_MARKER(0x8005B798, "SLUS_005.43");
    /* Memory card disabled pending xport_save/load */
}

sint32 scene_clear_slot_flags(void)
{
    sint32 offset;
    FUNCTION_MARKER(0x80015630, "SLUS_005.43");
    for (offset = 0x64C; offset >= 0; offset -= 0x34)
        w_u16(0x800521A8 + offset, 0);
    w_u32(0x8003114C, 0);
    return offset;
}

sint32 vector_average_s16(uint32 destination, uint32 first, uint32 second)
{
    sint32 value;
    FUNCTION_MARKER(0x800161C8, "SLUS_005.43");
    value = r_s16(first);
    value += r_s16(second);
    value >>= 1;
    w_u16(destination, value);
    value = r_s16(first + 2);
    value += r_s16(second + 2);
    value >>= 1;
    w_u16(destination + 2, value);
    value = r_s16(first + 4);
    value += r_s16(second + 4);
    value >>= 1;
    w_u16(destination + 4, value);
    return value;
}

sint32 vector_average_u8_pair(uint32 destination, uint32 first, uint32 second)
{
    sint32 value;
    FUNCTION_MARKER(0x80016214, "SLUS_005.43");
    value = r_u8(first);
    value += r_u8(second);
    value >>= 1;
    w_u8(destination, value);
    value = r_u8(first + 1);
    value += r_u8(second + 1);
    value >>= 1;
    w_u8(destination + 1, value);
    return value;
}

uint32 disc_get_toc(void)
{
    FUNCTION_MARKER(0x8001FD4C, "SLUS_005.43");
    return 0x800422B0;
}

uint32 disc_file_size_handle(uint32 handle)
{
    uint32 offset;
    FUNCTION_MARKER(0x8001FD5C, "SLUS_005.43");
    offset = (handle - 1) << 4;
    if (r_u32(0x80040698 + offset) != handle)
        return 0xFFFFFFFF;
    return r_u32(0x800406A0 + offset);
}

uint32 disc_file_start_sector(uint32 handle)
{
    uint32 offset;
    FUNCTION_MARKER(0x8001FD98, "SLUS_005.43");
    offset = (handle - 1) << 4;
    if (r_u32(0x80040698 + offset) != handle)
        return 0xFFFFFFFF;
    return r_u32(0x8004069C + offset);
}

sint32 disc_name_matches(uint32 name, uint32 candidate)
{
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x80020228, "SLUS_005.43");
    do
    {
        second = r_u8(candidate++);
        first = r_u8(name++);
    } while (first == second);
    first = r_u8(name - 1);
    if (first != ';')
        return 0;
    return r_u8(candidate - 1) == 0;
}

uint32 disc_file_close(uint32 handle)
{
    uint32 slot;
    uint32 result;
    FUNCTION_MARKER(0x80020C74, "SLUS_005.43");
    slot = 0x80040698 + ((handle - 1) << 4);
    result = r_u32(slot);
    if (result)
    {
        result = r_u32(0x80031174);
        w_u32(slot, 0);
        --result;
        w_u32(0x80031174, result);
    }
    return result;
}
