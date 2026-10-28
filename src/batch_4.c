#include "game_functions.h"
#include <stdio.h>
#include <string.h>

uint32 render_set_blend_mode(uint32 blend, sint32 depth)
{
    uint32 packet;
    uint32 result;
    uint16 page;
    FUNCTION_MARKER(0x800221A8, "SLUS_005.43");
    packet = r_u32(0x80040690);
    page = GetTPage(0, (sint32)blend, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(packet, sizeof(DR_MODE)), 1, 1, page, 0);
    result = render_insert_primitive(depth >> 4, packet);
    w_u32(0x80040690, packet + 12);
    return result;
}

sint32 texture_upload_at(uint32 source, sint16 x, sint16 y, sint16 clut_x, sint16 clut_y)
{
    TEX_INFO info;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x80022224, "SLUS_005.43");
    if (r_u32(source) != 16)
        return 0;
    texture_read_info(source + 4, &info);
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = info.pixel.w;
    rectangle.h = info.pixel.h;
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(info.pixel_data, (uint32)(uint16)rectangle.w * (uint16)rectangle.h * 2));
    DrawSync(0);
    if (info.mode & 8)
    {
        rectangle.x = clut_x;
        rectangle.y = clut_y;
        rectangle.w = info.clut.w;
        rectangle.h = info.clut.h;
        LoadImagePSX(&rectangle, (uint32 *)psx_addr(info.clut_data, (uint32)(uint16)rectangle.w * (uint16)rectangle.h * 2));
        DrawSync(0);
    }
    return 1;
}

sint32 texture_upload(uint32 source, TEX_INFO *info, uint32 mode)
{
    PSX_RECT rectangle;
    uint32 index;
    uint32 color;
    FUNCTION_MARKER(0x800222FC, "SLUS_005.43");
    if (r_u32(source) != 16)
        return 0;
    if (mode - 1 < 2)
    {
        rectangle.x = info->pixel.x;
        rectangle.y = info->pixel.y;
        rectangle.w = info->pixel.w;
        rectangle.h = info->pixel.h;
        LoadImagePSX(&rectangle, (uint32 *)psx_addr(info->pixel_data, (uint32)(uint16)rectangle.w * (uint16)rectangle.h * 2));
        DrawSync(0);
    }
    if (!(info->mode & 8))
        return 1;
    if (mode && mode != 2)
        return 1;
    rectangle.x = info->clut.x;
    rectangle.y = info->clut.y;
    rectangle.w = info->clut.w;
    rectangle.h = info->clut.h;
    if (r_u32(0x800311C8))
    {
        for (index = 0; index < (uint16)info->clut.w; ++index)
        {
            color = info->clut_data + (index << 1);
            w_u16(color, r_u16(color) | 0x8000);
        }
    }
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(info->clut_data, (uint32)(uint16)rectangle.w * (uint16)rectangle.h * 2));
    DrawSync(0);
    return 1;
}

sint32 texture_read_info(uint32 source, TEX_INFO *info)
{
    uint32 format;
    FUNCTION_MARKER(0x8002244C, "SLUS_005.43");
    texture_decode_tim_native(source, info);
    format = info->mode & 3;
    if (format == 0)
        info->clut.w = 16;
    else if (format == 1)
        info->clut.w = 256;
    else
        info->clut.w = 0;
    info->clut.h = 1;
    return 1;
}

sint32 math_scale_cos(uint32 scale, uint32 angle)
{
    uint32 product;
    FUNCTION_MARKER(0x8002E188, "SLUS_005.43");
    product = (uint32)rcos((sint32)(angle & 0xFFF)) * scale;
    return (sint32)product >> 12;
}

sint32 math_scale_sin(uint32 scale, uint32 angle)
{
    uint32 product;
    FUNCTION_MARKER(0x8002E1C0, "SLUS_005.43");
    product = (uint32)rsin((sint32)(angle & 0xFFF)) * scale;
    return (sint32)product >> 12;
}

uint32 light_set_angles(sint32 index, uint32 red, uint32 green, uint32 blue, uint32 azimuth, uint32 elevation)
{
    MATRIX light;
    MATRIX color;
    sint32 horizontal;
    sint32 forward;
    sint32 vertical;
    sint16 negative_horizontal;
    sint16 negative_forward;
    uint32 row;
    FUNCTION_MARKER(0x80022BD8, "SLUS_005.43");
    w_u32(0x800311D0 + ((uint32)index << 2), 1);
    ReadLightMatrix(&light);
    ReadColorMatrix(&color);
    horizontal = math_scale_sin(4095, azimuth);
    horizontal = math_scale_cos((uint32)horizontal, elevation);
    forward = math_scale_cos(4095, azimuth);
    forward = math_scale_cos((uint32)forward, elevation);
    vertical = math_scale_sin(4095, elevation);
    if (index >= 0 && index < 3)
    {
        row = (uint32)index;
        negative_horizontal = (sint16)(0u - (uint32)horizontal);
        negative_forward = (sint16)(0u - (uint32)forward);
        light.m[row][1] = (sint16)vertical;
        w_u16(0x80041F2A + row * 6, (uint32)vertical);
        light.m[row][0] = negative_horizontal;
        light.m[row][2] = negative_forward;
        w_u16(0x80041F28 + row * 6, (uint16)negative_horizontal);
        w_u16(0x80041F2C + row * 6, (uint16)negative_forward);
        color.m[0][row] = (sint16)((sint32)(red << 12) / 255);
        color.m[1][row] = (sint16)((sint32)(green << 12) / 255);
        color.m[2][row] = (sint16)((sint32)(blue << 12) / 255);
    }
    SetLightMatrix(&light);
    SetColorMatrix(&color);
    row = (uint32)index << 2;
    w_u8(0x8003FE68 + row, red);
    w_u8(0x8003FE69 + row, green);
    w_u8(0x8003FE6A + row, blue);
    return row;
}

uint32 light_set_direction(uint32 index, uint32 x, uint32 y, uint32 z)
{
    uint32 sum;
    sint32 length;
    uint32 nx;
    uint32 ny;
    uint32 nz;
    uint32 row;
    FUNCTION_MARKER(0x80022EE4, "SLUS_005.43");
    ReadLightMatrix((MATRIX *)psx_addr(0x80041F00, sizeof(MATRIX)));
    sum = x * x + y * y + z * z;
    w_u32(0x800311D0 + (index << 2), 1);
    length = (sint32)SquareRoot0((sint32)sum);
    if (!length)
        length = 4096;
    nx = (uint32)((sint32)(x << 12) / length);
    ny = (uint32)((sint32)(y << 12) / length);
    nz = (uint32)((sint32)(z << 12) / length);
    row = ((index << 1) + index) << 1;
    w_u16(0x80041F00 + row, 0u - nx);
    w_u16(0x80041F02 + row, 0u - ny);
    w_u16(0x80041F04 + row, 0u - nz);
    w_u16(0x80041F28 + row, 0u - nx);
    w_u16(0x80041F2A + row, 0u - ny);
    w_u16(0x80041F2C + row, 0u - nz);
    SetLightMatrix((MATRIX *)psx_addr(0x80041F00, sizeof(MATRIX)));
    return row;
}

uint32 light_set_color(uint32 index, uint32 red, uint32 green, uint32 blue)
{
    MATRIX color;
    sint32 result;
    uint32 offset;
    FUNCTION_MARKER(0x8002309C, "SLUS_005.43");
    ReadColorMatrix(&color);
    color.m[0][index] = (sint16)((sint32)(red << 12) / 255);
    color.m[1][index] = (sint16)((sint32)(green << 12) / 255);
    result = (sint32)(blue << 12) / 255;
    color.m[2][index] = (sint16)result;
    SetColorMatrix(&color);
    offset = index << 2;
    w_u8(0x8003FE68 + offset, red);
    w_u8(0x8003FE69 + offset, green);
    w_u8(0x8003FE6A + offset, blue);
    return (uint32)result;
}

uint32 render_init_view(uint32 width, uint32 height, uint32 distance)
{
    uint32 active;
    uint32 index;
    sint32 center_x;
    sint32 center_y;
    FUNCTION_MARKER(0x800224BC, "SLUS_005.43");
    active = r_u32(0x800310A4);
    if (active)
        return active;
    w_u32(0x80031198, (uint32)((sint32)width / 2));
    w_u32(0x800311B4, (uint32)((sint32)height / 2));
    w_u32(0x8003119C, width);
    w_u32(0x800311B0, height);
    w_u32(0x800313F8, distance);
    w_u32(0x800313FC, (uint32)((sint32)distance / 2));
    w_u32(0x800311A0, 0);
    w_u32(0x800311A8, 0);
    w_u32(0x800311A4, width);
    w_u32(0x800311AC, height);
    render_init_buffers();
    for (index = 0; index < 4096; ++index)
        w_u16(0x8003DC68 + index * 2, (uint32)rsin((sint32)index));
    light_set_angles(0, 0, 0, 0, 0, 0);
    light_set_angles(1, 0, 0, 0, 0, 0);
    light_set_angles(2, 0, 0, 0, 0, 0);
    center_x = r_s32(0x80031198);
    center_y = r_s32(0x800311B4);
    w_u32(0x8001024C, (uint32)center_x);
    w_u32(0x80010250, (uint32)center_y);
    SetGeomOffset(center_x, center_y);
    SetGeomScreen(r_s32(0x800313F8));
    return 8;
}

uint32 render_init_buffers(void)
{
    uint32 active;
    uint32 owner;
    uint32 video;
    uint32 coordinate;
    uint32 offset;
    sint32 height;
    FUNCTION_MARKER(0x8002261C, "SLUS_005.43");
    active = r_u32(0x800310A4);
    if (active)
        return active;
    w_u32(0x8005283C, r_u32(0x800311B0));
    ResetGraph(1);
    video = r_u32(0x800311C0);
    w_u32(0x800311CC, 1);
    w_u32(0x80042170, 0x80035080);
    SetVideoMode(video != 0);
    height = r_s32(0x800311B0);
    if (height >= 480)
    {
        owner = r_u32(0x80042170);
        SetDefDrawEnv((DRAWENV *)psx_addr(owner, sizeof(DRAWENV)), 0, 0, r_s32(0x8003119C), height);
        owner = r_u32(0x80042170);
        SetDefDrawEnv((DRAWENV *)psx_addr(owner + 0x45F4, sizeof(DRAWENV)), 0, 0, r_s32(0x8003119C), r_s32(0x800311B0));
        owner = r_u32(0x80042170);
        SetDefDispEnv((DISPENV *)psx_addr(owner + 92, sizeof(DISPENV)), 0, 0, r_s32(0x8003119C), r_s32(0x800311B0));
        owner = r_u32(0x80042170);
        SetDefDispEnv((DISPENV *)psx_addr(owner + 0x4650, sizeof(DISPENV)), 0, 0, r_s32(0x8003119C), r_s32(0x800311B0));
        w_u8(r_u32(0x80042170) + 108, 1);
        w_u8(r_u32(0x80042170) + 0x4660, 1);
    }
    else
    {
        owner = r_u32(0x80042170);
        w_u32(0x8005283C, 256);
        SetDefDrawEnv((DRAWENV *)psx_addr(owner, sizeof(DRAWENV)), 0, 0, r_s32(0x8003119C), height);
        owner = r_u32(0x80042170);
        SetDefDrawEnv((DRAWENV *)psx_addr(owner + 0x45F4, sizeof(DRAWENV)), 0, r_s32(0x8005283C), r_s32(0x8003119C), r_s32(0x800311B0));
        owner = r_u32(0x80042170);
        SetDefDispEnv((DISPENV *)psx_addr(owner + 92, sizeof(DISPENV)), 0, r_s32(0x8005283C), r_s32(0x8003119C), r_s32(0x800311B0));
        owner = r_u32(0x80042170);
        SetDefDispEnv((DISPENV *)psx_addr(owner + 0x4650, sizeof(DISPENV)), 0, 0, r_s32(0x8003119C), r_s32(0x800311B0));
        w_u8(r_u32(0x80042170) + 108, 0);
        w_u8(r_u32(0x80042170) + 0x4660, 0);
    }
    height = r_s32(0x800311B0);
    if (height < 240)
    {
        video = r_u32(0x800311C0);
        owner = r_u32(0x80042170);
        coordinate = r_u16(0x80010564);
        w_u16(owner + 0x4658, coordinate);
        w_u16(owner + 100, coordinate);
        offset = r_u32(0x80010568);
        w_u16(owner + 0x465E, (uint32)height);
        w_u16(owner + 106, (uint32)height);
        coordinate = (video ? 256u : 240u) - (uint32)height;
        offset += (uint32)((sint32)coordinate >> 1);
        w_u16(owner + 0x465A, offset);
        w_u16(owner + 102, offset);
    }
    else
    {
        owner = r_u32(0x80042170);
        coordinate = r_u16(0x80010564);
        w_u16(owner + 0x4658, coordinate);
        w_u16(owner + 100, coordinate);
        coordinate = r_u16(0x80010568);
        w_u16(owner + 0x465E, (uint32)height);
        w_u16(owner + 106, (uint32)height);
        w_u16(owner + 0x465A, coordinate);
        w_u16(owner + 102, coordinate);
    }
    if (r_u32(0x800311C0))
    {
        owner = r_u32(0x80042170);
        coordinate = r_u16(0x800311B0);
        w_u16(owner + 0x4656, coordinate);
        w_u16(owner + 98, coordinate);
    }
    w_u8(r_u32(0x80042170) + 110, r_u32(0x800311C0));
    w_u8(r_u32(0x80042170) + 0x4662, r_u32(0x800311C0));
    w_u8(r_u32(0x80042170) + 109, 0);
    w_u8(r_u32(0x80042170) + 0x4661, 0);
    w_u8(r_u32(0x80042170) + 24, 1);
    w_u8(r_u32(0x80042170) + 25, 0);
    w_u8(r_u32(0x80042170) + 26, 0);
    w_u8(r_u32(0x80042170) + 27, 0);
    w_u8(r_u32(0x80042170) + 0x460C, 1);
    w_u8(r_u32(0x80042170) + 0x460D, 0);
    w_u8(r_u32(0x80042170) + 0x460E, 0);
    w_u8(r_u32(0x80042170) + 0x460F, 0);
    owner = r_u32(0x80042170);
    w_u32(0x8004608C, owner);
    return owner;
}

uint32 light_set_scaled_color(uint32 index, uint32 red, uint32 green, uint32 blue)
{
    MATRIX color;
    sint32 result;
    FUNCTION_MARKER(0x80023198, "SLUS_005.43");
    ReadColorMatrix(&color);
    color.m[0][index] = (sint16)((sint32)(red << 12) / 255);
    color.m[1][index] = (sint16)((sint32)(green << 12) / 255);
    result = (sint32)(blue << 12) / 255;
    color.m[2][index] = (sint16)result;
    SetColorMatrix(&color);
    return (uint32)result;
}

void render_set_fog_far(sint32 distance, sint32 projection)
{
    FUNCTION_MARKER(0x80023268, "SLUS_005.43");
    SetFogFar(distance, projection);
}

uint32 camera_build_axes(VECTOR *forward_input, VECTOR *up_input)
{
    VECTOR forward;
    VECTOR up;
    VECTOR right;
    VECTOR vertical;
    sint32 width;
    sint32 x;
    sint32 y;
    sint32 z;
    uint32 result;
    FUNCTION_MARKER(0x800234E0, "SLUS_005.43");
    vector_normalize(forward_input, &forward);
    vector_normalize(up_input, &up);
    OuterProduct12(&forward, &up, &right);
    vector_normalize(&right, &right);
    OuterProduct12(&forward, &right, &vertical);
    vector_normalize(&vertical, &vertical);
    w_u16(0x80035060, (uint32)right.vx);
    w_u16(0x80035062, (uint32)right.vy);
    w_u16(0x80035064, (uint32)right.vz);
    w_u16(0x80035068, (uint32)vertical.vy);
    width = r_s32(0x8003119C);
    w_u16(0x80035066, (uint32)vertical.vx);
    w_u16(0x8003506A, (uint32)vertical.vz);
    w_u16(0x8003506C, (uint32)forward.vx);
    w_u16(0x8003506E, (uint32)forward.vy);
    w_u16(0x80035070, (uint32)forward.vz);
    if (width == 320)
        return 1;
    if (width < 321)
    {
        if (width != 256)
            return 0x66660000;
        x = (sint16)right.vx * 4;
        y = (sint16)right.vy * 4;
        z = (sint16)right.vz * 4;
    }
    else if (width == 512)
    {
        x = (sint16)right.vx * 8;
        y = (sint16)right.vy * 8;
        z = (sint16)right.vz * 8;
    }
    else
    {
        result = (uint32)right.vx << 1;
        if (width != 640)
            return result;
        w_u16(0x80035060, result);
        w_u16(0x80035062, (uint32)right.vy << 1);
        result = (uint32)right.vz << 1;
        w_u16(0x80035064, result);
        return result;
    }
    w_u16(0x80035060, (uint32)(x / 5));
    w_u16(0x80035062, (uint32)(y / 5));
    result = (uint32)(z / 5);
    w_u16(0x80035064, result);
    return result;
}

uint32 camera_follow_target(void)
{
    VECTOR direction;
    uint32 object;
    uint32 camera;
    uint32 mode;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 a;
    uint32 b;
    uint32 c;
    uint32 old_x;
    uint32 old_y;
    uint32 old_z;
    uint32 dx;
    uint32 dy;
    uint32 dz;
    sint32 magnitude;
    FUNCTION_MARKER(0x800236B0, "SLUS_005.43");
    camera = r_u32(0x80042174);
    object = r_u32(0x80040FDC);
    mode = r_u32(0x800311DC);
    w_u32(0x800311F0, 0x155C);
    w_u32(0x80040C94, camera);
    w_u32(0x8004EB1C, object);
    if (mode)
    {
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        a = (uint32)r_s16(object + 0x128);
        b = (uint32)r_s16(object + 0x12A);
        w_u32(0x800311E0, x);
        x += a;
        w_u32(0x800311E4, y);
        y += b;
        c = (uint32)r_s16(object + 0x12C);
        a = r_u16(object + 0x122);
        w_u32(0x800311E8, z);
        w_u32(0x800311E0, x);
        z += c;
        x -= (uint32)((sint16)a >> 2);
        b = r_u16(object + 0x124);
        a = r_u16(object + 0x126);
        w_u32(0x800311E4, y);
        w_u32(0x800311E8, z);
        w_u32(0x800311E0, x);
        y -= (uint32)((sint16)b >> 2);
        z -= (uint32)((sint16)a >> 2);
        w_u32(0x800311E4, y);
        w_u32(0x800311E8, z);
    }
    else
    {
        a = (uint32)(r_s16(object + 0x128) >> 2);
        b = (uint32)(r_s16(object + 0x12A) >> 2);
        c = (uint32)(r_s16(object + 0x12C) >> 2);
        a += (uint32)(r_s16(object + 0x122) >> 3);
        b += (uint32)(r_s16(object + 0x124) >> 3);
        old_z = (uint32)(r_s16(object + 0x126) >> 3);
        x = r_u32(0x800311E0);
        c += old_z;
        dx = r_u32(object + 8) - (a << 2) - x;
        x += (uint32)((sint32)dx >> 2);
        old_y = r_u32(object + 12);
        w_u32(0x800311E0, x);
        y = r_u32(0x800311E4);
        dy = old_y - (b << 2) - y;
        y += (uint32)((sint32)dy >> 2);
        old_z = r_u32(object + 16);
        w_u32(0x800311E4, y);
        z = r_u32(0x800311E8);
        dz = old_z - (c << 2) - z;
        z += (uint32)((sint32)dz >> 2);
        w_u32(0x800311E8, z);
    }
    object = r_u32(0x8004EB1C);
    x = r_u32(0x800311E0);
    y = r_u32(0x800311E4);
    a = r_u32(object + 8);
    b = r_u32(object + 12);
    dx = a - x;
    dy = b - y;
    c = r_u32(object + 16);
    z = r_u32(0x800311E8);
    mode = r_u32(0x800311DC);
    dz = c - z;
    if (mode)
    {
        mode = r_u32(0x800311F0);
        a = r_u16(object + (mode == 0x155C ? 0x128 : 0x120));
        b = r_u16(object + (mode == 0x155C ? 0x12A : 0x126));
        dx -= (uint32)((sint16)a >> 1);
        dy -= (uint32)((sint16)b >> 1);
        dz -= (uint32)(r_s16(object + 0x12C) >> 1);
    }
    else
    {
        mode = r_u32(0x800311F0);
        a = (uint32)r_s16(object + (mode == 0x155C ? 0x128 : 0x120));
        c = (uint32)r_s16(object + 0x12C);
        dx += a;
        b = (uint32)r_s16(object + (mode == 0x155C ? 0x12A : 0x126));
        dz += c;
        dy += b;
    }
    for (;;)
    {
        magnitude = (sint32)dx;
        if (magnitude < 0)
            magnitude = (sint32)(0u - (uint32)magnitude);
        if (magnitude < 32001)
        {
            magnitude = (sint32)dy;
            if (magnitude < 0)
                magnitude = (sint32)(0u - (uint32)magnitude);
            if (magnitude < 32001)
            {
                magnitude = (sint32)dz;
                if (magnitude < 0)
                    magnitude = (sint32)(0u - (uint32)magnitude);
                if (magnitude < 32001)
                    break;
            }
        }
        dx = (uint32)((sint32)dx >> 1);
        dy = (uint32)((sint32)dy >> 1);
        dz = (uint32)((sint32)dz >> 1);
    }
    camera = r_u32(0x80040C94);
    x = r_u32(0x800311E0);
    y = r_u32(0x800311E4);
    z = r_u32(0x800311E8);
    old_x = r_u32(camera + 8);
    old_y = r_u32(camera + 12);
    old_z = r_u32(camera + 16);
    w_u32(camera + 8, x);
    w_u32(camera + 12, y);
    w_u32(camera + 16, z);
    w_u32(camera + 252, x - old_x);
    a = r_u32(camera + 252);
    w_u32(camera + 256, y - old_y);
    w_u32(camera + 260, z - old_z);
    c = r_u32(camera + 260);
    w_u32(camera + 20, a << 8);
    b = r_u32(camera + 256);
    w_u32(camera + 28, c << 8);
    w_u32(camera + 24, b << 8);
    mode = r_u32(0x800311F0);
    direction.vx = (sint32)dx;
    direction.vy = (sint32)dy;
    direction.vz = (sint32)dz;
    object = r_u32(mode == 0x155C ? 0x8003FE74 : 0x8004EB1C);
    a = 0u - (uint32)r_s16(object + 0x11E);
    b = 0u - (uint32)r_s16(object + 0x124);
    c = 0u - (uint32)r_s16(object + 0x12A);
    x = r_u32(0x80033D40);
    y = r_u32(0x80033D44);
    z = r_u32(0x80033D48);
    a -= x;
    b -= y;
    c -= z;
    x += (uint32)((sint32)a >> 4);
    y += (uint32)((sint32)b >> 4);
    z += (uint32)((sint32)c >> 4);
    w_u32(0x80033D40, x);
    w_u32(0x80033D44, y);
    w_u32(0x80033D48, z);
    camera_build_axes(&direction, (VECTOR *)psx_addr(0x80033D40, sizeof(VECTOR)));
    return camera_apply_vertical_scale();
}

uint32 camera_apply_vertical_scale(void)
{
    uint32 factor;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 basis[9];
    FUNCTION_MARKER(0x8002C548, "SLUS_005.43");
    if (r_u32(0x800311C0))
    {
        first = (uint32)r_s16(0x80035066);
        factor = r_u32(0x800311C4);
        first *= factor;
        second = (uint32)r_s16(0x80035068) * factor;
        third = (uint32)r_s16(0x8003506A) * factor;
        w_u16(0x80035066, (uint32)((sint32)first / 240));
        w_u16(0x80035068, (uint32)((sint32)second / 240));
        w_u16(0x8003506A, (uint32)((sint32)third / 240));
    }
    basis[0] = r_u16(0x80035060);
    basis[1] = r_u16(0x80035066);
    basis[2] = r_u16(0x8003506C);
    basis[3] = r_u16(0x80035062);
    basis[4] = r_u16(0x80035068);
    basis[5] = r_u16(0x8003506E);
    basis[6] = r_u16(0x80035064);
    basis[7] = r_u16(0x8003506A);
    basis[8] = r_u16(0x80035070);
    w_u16(0x80034D58, basis[0]);
    w_u16(0x80034D5E, basis[1]);
    w_u16(0x80034D64, basis[2]);
    w_u16(0x80034D5A, basis[3]);
    w_u16(0x80034D60, basis[4]);
    w_u16(0x80034D66, basis[5]);
    w_u16(0x80034D5C, basis[6]);
    w_u16(0x80034D62, basis[7]);
    w_u16(0x80034D68, basis[8]);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    SetLightMatrix((MATRIX *)psx_addr(0x80041F00, sizeof(MATRIX)));
    return basis[0];
}

uint32 object_list_prepend(uint32 object, uint32 list)
{
    uint32 slot;
    uint32 head;
    FUNCTION_MARKER(0x800245C0, "SLUS_005.43");
    slot = 0x80031200 + (list << 3);
    head = r_u32(slot);
    if (head)
    {
        w_u32(object + 472, r_u32(head + 472));
        head = r_u32(slot);
        w_u32(head + 472, object);
        head = r_u32(slot);
    }
    w_u32(object + 476, head);
    w_u32(slot, object);
    return head;
}

uint32 object_list_remove(uint32 object, uint32 list)
{
    uint32 previous;
    uint32 next;
    uint32 offset;
    FUNCTION_MARKER(0x80024634, "SLUS_005.43");
    previous = r_u32(object + 472);
    next = r_u32(object + 476);
    offset = list << 3;
    if (previous)
        w_u32(previous + 476, next);
    else
    {
        w_u32(0x80031200 + offset, next);
        if (next)
            w_u32(next + 472, 0);
    }
    if (next)
        w_u32(next + 472, previous);
    else
    {
        w_u32(0x80031204 + offset, previous);
        if (previous)
            w_u32(previous + 476, 0);
    }
    w_u32(object + 472, 0);
    w_u32(object + 476, 0);
    return offset;
}

uint32 object_link_chain(uint32 object)
{
    uint32 head;
    uint32 result;
    FUNCTION_MARKER(0x800246A0, "SLUS_005.43");
    head = r_u32(0x80052E68);
    if (head)
    {
        w_u32(object + 464, r_u32(head + 464));
        w_u32(head + 464, object);
    }
    result = r_u32(0x80052E68);
    w_u32(0x80052E68, object);
    w_u32(object + 468, result);
    return result;
}

uint32 object_unlink_chain(uint32 object)
{
    uint32 previous;
    uint32 next;
    FUNCTION_MARKER(0x800246DC, "SLUS_005.43");
    previous = r_u32(object + 464);
    next = r_u32(object + 468);
    if (previous)
        w_u32(previous + 468, next);
    else
    {
        w_u32(0x8005367C, next);
        if (next)
            w_u32(next + 464, 0);
    }
    if (next)
        w_u32(next + 464, previous);
    else
    {
        w_u32(0x80042164, previous);
        if (previous)
            w_u32(previous + 468, 0);
    }
    return next;
}

uint32 camera_update_modes(void)
{
    VECTOR direction;
    uint32 object;
    uint32 source;
    uint32 camera;
    uint32 mode;
    uint32 factor;
    uint32 product;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 a;
    uint32 b;
    uint32 c;
    uint32 old_x;
    uint32 old_y;
    uint32 old_z;
    uint32 dx;
    uint32 dy;
    uint32 dz;
    sint32 magnitude;
    FUNCTION_MARKER(0x80023B34, "SLUS_005.43");
    dx = 0;
    dy = 0;
    dz = 0;
    object = r_u32(0x8004EB34);
    mode = r_u32(0x8004EB30);
    w_u32(0x80040C94, 0x80040CA0);
    w_u32(0x8004EB1C, object);
    if (mode & 2)
    {
        product = (uint32)r_s16(object + 0x120);
        factor = r_u32(0x8004EB64);
        product *= factor;
        a = (uint32)((sint32)product >> 12);
        product = (uint32)r_s16(object + 0x126) * factor;
        b = (uint32)((sint32)product >> 12);
        product = (uint32)r_s16(object + 0x12C) * factor;
        c = (uint32)((sint32)product >> 12);
        product = (uint32)r_s16(object + 0x11E) * factor;
        a += (uint32)((sint32)product >> 13);
        product = (uint32)r_s16(object + 0x124) * factor;
        b += (uint32)((sint32)product >> 13);
        product = (uint32)r_s16(object + 0x12A) * factor;
        mode = r_u32(0x8004EB6C);
        c += (uint32)((sint32)product >> 13);
        if (mode)
        {
            x = r_u32(object + 8);
            y = r_u32(object + 12);
            z = r_u32(object + 16);
            w_u32(0x8004EB6C, 0);
            w_u32(0x800311E0, x - a);
            w_u32(0x800311E4, y - b);
            w_u32(0x800311E8, z - c);
        }
        else
        {
            x = r_u32(object + 8);
            old_x = r_u32(0x800311E0);
            mode = r_u32(0x8004EB68);
            dx = (uint32)((sint32)(x - a - old_x) >> 2);
            y = r_u32(object + 12);
            old_y = r_u32(0x800311E4);
            dy = (uint32)((sint32)(y - b - old_y) >> 2);
            z = r_u32(object + 16);
            old_z = r_u32(0x800311E8);
            dz = (uint32)((sint32)(z - c - old_z) >> 2);
            if (mode)
            {
                w_u32(0x800311E0, old_x + dx);
                w_u32(0x800311E4, old_y + dy);
                w_u32(0x800311E8, old_z + dz);
            }
        }
        if (r_u32(0x8004EB30) & 4)
        {
            source = r_u32(0x8004EB38);
            object = r_u32(0x8004EB1C);
            a = r_u32(source + 8) - r_u32(object + 8);
            x = r_u32(0x800311E0);
            a = (uint32)((sint32)(a << 6) >> 8);
            b = r_u32(source + 12) - r_u32(object + 12);
            b = (uint32)((sint32)(b << 6) >> 8);
            c = r_u32(source + 16) - r_u32(object + 16);
            c = (uint32)((sint32)(c << 6) >> 8);
            dx = a + r_u32(object + 8) - x;
            a = r_u32(object + 12);
            y = r_u32(0x800311E4);
            dy = b + a - y;
            a = r_u32(object + 16);
            z = r_u32(0x800311E8);
            dz = c + a - z;
        }
        else
        {
            object = r_u32(0x8004EB1C);
            x = r_u32(0x800311E0);
            a = r_u32(object + 8);
            b = r_u32(object + 12);
            c = r_u32(object + 16);
            dx = a - x;
            y = r_u32(0x800311E4);
            z = r_u32(0x800311E8);
            dy = b - y;
            dz = c - z;
        }
        object = r_u32(0x8004EB1C);
        source = object;
        if (object == r_u32(0x80040FDC))
            source = r_u32(0x8003FE74);
        a = 0u - (uint32)r_s16(source + 0x11E);
        b = 0u - (uint32)r_s16(source + 0x124);
        c = 0u - (uint32)r_s16(source + 0x12A);
        if (!(r_u32(0x8004EB30) & 0x20))
        {
            x = r_u32(0x80033D40);
            y = r_u32(0x80033D44);
            z = r_u32(0x80033D48);
            a -= x;
            b -= y;
            c -= z;
            x += (uint32)((sint32)a >> 4);
            y += (uint32)((sint32)b >> 4);
            z += (uint32)((sint32)c >> 4);
            w_u32(0x80033D40, x);
            w_u32(0x80033D44, y);
            w_u32(0x80033D48, z);
        }
    }
    if (r_u32(0x8004EB30) & 0x10)
    {
        source = r_u32(0x8004EB40);
        object = r_u32(0x8004EB1C);
        x = r_u32(source);
        y = r_u32(source + 4);
        a = r_u32(object + 8);
        b = r_u32(object + 12);
        dx = a - x;
        dy = b - y;
        z = r_u32(source + 8);
        c = r_u32(object + 16);
        old_x = r_u32(0x80040FDC);
        w_u32(0x800311E0, x);
        w_u32(0x800311E4, y);
        w_u32(0x800311E8, z);
        dz = c - z;
        source = object;
        if (object == old_x)
            source = r_u32(0x8003FE74);
        a = 0u - (uint32)r_s16(source + 0x11E);
        b = 0u - (uint32)r_s16(source + 0x124);
        c = 0u - (uint32)r_s16(source + 0x12A);
        if (!(r_u32(0x8004EB30) & 0x20))
        {
            x = r_u32(0x80033D40);
            y = r_u32(0x80033D44);
            z = r_u32(0x80033D48);
            a -= x;
            b -= y;
            c -= z;
            x += (uint32)((sint32)a >> 4);
            y += (uint32)((sint32)b >> 4);
            z += (uint32)((sint32)c >> 4);
            w_u32(0x80033D40, x);
            w_u32(0x80033D44, y);
            w_u32(0x80033D48, z);
        }
    }
    mode = r_u32(0x8004EB30);
    if (mode == 0 || mode == 8)
    {
        if (mode == 0)
        {
            object = r_u32(0x8004EB1C);
            x = r_u32(0x800311E0);
            y = r_u32(0x800311E4);
            a = r_u32(object + 8);
            b = r_u32(object + 12);
            dx = a - x;
            dy = b - y;
            c = r_u32(object + 16);
            z = r_u32(0x800311E8);
            dz = c - z;
        }
        else
        {
            source = r_u32(0x8004EB3C);
            x = r_u32(0x800311E0);
            y = r_u32(0x800311E4);
            a = r_u32(source);
            b = r_u32(source + 4);
            dx = a - x;
            dy = b - y;
            c = r_u32(source + 8);
            z = r_u32(0x800311E8);
            object = r_u32(0x8004EB1C);
            dz = c - z;
        }
        source = object;
        if (object == r_u32(0x80040FDC))
            source = r_u32(0x8003FE74);
        a = 0u - (uint32)r_s16(source + 0x11E);
        b = 0u - (uint32)r_s16(source + 0x124);
        c = 0u - (uint32)r_s16(source + 0x12A);
        if (!(r_u32(0x8004EB30) & 0x20))
        {
            x = r_u32(0x80033D40);
            y = r_u32(0x80033D44);
            z = r_u32(0x80033D48);
            a -= x;
            b -= y;
            c -= z;
            x += (uint32)((sint32)a >> 4);
            y += (uint32)((sint32)b >> 4);
            z += (uint32)((sint32)c >> 4);
            w_u32(0x80033D40, x);
            w_u32(0x80033D44, y);
            w_u32(0x80033D48, z);
        }
    }
    for (;;)
    {
        magnitude = (sint32)dx;
        if (magnitude < 0)
            magnitude = (sint32)(0u - (uint32)magnitude);
        if (magnitude < 16001)
        {
            magnitude = (sint32)dy;
            if (magnitude < 0)
                magnitude = (sint32)(0u - (uint32)magnitude);
            if (magnitude < 16001)
            {
                magnitude = (sint32)dz;
                if (magnitude < 0)
                    magnitude = (sint32)(0u - (uint32)magnitude);
                if (magnitude < 16001)
                    break;
            }
        }
        dx = (uint32)((sint32)dx >> 1);
        dy = (uint32)((sint32)dy >> 1);
        dz = (uint32)((sint32)dz >> 1);
    }
    camera = r_u32(0x80040C94);
    x = r_u32(0x800311E0);
    y = r_u32(0x800311E4);
    z = r_u32(0x800311E8);
    old_x = r_u32(camera + 8);
    old_y = r_u32(camera + 12);
    old_z = r_u32(camera + 16);
    w_u32(camera + 8, x);
    w_u32(camera + 12, y);
    w_u32(camera + 16, z);
    w_u32(camera + 252, x - old_x);
    a = r_u32(camera + 252);
    w_u32(camera + 260, z - old_z);
    w_u32(camera + 256, y - old_y);
    c = r_u32(camera + 260);
    w_u32(camera + 20, a << 8);
    b = r_u32(camera + 256);
    w_u32(camera + 28, c << 8);
    w_u32(camera + 24, b << 8);
    direction.vx = (sint32)dx;
    direction.vy = (sint32)dy;
    direction.vz = (sint32)dz;
    camera_build_axes(&direction, (VECTOR *)psx_addr(0x80033D40, sizeof(VECTOR)));
    return camera_apply_vertical_scale();
}

uint32 object_allocate(uint32 list)
{
    uint32 object;
    uint32 next;
    uint32 tail;
    FUNCTION_MARKER(0x800247F8, "SLUS_005.43");
    w_u32(0x800311F4, r_u32(0x800311F4) + 1);
    list &= 255;
    if (list >= 10)
        diagnostic_report(list, 0x80011C14, (intptr)0x80011C1C);
    object = r_u32(0x80052E68);
    if (!object)
    {
        diagnostic_report(1200, 0x80011C20, (intptr)0x80011C1C);
        return 0;
    }
    next = r_u32(object + 468);
    w_u32(0x80052E68, next);
    if (next)
        w_u32(next + 464, 0);
    tail = r_u32(0x80042164);
    w_u32(object + 468, 0);
    w_u32(object + 464, tail);
    if (tail)
        w_u32(tail + 468, object);
    else
        w_u32(0x8005367C, object);
    w_u32(0x80042164, object);
    w_u8(object + 618, list);
    if (list)
        object_list_prepend(object, list);
    else
    {
        w_u32(object + 476, 0);
        w_u32(object + 472, 0);
    }
    return object;
}

uint32 object_flush_removed(void)
{
    uint32 index;
    uint32 object;
    uint32 list;
    uint32 count;
    uint32 allocated;
    FUNCTION_MARKER(0x80024944, "SLUS_005.43");
    count = r_u32(0x800311F8);
    index = 0;
    if (count)
    {
        do
        {
            object = r_u32(0x80046090 + ((index & 0xFFFF) << 2));
            w_u32(object, 0);
            w_u32(object + 116, 0xFFFFFFFF);
            object_unlink_chain(object);
            object_link_chain(object);
            list = r_u8(object + 618);
            if (list)
                object_list_remove(object, list);
            ++index;
            allocated = r_u32(0x800311F4);
            count = r_u32(0x800311F8);
            w_u32(0x800311F4, allocated - 1);
        } while ((index & 0xFFFF) < count);
    }
    w_u32(0x800311F8, 0);
    return index & 0xFFFF;
}

sint32 sound_init_heap(void)
{
    FUNCTION_MARKER(0x80024A60, "SLUS_005.43");
    return SpuInitMalloc(128, (char *)psx_addr(0x80052A58, 129 * sizeof(SpuMallocRecord)));
}

uint32 sound_init(uint32 mode)
{
    SpuCommonAttr attribute;
    uint32 index;
    uint32 offset;
    FUNCTION_MARKER(0x80024A88, "SLUS_005.43");
    sound_set_mode((sint32)(mode & 255));
    SpuInit();
    SpuInitMalloc(128, (char *)psx_addr(0x80052A58, 129 * sizeof(SpuMallocRecord)));
    attribute.mask = 3;
    attribute.mvol.left = 0x3FC0;
    attribute.mvol.right = 0x3FC0;
    SpuSetCommonAttr(&attribute);
    SpuSetReverb(0);
    for (index = 0, offset = 0; index < 128; ++index, offset += 20)
    {
        w_u32(0x80032CF0 + offset, 0);
        w_u32(0x80032CF4 + offset, 0);
        w_u32(0x80032CF8 + offset, 0);
        w_u32(0x80032CFC + offset, 0);
    }
    for (index = 0, offset = 0; index < 24; ++index, offset += 28)
    {
        w_u32(0x800336F0 + offset, 0);
        w_u32(0x800336F4 + offset, 0);
        w_u32(0x80033994 + offset, 0);
        w_u8(0x800339A4 + offset, 255);
        w_u8(0x800339A5 + offset, 255);
        w_u8(0x80033CF4 + index, 0);
    }
    w_u8(0x8003125A, 0);
    w_u8(0x80031258, 1);
    w_u8(0x80033C30, mode);
    w_u8(0x80031259, 0);
    w_u8(0x8003125C, 0);
    return 1;
}

sint32 sound_mute_voices(void)
{
    SpuVoiceAttr attribute;
    uint32 voice;
    uint32 bit;
    uint32 offset;
    FUNCTION_MARKER(0x80024D64, "SLUS_005.43");
    for (voice = 0, offset = 0; voice < 24; ++voice, offset += 28)
    {
        bit = 1u << voice;
        attribute.voice = bit;
        attribute.mask = 3;
        SpuGetVoiceAttr(&attribute);
        w_u32(0x80033C34 + voice * 8, (uint32)attribute.volume.left);
        w_u32(0x80033C38 + voice * 8, (uint32)attribute.volume.right);
        attribute.voice = bit;
        attribute.mask = 3;
        attribute.volume.left = 0;
        attribute.volume.right = 0;
        w_u32(0x800336F8 + offset, 0);
        w_u32(0x800336FC + offset, 0);
        SpuSetVoiceAttr(&attribute);
    }
    return 0;
}

sint32 sound_restore_voices(void)
{
    SpuVoiceAttr attribute;
    uint32 voice;
    uint32 offset;
    FUNCTION_MARKER(0x80024E44, "SLUS_005.43");
    for (voice = 0, offset = 0; voice < 24; ++voice, offset += 28)
    {
        attribute.voice = 1u << voice;
        attribute.mask = 3;
        attribute.volume.left = (sint16)r_u32(0x80033C34 + voice * 8);
        attribute.volume.right = (sint16)r_u32(0x80033C38 + voice * 8);
        w_u32(0x800336F8 + offset, (uint32)attribute.volume.left);
        w_u32(0x800336FC + offset, (uint32)attribute.volume.right);
        SpuSetVoiceAttr(&attribute);
    }
    return 0;
}

sint32 sound_transfer_vag(uint32 data, uint32 second_data)
{
    uint32 size;
    uint32 frequency;
    uint32 slot;
    uint32 entry;
    uint32 count;
    uint32 second_size;
    sint32 address;
    FUNCTION_MARKER(0x80024F08, "SLUS_005.43");
    if (!r_u8(0x80031258))
        return -1;
    size = data_swap_endian(r_u32(data + 12));
    address = SpuMalloc((sint32)size);
    if (address == -1)
        return -2;
    for (slot = 0; slot < 128; ++slot)
    {
        if (!r_u32(0x80032CF4 + slot * 20))
            break;
    }
    if (slot == 128)
        return -3;
    frequency = data_swap_endian(r_u32(data + 16));
    entry = 0x80032CF0 + slot * 20;
    w_u32(entry, (uint32)address);
    w_u32(entry + 4, size);
    w_u32(entry + 16, (uint32)((sint32)(frequency << 12) / 44100));
    SpuSetTransferStartAddr((uint32)address);
    count = size > 0x7EFF0 ? 0x7EFF0 : size;
    SpuWrite(psx_addr(data + 48, count), count);
    SpuIsTransferCompleted(1);
    if (second_data)
    {
        second_size = data_swap_endian(r_u32(second_data + 12));
        address = SpuMalloc((sint32)second_size);
        if (address == -1)
            return -2;
        if (data_swap_endian(r_u32(second_data + 16)) != frequency)
            return -4;
        w_u32(entry + 8, (uint32)address);
        w_u32(entry + 12, second_size);
        SpuSetTransferStartAddr((uint32)address);
        count = second_size > 0x7EFF0 ? 0x7EFF0 : second_size;
        SpuWrite(psx_addr(second_data + 48, count), count);
        SpuIsTransferCompleted(1);
    }
    return (sint32)slot;
}

void sound_tick(void)
{
    uint32 voice;
    uint32 entry;
    uint32 countdown;
    uint32 flags;
    uint32 key_mask;
    uint32 metadata;
    uint32 linked;
    uint32 target;
    uint32 value;
    sint32 status;
    sint32 second_voice;
    sint32 direct_countdown;
    FUNCTION_MARKER(0x800251E4, "SLUS_005.43");
    if (!r_u8(0x80031258))
        return;
    key_mask = 0;
    for (voice = 0; voice < 24; ++voice)
    {
        entry = 0x800336F0 + voice * 28;
        countdown = r_u32(entry + 4);
        if (countdown == 1)
            key_mask |= 1u << voice;
        flags = r_u32(entry);
        direct_countdown = (flags & 1) && countdown;
        if ((flags & 1) && !countdown)
        {
            status = SpuGetKeyStatus(1u << voice);
            if (status == 0 || status == 3)
            {
                value = r_u32(entry + 20);
                w_u32(entry, 0);
                metadata = 0x80033994 + value * 28;
                flags = r_u32(metadata);
                if (!(flags & 2))
                    w_u32(metadata, 0);
                else if (flags & 4)
                {
                    linked = r_u32(metadata + 12);
                    linked = 0x800339A4 + linked * 28;
                    value = (uint32)r_s8(linked);
                    target = 0x800336F0 + value * 28;
                    w_u32(target, r_u32(target) & 0xFFFFFFFB);
                    second_voice = r_s8(linked + 1);
                    if (second_voice >= 0)
                    {
                        target = 0x800336F0 + (uint32)second_voice * 28;
                        w_u32(target, r_u32(target) & 0xFFFFFFFB);
                    }
                }
            }
        }
        if (direct_countdown || r_u32(entry + 4))
        {
            if (!(r_u32(entry) & 4))
                w_u32(entry + 4, r_u32(entry + 4) - 1);
        }
    }
    if ((sint32)key_mask > 0)
        SpuSetKey(1, key_mask);
}

sint32 sound_start_voice(sint32 sample, uint32 volume, sint32 left, sint32 right, uint32 pitch, uint32 flags)
{
    uint32 voice;
    uint32 offset;
    uint32 sample_offset;
    uint32 base_pitch;
    uint32 product;
    uint32 scaled;
    SpuVoiceAttr attribute;
    FUNCTION_MARKER(0x8002543C, "SLUS_005.43");
    if (!r_u8(0x80031258))
        return 0;
    if (!r_u8(0x8003125C))
        return 0;
    voice = sound_find_voice();
    product = (volume & 255) * r_u8(0x8003125B);
    scaled = product / 255;
    sample_offset = (uint32)sample * 20;
    offset = voice * 28;
    base_pitch = r_u32(0x80032D00 + sample_offset);
    w_u32(0x80033994 + offset, 1);
    product = pitch * base_pitch;
    w_u32(0x80033998 + offset, flags);
    w_u8(0x800339A7 + offset, (uint32)left);
    w_u8(0x800339A8 + offset, (uint32)right);
    w_u32(0x8003399C + offset, pitch);
    w_u8(0x800339A4 + offset, voice);
    w_u32(0x800336F0 + offset, 1);
    w_u32(0x800336F4 + offset, 1);
    w_u32(0x80033990 + offset, base_pitch);
    w_u8(0x800339A6 + offset, scaled);
    if ((sint32)product < 0)
        product += 255;
    w_u32(0x80033700 + offset, (uint32)((sint32)(product << 8) >> 16));
    w_u32(0x80033704 + offset, voice);
    attribute.voice = 1u << (voice & 31);
    attribute.mask = 0x90;
    attribute.addr = r_u32(0x80032CF0 + sample_offset);
    attribute.pitch = (uint16)r_u32(0x80033700 + offset);
    SpuSetVoiceAttr(&attribute);
    w_u8(0x800339A5 + offset, 255);
    return sound_update_voice(voice);
}

sint32 sound_update_voice_mode1(uint32 voice)
{
    SpuVoiceAttr attribute;
    uint32 offset;
    uint32 channel;
    uint32 channel_offset;
    uint32 volume;
    FUNCTION_MARKER(0x80025DB4, "SLUS_005.43");
    offset = voice * 28;
    channel = (uint32)r_s8(0x800339A4 + offset);
    channel_offset = channel * 28;
    volume = r_u8(0x800339A6 + offset);
    if (r_u32(0x80033704 + channel_offset) != voice)
        return (sint32)volume;
    volume <<= 6;
    attribute.voice = 1u << (channel & 31);
    attribute.mask = 3;
    attribute.volume.left = (sint16)volume;
    attribute.volume.right = (sint16)volume;
    w_u32(0x800336F8 + channel_offset, volume);
    w_u32(0x800336FC + channel_offset, volume);
    SpuSetVoiceAttr(&attribute);
    return 0;
}

sint32 sound_update_voice_mode2(uint32 voice)
{
    SpuVoiceAttr attribute;
    uint32 offset;
    uint32 channel;
    uint32 channel_offset;
    uint32 pan;
    uint32 volume;
    uint32 left;
    uint32 right;
    uint32 flags;
    FUNCTION_MARKER(0x80025E68, "SLUS_005.43");
    offset = voice * 28;
    volume = r_u8(0x800339A6 + offset);
    flags = r_u32(0x80033998 + offset);
    pan = r_u8(0x800339A7 + offset);
    volume <<= 6;
    if (flags & 2)
    {
        left = volume;
        right = 0u - volume;
    }
    else
    {
        left = (volume * r_u8(0x80031260 + pan)) >> 8;
        right = (volume * r_u8(0x8003127E - pan)) >> 8;
    }
    channel = (uint32)r_s8(0x800339A4 + offset);
    channel_offset = channel * 28;
    if (r_u32(0x80033704 + channel_offset) != voice)
        return 1;
    attribute.voice = 1u << (channel & 31);
    attribute.mask = 3;
    attribute.volume.left = (sint16)left;
    attribute.volume.right = (sint16)right;
    w_u32(0x800336F8 + channel_offset, left);
    w_u32(0x800336FC + channel_offset, right);
    SpuSetVoiceAttr(&attribute);
    return 0;
}

sint32 sound_update_voice_mode3(uint32 voice)
{
    SpuVoiceAttr attribute;
    uint32 offset;
    uint32 pan;
    uint32 volume;
    uint32 left;
    uint32 right;
    uint32 second_left;
    uint32 second_right;
    uint32 channel;
    uint32 channel_offset;
    sint32 second;
    FUNCTION_MARKER(0x80025F9C, "SLUS_005.43");
    offset = voice * 28;
    volume = r_u8(0x800339A6 + offset);
    pan = r_u8(0x800339A7 + offset);
    volume <<= 6;
    second = r_s8(0x800339A5 + offset);
    second_left = 0;
    if (second != -1)
    {
        pan <<= 2;
        left = (r_u8(0x800312A0 + pan) * volume) >> 8;
        right = (r_u8(0x800312A1 + pan) * volume) >> 8;
        second_left = (r_u8(0x800312A2 + pan) * volume) >> 8;
        second_right = (r_u8(0x800312A3 + pan) * volume) >> 8;
    }
    else
    {
        left = (volume * r_u8(0x80031260 + pan)) >> 8;
        right = (volume * r_u8(0x8003127E - pan)) >> 8;
        second_right = 0;
    }
    if (r_u32(0x80033998 + offset) & 2)
    {
        left = volume;
        right = 0u - volume;
        second_left = 0;
        second_right = 0;
    }
    channel = (uint32)r_s8(0x800339A4 + offset);
    channel_offset = channel * 28;
    if (r_u32(0x80033704 + channel_offset) == voice)
    {
        attribute.voice = 1u << (channel & 31);
        attribute.mask = 3;
        attribute.volume.left = (sint16)left;
        attribute.volume.right = (sint16)right;
        w_u32(0x800336F8 + channel_offset, left);
        w_u32(0x800336FC + channel_offset, right);
        SpuSetVoiceAttr(&attribute);
    }
    second = r_s8(0x800339A5 + offset);
    if (second == -1)
        return -8;
    channel = (uint32)second;
    channel_offset = channel * 28;
    if (r_u32(0x80033704 + channel_offset) != voice)
        return 1;
    attribute.voice = 1u << (channel & 31);
    attribute.mask = 3;
    attribute.volume.left = (sint16)second_left;
    attribute.volume.right = (sint16)second_right;
    w_u32(0x800336F8 + channel_offset, second_left);
    w_u32(0x800336FC + channel_offset, second_right);
    SpuSetVoiceAttr(&attribute);
    return 0;
}

sint32 sound_update_voice_mode4(uint32 voice)
{
    SpuVoiceAttr attribute;
    uint32 offset;
    uint32 volume;
    uint32 flags;
    uint32 pan;
    uint32 depth;
    uint32 front;
    uint32 back;
    uint32 left;
    uint32 right;
    uint32 back_left;
    uint32 back_right;
    uint32 channel;
    uint32 channel_offset;
    uint32 owner;
    FUNCTION_MARKER(0x80026228, "SLUS_005.43");
    offset = voice * 28;
    volume = r_u8(0x800339A6 + offset);
    flags = r_u32(0x80033998 + offset);
    pan = r_u8(0x800339A7 + offset);
    depth = r_u8(0x800339A8 + offset);
    volume <<= 6;
    front = volume;
    back = volume;
    if (flags & 4)
    {
        if (depth < 15)
            front = (volume * depth) / 15;
        else if (depth >= 16)
            back = (uint32)((sint32)(volume * (31u - depth)) / 15);
    }
    left = (uint32)((sint32)(front * r_u8(0x80031260 + pan)) >> 8);
    right = (uint32)((sint32)(front * r_u8(0x8003127E - pan)) >> 8);
    back_left = (uint32)((sint32)(back * r_u8(0x80031280 + pan)) >> 8);
    back_right = (uint32)((sint32)(back * r_u8(0x8003129E - pan)) >> 8);
    if ((flags & 6) == 2 && r_u8(0x80033C30) != 1)
    {
        left = back_left;
        right = 0u - back_right;
    }
    channel = (uint32)r_s8(0x800339A4 + offset);
    channel_offset = channel * 28;
    if (r_u32(0x80033704 + channel_offset) == voice)
    {
        attribute.voice = 1u << (channel & 31);
        attribute.mask = 3;
        attribute.volume.left = (sint16)left;
        attribute.volume.right = (sint16)right;
        w_u32(0x800336F8 + channel_offset, left);
        w_u32(0x800336FC + channel_offset, right);
        SpuSetVoiceAttr(&attribute);
    }
    if (r_u8(0x80033C30) != 4 || !(flags & 4))
        return (sint32)(flags & 4);
    channel = (uint32)r_s8(0x800339A5 + offset);
    channel_offset = channel * 28;
    owner = r_u32(0x80033704 + channel_offset);
    if (owner != voice)
        return (sint32)owner;
    right = 0u - back_right;
    attribute.voice = 1u << (channel & 31);
    attribute.mask = 3;
    attribute.volume.left = (sint16)back_left;
    attribute.volume.right = (sint16)right;
    w_u32(0x800336F8 + channel_offset, back_left);
    w_u32(0x800336FC + channel_offset, right);
    SpuSetVoiceAttr(&attribute);
    return 0;
}

sint32 sound_start_voice_at(sint32 sample, uint32 volume, sint32 left, sint32 right, uint32 pitch, uint32 flags, uint32 voice)
{
    uint32 offset;
    uint32 sample_offset;
    uint32 base_pitch;
    uint32 product;
    uint32 scaled;
    SpuVoiceAttr attribute;
    FUNCTION_MARKER(0x800265B8, "SLUS_005.43");
    if (!r_u8(0x80031258))
        return 0;
    if (!r_u8(0x8003125C))
        return 0;
    product = (volume & 255) * r_u8(0x8003125B);
    scaled = product / 255;
    sample_offset = (uint32)sample * 20;
    offset = voice * 28;
    base_pitch = r_u32(0x80032D00 + sample_offset);
    w_u32(0x80033994 + offset, 1);
    product = pitch * base_pitch;
    w_u32(0x80033998 + offset, flags);
    w_u8(0x800339A7 + offset, (uint32)left);
    w_u8(0x800339A8 + offset, (uint32)right);
    w_u32(0x8003399C + offset, pitch);
    w_u8(0x800339A4 + offset, voice);
    w_u32(0x800336F0 + offset, 1);
    w_u32(0x800336F4 + offset, 1);
    w_u32(0x80033990 + offset, base_pitch);
    w_u8(0x800339A6 + offset, scaled);
    if ((sint32)product < 0)
        product += 255;
    w_u32(0x80033700 + offset, (uint32)((sint32)(product << 8) >> 16));
    w_u32(0x80033704 + offset, voice);
    attribute.voice = 1u << (voice & 31);
    attribute.mask = 0x90;
    attribute.addr = r_u32(0x80032CF0 + sample_offset);
    attribute.pitch = (uint16)r_u32(0x80033700 + offset);
    SpuSetVoiceAttr(&attribute);
    w_u8(0x800339A5 + offset, 255);
    return sound_update_voice(voice);
}

void sound_init_level(uint32 variant, uint32 level)
{
    uint32 buffer;
    FUNCTION_MARKER(0x80026978, "SLUS_005.43");
    buffer = heap_alloc_back(0x7D000);
    if (!buffer)
        diagnostic_report(100, 0x80011DEC, (intptr)0x80011E14);
    sound_stop_all();
    sound_init(2);
    sound_load_samples(buffer);
    sound_resume();
    sound_set_master(255);
    sound_enable_sample(20);
    sound_set_level(level);
    disc_set_cd_volume(r_s16(0x800118FA));
    heap_free(buffer);
}

sint32 sound_offset_to_pan(sint32 offset)
{
    uint32 value;
    uint32 magnitude;
    FUNCTION_MARKER(0x80026A34, "SLUS_005.43");
    if (offset > 0)
    {
        value = (uint32)(offset >> 8) + 15;
        if ((sint8)value >= 31)
            value = 30;
    }
    else
    {
        magnitude = (uint32)offset;
        if (offset < 0)
            magnitude = 0u - magnitude;
        value = 15u - (uint32)((sint32)magnitude >> 8);
        if ((sint8)value < 0)
            value = 0;
    }
    return (sint8)value;
}

static sint32 sound_play_at_listener_body(uint32 position, const VECTOR *native_position, uint32 sample, uint32 unused, uint32 listener)
{
    VECTOR delta;
    sint32 magnitude;
    sint32 distance;
    sint32 divisor;
    sint32 pan;
    sint32 depth;
    sint32 volume;
    sint32 attenuation;
    sint32 level;
    sint32 result;
    uint32 value;
    uint32 squared;
    pan = 0;
    depth = 0;
    value = (native_position ? (uint32)native_position->vx : r_u32(position)) - r_u32(listener + 8);
    delta.vx = (sint32)value >> 2;
    value = (native_position ? (uint32)native_position->vy : r_u32(position + 4)) - r_u32(listener + 12);
    delta.vy = (sint32)value >> 2;
    value = (native_position ? (uint32)native_position->vz : r_u32(position + 8)) - r_u32(listener + 16);
    delta.vz = (sint32)value >> 2;
    ApplyMatrixLV((MATRIX *)psx_addr(r_u32(0x80040FDC) + 284, sizeof(MATRIX)), &delta, &delta);
    magnitude = delta.vx;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude >= 0x4000)
        return 0;
    magnitude = delta.vy;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude >= 0x4000)
        return 0;
    magnitude = delta.vz;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude >= 0x4000)
        return 0;
    distance = vector_length_approx(delta.vx, delta.vy, delta.vz);
    if (distance)
    {
        squared = (uint32)delta.vx * (uint32)delta.vx;
        squared += (uint32)delta.vy * (uint32)delta.vy;
        squared += (uint32)delta.vz * (uint32)delta.vz;
        divisor = (sint32)SquareRoot0((sint32)squared);
        if (!divisor)
            divisor = 1;
        pan = (sint32)((uint32)delta.vx << 12) / divisor;
        depth = (sint32)((uint32)delta.vz << 12) / divisor;
        pan = (sint8)sound_offset_to_pan(pan);
        depth = (sint8)sound_offset_to_pan(depth);
        value = 0x4000u - (uint32)distance;
        attenuation = (sint32)value >> 6;
        if ((sint32)value < 0)
            attenuation = 0;
        if (attenuation >= 256)
            attenuation = 255;
        level = r_s16(0x800118FC);
        result = level;
        volume = (sint32)((uint32)level * (uint32)attenuation) >> 9;
    }
    else
    {
        value = r_u16(0x800118FC) << 16;
        result = (sint32)value;
        volume = (sint32)value >> 17;
    }
    if (volume > 0)
        return sound_start_voice(r_s32(0x80042958 + ((sample & 255) << 2)), (uint32)volume & 255, (uint32)pan & 255, (uint32)depth & 255, 256, r_u32(0x800313A0));
    return result;
}

sint32 sound_play_at_listener(uint32 position, uint32 sample, uint32 unused, uint32 listener)
{
    FUNCTION_MARKER(0x80026AA0, "SLUS_005.43");
    return sound_play_at_listener_body(position, NULL, sample, unused, listener);
}

sint32 sound_play_at_listener_native(const VECTOR *position, uint32 sample, uint32 unused, uint32 listener)
{
    return sound_play_at_listener_body(0, position, sample, unused, listener);
}

sint32 sound_play_at_camera(uint32 position, uint32 sample, uint32 pitch)
{
    VECTOR delta;
    uint32 listener;
    sint32 magnitude;
    sint32 distance;
    sint32 divisor;
    sint32 pan;
    sint32 depth;
    sint32 volume;
    sint32 attenuation;
    sint32 level;
    sint32 result;
    uint32 value;
    uint32 squared;
    FUNCTION_MARKER(0x80026D28, "SLUS_005.43");
    listener = r_u32(0x80040C94);
    pan = 0;
    depth = 0;
    value = r_u32(position) - r_u32(listener + 8);
    delta.vx = (sint32)value >> 2;
    value = r_u32(position + 4) - r_u32(listener + 12);
    delta.vy = (sint32)value >> 2;
    value = r_u32(position + 8) - r_u32(listener + 16);
    delta.vz = (sint32)value >> 2;
    ApplyMatrixLV((MATRIX *)psx_addr(r_u32(0x80040FDC) + 284, sizeof(MATRIX)), &delta, &delta);
    magnitude = delta.vx;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude > 199999)
        return 1;
    magnitude = delta.vy;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude > 199999)
        return 1;
    magnitude = delta.vz;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude > 199999)
        return 1;
    distance = vector_length_approx(delta.vx, delta.vy, delta.vz);
    if (distance)
    {
        squared = (uint32)delta.vx * (uint32)delta.vx;
        squared += (uint32)delta.vy * (uint32)delta.vy;
        squared += (uint32)delta.vz * (uint32)delta.vz;
        divisor = (sint32)SquareRoot0((sint32)squared);
        if (!divisor)
            divisor = 1;
        pan = (sint32)((uint32)delta.vx << 12) / divisor;
        depth = (sint32)((uint32)delta.vz << 12) / divisor;
        pan = (sint8)sound_offset_to_pan(pan);
        depth = (sint8)sound_offset_to_pan(depth);
        value = 200000u - (uint32)distance;
        attenuation = (sint32)value >> 7;
        if ((sint32)value < 0)
            attenuation = 0;
        if (attenuation >= 256)
            attenuation = 255;
        level = r_s16(0x800118FC);
        result = level;
        volume = (sint32)((uint32)level * (uint32)attenuation) >> 8;
    }
    else
    {
        volume = r_s16(0x800118FC);
        result = 0;
    }
    if (volume > 0)
        return sound_start_voice(r_s32(0x80042958 + ((sample & 255) << 2)), (uint32)volume & 255, (uint32)pan & 255, (uint32)depth & 255, pitch, r_u32(0x800313A0));
    return result;
}

sint32 sound_play_distant_at_camera(uint32 position, uint32 sample, uint32 pitch)
{
    VECTOR delta;
    uint32 listener;
    sint32 magnitude;
    sint32 distance;
    sint32 divisor;
    sint32 pan;
    sint32 depth;
    sint32 volume;
    sint32 attenuation;
    sint32 level;
    sint32 result;
    uint32 value;
    uint32 squared;
    FUNCTION_MARKER(0x80026FC4, "SLUS_005.43");
    listener = r_u32(0x80040C94);
    pan = 0;
    depth = 0;
    value = r_u32(position) - r_u32(listener + 8);
    delta.vx = (sint32)value >> 2;
    value = r_u32(position + 4) - r_u32(listener + 12);
    delta.vy = (sint32)value >> 2;
    value = r_u32(position + 8) - r_u32(listener + 16);
    delta.vz = (sint32)value >> 2;
    ApplyMatrixLV((MATRIX *)psx_addr(r_u32(0x80040FDC) + 284, sizeof(MATRIX)), &delta, &delta);
    magnitude = delta.vx;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude > 399999)
        return 1;
    magnitude = delta.vy;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude > 399999)
        return 1;
    magnitude = delta.vz;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude > 399999)
        return 1;
    distance = vector_length_approx(delta.vx, delta.vy, delta.vz);
    if (distance)
    {
        squared = (uint32)delta.vx * (uint32)delta.vx;
        squared += (uint32)delta.vy * (uint32)delta.vy;
        squared += (uint32)delta.vz * (uint32)delta.vz;
        divisor = (sint32)SquareRoot0((sint32)squared);
        if (!divisor)
            divisor = 1;
        pan = (sint32)((uint32)delta.vx << 12) / divisor;
        depth = (sint32)((uint32)delta.vz << 12) / divisor;
        pan = (sint8)sound_offset_to_pan(pan);
        depth = (sint8)sound_offset_to_pan(depth);
        value = 400000u - (uint32)distance;
        attenuation = (sint32)value >> 7;
        if ((sint32)value < 0)
            attenuation = 0;
        if (attenuation >= 256)
            attenuation = 255;
        level = r_s16(0x800118FC);
        result = level;
        volume = (sint32)((uint32)level * (uint32)attenuation) >> 8;
    }
    else
    {
        volume = r_s16(0x800118FC);
        result = 0;
    }
    if (volume > 0)
        return sound_start_voice(r_s32(0x80042958 + ((sample & 255) << 2)), (uint32)volume & 255, (uint32)pan & 255, (uint32)depth & 255, pitch, r_u32(0x800313A0));
    return result;
}

sint32 sound_play_quiet_at_listener(uint32 position, uint32 sample, uint32 unused, uint32 listener)
{
    VECTOR delta;
    sint32 magnitude;
    sint32 distance;
    sint32 divisor;
    sint32 pan;
    sint32 depth;
    sint32 volume;
    sint32 attenuation;
    sint32 level;
    sint32 result;
    uint32 value;
    uint32 squared;
    FUNCTION_MARKER(0x80027268, "SLUS_005.43");
    pan = 0;
    depth = 0;
    value = r_u32(position) - r_u32(listener + 8);
    delta.vx = (sint32)value >> 2;
    value = r_u32(position + 4) - r_u32(listener + 12);
    delta.vy = (sint32)value >> 2;
    value = r_u32(position + 8) - r_u32(listener + 16);
    delta.vz = (sint32)value >> 2;
    ApplyMatrixLV((MATRIX *)psx_addr(r_u32(0x80040FDC) + 284, sizeof(MATRIX)), &delta, &delta);
    magnitude = delta.vx;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude >= 0x4000)
        return 0;
    magnitude = delta.vy;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude >= 0x4000)
        return 0;
    magnitude = delta.vz;
    if (magnitude < 0)
        magnitude = (sint32)(0u - (uint32)magnitude);
    if (magnitude >= 0x4000)
        return 0;
    distance = vector_length_approx(delta.vx, delta.vy, delta.vz);
    if (distance)
    {
        squared = (uint32)delta.vx * (uint32)delta.vx;
        squared += (uint32)delta.vy * (uint32)delta.vy;
        squared += (uint32)delta.vz * (uint32)delta.vz;
        divisor = (sint32)SquareRoot0((sint32)squared);
        if (!divisor)
            divisor = 1;
        pan = (sint32)((uint32)delta.vx << 12) / divisor;
        depth = (sint32)((uint32)delta.vz << 12) / divisor;
        pan = (sint8)sound_offset_to_pan(pan);
        depth = (sint8)sound_offset_to_pan(depth);
        value = 0x4000u - (uint32)distance;
        attenuation = (sint32)value >> 6;
        if ((sint32)value < 0)
            attenuation = 0;
        if (attenuation >= 256)
            attenuation = 255;
        level = r_s16(0x800118FC);
        result = level;
        volume = (sint32)((uint32)level * (uint32)attenuation) >> 10;
    }
    else
    {
        value = r_u16(0x800118FC) << 16;
        result = (sint32)value;
        volume = (sint32)value >> 18;
    }
    if (volume > 0)
        return sound_start_voice(r_s32(0x80042958 + ((sample & 255) << 2)), (uint32)volume & 255, (uint32)pan & 255, (uint32)depth & 255, 256, r_u32(0x800313A0));
    return result;
}

sint32 sound_play_sample_full(uint32 sample)
{
    uint32 flags;
    uint32 volume;
    sint32 sound;
    FUNCTION_MARKER(0x80027550, "SLUS_005.43");
    flags = r_u32(0x800313A0);
    volume = r_u8(0x800118FC);
    sound = r_s32(0x80042958 + ((sample & 255) << 2));
    return sound_start_voice(sound, volume, 15, 15, 256, flags);
}

uint32 resource_load_sector_index(const char *index_name, const char *data_name)
{
#ifndef CW_EXTRACTED_XA
    char path[64];
    CdlFILE file;
#endif
    uint32 size;
    uint32 buffer;
    uint32 entry;
    uint32 base_sector;
    uint32 sector;
    uint32 end_sector;
    sint32 index;
    FUNCTION_MARKER(0x8002767C, "SLUS_005.43");
    size = resource_file_size(index_name);
    buffer = heap_alloc_back(size);
    resource_load_into(index_name, buffer);
#ifdef CW_EXTRACTED_XA
    if (!game_stream_package_base(data_name, &base_sector))
        return buffer;
    entry = buffer + 8;
#else
    sprintf(path, "\\%s;1", data_name);
    entry = buffer + 8;
    if (!CdSearchFile(&file, path))
        return buffer;
    base_sector = (uint32)CdPosToInt(&file.pos);
#endif
    if (r_s32(buffer + 4) <= 0)
        return buffer;
    index = 0;
    do
    {
        ++index;
        w_u32(entry, r_u32(entry) + base_sector);
        end_sector = r_u32(entry + 4) + base_sector;
        sector = r_u32(entry);
        w_u32(entry + 4, end_sector);
        CdIntToPos((sint32)sector, (CdlLOC *)psx_addr(entry + 8, sizeof(CdlLOC)));
        entry += 24;
    } while (index < r_s32(buffer + 4));
    return buffer;
}

uint32 stream_clear_queue(void)
{
    uint32 producer;
    sint32 offset;
    FUNCTION_MARKER(0x80027A08, "SLUS_005.43");
    producer = r_u32(0x80041EE0);
    w_u32(0x80041EDC, producer);
    w_u32(0x800414FC + producer * 316, 0);
    for (offset = 2212; offset >= 0; offset -= 316)
        w_u8(0x8004152C + (uint32)offset, 0);
    return stream_stop();
}

sint32 stream_enqueue(uint32 asset, uint32 value, const char *text)
{
    uint32 producer;
    uint32 record;
    uint32 current;
    FUNCTION_MARKER(0x80027B04, "SLUS_005.43");
    if (((r_u32(0x80041EE0) + 1) & 7) == r_u32(0x80041EDC))
        return 0;
    if (!r_u32(r_u32(0x80041004)) && !r_u32(r_u32(0x80041008)))
    {
        stream_request(asset, 20000, value, text);
        return 1;
    }
    producer = r_u32(0x80041EE0);
    record = 0x800414FC + producer * 316;
    w_u32(record, 0);
    w_u32(record + 4, 20000);
    w_u32(record + 32, asset);
    w_u32(record + 36, 0);
    w_u32(record + 40, 0);
    if (text)
        strcpy((char *)psx_addr(record + 48, strlen(text) + 1), text);
    else
        w_u8(record + 48, 0);
    w_u32(record + 44, value);
    w_u32(record, 1);
    current = r_u32(0x80040FF8);
    if (!current || !r_u32(current))
        w_u32(0x80040FE4, 4);
    w_u32(0x80041EE0, (producer + 1) & 7);
    return 1;
}

uint32 stream_request(uint32 asset, sint32 priority, uint32 value, const char *text)
{
    uint32 buffer;
    uint32 current;
    uint32 result;
    sint32 accept;
    FUNCTION_MARKER(0x80027CC0, "SLUS_005.43");
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
    w_u32(r_u32(0x80041008) + 4, (uint32)priority);
    w_u32(r_u32(0x80041008) + 32, asset);
    w_u32(r_u32(0x80041008) + 36, 0);
    w_u32(r_u32(0x80041008) + 40, 0);
    buffer = r_u32(0x80041008);
    if (text)
        strcpy((char *)psx_addr(buffer + 48, strlen(text) + 1), text);
    else
        w_u8(r_u32(0x80041008) + 48, 0);
    w_u32(r_u32(0x80041008) + 44, value);
    w_u32(r_u32(0x80041008), 1);
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
