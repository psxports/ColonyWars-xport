#include "game_functions.h"
#include <string.h>

void game_try_fire_motion_missile(uint32 object)
{
    uint32 metadata, target, offset;
    FUNCTION_MARKER(0x800EC060, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    if (r_u8(metadata + 0x14C) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076D38, 0);
        return;
    }
    if (r_u32(metadata + 0x174) == 0)
        return;
    target = r_u32(object + 0x178);
    offset = r_u32(metadata + 0x150);
    game_create_profile_06_projectile(object, target, offset, 1);
    w_u32(metadata + 0x170, r_u32(0x800311B8));
    game_synchronize_missile_cooldowns(object);
}

void game_try_fire_stun_missile(uint32 object)
{
    uint32 metadata;
    FUNCTION_MARKER(0x800EC0F4, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    if (r_u8(metadata + 0x178) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076D5C, 0);
        return;
    }
    if (r_u32(metadata + 0x1A0) == 0)
        return;
    game_create_stun_missile(object);
    w_u32(metadata + 0x19C, r_u32(0x800311B8));
    game_synchronize_missile_cooldowns(object);
}

void game_try_fire_antishield_missile(uint32 object)
{
    uint32 metadata, target, offset;
    FUNCTION_MARKER(0x800EC204, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    if (r_u8(metadata + 0x1D0) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076D9C, 0);
        return;
    }
    if (r_u32(metadata + 0x1F8) == 0)
        return;
    target = r_u32(object + 0x178);
    offset = r_u32(metadata + 0x1D4);
    game_create_antishield_missile(object, target, offset, 2);
    w_u32(metadata + 0x1F4, r_u32(0x800311B8));
    game_synchronize_missile_cooldowns(object);
}

void game_try_fire_rear_lasers(uint32 object)
{
    FUNCTION_MARKER(0x800EB9F0, "GAME.OVL");
    uint32 metadata, row, cursor, index, target, offset;
    metadata = r_u32(object + 0x110);
    row = metadata + 0x30;
    if (r_u8(row) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076C64, 0);
        return;
    }
    if (r_u32(row + 0x2C) == 0)
        return;
    index = 0;
    cursor = row;
    if (r_s32(row + 4) > 0) {
        do {
            target = r_u32(object + 0x178);
            offset = r_u32(cursor + 8);
            cursor += 4;
            game_create_rear_laser(object, target, offset);
            index += 1;
        } while ((sint32)index < r_s32(row + 4));
    }
    w_u32(row + 0x24, r_u32(0x800311B8));
}

void game_try_fire_scatter_lasers(uint32 object)
{
    FUNCTION_MARKER(0x800EBF00, "GAME.OVL");
    uint32 metadata, row, cursor, index, target, offset;
    metadata = r_u32(object + 0x110);
    row = metadata + 0xF0;
    if (r_u8(row) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076CF0, 0);
        return;
    }
    if (r_u32(row + 0x2C) == 0)
        return;
    index = 0;
    cursor = row;
    if (r_s32(row + 4) > 0) {
        do {
            target = r_u32(object + 0x178);
            offset = r_u32(cursor + 8);
            cursor += 4;
            game_create_scatter_laser(object, target, offset);
            index += 1;
        } while ((sint32)index < r_s32(row + 4));
    }
    w_u32(row + 0x24, r_u32(0x800311B8));
}

void game_try_fire_hull_lasers(uint32 object)
{
    FUNCTION_MARKER(0x800EB8A4, "GAME.OVL");
    uint32 metadata, row, count, target, offset, first, second, third;
    metadata = r_u32(object + 0x110);
    row = metadata + 0x0;
    if (r_u8(row) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076C34, 0);
        return;
    }
    if (r_u32(row + 0x2C) == 0)
        return;
    count = r_u32(row + 4);
    if (count == 1 || count == 2 || count == 3) {
        target = r_u32(object + 0x178);
        offset = r_u32(row + 8);
        first = game_create_hull_laser(object, target, offset);
        if (count >= 2) {
            target = r_u32(object + 0x178);
            offset = r_u32(row + 0xC);
            second = game_create_hull_laser(object, target, offset);
            if (count == 2) {
                w_u32(first + 0x70, second);
            } else {
                target = r_u32(object + 0x178);
                offset = r_u32(row + 0x10);
                third = game_create_hull_laser(object, target, offset);
                w_u32(first + 0x70, third);
                w_u32(second + 0x70, third);
            }
        }
    } else {
        diagnostic_report(r_u32(object + 4), 0x80076C54, 0x80076BD0);
    }
    w_u32(row + 0x24, r_u32(0x800311B8));
}

void game_try_fire_antishield_lasers(uint32 object)
{
    FUNCTION_MARKER(0x800EBABC, "GAME.OVL");
    uint32 metadata, row, count, target, offset, first, second, third;
    metadata = r_u32(object + 0x110);
    row = metadata + 0x60;
    if (r_u8(row) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076C84, 0);
        return;
    }
    if (r_u32(row + 0x2C) == 0)
        return;
    count = r_u32(row + 4);
    if (count == 1 || count == 2 || count == 3) {
        target = r_u32(object + 0x178);
        offset = r_u32(row + 8);
        first = game_create_antishield_laser(object, target, offset);
        if (count >= 2) {
            target = r_u32(object + 0x178);
            offset = r_u32(row + 0xC);
            second = game_create_antishield_laser(object, target, offset);
            if (count == 2) {
                w_u32(first + 0x70, second);
            } else {
                target = r_u32(object + 0x178);
                offset = r_u32(row + 0x10);
                third = game_create_antishield_laser(object, target, offset);
                w_u32(first + 0x70, third);
                w_u32(second + 0x70, third);
            }
        }
    } else {
        diagnostic_report(r_u32(object + 4), 0x80076C54, 0x80076BD0);
    }
    w_u32(row + 0x24, r_u32(0x800311B8));
}

void game_try_fire_emp_lasers(uint32 object)
{
    FUNCTION_MARKER(0x800EBC58, "GAME.OVL");
    uint32 metadata, row, count, target, offset, first, second, third;
    metadata = r_u32(object + 0x110);
    row = metadata + 0x90;
    if (r_u8(row) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076CAC, 0);
        return;
    }
    if (r_u32(row + 0x2C) == 0)
        return;
    count = r_u32(row + 4);
    if (count == 1 || count == 2 || count == 3) {
        target = r_u32(object + 0x178);
        offset = r_u32(row + 8);
        first = game_create_emp_laser(object, target, offset);
        if (count >= 2) {
            target = r_u32(object + 0x178);
            offset = r_u32(row + 0xC);
            second = game_create_emp_laser(object, target, offset);
            if (count == 2) {
                w_u32(first + 0x70, second);
            } else {
                target = r_u32(object + 0x178);
                offset = r_u32(row + 0x10);
                third = game_create_emp_laser(object, target, offset);
                w_u32(first + 0x70, third);
                w_u32(second + 0x70, third);
            }
        }
    } else {
        diagnostic_report(r_u32(object + 4), 0x80076C54, 0x80076BD0);
    }
    w_u32(row + 0x24, r_u32(0x800311B8));
}

void game_try_fire_ki_plasma_lasers(uint32 object)
{
    FUNCTION_MARKER(0x800EBDAC, "GAME.OVL");
    uint32 metadata, row, count, target, offset, first, second, third;
    metadata = r_u32(object + 0x110);
    row = metadata + 0xC0;
    if (r_u8(row) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076CCC, 0);
        return;
    }
    if (r_u32(row + 0x2C) == 0)
        return;
    count = r_u32(row + 4);
    if (count == 1 || count == 2 || count == 3) {
        target = r_u32(object + 0x178);
        offset = r_u32(row + 8);
        first = game_create_ki_plasma_laser(object, target, offset);
        if (count >= 2) {
            target = r_u32(object + 0x178);
            offset = r_u32(row + 0xC);
            second = game_create_ki_plasma_laser(object, target, offset);
            if (count == 2) {
                w_u32(first + 0x70, second);
            } else {
                target = r_u32(object + 0x178);
                offset = r_u32(row + 0x10);
                third = game_create_ki_plasma_laser(object, target, offset);
                w_u32(first + 0x70, third);
                w_u32(second + 0x70, third);
            }
        }
    } else {
        diagnostic_report(r_u32(object + 4), 0x80076C54, 0x80076BD0);
    }
    w_u32(row + 0x24, r_u32(0x800311B8));
}

void game_try_fire_primary_lasers(uint32 object)
{
    FUNCTION_MARKER(0x800EBC10, "GAME.OVL");
    uint32 target;
    target = r_u32(object + 0x178);
    if (r_s32(target + 0x234) > 0)
        game_try_fire_antishield_lasers(object);
    else
        game_try_fire_hull_lasers(object);
}

uint32 game_create_emp_laser(uint32 owner, uint32 target, uint32 offset)
{
    FUNCTION_MARKER(0x800B1884, "GAME.OVL");
    uint32 effect;
    effect = game_create_profile_effect(owner, target, offset, 0x800EF12C);
    w_u32(effect + 0x10, 0x40FFC800);
    w_u32(effect + 0xC, 0x40FF5A00);
    w_u32(effect + 0x68, 0x800EEB6C);
    return effect;
}

static uint32 game_create_hull_laser_body(uint32 owner, uint32 target, uint32 offset, SVECTOR *native_offset)
{
    uint32 effect;
    effect = native_offset != NULL ? game_create_profile_effect_native(owner, target, native_offset, 0x800EF140) : game_create_profile_effect(owner, target, offset, 0x800EF140);
    w_u32(effect + 0x14, 0x800EF1F4);
    w_u32(effect + 0x1C, 3);
    w_u32(effect + 0x68, 0x800EEB2C);
    return effect;
}

uint32 game_create_hull_laser(uint32 owner, uint32 target, uint32 offset)
{
    FUNCTION_MARKER(0x800B1994, "GAME.OVL");
    return game_create_hull_laser_body(owner, target, offset, NULL);
}

uint32 game_create_hull_laser_native(uint32 owner, uint32 target, SVECTOR *offset)
{
    return game_create_hull_laser_body(owner, target, 0, offset);
}

uint32 game_create_antishield_laser(uint32 owner, uint32 target, uint32 offset)
{
    FUNCTION_MARKER(0x800B1A40, "GAME.OVL");
    uint32 effect;
    effect = game_create_profile_effect(owner, target, offset, 0x800EF168);
    w_u32(effect + 0x14, 0x800EF274);
    w_u32(effect + 0x1C, 3);
    w_u32(effect + 0x68, 0x800EEB4C);
    return effect;
}

uint32 game_create_rear_laser(uint32 owner, uint32 target, uint32 offset)
{
    FUNCTION_MARKER(0x800B19D8, "GAME.OVL");
    uint32 effect, x, z, y;
    effect = game_create_profile_effect(owner, target, offset, 0x800EF140);
    w_u32(effect + 0x14, 0x800EF1F4);
    w_u32(effect + 0x1C, 3);
    x = r_u32(effect + 0x20);
    w_u32(effect + 0x68, 0x800EEB2C);
    z = r_u32(effect + 0x28);
    w_u32(effect + 0x20, 0 - x);
    y = r_u32(effect + 0x24);
    w_u32(effect + 0x28, 0 - z);
    w_u32(effect + 0x24, 0 - y);
    return effect;
}

uint32 game_create_ki_plasma_laser(uint32 owner, uint32 target, uint32 offset)
{
    uint32 effect, x, y, z, owner_y, owner_z, position;
    FUNCTION_MARKER(0x800B18CC, "GAME.OVL");
    effect = game_create_profile_effect(owner, target, offset, 0x800EF1E0);
    if (effect == 0)
        return 0;
    x = r_u32(effect + 0x20);
    w_u32(effect + 0x14, 0x800EF1F4);
    w_u32(effect + 0x18, 0);
    x += r_u32(owner + 0xFC);
    w_u32(effect + 0x20, x);
    y = r_u32(effect + 0x24);
    owner_y = r_u32(owner + 0x100);
    x = r_u32(effect + 0x20);
    w_u32(effect + 0x24, y + owner_y);
    position = r_u32(effect);
    owner_z = r_u32(owner + 0x104);
    w_u32(effect, position + (x << 1));
    z = r_u32(effect + 0x28);
    w_u32(effect + 0x68, 0x800EEB3C);
    w_u32(effect + 0x28, z + owner_z);
    y = r_u32(effect + 0x24);
    position = r_u32(effect + 4);
    z = r_u32(effect + 0x28);
    w_u32(effect + 4, position + (y << 1));
    position = r_u32(effect + 8);
    w_u32(effect + 8, position + (z << 1));
    return effect;
}

uint32 game_create_scatter_laser(uint32 owner, uint32 target, uint32 offset)
{
    uint32 effect, component;
    sint32 angle;
    FUNCTION_MARKER(0x800B177C, "GAME.OVL");
    effect = game_create_profile_effect(owner, target, offset, 0x800EF118);
    w_u32(effect + 0x14, 0x800EF2F4);
    angle = random_between(-56, 56);
    matrix_rotate_y((MATRIX *)psx_addr(0x80042188, 20),
        (MATRIX *)psx_addr(owner + 0x11C, 20), (uint32)angle);
    angle = random_between(-56, 56);
    matrix_rotate_x((MATRIX *)psx_addr(0x8004EAF8, 20),
        (MATRIX *)psx_addr(0x80042188, 20), (uint32)angle);
    component = (uint32)r_s16(0x8004EAFC);
    component = (((component << 5) - component) << 2) + component;
    w_u32(effect + 0x20, (sint32)component >> 8);
    component = (uint32)r_s16(0x8004EB02);
    component = (((component << 5) - component) << 2) + component;
    w_u32(effect + 0x24, (sint32)component >> 8);
    component = (uint32)r_s16(0x8004EB08);
    w_u32(effect + 0x68, 0x800EEB5C);
    component = (((component << 5) - component) << 2) + component;
    w_u32(effect + 0x28, (sint32)component >> 8);
    return effect;
}

static uint32 game_create_model_effect_body(uint32 position, const VECTOR *native_position, sint32 model_index)
{
    uint32 effect, payload, index, table, value;
    effect = game_allocate_effect(3, 0);
    payload = heap_alloc_back(0x3C);
    index = (uint32)(sint32)(sint16)model_index;
    w_u32(effect + 0x6C, payload);
    w_u32(payload + 0x24, index);
    table = r_u32(0x80046178);
    payload = r_u32(effect + 0x6C);
    value = r_u32(table + (((index << 5) + index) << 2));
    w_u32(payload + 0x28, value);
    payload = r_u32(effect + 0x6C);
    w_u16(effect + 0x3C, 1);
    matrix_set_identity_native((MATRIX *)psx_addr(payload + 4, 18));
    value = native_position ? (uint32)native_position->vx : r_u32(position);
    payload = r_u32(effect + 0x6C);
    w_u32(effect, value);
    w_u32(effect + 4, native_position ? (uint32)native_position->vy : r_u32(position + 4));
    w_u32(effect + 8, native_position ? (uint32)native_position->vz : r_u32(position + 8));
    w_u16(payload + 0x2E, 0);
    payload = r_u32(effect + 0x6C);
    w_u16(payload + 0x30, 0);
    payload = r_u32(effect + 0x6C);
    w_u16(payload + 0x32, 0);
    payload = r_u32(effect + 0x6C);
    w_u16(payload + 0x2C, 1);
    payload = r_u32(effect + 0x6C);
    w_u32(effect + 0x2C, 0x12C);
    w_u16(effect + 0x34, 0);
    w_u32(payload, 0);
    w_u16(effect + 0x40, 0x1B58);
    return effect;
}

uint32 game_create_model_effect(uint32 position, sint32 model_index)
{
    FUNCTION_MARKER(0x800B1FE4, "GAME.OVL");
    return game_create_model_effect_body(position, 0, model_index);
}

uint32 game_create_model_effect_native(const VECTOR *position, sint32 model_index)
{
    return game_create_model_effect_body(0, position, model_index);
}

void game_clear_trail_slots(void)
{
    sint32 offset;
    FUNCTION_MARKER(0x800B2710, "GAME.OVL");
    for (offset = 0x1644; offset >= 0; offset -= 0x12C)
        w_u32(0x80124D68 + (uint32)offset, 0);
}

uint32 game_draw_laser_projectile(uint32 object)
{
    FUNCTION_MARKER(0x800B37B0, "GAME.OVL");
    return object_draw_visible(object);
}

void game_bind_overlay_effect_textures(void)
{
    FUNCTION_MARKER(0x800B5D78, "GAME.OVL");
    game_bind_mesh_effect_texture(0x194, 1);
    game_bind_mesh_effect_texture(0x195, 2);
    game_bind_mesh_effect_texture(0x196, 3);
    game_bind_mesh_effect_texture(0x1A3, 4);
    game_bind_mesh_effect_texture(0x190, 0x1A);
    game_bind_mesh_effect_texture(0x191, 0x1B);
    game_bind_mesh_effect_texture(0x192, 0x1C);
    game_bind_mesh_effect_texture(0x193, 0x1D);
    game_bind_mesh_effect_texture(0xE4, 0x1E);
    game_bind_mesh_effect_texture(0x1A2, 0x1F);
}

uint32 game_pack_palette_color(sint32 red, sint32 green, sint32 blue)
{
    uint32 value;
    FUNCTION_MARKER(0x800B5E30, "GAME.OVL");
    value = (uint32)(red >> 3);
    value += ((uint32)(green >> 3) << 5) + 0xFFFF8000;
    value += (uint32)(blue >> 3) << 10;
    return value & 0xFFFF;
}

sint32 game_upload_palette_row(uint32 x, uint32 y, uint32 pixels)
{
    FUNCTION_MARKER(0x800B5E5C, "GAME.OVL");
    return game_upload_palette_row_native(x, y, (const uint16 *)psx_addr(pixels, 32));
}

sint32 game_upload_palette_row_native(uint32 x, uint32 y, const uint16 *pixels)
{
    PSX_RECT rectangle;
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    rectangle.w = 16;
    rectangle.h = 1;
    LoadImagePSX(&rectangle, (uint32 *)pixels);
    return DrawSync(0);
}

void game_initialize_head_models_and_palettes(void)
{
    char bundle_name[32], model_name[32];
    uint16 palette[16];
    uint32 type, table, index, offset, model, first, second, geometry, value, row, slot;
    sint32 count, scan, width, height, color;
    FUNCTION_MARKER(0x800B5EA0, "GAME.OVL");
    w_u32(0x80118260, 300);
    width = r_s32(0x8003119C);
    height = r_s32(0x800311B0);
    w_u32(0x800FB420, 0);
    w_u32(0x800EF4A4, 0);
    width >>= 1;
    height >>= 1;
    w_u32(0x800F4F34, width);
    w_u32(0x800F4F38, height);
    w_u32(0x800F80F8, width);
    w_u32(0x800F8148, height);
    for (type = 1; type < 17; ++type) {
        w_u32(0x8010AC90 + (type - 1) * 4, 0);
        if (type == 7)
            continue;
        value = r_u32(0x80127044);
        count = (sint32)(value + r_u32(0x80127040));
        value = 0;
        for (scan = 0; scan < count; ++scan) {
            if (r_u32(0x80119C54 + (uint32)scan * 0x34) == type)
                value = 1;
        }
        if (value == 0)
            continue;
        sprintf(bundle_name, "W_%d_HD.BND", (int)type);
        w_u32(0x80118264, 1);
        index = game_load_resource_bundle(bundle_name);
        w_u32(0x800EDD1C, index);
        game_bind_model_name_native(bundle_name, index);
        sprintf(model_name, "W_%d_HD", (int)type);
        index = game_find_model_name_native(model_name);
        offset = ((index << 5) + index) << 2;
        table = r_u32(0x80046178);
        model = r_u32(table + offset);
        model = mesh_import_primitives(model);
        table = r_u32(0x80046178);
        w_u32(0x8010AC90 + (type - 1) * 4, model);
        value = r_u32(table + offset);
        heap_free(value - 0xC);
        table = r_u32(0x80046178);
        value = r_u32(0x8010AC90 + (type - 1) * 4);
        w_u32(table + offset, value);
        w_u32(table + offset + 0x74, 1);
    }
    first = r_u32(0x800EC890);
    table = r_u32(0x80046178);
    second = r_u32(0x800EC894);
    first = r_u32(table + (((first << 5) + first) << 2));
    second = r_u32(table + (((second << 5) + second) << 2));
    geometry = r_u32(first + 0x10);
    w_u32(geometry + 0x1014, 0xFF808080);
    w_u32(r_u32(first + 0x10) + 0x10C8, 0xFF808080);
    w_u32(r_u32(first + 0x10) + 0xEE8, 0xFF808080);
    w_u32(r_u32(first + 0x10) + 0x11F4, 0xFF808080);
    w_u32(r_u32(first + 0x10) + 0xFD8, 0xFF808080);
    w_u32(r_u32(second + 0x10) + 0x1014, 0xFF808080);
    w_u32(r_u32(second + 0x10) + 0x10C8, 0xFF808080);
    w_u32(r_u32(second + 0x10) + 0xEE8, 0xFF808080);
    w_u32(r_u32(second + 0x10) + 0x11F4, 0xFF808080);
    geometry = r_u32(second + 0x10);
    w_u32(0x80123F48, first);
    w_u32(0x800F3D9C, second);
    w_u32(geometry + 0xFD8, 0xFF808080);
    for (index = 0; index < 16; ++index) {
        value = ((0x200 + index * 16) >> 4) & 0x3F;
        w_u16(0x800F33B8 + index * 2, value | 0x7E80);
        w_u16(0x80108968 + index * 2, value | 0x7EC0);
    }
    for (row = 0; row < 15; ++row) {
        for (scan = 14; scan >= 0; --scan)
            palette[scan] = 0x8000;
        palette[15] = (uint16)game_pack_palette_color(20, 20, 20);
        for (index = 1; index <= row; ++index)
            palette[index] = (uint16)game_pack_palette_color((15 - index) * 10 + 100, 100 + index * 10, 100 + index * 10);
        game_upload_palette_row_native(0x200 + row * 16, 0x1FA, palette);
    }
    for (row = 0; row < 7; ++row) {
        for (scan = 15; scan >= 0; --scan)
            palette[scan] = 0x8000;
        for (index = 0; index < row; ++index) {
            slot = (5 - index) * 2 + 1;
            palette[slot] = (uint16)game_pack_palette_color(255, 255, 0);
            palette[slot + 1] = (uint16)game_pack_palette_color(255, 0, 0);
        }
        game_upload_palette_row_native(0x200 + row * 16, 0x1FB, palette);
    }
    for (scan = 15; scan >= 0; --scan)
        palette[scan] = 0x8000;
    for (index = 1; index < 8; ++index) {
        color = (7 - index) * 255 / 7;
        palette[index] = (uint16)game_pack_palette_color(color, color, color);
    }
    for (index = 0; index < 8; ++index) {
        color = (7 - index) * 255 / 7;
        palette[index + 8] = (uint16)game_pack_palette_color(color, color, color);
    }
    game_upload_palette_row_native(0x2E0, 0x1FB, palette);
    for (scan = 15; scan >= 0; --scan)
        palette[scan] = 0x8000;
    for (index = 0; index < 8; ++index) {
        color = (7 - index) * 255 / 7;
        palette[index + 8] = (uint16)game_pack_palette_color(color, color, color);
    }
    game_upload_palette_row_native(0x2F0, 0x1FB, palette);
}

static void game_emit_model_fragments_body(uint32 position, const VECTOR *native_position, sint32 count, uint32 mode)
{
    MATRIX first_rotation, second_rotation;
    uint32 used, limit, index, effect, payload, next_payload, value, speed, product;
    sint32 random_value;
    used = r_u32(0x80042178);
    limit = r_u32(0x80010238);
    if (mode != 0) {
        if ((sint32)limit < (sint32)used)
            count >>= 1;
    } else {
        if ((sint32)limit < (sint32)used)
            return;
        if ((sint32)(limit - 100) < (sint32)used)
            count = 1;
    }
    for (index = 0; (sint32)index < count; ++index) {
        value = r_u32(0x800EDFCC) + 3;
        effect = native_position != NULL ? game_create_model_effect_native(native_position, (sint32)(sint16)value) : game_create_model_effect(position, (sint32)(sint16)value);
        payload = r_u32(effect + 0x6C);
        w_u16(effect + 0x3C, 0x6B);
        w_u16(effect + 0x46, 0);
        w_u32(payload, 0);
        random_value = random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C);
        w_u16(payload + 0x34, random_value);
        random_value = random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C);
        w_u16(payload + 0x36, random_value);
        random_value = random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C);
        w_u16(payload + 0x38, random_value);
        random_value = random_between(0, 0xFFF);
        payload = r_u32(effect + 0x6C);
        matrix_rotate_y(&first_rotation, (MATRIX *)psx_addr(payload + 4, 20), 0 - (uint32)random_value);
        random_value = random_between(0, 0xFFF);
        matrix_rotate_x(&second_rotation, &first_rotation, (uint32)random_value);
        random_value = random_between(0, 0xFFF);
        payload = r_u32(effect + 0x6C);
        matrix_rotate_z((MATRIX *)psx_addr(payload + 4, 20), &second_rotation, 0 - (uint32)random_value);
        if (index == 0 && mode != 0) {
            w_u16(effect + 0x34, (uint16)-2);
            speed = 0x226;
        } else {
            w_u16(effect + 0x34, (uint16)-3);
            speed = (uint32)random_between(0x172, 0x1F4);
        }
        payload = r_u32(effect + 0x6C);
        product = (uint32)r_s16(payload + 8) * speed;
        next_payload = r_u32(effect + 0x6C);
        w_u32(effect + 0x20, (sint32)product >> 12);
        product = (uint32)r_s16(next_payload + 0xE) * speed;
        payload = r_u32(effect + 0x6C);
        w_u32(effect + 0x24, (sint32)product >> 12);
        product = (uint32)r_s16(payload + 0x14) * speed;
        w_u32(effect + 0x30, 0x7FFFFFF);
        w_u32(effect + 0x60, 0x8002493C);
        w_u32(effect + 0x2C, 0x96);
        w_u32(effect + 0x28, (sint32)product >> 12);
        value = r_u32(0x800595CC);
        w_u32(0x800595CC, value + 1);
    }
}

void game_emit_model_fragments(uint32 position, sint32 count, uint32 mode)
{
    FUNCTION_MARKER(0x800A2F58, "GAME.OVL");
    game_emit_model_fragments_body(position, NULL, count, mode);
}

void game_emit_model_fragments_native(const VECTOR *position, sint32 count, uint32 mode)
{
    game_emit_model_fragments_body(0, position, count, mode);
}

void game_handle_effect_contact(uint32 effect, uint32 other)
{
    uint32 kind, x, y, z, vx, vy, vz, value;
    sint32 random_value;
    FUNCTION_MARKER(0x800B2520, "GAME.OVL");
    kind = (uint32)r_s16(effect + 0x3C);
    if (kind != 0x64 && kind != 0x69)
        return;
    vx = r_u32(effect) - r_u32(other);
    vy = r_u32(effect + 4) - r_u32(other + 4);
    vz = r_u32(effect + 8) - r_u32(other + 8);
    x = r_u32(effect);
    y = r_u32(effect + 4);
    z = r_u32(effect + 8);
    game_emit_narrow_particles(kind == 0x64 ? 8 : 12, x, y, z, vx, vy, vz);
    if (kind == 0x64) {
        random_value = random_between(-50, 50);
        value = r_u32(effect + 0x28);
        w_u32(effect + 0x28, value + (uint32)random_value);
        random_value = random_between(-50, 50);
        value = r_u32(effect + 0x20);
        w_u32(effect + 0x20, value + (uint32)random_value);
        sound_play_sample(4);
        if (random_between(0, 120) < 3) {
            value = r_u16(effect + 0x34) - 1;
            w_u16(effect + 0x34, value);
            if ((sint16)value < -3)
                game_mark_effect_for_removal(effect);
            random_value = random_between(12, 18);
            game_emit_model_fragments(effect, random_value, 1);
        } else {
            random_value = random_between(1, 2);
            game_emit_model_fragments(effect, random_value, 0);
        }
    } else {
        x = r_u32(effect);
        vx = r_u32(other + 0x20);
        w_u32(effect, x + vx);
        y = r_u32(effect + 4);
        vy = r_u32(other + 0x24);
        w_u32(effect + 4, y + vy);
        z = r_u32(effect + 8);
        vz = r_u32(other + 0x28);
        w_u32(effect + 8, z + vz);
        sound_play_sample(4);
    }
}

uint32 game_draw_horizontal_reticle(uint32 packet)
{
    uint32 x, y, ordering, current;
    FUNCTION_MARKER(0x800B5C74, "GAME.OVL");
    w_u32(packet + 4, 0x8040);
    x = r_u32(0x80031198);
    y = r_u32(0x800311B4);
    w_u16(packet + 8, x - 8);
    w_u16(packet + 0xA, y);
    w_u16(packet + 0xC, x - 16);
    w_u16(packet + 0xE, y);
    setLineF2((LINE_F2 *)psx_addr(packet, 8));
    current = r_u32(0x80040690);
    SetSemiTrans(psx_addr(current, 8), 1);
    ordering = r_u32(0x8004608C);
    current = r_u32(0x80040690);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(current, 4));
    current = r_u32(0x80040690);
    packet = current + 0x10;
    w_u32(0x80040690, packet);
    w_u32(current + 0x14, 0x8040);
    x = r_u32(0x80031198);
    y = r_u32(0x800311B4);
    w_u16(current + 0x18, x + 8);
    w_u16(current + 0x1A, y);
    w_u16(current + 0x1C, x + 16);
    w_u16(current + 0x1E, y);
    setLineF2((LINE_F2 *)psx_addr(packet, 8));
    current = r_u32(0x80040690);
    SetSemiTrans(psx_addr(current, 8), 1);
    ordering = r_u32(0x8004608C);
    current = r_u32(0x80040690);
    AddPrim(psx_addr(ordering + 0x70, 4), psx_addr(current, 4));
    current = r_u32(0x80040690) + 0x10;
    w_u32(0x80040690, current);
    return current;
}

void game_draw_burst_effect(uint32 object)
{
    SVECTOR vertex;
    sint32 screen, projection, flags, divisor, radius;
    uint32 camera, x, y, z, ax, ay, az, shifts, depth;
    FUNCTION_MARKER(0x800B3C08, "GAME.OVL");
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    camera = r_u32(0x80040C94);
    x = r_u32(object + 8) - r_u32(camera + 8);
    y = r_u32(object + 0xC) - r_u32(camera + 0xC);
    z = r_u32(object + 0x10) - r_u32(camera + 0x10);
    shifts = 0;
    for (;;) {
        ax = (sint32)x < 0 ? 0 - x : x;
        ay = (sint32)y < 0 ? 0 - y : y;
        az = (sint32)z < 0 ? 0 - z : z;
        if ((sint32)ax <= 32000 && (sint32)ay <= 32000 && (sint32)az <= 32000)
            break;
        ++shifts;
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
    }
    vertex.vx = (sint16)x;
    vertex.vy = (sint16)y;
    vertex.vz = (sint16)z;
    vertex.pad = 0;
    depth = (uint32)RotTransPers(&vertex, &screen, &projection, &flags);
    depth <<= (shifts + 2) & 31;
    if ((sint32)depth > 0) {
        divisor = (sint32)depth >> 1;
        if (divisor == 0)
            xport_mips_break(7);
        radius = (0xB71B000 / divisor) >> 9;
        game_draw_projected_ring((sint16)screen, (sint16)((uint32)screen >> 16), radius, 0x200000, 0x500000);
    }
    x = r_u32(object + 8);
    y = r_u32(object + 0xC);
    z = r_u32(object + 0x10);
    game_emit_radial_effect(x, y, z, 0x4B0, 0x805090);
    game_draw_position_glow(object + 8, 0xFFD0A0, 0xBB8);
    x = r_u32(object + 8);
    y = r_u32(object + 0xC);
    z = r_u32(object + 0x10);
    game_emit_drifting_particles(8, x, y, z);
}

void game_draw_pulsing_burst_effect(uint32 object)
{
    SVECTOR vertex;
    sint32 screen, projection, flags, divisor, radius;
    uint32 factor, camera, x, y, z, ax, ay, az, shifts, depth, size, product;
    FUNCTION_MARKER(0x800B4184, "GAME.OVL");
    factor = (uint32)((rsin(r_u32(0x800311B8) << 8) >> 2) + 0x1000);
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    camera = r_u32(0x80040C94);
    x = r_u32(object + 8) - r_u32(camera + 8);
    y = r_u32(object + 0xC) - r_u32(camera + 0xC);
    z = r_u32(object + 0x10) - r_u32(camera + 0x10);
    shifts = 0;
    for (;;) {
        ax = (sint32)x < 0 ? 0 - x : x;
        ay = (sint32)y < 0 ? 0 - y : y;
        az = (sint32)z < 0 ? 0 - z : z;
        if ((sint32)ax <= 32000 && (sint32)ay <= 32000 && (sint32)az <= 32000)
            break;
        ++shifts;
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
    }
    vertex.vx = (sint16)x;
    vertex.vy = (sint16)y;
    vertex.vz = (sint16)z;
    vertex.pad = 0;
    depth = (uint32)RotTransPers(&vertex, &screen, &projection, &flags);
    depth <<= (shifts + 2) & 31;
    if ((sint32)depth > 0) {
        divisor = (sint32)depth >> 1;
        if (divisor == 0)
            xport_mips_break(7);
        radius = (0xB71B000 / divisor) >> 9;
        product = (uint32)radius * factor;
        radius = (sint32)product >> 12;
        game_draw_projected_ring((sint16)screen, (sint16)((uint32)screen >> 16), radius, 0x20, 0x50);
    }
    size = (factor << 2) + factor;
    size = ((size << 4) - size) << 4;
    x = r_u32(object + 8);
    y = r_u32(object + 0xC);
    z = r_u32(object + 0x10);
    game_emit_radial_effect(x, y, z, (uint32)((sint32)size >> 12), 0x208080);
    size = ((factor << 1) + factor) << 4;
    size -= factor;
    size = (size << 3) - factor;
    size = (size << 3);
    game_draw_position_glow(object + 8, 0xA0D0FF, (uint32)((sint32)size >> 12));
}

static uint32 game_ring_add(uint32 first, uint32 second, uint32 pc)
{
    sint64 sum = (sint64)(sint32)first + (sint32)second;
    if (sum > 2147483647 || sum < (-2147483647 - 1))
        xport_mips_overflow_exception(pc);
    return first + second;
}

void game_draw_projected_ring(sint32 x, sint32 y, sint32 radius, uint32 center_color, uint32 edge_color)
{
    uint32 angle, outer_radius, sine, cosine, product, first_x, first_y, next_x, next_y;
    uint32 packet, ordering;
    FUNCTION_MARKER(0x800CE3D4, "GAME.OVL");
    outer_radius = game_ring_add((uint32)(radius >> 2), (uint32)radius, 0x800CE424);
    angle = 0;
    do {
        sine = (uint32)r_s16(0x8003DC68 + (angle & 0xFFF) * 2);
        cosine = (uint32)r_s16(0x8003DC68 + ((angle + 0x400) & 0xFFF) * 2);
        product = sine * (uint32)radius;
        first_x = game_ring_add((uint32)((sint32)product >> 12), (uint32)x, 0x800CE470);
        product = cosine * (uint32)radius;
        first_y = game_ring_add((uint32)((sint32)product >> 13), (uint32)y, 0x800CE480);
        product = sine * outer_radius;
        game_ring_add((uint32)((sint32)product >> 12), (uint32)x, 0x800CE490);
        product = cosine * outer_radius;
        game_ring_add((uint32)((sint32)product >> 13), (uint32)y, 0x800CE4A0);
        angle += 0x100;
        sine = (uint32)r_s16(0x8003DC68 + (angle & 0xFFF) * 2);
        cosine = (uint32)r_s16(0x8003DC68 + ((angle + 0x400) & 0xFFF) * 2);
        product = sine * (uint32)radius;
        next_x = game_ring_add((uint32)((sint32)product >> 12), (uint32)x, 0x800CE4F0);
        product = cosine * (uint32)radius;
        next_y = game_ring_add((uint32)((sint32)product >> 13), (uint32)y, 0x800CE500);
        product = sine * outer_radius;
        game_ring_add((uint32)((sint32)product >> 12), (uint32)x, 0x800CE510);
        product = cosine * outer_radius;
        game_ring_add((uint32)((sint32)product >> 13), (uint32)y, 0x800CE520);
        packet = r_u32(0x80040690);
        w_u32(packet + 4, center_color);
        w_u32(packet + 0xC, edge_color);
        w_u32(packet + 0x14, edge_color);
        w_u8(packet + 7, 0x32);
        w_u8(packet + 3, 6);
        w_u16(packet + 8, x);
        w_u16(packet + 0xA, y);
        w_u16(packet + 0x10, first_x);
        w_u16(packet + 0x12, first_y);
        w_u16(packet + 0x18, next_x);
        w_u16(packet + 0x1A, next_y);
        ordering = r_u32(0x800421AC);
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        packet = r_u32(0x80040690);
        packet = game_ring_add(packet, 0x1C, 0x800CE5BC);
        w_u32(0x80040690, packet);
    } while (angle < 0x1000);
}

static void game_draw_position_glow_body(uint32 position, const uint32 *native_position, uint32 color, uint32 size)
{
    SVECTOR vertex;
    sint32 screen, projection, flags, quotient;
    uint32 camera, x, y, z, ax, ay, az, scaled_size, size_shifts, shifts, angle, depth;
    uint32 numerator, large_radius, small_radius, packet, secondary_angle, product;
    uint32 sx, sy, iteration, side, dim_color;
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    camera = r_u32(0x80040C94);
    x = (native_position ? native_position[0] : r_u32(position)) - r_u32(camera + 8);
    y = (native_position ? native_position[1] : r_u32(position + 4)) - r_u32(camera + 0xC);
    angle = r_u32(0x800311B8) << 5;
    z = (native_position ? native_position[2] : r_u32(position + 8)) - r_u32(camera + 0x10);
    scaled_size = size;
    size_shifts = 0;
    while ((sint32)scaled_size > 64000) {
        scaled_size = (uint32)((sint32)scaled_size >> 1);
        x = (uint32)((sint32)x >> 1);
        z = (uint32)((sint32)z >> 1);
        y = (uint32)((sint32)y >> 1);
        ++size_shifts;
    }
    shifts = 0;
    for (;;) {
        ax = (sint32)x < 0 ? 0 - x : x;
        ay = (sint32)y < 0 ? 0 - y : y;
        az = (sint32)z < 0 ? 0 - z : z;
        if ((sint32)ax <= 16380 && (sint32)ay <= 16380 && (sint32)az <= 16380)
            break;
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
        ++shifts;
    }
    vertex.vx = (sint16)x;
    vertex.vy = (sint16)y;
    vertex.vz = (sint16)z;
    vertex.pad = 0;
    depth = (uint32)RotTransPers(&vertex, &screen, &projection, &flags);
    depth <<= ((uint32)(sint32)(sint16)shifts + 2) & 31;
    if ((sint32)depth < 32)
        return;
    numerator = (((scaled_size << 5) - scaled_size) << 2) + scaled_size;
    numerator <<= 9;
    quotient = (sint32)numerator / (sint32)depth;
    depth += (uint32)(sint32)(sint16)size_shifts;
    large_radius = (uint32)(quotient >> 8);
    small_radius = (uint32)(quotient >> 11);
    sx = (uint16)screen;
    sy = (uint16)((uint32)screen >> 16);
    for (iteration = 0; iteration < 4; ++iteration) {
        for (side = 0; side < 2; ++side) {
            packet = r_u32(0x80040690);
            w_u32(0x80040690, packet + 0x1C);
            w_u32(packet + 4, color);
            w_u32(packet + 0xC, 0);
            w_u32(packet + 0x14, 0);
            SetPolyG3(psx_addr(packet, 8));
            SetSemiTrans(psx_addr(packet, 8), 1);
            w_u16(packet + 8, sx);
            w_u16(packet + 0xA, sy);
            product = large_radius * (uint32)rsin(angle);
            w_u16(packet + 0x10, sx + (uint32)((sint32)product >> 12));
            product = large_radius * (uint32)rcos(angle);
            secondary_angle = side == 0 ? angle - 0x400 : angle + 0x400;
            w_u16(packet + 0x12, sy + (uint32)((sint32)product >> 13));
            product = small_radius * (uint32)rsin(secondary_angle);
            w_u16(packet + 0x18, sx + (uint32)((sint32)product >> 12));
            product = small_radius * (uint32)rcos(secondary_angle);
            w_u16(packet + 0x1A, sy + (uint32)((sint32)product >> 13));
            if (side != 0)
                angle = secondary_angle;
            render_insert_primitive((sint32)depth, packet);
        }
    }
    dim_color = ((uint32)((sint32)color >> 2) & 0x3F) + ((uint32)((sint32)color >> 2) & 0x3F00);
    dim_color += (((uint32)((sint32)color >> 18) & 0x3F) << 16);
    x = (native_position ? native_position[0] : r_u32(position));
    y = (native_position ? native_position[1] : r_u32(position + 4));
    z = (native_position ? native_position[2] : r_u32(position + 8));
    game_emit_radial_effect(x, y, z, (uint32)((sint32)size >> 1), dim_color);
    x = (native_position ? native_position[0] : r_u32(position));
    y = (native_position ? native_position[1] : r_u32(position + 4));
    z = (native_position ? native_position[2] : r_u32(position + 8));
    game_emit_radial_effect(x, y, z, (uint32)((sint32)size >> 3), color);
}

void game_draw_position_glow(uint32 position, uint32 color, uint32 size)
{
    FUNCTION_MARKER(0x8008E590, "GAME.OVL");
    game_draw_position_glow_body(position, NULL, color, size);
}

void game_draw_position_glow_native(const uint32 position[3], uint32 color, uint32 size)
{
    game_draw_position_glow_body(0, position, color, size);
}

sint32 anim_play_default_movie(void)
{
    FUNCTION_MARKER(0x80091824, "ANIM.OVL");
    w_u8(0x8009191A, 1);
    w_u32(0x80091928, 0);
    return anim_play_movie(0x8001117C);
}

sint32 anim_play_movie_with_skip_delay(uint32 path)
{
    FUNCTION_MARKER(0x80091860, "ANIM.OVL");
    w_u8(0x8009191A, 1);
    w_u32(0x80091928, 0x37);
    return anim_play_movie(path);
}

sint32 anim_play_movie_without_clear(uint32 path)
{
    FUNCTION_MARKER(0x80091898, "ANIM.OVL");
    w_u8(0x8009191A, 0);
    w_u32(0x80091928, 0);
    return anim_play_movie(path);
}

static sint32 game_project_orbit_point(uint32 radius, uint32 angle, sint32 *screen)
{
    SVECTOR vertex;
    uint32 product;
    sint32 projection, flags;
    product = radius * (uint32)rsin(angle);
    vertex.vy = 0;
    vertex.vx = (sint16)((sint32)product >> 12);
    product = radius * (uint32)rcos(angle);
    vertex.vz = (sint16)((sint32)product >> 12);
    vertex.pad = 0;
    return RotTransPers(&vertex, screen, &projection, &flags);
}

static void game_emit_orbit_triangle(sint32 first, sint32 second, sint32 third, sint32 depth)
{
    uint32 packet;
    packet = r_u32(0x80040690);
    w_u32(0x80040690, packet + 0x1C);
    w_u32(packet + 8, first);
    w_u32(packet + 0x10, second);
    w_u32(packet + 4, 0);
    w_u32(packet + 0xC, 0xFF9040);
    w_u32(packet + 0x14, 0);
    w_u32(packet + 0x18, third);
    SetPolyG3(psx_addr(packet, 8));
    SetSemiTrans(psx_addr(packet, 8), 1);
    render_insert_primitive(depth, packet);
}

void game_draw_orbiting_projectile_effect(uint32 object)
{
    SVECTOR origin = {0};
    sint32 first, outer, center, trailing, leading, depth, projection, flags;
    uint32 ring, quarter, radius, outer_radius, leading_radius, angle, x, y, z;
    FUNCTION_MARKER(0x800B37D0, "GAME.OVL");
    radius = 0x200;
    outer_radius = 0x400;
    for (ring = 0; ring < 3; ++ring) {
        leading_radius = radius + (radius >> 1);
        angle = (ring + r_u32(0x800311B8)) << 8;
        for (quarter = 0; quarter < 4; ++quarter) {
            game_project_orbit_point(radius, angle, &first);
            game_project_orbit_point(outer_radius, angle - 0x100, &outer);
            RotTransPers(&origin, &center, &projection, &flags);
            game_project_orbit_point(radius, angle - 0x140, &trailing);
            depth = game_project_orbit_point(leading_radius, angle + 0x100, &leading);
            game_emit_orbit_triangle(center, first, leading, depth);
            game_emit_orbit_triangle(outer, first, leading, depth);
            angle += 0x400;
            game_emit_orbit_triangle(outer, first, trailing, depth);
            game_emit_orbit_triangle(center, first, trailing, depth);
        }
        outer_radius += 0x100;
        radius += 0x80;
    }
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    x = r_u32(object + 8);
    y = r_u32(object + 0xC);
    z = r_u32(object + 0x10);
    game_emit_radial_effect(x, y, z, 0x800, 0xFF0000);
    x = r_u32(object + 8);
    y = r_u32(object + 0xC);
    z = r_u32(object + 0x10);
    game_emit_radial_effect(x, y, z, 0x400, 0x404040);
    x = r_u32(object + 8);
    y = r_u32(object + 0xC);
    z = r_u32(object + 0x10);
    game_emit_radial_effect(x, y, z, 0x200, 0xFFFFFF);
}

void game_begin_mesh_effect_sequence(uint32 object)
{
    FUNCTION_MARKER(0x80083B38, "GAME.OVL");
    w_u32(object + 0x3C, 0x8002493C);
    w_u32(object + 0x5C, 0x800D332C);
    w_u32(object + 0x48, 0x800D3478);
    w_u32(object, 0x29B);
    w_u32(object + 0x238, 0x29B);
    game_initialize_radial_mesh(0x12C, 0x64, 0x400);
    w_u32(0x800ED394, 0xF);
    w_u16(0x800595F2, 1);
}

void game_orient_radial_mesh(uint32 x, uint32 y, uint32 z)
{
    VECTOR direction = {0}, reference = {0}, basis = {0}, first_axis = {0}, second_axis = {0};
    uint32 ax, ay, az, product;
    FUNCTION_MARKER(0x800907D0, "GAME.OVL");
    for (;;) {
        ax = (sint32)x < 0 ? 0 - x : x;
        ay = (sint32)y < 0 ? 0 - y : y;
        az = (sint32)z < 0 ? 0 - z : z;
        if ((sint32)ax <= 32000 && (sint32)ay <= 32000 && (sint32)az <= 32000)
            break;
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
    }
    direction.vx = (sint32)x;
    direction.vy = (sint32)y;
    direction.vz = (sint32)z;
    vector_normalize(&direction, &direction);
    product = (uint32)direction.vx * 1500;
    w_u32(0x800ED3AC, (sint32)product >> 12);
    product = (uint32)direction.vy * 1500;
    w_u32(0x800ED3B0, (sint32)product >> 12);
    product = (uint32)direction.vz * 1500;
    w_u32(0x800ED3B4, (sint32)product >> 12);
    direction.vx = (sint32)(0 - (uint32)direction.vx);
    direction.vy = (sint32)(0 - (uint32)direction.vy);
    direction.vz = (sint32)(0 - (uint32)direction.vz);
    reference.vx = direction.vz;
    reference.vy = direction.vx;
    reference.vz = direction.vy;
    vector_normalize(&reference, &basis);
    OuterProduct12(&direction, &basis, &first_axis);
    vector_normalize(&first_axis, &first_axis);
    OuterProduct12(&direction, &first_axis, &second_axis);
    vector_normalize(&second_axis, &second_axis);
    w_u32(0x800F4E8C, 0);
    w_u32(0x800F4E90, 0);
    w_u32(0x800F4E94, 0);
    w_u16(0x800F4E78, first_axis.vx);
    w_u16(0x800F4E7E, first_axis.vy);
    w_u16(0x800F4E84, first_axis.vz);
    w_u16(0x800F4E7A, second_axis.vx);
    w_u16(0x800F4E80, second_axis.vy);
    w_u16(0x800F4E86, second_axis.vz);
    w_u16(0x800F4E7C, direction.vx);
    w_u16(0x800F4E82, direction.vy);
    w_u16(0x800F4E88, direction.vz);
}

static sint32 game_radial_texture_coordinate(uint32 value)
{
    uint32 product = (((value << 3) - value) << 5) - value;
    return (sint32)product / 2048;
}

void game_initialize_radial_mesh(uint32 x, uint32 y, uint32 z)
{
    uint32 allocation, sector, radial, copy, vertex, next, packet, base_u, base_v;
    uint32 radius, actual_radius, product, seed, page_x, page_y, palette_x, palette_y;
    sint32 height, rise, red, green, blue, back_red, back_green, back_blue;
    sint32 uv[8];
    FUNCTION_MARKER(0x80090A38, "GAME.OVL");
    allocation = heap_alloc_back(0x6800);
    w_u32(0x80123F60, allocation);
    w_u32(0x800ED3A8, 1);
    w_u32(0x800ED394, 0);
    game_orient_radial_mesh(x, y, z);
    base_u = r_u32(0x80126B1C);
    base_v = r_u32(0x80126B9C);
    for (sector = 0; sector < 16; ++sector) {
        height = 0;
        rise = 150;
        radius = 64;
        for (radial = 0; radial < 16; ++radial) {
            vertex = 0x800F4658 + sector * 128 + radial * 8;
            product = radius * (uint32)rsin(sector * 0x100);
            w_u16(vertex, (sint32)product >> 12);
            product = radius * (uint32)rcos(sector * 0x100);
            w_u16(vertex + 4, height);
            height += rise;
            rise -= 16;
            w_u16(vertex + 2, (sint32)product >> 12);
            if (rise < 0)
                rise = 0;
            radius += 64;
        }
    }
    for (sector = 0; sector < 16; ++sector) {
        for (radial = 0; radial < 15; ++radial) {
            for (copy = 0; copy < 2; ++copy) {
                vertex = 0x800F4658 + sector * 128 + radial * 8;
                next = 0x800F4658 + ((sector + 1) & 15) * 128 + radial * 8;
                uv[0] = r_s16(vertex) + 1024;
                uv[2] = r_s16(next) + 1024;
                uv[3] = r_s16(next + 2) + 1024;
                allocation = r_u32(0x80123F60);
                uv[4] = r_s16(vertex + 8) + 1024;
                uv[6] = r_s16(next + 8) + 1024;
                uv[7] = r_s16(next + 10) + 1024;
                uv[1] = r_s16(vertex + 2) + 1024;
                uv[5] = r_s16(vertex + 10) + 1024;
                packet = allocation + sector * 0x680 + radial * 0x68 + copy * 0x34;
                uv[0] = game_radial_texture_coordinate((uint32)uv[0]);
                uv[2] = game_radial_texture_coordinate((uint32)uv[2]);
                uv[4] = game_radial_texture_coordinate((uint32)uv[4]);
                uv[6] = game_radial_texture_coordinate((uint32)uv[6]);
                uv[1] = game_radial_texture_coordinate((uint32)uv[1]);
                uv[3] = game_radial_texture_coordinate((uint32)uv[3]);
                uv[5] = game_radial_texture_coordinate((uint32)uv[5]);
                uv[7] = game_radial_texture_coordinate((uint32)uv[7]);
                SetPolyGT4(psx_addr(packet, 8));
                if (radial == 0) {
                    red = green = blue = 0;
                } else {
                    red = (16 - radial) * (111 - radial) / 16;
                    if (red >= 256)
                        red = 255;
                    blue = (16 - radial) * (radial * 16 + 96) / 16;
                    if (blue >= 256)
                        blue = 255;
                    green = (red + blue) >> 1;
                }
                w_u8(packet + 4, red);
                w_u8(packet + 5, green);
                w_u8(packet + 6, blue);
                w_u8(packet + 0x10, red);
                w_u8(packet + 0x11, green);
                w_u8(packet + 0x12, blue);
                if (radial == 14) {
                    back_red = back_green = back_blue = 0;
                } else {
                    back_red = (15 - radial) * (110 - radial) / 16;
                    if (back_red >= 256)
                        back_red = 255;
                    back_blue = (15 - radial) * ((radial + 1) * 16 + 96) / 16;
                    if (back_blue >= 256)
                        back_blue = 255;
                    back_green = (back_red + back_blue) >> 1;
                }
                w_u8(packet + 0x1C, back_red);
                w_u8(packet + 0x1D, back_green);
                w_u8(packet + 0x1E, back_blue);
                w_u8(packet + 0x28, back_red);
                w_u8(packet + 0x29, back_green);
                w_u8(packet + 0x2A, back_blue);
                w_u8(packet + 0xC, base_u + uv[0]);
                w_u8(packet + 0xD, base_v + uv[1]);
                w_u8(packet + 0x18, base_u + uv[2]);
                w_u8(packet + 0x19, base_v + uv[3]);
                w_u8(packet + 0x24, base_u + uv[4]);
                w_u8(packet + 0x25, base_v + uv[5]);
                w_u8(packet + 0x30, base_u + uv[6]);
                w_u8(packet + 0x31, base_v + uv[7]);
                page_x = r_u32(0x800F3CEC);
                page_y = r_u32(0x800F3D94);
                w_u16(packet + 0x1A, GetTPage(0, 1, page_x, page_y));
                palette_x = r_u32(0x800F3244);
                palette_y = r_u32(0x800F32C4);
                w_u16(packet + 0xE, GetClut(palette_x, palette_y));
            }
        }
    }
    for (sector = 0; sector < 16; ++sector) {
        height = 0;
        rise = 150;
        radius = 64;
        for (radial = 0; radial < 16; ++radial) {
            seed = r_u32(0x800EF50C);
            seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
            actual_radius = radius + (seed & 0x7FFF) % (radius >> 4);
            w_u32(0x800EF50C, seed);
            product = actual_radius * (uint32)rsin(sector * 0x100);
            vertex = 0x800F4658 + sector * 128 + radial * 8;
            w_u16(vertex, (sint32)product >> 12);
            product = actual_radius * (uint32)rcos(sector * 0x100);
            w_u16(vertex + 4, height);
            height += rise;
            rise -= 16;
            w_u16(vertex + 2, (sint32)product >> 12);
            if (rise < 0)
                rise = 0;
            radius += 64;
        }
    }
}

void game_clear_model_binding_slots(void)
{
    uint32 offset;
    FUNCTION_MARKER(0x80091210, "GAME.OVL");
    w_u16(0x800FD6F8, 0xFFFF);
    for (offset = 0; offset < 0x21C0; offset += 0x10) {
        w_u16(0x80121D82 + offset, 0xFFFF);
        w_u16(0x80121D80 + offset, 0xFFFF);
    }
}

void game_clear_scene_resource_state(void)
{
    FUNCTION_MARKER(0x80091BA4, "GAME.OVL");
    w_u32(0x800F2B1C, 0);
    w_u32(0x800F2B10, 0);
    w_u32(0x800F2B14, 0);
    w_u32(0x800F2B18, 0);
    w_u32(0x800F2B20, 0);
}

void game_apply_control_rotation(uint32 matrix, uint32 yaw, uint32 pitch, uint32 roll)
{
    MATRIX first, second;
    MATRIX *target;
    FUNCTION_MARKER(0x80092808, "GAME.OVL");
    if (yaw == 0 && pitch == 0 && roll == 0)
        return;
    target = (MATRIX *)psx_addr(matrix, 20);
    if (yaw != 0) {
        if (pitch != 0) {
            matrix_rotate_y_before_native(&first, target, yaw);
            if (roll != 0) {
                matrix_rotate_x_before_native(&second, &first, pitch);
                matrix_rotate_z_before_native(target, &second, roll);
            } else {
                matrix_rotate_x_before_native(target, &first, pitch);
            }
        } else {
            matrix_rotate_y_before_native(&first, target, yaw);
            if (roll != 0)
                matrix_rotate_z_before_native(target, &first, roll);
            else
                matrix_copy_rotation_native(matrix, &first);
        }
    } else if (pitch != 0) {
        if (roll != 0) {
            matrix_rotate_x_before_native(&first, target, pitch);
            matrix_rotate_z_before_native(target, &first, roll);
        } else {
            matrix_rotate_x_before_native(target, target, pitch);
        }
    } else {
        matrix_rotate_z_before_native(&first, target, roll);
        matrix_copy_rotation_native(matrix, &first);
    }
}

static void game_track_control_axis(uint32 current_address, uint32 accumulator_address, uint32 target, uint32 step,
    uint32 reduction_address, uint32 accumulator_step, sint32 limit, uint32 third_axis)
{
    uint32 value, accumulator, reduced, reduction;
    if (target == 0) {
        value = r_u32(current_address);
        if (third_axis != 0) {
            accumulator = r_u32(accumulator_address);
            reduction = r_u32(reduction_address);
            w_u32(accumulator_address, accumulator - (uint32)((sint32)accumulator >> 3));
            reduced = (uint32)game_reduce_without_sign_change((sint32)value, reduction);
            w_u32(current_address, reduced);
        } else {
            reduction = r_u32(reduction_address);
            reduced = (uint32)game_reduce_without_sign_change((sint32)value, reduction);
            accumulator = r_u32(accumulator_address);
            w_u32(current_address, reduced);
            w_u32(accumulator_address, accumulator - (uint32)((sint32)accumulator >> 4));
        }
        return;
    }
    value = r_u32(current_address);
    if ((sint32)value < (sint32)target) {
        accumulator = r_u32(accumulator_address);
        if ((sint32)accumulator > -limit) {
            accumulator -= accumulator_step;
            w_u32(accumulator_address, accumulator);
            if ((sint32)accumulator < -limit)
                w_u32(accumulator_address, (uint32)-limit);
        }
        value = r_u32(current_address) + step;
        w_u32(current_address, value);
        if ((sint32)target < (sint32)value)
            w_u32(current_address, target);
    } else if ((sint32)target < (sint32)value) {
        accumulator = r_u32(accumulator_address);
        if ((sint32)accumulator < limit) {
            accumulator += accumulator_step;
            w_u32(accumulator_address, accumulator);
            if ((sint32)accumulator > limit)
                w_u32(accumulator_address, (uint32)limit);
        }
        value = r_u32(current_address) - step;
        w_u32(current_address, value);
        if ((sint32)value < (sint32)target)
            w_u32(current_address, target);
    }
}

void game_update_flight_controls(uint32 object)
{
    uint32 mode, intensity, value, product, boost, yaw, pitch, roll, yaw_step, pitch_step, roll_step;
    uint32 thrust, thrust_target, thrust_step, bound, current, camera, first_input, second_input;
    sint32 first_angle, second_angle, third_angle;
    mode = r_u32(0x800313DC);
    FUNCTION_MARKER(0x80091BDC, "GAME.OVL");
    if ((sint32)mode < 4)
        w_u32(0x800313DC, 3);
    if (r_u32(0x800311DC) == 0) {
        if ((r_u8(0x80040F33) != 0 || r_u32(0x800ED430) != 0) &&
            (r_u8(0x80040F34) != 0 || r_u32(0x800ED434) != 0)) {
            w_u8(0x80040F33, 0);
            w_u8(0x80040F34, 0);
            w_u32(0x800311DC, 1);
        }
    } else if (r_u8(0x80040F2C) == 0 || r_u8(0x80040F2D) == 0) {
        w_u32(0x800311DC, 0);
    }
    if (r_u32(0x800EC884) == 0) {
        intensity = r_u8(0x80040F20);
        if (intensity != 0) {
            product = r_u32(0x800F2B40) * intensity;
            yaw = (uint32)((sint32)(0 - product) >> 7);
            product = r_u32(0x800F2B3C) * intensity;
            value = r_u32(0x800F2B10);
            yaw_step = (uint32)((sint32)product >> 7);
            if ((sint32)value > 0) {
                boost = r_u32(0x800F2B44) * intensity;
                yaw_step += (uint32)((sint32)boost >> 8);
            }
        } else {
            intensity = r_u8(0x80040F21);
            yaw = yaw_step = 0;
            if (intensity != 0) {
                product = r_u32(0x800F2B40) * intensity;
                yaw = (uint32)((sint32)product >> 7);
                product = r_u32(0x800F2B3C) * intensity;
                value = r_u32(0x800F2B10);
                yaw_step = (uint32)((sint32)product >> 7);
                if ((sint32)value < 0) {
                    boost = r_u32(0x800F2B44) * intensity;
                    yaw_step += (uint32)((sint32)boost >> 8);
                }
            }
        }
        intensity = r_u8(0x80040F22);
        if (intensity != 0) {
            product = r_u32(0x800F2B4C) * intensity;
            pitch = (uint32)((sint32)product >> 7);
            product = r_u32(0x800F2B48) * intensity;
            value = r_u32(0x800F2B14);
            pitch_step = (uint32)((sint32)product >> 7);
            if ((sint32)value < 0) {
                boost = r_u32(0x800F2B50) * intensity;
                pitch_step += (uint32)((sint32)boost >> 8);
            }
        } else {
            intensity = r_u8(0x80040F23);
            pitch = pitch_step = 0;
            if (intensity != 0) {
                product = r_u32(0x800F2B4C) * intensity;
                pitch = (uint32)((sint32)(0 - product) >> 7);
                product = r_u32(0x800F2B48) * intensity;
                value = r_u32(0x800F2B14);
                pitch_step = (uint32)((sint32)product >> 7);
                if ((sint32)value > 0) {
                    boost = r_u32(0x800F2B50) * intensity;
                    pitch_step += (uint32)((sint32)boost >> 8);
                }
            }
        }
        intensity = r_u8(0x80040F24);
        if (intensity != 0 && r_u32(0x800311DC) == 0) {
            product = r_u32(0x800F2B58) * intensity;
            roll = (uint32)((sint32)(0 - product) >> 7);
            product = r_u32(0x800F2B54) * intensity;
            value = r_u32(0x800F2B18);
            roll_step = (uint32)((sint32)product >> 7);
            if ((sint32)value > 0) {
                boost = r_u32(0x800F2B5C) * intensity;
                roll_step += (uint32)((sint32)boost >> 8);
            }
        } else {
            intensity = r_u8(0x80040F25);
            roll = roll_step = 0;
            if (intensity != 0 && r_u32(0x800311DC) == 0) {
                product = r_u32(0x800F2B58) * intensity;
                roll = (uint32)((sint32)product >> 7);
                product = r_u32(0x800F2B54) * intensity;
                value = r_u32(0x800F2B18);
                roll_step = (uint32)((sint32)product >> 7);
                if ((sint32)value < 0) {
                    boost = r_u32(0x800F2B5C) * intensity;
                    roll_step += (uint32)((sint32)boost >> 8);
                }
            }
        }
        value = r_u32(0x800EF4E4);
        thrust = r_u32(0x800F2B24);
        if (value != 0)
            thrust = (uint32)((sint32)thrust >> 1);
        intensity = r_u8(0x80040F27);
        if (intensity != 0) {
            product = thrust * intensity;
            boost = r_u32(0x800F2B28) * intensity;
            thrust_target = (uint32)((sint32)product >> 7);
            thrust_step = (uint32)((sint32)boost >> 7);
        } else {
            intensity = r_u8(0x80040F26);
            thrust_target = thrust_step = 0;
            if (intensity != 0) {
                value = (uint32)((sint32)((thrust << 1) + thrust) >> 2);
                product = value * intensity;
                boost = r_u32(0x800F2B28) * intensity;
                thrust_target = (uint32)((sint32)(0 - product) >> 7);
                thrust_step = (uint32)((sint32)boost >> 7);
            }
        }
        game_track_control_axis(0x800F2B10, 0x800ECE78, yaw, yaw_step, 0x800F2B44, 0x400, 0x2000, 0);
        game_track_control_axis(0x800F2B14, 0x800ECE74, pitch, pitch_step, 0x800F2B50, 0x300, 0x1800, 0);
        if (roll != 0 && r_u8(0x80040F24) != 0 && r_u8(0x80040F25) != 0)
            roll = 0;
        game_track_control_axis(0x800F2B18, 0x800ECE7C, roll, roll_step, 0x800F2B5C, 0x1000, 0x8000, 1);
        if (thrust_target == 0) {
            w_u32(0x800F2B1C, 0);
        } else {
            current = r_u32(0x800F2B1C);
            if ((sint32)thrust_target >= (sint32)current) {
                current += thrust_step;
                w_u32(0x800F2B1C, current);
                if ((sint32)thrust_target < (sint32)current)
                    w_u32(0x800F2B1C, thrust_target);
            } else if ((sint32)thrust_target < 0) {
                if ((sint32)current > 0)
                    w_u32(0x800F2B1C, 0);
                current = r_u32(0x800F2B1C);
                if ((sint32)thrust_target < (sint32)current)
                    w_u32(0x800F2B1C, current - thrust_step);
            }
        }
        thrust = r_u32(0x800F2B1C);
        value = r_u32(0x800F2B40);
        current = r_u32(0x800F2B10);
        w_u32(0x800ECE84, (sint32)thrust >> 11);
        boost = (uint32)((sint32)thrust >> 5);
        bound = value - boost;
        if ((sint32)bound < (sint32)current)
            w_u32(0x800F2B10, bound);
        else {
            bound = boost - value;
            if ((sint32)current < (sint32)bound)
                w_u32(0x800F2B10, bound);
        }
        thrust = r_u32(0x800F2B1C);
        value = r_u32(0x800F2B4C);
        current = r_u32(0x800F2B14);
        boost = (uint32)((sint32)thrust >> 5);
        bound = value - boost;
        if ((sint32)bound < (sint32)current)
            w_u32(0x800F2B14, bound);
        else {
            bound = boost - value;
            if ((sint32)current < (sint32)bound)
                w_u32(0x800F2B14, bound);
        }
    }
    first_angle = integer_shift_away(r_s32(0x800F2B10), 8);
    second_angle = integer_shift_away(r_s32(0x800F2B14), 8);
    third_angle = integer_shift_away(r_s32(0x800F2B18), 8);
    game_apply_control_rotation(object + 0x11C, first_angle, second_angle, third_angle);
    if (r_u32(0x800F2B10) != 0 || r_u32(0x800F2B14) != 0 || r_u32(0x800F2B18) != 0)
        matrix_normalize(object + 0x11C, 1);
    object_extract_angles(object, object);
    if (r_s32(0x800313DC) < 4) {
        if (r_u32(0x800311DC) != 0) {
            matrix_rotate_y_before(0x8004EAF8, object + 0x11C, 0x800);
            matrix_copy_rotation(0x80035060, 0x8004EAF8);
        } else {
            matrix_copy_rotation(0x80035060, object + 0x11C);
        }
    }
    camera = r_u32(0x80042174);
    matrix_copy_rotation(camera + 0x11C, 0x80035060);
    first_angle = r_s16(0x80035060) * 8;
    second_angle = r_s16(0x80035062) * 8;
    third_angle = r_s16(0x80035064) * 8;
    w_u16(0x80035060, first_angle / 5);
    w_u16(0x80035062, second_angle / 5);
    w_u16(0x80035064, third_angle / 5);
    thrust = r_u32(0x800F2B1C);
    value = (uint32)r_s16(object + 0x128);
    product = value * (uint32)((sint32)thrust >> 8);
    w_u32(object + 0xFC, (sint32)product >> 12);
    thrust = r_u32(0x800F2B1C);
    value = (uint32)r_s16(object + 0x12A);
    product = value * (uint32)((sint32)thrust >> 8);
    w_u32(object + 0x100, (sint32)product >> 12);
    thrust = r_u32(0x800F2B1C);
    value = (uint32)r_s16(object + 0x12C);
    product = value * (uint32)((sint32)thrust >> 8);
    w_u32(object + 0x104, (sint32)product >> 12);
    w_u32(object + 0xEC, r_u32(0x800F2B1C));
    game_update_object_movement(object);
    if (r_s32(0x800313DC) >= 4)
        w_u32(0x80040C94, r_u32(0x80042174));
    else
        w_u32(0x80040C94, object);
    game_update_engine_audio(0x800F2B10);
    camera_dispatch();
    first_input = r_u8(0x80040F33);
    second_input = r_u8(0x80040F34);
    w_u32(0x800ED430, first_input);
    w_u32(0x800ED434, second_input);
}

void game_update_engine_audio(uint32 controls)
{
    uint32 thrust, limit, level, value, product, voice, numerator, divisor, magnitude;
    uint32 adjust_volume;
    FUNCTION_MARKER(0x800E0090, "GAME.OVL");
    thrust = r_u32(controls + 0xC);
    limit = r_u32(controls + 0x14);
    adjust_volume = 0;
    if ((sint32)limit < (sint32)thrust) {
        level = r_u32(0x8010AC48);
        if ((sint32)level < 0x50) {
            level += 0x10;
            w_u32(0x8010AC48, level);
            if ((sint32)level > 0x50)
                w_u32(0x8010AC48, 0x50);
            adjust_volume = 1;
        }
    } else {
        level = r_u32(0x8010AC48);
        if ((sint32)level > 0) {
            level -= 0x10;
            w_u32(0x8010AC48, level);
            if ((sint32)level < 0)
                w_u32(0x8010AC48, 0);
            adjust_volume = 1;
        }
    }
    if (adjust_volume != 0) {
        if (r_u32(0x800EC884) != 0 || r_u32(0x800EC868) != 0)
            w_u32(0x8010AC48, 0);
        value = (uint32)r_s16(0x800118FC);
        level = r_u32(0x8010AC48);
        product = level * value;
        voice = r_u32(0x800FB438);
        sound_set_voice_volume(voice, (uint32)((sint32)product >> 8) & 0xFF);
    }
    divisor = r_u32(controls + 0x14);
    if (divisor == 0)
        divisor = 1;
    thrust = r_u32(controls + 0xC);
    limit = r_u32(controls + 0x14);
    magnitude = (sint32)thrust < 0 ? 0 - thrust : thrust;
    if ((sint32)limit < (sint32)magnitude)
        magnitude = limit;
    numerator = ((((magnitude << 1) + magnitude) << 3) + magnitude) << 2;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(0x5D);
    level = (uint32)((sint32)numerator / (sint32)divisor) + 0x50;
    if ((sint32)level >= 0x100)
        level = 0xFF;
    if (r_u32(0x800EC884) != 0 || r_u32(0x800EC868) != 0)
        level = 0;
    if (level != r_u32(0x80108A20)) {
        voice = r_u32(0x800F8100);
        sound_set_voice_pitch(voice, level + 0x64);
        value = (uint32)r_s16(0x800118FC);
        product = level * value;
        voice = r_u32(0x800F8100);
        sound_set_voice_volume(voice, (uint32)((sint32)product >> 8) & 0xFF);
        w_u32(0x80108A20, level);
    }
}

uint32 game_get_controller_state(void)
{
    FUNCTION_MARKER(0x80092B24, "GAME.OVL");
    return 0x80040CA0;
}

void game_initialize_direction_indicator(void)
{
    FUNCTION_MARKER(0x80092EEC, "GAME.OVL");
    if (r_s16(0x800595F0) != 0)
        return;
    w_u16(0x80123F78, 0xF000);
    w_u32(0x80123F80, 0xFFFFF000);
    w_u16(0x80123F92, 0x40);
    w_u16(0x80123F7A, 0);
    w_u32(0x80123F84, 0);
    w_u16(0x80123F7C, 0);
    w_u32(0x80123F88, 0);
    w_u16(0x80123F90, 0);
    w_u16(0x800595F0, 1);
}

void game_config_select_ship(const uint32 *arguments, uint32 first)
{
    uint32 text, length, identifier, offset;
    FUNCTION_MARKER(0x80093274, "GAME.OVL");
    text = arguments[first + 1];
    length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    identifier = (uint32)string_to_integer((const char *)psx_addr(text, (size_t)length + 1));
    w_u32(0x8012401C, identifier);
    if ((sint32)identifier >= 20)
        diagnostic_report(0x29A, 0x80074F64, 0x80074F80);
    identifier = r_u32(0x8012401C);
    w_u32(0x800ED490, 0);
    offset = ((identifier << 4) - identifier) << 2;
    w_u16(0x800536BA + offset, 0);
    w_u16(0x800536BE + offset, 0);
    w_u16(0x800536C0 + offset, 0);
}

void game_config_set_ship_model(const uint32 *arguments, uint32 first)
{
    char path[128];
    const char *name;
    uint32 identifier, offset, text, length, model, table, index, gate_model;
    FUNCTION_MARKER(0x80093318, "GAME.OVL");
    identifier = r_u32(0x8012401C);
    w_u32(0x800ED468, 0xFFFFFFFF);
    text = arguments[first + 1];
    length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    name = (const char *)psx_addr(text, (size_t)length + 1);
    offset = ((identifier << 4) - identifier) << 2;
    strcpy((char *)psx_addr(0x800536C6 + offset, (size_t)length + 1), name);
    if (strcmp(name, "NESCAPE") == 0 || strcmp(name, "LESCAPE") == 0) {
        identifier = r_u32(0x8012401C);
        w_u32(0x800ED468, 7);
        offset = ((identifier << 4) - identifier) << 2;
        w_u16(0x80059614, identifier);
        w_u32(0x80053698 + offset, 7);
    }
    if (strcmp(name, "BIGAST") == 0)
        w_u32(0x800ED468, 6);
    if (strcmp(name, "NMINING") == 0)
        w_u16(0x80059618, r_u32(0x8012401C));
    if (strcmp(name, "WARPGATE") == 0) {
        identifier = r_u32(0x8012401C);
        w_u32(0x800ED468, 0);
        offset = ((identifier << 4) - identifier) << 2;
        gate_model = r_u32(0x800ED498);
        w_u16(0x800536B8 + offset, gate_model);
        return;
    }
    if (strcmp(name, "JUMPGATE") == 0) {
        identifier = r_u32(0x8012401C);
        w_u32(0x800ED468, 0);
        offset = ((identifier << 4) - identifier) << 2;
        gate_model = r_u32(0x800ED494);
        w_u16(0x800536B8 + offset, gate_model);
        return;
    }
    if (r_u32(0x800EDCF0) == 0 && game_find_model_name(text) == 0) {
        strcpy(path, name);
        strcat(path, ".BND");
        game_load_bound_bundle_native(path);
    }
    text = arguments[first + 1];
    model = game_find_model_name(text);
    identifier = r_u32(0x8012401C);
    offset = ((identifier << 4) - identifier) << 2;
    w_u16(0x800536B8 + offset, model);
    if (identifier != 0)
        return;
    text = arguments[first + 1];
    w_u32(0x800ECBD0, 0);
    length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    strcpy(path, (const char *)psx_addr(text, (size_t)length + 1));
    table = 0x800ECBD4;
    for (index = 0; index < 10; ++index) {
        length = 0;
        while (r_u8(table + length) != 0)
            ++length;
        if (strcmp(path, (const char *)psx_addr(table, (size_t)length + 1)) == 0) {
            w_u32(0x800ECBD0, index);
            break;
        }
        table += 0x2C;
    }
}

void game_config_set_ship_type(const uint32 *arguments, uint32 first)
{
    uint32 type, identifier, offset, text, length;
    FUNCTION_MARKER(0x800935D4, "GAME.OVL");
    type = r_u32(0x800ED468);
    if (type != 0xFFFFFFFF) {
        identifier = r_u32(0x8012401C);
    } else {
        text = arguments[first + 1];
        length = 0;
        while (r_u8(text + length) != 0)
            ++length;
        type = (uint32)string_to_integer((const char *)psx_addr(text, (size_t)length + 1));
        identifier = r_u32(0x8012401C);
    }
    offset = ((identifier << 4) - identifier) << 2;
    w_u32(0x80053698 + offset, type);
}

void game_update_stream_callback(void)
{
    uint32 active;
    active = r_u32(0x800ED43C);
    FUNCTION_MARKER(0x80092B3C, "GAME.OVL");
    if (active != 0)
        return;
    w_u32(0x800ED43C, 1);
    stream_update();
    w_u32(0x800ED43C, 0);
}

void game_initialize_effect_pool(void)
{
    static const uint32 divisor_addresses[6] = {0x800EF184, 0x800EF1AC, 0x800EF15C, 0x800EF148, 0x800EF170, 0x800EF134};
    sint32 quotient[6], divisor;
    uint32 node, previous, index, table;
    FUNCTION_MARKER(0x800AFFA4, "GAME.OVL");
    node = 0x800FE340;
    previous = 0;
    while (node < 0x801075F8) {
        w_u32(node + 0x4C, previous);
        previous = node;
        w_u16(node + 0x3C, 0);
        w_u32(node + 0x50, 0);
        w_u32(node + 0x54, 0);
        w_u32(node + 0x48, node + 0x78);
        node += 0x78;
    }
    w_u32(node + 0x4C, previous);
    w_u32(node + 0x48, 0);
    w_u32(0x800FB408, 0x800FE340);
    w_u32(0x80124928, 0);
    table = 0x800F3394;
    for (index = 0; index < 10; ++index) {
        w_u32(table, 0);
        table -= 4;
    }
    for (index = 0; index < 6; ++index) {
        divisor = r_s32(divisor_addresses[index]);
        if (divisor == 0)
            xport_mips_break(7);
        quotient[index] = 150000 / divisor;
    }
    w_u32(0x800EF110, 0);
    w_u32(0x800EF18C, quotient[0]);
    w_u32(0x800EF1B4, quotient[1]);
    w_u32(0x800EF164, quotient[2]);
    w_u32(0x800EF18C, quotient[0]);
    w_u32(0x800EF1B4, quotient[1]);
    w_u32(0x800EF150, quotient[3]);
    w_u32(0x800EF178, quotient[4]);
    w_u32(0x800EF13C, quotient[5]);
}

void game_clear_scene_effect_flags(void)
{
    sint32 offset;
    FUNCTION_MARKER(0x800DE6E4, "GAME.OVL");
    for (offset = 0x64C; offset >= 0; offset -= 0x34)
        w_u8(0x801094E8 + (uint32)offset, 0);
}

void game_clear_scene_link_slots(void)
{
    uint32 index, offset;
    FUNCTION_MARKER(0x8008CCA4, "GAME.OVL");
    w_u32(0x800ED390, 0);
    offset = 0;
    for (index = 0; index < 64; ++index) {
        w_u32(0x80124030 + offset, 0);
        w_u32(0x80124040 + offset, 0);
        offset += 0x24;
    }
}

void game_clear_auxiliary_effect_slots(void)
{
    sint32 offset;
    FUNCTION_MARKER(0x800BC9F0, "GAME.OVL");
    for (offset = 0x1B0; offset >= 0; offset -= 0x30)
        w_u32(0x801077F4 + (uint32)offset, 0);
    for (offset = 0x1B0; offset >= 0; offset -= 0x30)
        w_u32(0x800FD53C + (uint32)offset, 0);
}

void game_clear_particle_records(void)
{
    uint32 index, offset, record, fresh_base;
    FUNCTION_MARKER(0x800AFEA0, "GAME.OVL");
    offset = 0;
    for (index = 0; index < 0x1C2; ++index) {
        record = r_u32(0x80123F64) + offset;
        w_u8(record, 0);
        fresh_base = r_u32(0x80123F64);
        w_u32(record + 8, 0);
        w_u32(record + 0xC, 0);
        w_u32(record + 0x10, 0);
        w_u16(record + 2, 0);
        w_u16(record + 4, 0);
        w_u16(record + 6, 0);
        w_u32(record + 0x18, 0);
        w_u8(fresh_base + offset + 1, 0);
        offset += 0x1C;
    }
}

void game_clear_ship_effect_counters(void)
{
    sint32 offset;
    FUNCTION_MARKER(0x80087458, "GAME.OVL");
    w_u16(0x800F3C68, 0);
    w_u16(0x800F3C6A, 0);
    w_u16(0x800F3C6C, 0);
    for (offset = 0x16E; offset >= 0; offset -= 0x7A)
        w_u16(0x80126928 + (uint32)offset, 0);
}

void game_initialize_local_starfield(void)
{
    uint32 index, offset, value, state;
    FUNCTION_MARKER(0x8008C5CC, "GAME.OVL");
    game_initialize_spherical_starfield();
    offset = 0;
    for (index = 0; index < 0x159; ++index) {
        value = (uint32)random_between(-0x4000, 0x4000);
        w_u16(0x80116560 + offset, value);
        value = (uint32)random_between(-0x4000, 0x4000);
        w_u16(0x80116562 + offset, value);
        value = (uint32)random_between(-0x4000, 0x4000);
        w_u16(0x80116564 + offset, value);
        value = (uint32)random_between(0, 2);
        state = r_u32(0x800EF50C);
        w_u16(0x80116570 + offset, value);
        state = (uint32)((sint32)state >> 1) ^ (state << 3);
        w_u32(0x800EF50C, state);
        value = (uint32)random_between(0x50, 0xFF);
        w_u32(0x80116568 + offset, 0x68000000 + (value << 16) + (value << 8) + value);
        offset += 0x14;
    }
}

void game_initialize_spherical_starfield(void)
{
    MATRIX matrix = {0};
    SVECTOR angles = {0}, point = {0};
    VECTOR output = {0};
    uint32 index, address, state, first_state, second_state, squared;
    sint32 x, y, z, length, azimuth, elevation, flags;
    FUNCTION_MARKER(0x8008CB0C, "GAME.OVL");
    SetTransMatrix(&matrix);
    address = 0x800F4224;
    for (index = 0; index < 128; ++index) {
        w_u32(address, 0);
        address -= 4;
    }
    w_u32(0x800ED38C, 0);
    for (index = 0; index < 0x38E; ++index) {
        state = r_u32(0x800EF50C);
        first_state = (uint32)((sint32)state >> 1) ^ (state << 3);
        x = (sint32)(first_state & 0xFFF) - 0x800;
        second_state = (uint32)((sint32)first_state >> 1) ^ (first_state << 3);
        state = (uint32)((sint32)second_state >> 1) ^ (second_state << 3);
        y = (sint32)(state & 0xFFF) - 0x800;
        z = (sint32)(second_state & 0xFFF) - 0x800;
        squared = (uint32)x * (uint32)x + (uint32)y * (uint32)y;
        w_u32(0x800EF50C, state);
        length = (sint32)SquareRoot0((sint32)squared);
        azimuth = ratan2(x, y);
        elevation = ratan2(length, z);
        angles.vx = 0;
        angles.vy = (sint16)azimuth;
        angles.vz = 0;
        RotMatrixYXZ(&angles, &matrix);
        SetRotMatrix(&matrix);
        point.vx = 0;
        point.vy = (sint16)rcos(elevation);
        point.vz = (sint16)rsin(elevation);
        RotTrans(&point, &output, &flags);
        game_insert_spherical_star(output.vx, output.vy, output.vz, elevation, azimuth);
    }
}

void game_insert_spherical_star(uint32 x, uint32 y, uint32 z, uint32 elevation, uint32 azimuth)
{
    uint32 band, column, cell, offset, record, head, tail, next;
    uint32 state, first_state, second_state, third_state, jitter_x, jitter_y;
    FUNCTION_MARKER(0x8008C808, "GAME.OVL");
    elevation = (elevation + 0x400) & 0x7FF;
    band = elevation >> 8;
    column = (azimuth & 0xFFF) >> 8;
    offset = r_u32(0x800ED38C) * 12;
    cell = 0x800F4028 + ((band * 16 + column) << 2);
    record = 0x8011F2C0 + offset;
    w_u16(record, x);
    w_u16(record + 2, y);
    w_u16(record + 4, z);
    w_u32(record + 8, 0);
    head = r_u32(cell);
    if (head == 0) {
        w_u32(cell, record);
        column = ((0u - azimuth) & 0xFFF) >> 8;
        w_u32(0x80118A88 + ((band * 16 + column) << 2), record);
        band = ((0x800 - elevation) & 0x7FF) >> 8;
        column = (azimuth & 0xFFF) >> 8;
        w_u32(0x80118C88 + ((band * 16 + column) << 2), record);
    } else {
        jitter_x = r_u16(record);
        state = r_u32(0x800EF50C);
        jitter_y = r_u16(record + 2);
        first_state = (uint32)((sint32)state >> 1) ^ (state << 3);
        jitter_x = jitter_x - 0x200 + (first_state & 0x3FF);
        second_state = (uint32)((sint32)first_state >> 1) ^ (first_state << 3);
        jitter_y = jitter_y - 0x200 + (second_state & 0x3FF);
        w_u16(record + 2, jitter_y);
        third_state = (uint32)((sint32)second_state >> 1) ^ (second_state << 3);
        w_u16(record, jitter_x);
        z = r_u16(record + 4);
        z = z - 0x200 + (third_state & 0x3FF);
        w_u16(record + 4, z);
        tail = r_u32(cell);
        next = r_u32(tail + 8);
        w_u32(0x800EF50C, first_state);
        w_u32(0x800EF50C, second_state);
        w_u32(0x800EF50C, third_state);
        while (next != 0) {
            tail = r_u32(tail + 8);
            next = r_u32(tail + 8);
        }
        w_u32(tail + 8, 0x8011F2C0 + r_u32(0x800ED38C) * 12);
    }
    w_u32(0x800ED38C, r_u32(0x800ED38C) + 1);
}

static sint32 anim_movie_source_frame(void *context, uint32 number)
{
    w_u16(0x80091934, number);
    return 1;
}

sint32 anim_play_movie(uint32 path)
{
    char logical_path[64];
    uint32 index;
    const char *runtime_path;
    XportFmvPresentation presentation;
    sint32 result;
    if (path == 0)
        return 0;
    for (index = 0; index < sizeof(logical_path); ++index) {
        logical_path[index] = (char)r_u8(path + index);
        if (logical_path[index] == 0)
            break;
    }
    if (index == sizeof(logical_path) || index == 0)
        return 0;
    runtime_path = game_movie_resolve_path(logical_path);
    if (runtime_path == NULL)
        return 0;
    w_u16(0x8009191C, 1);
    w_u32(0x80091904, 448);
    w_u16(0x80091918, 240);
    w_u16(0x80091908, 320);
    w_u32(0x80091924, 0);
    w_u16(0x80091934, 0);
    memset(&presentation, 0, sizeof(presentation));
    presentation.width = 320;
    presentation.height = 240;
    presentation.video_y = 24;
    presentation.controllers = 1;
    presentation.skip_buttons = PADstart | PADRdown;
    presentation.frame = anim_movie_source_frame;
    result = xport_fmv_play_presented(runtime_path, 0, 0, &presentation);
    if (!game_mount_stream_source())
        result = -1;
    if (result < 0)
        return 0;
    w_u32(0x80091924, result == 2 ? 2 : 1);
    return 2;
}
