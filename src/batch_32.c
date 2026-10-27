#include "game_functions.h"
#include <stdio.h>
#include <string.h>

uint32 game_script_increment_data_count(void)
{
    uint32 count = r_u32(0x80059190);
    FUNCTION_MARKER(0x80093C2C, "GAME.OVL");
    ++count;
    w_u32(0x80059190, count);
    return count;
}

static const char *game_boundary_text_native(uint32 address)
{
    uint32 length = 0;
    while (r_u8(address + length) != 0)
        ++length;
    return (const char *)psx_addr(address, (size_t)length + 1);
}

void game_update_object_mission_boundary(uint32 object)
{
    sint32 paused = r_s16(0x800ECD8C);
    uint32 ticks, identifier, target, player, x, y, z, px, py, pz, hard, soft, text, value;
    sint32 distance;
    FUNCTION_MARKER(0x80095CD0, "GAME.OVL");
    if (paused != 0)
        return;
    ticks = r_u32(object + 0xB4) - 1;
    w_u32(object + 0xB4, ticks);
    if ((sint32)ticks > 0)
        return;
    identifier = r_u32(object + 0xA8);
    target = game_find_script_ship_near(object, identifier, 0x7FFFFFF, 0);
    if (target != 0) {
        player = r_u32(0x80040FDC);
        x = r_u32(target + 8);
        y = r_u32(target + 12);
        px = r_u32(player + 8);
        py = r_u32(player + 12);
        pz = r_u32(player + 16);
        z = r_u32(target + 16);
        distance = vector_length_approx((sint32)(x - px), (sint32)(y - py), (sint32)(z - pz));
        hard = r_u32(object + 0xB0);
        w_u32(0x80046174, (uint32)distance);
        if ((sint32)hard < distance) {
            text = r_u32(0x800F2464);
            stream_enqueue(300, 1, game_boundary_text_native(text));
            player = r_u32(0x80040FDC);
            w_u32(player + 0x40, 0x8007ADB8);
            value = r_u32(target + 8);
            w_u32(player + 0x214, value);
            value = r_u32(target + 12);
            w_u32(player + 0x218, value);
            value = r_u32(target + 16);
            w_u32(player + 0x21C, value);
            soft = r_u32(object + 0xAC);
            w_u32(player + 0xA8, soft - 40000);
        } else {
            soft = r_u32(object + 0xAC);
            if ((sint32)soft < distance) {
                text = r_u32(0x800F2460);
                stream_enqueue(300, 1, game_boundary_text_native(text));
            }
        }
    }
    w_u32(object + 0xB4, 10);
}

uint32 game_intersect_segment_with_face(uint32 face, uint32 start, uint32 end)
{
    VECTOR difference;
    uint32 axis, vertex, coordinate, hit[3], negative, denominator, next, first;
    sint32 start_side, end_side, numerator, quotient;
    FUNCTION_MARKER(0x80098140, "GAME.OVL");
    for (axis = 0; axis < 3; ++axis) {
        vertex = (uint32)r_s16(face + 0xC + axis * 2);
        coordinate = r_u32(start + axis * 4);
        if (axis == 0) difference.vx = (sint32)(coordinate - vertex);
        else if (axis == 1) difference.vy = (sint32)(coordinate - vertex);
        else difference.vz = (sint32)(coordinate - vertex);
    }
    start_side = vector_dot_s16_s32_native(face + 4, &difference) >> 12;
    if (start_side >= 0)
        return 0;
    for (axis = 0; axis < 3; ++axis) {
        vertex = (uint32)r_s16(face + 0xC + axis * 2);
        coordinate = r_u32(end + axis * 4);
        if (axis == 0) difference.vx = (sint32)(coordinate - vertex);
        else if (axis == 1) difference.vy = (sint32)(coordinate - vertex);
        else difference.vz = (sint32)(coordinate - vertex);
    }
    end_side = vector_dot_s16_s32_native(face + 4, &difference) >> 12;
    if (end_side <= 0)
        return 0;
    negative = 0 - (uint32)start_side;
    denominator = negative + (uint32)end_side;
    for (axis = 0; axis < 3; ++axis) {
        next = r_u32(end + axis * 4);
        first = r_u32(start + axis * 4);
        numerator = (sint32)((next - first) * negative);
        if (denominator == 0)
            trap(7);
        if ((sint32)denominator == -1 && numerator == (sint32)0x80000000)
            trap(0x5D);
        quotient = (sint32)((sint64)numerator / (sint32)denominator);
        hit[axis] = first + (uint32)quotient;
        w_u32(0x800FA210 + axis * 4, hit[axis]);
    }
    vertex = (uint32)r_s16(face + 0xC);
    difference.vx = (sint32)(hit[0] - vertex);
    vertex = (uint32)r_s16(face + 0xE);
    difference.vy = (sint32)(hit[1] - vertex);
    vertex = (uint32)r_s16(face + 0x10);
    difference.vz = (sint32)(hit[2] - vertex);
    if (vector_dot_s16_s32_native(face + 0x1C, &difference) > 0)
        return 0;
    if (vector_dot_s16_s32_native(face + 0x24, &difference) > 0)
        return 0;
    for (axis = 0; axis < 3; ++axis) {
        vertex = (uint32)r_s16(face + 0x14 + axis * 2);
        coordinate = r_u32(0x800FA210 + axis * 4);
        if (axis == 0) difference.vx = (sint32)(coordinate - vertex);
        else if (axis == 1) difference.vy = (sint32)(coordinate - vertex);
        else difference.vz = (sint32)(coordinate - vertex);
    }
    if (vector_dot_s16_s32_native(face + 0x2C, &difference) > 0)
        return 0;
    if (r_u32(face) == 0x44415143 && vector_dot_s16_s32_native(face + 0x34, &difference) > 0)
        return 0;
    for (axis = 0; axis < 3; ++axis) {
        coordinate = (uint32)r_s16(face + 4 + axis * 2);
        w_u32(0x801079B0 + axis * 4, coordinate);
    }
    return 1;
}

sint32 game_transform_vector_by_transposed_basis(uint32 matrix, uint32 input, uint32 output)
{
    uint32 column, a, b, x, y, z;
    sint32 result = 0;
    FUNCTION_MARKER(0x8009851C, "GAME.OVL");
    for (column = 0; column < 3; ++column) {
        a = (uint32)r_s16(matrix + column * 2);
        b = r_u32(input);
        x = a * b;
        a = (uint32)r_s16(matrix + 6 + column * 2);
        b = r_u32(input + 4);
        y = a * b;
        a = (uint32)r_s16(matrix + 12 + column * 2);
        b = r_u32(input + 8);
        z = a * b;
        result = (sint32)(x + y + z) >> 12;
        w_u32(output + column * 4, (uint32)result);
    }
    return result;
}

uint32 game_reset_clock_after_600_ticks(void)
{
    uint32 clock, started;
    FUNCTION_MARKER(0x8009A99C, "GAME.OVL");
    clock = r_u32(0x800311B8);
    started = r_u32(0x8010AC50);
    if (clock - started < 601)
        return 0;
    w_u32(0x8010AC50, 0);
    w_u32(0x800311B8, 0);
    return 1;
}

uint32 game_copy_player_adjusted_basis_components(uint32 source, uint32 destination)
{
    uint32 value;
    FUNCTION_MARKER(0x800AE88C, "GAME.OVL");
    if (source == r_u32(0x80040FDC)) {
        value = r_u16(source + 0x128);
        w_u16(destination + 0x120, value);
        value = r_u16(source + 0x12A);
        w_u16(destination + 0x126, value);
    } else {
        value = r_u16(source + 0x120);
        w_u16(destination + 0x120, value);
        value = r_u16(source + 0x126);
        w_u16(destination + 0x126, value);
    }
    value = r_u16(source + 0x12C);
    w_u16(destination + 0x12C, value);
    return value;
}

uint32 game_get_buffer_f4f80(void)
{
    FUNCTION_MARKER(0x800AF6DC, "GAME.OVL");
    return 0x800F4F80;
}

uint32 game_emit_object_world_spark_native(uint32 object, const VECTOR *position, uint32 color, uint32 size)
{
    VECTOR relative, transformed;
    uint32 value, component, index, offset, next, adjusted, result;
    FUNCTION_MARKER(0x800C76B4, "GAME.OVL");
    value = (uint32)position->vx;
    component = r_u32(object + 8);
    relative.vx = (sint32)(value - component);
    value = (uint32)position->vy;
    component = r_u32(object + 12);
    relative.vy = (sint32)(value - component);
    value = (uint32)position->vz;
    component = r_u32(object + 16);
    relative.vz = (sint32)(value - component);
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11C, 20), &relative, &transformed);
    index = r_u32(0x800EF4FC);
    offset = index * 28;
    w_u32(0x80118090 + offset, object);
    w_u32(0x801180A4 + offset, color);
    w_u32(0x801180A8 + offset, size);
    w_u32(0x80118094 + offset, (uint32)transformed.vx);
    next = index + 1;
    w_u32(0x80118098 + offset, (uint32)transformed.vy);
    w_u32(0x8011809C + offset, (uint32)transformed.vz);
    adjusted = (sint32)next >= 0 ? next : index + 16;
    result = next - ((uint32)((sint32)adjusted >> 4) << 4);
    w_u32(0x800EF4FC, result);
    return result;
}

void game_handle_ship_hit_native(uint32 object, VECTOR *position, VECTOR *vector, uint32 other, uint32 projectile, uint32 kind, uint32 profile)
{
    VECTOR temporary, velocity, normalized, rotated;
    uint32 old_shield, old_hull, old_stat, value, second, type, callback;
    uint32 x, y, z, vx, vy, vz, damage, flags, owner, changed;
    uint32 old_damage, delta, capacity, stride, percent, linked, player;
    uint32 low_limit, high_limit, low_fallback, high_fallback;
    uint32 asset, text, record, selection, latch, format, name_slot;
    sint32 magnitude, side;
    FUNCTION_MARKER(0x8009FA2C, "GAME.OVL");
    if (r_u32(object) == 0)
        return;
    if (other != 0 && r_u32(other) == 0)
        return;
    if (kind != 0xCB) {
        owner = r_u32(object + 0x1A8);
        if (owner == r_u32(0x80040FDC)) {
            w_u8(object + 0x271, 1);
            value = r_u32(0x800311B8);
            w_u32(object + 0x8C, value);
        }
    }
    old_shield = r_u32(object + 0x234);
    old_hull = r_u32(object + 0x238);
    type = r_u32(object);
    old_stat = r_u32(object + 0x244);
    if (type == 0x6A) {
        game_emit_model_fragments_native(position, 12, 0);
        game_emit_projectile_fragments_native(position, object, 16, vector);
        if (other != 0)
            return;
        owner = r_u32(object + 0x1A8);
        if (owner != r_u32(0x80040FDC))
            return;
    }
    if ((r_u32(object + 0x114) & 0x10) != 0 && (sint32)old_hull <= 0) {
        value = r_u32(0x800EF50C);
        value = (uint32)((sint32)value >> 1) ^ (value << 3);
        w_u32(0x800EF50C, value);
        if ((value & 15) == 0) {
            value = r_u32(other + 0xFC);
            temporary.vx = (sint32)((uint32)position->vx + (value << 4));
            value = r_u32(other + 0x100);
            temporary.vy = (sint32)((uint32)position->vy + (value << 4));
            value = r_u32(other + 0x104);
            temporary.vz = (sint32)((uint32)position->vz + (value << 4));
            game_emit_object_world_spark_native(object, &temporary, 0x104080, 0x400);
        }
    }
    switch (kind) {
    case 0x69:
        x = (uint32)position->vx;
        y = (uint32)position->vy;
        z = (uint32)position->vz;
        game_emit_hit_flash(x, y, z);
        player = r_u32(0x80040C94);
        sound_play_at_listener_native(position, 4, 0, player);
        goto destroyed_without_sound;
    case 0x64:
        vx = 0u - (uint32)vector->vx;
        vy = 0u - (uint32)vector->vy;
        vz = 0u - (uint32)vector->vz;
        x = (uint32)position->vx;
        y = (uint32)position->vy;
        z = (uint32)position->vz;
        game_emit_narrow_particles(8, x, y, z, vx, vy, vz);
        if ((r_u32(object + 0x114) & 0x10) != 0) {
            value = (uint32)random_between(12, 20);
            game_emit_velocity_fragments_native(position, vector, value);
        } else {
            game_emit_model_fragments_native(position, 12, 0);
        }
        if (r_s32(object + 0x234) > 0) {
            damage = (uint32)random_between(1, 3);
            value = r_u32(object + 0x234);
            w_u32(object + 0x234, value - damage);
        } else {
            damage = (uint32)random_between(2, 5);
            value = r_u32(object + 0x238);
            w_u32(object + 0x238, value - damage);
        }
        if (r_s32(object + 0x238) > 0)
            return;
        goto destroyed_without_sound;
    case 0xD6:
        x = (uint32)position->vx;
        y = (uint32)position->vy;
        z = (uint32)position->vz;
        game_emit_flash_bca38(x, y, z);
        break;
    case 7:
        vx = 0u - (uint32)vector->vx;
        vy = 0u - (uint32)vector->vy;
        vz = 0u - (uint32)vector->vz;
        x = (uint32)position->vx;
        y = (uint32)position->vy;
        z = (uint32)position->vz;
        game_emit_narrow_particles(8, x, y, z, vx, vy, vz);
        if (other != 0 && (r_u32(other + 0x114) & 0x10) != 0) {
            w_u32(object + 0xFC, 0);
            w_u32(object + 0x100, 0);
            w_u32(object + 0x104, 0);
            game_update_object_movement(object);
            game_update_object_movement(object);
            return;
        }
        value = r_u32(object + 0x234);
        damage = r_u32(profile + 8);
        w_u32(object + 0x234, value - damage);
        value = r_u32(object + 0x238);
        damage = r_u32(profile + 4);
        value -= damage;
        w_u32(object + 0x238, value);
        if ((sint32)value > 0)
            return;
        goto destroyed_without_sound;
    case 0x6A:
        goto callback_without_sound;
    }
    if (r_u32(0x800F18AC) != 0 && profile != 0x800EECBC) {
        if (r_s32(object + 0x234) <= 0)
            goto hull_damage;
        if (r_u8(object + 0x273) == 0)
            w_u8(object + 0x273, 1);
    }
    value = r_u32(object + 0x234);
    if ((sint32)value <= 0)
        goto hull_damage;
    damage = r_u32(profile + 8);
    if ((sint32)damage > 0 && (r_u32(profile + 12) & 1) == 0) {
        second = r_u32(object + 0x14);
        w_u32(object + 0x234, value - damage);
        velocity.vx = (sint32)second >> 8;
        velocity.vy = r_s32(object + 0x18) >> 8;
        velocity.vz = r_s32(object + 0x1C) >> 8;
        game_emit_camera_particle_native(position, &velocity, 0xFF0000, 0x800);
        game_emit_camera_particle_native(position, &velocity, 0xFFFFFF, 0x400);
        if (r_s32(object + 0x234) > 0)
            goto shield_profile;
        if ((r_u32(object + 0x114) & 0x20000000) != 0)
            game_begin_ship_allegiance_change(object);
    }
    if (r_s32(object + 0x234) <= 0)
        goto hull_damage;
shield_profile:
    if ((r_u32(profile + 12) & 1) == 0)
        goto collision_attribution;
hull_damage:
    if (r_u32(0x800F18AC) != 0 && r_u8(object + 0x273) != 16)
        w_u8(object + 0x273, 4);
    value = r_u32(object + 0x238);
    damage = r_u32(profile + 4);
    w_u32(object + 0x238, value - damage);
    damage = r_u32(profile);
    if ((sint32)damage > 0)
        game_apply_sensor_damage(object, (sint16)damage);
    damage = r_u32(profile + 4);
    magnitude = (sint16)(damage << 3);
    if (magnitude >= 26)
        magnitude = 25;
    else if (magnitude < 4)
        magnitude = 4;
    if (magnitude < 10) {
        if ((r_u32(object + 0x114) & 0x10) != 0) {
            ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), (VECTOR *)psx_addr(0x801079B0, 12), &rotated);
            w_u32(0x801079B0, (uint32)rotated.vx);
            w_u32(0x801079B4, (uint32)rotated.vy);
            w_u32(0x801079B8, (uint32)rotated.vz);
            vector_normalize(vector, &normalized);
            game_reflect_vector_native(&normalized, (const VECTOR *)psx_addr(0x801079B0, 12), &temporary);
            vx = (uint32)temporary.vx;
            vy = (uint32)temporary.vy;
            vz = (uint32)temporary.vz;
            x = (uint32)position->vx;
            y = (uint32)position->vy;
            z = (uint32)position->vz;
            game_emit_reflected_particles((uint32)magnitude + 6, x, y, z, vx, vy, vz);
        } else {
            value = (uint32)position->vx;
            vx = value - r_u32(object + 8);
            value = (uint32)position->vy;
            vy = value - r_u32(object + 12);
            value = (uint32)position->vz;
            vz = value - r_u32(object + 16);
            x = (uint32)position->vx;
            y = (uint32)position->vy;
            z = (uint32)position->vz;
            game_emit_narrow_particles((uint32)magnitude + 6, x, y, z, vx, vy, vz);
        }
    } else {
        flags = r_u32(object + 0x114);
        game_emit_projectile_fragments_native(position, object, magnitude, (flags & 0x10) != 0 ? vector : NULL);
        velocity.vx = 0;
        velocity.vy = 0;
        velocity.vz = 0;
        game_emit_camera_particle_native(position, &velocity, 0x202050, 8000);
    }
collision_attribution:
    if (other != 0) {
        type = r_u32(object);
        if (type == 1 || type == 3 || type == 0x66) {
            type = r_u32(other);
            if (type == 1 || type == 3 || type == 0x66) {
                w_u32(object + 0x238, old_hull);
                w_u32(object + 0x234, old_shield);
            }
        }
        if (r_u32(object) == 7)
            return;
        type = r_u32(other);
        if (type != 3 && type != 1 && type != 0x6A && type != 0x66) {
            value = r_u32(other + 0x160);
            w_u32(object + 0x1B0, value);
            value = r_u16(other + 0x164);
            w_u16(object + 0x1B4, value);
            value = r_u32(other + 0x160);
            w_u32(object + 0x1A8, value);
            value = r_u16(other + 0x164);
            w_u16(object + 0x1AC, value);
            callback = r_u32(other + 0x5C);
            psx_dispatch_guest_call(callback, 1, other, 0, 0);
        }
    }
    value = r_u32(object + 0x244);
    owner = r_u32(object + 0x1A8);
    changed = value != old_stat;
    if (owner != r_u32(0x80040FDC))
        goto autonomous_response;
    if (r_u32(0x800F18AC) != 0)
        goto shield_response;
    side = r_s16(0x80059612);
    type = r_u32(object);
    old_damage = r_u32(object + 0x240);
    if (type != (uint32)side) {
        value = r_u32(0x800311B8);
        w_u32(0x800EEB20, value);
    }
    if (changed == 0) {
        value = r_u32(object + 0x234);
        second = r_u32(object + 0x238);
        delta = old_shield - value;
        second = old_hull - second;
        w_u32(0x8011F130, second);
        if ((sint32)delta < (sint32)second)
            delta = second;
        low_limit = r_u16(0x800595D0);
        high_limit = r_u16(0x800595D8);
        stride = r_u32(object + 4) * 60;
        low_fallback = low_limit;
        value = r_u32(0x800536A4 + stride);
        second = r_u32(0x800536A8 + stride);
        high_fallback = high_limit;
        capacity = value + second;
    } else {
        low_limit = 45;
        high_limit = 85;
        low_fallback = 50;
        high_fallback = 90;
        value = r_u32(object + 0x244);
        stride = r_u32(object + 4) * 60;
        delta = old_stat - value;
        capacity = r_u32(0x800536AC + stride);
    }
    value = r_u32(object + 0x240);
    type = r_u32(object);
    w_u32(object + 0x240, value + delta);
    if (type != 1 && type != (uint32)(sint32)r_s16(0x80059612)) {
        side = r_s16(0x80059614);
        if (r_u32(object + 4) != (uint32)side)
            goto clear_attribution;
        if (r_u32(0x800595F8) != 0x50006)
            goto clear_attribution;
    }
    if (r_u32(0x800595C8) != 0)
        goto clear_attribution;
    if ((r_u32(object + 0x114) & 0x10000000) != 0 || kind == 0x66)
        goto clear_attribution;
    if (r_u32(0x800EEB24) != 0)
        goto clear_attribution;
    if ((sint32)capacity < 36 && changed != 1) {
        side = r_s16(0x80059614);
        if (r_u32(object + 4) != (uint32)side)
            goto clear_attribution;
    }
    value = r_u32(object + 0x240) * 100;
    if (capacity == 0)
        trap(7);
    if (capacity == 0xFFFFFFFF && value == 0x80000000)
        trap(0x5D);
    percent = (uint32)((sint64)(sint32)value / (sint32)capacity);
    side = r_s16(0x8005961A);
    if (side == 2) {
        side = r_s16(0x80059614);
        if (r_u32(object + 4) == (uint32)side && r_s32(object + 0x238) > 0)
            goto lesser_warning_state;
        flags = r_u32(object + 0x114);
        if (((flags & 0x10) != 0 && (sint32)percent > (sint16)high_limit) || (sint32)percent > (sint16)high_fallback) {
            side = r_s16(0x80059612);
            w_u16(0x80059610, side == 1 ? 8 : 1);
            stream_clear_queue();
            message_init();
            side = r_s16(0x80059612);
            w_u16(0x8005961A, 3);
            if (side == 3) {
                stream_clear_queue();
                message_init();
                text = r_u32(0x800F2450);
                message_add((char *)game_boundary_text_native(text), 0, 15);
                record = r_u32(0x800F2B08);
                value = r_u32(record + 0x32FC);
                second = r_u32(record + 0x3304);
                stream_request_segments(record + 0x32F8, value, second, 0x800118FE, 20001, 0, NULL);
            } else {
                record = r_u32(0x800F2B08);
                text = r_u32(0x800F2454);
                value = r_u32(record + 0x35C);
                second = r_u32(record + 0x364);
                stream_request_segments(record + 0x358, value, second, 0x800118FE, 30000, 0, game_boundary_text_native(text));
            }
            game_create_controller_behind_player();
            value = r_u32(0x800311B8);
            side = r_s16(0x80059612);
            w_u16(0x80059612, 1);
            w_u32(0x800EEB1C, value);
            w_u32(0x800EEB24, value);
            w_u32(0x800EEB28, (uint32)side);
            linked = object_lookup(1);
            if (linked == 0)
                goto clear_attribution;
            player = r_u32(0x80040FDC);
            do {
                if (linked != player) {
                    flags = r_u32(linked + 0x114);
                    w_u32(linked + 0x178, player);
                    value = r_u16(player + 0x24A);
                    flags |= 0x10000000;
                    w_u32(linked + 0x114, flags);
                    w_u16(linked + 0x17C, value);
                    if ((flags & 0x10) == 0) {
                        w_u32(linked + 0x58, 0x800D9D90);
                        w_u32(linked + 0x40, 0x800DB30C);
                    }
                }
                linked = r_u32(linked + 0x1DC);
            } while (linked != 0);
            goto clear_attribution;
        }
lesser_warning_state:
        side = r_s16(0x8005961A);
    }
    if (side == 1) {
        flags = r_u32(object + 0x114);
        if (((flags & 0x10) != 0 && (sint32)percent > (sint16)low_limit) || (sint32)percent > (sint16)low_fallback) {
            record = r_u32(0x80041004);
            w_u16(0x8005961A, 2);
            if (r_u32(record) != 0)
                goto clear_attribution;
            if (r_s16(0x80059612) != 1)
                goto clear_attribution;
            value = r_u32(0x800EEB20);
            second = r_u32(0x800311B8);
            if (value + 120 >= second)
                goto clear_attribution;
            value = (uint32)random_between(0, 1);
            if (value == 0) {
                text = r_u32(0x800F23C4);
                asset = 28;
            } else {
                text = r_u32(0x800F23C8);
                asset = 27;
            }
            goto radio_warning;
        }
    }
    value = r_u32(0x800EEB1C);
    second = r_u32(0x800311B8);
    if ((sint32)(value + 300) >= (sint32)second)
        goto clear_attribution;
    if (r_s16(0x8005961A) == 0)
        w_u16(0x8005961A, 1);
    record = r_u32(0x80041004);
    if (r_u32(record) != 0)
        goto clear_attribution;
    record = r_u32(0x800EF4E0);
    if (record != 0 && r_u32(record) == 3)
        goto clear_attribution;
    if (old_damage == r_u32(object + 0x240))
        goto clear_attribution;
    text = r_u32(0x800F23C0);
    asset = 30;
radio_warning:
    game_request_radio_message(asset, text);
    value = r_u32(0x800311B8);
    w_u32(0x800EEB1C, value);
clear_attribution:
    w_u32(object + 0x1A8, 0);
autonomous_response:
    if (r_u32(0x800F18AC) == 0 && r_u32(object) == 3 && (r_u32(object + 0x114) & 0x50440010) == 0) {
        stride = r_u32(object + 4) * 60;
        value = r_u32(0x800536A4 + stride);
        second = r_u32(0x800536A8 + stride);
        capacity = value + second;
        value = r_u32(object + 0x240) * 100;
        if (capacity == 0)
            trap(7);
        if (capacity == 0xFFFFFFFF && value == 0x80000000)
            trap(0x5D);
        percent = (uint32)((sint64)(sint32)value / (sint32)capacity);
        if ((sint32)percent >= 31) {
            player = r_u32(0x80040FDC);
            flags = r_u32(object + 0x114);
            w_u32(object + 0x178, player);
            value = r_u16(player + 0x24A);
            w_u32(object + 0x114, flags | 0x10000000);
            w_u32(object + 0x40, 0x800DBE20);
            w_u16(object + 0x17C, value);
        }
    }
shield_response:
    if (old_shield != r_u32(object + 0x234)) {
        if (kind != 1 && kind != 3 && kind != 0x66 && projectile != 0) {
            owner = r_u32(projectile + 0x58);
            if (owner != 0) {
                value = r_u16(owner + 0x24A);
                second = r_u16(projectile + 0x5C);
                if (value == second) {
                    type = r_u32(owner);
                    w_u32(0x8011F130, type);
                    if (type == 0x66) {
                        value = (uint32)(sint32)r_s16(0x80059612);
                        w_u32(0x8011F130, value);
                    }
                    flags = r_u32(object + 0x114);
                    if ((flags & 0x210) == 0x10) {
                        value = r_u32(0x8011F130);
                        type = r_u32(object);
                        if (value != type && type == 1) {
                            value = r_u32(object + 0x234);
                            stride = r_u32(object + 4) * 60;
                            value = (value << 3) - value;
                            second = r_u32(0x800536A4 + stride);
                            if (((sint32)value >> 3) < (sint32)second) {
                                name_slot = (uint32)(sint32)r_s16(object + 0x24E);
                                format = r_u32(0x800F23A8);
                                sprintfPSX(0x800FA368, format, 0x80059E70 + name_slot * 44);
                                game_queue_hud_message(0x800FA368);
                                flags = r_u32(object + 0x114);
                                w_u32(object + 0x114, flags | 0x200);
                            }
                        }
                    }
                }
            }
        }
        if (r_s32(object + 0x234) <= 0) {
            if (r_u32(0x800F18AC) == 0) {
                if ((r_u32(object + 0x114) & 0x10) != 0) {
                    name_slot = (uint32)(sint32)r_s16(object + 0x24E);
                    format = r_u32(0x800F23AC);
                    sprintfPSX(0x800FA368, format, 0x80059E70 + name_slot * 44);
                    game_queue_hud_message(0x800FA368);
                    w_u32(object + 0x234, 0);
                    goto shield_sound;
                }
                stride = r_u32(object + 4) * 60;
                if (r_u32(0x80053698 + stride) == 1 && r_u8(0x800EEB04) == 0) {
                    if (random_between(0, 100) < 25) {
                        record = r_u32(0x80041004);
                        if (r_u32(record) == 0) {
                            value = r_u8(0x800EEB05);
                            selection = (uint32)random_between(0, (sint32)(value - 1));
                            value = r_u8(object + 0x277);
                            record = r_u32(0x800EEB08);
                            player = r_u32(0x800F2B08);
                            value = (uint32)(sint32)r_s16(record + value * 2) + selection;
                            record = player + value * 24;
                            second = r_u32(record + 0x14);
                            text = r_u32(record + 0x1C);
                            w_u32(0x8011F130, value);
                            stream_request_segments(record + 0x10, second, text, 0x800118FE, 1, 0, NULL);
                            w_u8(0x800EEB04, 1);
                        }
                    }
                }
            }
            w_u32(object + 0x234, 0);
        }
shield_sound:
        player = r_u32(0x80040C94);
        sound_play_at_listener(object + 8, 3, 0, player);
    }
    if (old_hull == r_u32(object + 0x238) && old_shield == r_u32(object + 0x234))
        return;
    if ((r_u32(other + 0x114) & 4) != 0) {
        w_u32(object + 0x238, old_hull);
        w_u32(object + 0x234, old_shield);
    }
    owner = r_u32(object + 0x1A8);
    if (owner != 0 && (r_u32(owner + 0x114) & 0x100) != 0) {
        value = r_u32(object + 0x238);
        if (old_hull != value) {
            second = r_u16(object + 0x248);
            w_u16(object + 0x248, second + old_hull - value);
        }
        value = r_u32(object + 0x234);
        if (old_shield != value) {
            second = r_u16(object + 0x248);
            w_u16(object + 0x248, second + old_shield - value);
        }
    }
    value = r_u32(object + 0x23C);
    if ((sint32)value > 0) {
        second = r_u32(profile + 8);
        damage = r_u32(profile + 4);
        flags = r_u32(profile);
        owner = r_u32(object + 0x1A8);
        player = r_u32(0x80040FDC);
        value -= second + damage + flags;
        w_u32(object + 0x23C, value);
        if (owner != player) {
            second = r_u32(object + 0x238);
            damage = r_u32(object + 0x234);
        } else {
            second = r_u32(object + 0x234);
            damage = r_u32(object + 0x238);
        }
        if ((sint32)(second + damage) < (sint32)value) {
            w_u32(object + 0x238, old_hull);
            w_u32(object + 0x234, old_shield);
        }
        return;
    }
    if (r_s32(object + 0x238) > 0) {
        if (r_u32(object) == 0x6A)
            goto hull_sound;
        side = r_s16(0x80059618);
        if (r_u32(object + 4) == (uint32)side) {
            value = (uint32)random_between(2, 8);
            game_emit_velocity_fragments_native(position, vector, value);
        }
        flags = r_u32(object + 0x114);
        if ((flags & 2) != 0)
            goto hull_sound;
        if ((flags & 0x10) != 0) {
            stride = r_u32(object + 4) * 60;
            value = r_u32(object + 0x238);
            second = r_u32(0x800536A8 + stride);
            if ((sint32)(value << 3) >= (sint32)second)
                goto hull_sound;
            name_slot = (uint32)(sint32)r_s16(object + 0x24E);
            format = r_u32(0x800F23B0);
            sprintfPSX(0x800FA368, format, 0x80059E70 + name_slot * 44);
            game_queue_hud_message(0x800FA368);
            flags = r_u32(object + 0x114);
            w_u32(object + 0x114, flags | 2);
            goto hull_sound;
        }
        stride = r_u32(object + 4) * 60;
        value = r_u32(0x800536A8 + stride);
        second = r_u32(object + 0x238);
        if ((sint32)(value << 1) < (sint32)second || (sint32)(value << 2) >= (sint32)second)
            goto hull_sound;
        if (r_u32(0x80053698 + stride) != 1 || r_u8(0x800EEAF4) != 0)
            goto hull_sound;
        if (random_between(0, 100) >= 65)
            goto hull_sound;
        record = r_u32(0x80041004);
        if (r_u32(record) != 0)
            goto hull_sound;
        value = r_u8(0x800EEAF5);
        selection = (uint32)random_between(0, (sint32)(value - 1));
        value = r_u8(object + 0x277);
        record = r_u32(0x800EEAF8);
        player = r_u32(0x800F2B08);
        value = (uint32)(sint32)r_s16(record + value * 2) + selection;
        record = player + value * 24;
        second = r_u32(record + 0x14);
        text = r_u32(record + 0x1C);
        w_u32(0x8011F130, value);
        stream_request_segments(record + 0x10, second, text, 0x800118FE, 1, 0, NULL);
        w_u8(0x800EEAF4, 1);
        goto hull_sound;
    }
    value = r_u16(0x80059610);
    if (value - 1 < 2 && (r_u32(object + 0x114) & 0x10) != 0)
        w_u32(object + 0x238, old_hull);
    if (r_u32(0x800EC884) != 0) {
        w_u32(object + 0x238, old_hull);
        goto hull_sound;
    }
    stride = r_u32(object + 4) * 60;
    if (r_u32(0x80053698 + stride) != 3)
        goto destroyed_with_sound;
    latch = 0x800EEB0C;
    if (r_u8(latch) == 0) {
        value = r_u32(object + 0x1B0);
        if (value != r_u32(0x80040FDC)) {
            if (random_between(0, 100) < 100) {
                record = r_u32(0x80041004);
                if (r_u32(record) != 0)
                    goto destroyed_with_sound;
                value = r_u8(0x800EEB0D);
                selection = (uint32)random_between(0, (sint32)(value - 1));
                value = r_u8(object + 0x277);
                record = r_u32(0x800EEB10);
                goto destroyed_radio_record;
            }
        }
    }
    latch = 0x800EEB14;
    if (r_u8(latch) != 0)
        goto destroyed_with_sound;
    value = r_u32(object + 0x1B0);
    if (value != r_u32(0x80040FDC))
        goto destroyed_with_sound;
    if (game_count_active_standard_objects() == 0)
        goto destroyed_with_sound;
    if (random_between(0, 100) >= 35)
        goto destroyed_with_sound;
    record = r_u32(0x80041004);
    if (r_u32(record) != 0)
        goto destroyed_with_sound;
    value = r_u8(0x800EEB15);
    selection = (uint32)random_between(0, (sint32)(value - 1));
    value = r_u32(0x800ED460);
    record = r_u32(0x800EEB18);
destroyed_radio_record:
    value = (uint32)(sint32)r_s16(record + value * 2);
    player = r_u32(0x800F2B08);
    value += selection;
    record = player + value * 24;
    second = r_u32(record + 0x14);
    text = r_u32(record + 0x1C);
    w_u32(0x8011F130, value);
    stream_request_segments(record + 0x10, second, text, 0x800118FE, 1, 0, NULL);
    w_u8(latch, 1);
destroyed_with_sound:
    game_record_destroyed_object(object, other, projectile);
    callback = r_u32(object + 0x5C);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
hull_sound:
    player = r_u32(0x80040C94);
    sound_play_at_listener(object + 8, 2, 0, player);
    return;
destroyed_without_sound:
    game_record_destroyed_object(object, other, projectile);
callback_without_sound:
    callback = r_u32(object + 0x5C);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}

uint32 game_emit_flash_bca38(uint32 x, uint32 y, uint32 z)
{
    uint32 index, offset, next, multiple;
    sint32 quotient;
    FUNCTION_MARKER(0x800BCA38, "GAME.OVL");
    index = r_u32(0x800EF504);
    offset = index * 48;
    next = index + 1;
    w_u32(0x801077D0 + offset, x);
    w_u32(0x801077F8 + offset, 13);
    w_u32(0x801077D4 + offset, y);
    w_u32(0x801077D8 + offset, z);
    w_u32(0x801077E0 + offset, 0);
    w_u32(0x801077F4 + offset, 255);
    w_u32(0x801077E8 + offset, 255);
    w_u32(0x801077EC + offset, 255);
    w_u32(0x801077F0 + offset, 255);
    w_u32(0x801077E4 + offset, 128);
    w_u32(0x801077FC + offset, 0);
    quotient = (sint32)next / 10;
    multiple = (uint32)quotient * 10;
    w_u32(0x800EF504, next - multiple);
    return multiple;
}

void game_update_boundary_autopilot(uint32 object)
{
    uint32 player = r_u32(0x80040FDC);
    uint32 target_x, target_y, target_z, x, y, z, yaw, pitch;
    uint32 value, limit, increment, axis, coefficient, text;
    sint32 first, second, third;
    FUNCTION_MARKER(0x8007ADC0, "GAME.OVL");
    target_x = r_u32(object + 0x214);
    target_y = r_u32(object + 0x218);
    w_u16(0x800ECD8C, 1);
    target_z = r_u32(object + 0x21C);
    w_u32(0x800311DC, 0);
    x = r_u32(player + 8);
    z = r_u32(player + 16);
    y = r_u32(player + 12);
    game_set_relative_target_angles(object, target_x - x, target_y - y, target_z - z);
    value = r_u32(0x80033D1C);
    yaw = (uint32)game_limit_signed_step((sint32)value, 12);
    value = r_u32(0x80033D30);
    w_u32(0x80033D1C, yaw);
    pitch = (uint32)game_limit_signed_step((sint32)value, 12);
    yaw = r_u32(0x80033D1C);
    w_u32(0x80033D30, pitch);
    game_apply_control_rotation(object + 0x11C, yaw, 0u - pitch, 0);
    if (r_u32(0x80033D1C) != 0 || r_u32(0x80033D30) != 0)
        matrix_normalize(object + 0x11C, 1);
    object_extract_angles(object, object);
    matrix_copy_rotation(0x80035060, object + 0x11C);
    first = r_s16(0x80035060) * 8;
    second = r_s16(0x80035062) * 8;
    third = r_s16(0x80035064) * 8;
    w_u16(0x80035060, (uint16)(first / 5));
    w_u16(0x80035062, (uint16)(second / 5));
    w_u16(0x80035064, (uint16)(third / 5));
    camera_dispatch();
    value = r_u32(0x800F2B1C);
    limit = r_u32(0x800F2B24);
    if ((sint32)value < (sint32)limit) {
        increment = r_u32(0x800F2B28);
        w_u32(0x800F2B1C, value + increment);
    }
    for (axis = 0; axis < 3; ++axis) {
        value = r_u32(0x800F2B1C);
        coefficient = (uint32)(sint32)r_s16(object + 0x128 + axis * 2);
        value = (uint32)((sint32)value >> 8);
        value *= coefficient;
        w_u32(object + 0xFC + axis * 4, (uint32)((sint32)value >> 12));
    }
    game_update_object_movement(object);
    value = r_u32(object + 0xA8);
    if (value != 0xFFFFFFFF) {
        limit = r_u32(0x80046174);
        if ((sint32)limit < (sint32)value) {
            text = r_u32(0x800F2468);
            stream_enqueue(300, 1, game_boundary_text_native(text));
            w_u32(object + 0x9C, 20);
            w_u32(object + 0xA8, 0xFFFFFFFF);
        }
    } else {
        value = r_u32(object + 0x9C) - 1;
        w_u32(object + 0x9C, value);
        if ((sint32)value <= 0) {
            w_u16(0x80118084, 10);
            w_u32(object + 0x40, 0x80078F5C);
            w_u16(0x800ECD8C, 0);
        }
    }
}

void game_return_7a8dc(void)
{
    FUNCTION_MARKER(0x8007A8DC, "GAME.OVL");
}

void game_update_player_end_delay(uint32 object)
{
    uint32 callback, value, first, second, mode;
    FUNCTION_MARKER(0x8007A8E4, "GAME.OVL");
    callback = r_u32(0x800ECEAC);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
    value = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, value);
    if ((sint32)value > 0)
        return;
    if (r_u32(0x800EC86C) != 0) {
        render_set_reduced_height(512);
        game_refresh_mission_state();
        w_u32(object + 0x40, 0x80078F5C);
        w_u32(0x800EC88C, 1);
        game_remove_objects_by_type(1);
        game_remove_objects_by_type(3);
    } else {
        first = r_u16(0x80059600);
        second = r_u16(0x80059602);
        mode = r_u32(0x800F32C8);
        w_u32(0x800ECEA8, 3);
        w_u16(0x800595FC, first);
        w_u16(0x800595FE, second);
        if (mode == 0)
            w_u32(0x800313DC, 3);
        if (mode == 1)
            w_u32(0x800313DC, 4);
    }
}

void game_return_7a9cc(void)
{
    FUNCTION_MARKER(0x8007A9CC, "GAME.OVL");
}

void game_return_7a9d4(void)
{
    FUNCTION_MARKER(0x8007A9D4, "GAME.OVL");
}

uint32 game_prepare_mission_completion_state(void)
{
    FUNCTION_MARKER(0x8007BD84, "GAME.OVL");
    render_set_reduced_height(512);
    game_refresh_mission_state();
    w_u32(0x800EC884, 2);
    return 2;
}

uint32 game_initialize_status_slot_type14(uint32 index)
{
    uint32 offset;
    FUNCTION_MARKER(0x8007E2E0, "GAME.OVL");
    offset = index * 52;
    w_u32(0x80119C5C + offset, 0x8007EF74);
    w_u16(0x80119C60 + offset, 3);
    w_u32(0x80119C54 + offset, 14);
    w_u16(0x80119C62 + offset, 0);
    return offset;
}

void game_update_rotating_laser_object(uint32 object)
{
    uint32 enabled = r_u32(0x800ECD9C);
    uint32 value, clock, slot, target;
    SVECTOR angles;
    FUNCTION_MARKER(0x8007B05C, "GAME.OVL");
    if (enabled != 0) {
        value = r_u32(object + 8);
        clock = r_u32(0x800311B8);
        value -= 256;
        slot = clock & 31;
        w_u32(object + 8, value);
        if (slot < 7 && r_u32(0x800FA2A8 + slot * 4) == object) {
            value = r_u32(0x800EF50C);
            value = (uint32)((sint32)value >> 1) ^ (value << 3);
            w_u32(0x800EF50C, value);
            if ((value & 1) == 0) {
                target = r_u32(0x80040FDC);
                angles.vx = 0;
                angles.vy = 0;
                angles.vz = 10000;
                game_create_hull_laser_native(object, target, &angles);
            }
        }
    }
    angles.vx = 2048;
    angles.vy = 1024;
    angles.vz = 0;
    RotMatrix(&angles, (MATRIX *)psx_addr(object + 0x11C, 18));
}

void game_return_7e34c(void)
{
    FUNCTION_MARKER(0x8007E34C, "GAME.OVL");
}

void game_update_player_end_sequence(uint32 object)
{
    uint32 mode, value, first, second, third, fourth, fifth, sixth;
    uint32 x, y, z, index, slot, ship, player, flags, type;
    uint32 model_index, model_offset, model_base, record, timer, special;
    sint32 case_x, case_z;
    FUNCTION_MARKER(0x8007B12C, "GAME.OVL");
    game_update_exit_request();
    mode = r_u32(0x800EC88C);
    if (mode == 0) {
        if (r_u32(0x800F32C8) != 0) {
            value = r_u32(0x80126A98) + 32;
            w_u32(0x80126A98, value);
            camera_set_follow_distance(value, 1);
        }
        first = r_u16(object + 0x122);
        x = r_u32(object + 8);
        second = r_u16(object + 0x124);
        fifth = r_u16(object + 0x12A);
        x -= (uint32)((sint16)first >> 6);
        w_u32(object + 8, x);
        y = r_u32(object + 12);
        third = r_u16(object + 0x126);
        y -= (uint32)((sint16)second >> 6);
        w_u32(object + 12, y);
        z = r_u32(object + 16);
        fourth = r_u16(object + 0x128);
        z -= (uint32)((sint16)third >> 6);
        w_u32(object + 16, z);
        x = r_u32(object + 8);
        sixth = r_u16(object + 0x12C);
        w_u32(object + 8, x - (uint32)((sint16)fourth >> 6));
        y = r_u32(object + 12);
        w_u32(object + 12, y - (uint32)((sint16)fifth >> 6));
        z = r_u32(object + 16);
        w_u32(object + 16, z - (uint32)((sint16)sixth >> 6));
        return;
    }
    if (mode == 1) {
        w_u32(object + 8, 0);
        w_u32(object + 12, 0x4E2000);
        w_u32(object + 16, 0);
        camera_reset_follow();
        camera_set_target(object);
        w_u32(0x80118A70, (uint32)-30000);
        y = r_u32(object + 12);
        w_u32(0x80118A78, 1000);
        w_u32(0x80118A74, y + 600);
        camera_set_position(0x80118A70);
        w_u32(0x800EC88C, 2);
        RotMatrix((SVECTOR *)psx_addr(0x800ECD94, 6), (MATRIX *)psx_addr(object + 0x11C, 18));
        camera_set_up(0, 4096, 0);
        special = r_u32(0x800EC870);
        w_u32(object + 0x9C, 1650);
        if (special != 0) {
            w_u32(object + 0x9C, 900);
            w_u32(0x800ECD9C, 0);
            for (index = 0; index < 7; ++index) {
                slot = 0x800FA2A8 + index * 4;
                switch (index) {
                case 0: case_x = -10000; case_z = 3000; break;
                case 1: case_x = -7000; case_z = -3000; break;
                case 2: case_x = -4000; case_z = 300; break;
                case 3: case_x = -1000; case_z = 1000; break;
                case 4: case_x = 0; case_z = -2000; break;
                case 5: case_x = 2000; case_z = -1000; break;
                default: case_x = 4000; case_z = 2500; break;
                }
                y = r_u32(object + 12);
                ship = game_create_object(1, (uint32)case_x, y + 0x7D000, (uint32)case_z, 0);
                w_u32(slot, ship);
                ship = r_u32(slot);
                w_u32(ship + 0x3C, 0x800D1C3C);
                ship = r_u32(slot);
                value = (uint32)(sint32)r_s16(0x80059614);
                w_u32(ship + 4, value);
                ship = r_u32(slot);
                w_u32(ship + 0x1E8, 0);
                ship = r_u32(slot);
                w_u32(ship + 0x1EC, 0);
                ship = r_u32(slot);
                w_u32(ship + 0x1F0, 0);
                ship = r_u32(slot);
                w_u32(ship + 0x40, 0x8007B054);
                ship = r_u32(slot);
                w_u32(ship + 0x9C, 1);
                ship = r_u32(slot);
                w_u32(ship + 0x84, (uint32)-3);
                ship = r_u32(slot);
                player = r_u32(0x80040FDC);
                w_u32(ship + 0xD8, 200000);
                ship = r_u32(slot);
                w_u32(ship + 0x170, player);
                ship = r_u32(slot);
                value = r_u16(player + 0x24A);
                w_u16(ship + 0x174, value);
                ship = r_u32(slot);
                w_u32(ship + 0x48, 0x800A5030);
                ship = r_u32(slot);
                flags = r_u32(ship + 0x70);
                w_u32(ship + 0x70, flags | 0x100000);
                ship = r_u32(slot);
                w_u32(ship + 0x238, 255);
                ship = r_u32(slot);
                w_u32(ship + 0x114, 0);
                ship = r_u32(slot);
                value = r_u32(0x800ECBD0);
                w_u32(ship + 0x68, 0);
                ship = r_u32(slot);
                w_u32(ship, 1);
                model_offset = value * 44;
                ship = r_u32(slot);
                model_index = (uint32)(sint32)r_s16(0x800ECBEC + model_offset);
                w_u32(ship + 0x74, model_index);
                model_index = (uint32)(sint32)r_s16(0x800ECBEC + model_offset);
                model_base = r_u32(0x80046178);
                record = model_base + model_index * 132;
                ship = r_u32(slot);
                value = r_u32(record);
                w_u32(ship + 0x64, value);
            }
            model_index = r_u32(0x800ECDA0);
            if ((sint32)model_index > 0) {
                value = r_u32(0x800ECDA4) - 2;
                w_u32(0x800ECDA4, value);
                if ((sint32)value < 0)
                    w_u32(0x800ECDA4, 0);
                ship = r_u32(0x800FA2C0);
                w_u32(ship + 0x114, 16);
                ship = r_u32(0x800FA2C0);
                w_u32(ship + 0x68, 1);
                ship = r_u32(0x800FA2C0);
                w_u32(ship + 0x74, model_index);
                record = model_base + model_index * 132;
                ship = r_u32(0x800FA2C0);
                value = r_u32(record);
                type = r_u32(0x800ECDA4);
                w_u32(ship + 0x64, value);
                ship = r_u32(0x800FA2C0);
                w_u32(ship + 0x84, type);
                value = 2000u << (type & 31);
                ship = r_u32(0x800FA2C0);
                w_u32(ship + 8, value + 11000);
                ship = r_u32(0x800FA2C0);
                value = r_u32(ship + 16);
                w_u32(ship + 16, value << (type & 31));
            }
            game_request_radio_message(37, 0x800744DC);
        } else {
            game_request_radio_message(800, 0x800744DC);
        }
        return;
    }
    x = r_u32(object + 8);
    special = r_u32(0x800EC870);
    w_u32(object + 8, x - 64);
    if (special != 0) {
        value = r_u32(0x800311E0);
        x -= 160;
        w_u32(object + 8, x);
        if ((sint32)x < (sint32)value) {
            for (index = 0; index < 6; ++index) {
                y = r_u32(object + 12);
                ship = r_u32(0x800FA2A8 + index * 4);
                switch (index) {
                case 0: value = y - 1000; break;
                case 1: value = y + 1000; break;
                case 2: value = y - 1000; break;
                case 3: value = y + 500; break;
                case 4: value = y - 1500; break;
                default: value = y + 1500; break;
                }
                w_u32(ship + 12, value);
            }
            ship = r_u32(0x800FA2C0);
            y = r_u32(object + 12);
            model_index = r_u32(0x800ECDA0);
            w_u32(ship + 12, y - 750);
            if ((sint32)model_index > 0) {
                value = r_u32(0x800ECDA4);
                value = 3000u << (((uint32)((sint32)value >> 1)) & 31);
                y = r_u32(object + 12);
                ship = r_u32(0x800FA2C0);
                w_u32(ship + 12, y - value);
            }
            w_u32(0x800ECD9C, 1);
        }
    }
    value = r_u16(0x800ECD94) + 1;
    w_u16(0x800ECD94, value);
    first = r_u16(0x800ECD96);
    second = r_u16(0x800ECD98);
    w_u16(0x800ECD96, first + 2);
    w_u16(0x800ECD98, second - 3);
    RotMatrix((SVECTOR *)psx_addr(0x800ECD94, 6), (MATRIX *)psx_addr(object + 0x11C, 18));
    camera_set_up(0, 4096, 0);
    w_u32(0x80118A70, (uint32)-32);
    w_u32(0x80118A74, 0);
    w_u32(0x80118A78, 0);
    camera_apply_translation(0x80118A70);
    timer = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, timer);
    if ((sint32)timer >= 751)
        return;
    record = r_u32(0x80041004);
    if (r_u32(record) != 0)
        return;
    first = r_u16(0x80059600);
    second = r_u16(0x80059602);
    mode = r_u32(0x800F32C8);
    w_u32(0x800ECEA8, 3);
    w_u16(0x800595FC, first);
    w_u16(0x800595FE, second);
    if (mode == 0)
        w_u32(0x800313DC, 3);
    if (mode == 1)
        w_u32(0x800313DC, 4);
}

sint32 game_run_scene(void)
{
    uint32 timing = r_u32(0x800311C0);
    uint32 value, callback, first, second, offset, record, sectors, asset;
    sint32 projection;
    FUNCTION_MARKER(0x80082880, "GAME.OVL");
    w_u8(0x800313D4, 0);
    w_u32(0x80059630, 0);
    w_u16(0x800ECEBC, timing != 0 ? 12 : 10);
    game_initialize_model_name_cursor();
    game_initialize_weapon_defaults();
    if (resource_exists("GAME.CNF") != 0)
        game_load_configuration("GAME.CNF");
    object_initialize_pool();
    projection = r_s32(0x800313F8);
    render_set_fog_far(10, projection);
    render_set_near_depth(100);
    render_set_far_depth(0x40000000);
    SetRCnt(0xF2000001, 0xFFFF, 0x2000);
    StartRCnt(0xF2000001);
    system_reset_counter_one();
    value = pad_update(0);
    w_u32(0x8004616C, value);
    memset(psx_addr(0x80040F20, 21), 0, 21);
    memset(psx_addr(0x80040F35, 21), 0, 21);
    game_initialize_particle_tables();
    game_initialize_curve_tables();
    callback = r_u32(0x800ECEB0);
    w_u32(0x800ECEA8, 0);
    psx_dispatch_guest_call(callback, 1, 0, 0, 0);
    disc_wait_stream();
    VSync(0);
    first = (uint32)(sint32)r_s16(0x800595FA);
    second = (uint32)(sint32)r_s16(0x800595F8);
    offset = (second * 7 + first - 1) * 4;
    value = (uint32)(sint32)r_s16(0x800F15F6 + offset);
    record = r_u32(0x800F2B08) + value * 24;
    sectors = r_u32(record + 0x14);
    asset = r_u32(record + 0x1C);
    stream_request_position(record + 0x10, sectors, asset);
    if (r_u32(0x800ECEA8) == 0) {
        do {
            callback = r_u32(0x800ECEB4);
            psx_dispatch_guest_call(callback, 0, 0, 0, 0);
        } while (r_u32(0x800ECEA8) == 0);
    }
    game_shutdown_scene();
    return r_s16(0x800ECEA8);
}

void game_initialize_particle_tables(void)
{
    uint32 storage;
    FUNCTION_MARKER(0x800AF6EC, "GAME.OVL");
    storage = game_get_buffer_f4f80();
    w_u32(0x80123F64, storage);
    game_clear_particle_records();
#ifndef NDEBUG
    printf("\ninit particle table done\n");
#endif
}

uint32 game_initialize_curve_tables(void)
{
    uint32 index, t, u, u_squared, t_squared, first, second, third, fourth;
    uint32 intensity, half, packed, mirrored;
    FUNCTION_MARKER(0x800CA100, "GAME.OVL");
    for (index = 0; index < 16; ++index) {
        t = index * 256 / 15;
        u = 256 - t;
        u_squared = (uint32)((sint32)(u * u) >> 8);
        t_squared = (uint32)((sint32)(t * t) >> 8);
        first = u * u_squared;
        second = u_squared * (t * 3);
        third = u * (t_squared * 3);
        fourth = t_squared * t;
        w_u32(0x800F80B8 + index * 4, (uint32)((sint32)first >> 8));
        w_u32(0x800F8108 + index * 4, (uint32)((sint32)second >> 8));
        w_u32(0x800F8150 + index * 4, (uint32)((sint32)third >> 8));
        w_u32(0x800F8190 + index * 4, (uint32)((sint32)fourth >> 8));
    }
    for (index = 0; index < 8; ++index) {
        intensity = (index + 1) * 255;
        intensity = (uint32)((sint32)intensity >> 5);
        if ((sint32)intensity >= 256)
            intensity = 255;
        mirrored = 15 - index;
        half = (uint32)((sint32)intensity >> 1);
        packed = (intensity << 8) + half;
        w_u32(0x8011A128 + index * 4, packed);
        w_u32(0x8011A128 + mirrored * 4, packed);
        w_u32(0x8011A168 + index * 4, half << 8);
        w_u32(0x8011A168 + mirrored * 4, half << 8);
    }
    w_u32(0x801079C8, 0);
    w_u32(0x801079CC, 0);
    w_u32(0x801079D0, 0);
    w_u32(0x801079F8, 0);
    w_u32(0x801079FC, 0);
    w_u32(0x80107A00, 0xF00);
    return 0xF00;
}

uint32 game_update_target_aim_statistics(uint32 object, uint32 target)
{
    uint32 x, y, z, component, value, first, second, angle, distance;
    uint32 object_yaw_delta, object_pitch_delta, camera_yaw_delta;
    FUNCTION_MARKER(0x80083320, "GAME.OVL");
    game_canonicalize_object_angles(object);
    x = r_u32(target + 8);
    component = r_u32(object + 8);
    y = r_u32(target + 12);
    x -= component;
    component = r_u32(object + 12);
    z = r_u32(target + 16);
    y -= component;
    component = r_u32(object + 16);
    w_u32(0x80109B44, x);
    w_u32(0x80109B48, y);
    z -= component;
    w_u32(0x80109B4C, z);
    distance = vector_length_approx(x, y, z);
    first = r_u32(0x80109B44);
    second = r_u32(0x80109B4C);
    w_u32(0x80046174, distance);
    angle = (uint32)ratan2((sint32)first, (sint32)second);
    first = r_u32(0x80109B48);
    second = r_u32(0x80109B4C);
    w_u32(0x80033D1C, angle);
    if ((sint32)second < 0)
        second = 0u - second;
    angle = (uint32)ratan2((sint32)(0u - first), (sint32)second);
    first = r_u32(0x80033D1C);
    second = r_u32(object + 0x20);
    w_u32(0x80033D30, angle);
    angle = (uint32)game_wrap_signed_angle(first - second);
    first = r_u32(0x80033D30);
    second = r_u32(object + 0x24);
    w_u32(0x80033D18, angle);
    angle = (uint32)game_wrap_signed_angle(first - second);
    first = r_u32(target + 0x20);
    w_u32(0x80033D20, angle);
    second = r_u32(0x80033D1C);
    angle = (uint32)game_wrap_signed_angle(second - (first - 2048));
    first = r_u32(0x80033D30);
    w_u32(0x80040C98, angle);
    second = r_u32(target + 0x24);
    angle = (uint32)game_wrap_signed_angle(0u - first - second);
    object_yaw_delta = r_u32(0x80033D18);
    w_u32(0x80040F1C, angle);
    if ((sint32)angle < 0)
        angle = 0u - angle;
    w_u32(0x80033D34, angle);
    object_pitch_delta = r_u32(0x80033D20);
    if ((sint32)object_yaw_delta < 0)
        object_yaw_delta = 0u - object_yaw_delta;
    w_u32(0x80052E6C, object_yaw_delta);
    camera_yaw_delta = r_u32(0x80040C98);
    value = object_pitch_delta;
    if ((sint32)value < 0)
        value = 0u - value;
    w_u32(0x80053674, value);
    if ((sint32)camera_yaw_delta < 0)
        camera_yaw_delta = 0u - camera_yaw_delta;
    w_u32(0x80033D24, camera_yaw_delta);
    return value;
}

uint32 game_transform_bounded_object_point(uint32 unused, uint32 object, uint32 output)
{
    VECTOR rotated;
    uint32 matrix, value, x, y, z, player;
    FUNCTION_MARKER(0x80085848, "GAME.OVL");
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 18));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    matrix = r_u32(0x80042174) + 0x11C;
    ApplyMatrixLV((MATRIX *)psx_addr(matrix, 20), (VECTOR *)psx_addr(object + 8, 12), &rotated);
    w_u32(output, (uint32)rotated.vx);
    w_u32(output + 4, (uint32)rotated.vy);
    w_u32(output + 8, (uint32)rotated.vz);
    value = r_u32(output);
    if ((sint32)value < 0)
        value = 0u - value;
    if ((sint32)value >= 16385)
        goto halve_point;
    value = r_u32(output + 4);
    if ((sint32)value < 0)
        value = 0u - value;
    if ((sint32)value >= 16385)
        goto halve_point;
    value = r_u32(output + 8);
    if ((sint32)value < 0)
        value = 0u - value;
    if ((sint32)value < 16385)
        goto transformed_point;
halve_point:
    y = r_u32(output + 4);
    x = r_u32(output);
    y = (uint32)((sint32)y >> 1);
    w_u32(output + 4, y);
    z = r_u32(output + 8);
    x = (uint32)((sint32)x >> 1);
    w_u32(output, x);
    value = r_u32(output);
    z = (uint32)((sint32)z >> 1);
    if ((sint32)value < 0)
        value = 0u - value;
    w_u32(output + 8, z);
    if ((sint32)value >= 16385)
        goto halve_point;
    value = r_u32(output + 4);
    if ((sint32)value < 0)
        value = 0u - value;
    if ((sint32)value >= 16385)
        goto halve_point;
    value = z;
    if ((sint32)value < 0)
        value = 0u - value;
    if ((sint32)value >= 16385)
        goto halve_point;
transformed_point:
    matrix = r_u32(0x80042174) + 0x11C;
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(matrix, 20), (VECTOR *)psx_addr(output, 12), (VECTOR *)psx_addr(output, 12));
    player = r_u32(0x80040FDC);
    value = r_u32(output);
    x = r_u32(player + 8);
    w_u32(output, value + x);
    value = r_u32(output + 4);
    y = r_u32(player + 12);
    w_u32(output + 4, value + y);
    value = r_u32(output + 8);
    z = r_u32(player + 16);
    value += z;
    w_u32(output + 8, value);
    return value;
}

void game_emit_slow_particles(uint32 count, uint32 x, uint32 y, uint32 z, uint32 vx, uint32 vy, uint32 vz)
{
    uint32 base_x, base_y, base_z, index, next, offset, storage, record;
    uint32 seed, first, second, third, value, quotient;
    FUNCTION_MARKER(0x800AFC0C, "GAME.OVL");
    if ((sint32)count <= 0)
        return;
    base_x = (uint32)((sint32)vx >> 5) - 16;
    base_y = (uint32)((sint32)vy >> 5) - 16;
    base_z = (uint32)((sint32)vz >> 5) - 16;
    do {
        --count;
        index = r_u32(0x800EF10C);
        storage = r_u32(0x80123F64);
        seed = r_u32(0x800EF50C);
        offset = index * 28;
        record = storage + offset;
        first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        value = (first & 31) + base_x;
        next = index + 1;
        second = (uint32)((sint32)first >> 1) ^ (first << 3);
        w_u32(0x800EF50C, first);
        w_u16(record + 2, value);
        value = (second & 31) + base_y;
        w_u16(record + 4, value);
        third = (uint32)((sint32)second >> 1) ^ (second << 3);
        w_u32(0x800EF50C, second);
        w_u32(0x800EF50C, third);
        value = (third & 31) + base_z;
        w_u16(record + 6, value);
        w_u32(record + 8, x);
        w_u32(record + 12, y);
        w_u32(record + 16, z);
        quotient = (uint32)((sint32)next / 450);
        w_u32(0x800EF10C, next - quotient * 450);
        w_u8(record, 2);
        storage = r_u32(0x80123F64);
        w_u32(record + 24, 32);
        w_u8(storage + offset + 1, 0);
    } while ((sint32)count > 0);
}

uint32 game_clear_particle_activity(void)
{
    uint32 index, storage;
    FUNCTION_MARKER(0x800AFF00, "GAME.OVL");
    for (index = 0; index < 450; ++index) {
        storage = r_u32(0x80123F64);
        w_u8(storage + index * 28 + 1, 0);
    }
    return 0;
}

uint32 game_append_flash_reference(uint32 reference)
{
    uint32 count;
    FUNCTION_MARKER(0x800BBC8C, "GAME.OVL");
    count = r_u32(0x80109B38);
    w_u32(0x800FB440 + count * 4, reference);
    ++count;
    w_u32(0x80109B38, count);
    return count;
}

uint32 game_draw_twisted_particle_trail(uint32 trail, uint32 color_shift)
{
    uint32 screen[10], depth[10], clip[10], width[10];
    uint32 old_phase, phase, reverse_phase, index, next, point, owner, camera;
    uint32 x, y, z, value, delta_x, packed, color, first, second, packet;
    uint32 pass, style, shift, first_angle, second_angle, product, coordinate;
    SVECTOR position;
    sint32 projection, projected_depth;
    FUNCTION_MARKER(0x800B2820, "GAME.OVL");
    old_phase = r_u32(trail + 0x128);
    w_u32(trail + 0x128, old_phase + 204);
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 18));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    owner = r_u32(trail + 0x120);
    color = 255u << (color_shift & 31);
    x = r_u32(owner + 8);
    y = r_u32(owner + 12);
    z = r_u32(owner + 16);
    game_emit_radial_effect(x, y, z, 1200, color);
    owner = r_u32(trail + 0x120);
    x = r_u32(owner + 8);
    y = r_u32(owner + 12);
    z = r_u32(owner + 16);
    game_emit_radial_effect(x, y, z, 600, 0xFFFFFF);
    for (index = 0; index < 10; ++index) {
        point = trail + index * 28;
        camera = r_u32(0x80040C94);
        first = r_u32(point);
        second = r_u32(camera + 8);
        delta_x = first - second;
        value = delta_x;
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)value > 0x1FFFF)
            goto clipped_trail_point;
        first = r_u32(point + 4);
        second = r_u32(camera + 12);
        value = first - second;
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)value > 0x1FFFF)
            goto clipped_trail_point;
        first = r_u32(point + 8);
        second = r_u32(camera + 16);
        value = first - second;
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)value > 0x1FFFF)
            goto clipped_trail_point;
        position.vx = (sint16)((sint32)delta_x >> 3);
        first = r_u32(point + 4);
        second = r_u32(camera + 12);
        position.vy = (sint16)((sint32)(first - second) >> 3);
        first = r_u32(point + 8);
        second = r_u32(camera + 16);
        position.vz = (sint16)((sint32)(first - second) >> 3);
        projected_depth = RotTransPers(&position, (sint32 *)&screen[index], &projection, (sint32 *)&clip[index]);
        depth[index] = (uint32)projected_depth;
        if (depth[index] == 0)
            depth[index] = 1;
        value = depth[index] << 2;
        if (value == 0)
            trap(7);
        packed = 32000u / value;
        depth[index] = value;
        width[index] = packed;
        continue;
clipped_trail_point:
        clip[index] = 0xFFFFFFFF;
    }
    phase = old_phase;
    reverse_phase = 0u - old_phase;
    for (index = 0; index < 10; ++index) {
        next = (index + 1) % 10;
        value = r_u32(trail + 0x11C);
        point = trail + index * 28;
        if (value == next || index == value)
            goto fade_trail_point;
        if (r_u32(point + 20) == 0)
            goto fade_trail_point;
        if ((clip[index] & 0xFFFF0000) != 0 || (clip[next] & 0xFFFF0000) != 0)
            goto fade_trail_point;
        for (pass = 0; pass < 8; ++pass) {
            style = pass & 3;
            shift = style < 2 ? 16 : 13;
            first_angle = pass < 4 ? phase + 1024 : reverse_phase;
            second_angle = pass < 4 ? phase + 1228 : reverse_phase - 204;
            packet = r_u32(0x80040690);
            if (style == 1 || style == 3)
                first = r_u32(point + 24);
            w_u32(0x80040690, packet + 36);
            if (style == 0) {
                w_u32(packet + 4, 0);
                first = r_u32(point + 24);
                w_u32(packet + 20, 0);
                color = first | (first << 8) | (first << 16);
                w_u32(packet + 12, color);
                first = r_u32(point + 24);
                second = r_u32(trail + next * 28 + 24);
                color = second | (first << 8) | (first << 16);
                w_u32(packet + 28, color);
            } else if (style == 1) {
                color = first | (first << 8) | (first << 16);
                w_u32(packet + 4, color);
                w_u32(packet + 12, 0);
                first = r_u32(point + 24);
                second = r_u32(trail + next * 28 + 24);
                w_u32(packet + 28, 0);
                color = second | (first << 8) | (first << 16);
                w_u32(packet + 20, color);
            } else if (style == 2) {
                w_u32(packet + 4, 0);
                first = r_u32(point + 24);
                w_u32(packet + 20, 0);
                w_u32(packet + 12, first << (color_shift & 31));
                second = r_u32(trail + next * 28 + 24);
                w_u32(packet + 28, second << (color_shift & 31));
            } else {
                w_u32(packet + 4, first << (color_shift & 31));
                w_u32(packet + 12, 0);
                second = r_u32(trail + next * 28 + 24);
                w_u32(packet + 28, 0);
                w_u32(packet + 20, second << (color_shift & 31));
            }
            if ((pass & 1) == 0) {
                product = (uint32)rcos((sint32)first_angle) * width[index];
                coordinate = (uint16)screen[index];
                value = (uint32)((sint32)product >> shift);
                w_u16(packet + 8, coordinate + value);
                product = (uint32)rsin((sint32)first_angle) * width[index];
                coordinate = (uint16)(screen[index] >> 16);
                value = (uint32)((sint32)product >> shift);
                w_u16(packet + 10, coordinate + value);
                w_u16(packet + 16, (uint16)screen[index]);
                w_u16(packet + 18, (uint16)(screen[index] >> 16));
                product = (uint32)rcos((sint32)second_angle) * width[next];
                coordinate = (uint16)screen[next];
                value = (uint32)((sint32)product >> shift);
                w_u16(packet + 24, coordinate + value);
                product = (uint32)rsin((sint32)second_angle) * width[next];
                coordinate = (uint16)(screen[next] >> 16);
                value = (uint32)((sint32)product >> shift);
                w_u16(packet + 26, coordinate + value);
                w_u16(packet + 32, (uint16)screen[next]);
                w_u16(packet + 34, (uint16)(screen[next] >> 16));
            } else {
                w_u16(packet + 8, (uint16)screen[index]);
                w_u16(packet + 10, (uint16)(screen[index] >> 16));
                product = (uint32)rcos((sint32)first_angle) * width[index];
                coordinate = (uint16)screen[index];
                value = (uint32)((sint32)product >> shift);
                w_u16(packet + 16, coordinate - value);
                product = (uint32)rsin((sint32)first_angle) * width[index];
                coordinate = (uint16)(screen[index] >> 16);
                value = (uint32)((sint32)product >> shift);
                w_u16(packet + 18, coordinate - value);
                w_u16(packet + 24, (uint16)screen[next]);
                w_u16(packet + 26, (uint16)(screen[next] >> 16));
                product = (uint32)rcos((sint32)second_angle) * width[next];
                coordinate = (uint16)screen[next];
                value = (uint32)((sint32)product >> shift);
                w_u16(packet + 32, coordinate - value);
                product = (uint32)rsin((sint32)second_angle) * width[next];
                coordinate = (uint16)(screen[next] >> 16);
                value = (uint32)((sint32)product >> shift);
                w_u16(packet + 34, coordinate - value);
            }
            SetPolyG4(psx_addr(packet, 36));
            SetSemiTrans(psx_addr(packet, 36), 1);
            render_insert_primitive((sint32)depth[index], packet);
        }
fade_trail_point:
        value = r_u32(point + 24) - 28;
        w_u32(point + 24, value);
        if ((sint32)value < 0)
            w_u32(point + 24, 0);
        reverse_phase -= 204;
        phase += 204;
    }
    return 0;
}

static void game_reticle_finish_line(uint32 packet)
{
    uint32 ordering;
    setLineF2((LINE_F2 *)psx_addr(packet, 16));
    packet = r_u32(0x80040690);
    SetSemiTrans(psx_addr(packet, 16), 1);
    ordering = r_u32(0x8004608C);
    packet = r_u32(0x80040690);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 16));
}

static uint32 game_reticle_advance_line(uint32 color, uint32 write_color)
{
    uint32 previous = r_u32(0x80040690);
    w_u32(0x80040690, previous + 16);
    if (write_color != 0)
        w_u32(previous + 20, color);
    return previous;
}

static void game_reticle_draw_elliptic_line(uint32 first_angle, uint32 second_angle, uint32 radius_mode, uint32 outer_x, uint32 outer_y, uint32 dynamic_phase)
{
    uint32 coordinate, angle, result, radius, product, center, packet;
    angle = dynamic_phase != 0 ? r_u32(0x800EF4A0) + first_angle : first_angle;
    for (coordinate = 0; coordinate < 4; ++coordinate) {
        result = (uint32)((coordinate & 1) == 0 ? rcos((sint32)angle) : rsin((sint32)angle));
        if (radius_mode == 1 || (radius_mode == 2 && coordinate >= 2)) {
            radius = (coordinate & 1) == 0 ? outer_x : outer_y;
        } else {
            radius = r_u32(0x800EF48C);
            if ((coordinate & 1) != 0)
                radius = (uint32)((sint32)radius >> 1);
        }
        product = radius * result;
        if (dynamic_phase != 0 && coordinate < 3)
            angle = r_u32(0x800EF4A0) + first_angle;
        else if (dynamic_phase == 0)
            angle = coordinate == 0 ? first_angle : second_angle;
        if (coordinate == 3) {
            packet = r_u32(0x80040690);
            center = r_u32(0x800F4F38);
        } else {
            center = r_u32((coordinate & 1) == 0 ? 0x800F4F34 : 0x800F4F38);
            packet = r_u32(0x80040690);
        }
        w_u16(packet + 8 + coordinate * 2, center + (uint32)((sint32)product >> 12));
    }
    game_reticle_finish_line(packet);
}

uint32 game_draw_target_lock_reticle(void)
{
    uint32 selected = r_u32(0x800EF4CC);
    uint32 target, state, value, second, radius, outer_x, outer_y;
    uint32 delta, limit, lock, phase, step, quarter, offset, first_angle, second_angle;
    uint32 packet, previous, x, y, color, line, timing, name;
    static const sint16 blink_lines[12][4] = {
        {-32, -12, -24, -16}, {-32, -12, -8, -4}, {-8, -4, -24, -16},
        {32, -12, 24, -16}, {32, -12, 8, -4}, {8, -4, 24, -16},
        {32, 12, 24, 16}, {8, 4, 24, 16}, {32, 12, 8, 4},
        {-32, 12, -24, 16}, {-8, 4, -24, 16}, {-32, 12, -8, 4}
    };
    static const sint16 target_lines[4][4] = {
        {-28, -4, -8, -14}, {28, -4, 8, -14},
        {-28, 4, -8, 14}, {28, 4, 8, 14}
    };
    FUNCTION_MARKER(0x800B43E0, "GAME.OVL");
    if (r_u32(0x80119C54 + selected * 52) == 15)
        return 15;
    target = r_u32(0x800EF4E0);
    if (target != 0) {
        state = r_u32(0x800EF4BC);
        if (state == 4)
            goto acquiring_lock;
        if (state == 5 || state == 6) {
            if (state != 5 || (r_u32(0x800311B8) & 1) == 0)
                goto target_marker;
            x = (uint32)(sint32)r_s16(target + 0x108);
            packet = r_u32(0x80040690);
            y = (uint32)(sint32)r_s16(target + 0x10A);
            w_u16(packet + 8, x - 32);
            w_u32(0x800F4F34, x);
            w_u32(0x800F4F38, y);
            w_u32(packet + 4, 255);
            w_u16(packet + 10, y - 12);
            w_u16(packet + 12, x - 24);
            w_u16(packet + 14, y - 16);
            game_reticle_finish_line(packet);
            for (line = 1; line < 12; ++line) {
                previous = game_reticle_advance_line(255, 1);
                x = r_u32(0x800F4F34);
                y = r_u32(0x800F4F38);
                w_u16(previous + 24, x + (uint32)(sint32)blink_lines[line][0]);
                w_u16(previous + 26, y + (uint32)(sint32)blink_lines[line][1]);
                w_u16(previous + 28, x + (uint32)(sint32)blink_lines[line][2]);
                w_u16(previous + 30, y + (uint32)(sint32)blink_lines[line][3]);
                game_reticle_finish_line(previous + 16);
            }
            game_reticle_advance_line(0, 0);
            goto target_marker;
        }
    }
    state = r_u32(0x800EF4BC);
    w_u32(0x800EF48C, 512);
    if (state == 5) {
        value = r_u32(0x800EC878);
        second = (uint32)(sint32)r_s16(0x80119C62 + value * 52);
        w_u32(0x800EF4BC, second == 1 ? 4 : 6);
    }
    goto target_marker;
acquiring_lock:
    x = (uint32)(sint32)r_s16(target + 0x108);
    y = (uint32)(sint32)r_s16(target + 0x10A);
    radius = r_u32(0x800EF48C);
    w_u32(0x800F4F34, x);
    w_u32(0x800F4F38, y);
    if ((sint32)radius >= 33) {
        radius -= (uint32)((sint32)radius >> 2);
        w_u32(0x800EF48C, radius);
        if ((sint32)radius < 33)
            w_u32(0x800EF48C, 32);
    }
    radius = r_u32(0x800EF48C);
    if (radius == 32) {
        value = r_u32(0x800F4F34);
        second = r_u32(0x80031198);
        delta = value - second;
        limit = r_u32(0x8003119C);
        if ((sint32)delta < 0)
            delta = 0u - delta;
        lock = (sint32)delta < ((sint32)limit >> 4);
        if (lock != 0) {
            value = r_u32(0x800F4F38);
            second = r_u32(0x800311B4);
            delta = value - second;
            limit = r_u32(0x800311B0);
            if ((sint32)delta < 0)
                delta = 0u - delta;
            lock = (sint32)delta < ((sint32)limit >> 4);
            if (lock != 0) {
                value = r_u32(0x800EF4E0);
                if (r_s32(value + 0x10C) < 10000) {
                    state = r_u32(0x800EF4BC);
                    if (state != 5)
                        sound_play_sample(17);
                    w_u32(0x800EF4BC, 5);
                }
            }
        }
        radius = r_u32(0x800EF48C);
    }
    outer_x = radius + (uint32)((sint32)radius >> 1);
    outer_y = (uint32)((sint32)outer_x >> 1);
    for (step = 256; step < 1024; step += 256) {
        for (quarter = 0; quarter < 4; ++quarter) {
            phase = r_u32(0x800EF4A0);
            offset = quarter == 3 ? 0xC0A : quarter * 1024;
            first_angle = phase + step + offset;
            second_angle = first_angle + 256;
            if (quarter == 0) {
                packet = r_u32(0x80040690);
                w_u32(packet + 4, 255);
            } else {
                game_reticle_advance_line(255, 1);
            }
            game_reticle_draw_elliptic_line(first_angle, second_angle, 0, outer_x, outer_y, 0);
            game_reticle_advance_line(255, 1);
            game_reticle_draw_elliptic_line(first_angle, second_angle, 1, outer_x, outer_y, 0);
        }
        previous = game_reticle_advance_line(0, 0);
    }
    w_u32(previous + 20, 255);
    for (quarter = 0; quarter < 4; ++quarter) {
        offset = quarter == 3 ? 0xC0A : quarter * 1024;
        if (quarter != 0)
            game_reticle_advance_line(255, 1);
        game_reticle_draw_elliptic_line(offset, offset, 2, outer_x, outer_y, 1);
    }
    game_reticle_advance_line(0, 0);
target_marker:
    target = r_u32(0x800EF4E0);
    if (target != 0) {
        w_u8(0x8004617C, 128);
        w_u8(0x80033D38, 128);
        w_u8(0x80053680, 128);
        w_u32(0x80033D2C, 242);
        value = r_u32(target);
        color = value == 1 ? 0xFF00 : 0xFF4000;
        x = (uint32)(sint32)r_s16(target + 0x108);
        packet = r_u32(0x80040690);
        y = (uint32)(sint32)r_s16(target + 0x10A);
        w_u16(packet + 8, x - 28);
        w_u32(0x800F4F34, x);
        w_u32(0x800F4F38, y);
        w_u32(packet + 4, color);
        w_u16(packet + 10, y - 4);
        w_u16(packet + 12, x - 8);
        w_u16(packet + 14, y - 14);
        game_reticle_finish_line(packet);
        for (line = 1; line < 4; ++line) {
            previous = game_reticle_advance_line(color, 1);
            x = r_u32(0x800F4F34);
            y = r_u32(0x800F4F38);
            w_u16(previous + 24, x + (uint32)(sint32)target_lines[line][0]);
            w_u16(previous + 26, y + (uint32)(sint32)target_lines[line][1]);
            w_u16(previous + 28, x + (uint32)(sint32)target_lines[line][2]);
            w_u16(previous + 30, y + (uint32)(sint32)target_lines[line][3]);
            game_reticle_finish_line(previous + 16);
        }
        packet = r_u32(0x80040690);
        x = r_u32(0x800F4F34);
        w_u32(0x80040690, packet + 16);
        y = r_u32(0x800F4F38);
        w_u32(0x80053688, x + 18);
        timing = r_u32(0x800311C0);
        y += 8;
        w_u32(0x8005368C, y);
        if (timing != 0)
            w_u32(0x8005368C, y + (uint32)((sint32)y >> 4));
        target = r_u32(0x800EF4E0);
        name = (uint32)(sint32)r_s16(target + 0x24E);
        font_draw_current(game_boundary_text_native(0x80059E70 + name * 44));
    }
    value = r_u32(0x800EF4A0) + 64;
    w_u32(0x800EF4A0, value);
    return value;
}

