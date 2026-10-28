#include "game_functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char *game_task_text_native(uint32 address)
{
    uint32 length = 0;
    while (r_u8(address + length) != 0)
        ++length;
    return (const char *)psx_addr(address, (size_t)length + 1);
}

uint32 game_ship_task_40(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, flags;
    FUNCTION_MARKER(0x8009E374, "GAME.OVL");
    if (r_u32(object) != 1) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075C68, 0x80075A1C);
    }
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x20000000);
    game_skip_ship_task(object, ordinal);
    return 0;
}

void game_begin_ship_allegiance_change(uint32 object)
{
    uint32 offset, value, flags;
    FUNCTION_MARKER(0x800A2830, "GAME.OVL");
    w_u32(object + 0x40, 0x800A2A94);
    w_u32(object + 0x88, 1);
    for (offset = 0xA8; offset < 0xBC; offset += 4) {
        value = (uint32)random_between(1, 80);
        w_u32(object + offset, value);
    }
    value = (uint32)random_between(1, 80);
    flags = r_u32(object + 0x114);
    w_u32(object + 0xBC, value);
    w_u32(object + 0xC0, 128);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x114, flags | 0x2000);
    game_replace_ship_for_category(object, 3);
}

uint32 game_ship_task_41(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags, clock, start, elapsed, threshold;
    FUNCTION_MARKER(0x8009E3EC, "GAME.OVL");
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x20000000);
    clock = r_u32(0x800311B8);
    start = r_u32(0x800595B8);
    elapsed = (uint32)(((uint64)(clock - start) * 0x88888889u) >> 32) >> 4;
    threshold = game_get_ship_task_parameter(task, 2);
    if ((sint32)elapsed >= (sint32)threshold)
        return 0;
    game_begin_ship_allegiance_change(object);
    game_skip_ship_task(object, ordinal);
    return 1;
}

uint32 game_ship_task_43(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    FUNCTION_MARKER(0x8009E56C, "GAME.OVL");
    w_u32(0x800F18DC, object);
    w_u32(object + 0x40, 0x800E6264);
    return 1;
}

uint32 game_ship_task_44(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, target;
    FUNCTION_MARKER(0x8009E588, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_select_ship_task_target(object, 0, 0, identifier, 0);
    if (target == 0 || (r_u32(target + 0x114) & 0x40000000))
        return 0;
    w_u32(object + 0x40, 0x800A3AE0);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x252, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x9C, 30);
    return 1;
}

uint32 game_create_moonbase_grid_controller(uint32 object)
{
    uint32 index, identifier, address, x, y, z, controller, generation, origin, target, delta;
    sint32 high;
    sint64 product;
    FUNCTION_MARKER(0x800AB0FC, "GAME.OVL");
    index = r_u32(0x800ED710);
    if (index == 0xFFFFFFFF) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075DCC, 0x80075D4C);
    }
    index = r_u32(0x800ED710);
    address = 0x801076EC + index * 36;
    x = r_u32(address);
    y = r_u32(address + 4);
    z = r_u32(address + 8);
    controller = game_create_object(0, x, y, z, 0);
    w_u32(object + 0x40, 0x8002493C);
    w_u32(controller + 0x40, 0x800AB270);
    w_u32(controller + 0x178, object);
    generation = r_u16(object + 0x24A);
    w_u32(controller + 0x9C, 120);
    origin = r_u32(controller + 8);
    w_u32(controller + 0x88, 1);
    w_u16(controller + 0x17C, generation);
    target = r_u32(object + 8);
    delta = target - origin;
    product = (sint64)(sint32)delta * 0x68DB8BAD;
    high = (sint32)(product >> 32);
    w_u32(controller + 0x214, (uint32)(high >> 12) - (uint32)((sint32)delta >> 31));
    target = r_u32(object + 12);
    origin = r_u32(controller + 12);
    delta = target - origin;
    product = (sint64)(sint32)delta * 0x68DB8BAD;
    high = (sint32)(product >> 32);
    w_u32(controller + 0x218, (uint32)(high >> 12) - (uint32)((sint32)delta >> 31));
    target = r_u32(object + 16);
    origin = r_u32(controller + 16);
    delta = target - origin;
    product = (sint64)(sint32)delta * 0x68DB8BAD;
    w_u32(controller + 0xA8, 0);
    w_u32(controller + 0xAC, 0);
    w_u32(controller + 0xB4, 1);
    w_u32(controller + 0xB0, 16);
    w_u32(controller + 0xB8, 0x07FFFFFF);
    high = (sint32)(product >> 32);
    w_u32(controller + 0x21C, (uint32)(high >> 12) - (uint32)((sint32)delta >> 31));
    return controller;
}

uint32 game_ship_task_45(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 threshold, clock, start, elapsed, message;
    FUNCTION_MARKER(0x8009E68C, "GAME.OVL");
    threshold = game_get_ship_task_parameter(task, 2);
    clock = r_u32(0x800311B8);
    start = r_u32(0x800595B8);
    elapsed = (uint32)(((uint64)(clock - start) * 0x88888889u) >> 32) >> 4;
    if (threshold < elapsed) {
        message = r_u32(0x800F2570);
        game_queue_hud_message(message);
        game_create_moonbase_grid_controller(object);
        game_skip_ship_task(object, ordinal);
    }
    return 0;
}

uint32 game_ship_task_46(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, target, actor;
    FUNCTION_MARKER(0x8009E728, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0);
    if (target == 0)
        return 0;
    if (!(r_u32(target + 0x114) & 0x10)) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075C88, 0x80075A1C);
    }
    actor = r_u32(object + 0x110);
    if (r_u8(actor + 0x46F) == 0)
        return 0;
    w_u32(object + 0x40, 0x800A3AE0);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x252, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x9C, 30);
    return 1;
}

uint32 game_ship_task_47(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 side, identifier, player, previous, next, address, head;
    sint32 current;
    FUNCTION_MARKER(0x8009E7EC, "GAME.OVL");
    side = game_get_ship_task_parameter(task, 2);
    current = r_s16(0x80059612);
    if (side != (uint32)current) {
        if (side != 3 && side != 1) {
            identifier = r_u32(object + 4);
            diagnostic_report(identifier, 0x80075CA0, 0x80075A1C);
            current = r_s16(0x80059612);
        }
        if (current != 3 && current != 1) {
            identifier = r_u32(object + 4);
            diagnostic_report(identifier, 0x80075CB4, 0x80075A1C);
        }
        player = r_u32(0x80040FDC);
        previous = r_u32(player + 0x1D8);
        if (previous != 0) {
            next = r_u32(player + 0x1DC);
            w_u32(previous + 0x1DC, next);
            player = r_u32(0x80040FDC);
        }
        next = r_u32(player + 0x1DC);
        if (next != 0) {
            previous = r_u32(player + 0x1D8);
            w_u32(next + 0x1D8, previous);
        }
        current = r_s16(0x80059612);
        address = 0x80031200 + (uint32)current * 8;
        head = r_u32(address);
        player = r_u32(0x80040FDC);
        if (head == player) {
            next = r_u32(head + 0x1DC);
            w_u32(address, next);
        }
        address = 0x80031200 + side * 8;
        head = r_u32(address);
        player = r_u32(0x80040FDC);
        w_u32(head + 0x1D8, player);
        head = r_u32(address);
        w_u32(player + 0x1DC, head);
        w_u32(address, player);
        w_u16(0x80059612, side);
    }
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_48(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 index, identifier, address, px, x, py, pz, z, y, dx, dy, dz, message, value;
    sint32 distance, numerator, vx, vy, vz;
    FUNCTION_MARKER(0x8009E9A8, "GAME.OVL");
    index = r_u32(0x800ED70C);
    if (index == 0xFFFFFFFF) {
        identifier = r_u32(object + 4);
        diagnostic_report(identifier, 0x80075CC8, 0x80075A1C);
    }
    index = r_u32(0x800ED70C);
    w_u32(object + 0x40, 0x800AB8A0);
    address = 0x801076EC + index * 36;
    px = r_u32(address);
    x = r_u32(object + 8);
    py = r_u32(address + 4);
    pz = r_u32(address + 8);
    z = r_u32(object + 16);
    dx = px - x;
    dz = pz - z;
    y = r_u32(object + 12);
    dy = py - y;
    distance = vector_length_approx((sint32)dx, (sint32)dy, (sint32)dz);
    numerator = (sint32)(dx * 48);
    if (distance == 0)
        trap(7);
    if (distance == -1 && numerator == (sint32)0x80000000u)
        trap(0x5D);
    vx = numerator / distance;
    numerator = (sint32)(dy * 48);
    if (distance == 0)
        trap(7);
    if (distance == -1 && numerator == (sint32)0x80000000u)
        trap(0x5D);
    vy = numerator / distance;
    numerator = (sint32)(dz * 48);
    if (distance == 0)
        trap(7);
    if (distance == -1 && numerator == (sint32)0x80000000u)
        trap(0x5D);
    vz = numerator / distance;
    w_u32(object + 0x104, 0);
    w_u32(object + 0x100, 0);
    w_u32(object + 0xFC, 0);
    w_u16(object + 0x224, (uint32)vx);
    w_u16(object + 0x226, (uint32)vy);
    w_u16(object + 0x228, (uint32)vz);
    message = r_u32(0x800F2574);
    w_u32(0x80046174, (uint32)distance);
    game_queue_hud_message(message);
    value = (uint32)random_signed_between(5, 13);
    w_u32(object + 0xA8, value);
    value = (uint32)random_signed_between(5, 13);
    w_u32(object + 0xAC, value);
    value = (uint32)random_signed_between(5, 13);
    w_u32(object + 0xB0, value);
    w_u32(object + 0xBC, 0);
    w_u32(object + 0xB8, 0);
    w_u32(object + 0xB4, 0);
    w_u32(object + 0xC0, 0);
    w_u32(object + 0xC4, 0);
    w_u32(object + 0x9C, 0);
    return 1;
}

uint32 game_ship_task_50(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags;
    FUNCTION_MARKER(0x8009ED80, "GAME.OVL");
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x80000000u);
    return 0;
}

uint32 game_ship_task_51(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 flags;
    FUNCTION_MARKER(0x8009ED98, "GAME.OVL");
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x400);
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_52(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 ship, limit, offset, category, count, flags, value, slot, format;
    FUNCTION_MARKER(0x8009EDC4, "GAME.OVL");
    if (!(r_u32(object + 0x114) & 0x8000))
        return 0;
    ship = game_get_ship_task_parameter(task, 2);
    limit = game_get_ship_task_parameter(task, 3);
    offset = ship * 60;
    category = r_u32(0x80053698 + offset);
    count = game_count_script_ship_objects(ship, category);
    if ((sint32)limit < (sint32)count)
        return 0;
    if ((sint32)limit >= r_s16(0x800536C2 + offset))
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
    sprintfPSX(0x800FA368, format, 0x80059E70 + slot * 44);
    game_queue_hud_message(0x800FA368);
    game_skip_ship_task(object, ordinal);
    return 1;
}

uint32 game_ship_task_53(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 ship, threshold;
    FUNCTION_MARKER(0x8009EF7C, "GAME.OVL");
    ship = game_get_ship_task_parameter(task, 2);
    threshold = game_get_ship_task_parameter(task, 3);
    if (r_s16(0x800536C0 + ship * 60) < (sint32)threshold) {
        w_u32(object + 0x40, 0x800A3AE0);
        w_u16(object + 0x250, 0);
        w_u16(object + 0x252, 0);
        w_u16(object + 0x254, 0);
        w_u32(object + 0xE8, 0);
        w_u32(object + 0x9C, 30);
        return 1;
    }
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_54(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, target, flags, speed, slot, format;
    FUNCTION_MARKER(0x8009F038, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near(object, identifier, 0x07FFFFFF, 0);
    if (target == 0)
        return 0;
    w_u32(target + 0x9C, 30);
    w_u32(target + 0x40, 0x800A7864);
    flags = r_u32(target + 0x114);
    identifier = r_u32(target + 4);
    w_u32(target + 0x88, 0);
    w_u32(target + 0x114, flags | 0x200000);
    speed = r_u32(0x8005369C + identifier * 60);
    w_u32(target + 0xAC, 0x07FFFFFF);
    w_u32(target + 0xE8, speed);
    slot = (uint32)r_s16(target + 0x24E);
    format = r_u32(0x800F2564);
    sprintfPSX(0x800FA368, format, 0x80059E70 + slot * 44);
    game_queue_hud_message(0x800FA368);
    game_skip_ship_task(object, ordinal);
    return 0;
}

uint32 game_ship_task_55(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 mode, time, clock, flags;
    FUNCTION_MARKER(0x8009F148, "GAME.OVL");
    mode = game_get_ship_task_parameter(task, 2);
    time = game_get_ship_task_parameter(task, 3);
    clock = r_u32(0x800311B8);
    if (time * 30 < clock) {
        if (mode != 0) {
            flags = r_u32(object + 0x114);
            w_u32(object + 0x114, flags | 0x4000);
        } else {
            flags = r_u32(object + 0x114);
            if (flags & 0x4000)
                w_u32(object + 0x114, flags ^ 0x4000);
        }
        game_skip_ship_task(object, ordinal);
    }
    return 0;
}

uint32 game_ship_task_56(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 category, time, clock, health, shield, identifier, maximum, offset, x, y, z, created;
    uint32 buffer, source, destination, index, first, second, third, fourth, callback, value;
    FUNCTION_MARKER(0x8009F1FC, "GAME.OVL");
    category = game_get_ship_task_parameter(task, 2);
    time = game_get_ship_task_parameter(task, 3);
    clock = r_u32(0x800311B8);
    if (time * 30 >= clock)
        return 0;
    if (r_u32(object) == category)
        return 0;
    shield = r_u32(object + 0x238);
    health = r_u32(object + 0x234);
    identifier = r_u32(object + 4);
    maximum = r_u32(object + 0x244);
    w_u32(object + 0x234, 10);
    w_u32(object + 0x238, 10);
    w_u32(object + 0x3C, 0x8002493C);
    w_u32(object + 0x40, 0x800D3478);
    offset = identifier * 60;
    w_u32(0x80053698 + offset, category);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    created = game_spawn_script_ship(identifier, x, y, z, 0, 0, 0);
    w_u32(created + 0x234, health);
    w_u32(created + 0x238, shield);
    w_u32(created + 0x244, maximum);
    value = (uint32)r_s16(0x800536C4 + offset);
    w_u32(created + 0xDC, value);
    buffer = heap_alloc_back(1024);
    w_u32(created + 0xF4, buffer);
    source = r_u32(object + 0xF4);
    destination = buffer;
    for (index = 0; index < 0x3F0; index += 16) {
        first = r_u32(source + index);
        second = r_u32(source + index + 4);
        third = r_u32(source + index + 8);
        fourth = r_u32(source + index + 12);
        w_u32(destination + index, first);
        w_u32(destination + index + 4, second);
        w_u32(destination + index + 8, third);
        w_u32(destination + index + 12, fourth);
    }
    first = r_u32(source + 0x3F0);
    second = r_u32(source + 0x3F4);
    third = r_u32(source + 0x3F8);
    fourth = (uint32)r_s16(source + 0x3FC);
    w_u32(destination + 0x3F0, first);
    w_u32(destination + 0x3F4, second);
    w_u32(destination + 0x3F8, third);
    w_u16(destination + 0x3FC, fourth);
    value = (uint32)r_s8(source + 0x3FE);
    w_u8(destination + 0x3FE, value);
    identifier = r_u32(created + 4);
    callback = game_find_ship_start_callback(identifier);
    psx_dispatch_guest_call(callback, 1, created, 0, 0);
    w_u32(created, category);
    w_u32(created + 0x40, 0x8009AB58);
    value = r_u32(object + 0x20);
    w_u32(created + 0x20, value);
    value = r_u32(object + 0x24);
    w_u32(created + 0x24, value);
    value = r_u32(object + 0x28);
    w_u32(created + 0x28, value);
    value = r_u16(object + 0x24C);
    w_u16(created + 0x24C, value);
    matrix_copy_rotation(created + 0x11C, object + 0x11C);
    return 1;
}

uint32 game_ship_task_57(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, target;
    FUNCTION_MARKER(0x8009F404, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_select_ship_task_target(object, 0, 0, identifier, 0);
    if (target == 0 || (r_u32(target + 0x114) & 0x00400000))
        return 0;
    w_u32(object + 0x40, 0x800A3AE0);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x252, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x9C, 30);
    return 1;
}

uint32 game_ship_task_58(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, target, state;
    FUNCTION_MARKER(0x8009F494, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_select_ship_task_target(object, 0, 0, identifier, 0);
    if (target == 0 || (r_u32(target + 0x114) & 0x40000)) {
        state = r_u32(object + 0x208);
        if (state != 0 && r_u32(state + 0x38) != 0)
            return 0;
    }
    w_u32(object + 0x40, 0x800A3AE0);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x252, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x9C, 30);
    return 1;
}

uint32 game_ship_task_59(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 ship;
    FUNCTION_MARKER(0x8009F540, "GAME.OVL");
    ship = game_get_ship_task_parameter(task, 2);
    if (r_s16(0x800536BE + ship * 60) <= 0)
        return 0;
    w_u32(object + 0x40, 0x800A3AE0);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x252, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0xE8, 0);
    w_u32(object + 0x9C, 30);
    return 1;
}

static void game_task_append_message_native(uint32 suffix)
{
    const char *text = game_task_text_native(0x800FA368);
    const char *tail = game_task_text_native(suffix);
    size_t length = strlen(text) + strlen(tail) + 1;
    strcat((char *)psx_addr(0x800FA368, length), tail);
}

uint32 game_ship_task_49(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 threshold, clock, start, clock_product_high, index, slot, format, identifier;
    const char *native_format, *name, *second;
    FUNCTION_MARKER(0x8009EBAC, "GAME.OVL");
    if (r_u32(0x800595C8) == 1)
        return 0;
    threshold = game_get_ship_task_parameter(task, 2);
    clock = r_u32(0x800311B8);
    start = r_u32(0x800595B8);
    clock_product_high = (uint32)(((uint64)(clock - start) * 0x88888889u) >> 32);
    if (threshold >= (clock_product_high >> 4))
        return 0;
    index = r_u32(0x800ED710);
    if (index != 0xFFFFFFFF) {
        slot = (uint32)r_s16(object + 0x24E);
        format = r_u32(0x800F257C);
        sprintfPSX(0x800FA368, format, 0);
        index = r_u32(0x800ED710);
        w_u32(object + 0xBC, index);
        game_task_append_message_native(0x80075CE0);
        w_u32(object + 0x40, 0x800ABA30);
    } else {
        index = r_u32(0x800ED714);
        if (index != 0xFFFFFFFF) {
            slot = (uint32)r_s16(object + 0x24E);
            format = r_u32(0x800F2578);
            native_format = game_task_text_native(format);
            name = game_task_text_native(0x80059E70 + slot * 44);
            second = game_task_text_native(clock_product_high);
            sprintf((char *)psx_addr(0x800FA368, 0x100), native_format, name, second);
            index = r_u32(0x800ED714);
            w_u32(object + 0xBC, index);
            game_task_append_message_native(0x80075CE8);
            w_u32(object + 0x40, 0x800AC008);
        } else {
            identifier = r_u32(object + 4);
            diagnostic_report(identifier, 0x80075CF0, 0x80075A1C);
        }
    }
    game_skip_ship_task(object, ordinal);
    w_u32(object + 0x9C, 900);
    w_u32(object + 0xA8, 0);
    w_u32(object + 0xAC, 0);
    game_queue_hud_message(0x800FA368);
    return 1;
}

uint32 game_ship_task_42(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor)
{
    uint32 identifier, target, player, last_z_magnitude, format, callback;
    FUNCTION_MARKER(0x8009E490, "GAME.OVL");
    identifier = game_get_ship_task_parameter(task, 2);
    target = game_find_script_ship_near_with_distance_input(object, identifier, 0x07FFFFFF, 0, &last_z_magnitude);
    if (target == 0)
        return 0;
    player = r_u32(0x80040FDC);
    if (target == player) {
        format = r_u32(0x800F2580);
        sprintfPSX(0x800FA368, format, last_z_magnitude);
        stream_enqueue(400, 0, game_task_text_native(0x800FA368));
        (void)r_u32(0x80040FDC);
        game_fail_current_mission();
    } else {
        callback = r_u32(target + 0x5C);
        psx_dispatch_guest_call(callback, 1, target, 0, 0);
    }
    game_skip_ship_task(object, ordinal);
    return 0;
}

void game_replace_ship_for_category(uint32 object, uint32 category)
{
    uint32 identifier, shield, health, maximum, x, y, z, created, value, buffer, source;
    uint32 index, first, second, third, fourth, model, table, mesh, callback;
    FUNCTION_MARKER(0x800A28EC, "GAME.OVL");
    if (r_u32(object) == category)
        return;
    identifier = r_u32(object + 4);
    shield = r_u32(object + 0x238);
    health = r_u32(object + 0x234);
    maximum = r_u32(object + 0x244);
    w_u32(0x80053698 + identifier * 60, 3);
    identifier = r_u32(object + 4);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    created = game_spawn_script_ship(identifier, x, y, z, 0, 0, 0);
    w_u32(created + 0x234, health);
    w_u32(created + 0x238, shield);
    w_u32(created + 0x244, maximum);
    identifier = r_u32(object + 4);
    value = (uint32)r_s16(0x800536C4 + identifier * 60);
    w_u32(created + 0xDC, value);
    buffer = heap_alloc_back(1024);
    w_u32(created + 0xF4, buffer);
    source = r_u32(object + 0xF4);
    for (index = 0; index < 1024; index += 16) {
        first = r_u32(source + index);
        second = r_u32(source + index + 4);
        third = r_u32(source + index + 8);
        fourth = r_u32(source + index + 12);
        w_u32(buffer + index, first);
        w_u32(buffer + index + 4, second);
        w_u32(buffer + index + 8, third);
        w_u32(buffer + index + 12, fourth);
    }
    model = r_u32(0x800EDD3C);
    identifier = r_u32(created + 4);
    w_u32(created + 0x74, model);
    table = r_u32(0x80046178);
    mesh = r_u32(table + model * 132);
    w_u32(created + 0x64, mesh);
    callback = game_find_ship_start_callback(identifier);
    psx_dispatch_guest_call(callback, 1, created, 0, 0);
    value = r_u32(object + 0x20);
    w_u32(created + 0x20, value);
    value = r_u32(object + 0x24);
    w_u32(created + 0x24, value);
    value = r_u32(object + 0x28);
    w_u32(created + 0x28, value);
    value = r_u16(object + 0x24C);
    w_u16(created + 0x24C, value);
    matrix_copy_rotation(created + 0x11C, object + 0x11C);
    callback = r_u32(object + 0x48);
    w_u32(object + 0x238, 1);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}

void game_tick_ship_task_delay(uint32 object)
{
    uint32 ticks;
    FUNCTION_MARKER(0x800A3A70, "GAME.OVL");
    ticks = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks <= 0)
        w_u32(object + 0x40, 0x8009AB58);
}

void game_move_ship_and_tick_task_delay(uint32 object)
{
    uint32 ticks;
    FUNCTION_MARKER(0x800A3A98, "GAME.OVL");
    game_update_object_movement(object);
    ticks = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks <= 0)
        w_u32(object + 0x40, 0x8009AB58);
}

void game_update_ship_task_wait(uint32 object)
{
    uint32 flags, ticks;
    FUNCTION_MARKER(0x800A3AE0, "GAME.OVL");
    game_update_object_state(object);
    w_u32(object + 0xFC, 0);
    w_u32(object + 0x100, 0);
    w_u32(object + 0x104, 0);
    game_update_object_movement(object);
    flags = r_u32(object + 0x114);
    ticks = r_u32(object + 0x9C);
    w_u32(object + 0x114, flags | 0x2000);
    --ticks;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks <= 0)
        w_u32(object + 0x40, 0x8009AB58);
}

void game_begin_ship_offset_follow_maneuver(uint32 object)
{
    uint32 target, identifier, yaw, speed, pitch, distance;
    FUNCTION_MARKER(0x800A457C, "GAME.OVL");
    target = r_u32(object + 0x170);
    game_steer_to_target_offset(object, target, object + 0x214, 0);
    identifier = r_u32(object + 4);
    yaw = r_u32(0x80033D1C);
    speed = r_u32(0x8005369C + identifier * 60);
    pitch = r_u32(0x80033D30);
    w_u32(object + 0xE8, speed);
    game_begin_yaw_maneuver(object, 30, 0x8009AB58, yaw, pitch, 0x800A461C);
    distance = r_u32(0x80046174);
    w_u32(object + 0xA8, distance);
    game_update_object_movement(object);
}

void game_update_ship_retreat_task(uint32 object)
{
    uint32 target, yaw, pitch, distance, range;
    FUNCTION_MARKER(0x800A48F4, "GAME.OVL");
    target = r_u32(object + 0x170);
    if (r_u32(target) == 0) {
        w_u32(object + 0x40, 0x8009AB58);
    } else {
        game_evaluate_target_occlusion(object, target, 1);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u16(object + 0x250, yaw);
        w_u16(object + 0x252, pitch);
    }
    game_update_object_state(object);
    game_update_object_movement(object);
    distance = r_u32(0x80046174);
    range = r_u32(object + 0xA8);
    if ((sint32)distance < (sint32)range)
        w_u32(object + 0x40, 0x8009AB58);
    else
        game_tick_ship_task_delay(object);
}

void game_update_ship_offset_follow_maneuver(uint32 object)
{
    uint32 identifier, target, speed, target_speed, current, generation, distance, phase, quotient, frame;
    uint32 yaw, pitch;
    FUNCTION_MARKER(0x800A461C, "GAME.OVL");
    identifier = r_u32(object + 4);
    target = r_u32(object + 0x170);
    speed = r_u32(0x8005369C + identifier * 60);
    target_speed = r_u32(target + 0xEC) * 2;
    if ((sint32)speed < (sint32)target_speed)
        speed = target_speed;
    if (target == 0)
        goto invalid_target;
    current = r_u16(target + 0x24A);
    generation = r_u16(object + 0x174);
    if (current != generation)
        goto invalid_target;
    game_steer_to_target_offset(object, target, object + 0x214, 0);
    distance = r_u32(0x80046174);
    if ((sint32)distance >= 20000)
        w_u32(object + 0xE8, speed);
    else
        w_u32(object + 0xE8, (uint32)((sint32)(distance * speed) / 20000));
    generation = r_u16(object + 0x24A);
    quotient = (uint32)(((uint64)generation * 0x88888889u) >> 32) >> 4;
    phase = (generation - quotient * 30) & 0xFFFF;
    frame = r_u32(0x800311B8);
    if (frame + phase == 4)
        w_u32(object + 0x40, 0x8009AB58);
    if (r_u32(target + 0xEC) == 0 && r_s32(0x80046174) < 5000) {
        game_evaluate_target_occlusion(object, target, 0);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u16(object + 0x250, 0 - yaw);
        w_u16(object + 0x252, 0 - pitch);
        game_update_object_state(object);
    }
    return;
invalid_target:
    w_u32(object + 0xE0, 123);
    w_u32(object + 0x40, 0x8009AB58);
}

void game_update_changed_ship_threats(void)
{
    uint32 classes[2] = {3, 1};
    uint32 group, object, flags, target, previous, generation, callback;
    FUNCTION_MARKER(0x800A4E20, "GAME.OVL");
    for (group = 0; group < 2; ++group) {
        object = object_lookup(classes[group]);
        while (object != 0) {
            if (r_u32(object) != 0) {
                flags = r_u32(object + 0x114);
                if (!(flags & 0x10)) {
                    target = r_u32(object + 0x190);
                    if (target != 0) {
                        previous = r_u32(object + 0x198);
                        if (target != previous && !(flags & 0x2000)) {
                            generation = r_u16(object + 0x194);
                            callback = r_u32(object + 0x58);
                            w_u32(object + 0x198, target);
                            w_u16(object + 0x19C, generation);
                            psx_dispatch_guest_call(callback, 2, object, target, 0);
                        }
                    }
                }
            }
            object = r_u32(object + 0x1DC);
        }
    }
}

void game_update_escape_pod_movement(uint32 object)
{
    uint32 target, yaw, lifetime, roll, ticks, scale, tx, x, ty, y, tz, z, distance, model, models, radius;
    FUNCTION_MARKER(0x800A4F00, "GAME.OVL");
    target = r_u32(object + 0x170);
    if (target != 0 && r_u32(target) == 0)
        w_u32(object + 0x170, 0);
    yaw = r_u32(object + 0xA8);
    lifetime = r_u32(object + 0xB0);
    roll = r_u32(object + 0xAC);
    --lifetime;
    w_u32(object + 0x20, yaw);
    w_u32(object + 0x28, roll);
    w_u32(object + 0xB0, lifetime);
    if (target != 0) {
        ticks = r_u32(object + 0x9C) - 1;
        w_u32(object + 0x9C, ticks);
        if ((sint32)ticks <= 0) {
            scale = r_u32(target + 0x84);
            if ((sint32)scale < 0)
                scale = 0;
            tx = r_u32(target + 8);
            x = r_u32(object + 8);
            ty = r_u32(target + 12);
            y = r_u32(object + 12);
            tz = r_u32(target + 16);
            z = r_u32(object + 16);
            distance = (uint32)vector_length_approx((sint32)(tx - x), (sint32)(ty - y), (sint32)(tz - z));
            model = r_u32(object + 0x74);
            models = r_u32(0x80046178);
            radius = r_u32(models + model * 132 + 0x6C);
            w_u32(0x80046174, distance);
            radius = (radius << (scale & 31)) + 30000;
            if ((sint32)radius < (sint32)distance)
                w_u32(object + 0x170, 0);
            ticks = (uint32)random_between(90, 150);
            w_u32(object + 0x9C, ticks);
        }
    } else if ((sint32)lifetime <= 0) {
        w_u32(object + 0xE8, 0);
    }
    game_update_object_state(object);
    game_update_object_movement(object);
}

void game_update_carrier_ship_launch(uint32 object)
{
    uint32 carrier, callback, bay, ticks, flags, value, saved_flags, pitch, yaw;
    FUNCTION_MARKER(0x800A3CAC, "GAME.OVL");
    carrier = r_u32(object + 0x160);
    if (carrier == 0) {
        callback = r_u32(object + 0x48);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
        return;
    }
    game_update_object_state(object);
    game_update_object_movement(object);
    bay = r_u32(object + 0xA8);
    if (r_s16(bay + 0x74) < 4)
        game_draw_carrier_launch_bay(carrier, bay);
    ticks = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks <= 0 && r_s16(bay + 0x74) == 1) {
        w_u16(bay + 0x74, 3);
        w_u16(bay + 0x76, 10);
        w_u16(bay + 0x78, 0xFFFF);
        w_u16(bay + 0x7A, 0);
        w_u32(object + 0x9C, 40);
    }
    if (r_s16(bay + 0x74) < 4)
        return;
    ticks = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks > 0)
        return;
    flags = r_u32(object + 0x114);
    if (flags & 8)
        w_u32(object + 0x114, flags ^ 8);
    callback = r_u32(object + 0x4C);
    w_u32(object + 0x4C, 0x8002493C);
    w_u32(object + 0x58, callback);
    w_u16(bay, 0);
    w_u32(object + 0x44, 0x8009FA2C);
    value = (uint32)random_signed_between(20, 40);
    w_u16(object + 0x250, value);
    value = (uint32)random_signed_between(5, 20);
    saved_flags = r_u32(object + 0xC0);
    pitch = r_u32(0x80033D30);
    yaw = r_u32(0x80033D1C);
    w_u16(object + 0x252, value);
    w_u32(object + 0x114, saved_flags);
    game_begin_yaw_maneuver(object, 30, 0x8009AB58, yaw, pitch, 0x8002493C);
}

void game_update_carrier_ship_docking(uint32 object)
{
    uint32 carrier, category, bay, state, identifier, speed, carrier_speed, yaw, pitch, distance, previous;
    uint32 flags, first, second, angle, turn, stored, value, callback, slot, format;
    sint32 own_first, own_second, carrier_first, carrier_second;
    FUNCTION_MARKER(0x800A49A4, "GAME.OVL");
    carrier = r_u32(object + 0x170);
    category = r_u32(carrier);
    bay = r_u32(object + 0x20C);
    if (category == 0) {
        w_u32(object + 0x40, 0x8009AB58);
        return;
    }
    state = r_u32(object + 0x88);
    switch (state) {
    case 1:
        identifier = r_u32(object + 4);
        speed = r_u32(0x8005369C + identifier * 60);
        w_u32(object + 0xE8, speed);
        carrier_speed = r_u32(carrier + 0xEC);
        if ((sint32)speed < (sint32)carrier_speed)
            w_u32(object + 0xE8, carrier_speed + 0x1400);
        game_steer_to_target_offset(object, carrier, bay + 0x44, 0);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u16(object + 0x254, 0);
        w_u16(object + 0x250, yaw);
        w_u16(object + 0x252, pitch);
        game_update_object_state(object);
        game_update_object_movement(object);
        distance = r_u32(0x80046174);
        if ((sint32)distance < 1000)
            goto begin_docking_alignment;
        if ((sint32)distance < 5000) {
            previous = r_u32(object + 0xAC);
            if ((sint32)distance < (sint32)previous)
                goto begin_docking_alignment;
        }
        distance = r_u32(0x80046174);
        w_u32(object + 0xAC, distance);
        return;
    begin_docking_alignment:
        w_u32(object + 0x14, 0);
        w_u32(object + 0x18, 0);
        w_u32(object + 0x1C, 0);
        w_u32(object + 0x88, 2);
        return;
    case 2:
        flags = r_u32(object + 0x114);
        w_u32(object + 0x114, flags | 0x2040);
        game_position_ship_at_carrier_offset(carrier, bay + 0x44, object);
        game_steer_to_target_offset(object, carrier, bay + 0x34, 0);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u16(object + 0x254, 0);
        w_u16(object + 0x250, yaw);
        w_u16(object + 0x252, pitch);
        game_update_object_state(object);
        if (r_u32(0x80033D1C) != 0 || r_u32(0x80033D30) != 0)
            return;
        w_u32(object + 0x88, 3);
        w_u16(bay + 0x78, 1);
        w_u16(bay + 0x7A, 10);
        w_u16(bay + 0x76, 0);
        w_u16(bay + 0x74, 0);
        w_u32(object + 0xB0, 0xFFFFFFFF);
        return;
    case 3:
        flags = r_u32(object + 0x114);
        own_first = r_s16(object + 0x124);
        own_second = r_s16(object + 0x122);
        w_u32(object + 0x114, flags | 0x2000);
        carrier_first = r_s16(carrier + 0x124);
        carrier_second = r_s16(carrier + 0x122);
        angle = 1024 - (uint32)ratan2(carrier_first - own_first, carrier_second - own_second);
        turn = (sint32)angle > 0 ? 1 : ((sint32)angle < 0 ? 2 : 0);
        w_u32(object + 0xAC, turn);
        stored = r_u32(object + 0xB0);
        turn = r_u32(object + 0xAC);
        if (stored == turn) {
            game_update_object_state(object);
            w_u16(object + 0x254, angle);
        } else if (stored != 0xFFFFFFFF) {
            w_u32(object + 0xAC, 0);
        }
        value = r_u32(object + 0xAC);
        turn = r_u32(object + 0xAC);
        w_u32(object + 0xB0, value);
        if (turn == 0)
            w_u32(object + 0x88, 4);
        return;
    case 4:
        first = r_u32(object + 0x170);
        w_u32(object + 0x44, 0x8002493C);
        game_draw_carrier_launch_bay(first, bay);
        flags = r_u32(object + 0x114);
        w_u32(object + 0x114, flags | 0x2000);
        game_steer_to_target_offset(object, carrier, bay + 0x34, 0);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u16(object + 0x254, 0);
        w_u16(object + 0x250, yaw);
        w_u16(object + 0x252, pitch);
        game_update_object_state(object);
        game_update_object_movement(object);
        if (r_s32(0x80046174) >= 2000)
            return;
        w_u32(object + 0x88, 5);
        w_u32(object + 0x3C, 0x8002493C);
        w_u16(bay + 0x76, 10);
        w_u16(bay + 0x78, 0xFFFF);
        w_u16(bay + 0x7A, 0);
        return;
    case 5:
        first = r_u32(object + 0x170);
        game_draw_carrier_launch_bay(first, bay);
        first = (uint32)r_s16(bay + 0x76);
        second = (uint32)r_s16(bay + 0x7A);
        if (first != second)
            return;
        slot = (uint32)r_s16(object + 0x24E);
        format = r_u32(0x800F23B4);
        sprintfPSX(0x800FA368, format, 0x80059E70 + slot * 44);
        game_queue_hud_message(0x800FA368);
        w_u32(object + 0x20C, 0);
        w_u16(bay, 0);
        callback = r_u32(object + 0x48);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
        return;
    default:
        return;
    }
}

uint32 game_find_active_ship_by_identifier(uint32 identifier, uint32 category)
{
    uint32 slot, object, current, generation;
    FUNCTION_MARKER(0x800A268C, "GAME.OVL");
    if ((sint32)identifier >= 1000) {
        slot = (identifier - 1000) * 44;
        object = r_u32(0x80059E68 + slot);
        if (object == 0)
            return 0;
        current = r_u16(object + 0x24A);
        generation = r_u16(0x80059E6C + slot);
        if (current != generation || (r_u32(object + 0x114) & 0x8000))
            return 0;
        return object;
    }
    object = object_lookup(category);
    while (object != 0) {
        if (r_u32(object + 4) == identifier && r_u32(object) != 0 && !(r_u32(object + 0x114) & 0x8000))
            return object;
        object = r_u32(object + 0x1DC);
    }
    return 0;
}

static uint32 game_distance_to_nearest_large_ship_body(uint32 position, const uint32 *native_position)
{
    uint32 classes[3], group, object, x, px, y, py, pz, z, distance, nearest = 0x07FFFFFF;
    classes[0] = r_u32(0x80075D08);
    classes[1] = r_u32(0x80075D0C);
    classes[2] = r_u32(0x80075D10);
    for (group = 0; group < 2; ++group) {
        object = object_lookup(classes[group]);
        while (object != 0) {
            if (r_u32(object + 0x114) & 0x10) {
                x = r_u32(object + 8);
                px = native_position ? native_position[0] : r_u32(position);
                y = r_u32(object + 12);
                py = native_position ? native_position[1] : r_u32(position + 4);
                pz = native_position ? native_position[2] : r_u32(position + 8);
                z = r_u32(object + 16);
                distance = (uint32)vector_length_approx((sint32)(x - px), (sint32)(y - py), (sint32)(z - pz));
                w_u32(0x8011F130, distance);
                if ((sint32)distance < (sint32)nearest)
                    nearest = distance;
            }
            object = r_u32(object + 0x1DC);
        }
    }
    return nearest;
}

uint32 game_distance_to_nearest_large_ship(uint32 position)
{
    FUNCTION_MARKER(0x800A138C, "GAME.OVL");
    return game_distance_to_nearest_large_ship_body(position, NULL);
}

uint32 game_distance_to_nearest_large_ship_native(const uint32 position[3])
{
    return game_distance_to_nearest_large_ship_body(0, position);
}

void game_update_distant_rotating_model(uint32 object)
{
    uint32 accumulated, step, yaw, ticks, low, player, model, x, y, px, z, py, pz;
    uint32 distance, scale, fixed_model, models, mesh;
    FUNCTION_MARKER(0x800A358C, "GAME.OVL");
    accumulated = r_u32(object + 0xAC);
    step = r_u32(object + 0xA8);
    accumulated += step;
    yaw = r_u32(object + 0x20);
    w_u32(object + 0xAC, accumulated);
    yaw += (uint32)((sint32)accumulated >> 8);
    w_u32(object + 0x20, yaw);
    ticks = r_u32(object + 0x9C);
    low = r_u8(object + 0xAC);
    --ticks;
    w_u32(object + 0x9C, ticks);
    w_u32(object + 0xAC, low);
    if ((sint32)ticks > 0)
        return;
    player = r_u32(0x80040FDC);
    x = r_u32(object + 8);
    model = r_u32(object + 0xB4);
    y = r_u32(object + 12);
    px = r_u32(player + 8);
    z = r_u32(object + 16);
    py = r_u32(player + 12);
    pz = r_u32(player + 16);
    distance = (uint32)vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
    scale = r_u32(object + 0x84);
    fixed_model = r_u32(object + 0xB8);
    distance <<= scale & 31;
    if (fixed_model == 0 && distance > 0x8CE20000u)
        ++model;
    models = r_u32(0x80046178);
    w_u32(object + 0x74, model);
    mesh = r_u32(models + model * 132);
    w_u32(object + 0x9C, 5400);
    w_u32(object + 0x64, mesh);
}

void game_update_belt_asteroid_spawner(uint32 controller)
{
    uint32 chance, probability, effect, player, x, y, px, py, pz, z, distance;
    uint32 payload, axis, coefficient, offset, position;
    FUNCTION_MARKER(0x800A5380, "GAME.OVL");
    chance = (uint32)random_between(0, 100);
    probability = r_u32(controller + 0xAC);
    if ((sint32)chance >= (sint32)probability)
        return;
    if (r_s32(0x800595CC) >= 80)
        return;
    effect = game_create_belt_asteroid(controller);
    player = r_u32(0x80040FDC);
    x = r_u32(controller + 8);
    y = r_u32(controller + 12);
    px = r_u32(player + 8);
    py = r_u32(player + 12);
    pz = r_u32(player + 16);
    z = r_u32(controller + 16);
    distance = (uint32)vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
    if ((sint32)distance > 199999)
        return;
    payload = r_u32(effect + 0x6C);
    for (axis = 0; axis < 3; ++axis) {
        coefficient = (uint32)r_s16(payload + 8 + axis * 6);
        offset = (uint32)((sint32)(coefficient * 400000) >> 12);
        position = r_u32(effect + axis * 4);
        if (axis < 2)
            payload = r_u32(effect + 0x6C);
        w_u32(effect + axis * 4, position - offset);
    }
}

void game_process_object_list_contacts(uint32 head, uint32 object, sint32 range)
{
    uint32 updated, frame, kind, flags, first, second, dx, dy, dz, hit, generation, profile, callback;
    FUNCTION_MARKER(0x800A1748, "GAME.OVL");
    while (head != 0) {
        updated = r_u32(head + 0xF8) + 1;
        frame = r_u32(0x800311B8);
        if (frame < updated)
            goto next_contact;
        kind = r_u32(head);
        if (kind == 0 || kind == 0xD4)
            goto next_contact;
        flags = r_u32(object + 0x114);
        if (!(flags & 0x800)) {
            first = r_u32(object + 8);
            second = r_u32(head + 8);
            dx = first - second;
            if ((sint32)dx < 0)
                dx = 0 - dx;
            if (range < (sint32)dx)
                goto next_contact;
            first = r_u32(object + 12);
            second = r_u32(head + 12);
            dy = first - second;
            if ((sint32)dy < 0)
                dy = 0 - dy;
            if (range < (sint32)dy)
                goto next_contact;
            first = r_u32(object + 16);
            second = r_u32(head + 16);
            dz = first - second;
            if ((sint32)dz < 0)
                dz = 0 - dz;
            if (range < (sint32)dz)
                goto next_contact;
            hit = range < vector_length_approx((sint32)dx, (sint32)dy, (sint32)dz);
        } else {
            hit = game_check_swept_object_collision(object, head);
        }
        if (hit != 0) {
            w_u32(head + 0x178, object);
            generation = r_u16(object + 0x24A);
            w_u16(head + 0x17C, generation);
            kind = r_u32(head);
            profile = r_u32(head + 0x1E4);
            callback = r_u32(object + 0x44);
            game_dispatch_object_hit(callback, object, head + 8, head + 0xFC, head, 0, kind, profile);
        }
next_contact:
        head = r_u32(head + 0x1DC);
    }
}

static void game_emit_velocity_fragments_body(uint32 position, const VECTOR *native_position, uint32 velocity, const VECTOR *native_velocity, uint32 count)
{
    uint32 active = r_u32(0x80042178);
    uint32 budget = r_u32(0x80010238);
    uint32 index, model, effect, payload, value, scale, component, axis, delta, total;
    if ((sint32)budget < (sint32)active)
        count = (uint32)((sint32)count >> 1);
    for (index = 0; (sint32)index < (sint32)count; ++index) {
        model = r_u32(0x800EDFCC);
        effect = native_position != NULL ? game_create_model_effect_native(native_position, (sint16)(model + 3)) : game_create_model_effect(position, (sint16)(model + 3));
        payload = r_u32(effect + 0x6C);
        w_u16(effect + 0x3C, 107);
        w_u16(effect + 0x46, 0);
        w_u32(payload, 0);
        value = (uint32)random_between(-4, -3);
        w_u16(effect + 0x34, value);
        scale = (uint32)random_between(54, 114);
        for (axis = 0; axis < 3; ++axis) {
            value = (uint32)random_between(-50, 50);
            component = native_velocity != NULL ? (uint32)(axis == 0 ? native_velocity->vx : axis == 1 ? native_velocity->vy : native_velocity->vz) : r_u32(velocity + axis * 4);
            delta = (uint32)((sint32)(scale * component) >> 6);
            w_u32(effect + 0x20 + axis * 4, value - delta);
        }
        for (axis = 0; axis < 3; ++axis) {
            value = (uint32)random_signed_between(3, 24);
            payload = r_u32(effect + 0x6C);
            w_u16(payload + 0x34 + axis * 2, value);
        }
        w_u32(effect + 0x60, 0x8002493C);
        w_u32(effect + 0x2C, 210);
        w_u32(effect + 0x30, 0x07FFFFFF);
        total = r_u32(0x800595CC);
        w_u32(0x800595CC, total + 1);
    }
}

void game_emit_velocity_fragments(uint32 position, uint32 velocity, uint32 count)
{
    FUNCTION_MARKER(0x800A31C0, "GAME.OVL");
    game_emit_velocity_fragments_body(position, NULL, velocity, NULL, count);
}

void game_emit_velocity_fragments_native(const VECTOR *position, const VECTOR *velocity, uint32 count)
{
    game_emit_velocity_fragments_body(0, position, 0, velocity, count);
}

void game_create_controller_behind_player(void)
{
    uint32 player = r_u32(0x80040FDC);
    uint32 position[3], coefficient, axis, index, offset, object;
    FUNCTION_MARKER(0x800A1140, "GAME.OVL");
    position[0] = r_u32(player + 8);
    position[1] = r_u32(player + 12);
    position[2] = r_u32(player + 16);
    for (axis = 0; axis < 3; ++axis) {
        coefficient = (uint32)(sint32)r_s16(player + 0x128 + axis * 2);
        position[axis] -= (uint32)((sint32)(coefficient * 80000u) >> 12);
    }
    while ((sint32)game_distance_to_nearest_large_ship_native(position) <= 79999) {
        player = r_u32(0x80040FDC);
        for (axis = 0; axis < 3; ++axis) {
            coefficient = (uint32)(sint32)r_s16(player + 0x122 + axis * 2);
            position[axis] -= (uint32)((sint32)(coefficient * 20000u) >> 12);
        }
    }
    for (index = 0, offset = 0; index < 20; ++index, offset += 60) {
        if (r_u32(0x80053698 + offset) != 1)
            continue;
        if (r_s16(0x800536BC + offset) != 1)
            continue;
        object = game_create_object(0, position[0], position[1], position[2], 0);
        matrix_set_identity(object + 0x11C);
        w_u32(object + 0x40, 0x800A7CCC);
        w_u32(object + 0x88, 0);
        w_u32(object + 0xAC, index);
        w_u32(object + 0xB0, 0xFFFFFFFF);
        w_u32(object + 0xB4, 1);
        w_u32(object + 0xB8, 0);
        w_u32(object, 0);
        w_u32(object + 4, index);
        break;
    }
}

void game_finalize_ship_removal(uint32 object)
{
    uint32 identifier, counter, tracked;
    FUNCTION_MARKER(0x800A5030, "GAME.OVL");
    if (r_s32(object + 0x238) > 0 && (identifier = r_u32(object + 4)) - 1u < 19u) {
        counter = 0x800536C0 + identifier * 60;
        w_u16(counter, (uint16)(r_u16(counter) + 1));
    } else {
        game_emit_object_debris(object);
        identifier = r_u32(object + 4);
        counter = 0x800536BE + identifier * 60;
        w_u16(counter, (uint16)(r_u16(counter) + 1));
    }
    tracked = r_u32(0x800EF4E0);
    w_u32(object + 0x118, 0);
    if (object == tracked)
        w_u32(0x800EF4E0, 0);
    if (object == r_u32(0x800EF4E4))
        w_u32(0x800EF4E4, 0);
    game_mark_object_for_removal(object);
}

void game_update_delayed_ship_behavior(uint32 object)
{
    uint32 ticks, flags, data, player, generation, yaw, pitch;
    FUNCTION_MARKER(0x800A8400, "GAME.OVL");
    game_update_object_state(object);
    game_update_object_movement(object);
    ticks = r_u32(object + 0x9C) - 1u;
    w_u32(object + 0x9C, ticks);
    if ((sint32)ticks > 0)
        return;
    if (r_u32(object + 0xC0) == 0) {
        if (r_u32(object + 0x114) & 0x10) {
            data = r_u32(object + 0x110);
            w_u32(object + 0x40, 0x8009AB58);
            if (r_u8(data + 0x46F) == 2)
                w_u8(data + 0x46F, 1);
        } else {
            yaw = (uint32)random_signed_between(80, 880);
            w_u32(0x80033D1C, yaw);
            pitch = (uint32)random_signed_between(40, 280);
            w_u32(0x80033D30, pitch);
            yaw = r_u32(0x80033D1C);
            game_begin_yaw_maneuver(object, 30, 0x8009AB58, yaw, pitch, 0x8002493C);
        }
        return;
    }
    flags = r_u32(object + 0x114);
    if (flags & 0x10)
        w_u32(object + 0x114, flags | 0x10);
    player = r_u32(0x80040FDC);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x178, player);
    generation = r_u16(player + 0x24A);
    w_u32(object + 0x114, flags | 0x10000000);
    w_u32(object + 0x58, 0x800D9D90);
    w_u32(object + 0x40, 0x800DB30C);
    w_u16(object + 0x17C, (uint16)generation);
}

void game_draw_and_select_target(uint32 object)
{
    uint32 row, camera, x, y, z, magnitude, selection, mode;
    sint32 screen, projection, flags, depth, maximum, sx, sy;
    SVECTOR vertex;
    FUNCTION_MARKER(0x800A5E84, "GAME.OVL");
    if ((sint32)object_draw_visible(object) > 0)
        goto clear_target;
    row = r_u32(0x800EF4CC) * 52;
    if (r_s16(0x80119C60 + row) != 3)
        return;
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    camera = r_u32(0x80040C94);
    x = r_u32(object + 8);
    x -= r_u32(camera + 8);
    magnitude = (sint32)x < 0 ? 0u - x : x;
    if (((sint32)magnitude >> 2) >= 32000)
        goto clear_target;
    y = r_u32(object + 12);
    y -= r_u32(camera + 12);
    magnitude = (sint32)y < 0 ? 0u - y : y;
    if (((sint32)magnitude >> 2) >= 32000)
        goto clear_target;
    z = r_u32(object + 16);
    z -= r_u32(camera + 16);
    magnitude = (sint32)z < 0 ? 0u - z : z;
    if (((sint32)magnitude >> 2) >= 32000)
        goto clear_target;
    vertex.vx = (sint16)((sint32)x >> 2);
    y = r_u32(object + 12);
    y -= r_u32(camera + 12);
    vertex.vy = (sint16)((sint32)y >> 2);
    z = r_u32(object + 16);
    z -= r_u32(camera + 16);
    vertex.vz = (sint16)((sint32)z >> 2);
    depth = RotTransPers(&vertex, &screen, &projection, &flags);
    w_u16(object + 0x108, (uint16)screen);
    sy = (sint16)((uint32)screen >> 16);
    row = r_u32(0x800EF4CC) * 52;
    w_u32(object + 0x10C, (uint32)depth);
    w_u16(object + 0x10A, (uint16)sy);
    selection = r_u32(0x80119C54 + row);
    maximum = 32000;
    if (selection == 6) {
        mode = r_u32(0x800311C0);
        w_u32(0x80053688, 64);
        w_u32(0x8005368C, 64);
        if (mode != 0)
            w_u32(0x8005368C, 68);
        maximum = 4000;
    }
    if (depth < 201 || depth >= maximum)
        goto clear_target;
    sx = (sint16)(uint16)screen;
    if (sx <= 0 || sx >= r_s32(0x8003119C))
        goto clear_target;
    if (sy <= 0 || sy >= r_s32(0x800311B0))
        goto clear_target;
    if (r_u32(0x800EF4E0) == 0)
        w_u32(0x800EF4E0, object);
    return;
clear_target:
    if (r_u32(0x800EF4E0) == object)
        w_u32(0x800EF4E0, 0);
}

void game_update_tractor_pull(uint32 object)
{
    uint32 link, pulled, x, y, z, target, distance, force, value, previous, flags;
    FUNCTION_MARKER(0x800A61B8, "GAME.OVL");
    w_u32(object + 0xE8, 0);
    game_update_object_state(object);
    game_update_object_movement(object);
    link = r_u32(object + 0x208);
    if (link == 0)
        diagnostic_report(r_u32(object + 4), 0x80075D50, 0x80075D4C);
    pulled = r_u32(link + 0x38);
    if (r_u32(pulled) == 0) {
        w_u32(pulled + 0x40, 0x8009AB58);
        flags = r_u32(object + 0x114);
        w_u32(object + 0x114, flags ^ 0x20000);
        return;
    }
    if (r_u32(object + 0x88) != 0)
        return;
    x = r_u32(link);
    target = r_u32(pulled + 8);
    x -= target;
    y = r_u32(link + 4);
    target = r_u32(pulled + 12);
    y -= target;
    z = r_u32(link + 8);
    target = r_u32(pulled + 16);
    z -= target;
    distance = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    w_u32(0x80046174, distance);
    if (distance == 0)
        w_u32(0x80046174, 1);
    distance = r_u32(0x80046174);
    if ((sint32)distance >= 5001) {
        x = (uint32)((sint32)(x << 12) / (sint32)distance);
        y = (uint32)((sint32)(y << 12) / (sint32)distance);
        z = (uint32)((sint32)(z << 12) / (sint32)distance);
        force = r_u32(pulled + 0x114) & 0x10 ? 2200 : 1400;
        value = r_u32(pulled + 0x14);
        w_u32(pulled + 0x14, value + (uint32)((sint32)(x * force) >> 12));
        value = r_u32(pulled + 0x18);
        w_u32(pulled + 0x18, value + (uint32)((sint32)(y * force) >> 12));
        value = r_u32(pulled + 0x1C);
        w_u32(pulled + 0x1C, value + (uint32)((sint32)(z * force) >> 12));
    }
    game_update_object_movement(pulled);
    distance = r_u32(0x80046174);
    if ((sint32)distance < 400)
        goto reached;
    previous = r_u32(object + 0xA8);
    if ((sint32)previous < (sint32)distance && (sint32)distance < 13000) {
reached:
        if (r_u32(object + 0xAC) == 0) {
            w_u32(object + 0x40, 0x8009AB58);
            pulled = r_u32(link + 0x38);
            w_u32(pulled + 0x40, 0x8009AB58);
            flags = r_u32(object + 0x114);
            w_u32(object + 0x114, flags ^ 0x20000);
            return;
        }
        w_u32(object + 0x88, 1);
    }
    w_u32(object + 0xA8, r_u32(0x80046174));
}

void game_update_ship_point_approach(uint32 object)
{
    uint32 ticks, tx, ty, tz, x, y, z, radius = 2500, model, scale, models, descriptor;
    uint32 axis, angle_address, angle, quarter, coefficient, delta, distance, previous;
    uint32 flags, threshold, baseline, identifier, speed, half, retain_speed;
    FUNCTION_MARKER(0x800A3E38, "GAME.OVL");
    ticks = r_u32(object + 0xBC) - 1u;
    w_u32(object + 0xBC, ticks);
    if ((sint32)ticks <= 0) {
        tx = r_u32(object + 0x214);
        x = r_u32(object + 8);
        ty = r_u32(object + 0x218);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        tz = r_u32(object + 0x21C);
        game_set_relative_target_angles(object, tx - x, ty - y, tz - z);
        if (r_u32(object + 0x114) & 0x10) {
            model = r_u32(object + 0x74);
            scale = r_u32(object + 0x84);
            models = r_u32(0x80046178);
            descriptor = r_u32(models + model * 132 + 4);
            radius = r_u32(descriptor + 0x10);
            if ((sint32)scale > 0)
                radius <<= scale & 31;
            else
                radius = (uint32)((sint32)radius >> ((0u - scale) & 31));
            for (axis = 0; axis < 2; ++axis) {
                angle_address = axis == 0 ? 0x80033D1C : 0x80033D30;
                angle = r_u32(angle_address);
                if (angle == 0)
                    continue;
                quarter = (uint32)((sint32)radius / 4);
                tx = r_u32(object + 0x214);
                x = r_u32(object + 8);
                coefficient = (uint32)(sint32)r_s16(object + 0x11C + axis * 2);
                delta = (uint32)((sint32)(coefficient * quarter) >> 12);
                x = (sint32)angle < 0 ? tx - x + delta : tx - x - delta;
                coefficient = (uint32)(sint32)r_s16(object + 0x122 + axis * 2);
                ty = r_u32(object + 0x218);
                y = r_u32(object + 12);
                delta = (uint32)((sint32)(coefficient * quarter) >> 12);
                y = (sint32)angle < 0 ? ty - y + delta : ty - y - delta;
                coefficient = (uint32)(sint32)r_s16(object + 0x128 + axis * 2);
                tz = r_u32(object + 0x21C);
                z = r_u32(object + 16);
                delta = (uint32)((sint32)(coefficient * quarter) >> 12);
                z = (sint32)angle < 0 ? tz - z + delta : tz - z - delta;
                game_set_relative_target_angles(object, x, y, z);
                delta = r_u32(angle_address);
                if (((sint32)angle < 0 && (sint32)delta < 0) || ((sint32)angle > 0 && (sint32)delta > 0)) {
                    tx = r_u32(object + 0x214);
                    x = r_u32(object + 8);
                    ty = r_u32(object + 0x218);
                    y = r_u32(object + 12);
                    z = r_u32(object + 16);
                    tz = r_u32(object + 0x21C);
                    game_set_relative_target_angles(object, tx - x, ty - y, tz - z);
                } else {
                    w_u32(angle_address, 0);
                }
            }
        }
        threshold = (uint32)((sint32)radius >> 1) + 1000;
        distance = r_u32(0x80046174);
        if ((sint32)distance < (sint32)threshold)
            goto point_reached;
        previous = r_u32(object + 0xA8);
        if ((sint32)previous < (sint32)distance && (sint32)distance <= 49999) {
point_reached:
            w_u32(object + 0x40, 0x8009AB58);
            w_u32(object + 0xA8, 0xFFFFFFFF);
        } else {
            x = r_u32(0x80033D1C);
            y = r_u32(0x80033D30);
            distance = r_u32(0x80046174);
            w_u16(object + 0x254, 0);
            w_u16(object + 0x250, (uint16)x);
            w_u16(object + 0x252, (uint16)y);
            w_u32(object + 0xA8, distance);
        }
        flags = r_u32(object + 0x114);
        if (flags & 0x10) {
            distance = r_u32(0x80046174);
            if ((sint32)distance > 399999)
                w_u32(object + 0xBC, 120);
            distance = r_u32(0x80046174);
            if ((sint32)distance > 200000)
                w_u32(object + 0xBC, 8);
            else if ((sint32)distance > 100000)
                w_u32(object + 0xBC, 2);
            else if ((sint32)distance > 50000)
                w_u32(object + 0xBC, 1);
        } else {
            distance = r_u32(0x80046174);
            ticks = (sint32)distance > 65535 ? 120u : (uint32)((sint32)(distance * 119u) >> 16) + 1u;
            w_u32(object + 0xBC, ticks);
        }
    } else {
        tx = r_u32(object + 0x214);
        x = r_u32(object + 8);
        ty = r_u32(object + 0x218);
        tz = r_u32(object + 0x21C);
        x = tx - x;
        y = r_u32(object + 12);
        if ((sint32)x < 0)
            x = 0u - x;
        y = ty - y;
        z = r_u32(object + 16);
        if ((sint32)y < 0)
            y = 0u - y;
        z = tz - z;
        if ((sint32)z < 0)
            z = 0u - z;
        distance = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
        w_u32(0x80046174, distance);
        w_u32(object + 0xA8, distance);
    }
    game_update_object_state(object);
    distance = r_u32(0x80046174);
    if ((sint32)distance > 32768) {
        baseline = r_u32(object + 0xC0) != 0 ? 0x800536A0 : 0x8005369C;
        identifier = r_u32(object + 4);
        speed = r_u32(baseline + identifier * 60);
    } else {
        retain_speed = r_u32(object + 0xB8);
        baseline = r_u32(object + 0xC0) != 0 ? 0x800536A0 : 0x8005369C;
        identifier = r_u32(object + 4);
        speed = r_u32(baseline + identifier * 60);
        if (retain_speed != 0) {
            half = (uint32)((sint32)speed >> 1);
            speed = half + (uint32)((sint32)(distance * (speed - half)) >> 15);
        } else {
            speed = (uint32)((sint32)(distance * speed) >> 15);
        }
    }
    w_u32(object + 0xE8, speed);
    game_update_object_movement(object);
    game_tick_ship_task_delay(object);
}

void game_update_tractor_bay_retrieval(uint32 object)
{
    uint32 link, bay, pulled, state, flags, axis, delta[3], value, distance, numerator;
    uint32 yaw, pitch, callback, identifier, counter, dot;
    VECTOR transformed, direction;
    FUNCTION_MARKER(0x800A6478, "GAME.OVL");
    link = r_u32(object + 0x208);
    bay = r_u32(object + 0xA8);
    pulled = r_u32(link + 0x38);
    w_u32(link + 0x34, 1000);
    if (pulled == 0 || r_u32(pulled) == 0) {
        w_u32(object + 0x40, 0x8009AB58);
        flags = r_u32(object + 0x114);
        w_u32(object + 0x114, flags ^ 0x20000);
        w_u16(bay, 0);
        w_u32(link + 0x38, 0);
        return;
    }
    state = r_u32(object + 0x88);
    switch (state) {
    case 1:
        flags = r_u32(pulled + 0x114);
        w_u32(pulled + 0x114, flags | 0x2000);
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), (VECTOR *)psx_addr(bay + 0x64, 12), &transformed);
        value = r_u32(object + 8);
        delta[0] = (uint32)transformed.vx + value;
        value = r_u32(pulled + 8);
        delta[0] -= value;
        value = r_u32(object + 12);
        delta[1] = (uint32)transformed.vy + value;
        value = r_u32(pulled + 12);
        delta[1] -= value;
        value = r_u32(object + 16);
        delta[2] = (uint32)transformed.vz + value;
        value = r_u32(pulled + 16);
        delta[2] -= value;
        distance = (uint32)vector_length_approx((sint32)delta[0], (sint32)delta[1], (sint32)delta[2]);
        w_u32(0x80046174, distance);
        if (distance == 0)
            w_u32(0x80046174, 1);
        distance = r_u32(0x80046174);
        if ((sint32)distance < 5001) {
            w_u32(object + 0x88, 11);
            return;
        }
        for (axis = 0; axis < 3; ++axis)
            delta[axis] = (uint32)((sint32)(delta[axis] << 12) / (sint32)distance);
        for (axis = 0; axis < 3; ++axis) {
            value = r_u32(pulled + 0x14 + axis * 4);
            w_u32(pulled + 0x14 + axis * 4, value + (uint32)((sint32)(delta[axis] * 1400u) >> 12));
        }
        game_update_object_state(pulled);
        game_update_object_movement(pulled);
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), (VECTOR *)psx_addr(bay + 0x24, 12), &transformed);
        transformed.vx = (sint32)((uint32)transformed.vx + r_u32(object + 8));
        transformed.vy = (sint32)((uint32)transformed.vy + r_u32(object + 12));
        transformed.vz = (sint32)((uint32)transformed.vz + r_u32(object + 16));
        value = r_u32(pulled + 8);
        delta[0] = value - (uint32)transformed.vx;
        value = r_u32(pulled + 12);
        delta[1] = value - (uint32)transformed.vy;
        value = r_u32(pulled + 16);
        delta[2] = value - (uint32)transformed.vz;
        distance = (uint32)vector_length_approx((sint32)delta[0], (sint32)delta[1], (sint32)delta[2]);
        for (axis = 0; axis < 3; ++axis) {
            numerator = delta[axis] << 12;
            if (distance == 0 || (numerator == 0x80000000 && distance == 0xFFFFFFFF))
                abort();
            delta[axis] = (uint32)((sint32)numerator / (sint32)distance);
        }
        w_u32(0x80046174, distance);
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), (VECTOR *)psx_addr(bay + 0x54, 12), &direction);
        VectorNormal(&direction, &direction);
        dot = (uint32)((sint32)(delta[0] * (uint32)direction.vx) >> 12);
        dot += (uint32)((sint32)(delta[1] * (uint32)direction.vy) >> 12);
        dot += (uint32)((sint32)(delta[2] * (uint32)direction.vz) >> 12);
        w_u32(0x80046174, dot);
        if ((sint32)dot < 0)
            dot = 0u - dot;
        if ((sint32)dot >= 3801)
            w_u32(object + 0x88, 11);
        return;
    case 11:
        game_steer_to_target_offset(pulled, object, bay + 0x34, 0);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u32(pulled + 0xE8, 0);
        w_u16(pulled + 0x250, (uint16)yaw);
        w_u16(pulled + 0x252, (uint16)pitch);
        game_update_object_state(pulled);
        game_update_object_movement(pulled);
        value = (uint32)(sint32)r_s16(pulled + 0x250);
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)value >= 2)
            return;
        value = (uint32)(sint32)r_s16(pulled + 0x252);
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)value < 2)
            w_u32(object + 0x88, 2);
        return;
    case 2:
        game_steer_to_target_offset(pulled, object, bay + 0x34, 0);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u32(pulled + 0xE8, 0x8200);
        w_u32(pulled + 0x20, yaw);
        w_u32(pulled + 0x24, pitch);
        game_update_object_state(pulled);
        game_update_object_movement(pulled);
        if (r_s32(0x80046174) < 20000) {
            w_u32(object + 0x88, 3);
            w_u16(bay + 0x78, 1);
            w_u16(bay + 0x76, 0);
            w_u16(bay + 0x7A, 10);
            w_u16(bay + 0x74, 0);
        }
        return;
    case 3:
        game_steer_to_target_offset(pulled, object, bay + 0x34, 0);
        w_u32(pulled + 0xE8, 0x8200);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u32(pulled + 0xA0, 0);
        w_u32(pulled + 0x44, 0x8002493C);
        w_u32(pulled + 0x20, yaw);
        w_u32(pulled + 0x24, pitch);
        game_update_object_state(pulled);
        game_update_object_movement(pulled);
        game_draw_carrier_launch_bay(object, bay);
        if (r_s32(0x80046174) < 2000) {
            w_u32(object + 0x88, 4);
            w_u32(pulled + 0x3C, 0x8002493C);
            w_u16(bay + 0x76, 10);
            w_u16(bay + 0x78, 0xFFFF);
            w_u16(bay + 0x7A, 0);
        }
        return;
    case 4:
        game_draw_carrier_launch_bay(object, bay);
        value = (uint32)(sint32)r_s16(bay + 0x76);
        if (value != (uint32)(sint32)r_s16(bay + 0x7A))
            return;
        counter = (uint32)(sint32)r_s16(0x80059614);
        identifier = r_u32(pulled + 4);
        if (identifier == counter) {
            value = r_u8(object + 0x26F);
            w_u8(object + 0x26F, (uint8)(value + 1));
        }
        callback = r_u32(pulled + 0x48);
        psx_dispatch_guest_call(callback, 1, pulled, 0, 0);
        w_u16(bay, 0);
        w_u32(object + 0x40, 0x8009AB58);
        flags = r_u32(object + 0x114);
        w_u32(object + 0x114, flags ^ 0x20000);
        w_u32(link + 0x38, 0);
        return;
    default:
        return;
    }
}

static sint32 game_allocate_warp_effect_body(uint32 position, const uint32 *native_position, uint32 matrix, uint32 object)
{
    uint32 slot, offset, axis, column, row, element, value, last;
    slot = object == r_u32(0x80040FDC) ? 0 : (object == r_u32(0x8003FE74) ? 0 : 1);
    for (; slot < 2; ++slot) {
        offset = slot * 0x5C48;
        if (r_u32(0x8010ACD0 + offset) != 0)
            continue;
        for (axis = 0; axis < 3; ++axis) {
            value = native_position ? native_position[axis] : r_u32(position + axis * 4);
            w_u32(0x8010AD08 + offset + axis * 4, value);
        }
        for (column = 0; column < 3; ++column) {
            for (row = 0; row < 3; ++row) {
                element = column * 2 + row * 6;
                if (element == 16)
                    continue;
                value = r_u16(matrix + element);
                w_u16(0x8010ACE8 + offset + element, (uint16)value);
            }
        }
        last = r_u16(matrix + 16);
        w_u32(0x8010ACD0 + offset, 2);
        w_u32(0x8010ACE4 + offset, object);
        w_u32(0x8010ACE0 + offset, 0);
        w_u32(0x8010ACDC + offset, 0);
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

sint32 game_allocate_warp_effect(uint32 position, uint32 matrix, uint32 object)
{
    FUNCTION_MARKER(0x80090160, "GAME.OVL");
    return game_allocate_warp_effect_body(position, NULL, matrix, object);
}

sint32 game_allocate_warp_effect_native(const uint32 position[3], uint32 matrix, uint32 object)
{
    return game_allocate_warp_effect_body(0, position, matrix, object);
}

void game_update_ship_warp_departure(uint32 object)
{
    uint32 state, ticks, identifier, speed, axis, coefficient, position[3], flags;
    uint32 index, offset, x, y, z, px, py, pz, distance, previous, view, callback;
    sint32 slot;
    FUNCTION_MARKER(0x800A7864, "GAME.OVL");
    state = r_u32(object + 0x88);
    switch (state) {
    case 0:
        ticks = r_u32(object + 0x9C) - 1u;
        w_u32(object + 0x9C, ticks);
        if ((sint32)ticks <= 0) {
            w_u32(object + 0x88, 1);
        } else {
            identifier = r_u32(object + 4);
            w_u16(object + 0x250, 0);
            w_u16(object + 0x252, 0);
            w_u16(object + 0x254, 0);
            speed = r_u32(0x8005369C + identifier * 60);
            w_u32(object + 0xE8, speed << 1);
        }
        break;
    case 1:
        for (axis = 0; axis < 3; ++axis) {
            coefficient = (uint32)(sint32)r_s16(object + 0x120 + axis * 6);
            position[axis] = r_u32(object + 8 + axis * 4) + coefficient * 10u;
        }
        slot = game_allocate_warp_effect_native(position, object + 0x11C, object);
        if (slot < 0)
            break;
        w_u32(object + 0xA8, (uint32)slot);
        flags = r_u32(object + 0x114);
        w_u32(object + 0x88, 2);
        if (flags & 0x10) {
            index = r_u32(object + 0xA8);
            w_u32(0x8010ACE0 + index * 0x5C48, 2);
        }
        break;
    case 2:
    case 3:
        index = r_u32(object + 0xA8);
        if (state == 2) {
            w_u32(0x8010ACDC + index * 0x5C48, 1);
        } else {
            w_u32(0x8010ACD0 + index * 0x5C48, 6);
            index = r_u32(object + 0xA8);
            w_u32(0x8010ACD4 + index * 0x5C48, 255);
        }
        index = r_u32(object + 0xA8);
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        offset = index * 0x5C48;
        px = r_u32(0x8010AD08 + offset);
        z = r_u32(object + 16);
        py = r_u32(0x8010AD0C + offset);
        pz = r_u32(0x8010AD10 + offset);
        distance = (uint32)vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
        if (state == 2) {
            previous = r_u32(object + 0xAC);
            w_u32(0x80046174, distance);
            if ((sint32)previous < (sint32)distance)
                w_u32(object + 0x88, 3);
            else
                w_u32(object + 0xAC, distance);
        } else {
            w_u32(0x80046174, distance);
            if ((sint32)distance < 20001)
                break;
            w_u32(object + 0x88, 4);
            view = r_u32(0x8003FE74);
            w_u32(object + 0x44, 0x8002493C);
            w_u32(object + 0xA0, 0);
            if (object != view) {
                w_u32(object + 0x3C, 0x8002493C);
                w_u32(object + 0x114, 0x2000);
                w_u32(object + 0x118, 0);
            } else {
                w_u32(0x800EC868, 1);
            }
            index = r_u32(object + 0xA8);
            w_u32(0x8010ACD4 + index * 0x5C48, 0);
        }
        break;
    case 4:
        index = r_u32(object + 0xA8);
        if (r_u32(0x8010ACD0 + index * 0x5C48) != 0)
            break;
        view = r_u32(0x8003FE74);
        if (object != view) {
            callback = r_u32(object + 0x48);
            psx_dispatch_guest_call(callback, 1, object, 0, 0);
        }
        return;
    default:
        break;
    }
    game_update_object_state(object);
    game_update_object_movement(object);
}
