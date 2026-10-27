#include "game_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint32 game_set_screen_tint(uint32 red, uint32 green, uint32 blue)
{
    FUNCTION_MARKER(0x8008281C, "GAME.OVL");
    w_u32(0x800ECE9Cu, red);
    w_u32(0x800ECEA0u, green);
    w_u32(0x800ECEA4u, blue);
    if ((sint32)red >= 256)
        w_u32(0x800ECE9Cu, 255u);
    if ((sint32)green >= 256)
        w_u32(0x800ECEA0u, 255u);
    if ((sint32)blue >= 256)
        w_u32(0x800ECEA4u, 255u);
    return 255u;
}

void game_attach_object_trail(uint32 object, uint32 type)
{
    uint32 slot, offset, sample;
    FUNCTION_MARKER(0x800B2738, "GAME.OVL");
    if (type == 0u)
        return;
    offset = 0u;
    for (slot = 0u; slot < 20u; ++slot, offset += 300u) {
        if (r_u32(0x80124D68u + offset) != 0u)
            continue;
        w_u32(0x80124D68u + offset, type);
        w_u32(0x80124D70u + offset, object);
        w_u32(0x80124D6Cu + offset, 0u);
        w_u32(0x80124D78u + offset, 0u);
        w_u32(0x80124D74u + offset, 0u);
        for (sample = 0u; sample < 10u; ++sample, offset += 28u) {
            w_u32(0x80124C64u + offset, 0u);
            w_u32(0x80124C68u + offset, 0u);
        }
        return;
    }
#ifndef NDEBUG
    printf("failed to attatch trail to object\n");
#endif
}

uint32 game_emit_vector_effect_native(uint32 position, VECTOR *direction, uint32 color, uint32 size)
{
    FUNCTION_MARKER(0x800C734C, "GAME.OVL");
    return game_emit_vector_effect_at_native((const VECTOR *)psx_addr(position, 12u), direction, color, size);
}

uint32 game_emit_vector_effect_at_native(const VECTOR *position, VECTOR *direction, uint32 color, uint32 size)
{
    uint32 random, index, record, value, next;
    sint32 divisor, remainder;
    random = r_u32(0x800EF50Cu);
    divisor = (sint32)size >> 1;
    random = (uint32)((sint32)random >> 1) ^ (random << 3);
    if (divisor == 0)
        xport_mips_break(7u);
    remainder = (sint32)(random & 32767u) % divisor;
    index = r_u32(0x800EF4F4u);
    record = 0x800FB5C8u + index * 56u;
    w_u32(record + 16u, 200u);
    w_u32(record + 28u, color & 255u);
    w_u32(record + 32u, (color >> 8) & 255u);
    w_u32(record + 36u, (color >> 16) & 255u);
    w_u32(record + 24u, size + (uint32)remainder);
    w_u32(record, (uint32)position->vx);
    w_u32(record + 4u, (uint32)position->vy);
    w_u32(record + 8u, (uint32)position->vz);
    value = (uint32)direction->vx;
    next = index + 1u;
    w_u32(0x800EF50Cu, random);
    w_u32(record + 40u, value);
    w_u32(record + 44u, (uint32)direction->vy);
    value = (uint32)direction->vz;
    w_u32(record + 20u, 20u);
    w_u32(record + 48u, value);
    next = (uint32)((sint32)next % 128);
    w_u32(0x800EF4F4u, next);
    return next;
}

void game_emit_radial_effect(uint32 x, uint32 y, uint32 z, uint32 size, uint32 color)
{
    SVECTOR vertex;
    sint32 screen, projection, flags, divisor, half_width, half_height;
    uint32 object, dx, dy, dz, ax, ay, az, shifts, depth, packet, numerator;
    uint32 left, right, top, bottom, page_x, page_y, palette_x, palette_y;
    uint16 page, palette;
    FUNCTION_MARKER(0x800C6E9C, "GAME.OVL");
    object = r_u32(0x80040C94u);
    dx = x - r_u32(object + 8u);
    dy = y - r_u32(object + 12u);
    dz = z - r_u32(object + 16u);
    shifts = 0u;
    for (;;) {
        ax = (sint32)dx < 0 ? 0u - dx : dx;
        ay = (sint32)dy < 0 ? 0u - dy : dy;
        az = (sint32)dz < 0 ? 0u - dz : dz;
        if ((sint32)ax < 32001 && (sint32)ay < 32001 && (sint32)az < 32001)
            break;
        ++shifts;
        dx = (uint32)((sint32)dx >> 1);
        dy = (uint32)((sint32)dy >> 1);
        dz = (uint32)((sint32)dz >> 1);
    }
    vertex.vx = (sint16)dx;
    vertex.vy = (sint16)dy;
    vertex.vz = (sint16)dz;
    depth = (uint32)RotTransPers(&vertex, &screen, &projection, &flags);
    depth <<= (shifts + 2u) & 31u;
    if ((sint32)depth <= 0)
        return;
    numerator = size * 64000u;
    divisor = (sint32)depth >> 1;
    if (divisor == 0)
        xport_mips_break(7u);
    half_width = ((sint32)numerator / divisor) >> 9;
    depth -= 8192u;
    if ((sint32)depth <= 0)
        depth = 1u;
    if (half_width >= 65)
        half_width = 64;
    half_height = half_width >> 1;
    x = (uint32)(sint32)(sint16)screen;
    packet = r_u32(0x80040690u);
    y = (uint32)(sint32)(sint16)((uint32)screen >> 16);
    left = x - (uint32)half_width;
    right = x + (uint32)half_width;
    top = y - (uint32)half_height;
    bottom = y + (uint32)half_height;
    w_u32(packet + 4u, color);
    SetPolyFT4((POLY_FT4 *)psx_addr(packet, sizeof(POLY_FT4)));
    SetSemiTrans((POLY_FT4 *)psx_addr(packet, sizeof(POLY_FT4)), 1);
    w_u8(packet + 12u, (uint8)r_u32(0x800F4020u));
    w_u8(packet + 13u, (uint8)r_u32(0x800F4230u));
    w_u8(packet + 20u, (uint8)r_u32(0x800F422Cu));
    w_u8(packet + 21u, (uint8)r_u32(0x800F4230u));
    w_u8(packet + 28u, (uint8)r_u32(0x800F4020u));
    w_u8(packet + 29u, (uint8)r_u32(0x800F4234u));
    w_u8(packet + 36u, (uint8)r_u32(0x800F422Cu));
    w_u8(packet + 37u, (uint8)r_u32(0x800F4234u));
    page_x = r_u32(0x800F4F60u);
    page_y = r_u32(0x800F4F70u);
    w_u16(packet + 8u, (uint16)left);
    w_u16(packet + 10u, (uint16)top);
    w_u16(packet + 16u, (uint16)right);
    w_u16(packet + 18u, (uint16)top);
    w_u16(packet + 24u, (uint16)left);
    w_u16(packet + 26u, (uint16)bottom);
    w_u16(packet + 32u, (uint16)right);
    w_u16(packet + 34u, (uint16)bottom);
    page = GetTPage(0, 1, (sint16)page_x, (sint16)page_y);
    palette_x = r_u32(0x800F4F28u);
    palette_y = r_u32(0x800F4F30u);
    w_u16(packet + 22u, page);
    palette = GetClut((sint32)palette_x, (sint16)palette_y);
    w_u16(packet + 14u, palette);
    render_insert_primitive((sint32)depth, packet);
    w_u32(0x80040690u, r_u32(0x80040690u) + 40u);
}

void game_fire_auxiliary_weapon(uint32 slot)
{
    uint32 player, target, projectile, linked, identifier;
    sint32 component;
    FUNCTION_MARKER(0x8007EF74, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    target = r_u32(0x800EF4E0u);
    w_u32(0x80119C50u + slot * 52u, 30u);
    projectile = game_create_weapon_projectile(player, target, 0x800ECDFCu, 0u);
    player = r_u32(0x80040FDCu);
    w_u32(projectile + 0x1E4u, 0x800EED7Cu);
    w_u32(projectile, 212u);
    w_u32(projectile + 0x3Cu, 0x800B37B0u);
    w_u32(projectile + 0x38u, 0x800B4184u);
    w_u32(projectile + 0x40u, 0x800812E0u);
    w_u32(projectile + 0x9Cu, 120u);
    component = -r_s16(player + 0x128u);
    w_u32(projectile + 0xFCu, (uint32)(component >> 5));
    component = -r_s16(player + 0x12Au);
    w_u32(projectile + 0x100u, (uint32)(component >> 5));
    component = r_s16(player + 0x12Cu);
    w_u32(projectile + 0x14u, 0u);
    w_u32(projectile + 0x18u, 0u);
    w_u32(projectile + 0x1Cu, 0u);
    w_u32(projectile + 0x104u, (uint32)((-component) >> 5));
    sound_play_sample_full(14u);
    player = r_u32(0x80040FDCu);
    target = r_u32(player + 0x1A0u);
    if (target == 0u)
        return;
    identifier = r_u16(target + 0x24Au);
    if (identifier != r_u16(player + 0x1A4u))
        return;
    linked = r_u32(target + 0x178u);
    if (linked != player)
        return;
    w_u32(target + 0x178u, projectile);
    target = r_u32(linked + 0x1A0u);
    identifier = r_u16(projectile + 0x24Au);
    w_u16(target + 0x17Cu, (uint16)identifier);
}

void game_dispatch_destruction_shape(uint32 object)
{
    FUNCTION_MARKER(0x800C2428, "GAME.OVL");
    if (r_u32(object + 0x68u) != 0u)
        game_emit_model_breakup(object);
    else
        game_emit_object_debris(object);
}

void game_launch_escape_pod_sequence(void)
{
    uint32 lists[3], list, object, player, pod, x, y, z, px, py, pz, value, component, random, next, intensity;
    uint32 row, column, result, model;
    sint32 nearest, distance;
    FUNCTION_MARKER(0x8007B840, "GAME.OVL");
    lists[0] = r_u32(0x80074500u);
    lists[1] = r_u32(0x80074504u);
    lists[2] = r_u32(0x80074508u);
    for (;;) {
        nearest = 0x7FFFFFF;
        for (list = 0u; list < 3u; ++list) {
            object = object_lookup(lists[list]);
            while (object != 0u) {
                if ((r_u32(object + 0x114u) & 0x10u) != 0u) {
                    player = r_u32(0x80040FDCu);
                    x = r_u32(object + 8u);
                    y = r_u32(object + 12u);
                    px = r_u32(player + 8u);
                    py = r_u32(player + 12u);
                    pz = r_u32(player + 16u);
                    z = r_u32(object + 16u);
                    distance = vector_length_approx((sint32)(x - px), (sint32)(y - py), (sint32)(z - pz));
                    w_u32(0x8011F130u, (uint32)distance);
                    if (distance < nearest)
                        nearest = distance;
                }
                object = r_u32(object + 0x1DCu);
            }
        }
        if (nearest > 160000)
            break;
        player = r_u32(0x80040FDCu);
        x = (uint32)(sint32)r_s16(player + 0x128u);
        y = (uint32)(sint32)r_s16(player + 0x12Au);
        value = x * 625u;
        px = r_u32(player + 8u) - (uint32)((sint32)value >> 8);
        value = y * 625u;
        w_u32(player + 8u, px);
        py = r_u32(player + 12u);
        z = (uint32)(sint32)r_s16(player + 0x12Cu);
        py -= (uint32)((sint32)value >> 8);
        value = z * 625u;
        w_u32(player + 12u, py);
        pz = r_u32(player + 16u) - (uint32)((sint32)value >> 8);
        w_u32(player + 16u, pz);
    }
    player = r_u32(0x80040FDCu);
    model = r_u32(0x800EDDE0u);
    x = r_u32(player + 8u);
    y = r_u32(player + 12u);
    z = r_u32(player + 16u);
    w_u32(0x800EEF14u, r_u32(0x800311B8u));
    pod = game_create_object(model, x, y, z, 0u);
    w_u32(pod + 0x3Cu, 0x8002D2BCu);
    w_u32(pod + 0x38u, 0x8002B8FCu);
    value = (uint32)(sint32)r_s16(0x80059614u);
    player = r_u32(0x80040FDCu);
    w_u32(pod + 4u, value);
    for (row = 0u; row < 3u; ++row) {
        for (column = 0u; column < 3u; ++column) {
            component = r_u16(player + 0x11Cu + column * 6u + row * 2u);
            if (row == 2u && column == 2u) {
                w_u32(pod + 0x40u, 0x8007B12Cu);
                w_u32(pod + 0x9Cu, 1u);
                w_u32(pod + 0x84u, 0xFFFFFFFDu);
            }
            w_u16(pod + 0x11Cu + row * 6u + column * 2u, (uint16)component);
        }
    }
    value = (uint32)random_signed_between(3, 32);
    w_u32(pod + 0xA8u, value);
    value = (uint32)random_signed_between(3, 32);
    random = r_u32(0x800EF50Cu);
    player = r_u32(0x80040FDCu);
    w_u32(pod + 0xACu, value);
    w_u32(pod + 0xD8u, 200000u);
    next = (uint32)((sint32)random >> 1) ^ (random << 3);
    intensity = (next & 15u) | 32u;
    random = (uint32)((sint32)next >> 1) ^ (next << 3);
    w_u32(pod + 0x170u, player);
    component = r_u16(player + 0x24Au);
    w_u32(0x800EF50Cu, random);
    w_u16(pod + 0x174u, (uint16)component);
    game_attach_object_light(pod, 10u, 0xBBBBBBu, intensity, random & 31u);
    player = r_u32(0x80040FDCu);
    w_u32(pod + 0x48u, 0x800A5030u);
    w_u32(pod + 0xF0u, 512u);
    component = r_u16(player + 0x122u);
    value = r_u32(pod + 8u) - (uint32)((sint32)(sint16)component >> 11);
    w_u32(pod + 8u, value);
    component = r_u16(player + 0x124u);
    value = r_u32(pod + 12u) - (uint32)((sint32)(sint16)component >> 11);
    w_u32(pod + 12u, value);
    component = r_u16(player + 0x126u);
    value = r_u32(pod + 16u) - (uint32)((sint32)(sint16)component >> 11);
    w_u32(pod + 16u, value);
    component = r_u16(player + 0x128u);
    value = r_u32(pod + 8u) - (uint32)((sint32)(sint16)component >> 11);
    w_u32(pod + 8u, value);
    component = r_u16(player + 0x12Au);
    value = r_u32(pod + 12u) - (uint32)((sint32)(sint16)component >> 11);
    w_u32(pod + 12u, value);
    component = r_u16(player + 0x12Cu);
    value = r_u32(pod + 16u) - (uint32)((sint32)(sint16)component >> 11);
    w_u32(pod + 16u, value);
    x = r_u32(player + 8u);
    y = r_u32(player + 12u);
    z = r_u32(player + 16u);
    game_emit_wide_particles(8u, x, y, z, 0u, 0u, 0u);
    player = r_u32(0x80040FDCu);
    x = r_u32(player + 8u);
    y = r_u32(player + 12u);
    z = r_u32(player + 16u);
    game_emit_narrow_particles(8u, x, y, z, 0u, 0u, 0u);
    game_set_screen_tint(255u, 255u, 255u);
    value = r_u32(0x800313DCu);
    w_u32(0x8011F2B8u, pod);
    w_u32(0x800F32C8u, (sint32)value >= 4 ? 1u : 0u);
    w_u32(0x800F32C8u, 1u);
    w_u32(0x80126A98u, 1024u);
    w_u32(0x800313DCu, 5u);
    w_u32(0x800EC888u, 60u);
    camera_reset_follow();
    if (r_u32(0x800F32C8u) != 0u) {
        camera_set_target_pair(pod, r_u32(0x8003FE74u));
        camera_set_follow_distance(r_u32(0x80126A98u), 1u);
    } else {
        camera_set_target(r_u32(0x80040FDCu));
        camera_set_look_at_point(pod + 8u);
    }
    camera_request_snap();
    message_init();
    result = (uint32)random_between(0, 2);
    w_u32(0x8011F130u, result);
    if (result == 0u)
        game_request_radio_message(21u, r_u32(0x800F23D8u));
    else if (result == 1u)
        game_request_radio_message(20u, r_u32(0x800F23DCu));
    else if (result == 2u)
        game_request_radio_message(17u, r_u32(0x800F23E0u));
}

uint32 game_create_weapon_projectile(uint32 owner, uint32 target, uint32 offset, uint32 mode)
{
    uint32 type, kind, projectile, value, player, identifier, x, y, z;
    FUNCTION_MARKER(0x8007FA2C, "GAME.OVL");
    type = r_u32(owner);
    kind = type == 3u ? 4u : type == 1u ? 2u : 5u;
    projectile = game_create_object(0u, 0u, 0u, 0u, kind);
    w_u32(projectile + 0x1E4u, 0x800EEB9Cu);
    if (offset != 0u) {
        game_position_at_object_offset(owner, offset, projectile);
    } else {
        w_u32(projectile + 8u, r_u32(owner + 8u));
        w_u32(projectile + 12u, r_u32(owner + 12u));
        w_u32(projectile + 16u, r_u32(owner + 16u));
    }
    object_extract_angles(projectile, owner);
    w_u32(projectile + 0x3Cu, 0x8002493Cu);
    w_u32(projectile, 207u);
    w_u32(projectile + 0x160u, owner);
    identifier = r_u16(owner + 0x24Au);
    player = r_u32(0x80040FDCu);
    w_u32(projectile + 0x178u, target);
    w_u16(projectile + 0x164u, (uint16)identifier);
    identifier = r_u16(target + 0x24Au);
    w_u32(projectile + 0xECu, 200u);
    w_u32(projectile + 0x9Cu, 5u);
    w_u16(projectile + 0x17Cu, (uint16)identifier);
    w_u32(projectile + 0xC4u, target == player ? 30u : 0u);
    w_u32(projectile + 0x40u, 0x8008068Cu);
    w_u32(projectile + 0x5Cu, 0x8008132Cu);
    w_u32(projectile + 0x48u, 0x8008132Cu);
    game_get_object_forward(owner, projectile + 0xFCu);
    x = r_u32(projectile + 0xFCu);
    z = r_u32(projectile + 0x104u);
    w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 3));
    y = r_u32(projectile + 0x100u);
    w_u32(projectile + 0x104u, (uint32)((sint32)z >> 3));
    z = r_u32(projectile + 0x104u);
    w_u32(projectile + 0x100u, (uint32)((sint32)y >> 3));
    x = r_u32(projectile + 0xFCu);
    y = r_u32(projectile + 0x100u);
    w_u32(projectile + 0x1Cu, z);
    w_u32(projectile + 0x14u, x);
    w_u32(projectile + 0x18u, y);
    if (target != 0u) {
        value = r_u32(target);
        if (value != 0u) {
            w_u32(target + 0x1A0u, projectile);
            identifier = r_u16(projectile + 0x24Au);
            w_u16(target + 0x1A4u, (uint16)identifier);
        }
    }
    game_attach_object_trail(projectile, mode);
    return projectile;
}

uint32 game_get_object_forward(uint32 object, uint32 destination)
{
    uint32 player, value;
    FUNCTION_MARKER(0x800AE8DC, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    if (object == player) {
        value = (uint32)(sint32)r_s16(object + 0x128u);
        w_u32(destination, value);
        value = (uint32)(sint32)r_s16(object + 0x12Au);
        w_u32(destination + 4u, value);
    } else {
        value = (uint32)(sint32)r_s16(object + 0x120u);
        w_u32(destination, value);
        value = (uint32)(sint32)r_s16(object + 0x126u);
        w_u32(destination + 4u, value);
    }
    value = (uint32)(sint32)r_s16(object + 0x12Cu);
    w_u32(destination + 8u, value);
    return value;
}

VECTOR *game_apply_transposed_short_matrix_native(const MATRIX *matrix, SVECTOR *input, VECTOR *output)
{
    MATRIX transposed;
    FUNCTION_MARKER(0x80097BE8, "GAME.OVL");
    transposed.m[0][0] = matrix->m[0][0];
    transposed.m[0][1] = matrix->m[1][0];
    transposed.m[0][2] = matrix->m[2][0];
    transposed.m[1][0] = matrix->m[0][1];
    transposed.m[1][1] = matrix->m[1][1];
    transposed.m[1][2] = matrix->m[2][1];
    transposed.m[2][0] = matrix->m[0][2];
    transposed.m[2][1] = matrix->m[1][2];
    transposed.m[2][2] = matrix->m[2][2];
    return ApplyMatrix(&transposed, input, output);
}

uint32 game_position_at_object_offset(uint32 object, uint32 offset, uint32 destination)
{
    uint32 player, value, component;
    FUNCTION_MARKER(0x800A47E4, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    if (object == player)
        game_apply_transposed_short_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u),
            (SVECTOR *)psx_addr(offset, 8u), (VECTOR *)psx_addr(destination + 8u, 12u));
    else
        ApplyMatrix((MATRIX *)psx_addr(object + 0x11Cu, 20u),
            (SVECTOR *)psx_addr(offset, 8u), (VECTOR *)psx_addr(destination + 8u, 12u));
    value = r_u32(destination + 8u);
    component = r_u32(object + 8u);
    w_u32(destination + 8u, value + component);
    value = r_u32(destination + 12u);
    component = r_u32(object + 12u);
    w_u32(destination + 12u, value + component);
    value = r_u32(destination + 16u);
    component = r_u32(object + 16u);
    value += component;
    w_u32(destination + 16u, value);
    return value;
}

void game_emit_wide_particles(uint32 count, uint32 x, uint32 y, uint32 z, uint32 vx, uint32 vy, uint32 vz)
{
    uint32 index, offset, record, base, random, next, value, step_x, step_y, step_z;
    FUNCTION_MARKER(0x800AFD4C, "GAME.OVL");
    if ((sint32)count <= 0)
        return;
    step_x = (uint32)((sint32)vx >> 6);
    step_y = (uint32)((sint32)vy >> 6);
    step_z = (uint32)((sint32)vz >> 6);
    do {
        --count;
        index = r_u32(0x800EF10Cu);
        base = r_u32(0x80123F64u);
        random = r_u32(0x800EF50Cu);
        offset = index * 28u;
        record = base + offset;
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        value = step_x + (next & 511u) - 256u;
        w_u32(0x800EF50Cu, next);
        random = (uint32)((sint32)next >> 1) ^ (next << 3);
        w_u16(record + 2u, (uint16)value);
        value = step_y + (random & 511u) - 256u;
        ++index;
        w_u32(0x800EF50Cu, random);
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        w_u16(record + 4u, (uint16)value);
        value = step_z + (next & 511u) - 256u;
        w_u32(0x800EF50Cu, next);
        random = (uint32)((sint32)next >> 1) ^ (next << 3);
        w_u16(record + 6u, (uint16)value);
        w_u32(0x800EF50Cu, random);
        index = (uint32)((sint32)index % 450);
        w_u32(0x800EF10Cu, index);
        w_u8(record, 2u);
        base = r_u32(0x80123F64u);
        w_u32(record + 8u, x);
        w_u32(record + 12u, y);
        w_u32(record + 16u, z);
        w_u32(record + 24u, random & 63u);
        w_u8(base + offset + 1u, 0u);
    } while ((sint32)count > 0);
}

void game_emit_narrow_particles(uint32 count, uint32 x, uint32 y, uint32 z, uint32 vx, uint32 vy, uint32 vz)
{
    uint32 index, offset, record, base, random, next, value, step_x, step_y, step_z;
    FUNCTION_MARKER(0x800AF964, "GAME.OVL");
    if ((sint32)count <= 0)
        return;
    step_x = (uint32)((sint32)vx >> 6);
    step_y = (uint32)((sint32)vy >> 6);
    step_z = (uint32)((sint32)vz >> 6);
    do {
        --count;
        index = r_u32(0x800EF10Cu);
        base = r_u32(0x80123F64u);
        random = r_u32(0x800EF50Cu);
        offset = index * 28u;
        record = base + offset;
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        value = step_x + (next & 127u) - 64u;
        w_u32(0x800EF50Cu, next);
        random = (uint32)((sint32)next >> 1) ^ (next << 3);
        w_u16(record + 2u, (uint16)value);
        value = step_y + (random & 127u) - 64u;
        ++index;
        w_u32(0x800EF50Cu, random);
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        w_u16(record + 4u, (uint16)value);
        value = step_z + (next & 127u) - 64u;
        w_u32(0x800EF50Cu, next);
        random = (uint32)((sint32)next >> 1) ^ (next << 3);
        w_u16(record + 6u, (uint16)value);
        w_u32(0x800EF50Cu, random);
        index = (uint32)((sint32)index % 450);
        w_u32(0x800EF10Cu, index);
        w_u8(record, 2u);
        base = r_u32(0x80123F64u);
        w_u32(record + 8u, x);
        w_u32(record + 12u, y);
        w_u32(record + 16u, z);
        w_u32(record + 24u, random & 63u);
        w_u8(base + offset + 1u, 0u);
    } while ((sint32)count > 0);
}

void game_attach_object_light(uint32 object, uint32 type, uint32 color, uint32 intensity, uint32 parameter)
{
    uint32 count, mesh, tail, vertices, record, source, scale, value;
    FUNCTION_MARKER(0x8008CCEC, "GAME.OVL");
    count = r_u32(0x800ED390u);
    if ((sint32)count >= 64) {
#ifndef NDEBUG
        printf("run out of lights!\n");
#endif
        return;
    }
    mesh = r_u32(object + 0x64u);
    tail = r_u32(object + 0x118u);
    vertices = r_u32(mesh);
    if (tail == 0u) {
        record = 0x80124020u + count * 36u;
        w_u32(object + 0x118u, record);
    } else {
        while (r_u32(tail + 32u) != 0u)
            tail = r_u32(tail + 32u);
        count = r_u32(0x800ED390u);
        record = 0x80124020u + count * 36u;
        w_u32(tail + 32u, record);
    }
    scale = r_u32(object + 0x84u);
    source = vertices + (type << 3);
    count = r_u32(0x800ED390u);
    record = 0x80124020u + count * 36u;
    if ((sint32)scale > 0) {
        value = (uint32)(sint32)r_s16(source);
        w_u32(record, value << (scale & 31u));
        value = (uint32)(sint32)r_s16(source + 2u);
        scale = r_u32(object + 0x84u);
        w_u32(record + 4u, value << (scale & 31u));
        value = (uint32)(sint32)r_s16(source + 4u);
        scale = r_u32(object + 0x84u);
        w_u32(record + 8u, value << (scale & 31u));
    } else {
        value = (uint32)(sint32)r_s16(source);
        w_u32(record, (uint32)((sint32)value >> ((0u - scale) & 31u)));
        scale = r_u32(object + 0x84u);
        value = (uint32)(sint32)r_s16(source + 2u);
        w_u32(record + 4u, (uint32)((sint32)value >> ((0u - scale) & 31u)));
        scale = r_u32(object + 0x84u);
        value = (uint32)(sint32)r_s16(source + 4u);
        w_u32(record + 8u, (uint32)((sint32)value >> ((0u - scale) & 31u)));
    }
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

void game_fire_weapon_profile_16(uint32 slot)
{
    uint32 record, value, rate, region, limit;
    FUNCTION_MARKER(0x8007E474, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    if (r_u32(record + 24u) != 0u) {
        value = r_u32(record + 28u);
        rate = r_u32(record + 12u);
        region = r_u32(0x800311C0u);
        value += rate;
        w_u32(record + 28u, value);
        if (region != 0u && r_u32(0x800F2B68u) != 0u)
            w_u32(record + 28u, value + r_u32(record + 12u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)limit < (sint32)value)
            w_u32(record + 28u, limit);
    }
    if (r_u32(record + 32u) == 0u) {
        game_emit_weapon_profile_16();
        if (r_u32(record + 24u) != 0u)
            w_u32(record + 32u, r_u32(record + 4u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)value >= (sint32)limit)
            w_u32(record + 32u, r_u32(record + 8u));
    }
    if (r_u32(record + 24u) == 0u) {
        value = r_u32(record + 4u);
        w_u32(record + 24u, 1u);
        w_u32(record + 32u, value);
    }
}

void game_emit_weapon_profile_16(void)
{
    uint32 selected, state, profile, player, volume;
    sint32 sample;
    FUNCTION_MARKER(0x8007F244, "GAME.OVL");
    selected = r_u32(0x800ECBD0u);
    state = r_u32(0x800313DCu);
    profile = r_u32(0x800ECBF8u + selected * 44u);
    if (r_s16(profile + 0x16u) != 0) {
        player = r_u32(0x80040FDCu);
        game_create_profile_16_effect(player, player, 0x800ECE58u + (state != 3u ? 8u : 0u));
    }
    sample = r_s32(0x800429C0u);
    volume = r_u16(0x800118FCu);
    sound_start_voice_at(sample, (volume >> 3) & 255u, 0, 0, 128u, 0u, 20u);
}

uint32 game_create_profile_16_effect(uint32 owner, uint32 target, uint32 offset)
{
    uint32 effect, x, y, z, px, py, pz;
    FUNCTION_MARKER(0x800B1AF8, "GAME.OVL");
    effect = game_create_profile_effect(owner, target, offset, 0x800EF17Cu);
    w_u32(effect + 0x14u, 0x800EF1F4u);
    w_u32(effect + 0x1Cu, 2u);
    w_u32(effect + 0x68u, 0x800EEC9Cu);
    x = r_u32(effect + 0x20u);
    y = r_u32(effect + 0x24u);
    w_u32(effect + 0x20u, 0u - x);
    z = r_u32(effect + 0x28u);
    w_u32(effect + 0x24u, 0u - y);
    px = r_u32(effect);
    x = r_u32(effect + 0x20u);
    px += x;
    w_u32(effect + 0x28u, 0u - z);
    py = r_u32(effect + 4u);
    y = r_u32(effect + 0x24u);
    w_u32(effect, px);
    pz = r_u32(effect + 8u);
    z = r_u32(effect + 0x28u);
    w_u32(effect + 4u, py + y);
    w_u32(effect + 8u, pz + z);
    return effect;
}

uint32 game_create_profile_effect(uint32 owner, uint32 target, uint32 offset, uint32 profile)
{
    FUNCTION_MARKER(0x800B1514, "GAME.OVL");
    return game_create_profile_effect_native(owner, target,
        offset != 0u ? (SVECTOR *)psx_addr(offset, 8u) : NULL, profile);
}

uint32 game_create_profile_effect_native(uint32 owner, uint32 target, SVECTOR *offset, uint32 profile)
{
    uint32 effect, player, value, component, scale, shift, x, y, z, product, vy_offset;
    effect = game_allocate_effect(r_u32(profile), owner);
    if (offset != NULL) {
        player = r_u32(0x80040FDCu);
        if (owner == player) {
            game_apply_transposed_short_matrix_native((const MATRIX *)psx_addr(owner + 0x11Cu, 18u),
                offset, (VECTOR *)psx_addr(effect, 12u));
        } else {
            ApplyMatrix((MATRIX *)psx_addr(owner + 0x11Cu, 20u),
                offset, (VECTOR *)psx_addr(effect, 12u));
            scale = (uint32)(sint32)(sint16)r_u32(owner + 0x84u);
            x = r_u32(effect);
            z = r_u32(effect + 8u);
            if ((sint32)scale < 0) {
                shift = (0u - scale) & 31u;
                w_u32(effect, (uint32)((sint32)x >> shift));
                y = r_u32(effect + 4u);
                w_u32(effect + 8u, (uint32)((sint32)z >> shift));
                w_u32(effect + 4u, (uint32)((sint32)y >> shift));
            } else {
                shift = scale & 31u;
                w_u32(effect, x << shift);
                y = r_u32(effect + 4u);
                w_u32(effect + 8u, z << shift);
                w_u32(effect + 4u, y << shift);
            }
        }
        value = r_u32(effect);
        component = r_u32(owner + 8u);
        w_u32(effect, value + component);
        value = r_u32(effect + 4u);
        component = r_u32(owner + 12u);
        w_u32(effect + 4u, value + component);
        value = r_u32(effect + 8u);
        component = r_u32(owner + 16u);
        value += component;
    } else {
        w_u32(effect, r_u32(owner + 8u));
        w_u32(effect + 4u, r_u32(owner + 12u));
        value = r_u32(owner + 16u);
    }
    w_u32(effect + 8u, value);
    value = r_u32(profile + 4u);
    player = r_u32(0x80040FDCu);
    w_u16(effect + 0x3Cu, (uint16)value);
    value = r_u32(profile + 16u);
    w_u32(effect + 0x2Cu, value);
    if (owner == player) {
        component = (uint32)(sint32)r_s16(owner + 0x128u);
        vy_offset = 0x12Au;
    } else {
        component = (uint32)(sint32)r_s16(owner + 0x120u);
        vy_offset = 0x126u;
    }
    product = component * r_u32(profile + 8u);
    w_u32(effect + 0x20u, (uint32)((sint32)product >> 12));
    component = (uint32)(sint32)r_s16(owner + vy_offset);
    product = component * r_u32(profile + 8u);
    w_u32(effect + 0x24u, (uint32)((sint32)product >> 12));
    component = (uint32)(sint32)r_s16(owner + 0x12Cu);
    product = component * r_u32(profile + 8u);
    w_u32(effect + 0x28u, (uint32)((sint32)product >> 12));
    x = r_u32(effect + 0x20u);
    product = x * r_u32(profile + 12u);
    y = r_u32(effect + 0x24u);
    w_u16(effect + 0x36u, (uint16)product);
    product = y * r_u32(profile + 12u);
    z = r_u32(effect + 0x28u);
    w_u16(effect + 0x38u, (uint16)product);
    product = z * r_u32(profile + 12u);
    w_u16(effect + 0x3Au, (uint16)product);
    return effect;
}

uint32 game_allocate_effect(uint32 type, uint32 owner)
{
    uint32 effect, next, head, table, value, identifier, removed;
    sint32 category;
    FUNCTION_MARKER(0x800B01CC, "GAME.OVL");
    effect = r_u32(0x800FB408u);
    if (effect != 0u) {
        next = r_u32(effect + 0x48u);
        head = r_u32(0x80124928u);
        w_u32(0x800FB408u, next);
        w_u32(next + 0x4Cu, 0u);
        w_u32(effect + 0x48u, head);
        w_u32(effect + 0x4Cu, 0u);
        if (head != 0u)
            w_u32(head + 0x4Cu, effect);
        table = 0x800F3370u + type * 4u;
        w_u16(effect + 0x3Eu, (uint16)type);
        value = r_u32(table);
        w_u32(effect + 0x54u, 0u);
        w_u32(effect + 0x50u, value);
        value = r_u32(table);
        w_u32(0x80124928u, effect);
        if (value != 0u)
            w_u32(value + 0x54u, effect);
        w_u32(table, effect);
        value = r_u32(0x800EF110u);
        w_u16(effect + 0x40u, 2800u);
        w_u32(effect + 0x60u, 0x8002493Cu);
        w_u16(effect + 0x46u, 1u);
        w_u16(effect + 0x34u, 2u);
        w_u32(effect + 0x6Cu, 0u);
        w_u32(0x800EF110u, value + 1u);
        if (owner != 0u) {
            w_u32(effect + 0x58u, owner);
            identifier = r_u16(owner + 0x24Au);
            w_u16(effect + 0x5Cu, (uint16)identifier);
            value = r_u32(owner);
            w_u16(effect + 0x64u, (uint16)value);
        } else {
            w_u32(effect + 0x58u, 0u);
            w_u16(effect + 0x5Cu, 0u);
            w_u16(effect + 0x64u, 0u);
        }
    } else {
        effect = r_u32(0x80124928u);
        removed = 0u;
        while (effect != 0u) {
            category = r_s16(effect + 0x3Cu);
            if (category == 107 && r_s32(effect + 0x2Cu) < 60) {
                value = r_u32(0x800595CCu);
                ++removed;
                w_u32(0x800595CCu, value - 1u);
                game_mark_effect_for_removal(effect);
                category = r_s16(effect + 0x3Cu);
            }
            value = r_u32(0x800EF158u);
            if ((uint32)category == value && r_s32(effect + 0x2Cu) < 30) {
                game_mark_effect_for_removal(effect);
                ++removed;
            }
            effect = r_u32(effect + 0x48u);
        }
        if ((removed << 16) == 0u)
            diagnostic_report(123u, 0x80075EC4u, 0x80075EDCu);
        game_process_effect_removals();
        effect = game_allocate_effect(type, owner);
    }
    w_u16(effect + 0x66u, 0u);
    w_u32(effect + 0x70u, 0u);
    w_u16(effect + 0x74u, 0u);
    return effect;
}

void game_mark_effect_for_removal(uint32 effect)
{
    uint32 count;
    FUNCTION_MARKER(0x800B1478, "GAME.OVL");
    if (r_s16(effect + 0x3Cu) == 0)
        return;
    count = r_u32(0x800EF114u);
    w_u16(effect + 0x3Cu, 0u);
    w_u32(0x801263C0u + count * 4u, effect);
    w_u32(0x800EF114u, count + 1u);
}

void game_unlink_effect_type(uint32 effect)
{
    uint32 previous, next, type;
    FUNCTION_MARKER(0x800B13DC, "GAME.OVL");
    previous = r_u32(effect + 0x54u);
    if (previous != 0u) {
        next = r_u32(effect + 0x50u);
        if (next != 0u) {
            w_u32(previous + 0x50u, next);
            next = r_u32(effect + 0x50u);
            previous = r_u32(effect + 0x54u);
            w_u32(next + 0x54u, previous);
        } else {
            w_u32(previous + 0x50u, 0u);
        }
    } else {
        next = r_u32(effect + 0x50u);
        if (next != 0u) {
            w_u32(next + 0x54u, 0u);
            type = (uint32)(sint32)r_s16(effect + 0x3Eu);
            next = r_u32(effect + 0x50u);
            w_u32(0x800F3370u + type * 4u, next);
        } else {
            type = (uint32)(sint32)r_s16(effect + 0x3Eu);
            w_u32(0x800F3370u + type * 4u, 0u);
        }
    }
    w_u32(effect + 0x50u, 0u);
    w_u32(effect + 0x54u, 0u);
}

void game_process_effect_removals(void)
{
    uint32 index, effect, allocation, previous, next, count, head;
    FUNCTION_MARKER(0x800B12A4, "GAME.OVL");
    count = r_u32(0x800EF114u);
    if ((sint32)count <= 0)
        return;
    index = 0u;
    do {
        effect = r_u32(0x801263C0u + (index & 65535u) * 4u);
        allocation = r_u32(effect + 0x6Cu);
        if (allocation != 0u)
            heap_free(allocation);
        previous = r_u32(effect + 0x4Cu);
        if (previous != 0u) {
            next = r_u32(effect + 0x48u);
            if (next != 0u) {
                w_u32(previous + 0x48u, next);
                next = r_u32(effect + 0x48u);
                previous = r_u32(effect + 0x4Cu);
                w_u32(next + 0x4Cu, previous);
            } else {
                w_u32(previous + 0x48u, 0u);
            }
        } else {
            next = r_u32(effect + 0x48u);
            if (next != 0u) {
                w_u32(next + 0x4Cu, 0u);
                w_u32(0x80124928u, r_u32(effect + 0x48u));
            } else {
                w_u32(0x80124928u, 0u);
            }
        }
        w_u32(0x800EF110u, r_u32(0x800EF110u) - 1u);
        game_unlink_effect_type(effect);
        head = r_u32(0x800FB408u);
        count = r_u32(0x800EF114u);
        ++index;
        w_u32(0x800FB408u, effect);
        w_u32(effect + 0x48u, head);
        w_u32(head + 0x4Cu, effect);
    } while ((sint32)(index & 65535u) < (sint32)count);
}


void game_fire_weapon_profile_12(uint32 slot)
{
    uint32 record, value, rate, region, limit;
    FUNCTION_MARKER(0x8007E354, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    if (r_u32(record + 24u) != 0u) {
        value = r_u32(record + 28u);
        rate = r_u32(record + 12u);
        region = r_u32(0x800311C0u);
        value += rate;
        w_u32(record + 28u, value);
        if (region != 0u && r_u32(0x800F2B68u) != 0u)
            w_u32(record + 28u, value + r_u32(record + 12u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)limit < (sint32)value)
            w_u32(record + 28u, limit);
    }
    if (r_u32(record + 32u) == 0u) {
        game_emit_weapon_profile_12();
        if (r_u32(record + 24u) != 0u)
            w_u32(record + 32u, r_u32(record + 4u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)value >= (sint32)limit)
            w_u32(record + 32u, r_u32(record + 8u));
    }
    if (r_u32(record + 24u) == 0u) {
        value = r_u32(record + 4u);
        w_u32(record + 24u, 1u);
        w_u32(record + 32u, value);
    }
}


void game_fire_weapon_profile_18(uint32 slot)
{
    uint32 record, value, rate, region, limit;
    FUNCTION_MARKER(0x8007E594, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    if (r_u32(record + 24u) != 0u) {
        value = r_u32(record + 28u);
        rate = r_u32(record + 12u);
        region = r_u32(0x800311C0u);
        value += rate;
        w_u32(record + 28u, value);
        if (region != 0u && r_u32(0x800F2B68u) != 0u)
            w_u32(record + 28u, value + r_u32(record + 12u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)limit < (sint32)value)
            w_u32(record + 28u, limit);
    }
    if (r_u32(record + 32u) == 0u) {
        game_emit_weapon_profile_18();
        if (r_u32(record + 24u) != 0u)
            w_u32(record + 32u, r_u32(record + 4u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)value >= (sint32)limit)
            w_u32(record + 32u, r_u32(record + 8u));
    }
    if (r_u32(record + 24u) == 0u) {
        value = r_u32(record + 4u);
        w_u32(record + 24u, 1u);
        w_u32(record + 32u, value);
    }
}


void game_fire_weapon_profile_10(uint32 slot)
{
    uint32 record, value, rate, region, limit;
    FUNCTION_MARKER(0x8007E6B4, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    if (r_u32(record + 24u) != 0u) {
        value = r_u32(record + 28u);
        rate = r_u32(record + 12u);
        region = r_u32(0x800311C0u);
        value += rate;
        w_u32(record + 28u, value);
        if (region != 0u && r_u32(0x800F2B68u) != 0u)
            w_u32(record + 28u, value + r_u32(record + 12u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)limit < (sint32)value)
            w_u32(record + 28u, limit);
    }
    if (r_u32(record + 32u) == 0u) {
        game_emit_weapon_profile_10();
        if (r_u32(record + 24u) != 0u)
            w_u32(record + 32u, r_u32(record + 4u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)value >= (sint32)limit)
            w_u32(record + 32u, r_u32(record + 8u));
    }
    if (r_u32(record + 24u) == 0u) {
        value = r_u32(record + 4u);
        w_u32(record + 24u, 1u);
        w_u32(record + 32u, value);
    }
}


void game_fire_weapon_profile_1a(uint32 slot)
{
    uint32 record, value, rate, region, limit;
    FUNCTION_MARKER(0x8007E7D4, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    if (r_u32(record + 24u) != 0u) {
        value = r_u32(record + 28u);
        rate = r_u32(record + 12u);
        region = r_u32(0x800311C0u);
        value += rate;
        w_u32(record + 28u, value);
        if (region != 0u && r_u32(0x800F2B68u) != 0u)
            w_u32(record + 28u, value + r_u32(record + 12u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)limit < (sint32)value)
            w_u32(record + 28u, limit);
    }
    if (r_u32(record + 32u) == 0u) {
        game_emit_weapon_profile_1a();
        if (r_u32(record + 24u) != 0u)
            w_u32(record + 32u, r_u32(record + 4u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)value >= (sint32)limit)
            w_u32(record + 32u, r_u32(record + 8u));
    }
    if (r_u32(record + 24u) == 0u) {
        value = r_u32(record + 4u);
        w_u32(record + 24u, 1u);
        w_u32(record + 32u, value);
    }
}


void game_fire_weapon_profile_14(uint32 slot)
{
    uint32 record, value, rate, region, limit;
    FUNCTION_MARKER(0x8007E8F4, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    if (r_u32(record + 24u) != 0u) {
        value = r_u32(record + 28u);
        rate = r_u32(record + 12u);
        region = r_u32(0x800311C0u);
        value += rate;
        w_u32(record + 28u, value);
        if (region != 0u && r_u32(0x800F2B68u) != 0u)
            w_u32(record + 28u, value + r_u32(record + 12u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)limit < (sint32)value)
            w_u32(record + 28u, limit);
    }
    if (r_u32(record + 32u) == 0u) {
        game_emit_weapon_profile_14();
        if (r_u32(record + 24u) != 0u)
            w_u32(record + 32u, r_u32(record + 4u));
        value = r_u32(record + 28u);
        limit = r_u32(record + 20u);
        if ((sint32)value >= (sint32)limit)
            w_u32(record + 32u, r_u32(record + 8u));
    }
    if (r_u32(record + 24u) == 0u) {
        value = r_u32(record + 4u);
        w_u32(record + 24u, 1u);
        w_u32(record + 32u, value);
    }
}

void game_emit_weapon_profile_12(void)
{
    uint32 selected, state, profile, offset, player, center = 0u, first, last, volume;
    sint32 mode, sample;
    FUNCTION_MARKER(0x8007F0CC, "GAME.OVL");
    selected = r_u32(0x800ECBD0u);
    state = r_u32(0x800313DCu);
    profile = r_u32(0x800ECBF8u + selected * 44u);
    mode = r_s16(profile + 0x12u);
    offset = state != 3u ? 8u : 0u;
    if (mode != 0) {
        if (mode == 1 || mode == 3) {
            player = r_u32(0x80040FDCu);
            center = game_create_profile_12_effect(player, player, 0x800ECE48u + offset);
        }
        if (mode != 1) {
            player = r_u32(0x80040FDCu);
            first = game_create_profile_12_effect(player, player, 0x800ECE08u + offset);
            player = r_u32(0x80040FDCu);
            last = game_create_profile_12_effect(player, player, 0x800ECE18u + offset);
            if (mode == 2)
                w_u32(first + 0x70u, last);
            if (mode == 3)
                w_u32(center + 0x70u, last);
        }
    }
    w_u8(0x800ECE98u, 64u);
    w_u8(0x800ECE99u, 16u);
    sample = r_s32(0x800429C0u);
    volume = r_u16(0x800118FCu);
    w_u8(0x800ECE9Au, 0u);
    sound_start_voice_at(sample, (volume >> 3) & 255u, 0, 0, 128u, 0u, 20u);
}

void game_emit_weapon_profile_10(void)
{
    uint32 selected, state, profile, offset, player, center = 0u, first, last, volume;
    sint32 mode, sample;
    FUNCTION_MARKER(0x8007F490, "GAME.OVL");
    selected = r_u32(0x800ECBD0u);
    state = r_u32(0x800313DCu);
    profile = r_u32(0x800ECBF8u + selected * 44u);
    mode = r_s16(profile + 0x10u);
    offset = state != 3u ? 8u : 0u;
    if (mode != 0) {
        if (mode == 1 || mode == 3) {
            player = r_u32(0x80040FDCu);
            center = game_create_profile_10_effect(player, player, 0x800ECE48u + offset);
        }
        if (mode != 1) {
            player = r_u32(0x80040FDCu);
            first = game_create_profile_10_effect(player, player, 0x800ECE08u + offset);
            player = r_u32(0x80040FDCu);
            last = game_create_profile_10_effect(player, player, 0x800ECE18u + offset);
            if (mode == 2)
                w_u32(first + 0x70u, last);
            if (mode == 3)
                w_u32(center + 0x70u, last);
        }
    }
    sample = r_s32(0x800429CCu);
    volume = r_u16(0x800118FCu);
    sound_start_voice_at(sample, (volume >> 2) & 255u, 0, 0, 64u, 0u, 20u);
    w_u8(0x800ECE98u, 32u);
    w_u8(0x800ECE99u, 10u);
    w_u8(0x800ECE9Au, 64u);
}

void game_emit_weapon_profile_14(void)
{
    uint32 selected, state, profile, offset, player, center = 0u, first, last, volume;
    sint32 mode, sample;
    FUNCTION_MARKER(0x8007F744, "GAME.OVL");
    selected = r_u32(0x800ECBD0u);
    state = r_u32(0x800313DCu);
    profile = r_u32(0x800ECBF8u + selected * 44u);
    mode = r_s16(profile + 0x14u);
    offset = state != 3u ? 8u : 0u;
    if (mode != 0) {
        if (mode == 1 || mode == 3) {
            player = r_u32(0x80040FDCu);
            center = game_create_profile_14_effect(player, player, 0x800ECE48u + offset);
        }
        if (mode != 1) {
            player = r_u32(0x80040FDCu);
            first = game_create_profile_14_effect(player, player, 0x800ECE08u + offset);
            player = r_u32(0x80040FDCu);
            last = game_create_profile_14_effect(player, player, 0x800ECE18u + offset);
            if (mode == 2)
                w_u32(first + 0x70u, last);
            if (mode == 3)
                w_u32(center + 0x70u, last);
        }
    }
    sample = r_s32(0x800429C0u);
    volume = r_u16(0x800118FCu);
    sound_start_voice_at(sample, (volume >> 3) & 255u, 0, 0, 80u, 0u, 20u);
    w_u8(0x800ECE99u, 16u);
    w_u8(0x800ECE98u, 0u);
    w_u8(0x800ECE9Au, 80u);
}

void game_emit_weapon_profile_18(void)
{
    uint32 selected, state, profile, offset, player, volume;
    sint32 mode, sample;
    FUNCTION_MARKER(0x8007F2FC, "GAME.OVL");
    selected = r_u32(0x800ECBD0u);
    state = r_u32(0x800313DCu);
    profile = r_u32(0x800ECBF8u + selected * 44u);
    mode = r_s16(profile + 0x18u);
    offset = state != 3u ? 8u : 0u;
    if (mode != 0) {
        if (mode == 1 || mode == 3) {
            player = r_u32(0x80040FDCu);
            game_create_profile_18_effect(player, player, 0x800ECE48u + offset);
        }
        if (mode != 1) {
            player = r_u32(0x80040FDCu);
            game_create_profile_18_effect(player, player, 0x800ECE08u + offset);
            player = r_u32(0x80040FDCu);
            game_create_profile_18_effect(player, player, 0x800ECE18u + offset);
        }
        if (mode == 4) {
            player = r_u32(0x80040FDCu);
            game_create_profile_18_effect(player, player, 0x800ECE28u + offset);
            player = r_u32(0x80040FDCu);
            game_create_profile_18_effect(player, player, 0x800ECE38u + offset);
        }
    }
    sample = r_s32(0x800429BCu);
    volume = r_u16(0x800118FCu);
    sound_start_voice_at(sample, (volume >> 2) & 255u, 0, 0, 256u, 0u, 20u);
    w_u8(0x800ECE98u, 64u);
    w_u8(0x800ECE99u, 16u);
    w_u8(0x800ECE9Au, 0u);
}

void game_emit_weapon_profile_1a(void)
{
    uint32 state, selected, profile, offset, player, center = 0u, first, last;
    sint32 mode;
    FUNCTION_MARKER(0x8007F60C, "GAME.OVL");
    state = r_u32(0x800313DCu);
    selected = r_u32(0x800ECBD0u);
    profile = r_u32(0x800ECBF8u + selected * 44u);
    mode = r_s16(profile + 0x1Au);
    offset = state != 3u ? 8u : 0u;
    if (mode == 1 || mode == 3) {
        player = r_u32(0x80040FDCu);
        center = game_create_profile_1a_effect(player, player, 0x800ECE48u + offset);
    }
    if (mode != 1) {
        player = r_u32(0x80040FDCu);
        first = game_create_profile_1a_effect(player, player, 0x800ECE08u + offset);
        player = r_u32(0x80040FDCu);
        last = game_create_profile_1a_effect(player, player, 0x800ECE18u + offset);
        if (mode == 2)
            w_u32(first + 0x70u, last);
        if (mode == 3)
            w_u32(center + 0x70u, last);
    }
    w_u8(0x800ECE99u, 16u);
    w_u8(0x800ECE98u, 0u);
    w_u8(0x800ECE9Au, 64u);
}

uint32 game_create_profile_12_effect(uint32 owner, uint32 target, uint32 offset)
{
    uint32 effect, x, y, z, vx, vy, vz;
    FUNCTION_MARKER(0x800B1A84, "GAME.OVL");
    effect = game_create_profile_effect(owner, target, offset, 0x800EF17Cu);
    x = r_u32(effect);
    vx = r_u32(effect + 0x20u);
    w_u32(effect + 0x14u, 0x800EF1F4u);
    w_u32(effect + 0x1Cu, 2u);
    w_u32(effect + 0x68u, 0x800EEC9Cu);
    y = r_u32(effect + 4u);
    vz = r_u32(effect + 0x28u);
    vy = r_u32(effect + 0x24u);
    w_u32(effect, x + vx);
    z = r_u32(effect + 8u);
    w_u32(effect + 4u, y + vy);
    w_u32(effect + 8u, z + vz);
    return effect;
}

uint32 game_create_profile_14_effect(uint32 owner, uint32 target, uint32 offset)
{
    uint32 effect, x, y, z, vx, vy, vz;
    FUNCTION_MARKER(0x800B1C84, "GAME.OVL");
    effect = game_create_profile_effect(owner, target, offset, 0x800EF1A4u);
    x = r_u32(effect);
    vx = r_u32(effect + 0x20u);
    w_u32(effect + 0x14u, 0x800EF274u);
    w_u32(effect + 0x1Cu, 2u);
    w_u32(effect + 0x68u, 0x800EECBCu);
    y = r_u32(effect + 4u);
    vz = r_u32(effect + 0x28u);
    vy = r_u32(effect + 0x24u);
    w_u32(effect, x + vx);
    z = r_u32(effect + 8u);
    w_u32(effect + 4u, y + vy);
    w_u32(effect + 8u, z + vz);
    return effect;
}

uint32 game_create_profile_18_effect(uint32 owner, uint32 target, uint32 offset)
{
    uint32 effect, random, next, vx, vy, vz, x, y, z;
    FUNCTION_MARKER(0x800B1B90, "GAME.OVL");
    effect = game_create_profile_effect(owner, target, offset, 0x800EF190u);
    random = r_u32(0x800EF50Cu);
    vx = r_u32(effect + 0x20u);
    vz = r_u32(effect + 0x28u);
    w_u32(effect + 0x14u, 0x800EF2F4u);
    w_u32(effect + 0x1Cu, 2u);
    w_u32(effect + 0x68u, 0x800EECCCu);
    next = (uint32)((sint32)random >> 1) ^ (random << 3);
    w_u32(effect + 0x20u, vx - 63u + (next & 127u));
    w_u32(0x800EF50Cu, next);
    random = (uint32)((sint32)next >> 1) ^ (next << 3);
    vy = r_u32(effect + 0x24u);
    vz -= 63u;
    w_u32(0x800EF50Cu, random);
    w_u32(effect + 0x24u, vy - 63u + (random & 127u));
    next = (uint32)((sint32)random >> 1) ^ (random << 3);
    w_u32(0x800EF50Cu, next);
    vz += next & 127u;
    x = r_u32(effect);
    vx = r_u32(effect + 0x20u);
    vy = r_u32(effect + 0x24u);
    w_u32(effect + 0x28u, vz);
    vz = r_u32(effect + 0x28u);
    w_u32(effect, x + vx);
    y = r_u32(effect + 4u);
    z = r_u32(effect + 8u);
    w_u32(effect + 4u, y + vy);
    w_u32(effect + 8u, z + vz);
    return effect;
}

uint32 game_create_profile_10_effect(uint32 owner, uint32 target, uint32 offset)
{
    uint32 effect, state, vx, vy, vz, velocity, x, y, z;
    FUNCTION_MARKER(0x800B1CF8, "GAME.OVL");
    effect = game_create_profile_effect(owner, target, offset, 0x800EF1CCu);
    if (effect == 0u)
        return 0u;
    state = r_u32(0x800311DCu);
    w_u32(effect + 0x14u, 0x800EF1F4u);
    w_u32(effect + 0x18u, 0u);
    if (state == 0u && r_s32(0x800313DCu) < 4) {
        game_draw_projected_ring(64u, r_u32(0x800311B0u), 96u, 0xFF8060u, 0u);
        game_draw_projected_ring(64u, r_u32(0x800311B0u), 128u, 0xFF0000u, 0u);
        game_draw_projected_ring(448u, r_u32(0x800311B0u), 96u, 0xFF8060u, 0u);
        game_draw_projected_ring(448u, r_u32(0x800311B0u), 128u, 0xFF0000u, 0u);
        state = 1u;
    } else {
        state = 3u;
    }
    w_u32(effect + 0x1Cu, state);
    vx = r_u32(effect + 0x20u);
    velocity = r_u32(owner + 0xFCu);
    vy = r_u32(effect + 0x24u);
    w_u32(effect + 0x20u, vx + velocity);
    velocity = r_u32(owner + 0x100u);
    w_u32(effect + 0x24u, vy + velocity);
    x = r_u32(effect);
    vx = r_u32(effect + 0x20u);
    velocity = r_u32(owner + 0x104u);
    w_u32(effect, x + vx);
    vz = r_u32(effect + 0x28u);
    w_u32(effect + 0x68u, 0x800EECACu);
    z = r_u32(effect + 8u);
    w_u32(effect + 0x28u, vz + velocity);
    y = r_u32(effect + 4u);
    vy = r_u32(effect + 0x24u);
    vz = r_u32(effect + 0x28u);
    w_u32(effect + 4u, y + vy);
    w_u32(effect + 8u, z + vz);
    return effect;
}

uint32 game_create_profile_1a_effect(uint32 owner, uint32 target, uint32 offset)
{
    uint32 effect, state, vx, vy, vz, velocity, x, y, z;
    FUNCTION_MARKER(0x800B1E68, "GAME.OVL");
    effect = game_create_profile_effect(owner, target, offset, 0x800EF12Cu);
    if (effect == 0u)
        return 0u;
    w_u32(effect + 12u, 0x40FF5A00u);
    state = r_u32(0x800311DCu);
    w_u32(effect + 16u, 0x40FFC800u);
    w_u32(effect + 0x68u, 0x800EECDCu);
    if (state == 0u && r_s32(0x800313DCu) < 4) {
        game_draw_projected_ring(64u, r_u32(0x800311B0u), 96u, 0xFF8060u, 0u);
        game_draw_projected_ring(64u, r_u32(0x800311B0u), 128u, 0xFF0000u, 0u);
        game_draw_projected_ring(448u, r_u32(0x800311B0u), 96u, 0xFF8060u, 0u);
        game_draw_projected_ring(448u, r_u32(0x800311B0u), 128u, 0xFF0000u, 0u);
        state = 1u;
    } else {
        state = 3u;
    }
    w_u32(effect + 0x1Cu, state);
    vx = r_u32(effect + 0x20u);
    velocity = r_u32(owner + 0xFCu);
    w_u32(effect + 0x20u, vx + velocity);
    vy = r_u32(effect + 0x24u);
    velocity = r_u32(owner + 0x100u);
    vx = r_u32(effect + 0x20u);
    vz = r_u32(effect + 0x28u);
    w_u32(effect + 0x24u, vy + velocity);
    x = r_u32(effect);
    velocity = r_u32(owner + 0x104u);
    w_u32(effect, x + vx);
    y = r_u32(effect + 4u);
    vy = r_u32(effect + 0x24u);
    w_u32(effect + 0x28u, vz + velocity);
    z = r_u32(effect + 8u);
    vz = r_u32(effect + 0x28u);
    w_u32(effect + 4u, y + vy);
    w_u32(effect + 8u, z + vz);
    return effect;
}

void game_fire_weapon_profile_02(uint32 slot)
{
    uint32 player, target, projectile;
    FUNCTION_MARKER(0x8007EA14, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    target = r_u32(0x800EF4E0u);
    w_u32(0x80119C50u + slot * 52u, 30u);
    projectile = game_create_weapon_projectile(player, target, 0x800ECDFCu, 1u);
    w_u32(projectile + 0x1E4u, 0x800EED0Cu);
    sound_play_sample_full(21u);
}

void game_fire_mission_weapon(uint32 slot)
{
    uint32 player, target, projectile;
    FUNCTION_MARKER(0x8007EA8C, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    target = r_u32(0x800EF4E0u);
    w_u32(0x80119C50u + slot * 52u, 30u);
    projectile = game_create_mission_projectile(player, target, 0x800ECDFCu, 3u);
    w_u32(projectile + 0x1E4u, 0x800EED3Cu);
    w_u32(projectile + 0x178u, 0u);
    sound_play_sample_full(21u);
}

void game_fire_weapon_profile_06(uint32 slot)
{
    uint32 record, player, target, projectile, mode;
    FUNCTION_MARKER(0x8007EB08, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    player = r_u32(0x80040FDCu);
    target = r_u32(0x800EF4E0u);
    mode = (uint32)(sint32)r_s16(record + 48u);
    w_u32(record + 32u, 30u);
    projectile = game_create_profile_06_projectile(player, target, 0x800ECDFCu, mode);
    w_u32(projectile + 0x1E4u, 0x800EED2Cu);
    sound_play_sample_full(21u);
}

void game_fire_weapon_profile_04(uint32 slot)
{
    uint32 player, target, projectile;
    FUNCTION_MARKER(0x8007EB80, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    target = r_u32(0x800EF4E0u);
    w_u32(0x80119C50u + slot * 52u, 30u);
    projectile = game_create_weapon_projectile(player, target, 0x800ECDFCu, 2u);
    w_u32(projectile + 0x1E4u, 0x800EED1Cu);
    sound_play_sample_full(21u);
}

void game_fire_weapon_profile_00(uint32 slot)
{
    uint32 player, target, projectile;
    FUNCTION_MARKER(0x8007EBF8, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    target = r_u32(0x800EF4E0u);
    w_u32(0x80119C50u + slot * 52u, 30u);
    projectile = game_create_weapon_projectile(player, target, 0x800ECDFCu, 0u);
    w_u32(projectile + 0x1E4u, 0x800EED4Cu);
    w_u32(projectile, 209u);
    w_u32(projectile + 0x3Cu, 0x800B37B0u);
    w_u32(projectile + 0x38u, 0x800B3EE8u);
    sound_play_sample_full(21u);
}

void game_fire_weapon_profile_0c(uint32 slot)
{
    uint32 player, projectile;
    FUNCTION_MARKER(0x8007EC90, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    w_u32(0x80119C50u + slot * 52u, 30u);
    projectile = game_create_profile_0c_projectile(player, 0x800ECDFCu);
    w_u32(projectile + 0x1E4u, 0x800EED9Cu);
    w_u32(projectile + 0x178u, 0u);
    sound_play_sample_full(21u);
}

void game_fire_weapon_profile_0e(uint32 slot)
{
    uint32 player, target, projectile, component, velocity, next_component;
    FUNCTION_MARKER(0x8007ED04, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    target = r_u32(0x800EF4E0u);
    w_u32(0x80119C50u + slot * 52u, 30u);
    projectile = game_create_weapon_projectile(player, target, 0x800ECDFCu, 0u);
    player = r_u32(0x80040FDCu);
    w_u32(projectile + 0x1E4u, 0x800EED5Cu);
    w_u32(projectile, 205u);
    w_u32(projectile + 0x3Cu, 0x800B37B0u);
    w_u32(projectile + 0x178u, 0u);
    w_u32(projectile + 0x38u, 0x800B3E14u);
    component = (uint32)(sint32)r_s16(player + 0x128u);
    velocity = r_u32(player + 0x14u) - (component << 6);
    next_component = (uint32)(sint32)r_s16(player + 0x12Au);
    w_u32(player + 0x14u, velocity);
    velocity = r_u32(player + 0x18u) - (next_component << 6);
    component = (uint32)(sint32)r_s16(player + 0x12Cu);
    w_u32(player + 0x18u, velocity);
    velocity = r_u32(player + 0x1Cu) - (component << 6);
    w_u32(player + 0x1Cu, velocity);
    sound_play_sample_full(21u);
}

void game_fire_weapon_profile_0a(uint32 slot)
{
    uint32 player, target, projectile, component, velocity, next_component;
    FUNCTION_MARKER(0x8007EDE4, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    target = r_u32(0x800EF4E0u);
    w_u32(0x80119C50u + slot * 52u, 30u);
    projectile = game_create_weapon_projectile(player, target, 0x800ECDFCu, 0u);
    player = r_u32(0x80040FDCu);
    w_u32(projectile + 0x1E4u, 0x800EED6Cu);
    w_u32(projectile, 206u);
    w_u32(projectile + 0x3Cu, 0x800B37B0u);
    w_u32(projectile + 0x178u, 0u);
    w_u32(projectile + 0x38u, 0x800B3C08u);
    component = (uint32)(sint32)r_s16(player + 0x128u);
    velocity = r_u32(player + 0x14u) - (component << 6);
    next_component = (uint32)(sint32)r_s16(player + 0x12Au);
    w_u32(player + 0x14u, velocity);
    velocity = r_u32(player + 0x18u) - (next_component << 6);
    component = (uint32)(sint32)r_s16(player + 0x12Cu);
    w_u32(player + 0x18u, velocity);
    velocity = r_u32(player + 0x1Cu) - (component << 6);
    w_u32(player + 0x1Cu, velocity);
    sound_play_sample_full(21u);
}

void game_fire_weapon_profile_20(uint32 slot)
{
    uint32 previous, target, flags, y, player, x, px, py, z, pz, distance;
    FUNCTION_MARKER(0x8007EECC, "GAME.OVL");
    previous = r_u32(0x800EF4E4u);
    if (previous != 0u) {
        flags = r_u32(previous + 0x114u);
        if ((flags & 0x40000u) != 0u)
            w_u32(previous + 0x114u, flags ^ 0x40000u);
    }
    target = r_u32(0x800EF4E0u);
    if ((r_u32(target + 0x114u) & 0x40000u) != 0u)
        return;
    flags = r_u32(target + 0x114u);
    y = r_u32(target + 12u);
    w_u32(0x800EF4E4u, target);
    w_u32(target + 0x114u, flags | 0x40000u);
    player = r_u32(0x80040FDCu);
    x = r_u32(target + 8u);
    px = r_u32(player + 8u);
    py = r_u32(player + 12u);
    z = r_u32(target + 16u);
    pz = r_u32(player + 16u);
    distance = (uint32)vector_length_approx((sint32)(x - px), (sint32)(y - py), (sint32)(z - pz));
    w_u32(0x800EF554u, distance);
    w_u32(0x80123F40u, distance);
}

uint32 game_create_profile_0c_projectile(uint32 owner, uint32 offset)
{
    uint32 type, kind, projectile, identifier, x, y, z;
    FUNCTION_MARKER(0x8007F8BC, "GAME.OVL");
    type = r_u32(owner);
    kind = type == 3u ? 4u : type == 1u ? 2u : 5u;
    projectile = game_create_object(0u, 0u, 0u, 0u, kind);
    if (offset != 0u) {
        game_position_at_object_offset(owner, offset, projectile);
    } else {
        w_u32(projectile + 8u, r_u32(owner + 8u));
        w_u32(projectile + 12u, r_u32(owner + 12u));
        w_u32(projectile + 16u, r_u32(owner + 16u));
    }
    object_extract_angles(projectile, owner);
    w_u32(projectile + 0x3Cu, 0x8002493Cu);
    w_u32(projectile, 207u);
    w_u32(projectile + 0x160u, owner);
    identifier = r_u16(owner + 0x24Au);
    w_u32(projectile + 0xECu, 1600u);
    w_u32(projectile + 0x9Cu, 40u);
    w_u32(projectile + 0x40u, 0x800807D8u);
    w_u32(projectile + 0x5Cu, 0x80080828u);
    w_u32(projectile + 0x48u, 0x8008132Cu);
    w_u16(projectile + 0x164u, (uint16)identifier);
    game_get_object_forward(owner, projectile + 0xFCu);
    x = r_u32(projectile + 0xFCu);
    z = r_u32(projectile + 0x104u);
    w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 3));
    y = r_u32(projectile + 0x100u);
    w_u32(projectile + 0x104u, (uint32)((sint32)z >> 3));
    z = r_u32(projectile + 0x104u);
    w_u32(projectile + 0x100u, (uint32)((sint32)y >> 3));
    x = r_u32(projectile + 0xFCu);
    y = r_u32(projectile + 0x100u);
    w_u32(projectile + 0x1Cu, z);
    w_u32(projectile + 0x14u, x);
    w_u32(projectile + 0x18u, y);
    game_attach_object_trail(projectile, 1u);
    return projectile;
}

uint32 game_create_mission_projectile(uint32 owner, uint32 target, uint32 offset, uint32 mode)
{
    uint32 type, kind, projectile, identifier, x, y, z;
    FUNCTION_MARKER(0x8007FC04, "GAME.OVL");
    type = r_u32(owner);
    kind = type == 3u ? 4u : type == 1u ? 2u : 5u;
    projectile = game_create_object(0u, 0u, 0u, 0u, kind);
    w_u32(projectile + 0x1E4u, 0x800EEBCCu);
    if (offset != 0u) {
        game_position_at_object_offset(owner, offset, projectile);
    } else {
        w_u32(projectile + 8u, r_u32(owner + 8u));
        w_u32(projectile + 12u, r_u32(owner + 12u));
        w_u32(projectile + 16u, r_u32(owner + 16u));
    }
    object_extract_angles(projectile, owner);
    w_u32(projectile + 0x3Cu, 0x8002493Cu);
    w_u32(projectile, 211u);
    w_u32(projectile + 0x160u, owner);
    identifier = r_u16(owner + 0x24Au);
    w_u32(projectile + 0x178u, target);
    w_u16(projectile + 0x164u, (uint16)identifier);
    identifier = r_u16(target + 0x24Au);
    w_u32(projectile + 0xECu, 800u);
    w_u32(projectile + 0x9Cu, 30u);
    w_u32(projectile + 0x40u, 0x8008068Cu);
    w_u32(projectile + 0x5Cu, 0x8008146Cu);
    w_u32(projectile + 0x48u, 0x8008157Cu);
    w_u16(projectile + 0x17Cu, (uint16)identifier);
    game_get_object_forward(owner, projectile + 0xFCu);
    x = r_u32(projectile + 0xFCu);
    z = r_u32(projectile + 0x104u);
    w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 3));
    y = r_u32(projectile + 0x100u);
    w_u32(projectile + 0x104u, (uint32)((sint32)z >> 3));
    z = r_u32(projectile + 0x104u);
    w_u32(projectile + 0x100u, (uint32)((sint32)y >> 3));
    x = r_u32(projectile + 0xFCu);
    y = r_u32(projectile + 0x100u);
    w_u32(projectile + 0x1Cu, z);
    w_u32(projectile + 0x14u, x);
    w_u32(projectile + 0x18u, y);
    if (target != 0u && r_u32(target) != 0u) {
        w_u32(target + 0x1A0u, projectile);
        identifier = r_u16(projectile + 0x24Au);
        w_u16(target + 0x1A4u, (uint16)identifier);
    }
    game_attach_object_trail(projectile, mode);
    return projectile;
}

uint32 game_create_profile_06_projectile(uint32 owner, uint32 target, uint32 offset, uint32 mode)
{
    uint32 type, kind, projectile, identifier, x, y, z, player, row, column, source_offset, destination_offset;
    FUNCTION_MARKER(0x8007FF84, "GAME.OVL");
    type = r_u32(owner);
    kind = type == 3u ? 4u : type == 1u ? 2u : 5u;
    projectile = game_create_object(0u, 0u, 0u, 0u, kind);
    w_u32(projectile + 0x1E4u, 0x800EEBBCu);
    if (offset != 0u) {
        game_position_at_object_offset(owner, offset, projectile);
    } else {
        w_u32(projectile + 8u, r_u32(owner + 8u));
        w_u32(projectile + 12u, r_u32(owner + 12u));
        w_u32(projectile + 16u, r_u32(owner + 16u));
    }
    object_extract_angles(projectile, owner);
    w_u32(projectile + 0x3Cu, 0x800B37B0u);
    w_u32(projectile + 0x38u, 0x800B37D0u);
    w_u32(projectile, 210u);
    w_u32(projectile + 0x160u, owner);
    identifier = r_u16(owner + 0x24Au);
    w_u32(projectile + 0x178u, target);
    w_u16(projectile + 0x164u, (uint16)identifier);
    identifier = r_u16(target + 0x24Au);
    w_u32(projectile + 0xECu, 400u);
    w_u32(projectile + 0x9Cu, 120u);
    w_u32(projectile + 0x40u, 0x80080EF4u);
    w_u32(projectile + 0x5Cu, 0x8008132Cu);
    w_u32(projectile + 0x48u, 0x8008132Cu);
    w_u16(projectile + 0x17Cu, (uint16)identifier);
    game_get_object_forward(owner, projectile + 0xFCu);
    x = r_u32(projectile + 0xFCu);
    z = r_u32(projectile + 0x104u);
    w_u32(projectile + 0xFCu, (uint32)((sint32)x >> 3));
    y = r_u32(projectile + 0x100u);
    w_u32(projectile + 0x104u, (uint32)((sint32)z >> 3));
    z = r_u32(projectile + 0x104u);
    w_u32(projectile + 0x100u, (uint32)((sint32)y >> 3));
    x = r_u32(projectile + 0xFCu);
    y = r_u32(projectile + 0x100u);
    w_u32(projectile + 0x1Cu, z);
    w_u32(projectile + 0x14u, x);
    w_u32(projectile + 0x18u, y);
    if (target != 0u && r_u32(target) != 0u) {
        w_u32(target + 0x1A0u, projectile);
        identifier = r_u16(projectile + 0x24Au);
        w_u16(target + 0x1A4u, (uint16)identifier);
    }
    player = r_u32(0x80040FDCu);
    for (row = 0u; row < 3u; ++row) {
        for (column = 0u; column < 3u; ++column) {
            destination_offset = column * 6u + row * 2u;
            source_offset = owner == player ? row * 6u + column * 2u : destination_offset;
            identifier = r_u16(owner + 0x11Cu + source_offset);
            w_u16(projectile + 0x11Cu + destination_offset, (uint16)identifier);
        }
    }
    return projectile;
}

void game_detach_object_trails(uint32 object)
{
    uint32 slot, offset = 0u;
    FUNCTION_MARKER(0x800B3764, "GAME.OVL");
    for (slot = 0u; slot < 20u; ++slot, offset += 300u) {
        if (r_u32(0x80124D70u + offset) == object)
            w_u32(0x80124D68u + offset, 0u);
    }
}

void game_destroy_weapon_projectile(uint32 projectile)
{
    uint32 target, identifier, x, y, z;
    sint32 count;
    FUNCTION_MARKER(0x8008132C, "GAME.OVL");
    if (r_s32(projectile + 0x9Cu) > 0) {
        count = random_between(6, 12);
        game_emit_projectile_fragments(projectile + 8u, projectile, (sint16)count, 0u);
        sound_play_distant_at_camera(projectile + 8u, 4u, 192u);
        x = r_u32(projectile + 8u);
        y = r_u32(projectile + 12u);
        z = r_u32(projectile + 16u);
        game_emit_projectile_impact(x, y, z);
    }
    target = r_u32(projectile + 0x178u);
    if (target != 0u) {
        identifier = r_u16(projectile + 0x17Cu);
        if (identifier == r_u16(target + 0x24Au) && r_u32(target) != 0u
            && r_u32(target + 0x1A0u) == projectile)
            w_u32(target + 0x1A0u, 0u);
    }
    game_detach_object_trails(projectile);
    game_mark_object_for_removal(projectile);
}

void game_destroy_mission_projectile(uint32 projectile)
{
    FUNCTION_MARKER(0x8008157C, "GAME.OVL");
    game_detach_object_trails(projectile);
    game_mark_object_for_removal(projectile);
}

void game_emit_projectile_impact(uint32 x, uint32 y, uint32 z)
{
    uint32 index, record;
    FUNCTION_MARKER(0x800BCB48, "GAME.OVL");
    index = r_u32(0x800EF504u);
    record = 0x801077D0u + index * 48u;
    ++index;
    w_u32(record, x);
    w_u32(record + 40u, 13u);
    w_u32(record + 20u, 128u);
    w_u32(record + 4u, y);
    w_u32(record + 8u, z);
    w_u32(record + 16u, 0u);
    w_u32(record + 36u, 255u);
    w_u32(record + 24u, 255u);
    w_u32(record + 28u, 255u);
    w_u32(record + 32u, 255u);
    w_u32(record + 44u, 3u);
    w_u32(0x800EF504u, (uint32)((sint32)index % 10));
}


void game_emit_reflected_particles(uint32 count, uint32 x, uint32 y, uint32 z, uint32 vx, uint32 vy, uint32 vz)
{
    uint32 index, offset, record, base, random, next, value, step_x, step_y, step_z;
    FUNCTION_MARKER(0x800AFAB8, "GAME.OVL");
    if ((sint32)count <= 0)
        return;
    step_x = (uint32)((sint32)vx >> 4);
    step_y = (uint32)((sint32)vy >> 4);
    step_z = (uint32)((sint32)vz >> 4);
    do {
        --count;
        index = r_u32(0x800EF10Cu);
        base = r_u32(0x80123F64u);
        random = r_u32(0x800EF50Cu);
        offset = index * 28u;
        record = base + offset;
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        value = step_x + (next & 127u) - 64u;
        w_u32(0x800EF50Cu, next);
        random = (uint32)((sint32)next >> 1) ^ (next << 3);
        w_u16(record + 2u, (uint16)value);
        value = step_y + (random & 127u) - 64u;
        ++index;
        w_u32(0x800EF50Cu, random);
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        w_u16(record + 4u, (uint16)value);
        value = step_z + (next & 127u) - 64u;
        w_u32(0x800EF50Cu, next);
        random = (uint32)((sint32)next >> 1) ^ (next << 3);
        w_u16(record + 6u, (uint16)value);
        w_u32(0x800EF50Cu, random);
        index = (uint32)((sint32)index % 450);
        w_u32(0x800EF10Cu, index);
        w_u8(record, 2u);
        base = r_u32(0x80123F64u);
        w_u32(record + 8u, x);
        w_u32(record + 12u, y);
        w_u32(record + 16u, z);
        w_u32(record + 24u, random & 63u);
        w_u8(base + offset + 1u, 0u);
    } while ((sint32)count > 0);
}

sint32 game_reflect_vector_native(const VECTOR *input, const VECTOR *normal, VECTOR *output)
{
    VECTOR reflected;
    uint32 normal_x, x, product_x, product_y, product_z, value, component, doubled;
    FUNCTION_MARKER(0x80097C70, "GAME.OVL");
    normal_x = (uint32)normal->vx;
    x = (uint32)input->vx;
    product_x = normal_x * x;
    value = (uint32)normal->vy;
    component = (uint32)input->vy;
    product_y = value * component;
    value = (uint32)normal->vz;
    component = (uint32)input->vz;
    product_z = value * component;
    doubled = ((uint32)((sint32)product_x >> 12) + (uint32)((sint32)product_y >> 12)
        + (uint32)((sint32)product_z >> 12)) << 1;
    value = doubled * normal_x;
    reflected.vx = (sint32)(x - (uint32)((sint32)value >> 12));
    value = doubled * (uint32)normal->vy;
    component = (uint32)input->vy;
    reflected.vy = (sint32)(component - (uint32)((sint32)value >> 12));
    value = doubled * (uint32)normal->vz;
    component = (uint32)input->vz;
    reflected.vz = (sint32)(component - (uint32)((sint32)value >> 12));
    return vector_normalize(&reflected, output);
}

static void game_emit_projectile_fragments_body(uint32 position, const VECTOR *native_position, uint32 object, sint32 count, uint32 mode, VECTOR *native_mode)
{
    VECTOR rotated, normalized, reflected;
    uint32 x, y, z, vx, vy, vz, component, listener;
    count = (sint16)count;
    if (count != 0) {
        if ((r_u32(object + 0x114u) & 0x10u) != 0u && (mode != 0u || native_mode != NULL)) {
            ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u),
                (VECTOR *)psx_addr(0x801079B0u, 12u), &rotated);
            w_u32(0x801079B0u, (uint32)rotated.vx);
            w_u32(0x801079B4u, (uint32)rotated.vy);
            w_u32(0x801079B8u, (uint32)rotated.vz);
            vector_normalize(native_mode != NULL ? native_mode : (VECTOR *)psx_addr(mode, 12u), &normalized);
            game_reflect_vector_native(&normalized, (const VECTOR *)psx_addr(0x801079B0u, 12u), &reflected);
            x = (native_position != NULL ? (uint32)native_position->vx : r_u32(position));
            y = (native_position != NULL ? (uint32)native_position->vy : r_u32(position + 4u));
            z = (native_position != NULL ? (uint32)native_position->vz : r_u32(position + 8u));
            game_emit_reflected_particles((uint32)count, x, y, z,
                (uint32)reflected.vx, (uint32)reflected.vy, (uint32)reflected.vz);
        } else {
            vx = (native_position != NULL ? (uint32)native_position->vx : r_u32(position));
            component = r_u32(object + 8u);
            vx -= component;
            vy = (native_position != NULL ? (uint32)native_position->vy : r_u32(position + 4u));
            component = r_u32(object + 12u);
            vy -= component;
            vz = (native_position != NULL ? (uint32)native_position->vz : r_u32(position + 8u));
            component = r_u32(object + 16u);
            vz -= component;
            x = (native_position != NULL ? (uint32)native_position->vx : r_u32(position));
            y = (native_position != NULL ? (uint32)native_position->vy : r_u32(position + 4u));
            z = (native_position != NULL ? (uint32)native_position->vz : r_u32(position + 8u));
            game_emit_narrow_particles((uint32)count, x, y, z, vx, vy, vz);
        }
    }
    x = (native_position != NULL ? (uint32)native_position->vx : r_u32(position));
    y = (native_position != NULL ? (uint32)native_position->vy : r_u32(position + 4u));
    z = (native_position != NULL ? (uint32)native_position->vz : r_u32(position + 8u));
    game_emit_fragment_flash(x, y, z);
    listener = r_u32(0x80040C94u);
    if (native_position != NULL)
        sound_play_at_listener_native(native_position, 5u, 0u, listener);
    else
        sound_play_at_listener(position, 5u, 0u, listener);
}

void game_emit_projectile_fragments(uint32 position, uint32 object, sint32 count, uint32 mode)
{
    FUNCTION_MARKER(0x800A2E08, "GAME.OVL");
    game_emit_projectile_fragments_body(position, NULL, object, count, mode, NULL);
}

void game_emit_projectile_fragments_native(const VECTOR *position, uint32 object, sint32 count, VECTOR *mode)
{
    game_emit_projectile_fragments_body(0, position, object, count, 0, mode);
}

uint32 game_allocate_flash(uint32 type)
{
    uint32 effect, next, head, table, value, count, other_count;
    sint32 minimum = 9999, time;
    FUNCTION_MARKER(0x800BB9DC, "GAME.OVL");
    effect = r_u32(0x801094B0u);
    if (effect != 0u) {
        next = r_u32(effect + 0x10u);
        head = r_u32(0x80108A48u);
        w_u32(0x801094B0u, next);
        w_u32(next + 0x14u, 0u);
        w_u32(effect + 0x10u, head);
        w_u32(effect + 0x14u, 0u);
        if (head != 0u)
            w_u32(head + 0x14u, effect);
        table = 0x800FB3E0u + type * 4u;
        w_u16(effect + 0x2Au, (uint16)type);
        value = r_u32(table);
        w_u32(effect + 0x1Cu, 0u);
        w_u32(effect + 0x18u, value);
        value = r_u32(table);
        w_u32(0x80108A48u, effect);
        if (value != 0u)
            w_u32(value + 0x1Cu, effect);
        count = r_u32(0x80108A2Cu);
        w_u32(table, effect);
        other_count = r_u32(0x80126D60u);
        w_u32(0x80108A2Cu, count + 1u);
        w_u32(0x80126D60u, other_count + 1u);
        return effect;
    }
    effect = r_u32(0x800FB3E0u);
    while (effect != 0u) {
        time = r_s16(effect + 0x26u);
        if (time < minimum)
            minimum = time;
        effect = r_u32(effect + 0x18u);
    }
    (void)minimum;
    return effect;
}

void game_emit_fragment_flash(uint32 x, uint32 y, uint32 z)
{
    uint32 effect, value;
    FUNCTION_MARKER(0x800BBD64, "GAME.OVL");
    effect = game_allocate_flash(0u);
    if (effect == 0u)
        return;
    w_u32(effect + 0x20u, 0xC0C0C0u);
    w_u16(effect + 0x24u, 500u);
    w_u32(effect + 0xCu, 384u);
    w_u32(effect, x);
    w_u32(effect + 4u, y);
    w_u32(effect + 8u, z);
    w_u16(effect + 0x26u, 0u);
    w_u16(effect + 0x28u, 16u);
    value = (uint32)random_between(0, 30);
    value = r_u32(0x800313F8u) + value;
    w_u16(effect + 0x2Cu, (uint16)value);
}

void game_collide_mission_projectile(uint32 projectile)
{
    uint32 target, flags, category, format_address, name_address, value, callback;
    FUNCTION_MARKER(0x8008146C, "GAME.OVL");
    target = r_u32(projectile + 0x178u);
    if (r_u32(target) != 0u && (r_u32(target + 0x114u) & 0x800000u) == 0u) {
        category = (uint32)(sint32)r_s16(target + 0x24Eu);
        format_address = r_u32(0x800F2580u);
        name_address = 0x80059E70u + category * 44u;
        sprintfPSX(0x800FA368u, format_address, name_address);
        game_queue_hud_message(0x800FA368u);
        value = r_u32(target + 0x84u);
        game_emit_sized_fragment_flash(projectile + 8u, 1000u << (value & 31u));
        flags = r_u32(target + 0x114u);
        w_u32(target + 0x114u, flags | 0x40800000u);
        game_attach_position_light(target, projectile + 8u, 0xFFFFFFu, 8u, 0u);
        w_u32(target + 0x40u, 0x800815ACu);
    }
    callback = r_u32(projectile + 0x48u);
    psx_dispatch_guest_call(callback, 1u, projectile, 0u, 0u);
}
