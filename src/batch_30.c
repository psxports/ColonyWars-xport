#include "game_functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static sint32 game_allocate_arrival_warp_effect_body(uint32 position, uint32 matrix, const VECTOR *native_position, const MATRIX *native_matrix, uint32 object, int native_input)
{
    uint32 slot, offset, axis, column, row, element, value, last;
    slot = object == r_u32(0x80040FDC) ? 0 : (object == r_u32(0x8003FE74) ? 0 : 1);
    for (; slot < 2; ++slot) {
        offset = slot * 0x5C48;
        if (r_u32(0x8010ACD0 + offset) != 0)
            continue;
        for (axis = 0; axis < 3; ++axis) {
            value = native_input ? (axis == 0 ? (uint32)native_position->vx : axis == 1 ? (uint32)native_position->vy : (uint32)native_position->vz) : r_u32(position + axis * 4);
            w_u32(0x8010AD08 + offset + axis * 4, value);
        }
        for (column = 0; column < 3; ++column) {
            for (row = 0; row < 3; ++row) {
                element = column * 2 + row * 6;
                if (element == 16)
                    continue;
                value = native_input ? (uint16)native_matrix->m[row][column] : r_u16(matrix + element);
                w_u16(0x8010ACE8 + offset + element, (uint16)value);
            }
        }
        last = native_input ? (uint16)native_matrix->m[2][2] : r_u16(matrix + 16);
        w_u32(0x8010ACD0 + offset, 1);
        w_u32(0x8010ACE4 + offset, object);
        w_u32(0x8010ACDC + offset, 0);
        w_u32(0x8010ACE0 + offset, 0);
        w_u32(0x8010ACD4 + offset, 32);
        w_u16(0x8010ACF8 + offset, (uint16)last);
        for (row = 0; row < 16; ++row) {
            for (column = 0; column < 16; ++column) {
                w_u16(0x8010FD1C + offset + row * 128 + column * 8, 0);
                w_u32(0x80110518 + offset + row * 64 + column * 4, 0);
            }
        }
        return (sint32)slot;
    }
    return -1;
}

sint32 game_allocate_arrival_warp_effect(uint32 position, uint32 matrix, uint32 object)
{
    FUNCTION_MARKER(0x8008FEF0, "GAME.OVL");
    return game_allocate_arrival_warp_effect_body(position, matrix, NULL, NULL, object, 0);
}

sint32 game_allocate_arrival_warp_effect_native(const VECTOR *position, const MATRIX *matrix, uint32 object)
{
    return game_allocate_arrival_warp_effect_body(0, 0, position, matrix, object, 1);
}

static const char *game_jumpgate_text_native(uint32 address)
{
    uint32 length = 0;
    while (r_u8(address + length) != 0)
        ++length;
    return (const char *)psx_addr(address, (size_t)length + 1);
}

void game_update_jumpgate_ship_controller(uint32 object)
{
    uint32 state, index, offset, flags, ship, task_list, identifier, mx, my, mz, speed;
    uint32 x, y, z, px, py, pz, distance, count, ticks, callback, format, slot, bound;
    uint32 resources, table, base, sectors, asset, random;
    sint32 allocated;
    FUNCTION_MARKER(0x800A7CCC, "GAME.OVL");
    if (r_u32(object + 0x88) == 0) {
        allocated = game_allocate_arrival_warp_effect(object + 8, object + 0x11C, object);
        if (allocated >= 0) {
            w_u32(object + 0x88, 1);
            w_u32(object + 0xA8, (uint32)allocated);
            flags = r_u32(object + 0xB8);
            w_u32(0x8010ACE0 + (uint32)allocated * 0x5C48, flags);
        }
    }
    state = r_u32(object + 0x88);
    switch (state) {
    case 1:
        index = r_u32(object + 0xA8);
        w_u32(0x8010ACDC + index * 0x5C48, 1);
        index = r_u32(object + 0xA8);
        if (r_u32(0x8010ACD0 + index * 0x5C48) != 9)
            return;
        identifier = r_u32(object + 0xAC);
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        ship = game_spawn_script_ship(identifier, x, y, z, 0, 0, 0);
        index = r_u32(object + 0xA8);
        matrix_copy_rotation(ship + 0x11C, 0x8010ACE8 + index * 0x5C48);
        task_list = r_u32(object + 0xB0);
        if (task_list != 0xFFFFFFFF) {
            game_assign_ship_task_list(ship, task_list);
        } else {
            task_list = r_u32(object + 4);
            game_assign_ship_task_list(ship, task_list);
            flags = r_u32(ship + 0x114);
            w_u32(ship + 0x114, flags | 0x10000000);
        }
        if (r_u32(ship + 0x114) & 0x10) {
            base = r_u32(ship + 0x110);
            w_u8(base + 0x46F, 2);
        }
        identifier = r_u32(ship + 4);
        w_u32(ship + 0x40, 0x800A8400);
        mx = (uint32)(sint32)r_s16(ship + 0x120);
        speed = r_u32(0x8005369C + identifier * 60);
        my = (uint32)(sint32)r_s16(ship + 0x126);
        w_u32(ship + 0x18, my << 6);
        w_u32(ship + 0x14, mx << 6);
        mz = (uint32)(sint32)r_s16(ship + 0x12C);
        w_u32(ship + 0x9C, 60);
        w_u32(ship + 0xEC, speed << 1);
        w_u32(ship + 0xE8, speed << 1);
        w_u32(ship + 0x1C, mz << 6);
        w_u32(ship + 0xC0, r_u32(object + 0xB0) == 0xFFFFFFFF ? 1 : 0);
        slot = (uint32)(sint32)r_s16(ship + 0x24E);
        format = r_u32(0x800F23B8);
        sprintfPSX(0x800FA368, format, 0x80059E70 + slot * 44);
        game_queue_hud_message(0x800FA368);
        index = r_u32(object + 0xA8);
        w_u32(object + 0x170, ship);
        w_u32(0x8010ACE4 + index * 0x5C48, ship);
        index = r_u32(object + 0xA8);
        w_u32(0x8010ACD0 + index * 0x5C48, 6);
        index = r_u32(object + 0xA8);
        w_u32(0x8010ACD4 + index * 0x5C48, 255);
        w_u32(object + 0x88, 2);
        return;
    case 2:
        index = r_u32(object + 0xA8);
        w_u32(0x8010ACD0 + index * 0x5C48, 6);
        index = r_u32(object + 0xA8);
        w_u32(0x8010ACD4 + index * 0x5C48, 255);
        ship = r_u32(object + 0x170);
        game_draw_position_glow(ship + 8, 0x808080, 9000);
        index = r_u32(object + 0xA8);
        offset = index * 0x5C48;
        ship = r_u32(object + 0x170);
        px = r_u32(0x8010AD08 + offset);
        py = r_u32(0x8010AD0C + offset);
        pz = r_u32(0x8010AD10 + offset);
        x = r_u32(ship + 8);
        z = r_u32(ship + 16);
        y = r_u32(ship + 12);
        distance = (uint32)vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
        w_u32(0x80046174, distance);
        if ((sint32)distance < 20001)
            return;
        index = r_u32(object + 0xA8);
        w_u32(0x8010ACD0 + index * 0x5C48, 9);
        index = r_u32(object + 0xA8);
        w_u32(0x8010ACD4 + index * 0x5C48, 128);
        count = r_u32(object + 0xB4) - 1u;
        w_u32(object + 0xB4, count);
        if ((sint32)count > 0) {
            w_u32(object + 0x88, 4);
            w_u32(object + 0x9C, 60);
        } else {
            w_u32(object + 0x88, 3);
        }
        return;
    case 3:
        index = r_u32(object + 0xA8);
        offset = index * 0x5C48;
        if (r_u32(0x8010ACD0 + offset) != 0)
            return;
        w_u32(0x8010ACDC + offset, 0);
        if (r_u32(object + 0xB0) == 0xFFFFFFFF) {
            callback = r_u32(object + 0x48);
            psx_dispatch_guest_call(callback, 1, object, 0, 0);
            return;
        }
        identifier = r_u32(object + 0xAC);
        w_u32(object + 0x40, 0x8009AB58);
        if (r_u32(0x80053698 + identifier * 60) != 3 || r_u8(0x800EEAFC) != 0)
            return;
        if (game_count_active_standard_objects() == 0 || random_between(0, 100) >= 15)
            return;
        base = r_u32(0x80041004);
        if (r_u32(base) != 0)
            return;
        bound = r_u8(0x800EEAFD);
        random = (uint32)random_between(0, (sint32)bound - 1);
        index = r_u32(0x800ED460);
        table = r_u32(0x800EEB00);
        identifier = (uint32)(sint32)r_s16(table + index * 2);
        resources = r_u32(0x800F2B08);
        identifier += random;
        base = resources + identifier * 24;
        sectors = r_u32(base + 0x14);
        asset = r_u32(base + 0x1C);
        w_u32(0x8011F130, identifier);
        stream_request_segments(base + 0x10, sectors, asset, 0x800118FE, 1, 0, NULL);
        w_u8(0x800EEAFC, 1);
        return;
    case 4:
        ticks = r_u32(object + 0x9C) - 1u;
        w_u32(object + 0x9C, ticks);
        if ((sint32)ticks <= 0)
            w_u32(object + 0x88, 1);
        return;
    default:
        return;
    }
}

void game_update_ship_entry_placement(uint32 object)
{
    uint32 offsets[9], axis, index, target, state, special = 0, identifier, row, next;
    uint32 delta[3], length, numerator, value, other, flags, random, extent, payload;
    uint32 range, camera, position[3], ticks, bound, magnitude;
    VECTOR direction, up;
    FUNCTION_MARKER(0x800A853C, "GAME.OVL");
    for (index = 0; index < 9; ++index)
        offsets[index] = r_u32(0x80075DA0 + index * 4);
    value = r_u32(0x800595F8);
    target = r_u32(object + 0x170);
    if (value == 0x50002) {
        identifier = r_u32(object + 4);
        row = identifier * 60;
        if (r_s16(0x800536BC + row) == 2)
            special = strcmp(game_jumpgate_text_native(0x800536C6 + row), "NTITAN") == 0;
    }
    if (special) {
        delta[0] = r_u32(0x800ED3AC);
        delta[1] = r_u32(0x800ED3B0);
        delta[2] = r_u32(0x800ED3B4);
        length = (uint32)vector_length_approx((sint32)delta[0], (sint32)delta[1], (sint32)delta[2]);
        for (axis = 0; axis < 3; ++axis) {
            numerator = delta[axis] << 12;
            if (length == 0 || (numerator == 0x80000000 && length == 0xFFFFFFFF))
                abort();
            delta[axis] = (uint32)((sint32)numerator / (sint32)length);
        }
        for (axis = 0; axis < 3; ++axis) {
            if (axis == 2)
                w_u32(0x8011F130, length);
            value = r_u32(object + 8 + axis * 4);
            w_u32(object + 8 + axis * 4, value + delta[axis] * 1000u);
        }
        game_set_screen_tint(255, 255, 255);
        for (axis = 0; axis < 3; ++axis) {
            value = r_u32(object + 8 + axis * 4);
            other = r_u32(target + 8 + axis * 4);
            delta[axis] = value - other;
        }
        for (;;) {
            bound = 1;
            for (axis = 0; axis < 3; ++axis) {
                magnitude = (sint32)delta[axis] < 0 ? 0u - delta[axis] : delta[axis];
                if ((sint32)magnitude >= 16385) {
                    bound = 0;
                    break;
                }
            }
            if (bound)
                break;
            for (axis = 0; axis < 3; ++axis)
                delta[axis] = (uint32)((sint32)delta[axis] >> 1);
        }
        direction.vx = (sint32)delta[0];
        direction.vy = (sint32)delta[1];
        direction.vz = (sint32)delta[2];
        vector_normalize(&direction, &direction);
        up.vx = 0;
        up.vy = -4096;
        up.vz = 0;
        object_basis_from_vectors(&direction, &up, object);
        w_u32(object + 0x14, (0u - (uint32)direction.vx) << 11);
        w_u32(object + 0x18, (0u - (uint32)direction.vy) << 11);
        w_u32(object + 0x1C, (0u - (uint32)direction.vz) << 11);
        game_emit_model_breakup(object);
        value = r_u32(object + 0x48);
        psx_dispatch_guest_call(value, 1, object, 0, 0);
        return;
    }
    state = r_u32(object + 0x88);
    if (state == 0) {
        if (!(r_u32(object + 0x114) & 0x10)) {
            index = r_u32(0x800EEF10);
            value = r_u32(object + 8);
            w_u32(object + 8, value + offsets[index * 3]);
            value = r_u32(object + 12);
            other = offsets[index * 3 + 1];
            next = index + 1;
            w_u32(0x800EEF10, next);
            w_u32(object + 12, value + other);
            value = r_u32(object + 16);
            other = offsets[index * 3 + 2];
            w_u32(0x8011F130, index * 3);
            w_u32(object + 16, value + other);
            if ((sint32)next >= 3)
                w_u32(0x800EEF10, 0);
        }
        delta[0] = r_u32(0x800ED3AC);
        delta[1] = r_u32(0x800ED3B0);
        delta[2] = r_u32(0x800ED3B4);
        length = (uint32)vector_length_approx((sint32)delta[0], (sint32)delta[1], (sint32)delta[2]);
        for (axis = 0; axis < 3; ++axis) {
            numerator = delta[axis] << 12;
            if (length == 0 || (numerator == 0x80000000 && length == 0xFFFFFFFF))
                abort();
            delta[axis] = (uint32)((sint32)numerator / (sint32)length);
        }
        flags = r_u32(object + 0x114);
        w_u32(0x8011F130, length);
        random = r_u32(0x800EF50C);
        random = (uint32)((sint32)random >> 1) ^ (random << 3);
        extent = (random & 2047) + ((flags & 0x10) ? 3000u : 300u);
        w_u32(0x800EF50C, random);
        value = r_u32(object + 8);
        w_u32(object + 8, value + delta[0] * extent);
        value = r_u32(object + 12);
        w_u32(object + 12, value + delta[1] * extent);
        flags = r_u32(object + 0x114);
        value = r_u32(object + 16);
        w_u32(object + 16, value + delta[2] * extent);
        w_u32(0x8011F130, (flags & 0x10) ? 100000 : 10000);
        range = r_u32(0x8011F130);
        random = (uint32)random_between((sint32)(0u - range), (sint32)range);
        range = r_u32(0x8011F130);
        value = r_u32(object + 8);
        w_u32(object + 8, value + random);
        random = (uint32)random_between((sint32)(0u - range), (sint32)range);
        range = r_u32(0x8011F130);
        value = r_u32(object + 12);
        w_u32(object + 12, value + random);
        random = (uint32)random_between((sint32)(0u - range), (sint32)range);
        payload = r_u32(object + 0x6C);
        w_u32(object + 0x88, 1);
        w_u32(object + 0x3C, 0x800D1C3C);
        value = r_u32(object + 16);
        w_u32(object + 0xD8, 0x07FFFFFF);
        w_u32(object + 0x6C, 0);
        w_u32(object + 0xC4, 30);
        w_u32(object + 16, value + random);
        w_u32(object + 0xC0, payload);
        game_set_screen_tint(128, 128, 128);
        game_attach_object_trail(object, 2);
        game_evaluate_target_occlusion(object, target, 0);
        value = r_u32(0x80033D1C);
        other = r_u32(0x80033D30);
        w_u32(object + 0x20, value);
        w_u32(object + 0x24, other);
        return;
    }
    if (state != 1)
        return;
    ticks = r_u32(object + 0xC4);
    if ((sint32)ticks > 0) {
        camera = r_u32(0x80040C94);
        w_u32(object + 0xC4, ticks - 1u);
        for (axis = 0; axis < 3; ++axis) {
            value = r_u32(0x800ED3AC + axis * 4);
            other = r_u32(camera + 8 + axis * 4);
            position[axis] = value + other;
        }
        ticks = r_u32(object + 0xC4);
        game_draw_position_glow_native(position, 0xFFFFFF, ticks * 50u);
    }
    for (axis = 0; axis < 3; ++axis) {
        value = r_u32(object + 8 + axis * 4);
        other = r_u32(target + 8 + axis * 4);
        delta[axis] = value - other;
    }
    for (axis = 0; axis < 3; ++axis) {
        value = r_u32(object + 8 + axis * 4);
        w_u32(object + 8 + axis * 4, value - (uint32)((sint32)delta[axis] >> 4));
    }
    magnitude = (sint32)delta[0] < 0 ? 0u - delta[0] : delta[0];
    if ((sint32)magnitude >= 4096)
        return;
    flags = r_u32(object + 0x114);
    if (flags & 0x2000)
        w_u32(object + 0x114, flags ^ 0x2000);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0xEC, 0);
    w_u32(object + 0xFC, 0);
    w_u32(object + 0x14, 0);
    w_u32(object + 0x100, 0);
    w_u32(object + 0x18, 0);
    w_u32(object + 0x104, 0);
    w_u32(object + 0x1C, 0);
    w_u32(object + 0x40, 0x8009AB58);
    game_detach_object_trails(object);
}

void game_update_ship_timed_departure(uint32 object)
{
    uint32 state, tx, ty, tz, x, y, z, yaw, pitch, ay, ap, ticks, callback;
    FUNCTION_MARKER(0x800A8D44, "GAME.OVL");
    state = r_u32(object + 0x88);
    if (state == 0) {
        tx = r_u32(0x800ED3AC);
        ty = r_u32(0x800ED3B0);
        tz = r_u32(0x800ED3B4);
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        game_set_relative_target_angles(object, (tx << 16) - x, (ty << 16) - y, (tz << 16) - z);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u16(object + 0x250, (uint16)yaw);
        ay = (sint32)yaw < 0 ? 0u - yaw : yaw;
        w_u16(object + 0x252, (uint16)pitch);
        ap = (sint32)pitch < 0 ? 0u - pitch : pitch;
        if ((sint32)(ay + ap) < 2) {
            w_u32(object + 0x88, 1);
            w_u32(object + 0x9C, 180);
            w_u32(object + 0xE8, 0xC35000);
            w_u32(object + 0xF0, 70000);
        }
    } else if (state == 1) {
        ticks = r_u32(object + 0x9C) - 1u;
        w_u32(object + 0x9C, ticks);
        if ((sint32)ticks <= 0) {
            callback = r_u32(object + 0x48);
            psx_dispatch_guest_call(callback, 1, object, 0, 0);
            return;
        }
    }
    game_update_object_state(object);
    game_update_object_movement(object);
}

void game_replace_group_member_states(uint32 object, uint32 from_state, uint32 to_state)
{
    uint32 metadata, group, limit, index = 0, member, linked, offset, fresh;
    FUNCTION_MARKER(0x800A8E70, "GAME.OVL");
    metadata = r_u32(object + 0x210);
    group = r_u32(metadata);
    limit = r_u8(0x8011F053 + group * 100) + 1u;
    if ((sint32)limit <= 0)
        return;
    do {
        group = r_u32(metadata);
        offset = index * 8;
        member = r_u32(0x8011EFF0 + group * 100 + offset);
        linked = r_u32(member + 0x210);
        if (r_u8(linked + 0x28) == from_state) {
            w_u8(linked + 0x28, (uint8)to_state);
            fresh = r_u32(object + 0x210);
            group = r_u32(fresh);
            member = r_u32(0x8011EFF0 + group * 100 + offset);
            linked = r_u32(member + 0x210);
            w_u8(linked + 0x29, 2);
        }
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        limit = r_u8(0x8011F053 + group * 100) + 1u;
        ++index;
    } while ((sint32)index < (sint32)limit);
}

void game_update_ship_formation(uint32 object)
{
    uint32 metadata, leader, generation, group, row, count, member, member_data, selected = 0;
    uint32 index, cursor, destination, source, value, other, flags, callback, slot;
    uint32 offset_x = 0, offset_y = 0, offset_z = 0, damage, candidate, target, category;
    uint32 x, y, z, px, py, pz, mx, my, mz, dx, dy, dz, yaw, pitch, speed, distance;
    uint32 basis[9], desired_x, desired_y, desired_z, magnitude, step, phase;
    VECTOR input, output;
    FUNCTION_MARKER(0x800A8FA4, "GAME.OVL");
    metadata = r_u32(object + 0x210);
    leader = r_u32(metadata + 0x18);
    if (leader == 0)
        goto elect_leader;
    generation = r_u16(metadata + 0x1C);
    value = r_u16(leader + 0x24A);
    if (generation == value && r_u32(leader) != 0)
        goto leader_ready;
    if (r_u32(0x800F4F68) == leader)
        w_u32(0x800F4F68, 0);
    metadata = r_u32(object + 0x210);
    group = r_u32(metadata);
    count = r_u8(0x8011F053 + group * 100);
    if (count == 4) {
        game_replace_group_member_states(object, 1, 0);
        game_replace_group_member_states(object, 3, 1);
        game_replace_group_member_states(object, 4, 3);
    } else if (count == 3) {
        game_replace_group_member_states(object, 3, 0);
    } else if (count == 2) {
        game_replace_group_member_states(object, 2, 0);
    } else if (count == 1) {
        game_replace_group_member_states(object, 1, 0);
        w_u8(object + 0x270, 0);
    }
    for (index = 0; (sint32)index < (sint32)count; ++index) {
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        row = group * 100;
        member = r_u32(0x8011EFF0 + row + index * 8);
        member_data = r_u32(member + 0x210);
        if (r_u8(member_data + 0x28) != 0)
            continue;
        value = r_u8(0x8011F053 + row);
        w_u8(0x8011F053 + row, (uint8)(value - 1u));
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        value = r_u8(0x8011F053 + group * 100);
        selected = member;
        if ((sint32)index < (sint32)value) {
            cursor = index;
            do {
                destination = cursor * 8;
                ++cursor;
                group = r_u32(metadata);
                row = group * 100;
                source = cursor * 8;
                value = r_u32(0x8011EFF0 + row + source);
                w_u32(0x8011EFF0 + row + destination, value);
                metadata = r_u32(object + 0x210);
                group = r_u32(metadata);
                row = group * 100;
                value = r_u16(0x8011EFF4 + row + source);
                w_u16(0x8011EFF4 + row + destination, (uint16)value);
                metadata = r_u32(object + 0x210);
                group = r_u32(metadata);
                value = r_u8(0x8011F053 + group * 100);
            } while ((sint32)cursor < (sint32)value);
        }
    }
    metadata = r_u32(object + 0x210);
    group = r_u32(metadata);
    count = r_u8(0x8011F053 + group * 100);
    index = 0;
    while ((sint32)index < (sint32)count) {
        group = r_u32(metadata);
        member = r_u32(0x8011EFF0 + group * 100 + index * 8);
        member_data = r_u32(member + 0x210);
        w_u32(member_data + 0x18, selected);
        member_data = r_u32(member + 0x210);
        generation = r_u16(selected + 0x24A);
        w_u16(member_data + 0x1C, (uint16)generation);
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        member = r_u32(0x8011EFF0 + group * 100 + index * 8);
        member_data = r_u32(member + 0x210);
        w_u8(member_data + 0x29, 2);
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        count = r_u8(0x8011F053 + group * 100);
        ++index;
    }
    member_data = r_u32(selected + 0x210);
    w_u32(member_data + 0x18, selected);
    member_data = r_u32(selected + 0x210);
    generation = r_u16(selected + 0x24A);
    w_u16(member_data + 0x1C, (uint16)generation);
    member_data = r_u32(selected + 0x210);
    w_u32(member_data + 4, 0xFFFFFFFF);
    w_u8(selected + 0x26C, 1);
    w_u32(selected + 0x40, 0x8009AB58);
    game_begin_script_ship_task(selected);
leader_ready:
    metadata = r_u32(object + 0x210);
    leader = r_u32(metadata + 0x18);
    if (leader != 0)
        goto have_leader;
elect_leader:
    metadata = r_u32(object + 0x210);
    value = r_u8(0x800FD6FA);
    group = r_u32(metadata);
    if (value == group && (leader = r_u32(0x800F4F68)) != 0) {
        w_u32(metadata + 0x18, leader);
        metadata = r_u32(object + 0x210);
        leader = r_u32(metadata + 0x18);
        generation = r_u16(leader + 0x24A);
        w_u16(metadata + 0x1C, (uint16)generation);
        leader = r_u32(0x800F4F68);
        member_data = r_u32(leader + 0x210);
        w_u32(member_data + 4, 0xFFFFFFFF);
        leader = r_u32(0x800F4F68);
        if (leader == object) {
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            w_u8(0x8011F053 + group * 100, 0);
            w_u8(0x800FD6FA, 0);
            w_u32(0x800F4F68, 0);
            w_u16(0x800F4F6C, 0);
        }
    } else if (r_u8(0x800FD6FA) == 0) {
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        w_u32(0x800F4F68, object);
        w_u8(0x800FD6FA, (uint8)group);
    }
    goto check_leader;
have_leader:
    if (leader == object)
        goto lead_group;
    metadata = r_u32(object + 0x210);
    w_u32(object + 0x40, 0x8002493C);
    if (r_u8(metadata + 0x29) == 3) {
        flags = r_u32(object + 0x114);
        if (flags & 1)
            w_u32(object + 0x114, flags ^ 1);
        metadata = r_u32(object + 0x210);
        leader = r_u32(metadata + 0x18);
        mx = (uint32)(sint32)r_s16(leader + 0x120) * 40000u;
        my = (uint32)(sint32)r_s16(leader + 0x126) * 40000u;
        x = r_u32(object + 8);
        mz = (uint32)(sint32)r_s16(leader + 0x12C) * 40000u;
        px = r_u32(leader + 8);
        py = r_u32(leader + 12);
        pz = r_u32(leader + 16);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        game_set_relative_target_angles(object, px - x - (uint32)((sint32)mx >> 12), py - y - (uint32)((sint32)my >> 12), pz - z - (uint32)((sint32)mz >> 12));
        yaw = r_u32(0x80033D18);
        pitch = r_u32(0x80033D20);
        value = r_u32(object + 4);
        w_u16(object + 0x250, (uint16)yaw);
        w_u16(object + 0x252, (uint16)pitch);
        speed = r_u32(0x8005369C + value * 60);
        metadata = r_u32(object + 0x210);
        w_u32(object + 0xE8, speed);
        leader = r_u32(metadata + 0x18);
        value = r_u32(leader + 0xEC) << 1;
        if ((sint32)speed < (sint32)value)
            w_u32(object + 0xE8, value);
        game_update_object_state(object);
        game_update_object_movement(object);
        if (r_s32(0x80046174) < 10000) {
            metadata = r_u32(object + 0x210);
            w_u8(metadata + 0x29, 2);
        }
    }
    metadata = r_u32(object + 0x210);
    if (r_u8(metadata + 0x29) == 2) {
        flags = r_u32(object + 0x114);
        metadata = r_u32(object + 0x210);
        w_u32(object + 0x114, flags | 1);
        leader = r_u32(metadata + 0x18);
        mx = (uint32)(sint32)r_s16(leader + 0x120) * 10000u;
        my = (uint32)(sint32)r_s16(leader + 0x126) * 10000u;
        x = r_u32(object + 8);
        px = r_u32(leader + 8);
        mz = (uint32)(sint32)r_s16(leader + 0x12C) * 10000u;
        py = r_u32(leader + 12);
        pz = r_u32(leader + 16);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        dx = px - x - (uint32)((sint32)mx >> 12);
        dy = py - y - (uint32)((sint32)my >> 12);
        dz = pz - z - (uint32)((sint32)mz >> 12);
        slot = r_u8(metadata + 0x28);
        mx = (uint32)(sint32)r_s16(leader + 0x11E) * slot;
        my = (uint32)(sint32)r_s16(leader + 0x124) * slot;
        mz = (uint32)(sint32)r_s16(leader + 0x12A) * slot;
        if (slot & 1) {
            dx -= mx;
            dy -= my;
            dz -= mz;
        } else {
            dx += mx;
            dy += my;
            dz += mz;
        }
        game_set_relative_target_angles(object, dx, dy, dz);
        yaw = r_u32(0x80033D18);
        pitch = r_u32(0x80033D20);
        value = r_u32(object + 4);
        w_u16(object + 0x250, (uint16)yaw);
        w_u16(object + 0x252, (uint16)pitch);
        speed = r_u32(0x8005369C + value * 60);
        metadata = r_u32(object + 0x210);
        w_u32(object + 0xE8, speed);
        leader = r_u32(metadata + 0x18);
        value = r_u32(leader + 0xEC) << 1;
        if ((sint32)speed < (sint32)value)
            w_u32(object + 0xE8, value);
        game_update_object_state(object);
        game_update_object_movement(object);
    }
    metadata = r_u32(object + 0x210);
    if (r_u8(metadata + 0x28) == 0) {
        leader = r_u32(metadata + 0x18);
        member_data = r_u32(leader + 0x210);
        w_u32(member_data + 0x20, object);
        metadata = r_u32(object + 0x210);
        leader = r_u32(metadata + 0x18);
        member_data = r_u32(leader + 0x210);
        generation = r_u16(object + 0x24A);
        w_u16(member_data + 0x24, (uint16)generation);
    }
check_leader:
    metadata = r_u32(object + 0x210);
    if (r_u32(metadata + 0x18) != object)
        return;
lead_group:
    flags = r_u32(object + 0x114);
    callback = r_u32(object + 0x40);
    w_u32(object + 0x114, flags | 1);
    if (callback == 0x8002493C)
        w_u32(object + 0x40, 0x8009AB58);
    metadata = r_u32(object + 0x210);
    value = r_u8(0x800FD6FA);
    group = r_u32(metadata);
    if (value == group && r_u32(0x800F4F68) != object) {
        leader = r_u32(0x800F4F68);
        member_data = r_u32(leader + 0x210);
        w_u32(member_data + 0x18, object);
        leader = r_u32(0x800F4F68);
        member_data = r_u32(leader + 0x210);
        generation = r_u16(object + 0x24A);
        w_u8(0x800FD6FA, 0);
        w_u16(member_data + 0x1C, (uint16)generation);
        w_u32(0x800F4F68, 0);
        w_u16(0x800F4F6C, 0);
        metadata = r_u32(object + 0x210);
        w_u32(metadata + 4, 0xFFFFFFFF);
    }
    metadata = r_u32(object + 0x210);
    candidate = r_u32(metadata + 0x20);
    if (candidate != 0 && r_u32(candidate) != 0) {
        generation = r_u16(metadata + 0x24);
        value = r_u16(candidate + 0x24A);
        if (generation == value) {
            group = r_u32(metadata);
            row = group * 100;
            value = r_u8(0x8011F053 + row);
            w_u8(0x8011F053 + row, (uint8)(value + 1));
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            row = group * 100;
            count = r_u8(0x8011F053 + row);
            index = count - 1u;
            candidate = r_u32(metadata + 0x20);
            w_u32(0x8011EFF0 + row + index * 8, candidate);
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            generation = r_u16(metadata + 0x24);
            w_u16(0x8011EFF4 + group * 100 + index * 8, (uint16)generation);
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            candidate = r_u32(metadata + 0x20);
            value = r_u8(0x8011F053 + group * 100);
            member_data = r_u32(candidate + 0x210);
            w_u8(member_data + 0x28, (uint8)value);
            metadata = r_u32(object + 0x210);
            w_u32(metadata + 0x20, 0);
        }
    }
    target = r_u32(object + 0x1B0);
    damage = 0;
    if (target != 0) {
        metadata = r_u32(object + 0x210);
        value = r_u32(metadata + 4);
        if (value != 0xFFFFFFFF) {
            generation = (uint32)(sint32)r_s16(object + 0x1BC);
            if (value != generation) {
                category = r_u32(target);
                value = r_u32(object);
                if (category != value && !(category == 102 && value == 1))
                    damage = generation * category;
            }
        }
    }
    metadata = r_u32(object + 0x210);
    value = (uint32)(sint32)r_s16(object + 0x1BC);
    w_u32(metadata + 4, value);
    metadata = r_u32(object + 0x210);
    group = r_u32(metadata);
    count = r_u8(0x8011F053 + group * 100);
    index = 0;
    while ((sint32)index < (sint32)count) {
        group = r_u32(metadata);
        member = r_u32(0x8011EFF0 + group * 100 + index * 8);
        w_u8(member + 0x26C, 1);
        target = r_u32(object + 0x178);
        game_evaluate_target_occlusion(member, target, 0);
        target = r_u32(object + 0x178);
        if (target != 0 && r_u32(target) != 0 && r_s32(0x80046174) < 26000) {
            w_u32(member + 0x178, target);
            generation = r_u16(object + 0x17C);
            w_u16(member + 0x17C, (uint16)generation);
            game_update_target_weapons(object);
        }
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        generation = r_u16(member + 0x24A);
        value = r_u16(0x8011EFF4 + group * 100 + index * 8);
        if (generation != value || r_u32(member) == 0) {
            w_u32(metadata + 4, 0xFFFFFFFF);
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            row = group * 100;
            value = r_u8(0x8011F053 + row);
            w_u8(0x8011F053 + row, (uint8)(value - 1u));
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            value = r_u8(0x8011F053 + group * 100);
            if ((sint32)index < (sint32)value) {
                cursor = index;
                do {
                    destination = cursor * 8;
                    ++cursor;
                    group = r_u32(metadata);
                    row = group * 100;
                    source = cursor * 8;
                    value = r_u32(0x8011EFF0 + row + source);
                    w_u32(0x8011EFF0 + row + destination, value);
                    metadata = r_u32(object + 0x210);
                    group = r_u32(metadata);
                    row = group * 100;
                    value = r_u16(0x8011EFF4 + row + source);
                    w_u16(0x8011EFF4 + row + destination, (uint16)value);
                    metadata = r_u32(object + 0x210);
                    group = r_u32(metadata);
                    value = r_u8(0x8011F053 + group * 100);
                } while ((sint32)cursor < (sint32)value);
            }
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            count = r_u8(0x8011F053 + group * 100);
            if (count == 3) {
                member_data = r_u32(member + 0x210);
                if (r_u8(member_data + 0x28) == 1) {
                    game_replace_group_member_states(object, 3, 1);
                    game_replace_group_member_states(object, 4, 3);
                }
                member_data = r_u32(member + 0x210);
                if (r_u8(member_data + 0x28) == 2)
                    game_replace_group_member_states(object, 4, 2);
                member_data = r_u32(member + 0x210);
                if (r_u8(member_data + 0x28) == count)
                    game_replace_group_member_states(object, 4, 3);
            } else if (count == 2) {
                member_data = r_u32(member + 0x210);
                if (r_u8(member_data + 0x28) == 1)
                    game_replace_group_member_states(object, 3, 1);
                member_data = r_u32(member + 0x210);
                if (r_u8(member_data + 0x28) == 2)
                    game_replace_group_member_states(object, 3, 2);
            } else if (count == 1) {
                member_data = r_u32(member + 0x210);
                if (r_u8(member_data + 0x28) == 1)
                    game_replace_group_member_states(object, 2, 1);
            }
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            member = r_u32(0x8011EFF0 + group * 100 + index * 8);
        }
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        count = r_u8(0x8011F053 + group * 100);
        if (count >= 1 && count <= 4) {
            member_data = r_u32(member + 0x210);
            slot = r_u8(member_data + 0x28);
            if (count >= 2) {
                if (slot == 1) {
                    offset_x = (uint32)-5000;
                    offset_y = 3000;
                    offset_z = (uint32)-5000;
                }
                if (slot == 2) {
                    offset_x = 5000;
                    offset_y = 3000;
                    offset_z = (uint32)-5000;
                }
                if (count >= 3 && slot == 3) {
                    offset_x = count == 4 ? (uint32)-10000 : 0;
                    offset_y = 6000;
                    offset_z = (uint32)-10000;
                }
                if (count == 4 && slot == 4) {
                    offset_x = 10000;
                    offset_y = 6000;
                    offset_z = (uint32)-10000;
                }
            } else if (slot == 1) {
                offset_x = (uint32)-5000;
                offset_y = 3000;
                offset_z = (uint32)-5000;
            }
        }
        value = r_u32(object + 0xEC);
        step = (uint32)((sint32)value >> 9) << 3;
        phase = (r_u32(0x800311B8) + index * 32) & 511;
        offset_x += phase > 256 ? step : (uint32)-1024;
        phase = (r_u32(0x800311B8) + (index + 7) * 32) & 511;
        offset_y += phase > 256 ? step : (uint32)-512;
        phase = (r_u32(0x800311B8) + (index + 3) * 32) & 511;
        offset_z += phase > 256 ? step : 0u - step;
        x = r_u32(member + 8);
        px = r_u32(object + 8);
        y = r_u32(member + 12);
        py = r_u32(object + 12);
        pz = r_u32(object + 16);
        z = r_u32(member + 16);
        distance = (uint32)vector_length_approx((sint32)(x - px), (sint32)(y - py), (sint32)(z - pz));
        member_data = r_u32(member + 0x210);
        w_u32(0x80046174, distance);
        if (r_u8(member_data + 0x29) == 0) {
            if ((sint32)distance > 20000)
                w_u8(member_data + 0x29, 3);
            member_data = r_u32(member + 0x210);
            if (r_u8(member_data + 0x29) == 0 && r_s32(0x80046174) <= 20000)
                w_u8(member_data + 0x29, 2);
        }
        member_data = r_u32(member + 0x210);
        if (r_u8(member_data + 0x29) == 2 && r_s32(0x80046174) < 20000) {
            input.vx = (sint32)r_u32(member + 0xFC);
            input.vy = (sint32)r_u32(member + 0x100);
            input.vz = (sint32)r_u32(member + 0x104);
            game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11C, 18), &input, &output);
            member_data = r_u32(member + 0x210);
            w_u32(member_data + 8, (uint32)output.vx);
            member_data = r_u32(member + 0x210);
            w_u32(member_data + 12, (uint32)output.vy);
            member_data = r_u32(member + 0x210);
            w_u32(member_data + 16, (uint32)output.vz);
            member_data = r_u32(member + 0x210);
            w_u8(member_data + 0x29, 1);
        }
        member_data = r_u32(member + 0x210);
        if (r_u8(member_data + 0x29) == 1) {
            basis[0] = (uint32)(sint32)r_s16(object + 0x11C);
            basis[1] = (uint32)(sint32)r_s16(object + 0x11E);
            basis[2] = (uint32)(sint32)r_s16(object + 0x120);
            basis[3] = (uint32)(sint32)r_s16(object + 0x122);
            basis[4] = (uint32)(sint32)r_s16(object + 0x124);
            basis[5] = (uint32)(sint32)r_s16(object + 0x126);
            basis[6] = (uint32)(sint32)r_s16(object + 0x128);
            basis[7] = (uint32)(sint32)r_s16(object + 0x12A);
            basis[8] = (uint32)(sint32)r_s16(object + 0x12C);
            value = r_u32(object + 8);
            desired_x = value + (uint32)((sint32)(basis[0] * offset_x + basis[1] * offset_y + basis[2] * offset_z) >> 12);
            value = r_u32(object + 12);
            x = r_u32(member + 8);
            desired_y = value + (uint32)((sint32)(basis[3] * offset_x + basis[4] * offset_y + basis[5] * offset_z) >> 12);
            value = r_u32(object + 16);
            desired_z = value + (uint32)((sint32)(basis[6] * offset_x + basis[7] * offset_y + basis[8] * offset_z) >> 12);
            value = desired_x - x;
            magnitude = (sint32)value < 0 ? 0u - value : value;
            if ((sint32)magnitude >= 17) {
                value = r_u32(member_data + 8);
                w_u32(member_data + 8, value + ((sint32)x < (sint32)desired_x ? 8u : (uint32)-8));
            }
            y = r_u32(member + 12);
            value = desired_y - y;
            magnitude = (sint32)value < 0 ? 0u - value : value;
            if ((sint32)magnitude >= 17) {
                member_data = r_u32(member + 0x210);
                value = r_u32(member_data + 12);
                w_u32(member_data + 12, value + ((sint32)y < (sint32)desired_y ? 8u : (uint32)-8));
            }
            z = r_u32(member + 16);
            value = desired_z - z;
            magnitude = (sint32)value < 0 ? 0u - value : value;
            if ((sint32)magnitude >= 17) {
                member_data = r_u32(member + 0x210);
                value = r_u32(member_data + 16);
                w_u32(member_data + 16, value + ((sint32)z < (sint32)desired_z ? 8u : (uint32)-8));
            }
            member_data = r_u32(member + 0x210);
            value = r_u32(member_data + 8);
            if ((sint32)value < -265) {
                w_u32(member_data + 8, (uint32)-256);
                member_data = r_u32(member + 0x210);
            }
            value = r_u32(member_data + 8);
            if ((sint32)value > 256) {
                w_u32(member_data + 8, 256);
                member_data = r_u32(member + 0x210);
            }
            value = r_u32(member_data + 12);
            if ((sint32)value < -256)
                w_u32(member_data + 12, (uint32)-256);
            member_data = r_u32(member + 0x210);
            value = r_u32(member_data + 12);
            if ((sint32)value > 256) {
                w_u32(member_data + 12, 256);
                member_data = r_u32(member + 0x210);
            }
            value = r_u32(member_data + 16);
            if ((sint32)value < -256)
                w_u32(member_data + 16, (uint32)-256);
            member_data = r_u32(member + 0x210);
            value = r_u32(member_data + 16);
            if ((sint32)value > 256) {
                w_u32(member_data + 16, 256);
                member_data = r_u32(member + 0x210);
            }
            value = r_u32(member_data + 8);
            if ((sint32)value < 0)
                w_u32(member_data + 8, value + 2);
            member_data = r_u32(member + 0x210);
            value = r_u32(member_data + 8);
            if ((sint32)value > 0) {
                w_u32(member_data + 8, value - 2);
                member_data = r_u32(member + 0x210);
            }
            value = r_u32(member_data + 12);
            if ((sint32)value < 0) {
                w_u32(member_data + 12, value + 2);
                member_data = r_u32(member + 0x210);
                value = r_u32(member_data + 12);
            }
            if ((sint32)value > 0)
                w_u32(member_data + 12, value - 2);
            member_data = r_u32(member + 0x210);
            value = r_u32(member_data + 16);
            if ((sint32)value < 0) {
                w_u32(member_data + 16, value + 2);
                member_data = r_u32(member + 0x210);
                value = r_u32(member_data + 16);
            }
            if ((sint32)value > 0)
                w_u32(member_data + 16, value - 2);
            value = r_u32(object + 8);
            other = r_u32(member + 8);
            dx = (basis[2] << 3) + value - other;
            value = r_u32(object + 12);
            other = r_u32(member + 12);
            dy = (basis[5] << 3) + value - other;
            value = r_u32(object + 16);
            other = r_u32(member + 16);
            dz = (basis[8] << 3) + value - other;
            game_set_relative_target_angles(member, dx, dy, dz);
            yaw = r_u32(0x80033D18);
            pitch = r_u32(0x80033D20);
            w_u32(member + 0xEC, 0);
            w_u32(member + 0xE8, 0);
            w_u16(member + 0x250, (uint16)yaw);
            w_u16(member + 0x252, (uint16)pitch);
            game_update_object_state(member);
            member_data = r_u32(member + 0x210);
            value = r_u32(member_data + 8);
            other = r_u32(object + 0xFC);
            value += other;
            x = r_u32(member + 8);
            member_data = r_u32(member + 0x210);
            w_u32(member + 8, x + value);
            value = r_u32(member_data + 12);
            other = r_u32(object + 0x100);
            value += other;
            y = r_u32(member + 12);
            member_data = r_u32(member + 0x210);
            w_u32(member + 12, y + value);
            value = r_u32(member_data + 16);
            other = r_u32(object + 0x104);
            z = r_u32(member + 16);
            w_u32(member + 16, z + value + other);
            value = r_u32(object + 0xEC);
            w_u32(member + 0xEC, value);
            value = r_u16(object + 0x254);
            w_u16(member + 0x254, (uint16)value);
        }
        metadata = r_u32(object + 0x210);
        if (r_u32(metadata + 4) != 0xFFFFFFFF) {
            member_data = r_u32(member + 0x210);
            generation = (uint32)(sint32)r_s16(member + 0x1BC);
            value = r_u32(member_data + 4);
            if (value != generation) {
                target = r_u32(member + 0x1B0);
                category = r_u32(member);
                value = r_u32(target);
                if (value != category && !(value == 102 && category == 1))
                    damage += generation * value;
            }
        }
        member_data = r_u32(member + 0x210);
        value = (uint32)(sint32)r_s16(member + 0x1BC);
        w_u32(member_data + 4, value);
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        count = r_u8(0x8011F053 + group * 100);
        ++index;
    }
    if (damage != 0 || r_u8(object + 0x270) == 255) {
        if (object != 0 && r_u32(object) != 0) {
            flags = r_u32(object + 0x114);
            w_u8(object + 0x26C, 1);
            w_u8(object + 0x270, 0);
            if (flags & 1)
                w_u32(object + 0x114, flags ^ 1);
            w_u32(object + 0x40, 0x8009AB58);
            game_begin_script_ship_task(object);
        }
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        count = r_u8(0x8011F053 + group * 100);
        index = 0;
        while ((sint32)index < (sint32)count) {
            group = r_u32(metadata);
            member = r_u32(0x8011EFF0 + group * 100 + index * 8);
            if (member != 0 && r_u32(member) != 0) {
                flags = r_u32(member + 0x114);
                w_u8(member + 0x270, 0);
                if (flags & 1)
                    w_u32(member + 0x114, flags ^ 1);
                w_u32(member + 0x40, 0x8009AB58);
                game_begin_script_ship_task(member);
            }
            metadata = r_u32(object + 0x210);
            group = r_u32(metadata);
            count = r_u8(0x8011F053 + group * 100);
            ++index;
        }
        metadata = r_u32(object + 0x210);
        group = r_u32(metadata);
        w_u8(0x8011F053 + group * 100, 0);
    }
    metadata = r_u32(object + 0x210);
    if (r_u32(metadata + 4) == 0xFFFFFFFF)
        w_u32(metadata + 4, 0);
}

void game_spawn_approaching_asteroid(uint32 controller)
{
    uint32 models[2], chance, probability, count, limit, choice, player, effect, payload;
    uint32 radius, azimuth, elevation, value, component, x, y, z, px, py, pz, distance;
    uint32 yaw, pitch, speed, next_payload;
    VECTOR relative, local;
    MATRIX rotated = {0};
    FUNCTION_MARKER(0x800AA91C, "GAME.OVL");
    models[0] = r_u32(0x800EDFCC);
    models[1] = r_u32(0x800EDFD0);
    chance = (uint32)random_between(0, 100);
    probability = r_u32(controller + 0xAC);
    if ((sint32)chance >= (sint32)probability)
        return;
    count = r_u32(0x800595CC);
    limit = r_u32(controller + 0xA8);
    if ((sint32)count >= (sint32)limit || (sint32)count >= 80)
        return;
    choice = (uint32)random_between(0, 1);
    player = r_u32(0x80040FDC);
    effect = game_create_model_effect(player + 8, (sint32)(sint16)(uint16)models[choice]);
    payload = r_u32(effect + 0x6C);
    w_u16(effect + 0x3C, 100);
    w_u32(payload, 0x800EEEF0);
    radius = (uint32)random_signed_between(175000, 250000);
    w_u32(0x80046174, radius);
    azimuth = (uint32)random_between(0, 4095);
    elevation = (uint32)random_between(0, 4095);
    radius = r_u32(0x80046174);
    component = (uint32)math_scale_sin(radius, azimuth);
    elevation = 0u - elevation;
    component = (uint32)math_scale_cos(component, elevation);
    value = r_u32(effect);
    radius = r_u32(0x80046174);
    w_u32(effect, value + component);
    component = (uint32)math_scale_cos(radius, azimuth);
    component = (uint32)math_scale_cos(component, elevation);
    value = r_u32(effect + 4);
    radius = r_u32(0x80046174);
    w_u32(effect + 4, value + component);
    component = (uint32)math_scale_sin(radius, elevation);
    value = r_u32(effect + 8);
    w_u32(effect + 8, value + component);
    value = (uint32)random_signed_between(3, 24);
    payload = r_u32(effect + 0x6C);
    w_u16(payload + 0x34, (uint16)value);
    value = (uint32)random_signed_between(3, 24);
    payload = r_u32(effect + 0x6C);
    w_u16(payload + 0x36, (uint16)value);
    value = (uint32)random_signed_between(3, 24);
    payload = r_u32(effect + 0x6C);
    player = r_u32(0x80040FDC);
    w_u16(payload + 0x38, (uint16)value);
    px = r_u32(player + 8);
    x = r_u32(effect);
    py = r_u32(player + 12);
    y = r_u32(effect + 4);
    pz = r_u32(player + 16);
    z = r_u32(effect + 8);
    relative.vx = (sint32)(px - x);
    relative.vy = (sint32)(py - y);
    relative.vz = (sint32)(pz - z);
    distance = (uint32)vector_length_approx(relative.vx, relative.vy, relative.vz);
    payload = r_u32(effect + 0x6C);
    w_u32(0x80046174, distance);
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(payload + 4, 18), &relative, &local);
    yaw = (uint32)ratan2(local.vx, local.vz);
    y = (uint32)local.vy;
    z = (uint32)local.vz;
    w_u32(0x80033D1C, yaw);
    if ((sint32)z < 0)
        z = 0u - z;
    pitch = (uint32)ratan2((sint32)(0u - y), (sint32)z);
    payload = r_u32(effect + 0x6C);
    yaw = r_u32(0x80033D1C);
    w_u32(0x80033D30, pitch);
    matrix_rotate_y(&rotated, (MATRIX *)psx_addr(payload + 4, 20), 0u - yaw);
    payload = r_u32(effect + 0x6C);
    pitch = r_u32(0x80033D30);
    matrix_rotate_x((MATRIX *)psx_addr(payload + 4, 20), &rotated, pitch);
    w_u32(effect + 0x68, 0x800EEC8C);
    speed = (uint32)random_between(400, 600);
    payload = r_u32(effect + 0x6C);
    component = (uint32)(sint32)r_s16(payload + 8);
    next_payload = r_u32(effect + 0x6C);
    w_u32(effect + 0x20, (uint32)((sint32)(component * speed) >> 12));
    component = (uint32)(sint32)r_s16(next_payload + 0xE);
    next_payload = r_u32(effect + 0x6C);
    w_u32(effect + 0x24, (uint32)((sint32)(component * speed) >> 12));
    component = (uint32)(sint32)r_s16(next_payload + 0x14);
    w_u32(effect + 0x28, (uint32)((sint32)(component * speed) >> 12));
    value = (uint32)random_between(-3, 0);
    w_u16(effect + 0x34, (uint16)value);
    w_u32(effect + 0x60, 0x800B2520);
    w_u32(effect + 0x2C, 450);
    w_u32(effect + 0x30, 0x07FFFFFF);
    count = r_u32(0x800595CC);
    w_u32(0x800595CC, count + 1);
}

void game_update_ship_random_point(uint32 object)
{
    uint32 metadata, random, base, x, y, z, tx, ty, tz, yaw, pitch, magnitude, roll;
    uint32 distance, identifier, speed, previous, ticks;
    FUNCTION_MARKER(0x800AAE44, "GAME.OVL");
    metadata = r_u32(object + 0x200);
    if (r_u32(metadata + 0x18) == 0) {
        random = (uint32)random_signed_between(1, 25000);
        base = r_u32(object + 0x214);
        metadata = r_u32(object + 0x200);
        w_u32(metadata + 0x18, base + random);
        random = (uint32)random_signed_between(1, 25000);
        base = r_u32(object + 0x218);
        metadata = r_u32(object + 0x200);
        w_u32(metadata + 0x1C, base + random);
        random = (uint32)random_signed_between(1, 25000);
        base = r_u32(object + 0x21C);
        metadata = r_u32(object + 0x200);
        w_u32(metadata + 0x20, base + random);
    }
    metadata = r_u32(object + 0x200);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    tx = r_u32(metadata + 0x18);
    z = r_u32(object + 16);
    ty = r_u32(metadata + 0x1C);
    tz = r_u32(metadata + 0x20);
    game_set_relative_target_angles(object, tx - x, ty - y, tz - z);
    yaw = r_u32(0x80033D1C);
    pitch = r_u32(0x80033D30);
    w_u16(object + 0x250, (uint16)yaw);
    yaw = (uint32)(sint32)r_s16(object + 0x250);
    magnitude = (sint32)yaw < 0 ? 0u - yaw : yaw;
    w_u16(object + 0x252, (uint16)pitch);
    roll = 0;
    if ((sint32)magnitude < 32) {
        pitch = (uint32)(sint32)(sint16)(uint16)pitch;
        magnitude = (sint32)pitch < 0 ? 0u - pitch : pitch;
        if ((sint32)magnitude < 32)
            roll = 32;
    }
    w_u16(object + 0x254, (uint16)roll);
    distance = r_u32(0x80046174);
    identifier = r_u32(object + 4);
    speed = r_u32(0x8005369C + identifier * 60);
    if ((sint32)distance <= 10000)
        speed = (uint32)((sint32)(distance * speed) / 10000);
    w_u32(object + 0xE8, speed);
    metadata = r_u32(object + 0x200);
    distance = r_u32(0x80046174);
    previous = r_u32(metadata + 0x38);
    if ((sint32)previous < (sint32)distance && previous != 0)
        w_u32(metadata + 0x18, 0);
    metadata = r_u32(object + 0x200);
    distance = r_u32(0x80046174);
    w_u32(metadata + 0x38, distance);
    game_update_object_state(object);
    game_update_object_movement(object);
    ticks = r_u32(object + 0x9C) - 1u;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks <= 0)
        w_u32(object + 0x40, 0x8009AB58);
}

void game_update_warphole_lifetime(uint32 object)
{
    uint32 ticks = r_u32(0x800ED394);
    uint32 message, identifier, counter, callback;
    FUNCTION_MARKER(0x800AB06C, "GAME.OVL");
    --ticks;
    w_u32(0x800ED394, ticks);
    if ((sint32)ticks > 0)
        return;
    message = r_u32(0x800F23BC);
    game_queue_hud_message(message);
    identifier = r_u32(object + 4);
    counter = 0x800536BE + identifier * 60;
    w_u16(counter, (uint16)(r_u16(counter) + 1));
    callback = r_u32(object + 0x48);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}

void game_draw_large_warp_glow(uint32 position, uint32 radius, uint32 color, uint32 phase)
{
    FUNCTION_MARKER(0x800AB878, "GAME.OVL");
    game_draw_position_glow(position, color, 200000);
}

void game_draw_large_warp_glow_native(const VECTOR *position, uint32 radius, uint32 color, uint32 phase)
{
    uint32 point[3];
    point[0] = (uint32)position->vx;
    point[1] = (uint32)position->vy;
    point[2] = (uint32)position->vz;
    game_draw_position_glow_native(point, color, 200000);
}

void game_update_drifting_object(uint32 object)
{
    uint32 ax, ay, az, vx, vy, vz, x, y, z, ticks, axis, value, target, age, callback;
    FUNCTION_MARKER(0x800AB8A0, "GAME.OVL");
    ax = r_u32(object + 0xB4);
    ay = r_u32(object + 0xB8);
    az = r_u32(object + 0xBC);
    vy = r_u32(object + 0x100);
    w_u32(object + 0x20, ax);
    x = r_u32(object + 8);
    w_u32(object + 0x28, az);
    vx = r_u32(object + 0xFC);
    w_u32(object + 0x24, ay);
    y = r_u32(object + 12);
    w_u32(object + 8, x + vx);
    z = r_u32(object + 16);
    vz = r_u32(object + 0x104);
    w_u32(object + 12, y + vy);
    ticks = r_u32(object + 0xC0) - 1u;
    w_u32(object + 16, z + vz);
    w_u32(object + 0xC0, ticks);
    if ((sint32)ticks <= 0) {
        for (axis = 0; axis < 3; ++axis) {
            value = r_u32(object + 0xB4 + axis * 4);
            target = r_u32(object + 0xA8 + axis * 4);
            if (value != target)
                w_u32(object + 0xB4 + axis * 4, value + ((sint32)target < 0 ? (uint32)-1 : 1u));
        }
        w_u32(object + 0xC0, 120);
    }
    ticks = r_u32(object + 0xC4) - 1u;
    w_u32(object + 0xC4, ticks);
    if ((sint32)ticks <= 0) {
        ax = (uint32)(sint32)r_s16(object + 0x224);
        vx = r_u32(object + 0xFC);
        az = (uint32)(sint32)r_s16(object + 0x228);
        w_u32(object + 0xC4, 20);
        vz = r_u32(object + 0x104);
        ay = (uint32)(sint32)r_s16(object + 0x226);
        w_u32(object + 0xFC, vx + ax);
        vy = r_u32(object + 0x100);
        w_u32(object + 0x104, vz + az);
        w_u32(object + 0x100, vy + ay);
    }
    age = r_u32(object + 0x9C) + 1u;
    w_u32(object + 0x9C, age);
    if ((sint32)age >= 1350) {
        callback = r_u32(object + 0x48);
        w_u32(object + 0x234, 0);
        w_u32(object + 0x238, 0);
        w_u32(object + 0x104, 0);
        w_u32(object + 0x100, 0);
        w_u32(object + 0xFC, 0);
        w_u32(object + 0x1C, 0);
        w_u32(object + 0x18, 0);
        w_u32(object + 0x14, 0);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
    }
}

void game_update_warphole_impact(uint32 object)
{
    const uint32 colors[4] = {0xA0A000, 0x808000, 0x606000, 0x303000};
    const uint32 glows[4] = {0xFFFFFF, 0xC0C0C0, 0x808080, 0x404040};
    uint32 target, state, value, other, ticks, vx, vy, vz, x, y, z, ax, ay, az, player;
    uint32 px, py, pz, distance, previous, listener, seed, first, second, third, scale;
    uint32 iteration, velocities[3], radius, phase, callback;
    sint32 denominator, quotient;
    VECTOR point, difference;
    FUNCTION_MARKER(0x800AB270, "GAME.OVL");
    target = r_u32(object + 0x178);
    if (r_u32(target) == 0) {
        callback = r_u32(object + 0x48);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
        return;
    }
    state = r_u32(object + 0x88);
    if (state == 1) {
        value = r_u32(object + 0xA8);
        if ((sint32)value < 11)
            w_u32(object + 0xA8, value + 1);
        ticks = r_u32(object + 0x9C) - 1u;
        w_u32(object + 0x9C, ticks);
        if ((sint32)ticks <= 0)
            w_u32(object + 0x88, 2);
    } else if (state == 2) {
        vx = r_u32(object + 0xFC);
        x = r_u32(object + 8);
        ax = r_u32(object + 0x214);
        ay = r_u32(object + 0x218);
        player = r_u32(0x80040FDC);
        x += (uint32)((sint32)vx >> 3);
        vy = r_u32(object + 0x100);
        w_u32(object + 8, x);
        y = r_u32(object + 12);
        y += (uint32)((sint32)vy >> 3);
        vz = r_u32(object + 0x104);
        w_u32(object + 12, y);
        z = r_u32(object + 16);
        z += (uint32)((sint32)vz >> 3);
        vx = r_u32(object + 0xFC);
        w_u32(object + 16, z);
        vy = r_u32(object + 0x100);
        w_u32(object + 0xFC, vx + ax);
        vz = r_u32(object + 0x104);
        w_u32(object + 0x100, vy + ay);
        az = r_u32(object + 0x21C);
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        w_u32(object + 0x104, vz + az);
        px = r_u32(player + 8);
        py = r_u32(player + 12);
        pz = r_u32(player + 16);
        z = r_u32(object + 16);
        distance = (uint32)vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
        denominator = (sint32)distance >> 4;
        if (denominator == 0)
            abort();
        quotient = 250000 / denominator;
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        w_u32(0x80046174, distance);
        w_u32(object + 0xA8, (uint32)quotient + 11);
        px = r_u32(target + 8);
        x = r_u32(object + 8);
        pz = r_u32(target + 16);
        py = r_u32(target + 12);
        distance = (uint32)vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
        previous = r_u32(object + 0xB8);
        w_u32(0x80046174, distance);
        if ((sint32)previous < (sint32)distance) {
            game_get_target_vertex_vector_native(target, object, &point);
            ApplyMatrixLV((MATRIX *)psx_addr(target + 0x11C, 20), &point, &point);
            point.vx = (sint32)((uint32)point.vx + r_u32(target + 8));
            point.vy = (sint32)((uint32)point.vy + r_u32(target + 12));
            point.vz = (sint32)((uint32)point.vz + r_u32(target + 16));
            listener = r_u32(0x80040C94);
            sound_play_at_listener_native(&point, 4, 0, listener);
            seed = r_u32(0x800EF50C);
            seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
            scale = r_u32(target + 0x84);
            w_u32(0x800EF50C, seed);
            game_emit_sized_fragment_flash_native(&point, ((seed & 127) + 384) << (scale & 31));
            game_emit_projectile_impact((uint32)point.vx, (uint32)point.vy, (uint32)point.vz);
            for (iteration = 0; iteration < 32; ++iteration) {
                seed = r_u32(0x800EF50C);
                first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
                velocities[0] = ((first & 4095) - 2048u) << 7;
                second = (uint32)((sint32)first >> 1) ^ (first << 3);
                velocities[1] = ((second & 4095) - 2048u) << 7;
                third = (uint32)((sint32)second >> 1) ^ (second << 3);
                velocities[2] = ((third & 4095) - 2048u) << 7;
                w_u32(0x800EF50C, first);
                w_u32(0x800EF50C, second);
                w_u32(0x800EF50C, third);
                game_emit_wide_particles(2, (uint32)point.vx, (uint32)point.vy, (uint32)point.vz, velocities[0], velocities[1], velocities[2]);
                seed = r_u32(0x800EF50C);
                first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
                velocities[0] = ((first & 4095) - 2048u) << 7;
                second = (uint32)((sint32)first >> 1) ^ (first << 3);
                velocities[1] = ((second & 4095) - 2048u) << 7;
                third = (uint32)((sint32)second >> 1) ^ (second << 3);
                velocities[2] = ((third & 4095) - 2048u) << 7;
                w_u32(0x800EF50C, first);
                w_u32(0x800EF50C, second);
                w_u32(0x800EF50C, third);
                game_emit_narrow_particles(2, (uint32)point.vx, (uint32)point.vy, (uint32)point.vz, velocities[0], velocities[1], velocities[2]);
            }
            game_create_moonbase_grid_controller(target);
            value = r_u32(object + 8);
            other = r_u32(target + 8);
            difference.vx = (sint32)(value - other);
            value = r_u32(object + 12);
            other = r_u32(target + 12);
            difference.vy = (sint32)(value - other);
            value = r_u32(object + 16);
            other = r_u32(target + 16);
            difference.vz = (sint32)(value - other);
            game_handle_ship_hit_native(target, &point, &difference, 0, 0, 1, 0x800EEC6C);
            callback = r_u32(object + 0x48);
            psx_dispatch_guest_call(callback, 1, object, 0, 0);
            return;
        }
        w_u32(object + 0xB8, distance);
    }
    value = r_u32(object + 0xAC);
    other = r_u32(object + 0xB0);
    scale = r_u32(object + 0xB0);
    w_u32(object + 0xAC, value + other);
    if ((sint32)scale < 320)
        w_u32(object + 0xB0, scale + 1);
    point.vx = (sint32)r_u32(object + 8);
    point.vy = (sint32)r_u32(object + 12);
    point.vz = (sint32)r_u32(object + 16);
    for (iteration = 0; iteration < 4; ++iteration) {
        if (iteration != 0) {
            value = r_u32(object + 0xFC);
            point.vx = (sint32)((uint32)point.vx - (uint32)((sint32)value >> 4));
            value = r_u32(object + 0x100);
            point.vy = (sint32)((uint32)point.vy - (uint32)((sint32)value >> 4));
            value = r_u32(object + 0x104);
            point.vz = (sint32)((uint32)point.vz - (uint32)((sint32)value >> 4));
        }
        radius = r_u32(object + 0xA8);
        game_draw_radial_glow_native(&point, radius, colors[iteration], 0);
        radius = r_u32(object + 0xA8);
        phase = r_u32(object + 0xAC);
        radius = (uint32)((sint32)(radius * 5u) >> 2);
        game_draw_large_warp_glow_native(&point, radius, glows[iteration], phase);
    }
}

void game_update_target_beam_sequence(uint32 object)
{
    uint32 index, target, x, y, z, other, magnitude, ticks, phase, scale, shift, value, callback;
    uint32 a, b, c, draw_first;
    VECTOR direction, up, forward, point, difference;
    SVECTOR offset = {0};
    FUNCTION_MARKER(0x800ABA30, "GAME.OVL");
    index = r_u32(object + 0xBC);
    target = r_u32(0x801076F8 + index * 36);
    other = r_u32(object + 8);
    x = r_u32(target + 8) - other;
    y = r_u32(target + 12);
    other = r_u32(object + 12);
    y -= other;
    z = r_u32(target + 16);
    other = r_u32(object + 16);
    z -= other;
    for (;;) {
        magnitude = (sint32)x < 0 ? 0u - x : x;
        if ((sint32)magnitude < 16385) {
            magnitude = (sint32)y < 0 ? 0u - y : y;
            if ((sint32)magnitude < 16385) {
                magnitude = (sint32)z < 0 ? 0u - z : z;
                if ((sint32)magnitude < 16385)
                    break;
            }
        }
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
    }
    direction.vx = (sint32)x;
    direction.vy = (sint32)y;
    direction.vz = (sint32)z;
    vector_normalize(&direction, &direction);
    up.vx = (sint32)(0u - (uint32)(sint32)r_s16(object + 0x11E));
    up.vy = (sint32)(0u - (uint32)(sint32)r_s16(object + 0x124));
    up.vz = (sint32)(0u - (uint32)(sint32)r_s16(object + 0x12A));
    a = (uint32)(sint32)r_s16(object + 0x120);
    x = a + (uint32)((sint32)((uint32)direction.vx - a) >> 4);
    b = (uint32)(sint32)r_s16(object + 0x126);
    y = b + (uint32)((sint32)((uint32)direction.vy - b) >> 4);
    c = (uint32)(sint32)r_s16(object + 0x12C);
    z = c + (uint32)((sint32)((uint32)direction.vz - c) >> 4);
    forward.vx = (sint32)x;
    forward.vy = (sint32)y;
    forward.vz = (sint32)z;
    ticks = r_u32(object + 0x9C);
    if ((sint32)ticks < -90 && r_s32(object + 0xA8) < 129) {
        forward.vx = (sint32)(0u - (uint32)forward.vx);
        forward.vy = (sint32)(0u - (uint32)forward.vy);
        forward.vz = (sint32)(0u - (uint32)forward.vz);
        object_basis_from_vectors(&forward, &up, object);
        w_u16(0x800FB3C0, 0);
        w_u16(0x800FB3C4, 0xF8F8);
        w_u32(0x800FB3D0, 255);
        w_u32(0x800FB3CC, 50000);
        w_u16(0x800FB3C2, 0);
        w_u32(0x800FB3C8, 12000);
        game_draw_attached_record(object, 0x800FB3C0, 1);
        forward.vx = (sint32)(0u - (uint32)forward.vx);
        forward.vz = (sint32)(0u - (uint32)forward.vz);
        forward.vy = (sint32)(0u - (uint32)forward.vy);
    }
    object_basis_from_vectors(&forward, &up, object);
    ticks = r_u32(object + 0x9C);
    w_u16(object + 0x250, 0);
    w_u32(object + 0x20, 0);
    w_u16(object + 0x252, 0);
    w_u32(object + 0x24, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0x28, 0);
    --ticks;
    w_u32(object + 0x9C, ticks);
    draw_first = (sint32)ticks >= -59;
    if (draw_first || (sint32)ticks < -90) {
        offset.vx = 0;
        offset.vy = draw_first ? 900 : 0;
        offset.vz = draw_first ? 1600 : 2000;
        ApplyMatrix((MATRIX *)psx_addr(object + 0x11C, 20), &offset, &point);
        scale = r_u32(object + 0x84);
        shift = ((sint32)scale < 0 ? 0u - scale : scale) & 31;
        if ((sint32)scale < 0) {
            point.vx = (sint32)point.vx >> shift;
            point.vz = (sint32)point.vz >> shift;
            point.vy = (sint32)point.vy >> shift;
        } else if ((sint32)scale > 0) {
            point.vx = (sint32)((uint32)point.vx << shift);
            point.vz = (sint32)((uint32)point.vz << shift);
            point.vy = (sint32)((uint32)point.vy << shift);
        }
        value = r_u32(object + 8);
        point.vx = (sint32)((uint32)point.vx + value);
        value = r_u32(object + 12);
        point.vy = (sint32)((uint32)point.vy + value);
        value = r_u32(object + 16);
        point.vz = (sint32)((uint32)point.vz + value);
        value = r_u32(target + 8);
        difference.vx = (sint32)(value - (uint32)point.vx);
        value = r_u32(target + 12);
        difference.vy = (sint32)(value - (uint32)point.vy);
        value = r_u32(target + 16);
        difference.vz = (sint32)(value - (uint32)point.vz);
        if (draw_first) {
            game_draw_target_beam_native(&point, &difference, 0);
            point.vx = 0;
            point.vy = 0;
            point.vz = 2000;
            game_emit_object_spark_native(object, &point, 512, 0xFF2020);
            game_emit_object_spark_native(object, &point, 512, 0xFF2020);
            game_emit_object_spark_native(object, &point, 512, 0xFF2020);
        } else {
            ticks = r_u32(object + 0x9C);
            phase = (sint32)ticks < -149 ? r_u32(object + 0xA8) + 1u : 1u;
            w_u32(object + 0xA8, phase);
            phase = r_u32(object + 0xA8);
            if (phase == 2)
                game_set_screen_tint(64, 64, 64);
            phase = r_u32(object + 0xA8);
            if (phase == 128) {
                value = r_u32(target + 8);
                w_u32(0x800ECDA8, value);
                value = r_u32(target + 12);
                w_u32(0x800ECDAC, value);
                value = r_u32(target + 16);
                w_u32(0x800ECDB8, 1);
                w_u32(0x800ECDB0, value);
                game_set_screen_tint(64, 64, 64);
                callback = r_u32(target + 0x48);
                psx_dispatch_guest_call(callback, 1, target, 0, 0);
            } else if ((sint32)phase < 129) {
                game_draw_target_beam_native(&point, &difference, phase);
            }
        }
    }
    if (r_s32(object + 0x9C) < -30 && r_u32(0x800EC884) == 0)
        game_begin_object_camera_sequence(object, target);
}

uint32 game_remove_other_objects_of_type(uint32 type, uint32 except)
{
    uint32 object, kind, next, callback;
    FUNCTION_MARKER(0x800ADCE0, "GAME.OVL");
    object = r_u32(0x8005367C);
    while (object != 0) {
        kind = r_u32(object);
        next = r_u32(object + 0x1D4);
        if ((kind == type && object != except) || kind == 0xFACE) {
            callback = r_u32(object + 0x48);
            psx_dispatch_guest_call(callback, 1, object, 0, 0);
        }
        object = next;
    }
    return 0;
}

void game_begin_object_camera_sequence(uint32 object, uint32 target)
{
    uint32 first, second, clock, selected, value;
    FUNCTION_MARKER(0x8007CD88, "GAME.OVL");
    first = r_u16(0x80059604);
    second = r_u16(0x80059606);
    clock = r_u32(0x800311B8);
    w_u32(0x800EEF54, 1);
    w_u16(0x80059610, 2);
    w_u32(0x80123F44, object);
    w_u32(0x80123F68, target);
    w_u16(0x800595FC, (uint16)first);
    w_u16(0x800595FE, (uint16)second);
    w_u32(0x800EEF14, clock);
    render_set_reduced_height(512);
    game_refresh_mission_state();
    w_u32(0x800EC884, 4);
    game_remove_objects_by_type(3);
    game_remove_other_objects_of_type(1, object);
    w_u32(0x800EC868, 1);
    camera_reset_follow();
    camera_set_look_at_object(0x8010AB58);
    selected = r_u32(0x80123F44);
    camera_set_target(selected);
    selected = r_u32(0x80123F44);
    value = r_u32(selected + 8);
    w_u32(0x8010AB58, value);
    value = r_u32(selected + 12);
    w_u32(0x8010AB5C, value);
    value = r_u32(selected + 16);
    w_u32(0x800313DC, 5);
    w_u32(0x8010AB60, value);
}

void game_begin_target_effect_camera(uint32 object, uint32 target)
{
    uint32 clock, selected, value;
    FUNCTION_MARKER(0x8007CEAC, "GAME.OVL");
    clock = r_u32(0x800311B8);
    w_u32(0x800EEF54, 1);
    w_u32(0x80123F44, object);
    w_u32(0x80123F68, target);
    w_u32(0x800EEF14, clock);
    render_set_reduced_height(512);
    game_refresh_mission_state();
    w_u32(0x800EC884, 5);
    game_remove_other_objects_of_type(3, object);
    game_remove_objects_by_type(1);
    w_u32(0x800EC868, 1);
    camera_reset_follow();
    camera_set_look_at_object(0x8010AB58);
    selected = r_u32(0x80123F44);
    camera_set_target(selected);
    selected = r_u32(0x80123F44);
    value = r_u32(selected + 8);
    w_u32(0x8010AB58, value);
    value = r_u32(selected + 12);
    w_u32(0x8010AB5C, value);
    value = r_u32(selected + 16);
    w_u32(0x800313DC, 5);
    w_u32(0x8010AB60, value);
    sound_play_sample_pitch(24, 100);
    sound_play_sample_pitch(24, 100);
    sound_play_sample_pitch(24, 128);
    sound_play_sample_pitch(24, 80);
}

void game_draw_target_effect_view(uint32 object)
{
    uint32 index, target, active, camera, x, y, z, value, other, scale, initial_scale;
    uint32 shifts = 0, magnitude, reduce, axis, next, phase;
    VECTOR forward, up, projected;
    SVECTOR angles = {0};
    FUNCTION_MARKER(0x800AC008, "GAME.OVL");
    index = r_u32(object + 0xBC);
    active = r_u32(0x800EC884);
    target = r_u32(0x801076F8 + index * 36);
    if (active == 0)
        game_begin_target_effect_camera(object, target);
    camera = r_u32(0x80040C94);
    x = r_u32(target + 8);
    other = r_u32(camera + 8);
    x -= other;
    w_u32(0x8004EAA8, x);
    y = r_u32(target + 12);
    other = r_u32(camera + 12);
    y -= other;
    w_u32(0x8004EAAC, y);
    other = r_u32(camera + 16);
    z = r_u32(target + 16) - other;
    w_u32(0x8004EAB0, z);
    initial_scale = r_u32(target + 0x84);
    if ((sint32)initial_scale > 0) {
        scale = r_u32(target + 0x84);
        w_u32(0x8004EAA8, (uint32)((sint32)x >> (scale & 31)));
        scale = r_u32(target + 0x84);
        w_u32(0x8004EAAC, (uint32)((sint32)y >> (scale & 31)));
        scale = r_u32(target + 0x84);
        w_u32(0x80053670, initial_scale);
        w_u32(0x8004EAB0, (uint32)((sint32)z >> (scale & 31)));
    } else {
        w_u32(0x80053670, 0);
    }
    value = r_u32(0x8004EAA8);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if ((sint32)magnitude >= 16385)
        goto reduce_camera_vector;
    value = r_u32(0x8004EAAC);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if ((sint32)magnitude >= 16385)
        goto reduce_camera_vector;
    value = r_u32(0x8004EAB0);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if ((sint32)magnitude < 16385)
        goto camera_vector_ready;
reduce_camera_vector:
    do {
        x = (uint32)((sint32)r_u32(0x8004EAA8) >> 1);
        w_u32(0x8004EAA8, x);
        y = r_u32(0x8004EAAC);
        z = r_u32(0x8004EAB0);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
        w_u32(0x8004EAAC, y);
        w_u32(0x8004EAB0, z);
        ++shifts;
        magnitude = (sint32)x < 0 ? 0u - x : x;
        reduce = (sint32)magnitude >= 16385;
        if (!reduce) {
            magnitude = (sint32)y < 0 ? 0u - y : y;
            reduce = (sint32)magnitude >= 16385;
            if (!reduce) {
                magnitude = (sint32)z < 0 ? 0u - z : z;
                reduce = (sint32)magnitude >= 16385;
            }
        }
    } while (reduce);
camera_vector_ready:
    if (r_s32(object + 0x84) < 0) {
        scale = r_u32(target + 0x84);
        shifts += (sint32)scale < 0 ? 0u - scale : scale;
    }
    ApplyMatrixLV((MATRIX *)psx_addr(0x80035060, 20), (VECTOR *)psx_addr(0x8004EAA8, 12), &projected);
    w_u32(0x8004EAD0, (uint32)projected.vx);
    w_u32(0x8004EAD4, (uint32)projected.vy);
    w_u32(0x8004EAD8, (uint32)projected.vz);
    x = r_u32(target + 8);
    other = r_u32(object + 8);
    x -= other;
    y = r_u32(target + 12);
    other = r_u32(object + 12);
    y -= other;
    z = r_u32(target + 16);
    other = r_u32(object + 16);
    z -= other;
    for (;;) {
        magnitude = (sint32)x < 0 ? 0u - x : x;
        if ((sint32)magnitude < 16385) {
            magnitude = (sint32)y < 0 ? 0u - y : y;
            if ((sint32)magnitude < 16385) {
                magnitude = (sint32)z < 0 ? 0u - z : z;
                if ((sint32)magnitude < 16385)
                    break;
            }
        }
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
    }
    forward.vx = (sint32)x;
    forward.vy = (sint32)y;
    forward.vz = (sint32)z;
    up.vx = (sint32)(0u - (uint32)(sint32)r_s16(object + 0x11E));
    up.vy = (sint32)(0u - (uint32)(sint32)r_s16(object + 0x124));
    up.vz = (sint32)(0u - (uint32)(sint32)r_s16(object + 0x12A));
    matrix_basis_from_vectors(&forward, &up, 0x8004EAF8);
    angles.vx = 1024;
    angles.vy = 0;
    angles.vz = 32;
    value = (uint32)(sint32)r_s16(0x8004EAF8);
    for (axis = 0; axis < 9; ++axis) {
        if (axis < 8)
            next = (uint32)(sint32)r_s16(0x8004EAF8 + (axis + 1) * 2);
        w_u16(0x8004EAF8 + axis * 2, (uint16)((sint32)(value * 5u) >> 3));
        if (axis < 8)
            value = next;
    }
    RotMatrix(&angles, (MATRIX *)psx_addr(0x80034DA0, 20));
    MulMatrix0((MATRIX *)psx_addr(0x8004EAF8, 20), (MATRIX *)psx_addr(0x80034DA0, 20), (MATRIX *)psx_addr(0x8004EAF8, 20));
    phase = r_u32(0x800311B8);
    angles.vx = 0;
    angles.vz = 0;
    angles.vy = (sint16)(phase << 1);
    RotMatrix(&angles, (MATRIX *)psx_addr(0x80034DA0, 20));
    MulMatrix0((MATRIX *)psx_addr(0x8004EAF8, 20), (MATRIX *)psx_addr(0x80034DA0, 20), (MATRIX *)psx_addr(0x8004EAF8, 20));
    matrix_shift(0x8004EAF8, 0x80034DA0, shifts);
    MulMatrix0((MATRIX *)psx_addr(0x80035060, 20), (MATRIX *)psx_addr(0x80034DA0, 20), (MATRIX *)psx_addr(0x800420F0, 20));
    SetRotMatrix((MATRIX *)psx_addr(0x800420F0, 20));
    x = r_u32(0x8004EAD0);
    y = r_u32(0x8004EAD4);
    z = r_u32(0x8004EAD8);
    w_u32(0x80034D6C, x);
    w_u32(0x80034D70, y);
    w_u32(0x80034D74, z);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    phase = r_u32(0x800ECDB8);
    game_draw_target_effect_field(phase);
}

void game_update_mission_script_controller(uint32 object)
{
    sint32 paused = r_s16(0x800595F6);
    uint32 ticks, immediate, pointer, player, callback, index, offset, cx, cy, cz, x, y, z;
    uint32 distance, limit, message, thread, count, command, opcode, operand, kind, result;
    FUNCTION_MARKER(0x800AC624, "GAME.OVL");
    if (paused != 0 || r_u32(0x800EEB24) != 0 || r_u32(0x800EEF54) != 0)
        return;
    if (r_u32(0x800595FC) != 0) {
        ticks = r_u32(object + 0x9C) - 1u;
        w_u32(object + 0x9C, ticks);
        immediate = r_u32(0x800F18AC);
        if (!(immediate != 0 && (sint32)ticks < 3) && r_s32(object + 0x9C) > 0)
            return;
        pointer = r_u32(0x80041004);
        if (r_u32(pointer) != 0)
            return;
        player = r_u32(0x80040FDC);
        game_begin_mission_completion(player);
        callback = r_u32(object + 0x48);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
        return;
    }
    if (r_s16(0x80118078) != -1) {
        ticks = r_u16(0x80118084) - 1u;
        w_u16(0x80118084, (uint16)ticks);
        if ((sint16)(uint16)ticks <= 0) {
            index = (uint32)(sint32)r_s16(0x8011807A);
            offset = index * 12;
            player = r_u32(0x80040FDC);
            cx = r_u32(0x80126D68 + offset);
            cy = r_u32(0x80126D6C + offset);
            x = r_u32(player + 8);
            y = r_u32(player + 12);
            z = r_u32(player + 16);
            cz = r_u32(0x80126D70 + offset);
            distance = (uint32)vector_length_approx((sint32)(cx - x), (sint32)(cy - y), (sint32)(cz - z));
            limit = r_u32(0x8011808C);
            w_u32(0x80046174, distance);
            if ((sint32)limit < (sint32)distance) {
                message = r_u32(0x800F2464);
                stream_enqueue(300, 1, game_jumpgate_text_native(message));
                player = r_u32(0x80040FDC);
                w_u16(0x80118084, 3000);
                w_u32(player + 0x40, 0x8007ADB8);
                cx = r_u32(0x80126D68 + offset);
                w_u32(player + 0x214, cx);
                cy = r_u32(0x80126D6C + offset);
                w_u32(player + 0x218, cy);
                cz = r_u32(0x80126D70 + offset);
                w_u32(player + 0x21C, cz);
                limit = r_u32(0x80118088);
                w_u32(player + 0xA8, limit - 40000u);
            } else {
                limit = r_u32(0x80118088);
                if ((sint32)limit < (sint32)distance) {
                    message = r_u32(0x800F2460);
                    stream_enqueue(300, 1, game_jumpgate_text_native(message));
                }
            }
            w_u16(0x80118084, 10);
        }
    }
    count = r_u32(0x80059194);
    if ((sint32)count <= 0)
        return;
    thread = 0;
    do {
        index = r_u32(0x80059198 + thread * 4);
        command = 0x80058E70 + index * 4;
        for (;;) {
            opcode = r_u32(command);
            if (opcode == 666) {
                w_u32(object + 0x9C, 6);
                operand = r_u32(command + 4);
                w_u16(0x800595FC, (uint16)operand);
                operand = r_u32(command + 8);
                immediate = r_u32(0x800F18AC);
                w_u16(0x800595FE, (uint16)operand);
                if (immediate == 0) {
                    operand = r_u32(command + 12);
                    w_u16(0x80059610, operand == 0 ? 1 : 2);
                }
                return;
            }
            operand = r_u32(0x800591AC + (opcode - 1u) * 4);
            kind = r_u32(0x80058E70 + operand * 4);
            callback = r_u32(0x800EEF18 + kind * 4);
            result = psx_dispatch_guest_call(callback, 1, operand + 1u, 0, 0);
            command += 4;
            if ((uint16)result == 0)
                break;
        }
        count = r_u32(0x80059194);
        ++thread;
    } while ((sint32)thread < (sint32)count);
}

uint32 game_script_condition_ship_missing(uint32 operand)
{
    uint32 identifier, category;
    FUNCTION_MARKER(0x800AC9B8, "GAME.OVL");
    identifier = r_u32(0x80058E70 + operand * 4);
    category = r_u32(0x80053698 + identifier * 60);
    return game_find_active_ship_by_identifier(identifier, category) == 0;
}

uint32 game_script_condition_elapsed_time(uint32 operand)
{
    uint32 clock, started, deadline, elapsed, remaining, minutes, format, suffix, length, suffix_length;
    const char *native_suffix;
    FUNCTION_MARKER(0x800ACA0C, "GAME.OVL");
    clock = r_u32(0x800311B8);
    started = r_u32(0x800595B8);
    deadline = r_u32(0x80058E70 + operand * 4);
    elapsed = clock - started + 1u;
    if ((sint32)deadline <= 35999) {
        remaining = deadline - elapsed;
        minutes = (uint32)((sint32)remaining / 1800);
        w_u32(0x8011F130, minutes);
        if (minutes * 1800u == remaining && (sint32)minutes > 0) {
            format = r_u32(0x800F2584);
            sprintfPSX(0x800FA368, format, minutes);
            minutes = r_u32(0x8011F130);
            if ((sint32)minutes >= 2) {
                suffix = 0x80075DE0;
                native_suffix = game_jumpgate_text_native(suffix);
                length = (uint32)strlen(game_jumpgate_text_native(0x800FA368));
                suffix_length = (uint32)strlen(native_suffix);
                strcat((char *)psx_addr(0x800FA368, (size_t)length + suffix_length + 1), native_suffix);
            }
            suffix = r_u32(0x800F2588);
            native_suffix = game_jumpgate_text_native(suffix);
            length = (uint32)strlen(game_jumpgate_text_native(0x800FA368));
            suffix_length = (uint32)strlen(native_suffix);
            strcat((char *)psx_addr(0x800FA368, (size_t)length + suffix_length + 1), native_suffix);
            game_queue_hud_message(0x800FA368);
        }
    }
    return (sint32)deadline < (sint32)elapsed;
}
