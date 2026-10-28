#include "game_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void game_queue_hud_message(uint32 text)
{
    uint32 length, index, destination, value;
    sint32 count;
    FUNCTION_MARKER(0x80087498, "GAME.OVL");
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    if ((sint32)length >= 121)
        diagnostic_report(0u, 0x80074660u, 0x8007465Cu);
    index = (uint32)(sint32)r_s16(0x800F3C6Au);
    destination = 0x801268B0u + index * 122u;
    strcpy((char *)psx_addr(destination, (size_t)length + 1u),
        (const char *)psx_addr(text, (size_t)length + 1u));
    index = (uint32)(sint32)r_s16(0x800F3C6Au);
    w_u16(0x80126928u + index * 122u, 90u);
    count = r_s16(0x800F3C68u);
    if (count < 4)
        w_u16(0x800F3C68u, (uint16)((uint32)count + 1u));
    value = r_u16(0x800F3C6Au) + 1u;
    w_u16(0x800F3C6Au, (uint16)value);
    if ((sint16)value >= 4)
        w_u16(0x800F3C6Au, 0u);
}

void game_emit_sized_fragment_flash(uint32 position, uint32 size)
{
    uint32 effect, value, z, phase;
    FUNCTION_MARKER(0x800BBE0C, "GAME.OVL");
    effect = game_allocate_flash(0u);
    if (effect == 0u)
        return;
    value = r_u32(position);
    w_u32(effect, value);
    value = r_u32(position + 4u);
    w_u32(effect + 4u, value);
    z = r_u32(position + 8u);
    w_u32(effect + 0x20u, 0xC0C0C0u);
    phase = r_u32(0x800313F8u);
    w_u32(effect + 0xCu, 384u);
    w_u16(effect + 0x24u, (uint16)size);
    w_u16(effect + 0x26u, 0u);
    w_u16(effect + 0x28u, 16u);
    w_u16(effect + 0x2Cu, (uint16)phase);
    w_u32(effect + 8u, z);
}

void game_attach_position_light(uint32 object, uint32 position, uint32 color, uint32 intensity, uint32 parameter)
{
    VECTOR relative, transformed;
    uint32 count, head, record, value, component;
    FUNCTION_MARKER(0x8008CF24, "GAME.OVL");
    count = r_u32(0x800ED390u);
    if ((sint32)count >= 64) {
#ifndef NDEBUG
        printf("run out of lights!\n");
#endif
        return;
    }
    head = r_u32(object + 0x118u);
    if (head == 0u) {
        record = 0x80124020u + count * 36u;
        w_u32(object + 0x118u, record);
    } else {
        while (r_u32(head + 32u) != 0u)
            head = r_u32(head + 32u);
        count = r_u32(0x800ED390u);
        record = 0x80124020u + count * 36u;
        w_u32(head + 32u, record);
    }
    value = r_u32(position);
    component = r_u32(object + 8u);
    relative.vx = (sint32)(value - component);
    value = r_u32(position + 4u);
    component = r_u32(object + 12u);
    relative.vy = (sint32)(value - component);
    value = r_u32(position + 8u);
    component = r_u32(object + 16u);
    count = r_u32(0x800ED390u);
    relative.vz = (sint32)(value - component);
    record = 0x80124020u + count * 36u;
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &relative, &transformed);
    w_u32(record, (uint32)transformed.vx);
    w_u32(record + 4u, (uint32)transformed.vy);
    w_u32(record + 8u, (uint32)transformed.vz);
    count = r_u32(0x800ED390u);
    record = 0x80124020u + count * 36u;
    ++count;
    w_u32(record + 16u, object);
    w_u32(record + 20u, parameter);
    w_u32(record + 24u, intensity);
    w_u32(record + 28u, color);
    w_u32(record + 32u, 0u);
    w_u32(0x800ED390u, count);
}

void game_update_reprogrammed_target(uint32 object)
{
    FUNCTION_MARKER(0x800815AC, "GAME.OVL");
    w_u32(object + 0xE8u, 0u);
    game_update_object_state(object);
    game_update_object_movement(object);
}

sint32 game_limit_signed_step(sint32 value, sint32 limit)
{
    uint32 magnitude;
    FUNCTION_MARKER(0x80081CD8, "GAME.OVL");
    magnitude = value < 0 ? 0u - (uint32)value : (uint32)value;
    if (limit < (sint32)magnitude)
        return value > 0 ? limit : (sint32)(0u - (uint32)limit);
    return value;
}

void game_update_object_state(uint32 object)
{
    uint32 step, current, desired, value, axis, field, fraction, remaining, product_x, product_y, product_z, component;
    sint32 delta, ticks, movement;
    FUNCTION_MARKER(0x80081B14, "GAME.OVL");
    step = (uint32)(sint32)r_s16(object + 0x260u) << 3;
    current = r_u32(object + 0xECu);
    desired = r_u32(object + 0xE8u);
    if ((sint32)current < (sint32)desired) {
        value = current + r_u32(object + 0xF0u);
        w_u32(object + 0xECu, value);
        if ((sint32)desired < (sint32)value)
            w_u32(object + 0xECu, desired);
    } else if ((sint32)desired < (sint32)current) {
        value = r_u32(object + 0xF0u);
        desired = r_u32(object + 0xE8u);
        value = current - value;
        w_u32(object + 0xECu, value);
        if ((sint32)value < (sint32)desired)
            w_u32(object + 0xECu, desired);
    }
    for (axis = 0u; axis < 3u; ++axis) {
        field = object + 0x250u + axis * 2u;
        delta = r_s16(field);
        if (delta != 0) {
            fraction = r_u16(object + 0x256u + axis * 2u);
            w_u16(object + 0x1F8u + axis * 2u, (uint16)delta);
            fraction += step;
            ticks = (sint32)(sint16)fraction >> 8;
            w_u16(object + 0x256u + axis * 2u, (uint16)(fraction & 255u));
            if (ticks != 0) {
                movement = game_limit_signed_step(r_s16(field), ticks);
                remaining = r_u16(field);
                w_u32(object + 0x20u + axis * 4u, (uint32)movement);
                w_u16(field, (uint16)(remaining - (uint32)movement));
            }
        }
    }
    value = r_u32(object + 0xECu);
    component = (uint32)(sint32)r_s16(object + 0x120u);
    product_x = component * (uint32)((sint32)value >> 8);
    value = r_u32(object + 0xECu);
    component = (uint32)(sint32)r_s16(object + 0x126u);
    product_y = component * (uint32)((sint32)value >> 8);
    value = r_u32(object + 0xECu);
    component = (uint32)(sint32)r_s16(object + 0x12Cu);
    product_z = component * (uint32)((sint32)value >> 8);
    w_u32(object + 0xFCu, (uint32)((sint32)product_x >> 12));
    w_u32(object + 0x100u, (uint32)((sint32)product_y >> 12));
    w_u32(object + 0x104u, (uint32)((sint32)product_z >> 12));
}

MATRIX *game_multiply_transposed_right_native(MATRIX *left, const MATRIX *right, MATRIX *output)
{
    MATRIX transposed;
    FUNCTION_MARKER(0x80081614, "GAME.OVL");
    transposed.m[0][0] = right->m[0][0];
    transposed.m[1][0] = right->m[0][1];
    transposed.m[2][0] = right->m[0][2];
    transposed.m[0][1] = right->m[1][0];
    transposed.m[1][1] = right->m[1][1];
    transposed.m[2][1] = right->m[1][2];
    transposed.m[0][2] = right->m[2][0];
    transposed.m[1][2] = right->m[2][1];
    transposed.m[2][2] = right->m[2][2];
    return MulMatrix0(left, &transposed, output);
}

uint32 game_update_object_movement(uint32 object)
{
    SVECTOR rotation;
    MATRIX delta;
    uint32 value, x, y, z, vx, vy, vz, px, py, pz, player, spin, region;
    sint32 speed;
    int damp = 1;
    FUNCTION_MARKER(0x8008169C, "GAME.OVL");
    if ((r_u32(object + 0x114u) & 0x4000000u) != 0u)
        return 0u;
    x = r_u32(object + 0xFCu) * 12u;
    vx = r_u32(object + 0x14u);
    y = r_u32(object + 0x100u) * 12u;
    w_u32(object + 0x14u, vx + x);
    vy = r_u32(object + 0x18u);
    z = r_u32(object + 0x104u) * 12u;
    w_u32(object + 0x18u, vy + y);
    vz = r_u32(object + 0x1Cu);
    vx = r_u32(object + 0x14u);
    w_u32(object + 0x1Cu, vz + z);
    px = r_u32(object + 8u);
    vy = r_u32(object + 0x18u);
    w_u32(object + 8u, px + (uint32)((sint32)vx >> 8));
    py = r_u32(object + 12u);
    vz = r_u32(object + 0x1Cu);
    w_u32(object + 12u, py + (uint32)((sint32)vy >> 8));
    pz = r_u32(object + 16u);
    player = r_u32(0x80040FDCu);
    w_u32(object + 16u, pz + (uint32)((sint32)vz >> 8));
    if (object == player) {
        vx = r_u32(object + 0x14u);
        vy = r_u32(object + 0x18u);
        vz = r_u32(object + 0x1Cu);
        speed = vector_length_approx((sint32)vx >> 8, (sint32)vy >> 8, (sint32)vz >> 8);
        if (speed < 51)
            damp = 0;
    }
    if (damp) {
        vx = r_u32(object + 0x14u);
        vy = r_u32(object + 0x18u);
        w_u32(object + 0x14u, vx - (uint32)((sint32)vx >> 5));
        vz = r_u32(object + 0x1Cu);
        w_u32(object + 0x18u, vy - (uint32)((sint32)vy >> 5));
        w_u32(object + 0x1Cu, vz - (uint32)((sint32)vz >> 5));
    }
    spin = r_u32(object + 0x15Cu);
    if (spin == 0u)
        return 0u;
    region = r_u32(0x800311C0u);
    rotation.vx = (sint16)((sint32)spin >> 8);
    rotation.vy = 0;
    rotation.vz = 0;
    w_u32(0x80053688u, 128u);
    w_u32(0x8005368Cu, 64u);
    if (region != 0u)
        w_u32(0x8005368Cu, 68u);
    RotMatrix(&rotation, &delta);
    MulMatrix0((MATRIX *)psx_addr(object + 0x13Cu, 20u), &delta, &delta);
    game_multiply_transposed_right_native(&delta, (const MATRIX *)psx_addr(object + 0x13Cu, 18u), &delta);
    player = r_u32(0x80040FDCu);
    if (object != player) {
        if ((r_u32(object + 0x114u) & 0x80000u) == 0u) {
            value = r_u32(object + 0x15Cu);
            w_u32(object + 0x15Cu, value - (uint32)((sint32)value >> 5));
        }
        MulMatrix0(&delta, (MATRIX *)psx_addr(object + 0x11Cu, 20u), (MATRIX *)psx_addr(object + 0x11Cu, 20u));
    } else {
        if ((r_u32(object + 0x114u) & 0x80000u) == 0u) {
            value = r_u32(object + 0x15Cu);
            w_u32(object + 0x15Cu, value - (uint32)((sint32)value >> 4));
        }
        if (r_s32(object + 0x15Cu) < 32)
            w_u32(object + 0x15Cu, 0u);
        MulMatrix0((MATRIX *)psx_addr(object + 0x11Cu, 20u), &delta, (MATRIX *)psx_addr(object + 0x11Cu, 20u));
    }
    matrix_normalize(object + 0x11Cu, 1u);
    return 0u;
}

uint32 game_integrate_projectile(uint32 object)
{
    uint32 x, y, z, vx, vy, vz, px, py, pz;
    FUNCTION_MARKER(0x800805C0, "GAME.OVL");
    x = r_u32(object + 0xFCu) * 48u;
    vx = r_u32(object + 0x14u);
    y = r_u32(object + 0x100u) * 48u;
    w_u32(object + 0x14u, vx + x);
    vy = r_u32(object + 0x18u);
    z = r_u32(object + 0x104u) * 48u;
    w_u32(object + 0x18u, vy + y);
    vz = r_u32(object + 0x1Cu);
    vx = r_u32(object + 0x14u);
    w_u32(object + 0x1Cu, vz + z);
    vx -= (uint32)((sint32)vx >> 3);
    vy = r_u32(object + 0x18u);
    vz = r_u32(object + 0x1Cu);
    w_u32(object + 0x14u, vx);
    vy -= (uint32)((sint32)vy >> 3);
    vz -= (uint32)((sint32)vz >> 3);
    w_u32(object + 0x1Cu, vz);
    vx = r_u32(object + 0x14u);
    px = r_u32(object + 8u);
    w_u32(object + 0x18u, vy);
    px += (uint32)((sint32)vx >> 5);
    vy = r_u32(object + 0x18u);
    w_u32(object + 8u, px);
    py = r_u32(object + 12u);
    py += (uint32)((sint32)vy >> 5);
    vz = r_u32(object + 0x1Cu);
    w_u32(object + 12u, py);
    pz = r_u32(object + 16u);
    w_u32(object + 16u, pz + (uint32)((sint32)vz >> 5));
    return 0u;
}

void game_update_weapon_projectile(uint32 projectile)
{
    uint32 target, player, x, y, z, value, callback, identifier;
    FUNCTION_MARKER(0x8008068C, "GAME.OVL");
    target = r_u32(projectile + 0x178u);
    player = r_u32(0x80040FDCu);
    if (target == player) {
        value = r_u32(0x800311B8u);
        w_u32(0x800ECE68u, 1u);
        if ((value & 1u) != 0u)
            w_u32(projectile + 0x9Cu, r_u32(projectile + 0x9Cu) - 1u);
        x = r_u32(projectile + 0xFCu);
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 2));
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0x104u, (uint32)((sint32)z >> 2));
        w_u32(projectile + 0x100u, (uint32)((sint32)y >> 2));
        game_integrate_projectile(projectile);
        x = r_u32(projectile + 0xFCu);
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0xFCu, x << 2);
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0x104u, z << 2);
        w_u32(projectile + 0x100u, y << 2);
    } else {
        w_u32(projectile + 0x9Cu, r_u32(projectile + 0x9Cu) - 1u);
        game_integrate_projectile(projectile);
    }
    if (r_s32(projectile + 0x9Cu) > 0)
        return;
    if (r_u32(projectile) == 211u) {
        callback = r_u32(projectile + 0x48u);
        psx_dispatch_guest_call(callback, 1u, projectile, 0u, 0u);
        return;
    }
    callback = 0x80081294u;
    if (target != 0u) {
        identifier = r_u16(target + 0x24Au);
        if (identifier == r_u16(projectile + 0x17Cu))
            callback = 0x80080B8Cu;
    }
    w_u32(projectile + 0x40u, callback);
    w_u32(projectile + 0x9Cu, 120u);
}

void game_update_profile_0c_projectile(uint32 projectile)
{
    uint32 lifetime;
    FUNCTION_MARKER(0x800807D8, "GAME.OVL");
    game_integrate_projectile(projectile);
    lifetime = r_u32(projectile + 0x9Cu) - 1u;
    w_u32(projectile + 0x9Cu, lifetime);
    if ((sint32)lifetime <= 0) {
        w_u32(projectile + 0x40u, 0x80080828u);
        w_u32(projectile + 0x9Cu, 16u);
    }
}

void game_update_expiring_projectile(uint32 projectile)
{
    uint32 lifetime, callback;
    FUNCTION_MARKER(0x80081294, "GAME.OVL");
    game_integrate_projectile(projectile);
    lifetime = r_u32(projectile + 0x9Cu) - 1u;
    w_u32(projectile + 0x9Cu, lifetime);
    if ((sint32)lifetime < 0) {
        callback = r_u32(projectile + 0x48u);
        psx_dispatch_guest_call(callback, 1u, projectile, 0u, 0u);
    }
}

void game_update_auxiliary_projectile(uint32 projectile)
{
    uint32 lifetime, callback;
    FUNCTION_MARKER(0x800812E0, "GAME.OVL");
    game_integrate_projectile(projectile);
    lifetime = r_u32(projectile + 0x9Cu) - 1u;
    w_u32(projectile + 0x9Cu, lifetime);
    if ((sint32)lifetime < 0) {
        callback = r_u32(projectile + 0x48u);
        psx_dispatch_guest_call(callback, 1u, projectile, 0u, 0u);
    }
}

void game_split_projectile(uint32 projectile)
{
    SVECTOR offset;
    uint32 coordinate, index, effect, state, even_x, even_y, even_z, owner, identifier;
    uint32 x, y, z, squared, value, product, px, py, pz, scale, callback;
    sint32 length;
    FUNCTION_MARKER(0x80080828, "GAME.OVL");
    memcpy(&offset, psx_addr(0x8007460Cu, 8u), 8u);
    coordinate = r_u32(projectile + 8u);
    game_emit_wide_particles(32u, coordinate, coordinate, coordinate, 0u, 0u, 0u);
    coordinate = r_u32(projectile + 8u);
    game_emit_narrow_particles(32u, coordinate, coordinate, coordinate, 0u, 0u, 0u);
    for (index = 0u; index < 32u; ++index) {
        effect = game_create_profile_effect_native(projectile, projectile, &offset, 0x800EF1B8u);
        if (effect == 0u)
            continue;
        state = r_u32(0x800EF50Cu);
        w_u32(effect + 0x14u, 0x800EF2F4u);
        state = (uint32)((sint32)state >> 1) ^ (state << 3);
        x = state & 32767u;
        even_x = (uint32)((sint32)state >> 1) ^ (state << 3);
        x -= even_x & 32767u;
        squared = x * x;
        state = (uint32)((sint32)even_x >> 1) ^ (even_x << 3);
        y = state & 32767u;
        even_y = (uint32)((sint32)state >> 1) ^ (state << 3);
        y -= even_y & 32767u;
        squared += y * y;
        w_u32(effect + 0x1Cu, 3u);
        w_u32(effect + 0x2Cu, 16u);
        state = (uint32)((sint32)even_y >> 1) ^ (even_y << 3);
        z = state & 32767u;
        even_z = (uint32)((sint32)state >> 1) ^ (state << 3);
        z -= even_z & 32767u;
        owner = r_u32(projectile + 0x160u);
        squared += z * z;
        w_u32(effect + 0x58u, owner);
        identifier = r_u16(projectile + 0x164u);
        w_u32(0x800EF50Cu, even_x);
        w_u32(0x800EF50Cu, even_y);
        w_u32(0x800EF50Cu, even_z);
        w_u16(effect + 0x5Cu, (uint16)identifier);
        length = (sint32)SquareRoot0((sint32)squared);
        if (length == 0)
            length = 1;
        w_u32(effect + 0x20u, (uint32)((sint32)(x * 2000u) / length));
        w_u32(effect + 0x24u, (uint32)((sint32)(y * 2000u) / length));
        w_u32(effect + 0x28u, (uint32)((sint32)(z * 2000u) / length));
        x = r_u32(effect + 0x20u);
        scale = r_u32(0x800EF14Cu);
        product = x * scale;
        y = r_u32(effect + 0x24u);
        w_u16(effect + 0x36u, (uint16)product);
        scale = r_u32(0x800EF14Cu);
        product = y * scale;
        value = r_u32(effect);
        x = r_u32(effect + 0x20u);
        z = r_u32(effect + 0x28u);
        px = value + x;
        pz = r_u32(effect + 8u);
        w_u16(effect + 0x38u, (uint16)product);
        scale = r_u32(0x800EF14Cu);
        w_u32(effect, px);
        product = z * scale;
        py = r_u32(effect + 4u);
        y = r_u32(effect + 0x24u);
        z = r_u32(effect + 0x28u);
        w_u32(effect + 4u, py + y);
        w_u32(effect + 0x68u, 0x800EED9Cu);
        w_u32(effect + 8u, pz + z);
        w_u16(effect + 0x3Cu, 214u);
        w_u16(effect + 0x3Au, (uint16)product);
    }
    callback = r_u32(projectile + 0x48u);
    psx_dispatch_guest_call(callback, 1u, projectile, 0u, 0u);
}

void game_update_homing_projectile(uint32 projectile)
{
    uint32 lifetime, callback, target, identifier, x, y, z, value, kind;
    VECTOR saved, direction, side, vertical;
    FUNCTION_MARKER(0x80080B8C, "GAME.OVL");
    lifetime = r_u32(projectile + 0x9Cu) - 1u;
    w_u32(projectile + 0x9Cu, lifetime);
    if ((sint32)lifetime <= 0) {
        callback = r_u32(projectile + 0x48u);
        psx_dispatch_guest_call(callback, 1u, projectile, 0u, 0u);
        return;
    }
    target = r_u32(projectile + 0x178u);
    if (target == r_u32(0x80040FDCu))
        w_u32(0x800ECE68u, 1u);
    identifier = r_u16(target + 0x24Au);
    if (identifier != r_u16(projectile + 0x17Cu) || target == 0u) {
        w_u32(projectile + 0x40u, 0x80081294u);
        game_update_expiring_projectile(projectile);
        return;
    }
    saved.vx = r_s32(projectile + 0xFCu);
    saved.vy = r_s32(projectile + 0x100u);
    saved.vz = r_s32(projectile + 0x104u);
    x = r_u32(target + 8u);
    value = r_u32(projectile + 8u);
    w_u32(projectile + 0xFCu, x - value);
    y = r_u32(target + 12u);
    value = r_u32(projectile + 12u);
    w_u32(projectile + 0x100u, y - value);
    z = r_u32(target + 16u);
    value = r_u32(projectile + 16u);
    x = r_u32(projectile + 0xFCu);
    w_u32(projectile + 0x104u, z - value);
    direction.vx = (sint32)x;
    direction.vy = r_s32(projectile + 0x100u);
    direction.vz = r_s32(projectile + 0x104u);
    vector_normalize((VECTOR *)psx_addr(projectile + 0xFCu, 12u),
        (VECTOR *)psx_addr(projectile + 0xFCu, 12u));
    kind = r_u32(projectile);
    if (kind == 210u) {
        x = r_u32(projectile + 0xFCu);
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 5));
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0x104u, (uint32)((sint32)z >> 5));
        w_u32(projectile + 0x100u, (uint32)((sint32)y >> 5));
        vector_normalize(&saved, &saved);
        vector_normalize(&direction, &direction);
        value = (uint32)saved.vx - (uint32)direction.vx;
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)value >= 129) {
            value = (uint32)saved.vy - (uint32)direction.vy;
            if ((sint32)value < 0)
                value = 0u - value;
            if ((sint32)value >= 129) {
                value = (uint32)saved.vz - (uint32)direction.vz;
                if ((sint32)value < 0)
                    value = 0u - value;
                if ((sint32)value >= 129) {
                    OuterProduct12(&direction, &saved, &side);
                    vector_normalize(&side, &side);
                    OuterProduct12(&direction, &side, &vertical);
                    vector_normalize(&vertical, &vertical);
                    w_u16(projectile + 0x11Cu, (uint16)side.vx);
                    w_u16(projectile + 0x122u, (uint16)side.vy);
                    w_u16(projectile + 0x128u, (uint16)side.vz);
                    w_u16(projectile + 0x11Eu, (uint16)vertical.vx);
                    w_u16(projectile + 0x124u, (uint16)vertical.vy);
                    w_u16(projectile + 0x12Au, (uint16)vertical.vz);
                    w_u16(projectile + 0x120u, (uint16)direction.vx);
                    w_u16(projectile + 0x126u, (uint16)direction.vy);
                    w_u16(projectile + 0x12Cu, (uint16)direction.vz);
                }
            }
        }
    } else {
        x = r_u32(projectile + 0xFCu);
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 3));
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0x100u, (uint32)((sint32)y >> 3));
        kind = r_u32(projectile);
        w_u32(projectile + 0x104u, (uint32)((sint32)z >> 3));
        if (kind - 205u < 2u) {
            x = r_u32(projectile + 0xFCu);
            z = r_u32(projectile + 0x104u);
            w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 3));
            y = r_u32(projectile + 0x100u);
            w_u32(projectile + 0x104u, (uint32)((sint32)z >> 3));
            w_u32(projectile + 0x100u, (uint32)((sint32)y >> 3));
        }
    }
    if (target == r_u32(0x80040FDCu)) {
        x = r_u32(projectile + 0xFCu);
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 2));
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0x104u, (uint32)((sint32)z >> 2));
        w_u32(projectile + 0x100u, (uint32)((sint32)y >> 2));
        game_integrate_projectile(projectile);
        x = r_u32(projectile + 0xFCu);
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0xFCu, x << 2);
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0x104u, z << 2);
        w_u32(projectile + 0x100u, y << 2);
    } else {
        game_integrate_projectile(projectile);
    }
}

void game_update_speed_gated_projectile(uint32 projectile)
{
    uint32 lifetime, callback, target, identifier, x, y, z, value, kind;
    VECTOR saved, direction, side, vertical;
    FUNCTION_MARKER(0x80080EF4, "GAME.OVL");
    lifetime = r_u32(projectile + 0x9Cu) - 1u;
    w_u32(projectile + 0x9Cu, lifetime);
    if ((sint32)lifetime <= 0) {
        callback = r_u32(projectile + 0x48u);
        psx_dispatch_guest_call(callback, 1u, projectile, 0u, 0u);
        return;
    }
    target = r_u32(projectile + 0x178u);
    x = r_u32(target + 0x14u);
    y = r_u32(target + 0x18u);
    z = r_u32(target + 0x1Cu);
    if (vector_length_approx((sint32)x >> 8, (sint32)y >> 8, (sint32)z >> 8) < 256)
        w_u32(projectile + 0x40u, 0x80081294u);
    if (target == r_u32(0x80040FDCu))
        w_u32(0x800ECE68u, 1u);
    identifier = r_u16(target + 0x24Au);
    if (identifier != r_u16(projectile + 0x17Cu) || target == 0u) {
        w_u32(projectile + 0x40u, 0x80081294u);
        game_update_expiring_projectile(projectile);
        return;
    }
    saved.vx = r_s32(projectile + 0xFCu);
    saved.vy = r_s32(projectile + 0x100u);
    saved.vz = r_s32(projectile + 0x104u);
    x = r_u32(target + 8u);
    value = r_u32(projectile + 8u);
    w_u32(projectile + 0xFCu, x - value);
    y = r_u32(target + 12u);
    value = r_u32(projectile + 12u);
    w_u32(projectile + 0x100u, y - value);
    z = r_u32(target + 16u);
    value = r_u32(projectile + 16u);
    x = r_u32(projectile + 0xFCu);
    w_u32(projectile + 0x104u, z - value);
    direction.vx = (sint32)x;
    direction.vy = r_s32(projectile + 0x100u);
    direction.vz = r_s32(projectile + 0x104u);
    vector_normalize((VECTOR *)psx_addr(projectile + 0xFCu, 12u),
        (VECTOR *)psx_addr(projectile + 0xFCu, 12u));
    kind = r_u32(projectile);
    if (kind == 210u) {
        x = r_u32(projectile + 0xFCu);
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 5));
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0x104u, (uint32)((sint32)z >> 5));
        w_u32(projectile + 0x100u, (uint32)((sint32)y >> 5));
        vector_normalize(&saved, &saved);
        vector_normalize(&direction, &direction);
        value = (uint32)saved.vx - (uint32)direction.vx;
        if ((sint32)value < 0)
            value = 0u - value;
        if ((sint32)value >= 129) {
            value = (uint32)saved.vy - (uint32)direction.vy;
            if ((sint32)value < 0)
                value = 0u - value;
            if ((sint32)value >= 129) {
                value = (uint32)saved.vz - (uint32)direction.vz;
                if ((sint32)value < 0)
                    value = 0u - value;
                if ((sint32)value >= 129) {
                    OuterProduct12(&direction, &saved, &side);
                    vector_normalize(&side, &side);
                    OuterProduct12(&direction, &side, &vertical);
                    vector_normalize(&vertical, &vertical);
                    w_u16(projectile + 0x11Cu, (uint16)side.vx);
                    w_u16(projectile + 0x122u, (uint16)side.vy);
                    w_u16(projectile + 0x128u, (uint16)side.vz);
                    w_u16(projectile + 0x11Eu, (uint16)vertical.vx);
                    w_u16(projectile + 0x124u, (uint16)vertical.vy);
                    w_u16(projectile + 0x12Au, (uint16)vertical.vz);
                    w_u16(projectile + 0x120u, (uint16)direction.vx);
                    w_u16(projectile + 0x126u, (uint16)direction.vy);
                    w_u16(projectile + 0x12Cu, (uint16)direction.vz);
                }
            }
        }
    } else {
        x = r_u32(projectile + 0xFCu);
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 3));
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0x100u, (uint32)((sint32)y >> 3));
        kind = r_u32(projectile);
        w_u32(projectile + 0x104u, (uint32)((sint32)z >> 3));
        if (kind - 205u < 2u) {
            x = r_u32(projectile + 0xFCu);
            z = r_u32(projectile + 0x104u);
            w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 3));
            y = r_u32(projectile + 0x100u);
            w_u32(projectile + 0x104u, (uint32)((sint32)z >> 3));
            w_u32(projectile + 0x100u, (uint32)((sint32)y >> 3));
        }
    }
    if (target == r_u32(0x80040FDCu)) {
        x = r_u32(projectile + 0xFCu);
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 2));
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0x104u, (uint32)((sint32)z >> 2));
        w_u32(projectile + 0x100u, (uint32)((sint32)y >> 2));
        game_integrate_projectile(projectile);
        x = r_u32(projectile + 0xFCu);
        z = r_u32(projectile + 0x104u);
        w_u32(projectile + 0xFCu, x << 2);
        y = r_u32(projectile + 0x100u);
        w_u32(projectile + 0x104u, z << 2);
        w_u32(projectile + 0x100u, y << 2);
    } else {
        game_integrate_projectile(projectile);
    }
}

uint32 game_remove_fragmenting_projectile(uint32 projectile)
{
    sint32 count;
    FUNCTION_MARKER(0x80081408, "GAME.OVL");
    if (r_s32(projectile + 0x9Cu) > 0) {
        count = (sint32)(sint16)random_between(6, 12);
        game_emit_projectile_fragments(projectile + 8u, projectile, count, 0u);
    }
    game_detach_object_trails(projectile);
    return game_mark_object_for_removal(projectile);
}

sint32 game_decrement_nonnegative(sint32 value)
{
    uint32 result;
    FUNCTION_MARKER(0x800815E0, "GAME.OVL");
    result = (uint32)value - 1u;
    return (sint32)result < 0 ? 0 : (sint32)result;
}

sint32 game_wrap_signed_angle(uint32 angle)
{
    FUNCTION_MARKER(0x800815F8, "GAME.OVL");
    angle &= 4095u;
    if (angle >= 2048u)
        angle -= 4096u;
    return (sint32)angle;
}

sint32 game_set_acceleration_from_angles(uint32 object, uint32 scale)
{
    uint32 angle, component;
    sint32 result;
    FUNCTION_MARKER(0x800818F8, "GAME.OVL");
    angle = r_u32(object + 0x20u);
    component = (uint32)math_scale_sin(scale, angle);
    angle = r_u32(object + 0x24u);
    result = math_scale_cos(component, 0u - angle);
    angle = r_u32(object + 0x20u);
    w_u32(object + 0xFCu, (uint32)result);
    component = (uint32)math_scale_cos(scale, angle);
    angle = r_u32(object + 0x24u);
    result = math_scale_cos(component, 0u - angle);
    angle = r_u32(object + 0x24u);
    w_u32(object + 0x104u, (uint32)result);
    result = math_scale_sin(scale, 0u - angle);
    w_u32(object + 0x100u, (uint32)result);
    return result;
}

sint32 game_reduce_without_sign_change(sint32 value, uint32 step)
{
    sint32 sign, current_sign;
    uint32 result;
    FUNCTION_MARKER(0x8008197C, "GAME.OVL");
    sign = integer_sign(value);
    current_sign = integer_sign(value);
    result = (uint32)value + (uint32)current_sign * (0u - step);
    return integer_sign((sint32)result) == sign ? (sint32)result : 0;
}

uint32 game_canonicalize_object_angles(uint32 object)
{
    uint32 x, y, z, condition;
    FUNCTION_MARKER(0x800819EC, "GAME.OVL");
    x = r_u32(object + 0x20u);
    y = r_u32(object + 0x24u);
    w_u32(object + 0x20u, x & 4095u);
    z = r_u32(object + 0x28u);
    w_u32(object + 0x24u, y & 4095u);
    y = r_u32(object + 0x24u);
    condition = y - 1025u;
    w_u32(object + 0x28u, z & 4095u);
    if (condition < 1024u) {
        z = r_u32(object + 0x28u);
        x = r_u32(object + 0x20u);
        w_u32(object + 0x28u, z + 2048u);
        y = r_u32(object + 0x24u);
        w_u32(object + 0x20u, x + 2048u);
        w_u32(object + 0x24u, 2048u - y);
    }
    x = r_u32(object + 0x20u);
    y = r_u32(object + 0x24u);
    w_u32(object + 0x20u, x & 4095u);
    z = r_u32(object + 0x28u);
    w_u32(object + 0x24u, y & 4095u);
    y = r_u32(object + 0x24u);
    condition = y - 2049u;
    w_u32(object + 0x28u, z & 4095u);
    if (condition < 1023u) {
        x = r_u32(object + 0x20u);
        y = r_u32(object + 0x24u);
        w_u32(object + 0x20u, x + 2048u);
        z = r_u32(object + 0x28u);
        w_u32(object + 0x24u, y - 2048u);
        w_u32(object + 0x28u, z + 2048u);
    }
    x = r_u32(object + 0x20u);
    z = r_u32(object + 0x28u);
    w_u32(object + 0x20u, x & 4095u);
    y = r_u32(object + 0x24u);
    w_u32(object + 0x28u, z & 4095u);
    w_u32(object + 0x24u, y & 4095u);
    return y & 4095u;
}

sint32 game_set_acceleration_from_basis(uint32 object, uint32 scale)
{
    uint32 x, y, z;
    FUNCTION_MARKER(0x80081AC8, "GAME.OVL");
    x = (uint32)(sint32)r_s16(object + 0x120u) * scale;
    y = (uint32)(sint32)r_s16(object + 0x126u) * scale;
    z = (uint32)(sint32)r_s16(object + 0x12Cu) * scale;
    w_u32(object + 0xFCu, (uint32)((sint32)x >> 12));
    w_u32(object + 0x100u, (uint32)((sint32)y >> 12));
    w_u32(object + 0x104u, (uint32)((sint32)z >> 12));
    return (sint32)z >> 12;
}

uint32 game_step_toward_angle(uint32 target, uint32 current, sint32 limit)
{
    sint32 difference, sign, magnitude;
    FUNCTION_MARKER(0x80081D0C, "GAME.OVL");
    difference = game_wrap_signed_angle(target - current);
    sign = integer_sign(difference);
    magnitude = difference < 0 ? -difference : difference;
    if (limit < magnitude)
        magnitude = limit;
    return current + (uint32)sign * (uint32)magnitude;
}

uint32 game_create_view_controller(void)
{
    uint32 object, offset, identifier, model;
    FUNCTION_MARKER(0x8008246C, "GAME.OVL");
    object = game_create_object(0u, 0u, 0u, 300u, 0u);
    offset = r_u32(0x800ECBD0u) * 44u;
    identifier = (uint32)(sint32)r_s16(0x800ECBEEu + offset);
    w_u32(object + 0x74u, identifier);
    identifier = (uint32)(sint32)r_s16(0x800ECBEEu + offset);
    model = r_u32(0x80046178u);
    model = r_u32(model + identifier * 132u);
    w_u32(object + 0x3Cu, 0x80081D84u);
    w_u16(object + 0x228u, (uint16)-1240);
    w_u16(object + 0x224u, 0u);
    w_u32(object + 0x214u, 0u);
    w_u16(object + 0x226u, 0u);
    w_u32(object + 0x218u, 0u);
    w_u32(object + 0x21Cu, 0u - 1240u);
    w_u32(object + 0x64u, model);
    return object;
}

uint32 game_cycle_view_mode(void)
{
    sint32 index, mode;
    uint32 state, player;
    FUNCTION_MARKER(0x80082530, "GAME.OVL");
    index = (sint32)r_s16(0x800118F4u) + 1;
    index %= 3;
    mode = r_s32(0x800313DCu);
    w_u16(0x800118F4u, (uint16)index);
    if (mode >= 4) {
        w_u32(0x800313DCu, 3u);
        w_u32(0x800ECE70u, 1u);
        return 1u;
    }
    state = r_u32(0x800ECE70u);
    if (state == 1u) {
        w_u32(0x800ECE70u, 0u);
    } else {
        player = r_u32(0x80040FDCu);
        w_u32(0x800313DCu, 4u);
        w_u32(0x8004EB1Cu, player);
    }
    return 4u;
}

void game_draw_screen_flash(void)
{
    uint32 red, green, blue, packet, width, height, table, page, value;
    FUNCTION_MARKER(0x800825DC, "GAME.OVL");
    red = r_u32(0x800ECE9Cu);
    green = r_u32(0x800ECEA0u);
    blue = r_u32(0x800ECEA4u);
    if (red + green + blue == 0u && r_s32(0x800310A8u) <= 0)
        return;
    packet = r_u32(0x80040690u);
    w_u32(0x80040690u, packet + 16u);
    setTile(psx_addr(packet, 16u));
    setSemiTrans(psx_addr(packet, 16u), 1);
    red = r_u32(0x800ECE9Cu);
    width = r_u32(0x8003119Cu);
    height = r_u32(0x800311B0u);
    w_u8(packet + 4u, (uint8)red);
    green = r_u32(0x800ECEA0u);
    w_u8(packet + 5u, (uint8)green);
    blue = r_u32(0x800ECEA4u);
    w_u16(packet + 8u, 0u);
    w_u16(packet + 10u, 0u);
    w_u16(packet + 12u, (uint16)width);
    w_u16(packet + 14u, (uint16)height);
    w_u8(packet + 6u, (uint8)blue);
    red = r_u32(0x800ECE9Cu) - 5u;
    green = r_u32(0x800ECEA0u) - 5u;
    blue = r_u32(0x800ECEA4u) - 5u;
    w_u32(0x800ECE9Cu, red);
    w_u32(0x800ECEA0u, green);
    w_u32(0x800ECEA4u, blue);
    if ((sint32)red < 0)
        w_u32(0x800ECE9Cu, 0u);
    if ((sint32)green < 0)
        w_u32(0x800ECEA0u, 0u);
    if ((sint32)blue < 0)
        w_u32(0x800ECEA4u, 0u);
    table = r_u32(0x8004608Cu) + 0x70u;
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 16u));
    if (r_s32(0x800310A8u) <= 0)
        return;
    packet = r_u32(0x80040690u);
    page = GetTPage(0, 2, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12u), 1, 1, (uint16)page, NULL);
    table = r_u32(0x8004608Cu) + 0x70u;
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 12u));
    value = r_u32(0x800310A8u) - 10u;
    w_u32(0x80040690u, packet + 12u);
    w_u32(0x800310A8u, value);
    if ((sint32)value < 0)
        w_u32(0x800310A8u, 0u);
    red = r_u32(0x800ECE9Cu) - 5u;
    green = r_u32(0x800ECEA0u) - 5u;
    blue = r_u32(0x800ECEA4u) - 5u;
    w_u32(0x800ECE9Cu, red);
    w_u32(0x800ECEA0u, green);
    w_u32(0x800ECEA4u, blue);
    if ((sint32)red < 0)
        w_u32(0x800ECE9Cu, 0u);
    if ((sint32)green < 0)
        w_u32(0x800ECEA0u, 0u);
    if ((sint32)blue < 0)
        w_u32(0x800ECEA4u, 0u);
}

void game_update_view_controller(uint32 object)
{
    uint32 red, green, blue, base_red, base_green, base_blue, player, value, scale;
    uint32 x, y, z, timing, amplitude;
    sint32 mode;
    VECTOR position;
    SVECTOR angles;
    MATRIX delta, light;
    mode = r_s32(0x800313DCu);
    FUNCTION_MARKER(0x80081D8C, "GAME.OVL");
    if (mode < 4 && r_u32(0x800ECE70u) == 1u &&
        (r_u8(0x800ECE98u) != 0u || r_u8(0x800ECE99u) != 0u || r_u8(0x800ECE9Au) != 0u)) {
        red = r_u8(0x800ECE98u);
        base_red = r_u8(0x80042900u);
        green = r_u8(0x800ECE99u);
        blue = r_u8(0x800ECE9Au);
        base_green = r_u8(0x80042901u);
        base_blue = r_u8(0x80042902u);
        red += base_red;
        green += base_green;
        blue += base_blue;
        SetBackColor(red < 256u ? red : 255u, green < 256u ? green : 255u, blue < 256u ? blue : 255u);
    }
    if (r_u32(0x800311DCu) != 0u || r_u32(0x800EC884u) != 0u)
        return;
    mode = r_s32(0x800313DCu);
    if (mode >= 4) {
        value = r_u32(0x8004EB1Cu);
        player = r_u32(0x80040FDCu);
        if (value != player)
            return;
        value = 0u;
    } else {
        value = r_u32(0x800ECE70u);
    }
    if (value == 0u) {
        w_u32(0x800ECE74u, 0u);
        w_u32(0x800ECE78u, 0u);
        w_u32(0x800ECE7Cu, 0u);
        w_u32(0x800ECE84u, 0u);
    }
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u),
        (VECTOR *)psx_addr(object + 0x214u, 12u), &position);
    w_u32(object + 8u, (uint32)position.vx);
    w_u32(object + 12u, (uint32)position.vy);
    w_u32(object + 16u, (uint32)position.vz);
    matrix_copy_rotation(0x800FA340u, object + 0x11Cu);
    x = (uint32)((sint32)r_s16(0x800FA340u) * 8 / 5);
    y = (uint32)((sint32)r_s16(0x800FA342u) * 8 / 5);
    z = (uint32)((sint32)r_s16(0x800FA344u) * 8 / 5);
    w_u16(0x800FA340u, (uint16)x);
    timing = r_u32(0x800311C0u);
    w_u16(0x800FA342u, (uint16)y);
    w_u16(0x800FA344u, (uint16)z);
    if (timing != 0u) {
        x = (uint32)(sint32)r_s16(0x800FA346u);
        scale = r_u32(0x800311C4u);
        x *= scale;
        y = (uint32)(sint32)r_s16(0x800FA348u) * scale;
        z = (uint32)(sint32)r_s16(0x800FA34Au) * scale;
        w_u16(0x800FA346u, (uint16)((sint32)x / 240));
        w_u16(0x800FA348u, (uint16)((sint32)y / 240));
        w_u16(0x800FA34Au, (uint16)((sint32)z / 240));
    }
    if (r_u32(0x800313DCu) == 3u && r_u32(0x800ECE70u) == 1u) {
        amplitude = (uint32)(r_s32(0x800ECE84u) >> 1);
        x = (uint32)(sint32)r_s16(0x800FA34Cu) * amplitude;
        y = (uint32)(sint32)r_s16(0x800FA34Eu) * amplitude;
        z = (uint32)(sint32)r_s16(0x800FA350u) * amplitude;
        w_u32(0x800ECE84u, amplitude);
        w_u32(0x80034D6Cu, r_u32(object + 8u) + (uint32)((sint32)x >> 10));
        w_u32(0x80034D70u, r_u32(object + 12u) + (uint32)((sint32)y >> 10));
        w_u32(0x80034D74u, r_u32(object + 16u) + (uint32)((sint32)z >> 10));
    } else {
        w_u32(0x80034D6Cu, r_u32(object + 8u));
        w_u32(0x80034D70u, r_u32(object + 12u));
        w_u32(0x80034D74u, r_u32(object + 16u));
    }
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    x = r_u32(0x800ECE74u);
    z = r_u32(0x800ECE7Cu);
    angles.vx = (sint16)((sint32)x >> 8);
    y = r_u32(0x800ECE78u);
    angles.vz = (sint16)((sint32)z >> 8);
    angles.vy = (sint16)((sint32)y >> 8);
    RotMatrix(&angles, &delta);
    MulMatrix0((MATRIX *)psx_addr(0x800FA340u, 20u), &delta,
        (MATRIX *)psx_addr(0x800FA340u, 20u));
    matrix_copy_rotation(0x800F32D0u, object + 0x11Cu);
    x = (uint32)((sint32)r_s16(0x800F32D0u) * 8 / 5);
    y = (uint32)((sint32)r_s16(0x800F32D2u) * 8 / 5);
    z = (uint32)((sint32)r_s16(0x800F32D4u) * 8 / 5);
    w_u16(0x800F32D0u, (uint16)x);
    timing = r_u32(0x800311C0u);
    w_u16(0x800F32D2u, (uint16)y);
    w_u16(0x800F32D4u, (uint16)z);
    if (timing != 0u) {
        x = (uint32)(sint32)r_s16(0x800F32D6u);
        scale = r_u32(0x800311C4u);
        x *= scale;
        y = (uint32)(sint32)r_s16(0x800F32D8u) * scale;
        z = (uint32)(sint32)r_s16(0x800F32DAu) * scale;
        w_u16(0x800F32D6u, (uint16)((sint32)x / 240));
        w_u16(0x800F32D8u, (uint16)((sint32)y / 240));
        w_u16(0x800F32DAu, (uint16)((sint32)z / 240));
    }
    angles.vx = (sint16)(angles.vx >> 1);
    angles.vy = (sint16)(angles.vy >> 1);
    angles.vz = (sint16)(angles.vz >> 1);
    RotMatrix(&angles, &delta);
    MulMatrix0((MATRIX *)psx_addr(0x800F32D0u, 20u), &delta,
        (MATRIX *)psx_addr(0x800F32D0u, 20u));
    player = r_u32(0x80040FDCu);
    light.m[0][0] = r_s16(player + 0x11Cu);
    light.m[1][0] = r_s16(player + 0x11Eu);
    light.m[2][0] = r_s16(player + 0x120u);
    light.m[0][1] = r_s16(player + 0x122u);
    light.m[1][1] = r_s16(player + 0x124u);
    light.m[2][1] = r_s16(player + 0x126u);
    light.m[0][2] = r_s16(player + 0x128u);
    value = r_u16(player + 0x12Au);
    w_u32(0x80052838u, 0u);
    light.m[1][2] = (sint16)value;
    value = r_u16(player + 0x12Cu);
    w_u32(0x800313D8u, 0xFFFFFFFFu);
    light.m[2][2] = (sint16)value;
    MulMatrix0((MATRIX *)psx_addr(0x80041F28u, 20u), &light, &light);
    SetLightMatrix(&light);
    SetRotMatrix((MATRIX *)psx_addr(0x800FA340u, 20u));
    game_draw_flight_hud(0x800FA340u, 0x80034D58u, 0x800F32D0u);
    red = r_u8(0x80042900u);
    green = r_u8(0x80042901u);
    blue = r_u8(0x80042902u);
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetBackColor(red, green, blue);
    w_u8(0x800ECE98u, 0u);
    w_u8(0x800ECE99u, 0u);
    w_u8(0x800ECE9Au, 0u);
}

uint32 game_poll_frame_input(void)
{
    uint32 current, previous, pressed;
    FUNCTION_MARKER(0x80082A48, "GAME.OVL");
    if (r_u32(0x8001020Cu) == 1u) {
        w_u32(0x80053678u, 0u);
        w_u32(0x8004616Cu, 0u);
    } else {
        previous = r_u32(0x8004616Cu);
        w_u32(0x80053678u, previous);
        current = (uint32)pad_update(0u);
        w_u32(0x8004616Cu, current);
    }
    if (r_u32(0x8004EB24u) == 1u)
        w_u32(0x800ECEA8u, 3u);
    current = r_u32(0x8004616Cu);
    previous = r_u32(0x80053678u);
    pressed = (current ^ previous) & current;
    w_u32(0x80053684u, pressed);
    return pressed;
}

void game_initialize_weapon_defaults(void)
{
    FUNCTION_MARKER(0x80082CCC, "GAME.OVL");
    w_u32(0x800118E0u, 1u);
    w_u32(0x800118ECu, 250u);
    w_u32(0x800118F0u, 350u);
    w_u32(0x800ED42Cu, 0x800ED3BCu);
    w_u32(0x800EF184u, 600u);
    w_u32(0x800EF1ACu, 600u);
    w_u32(0x800118E4u, 0u);
    w_u32(0x800118E8u, 0u);
    w_u32(0x800EF148u, 900u);
    w_u32(0x800EF170u, 900u);
}

sint32 game_measure_mesh_radius(uint32 mesh)
{
    uint32 vertex, index;
    sint32 count, maximum, length, x, y, z;
    FUNCTION_MARKER(0x8007D2FC, "GAME.OVL");
    maximum = 0;
    index = 0u;
    count = r_s32(mesh + 4u);
    vertex = r_u32(mesh);
    if (count <= 0)
        return maximum;
    do {
        x = r_s16(vertex);
        y = r_s16(vertex + 2u);
        z = r_s16(vertex + 4u);
        length = vector_length_approx(x, y, z);
        if (maximum < length)
            maximum = length;
        count = r_s32(mesh + 4u);
        ++index;
        vertex += 8u;
    } while ((sint32)index < count);
    return maximum;
}

void game_measure_model_axis_bounds(uint32 model)
{
    uint32 mesh, vertex, index;
    sint32 count, x, y, z, extent_x, extent_y, extent_z;
    FUNCTION_MARKER(0x8007D1EC, "GAME.OVL");
    extent_x = 0;
    extent_y = 0;
    extent_z = 0;
    mesh = r_u32(model);
    count = r_s32(mesh + 4u);
    vertex = r_u32(mesh);
    for (index = 0u; (sint32)index < count; ++index) {
        x = r_s16(vertex);
        y = r_s16(vertex + 2u);
        z = r_s16(vertex + 4u);
        if (x < 0)
            x = -x;
        if (extent_x < x)
            extent_x = x;
        if (y < 0)
            y = -y;
        if (extent_y < y)
            extent_y = y;
        if (z < 0)
            z = -z;
        if (extent_z < z)
            extent_z = z;
        vertex += 8u;
    }
    w_u16(model + 0x7Au, (uint16)extent_x);
    w_u16(model + 0x7Cu, (uint16)extent_y);
    w_u16(model + 0x7Eu, (uint16)extent_z);
}

void game_initialize_model_bounds(uint32 model)
{
    uint32 mesh, radius, depth, width, squared, root, product;
    FUNCTION_MARKER(0x80082C44, "GAME.OVL");
    mesh = r_u32(model);
    radius = (uint32)game_measure_mesh_radius(mesh);
    depth = r_u32(0x800313F8u);
    squared = depth * depth;
    width = r_u32(0x8003119Cu);
    w_u32(model + 0x6Cu, radius);
    root = SquareRoot0((sint32)((width << 7) + squared));
    radius = r_u32(model + 0x6Cu);
    product = radius * (root << 1);
    w_u16(model + 0x78u, (uint16)((sint32)product / 512));
    game_measure_model_axis_bounds(model);
}

uint32 game_relocate_tmd_meshes(uint32 data)
{
    uint32 count, base, mesh, index, vertices, normals, primitives, first;
    FUNCTION_MARKER(0x8007D0E0, "GAME.OVL");
    count = r_u16(data + 8u);
    base = data + 12u;
    mesh = base;
    first = 0u;
    for (index = 0u; index < count; ++index) {
        vertices = r_u32(mesh);
        primitives = r_u32(mesh + 0x10u);
        w_u32(mesh, vertices + base);
        normals = r_u32(mesh + 8u);
        w_u32(mesh + 0x10u, primitives + base);
        w_u32(mesh + 8u, normals + base);
        if (index == 0u)
            first = mesh;
        mesh += 28u;
    }
    return first;
}

void game_set_mesh_flat_colors(uint32 mesh, uint32 red, uint32 green, uint32 blue)
{
    uint32 primitive, index, code, length;
    sint32 count;
    FUNCTION_MARKER(0x8007CFD0, "GAME.OVL");
    count = r_s32(mesh + 0x14u);
    primitive = r_u32(mesh + 0x10u);
    for (index = 0u; (sint32)index < count; ++index) {
        code = r_u8(primitive + 3u) & 0xFCu;
        if (code == 32u || code == 40u || code == 48u || code == 56u) {
            w_u8(primitive + 4u, (uint8)red);
            w_u8(primitive + 5u, (uint8)green);
            w_u8(primitive + 6u, (uint8)blue);
        }
        length = r_u8(primitive + 1u);
        primitive += (length << 2) + 4u;
    }
}

uint32 game_load_tmd_model(const char *name)
{
    uint32 index, size, data, mesh, slot;
    FUNCTION_MARKER(0x80082B70, "GAME.OVL");
    index = r_u32(0x800ECEB8u);
    size = disc_file_size(name);
    data = heap_alloc_front(size);
    resource_load_into(name, data);
    if ((sint32)index >= 55)
        diagnostic_report(345u, 0x80074620u, 0x80074630u);
    mesh = game_relocate_tmd_meshes(data);
    slot = r_u32(0x80046178u) + index * 132u;
    w_u32(slot, mesh);
    game_set_mesh_flat_colors(mesh, 255u, 255u, 255u);
    slot = r_u32(0x80046178u) + index * 132u;
    game_initialize_model_bounds(slot);
    w_u32(0x800ECEB8u, r_u32(0x800ECEB8u) + 1u);
    return index;
}

uint32 game_load_resource_bundle(const char *name)
{
    uint32 size, data, first, end;
    FUNCTION_MARKER(0x80082AEC, "GAME.OVL");
    size = disc_file_size(name);
    data = heap_alloc_front(size);
    resource_load_into(name, data);
    first = r_u32(0x800ECEB8u);
    end = game_import_resource_bundle(data, first);
    w_u32(0x800ECEB8u, end);
    heap_free(data);
    end = r_u32(0x800ECEB8u);
    w_u32(0x800ECEC0u, end - first);
    return first;
}

uint32 game_copy_tmd_meshes(uint32 source, uint32 destination, sint32 size)
{
    uint32 input, output, base, count, index, vertices, normals, primitives, first;
    sint32 words;
    FUNCTION_MARKER(0x8007D150, "GAME.OVL");
    input = source;
    output = destination;
    words = size >> 2;
    for (index = 0u; (sint32)index < words; ++index) {
        vertices = r_u32(input);
        input += 4u;
        w_u32(output, vertices);
        output += 4u;
    }
    count = r_u16(destination + 8u);
    base = destination + 12u;
    output = base;
    first = 0u;
    for (index = 0u; index < count; ++index) {
        vertices = r_u32(output);
        primitives = r_u32(output + 0x10u);
        w_u32(output, vertices + base);
        normals = r_u32(output + 8u);
        w_u32(output + 0x10u, primitives + base);
        w_u32(output + 8u, normals + base);
        if (index == 0u)
            first = output;
        output += 28u;
    }
    return first;
}

uint32 game_relocate_collision_data(uint32 data)
{
    uint32 first, second;
    FUNCTION_MARKER(0x80098124, "GAME.OVL");
    first = r_u32(data);
    second = r_u32(data + 8u);
    first += data;
    second += data;
    w_u32(data, first);
    w_u32(data + 8u, second);
    return first;
}

uint32 game_relocate_tmd_texture(uint32 model, const TEX_INFO *info, uint32 texture_index)
{
    uint32 primitive;
    uint32 format;
    uint32 u;
    uint32 v;
    uint32 clut;
    uint32 page;
    uint32 opcode;
    uint32 index;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    sint32 count;
    sint32 tag;
    FUNCTION_MARKER(0x8007D39C, "GAME.OVL");
    primitive = r_u32(model + 16);
    format = info->mode & 3;
    u = ((uint32)(uint16)info->pixel.x & 63) << ((2 - format) & 31);
    v = (uint16)info->pixel.y & 255;
    clut = ((uint32)(sint32)info->clut.y << 6) + (uint32)((sint32)info->clut.x >> 4);
    page = (uint32)((sint32)info->pixel.x >> 6);
    page += (uint32)((sint32)info->pixel.y >> 8) << 4;
    page += format << 7;
    page += r_u32(0x800311C8) << 5;
    count = r_s32(model + 20);
    tag = (sint16)(texture_index | 0x8000);
    for (index = 0; (sint32)index < count; ++index)
    {
        opcode = r_u8(primitive + 3) & 0xFC;
        if ((opcode == 0x24 || opcode == 0x34 || opcode == 0x2C || opcode == 0x3C) && r_s16(primitive + 10) == tag)
        {
            first = r_u8(primitive + 4);
            second = r_u8(primitive + 8);
            w_u16(primitive + 6, clut);
            w_u16(primitive + 10, page);
            w_u8(primitive + 4, first + u);
            third = r_u8(primitive + 12);
            w_u8(primitive + 8, second + u);
            if (opcode == 0x2C || opcode == 0x3C)
            {
                fourth = r_u8(primitive + 16);
                w_u8(primitive + 12, third + u);
                first = r_u8(primitive + 5);
                w_u8(primitive + 16, fourth + u);
                second = r_u8(primitive + 9);
                w_u8(primitive + 5, first + v);
                third = r_u8(primitive + 13);
                w_u8(primitive + 9, second + v);
                fourth = r_u8(primitive + 17);
                w_u8(primitive + 13, third + v);
                w_u8(primitive + 17, fourth + v);
            }
            else
            {
                first = r_u8(primitive + 5);
                w_u8(primitive + 12, third + u);
                second = r_u8(primitive + 9);
                w_u8(primitive + 5, first + v);
                third = r_u8(primitive + 13);
                w_u8(primitive + 9, second + v);
                w_u8(primitive + 13, third + v);
            }
        }
        primitive += ((uint32)r_u8(primitive + 1) << 2) + 4;
    }
    return 0;
}

uint32 game_import_resource_bundle(uint32 data, uint32 first)
{
    TEX_INFO info;
    uint32 end, cursor, chunk, kind, models, texture, collision, fragment_seen;
    uint32 next, allocation, mesh, slot, index, value, count, input, output;
    uint32 cache_a, cache_b, name, converted, flag, total;
    sint32 key_a, key_b, shared, cache_state;
    FUNCTION_MARKER(0x8009125C, "GAME.OVL");
    end = 0u;
    models = 0u;
    texture = 0u;
    collision = 0u;
    fragment_seen = 0u;
    value = r_u32(data);
    w_u16(0x800F3C6Eu, 0u);
    if (value == 0x59444F42u)
        end = data + r_u32(data + 4u) + 8u;
    else
        diagnostic_report(120u, 0x800746E4u, 0x800746FCu);
    cursor = data + 8u;
    while (cursor < end) {
        chunk = cursor;
        next = first + models;
        kind = r_u32(chunk);
        cursor += 8u;
        if (kind == 0x53444D54u) {
            texture = 0u;
            allocation = heap_alloc_back(r_u32(chunk + 4u));
            mesh = game_copy_tmd_meshes(cursor, allocation, r_s32(chunk + 4u));
            ++models;
            slot = r_u32(0x80046178u) + next * 132u;
            w_u32(slot, mesh);
            w_u32(slot + 4u, collision);
            game_initialize_model_bounds(slot);
        } else if (kind == 0x44495247u) {
            allocation = heap_alloc_back(r_u32(chunk + 4u));
            slot = r_u32(0x80046178u) + next * 132u;
            w_u32(slot - 124u, allocation);
            count = (uint32)(r_s32(chunk + 4u) >> 2);
            input = cursor;
            output = allocation;
            index = 0u;
            while ((sint32)index < (sint32)count) {
                value = r_u32(input);
                input += 4u;
                ++index;
                w_u32(output, value);
                count = (uint32)(r_s32(chunk + 4u) >> 2);
                output += 4u;
            }
        } else if (kind == 0x534D4954u) {
            if (r_s16(0x800F3C6Eu) != 0) {
                index = r_u32(0x801077C0u);
                key_a = r_s16(0x800FA220u + index * 2u);
                key_b = r_s16(0x800F3DA0u + index * 2u);
                texture_read_info(cursor + 4u, &info);
                index = r_u32(0x801077C0u);
                cache_b = 0x80121D80u + (uint32)key_b * 16u;
                cache_state = r_s16(cache_b + 2u);
                w_u32(0x801077C0u, index + 1u);
                if (cache_state == -1) {
                    texture_place_info(&info, 0u);
                    texture_upload(cursor, &info, 0u);
                    w_u16(cache_b + 2u, 0u);
                    w_u16(cache_b + 8u, (uint16)info.clut.x);
                    w_u16(cache_b + 10u, (uint16)info.clut.y);
                } else {
                    info.clut.x = r_s16(cache_b + 8u);
                    info.clut.y = r_s16(cache_b + 10u);
                }
                cache_a = 0x80121D80u + (uint32)key_a * 16u;
                if (r_s16(cache_a) == -1) {
                    texture_place_info(&info, 1u);
                    texture_upload(cursor, &info, 1u);
                    w_u16(cache_a, 0u);
                    w_u16(cache_a + 4u, (uint16)info.pixel.x);
                    w_u16(cache_a + 6u, (uint16)info.pixel.y);
                    w_u32(cache_a + 12u, info.mode);
                } else {
                    info.pixel.x = r_s16(cache_a + 4u);
                    info.pixel.y = r_s16(cache_a + 6u);
                    info.mode = r_u32(cache_a + 12u);
                }
            } else {
                texture_read_info(cursor + 4u, &info);
                texture_place_info(&info, 2u);
                texture_upload(cursor, &info, 2u);
            }
            for (index = 0u; (sint32)index < (sint32)models; ++index) {
                slot = r_u32(0x80046178u) + (first + index) * 132u;
                game_relocate_tmd_texture(r_u32(slot), &info, texture);
            }
            ++texture;
        } else if (kind == 0x4D415256u) {
            value = (uint32)(sint32)r_s16(cursor);
            shared = r_s16(0x800FD6F8u);
            w_u32(0x801077C0u, value);
            value = r_u16(cursor + 2u);
            if (shared == -1)
                w_u16(0x800FD6F8u, (uint16)value);
            else if (shared != (sint32)(sint16)value)
                diagnostic_report(0u, 0x80074700u, 0x800746FCu);
            count = r_u32(0x801077C0u);
            input = cursor + 4u;
            for (index = 0u; (sint32)index < (sint32)count; ++index) {
                value = r_u16(input);
                input += 2u;
                w_u16(0x800FA220u + index * 2u, (uint16)value);
                value = r_u16(input);
                input += 2u;
                w_u16(0x800F3DA0u + index * 2u, (uint16)value);
            }
            w_u32(0x801077C0u, 0u);
            w_u16(0x800F3C6Eu, 1u);
        } else if (kind == 0x4C4C4F43u) {
            collision = heap_alloc_back(r_u32(chunk + 4u));
            slot = r_u32(0x80046178u) + next * 132u;
            w_u32(slot - 128u, collision);
            w_u16(slot - 4u, 1u);
            count = (uint32)(r_s32(chunk + 4u) >> 2);
            input = cursor;
            output = collision;
            index = 0u;
            while ((sint32)index < (sint32)count) {
                value = r_u32(input);
                input += 4u;
                ++index;
                w_u32(output, value);
                count = (uint32)(r_s32(chunk + 4u) >> 2);
                output += 4u;
            }
            game_relocate_collision_data(collision);
        } else if (kind == 0x47415246u) {
            index = r_u32(0x8012401Cu);
            name = 0x800536C6u + index * 60u;
            game_bind_model_name(name, first);
            index = r_u32(0x8012401Cu);
            fragment_seen = 1u;
            name = 0x800536C6u + index * 60u;
            value = game_find_model_name(name);
            index = r_u32(0x8012401Cu);
            w_u32(0x8011F130u, value);
            name = 0x800536C6u + index * 60u;
            flag = strncmp((const char *)psx_addr(name, 9u), "WARPGATE", 9u) == 0;
            if (flag == 0u) {
                index = r_u32(0x8012401Cu);
                name = 0x800536C6u + index * 60u;
                flag = strncmp((const char *)psx_addr(name, 9u), "JUMPGATE", 9u) == 0;
            }
            if (flag == 0u) {
                index = r_u32(0x8011F130u);
                slot = r_u32(0x80046178u) + index * 132u;
                if (r_u32(slot + 0x74u) == 0u) {
                    converted = mesh_import_primitives(r_u32(slot));
                    index = r_u32(0x8011F130u);
                    slot = r_u32(0x80046178u) + index * 132u;
                    heap_free(r_u32(slot) - 12u);
                    index = r_u32(0x8011F130u);
                    slot = r_u32(0x80046178u) + index * 132u;
                    w_u32(slot, converted);
                    w_u32(slot + 0x74u, 1u);
                }
            }
            index = r_u32(0x8011F130u);
            count = r_u32(chunk + 4u);
            slot = r_u32(0x80046178u) + index * 132u;
            mesh = r_u32(slot);
            mesh_load_fragments(cursor, slot, mesh);
        } else if (kind != 0x11u && kind != 0x5354414Du && kind != 0x59444F42u) {
            diagnostic_report(666u, 0x8007472Cu, 0x800746FCu);
        }
        value = r_u32(chunk + 4u);
        cursor += value;
    }
    if (fragment_seen != 0u) {
        for (index = 0u; (sint32)index < (sint32)models; ++index) {
            value = first + index;
            slot = r_u32(0x80046178u) + value * 132u;
            flag = r_u32(slot + 0x74u);
            w_u32(0x8011F130u, value);
            if (flag == 0u) {
                converted = mesh_import_primitives(r_u32(slot));
                value = r_u32(0x8011F130u);
                slot = r_u32(0x80046178u) + value * 132u;
                heap_free(r_u32(slot) - 12u);
                value = r_u32(0x8011F130u);
                slot = r_u32(0x80046178u) + value * 132u;
                w_u32(slot, converted);
                w_u32(slot + 0x74u, 1u);
            }
        }
    }
    if (models == 0u)
        diagnostic_report(0u, 0x8007473Cu, 0x800746FCu);
    total = r_u32(0x80118264u);
    w_u32(0x80118264u, total - 1u + models);
    return first + models;
}

uint32 game_uppercase_text(char *text)
{
    uint32 value;
    FUNCTION_MARKER(0x80097A74, "GAME.OVL");
    do {
        value = (uint8)*text;
        if (value - 97u < 26u)
            *text = (char)(uint8)((uint8)*text + 224u);
        ++text;
    } while (value != 0u);
    return 0u;
}

uint32 game_find_model_name(uint32 name)
{
    uint32 length;
    FUNCTION_MARKER(0x800975C4, "GAME.OVL");
    length = 0u;
    while (r_u8(name + length) != 0u)
        ++length;
    return game_find_model_name_native((const char *)psx_addr(name, (size_t)length + 1u));
}

uint32 game_find_model_name_native(const char *name)
{
    char text[64];
    char *extension;
    uint32 length, entry, pointer;
    strcpy(text, name);
    extension = strchr(text, '.');
    if (extension != NULL)
        *extension = '\0';
    game_uppercase_text(text);
    entry = 0x800EE444u;
    while (r_u32(entry) != 0u) {
        pointer = r_u32(entry);
        length = 0u;
        while (r_u8(pointer + length) != 0u)
            ++length;
        if (strcmp((const char *)psx_addr(pointer, (size_t)length + 1u), text) == 0)
            return r_u32(r_u32(entry + 4u));
        entry += 8u;
    }
    diagnostic_report(666u, 0x8007591Cu, 0x8007592Cu);
    return 0u;
}

void game_bind_model_name(uint32 name, uint32 first)
{
    uint32 length;
    FUNCTION_MARKER(0x80097680, "GAME.OVL");
    length = 0u;
    while (r_u8(name + length) != 0u)
        ++length;
    game_bind_model_name_native((const char *)psx_addr(name, (size_t)length + 1u), first);
}

void game_bind_model_name_native(const char *name, uint32 first)
{
    char text[128];
    char *extension;
    uint32 length, entry, pointer;
    strcpy(text, name);
    extension = strchr(text, '.');
    if (extension != NULL)
        *extension = '\0';
    game_uppercase_text(text);
    entry = 0x800EE444u;
    while (r_u32(entry) != 0u) {
        pointer = r_u32(entry);
        length = 0u;
        while (r_u8(pointer + length) != 0u)
            ++length;
        if (strcmp((const char *)psx_addr(pointer, (size_t)length + 1u), text) == 0) {
            pointer = r_u32(entry + 4u);
            w_u32(pointer, first);
            return;
        }
        entry += 8u;
    }
    diagnostic_report(666u, 0x80075930u, 0x8007592Cu);
}

uint32 game_get_variable_address(uint32 name)
{
    char text[128];
    char *extension;
    uint32 length, entry, pointer, index;
    FUNCTION_MARKER(0x80097740, "GAME.OVL");
    if (r_u32(0x800EDD24u) != 0u) {
        ++name;
        length = 0u;
        while (r_u8(name + length) != 0u)
            ++length;
        index = (uint32)string_to_integer((const char *)psx_addr(name, (size_t)length + 1u));
        if (index >= 284u)
            diagnostic_report(0u, 0x8007593Cu, 0x8007592Cu);
        return r_u32(0x800EDFD4u + (index << 2));
    }
    length = 0u;
    while (r_u8(name + length) != 0u)
        ++length;
    strcpy(text, (const char *)psx_addr(name, (size_t)length + 1u));
    extension = strchr(text, '.');
    if (extension != NULL)
        *extension = '\0';
    game_uppercase_text(text);
    entry = 0x800EE444u;
    while (r_u32(entry) != 0u) {
        pointer = r_u32(entry);
        length = 0u;
        while (r_u8(pointer + length) != 0u)
            ++length;
        if (strcmp((const char *)psx_addr(pointer, (size_t)length + 1u), text) == 0)
            return r_u32(entry + 4u);
        entry += 8u;
    }
    return 0u;
}

uint32 game_initialize_model_name_cursor(void)
{
    uint32 entry, pointer, length;
    FUNCTION_MARKER(0x80097AB4, "GAME.OVL");
    entry = 0x800EE444u;
    for (;;) {
        w_u32(0x800F3D98u, entry);
        entry = r_u32(0x800F3D98u);
        pointer = r_u32(entry);
        length = 0u;
        while (r_u8(pointer + length) != 0u)
            ++length;
        if (strcmp((const char *)psx_addr(pointer, (size_t)length + 1u), "MODEL_START") == 0)
            break;
        entry = r_u32(0x800F3D98u) + 8u;
    }
    entry = r_u32(0x800F3D98u) + 8u;
    w_u32(0x800F3D98u, entry);
    return entry;
}

sint32 game_config_set_condition(const uint32 *arguments, uint32 first)
{
    uint32 name, length;
    sint32 value;
    FUNCTION_MARKER(0x80097B2C, "GAME.OVL");
    name = arguments[first + 1u];
    length = 0u;
    while (r_u8(name + length) != 0u)
        ++length;
    value = string_to_integer((const char *)psx_addr(name, (size_t)length + 1u));
    w_u32(0x800EDCF0u, (uint32)value);
    return value;
}

void game_config_load_tmd(const uint32 *arguments, uint32 first)
{
    uint32 name, length, index;
    FUNCTION_MARKER(0x800974C8, "GAME.OVL");
    if (r_u32(0x800EDCF0u) == 0u)
        return;
    name = arguments[first + 1u];
    w_u32(0x80118264u, 1u);
    length = 0u;
    while (r_u8(name + length) != 0u)
        ++length;
    index = game_load_tmd_model((const char *)psx_addr(name, (size_t)length + 1u));
    name = arguments[first + 1u];
    w_u32(0x800EDD1Cu, index);
    game_bind_model_name(name, index);
}

static void game_load_bound_bundle_body(const char *name)
{
    uint32 index;
    index = game_load_resource_bundle(name);
    w_u32(0x800EDD1C, index);
    game_bind_model_name_native(name, index);
}

void game_load_bound_bundle(uint32 name)
{
    uint32 length;
    FUNCTION_MARKER(0x8009756C, "GAME.OVL");
    w_u32(0x80118264u, 1u);
    length = 0u;
    while (r_u8(name + length) != 0u)
        ++length;
    game_load_bound_bundle_body((const char *)psx_addr(name, (size_t)length + 1u));
}

void game_load_bound_bundle_native(const char *name)
{
    w_u32(0x80118264, 1);
    game_load_bound_bundle_body(name);
}

void game_config_load_bundle(const uint32 *arguments, uint32 first)
{
    uint32 condition;
    FUNCTION_MARKER(0x80097528, "GAME.OVL");
    condition = r_u32(0x800EDCF0u);
    w_u32(0x800311C8u, 1u);
    if (condition != 0u)
        game_load_bound_bundle(arguments[first + 1u]);
}

void game_stop_config_parse(void)
{
    FUNCTION_MARKER(0x800975B4, "GAME.OVL");
    w_u32(0x8011F11Cu, 0u);
}

void game_scale_mesh(uint32 mesh, uint32 scale)
{
    uint32 vertex, index, x, y, z;
    sint32 count;
    FUNCTION_MARKER(0x8007D050, "GAME.OVL");
    count = r_s32(mesh + 4u);
    vertex = r_u32(mesh);
    for (index = 0u; (sint32)index < count; ++index) {
        x = (uint32)(sint32)r_s16(vertex) * scale;
        y = (uint32)(sint32)r_s16(vertex + 2u) * scale;
        z = (uint32)(sint32)r_s16(vertex + 4u) * scale;
        w_u16(vertex, (uint16)((sint32)x / 4096));
        w_u16(vertex + 2u, (uint16)((sint32)y / 4096));
        w_u16(vertex + 4u, (uint16)((sint32)z / 4096));
        vertex += 8u;
    }
}

void game_config_scale_models(const uint32 *arguments, uint32 first)
{
    uint32 condition, index, offset, name, length, base, table, slot, scale;
    sint32 count;
    condition = r_u32(0x800EDCF0u);
    FUNCTION_MARKER(0x80097838, "GAME.OVL");
    if (condition == 0u)
        return;
    count = r_s32(0x80118264u);
    if (count <= 0)
        return;
    index = 0u;
    offset = 0u;
    do {
        name = arguments[first + 1u];
        length = 0u;
        while (r_u8(name + length) != 0u)
            ++length;
        scale = (uint32)string_to_integer((const char *)psx_addr(name, (size_t)length + 1u));
        base = r_u32(0x800EDD1Cu);
        table = r_u32(0x80046178u);
        slot = table + (base + index) * 132u;
        game_scale_mesh(r_u32(slot), scale);
        ++index;
        base = r_u32(0x800EDD1Cu);
        table = r_u32(0x80046178u);
        game_initialize_model_bounds(table + base * 132u + offset);
        count = r_s32(0x80118264u);
        offset += 132u;
    } while ((sint32)index < count);
}

void game_config_set_variable(const uint32 *arguments, uint32 first)
{
    uint32 destination, name, value, length, character;
    FUNCTION_MARKER(0x80097234, "GAME.OVL");
    destination = game_get_variable_address(arguments[first + 1u]);
    if (destination == 0u)
        return;
    name = arguments[first + 2u];
    character = r_u8(name);
    if (character - 48u < 10u) {
        length = 0u;
        while (r_u8(name + length) != 0u)
            ++length;
        value = (uint32)string_to_integer((const char *)psx_addr(name, (size_t)length + 1u));
    } else {
        value = game_get_variable_address(name);
    }
    w_u32(destination, value);
}

void game_config_set_pal_variable(const uint32 *arguments, uint32 first)
{
    uint32 destination, name, value, length, character;
    FUNCTION_MARKER(0x800972B8, "GAME.OVL");
    destination = game_get_variable_address(arguments[first + 1u]);
    if (destination == 0u)
        return;
    name = arguments[first + 2u];
    character = r_u8(name);
    if (character - 48u < 10u) {
        length = 0u;
        while (r_u8(name + length) != 0u)
            ++length;
        value = (uint32)string_to_integer((const char *)psx_addr(name, (size_t)length + 1u));
    } else {
        value = game_get_variable_address(name);
    }
    w_u32(destination, value);
}

void game_config_raise_pal_variable(const uint32 *arguments, uint32 first)
{
    uint32 destination, name, value, length, character;
    FUNCTION_MARKER(0x8009733C, "GAME.OVL");
    destination = game_get_variable_address(arguments[first + 1u]);
    if (destination == 0u)
        return;
    name = arguments[first + 2u];
    character = r_u8(name);
    if (character - 48u < 10u) {
        length = 0u;
        while (r_u8(name + length) != 0u)
            ++length;
        value = (uint32)string_to_integer((const char *)psx_addr(name, (size_t)length + 1u));
    } else {
        value = game_get_variable_address(name);
    }
    if (r_u32(0x800311C0u) != 0u)
        value = (uint32)((sint32)(value * 36u) / 25);
    w_u32(destination, value);
}

void game_config_lower_pal_variable(const uint32 *arguments, uint32 first)
{
    uint32 destination, name, value, length, character;
    FUNCTION_MARKER(0x800973FC, "GAME.OVL");
    destination = game_get_variable_address(arguments[first + 1u]);
    if (destination == 0u)
        return;
    name = arguments[first + 2u];
    character = r_u8(name);
    if (character - 48u < 10u) {
        length = 0u;
        while (r_u8(name + length) != 0u)
            ++length;
        value = (uint32)string_to_integer((const char *)psx_addr(name, (size_t)length + 1u));
    } else {
        value = game_get_variable_address(name);
    }
    if (r_u32(0x800311C0u) != 0u)
        value = (uint32)((sint32)(value * 50u) / 60);
    w_u32(destination, value);
}

static void game_dispatch_config_command(uint32 target, const uint32 *arguments, uint32 first)
{
    switch (target) {
    case 0x800974C8u:
        game_config_load_tmd(arguments, first);
        break;
    case 0x80097528u:
        game_config_load_bundle(arguments, first);
        break;
    case 0x80097830u:
        game_config_scale_models(arguments, first);
        break;
    case 0x80097234u:
        game_config_set_variable(arguments, first);
        break;
    case 0x800972B8u:
        game_config_set_pal_variable(arguments, first);
        break;
    case 0x8009733Cu:
        game_config_raise_pal_variable(arguments, first);
        break;
    case 0x800973FCu:
        game_config_lower_pal_variable(arguments, first);
        break;
    case 0x80097B2Cu:
        game_config_set_condition(arguments, first);
        break;
    case 0x800975B4u:
        game_stop_config_parse();
        break;
    default:
        abort();
    }
}

void game_load_configuration(const char *name)
{
    uint32 arguments[20];
    uint32 line, size, data, loaded, cursor, end, count, text, length, callback;
    sint32 command;
    FUNCTION_MARKER(0x80097910, "GAME.OVL");
    line = heap_alloc_back(3000u);
    size = disc_file_size(name);
    if (size == 0u)
        return;
    data = heap_alloc_back(size);
    loaded = resource_load_into(name, data);
    if (loaded == 0u)
        return;
    cursor = data;
    end = data + loaded;
    w_u32(0x8011F11Cu, 1u);
    do {
        w_u32(0x8011A3B8u, 0u);
        cursor = config_read_line(cursor, line);
        if (r_u8(line) != 59u) {
            count = config_split_arguments(line, arguments);
            if ((sint32)count > 0) {
                text = arguments[0];
                length = 0u;
                while (r_u8(text + length) != 0u)
                    ++length;
                command = string_table_find((const char *)psx_addr(text, (size_t)length + 1u), 0x800EDCF4u);
                if (command == -1)
                    diagnostic_report(666u, 0x80075958u, (intptr)name);
                callback = r_u32(0x800EE99Cu + ((uint32)command << 2));
                game_dispatch_config_command(callback, arguments, 0u);
            }
        }
    } while (cursor < end && r_u32(0x8011F11Cu) == 1u);
    heap_free(data);
    heap_free(line);
}

void game_stop_scene_stream(void)
{
    uint32 index;
    FUNCTION_MARKER(0x80082F8C, "GAME.OVL");
    stream_stop();
    stream_cancel_current();
    heap_free(r_u32(0x800F2B08u));
    for (index = 0u; index < 20u; ++index)
        VSync(0);
    VSyncCallbackPSX(0);
}

void game_free_effect_payloads(void)
{
    uint32 effect, allocation;
    FUNCTION_MARKER(0x800B14BC, "GAME.OVL");
    effect = r_u32(0x80124928u);
    while (effect != 0u) {
        allocation = r_u32(effect + 0x6Cu);
        if (allocation != 0u)
            heap_free(allocation);
        effect = r_u32(effect + 0x48u);
    }
}
