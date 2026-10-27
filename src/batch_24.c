#include "game_functions.h"

sint32 game_count_active_standard_objects(void)
{
    uint32 object, count = 0u;
    FUNCTION_MARKER(0x800A2C4C, "GAME.OVL");
    object = object_lookup(1u);
    while (object != 0u) {
        if ((r_u32(object + 0x114u) & 16u) == 0u && r_u32(object) != 0x66u)
            ++count;
        object = r_u32(object + 0x1DCu);
    }
    return (sint32)(sint16)(uint16)count;
}

void game_initialize_model_table(void)
{
    uint32 entry, destination, table, index;
    FUNCTION_MARKER(0x800830CC, "GAME.OVL");
    entry = r_u32(0x800F3D98u);
    while (r_u32(entry) != 0u) {
        destination = r_u32(entry + 4u);
        entry += 8u;
        w_u32(destination, 0u);
    }
    table = heap_alloc_back(0x1C93u);
    w_u32(0x80046178u, table);
    for (index = 0u; index < 55u; ++index) {
        w_u32(table, 0u);
        w_u32(table + 0x74u, 0u);
        w_u32(table + 0x70u, 0u);
        w_u32(table + 4u, 0u);
        w_u32(table + 8u, 0u);
        w_u16(table + 0x80u, 0u);
        table += 132u;
    }
}

void game_canonicalize_angle_pair(uint32 first_angle, uint32 second_angle)
{
    uint32 value, second;
    FUNCTION_MARKER(0x800834CC, "GAME.OVL");
    value = r_u32(first_angle) & 0xFFFu;
    w_u32(first_angle, value);
    second = r_u32(second_angle) & 0xFFFu;
    w_u32(second_angle, second);
    if (second - 0x400u < 0x401u) {
        value = (r_u32(first_angle) + 0x800u) & 0xFFFu;
        w_u32(first_angle, value);
        value = 0x800u - r_u32(second_angle);
        w_u32(second_angle, value & 0xFFFu);
    } else if (second - 0x800u < 0x401u) {
        value = (r_u32(first_angle) + 0x800u) & 0xFFFu;
        w_u32(first_angle, value);
        value = r_u32(second_angle) - 0x800u;
        w_u32(second_angle, value & 0xFFFu);
    }
}

void game_set_object_rotation(uint32 object, uint32 yaw, uint32 pitch, uint32 roll)
{
    MATRIX first, second;
    MATRIX *matrix;
    FUNCTION_MARKER(0x80083A7C, "GAME.OVL");
    matrix = (MATRIX *)psx_addr(object + 0x11Cu, 20u);
    matrix_set_identity_native(matrix);
    matrix_rotate_y(&first, matrix, 0u - yaw);
    matrix_rotate_x(&second, &first, pitch);
    matrix_rotate_z(matrix, &second, 0u - roll);
}

void game_fire_antishield_projectile(uint32 object)
{
    uint32 owner, target, offset, projectile, basis, velocity;
    FUNCTION_MARKER(0x800803C4, "GAME.OVL");
    owner = r_u32(object + 0x110u);
    target = r_u32(object + 0x178u);
    offset = r_u32(owner + 0x200u);
    projectile = game_create_weapon_projectile(object, target, offset, 0u);
    w_u32(projectile + 0x1E4u, 0x800EEBFCu);
    w_u32(projectile, 0xCEu);
    w_u32(projectile + 0x3Cu, 0x800B37B0u);
    w_u32(projectile + 0x38u, 0x800B3C08u);
    basis = (uint32)(sint32)r_s16(object + 0x120u) << 6;
    velocity = r_u32(object + 0x14u);
    velocity -= basis;
    basis = (uint32)(sint32)r_s16(object + 0x126u) << 6;
    w_u32(object + 0x14u, velocity);
    velocity = r_u32(object + 0x18u);
    velocity -= basis;
    basis = (uint32)(sint32)r_s16(object + 0x12Cu) << 6;
    w_u32(object + 0x18u, velocity);
    velocity = r_u32(object + 0x1Cu);
    w_u32(object + 0x1Cu, velocity - basis);
}

void game_initialize_object_profile_29a(uint32 object)
{
    FUNCTION_MARKER(0x80083B00, "GAME.OVL");
    w_u32(object + 0x3Cu, 0x8002493Cu);
    w_u32(object + 0x5Cu, 0x800D332Cu);
    w_u32(object + 0x48u, 0x800D3478u);
    w_u32(object, 0xFACEu);
    w_u32(object + 0x238u, 0x29Au);
}

uint32 game_create_object_link_native(uint32 source, uint32 target, const VECTOR *origin, const VECTOR *first_offset, const VECTOR *second_offset)
{
    uint32 object, record, flags, value, serial, first, adjustment, x, y, z;
    uint32 second_x, second_y, second_z, delta_x, delta_y, delta_z, player, color;
    sint32 distance;
    FUNCTION_MARKER(0x80083BA8, "GAME.OVL");
    object = game_create_object(0u, 0u, 0u, 0u, 0u);
    w_u32(object + 0x40u, 0x80083F5Cu);
    w_u32(object + 0x88u, 0u);
    w_u32(object + 0x48u, 0x80083F24u);
    value = r_u32(source + 0x84u);
    w_u32(object + 0x84u, value);
    record = heap_alloc_back(0x7Cu);
    w_u32(object + 0xA8u, record);
    flags = r_u32(source + 0x114u);
    if ((sint32)flags < 0)
        value = r_u32(0x800EEB7Cu);
    else if ((flags & 0x08000000u) != 0u)
        value = r_u32(0x800EEB90u);
    else
        value = r_u32(0x800EEB80u);
    w_u16(record + 0x60u, value);
    w_u32(record, source);
    serial = r_u16(source + 0x24Au);
    w_u32(record + 8u, target);
    w_u16(record + 4u, serial);
    serial = r_u16(target + 0x24Au);
    w_u16(record + 0xCu, serial);
    w_u32(record + 0x10u, (uint32)origin->vx);
    w_u32(record + 0x14u, (uint32)origin->vy);
    w_u32(record + 0x18u, (uint32)origin->vz);
    value = (uint32)origin->vx;
    first = (uint32)first_offset->vx;
    adjustment = (uint32)((sint32)(value + first) >> 7);
    w_u32(record + 0x20u, first - adjustment);
    value = (uint32)first_offset->vy;
    w_u32(record + 0x24u, value - adjustment);
    value = (uint32)first_offset->vz;
    w_u32(record + 0x28u, value - adjustment);
    x = (uint32)origin->vx;
    second_x = (uint32)second_offset->vx;
    y = (uint32)origin->vy;
    z = (uint32)origin->vz;
    second_z = (uint32)second_offset->vz;
    delta_x = (uint32)((sint32)(x - second_x) >> 7);
    delta_z = (uint32)((sint32)(z - second_z) >> 7);
    second_y = (uint32)second_offset->vy;
    w_u32(record + 0x30u, second_x - delta_x);
    delta_y = (uint32)((sint32)(y - second_y) >> 7);
    second_y = (uint32)second_offset->vy;
    w_u32(record + 0x34u, second_y - delta_y);
    second_z = (uint32)second_offset->vz;
    w_u32(record + 0x50u, 1u);
    w_u16(record + 0x5Eu, 0u);
    w_u16(record + 0x58u, 1u);
    w_u32(record + 0x38u, second_z - delta_z);
    w_u16(record + 0x5Au, 1u);
    player = r_u32(0x80040FDCu);
    if (target != player && r_s32(source + 0x114u) < 0 && target != r_u32(source + 0x188u)) {
        w_u32(record + 0x64u, 255u);
        w_u32(record + 0x68u, 0x202020u);
        color = 0x226464u;
    } else {
        if (r_u32(source) == 3u) {
            value = 0xDDDDDDu;
            color = 0xFFAA22u;
        } else {
            value = 0xFFFFFFu;
            color = 0x44FF44u;
        }
        w_u32(record + 0x64u, value);
        w_u32(record + 0x68u, 0u);
    }
    w_u32(record + 0x6Cu, color);
    w_u32(record + 0x70u, 0u);
    player = r_u32(0x80040FDCu);
    if (target == player) {
        w_u32(record + 0x74u, 12000u);
        value = 60000u;
    } else {
        w_u32(record + 0x74u, 1000u);
        value = 5000u;
    }
    w_u32(record + 0x78u, value);
    player = r_u32(0x80040FDCu);
    x = r_u32(source + 8u);
    y = r_u32(source + 12u);
    second_x = r_u32(player + 8u);
    second_y = r_u32(player + 12u);
    second_z = r_u32(player + 16u);
    z = r_u32(source + 16u);
    distance = vector_length_approx((sint32)(x - second_x), (sint32)(y - second_y), (sint32)(z - second_z));
    w_u32(0x80046174u, (uint32)distance);
    value = 22000u;
    if (distance <= 750000) {
        player = r_u32(0x80040FDCu);
        if (target == player) {
            if (distance <= 249999)
                value = 6000u;
            else if (distance > 339999)
                value = 20000u;
            else
                value = 12000u;
        } else {
            x = r_u32(target + 8u);
            second_x = r_u32(player + 8u);
            y = r_u32(target + 12u);
            second_y = r_u32(player + 12u);
            second_z = r_u32(player + 16u);
            z = r_u32(target + 16u);
            distance = vector_length_approx((sint32)(x - second_x), (sint32)(y - second_y), (sint32)(z - second_z));
            w_u32(0x80046174u, (uint32)distance);
            if (distance <= 750000)
                value = 14200u;
        }
    }
    w_u16(record + 0x62u, value);
    return value;
}

uint32 game_release_object_link(uint32 object)
{
    uint32 record;
    FUNCTION_MARKER(0x80083F24, "GAME.OVL");
    record = r_u32(object + 0xA8u);
    heap_free(record);
    return game_mark_object_for_removal(object);
}

void game_draw_radial_glow_native(const VECTOR *position, uint32 radius, uint32 color, uint32 perspective)
{
    SVECTOR point;
    sint32 screen, fog, flag, quarter_depth;
    uint32 camera, x, y, z, magnitude_x, magnitude_y, magnitude_z, shift = 0;
    uint32 depth, divisor, packet, phase = 0, index, angle, product, value, center_x, center_y;
    FUNCTION_MARKER(0x800849F4, "GAME.OVL");
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    camera = r_u32(0x80040C94);
    x = (uint32)position->vx - r_u32(camera + 8);
    y = (uint32)position->vy - r_u32(camera + 12);
    z = (uint32)position->vz - r_u32(camera + 16);
    for (;;) {
        magnitude_x = (sint32)x < 0 ? 0 - x : x;
        magnitude_y = (sint32)y < 0 ? 0 - y : y;
        magnitude_z = (sint32)z < 0 ? 0 - z : z;
        if ((sint32)magnitude_x < 0x4000 && (sint32)magnitude_y < 0x4000 && (sint32)magnitude_z < 0x4000)
            break;
        ++shift;
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
    }
    point.vx = (sint16)x;
    point.vy = (sint16)y;
    point.vz = (sint16)z;
    quarter_depth = RotTransPers(&point, &screen, &fog, &flag);
    depth = (uint32)quarter_depth << (((uint32)(sint32)(sint16)shift + 2) & 31);
    if ((sint32)depth < 26 || ((uint32)flag & 0xFFFF0000) != 0)
        return;
    if ((uint16)perspective != 0) {
        divisor = (uint32)((sint32)depth >> 4);
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && radius == 0x80000000)
            xport_mips_break(0x5D);
        radius = (uint32)((sint64)(sint32)radius / (sint32)divisor);
    }
    center_x = (uint16)screen;
    center_y = (uint16)((uint32)screen >> 16);
    for (index = 0; index < 12; ++index) {
        packet = r_u32(0x80040690);
        w_u32(0x80040690, packet + 28);
        w_u32(packet + 4, color);
        w_u32(packet + 12, 0);
        w_u32(packet + 20, 0);
        setPolyG3((POLY_G3 *)psx_addr(packet, 28));
        angle = (uint32)(sint32)(sint16)phase;
        w_u16(packet + 8, center_x);
        w_u16(packet + 10, center_y);
        product = radius * (uint32)rcos((sint32)angle);
        value = center_x + (uint32)((sint32)product >> 12);
        w_u16(packet + 16, value);
        product = radius * (uint32)rsin((sint32)angle);
        angle += 0x155;
        value = center_y + (uint32)((sint32)product >> 13);
        w_u16(packet + 18, value);
        product = radius * (uint32)rcos((sint32)angle);
        value = center_x + (uint32)((sint32)product >> 12);
        w_u16(packet + 24, value);
        product = radius * (uint32)rsin((sint32)angle);
        phase += 0x155;
        value = r_u8(packet + 7);
        w_u8(packet + 7, value | 2);
        value = center_y + (uint32)((sint32)product >> 13);
        w_u16(packet + 26, value);
        render_insert_primitive((sint32)depth, packet);
    }
}

sint32 game_choose_object_link_offsets_native(uint32 source, uint32 target, VECTOR *first_output, VECTOR *second_output)
{
    VECTOR relative, local;
    uint32 model, models, descriptor, grid, axis, scale, shift, components[3];
    uint32 axial, atan_y, atan_x, extent, upper, lower, value, denominator, row, column;
    uint32 mesh, vertex, selector, frame;
    sint32 coordinate;
    FUNCTION_MARKER(0x80084D60, "GAME.OVL");
    model = r_u32(source + 0x74);
    value = r_u32(source + 8);
    models = r_u32(0x80046178);
    descriptor = models + model * 132;
    relative.vx = (sint32)(r_u32(target + 8) - value);
    grid = r_u32(descriptor + 8);
    relative.vy = (sint32)(r_u32(target + 12) - r_u32(source + 12));
    relative.vz = (sint32)(r_u32(target + 16) - r_u32(source + 16));
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(source + 0x11C, 18), &relative, &local);
    scale = r_u32(source + 0x84);
    components[0] = (uint32)local.vx;
    components[1] = (uint32)local.vy;
    components[2] = (uint32)local.vz;
    if ((sint32)scale > 0) {
        components[0] = (uint32)((sint32)components[0] >> (scale & 31));
        for (axis = 1; axis < 3; ++axis) {
            scale = r_u32(source + 0x84);
            components[axis] = (uint32)((sint32)components[axis] >> (scale & 31));
        }
    } else if ((sint32)scale < 0) {
        components[0] <<= ((0 - scale) & 31);
        for (axis = 1; axis < 3; ++axis) {
            scale = r_u32(source + 0x84);
            shift = (sint32)scale < 0 ? 0 - scale : scale;
            components[axis] <<= (shift & 31);
        }
    }
    axis = r_u32(grid);
    if (axis == 0) {
        axial = components[0];
        atan_x = components[1];
        atan_y = components[2];
    } else if (axis == 1) {
        axial = components[1];
        atan_x = components[0];
        atan_y = components[2];
    } else {
        axial = components[2];
        atan_x = components[1];
        atan_y = components[0];
    }
    extent = r_u32(grid + 4);
    lower = (uint32)((sint32)(0 - extent) >> 1);
    upper = (uint32)((sint32)extent >> 1);
    row = 0;
    if ((sint32)lower < (sint32)axial) {
        row = 15;
        if ((sint32)axial < (sint32)upper) {
            value = 1;
            if ((sint32)axial < 0) {
                value = (0 - upper) - axial;
                if ((sint32)value < 0)
                    value = 0 - value;
            } else if ((sint32)axial > 0) {
                value = upper + axial;
            }
            denominator = r_u32(grid + 4);
            value <<= 4;
            if (denominator == 0)
                xport_mips_break(7);
            if (denominator == 0xFFFFFFFF && value == 0x80000000)
                xport_mips_break(0x5D);
            row = (uint32)((sint64)(sint32)value / (sint32)denominator);
            if ((sint32)row >= 16)
                row = 15;
            if ((sint32)row < 0)
                row = 0;
        }
    }
    column = (uint32)(ratan2((sint32)atan_y, (sint32)atan_x) >> 8);
    if ((sint32)column < 0)
        column += 15;
    model = r_u32(source + 0x74);
    models = r_u32(0x80046178);
    descriptor = models + model * 132;
    mesh = r_u32(descriptor);
    value = (uint32)(sint32)r_s16(grid + row * 34 + column * 2 + 8);
    vertex = r_u32(mesh) + (value << 3);
    first_output->vx = r_s16(vertex);
    first_output->vy = r_s16(vertex + 2);
    first_output->vz = r_s16(vertex + 4);
    selector = (uint32)random_between(0, 2) & 255;
    frame = r_u32(0x800311B8);
    if (selector == 0) {
        column += (frame & 3) != 0 ? -7 : 7;
    } else if (selector == 1) {
        row += (frame & 3) != 0 ? -7 : 7;
    } else {
        value = (frame & 3) != 0 ? -7 : 7;
        row += value;
        column += value;
    }
    if ((sint32)column < 0)
        column += 15;
    if ((sint32)column >= 16)
        column -= 15;
    if ((sint32)row < 0)
        row += 15;
    if ((sint32)row >= 16)
        row -= 15;
    value = (uint32)(sint32)r_s16(grid + row * 34 + column * 2 + 8);
    vertex = r_u32(mesh) + (value << 3);
    second_output->vx = r_s16(vertex);
    second_output->vy = r_s16(vertex + 2);
    coordinate = r_s16(vertex + 4);
    second_output->vz = coordinate;
    return coordinate;
}

void game_update_link_audio_native(const VECTOR *position)
{
    VECTOR relative;
    uint32 camera, player, value, component, sum, numerator, denominator, pan = 15, volume, voice;
    sint32 distance, length;
    FUNCTION_MARKER(0x800E05E0, "GAME.OVL");
    camera = r_u32(0x80040C94);
    value = (uint32)position->vx - r_u32(camera + 8);
    relative.vx = (sint32)value >> 2;
    value = (uint32)position->vy;
    component = r_u32(camera + 12);
    w_u32(0x800F159C, 1);
    relative.vy = (sint32)(value - component) >> 2;
    value = (uint32)position->vz;
    player = r_u32(0x80040FDC);
    component = r_u32(camera + 16);
    relative.vz = (sint32)(value - component) >> 2;
    ApplyMatrixLV((MATRIX *)psx_addr(player + 0x11C, 20), &relative, &relative);
    value = (uint32)relative.vx;
    value = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)value > 199999)
        return;
    value = (uint32)relative.vy;
    value = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)value > 199999)
        return;
    value = (uint32)relative.vz;
    value = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)value > 199999)
        return;
    distance = vector_length_approx(relative.vx, relative.vy, relative.vz);
    if (distance == 0) {
        volume = (uint32)(sint32)r_s16(0x800118FC);
    } else {
        sum = (uint32)relative.vx * (uint32)relative.vx;
        sum += (uint32)relative.vy * (uint32)relative.vy;
        sum += (uint32)relative.vz * (uint32)relative.vz;
        length = (sint32)SquareRoot0((sint32)sum);
        if (length == 0)
            length = 1;
        denominator = (uint32)length;
        numerator = (uint32)relative.vx << 12;
        if (denominator == 0)
            xport_mips_break(7);
        if (denominator == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(0x5D);
        value = (uint32)((sint64)(sint32)numerator / (sint32)denominator);
        pan = (uint32)(sint32)(sint8)sound_offset_to_pan((sint32)value);
        volume = 200000 - (uint32)distance;
        if ((sint32)volume < 0)
            volume = 0;
        volume = (uint32)((sint32)volume >> 9);
        if ((sint32)volume >= 256)
            volume = 255;
        value = (uint32)(sint32)r_s16(0x800118FC);
        value *= volume;
        volume = (uint32)((sint32)value >> 8);
    }
    voice = r_u32(0x800FD4F8);
    sound_set_voice_left(voice, pan & 255);
    voice = r_u32(0x800FD4F8);
    sound_set_voice_volume(voice, volume & 255);
}

void game_emit_sized_fragment_flash_native(const VECTOR *position, uint32 size)
{
    uint32 effect, value, z, phase;
    effect = game_allocate_flash(0);
    if (effect == 0)
        return;
    value = (uint32)position->vx;
    w_u32(effect, value);
    value = (uint32)position->vy;
    w_u32(effect + 4, value);
    z = (uint32)position->vz;
    w_u32(effect + 0x20, 0xC0C0C0);
    phase = r_u32(0x800313F8);
    w_u32(effect + 12, 384);
    w_u16(effect + 0x24, size);
    w_u16(effect + 0x26, 0);
    w_u16(effect + 0x28, 16);
    w_u16(effect + 0x2C, phase);
    w_u32(effect + 8, z);
}

uint32 game_apply_object_link_contact_native(uint32 object, uint32 record, const VECTOR *step)
{
    VECTOR impact;
    uint32 value, target, player, initial_health, seed, size, shift, source, flags, callback;
    uint32 x, y, z, dx, dy, dz, damage, shield, remaining;
    FUNCTION_MARKER(0x80085144, "GAME.OVL");
    value = (uint32)step->vx;
    target = r_u32(record + 8);
    x = r_u32(record + 0x40);
    player = r_u32(0x80040FDC);
    initial_health = r_u32(target + 0x238);
    impact.vx = (sint32)(x + (value << 1));
    value = (uint32)step->vy;
    impact.vy = (sint32)(r_u32(record + 0x44) + (value << 1));
    value = (uint32)step->vz;
    impact.vz = (sint32)(r_u32(record + 0x48) + (value << 1));
    if (target != player) {
        seed = r_u32(0x800EF50C);
        seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        size = (seed & 127) + 256;
        shift = r_u32(object + 0x84);
        w_u32(0x800EF50C, seed);
        game_emit_sized_fragment_flash(record + 0x40, size << (shift & 31));
    } else {
        game_set_screen_tint(230, 0, 0);
        game_apply_player_damage_native(7, &impact);
        player = r_u32(0x80040FDC);
        value = (uint32)step->vx << 3;
        x = r_u32(player + 0x14);
        w_u32(player + 0x14, x + value);
        value = (uint32)step->vy << 3;
        y = r_u32(player + 0x18);
        w_u32(player + 0x18, y + value);
        value = (uint32)step->vz << 3;
        z = r_u32(player + 0x1C);
        w_u32(player + 0x1C, z + value);
        sound_play_at_listener(player + 8, 4, player, r_u32(0x80040C94));
    }
    sound_play_sample(4);
    source = r_u32(record);
    value = r_u32(record + 0x40);
    dx = (uint32)((sint32)(r_u32(source + 8) - value) >> 2);
    source = r_u32(record);
    value = r_u32(record + 0x44);
    dy = (uint32)((sint32)(r_u32(source + 12) - value) >> 2);
    source = r_u32(record);
    value = r_u32(record + 0x48);
    dz = (uint32)((sint32)(r_u32(source + 16) - value) >> 2);
    x = r_u32(record + 0x40);
    y = r_u32(record + 0x44);
    z = r_u32(record + 0x48);
    game_emit_narrow_particles(20, x, y, z, dx, dy, dz);
    source = r_u32(record);
    flags = r_u32(source + 0x114);
    if ((flags & 0x01000000) != 0)
        goto contact_complete;
    if ((flags & 0x02000000) != 0 && r_u32(source + 0x188) == target) {
        if ((sint32)initial_health > 0) {
            w_u32(target + 0x234, 0xFFFFFFFF);
            w_u32(target + 0x238, 0xFFFFFFFF);
            goto contact_destroy;
        }
        goto contact_complete;
    }
    source = r_u32(record);
    if (r_s32(source + 0x114) < 0) {
        game_apply_sensor_damage(target, r_s16(record + 0x60));
        goto contact_complete;
    }
    damage = (uint32)(sint32)r_s16(record + 0x60);
    shield = r_u32(target + 0x234);
    remaining = damage;
    if ((sint32)shield > 0) {
        remaining = damage - shield;
        value = shield - damage;
        w_u32(target + 0x234, value);
        if ((sint32)value < 0)
            w_u32(target + 0x234, 0);
    }
    if ((sint32)remaining <= 0)
        goto contact_complete;
    value = r_u32(target + 0x238) - remaining;
    w_u32(target + 0x238, value);
    if ((sint32)value > 0 || (sint32)initial_health <= 0)
        goto contact_complete;
contact_destroy:
    game_record_destroyed_object(target, 0, 0);
    callback = r_u32(target + 0x5C);
    psx_dispatch_guest_call(callback, 1, target, 0, 0);
contact_complete:
    w_u32(record + 0x50, 1);
    w_u16(record + 0x5E, 0);
    return 1;
}

void game_reset_object_control_link(uint32 object)
{
    uint32 flags, link;
    FUNCTION_MARKER(0x8009ADC8, "GAME.OVL");
    flags = r_u32(object + 0x114);
    w_u32(object + 0xE0, 0x29A);
    if ((flags & 0x40) != 0) {
        link = r_u32(object + 0xA8);
        w_u32(object + 0x114, flags ^ 0x40);
        w_u16(link, 0);
    }
}

void game_apply_sensor_damage(uint32 object, sint32 damage)
{
    uint32 flags, kind, health, category, threshold, magnitude, format, serial, name, timer, attacker, player;
    FUNCTION_MARKER(0x8009F5C4, "GAME.OVL");
    flags = r_u32(object + 0x114);
    if ((flags & 0x400) != 0)
        return;
    kind = r_u32(object);
    if (kind == 0x6A)
        return;
    if ((flags & 16) == 0 && r_s32(object + 0x244) <= 0)
        return;
    w_u8(object + 0x276, 180);
    health = r_u32(object + 0x244);
    flags = r_u32(object + 0x114);
    health -= (uint32)(sint32)(sint16)damage;
    w_u32(object + 0x244, health);
    if ((flags & 0x40000000) != 0)
        return;
    if ((flags & 0x40800000) != 0)
        return;
    if ((flags & 0x00400000) != 0) {
        category = r_u32(object + 4);
        health = r_u32(object + 0x244);
        threshold = r_u32(0x800536AC + category * 60);
        magnitude = (sint32)health < 0 ? 0 - health : health;
        if ((sint32)threshold >= (sint32)magnitude)
            return;
        w_u32(object + 0x114, flags | 0x40002000);
        w_u32(object + 0x58, 0x8002493C);
        serial = (uint32)(sint32)r_s16(object + 0x24E);
        format = r_u32(0x800F239C);
        name = 0x80059E70 + serial * 44;
        sprintfPSX(0x800FA368, format, name);
        game_queue_hud_message(0x800FA368);
        w_u32(object + 0x40, 0x8009F9F8);
        return;
    }
    if (r_s32(object + 0x244) > 0)
        return;
    flags = r_u32(object + 0x114);
    w_u32(object + 0x244, 0);
    timer = (flags & 16) != 0 ? 900 : 600;
    w_u32(object + 0x9C, timer);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x40, 0x8009F84C);
    if ((flags & 16) != 0) {
        w_u32(object + 0xE8, 0);
    } else {
        category = r_u32(object + 4);
        health = r_u32(0x8005369C + category * 60);
        w_u32(object + 0xE8, (uint32)((sint32)health >> 2));
    }
    w_u32(object + 0x58, 0x8002493C);
    game_reset_object_control_link(object);
    flags = r_u32(object + 0x114);
    w_u32(object + 0x114, flags | 0x00482000);
    serial = (uint32)(sint32)r_s16(object + 0x24E);
    format = r_u32(0x800F23A0);
    name = 0x80059E70 + serial * 44;
    sprintfPSX(0x800FA368, format, name);
    game_queue_hud_message(0x800FA368);
    attacker = r_u32(object + 0x1A8);
    player = r_u32(0x80040FDC);
    if (attacker == player && (r_u32(object + 0x114) & 16) != 0)
        game_queue_hud_message(r_u32(0x800F23A4));
}

typedef struct GameLinkProjection {
    sint32 screen[3];
    sint32 flags;
    uint32 weighted_depth;
    sint32 outer_plus;
    sint32 outer_minus;
} GameLinkProjection;

static uint32 game_link_checked_add(uint32 first, uint32 second, uint32 pc)
{
    sint64 value = (sint64)(sint32)first + (sint32)second;
    if (value < -2147483647 - 1 || value > 2147483647)
        xport_mips_overflow_exception(pc);
    return (uint32)value;
}

static uint32 game_link_checked_subtract(uint32 first, uint32 second, uint32 pc)
{
    sint64 value = (sint64)(sint32)first - (sint32)second;
    if (value < -2147483647 - 1 || value > 2147483647)
        xport_mips_overflow_exception(pc);
    return (uint32)value;
}

static void game_project_link_section(const uint32 point[3], const uint32 thin[3], const uint32 wide[3], uint32 shift,
    uint32 plus_pc, uint32 minus_pc, uint32 depth_pc, uint32 wide_plus_pc, uint32 wide_minus_pc, GameLinkProjection *projection)
{
    SVECTOR vertices[3], outer;
    uint32 plus[3], minus[3], axis, value;
    sint32 depths[3], ignored_flags;
    vertices[0].vx = (sint16)point[0];
    vertices[0].vy = (sint16)point[1];
    vertices[0].vz = (sint16)point[2];
    for (axis = 0; axis < 3; ++axis)
        plus[axis] = game_link_checked_add(point[axis], thin[axis], plus_pc + axis * 4);
    vertices[1].vx = (sint16)plus[0];
    vertices[1].vy = (sint16)plus[1];
    vertices[1].vz = (sint16)plus[2];
    for (axis = 0; axis < 3; ++axis)
        minus[axis] = game_link_checked_subtract(point[axis], thin[axis], minus_pc + axis * 4);
    vertices[2].vx = (sint16)minus[0];
    vertices[2].vy = (sint16)minus[1];
    vertices[2].vz = (sint16)minus[2];
    gte_project3_full_depth(vertices, projection->screen, depths, &projection->flags);
    value = game_link_checked_add((uint32)depths[0], (uint32)depths[0], depth_pc);
    value = game_link_checked_add((uint32)depths[1], value, depth_pc + 4);
    value = game_link_checked_add((uint32)depths[2], value, depth_pc + 8);
    projection->weighted_depth = value << (shift & 31);
    for (axis = 0; axis < 3; ++axis)
        plus[axis] = game_link_checked_add(point[axis], wide[axis], wide_plus_pc + axis * 4);
    outer.vx = (sint16)plus[0];
    outer.vy = (sint16)plus[1];
    outer.vz = (sint16)plus[2];
    gte_project_full_depth(&outer, &projection->outer_plus, &ignored_flags);
    for (axis = 0; axis < 3; ++axis)
        minus[axis] = game_link_checked_subtract(point[axis], wide[axis], wide_minus_pc + axis * 4);
    outer.vx = (sint16)minus[0];
    outer.vy = (sint16)minus[1];
    outer.vz = (sint16)minus[2];
    gte_project_full_depth(&outer, &projection->outer_minus, &ignored_flags);
}

static void game_emit_link_quad(uint32 start_center, uint32 end_center, uint32 start_edge, uint32 end_edge,
    uint32 index, sint32 count, uint32 first_color, uint32 second_color, uint32 depth, uint32 index_pc)
{
    uint32 packet, value, base, slot;
    packet = r_u32(0x80040690);
    w_u32(packet + 8, start_center);
    w_u32(packet + 16, end_center);
    w_u32(packet + 24, index == 0 ? start_center : start_edge);
    value = game_link_checked_add(index, 1, index_pc);
    value = game_link_checked_subtract(value, (uint32)count, index_pc + 4);
    w_u32(packet + 32, value == 0 ? end_center : end_edge);
    w_u32(packet + 4, first_color);
    w_u32(packet + 12, first_color);
    w_u32(packet + 20, second_color);
    w_u32(packet + 28, second_color);
    w_u8(packet + 7, 0x3A);
    w_u8(packet + 3, 8);
    value = game_link_checked_add(packet, 36, index_pc + 0x50);
    w_u32(0x80040690, value);
    base = r_u32(0x800421AC);
    if ((sint32)depth >= 0x4000) {
        value = game_link_checked_subtract(depth, 0x3FFFFE, index_pc + 0x84);
        if ((sint32)value > 0)
            depth = 0x3FFFFE;
        value = game_link_checked_add(depth >> 11, 320, index_pc + 0x9C);
        slot = game_link_checked_add(value << 2, base, index_pc + 0xA4);
    } else {
        if ((sint32)depth < 0)
            depth = 0;
        value = game_link_checked_add(depth >> 6, 64, index_pc + 0xC0);
        slot = game_link_checked_add(value << 2, base, index_pc + 0xC8);
    }
    AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
}

void game_draw_object_link_segments_native(uint32 record, const VECTOR *start, const VECTOR *step, sint32 count, const VECTOR *cross)
{
    uint32 increment[3], current[3], next[3], scaled_start[3], scaled_end[3], normal[3], thin[3], wide[3];
    uint32 index = 0, have_output = 0, axis, value, magnitude, shift, mask, width, state, limit, delta, phase, sine, product, wave, color0, color1;
    GameLinkProjection first, second;
    FUNCTION_MARKER(0x800CD5F0, "GAME.OVL");
    increment[0] = (uint32)step->vx;
    increment[1] = (uint32)step->vy;
    increment[2] = (uint32)step->vz;
    current[0] = (uint32)start->vx;
    current[1] = (uint32)start->vy;
    current[2] = (uint32)start->vz;
    do {
        for (axis = 0; axis < 3; ++axis)
            next[axis] = game_link_checked_add(current[axis], increment[axis], 0x800CD660 + axis * 4);
        mask = 0;
        for (axis = 0; axis < 6; ++axis) {
            static const uint32 negate_pc[6] = { 0x800CD68C, 0x800CD698, 0x800CD6A8, 0x800CD6B8, 0x800CD6C8, 0x800CD6D8 };
            value = axis < 3 ? current[axis] : next[axis - 3];
            magnitude = (sint32)value > 0 ? value : game_link_checked_subtract(0, value, negate_pc[axis]);
            mask |= magnitude;
        }
        shift = 0;
        value = mask >> 14;
        if (value != 0) {
            do {
                value = (uint32)((sint32)value >> 1);
                shift = game_link_checked_add(shift, 1, 0x800CD6F0);
            } while ((sint32)value > 0);
        }
        for (axis = 0; axis < 3; ++axis) {
            scaled_start[axis] = (uint32)((sint32)current[axis] >> (shift & 31));
            scaled_end[axis] = (uint32)((sint32)next[axis] >> (shift & 31));
        }
        value = game_link_checked_add(shift, (uint32)-26, 0x800CD714);
        if ((sint32)value > 0)
            return;
        width = r_u32(record + 0x74);
        normal[0] = (uint32)cross->vx;
        normal[1] = (uint32)cross->vy;
        normal[2] = (uint32)cross->vz;
        state = (uint32)(sint32)r_s16(record + 0x58);
        limit = r_u32(record + 0x50);
        state = game_link_checked_add(state, (uint32)-2, 0x800CD740);
        limit = game_link_checked_add(limit, (uint32)-2, 0x800CD748);
        if (state == 0) {
            delta = game_link_checked_subtract(index, limit, 0x800CD74C);
            limit = game_link_checked_add(limit, (uint32)-3, 0x800CD754);
            if (delta == 0)
                width = game_link_checked_add(width, 1500, 0x800CD760);
        }
        product = normal[0] * width;
        phase = r_u32(0x800F4EF4);
        value = (phase & 0xFFF) << 1;
        value = game_link_checked_add(value, 0x8003DC68, 0x800CD784);
        sine = (uint32)(sint32)r_s16(value);
        shift = game_link_checked_add(shift, 12, 0x800CD7A0);
        thin[0] = (uint32)((sint32)product >> (shift & 31));
        thin[1] = (uint32)((sint32)(normal[1] * width) >> (shift & 31));
        thin[2] = (uint32)((sint32)(normal[2] * width) >> (shift & 31));
        sine = (uint32)((sint32)sine >> 11);
        shift = game_link_checked_add(shift, (uint32)-12, 0x800CD7D0);
        wave = sine * 950;
        if ((sint32)wave <= 0)
            wave = game_link_checked_subtract(0, wave, 0x800CD7EC);
        width = r_u32(record + 0x78);
        width = game_link_checked_add(width, wave, 0x800CD7F8);
        product = normal[0] * width;
        shift = game_link_checked_add(shift, 12, 0x800CD810);
        wide[0] = (uint32)((sint32)product >> (shift & 31));
        product = normal[1] * width;
        phase = game_link_checked_subtract(phase, 256, 0x800CD830);
        w_u32(0x800F4EF4, phase);
        wide[1] = (uint32)((sint32)product >> (shift & 31));
        wide[2] = (uint32)((sint32)(normal[2] * width) >> (shift & 31));
        shift = game_link_checked_add(shift, (uint32)-12, 0x800CD84C);
        if (have_output == 0) {
            game_project_link_section(scaled_start, thin, wide, shift,
                0x800CD86C, 0x800CD88C, 0x800CD910, 0x800CD924, 0x800CD964, &first);
        } else {
            first = second;
        }
        game_project_link_section(scaled_end, thin, wide, shift,
            0x800CD9FC, 0x800CDA1C, 0x800CDAA4, 0x800CDAB8, 0x800CDAF8, &second);
        if (((uint32)first.flags & (uint32)second.flags & 0x80460000) == 0 &&
            (sint32)first.weighted_depth > 0 && (sint32)second.weighted_depth > 0) {
            color0 = r_u32(record + 0x64);
            color1 = r_u32(record + 0x68);
            game_emit_link_quad((uint32)first.screen[0], (uint32)second.screen[0],
                (uint32)first.screen[1], (uint32)second.screen[1], index, count, color0, color1, first.weighted_depth, 0x800CDBD0);
            game_emit_link_quad((uint32)first.screen[0], (uint32)second.screen[0],
                (uint32)first.screen[2], (uint32)second.screen[2], index, count, color0, color1, first.weighted_depth, 0x800CDD10);
            color0 = r_u32(record + 0x6C);
            color1 = r_u32(record + 0x70);
            game_emit_link_quad((uint32)first.screen[0], (uint32)second.screen[0],
                (uint32)first.outer_plus, (uint32)second.outer_plus, index, count, color0, color1, second.weighted_depth, 0x800CDE58);
            game_emit_link_quad((uint32)first.screen[0], (uint32)second.screen[0],
                (uint32)first.outer_minus, (uint32)second.outer_minus, index, count, color0, color1, second.weighted_depth, 0x800CDF98);
            have_output = 1;
        }
        current[0] = next[0];
        current[1] = next[1];
        current[2] = next[2];
        index = game_link_checked_add(index, 1, 0x800CE0BC);
        value = game_link_checked_subtract((uint32)count, index, 0x800CE0C0);
    } while ((sint32)value > 0);
}

static uint32 game_link_signed_quotient(uint32 numerator, uint32 denominator)
{
    if (denominator == 0)
        xport_mips_break(7);
    if (denominator == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(0x5D);
    return (uint32)((sint64)(sint32)numerator / (sint32)denominator);
}

static void game_link_transform_target(uint32 record, uint32 offset, VECTOR *output)
{
    uint32 target, player;
    target = r_u32(record + 8);
    player = r_u32(0x80040FDC);
    if (target == player)
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(target + 0x11C, 18),
            (VECTOR *)psx_addr(record + offset, 12), output);
    else
        ApplyMatrixLV((MATRIX *)psx_addr(target + 0x11C, 20), (VECTOR *)psx_addr(record + offset, 12), output);
    target = r_u32(record + 8);
    output->vx = (sint32)((uint32)output->vx + r_u32(target + 8));
    target = r_u32(record + 8);
    output->vy = (sint32)((uint32)output->vy + r_u32(target + 12));
    target = r_u32(record + 8);
    output->vz = (sint32)((uint32)output->vz + r_u32(target + 16));
}

void game_update_object_link(uint32 object)
{
    VECTOR first, target_point, alternate, difference, step, camera_relative, camera_direction, step_direction, cross, midpoint;
    uint32 record, source, target, callback, frame, value, state, offset, phase, length, full_count, short_count, divisor;
    uint32 counter, factor, product, camera, draw_count, seed, size, shift, x, y, z, dx, dy, dz;
    FUNCTION_MARKER(0x80083F5C, "GAME.OVL");
    record = r_u32(object + 0xA8);
    source = r_u32(record);
    if (r_u32(source) == 0)
        goto link_remove;
    target = r_u32(record + 8);
    if (r_u32(target) == 0)
        goto link_remove;
    source = r_u32(record);
    frame = r_u32(0x800311B8);
    phase = (frame + (frame << 2)) << 6;
    w_u32(0x800F4EF4, phase);
    ApplyMatrixLV((MATRIX *)psx_addr(source + 0x11C, 20), (VECTOR *)psx_addr(record + 0x10, 12), &first);
    source = r_u32(record);
    first.vx = (sint32)((uint32)first.vx + r_u32(source + 8));
    source = r_u32(record);
    first.vy = (sint32)((uint32)first.vy + r_u32(source + 12));
    source = r_u32(record);
    first.vz = (sint32)((uint32)first.vz + r_u32(source + 16));
    offset = r_s16(record + 0x58) == 3 ? 0x30 : 0x20;
    game_link_transform_target(record, offset, &target_point);
    if (r_s16(record + 0x58) == 2) {
        game_link_transform_target(record, 0x30, &alternate);
        value = (uint32)((sint32)((uint32)alternate.vx - (uint32)target_point.vx) >> 5);
        factor = (uint32)(sint32)r_s16(record + 0x5C);
        target_point.vx = (sint32)((uint32)target_point.vx + value * factor);
        value = (uint32)((sint32)((uint32)alternate.vy - (uint32)target_point.vy) >> 5);
        factor = (uint32)(sint32)r_s16(record + 0x5C);
        target_point.vy = (sint32)((uint32)target_point.vy + value * factor);
        value = (uint32)((sint32)((uint32)alternate.vz - (uint32)target_point.vz) >> 5);
        factor = (uint32)(sint32)r_s16(record + 0x5C);
        target_point.vz = (sint32)((uint32)target_point.vz + value * factor);
        counter = r_u16(record + 0x5C) + 1;
        w_u16(record + 0x5C, counter);
        if ((sint16)counter >= 32) {
            w_u16(record + 0x58, 3);
            w_u16(record + 0x5A, 1);
        }
    }
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    state = (uint32)(sint32)r_s16(record + 0x58);
    if (state != 3) {
        game_emit_radial_effect((uint32)first.vx, (uint32)first.vy, (uint32)first.vz, 2048, 0x808080);
        game_emit_radial_effect((uint32)first.vx, (uint32)first.vy, (uint32)first.vz, 4096, 0xFF0000);
        state = (uint32)(sint32)r_s16(record + 0x58);
    }
    if (state != 1) {
        game_emit_radial_effect((uint32)target_point.vx, (uint32)target_point.vy, (uint32)target_point.vz, 1024, 0x808080);
        game_emit_radial_effect((uint32)target_point.vx, (uint32)target_point.vy, (uint32)target_point.vz, 2048, 255);
    }
    difference.vx = (sint32)((uint32)target_point.vx - (uint32)first.vx);
    difference.vy = (sint32)((uint32)target_point.vy - (uint32)first.vy);
    difference.vz = (sint32)((uint32)target_point.vz - (uint32)first.vz);
    length = (uint32)vector_length_approx(difference.vx, difference.vy, difference.vz);
    divisor = (uint32)(sint32)r_s16(record + 0x62);
    full_count = game_link_signed_quotient(length, divisor) + 1;
    short_count = (uint32)(sint32)(sint16)full_count;
    step.vx = (sint32)game_link_signed_quotient((uint32)difference.vx, short_count);
    step.vy = (sint32)game_link_signed_quotient((uint32)difference.vy, short_count);
    step.vz = (sint32)game_link_signed_quotient((uint32)difference.vz, short_count);
    w_u32(record + 0x54, length);
    camera = r_u32(0x80040C94);
    w_u32(0x800F4F10, length);
    camera_relative.vx = (sint32)((uint32)first.vx - r_u32(camera + 8));
    camera_relative.vy = (sint32)((uint32)first.vy - r_u32(camera + 12));
    camera_relative.vz = (sint32)((uint32)first.vz - r_u32(camera + 16));
    length = (uint32)vector_length_approx(camera_relative.vx, camera_relative.vy, camera_relative.vz);
    camera_direction.vx = (sint32)game_link_signed_quotient((uint32)camera_relative.vx << 12, length);
    camera_direction.vy = (sint32)game_link_signed_quotient((uint32)camera_relative.vy << 12, length);
    camera_direction.vz = (sint32)game_link_signed_quotient((uint32)camera_relative.vz << 12, length);
    w_u32(0x800F4F10, length);
    length = (uint32)vector_length_approx(step.vx, step.vy, step.vz);
    step_direction.vx = (sint32)game_link_signed_quotient((uint32)step.vx << 12, length);
    step_direction.vy = (sint32)game_link_signed_quotient((uint32)step.vy << 12, length);
    step_direction.vz = (sint32)game_link_signed_quotient((uint32)step.vz << 12, length);
    w_u32(0x800F4F10, length);
    OuterProduct12(&camera_direction, &step_direction, &cross);
    if (r_s16(record + 0x58) == 2) {
        value = (uint32)step.vx;
        product = value * r_u32(record + 0x50);
        value = (uint32)first.vx - (uint32)((sint32)value >> 1);
        w_u32(record + 0x40, value + product);
        value = (uint32)step.vy;
        product = value * r_u32(record + 0x50);
        value = (uint32)first.vy - (uint32)((sint32)value >> 1);
        w_u32(record + 0x44, value + product);
        value = (uint32)step.vz;
        product = value * r_u32(record + 0x50);
        value = (uint32)first.vz - (uint32)((sint32)value >> 1);
        w_u32(record + 0x48, value + product);
        game_draw_radial_glow_native((const VECTOR *)psx_addr(record + 0x40, 12), 60000, 0x30FFFF, 1);
        game_draw_radial_glow_native((const VECTOR *)psx_addr(record + 0x40, 12), 200000, 0x6060FF, 1);
        if (r_s16(record + 0x5E) != 0) {
            game_apply_object_link_contact_native(object, record, &step);
        } else {
            value = r_u32(record + 0x50) + 2;
            w_u32(record + 0x50, value);
            if ((sint32)value >= (sint32)short_count) {
                w_u32(record + 0x50, short_count);
                w_u16(record + 0x5E, 1);
            }
        }
    }
    camera = r_u32(0x80040C94);
    first.vx = (sint32)((uint32)first.vx - r_u32(camera + 8));
    first.vy = (sint32)((uint32)first.vy - r_u32(camera + 12));
    value = r_u32(camera + 16);
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    first.vz = (sint32)((uint32)first.vz - value);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    state = (uint32)(sint32)r_s16(record + 0x58);
    draw_count = full_count;
    if (state == 1) {
        counter = r_u16(record + 0x5A) + 4;
        w_u16(record + 0x5A, counter);
        if ((sint16)counter < (sint16)full_count) {
            draw_count = counter;
        } else {
            seed = r_u32(0x800EF50C);
            seed = (uint32)((sint32)seed >> 1) ^ (seed << 3);
            size = (seed & 127) + 350;
            shift = r_u32(object + 0x84);
            w_u32(0x800EF50C, seed);
            game_emit_sized_fragment_flash_native(&target_point, size << (shift & 31));
            sound_play_sample(4);
            w_u16(record + 0x5A, full_count);
            w_u16(record + 0x58, 2);
            w_u16(record + 0x5C, 0);
        }
    } else if (state == 3) {
        counter = r_u16(record + 0x5A) + 4;
        w_u16(record + 0x5A, counter);
        factor = (uint32)(sint32)(sint16)counter;
        if ((sint32)factor >= (sint32)(sint16)full_count) {
            callback = r_u32(object + 0x48);
            psx_dispatch_guest_call(callback, 1, object, 0, 0);
            sound_set_voice_volume(r_u32(0x800FD4F8), 0);
            return;
        }
        first.vx = (sint32)((uint32)first.vx + (uint32)step.vx * factor);
        factor = (uint32)(sint32)r_s16(record + 0x5A);
        first.vy = (sint32)((uint32)first.vy + (uint32)step.vy * factor);
        factor = (uint32)(sint32)r_s16(record + 0x5A);
        first.vz = (sint32)((uint32)first.vz + (uint32)step.vz * factor);
        draw_count -= r_u16(record + 0x5A);
    }
    target = r_u32(record + 8);
    source = r_u32(record);
    x = r_u32(target + 8);
    value = r_u32(source + 8);
    camera = r_u32(0x80040C94);
    x = (uint32)((sint32)(x + value) >> 1);
    midpoint.vx = (sint32)(x + r_u32(camera + 8));
    target = r_u32(record + 8);
    source = r_u32(record);
    y = r_u32(target + 12);
    value = r_u32(source + 12);
    y = (uint32)((sint32)(y + value) >> 1);
    midpoint.vy = (sint32)(y + r_u32(camera + 12));
    target = r_u32(record + 8);
    source = r_u32(record);
    z = r_u32(target + 16);
    value = r_u32(source + 16);
    z = (uint32)((sint32)(z + value) >> 1);
    midpoint.vz = (sint32)(z + r_u32(camera + 16));
    game_draw_object_link_segments_native(record, &first, &step, (sint32)(sint16)draw_count, &cross);
    if (r_s16(record + 0x58) == 2 && (r_u32(0x800311B8) & 1) != 0) {
        source = r_u32(record);
        dx = (uint32)((sint32)(r_u32(source + 8) - (uint32)target_point.vx) >> 2);
        source = r_u32(record);
        dy = (uint32)((sint32)(r_u32(source + 12) - (uint32)target_point.vy) >> 2);
        source = r_u32(record);
        dz = (uint32)((sint32)(r_u32(source + 16) - (uint32)target_point.vz) >> 2);
        game_emit_narrow_particles(2, (uint32)target_point.vx, (uint32)target_point.vy, (uint32)target_point.vz, dx, dy, dz);
    }
    game_update_link_audio_native(&midpoint);
    return;
link_remove:
    callback = r_u32(object + 0x48);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
}

void game_update_automatic_object_links(uint32 object)
{
    VECTOR origin, first_offset, second_offset;
    uint32 target, player, owner, flags, faction, index, serial, selected_serial, kind;
    uint32 x, y, z, sx, sy, sz, source_model, target_model, models, first_extent, second_extent, first_scale, second_scale;
    uint32 threshold, distance, scale, wait, frame, value;
    sint32 delay;
    FUNCTION_MARKER(0x800A6C18, "GAME.OVL");
    target = r_u32(object + 0x178);
    player = r_u32(0x80040FDC);
    if (target == player) {
        faction = (uint32)(sint32)r_s16(0x80059612);
        if ((faction == 3 && r_u32(object) == 3) || (faction == 1 && r_u32(object) == 1)) {
            if ((r_u32(object + 0x114) & 0x10000000) == 0)
                return;
        }
    }
    owner = r_u32(object + 0x110);
    for (index = 0; index < 2; ++index) {
        if (index != 0)
            target = r_u32(object + 0x188);
        if (target == 0)
            continue;
        flags = r_u32(target + 0x114);
        if ((flags & 16) == 0 || r_u32(target) == 0)
            continue;
        if (r_u32(owner + 0x474) == 0 || r_u32(owner + 0x480) == 0)
            continue;
        serial = r_u16(target + 0x24A);
        selected_serial = r_u16(object + (index == 0 ? 0x17C : 0x18C));
        if (serial != selected_serial || (flags & 0x8000) != 0)
            continue;
        x = r_u32(target + 8);
        sx = r_u32(object + 8);
        y = r_u32(target + 12);
        sy = r_u32(object + 12);
        sz = r_u32(object + 16);
        z = r_u32(target + 16);
        distance = (uint32)vector_length_approx((sint32)(x - sx), (sint32)(y - sy), (sint32)(z - sz));
        source_model = r_u32(object + 0x74);
        models = r_u32(0x80046178);
        target_model = r_u32(target + 0x74);
        first_extent = r_u32(models + source_model * 132 + 0x6C);
        first_scale = r_u32(object + 0x84);
        second_extent = r_u32(models + target_model * 132 + 0x6C);
        second_scale = r_u32(target + 0x84);
        threshold = (second_extent << (second_scale & 31)) + (index == 0 ? 140000 : 180000);
        threshold += first_extent << (first_scale & 31);
        if ((sint32)distance >= (sint32)threshold)
            continue;
        game_get_target_vertex_vector_native(object, target, &origin);
        scale = r_u32(object + 0x84);
        origin.vx = (sint32)((uint32)origin.vx << (scale & 31));
        scale = r_u32(object + 0x84);
        origin.vy = (sint32)((uint32)origin.vy << (scale & 31));
        scale = r_u32(object + 0x84);
        origin.vz = (sint32)((uint32)origin.vz << (scale & 31));
        game_choose_object_link_offsets_native(target, object, &first_offset, &second_offset);
        scale = r_u32(target + 0x84);
        if ((sint32)scale > 0) {
            first_offset.vx = (sint32)((uint32)first_offset.vx << (scale & 31));
            scale = r_u32(target + 0x84);
            first_offset.vy = (sint32)((uint32)first_offset.vy << (scale & 31));
            scale = r_u32(target + 0x84);
            first_offset.vz = (sint32)((uint32)first_offset.vz << (scale & 31));
            scale = r_u32(target + 0x84);
            second_offset.vx = (sint32)((uint32)second_offset.vx << (scale & 31));
            scale = r_u32(target + 0x84);
            second_offset.vy = (sint32)((uint32)second_offset.vy << (scale & 31));
            scale = r_u32(target + 0x84);
            second_offset.vz = (sint32)((uint32)second_offset.vz << (scale & 31));
        }
        game_create_object_link_native(object, target, &origin, &first_offset, &second_offset);
        delay = random_between(150, 300);
        frame = r_u32(0x800311B8);
        w_u32(owner + 0x47C, frame + (uint32)delay);
    }
    if (r_u8(owner + 0x46F) == 0)
        return;
    faction = (uint32)(sint32)r_s16(0x80059612);
    kind = r_u32(object);
    if (faction == kind && (r_u32(object + 0x114) & 0x10000000) == 0)
        return;
    player = r_u32(0x80040FDC);
    if (r_s32(player + 0xD8) < 46 || r_s32(player + 0xDC) > 0 || r_u32(0x800EDD20) != 0)
        return;
    wait = r_u32(object + 0xF8) + 120;
    frame = r_u32(0x800311B8);
    if (wait >= frame)
        return;
    x = r_u32(player + 8);
    sx = r_u32(object + 8);
    y = r_u32(player + 12);
    sy = r_u32(object + 12);
    z = r_u32(player + 16);
    sz = r_u32(object + 16);
    distance = (uint32)vector_length_approx((sint32)(x - sx), (sint32)(y - sy), (sint32)(z - sz));
    source_model = r_u32(object + 0x74);
    models = r_u32(0x80046178);
    first_extent = r_u32(models + source_model * 132 + 0x6C);
    scale = r_u32(object + 0x84);
    threshold = (first_extent << (scale & 31)) + 140000;
    if ((sint32)distance < (sint32)threshold) {
        player = r_u32(0x80040FDC);
        if (r_s32(player + 0xDC) > 0)
            return;
        game_get_target_vertex_vector_native(object, player, &origin);
        player = r_u32(0x80040FDC);
        first_offset.vy = 1000;
        second_offset.vy = 1000;
        first_offset.vz = 2000;
        second_offset.vz = 2000;
        second_offset.vx = 0;
        first_offset.vx = 0;
        game_create_object_link_native(object, player, &origin, &first_offset, &second_offset);
        player = r_u32(0x80040FDC);
        w_u32(player + 0xDC, 90);
    } else if ((sint32)distance > 500000) {
        player = r_u32(0x80040FDC);
        value = 30;
        w_u32(player + 0xDC, value);
    }
}

void game_update_object_automatic_weapons(uint32 object)
{
    uint32 metadata, base, entry, index = 0, count, flags, target, projectile;
    FUNCTION_MARKER(0x800EC484, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    base = metadata + 0x2AC;
    if (r_s32(object + 0x244) > 0)
        game_update_automatic_object_links(object);
    if (r_u8(metadata + 0x46F) != 1)
        return;
    if ((r_u32(object + 0x114) & 0x00400000) != 0)
        return;
    if (r_u8(metadata + 0x470) == 0)
        return;
    entry = base;
    do {
        if (r_u8(entry) != 0 && r_u8(entry + 0x1D) != 0) {
            flags = r_u32(object + 0x114);
            target = r_u32(entry + 12);
            projectile = game_create_aimed_projectile(object, target, flags & 0x10000);
            if (projectile != 0)
                w_u32(entry + 0x14, r_u32(0x800311B8));
        }
        count = r_u8(base + 0x1C4);
        ++index;
        entry += 32;
    } while ((sint32)index < (sint32)count);
}

void game_update_temporary_sensor_damage(uint32 object)
{
    uint32 flags, timer, controller, serial;
    FUNCTION_MARKER(0x8009F84C, "GAME.OVL");
    game_update_object_state(object);
    game_update_object_movement(object);
    flags = r_u32(object + 0x114);
    if ((flags & 0x40010) == 0x40010)
        return;
    timer = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, timer);
    if ((sint32)timer > 0)
        return;
    w_u32(object + 0x244, 1);
    w_u32(object + 0x58, 0x800D9D90);
    flags = r_u32(object + 0x114);
    w_u8(object + 0x276, 0);
    w_u32(object + 0x40, 0x8009AB58);
    if ((flags & 0x00400000) != 0)
        w_u32(object + 0x114, flags ^ 0x00400000);
    flags = r_u32(object + 0x114);
    if ((flags & 0x00080000) != 0)
        w_u32(object + 0x114, flags ^ 0x00080000);
    controller = game_create_object(0, 0, 0, 0, 0);
    w_u32(controller + 0x40, 0x8009F944);
    w_u32(controller + 0x170, object);
    serial = r_u16(object + 0x24A);
    w_u32(controller + 0xA8, 50);
    w_u32(controller + 0xAC, 0);
    w_u32(controller, 0x74CBB1);
    w_u16(controller + 0x174, serial);
}

void game_update_sensor_recovery(uint32 controller)
{
    uint32 object, sum, increment, kind, health, maximum;
    FUNCTION_MARKER(0x8009F944, "GAME.OVL");
    object = r_u32(controller + 0x170);
    if (r_u32(object) == 0 || r_s32(object + 0x244) <= 0)
        goto recovery_remove;
    sum = r_u32(controller + 0xAC);
    increment = r_u32(controller + 0xA8);
    sum += increment;
    w_u32(controller + 0xAC, sum);
    if ((sum & 0xFF00) != 0) {
        w_u32(controller + 0xAC, sum & 255);
        health = r_u32(object + 0x244);
        w_u32(object + 0x244, health + 1);
    }
    kind = r_u32(object + 4);
    health = r_u32(object + 0x244);
    maximum = r_u32(0x800536AC + kind * 60);
    if ((sint32)health < (sint32)maximum)
        return;
recovery_remove:
    game_mark_object_for_removal(controller);
}

void game_update_permanent_sensor_damage(uint32 object)
{
    FUNCTION_MARKER(0x8009F9F8, "GAME.OVL");
    w_u32(object + 0xE8, 0);
    game_update_object_state(object);
    game_update_object_movement(object);
}

void game_fire_target_decoy(uint32 object)
{
    uint32 metadata, target, offset, projectile, value, threat, serial, expected, selected;
    FUNCTION_MARKER(0x800804B8, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    target = r_u32(object + 0x178);
    offset = r_u32(metadata + 0x284);
    projectile = game_create_weapon_projectile(object, target, offset, 0);
    w_u32(projectile + 0x1E4, 0x800EEC0C);
    w_u32(projectile, 0xD4);
    w_u32(projectile + 0x3C, 0x800B37B0);
    w_u32(projectile + 0x38, 0x800B4184);
    w_u32(projectile + 0x40, 0x800812E0);
    w_u32(projectile + 0x9C, 120);
    value = 0 - (uint32)(sint32)r_s16(object + 0x120);
    w_u32(projectile + 0xFC, (uint32)((sint32)value >> 5));
    value = 0 - (uint32)(sint32)r_s16(object + 0x126);
    w_u32(projectile + 0x100, (uint32)((sint32)value >> 5));
    value = (uint32)(sint32)r_s16(object + 0x12C);
    w_u32(projectile + 0x14, 0);
    w_u32(projectile + 0x18, 0);
    w_u32(projectile + 0x1C, 0);
    value = 0 - value;
    w_u32(projectile + 0x104, (uint32)((sint32)value >> 5));
    threat = r_u32(object + 0x1A0);
    if (threat == 0)
        return;
    serial = r_u16(threat + 0x24A);
    expected = r_u16(object + 0x1A4);
    if (serial != expected)
        return;
    selected = r_u32(threat + 0x178);
    if (selected != object)
        return;
    w_u32(threat + 0x178, projectile);
    threat = r_u32(selected + 0x1A0);
    serial = r_u16(projectile + 0x24A);
    w_u16(threat + 0x17C, serial);
}

void game_try_launch_target_decoy(uint32 object)
{
    uint32 metadata, choice;
    FUNCTION_MARKER(0x800EC414, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    choice = (uint32)random_between(1, 2);
    if (choice == 1 || r_u32(metadata + 0x2A8) == 0)
        return;
    game_fire_target_decoy(object);
    w_u32(metadata + 0x2A4, r_u32(0x800311B8));
}

uint32 game_set_relative_target_angles(uint32 object, uint32 x, uint32 y, uint32 z)
{
    VECTOR relative, local;
    uint32 length, player, yaw, pitch, vertical, depth;
    FUNCTION_MARKER(0x80083988, "GAME.OVL");
    relative.vx = (sint32)x;
    relative.vy = (sint32)y;
    relative.vz = (sint32)z;
    length = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    player = r_u32(0x80040FDC);
    w_u32(0x80046174, length);
    if (object == player)
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11C, 20), &relative, &local);
    else
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11C, 18), &relative, &local);
    yaw = (uint32)ratan2(local.vx, local.vz);
    vertical = (uint32)local.vy;
    depth = (uint32)local.vz;
    w_u32(0x80033D1C, yaw);
    w_u32(0x80033D18, yaw);
    if ((sint32)depth < 0)
        depth = 0 - depth;
    pitch = (uint32)ratan2((sint32)(0 - vertical), (sint32)depth);
    yaw = r_u32(0x80033D18);
    w_u32(0x80033D30, pitch);
    w_u32(0x80033D20, pitch);
    if ((sint32)pitch < 0)
        pitch = 0 - pitch;
    w_u32(0x80053674, pitch);
    if ((sint32)yaw < 0)
        yaw = 0 - yaw;
    w_u32(0x80052E6C, yaw);
    return pitch;
}

void game_update_timed_maneuver(uint32 object)
{
    uint32 callback, timer;
    FUNCTION_MARKER(0x800C9774, "GAME.OVL");
    callback = r_u32(object + 0x4C);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
    game_update_object_state(object);
    game_update_object_movement(object);
    timer = r_u32(object + 0x9C) - 1;
    w_u32(object + 0x9C, timer);
    if ((sint32)timer <= 0)
        w_u32(object + 0x40, r_u32(object + 0x50));
}

void game_begin_yaw_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 yaw, uint32 pitch, uint32 update)
{
    FUNCTION_MARKER(0x800C97D8, "GAME.OVL");
    w_u32(object + 0x9C, ticks);
    w_u32(object + 0x50, completion);
    w_u16(object + 0x250, yaw);
    w_u16(object + 0x254, 0);
    w_u32(object + 0x40, 0x800C9774);
    w_u16(object + 0x252, pitch);
    w_u32(object + 0x4C, update);
    game_update_object_state(object);
}

void game_begin_roll_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 roll, uint32 pitch, uint32 update)
{
    FUNCTION_MARKER(0x800C9820, "GAME.OVL");
    w_u32(object + 0x9C, ticks);
    w_u32(object + 0x50, completion);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x254, roll);
    w_u32(object + 0x40, 0x800C9774);
    w_u16(object + 0x252, pitch);
    w_u32(object + 0x4C, update);
    game_update_object_state(object);
}

void game_update_target_maneuver(uint32 object)
{
    uint32 target, threat, heading, kind, speed, value, yaw, pitch, x, y, z, velocity, dx, dy, dz;
    sint32 choice, random_value;
    FUNCTION_MARKER(0x800D9D90, "GAME.OVL");
    target = r_u32(object + 0x190);
    if ((r_u32(target + 0x114) & 16) != 0 && r_u32(object + 0x178) == target) {
        w_u32(object + 0x198, 0);
        return;
    }
    threat = r_u32(object + 0x1A0);
    if (threat != 0 && r_u32(threat + 0x178) == object)
        game_try_launch_target_decoy(object);
    game_evaluate_target_occlusion(object, target, 1);
    heading = r_u32(0x80052E6C);
    if ((sint32)heading >= 1025 && r_s32(object + 0xEC) >= 25601)
        return;
    kind = r_u32(object + 4);
    speed = (uint32)((sint32)r_u32(0x8005369C + kind * 60) >> 2);
    w_u32(object + 0xE8, (speed << 1) + speed);
    if ((r_u32(target + 0x114) & 16) != 0) {
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        w_u32(0x80033D1C, 0 - yaw);
        w_u32(0x80033D30, 0 - pitch);
        choice = random_between(0, 1);
        if (choice == 0) {
            random_value = random_signed_between(1680, 2000);
            yaw = r_u32(0x80033D1C);
            w_u32(0x80033D1C, (yaw + (uint32)random_value) & 0xFFF);
            random_value = random_signed_between(40, 80);
        } else {
            random_value = random_signed_between(40, 80);
            yaw = r_u32(0x80033D1C);
            w_u32(0x80033D1C, (yaw + (uint32)random_value) & 0xFFF);
            random_value = random_signed_between(1520, 2000);
        }
        pitch = r_u32(0x80033D30);
        w_u32(0x80033D30, (pitch + (uint32)random_value) & 0xFFF);
        goto target_maneuver_begin;
    }
    if ((sint32)heading < 256 && r_s32(0x80053674) < 256 &&
        r_s32(0x80033D24) >= 1521 && r_s32(0x80046174) >= 3001) {
        value = r_u32(target + 0xE8);
        w_u32(object + 0xE8, (uint32)((sint32)value >> 1));
        goto target_maneuver_begin;
    }
    if (r_s32(0x80033D34) >= 1521 && r_s32(0x80033D24) >= 1521) {
        speed = r_u32(target + 0xE8);
        w_u32(object + 0xE8, (sint32)speed >= 25601 ? speed - 2560 : speed);
        return;
    }
    if (r_s32(target + 0xE8) > 0) {
        velocity = r_u32(target + 0xFC);
        x = r_u32(target + 8);
        y = r_u32(target + 12);
        z = r_u32(target + 16);
        dx = x - velocity * 3000;
        value = r_u32(object + 8);
        velocity = r_u32(target + 0x100);
        dx -= value;
        dy = y - velocity * 3000;
        value = r_u32(object + 12);
        velocity = r_u32(target + 0x104);
        dy -= value;
        value = r_u32(object + 16);
        dz = z - velocity * 3000 - value;
        game_set_relative_target_angles(object, dx, dy, dz);
    } else {
        random_value = random_signed_between(800, 1200);
        pitch = r_u32(0x80033D30);
        w_u32(0x80033D30, (pitch + (uint32)random_value) & 0xFFF);
    }
target_maneuver_begin:
    choice = random_between(0, 100);
    yaw = r_u32(0x80033D1C);
    pitch = r_u32(0x80033D30);
    if (choice < 50)
        game_begin_yaw_maneuver(object, 60, 0x8009AB58, yaw, pitch, 0x800DA150);
    else
        game_begin_roll_maneuver(object, 60, 0x8009AB58, yaw, pitch, 0x800DA150);
}

uint32 game_update_maneuver_bias(uint32 object)
{
    uint32 heading;
    FUNCTION_MARKER(0x800DA414, "GAME.OVL");
    heading = r_u32(0x80052E6C);
    return game_update_maneuver_bias_from_heading(object, heading);
}

uint32 game_update_maneuver_bias_from_heading(uint32 object, uint32 heading)
{
    uint32 state, distance, angle, sign, previous, value;
    sint32 bias = 0, limit, current;
    FUNCTION_MARKER(0x800DA41C, "GAME.OVL");
    if ((sint32)heading < 80 && r_s32(0x80053674) < 80) {
        bias = 2;
    } else {
        distance = r_u32(0x80046174);
        angle = (uint32)ratan2(11000, (sint32)distance);
        if ((sint32)angle < 0)
            angle = 0 - angle;
        limit = (sint16)angle;
        if (r_s32(0x80052E6C) < limit && r_s32(0x80053674) < limit)
            ++bias;
    }
    if (r_s32(0x80033D24) < 80 && r_s32(0x80033D34) < 80) {
        bias -= 2;
    } else {
        distance = r_u32(0x80046174);
        angle = (uint32)ratan2(11000, (sint32)distance);
        if ((sint32)angle < 0)
            angle = 0 - angle;
        limit = (sint16)angle;
        if (r_s32(0x80033D24) < limit && r_s32(0x80033D34) < limit)
            --bias;
    }
    if ((sint16)bias == 0) {
        state = r_u32(object + 0x22C);
        current = r_s16(state + 12);
        if (current > 0)
            w_u16(state + 12, (uint32)current - 1);
        else if (current < 0)
            w_u16(state + 12, (uint32)current + 1);
        state = r_u32(object + 0x22C);
        value = r_u16(state + 14);
        w_u16(state + 14, value - 1);
    } else {
        sign = (uint32)(uint16)bias & 0x8000;
        state = r_u32(object + 0x22C);
        previous = (uint32)(sint32)r_s16(state + 16) & 0x8000;
        if (previous != sign)
            w_u16(state + 14, 0);
        state = r_u32(object + 0x22C);
        value = r_u16(state + 12) + (uint32)bias;
        w_u16(state + 12, value);
        state = r_u32(object + 0x22C);
        value = r_u16(state + 14);
        w_u16(state + 14, value + 1);
        state = r_u32(object + 0x22C);
        current = r_s16(state + 12);
        if (current >= 451)
            w_u16(state + 12, 450);
        else if (current < -450)
            w_u16(state + 12, (uint32)-450);
    }
    state = r_u32(object + 0x22C);
    value = r_u16(state + 12);
    w_u16(state + 16, value);
    return value;
}

void game_update_target_maneuver_angles(uint32 object)
{
    uint32 target, heading, pitch, yaw;
    FUNCTION_MARKER(0x800DA150, "GAME.OVL");
    target = r_u32(object + 0x190);
    if (target == 0) {
        w_u32(object + 0x9C, 1);
        return;
    }
    game_update_maneuver_bias(object);
    game_evaluate_target_occlusion(object, target, 0);
    heading = r_u32(0x80033D1C);
    if ((sint32)heading < 0)
        heading = 0 - heading;
    if ((sint32)heading >= 1281) {
        w_u16(object + 0x250, 0);
        w_u16(object + 0x252, 0);
        w_u16(object + 0x254, 0);
    }
    if ((r_u32(target + 0x114) & 16) != 0) {
        pitch = r_u32(0x80033D30);
        yaw = r_u32(0x80033D1C);
        w_u16(object + 0x252, 0 - pitch);
        w_u16(object + 0x250, 0 - yaw);
    }
}

void game_update_cached_target_tracking(uint32 object)
{
    uint32 target, state, value, timer, yaw, pitch, distance, cache, count, x, y, z, sx, sy, sz, first_cache, second_cache, range, threat;
    sint32 refresh;
    FUNCTION_MARKER(0x800DA640, "GAME.OVL");
    target = r_u32(object + 0x178);
    if (r_u32(target) == 0)
        w_u32(object + 0x40, 0x8009AB58);
    state = r_u32(object + 0x22C);
    value = r_u16(state + 10) - 1;
    w_u16(state + 10, value);
    state = r_u32(object + 0x22C);
    timer = (uint32)(sint32)r_s16(state + 10);
    refresh = 0;
    if ((sint32)timer <= 0) {
        target = r_u32(object + 0x178);
        refresh = r_u32(target) != 0;
    }
    if (refresh != 0) {
        game_evaluate_target_occlusion(object, target, 1);
        yaw = r_u32(0x80033D1C);
        pitch = r_u32(0x80033D30);
        distance = r_u32(0x80046174);
        state = r_u32(object + 0x22C);
        cache = r_u32(0x80040C98);
        w_u16(object + 0x25C, yaw);
        w_u16(object + 0x25E, pitch);
        w_u32(object + 0x230, distance);
        w_u16(state + 4, cache);
        state = r_u32(object + 0x22C);
        cache = r_u32(0x80040F1C);
        w_u16(state + 6, cache);
        count = (sint32)distance > 79999 ? 3 : (sint32)distance > 49999 ? 2 : 1;
        state = r_u32(object + 0x22C);
        w_u16(state + 10, count);
        game_update_maneuver_bias(object);
    } else {
        target = r_u32(object + 0x178);
        pitch = (uint32)(sint32)r_s16(object + 0x25E);
        yaw = (uint32)(sint32)r_s16(object + 0x25C);
        sx = r_u32(object + 8);
        x = r_u32(target + 8);
        w_u32(0x80033D20, pitch);
        w_u32(0x80033D30, pitch);
        y = r_u32(target + 12);
        w_u32(0x80033D18, yaw);
        w_u32(0x80033D1C, yaw);
        state = r_u32(object + 0x22C);
        sy = r_u32(object + 12);
        first_cache = (uint32)(sint32)r_s16(state + 4);
        state = r_u32(object + 0x22C);
        z = r_u32(target + 16);
        sz = r_u32(object + 16);
        second_cache = (uint32)(sint32)r_s16(state + 6);
        w_u32(0x80040C98, first_cache);
        w_u32(0x80040F1C, second_cache);
        distance = (uint32)vector_length_approx((sint32)(x - sx), (sint32)(y - sy), (sint32)(z - sz));
        w_u32(0x80046174, distance);
        w_u32(object + 0x230, distance);
    }
    target = r_u32(object + 0x178);
    game_evaluate_target_occlusion(object, target, 1);
    target = r_u32(object + 0x178);
    range = (r_u32(target + 0x114) & 16) != 0 ? 180000 : 40000;
    if (r_s32(0x80046174) < (sint32)range)
        game_update_target_weapons(object);
    threat = r_u32(object + 0x1A0);
    if (threat != 0 && r_u32(threat + 0x178) == object)
        game_try_launch_target_decoy(object);
}

uint32 game_create_plasma_torpedo(uint32 owner, uint32 target, uint32 offset)
{
    uint32 type, kind, projectile, serial, x, y, z;
    FUNCTION_MARKER(0x80080214, "GAME.OVL");
    type = r_u32(owner);
    kind = type == 3 ? 4 : type == 1 ? 2 : 5;
    projectile = game_create_object(0, 0, 0, 0, kind);
    w_u32(projectile + 0x1E4, 0x800EEBEC);
    if (offset != 0) {
        game_position_at_object_offset(owner, offset, projectile);
    } else {
        w_u32(projectile + 8, r_u32(owner + 8));
        w_u32(projectile + 12, r_u32(owner + 12));
        w_u32(projectile + 16, r_u32(owner + 16));
    }
    object_extract_angles(projectile, owner);
    w_u32(projectile + 0x3C, 0x800B37B0);
    w_u32(projectile + 0x38, 0x800B37D0);
    w_u32(projectile, 0xCF);
    w_u32(projectile + 0x160, owner);
    serial = r_u16(owner + 0x24A);
    w_u32(projectile + 0x178, target);
    w_u16(projectile + 0x164, serial);
    serial = r_u16(target + 0x24A);
    w_u32(projectile + 0xEC, 200);
    w_u32(projectile + 0x9C, 5);
    w_u32(projectile + 0x40, 0x8008068C);
    w_u32(projectile + 0x5C, 0x8008132C);
    w_u32(projectile + 0x48, 0x8008132C);
    w_u16(projectile + 0x17C, serial);
    game_get_object_forward(owner, projectile + 0xFC);
    x = r_u32(projectile + 0xFC);
    z = r_u32(projectile + 0x104);
    w_u32(projectile + 0xFC, (uint32)((sint32)x >> 3));
    y = r_u32(projectile + 0x100);
    w_u32(projectile + 0x104, (uint32)((sint32)z >> 3));
    z = r_u32(projectile + 0x104);
    w_u32(projectile + 0x100, (uint32)((sint32)y >> 3));
    x = r_u32(projectile + 0xFC);
    y = r_u32(projectile + 0x100);
    w_u32(projectile + 0x1C, z);
    w_u32(projectile + 0x14, x);
    w_u32(projectile + 0x18, y);
    if (target != 0 && r_u32(target) != 0) {
        w_u32(target + 0x1A0, projectile);
        serial = r_u16(projectile + 0x24A);
        w_u16(target + 0x1A4, serial);
    }
    return projectile;
}

void game_try_fire_plasma_torpedo(uint32 object)
{
    uint32 metadata, target, offset;
    FUNCTION_MARKER(0x800EC298, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    if (r_u8(metadata + 0x228) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076DC4, 0);
        return;
    }
    if (r_u32(metadata + 0x250) == 0)
        return;
    target = r_u32(object + 0x178);
    offset = r_u32(metadata + 0x22C);
    game_create_plasma_torpedo(object, target, offset);
    w_u32(metadata + 0x24C, r_u32(0x800311B8));
}

void game_try_fire_burst_torpedo(uint32 object)
{
    uint32 metadata;
    FUNCTION_MARKER(0x800EC31C, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    if (r_u8(metadata + 0x254) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076DE8, 0);
        return;
    }
    if (r_u32(metadata + 0x27C) == 0)
        return;
    game_create_profile_0c_projectile(object, 0);
    w_u32(metadata + 0x278, r_u32(0x800311B8));
}

void game_try_fire_antishield_torpedo(uint32 object)
{
    uint32 metadata;
    FUNCTION_MARKER(0x800EC398, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    if (r_u8(metadata + 0x1FC) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076E0C, 0);
        return;
    }
    if (r_u32(metadata + 0x224) == 0)
        return;
    game_fire_antishield_projectile(object);
    w_u32(metadata + 0x220, r_u32(0x800311B8));
}

void game_update_target_weapons(uint32 object)
{
    uint32 target, metadata, descriptor, distance, value, limit, player;
    sint32 aligned = 0, allow_missile;
    FUNCTION_MARKER(0x800DE274, "GAME.OVL");
    target = r_u32(object + 0x178);
    metadata = r_u32(object + 0x110);
    game_evaluate_target_occlusion(object, target, 0);
    if (r_u32(object) == 3 && (r_u32(object + 0x114) & 16) == 0) {
        descriptor = r_u32(object + 0x278);
        if (descriptor != 0 && r_u32(descriptor) != 0) {
            distance = r_u32(0x80046174);
            limit = r_u32(descriptor + 8);
            if ((sint32)limit < (sint32)distance) {
                value = (uint32)(sint32)r_s16(descriptor + 12);
                value = (sint32)value < 0 ? 0 - value : value;
                if ((sint32)value < 440) {
                    value = (uint32)(sint32)r_s16(descriptor + 14);
                    value = (sint32)value < 0 ? 0 - value : value;
                    if ((sint32)value < 440)
                        return;
                }
            }
        }
    }
    if (r_u8(metadata + 0x30) != 0) {
        value = r_u32(0x80033D18);
        value = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)value >= 1601) {
            value = r_u32(0x80033D20);
            value = (sint32)value < 0 ? 0 - value : value;
            if ((sint32)value < 512 && r_s32(0x80046174) <= 99999)
                game_try_fire_rear_lasers(object);
        }
    }
    value = r_u32(0x80033D18);
    value = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)value < 128) {
        value = r_u32(0x80033D20);
        value = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)value < 128 && r_s32(0x80046174) <= 99999)
            aligned = 1;
    }
    if (aligned == 0 && (r_u32(target + 0x114) & 16) == 0)
        goto target_missiles;
    if (r_u8(metadata + 0xC0) != 0) {
        game_try_fire_ki_plasma_lasers(object);
    } else if (r_u8(metadata + 0xF0) != 0) {
        game_try_fire_scatter_lasers(object);
    } else if (r_s32(target + 0x244) > 0 && r_u8(metadata + 0x90) != 0 &&
        (r_u32(0x800311B8) & 0x258) < 150) {
        game_try_fire_emp_lasers(object);
    } else if (r_s16(object + 0x266) != 1) {
        game_try_fire_primary_lasers(object);
    }
    if ((r_u32(target + 0x114) & 16) == 0)
        goto target_missiles;
    player = r_u32(0x80040FDC);
    if (target != player && random_between(1, 100) != 1)
        return;
    if (r_s32(target + 0x234) > 0 && r_u8(metadata + 0x1FC) != 0 && r_u32(metadata + 0x224) != 0) {
        game_try_fire_antishield_torpedo(object);
        return;
    }
    if (r_u8(metadata + 0x228) != 0 && r_u32(metadata + 0x250) != 0) {
        game_try_fire_plasma_torpedo(object);
        return;
    }
    if (r_u8(metadata + 0x254) != 0)
        game_try_fire_burst_torpedo(object);
    return;
target_missiles:
    player = r_u32(0x80040FDC);
    allow_missile = 0;
    if (target == player && random_between(1, 2) == 1)
        allow_missile = 1;
    if (allow_missile == 0 && random_between(1, 400) != 1)
        return;
    if (r_s32(target + 0x244) > 0 && r_u8(metadata + 0x178) != 0 && r_u32(metadata + 0x1A0) != 0) {
        value = r_u32(object + 0x178);
        player = r_u32(0x80040FDC);
        if (value != player) {
            game_try_fire_stun_missile(object);
            return;
        }
    }
    if (r_s16(object + 0x266) == 1)
        return;
    if (r_s32(target + 0x234) > 0 && r_u8(metadata + 0x1D0) != 0 && r_u32(metadata + 0x1F8) != 0) {
        game_try_fire_antishield_missile(object);
        return;
    }
    if (r_u8(metadata + 0x14C) != 0 && r_u32(metadata + 0x174) != 0) {
        game_try_fire_motion_missile(object);
        return;
    }
    if (r_u8(metadata + 0x120) != 0)
        game_try_fire_tracker_missile(object);
}

uint32 game_create_antishield_missile(uint32 owner, uint32 target, uint32 offset, uint32 trail)
{
    uint32 type, kind, projectile, serial, x, y, z;
    FUNCTION_MARKER(0x8007FDC8, "GAME.OVL");
    type = r_u32(owner);
    kind = type == 3 ? 4 : type == 1 ? 2 : 5;
    projectile = game_create_object(0, 0, 0, 0, kind);
    w_u32(projectile + 0x1E4, 0x800EEBAC);
    if (offset != 0) {
        game_position_at_object_offset(owner, offset, projectile);
    } else {
        w_u32(projectile + 8, r_u32(owner + 8));
        w_u32(projectile + 12, r_u32(owner + 12));
        w_u32(projectile + 16, r_u32(owner + 16));
    }
    object_extract_angles(projectile, owner);
    w_u32(projectile + 0x3C, 0x8002493C);
    w_u32(projectile, 0xCF);
    w_u32(projectile + 0x160, owner);
    serial = r_u16(owner + 0x24A);
    w_u32(projectile + 0x178, target);
    w_u16(projectile + 0x164, serial);
    serial = r_u16(target + 0x24A);
    w_u32(projectile + 0xEC, 800);
    w_u32(projectile + 0x9C, 5);
    w_u32(projectile + 0x40, 0x8008068C);
    w_u32(projectile + 0x5C, 0x8008132C);
    w_u32(projectile + 0x48, 0x8008132C);
    w_u16(projectile + 0x17C, serial);
    game_get_object_forward(owner, projectile + 0xFC);
    x = r_u32(projectile + 0xFC);
    z = r_u32(projectile + 0x104);
    w_u32(projectile + 0xFC, (uint32)((sint32)x >> 3));
    y = r_u32(projectile + 0x100);
    w_u32(projectile + 0x104, (uint32)((sint32)z >> 3));
    z = r_u32(projectile + 0x104);
    w_u32(projectile + 0x100, (uint32)((sint32)y >> 3));
    x = r_u32(projectile + 0xFC);
    y = r_u32(projectile + 0x100);
    w_u32(projectile + 0x1C, z);
    w_u32(projectile + 0x14, x);
    w_u32(projectile + 0x18, y);
    if (target != 0 && r_u32(target) != 0) {
        w_u32(target + 0x1A0, projectile);
        serial = r_u16(projectile + 0x24A);
        w_u16(target + 0x1A4, serial);
    }
    game_attach_object_trail(projectile, trail);
    return projectile;
}

void game_synchronize_missile_cooldowns(uint32 object)
{
    uint32 metadata, offset, previous, frame;
    FUNCTION_MARKER(0x800EC724, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    if (metadata == 0)
        return;
    for (offset = 0x120; offset <= 0x1D0; offset += 44) {
        if (r_u8(metadata + offset) == 0)
            continue;
        previous = r_u32(metadata + offset + 0x24);
        frame = r_u32(0x800311B8);
        w_u32(metadata + offset + 0x20, 0);
        if (previous != frame)
            w_u32(metadata + offset + 0x28, 0);
        frame = r_u32(0x800311B8);
        w_u32(metadata + offset + 0x24, frame);
    }
}

uint32 game_create_stun_missile(uint32 object)
{
    uint32 metadata, target, offset, projectile;
    FUNCTION_MARKER(0x80080464, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    target = r_u32(object + 0x178);
    offset = r_u32(metadata + 0x17C);
    projectile = game_create_weapon_projectile(object, target, offset, 0);
    w_u32(projectile + 0x1E4, 0x800EEBFC);
    w_u32(projectile, 0xD1);
    w_u32(projectile + 0x3C, 0x800B37B0);
    w_u32(projectile + 0x38, 0x800B3EE8);
    return projectile;
}

void game_try_fire_tracker_missile(uint32 object)
{
    uint32 metadata, target, offset;
    FUNCTION_MARKER(0x800EBFCC, "GAME.OVL");
    metadata = r_u32(object + 0x110);
    if (r_u8(metadata + 0x120) == 0) {
        diagnostic_report(r_u32(object + 4), 0x80076D14, 0);
        return;
    }
    if (r_u32(metadata + 0x148) == 0)
        return;
    target = r_u32(object + 0x178);
    offset = r_u32(metadata + 0x124);
    game_create_weapon_projectile(object, target, offset, 1);
    w_u32(metadata + 0x144, r_u32(0x800311B8));
    game_synchronize_missile_cooldowns(object);
}
