#include "game_functions.h"
#include <stdio.h>
#include <string.h>

static uint32 game_curve_checked_add(uint32 first, uint32 second, uint32 pc)
{
    sint64 result = (sint64)(sint32)first + (sint32)second;
    if (result > 2147483647 || result < (-2147483647 - 1))
        xport_mips_overflow_exception(pc);
    return first + second;
}

static uint32 game_curve_checked_subtract(uint32 first, uint32 second, uint32 pc)
{
    sint64 result = (sint64)(sint32)first - (sint32)second;
    if (result > 2147483647 || result < (-2147483647 - 1))
        xport_mips_overflow_exception(pc);
    return first - second;
}

static uint32 game_curve_component(uint32 points, const VECTOR *native, uint32 camera, uint32 axis, const uint32 weights[3])
{
    uint32 origin = r_u32(camera + 8 + axis * 4);
    uint32 first, second, third, fourth;
    uint32 pc = 0x800D0390 + axis * 0x54, value;
    if (native)
    {
        memcpy(&first, (const uint8 *)&native[0] + axis * 4, 4);
        memcpy(&second, (const uint8 *)&native[1] + axis * 4, 4);
        memcpy(&third, (const uint8 *)&native[2] + axis * 4, 4);
        memcpy(&fourth, (const uint8 *)&native[3] + axis * 4, 4);
    }
    else
    {
        first = r_u32(points + axis * 4);
        second = r_u32(points + 16 + axis * 4);
        third = r_u32(points + 32 + axis * 4);
        fourth = r_u32(points + 48 + axis * 4);
    }
    second = game_curve_checked_subtract(second, first, pc);
    third = game_curve_checked_subtract(third, first, pc + 4);
    fourth = game_curve_checked_subtract(fourth, first, pc + 8);
    second *= weights[0]; third *= weights[1]; fourth *= weights[2];
    first = game_curve_checked_subtract(first, origin, pc + 0x24);
    second = (uint32)((sint32)second >> 8);
    third = (uint32)((sint32)third >> 8);
    fourth = (uint32)((sint32)fourth >> 8);
    value = game_curve_checked_add(first, second, pc + 0x34);
    value = game_curve_checked_add(value, third, pc + 0x38);
    return game_curve_checked_add(value, fourth, pc + 0x3C);
}

static void game_curve_line(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 color, uint32 depth, uint32 link_pc, uint32 advance_pc)
{
    uint32 packet = r_u32(0x80040690), ordering, offset;
    w_u32(packet + 4, color); w_u32(packet + 12, color);
    w_u8(packet + 7, 0x52); w_u8(packet + 3, 4);
    w_u16(packet + 8, x0); w_u16(packet + 10, y0);
    w_u16(packet + 16, x1); w_u16(packet + 18, y1);
    ordering = r_u32(0x800421AC);
    if ((sint32)depth >= 0x4000) {
        if ((sint32)game_curve_checked_subtract(depth, 0x3FFFFE, link_pc + 0x1C) > 0)
            depth = 0x3FFFFE;
        offset = ((depth >> 11) + 0x140) << 2;
        ordering = game_curve_checked_add(offset, ordering, link_pc + 0x3C);
    } else {
        if ((sint32)depth < 0) depth = 0;
        offset = ((depth >> 6) + 0x40) << 2;
        ordering = game_curve_checked_add(offset, ordering, link_pc + 0x60);
    }
    AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
    packet = r_u32(0x80040690);
    w_u32(0x80040690, game_curve_checked_add(packet, 20, advance_pc));
}

static void game_draw_curve_buffer(uint32 points, const VECTOR *native, uint32 camera)
{
    uint32 weights[3], coordinates[3], index, axis, pass, phase, color_address, color, depth;
    uint32 x0, y0, x1, y1;
    sint32 range_sum, screen, flags;
    SVECTOR point;
    for (index = 0; index < 16; ++index) {
        weights[0] = r_u32(0x800F8108 + index * 4);
        weights[1] = r_u32(0x800F8150 + index * 4);
        weights[2] = r_u32(0x800F8190 + index * 4);
        for (axis = 0; axis < 3; ++axis)
            coordinates[axis] = game_curve_component(points, native, camera, axis, weights);
        range_sum = 0;
        for (axis = 0; axis < 3; ++axis) {
            coordinates[axis] = (uint32)((sint32)coordinates[axis] >> 4);
            range_sum += (((sint32)coordinates[axis] >> 15) + 1) >> 1;
        }
        if (range_sum != 0) return;
        point.vx = (sint16)coordinates[0]; point.vy = (sint16)coordinates[1]; point.vz = (sint16)coordinates[2];
        depth = (uint32)gte_project_full_depth(&point, &screen, &flags);
        w_u32(0x800F2AC8 + index * 4, (uint32)flags);
        w_u32(0x800510F8 + index * 4, depth << 4);
        w_u32(0x80033D50 + index * 4, (uint32)screen);
    }
    for (pass = 0; pass < 2; ++pass) {
        phase = r_u32(0x800EF550);
        for (index = 0; index < 15; ++index) {
            x0 = (uint32)r_s16(0x80033D50 + index * 4);
            y0 = (uint32)r_s16(0x80033D52 + index * 4);
            x1 = (uint32)r_s16(0x80033D54 + index * 4);
            y1 = (uint32)r_s16(0x80033D56 + index * 4);
            if ((r_u32(0x800F2AC8 + index * 4) >> 16) != 0) continue;
            depth = r_u32(0x800510F8 + index * 4);
            depth = game_curve_checked_add(depth, (uint32)-16, pass == 0 ? 0x800D0580 : 0x800D0704);
            if ((sint32)depth <= 0) continue;
            color_address = game_curve_checked_add(0x8011A128 + pass * 64, phase << 2, pass == 0 ? 0x800D0590 : 0x800D0714);
            color = r_u32(color_address);
            phase = game_curve_checked_add(phase, 1, pass == 0 ? 0x800D0598 : 0x800D071C) & 15;
            if (pass == 0) {
                game_curve_line(x0, y0, x1, y1, color, depth, 0x800D05DC, 0x800D0688);
            } else {
                --x0; --x1;
                game_curve_line(x0, y0, x1, y1, color, depth, 0x800D0768, 0x800D0814);
                x0 += 2; x1 += 2;
                game_curve_line(x0, y0, x1, y1, color, depth, 0x800D0864, 0x800D0910);
                --x0; --x1; --y0; --y1;
                game_curve_line(x0, y0, x1, y1, color, depth, 0x800D0968, 0x800D0A14);
                y0 += 2; y1 += 2;
                game_curve_line(x0, y0, x1, y1, color, depth, 0x800D0A64, 0x800D0B10);
            }
        }
    }
}

void game_draw_weighted_curve(uint32 points, uint32 camera)
{
    FUNCTION_MARKER(0x800D02FC, "GAME.OVL");
    game_draw_curve_buffer(points, NULL, camera);
}

void game_draw_tractor_curve_native(const VECTOR *points, uint32 camera)
{
    game_draw_curve_buffer(0, points, camera);
}

uint32 game_finish_object_action(uint32 object)
{
    uint32 type, callback;
    FUNCTION_MARKER(0x800D332C, "GAME.OVL");
    type = r_u32(object);
    w_u32(object + 0x118, 0); w_u8(object + 0x270, 0);
    if (type == 1 || type == 3) w_u32(object + 0x238, 0);
    w_u32(object + 0x44, 0x8002493C);
    callback = r_u32(object + 0x48);
    w_u32(object + 0xA0, 0);
    return psx_dispatch_guest_call(callback, 1, object, 0, 0);
}

uint32 game_update_object_removal_delay(uint32 object)
{
    uint32 remaining;
    FUNCTION_MARKER(0x800D3384, "GAME.OVL");
    remaining = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, remaining);
    if ((sint32)remaining <= 0) {
        remaining = 0x800D3478;
        w_u32(object + 0x40, remaining);
    }
    return remaining;
}

void game_begin_object_destruction(uint32 object)
{
    uint32 tracked;
    FUNCTION_MARKER(0x800D33AC, "GAME.OVL");
    if (r_s32(object + 0x238) > 0) {
        game_remove_object_and_resources(object);
        return;
    }
    game_emit_model_breakup(object);
    tracked = r_u32(0x800EF4E0);
    w_u32(object + 0x9C, 60); w_u32(object + 0x40, 0x800D3384);
    w_u32(object + 0x44, 0x8002493C); w_u32(object + 0x5C, 0x8002493C);
    w_u32(object + 0x3C, 0x8002493C); w_u32(object + 0x48, 0x800D3478);
    if (object == tracked) w_u32(0x800EF4E0, 0);
    if (object == r_u32(0x800EF4E4)) w_u32(0x800EF4E4, 0);
    if (object == r_u32(0x800EF4E8)) w_u32(0x800EF4E8, 0);
}

static const char *game_lifecycle_text_native(uint32 address)
{
    uint32 length = 0;
    while (r_u8(address + length) != 0) ++length;
    return (const char *)psx_addr(address, (size_t)length + 1);
}

void game_remove_object_and_resources(uint32 object)
{
    uint32 value, offset, slot, format, tracked, relation, related, flags, pointer, index, counter_address;
    static const uint32 released_fields[] = {0x200, 0x204, 0x210, 0x278, 0x1E8, 0x1EC, 0x1F0, 0x1F4};
    FUNCTION_MARKER(0x800D3478, "GAME.OVL");
    if (r_u32(object) == 0) return;
    value = r_u32(object + 0x238);
    w_u32(object + 0x3C, 0x8002493C);
    if ((sint32)value <= 0) {
        game_expire_attached_effects(object);
        if (!(r_u32(object + 0x114) & 0x10)) game_emit_object_debris(object);
        offset = r_u32(object + 4) * 60;
        value = r_u16(0x800536BE + offset);
        w_u16(0x800536BE + offset, value + 1);
        if (r_s16(object + 0x268) != 0) {
            slot = (uint32)r_s16(object + 0x24E);
            w_u16(0x80059E90 + slot * 44, 1);
        }
        if (r_u32(0x800F18AC) == 0 && (r_u32(object + 0x114) & 0x10)) {
            value = r_u32(object);
            slot = (uint32)r_s16(object + 0x24E);
            format = r_u32(value == 3 ? 0x800F246C : 0x800F2470);
            sprintfPSX(0x800FA368, format, 0x80059E70 + slot * 44);
            game_queue_hud_message(0x800FA368);
        }
    } else {
        offset = r_u32(object + 4) * 60;
        value = r_u16(0x800536C0 + offset);
        w_u16(0x800536C0 + offset, value + 1);
        if (r_s16(object + 0x268) != 0) {
            slot = (uint32)r_s16(object + 0x24E);
            w_u16(0x80059E92 + slot * 44, 1);
        }
    }
    if (object == r_u32(0x800EF4E0)) w_u32(0x800EF4E0, 0);
    if (object == r_u32(0x800EF4E4)) w_u32(0x800EF4E4, 0);
    if (object == r_u32(0x800EF4E8)) w_u32(0x800EF4E8, 0);
    pointer = r_u32(object + 0x20C);
    if (pointer != 0) {
        tracked = r_u32(object + 0x170);
        if (tracked != 0) {
            value = r_u16(object + 0x174);
            if (value == r_u16(tracked + 0x24A)) w_u16(pointer, 0);
        }
    }
    if (r_s16(object + 0x1C4) != 0) heap_free(r_u32(object + 0x1CC));
    pointer = r_u32(object + 0x22C);
    if (pointer != 0) heap_free(pointer);
    index = 0;
    if (r_u8(object + 0x272) != 0) {
        do {
            game_create_escape_pod(object);
            value = r_u8(object + 0x272);
            ++index;
        } while ((sint32)index < (sint32)value);
    }
    relation = r_u32(object + 0x208);
    if (relation != 0) {
        related = r_u32(relation + 0x38);
        if (related != 0) {
            value = r_u16(relation + 0x3C);
            if (value == r_u16(related + 0x24A)) {
                flags = r_u32(related + 0x114);
                if (flags & 0x40000) w_u32(related + 0x114, flags ^ 0x40000);
                related = r_u32(relation + 0x38);
                value = r_u32(related + 0xDC);
                if (value == 0) w_u32(related + 0x40, r_u32(relation + 0x44));
                else w_u32(related + 0x40, 0x8009AB58);
                related = r_u32(relation + 0x38);
                value = r_u32(relation + 0x48);
                w_u32(related + 0x3C, value);
                related = r_u32(relation + 0x38);
                value = r_u32(relation + 0x4C);
                w_u32(related + 0x44, value);
            }
        }
        heap_free(r_u32(object + 0x208));
    }
    value = r_u32(object);
    counter_address = value == 3 ? 0x800595B0 : value == 1 ? 0x800595B4 : 0;
    if (counter_address != 0) w_u32(counter_address, r_u32(counter_address) - 1);
    slot = (uint32)r_s16(object + 0x24E);
    w_u32(0x80059E68 + slot * 44, 0);
    pointer = r_u32(object + 0xF4);
    if (pointer != 0) heap_free(pointer);
    if (r_u32(object + 0x114) & 0x1000) {
        tracked = r_u32(object + 0x170);
        if (tracked != 0) {
            value = r_u16(object + 0x174);
            if (value == r_u16(tracked + 0x24A)) {
                value = r_u8(tracked + 0x26D);
                if (value != 0) w_u8(tracked + 0x26D, value - 1);
            }
        }
    }
    tracked = r_u32(0x800EF4E0);
    w_u32(object + 0x118, 0);
    if (object == tracked) w_u32(0x800EF4E0, 0);
    game_free_object_auxiliary_buffer(object);
    for (index = 0; index < sizeof(released_fields) / sizeof(released_fields[0]); ++index) {
        pointer = r_u32(object + released_fields[index]);
        if (pointer != 0) heap_free(pointer);
    }
    game_mark_object_for_removal(object);
}

void game_free_object_auxiliary_buffer(uint32 object)
{
    uint32 pointer;
    FUNCTION_MARKER(0x800EB138, "GAME.OVL");
    pointer = r_u32(object + 0x110);
    if (pointer != 0) heap_free(pointer);
}

static void game_initialize_attached_effect_record(uint32 object, uint32 field, uint32 vertices, uint32 vertex_a, uint32 vertex_b, uint32 width, uint32 length, uint32 color_a, uint32 color_b, uint32 offset)
{
    uint32 allocated, first, second, destination;
    sint32 first_coordinate, second_coordinate, midpoint;
    if (vertex_a == 0xFFFFFFFF) {
        w_u32(object + field, 0);
        return;
    }
    allocated = heap_alloc_back(36);
    first = vertices + (vertex_a << 3); second = vertices + (vertex_b << 3);
    w_u32(object + field, allocated);
    first_coordinate = r_s16(first); second_coordinate = r_s16(second);
    midpoint = (first_coordinate + second_coordinate) / 2;
    w_u16(allocated, (uint32)midpoint);
    first_coordinate = r_s16(first + 2); second_coordinate = r_s16(second + 2);
    midpoint = (first_coordinate + second_coordinate) / 2;
    destination = r_u32(object + field);
    w_u16(destination + 2, (uint32)midpoint);
    first_coordinate = r_s16(first + 4); second_coordinate = r_s16(second + 4);
    midpoint = (first_coordinate + second_coordinate) / 2;
    destination = r_u32(object + field);
    w_u16(destination + 4, (uint32)midpoint);
    destination = r_u32(object + field); w_u32(destination + 8, width);
    destination = r_u32(object + field); w_u32(destination + 12, length);
    destination = r_u32(object + field); w_u32(destination + 20, color_a);
    destination = r_u32(object + field); w_u32(destination + 24, color_b);
    destination = r_u32(object + field); w_u32(destination + 28, offset);
}

void game_initialize_four_attached_effects(uint32 object, uint32 first_a, uint32 first_b, uint32 first_width, uint32 first_length, uint32 first_color_a, uint32 first_color_b, uint32 first_offset, uint32 second_a, uint32 second_b, uint32 second_width, uint32 second_length, uint32 second_color_a, uint32 second_color_b, uint32 second_offset, uint32 third_a, uint32 third_b, uint32 third_width, uint32 third_length, uint32 third_color_a, uint32 third_color_b, uint32 third_offset, uint32 fourth_a, uint32 fourth_b, uint32 fourth_width, uint32 fourth_length, uint32 fourth_color_a, uint32 fourth_color_b, uint32 fourth_offset)
{
    uint32 model, vertices;
    FUNCTION_MARKER(0x800D39D8, "GAME.OVL");
    model = r_u32(object + 0x64);
    vertices = r_u32(model);
    game_initialize_attached_effect_record(object, 0x1E8, vertices, first_a, first_b, first_width, first_length, first_color_a, first_color_b, first_offset);
    game_initialize_attached_effect_record(object, 0x1EC, vertices, second_a, second_b, second_width, second_length, second_color_a, second_color_b, second_offset);
    game_initialize_attached_effect_record(object, 0x1F0, vertices, third_a, third_b, third_width, third_length, third_color_a, third_color_b, third_offset);
    game_initialize_attached_effect_record(object, 0x1F4, vertices, fourth_a, fourth_b, fourth_width, fourth_length, fourth_color_a, fourth_color_b, fourth_offset);
}

void game_initialize_three_attached_effects(uint32 object, uint32 first_a, uint32 first_b, uint32 first_width, uint32 first_length, uint32 first_color_a, uint32 first_color_b, uint32 first_offset, uint32 second_a, uint32 second_b, uint32 second_width, uint32 second_length, uint32 second_color_a, uint32 second_color_b, uint32 second_offset, uint32 third_a, uint32 third_b, uint32 third_width, uint32 third_length, uint32 third_color_a, uint32 third_color_b, uint32 third_offset)
{
    FUNCTION_MARKER(0x800D3E00, "GAME.OVL");
    game_initialize_four_attached_effects(object, first_a, first_b, first_width, first_length, first_color_a, first_color_b, first_offset, second_a, second_b, second_width, second_length, second_color_a, second_color_b, second_offset, third_a, third_b, third_width, third_length, third_color_a, third_color_b, third_offset, -1, -1, -1, -1, -1, -1, -1);
}

void game_initialize_two_attached_effects(uint32 object, uint32 first_a, uint32 first_b, uint32 first_width, uint32 first_length, uint32 first_color_a, uint32 first_color_b, uint32 first_offset, uint32 second_a, uint32 second_b, uint32 second_width, uint32 second_length, uint32 second_color_a, uint32 second_color_b, uint32 second_offset)
{
    FUNCTION_MARKER(0x800D3F0C, "GAME.OVL");
    game_initialize_four_attached_effects(object, first_a, first_b, first_width, first_length, first_color_a, first_color_b, first_offset, second_a, second_b, second_width, second_length, second_color_a, second_color_b, second_offset, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
}

void game_initialize_one_attached_effect(uint32 object, uint32 vertex_a, uint32 vertex_b, uint32 width, uint32 length, uint32 color_a, uint32 color_b, uint32 offset)
{
    FUNCTION_MARKER(0x800D3FCC, "GAME.OVL");
    game_initialize_four_attached_effects(object, vertex_a, vertex_b, width, length, color_a, color_b, offset, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
}

uint32 game_reset_object_control_state(uint32 object)
{
    uint32 pointer, value;
    FUNCTION_MARKER(0x800D4060, "GAME.OVL");
    w_u8(r_u32(object + 0x200), 0);
    w_u16(r_u32(object + 0x200) + 2, 0);
    w_u16(r_u32(object + 0x200) + 4, 0);
    w_u16(r_u32(object + 0x200) + 6, 0);
    w_u16(r_u32(object + 0x200) + 8, 0);
    w_u16(r_u32(object + 0x200) + 10, 0);
    w_u8(r_u32(object + 0x200) + 12, 0);
    w_u32(r_u32(object + 0x200) + 16, 0);
    w_u16(r_u32(object + 0x200) + 20, 0);
    w_u16(r_u32(object + 0x200) + 22, 0);
    w_u32(r_u32(object + 0x200) + 24, 0);
    w_u32(r_u32(object + 0x200) + 28, 0);
    w_u32(r_u32(object + 0x200) + 32, 0);
    w_u32(r_u32(object + 0x200) + 36, 0);
    w_u32(r_u32(object + 0x200) + 40, 0);
    w_u32(r_u32(object + 0x200) + 44, 0);
    w_u32(r_u32(object + 0x200) + 48, 0);
    w_u16(r_u32(object + 0x200) + 52, 0);
    w_u32(r_u32(object + 0x200) + 56, 0);
    pointer = r_u32(object + 0x200);
    w_u32(pointer + 68, 0); w_u32(pointer + 64, 0); w_u32(pointer + 60, 0);
    w_u32(r_u32(object + 0x200) + 76, 0);
    w_u32(r_u32(object + 0x200) + 92, 0);
    w_u32(r_u32(object + 0x200) + 108, 0);
    w_u32(r_u32(object + 0x200) + 124, 0);
    w_u32(r_u32(object + 0x200) + 140, 0);
    pointer = r_u32(object + 0x200);
    w_u32(pointer + 160, 0); w_u32(pointer + 156, 0);
    pointer = r_u32(object + 0x200);
    value = r_u32(pointer + 172);
    w_u32(pointer + 168, value); w_u32(pointer + 164, value);
    pointer = r_u32(object + 0x200);
    value = r_u32(pointer + 184);
    w_u32(pointer + 180, value); w_u32(pointer + 176, value);
    return pointer;
}

uint32 game_allocate_object_state_22c(uint32 object)
{
    uint32 pointer;
    FUNCTION_MARKER(0x800D4318, "GAME.OVL");
    pointer = heap_alloc_back(20);
    w_u32(object + 0x22C, pointer);
    w_u32(pointer, 0);
    pointer = r_u32(object + 0x22C);
    w_u16(pointer + 8, 0);
    return pointer;
}

static uint32 game_initialize_object_control_body(uint32 object, uint32 hit_callback, uint32 callback_table)
{
    uint32 pointer, type, flags;
    w_u32(object + 0x3C, 0x800D1C3C);
    w_u32(object + 0x5C, 0x800D332C);
    w_u32(object + 0x48, 0x800D3478);
    w_u32(object + 0x54, hit_callback);
    w_u32(object + 0x58, 0x800D9D90);
    w_u32(object + 0x6C, callback_table);
    pointer = heap_alloc_back(192);
    w_u32(object + 0x200, pointer);
    game_reset_object_control_state(object);
    w_u16(r_u32(object + 0x200) + 4, 0);
    w_u16(r_u32(object + 0x200) + 6, 0);
    w_u16(r_u32(object + 0x200) + 8, 0);
    w_u16(r_u32(object + 0x200) + 2, 0);
    w_u8(r_u32(object + 0x200) + 12, 0);
    w_u16(r_u32(object + 0x200) + 10, 0);
    w_u32(r_u32(object + 0x200) + 16, 0);
    w_u16(r_u32(object + 0x200) + 20, 0);
    w_u16(r_u32(object + 0x200) + 22, 0);
    w_u8(r_u32(object + 0x200), 0);
    type = r_u32(object + 4);
    if (r_u8(0x800536C6 + type * 60) == 0x46) {
        flags = r_u32(object + 0x114);
        w_u32(object + 0x114, flags | 0x100);
    }
    return game_allocate_object_state_22c(object);
}

uint32 game_initialize_object_control(uint32 object)
{
    FUNCTION_MARKER(0x800D41D8, "GAME.OVL");
    return game_initialize_object_control_body(object, 0x800DA210, 0x800EF574);
}

uint32 game_initialize_object_control_alternate(uint32 object)
{
    FUNCTION_MARKER(0x800D4358, "GAME.OVL");
    return game_initialize_object_control_body(object, 0x800DB30C, 0x800EF594);
}

static void game_attach_initial_random_light(uint32 object, uint32 type, uint32 color)
{
    uint32 seed, mixed, intensity, parameter;
    seed = r_u32(0x800EF50C);
    mixed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    intensity = (mixed & 15) | 32;
    seed = (uint32)((sint32)mixed >> 1) ^ (mixed << 3);
    w_u32(0x800EF50C, seed);
    parameter = seed & 31;
    game_attach_object_light(object, type, color, intensity, parameter);
}

void game_initialize_object_variant_4498(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800D4498, "GAME.OVL");
    game_initialize_object_control(object);
    game_attach_initial_random_light(object, 25, 0xFF0000);
    game_configure_object_subsystems(object, 0x800F07FC);
    w_u16(object + 0x264, 1);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x100000);
    game_initialize_one_attached_effect(object, 9, 49, 500, 2000, 0xFFFFFF, 0x2080FF, 0);
}

void game_initialize_object_variant_4568(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800D4568, "GAME.OVL");
    game_initialize_object_control(object);
    game_attach_initial_random_light(object, 20, 0x09AFFF);
    game_configure_object_subsystems(object, 0x800F0910);
    w_u16(object + 0x264, 1);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x100000);
    game_initialize_three_attached_effects(object, 28, 32, 500, 2000, 0xFFFFFF, 0x2080FF, 0, 1, 19, 750, 3000, 0xFFFFFF, 0x2080FF, 0, 41, 58, 500, 2000, 0xFFFFFF, 0x2080FF, 0);
}

void game_initialize_object_variant_4690(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800D4690, "GAME.OVL");
    game_initialize_object_control(object);
    game_configure_object_subsystems(object, 0x800F09DC);
    game_attach_initial_random_light(object, 27, 0xFF);
    w_u32(object + 0x6C, 0x800EF574);
    flags = r_u32(object + 0x114);
    w_u16(object + 0x264, 0);
    w_u32(object + 0x114, flags | 0x100000);
    game_initialize_one_attached_effect(object, 0, 2, 500, 2000, 0xFFFFFF, 0x2080FF, 0);
}

void game_initialize_object_variant_4768(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800D4768, "GAME.OVL");
    game_initialize_object_control(object);
    game_attach_initial_random_light(object, 5, 0xECFF02);
    game_configure_object_subsystems(object, 0x800F0AA0);
    w_u32(object + 0x6C, 0x800EF574);
    flags = r_u32(object + 0x114);
    w_u16(object + 0x264, 0);
    w_u32(object + 0x114, flags | 0x100000);
    game_initialize_one_attached_effect(object, 0, 2, 500, 2000, 0xFFFFFF, 0x2080FF, 0);
}

void game_initialize_object_variant_4844(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800D4844, "GAME.OVL");
    game_initialize_object_control(object);
    game_attach_initial_random_light(object, 27, 0x600060);
    game_configure_object_subsystems(object, 0x800F0BAC);
    flags = r_u32(object + 0x114);
    w_u16(object + 0x264, 0);
    w_u32(object + 0x114, flags | 0x100000);
    game_initialize_two_attached_effects(object, 50, 6, 500, 2000, 0xFFFFFF, 0x2080FF, 0, 53, 9, 500, 2000, 0xFFFFFF, 0x2080FF, 0);
}

void game_initialize_object_variant_493c(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800D493C, "GAME.OVL");
    game_initialize_object_control(object);
    game_configure_object_subsystems(object, 0x800F0DFC);
    game_attach_initial_random_light(object, 42, 0xFF2020);
    game_attach_initial_random_light(object, 46, 0xFF2020);
    w_u16(object + 0x264, 1);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x100000);
    game_initialize_one_attached_effect(object, 1, 16, 500, 2000, 0xFFFFFF, 0x2080FF, 0);
}

void game_initialize_object_variant_4a5c(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800D4A5C, "GAME.OVL");
    game_initialize_object_control(object);
    game_attach_initial_random_light(object, 5, 0xFF2020);
    game_configure_object_subsystems(object, 0x800F0CC0);
    w_u16(object + 0x264, 1);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x100000);
    game_initialize_two_attached_effects(object, 20, 38, 500, 2000, 0xFFFFFF, 0x2080FF, 0, 39, 57, 500, 2000, 0xFFFFFF, 0x2080FF, 0);
}

static void game_attach_model_initial_light(uint32 object, uint32 expected_name, uint32 matching_color, uint32 type)
{
    uint32 model = r_u32(object + 4), color;
    const char *name = game_lifecycle_text_native(0x800536C6 + model * 60);
    const char *expected = game_lifecycle_text_native(expected_name);
    color = strcmp(name, expected) == 0 ? matching_color : 0x025E22;
    game_attach_initial_random_light(object, type, color);
}

void game_initialize_object_variant_4b58(uint32 object)
{
    FUNCTION_MARKER(0x800D4B58, "GAME.OVL");
    game_initialize_object_control_alternate(object);
    game_configure_object_subsystems(object, 0x800F0F28);
    game_attach_model_initial_light(object, 0x80075FD8, 0xECFF02, 1);
    game_initialize_one_attached_effect(object, 77, 74, 357, 1500, 0xFFFFFF, 0xFF4000, 0);
}

void game_initialize_object_variant_4c48(uint32 object)
{
    FUNCTION_MARKER(0x800D4C48, "GAME.OVL");
    game_initialize_object_control_alternate(object);
    game_configure_object_subsystems(object, 0x800F0FFC);
    game_attach_model_initial_light(object, 0x80075FE4, 0xFF, 1);
    game_initialize_two_attached_effects(object, 127, 128, 500, 2000, 0xFFFFFF, 0xFF4000, 0, 131, 132, 500, 2000, 0xFFFFFF, 0xFF4000, 0);
}

void game_initialize_object_variant_4d5c(uint32 object)
{
    FUNCTION_MARKER(0x800D4D5C, "GAME.OVL");
    game_initialize_object_control_alternate(object);
    game_configure_object_subsystems(object, 0x800F1110);
    game_attach_model_initial_light(object, 0x80075FEC, 0x600060, 84);
    game_initialize_two_attached_effects(object, 89, 90, 500, 2000, 0xFFFFFF, 0xFF4000, 0, 93, 94, 500, 2000, 0xFFFFFF, 0xFF4000, 0);
}

void game_initialize_object_variant_4e74(uint32 object)
{
    FUNCTION_MARKER(0x800D4E74, "GAME.OVL");
    game_initialize_object_control_alternate(object);
    game_configure_object_subsystems(object, 0x800F122C);
    game_attach_model_initial_light(object, 0x80075FF8, 0xFF0000, 76);
    game_initialize_one_attached_effect(object, 81, 80, 500, 2000, 0xFFFFFF, 0xFF4000, -100);
}

void game_initialize_object_variant_4f64(uint32 object)
{
    FUNCTION_MARKER(0x800D4F64, "GAME.OVL");
    game_initialize_object_control_alternate(object);
    game_configure_object_subsystems(object, 0x800F1348);
    game_attach_model_initial_light(object, 0x80076000, 0x09AFFF, 19);
    game_initialize_one_attached_effect(object, 126, 126, 700, 2500, 0xFFFFFF, 0xFF4000, 0);
}

void game_initialize_object_variant_5054(uint32 object)
{
    FUNCTION_MARKER(0x800D5054, "GAME.OVL");
    game_initialize_object_control_alternate(object);
    game_configure_object_subsystems(object, 0x800F148C);
    game_attach_model_initial_light(object, 0x80076008, 0xFF2020, 86);
    game_initialize_one_attached_effect(object, 37, 43, 500, 2000, 0xFFFFFF, 0xFF4000, 0);
}

uint32 game_initialize_object_variant_5144(uint32 object)
{
    uint32 flags, seed, mixed;
    FUNCTION_MARKER(0x800D5144, "GAME.OVL");
    game_initialize_object_control_alternate(object);
    flags = r_u32(object + 0x114);
    seed = r_u32(0x800EF50C);
    mixed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    w_u32(object + 0x114, flags | 0x200000);
    seed = (uint32)((sint32)mixed >> 1) ^ (mixed << 3);
    w_u32(0x800EF50C, seed);
    game_attach_object_light(object, 10, 0xBBBBBB, (mixed & 15) | 32, seed & 31);
    w_u32(object + 0x3C, 0x800A5E84);
    return 0x800A5E84;
}

void game_initialize_object_variant_51d4(uint32 object)
{
    FUNCTION_MARKER(0x800D51D4, "GAME.OVL");
    game_initialize_object_control(object);
    game_attach_initial_random_light(object, 86, 0x025E22);
    game_initialize_two_attached_effects(object, 14, 2, 250, 750, 0xFFFFFF, 0xFF4000, 0, 4, 9, 250, 750, 0xFFFFFF, 0xFF4000, 0);
}

void game_initialize_object_variant_52a8(uint32 object)
{
    FUNCTION_MARKER(0x800D52A8, "GAME.OVL");
    game_initialize_object_control(object);
    game_attach_initial_random_light(object, 86, 0x025E22);
    game_configure_object_subsystems(object, 0x800F1550);
    game_initialize_two_attached_effects(object, 14, 2, 250, 750, 0xFFFFFF, 0xFF4000, 0, 4, 9, 250, 750, 0xFFFFFF, 0xFF4000, 0);
}

void game_initialize_object_state_204(uint32 object)
{
    uint32 pointer, type, flags;
    FUNCTION_MARKER(0x800D5440, "GAME.OVL");
    w_u32(object + 0x3C, 0x800D1C3C);
    w_u32(object + 0x5C, 0x800D332C);
    w_u32(object + 0x48, 0x800D33AC);
    w_u32(object + 0x54, 0x8002493C);
    w_u32(object + 0x58, 0x8002493C);
    pointer = heap_alloc_back(324);
    type = r_u32(object + 4);
    w_u32(object + 0x204, pointer);
    w_u32(object + 0x6C, 0x800EF5B4);
    w_u16(object + 0x1C4, 0); w_u32(object + 0x1CC, 0);
    if (r_u8(0x800536C6 + type * 60) == 0x46) {
        flags = r_u32(object + 0x114);
        w_u32(object + 0x114, flags | 0x100);
    }
}

void game_initialize_object_variant_538c(uint32 object)
{
    FUNCTION_MARKER(0x800D538C, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F0384);
    game_initialize_three_attached_effects(object, 9, 118, 250, 750, 0xFFFFFF, 0xFF4000, 0, 8, 119, 250, 750, 0xFFFFFF, 0xFF4000, 0, 118, 8, 250, 750, 0xFFFFFF, 0xFF4000, 0);
}

static void game_initialize_carrier_bay_center(uint32 object, uint32 record)
{
    uint32 scale, x0, x1, y0, y1, z0, z1, x, y, z;
    scale = r_u32(object + 0x84);
    if ((sint32)scale > 0) {
        x0 = (uint32)r_s16(record + 12); x1 = (uint32)r_s16(record + 20);
        y1 = (uint32)r_s16(record + 22);
        x = (uint32)((sint32)(x0 + x1) >> 1);
        y0 = (uint32)r_s16(record + 14);
        w_u32(record + 36, x << (scale & 31));
        z0 = (uint32)r_s16(record + 16); z1 = (uint32)r_s16(record + 24);
        y = (uint32)((sint32)(y0 + y1) >> 1);
        scale = r_u32(object + 0x84);
        z = z0 + z1;
        w_u32(record + 40, y << (scale & 31));
        scale = r_u32(object + 0x84);
        z = (uint32)((sint32)z >> 1);
        w_u32(record + 44, z << (scale & 31));
    } else {
        x0 = (uint32)r_s16(record + 12); x1 = (uint32)r_s16(record + 20);
        y0 = (uint32)r_s16(record + 14);
        x = (uint32)((sint32)(x0 + x1) >> 1);
        if ((sint32)scale < 0) scale = 0 - scale;
        y1 = (uint32)r_s16(record + 22);
        w_u32(record + 36, (uint32)((sint32)x >> (scale & 31)));
        scale = r_u32(object + 0x84);
        z0 = (uint32)r_s16(record + 16);
        y = (uint32)((sint32)(y0 + y1) >> 1);
        if ((sint32)scale < 0) scale = 0 - scale;
        z1 = (uint32)r_s16(record + 24);
        w_u32(record + 40, (uint32)((sint32)y >> (scale & 31)));
        scale = r_u32(object + 0x84);
        z = (uint32)((sint32)(z0 + z1) >> 1);
        if ((sint32)scale < 0) scale = 0 - scale;
        w_u32(record + 44, (uint32)((sint32)z >> (scale & 31)));
    }
}

static void game_initialize_carrier_bay_geometry_body(uint32 object, uint32 configuration, const uint32 *native_configuration, int native_input)
{
    uint32 metadata, type, scale, radius, count, first_parameter, second_parameter, model, vertices;
    uint32 record, index, corner, axis, vertex, coordinate, far_distance, center, product, left, right;
    uint32 normal_components[3];
    VECTOR first, second, normal;
    type = r_u32(object + 0x74);
    metadata = r_u32(0x80046178) + type * 132;
    scale = r_u32(object + 0x84);
    radius = r_u32(metadata + 108);
    if ((sint32)scale > 0) radius <<= scale & 31;
    else {
        if ((sint32)scale < 0) scale = 0 - scale;
        radius = (uint32)((sint32)radius >> (scale & 31));
    }
    count = native_input ? *native_configuration++ : r_u32(configuration);
    if (!native_input) configuration += 4;
    first_parameter = native_input ? *native_configuration++ : r_u32(configuration);
    if (!native_input) configuration += 4;
    second_parameter = native_input ? *native_configuration++ : r_u32(configuration);
    if (!native_input) configuration += 4;
    model = r_u32(object + 0x64);
    vertices = r_u32(model);
    w_u16(object + 0x1C4, count); w_u16(object + 0x1C6, first_parameter);
    w_u16(object + 0x1C8, second_parameter);
    record = heap_alloc_back(count * 124 + 4);
    w_u32(object + 0x1CC, record);
    if ((sint32)count <= 0) return;
    far_distance = radius + 6000;
    for (index = 0; (sint32)index < (sint32)count; ++index) {
        w_u16(record, 0); w_u16(record + 2, 4);
        for (corner = 0; corner < 4; ++corner) {
            for (axis = 0; axis < 3; ++axis) {
                vertex = native_input ? *native_configuration : r_u32(configuration);
                coordinate = r_u16(vertices + (vertex << 3) + axis * 2);
                w_u16(record + 4 + corner * 8 + axis * 2, coordinate);
            }
            if (native_input) ++native_configuration;
            else configuration += 4;
        }
        game_initialize_carrier_bay_center(object, record);
        left = (uint32)r_s16(record + 4); right = (uint32)r_s16(record + 12);
        first.vx = (sint32)(left - right);
        left = (uint32)r_s16(record + 6); right = (uint32)r_s16(record + 14);
        first.vy = (sint32)(left - right);
        left = (uint32)r_s16(record + 8); right = (uint32)r_s16(record + 16);
        first.vz = (sint32)(left - right);
        VectorNormal(&first, &first);
        left = (uint32)r_s16(record + 4); right = (uint32)r_s16(record + 20);
        second.vx = (sint32)(left - right);
        left = (uint32)r_s16(record + 6); right = (uint32)r_s16(record + 22);
        second.vy = (sint32)(left - right);
        left = (uint32)r_s16(record + 8); right = (uint32)r_s16(record + 24);
        second.vz = (sint32)(left - right);
        VectorNormal(&second, &second);
        OuterProduct12(&first, &second, &normal);
        VectorNormal(&normal, &normal);
        normal_components[0] = (uint32)normal.vx;
        normal_components[1] = (uint32)normal.vy;
        normal_components[2] = (uint32)normal.vz;
        for (axis = 0; axis < 3; ++axis) w_u32(record + 84 + axis * 4, 0 - normal_components[axis]);
        for (axis = 0; axis < 3; ++axis) {
            product = normal_components[axis] * 4000;
            center = r_u32(record + 36 + axis * 4);
            w_u32(record + 52 + axis * 4, center + (uint32)((sint32)product >> 12));
        }
        for (axis = 0; axis < 3; ++axis) {
            product = normal_components[axis] * 24000;
            center = r_u32(record + 36 + axis * 4);
            w_u32(record + 68 + axis * 4, center - (uint32)((sint32)product >> 12));
        }
        for (axis = 0; axis < 3; ++axis) {
            product = normal_components[axis] * far_distance;
            center = r_u32(record + 36 + axis * 4);
            w_u32(record + 100 + axis * 4, center - (uint32)((sint32)product >> 12));
        }
        record += 124;
    }
}

void game_initialize_carrier_bay_geometry(uint32 object, uint32 configuration)
{
    FUNCTION_MARKER(0x800D54F4, "GAME.OVL");
    game_initialize_carrier_bay_geometry_body(object, configuration, NULL, 0);
}

void game_initialize_carrier_bay_geometry_native(uint32 object, const uint32 *configuration)
{
    game_initialize_carrier_bay_geometry_body(object, 0, configuration, 1);
}

void game_add_attack_run(uint32 object, uint32 first_vertex, uint32 last_vertex, uint32 number)
{
    uint32 type, model, vertices, vertex_count, index, offset, scale, first, last, pointer, coordinate, axis;
    FUNCTION_MARKER(0x800D7FB4, "GAME.OVL");
    type = r_u32(object + 0x74);
    model = r_u32(r_u32(0x80046178) + type * 132);
    vertices = r_u32(model); vertex_count = r_u32(model + 4);
    if ((sint32)first_vertex < 0 || (sint32)vertex_count < (sint32)(first_vertex + 1))
        diagnostic_report(r_u32(object + 4), 0x80076398, 0);
    if ((sint32)last_vertex < 0 || (sint32)vertex_count < (sint32)(last_vertex + 1))
        diagnostic_report(r_u32(object + 4), 0x800763B8, 0);
    index = number - 1;
    if (index >= 10) diagnostic_report(r_u32(object + 4), 0x800763D8, 0);
    scale = r_u32(object + 0x84);
    offset = index << 4;
    first = vertices + (first_vertex << 3); last = vertices + (last_vertex << 3);
    if ((sint32)scale > 0) {
        pointer = r_u32(object + 0x204); coordinate = (uint32)r_s16(first);
        w_u32(pointer + offset, coordinate << (scale & 31));
        pointer = r_u32(object + 0x204); coordinate = (uint32)r_s16(first + 2);
        scale = r_u32(object + 0x84);
        w_u32(pointer + offset + 4, coordinate << (scale & 31));
        coordinate = (uint32)r_s16(first + 4);
        pointer = r_u32(object + 0x204); scale = r_u32(object + 0x84);
        w_u32(pointer + offset + 8, coordinate << (scale & 31));
        for (axis = 0; axis < 3; ++axis) {
            pointer = r_u32(object + 0x204); coordinate = (uint32)r_s16(last + axis * 2);
            scale = r_u32(object + 0x84);
            w_u32(pointer + offset + 160 + axis * 4, coordinate << (scale & 31));
        }
    } else {
        scale = 0 - scale;
        pointer = r_u32(object + 0x204); coordinate = (uint32)r_s16(first);
        w_u32(pointer + offset, (uint32)((sint32)coordinate >> (scale & 31)));
        pointer = r_u32(object + 0x204); scale = r_u32(object + 0x84);
        coordinate = (uint32)r_s16(first + 2);
        scale = 0 - scale;
        w_u32(pointer + offset + 4, (uint32)((sint32)coordinate >> (scale & 31)));
        coordinate = (uint32)r_s16(first + 4);
        pointer = r_u32(object + 0x204); scale = r_u32(object + 0x84);
        scale = 0 - scale;
        w_u32(pointer + offset + 8, (uint32)((sint32)coordinate >> (scale & 31)));
        for (axis = 0; axis < 3; ++axis) {
            pointer = r_u32(object + 0x204); scale = r_u32(object + 0x84);
            coordinate = (uint32)r_s16(last + axis * 2);
            scale = 0 - scale;
            w_u32(pointer + offset + 160 + axis * 4, (uint32)((sint32)coordinate >> (scale & 31)));
        }
    }
}

uint32 game_create_linked_object_part(uint32 parent, uint32 previous, uint32 index)
{
    uint32 type, model, part, callback, value, identifier, counter;
    FUNCTION_MARKER(0x800DFA78, "GAME.OVL");
    type = r_u32(parent + 4);
    model = (uint32)r_s16(0x800536B8 + type * 60) + index + 3;
    part = game_create_object(model, 0, 0, 0, 6);
    w_u32(part + 0x54, 0x8002493C); w_u32(part + 0x58, 0x8002493C);
    callback = r_u32(parent + 0x44);
    w_u32(part + 0x3C, 0x8002D2BC); w_u32(part + 0x48, 0x800DFA38);
    w_u32(part + 0x5C, 0x800DF768); w_u32(part + 0x60, 0x8002493C);
    w_u32(part + 0x40, 0x800DF704); w_u32(part + 0x44, callback);
    matrix_copy_rotation(part + 0x11C, previous + 0x11C);
    value = r_u32(parent + 0x84); w_u32(part + 0x84, value);
    identifier = r_u16(parent + 0x24E);
    w_u32(part + 0x6C, 0); w_u32(part + 0xD8, 2000000);
    w_u16(part + 0x24E, identifier);
    value = r_u32(parent + 0x70); w_u32(part + 0x70, value);
    value = r_u32(parent + 4); w_u32(part + 4, value);
    value = r_u32(parent + 0xA0); w_u32(part + 0xA0, value);
    value = r_u32(parent + 0x114); w_u32(part + 0x114, value | 0x2480);
    value = r_u32(parent + 0x238); w_u32(part + 0x238, value);
    value = r_u32(parent + 0x234);
    w_u32(part + 0x1E4, 0x800EEC5C); w_u32(part + 0x234, value);
    game_attach_linked_part_effect(previous, part, index);
    w_u32(part + 0x160, previous); w_u32(previous + 0x168, part);
    type = r_u32(part);
    w_u32(part + 0x170, parent); w_u32(part + 0x9C, 1);
    counter = type == 3 ? 0x800595B0 : type == 1 ? 0x800595B4 : 0;
    if (counter != 0) w_u32(counter, r_u32(counter) + 1);
    return part;
}

void game_create_two_linked_object_parts(uint32 parent)
{
    uint32 previous = parent, part, pointer, index, run;
    static const uint32 runs[10][2] = {{2, 3}, {6, 7}, {4, 5}, {0, 1}, {1, 4}, {6, 3}, {2, 3}, {4, 5}, {0, 1}, {6, 7}};
    FUNCTION_MARKER(0x800DFC24, "GAME.OVL");
    for (index = 0; index < 2; ++index) {
        part = game_create_linked_object_part(parent, previous, index);
        w_u32(part + 0x1E8, 0); w_u32(part + 0x1EC, 0);
        w_u32(part + 0x1F0, 0); w_u32(part + 0x1F4, 0);
        pointer = heap_alloc_back(324);
        w_u32(part + 0x204, pointer);
        for (run = 0; run < 10; ++run)
            game_add_attack_run(part, runs[run][0], runs[run][1], run + 1);
        previous = part;
    }
}

void game_initialize_object_variant_5a90(uint32 object)
{
    uint32 run;
    static const uint32 runs[10][2] = {{93, 122}, {120, 123}, {68, 69}, {126, 3}, {0, 1}, {34, 35}, {66, 75}, {93, 97}, {2, 126}, {117, 118}};
    FUNCTION_MARKER(0x800D5A90, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F0384);
    game_configure_object_subsystems(object, 0x800EFBA8);
    w_u32(object + 0x60, 0x800EC484);
    game_create_two_linked_object_parts(object);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_three_attached_effects(object, 77, 80, 550, 2500, 0xFFFFFF, 0x20C0FF, 750, 107, 78, 550, 2500, 0xFFFFFF, 0x20C0FF, 750, 107, 80, 1350, 3750, 0xFFFFFF, 0x20C0FF, 750);
}

void game_initialize_object_variant_5c3c(uint32 object)
{
    uint32 seed, mixed, run;
    static const uint32 runs[10][2] = {{95, 102}, {96, 97}, {95, 98}, {85, 86}, {100, 105}, {94, 106}, {32, 8}, {15, 45}, {65, 57}, {51, 60}};
    FUNCTION_MARKER(0x800D5C3C, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F03A4);
    game_configure_object_subsystems(object, 0x800EFC04);
    seed = r_u32(0x800EF50C);
    w_u32(object + 0x60, 0x800EC484);
    mixed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    seed = (uint32)((sint32)mixed >> 1) ^ (mixed << 3);
    w_u32(0x800EF50C, seed);
    game_attach_object_light(object, 5, 0xFFFFFF, (mixed & 15) | 32, seed & 31);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_one_attached_effect(object, 95, 106, 600, 2000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_5dd8(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{105, 91}, {104, 103}, {90, 89}, {38, 39}, {88, 106}, {45, 44}, {22, 24}, {58, 59}, {84, 85}, {106, 104}};
    FUNCTION_MARKER(0x800D5DD8, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076014 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F03C4);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFAF0);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_one_attached_effect(object, 77, 80, 600, 2000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_5f8c(uint32 object)
{
    uint32 run;
    static const uint32 runs[10][2] = {{31, 30}, {28, 7}, {36, 117}, {17, 11}, {8, 14}, {3, 10}, {4, 3}, {56, 119}, {43, 0}, {100, 46}};
    FUNCTION_MARKER(0x800D5F8C, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F03E4);
    game_configure_object_subsystems(object, 0x800EFB4C);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_three_attached_effects(object, 62, 59, 1500, 5000, 0xFFFFFF, 0x20C0FF, 1500, 60, 64, 1500, 5000, 0xFFFFFF, 0x20C0FF, 1500, 63, 68, 3500, 7500, 0xFFFFFF, 0x20C0FF, 1500);
}

void game_initialize_object_variant_6130(uint32 object)
{
    uint32 run;
    static const uint32 runs[10][2] = {{19, 42}, {148, 22}, {148, 42}, {117, 43}, {29, 21}, {46, 48}, {136, 134}, {144, 145}, {53, 76}, {58, 78}};
    FUNCTION_MARKER(0x800D6130, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x60, 0x800EC484);
    game_configure_object_subsystems(object, 0x800EFCBC);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_one_attached_effect(object, 134, 145, 1500, 5000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_6274(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{121, 124}, {113, 117}, {54, 55}, {54, 52}, {44, 45}, {21, 28}, {15, 26}, {24, 28}, {25, 29}, {175, 25}};
    FUNCTION_MARKER(0x800D6274, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076050 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F0404);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    w_u32(object + 0x60, 0x800EC484);
    game_configure_object_subsystems(object, 0x800EFD18);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_two_attached_effects(object, 179, 186, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0, 161, 168, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_6450(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{111, 112}, {111, 122}, {103, 104}, {16, 19}, {24, 25}, {83, 108}, {155, 150}, {175, 169}, {171, 164}, {147, 157}};
    FUNCTION_MARKER(0x800D6450, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x8007608C + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F0424);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF644);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_two_attached_effects(object, 166, 177, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0, 148, 159, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_662c(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{22, 43}, {43, 63}, {43, 62}, {72, 73}, {107, 121}, {122, 3}, {70, 71}, {154, 151}, {147, 163}, {220, 212}};
    FUNCTION_MARKER(0x800D662C, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x800760C8 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F0444);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF6A0);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_three_attached_effects(object, 175, 186, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0, 193, 204, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0, 211, 222, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_682c(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{28, 25}, {125, 118}, {31, 29}, {175, 65}, {198, 199}, {198, 151}, {0, 4}, {10, 7}, {130, 115}, {207, 191}};
    FUNCTION_MARKER(0x800D682C, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076104 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F0464);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF6FC);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_four_attached_effects(object, 283, 294, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0, 247, 258, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0, 301, 312, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0, 265, 276, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_6a50(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{93, 112}, {93, 96}, {14, 155}, {118, 22}, {139, 26}, {23, 98}, {76, 57}, {147, 21}, {50, 62}, {67, 47}};
    FUNCTION_MARKER(0x800D6A50, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076140 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F0484);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF5E8);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_two_attached_effects(object, 133, 140, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0, 151, 158, 7500, 25000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_6c2c(uint32 object)
{
    uint32 configuration[11], word, run;
    static const uint32 runs[10][2] = {{86, 88}, {85, 96}, {75, 76}, {40, 45}, {63, 19}, {15, 64}, {25, 16}, {2, 14}, {6, 20}, {0, 25}};
    FUNCTION_MARKER(0x800D6C2C, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x8007617C + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F04A4);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_one_attached_effect(object, 85, 96, 500, 1500, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_6dc4(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{37, 48}, {9, 89}, {158, 169}, {158, 166}, {28, 95}, {227, 218}, {223, 230}, {179, 199}, {22, 48}, {89, 41}};
    FUNCTION_MARKER(0x800D6DC4, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x800761A8 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F04C4);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFFA0);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_three_attached_effects(object, 246, 239, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0, 207, 214, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0, 231, 220, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_6fc4(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{224, 233}, {117, 23}, {25, 20}, {54, 90}, {203, 217}, {128, 129}, {174, 172}, {153, 157}, {105, 187}, {26, 27}};
    FUNCTION_MARKER(0x800D6FC4, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x800761E4 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F04C4);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFFFC);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_four_attached_effects(object, 284, 291, 30000, 100000, 0xFFFFFF, 0x20C0FF, 0, 297, 308, 30000, 100000, 0xFFFFFF, 0x20C0FF, 0, 275, 268, 30000, 100000, 0xFFFFFF, 0x20C0FF, 0, 249, 260, 30000, 100000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_71ec(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{164, 165}, {142, 143}, {78, 80}, {163, 121}, {68, 66}, {143, 145}, {88, 165}, {119, 126}, {78, 51}, {112, 113}};
    FUNCTION_MARKER(0x800D71EC, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076220 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F04DC);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFDD0);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_two_attached_effects(object, 325, 336, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0, 307, 318, 15000, 50000, 0xFFFFFF, 0x20C0FF, 0);
}

void game_initialize_object_variant_73c8(uint32 object)
{
    uint32 configuration[11], word, run, flags;
    static const uint32 runs[10][2] = {{162, 158}, {128, 119}, {96, 95}, {297, 290}, {190, 177}, {294, 295}, {282, 40}, {117, 123}, {165, 168}, {107, 177}};
    FUNCTION_MARKER(0x800D73C8, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x8007625C + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F04FC);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF9DC);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}

void game_initialize_object_variant_7554(uint32 object)
{
    uint32 configuration[11], word, run, flags;
    static const uint32 runs[10][2] = {{162, 158}, {167, 171}, {109, 115}, {282, 40}, {123, 117}, {155, 125}, {188, 179}, {290, 291}, {292, 293}, {40, 49}};
    FUNCTION_MARKER(0x800D7554, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x8007625C + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F04FC);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFA38);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}

void game_initialize_object_variant_76e0(uint32 object)
{
    uint32 configuration[23], word, run, flags;
    static const uint32 runs[10][2] = {{191, 47}, {54, 50}, {29, 28}, {179, 17}, {233, 0}, {188, 96}, {2, 97}, {155, 89}, {234, 17}, {127, 186}};
    FUNCTION_MARKER(0x800D76E0, "GAME.OVL");
    for (word = 0; word < 23; ++word) configuration[word] = r_u32(0x80076288 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F051C);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF924);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}

void game_initialize_object_variant_786c(uint32 object)
{
    uint32 configuration[23], word, run, flags;
    static const uint32 runs[10][2] = {{53, 47}, {55, 51}, {80, 74}, {335, 108}, {140, 24}, {22, 28}, {36, 41}, {167, 48}, {170, 45}, {236, 75}};
    FUNCTION_MARKER(0x800D786C, "GAME.OVL");
    for (word = 0; word < 23; ++word) configuration[word] = r_u32(0x80076288 + word * 4);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x04000000);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F051C);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF980);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}

void game_initialize_object_variant_79f8(uint32 object)
{
    uint32 configuration[15], word, run, flags;
    static const uint32 runs[10][2] = {{317, 227}, {229, 230}, {117, 123}, {121, 122}, {11, 12}, {144, 254}, {213, 193}, {174, 192}, {83, 103}, {288, 303}};
    FUNCTION_MARKER(0x800D79F8, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x800762E4 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F053C);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF810);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}

void game_initialize_object_variant_7b84(uint32 object)
{
    uint32 configuration[15], word, run, flags;
    static const uint32 runs[10][2] = {{292, 35}, {25, 28}, {37, 33}, {24, 34}, {29, 294}, {36, 39}, {147, 138}, {171, 57}, {172, 176}, {205, 204}};
    FUNCTION_MARKER(0x800D7B84, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076320 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F055C);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF86C);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}

void game_initialize_object_variant_7d10(uint32 object)
{
    uint32 configuration[15], word, run, flags;
    static const uint32 runs[10][2] = {{154, 152}, {32, 33}, {34, 1}, {164, 227}, {94, 88}, {146, 145}, {142, 143}, {81, 276}, {60, 72}, {7, 0}};
    FUNCTION_MARKER(0x800D7D10, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x8007635C + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F057C);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EF8C8);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}

void game_initialize_object_variant_7e9c(uint32 object)
{
    uint32 run;
    static const uint32 runs[10][2] = {{210, 205}, {226, 227}, {50, 51}, {219, 218}, {85, 81}, {112, 113}, {197, 199}, {118, 98}, {107, 109}, {84, 82}};
    FUNCTION_MARKER(0x800D7E9C, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F059C);
    game_configure_object_subsystems(object, 0x800EFA94);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}

void game_initialize_object_variant_8210(uint32 object)
{
    uint32 configuration[11], word, run, seed, mixed;
    static const uint32 runs[10][2] = {{93, 94}, {20, 22}, {42, 70}, {69, 41}, {120, 117}, {119, 125}, {118, 37}, {2, 5}, {47, 53}, {73, 89}};
    FUNCTION_MARKER(0x800D8210, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x800763F4 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F03A4);
    game_configure_object_subsystems(object, 0x800F02DC);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    seed = r_u32(0x800EF50C);
    w_u32(object + 0x60, 0x800EC484);
    mixed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    seed = (uint32)((sint32)mixed >> 1) ^ (mixed << 3);
    w_u32(0x800EF50C, seed);
    game_attach_object_light(object, 5, 0x025E22, (mixed & 15) | 32, seed & 31);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_one_attached_effect(object, 81, 85, 750, 2500, 0xFFFFFF, 0xFF8080, 0);
}

void game_initialize_object_variant_8410(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{38, 39}, {89, 90}, {103, 105}, {106, 88}, {62, 64}, {84, 85}, {59, 60}, {26, 28}, {0, 3}, {100, 105}};
    FUNCTION_MARKER(0x800D8410, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076420 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F03C4);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800F01C8);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_one_attached_effect(object, 79, 83, 3750, 12500, 0xFFFFFF, 0xFF8080, 0);
}

void game_initialize_object_variant_85c4(uint32 object)
{
    uint32 run;
    static const uint32 runs[10][2] = {{31, 30}, {28, 7}, {36, 117}, {17, 11}, {8, 14}, {3, 10}, {4, 3}, {56, 119}, {43, 0}, {100, 46}};
    FUNCTION_MARKER(0x800D85C4, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F03E4);
    game_configure_object_subsystems(object, 0x800F0224);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_three_attached_effects(object, 62, 59, 1500, 5000, 0xFFFFFF, 0xFF8080, 1500, 60, 64, 1500, 5000, 0xFFFFFF, 0xFF8080, 1500, 63, 68, 3500, 7500, 0xFFFFFF, 0xFF8080, 1500);
}

void game_initialize_object_variant_8768(uint32 object)
{
    uint32 configuration[11], word, run;
    static const uint32 runs[10][2] = {{202, 200}, {33, 121}, {18, 118}, {241, 201}, {200, 221}, {134, 137}, {132, 136}, {145, 142}, {212, 214}, {58, 68}};
    FUNCTION_MARKER(0x800D8768, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x8007645C + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F05BC);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800F0338);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_two_attached_effects(object, 247, 249, 1200, 4000, 0xFFFFFF, 0xFF8080, 0, 251, 259, 1200, 4000, 0xFFFFFF, 0xFF8080, 0);
}

void game_update_attached_model_object(uint32 object)
{
    uint32 parent, callback, value;
    FUNCTION_MARKER(0x800D8B64, "GAME.OVL");
    parent = r_u32(object + 0x170);
    if (r_u32(parent) == 0) {
        callback = r_u32(object + 0x48);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
        return;
    }
    game_position_ship_at_carrier_offset(parent, object + 0x214, object);
    matrix_copy_rotation(object + 0x11C, parent + 0x11C);
    value = r_u32(object + 0xA8);
    w_u32(object + 0x20, value);
}

void game_handle_attached_model_hit_native(uint32 object, VECTOR *position, VECTOR *vector, uint32 other, uint32 projectile, uint32 kind, uint32 profile)
{
    uint32 type, parent, value;
    FUNCTION_MARKER(0x800D8BE4, "GAME.OVL");
    type = r_u32(object); parent = r_u32(object + 0x170);
    if (type == 0) return;
    if (r_u32(parent) == 0) return;
    value = r_u32(object + 0x1A8); w_u32(parent + 0x1A8, value);
    value = r_u16(object + 0x1AC); w_u16(parent + 0x1AC, value);
    game_handle_ship_hit_native(parent, position, vector, other, projectile, kind, profile);
}

void game_initialize_object_variant_8944(uint32 object)
{
    uint32 run, x, y, z, model, child, generation, scale, first_shift, second_shift;
    static const uint32 runs[10][2] = {{31, 28}, {44, 45}, {42, 43}, {30, 16}, {6, 13}, {159, 138}, {151, 146}, {46, 47}, {169, 155}, {3, 0}};
    FUNCTION_MARKER(0x800D8944, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F05DC);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    x = r_u32(object + 8); y = r_u32(object + 12);
    model = r_u32(0x800EDDD0); z = r_u32(object + 16);
    child = game_create_object(model + 3, x, y, z, 6);
    w_u32(child + 0x40, 0x800D8B64); w_u32(child + 0x44, 0x800D8BE4);
    w_u32(child, 6); w_u32(child + 0x3C, 0x8002D2BC);
    w_u32(child + 0x170, object);
    generation = r_u16(object + 0x24A); w_u16(child + 0x174, generation);
    scale = r_u32(object + 0x84);
    w_u32(child + 0x58, 0x8002493C); w_u32(child + 0x1E4, 0x800EEC5C);
    w_u32(child + 0x114, 0); w_u32(child + 0x214, 0); w_u32(child + 0xA8, 0);
    w_u32(child + 0x84, scale);
    first_shift = r_u32(child + 0x84); second_shift = r_u32(child + 0x84);
    if ((sint32)first_shift < 0) first_shift = 0 - first_shift;
    if ((sint32)second_shift < 0) second_shift = 0 - second_shift;
    w_u32(child + 0x218, (uint32)-7850 << (first_shift & 31));
    w_u32(child + 0x21C, (uint32)-8300 << (second_shift & 31));
    game_initialize_two_attached_effects(object, 130, 132, 4500, 15000, 0xFFFFFF, 0xFF8080, 0, 134, 136, 4500, 15000, 0xFFFFFF, 0xFF8080, 0);
}

void game_initialize_object_variant_8c50(uint32 object)
{
    uint32 configuration[11], word, run;
    static const uint32 runs[10][2] = {{26, 24}, {24, 27}, {28, 29}, {94, 95}, {143, 20}, {22, 123}, {154, 156}, {7, 3}, {91, 7}, {165, 80}};
    FUNCTION_MARKER(0x800D8C50, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x80076488 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F05FC);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFE8C);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_two_attached_effects(object, 74, 76, 15000, 50000, 0xFFFFFF, 0xFF8080, 0, 67, 69, 15000, 50000, 0xFFFFFF, 0xFF8080, 0);
}

void game_initialize_object_variant_8e2c(uint32 object)
{
    uint32 configuration[11], word, run;
    static const uint32 runs[10][2] = {{36, 37}, {11, 9}, {12, 13}, {212, 213}, {37, 217}, {241, 242}, {218, 219}, {240, 241}, {243, 252}, {206, 26}};
    FUNCTION_MARKER(0x800D8E2C, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x800764B4 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F061C);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFEE8);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_three_attached_effects(object, 158, 164, 22500, 75000, 0xFFFFFF, 0xFF8080, 0, 160, 163, 22500, 75000, 0xFFFFFF, 0xFF8080, 0, 164, 161, 22500, 75000, 0xFFFFFF, 0xFF8080, 0);
}

void game_initialize_object_variant_9030(uint32 object)
{
    uint32 configuration[19], word, run;
    static const uint32 runs[10][2] = {{145, 142}, {136, 132}, {187, 139}, {18, 129}, {202, 200}, {201, 226}, {159, 161}, {67, 68}, {241, 129}, {11, 10}};
    FUNCTION_MARKER(0x800D9030, "GAME.OVL");
    for (word = 0; word < 19; ++word) configuration[word] = r_u32(0x800764E0 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F063C);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFF44);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_three_attached_effects(object, 106, 106, 20000, 90000, 0xFFFFFF, 0xFF8080, 15000, 112, 112, 20000, 90000, 0xFFFFFF, 0xFF8080, 15000, 106, 112, 30000, 100000, 0xFFFFFF, 0xFF8080, 15000);
}

void game_initialize_object_variant_923c(uint32 object)
{
    uint32 configuration[11], word, run;
    static const uint32 runs[10][2] = {{10, 11}, {47, 65}, {45, 64}, {6, 7}, {4, 5}, {67, 50}, {48, 50}, {128, 127}, {139, 137}, {8, 144}};
    FUNCTION_MARKER(0x800D923C, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x8007652C + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F065C);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800EFE30);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_two_attached_effects(object, 85, 86, 7500, 25000, 0xFFFFFF, 0xFF8080, 0, 89, 90, 7500, 25000, 0xFFFFFF, 0xFF8080, 0);
}

void game_initialize_object_variant_9418(uint32 object)
{
    uint32 configuration[11], word, run;
    static const uint32 runs[10][2] = {{31, 37}, {30, 34}, {39, 46}, {48, 54}, {46, 45}, {18, 26}, {89, 5}, {24, 11}, {43, 44}, {29, 30}};
    FUNCTION_MARKER(0x800D9418, "GAME.OVL");
    for (word = 0; word < 11; ++word) configuration[word] = r_u32(0x80076558 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F067C);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_one_attached_effect(object, 107, 109, 1500, 5000, 0xFFFFFF, 0xFF8080, 1500);
}

void game_initialize_object_variant_95b4(uint32 object)
{
    uint32 run;
    static const uint32 runs[10][2] = {{93, 121}, {97, 122}, {2, 3}, {126, 127}, {0, 1}, {116, 115}, {32, 33}, {42, 68}, {74, 71}, {122, 123}};
    FUNCTION_MARKER(0x800D95B4, "GAME.OVL");
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F0384);
    game_configure_object_subsystems(object, 0x800F0280);
    w_u32(object + 0x60, 0x800EC484);
    game_create_two_linked_object_parts(object);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
    game_initialize_three_attached_effects(object, 76, 107, 750, 2500, 0xFFFFFF, 0xFF8080, 500, 78, 107, 750, 2500, 0xFFFFFF, 0xFF8080, 500, 107, 80, 1750, 3750, 0xFFFFFF, 0xFF8080, 500);
}

void game_initialize_object_variant_9760(uint32 object)
{
    uint32 configuration[23], word, run;
    static const uint32 runs[10][2] = {{191, 47}, {54, 50}, {29, 28}, {179, 17}, {233, 0}, {188, 96}, {2, 97}, {155, 89}, {234, 17}, {127, 186}};
    FUNCTION_MARKER(0x800D9760, "GAME.OVL");
    for (word = 0; word < 23; ++word) configuration[word] = r_u32(0x80076584 + word * 4);
    w_u32(object + 0x114, r_u32(object + 0x114) | 0x04000000);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F051C);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800F016C);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}
void game_initialize_object_variant_98ec(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{317, 227}, {229, 230}, {117, 123}, {121, 122}, {11, 12}, {144, 254}, {213, 193}, {174, 192}, {83, 103}, {288, 303}};
    FUNCTION_MARKER(0x800D98EC, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x800765E0 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F053C);
    w_u32(object + 0x114, r_u32(object + 0x114) | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800F0058);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}
void game_initialize_object_variant_9a78(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{154, 152}, {61, 99}, {142, 151}, {188, 197}, {128, 133}, {59, 98}, {90, 93}, {151, 112}, {34, 26}, {75, 81}};
    FUNCTION_MARKER(0x800D9A78, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x8007661C + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F055C);
    w_u32(object + 0x114, r_u32(object + 0x114) | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800F00B4);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}
void game_initialize_object_variant_9c04(uint32 object)
{
    uint32 configuration[15], word, run;
    static const uint32 runs[10][2] = {{154, 152}, {32, 33}, {34, 1}, {164, 227}, {94, 88}, {146, 145}, {142, 143}, {81, 276}, {60, 72}, {7, 0}};
    FUNCTION_MARKER(0x800D9C04, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076658 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F055C);
    w_u32(object + 0x114, r_u32(object + 0x114) | 0x04000000);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    game_configure_object_subsystems(object, 0x800F0110);
    w_u32(object + 0x60, 0x800EC484);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}
uint32 game_reset_target_tracking_cache(uint32 object)
{
    uint32 first, state, index;
    static const uint32 offsets[17] = {0x16C, 0x198, 0x1C4, 0x21C, 0x248, 0x274, 0x2A0, 0x1F0, 0x148, 0x174, 0x1A0, 0x1CC, 0x224, 0x250, 0x27C, 0x2A8, 0x1F8};
    FUNCTION_MARKER(0x800DB220, "GAME.OVL");
    first = r_u32(object + 0x110);
    w_u16(object + 0x25C, 5000);
    w_u16(object + 0x25E, 5000);
    w_u32(object + 0x230, 5000000);
    w_u32(first + 0x140, 0);
    for (index = 0; index < 17; ++index) {
        state = r_u32(object + 0x110);
        w_u32(state + offsets[index], 0);
    }
    return state;
}
void game_begin_target_tracking(uint32 object)
{
    uint32 saved, target, generation, kind, value, mode, callback;
    FUNCTION_MARKER(0x800DA210, "GAME.OVL");
    saved = r_u32(object + 0x180);
    target = r_u32(object + 0x178);
    if (saved == target) {
        generation = r_u16(object + 0x184);
        if (generation == r_u16(saved + 0x24A)) goto reset_control;
    }
    w_u16(r_u32(object + 0x22C) + 0xC, 0);
    w_u16(r_u32(object + 0x22C) + 0xE, 0);
    w_u16(r_u32(object + 0x22C) + 0x10, 0);
    w_u8(r_u32(object + 0x200), 0);
    w_u16(object + 0x25C, 0x7F8);
    w_u16(object + 0x25E, 0x7F8);
    game_reset_target_tracking_cache(object);
reset_control:
    w_u16(r_u32(object + 0x22C) + 0xA, 0);
    w_u32(r_u32(object + 0x22C), 0);
    w_u16(r_u32(object + 0x22C) + 8, 0);
    kind = r_u32(object + 4);
    w_u32(object + 0x88, 0);
    w_u32(object + 0x4C, 0x800DA640);
    value = r_u32(0x8005369C + kind * 60);
    kind = r_u32(object + 4);
    w_u32(object + 0xE8, value);
    value = r_u16(0x800536B4 + kind * 60);
    mode = r_u32(0x800EE9D8);
    w_u16(object + 0x266, 0);
    w_u16(object + 0x260, value);
    if (mode == 1) {
        w_u16(object + 0x266, 1);
    } else if ((r_u16(object + 0x264) & 1) != 0 && random_between(0, 100) < 35) {
        target = r_u32(object + 0x178);
        if (target == r_u32(0x80040FDC)) goto player_target;
        w_u16(object + 0x266, r_u16(object + 0x266) | 1);
    }
    target = r_u32(object + 0x178);
    if (target == r_u32(0x80040FDC)) {
player_target:
        w_u32(object + 0x58, 0x800D9D90);
        w_u32(object + 0x40, 0x800DBE20);
    } else if ((r_u32(target + 0x114) & 0x10) != 0) {
        w_u32(object + 0x58, 0x800DAFDC);
        w_u32(object + 0x40, 0x800DB588);
    } else {
        w_u32(object + 0x58, 0x800D9D90);
        w_u32(object + 0x40, 0x800DA8BC);
    }
    callback = r_u32(object + 0x40);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}
void game_start_target_tracking_maneuver(uint32 object)
{
    uint32 callback;
    FUNCTION_MARKER(0x800DA870, "GAME.OVL");
    w_u32(object + 0x88, 1);
    game_start_ai_mode_maneuver(object, 0x800DA640, 60);
    callback = r_u32(object + 0x40);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}
void game_update_target_collision_avoidance(uint32 object)
{
    uint32 target, active, x, y, z, vx, vy, vz, denominator, dot, numerator, time, result, value, first, second;
    FUNCTION_MARKER(0x800DAFDC, "GAME.OVL");
    target = r_u32(object + 0x190);
    active = 0;
    if (target != r_u32(object + 0x178)) {
        game_update_target_maneuver(object);
        return;
    }
    if ((r_u32(target + 0x114) & 0x10) != 0) goto clear_avoidance;
    game_evaluate_target_occlusion(object, target, 0);
    value = r_u16(object + 0x24A);
    if (((r_u32(0x800311B8) + value) & 3) != 0) goto check_distance;
    value = r_u32(0x80033D18);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value >= 1024) goto check_distance;
    value = r_u32(0x80033D20);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value >= 1024) goto check_distance;
    first = r_u32(object + 0xFC); second = r_u32(target + 0xFC); vx = first - second;
    first = r_u32(object + 0x100); second = r_u32(target + 0x100); vy = first - second;
    first = r_u32(object + 0x104); second = r_u32(target + 0x104); vz = first - second;
    denominator = vx * vx + vy * vy + vz * vz;
    if (denominator == 0) goto check_distance;
    first = r_u32(object + 8); second = r_u32(target + 8); x = first - second;
    first = r_u32(object + 12); second = r_u32(target + 12); y = first - second;
    first = r_u32(object + 16); second = r_u32(target + 16); z = first - second;
    dot = x * vx + y * vy + z * vz;
    numerator = 0 - dot;
    if (denominator == 0xFFFFFFFF && numerator == 0x80000000) trap(0x5D);
    time = (uint32)((sint32)numerator / (sint32)denominator);
    if (time >= 50) goto check_distance;
    result = time * (denominator * time + (dot << 1)) + x * x + y * y + z * z + 0xFA0A1F00;
    if ((sint32)result < 0) active = 1;
check_distance:
    if (r_s32(0x80046174) < 5000) active = 1;
    if (active != 0) {
        w_u8(r_u32(object + 0x200), 14);
        return;
    }
clear_avoidance:
    w_u32(object + 0x198, 0);
}
void game_begin_alternate_target_tracking(uint32 object)
{
    uint32 saved, target, generation, mode, callback;
    FUNCTION_MARKER(0x800DB30C, "GAME.OVL");
    w_u16(r_u32(object + 0x22C) + 0xA, 0);
    saved = r_u32(object + 0x180);
    target = r_u32(object + 0x178);
    if (saved == target) {
        generation = r_u16(object + 0x184);
        if (generation == r_u16(saved + 0x24A)) goto select_tracking;
    }
    w_u8(r_u32(object + 0x200), 0);
    w_u16(r_u32(object + 0x200) + 2, 0);
    w_u16(r_u32(object + 0x200) + 4, 0);
    w_u16(r_u32(object + 0x200) + 6, 0);
    w_u16(r_u32(object + 0x200) + 8, 0);
    w_u8(r_u32(object + 0x200) + 0xC, 1);
    w_u32(object + 0x198, 0);
    w_u16(object + 0x25C, 0x7F8);
    w_u16(object + 0x25E, 0x7F8);
    game_reset_target_tracking_cache(object);
select_tracking:
    mode = r_u32(0x800EE9D8);
    w_u16(object + 0x266, 0);
    if (mode == 1) w_u16(object + 0x266, mode);
    target = r_u32(object + 0x178);
    if (target != r_u32(0x80040FDC) && (r_u32(target + 0x114) & 0x10) != 0)
        w_u32(object + 0x40, 0x800DB588);
    else
        w_u32(object + 0x40, 0x800DBE20);
    callback = r_u32(object + 0x40);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}
uint32 game_copy_attack_run_to_control(uint32 owner, uint32 object, uint32 run)
{
    uint32 offset, source, destination, index, value, frame, saved[3];
    static const uint32 source_offsets[5] = {0, 4, 8, 0xA0, 0xA4};
    static const uint32 destination_offsets[5] = {0x3C, 0x40, 0x44, 0x4C, 0x50};
    FUNCTION_MARKER(0x800DB430, "GAME.OVL");
    offset = run << 4;
    for (index = 0; index < 5; ++index) {
        source = r_u32(owner + 0x204);
        destination = r_u32(object + 0x200);
        value = r_u32(source + offset + source_offsets[index]);
        w_u32(destination + destination_offsets[index], value);
    }
    source = r_u32(owner + 0x204);
    value = r_u32(source + offset + 0xA8);
    frame = r_u32(0x800311B8);
    destination = r_u32(object + 0x200);
    w_u32(destination + 0x54, value);
    if ((frame & 1) == 0) return 0;
    for (index = 0; index < 3; ++index) {
        destination = r_u32(object + 0x200);
        saved[index] = r_u32(destination + 0x3C + index * 4);
    }
    for (index = 0; index < 3; ++index) {
        destination = r_u32(object + 0x200);
        value = r_u32(destination + 0x4C + index * 4);
        w_u32(destination + 0x3C + index * 4, value);
    }
    for (index = 0; index < 3; ++index) {
        destination = r_u32(object + 0x200);
        value = saved[index];
        w_u32(destination + 0x4C + index * 4, value);
    }
    return value;
}
void game_release_linked_part_effects(uint32 object)
{
    uint32 index, offset;
    FUNCTION_MARKER(0x800DE70C, "GAME.OVL");
    for (index = 0; index < 32; ++index) {
        offset = index * 52;
        if (r_u32(0x801094E0 + offset) == object || r_u32(0x801094E4 + offset) == object)
            w_u8(0x801094E8 + offset, 0);
    }
}
void game_attach_linked_part_effect(uint32 previous, uint32 part, uint32 index)
{
    uint32 slot, offset, vertices, model, table, first_vertex, axis, value, second, third, fourth, scale, x, y, z, next_x, next_y, next_z, field;
    sint32 sum;
    static const uint32 fields[6] = {0x801094B8, 0x801094BC, 0x801094C0, 0x801094C8, 0x801094CC, 0x801094D0};
    FUNCTION_MARKER(0x800DE774, "GAME.OVL");
    for (slot = 0; slot < 32; ++slot)
        if (r_u8(0x801094E8 + slot * 52) == 0) break;
    if (slot == 32) {
#ifndef NDEBUG
        printf("%s", game_lifecycle_text_native(0x80076A2C));
#endif
        return;
    }
    model = r_u32(previous + 0x74);
    table = r_u32(0x80046178);
    vertices = r_u32(r_u32(table + model * 132));
    offset = slot * 52;
    w_u8(0x801094E8 + offset, 1);
    first_vertex = index == 0 ? 0x3C0 : 0x60;
    for (axis = 0; axis < 2; ++axis) {
        value = (uint32)(sint32)r_s16(vertices + first_vertex + axis * 2);
        second = (uint32)(sint32)r_s16(vertices + first_vertex + 8 + axis * 2);
        third = (uint32)(sint32)r_s16(vertices + first_vertex + 16 + axis * 2);
        fourth = (uint32)(sint32)r_s16(vertices + first_vertex + 24 + axis * 2);
        sum = (sint32)(value + second + third + fourth);
        w_u32(0x801094B8 + offset + axis * 4, (uint32)(sum >> 2));
    }
    value = (uint32)(sint32)r_s16(vertices + first_vertex + 4);
    second = (uint32)(sint32)r_s16(vertices + first_vertex + 12);
    third = (uint32)(sint32)r_s16(vertices + first_vertex + 20);
    fourth = (uint32)(sint32)r_s16(vertices + first_vertex + 28);
    w_u32(0x801094D8 + offset, index == 0 ? 4 : 1);
    w_u32(0x801094DC + offset, 1);
    sum = (sint32)(value + second + third + fourth);
    w_u32(0x801094C0 + offset, (uint32)(sum >> 2));
    if (index == 0) {
        model = r_u32(part + 0x74);
        table = r_u32(0x80046178);
        vertices = r_u32(r_u32(table + model * 132));
    }
    for (axis = 0; axis < 3; ++axis) {
        value = (uint32)(sint32)r_s16(vertices + 0x40 + axis * 2);
        second = (uint32)(sint32)r_s16(vertices + 0x48 + axis * 2);
        third = (uint32)(sint32)r_s16(vertices + 0x50 + axis * 2);
        fourth = (uint32)(sint32)r_s16(vertices + 0x58 + axis * 2);
        sum = (sint32)(value + second + third + fourth);
        w_u32(0x801094C8 + offset + axis * 4, (uint32)(sum >> 2));
    }
    scale = r_u32(previous + 0x84);
    if ((sint32)scale > 0) {
        for (field = 0; field < 6; ++field) {
            value = r_u32(fields[field] + offset);
            if (field != 0) scale = r_u32(previous + 0x84);
            w_u32(fields[field] + offset, value << (scale & 31));
        }
    } else {
        if ((sint32)scale < 0) scale = 0 - scale;
        scale &= 31;
        x = r_u32(0x801094B8 + offset);
        y = r_u32(0x801094BC + offset);
        w_u32(0x801094B8 + offset, (uint32)((sint32)x >> scale));
        z = r_u32(0x801094C0 + offset);
        w_u32(0x801094BC + offset, (uint32)((sint32)y >> scale));
        next_x = r_u32(0x801094C8 + offset);
        w_u32(0x801094C0 + offset, (uint32)((sint32)z >> scale));
        next_y = r_u32(0x801094CC + offset);
        w_u32(0x801094C8 + offset, (uint32)((sint32)next_x >> scale));
        next_z = r_u32(0x801094D0 + offset);
        w_u32(0x801094CC + offset, (uint32)((sint32)next_y >> scale));
        w_u32(0x801094D0 + offset, (uint32)((sint32)next_z >> scale));
    }
    w_u32(0x801094E0 + offset, previous);
    w_u32(0x801094E4 + offset, part);
}
static void game_aim_at_attack_run_point(uint32 object, uint32 target, uint32 field)
{
    VECTOR point, transformed;
    uint32 state, target_x, target_y, target_z, object_x, object_y, object_z, x, y, z;
    state = r_u32(object + 0x200);
    point.vx = r_s32(state + field);
    point.vy = r_s32(state + field + 4);
    point.vz = r_s32(state + field + 8);
    point.pad = r_s32(state + field + 12);
    ApplyMatrixLV((MATRIX *)psx_addr(target + 0x11C, 20), &point, &transformed);
    target_x = r_u32(target + 8);
    target_y = r_u32(target + 12);
    object_z = r_u32(object + 16);
    x = (uint32)transformed.vx + target_x;
    y = (uint32)transformed.vy + target_y;
    target_z = r_u32(target + 16);
    object_x = r_u32(object + 8);
    z = (uint32)transformed.vz + target_z;
    object_y = r_u32(object + 12);
    game_set_relative_target_angles(object, x - object_x, y - object_y, z - object_z);
}
void game_update_large_target_attack(uint32 object)
{
    uint32 target, model, scale, table, metadata, radius, parent, state, mode, distance, old_distance, threshold, kind, value, yaw, pitch, x, y, z, px, py, pz, sum, quotient;
    sint32 length, below, small;
    FUNCTION_MARKER(0x800DB588, "GAME.OVL");
    target = r_u32(object + 0x178);
    if (r_u32(target) == 0) goto reset_attack;
    model = r_u32(target + 0x74);
    scale = r_u32(target + 0x84);
    table = r_u32(0x80046178);
    metadata = r_u32(table + model * 132 + 4);
    radius = r_u32(metadata + 0x10);
    if ((sint32)scale > 0) radius <<= scale & 31;
    else radius = (uint32)((sint32)radius >> ((0 - scale) & 31));
    radius = (uint32)((sint32)(radius * 1448) >> 10);
    if (r_u32(target + 0x40) == 0x800A49A4) {
        parent = r_u32(target + 0x170);
        x = r_u32(target + 8); y = r_u32(target + 12); z = r_u32(target + 16);
        px = r_u32(parent + 8); py = r_u32(parent + 12); pz = r_u32(parent + 16);
        length = vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
        model = r_u32(parent + 0x74);
        table = r_u32(0x80046178);
        scale = r_u32(parent + 0x84);
        metadata = r_u32(table + model * 132 + 4);
        value = r_u32(metadata + 0x10);
        if ((sint32)scale > 0) value <<= scale & 31;
        else value = (uint32)((sint32)value >> ((0 - scale) & 31));
        if (length < (sint32)(radius + value)) {
            game_skip_ship_task(object, r_u32(object + 0xE0));
            goto reset_attack;
        }
    }
    game_evaluate_target_occlusion(object, target, 0);
    state = r_u32(object + 0x200);
    mode = r_u8(state);
    old_distance = r_u32(0x80046174);
    if (mode == 0) w_u8(state, 2);
    if ((sint32)radius < (sint32)old_distance) {
        state = r_u32(object + 0x200);
        if (r_u8(state) == 1) goto first_point;
        value = (uint32)random_between(0, 9);
        game_copy_attack_run_to_control(target, object, value);
        w_u8(r_u32(object + 0x200), 1);
        w_u32(r_u32(object + 0x200) + 0x9C, 0xFFFFFFFF);
    }
    if (r_u8(r_u32(object + 0x200)) != 1) goto second_mode;
first_point:
    game_aim_at_attack_run_point(object, target, 0x3C);
    distance = r_u32(0x80046174);
    if ((sint32)distance >= (sint32)old_distance) {
        w_u8(r_u32(object + 0x200), 2);
        goto second_mode;
    }
    yaw = r_u32(0x80033D1C); pitch = r_u32(0x80033D30);
    w_u16(object + 0x250, yaw); w_u16(object + 0x252, pitch);
    kind = r_u32(object + 4);
    value = r_u32(((sint32)distance <= 74999 ? 0x8005369C : 0x800536A0) + kind * 60);
    w_u32(object + 0xE8, value);
    scale = r_u32(target + 0x84);
    threshold = (sint32)scale >= 0 ? 50000u << (scale & 31) : (uint32)((sint32)50000 >> ((0 - scale) & 31));
    if (r_s32(target + 0x84) >= 4) threshold += 40000;
    value = (uint32)(sint32)r_s16(object + 0x250);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value < 128) {
        value = (uint32)(sint32)r_s16(object + 0x252);
        if ((sint32)value < 0) value = 0 - value;
        if ((sint32)value < 128 && r_s32(0x80046174) < (sint32)threshold) {
            w_u32(object + 0x28, 32);
            if (random_between(1, 3) != 1) game_update_target_weapons(object);
        }
    }
    scale = r_u32(target + 0x84);
    threshold = (sint32)scale > 0 ? 15000u << (scale & 31) : (uint32)((sint32)15000 >> ((0 - scale) & 31));
    if ((sint32)threshold > 40000) threshold = 40000;
    small = r_s32(target + 0x84) < 4;
    if (!small) threshold += 40000;
    distance = r_u32(0x80046174);
    below = (sint32)distance < (sint32)threshold;
    if (!below) {
        value = r_u32(r_u32(object + 0x200) + 0x9C);
        if ((sint32)value >= (sint32)distance || value == 0xFFFFFFFF || (sint32)distance >= (sint32)(threshold << 1)) goto save_first_distance;
    }
    if (!small && below) {
        w_u8(r_u32(object + 0x200), 3);
        w_u32(r_u32(object + 0x200) + 0x9C, 0xFFFFFFFF);
    } else w_u8(r_u32(object + 0x200), 2);
save_first_distance:
    state = r_u32(object + 0x200);
    distance = r_u32(0x80046174);
    w_u32(state + 0x9C, distance);
second_mode:
    state = r_u32(object + 0x200);
    if (r_u8(state) != 3) goto retreat_mode;
    game_aim_at_attack_run_point(object, target, 0x4C);
    yaw = r_u32(0x80033D1C); distance = r_u32(0x80046174); pitch = r_u32(0x80033D30);
    w_u16(object + 0x250, yaw); w_u16(object + 0x252, pitch);
    kind = r_u32(object + 4);
    value = r_u32(((sint32)distance <= 74999 ? 0x8005369C : 0x800536A0) + kind * 60);
    w_u32(object + 0xE8, value);
    scale = r_u32(target + 0x84);
    threshold = (sint32)scale > 0 ? 50000u << (scale & 31) : (uint32)((sint32)50000 >> ((0 - scale) & 31));
    value = (uint32)(sint32)r_s16(object + 0x250);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value < 128) {
        value = (uint32)(sint32)r_s16(object + 0x252);
        if ((sint32)value < 0) value = 0 - value;
        if ((sint32)value < 128 && r_s32(0x80046174) < (sint32)threshold) {
            w_u32(object + 0x28, 32);
            if (random_between(1, 3) != 1) game_update_target_weapons(object);
        }
    }
    scale = r_u32(target + 0x84);
    threshold = (sint32)scale > 0 ? 5000u << (scale & 31) : (uint32)((sint32)5000 >> ((0 - scale) & 31));
    if ((sint32)threshold > 40000) threshold = 40000;
    distance = r_u32(0x80046174);
    threshold += 10000;
    if ((sint32)distance >= (sint32)threshold) {
        value = r_u32(r_u32(object + 0x200) + 0x9C);
        if ((sint32)value >= (sint32)distance || value == 0xFFFFFFFF || (sint32)distance >= (sint32)(threshold << 1)) goto save_second_distance;
    }
    w_u8(r_u32(object + 0x200), 2);
save_second_distance:
    state = r_u32(object + 0x200);
    distance = r_u32(0x80046174);
    w_u32(state + 0x9C, distance);
retreat_mode:
    if (r_u8(r_u32(object + 0x200)) == 2) {
        game_evaluate_target_occlusion(object, target, 0);
        yaw = r_u32(0x80033D1C);
        if ((sint32)yaw >= -959) w_u16(object + 0x250, 0 - yaw);
        pitch = r_u32(0x80033D30); yaw = r_u32(0x80033D1C);
        w_u16(object + 0x252, 0 - pitch);
        if (yaw == 0 && pitch == 0) w_u16(object + 0x252, 1024);
        kind = r_u32(object + 4);
        value = r_u32(0x800536A0 + kind * 60);
        yaw = 0 - r_u32(0x80033D1C);
        w_u32(object + 0xE8, value);
        if ((sint32)yaw >= 1921) w_u32(object + 0x28, 48);
    }
    kind = r_u32(object + 4);
    value = r_u32(object + 0xE8);
    threshold = r_u32(0x8005369C + kind * 60);
    if ((sint32)threshold < (sint32)value || (sint32)threshold < r_s32(object + 0xEC))
        value = (uint32)(sint32)r_s16(0x800536B2 + kind * 60);
    else value = (uint32)(sint32)r_s16(0x800536B2 + kind * 60) << 1;
    w_u32(object + 0xF0, value);
    game_update_object_state(object);
    game_update_object_movement(object);
    if (r_u32(target) == 0) goto reset_attack;
    value = r_u16(object + 0x24A);
    sum = r_u32(0x800311B8) + value;
    quotient = (uint32)(((uint64)sum * 0x88888889) >> 32) >> 5;
    if (quotient * 60 != sum - 3) return;
reset_attack:
    w_u32(object + 0x40, 0x8009AB58);
}
void game_update_linked_part_constraint(uint32 first, uint32 second, uint32 first_offset, uint32 second_offset, uint32 amplitude)
{
    VECTOR points[4], offset, direction;
    SVECTOR angles;
    MATRIX rotation;
    uint32 camera, camera_x, camera_y, camera_z, value, component, width, phase, product, delta[3], normal[3], numerator, correction, field, player, pitch, yaw;
    sint32 visible, length, positive;
    FUNCTION_MARKER(0x800DECE8, "GAME.OVL");
    camera = r_u32(0x80040C94);
    value = r_u32(first + 8); camera_x = r_u32(camera + 8); value -= camera_x;
    if ((sint32)value < 0) value = 0 - value;
    visible = 1;
    if ((sint32)value > 328000) goto invisible;
    value = r_u32(first + 12); camera_y = r_u32(camera + 12); value -= camera_y;
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value > 328000) goto invisible;
    value = r_u32(first + 16); camera_z = r_u32(camera + 16); value -= camera_z;
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value > 328000) goto invisible;
    value = r_u32(second + 8) - camera_x;
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value > 328000) goto invisible;
    value = r_u32(second + 12) - camera_y;
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value > 328000) goto invisible;
    value = r_u32(second + 16) - camera_z;
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value <= 328000) goto world_points;
invisible:
    visible = 0;
world_points:
    offset.vx = r_s32(first_offset);
    player = r_u32(0x80040FDC);
    offset.vy = r_s32(first_offset + 4); offset.vz = r_s32(first_offset + 8);
    if (first == player)
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(first + 0x11C, 18), &offset, &offset);
    else ApplyMatrixLV((MATRIX *)psx_addr(first + 0x11C, 20), &offset, &offset);
    points[0].vx = (sint32)(r_u32(first + 8) + (uint32)offset.vx);
    points[0].vy = (sint32)(r_u32(first + 12) + (uint32)offset.vy);
    points[0].vz = (sint32)(r_u32(first + 16) + (uint32)offset.vz);
    offset.vx = r_s32(second_offset);
    player = r_u32(0x80040FDC);
    offset.vy = r_s32(second_offset + 4); offset.vz = r_s32(second_offset + 8);
    if (second == player)
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(second + 0x11C, 18), &offset, &offset);
    else ApplyMatrixLV((MATRIX *)psx_addr(second + 0x11C, 20), &offset, &offset);
    points[3].vx = (sint32)(r_u32(second + 8) + (uint32)offset.vx);
    points[3].vy = (sint32)(r_u32(second + 12) + (uint32)offset.vy);
    points[3].vz = (sint32)(r_u32(second + 16) + (uint32)offset.vz);
    value = r_u32(first + 0x84); component = r_u32(second + 0x84);
    width = (amplitude << (value & 31)) + (amplitude << (component & 31));
    width = (uint32)((sint32)(width + (width >> 31)) >> 1);
    if (visible) {
        delta[0] = (uint32)points[3].vx - (uint32)points[0].vx;
        delta[1] = (uint32)points[3].vy - (uint32)points[0].vy;
        delta[2] = (uint32)points[3].vz - (uint32)points[0].vz;
        phase = r_u32(0x800311B8) << 6;
        product = amplitude * (uint32)rsin((sint32)phase);
        points[1].vx = (sint32)((uint32)points[0].vx + (uint32)((sint32)(delta[0] * 20) >> 6) + (uint32)((sint32)product >> 14));
        phase = r_u32(0x800311B8) << 7;
        product = amplitude * (uint32)rcos((sint32)phase);
        points[1].vy = (sint32)((uint32)points[0].vy + (uint32)((sint32)(delta[1] * 20) >> 6) - (uint32)((sint32)product >> 14));
        phase = (0 - r_u32(0x800311B8)) << 8;
        product = amplitude * (uint32)rsin((sint32)phase);
        points[1].vz = (sint32)((uint32)points[0].vz + (uint32)((sint32)(delta[2] * 20) >> 6) + (uint32)((sint32)product >> 14));
        phase = r_u32(0x800311B8) << 8;
        product = amplitude * (uint32)rsin((sint32)phase);
        points[2].vx = (sint32)((uint32)points[0].vx + (uint32)((sint32)(delta[0] * 40) >> 6) - (uint32)((sint32)product >> 14));
        phase = r_u32(0x800311B8) << 7;
        product = amplitude * (uint32)rcos((sint32)phase);
        points[2].vy = (sint32)((uint32)points[0].vy + (uint32)((sint32)(delta[1] * 40) >> 6) + (uint32)((sint32)product >> 14));
        phase = (0 - r_u32(0x800311B8)) << 6;
        product = amplitude * (uint32)rsin((sint32)phase);
        points[2].vz = (sint32)((uint32)points[0].vz + (uint32)((sint32)(delta[2] * 40) >> 6) - (uint32)((sint32)product >> 14));
        game_draw_tractor_beam_native(points, 0);
    }
    delta[0] = (uint32)points[3].vx - (uint32)points[0].vx;
    delta[1] = (uint32)points[3].vy - (uint32)points[0].vy;
    delta[2] = (uint32)points[3].vz - (uint32)points[0].vz;
    length = vector_length_approx((sint32)delta[0], (sint32)delta[1], (sint32)delta[2]);
    if (length == 0) length = 1;
    for (field = 0; field < 3; ++field) {
        numerator = delta[field] << 12;
        if (length == -1 && numerator == 0x80000000) trap(0x5D);
        normal[field] = (uint32)((sint32)numerator / length);
    }
    correction = (uint32)((sint32)((uint32)length - width) >> 1);
    positive = (sint32)correction > 0;
    if (correction != 0) {
        if ((sint32)correction < 0) correction = 0 - correction;
        for (field = 0; field < 3; ++field) {
            product = normal[field] * correction;
            value = r_u32(second + 0x14 + field * 4);
            component = (uint32)((sint32)product >> 13);
            w_u32(second + 0x14 + field * 4, positive ? value - component : value + component);
        }
        for (field = 0; field < 3; ++field) {
            product = normal[field] * correction;
            value = r_u32(first + 0x14 + field * 4);
            component = (uint32)((sint32)product >> 13);
            w_u32(first + 0x14 + field * 4, positive ? value + component : value - component);
        }
    }
    direction.vx = (sint32)normal[0]; direction.vy = (sint32)normal[1]; direction.vz = (sint32)normal[2];
    player = r_u32(0x80040FDC);
    if (first == player) ApplyMatrixLV((MATRIX *)psx_addr(first + 0x11C, 20), &direction, &direction);
    else game_apply_transposed_matrix_native((const MATRIX *)psx_addr(first + 0x11C, 18), &direction, &direction);
    yaw = (uint32)ratan2(direction.vx, direction.vz);
    value = (uint32)direction.vz;
    if ((sint32)value < 0) value = 0 - value;
    pitch = (uint32)ratan2((sint32)(0 - (uint32)direction.vy), (sint32)value);
    angles.vx = (sint16)((sint32)pitch >> 1); angles.vy = (sint16)((sint32)yaw >> 1); angles.vz = 0;
    RotMatrixYXZ(&angles, &rotation);
    player = r_u32(0x80040FDC);
    if (first != player)
        MulMatrix0((MATRIX *)psx_addr(first + 0x11C, 20), &rotation, (MATRIX *)psx_addr(first + 0x11C, 20));
    else MulMatrix0(&rotation, (MATRIX *)psx_addr(first + 0x11C, 20), (MATRIX *)psx_addr(first + 0x11C, 20));
    matrix_normalize(first + 0x11C, 1);
}
