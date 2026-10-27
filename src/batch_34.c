#include "game_functions.h"
#include <stdlib.h>

static uint32 game_small_attached_sar(uint32 value, uint32 shift)
{
    return (uint32)((sint32)value >> (shift & 31));
}

static sint32 game_small_attached_divide(uint32 numerator, uint32 denominator)
{
    if (denominator == 0 || (numerator == 0x80000000u && denominator == 0xFFFFFFFFu))
        abort();
    return (sint32)numerator / (sint32)denominator;
}

static void game_small_attached_normalize(uint32 vector[3])
{
    uint32 x = vector[0], y = vector[1], z = vector[2];
    uint32 ax, ay, az, larger, smaller, length;
    ax = (sint32)x < 0 ? 0u - x : x;
    ay = (sint32)y < 0 ? 0u - y : y;
    az = (sint32)z < 0 ? 0u - z : z;
    larger = (sint32)ax < (sint32)az ? az : ax;
    smaller = (sint32)ax < (sint32)az ? ax : az;
    length = game_small_attached_sar((larger << 4) + smaller * 5, 4);
    larger = (sint32)length < (sint32)ay ? ay : length;
    smaller = (sint32)length < (sint32)ay ? length : ay;
    length = game_small_attached_sar((larger << 4) + smaller * 5, 4) + 1;
    vector[0] = (uint32)game_small_attached_divide(x << 12, length);
    vector[1] = (uint32)game_small_attached_divide(y << 12, length);
    vector[2] = (uint32)game_small_attached_divide(z << 12, length);
}

static void game_small_attached_geometry(const uint32 side[3], const uint32 up[3], const uint32 back[3], const uint32 centers[3], uint32 scale_shift)
{
    uint32 thickness = r_u32(0x800FD71C);
    uint32 width = r_u32(0x80124018);
    w_u32(0x800FD71C, game_small_attached_sar(thickness, 2));
    w_u16(0x8010AB90, game_small_attached_sar((side[0] * width), (12 + scale_shift)));
    w_u16(0x8010ABD0, (centers[0] + ((0u - 1) * game_small_attached_sar((side[0] * width), (12 + scale_shift)))));
    w_u16(0x8010AB90, (centers[0] + game_small_attached_sar((side[0] * width), (12 + scale_shift))));
    w_u16(0x8010AB92, game_small_attached_sar((side[1] * width), (12 + scale_shift)));
    w_u16(0x8010AB92, (centers[1] + game_small_attached_sar((side[1] * width), (12 + scale_shift))));
    w_u16(0x8010ABD2, (centers[1] + ((0u - 1) * game_small_attached_sar((side[1] * width), (12 + scale_shift)))));
    w_u16(0x8010AB94, game_small_attached_sar((side[2] * width), (12 + scale_shift)));
    w_u16(0x8010ABD4, (centers[2] + ((0u - 1) * game_small_attached_sar((side[2] * width), (12 + scale_shift)))));
    w_u16(0x8010AB94, (centers[2] + game_small_attached_sar((side[2] * width), (12 + scale_shift))));
    w_u16(0x8010ABA0, game_small_attached_sar((width * game_small_attached_sar((724 * side[0]), 10)), (12 + scale_shift)));
    w_u16(0x8010ABC0, ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[0]), 10)), (12 + scale_shift))));
    w_u16(0x8010AC00, (centers[0] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * up[0]), 10)), (12 + scale_shift))) + game_small_attached_sar((width * game_small_attached_sar((724 * side[0]), 10)), (12 + scale_shift))));
    w_u16(0x8010AC38, (centers[0] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[0]), 10)), (12 + scale_shift))) + game_small_attached_sar((width * game_small_attached_sar((724 * up[0]), 10)), (12 + scale_shift))));
    w_u16(0x8010AC18, game_small_attached_sar((width * game_small_attached_sar((724 * up[0]), 10)), (12 + scale_shift)));
    w_u16(0x8010ABE0, (centers[0] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[0]), 10)), (12 + scale_shift))) + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * up[0]), 10)), (12 + scale_shift)))));
    w_u16(0x8010AC18, (centers[0] + game_small_attached_sar((width * game_small_attached_sar((724 * side[0]), 10)), (12 + scale_shift)) + game_small_attached_sar((width * game_small_attached_sar((724 * up[0]), 10)), (12 + scale_shift))));
    w_u16(0x8010ABA0, (centers[0] + game_small_attached_sar((width * game_small_attached_sar((724 * side[0]), 10)), (12 + scale_shift)) + game_small_attached_sar((2 * back[0] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010ABC0, (centers[0] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[0]), 10)), (12 + scale_shift))) + game_small_attached_sar((2 * back[0] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010AC02, (centers[1] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * up[1]), 10)), (12 + scale_shift))) + game_small_attached_sar((width * game_small_attached_sar((724 * side[1]), 10)), (12 + scale_shift))));
    w_u16(0x8010AC3A, (centers[1] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[1]), 10)), (12 + scale_shift))) + game_small_attached_sar((width * game_small_attached_sar((724 * up[1]), 10)), (12 + scale_shift))));
    w_u16(0x8010AC1A, game_small_attached_sar((width * game_small_attached_sar((724 * up[1]), 10)), (12 + scale_shift)));
    w_u16(0x8010ABA2, game_small_attached_sar((width * game_small_attached_sar((724 * side[1]), 10)), (12 + scale_shift)));
    w_u16(0x8010ABC2, ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[1]), 10)), (12 + scale_shift))));
    w_u16(0x8010ABA2, (centers[1] + game_small_attached_sar((width * game_small_attached_sar((724 * side[1]), 10)), (12 + scale_shift)) + game_small_attached_sar((2 * back[1] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010ABE2, (centers[1] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[1]), 10)), (12 + scale_shift))) + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * up[1]), 10)), (12 + scale_shift)))));
    w_u16(0x8010AC1A, (centers[1] + game_small_attached_sar((width * game_small_attached_sar((724 * side[1]), 10)), (12 + scale_shift)) + game_small_attached_sar((width * game_small_attached_sar((724 * up[1]), 10)), (12 + scale_shift))));
    w_u16(0x8010ABC2, (centers[1] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[1]), 10)), (12 + scale_shift))) + game_small_attached_sar((2 * back[1] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010ABA4, game_small_attached_sar((width * game_small_attached_sar((724 * side[2]), 10)), (12 + scale_shift)));
    w_u16(0x8010ABC4, ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[2]), 10)), (12 + scale_shift))));
    w_u16(0x8010AC1C, game_small_attached_sar((width * game_small_attached_sar((724 * up[2]), 10)), (12 + scale_shift)));
    w_u16(0x8010AC04, (centers[2] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * up[2]), 10)), (12 + scale_shift))) + game_small_attached_sar((width * game_small_attached_sar((724 * side[2]), 10)), (12 + scale_shift))));
    w_u16(0x8010AC3C, (centers[2] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[2]), 10)), (12 + scale_shift))) + game_small_attached_sar((width * game_small_attached_sar((724 * up[2]), 10)), (12 + scale_shift))));
    w_u16(0x8010ABA4, (centers[2] + game_small_attached_sar((width * game_small_attached_sar((724 * side[2]), 10)), (12 + scale_shift)) + game_small_attached_sar((2 * back[2] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010ABE4, (centers[2] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[2]), 10)), (12 + scale_shift))) + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * up[2]), 10)), (12 + scale_shift)))));
    w_u16(0x8010AC1C, (centers[2] + game_small_attached_sar((width * game_small_attached_sar((724 * side[2]), 10)), (12 + scale_shift)) + game_small_attached_sar((width * game_small_attached_sar((724 * up[2]), 10)), (12 + scale_shift))));
    w_u16(0x8010ABC4, (centers[2] + ((0u - 1) * game_small_attached_sar((width * game_small_attached_sar((724 * side[2]), 10)), (12 + scale_shift))) + game_small_attached_sar((2 * back[2] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010AC28, game_small_attached_sar((up[0] * width), (12 + scale_shift)));
    w_u16(0x8010ABF0, (centers[0] + ((0u - 1) * game_small_attached_sar((up[0] * width), (12 + scale_shift)))));
    w_u16(0x8010ABB0, (centers[0] + game_small_attached_sar((4 * back[0] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010AC28, (centers[0] + game_small_attached_sar((up[0] * width), (12 + scale_shift))));
    w_u16(0x8010AC2A, game_small_attached_sar((up[1] * width), (12 + scale_shift)));
    w_u16(0x8010ABB4, (centers[2] + game_small_attached_sar((4 * back[2] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010ABB2, (centers[1] + game_small_attached_sar((4 * back[1] * game_small_attached_sar(thickness, 2)), (12 + scale_shift))));
    w_u16(0x8010AC2C, game_small_attached_sar((up[2] * width), (12 + scale_shift)));
    w_u16(0x8010ABF2, (centers[1] + ((0u - 1) * game_small_attached_sar((up[1] * width), (12 + scale_shift)))));
    w_u16(0x8010AC2A, (centers[1] + game_small_attached_sar((up[1] * width), (12 + scale_shift))));
    w_u16(0x8010ABF4, (centers[2] + ((0u - 1) * game_small_attached_sar((up[2] * width), (12 + scale_shift)))));
    w_u16(0x8010AC2C, (centers[2] + game_small_attached_sar((up[2] * width), (12 + scale_shift))));
}

static sint32 game_small_attached_project(uint32 first, uint32 second, uint32 third, uint32 first_screen, uint32 second_screen, uint32 third_screen)
{
    uint32 vertex, address;
    uint32 vertices[3];
    SVECTOR input[3];
    sint32 screen[3], depth[3], flags;
    vertices[0] = first; vertices[1] = second; vertices[2] = third;
    for (vertex = 0; vertex < 3; ++vertex) {
        address = vertices[vertex];
        input[vertex].vx = r_s16(address);
        input[vertex].vy = r_s16(address + 2);
        input[vertex].vz = r_s16(address + 4);
    }
    gte_project3_full_depth(input, screen, depth, &flags);
    w_u32(first_screen, (uint32)screen[0]);
    w_u32(second_screen, (uint32)screen[1]);
    w_u32(third_screen, (uint32)screen[2]);
    return depth[2] >> 2;
}

static void game_small_attached_select_point(uint32 destination, uint32 alternate, uint32 extra_source, uint32 extra_destination, sint32 center_x, sint32 center_y)
{
    uint32 x, y, next_x, next_y, first_distance, second_distance, extra_x, extra_y;
    x = (uint32)r_s16(destination);
    x = game_small_attached_sar(x - (uint32)center_x, 1);
    y = (uint32)r_s16(destination + 2) - (uint32)center_y;
    first_distance = x * x + y * y;
    next_x = (uint32)r_s16(alternate);
    x = game_small_attached_sar(next_x - (uint32)center_x, 1);
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

static void game_small_attached_emit_packets(uint32 center, uint32 primary_depth, uint32 secondary_depth, uint32 primary_color, uint32 secondary_color)
{
    uint32 step = 1, third = 4, current = 0x80033D54;
    uint32 first, second, packet, value, coordinate;
    do {
        first = step + 2;
        if (first >= 17)
            first = step - 14;
        second = step + 4;
        if (second >= 17)
            second = step - 12;
        first *= 4;
        second *= 4;
        packet = r_u32(0x80040690);
        w_u32(0x80040690, packet + 144);
        value = r_u32(0x80033D50 + second);
        w_u32(packet + 8, value);
        step += 4;
        w_u32(packet + 16, center);
        value = r_u32(0x80033D50 + first);
        w_u32(packet + 24, value);
        value = r_u32(current);
        w_u8(packet + 3, 8);
        w_u8(packet + 7, 0x38);
        w_u32(packet + 4, 0x3A000000);
        w_u32(packet + 12, primary_color);
        w_u32(packet + 20, 0);
        w_u32(packet + 28, 0);
        w_u32(packet + 32, value);
        render_insert_primitive((sint32)primary_depth, packet);
        value = (uint32)r_s16(0x80033D52 + second);
        packet = r_u32(0x80040690);
        value = game_small_attached_sar(value + (uint32)(sint32)(sint16)(center >> 16), 1);
        w_u16(packet + 10, value);
        coordinate = (uint32)r_s16(0x80033D50 + second);
        w_u32(0x80040690, packet + 144);
        coordinate = game_small_attached_sar(coordinate + (uint32)(sint32)(sint16)center, 1);
        w_u32(packet + 16, center);
        w_u16(packet + 8, coordinate);
        value = (uint32)r_s16(0x80033D50 + first);
        current += 16;
        value = game_small_attached_sar(value + (uint32)(sint32)(sint16)center, 1);
        w_u16(packet + 24, value);
        value = (uint32)r_s16(0x80033D52 + first);
        value = game_small_attached_sar(value + (uint32)(sint32)(sint16)(center >> 16), 1);
        w_u16(packet + 26, value);
        value = (uint32)r_s16(0x80033D50 + third);
        value = game_small_attached_sar(value + (uint32)(sint32)(sint16)center, 1);
        w_u16(packet + 32, value);
        value = (uint32)r_s16(0x80033D52 + third);
        w_u8(packet + 3, 8);
        w_u8(packet + 7, 0x38);
        w_u32(packet + 4, 0x3A000000);
        third += 16;
        w_u32(packet + 20, 0);
        w_u32(packet + 28, 0);
        value = game_small_attached_sar(value + (uint32)(sint32)(sint16)(center >> 16), 1);
        w_u32(packet + 12, secondary_color);
        w_u16(packet + 34, value);
        render_insert_primitive((sint32)secondary_depth, packet);
    } while (step < 17);
}

void game_draw_attached_record_small(uint32 object, uint32 record)
{
    uint32 x, y, z, value, combined, count, index, absolute;
    uint32 direction[3], side[3], up[3], back[3], centers[3];
    uint32 first_depth, second_depth, primary_depth, secondary_depth, primary_color, secondary_color, flags_value;
    sint32 screen, projection, flags, initial_depth;
    SVECTOR center;
    x = r_u32(0x800F4F40);
    y = r_u32(0x800F4F44);
    FUNCTION_MARKER(0x800C4604, "GAME.OVL");
    z = r_u32(0x800F4F48);
    x = (sint32)x < 0 ? 0u - x : x;
    y = (sint32)y < 0 ? 0u - y : y;
    z = (sint32)z < 0 ? 0u - z : z;
    combined = x | y | z;
    for (index = 0; index < 3; ++index) {
        value = r_u32(0x800F4F50 + index * 4);
        absolute = (sint32)value < 0 ? 0u - value : value;
        combined |= absolute;
    }
    combined = game_small_attached_sar(combined, 14);
    count = 0;
    while (combined != 0) {
        combined >>= 1;
        ++count;
    }
    if (count >= 6)
        return;
    x = r_u32(0x800F4F40);
    z = r_u32(0x800F4F48);
    center.vx = (sint16)game_small_attached_sar(x, count);
    y = r_u32(0x800F4F44);
    center.vz = (sint16)game_small_attached_sar(z, count);
    center.vy = (sint16)game_small_attached_sar(y, count);
    matrix_copy_rotation(0x80118058, 0x80034D58);
    matrix_shift(0x80118058, 0x80124C28, count);
    w_u32(0x80034D6C, 0); w_u32(0x80034D70, 0); w_u32(0x80034D74, 0);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    SetRotMatrix((MATRIX *)psx_addr(0x80124C28, 20));
    initial_depth = RotTransPers(&center, &screen, &projection, &flags);
    first_depth = (uint32)initial_depth << ((count * 2) & 31);
    direction[0] = (uint32)(sint32)center.vx;
    direction[1] = (uint32)(sint32)center.vy;
    direction[2] = (uint32)(sint32)center.vz;
    game_small_attached_normalize(direction);
    back[1] = 0u - (uint32)r_s16(object + 0x126);
    back[2] = 0u - (uint32)r_s16(object + 0x12C);
    side[0] = game_small_attached_sar(back[2] * direction[1] - back[1] * direction[2], 12);
    back[0] = 0u - (uint32)r_s16(object + 0x120);
    side[1] = game_small_attached_sar(back[0] * direction[2] - back[2] * direction[0], 12);
    side[2] = game_small_attached_sar(back[1] * direction[0] - back[0] * direction[1], 12);
    game_small_attached_normalize(side);
    up[0] = game_small_attached_sar(side[1] * direction[2] - side[2] * direction[1], 12);
    up[1] = game_small_attached_sar(side[2] * direction[0] - side[0] * direction[2], 12);
    up[2] = game_small_attached_sar(side[0] * direction[1] - side[1] * direction[0], 12);
    game_small_attached_normalize(up);
    centers[0] = (uint16)center.vx; centers[1] = (uint16)center.vy; centers[2] = (uint16)center.vz;
    game_small_attached_geometry(side, up, back, centers, count);
    second_depth = (uint32)game_small_attached_project(0x8010AB90, 0x8010ABA0, 0x8010ABB0, 0x80033D54, 0x80033D5C, 0x80033D64);
    w_u32(0x8005110C, second_depth << ((count * 2) & 31));
    game_small_attached_project(0x8010ABC0, 0x8010ABD0, 0x8010ABE0, 0x80033D6C, 0x80033D74, 0x80033D7C);
    game_small_attached_project(0x8010ABF0, 0x8010AC00, 0x8010AC18, 0x80033D84, 0x80033D8C, 0x80033D98);
    game_small_attached_project(0x8010AC28, 0x8010AC38, 0x8010AC38, 0x80033DA0, 0x80033DA8, 0x80033DA8);
    game_small_attached_select_point(0x80033D64, 0x80033DA0, 0, 0, (sint16)screen, (sint16)((uint32)screen >> 16));
    game_small_attached_select_point(0x80033D5C, 0x80033D98, 0x80033DA8, 0x80033D6C, (sint16)screen, (sint16)((uint32)screen >> 16));
    if (first_depth == 0 && r_u32(0x8005110C) == 0)
        return;
    flags_value = r_u32(object + 0x114);
    secondary_color = r_u32(record + 0x14);
    second_depth = r_u32(0x8005110C);
    if ((flags_value & 0x10) != 0) {
        primary_depth = (first_depth + second_depth - 100) * 2;
        secondary_depth = primary_depth;
        index = r_u32(object + 4);
        value = r_u32(object + 0xEC);
        if (r_s32(0x8005369C + index * 60) < (sint32)value)
            primary_color = 0xFF;
        else
            primary_color = r_u32(record + 0x18);
    } else {
        primary_color = r_u32(record + 0x18);
        value = first_depth + second_depth;
        primary_depth = value * 2 - 20;
        secondary_depth = (first_depth + game_small_attached_sar(value, 1)) * 2 - 20;
    }
    game_small_attached_emit_packets((uint32)screen, primary_depth, secondary_depth, primary_color, secondary_color);
}

static void game_target_beam_advance(void)
{
    uint32 x, y, z, dx, dy, dz;
    x = r_u32(0x800FD508); dx = r_u32(0x80108A30);
    w_u32(0x800FD508, x + dx);
    y = r_u32(0x800FD50C); dy = r_u32(0x80108A34);
    z = r_u32(0x800FD510); dz = r_u32(0x80108A38);
    w_u32(0x800FD50C, y + dy);
    w_u32(0x800FD510, z + dz);
}

static void game_target_beam_limit_step(sint32 limit)
{
    uint32 x, y, z;
    sint32 length;
    for (;;) {
        x = r_u32(0x80108A30); y = r_u32(0x80108A34); z = r_u32(0x80108A38);
        length = vector_length_approx((sint32)x, (sint32)y, (sint32)z);
        if (length < limit)
            return;
        x = r_u32(0x80108A30); z = r_u32(0x80108A38);
        w_u32(0x80108A30, game_small_attached_sar(x, 1));
        y = r_u32(0x80108A34);
        w_u32(0x80108A38, game_small_attached_sar(z, 1));
        w_u32(0x80108A34, game_small_attached_sar(y, 1));
    }
}

static void game_target_beam_project(uint32 x, uint32 y, uint32 z, uint32 index)
{
    uint32 shift = 2, ax, ay, az;
    SVECTOR point;
    sint32 screen, projection, flags, depth;
    for (;;) {
        ax = (sint32)x < 0 ? 0u - x : x;
        ay = (sint32)y < 0 ? 0u - y : y;
        az = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)ax < 0x4000 && (sint32)ay < 0x4000 && (sint32)az < 0x4000)
            break;
        ++shift;
        x = game_small_attached_sar(x, 1);
        y = game_small_attached_sar(y, 1);
        z = game_small_attached_sar(z, 1);
    }
    point.vx = (sint16)x; point.vy = (sint16)y; point.vz = (sint16)z;
    depth = RotTransPers(&point, &screen, &projection, &flags);
    w_u32(0x80033D50 + index * 4, (uint32)screen);
    w_u32(0x800510F8 + index * 4, (uint32)projection);
    w_u32(0x800510F8 + index * 4, (uint32)depth << (shift & 31));
}

void game_draw_target_beam_native(VECTOR *start, VECTOR *end, uint32 mode)
{
    uint32 packet, value, first, second, camera, x, y, z, end_x, end_y, end_z;
    uint32 uvs[4], page, clut, row, vertex, index, next, color, angle;
    uint32 dx, dy, dz;
    VECTOR forward, up;
    FUNCTION_MARKER(0x800C828C, "GAME.OVL");
    packet = r_u32(0x80040690);
    value = r_u32(0x80126AB0);
    w_u8(packet + 12, value);
    value = r_u32(0x80126B30);
    w_u8(packet + 13, value);
    first = r_u32(0x800F3BF8); second = r_u32(0x80126AB0);
    w_u8(packet + 20, second + first - 1);
    value = r_u32(0x80126B30);
    w_u8(packet + 21, value);
    value = r_u32(0x80126AB0);
    w_u8(packet + 28, value);
    first = r_u32(0x800F3300); second = r_u32(0x80126B30);
    w_u8(packet + 29, second + first - 1);
    first = r_u32(0x800F3BF8); second = r_u32(0x80126AB0);
    w_u8(packet + 36, second + first - 1);
    first = r_u32(0x800F3300); second = r_u32(0x80126B30);
    w_u8(packet + 37, second + first - 1);
    first = r_u32(0x800F3C80); second = r_u32(0x800F3D28);
    value = GetTPage(0, 1, (sint32)first, (sint32)second);
    w_u16(packet + 22, value);
    first = r_u32(0x800F31D8); second = r_u32(0x800F3258);
    clut = GetClut((sint32)first, (sint32)second);
    page = r_u16(packet + 22);
    camera = r_u32(0x80040C94);
    uvs[0] = r_u16(packet + 12);
    uvs[1] = r_u16(packet + 20);
    uvs[2] = r_u16(packet + 28);
    uvs[3] = r_u16(packet + 36);
    w_u16(packet + 14, clut);
    up.vx = 0; up.vy = -4096; up.vz = 0;
    w_u32(0x80034D6C, 0); w_u32(0x80034D70, 0); w_u32(0x80034D74, 0);
    first = r_u32(camera + 8); second = (uint32)start->vx;
    x = second - first;
    w_u32(0x800FD508, x);
    first = (uint32)start->vy; second = r_u32(camera + 12);
    y = first - second;
    w_u32(0x800FD50C, y);
    first = (uint32)start->vz; second = r_u32(camera + 16);
    z = first - second;
    w_u32(0x800FD510, z);
    first = (uint32)end->vx; second = r_u32(camera + 8);
    end_x = first - second;
    w_u32(0x800FB428, end_x);
    first = (uint32)end->vy; second = r_u32(camera + 12);
    end_y = first - second;
    w_u32(0x800FB42C, end_y);
    first = (uint32)end->vz; second = r_u32(camera + 16);
    w_u32(0x80108A30, game_small_attached_sar(end_x - x, 5));
    w_u32(0x80108A34, game_small_attached_sar(end_y - y, 5));
    end_z = first - second;
    w_u32(0x800FB430, end_z);
    w_u32(0x80108A38, game_small_attached_sar(end_z - z, 5));
    if ((sint32)mode > 0) {
        game_target_beam_limit_step(20001);
        forward.vx = r_s32(0x80108A30); forward.vy = r_s32(0x80108A34); forward.vz = r_s32(0x80108A38);
        matrix_basis_from_vectors(&forward, &up, 0x800F3398);
        for (vertex = 0; vertex < 8; ++vertex) {
            angle = (vertex << 9) + (r_u32(0x800311B8) << 6);
            value = (uint32)rsin((sint32)angle) >> 2;
            w_u16(0x800FB580 + vertex * 8, value);
            value = (uint32)rcos((sint32)angle) >> 2;
            w_u16(0x800FB582 + vertex * 8, value);
            w_u16(0x800FB584 + vertex * 8, 0);
            ApplyMatrixSV((MATRIX *)psx_addr(0x800F3398, 20), (SVECTOR *)psx_addr(0x800FB580 + vertex * 8, 8), (SVECTOR *)psx_addr(0x800FB580 + vertex * 8, 8));
        }
        SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
        for (row = 0; row < 256; row += 8) {
            for (vertex = 0; vertex < 8; ++vertex) {
                if (row > 0) {
                    first = (uint32)r_s16(0x800FB580 + vertex * 8); second = r_u32(0x800FD508);
                    x = second + first;
                    first = (uint32)r_s16(0x800FB582 + vertex * 8); second = r_u32(0x800FD50C);
                    y = second + first;
                    first = (uint32)r_s16(0x800FB584 + vertex * 8); second = r_u32(0x800FD510);
                    z = second + first;
                } else {
                    x = r_u32(0x800FD508); y = r_u32(0x800FD50C); z = r_u32(0x800FD510);
                }
                game_target_beam_project(x, y, z, row + vertex);
            }
            game_target_beam_advance();
        }
        for (row = 0; row < 248; row += 8) {
            value = (256 - row) >> 2;
            color = value + (value << 8) + (value << 16);
            for (vertex = 0; vertex < 8; ++vertex) {
                index = row + vertex;
                if (r_s32(0x800510F8 + index * 4) <= 0)
                    continue;
                next = row + ((vertex + 1) & 7);
                packet = r_u32(0x80040690);
                w_u32(0x80040690, packet + 40);
                w_u32(packet + 4, color);
                SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
                w_u16(packet + 12, uvs[1]); w_u16(packet + 20, uvs[3]);
                w_u16(packet + 28, uvs[0]); w_u16(packet + 36, uvs[2]);
                w_u16(packet + 22, page); w_u16(packet + 14, clut);
                value = r_u32(0x80033D50 + index * 4); w_u32(packet + 8, value);
                value = r_u32(0x80033D50 + next * 4); w_u32(packet + 16, value);
                value = r_u32(0x80033D70 + index * 4); w_u32(packet + 24, value);
                value = r_u32(0x80033D70 + next * 4); w_u32(packet + 32, value);
                SetSemiTrans(psx_addr(packet, 40), 1);
                render_insert_primitive(r_s32(0x80051118 + index * 4), packet);
            }
        }
        if ((uint32)mode - 2 < 126) {
            dx = r_u32(0x80108A30) * (uint32)mode;
            dy = r_u32(0x80108A34) * (uint32)mode;
            dz = r_u32(0x80108A38) * (uint32)mode;
            value = (uint32)start->vx;
            w_u32(0x800FD508, value + dx);
            value = (uint32)start->vy; x = r_u32(0x800FD508);
            w_u32(0x800FD50C, value + dy);
            value = (uint32)start->vz; y = r_u32(0x800FD50C);
            w_u32(0x800FD510, value + dz);
            z = r_u32(0x800FD510);
            game_emit_radial_effect((sint32)x, (sint32)y, (sint32)z, 9000, 0xFFFFFF);
            x = r_u32(0x800FD508); y = r_u32(0x800FD50C); z = r_u32(0x800FD510);
            game_emit_radial_effect((sint32)x, (sint32)y, (sint32)z, 15000, 0xFF0000);
        }
    } else {
        SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
        game_target_beam_limit_step(4001);
        for (index = 0; index < 64; ++index) {
            x = r_u32(0x800FD508); y = r_u32(0x800FD50C); z = r_u32(0x800FD510);
            game_target_beam_project(x, y, z, index);
            game_target_beam_advance();
        }
        for (index = 0; index < 63; ++index) {
            if (r_s32(0x800510F8 + index * 4) <= 0)
                continue;
            packet = r_u32(0x80040690);
            w_u32(0x80040690, packet + 16);
            w_u32(packet + 4, (63 - index) << 2);
            setLineF2((LINE_F2 *)psx_addr(packet, 16));
            value = r_u32(0x80033D50 + index * 4); w_u32(packet + 8, value);
            value = r_u32(0x80033D54 + index * 4); w_u32(packet + 12, value);
            SetSemiTrans(psx_addr(packet, 16), 1);
            render_insert_primitive(r_s32(0x800510FC + index * 4), packet);
        }
    }
}

static void game_target_field_project(uint32 depth_base)
{
    uint32 group, vertex;
    sint32 screen[3], depth[3], flags;
    for (group = 0; group < 341; ++group) {
        gte_project3_full_depth((const SVECTOR *)psx_addr(0x800F8210 + group * 24, 24), screen, depth, &flags);
        for (vertex = 0; vertex < 3; ++vertex)
            w_u32(0x80033D50 + group * 12 + vertex * 4, (uint32)screen[vertex]);
        for (vertex = 0; vertex < 3; ++vertex)
            w_u16(depth_base + group * 6 + vertex * 2, (uint32)depth[vertex]);
        w_u16(0x80059660 + group * 2, (uint32)flags >> 12);
    }
}

void game_draw_target_effect_field(uint32 phase)
{
    uint32 seed = 0x12345678;
    uint32 depth_base, value, angle, brightness, index, row, column;
    uint32 initial, current, previous, envelope, next_column, next_row;
    uint32 first, second, third, fourth, packet, screen_first, screen_second, screen_third;
    sint32 screen, projection, flags, depth;
    FUNCTION_MARKER(0x800E3EA8, "GAME.OVL");
    if ((sint32)phase >= 256)
        phase = 255;
    depth_base = r_u32(0x800FD718);
    game_target_field_project(depth_base);
    depth = RotTransPers((SVECTOR *)psx_addr(0x800FA208, 8), &screen, &projection, &flags);
    w_u32(0x80034D4C, (uint32)screen);
    depth_base = r_u32(0x800FD718);
    w_u16(depth_base + 0x7FE, (uint32)depth << 2);
    for (index = 0; index < 1024; ++index) {
        seed = game_small_attached_sar(seed, 1) ^ (seed << 3);
        angle = (r_u32(0x800311B8) << 5) + (seed & 0xFFF);
        value = (uint32)rsin((sint32)angle);
        brightness = game_small_attached_sar(value * 80, 12) + 224;
        if ((sint32)brightness >= 256)
            brightness = 255;
        w_u8(0x80059660 + index, brightness);
    }
    initial = phase;
    current = game_small_attached_sar(phase * 255, 8);
    previous = current;
    envelope = 255;
    for (row = 0; row < 31; ++row) {
        if ((sint32)current > 0) {
            for (column = 0; column < 32; ++column) {
                first = row * 32 + column;
                depth_base = r_u32(0x800FD718);
                if (r_s16(depth_base + first * 2) < 2401)
                    continue;
                next_column = (column + 1) & 31;
                next_row = (row + 1) & 31;
                second = row * 32 + next_column;
                third = next_row * 32 + column;
                fourth = next_row * 32 + next_column;
                screen_first = r_u32(0x80033D50 + second * 4);
                screen_second = r_u32(0x80033D50 + fourth * 4);
                screen_third = r_u32(0x80033D50 + third * 4);
                if (NormalClip((sint32)screen_first, (sint32)screen_second, (sint32)screen_third) >= 0)
                    continue;
                packet = r_u32(0x80040690);
                w_u32(0x80040690, packet + 36);
                value = r_u32(0x80033D50 + first * 4); w_u32(packet + 8, value);
                value = r_u32(0x80033D50 + second * 4); w_u32(packet + 16, value);
                value = r_u32(0x80033D50 + third * 4); w_u32(packet + 24, value);
                value = r_u32(0x80033D50 + fourth * 4); w_u32(packet + 32, value);
                value = r_u8(0x80059660 + first);
                value = game_small_attached_sar(value * previous, 8) << 8;
                w_u32(packet + 4, value);
                value = r_u8(0x80059660 + second);
                value = game_small_attached_sar(value * previous, 8) << 8;
                w_u32(packet + 12, value);
                value = r_u8(0x80059660 + third);
                value = game_small_attached_sar(value * current, 8) << 8;
                w_u32(packet + 20, value);
                value = r_u8(0x80059660 + fourth);
                value = game_small_attached_sar(value * current, 8) << 8;
                w_u32(packet + 28, value);
                SetPolyG4((POLY_G4 *)psx_addr(packet, 36));
                SetSemiTrans(psx_addr(packet, 36), 1);
                value = r_u32(0x8004608C);
                AddPrim(psx_addr(value + 0x455C, 4), psx_addr(packet, 4));
            }
        }
        previous = current;
        envelope -= 25;
        if (row == game_small_attached_sar(initial, 1) - 1)
            current = 128;
        else
            current = game_small_attached_sar(envelope * initial, 8);
    }
}

void game_draw_launch_bay_edge_native(const VECTOR *normal, const SVECTOR *first, const SVECTOR *second, sint32 depth, sint32 progress)
{
    uint32 factor, value, packet;
    SVECTOR offset_first, offset_second;
    sint32 screen[4], projection, flags;
    FUNCTION_MARKER(0x800D311C, "GAME.OVL");
    factor = (uint32)((sint32)((uint32)progress * 175) / 10);
    value = game_small_attached_sar((uint32)normal->vx * factor, 12);
    offset_first.vx = (sint16)((uint16)first->vx + value);
    value = game_small_attached_sar((uint32)normal->vy * factor, 12);
    offset_first.vy = (sint16)((uint16)first->vy + value);
    value = game_small_attached_sar((uint32)normal->vz * factor, 12);
    offset_first.vz = (sint16)((uint16)first->vz + value);
    value = game_small_attached_sar((uint32)normal->vx * factor, 12);
    offset_second.vx = (sint16)((uint16)second->vx + value);
    value = game_small_attached_sar((uint32)normal->vy * factor, 12);
    offset_second.vy = (sint16)((uint16)second->vy + value);
    packet = r_u32(0x80040690);
    w_u32(0x80040690, packet + 36);
    value = game_small_attached_sar((uint32)normal->vz * factor, 12);
    offset_second.vz = (sint16)((uint16)second->vz + value);
    RotTransPers((SVECTOR *)first, &screen[0], &projection, &flags);
    RotTransPers((SVECTOR *)second, &screen[1], &projection, &flags);
    RotTransPers(&offset_first, &screen[2], &projection, &flags);
    RotTransPers(&offset_second, &screen[3], &projection, &flags);
    w_u32(packet + 4, 0x808080);
    w_u32(packet + 12, 0x808080);
    w_u32(packet + 20, 0);
    w_u32(packet + 28, 0);
    w_u32(packet + 8, (uint32)screen[0]);
    w_u32(packet + 16, (uint32)screen[1]);
    w_u32(packet + 24, (uint32)screen[2]);
    w_u32(packet + 32, (uint32)screen[3]);
    SetPolyG4((POLY_G4 *)psx_addr(packet, 36));
    SetSemiTrans(psx_addr(packet, 36), 1);
    render_insert_primitive((sint16)depth, packet);
}

void game_draw_carrier_launch_bay(uint32 carrier, uint32 bay)
{
    VECTOR normal, relative, translation;
    MATRIX shifted, combined;
    SVECTOR points[4];
    uint32 texture_mode, texture_u, texture_v, texture_width, texture_height, texture_shift;
    uint32 camera, first, second, x, y, z, scale, total_shift, extra_shift, ax, ay, az;
    uint32 centers[2][3], corners[4][3], middles[2][3], coordinates[4][3];
    uint32 pair, axis, point, base, midpoint, value, factor, numerator, denominator, packet, depth_sum;
    sint32 screen[4], projection, flags[4], depths[4], progress, target, edge_depth;
    FUNCTION_MARKER(0x800D25C8, "GAME.OVL");
    ApplyMatrixLV((MATRIX *)psx_addr(carrier + 0x11C, 20), (VECTOR *)psx_addr(bay + 0x54, 12), &normal);
    texture_mode = (uint32)r_s16(0x800504EC);
    texture_u = r_u16(0x800504E0);
    camera = r_u32(0x80040C94);
    texture_v = r_u8(0x800504E2);
    texture_shift = (2 - texture_mode) & 31;
    first = r_u32(carrier + 8); second = r_u32(camera + 8);
    texture_u = (uint16)((texture_u & 63) << texture_shift);
    texture_width = (uint32)r_s16(0x800504E8);
    texture_height = r_u16(0x800504EA);
    x = first - second;
    first = r_u32(carrier + 12); second = r_u32(camera + 12);
    texture_width = (uint16)((texture_width << texture_shift) - 1);
    y = first - second;
    first = r_u32(carrier + 16); second = r_u32(camera + 16);
    z = first - second;
    scale = r_u32(carrier + 0x84);
    if ((sint32)scale > 0) {
        total_shift = scale;
        x = game_small_attached_sar(x, scale);
        scale = r_u32(carrier + 0x84);
        y = game_small_attached_sar(y, scale);
        scale = r_u32(carrier + 0x84);
        z = game_small_attached_sar(z, scale);
        extra_shift = 0;
    } else {
        total_shift = 0;
        extra_shift = (sint32)scale < 0 ? 0u - scale : scale;
    }
    for (;;) {
        ax = (sint32)x < 0 ? 0u - x : x;
        ay = (sint32)y < 0 ? 0u - y : y;
        az = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)ax < 0x3FFD && (sint32)ay < 0x3FFD && (sint32)az < 0x3FFD)
            break;
        ++extra_shift;
        ++total_shift;
        x = game_small_attached_sar(x, 1);
        y = game_small_attached_sar(y, 1);
        z = game_small_attached_sar(z, 1);
    }
    relative.vx = (sint32)x; relative.vy = (sint32)y; relative.vz = (sint32)z;
    ApplyMatrixLV((MATRIX *)psx_addr(0x80035060, 20), &relative, &translation);
    matrix_shift_native((MATRIX *)psx_addr(carrier + 0x11C, 20), &shifted, extra_shift);
    MulMatrix0((MATRIX *)psx_addr(0x80035060, 20), &shifted, &combined);
    SetRotMatrix(&combined);
    w_u32(0x80034D6C, (uint32)translation.vx);
    w_u32(0x80034D70, (uint32)translation.vy);
    w_u32(0x80034D74, (uint32)translation.vz);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    progress = r_s16(bay + 0x76);
    target = r_s16(bay + 0x7A);
    if (progress != target) {
        value = r_u16(bay + 0x78);
        target = r_s16(bay + 0x7A);
        value += (uint32)progress;
        w_u16(bay + 0x76, value);
        if ((sint16)value == target) {
            value = r_u16(bay + 0x74);
            w_u16(bay + 0x74, value + 1);
        }
    }
    for (pair = 0; pair < 2; ++pair) {
        base = bay + 4 + pair * 16;
        for (axis = 0; axis < 3; ++axis) {
            first = (uint32)r_s16(base + axis * 2);
            second = (uint32)r_s16(base + 8 + axis * 2);
            centers[pair][axis] = game_small_attached_sar(first + second, 1);
        }
        for (point = 0; point < 2; ++point) {
            for (axis = 0; axis < 3; ++axis) {
                value = (uint32)r_s16(base + point * 8 + axis * 2);
                factor = (uint32)r_s16(carrier + 0x1C6);
                midpoint = centers[pair][axis];
                value = game_small_attached_sar((midpoint - value) * factor, 7);
                corners[pair * 2 + point][axis] = midpoint - value;
            }
        }
    }
    for (axis = 0; axis < 3; ++axis) {
        middles[0][axis] = game_small_attached_sar(corners[0][axis] + corners[2][axis], 1);
        middles[1][axis] = game_small_attached_sar(corners[1][axis] + corners[3][axis], 1);
    }
    for (point = 0; point < 4; ++point) {
        for (axis = 0; axis < 3; ++axis) {
            midpoint = middles[point & 1][axis];
            progress = r_s16(bay + 0x76);
            value = (uint32)(sint32)(sint16)midpoint - corners[point][axis];
            numerator = value * (uint32)progress;
            denominator = (uint32)r_s16(carrier + 0x1C8);
            value = (uint32)game_small_attached_divide(numerator, denominator);
            coordinates[point][axis] = midpoint - value;
        }
    }
    for (point = 0; point < 4; ++point) {
        points[point].vx = (sint16)coordinates[point][0];
        points[point].vy = (sint16)coordinates[point][1];
        points[point].vz = (sint16)coordinates[point][2];
    }
    depths[0] = RotTransPers(&points[0], &screen[0], &projection, &flags[0]);
    depth_sum = (uint32)depths[0] << (total_shift & 31);
    depths[1] = RotTransPers(&points[1], &screen[1], &projection, &flags[1]);
    depth_sum += (uint32)depths[1] << (total_shift & 31);
    depths[2] = RotTransPers(&points[2], &screen[2], &projection, &flags[2]);
    depth_sum += (uint32)depths[2] << (total_shift & 31);
    depths[3] = RotTransPers(&points[3], &screen[3], &projection, &flags[3]);
    depth_sum += (uint32)depths[3] << (total_shift & 31);
    if (((uint32)flags[0] & 0xFFFF0000) != 0 || (sint32)depth_sum < 101)
        return;
    if ((sint32)depth_sum >= 4001)
        depth_sum -= 4000;
    packet = r_u32(0x80040690);
    w_u32(0x80040690, packet + 40);
    w_u8(packet + 12, (uint8)texture_u);
    value = texture_width + (uint8)texture_u;
    w_u8(packet + 20, value);
    w_u8(packet + 28, (uint8)texture_u);
    w_u8(packet + 36, value);
    w_u8(packet + 13, (uint8)texture_v);
    w_u8(packet + 21, (uint8)texture_v);
    value = (uint16)texture_v + texture_height;
    w_u8(packet + 29, value); w_u8(packet + 37, value);
    w_u32(packet + 8, (uint32)screen[0]); w_u32(packet + 16, (uint32)screen[1]);
    w_u32(packet + 24, (uint32)screen[2]); w_u32(packet + 32, (uint32)screen[3]);
    first = (uint32)r_s16(0x800504E0); second = (uint32)r_s16(0x800504E2);
    value = GetTPage((sint32)texture_mode, 1, (sint32)first, (sint32)second);
    w_u16(packet + 22, value);
    first = (uint32)r_s16(0x800504E4); second = (uint32)r_s16(0x800504E6);
    value = GetClut((sint32)first, (sint32)second);
    w_u16(packet + 14, value);
    w_u32(packet + 4, 0x808080);
    SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
    render_insert_primitive((sint32)depth_sum, packet);
    edge_depth = (sint16)depth_sum;
    progress = r_s16(bay + 0x76);
    game_draw_launch_bay_edge_native(&normal, &points[0], &points[1], edge_depth, progress);
    progress = r_s16(bay + 0x76);
    game_draw_launch_bay_edge_native(&normal, &points[1], &points[3], edge_depth, progress);
    progress = r_s16(bay + 0x76);
    game_draw_launch_bay_edge_native(&normal, &points[3], &points[2], edge_depth, progress);
    progress = r_s16(bay + 0x76);
    game_draw_launch_bay_edge_native(&normal, &points[2], &points[0], edge_depth, progress);
}

sint32 game_normalize_vector_exact_in_place(uint32 vector)
{
    uint32 x, y, z, squared, length;
    sint32 normalized_x, normalized_y, normalized_z;
    FUNCTION_MARKER(0x800C694C, "GAME.OVL");
    x = r_u32(vector);
    squared = x * x;
    y = r_u32(vector + 4);
    squared += y * y;
    z = r_u32(vector + 8);
    squared += z * z;
    length = SquareRoot0((sint32)squared);
    x = r_u32(vector);
    normalized_x = game_small_attached_divide(x << 12, length);
    y = r_u32(vector + 4);
    normalized_y = game_small_attached_divide(y << 12, length);
    z = r_u32(vector + 8);
    normalized_z = game_small_attached_divide(z << 12, length);
    w_u32(vector, (uint32)normalized_x);
    w_u32(vector + 4, (uint32)normalized_y);
    w_u32(vector + 8, (uint32)normalized_z);
    return (sint32)length;
}

sint32 game_build_effect_palette_variants(void)
{
    uint16 colors[16];
    PSX_RECT rectangle;
    uint32 row, index, destination, strength, red, green, blue, value;
    FUNCTION_MARKER(0x800C6A60, "GAME.OVL");
    for (row = 0; row < 16; ++row) {
        index = 16;
        do {
            --index;
            colors[index] = 0;
        } while (index != 0);
        strength = (15 - row) << 4;
        for (index = 0; index < 16; ++index) {
            red = r_u8(0x800EF518 + index) * strength;
            green = r_u8(0x800EF528 + index) * strength;
            blue = r_u8(0x800EF538 + index) * strength;
            value = (red >> 11) + ((green >> 11) << 5) + ((blue >> 11) << 10);
            destination = index + row - 8;
            if (destination - 2 < 13)
                colors[destination] = (uint16)(value | 0x8000);
        }
        value = r_u32(0x80108928 + row * 4);
        rectangle.x = (sint16)value;
        value = r_u32(0x80108988 + row * 4);
        rectangle.w = 16;
        rectangle.h = 1;
        rectangle.y = (sint16)value;
        LoadImagePSX(&rectangle, (uint32 *)colors);
        DrawSync(0);
    }
    return 0;
}

static uint32 game_annulus_checked_add(uint32 first, uint32 second, uint32 pc)
{
    sint64 result = (sint64)(sint32)first + (sint32)second;
    if (result > 2147483647 || result < (-2147483647 - 1))
        xport_mips_overflow_exception(pc);
    return first + second;
}

static uint32 game_annulus_checked_subtract(uint32 first, uint32 second, uint32 pc)
{
    sint64 result = (sint64)(sint32)first - (sint32)second;
    if (result > 2147483647 || result < (-2147483647 - 1))
        xport_mips_overflow_exception(pc);
    return first - second;
}

static uint32 game_annulus_project(uint32 radius, uint32 sine, uint32 cosine, sint32 *screen)
{
    SVECTOR point;
    sint32 projection, flags;
    point.vx = (sint16)game_small_attached_sar(radius * sine, 12);
    point.vy = (sint16)game_small_attached_sar(radius * cosine, 12);
    point.vz = 0;
    RotTransPers(&point, screen, &projection, &flags);
    return xport_gte_read_data(19);
}

static void game_draw_annulus_body(uint32 descriptor, const uint32 *native_fields)
{
    uint32 first_radius, second_radius, first_color, second_color;
    uint32 center_depth, first_depth, value, ordering, alternate, selected;
    uint32 angle, sine, cosine, packet;
    sint32 screen[4], unused_screen;
    first_radius = native_fields != NULL ? native_fields[0] : r_u32(descriptor + 12);
    second_radius = native_fields != NULL ? native_fields[1] : r_u32(descriptor + 16);
    center_depth = game_annulus_project(0, 0, 0, &unused_screen);
    center_depth <<= r_u32(0x80053670) & 31;
    ordering = r_u32(0x800421AC) + 0x4508;
    alternate = game_annulus_checked_add(ordering, -12, 0x800CED68);
    for (angle = 4096; angle > 0; angle -= 128) {
        sine = (uint32)r_s16(0x8003DC68 + (angle & 0xFFF) * 2);
        cosine = (uint32)r_s16(0x8003DC68 + ((angle + 1024) & 0xFFF) * 2);
        first_depth = game_annulus_project(second_radius, sine, cosine, &screen[0]);
        first_depth <<= r_u32(0x80053670) & 31;
        value = game_annulus_project(first_radius, sine, cosine, &screen[1]);
        value <<= r_u32(0x80053670) & 31;
        sine = (uint32)r_s16(0x8003DC68 + ((angle + 128) & 0xFFF) * 2);
        cosine = (uint32)r_s16(0x8003DC68 + ((angle + 1152) & 0xFFF) * 2);
        value = game_annulus_project(second_radius, sine, cosine, &screen[2]);
        value <<= r_u32(0x80053670) & 31;
        value = game_annulus_project(first_radius, sine, cosine, &screen[3]);
        value <<= r_u32(0x80053670) & 31;
        first_color = native_fields != NULL ? native_fields[2] : r_u32(descriptor + 36);
        second_color = native_fields != NULL ? native_fields[3] : r_u32(descriptor + 44);
        value = game_annulus_checked_subtract(first_depth, center_depth, 0x800CEF74);
        selected = (sint32)value >= 0 ? ordering : alternate;
        packet = r_u32(0x80040690);
        w_u32(packet + 4, second_color);
        w_u32(packet + 12, first_color);
        w_u32(packet + 20, second_color);
        w_u32(packet + 28, first_color);
        w_u8(packet + 7, 0x3A);
        w_u8(packet + 3, 8);
        w_u32(packet + 8, (uint32)screen[0]);
        w_u32(packet + 16, (uint32)screen[1]);
        w_u32(packet + 24, (uint32)screen[2]);
        w_u32(packet + 32, (uint32)screen[3]);
        AddPrim(psx_addr(selected, 4), psx_addr(packet, 4));
        packet = r_u32(0x80040690);
        w_u32(0x80040690, game_annulus_checked_add(packet, 36, 0x800CF010));
    }
}

void game_draw_annulus(uint32 descriptor)
{
    FUNCTION_MARKER(0x800CECCC, "GAME.OVL");
    game_draw_annulus_body(descriptor, NULL);
}

void game_draw_annulus_native(uint32 first_radius, uint32 second_radius, uint32 first_color, uint32 second_color)
{
    uint32 fields[4];
    fields[0] = first_radius; fields[1] = second_radius;
    fields[2] = first_color; fields[3] = second_color;
    game_draw_annulus_body(0, fields);
}

void game_draw_camera_annulus(uint32 first_radius, uint32 second_radius, uint32 first_color, uint32 second_color)
{
    FUNCTION_MARKER(0x800C68B4, "GAME.OVL");
    SetRotMatrix((MATRIX *)psx_addr(0x800420F0, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    game_draw_annulus_native(first_radius, second_radius, first_color, second_color);
}

static void game_effect_quad_mesh_project(void)
{
    uint32 index, mac0;
    sint32 screen, depth, flags;
    for (index = 0; index < 160; ++index) {
        depth = gte_project_full_depth((const SVECTOR *)psx_addr(0x80118EC8 + index * 8, 8), &screen, &flags);
        w_u32(0x1F800000 + index * 4, (uint32)screen);
        mac0 = xport_gte_read_data(24);
        w_u16(0x8003FE80 + index * 2, (uint32)depth);
        w_u16(0x800510F8 + index * 2, mac0);
        w_u16(0x80059660 + index * 2, (uint32)flags >> 12);
    }
}

sint32 game_draw_effect_quad_mesh(uint32 variant)
{
    uint32 packet, index, source, first_pointer, pointer, value, depth_offset, ordering, origin_depth;
    sint32 screen, projection, flags, depth;
    SVECTOR origin;
    FUNCTION_MARKER(0x800E2BD4, "GAME.OVL");
    origin.vx = 0; origin.vy = 0; origin.vz = 0;
    SetRotMatrix((MATRIX *)psx_addr(0x800420F0, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    depth = RotTransPers(&origin, &screen, &projection, &flags);
    w_u32(0x1F800000, (uint32)screen);
    origin_depth = (uint32)depth << 2;
    game_effect_quad_mesh_project();
    value = r_u32(0x800311B8) & 1;
    source = 0x801193C8 + value * 20 + (variant << 2);
    packet = r_u32(source);
    for (index = 0; index < 128; ++index) {
        source = 0x80118268 + index * 16;
        first_pointer = r_u32(source);
        value = r_u32(first_pointer);
        w_u32(packet + 8, value);
        pointer = r_u32(source + 4);
        value = r_u32(pointer);
        w_u32(packet + 16, value);
        pointer = r_u32(source + 8);
        value = r_u32(pointer);
        w_u32(packet + 24, value);
        pointer = r_u32(source + 12);
        value = r_u32(pointer);
        w_u32(packet + 32, value);
        depth_offset = (uint32)((sint32)(first_pointer << 14) >> 16) << 1;
        value = (uint32)r_s16(0x8003FE80 + depth_offset);
        ordering = r_u32(0x8004608C);
        ordering += (sint32)value < (sint32)origin_depth ? 0x456C : 0x45A4;
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        packet += 36;
    }
    return 0;
}

void game_draw_visible_quad_mesh_effect(uint32 object)
{
    uint32 status, value;
    MATRIX rotation;
    FUNCTION_MARKER(0x800C682C, "GAME.OVL");
    status = object_draw_visible(object);
    if (r_s32(object + 0x80) < 101 || status != 0)
        return;
    if (r_u32(object + 0xB0) == 0)
        return;
    matrix_rotate_x(&rotation, (MATRIX *)psx_addr(0x800420F0, 20), -1024);
    matrix_copy_rotation_native(0x800420F0, &rotation);
    value = r_u32(object + 0xBC);
    game_draw_effect_quad_mesh(value);
}

sint32 game_emit_long_lived_particle(uint32 position, uint32 velocity, uint32 color, uint32 size)
{
    uint32 seed, numerator, denominator, quotient, remainder, index, record, value, next;
    sint32 next_index;
    FUNCTION_MARKER(0x800C71B0, "GAME.OVL");
    seed = r_u32(0x800EF50C);
    denominator = game_small_attached_sar(size, 1);
    seed = game_small_attached_sar(seed, 1) ^ (seed << 3);
    numerator = seed & 0x7FFF;
    quotient = (uint32)game_small_attached_divide(numerator, denominator);
    remainder = numerator - quotient * denominator;
    index = r_u32(0x800EF4F4);
    record = 0x800FB5C8 + index * 56;
    w_u32(record + 16, 200);
    w_u32(record + 28, color & 255);
    w_u32(record + 32, (color >> 8) & 255);
    w_u32(record + 36, (color >> 16) & 255);
    w_u32(record + 24, size + remainder);
    value = r_u32(position); w_u32(record, value);
    value = r_u32(position + 4); w_u32(record + 4, value);
    value = r_u32(position + 8); w_u32(record + 8, value);
    value = r_u32(velocity);
    next = index + 1;
    w_u32(0x800EF50C, seed);
    w_u32(record + 40, value);
    value = r_u32(velocity + 4); w_u32(record + 44, value);
    value = r_u32(velocity + 8);
    w_u32(record + 20, 4);
    w_u32(record + 48, value);
    next_index = (sint32)next % 128;
    w_u32(0x800EF4F4, (uint32)next_index);
    return next_index;
}

sint32 game_expire_attached_effects(uint32 owner)
{
    uint32 index, record;
    FUNCTION_MARKER(0x800C79BC, "GAME.OVL");
    for (index = 0; index < 16; ++index) {
        record = 0x80118090 + index * 28;
        if (r_s32(record + 20) > 0 && r_u32(record) == owner)
            w_u32(record + 20, 0);
    }
    return 0;
}

void game_draw_attached_triangle(uint32 object, uint32 record)
{
    uint32 value, magnitude, width, length, scale, offset, camera, first, second;
    uint32 relative[6], count, index, absolute, fits, cross[3], product, depth, packet, color;
    uint32 player, kind, health, threshold;
    VECTOR start, end;
    SVECTOR start_point, end_point, side_point;
    sint32 screen[4], projection, flags, initial_depth, end_depth;
    FUNCTION_MARKER(0x800C55A4, "GAME.OVL");
    width = (uint32)(r_s32(record + 8) / 2);
    magnitude = r_u32(record + 16);
    if ((sint32)magnitude < 0)
        magnitude = 0u - magnitude;
    product = width * magnitude;
    value = r_u32(record + 12);
    width += (uint32)((sint32)product / 256);
    product = value * magnitude;
    length = (uint32)((sint32)product / 256);
    value = (uint32)random_between(896, 1024);
    length = (uint32)((sint32)(length * value) / 1024);
    scale = r_u32(object + 0x84);
    if ((sint32)scale > 0) {
        value = (uint32)r_s16(record);
        start.vx = (sint32)(value << (scale & 31));
        value = (uint32)r_s16(record + 2); scale = r_u32(object + 0x84);
        start.vy = (sint32)(value << (scale & 31));
        offset = r_u32(record + 0x1C);
        value = (uint32)r_s16(record + 4); scale = r_u32(object + 0x84);
        value <<= scale & 31;
        value -= offset == 0 ? game_small_attached_sar(width, 2) : offset;
        start.vz = (sint32)value;
    } else {
        value = (uint32)r_s16(record);
        start.vx = (sint32)game_small_attached_sar(value, 0u - scale);
        scale = r_u32(object + 0x84); value = (uint32)r_s16(record + 2);
        start.vy = (sint32)game_small_attached_sar(value, 0u - scale);
        offset = r_u32(record + 0x1C);
        scale = r_u32(object + 0x84); value = (uint32)r_s16(record + 4);
        value = game_small_attached_sar(value, 0u - scale);
        value -= offset == 0 ? game_small_attached_sar(width, 2) : offset;
        start.vz = (sint32)value;
    }
    end.vx = start.vx; end.vy = start.vy;
    end.vz = (sint32)((uint32)start.vz - length);
    ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), &start, &start);
    camera = r_u32(0x80040C94);
    first = r_u32(object + 8); second = r_u32(camera + 8);
    start.vx = (sint32)((uint32)start.vx + first - second);
    first = r_u32(object + 12); second = r_u32(camera + 12);
    start.vy = (sint32)((uint32)start.vy + first - second);
    first = r_u32(object + 16); second = r_u32(camera + 16);
    start.vz = (sint32)((uint32)start.vz + first - second);
    ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), &end, &end);
    camera = r_u32(0x80040C94);
    first = r_u32(object + 8); second = r_u32(camera + 8);
    end.vx = (sint32)((uint32)end.vx + first - second);
    first = r_u32(object + 12); second = r_u32(camera + 12);
    end.vy = (sint32)((uint32)end.vy + first - second);
    first = r_u32(object + 16); second = r_u32(camera + 16);
    end.vz = (sint32)((uint32)end.vz + first - second);
    relative[0] = (uint32)start.vx; relative[1] = (uint32)start.vy; relative[2] = (uint32)start.vz;
    relative[3] = (uint32)end.vx; relative[4] = (uint32)end.vy; relative[5] = (uint32)end.vz;
    count = 0;
    for (;;) {
        fits = 1;
        for (index = 0; index < 6; ++index) {
            value = relative[index];
            absolute = (sint32)value < 0 ? 0u - value : value;
            if ((sint32)absolute >= 0x4001) {
                fits = 0;
                break;
            }
        }
        if (fits != 0)
            break;
        ++count;
        for (index = 0; index < 6; ++index)
            relative[index] = game_small_attached_sar(relative[index], 1);
    }
    start_point.vx = (sint16)relative[0]; start_point.vy = (sint16)relative[1]; start_point.vz = (sint16)relative[2];
    end_point.vx = (sint16)relative[3]; end_point.vy = (sint16)relative[4]; end_point.vz = (sint16)relative[5];
    matrix_copy_rotation(0x80118058, 0x80034D58);
    matrix_shift(0x80118058, 0x80124C28, count);
    w_u32(0x80034D6C, 0); w_u32(0x80034D70, 0); w_u32(0x80034D74, 0);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    SetRotMatrix((MATRIX *)psx_addr(0x80124C28, 20));
    initial_depth = RotTransPers(&start_point, &screen[0], &projection, &flags);
    depth = (uint32)initial_depth << ((count * 2) & 31);
    end_depth = RotTransPers(&end_point, &screen[1], &projection, &flags);
    player = r_u32(0x80040FDC);
    first = (uint32)r_s16(object + 0x126); second = (uint32)r_s16(player + 0x12C);
    product = first * second;
    first = (uint32)r_s16(object + 0x12C); second = (uint32)r_s16(player + 0x12A);
    cross[0] = game_small_attached_sar(product - first * second, 12);
    first = (uint32)r_s16(object + 0x12C); second = (uint32)r_s16(player + 0x128);
    product = first * second;
    first = (uint32)r_s16(object + 0x120); second = (uint32)r_s16(player + 0x12C);
    cross[1] = game_small_attached_sar(product - first * second, 12);
    first = (uint32)r_s16(object + 0x120); second = (uint32)r_s16(player + 0x12A);
    product = first * second;
    first = (uint32)r_s16(object + 0x126); second = (uint32)r_s16(player + 0x128);
    cross[2] = game_small_attached_sar(product - first * second, 12);
    side_point.vx = (sint16)((uint16)start_point.vx + game_small_attached_sar(cross[0] * width, count + 13));
    side_point.vy = (sint16)((uint16)start_point.vy + game_small_attached_sar(cross[1] * width, count + 13));
    side_point.vz = (sint16)((uint16)start_point.vz + game_small_attached_sar(cross[2] * width, count + 13));
    RotTransPers(&side_point, &screen[2], &projection, &flags);
    side_point.vx = (sint16)((uint16)start_point.vx - game_small_attached_sar(cross[0] * width, count + 13));
    side_point.vy = (sint16)((uint16)start_point.vy - game_small_attached_sar(cross[1] * width, count + 13));
    side_point.vz = (sint16)((uint16)start_point.vz - game_small_attached_sar(cross[2] * width, count + 13));
    RotTransPers(&side_point, &screen[3], &projection, &flags);
    if (depth == 0 && end_depth == 0)
        return;
    kind = r_u32(object + 4);
    health = r_u32(object + 0xEC);
    threshold = r_u32(0x8005369C + kind * 60);
    color = r_u32(record + 24);
    if ((sint32)threshold < (sint32)health && (r_u32(object + 0x114) & 0x10) != 0)
        color = 0xFF;
    packet = r_u32(0x80040690);
    w_u32(0x80040690, packet + 112);
    w_u32(packet + 8, (uint32)screen[2]);
    w_u32(packet + 16, (uint32)screen[3]);
    w_u8(packet + 3, 6);
    w_u32(packet + 4, color);
    w_u32(packet + 12, 0); w_u32(packet + 20, 0);
    w_u8(packet + 7, 0x32);
    w_u32(packet + 24, (uint32)screen[1]);
    render_insert_primitive((sint32)(depth << 2), packet);
}

static void game_engine_effect_position(uint32 object, uint32 vertices, uint32 record_field, uint32 anchor_field, uint32 config_offset, uint32 config, uint32 first_vertex)
{
    uint32 axis, second_vertex, record, first, second, value;
    for (axis = 0; axis < 3; ++axis) {
        if (axis != 0) {
            config = r_u32(0x800ECBFC + config_offset);
            first_vertex = r_u32(config + anchor_field);
        }
        second_vertex = r_u32(config + anchor_field + 4);
        first = (uint32)r_s16(vertices + first_vertex * 8 + axis * 2);
        second = (uint32)r_s16(vertices + second_vertex * 8 + axis * 2);
        record = r_u32(object + record_field);
        value = (uint32)((sint32)(first + second) / 2);
        w_u16(record + axis * 2, value);
    }
    config = r_u32(0x800ECBFC + config_offset);
    record = r_u32(object + record_field);
    value = r_u32(config + anchor_field + 8);
    w_u32(record + 8, value);
    config = r_u32(0x800ECBFC + config_offset);
    record = r_u32(object + record_field);
    value = r_u32(config + anchor_field + 12);
    w_u32(record + 12, value);
}

static void game_engine_effect_blend(uint32 object, uint32 record_field, uint32 ratio)
{
    uint32 record, current, candidate, reverse, config, value, target, product;
    reverse = (sint32)ratio < 0;
    if (ratio == 0) {
        record = r_u32(object + record_field);
        reverse = r_s32(record + 16) < 0;
    }
    if (reverse != 0) {
        record = r_u32(object + record_field);
        current = r_u32(record + 16);
        if ((sint32)ratio < (sint32)current)
            value = ratio;
        else {
            candidate = current + 16;
            value = (sint32)candidate < (sint32)ratio ? current + 8 : ratio;
        }
        w_u32(record + 16, value);
        return;
    }
    value = r_u32(0x800ECBD0);
    config = r_u32(0x800ECBFC + value * 44);
    value = r_u32(config + 16);
    target = ratio;
    if (value != 0xFFFFFFFF) {
        record = r_u32(object + record_field);
        value = (uint32)r_s16(record);
        product = r_u32(0x800F2B10) * value;
        if ((sint32)product > 0)
            target = (uint32)((sint32)(ratio * 3) / 4);
    }
    record = r_u32(object + record_field);
    current = r_u32(record + 16);
    if ((sint32)target >= (sint32)current)
        value = target;
    else
        value = (sint32)current < 17 ? 16 : current - 8;
    w_u32(record + 16, value);
}

void game_draw_engine_effects(uint32 object)
{
    uint32 value, kind, denominator, ratio, mesh, vertices, magnitude, record, factor, color;
    uint32 slot, record_field, anchor_field, config_index, config_offset, config, first_vertex, mode;
    FUNCTION_MARKER(0x800C5E0C, "GAME.OVL");
    value = r_u32(object + 0xEC);
    kind = r_u32(object + 4);
    denominator = r_u32(0x8005369C + kind * 60) + 1;
    ratio = (uint32)game_small_attached_divide(value * 255, denominator);
    mesh = r_u32(object + 0x64);
    vertices = r_u32(mesh);
    magnitude = (sint32)ratio < 0 ? 0u - ratio : ratio;
    if ((sint32)magnitude >= 256)
        ratio = (sint32)ratio < 0 ? (uint32)-255 : 255;
    for (slot = 0; slot < 3; ++slot) {
        record = r_u32(object + 0x1E8 + slot * 4);
        factor = r_u32(record + 16);
        if ((sint32)factor >= 256)
            w_u32(record + 16, 255);
        else if ((sint32)factor < -255)
            w_u32(record + 16, -255);
    }
    if ((sint32)ratio > 0)
        color = 0xFF8040;
    else if (ratio != 0)
        color = 0x2040FF;
    else {
        record = r_u32(object + 0x1E8);
        color = r_s32(record + 16) > 0 ? 0xFF8040 : 0x2040FF;
    }
    for (slot = 0; slot < 3; ++slot) {
        record_field = 0x1E8 + slot * 4;
        anchor_field = slot * 16;
        config_index = r_u32(0x800ECBD0);
        config_offset = config_index * 44;
        config = r_u32(0x800ECBFC + config_offset);
        first_vertex = r_u32(config + anchor_field);
        if (first_vertex == 0xFFFFFFFF)
            continue;
        game_engine_effect_position(object, vertices, record_field, anchor_field, config_offset, config, first_vertex);
        game_engine_effect_blend(object, record_field, ratio);
        record = r_u32(object + record_field);
        if (slot == 0) {
            config_index = r_u32(0x800ECBD0);
            w_u32(record + 28, 0);
            config = r_u32(0x800ECBFC + config_index * 44);
            value = r_u32(config);
            if (value == 0x51 || value == 0x25) {
                record = r_u32(object + record_field);
                w_u32(record + 28, value == 0x51 ? (uint32)-100 : 100);
            }
            record = r_u32(object + record_field);
            config_index = r_u32(0x800ECBD0);
            w_u32(record + 20, 0xFFFFFF);
            record = r_u32(object + record_field);
            w_u32(record + 24, color);
            config = r_u32(0x800ECBFC + config_index * 44);
            value = r_u32(config);
            mode = value == 0x2B ? 0 : 1;
        } else {
            w_u32(record + 28, 0);
            record = r_u32(object + record_field);
            w_u32(record + 20, 0xFFFFFF);
            record = r_u32(object + record_field);
            w_u32(record + 24, color);
            mode = 1;
        }
        record = r_u32(object + record_field);
        game_draw_attached_record(object, record, mode);
    }
}

static uint32 game_camera_effect_seed_step(uint32 seed)
{
    return game_small_attached_sar(seed, 1) ^ (seed << 3);
}

static void game_camera_effect_spark(const VECTOR *direction, const uint32 raw[3], uint32 factor)
{
    uint32 camera, x, y, z, dx, dy, dz;
    dx = (uint32)(sint32)(sint16)game_small_attached_sar(raw[0] * factor, 3);
    dy = (uint32)(sint32)(sint16)game_small_attached_sar(raw[1] * factor, 3);
    dz = (uint32)(sint32)(sint16)game_small_attached_sar(raw[2] * factor, 3);
    camera = r_u32(0x80040C94);
    x = r_u32(camera + 8); y = r_u32(camera + 12); z = r_u32(camera + 16);
    game_emit_radial_effect((sint32)((uint32)direction->vx + x + dx), (sint32)((uint32)direction->vy + y + dy), (sint32)((uint32)direction->vz + z + dz), 100, 0x202020);
}

static void game_camera_effect_ring_point(uint32 angle, uint32 radius, uint32 destination)
{
    SVECTOR point;
    uint32 value;
    sint32 screen, projection, flags;
    value = (uint32)rsin((sint32)angle);
    point.vy = 0;
    point.vx = (sint16)game_small_attached_sar(radius * value, 12);
    value = (uint32)rcos((sint32)angle);
    point.vz = (sint16)game_small_attached_sar(radius * value, 12);
    RotTransPers(&point, &screen, &projection, &flags);
    w_u32(destination, (uint32)screen);
}

void game_process_camera_vector(uint32 position, uint32 parameter)
{
    uint32 camera, x, y, z, first, second, ax, ay, az, value, seed, product, fade;
    uint32 raw[3], index, axis, radius, inner, outer, jitter, angle, brightness, color;
    uint32 ordering, next, band, packet, next_color, screen_base;
    sint64 reciprocal_product;
    VECTOR direction;
    SVECTOR angles;
    FUNCTION_MARKER(0x800C8C2C, "GAME.OVL");
    camera = r_u32(0x80040C94);
    first = r_u32(position); second = r_u32(camera + 8); x = first - second;
    first = r_u32(position + 4); second = r_u32(camera + 12); y = first - second;
    first = r_u32(position + 8); second = r_u32(camera + 16); z = first - second;
    for (;;) {
        ax = (sint32)x < 0 ? 0u - x : x;
        ay = (sint32)y < 0 ? 0u - y : y;
        az = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)ax < 0x4000 && (sint32)ay < 0x4000 && (sint32)az < 0x4000)
            break;
        x = game_small_attached_sar(x, 1); y = game_small_attached_sar(y, 1); z = game_small_attached_sar(z, 1);
    }
    direction.vx = (sint32)x; direction.vy = (sint32)y; direction.vz = (sint32)z;
    vector_normalize(&direction, &direction);
    if ((sint32)parameter < 40) {
        x = r_u32(0x800ECE9C); y = r_u32(0x800ECEA0); z = r_u32(0x800ECEA4);
        game_set_screen_tint(x + 40, y + 40, z + 40);
    }
    w_u32(0x80034D6C, 0); w_u32(0x80034D70, 0); w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    camera = r_u32(0x80040C94);
    x = r_u32(camera + 8); y = r_u32(camera + 12); z = r_u32(camera + 16);
    game_emit_radial_effect((sint32)((uint32)direction.vx + x), (sint32)((uint32)direction.vy + y), (sint32)((uint32)direction.vz + z), 600, 0x808080);
    camera = r_u32(0x80040C94);
    x = r_u32(camera + 8); y = r_u32(camera + 12); z = r_u32(camera + 16);
    game_emit_radial_effect((sint32)((uint32)direction.vx + x), (sint32)((uint32)direction.vy + y), (sint32)((uint32)direction.vz + z), 800, 0x403030);
    value = (20000 - (parameter << 5)) * 255;
    reciprocal_product = (sint64)(sint32)value * 0x68DB8BAD;
    fade = (uint32)((sint32)(reciprocal_product >> 32) >> 13) - game_small_attached_sar(value, 31);
    seed = 0x12345678;
    for (index = 0; index < 80; ++index) {
        for (axis = 0; axis < 3; ++axis) {
            seed = game_camera_effect_seed_step(seed);
            raw[axis] = (uint32)(sint32)(sint16)((seed & 0x1FF) - 255);
        }
        game_camera_effect_spark(&direction, raw, parameter);
        game_camera_effect_spark(&direction, raw, parameter - 8);
    }
    radius = parameter << 5;
    inner = radius - 4000;
    outer = radius + 256;
    direction.vx = (sint32)game_small_attached_sar((uint32)direction.vx * 20000, 12);
    direction.vy = (sint32)game_small_attached_sar((uint32)direction.vy * 20000, 12);
    direction.vz = (sint32)game_small_attached_sar((uint32)direction.vz * 20000, 12);
    if ((sint32)inner < 0)
        inner = 0;
    angles.vx = 333; angles.vy = 333; angles.vz = 333;
    RotMatrix(&angles, (MATRIX *)psx_addr(0x800F3398, 20));
    MulMatrix0((MATRIX *)psx_addr(0x80035060, 20), (MATRIX *)psx_addr(0x800F3398, 20), (MATRIX *)psx_addr(0x800F3398, 20));
    direction.vx = (sint32)game_small_attached_sar((uint32)direction.vx, 1);
    direction.vz = (sint32)game_small_attached_sar((uint32)direction.vz, 1);
    direction.vy = (sint32)game_small_attached_sar((uint32)direction.vy, 1);
    ApplyMatrixLV((MATRIX *)psx_addr(0x80035060, 20), &direction, &direction);
    w_u32(0x80034D6C, (uint32)direction.vx);
    w_u32(0x80034D70, (uint32)direction.vy);
    w_u32(0x80034D74, (uint32)direction.vz);
    SetRotMatrix((MATRIX *)psx_addr(0x800F3398, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    for (index = 0; index < 64; ++index) {
        seed = game_camera_effect_seed_step(seed);
        jitter = game_small_attached_sar((seed & 255) * radius, 12);
        angle = index << 6;
        game_camera_effect_ring_point(angle, radius + jitter, 0x80033D50 + index * 4);
        game_camera_effect_ring_point(angle, inner + jitter, 0x80033E50 + index * 4);
        game_camera_effect_ring_point(angle, outer + jitter, 0x80033F50 + index * 4);
        seed = game_camera_effect_seed_step(seed);
        product = ((seed & 31) + 224) * fade;
        brightness = game_small_attached_sar(product, 8);
        color = brightness + (brightness << 8) + (brightness << 16);
        w_u32(0x800510F8 + index * 4, color);
    }
    ordering = r_u32(0x8004608C) + 0x4570;
    for (index = 0; index < 64; ++index) {
        next = (index + 1) & 63;
        for (band = 0; band < 2; ++band) {
            packet = r_u32(0x80040690);
            color = r_u32(0x800510F8 + index * 4);
            w_u32(0x80040690, packet + 36);
            w_u32(packet + 4, color);
            next_color = r_u32(0x800510F8 + next * 4);
            w_u32(packet + 20, 0); w_u32(packet + 28, 0);
            w_u32(packet + 12, next_color);
            SetPolyG4((POLY_G4 *)psx_addr(packet, 36));
            value = r_u32(0x80033D50 + index * 4); w_u32(packet + 8, value);
            value = r_u32(0x80033D50 + next * 4); w_u32(packet + 16, value);
            screen_base = band == 0 ? 0x80033E50 : 0x80033F50;
            value = r_u32(screen_base + index * 4); w_u32(packet + 24, value);
            value = r_u32(screen_base + next * 4); w_u32(packet + 32, value);
            SetSemiTrans(psx_addr(packet, 36), 1);
            AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        }
    }
}

uint32 game_update_following_roll_maneuver(uint32 object)
{
    uint32 callback, value, target, speed, kind, limit, current, desired, first, second, x, y;
    FUNCTION_MARKER(0x800C9510, "GAME.OVL");
    callback = r_u32(object + 0x4C);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
    value = r_u32(0x80046174);
    target = r_u32(object + 0x178);
    speed = r_u32(target + 0xEC);
    if ((sint32)value < 12000)
        speed = (uint32)((sint32)((speed << 3) - speed) >> 3);
    w_u32(object + 0xE8, speed);
    kind = r_u32(object + 4);
    limit = r_u32(0x8005369C + kind * 60);
    value = r_u32(object + 0xE8);
    limit <<= 1;
    if ((sint32)value < (sint32)limit)
        w_u32(object + 0xE8, limit);
    if (r_s32(object + 0x9C) < 6) {
        value = r_u32(object + 0xAC);
        value = (uint32)game_reduce_without_sign_change((sint32)value, 10);
        w_u32(object + 0xAC, value);
    } else {
        current = r_u32(object + 0xAC);
        desired = r_u32(object + 0xA8);
        first = (sint32)current < 0 ? 0u - current : current;
        second = (sint32)desired < 0 ? 0u - desired : desired;
        if ((sint32)first < (sint32)second) {
            value = r_u32(object + 0xB0);
            w_u32(object + 0xAC, current + value);
        }
    }
    if (r_s32(0x80052E6C) < 176 && r_s32(0x80053674) < 176) {
        value = r_u32(object + 0xAC);
        w_u32(object + 0x28, value);
    }
    value = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, value);
    if ((sint32)value > 0) {
        x = r_u32(0x80033D1C); y = r_u32(0x80033D30);
        w_u16(object + 0x250, x); w_u16(object + 0x252, y);
    } else {
        value = r_u32(object + 0x50);
        w_u32(object + 0x40, value);
    }
    game_update_object_state(object);
    return game_update_object_movement(object);
}

sint32 game_begin_following_roll_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 update)
{
    uint32 kind, limit, current, value, step;
    FUNCTION_MARKER(0x800C96B8, "GAME.OVL");
    w_u32(object + 0x9C, ticks);
    w_u32(object + 0x50, completion);
    w_u32(object + 0x4C, update);
    w_u32(object + 0x40, 0x800C9510);
    game_update_object_state(object);
    kind = r_u32(object + 4);
    limit = r_u32(0x8005369C + kind * 60);
    current = r_u32(object + 0xEC);
    limit = (uint32)((sint32)limit >> 2);
    limit *= 3;
    if ((sint32)current < (sint32)limit)
        value = (uint32)random_signed_between(60, 80);
    else
        value = (uint32)random_signed_between(100, 130);
    w_u32(object + 0xA8, value);
    value = r_u32(object + 0xA8);
    w_u32(object + 0xAC, 0);
    step = (sint32)value < 0 ? (uint32)-10 : 10;
    w_u32(object + 0xB0, step);
    return (sint32)step;
}

uint32 game_update_spinning_maneuver(uint32 object)
{
    uint32 callback, value, step;
    FUNCTION_MARKER(0x800C9868, "GAME.OVL");
    callback = r_u32(object + 0x4C);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
    value = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, value);
    if ((sint32)value <= 0) {
        value = r_u32(object + 0x50);
        w_u32(object + 0x40, value);
    }
    value = r_u32(object + 0x28); step = r_u32(object + 0xBC);
    w_u32(object + 0x28, value + step);
    game_update_object_state(object);
    return game_update_object_movement(object);
}

void game_begin_spinning_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 yaw, uint32 pitch, uint32 update)
{
    uint32 value;
    FUNCTION_MARKER(0x800C98DC, "GAME.OVL");
    w_u32(object + 0x9C, ticks);
    w_u32(object + 0x50, completion);
    w_u32(object + 0x40, 0x800C9868);
    w_u16(object + 0x250, yaw);
    w_u16(object + 0x252, pitch);
    w_u32(object + 0x4C, update);
    value = (uint32)random_between(2, 6);
    w_u32(object + 0xBC, value);
    game_update_object_state(object);
}

uint32 game_update_four_phase_turn_maneuver(uint32 object)
{
    uint32 callback, state, kind, speed, remaining, value, counter, magnitude, duration;
    FUNCTION_MARKER(0x800C9940, "GAME.OVL");
    callback = r_u32(object + 0x4C);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
    state = r_u32(object + 0xB0);
    if (state == 1 || state == 3) {
        kind = r_u32(object + 4);
        speed = r_u32(0x8005369C + kind * 60);
        remaining = r_u32(object + 0xAC);
        w_u32(object + 0xE8, speed);
        if (remaining != 0) {
            w_u32(object + 0xAC, remaining - 1);
            value = r_u32(object + 0xA8);
            counter = r_u32(object + 0xAC);
            w_u32(object + 0x24, value);
            if (counter == 0) {
                value = r_u32(object + 0xA8);
                magnitude = (sint32)value < 0 ? 0u - value : value;
                duration = (uint32)game_small_attached_divide(2048, magnitude);
                value = r_u32(object + 0xB0);
                w_u32(object + 0x9C, 60);
                w_u32(object + 0xB0, value + 1);
                w_u32(object + 0xAC, duration);
            }
        }
    } else if (state == 2) {
        kind = r_u32(object + 4);
        remaining = r_u32(object + 0x9C);
        speed = r_u32(0x800536A0 + kind * 60);
        remaining -= 1;
        w_u32(object + 0x9C, remaining);
        w_u32(object + 0xE8, speed);
        if ((sint32)remaining <= 0) {
            value = r_u32(object + 0xB0);
            w_u32(object + 0x9C, 30);
            w_u32(object + 0xB0, value + 1);
        }
    } else if (state == 4) {
        kind = r_u32(object + 4);
        remaining = r_u32(object + 0x9C);
        speed = r_u32(0x8005369C + kind * 60);
        remaining -= 1;
        w_u32(object + 0x9C, remaining);
        w_u32(object + 0xE8, speed);
        if ((sint32)remaining <= 0) {
            value = r_u32(object + 0x50);
            w_u32(object + 0x40, value);
        }
    }
    game_update_object_state(object);
    return game_update_object_movement(object);
}

sint32 game_begin_four_phase_turn_maneuver(uint32 object, uint32 completion)
{
    uint32 value, magnitude, duration;
    FUNCTION_MARKER(0x800C9AEC, "GAME.OVL");
    w_u32(object + 0x50, completion);
    w_u32(object + 0x40, 0x800C9940);
    w_u16(object + 0x250, 0); w_u16(object + 0x252, 0); w_u16(object + 0x254, 0);
    value = (uint32)random_signed_between(45, 60);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    duration = (uint32)game_small_attached_divide(2048, magnitude);
    w_u32(object + 0xA8, value);
    w_u32(object + 0xB0, 1);
    w_u32(object + 0xAC, duration);
    return 1;
}

uint32 game_update_roll_transition_maneuver(uint32 object)
{
    uint32 callback, state, remaining, value, counter, kind, speed;
    FUNCTION_MARKER(0x800C9B84, "GAME.OVL");
    callback = r_u32(object + 0x4C);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
    state = r_u32(object + 0xB0);
    if (state == 1) {
        remaining = r_u32(object + 0xAC) - 1;
        w_u32(object + 0xAC, remaining);
        value = r_u32(object + 0xA8);
        counter = r_u32(object + 0xAC);
        w_u32(object + 0x28, value);
        if (counter == 0) {
            state = r_u32(object + 0xB0);
            w_u16(object + 0x252, 2048);
            w_u32(object + 0xB0, state + 1);
            value = (uint32)random_between(1, 2);
            w_u32(object + 0x9C, value * 30);
        }
    } else {
        kind = r_u32(object + 4);
        remaining = r_u32(object + 0x9C);
        speed = r_u32(0x800536A0 + kind * 60);
        remaining -= 1;
        w_u32(object + 0x9C, remaining);
        w_u32(object + 0xE8, speed);
        if ((sint32)remaining <= 0) {
            value = r_u32(object + 0x50);
            w_u32(object + 0x40, value);
        }
    }
    game_update_object_state(object);
    return game_update_object_movement(object);
}

sint32 game_begin_roll_transition_maneuver(uint32 object, uint32 completion)
{
    uint32 value, magnitude, duration, kind, speed;
    FUNCTION_MARKER(0x800C9C74, "GAME.OVL");
    w_u32(object + 0x50, completion);
    w_u32(object + 0x40, 0x800C9B84);
    w_u16(object + 0x250, 0); w_u16(object + 0x252, 0); w_u16(object + 0x254, 0);
    value = (uint32)random_signed_between(30, 45);
    w_u32(object + 0xA8, value);
    value = (uint32)random_between(1024, 1440);
    magnitude = r_u32(object + 0xA8);
    if ((sint32)magnitude < 0)
        magnitude = 0u - magnitude;
    duration = (uint32)game_small_attached_divide(value, magnitude);
    kind = r_u32(object + 4);
    w_u32(object + 0xB0, 1);
    w_u32(object + 0xAC, duration);
    speed = r_u32(0x8005369C + kind * 60);
    value = (uint32)((sint32)(speed * 3) >> 2);
    w_u32(object + 0xE8, value);
    return (sint32)value;
}

void game_start_ai_mode_maneuver(uint32 object, uint32 update, uint32 ticks)
{
    uint32 mode, value, flags, kind, speed, yaw, pitch, random, factor;
    FUNCTION_MARKER(0x800C9D50, "GAME.OVL");
    mode = r_u32(object + 0x88);
    switch (mode) {
    case 1:
        game_begin_timed_maneuver(object, ticks, 0x8009AB58, update);
        break;
    case 3:
        random = (uint32)random_signed_between(240, 640);
        value = r_u32(0x80033D1C);
        w_u32(0x80033D1C, value + random);
        random = (uint32)random_signed_between(240, 640);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        kind = r_u32(object + 4);
        pitch += random;
        flags = r_u32(object + 0x114);
        speed = r_u32(0x800536A0 + kind * 60);
        w_u32(0x80033D30, pitch);
        w_u32(object + 0x114, flags | 0x2000);
        w_u32(object + 0xE8, speed + 400);
        game_begin_yaw_maneuver(object, ticks, 0x8009AB58, yaw, pitch, update);
        break;
    case 4:
        yaw = r_u32(0x80033D1C); pitch = r_u32(0x80033D30);
        game_begin_yaw_maneuver(object, ticks, 0x8009AB58, yaw, pitch, update);
        break;
    case 5:
        flags = r_u32(object + 0x114);
        kind = r_u32(object + 4);
        w_u32(object + 0x114, flags | 0x2000);
        speed = r_u32(0x8005369C + kind * 60);
        kind = r_u32(object + 4);
        w_u32(object + 0xE8, speed);
        factor = r_u16(0x800536B4 + kind * 60);
        w_u16(object + 0x260, factor);
        game_begin_four_phase_turn_maneuver(object, 0x8009AB58);
        break;
    case 6:
        game_begin_following_roll_maneuver(object, ticks, 0x8009AB58, update);
        break;
    case 7:
        kind = r_u32(object + 4);
        yaw = r_u32(0x80033D1C);
        flags = r_u32(object + 0x114);
        speed = r_u32(0x800536A0 + kind * 60);
        w_u32(object + 0x114, flags | 0x2000);
        pitch = r_u32(0x80033D30);
        w_u32(object + 0xE8, speed + 50);
        game_begin_yaw_maneuver(object, ticks, 0x8009AB58, yaw, pitch, update);
        break;
    case 8:
        yaw = r_u32(0x80033D1C);
        flags = r_u32(object + 0x114);
        pitch = r_u32(0x80033D30);
        w_u32(object + 0x114, flags | 0x2000);
        game_begin_yaw_maneuver(object, ticks, 0x8009AB58, yaw, pitch, update);
        break;
    case 9:
        kind = r_u32(object + 4);
        speed = r_u32(0x8005369C + kind * 60);
        kind = r_u32(object + 4);
        w_u32(object + 0xE8, speed);
        factor = r_u16(0x800536B4 + kind * 60);
        factor = (uint32)((sint32)(sint16)factor >> 2) * 5;
        w_u16(object + 0x260, factor);
        game_begin_roll_transition_maneuver(object, 0x8009AB58);
        break;
    default:
        diagnostic_report(r_u32(object + 0x88), 0x80075F30, 0x80075F48);
        break;
    }
}

sint32 game_update_pitch_alignment_timer(uint32 object)
{
    uint32 value, magnitude, kind, duration;
    FUNCTION_MARKER(0x800CA060, "GAME.OVL");
    value = (uint32)r_s16(object + 0x252);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if ((sint32)magnitude < 400 && r_u32(object + 0xA8) != 0) {
        w_u32(object + 0xA8, 0);
        value = (uint32)random_between(1, 2);
        duration = value * 30;
        kind = r_u32(object + 4);
        w_u32(object + 0x9C, duration);
        value = r_u32(0x8005369C + kind * 60);
        w_u32(object + 0xE8, value);
    } else {
        value = 10;
        w_u32(object + 0x9C, value);
    }
    return (sint32)value;
}

void game_draw_projected_fan(sint32 x, sint32 y, sint32 radius, uint32 center_color, uint32 edge_color)
{
    uint32 angle, sine, cosine, product, first_x, first_y, next_x, next_y, packet, ordering;
    FUNCTION_MARKER(0x800CE7FC, "GAME.OVL");
    game_annulus_checked_add((uint32)(radius >> 1), (uint32)radius, 0x800CE848);
    for (angle = 0; angle < 4096; angle += 256) {
        sine = (uint32)r_s16(0x8003DC68 + (angle & 0xFFF) * 2);
        cosine = (uint32)r_s16(0x8003DC68 + ((angle + 1024) & 0xFFF) * 2);
        product = sine * (uint32)radius;
        first_x = game_annulus_checked_add(game_small_attached_sar(product, 12), (uint32)x, 0x800CE894);
        product = cosine * (uint32)radius;
        first_y = game_annulus_checked_add(game_small_attached_sar(product, 13), (uint32)y, 0x800CE8A4);
        sine = (uint32)r_s16(0x8003DC68 + ((angle + 256) & 0xFFF) * 2);
        cosine = (uint32)r_s16(0x8003DC68 + ((angle + 1280) & 0xFFF) * 2);
        product = sine * (uint32)radius;
        next_x = game_annulus_checked_add(game_small_attached_sar(product, 12), (uint32)x, 0x800CE8F4);
        product = cosine * (uint32)radius;
        next_y = game_annulus_checked_add(game_small_attached_sar(product, 13), (uint32)y, 0x800CE904);
        packet = r_u32(0x80040690);
        w_u32(packet + 4, center_color);
        w_u32(packet + 12, edge_color); w_u32(packet + 20, edge_color);
        w_u8(packet + 7, 0x30); w_u8(packet + 3, 6);
        w_u16(packet + 8, (uint32)x); w_u16(packet + 10, (uint32)y);
        w_u16(packet + 16, first_x); w_u16(packet + 18, first_y);
        w_u16(packet + 24, next_x); w_u16(packet + 26, next_y);
        ordering = r_u32(0x800421AC) + 0x4504;
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        packet = r_u32(0x80040690);
        w_u32(0x80040690, game_annulus_checked_add(packet, 28, 0x800CE9A4));
    }
}

void game_draw_twisted_ring(uint32 descriptor, uint32 first_angle_offset, uint32 second_angle_offset)
{
    uint32 first_angle, second_angle, x, y, z, first_radius, second_radius, z_offset;
    uint32 sine, cosine, cosine_angle, difference, screen_pointer, pointer, toggle, index;
    uint32 first_color, second_color, third_color, fourth_color, ordering, packet;
    uint32 first_screen, second_screen, third_screen, fourth_screen;
    SVECTOR point;
    sint32 screen[2], projection, flags;
    FUNCTION_MARKER(0x800CE9EC, "GAME.OVL");
    first_angle = game_annulus_checked_add(4096, second_angle_offset, 0x800CEA24);
    second_angle = game_annulus_checked_add(4096, first_angle_offset, 0x800CEA28);
    x = r_u32(descriptor); y = r_u32(descriptor + 4); z = r_u32(descriptor + 8);
    first_radius = r_u32(descriptor + 12); second_radius = r_u32(descriptor + 16);
    z_offset = r_u32(descriptor + 20);
    screen_pointer = 0x80033D50;
    do {
        sine = (uint32)r_s16(0x8003DC68 + (first_angle & 0xFFF) * 2);
        cosine_angle = game_annulus_checked_add(first_angle, 1024, 0x800CEA70);
        cosine = (uint32)r_s16(0x8003DC68 + (cosine_angle & 0xFFF) * 2);
        point.vx = (sint16)game_annulus_checked_add(game_small_attached_sar(second_radius * sine, 12), x, 0x800CEAA8);
        point.vy = (sint16)game_annulus_checked_add(game_small_attached_sar(second_radius * cosine, 12), y, 0x800CEAAC);
        point.vz = (sint16)game_annulus_checked_subtract(z, z_offset, 0x800CEAB0);
        RotTransPers(&point, &screen[0], &projection, &flags);
        sine = (uint32)r_s16(0x8003DC68 + (second_angle & 0xFFF) * 2);
        cosine_angle = game_annulus_checked_add(second_angle, 1024, 0x800CEB08);
        cosine = (uint32)r_s16(0x8003DC68 + (cosine_angle & 0xFFF) * 2);
        point.vx = (sint16)game_annulus_checked_add(game_small_attached_sar(first_radius * sine, 12), x, 0x800CEB40);
        point.vy = (sint16)game_annulus_checked_add(game_small_attached_sar(first_radius * cosine, 12), y, 0x800CEB44);
        point.vz = (sint16)z;
        RotTransPers(&point, &screen[1], &projection, &flags);
        w_u32(screen_pointer, (uint32)screen[0]);
        w_u32(screen_pointer + 4, (uint32)screen[1]);
        first_angle = game_annulus_checked_subtract(first_angle, 256, 0x800CEB88);
        second_angle = game_annulus_checked_subtract(second_angle, 256, 0x800CEB8C);
        screen_pointer = game_annulus_checked_add(screen_pointer, 8, 0x800CEB90);
        difference = game_annulus_checked_subtract(second_angle, first_angle_offset, 0x800CEB94);
    } while ((sint32)difference >= 0);
    screen_pointer = 0x80033D50;
    toggle = 4;
    for (index = 0; index < 16; ++index) {
        pointer = game_annulus_checked_add(descriptor, toggle, 0x800CEBB8);
        first_color = r_u32(pointer + 36);
        second_color = r_u32(pointer + 44);
        toggle = 4 - toggle;
        pointer = game_annulus_checked_add(descriptor, toggle, 0x800CEBC8);
        ordering = r_u32(0x800421AC);
        third_color = r_u32(pointer + 44);
        fourth_color = r_u32(pointer + 36);
        ordering += 0x4504;
        first_screen = r_u32(screen_pointer); second_screen = r_u32(screen_pointer + 4);
        third_screen = r_u32(screen_pointer + 8); fourth_screen = r_u32(screen_pointer + 12);
        packet = r_u32(0x80040690);
        w_u32(packet + 4, second_color); w_u32(packet + 12, first_color);
        w_u32(packet + 20, third_color); w_u32(packet + 28, fourth_color);
        w_u8(packet + 7, 0x3A); w_u8(packet + 3, 8);
        w_u32(packet + 8, first_screen); w_u32(packet + 16, second_screen);
        w_u32(packet + 24, third_screen); w_u32(packet + 32, fourth_screen);
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        packet = r_u32(0x80040690);
        w_u32(0x80040690, game_annulus_checked_add(packet, 36, 0x800CEC7C));
        screen_pointer = game_annulus_checked_add(screen_pointer, 8, 0x800CEC88);
    }
}

static void game_streak_link(uint32 packet, uint32 depth, uint32 pc)
{
    uint32 ordering = r_u32(0x800421AC), offset;
    if ((sint32)depth >= 0x4000) {
        if ((sint32)game_annulus_checked_subtract(depth, 0x3FFFFE, pc + 0x1C) > 0)
            depth = 0x3FFFFE;
        offset = ((depth >> 11) + 0x140) << 2;
        ordering = game_annulus_checked_add(offset, ordering, pc + 0x3C);
    } else {
        if ((sint32)depth < 0) depth = 0;
        offset = ((depth >> 6) + 0x40) << 2;
        ordering = game_annulus_checked_add(offset, ordering, pc + 0x60);
    }
    AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
}

static void game_streak_quad_tail(uint32 packet, uint32 color, uint32 depth, uint32 advance_pc, uint32 link_pc)
{
    w_u32(packet + 4, color); w_u32(packet + 20, 0);
    w_u32(packet + 28, 0); w_u32(packet + 12, color);
    w_u8(packet + 7, 0x3A); w_u8(packet + 3, 8);
    w_u32(0x80040690, game_annulus_checked_add(packet, 36, advance_pc));
    game_streak_link(packet, depth, link_pc);
}

static void game_streak_triangle_tail(uint32 packet, uint32 color, uint32 depth, uint32 advance_pc, uint32 link_pc)
{
    w_u32(packet + 4, color); w_u32(packet + 12, 0); w_u32(packet + 20, 0);
    w_u8(packet + 7, 0x32); w_u8(packet + 3, 6);
    w_u32(0x80040690, game_annulus_checked_add(packet, 28, advance_pc));
    game_streak_link(packet, depth, link_pc);
}

void game_update_and_draw_streak_list(uint32 record, uint32 camera)
{
    uint32 color_pointer, remaining, inner_color, outer_color, coordinate[6], origin[3], delta[3];
    uint32 absolute, magnitude, shift, maximum_shift, first_screen, second_screen, temporary;
    uint32 first_depth, second_depth, first_width, second_width, dx, dy, ax, ay, smaller, larger, length;
    uint32 width_shift, width_x, half_width_x, width_y, half_width_y;
    uint32 end_width_x, end_half_x, end_width_y, end_half_y, fx, fy, ex, ey, packet, value, index;
    uint32 velocity[3], position[3], pool_count, pool_address;
    static const uint32 relative_pc[3] = {0x800CC458, 0x800CC45C, 0x800CC460};
    static const uint32 endpoint_pc[3] = {0x800CC470, 0x800CC474, 0x800CC478};
    static const uint32 negate_pc[6] = {0x800CC490, 0x800CC49C, 0x800CC4AC, 0x800CC4BC, 0x800CC4CC, 0x800CC4DC};
    SVECTOR point;
    sint32 screen, flags;
    FUNCTION_MARKER(0x800CC3BC, "GAME.OVL");
    while (record != 0) {
        if (r_u16(record + 60) == 0) goto update_streak;
        remaining = r_u16(record + 44);
        color_pointer = r_u32(record + 20);
        value = game_annulus_checked_subtract(16, remaining, 0x800CC418);
        if ((sint32)value > 0)
            color_pointer = game_annulus_checked_add(color_pointer, value << 3, 0x800CC428);
        inner_color = r_u32(color_pointer); outer_color = r_u32(color_pointer + 4);
        w_u16(record + 66, 0);
        for (index = 0; index < 3; ++index) origin[index] = r_u32(camera + 8 + index * 4);
        for (index = 0; index < 3; ++index) coordinate[index] = r_u32(record + index * 4);
        for (index = 0; index < 3; ++index)
            coordinate[index] = game_annulus_checked_subtract(coordinate[index], origin[index], relative_pc[index]);
        for (index = 0; index < 3; ++index) delta[index] = (uint32)r_s16(record + 54 + index * 2);
        for (index = 0; index < 3; ++index)
            coordinate[index + 3] = game_annulus_checked_add(delta[index], coordinate[index], endpoint_pc[index]);
        magnitude = 0;
        for (index = 0; index < 6; ++index) {
            absolute = coordinate[index];
            if ((sint32)absolute <= 0)
                absolute = game_annulus_checked_subtract(0, absolute, negate_pc[index]);
            magnitude |= absolute;
        }
        magnitude >>= 14;
        shift = 0;
        while (magnitude != 0) { magnitude >>= 1; ++shift; }
        if (shift != 0)
            for (index = 0; index < 6; ++index) coordinate[index] = game_small_attached_sar(coordinate[index], shift);
        maximum_shift = r_u32(record + 28);
        if ((sint32)game_annulus_checked_subtract(shift, maximum_shift, 0x800CC520) > 0) goto update_streak;
        point.vx = (sint16)coordinate[0]; point.vy = (sint16)coordinate[1]; point.vz = (sint16)coordinate[2];
        first_depth = (uint32)gte_project_full_depth(&point, &screen, &flags);
        if ((uint32)flags & 0x80460000) goto update_streak;
        first_depth <<= shift & 31;
        if ((sint32)game_annulus_checked_subtract(25, first_depth, 0x800CC578) > 0) goto update_streak;
        first_screen = (uint32)screen;
        point.vx = (sint16)coordinate[3]; point.vy = (sint16)coordinate[4]; point.vz = (sint16)coordinate[5];
        second_depth = (uint32)gte_project_full_depth(&point, &screen, &flags);
        if ((uint32)flags & 0x80460000) goto update_streak;
        second_depth <<= shift & 31;
        if ((sint32)game_annulus_checked_subtract(25, second_depth, 0x800CC5CC) > 0) goto update_streak;
        second_screen = (uint32)screen;
        first_depth >>= 2; second_depth >>= 2;
        w_u16(record + 66, 1);
        value = game_annulus_checked_subtract((uint32)(sint16)first_screen, (uint32)(sint16)second_screen, 0x800CC5FC);
        if ((sint32)value >= 0) {
            temporary = first_screen; first_screen = second_screen; second_screen = temporary;
            temporary = first_depth; first_depth = second_depth; second_depth = temporary;
        }
        first_width = 32000 / first_depth;
        fx = (uint32)(sint16)first_screen; ex = (uint32)(sint16)second_screen;
        dx = game_annulus_checked_subtract(fx, ex, 0x800CC640);
        if ((sint32)first_width <= 0) first_width = 1;
        second_width = 32000 / second_depth;
        fy = game_small_attached_sar(first_screen, 16); ey = game_small_attached_sar(second_screen, 16);
        dy = game_annulus_checked_subtract(fy, ey, 0x800CC664);
        if ((sint32)second_width <= 0) second_width = 1;
        first_depth <<= 2; second_depth <<= 2;
        ax = (sint32)dx < 0 ? 0 - dx : dx; ay = (sint32)dy < 0 ? 0 - dy : dy;
        smaller = ax <= ay ? ax : ay; larger = ax <= ay ? ay : ax;
        value = game_annulus_checked_add(smaller, smaller << 2, 0x800CC6D0);
        value = game_annulus_checked_add(value, larger << 4, 0x800CC6D8);
        length = value >> 4;
        if ((sint32)length <= 0) length = 1;
        w_u32(0x8011F130, shift);
        dx = (uint32)((sint32)(dx << 3) / (sint32)length);
        dy = (uint32)((sint32)(dy << 3) / (sint32)length);
        width_shift = r_u16(record + 52);
        width_x = game_small_attached_sar(dx * first_width, width_shift); half_width_x = game_small_attached_sar(width_x, 1);
        width_y = game_small_attached_sar(dy * first_width, width_shift); half_width_y = game_small_attached_sar(width_y, 1);
        end_width_x = game_small_attached_sar(dx * second_width, width_shift); end_half_x = game_small_attached_sar(end_width_x, 1);
        end_width_y = game_small_attached_sar(dy * second_width, width_shift); end_half_y = game_small_attached_sar(end_width_y, 1);
        packet = r_u32(0x80040690);
        w_u32(packet + 8, first_screen); w_u32(packet + 16, second_screen);
        w_u16(packet + 24, game_annulus_checked_subtract(fx, width_y, 0x800CC770));
        w_u16(packet + 26, game_annulus_checked_add(fy, half_width_x, 0x800CC784));
        w_u16(packet + 32, game_annulus_checked_subtract(ex, end_width_y, 0x800CC798));
        w_u16(packet + 34, game_annulus_checked_add(ey, end_half_x, 0x800CC7AC));
        game_streak_quad_tail(packet, outer_color, first_depth, 0x800CC7F0, 0x800CC808);
        packet = r_u32(0x80040690);
        w_u32(packet + 8, first_screen); w_u32(packet + 16, second_screen);
        w_u16(packet + 24, game_annulus_checked_add(fx, width_y, 0x800CC8C8));
        w_u16(packet + 26, game_annulus_checked_subtract(fy, half_width_x, 0x800CC8DC));
        w_u16(packet + 32, game_annulus_checked_add(ex, end_width_y, 0x800CC8F0));
        w_u16(packet + 34, game_annulus_checked_subtract(ey, end_half_x, 0x800CC904));
        game_streak_quad_tail(packet, outer_color, first_depth, 0x800CC948, 0x800CC960);
        packet = r_u32(0x80040690);
        w_u32(packet + 8, first_screen);
        w_u16(packet + 16, game_annulus_checked_add(fx, width_y, 0x800CCA1C));
        w_u16(packet + 18, game_annulus_checked_subtract(fy, half_width_x, 0x800CCA30));
        w_u16(packet + 24, game_annulus_checked_add(fx, width_x, 0x800CCA44));
        w_u16(packet + 26, game_annulus_checked_add(fy, half_width_y, 0x800CCA58));
        game_streak_triangle_tail(packet, outer_color, first_depth, 0x800CCA98, 0x800CCAB0);
        packet = r_u32(0x80040690);
        w_u32(packet + 8, first_screen);
        w_u16(packet + 16, game_annulus_checked_subtract(fx, width_y, 0x800CCB6C));
        w_u16(packet + 18, game_annulus_checked_add(fy, half_width_x, 0x800CCB80));
        w_u16(packet + 24, game_annulus_checked_add(fx, width_x, 0x800CCB94));
        w_u16(packet + 26, game_annulus_checked_add(fy, half_width_y, 0x800CCBA8));
        game_streak_triangle_tail(packet, outer_color, first_depth, 0x800CCBE8, 0x800CCC00);
        packet = r_u32(0x80040690);
        w_u32(packet + 8, second_screen);
        w_u16(packet + 16, game_annulus_checked_add(ex, end_width_y, 0x800CCCBC));
        w_u16(packet + 18, game_annulus_checked_subtract(ey, end_half_x, 0x800CCCD0));
        w_u16(packet + 24, game_annulus_checked_subtract(ex, end_width_x, 0x800CCCE4));
        w_u16(packet + 26, game_annulus_checked_subtract(ey, end_half_y, 0x800CCCF8));
        game_streak_triangle_tail(packet, outer_color, first_depth, 0x800CCD38, 0x800CCD50);
        packet = r_u32(0x80040690);
        w_u32(packet + 8, second_screen);
        w_u16(packet + 16, game_annulus_checked_subtract(ex, end_width_y, 0x800CCE0C));
        w_u16(packet + 18, game_annulus_checked_add(ey, end_half_x, 0x800CCE20));
        w_u16(packet + 24, game_annulus_checked_subtract(ex, end_width_x, 0x800CCE34));
        w_u16(packet + 26, game_annulus_checked_subtract(ey, end_half_y, 0x800CCE48));
        game_streak_triangle_tail(packet, outer_color, first_depth, 0x800CCE88, 0x800CCEA0);
        value = r_u32(0x8011F130);
        if ((sint32)game_annulus_checked_add(value, (uint32)-3, 0x800CCF50) > 0) goto update_streak;
        first_width = game_annulus_checked_add(first_width, (uint32)-4, 0x800CCF5C);
        if ((sint32)first_width < 0) {
            packet = r_u32(0x80040690);
            w_u8(packet + 3, 3); w_u32(packet + 4, inner_color); w_u8(packet + 7, 0x40);
            w_u32(packet + 8, first_screen); w_u32(packet + 12, second_screen);
            w_u32(0x80040690, game_annulus_checked_add(packet, 16, 0x800CD290));
            game_streak_link(packet, first_depth, 0x800CD2A8);
        } else {
            half_width_x = game_small_attached_sar(width_x, 2); width_x = game_small_attached_sar(width_y, 2);
            width_y = game_small_attached_sar(half_width_x, 1); half_width_y = game_small_attached_sar(width_x, 1);
            end_half_x = game_small_attached_sar(end_width_x, 2); end_width_x = game_small_attached_sar(end_width_y, 2);
            end_width_y = game_small_attached_sar(end_half_x, 1); end_half_y = game_small_attached_sar(end_width_x, 1);
            packet = r_u32(0x80040690);
            value = game_annulus_checked_subtract(fx, width_x, 0x800CCF98);
            w_u16(packet + 24, game_annulus_checked_subtract(value, half_width_x, 0x800CCF9C));
            value = game_annulus_checked_add(fy, width_y, 0x800CCFB0);
            w_u16(packet + 26, game_annulus_checked_subtract(value, half_width_y, 0x800CCFB4));
            value = game_annulus_checked_subtract(ex, end_width_x, 0x800CCFC8);
            w_u16(packet + 32, game_annulus_checked_add(value, end_half_x, 0x800CCFCC));
            value = game_annulus_checked_add(ey, end_width_y, 0x800CCFE0);
            w_u16(packet + 34, game_annulus_checked_add(value, end_half_y, 0x800CCFE4));
            w_u32(packet + 8, first_screen); w_u32(packet + 16, second_screen);
            game_streak_quad_tail(packet, inner_color, first_depth, 0x800CD030, 0x800CD048);
            packet = r_u32(0x80040690);
            w_u32(packet + 8, first_screen); w_u32(packet + 16, second_screen);
            value = game_annulus_checked_add(fx, width_x, 0x800CD108);
            w_u16(packet + 24, game_annulus_checked_subtract(value, half_width_x, 0x800CD10C));
            value = game_annulus_checked_subtract(fy, width_y, 0x800CD120);
            w_u16(packet + 26, game_annulus_checked_subtract(value, half_width_y, 0x800CD124));
            value = game_annulus_checked_add(ex, end_width_x, 0x800CD138);
            w_u16(packet + 32, game_annulus_checked_add(value, end_half_x, 0x800CD13C));
            value = game_annulus_checked_subtract(ey, end_width_y, 0x800CD150);
            w_u16(packet + 34, game_annulus_checked_add(value, end_half_y, 0x800CD154));
            game_streak_quad_tail(packet, inner_color, first_depth, 0x800CD198, 0x800CD1B0);
        }
update_streak:
        remaining = r_u16(record + 44);
        remaining = game_annulus_checked_add(remaining, (uint32)-1, 0x800CD354);
        w_u16(record + 44, remaining);
        if ((sint32)remaining <= 0 && r_u16(record + 60) != 0) {
            w_u16(record + 60, 0);
            pool_count = r_u32(0x800EF114);
            pool_address = game_annulus_checked_add(0x801263C0, pool_count << 2, 0x800CD390);
            w_u32(pool_address, record);
            w_u32(0x800EF114, pool_count + 1);
        }
        for (index = 0; index < 3; ++index) velocity[index] = r_u32(record + 32 + index * 4);
        for (index = 0; index < 3; ++index) position[index] = r_u32(record + index * 4);
        w_u32(record, game_annulus_checked_add(position[0], velocity[0], 0x800CD3C4));
        w_u32(record + 4, game_annulus_checked_add(position[1], velocity[1], 0x800CD3CC));
        w_u32(record + 8, game_annulus_checked_add(position[2], velocity[2], 0x800CD3D4));
        record = r_u32(record + 80);
    }
}
