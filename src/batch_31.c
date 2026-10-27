#include "game_functions.h"
#include <stdio.h>
#include <string.h>

uint32 game_script_condition_elapsed_time_unsigned(uint32 operand)
{
    uint32 clock, started, deadline;
    FUNCTION_MARKER(0x800ACB24, "GAME.OVL");
    clock = r_u32(0x800311B8);
    started = r_u32(0x800595B8);
    deadline = r_u32(0x80058E70 + operand * 4);
    return deadline < clock - started;
}

uint32 game_script_condition_primary_count_zero(void)
{
    uint32 count;
    FUNCTION_MARKER(0x800ACB54, "GAME.OVL");
    if (r_u32(0x800595B0) != 0) {
        w_u32(0x800595E0, 0);
        return 0;
    }
    count = r_u32(0x800595E0) + 1;
    w_u32(0x800595E0, count);
    return (sint32)count >= 4;
}

uint32 game_script_condition_secondary_count_zero(void)
{
    uint32 count;
    FUNCTION_MARKER(0x800ACB9C, "GAME.OVL");
    if (r_u32(0x800595B4) != 0) {
        w_u32(0x800595E4, 0);
        return 0;
    }
    count = r_u32(0x800595E4) + 1;
    w_u32(0x800595E4, count);
    return (sint32)count >= 4;
}

uint32 game_script_condition_special_state_one(uint32 operand)
{
    uint32 identifier, offset;
    FUNCTION_MARKER(0x800ACBE4, "GAME.OVL");
    identifier = r_u32(0x80058E70 + operand * 4);
    offset = (identifier - 1000) * 44;
    return r_s16(0x80059E8E + offset) == 1;
}

uint32 game_script_condition_destroyed_count(uint32 operand)
{
    uint32 identifier, side, category, threshold, count = 0, offset;
    sint32 destroyed;
    FUNCTION_MARKER(0x800ACC34, "GAME.OVL");
    offset = operand * 4;
    identifier = r_u32(0x80058E78 + offset);
    side = r_u32(0x80058E70 + offset);
    category = r_u32(0x80058E74 + offset);
    threshold = r_u32(0x80058E7C + offset);
    if ((sint32)identifier >= 1000)
        return r_s16(0x80059E90 + (identifier - 1000) * 44) != 0;
    if (identifier != 0) {
        count = (uint32)r_s16(0x800536BE + identifier * 60);
    } else {
        for (offset = 0; offset < 1200; offset += 60) {
            if (r_u32(0x80053698 + offset) != category)
                continue;
            destroyed = r_s16(0x800536BE + offset);
            if (destroyed == 0)
                continue;
            if (side == 3 || (side == 2 && r_s16(0x800536BC + offset) == 2))
                count += (uint32)destroyed;
            else if (side == 1 && r_s16(0x800536BC + offset) == 1)
                count += (uint32)r_s16(0x800536BE + offset);
        }
    }
    return (sint32)count >= (sint32)threshold;
}

uint32 game_script_condition_secondary_event_count(uint32 operand)
{
    uint32 identifier, side, category, threshold, count = 0, offset;
    sint32 destroyed;
    FUNCTION_MARKER(0x800ACDB0, "GAME.OVL");
    offset = operand * 4;
    identifier = r_u32(0x80058E78 + offset);
    side = r_u32(0x80058E70 + offset);
    category = r_u32(0x80058E74 + offset);
    threshold = r_u32(0x80058E7C + offset);
    if ((sint32)identifier >= 1000)
        return r_s16(0x80059E92 + (identifier - 1000) * 44) != 0;
    if (identifier != 0) {
        count = (uint32)r_s16(0x800536C0 + identifier * 60);
    } else {
        for (offset = 0; offset < 1200; offset += 60) {
            if (r_u32(0x80053698 + offset) != category)
                continue;
            destroyed = r_s16(0x800536C0 + offset);
            if (destroyed == 0)
                continue;
            if (side == 3 || (side == 2 && r_s16(0x800536BC + offset) == 2))
                count += (uint32)destroyed;
            else if (side == 1 && r_s16(0x800536BC + offset) == 1)
                count += (uint32)r_s16(0x800536C0 + offset);
        }
    }
    return (sint32)count >= (sint32)threshold;
}

uint32 game_script_condition_mode_five(void)
{
    FUNCTION_MARKER(0x800ACFF0, "GAME.OVL");
    if (r_u32(0x800F18B8) == 0)
        return 0;
    w_u16(0x80059610, 5);
    w_u32(0x800ECEA8, 3);
    return 1;
}

uint32 game_script_condition_mode_six(void)
{
    FUNCTION_MARKER(0x800AD02C, "GAME.OVL");
    if (r_u32(0x800F18D8) != 0)
        return 0;
    w_u16(0x80059610, 6);
    return 1;
}

uint32 game_script_condition_disabled_count(uint32 operand)
{
    uint32 identifier, category, threshold, count = 0, object, flags, offset;
    FUNCTION_MARKER(0x800AD058, "GAME.OVL");
    offset = operand * 4;
    identifier = r_u32(0x80058E78 + offset);
    category = r_u32(0x80058E74 + offset);
    threshold = r_u32(0x80058E7C + offset);
    if (identifier == 0) {
        if (category != 3 && category != 1)
            diagnostic_report(0x29A, 0x80075DE4, 0x80075DFC);
        object = object_lookup(category);
        while (object != 0) {
            flags = r_u32(object + 0x114);
            if ((flags & 0x40000010) == 0x40000010)
                ++count;
            object = r_u32(object + 0x1DC);
        }
    } else if ((sint32)identifier >= 1000) {
        object = r_u32(0x80059E68 + (identifier - 1000) * 44);
        if (object != 0 && (r_u32(object + 0x114) & 0x40000010) == 0x40000010)
            count = 1;
    } else {
        category = r_u32(0x80053698 + identifier * 60);
        object = object_lookup(category);
        while (object != 0) {
            flags = r_u32(object + 0x114);
            if ((flags & 0x40000010) == 0x40000010)
                ++count;
            object = r_u32(object + 0x1DC);
        }
    }
    return (sint32)count >= (sint32)threshold;
}

sint32 game_script_condition_status(void)
{
    FUNCTION_MARKER(0x800AD200, "GAME.OVL");
    return r_s16(0x800595F4);
}

uint32 game_script_condition_no_enabled_objects(void)
{
    uint32 object, count = 0;
    FUNCTION_MARKER(0x800AD210, "GAME.OVL");
    object = object_lookup(3);
    while (object != 0) {
        if ((r_u32(object + 0x114) & 0x10) == 0)
            ++count;
        object = r_u32(object + 0x1DC);
    }
    return count == 0;
}

static const char *game_script_text_native(uint32 address)
{
    uint32 length = 0;
    while (r_u8(address + length) != 0)
        ++length;
    return (const char *)psx_addr(address, (size_t)length + 1);
}

uint32 game_script_condition_rings_complete(uint32 operand)
{
    uint32 index, format;
    FUNCTION_MARKER(0x800ACF2C, "GAME.OVL");
    if (r_s32(0x800F18B4) > 0)
        return 1;
    index = r_u32(0x800F18A0);
    index = r_u32(0x80058E6C + (operand + index) * 4);
    if (r_u32(0x80124934 + (index - 1) * 12) == 0)
        return 0;
    format = r_u32(0x800F2404);
    sprintfPSX(0x800FA368, format, 0);
    game_request_radio_message(0x149, 0x800FA368);
    w_u32(0x800F18B4, 1);
    return 1;
}

void game_begin_mission_completion(uint32 player)
{
    uint32 mode = r_u32(0x800EC884);
    uint32 clock, selector, offset, stage, mission, index, selected, name;
    uint32 duration, view, model, flags, completion;
    FUNCTION_MARKER(0x800AD528, "GAME.OVL");
    if (mode != 0)
        return;
    clock = r_u32(0x800311B8);
    w_u16(0x8005961C, 0);
    w_u32(0x800EEF14, clock);
    game_remove_objects_by_type(104);
    if (r_u32(0x800F18AC) == 0) {
        selector = (uint32)r_s16(0x80059610);
        stage = (uint32)r_s16(0x800595FA);
        mission = (uint32)r_s16(0x800595F8);
        offset = (stage - 1) * 3 + mission * 21;
        selector = r_u8((selector == 2 ? 0x800EEF78 : 0x800EEF79) + offset);
        render_set_reduced_height(512);
        game_refresh_mission_state();
        if (selector == 0) {
            stage = (uint32)r_s16(0x800595FA);
            mission = (uint32)r_s16(0x800595F8);
            offset = (stage - 1) * 3 + mission * 21;
            selected = r_u8(0x800EEF7A + offset);
            w_u32(0x8011F130, selected);
            for (index = 0; index < 20; ++index) {
                selected = r_u32(0x8011F130);
                name = r_u32(0x800EEF58 + selected * 4);
                if (strcmp(game_script_text_native(name), game_script_text_native(0x800536C6 + index * 60)) == 0) {
                    w_u32(0x8011F130, index);
                    break;
                }
            }
            if (index >= 20)
                diagnostic_report(r_u32(0x8011F130), 0x80075E5C, 0x80075DFC);
            selected = r_u32(0x8011F130);
            offset = selected * 60;
            if (r_u32(0x80053698 + offset) == 1 &&
                r_s16(0x800536BC + offset) == 2 &&
                r_s16(0x800536BE + offset) == 0)
                game_begin_docking_completion(selected);
            else
                game_begin_mission_completion_sequence_one();
        } else if (selector == 1) {
            game_begin_mission_completion_sequence_one();
        } else if (selector == 2) {
            game_begin_mission_completion_sequence_two();
        } else {
            (void)r_u32(0x80040FDC);
            game_fail_current_mission();
        }
    } else {
        duration = r_u32(player + 0x9C);
        view = r_u32(0x80046178);
        w_u32(player + 0x40, 0x800AD890);
        w_u32(player, 666);
        w_u32(player + 0xA0, 0);
        w_u32(player + 0x74, 0);
        model = r_u32(view);
        w_u32(player + 0x70, 10);
        w_u32(player + 0x3C, 0x8002D2BC);
        w_u32(player + 0x64, model);
        game_create_temporary_camera(player, duration);
        w_u32(player + 0x9C, 30);
    }
    completion = r_u32(0x800595FC);
    flags = r_u32(0x800595F8);
    w_u32(player + 0xA8, completion == flags ? 0 : 1);
}

void game_update_mission_completion_delay(uint32 object)
{
    uint32 remaining;
    FUNCTION_MARKER(0x800AD890, "GAME.OVL");
    remaining = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, remaining);
    if ((sint32)remaining < 0)
        w_u32(0x800ECEA8, 3);
}

void game_begin_mission_completion_sequence_one(void)
{
    uint32 player, view, value, flags, identifier, speed, mode, x, y, z, sample, text;
    FUNCTION_MARKER(0x8007BDB8, "GAME.OVL");
    render_set_reduced_height(512);
    game_refresh_mission_state();
    game_copy_player_view_transform();
    game_remove_objects_by_type(3);
    player = r_u32(0x80040FDC);
    game_remove_other_objects_of_type(1, player);
    player = r_u32(0x80040FDC);
    view = r_u32(0x8003FE74);
    value = r_u32(player + 8);
    w_u32(0x800FB410, value);
    value = r_u32(player + 12);
    w_u32(0x800FB414, value);
    value = r_u32(player + 16);
    w_u32(0x800FB418, value);
    w_u32(view + 0x9C, 30);
    w_u32(view + 0x40, 0x800A7864);
    flags = r_u32(view + 0x114);
    identifier = r_u32(view + 4);
    w_u32(view + 0x88, 0);
    w_u32(view + 0x114, flags | 0x200000);
    speed = r_u32(0x8005369C + identifier * 60);
    mode = r_u32(0x800313DC);
    w_u32(0x800EC884, 3);
    w_u32(view + 0xAC, 0x7FFFFFF);
    w_u32(view + 0xE8, speed);
    w_u32(0x800F32C8, (sint32)mode >= 4 ? 1 : 0);
    w_u32(0x800313DC, 5);
    w_u32(0x800F32C8, 1);
    w_u32(0x80108A40, 450);
    camera_reset_follow();
    camera_set_look_at_object(0x8010AB58);
    view = r_u32(0x8003FE74);
    value = r_u32(view + 8);
    w_u32(0x8010AB58, value);
    value = r_u32(view + 12);
    w_u32(0x8010AB5C, value);
    value = r_u32(view + 16);
    w_u32(0x8010AB60, value);
    value = (uint32)r_s16(view + 0x11C);
    x = r_u32(view + 8) + (value << 2);
    w_u32(0x80118A70, x);
    value = (uint32)r_s16(view + 0x122);
    y = r_u32(view + 12) + (value << 2);
    w_u32(0x80118A74, y);
    value = (uint32)r_s16(view + 0x128);
    z = r_u32(view + 16) + (value << 2);
    w_u32(0x80118A78, z);
    x += (uint32)r_s16(view + 0x120) << 2;
    w_u32(0x80118A70, x);
    y += (uint32)r_s16(view + 0x126) << 2;
    w_u32(0x80118A74, y);
    z += (uint32)r_s16(view + 0x12C) << 2;
    w_u32(0x80118A78, z);
    x += (uint32)r_s16(view + 0x120) << 1;
    w_u32(0x80118A70, x);
    y += (uint32)r_s16(view + 0x126) << 1;
    w_u32(0x80118A74, y);
    z += (uint32)r_s16(view + 0x12C) << 1;
    w_u32(0x80118A78, z);
    camera_set_position(0x80118A70);
    view = r_u32(0x8003FE74);
    camera_set_target(view);
    view = r_u32(0x8003FE74);
    value = (uint32)r_s16(view + 0x120);
    w_u32(0x8010AB70, value);
    value = (uint32)r_s16(view + 0x126);
    w_u32(0x8010AB74, value);
    value = (uint32)r_s16(view + 0x12C);
    w_u32(0x800EC87C, 1);
    w_u32(0x800EC880, 1);
    w_u32(0x8010AB78, value);
    value = (uint32)random_between(0, 2);
    w_u32(0x8011F130, value);
    if (value == 0) {
        sample = 0x53;
        text = r_u32(0x800F23E4);
    } else if (value == 1) {
        text = r_u32(0x800F23E8);
        sample = 0x56;
    } else {
        sample = 0x57;
        text = r_u32(0x800F23F4);
    }
    game_request_radio_message(sample, text);
}

void game_begin_mission_completion_sequence_two(void)
{
    uint32 player, view, value, mode, x, y, z, text;
    VECTOR up;
    FUNCTION_MARKER(0x8007C0CC, "GAME.OVL");
    render_set_reduced_height(512);
    game_refresh_mission_state();
    game_copy_player_view_transform();
    game_remove_objects_by_type(3);
    player = r_u32(0x80040FDC);
    game_remove_other_objects_of_type(1, player);
    player = r_u32(0x80040FDC);
    value = r_u32(player + 8);
    w_u32(0x800FB410, value);
    value = r_u32(player + 12);
    w_u32(0x800FB414, value);
    value = r_u32(player + 16);
    w_u32(0x800FB418, value);
    up.vx = r_s16(player + 0x122);
    up.vy = r_s16(player + 0x124);
    up.vz = r_s16(player + 0x126);
    w_u32(0x800EC884, 1);
    vector_normalize((VECTOR *)psx_addr(0x800ED3AC, 12), (VECTOR *)psx_addr(0x8010AB70, 12));
    mode = r_u32(0x800313DC);
    w_u32(0x800F32C8, (sint32)mode >= 4 ? 1 : 0);
    view = r_u32(0x8003FE74);
    w_u32(0x80126A98, 1024);
    w_u32(0x800F32C8, 1);
    w_u32(0x800313DC, 5);
    object_basis_from_vectors((VECTOR *)psx_addr(0x8010AB70, 12), &up, view);
    camera_reset_follow();
    view = r_u32(0x8003FE74);
    camera_set_target(view);
    view = r_u32(0x8003FE74);
    value = (uint32)r_s16(view + 0x11C);
    x = r_u32(view + 8) + value;
    w_u32(0x80118A70, x);
    value = (uint32)r_s16(view + 0x122);
    y = r_u32(view + 12) + value;
    w_u32(0x80118A74, y);
    value = (uint32)r_s16(view + 0x128);
    z = r_u32(view + 16) + value;
    w_u32(0x80118A78, z);
    x += (uint32)r_s16(view + 0x120) << 1;
    w_u32(0x80118A70, x);
    y += (uint32)r_s16(view + 0x126) << 1;
    w_u32(0x80118A74, y);
    z += (uint32)r_s16(view + 0x12C) << 1;
    w_u32(0x80118A78, z);
    camera_set_position(0x80118A70);
    text = r_u32(0x800F23F8);
    w_u32(0x800EC87C, 1);
    w_u32(0x80108A40, 120);
    game_request_radio_message(0x20, text);
}

uint32 game_create_docking_sequence_object(uint32 owner, uint32 side, uint32 up, uint32 forward, uint32 yaw)
{
    uint32 index = r_u32(0x800ECBD0);
    uint32 model, x, y, z, object, flags, sx, sy, sz, ux, uy, uz, fx, fy, fz, value;
    FUNCTION_MARKER(0x800871EC, "GAME.OVL");
    model = (uint32)r_s16(0x800ECBEC + index * 44);
    x = r_u32(owner + 8);
    y = r_u32(owner + 12);
    z = r_u32(owner + 16);
    object = game_create_object(model, x, y, z, 0);
    w_u32(object + 0x84, 0xFFFFFFFF);
    w_u32(object + 4, 123);
    w_u32(object + 0x40, 0x80087438);
    w_u32(object + 0x3C, 0x8002D2BC);
    flags = r_u32(object + 0x70);
    w_u32(object + 0xD8, 0x895440);
    w_u32(object + 0x28, 0);
    w_u32(object + 0x24, 0);
    w_u32(object + 0x20, 0);
    w_u32(object + 0x68, 0);
    w_u32(object + 0xD4, 0x808080);
    w_u32(object + 0x70, flags | 0x100000);
    matrix_copy_rotation(object + 0x11C, owner + 0x11C);
    w_u32(object + 0x20, yaw);
    w_u32(object + 0x28, 0);
    w_u32(object + 0x24, 0);
    game_rotate_object_basis(object);
    sx = (uint32)r_s16(object + 0x11C) * side;
    sy = (uint32)r_s16(object + 0x122) * side;
    sz = (uint32)r_s16(object + 0x128) * side;
    ux = (uint32)r_s16(object + 0x11E) * up;
    uy = (uint32)r_s16(object + 0x124) * up;
    uz = (uint32)r_s16(object + 0x12A) * up;
    value = (uint32)r_s16(object + 0x120);
    x = r_u32(object + 8);
    fx = value * forward;
    w_u32(object + 8, x + (uint32)((sint32)sx >> 12));
    y = r_u32(object + 12);
    value = (uint32)r_s16(object + 0x126);
    fy = value * forward;
    w_u32(object + 12, y + (uint32)((sint32)sy >> 12));
    z = r_u32(object + 16);
    w_u32(object + 16, z + (uint32)((sint32)sz >> 12));
    x = r_u32(object + 8);
    w_u32(object + 8, x + (uint32)((sint32)ux >> 12));
    y = r_u32(object + 12);
    value = (uint32)r_s16(object + 0x12C);
    fz = value * forward;
    w_u32(object + 0x28, 0);
    w_u32(object + 0x24, 0);
    w_u32(object + 0x20, 0);
    w_u32(object + 12, y + (uint32)((sint32)uy >> 12));
    z = r_u32(object + 16);
    w_u32(object + 16, z + (uint32)((sint32)uz >> 12));
    x = r_u32(object + 8);
    w_u32(object + 8, x + (uint32)((sint32)fx >> 12));
    y = r_u32(object + 12);
    w_u32(object + 12, y + (uint32)((sint32)fy >> 12));
    z = r_u32(object + 16);
    w_u32(object + 16, z + (uint32)((sint32)fz >> 12));
    game_set_acceleration_from_basis(object, 1200);
    return object;
}

void game_update_docking_sequence_object(uint32 object)
{
    FUNCTION_MARKER(0x80087438, "GAME.OVL");
    game_update_object_movement(object);
}

void game_begin_docking_completion(uint32 identifier)
{
    uint32 x, y, z, carrier, player, callback, bay, view, value, flags, axis, offset;
    uint32 count, index, target, model, effect, product, random, yaw, pitch, next, text;
    sint32 length, numerator, normals[3], route;
    uint32 difference[3];
    VECTOR rotated, camera;
    FUNCTION_MARKER(0x800868A0, "GAME.OVL");
    x = r_u32(0x80074634);
    y = r_u32(0x80074638);
    z = r_u32(0x8007463C);
    (void)r_u32(0x80074640);
    game_remove_objects_by_type(3);
    player = r_u32(0x80040FDC);
    game_remove_other_objects_of_type(1, player);
    carrier = game_spawn_script_ship(identifier, x, y, z, 0, 0, 0);
    callback = game_find_ship_start_callback(identifier);
    psx_dispatch_guest_call(callback, 1, carrier, 0, 0);
    player = r_u32(0x80040FDC);
    w_u32(carrier + 0x5C, 0x800D3478);
    w_u32(carrier + 0x48, 0x800D3478);
    w_u32(carrier + 0x3C, 0x800D1C3C);
    w_u32(carrier + 0x60, 0x8002493C);
    w_u32(carrier + 0x58, 0x8002493C);
    w_u32(carrier + 0x40, 0x8002493C);
    w_u32(carrier + 0xF4, 0);
    w_u32(carrier + 0x234, 1000);
    w_u32(carrier + 0x238, 1000);
    bay = game_find_free_carrier_bay_near_target(carrier, player);
    if (bay == 0)
        diagnostic_report(666, 0x80074644, 0x8007465C);
    view = r_u32(0x8003FE74);
    w_u32(view + 4, 1);
    w_u32(view + 0x40, 0x8008611C);
    w_u32(view + 0x3C, 0x8002D2BC);
    flags = r_u32(view + 0x70);
    w_u32(view + 0x70, flags | 0x100000);
    flags = r_u32(view + 0x114);
    w_u32(view + 0x114, flags | 0x200000);
    matrix_set_identity(view + 0x11C);
    w_u32(view + 0xA8, bay);
    w_u32(view + 0x170, carrier);
    w_u32(view + 0xEC, 0);
    w_u16(view + 0x24C, 1);
    game_position_ship_at_carrier_offset(carrier, bay + 0x24, view);
    ApplyMatrixLV((MATRIX *)psx_addr(carrier + 0x11C, 20), (VECTOR *)psx_addr(bay + 0x54, 12), &rotated);
    product = (uint32)rotated.vx * 19000;
    value = r_u32(view + 8);
    w_u32(view + 8, value + (uint32)((sint32)product >> 10));
    product = (uint32)rotated.vy * 19000;
    value = r_u32(view + 12);
    w_u32(view + 12, value + (uint32)((sint32)product >> 10));
    product = (uint32)rotated.vz * 19000;
    value = r_u32(view + 16);
    w_u32(view + 16, value + (uint32)((sint32)product >> 10));
    for (axis = 0; axis < 3; ++axis) {
        x = r_u32(carrier + 8 + axis * 4);
        y = r_u32(view + 8 + axis * 4);
        difference[axis] = x - y;
    }
    length = vector_length_approx((sint32)difference[0], (sint32)difference[1], (sint32)difference[2]);
    for (axis = 0; axis < 3; ++axis) {
        numerator = (sint32)(difference[axis] << 12);
        if (length == 0)
            trap(7);
        if (length == -1 && numerator == (sint32)0x80000000)
            trap(0x5D);
        normals[axis] = (sint32)((sint64)numerator / length);
    }
    w_u32(0x80046174, (uint32)length);
    light_set_color(1, 128, 128, 128);
    light_set_direction(1, (uint32)normals[0], (uint32)normals[1], (uint32)normals[2]);
    count = r_u32(0x800595C0);
    if (count != 0) {
        w_u32(0x8011F130, 0);
        if ((sint32)count > 0) {
            do {
                index = r_u32(0x8011F130);
                target = r_u32(0x801076F8 + index * 36);
                callback = r_u32(target + 0x48);
                psx_dispatch_guest_call(callback, 1, target, 0, 0);
                index = r_u32(0x8011F130);
                count = r_u32(0x800595C0);
                ++index;
                w_u32(0x8011F130, index);
            } while ((sint32)index < (sint32)count);
        }
        model = r_u32(0x801076E8);
        x = r_u32(carrier + 8);
        y = r_u32(carrier + 12);
        z = r_u32(carrier + 16);
        effect = game_create_object(model, x, y, z, 0);
        w_u32(effect + 0x3C, 0x800C682C);
        w_u32(effect + 0xD4, 0x808080);
        w_u32(effect + 0x6C, 0);
        w_u32(effect + 0xA4, 0x1140);
        random = (uint32)random_between(13, 15);
        w_u32(effect + 0x84, random);
        for (axis = 0; axis < 3; ++axis) {
            product = (uint32)r_s16(view + 0x120 + axis * 6) * 75000;
            value = r_u32(effect + 8 + axis * 4);
            w_u32(effect + 8 + axis * 4, value - product);
        }
        random = (uint32)random_signed_between(10000, 30000);
        for (axis = 0; axis < 3; ++axis) {
            product = (uint32)r_s16(view + 0x11E + axis * 6) * random;
            if (axis == 2)
                w_u32(0x8011F130, random);
            value = r_u32(effect + 8 + axis * 4);
            w_u32(effect + 8 + axis * 4, value - product);
        }
        random = (uint32)random_signed_between(20000, 50000);
        for (axis = 0; axis < 3; ++axis) {
            product = (uint32)r_s16(view + 0x11C + axis * 6) * random;
            if (axis == 2) {
                w_u32(effect + 0xBC, 0);
                w_u32(effect + 0xB0, 0);
            }
            value = r_u32(effect + 8 + axis * 4);
            w_u32(effect + 8 + axis * 4, value - product);
        }
        route = r_s16(0x80107706);
        w_u32(0x8011F130, random);
        if (route != 0) {
            w_u32(effect + 0xB0, 1);
            route = r_s16(0x80107706);
            if (route >= 18)
                route = 17;
            offset = ((uint32)route - 1) * 12;
            x = r_u32(0x800EEE24 + offset);
            y = r_u32(0x800EEE28 + offset);
            z = r_u32(0x800EEE2C + offset);
            value = game_create_gradient_strip_set(x, y, z);
            w_u32(effect + 0xBC, value);
        }
    }
    w_u32(view + 0x88, 1);
    w_u32(view + 0xAC, 1);
    w_u32(view + 0x9C, 0);
    w_u32(view + 0xB0, 90);
    w_u32(0x8011F130, 0);
    do {
        game_steer_to_target_offset(view, carrier, bay + 0x24, 0);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u32(view + 0x28, 0);
        w_u32(view + 0x20, yaw);
        w_u32(view + 0x24, pitch);
        game_rotate_object_basis(view);
        index = r_u32(0x8011F130);
        w_u32(view + 0x28, 0);
        w_u32(view + 0x24, 0);
        w_u32(view + 0x20, 0);
        ++index;
        w_u32(0x8011F130, index);
    } while ((sint32)index < 2);
    x = r_u16(view + 0x11C);
    z = r_u16(view + 0x128);
    w_u16(view + 0x224, (uint16)x);
    y = r_u16(view + 0x122);
    value = (uint32)r_s16(view + 0x224);
    w_u16(view + 0x228, (uint16)z);
    w_u16(view + 0x226, (uint16)y);
    product = value * 375;
    next = (uint32)r_s16(view + 0x226);
    w_u32(view + 0x214, (uint32)((sint32)product >> 9));
    product = next * 375;
    next = (uint32)r_s16(view + 0x228);
    w_u32(view + 0x218, (uint32)((sint32)product >> 9));
    product = next * 375;
    w_u32(view + 0x21C, (uint32)((sint32)product >> 9));
    random = (uint32)random_between(0, 100);
    w_u32(0x8011F130, random);
    if ((sint32)random < 35) {
        game_create_docking_sequence_object(view, -47000, 7000, -200000, 1024);
        game_create_docking_sequence_object(view, -43000, 9000, -240000, 1024);
        game_create_docking_sequence_object(view, -51000, 9000, -240000, 1024);
    } else if ((sint32)random < 70) {
        game_create_docking_sequence_object(view, -27000, 7000, -240000, 3072);
        game_create_docking_sequence_object(view, -23000, 9500, -300000, 3072);
        game_create_docking_sequence_object(view, -31000, 9500, -300000, 3072);
    } else {
        game_create_docking_sequence_object(view, -47000, 12000, -150000, 1024);
        game_create_docking_sequence_object(view, -43000, 12000, -200000, 1024);
        game_create_docking_sequence_object(view, -51000, 12000, -250000, 1024);
        game_create_docking_sequence_object(view, -51000, 12000, -300000, 1024);
    }
    w_u32(0x800EC884, 2);
    w_u32(0x800F32C8, 1);
    w_u32(0x800313DC, 5);
    camera_reset_follow();
    value = r_u32(0x8003FE74);
    camera_set_target(value);
    x = r_u32(view + 8);
    y = r_u32(view + 0x214);
    camera.vx = (sint32)(x - y);
    x = r_u32(view + 12);
    y = r_u32(view + 0x218);
    camera.vy = (sint32)(x - y);
    x = r_u32(view + 16);
    y = r_u32(view + 0x21C);
    camera.vz = (sint32)(x - y);
    camera_set_position_native(&camera);
    camera_dispatch();
    random = (uint32)random_between(0, 1);
    if (random == 0) {
        text = r_u32(0x800F23EC);
        game_request_radio_message(0x4D, text);
    } else {
        text = r_u32(0x800F23F0);
        game_request_radio_message(0x4E, text);
    }
}

uint32 game_request_stream_preset_one(void)
{
    uint32 sectors, asset;
    FUNCTION_MARKER(0x80087808, "GAME.OVL");
    sectors = r_u32(0x800105AC);
    asset = r_u32(0x800105B4);
    return stream_request_position(0x800105A8, sectors, asset);
}

uint32 game_request_stream_preset_two(void)
{
    uint32 buffer = r_u32(0x80041000);
    uint32 sectors, asset, word, destination, shift, mask, result;
    FUNCTION_MARKER(0x80087848, "GAME.OVL");
    sectors = r_u32(0x800105C4);
    asset = r_u32(0x800105CC);
    w_u32(buffer, 0);
    buffer = r_u32(0x80041000);
    w_u32(buffer + 4, 0);
    buffer = r_u32(0x80041000);
    (void)r_u32(0x800105C0);
    word = r_u32(0x800105C0);
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
    result = (uint32)system_cd_position_to_sector((CdlLOC *)psx_addr(0x800105C0, 3));
    buffer = r_u32(0x80041000);
    w_u32(buffer + 0x1C, result);
    buffer = r_u32(0x80041000);
    w_u32(buffer + 0x0C, sectors);
    buffer = r_u32(0x80041000);
    w_u32(buffer + 0x14, 0x800118FE);
    buffer = r_u32(0x80041000);
    w_u32(buffer + 0x18, 75);
    buffer = r_u32(0x80041000);
    w_u32(buffer + 0x20, asset);
    buffer = r_u32(0x80041000);
    w_u32(buffer + 0x24, 0);
    buffer = r_u32(0x80041000);
    w_u32(buffer + 0x28, 0);
    buffer = r_u32(0x80041000);
    w_u8(buffer + 0x30, 0);
    buffer = r_u32(0x80041000);
    w_u32(buffer + 0x2C, 0);
    buffer = r_u32(0x80041000);
    w_u32(0x80041EE8, 0);
    w_u32(buffer, 3);
    result = r_u32(0x80040FF8);
    if (result != 0) {
        result = r_u32(result);
        if (result != 0)
            return result;
    }
    w_u32(0x80040FE4, 4);
    return 4;
}

uint32 game_request_stream_preset_three(void)
{
    uint32 sectors, asset;
    FUNCTION_MARKER(0x800879A0, "GAME.OVL");
    disc_wait_stream();
    sectors = r_u32(0x800105DC);
    asset = r_u32(0x800105E4);
    stream_request_segments(0x800105D8, sectors, asset, 0x800118FE, 1, 0, NULL);
    return r_u32(0x800105E4);
}

void game_adjust_display_position(void)
{
    DISPENV environment;
    uint32 flags, pressed, previous;
    FUNCTION_MARKER(0x80087E3C, "GAME.OVL");
    SetDefDispEnv(&environment, 0, 0, 640, 512);
    flags = r_u32(0x80053684);
    environment.isinter = 1;
    while ((flags & 8) == 0) {
        if ((flags & 0x8000) != 0)
            environment.disp.x = (sint16)((uint16)environment.disp.x - 4);
        if ((flags & 0x2000) != 0)
            environment.disp.x = (sint16)((uint16)environment.disp.x + 4);
        if ((flags & 0x1000) != 0)
            environment.disp.y = (sint16)((uint16)environment.disp.y - 1);
        if ((flags & 0x4000) != 0)
            environment.disp.y = (sint16)((uint16)environment.disp.y + 1);
        VSync(0);
        PutDispEnv(&environment);
        pressed = (uint32)pad_update(0);
        previous = r_u32(0x80053678);
        w_u32(0x8004616C, pressed);
        flags = (pressed ^ previous) & pressed;
        w_u32(0x80053684, flags);
    }
}

void game_reserved_leaf(void)
{
    FUNCTION_MARKER(0x80087F44, "GAME.OVL");
    /* Original body returns without an operation */
}

void game_restore_pause_setting_values(void)
{
    uint32 offset, address, value;
    FUNCTION_MARKER(0x8008A928, "GAME.OVL");
    stream_stop();
    stream_cancel_current();
    if (r_u32(0x800FA26C) != 1) {
        for (offset = 0; offset < 24; offset += 8) {
            address = r_u32(0x800ECEE4 + offset);
            value = r_u16(0x800ECEE8 + offset);
            w_u16(address, (uint16)value);
        }
    }
}

void game_draw_pause_settings(void)
{
    uint32 index, menu, selected, y = 76;
    FUNCTION_MARKER(0x8008AD20, "GAME.OVL");
    game_draw_pause_grid_background();
    game_draw_pause_options();
    game_draw_pause_panel();
    game_draw_pause_heading();
    for (index = 0; index < 3; ++index) {
        menu = r_u32(0x80124C00);
        selected = r_u32(menu);
        game_draw_pause_setting_row(index, 57, y, selected == index ? 1 : 0);
        y += 28;
    }
    game_draw_pause_warp_grid();
}

void game_leave_pause_settings(void)
{
    uint32 menu, record, flags, a, b, c, d, e, f;
    FUNCTION_MARKER(0x8008AEF8, "GAME.OVL");
    menu = r_u32(0x80124C00);
    w_u32(0x800FB3BC, 0);
    w_u32(menu + 0x10, 4);
    record = r_u32(menu + 0x1C);
    flags = r_u32(0x80108A44);
    a = r_u32(record);
    b = r_u32(record + 4);
    c = r_u32(record + 8);
    d = r_u32(record + 12);
    e = r_u32(record);
    f = r_u32(record + 4);
    w_u32(0x80108A44, flags | 0x10);
    w_u32(record + 0x18, a);
    w_u32(record + 0x1C, b);
    w_u32(record + 0x20, c);
    w_u32(record + 0x24, d);
    w_u32(record + 0x10, e);
    w_u32(record + 0x14, f);
}

void game_accept_pause_settings(void)
{
    FUNCTION_MARKER(0x8008AF5C, "GAME.OVL");
    game_leave_pause_settings();
}

void game_snapshot_pause_settings(void)
{
    uint32 offset, address, value;
    FUNCTION_MARKER(0x8008AF7C, "GAME.OVL");
    game_request_stream_preset_one();
    game_leave_pause_settings();
    for (offset = 0; offset < 24; offset += 8) {
        address = r_u32(0x800ECEE4 + offset);
        value = r_u16(address);
        w_u16(0x800ECEE8 + offset, (uint16)value);
    }
}

void game_update_pause_settings(void)
{
    uint32 menu = r_u32(0x80124C00);
    uint32 previous, index, controls, pressed, mask, address, count, flags, record, callback, value;
    uint32 changed = 0, result;
    sint32 limit, step, current;
    FUNCTION_MARKER(0x8008A9A0, "GAME.OVL");
    previous = r_u32(menu);
    game_update_pause_selection();
    menu = r_u32(0x80124C00);
    index = r_u32(menu);
    if (previous != index) {
        stream_stop();
        stream_cancel_current();
        menu = r_u32(0x80124C00);
        index = r_u32(menu);
        if (index == 0)
            game_request_stream_preset_one();
        else if (index == 2)
            game_request_stream_preset_two();
        w_u32(0x800ED384, 0);
    }
    menu = r_u32(0x80124C00);
    controls = r_u32(0x800ED42C);
    index = r_u32(menu);
    pressed = r_u32(0x8004616C);
    mask = r_u32(controls + 4);
    address = r_u32(0x800ECEE4 + index * 8);
    if ((pressed & mask) != 0) {
        limit = r_s16(0x800ECEEA + index * 8);
        step = limit / 255;
        current = r_s16(address);
        result = (uint32)(current + step);
        if (limit < (sint32)result) {
            w_u16(address, (uint16)limit);
        } else {
            w_u16(address, (uint16)result);
            count = r_u32(0x800ED384);
            changed = 1;
            w_u32(0x800ED384, count + 1);
        }
    }
    controls = r_u32(0x800ED42C);
    pressed = r_u32(0x8004616C);
    mask = r_u32(controls);
    if ((pressed & mask) != 0) {
        menu = r_u32(0x80124C00);
        index = r_u32(menu);
        limit = r_s16(0x800ECEEA + index * 8);
        current = r_u16(address);
        step = limit / 255;
        result = (uint32)current - (uint32)step;
        w_u16(address, (uint16)result);
        if ((sint16)result < 0) {
            w_u16(address, 0);
        } else {
            count = r_u32(0x800ED384);
            changed = 1;
            w_u32(0x800ED384, count + 1);
        }
    }
    if (changed == 1 && r_u32(0x800ED384) == 15) {
        menu = r_u32(0x80124C00);
        if (r_u32(menu) == 1) {
            sound_play_sample_pitch(10, 128);
            sound_play_sample_pitch(10, 128);
            sound_play_sample_pitch(10, 128);
            sound_play_sample_pitch(10, 128);
            w_u32(0x800ED384, 0);
        }
    }
    flags = r_u32(0x80053684);
    if ((flags & 0x40) != 0) {
        menu = r_u32(0x80124C00);
        index = r_u32(menu);
        w_u32(0x800FB3BC, 0);
        w_u32(menu + 0x10, 4);
        record = menu + 0x30 + index * 28;
        flags = r_u32(0x80108A44);
        callback = r_u32(record);
        w_u32(0x80108A44, flags | 8);
        psx_dispatch_guest_call(callback, 1, index, 0, 0);
        value = r_u32(record + 8);
        w_u32(0x800FA26C, 1);
        w_u32(0x800ECF14, 1);
        w_u32(0x80124C48, value);
        sound_play_sample_pitch(11, 128);
    } else if ((flags & 0x10) != 0) {
        menu = r_u32(0x80124C00);
        index = r_u32(menu);
        w_u32(0x800FB3BC, 0);
        w_u32(menu + 0x10, 4);
        record = menu + 0x30 + index * 28;
        flags = r_u32(0x80108A44);
        callback = r_u32(record + 4);
        w_u32(0x80108A44, flags | 8);
        psx_dispatch_guest_call(callback, 1, index, 0, 0);
        value = r_u32(record + 12);
        w_u32(0x800FA26C, 2);
        w_u32(0x80124C48, value);
        sound_play_sample_pitch(11, 128);
    }
}

void game_resume_from_pause_settings(void)
{
    uint32 menu, record, a, b, c, d, e, f;
    FUNCTION_MARKER(0x8008B02C, "GAME.OVL");
    menu = r_u32(0x80124C00);
    record = r_u32(menu + 0x1C);
    a = r_u32(record + 8);
    b = r_u32(record + 12);
    c = r_u32(record);
    d = r_u32(record + 4);
    e = r_u32(record + 8);
    f = r_u32(record + 12);
    w_u32(0x800ECEA8, 1);
    w_u32(record + 0x18, a);
    w_u32(record + 0x1C, b);
    w_u32(record + 0x20, c);
    w_u32(record + 0x24, d);
    w_u32(record + 0x10, e);
    w_u32(record + 0x14, f);
}

void game_draw_pause_resource_wait(void)
{
    uint32 result, count, menu, flags, index, record, callback, value;
    FUNCTION_MARKER(0x8008C470, "GAME.OVL");
    game_draw_pause_grid_background();
    game_draw_pause_options();
    game_draw_pause_panel();
    game_draw_pause_heading();
    result = game_poll_resource_job();
    if (result == 1) {
        count = r_u32(0x80119C28);
        if ((sint32)count < 300) {
            w_u32(0x80119C28, count + 1);
            count = r_u32(0x80119C28);
        }
    } else {
        count = r_u32(0x80119C28);
    }
    if (count == 300) {
        menu = r_u32(0x80124C00);
        w_u32(0x80119C28, 301);
        flags = r_u32(0x80108A44);
        index = r_u32(menu);
        w_u32(0x80108A44, flags | 8);
        w_u32(menu + 0x10, 4);
        record = menu + 0x30 + index * 28;
        callback = r_u32(record);
        w_u32(0x800FB3BC, 0);
        psx_dispatch_guest_call(callback, 1, menu, 0, 0);
        value = r_u32(record + 8);
        w_u32(0x800ECF14, 1);
        w_u32(0x80124C48, value);
    }
    game_draw_pause_warp_grid();
}

void game_start_pause_resource_wait(void)
{
    FUNCTION_MARKER(0x8008C56C, "GAME.OVL");
    w_u32(0x80119C28, 0);
    game_leave_pause_settings();
    game_begin_resource_job();
}

void game_cancel_pause_resource_wait(void)
{
    FUNCTION_MARKER(0x8008C59C, "GAME.OVL");
    stream_stop();
    stream_cancel_current();
    game_release_resource_job_buffer();
}

void game_release_resource_job_buffer(void)
{
    uint32 buffer;
    FUNCTION_MARKER(0x800E9B94, "GAME.OVL");
    buffer = r_u32(0x800F1930);
    if (buffer != 0)
        heap_free(buffer);
    w_u32(0x800F1930, 0);
}

void game_draw_pause_setting_row(uint32 index, uint32 x, uint32 y, uint32 selected)
{
    uint32 mode = r_u32(0x800311C0);
    uint32 color, offset, setting, menu, text, packet, second, shift, value, other, saved;
    uint32 ordering, page, clut;
    sint32 current, maximum, numerator, quotient, fill, natural, cx, cy, tp, tu, tv;
    FUNCTION_MARKER(0x8008B16C, "GAME.OVL");
    if (mode != 0)
        y += (uint32)((sint32)y >> 4);
    color = selected == 1 ? 128 : 64;
    offset = index * 8;
    setting = r_u32(0x800ECEE4 + offset);
    menu = r_u32(0x80124C00);
    current = r_s16(setting);
    w_u32(0x80033D2C, 0);
    value = r_u32(menu + 0x44 + index * 28);
    w_u8(0x8004617C, (uint8)color);
    text = r_u32(0x800F24C4 + value * 4);
    w_u8(0x80033D38, (uint8)color);
    w_u8(0x80053680, (uint8)color);
    w_u32(0x8005368C, y - 12);
    w_u32(0x80053688, x + 8);
    font_draw_current(game_script_text_native(text));
    maximum = r_s16(0x800ECEEA + offset);
    numerator = (sint32)((uint32)current * 1000);
    if (maximum == 0)
        trap(7);
    if (maximum == -1 && numerator == (sint32)0x80000000)
        trap(0x5D);
    quotient = (sint32)((sint64)numerator / maximum);
    fill = (sint32)((uint32)quotient * 209) / 1000;
    packet = r_u32(0x80040690);
    w_u32(0x80040690, packet + 40);
    SetSprt((SPRT *)psx_addr(packet, 20));
    shift = (2 - (uint32)r_s16(0x8004FA7C)) & 31;
    value = r_u16(0x8004FA70);
    w_u8(packet + 0xC, (uint8)((value & 63) << shift));
    value = r_u16(0x8004FA72);
    w_u16(packet + 0xA, (uint16)y);
    w_u16(packet + 8, (uint16)x);
    w_u8(packet + 0xD, (uint8)value);
    natural = r_s16(0x8004FA7E);
    second = packet + 20;
    if (natural < fill)
        w_u16(packet + 0x10, (uint16)natural);
    else
        w_u16(packet + 0x10, (uint16)fill);
    value = r_u16(0x8004FA7A);
    w_u8(packet + 4, (uint8)color);
    w_u8(packet + 5, (uint8)color);
    w_u16(packet + 0x12, (uint16)value);
    w_u8(packet + 6, (uint8)color);
    cx = r_s16(0x8004FA74);
    cy = r_s16(0x8004FA76);
    clut = GetClut(cx, cy);
    ordering = r_u32(0x8004608C);
    w_u16(packet + 0xE, (uint16)clut);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 20));
    SetSprt((SPRT *)psx_addr(second, 20));
    shift = (2 - (uint32)r_s16(0x8004FA5C)) & 31;
    value = r_u16(0x8004FA50);
    w_u8(second + 0xC, (uint8)((value & 63) << shift));
    value = r_u16(0x8004FA52);
    w_u16(second + 0xA, (uint16)y);
    w_u16(second + 8, (uint16)x);
    w_u8(second + 0xD, (uint8)value);
    value = r_u16(0x8004FA5E);
    w_u16(second + 0x10, (uint16)value);
    value = r_u16(0x8004FA5A);
    w_u8(second + 4, (uint8)color);
    w_u8(second + 5, (uint8)color);
    w_u16(second + 0x12, (uint16)value);
    w_u8(second + 6, (uint8)color);
    cx = r_s16(0x8004FA54);
    cy = r_s16(0x8004FA56);
    clut = GetClut(cx, cy);
    ordering = r_u32(0x8004608C);
    w_u16(second + 0xE, (uint16)clut);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(second, 20));
    natural = r_s16(0x8004FA7E);
    if (natural < fill) {
        packet = r_u32(0x80040690);
        w_u32(0x80040690, packet + 40);
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        value = r_u16(0x8004FA00);
        w_u8(packet + 0xC, (uint8)((value & 63) << shift));
        value = r_u16(0x8004FA02);
        w_u8(packet + 0xD, (uint8)value);
        value = r_u8(packet + 0xC);
        other = r_u8(0x8004FA0E);
        saved = r_u8(packet + 0xD);
        w_u8(packet + 0x1C, (uint8)value);
        value = r_u8(packet + 0xC);
        --other;
        w_u8(packet + 0x15, (uint8)saved);
        w_u8(packet + 0x14, (uint8)(value + other));
        value = r_u8(packet + 0xD);
        other = r_u8(0x8004FA0A);
        w_u8(packet + 0x1D, (uint8)(value + other));
        value = r_u8(packet + 0x14);
        other = r_u8(packet + 0x1D);
        w_u8(packet + 0x24, (uint8)value);
        w_u8(packet + 0x25, (uint8)other);
        value = r_u16(0x8004FA7E);
        w_u16(packet + 0xA, (uint16)y);
        w_u16(packet + 8, (uint16)(value + x));
        value = r_u16(packet + 8);
        other = r_u16(0x8004FA7E);
        saved = r_u16(packet + 8);
        w_u16(packet + 0x12, (uint16)y);
        w_u16(packet + 0x10, (uint16)(value + (uint32)fill - other));
        w_u16(packet + 0x18, (uint16)saved);
        value = r_u16(0x8004FA0A);
        w_u16(packet + 0x1A, (uint16)(value + y));
        value = r_u16(packet + 0x10);
        other = r_u16(packet + 0x1A);
        w_u16(packet + 0x20, (uint16)value);
        w_u16(packet + 0x22, (uint16)other);
        cx = r_s16(0x8004FA04);
        cy = r_s16(0x8004FA06);
        clut = GetClut(cx, cy);
        w_u16(packet + 0xE, (uint16)clut);
        tp = r_s16(0x8004FA0C);
        tu = r_s16(0x8004FA00);
        tv = r_s16(0x8004FA02);
        page = GetTPage(tp, 0, tu, tv);
        w_u8(packet + 4, (uint8)color);
        w_u8(packet + 5, (uint8)color);
        w_u8(packet + 6, (uint8)color);
        ordering = r_u32(0x8004608C);
        w_u16(packet + 0x16, (uint16)page);
        AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 40));
    }
    packet = r_u32(0x80040690);
    w_u32(0x80040690, packet + 40);
    SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
    value = r_u16(0x8004F9D0);
    w_u8(packet + 0xC, (uint8)((value & 63) << shift));
    value = r_u16(0x8004F9D2);
    w_u8(packet + 0xD, (uint8)value);
    value = r_u8(packet + 0xC);
    other = r_u8(0x8004F9DE);
    saved = r_u8(packet + 0xD);
    w_u8(packet + 0x1C, (uint8)value);
    value = r_u8(packet + 0xC);
    --other;
    w_u8(packet + 0x15, (uint8)saved);
    w_u8(packet + 0x14, (uint8)(value + other));
    value = r_u8(packet + 0xD);
    other = r_u8(0x8004F9DA);
    w_u8(packet + 0x1D, (uint8)(value + other));
    value = r_u8(packet + 0x14);
    other = r_u8(packet + 0x1D);
    w_u8(packet + 0x24, (uint8)value);
    w_u8(packet + 0x25, (uint8)other);
    value = r_u16(second + 0x10);
    w_u16(packet + 0xA, (uint16)y);
    w_u16(packet + 8, (uint16)(value + x));
    value = r_u16(0x8004FA5E);
    other = r_u16(0x8004FA4E);
    w_u16(packet + 0x12, (uint16)y);
    saved = 210 - value - other;
    value = r_u16(packet + 8);
    other = r_u16(packet + 8);
    w_u16(packet + 0x10, (uint16)(value + saved));
    w_u16(packet + 0x18, (uint16)other);
    value = r_u16(0x8004F9DA);
    w_u16(packet + 0x1A, (uint16)(value + y));
    value = r_u16(packet + 0x10);
    other = r_u16(packet + 0x1A);
    w_u16(packet + 0x20, (uint16)value);
    w_u16(packet + 0x22, (uint16)other);
    cx = r_s16(0x8004F9D4);
    cy = r_s16(0x8004F9D6);
    clut = GetClut(cx, cy);
    w_u16(packet + 0xE, (uint16)clut);
    w_u8(packet + 4, (uint8)color);
    w_u8(packet + 5, (uint8)color);
    w_u8(packet + 6, (uint8)color);
    tp = r_s16(0x8004F9DC);
    tu = r_s16(0x8004F9D0);
    tv = r_s16(0x8004F9D2);
    page = GetTPage(tp, 0, tu, tv);
    ordering = r_u32(0x8004608C);
    w_u16(packet + 0x16, (uint16)page);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 40));
    packet = r_u32(0x80040690);
    w_u32(0x80040690, packet + 20);
    SetSprt((SPRT *)psx_addr(packet, 20));
    shift = (2 - (uint32)r_s16(0x8004FA4C)) & 31;
    value = r_u16(0x8004FA40);
    w_u8(packet + 0xC, (uint8)((value & 63) << shift));
    value = r_u16(0x8004FA42);
    w_u8(packet + 0xD, (uint8)value);
    value = r_u16(0x8004FA4E);
    w_u16(packet + 0xA, (uint16)y);
    w_u16(packet + 8, (uint16)(x + 210 - value));
    value = r_u16(0x8004FA4E);
    w_u16(packet + 0x10, (uint16)value);
    value = r_u16(0x8004FA4A);
    w_u16(packet + 0x12, (uint16)value);
    cx = r_s16(0x8004FA44);
    cy = r_s16(0x8004FA46);
    clut = GetClut(cx, cy);
    w_u8(packet + 4, (uint8)color);
    w_u8(packet + 5, (uint8)color);
    w_u8(packet + 6, (uint8)color);
    ordering = r_u32(0x8004608C);
    w_u16(packet + 0xE, (uint16)clut);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 20));
    tp = r_s16(0x8004FA4C);
    tu = r_s16(0x8004FA40);
    tv = r_s16(0x8004FA42);
    page = GetTPage(tp, 1, tu, tv);
    packet = r_u32(0x80040690);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)page, NULL);
    ordering = r_u32(0x8004608C);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 12));
    w_u32(0x80040690, packet + 12);
}

static void game_draw_resource_text_cursor(uint32 x, uint32 y)
{
    uint32 packet, ordering;
    packet = r_u32(0x80040690);
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x60);
    w_u16(packet + 8, (uint16)x);
    w_u8(packet + 4, 255);
    w_u8(packet + 5, 255);
    w_u8(packet + 6, 255);
    ordering = r_u32(0x8004608C);
    w_u16(packet + 0xC, 6);
    w_u16(packet + 0xE, 12);
    w_u16(packet + 0xA, (uint16)y);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(packet, 16));
    w_u32(0x80040690, packet + 16);
}

uint32 game_poll_resource_job(void)
{
    uint32 frame = r_u32(0x800F238C);
    uint32 stream_state = r_u32(0x80040FE4);
    uint32 x, y, mode, index, total, cursor, line, extra, spaces, value, next, first, second;
    uint32 layout[6], scroll = 0;
    sint32 remaining;
    FUNCTION_MARKER(0x800E96E4, "GAME.OVL");
    w_u32(0x800F238C, frame + 1);
    if (stream_state == 6 || r_s16(0x800118FE) == 0)
        w_u32(0x800F1958, 1);
    if (r_u8(0x8004EA28) == 0) {
        x = r_u32(0x800F2384);
        y = r_u32(0x800F2388);
        mode = r_u32(0x800311C0);
        w_u32(0x80033D2C, 84);
        w_u8(0x8004617C, 160);
        w_u8(0x80033D38, 160);
        w_u8(0x80053680, 160);
        w_u32(0x80053688, x);
        w_u32(0x8005368C, y);
        if (mode != 0)
            w_u32(0x8005368C, y + (uint32)((sint32)y >> 4));
        font_draw_current(game_script_text_native(0x80010210));
    }
    w_u32(0x80033D2C, 84);
    layout[1] = r_u32(0x800F193C);
    total = r_u32(0x800F1934);
    w_u8(0x8004617C, 128);
    w_u8(0x80033D38, 128);
    w_u8(0x80053680, 128);
    layout[2] = 0x505060;
    line = 0x800F1988;
    extra = 0x800F21F8;
    spaces = 0x800F22C0;
    for (index = 1; (sint32)index < (sint32)total; ++index) {
        cursor = r_u32(0x800F1938);
        if ((sint32)cursor < (sint32)index)
            break;
        layout[0] = 40;
        if (layout[1] - 65 < 88) {
            layout[3] = r_u32(extra);
            layout[4] = r_u32(spaces);
            layout[5] = 38;
            if (index == cursor) {
                layout[5] = r_u32(0x800F1940);
                remaining = font_draw_justified_native(game_script_text_native(line), layout);
                if (remaining > 0) {
                    value = r_u32(0x800F1938);
                    w_u32(0x800F1940, 0);
                    w_u32(0x800F1938, value + 1);
                }
                if ((r_u32(0x800F238C) & 2) != 0)
                    game_draw_resource_text_cursor(layout[0], layout[1]);
            } else {
                font_draw_justified_native(game_script_text_native(line), layout);
            }
            if ((sint32)layout[1] >= 142)
                scroll = 1;
        } else if ((sint32)layout[1] < 65 && (sint32)(layout[1] + 12) >= 66) {
            layout[3] = r_u32(extra);
            layout[4] = r_u32(spaces);
            layout[5] = 38;
            font_draw_justified_native(game_script_text_native(line), layout);
        } else if (layout[1] - 153 < 12) {
            layout[3] = r_u32(extra);
            scroll = 1;
            layout[4] = r_u32(spaces);
            cursor = r_u32(0x800F1938);
            layout[5] = 38;
            if (index == cursor) {
                layout[5] = r_u32(0x800F1940);
                remaining = font_draw_justified_native(game_script_text_native(line), layout);
                if (remaining > 0) {
                    value = r_u32(0x800F1938);
                    w_u32(0x800F1940, 0);
                    w_u32(0x800F1938, value + 1);
                }
                if ((r_u32(0x800F238C) & 2) != 0)
                    game_draw_resource_text_cursor(layout[0], layout[1]);
            } else {
                font_draw_justified_native(game_script_text_native(line), layout);
            }
        }
        line += 44;
        spaces += 4;
        layout[1] += 12;
        extra += 4;
    }
    if (r_u32(0x800F1958) == 1) {
        value = r_u32(0x800F1948);
        next = r_u32(0x800F1944);
        value += next;
        w_u32(0x800F1948, value);
        if ((sint32)value >= 256) {
            next = r_u32(0x800F1940);
            w_u32(0x800F1948, value - 256);
            w_u32(0x800F1940, next + 1);
        }
    }
    if (scroll != 0) {
        value = r_u32(0x800F193C);
        w_u32(0x800F193C, value - 2);
    }
    first = r_u32(0x800F1934);
    second = r_u32(0x800F1938);
    return first == second;
}

void game_begin_resource_job(void)
{
    char filename[24], *extension;
    uint32 size, buffer, count = 0, asset, mode, next_mode;
    sint32 divisor, numerator, interval, ticks;
    FUNCTION_MARKER(0x800E9420, "GAME.OVL");
    if (r_u32(0x800F192C) != 0) {
        w_u32(0x800F192C, 0);
        return;
    }
    w_u32(0x800F193C, 65);
    w_u32(0x800F1938, 1);
    w_u32(0x800F1940, 0);
    strcpy(filename, game_script_text_native(0x80010518));
    extension = strchr(filename, '.');
    if (extension != NULL)
        *extension = 0;
    strcat(filename, game_script_text_native(0x80076B84));
    size = disc_file_size(filename);
    buffer = heap_alloc_back(size + 4);
    w_u32(0x800F1930, buffer);
    if (buffer != 0) {
        resource_load_into(filename, buffer);
        buffer = r_u32(0x800F1930);
        w_u8(buffer + size, 0);
        buffer = r_u32(0x800F1930);
        w_u8(buffer + size + 1, 0);
        buffer = r_u32(0x800F1930);
        w_u32(0x80033D2C, 84);
        count = game_parse_briefing_text(buffer, 0x800F192C);
        w_u32(0x800F1938, 1);
    } else {
        w_u32(0x800F1934, 0);
    }
    w_u32(0x800F1958, 0);
    asset = game_request_stream_preset_three();
    if ((sint32)count > 0) {
        mode = r_u32(0x800311C0);
        divisor = (sint32)asset / (mode != 0 ? 6 : 5);
        if (divisor == 0)
            trap(7);
        interval = (sint32)((sint64)0x1900000 / divisor);
        next_mode = r_u32(0x800311C0);
        w_u32(0x800F194C, (uint32)interval);
        ticks = (sint32)(asset - 600) / (next_mode != 0 ? 6 : 5);
        numerator = (sint32)((uint32)ticks << 8);
        if (count == 0)
            trap(7);
        if ((sint32)count == -1 && numerator == (sint32)0x80000000)
            trap(0x5D);
        divisor = (sint32)((sint64)numerator / (sint32)count);
        if (divisor == 0)
            trap(7);
        interval = (sint32)((sint64)65536 / divisor);
        w_u32(0x800F1944, (uint32)interval);
    } else {
        w_u32(0x800F1944, 50);
        w_u32(0x800F194C, 50);
    }
    w_u32(0x800F1950, 0);
    w_u32(0x800F1948, 0);
}

static uint32 game_resource_text_find(uint32 text, int character)
{
    const char *native_text = game_script_text_native(text);
    const char *found = strchr(native_text, character);
    return found != NULL ? text + (uint32)(found - native_text) : 0;
}

uint32 game_parse_briefing_text(uint32 text, uint32 state)
{
    uint32 line_end, candidate, index, spaces, length, destination, width, next, character;
    uint32 total = 0, full_line = 0;
    FUNCTION_MARKER(0x800E9208, "GAME.OVL");
    w_u32(state + 8, 0);
    while (r_u8(text) != 0) {
        line_end = game_resource_text_find(text, 13);
        index = r_u32(state + 8);
        w_u32(state + 0x990 + index * 4, 0xFFFFFFFE);
        if (line_end != 0 && (sint32)(line_end - text) < 39) {
            full_line = 1;
        } else {
            line_end = text;
            candidate = text;
            if (text != 0) {
                while ((sint32)(candidate - text) < 38) {
                    line_end = candidate;
                    candidate = game_resource_text_find(candidate + 1, ' ');
                    index = r_u32(state + 8);
                    spaces = r_u32(state + 0x990 + index * 4);
                    w_u32(state + 0x990 + index * 4, spaces + 1);
                    if (candidate == 0)
                        break;
                }
            }
            if (line_end == text) {
                full_line = 1;
                length = (uint32)strlen(game_script_text_native(text));
                line_end = text + length;
            }
        }
        length = line_end - text;
        index = r_u32(state + 8);
        destination = state + 0x30 + index * 44;
        strncpy((char *)psx_addr(destination, (size_t)length), game_script_text_native(text), (size_t)length);
        index = r_u32(state + 8);
        total += length;
        destination = state + index * 44 + length;
        w_u8(destination + 0x30, 0);
        index = r_u32(state + 8);
        if (full_line != 0) {
            w_u32(state + 0x8C8 + index * 4, 0);
        } else {
            destination = state + 0x30 + index * 44;
            width = (uint32)text_measure_width_without_brackets(destination);
            index = r_u32(state + 8);
            w_u32(state + 0x8C8 + index * 4, 270 - width);
        }
        full_line = 0;
        next = line_end + 1;
        character = r_u8(next);
        while (character == 13 || character == 10) {
            ++next;
            character = r_u8(next);
        }
        text = next;
        index = r_u32(state + 8) + 1;
        w_u32(state + 8, index);
        if ((sint32)index >= 50)
            break;
    }
    return total;
}

void game_draw_projected_light_occlusion(uint32 position, uint32 index)
{
    sint32 screen, projection, flags, depth, radius, quarter, eighth, half, sx, sy, brightness = 255;
    sint32 trig, attenuation, small_x, small_y, offset_x[8], offset_y[8];
    const sint32 sizes[8] = {10, 16, 8, 48, 32, 64, 32, 16};
    uint32 count = 0, i, rectangle, pixel, product, value, x, y, ordering, buffer;
    uint32 dx, dy, squared, parity, color, edge, triple_x, triple_y, nine_x, nine_y;
    FUNCTION_MARKER(0x8008D410, "GAME.OVL");
    radius = (sint16)r_u16(0x80123F92 + index * 28);
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    quarter = radius >> 2;
    eighth = radius >> 3;
    depth = RotTransPers((SVECTOR *)psx_addr(position, 8), &screen, &projection, &flags);
    if (depth != 0) {
        value = (uint32)r_s16(0x80123F92 + index * 28);
        half = radius >> 1;
        sx = (sint16)(uint32)screen;
        sy = (sint16)((uint32)screen >> 16);
        if ((sint32)value < 80) {
            game_draw_projected_glow(sx, sy, half, 0xFFFFFF, 0);
            game_draw_projected_glow(sx, sy, (sint32)((uint32)half + (uint32)eighth), 0xFFFFFF, 0);
            game_draw_projected_glow(sx, sy, (sint32)((uint32)half + (uint32)quarter), 0xA0FF, 0);
        }
        for (i = 0; i < 16; ++i) {
            value = r_u32(0x80119B28 + index * 4 + i * 16);
            if ((value & 0x7FFF) != 0x7FFF)
                ++count;
        }
        for (i = 0; i < 16; ++i) {
            rectangle = 0x800F3E10 + index * 8 + i * 32;
            w_u16(rectangle + 4, 1);
            w_u16(rectangle + 6, 1);
            trig = rsin((sint32)(i * 256));
            product = (uint32)quarter * (uint32)trig;
            value = r_u32(0x800F3DF0 + index * 4);
            w_u16(rectangle, (uint16)(value + (uint32)((sint32)product >> 12)));
            trig = rcos((sint32)(i * 256));
            product = (uint32)eighth * (uint32)trig;
            value = r_u32(0x800F3E00 + index * 4);
            y = value + (uint32)((sint32)product >> 12);
            x = r_u16(rectangle);
            w_u16(rectangle + 2, (uint16)y);
            if (x < 512 && (y & 0xFFFF) < 256) {
                ordering = r_u32(0x8004608C);
                buffer = r_u32(0x80042170);
                if (ordering == buffer)
                    w_u16(rectangle + 2, (uint16)(y + 256));
                pixel = 0x80119B28 + index * 4 + i * 16;
                StoreImage((PSX_RECT *)psx_addr(rectangle, 8), (uint32 *)psx_addr(pixel, 2));
            }
        }
        brightness = (sint32)(255 - count * 15);
        sx = (sint16)(uint32)screen;
        w_u32(0x800F3DF0 + index * 4, (uint32)sx);
        parity = r_u32(0x800311B8) & 1;
        sy = (sint16)((uint32)screen >> 16);
        w_u32(0x800F3E00 + index * 4, (uint32)sy);
        if (count == 16)
            return;
        if (r_s16(0x80123F92 + index * 28) < 80) {
            color = (uint32)brightness + ((uint32)brightness << 8);
            game_draw_projected_ring(sx, sy, (sint32)((uint32)radius + parity), color, 0);
            game_draw_projected_ring(sx, sy,
                (sint32)((uint32)radius + (uint32)(radius >> 1) + parity), (uint32)brightness, 0);
        }
    }
    depth = RotTransPers((SVECTOR *)psx_addr(position, 8), &screen, &projection, &flags);
    if ((uint32)depth < 64)
        return;
    sx = (sint16)(uint32)screen;
    sy = (sint16)((uint32)screen >> 16);
    dx = (256 - (uint32)sx) << 8;
    dy = (120 - (uint32)sy) << 8;
    small_x = (sint32)dx >> 3;
    small_y = (sint32)dy >> 3;
    squared = (uint32)small_x * (uint32)small_x + (uint32)small_y * (uint32)small_y;
    attenuation = (sint32)(255 - (uint32)(SquareRoot0((sint32)squared) >> 6));
    product = (uint32)brightness * (uint32)attenuation;
    brightness = (sint32)product >> 8;
    if (brightness < 0)
        brightness = 0;
    if (brightness >= 256)
        brightness = 255;
    value = (uint32)brightness << 4;
    color = (value >> 8) + (value & 0xFF00) + (((uint32)brightness << 12) & 0xFF0000);
    edge = value >> 8;
    triple_x = (uint32)small_x * 3;
    triple_y = (uint32)small_y * 3;
    nine_x = (uint32)small_x * 9;
    nine_y = (uint32)small_y * 9;
    offset_x[0] = (sint32)dx >> 11;
    offset_y[0] = (sint32)dy >> 11;
    offset_x[1] = (sint32)triple_x >> 8;
    offset_y[1] = (sint32)triple_y >> 8;
    offset_x[2] = small_x >> 6;
    offset_y[2] = small_y >> 6;
    offset_x[3] = (sint32)triple_x >> 7;
    offset_y[3] = (sint32)triple_y >> 7;
    offset_x[4] = (sint32)nine_x >> 8;
    offset_y[4] = (sint32)nine_y >> 8;
    offset_x[5] = (sint32)triple_x >> 6;
    offset_y[5] = (sint32)triple_y >> 6;
    offset_x[6] = small_x >> 4;
    offset_y[6] = small_y >> 4;
    offset_x[7] = (sint32)nine_x >> 7;
    offset_y[7] = (sint32)nine_y >> 7;
    for (i = 0; i < 8; ++i)
        game_draw_projected_ring((sint32)((uint32)sx + (uint32)offset_x[i]),
            (sint32)((uint32)sy + (uint32)offset_y[i]), sizes[i], color, edge);
}

void game_draw_red_light_occlusion(uint32 position, uint32 index)
{
    sint32 screen, projection, flags, depth, radius, quarter, eighth, half, sx, sy, brightness = 255;
    sint32 trig, attenuation, small_x, small_y, offset_x[8], offset_y[8];
    const sint32 sizes[8] = {10, 16, 8, 48, 32, 64, 32, 16};
    uint32 count = 0, i, rectangle, pixel, product, value, x, y, ordering, buffer;
    uint32 dx, dy, squared, parity, color, edge, triple_x, triple_y, nine_x, nine_y;
    FUNCTION_MARKER(0x8008D9B8, "GAME.OVL");
    radius = (sint16)r_u16(0x80123F92 + index * 28);
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    quarter = radius >> 2;
    eighth = radius >> 3;
    depth = RotTransPers((SVECTOR *)psx_addr(position, 8), &screen, &projection, &flags);
    if (depth != 0) {
        value = (uint32)r_s16(0x80123F92 + index * 28);
        half = radius >> 1;
        sx = (sint16)(uint32)screen;
        sy = (sint16)((uint32)screen >> 16);
        if ((sint32)value < 80) {
            game_draw_projected_glow(sx, sy, half, 0xFFFFFF, 0);
            game_draw_projected_glow(sx, sy, (sint32)((uint32)half + (uint32)eighth), 0xFFFFFF, 0);
            game_draw_projected_glow(sx, sy, (sint32)((uint32)half + (uint32)quarter), 0xFF, 0);
        }
        for (i = 0; i < 16; ++i) {
            value = r_u32(0x80119B28 + index * 4 + i * 16);
            if ((value & 0x7FFF) != 0x7FFF)
                ++count;
        }
        for (i = 0; i < 16; ++i) {
            rectangle = 0x800F3E10 + index * 8 + i * 32;
            w_u16(rectangle + 4, 1);
            w_u16(rectangle + 6, 1);
            trig = rsin((sint32)(i * 256));
            product = (uint32)quarter * (uint32)trig;
            value = r_u32(0x800F3DF0 + index * 4);
            w_u16(rectangle, (uint16)(value + (uint32)((sint32)product >> 12)));
            trig = rcos((sint32)(i * 256));
            product = (uint32)eighth * (uint32)trig;
            value = r_u32(0x800F3E00 + index * 4);
            y = value + (uint32)((sint32)product >> 12);
            x = r_u16(rectangle);
            w_u16(rectangle + 2, (uint16)y);
            if (x < 512 && (y & 0xFFFF) < 256) {
                ordering = r_u32(0x8004608C);
                buffer = r_u32(0x80042170);
                if (ordering == buffer)
                    w_u16(rectangle + 2, (uint16)(y + 256));
                pixel = 0x80119B28 + index * 4 + i * 16;
                StoreImage((PSX_RECT *)psx_addr(rectangle, 8), (uint32 *)psx_addr(pixel, 2));
            }
        }
        brightness = (sint32)(255 - count * 15);
        sx = (sint16)(uint32)screen;
        w_u32(0x800F3DF0 + index * 4, (uint32)sx);
        parity = r_u32(0x800311B8) & 1;
        sy = (sint16)((uint32)screen >> 16);
        w_u32(0x800F3E00 + index * 4, (uint32)sy);
        if (count == 16)
            return;
        if (r_s16(0x80123F92 + index * 28) < 80) {
            color = (uint32)brightness;
            game_draw_projected_ring(sx, sy, (sint32)((uint32)radius + parity), color, 0);
            game_draw_projected_ring(sx, sy,
                (sint32)((uint32)radius + (uint32)(radius >> 1) + parity), (uint32)brightness, 0);
        }
    }
    depth = RotTransPers((SVECTOR *)psx_addr(position, 8), &screen, &projection, &flags);
    if ((uint32)depth < 64)
        return;
    sx = (sint16)(uint32)screen;
    sy = (sint16)((uint32)screen >> 16);
    dx = (256 - (uint32)sx) << 8;
    dy = (120 - (uint32)sy) << 8;
    small_x = (sint32)dx >> 3;
    small_y = (sint32)dy >> 3;
    squared = (uint32)small_x * (uint32)small_x + (uint32)small_y * (uint32)small_y;
    attenuation = (sint32)(255 - (uint32)(SquareRoot0((sint32)squared) >> 6));
    product = (uint32)brightness * (uint32)attenuation;
    brightness = (sint32)product >> 8;
    if (brightness < 0)
        brightness = 0;
    if (brightness >= 256)
        brightness = 255;
    value = (uint32)brightness << 4;
    product = (uint32)brightness * 5;
    color = (value >> 8) + (product & 0xFF00) + ((product << 8) & 0xFF0000);
    edge = value >> 8;
    triple_x = (uint32)small_x * 3;
    triple_y = (uint32)small_y * 3;
    nine_x = (uint32)small_x * 9;
    nine_y = (uint32)small_y * 9;
    offset_x[0] = (sint32)dx >> 11;
    offset_y[0] = (sint32)dy >> 11;
    offset_x[1] = (sint32)triple_x >> 8;
    offset_y[1] = (sint32)triple_y >> 8;
    offset_x[2] = small_x >> 6;
    offset_y[2] = small_y >> 6;
    offset_x[3] = (sint32)triple_x >> 7;
    offset_y[3] = (sint32)triple_y >> 7;
    offset_x[4] = (sint32)nine_x >> 8;
    offset_y[4] = (sint32)nine_y >> 8;
    offset_x[5] = (sint32)triple_x >> 6;
    offset_y[5] = (sint32)triple_y >> 6;
    offset_x[6] = small_x >> 4;
    offset_y[6] = small_y >> 4;
    offset_x[7] = (sint32)nine_x >> 7;
    offset_y[7] = (sint32)nine_y >> 7;
    for (i = 0; i < 8; ++i)
        game_draw_projected_ring((sint32)((uint32)sx + (uint32)offset_x[i]),
            (sint32)((uint32)sy + (uint32)offset_y[i]), sizes[i], color, edge);
}

void game_draw_violet_light_occlusion(uint32 position, uint32 index)
{
    sint32 screen, projection, flags, depth, radius, quarter, eighth, half, sx, sy, brightness = 255;
    sint32 trig, attenuation, small_x, small_y, offset_x[8], offset_y[8];
    const sint32 sizes[8] = {10, 16, 8, 48, 32, 64, 32, 16};
    uint32 count = 0, i, rectangle, pixel, product, value, x, y, ordering, buffer;
    uint32 dx, dy, squared, parity, color, edge, triple_x, triple_y, nine_x, nine_y;
    FUNCTION_MARKER(0x8008DF64, "GAME.OVL");
    radius = (sint16)r_u16(0x80123F92 + index * 28);
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    quarter = radius >> 2;
    eighth = radius >> 3;
    depth = RotTransPers((SVECTOR *)psx_addr(position, 8), &screen, &projection, &flags);
    if (depth != 0) {
        value = (uint32)r_s16(0x80123F92 + index * 28);
        half = radius >> 1;
        sx = (sint16)(uint32)screen;
        sy = (sint16)((uint32)screen >> 16);
        if ((sint32)value < 80) {
            game_draw_projected_glow(sx, sy, half, 0xFFFFFF, 0);
            game_draw_projected_glow(sx, sy, (sint32)((uint32)half + (uint32)eighth), 0xFFFFFF, 0);
            game_draw_projected_glow(sx, sy, (sint32)((uint32)half + (uint32)quarter), 0xFF00FF, 0);
        }
        for (i = 0; i < 16; ++i) {
            value = r_u32(0x80119B28 + index * 4 + i * 16);
            if ((value & 0x7FFF) != 0x7FFF)
                ++count;
        }
        for (i = 0; i < 16; ++i) {
            rectangle = 0x800F3E10 + index * 8 + i * 32;
            w_u16(rectangle + 4, 1);
            w_u16(rectangle + 6, 1);
            trig = rsin((sint32)(i * 256));
            product = (uint32)quarter * (uint32)trig;
            value = r_u32(0x800F3DF0 + index * 4);
            w_u16(rectangle, (uint16)(value + (uint32)((sint32)product >> 12)));
            trig = rcos((sint32)(i * 256));
            product = (uint32)eighth * (uint32)trig;
            value = r_u32(0x800F3E00 + index * 4);
            y = value + (uint32)((sint32)product >> 12);
            x = r_u16(rectangle);
            w_u16(rectangle + 2, (uint16)y);
            if (x < 512 && (y & 0xFFFF) < 256) {
                ordering = r_u32(0x8004608C);
                buffer = r_u32(0x80042170);
                if (ordering == buffer)
                    w_u16(rectangle + 2, (uint16)(y + 256));
                pixel = 0x80119B28 + index * 4 + i * 16;
                StoreImage((PSX_RECT *)psx_addr(rectangle, 8), (uint32 *)psx_addr(pixel, 2));
            }
        }
        brightness = (sint32)(255 - count * 15);
        sx = (sint16)(uint32)screen;
        w_u32(0x800F3DF0 + index * 4, (uint32)sx);
        parity = r_u32(0x800311B8) & 1;
        sy = (sint16)((uint32)screen >> 16);
        w_u32(0x800F3E00 + index * 4, (uint32)sy);
        if (count == 16)
            return;
        if (r_s16(0x80123F92 + index * 28) < 80) {
            color = (uint32)brightness;
            game_draw_projected_ring(sx, sy, (sint32)((uint32)radius + parity), color, 0);
            game_draw_projected_ring(sx, sy,
                (sint32)((uint32)radius + (uint32)(radius >> 1) + parity),
                (uint32)brightness + ((uint32)brightness << 16), 0);
        }
    }
    depth = RotTransPers((SVECTOR *)psx_addr(position, 8), &screen, &projection, &flags);
    if ((uint32)depth < 64)
        return;
    sx = (sint16)(uint32)screen;
    sy = (sint16)((uint32)screen >> 16);
    dx = (256 - (uint32)sx) << 8;
    dy = (120 - (uint32)sy) << 8;
    small_x = (sint32)dx >> 3;
    small_y = (sint32)dy >> 3;
    squared = (uint32)small_x * (uint32)small_x + (uint32)small_y * (uint32)small_y;
    attenuation = (sint32)(255 - (uint32)(SquareRoot0((sint32)squared) >> 6));
    product = (uint32)brightness * (uint32)attenuation;
    brightness = (sint32)product >> 8;
    if (brightness < 0)
        brightness = 0;
    if (brightness >= 256)
        brightness = 255;
    value = (uint32)brightness << 4;
    color = (value >> 8) + ((value >> 8) << 8) + ((value >> 8) << 16);
    edge = (value >> 8) + ((value >> 8) << 16);
    triple_x = (uint32)small_x * 3;
    triple_y = (uint32)small_y * 3;
    nine_x = (uint32)small_x * 9;
    nine_y = (uint32)small_y * 9;
    offset_x[0] = (sint32)dx >> 11;
    offset_y[0] = (sint32)dy >> 11;
    offset_x[1] = (sint32)triple_x >> 8;
    offset_y[1] = (sint32)triple_y >> 8;
    offset_x[2] = small_x >> 6;
    offset_y[2] = small_y >> 6;
    offset_x[3] = (sint32)triple_x >> 7;
    offset_y[3] = (sint32)triple_y >> 7;
    offset_x[4] = (sint32)nine_x >> 8;
    offset_y[4] = (sint32)nine_y >> 8;
    offset_x[5] = (sint32)triple_x >> 6;
    offset_y[5] = (sint32)triple_y >> 6;
    offset_x[6] = small_x >> 4;
    offset_y[6] = small_y >> 4;
    offset_x[7] = (sint32)nine_x >> 7;
    offset_y[7] = (sint32)nine_y >> 7;
    for (i = 0; i < 8; ++i)
        game_draw_projected_ring((sint32)((uint32)sx + (uint32)offset_x[i]),
            (sint32)((uint32)sy + (uint32)offset_y[i]), sizes[i], color, edge);
}

void game_update_warp_grid(uint32 index)
{
    uint32 offset, state, phase, timer, shift, scale, value, product, row, column;
    uint32 red, green = 0, blue, camera, x, y, z, camera_x, camera_y, camera_z;
    uint32 point, linear, packet, packet_base, parity, last_y, depth_value, velocity;
    uint32 next_row, previous_row, neighbors, sum, height, object, ticks, depth_address;
    sint32 screen, projection, flags, depth, tx, ty, tz, hx, hy, hz, length, average;
    MATRIX scaled, rotation;
    SVECTOR angle;
    VECTOR difference;
    const uint32 injection_shifts[6] = {4, 4, 5, 6, 7, 8};
    FUNCTION_MARKER(0x8008F03C, "GAME.OVL");
    offset = index * 0x5C48;
    if (r_u32(0x8010ACD0 + offset) == 0)
        return;
    phase = r_u32(0x8010ACD8 + offset);
    state = r_u32(0x8010ACD0 + offset);
    phase += 8;
    w_u32(0x8010ACD8 + offset, phase);
    if (state - 3 < 2) {
        timer = r_u32(0x8010ACD4 + offset);
        value = 64 - timer;
        red = (uint8)(value << 1);
        blue = (uint8)(value << 2);
        shift = r_u32(0x8010ACE0 + offset);
        timer = r_u32(0x8010ACD4 + offset);
        scale = value << (shift & 31);
        w_u32(0x8010ACD8 + offset, phase + (timer << 2));
    } else if (state == 7) {
        timer = r_u32(0x8010ACD4 + offset);
        red = (uint8)(timer << 1);
        shift = r_u32(0x8010ACE0 + offset);
        blue = (uint8)(timer << 2);
        scale = timer << (shift & 31);
        timer = r_u32(0x8010ACD4 + offset);
        w_u32(0x8010ACD8 + offset, phase + ((64 - timer) << 2));
    } else {
        red = 128;
        blue = 255;
        shift = r_u32(0x8010ACE0 + offset);
        scale = (uint32)64 << (shift & 31);
    }
    for (column = 0; column < 3; ++column) {
        for (row = 0; row < 3; ++row) {
            value = (uint32)r_s16(0x8010ACE8 + offset + row * 6 + column * 2);
            product = value * scale;
            scaled.m[row][column] = (sint16)((sint32)product >> 6);
        }
    }
    MulMatrix0((MATRIX *)psx_addr(0x80035060, 20), &scaled, &scaled);
    phase = r_u32(0x8010ACD8 + offset);
    angle.vx = 0;
    angle.vy = 0;
    angle.vz = (sint16)phase;
    RotMatrixYXZ(&angle, &rotation);
    MulMatrix0(&scaled, &rotation, &scaled);
    camera = r_u32(0x80040C94);
    x = r_u32(0x8010AD08 + offset);
    camera_x = r_u32(camera + 8);
    difference.vx = (sint32)(x - camera_x);
    y = r_u32(0x8010AD0C + offset);
    camera_y = r_u32(camera + 12);
    difference.vy = (sint32)(y - camera_y);
    z = r_u32(0x8010AD10 + offset);
    camera_z = r_u32(camera + 16);
    difference.vz = (sint32)(z - camera_z);
    ApplyMatrixLV((MATRIX *)psx_addr(0x80035060, 20), &difference, &difference);
    tx = difference.vx >> 4;
    w_u32(0x80034D6C, (uint32)tx);
    hx = tx < 0 ? -tx : tx;
    ty = difference.vy >> 4;
    tz = difference.vz >> 4;
    w_u32(0x80034D70, (uint32)ty);
    w_u32(0x80034D74, (uint32)tz);
    hy = ty < 0 ? -ty : ty;
    hz = tz < 0 ? -tz : tz;
    if (hx < 32000 && hy < 32000 && hz < 32000) {
        SetRotMatrix(&scaled);
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
        angle.vx = 0;
        angle.vy = 0;
        angle.vz = 0;
        depth = RotTransPers(&angle, &screen, &projection, &flags);
        if (!(depth < 33 && ((uint32)flags & 0xFFFF0000) != 0)) {
            state = r_u32(0x8010ACD0 + offset);
            if (state - 3 < 2 || state - 5 < 2 || state == 9 || state == 7) {
                for (row = 0; row < 16; ++row) {
                    for (column = 0; column < 16; ++column) {
                        linear = row * 16 + column;
                        point = 0x8010FD18 + offset + row * 128 + column * 8;
                        depth = RotTransPers((SVECTOR *)psx_addr(point, 8), &screen, &projection, &flags);
                        w_u32(0x80033D50 + linear * 4, (uint32)screen);
                        w_u32(0x800510F8 + linear * 4, (uint32)depth);
                    }
                }
                depth_address = 0x800510F8 + 255 * 4;
                for (row = 0; row < 16; ++row) {
                    next_row = ((row + 1) & 15) * 16;
                    for (column = 0; column < 15; ++column) {
                        parity = r_u32(0x800311B8) & 1;
                        packet_base = offset + row * 0x500 + column * 0x50;
                        packet = 0x8010AD18 + packet_base + parity * 40;
                        depth_value = r_u32(depth_address);
                        linear = row * 16 + column;
                        value = r_u16(0x80033D50 + linear * 4);
                        w_u16(packet + 8, (uint16)value);
                        value = r_u16(0x80033D52 + linear * 4);
                        w_u16(packet + 10, (uint16)value);
                        linear = next_row + column;
                        value = r_u16(0x80033D50 + linear * 4);
                        w_u16(packet + 16, (uint16)value);
                        value = r_u16(0x80033D52 + linear * 4);
                        w_u16(packet + 18, (uint16)value);
                        linear = row * 16 + column + 1;
                        value = r_u16(0x80033D50 + linear * 4);
                        w_u16(packet + 24, (uint16)value);
                        value = r_u16(0x80033D52 + linear * 4);
                        w_u16(packet + 26, (uint16)value);
                        linear = next_row + column + 1;
                        value = r_u16(0x80033D50 + linear * 4);
                        w_u16(packet + 32, (uint16)value);
                        last_y = r_u16(0x80033D52 + linear * 4);
                        depth_value <<= 6;
                        w_u8(packet + 4, (uint8)red);
                        parity = r_u32(0x800311B8) & 1;
                        w_u16(packet + 34, (uint16)last_y);
                        w_u8(0x8010AD1D + packet_base + parity * 40, (uint8)green);
                        parity = r_u32(0x800311B8) & 1;
                        w_u8(0x8010AD1E + packet_base + parity * 40, (uint8)blue);
                        parity = r_u32(0x800311B8) & 1;
                        render_insert_primitive((sint32)depth_value, 0x8010AD18 + packet_base + parity * 40);
                    }
                }
            }
        }
    }
    for (row = 0; row < 16; ++row) {
        next_row = ((row + 1) & 15) * 128;
        previous_row = ((row - 1) & 15) * 128;
        for (column = 0; column < 16; ++column) {
            point = 0x8010FD18 + offset + row * 128 + column * 8;
            height = (uint32)r_s16(point + 4);
            neighbors = 0;
            sum = 0;
            if (column < 15) {
                neighbors = 3;
                x = (uint32)r_s16(0x8010FD1C + offset + next_row + (column + 1) * 8);
                y = (uint32)r_s16(point + 12);
                z = (uint32)r_s16(0x8010FD1C + offset + previous_row + (column + 1) * 8);
                sum = (y - height) + (x - height) + (z - height);
            }
            if (column > 0) {
                neighbors += 3;
                x = (uint32)r_s16(point - 4);
                y = (uint32)r_s16(0x8010FD1C + offset + next_row + (column - 1) * 8);
                z = (uint32)r_s16(0x8010FD1C + offset + previous_row + (column - 1) * 8);
                sum += x - height;
                sum += y - height;
                sum += z - height;
            }
            neighbors += 2;
            x = (uint32)r_s16(0x8010FD1C + offset + previous_row + column * 8);
            y = (uint32)r_s16(0x8010FD1C + offset + next_row + column * 8);
            sum += x - height;
            sum += y - height;
            average = (sint32)sum / (sint32)neighbors;
            point = 0x8010FD18 + offset + row * 128 + column * 8;
            value = (uint32)r_s16(point + 4);
            velocity = r_u32(0x80110518 + offset + row * 64 + column * 4);
            value = (uint32)((sint32)(0 - value) >> 2);
            velocity += value + (uint32)average;
            product = velocity * 15;
            w_u32(0x80110518 + offset + row * 64 + column * 4, (uint32)((sint32)product >> 4));
            value = r_u16(point + 4);
            w_u16(point + 4, (uint16)(value + (uint32)((sint32)product >> 8)));
        }
    }
    state = r_u32(0x8010ACD0 + offset);
    if (state == 6) {
        object = r_u32(0x8010ACE4 + offset);
        x = r_u32(0x8010AD08 + offset);
        y = r_u32(0x8010AD0C + offset);
        camera_x = r_u32(object + 8);
        camera_y = r_u32(object + 12);
        camera_z = r_u32(object + 16);
        z = r_u32(0x8010AD10 + offset);
        length = vector_length_approx((sint32)(x - camera_x), (sint32)(y - camera_y), (sint32)(z - camera_z));
        for (row = 0; row < 16; ++row) {
            for (column = 0; column < 6; ++column) {
                shift = r_u32(0x8010ACE0 + offset);
                value = (uint32)(length >> ((shift + injection_shifts[column]) & 31));
                w_u16(0x8010FD1C + offset + row * 128 + column * 8, (uint16)value);
            }
        }
        if (r_u32(0x8010ACD4 + offset) == 0)
            sound_play_distant_at_camera(0x8010AD08 + offset, 24, 256);
    } else if (state - 1 < 2 || state == 8) {
        timer = r_u32(0x8010ACD4 + offset);
        game_draw_position_glow(0x8010AD08 + offset, 0x808080, timer * 300);
    }
    timer = r_u32(0x8010ACD4 + offset) - 1;
    w_u32(0x8010ACD4 + offset, timer);
    timer = r_u32(0x8010ACD4 + offset);
    if ((sint32)timer > 0)
        return;
    state = r_u32(0x8010ACD0 + offset);
    switch (state) {
    case 8:
        w_u32(0x8010ACD0 + offset, 0);
        return;
    case 7:
        w_u32(0x8010ACD0 + offset, 8);
        ticks = 32;
        break;
    case 3:
    case 4:
    case 5:
    case 6:
        w_u32(0x8010ACD0 + offset, 9);
        ticks = 160;
        break;
    case 1:
        w_u32(0x8010ACD0 + offset, 3);
        ticks = 64;
        break;
    case 2:
        w_u32(0x8010ACD0 + offset, 4);
        ticks = 64;
        break;
    case 9:
        w_u32(0x8010ACD0 + offset, 7);
        ticks = 63;
        break;
    default:
        return;
    }
    w_u32(0x8010ACD4 + offset, ticks);
}

