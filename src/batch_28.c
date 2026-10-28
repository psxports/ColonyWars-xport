#include "game_functions.h"
#include <stdio.h>

uint32 game_ship_task_19(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 count, threshold, identifier, health_a, health_b, maximum_a, maximum_b, percent, index;
    uint32 format;
    sint32 numerator, denominator, quotient;
    FUNCTION_MARKER(0x8009CCC0, "GAME.OVL");
    if (r_u8(object + 0x26E) == 1) {
        game_skip_ship_task(object, ordinal);
        return 0;
    }
    count = game_get_ship_task_parameter(task, 3);
    w_u8(object + 0x272, count);
    threshold = game_get_ship_task_parameter(task, 2);
    if (threshold == 0) {
        w_u8(object + 0x272, count);
        game_skip_ship_task(object, ordinal);
        return 0;
    }
    identifier = r_u32(object + 4);
    health_a = r_u32(object + 0x234);
    health_b = r_u32(object + 0x238);
    maximum_a = r_u32(0x800536A4 + identifier * 60);
    maximum_b = r_u32(0x800536A8 + identifier * 60);
    numerator = (sint32)((health_a + health_b) << 8);
    denominator = (sint32)(maximum_a + maximum_b);
    if (denominator == 0)
        trap(7);
    if (denominator == -1 && numerator == (sint32)0x80000000u)
        trap(0x5D);
    quotient = numerator / denominator;
    percent = (uint32)((sint32)((uint32)quotient * 100) >> 8);
    if ((sint32)percent >= (sint32)threshold)
        return 0;
    format = r_u32(0x800F2458);
    sprintfPSX(0x800FA368, format, 0);
    game_queue_hud_message(0x800FA368);
    for (index = 0; (sint32)index < (sint32)count; ++index)
        game_create_escape_pod(object);
    w_u8(object + 0x272, 0);
    w_u8(object + 0x26E, 1);
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_create_escape_pod(uint32 carrier)
{
    uint32 x, y, z, model, object, identifier, value, yaw, pitch, roll, generation, flags, slot;
    uint32 health, seed, mixed, intensity, parameter;
    FUNCTION_MARKER(0x800A5BE4, "GAME.OVL");
    x = r_u32(carrier + 8);
    y = r_u32(carrier + 12);
    model = r_u32(0x800EDDE0);
    z = r_u32(carrier + 16);
    object = game_create_object(model, x, y, z, 7);
    identifier = (uint32)r_s16(0x80059614);
    w_u32(object + 0x3C, 0x800A5E84);
    value = r_u32(object + 0x70);
    w_u32(object + 4, identifier);
    w_u32(object + 0x70, value | 0x100000);
    matrix_set_identity(object + 0x11C);
    w_u32(object + 0x38, 0x8002B8FC);
    w_u32(object + 0x6C, 0x800EF574);
    w_u32(object + 0x1E4, 0x800EEC7C);
    w_u32(object + 0x58, 0x8002493C);
    w_u32(object + 0x40, 0x800A4F00);
    w_u32(object + 0x44, 0x8009FA2C);
    w_u32(object + 0x9C, 1);
    w_u32(object + 0xB0, 60);
    yaw = (uint32)random_between(0, 4095);
    pitch = (uint32)random_between(0, 4095);
    roll = (uint32)random_between(0, 4095);
    game_set_object_rotation(object, yaw, pitch, roll);
    value = (uint32)random_between(100, 150);
    game_set_acceleration_from_basis(object, value);
    w_u32(object + 0x84, 0xFFFFFFFF);
    value = (uint32)random_signed_between(3, 32);
    w_u32(object + 0xA8, value);
    value = (uint32)random_signed_between(3, 32);
    w_u32(object + 0xAC, value);
    w_u32(object + 0xD8, 200000);
    w_u32(object + 0x170, carrier);
    generation = r_u16(carrier + 0x24A);
    w_u32(object + 0xE8, 32000);
    w_u32(object + 0xEC, 32000);
    w_u16(object + 0x260, 128);
    w_u32(object + 0xF0, 2048);
    w_u32(object + 0x244, 1000000);
    w_u32(object + 0x234, 50);
    w_u32(object + 0x238, 50);
    flags = r_u32(object + 0x114);
    slot = r_u16(0x800EF572);
    w_u16(object + 0x174, generation);
    identifier = r_u32(object + 4);
    health = r_u32(object + 0x234);
    w_u32(object + 0x114, flags | 0x200000);
    w_u16(object + 0x24E, slot);
    seed = r_u32(0x800EF50C);
    w_u32(0x800536A4 + identifier * 60, health);
    mixed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    intensity = (mixed & 15) | 32;
    identifier = r_u32(object + 4);
    health = r_u32(object + 0x238);
    w_u32(0x800536A8 + identifier * 60, health);
    seed = (uint32)((sint32)mixed >> 1) ^ (mixed << 3);
    w_u32(0x800EF50C, seed);
    parameter = seed & 31;
    identifier = r_u32(object + 4);
    health = r_u32(object + 0x244);
    w_u32(0x800536AC + identifier * 60, health);
    game_attach_object_light(object, 10, 0xBBBBBB, intensity, parameter);
    w_u32(object + 0x5C, 0x800A5030);
    w_u32(object + 0x48, 0x800A5030);
    return object;
}

uint32 game_ship_task_18(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 target, stored_generation, generation, identifier, limit, threshold, own_id;
    uint32 maximum_a, maximum_b, health_a, health_b, buffer, position, category, remaining;
    uint32 assigned, candidate, flags, callback;
    sint32 numerator, denominator, percent;
    FUNCTION_MARKER(0x8009CAC0, "GAME.OVL");
    target = r_u32(object + 0x1A8);
    if (target == 0)
        return 0;
    stored_generation = r_u16(object + 0x1AC);
    generation = r_u16(target + 0x24A);
    if (stored_generation != generation)
        return 0;
    identifier = game_get_ship_task_parameter(task, 2);
    limit = game_get_ship_task_parameter(task, 3);
    threshold = game_get_ship_task_parameter(task, 4);
    own_id = r_u32(object + 4);
    maximum_a = r_u32(0x800536A4 + own_id * 60);
    maximum_b = r_u32(0x800536A8 + own_id * 60);
    health_a = r_u32(object + 0x234);
    health_b = r_u32(object + 0x238);
    numerator = (sint32)((health_a + health_b) * 100);
    denominator = (sint32)(maximum_a + maximum_b);
    if (denominator == 0)
        trap(7);
    if (denominator == -1 && numerator == (sint32)0x80000000u)
        trap(0x5D);
    percent = numerator / denominator;
    if (percent >= (sint32)threshold)
        return 0;
    buffer = heap_alloc_back(1024);
    position = buffer;
    category = r_u32(object);
    remaining = game_collect_matching_ships(identifier, buffer, category, 0);
    assigned = r_u8(object + 0x26D);
    if ((sint32)assigned < (sint32)limit && (sint32)remaining > 0) {
        do {
            candidate = r_u32(position);
            target = r_u32(object + 0x1A8);
            callback = r_u32(candidate + 0x178);
            position += 4;
            if (target != callback) {
                flags = r_u32(candidate + 0x114);
                if (!(flags & 0x1000)) {
                    w_u32(candidate + 0x114, flags | 0x1000);
                    w_u32(candidate + 0x170, object);
                    generation = r_u16(object + 0x24A);
                    w_u16(candidate + 0x174, generation);
                    target = r_u32(object + 0x1A8);
                    w_u32(candidate + 0x178, target);
                    callback = r_u32(candidate + 0x54);
                    generation = r_u16(object + 0x1AC);
                    w_u32(candidate + 0x40, callback);
                    w_u16(candidate + 0x17C, generation);
                    psx_dispatch_guest_call(callback, 1, candidate, 0, 0);
                    assigned = r_u8(object + 0x26D);
                    --limit;
                    w_u8(object + 0x26D, assigned + 1);
                }
            }
            assigned = r_u8(object + 0x26D);
            --remaining;
            if ((sint32)assigned >= (sint32)limit)
                break;
        } while ((sint32)remaining > 0);
    }
    heap_free(buffer);
    return 0;
}

uint32 game_collect_matching_ships(uint32 identifier, uint32 output, uint32 category, uint32 excluded_flags)
{
    uint32 object, flags, count = 0;
    FUNCTION_MARKER(0x800AE624, "GAME.OVL");
    object = object_lookup(category);
    while (object != 0) {
        if (r_u32(object + 4) == identifier) {
            flags = r_u32(object + 0x114);
            if (!(flags & 0x8000) && !(flags & excluded_flags)) {
                w_u32(output, object);
                output += 4;
                ++count;
            }
        }
        object = r_u32(object + 0x1DC);
    }
    return count;
}

uint32 game_ship_task_17(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, threshold, category, selection, target, count = 0, type;
    FUNCTION_MARKER(0x8009C96C, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    threshold = game_get_ship_task_parameter(task, 3);
    category = game_get_ship_task_parameter(task, 4);
    selection = game_get_ship_task_parameter(task, 5);
    target = game_select_ship_task_target(object, selection, category, identifier, 0);
    if (target != 0) {
        type = r_u32(target);
        count = game_count_script_ship_objects(identifier, type);
    } else if (identifier == 0) {
        if (selection != 0)
            count = game_count_objects_by_category(category, selection);
        else if (category == 3)
            count = r_u32(0x800595B0);
        else
            count = r_u32(0x800595B4);
    }
    if ((sint32)threshold < (sint32)count) {
        w_u32(object + 0x40, 0x800A3AE0);
        w_u32(object + 0xE8, 0);
        w_u32(object + 0x9C, 30);
        return 1;
    }
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_16(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 state, value;
    FUNCTION_MARKER(0x8009C834, "GAME.OVL");
    state = r_u32(object + 0x210);
    if (state == 0) {
        state = heap_alloc_back(48);
        w_u32(object + 0x210, state);
        w_u32(state + 0x18, 0);
        state = r_u32(object + 0x210);
        w_u8(state + 0x28, 0);
        state = r_u32(object + 0x210);
        w_u8(object + 0x270, 0);
        w_u32(state, 0);
        state = r_u32(object + 0x210);
        w_u32(state + 0x20, 0);
        state = r_u32(object + 0x210);
        w_u16(state + 0x24, 0);
        state = r_u32(object + 0x210);
        w_u8(state + 0x29, 0);
        state = r_u32(object + 0x210);
        w_u32(state + 8, 0);
        state = r_u32(object + 0x210);
        w_u32(state + 12, 0);
        state = r_u32(object + 0x210);
        w_u32(state + 16, 0);
        state = r_u32(object + 0x210);
        w_u32(state + 4, 0xFFFFFFFF);
        w_u16(object + 0x1BC, 0);
    }
    value = game_get_ship_task_parameter(task, 2);
    state = r_u32(object + 0x210);
    w_u32(state, value);
    state = r_u32(object + 0x210);
    if (r_s32(state) >= 3)
        diagnostic_report(0, 0x80075AFC, 0x80075A1C);
    w_u8(object + 0x270, 1);
    game_skip_ship_task(object, ordinal);
    w_u32(object + 0x40, 0x8002493C);
    return 0;
}

uint32 game_ship_task_14(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 blocked = r_u32(0x800EC884);
    uint32 identifier, carrier, bay, generation, flags;
    FUNCTION_MARKER(0x8009C38C, "GAME.OVL");
    if (blocked != 0)
        return 0;
    if (r_u32(object + 0xE0) == ordinal && game_validate_ship_task_target(object) != 0) {
        w_u32(object + 0x40, 0x800A49A4);
        return 1;
    }
    identifier = game_get_ship_task_parameter(task, 2);
    carrier = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0);
    if (carrier == 0)
        return 0;
    if (!(r_u32(carrier + 0x114) & 0x10))
        diagnostic_report(identifier, 0x80075ABC, 0x80075A1C);
    if (r_s16(carrier + 0x1C4) == 0) {
        identifier = r_u32(carrier + 4);
        diagnostic_report(identifier, 0x80075ADC, 0x80075A1C);
    }
    bay = game_find_free_carrier_bay_near_target(carrier, object);
    if (bay == 0)
        return 0;
    w_u32(object + 0x40, 0x800A49A4);
    w_u32(object + 0x170, carrier);
    generation = r_u16(carrier + 0x24A);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x88, 1);
    w_u32(object + 0x20C, bay);
    w_u16(object + 0x174, generation);
    w_u32(object + 0x114, flags | 0x40);
    w_u16(bay, 30);
    return 1;
}

uint32 game_ship_task_13(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 target, generation, stored_generation, tx, x, ty, y, tz, z, distance, range, identifier, speed;
    FUNCTION_MARKER(0x8009C1DC, "GAME.OVL");
    if (r_u32(object + 0xE0) == ordinal) {
        target = r_u32(object + 0x170);
        if (target == 0)
            return 0;
        generation = r_u16(target + 0x24A);
        stored_generation = r_u16(object + 0x174);
        if (generation != stored_generation)
            return 0;
        tx = r_u32(target + 8);
        x = r_u32(object + 8);
        ty = r_u32(target + 12);
        y = r_u32(object + 12);
        tz = r_u32(target + 16);
        z = r_u32(object + 16);
        distance = (uint32)vector_length_approx((sint32)(tx - x), (sint32)(ty - y), (sint32)(tz - z));
        range = r_u32(object + 0xA8);
        w_u32(0x80046174, distance);
        if ((sint32)range < (sint32)distance) {
            identifier = r_u32(object + 4);
            goto begin;
        }
    } else {
        identifier = game_get_ship_task_parameter(task, 2);
        target = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0);
        if (target == 0)
            return 0;
        range = game_get_ship_task_parameter(task, 3);
        tx = r_u32(target + 8);
        x = r_u32(object + 8);
        ty = r_u32(target + 12);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        tz = r_u32(target + 16);
        distance = (uint32)vector_length_approx((sint32)(tx - x), (sint32)(ty - y), (sint32)(tz - z));
        w_u32(0x80046174, distance);
        if ((sint32)range < (sint32)distance) {
            w_u32(object + 0x170, target);
            generation = r_u16(target + 0x24A);
            identifier = r_u32(object + 4);
            w_u32(object + 0xA8, range);
            w_u16(object + 0x174, generation);
            goto begin;
        }
    }
    if (game_get_ship_task_parameter(task, 1) != 0)
        game_skip_ship_task(object, ordinal);
    return 0;
begin:
    speed = r_u32(0x8005369C + identifier * 60);
    w_u32(object + 0x40, 0x800A48F4);
    w_u32(object + 0x9C, 30);
    w_u32(object + 0xE8, speed);
    return 1;
}

uint32 game_ship_task_12(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, target, range, opponent, generation, callback, value;
    FUNCTION_MARKER(0x8009C0B0, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0);
    if (target == 0)
        return 0;
    range = game_get_ship_task_parameter(task, 3);
    opponent = game_find_nearby_opposing_ship(object, range);
    if (opponent != 0) {
        w_u32(object + 0x170, 0);
        w_u32(object + 0x178, opponent);
        generation = r_u16(opponent + 0x24A);
        callback = r_u32(object + 0x54);
        w_u16(object + 0x17C, generation);
        w_u32(object + 0x40, callback);
        return 1;
    }
    if (r_u32(object + 0xE0) != ordinal || r_u32(object + 0x170) != target) {
        w_u32(object + 0x88, 1);
        w_u32(object + 0x170, target);
        generation = r_u16(target + 0x24A);
        w_u16(object + 0x174, generation);
        value = game_get_ship_task_parameter(task, 4);
        w_u32(object + 0x214, value);
        value = game_get_ship_task_parameter(task, 5);
        w_u32(object + 0x218, value);
        value = game_get_ship_task_parameter(task, 6);
        w_u32(object + 0x21C, value);
    }
    w_u32(object + 0x40, 0x800A457C);
    w_u32(object + 0x9C, 30);
    return 1;
}

uint32 game_find_nearby_opposing_ship(uint32 object, uint32 range)
{
    uint32 target, tx, x, ty, y, z, tz, distance;
    FUNCTION_MARKER(0x800A4D7C, "GAME.OVL");
    if (r_u32(object) == 3)
        target = game_find_nearest_type_one(object, 0);
    else
        target = game_find_nearest_type_three(object, 0);
    if (target == 0)
        return 0;
    tx = r_u32(target + 8);
    x = r_u32(object + 8);
    ty = r_u32(target + 12);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    tz = r_u32(target + 16);
    distance = (uint32)vector_length_approx((sint32)(tx - x), (sint32)(ty - y), (sint32)(tz - z));
    return (sint32)distance < (sint32)range ? target : 0;
}


uint32 game_adjust_maneuver_offsets(uint32 yaw, uint32 pitch)
{
    uint32 value;
    sint32 direction;
    FUNCTION_MARKER(0x800A3684, "GAME.OVL");
    direction = r_s32(0x80033D18);
    value = r_u32(0x80033D1C);
    value = direction > 0 ? value - yaw : value + yaw;
    w_u32(0x80033D1C, value);
    direction = r_s32(0x80033D20);
    value = r_u32(0x80033D30);
    value = direction > 0 ? value - pitch : value + pitch;
    w_u32(0x80033D30, value);
    return value;
}

void game_begin_timed_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 update)
{
    FUNCTION_MARKER(0x800C94DC, "GAME.OVL");
    w_u32(object + 0x9C, ticks);
    w_u32(object + 0x50, completion);
    w_u32(object + 0x4C, update);
    w_u32(object + 0x40, 0x800C9460);
    game_update_object_state(object);
}

void game_update_ship_task_approach(uint32 object)
{
    uint32 target, generation, current;
    FUNCTION_MARKER(0x800A3704, "GAME.OVL");
    target = r_u32(object + 0x170);
    if (target != 0) {
        current = r_u16(target + 0x24A);
        generation = r_u16(object + 0x174);
        if (current == generation) {
            game_steer_to_target_offset(object, target, object + 0x214, 0);
            return;
        }
    }
    w_u32(object + 0x40, 0x8009AB58);
    w_u32(object + 0x170, 0);
}

void game_check_ship_task_approach_target(uint32 object)
{
    uint32 target, current, generation;
    FUNCTION_MARKER(0x800A3760, "GAME.OVL");
    target = r_u32(object + 0x170);
    if (target != 0) {
        current = r_u16(target + 0x24A);
        generation = r_u16(object + 0x174);
        if (current == generation)
            return;
    }
    w_u32(object + 0x40, 0x8009AB58);
}

void game_tick_timed_maneuver(uint32 object)
{
    uint32 callback, yaw, pitch, ticks;
    FUNCTION_MARKER(0x800C9460, "GAME.OVL");
    callback = r_u32(object + 0x4C);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
    yaw = r_u32(0x80033D1C);
    pitch = r_u32(0x80033D30);
    w_u16(object + 0x250, yaw);
    w_u16(object + 0x252, pitch);
    game_update_object_state(object);
    game_update_object_movement(object);
    ticks = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks <= 0) {
        callback = r_u32(object + 0x50);
        w_u32(object + 0x40, callback);
    }
}

uint32 game_validate_ship_attack_target(uint32 object)
{
    uint32 target, generation, current, flags;
    FUNCTION_MARKER(0x8009AE28, "GAME.OVL");
    target = r_u32(object + 0x178);
    if (target != 0) {
        current = r_u16(target + 0x24A);
        generation = r_u16(object + 0x17C);
        if (generation == current) {
            flags = r_u32(object + 0x114);
            if (flags & 0x10)
                return (r_u32(target + 0x114) & 0x8000) == 0;
            return 1;
        }
    }
    w_u32(object + 0x178, 0);
    return 0;
}

uint32 game_ship_task_06(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, identifier, target, generation, callback;
    FUNCTION_MARKER(0x8009B2B0, "GAME.OVL");
    flags = r_u32(object + 0x114);
    if (flags & 0x4000)
        return 0;
    if (flags & 0x10) {
        identifier = game_get_ship_task_parameter(task, 2);
        target = game_find_script_ship_near(object, identifier, 0x7FFFFFF, 0);
        if (target == 0)
            return 0;
        w_u32(object + 0x178, target);
        generation = r_u16(target + 0x24A);
        w_u16(object + 0x17C, generation);
        return 0;
    }
    if (r_u32(object + 0xE0) == ordinal && game_validate_ship_attack_target(object) != 0) {
        callback = r_u32(object + 0x54);
        w_u32(object + 0x40, callback);
        return 1;
    }
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near(object, identifier, 0x7FFFFFF, 0);
    if (target == 0)
        return 0;
    if (target == object)
        diagnostic_report(r_u32(object + 4), 0x80075A20, 0x80075A1C);
    w_u32(object + 0x178, target);
    generation = r_u16(target + 0x24A);
    callback = r_u32(object + 0x54);
    w_u16(object + 0x17C, generation);
    w_u32(object + 0x40, callback);
    return 1;
}

uint32 game_ship_task_11(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, current, speed, value, count, address, waypoint, tx, ty, tz, x, y, z;
    uint32 yaw, pitch, distance, ticks;
    FUNCTION_MARKER(0x8009BE5C, "GAME.OVL");
    identifier = r_u32(object + 4);
    current = r_u32(object + 0xE0);
    speed = r_u32(0x8005369C + identifier * 60);
    w_u32(object + 0xE8, speed);
    if (current != ordinal) {
        count = game_get_ship_task_parameter(task, 0);
        w_u32(object + 0xB0, count);
        if ((sint32)count < 2) {
            identifier = r_u32(object + 4);
            diagnostic_report(identifier, 0x80075A9C, 0x80075A1C);
        }
        w_u32(object + 0xB4, 2);
    } else if (r_u32(object + 0xA8) == 0xFFFFFFFF) {
        value = r_u32(object + 0xB4);
        count = r_u32(object + 0xB0);
        ++value;
        w_u32(object + 0xB4, value);
        if ((sint32)value > (sint32)count) {
            address = *cursor;
            value = r_u32(address);
            if (value != 0) {
                w_u32(address, value - 1);
                address = *cursor;
                if (r_s32(address) <= 0) {
                    game_skip_ship_task(object, ordinal);
                    return 0;
                }
            }
            w_u32(object + 0xB4, 2);
        }
    }
    value = r_u32(object + 0xB4);
    waypoint = game_get_ship_task_parameter(task, value);
    if ((sint32)waypoint >= r_s32(0x800EDB34) || (sint32)waypoint <= 0)
        diagnostic_report(waypoint, 0x80075A88, 0x80075A1C);
    address = 0x80126D68 + waypoint * 12;
    value = r_u32(address);
    x = r_u32(object + 8);
    z = r_u32(object + 16);
    w_u32(object + 0x214, value);
    value = r_u32(address + 4);
    w_u32(object + 0x218, value);
    tx = r_u32(object + 0x214);
    tz = r_u32(address + 8);
    w_u32(object + 0xA8, 0x07FFFFFF);
    y = r_u32(object + 12);
    ty = r_u32(object + 0x218);
    w_u32(object + 0x21C, tz);
    tz = r_u32(object + 0x21C);
    game_set_relative_target_angles(object, tx - x, ty - y, tz - z);
    yaw = r_u32(0x80033D1C);
    pitch = r_u32(0x80033D30);
    distance = r_u32(0x80046174);
    w_u32(object + 0x40, 0x800A3E38);
    w_u32(object + 0xC0, 0);
    w_u32(object + 0xB8, 1);
    w_u32(object + 0xBC, 1);
    w_u16(object + 0x250, yaw);
    w_u16(object + 0x252, pitch);
    if ((sint32)distance > 15000)
        ticks = (uint32)random_between(90, 240);
    else if ((sint32)distance > 4000)
        ticks = (uint32)random_between(30, 70);
    else
        ticks = 5;
    w_u32(object + 0x9C, ticks);
    return 1;
}

uint32 game_ship_task_10(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, range, target, current_target, generation, offset, yaw, pitch, speed, ticks;
    sint32 low, high;
    FUNCTION_MARKER(0x8009BCE0, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    range = game_get_ship_task_parameter(task, 3);
    if (range == 0)
        range = 0x07FFFFFF;
    target = game_find_script_ship_near(object, identifier, range, 0);
    if (target == 0)
        return 0;
    w_u32(object + 0x178, target);
    current_target = r_u32(object + 0x178);
    generation = r_u16(target + 0x24A);
    w_u16(object + 0x17C, generation);
    game_evaluate_target_occlusion(object, current_target, 0);
    if (r_u32(target + 0x114) & 0x10) {
        offset = (uint32)random_signed_between(0x6E0, 0x7F8);
        yaw = r_u32(0x80033D1C);
        low = 0x28;
        high = 0x78;
    } else {
        offset = (uint32)random_signed_between(0x5F0, 0x780);
        yaw = r_u32(0x80033D1C);
        low = 0x50;
        high = 0xF0;
    }
    w_u32(0x80033D1C, yaw + offset);
    offset = (uint32)random_signed_between(low, high);
    pitch = r_u32(0x80033D30);
    w_u32(0x80033D30, pitch + offset);
    identifier = r_u32(object + 4);
    speed = r_u32(0x8005369C + identifier * 60);
    w_u32(object + 0xE8, speed + 0x140);
    ticks = (uint32)random_between(30, 50);
    yaw = r_u32(0x80033D1C);
    pitch = r_u32(0x80033D30);
    game_begin_yaw_maneuver(object, ticks, 0x8009AB58, yaw, pitch, 0x8002493C);
    return 1;
}

uint32 game_ship_task_09(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 waypoint, address, value, tx, x, y, tz, z, ty, yaw, pitch, identifier, speed;
    FUNCTION_MARKER(0x8009BB20, "GAME.OVL");
    if (r_u32(object + 0xE0) == ordinal) {
        if (r_u32(object + 0xA8) == 0xFFFFFFFF) {
            address = *cursor;
            if (r_u32(address) != 0)
                game_skip_ship_task(object, ordinal);
            return 0;
        }
    } else {
        waypoint = game_get_ship_task_parameter(task, 2);
        if ((sint32)waypoint >= r_s32(0x800EDB34) || (sint32)waypoint <= 0)
            diagnostic_report(0, 0x80075A88, 0x80075A1C);
        address = 0x80126D68 + waypoint * 12;
        value = r_u32(address);
        w_u32(object + 0x214, value);
        value = r_u32(address + 4);
        w_u32(object + 0x218, value);
        value = r_u32(address + 8);
        w_u32(object + 0x21C, value);
    }
    w_u32(object + 0x40, 0x800A3E38);
    w_u32(object + 0xA8, 0x07FFFFFF);
    w_u32(object + 0xB8, 0);
    w_u32(object + 0xBC, 1);
    value = game_get_ship_task_parameter(task, 3);
    tx = r_u32(object + 0x214);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    w_u32(object + 0xC0, value);
    tz = r_u32(object + 0x21C);
    z = r_u32(object + 16);
    ty = r_u32(object + 0x218);
    game_set_relative_target_angles(object, tx - x, ty - y, z - tz);
    yaw = r_u32(0x80033D1C);
    pitch = r_u32(0x80033D30);
    identifier = r_u32(object + 4);
    w_u16(object + 0x250, yaw);
    w_u16(object + 0x252, pitch);
    speed = r_u32(0x8005369C + identifier * 60);
    w_u32(object + 0x9C, 30);
    w_u32(object + 0xE8, speed);
    return 1;
}

void game_steer_to_target_offset(uint32 object, uint32 target, uint32 offset, uint32 mode)
{
    VECTOR rotated, relative, transformed;
    uint32 other, own, distance, reference, angle, vertical, depth, value, second;
    FUNCTION_MARKER(0x8008355C, "GAME.OVL");
    ApplyMatrixLV((MATRIX *)psx_addr(target + 0x11C, 20), (VECTOR *)psx_addr(offset, 12), &rotated);
    other = r_u32(target + 8);
    own = r_u32(object + 8);
    relative.vx = (sint32)(other - own + (uint32)rotated.vx);
    other = r_u32(target + 12);
    own = r_u32(object + 12);
    relative.vy = (sint32)(other - own + (uint32)rotated.vy);
    other = r_u32(target + 16);
    own = r_u32(object + 16);
    relative.vz = (sint32)(other - own + (uint32)rotated.vz);
    distance = (uint32)vector_length_approx(relative.vx, relative.vy, relative.vz);
    reference = r_u32(0x80040FDC);
    w_u32(0x80046174, distance);
    if (object == reference)
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), &relative, &transformed);
    else
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11C, 18), &relative, &transformed);
    angle = (uint32)ratan2(transformed.vx, transformed.vz);
    distance = r_u32(0x80046174);
    vertical = (uint32)transformed.vy;
    w_u32(0x80033D1C, angle);
    w_u32(0x80033D18, angle);
    angle = (uint32)ratan2((sint32)(0 - vertical), (sint32)distance);
    w_u32(0x80033D30, angle);
    w_u32(0x80033D20, angle);
    if ((mode << 16) != 0) {
        relative.vx = (sint32)(0 - (uint32)relative.vx);
        relative.vy = (sint32)(0 - (uint32)relative.vy);
        reference = r_u32(0x80040FDC);
        relative.vz = (sint32)(0 - (uint32)relative.vz);
        if (target == reference)
            ApplyMatrixLV((MATRIX *)psx_addr(target + 0x11C, 20), &relative, &transformed);
        else
            game_apply_transposed_matrix_native((const MATRIX *)psx_addr(target + 0x11C, 18), &relative, &transformed);
        angle = (uint32)ratan2(transformed.vx, transformed.vz);
        vertical = (uint32)transformed.vy;
        depth = (uint32)transformed.vz;
        w_u32(0x80040C98, angle);
        if ((sint32)depth < 0)
            depth = 0 - depth;
        angle = (uint32)ratan2((sint32)(0 - vertical), (sint32)depth);
        w_u32(0x80040F1C, angle);
    }
    value = r_u32(0x80033D18);
    second = r_u32(0x80033D20);
    if ((sint32)value < 0)
        value = 0 - value;
    w_u32(0x80052E6C, value);
    value = r_u32(0x80040C98);
    if ((sint32)second < 0)
        second = 0 - second;
    w_u32(0x80053674, second);
    second = r_u32(0x80040F1C);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)second < 0)
        second = 0 - second;
    w_u32(0x80033D24, value);
    w_u32(0x80033D34, second);
}

uint32 game_select_ship_task_target(uint32 object, uint32 selection, uint32 group, uint32 identifier, uint32 excluded_flags)
{
    FUNCTION_MARKER(0x800A5AD4, "GAME.OVL");
    if (selection == 0)
        return game_find_script_ship_near(object, identifier, 0x7FFFFFF, excluded_flags);
    if (selection == 3) {
        if (group == 3)
            return game_find_nearest_type_three(object, excluded_flags);
        return game_find_nearest_type_one(object, excluded_flags);
    }
    return game_find_nearest_object(object, selection == 2 ? 0 : 1, group, excluded_flags);
}

void game_report_tractor_capture(uint32 object, uint32 target)
{
    uint32 slot, format, name;
    FUNCTION_MARKER(0x8009E618, "GAME.OVL");
    if (r_s16(object + 0x262) != 170)
        return;
    slot = (uint32)r_s16(target + 0x24E);
    format = r_u32(0x800F256C);
    name = 0x80059E70 + slot * 44;
    sprintfPSX(0x800FA368, format, name);
    game_queue_hud_message(0x800FA368);
}

uint32 game_ship_task_07(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, selection, group, identifier, target, limit, x, y, z, ox, oy, oz, distance;
    uint32 state, value, generation, callback;
    FUNCTION_MARKER(0x8009B3D8, "GAME.OVL");
    flags = r_u32(object + 0x114);
    if ((flags & 0x8000) || (flags & 0x20000))
        return 0;
    selection = game_get_ship_task_parameter(task, 2);
    group = game_get_ship_task_parameter(task, 3);
    identifier = game_get_ship_task_parameter(task, 4);
    target = game_select_ship_task_target(object, selection, group, identifier, 0x40000);
    if (target == 0)
        return 0;
    limit = game_get_ship_task_parameter(task, 5);
    x = r_u32(target + 8);
    ox = r_u32(object + 8);
    y = r_u32(target + 12);
    oy = r_u32(object + 12);
    oz = r_u32(object + 16);
    z = r_u32(target + 16);
    distance = vector_length_approx(x - ox, y - oy, z - oz);
    w_u32(0x80046174, distance);
    if ((sint32)distance >= (sint32)limit)
        return 0;
    if (r_u32(object + 0xE0) != ordinal)
        game_report_tractor_capture(object, target);
    if (r_u32(object + 0x208) == 0) {
        value = heap_alloc_back(80);
        w_u32(object + 0x208, value);
    }
    state = r_u32(object + 0x208);
    w_u32(state + 0x30, limit);
    value = game_get_ship_task_parameter(task, 6);
    w_u32(state + 0x34, value);
    if (r_u32(object + 0x114) & 0x10) {
        game_get_target_vertex_vector(object, target, state + 0x10);
    } else {
        w_u32(state + 0x10, 0);
        w_u32(state + 0x14, 0);
        w_u32(state + 0x18, 0);
    }
    if ((r_u32(target + 0x114) & 0x10) && r_s32(target + 0x84) > 0) {
        game_get_target_vertex_vector(target, object, state + 0x20);
    } else {
        w_u32(state + 0x20, 0);
        w_u32(state + 0x24, 0);
        w_u32(state + 0x28, 0);
    }
    w_u32(state + 0x38, target);
    generation = r_u16(target + 0x24A);
    w_u16(state + 0x40, 0);
    w_u16(state + 0x3C, generation);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x20000);
    callback = r_u32(target + 0x40);
    w_u32(state + 0x44, callback);
    callback = r_u32(target + 0x3C);
    w_u32(target + 0x40, 0x8002493C);
    w_u32(state + 0x48, callback);
    callback = r_u32(target + 0x44);
    w_u32(state + 0x4C, callback);
    flags = r_u32(target + 0x114);
    w_u16(target + 0x252, 0);
    w_u16(target + 0x250, 0);
    w_u32(target + 0xE8, 0);
    w_u32(target + 0xEC, 0);
    w_u32(target + 0x114, flags | 0x40000);
    game_skip_ship_task(object, ordinal);
    return 0;
}

static void game_queue_ship_launch_message_native(uint32 format, uint32 name)
{
    sprintfPSX(0x800FA368, format, name);
    game_queue_hud_message(0x800FA368);
}

uint32 game_count_matching_active_ship_objects(uint32 reference)
{
    uint32 classes[2] = {3, 1};
    uint32 group, object, identifier, count = 0;
    FUNCTION_MARKER(0x800AE19C, "GAME.OVL");
    for (group = 0; group < 2; ++group) {
        object = object_lookup(classes[group]);
        if (object != 0) {
            identifier = r_u32(reference + 4);
            do {
                if (r_u32(object + 4) == identifier && !(r_u32(object + 0x114) & 0x8000))
                    ++count;
                object = r_u32(object + 0x1DC);
            } while (object != 0);
        }
    }
    return count;
}

void game_position_ship_at_carrier_offset(uint32 carrier, uint32 offset, uint32 ship)
{
    VECTOR position;
    uint32 value, origin;
    FUNCTION_MARKER(0x800A487C, "GAME.OVL");
    ApplyMatrixLV((MATRIX *)psx_addr(carrier + 0x11C, 20),
        (VECTOR *)psx_addr(offset, 12), &position);
    w_u32(ship + 8, (uint32)position.vx);
    w_u32(ship + 12, (uint32)position.vy);
    w_u32(ship + 16, (uint32)position.vz);
    value = r_u32(ship + 8);
    origin = r_u32(carrier + 8);
    w_u32(ship + 8, value + origin);
    value = r_u32(ship + 12);
    origin = r_u32(carrier + 12);
    w_u32(ship + 12, value + origin);
    value = r_u32(ship + 16);
    origin = r_u32(carrier + 16);
    w_u32(ship + 16, value + origin);
}

uint32 game_find_free_carrier_bay_near_target(uint32 carrier, uint32 target)
{
    VECTOR position;
    uint32 bay, index = 0, best = 0, nearest = 0x07FFFFFF;
    uint32 x, y, z, tx, ty, tz, distance;
    sint32 count;
    FUNCTION_MARKER(0x800A6B08, "GAME.OVL");
    count = r_s16(carrier + 0x1C4);
    bay = r_u32(carrier + 0x1CC);
    if (count <= 0)
        return 0;
    do {
        if (r_s16(bay) == 0) {
            ApplyMatrixLV((MATRIX *)psx_addr(carrier + 0x11C, 20),
                (VECTOR *)psx_addr(bay + 0x44, 12), &position);
            x = (uint32)position.vx + r_u32(carrier + 8);
            y = (uint32)position.vy + r_u32(carrier + 12);
            z = (uint32)position.vz + r_u32(carrier + 16);
            tx = r_u32(target + 8);
            tz = r_u32(target + 16);
            ty = r_u32(target + 12);
            distance = (uint32)vector_length_approx((sint32)(tx - x), (sint32)(ty - y), (sint32)(tz - z));
            w_u32(0x80046174, distance);
            if ((sint32)distance < (sint32)nearest) {
                best = bay;
                nearest = distance;
            }
        }
        count = r_s16(carrier + 0x1C4);
        ++index;
        bay += 0x7C;
    } while ((sint32)index < count);
    return best;
}

uint32 game_select_carrier_launch_bay(uint32 object)
{
    uint32 opposite, target;
    FUNCTION_MARKER(0x800AC560, "GAME.OVL");
    opposite = r_u32(object) == 3 ? 1 : 3;
    target = game_find_nearest_object(object, 1, opposite, 0);
    if (target == 0)
        target = r_u32(0x80040FDC);
    return game_find_free_carrier_bay_near_target(object, target);
}

void game_initialize_ship_launch(uint32 ship, uint32 carrier, uint32 bay)
{
    uint32 flags, identifier, fresh_identifier, generation, callback, category, value, yaw, pitch;
    FUNCTION_MARKER(0x800A3B44, "GAME.OVL");
    flags = r_u32(ship + 0x114);
    identifier = r_u32(ship + 4);
    w_u32(ship + 0x44, 0x8002493C);
    w_u32(ship + 0x160, carrier);
    w_u32(ship + 0x114, flags | 8);
    generation = r_u16(carrier + 0x24A);
    w_u32(ship + 0x40, 0x800A3CAC);
    fresh_identifier = r_u32(ship + 4);
    w_u16(ship + 0x164, generation);
    value = r_u32(0x800536A0 + identifier * 60);
    w_u32(ship + 0xE8, value);
    category = r_u32(0x80053698 + fresh_identifier * 60);
    flags = r_u32(ship + 0x114);
    callback = r_u32(ship + 0x58);
    w_u32(ship + 0xA8, bay);
    w_u32(ship + 0x58, 0x8002493C);
    w_u32(ship + 0xC0, flags);
    w_u32(ship + 0x4C, callback);
    w_u32(ship, category);
    game_position_ship_at_carrier_offset(carrier, bay + 0x34, ship);
    identifier = r_u32(ship + 4);
    value = r_u32(0x800536A0 + identifier * 60);
    w_u32(ship + 0xE8, value + 0x2800);
    matrix_copy_rotation(ship + 0x11C, carrier + 0x11C);
    game_steer_to_target_offset(ship, carrier, bay + 0x24, 0);
    yaw = r_u32(0x80033D1C);
    pitch = r_u32(0x80033D30);
    w_u32(ship + 0x28, 0);
    w_u32(ship + 0x20, yaw);
    w_u32(ship + 0x24, pitch);
    w_u16(bay + 0x78, 1);
    w_u16(bay + 0x7A, 10);
    w_u16(bay + 0x76, 0);
    w_u16(bay + 0x74, 0);
    w_u32(ship + 0x9C, 90);
    w_u16(bay, 120);
}

uint32 game_ship_task_08(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 blocked = r_u32(0x800EC884);
    uint32 ship, desired, category, identifier, selection, count, type, existing, own_id, model, missing;
    uint32 list, value, address, yaw, pitch, roll, x, y, z, created, flags, generation, slot, format, bay;
    FUNCTION_MARKER(0x8009B628, "GAME.OVL");
    if (blocked != 0)
        return 0;
    ship = game_get_ship_task_parameter(task, 2);
    desired = game_get_ship_task_parameter(task, 4);
    if (r_u32(object + 4) == ship)
        diagnostic_report(ship, 0x80075A38, 0x80075A1C);
    category = game_get_ship_task_parameter(task, 5);
    identifier = game_get_ship_task_parameter(task, 6);
    selection = game_get_ship_task_parameter(task, 7);
    if (selection == 0) {
        count = game_count_objects_by_category(identifier, category);
    } else {
        value = game_select_ship_task_target(object, category, identifier, selection, 0);
        count = game_count_matching_active_ship_objects(value);
    }
    type = r_u32(0x80053698 + ship * 60);
    existing = game_count_script_ship_objects(ship, type);
    if ((sint32)count >= (sint32)desired || (sint32)existing >= (sint32)desired)
        return 0;
    own_id = r_u32(object + 4);
    model = (uint32)r_s16(0x800536B8 + own_id * 60);
    if (model == r_u32(0x800ED494)) {
        w_u32(object + 0x40, 0x800A7CCC);
        w_u32(object + 0x88, 0);
        w_u32(object + 0xAC, ship);
        list = game_get_ship_task_parameter(task, 3);
        w_u32(object + 0xB0, list);
        missing = desired - existing;
        w_u32(object + 0xB4, missing);
        w_u32(object + 0xB8, r_s16(0x800536BC + ship * 60) == 2 ? 2 : 0);
        if (game_get_ship_task_parameter(task, 1) != 0) {
            address = *cursor;
            value = r_u32(address);
            w_u32(address, value - missing);
            address = *cursor;
            value = r_u32(address);
            if ((sint32)value <= 0)
                game_skip_ship_task(object, ordinal);
        }
        return 1;
    }
    if (model == r_u32(0x800ED498)) {
        missing = desired - existing;
        if ((sint32)missing >= 4)
            diagnostic_report(missing, 0x80075A60, 0x80075A1C);
        yaw = r_u32(object + 0x20);
        pitch = r_u32(object + 0x24);
        roll = r_u32(object + 0x28);
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        created = game_spawn_script_ship(ship, x, y, z, yaw, pitch, roll);
        list = game_get_ship_task_parameter(task, 3);
        game_assign_ship_task_list(created, list);
        w_u32(created + 0x40, 0x800A853C);
        flags = r_u32(created + 0x114);
        w_u32(created + 0x88, 0);
        w_u32(created + 0x170, object);
        generation = r_u16(object + 0x24A);
        w_u32(created + 0x114, flags | 0x2000);
        slot = (uint32)r_s16(created + 0x24E);
        w_u16(created + 0x174, generation);
        format = r_u32(0x800F2554);
        game_queue_ship_launch_message_native(format, 0x80059E70 + slot * 44);
        address = *cursor;
        value = r_u32(address);
        w_u32(address, value - 1);
        address = *cursor;
        value = r_u32(address);
        if ((sint32)value > 0) {
            w_u32(object + 0x40, 0x800A3AE0);
            w_u32(object + 0x9C, 30);
            return 1;
        }
        game_skip_ship_task(object, ordinal);
        return 0;
    }
    if (r_s16(object + 0x1C4) == 0)
        diagnostic_report(own_id, 0x80075A70, 0x80075A1C);
    if (r_u32(object + 0x114) & 0x8000)
        return 0;
    bay = game_select_carrier_launch_bay(object);
    if (bay == 0)
        return 0;
    yaw = r_u32(object + 0x20);
    pitch = r_u32(object + 0x24);
    roll = r_u32(object + 0x28);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    created = game_spawn_script_ship(ship, x, y, z, yaw, pitch, roll);
    list = game_get_ship_task_parameter(task, 3);
    game_assign_ship_task_list(created, list);
    game_initialize_ship_launch(created, object, bay);
    if (game_get_ship_task_parameter(task, 1) != 0) {
        address = *cursor;
        value = r_u32(address);
        w_u32(address, value - 1);
        address = *cursor;
        if (r_u32(address) == 0)
            game_skip_ship_task(object, ordinal);
    }
    slot = (uint32)r_s16(created + 0x24E);
    format = r_u32(0x800F2558);
    game_queue_ship_launch_message_native(format, 0x80059E70 + slot * 44);
    w_u32(object + 0x40, 0x800A3A98);
    w_u32(object + 0x9C, 10);
    return 1;
}

void game_update_deferred_ship_launch(uint32 object)
{
    uint32 carrier, stored_generation, generation, bay, yaw, pitch, roll, ship, x, y, z;
    uint32 created, list, slot, format, count;
    FUNCTION_MARKER(0x8009C6E4, "GAME.OVL");
    carrier = r_u32(object + 0x170);
    if (r_u32(carrier + 0x114) & 0x40000000) {
        game_mark_object_for_removal(object);
        return;
    }
    if (r_u32(carrier) == 0) {
        game_mark_object_for_removal(object);
        return;
    }
    stored_generation = r_u16(object + 0x174);
    generation = r_u16(carrier + 0x24A);
    if (stored_generation != generation) {
        game_mark_object_for_removal(object);
        return;
    }
    bay = game_select_carrier_launch_bay(carrier);
    if (bay == 0)
        return;
    yaw = r_u32(carrier + 0x20);
    pitch = r_u32(carrier + 0x24);
    roll = r_u32(carrier + 0x28);
    ship = r_u32(object + 0xA8);
    x = r_u32(carrier + 8);
    y = r_u32(carrier + 12);
    z = r_u32(carrier + 16);
    created = game_spawn_script_ship(ship, x, y, z, yaw, pitch, roll);
    list = r_u32(object + 0xAC);
    game_assign_ship_task_list(created, list);
    game_initialize_ship_launch(created, carrier, bay);
    slot = (uint32)r_s16(created + 0x24E);
    format = r_u32(0x800F2558);
    game_queue_ship_launch_message_native(format, 0x80059E70 + slot * 44);
    count = r_u32(object + 0xB0) - 1;
    w_u32(object + 0xB0, count);
    if ((sint32)count <= 0)
        game_mark_object_for_removal(object);
}

uint32 game_ship_task_15(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, own_id, carrier, ship, desired, address, count, category, existing, controller;
    uint32 generation, list, value;
    FUNCTION_MARKER(0x8009C4DC, "GAME.OVL");
    if (r_u32(object + 0x114) & 0x8000)
        return 0;
    if (r_u32(0x800EC884) != 0)
        return 0;
    identifier = game_get_ship_task_parameter(task, 2);
    own_id = r_u32(object + 4);
    if (identifier == own_id || identifier - 1000 == own_id)
        carrier = object;
    else
        carrier = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0);
    if (carrier == 0 || r_s16(carrier + 0x1C4) == 0)
        return 0;
    ship = game_get_ship_task_parameter(task, 3);
    desired = game_get_ship_task_parameter(task, 5);
    address = *cursor;
    count = r_u32(address);
    if (count == 0)
        count = 1;
    category = r_u32(0x80053698 + ship * 60);
    existing = game_count_script_ship_objects(ship, category);
    if ((sint32)existing >= (sint32)desired && desired != 0)
        return 0;
    if (desired != 0)
        count = desired - existing;
    controller = game_create_object(0, 0, 0, 0, 0);
    w_u32(controller + 0x40, 0x8009C6E4);
    w_u32(controller + 0x170, carrier);
    generation = r_u16(carrier + 0x24A);
    w_u32(controller + 0xA8, ship);
    w_u16(controller + 0x174, generation);
    list = game_get_ship_task_parameter(task, 4);
    w_u32(controller + 0xAC, list);
    w_u32(controller + 0xB0, count);
    w_u8(controller + 0x26B, 30);
    w_u8(controller + 0x26C, 30);
    if (game_get_ship_task_parameter(task, 1) != 0) {
        address = *cursor;
        value = r_u32(address);
        w_u32(address, value - count);
        address = *cursor;
        if (r_s32(address) <= 0)
            game_skip_ship_task(object, ordinal);
    }
    return 0;
}

uint32 game_ship_task_20(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 ticks, flags, controller, generation, value, offset, slot, format;
    FUNCTION_MARKER(0x8009CE50, "GAME.OVL");
    if (r_u32(object + 0x114) & 0x8000)
        return 0;
    ticks = game_get_ship_task_parameter(task, 2) * 30;
    if (ticks == 0) {
        flags = r_u32(object + 0x114);
        w_u32(object + 0x3C, 0x8002493C);
        w_u32(object + 0x114, flags | 0x8000);
    } else {
        controller = game_create_object(0, 0, 0, 0, 0);
        w_u32(controller + 0x40, 0x800A5138);
        w_u32(controller + 0x9C, ticks);
        w_u32(controller + 0x170, object);
        generation = r_u16(object + 0x24A);
        w_u32(controller + 0xA8, 2);
        w_u32(controller + 0xAC, 255);
        w_u16(controller + 0x174, generation);
        for (offset = 0xB0; offset <= 0xC4; offset += 4) {
            value = (uint32)random_between(1, 80);
            w_u32(controller + offset, value);
        }
        slot = (uint32)r_s16(object + 0x24E);
        format = r_u32(0x800F255C);
        game_queue_ship_launch_message_native(format, 0x80059E70 + slot * 44);
    }
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_21(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 selection, group, range, identifier, target, tx, x, ty, y, tz, z, distance;
    uint32 flags, offset, value, slot, format;
    FUNCTION_MARKER(0x8009CFD8, "GAME.OVL");
    if (!(r_u32(object + 0x114) & 0x8000))
        return 0;
    selection = game_get_ship_task_parameter(task, 2);
    group = game_get_ship_task_parameter(task, 3);
    range = game_get_ship_task_parameter(task, 5);
    if (range == 0)
        range = 0x07FFFFFF;
    if (selection == 0) {
        identifier = game_get_ship_task_parameter(task, 4);
        if (identifier == 0)
            target = r_u32(0x80040FDC);
        else
            target = game_find_script_ship_near(object, identifier, range, 0);
    } else if (selection == 3) {
        if (group == 3)
            target = game_find_nearest_type_three(object, 0);
        else
            target = game_find_nearest_type_one(object, 0);
    } else {
        target = game_find_nearest_object(object, selection == 2 ? 1 : 0, group, 0);
    }
    if (target == 0)
        return 0;
    tx = r_u32(target + 8);
    x = r_u32(object + 8);
    ty = r_u32(target + 12);
    y = r_u32(object + 12);
    tz = r_u32(target + 16);
    z = r_u32(object + 16);
    distance = (uint32)vector_length_approx((sint32)(tx - x), (sint32)(ty - y), (sint32)(tz - z));
    w_u32(0x80046174, distance);
    if ((sint32)distance >= (sint32)range)
        return 0;
    game_reposition_cloaked_ship(object);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x3C, 0x800D1C3C);
    w_u32(object + 0x40, 0x800A52D0);
    w_u32(object + 0xA8, 2);
    w_u32(object + 0xAC, 0);
    w_u32(object + 0x114, flags ^ 0x8000);
    for (offset = 0xB0; offset <= 0xC4; offset += 4) {
        value = (uint32)random_between(1, 80);
        w_u32(object + offset, value);
    }
    slot = (uint32)r_s16(object + 0x24E);
    format = r_u32(0x800F2560);
    game_queue_ship_launch_message_native(format, 0x80059E70 + slot * 44);
    game_skip_ship_task(object, ordinal);
    return 1;
}

uint32 game_ship_task_22(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, address;
    FUNCTION_MARKER(0x8009D254, "GAME.OVL");
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x10000);
    address = *cursor;
    if (r_s32(address) <= 0)
        game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_23(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 actor, ticks, controller, generation, address;
    FUNCTION_MARKER(0x8009D29C, "GAME.OVL");
    actor = r_u32(object + 0x110);
    w_u8(actor + 0x46F, 0);
    ticks = game_get_ship_task_parameter(task, 2) * 30;
    if ((sint32)ticks > 0) {
        controller = game_create_object(1, 0, 0, 0, 0);
        w_u32(controller + 0x40, 0x8009D360);
        w_u32(controller + 0x170, object);
        generation = r_u16(object + 0x24A);
        w_u32(controller + 0xA8, ticks);
        w_u16(controller + 0x174, generation);
    }
    address = *cursor;
    if (r_s32(address) <= 0)
        game_skip_ship_task(object, ordinal);
    return 0;
}

void game_update_actor_flag_delay(uint32 object)
{
    uint32 target, stored_generation, generation, ticks, actor;
    FUNCTION_MARKER(0x8009D360, "GAME.OVL");
    target = r_u32(object + 0x170);
    if (r_u32(target) != 0) {
        stored_generation = r_u16(object + 0x174);
        generation = r_u16(target + 0x24A);
        if (stored_generation == generation) {
            ticks = r_u32(object + 0xA8) - 1;
            w_u32(object + 0xA8, ticks);
            if ((sint32)ticks > 0)
                return;
            actor = r_u32(target + 0x110);
            w_u8(actor + 0x46F, 1);
        }
    }
    game_mark_object_for_removal(object);
}

void game_reposition_cloaked_ship(uint32 object)
{
    uint32 model, x, y, z, table, flags, radius, scale, attempt = 0, group, candidate;
    uint32 tx, ox, ty, oy, oz, tz, distance, clear;
    sint16 classes[2] = {1, 3};
    FUNCTION_MARKER(0x800AAC84, "GAME.OVL");
    model = r_u32(object + 0x74);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    table = r_u32(0x80046178) + model * 132;
    flags = r_u32(object + 0x114);
    radius = r_u32(table + 0x6C);
    if (flags & 0x10) {
        scale = r_u32(object + 0x84);
        if ((sint32)scale < 0)
            scale = 0 - scale;
        radius <<= scale & 31;
    }
    for (;;) {
        clear = 1;
        for (group = 0; group < 2; ++group) {
            candidate = object_lookup((uint32)classes[group]);
            while (candidate != 0) {
                if (!(r_u32(candidate + 0x114) & 0x10)) {
                    tx = r_u32(candidate + 8);
                    ox = r_u32(object + 8);
                    ty = r_u32(candidate + 12);
                    oy = r_u32(object + 12);
                    oz = r_u32(object + 16);
                    tz = r_u32(candidate + 16);
                    distance = (uint32)vector_length_approx((sint32)(tx - ox), (sint32)(ty - oy), (sint32)(tz - oz));
                    w_u32(0x80046174, distance);
                    if ((sint32)distance < (sint32)radius)
                        clear = 0;
                }
                candidate = r_u32(candidate + 0x1DC);
            }
        }
        if (clear != 0)
            return;
        ++attempt;
        if ((sint32)attempt >= 10)
            return;
        w_u32(object + 8, x);
        w_u32(object + 12, y + 20000);
        w_u32(object + 16, z);
    }
}

uint32 game_ship_task_24(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, current, target, generation, callback;
    FUNCTION_MARKER(0x8009D3CC, "GAME.OVL");
    if (!(r_u16(object + 0x264) & 1)) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075B9C, 0x80075A1C);
    }
    current = r_u32(object + 0xE0);
    w_u32(0x800EE9D8, 1);
    if (current == ordinal && game_validate_ship_attack_target(object) != 0) {
        callback = r_u32(object + 0x54);
        w_u32(object + 0x40, callback);
        return 1;
    }
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0);
    if (target == 0)
        return 0;
    if (target == object) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075A20, 0x80075A1C);
    }
    w_u32(object + 0x178, target);
    generation = r_u16(target + 0x24A);
    callback = r_u32(object + 0x54);
    w_u16(object + 0x17C, generation);
    w_u32(object + 0x40, callback);
    return 1;
}

uint32 game_ship_task_25(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 selection, group, identifier, threshold, target, count, pods;
    FUNCTION_MARKER(0x8009D4DC, "GAME.OVL");
    if (r_u8(object + 0x26E) == 1) {
        game_skip_ship_task(object, ordinal);
        return 0;
    }
    selection = game_get_ship_task_parameter(task, 2);
    group = game_get_ship_task_parameter(task, 3);
    identifier = game_get_ship_task_parameter(task, 4);
    threshold = game_get_ship_task_parameter(task, 5);
    target = game_select_ship_task_target(object, selection, group, identifier, 0);
    count = target != 0 ? game_count_matching_active_ship_objects(target) : 0;
    if ((sint32)threshold < (sint32)count)
        return 0;
    pods = game_get_ship_task_parameter(task, 6);
    w_u32(object + 0xA8, pods);
    w_u32(object + 0x40, 0x800A5B5C);
    w_u8(object + 0x26E, 1);
    w_u32(object + 0xE8, 0);
    game_skip_ship_task(object, ordinal);
    return 1;
}

uint32 game_find_nearest_object_in_range(uint32 object, uint32 category, uint32 range, uint32 excluded_flags)
{
    uint32 candidate, best = 0, x, cx, y, cy, cz, z, distance, flags;
    FUNCTION_MARKER(0x800AE434, "GAME.OVL");
    candidate = object_lookup(category);
    while (candidate != 0) {
        x = r_u32(object + 8);
        cx = r_u32(candidate + 8);
        y = r_u32(object + 12);
        cy = r_u32(candidate + 12);
        cz = r_u32(candidate + 16);
        z = r_u32(object + 16);
        distance = (uint32)vector_length_approx((sint32)(x - cx), (sint32)(y - cy), (sint32)(z - cz));
        w_u32(0x80046174, distance);
        if ((sint32)distance < (sint32)range) {
            flags = r_u32(candidate + 0x114);
            if (!(flags & 0x8000) && !(flags & excluded_flags)) {
                range = distance;
                best = candidate;
            }
        }
        candidate = r_u32(candidate + 0x1DC);
    }
    return best;
}

uint32 game_ship_task_26(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 range, target, slot, format, flags, offset, value;
    FUNCTION_MARKER(0x8009D600, "GAME.OVL");
    if (!(r_u32(object + 0x114) & 0x8000))
        return 0;
    range = game_get_ship_task_parameter(task, 2);
    if (range == 0)
        range = 0x07FFFFFF;
    target = game_find_nearest_object_in_range(object, 7, range, 0);
    if (target == 0)
        return 0;
    game_reposition_cloaked_ship(object);
    slot = (uint32)r_s16(object + 0x24E);
    format = r_u32(0x800F2560);
    game_queue_ship_launch_message_native(format, 0x80059E70 + slot * 44);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x3C, 0x800D1C3C);
    w_u32(object + 0x40, 0x800A52D0);
    w_u32(object + 0xA8, 2);
    w_u32(object + 0xAC, 0);
    w_u32(object + 0x114, flags ^ 0x8000);
    for (offset = 0xB0; offset <= 0xC4; offset += 4) {
        value = (uint32)random_between(1, 80);
        w_u32(object + offset, value);
    }
    game_skip_ship_task(object, ordinal);
    return 1;
}

uint32 game_ship_task_27(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 slot;
    FUNCTION_MARKER(0x8009D770, "GAME.OVL");
    slot = (uint32)r_s16(object + 0x24E);
    w_u16(0x80059E8E + slot * 44, 1);
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_28(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, state, target, stored_generation, generation, callback;
    FUNCTION_MARKER(0x8009D7C0, "GAME.OVL");
    flags = r_u32(object + 0x114);
    if (flags & 0x20000) {
        state = r_u32(object + 0x208);
        w_u32(object + 0x114, flags ^ 0x20000);
        if (state != 0) {
            target = r_u32(state + 0x38);
            if (target != 0) {
                stored_generation = r_u16(state + 0x3C);
                generation = r_u16(target + 0x24A);
                if (stored_generation == generation) {
                    flags = r_u32(target + 0x114);
                    w_u32(target + 0x114, flags ^ 0x40000);
                    target = r_u32(state + 0x38);
                    if (r_u32(target + 0xDC) == 0) {
                        callback = r_u32(state + 0x44);
                        w_u32(target + 0x40, callback);
                    } else {
                        w_u32(target + 0x40, 0x8009AB58);
                    }
                    target = r_u32(state + 0x38);
                    callback = r_u32(state + 0x48);
                    w_u32(target + 0x3C, callback);
                    target = r_u32(state + 0x38);
                    callback = r_u32(state + 0x4C);
                    w_u32(target + 0x44, callback);
                }
            }
        }
    }
    game_skip_ship_task(object, ordinal);
    return 0;
}

void game_stop_and_launch_escape_pods(uint32 object)
{
    uint32 index, count;
    FUNCTION_MARKER(0x800A5B5C, "GAME.OVL");
    w_u32(object + 0xE8, 0);
    game_update_object_state(object);
    game_update_object_movement(object);
    if (r_u32(object + 0xEC) != 0)
        return;
    count = r_u32(object + 0xA8);
    if ((sint32)count <= 0)
        return;
    index = 0;
    do {
        game_create_escape_pod(object);
        count = r_u32(object + 0xA8);
        ++index;
        w_u32(object + 0x40, 0x8009AB58);
    } while ((sint32)index < (sint32)count);
}

uint32 game_ship_task_29(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, identifier, target, waypoint, range, tx, x, ty, y, z, tz, distance;
    uint32 state, address, value, generation, callback;
    FUNCTION_MARKER(0x8009D88C, "GAME.OVL");
    flags = r_u32(object + 0x114);
    if ((flags & 0x8000) || (flags & 0x20000))
        return 0;
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0x40000);
    if (target == 0)
        return 0;
    waypoint = game_get_ship_task_parameter(task, 3);
    if ((sint32)waypoint >= r_s32(0x800EDB34) || (sint32)waypoint <= 0)
        diagnostic_report(waypoint, 0x80075A88, 0x80075A1C);
    range = game_get_ship_task_parameter(task, 4);
    tx = r_u32(target + 8);
    x = r_u32(object + 8);
    ty = r_u32(target + 12);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    tz = r_u32(target + 16);
    distance = (uint32)vector_length_approx((sint32)(tx - x), (sint32)(ty - y), (sint32)(tz - z));
    w_u32(0x80046174, distance);
    if ((sint32)distance >= (sint32)range)
        return 0;
    if (r_u32(object + 0xE0) != ordinal)
        game_report_tractor_capture(object, target);
    if (r_u32(object + 0x208) == 0) {
        state = heap_alloc_back(80);
        w_u32(object + 0x208, state);
    }
    address = 0x80126D68 + waypoint * 12;
    state = r_u32(object + 0x208);
    value = r_u32(address);
    w_u32(state, value);
    value = r_u32(address + 4);
    w_u32(state + 4, value);
    value = r_u32(address + 8);
    w_u32(state + 8, value);
    if (r_u32(object + 0x114) & 0x10) {
        game_get_target_vertex_vector(object, target, state + 0x10);
    } else {
        w_u32(state + 0x10, 0);
        w_u32(state + 0x14, 0);
        w_u32(state + 0x18, 0);
    }
    if (r_u32(target + 0x114) & 0x10) {
        game_get_target_vertex_vector(target, object, state + 0x20);
    } else {
        w_u32(state + 0x20, 0);
        w_u32(state + 0x24, 0);
        w_u32(state + 0x28, 0);
    }
    w_u32(state + 0x38, target);
    generation = r_u16(target + 0x24A);
    distance = r_u32(0x80046174);
    w_u32(state + 0x30, range);
    w_u16(state + 0x40, 1);
    w_u16(state + 0x3C, generation);
    w_u32(state + 0x34, distance);
    flags = r_u32(object + 0x114);
    w_u32(object + 0xA8, 0x07FFFFFF);
    w_u32(object + 0x114, flags | 0x20000);
    value = game_get_ship_task_parameter(task, 5);
    w_u32(object + 0xAC, value);
    w_u32(object + 0x88, 0);
    w_u32(object + 0x40, 0x800A61B8);
    callback = r_u32(target + 0x40);
    w_u32(state + 0x44, callback);
    callback = r_u32(target + 0x3C);
    w_u32(target + 0x40, 0x8002493C);
    w_u32(state + 0x48, callback);
    callback = r_u32(target + 0x44);
    w_u32(state + 0x4C, callback);
    flags = r_u32(target + 0x114);
    w_u16(target + 0x252, 0);
    w_u16(target + 0x250, 0);
    w_u32(target + 0xE8, 0);
    w_u32(target + 0xEC, 0);
    w_u32(target + 0x114, flags | 0x40000);
    game_skip_ship_task(object, ordinal);
    return 1;
}

uint32 game_ship_task_30(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, identifier, target, range, tx, x, ty, y, z, tz, distance, bay, state, value, generation;
    FUNCTION_MARKER(0x8009DB6C, "GAME.OVL");
    flags = r_u32(object + 0x114);
    if (flags & 0x8000)
        return 0;
    if (!(flags & 0x10)) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075BC4, 0x80075A1C);
    }
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0x40000);
    range = game_get_ship_task_parameter(task, 3);
    if (range == 0)
        range = 0x07FFFFFF;
    if (target == 0 || r_u32(target) == 0)
        return 0;
    tx = r_u32(target + 8);
    x = r_u32(object + 8);
    ty = r_u32(target + 12);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    tz = r_u32(target + 16);
    distance = (uint32)vector_length_approx((sint32)(tx - x), (sint32)(ty - y), (sint32)(tz - z));
    w_u32(0x80046174, distance);
    if ((sint32)distance >= (sint32)range)
        return 0;
    bay = game_find_free_carrier_bay_near_target(object, target);
    if (bay == 0)
        return 0;
    game_report_tractor_capture(object, target);
    w_u32(object + 0x40, 0x800A6478);
    w_u16(bay, 30);
    state = r_u32(object + 0x208);
    w_u32(object + 0xA8, bay);
    w_u32(object + 0x88, 1);
    if (state == 0) {
        state = heap_alloc_back(80);
        w_u32(object + 0x208, state);
    }
    state = r_u32(object + 0x208);
    value = r_u32(bay + 0x24);
    w_u32(state + 0x10, value);
    value = r_u32(bay + 0x28);
    w_u32(state + 0x14, value);
    value = r_u32(bay + 0x2C);
    w_u32(state + 0x18, value);
    if (r_u32(target + 0x114) & 0x10) {
        game_get_target_vertex_vector(target, object, state + 0x20);
    } else {
        w_u32(state + 0x20, 0);
        w_u32(state + 0x24, 0);
        w_u32(state + 0x28, 0);
    }
    w_u32(state + 0x38, target);
    generation = r_u16(target + 0x24A);
    w_u16(state + 0x40, 1);
    w_u16(state + 0x3C, generation);
    value = r_u32(target + 0x40);
    w_u32(state + 0x44, value);
    value = r_u32(target + 0x3C);
    w_u32(target + 0x40, 0x8002493C);
    w_u32(state + 0x48, value);
    value = r_u32(target + 0x44);
    w_u32(state + 0x4C, value);
    flags = r_u32(target + 0x114);
    w_u16(target + 0x252, 0);
    w_u16(target + 0x250, 0);
    w_u32(target + 0xE8, 0);
    w_u32(target + 0xEC, 0);
    w_u32(target + 0x114, flags | 0x40000);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x20000);
    return 1;
}

uint32 game_ship_task_31(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 selection, group, identifier, target;
    FUNCTION_MARKER(0x8009DDA4, "GAME.OVL");
    selection = game_get_ship_task_parameter(task, 2);
    group = game_get_ship_task_parameter(task, 3);
    identifier = game_get_ship_task_parameter(task, 4);
    target = game_select_ship_task_target(object, selection, group, identifier, 0);
    if (target == 0)
        return 0;
    w_u32(object + 0x40, 0x800A3AE0);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x252, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x9C, 30);
    return 1;
}

uint32 game_ship_task_32(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, identifier, speed, slot, format;
    FUNCTION_MARKER(0x8009DE54, "GAME.OVL");
    w_u32(object + 0x9C, 30);
    w_u32(object + 0x40, 0x800A7864);
    flags = r_u32(object + 0x114);
    identifier = r_u32(object + 4);
    w_u32(object + 0x88, 0);
    w_u32(object + 0x114, flags | 0x200000);
    speed = r_u32(0x8005369C + identifier * 60);
    slot = (uint32)r_s16(object + 0x24E);
    w_u32(object + 0xAC, 0x07FFFFFF);
    w_u32(object + 0xE8, speed);
    format = r_u32(0x800F2564);
    game_queue_ship_launch_message_native(format, 0x80059E70 + slot * 44);
    return 1;
}

uint32 game_ship_task_33(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 value, identifier;
    sint32 maximum;
    FUNCTION_MARKER(0x8009DF18, "GAME.OVL");
    value = game_get_ship_task_parameter(task, 2);
    w_u32(object + 0x23C, value);
    if (value == 0 && r_s32(object + 0x238) <= 0) {
        identifier = r_u32(object + 4);
        maximum = r_s32(0x800536A8 + identifier * 60);
        w_u32(object + 0x238, maximum < 101 ? 10 : (uint32)(maximum >> 3));
    }
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_34(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, flags, slot, format;
    FUNCTION_MARKER(0x8009DFB8, "GAME.OVL");
    if (r_s16(0x800595F2) == 0) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075BDC, 0x80075A1C);
        return 0;
    }
    w_u32(object + 0x40, 0x800A8D44);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x88, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x114, flags | 0x200000);
    slot = (uint32)r_s16(object + 0x24E);
    format = r_u32(0x800F2568);
    game_queue_ship_launch_message_native(format, 0x80059E70 + slot * 44);
    return 1;
}

uint32 game_ship_task_35(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier;
    FUNCTION_MARKER(0x8009E078, "GAME.OVL");
    if (r_u8(object + 0x270) == 1) {
        w_u8(object + 0x270, 255);
    } else {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075BF4, 0x80075A1C);
    }
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_36(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 mode, flags, address;
    FUNCTION_MARKER(0x8009E0EC, "GAME.OVL");
    mode = game_get_ship_task_parameter(task, 2);
    flags = r_u32(object + 0x114);
    if (flags & 0x01000000) {
        if (mode == 1)
            w_u32(object + 0x114, flags ^ 0x01000000);
    } else if (mode == 0) {
        w_u32(object + 0x114, flags | 0x01000000);
    }
    address = *cursor;
    if (r_s32(address) <= 0)
        game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_37(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 selection, group, identifier, target, generation, flags;
    FUNCTION_MARKER(0x8009E190, "GAME.OVL");
    selection = game_get_ship_task_parameter(task, 2);
    group = game_get_ship_task_parameter(task, 3);
    identifier = game_get_ship_task_parameter(task, 4);
    target = game_select_ship_task_target(object, selection, group, identifier, 0);
    if (target != 0) {
        w_u32(object + 0x188, target);
        generation = r_u16(target + 0x24A);
        w_u16(object + 0x18C, generation);
        flags = r_u32(object + 0x114);
        w_u32(object + 0x114, flags | 0x02000000);
    } else {
        flags = r_u32(object + 0x114);
        if (flags & 0x02000000)
            w_u32(object + 0x114, flags ^ 0x02000000);
    }
    return 0;
}

uint32 game_ship_task_38(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, x, y, z, state;
    FUNCTION_MARKER(0x8009E24C, "GAME.OVL");
    if (r_u32(object + 0x114) & 0x10) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075C28, 0x80075A1C);
    }
    if (r_u32(object + 0xE0) != ordinal) {
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        state = r_u32(object + 0x200);
        w_u32(object + 0x214, x);
        w_u32(object + 0x218, y);
        w_u32(object + 0x21C, z);
        w_u32(state + 0x18, 0);
    }
    w_u32(object + 0x9C, 90);
    w_u32(object + 0x40, 0x800AAE44);
    return 1;
}

uint32 game_ship_task_39(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, model;
    FUNCTION_MARKER(0x8009E2F0, "GAME.OVL");
    identifier = r_u32(object + 4);
    model = (uint32)r_s16(0x800536B8 + identifier * 60);
    if (model != r_u32(0x800ED498))
        diagnostic_report(identifier, 0x80075C4C, 0x80075A1C);
    w_u32(object + 0x40, 0x800AB064);
    return 1;
}
