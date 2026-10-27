#include "game_functions.h"
#include <stdlib.h>

static uint32 game_radar_vertex(uint32 index)
{
    static const uint32 vertices[8] = {
        0x800FA280, 0x800FA290, 0x800FA2A0, 0x800FA2C8,
        0x800FA2D8, 0x800FA2E0, 0x800FA2F0, 0x800FA338
    };
    return vertices[index];
}

static void game_radar_translate_geometry(void)
{
    uint32 x, y, z, next_x, next_y;
    x = r_u16(0x800FA280);
    y = r_u16(0x800FA282);
    next_x = r_u16(0x800FA290);
    w_u16(0x800FA282, y + 600);
    z = r_u16(0x800FA284);
    w_u16(0x800FA280, x);
    next_y = r_u16(0x800FA292);
    w_u16(0x800FA290, next_x);
    x = r_u16(0x800FA2A0);
    w_u16(0x800FA284, z + 2800);
    z = r_u16(0x800FA294);
    w_u16(0x800FA292, next_y + 600);
    y = r_u16(0x800FA2A2);
    w_u16(0x800FA2A0, x);
    next_x = r_u16(0x800FA2C8);
    w_u16(0x800FA294, z + 2800);
    z = r_u16(0x800FA2A4);
    w_u16(0x800FA2A2, y + 600);
    next_y = r_u16(0x800FA2CA);
    w_u16(0x800FA2C8, next_x);
    w_u16(0x800FA2A4, z + 2800);
    z = r_u16(0x800FA2CC);
    w_u16(0x800FA2CA, next_y + 600);
    x = r_u16(0x800FA2D8);
    y = r_u16(0x800FA2DA);
    w_u16(0x800FA2CC, z + 2800);
    w_u16(0x800FA2D8, x);
    z = r_u16(0x800FA2DC);
    next_x = r_u16(0x800FA2E0);
    w_u16(0x800FA2DA, y + 600);
    next_y = r_u16(0x800FA2E2);
    w_u16(0x800FA2DC, z + 2800);
    w_u16(0x800FA2E0, next_x);
    w_u16(0x800FA2E2, next_y + 600);
    z = r_u16(0x800FA2E4);
    x = r_u16(0x800FA2F0);
    y = r_u16(0x800FA2F2);
    w_u16(0x800FA2E4, z + 2800);
    w_u16(0x800FA2F0, x);
    z = r_u16(0x800FA2F4);
    w_u16(0x800FA2F2, y + 600);
    next_x = r_u16(0x800FA338);
    w_u16(0x800FA2F4, z + 2800);
    next_y = r_u16(0x800FA33A);
    w_u16(0x800FA338, next_x);
    z = r_u16(0x800FA33C);
    w_u16(0x800FA33A, next_y + 600);
    w_u16(0x800FA33C, z + 2800);
}

static void game_radar_transform_geometry(uint32 first_player, uint32 rotation_matrix)
{
    uint32 index, point, player;
    for (index = 0; index < 8; ++index) {
        point = game_radar_vertex(index);
        player = index == 0 ? first_player : r_u32(0x80040FDC);
        ApplyMatrixSV((MATRIX *)psx_addr(player + 0x11C, 20), (SVECTOR *)psx_addr(point, 8), (SVECTOR *)psx_addr(point, 8));
    }
    game_radar_translate_geometry();
    SetRotMatrix((MATRIX *)psx_addr(rotation_matrix, 20));
}

static void game_radar_finish_line(uint32 packet)
{
    uint32 ordering;
    setLineF2((LINE_F2 *)psx_addr(packet, 16));
    packet = r_u32(0x80040690);
    SetSemiTrans(psx_addr(packet, 16), 1);
    ordering = r_u32(0x8004608C);
    packet = r_u32(0x80040690);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 16));
}

static uint32 game_radar_advance_line(uint32 color, uint32 write_color)
{
    uint32 previous = r_u32(0x80040690);
    w_u32(0x80040690, previous + 16);
    if (write_color != 0)
        w_u32(previous + 20, color);
    return previous;
}

static void game_radar_draw_line(uint32 packet, uint32 first, uint32 second, sint32 x0, sint32 y0, sint32 x1, sint32 y1)
{
    uint32 value;
    value = r_u16(first);
    w_u16(packet + 8, value + (uint32)x0);
    value = r_u16(first + 2);
    w_u16(packet + 10, value + (uint32)y0);
    value = r_u16(second);
    w_u16(packet + 12, value + (uint32)x1);
    value = r_u16(second + 2);
    w_u16(packet + 14, value + (uint32)y1);
    game_radar_finish_line(packet);
}

static uint32 game_radar_draw_diamond(uint32 color)
{
    uint32 packet, previous, line;
    static const sint16 offsets[4][4] = {
        {0, 2, 2, 0}, {0, -2, 2, 0}, {0, 2, -2, 0}, {0, -2, -2, 0}
    };
    packet = r_u32(0x80040690);
    w_u32(packet + 4, color);
    for (line = 0; line < 4; ++line) {
        if (line != 0) {
            previous = game_radar_advance_line(color, 1);
            packet = previous + 16;
        }
        game_radar_draw_line(packet, 0x800FA268, 0x800FA268, offsets[line][0], offsets[line][1], offsets[line][2], offsets[line][3]);
    }
    return game_radar_advance_line(0, 0);
}

static void game_radar_draw_plus(uint32 previous, uint32 color)
{
    w_u32(previous + 20, color);
    game_radar_draw_line(previous + 16, 0x800FA268, 0x800FA268, 0, 1, 0, -1);
    previous = game_radar_advance_line(color, 1);
    game_radar_draw_line(previous + 16, 0x800FA268, 0x800FA268, -1, 0, 1, 0);
    game_radar_advance_line(0, 0);
}

static uint32 game_radar_project_marker(uint32 distance, uint32 rotation_matrix)
{
    uint32 scale, shift, distance_product, x, y, z, x_product, y_product, z_product;
    uint32 scaled_distance, value, player, visible;
    sint32 projection, flags;
    scale = r_u32(0x800EF484);
    distance_product = distance * scale;
    x = r_u32(0x800FE330);
    x_product = (x << 8) * scale;
    y = r_u32(0x800FE334);
    y_product = (y << 8) * scale;
    z = r_u32(0x800FE338);
    z_product = (z << 8) * scale;
    shift = r_u32(0x800EF480) & 31;
    scaled_distance = (uint32)((sint32)distance_product >> shift);
    w_u16(0x800FA280, (uint32)((sint32)x_product >> shift));
    w_u16(0x800FA282, (uint32)((sint32)y_product >> shift));
    w_u16(0x800FA284, (uint32)((sint32)z_product >> shift));
    if ((sint32)scaled_distance > 500) {
        value = (sint32)x < 0 ? 0u - x : x;
        if ((sint32)value > 16000)
            goto halve_marker;
        value = (sint32)y < 0 ? 0u - y : y;
        if ((sint32)value > 16000)
            goto halve_marker;
        value = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)value <= 16000)
            goto normalize_marker;
halve_marker:
        x = r_u32(0x800FE330);
        y = r_u32(0x800FE334);
        z = r_u32(0x800FE338);
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
        w_u32(0x800FE330, x);
        value = (sint32)x < 0 ? 0u - x : x;
        w_u32(0x800FE334, y);
        w_u32(0x800FE338, z);
        if ((sint32)value > 16000)
            goto halve_marker;
        value = (sint32)y < 0 ? 0u - y : y;
        if ((sint32)value > 16000)
            goto halve_marker;
        value = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)value > 16000)
            goto halve_marker;
normalize_marker:
        vector_normalize((VECTOR *)psx_addr(0x800FE330, 12), (VECTOR *)psx_addr(0x800FE330, 12));
        x = r_u32(0x800FE330);
        x_product = x * 125;
        y = r_u32(0x800FE334);
        w_u16(0x800FA280, x_product >> 10);
        y_product = y * 125;
        z = r_u32(0x800FE338);
        w_u16(0x800FA282, y_product >> 10);
        w_u16(0x800FA284, (z * 125) >> 10);
    }
    player = r_u32(0x80040FDC);
    ApplyMatrixSV((MATRIX *)psx_addr(player + 0x11C, 20), (SVECTOR *)psx_addr(0x800FA280, 8), (SVECTOR *)psx_addr(0x800FA280, 8));
    z = (uint32)(sint32)r_s16(0x800FA284);
    x = r_u16(0x800FA280);
    y = r_u16(0x800FA282);
    visible = (~z) >> 31;
    w_u16(0x800FA280, x);
    w_u16(0x800FA282, y + 600);
    w_u16(0x800FA284, z + 2800);
    SetRotMatrix((MATRIX *)psx_addr(rotation_matrix, 20));
    RotTransPers((SVECTOR *)psx_addr(0x800FA280, 8), (sint32 *)psx_addr(0x800FA268, 4), &projection, &flags);
    return visible;
}

void game_draw_radar(uint32 alternate_origin, uint32 rotation_matrix)
{
    uint32 identifiers[3], timing, x, y, z, value, second, plane, vertex, axis;
    uint32 player, point, shift, scale, extent, negative, index, packet, previous;
    uint32 nearest, group, object, distance, visible, flags, color, plus_color, block;
    sint32 projection, projected_flags;
    static const sint16 planes[3][8][3] = {
        {{-500,0,-500},{0,0,-500},{500,0,-500},{-500,0,0},{500,0,0},{-500,0,500},{0,0,500},{500,0,500}},
        {{0,-500,-500},{0,0,-500},{0,500,-500},{0,-500,0},{0,500,0},{0,-500,500},{0,0,500},{0,500,500}},
        {{-500,-500,0},{0,-500,0},{500,-500,0},{-500,0,0},{500,0,0},{-500,500,0},{0,500,0},{500,500,0}}
    };
    static const sint16 cube[8][3] = {
        {-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},
        {-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}
    };
    static const uint32 screens[8] = {
        0x800FA268,0x800FA270,0x800FA274,0x800FA278,
        0x800FA288,0x800FA298,0x800FA2C4,0x800FA2D0
    };
    static const uint32 edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{4,0},{5,1},{6,2},{7,3}
    };
    FUNCTION_MARKER(0x800B6454, "GAME.OVL");
    identifiers[0] = r_u32(0x80075F18);
    identifiers[1] = r_u32(0x80075F1C);
    identifiers[2] = r_u32(0x80075F20);
    if (alternate_origin != 0) {
        timing = r_u32(0x800311C0);
        y = r_u32(0x800311B4);
        w_u32(0x8001024C, 78);
        x = 78;
        if (timing == 0)
            y += 5;
    } else {
        timing = r_u32(0x800311C0);
        if (timing != 0) {
            x = r_u32(0x80031198);
            y = r_u32(0x800311B4);
        } else {
            y = r_u32(0x800311B4);
            x = r_u32(0x80031198);
            y += 16;
        }
        w_u32(0x8001024C, x);
    }
    w_u32(0x80010250, y);
    system_set_projection_offset((sint32)x, (sint32)y);
    SetTransMatrix((MATRIX *)psx_addr(0x80124C08, 32));
    for (plane = 0; plane < 3; ++plane) {
        player = r_u32(0x80040FDC);
        for (vertex = 0; vertex < 8; ++vertex) {
            point = game_radar_vertex(vertex);
            for (axis = 0; axis < 3; ++axis)
                w_u16(point + axis * 2, (uint16)planes[plane][vertex][axis]);
        }
        if (plane == 0) {
            w_u16(0x800FA360, 0);
            w_u16(0x800FA362, 600);
            w_u16(0x800FA364, 2800);
        }
        game_radar_transform_geometry(player, rotation_matrix);
        game_draw_mesh_effect_quad(0x800FA280, 0x800FA290, 0x800FA2C8, 0x800FA360, 26, 0x600040, 1);
        game_draw_mesh_effect_quad(0x800FA290, 0x800FA2A0, 0x800FA360, 0x800FA2D8, 27, 0x600040, 1);
        game_draw_mesh_effect_quad(0x800FA2C8, 0x800FA360, 0x800FA2E0, 0x800FA2F0, 28, 0x600040, 1);
        game_draw_mesh_effect_quad(0x800FA360, 0x800FA2D8, 0x800FA2F0, 0x800FA338, 29, 0x600040, 1);
    }
    for (shift = 6; shift < 12; shift += 2) {
        scale = r_u32(0x800EF484);
        value = (500u << shift) * scale;
        second = r_u32(0x800EF480);
        extent = (uint32)((sint32)value >> (second & 31));
        negative = 0u - extent;
        if ((sint32)extent >= 500)
            continue;
        player = r_u32(0x80040FDC);
        for (vertex = 0; vertex < 8; ++vertex) {
            point = game_radar_vertex(vertex);
            for (axis = 0; axis < 3; ++axis)
                w_u16(point + axis * 2, cube[vertex][axis] < 0 ? negative : extent);
        }
        game_radar_transform_geometry(player, rotation_matrix);
        for (vertex = 0; vertex < 8; ++vertex)
            RotTransPers((SVECTOR *)psx_addr(game_radar_vertex(vertex), 8), (sint32 *)psx_addr(screens[vertex], 4), &projection, &projected_flags);
        packet = r_u32(0x80040690);
        w_u32(packet + 4, 0x2020);
        for (index = 0; index < 12; ++index) {
            if (index != 0) {
                previous = game_radar_advance_line(0x2020, 1);
                packet = previous + 16;
            }
            game_radar_draw_line(packet, screens[edges[index][0]], screens[edges[index][1]], 0, 0, 0, 0);
        }
        game_radar_advance_line(0, 0);
    }
    nearest = 0x7FFFFFFF;
    w_u32(0x800F3DE8, 0);
    for (group = 0; group < 3; ++group) {
        object = object_lookup(identifiers[group]);
        while (object != 0) {
            flags = r_u32(object + 0x114);
            if ((flags & 0x8080) != 0)
                goto next_radar_object;
            player = r_u32(0x80040FDC);
            x = r_u32(object + 8);
            value = r_u32(player + 8);
            x = (uint32)((sint32)(x - value) >> 8);
            w_u32(0x800FE330, x);
            y = r_u32(object + 12);
            value = r_u32(player + 12);
            y = (uint32)((sint32)(y - value) >> 8);
            w_u32(0x800FE334, y);
            z = r_u32(object + 16);
            value = r_u32(player + 16);
            z = (uint32)((sint32)(z - value) >> 8);
            w_u32(0x800FE338, z);
            distance = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z) << 8;
            if ((sint32)distance < (sint32)nearest && group == 0) {
                nearest = distance;
                w_u32(0x800F3DE8, object);
            }
            visible = game_radar_project_marker(distance, rotation_matrix);
            player = r_u32(0x80040FDC);
            if (object == player) {
                previous = game_radar_draw_diamond(0x2020);
                game_radar_draw_plus(previous, 0x4040);
            } else {
                if (group == 0) {
                    flags = r_u32(object + 0x114);
                    if ((flags & 0x20000000) == 0) {
                        value = r_u32(0x800311B8);
                        if ((value & 3) >= 3)
                            goto next_radar_object;
                        color = (flags & 0x800000) != 0 ? 0x7F7F7F : 0x7F2000;
                        plus_color = color << 1;
                    } else {
                        color = 0x808080;
                        plus_color = 0xFF60;
                    }
                } else {
                    color = group == 1 ? 0x8020 : 0x808080;
                    plus_color = 0xFF60;
                }
                previous = game_radar_draw_diamond(color);
                if (visible != 0)
                    game_radar_draw_plus(previous, plus_color);
            }
next_radar_object:
            object = r_u32(object + 0x1DC);
        }
    }
    for (index = 0; index < 2; ++index) {
        block = 0x8010ACD0 + index * 0x5C48;
        if (r_u32(block) == 0)
            continue;
        player = r_u32(0x80040FDC);
        x = r_u32(block + 0x38);
        value = r_u32(player + 8);
        x = (uint32)((sint32)(x - value) >> 8);
        w_u32(0x800FE330, x);
        y = r_u32(block + 0x3C);
        value = r_u32(player + 12);
        y = (uint32)((sint32)(y - value) >> 8);
        w_u32(0x800FE334, y);
        value = r_u32(player + 16);
        z = r_u32(block + 0x40);
        z = (uint32)((sint32)(z - value) >> 8);
        w_u32(0x800FE338, z);
        distance = SquareRoot0((sint32)(x * x + y * y + z * z)) << 8;
        visible = game_radar_project_marker(distance, rotation_matrix);
        previous = game_radar_draw_diamond(128);
        if (visible != 0)
            game_radar_draw_plus(previous, 255);
    }
    if (((sint32)nearest >> 6) < 500)
        value = 1024;
    else if (((sint32)nearest >> 8) < 500)
        value = 256;
    else if (((sint32)nearest >> 10) < 500)
        value = 64;
    else
        value = 16;
    w_u32(0x800EF488, value);
    value = r_u32(0x800EF488);
    second = r_u32(0x800EF484);
    if ((sint32)second < (sint32)value) {
        w_u32(0x800EF484, second + 16);
        second = r_u32(0x800EF484);
    }
    if ((sint32)value < (sint32)second)
        w_u32(0x800EF484, second - 16);
    x = r_u32(0x80031198);
    y = r_u32(0x800311B4);
    w_u32(0x8001024C, x);
    w_u32(0x80010250, y);
    system_set_projection_offset((sint32)x, (sint32)y);
}

sint32 game_project_camera_point_native(const VECTOR *point, uint32 screen, sint32 *depth)
{
    sint32 numerator, divisor, quotient;
    uint32 value;
    FUNCTION_MARKER(0x800C81C0, "GAME.OVL");
    numerator = (sint32)((uint32)point->vx * 200u);
    divisor = point->vz;
    if (divisor == 0 || (numerator == (sint32)0x80000000u && divisor == -1))
        abort();
    quotient = numerator / divisor;
    value = r_u32(0x80031198) + (uint32)quotient;
    w_u16(screen, value);
    numerator = (sint32)((uint32)point->vy * 200u);
    divisor = point->vz;
    if (divisor == 0 || (numerator == (sint32)0x80000000u && divisor == -1))
        abort();
    quotient = numerator / divisor;
    value = r_u32(0x800311B4) + (uint32)quotient;
    w_u16(screen + 2, value);
    value = (uint32)point->vz;
    *depth = (sint32)value;
    return (sint32)value;
}

void game_handle_debris_hit(uint32 object, uint32 position, uint32 velocity, uint32 other)
{
    sint32 count, player_model;
    FUNCTION_MARKER(0x800BF5FC, "GAME.OVL");
    if (r_u32(object) == 0)
        return;
    if (other != 0 && r_u32(other) == 0)
        return;
    player_model = r_s16(0x80059618);
    if (r_u32(object + 4) == (uint32)player_model) {
        count = random_between(6, 14);
        game_emit_velocity_fragments(position, velocity, (uint32)count);
    }
    game_emit_projectile_fragments(position, object, 10, 0);
}

void game_update_debris_fragment(uint32 object)
{
    uint32 x, y, z, vx, vy, vz, value, first, second, third, player, distance, delta;
    SVECTOR angles;
    FUNCTION_MARKER(0x800BF458, "GAME.OVL");
    x = r_u32(object + 8);
    vx = r_u32(object + 0x14);
    y = r_u32(object + 12);
    vy = r_u32(object + 0x18);
    w_u32(object + 8, x + vx);
    z = r_u32(object + 16);
    w_u32(object + 12, y + vy);
    vz = r_u32(object + 0x1C);
    value = r_u32(object + 0x9C);
    w_u32(object + 16, z + vz);
    if ((sint32)value < 0) {
        angles.vx = (sint16)r_u32(object + 0xA8);
        angles.vy = (sint16)r_u32(object + 0xAC);
        angles.vz = (sint16)r_u32(object + 0xB0);
        RotMatrix(&angles, (MATRIX *)psx_addr(0x800F4E58, 18));
        MulMatrix0((MATRIX *)psx_addr(object + 0x11C, 20),
            (MATRIX *)psx_addr(0x800F4E58, 20), (MATRIX *)psx_addr(object + 0x11C, 20));
        matrix_normalize(object + 0x11C, 1);
    }
    value = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, value);
    if (value == 0) {
        value = r_u32(object + 0x14);
        first = r_u32(object + 0xB4);
        second = r_u32(object + 0xB8);
        third = r_u32(object + 0xBC);
        w_u32(object + 0x14, value + first);
        value = r_u32(object + 0x18);
        first = r_u32(object + 0x1C);
        w_u32(object + 0x18, value + second);
        w_u32(object + 0x1C, first + third);
    }
    player = r_u32(0x80040FDC);
    x = r_u32(object + 8);
    first = r_u32(player + 8);
    delta = x - first;
    if ((sint32)delta < 0)
        delta = 0u - delta;
    distance = (sint32)delta > 0 ? delta : 0;
    y = r_u32(object + 12);
    first = r_u32(player + 12);
    delta = y - first;
    if ((sint32)delta < 0)
        delta = 0u - delta;
    if ((sint32)distance < (sint32)delta)
        distance = delta;
    value = r_u32(object + 0x9C);
    first = r_u32(object + 0x104);
    if ((sint32)first < (sint32)(0u - value) && r_u32(0x800EC884) == 0) {
        game_emit_debris_expiration_effects(object);
        game_mark_object_for_removal(object);
    }
}

void game_draw_eight_triangle_effect(uint32 x, uint32 y, sint32 radius, sint32 depth)
{
    uint32 seed, first, second, half, quarter, height, width, tip;
    uint32 left, right, top, bottom, near_left, near_right, near_top, near_bottom;
    uint32 vertices[8][6], packet, triangle, component;
    static const uint8 order[8][6] = {
        {0,1,2,4,5,3}, {0,1,2,4,5,3}, {0,1,3,4,5,2}, {0,1,2,3,4,5},
        {1,0,2,4,5,3}, {0,1,3,4,5,2}, {1,2,0,4,5,3}, {1,2,3,4,5,0}
    };
    static const uint8 offsets[6] = {8,10,16,18,24,26};
    FUNCTION_MARKER(0x800BC568, "GAME.OVL");
    if (radius < 2)
        radius = 2;
    half = (uint32)(radius >> 1);
    if (depth != 0x114F) {
        seed = r_u32(0x800EF50C);
        first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        second = (uint32)((sint32)first >> 1) ^ (first << 3);
        quarter = (uint32)(radius >> 2);
        w_u32(0x800EF50C, second);
        x += (first & 0x7FFF) % half - quarter;
        y += (second & 0x7FFF) % half - quarter;
    }
    height = (uint32)(radius >> 4);
    width = (uint32)(radius >> 6);
    tip = (uint32)(radius >> 7);
    left = x - height;
    right = x + height;
    top = y - height;
    bottom = y + height;
    near_left = x - width;
    near_right = x + width;
    near_top = y - tip;
    near_bottom = y + tip;
    vertices[0][0] = x; vertices[0][1] = top; vertices[0][2] = near_left; vertices[0][3] = near_top;
    vertices[1][0] = x; vertices[1][1] = top; vertices[1][2] = near_right; vertices[1][3] = near_top;
    vertices[2][0] = x; vertices[2][1] = bottom; vertices[2][2] = near_left; vertices[2][3] = near_bottom;
    vertices[3][0] = x; vertices[3][1] = bottom; vertices[3][2] = near_right; vertices[3][3] = near_bottom;
    vertices[4][0] = left; vertices[4][1] = y; vertices[4][2] = near_left; vertices[4][3] = near_top;
    vertices[5][0] = left; vertices[5][1] = y; vertices[5][2] = near_left; vertices[5][3] = near_bottom;
    vertices[6][0] = right; vertices[6][1] = y; vertices[6][2] = near_right; vertices[6][3] = near_top;
    vertices[7][0] = right; vertices[7][1] = y; vertices[7][2] = near_right; vertices[7][3] = near_bottom;
    for (triangle = 0; triangle < 8; ++triangle) {
        vertices[triangle][4] = x;
        vertices[triangle][5] = y;
        packet = r_u32(0x80040690);
        w_u32(0x80040690, packet + 28);
        w_u32(packet + 4, 0);
        w_u32(packet + 12, 0);
        w_u32(packet + 20, 0x204080);
        for (component = 0; component < 6; ++component)
            w_u16(packet + offsets[order[triangle][component]], vertices[triangle][order[triangle][component]]);
        SetPolyG3((POLY_G3 *)psx_addr(packet, 28));
        SetSemiTrans((POLY_G3 *)psx_addr(packet, 28), 1);
        render_insert_primitive(depth, packet);
    }
}

void game_emit_model_breakup(uint32 object)
{
    uint32 model, model_base, record, mesh, scale, shift, index, fragment, value;
    uint32 flags, x, y, z, count, seed, amplitude, next_seed, first, second, third;
    uint32 random_x, random_y, random_z, lifetime, product;
    VECTOR point;
    FUNCTION_MARKER(0x800BF6A4, "GAME.OVL");
    model = r_u32(object + 0x74);
    model_base = r_u32(0x80046178);
    count = r_u32(model_base + model * 132 + 0x70);
    if ((sint32)count <= 0)
        return;
    sound_play_distant_at_camera(object + 8, 4, 64);
    model = r_u32(object + 0x74);
    model_base = r_u32(0x80046178);
    count = r_u32(model_base + model * 132 + 0x70);
    if ((sint32)count <= 0)
        return;
    index = 0;
    do {
        record = model_base + model * 132;
        scale = r_u32(object + 0x84);
        mesh = r_u32(record + 12 + index * 4);
        value = (uint32)r_s16(mesh + 0x28);
        if ((sint32)scale > 0) {
            point.vx = (sint32)(value << (scale & 31));
            value = (uint32)r_s16(mesh + 0x2A);
            scale = r_u32(object + 0x84);
            point.vy = (sint32)(value << (scale & 31));
            value = (uint32)r_s16(mesh + 0x2C);
            scale = r_u32(object + 0x84);
            point.vz = (sint32)(value << (scale & 31));
        } else {
            shift = (sint32)scale < 0 ? 0u - scale : scale;
            point.vx = (sint32)value >> (shift & 31);
            scale = r_u32(object + 0x84);
            value = (uint32)r_s16(mesh + 0x2A);
            shift = (sint32)scale < 0 ? 0u - scale : scale;
            point.vy = (sint32)value >> (shift & 31);
            scale = r_u32(object + 0x84);
            value = (uint32)r_s16(mesh + 0x2C);
            shift = (sint32)scale < 0 ? 0u - scale : scale;
            point.vz = (sint32)value >> (shift & 31);
        }
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), &point, &point);
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        fragment = game_create_object(0, x + (uint32)point.vx,
            y + (uint32)point.vy, z + (uint32)point.vz, 6);
        model = r_u32(object + 0x74);
        w_u32(fragment + 0x74, model);
        model = r_u32(object + 0x74);
        model_base = r_u32(0x80046178);
        mesh = r_u32(model_base + model * 132 + index * 4 + 12);
        w_u32(fragment + 0x68, 1);
        w_u32(fragment + 0x64, mesh);
        scale = r_u32(object + 0x84);
        flags = r_u32(fragment + 0x114);
        w_u32(fragment + 0x40, 0x800BF458);
        w_u32(fragment + 0x44, 0x800BF5FC);
        w_u32(fragment + 0x58, 0x8002493C);
        w_u32(fragment + 0x88, index);
        w_u32(fragment + 0x238, 9999);
        w_u32(fragment + 0x234, 0);
        w_u32(fragment + 0x114, flags | 0x2894);
        w_u32(fragment + 0x84, scale);
        vector_normalize(&point, &point);
        scale = r_u32(object + 0x84);
        if ((sint32)scale > 0) {
            seed = r_u32(0x800EF50C);
            scale = r_u32(object + 0x84);
            first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
            amplitude = ((first & 31) + 4) << (scale & 31);
        } else {
            seed = r_u32(0x800EF50C);
            first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
            amplitude = (first & 31) + 4;
        }
        w_u32(0x800EF50C, first);
        x = r_u32(object + 0x14);
        seed = r_u32(0x800EF50C);
        next_seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        w_u32(fragment + 0x14, (uint32)((sint32)x >> 8));
        y = r_u32(object + 0x18);
        w_u32(fragment + 0x18, (uint32)((sint32)y >> 8));
        z = r_u32(object + 0x1C);
        w_u32(fragment + 0xFC, 255);
        w_u32(fragment + 0x100, 0);
        w_u32(fragment + 0x104, (next_seed & 255) + 90);
        w_u32(fragment + 0x1C, (uint32)((sint32)z >> 8));
        product = (uint32)point.vx * amplitude;
        w_u32(fragment + 0xB4, (uint32)((sint32)product >> 12));
        product = (uint32)point.vy * amplitude;
        w_u32(fragment + 0xB8, (uint32)((sint32)product >> 12));
        product = (uint32)point.vz * amplitude;
        w_u32(0x800EF50C, next_seed);
        w_u32(fragment + 0x6C, 0);
        w_u32(fragment + 0xD8, 5000000);
        w_u32(fragment + 4, 0);
        w_u32(fragment + 0x9C, 60);
        w_u32(fragment + 0xBC, (uint32)((sint32)product >> 12));
        matrix_copy_rotation(fragment + 0x11C, object + 0x11C);
        value = r_u32(object + 0x20);
        w_u32(fragment + 0x20, value);
        seed = r_u32(0x800EF50C);
        first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        random_x = first & 0x7FFF;
        value = r_u32(object + 0x24);
        w_u32(fragment + 0x24, value);
        value = r_u32(object + 0x28);
        w_u32(fragment + 0x28, value);
        lifetime = r_u32(object + 0x9C);
        w_u32(fragment + 0x3C, 0x800BD868);
        second = (uint32)((sint32)first >> 1) ^ (first << 3);
        random_y = second & 0x7FFF;
        w_u32(fragment + 0x1E4, 0x800EEC5C);
        w_u16(fragment + 0x24C, lifetime);
        third = (uint32)((sint32)second >> 1) ^ (second << 3);
        random_z = third & 0x7FFF;
        w_u32(fragment + 0xA8, random_x % 5 - 2);
        w_u32(fragment + 0xAC, random_y % 5 - 2);
        w_u32(fragment + 0xB0, random_z % 5 - 2);
        model = r_u32(object + 0x74);
        model_base = r_u32(0x80046178);
        count = r_u32(model_base + model * 132 + 0x70);
        ++index;
        w_u32(0x800EF50C, first);
        w_u32(0x800EF50C, second);
        w_u32(0x800EF50C, third);
    } while ((sint32)index < (sint32)count);
}

void game_draw_debris_fragment(uint32 object)
{
    uint32 hidden, mesh, clock, value, limit, color, index, offset, face, seed, weight;
    uint32 axis, side, vertex, base, first, second, third, product, destination, packet, table;
    uint32 scale, positive, shift, component, products[3], x, y, z, vx, vy, vz;
    sint32 timer, projection, flags, screen_first, screen_second, depth;
    VECTOR transformed;
    FUNCTION_MARKER(0x800BD868, "GAME.OVL");
    hidden = object_draw_visible(object);
    timer = r_s32(object + 0x9C);
    if (timer < -49)
        goto emit_particles;
    clock = r_u32(0x800311B8);
    mesh = r_u32(object + 0x64);
    if ((clock & 1) == 0) {
        value = r_u32(object + 0x100);
        w_u32(object + 0x100, value + 1);
    }
    value = r_u32(object + 0x100);
    limit = r_u32(mesh + 0x30);
    if ((sint32)limit < (sint32)value) {
        value = r_u32(object + 0x9C);
        w_u32(object + 0x100, limit);
        if (value + 9 < 9) {
            game_set_screen_tint(255, 255, 255);
            timer = r_s32(object + 0x9C);
            if (timer >= -2) {
                sound_play_distant_at_camera(object + 8, 4, 128);
                sound_play_distant_at_camera(object + 8, 4, 64);
            }
        }
    }
    if (hidden != 0)
        return;
    timer = r_s32(object + 0x9C);
    if (timer > 0)
        color = 0xFFFFFF;
    else {
        color = ((uint32)timer + 50) * 5;
        color = color + (color << 8) + (color << 16);
    }
    if (r_s32(object + 0x100) <= 0)
        return;
    index = 0;
    offset = 0;
    do {
        face = r_u32(mesh + 0x10) + offset;
        seed = r_u32(0x800EF50C);
        seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        w_u32(0x800EF50C, seed);
        if ((seed & 7) == 0 && (index & 1) != 0)
            weight = 350;
        else
            weight = 300 - ((r_u32(0x800311B8) & 1) << 2);
        for (axis = 0; axis < 6; axis += 2) {
            for (side = 0; side < 2; ++side) {
                vertex = r_u32(face + 4 + side * 4);
                base = r_u32(mesh);
                first = r_u16(base + vertex * 8 + axis);
                destination = 0x80108918 + side * 8 + axis;
                w_u16(destination, first);
                vertex = r_u32(face + 12);
                base = r_u32(mesh);
                second = (uint32)r_s16(base + vertex * 8 + axis);
                product = ((uint32)(sint32)(sint16)first - second) * weight;
                w_u16(destination, first + (uint32)((sint32)product >> 8));
            }
        }
        RotTransPers((SVECTOR *)psx_addr(0x80108918, 8), &screen_first, &projection, &flags);
        depth = RotTransPers((SVECTOR *)psx_addr(0x80108920, 8), &screen_second, &projection, &flags);
        packet = r_u32(0x80040690);
        shift = r_u32(0x80053670) + 2;
        depth = (sint32)((uint32)depth << (shift & 31));
        w_u32(0x80040690, packet + 36);
        w_u32(packet + 4, 0);
        w_u32(packet + 12, 0);
        w_u32(packet + 20, color);
        w_u32(packet + 28, color);
        SetPolyG4((POLY_G4 *)psx_addr(packet, 36));
        SetSemiTrans((POLY_G4 *)psx_addr(packet, 36), 1);
        w_u32(packet + 8, (uint32)screen_first);
        w_u32(packet + 16, (uint32)screen_second);
        vertex = r_u32(face + 4);
        table = r_u32(0x80046088);
        value = r_u32(table + vertex * 4);
        offset += 60;
        w_u32(packet + 24, value);
        vertex = r_u32(face + 8);
        value = r_u32(table + vertex * 4);
        w_u32(packet + 32, value);
        render_insert_primitive(depth, packet);
        limit = r_u32(object + 0x100);
        ++index;
    } while ((sint32)index < (sint32)limit);
    return;
emit_particles:
    if (hidden != 0)
        return;
    mesh = r_u32(object + 0x64);
    if (r_s32(mesh + 0x30) <= 0)
        return;
    index = 0;
    offset = 0;
    do {
        seed = r_u32(0x800EF50C);
        seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        w_u32(0x800EF50C, seed);
        if ((seed & 3) != 0)
            goto next_face;
        seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        weight = seed & 0x7FF;
        w_u32(0x800EF50C, seed);
        face = r_u32(mesh + 0x10);
        scale = r_u32(object + 0x84);
        face += offset;
        positive = (sint32)scale > 0;
        for (component = 0; component < 6; ++component) {
            vertex = r_u32(face + (component < 3 ? 4 : 12));
            base = r_u32(mesh);
            if (!positive && component != 0)
                scale = r_u32(object + 0x84);
            value = (uint32)r_s16(base + vertex * 8 + (component % 3) * 2);
            if (positive && component != 0)
                scale = r_u32(object + 0x84);
            if (positive)
                value <<= scale & 31;
            else {
                shift = (sint32)scale < 0 ? 0u - scale : scale;
                value = (uint32)((sint32)value >> (shift & 31));
            }
            destination = component < 3 ? 0x801089E0 + component * 4 : 0x801089F8 + (component - 3) * 4;
            w_u32(destination, value);
        }
        for (axis = 0; axis < 3; ++axis) {
            first = r_u32(0x801089E0 + axis * 4);
            second = r_u32(0x801089F8 + axis * 4);
            products[axis] = first * weight + second * (4096 - weight);
        }
        for (axis = 0; axis < 3; ++axis)
            w_u32(0x801089E0 + axis * 4, (uint32)((sint32)products[axis] >> 12));
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20),
            (VECTOR *)psx_addr(0x801089E0, 12), &transformed);
        w_u32(0x801089E0, (uint32)transformed.vx);
        w_u32(0x801089E4, (uint32)transformed.vy);
        w_u32(0x801089E8, (uint32)transformed.vz);
        vertex = r_u32(face + 0x14);
        base = r_u32(mesh + 8);
        ApplyMatrixSV((MATRIX *)psx_addr(object + 0x11C, 20),
            (SVECTOR *)psx_addr(base + vertex * 8, 8), (SVECTOR *)psx_addr(0x8010AB50, 8));
        first = r_u32(0x801089E0);
        x = r_u32(object + 8);
        second = r_u32(0x801089E4);
        y = r_u32(object + 12);
        third = r_u32(0x801089E8);
        z = r_u32(object + 16);
        vz = (uint32)r_s16(0x8010AB54);
        vx = (uint32)r_s16(0x8010AB50);
        vy = (uint32)r_s16(0x8010AB52);
        game_emit_slow_particles(1, first + x, second + y, third + z, vx, vy, vz);
next_face:
        limit = r_u32(mesh + 0x30);
        ++index;
        offset += 60;
    } while ((sint32)index < (sint32)limit);
}

static sint32 game_hud_divide(uint32 numerator, uint32 denominator)
{
    if (denominator == 0 || (numerator == 0x80000000u && denominator == 0xFFFFFFFFu))
        abort();
    return (sint32)numerator / (sint32)denominator;
}

static const char *game_hud_text(uint32 address)
{
    uint32 length = 0;
    while (r_u8(address + length) != 0)
        ++length;
    return (const char *)psx_addr(address, (size_t)length + 1);
}

static void game_hud_finish_line(uint32 packet)
{
    uint32 ordering;
    setLineF2((LINE_F2 *)psx_addr(packet, 16));
    packet = r_u32(0x80040690);
    SetSemiTrans(psx_addr(packet, 16), 1);
    ordering = r_u32(0x8004608C);
    packet = r_u32(0x80040690);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 4));
}

static void game_hud_draw_direction(uint32 x, uint32 y, uint32 angle, uint32 color)
{
    uint32 mode, packet, value, outer, side;
    mode = r_u32(0x800311C0);
    packet = r_u32(0x80040690);
    w_u32(packet + 4, color);
    for (side = 0; side < 2; ++side) {
        outer = side == 0 ? angle - 256 : angle + 256;
        value = (uint32)rcos((sint32)outer) * 30;
        packet = r_u32(0x80040690);
        w_u16(packet + 8, x + (uint32)((sint32)value >> 12));
        value = (uint32)rsin((sint32)outer) * (mode != 0 ? 36 : 30);
        packet = r_u32(0x80040690);
        w_u16(packet + 10, y + (uint32)((sint32)value >> 13));
        value = (uint32)rcos((sint32)angle) * 40;
        packet = r_u32(0x80040690);
        w_u16(packet + 12, x + (uint32)((sint32)value >> 12));
        value = (uint32)rsin((sint32)angle) * (mode != 0 ? 47 : 40);
        packet = r_u32(0x80040690);
        w_u16(packet + 14, y + (uint32)((sint32)value >> 13));
        game_hud_finish_line(packet);
        packet = r_u32(0x80040690);
        w_u32(0x80040690, packet + 16);
        if (side == 0)
            w_u32(packet + 20, color);
    }
}

static void game_hud_draw_gauge_quad(uint32 angle, uint32 next, uint32 outer, uint32 current_color, uint32 previous_color)
{
    uint32 packet, mode, outer_y, inner_y, vertex, value, center, ordering, phase, radius;
    packet = r_u32(0x80040690);
    w_u32(0x80040690, packet + 36);
    w_u32(packet + 20, current_color << 8);
    w_u32(packet + 4, 0);
    w_u32(packet + 12, 0);
    w_u32(packet + 28, previous_color << 8);
    SetPolyG4((POLY_G4 *)psx_addr(packet, 36));
    SetSemiTrans(psx_addr(packet, 36), 1);
    mode = r_u32(0x800311C0);
    outer_y = mode != 0 ? (outer == 50 ? 57 : 39) : outer;
    inner_y = mode != 0 ? 46 : 40;
    for (vertex = 0; vertex < 4; ++vertex) {
        phase = (vertex & 1) != 0 ? next : angle;
        radius = vertex < 2 ? outer : 40;
        value = (uint32)rcos((sint32)phase) * radius;
        center = r_u32(0x80031198);
        w_u16(packet + 8 + vertex * 8, center + (uint32)((sint32)value >> 12));
        radius = vertex < 2 ? outer_y : inner_y;
        value = (uint32)rsin((sint32)phase) * radius;
        center = r_u32(0x800311B4);
        w_u16(packet + 10 + vertex * 8, center + (uint32)((sint32)value >> 13));
    }
    ordering = r_u32(0x8004608C);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 4));
}

static void game_hud_update_weapon_fade(uint32 bank, uint32 selected_slot, uint32 locked_states)
{
    uint32 state, value, next, slot;
    state = r_u32(bank);
    if (state == 0) {
        value = r_u32(bank + 8);
        if ((sint32)value > 0) {
            next = value - 20;
            w_u32(bank + 8, next);
            if ((sint32)next < 0)
                w_u32(bank + 8, 0);
            return;
        }
    }
    state = r_u32(bank);
    if (state == 1) {
        value = r_u32(bank + 8);
        if ((sint32)value < 255)
            goto increase_fade;
        state = r_u32(bank);
    }
    if (state == 3) {
        value = r_u32(bank + 8);
        if ((sint32)value < 255) {
            next = value + 20;
            w_u32(bank + 8, next);
            if ((sint32)next >= 256)
                w_u32(bank + 8, 255);
            w_u32(bank + 4, 64);
            return;
        }
        if (value == 255) {
            next = r_u32(bank + 4) - 1;
            w_u32(bank + 4, next);
            if ((sint32)next <= 0)
                w_u32(bank, 0);
            return;
        }
    }
    state = r_u32(bank);
    if (state == 2) {
        value = r_u32(bank + 8);
        if ((sint32)value < 255)
            goto increase_fade;
        value = r_u32(bank + 12);
        if ((sint32)value < 255) {
            next = value + 20;
            w_u32(bank + 12, next);
            if ((sint32)next < 255)
                return;
        }
        slot = r_u32(selected_slot);
        w_u32(bank, 3);
        w_u32(bank + 12, 0);
        w_u32(bank + 4, 64);
        w_u32(bank + 16, slot);
        return;
    }
    if (locked_states == 0 || (state != 4 && state != 6 && state != 5))
        return;
    value = r_u32(bank + 8);
increase_fade:
    next = value + 20;
    w_u32(bank + 8, next);
    if ((sint32)next >= 256)
        w_u32(bank + 8, 255);
}

static void game_hud_transform_scratch(uint32 matrix)
{
    VECTOR result;
    ApplyMatrixLV((MATRIX *)psx_addr(matrix, 20), (VECTOR *)psx_addr(0x800FE330, 12), &result);
    w_u32(0x800FE330, (uint32)result.vx);
    w_u32(0x800FE334, (uint32)result.vy);
    w_u32(0x800FE338, (uint32)result.vz);
}

static void game_hud_scale_first_row(uint32 matrix)
{
    sint32 x, y, z;
    x = r_s16(matrix) * 8;
    y = r_s16(matrix + 2) * 8;
    z = r_s16(matrix + 4) * 8;
    w_u16(matrix, (uint32)(x / 5));
    w_u16(matrix + 2, (uint32)(y / 5));
    w_u16(matrix + 4, (uint32)(z / 5));
}

static void game_hud_prepare_weapon_icon(uint32 rotation, uint32 x, uint32 negative)
{
    uint32 y, z, clock, angle;
    w_u32(0x800FE330, x);
    w_u32(0x800FE334, 850);
    w_u32(0x800FE338, 1800);
    game_hud_transform_scratch(rotation);
    w_u16(0x800FA280, 0);
    x = r_u32(0x800FE330);
    y = r_u32(0x800FE334);
    z = r_u32(0x800FE338);
    clock = r_u32(0x800311B8);
    w_u32(0x80124C1C, x);
    angle = negative != 0 ? 0u - clock : clock;
    w_u32(0x80124C20, y);
    w_u32(0x80124C24, z);
    w_u16(0x800FA282, angle << 6);
    w_u16(0x800FA284, clock << 3);
    RotMatrix((SVECTOR *)psx_addr(0x800FA280, 6), (MATRIX *)psx_addr(0x800F4238, 18));
    MulMatrix0((MATRIX *)psx_addr(0x800F4238, 20), (MATRIX *)psx_addr(rotation, 20), (MATRIX *)psx_addr(0x800F4238, 20));
    game_hud_scale_first_row(0x800F4238);
    SetRotMatrix((MATRIX *)psx_addr(0x800F4238, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80124C08, 32));
}

static uint32 game_hud_meter_index(uint32 value)
{
    sint32 index = (sint32)(value * 14) >> 8;
    if (index >= 15)
        index = 14;
    if (index < 0)
        index = 0;
    return (uint32)index;
}

void game_draw_flight_hud(uint32 cockpit_rotation, uint32 cockpit_transform, uint32 view_rotation)
{
    uint32 saved_x = r_u32(0x8001024C);
    uint32 saved_y, x, y, z, a, b, c, player, camera, target, angle = 0, indicator;
    uint32 packet, ordering, value, index, color, previous, next, alternate = 0;
    uint32 mode, model_left, model_right, weapon_color, binding, data, tex, capacity;
    uint32 phase, text;
    sint32 projection, flags, fraction, row_x, row_y, row_z;
    VECTOR point;
    FUNCTION_MARKER(0x800B8E4C, "GAME.OVL");
    saved_y = r_u32(0x80010250);
    system_read_counter_one();
    if (r_u32(0x800311B8) == 2) {
        sound_play_sample_pitch(15, 110);
        sound_play_sample_pitch(15, 110);
        sound_play_sample_pitch(15, 110);
        sound_play_sample_pitch(15, 110);
    }
    if (r_s32(0x800313DC) < 4) {
        x = r_u32(0x80031198);
        y = r_u32(0x800311B4);
    } else {
        w_u32(0x80034D6C, 0);
        w_u32(0x80034D70, 0);
        w_u32(0x80034D74, 0);
        SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
        player = r_u32(0x80040FDC);
        camera = r_u32(0x80040C94);
        for (index = 0; index < 3; ++index) {
            a = r_u16(player + 0x128 + index * 2);
            b = r_u32(player + 8 + index * 4);
            c = r_u32(camera + 8 + index * 4);
            w_u16(0x800FA280 + index * 2, b + (a << 2) - c);
        }
        RotTransPers((SVECTOR *)psx_addr(0x800FA280, 8), (sint32 *)psx_addr(0x800FA268, 4), &projection, &flags);
        x = (uint32)r_s16(0x800FA268);
        y = (uint32)r_s16(0x800FA26A);
    }
    target = r_u32(0x800F3DE8);
    if (target != 0) {
        camera = r_u32(0x80040C94);
        a = r_u32(target + 8); b = r_u32(camera + 8); point.vx = (sint32)(a - b);
        a = r_u32(target + 12); b = r_u32(camera + 12); point.vy = (sint32)(a - b);
        a = r_u32(target + 16); b = r_u32(camera + 16); point.vz = (sint32)(a - b);
        ApplyMatrixLV((MATRIX *)psx_addr(0x80034D58, 20), &point, &point);
        a = (uint32)point.vx;
        b = (uint32)point.vy;
        indicator = 1;
        if (point.vz > 0) {
            game_project_camera_point_native(&point, 0x800FA268, &projection);
            c = r_u16(0x800FA268) - x;
            value = r_u16(0x800FA26A) - y;
            w_u16(0x800FA268, c);
            w_u16(0x800FA26A, value);
            c = (uint32)(sint32)(sint16)c;
            value = (uint32)(sint32)(sint16)value;
            projection = SquareRoot0((sint32)(c * c + value * value));
            if (projection < 40)
                indicator = 0;
        }
        if (indicator != 0) {
            angle = (uint32)ratan2((sint32)a, (sint32)b);
            game_hud_draw_direction(x, y, angle, 0xFF0000);
        }
        for (index = 0; index < 16; ++index) {
            mode = r_u32(0x800311C0);
            packet = r_u32(0x80040690);
            w_u32(packet + 4, 0xFF0000);
            phase = angle + index * 256;
            value = (uint32)rcos((sint32)phase) * 25;
            packet = r_u32(0x80040690);
            w_u16(packet + 8, x + (uint32)((sint32)value >> 12));
            value = (uint32)rsin((sint32)phase) * (mode != 0 ? 30 : 25);
            packet = r_u32(0x80040690);
            w_u16(packet + 10, y + (uint32)((sint32)value >> 13));
            SetTile1((TILE_1 *)psx_addr(packet, 12));
            packet = r_u32(0x80040690);
            SetSemiTrans(psx_addr(packet, 12), 1);
            ordering = r_u32(0x8004608C);
            packet = r_u32(0x80040690);
            AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 4));
            packet = r_u32(0x80040690);
            w_u32(0x80040690, packet + 12);
        }
    }
    target = r_u32(0x800EF4E0);
    if (target != 0 && r_u32(0x800EF4BC) == 5) {
        a = (uint32)r_s16(target + 0x108);
        b = (uint32)r_s16(target + 0x10A);
        angle = (uint32)ratan2((sint32)(a - x), (sint32)(b - y));
        game_hud_draw_direction(x, y, angle, 255);
    }
    if (r_s32(0x800313DC) < 4) {
        value = r_u32(0x800EC8A0);
        if ((sint32)value > 0) {
            a = r_u32(0x800EC89C);
            fraction = game_hud_divide(value << 12, a);
            value = 1;
            if (fraction >= 4096) {
                fraction = 4096;
                value = (r_u32(0x800311B8) & 3) < 2 ? 255 : 0;
            }
            if ((sint32)value > 0) {
                if (fraction == 0)
                    fraction = 1;
                color = fraction >= 4096 ? 0x5050 : 0;
                if (fraction > 0) {
                    angle = 0;
                    next = 128;
                    do {
                        previous = color;
                        color = fraction >= 4096 ? 0x5050 : (uint32)game_hud_divide(angle << 7, (uint32)fraction);
                        game_hud_draw_gauge_quad(angle, next, 50, color, previous);
                        game_hud_draw_gauge_quad(angle, next, 35, color, previous);
                        next += 256;
                        angle += 256;
                    } while ((sint32)angle < fraction);
                }
            }
        }
    }
    w_u32(0x800FE330, 0);
    w_u32(0x800FE334, 300);
    w_u32(0x800FE338, 0);
    game_hud_transform_scratch(view_rotation);
    x = r_u32(0x800FE330); y = r_u32(0x800FE334); z = r_u32(0x800FE338);
    value = r_u32(0x8001020C);
    w_u32(0x80124C1C, x); w_u32(0x80124C20, y); w_u32(0x80124C24, z);
    if (value != 0)
        goto restore_projection;
    system_read_counter_one();
    target = r_u32(0x800EF4E0);
    w_u32(0x800EF4E8, target);
    if (r_s32(0x800313DC) < 4 && r_u32(0x800ECE70) != 0)
        value = 56;
    else {
        alternate = 1;
        w_u32(0x800ECE84, 0);
        value = 200;
    }
    w_u32(0x80031120, value);
    value = r_u32(0x800EF4A4);
    w_u32(0x800EF4A4, value + 1);
    SetRotMatrix((MATRIX *)psx_addr(view_rotation, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80124C08, 32));
    game_draw_target_lock_reticle();
    if (r_s32(0x800313DC) < 4)
        game_draw_horizontal_reticle(r_u32(0x80040690));
    game_draw_radar(alternate, view_rotation);
    SetRotMatrix((MATRIX *)psx_addr(view_rotation, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80124C08, 32));
    game_hud_update_weapon_fade(0x800EF4A8, 0x800EC874, 0);
    game_hud_update_weapon_fade(0x800EF4BC, 0x800EC878, 1);
    RotMatrix((SVECTOR *)psx_addr(0x800EF4EC, 6), (MATRIX *)psx_addr(0x80124C08, 18));
    row_x = r_s16(0x80124C08) * 8;
    row_y = r_s16(0x80124C0A) * 8;
    value = r_u16(0x800EF4EC);
    w_u16(0x800EF4EC, value + 10);
    a = r_u16(0x800EF4EE);
    row_z = r_s16(0x80124C0C) * 8;
    b = r_u16(0x800EF4F0);
    w_u16(0x800EF4EE, a + 20);
    mode = r_u32(0x800311C0);
    w_u16(0x800EF4F0, b + 30);
    w_u16(0x80124C08, (uint32)(row_x / 5));
    w_u16(0x80124C0A, (uint32)(row_y / 5));
    w_u16(0x80124C0C, (uint32)(row_z / 5));
    if (mode != 0) {
        a = (uint32)r_s16(0x80124C0E);
        value = r_u32(0x800311C4);
        a *= value;
        b = (uint32)r_s16(0x80124C10) * value;
        c = (uint32)r_s16(0x80124C12) * value;
        w_u16(0x80124C0E, (uint32)((sint32)a / 240));
        w_u16(0x80124C10, (uint32)((sint32)b / 240));
        w_u16(0x80124C12, (uint32)((sint32)c / 240));
    }
    w_u32(0x80124C1C, 0); w_u32(0x80124C20, 0); w_u32(0x80124C24, 40000);
    SetRotMatrix((MATRIX *)psx_addr(0x80124C08, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80124C08, 32));
    a = r_u32(0x800EC874); b = r_u32(0x800EC878);
    model_left = r_u32(0x80119C54 + a * 52) - 1;
    model_right = r_u32(0x80119C54 + b * 52) - 1;
    value = r_u32(0x800EF4BC);
    if (value == 4 || value == 6)
        weapon_color = r_u32(0x800EF4C4);
    else if (value == 5) {
        if ((r_u32(0x800311B8) & 1) != 0)
            weapon_color = r_u32(0x800EF4C4);
        else {
            value = r_u32(0x800EF4C4);
            weapon_color = value + (value << 8);
        }
    } else {
        value = r_u32(0x800EF4C4);
        weapon_color = (uint32)((sint32)value >> 1) << 8;
    }
    if ((alternate & 255) != 0) {
        mode = r_u32(0x800311C0);
        y = mode != 0 ? (uint32)-58 : (uint32)-32;
        x = r_u32(0x80031198);
        w_u32(0x80010250, y); w_u32(0x8001024C, x);
        system_set_projection_offset((sint32)x, (sint32)y);
    } else if (r_u32(0x800311C0) != 0) {
        y = r_u32(0x800311B4);
        x = r_u32(0x80031198);
        y -= 20;
        w_u32(0x8001024C, x); w_u32(0x80010250, y);
        system_set_projection_offset((sint32)x, (sint32)y);
    }
    value = r_u32(0x800EF4B0);
    w_u8(0x8004617C, 128); w_u8(0x80033D38, 128); w_u8(0x80053680, 128);
    w_u32(0x80033D2C, 242);
    if ((sint32)value > 0 && r_u32(0x80127038) != 0) {
        game_hud_prepare_weapon_icon(view_rotation, (uint32)-900, 1);
        value = r_u32(0x800EF4B0);
        a = r_u32(0x8010AC90 + model_left * 4);
        mesh_render_adaptive_alternate(a, (uint32)((sint32)value >> 1) << 8);
        mode = r_u32(0x800311C0);
        w_u32(0x80053688, 32);
        w_u32(0x8005368C, alternate != 0 ? 64 : 220);
        if (mode != 0)
            w_u32(0x8005368C, alternate != 0 ? 68 : 233);
        text = r_u32(0x800F2514 + model_left * 4);
        font_draw_current(game_hud_text(text));
    }
    if (r_s32(0x800EF4C4) > 0 && r_u32(0x80127038) != 0) {
        game_hud_prepare_weapon_icon(view_rotation, 900, 0);
        a = r_u32(0x8010AC90 + model_right * 4);
        mesh_render_adaptive_alternate(a, weapon_color);
        text = 0x800F2514 + model_right * 4;
        value = r_u32(text);
        value = text_measure_width(value);
        w_u32(0x80053688, 480 - value);
        mode = r_u32(0x800311C0);
        w_u32(0x8005368C, alternate != 0 ? 64 : 220);
        if (mode != 0)
            w_u32(0x8005368C, alternate != 0 ? 68 : 233);
        value = r_u32(text);
        font_draw_current(game_hud_text(value));
    }
    player = r_u32(0x80040FDC);
    a = r_u32(player + 0x14); b = r_u32(player + 0x18); c = r_u32(player + 0x1C);
    value = vector_length_approx((sint32)a >> 8, (sint32)b >> 8, (sint32)c >> 8);
    index = game_hud_meter_index((uint32)((sint32)value >> 1));
    binding = r_u32(0x80123F48);
    tex = r_u16(0x800F33B8 + index * 2);
    player = r_u32(0x80040FDC);
    data = r_u32(binding + 16);
    w_u16(data + 0x11E8, tex);
    value = r_u32(player + 0x234);
    if ((sint32)value > 0) {
        data = r_u32(binding + 16);
        w_u32(data + 0xEE8, 0xFFFF8080);
        value *= 330;
        data = r_u32(binding + 16);
        tex = r_u16(0x80108984);
    } else {
        value = r_u32(player + 0x238);
        data = r_u32(binding + 16);
        w_u32(data + 0xEE8, 0xFF8080FF);
        value *= 330;
        data = r_u32(binding + 16);
        tex = r_u16(0x80108986);
    }
    index = game_hud_meter_index((uint32)((sint32)value >> 8));
    w_u16(data + 0x1008, tex);
    binding = r_u32(0x80123F48);
    data = r_u32(binding + 16);
    tex = r_u16(0x800F33B8 + index * 2);
    w_u16(data + 0xEDC, tex);
    target = r_u32(0x800EF4E8);
    if (target != 0) {
        value = r_u32(target + 0x234);
        if ((sint32)value > 0) {
            a = r_u32(target + 4);
            binding = r_u32(0x800F3D9C);
            capacity = r_u32(0x800536A4 + a * 60);
            data = r_u32(binding + 16);
            w_u32(data + 0xEE8, 0xFFFF8080);
            data = r_u32(binding + 16);
            tex = r_u16(0x80108984);
        } else {
            a = r_u32(target + 4);
            value = r_u32(target + 0x238);
            binding = r_u32(0x800F3D9C);
            capacity = r_u32(0x800536A8 + a * 60);
            data = r_u32(binding + 16);
            w_u32(data + 0xEE8, 0xFF8080FF);
            data = r_u32(binding + 16);
            tex = r_u16(0x80108986);
        }
        fraction = game_hud_divide(value << 8, capacity);
        w_u16(data + 0x1008, tex);
        index = game_hud_meter_index((uint32)fraction);
        binding = r_u32(0x800F3D9C);
        tex = r_u16(0x800F33B8 + index * 2);
        target = r_u32(0x800EF4E8);
        data = r_u32(binding + 16);
        w_u16(data + 0xEDC, tex);
        a = r_u32(target + 4);
        value = r_u32(target + 0x244);
        capacity = r_u32(0x800536AC + a * 60);
        fraction = game_hud_divide(value << 8, capacity);
        index = game_hud_meter_index((uint32)fraction);
        data = r_u32(binding + 16);
        tex = r_u16(0x800F33B8 + index * 2);
        w_u16(data + 0x11E8, tex);
    } else {
        binding = r_u32(0x800F3D9C);
        tex = r_u16(0x800F33B8);
        data = r_u32(binding + 16);
        w_u16(data + 0xEDC, tex);
        data = r_u32(binding + 16);
        tex = r_u16(0x800F33B8);
        w_u16(data + 0x11E8, tex);
    }
    index = 0;
    if (r_u32(0x80127038) != 0) {
        a = r_u32(0x800EC874);
        value = r_u32(0x80119C4C + a * 52);
        capacity = r_u32(0x80119C44 + a * 52);
        index = (uint32)game_hud_divide(value * 6, capacity);
        if ((sint32)index >= 7)
            index = 6;
    }
    binding = r_u32(0x80123F48);
    data = r_u32(binding + 16);
    tex = r_u16(0x80108968 + index * 2);
    w_u16(data + 0xFCC, tex);
    index = 0;
    if (r_u32(0x80127040) != 0) {
        a = r_u32(0x800EC878);
        index = r_u32(0x80119C58 + a * 52);
        if ((sint32)index >= 7)
            index = 6;
    }
    binding = r_u32(0x800F3D9C);
    data = r_u32(binding + 16);
    tex = r_u16(0x80108968 + index * 2);
    w_u16(data + 0xFCC, tex);
    w_u32(0x800FE330, (uint32)-2500); w_u32(0x800FE334, 650); w_u32(0x800FE338, 2500);
    game_hud_transform_scratch(view_rotation);
    x = r_u32(0x800FE330); y = r_u32(0x800FE334);
    value = r_u32(0x800311B8);
    z = r_u32(0x800FE338);
    phase = value << 5;
    w_u32(0x80124C1C, x); w_u32(0x80124C20, y); w_u32(0x80124C24, z);
    if ((sint32)phase < 1024) {
        w_u16(0x800FA280, 894 - phase);
        w_u16(0x800FA282, phase - 1024);
    } else {
        w_u16(0x800FA280, (uint32)-130); w_u16(0x800FA282, 0);
    }
    w_u16(0x800FA284, 0);
    RotMatrix((SVECTOR *)psx_addr(0x800FA280, 6), (MATRIX *)psx_addr(0x800F4238, 18));
    MulMatrix0((MATRIX *)psx_addr(0x800F4238, 20), (MATRIX *)psx_addr(view_rotation, 20), (MATRIX *)psx_addr(0x800F4238, 20));
    SetRotMatrix((MATRIX *)psx_addr(0x800F4238, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80124C08, 32));
    a = r_u32(0x800EC890); b = r_u32(0x80046178);
    mesh_render_fixed_alternate(r_u32(b + a * 132));
    if ((sint32)phase >= 1025)
        phase = 1024;
    value = (1024 - phase) * 3;
    w_u32(0x800FE330, (uint32)((sint32)value >> 1) + 2500);
    w_u32(0x800FE334, 650); w_u32(0x800FE338, 2500);
    game_hud_transform_scratch(view_rotation);
    x = r_u32(0x800FE330); y = r_u32(0x800FE334); z = r_u32(0x800FE338);
    w_u32(0x80124C1C, x); w_u32(0x80124C20, y); w_u32(0x80124C24, z);
    if ((sint32)phase < 1024) {
        w_u16(0x800FA280, 894 - phase);
        value = 0u - ((phase - 1024) * 10);
        w_u16(0x800FA282, (uint32)((sint32)value >> 3));
    } else {
        w_u16(0x800FA280, (uint32)-130); w_u16(0x800FA282, 0);
    }
    w_u16(0x800FA284, 0);
    RotMatrix((SVECTOR *)psx_addr(0x800FA280, 6), (MATRIX *)psx_addr(0x800F4238, 18));
    MulMatrix0((MATRIX *)psx_addr(0x800F4238, 20), (MATRIX *)psx_addr(view_rotation, 20), (MATRIX *)psx_addr(0x800F4238, 20));
    SetRotMatrix((MATRIX *)psx_addr(0x800F4238, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80124C08, 32));
    a = r_u32(0x800EC894); b = r_u32(0x80046178);
    mesh_render_fixed_alternate(r_u32(b + a * 132));
    x = r_u32(0x80031198); y = r_u32(0x800311B4);
    w_u32(0x8001024C, x); w_u32(0x80010250, y);
    system_set_projection_offset((sint32)x, (sint32)y);
    if ((alternate & 255) == 0) {
        w_u32(0x800FE330, 0); w_u32(0x800FE334, (uint32)-1000); w_u32(0x800FE338, 13000);
        game_hud_transform_scratch(cockpit_rotation);
        x = r_u32(0x800FE330); a = r_u32(cockpit_transform + 20);
        w_u32(0x80124C1C, x + a);
        y = r_u32(0x800FE334); b = r_u32(cockpit_transform + 24);
        w_u32(0x80124C20, y + b);
        z = r_u32(0x800FE338); c = r_u32(cockpit_transform + 28);
        w_u32(0x80124C24, z + c);
        SetRotMatrix((MATRIX *)psx_addr(cockpit_rotation, 20));
        SetTransMatrix((MATRIX *)psx_addr(0x80124C08, 32));
        a = r_u32(0x800EC898); b = r_u32(0x80046178);
        mesh_render_fixed_alternate(r_u32(b + a * 132));
    }
    x = r_u32(0x80031198); y = r_u32(0x800311B4);
    w_u32(0x8001024C, x); w_u32(0x80010250, y);
    system_set_projection_offset((sint32)x, (sint32)y);
restore_projection:
    w_u32(0x8001024C, saved_x); w_u32(0x80010250, saved_y);
    system_set_projection_offset((sint32)saved_x, (sint32)saved_y);
}

static void game_debris_initialize_face_record(uint32 object, uint32 face, uint32 corners)
{
    uint32 offset, seed, next, value;
    offset = r_u32(0x800EF510) * 76;
    seed = r_u32(0x800EF50C);
    seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    w_u16(0x8011A3E8 + offset, (seed & 63) + 32);
    w_u16(0x8011A3EC + offset, corners);
    value = r_u16(face + 0x18);
    w_u16(0x8011A42A + offset, value);
    value = r_u16(face + 0x1A);
    w_u32(0x800EF50C, seed);
    w_u16(0x8011A428 + offset, value);
    value = r_u16(face + 0x1C);
    w_u16(0x8011A42C + offset, value);
    offset = r_u32(0x800EF510) * 76;
    value = r_u16(face + 0x1E);
    w_u16(0x8011A42E + offset, value);
    offset = r_u32(0x800EF510) * 76;
    value = r_u16(face + 0x20);
    w_u16(0x8011A430 + offset, value);
    if (corners == 4) {
        offset = r_u32(0x800EF510) * 76;
        value = r_u16(face + 0x22);
        w_u16(0x8011A432 + offset, value);
    }
    seed = r_u32(0x800EF50C);
    offset = r_u32(0x800EF510) * 76;
    seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    value = r_u32(object + 0x14);
    w_u32(0x800EF50C, seed);
    w_u16(0x8011A420 + offset, (uint32)((sint32)value >> 8));
    next = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    value = r_u32(object + 0x18);
    w_u32(0x800EF50C, next);
    w_u16(0x8011A422 + offset, (uint32)((sint32)value >> 8));
    value = r_u32(object + 0x1C);
    w_u16(0x8011A3EA + offset, (next & 63) + 192);
    w_u16(0x8011A3EE + offset, 511);
    w_u16(0x8011A424 + offset, (uint32)((sint32)value >> 8));
}

static void game_debris_rotate_face_vertex(uint32 object, uint32 face, uint32 vertices, uint32 vertex, VECTOR *output)
{
    uint32 components[3], index, axis, scale, positive, shift, value;
    SVECTOR input;
    for (axis = 0; axis < 3; ++axis) {
        index = r_u32(face + 4 + vertex * 4);
        components[axis] = (uint32)(r_s16(vertices + index * 8 + axis * 2) >> 1);
    }
    scale = r_u32(object + 0x84);
    positive = (sint32)scale > 0;
    for (axis = 0; axis < 3; ++axis) {
        if (axis != 0)
            scale = r_u32(object + 0x84);
        if (positive)
            value = components[axis] << (scale & 31);
        else {
            shift = (sint32)scale < 0 ? 0u - scale : scale;
            value = (uint32)((sint32)components[axis] >> (shift & 31));
        }
        if (axis == 0)
            input.vx = (sint16)value;
        else if (axis == 1)
            input.vy = (sint16)value;
        else
            input.vz = (sint16)value;
    }
    ApplyRotMatrix(&input, output);
    /* Use the SDK's signed IR outputs */
    output->vx = (sint32)xport_gte_read_data(9);
    output->vy = (sint32)xport_gte_read_data(10);
    output->vz = (sint32)xport_gte_read_data(11);
}

void game_emit_debris_expiration_effects(uint32 object)
{
    uint32 mesh, vertices, shift, index, face, corners, value, slot, offset, axis, vertex;
    uint32 x, y, z, length, quotient, world[4][3], center[3], sum, next;
    VECTOR point[4];
    FUNCTION_MARKER(0x800BE09C, "GAME.OVL");
    value = r_u32(object + 0x84);
    mesh = r_u32(object + 0x64);
    shift = (uint32)((sint32)value >> 1) + 6;
    sound_play_at_camera(object + 8, 4, 128);
    sound_play_at_camera(object + 8, 4, 128);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    vertices = r_u32(mesh);
    game_emit_projectile_impact(x, y, z);
    if (r_s32(mesh + 0x14) <= 0)
        return;
    index = 0;
    do {
        face = r_u32(mesh + 0x10) + index * 60;
        value = r_u32(face);
        corners = value == 3 ? 3 : 4;
        game_debris_initialize_face_record(object, face, corners);
        SetRotMatrix((MATRIX *)psx_addr(object + 0x11C, 20));
        SetTransMatrix((MATRIX *)psx_addr(object + 0x11C, 32));
        game_debris_rotate_face_vertex(object, face, vertices, 0, &point[0]);
        length = (uint32)vector_length_approx(point[0].vx, point[0].vy, point[0].vz);
        if (length == 0)
            length = 1;
        quotient = (uint32)game_hud_divide((uint32)point[0].vx << (shift & 31), length);
        offset = r_u32(0x800EF510) * 76;
        value = r_u16(0x8011A420 + offset);
        w_u16(0x8011A420 + offset, value + quotient);
        quotient = (uint32)game_hud_divide((uint32)point[0].vy << (shift & 31), length);
        value = r_u16(0x8011A422 + offset);
        w_u16(0x8011A422 + offset, value + quotient);
        quotient = (uint32)game_hud_divide((uint32)point[0].vz << (shift & 31), length);
        value = r_u16(0x8011A424 + offset);
        w_u16(0x8011A424 + offset, value + quotient);
        for (vertex = 1; vertex < corners; ++vertex)
            game_debris_rotate_face_vertex(object, face, vertices, vertex, &point[vertex]);
        for (vertex = 0; vertex < corners; ++vertex) {
            world[vertex][0] = (uint32)point[vertex].vx << 1;
            world[vertex][1] = (uint32)point[vertex].vy << 1;
            world[vertex][2] = (uint32)point[vertex].vz << 1;
        }
        for (axis = 0; axis < 3; ++axis) {
            sum = 0;
            for (vertex = 0; vertex < corners; ++vertex) {
                value = r_u32(object + 8 + axis * 4);
                world[vertex][axis] += value;
                if ((corners == 4 && axis == 1 && vertex == 1) ||
                    (corners == 3 && axis == 2 && vertex == 2))
                    slot = r_u32(0x800EF510);
                sum += world[vertex][axis];
            }
            center[axis] = corners == 3 ? (uint32)((sint32)sum / 3) : (uint32)((sint32)sum >> 2);
        }
        offset = slot * 76;
        for (vertex = 0; vertex < corners; ++vertex) {
            for (axis = 0; axis < 3; ++axis) {
                if (vertex == corners - 1 && axis == 2)
                    continue;
                w_u16(0x8011A3F0 + offset + vertex * 8 + axis * 2, world[vertex][axis] - center[axis]);
            }
        }
        w_u32(0x8011A410 + offset, center[0]);
        w_u32(0x8011A414 + offset, center[1]);
        w_u32(0x8011A418 + offset, center[2]);
        w_u16(0x8011A3F4 + offset + (corners - 1) * 8, world[corners - 1][2] - center[2]);
        next = slot + 1;
        w_u32(0x800EF510, (uint32)((sint32)next % 256));
        value = r_u32(mesh + 0x14);
        ++index;
    } while ((sint32)index < (sint32)value);
}

static uint32 game_glow_checked_add(uint32 first, uint32 second, uint32 pc)
{
    sint64 sum = (sint64)(sint32)first + (sint32)second;
    if (sum > 2147483647 || sum < (-2147483647 - 1))
        xport_mips_overflow_exception(pc);
    return first + second;
}

void game_draw_projected_glow(sint32 x, sint32 y, sint32 radius, uint32 center_color, uint32 edge_color)
{
    uint32 angle, sine, cosine, product, first_x, first_y, next_x, next_y, packet, ordering;
    FUNCTION_MARKER(0x800CE604, "GAME.OVL");
    game_glow_checked_add((uint32)(radius >> 1), (uint32)radius, 0x800CE654);
    angle = 0;
    do {
        sine = (uint32)r_s16(0x8003DC68 + (angle & 0xFFF) * 2);
        cosine = (uint32)r_s16(0x8003DC68 + ((angle + 1024) & 0xFFF) * 2);
        product = sine * (uint32)radius;
        first_x = game_glow_checked_add((uint32)((sint32)product >> 12), (uint32)x, 0x800CE6A0);
        product = cosine * (uint32)radius;
        first_y = game_glow_checked_add((uint32)((sint32)product >> 13), (uint32)y, 0x800CE6B0);
        angle += 256;
        sine = (uint32)r_s16(0x8003DC68 + (angle & 0xFFF) * 2);
        cosine = (uint32)r_s16(0x8003DC68 + ((angle + 1024) & 0xFFF) * 2);
        product = sine * (uint32)radius;
        next_x = game_glow_checked_add((uint32)((sint32)product >> 12), (uint32)x, 0x800CE700);
        product = cosine * (uint32)radius;
        next_y = game_glow_checked_add((uint32)((sint32)product >> 13), (uint32)y, 0x800CE710);
        packet = r_u32(0x80040690);
        w_u32(packet + 4, center_color);
        w_u32(packet + 12, edge_color);
        w_u32(packet + 20, edge_color);
        w_u8(packet + 7, 0x32);
        w_u8(packet + 3, 6);
        w_u16(packet + 8, (uint32)x);
        w_u16(packet + 10, (uint32)y);
        w_u16(packet + 16, first_x);
        w_u16(packet + 18, first_y);
        w_u16(packet + 24, next_x);
        w_u16(packet + 26, next_y);
        ordering = r_u32(0x800421AC);
        AddPrim(psx_addr(ordering + 0x4504, 4), psx_addr(packet, 4));
        packet = r_u32(0x80040690);
        packet = game_glow_checked_add(packet, 28, 0x800CE7B0);
        w_u32(0x80040690, packet);
    } while (angle < 4096);
}

static void game_debris_initialize_packed_record(uint32 object, uint32 primitive, uint32 corners, uint32 mask)
{
    uint32 offset, seed, next, value, vertex;
    offset = r_u32(0x800EF510) * 76;
    seed = r_u32(0x800EF50C);
    seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    w_u16(0x8011A3E8 + offset, (seed & 63) + 32);
    w_u16(0x8011A3EC + offset, 3);
    value = r_u16(primitive + 6);
    w_u16(0x8011A42A + offset, value);
    value = r_u16(primitive + 10);
    w_u32(0x800EF50C, seed);
    w_u16(0x8011A428 + offset, value);
    for (vertex = 0; vertex < corners; ++vertex) {
        if (vertex != 0)
            offset = r_u32(0x800EF510) * 76;
        value = r_u8(primitive + 4 + vertex * 4);
        w_u8(0x8011A42C + offset + vertex * 2, value);
    }
    for (vertex = 0; vertex < corners; ++vertex) {
        offset = r_u32(0x800EF510) * 76;
        value = r_u8(primitive + 5 + vertex * 4);
        w_u8(0x8011A42D + offset + vertex * 2, value);
    }
    if (corners == 3) {
        offset = r_u32(0x800EF510) * 76;
        seed = r_u32(0x800EF50C);
        value = r_u32(object + 0x14);
        w_u16(0x8011A420 + offset, (uint32)((sint32)value >> 8));
        value = r_u32(object + 0x18);
        w_u16(0x8011A422 + offset, (uint32)((sint32)value >> 8));
        value = r_u32(object + 0x1C);
        next = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        w_u32(0x800EF50C, next);
        w_u16(0x8011A424 + offset, (uint32)((sint32)value >> 8));
        seed = (uint32)((sint32)next >> 1) ^ (next << 3);
        w_u32(0x800EF50C, seed);
        w_u16(0x8011A3EA + offset, (seed & 63) + 192);
        w_u16(0x8011A3EE + offset, mask);
    } else {
        seed = r_u32(0x800EF50C);
        next = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        w_u32(0x800EF50C, next);
        offset = r_u32(0x800EF510) * 76;
        value = r_u32(object + 0x14);
        w_u16(0x8011A420 + offset, (uint32)((sint32)value >> 8));
        seed = (uint32)((sint32)next >> 1) ^ (next << 3);
        value = r_u32(object + 0x18);
        w_u32(0x800EF50C, seed);
        w_u16(0x8011A422 + offset, (uint32)((sint32)value >> 8));
        value = r_u32(object + 0x1C);
        w_u16(0x8011A3EA + offset, (seed & 63) + 192);
        w_u16(0x8011A3EE + offset, mask);
        w_u16(0x8011A424 + offset, (uint32)((sint32)value >> 8));
    }
}

static void game_debris_rotate_packed_vertex(uint32 object, uint32 primitive, uint32 vertices, uint32 index_offset, VECTOR *output)
{
    uint32 components[3], index, axis, scale, left_shift, shift, value;
    SVECTOR input;
    for (axis = 0; axis < 3; ++axis) {
        index = (uint32)r_s16(primitive + index_offset);
        components[axis] = (uint32)(r_s16(vertices + index * 8 + axis * 2) >> 1);
    }
    scale = r_u32(object + 0x84);
    left_shift = (sint32)scale >= 0;
    for (axis = 0; axis < 3; ++axis) {
        if (axis != 0)
            scale = r_u32(object + 0x84);
        if (left_shift)
            value = components[axis] << (scale & 31);
        else {
            shift = (sint32)scale < 0 ? 0u - scale : scale;
            value = (uint32)((sint32)components[axis] >> (shift & 31));
        }
        if (axis == 0)
            input.vx = (sint16)value;
        else if (axis == 1)
            input.vy = (sint16)value;
        else
            input.vz = (sint16)value;
    }
    ApplyRotMatrix(&input, output);
    output->vx = (sint32)xport_gte_read_data(9);
    output->vy = (sint32)xport_gte_read_data(10);
    output->vz = (sint32)xport_gte_read_data(11);
}

void game_emit_object_debris(uint32 object)
{
    uint32 scale, mesh, mask, shift, count, value, primitive, vertices, flags, x, y, z;
    uint32 lower, frame, upper, index, format, corners, vertex, axis, length, quotient;
    uint32 offset, slot, world[4][3], center[3], sum, next;
    VECTOR point[4];
    static const uint32 indices[4][4] = {
        {0x12,0x14,0x16,0}, {0x12,0x16,0x1A,0},
        {0x16,0x18,0x1A,0x1C}, {0x16,0x1A,0x1E,0x22}
    };
    FUNCTION_MARKER(0x800BFC70, "GAME.OVL");
    scale = r_u32(object + 0x84);
    mesh = r_u32(object + 0x64);
    mask = (sint32)scale < 0 ? 127 : 511;
    shift = (sint32)scale < 0 ? 7 : 6;
    sound_play_at_camera(object + 8, 4, 256);
    sound_play_at_camera(object + 8, 4, 128);
    count = r_u32(mesh + 0x14);
    flags = r_u32(object + 0x70);
    primitive = r_u32(mesh + 0x10);
    vertices = r_u32(mesh);
    x = r_u32(object + 8); y = r_u32(object + 12); z = r_u32(object + 16);
    if ((flags & 0x10) != 0)
        game_emit_hit_flash(x, y, z);
    else
        game_emit_flash_bca38(x, y, z);
    lower = r_u32(0x80042178);
    frame = r_u32(0x80010240);
    if ((sint32)frame < (sint32)lower)
        count = (uint32)((sint32)count >> 1);
    else {
        upper = r_u32(0x80042180);
        if ((sint32)frame < (sint32)upper)
            count = (uint32)((sint32)count >> 1);
        else if ((sint32)(frame - 30) < (sint32)lower)
            count = (uint32)((sint32)(count * 3) >> 2);
    }
    if ((sint32)count <= 0)
        return;
    index = 0;
    do {
        value = r_u8(primitive + 3) & 0xFC;
        switch (value) {
        case 0x24: format = 0; corners = 3; break;
        case 0x34: format = 1; corners = 3; break;
        case 0x2C: format = 2; corners = 4; break;
        case 0x3C: format = 3; corners = 4; break;
        default: goto next_primitive;
        }
        game_debris_initialize_packed_record(object, primitive, corners, mask);
        SetRotMatrix((MATRIX *)psx_addr(object + 0x11C, 20));
        SetTransMatrix((MATRIX *)psx_addr(object + 0x11C, 32));
        game_debris_rotate_packed_vertex(object, primitive, vertices, indices[format][0], &point[0]);
        length = (uint32)vector_length_approx(point[0].vx, point[0].vy, point[0].vz);
        if (length == 0)
            length = 1;
        quotient = (uint32)game_hud_divide((uint32)point[0].vx << shift, length);
        offset = r_u32(0x800EF510) * 76;
        value = r_u16(0x8011A420 + offset);
        w_u16(0x8011A420 + offset, value + quotient);
        quotient = (uint32)game_hud_divide((uint32)point[0].vy << shift, length);
        value = r_u16(0x8011A422 + offset);
        w_u16(0x8011A422 + offset, value + quotient);
        quotient = (uint32)game_hud_divide((uint32)point[0].vz << shift, length);
        value = r_u16(0x8011A424 + offset);
        w_u16(0x8011A424 + offset, value + quotient);
        for (vertex = 1; vertex < corners; ++vertex)
            game_debris_rotate_packed_vertex(object, primitive, vertices, indices[format][vertex], &point[vertex]);
        for (vertex = 0; vertex < corners; ++vertex) {
            world[vertex][0] = (uint32)point[vertex].vx << 1;
            world[vertex][1] = (uint32)point[vertex].vy << 1;
            world[vertex][2] = (uint32)point[vertex].vz << 1;
        }
        for (axis = 0; axis < 3; ++axis) {
            sum = 0;
            for (vertex = 0; vertex < corners; ++vertex) {
                value = r_u32(object + 8 + axis * 4);
                world[vertex][axis] += value;
                sum += world[vertex][axis];
            }
            center[axis] = (uint32)((sint32)sum / (sint32)corners);
        }
        slot = r_u32(0x800EF510);
        offset = slot * 76;
        for (vertex = 0; vertex < corners; ++vertex) {
            for (axis = 0; axis < 3; ++axis) {
                if (vertex == corners - 1 && axis == 2)
                    continue;
                w_u16(0x8011A3F0 + offset + vertex * 8 + axis * 2, world[vertex][axis] - center[axis]);
            }
        }
        w_u32(0x8011A410 + offset, center[0]);
        w_u32(0x8011A414 + offset, center[1]);
        w_u32(0x8011A418 + offset, center[2]);
        w_u16(0x8011A3F4 + offset + (corners - 1) * 8, world[corners - 1][2] - center[2]);
        next = slot + 1;
        w_u32(0x800EF510, (uint32)((sint32)next % 256));
next_primitive:
        value = r_u8(primitive + 1);
        ++index;
        primitive += value * 4 + 4;
    } while ((sint32)index < (sint32)count);
}

static uint32 game_attached_sar(uint32 value, uint32 shift)
{
    return (uint32)((sint32)value >> (shift & 31));
}

static void game_attached_transform_point(uint32 matrix, uint32 point)
{
    VECTOR result;
    ApplyMatrixLV((MATRIX *)psx_addr(matrix, 20), (VECTOR *)psx_addr(point, 12), &result);
    w_u32(point, (uint32)result.vx);
    w_u32(point + 4, (uint32)result.vy);
    w_u32(point + 8, (uint32)result.vz);
}

static void game_attached_normalize(uint32 vector[3])
{
    uint32 x = vector[0], y = vector[1], z = vector[2];
    uint32 ax, ay, az, larger, smaller, length;
    ax = (sint32)x < 0 ? 0u - x : x;
    ay = (sint32)y < 0 ? 0u - y : y;
    az = (sint32)z < 0 ? 0u - z : z;
    larger = (sint32)ax < (sint32)az ? az : ax;
    smaller = (sint32)ax < (sint32)az ? ax : az;
    length = game_attached_sar((larger << 4) + smaller * 5, 4);
    larger = (sint32)length < (sint32)ay ? ay : length;
    smaller = (sint32)length < (sint32)ay ? length : ay;
    length = game_attached_sar((larger << 4) + smaller * 5, 4) + 1;
    vector[0] = (uint32)game_hud_divide(x << 12, length);
    vector[1] = (uint32)game_hud_divide(y << 12, length);
    vector[2] = (uint32)game_hud_divide(z << 12, length);
}

static sint32 game_attached_project_outline(void)
{
    uint32 group, vertex;
    sint32 screen[3], depth[3], flags;
    SVECTOR input[3];
    static const uint32 last_vertices[3] = {0x8010AC38, 0x8010AC40, 0x8010ABB0};
    static const uint32 last_screens[3] = {0x80033DA8, 0x80033DAC, 0x80033D64};
    for (group = 0; group < 7; ++group) {
        gte_project3_full_depth((const SVECTOR *)psx_addr(0x8010AB90 + group * 24, 24), screen, depth, &flags);
        for (vertex = 0; vertex < 3; ++vertex)
            w_u32(0x80033D54 + group * 12 + vertex * 4, (uint32)screen[vertex]);
        for (vertex = 0; vertex < 3; ++vertex)
            w_u16(0x800510F8 + group * 6 + vertex * 2, (uint32)depth[vertex]);
        w_u16(0x80059660 + group * 2, (uint32)flags >> 12);
    }
    for (vertex = 0; vertex < 3; ++vertex) {
        input[vertex].vx = r_s16(last_vertices[vertex]);
        input[vertex].vy = r_s16(last_vertices[vertex] + 2);
        input[vertex].vz = r_s16(last_vertices[vertex] + 4);
    }
    gte_project3_full_depth(input, screen, depth, &flags);
    for (vertex = 0; vertex < 3; ++vertex)
        w_u32(last_screens[vertex], (uint32)screen[vertex]);
    return depth[2] >> 2;
}

static void game_attached_select_screen_point(uint32 destination, uint32 alternate, uint32 extra_source, uint32 extra_destination, sint32 center_x, sint32 center_y)
{
    uint32 x, y, next_x, next_y, first_distance, second_distance, extra_x, extra_y;
    x = (uint32)r_s16(destination);
    x = game_attached_sar(x - (uint32)center_x, 1);
    y = (uint32)r_s16(destination + 2) - (uint32)center_y;
    first_distance = x * x + y * y;
    next_x = (uint32)r_s16(alternate);
    x = game_attached_sar(next_x - (uint32)center_x, 1);
    next_y = (uint32)r_s16(alternate + 2);
    y = next_y - (uint32)center_y;
    second_distance = x * x + y * y;
    if ((sint32)first_distance < (sint32)second_distance) {
        if (extra_source != 0) {
            extra_x = r_u16(extra_source);
            extra_y = r_u16(extra_source + 2);
        }
        w_u16(destination, next_x);
        w_u16(destination + 2, next_y);
        if (extra_source != 0) {
            w_u16(extra_destination, extra_x);
            w_u16(extra_destination + 2, extra_y);
        }
    }
}

static void game_attached_emit_packets(uint32 center, uint32 primary_depth, uint32 secondary_depth, uint32 primary_color, uint32 secondary_color)
{
    uint32 packet, index, half_center, step, first, second, third, value, source;
    packet = r_u32(0x80040690);
    for (index = 0; index < 16; ++index)
        w_u32(packet + index * 36 + 16, center);
    packet = r_u32(0x80040690);
    half_center = (uint16)((sint16)center >> 1);
    half_center |= (uint32)(uint16)((sint16)(center >> 16) >> 1) << 16;
    for (index = 1; index < 16; index += 2) {
        w_u32(packet + index * 36 + 8, half_center);
        w_u32(packet + index * 36 + 24, half_center);
        w_u32(packet + index * 36 + 32, half_center);
    }
    packet = r_u32(0x80040690);
    for (index = 0; index < 16; ++index) {
        w_u8(packet + index * 36 + 3, 8);
        w_u8(packet + index * 36 + 7, 0x38);
    }
    packet = r_u32(0x80040690);
    for (index = 0; index < 16; ++index) {
        w_u32(packet + index * 36 + 4, 0x3A000000);
        w_u32(packet + index * 36 + 20, 0);
        w_u32(packet + index * 36 + 28, 0);
    }
    packet = r_u32(0x80040690);
    for (index = 0; index < 16; index += 2)
        w_u32(packet + index * 36 + 12, primary_color);
    packet = r_u32(0x80040690);
    for (index = 1; index < 16; index += 2)
        w_u32(packet + index * 36 + 12, secondary_color);
    step = 1;
    do {
        first = ((step & 15) + 1) * 4;
        second = ((((step & 15) + 1) & 15) + 1) * 4;
        third = step * 4;
        packet = r_u32(0x80040690);
        w_u32(0x80040690, packet + 36);
        value = r_u32(0x80033D50 + second);
        w_u32(packet + 8, value);
        value = r_u32(0x80033D50 + first);
        w_u32(packet + 24, value);
        value = r_u32(0x80033D50 + third);
        step += 2;
        w_u32(packet + 32, value);
        render_insert_primitive((sint32)primary_depth, packet);
        source = r_u16(0x80033D50 + second);
        packet = r_u32(0x80040690);
        value = r_u16(packet + 8);
        w_u16(packet + 8, value + (uint32)((sint16)source >> 1));
        source = r_u16(0x80033D52 + second);
        value = r_u16(packet + 10);
        w_u16(packet + 10, value + (uint32)((sint16)source >> 1));
        source = r_u16(0x80033D50 + first);
        value = r_u16(packet + 24);
        w_u16(packet + 24, value + (uint32)((sint16)source >> 1));
        source = r_u16(0x80033D52 + first);
        value = r_u16(packet + 26);
        w_u16(packet + 26, value + (uint32)((sint16)source >> 1));
        source = r_u16(0x80033D50 + third);
        w_u32(0x80040690, packet + 36);
        value = r_u16(packet + 32);
        w_u16(packet + 32, value + (uint32)((sint16)source >> 1));
        source = r_u16(0x80033D52 + third);
        value = r_u16(packet + 34);
        w_u16(packet + 34, value + (uint32)((sint16)source >> 1));
        render_insert_primitive((sint32)secondary_depth, packet);
    } while (step < 17);
}

static void game_attached_apply_player_contact(uint32 object, uint32 color)
{
    uint32 x, y, z, bx, by, bz, value, length, player, shield, brightness, red, green, blue;
    sint32 distance;
    if ((r_u32(object + 0x114) & 0x10) == 0 || r_s16(0x800595F6) != 0)
        return;
    bx = (uint32)r_s16(object + 0x120);
    x = r_u32(0x800F4F40);
    y = r_u32(0x800F4F44);
    by = (uint32)r_s16(object + 0x126);
    z = r_u32(0x800F4F48);
    bz = (uint32)r_s16(object + 0x12C);
    if ((sint32)(x * bx + y * by + z * bz) <= 0)
        return;
    length = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    x = r_u32(0x800F4F50); y = r_u32(0x800F4F54); z = r_u32(0x800F4F58);
    value = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    length += value;
    value = game_attached_sar(r_u32(0x800FD71C), 8) + 1;
    distance = game_hud_divide(length, value);
    if (distance < 1500) {
        game_set_screen_tint(245, 245, 192);
        player = r_u32(0x80040FDC);
        bx = (uint32)r_s16(object + 0x120); value = r_u32(player + 0x14);
        w_u32(player + 0x14, value - bx * 5);
        by = (uint32)r_s16(object + 0x126); value = r_u32(player + 0x18);
        w_u32(player + 0x18, value - by * 5);
        bz = (uint32)r_s16(object + 0x12C); value = r_u32(player + 0x1C);
        w_u32(player + 0x1C, value - bz * 5);
        x = (uint32)random_signed_between(5000, 10000);
        player = r_u32(0x80040FDC); value = r_u32(player + 0x14);
        w_u32(player + 0x14, value + x);
        y = (uint32)random_signed_between(5000, 10000);
        player = r_u32(0x80040FDC); value = r_u32(player + 0x18);
        w_u32(player + 0x18, value + y);
        z = (uint32)random_signed_between(5000, 10000);
        player = r_u32(0x80040FDC);
        value = r_u32(player + 0x1C); shield = r_u32(player + 0x234);
        w_u32(player + 0x1C, value + z);
        if ((sint32)shield > 0) {
            value = shield - 5;
            w_u32(player + 0x234, value);
            if ((sint32)value < 0)
                w_u32(player + 0x234, 0);
        } else {
            value = r_u32(player + 0x238) - 5;
            w_u32(player + 0x238, value);
            if ((sint32)value < 0) {
                w_u32(player + 0x238, 0);
                game_fail_current_mission();
            }
        }
    } else if (distance < 2000) {
        value = (2000u - (uint32)distance) << 8;
        brightness = (uint32)((sint32)value / 500);
        brightness += (uint32)random_between(0, 16);
        if ((sint32)brightness >= 256)
            brightness = 255;
        red = ((color & 255) * brightness) >> 8;
        green = (((color >> 8) & 255) * brightness) >> 8;
        blue = (((color >> 16) & 255) * brightness) >> 8;
        value = red + (green << 8) + (blue << 16);
        game_set_screen_tint(value & 255, (value >> 8) & 255, (value >> 16) & 255);
        if (distance < 1700) {
            x = (uint32)random_between(-100, 100);
            player = r_u32(0x80040FDC); value = r_u32(player + 8);
            w_u32(player + 8, value + x);
            y = (uint32)random_between(-100, 100);
            player = r_u32(0x80040FDC); value = r_u32(player + 12);
            w_u32(player + 12, value + y);
            z = (uint32)random_between(-100, 100);
            player = r_u32(0x80040FDC); value = r_u32(player + 16);
            w_u32(player + 16, value + z);
        }
    }
}

static void game_attached_build_geometry(const uint32 center[3], const uint32 side[3], const uint32 up[3], const uint32 back[3], uint32 scale_shift)
{
    uint32 width = r_u32(0x80124018);
    uint32 thickness = r_u32(0x800FD71C);
    w_u32(0x800FD71C, game_attached_sar(thickness, 2));
    w_u16(0x8010AB90, game_attached_sar(side[0]*width, scale_shift + 12));
    w_u16(0x8010ABD0, center[0] - game_attached_sar(side[0]*width, scale_shift + 12));
    w_u16(0x8010AB90, center[0] + game_attached_sar(side[0]*width, scale_shift + 12));
    w_u16(0x8010AB92, game_attached_sar(side[1]*width, scale_shift + 12));
    w_u16(0x8010ABD2, center[1] - game_attached_sar(side[1]*width, scale_shift + 12));
    w_u16(0x8010AB92, center[1] + game_attached_sar(side[1]*width, scale_shift + 12));
    w_u16(0x8010AB94, game_attached_sar(side[2]*width, scale_shift + 12));
    w_u16(0x8010ABD4, center[2] - game_attached_sar(side[2]*width, scale_shift + 12));
    w_u16(0x8010AB94, center[2] + game_attached_sar(side[2]*width, scale_shift + 12));
    w_u16(0x8010AB98, game_attached_sar(width*game_attached_sar(946*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABC8, 0 - game_attached_sar(width*game_attached_sar(946*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABD8, center[0] - game_attached_sar(width*game_attached_sar(946*side[0], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC08, center[0] + game_attached_sar(width*game_attached_sar(946*side[0], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC10, game_attached_sar(width*game_attached_sar(392*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC40, center[0] - game_attached_sar(width*game_attached_sar(946*side[0], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC10, center[0] + game_attached_sar(width*game_attached_sar(946*side[0], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*up[0], 10), scale_shift + 12));
    w_u16(0x8010AB98, center[0] + game_attached_sar(back[0]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABC8, center[0] + game_attached_sar(back[0]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*side[0], 10), scale_shift + 12));
    w_u16(0x8010AB9A, game_attached_sar(width*game_attached_sar(946*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABCA, 0 - game_attached_sar(width*game_attached_sar(946*side[1], 10), scale_shift + 12));
    w_u16(0x8010AC12, game_attached_sar(width*game_attached_sar(392*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC0A, center[1] + game_attached_sar(width*game_attached_sar(946*side[1], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC42, center[1] - game_attached_sar(width*game_attached_sar(946*side[1], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*up[1], 10), scale_shift + 12));
    w_u16(0x8010ABDA, center[1] - game_attached_sar(width*game_attached_sar(946*side[1], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC12, center[1] + game_attached_sar(width*game_attached_sar(946*side[1], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*up[1], 10), scale_shift + 12));
    w_u16(0x8010AB9A, center[1] + game_attached_sar(back[1]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABCA, center[1] + game_attached_sar(back[1]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*side[1], 10), scale_shift + 12));
    w_u16(0x8010AB9C, game_attached_sar(width*game_attached_sar(946*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABCC, 0 - game_attached_sar(width*game_attached_sar(946*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABA0, game_attached_sar(width*game_attached_sar(724*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABC0, 0 - game_attached_sar(width*game_attached_sar(724*side[0], 10), scale_shift + 12));
    w_u16(0x8010AB9C, center[2] + game_attached_sar(back[2]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABCC, center[2] + game_attached_sar(back[2]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*side[2], 10), scale_shift + 12));
    w_u16(0x8010AC18, game_attached_sar(width*game_attached_sar(724*up[0], 10), scale_shift + 12));
    w_u16(0x8010ABE0, center[0] - game_attached_sar(width*game_attached_sar(724*side[0], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC00, center[0] + game_attached_sar(width*game_attached_sar(724*side[0], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC38, center[0] - game_attached_sar(width*game_attached_sar(724*side[0], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC18, center[0] + game_attached_sar(width*game_attached_sar(724*side[0], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*up[0], 10), scale_shift + 12));
    w_u16(0x8010ABA0, center[0] + game_attached_sar(2*back[0]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABC0, center[0] + game_attached_sar(2*back[0]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABA2, game_attached_sar(width*game_attached_sar(724*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABC2, 0 - game_attached_sar(width*game_attached_sar(724*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABE2, center[1] - game_attached_sar(width*game_attached_sar(724*side[1], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC02, center[1] + game_attached_sar(width*game_attached_sar(724*side[1], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC3A, center[1] - game_attached_sar(width*game_attached_sar(724*side[1], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC1A, game_attached_sar(width*game_attached_sar(724*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC1A, center[1] + game_attached_sar(width*game_attached_sar(724*side[1], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*up[1], 10), scale_shift + 12));
    w_u16(0x8010ABA2, center[1] + game_attached_sar(2*back[1]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*side[1], 10), scale_shift + 12));
    w_u16(0x8010AC14, game_attached_sar(width*game_attached_sar(392*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC0C, center[2] + game_attached_sar(width*game_attached_sar(946*side[2], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*up[2], 10), scale_shift + 12));
    w_u16(0x8010ABDC, center[2] - game_attached_sar(width*game_attached_sar(946*side[2], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC44, center[2] - game_attached_sar(width*game_attached_sar(946*side[2], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC14, center[2] + game_attached_sar(width*game_attached_sar(946*side[2], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*up[2], 10), scale_shift + 12));
    w_u16(0x8010ABC2, center[1] + game_attached_sar(2*back[1]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABA4, game_attached_sar(width*game_attached_sar(724*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABC4, 0 - game_attached_sar(width*game_attached_sar(724*side[2], 10), scale_shift + 12));
    w_u16(0x8010AC1C, game_attached_sar(width*game_attached_sar(724*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC04, center[2] + game_attached_sar(width*game_attached_sar(724*side[2], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC3C, center[2] - game_attached_sar(width*game_attached_sar(724*side[2], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*up[2], 10), scale_shift + 12));
    w_u16(0x8010ABE4, center[2] - game_attached_sar(width*game_attached_sar(724*side[2], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC1C, center[2] + game_attached_sar(width*game_attached_sar(724*side[2], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*up[2], 10), scale_shift + 12));
    w_u16(0x8010ABA4, center[2] + game_attached_sar(2*back[2]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(724*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABC4, center[2] + game_attached_sar(2*back[2]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(724*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABA8, game_attached_sar(width*game_attached_sar(392*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABB8, 0 - game_attached_sar(width*game_attached_sar(392*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABF8, center[0] + game_attached_sar(width*game_attached_sar(392*side[0], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC30, center[0] - game_attached_sar(width*game_attached_sar(392*side[0], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC20, game_attached_sar(width*game_attached_sar(946*up[0], 10), scale_shift + 12));
    w_u16(0x8010ABE8, center[0] - game_attached_sar(width*game_attached_sar(392*side[0], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*up[0], 10), scale_shift + 12));
    w_u16(0x8010AC20, center[0] + game_attached_sar(width*game_attached_sar(392*side[0], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*up[0], 10), scale_shift + 12));
    w_u16(0x8010ABA8, center[0] + game_attached_sar(3*back[0]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABB8, center[0] + game_attached_sar(3*back[0]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*side[0], 10), scale_shift + 12));
    w_u16(0x8010ABAA, game_attached_sar(width*game_attached_sar(392*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABBA, 0 - game_attached_sar(width*game_attached_sar(392*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABFA, center[1] + game_attached_sar(width*game_attached_sar(392*side[1], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC32, center[1] - game_attached_sar(width*game_attached_sar(392*side[1], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC22, game_attached_sar(width*game_attached_sar(946*up[1], 10), scale_shift + 12));
    w_u16(0x8010ABEA, center[1] - game_attached_sar(width*game_attached_sar(392*side[1], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*up[1], 10), scale_shift + 12));
    w_u16(0x8010AC22, center[1] + game_attached_sar(width*game_attached_sar(392*side[1], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*up[1], 10), scale_shift + 12));
    w_u16(0x8010ABAA, center[1] + game_attached_sar(3*back[1]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABBA, center[1] + game_attached_sar(3*back[1]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*side[1], 10), scale_shift + 12));
    w_u16(0x8010ABAC, game_attached_sar(width*game_attached_sar(392*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABBC, 0 - game_attached_sar(width*game_attached_sar(392*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABFC, center[2] + game_attached_sar(width*game_attached_sar(392*side[2], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC34, center[2] - game_attached_sar(width*game_attached_sar(392*side[2], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC24, game_attached_sar(width*game_attached_sar(946*up[2], 10), scale_shift + 12));
    w_u16(0x8010ABEC, center[2] - game_attached_sar(width*game_attached_sar(392*side[2], 10), scale_shift + 12) - game_attached_sar(width*game_attached_sar(946*up[2], 10), scale_shift + 12));
    w_u16(0x8010AC24, center[2] + game_attached_sar(width*game_attached_sar(392*side[2], 10), scale_shift + 12) + game_attached_sar(width*game_attached_sar(946*up[2], 10), scale_shift + 12));
    w_u16(0x8010ABAC, center[2] + game_attached_sar(3*back[2]*game_attached_sar(thickness, 2), scale_shift + 12) + game_attached_sar(width*game_attached_sar(392*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABBC, center[2] + game_attached_sar(3*back[2]*game_attached_sar(thickness, 2), scale_shift + 12) - game_attached_sar(width*game_attached_sar(392*side[2], 10), scale_shift + 12));
    w_u16(0x8010ABB0, center[0] + game_attached_sar(4*back[0]*game_attached_sar(thickness, 2), scale_shift + 12));
    w_u16(0x8010AC28, game_attached_sar(up[0]*width, scale_shift + 12));
    w_u16(0x8010ABF0, center[0] - game_attached_sar(up[0]*width, scale_shift + 12));
    w_u16(0x8010AC28, center[0] + game_attached_sar(up[0]*width, scale_shift + 12));
    w_u16(0x8010ABB2, center[1] + game_attached_sar(4*back[1]*game_attached_sar(thickness, 2), scale_shift + 12));
    w_u16(0x8010AC2A, game_attached_sar(up[1]*width, scale_shift + 12));
    w_u16(0x8010ABF2, center[1] - game_attached_sar(up[1]*width, scale_shift + 12));
    w_u16(0x8010AC2A, center[1] + game_attached_sar(up[1]*width, scale_shift + 12));
    w_u16(0x8010ABB4, center[2] + game_attached_sar(4*back[2]*game_attached_sar(thickness, 2), scale_shift + 12));
    w_u16(0x8010AC2C, game_attached_sar(up[2]*width, scale_shift + 12));
    w_u16(0x8010AC2C, center[2] + game_attached_sar(up[2]*width, scale_shift + 12));
    w_u16(0x8010ABF4, center[2] - game_attached_sar(up[2]*width, scale_shift + 12));
}

void game_draw_attached_record(uint32 object, uint32 record, uint32 mode)
{
    uint32 value, factor, first, second, third, product, width, thickness, seed, scale, offset;
    uint32 camera, x, y, z, first_x, first_y, first_z, count, combined, index, absolute;
    uint32 direction[3], side[3], up[3], back[3], centers[3];
    uint32 first_depth, second_depth, primary_depth, secondary_depth, primary_color, secondary_color;
    sint32 length, size, screen, projection, flags, initial_depth, center_x, center_y;
    SVECTOR center;
    FUNCTION_MARKER(0x800C2638, "GAME.OVL");
    value = r_u32(record + 0x10);
    if (value + 7 < 15)
        w_u32(record + 0x10, (sint32)value < 0 ? (uint32)-8 : 8);
    factor = r_u32(record + 0x10);
    if ((sint32)factor >= 0) {
        first = r_u32(record + 12);
        second = r_u32(record + 16);
        product = first * second;
        first = r_u32(record + 8);
        second = first * factor;
        seed = r_u32(0x800EF50C);
        thickness = game_attached_sar(product, 8);
        w_u32(0x800FD71C, thickness);
        width = game_attached_sar(first + game_attached_sar(second, 8), 1);
        w_u32(0x80124018, width);
        w_u32(0x800FD71C, thickness + game_attached_sar(thickness * (seed & 7), 5));
    } else {
        first = r_u32(record + 8);
        product = first * (0u - factor);
        w_u32(0x800FD71C, 1);
        w_u32(0x80124018, game_attached_sar(first + game_attached_sar(product, 8), 1));
    }
    scale = r_u32(object + 0x84);
    if ((sint32)scale > 0) {
        value = (uint32)r_s16(record) << (scale & 31);
        w_u32(0x800F4F50, value); w_u32(0x800F4F40, value);
        value = (uint32)r_s16(record + 2);
        scale = r_u32(object + 0x84);
        value <<= scale & 31;
        w_u32(0x800F4F54, value); w_u32(0x800F4F44, value);
        offset = r_u32(record + 0x1C);
        value = (uint32)r_s16(record + 4);
        scale = r_u32(object + 0x84);
        value <<= scale & 31;
        if (offset == 0)
            value -= game_attached_sar(r_u32(0x80124018), 2);
        else
            value -= offset;
        w_u32(0x800F4F48, value);
    } else {
        value = (uint32)r_s16(record);
        value = game_attached_sar(value, 0u - scale);
        w_u32(0x800F4F50, value); w_u32(0x800F4F40, value);
        scale = r_u32(object + 0x84);
        value = (uint32)r_s16(record + 2);
        value = game_attached_sar(value, 0u - scale);
        w_u32(0x800F4F54, value); w_u32(0x800F4F44, value);
        offset = r_u32(record + 0x1C);
        scale = r_u32(object + 0x84);
        value = (uint32)r_s16(record + 4);
        if (offset == 0) {
            width = r_u32(0x80124018);
            value = game_attached_sar(value, 0u - scale) - game_attached_sar(width, 2);
        } else
            value = game_attached_sar(value, 0u - scale) - offset;
        w_u32(0x800F4F48, value);
    }
    first = r_u32(0x800F4F48); second = r_u32(0x800FD71C);
    w_u32(0x800F4F58, first - second);
    game_attached_transform_point(object + 0x11C, 0x800F4F40);
    game_attached_transform_point(object + 0x11C, 0x800F4F50);
    camera = r_u32(0x80040C94);
    first = r_u32(object + 8); second = r_u32(camera + 8); third = r_u32(0x800F4F40);
    first_x = third + first - second;
    w_u32(0x800F4F40, first_x);
    first = r_u32(object + 8); second = r_u32(camera + 8); third = r_u32(0x800F4F50);
    w_u32(0x800F4F50, third + first - second);
    first = r_u32(object + 12); second = r_u32(camera + 12); third = r_u32(0x800F4F44);
    first_y = third + first - second;
    w_u32(0x800F4F44, first_y);
    first = r_u32(object + 12); second = r_u32(camera + 12); third = r_u32(0x800F4F54);
    w_u32(0x800F4F54, third + first - second);
    first = r_u32(object + 16); second = r_u32(camera + 16); third = r_u32(0x800F4F58);
    w_u32(0x800F4F58, third + first - second);
    first = r_u32(object + 16); second = r_u32(camera + 16); third = r_u32(0x800F4F48);
    first_z = third + first - second;
    w_u32(0x800F4F48, first_z);
    length = vector_length_approx((sint32)first_x, (sint32)first_y, (sint32)first_z);
    value = r_u32(record + 8);
    size = game_hud_divide(value << 8, (uint32)(length >> 8) + 1);
    if (size < 3000) {
        game_draw_attached_record_small(object, record);
        return;
    }
    combined = 0;
    for (index = 0; index < 6; ++index) {
        value = r_u32(index < 3 ? 0x800F4F40 + index * 4 : 0x800F4F50 + (index - 3) * 4);
        absolute = (sint32)value < 0 ? 0u - value : value;
        combined |= absolute;
    }
    count = 0;
    combined = game_attached_sar(combined, 14);
    if (combined != 0) {
        do {
            combined >>= 1;
            ++count;
        } while (combined != 0);
    }
    x = r_u32(0x800F4F40); y = r_u32(0x800F4F44); z = r_u32(0x800F4F48);
    center.vx = (sint16)game_attached_sar(x, count);
    center.vy = (sint16)game_attached_sar(y, count);
    center.vz = (sint16)game_attached_sar(z, count);
    direction[0] = (uint32)(sint32)center.vx;
    direction[1] = (uint32)(sint32)center.vy;
    direction[2] = (uint32)(sint32)center.vz;
    game_attached_normalize(direction);
    back[1] = 0u - (uint32)r_s16(object + 0x126);
    back[2] = 0u - (uint32)r_s16(object + 0x12C);
    side[0] = game_attached_sar(back[2] * direction[1] - back[1] * direction[2], 12);
    back[0] = 0u - (uint32)r_s16(object + 0x120);
    side[1] = game_attached_sar(back[0] * direction[2] - back[2] * direction[0], 12);
    side[2] = game_attached_sar(back[1] * direction[0] - back[0] * direction[1], 12);
    game_attached_normalize(side);
    up[0] = game_attached_sar(side[1] * direction[2] - side[2] * direction[1], 12);
    up[1] = game_attached_sar(side[2] * direction[0] - side[0] * direction[2], 12);
    up[2] = game_attached_sar(side[0] * direction[1] - side[1] * direction[0], 12);
    game_attached_normalize(up);
    matrix_copy_rotation(0x80118058, 0x80034D58);
    matrix_shift(0x80118058, 0x80124C28, count);
    w_u32(0x80034D6C, 0); w_u32(0x80034D70, 0); w_u32(0x80034D74, 0);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    SetRotMatrix((MATRIX *)psx_addr(0x80124C28, 20));
    initial_depth = RotTransPers(&center, &screen, &projection, &flags);
    centers[0] = (uint16)center.vx; centers[1] = (uint16)center.vy; centers[2] = (uint16)center.vz;
    game_attached_build_geometry(centers, side, up, back, count);
    first_depth = (uint32)initial_depth << ((count * 2) & 31);
    second_depth = (uint32)game_attached_project_outline() << ((count * 2) & 31);
    w_u32(0x8005110C, second_depth);
    center_x = (sint16)screen;
    center_y = (sint16)((uint32)screen >> 16);
    game_attached_select_screen_point(0x80033D58, 0x80033D94, 0x80033DAC, 0x80033D70, center_x, center_y);
    game_attached_select_screen_point(0x80033D64, 0x80033DA0, 0, 0, center_x, center_y);
    game_attached_select_screen_point(0x80033D60, 0x80033D9C, 0x80033DA4, 0x80033D68, center_x, center_y);
    game_attached_select_screen_point(0x80033D5C, 0x80033D98, 0x80033DA8, 0x80033D6C, center_x, center_y);
    if (first_depth == 0 && second_depth == 0)
        return;
    flags = (sint32)r_u32(object + 0x114);
    secondary_color = r_u32(record + 0x14);
    if (((uint32)flags & 0x10) != 0) {
        primary_depth = (first_depth + second_depth - 100) * 2;
        secondary_depth = primary_depth;
        first = r_u32(object + 4);
        second = r_u32(object + 0xEC);
        third = r_u32(0x8005369C + first * 60);
        primary_color = (sint32)third < (sint32)second ? 255 : r_u32(record + 0x18);
    } else {
        if (mode == 1) {
            value = first_depth + second_depth;
            primary_depth = value * 2 - 20;
            secondary_depth = (first_depth + game_attached_sar(value, 1)) * 2 - 20;
        } else {
            primary_depth = 0;
            secondary_depth = 0;
        }
        primary_color = r_u32(record + 0x18);
    }
    game_attached_emit_packets((uint32)screen, primary_depth, secondary_depth, primary_color, secondary_color);
    game_attached_apply_player_contact(object, primary_color);
}
