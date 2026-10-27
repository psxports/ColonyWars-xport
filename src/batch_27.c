#include "game_functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void game_apply_script_ship_properties(uint32 object)
{
    uint32 offset, value, model, table, slot;
    FUNCTION_MARKER(0x800969A4, "GAME.OVL");
    offset = r_u32(object + 4) * 60;
    value = r_u32(0x800536A8 + offset);
    model = r_u32(object + 0x74);
    w_u32(object + 0x238, value);
    value = r_u32(0x800536AC + offset);
    w_u32(object + 0xEC, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x244, value);
    value = r_u16(0x800536B4 + offset);
    w_u32(object + 0x110, 0);
    w_u16(object + 0x260, value);
    value = r_u16(0x800536B0 + offset);
    w_u16(object + 0x262, value);
    value = r_u32(0x800536A4 + offset);
    w_u32(object + 0x234, value);
    value = (uint32)r_s16(0x800536B2 + offset);
    w_u32(object + 0xF0, value);
    value = (uint32)r_s16(0x800536B6 + offset);
    w_u32(object + 0x84, value);
    table = r_u32(0x80046178);
    if (r_u32(table + model * 132 + 4) != 0)
        w_u32(object + 0x114, r_u32(object + 0x114) | 0x800);
    value = r_u16(0x800536BA + offset);
    w_u16(object + 0x24E, value);
    slot = (uint32)(sint32)(sint16)value * 44;
    w_u32(0x80059E68 + slot, object);
    slot = (uint32)r_s16(object + 0x24E) * 44;
    value = r_u16(object + 0x24A);
    w_u16(0x80059E6C + slot, value);
}

void game_tick_script_ship_start(uint32 object)
{
    uint32 remaining;
    FUNCTION_MARKER(0x80096B18, "GAME.OVL");
    remaining = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, remaining);
    if ((sint32)remaining <= 0)
        game_begin_script_ship_task(object);
}

void game_script_ship_attack_error(uint32 object)
{
    FUNCTION_MARKER(0x80097198, "GAME.OVL");
    diagnostic_report(r_u32(object + 4), 0x800752B8, 0x80074F80);
}

void game_script_ship_avoid_error(uint32 object)
{
    FUNCTION_MARKER(0x800971CC, "GAME.OVL");
    diagnostic_report(r_u32(object + 4), 0x800752C8, 0x80074F80);
}

void game_script_ship_death_error(uint32 object)
{
    FUNCTION_MARKER(0x80097200, "GAME.OVL");
    diagnostic_report(r_u32(object + 4), 0x800752D8, 0x80074F80);
}

void game_tick_script_events(void)
{
    uint32 event, index, target, completed;
    sint32 kind;
    FUNCTION_MARKER(0x800E43E0, "GAME.OVL");
    if (r_u32(0x800595C8) != 0)
        return;
    if (r_u32(0x800EEB24) != 0)
        return;
    if (r_s32(0x800595C4) <= 0)
        return;
    event = 0x80126C98;
    index = 0;
    do {
        kind = r_s16(event + 12);
        if (kind != -1) {
            target = r_u32(0x800F1808 + ((uint32)kind << 2));
            /* The original empty callback preserves its loaded target as the result */
            if (target == 0x8002493C)
                completed = target;
            else
                completed = psx_dispatch_guest_call(target, 1, event, 0, 0);
            if (completed != 0)
                w_u16(event + 12, 0xFFFF);
        }
        ++index;
        event += 20;
    } while ((sint32)index < r_s32(0x800595C4));
}

uint32 game_script_event_message(uint32 event)
{
    FUNCTION_MARKER(0x800E44F8, "GAME.OVL");
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_mission_success(uint32 event)
{
    FUNCTION_MARKER(0x800E4518, "GAME.OVL");
    if (r_s16(0x80059610) != 2)
        return 0;
    w_u32(0x800595C8, 1);
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_mission_failure(uint32 event)
{
    uint32 text, length, mission, chapter, message, record, sectors, asset;
    FUNCTION_MARKER(0x800E4560, "GAME.OVL");
    if (r_s16(0x80059610) != 1)
        return 0;
    if (r_u32(0x800595F8) != 0x30007) {
        w_u32(0x800595C8, 1);
        game_play_condition_message(event);
        return 1;
    }
    stream_clear_queue();
    message_init();
    text = 0x80108A50 + (uint32)r_s16(event + 16) * 265;
    length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    message_add((char *)psx_addr(text, (size_t)length + 1), 0, 15);
    chapter = (uint32)r_s16(0x800595FA) - 1;
    mission = (uint32)r_s16(0x800595F8);
    message = (uint32)r_s16(event + 14);
    record = (uint32)r_s16(0x800F15F4 + (mission * 7 + chapter) * 4);
    record = (message + record - 1) * 24;
    record += r_u32(0x800F2B08);
    sectors = r_u32(record + 0x14);
    asset = r_u32(record + 0x1C);
    stream_request_segments(record + 0x10, sectors, asset, 0x800118FE, 20001, 0, 0);
    return 1;
}

uint32 game_script_event_category_below(uint32 event)
{
    uint32 count;
    sint32 initial, kind, minimum, threshold;
    FUNCTION_MARKER(0x800E4690, "GAME.OVL");
    count = game_count_objects_by_category((uint32)r_s16(event), 1);
    kind = r_s16(event);
    initial = r_s16(kind == 3 ? 0x800595E8 : 0x800595EA);
    minimum = r_s32(event + 8);
    if (initial < minimum)
        return 0;
    threshold = r_s32(event + 4);
    if ((sint32)count >= threshold)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_count(uint32 event)
{
    uint32 offset;
    sint32 present, required;
    FUNCTION_MARKER(0x800E4720, "GAME.OVL");
    offset = (uint32)r_s16(event) * 60;
    present = r_s16(0x800536BE + offset);
    required = r_s32(event + 4);
    if (present < required)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_category_above(uint32 event)
{
    uint32 count;
    sint32 initial, kind, minimum, threshold;
    FUNCTION_MARKER(0x800E477C, "GAME.OVL");
    kind = r_s16(event);
    initial = r_s16(kind == 3 ? 0x800595E8 : 0x800595EA);
    count = game_count_objects_by_category((uint32)r_s16(event), 1);
    minimum = r_s32(event + 8);
    if (initial < minimum)
        return 0;
    threshold = r_s32(event + 4);
    if (threshold >= (sint32)count)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_reenabled(uint32 event)
{
    uint32 object, identifier, type, flags;
    sint32 state;
    FUNCTION_MARKER(0x800E4818, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    type = r_u32(0x80053698 + identifier * 60);
    object = object_lookup(type);
    if (object == 0)
        return 0;
    identifier = (uint32)r_s16(event);
    while (r_u32(object + 4) != identifier) {
        object = r_u32(object + 0x1DC);
        if (object == 0)
            return 0;
    }
    state = r_s16(event + 18);
    if (state == 0) {
        flags = r_u32(object + 0x114);
        if (flags & 0x8000)
            w_u16(event + 18, 1);
        return 0;
    }
    if (state != 1)
        return 0;
    if (r_u32(object + 0x114) & 0x8000)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_counter(uint32 event)
{
    uint32 player, identifier, object, count;
    sint32 required;
    FUNCTION_MARKER(0x800E491C, "GAME.OVL");
    player = r_u32(0x80040FDC);
    identifier = (uint32)r_s16(event);
    object = game_find_script_ship_near(player, identifier, 0x7FFFFFF, 0);
    if (object == 0)
        return 0;
    count = r_u8(object + 0x26F);
    required = r_s32(event + 4);
    if ((sint32)count < required)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_health_below(uint32 event)
{
    uint32 identifier, threshold, object;
    FUNCTION_MARKER(0x800E4990, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    threshold = r_u32(event + 4);
    object = game_find_script_ship_health_below(identifier, threshold);
    if (object == 0)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_population_below(uint32 event)
{
    uint32 identifier, type, count;
    sint32 required;
    FUNCTION_MARKER(0x800E49D8, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    type = r_u32(0x80053698 + identifier * 60);
    count = game_count_script_ship_objects(identifier, type);
    required = r_s32(event + 4);
    if (required <= (sint32)count)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_distance_below(uint32 event)
{
    uint32 player, identifier, object, x, y, z, px, py, pz, distance;
    sint32 required;
    FUNCTION_MARKER(0x800E4A48, "GAME.OVL");
    player = r_u32(0x80040FDC);
    identifier = (uint32)r_s16(event);
    object = game_find_script_ship_near(player, identifier, 0x7FFFFFF, 0);
    if (object == 0)
        return 0;
    player = r_u32(0x80040FDC);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    px = r_u32(player + 8);
    z = r_u32(object + 16);
    py = r_u32(player + 12);
    pz = r_u32(player + 16);
    distance = vector_length_approx(px - x, py - y, pz - z);
    required = r_s32(event + 4);
    w_u32(0x80046174, distance);
    if (required <= (sint32)distance)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_signal(uint32 event)
{
    uint32 identifier, type, object;
    FUNCTION_MARKER(0x800E4AE8, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    type = r_u32(0x80053698 + identifier * 60);
    object = object_lookup(type);
    while (object != 0) {
        identifier = (uint32)r_s16(event);
        if (r_u32(object + 4) == identifier && r_u8(object + 0x271) != 0) {
            game_play_condition_message(event);
            return 1;
        }
        object = r_u32(object + 0x1DC);
    }
    return 0;
}

uint32 game_script_event_ship_flag(uint32 event)
{
    uint32 player, identifier, object;
    FUNCTION_MARKER(0x800E4B88, "GAME.OVL");
    player = r_u32(0x80040FDC);
    identifier = (uint32)r_s16(event);
    object = game_find_script_ship_near(player, identifier, 0x7FFFFFF, 0);
    if (object == 0 || !(r_u32(object + 0x114) & 0x40000))
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_secondary_count(uint32 event)
{
    uint32 offset;
    sint32 present, required;
    FUNCTION_MARKER(0x800E4BF8, "GAME.OVL");
    offset = (uint32)r_s16(event) * 60;
    present = r_s16(0x800536C0 + offset);
    required = r_s32(event + 4);
    if (present < required)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_age(uint32 event)
{
    uint32 object, identifier, frame, duration, created;
    FUNCTION_MARKER(0x800E4C54, "GAME.OVL");
    object = object_lookup(3);
    if (object == 0)
        return 0;
    identifier = (uint32)r_s16(event);
    frame = r_u32(0x800311B8);
    do {
        if (r_u32(object + 4) == identifier) {
            duration = r_u32(event + 8) * 30;
            created = r_u32(object + 0x8C);
            if (created + duration < frame) {
                game_play_condition_message(event);
                return 1;
            }
        }
        object = r_u32(object + 0x1DC);
    } while (object != 0);
    return 0;
}

uint32 game_count_script_ship_objects(uint32 identifier, uint32 type)
{
    uint32 object, slot, current, generation, count = 0;
    FUNCTION_MARKER(0x800AE0BC, "GAME.OVL");
    if ((sint32)identifier >= 1000) {
        slot = (identifier - 1000) * 44;
        object = r_u32(0x80059E68 + slot);
        if (object == 0)
            return 0;
        current = r_u16(object + 0x24A);
        generation = r_u16(0x80059E6C + slot);
        if (current != generation)
            return 0;
        return (r_u32(object + 0x114) & 0x8000) == 0;
    }
    object = object_lookup(type);
    while (object != 0) {
        if (r_u32(object + 4) == identifier)
            ++count;
        object = r_u32(object + 0x1DC);
    }
    return count;
}

static uint32 game_find_script_ship_near_body(uint32 object, uint32 identifier, uint32 range, uint32 flags, uint32 *last_z_magnitude)
{
    uint32 candidate, best = 0, slot, current, generation, type;
    uint32 x, y, z, ox, oy, oz, distance, dz;
    if ((sint32)identifier >= 1000) {
        slot = (identifier - 1000) * 44;
        candidate = r_u32(0x80059E68 + slot);
        if (candidate == 0)
            return 0;
        current = r_u16(candidate + 0x24A);
        generation = r_u16(0x80059E6C + slot);
        if (current != generation || (r_u32(candidate + 0x114) & 0x8000))
            return 0;
        x = r_u32(candidate + 8);
        ox = r_u32(object + 8);
        y = r_u32(candidate + 12);
        oy = r_u32(object + 12);
        oz = r_u32(object + 16);
        z = r_u32(candidate + 16);
        distance = vector_length_approx(x - ox, y - oy, z - oz);
        if (last_z_magnitude != NULL) {
            dz = z - oz;
            *last_z_magnitude = (sint32)dz < 0 ? 0 - dz : dz;
        }
        if ((sint32)range < (sint32)distance)
            return 0;
        return (r_u32(candidate + 0x114) & flags) == 0 ? candidate : 0;
    }
    if (identifier == 0) {
        candidate = r_u32(0x80040FDC);
        ox = r_u32(object + 8);
        oy = r_u32(object + 12);
        x = r_u32(candidate + 8);
        oz = r_u32(object + 16);
        y = r_u32(candidate + 12);
        z = r_u32(candidate + 16);
        distance = vector_length_approx(x - ox, y - oy, z - oz);
        if (last_z_magnitude != NULL) {
            dz = z - oz;
            *last_z_magnitude = (sint32)dz < 0 ? 0 - dz : dz;
        }
        if ((sint32)range < (sint32)distance)
            return 0;
        return r_u32(0x80040FDC);
    }
    type = r_u32(0x80053698 + identifier * 60);
    ++range;
    candidate = object_lookup(type);
    while (candidate != 0) {
        if (r_u32(candidate + 4) == identifier && candidate != object && !(r_u32(candidate + 0x114) & 0x8000)) {
            x = r_u32(candidate + 8);
            ox = r_u32(object + 8);
            y = r_u32(candidate + 12);
            oy = r_u32(object + 12);
            oz = r_u32(object + 16);
            z = r_u32(candidate + 16);
            distance = vector_length_approx(x - ox, y - oy, z - oz);
            if (last_z_magnitude != NULL) {
                dz = z - oz;
                *last_z_magnitude = (sint32)dz < 0 ? 0 - dz : dz;
            }
            if ((sint32)distance < (sint32)range && !(r_u32(candidate + 0x114) & flags)) {
                range = distance;
                best = candidate;
            }
        }
        candidate = r_u32(candidate + 0x1DC);
    }
    return best;
}

uint32 game_find_script_ship_near(uint32 object, uint32 identifier, uint32 range, uint32 flags)
{
    FUNCTION_MARKER(0x800A2458, "GAME.OVL");
    return game_find_script_ship_near_body(object, identifier, range, flags, NULL);
}

uint32 game_find_script_ship_near_with_distance_input(uint32 object, uint32 identifier, uint32 range, uint32 flags, uint32 *last_z_magnitude)
{
    return game_find_script_ship_near_body(object, identifier, range, flags, last_z_magnitude);
}


uint32 game_find_script_ship_health_below(uint32 identifier, uint32 threshold)
{
    uint32 offset, type, object, maximum_first, maximum_second, current_first, current_second;
    uint32 denominator, numerator;
    sint32 percent;
    FUNCTION_MARKER(0x800AE76C, "GAME.OVL");
    offset = identifier * 60;
    type = r_u32(0x80053698 + offset);
    object = object_lookup(type);
    while (object != 0) {
        if (r_u32(object + 4) == identifier) {
            maximum_first = r_u32(0x800536A4 + offset);
            maximum_second = r_u32(0x800536A8 + offset);
            current_first = r_u32(object + 0x234);
            current_second = r_u32(object + 0x238);
            denominator = maximum_first + maximum_second;
            numerator = (current_first + current_second) * 100;
            if (denominator == 0 || (denominator == 0xFFFFFFFF && numerator == 0x80000000))
                abort();
            percent = (sint32)numerator / (sint32)denominator;
            if (percent < (sint32)threshold)
                return object;
        }
        object = r_u32(object + 0x1DC);
    }
    return 0;
}

uint32 game_script_event_elapsed_with_ship(uint32 event)
{
    uint32 duration, frame, started, identifier, player;
    FUNCTION_MARKER(0x800E4D10, "GAME.OVL");
    duration = r_u32(event + 8);
    frame = r_u32(0x800311B8);
    started = r_u32(0x800595B8);
    if (started + duration * 30 >= frame)
        return 0;
    identifier = (uint32)r_s16(event);
    if (identifier != 0) {
        player = r_u32(0x80040FDC);
        if (game_find_script_ship_near(player, identifier, 0x7FFFFFF, 0) == 0)
            return 0;
    }
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_primary_depleted(uint32 event)
{
    uint32 player, identifier, object;
    FUNCTION_MARKER(0x800E4DA4, "GAME.OVL");
    player = r_u32(0x80040FDC);
    identifier = (uint32)r_s16(event);
    object = game_find_script_ship_near(player, identifier, 0x7FFFFFF, 0);
    if (object == 0 || r_s32(object + 0x234) > 0)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_flag_count(uint32 event)
{
    uint32 identifier, type, object, count = 0, required;
    FUNCTION_MARKER(0x800E4E10, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    type = r_u32(0x80053698 + identifier * 60);
    object = object_lookup(type);
    while (object != 0) {
        identifier = (uint32)r_s16(event);
        if (r_u32(object + 4) == identifier && (r_u32(object + 0x114) & 0x40000000)) {
            required = r_u32(event + 4);
            ++count;
            if ((sint32)count >= (sint32)required) {
                game_play_condition_message(event);
                return 1;
            }
        }
        object = r_u32(object + 0x1DC);
    }
    return 0;
}

uint32 game_script_event_elapsed(uint32 event)
{
    uint32 duration, frame;
    FUNCTION_MARKER(0x800E4ED4, "GAME.OVL");
    duration = r_u32(event + 8);
    frame = r_u32(0x800311B8);
    if (duration * 30 >= frame)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_ratio_reached(uint32 event)
{
    uint32 identifier, offset, first, second, type, maximum, object, numerator, required;
    sint32 percent;
    FUNCTION_MARKER(0x800E4F20, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    offset = identifier * 60;
    first = r_u32(0x800536A4 + offset);
    second = r_u32(0x800536A8 + offset);
    type = r_u32(0x80053698 + offset);
    maximum = first + second;
    object = object_lookup(type);
    while (object != 0) {
        identifier = (uint32)r_s16(event);
        if (r_u32(object + 4) == identifier) {
            numerator = (uint32)r_s16(object + 0x248) * 100;
            if (maximum == 0 || (maximum == 0xFFFFFFFF && numerator == 0x80000000))
                abort();
            percent = (sint32)numerator / (sint32)maximum;
            required = r_u32(event + 4);
            if (percent >= (sint32)required) {
                game_play_condition_message(event);
                return 1;
            }
        }
        object = r_u32(object + 0x1DC);
    }
    return 0;
}

uint32 game_script_event_ship_idle_time(uint32 event)
{
    uint32 player, identifier, object, frame, last, duration;
    FUNCTION_MARKER(0x800E5034, "GAME.OVL");
    player = r_u32(0x80040FDC);
    identifier = (uint32)r_s16(event);
    object = game_find_script_ship_near(player, identifier, 0x7FFFFFF, 0);
    if (object == 0)
        return 0;
    frame = r_u32(0x800311B8);
    last = r_u32(object + 0xF8);
    duration = r_u32(event + 8);
    if (duration * 30 >= frame - last)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_type_seven_count(uint32 event)
{
    uint32 object, count = 0, required;
    FUNCTION_MARKER(0x800E50BC, "GAME.OVL");
    object = object_lookup(7);
    while (object != 0) {
        object = r_u32(object + 0x1DC);
        ++count;
    }
    required = r_u32(event + 4);
    if ((sint32)count < (sint32)required)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_task_flag_count(uint32 event)
{
    uint32 identifier, type, object, count = 0, required;
    FUNCTION_MARKER(0x800E52A8, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    type = r_u32(0x80053698 + identifier * 60);
    object = object_lookup(type);
    while (object != 0) {
        identifier = (uint32)r_s16(event);
        if (r_u32(object + 4) == identifier && (r_u32(object + 0x114) & 0x400000)) {
            required = r_u32(event + 4);
            ++count;
            if ((sint32)count >= (sint32)required) {
                game_play_condition_message(event);
                return 1;
            }
        }
        object = r_u32(object + 0x1DC);
    }
    return 0;
}

uint32 game_script_event_ship_slot_flag(uint32 event)
{
    uint32 player, identifier, object, slot;
    FUNCTION_MARKER(0x800E536C, "GAME.OVL");
    player = r_u32(0x80040FDC);
    identifier = (uint32)r_s16(event);
    object = game_find_script_ship_near(player, identifier, 0x7FFFFFF, 0);
    if (object == 0)
        return 0;
    slot = (uint32)r_s16(object + 0x24E) * 44;
    if (r_s16(0x80059E8E + slot) == 0)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_category_flag_count(uint32 event)
{
    uint32 object, count = 0, required;
    FUNCTION_MARKER(0x800E5400, "GAME.OVL");
    object = object_lookup((uint32)r_s16(event));
    while (object != 0) {
        if ((r_u32(object + 0x114) & 0x40000010) == 0x40000010)
            ++count;
        object = r_u32(object + 0x1DC);
    }
    required = r_u32(event + 4);
    if ((sint32)count < (sint32)required)
        return 0;
    game_play_condition_message(event);
    return 1;
}

uint32 game_script_event_ship_count_with_type_three(uint32 event)
{
    uint32 identifier, object, count = 0, required;
    sint32 present;
    FUNCTION_MARKER(0x800E5494, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    present = r_s16(0x800536BE + identifier * 60);
    object = object_lookup(3);
    while (object != 0) {
        object = r_u32(object + 0x1DC);
        ++count;
    }
    if (count == 0)
        return 0;
    required = r_u32(event + 4);
    if (present < (sint32)required)
        return 0;
    game_play_condition_message(event);
    return 1;
}

void game_initialize_training_controller(void)
{
    uint32 scenario, object, offset;
    FUNCTION_MARKER(0x800E8FA8, "GAME.OVL");
    if (r_s16(0x800595F8) != 0)
        return;
    scenario = (uint32)(sint32)(sint16)(r_u16(0x800595FA) - 1);
    w_u32(0x800F18B4, 0);
    w_u32(0x800F18AC, 1);
    if (scenario >= 6)
        return;
    switch (scenario) {
    case 0:
        object = game_create_object(0, 0, 0, 0, 0);
        matrix_set_identity(object + 0x11C);
        w_u32(object + 0x9C, 150);
        w_u32(object + 0x40, 0x800E80C4);
        break;
    case 1:
        game_load_bound_bundle(0x80076B60);
        w_u32(0x800F18A8, 1);
        w_u32(0x800F18B0, 1);
        w_u32(0x800F18A0, 0);
        w_u32(0x800F18A4, 0);
        w_u32(0x800F18B8, 0);
        object = game_create_object(0, 0, 0, 0, 0);
        w_u32(object + 0x40, 0x800E8114);
        w_u32(object + 0xA8, 0xFFFFFFFF);
        for (offset = 0; offset < 60 * 12; offset += 12) {
            w_u32(0x80124930 + offset, 0);
            w_u32(0x80124934 + offset, 0);
            w_u32(0x80124938 + offset, 0);
        }
        break;
    case 2:
        w_u32(0x800F18D8, 1);
        object = game_create_object(0, 0, 0, 0, 0);
        w_u32(object + 0x40, 0x800E8228);
        w_u32(object + 0xA8, 0);
        w_u32(object + 0xAC, 0);
        w_u32(object + 0xB0, 0);
        w_u32(object + 0xB4, 0);
        w_u32(object + 0xB8, 0);
        w_u32(object + 0xBC, 0);
        break;
    case 3:
    case 4:
        object = game_create_object(0, 0, 0, 0, 0);
        w_u32(object + 0x40, 0x800E89D0);
        w_u32(object + 0xA8, 0);
        w_u32(object + 0xAC, scenario == 3 ? 5 : 3);
        break;
    case 5:
        object = game_create_object(0, 0, 0, 0, 0);
        w_u32(object + 0x40, 0x800E8C64);
        w_u32(object + 0xA8, 0);
        w_u32(object + 0xAC, 3);
        w_u32(object + 0xB0, 0);
        w_u32(object + 0xB4, 0);
        w_u32(object + 0xB8, 0);
        w_u32(object + 0xBC, 0);
        break;
    }
}

uint32 game_create_gradient_strip_set(uint32 first, uint32 second, uint32 third)
{
    uint32 bank, section, index, slot, packet, count;
    uint32 colors[5];
    FUNCTION_MARKER(0x800E25DC, "GAME.OVL");
    count = r_u32(0x800F15E8);
    if ((sint32)count >= 5)
        return 0;
    colors[0] = 0;
    colors[1] = first;
    colors[2] = second;
    colors[3] = third;
    colors[4] = 0;
    for (bank = 0; bank < 2; ++bank) {
        slot = r_u32(0x800F15E8);
        packet = r_u32(0x801193C8 + bank * 20 + slot * 4);
        for (section = 0; section < 4; ++section) {
            for (index = 0; index < 32; ++index) {
                w_u32(packet + 4, colors[section]);
                w_u32(packet + 12, colors[section]);
                w_u32(packet + 20, colors[section + 1]);
                w_u32(packet + 28, colors[section + 1]);
                SetPolyG4((POLY_G4 *)psx_addr(packet, 36));
                SetSemiTrans(psx_addr(packet, 8), 1);
                packet += 36;
            }
        }
    }
    count = r_u32(0x800F15E8);
    w_u32(0x800F15E8, count + 1);
    return count;
}

void game_initialize_configured_objects(void)
{
    uint32 index = 0, offset = 0, model, x, y, z, object, value, color_offset, first, second, third;
    FUNCTION_MARKER(0x800A337C, "GAME.OVL");
    if (r_s32(0x800595C0) <= 0)
        return;
    do {
        model = r_u32(0x801076E8 + offset);
        x = r_u32(0x801076EC + offset);
        y = r_u32(0x801076F0 + offset);
        z = r_u32(0x801076F4 + offset);
        object = game_create_object(model, x, y, z, 0);
        w_u32(0x801076F8 + offset, object);
        w_u32(object + 0x3C, 0x800C682C);
        w_u32(object + 0x70, 10);
        w_u32(object, 12345);
        w_u32(object + 0xA4, 0x1140);
        w_u32(object + 0xD4, 0x808080);
        w_u32(object + 0xA8, 4);
        w_u32(object + 0xA0, 4);
        w_u32(object + 0x6C, 0);
        w_u32(object + 0x114, 0);
        w_u32(object + 0x40, 0x800A358C);
        value = (uint32)r_s16(0x801076FC + offset);
        w_u32(object + 0x24, 0);
        w_u32(object + 0x28, 0);
        w_u32(object + 0x20, value);
        value = (uint32)r_s16(0x80107702 + offset);
        w_u32(object + 0x84, value);
        value = (uint32)r_s16(0x80107704 + offset);
        w_u32(object + 0xAC, 0);
        w_u32(object + 0xA8, value);
        value = (uint32)r_s16(0x80107706 + offset);
        w_u32(object + 0xB0, value);
        if (value != 0) {
            if ((sint32)value >= 17)
                w_u32(object + 0xB0, 16);
            value = r_u32(object + 0xB0);
            color_offset = (value - 1) * 12;
            first = r_u32(0x800EEE24 + color_offset);
            second = r_u32(0x800EEE28 + color_offset);
            third = r_u32(0x800EEE2C + color_offset);
            value = game_create_gradient_strip_set(first, second, third);
            w_u32(object + 0xBC, value);
        }
        value = r_u32(0x801076E8 + offset);
        ++index;
        w_u32(object + 0xB4, value);
        value = (uint32)r_s16(0x80107708 + offset);
        w_u32(object + 0x9C, 1);
        w_u32(object + 0xB8, value);
        offset += 36;
    } while ((sint32)index < r_s32(0x800595C0));
}

void game_load_player_models(uint32 player_type)
{
    char bundle[32];
    uint32 offset = player_type * 44, name, model, length, index, slot, table, record, source, imported, stats;
    FUNCTION_MARKER(0x8007A9DC, "GAME.OVL");
    name = 0x800ECBD4 + offset;
    model = game_find_model_name(name);
    if (model == 0) {
        length = 0;
        while (r_u8(name + length) != 0)
            ++length;
        strcpy(bundle, (const char *)psx_addr(name, (size_t)length + 1));
        strcat(bundle, (const char *)psx_addr(0x800744AC, 5));
        game_load_bound_bundle_native(bundle);
    }
    model = game_find_model_name(name);
    w_u16(0x800ECBEC + offset, model);
    for (index = 0; index < 3; ++index) {
        if (index == 0) {
            name = 0x800744B4;
            if (game_find_model_name(name) == 0)
                game_load_bound_bundle(0x800744BC);
        } else if (index == 1) {
            name = 0x800744C8;
            if (game_find_model_name(name) == 0)
                game_load_bound_bundle(0x800744D0);
        } else {
            name = 0x800ECBE0 + offset;
            length = 0;
            while (r_u8(name + length) != 0)
                ++length;
            strcpy(bundle, (const char *)psx_addr(name, (size_t)length + 1));
            strcat(bundle, (const char *)psx_addr(0x800744AC, 5));
            game_load_bound_bundle_native(bundle);
        }
        slot = 0x800EC890 + index * 4;
        model = game_find_model_name(name);
        table = r_u32(0x80046178);
        source = r_u32(table + model * 132);
        w_u32(slot, model);
        imported = mesh_import_primitives(source);
        model = r_u32(slot);
        table = r_u32(0x80046178);
        source = r_u32(table + model * 132);
        heap_free(source - 12);
        model = r_u32(slot);
        table = r_u32(0x80046178);
        record = table + model * 132;
        w_u32(record, imported);
        w_u32(record + 0x74, 1);
    }
    stats = r_u32(0x800ECBF4 + offset);
    w_u32(0x800F2B24, r_u32(stats + 4));
    w_u32(0x800F2B28, r_u32(stats));
    w_u32(0x800F2B2C, r_u32(stats + 8));
    w_u32(0x800F2B30, r_u32(stats + 16));
    w_u32(0x800F2B34, r_u32(stats + 12));
    w_u32(0x800F2B38, r_u32(stats + 20));
    w_u32(0x800F2B40, r_u32(stats + 28));
    w_u32(0x800F2B3C, r_u32(stats + 24));
    w_u32(0x800F2B44, r_u32(stats + 32));
    w_u32(0x800F2B4C, r_u32(stats + 40));
    w_u32(0x800F2B48, r_u32(stats + 36));
    w_u32(0x800F2B50, r_u32(stats + 44));
    w_u32(0x800F2B58, r_u32(stats + 52));
    w_u32(0x800F2B54, r_u32(stats + 48));
    w_u32(0x800F2B5C, r_u32(stats + 56));
}

void game_initialize_demo_input(uint32 recording)
{
    uint32 flags, object;
    FUNCTION_MARKER(0x8009A29C, "GAME.OVL");
    flags = r_u32(0x80031138);
    w_u32(0x8010AC50, 0);
    w_u32(0x800311B8, 0);
    w_u32(0x8004EB24, 0);
    if ((flags & 3) == 0)
        return;
    flags = r_u32(0x80031138);
    w_u32(0x800313DC, 3);
    w_u32(0x800ECE70, 1);
    w_u16(0x800118F4, 0);
    if (flags & 2)
        game_load_demo_input(recording);
    else if (flags & 1)
        game_begin_input_recording();
    object = game_create_object(0, 0, 0, 0, 0);
    w_u32(object + 0x40, 0x8009A9DC);
}

void game_begin_input_recording(void)
{
    uint32 allocation, offset, first, second, wanted, index = 0, cursor, value, selected;
    uint32 words[4];
    FUNCTION_MARKER(0x8009A354, "GAME.OVL");
    w_u16(0x80033D28, 0);
    allocation = heap_alloc_back(0x244C);
    for (offset = 0; offset < 32; offset += 8) {
        first = r_u32(0x8004EA28 + offset);
        second = r_u32(0x8004EA2C + offset);
        w_u32(0x80040F50 + offset, first);
        w_u32(0x80040F54 + offset, second);
    }
    w_u32(0x8003112C, allocation);
    value = r_u8(0x800313BC);
    wanted = r_u8(0x8004EA29);
    selected = 0;
    if (value != 0xFF) {
        cursor = 0x800313BC;
        for (;;) {
            value = r_u8(cursor);
            ++cursor;
            if (value == wanted)
                break;
            value = r_u8(cursor);
            ++index;
            if (value == 0xFF)
                break;
        }
        if (r_u8(0x800313BC + index) != 0xFF)
            selected = 0x800421C0 + index * 48;
    }
    w_u32(0x800313C4, selected);
    selected = r_u32(0x800313C4);
    for (offset = 0; offset < 48; offset += 16) {
        words[0] = r_u32(selected + offset);
        words[1] = r_u32(selected + offset + 4);
        words[2] = r_u32(selected + offset + 8);
        words[3] = r_u32(selected + offset + 12);
        w_u32(0x800428C8 + offset, words[0]);
        w_u32(0x800428CC + offset, words[1]);
        w_u32(0x800428D0 + offset, words[2]);
        w_u32(0x800428D4 + offset, words[3]);
    }
    value = r_u16(0x80033D28);
    w_u32(0x800428F8, 0);
    w_u16(0x80033D28, value + 0x5C);
}

uint32 game_initialize_level_scene(void)
{
    PSX_RECT rectangle;
    uint32 first, second, object, state, offset_x, offset_y;
    sint32 index;
    FUNCTION_MARKER(0x80092B74, "GAME.OVL");
    first = r_u32(0x8007474C);
    second = r_u32(0x80074750);
    rectangle.x = (sint16)first;
    rectangle.y = (sint16)(first >> 16);
    rectangle.w = (sint16)second;
    rectangle.h = (sint16)(second >> 16);
    w_u16(0x80031254, 0);
    object_initialize_pool();
    game_initialize_model_table();
    game_initialize_demo_input(0x80010518);
    game_parse_level_numbers(0x80010518);
    state = game_get_controller_state();
    w_u32(0x80042174, state);
    game_initialize_player_objects();
    object = r_u32(0x80040FDC);
    w_u32(0x8004EB1C, object);
    game_clear_scene_resource_state();
    object = game_create_object(0, 0, 0, 0, 0);
    w_u32(object + 0x40, 0x80082A48);
    srandPSX(0x87654321);
    w_u32(0x800311BC, 0x12345678);
    w_u32(0x800EF50C, 0x12345678);
    w_u32(0x80053684, 0);
    w_u32(0x80053678, 0);
    w_u32(0x8004616C, 0);
    w_u32(0x8001020C, 0);
    game_initialize_effect_pool();
    game_initialize_mesh_effect_pool();
    game_initialize_local_starfield();
    game_clear_scene_effect_flags();
    game_clear_scene_link_slots();
    game_clear_auxiliary_effect_slots();
    game_clear_trail_slots();
    game_clear_particle_records();
    game_initialize_fragment_slots();
    if (r_u32(0x80031134) == 0)
        game_clear_ship_effect_counters();
    game_initialize_sky_resources();
    game_clear_model_binding_slots();
    game_load_scene_texture_page();
    game_bind_overlay_effect_textures();
    message_init();
    w_u32(0x800ECBD0, 0);
    game_initialize_training_controller();
    game_initialize_level_script();
    game_initialize_direction_indicator();
    game_initialize_scene_sky();
    if (r_u32(0x80031134) == 0) {
        state = r_u32(0x800ECBD0);
        game_load_player_models(state);
        game_initialize_player_weapon_slots();
    }
    game_initialize_head_models_and_palettes();
    game_initialize_blob_effect_resources();
    game_position_scene_effects();
    game_initialize_configured_objects();
    if (r_u32(0x80031134) == 0)
        game_create_view_controller();
    game_initialize_scene_grid();
    scene_stop_loading();
    sound_set_level((uint32)r_s16(0x800118FC));
    game_initialize_scene_effect_voices();
    VSync(0);
    SetDispMask(0);
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);
    state = resource_load_sector_index((const char *)psx_addr(0x80074754, 13), (const char *)psx_addr(0x80074764, 13));
    w_u32(0x800F2B08, state);
    stream_init();
    VSyncCallbackPSX(0x80092B34);
    w_u32(0x800F4F68, 0);
    w_u16(0x800F4F6C, 0);
    w_u8(0x800FD6FA, 0);
    for (index = 200; index >= 0; index -= 100)
        w_u8(0x8011F053 + (uint32)index, 0);
    render_set_full_height(512);
    offset_x = r_u32(0x80031198);
    offset_y = r_u32(0x800311B4);
    w_u32(0x8001024C, offset_x);
    w_u32(0x80010250, offset_y);
    system_set_projection_offset((sint32)offset_x, (sint32)offset_y);
    game_refresh_mission_state();
    game_initialize_credits_controller();
    srandPSX(0x87654321);
    w_u32(0x800311BC, 0x12345678);
    w_u32(0x800EF50C, 0x12345678);
    return game_set_screen_tint(255, 255, 255);
}

void game_initialize_scene_grid(void)
{
    SVECTOR rotation;
    uint32 bank, offset, row, column, copy, angle, radius, vertex, product, seed, jitter;
    uint32 base_u, base_v, packet, next_row, current, next, page_x, page_y, clut_x, clut_y, value;
    sint32 uv[8];
    uint32 component;
    FUNCTION_MARKER(0x8008EA60, "GAME.OVL");
    rotation.vx = 1024;
    rotation.vy = 0;
    rotation.vz = 0;
    for (bank = 0; bank < 2; ++bank) {
        offset = bank * 0x5C48;
        w_u32(0x8010ACD0 + offset, 0);
        w_u32(0x8010ACD4 + offset, 0);
        w_u32(0x8010AD08 + offset, 0);
        w_u32(0x8010AD0C + offset, 0);
        w_u32(0x8010AD10 + offset, 0);
        w_u32(0x8010ACD8 + offset, 0);
        RotMatrixYXZ(&rotation, (MATRIX *)psx_addr(0x8010ACE8 + offset, 20));
        base_u = r_u32(0x80126B18);
        base_v = r_u32(0x80126B98);
        for (row = 0; row < 16; ++row) {
            angle = row * 256;
            for (column = 0; column < 16; ++column) {
                radius = (column + 1) * 64;
                product = radius * (uint32)rsin((sint32)angle);
                vertex = 0x8010FD18 + offset + row * 128 + column * 8;
                w_u16(vertex, (uint32)((sint32)product >> 12));
                product = radius * (uint32)rcos((sint32)angle);
                seed = r_u32(0x800EF50C);
                w_u16(vertex + 4, 0);
                seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
                w_u32(0x800EF50C, seed);
                jitter = (seed & 0xFF) - 128;
                w_u16(vertex + 2, (uint32)((sint32)product >> 12));
                w_u32(0x80110518 + offset + row * 64 + column * 4, jitter);
            }
        }
        for (row = 0; row < 16; ++row) {
            next_row = (row + 1) & 15;
            for (column = 0; column < 15; ++column) {
                for (copy = 0; copy < 2; ++copy) {
                    current = 0x8010FD18 + offset + row * 128 + column * 8;
                    next = 0x8010FD18 + offset + next_row * 128 + column * 8;
                    uv[0] = r_s16(current) + 1024;
                    uv[1] = r_s16(current + 2) + 1024;
                    uv[2] = r_s16(next) + 1024;
                    uv[3] = r_s16(next + 2) + 1024;
                    uv[4] = r_s16(current + 8) + 1024;
                    uv[5] = r_s16(current + 10) + 1024;
                    uv[6] = r_s16(next + 8) + 1024;
                    uv[7] = r_s16(next + 10) + 1024;
                    for (component = 0; component < 8; ++component)
                        uv[component] = uv[component] * 191 / 2048;
                    packet = 0x8010AD18 + offset + row * 1280 + column * 80 + copy * 40;
                    SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
                    w_u8(packet + 4, 128);
                    w_u8(packet + 5, 0);
                    w_u8(packet + 6, 255);
                    w_u8(packet + 12, base_u + (uint32)uv[0]);
                    w_u8(packet + 13, base_v + (uint32)uv[1]);
                    w_u8(packet + 20, base_u + (uint32)uv[2]);
                    w_u8(packet + 21, base_v + (uint32)uv[3]);
                    w_u8(packet + 28, base_u + (uint32)uv[4]);
                    w_u8(packet + 29, base_v + (uint32)uv[5]);
                    w_u8(packet + 36, base_u + (uint32)uv[6]);
                    w_u8(packet + 37, base_v + (uint32)uv[7]);
                    page_x = r_u32(0x800F3CE8);
                    page_y = r_u32(0x800F3D90);
                    value = GetTPage(0, 1, (sint32)page_x, (sint32)page_y);
                    w_u16(packet + 22, value);
                    clut_x = r_u32(0x800F3240);
                    clut_y = r_u32(0x800F32C0);
                    value = GetClut((sint32)clut_x, (sint32)clut_y);
                    w_u16(packet + 14, value);
                    SetSemiTrans(psx_addr(packet, 8), 1);
                }
            }
        }
    }
}

static const char *game_credits_guest_text_native(uint32 text)
{
    uint32 length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    return (const char *)psx_addr(text, (size_t)length + 1);
}

void game_fill_credits_records(void)
{
    uint32 end, cursor, count, index, line, previous, base, record, value, token, next_index;
    const char *text;
    FUNCTION_MARKER(0x800E9BD8, "GAME.OVL");
    end = r_u32(0x80109B3C);
    cursor = r_u32(0x8010AB80);
    if (cursor >= end - 2)
        return;
    do {
        count = r_u32(0x800F3BE0);
        if ((sint32)count >= 30)
            return;
        index = r_u32(0x801079C0);
        line = r_u32(0x8011EFE8);
        previous = r_u32(0x80118A68);
        base = r_u32(0x80118A84);
        w_u32(0x800F4F2C, base + index * 72);
        w_u32(0x800F4F78, base + previous * 72);
        cursor = config_read_line(cursor, line);
        record = r_u32(0x800F4F2C);
        line = r_u32(0x8011EFE8);
        w_u32(0x8010AB80, cursor);
        text = game_credits_guest_text_native(line);
        sprintf((char *)psx_addr(record, strlen(text) + 1), "%s", text);
        cursor = r_u32(0x8010AB80);
        line = r_u32(0x8011EFE8);
        cursor = config_read_line(cursor, line);
        line = r_u32(0x8011EFE8);
        w_u32(0x8010AB80, cursor);
        config_split_arguments(line, (uint32 *)psx_addr(0x8011A088, 6004));
        token = r_u32(0x8011A088);
        value = (uint32)string_to_integer(game_credits_guest_text_native(token));
        record = r_u32(0x800F4F2C);
        w_u8(record + 0x43, value);
        token = r_u32(0x8011A08C);
        value = (uint32)string_to_integer(game_credits_guest_text_native(token));
        record = r_u32(0x800F4F2C);
        w_u8(record + 0x44, value);
        token = r_u32(0x8011A090);
        value = (uint32)string_to_integer(game_credits_guest_text_native(token));
        record = r_u32(0x800F4F2C);
        w_u8(record + 0x45, value);
        record = r_u32(0x800F4F2C);
        w_u8(record + 0x42, 0);
        if (r_u32(0x800F2394) == 1) {
            record = r_u32(0x800F4F2C);
            w_u16(record + 0x3C, 250);
        } else {
            previous = r_u32(0x800F4F78);
            value = r_u8(previous + 0x45);
            value += r_u16(previous + 0x3C);
            record = r_u32(0x800F4F2C);
            w_u16(record + 0x3C, value);
        }
        record = r_u32(0x800F4F2C);
        w_u32(0x800F2394, 0);
        w_u8(record + 0x46, 1);
        count = r_u32(0x800F3BE0);
        index = r_u32(0x801079C0);
        w_u32(0x80118A68, index);
        next_index = index + 1;
        w_u32(0x801079C0, next_index);
        w_u32(0x800F3BE0, count + 1);
        if ((sint32)next_index >= 30)
            w_u32(0x801079C0, 0);
        end = r_u32(0x80109B3C);
        cursor = r_u32(0x8010AB80);
    } while (cursor < end - 2);
}

void game_initialize_credits_controller(void)
{
    char filename[16];
    uint32 buffer, size, loaded, records, index, record, next, data, player;
    const char *empty;
    FUNCTION_MARKER(0x800EA1FC, "GAME.OVL");
    memcpy(filename, psx_addr(0x80076B90, 12), 12);
    memset(filename + 12, 0, 4);
    if (r_u32(0x80031134) == 0)
        return;
    w_u32(0x800F4228, 0);
    w_u32(0x800F3BE0, 0);
    w_u32(0x801079C0, 0);
    w_u32(0x80118A68, 0);
    w_u32(0x80108910, 0);
    buffer = heap_alloc_front(3000);
    w_u32(0x8011EFE8, buffer);
    size = disc_file_size(filename);
    if (size == 0)
        diagnostic_report(0, 0x80076B9C, 0x80076BBC);
    data = heap_alloc_front(size);
    w_u32(0x8011A3B4, data);
    loaded = resource_load_into(filename, data);
    records = heap_alloc_front(2160);
    w_u32(0x80118A84, records);
    w_u32(0x800F4F2C, records);
    for (index = 0; index < 30; ++index) {
        record = r_u32(0x800F4F2C);
        empty = game_credits_guest_text_native(0x80076BBC);
        sprintf((char *)psx_addr(record, strlen(empty) + 1), "%s", empty);
        record = r_u32(0x800F4F2C);
        w_u8(record + 0x43, 0);
        record = r_u32(0x800F4F2C);
        w_u8(record + 0x44, 0);
        record = r_u32(0x800F4F2C);
        w_u8(record + 0x45, 0);
        record = r_u32(0x800F4F2C);
        w_u8(record + 0x46, 0);
        next = r_u32(0x800F4F2C);
        w_u16(record + 0x3C, 0);
        w_u16(record + 0x3E, 0);
        w_u16(record + 0x40, 0);
        w_u32(0x800F4F2C, next + 72);
    }
    data = r_u32(0x8011A3B4);
    w_u32(0x8010AB80, data);
    w_u32(0x80109B3C, data + loaded);
    game_fill_credits_records();
    player = r_u32(0x80040FDC);
    w_u32(player + 0x40, 0x800EA410);
    w_u32(player + 0x3C, 0x8002493C);
    render_set_reduced_height(512);
    game_refresh_mission_state();
}

void game_load_demo_input(uint32 level_name)
{
    uint32 selection, name, word, fourth, fifth, sixth, suffix, length, size, allocation, loaded;
    uint32 data, mission, chapter, source, offset, count, words[4];
    FUNCTION_MARKER(0x8009A53C, "GAME.OVL");
    selection = r_u32(0x8003113C);
    w_u16(0x80033D28, 0);
    if (selection < 5) {
        name = 0x8007596C + selection * 8;
        word = r_u32(name);
        fourth = (uint32)r_s8(name + 4);
        w_u32(0x800F4EC0, word);
        w_u8(0x800F4EC4, fourth);
        fifth = (uint32)r_s8(name + 5);
        sixth = (uint32)r_s8(name + 6);
        w_u8(0x800F4EC5, fifth);
        w_u8(0x800F4EC6, sixth);
    } else {
        diagnostic_report(selection, 0x80075994, 0x8007599C);
    }
    suffix = r_u32(0x800311C0) != 0 ? 0x800759A0 : 0x800759A8;
    length = (uint32)strlen(game_credits_guest_text_native(0x800F4EC0));
    strcat((char *)psx_addr(0x800F4EC0, (size_t)length + 5), (const char *)psx_addr(suffix, 5));
    size = disc_file_size(game_credits_guest_text_native(0x800F4EC0));
    w_u32(0x800EE9D4, size);
    if (size != 0) {
        allocation = heap_alloc_back(0x244C);
        w_u32(0x8003112C, allocation);
        loaded = resource_load_into(game_credits_guest_text_native(0x800F4EC0), allocation);
        size = r_u32(0x800EE9D4);
        if (loaded != size)
            diagnostic_report(1200, 0x800759B0, 0x8007599C);
    } else {
        diagnostic_report(1000, 0x800759B0, 0x8007599C);
    }
    data = r_u32(0x8003112C);
    mission = r_u8(data);
    w_u16(0x800595F8, mission);
    chapter = r_u8(data + 1);
    w_u16(0x800595FA, chapter);
    sprintf((char *)psx_addr(level_name, 11), "%d_%d.CW", (int)mission, (int)chapter);
    source = r_u32(0x8003112C);
    count = r_u16(0x80033D28);
    source += 12;
    w_u16(0x80033D28, count + 12);
    for (offset = 0; offset < 24; offset += 12) {
        words[0] = r_u32(source + offset);
        words[1] = r_u32(source + offset + 4);
        words[2] = r_u32(source + offset + 8);
        w_u32(0x80040F50 + offset, words[0]);
        w_u32(0x80040F54 + offset, words[1]);
        w_u32(0x80040F58 + offset, words[2]);
    }
    words[0] = r_u32(source + 24);
    words[1] = r_u32(source + 28);
    w_u32(0x80040F68, words[0]);
    w_u32(0x80040F6C, words[1]);
    source += 32;
    count = r_u16(0x80033D28);
    w_u16(0x80033D28, count + 32);
    for (offset = 0; offset < 48; offset += 16) {
        words[0] = r_u32(source + offset);
        words[1] = r_u32(source + offset + 4);
        words[2] = r_u32(source + offset + 8);
        words[3] = r_u32(source + offset + 12);
        w_u32(0x800428C8 + offset, words[0]);
        w_u32(0x800428CC + offset, words[1]);
        w_u32(0x800428D0 + offset, words[2]);
        w_u32(0x800428D4 + offset, words[3]);
    }
    count = r_u16(0x80033D28);
    w_u32(0x800428F8, 0);
    w_u16(0x80033D28, count + 48);
}

void game_draw_demo_input_status(void)
{
    uint32 flags, standard, text;
    FUNCTION_MARKER(0x8009A9DC, "GAME.OVL");
    w_u32(0x80033D2C, 0);
    draw_set_color_rgb(128, 32, 32);
    flags = r_u32(0x80031138);
    if (flags & 1) {
        standard = r_u32(0x800311C0);
        w_u32(0x80053688, 250);
        w_u32(0x8005368C, 30);
        if (standard != 0)
            w_u32(0x8005368C, 31);
        text = 0x800759E0;
    } else if (flags & 2) {
        standard = r_u32(0x800311C0);
        w_u32(0x80053688, 200);
        w_u32(0x8005368C, 30);
        if (standard != 0)
            w_u32(0x8005368C, 31);
        text = 0x800759EC;
    } else {
        return;
    }
    font_draw_current(game_credits_guest_text_native(text));
}

void game_tick_credits_controller(uint32 object)
{
    uint32 paused = r_u32(0x80053684), matrix, speed, first, second, third;
    sint32 x, y, z;
    FUNCTION_MARKER(0x800EA418, "GAME.OVL");
    if (paused != 0) {
        w_u32(0x800ECEA8, 3);
        return;
    }
    matrix = object + 0x11C;
    game_apply_control_rotation(matrix, 2, 2, 3);
    matrix_normalize(matrix, 1);
    object_extract_angles(object, object);
    matrix_copy_rotation(0x80035060, matrix);
    x = r_s16(0x80035060) * 8;
    y = r_s16(0x80035062) * 8;
    z = r_s16(0x80035064) * 8;
    w_u16(0x80035060, (uint32)(x / 5));
    speed = r_u32(0x800F2390);
    w_u16(0x80035062, (uint32)(y / 5));
    w_u16(0x80035064, (uint32)(z / 5));
    first = (uint32)r_s16(object + 0x128) * speed;
    second = (uint32)r_s16(object + 0x12A) * speed;
    third = (uint32)r_s16(object + 0x12C) * speed;
    w_u32(object + 0xFC, (uint32)((sint32)first >> 12));
    w_u32(object + 0x100, (uint32)((sint32)second >> 12));
    w_u32(object + 0x104, (uint32)((sint32)third >> 12));
    game_update_object_movement(object);
    w_u32(0x80040C94, object);
    camera_dispatch();
    game_fill_credits_records();
    if (game_update_and_draw_credits() != 0)
        w_u32(0x800ECEA8, 3);
}

static void game_set_credits_fade_native(uint32 record, sint32 amount)
{
    uint32 style = r_u8(record + 0x44);
    if (style == 0) {
        w_u8(0x8004617C, 0);
        w_u8(0x80033D38, (uint32)amount);
    } else {
        w_u8(0x80033D38, (uint32)((amount * 5) >> 1));
        w_u8(0x8004617C, 0);
    }
    w_u8(0x80053680, (uint32)((amount * 7) >> 1));
}

uint32 game_update_and_draw_credits(void)
{
    uint32 index, scanned, base, record, frame, value, count, next_index;
    sint32 position, fade;
    FUNCTION_MARKER(0x800E9E1C, "GAME.OVL");
    index = r_u32(0x80108910);
    for (scanned = 0; scanned < 30; ++scanned) {
        base = r_u32(0x80118A84);
        record = base + index * 72;
        w_u32(0x800F4F2C, record);
        if (r_u8(record + 0x46) == 0)
            goto next_credits_record;
        frame = r_u32(0x800311B8);
        if (frame & 1)
            w_u16(record + 0x3C, r_u16(record + 0x3C) - 1);
        record = r_u32(0x800F4F2C);
        position = r_s16(record + 0x3C);
        if (position < -30) {
            w_u8(record + 0x46, 0);
            count = r_u32(0x800F3BE0) - 1;
            w_u32(0x800F3BE0, count);
            if (count == 0)
                return 1;
            next_index = r_u32(0x80108910) + 1;
            w_u32(0x80108910, next_index);
            if ((sint32)next_index >= 30)
                w_u32(0x80108910, 0);
        } else if ((uint16)(position - 21) < 139) {
            if (r_u8(record + 0x42) == 0) {
                w_u16(record + 0x3E, 31);
                w_u8(record + 0x42, 1);
            }
            record = r_u32(0x800F4F2C);
            fade = r_s16(record + 0x3E);
            game_set_credits_fade_native(record, fade > 0 ? 32 - fade : 32);
            record = r_u32(0x800F4F2C);
            value = r_u8(record + 0x43);
            w_u32(0x80033D2C, value == 0 ? 0 : 84);
            record = r_u32(0x800F4F2C);
            position = r_s16(record + 0x3C);
            fade = r_s16(record + 0x3E);
            font_draw_centered_outline(game_credits_guest_text_native(record), (uint32)position, fade);
            record = r_u32(0x800F4F2C);
            fade = r_s16(record + 0x3E);
            if (fade > 0)
                w_u16(record + 0x3E, (uint32)(fade - 1));
        } else if (position < 21) {
            if (r_u8(record + 0x42) == 1) {
                w_u16(record + 0x40, 1);
                w_u8(record + 0x42, 2);
            }
            record = r_u32(0x800F4F2C);
            fade = r_s16(record + 0x40);
            if (fade < 31) {
                game_set_credits_fade_native(record, 32 - fade);
                record = r_u32(0x800F4F2C);
                value = r_u8(record + 0x43);
                w_u32(0x80033D2C, value == 0 ? 0 : 84);
                record = r_u32(0x800F4F2C);
                position = r_s16(record + 0x3C);
                font_draw_centered_outline(game_credits_guest_text_native(record), (uint32)position, 0);
                record = r_u32(0x800F4F2C);
                value = r_u16(record + 0x40);
                w_u16(record + 0x40, value + 1);
            }
        }
next_credits_record:
        ++index;
        if ((sint32)index >= 30)
            index = 0;
    }
    return 0;
}

uint32 game_get_ship_task_parameter(uint32 task, uint32 parameter)
{
    uint32 arguments;
    FUNCTION_MARKER(0x80096CF0, "GAME.OVL");
    arguments = r_u32(0x800F27AC + (task << 3));
    return r_u32(arguments + (parameter << 2));
}

uint32 game_validate_ship_task_target(uint32 object)
{
    uint32 target, generation, current;
    FUNCTION_MARKER(0x8009ADF4, "GAME.OVL");
    target = r_u32(object + 0x170);
    if (target != 0) {
        generation = r_u16(object + 0x174);
        current = r_u16(target + 0x24A);
        if (generation == current)
            return 1;
    }
    w_u32(object + 0x170, 0);
    return 0;
}

uint32 game_ship_task_skip(void)
{
    FUNCTION_MARKER(0x8009AB50, "GAME.OVL");
    return 0;
}

uint32 game_ship_task_01(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, range, target, flags, generation, callback;
    FUNCTION_MARKER(0x8009AE84, "GAME.OVL");
    if (r_u32(object + 0x114) & 0x4000)
        return 0;
    identifier = game_get_ship_task_parameter(task, 2);
    range = game_get_ship_task_parameter(task, 3);
    target = game_find_script_ship_near(object, identifier, range, 0);
    if (target == 0)
        return 0;
    flags = r_u32(object + 0x114);
    if (flags & 0x10) {
        w_u32(object + 0x178, target);
        generation = r_u16(target + 0x24A);
        w_u16(object + 0x17C, generation);
        return 0;
    }
    w_u32(object + 0x178, target);
    generation = r_u16(target + 0x24A);
    callback = r_u32(object + 0x54);
    w_u16(object + 0x17C, generation);
    w_u32(object + 0x40, callback);
    return 1;
}

uint32 game_dispatch_ship_task_native(uint32 target, uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    switch (target) {
    case 0x8009AB50:
        return game_ship_task_skip();
    case 0x8009AE84:
        return game_ship_task_01(object, task, ordinal, cursor);
    case 0x8009AF44:
        return game_ship_task_02(object, task, ordinal, cursor);
    case 0x8009B03C:
        return game_ship_task_03(object, task, ordinal, cursor);
    case 0x8009B1C8:
        return game_ship_task_04(object, task, ordinal, cursor);
    case 0x8009B228:
        return game_ship_task_05(object, task, ordinal, cursor);
    case 0x8009B2B0:
        return game_ship_task_06(object, task, ordinal, cursor);
    case 0x8009B3D8:
        return game_ship_task_07(object, task, ordinal, cursor);
    case 0x8009B620:
        return game_ship_task_08(object, task, ordinal, cursor);
    case 0x8009BB20:
        return game_ship_task_09(object, task, ordinal, cursor);
    case 0x8009BCE0:
        return game_ship_task_10(object, task, ordinal, cursor);
    case 0x8009BE5C:
        return game_ship_task_11(object, task, ordinal, cursor);
    case 0x8009C0B0:
        return game_ship_task_12(object, task, ordinal, cursor);
    case 0x8009C1DC:
        return game_ship_task_13(object, task, ordinal, cursor);
    case 0x8009C384:
        return game_ship_task_14(object, task, ordinal, cursor);
    case 0x8009C4DC:
        return game_ship_task_15(object, task, ordinal, cursor);
    case 0x8009C834:
        return game_ship_task_16(object, task, ordinal, cursor);
    case 0x8009C96C:
        return game_ship_task_17(object, task, ordinal, cursor);
    case 0x8009CAC0:
        return game_ship_task_18(object, task, ordinal, cursor);
    case 0x8009CCC0:
        return game_ship_task_19(object, task, ordinal, cursor);
    case 0x8009CE50:
        return game_ship_task_20(object, task, ordinal, cursor);
    case 0x8009CFD8:
        return game_ship_task_21(object, task, ordinal, cursor);
    case 0x8009D254:
        return game_ship_task_22(object, task, ordinal, cursor);
    case 0x8009D29C:
        return game_ship_task_23(object, task, ordinal, cursor);
    case 0x8009D3CC:
        return game_ship_task_24(object, task, ordinal, cursor);
    case 0x8009D4DC:
        return game_ship_task_25(object, task, ordinal, cursor);
    case 0x8009D600:
        return game_ship_task_26(object, task, ordinal, cursor);
    case 0x8009D770:
        return game_ship_task_27(object, task, ordinal, cursor);
    case 0x8009D7C0:
        return game_ship_task_28(object, task, ordinal, cursor);
    case 0x8009D88C:
        return game_ship_task_29(object, task, ordinal, cursor);
    case 0x8009DB6C:
        return game_ship_task_30(object, task, ordinal, cursor);
    case 0x8009DDA4:
        return game_ship_task_31(object, task, ordinal, cursor);
    case 0x8009DE54:
        return game_ship_task_32(object, task, ordinal, cursor);
    case 0x8009DF18:
        return game_ship_task_33(object, task, ordinal, cursor);
    case 0x8009DFB8:
        return game_ship_task_34(object, task, ordinal, cursor);
    case 0x8009E078:
        return game_ship_task_35(object, task, ordinal, cursor);
    case 0x8009E0EC:
        return game_ship_task_36(object, task, ordinal, cursor);
    case 0x8009E190:
        return game_ship_task_37(object, task, ordinal, cursor);
    case 0x8009E24C:
        return game_ship_task_38(object, task, ordinal, cursor);
    case 0x8009E2F0:
        return game_ship_task_39(object, task, ordinal, cursor);
    case 0x8009E374:
        return game_ship_task_40(object, task, ordinal, cursor);
    case 0x8009E3EC:
        return game_ship_task_41(object, task, ordinal, cursor);
    case 0x8009E490:
        return game_ship_task_42(object, task, ordinal, cursor);
    case 0x8009E56C:
        return game_ship_task_43(object, task, ordinal, cursor);
    case 0x8009E588:
        return game_ship_task_44(object, task, ordinal, cursor);
    case 0x8009E68C:
        return game_ship_task_45(object, task, ordinal, cursor);
    case 0x8009E728:
        return game_ship_task_46(object, task, ordinal, cursor);
    case 0x8009E7EC:
        return game_ship_task_47(object, task, ordinal, cursor);
    case 0x8009E9A8:
        return game_ship_task_48(object, task, ordinal, cursor);
    case 0x8009EBAC:
        return game_ship_task_49(object, task, ordinal, cursor);
    case 0x8009ED80:
        return game_ship_task_50(object, task, ordinal, cursor);
    case 0x8009ED98:
        return game_ship_task_51(object, task, ordinal, cursor);
    case 0x8009EDC4:
        return game_ship_task_52(object, task, ordinal, cursor);
    case 0x8009EF7C:
        return game_ship_task_53(object, task, ordinal, cursor);
    case 0x8009F038:
        return game_ship_task_54(object, task, ordinal, cursor);
    case 0x8009F148:
        return game_ship_task_55(object, task, ordinal, cursor);
    case 0x8009F1FC:
        return game_ship_task_56(object, task, ordinal, cursor);
    case 0x8009F404:
        return game_ship_task_57(object, task, ordinal, cursor);
    case 0x8009F494:
        return game_ship_task_58(object, task, ordinal, cursor);
    case 0x8009F540:
        return game_ship_task_59(object, task, ordinal, cursor);
    default:
        abort();
    }
}

void game_begin_script_ship_task(uint32 object)
{
    uint32 flags, target, generation, other, callback, value, cursor, task, kind, ordinal = 0;
    FUNCTION_MARKER(0x8009AB58, "GAME.OVL");
    flags = r_u32(object + 0x114);
    w_u32(0x800EE9D8, 0);
    if ((flags & 0x10000010) == 0x10000000) {
        target = r_u32(0x80040FDC);
        w_u32(object + 0x178, target);
        generation = r_u16(target + 0x24A);
        w_u32(object + 0x40, 0x800DBE20);
        w_u16(object + 0x17C, generation);
        return;
    }
    if (r_s32(object + 0x244) <= 0 || (flags & 0x400000)) {
        flags = r_u32(object + 0x114);
        w_u32(object + 0x244, 1);
        if (flags & 0x400000)
            w_u32(object + 0x114, flags ^ 0x400000);
    }
    target = r_u32(object + 0x178);
    generation = r_u16(object + 0x17C);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x180, target);
    w_u16(object + 0x184, generation);
    if (flags & 0x2000)
        w_u32(object + 0x114, flags ^ 0x2000);
    if (r_u32(object + 0x114) & 0x1000) {
        target = r_u32(object + 0x178);
        if (target != 0) {
            generation = r_u16(object + 0x17C);
            value = r_u16(target + 0x24A);
            if (generation == value) {
                callback = r_u32(object + 0x54);
                w_u32(object + 0x40, callback);
                psx_dispatch_guest_call(callback, 1, object, 0, 0);
                return;
            }
        }
        flags = r_u32(object + 0x114);
        other = r_u32(object + 0x170);
        w_u32(object + 0x114, flags ^ 0x1000);
        if (other != 0) {
            generation = r_u16(object + 0x174);
            value = r_u16(other + 0x24A);
            if (generation == value)
                w_u8(other + 0x26D, r_u8(other + 0x26D) - 1);
        }
    }
    target = r_u32(object + 0x1A8);
    if (target != 0) {
        generation = r_u16(object + 0x1AC);
        value = r_u16(target + 0x24A);
        if (generation != value)
            w_u8(object + 0x26D, 0);
    } else {
        w_u8(object + 0x26D, 0);
    }
    cursor = r_u32(object + 0xF4);
    w_u32(object + 0x198, 0);
    for (;;) {
        task = r_u32(cursor);
        cursor += 4;
        if (task == 0xFFFFFFFF)
            diagnostic_report(r_u32(object + 4), 0x80075A00, 0x80075A1C);
        if (task != 0xFFFFFFFE) {
            kind = r_u32(0x800F27A8 + (task << 3));
            callback = r_u32(0x800EE9DC + (kind << 2));
            value = game_dispatch_ship_task_native(callback, object, task, ordinal, &cursor);
            if (value != 0) {
                value = r_u32(object + 0xE0);
                callback = r_u32(object + 0x40);
                w_u32(object + 0xE0, ordinal);
                w_u32(object + 0xE4, value);
                psx_dispatch_guest_call(callback, 1, object, 0, 0);
                return;
            }
        }
        ++ordinal;
        cursor += 4;
    }
}

uint32 game_find_nearest_type_one(uint32 object, uint32 excluded_flags)
{
    uint32 candidate, best = 0, minimum = 0x7FFFFFF, x, y, z, ox, oy, oz, distance, flags;
    FUNCTION_MARKER(0x800AE25C, "GAME.OVL");
    candidate = object_lookup(1);
    while (candidate != 0) {
        x = r_u32(candidate + 8);
        ox = r_u32(object + 8);
        y = r_u32(candidate + 12);
        oy = r_u32(object + 12);
        oz = r_u32(object + 16);
        z = r_u32(candidate + 16);
        distance = vector_length_approx(x - ox, y - oy, z - oz);
        w_u32(0x80046174, distance);
        if ((sint32)distance < (sint32)minimum && r_u32(candidate) != 0) {
            flags = r_u32(candidate + 0x114);
            if (!(flags & 0x8000) && !(flags & excluded_flags)) {
                minimum = distance;
                best = candidate;
            }
        }
        candidate = r_u32(candidate + 0x1DC);
    }
    return best;
}

uint32 game_find_nearest_type_three(uint32 object, uint32 excluded_flags)
{
    uint32 candidate, best = 0, minimum = 0x7FFFFFF, x, y, z, ox, oy, oz, distance, flags;
    FUNCTION_MARKER(0x800AE348, "GAME.OVL");
    candidate = object_lookup(3);
    while (candidate != 0) {
        x = r_u32(candidate + 8);
        ox = r_u32(object + 8);
        y = r_u32(candidate + 12);
        oy = r_u32(object + 12);
        oz = r_u32(object + 16);
        z = r_u32(candidate + 16);
        distance = vector_length_approx(x - ox, y - oy, z - oz);
        w_u32(0x80046174, distance);
        if ((sint32)distance < (sint32)minimum && r_u32(candidate) != 0) {
            flags = r_u32(candidate + 0x114);
            if (!(flags & 0x8000) && !(flags & excluded_flags)) {
                minimum = distance;
                best = candidate;
            }
        }
        candidate = r_u32(candidate + 0x1DC);
    }
    return best;
}

uint32 game_ship_task_02(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 range, identifier, target, generation, value;
    FUNCTION_MARKER(0x8009AF44, "GAME.OVL");
    range = game_get_ship_task_parameter(task, 3);
    if (r_u32(object + 0xE0) == ordinal && game_validate_ship_task_target(object) != 0) {
        game_begin_ship_task_approach(object, range);
        return 1;
    }
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near(object, identifier, 0x7FFFFFF, 0);
    if (target == 0)
        return 0;
    w_u32(object + 0x170, target);
    generation = r_u16(target + 0x24A);
    w_u16(object + 0x174, generation);
    value = game_get_ship_task_parameter(task, 4);
    w_u32(object + 0x214, value);
    value = game_get_ship_task_parameter(task, 5);
    w_u32(object + 0x218, value);
    value = game_get_ship_task_parameter(task, 6);
    w_u32(object + 0x21C, value);
    game_begin_ship_task_approach(object, range);
    return 1;
}

uint32 game_ship_task_03(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, group, target, mode, range, generation, callback, x, y, z, ox, oy, oz, distance;
    FUNCTION_MARKER(0x8009B03C, "GAME.OVL");
    flags = r_u32(object + 0x114);
    if (flags & 0x4000)
        return 0;
    if (flags & 0x10) {
        group = r_u32(object) == 3 ? 1 : 3;
        target = game_find_nearest_object(object, 0, group, 0);
        if (target != 0) {
            w_u32(object + 0x178, target);
            generation = r_u16(target + 0x24A);
            w_u16(object + 0x17C, generation);
        }
        return 0;
    }
    mode = game_get_ship_task_parameter(task, 2);
    if (mode == 0) {
        if (r_u32(object) == 3)
            target = game_find_nearest_type_one(object, 0);
        else
            target = game_find_nearest_type_three(object, 0);
    } else {
        group = r_u32(object) == 3 ? 1 : 3;
        target = game_find_nearest_object(object, mode == 1 ? 1 : 0, group, 0);
    }
    if (target == 0 || r_u32(target) == 0)
        return 0;
    range = game_get_ship_task_parameter(task, 3);
    if (range != 0) {
        x = r_u32(target + 8);
        ox = r_u32(object + 8);
        y = r_u32(target + 12);
        oy = r_u32(object + 12);
        oz = r_u32(object + 16);
        z = r_u32(target + 16);
        distance = vector_length_approx(x - ox, y - oy, z - oz);
        if ((sint32)range < (sint32)distance)
            return 0;
    }
    w_u32(object + 0x178, target);
    generation = r_u16(target + 0x24A);
    callback = r_u32(object + 0x54);
    w_u16(object + 0x17C, generation);
    w_u32(object + 0x40, callback);
    return 1;
}

uint32 game_ship_task_04(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 type;
    FUNCTION_MARKER(0x8009B1C8, "GAME.OVL");
    type = r_u32(object);
    if (type == 1 || type == 3) {
        w_u32(object + 0x238, 0);
        w_u32(object + 0x234, 0);
    }
    w_u32(object + 0x40, 0x8009B200);
    return 1;
}

uint32 game_skip_ship_task(uint32 object, uint32 ordinal)
{
    uint32 task;
    FUNCTION_MARKER(0x8009ADB0, "GAME.OVL");
    task = r_u32(object + 0xF4) + (ordinal << 3);
    w_u32(task, 0xFFFFFFFE);
    return task;
}

uint32 game_call_ship_death_callback(uint32 object)
{
    uint32 callback;
    FUNCTION_MARKER(0x8009B200, "GAME.OVL");
    callback = r_u32(object + 0x5C);
    return psx_dispatch_guest_call(callback, 1, object, 0, 0);
}

uint32 game_ship_task_05(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 address, value;
    FUNCTION_MARKER(0x8009B228, "GAME.OVL");
    address = *cursor;
    value = r_u32(address);
    if (value != 0) {
        w_u32(address, value - 1);
        address = *cursor;
        value = r_u32(address);
        if ((sint32)value <= 0) {
            game_skip_ship_task(object, ordinal);
            return 0;
        }
    }
    w_u32(object + 0x40, 0x800A3AE0);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x252, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x9C, 30);
    return 1;
}

void game_begin_ship_task_approach(uint32 object, uint32 range)
{
    uint32 target, generation, callback, distance, x, y, z, ox, oy, oz, value, index, next_index;
    uint32 model, table, extent, first, second, ticks, yaw, pitch;
    FUNCTION_MARKER(0x800A3798, "GAME.OVL");
    if (r_u32(object) == 3)
        target = game_find_nearest_type_one(object, 0);
    else
        target = game_find_nearest_type_three(object, 0);
    if (target != 0) {
        game_evaluate_target_occlusion(object, target, 1);
        distance = r_u32(0x80046174);
        if ((sint32)distance < (sint32)range) {
            w_u32(object + 0x178, target);
            generation = r_u16(target + 0x24A);
            callback = r_u32(object + 0x54);
            w_u16(object + 0x17C, generation);
            w_u32(object + 0x40, callback);
            return;
        }
    }
    target = r_u32(object + 0x170);
    if (r_s32(target + 0xEC) > 25600) {
        x = r_u32(target + 8);
        ox = r_u32(object + 8);
        y = r_u32(target + 12);
        oy = r_u32(object + 12);
        z = r_u32(target + 16);
        oz = r_u32(object + 16);
        distance = vector_length_approx(x - ox, y - oy, z - oz);
        target = r_u32(object + 0x170);
        value = r_u32(target + 0xE8);
        if ((sint32)distance <= 30000)
            value -= 1280;
        else
            value += 2560;
        w_u32(object + 0xE8, value);
        game_begin_timed_maneuver(object, 30, 0x8009AB58, 0x800A3704);
        return;
    }
    index = r_u32(object + 4);
    target = r_u32(object + 0x170);
    value = r_u32(0x8005369C + index * 60);
    next_index = r_u32(object + 4);
    w_u32(object + 0xE8, (uint32)((sint32)value >> 2) * 3);
    value = r_u16(0x800536B4 + next_index * 60);
    w_u16(object + 0x260, value);
    game_steer_to_target_offset(object, target, object + 0x214, 0);
    model = r_u32(object + 0x74);
    distance = r_u32(0x80046174);
    table = r_u32(0x80046178);
    extent = r_u32(table + model * 132 + 0x6C);
    if ((sint32)(extent + 0xFDE8) < (sint32)distance) {
        ticks = (uint32)random_between(1, 2) * 30;
        game_begin_timed_maneuver(object, ticks, 0x8009AB58, 0x800A3704);
        return;
    }
    if ((sint32)distance < (sint32)(extent + 15000)) {
        first = (uint32)random_between(1600, 2000);
        second = (uint32)random_between(40, 80);
        game_adjust_maneuver_offsets(first, second);
    } else {
        value = (uint32)random_signed_between(400, 1040);
        w_u32(0x80033D1C, r_u32(0x80033D1C) + value);
        value = (uint32)random_signed_between(240, 560);
        w_u32(0x80033D30, r_u32(0x80033D30) + value);
    }
    ticks = (uint32)random_between(1, 2) * 30;
    yaw = r_u32(0x80033D1C);
    pitch = r_u32(0x80033D30);
    game_begin_yaw_maneuver(object, ticks, 0x8009AB58, yaw, pitch, 0x800A3760);
}
