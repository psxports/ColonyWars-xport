#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>

void game_update_timed_rotating_part(uint32 object)
{
    uint32 remaining, callback, yaw, yaw_step, pitch, pitch_step;
    FUNCTION_MARKER(0x800DF5D0, "GAME.OVL");
    remaining = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, remaining);
    if ((sint32)remaining <= 0) {
        callback = r_u32(object + 0x48);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
        return;
    }
    yaw = r_u32(object + 0x28); yaw_step = r_u32(object + 0xB4);
    pitch = r_u32(object + 0x20); pitch_step = r_u32(object + 0xB8);
    w_u32(object + 0x28, yaw + yaw_step);
    w_u32(object + 0x20, pitch + pitch_step);
    game_update_object_movement(object);
}
uint32 game_integrate_linked_part_velocity(uint32 object)
{
    uint32 drive, value, next_drive, velocity_x, velocity_y, velocity_z, x, y, z, dy, dz;
    FUNCTION_MARKER(0x800DF638, "GAME.OVL");
    drive = r_u32(object + 0xFC) * 12;
    value = r_u32(object + 0x14); next_drive = r_u32(object + 0x100) * 12;
    w_u32(object + 0x14, value + drive);
    value = r_u32(object + 0x18); drive = r_u32(object + 0x104) * 12;
    w_u32(object + 0x18, value + next_drive);
    value = r_u32(object + 0x1C); velocity_x = r_u32(object + 0x14);
    w_u32(object + 0x1C, value + drive);
    x = r_u32(object + 8); velocity_y = r_u32(object + 0x18); velocity_z = r_u32(object + 0x1C);
    dy = (uint32)((sint32)velocity_y >> 8); dz = (uint32)((sint32)velocity_z >> 8);
    w_u32(object + 8, x + (uint32)((sint32)velocity_x >> 8));
    y = r_u32(object + 12); velocity_x = r_u32(object + 0x14);
    w_u32(object + 12, y + dy);
    z = r_u32(object + 16); velocity_y = r_u32(object + 0x18);
    w_u32(object + 16, z + dz);
    w_u32(object + 0x14, velocity_x - (uint32)((sint32)velocity_x >> 7));
    velocity_z = r_u32(object + 0x1C);
    w_u32(object + 0x18, velocity_y - (uint32)((sint32)velocity_y >> 7));
    w_u32(object + 0x1C, velocity_z - (uint32)((sint32)velocity_z >> 7));
    return 0;
}
void game_update_linked_part_parent_velocity(uint32 object)
{
    uint32 parent, value, callback;
    FUNCTION_MARKER(0x800DF704, "GAME.OVL");
    parent = r_u32(object + 0x160);
    value = r_u32(parent + 0xEC);
    w_u32(object + 0xEC, value);
    value = r_u32(parent);
    if (value != 0 && parent != 0) {
        game_integrate_linked_part_velocity(object);
        return;
    }
    callback = r_u32(object + 0x5C);
    w_u32(object + 0x9C, 0);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}
void game_begin_linked_part_breakup(uint32 object)
{
    uint32 flags, health, link, next, parent, value, sign, base, next_base, product, coefficient, x, y, z, vx, vy, vz;
    FUNCTION_MARKER(0x800DF768, "GAME.OVL");
    w_u32(object + 0x40, 0x800DF5D0);
    flags = r_u32(object + 0x114); health = r_u32(object + 0x238);
    w_u32(object + 0x114, flags ^ 0x80);
    if ((sint32)health > 0) w_u32(object + 0x9C, (uint32)random_between(150, 600));
    else w_u32(object + 0x9C, 1);
    game_release_linked_part_effects(object);
    if (r_u32(object + 0x168) != 0) {
        link = object;
        next = r_u32(link + 0x168);
        do {
            w_u32(next + 0x160, 0);
            link = r_u32(link + 0x168);
            if (link != 0) game_release_linked_part_effects(link);
            next = r_u32(link + 0x168);
        } while (next != 0);
    }
    parent = r_u32(object + 0x170);
    value = r_u32(parent + 0xEC);
    w_u32(object + 0xEC, value != 0 ? (uint32)((sint32)value >> 8) : 20);
    value = (uint32)random_between(0, 1);
    sign = value != 0 ? 1 : 0xFFFFFFFF;
    w_u32(object + 0xB4, value != 0 ? (uint32)-3 : 3);
    base = sign * r_u32(object + 0xEC);
    coefficient = (uint32)(sint32)r_s16(parent + 0x11E); product = base * coefficient;
    next_base = sign * r_u32(object + 0xEC);
    w_u32(object + 0xA8, (uint32)((sint32)product >> 12));
    coefficient = (uint32)(sint32)r_s16(parent + 0x124); product = next_base * coefficient;
    next_base = sign * r_u32(object + 0xEC);
    w_u32(object + 0xAC, (uint32)((sint32)product >> 12));
    coefficient = (uint32)(sint32)r_s16(parent + 0x12A); product = next_base * coefficient;
    w_u32(object + 0xB0, (uint32)((sint32)product >> 12));
    value = (uint32)random_between(0, 1);
    sign = value != 0 ? 0xFFFFFFFF : 1;
    w_u32(object + 0xB8, value != 0 ? 3 : (uint32)-3);
    base = sign * r_u32(object + 0xEC);
    coefficient = (uint32)(sint32)r_s16(parent + 0x11C); product = base * coefficient;
    next_base = sign * r_u32(object + 0xEC);
    value = r_u32(object + 0xA8) + (uint32)((sint32)product >> 12);
    w_u32(object + 0xA8, (uint32)((sint32)value >> 1));
    coefficient = (uint32)(sint32)r_s16(parent + 0x122); product = next_base * coefficient;
    next_base = sign * r_u32(object + 0xEC);
    value = r_u32(object + 0xAC) + (uint32)((sint32)product >> 12);
    w_u32(object + 0xAC, (uint32)((sint32)value >> 1));
    coefficient = (uint32)(sint32)r_s16(parent + 0x128); product = next_base * coefficient;
    value = r_u32(object + 0xB0) + (uint32)((sint32)product >> 12);
    w_u32(object + 0xB0, (uint32)((sint32)value >> 1));
    coefficient = (uint32)(sint32)r_s16(parent + 0x120); value = r_u32(object + 0xEC);
    product = value * coefficient;
    w_u32(object + 0xFC, (uint32)((sint32)product >> 12));
    coefficient = (uint32)(sint32)r_s16(parent + 0x126); value = r_u32(object + 0xEC);
    product = value * coefficient;
    w_u32(object + 0x100, (uint32)((sint32)product >> 12));
    coefficient = (uint32)(sint32)r_s16(parent + 0x12C); value = r_u32(object + 0xEC);
    y = r_u32(object + 0xAC); product = value * coefficient;
    vx = r_u32(object + 0xFC); x = r_u32(object + 0xA8);
    w_u32(object + 0x20, 0); w_u32(object + 0x24, 0); w_u32(object + 0x28, 0);
    vy = r_u32(object + 0x100);
    w_u32(object + 0xFC, (uint32)((sint32)(vx + x) >> 1));
    z = r_u32(object + 0xB0);
    w_u32(object + 0x104, (uint32)((sint32)product >> 12));
    vz = r_u32(object + 0x104);
    w_u32(object + 0x100, (uint32)((sint32)(vy + y) >> 1));
    w_u32(object + 0x104, (uint32)((sint32)(vz + z) >> 1));
}
void game_remove_expired_linked_part(uint32 object)
{
    FUNCTION_MARKER(0x800DFA38, "GAME.OVL");
    heap_free(r_u32(object + 0x204));
    game_emit_debris_expiration_effects(object);
    game_mark_object_for_removal(object);
}
sint32 game_set_voice_and_overlay_volume(uint32 voice, uint32 volume)
{
    uint32 overlay_voice, overlay_volume;
    FUNCTION_MARKER(0x800E0058, "GAME.OVL");
    sound_set_voice_volume(voice, volume);
    overlay_voice = r_u32(0x800FB438);
    overlay_volume = r_u8(0x80109B40);
    return sound_set_voice_volume(overlay_voice, overlay_volume);
}
uint32 game_initialize_large_effect_object(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800E21DC, "GAME.OVL");
    w_u32(object + 0x3C, 0x800D1C3C);
    w_u32(object + 0x5C, 0x800D332C);
    w_u32(object + 0x48, 0x800D33AC);
    w_u32(object + 0x84, 2);
    w_u32(object + 0x6C, 0x800EF5B4);
    flags = r_u32(object + 0x114) | 0x00200000;
    w_u32(object + 0x114, flags);
    return flags;
}
void game_initialize_object_variant_e1e44(uint32 object)
{
    uint32 kind, color, run;
    static const uint32 runs[10][2] = {{21, 7}, {18, 13}, {7, 5}, {11, 6}, {13, 5}, {17, 9}, {20, 17}, {27, 25}, {34, 21}, {4, 10}};
    FUNCTION_MARKER(0x800E1E44, "GAME.OVL");
    game_initialize_object_state_204(object);
    kind = r_u32(object);
    game_configure_object_subsystems(object, 0x800EFC60);
    color = kind == 3 ? 0x20C0FF : 0xFF8080;
    w_u32(object + 0x60, 0x800EC484);
    game_initialize_one_attached_effect(object, 38, 41, 2750, 12500, 0xFFFFFF, color, 750);
    w_u32(object + 0x6C, 0x800F15A0);
    w_u32(object + 0x114, r_u32(object + 0x114) | 0x08000000);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}
void game_initialize_object_variant_e1fdc(uint32 object)
{
    uint32 configuration[15], word, run, seed, first_mix, second_mix;
    static const uint32 runs[10][2] = {{3, 8}, {21, 14}, {13, 17}, {18, 22}, {5, 0}, {3, 8}, {21, 14}, {13, 17}, {18, 22}, {5, 0}};
    FUNCTION_MARKER(0x800E1FDC, "GAME.OVL");
    for (word = 0; word < 15; ++word) configuration[word] = r_u32(0x80076A44 + word * 4);
    game_initialize_object_state_204(object);
    w_u32(object + 0x6C, 0x800F15B8);
    game_configure_object_subsystems(object, 0x800EFD74);
    w_u32(object + 0x60, 0x800EC484);
    game_initialize_carrier_bay_geometry_native(object, configuration);
    w_u32(object + 0x3C, 0x8002D2BC);
    w_u32(object + 0x5C, 0x800D332C);
    w_u32(object + 0x48, 0x800D33AC);
    seed = r_u32(0x800EF50C);
    w_u16(object + 0x262, 170);
    first_mix = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    w_u32(object + 0x114, r_u32(object + 0x114) | 0x00200000);
    second_mix = (uint32)((sint32)first_mix >> 1) ^ (first_mix << 3);
    w_u32(0x800EF50C, second_mix);
    game_attach_object_light(object, 1, 0xFF, (first_mix & 15) | 32, second_mix & 31);
    for (run = 0; run < 10; ++run)
        game_add_attack_run(object, runs[run][0], runs[run][1], run + 1);
}
void game_initialize_asteroid_fragment(uint32 object)
{
    uint32 flags, kind, speed, value;
    FUNCTION_MARKER(0x800E2228, "GAME.OVL");
    w_u32(object + 0x40, 0x800E2590); w_u32(object + 0x3C, 0x8002D2BC);
    w_u32(object + 0x5C, 0x800E2314); w_u32(object + 0x48, 0x800D3478);
    w_u32(object + 0x6C, 0x800F15C8); w_u32(object, 106);
    flags = r_u32(object + 0x114); kind = r_u32(object + 4);
    w_u32(object + 0x114, flags | 0x00200000);
    speed = r_u32(0x8005369C + kind * 60);
    w_u16(object + 0x250, 0); w_u16(object + 0x252, 0); w_u16(object + 0x254, 0);
    w_u32(object + 0xE8, speed); w_u32(object + 0xEC, speed);
    game_update_object_state(object);
    value = (uint32)random_signed_between(3, 16); w_u32(object + 0xA8, value);
    value = (uint32)random_signed_between(3, 16); w_u32(object + 0xAC, value);
    value = (uint32)random_signed_between(3, 16); w_u32(object + 0xB0, value);
    game_load_asteroid_bundles();
    w_u32(object + 0x68, 0);
}
void game_break_asteroid_fragment(uint32 object)
{
    uint32 models[2], first_word, second_word, index, effect, payload, next_payload, value, x, y, z, callback, coefficient, product;
    SVECTOR angles;
    MATRIX first_rotation, second_rotation;
    FUNCTION_MARKER(0x800E2314, "GAME.OVL");
    models[0] = r_u32(0x800EDFCC); models[1] = r_u32(0x800EDFD0);
    first_word = r_u32(0x80076A80); second_word = r_u32(0x80076A84);
    angles.vx = (sint16)first_word; angles.vy = (sint16)(first_word >> 16);
    angles.vz = (sint16)second_word; angles.pad = (sint16)(second_word >> 16);
    game_set_screen_tint(255, 255, 255);
    x = r_u32(object + 8); y = r_u32(object + 12); z = r_u32(object + 16);
    game_emit_hit_flash(x, y, z);
    for (index = 0; index < 13; ++index) {
        value = (uint32)random_between(0, 1);
        effect = game_create_model_effect(object + 8, (sint16)models[value]);
        payload = r_u32(effect + 0x6C);
        w_u16(effect + 0x3C, 100);
        w_u32(payload, 0x800EEEF0);
        value = (uint32)random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C); w_u16(payload + 0x34, value);
        value = (uint32)random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C); w_u16(payload + 0x36, value);
        value = (uint32)random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C); w_u16(payload + 0x38, value);
        angles.vx = (sint16)random_signed_between(0, 4095);
        angles.vy = (sint16)random_signed_between(0, 4095);
        angles.vz = (sint16)random_signed_between(0, 4095);
        payload = r_u32(effect + 0x6C);
        matrix_rotate_y(&first_rotation, (MATRIX *)psx_addr(payload + 4, 20), 0 - (uint32)(sint32)angles.vx);
        matrix_rotate_x(&second_rotation, &first_rotation, (uint32)(sint32)angles.vy);
        payload = r_u32(effect + 0x6C);
        matrix_rotate_z((MATRIX *)psx_addr(payload + 4, 20), &second_rotation, 0 - (uint32)(sint32)angles.vz);
        payload = r_u32(effect + 0x6C);
        coefficient = (uint32)(sint32)r_s16(payload + 8); product = coefficient * 75;
        next_payload = r_u32(effect + 0x6C);
        w_u32(effect + 0x20, (uint32)((sint32)product >> 9));
        coefficient = (uint32)(sint32)r_s16(next_payload + 0xE); product = coefficient * 75;
        next_payload = r_u32(effect + 0x6C);
        w_u32(effect + 0x24, (uint32)((sint32)product >> 9));
        coefficient = (uint32)(sint32)r_s16(next_payload + 0x14); product = coefficient * 75;
        w_u32(effect + 0x28, (uint32)((sint32)product >> 9));
        value = (uint32)random_between(-2, 1);
        w_u16(effect + 0x34, value);
        w_u32(effect + 0x60, 0x800B2520);
        w_u32(effect + 0x30, 0x07FFFFFF);
        w_u32(effect + 0x2C, 360);
        w_u32(0x800595CC, r_u32(0x800595CC) + 1);
    }
    callback = r_u32(object + 0x48);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}
uint32 game_update_asteroid_fragment_motion(uint32 object)
{
    uint32 pitch, yaw, roll, x, y, z, vx, vy, vz;
    FUNCTION_MARKER(0x800E2590, "GAME.OVL");
    pitch = r_u32(object + 0xA8); yaw = r_u32(object + 0xAC);
    x = r_u32(object + 8); vx = r_u32(object + 0xFC); roll = r_u32(object + 0xB0);
    w_u32(object + 8, x + vx);
    y = r_u32(object + 12);
    w_u32(object + 0x20, pitch);
    vy = r_u32(object + 0x100); z = r_u32(object + 16);
    w_u32(object + 0x24, yaw);
    vz = r_u32(object + 0x104);
    w_u32(object + 0x28, roll);
    y += vy; z += vz;
    w_u32(object + 12, y); w_u32(object + 16, z);
    return y;
}
void game_configure_object_subsystems(uint32 object, uint32 configuration)
{
    uint32 block, bit, mask, record, primary, secondary, grid, scalar, primary_config, secondary_config, grid_config, scalar_config;
    uint32 index, cursor, destination, value, next, frame, mode, model, table, descriptor, random, stagger;
    FUNCTION_MARKER(0x800EB168, "GAME.OVL");
    block = heap_alloc_back(0x484);
    w_u32(object + 0x110, block);
    for (index = 0; index < 6; ++index) w_u8(block + index * 48, 0);
    for (index = 0; index < 9; ++index) w_u8(block + 0x120 + index * 44, 0);
    w_u8(block + 0x46F, 0); w_u32(block + 0x474, 0);
    for (bit = 0; bit < 32; ++bit) {
        mask = r_u32(configuration + 4) & (1u << bit);
        if (mask == 0) continue;
        primary = secondary = grid = scalar = 0;
        primary_config = secondary_config = grid_config = scalar_config = 0;
        if (bit < 6) {
            primary_config = r_u32(configuration + 8 + bit * 4);
            primary = block + bit * 48;
        } else if (bit < 15) {
            secondary_config = r_u32(configuration + 8 + bit * 4);
            secondary = block + 0x120 + (bit - 6) * 44;
        } else if (bit == 17) {
            model = r_u32(object + 0x74);
            table = r_u32(0x80046178);
            descriptor = r_u32(table + model * 132 + 8);
            grid_config = r_u32(configuration + 0x44);
            grid = block + 0x2AC;
            if (descriptor == 0) {
                diagnostic_report(0, 0x80076BD4, 0x80076BD0);
                return;
            }
        } else if (bit == 18) {
            scalar = block + 0x474;
            scalar_config = r_u32(configuration + 0x48);
        }
        if (primary_config == 0 && secondary_config == 0 && grid_config == 0 && scalar_config == 0)
            diagnostic_report(r_u32(object + 4), 0x80076BF0, 0);
        if (primary != 0) {
            w_u8(primary, 1);
            value = r_u32(primary_config); w_u32(primary + 4, value);
            value = r_u32(primary_config);
            if ((sint32)value > 0) {
                index = 0; cursor = primary_config; destination = primary;
                do {
                    value = r_u32(cursor + 0x28); cursor += 4; ++index;
                    w_u32(destination + 8, value);
                    value = r_u32(primary_config);
                    destination += 4;
                } while ((sint32)index < (sint32)value);
            }
            mode = (uint32)(sint32)r_s16(object + 0x262);
            if ((sint32)mode >= 3) mode = 2;
            cursor = primary_config + mode * 12;
            value = r_u32(cursor + 4); w_u32(primary + 0x18, value);
            value = r_u32(cursor + 8); w_u32(primary + 0x1C, value);
            frame = r_u32(0x800311B8); value = r_u32(cursor + 12);
            w_u32(primary + 0x28, 0); w_u32(primary + 0x2C, 1);
            w_u32(primary + 0x24, frame - 10000); w_u32(primary + 0x20, value);
        } else if (secondary != 0) {
            w_u8(secondary, 1);
            value = r_u32(secondary_config + 0x14); w_u32(secondary + 4, value);
            value = r_u32(secondary_config); w_u32(secondary + 8, value);
            value = r_u32(secondary_config + 4); w_u32(secondary + 0xC, value);
            value = r_u32(secondary_config + 8); w_u32(secondary + 0x10, value);
            value = r_u32(secondary_config + 0xC); w_u32(secondary + 0x14, value);
            value = r_u16(secondary_config + 0x10); w_u16(secondary + 0x18, value);
            next = r_u16(secondary_config + 0x12); frame = r_u32(0x800311B8);
            w_u32(secondary + 0x1C, 0); w_u32(secondary + 0x20, 0); w_u32(secondary + 0x28, 0);
            w_u32(secondary + 0x24, frame - 10000); w_u16(secondary + 0x1A, next);
        } else if (grid != 0) {
            w_u8(grid + 0x1C3, 1);
            value = r_u32(grid_config + 8); frame = r_u32(0x800311B8);
            w_u8(grid + 0x1C0, 0);
            w_u32(grid + 0x1B8, value); w_u32(grid + 0x1BC, frame - 60);
            value = r_u8(grid_config + 0xC); w_u8(grid + 0x1C2, 0); w_u8(grid + 0x1C1, value);
            value = r_u8(grid_config + 0xD); w_u8(grid + 0x1C4, value);
            for (index = 0; index < 10; ++index) {
                record = grid + index * 12;
                w_u32(record + 0x140, 0); w_u16(record + 0x144, 0);
                w_u8(record + 0x148, 0); w_u8(record + 0x149, 1);
            }
            value = r_u8(grid + 0x1C4);
            if ((sint32)value > 0) {
                index = 0; stagger = 0; record = grid;
                do {
                    w_u8(record, 1);
                    random = (uint32)random_between(1, 4);
                    value = (r_u16(grid_config) << 1) + random; w_u16(record + 2, value);
                    random = (uint32)random_between(2, 6);
                    value = (r_u16(grid_config + 2) << 1) + random; w_u16(record + 4, value);
                    value = r_u16(grid_config + 4) << 1; w_u16(record + 6, value);
                    value = r_u8(grid_config + 6); w_u8(record + 8, value);
                    frame = r_u32(0x800311B8);
                    w_u32(record + 0xC, 0); w_u16(record + 0x10, 0); w_u32(record + 0x18, 0);
                    w_u8(record + 0x1C, 0); w_u8(record + 0x1D, 0);
                    w_u32(record + 0x14, frame + stagger); stagger += 60;
                    ++index; value = r_u8(grid + 0x1C4); record += 32;
                } while ((sint32)index < (sint32)value);
            }
        } else if (scalar != 0) {
            w_u32(scalar, 1);
            value = r_u32(scalar_config); frame = r_u32(0x800311B8);
            w_u32(scalar + 0xC, 0); w_u32(scalar + 4, value); w_u32(scalar + 8, frame);
        } else diagnostic_report(r_u32(object + 4), 0x80076C10, 0x80076BD0);
    }
}
uint32 game_subtract_camera_drift(uint32 value, uint32 amount)
{
    uint32 difference;
    FUNCTION_MARKER(0x800E62AC, "GAME.OVL");
    difference = value - amount;
    return difference & (0u - (uint32)((difference ^ value) != 0));
}
void game_update_transition_camera(uint32 object)
{
    uint32 counter, parent, special, player, camera, axis, coefficient, parent_position, backward, sideways, value, next, x, y, z, px, py, pz, scale, frame;
    sint32 distance;
    FUNCTION_MARKER(0x800E62C8, "GAME.OVL");
    counter = r_u32(object + 0x9C); parent = r_u32(object + 0x170);
    special = r_u32(0x8003FE74); player = r_u32(0x80040FDC); camera = r_u32(0x80040C94);
    w_u32(object + 0x9C, counter + 1);
    w_u32(special + 0x84, 0);
    for (axis = 0; axis < 2; ++axis) {
        coefficient = (uint32)(sint32)r_s16(parent + 0x120 + axis * 6);
        parent_position = r_u32(parent + 8 + axis * 4);
        backward = (uint32)((sint32)(0 - ((coefficient * 13) << 12)) >> 12);
        coefficient = (uint32)(sint32)r_s16(player + 0x11C + axis * 2);
        sideways = (uint32)((sint32)(coefficient * 45000) >> 11);
        w_u32(camera + 8 + axis * 4, parent_position + backward + sideways);
    }
    coefficient = (uint32)(sint32)r_s16(parent + 0x12C);
    parent_position = r_u32(parent + 16);
    next = (uint32)(sint32)r_s16(player + 0x120);
    w_u32(camera + 0xFC, 0); w_u32(camera + 0x14, 0);
    w_u32(camera + 0x100, 0); w_u32(camera + 0x18, 0);
    w_u32(camera + 0x104, 0); w_u32(camera + 0x1C, 0);
    backward = (uint32)((sint32)(0 - ((coefficient * 13) << 12)) >> 12);
    sideways = (uint32)((sint32)(next * 45000) >> 11);
    w_u32(camera + 16, parent_position + backward + sideways);
    px = r_u32(parent + 8); x = r_u32(player + 8);
    py = r_u32(parent + 12); y = r_u32(player + 12);
    z = r_u32(player + 16); pz = r_u32(parent + 16);
    distance = vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
    if (distance <= 59999) {
        if (r_u32(0x800F189C) != 0) goto integrate_drift;
        player = r_u32(0x80040FDC);
        scale = (uint32)((sint32)r_u32(0x800F2B24) >> 6);
        value = r_u32(player + 0x214) * scale;
        w_u32(0x800F4EA8, (uint32)((sint32)value >> 12));
        value = r_u32(player + 0x218) * scale;
        w_u32(0x800F4EAC, (uint32)((sint32)value >> 12));
        value = r_u32(player + 0x21C) * scale;
        camera = r_u32(0x80040C94);
        w_u32(0x800F4EB0, (uint32)((sint32)value >> 12));
        value = r_u32(player + 8); next = r_u32(camera + 8); w_u32(0x800F4F00, value - next);
        value = r_u32(player + 12); next = r_u32(camera + 12); w_u32(0x800F4F04, value - next);
        value = r_u32(player + 16); next = r_u32(camera + 16);
        w_u32(0x800F189C, 1); w_u32(0x800F4F08, value - next);
    }
    if (r_u32(0x800F189C) != 0) goto integrate_drift;
    player = r_u32(0x80040FDC); camera = r_u32(0x80040C94);
    value = r_u32(player + 8); next = r_u32(camera + 8); w_u32(0x800F4F18, value - next);
    value = r_u32(player + 12); next = r_u32(camera + 12); w_u32(0x800F4F1C, value - next);
    value = r_u32(player + 16); next = r_u32(camera + 16); w_u32(0x800F4F20, value - next);
    goto bound_direction;
integrate_drift:
    frame = r_u32(0x800F4EA8);
    x = r_u32(0x800F4F00); y = r_u32(0x800F4F04); z = r_u32(0x800F4F08);
    w_u32(0x800F4F18, x); w_u32(0x800F4F1C, y); w_u32(0x800F4F20, z);
    w_u32(0x800F4F00, x + frame);
    value = r_u32(0x800F4EAC); next = r_u32(0x800F4EB0);
    w_u32(0x800F4F04, y + value); w_u32(0x800F4F08, z + next);
    value = game_subtract_camera_drift(frame, (uint32)((sint32)frame >> 4));
    next = r_u32(0x800F4EAC); w_u32(0x800F4EA8, value);
    value = game_subtract_camera_drift(next, (uint32)((sint32)next >> 4));
    next = r_u32(0x800F4EB0); w_u32(0x800F4EAC, value);
    value = game_subtract_camera_drift(next, (uint32)((sint32)next >> 4));
    w_u32(0x800F4EB0, value);
bound_direction:
    value = r_u32(0x800F4F18);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value >= 16385) goto halve_direction;
    value = r_u32(0x800F4F1C);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value >= 16385) goto halve_direction;
    value = r_u32(0x800F4F20);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value < 16385) goto build_axes;
halve_direction:
    for (;;) {
        x = (uint32)((sint32)r_u32(0x800F4F18) >> 1); w_u32(0x800F4F18, x);
        value = (sint32)x < 0 ? 0 - x : x;
        y = r_u32(0x800F4F1C); z = r_u32(0x800F4F20);
        y = (uint32)((sint32)y >> 1); z = (uint32)((sint32)z >> 1);
        w_u32(0x800F4F1C, y); w_u32(0x800F4F20, z);
        if ((sint32)value >= 16385) continue;
        value = (sint32)y < 0 ? 0 - y : y;
        if ((sint32)value >= 16385) continue;
        value = (sint32)z < 0 ? 0 - z : z;
        if ((sint32)value < 16385) break;
    }
build_axes:
    player = r_u32(0x80040FDC);
    value = (uint32)(sint32)r_s16(player + 0x122); w_u32(0x80033D40, 0 - value);
    value = (uint32)(sint32)r_s16(player + 0x124); w_u32(0x80033D44, 0 - value);
    value = (uint32)(sint32)r_s16(player + 0x126); w_u32(0x80033D48, 0 - value);
    camera_build_axes((VECTOR *)psx_addr(0x800F4F18, 12), (VECTOR *)psx_addr(0x80033D40, 12));
    camera_apply_vertical_scale();
}
void game_begin_transition_warp_effect(uint32 object, uint32 mode)
{
    VECTOR position;
    MATRIX matrix;
    uint32 player, coefficient, value, offset, effect, flags;
    sint32 slot;
    FUNCTION_MARKER(0x800E728C, "GAME.OVL");
    if (mode == 2) {
        player = r_u32(0x80040FDC);
        coefficient = (uint32)(sint32)r_s16(player + 0x128); value = r_u32(player + 8);
        position.vx = (sint32)(value + (uint32)((sint32)(0 - coefficient * 25000) >> 9));
        coefficient = (uint32)(sint32)r_s16(player + 0x12A); value = r_u32(player + 12);
        position.vy = (sint32)(value + (uint32)((sint32)(0 - coefficient * 25000) >> 9));
        coefficient = (uint32)(sint32)r_s16(player + 0x12C); value = r_u32(player + 16);
        position.vz = (sint32)(value + (uint32)((sint32)(0 - coefficient * 25000) >> 9));
        matrix_copy_rotation_to_native(&matrix, player + 0x11C);
    } else {
        matrix_copy_rotation_to_native(&matrix, object + 0x11C);
        position.vx = r_s32(object + 8); position.vy = r_s32(object + 12); position.vz = r_s32(object + 16);
    }
    player = r_u32(0x80040FDC);
    slot = game_allocate_arrival_warp_effect_native(&position, &matrix, player);
    if (slot < 0) return;
    offset = (uint32)slot * 0x5C48;
    player = r_u32(0x80040FDC);
    w_u32(0x8010ACDC + offset, 1); w_u32(0x8010ACE0 + offset, 2);
    w_u32(player + 0xA8, 1);
    effect = game_create_object(0, (uint32)position.vx, (uint32)position.vy, (uint32)position.vz, 0);
    flags = r_u32(effect + 0x70); w_u32(effect + 0x70, flags | 0x00100000);
    matrix_copy_rotation_native(effect + 0x11C, &matrix);
    w_u32(effect + 0x40, 0x800E6A24);
    w_u32(effect + 0xA0, 0); w_u32(effect + 0x88, 0);
    w_u32(effect + 0xB0, (uint32)slot); w_u32(effect + 0xA8, 1);
}
void game_finish_transition_with_warp(uint32 object)
{
    uint32 mode, callback;
    FUNCTION_MARKER(0x800E6264, "GAME.OVL");
    mode = r_u32(0x800F18B4);
    if ((sint32)mode <= 0) return;
    game_begin_transition_warp_effect(object, mode);
    callback = r_u32(object + 0x48);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}
void game_create_transition_camera(uint32 parent)
{
    uint32 player, x, y, z, camera;
    FUNCTION_MARKER(0x800E67B8, "GAME.OVL");
    player = r_u32(0x80040FDC);
    x = r_u32(player + 8); y = r_u32(player + 12); z = r_u32(player + 16);
    camera = game_create_object(0, x, y, z, 0);
    player = r_u32(0x80040FDC);
    w_u32(camera + 0x170, parent);
    w_u32(player + 0xA8, 0);
    matrix_copy_rotation(camera + 0x11C, player + 0x11C);
    w_u32(camera + 0x40, 0x800E62C8);
    w_u32(camera + 0x9C, 0);
    w_u32(0x800F189C, 0);
}
void game_update_transition_approach(uint32 object)
{
    VECTOR difference;
    uint32 parent, value, position, x, y, z, coefficient, frame, product, axis;
    sint32 distance;
    FUNCTION_MARKER(0x800E683C, "GAME.OVL");
    parent = r_u32(object + 0x170); value = r_u32(parent + 8); position = r_u32(object + 8);
    difference.vx = (sint32)(value - position);
    parent = r_u32(object + 0x170); value = r_u32(parent + 12); position = r_u32(object + 12);
    difference.vy = (sint32)(value - position);
    parent = r_u32(object + 0x170); value = r_u32(parent + 16); position = r_u32(object + 16);
    difference.vz = (sint32)(value - position);
    distance = vector_length_approx(difference.vx, difference.vy, difference.vz);
    w_u32(0x80046174, (uint32)distance);
    if (distance < 10000) w_u32(object + 0xB4, 0);
    if (r_u32(object + 0xB4) == 1) {
        for (;;) {
            x = (uint32)difference.vx; y = (uint32)difference.vy; z = (uint32)difference.vz;
            if ((sint32)x < 0) x = 0 - x;
            if ((sint32)y < 0) y = 0 - y;
            if ((sint32)z < 0) z = 0 - z;
            if ((sint32)x < 32001 && (sint32)y < 32001 && (sint32)z < 32001) break;
            difference.vx >>= 1; difference.vy >>= 1; difference.vz >>= 1;
        }
        vector_normalize(&difference, (VECTOR *)psx_addr(object + 0x214, 12));
    }
    for (axis = 0; axis < 3; ++axis) {
        frame = r_u32(0x800F2B24);
        coefficient = r_u32(object + 0x214 + axis * 4);
        product = coefficient * (uint32)((sint32)frame >> 4);
        position = r_u32(object + 8 + axis * 4);
        w_u32(object + 8 + axis * 4, position + (uint32)((sint32)product >> 12));
    }
}
void game_update_transition_warp_controller(uint32 object)
{
    VECTOR difference, local;
    uint32 player, value, other, mode, slot, distance, x, y, z, coefficient, coefficient_y, coefficient_z, yaw, pitch, camera;
    sint32 length;
    FUNCTION_MARKER(0x800E6A2C, "GAME.OVL");
    player = r_u32(0x80040FDC);
    value = r_u32(object + 8); other = r_u32(player + 8); difference.vx = (sint32)(other - value);
    other = r_u32(player + 12); value = r_u32(object + 12); difference.vy = (sint32)(other - value);
    other = r_u32(player + 16); value = r_u32(object + 16); difference.vz = (sint32)(other - value);
    length = vector_length_approx(difference.vx, difference.vy, difference.vz);
    mode = r_u32(object + 0x88);
    w_u32(0x80046174, (uint32)length);
    switch (mode) {
    case 0:
        slot = r_u32(object + 0xB0);
        if (r_u32(0x8010ACD0 + slot * 0x5C48) == 9) w_u32(object + 0x88, 1);
        break;
    case 1:
        slot = r_u32(object + 0xB0); distance = r_u32(0x80046174);
        w_u32(0x8010ACD4 + slot * 0x5C48, 255);
        slot = r_u32(object + 0xB0);
        w_u32(0x8010ACD0 + slot * 0x5C48, 9);
        if ((sint32)distance > 129999) break;
        player = r_u32(0x80040FDC);
        coefficient = (uint32)(sint32)r_s16(player + 0x128);
        coefficient_y = (uint32)(sint32)r_s16(player + 0x12A);
        w_u32(player + 0x170, object);
        x = r_u32(player + 8) - (uint32)((sint32)(coefficient * 625) >> 6);
        w_u32(player + 8, x);
        y = r_u32(player + 12);
        coefficient_z = (uint32)(sint32)r_s16(player + 0x12C);
        y -= (uint32)((sint32)(coefficient_y * 625) >> 6);
        w_u32(player + 12, y);
        z = r_u32(player + 16);
        x = r_u32(player + 8); y = r_u32(player + 12);
        z -= (uint32)((sint32)(coefficient_z * 625) >> 6);
        w_u32(player + 16, z);
        value = r_u32(object + 8); other = r_u32(object + 16);
        coefficient = r_u32(object + 12);
        game_set_relative_target_angles(player, value - x, coefficient - y, other - z);
        yaw = r_u32(0x80033D1C); player = r_u32(0x80040FDC); pitch = r_u32(0x80033D30);
        game_apply_control_rotation(player + 0x11C, yaw, 0 - pitch, 0);
        player = r_u32(0x80040FDC); matrix_normalize(player + 0x11C, 1);
        player = r_u32(0x80040FDC); matrix_copy_rotation(0x80035060, player + 0x11C);
        player = r_u32(0x80040FDC);
        coefficient = (uint32)(sint32)r_s16(player + 0x128); value = r_u32(player + 8);
        x = value + (uint32)((sint32)(coefficient * 35000) >> 10);
        slot = r_u32(object + 0xB0);
        w_u32(object + 8, x); w_u32(0x8010AD08 + slot * 0x5C48, x);
        coefficient = (uint32)(sint32)r_s16(player + 0x12A); value = r_u32(player + 12);
        y = value + (uint32)((sint32)(coefficient * 35000) >> 10);
        slot = r_u32(object + 0xB0);
        w_u32(object + 12, y); w_u32(0x8010AD0C + slot * 0x5C48, y);
        coefficient = (uint32)(sint32)r_s16(player + 0x12C); value = r_u32(player + 16);
        z = value + (uint32)((sint32)(coefficient * 35000) >> 10);
        slot = r_u32(object + 0xB0);
        w_u32(object + 16, z); w_u32(0x8010AD10 + slot * 0x5C48, z);
        x = (uint32)(sint32)r_s16(0x80035060) << 3;
        y = (uint32)(sint32)r_s16(0x80035062) << 3;
        z = (uint32)(sint32)r_s16(0x80035064) << 3;
        w_u16(0x80035060, (uint32)((sint32)x / 5));
        w_u16(0x80035062, (uint32)((sint32)y / 5));
        w_u16(0x80035064, (uint32)((sint32)z / 5));
        matrix_copy_rotation(0x80034D58, 0x80035060);
        camera_dispatch();
        w_u32(object + 0x88, 2);
        break;
    case 2:
        slot = r_u32(object + 0xB0); camera = r_u32(0x80042174);
        w_u32(0x800313DC, 4); w_u32(0x8010ACD4 + slot * 0x5C48, 255);
        slot = r_u32(object + 0xB0); player = r_u32(0x80040FDC);
        w_u32(0x800311DC, 1); w_u32(0x80040C94, camera); w_u32(0x8004EB1C, player);
        w_u32(0x8010ACD0 + slot * 0x5C48, 9); w_u16(0x8005961C, 0);
        game_refresh_mission_state();
        render_set_reduced_height(512);
        game_create_transition_camera(object);
        distance = r_u32(0x80046174); player = r_u32(0x80040FDC);
        w_u32(player + 0x40, 0x800E683C); w_u32(player, 0); w_u32(player + 0xB4, 1);
        w_u32(object + 0xA8, 0); w_u32(object + 0xAC, distance + 1000); w_u32(object + 0x88, 3);
        game_suspend_scene_engine_audio();
        break;
    case 3:
        slot = r_u32(object + 0xB0); w_u32(0x8010ACD4 + slot * 0x5C48, 255);
        slot = r_u32(object + 0xB0); w_u32(0x8010ACD0 + slot * 0x5C48, 9);
        distance = r_u32(0x80046174); value = r_u32(object + 0xAC);
        if ((sint32)value < (sint32)distance) {
            slot = r_u32(object + 0xB0); w_u32(0x8010ACD0 + slot * 0x5C48, 5);
            w_u32(object + 0x88, 4);
        } else w_u32(object + 0xAC, distance);
        break;
    case 4:
        slot = r_u32(object + 0xB0); w_u32(0x8010ACD0 + slot * 0x5C48, 6);
        if (r_s32(0x80046174) > 60000) {
            player = r_u32(0x80040FDC); w_u32(object + 0x88, 5);
            w_u32(player + 0x3C, 0x8002493C); w_u32(player + 0x40, 0x8002493C);
            slot = r_u32(object + 0xB0); w_u32(0x8010ACD4 + slot * 0x5C48, 0);
        }
        break;
    case 5:
        slot = r_u32(object + 0xB0);
        if (r_u32(0x8010ACD0 + slot * 0x5C48) == 0) w_u32(0x800F18B8, 1);
        break;
    default:
        break;
    }
    if (r_u32(object + 0xA8) == 0) return;
    difference.vx = (sint32)(0 - (uint32)difference.vx);
    difference.vz = (sint32)(0 - (uint32)difference.vz);
    difference.vy = (sint32)(0 - (uint32)difference.vy);
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11C, 18), &difference, &local);
    yaw = (uint32)ratan2(local.vx, local.vz); w_u32(object + 0x20, yaw);
    value = (uint32)local.vz;
    if ((sint32)value < 0) value = 0 - value;
    pitch = (uint32)ratan2((sint32)(0 - (uint32)local.vy), (sint32)value);
    w_u32(object + 0x24, pitch);
    game_rotate_object_basis(object);
    slot = r_u32(object + 0xB0);
    w_u32(object + 0x24, 0); w_u32(object + 0x20, 0);
    matrix_copy_rotation(0x8010ACE8 + slot * 0x5C48, object + 0x11C);
}
void game_attach_player_to_transition_warp(uint32 object)
{
    uint32 actor, value, slot, next_slot, callback, player;
    FUNCTION_MARKER(0x800E74A4, "GAME.OVL");
    if (r_u32(0x800F18C0) == 0) {
        actor = r_u32(0x800F18BC);
        if (r_u32(actor + 0x88) != 4) return;
        w_u32(actor + 0x3C, 0x8002493C);
        w_u32(r_u32(0x800F18BC) + 0x44, 0x8002493C);
        w_u32(r_u32(0x800F18BC) + 0xA0, 0);
        w_u32(r_u32(0x800F18BC) + 0x114, 0x2000);
        w_u32(r_u32(0x800F18BC) + 0x118, 0);
        actor = r_u32(0x800F18BC);
        w_u32(0x800F18C0, 1);
        value = r_u32(actor + 0xA8); w_u32(object + 0xB0, value);
        actor = r_u32(0x800F18BC); callback = r_u32(actor + 0x48);
        psx_dispatch_guest_call(callback, 1, actor, 0, 0);
        return;
    }
    slot = r_u32(object + 0xB0);
    if (r_u32(0x8010ACD0 + slot * 0x5C48) != 7) return;
    value = r_u32(0x8010AD08 + slot * 0x5C48); next_slot = r_u32(object + 0xB0);
    w_u32(object + 8, value);
    value = r_u32(0x8010AD0C + next_slot * 0x5C48); next_slot = r_u32(object + 0xB0);
    w_u32(object + 12, value);
    value = r_u32(0x8010AD10 + next_slot * 0x5C48); next_slot = r_u32(object + 0xB0);
    w_u32(object + 16, value);
    matrix_copy_rotation(object + 0x11C, 0x8010ACE8 + next_slot * 0x5C48);
    slot = r_u32(object + 0xB0); w_u32(0x8010ACD0 + slot * 0x5C48, 9);
    slot = r_u32(object + 0xB0); player = r_u32(0x80040FDC);
    w_u32(0x8010ACE4 + slot * 0x5C48, player);
    w_u32(object + 0x40, 0x800E6A24); w_u32(object + 0x88, 0); w_u32(object + 0xA8, 1);
}
void game_update_transition_departure(uint32 object)
{
    uint32 mode, fast, ticks, kind, next_kind, value, slot, offset, created, axis, coefficient, speed, position[3], products[3], x, y, z, wx, wy, wz, previous, callback;
    sint32 allocated, distance;
    FUNCTION_MARKER(0x800E7730, "GAME.OVL");
    mode = r_u32(object + 0x88); fast = 0;
    switch (mode) {
    case 0:
        ticks = r_u32(object + 0x9C) - 1; w_u32(object + 0x9C, ticks);
        if ((sint32)ticks > 0) {
            kind = r_u32(object + 4);
            w_u16(object + 0x250, 0); w_u16(object + 0x252, 0); w_u16(object + 0x254, 0);
            value = r_u32(0x8005369C + kind * 60); w_u32(object + 0xE8, value << 1);
        } else w_u32(object + 0x88, 1);
        fast = 1;
        break;
    case 1:
        for (axis = 0; axis < 3; ++axis) {
            coefficient = (uint32)(sint32)r_s16(object + 0x120 + axis * 6);
            value = r_u32(object + 8 + axis * 4);
            position[axis] = value + (uint32)((sint32)(coefficient * 25000) >> 9);
        }
        allocated = game_allocate_warp_effect_native(position, object + 0x11C, object);
        if (allocated >= 0) {
            created = game_create_object(0, position[0], position[1], position[2], 0);
            w_u32(created + 0xA0, 0);
            w_u32(object + 0xA8, (uint32)allocated);
            matrix_copy_rotation(0x8010ACE8 + (uint32)allocated * 0x5C48, object + 0x11C);
            for (axis = 0; axis < 3; ++axis) {
                slot = r_u32(object + 0xA8); offset = slot * 0x5C48;
                value = (uint32)(sint32)r_s16(0x8010ACEA + offset + axis * 6);
                w_u16(0x8010ACEA + offset + axis * 6, 0 - value);
            }
            for (axis = 0; axis < 3; ++axis) {
                slot = r_u32(object + 0xA8); offset = slot * 0x5C48;
                value = r_u16(0x8010ACEC + offset + axis * 6);
                w_u16(0x8010ACEC + offset + axis * 6, value);
            }
            slot = r_u32(object + 0xA8);
            w_u32(object + 0x88, 2); w_u32(0x8010ACE0 + slot * 0x5C48, 2);
            slot = r_u32(object + 0xA8); w_u32(0x8010ACDC + slot * 0x5C48, 1);
        }
        fast = 1;
        break;
    case 2:
        slot = r_u32(object + 0xA8); x = r_u32(object + 8); z = r_u32(object + 16);
        offset = slot * 0x5C48;
        value = r_u32(0x8010ACD0 + offset); wx = r_u32(0x8010AD08 + offset);
        fast = value != 9;
        wy = r_u32(0x8010AD0C + offset); y = r_u32(object + 12); wz = r_u32(0x8010AD10 + offset);
        distance = vector_length_approx((sint32)(wx - x), (sint32)(wy - y), (sint32)(wz - z));
        previous = r_u32(object + 0xAC); w_u32(0x80046174, (uint32)distance);
        if ((sint32)previous < distance) w_u32(object + 0x88, 3);
        else w_u32(object + 0xAC, (uint32)distance);
        break;
    case 3:
        slot = r_u32(object + 0xA8); w_u32(0x8010ACD0 + slot * 0x5C48, 6);
        slot = r_u32(object + 0xA8); w_u32(0x8010ACD4 + slot * 0x5C48, 255);
        slot = r_u32(object + 0xA8); x = r_u32(object + 8); y = r_u32(object + 12);
        offset = slot * 0x5C48;
        wx = r_u32(0x8010AD08 + offset); z = r_u32(object + 16);
        wy = r_u32(0x8010AD0C + offset); wz = r_u32(0x8010AD10 + offset);
        distance = vector_length_approx((sint32)(wx - x), (sint32)(wy - y), (sint32)(wz - z));
        w_u32(0x80046174, (uint32)distance);
        if (distance >= 20001) {
            slot = r_u32(object + 0xA8); w_u32(object + 0x88, 4);
            w_u32(object + 0x3C, 0x8002493C); w_u32(object + 0x40, 0x8002493C); w_u32(object + 0x44, 0x8002493C);
            w_u32(object + 0xA0, 0); w_u32(object + 0x114, 0x2000); w_u32(object + 0x118, 0);
            w_u32(0x8010ACD4 + slot * 0x5C48, 0);
        }
        break;
    case 4:
        slot = r_u32(object + 0xA8);
        if (r_u32(0x8010ACD0 + slot * 0x5C48) == 0) {
            callback = r_u32(object + 0x48);
            psx_dispatch_guest_call(callback, 1, object, 0, 0);
            return;
        }
        break;
    default:
        break;
    }
    if (fast == 0) {
        kind = r_u32(object + 4);
        for (axis = 0; axis < 3; ++axis) {
            speed = r_u32(0x800536A0 + kind * 60);
            coefficient = (uint32)(sint32)r_s16(object + 0x120 + axis * 6);
            value = coefficient * (uint32)((sint32)speed >> 8);
            x = r_u32(object + 8 + axis * 4);
            if (axis < 2) next_kind = r_u32(object + 4);
            w_u32(object + 8 + axis * 4, x + (uint32)((sint32)value >> 9));
            if (axis < 2) kind = next_kind;
        }
    } else {
        for (axis = 0; axis < 3; ++axis) {
            coefficient = (uint32)(sint32)r_s16(object + 0x120 + axis * 6); speed = r_u32(object + 0xEC);
            products[axis] = coefficient * speed;
        }
        for (axis = 0; axis < 3; ++axis) {
            x = r_u32(object + 8 + axis * 4);
            w_u32(object + 8 + axis * 4, x + (uint32)((sint32)products[axis] >> 12));
        }
    }
}
void game_reset_transition_object_limits(void)
{
    uint32 list, object;
    static const uint32 lists[2] = {3, 1};
    FUNCTION_MARKER(0x800E7FFC, "GAME.OVL");
    for (list = 0; list < 2; ++list) {
        object = object_lookup(lists[list]);
        while (object != 0) {
            w_u32(object + 0x234, 10000); w_u32(object + 0x238, 10000);
            object = r_u32(object + 0x1DC);
        }
    }
}
void game_reset_transition_list_limits(uint32 list)
{
    uint32 object;
    FUNCTION_MARKER(0x800E8084, "GAME.OVL");
    object = object_lookup(list);
    while (object != 0) {
        w_u32(object + 0x234, 10000); w_u32(object + 0x238, 10000);
        object = r_u32(object + 0x1DC);
    }
}
void game_update_transition_delay(uint32 object)
{
    uint32 ticks;
    FUNCTION_MARKER(0x800E80C4, "GAME.OVL");
    game_reset_player_ring_limits();
    game_reset_object_ring_limits();
    ticks = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks <= 0) w_u32(0x800F18B4, 1);
}
void game_resume_paused_transition_object(uint32 object)
{
    FUNCTION_MARKER(0x800E8200, "GAME.OVL");
    if (r_u32(object + 0xA8) != 1) return;
    w_u32(object + 0x40, 0x8009AB58);
    w_u32(object + 0xA8, 0);
}
void game_fire_equipped_mole_missile(uint32 object)
{
    uint32 state, target, offset, frame;
    FUNCTION_MARKER(0x800EC180, "GAME.OVL");
    state = r_u32(object + 0x110);
    if (r_u8(state + 0x1A4) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076D7C, 0);
        return;
    }
    if (r_u32(state + 0x1CC) == 0) return;
    target = r_u32(object + 0x178); offset = r_u32(state + 0x1A8);
    game_create_mission_projectile(object, target, offset, 1);
    frame = r_u32(0x800311B8); w_u32(state + 0x1C8, frame);
}
static void game_format_training_message(uint32 pointer)
{
    uint32 format, length;
    const char *text;
    format = r_u32(pointer);
    length = 0;
    while (r_u8(format + length) != 0) ++length;
    text = (const char *)psx_addr(format, (size_t)length + 1);
    sprintf((char *)psx_addr(0x800FA368, (size_t)length + 1), text);
}
void game_update_flight_training(uint32 object)
{
    uint32 found, warning, event, list, head, actor, player, value, next, timer, lost, phase, count, created;
    uint32 x, y, z, px, py, pz;
    sint32 distance;
    static const uint32 lists[2] = {3, 1};
    FUNCTION_MARKER(0x800E8228, "GAME.OVL");
    found = 0; warning = 0; event = 0;
    head = object_lookup(3); player = r_u32(0x80040FDC);
    game_collide_object_groups(head, player, 0, 1, 0);
    head = object_lookup(1); player = r_u32(0x80040FDC);
    game_collide_object_groups(head, player, 0, 1, 0);
    game_reset_player_ring_limits();
    game_reset_transition_object_limits();
    if (r_u32(0x800F190C) == 1) return;
    if (r_u32(0x800F18BC) == 0) {
        for (list = 0; list < 2; ++list) {
            head = object_lookup(lists[list]); w_u32(0x800F18BC, head);
            if (head != 0) {
                for (;;) {
                    actor = r_u32(0x800F18BC);
                    if (r_s16(actor + 0x262) == 3) { found = 1; break; }
                    next = r_u32(actor + 0x1DC); w_u32(0x800F18BC, next);
                    if (next == 0) break;
                }
            }
            if (found == 1) break;
        }
        actor = r_u32(0x800F18BC); w_u32(actor + 0x58, 0x8002493C);
        actor = r_u32(0x800F18BC); w_u32(0x800F18C0, 0);
        if (actor == 0) return;
    }
    actor = r_u32(0x800F18BC);
    if (r_u32(actor + 0x40) == 0x800A7864) {
        game_format_training_message(0x800F247C);
        game_request_radio_message(0x153, 0x800FA368);
        actor = r_u32(0x800F18BC); w_u32(actor + 0x40, 0x800E7730);
        actor = r_u32(0x800F18BC); w_u32(actor + 0x88, 0);
        actor = r_u32(0x800F18BC);
        x = r_u32(actor + 0x90); px = r_u32(actor + 8);
        y = r_u32(actor + 0x94); py = r_u32(actor + 12); pz = r_u32(actor + 16); z = r_u32(actor + 0x98);
        distance = vector_length_approx((sint32)(x - px), (sint32)(y - py), (sint32)(z - pz));
        actor = r_u32(0x800F18BC); w_u32(actor + 0xEC, (uint32)distance);
        created = game_create_object(0, 0, 0, 0, 0); w_u32(created + 0x40, 0x800E74A4);
        w_u32(0x800F190C, 1);
        goto emit_messages;
    }
    player = r_u32(0x80040FDC); timer = r_u32(0x800F1900);
    x = r_u32(actor + 8); px = r_u32(player + 8); py = r_u32(player + 12);
    w_u32(0x800F1900, timer - 1);
    z = r_u32(actor + 16); y = r_u32(actor + 12); pz = r_u32(player + 16);
    game_set_relative_target_angles(player, x - px, y - py, z - pz);
    value = r_u32(0x80033D1C);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value >= 512) goto drone_lost;
    value = r_u32(0x80033D30);
    if ((sint32)value < 0) value = 0 - value;
    if ((sint32)value >= 512 || r_s32(0x80046174) > 150000) goto drone_lost;
    value = r_u32(object + 0xA8);
    if (value == 2) {
        count = r_u32(object + 0xAC);
        if (count == 2) event = 1;
        else if (count == 3) event = 2;
    }
    w_u32(0x800F18FC, 0); w_u32(object + 0xA8, 0);
    goto emit_messages;
drone_lost:
    lost = r_u32(0x800F18FC); timer = r_u32(0x800F1900); ++lost;
    count = r_u32(0x800F1910);
    w_u32(0x800F18FC, lost); w_u32(0x800F1910, count + 1);
    if ((sint32)timer > 0) goto emit_messages;
    phase = r_u32(0x800F1908);
    if (phase == 0) {
        if (r_u32(object + 0xA8) == 0 && (sint32)lost >= 61) {
            count = r_u32(object + 0xAC);
            w_u32(object + 0xA8, 1); w_u32(object + 0xAC, count + 1);
        }
        if (r_u32(object + 0xA8) != 1) goto emit_messages;
        count = r_u32(object + 0xAC);
        switch (count) {
        case 1: warning = 1; w_u32(object + 0xA8, 2); break;
        case 2: warning = 2; w_u32(object + 0xA8, 2); break;
        case 3: warning = 3; w_u32(object + 0xA8, 2); break;
        case 4: warning = 5; w_u32(object + 0xA8, 2); break;
        default: break;
        }
    } else if (phase == 1 || phase == 2) {
        count = r_u32(0x800F1904);
        if ((sint32)count >= (phase == 1 ? 600 : 450)) warning = 4;
        w_u32(0x800F1904, count + 1);
    }
emit_messages:
    switch (warning) {
    case 1:
        game_format_training_message(0x800F2480); game_request_radio_message(0x14B, 0x800FA368);
        w_u32(0x800F1900, 240); w_u32(object + 0xA8, 0);
        break;
    case 2:
        game_format_training_message(0x800F2484); game_request_radio_message(0x14C, 0x800FA368);
        game_format_training_message(0x800F2488);
        actor = r_u32(0x800F18BC); w_u32(actor + 0x40, 0x800E8200);
        game_request_radio_message(0x14D, 0x800FA368);
        w_u32(0x800F1900, 300); w_u32(0x800F1908, 1); w_u32(0x800F1904, 0);
        break;
    case 3:
        game_format_training_message(0x800F2488);
        actor = r_u32(0x800F18BC); w_u32(actor + 0x40, 0x800E8200);
        game_request_radio_message(0x14D, 0x800FA368);
        w_u32(0x800F1900, 300); w_u32(0x800F1908, 2); w_u32(0x800F1904, 0);
        break;
    case 4:
    case 5:
        game_format_training_message(warning == 4 ? 0x800F248C : 0x800F2490);
        game_request_radio_message(warning == 4 ? 0x151 : 0x152, 0x800FA368);
        w_u32(0x800F18D8, 0); w_u32(0x800F190C, 1); w_u32(0x800F1910, 0);
        break;
    default:
        break;
    }
    if (event == 1) {
        game_format_training_message(0x800F2494); game_request_radio_message(0x150, 0x800FA368);
        actor = r_u32(0x800F18BC);
        w_u32(0x800F1908, 0); w_u32(0x800F1904, 0); w_u32(0x800F1900, 300); w_u32(actor + 0xA8, 1);
    } else if (event == 2) {
        game_format_training_message(0x800F2498);
        actor = r_u32(0x800F18BC); w_u32(actor + 0xA8, 1);
        game_request_radio_message(0x14F, 0x800FA368);
        w_u32(0x800F1908, 0); w_u32(0x800F1904, 0);
    }
}
static void game_format_training_target_count(sint32 count)
{
    uint32 format = r_u32(0x800F249C);
    sprintfPSX(0x800FA368, format, (uint32)count);
}
void game_announce_training_targets(sint32 count)
{
    uint32 asset;
    FUNCTION_MARKER(0x800E88F8, "GAME.OVL");
    if (count >= 2) {
        game_format_training_target_count(count);
        switch (count) {
        case 2: asset = 0x16A; break;
        case 3: asset = 0x16B; break;
        case 4: asset = 0x16C; break;
        case 5: asset = 0x16D; break;
        default: return;
        }
    } else {
        if (count != 1) return;
        game_format_training_message(0x800F24A0);
        asset = 0x169;
    }
    game_request_radio_message(asset, 0x800FA368);
}
void game_update_shield_target_training(uint32 object)
{
    const uint32 lists[2] = { 3, 1 };
    uint32 list, target, state, count, previous, remaining;
    FUNCTION_MARKER(0x800E89D0, "GAME.OVL");
    game_reset_player_ring_limits();
    game_reset_transition_list_limits(1);
    if (r_s32(0x800F18B4) > 0) return;
    if (r_u32(0x800F1914) == 0) {
        for (list = 0; list < 2; ++list) {
            target = object_lookup(lists[list]);
            while (target != 0) {
                if (r_s16(target + 0x262) == 3 && r_s32(target + 0x234) <= 0)
                    w_u8(target + 0x273, 0x10);
                target = r_u32(target + 0x1DC);
            }
        }
        w_u32(0x800F1914, 1);
    }
    for (list = 0; list < 2; ++list) {
        target = object_lookup(lists[list]);
        while (target != 0) {
            if (r_s16(target + 0x262) == 3) {
                state = r_u8(target + 0x273);
                if (state == 1) {
                    game_format_training_message(0x800F24A4);
                    game_request_radio_message(0x155, 0x800FA368);
                    w_u8(target + 0x273, 2);
                    state = r_u8(target + 0x273);
                }
                if (state == 4) {
                    game_format_training_message(0x800F24A8);
                    game_request_radio_message(0x156, 0x800FA368);
                    w_u8(target + 0x273, 0x10);
                }
            }
            target = r_u32(target + 0x1DC);
        }
    }
    count = 0;
    target = object_lookup(3);
    while (target != 0) {
        target = r_u32(target + 0x1DC);
        ++count;
    }
    previous = r_u32(0x800F18CC);
    if (previous == 0xFFFFFFFFu || (sint32)previous < (sint32)count)
        w_u32(0x800F18CC, count);
    previous = r_u32(0x800F18CC);
    if ((sint32)count < (sint32)previous) {
        remaining = r_u32(object + 0xAC) - 1;
        w_u32(object + 0xAC, remaining);
        if (remaining == 0) {
            game_format_training_message(0x800F24AC);
            game_request_radio_message(0x159, 0x800FA368);
            w_u32(0x800F18B4, 2);
        } else if ((sint32)remaining > 0) {
            game_announce_training_targets((sint32)remaining);
        }
        w_u32(0x800F18CC, count);
    }
}
void game_update_missile_target_training(uint32 object)
{
    uint32 counter, mode, target, total, fleet, drones, previous, remaining;
    FUNCTION_MARKER(0x800E8C6C, "GAME.OVL");
    counter = r_u32(0x800F1928);
    w_u32(0x800F1928, counter + 1);
    game_reset_player_ring_limits();
    game_reset_transition_list_limits(1);
    if (r_u32(0x800F18B4) == 1) return;
    mode = r_u32(0x800EF4BC);
    if (mode == 6 || mode == 4) {
        if (r_u32(0x800F191C) == 0 && r_s32(0x800F1928) >= 601) {
            game_format_training_message(0x800F24B0);
            game_request_radio_message(0x15F, 0x800FA368);
            w_u32(0x800F191C, 1);
        }
        mode = r_u32(0x800EF4BC);
    }
    if (mode == 5 && r_u32(0x800F1918) == 0 && r_s32(0x800F1928) >= 601) {
        game_format_training_message(0x800F24B4);
        game_request_radio_message(0x160, 0x800FA368);
        w_u32(0x800F1918, 1);
    }
    total = 0; fleet = 0; drones = 0;
    target = object_lookup(3);
    while (target != 0) {
        if ((r_u32(target + 0x114) & 0x10) != 0) ++fleet;
        else ++drones;
        target = r_u32(target + 0x1DC);
        ++total;
    }
    if ((sint32)total > 0 && fleet == 0 && r_u32(0x800F1920) == 0) {
        if (r_s32(0x800F1928) >= 601) {
            game_format_training_message(0x800F24B8);
            game_request_radio_message(0x165, 0x800FA368);
        }
        w_u32(0x800F1920, 1);
    }
    if ((sint32)total > 0 && drones == 0 && r_u32(0x800F1924) == 0) {
        if (r_s32(0x800F1928) >= 601) {
            game_format_training_message(0x800F24BC);
            game_request_radio_message(0x166, 0x800FA368);
        }
        w_u32(0x800F1924, 1);
    }
    previous = r_u32(0x800F18CC);
    if (previous == 0xFFFFFFFFu || (sint32)previous < (sint32)total)
        w_u32(0x800F18CC, total);
    previous = r_u32(0x800F18CC);
    if ((sint32)total < (sint32)previous) {
        remaining = r_u32(object + 0xAC) - 1;
        w_u32(object + 0xAC, remaining);
        if (remaining == 0) {
            game_format_training_message(0x800F24C0);
            game_request_radio_message(0x167, 0x800FA368);
            w_u32(0x800F18B4, 1);
        } else if ((sint32)remaining > 0 && r_s32(0x800F1928) >= 601) {
            game_announce_training_targets((sint32)remaining);
        }
        w_u32(0x800F18CC, total);
    }
}
uint32 front_finish_mode_transition(uint32 object)
{
    FUNCTION_MARKER(0x80095D5C, "FRONT.OVL");
    if (r_s16(0x800B737C) == 1) {
        w_u16(object + 0x2C, 2);
        return 2;
    }
    w_u32(0x800F2D28, 11);
    w_u16(object + 0x2C, 0);
    return 11;
}
void front_draw_wide_text_native(const char *text, uint32 x, uint32 y, uint32 color)
{
    uint32 character, bank, atlas, page, cached_page, shift, packet, table, value, opcode;
    uint32 atlas_x, atlas_y, row, column, bits, next_packet, pixel_y, shifted_y;
    uint32 red, green, blue, pixel_red, pixel_green, pixel_blue;
    sint32 glyph, depth, graph_type, bias, previous_page, character_height;
    uint16 clut;
    const uint8 *bitmap;
    FUNCTION_MARKER(0x80096C3C, "FRONT.OVL");
    page = 0; cached_page = 0;
    character = (uint8)text[0] | ((uint32)(uint8)text[1] << 8);
    if ((character & 0x80) == 0) {
        front_draw_colored_text_native(text, x, y, color);
        return;
    }
    bank = r_u32(0x8010D26C);
    value = r_u32(0x800FB870);
    green = (color >> 8) & 0xFF; blue = (color >> 16) & 0xFF; red = color & 0xFF;
    character_height = r_s16(0x8004EB7A + ((bank + value) << 4));
    bias = (16 - character_height) / 2;
    pixel_red = (red - 1) << 1;
    do {
        character = (uint32)(sint32)(sint16)((uint8)text[0] | ((uint32)(uint8)text[1] << 8));
        glyph = front_wide_character_glyph(character);
        if (glyph == -1) {
            x += 4;
        } else if (glyph >= 0) {
            bank = r_u32(0x800FB870);
            atlas = 0x8004EB70 + ((bank + (uint32)glyph) << 4);
            depth = r_s16(atlas + 12);
            graph_type = GetGraphType();
            shift = (2u - (uint32)depth) & 31;
            if (graph_type == 1 || GetGraphType() == 2) {
                atlas_y = r_u16(atlas + 2); atlas_x = r_u16(atlas);
                page = (((uint32)depth & 3) << 9) | ((atlas_y & 0x300) >> 3) |
                       0x80 | ((atlas_x & 0x3FF) >> 6);
            } else {
                atlas_y = r_u16(atlas + 2); atlas_x = r_u16(atlas);
                page = (((uint32)depth & 3) << 7) | ((atlas_y >> 4) & 0x10) |
                       0x20 | ((atlas_x & 0x3FF) >> 6) | ((atlas_y & 0x200) << 2);
            }
            if ((uint16)cached_page == 0) cached_page = (uint16)page;
            previous_page = (sint16)cached_page;
            if ((uint32)previous_page != page) {
                packet = r_u32(0x801132B8);
                cached_page = (uint16)page;
                SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)previous_page, NULL);
                table = r_u32(0x801132BC);
                next_packet = packet + 12;
                AddPrim(psx_addr(table, 4), psx_addr(packet, 12));
                w_u32(0x801132B8, next_packet);
            }
            packet = r_u32(0x801132B8);
            w_u8(packet + 3, 4); w_u8(packet + 7, 0x64);
            w_u16(packet + 8, x); w_u16(packet + 10, y);
            if (bias > 0) w_u16(packet + 10, y + (uint32)bias);
            value = r_u16(atlas);
            w_u8(packet + 12, (value & 63) << shift);
            w_u8(packet + 13, r_u16(atlas + 2));
            w_u16(packet + 16, r_u16(atlas + 14));
            w_u16(packet + 18, r_u16(atlas + 10));
            clut = GetClut(r_s16(atlas + 4), r_s16(atlas + 6));
            w_u16(packet + 14, clut);
            opcode = r_u8(packet + 7);
            w_u8(packet + 4, red); w_u8(packet + 5, green);
            w_u8(packet + 7, opcode | 2); w_u8(packet + 6, blue);
            table = r_u32(0x801132BC);
            AddPrim(psx_addr(table, 4), psx_addr(packet, 20));
            value = (uint32)r_s16(atlas + 14);
            w_u32(0x801132B8, packet + 20);
            x += value;
        } else {
            character = (uint32)(sint32)(sint16)((uint8)text[0] | ((uint32)(uint8)text[1] << 8));
            character = ((character & 0xFF) << 8) | ((character & 0xFF00) >> 8);
            bitmap = Krom2RawAdd(character);
            if (bitmap != NULL) {
                next_packet = r_u32(0x801132B8);
                pixel_green = (green - 1) << 1;
                pixel_blue = (blue - 1) << 1;
                packet = next_packet;
                for (row = 0; row < 15; ++row) {
                    bits = bitmap[0] | ((uint32)bitmap[1] << 8);
                    bits = (bits >> 8) | (bits << 8);
                    pixel_y = y + row;
                    shifted_y = pixel_y - (uint32)bias;
                    for (column = 0; column < 16; ++column) {
                        if ((bits & 0x8000) != 0) {
                            w_u8(packet + 7, 0x68); w_u8(packet + 3, 2);
                            w_u16(packet + 8, x + column); w_u16(packet + 10, pixel_y);
                            if (bias < 0) w_u16(packet + 10, shifted_y);
                            next_packet += 12;
                            w_u8(packet + 4, pixel_red); w_u8(packet + 5, pixel_green);
                            opcode = r_u8(packet + 7);
                            w_u8(packet + 6, pixel_blue); w_u8(packet + 7, opcode | 2);
                            table = r_u32(0x801132BC);
                            value = packet;
                            packet += 12;
                            AddPrim(psx_addr(table, 4), psx_addr(value, 12));
                        }
                        bits <<= 1;
                    }
                    bitmap += 2;
                }
                x += 16;
                w_u32(0x801132B8, next_packet);
            }
        }
        text += 2;
        character = (uint8)text[0] | ((uint32)(uint8)text[1] << 8);
    } while (character != 0);
    packet = r_u32(0x801132B8);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)page, NULL);
    table = r_u32(0x801132BC);
    next_packet = packet + 12;
    AddPrim(psx_addr(table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, next_packet);
}
void front_draw_wide_text(uint32 text, uint32 x, uint32 y, uint32 color)
{
    uint32 length = 0;
    if ((r_u16(text) & 0x80) != 0) {
        while (r_u16(text + length) != 0) length += 2;
        length += 2;
    } else {
        while (r_u8(text + length) != 0) ++length;
        ++length;
        if (length < 2) length = 2;
    }
    front_draw_wide_text_native((const char *)psx_addr(text, length), x, y, color);
}
static void game_scale_sky_cell(uint32 cell, uint32 factor)
{
    uint32 color, red, green, blue;
    color = r_u32(cell);
    red = (uint32)((sint32)((color & 0xFF) * factor) >> 8);
    green = (uint32)((sint32)(((color & 0xFF00) >> 8) * factor) >> 8);
    blue = (uint32)((sint32)((uint32)((sint32)color >> 16) * factor) >> 8);
    w_u32(cell, red + (green << 8) + (blue << 16));
}
static void game_add_sky_cell_color(uint32 cell, uint32 addition)
{
    uint32 color, red, green, blue;
    color = r_u32(cell);
    red = (addition & 0xFF) + (color & 0xFF);
    green = ((addition & 0xFF00) + (color & 0xFF00)) >> 8;
    blue = ((addition & 0xFF0000) + (color & 0xFF0000)) >> 16;
    if (red >= 256) red = 255;
    if (green >= 256) green = 255;
    if (blue >= 256) blue = 255;
    w_u32(cell, red + (green << 8) + (blue << 16));
}
void game_generate_scene_sky(uint32 seed, uint32 color, uint32 second_color, uint32 third_color, uint32 fourth_color)
{
    uint32 colors[4], sums[4], heights[4], cell, first, second, third, fourth;
    uint32 row, column, step, end_row, end_column, fill_row, fill_column, channel, component;
    uint32 random, mask, half, value, mode, level, initial_mode, stripe, stripe_count, offset;
    uint32 palette[6], factor, cosine, product, x, y, z;
    sint32 contributions[3], height, result, elevation, azimuth;
    VECTOR up;
    SVECTOR angles;
    MATRIX rotation;
    FUNCTION_MARKER(0x800E2E70, "GAME.OVL");
    initial_mode = r_u32(0x800F15EC);
    if (initial_mode != 0) {
        x = r_u32(0x801193FC); y = r_u32(0x80119400); z = r_u32(0x80119404);
        game_orient_radial_mesh(0 - x, 0 - y, 0 - z);
    }
    w_u32(0x800FD718, 0x800510F8);
    for (row = 0; row < 32; ++row) {
        for (column = 0; column < 32; ++column) {
            cell = 0x800F8210 + row * 256 + column * 8;
            w_u16(cell, 0); w_u16(cell + 2, 0); w_u16(cell + 4, 0); w_u16(cell + 6, 0);
        }
    }
    w_u32(0x800EF50C, seed);
    for (step = 16; step != 0; step >>= 1) {
        mask = (step << 4) & 0x7FFF; half = (step << 4) >> 1;
        for (row = 0; row < 32; row += step) {
            end_row = row + step;
            for (column = 0; column < 32; column += step) {
                end_column = column + step;
                first = 0x800F8210 + row * 256 + column * 8;
                second = 0x800F8210 + ((end_row & 31) << 8) + column * 8;
                third = 0x800F8210 + ((end_row & 31) << 8) + ((end_column & 31) << 3);
                fourth = 0x800F8210 + row * 256 + ((end_column & 31) << 3);
                sums[0] = (uint32)r_s16(first); value = (uint32)r_s16(second); sums[0] += value;
                value = (uint32)r_s16(third); sums[0] += value; value = (uint32)r_s16(fourth);
                sums[0] += value;
                sums[1] = (uint32)r_s16(first + 2); value = (uint32)r_s16(second + 2);
                sums[1] += value; value = (uint32)r_s16(fourth + 2);
                height = r_s16(third + 2); sums[1] += (uint32)height + value;
                sums[2] = (uint32)r_s16(fourth + 4); value = (uint32)r_s16(first + 4);
                height = r_s16(second + 4); sums[2] += value + (uint32)height;
                value = (uint32)r_s16(third + 4); sums[2] += value;
                sums[3] = (uint32)r_s16(first + 6); value = (uint32)r_s16(second + 6);
                height = r_s16(third + 6); sums[3] += value + (uint32)height;
                value = (uint32)r_s16(fourth + 6); sums[3] += value;
                random = r_u32(0x800EF50C);
                for (channel = 0; channel < 4; ++channel) {
                    random = (uint32)((sint32)random >> 1) ^ (random << 3);
                    heights[channel] = (uint32)((sint32)sums[channel] >> 2) + (random & mask) - half;
                }
                w_u32(0x800EF50C, random);
                for (fill_row = row; fill_row < end_row; ++fill_row) {
                    for (fill_column = column; fill_column < end_column; ++fill_column) {
                        cell = 0x800F8210 + fill_row * 256 + fill_column * 8;
                        w_u16(cell, heights[0]); w_u16(cell + 2, heights[1]);
                        w_u16(cell + 4, heights[2]); w_u16(cell + 6, heights[3]);
                    }
                }
            }
        }
    }
    colors[0] = color; colors[1] = second_color; colors[2] = third_color; colors[3] = fourth_color;
    for (row = 0; row < 32; ++row) {
        for (column = 0; column < 32; ++column) {
            cell = 0x800F8210 + row * 256 + column * 8;
            contributions[0] = 0; contributions[1] = 0; contributions[2] = 0;
            for (channel = 0; channel < 4; ++channel) {
                height = (sint16)r_u16(cell + channel * 2);
                height >>= 1;
                w_u16(cell + channel * 2, (uint16)height);
                if (channel != 3) height = r_s16(cell + channel * 2);
                for (component = 0; component < 3; ++component) {
                    product = ((colors[channel] >> (component * 8)) & 0xFF) * (uint32)height;
                    contributions[component] += (sint32)product >> 8;
                }
            }
            value = 0;
            for (component = 0; component < 3; ++component) {
                result = contributions[component] >> 1;
                if (result >= 256) result = 255;
                if (result < 0) result = 0;
                value += (uint32)result << (component * 8);
            }
            w_u32(0x80109B50 + row * 128 + column * 4, value);
        }
    }
    for (channel = 0; channel < 6; ++channel) palette[channel] = 0;
    level = r_u32(0x800F15F0); mode = r_u32(0x800F15EC);
    for (column = 0; column < 32; ++column) {
        if (mode == 0) {
            for (row = 0; row < 5; ++row) game_scale_sky_cell(0x80109B50 + row * 128 + column * 4, row * 51);
        } else {
            if (mode == 1) {
                palette[0] = 0xFFFFFF; palette[1] = 0xFFFF; palette[2] = 0x80FF;
                palette[3] = 0xFF; palette[4] = 0x80; palette[5] = 0x50;
            }
            if (mode == 2) {
                palette[0] = 0xFFFFFF; palette[1] = 0x20FF; palette[2] = 0xE0;
                palette[3] = 0x80; palette[4] = 0x40; palette[5] = 0x10;
            }
            if (mode == 3) {
                palette[0] = 0xFFFFFF; palette[1] = 0xFF00FF; palette[2] = 0xE000E0;
                palette[3] = 0x800080; palette[4] = 0x400040; palette[5] = 0x100010;
            }
            stripe_count = (sint32)level >= 151 ? 10 : ((sint32)level >= 121 ? 8 : ((sint32)level >= 80 ? 6 : 0));
            for (row = 0; row < stripe_count; ++row) {
                if (stripe_count == 10) {
                    stripe = row < 4 ? palette[0] : (row == 9 ? 0 : palette[row - 3]);
                } else if (stripe_count == 8) {
                    stripe = row < 2 ? palette[0] : (row == 7 ? 0 : palette[row - 1]);
                } else {
                    stripe = row < 2 ? palette[0] : (row == 2 ? palette[1] : (row == 3 ? palette[3] : (row == 4 ? palette[5] : 0)));
                }
                game_add_sky_cell_color(0x80109B50 + row * 128 + column * 4, stripe);
            }
        }
        offset = r_s16(0x800595F2) == 0 ? 31 : 23;
        for (row = 0; row < 7; ++row) {
            factor = (row * 255) / 7;
            game_scale_sky_cell(0x80109B50 + (offset - row) * 128 + column * 4, factor);
        }
        if (offset == 23) {
            for (row = 0; row < 8; ++row) w_u32(0x80109B50 + (31 - row) * 128 + column * 4, 0);
        }
    }
    for (row = 0; row < 32; ++row) {
        elevation = -1024 + (sint32)row * 64;
        for (column = 0; column < 32; ++column) {
            azimuth = (sint32)column * 128;
            cell = 0x800F8210 + row * 256 + column * 8;
            cosine = (uint32)((sint32)((uint32)rcos(elevation) << 12) >> 12);
            product = cosine * (uint32)rsin(azimuth);
            w_u16(cell, (sint32)product >> 12);
            w_u16(cell + 2, rsin(elevation));
            cosine = (uint32)((sint32)((uint32)rcos(elevation) << 12) >> 12);
            product = cosine * (uint32)rcos(azimuth);
            w_u16(cell + 4, (sint32)product >> 12);
        }
    }
    up.vx = 0; up.vy = 4096; up.vz = 0;
    matrix_basis_from_vectors((VECTOR *)psx_addr(0x800ED3AC, 12), &up, 0x80123FE8);
    angles.vx = 1024; angles.vy = 0; angles.vz = 0;
    RotMatrixYXZ(&angles, &rotation);
    MulMatrix0((MATRIX *)psx_addr(0x80123FE8, 20), &rotation, (MATRIX *)psx_addr(0x80123FE8, 20));
}
static uint32 game_combat_facing_score(uint32 dot)
{
    if ((sint32)dot > 3072) return 0xFFFFF000;
    if ((sint32)dot < -3072) return 4096;
    return 0 - (uint32)((sint32)(dot << 12) / 3072);
}
static void game_combat_unit_difference(uint32 object, uint32 target, uint32 *difference)
{
    uint32 x, y, z, other_x, other_y, other_z, length, component, numerator;
    x = r_u32(object + 8); other_x = r_u32(target + 8);
    y = r_u32(object + 12); z = r_u32(object + 16);
    other_y = r_u32(target + 12); other_z = r_u32(target + 16);
    difference[0] = x - other_x; difference[1] = y - other_y; difference[2] = z - other_z;
    length = (uint32)vector_length_approx((sint32)difference[0], (sint32)difference[1], (sint32)difference[2]);
    if (length == 0) length = 1;
    for (component = 0; component < 3; ++component) {
        numerator = difference[component] << 12;
        if (length == 0xFFFFFFFFu && numerator == 0x80000000u) trap(0x5D);
        difference[component] = (uint32)((sint64)(sint32)numerator / (sint32)length);
    }
}
static void game_apply_combat_choice(uint32 object, uint32 choice, uint32 row)
{
    static const uint8 choices[22][9] = {
        {2, 2, 3, 7, 12, 9, 0, 7, 13},
        {3, 10, 4, 10, 12, 11, 0, 11, 13},
        {0, 1, 3, 7, 12, 9, 0, 7, 7},
        {11, 10, 4, 10, 12, 11, 0, 11, 13},
        {0, 7, 0, 7, 12, 9, 0, 7, 7},
        {11, 10, 4, 10, 12, 11, 0, 11, 13},
        {2, 0, 3, 7, 0, 9, 0, 7, 0},
        {2, 11, 4, 11, 0, 11, 0, 11, 0},
        {0, 10, 4, 10, 12, 11, 0, 11, 13},
        {11, 10, 11, 10, 0, 11, 0, 11, 0},
        {0, 0, 4, 7, 12, 0, 0, 7, 7},
        {0, 10, 4, 10, 12, 11, 0, 11, 13},
        {2, 2, 3, 7, 12, 9, 0, 7, 7},
        {11, 0, 4, 0, 12, 11, 0, 11, 13},
        {2, 2, 3, 7, 12, 9, 0, 7, 7},
        {0, 10, 4, 10, 12, 0, 0, 0, 13},
        {0, 7, 3, 7, 0, 9, 0, 7, 0},
        {0, 7, 3, 7, 0, 9, 0, 7, 0},
        {0, 7, 3, 7, 0, 9, 0, 7, 0},
        {2, 0, 3, 10, 0, 11, 0, 10, 0},
        {2, 0, 3, 10, 0, 11, 0, 10, 0},
        {0, 0, 3, 7, 0, 9, 0, 7, 7}
    };
    uint32 next;
    next = choices[row][choice - 1];
    if (next != 0) w_u8(r_u32(object + 0x200), next);
}
void game_update_combat_maneuver(uint32 object)
{
    uint32 target, state, state_value, last_state, original_state, flag, best, index, value, other;
    uint32 distance, range_score, closing_score, heading_score, self_score, target_score, dot, difference[3];
    uint32 x, y, z, other_x, other_y, other_z, velocity, other_velocity, predicted, numerator, high;
    uint32 speed_mode, turn_mode, decoy_hint, waypoint, roll, boost, choice, timer, threshold;
    uint32 kind, metadata, desired, base_speed, yaw, pitch, serial, frame, quotient, threat;
    uint32 first, second, position, axis;
    sint32 maximum;
    FUNCTION_MARKER(0x800DBE20, "GAME.OVL");
    target = r_u32(object + 0x178);
    game_evaluate_target_occlusion(object, target, 1);
    distance = r_u32(0x80046174);
    w_u32(object + 0xE8, 0);
    if ((sint32)distance < 5000) range_score = 4096;
    else if ((sint32)distance > 115000) range_score = 0xFFFFF000;
    else {
        numerator = (distance - 5000) << 12;
        high = (uint32)(((sint64)(sint32)numerator * (sint32)0x98852841u) >> 32);
        value = (uint32)((sint32)(high + numerator) >> 15) - (uint32)((sint32)numerator >> 31);
        range_score = 4096 - value;
    }
    x = r_u32(object + 8); other_x = r_u32(target + 8);
    velocity = r_u32(object + 0x14); other_velocity = r_u32(target + 0x14);
    other_y = r_u32(target + 12); other_z = r_u32(target + 16);
    x = x - other_x + (uint32)((sint32)(velocity - other_velocity) >> 8);
    y = r_u32(object + 12); velocity = r_u32(object + 0x18); other_velocity = r_u32(target + 0x18);
    y = y - other_y + (uint32)((sint32)(velocity - other_velocity) >> 8);
    z = r_u32(object + 16); velocity = r_u32(object + 0x1C); other_velocity = r_u32(target + 0x1C);
    z = z - other_z + (uint32)((sint32)(velocity - other_velocity) >> 8);
    predicted = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    value = r_u32(0x80046174) - predicted;
    if ((sint32)value < -50) closing_score = 0xFFFFF000;
    else if ((sint32)value >= 51) closing_score = 4096;
    else closing_score = (uint32)((sint32)(value << 12) / 50);
    first = (uint32)(sint32)r_s16(object + 0x120); second = (uint32)(sint32)r_s16(target + 0x128);
    dot = first * second;
    first = (uint32)(sint32)r_s16(object + 0x126); second = (uint32)(sint32)r_s16(target + 0x12A);
    dot += first * second;
    first = (uint32)(sint32)r_s16(object + 0x12C); second = (uint32)(sint32)r_s16(target + 0x12C);
    dot += first * second;
    heading_score = game_combat_facing_score((uint32)((sint32)dot >> 12));
    game_combat_unit_difference(object, target, difference);
    dot = 0;
    for (axis = 0; axis < 3; ++axis)
        dot += (uint32)(sint32)r_s16(object + 0x120 + axis * 6) * (0 - difference[axis]);
    self_score = game_combat_facing_score((uint32)((sint32)dot >> 12));
    game_combat_unit_difference(object, target, difference);
    dot = 0;
    for (axis = 0; axis < 3; ++axis)
        dot += (uint32)(sint32)r_s16(target + 0x128 + axis * 2) * difference[axis];
    target_score = game_combat_facing_score((uint32)((sint32)dot >> 12));
    w_u32(0x8010AC5C, (closing_score - range_score) * 5);
    w_u32(0x8010AC60, 0 - (range_score + closing_score) * 5);
    w_u32(0x8010AC64, (range_score + heading_score) * 2 + closing_score * 3 - self_score - target_score + 4096);
    w_u32(0x8010AC68, range_score - closing_score + heading_score + (self_score + target_score) * 3 + 4096);
    value = range_score - heading_score * 3 - ((self_score + target_score + 4096) << 1);
    w_u32(0x8010AC74, (range_score - 4096) * 5);
    w_u32(0x8010AC78, range_score * 2 + closing_score * 4 - self_score * 3 + 4096);
    other = r_u32(0x8010AC58);
    w_u32(0x8010AC6C, value); w_u32(0x8010AC70, value);
    w_u32(0x8010AC7C, range_score * 2 + closing_score * 4 - target_score * 3 + 4096);
    if (other != 0xFFFFFFFFu) w_u32(0x8010AC58, 0xFFFFFFFFu);
    if ((r_u32(0x800311B8) & 0x3FF) < 450) {
        if ((sint32)r_u32(0x8010AC7C) < (sint32)value) w_u32(0x8010AC7C, value);
        w_u32(0x8010AC6C, 0);
    }
    maximum = r_s32(0x8010AC5C); best = 1;
    for (index = 1; index < 9; ++index) {
        value = r_u32(0x8010AC5C + index * 4);
        if (maximum < (sint32)value) { maximum = (sint32)value; best = index + 1; }
    }
    state = r_u32(object + 0x200); value = r_u8(state);
    if (value == 0 || value >= 21) w_u8(state, 1);
    decoy_hint = 0; speed_mode = 0; turn_mode = 0; waypoint = 0; roll = 0; boost = 0;
    state = r_u32(object + 0x200); original_state = r_u8(state);
    distance = r_u32(0x80046174);
    switch (original_state) {
    case 1:
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        game_apply_combat_choice(object, best, flag == 1 ? 0 : 1);
        break;
    case 2:
        speed_mode = 2; turn_mode = 2;
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        game_apply_combat_choice(object, best, flag == 1 ? 2 : 3);
        break;
    case 3:
        speed_mode = 2; turn_mode = 2;
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        game_apply_combat_choice(object, best, flag == 1 ? 4 : 5);
        if ((sint32)distance < 10000) w_u8(r_u32(object + 0x200), 5);
        break;
    case 4:
        waypoint = 1; w_u8(r_u32(object + 0x200), 5);
        break;
    case 5:
        speed_mode = 1; turn_mode = 1;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) w_u16(state + 0x34, 0);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) {
            if (best == 6) w_u8(state, 9);
            else if (best == 4 || best == 8) w_u8(state, 7);
        } else {
            if (best == 6 || best == 8) w_u8(state, 11);
            else if (best == 4) w_u8(state, 10);
        }
        state = r_u32(object + 0x200);
        if (r_s16(state + 0x34) >= 61) w_u8(state, flag == 1 ? 7 : 6);
        break;
    case 6:
        speed_mode = 3;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) w_u16(state + 0x34, 0);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        game_apply_combat_choice(object, best, flag == 1 ? 6 : 7);
        state = r_u32(object + 0x200);
        if (r_s16(state + 0x34) >= 61) w_u8(state, flag == 1 ? 1 : 11);
        break;
    case 7:
        speed_mode = 1; turn_mode = 4;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) w_u16(state + 0x34, 0);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) {
            if (best == 3) w_u8(state, 4);
            else if (best == 5) w_u8(state, 12);
            else if (best == 6) w_u8(state, 9);
        } else game_apply_combat_choice(object, best, 8);
        if ((sint32)distance > 100000) w_u8(r_u32(object + 0x200), flag == 1 ? 8 : 10);
        state = r_u32(object + 0x200);
        if (r_s16(state + 0x34) >= (flag == 1 ? 151 : 61)) w_u8(state, 1);
        break;
    case 8:
        speed_mode = 4; turn_mode = 4;
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) { if (best == 6) w_u8(state, 9); }
        else game_apply_combat_choice(object, best, 9);
        if ((sint32)distance > 100000) w_u8(r_u32(object + 0x200), flag == 1 ? 8 : 10);
        break;
    case 9:
        speed_mode = 4; turn_mode = 4;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) w_u16(state + 0x34, 0);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) {
            game_apply_combat_choice(object, best, 10);
            frame = r_u32(0x800311B8); state = r_u32(object + 0x200);
            timer = (uint32)(sint32)r_s16(state + 0x34);
            if ((frame & 31) + 420 < timer && (sint32)distance < 100000) w_u8(state + 12, 0);
        } else game_apply_combat_choice(object, best, 11);
        break;
    case 10:
        speed_mode = 1; turn_mode = 5;
        flag = r_u8(r_u32(object + 0x200) + 12);
        game_apply_combat_choice(object, best, flag == 1 ? 12 : 13);
        break;
    case 11:
        speed_mode = 3; turn_mode = 5;
        flag = r_u8(r_u32(object + 0x200) + 12);
        game_apply_combat_choice(object, best, flag == 1 ? 14 : 15);
        break;
    case 12:
        choice = (uint32)random_between(1, 3);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (choice == 1) w_u8(state, 17);
        else if (choice == 2) w_u8(state, 20);
        else if (choice == 3) w_u8(state, 14);
        break;
    case 13:
        speed_mode = 1; turn_mode = 2; decoy_hint = 1;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) w_u16(state + 0x34, 0);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) {
            if (best == 3) w_u8(state, 3);
            else if (best == 6) w_u8(state, 9);
            else if (best == 8) w_u8(state, 7);
        } else {
            if (best == 3) w_u8(state, 4);
            else if (best == 6 || best == 8) w_u8(state, 11);
        }
        state = r_u32(object + 0x200);
        if (r_s16(state + 0x34) >= 61) w_u8(state, 1);
        break;
    case 14:
        choice = (uint32)random_between(1, 2);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) game_apply_combat_choice(object, best, 16);
        else {
            if (best == 3) w_u8(state, 4);
            else if (best == 4) w_u8(state, 10);
            else if (best == 6 || best == 8) w_u8(state, 11);
        }
        if (choice == 1) w_u8(r_u32(object + 0x200), 16);
        else if (choice == 2) w_u8(r_u32(object + 0x200), 15);
        break;
    case 15:
    case 16:
        speed_mode = 2; turn_mode = original_state == 15 ? 7 : 8; decoy_hint = 1;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) {
            w_u16(state + 0x34, 0);
            choice = (uint32)random_between(1, 3);
            w_u16(r_u32(object + 0x200) + 2, choice);
        } else choice = (uint32)(sint32)r_s16(state + 2);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) game_apply_combat_choice(object, best, original_state == 15 ? 17 : 18);
        else {
            if (best == 3) w_u8(state, 4);
            else if (best == 4) w_u8(state, 10);
            else if (best == 6 || best == 8) w_u8(state, 11);
        }
        state = r_u32(object + 0x200); timer = (uint32)(sint32)r_s16(state + 0x34);
        value = (uint32)random_between(1, 2) + 1;
        threshold = ((value << 4) - value) << 1;
        if ((sint32)threshold < (sint32)timer) w_u8(r_u32(object + 0x200), original_state == 15 ? 16 : 15);
        if (choice == 1) w_u8(r_u32(object + 0x200), 1);
        break;
    case 17:
    case 18:
        speed_mode = original_state == 17 ? 3 : 5; turn_mode = 3; roll = 1;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) { w_u16(state + 0x34, 0); state = r_u32(object + 0x200); }
        flag = r_u8(state + 12);
        if (flag == 1) {
            if (best == 3) w_u8(state, 3);
            else if (best == 4 || best == 8) w_u8(state, 7);
            else if (best == 6) w_u8(state, 9);
        } else game_apply_combat_choice(object, best, original_state == 17 ? 19 : 20);
        if ((sint32)distance > 20000) w_u8(r_u32(object + 0x200), 19);
        state = r_u32(object + 0x200);
        if (r_s16(state + 0x34) >= (original_state == 17 ? 91 : 61)) w_u8(state, original_state == 17 ? 18 : 19);
        break;
    case 19:
        speed_mode = 1; turn_mode = 9;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) w_u16(state + 0x34, 0);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) {
            game_apply_combat_choice(object, best, 21);
            state = r_u32(object + 0x200);
            if (r_s16(state + 0x34) >= 61) w_u8(state, 1);
        } else if (flag == 0) {
            if (best == 3) w_u8(state, 4);
            else if (best == 4) w_u8(state, 10);
            else if (best == 6 || best == 8) w_u8(state, 11);
            state = r_u32(object + 0x200);
            if (r_s16(state + 0x34) >= 61) w_u8(state, 6);
        }
        break;
    case 20:
        speed_mode = 3; turn_mode = 6; boost = 1; decoy_hint = 1;
        state = r_u32(object + 0x200); state_value = r_u8(state); last_state = r_u32(state + 0x30);
        if (state_value != last_state) w_u16(state + 0x34, 0);
        state = r_u32(object + 0x200); flag = r_u8(state + 12);
        if (flag == 1) {
            if (best == 3 || best == 4 || best == 8) w_u8(state, 7);
            else if (best == 6) w_u8(state, 9);
        } else {
            if (best == 3) w_u8(state, 4);
            else if (best == 4) w_u8(state, 10);
            else if (best == 6 || best == 8) w_u8(state, 11);
        }
        state = r_u32(object + 0x200);
        if (r_s16(state + 0x34) >= 91) w_u8(state, 1);
        break;
    default:
        break;
    }
    state = r_u32(object + 0x200); value = r_u16(state + 0x34);
    w_u16(state + 0x34, value + 1);
    state = r_u32(object + 0x200);
    if (r_s16(state + 8) < 0) { w_u16(state + 8, 0); state = r_u32(object + 0x200); }
    if (original_state != r_u32(state + 0x30) && r_s16(state + 8) >= 31) w_u16(state + 8, 0);
    state = r_u32(object + 0x200); value = r_u16(state + 8); w_u16(state + 8, value + 1);
    if (decoy_hint == 1 && random_between(1, 50) < 5) {
        state = r_u32(object + 0x200);
        if (original_state != r_u32(state + 0x30)) game_try_launch_target_decoy(object);
    }
    state = r_u32(object + 0x200); w_u32(state + 0x30, original_state);
    kind = r_u32(object + 4); value = (uint32)(sint32)r_s16(0x800536B2 + kind * 60);
    w_u32(object + 0xF0, value);
    switch (speed_mode) {
    case 1:
        kind = r_u32(object + 4); value = r_u32(0x8005369C + kind * 60);
        w_u32(object + 0xE8, (sint32)(value * 3) >> 2);
        break;
    case 2:
        kind = r_u32(object + 4); w_u32(object + 0xE8, r_u32(0x8005369C + kind * 60));
        break;
    case 3:
        kind = r_u32(object + 4); value = r_u32(0x800536A0 + kind * 60);
        kind = r_u32(object + 4); w_u32(object + 0xE8, value);
        value = (uint32)(sint32)r_s16(0x800536B2 + kind * 60); w_u32(object + 0xF0, value << 1);
        break;
    case 4:
    case 5:
        value = r_u32(target + 0xEC); kind = r_u32(object + 4); value <<= 1;
        w_u32(object + 0xE8, value);
        other = r_u32(0x800536A0 + kind * 60);
        if ((sint32)other < (sint32)value) w_u32(object + 0xE8, other);
        kind = r_u32(object + 4); base_speed = r_u32(0x8005369C + kind * 60);
        value = r_u32(object + 0xE8);
        if ((sint32)value < ((sint32)base_speed >> 1))
            w_u32(object + 0xE8, speed_mode == 4 ? (uint32)((sint32)base_speed >> 1) : base_speed << 1);
        break;
    default:
        break;
    }
    kind = r_u32(object + 4); desired = r_u32(object + 0xE8); metadata = kind * 60;
    base_speed = r_u32(0x8005369C + metadata);
    if ((sint32)base_speed < (sint32)desired || (sint32)base_speed < r_s32(object + 0xEC)) {
        value = r_u32(object + 0xEC);
        other = (uint32)(sint32)r_s16(0x800536B2 + metadata);
        w_u32(object + 0xF0, other << ((sint32)desired < (sint32)value ? 2 : 1));
    }
    switch (turn_mode) {
    case 0:
        w_u16(object + 0x250, 0); w_u16(object + 0x252, 0); w_u16(object + 0x254, 0);
        break;
    case 1:
        state = r_u32(object + 0x200); x = r_u32(object + 8); y = r_u32(object + 12);
        other_x = r_u32(state + 0x18); z = r_u32(object + 16);
        other_y = r_u32(state + 0x1C); other_z = r_u32(state + 0x20);
        game_set_relative_target_angles(object, other_x - x, other_y - y, other_z - z);
        yaw = r_u32(0x80033D1C); pitch = r_u32(0x80033D30);
        w_u16(object + 0x250, yaw); w_u16(object + 0x252, pitch);
        break;
    case 2:
    case 3:
    case 5:
        yaw = r_u32(0x80033D1C); pitch = r_u32(0x80033D30);
        if (turn_mode == 3 || (turn_mode == 5 && (sint32)distance < 100000)) { yaw = 0 - yaw; pitch = 0 - pitch; }
        w_u16(object + 0x250, yaw); w_u16(object + 0x252, pitch);
        break;
    case 4:
        value = (uint32)(sint32)r_s16(target + 0x128);
        x = r_u32(target + 8); y = r_u32(target + 12); z = r_u32(target + 16);
        x -= (uint32)((sint32)(value * 125) >> 9); x -= r_u32(object + 8);
        value = (uint32)(sint32)r_s16(target + 0x12A);
        y -= (uint32)((sint32)(value * 125) >> 9); y -= r_u32(object + 12);
        value = (uint32)(sint32)r_s16(target + 0x12C);
        z -= (uint32)((sint32)(value * 125) >> 9); z -= r_u32(object + 16);
        game_set_relative_target_angles(object, x, y, z);
        yaw = r_u32(0x80033D1C); pitch = r_u32(0x80033D30);
        w_u16(object + 0x250, yaw); w_u16(object + 0x252, pitch);
        break;
    case 6:
        w_u16(object + 0x250, 0); w_u16(object + 0x252, 64);
        break;
    case 7:
    case 8:
        w_u16(object + 0x250, turn_mode == 7 ? 64 : (uint16)-64); w_u16(object + 0x252, 0);
        break;
    default:
        break;
    }
    yaw = r_u32(0x80033D1C); value = r_u32(0x80046174);
    w_u16(object + 0x25C, (sint32)yaw >> 3);
    pitch = r_u32(0x80033D30); w_u32(object + 0x230, value); w_u16(object + 0x25E, (sint32)pitch >> 3);
    game_update_target_weapons(object);
    if (waypoint == 1) {
        choice = (uint32)random_between(1, 2);
        for (axis = 0; axis < 3; ++axis) {
            first = (uint32)(sint32)r_s16(object + 0x120 + axis * 6);
            second = (uint32)(sint32)r_s16(object + 0x11C + axis * 6);
            state = r_u32(object + 0x200);
            value = choice == 1 ? first + second : first - second;
            position = r_u32(object + 8 + axis * 4);
            w_u32(state + 0x18 + axis * 4, position + (value << 2));
        }
    }
    if (roll == 1) w_u32(object + 0x28, 32);
    else w_u16(object + 0x254, 0);
    if (boost == 1) {
        state = r_u32(object + 0x200);
        if (r_s32(state + 0xAC) < 90) w_u32(object + 0x28, 128);
        state = r_u32(object + 0x200); value = r_u32(state + 0xAC); w_u32(state + 0xAC, value + 1);
    } else w_u32(r_u32(object + 0x200) + 0xAC, 0);
    game_update_object_state(object);
    if (turn_mode == 9) w_u32(object + 0x24, 32);
    game_update_object_movement(object);
    target = r_u32(object + 0x178); value = r_u32(target);
    if (value == 0) w_u32(object + 0x40, 0x8009AB58);
    else {
        serial = r_u16(object + 0x24A); frame = r_u32(0x800311B8); value = serial + frame;
        quotient = (uint32)(((uint64)value * 0x88888889u) >> 32) >> 5;
        if (quotient * 60 == value - 3) w_u32(object + 0x40, 0x8009AB58);
    }
    threat = r_u32(object + 0x1A0);
    if (threat != 0 && r_u32(threat + 0x178) == object) {
        frame = r_u32(0x800311B8); kind = r_u32(object + 4);
        if (((frame + kind) & 15) < 8) game_try_launch_target_decoy(object);
    }
}
uint32 front_save_named_settings(uint32 name, uint32 port, uint32 data)
{
    uint8 header[512];
    char path[64];
    char *title;
    uint32 frames, blocks, length;
    FUNCTION_MARKER(0x8009DD98, "FRONT.OVL");
    frames = r_u8(0x800F2DC0);
    blocks = (uint32)((sint32)(frames + 0x21C) >> 13) + 1;
    length = 0;
    while (r_u8(name + length) != 0) ++length;
    title = (char *)psx_addr(name, (size_t)length + 1);
    front_build_save_header(header, title, blocks);
    sprintf(path, "bu%d0:%s%s", (sint32)port, "BASLUS-00543", title);
    if ((front_create_card_file(path, blocks) & 0xFF) == 0) return 0;
    return (front_write_card_file(header, path, (const uint8 *)psx_addr(data, 640)) & 0xFF) != 0;
}

static uint32 game_docking_camera_divide(uint32 numerator, sint32 denominator)
{
    if (denominator == 0)
        trap(7);
    if (denominator == -1 && numerator == 0x80000000u)
        trap(0x5D);
    return (uint32)((sint64)(sint32)numerator / denominator);
}

void game_update_docking_camera(uint32 object)
{
    uint32 view = r_u32(0x8003FE74);
    uint32 carrier, bay, stage, phase, value, next, x, y, z, dx, dy, dz;
    uint32 up_x, up_y, up_z, distance;
    sint32 length;
    VECTOR camera;
    uint32 camera_ready = 0;
    FUNCTION_MARKER(0x80086124, "GAME.OVL");
    up_x = (uint32)r_s16(view + 0x11E);
    up_y = (uint32)r_s16(view + 0x124);
    up_z = (uint32)r_s16(view + 0x12A);
    camera_set_up(0u - up_x, 0u - up_y, 0u - up_z);
    stage = r_u32(object + 0x88);
    carrier = r_u32(object + 0x170);
    bay = r_u32(object + 0xA8);
    if ((sint32)stage < 3) {
        game_steer_to_target_offset(object, carrier, bay + 0x24, 0);
        y = r_u32(0x80033D30);
        x = r_u32(0x80033D1C);
        w_u32(object + 0x24, y);
        stage = r_u32(object + 0x88);
        w_u32(object + 0x28, 0);
        w_u32(object + 0x20, x);
        if (stage == 1) {
            w_u32(object + 0xE8, 0xFA00);
            value = 0x100;
        } else {
            w_u32(object + 0xE8, 0x1C200);
            value = 0x200;
        }
        w_u32(object + 0xF0, value);
        w_u16(object + 0x260, 0x100);
    }
    game_update_object_state(object);
    game_update_object_movement(object);
    stage = r_u32(object + 0x88);
    switch (stage) {
    case 1:
        if (r_s32(0x80046174) < 30000) {
            w_u32(object + 0x88, 2);
            w_u16(bay + 0x78, 1);
            w_u16(bay + 0x76, 0);
            w_u16(bay + 0x7A, 10);
            w_u16(bay + 0x74, 0);
        }
        break;
    case 2:
        game_draw_carrier_launch_bay(r_u32(object + 0x170), bay);
        if (r_s32(0x80046174) < 15000) {
            w_u32(object + 0x88, 3);
            w_u32(object + 0x9C, 60);
        }
        break;
    case 3:
        game_draw_carrier_launch_bay(r_u32(object + 0x170), bay);
        value = r_u32(object + 0x9C) - 1;
        w_u32(object + 0x9C, value);
        if ((sint32)value <= 0) {
            w_u32(object + 0x9C, 30);
            w_u32(object + 0x88, 4);
            w_u16(bay + 0x76, 10);
            w_u16(bay + 0x78, 0xFFFF);
            w_u16(bay + 0x7A, 0);
            w_u32(object + 0x118, 0);
        }
        break;
    case 4:
        x = (uint32)r_s16(bay + 0x76);
        y = (uint32)r_s16(bay + 0x7A);
        if (x != y)
            game_draw_carrier_launch_bay(r_u32(object + 0x170), bay);
        value = r_u32(object + 0x9C) - 1;
        w_u32(object + 0x9C, value);
        if ((sint32)value <= 0) {
            phase = r_u32(0x800F32C8);
            w_u32(0x800ECEA8, 3);
            if (phase == 0)
                w_u32(0x800313DC, 3);
            if (phase == 1)
                w_u32(0x800313DC, stage);
        }
        break;
    }
    value = r_u32(object + 0xB0);
    phase = r_u32(object + 0xAC);
    w_u32(object + 0xB0, value - 1);
    if (phase == 1) {
        x = (uint32)r_s16(object + 0x224);
        y = (uint32)r_s16(object + 0x226);
        value = r_u32(object + 0x214);
        value += (uint32)((sint32)(x * 3u) >> 9);
        w_u32(object + 0x214, value);
        value = r_u32(object + 0x218);
        z = (uint32)r_s16(object + 0x228);
        value += (uint32)((sint32)(y * 3u) >> 9);
        w_u32(object + 0x218, value);
        value = r_u32(object + 0x21C);
        next = r_u32(object + 0xB0);
        value += (uint32)((sint32)(z * 3u) >> 9);
        w_u32(object + 0x21C, value);
        if ((sint32)next <= 0)
            w_u32(object + 0xAC, 2);
    } else if (phase == 2) {
        value = (uint32)r_s16(object + 0x120) << 1;
        x = r_u32(object + 8) - value;
        value = (uint32)r_s16(object + 0x126) << 1;
        y = r_u32(object + 12) - value;
        value = (uint32)r_s16(object + 0x12C) << 1;
        z = r_u32(object + 16) - value;
        value = (uint32)r_s16(object + 0x11E);
        x -= (uint32)((sint32)(value * 625u) >> 10);
        value = (uint32)r_s16(object + 0x124);
        y -= (uint32)((sint32)(value * 625u) >> 10);
        value = (uint32)r_s16(object + 0x12A);
        z -= (uint32)((sint32)(value * 625u) >> 10);
        dx = r_u32(object + 8) - r_u32(object + 0x214);
        dy = r_u32(object + 12) - r_u32(object + 0x218);
        dz = r_u32(object + 16) - r_u32(object + 0x21C);
        dx = x - dx; dy = y - dy; dz = z - dz;
        length = vector_length_approx((sint32)dx, (sint32)dy, (sint32)dz);
        w_u32(0x8011F130, (uint32)length);
        if (length == 0)
            w_u32(0x8011F130, 1);
        length = r_s32(0x8011F130);
        dx = game_docking_camera_divide(dx << 5, length);
        dy = game_docking_camera_divide(dy << 5, length);
        dz = game_docking_camera_divide(dz << 5, length);
        value = r_u32(object + 0x214);
        w_u32(object + 0x214, value - dx);
        value = r_u32(object + 0x218);
        w_u32(object + 0x218, value - dy);
        value = r_u32(object + 0x21C);
        distance = r_u32(0x80046174);
        w_u32(object + 0x21C, value - dz);
        if ((sint32)distance < 25000) {
            x = r_u32(object + 8) - r_u32(object + 0x214);
            w_u32(object + 0xB4, x);
            camera.vx = (sint32)x;
            y = r_u32(object + 12) - r_u32(object + 0x218);
            w_u32(object + 0xB8, y);
            camera.vy = (sint32)y;
            z = r_u32(object + 16) - r_u32(object + 0x21C);
            w_u32(object + 0xBC, z);
            camera.vz = (sint32)z;
            camera_ready = 1;
            x = r_u32(carrier + 8) - r_u32(object + 8);
            w_u32(object + 0x214, x);
            y = r_u32(carrier + 12) - r_u32(object + 12);
            x = r_u32(object + 0x214);
            w_u32(object + 0x218, y);
            z = r_u32(carrier + 16) - r_u32(object + 16);
            y = r_u32(object + 0x218);
            w_u32(object + 0x21C, z);
            length = vector_length_approx((sint32)x, (sint32)y, (sint32)z);
            dx = game_docking_camera_divide(r_u32(object + 0x214) << 4, length);
            dy = game_docking_camera_divide(r_u32(object + 0x218) << 4, length);
            dz = game_docking_camera_divide(r_u32(object + 0x21C) << 4, length);
            w_u16(object + 0x224, dx);
            w_u16(object + 0x226, dy);
            w_u16(object + 0x228, dz);
            value = (uint32)r_s16(carrier + 0x11C);
            next = r_u16(object + 0x224);
            w_u16(object + 0x224, next + (uint32)((sint32)(value * 5u) >> 10));
            value = (uint32)r_s16(carrier + 0x122);
            w_u32(0x8011F130, (uint32)length);
            next = r_u16(object + 0x226);
            w_u16(object + 0x226, next + (uint32)((sint32)(value * 5u) >> 10));
            value = (uint32)r_s16(carrier + 0x128);
            w_u32(object + 0xE8, 0x6400);
            w_u32(object + 0xF0, 0x400);
            w_u32(object + 0xAC, 3);
            next = r_u16(object + 0x228);
            w_u16(object + 0x228, next + (uint32)((sint32)(value * 5u) >> 10));
        }
    } else if (phase == 3) {
        x = (uint32)r_s16(object + 0x224);
        value = r_u32(object + 0xB4) - x;
        camera.vx = (sint32)value;
        y = (uint32)r_s16(object + 0x226);
        next = r_u32(object + 0xB8);
        w_u32(object + 0xB4, value);
        value = next - y;
        camera.vy = (sint32)value;
        z = (uint32)r_s16(object + 0x228);
        next = r_u32(object + 0xBC);
        w_u32(object + 0xB8, value);
        value = next - z;
        camera.vz = (sint32)value;
        camera_ready = 1;
        w_u32(object + 0xBC, value);
    }
    if (r_s32(object + 0xAC) < 3) {
        camera.vx = (sint32)(r_u32(object + 8) - r_u32(object + 0x214));
        camera.vy = (sint32)(r_u32(object + 12) - r_u32(object + 0x218));
        camera.vz = (sint32)(r_u32(object + 16) - r_u32(object + 0x21C));
        camera_ready = 1;
    }
    if (!camera_ready)
        abort();
    camera_set_position_native(&camera);
}
