#include "game_functions.h"

void game_draw_object_link_native(uint32 first_object, uint32 second_object, VECTOR *first_offset, VECTOR *second_offset, uint32 distance, uint32 mode, uint32 power)
{
    VECTOR points[4], offset, difference, direction;
    SVECTOR angles;
    MATRIX rotation;
    uint32 player, frame, phase, value, product, numerator, components[3], normalized[3];
    uint32 index, delta, shift, base_shift, yaw, horizontal;
    sint32 length, force, amplitude;
    FUNCTION_MARKER(0x800CA4F0, "GAME.OVL");
    if ((sint32)distance >= 16001)
        distance = 32000u;
    player = r_u32(0x80040FDCu);
    base_shift = first_object == player ? 11u : 13u;
    offset.vx = first_offset->vx;
    offset.vy = first_offset->vy;
    offset.vz = first_offset->vz;
    if (first_object == player)
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(first_object + 0x11Cu, 18u), &offset, &offset);
    else
        ApplyMatrixLV((MATRIX *)psx_addr(first_object + 0x11Cu, 20u), &offset, &offset);
    points[0].vx = (sint32)(r_u32(first_object + 8u) + (uint32)offset.vx);
    points[0].vy = (sint32)(r_u32(first_object + 12u) + (uint32)offset.vy);
    points[0].vz = (sint32)(r_u32(first_object + 16u) + (uint32)offset.vz);
    offset.vx = second_offset->vx;
    player = r_u32(0x80040FDCu);
    offset.vy = second_offset->vy;
    offset.vz = second_offset->vz;
    if (second_object == player)
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(second_object + 0x11Cu, 18u), &offset, &offset);
    else
        ApplyMatrixLV((MATRIX *)psx_addr(second_object + 0x11Cu, 20u), &offset, &offset);
    value = r_u32(second_object + 8u);
    frame = r_u32(0x800311B8u);
    points[3].vx = (sint32)(value + (uint32)offset.vx);
    points[3].vy = (sint32)(r_u32(second_object + 12u) + (uint32)offset.vy);
    difference.vy = (sint32)((uint32)points[3].vy - (uint32)points[0].vy);
    difference.vx = (sint32)((uint32)points[3].vx - (uint32)points[0].vx);
    points[3].vz = (sint32)(r_u32(second_object + 16u) + (uint32)offset.vz);
    difference.vz = (sint32)((uint32)points[3].vz - (uint32)points[0].vz);
    amplitude = (sint32)distance >> 3;
    product = (uint32)amplitude * (uint32)rsin((sint32)(frame << 6));
    phase = r_u32(0x800311B8u) << 7;
    points[1].vx = (sint32)((uint32)points[0].vx + (uint32)((sint32)((uint32)difference.vx * 20u) >> 6) + (uint32)((sint32)product >> 14));
    product = (uint32)amplitude * (uint32)rcos((sint32)phase);
    phase = (0u - r_u32(0x800311B8u)) << 8;
    points[1].vy = (sint32)((uint32)points[0].vy + (uint32)((sint32)((uint32)difference.vy * 20u) >> 6) - (uint32)((sint32)product >> 14));
    product = (uint32)amplitude * (uint32)rsin((sint32)phase);
    phase = r_u32(0x800311B8u) << 8;
    points[1].vz = (sint32)((uint32)points[0].vz + (uint32)((sint32)((uint32)difference.vz * 20u) >> 6) + (uint32)((sint32)product >> 14));
    product = (uint32)amplitude * (uint32)rsin((sint32)phase);
    phase = r_u32(0x800311B8u) << 7;
    points[2].vx = (sint32)((uint32)points[0].vx + (uint32)((sint32)((uint32)difference.vx * 40u) >> 6) - (uint32)((sint32)product >> 14));
    product = (uint32)amplitude * (uint32)rcos((sint32)phase);
    phase = (0u - r_u32(0x800311B8u)) << 6;
    points[2].vy = (sint32)((uint32)points[0].vy + (uint32)((sint32)((uint32)difference.vy * 40u) >> 6) + (uint32)((sint32)product >> 14));
    product = (uint32)amplitude * (uint32)rsin((sint32)phase);
    points[2].vz = (sint32)((uint32)points[0].vz + (uint32)((sint32)((uint32)difference.vz * 40u) >> 6) - (uint32)((sint32)product >> 14));
    game_draw_tractor_beam_native(points, 0u);
    components[0] = (uint32)points[3].vx - (uint32)points[0].vx;
    components[1] = (uint32)points[3].vy - (uint32)points[0].vy;
    components[2] = (uint32)points[3].vz - (uint32)points[0].vz;
    length = vector_length_approx((sint32)components[0], (sint32)components[1], (sint32)components[2]);
    if (length == 0)
        length = 1;
    for (index = 0u; index < 3u; ++index) {
        numerator = components[index] << 12;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        normalized[index] = (uint32)((sint64)(sint32)numerator / length);
    }
    force = (sint32)((uint32)length - distance);
    if (force > 512)
        force = 512;
    if (force < -512)
        force = -512;
    delta = power - mode;
    shift = (delta + base_shift) & 31u;
    for (index = 0u; index < 3u; ++index) {
        product = normalized[index] * (uint32)force;
        value = r_u32(second_object + 0x14u + index * 4u);
        w_u32(second_object + 0x14u + index * 4u, value - (uint32)((sint32)product >> shift));
    }
    shift = (base_shift - delta) & 31u;
    for (index = 0u; index < 3u; ++index) {
        product = normalized[index] * (uint32)force;
        if (index == 2u)
            player = r_u32(0x80040FDCu);
        value = r_u32(first_object + 0x14u + index * 4u);
        w_u32(first_object + 0x14u + index * 4u, value + (uint32)((sint32)product >> shift));
    }
    if (first_object == player)
        return;
    direction.vx = (sint32)normalized[0];
    direction.vy = (sint32)normalized[1];
    direction.vz = (sint32)normalized[2];
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(first_object + 0x11Cu, 18u), &direction, &direction);
    if (first_object == r_u32(0x80040FDCu)) {
        product = (uint32)direction.vx * (uint32)direction.vx + (uint32)direction.vz * (uint32)direction.vz;
        horizontal = (uint32)SquareRoot0((sint32)product);
        yaw = (uint32)ratan2(direction.vx, direction.vz);
        value = 1024u - (uint32)ratan2((sint32)horizontal, direction.vy);
        angles.vx = (sint16)((sint32)value >> 4);
        angles.vy = (sint16)((sint32)(0u - yaw) >> 4);
    } else {
        yaw = (uint32)ratan2(direction.vx, direction.vz);
        value = (uint32)direction.vz;
        if ((sint32)value < 0)
            value = 0u - value;
        angles.vx = (sint16)(ratan2((sint32)(0u - (uint32)direction.vy), (sint32)value) >> 1);
        angles.vy = (sint16)((sint32)yaw >> 1);
    }
    angles.vz = 0;
    RotMatrixYXZ(&angles, &rotation);
    if (first_object == r_u32(0x80040FDCu)) {
        MulMatrix0(&rotation, (MATRIX *)psx_addr(first_object + 0x11Cu, 20u), (MATRIX *)psx_addr(first_object + 0x11Cu, 20u));
    } else {
        // The original swaps each off-diagonal pair twice
        MulMatrix0((MATRIX *)psx_addr(first_object + 0x11Cu, 20u), &rotation, (MATRIX *)psx_addr(first_object + 0x11Cu, 20u));
    }
    matrix_normalize(first_object + 0x11Cu, 1u);
}

void game_start_damage_state(void)
{
    uint32 object;
    FUNCTION_MARKER(0x8007A5C8, "GAME.OVL");
    if (r_u8(0x8004EAE2u) != 0u)
        return;
    if (r_u32(0x800ECD90u) == 0u) {
        object = game_create_object(0u, 0u, 0u, 0u, 0u);
        w_u32(0x800ECD90u, object);
    }
    object = r_u32(0x800ECD90u);
    w_u32(object + 0x40u, 0x8007A644u);
    w_u32(object + 0x48u, 0x8007A68Cu);
    w_u32(object + 0x9Cu, 10u);
}

void game_update_damage_state(uint32 object)
{
    uint32 remaining, callback;
    FUNCTION_MARKER(0x8007A644, "GAME.OVL");
    w_u8(0x8004EAE2u, 1u);
    remaining = r_u32(object + 0x9Cu) - 1u;
    w_u32(object + 0x9Cu, remaining);
    if ((sint32)remaining <= 0) {
        callback = r_u32(object + 0x48u);
        psx_dispatch_guest_call(callback, 1u, object, 0u, 0u);
    }
}

void game_finish_damage_state(uint32 object)
{
    FUNCTION_MARKER(0x8007A68C, "GAME.OVL");
    w_u8(0x8004EAE2u, 0u);
    game_mark_object_for_removal(object);
    w_u32(0x800ECD90u, 0u);
}

uint32 game_create_temporary_camera(uint32 owner, uint32 duration)
{
    uint32 object, x, y, z, value, yaw, pitch;
    FUNCTION_MARKER(0x8007A6BC, "GAME.OVL");
    x = r_u32(owner + 8u);
    y = r_u32(owner + 12u);
    z = r_u32(owner + 16u);
    object = game_create_object(0u, x, y, z, 0u);
    w_u32(object + 0x178u, owner);
    value = (uint32)random_between(-1024, 1024);
    w_u32(object + 0x24u, value);
    value = (uint32)random_between(0, 4095);
    w_u32(object + 0x20u, value);
    game_set_acceleration_from_basis(object, 200u);
    w_u32(object + 0x40u, 0x8007A770u);
    yaw = r_u32(object + 0x20u);
    pitch = r_u32(object + 0x24u);
    w_u32(object + 0x28u, 0u);
    w_u32(object + 0x9Cu, duration);
    w_u32(object + 0x20u, yaw + 2048u);
    w_u32(object + 0x24u, 0u - pitch);
    return object;
}

void game_update_temporary_camera(uint32 object)
{
    uint32 remaining;
    FUNCTION_MARKER(0x8007A770, "GAME.OVL");
    matrix_normalize(object + 0x11Cu, 1u);
    w_u32(0x800313DCu, 1u);
    w_u32(0x80040C94u, object);
    game_update_object_movement(object);
    remaining = r_u32(object + 0x9Cu);
    w_u32(object + 0x9Cu, remaining - 1u);
}

uint32 game_start_timed_view_effect(uint32 magnitude, uint32 duration)
{
    uint32 object;
    FUNCTION_MARKER(0x800A2CC4, "GAME.OVL");
    if (r_u32(0x800EEE10u) == 0u) {
        object = game_create_object(0u, 0u, 0u, 0u, 0u);
        w_u32(0x800EEE10u, object);
    }
    object = r_u32(0x800EEE10u);
    w_u32(object + 0x40u, 0x800A2D3Cu);
    w_u32(object + 0x9Cu, duration);
    w_u32(object + 0xB8u, magnitude);
    return object;
}

void game_apply_player_damage_native(uint32 damage, const VECTOR *position)
{
    uint32 player;
    FUNCTION_MARKER(0x8007A3E8, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    game_apply_player_damage_from_state_native(damage, position, player);
}

void game_apply_player_damage_from_state_native(uint32 damage, const VECTOR *position, uint32 player)
{
    VECTOR velocity;
    uint32 remaining, value;
    FUNCTION_MARKER(0x8007A3F0, "GAME.OVL");
    if (r_u32(player) != 0x66u)
        return;
    if (r_u32(0x800EC884u) != 0u)
        return;
    if (r_u32(0x800EC868u) != 0u)
        return;
    if (r_s32(0x800EC888u) > 0)
        return;
    game_start_damage_state();
    game_start_timed_view_effect(7u, 10u);
    player = r_u32(0x80040FDCu);
    if (r_s32(player + 0x234u) > 0) {
        sound_play_sample(3u);
        player = r_u32(0x80040FDCu);
        velocity.vx = r_s32(player + 0x14u) >> 8;
        velocity.vy = r_s32(player + 0x18u) >> 8;
        velocity.vz = r_s32(player + 0x1Cu) >> 8;
        game_emit_camera_particle_native(position, &velocity, 0xFF0000u, 2048u);
        game_emit_camera_particle_native(position, &velocity, 0xFFFFFFu, 1024u);
        player = r_u32(0x80040FDCu);
        remaining = r_u32(player + 0x234u) - damage;
        w_u32(player + 0x234u, remaining);
        if ((sint32)remaining < 0) {
            w_u32(player + 0x234u, 0u);
            game_request_radio_message(0x42u, r_u32(0x800F23D0u));
        }
    } else {
        sound_play_sample(2u);
        player = r_u32(0x80040FDCu);
        remaining = r_u32(player + 0x238u) - damage;
        w_u32(player + 0x238u, remaining);
        if (remaining - 1u < 24u && (r_u32(player + 0x114u) & 2u) == 0u) {
            game_request_radio_message(0x46u, r_u32(0x800F23D4u));
            player = r_u32(0x80040FDCu);
            value = r_u32(player + 0x114u);
            w_u32(player + 0x114u, value | 2u);
        }
        player = r_u32(0x80040FDCu);
        if (r_s32(player + 0x238u) < 0)
            w_u32(player + 0x238u, 0u);
    }
}

void game_update_timed_view_effect(uint32 object)
{
    uint32 remaining, magnitude, value, x, y;
    FUNCTION_MARKER(0x800A2D3C, "GAME.OVL");
    remaining = r_u32(object + 0x9Cu) - 1u;
    w_u32(object + 0x9Cu, remaining);
    if ((sint32)remaining < 0) {
        x = r_u32(0x80031198u);
        y = r_u32(0x800311B4u);
        w_u32(0x8001024Cu, x);
        w_u32(0x80010250u, y);
        system_set_projection_offset((sint32)x, (sint32)y);
        game_mark_object_for_removal(object);
        w_u32(0x800EEE10u, 0u);
    } else {
        magnitude = r_u32(object + 0xB8u);
        value = (uint32)random_between((sint32)(0u - magnitude), (sint32)magnitude);
        magnitude = r_u32(object + 0xB8u);
        x = r_u32(0x80031198u) + value;
        w_u32(0x8001024Cu, x);
        value = (uint32)random_between((sint32)(0u - magnitude), (sint32)magnitude);
        y = r_u32(0x800311B4u);
        x = r_u32(0x8001024Cu);
        y += value;
        w_u32(0x80010250u, y);
        system_set_projection_offset((sint32)x, (sint32)y);
    }
}

void game_handle_player_projectile_hit(uint32 object, uint32 position, uint32 unused, uint32 other, uint32 projectile, uint32 kind, uint32 damage_profile)
{
    VECTOR direction;
    uint32 old_hull, old_shield, active, source, identity, player, value, amount, flags, x, y, z, callback;
    FUNCTION_MARKER(0x80079E5C, "GAME.OVL");
    old_hull = r_u32(object + 0x238u);
    active = r_u32(object);
    old_shield = r_u32(object + 0x234u);
    if (active == 0u || r_u32(0x800595C8u) != 0u || r_u32(0x800EC884u) != 0u || r_s16(0x800595F6u) != 0)
        return;
    if (projectile != 0u && r_s16(projectile + 0x3Cu) == 0xD5) {
        source = r_u32(projectile + 0x58u);
        if (source != 0u) {
            identity = r_u16(source + 0x24Au);
            if (identity == r_u16(projectile + 0x5Cu)) {
                value = (uint32)r_s16(0x80059612u);
                if (r_u32(source) == value && r_s16(0x8005961Au) != 3)
                    return;
            }
        }
    }
    if (r_u32(0x800EC884u) != 0u || r_u32(0x800EC868u) != 0u || r_s32(0x800EC888u) > 0)
        return;
    game_start_damage_state();
    game_start_timed_view_effect(7u, 10u);
    player = r_u32(0x80040FDCu);
    if (r_s32(player + 0x234u) > 0) {
        if (r_s32(damage_profile + 8u) > 0 && (r_u32(damage_profile + 12u) & 1u) == 0u) {
            sound_play_sample(3u);
            player = r_u32(0x80040FDCu);
            direction.vx = r_s32(player + 0x14u) >> 8;
            direction.vy = r_s32(player + 0x18u) >> 8;
            direction.vz = r_s32(player + 0x1Cu) >> 8;
            game_emit_camera_particle_native((const VECTOR *)psx_addr(position, 12u), &direction, 0xFF0000u, 2048u);
            game_emit_camera_particle_native((const VECTOR *)psx_addr(position, 12u), &direction, 0xFFFFFFu, 1024u);
            game_set_screen_tint(0u, 0u, r_u32(damage_profile + 8u) << 4);
            value = r_u32(other);
            player = r_u32(0x80040FDCu);
            amount = r_u32(damage_profile + 8u);
            if (value - 0xCFu < 5u) {
                amount = (uint32)((sint32)amount >> 1);
                value = r_u32(player + 0x234u);
            } else {
                value = r_u32(player + 0x234u);
            }
            w_u32(player + 0x234u, value - amount);
            player = r_u32(0x80040FDCu);
            if (r_s32(player + 0x234u) < 0) {
                w_u32(player + 0x234u, 0u);
                if (r_u32(0x800F18ACu) == 0u)
                    game_request_radio_message(0x42u, r_u32(0x800F23D0u));
            }
            goto hit_complete;
        }
        player = r_u32(0x80040FDCu);
        if (r_s32(player + 0x234u) > 0 && (r_u32(damage_profile + 12u) & 1u) == 0u)
            goto hit_complete;
    }
    value = r_u32(other);
    player = r_u32(0x80040FDCu);
    amount = r_u32(damage_profile + 4u);
    if (value - 0xCFu < 5u) {
        value = r_u32(player + 0x238u);
        w_u32(player + 0x238u, value - (uint32)((sint32)amount >> 1));
        amount = r_u32(damage_profile);
        value = r_u32(player + 0x244u);
        w_u32(player + 0x244u, value - (uint32)((sint32)amount >> 1));
    } else {
        value = r_u32(player + 0x238u);
        w_u32(player + 0x238u, value - amount);
        value = r_u32(player + 0x244u);
        amount = r_u32(damage_profile);
        w_u32(player + 0x244u, value - amount);
    }
    game_set_screen_tint(r_u32(damage_profile + 4u) << 4, 0u, 0u);
    value = r_u32(damage_profile) << 4;
    game_set_screen_tint(value, 0u, value);
    player = r_u32(0x80040FDCu);
    game_emit_projectile_fragments(position, player, 12, 0u);
    sound_play_sample(2u);
    player = r_u32(0x80040FDCu);
    value = r_u32(player + 0x238u);
    if (value - 1u < 24u && (r_u32(player + 0x114u) & 2u) == 0u) {
        game_request_radio_message(0x46u, r_u32(0x800F23D4u));
        player = r_u32(0x80040FDCu);
        flags = r_u32(player + 0x114u);
        w_u32(player + 0x114u, flags | 2u);
    }
    player = r_u32(0x80040FDCu);
    if (r_s32(player + 0x238u) < 0) {
        w_u32(player + 0x238u, 0u);
        game_fail_current_mission();
    }
hit_complete:
    if (other != 0u && (r_u32(other + 0x114u) & 4u) != 0u) {
        w_u32(object + 0x238u, old_hull);
        w_u32(object + 0x234u, old_shield);
    }
    if (kind == 0x69u) {
        game_start_timed_view_effect(7u, 10u);
        sound_play_at_listener(position, 4u, 0u, r_u32(0x80040C94u));
        x = r_u32(position);
        y = r_u32(position + 4u);
        z = r_u32(position + 8u);
        game_emit_hit_flash(x, y, z);
    } else if (kind == 0x64u) {
        sound_play_at_listener(position, 4u, 0u, r_u32(0x80040C94u));
    }
    if (other == 0u)
        return;
    if ((r_u32(other + 0x114u) & 16u) == 0u) {
        player = r_u32(0x80040FDCu);
        value = r_u32(other + 8u);
        direction.vx = (sint32)(r_u32(player + 8u) - value);
        x = r_u32(player + 12u);
        direction.vy = (sint32)(x - r_u32(other + 12u));
        z = r_u32(player + 16u);
        direction.vz = (sint32)(z - r_u32(other + 16u));
        vector_normalize(&direction, &direction);
        player = r_u32(0x80040FDCu);
        value = r_u32(player + 0x14u);
        w_u32(player + 0x14u, value + ((uint32)direction.vx << 3));
        value = r_u32(player + 0x18u);
        w_u32(player + 0x18u, value + ((uint32)direction.vy << 3));
        value = r_u32(player + 0x1Cu);
        w_u32(player + 0x1Cu, value + ((uint32)direction.vz << 3));
    }
    value = r_u32(other);
    if (value != 3u && value != 1u && value != 7u && value != 0x6Au) {
        callback = r_u32(other + 0x5Cu);
        psx_dispatch_guest_call(callback, 1u, other, 0u, 0u);
    }
}

void game_emit_hit_flash(uint32 x, uint32 y, uint32 z)
{
    uint32 record, next;
    sint32 remainder;
    FUNCTION_MARKER(0x800BCC5C, "GAME.OVL");
    record = 0x800FD518u + r_u32(0x800EF504u) * 48u;
    next = r_u32(0x800EF508u) + 1u;
    w_u32(record, x);
    w_u32(record + 0x10u, 64u);
    w_u32(record + 0x28u, 4u);
    w_u32(record + 4u, y);
    w_u32(record + 8u, z);
    w_u32(record + 0x24u, 255u);
    w_u32(record + 0x18u, 255u);
    w_u32(record + 0x1Cu, 255u);
    w_u32(record + 0x20u, 255u);
    w_u32(record + 0x14u, 32u);
    remainder = (sint32)next % 10;
    w_u32(0x800EF508u, (uint32)remainder);
}

void game_collide_object_groups(uint32 head, uint32 object, sint32 range, uint32 mode, uint32 same_group)
{
    uint32 other, flags, object_flags, x, y, z, model, models, radius, second_radius, scale, other_scale;
    uint32 first_position, second_position, kind, profile, callback;
    sint32 distance;
    FUNCTION_MARKER(0x800A1C48, "GAME.OVL");
    other = head;
    if (other == 0u)
        return;
    do {
        if (other == object)
            goto next_collision_object;
        flags = r_u32(other + 0x114u);
        if ((flags & 0x8000u) != 0u || (object < other && same_group != 0u))
            goto next_collision_object;
        if (r_u32(other) == 0u || r_u32(other + 0xA0u) == 0u)
            goto next_collision_object;
        object_flags = r_u32(object + 0x114u);
        first_position = other + 8u;
        second_position = object + 8u;
        if ((object_flags & 0x800u) == 0u && (flags & 0x800u) == 0u) {
            x = r_u32(object + 8u) - r_u32(other + 8u);
            if ((sint32)x < 0)
                x = 0u - x;
            if ((sint32)x > 10000)
                goto next_collision_object;
            y = r_u32(object + 12u) - r_u32(other + 12u);
            if ((sint32)y < 0)
                y = 0u - y;
            if ((sint32)y > 10000)
                goto next_collision_object;
            z = r_u32(object + 16u) - r_u32(other + 16u);
            if ((sint32)z < 0)
                z = 0u - z;
            if ((sint32)z > 10000)
                goto next_collision_object;
            distance = vector_length_approx((sint32)x, (sint32)y, (sint32)z);
            if (mode != 0u && distance < 10000)
                game_notify_object_contact(object, other);
            if (distance >= range)
                goto next_collision_object;
        } else if ((object_flags & 0x800u) != 0u && (flags & 0x800u) != 0u) {
            if ((object_flags & 0x40000u) != 0u || (flags & 0x40000u) != 0u)
                goto next_collision_object;
            if (r_u32(0x800595F8u) == 0x1000Bu) {
                game_resolve_special_mission_collision(object, other);
                goto next_collision_object;
            }
            scale = r_u32(object + 0x84u);
            other_scale = r_u32(other + 0x84u);
            model = r_u32(object + 0x74u);
            models = r_u32(0x80046178u);
            radius = r_u32(models + model * 132u + 0x6Cu);
            if ((sint32)scale > 0) {
                radius <<= (scale - 1u) & 31u;
            } else {
                if ((sint32)scale < 0)
                    scale = 0u - scale;
                radius = (uint32)((sint32)radius >> (scale & 31u));
            }
            model = r_u32(other + 0x74u);
            models = r_u32(0x80046178u);
            second_radius = r_u32(models + model * 132u + 0x6Cu);
            if ((sint32)other_scale > 0) {
                second_radius <<= (other_scale - 1u) & 31u;
            } else {
                if ((sint32)other_scale < 0)
                    other_scale = 0u - other_scale;
                second_radius = (uint32)((sint32)second_radius >> (other_scale & 31u));
            }
            radius += second_radius;
            x = r_u32(other + 8u);
            x -= r_u32(object + 8u);
            y = r_u32(other + 12u);
            y -= r_u32(object + 12u);
            z = r_u32(object + 16u);
            z = r_u32(other + 16u) - z;
            distance = vector_length_approx((sint32)x, (sint32)y, (sint32)z);
            if (distance >= (sint32)radius)
                goto next_collision_object;
            if ((r_u32(object + 0x114u) & 0x40u) != 0u || (r_u32(other + 0x114u) & 0x40u) != 0u)
                goto next_collision_object;
        } else if ((object_flags & 0x800u) != 0u) {
            if (game_check_swept_object_collision(object, other) == 0u || (r_u32(other + 0x114u) & 0x48u) != 0u)
                goto next_collision_object;
            second_position = first_position;
        } else {
            if (game_check_swept_object_collision(other, object) == 0u || (r_u32(object + 0x114u) & 0x48u) != 0u)
                goto next_collision_object;
            first_position = second_position;
        }
        kind = r_u32(other);
        profile = r_u32(other + 0x1E4u);
        callback = r_u32(object + 0x44u);
        game_dispatch_object_hit(callback, object, first_position, other + 0xFCu, other, 0u, kind, profile);
        if (mode != 0u) {
            kind = r_u32(object);
            profile = r_u32(other + 0x1E4u);
            callback = r_u32(other + 0x44u);
            game_dispatch_object_hit(callback, other, second_position, object + 0xFCu, object, 0u, kind, profile);
        }
next_collision_object:
        other = r_u32(other + 0x1DCu);
    } while (other != 0u);
}

void game_resolve_special_mission_collision(uint32 first, uint32 second)
{
    VECTOR point, segment;
    uint32 models, model, component, radius, x, y, z, scale, callback;
    FUNCTION_MARKER(0x800A1A28, "GAME.OVL");
    model = r_u32(first + 0x74u);
    models = r_u32(0x80046178u);
    component = (uint32)(sint32)r_s16(first + 0x120u);
    radius = r_u32(models + model * 132u + 0x6Cu);
    x = (uint32)((sint32)(component * radius) >> 12);
    model = r_u32(first + 0x74u);
    component = (uint32)(sint32)r_s16(first + 0x126u);
    radius = r_u32(models + model * 132u + 0x6Cu);
    y = (uint32)((sint32)(component * radius) >> 12);
    model = r_u32(first + 0x74u);
    component = (uint32)(sint32)r_s16(first + 0x12Cu);
    radius = r_u32(models + model * 132u + 0x6Cu);
    z = (uint32)((sint32)(component * radius) >> 12);
    scale = r_u32(first + 0x84u);
    if ((sint32)scale > 0) {
        x <<= scale & 31u;
        scale = r_u32(first + 0x84u);
        y <<= scale & 31u;
        scale = r_u32(first + 0x84u);
        z <<= scale & 31u;
    } else {
        if ((sint32)scale < 0)
            scale = 0u - scale;
        x = (uint32)((sint32)x >> (scale & 31u));
        scale = r_u32(first + 0x84u);
        if ((sint32)scale < 0)
            scale = 0u - scale;
        y = (uint32)((sint32)y >> (scale & 31u));
        scale = r_u32(first + 0x84u);
        if ((sint32)scale < 0)
            scale = 0u - scale;
        z = (uint32)((sint32)z >> (scale & 31u));
    }
    x += r_u32(first + 8u);
    y += r_u32(first + 12u);
    z += r_u32(first + 16u);
    point.vx = (sint32)x;
    point.vy = (sint32)y;
    point.vz = (sint32)z;
    segment.vx = (sint32)(x - (r_u32(first + 0xFCu) << 3));
    segment.vy = (sint32)(y - (r_u32(first + 0x100u) << 3));
    segment.vz = (sint32)(z - (r_u32(first + 0x104u) << 3));
    // These velocity reads have no consumer in the original call
    (void)r_u32(first + 0x14u);
    (void)r_u32(first + 0x18u);
    (void)r_u32(first + 0x1Cu);
    if (game_test_model_segment_native(second, &point, &segment, 0u) != 0) {
        callback = r_u32(first + 0x5Cu);
        psx_dispatch_guest_call(callback, 1u, first, 0u, 0u);
        w_u32(second + 0x40u, 0x800A19D4u);
        w_u32(second + 0x9Cu, 60u);
    }
}

static void game_increment_object_loss_counter(uint32 total, uint32 credited, uint32 attacker, uint32 projectile)
{
    uint32 value, source, player;
    value = r_u16(total);
    w_u16(total, (uint16)(value + 1u));
    if (projectile != 0u) {
        source = r_u32(projectile + 0x58u);
        player = r_u32(0x80040FDCu);
        if (source == player)
            goto credit_player_loss;
    }
    if (attacker == 0u)
        return;
    source = r_u32(attacker + 0x160u);
    player = r_u32(0x80040FDCu);
    if (source != player)
        return;
credit_player_loss:
    value = r_u16(credited);
    w_u16(credited, (uint16)(value + 1u));
}

void game_record_destroyed_object(uint32 object, uint32 attacker, uint32 projectile)
{
    uint32 kind, flags;
    FUNCTION_MARKER(0x800A1490, "GAME.OVL");
    kind = r_u32(object);
    if (kind == 3u) {
        flags = r_u32(object + 0x114u);
        if ((flags & 16u) != 0u)
            game_increment_object_loss_counter(0x8005961Eu, 0x80059626u, attacker, projectile);
        else
            game_increment_object_loss_counter(0x80059620u, 0x80059628u, attacker, projectile);
        kind = r_u32(object);
    }
    if (kind == 1u) {
        flags = r_u32(object + 0x114u);
        if ((flags & 16u) != 0u)
            game_increment_object_loss_counter(0x80059622u, 0x8005962Au, attacker, projectile);
        else
            game_increment_object_loss_counter(0x80059624u, 0x8005962Cu, attacker, projectile);
    }
}

void game_finish_special_mission_collision(uint32 object)
{
    uint32 remaining, callback;
    FUNCTION_MARKER(0x800A19D4, "GAME.OVL");
    remaining = r_u32(object + 0x9Cu) - 1u;
    w_u32(object + 0x9Cu, remaining);
    if ((sint32)remaining <= 0) {
        game_record_destroyed_object(object, 0u, 0u);
        callback = r_u32(object + 0x5Cu);
        psx_dispatch_guest_call(callback, 1u, object, 0u, 0u);
    }
}

static uint32 game_collision_dot_guest(uint32 coefficients, uint32 stride, const uint32 *vector)
{
    uint32 value;
    value = (uint32)(sint32)r_s16(coefficients) * vector[0];
    value += (uint32)(sint32)r_s16(coefficients + stride) * vector[1];
    value += (uint32)(sint32)r_s16(coefficients + stride * 2u) * vector[2];
    return value;
}

static uint32 game_collision_bounds_contain(uint32 object, uint32 bounds, const uint32 *point)
{
    uint32 extents[3], index, value, center;
    extents[0] = (uint32)(sint32)r_s16(bounds + 8u);
    extents[1] = (uint32)(sint32)r_s16(bounds + 10u);
    extents[2] = (uint32)(sint32)r_s16(bounds + 12u);
    if (r_s32(object + 0x84u) < 4) {
        extents[0] <<= 1;
        extents[1] <<= 1;
        extents[2] <<= 1;
    }
    for (index = 0u; index < 3u; ++index) {
        center = (uint32)(sint32)r_s16(bounds + index * 2u);
        value = point[index] - center;
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)extents[index] < (sint32)value)
            return 0u;
    }
    return 1u;
}

static uint32 game_collision_signed_quotient(uint32 numerator, uint32 denominator)
{
    if (denominator == 0u)
        xport_mips_break(7u);
    if (denominator == 0xFFFFFFFFu && numerator == 0x80000000u)
        xport_mips_break(0x5Du);
    return (uint32)((sint64)(sint32)numerator / (sint32)denominator);
}

sint32 game_test_model_segment_native(uint32 object, const VECTOR *point, VECTOR *segment, uint32 output)
{
    VECTOR box_point;
    uint32 start[3], end[3], local_start[3], local_end[3], difference[3], relative[3], intersection[3];
    uint32 model, descriptor, root, radius, scale, shift, index, axis, value, extent, center;
    uint32 group, group_count, group_index, child, child_count, child_index, face_cursor, face_index, face;
    uint32 player, source, px, py, pz, x, y, z, negative_distance, denominator, numerator, edge, normal;
    sint32 start_distance, end_distance, distance;
    FUNCTION_MARKER(0x80098604, "GAME.OVL");
    value = (uint32)point->vx;
    if (value == (uint32)segment->vx && point->vy == segment->vy && point->vz == segment->vz) {
        x = (uint32)(r_s32(object + 0x14u) >> 5);
        segment->vx = (sint32)(value + x);
        y = (uint32)(r_s32(object + 0x18u) >> 5);
        segment->vy = (sint32)((uint32)point->vy + y);
        z = (uint32)(r_s32(object + 0x1Cu) >> 5);
        segment->vz = (sint32)((uint32)point->vz + z);
    }
    model = r_u32(object + 0x74u);
    descriptor = r_u32(r_u32(0x80046178u) + model * 132u + 4u);
    if (descriptor == 0u)
        return 0;
    start[0] = (uint32)point->vx - r_u32(object + 8u);
    root = r_u32(descriptor + 8u);
    radius = r_u32(descriptor + 16u);
    start[1] = (uint32)point->vy - r_u32(object + 12u);
    start[2] = (uint32)point->vz - r_u32(object + 16u);
    end[0] = (uint32)segment->vx - r_u32(object + 8u);
    end[1] = (uint32)segment->vy - r_u32(object + 12u);
    end[2] = (uint32)segment->vz - r_u32(object + 16u);
    scale = r_u32(object + 0x84u);
    shift = ((sint32)scale < 0 ? 0u - scale : scale) & 31u;
    if ((sint32)scale > 0) {
        for (index = 0u; index < 3u; ++index) {
            start[index] = (uint32)((sint32)start[index] >> shift);
            end[index] = (uint32)((sint32)end[index] >> shift);
        }
    } else if ((sint32)scale < 0) {
        for (index = 0u; index < 3u; ++index) {
            start[index] <<= shift;
            end[index] <<= shift;
        }
    }
    for (index = 0u; index < 6u; ++index) {
        value = index < 3u ? start[index] : end[index - 3u];
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)radius < (sint32)value)
            return 0;
    }
    if ((r_u32(object + 0x114u) & 4u) != 0u) {
        box_point.vx = (sint32)start[0];
        box_point.vy = (sint32)start[1];
        box_point.vz = (sint32)start[2];
        return (sint32)game_test_object_collision_box_native(object, &box_point);
    }
    w_u16(output + 0x74u, 1u);
    for (axis = 0u; axis < 3u; ++axis) {
        value = game_collision_dot_guest(object + 0x11Cu + axis * 2u, 6u, start);
        local_start[axis] = (uint32)((sint32)value >> 12);
    }
    for (axis = 0u; axis < 3u; ++axis) {
        center = (uint32)(sint32)r_s16(root + axis * 2u);
        extent = (uint32)(sint32)r_s16(root + 8u + axis * 2u);
        value = local_start[axis] - center;
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)extent < (sint32)value)
            return 0;
    }
    value = game_collision_dot_guest(object + 0x11Cu, 6u, end);
    group_count = r_u32(root + 0x18u);
    local_end[0] = (uint32)((sint32)value >> 12);
    for (axis = 1u; axis < 3u; ++axis) {
        value = game_collision_dot_guest(object + 0x11Cu + axis * 2u, 6u, end);
        local_end[axis] = (uint32)((sint32)value >> 12);
    }
    if ((sint32)group_count <= 0)
        return 0;
    group = root + 0x1Cu;
    group_index = 0u;
    do {
        if (!game_collision_bounds_contain(object, group, local_start))
            goto next_model_group;
        w_u16(output + 0x74u, 1u);
        value = r_u32(group + 0x10u);
        child_count = r_u32(group + 0x18u);
        child = root + value;
        if ((sint32)child_count <= 0)
            goto next_model_group;
        child_index = 0u;
        do {
            if (!game_collision_bounds_contain(object, child, local_start))
                goto next_model_child;
            if (r_u8(object + 0x275u) == 0u) {
                source = r_u32(output + 0x58u);
                player = r_u32(0x80040FDCu);
                if (source != player)
                    goto accept_model_bounds;
            }
            player = r_u32(0x80040FDCu);
            px = (uint32)point->vx;
            py = (uint32)point->vy;
            x = r_u32(player + 8u);
            y = r_u32(player + 12u);
            z = r_u32(player + 16u);
            pz = (uint32)point->vz;
            distance = vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
            if (distance > 100000) {
                source = r_u32(output + 0x58u);
                player = r_u32(0x80040FDCu);
                if (source != player)
                    goto accept_model_bounds;
            }
            if (r_s32(child + 0x14u) <= 0)
                goto next_model_child;
            for (axis = 0u; axis < 3u; ++axis)
                difference[axis] = local_end[axis] - local_start[axis];
            face_index = 0u;
            face_cursor = child + 0x1Cu;
            do {
                index = (uint32)(sint32)r_s16(face_cursor);
                face = r_u32(descriptor) + index * 60u;
                for (axis = 0u; axis < 3u; ++axis)
                    relative[axis] = local_start[axis] - (uint32)(sint32)r_s16(face + 12u + axis * 2u);
                value = game_collision_dot_guest(face + 4u, 2u, relative);
                start_distance = (sint32)value >> 12;
                if (start_distance >= 0)
                    goto next_model_face;
                for (axis = 0u; axis < 3u; ++axis)
                    relative[axis] = local_end[axis] - (uint32)(sint32)r_s16(face + 12u + axis * 2u);
                value = game_collision_dot_guest(face + 4u, 2u, relative);
                end_distance = (sint32)value >> 12;
                if (end_distance <= 0)
                    goto next_model_face;
                negative_distance = 0u - (uint32)start_distance;
                denominator = negative_distance + (uint32)end_distance;
                for (axis = 0u; axis < 3u; ++axis) {
                    numerator = difference[axis] * negative_distance;
                    intersection[axis] = local_start[axis] + game_collision_signed_quotient(numerator, denominator);
                }
                w_u32(0x800FA210u, intersection[0]);
                w_u32(0x800FA214u, intersection[1]);
                w_u32(0x800FA218u, intersection[2]);
                for (axis = 0u; axis < 3u; ++axis)
                    relative[axis] = intersection[axis] - (uint32)(sint32)r_s16(face + 12u + axis * 2u);
                for (edge = 0u; edge < 2u; ++edge) {
                    value = game_collision_dot_guest(face + 0x1Cu + edge * 8u, 2u, relative);
                    if ((sint32)value > 0)
                        goto next_model_face;
                }
                for (axis = 0u; axis < 3u; ++axis)
                    relative[axis] = intersection[axis] - (uint32)(sint32)r_s16(face + 20u + axis * 2u);
                value = game_collision_dot_guest(face + 0x2Cu, 2u, relative);
                if ((sint32)value > 0)
                    goto next_model_face;
                if (r_u32(face) == 0x44415143u) {
                    value = game_collision_dot_guest(face + 0x34u, 2u, relative);
                    if ((sint32)value > 0)
                        goto next_model_face;
                }
                normal = (uint32)(sint32)r_s16(face + 4u);
                w_u32(0x801079B0u, normal);
                normal = (uint32)(sint32)r_s16(face + 6u);
                w_u32(0x801079B4u, normal);
                normal = (uint32)(sint32)r_s16(face + 8u);
                w_u32(0x801079B8u, normal);
                return 1;
next_model_face:
                value = r_u32(child + 0x14u);
                ++face_index;
                face_cursor += 2u;
            } while ((sint32)face_index < (sint32)value);
            goto next_model_child;
accept_model_bounds:
            w_u32(0x800FA210u, local_start[0]);
            w_u32(0x800FA214u, local_start[1]);
            w_u32(0x800FA218u, local_start[2]);
            return 1;
next_model_child:
            ++child_index;
            value = r_u32(child + 0x10u);
            child += value;
        } while ((sint32)child_index < (sint32)child_count);
next_model_group:
        group += 0x1Cu;
        ++group_index;
    } while ((sint32)group_index < (sint32)group_count);
    return 0;
}

static uint32 game_collision_checked_sub(uint32 first, uint32 second, uint32 pc)
{
    sint64 result = (sint64)(sint32)first - (sint32)second;
    if (result < -2147483647LL - 1 || result > 2147483647LL)
        xport_mips_overflow_exception(pc);
    return (uint32)result;
}

void game_collide_effects_with_object(uint32 effect, uint32 object)
{
    uint32 flags, source, player, kind, object_x, object_y, object_z, limit, link, effect_flags, type, excluded;
    uint32 x, y, z, dx, dy, dz, vx, vy, vz, radius, low, high, distance, check_x, check_y, check_z;
    FUNCTION_MARKER(0x800CE104, "GAME.OVL");
    if (effect == 0u)
        return;
    flags = r_u32(object + 0x114u);
    source = r_u32(effect + 0x58u);
    if ((flags & 0x8000u) != 0u)
        return;
    player = r_u32(0x80040FDCu);
    (void)game_collision_checked_sub(source, player, 0x800CE164u);
    kind = r_u32(object);
    object_x = r_u32(object + 8u);
    object_y = r_u32(object + 12u);
    object_z = r_u32(object + 16u);
    limit = r_u32(0x800F4EA4u);
    do {
        link = r_u32(effect + 0x70u);
        if (kind == 0u)
            return;
        effect_flags = r_u16(effect + 0x46u);
        if (link != 0u && r_u16(link + 0x74u) == 0u)
            goto next_object_effect;
        type = r_u16(effect + 0x3Cu);
        excluded = r_u16(effect + 0x64u);
        excluded = game_collision_checked_sub(excluded, kind, 0x800CE1C8u);
        if ((effect_flags & 1u) == 0u || excluded == 0u || type == 0u)
            goto next_object_effect;
        x = r_u32(effect);
        y = r_u32(effect + 4u);
        z = r_u32(effect + 8u);
        dx = game_collision_checked_sub(x, object_x, 0x800CE1E8u);
        dy = game_collision_checked_sub(y, object_y, 0x800CE1ECu);
        dz = game_collision_checked_sub(z, object_z, 0x800CE1F0u);
        if ((sint32)dx <= 0)
            dx = game_collision_checked_sub(0u, dx, 0x800CE1FCu);
        if ((sint32)dy <= 0)
            dy = game_collision_checked_sub(0u, dy, 0x800CE208u);
        if ((sint32)dz <= 0)
            dz = game_collision_checked_sub(0u, dz, 0x800CE214u);
        if ((r_u32(object + 0x114u) & 0x800u) == 0u) {
            radius = r_u16(effect + 0x40u);
            if (dx > radius || dy > radius || dz > radius)
                goto next_object_effect;
            w_u16(effect + 0x74u, 1u);
            low = dx < dz ? dx : dz;
            high = dx < dz ? dz : dx;
            distance = (low * 5u + (high << 4)) >> 4;
            low = distance < dy ? distance : dy;
            high = distance < dy ? dy : distance;
            distance = (low * 5u + (high << 4)) >> 4;
            if (distance <= radius)
                game_resolve_sphere_effect_hit(object, effect);
        } else {
            if (r_u16(effect + 0x3Cu) == 0xD5u)
                goto next_object_effect;
            check_x = game_collision_checked_sub(dx, limit, 0x800CE30Cu);
            check_y = game_collision_checked_sub(dy, limit, 0x800CE314u);
            if ((sint32)check_x > 0)
                goto next_object_effect;
            check_z = game_collision_checked_sub(dz, limit, 0x800CE31Cu);
            if ((sint32)check_y > 0 || (sint32)check_z > 0)
                goto next_object_effect;
            w_u32(0x800F31A0u, x);
            w_u32(0x800F31A4u, y);
            w_u32(0x800F31A8u, z);
            vx = r_u32(effect + 0x20u);
            vy = r_u32(effect + 0x24u);
            vz = r_u32(effect + 0x28u);
            vx = game_collision_checked_sub(x, vx, 0x800CE350u);
            vy = game_collision_checked_sub(y, vy, 0x800CE354u);
            vz = game_collision_checked_sub(z, vz, 0x800CE358u);
            w_u32(0x800F31B0u, vx);
            w_u32(0x800F31B4u, vy);
            w_u32(0x800F31B8u, vz);
            if (game_test_model_segment_native(object,
                (const VECTOR *)psx_addr(0x800F31A0u, 12u),
                (VECTOR *)psx_addr(0x800F31B0u, 12u), effect) != 0)
                game_resolve_model_effect_hit(object, effect);
        }
next_object_effect:
        effect = r_u32(effect + 0x48u);
    } while (effect != 0u);
}

static void game_store_effect_hit_metadata(uint32 object, uint32 effect, uint32 owner)
{
    uint32 value;
    w_u32(object + 0x1B0u, owner);
    value = r_u16(effect + 0x5Cu);
    w_u16(object + 0x1B4u, (uint16)value);
    value = r_u32(effect + 0x58u);
    w_u32(object + 0x1A8u, value);
    value = r_u16(effect + 0x5Cu);
    w_u16(object + 0x1ACu, (uint16)value);
    value = r_u32(effect + 0x20u);
    w_u16(object + 0x1BCu, (uint16)value);
    value = r_u32(effect + 0x24u);
    w_u16(object + 0x1BEu, (uint16)value);
    value = r_u32(effect + 0x28u);
    w_u16(object + 0x1C0u, (uint16)value);
}

static void game_notify_effect_hit(uint32 object, uint32 effect)
{
    uint32 kind, profile, callback;
    kind = (uint32)(sint32)r_s16(effect + 0x3Cu);
    profile = r_u32(effect + 0x68u);
    callback = r_u32(object + 0x44u);
    game_dispatch_object_hit(callback, object, effect, effect + 0x20u, 0u, effect, kind, profile);
    game_mark_effect_for_removal(effect);
}

void game_resolve_sphere_effect_hit(uint32 object, uint32 effect)
{
    uint32 owner;
    FUNCTION_MARKER(0x800B22CC, "GAME.OVL");
    owner = r_u32(effect + 0x58u);
    game_store_effect_hit_metadata(object, effect, owner);
    game_apply_effect_impulse(object, effect, effect + 0x20u, 12u);
    game_notify_effect_hit(object, effect);
}

void game_resolve_model_effect_hit(uint32 object, uint32 effect)
{
    uint32 scale, value, component, owner;
    FUNCTION_MARKER(0x800B2390, "GAME.OVL");
    ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u),
        (VECTOR *)psx_addr(0x800FA210u, 12u), (VECTOR *)psx_addr(0x800FA210u, 16u));
    scale = r_u32(object + 0x84u);
    if ((sint32)scale >= 0) {
        value = r_u32(0x800FA210u);
        w_u32(effect, value << (scale & 31u));
        value = r_u32(0x800FA214u);
        scale = r_u32(object + 0x84u);
        w_u32(effect + 4u, value << (scale & 31u));
        value = r_u32(0x800FA218u);
        scale = r_u32(object + 0x84u);
        value <<= scale & 31u;
    } else {
        value = r_u32(0x800FA210u);
        scale = (0u - scale) & 31u;
        w_u32(effect, (uint32)((sint32)value >> scale));
        value = r_u32(0x800FA214u);
        w_u32(effect + 4u, (uint32)((sint32)value >> scale));
        value = r_u32(0x800FA218u);
        value = (uint32)((sint32)value >> scale);
    }
    w_u32(effect + 8u, value);
    value = r_u32(effect);
    component = r_u32(object + 8u);
    w_u32(effect, value + component);
    value = r_u32(effect + 4u);
    component = r_u32(object + 12u);
    w_u32(effect + 4u, value + component);
    value = r_u32(effect + 8u);
    component = r_u32(object + 16u);
    owner = r_u32(effect + 0x58u);
    w_u32(effect + 8u, value + component);
    game_store_effect_hit_metadata(object, effect, owner);
    game_notify_effect_hit(object, effect);
}

void game_apply_effect_impulse(uint32 object, uint32 effect, uint32 velocity, uint32 strength)
{
    VECTOR relative_velocity, relative_position, forward, radial, side, up;
    uint32 value, component, player, alignment, product, roll;
    FUNCTION_MARKER(0x800A728C, "GAME.OVL");
    value = r_u32(object + 0x14u);
    component = r_u32(velocity);
    relative_velocity.vx = (sint32)(component - (uint32)((sint32)value >> 8));
    value = r_u32(object + 0x18u);
    component = r_u32(velocity + 4u);
    relative_velocity.vy = (sint32)(component - (uint32)((sint32)value >> 8));
    value = r_u32(object + 0x1Cu);
    component = r_u32(velocity + 8u);
    relative_velocity.vz = (sint32)(component - (uint32)((sint32)value >> 8));
    if ((r_u32(object + 0x114u) & 0x240000u) != 0u)
        return;
    player = r_u32(0x80040FDCu);
    if (object == player) {
        value = r_u32(object + 0x234u);
        strength = 0u - strength;
        strength = (uint32)((sint32)strength >> ((sint32)value > 0 ? 4 : 2));
    } else if (r_s32(object + 0x234u) > 0) {
        strength = (uint32)((sint32)strength >> 2);
    }
    value = r_u32(effect);
    relative_position.vx = (sint32)(value - r_u32(object + 8u));
    value = r_u32(effect + 4u);
    relative_position.vy = (sint32)(value - r_u32(object + 12u));
    value = r_u32(effect + 8u);
    relative_position.vz = (sint32)(value - r_u32(object + 16u));
    vector_normalize(&relative_velocity, &forward);
    vector_normalize(&relative_position, &radial);
    OuterProduct12(&forward, &radial, &side);
    vector_normalize(&side, &side);
    OuterProduct12(&forward, &side, &up);
    vector_normalize(&up, &up);
    if (object == r_u32(0x80040FDCu)) {
        w_u16(object + 0x13Cu, (uint16)side.vx);
        w_u16(object + 0x13Eu, (uint16)side.vy);
        w_u16(object + 0x140u, (uint16)side.vz);
        w_u16(object + 0x142u, (uint16)up.vx);
        w_u16(object + 0x144u, (uint16)up.vy);
        w_u16(object + 0x146u, (uint16)up.vz);
        w_u16(object + 0x148u, (uint16)forward.vx);
        w_u16(object + 0x14Au, (uint16)forward.vy);
    } else {
        w_u16(object + 0x13Cu, (uint16)side.vx);
        w_u16(object + 0x142u, (uint16)side.vy);
        w_u16(object + 0x148u, (uint16)side.vz);
        w_u16(object + 0x13Eu, (uint16)up.vx);
        w_u16(object + 0x144u, (uint16)up.vy);
        w_u16(object + 0x14Au, (uint16)up.vz);
        w_u16(object + 0x140u, (uint16)forward.vx);
        w_u16(object + 0x146u, (uint16)forward.vy);
    }
    w_u16(object + 0x14Cu, (uint16)forward.vz);
    product = (uint32)radial.vx * (uint32)forward.vx;
    alignment = (uint32)((sint32)product >> 12);
    product = (uint32)radial.vy * (uint32)forward.vy;
    alignment += (uint32)((sint32)product >> 12);
    product = (uint32)radial.vz * (uint32)forward.vz;
    alignment += (uint32)((sint32)product >> 12);
    if ((sint32)alignment < 0)
        alignment = 0u - alignment;
    product = (uint32)relative_velocity.vx * alignment;
    value = r_u32(object + 0x14u);
    w_u32(object + 0x14u, value + (uint32)((sint32)product >> 8));
    product = (uint32)relative_velocity.vy * alignment;
    roll = (4096u - alignment) * ((0u - strength) << 8);
    value = r_u32(object + 0x18u);
    w_u32(object + 0x18u, value + (uint32)((sint32)product >> 8));
    product = (uint32)relative_velocity.vz * alignment;
    w_u32(object + 0x15Cu, (uint32)((sint32)roll >> 9));
    value = r_u32(object + 0x1Cu);
    w_u32(object + 0x1Cu, value + (uint32)((sint32)product >> 8));
}

void game_collide_effect_chains(uint32 effect, uint32 other)
{
    uint32 enabled, x, y, z, radius, type, other_enabled, other_type, dx, dy, dz, low, high, distance, callback;
    FUNCTION_MARKER(0x800CD428, "GAME.OVL");
    enabled = r_u16(effect + 0x42u);
    if (other == 0u || enabled == 0u)
        return;
    x = r_u32(effect);
    y = r_u32(effect + 4u);
    z = r_u32(effect + 8u);
    radius = r_u16(effect + 0x40u);
    type = r_u16(effect + 0x3Cu);
    other_enabled = r_u16(other + 0x42u);
    if (type == 0u)
        return;
    do {
        other_type = r_u16(other + 0x3Cu);
        if (other_enabled == 0u || other_type == 0u)
            goto next_effect_pair;
        dx = r_u32(other);
        dy = r_u32(other + 4u);
        dz = r_u32(other + 8u);
        dx = game_collision_checked_sub(dx, x, 0x800CD4B0u);
        dy = game_collision_checked_sub(dy, y, 0x800CD4B4u);
        dz = game_collision_checked_sub(dz, z, 0x800CD4B8u);
        if ((sint32)dx <= 0)
            dx = game_collision_checked_sub(0u, dx, 0x800CD4C4u);
        if (dx > radius)
            goto next_effect_pair;
        if ((sint32)dy <= 0)
            dy = game_collision_checked_sub(0u, dy, 0x800CD4DCu);
        if (dy > radius)
            goto next_effect_pair;
        if ((sint32)dz <= 0)
            dz = game_collision_checked_sub(0u, dz, 0x800CD4F4u);
        if (dz > radius)
            goto next_effect_pair;
        low = dx < dz ? dx : dz;
        high = dx < dz ? dz : dx;
        distance = (low * 5u + (high << 4)) >> 4;
        low = distance < dy ? distance : dy;
        high = distance < dy ? dy : distance;
        distance = (low * 5u + (high << 4)) >> 4;
        if (distance <= radius) {
            callback = r_u32(effect + 0x60u);
            psx_dispatch_guest_call(callback, 2u, effect, other, 0u);
            game_mark_effect_for_removal(other);
        }
next_effect_pair:
        other = r_u32(other + 0x50u);
        if (other != 0u)
            other_enabled = r_u16(other + 0x42u);
    } while (other != 0u);
}

static uint32 game_curve_checked_add(uint32 first, uint32 second, uint32 pc)
{
    sint64 result = (sint64)(sint32)first + (sint32)second;
    if (result < -2147483647LL - 1 || result > 2147483647LL)
        xport_mips_overflow_exception(pc);
    return (uint32)result;
}

static void game_emit_jittered_curve_line(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 depth, uint32 color, uint32 high_pc, uint32 low_pc, uint32 advance_pc)
{
    uint32 packet, base, offset, slot, value;
    packet = r_u32(0x80040690u);
    w_u32(packet + 4u, color);
    w_u32(packet + 12u, color);
    w_u8(packet + 7u, 0x52u);
    w_u8(packet + 3u, 4u);
    w_u16(packet + 8u, (uint16)x0);
    w_u16(packet + 10u, (uint16)y0);
    w_u16(packet + 16u, (uint16)x1);
    w_u16(packet + 18u, (uint16)y1);
    base = r_u32(0x800421ACu);
    if ((sint32)depth < 16384) {
        if ((sint32)depth < 0)
            depth = 0u;
        offset = ((depth >> 6) + 64u) << 2;
        slot = game_curve_checked_add(offset, base, low_pc);
    } else {
        if ((sint32)depth > 0x3FFFFE)
            depth = 0x3FFFFEu;
        offset = ((depth >> 11) + 320u) << 2;
        slot = game_curve_checked_add(offset, base, high_pc);
    }
    AddPrim(psx_addr(slot, 4u), psx_addr(packet, 4u));
    value = r_u32(0x80040690u);
    value = game_curve_checked_add(value, 20u, advance_pc);
    w_u32(0x80040690u, value);
}

void game_draw_jittered_curve_native(VECTOR *points, uint32 camera, uint32 jitter_shift)
{
    const sint32 *coordinate[4][3] = {
        {&points[0].vx, &points[0].vy, &points[0].vz},
        {&points[1].vx, &points[1].vy, &points[1].vz},
        {&points[2].vx, &points[2].vy, &points[2].vz},
        {&points[3].vx, &points[3].vy, &points[3].vz}
    };
    SVECTOR vertex;
    uint32 weight[3], value[4], difference[3], product[3], position[3];
    uint32 index, axis, term, camera_component, base, delta_pc, add_pc, seed, noise, depth;
    uint32 x0, y0, x1, y1, screen_address, depth_address, status_address;
    sint32 range_sum, screen, flags;
    FUNCTION_MARKER(0x800CF9F4, "GAME.OVL");
    for (index = 0u; index < 16u; ++index) {
        weight[0] = r_u32(0x800F8108u + index * 4u);
        weight[1] = r_u32(0x800F8150u + index * 4u);
        weight[2] = r_u32(0x800F8190u + index * 4u);
        for (axis = 0u; axis < 3u; ++axis) {
            camera_component = r_u32(camera + 8u + axis * 4u);
            value[0] = (uint32)*coordinate[0][axis];
            value[1] = (uint32)*coordinate[1][axis];
            value[2] = (uint32)*coordinate[2][axis];
            value[3] = (uint32)*coordinate[3][axis];
            delta_pc = 0x800CFA8Cu + axis * 0x54u;
            for (term = 0u; term < 3u; ++term)
                difference[term] = game_collision_checked_sub(value[term + 1u], value[0], delta_pc + term * 4u);
            for (term = 0u; term < 3u; ++term)
                product[term] = difference[term] * weight[term];
            base = game_collision_checked_sub(value[0], camera_component, 0x800CFAB0u + axis * 0x54u);
            add_pc = 0x800CFAC0u + axis * 0x54u;
            for (term = 0u; term < 3u; ++term)
                base = game_curve_checked_add(base, (uint32)((sint32)product[term] >> 8), add_pc + term * 4u);
            position[axis] = base;
        }
        for (axis = 0u; axis < 3u; ++axis) {
            seed = r_u32(0x800311BCu);
            seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
            w_u32(0x800311BCu, seed);
            noise = ((seed & 255u) - 127u) << (jitter_shift & 31u);
            position[axis] = game_curve_checked_add(position[axis], noise, 0x800CFBA8u + axis * 0x38u);
        }
        range_sum = 0;
        for (axis = 0u; axis < 3u; ++axis) {
            position[axis] = (uint32)((sint32)position[axis] >> 4);
            range_sum += (((sint32)position[axis] >> 15) + 1) >> 1;
        }
        if (range_sum != 0)
            return;
        vertex.vx = (sint16)position[0];
        vertex.vy = (sint16)position[1];
        vertex.vz = (sint16)position[2];
        depth = (uint32)gte_project_full_depth(&vertex, &screen, &flags);
        seed = xport_gte_read_data(31u);
        w_u32(0x800F2AC8u + index * 4u, seed);
        w_u32(0x800510F8u + index * 4u, depth << 4);
        w_u32(0x80033D50u + index * 4u, (uint32)screen);
    }
    for (index = 0u; index < 15u; ++index) {
        screen_address = 0x80033D50u + index * 4u;
        depth_address = 0x800510F8u + index * 4u;
        status_address = 0x800F2AC8u + index * 4u;
        x0 = (uint32)(sint32)r_s16(screen_address);
        y0 = (uint32)(sint32)r_s16(screen_address + 2u);
        x1 = (uint32)(sint32)r_s16(screen_address + 4u);
        y1 = (uint32)(sint32)r_s16(screen_address + 6u);
        if ((r_u32(status_address) >> 16) != 0u)
            continue;
        depth = game_collision_checked_sub(r_u32(depth_address), 16u, 0x800CFD30u);
        if ((sint32)depth <= 0)
            continue;
        game_emit_jittered_curve_line(x0, y0, x1, y1, depth, 0xFFFFFFu,
            0x800CFDB4u, 0x800CFDD8u, 0x800CFE24u);
    }
    for (index = 0u; index < 15u; ++index) {
        screen_address = 0x80033D50u + index * 4u;
        depth_address = 0x800510F8u + index * 4u;
        status_address = 0x800F2AC8u + index * 4u;
        x0 = (uint32)(sint32)r_s16(screen_address);
        y0 = (uint32)(sint32)r_s16(screen_address + 2u);
        x1 = (uint32)(sint32)r_s16(screen_address + 4u);
        y1 = (uint32)(sint32)r_s16(screen_address + 6u);
        if ((r_u32(status_address) >> 16) != 0u)
            continue;
        depth = game_collision_checked_sub(r_u32(depth_address), 16u, 0x800CFEACu);
        if ((sint32)depth <= 0)
            continue;
        game_emit_jittered_curve_line(x0 - 1u, y0, x1 - 1u, y1, depth, 0xFF0000u,
            0x800CFF38u, 0x800CFF5Cu, 0x800CFFA8u);
        game_emit_jittered_curve_line(x0 + 1u, y0, x1 + 1u, y1, depth, 0xFF0000u,
            0x800D0034u, 0x800D0058u, 0x800D00A4u);
        game_emit_jittered_curve_line(x0, y0 - 1u, x1, y1 - 1u, depth, 0xFF0000u,
            0x800D0138u, 0x800D015Cu, 0x800D01A8u);
        game_emit_jittered_curve_line(x0, y0 + 1u, x1, y1 + 1u, depth, 0xFF0000u,
            0x800D0234u, 0x800D0258u, 0x800D02A4u);
    }
}

void game_draw_jittered_beam_native(VECTOR *points, uint32 unused, uint32 jitter_shift)
{
    uint32 camera, x, y, z, value;
    sint32 distance;
    FUNCTION_MARKER(0x800CA354, "GAME.OVL");
    camera = r_u32(0x80040C94u);
    x = (uint32)points[0].vx;
    value = r_u32(camera + 8u);
    x -= value;
    y = (uint32)points[0].vy;
    value = r_u32(camera + 12u);
    y -= value;
    z = (uint32)points[0].vz;
    value = r_u32(camera + 16u);
    z -= value;
    distance = vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    if (distance > 500000)
        return;
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    camera = r_u32(0x80040C94u);
    game_draw_jittered_curve_native(points, camera, jitter_shift);
}

static uint32 game_effect_next_seed(uint32 seed)
{
    return (uint32)((sint32)seed >> 1) ^ (seed << 3);
}

void game_emit_point_sparks(uint32 count, uint32 x, uint32 y, uint32 z)
{
    uint32 slot, offset, base, record, seed, value, next_slot, fresh_base;
    FUNCTION_MARKER(0x800AF72C, "GAME.OVL");
    if ((sint32)count <= 0)
        return;
    do {
        --count;
        slot = r_u32(0x800EF10Cu);
        base = r_u32(0x80123F64u);
        seed = r_u32(0x800EF50Cu);
        offset = slot * 28u;
        record = base + offset;
        seed = game_effect_next_seed(seed);
        value = (seed & 127u) - 64u;
        next_slot = slot + 1u;
        w_u32(0x800EF50Cu, seed);
        w_u16(record + 2u, (uint16)value);
        seed = game_effect_next_seed(seed);
        value = (seed & 127u) - 64u;
        w_u32(0x800EF50Cu, seed);
        w_u16(record + 4u, (uint16)value);
        seed = game_effect_next_seed(seed);
        w_u32(0x800EF50Cu, seed);
        value = (seed & 127u) - 64u;
        next_slot = (uint32)((sint32)next_slot % 450);
        w_u32(0x800EF10Cu, next_slot);
        w_u8(record, 1u);
        fresh_base = r_u32(0x80123F64u);
        w_u32(record + 8u, x);
        w_u32(record + 12u, y);
        w_u32(record + 16u, z);
        w_u16(record + 6u, (uint16)value);
        w_u32(record + 24u, 64u);
        w_u8(fresh_base + offset + 1u, 0u);
    } while ((sint32)count > 0);
}

void game_draw_projectile_arcs(uint32 object)
{
    VECTOR points[4];
    sint32 *coordinate[4][3] = {
        {&points[0].vx, &points[0].vy, &points[0].vz},
        {&points[1].vx, &points[1].vy, &points[1].vz},
        {&points[2].vx, &points[2].vy, &points[2].vz},
        {&points[3].vx, &points[3].vy, &points[3].vz}
    };
    uint32 direction[3], curve, axis, point_index, seed, noise, value, x, y, z;
    FUNCTION_MARKER(0x800B3EE8, "GAME.OVL");
    x = r_u32(object + 8u);
    y = r_u32(object + 12u);
    z = r_u32(object + 16u);
    game_emit_point_sparks(8u, x, y, z);
    points[0].vx = r_s32(object + 8u);
    points[0].vy = r_s32(object + 12u);
    points[0].vz = r_s32(object + 16u);
    for (curve = 0u; curve < 2u; ++curve) {
        seed = r_u32(0x800EF50Cu);
        for (axis = 0u; axis < 3u; ++axis) {
            seed = game_effect_next_seed(seed);
            direction[axis] = (seed & 511u) - 256u;
            w_u32(0x800EF50Cu, seed);
        }
        seed = game_effect_next_seed(seed);
        w_u32(0x800EF50Cu, seed);
        for (point_index = 1u; point_index < 4u; ++point_index) {
            for (axis = 0u; axis < (point_index == 3u ? 1u : 3u); ++axis) {
                noise = seed & 255u;
                seed = game_effect_next_seed(seed);
                value = r_u32(object + 8u + axis * 4u);
                w_u32(0x800EF50Cu, seed);
                *coordinate[point_index][axis] = (sint32)(value + direction[axis] * point_index + noise - 127u);
            }
        }
        value = r_u32(object + 12u);
        points[3].vy = (sint32)(value + direction[1] * 3u + (seed & 255u) - 127u);
        value = r_u32(object + 16u);
        seed = game_effect_next_seed(seed);
        w_u32(0x800EF50Cu, seed);
        points[3].vz = (sint32)(value + direction[2] * 3u + (seed & 255u) - 127u);
        game_draw_jittered_beam_native(points, 0u, 0u);
    }
}

void game_draw_projectile_sparks(uint32 object)
{
    VECTOR velocity;
    uint32 x, y, z;
    FUNCTION_MARKER(0x800B3E14, "GAME.OVL");
    velocity.vx = 0;
    velocity.vy = 0;
    velocity.vz = 0;
    x = r_u32(object + 8u);
    y = r_u32(object + 12u);
    z = r_u32(object + 16u);
    game_emit_point_sparks(8u, x, y, z);
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    x = r_u32(object + 8u);
    y = r_u32(object + 12u);
    z = r_u32(object + 16u);
    game_emit_radial_effect(x, y, z, 1200u, 0x805090u);
    x = r_u32(object + 8u);
    y = r_u32(object + 12u);
    z = r_u32(object + 16u);
    game_emit_radial_effect(x, y, z, 2048u, 0x801060u);
    game_emit_camera_particle_native((const VECTOR *)psx_addr(object + 8u, 12u), &velocity, 0x801060u, 1200u);
}

static uint32 game_model_index_remainder(uint32 index, uint32 count)
{
    if (count == 0u)
        xport_mips_break(7u);
    if (count == 0xFFFFFFFFu && index == 0x80000000u)
        xport_mips_break(0x5Du);
    return (uint32)((sint64)(sint32)index % (sint32)count);
}

void game_draw_model_arcs(uint32 object, uint32 first_index, uint32 second_index)
{
    VECTOR points[4];
    sint32 *coordinate[4][3] = {
        {&points[0].vx, &points[0].vy, &points[0].vz},
        {&points[1].vx, &points[1].vy, &points[1].vz},
        {&points[2].vx, &points[2].vy, &points[2].vz},
        {&points[3].vx, &points[3].vy, &points[3].vz}
    };
    uint32 model, models, descriptor, count, vertices, value, scale, left_shift, shift, jitter_shift;
    uint32 next_index[2], point_index, axis, vertex, seed, component, matrix;
    FUNCTION_MARKER(0x800CABFC, "GAME.OVL");
    model = r_u32(object + 0x74u);
    value = r_u32(first_index);
    models = r_u32(0x80046178u);
    scale = r_u32(object + 0x84u);
    descriptor = r_u32(models + model * 132u);
    count = r_u32(descriptor + 4u);
    vertices = r_u32(descriptor);
    jitter_shift = (uint32)((sint32)(scale + 3u) >> 2);
    if ((sint32)count < (sint32)value)
        w_u32(first_index, game_model_index_remainder(value, count));
    value = r_u32(second_index);
    if ((sint32)count < (sint32)value)
        w_u32(second_index, game_model_index_remainder(value, count));
    scale = r_u32(object + 0x84u);
    left_shift = (sint32)scale > 0;
    shift = ((sint32)scale < 0 ? 0u - scale : scale) & 31u;
    for (point_index = 0u; point_index < 2u; ++point_index) {
        for (axis = 0u; axis < 3u; ++axis) {
            value = r_u32(point_index == 0u ? first_index : second_index);
            vertex = vertices + (value << 3);
            component = (uint32)(sint32)r_s16(vertex + axis * 2u);
            if (left_shift != 0u)
                component <<= shift;
            else
                component = (uint32)((sint32)component >> shift);
            *coordinate[point_index == 0u ? 0u : 3u][axis] = (sint32)component;
        }
    }
    next_index[0] = game_model_index_remainder(r_u32(first_index) + 1u, count);
    next_index[1] = game_model_index_remainder(r_u32(second_index) + 1u, count);
    for (point_index = 1u; point_index < 3u; ++point_index) {
        vertex = vertices + (next_index[point_index - 1u] << 3);
        for (axis = 0u; axis < 3u; ++axis) {
            component = (uint32)(sint32)r_s16(vertex + axis * 2u);
            if (left_shift != 0u)
                component <<= shift;
            else
                component = (uint32)((sint32)component >> shift);
            *coordinate[point_index][axis] = (sint32)component;
        }
    }
    matrix = object + 0x11Cu;
    for (point_index = 0u; point_index < 3u; ++point_index) {
        ApplyMatrixLV((MATRIX *)psx_addr(matrix, 20u), &points[point_index], &points[point_index]);
        points[point_index].vx = (sint32)((uint32)points[point_index].vx + r_u32(object + 8u));
        points[point_index].vy = (sint32)((uint32)points[point_index].vy + r_u32(object + 12u));
        points[point_index].vz = (sint32)((uint32)points[point_index].vz + r_u32(object + 16u));
    }
    ApplyMatrixLV((MATRIX *)psx_addr(matrix, 20u), &points[3], &points[3]);
    points[3].vx = (sint32)((uint32)points[3].vx + r_u32(object + 8u));
    component = r_u32(object + 12u);
    seed = r_u32(0x800EF50Cu);
    points[3].vy = (sint32)((uint32)points[3].vy + component);
    component = r_u32(object + 16u);
    seed = game_effect_next_seed(seed);
    w_u32(0x800EF50Cu, seed);
    points[3].vz = (sint32)((uint32)points[3].vz + component);
    if ((seed & 3u) == 0u) {
        value = r_u32(first_index) + 1u;
        w_u32(first_index, game_model_index_remainder(value, count));
    }
    seed = game_effect_next_seed(r_u32(0x800EF50Cu));
    w_u32(0x800EF50Cu, seed);
    if ((seed & 3u) == 0u) {
        value = r_u32(second_index) + 1u;
        w_u32(second_index, game_model_index_remainder(value, count));
    }
    SetRotMatrix((MATRIX *)psx_addr(matrix, 20u));
    SetTransMatrix((MATRIX *)psx_addr(matrix, 32u));
    game_draw_jittered_beam_native(points, 0u, jitter_shift);
}

void game_update_object_fade_out(uint32 controller)
{
    uint32 object, flags, value, remaining, step, callback, index;
    FUNCTION_MARKER(0x800A5138, "GAME.OVL");
    object = r_u32(controller + 0x170u);
    if (r_u32(object) == 0u)
        goto finish_object_fade_out;
    flags = r_u32(object + 0x114u);
    w_u32(object + 0x3Cu, 0x800A5264u);
    w_u32(object + 0x114u, flags | 0x20u);
    value = r_u32(controller + 0xACu);
    w_u32(object + 0xACu, value);
    remaining = r_u32(controller + 0x9Cu) - 1u;
    w_u32(controller + 0x9Cu, remaining);
    if ((sint32)remaining > 0)
        return;
    for (index = 0u; index < 3u; ++index)
        game_draw_model_arcs(object, controller + 0xB0u + index * 8u, controller + 0xB4u + index * 8u);
    value = r_u32(controller + 0xACu);
    step = r_u32(controller + 0xA8u);
    value -= step;
    w_u32(controller + 0xACu, value);
    if ((sint32)value > 0)
        return;
    if (r_u32(0x800EF4E0u) == object)
        w_u32(0x800EF4E0u, 0u);
    flags = r_u32(object + 0x114u);
    if ((flags & 0x20u) != 0u)
        w_u32(object + 0x114u, flags ^ 0x20u);
    flags = r_u32(object + 0x114u);
    w_u32(object + 0x3Cu, 0x8002493Cu);
    w_u32(object + 0x114u, flags | 0x8000u);
finish_object_fade_out:
    callback = r_u32(controller + 0x48u);
    psx_dispatch_guest_call(callback, 1u, controller, 0u, 0u);
}

void game_draw_fading_object(uint32 object)
{
    uint32 brightness;
    FUNCTION_MARKER(0x800A5264, "GAME.OVL");
    brightness = r_u32(object + 0xACu);
    if ((sint32)brightness >= 256)
        brightness = 255u;
    render_set_semi_trans(1);
    light_scale_scene(brightness);
    game_draw_scene_object(object);
    light_scale_scene(255u);
    render_set_semi_trans(0);
}

void game_update_object_fade_in(uint32 object)
{
    uint32 index, flags, value, step;
    FUNCTION_MARKER(0x800A52D0, "GAME.OVL");
    for (index = 0u; index < 3u; ++index)
        game_draw_model_arcs(object, object + 0xB0u + index * 8u, object + 0xB4u + index * 8u);
    w_u32(object + 0x3Cu, 0x800A5264u);
    flags = r_u32(object + 0x114u);
    value = r_u32(object + 0xACu);
    step = r_u32(object + 0xA8u);
    value += step;
    w_u32(object + 0xACu, value);
    w_u32(object + 0x114u, flags | 0x20u);
    if ((sint32)value >= 255) {
        flags = r_u32(object + 0x114u);
        if ((flags & 0x20u) != 0u)
            w_u32(object + 0x114u, flags ^ 0x20u);
        w_u32(object + 0x40u, 0x8009AB58u);
        w_u32(object + 0x3Cu, 0x800D1C3Cu);
    }
}

static sint32 game_scene_weapon_type(void)
{
    uint32 slot = r_u32(0x800EF4CCu);
    return r_s16(0x80119C60u + slot * 52u);
}

void game_draw_scene_object(uint32 object)
{
    VECTOR velocity;
    SVECTOR vertex;
    uint32 visible, flags, camera, relative[3], value, magnitude, distance, seed, axis, intensity;
    uint32 mode, special_mode, depth, slot, threshold, target, dx, dy, squared, candidate_distance, current_distance;
    uint32 first, center, selected_type;
    sint32 screen, projection, projection_flags;
    FUNCTION_MARKER(0x800D1C3C, "GAME.OVL");
    visible = object_draw_visible(object);
    if ((r_u32(object + 0x114u) & 16u) == 0u) {
        camera = r_u32(0x80040C94u);
        relative[0] = (uint32)((sint32)(r_u32(object + 8u) - r_u32(camera + 8u)) >> 2);
        relative[1] = (uint32)((sint32)(r_u32(object + 12u) - r_u32(camera + 12u)) >> 2);
        relative[2] = (uint32)((sint32)(r_u32(object + 16u) - r_u32(camera + 16u)) >> 2);
        for (axis = 0u; axis < 3u; ++axis) {
            magnitude = (sint32)relative[axis] < 0 ? 0u - relative[axis] : relative[axis];
            if ((sint32)magnitude >= 0x4180)
                goto scene_nearest_done;
        }
        distance = (uint32)vector_length_approx((sint32)relative[0], (sint32)relative[1], (sint32)relative[2]) << 2;
        if ((sint32)distance < r_s32(0x800F279Cu)) {
            w_u32(0x800F279Cu, distance);
            w_u32(0x800F2798u, object);
        } else if ((sint32)distance < r_s32(0x800F27A4u)) {
            w_u32(0x800F27A4u, distance);
            w_u32(0x800F27A0u, object);
        }
    }
scene_nearest_done:
    if (r_s32(object + 0x238u) < 20) {
        if ((r_u32(object + 0x114u) & 16u) != 0u)
            goto scene_object_audio;
        seed = r_u32(0x800EF50Cu);
        seed = game_effect_next_seed(seed);
        velocity.vx = (sint32)((seed & 127u) - 63u);
        w_u32(0x800EF50Cu, seed);
        seed = game_effect_next_seed(seed);
        velocity.vy = (sint32)((seed & 127u) - 63u);
        w_u32(0x800EF50Cu, seed);
        seed = game_effect_next_seed(seed);
        w_u32(0x800EF50Cu, seed);
        velocity.vz = (sint32)((seed & 127u) - 63u);
        game_emit_camera_particle_native((const VECTOR *)psx_addr(object + 8u, 12u), &velocity, 0x104080u, 1024u);
    }
    if ((r_u32(object + 0x114u) & 16u) == 0u)
        goto scene_audio_done;
scene_object_audio:
    intensity = r_u32(object + 0xECu);
    if ((sint32)intensity > 0)
        game_accumulate_effect_audio(object + 8u, intensity);
scene_audio_done:
    if (visible == 0u)
        game_draw_object_details(object);
    special_mode = 0u;
    if ((sint32)visible > 0)
        goto scene_reject_target;
    mode = r_u32(0x800EF4BCu);
    special_mode = mode - 4u < 2u;
    if (special_mode != 0u) {
        if (r_u32(object) != 3u && game_scene_weapon_type() != 3)
            goto scene_enforce_target_mode;
        if ((r_u32(object + 0x114u) & 16u) != 0u || game_scene_weapon_type() != 1) {
            if (game_scene_weapon_type() != 3)
                goto scene_enforce_target_mode;
        }
    }
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    camera = r_u32(0x80040C94u);
    relative[0] = r_u32(object + 8u) - r_u32(camera + 8u);
    magnitude = (sint32)relative[0] < 0 ? 0u - relative[0] : relative[0];
    if (((sint32)magnitude >> 4) >= 32000)
        goto scene_reject_target;
    value = r_u32(object + 12u) - r_u32(camera + 12u);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if (((sint32)magnitude >> 4) >= 32000)
        goto scene_reject_target;
    value = r_u32(object + 16u) - r_u32(camera + 16u);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if (((sint32)magnitude >> 4) >= 32000)
        goto scene_reject_target;
    vertex.vx = (sint16)((sint32)relative[0] >> 4);
    value = r_u32(object + 12u) - r_u32(camera + 12u);
    vertex.vy = (sint16)((sint32)value >> 4);
    value = r_u32(object + 16u) - r_u32(camera + 16u);
    vertex.vz = (sint16)((sint32)value >> 4);
    depth = (uint32)RotTransPers(&vertex, &screen, &projection, &projection_flags);
    w_u16(object + 0x108u, (uint16)screen);
    slot = 0u;
    if (special_mode != 0u)
        slot = r_u32(0x800EF4CCu);
    w_u32(object + 0x10Cu, depth);
    w_u16(object + 0x10Au, (uint16)((uint32)screen >> 16));
    if (special_mode == 0u) {
        if (depth - 1u >= 3999u)
            goto scene_reject_target;
    } else {
        selected_type = (uint32)(sint32)r_s16(0x80119C60u + slot * 52u);
        threshold = selected_type == 3u ? 1000u : 4000u;
        if ((sint32)depth >= (sint32)threshold || (sint32)depth <= 0)
            goto scene_reject_target;
        if (r_u32(0x800EF4BCu) == 5u)
            goto scene_target_done;
    }
    target = r_u32(0x800EF4E0u);
    if (target == 0u) {
        w_u32(0x800EF4E0u, object);
        if (special_mode != 0u)
            w_u32(0x800EF48Cu, 512u);
        goto scene_target_done;
    }
    center = r_u32(0x80031198u);
    dx = (uint32)(sint32)(sint16)screen - center;
    if ((sint32)dx < 0)
        dx = 0u - dx;
    first = dx * dx;
    center = r_u32(0x800311B4u);
    dy = (uint32)(sint32)(sint16)((uint32)screen >> 16) - center;
    if ((sint32)dy < 0)
        dy = 0u - dy;
    squared = first + dy * dy;
    candidate_distance = squared;
    if ((sint32)squared > 0)
        candidate_distance = SquareRoot0((sint32)squared);
    target = r_u32(0x800EF4E0u);
    center = r_u32(0x80031198u);
    value = (uint32)(sint32)r_s16(target + 0x108u);
    dx = value - center;
    if ((sint32)dx < 0)
        dx = 0u - dx;
    first = dx * dx;
    value = (uint32)(sint32)r_s16(target + 0x10Au);
    center = r_u32(0x800311B4u);
    dy = value - center;
    if ((sint32)dy < 0)
        dy = 0u - dy;
    squared = first + dy * dy;
    current_distance = squared;
    if ((sint32)squared > 0)
        current_distance = SquareRoot0((sint32)squared);
    if ((sint32)candidate_distance < (sint32)current_distance) {
        if (special_mode != 0u && r_u32(0x800EF4E0u) != object)
            w_u32(0x800EF48Cu, 512u);
        w_u32(0x800EF4E0u, object);
    }
    goto scene_target_done;
scene_reject_target:
    if (r_u32(0x800EF4E0u) == object) {
        if (special_mode != 0u) {
            mode = r_u32(0x800EF4BCu);
            w_u32(0x800EF4E0u, 0u);
            if (mode == 5u)
                w_u32(0x800EF4BCu, 4u);
            w_u32(0x800EF48Cu, 512u);
        } else {
            w_u32(0x800EF4E0u, 0u);
        }
    }
scene_target_done:
    if (game_scene_weapon_type() == 3)
        goto scene_object_arcs;
scene_enforce_target_mode:
    if (r_u32(0x800EF4E0u) == object) {
        mode = r_u32(0x800EF4BCu);
        if (mode - 4u < 2u) {
            if (r_u32(object) == 1u || (r_u32(object + 0x114u) & 16u) != 0u)
                w_u32(0x800EF4E0u, 0u);
        }
    }
scene_object_arcs:
    flags = r_u32(object + 0x114u);
    if ((flags & 0x80000u) != 0u) {
        seed = r_u32(0x800EF50Cu);
        for (axis = 0u; axis < 3u; ++axis) {
            seed = game_effect_next_seed(seed);
            w_u32(object + 0xB0u + axis * 4u, seed & 0x7FFFu);
            w_u32(0x800EF50Cu, seed);
        }
        seed = game_effect_next_seed(seed);
        w_u32(0x800EF50Cu, seed);
        w_u32(object + 0xBCu, seed & 0x7FFFu);
        game_draw_model_arcs(object, object + 0xB0u, object + 0xB4u);
        game_draw_model_arcs(object, object + 0xB8u, object + 0xBCu);
    }
}

void game_accumulate_effect_audio(uint32 position, uint32 intensity)
{
    uint32 camera;
    FUNCTION_MARKER(0x800E0454, "GAME.OVL");
    camera = r_u32(0x80040C94u);
    game_accumulate_effect_audio_from_camera(position, intensity, camera);
}

void game_accumulate_effect_audio_from_camera(uint32 position, uint32 intensity, uint32 camera)
{
    uint32 x, y, z, camera_x, camera_y, camera_z, value, weight, axis, component, product, accumulator;
    sint32 distance;
    FUNCTION_MARKER(0x800E045C, "GAME.OVL");
    x = r_u32(position);
    y = r_u32(position + 4u);
    camera_x = r_u32(camera + 8u);
    camera_y = r_u32(camera + 12u);
    camera_z = r_u32(camera + 16u);
    z = r_u32(position + 8u);
    distance = vector_length_approx((sint32)(x - camera_x), (sint32)(y - camera_y), (sint32)(z - camera_z));
    w_u32(0x80046174u, (uint32)distance);
    if (distance < 0)
        w_u32(0x80046174u, 0u);
    value = r_u32(0x80046174u);
    weight = 0u;
    if ((sint32)value <= 65536) {
        value = 65536u - value;
        w_u32(0x80046174u, value);
        value <<= 12;
        if ((sint32)value < 0)
            value += 65535u;
        camera = r_u32(0x80040C94u);
        weight = (uint32)((sint32)value >> 16);
        for (axis = 0u; axis < 3u; ++axis) {
            value = r_u32(position + axis * 4u);
            component = r_u32(camera + 8u + axis * 4u);
            product = (value - component) * weight;
            accumulator = r_u32(0x80123F50u + axis * 4u);
            accumulator += (uint32)((sint32)product >> 12);
            w_u32(0x80123F50u + axis * 4u, accumulator);
        }
    }
    intensity = (uint32)((sint32)intensity >> 7);
    if ((sint32)intensity >= 256)
        intensity = 255u;
    value = (uint32)((sint32)((weight << 8) - weight) >> 12);
    product = value * intensity;
    value = (uint32)((sint32)product >> 8);
    if ((sint32)value > 0) {
        accumulator = r_u32(0x801089C8u);
        w_u32(0x801089C8u, accumulator + value);
    }
}

void game_draw_object_details(uint32 object)
{
    uint32 intensity, kind, denominator, numerator, record, old_value, product, first, second, offset;
    FUNCTION_MARKER(0x800C5C20, "GAME.OVL");
    intensity = r_u32(object + 0xECu);
    if (intensity == 0u && (r_u32(object + 0x114u) & 16u) != 0u)
        return;
    kind = r_u32(object + 4u);
    numerator = (intensity << 8) - intensity;
    denominator = r_u32(0x8005369Cu + kind * 60u) + 1u;
    intensity = game_collision_signed_quotient(numerator, denominator);
    record = r_u32(object + 0x1E8u);
    old_value = r_u32(record + 0x10u);
    if ((sint32)(old_value - intensity) >= 17)
        intensity = old_value - 16u;
    if ((sint32)intensity < 16)
        intensity = 16u;
    if ((sint32)intensity >= 256)
        intensity = 255u;
    if (record != 0u) {
        w_u32(record + 0x10u, intensity);
        if (r_u32(object + 0x1ECu) != 0u) {
            record = r_u32(object + 0x1E8u);
            first = (uint32)(sint32)r_s16(object + 0x1F8u);
            second = (uint32)(sint32)r_s16(record);
            product = first * second;
            if ((sint32)product > 0)
                w_u32(record + 0x10u, (uint32)((sint32)intensity >> 1));
        }
        record = r_u32(object + 0x1E8u);
        game_draw_attached_record(object, record, 1u);
    }
    for (offset = 0x1ECu; offset <= 0x1F0u; offset += 4u) {
        record = r_u32(object + offset);
        if (record == 0u)
            continue;
        w_u32(record + 0x10u, intensity);
        record = r_u32(object + offset);
        first = (uint32)(sint32)r_s16(object + 0x1F8u);
        second = (uint32)(sint32)r_s16(record);
        product = first * second;
        if ((sint32)product > 0)
            w_u32(record + 0x10u, (uint32)((sint32)intensity >> 1));
        record = r_u32(object + offset);
        game_draw_attached_record(object, record, 1u);
    }
    record = r_u32(object + 0x1F4u);
    if (record != 0u) {
        w_u32(record + 0x10u, intensity);
        record = r_u32(object + 0x1F4u);
        game_draw_attached_record(object, record, 1u);
    }
}

void game_update_model_transition(uint32 object)
{
    uint32 state, value, color, model, models, flags;
    FUNCTION_MARKER(0x800A2A94, "GAME.OVL");
    game_draw_model_arcs(object, object + 0xA8u, object + 0xACu);
    game_draw_model_arcs(object, object + 0xB0u, object + 0xB4u);
    game_draw_model_arcs(object, object + 0xB8u, object + 0xBCu);
    state = r_u32(object + 0x88u);
    if (state == 1u) {
        value = r_u32(object + 0xC0u) - 3u;
        w_u32(object + 0xC0u, value);
        if ((sint32)value <= 0)
            w_u32(object + 0xC0u, 0u);
        value = r_u32(object + 0xC0u);
        color = value + (value << 8) + (value << 16);
        w_u32(object + 0xD4u, color);
        if (color == 0u) {
            model = r_u32(0x800EDD3Cu);
            w_u32(object + 0x9Cu, 45u);
            w_u32(object + 0x88u, 2u);
            w_u32(object + 0x74u, model);
            models = r_u32(0x80046178u);
            w_u32(object + 0x64u, r_u32(models + model * 132u));
        }
    } else if (state == 2u) {
        value = r_u32(object + 0x9Cu) - 1u;
        w_u32(object + 0x9Cu, value);
        if ((sint32)value <= 0)
            w_u32(object + 0x88u, 3u);
    } else if (state == 3u) {
        value = r_u32(object + 0xC0u) + 3u;
        w_u32(object + 0xC0u, value);
        if ((sint32)value >= 128) {
            w_u32(object + 0xC0u, 128u);
            flags = r_u32(object + 0x114u);
            w_u32(object + 0x40u, 0x8009AB58u);
            if ((flags & 0x20000000u) != 0u)
                w_u32(object + 0x114u, flags ^ 0x20000000u);
        }
        value = r_u32(object + 0xC0u);
        w_u32(0x8011F130u, value);
        if ((sint32)value >= 129)
            w_u32(0x8011F130u, 128u);
        value = r_u32(0x8011F130u);
        w_u32(object + 0xD4u, value + (value << 8) + (value << 16));
    }
    game_update_object_state(object);
    game_update_object_movement(object);
}
