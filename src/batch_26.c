#include "game_functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void game_load_scene_texture_page(void)
{
    uint32 buffer, length, count;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x800D164C, "GAME.OVL");
    tex_reset_regions();
    buffer = heap_alloc_back(0x40000);
    length = 0;
    while (r_u8(0x80010536 + length) != 0)
        ++length;
    count = resource_load_into((const char *)psx_addr(0x80010536, length + 1), 0x8004EB70) >> 4;
    if (count > 600)
        diagnostic_report(count, 0x80075F80, 0x80075F94);
    length = 0;
    while (r_u8(0x80010527 + length) != 0)
        ++length;
    resource_load_into((const char *)psx_addr(0x80010527, length + 1), buffer);
    rectangle.x = 512;
    rectangle.y = 256;
    rectangle.w = 512;
    rectangle.h = 256;
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(buffer, 0x40000));
    DrawSync(0);
    heap_free(buffer);
}

void game_parse_level_numbers(uint32 text)
{
    char digits[32];
    uint32 source_index, count, value;
    FUNCTION_MARKER(0x800D196C, "GAME.OVL");
    source_index = 0;
    count = 0;
    value = r_u8(text);
    while (value - 0x30 < 10) {
        digits[count++] = (char)r_u8(text + source_index);
        ++source_index;
        value = r_u8(text + source_index);
    }
    digits[count] = 0;
    w_u16(0x800595F8, (uint32)atoi(digits));
    ++source_index;
    count = 0;
    value = r_u8(text + source_index);
    while (value - 0x30 < 10) {
        digits[count++] = (char)r_u8(text + source_index);
        ++source_index;
        value = r_u8(text + source_index);
    }
    digits[count] = 0;
    w_u16(0x800595FA, (uint32)atoi(digits));
    w_u16(0x800595FC, 0);
    w_u16(0x800595FE, 0);
}

void game_initialize_fragment_slots(void)
{
    uint32 index, state;
    FUNCTION_MARKER(0x800BFB40, "GAME.OVL");
    for (index = 0; index < 16; ++index) {
        state = r_u32(0x800EF50C);
        state = (uint32)((sint32)state >> 1) ^ (state << 3);
        w_u32(0x800EF50C, state);
        state = (uint32)((sint32)state >> 1) ^ (state << 3);
        w_u32(0x800EF50C, state);
        state = (uint32)((sint32)state >> 1) ^ (state << 3);
        w_u32(0x800EF50C, state);
        state = (uint32)((sint32)state >> 1) ^ (state << 3);
        w_u32(0x800EF50C, state);
        state = (uint32)((sint32)state >> 1) ^ (state << 3);
        w_u32(0x800EF50C, state);
        state = (uint32)((sint32)state >> 1) ^ (state << 3);
        w_u32(0x800EF50C, state);
    }
    for (index = 0; index < 256; ++index) {
        w_u16(0x8011A3E8 + index * 0x4C, 0);
        w_u16(0x8011A3EC + index * 0x4C, 0);
    }
}

void game_initialize_scene_sky(void)
{
    sint32 level;
    uint32 seed, color, second_color;
    FUNCTION_MARKER(0x800E42AC, "GAME.OVL");
    level = r_s16(0x800595F8);
    if ((uint32)level <= 2) {
        seed = 0x44AF5EF7;
        color = 0xFF;
        second_color = 0x80;
    } else if ((uint32)level <= 4) {
        seed = 0x34EE9A9B;
        color = 0xFF;
        second_color = 0xFF0000;
    } else if ((uint32)level <= 8) {
        seed = 0xAAF44639;
        color = 0xFF8000;
        second_color = 0x804000;
    } else if ((uint32)level <= 12) {
        seed = 0xAC30FA53;
        color = 0xFFFFFF;
        second_color = 0x808080;
    } else {
        seed = 0x3FEE00E6;
        color = 0xE03000;
        second_color = 0x800030;
    }
    game_generate_scene_sky(seed, color, second_color, 0, 0);
}

void game_initialize_sky_geometry(void)
{
    uint32 row, column, offset, sine, cosine, cell, current, next;
    FUNCTION_MARKER(0x800E27A4, "GAME.OVL");
    for (row = 0; row < 2; ++row) {
        for (column = 0; column < 5; ++column) {
            current = heap_alloc_back(0x1200);
            w_u32(0x801193C8 + row * 0x14 + column * 4, current);
        }
    }
    for (column = 0; column < 32; ++column) {
        sine = (uint32)rsin(column << 7);
        cosine = (uint32)rcos(column << 7);
        offset = column * 8;
        w_u16(0x80118EC8 + offset, (sine * 375) >> 9);
        w_u16(0x80118ECA + offset, (cosine * 375) >> 9);
        w_u16(0x80118FC8 + offset, (sine * 875) >> 10);
        w_u16(0x80118FCA + offset, (cosine * 875) >> 10);
        w_u16(0x801191C8 + offset, (sine * 1125) >> 10);
        w_u16(0x801191CA + offset, (cosine * 1125) >> 10);
        w_u16(0x801192C8 + offset, (sine * 675) >> 9);
        w_u16(0x80118ECC + offset, 0);
        w_u16(0x80118FCC + offset, 0);
        w_u16(0x801190C8 + offset, (sine * 525) >> 9);
        w_u16(0x801190CA + offset, (cosine * 525) >> 9);
        w_u16(0x801190CC + offset, 0);
        w_u16(0x801191CC + offset, 0);
        w_u16(0x801192CA + offset, (cosine * 675) >> 9);
        w_u16(0x801192CC + offset, 0);
    }
    for (column = 0; column < 32; ++column) {
        next = (column + 1) & 31;
        for (row = 0; row < 3; ++row) {
            cell = 0x80118268 + (row * 32 + column) * 16;
            current = 0x1F800000 + (row * 32 + column) * 4;
            w_u32(cell, current);
            w_u32(cell + 4, 0x1F800000 + (row * 32 + next) * 4);
            w_u32(cell + 8, current + 128);
            w_u32(cell + 12, 0x1F800000 + ((row + 1) * 32 + next) * 4);
        }
        cell = 0x80118268 + (96 + column) * 16;
        w_u32(cell + 4, 0x1F800000 + (96 + next) * 4);
        w_u32(cell, 0x1F800000 + (96 + column) * 4);
        w_u32(cell + 8, 0x1F800000 + (128 + column) * 4);
        w_u32(cell + 12, 0x1F800000 + (128 + next) * 4);
    }
}

void game_initialize_sky_resources(void)
{
    FUNCTION_MARKER(0x800E2DA8, "GAME.OVL");
    w_u32(0x800F15E8, 0);
    game_initialize_sky_geometry();
}

void game_position_attached_effect(uint32 object, uint32 effect, uint32 position, uint32 anchor, uint32 distance)
{
    VECTOR relative, rotated_anchor;
    uint32 x, y, z;
    MATRIX *matrix;
    FUNCTION_MARKER(0x800DFD64, "GAME.OVL");
    relative.vx = r_s32(position);
    relative.vy = r_s32(position + 4);
    z = r_u32(position + 8);
    distance <<= r_u32(object + 0x84) & 31;
    relative.vz = (sint32)(z - distance);
    matrix = (MATRIX *)psx_addr(object + 0x11C, 20);
    ApplyMatrixLV(matrix, &relative, &relative);
    ApplyMatrixLV(matrix, (VECTOR *)psx_addr(anchor, 12), &rotated_anchor);
    x = r_u32(object + 8) + (uint32)relative.vx - (uint32)rotated_anchor.vx;
    w_u32(effect + 8, x);
    y = r_u32(object + 12) + (uint32)relative.vy - (uint32)rotated_anchor.vy;
    w_u32(effect + 12, y);
    z = r_u32(object + 16) + (uint32)relative.vz - (uint32)rotated_anchor.vz;
    w_u32(effect + 16, z);
}

void game_position_scene_effects(void)
{
    uint32 index, offset, object, effect;
    FUNCTION_MARKER(0x800DFE40, "GAME.OVL");
    for (index = 0; index < 32; ++index) {
        offset = index * 0x34;
        if (r_u8(0x801094E8 + offset) != 0) {
            object = r_u32(0x801094E0 + offset);
            effect = r_u32(0x801094E4 + offset);
            game_position_attached_effect(object, effect, 0x801094B8 + offset, 0x801094C8 + offset, 0x800);
        }
    }
}

void game_initialize_blob_effect_resources(void)
{
    uint32 buffer, size, index, component, u, v, right, bottom, texture_x, texture_y, clut_x, clut_y;
    sint32 offset;
    TEX_INFO info;
    PSX_RECT rectangle;
    uint16 palette[16];
    FUNCTION_MARKER(0x800C6BD8, "GAME.OVL");
    size = disc_file_size("BLOB.TIM");
    buffer = heap_alloc_back(size);
    resource_load_into("BLOB.TIM", buffer);
    texture_read_info(buffer + 4, &info);
    texture_place_info(&info, 1);
    texture_upload(buffer, &info, 1);
    texture_x = (uint32)(sint32)info.pixel.x;
    texture_y = (uint32)(sint32)info.pixel.y;
    w_u32(0x800F4F60, texture_x);
    w_u32(0x800F4F70, texture_y);
    u = (texture_x & 63) << 2;
    w_u32(0x800F4020, u);
    right = u + ((uint32)(uint16)info.pixel.w << 2) - 1;
    v = texture_y & 255;
    w_u32(0x800F4230, v);
    w_u32(0x800F422C, right);
    bottom = v + (uint16)info.pixel.h - 1;
    w_u32(0x800F4234, bottom);
    texture_place_info(&info, 0);
    texture_upload(buffer, &info, 0);
    w_u32(0x800F4F28, (uint32)(sint32)info.clut.x);
    w_u32(0x800F4F30, (uint32)(sint32)info.clut.y);
    texture_place_info(&info, 0);
    texture_upload(buffer, &info, 0);
    w_u32(0x800FD4FC, (uint32)(sint32)info.clut.x);
    w_u32(0x800FD500, (uint32)(sint32)info.clut.y);
    for (index = 0; index < 16; ++index) {
        component = (index * index) >> 4;
        palette[index] = (uint16)((component + (component << 5) + (component << 10)) | 0xFFFF8000);
    }
    rectangle.w = 16;
    palette[15] = 0;
    rectangle.h = 1;
    rectangle.x = (sint16)r_u32(0x800FD4FC);
    rectangle.y = (sint16)r_u32(0x800FD500);
    LoadImagePSX(&rectangle, (uint32 *)palette);
    DrawSync(0);
    heap_free(buffer);
    for (index = 0; index < 128; ++index) {
        w_u32(0x800FB5D8 + index * 0x38, 0);
        w_u32(0x800FB5DC + index * 0x38, 0);
    }
    for (offset = 0x1A4; offset >= 0; offset -= 0x1C)
        w_u32(0x801180A4 + (uint32)offset, 0);
    u = r_u32(0x800F4020);
    texture_x = r_u32(0x800F4F60);
    v = r_u32(0x800F4230);
    right = r_u32(0x800F422C);
    bottom = r_u32(0x800F4234);
    w_u8(0x8011A3A8, u);
    w_u8(0x8011A3AC, u);
    clut_y = r_u32(0x800F4F30);
    w_u8(0x8011A3A9, v);
    w_u8(0x8011A3AB, v);
    clut_x = r_u32(0x800F4F28);
    w_u8(0x8011A3AA, right);
    w_u8(0x8011A3AE, right);
    texture_y = r_u32(0x800F4F70);
    w_u8(0x8011A3AD, bottom);
    w_u8(0x8011A3AF, bottom);
    w_u16(0x8011A3B0, (clut_y << 6) | (((uint32)((sint32)clut_x >> 4)) & 63));
    w_u16(0x8011A3B2, GetTPage(0, 1, texture_x, texture_y));
}

void game_measure_level_model_extent(void)
{
    uint32 table, index, scale_address, shift, model, record, vertices, vertex_index, x, y, z;
    sint32 scale, signed_scale;
    FUNCTION_MARKER(0x800D1A80, "GAME.OVL");
    table = r_u32(0x80046178);
    w_u32(0x800F4EA4, 0);
    for (index = 0; index < 20; ++index) {
        scale_address = 0x800536B6 + index * 0x3C;
        scale = r_s16(scale_address);
        model = (uint32)r_s16(0x800536B8 + index * 0x3C);
        shift = (uint32)(scale < 0 ? -scale : scale) & 31;
        w_u32(0x8011F130, model);
        if (model == 0 || model == r_u32(0x800ED498) || model == r_u32(0x800ED494))
            continue;
        model = (uint32)r_s16(0x800536B8 + index * 0x3C);
        record = r_u32(table + model * 132);
        if (record == 0)
            continue;
        scale = r_s32(record + 4);
        vertices = r_u32(record);
        if (scale <= 0)
            continue;
        signed_scale = r_s16(scale_address);
        vertex_index = 0;
        do {
            scale = r_s16(vertices);
            x = (uint32)(scale < 0 ? -scale : scale);
            scale = r_s16(vertices + 4);
            z = (uint32)(scale < 0 ? -scale : scale);
            scale = r_s16(vertices + 2);
            y = (uint32)(scale < 0 ? -scale : scale);
            if (signed_scale > 0) {
                x <<= shift;
                y <<= shift;
                z <<= shift;
            } else if (signed_scale < 0) {
                x = (uint32)((sint32)x >> shift);
                y = (uint32)((sint32)y >> shift);
                z = (uint32)((sint32)z >> shift);
            }
            if (r_s32(0x800F4EA4) < (sint32)x)
                w_u32(0x800F4EA4, x);
            if (r_s32(0x800F4EA4) < (sint32)y)
                w_u32(0x800F4EA4, y);
            if (r_s32(0x800F4EA4) < (sint32)z)
                w_u32(0x800F4EA4, z);
            ++vertex_index;
            vertices += 8;
        } while ((sint32)vertex_index < r_s32(record + 4));
    }
}

void game_create_script_controller(void)
{
    uint32 object, frame;
    FUNCTION_MARKER(0x800AC5BC, "GAME.OVL");
    object = game_create_object(0, 0, 0, 0, 0);
    w_u8(object + 0x26B, 30);
    w_u8(object + 0x26C, 30);
    frame = r_u32(0x800311B8);
    w_u32(object + 0x40, 0x800AC61C);
    w_u32(0x800F4EA0, object);
    w_u32(0x800595B8, frame);
}

void game_create_script_event_controller(void)
{
    uint32 object;
    FUNCTION_MARKER(0x800E44B4, "GAME.OVL");
    object = game_create_object(0, 0, 0, 0, 0);
    w_u32(object + 0x40, 0x800E43E0);
    w_u8(object + 0x26B, 30);
    w_u8(object + 0x26C, 30);
}

void game_reset_level_script_state(void)
{
    uint32 index, offset, first, second, object, identifier, random_value;
    FUNCTION_MARKER(0x800AD270, "GAME.OVL");
    w_u16(0x80118078, 0xFFFF);
    w_u16(0x80059610, 3);
    w_u32(0x800595B0, 0);
    w_u32(0x800595B4, 0);
    w_u16(0x80059614, 0);
    w_u16(0x800595FC, 0);
    w_u16(0x800595FE, 0);
    w_u32(0x800595AC, 0);
    w_u32(0x80059194, 0);
    w_u32(0x80059190, 0);
    w_u32(0x800595C0, 0);
    w_u16(0x800595F6, 0);
    w_u32(0x800595CC, 0);
    w_u16(0x800595F0, 0);
    w_u16(0x800595F2, 0);
    w_u32(0x800595C4, 0);
    w_u32(0x800595C8, 0);
    w_u16(0x800595F4, 0);
    w_u32(0x800595E0, 0);
    w_u32(0x800595E4, 0);
    w_u16(0x80059618, 0xFF9C);
    w_u16(0x800595E8, 0);
    w_u16(0x800595EA, 0);
    w_u16(0x800595EC, 0);
    w_u16(0x800595EE, 0);
    w_u16(0x80059610, 0);
    for (index = 0; index < 20; ++index) {
        offset = index * 0x3C;
        w_u16(0x800536C0 + offset, 0);
        w_u16(0x800536BE + offset, 0);
        w_u16(0x800536C2 + offset, 0);
        w_u16(0x800536B8 + offset, 0);
        w_u8(0x800536C6 + offset, 0);
        w_u16(0x800536BC + offset, 0);
    }
    w_u32(0x800EEB1C, 0xFFFFFE98);
    w_u32(0x8012401C, 0);
    w_u32(0x800EEB24, 0);
    w_u16(0x8005961A, 0);
    w_u16(0x8005961C, 1);
    w_u32(0x800ED464, 0);
    w_u16(0x8005961E, 0);
    w_u16(0x80059620, 0);
    w_u16(0x80059622, 0);
    w_u16(0x80059624, 0);
    w_u16(0x80059626, 0);
    w_u16(0x80059628, 0);
    w_u16(0x8005962A, 0);
    w_u16(0x8005962C, 0);
    random_value = (uint32)random_between(0, 3);
    first = r_u32(0x80075E00);
    second = r_u32(0x80075E04);
    w_u32(0x80059E70, first);
    w_u32(0x80059E74, second);
    object = r_u32(0x80040FDC);
    w_u32(0x80059E68, object);
    identifier = r_u16(object + 0x24A);
    w_u32(0x800ED460, random_value);
    w_u16(0x80059E6C, identifier);
    for (index = 1; index < 19; ++index) {
        offset = index * 0x2C;
        w_u32(0x80059E68 + offset, 0);
        w_u16(0x80059E6C + offset, 0);
    }
    first = r_u32(0x800595F8);
    w_u32(0x800595BC, 1);
    if (first == 0x30007) {
        first = r_u32(0x800595D0);
        second = r_u32(0x800595D4);
        w_u32(0x800595D8, first);
        w_u32(0x800595DC, second);
    }
}

void game_validate_level_script(void)
{
    uint32 count;
    FUNCTION_MARKER(0x800AD8B8, "GAME.OVL");
    if (r_u32(0x80059194) == 0)
        diagnostic_report(666, 0x80075E68, 0x80075DFC);
    count = r_u32(0x80059190);
    if ((sint32)count >= 200)
        diagnostic_report(count, 0x80075E7C, 0x80075DFC);
}

void game_initialize_level_script(void)
{
    uint32 offset, first, second, third, fourth, fifth, object, identifier, index;
    char path[18];
    FUNCTION_MARKER(0x800D1704, "GAME.OVL");
    game_reset_level_script_state();
    if (r_u32(0x80031134) == 0)
        game_load_bound_bundle_native("LESCAPE.BND");
    w_u32(0x800311DC, 0);
    game_create_script_controller();
    if (r_u32(0x80031134) != 0)
        game_load_level_script(0x80075FA4);
    else
        game_load_level_script(0x80010518);
    game_validate_level_script();
    game_measure_level_model_extent();
    offset = r_u32(0x800595BC) * 0x2C;
    first = r_u32(0x80075FAC);
    second = r_u32(0x80075FB0);
    third = r_u8(0x80075FB4);
    w_u32(0x80059E70 + offset, first);
    w_u32(0x80059E74 + offset, second);
    w_u8(0x80059E78 + offset, third);
    fourth = r_u8(0x80075FB5);
    fifth = r_u8(0x80075FB6);
    w_u8(0x80059E79 + offset, fourth);
    w_u8(0x80059E7A + offset, fifth);
    index = r_u32(0x800595BC);
    object = r_u32(0x80040FDC);
    w_u32(0x80059E68 + index * 0x2C, object);
    index = r_u32(0x800595BC);
    identifier = r_u16(object + 0x24A);
    w_u16(0x80059E6C + index * 0x2C, identifier);
    index = r_u32(0x800595BC);
    w_u16(0x800EF572, index);
    ++index;
    w_u32(0x800595BC, index);
    if ((sint32)index >= 19)
        diagnostic_report(666, 0x80075FB8, 0x80075F94);
    if (r_u32(0x80031134) == 0)
        game_create_script_event_controller();
    sprintf(path, "%d_%d.MSG", (sint32)r_s16(0x800595F8), (sint32)r_s16(0x800595FA));
    for (index = 0; path[index] != 0; ++index)
        w_u8(0x800FA368 + index, (uint8)path[index]);
    w_u8(0x800FA368 + index, 0);
    if (resource_exists(path)) {
        game_load_level_script(0x800FA368);
        w_u16(0x800EF570, 1);
    } else {
        w_u16(0x800EF570, 0);
    }
}

static sint32 game_level_argument_integer_native(uint32 text)
{
    uint32 length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    return string_to_integer((const char *)psx_addr(text, length + 1));
}

void game_config_set_unique_ship_name(const uint32 *arguments, uint32 first)
{
    sint32 value;
    uint32 offset;
    FUNCTION_MARKER(0x80093668, "GAME.OVL");
    value = game_level_argument_integer_native(arguments[first + 1]);
    if (value < 0 || r_s32(0x800595BC) < value)
        diagnostic_report((uint32)value, 0x80074FC4, 0x80074F80);
    offset = r_u32(0x8012401C) * 60;
    w_u16(0x800536BA + offset, (uint32)value);
}

void game_config_set_ship_intelligence(const uint32 *arguments, uint32 first)
{
    uint32 value, offset;
    FUNCTION_MARKER(0x80093700, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    offset = r_u32(0x8012401C) * 60;
    w_u16(0x800536B0 + offset, value);
}

void game_config_set_ship_shield(const uint32 *arguments, uint32 first)
{
    uint32 value, offset;
    FUNCTION_MARKER(0x80093754, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    offset = r_u32(0x8012401C) * 60;
    w_u32(0x800536A4 + offset, value);
}

void game_config_fire(void)
{
    FUNCTION_MARKER(0x800937A8, "GAME.OVL");
}

void game_config_set_ship_velocity(const uint32 *arguments, uint32 first)
{
    uint32 value, offset;
    FUNCTION_MARKER(0x800937B0, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    offset = r_u32(0x8012401C) * 60;
    w_u32(0x8005369C + offset, value << 8);
    value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    offset = r_u32(0x8012401C) * 60;
    w_u32(0x800536A0 + offset, value << 8);
    value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    offset = r_u32(0x8012401C) * 60;
    w_u16(0x800536B2 + offset, value << 8);
    value = (uint32)game_level_argument_integer_native(arguments[first + 4]);
    offset = r_u32(0x8012401C) * 60;
    w_u16(0x800536B4 + offset, value);
}

void game_config_set_ship_sensor(const uint32 *arguments, uint32 first)
{
    uint32 value, offset;
    FUNCTION_MARKER(0x800938A8, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    if (value > 1000000)
        value = 1000000;
    offset = r_u32(0x8012401C) * 60;
    w_u32(0x800536AC + offset, value);
}

void game_config_set_ship_hull(const uint32 *arguments, uint32 first)
{
    uint32 value, offset;
    FUNCTION_MARKER(0x8009391C, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    offset = r_u32(0x8012401C) * 60;
    w_u32(0x800536A8 + offset, value);
}

void game_config_set_ship_scale(const uint32 *arguments, uint32 first)
{
    uint32 value, offset;
    FUNCTION_MARKER(0x80093970, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    offset = r_u32(0x8012401C) * 60;
    w_u16(0x800536B6 + offset, value);
}

void game_config_objective_text(void)
{
    FUNCTION_MARKER(0x80093C48, "GAME.OVL");
}

void game_config_set_ship_size(const uint32 *arguments, uint32 first)
{
    uint32 value, offset;
    FUNCTION_MARKER(0x80093C50, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    offset = r_u32(0x8012401C) * 60;
    w_u16(0x800536BC + offset, value);
}

void game_config_select_task_list(const uint32 *arguments, uint32 first)
{
    uint32 value;
    FUNCTION_MARKER(0x80093CA4, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    w_u32(0x800ED490, value);
    w_u32(0x80108908, 0);
}

void game_config_append_task_id(const uint32 *arguments, uint32 first)
{
    uint32 value, list, count, destination;
    FUNCTION_MARKER(0x80093CE0, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    list = r_u32(0x800ED490);
    count = r_u32(0x80108908);
    destination = 0x80119448 + list * 88 + (count << 2);
    ++count;
    w_u32(0x80108908, count);
    w_u32(destination, value);
    if ((sint32)count >= 22)
        diagnostic_report(list, 0x80074FFC, 0x80074F80);
}

void game_config_begin_task_commands(void)
{
    FUNCTION_MARKER(0x800952F8, "GAME.OVL");
    w_u32(0x80108914, 0);
    w_u32(0x8011A0D8, 1);
    w_u32(0x800EDB34, 1);
}

sint32 game_lookup_script_task(uint32 text, uint32 table)
{
    char name[128];
    char *dot;
    uint32 length, candidate;
    FUNCTION_MARKER(0x80096904, "GAME.OVL");
    length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    strcpy(name, (const char *)psx_addr(text, length + 1));
    dot = strchr(name, '.');
    if (dot != 0)
        *dot = 0;
    for (;;) {
        candidate = r_u32(table);
        if (candidate == 0)
            break;
        length = 0;
        while (r_u8(candidate + length) != 0)
            ++length;
        if (strcmp((const char *)psx_addr(candidate, length + 1), name) == 0)
            return r_s32(r_u32(table + 4));
        table += 8;
    }
    return name[0] == '-' ? 0 : -1;
}

uint32 game_encode_script_task(const uint32 *arguments, uint32 first, uint32 destination)
{
    uint32 mode, count, text;
    sint32 task, value, named_value;
    mode = r_u32(0x800ED464);
    FUNCTION_MARKER(0x80096B54, "GAME.OVL");
    text = arguments[first];
    if (mode != 0) {
        task = game_level_argument_integer_native(text);
    } else {
        task = game_lookup_script_task(text, 0x800ED874);
        if (task == -1)
            diagnostic_report(1000, 0x80075294, 0x80074F80);
    }
    if (task == 0) {
        w_u32(destination, 0xFFFFFFFF);
        return 1;
    }
    if (((uint32)task == r_u32(0x800ED7B4) || (uint32)task == r_u32(0x800ED7B8)) && r_u32(0x800EDD3C) == 0)
        game_load_bound_bundle_native("NSPECTRA.BND");
    w_u32(destination, (uint32)task);
    destination += 8;
    count = 1;
    for (;;) {
        text = arguments[first + count];
        value = game_level_argument_integer_native(text);
        named_value = game_lookup_script_task(text, 0x800ED874);
        if (value == -1)
            break;
        if ((uint32)named_value + 1 < 2)
            w_u32(destination, (uint32)value);
        else
            w_u32(destination, (uint32)named_value);
        destination += 4;
        ++count;
    }
    return count;
}

void game_config_append_task_command(const uint32 *arguments, uint32 first)
{
    uint32 destination, words, index, task;
    FUNCTION_MARKER(0x8009531C, "GAME.OVL");
    destination = 0x80058B50 + (r_u32(0x80108914) << 2);
    words = game_encode_script_task(arguments + first + 1, 0, destination);
    index = r_u32(0x8011A0D8);
    task = r_u32(destination);
    w_u32(0x800F27A8 + (index << 3), task);
    w_u32(0x800F27AC + (index << 3), destination + 4);
    ++index;
    w_u32(0x8011A0D8, index);
    if ((sint32)index >= 101)
        diagnostic_report(index, 0x8007513C, 0x80074F80);
    index = r_u32(0x80108914);
    w_u32(destination + 4, words - 1);
    w_u32(0x80108914, index + 1 + words);
    if (destination > 0x80058E70)
        diagnostic_report(666, 0x80075154, 0x80074F80);
}

void game_rotate_player_y(uint32 angle)
{
    MATRIX result;
    uint32 object;
    FUNCTION_MARKER(0x80092778, "GAME.OVL");
    if (angle == 0)
        return;
    object = r_u32(0x80040FDC);
    matrix_rotate_y_before_native(&result, (MATRIX *)psx_addr(object + 0x11C, 20), angle);
    object = r_u32(0x80040FDC);
    matrix_copy_rotation_native(object + 0x11C, &result);
}

void game_rotate_player_x(uint32 angle)
{
    MATRIX result;
    uint32 object;
    FUNCTION_MARKER(0x80092730, "GAME.OVL");
    if (angle == 0)
        return;
    object = r_u32(0x80040FDC);
    matrix_rotate_x_before_native(&result, (MATRIX *)psx_addr(object + 0x11C, 20), angle);
    object = r_u32(0x80040FDC);
    matrix_copy_rotation_native(object + 0x11C, &result);
}

void game_rotate_player_z(uint32 angle)
{
    MATRIX result;
    uint32 object;
    FUNCTION_MARKER(0x800927C0, "GAME.OVL");
    if (angle == 0)
        return;
    object = r_u32(0x80040FDC);
    matrix_rotate_z_before_native(&result, (MATRIX *)psx_addr(object + 0x11C, 20), angle);
    object = r_u32(0x80040FDC);
    matrix_copy_rotation_native(object + 0x11C, &result);
}

void game_load_asteroid_bundles(void)
{
    FUNCTION_MARKER(0x80094210, "GAME.OVL");
    if (game_find_model_name_native("ASTERB") == 0)
        game_load_bound_bundle_native("ASTERB.BND");
    if (game_find_model_name_native("ASTERA") == 0)
        game_load_bound_bundle_native("ASTERA.BND");
}

void game_config_create_asteroid_controller(const uint32 *arguments, uint32 first)
{
    uint32 object, value;
    FUNCTION_MARKER(0x80094174, "GAME.OVL");
    game_load_asteroid_bundles();
    object = game_create_object(0, 0, 0, 0, 0);
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    w_u32(object + 0xA8, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    w_u8(object + 0x26B, value * 30);
    value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    w_u32(object + 0xAC, value);
    w_u32(object + 0x40, 0x800AA91C);
}

uint32 game_find_ship_start_callback(uint32 ship)
{
    uint32 pointer, index;
    sint32 model;
    FUNCTION_MARKER(0x80094058, "GAME.OVL");
    pointer = r_u32(0x800ED49C);
    model = r_s16(0x800536B8 + ship * 60);
    w_u32(0x8011F130, 0);
    if (r_s32(pointer) != model) {
        do {
            index = r_u32(0x8011F130);
            pointer = r_u32(0x800ED49C + (index << 3));
            if (r_s32(pointer) == -1)
                diagnostic_report(ship, 0x80075040, 0x80074F80);
            index = r_u32(0x8011F130) + 1;
            pointer = r_u32(0x800ED49C + (index << 3));
            w_u32(0x8011F130, index);
        } while (r_s32(pointer) != model);
    }
    index = r_u32(0x8011F130);
    return r_u32(0x800ED4A0 + (index << 3));
}

void game_assign_ship_task_list(uint32 object, uint32 list)
{
    uint32 destination, index, task, parameters, callback;
    FUNCTION_MARKER(0x80093F50, "GAME.OVL");
    w_u32(object + 0xDC, list);
    destination = heap_alloc_back(0xB0);
    w_u32(object + 0xF4, destination);
    index = 0;
    for (;;) {
        list = r_u32(object + 0xDC);
        task = r_u32(0x80119448 + list * 88 + (index << 2));
        w_u32(destination, task);
        parameters = r_u32(0x800F27AC + (task << 3));
        ++index;
        if (task == 0)
            break;
        w_u32(destination + 4, r_u32(parameters + 4));
        destination += 8;
        if ((sint32)index >= 23)
            diagnostic_report(r_u32(object + 4), 0x80075028, 0x80074F80);
    }
    w_u32(destination, 0xFFFFFFFF);
    w_u32(destination + 4, 0);
    callback = game_find_ship_start_callback(r_u32(object + 4));
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}

void game_config_create_ship(const uint32 *arguments, uint32 first)
{
    uint32 ship, list, x, y, z, yaw, pitch, roll, object;
    FUNCTION_MARKER(0x80093D98, "GAME.OVL");
    ship = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    list = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    if (list == 0xFFFFFFFF && ship != 0)
        diagnostic_report(ship, 0x80075014, 0x80074F80);
    x = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    y = (uint32)game_level_argument_integer_native(arguments[first + 4]);
    z = (uint32)game_level_argument_integer_native(arguments[first + 5]);
    yaw = (uint32)game_level_argument_integer_native(arguments[first + 6]) << 3;
    pitch = (uint32)game_level_argument_integer_native(arguments[first + 7]) << 3;
    roll = (uint32)game_level_argument_integer_native(arguments[first + 8]) << 3;
    if (ship == 0) {
        object = r_u32(0x80040FDC);
        w_u32(object + 8, x);
        w_u32(object + 12, y);
        w_u32(object + 16, z);
        matrix_set_identity(object + 0x11C);
        game_rotate_player_y(yaw);
        game_rotate_player_x(pitch);
        game_rotate_player_z(roll);
        object = r_u32(0x80040FDC);
        object_extract_angles(object, object);
        object = r_u32(0x80040FDC);
        matrix_copy_rotation(0x80035060, object + 0x11C);
    } else {
        object = game_spawn_script_ship(ship, x, y, z, yaw, pitch, roll);
        game_assign_ship_task_list(object, list);
    }
    w_u16(0x800536C4 + ship * 60, list);
}

void game_config_add_waypoint(const uint32 *arguments, uint32 first)
{
    uint32 value, index, count;
    FUNCTION_MARKER(0x8009432C, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    index = r_u32(0x800EDB34);
    w_u32(0x80126D68 + index * 12, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    index = r_u32(0x800EDB34);
    w_u32(0x80126D6C + index * 12, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    index = r_u32(0x800EDB34);
    w_u32(0x80126D70 + index * 12, value);
    count = r_u32(0x800F18B0);
    ++index;
    w_u32(0x800EDB34, index);
    w_u32(0x800F18B0, count + 1);
    if ((sint32)index >= 60)
        diagnostic_report(index, 0x80075084, 0x80074F80);
}

void game_config_create_training_hoop(const uint32 *arguments, uint32 first)
{
    uint32 x, y, z, yaw, pitch, roll, object, flags, render_flags, frame, count, index;
    FUNCTION_MARKER(0x80094434, "GAME.OVL");
    x = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    y = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    z = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    yaw = (uint32)game_level_argument_integer_native(arguments[first + 4]) << 3;
    pitch = (uint32)game_level_argument_integer_native(arguments[first + 5]) << 3;
    roll = (uint32)game_level_argument_integer_native(arguments[first + 6]) << 3;
    object = game_create_object(r_u32(0x800EDE40), x, y, z, 3);
    w_u32(object + 0x6C, 0x800ED47C);
    w_u32(object + 0x38, 0x8002B8FC);
    w_u32(object + 0xA0, 6);
    w_u32(object + 0x44, 0x800E56CC);
    w_u32(object + 0x3C, 0x8002D2BC);
    w_u32(object + 0x40, 0x800E5BD0);
    flags = r_u32(object + 0x114);
    w_u32(object + 0xD8, 0x493E0);
    render_flags = r_u32(object + 0x70);
    w_u32(object + 0x84, 3);
    w_u32(object + 0x114, flags | 0x810);
    w_u32(object + 0x70, render_flags | 0x80);
    matrix_set_identity(object + 0x11C);
    w_u32(object + 0x20, yaw);
    w_u32(object + 0x24, pitch);
    w_u32(object + 0x28, roll);
    game_rotate_object_basis(object);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x34, 0xFFFFFFFF);
    w_u32(object + 0x30, 0xFFFFFFFF);
    w_u32(object + 0x2C, 0xFFFFFFFF);
    frame = r_u32(0x800311B8);
    w_u32(object + 0x114, flags | 0x80);
    count = r_u32(0x800F18B0);
    w_u32(object + 0xAC, frame);
    index = r_u32(0x800EDB34);
    w_u32(object + 0xA8, count);
    count = r_u32(0x800F18B0);
    w_u32(0x800EDB34, index + 1);
    ++count;
    w_u32(0x800F18B0, count);
    if ((sint32)count >= 60)
        diagnostic_report(count, 0x800750A4, 0x80074F80);
}

void game_config_set_player_launch(const uint32 *arguments, uint32 first)
{
    uint32 object, value;
    FUNCTION_MARKER(0x800945F4, "GAME.OVL");
    object = r_u32(0x80040FDC);
    w_u32(object + 0x40, 0x8007A9CC);
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    object = r_u32(0x80040FDC);
    w_u32(object + 0xDC, value);
}

void game_config_bind_ship_name(const uint32 *arguments, uint32 first)
{
    sint32 value;
    uint32 object, offset, identifier;
    FUNCTION_MARKER(0x8009463C, "GAME.OVL");
    value = game_level_argument_integer_native(arguments[first + 1]);
    if (value < 0 || r_s32(0x800595BC) < value)
        diagnostic_report((uint32)value, 0x80074FC4, 0x80074F80);
    offset = (uint32)value * 44;
    object = r_u32(0x800F80FC);
    w_u16(object + 0x24E, (uint32)value);
    w_u32(0x80059E68 + offset, object);
    identifier = r_u16(object + 0x24A);
    w_u16(0x80059E8E + offset, 0);
    w_u16(0x80059E92 + offset, 0);
    w_u16(0x80059E90 + offset, 0);
    w_u16(0x80059E6C + offset, identifier);
    w_u16(object + 0x268, 1);
}

void game_config_add_ship_name(const uint32 *arguments, uint32 first)
{
    uint32 text, length, offset, index, value, count;
    FUNCTION_MARKER(0x80094728, "GAME.OVL");
    text = arguments[first + 1];
    length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    if ((sint32)length >= 31)
        diagnostic_report(1000, 0x800750C8, 0x80074F80);
    offset = r_u32(0x800595BC) * 44;
    text = arguments[first + 1];
    index = 0;
    do {
        value = r_u8(text + index);
        w_u8(0x80059E70 + offset + index, value);
        ++index;
    } while (value != 0);
    count = r_u32(0x800595BC) + 1;
    w_u32(0x800595BC, count);
    if ((sint32)count >= 19)
        diagnostic_report(666, 0x800750E8, 0x80074F80);
}

void game_config_set_hoop_path(const uint32 *arguments, uint32 first)
{
    uint32 objective, offset, index, count, cursor;
    sint32 value;
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    FUNCTION_MARKER(0x800939D4, "GAME.OVL");
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 9);
    value = game_level_argument_integer_native(arguments[first + 1]);
    if (value == 0)
        diagnostic_report(666, 0x80074FE4, 0x80074F80);
    cursor = first;
    index = 0;
    count = 0;
    for (;;) {
        value = game_level_argument_integer_native(arguments[cursor + 1]);
        ++cursor;
        if (value <= 0)
            break;
        w_u32(0x80124930 + index, count);
        w_u32(0x80124934 + index, 0);
        w_u32(0x80124938 + index, (uint32)value);
        offset = r_u32(0x80059190);
        ++count;
        w_u32(0x80058E70 + ((offset + count) << 2), (uint32)value);
        index += 12;
        (void)game_level_argument_integer_native(arguments[cursor + 1]);
    }
    offset = r_u32(0x80059190);
    w_u32(0x800F18A0, count);
    w_u32(0x80059190, offset + 1 + count);
}

void game_config_add_jumpgate_objective(void)
{
    uint32 objective, offset;
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 10);
    offset = r_u32(0x80059190);
    FUNCTION_MARKER(0x80093BB0, "GAME.OVL");
    w_u32(0x80059190, offset + 1);
}

void game_config_add_drone_objective(void)
{
    uint32 objective, offset;
    FUNCTION_MARKER(0x80093BCC, "GAME.OVL");
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 11);
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 1);
}

void game_config_destroy_ship_objective(const uint32 *arguments, uint32 first)
{
    uint32 value, objective, offset;
    FUNCTION_MARKER(0x800947F0, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 1);
    offset = r_u32(0x80059190);
    w_u32(0x80058E74 + (offset << 2), value);
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 2);
}

void game_config_time_greater_objective(const uint32 *arguments, uint32 first)
{
    uint32 value, objective, offset;
    FUNCTION_MARKER(0x800948AC, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    if ((sint32)value <= 0x1A5DF)
        value *= 30;
    if ((sint32)value < 0)
        value = 0x7FFFFFF;
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 2);
    offset = r_u32(0x80059190);
    w_u32(0x80058E74 + (offset << 2), value);
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 2);
}

void game_config_time_less_objective(const uint32 *arguments, uint32 first)
{
    uint32 value, objective, offset;
    FUNCTION_MARKER(0x80094998, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 3);
    value *= 30;
    offset = r_u32(0x80059190);
    w_u32(0x80058E74 + (offset << 2), value);
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 2);
}

void game_config_ship_finished_objective(const uint32 *arguments, uint32 first)
{
    uint32 value, objective, offset;
    FUNCTION_MARKER(0x80094A5C, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    if ((sint32)value < 1000)
        diagnostic_report(value, 0x800750FC, 0x80074F80);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 6);
    offset = r_u32(0x80059190);
    w_u32(0x80058E74 + (offset << 2), value);
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 2);
}

void game_config_ships_destroyed_objective(const uint32 *arguments, uint32 first)
{
    uint32 first_value, second_value, third_value, fourth_value, objective, offset;
    FUNCTION_MARKER(0x80094B48, "GAME.OVL");
    first_value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    second_value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    third_value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    fourth_value = (uint32)game_level_argument_integer_native(arguments[first + 4]);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 7);
    offset = r_u32(0x80059190);
    w_u32(0x80058E74 + (offset << 2), first_value);
    offset = r_u32(0x80059190);
    w_u32(0x80058E78 + (offset << 2), second_value);
    offset = r_u32(0x80059190);
    w_u32(0x80058E7C + (offset << 2), third_value);
    offset = r_u32(0x80059190);
    w_u32(0x80058E80 + (offset << 2), fourth_value);
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 5);
}

void game_config_ships_docked_objective(const uint32 *arguments, uint32 first)
{
    uint32 first_value, second_value, third_value, fourth_value, objective, offset;
    FUNCTION_MARKER(0x80094CA0, "GAME.OVL");
    first_value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    second_value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    third_value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    fourth_value = (uint32)game_level_argument_integer_native(arguments[first + 4]);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 8);
    offset = r_u32(0x80059190);
    w_u32(0x80058E74 + (offset << 2), first_value);
    offset = r_u32(0x80059190);
    w_u32(0x80058E78 + (offset << 2), second_value);
    offset = r_u32(0x80059190);
    w_u32(0x80058E7C + (offset << 2), third_value);
    offset = r_u32(0x80059190);
    w_u32(0x80058E80 + (offset << 2), fourth_value);
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 5);
}

void game_config_ship_disabled_objective(const uint32 *arguments, uint32 first)
{
    uint32 value, objective, offset;
    FUNCTION_MARKER(0x80095134, "GAME.OVL");
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 12);
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    offset = r_u32(0x80059190);
    w_u32(0x80058E74 + (offset << 2), value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    offset = r_u32(0x80059190);
    w_u32(0x80058E78 + (offset << 2), value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    offset = r_u32(0x80059190);
    w_u32(0x80058E7C + (offset << 2), value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 4]);
    offset = r_u32(0x80059190);
    w_u32(0x80058E80 + (offset << 2), value);
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 5);
}

void game_config_navy_defeated_objective(void)
{
    uint32 objective, offset;
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 4);
    offset = r_u32(0x80059190);
    FUNCTION_MARKER(0x8009509C, "GAME.OVL");
    w_u32(0x80059190, offset + 1);
}

void game_config_league_defeated_objective(void)
{
    uint32 objective, offset;
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 5);
    offset = r_u32(0x80059190);
    FUNCTION_MARKER(0x80095118, "GAME.OVL");
    w_u32(0x80059190, offset + 1);
}

void game_config_planet_destroyed_objective(void)
{
    uint32 objective, offset;
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 13);
    offset = r_u32(0x80059190);
    FUNCTION_MARKER(0x800952DC, "GAME.OVL");
    w_u32(0x80059190, offset + 1);
}

void game_config_add_objective(const uint32 *arguments, uint32 first)
{
    uint32 objective, offset, count;
    sint32 value;
    objective = r_u32(0x80059194);
    offset = r_u32(0x80059190);
    FUNCTION_MARKER(0x80094E08, "GAME.OVL");
    w_u32(0x80059198 + (objective << 2), offset);
    objective = r_u32(0x80059194);
    w_u32(0x80059194, objective + 1);
    count = 0;
    value = game_level_argument_integer_native(arguments[first + 1]);
    while (value != -1) {
        offset = r_u32(0x80059190);
        w_u32(0x80058E70 + (offset << 2), (uint32)value);
        offset = r_u32(0x80059190);
        ++count;
        w_u32(0x80059190, offset + 1);
        value = game_level_argument_integer_native(arguments[first + count + 1]);
    }
    if (count == 0)
        diagnostic_report(0, 0x80075118, 0x80074F80);
}

void game_config_set_mission_ending(const uint32 *arguments, uint32 first)
{
    uint32 first_value, second_value, selector, offset;
    FUNCTION_MARKER(0x80094F20, "GAME.OVL");
    first_value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    second_value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    selector = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    offset = r_u32(0x80059190);
    w_u32(0x80058E70 + (offset << 2), 666);
    offset = r_u32(0x80059190);
    w_u32(0x80058E74 + (offset << 2), first_value);
    offset = r_u32(0x80059190);
    w_u32(0x80058E78 + (offset << 2), second_value);
    offset = r_u32(0x80059190);
    w_u32(0x80058E7C + (offset << 2), selector);
    if (selector == 0) {
        w_u16(0x80059600, first_value);
        w_u16(0x80059602, second_value);
    } else {
        w_u16(0x80059604, first_value);
        w_u16(0x80059606, second_value);
    }
    offset = r_u32(0x80059190);
    w_u32(0x80059190, offset + 4);
}

void game_config_set_scene_boundary(const uint32 *arguments, uint32 first)
{
    uint32 value;
    sint32 waypoint;
    FUNCTION_MARKER(0x80095AE8, "GAME.OVL");
    w_u16(0x80118078, 1);
    w_u16(0x80118084, 1);
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    w_u16(0x8011807A, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    w_u32(0x80118088, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    waypoint = r_s16(0x8011807A);
    w_u32(0x8011808C, value);
    if (waypoint == 0 || r_s32(0x800EDB34) < waypoint)
        diagnostic_report((uint32)waypoint, 0x800751B8, 0x80074F80);
}

void game_config_create_ship_boundary(const uint32 *arguments, uint32 first)
{
    uint32 object, value;
    FUNCTION_MARKER(0x80095BA0, "GAME.OVL");
    object = game_create_object(0, 0, 0, 0, 0);
    w_u32(object + 0x40, 0x80095CC8);
    w_u8(object + 0x26B, 30);
    w_u8(object + 0x26C, 30);
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    w_u32(object + 0xA8, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    w_u32(object + 0xAC, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    w_u32(object + 0xB0, value);
    w_u32(object + 0xB4, 1);
}

static const char *game_level_guest_text_native(uint32 text)
{
    uint32 length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    return (const char *)psx_addr(text, length + 1);
}

uint32 game_find_message_ship(uint32 text)
{
    uint32 index;
    FUNCTION_MARKER(0x800E512C, "GAME.OVL");
    for (index = 1; index < 21; ++index) {
        if (strcmp(game_level_guest_text_native(0x800536C6 + index * 60), game_level_guest_text_native(text)) == 0)
            return index;
    }
    return 0;
}

void game_config_add_message(const uint32 *arguments, uint32 first)
{
    char message[272];
    uint32 value, index, speaker, cursor, length, destination, character;
    FUNCTION_MARKER(0x800963F0, "GAME.OVL");
    value = (uint32)game_lookup_script_task(arguments[first + 1], 0x800ED874);
    index = r_u32(0x800595C4);
    w_u16(0x80126CA4 + index * 20, value);
    index = r_u32(0x800595C4);
    if (r_s16(0x80126CA4 + index * 20) == -1)
        diagnostic_report(666, 0x800751EC, 0x80074F80);
    if (strcmp(game_level_guest_text_native(arguments[first + 2]), "NAVY") == 0) {
        speaker = 3;
    } else if (strcmp(game_level_guest_text_native(arguments[first + 2]), "LEAGUE") == 0) {
        speaker = 1;
    } else {
        speaker = game_find_message_ship(arguments[first + 2]);
    }
    index = r_u32(0x800595C4);
    w_u16(0x80126C98 + index * 20, speaker);
    value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    index = r_u32(0x800595C4);
    w_u32(0x80126C9C + index * 20, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 4]);
    index = r_u32(0x800595C4);
    w_u32(0x80126CA0 + index * 20, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 5]);
    index = r_u32(0x800595C4);
    w_u16(0x80126CA6 + index * 20, value);
    index = r_u32(0x800595C4);
    w_u16(0x80126CA8 + index * 20, index);
    index = r_u32(0x800595C4);
    w_u16(0x80126CAA + index * 20, 0);
    strcpy(message, game_level_guest_text_native(arguments[first + 6]));
    length = (uint32)strlen(game_level_guest_text_native(arguments[first + 6]));
    cursor = first + 7;
    while (strcmp(game_level_guest_text_native(arguments[cursor]), "-1") != 0) {
        length += 1 + (uint32)strlen(game_level_guest_text_native(arguments[cursor]));
        if ((sint32)length >= 265)
            diagnostic_report(length, 0x80075210, 0x80074F80);
        strcat(message, " ");
        strcat(message, game_level_guest_text_native(arguments[cursor]));
        ++cursor;
    }
    index = r_u32(0x800595C4);
    destination = 0x80108A50 + index * 265;
    cursor = 0;
    do {
        character = (uint8)message[cursor];
        w_u8(destination + cursor, character);
        ++cursor;
    } while (character != 0);
    index = r_u32(0x800595C4) + 1;
    w_u32(0x800595C4, index);
    if ((sint32)index >= 11)
        diagnostic_report(index, 0x8007523C, 0x80074F80);
}

void game_config_set_token_mode(const uint32 *arguments, uint32 first)
{
    uint32 value;
    FUNCTION_MARKER(0x80096718, "GAME.OVL");
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    w_u32(0x800ED464, value);
}

void game_config_no_navy_fighters_objective(void)
{
    uint32 objective, offset;
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800591AC + (objective << 2), offset);
    objective = r_u32(0x800595AC);
    offset = r_u32(0x80059190);
    w_u32(0x800595AC, objective + 1);
    w_u32(0x80058E70 + (offset << 2), 14);
    offset = r_u32(0x80059190);
    FUNCTION_MARKER(0x80095CAC, "GAME.OVL");
    w_u32(0x80059190, offset + 1);
}

void game_config_create_asteroid_belt(const uint32 *arguments, uint32 first)
{
    uint32 object, value, callback;
    FUNCTION_MARKER(0x80095E0C, "GAME.OVL");
    game_load_asteroid_bundles();
    object = game_create_object(0, 0, 0, 0, 0);
    w_u32(object + 0x40, 0x800A5380);
    value = (uint32)game_level_argument_integer_native(arguments[first + 1]);
    w_u32(object + 0x214, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 2]);
    w_u32(object + 0x218, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 3]);
    w_u32(object + 0x21C, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 4]);
    w_u16(object + 0x224, value << 3);
    value = (uint32)game_level_argument_integer_native(arguments[first + 5]);
    w_u16(object + 0x226, value << 3);
    value = (uint32)game_level_argument_integer_native(arguments[first + 6]);
    w_u16(object + 0x228, value << 3);
    value = (uint32)game_level_argument_integer_native(arguments[first + 7]);
    w_u32(object + 0xA8, (uint32)((sint32)value >> 12));
    value = (uint32)game_level_argument_integer_native(arguments[first + 8]);
    w_u32(object + 0xAC, value);
    value = (uint32)game_level_argument_integer_native(arguments[first + 9]);
    w_u32(object + 0xB0, value);
    w_u8(object + 0x26B, 15);
    game_initialize_asteroid_belt(object);
    if (r_u32(object + 0xB0) == 0) {
        callback = r_u32(object + 0x48);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
    }
}

void game_initialize_asteroid_belt(uint32 controller)
{
    uint32 index, effect, value, x, y, z, column, payload, next_payload, coefficient, position;
    sint32 limit;
    FUNCTION_MARKER(0x800A5850, "GAME.OVL");
    if (r_u32(controller + 0xB0) != 0) {
        limit = r_s32(controller + 0xAC);
        if (limit >= 22)
            limit = 22;
        for (index = 0; (sint32)index < limit; ++index) {
            value = (uint32)random_between(200, 1200);
            effect = game_create_belt_asteroid(controller);
            x = r_u32(effect + 0x20) * value;
            y = r_u32(effect + 0x24) * value;
            z = r_u32(effect + 0x28) * value;
            position = r_u32(effect);
            w_u32(effect, position + x);
            position = r_u32(effect + 4);
            value = r_u32(effect + 8);
            w_u32(effect + 4, position + y);
            w_u32(effect + 8, value + z);
        }
        return;
    }
    if (r_s32(controller + 0xAC) <= 0)
        return;
    index = 0;
    do {
        effect = game_create_belt_asteroid(controller);
        for (column = 0; column < 3; ++column) {
            value = (uint32)random_signed_between(0, r_s32(controller + 0xA8));
            payload = r_u32(effect + 0x6C);
            coefficient = (uint32)r_s16(payload + 4 + column * 2);
            x = coefficient * value;
            next_payload = r_u32(effect + 0x6C);
            position = r_u32(effect);
            w_u32(effect, position + x);
            coefficient = (uint32)r_s16(next_payload + 10 + column * 2);
            y = coefficient * value;
            payload = r_u32(effect + 0x6C);
            position = r_u32(effect + 4);
            w_u32(effect + 4, position + y);
            coefficient = (uint32)r_s16(payload + 16 + column * 2);
            z = coefficient * value;
            position = r_u32(effect + 8);
            if (column == 2)
                w_u32(effect + 0x2C, 0x278D00);
            w_u32(effect + 8, position + z);
        }
        ++index;
    } while ((sint32)index < r_s32(controller + 0xAC));
}

void game_config_create_mine_field(const uint32 *arguments, uint32 first)
{
    VECTOR position;
    uint32 model, radius, quantity, index, effect, payload, value, attempts, allowed, column;
    uint32 coefficient, product, next_payload, coordinate, other, category, x, y, z;
    sint32 distance;
    FUNCTION_MARKER(0x80095F48, "GAME.OVL");
    if (game_find_model_name_native("MINE") == 0)
        game_load_bound_bundle_native("MINE.BND");
    model = game_find_model_name_native("MINE");
    position.vx = game_level_argument_integer_native(arguments[first + 1]);
    position.vy = game_level_argument_integer_native(arguments[first + 2]);
    position.vz = game_level_argument_integer_native(arguments[first + 3]);
    radius = (uint32)game_level_argument_integer_native(arguments[first + 4]);
    quantity = (uint32)game_level_argument_integer_native(arguments[first + 5]);
    for (index = 0; (sint32)index < (sint16)quantity; ++index) {
        effect = game_create_model_effect_native(&position, (sint16)model);
        w_u16(effect + 0x3C, 0x69);
        w_u16(effect + 0x64, 0);
        value = (uint32)random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C);
        w_u16(payload + 0x34, value);
        value = (uint32)random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C);
        w_u16(payload + 0x36, value);
        value = (uint32)random_signed_between(3, 24);
        payload = r_u32(effect + 0x6C);
        w_u16(payload + 0x38, value);
        attempts = 0;
        w_u32(effect + 0x68, 0x800EEC3C);
        do {
            for (column = 0; column < 3; ++column) {
                value = (uint32)random_signed_between(0, (sint32)radius);
                payload = r_u32(effect + 0x6C);
                coefficient = (uint32)r_s16(payload + 4 + column * 2);
                product = coefficient * value;
                next_payload = r_u32(effect + 0x6C);
                coordinate = r_u32(effect);
                w_u32(effect, coordinate + (uint32)((sint32)product >> 12));
                coefficient = (uint32)r_s16(next_payload + 10 + column * 2);
                product = coefficient * value;
                payload = r_u32(effect + 0x6C);
                coordinate = r_u32(effect + 4);
                w_u32(effect + 4, coordinate + (uint32)((sint32)product >> 12));
                coefficient = (uint32)r_s16(payload + 16 + column * 2);
                product = coefficient * value;
                coordinate = r_u32(effect + 8);
                w_u32(effect + 8, coordinate + (uint32)((sint32)product >> 12));
            }
            allowed = 1;
            for (category = 0; category < 2 && allowed != 0; ++category) {
                other = object_lookup(category == 0 ? 3 : 1);
                while (other != 0) {
                    x = r_u32(other + 8);
                    coordinate = r_u32(effect);
                    y = r_u32(other + 12);
                    product = r_u32(effect + 4);
                    z = r_u32(effect + 8);
                    x -= coordinate;
                    coordinate = r_u32(other + 16);
                    y -= product;
                    distance = vector_length_approx((sint32)x, (sint32)y, (sint32)(coordinate - z));
                    w_u32(0x80046174, (uint32)distance);
                    if (distance < 4000) {
                        allowed = 0;
                        break;
                    }
                    other = r_u32(other + 0x1DC);
                }
            }
            if (allowed != 0) {
                other = r_u32(0x800F337C);
                while (other != 0) {
                    if (other != effect) {
                        x = r_u32(other);
                        coordinate = r_u32(effect);
                        y = r_u32(other + 4);
                        product = r_u32(effect + 4);
                        z = r_u32(effect + 8);
                        x -= coordinate;
                        coordinate = r_u32(other + 8);
                        y -= product;
                        distance = vector_length_approx((sint32)x, (sint32)y, (sint32)(coordinate - z));
                        w_u32(0x80046174, (uint32)distance);
                        if (distance < 4000) {
                            allowed = 0;
                            break;
                        }
                    }
                    other = r_u32(other + 0x50);
                }
            }
            if (allowed == 0) {
                ++attempts;
                if ((sint16)attempts >= 10)
                    allowed = 1;
            }
        } while (allowed == 0);
        payload = r_u32(effect + 0x6C);
        w_u16(effect + 0x34, 0);
        w_u32(payload, 0x800ED46C);
        w_u32(effect + 0x60, 0x800B2520);
        w_u32(effect + 0x2C, 0x5F5E0FF);
    }
}

void game_exit_level_script(void)
{
    FUNCTION_MARKER(0x80093D78, "GAME.OVL");
    exit(0);
}

static void game_dispatch_level_command_native(uint32 target, const uint32 *arguments, uint32 first)
{
    switch (target) {
    case 0x80093274:
        game_config_select_ship(arguments, first);
        break;
    case 0x80093318:
        game_config_set_ship_model(arguments, first);
        break;
    case 0x800935D4:
        game_config_set_ship_type(arguments, first);
        break;
    case 0x80093668:
        game_config_set_unique_ship_name(arguments, first);
        break;
    case 0x80093700:
        game_config_set_ship_intelligence(arguments, first);
        break;
    case 0x80093754:
        game_config_set_ship_shield(arguments, first);
        break;
    case 0x800937A8:
        game_config_fire();
        break;
    case 0x800937B0:
        game_config_set_ship_velocity(arguments, first);
        break;
    case 0x800938A8:
        game_config_set_ship_sensor(arguments, first);
        break;
    case 0x8009391C:
        game_config_set_ship_hull(arguments, first);
        break;
    case 0x80093970:
        game_config_set_ship_scale(arguments, first);
        break;
    case 0x80093C50:
        game_config_set_ship_size(arguments, first);
        break;
    case 0x80093C48:
        game_config_objective_text();
        break;
    case 0x80093CA4:
        game_config_select_task_list(arguments, first);
        break;
    case 0x80093CE0:
        game_config_append_task_id(arguments, first);
        break;
    case 0x80093D98:
        game_config_create_ship(arguments, first);
        break;
    case 0x80094174:
        game_config_create_asteroid_controller(arguments, first);
        break;
    case 0x8009432C:
        game_config_add_waypoint(arguments, first);
        break;
    case 0x800945F4:
        game_config_set_player_launch(arguments, first);
        break;
    case 0x8009463C:
        game_config_bind_ship_name(arguments, first);
        break;
    case 0x80094728:
        game_config_add_ship_name(arguments, first);
        break;
    case 0x80095428:
        game_config_create_planet(arguments, first);
        break;
    case 0x80095AE8:
        game_config_set_scene_boundary(arguments, first);
        break;
    case 0x80095E0C:
        game_config_create_asteroid_belt(arguments, first);
        break;
    case 0x80095F48:
        game_config_create_mine_field(arguments, first);
        break;
    case 0x800952F8:
        game_config_begin_task_commands();
        break;
    case 0x8009531C:
        game_config_append_task_command(arguments, first);
        break;
    case 0x800947F0:
        game_config_destroy_ship_objective(arguments, first);
        break;
    case 0x800948AC:
        game_config_time_greater_objective(arguments, first);
        break;
    case 0x8009503C:
        game_config_navy_defeated_objective();
        break;
    case 0x800950B8:
        game_config_league_defeated_objective();
        break;
    case 0x80094998:
        game_config_time_less_objective(arguments, first);
        break;
    case 0x80094A5C:
        game_config_ship_finished_objective(arguments, first);
        break;
    case 0x80094B48:
        game_config_ships_destroyed_objective(arguments, first);
        break;
    case 0x80094CA0:
        game_config_ships_docked_objective(arguments, first);
        break;
    case 0x800939C4:
        game_config_set_hoop_path(arguments, first);
        break;
    case 0x80093B50:
        game_config_add_jumpgate_objective();
        break;
    case 0x80093BCC:
        game_config_add_drone_objective();
        break;
    case 0x80095134:
        game_config_ship_disabled_objective(arguments, first);
        break;
    case 0x8009527C:
        game_config_planet_destroyed_objective();
        break;
    case 0x80094DF8:
        game_config_add_objective(arguments, first);
        break;
    case 0x80094F20:
        game_config_set_mission_ending(arguments, first);
        break;
    case 0x80094434:
        game_config_create_training_hoop(arguments, first);
        break;
    case 0x800963F0:
        game_config_add_message(arguments, first);
        break;
    case 0x80096718:
        game_config_set_token_mode(arguments, first);
        break;
    case 0x80095BA0:
        game_config_create_ship_boundary(arguments, first);
        break;
    case 0x80095C4C:
        game_config_no_navy_fighters_objective();
        break;
    case 0x80093D78:
        game_exit_level_script();
        break;
    default:
        abort();
    }
}

void game_load_level_script(uint32 filename)
{
    uint32 arguments[1501];
    uint32 line, length, size, data, loaded, cursor, end, count, text, callback;
    sint32 command;
    const char *name;
    FUNCTION_MARKER(0x8009674C, "GAME.OVL");
    line = heap_alloc_front(3000);
    length = 0;
    while (r_u8(filename + length) != 0)
        ++length;
    name = (const char *)psx_addr(filename, length + 1);
    size = disc_file_size(name);
    if (size == 0)
        diagnostic_report(0, 0x80075260, 0x80074F80);
    data = heap_alloc_front(size + 4);
    loaded = resource_load_into(name, data);
    cursor = data;
    end = data + loaded;
    do {
        cursor = config_read_line(cursor, line);
        if (r_u8(line) != ';') {
            count = config_split_arguments(line, arguments);
            if ((sint32)count >= 71)
                diagnostic_report(count, 0x80075270, 0x80074F80);
            if ((sint32)count > 0) {
                text = arguments[0];
                if (r_u32(0x800ED464) == 0) {
                    command = string_table_find(game_level_guest_text_native(text), 0x800EDB38);
                    if (command == -1)
                        diagnostic_report(666, 0x80075280, filename);
                } else {
                    command = game_level_argument_integer_native(text);
                }
                callback = r_u32(0x800EDBF8 + ((uint32)command << 2));
                game_dispatch_level_command_native(callback, arguments, 0);
            }
        }
    } while (cursor < end);
    heap_free(data);
    heap_free(line);
}

uint32 game_create_belt_asteroid(uint32 controller)
{
    uint32 models[2], selected, effect, payload, next_payload, value, yaw, pitch, roll;
    uint32 column, coefficient, product, position, speed;
    MATRIX first, second;
    FUNCTION_MARKER(0x800A54F0, "GAME.OVL");
    models[0] = r_u32(0x800EDFCC);
    models[1] = r_u32(0x800EDFD0);
    selected = (uint32)random_between(0, 1);
    effect = game_create_model_effect(controller + 0x214, (sint16)models[selected]);
    payload = r_u32(effect + 0x6C);
    w_u16(effect + 0x3C, 100);
    w_u32(payload, 0x800EEEF0);
    value = (uint32)random_signed_between(3, 24);
    payload = r_u32(effect + 0x6C);
    w_u16(payload + 0x34, value);
    value = (uint32)random_signed_between(3, 24);
    payload = r_u32(effect + 0x6C);
    w_u16(payload + 0x36, value);
    value = (uint32)random_signed_between(3, 24);
    payload = r_u32(effect + 0x6C);
    w_u16(payload + 0x38, value);
    value = (uint32)random_signed_between(0, 50);
    yaw = (r_u16(controller + 0x224) + value) & 0xFFF;
    value = (uint32)random_signed_between(0, 50);
    pitch = (r_u16(controller + 0x226) + value) & 0xFFF;
    value = (uint32)random_signed_between(0, 50);
    roll = (r_u16(controller + 0x228) + value) & 0xFFF;
    payload = r_u32(effect + 0x6C);
    matrix_rotate_y(&first, (MATRIX *)psx_addr(payload + 4, 20), 0u - yaw);
    matrix_rotate_x(&second, &first, pitch);
    payload = r_u32(effect + 0x6C);
    matrix_rotate_z((MATRIX *)psx_addr(payload + 4, 20), &second, 0u - roll);
    for (column = 0; column < 2; ++column) {
        value = (uint32)random_signed_between(0, r_s32(controller + 0xA8));
        payload = r_u32(effect + 0x6C);
        coefficient = (uint32)r_s16(payload + 4 + column * 2);
        product = coefficient * value;
        next_payload = r_u32(effect + 0x6C);
        position = r_u32(effect);
        w_u32(effect, position + product);
        coefficient = (uint32)r_s16(next_payload + 10 + column * 2);
        product = coefficient * value;
        payload = r_u32(effect + 0x6C);
        position = r_u32(effect + 4);
        w_u32(effect + 4, position + product);
        coefficient = (uint32)r_s16(payload + 16 + column * 2);
        product = coefficient * value;
        position = r_u32(effect + 8);
        if (column == 1)
            w_u32(effect + 0x68, 0x800EEC8C);
        w_u32(effect + 8, position + product);
    }
    value = r_u32(controller + 0xB0);
    if ((sint32)value >= 51)
        speed = (uint32)random_between((sint32)(value - 40), (sint32)(value + 150));
    else
        speed = 0;
    payload = r_u32(effect + 0x6C);
    coefficient = (uint32)r_s16(payload + 8);
    product = coefficient * speed;
    next_payload = r_u32(effect + 0x6C);
    w_u32(effect + 0x20, (uint32)((sint32)product >> 12));
    coefficient = (uint32)r_s16(next_payload + 14);
    product = coefficient * speed;
    payload = r_u32(effect + 0x6C);
    w_u32(effect + 0x24, (uint32)((sint32)product >> 12));
    coefficient = (uint32)r_s16(payload + 20);
    product = coefficient * speed;
    w_u32(effect + 0x28, (uint32)((sint32)product >> 12));
    value = (uint32)random_between(-3, 0);
    w_u16(effect + 0x34, value);
    w_u32(effect + 0x60, 0x800B2520);
    w_u32(effect + 0x2C, 0x168);
    w_u32(effect + 0x30, 0x7FFFFFF);
    value = r_u32(0x800595CC);
    w_u32(0x800595CC, value + 1);
    return effect;
}

uint32 game_spawn_script_ship(uint32 ship, uint32 x, uint32 y, uint32 z, uint32 yaw, uint32 pitch, uint32 roll)
{
    uint32 offset = ship * 60, type, counter, object, frame, flags, render_flags, selector, allocation;
    sint32 model;
    FUNCTION_MARKER(0x80096D18, "GAME.OVL");
    type = r_u32(0x80053698 + offset);
    if (type == 3 || type == 1) {
        if (type == 3)
            counter = r_s16(0x800536BC + offset) == 2 ? 0x800595EC : 0x800595E8;
        else
            counter = r_s16(0x800536BC + offset) == 2 ? 0x800595EE : 0x800595EA;
        w_u16(counter, r_u16(counter) + 1);
    }
    type = r_u32(0x80053698 + offset);
    model = r_s16(0x800536B8 + offset);
    object = game_create_object(model, x, y, z, type);
    w_u16(0x800536C2 + offset, r_u16(0x800536C2 + offset) + 1);
    w_u8(object + 0x273, 0);
    frame = r_u32(0x800311B8);
    w_u32(object + 0x3C, 0x8002D2BC);
    w_u32(object + 0xA0, 6);
    w_u32(object + 0x54, 0x80097198);
    w_u32(object + 0x58, 0x800971CC);
    w_u32(object + 0x44, 0x8009FA2C);
    w_u32(object + 0x5C, 0x80097200);
    w_u32(object + 0x4C, 0x8002493C);
    w_u32(object + 0xE0, 0xFFFFFF85);
    w_u32(object + 0x40, 0x80096B18);
    w_u32(object + 0x114, 0);
    w_u32(object + 4, ship);
    w_u32(object + 0x178, 0);
    w_u32(object + 0x170, 0);
    w_u32(object + 0x1A8, 0);
    w_u32(object + 0x20, yaw);
    w_u32(object + 0x24, pitch);
    w_u32(object + 0x28, roll);
    w_u32(object + 0x214, 0);
    w_u32(object + 0x218, 0);
    w_u32(object + 0x21C, 0);
    w_u16(object + 0x224, 0);
    w_u16(object + 0x226, 0);
    w_u16(object + 0x228, 0);
    w_u32(object + 0x23C, 0);
    w_u32(object + 0x240, 0);
    w_u16(object + 0x248, 0);
    w_u32(object + 0x9C, 1);
    w_u32(object + 0x8C, frame);
    w_u32(object + 0xF8, frame);
    w_u8(object + 0x26D, 0);
    w_u16(object + 0x264, 0);
    w_u16(object + 0x268, 0);
    w_u32(object + 0x20C, 0);
    w_u32(object + 0x22C, 0);
    w_u32(object + 0x188, 0);
    w_u32(0x800F80FC, object);
    game_apply_script_ship_properties(object);
    if (r_s16(0x800536BC + offset) == 2) {
        w_u32(object + 0x1E4, 0x800EEC4C);
        flags = r_u32(object + 0x114);
        render_flags = r_u32(object + 0x70);
        w_u32(object + 0x114, flags | 0x10);
        w_u32(object + 0x70, render_flags | 0x80);
    } else {
        flags = r_u32(object + 0x114);
        if (flags & 0x10)
            w_u32(object + 0x114, flags ^ 0x10);
        w_u32(object + 0xD8, 0xFDE8);
        w_u32(object + 0x84, 0xFFFFFFFD);
        w_u32(object + 0x1E4, 0x800EEC5C);
    }
    type = r_u32(0x80053698 + offset);
    if (type == 3 || type == 1) {
        counter = type == 3 ? 0x800595B0 : 0x800595B4;
        w_u32(counter, r_u32(counter) + 1);
    }
    w_u16(object + 0x24C, 60);
    w_u32(object + 0x70, r_u32(object + 0x70) | 0x100000);
    matrix_set_identity(object + 0x11C);
    game_rotate_object_basis(object);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x28, 0);
    w_u32(object + 0x24, 0);
    w_u32(object + 0x20, 0);
    w_u32(object + 0x1B8, 0);
    w_u32(object + 0x200, 0);
    w_u32(object + 0x204, 0);
    w_u16(object + 0x1C4, 0);
    w_u32(object + 0x1CC, 0);
    w_u32(object + 0x208, 0);
    w_u8(object + 0x26E, 0);
    w_u8(object + 0x26F, 0);
    w_u8(object + 0x270, 0);
    w_u32(object + 0x210, 0);
    w_u8(object + 0x271, 0);
    w_u8(object + 0x272, 0);
    w_u32(object + 0x68, 0);
    if (flags & 0x10)
        w_u32(object + 0x68, 1);
    w_u8(object + 0x275, 0);
    w_u8(object + 0x276, 0);
    selector = r_u32(0x800ED460);
    w_u32(object + 0x1E8, 0);
    w_u32(object + 0x1EC, 0);
    w_u32(object + 0x1F0, 0);
    w_u32(object + 0x1F4, 0);
    w_u8(object + 0x277, selector);
    type = r_u32(0x80053698 + offset);
    if (type == 1 && r_s16(0x800595EA) >= 2 && (selector & 0xFF) == r_u32(0x800ED460)) {
        do {
            selector = (uint32)random_between(0, 3);
            w_u8(object + 0x277, selector);
        } while ((selector & 0xFF) == r_u32(0x800ED460));
    }
    if (r_u32(object) == 3 && !(r_u32(object + 0x114) & 0x10)) {
        allocation = heap_alloc_back(0x14);
        w_u32(object + 0x278, allocation);
        w_u16(allocation + 0x10, 1);
        allocation = r_u32(object + 0x278);
        w_u32(allocation, 0);
    }
    return object;
}
