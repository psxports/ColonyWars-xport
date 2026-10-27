#include "game_functions.h"

void game_draw_paired_glows(void)
{
    VECTOR transformed;
    SVECTOR vertex;
    sint32 screen, projection, flags;
    uint32 index, record, object, camera, x, y, z, value, ax, ay, az, shifts, depth, divisor, size, color;
    uint32 ordering_depth, screen_x, screen_y;
    FUNCTION_MARKER(0x800E5970, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    for (index = 0u; index < 2u; ++index) {
        record = r_u32(0x800F1894u + index * 4u);
        object = r_u32(record + 0x10u);
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
        shifts = 2u;
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u),
            (VECTOR *)psx_addr(record, 12u), &transformed);
        value = r_u32(object + 8u);
        camera = r_u32(0x80040C94u);
        x = (uint32)transformed.vx + value;
        x -= r_u32(camera + 8u);
        value = r_u32(object + 12u);
        y = r_u32(camera + 12u);
        y = (uint32)transformed.vy + value - y;
        value = r_u32(object + 16u);
        z = r_u32(camera + 16u);
        z = (uint32)transformed.vz + value - z;
        for (;;) {
            ax = (sint32)x < 0 ? 0u - x : x;
            ay = (sint32)y < 0 ? 0u - y : y;
            az = (sint32)z < 0 ? 0u - z : z;
            if ((sint32)ax < 0x3FAD && (sint32)ay < 0x3FAD && (sint32)az < 0x3FAD)
                break;
            ++shifts;
            x = (uint32)((sint32)x >> 1);
            y = (uint32)((sint32)y >> 1);
            z = (uint32)((sint32)z >> 1);
        }
        vertex.vx = (sint16)x;
        vertex.vy = (sint16)y;
        vertex.vz = (sint16)z;
        SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
        depth = (uint32)RotTransPers(&vertex, &screen, &projection, &flags);
        if (depth == 0u)
            continue;
        divisor = depth << (shifts & 31u);
        if (divisor == 0u)
            xport_mips_break(7u);
        size = 64000u / divisor;
        color = r_u32(record + 0x14u) & 0xFFFFFFu;
        if ((sint32)size < 80)
            size = 80u;
        if ((sint32)size >= 225)
            size = 224u;
        depth -= 8u;
        ordering_depth = depth << (shifts & 31u);
        if ((sint32)size >= 33) {
            screen_x = (uint32)(sint32)(sint16)screen;
            screen_y = (uint32)(sint32)(sint16)((uint32)screen >> 16);
            render_ellipse_fan(screen_x, screen_y, size >> 3, color, ordering_depth);
        }
        screen_x = (uint32)(sint32)(sint16)screen;
        screen_y = (uint32)(sint32)(sint16)((uint32)screen >> 16);
        game_draw_glow_rays(screen_x, screen_y, size << 1, ordering_depth, color);
    }
}


uint32 game_reset_player_ring_limits(void)
{
    uint32 player;
    FUNCTION_MARKER(0x800E7F44, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    w_u32(player + 0x238u, 10000u);
    w_u32(player + 0x234u, 10000u);
    return 10000u;
}


void game_reset_object_ring_limits(void)
{
    uint32 list, object;
    FUNCTION_MARKER(0x800E7F5C, "GAME.OVL");
    for (list = 0u; list < 2u; ++list) {
        object = object_lookup(list == 0u ? 3u : 1u);
        while (object != 0u) {
            if ((r_u32(object + 0x114u) & 16u) != 0u) {
                w_u32(object + 0x234u, 10000u);
                w_u32(object + 0x238u, 10000u);
            }
            object = r_u32(object + 0x1DCu);
        }
    }
}


void game_process_ring_crossing(uint32 object, uint32 direction)
{
    uint32 ring, offset, frame, timer, selected, expected, format, flags, value;
    FUNCTION_MARKER(0x800E5710, "GAME.OVL");
    ring = r_u32(object + 0xA8u);
    offset = (ring - 1u) * 12u;
    if (r_u32(0x80124934u + offset) != 0u)
        return;
    frame = r_u32(0x800311B8u);
    timer = r_u32(object + 0xACu);
    if (frame - timer < 60u)
        return;
    w_u32(object + 0xACu, frame);
    if (direction == 0u) {
        selected = r_u32(0x800F18A4u);
        ring = r_u32(object + 0xA8u);
        expected = r_u32(0x80124938u + selected * 12u);
        if (ring != expected)
            return;
        timer = r_u32(0x800F18F8u);
        if (frame - timer < 91u)
            return;
        format = r_u32(0x800F2474u);
        w_u32(0x800F18F8u, frame);
        sprintfPSX(0x800FA368u, format, frame);
        game_request_radio_message(0x145u, 0x800FA368u);
    } else if (direction == 1u) {
        selected = r_u32(0x800F18A4u);
        ring = r_u32(object + 0xA8u);
        expected = r_u32(0x80124938u + selected * 12u);
        if (ring != expected)
            return;
        w_u32(0x80124934u + offset, direction);
        flags = r_u32(object + 0x114u);
        w_u32(object + 0xD4u, 0x808080u);
        w_u32(object + 0x114u, flags | 0x80u);
        value = r_u32(0x800F18A4u);
        if (value == 0u) {
            format = r_u32(0x800F2478u);
            sprintfPSX(0x800FA368u, format, frame);
            game_request_radio_message(0x144u, 0x800FA368u);
            value = r_u32(0x800F18A4u);
        }
        w_u32(0x800F18A4u, value + 1u);
    }
}


uint32 game_crosses_face_native(uint32 face, const VECTOR *start, const VECTOR *end)
{
    static const uint32 first_add_pc[4] = {0x800D10FCu, 0x800D1130u, 0x800D1180u, 0x800D11C8u};
    static const uint32 second_add_pc[4] = {0x800D1108u, 0x800D113Cu, 0x800D118Cu, 0x800D11D4u};
    uint32 anchor_x, anchor_y, anchor_z, start_x, start_y, start_z, end_x, end_y, end_z;
    uint32 nx, ny, nz, first, second, product, sum, near_distance, far_distance, denominator;
    uint32 point_x, point_y, point_z, dx, dy, dz, edge, normal, kind = 0u;
    sint64 checked_sum;
    FUNCTION_MARKER(0x800D0F70, "GAME.OVL");
    anchor_x = (uint32)r_s16(face + 12u);
    anchor_y = (uint32)r_s16(face + 14u);
    anchor_z = (uint32)r_s16(face + 16u);
    start_x = (uint32)(sint32)(sint16)start->vx;
    start_y = (uint32)(sint32)(sint16)start->vy;
    start_z = (uint32)(sint32)(sint16)start->vz;
    nx = (uint32)r_s16(face + 4u);
    first = nx * (start_x - anchor_x);
    w_u32(0x801079B0u, nx);
    ny = (uint32)r_s16(face + 6u);
    second = ny * (start_y - anchor_y);
    w_u32(0x801079B4u, ny);
    nz = (uint32)r_s16(face + 8u);
    checked_sum = (sint64)(sint32)first + (sint32)second;
    if (checked_sum < -2147483647LL - 1 || checked_sum > 2147483647LL)
        xport_mips_overflow_exception(0x800D1008u);
    sum = (uint32)checked_sum;
    product = nz * (start_z - anchor_z);
    w_u32(0x801079B8u, nz);
    checked_sum = (sint64)(sint32)sum + (sint32)product;
    if (checked_sum < -2147483647LL - 1 || checked_sum > 2147483647LL)
        xport_mips_overflow_exception(0x800D1018u);
    near_distance = (uint32)((sint32)(uint32)checked_sum >> 12);
    if ((sint32)near_distance >= 0)
        return 0u;
    end_x = (uint32)(sint32)(sint16)end->vx;
    end_y = (uint32)(sint32)(sint16)end->vy;
    end_z = (uint32)(sint32)(sint16)end->vz;
    first = nx * (end_x - anchor_x);
    second = ny * (end_y - anchor_y);
    checked_sum = (sint64)(sint32)first + (sint32)second;
    if (checked_sum < -2147483647LL - 1 || checked_sum > 2147483647LL)
        xport_mips_overflow_exception(0x800D1050u);
    sum = (uint32)checked_sum;
    product = nz * (end_z - anchor_z);
    checked_sum = (sint64)(sint32)sum + (sint32)product;
    if (checked_sum < -2147483647LL - 1 || checked_sum > 2147483647LL)
        xport_mips_overflow_exception(0x800D105Cu);
    far_distance = (uint32)((sint32)(uint32)checked_sum >> 12);
    if ((sint32)far_distance <= 0)
        return 0u;
    near_distance = 0u - near_distance;
    denominator = near_distance + far_distance;
    product = (end_x - start_x) * near_distance;
    point_x = (uint32)((sint32)product / (sint32)denominator) + start_x;
    product = (end_y - start_y) * near_distance;
    point_y = (uint32)((sint32)product / (sint32)denominator) + start_y;
    product = (end_z - start_z) * near_distance;
    point_z = (uint32)((sint32)product / (sint32)denominator) + start_z;
    dx = point_x - anchor_x;
    dy = point_y - anchor_y;
    dz = point_z - anchor_z;
    for (edge = 0u; edge < 4u; ++edge) {
        if (edge == 2u) {
            anchor_x = (uint32)r_s16(face + 20u);
            anchor_y = (uint32)r_s16(face + 22u);
            anchor_z = (uint32)r_s16(face + 24u);
            dx = point_x - anchor_x;
            dy = point_y - anchor_y;
            dz = point_z - anchor_z;
        }
        normal = face + 28u + edge * 8u;
        nx = (uint32)r_s16(normal);
        ny = (uint32)r_s16(normal + 2u);
        first = nx * dx;
        nz = (uint32)r_s16(normal + 4u);
        second = ny * dy;
        checked_sum = (sint64)(sint32)first + (sint32)second;
        if (checked_sum < -2147483647LL - 1 || checked_sum > 2147483647LL)
            xport_mips_overflow_exception(first_add_pc[edge]);
        sum = (uint32)checked_sum;
        product = nz * dz;
        checked_sum = (sint64)(sint32)sum + (sint32)product;
        if (checked_sum < -2147483647LL - 1 || checked_sum > 2147483647LL)
            xport_mips_overflow_exception(second_add_pc[edge]);
        if (edge == 2u)
            kind = r_u32(face);
        if ((sint32)(uint32)checked_sum > 0)
            return 0u;
        if (edge == 2u && kind == 0x02A5B8A7u)
            return 0u;
    }
    w_u32(0x800FA210u, point_x);
    w_u32(0x800FA214u, point_y);
    w_u32(0x800FA218u, point_z);
    return 1u;
}


uint32 game_test_ring_crossing(uint32 object, uint32 target)
{
    VECTOR current, previous, local_current, local_previous;
    uint32 model, table, descriptor, target_value, source_value, bound, velocity, scale;
    uint32 x, y, z, px, py, pz, magnitude, mesh, offset, index, face_index, faces, face;
    sint32 count;
    FUNCTION_MARKER(0x800E5F48, "GAME.OVL");
    model = r_u32(object + 0x74u);
    table = r_u32(0x80046178u);
    descriptor = r_u32(table + model * 132u + 0x88u);
    if (descriptor == 0u)
        return 0u;
    target_value = r_u32(target + 8u);
    source_value = r_u32(object + 8u);
    bound = r_u32(descriptor + 16u);
    x = target_value - source_value;
    target_value = r_u32(target + 12u);
    source_value = r_u32(object + 12u);
    y = target_value - source_value;
    target_value = r_u32(target + 16u);
    source_value = r_u32(object + 16u);
    z = target_value - source_value;
    velocity = (uint32)(r_s32(target + 0x14u) >> 6);
    target_value = r_u32(target + 8u);
    source_value = r_u32(object + 8u);
    px = target_value - velocity - source_value;
    velocity = (uint32)(r_s32(target + 0x18u) >> 6);
    target_value = r_u32(target + 12u);
    source_value = r_u32(object + 12u);
    py = target_value - velocity - source_value;
    velocity = (uint32)(r_s32(target + 0x1Cu) >> 6);
    target_value = r_u32(target + 16u);
    source_value = r_u32(object + 16u);
    pz = target_value - velocity - source_value;
    scale = r_u32(object + 0x84u);
    bound <<= 1;
    if ((sint32)scale > 0) {
        x = (uint32)((sint32)x >> (scale & 31u));
        y = (uint32)((sint32)y >> (scale & 31u));
        z = (uint32)((sint32)z >> (scale & 31u));
        px = (uint32)((sint32)px >> (scale & 31u));
        py = (uint32)((sint32)py >> (scale & 31u));
        pz = (uint32)((sint32)pz >> (scale & 31u));
    } else if ((sint32)scale < 0) {
        scale = 0u - scale;
        x <<= scale & 31u;
        y <<= scale & 31u;
        z <<= scale & 31u;
        px <<= scale & 31u;
        py <<= scale & 31u;
        pz <<= scale & 31u;
    }
    magnitude = (sint32)x < 0 ? 0u - x : x;
    if ((sint32)bound < (sint32)magnitude)
        return 0u;
    magnitude = (sint32)y < 0 ? 0u - y : y;
    if ((sint32)bound < (sint32)magnitude)
        return 0u;
    magnitude = (sint32)z < 0 ? 0u - z : z;
    if ((sint32)bound < (sint32)magnitude)
        return 0u;
    magnitude = (sint32)px < 0 ? 0u - px : px;
    if ((sint32)bound < (sint32)magnitude)
        return 0u;
    magnitude = (sint32)py < 0 ? 0u - py : py;
    if ((sint32)bound < (sint32)magnitude)
        return 0u;
    magnitude = (sint32)pz < 0 ? 0u - pz : pz;
    if ((sint32)bound < (sint32)magnitude)
        return 0u;
    current.vx = (sint32)x;
    current.vy = (sint32)y;
    current.vz = (sint32)z;
    previous.vx = (sint32)px;
    previous.vy = (sint32)py;
    previous.vz = (sint32)pz;
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &current, &local_current);
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &previous, &local_previous);
    mesh = r_u32(descriptor + 8u);
    offset = r_u32(mesh + 0x2Cu);
    mesh += offset;
    count = r_s32(mesh + 0x14u);
    index = 0u;
    while ((sint32)index < count) {
        face_index = (uint32)r_s16(mesh + 0x1Cu + index * 2u);
        faces = r_u32(descriptor);
        face = faces + face_index * 60u;
        if (game_crosses_face_native(face, &local_current, &local_previous) != 0u)
            return 1u;
        if (game_crosses_face_native(face, &local_previous, &local_current) != 0u)
            return 2u;
        ++index;
        count = r_s32(mesh + 0x14u);
    }
    return 0u;
}


void game_update_training_rings(void)
{
    uint32 object, selected, identifier, expected, player, crossing;
    FUNCTION_MARKER(0x800E8114, "GAME.OVL");
    game_reset_player_ring_limits();
    game_reset_object_ring_limits();
    object = object_lookup(3u);
    while (object != 0u) {
        selected = r_u32(0x800F18A4u);
        identifier = r_u32(object + 0xA8u);
        expected = r_u32(0x80124938u + selected * 12u);
        if (identifier == expected) {
            player = r_u32(0x80040FDCu);
            crossing = game_test_ring_crossing(object, player);
            if (crossing == 1u) {
                game_process_ring_crossing(object, 1u);
                break;
            }
            if (crossing == 2u) {
                game_process_ring_crossing(object, 0u);
                break;
            }
        }
        object = r_u32(object + 0x1DCu);
    }
    if (r_u32(0x800F18B4u) == 0u)
        game_draw_paired_glows();
}


void game_set_ring_glow_vertex(uint32 object, uint32 index, uint32 color, uint32 output)
{
    uint32 mesh, vertices, scale, vertex, value;
    FUNCTION_MARKER(0x800E58CC, "GAME.OVL");
    mesh = r_u32(object + 0x64u);
    scale = r_u32(object + 0x84u);
    vertices = r_u32(mesh);
    vertex = vertices + (index << 3);
    if ((sint32)scale > 0) {
        value = (uint32)r_s16(vertex);
        w_u32(output, value << (scale & 31u));
        value = (uint32)r_s16(vertex + 2u);
        scale = r_u32(object + 0x84u);
        w_u32(output + 4u, value << (scale & 31u));
        value = (uint32)r_s16(vertex + 4u);
        scale = r_u32(object + 0x84u);
        w_u32(output + 8u, value << (scale & 31u));
    } else {
        value = (uint32)r_s16(vertex);
        scale = 0u - scale;
        w_u32(output, (uint32)((sint32)value >> (scale & 31u)));
        scale = r_u32(object + 0x84u);
        value = (uint32)r_s16(vertex + 2u);
        scale = 0u - scale;
        w_u32(output + 4u, (uint32)((sint32)value >> (scale & 31u)));
        scale = r_u32(object + 0x84u);
        value = (uint32)r_s16(vertex + 4u);
        scale = 0u - scale;
        w_u32(output + 8u, (uint32)((sint32)value >> (scale & 31u)));
    }
    w_u32(output + 0x10u, object);
    w_u32(output + 0x14u, color);
}


void game_update_ring_glow(uint32 object)
{
    VECTOR relative;
    uint32 selected, identifier, expected, timer, phase, player, source_value, target_value;
    uint32 x, y, z, scale, ax, ay, az, palette, color, index, flags;
    FUNCTION_MARKER(0x800E5BD0, "GAME.OVL");
    FUNCTION_MARKER(0x800E5BD8, "GAME.OVL");
    selected = r_u32(0x800F18A4u);
    identifier = r_u32(object + 0xA8u);
    expected = r_u32(0x80124938u + selected * 12u);
    if (identifier != expected) {
        flags = r_u32(object + 0x114u);
        w_u32(object + 0x9Cu, 0u);
        w_u32(object + 0x114u, flags | 0x80u);
        return;
    }
    timer = r_u32(object + 0x9Cu) - 1u;
    w_u32(object + 0x9Cu, timer);
    if ((sint32)timer <= 0) {
        phase = r_u32(object + 0xB0u) + 1u;
        w_u32(object + 0xB0u, phase);
        if (phase == 12u)
            w_u32(object + 0xB0u, 0u);
        player = r_u32(0x80040FDCu);
        source_value = r_u32(object + 8u);
        target_value = r_u32(player + 8u);
        x = target_value - source_value;
        target_value = r_u32(player + 12u);
        source_value = r_u32(object + 12u);
        y = target_value - source_value;
        target_value = r_u32(player + 16u);
        source_value = r_u32(object + 16u);
        z = target_value - source_value;
        scale = r_u32(object + 0x84u);
        if ((sint32)scale > 0) {
            x = (uint32)((sint32)x >> (scale & 31u));
            scale = r_u32(object + 0x84u);
            y = (uint32)((sint32)y >> (scale & 31u));
            scale = r_u32(object + 0x84u);
            z = (uint32)((sint32)z >> (scale & 31u));
        } else if ((sint32)scale < 0) {
            scale = 0u - scale;
            x <<= scale & 31u;
            scale = r_u32(object + 0x84u);
            if ((sint32)scale < 0)
                scale = 0u - scale;
            y <<= scale & 31u;
            scale = r_u32(object + 0x84u);
            if ((sint32)scale < 0)
                scale = 0u - scale;
            z <<= scale & 31u;
        }
        relative.vx = (sint32)x;
        relative.vy = (sint32)y;
        relative.vz = (sint32)z;
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &relative, &relative);
        for (;;) {
            x = (uint32)relative.vx;
            y = (uint32)relative.vy;
            z = (uint32)relative.vz;
            ax = (sint32)x < 0 ? 0u - x : x;
            ay = (sint32)y < 0 ? 0u - y : y;
            az = (sint32)z < 0 ? 0u - z : z;
            if ((sint32)ax < 16385 && (sint32)ay < 16385 && (sint32)az < 16385)
                break;
            relative.vx >>= 1;
            relative.vy >>= 1;
            relative.vz >>= 1;
        }
        if (((sint32)(z << 12) >> 12) > 0) {
            palette = 0x800F187Cu;
            color = 0xFFu;
        } else {
            palette = 0x800F1888u;
            color = 0xFF00u;
        }
        phase = r_u32(object + 0xB0u);
        index = (uint32)((sint32)phase % 12);
        index = r_u8(palette + index);
        (void)r_u32(0x800311B8u);
        game_set_ring_glow_vertex(object, index, color, 0x800FD4E0u);
        phase = r_u32(object + 0xB0u) + 6u;
        index = (uint32)((sint32)phase % 12);
        index = r_u8(palette + index);
        (void)r_u32(0x800311B8u);
        game_set_ring_glow_vertex(object, index, color, 0x800FD700u);
        w_u32(object + 0x9Cu, 3u);
    }
    flags = r_u32(object + 0x114u);
    if ((flags & 0x80u) != 0u)
        w_u32(object + 0x114u, flags ^ 0x80u);
    w_u32(object + 0xD4u, 0xFF00u);
}


void game_emit_ring_fragments(uint32 object, uint32 position, uint32 mode)
{
    uint32 count;
    FUNCTION_MARKER(0x800E56CC, "GAME.OVL");
    count = r_u32(0x800F18A8u);
    w_u32(0x800F18A8u, count + 1u);
    game_emit_projectile_fragments(position, object, 1, mode);
}


uint32 game_count_objects_by_category(uint32 identifier, uint32 category)
{
    uint32 object, count = 0u, model;
    FUNCTION_MARKER(0x800AE6C8, "GAME.OVL");
    object = object_lookup(identifier);
    while (object != 0u) {
        if (category == 3u)
            ++count;
        else {
            model = r_u32(object + 4u);
            if (r_s16(0x800536BCu + model * 60u) == (sint32)category) {
                if ((r_u32(object + 0x114u) & 0x8000u) == 0u)
                    ++count;
            }
        }
        object = r_u32(object + 0x1DCu);
    }
    return count;
}


sint32 game_play_condition_message(uint32 event)
{
    sint32 message, mission, chapter, table_value, text_index;
    uint32 table, index, record, text, sectors, asset, length;
    FUNCTION_MARKER(0x800E51B4, "GAME.OVL");
    message = r_s16(event + 14u);
    if (message == -1) {
        text_index = r_s16(event + 16u);
        text = 0x80108A50u + (uint32)text_index * 265u;
        length = 0u;
        while (r_u8(text + length) != 0u)
            ++length;
        return stream_enqueue(600u, 0u, (const char *)psx_addr(text, (size_t)length + 1u));
    }
    mission = r_s16(0x800595FAu);
    chapter = r_s16(0x800595F8u);
    table = r_u32(0x800F2B08u);
    index = (uint32)chapter * 7u + (uint32)mission - 1u;
    table_value = r_s16(0x800F15F4u + index * 4u);
    text_index = r_s16(event + 16u);
    record = table + ((uint32)message + (uint32)table_value - 1u) * 24u;
    text = 0x80108A50u + (uint32)text_index * 265u;
    sectors = r_u32(record + 20u);
    asset = r_u32(record + 28u);
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    return stream_enqueue_segments(record + 16u, sectors, asset, 0x800118FEu, 0u,
        (const char *)psx_addr(text, (size_t)length + 1u));
}


uint32 game_check_object_count_message(uint32 event)
{
    uint32 identifier, count, index, required;
    sint32 maximum, present;
    FUNCTION_MARKER(0x800E5538, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    count = game_count_objects_by_category(identifier, 1u);
    identifier = (uint32)r_s16(event);
    if (identifier == 3u)
        maximum = r_s16(0x800595E8u);
    else
        maximum = r_s16(0x800595EAu);
    index = r_u32(event + 8u);
    present = r_s16(0x800536BEu + index * 60u);
    if (maximum < (sint32)index)
        return 0u;
    required = r_u32(event + 4u);
    if ((sint32)count < (sint32)required && present <= 0) {
        game_play_condition_message(event);
        return 1u;
    }
    return 0u;
}


uint32 game_check_ring_count_message(uint32 event)
{
    uint32 identifier, object, count = 0u, required;
    sint32 present;
    FUNCTION_MARKER(0x800E560C, "GAME.OVL");
    identifier = (uint32)r_s16(event);
    present = r_s16(0x800536BEu + identifier * 60u);
    object = object_lookup(3u);
    while (object != 0u) {
        if ((r_u32(object + 0x114u) & 16u) != 0u)
            ++count;
        object = r_u32(object + 0x1DCu);
    }
    if (count == 0u)
        return 0u;
    required = r_u32(event + 4u);
    if (present < (sint32)required)
        return 0u;
    game_play_condition_message(event);
    return 1u;
}


void game_draw_timed_point_effects(uint32 mode, uint32 effects, uint32 count, uint32 camera)
{
    static const uint32 vertex_add_pc[3] = {0x800D0C58u, 0x800D0CF4u, 0x800D0D8Cu};
    SVECTOR vertex;
    sint32 screen[3], lifetime, fade;
    sint64 checked;
    uint32 position[3], velocity[3], camera_position[3], relative[3], coordinate[3];
    uint32 axis, corner, color, value, depth, register_value, packet, base, slot, head, tag, ordering_depth;
    uint16 uv0, uv1, uv2, page, palette;
    FUNCTION_MARKER(0x800D0B68, "GAME.OVL");
    do {
        lifetime = r_s16(effects);
        if (lifetime <= 0)
            goto next_timed_point;
        --lifetime;
        w_u16(effects, (uint16)lifetime);
        for (axis = 0u; axis < 3u; ++axis)
            position[axis] = r_u32(effects + 0x28u + axis * 4u);
        for (axis = 0u; axis < 3u; ++axis)
            velocity[axis] = (uint32)r_s16(effects + 0x38u + axis * 2u);
        for (axis = 0u; axis < 3u; ++axis) {
            checked = (sint64)(sint32)position[axis] + (sint32)velocity[axis];
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800D0BD8u + axis * 4u);
            position[axis] = (uint32)checked;
        }
        fade = r_s16(effects + 2u);
        for (axis = 0u; axis < 3u; ++axis)
            w_u32(effects + 0x28u + axis * 4u, position[axis]);
        if (fade != 0) {
            fade -= 4;
            if (fade < 0)
                fade = 0;
            w_u16(effects + 2u, (uint16)fade);
        }
        value = (uint32)(lifetime + fade);
        if (value >= 255u)
            value = 255u;
        color = value + (value << 8) + (value << 16);
        for (axis = 0u; axis < 3u; ++axis)
            camera_position[axis] = r_u32(camera + 8u + axis * 4u);
        for (axis = 0u; axis < 3u; ++axis) {
            checked = (sint64)(sint32)position[axis] - (sint32)camera_position[axis];
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800D0C40u + axis * 4u);
            relative[axis] = (uint32)checked;
        }
        for (corner = 0u; corner < 3u; ++corner) {
            for (axis = 0u; axis < 3u; ++axis)
                coordinate[axis] = (uint32)r_s16(effects + 8u + corner * 8u + axis * 2u);
            for (axis = 0u; axis < 3u; ++axis) {
                checked = (sint64)(sint32)coordinate[axis] + (sint32)relative[axis];
                if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                    xport_mips_overflow_exception(vertex_add_pc[corner] + axis * 4u);
                coordinate[axis] = (uint32)checked;
            }
            for (axis = 0u; axis < 3u; ++axis) {
                coordinate[axis] = (uint32)((sint32)coordinate[axis] >> 5);
                if ((sint32)coordinate[axis] > 16383)
                    goto next_timed_point;
            }
            vertex.vx = (sint16)coordinate[0];
            vertex.vy = (sint16)coordinate[1];
            vertex.vz = (sint16)coordinate[2];
            depth = (uint32)gte_project_full_depth(&vertex, &screen[corner], NULL);
            register_value = xport_gte_read_data(31u);
            if ((register_value >> 16) != 0u)
                goto next_timed_point;
            depth -= 64u;
            if ((sint32)depth <= 0)
                goto next_timed_point;
        }
        depth <<= 7;
        uv0 = r_u16(effects + 0x44u);
        uv1 = r_u16(effects + 0x46u);
        uv2 = r_u16(effects + 0x48u);
        page = r_u16(effects + 0x40u);
        palette = r_u16(effects + 0x42u);
        packet = r_u32(0x80040690u);
        w_u32(packet + 4u, color);
        w_u8(packet + 7u, 0x24u);
        w_u8(packet + 3u, 7u);
        w_u32(packet + 8u, (uint32)screen[0]);
        w_u32(packet + 16u, (uint32)screen[1]);
        w_u32(packet + 24u, (uint32)screen[2]);
        w_u16(packet + 12u, uv0);
        w_u16(packet + 20u, uv1);
        w_u16(packet + 28u, uv2);
        w_u16(packet + 14u, palette);
        w_u16(packet + 22u, page);
        base = r_u32(0x800421ACu);
        ordering_depth = depth;
        if ((sint32)ordering_depth < 0x4000) {
            if ((sint32)ordering_depth < 0)
                ordering_depth = 0u;
            value = ((ordering_depth >> 6) + 64u) * 4u;
            checked = (sint64)(sint32)value + (sint32)base;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800D0ED8u);
        } else {
            if ((sint32)(ordering_depth - 0x3FFFFEu) > 0)
                ordering_depth = 0x3FFFFEu;
            value = ((ordering_depth >> 11) + 320u) * 4u;
            checked = (sint64)(sint32)value + (sint32)base;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800D0EB4u);
        }
        slot = (uint32)checked;
        tag = r_u32(packet);
        head = r_u32(slot);
        w_u32(packet, (tag & 0xFF000000u) | (head & 0xFFFFFFu));
        head = r_u32(slot);
        w_u32(slot, (head & 0xFF000000u) | (packet & 0xFFFFFFu));
        value = r_u32(0x80040690u);
        checked = (sint64)(sint32)value + 32;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800D0F20u);
        w_u32(0x80040690u, (uint32)checked);
next_timed_point:
        checked = (sint64)(sint32)effects + 76;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800D0F30u);
        effects = (uint32)checked;
        checked = (sint64)(sint32)count - 1;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800D0F34u);
        count = (uint32)checked;
    } while ((sint32)count > 0);
}


uint32 game_test_object_collision_box_native(uint32 object, VECTOR *position)
{
    VECTOR local;
    uint32 model, table, state, descriptor, bounds, value, magnitude;
    sint32 extent;
    FUNCTION_MARKER(0x80098438, "GAME.OVL");
    model = r_u32(object + 0x74u);
    table = r_u32(0x80046178u);
    w_u32(0x800EE9C4u, object);
    state = r_u32(object + 0x88u);
    descriptor = r_u32(table + model * 132u + state * 4u + 0x3Cu);
    bounds = r_u32(descriptor + 8u);
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), position, &local);
    value = (uint32)local.vx;
    extent = r_s16(bounds + 8u);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if (extent < (sint32)magnitude)
        return 0u;
    value = (uint32)local.vy;
    extent = r_s16(bounds + 10u);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if (extent < (sint32)magnitude)
        return 0u;
    value = (uint32)local.vz;
    extent = r_s16(bounds + 12u);
    magnitude = (sint32)value < 0 ? 0u - value : value;
    if (extent < (sint32)magnitude)
        return 0u;
    w_u32(0x800FA210u, (uint32)local.vx);
    w_u32(0x800FA214u, (uint32)local.vy);
    w_u32(0x800FA218u, (uint32)local.vz);
    return 1u;
}


void game_set_collision_rotation_native(uint32 object, const VECTOR *position, const VECTOR *direction,
    const VECTOR *velocity, uint32 strength)
{
    VECTOR difference, relative, forward, radial, side, up;
    uint32 value, component, product, dot, magnitude;
    FUNCTION_MARKER(0x800A75B0, "GAME.OVL");
    value = (uint32)(velocity->vx >> 8);
    difference.vx = (sint32)((uint32)direction->vx - value);
    value = (uint32)(velocity->vy >> 8);
    difference.vy = (sint32)((uint32)direction->vy - value);
    value = (uint32)(velocity->vz >> 8);
    difference.vz = (sint32)((uint32)direction->vz - value);
    if ((r_u32(object + 0x114u) & 0x240000u) != 0u)
        return;
    value = (uint32)position->vx;
    component = r_u32(object + 8u);
    relative.vx = (sint32)(value - component);
    value = (uint32)position->vy;
    component = r_u32(object + 12u);
    strength = (uint32)((sint32)strength >> 2);
    relative.vy = (sint32)(value - component);
    value = (uint32)position->vz;
    component = r_u32(object + 16u);
    relative.vz = (sint32)(value - component);
    vector_normalize(&difference, &forward);
    vector_normalize(&relative, &radial);
    OuterProduct12(&forward, &radial, &side);
    vector_normalize(&side, &side);
    OuterProduct12(&forward, &side, &up);
    vector_normalize(&up, &up);
    if ((side.vx == 0 && side.vy == 0 && side.vz == 0)
        || (up.vx == 0 && up.vy == 0 && up.vz == 0)
        || (forward.vx == 0 && forward.vy == 0 && side.vz == 0)) {
        side.vx = 4096;
        side.vy = 0;
        side.vz = 0;
        up.vx = 0;
        up.vy = 4096;
        up.vz = 0;
        forward.vx = 0;
        forward.vy = 0;
        forward.vz = 4096;
    }
    w_u16(object + 0x13Cu, (uint16)side.vx);
    w_u16(object + 0x13Eu, (uint16)side.vy);
    w_u16(object + 0x140u, (uint16)side.vz);
    w_u16(object + 0x142u, (uint16)up.vx);
    w_u16(object + 0x144u, (uint16)up.vy);
    w_u16(object + 0x146u, (uint16)up.vz);
    w_u16(object + 0x148u, (uint16)forward.vx);
    w_u16(object + 0x14Au, (uint16)forward.vy);
    w_u16(object + 0x14Cu, (uint16)forward.vz);
    product = (uint32)radial.vx * (uint32)forward.vx;
    dot = (uint32)((sint32)product >> 12);
    product = (uint32)radial.vy * (uint32)forward.vy;
    dot += (uint32)((sint32)product >> 12);
    product = (uint32)radial.vz * (uint32)forward.vz;
    dot += (uint32)((sint32)product >> 12);
    magnitude = (sint32)dot < 0 ? 0u - dot : dot;
    product = (4096u - magnitude) * (strength << 8);
    w_u32(object + 0x15Cu, (uint32)((sint32)product >> 6));
}


void game_resolve_player_collision_native(uint32 object, const VECTOR *velocity)
{
    VECTOR incident, normal, reflected, contact, direction;
    uint32 x, y, z, value, component, magnitude, scale, player, seed, first, second, third;
    uint32 vx, vy, vz, nx, ny, nz, product;
    FUNCTION_MARKER(0x80097D3C, "GAME.OVL");
    value = (uint32)velocity->vx;
    x = r_u32(object + 0x14u) - value;
    value = (uint32)velocity->vy;
    y = r_u32(object + 0x18u) - value;
    value = (uint32)velocity->vz;
    z = r_u32(object + 0x1Cu) - value;
    magnitude = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    if ((sint32)magnitude < 40000)
        magnitude = 40000u;
    incident.vx = velocity->vx;
    incident.vy = velocity->vy;
    incident.vz = velocity->vz;
    vector_normalize(&incident, &incident);
    ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u),
        (VECTOR *)psx_addr(0x801079B0u, 12u), &normal);
    game_reflect_vector_native(&incident, &normal, &reflected);
    contact.vx = r_s32(0x800FA210u);
    contact.vy = r_s32(0x800FA214u);
    contact.vz = r_s32(0x800FA218u);
    ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u), &contact, &contact);
    scale = r_u32(object + 0x84u);
    if ((sint32)scale > 0) {
        contact.vx = (sint32)((uint32)contact.vx << (scale & 31u));
        scale = r_u32(object + 0x84u);
        contact.vy = (sint32)((uint32)contact.vy << (scale & 31u));
        scale = r_u32(object + 0x84u);
        contact.vz = (sint32)((uint32)contact.vz << (scale & 31u));
    } else {
        if ((sint32)scale < 0)
            scale = 0u - scale;
        contact.vx >>= scale & 31u;
        scale = r_u32(object + 0x84u);
        if ((sint32)scale < 0)
            scale = 0u - scale;
        contact.vy >>= scale & 31u;
        scale = r_u32(object + 0x84u);
        if ((sint32)scale < 0)
            scale = 0u - scale;
        contact.vz >>= scale & 31u;
    }
    component = r_u32(object + 8u);
    contact.vx = (sint32)((uint32)contact.vx + component);
    component = r_u32(object + 12u);
    contact.vy = (sint32)((uint32)contact.vy + component);
    component = r_u32(object + 16u);
    contact.vz = (sint32)((uint32)contact.vz + component);
    direction.vx = r_s32(object + 0x14u) >> 8;
    direction.vy = r_s32(object + 0x18u) >> 8;
    value = r_u32(object + 0x1Cu);
    player = r_u32(0x80040FDCu);
    direction.vz = (sint32)value >> 8;
    game_set_collision_rotation_native(player, &contact, &direction, velocity, 10u);
    seed = r_u32(0x800EF50Cu);
    first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    x = (uint32)contact.vx + (first & 511u) - 256u;
    nx = (uint32)normal.vx;
    w_u32(0x800EF50Cu, first);
    second = (uint32)((sint32)first >> 1) ^ (first << 3);
    y = (uint32)contact.vy + (second & 511u) - 256u;
    ny = (uint32)normal.vy;
    w_u32(0x800EF50Cu, second);
    third = (uint32)((sint32)second >> 1) ^ (second << 3);
    z = (uint32)contact.vz + (third & 511u) - 256u;
    nz = (uint32)normal.vz;
    w_u32(0x800EF50Cu, third);
    game_emit_narrow_particles(30u, x, y, z, nx, ny, nz);
    product = (uint32)reflected.vx * magnitude;
    player = r_u32(0x80040FDCu);
    w_u32(player + 0x14u, (uint32)((sint32)product >> 13));
    product = (uint32)reflected.vy * magnitude;
    w_u32(player + 0x18u, (uint32)((sint32)product >> 13));
    product = (uint32)reflected.vz * magnitude;
    w_u32(player + 0x1Cu, (uint32)((sint32)product >> 13));
    value = r_u32(player + 8u);
    w_u32(player + 8u, value + (uint32)(normal.vx >> 4));
    value = r_u32(player + 12u);
    w_u32(player + 12u, value + (uint32)(normal.vy >> 4));
    value = r_u32(player + 16u);
    w_u32(player + 16u, value + (uint32)(normal.vz >> 4));
    value = r_u32(player + 0x14u);
    w_u32(player + 0x14u, value + ((uint32)normal.vx << 1));
    value = r_u32(player + 0x18u);
    vx = r_u32(player + 0x14u);
    w_u32(player + 0x18u, value + ((uint32)normal.vy << 1));
    value = r_u32(player + 8u);
    w_u32(player + 8u, value + (uint32)((sint32)vx >> 8));
    value = r_u32(player + 0x1Cu);
    w_u32(player + 0x1Cu, value + ((uint32)normal.vz << 1));
    vy = r_u32(player + 0x18u);
    value = r_u32(player + 12u);
    vz = r_u32(player + 0x1Cu);
    w_u32(player + 12u, value + (uint32)((sint32)vy >> 8));
    value = r_u32(player + 16u);
    w_u32(player + 16u, value + (uint32)((sint32)vz >> 8));
    sound_play_sample_full(2u);
}


uint32 game_check_player_object_collision(uint32 object)
{
    VECTOR velocity, origin, local_origin, probe, local_probe;
    uint32 model, table, descriptor, reach, player, x, y, z, source_value, player_value, coefficient;
    uint32 first_scale, shift, scale, radius, sample, axis, offset, basis, product, magnitude, mesh;
    uint32 point[3], node, header, child, leaf, face_number, face_index, face_base, face, center, extent;
    sint32 child_count, leaf_count, face_count;
    FUNCTION_MARKER(0x800991E8, "GAME.OVL");
    model = r_u32(object + 0x74u);
    table = r_u32(0x80046178u);
    descriptor = r_u32(table + model * 132u + 4u);
    if (descriptor == 0u)
        return 0u;
    reach = r_u32(object + 0x40u) == 0x800E5BD0u ? 1024u : 3000u;
    player = r_u32(0x80040FDCu);
    velocity.vx = r_s32(player + 0x14u);
    velocity.vy = r_s32(player + 0x18u);
    velocity.vz = r_s32(player + 0x1Cu);
    player_value = r_u32(player + 8u);
    source_value = r_u32(object + 8u);
    first_scale = r_u32(object + 0x84u);
    x = player_value - source_value;
    player_value = r_u32(player + 12u);
    source_value = r_u32(object + 12u);
    y = player_value - source_value;
    player_value = r_u32(player + 16u);
    source_value = r_u32(object + 16u);
    z = player_value - source_value;
    scale = r_u32(object + 0x84u);
    shift = (sint32)first_scale < 0 ? 0u - first_scale : first_scale;
    if ((sint32)scale > 0) {
        x = (uint32)((sint32)x >> (shift & 31u));
        y = (uint32)((sint32)y >> (shift & 31u));
        z = (uint32)((sint32)z >> (shift & 31u));
    } else if ((sint32)scale < 0) {
        x <<= shift & 31u;
        y <<= shift & 31u;
        z <<= shift & 31u;
    }
    origin.vx = (sint32)x;
    origin.vy = (sint32)y;
    origin.vz = (sint32)z;
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &origin, &local_origin);
    radius = r_u32(descriptor + 16u);
    for (sample = 0u; sample < 6u; ++sample) {
        offset = (sample & 1u) != 0u ? 0u - reach : reach;
        axis = sample >> 1;
        player = r_u32(0x80040FDCu);
        basis = player + axis * 6u;
        coefficient = (uint32)r_s16(basis + 0x11Cu);
        source_value = r_u32(object + 8u);
        player_value = r_u32(player + 8u);
        product = coefficient * offset;
        x = player_value + (uint32)((sint32)product >> 12) - source_value;
        coefficient = (uint32)r_s16(basis + 0x11Eu);
        source_value = r_u32(object + 12u);
        player_value = r_u32(player + 12u);
        product = coefficient * offset;
        y = player_value + (uint32)((sint32)product >> 12) - source_value;
        coefficient = (uint32)r_s16(basis + 0x120u);
        source_value = r_u32(object + 16u);
        player_value = r_u32(player + 16u);
        product = coefficient * offset;
        z = player_value + (uint32)((sint32)product >> 12) - source_value;
        scale = r_u32(object + 0x84u);
        if ((sint32)scale > 0) {
            x = (uint32)((sint32)x >> (shift & 31u));
            y = (uint32)((sint32)y >> (shift & 31u));
            z = (uint32)((sint32)z >> (shift & 31u));
        } else if ((sint32)scale < 0) {
            x <<= shift & 31u;
            y <<= shift & 31u;
            z <<= shift & 31u;
        }
        magnitude = (sint32)x < 0 ? 0u - x : x;
        if ((sint32)magnitude >= (sint32)radius)
            continue;
        magnitude = (sint32)y < 0 ? 0u - y : y;
        if ((sint32)magnitude >= (sint32)radius)
            continue;
        magnitude = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)magnitude >= (sint32)radius)
            continue;
        probe.vx = (sint32)x;
        probe.vy = (sint32)y;
        probe.vz = (sint32)z;
        if ((r_u32(object + 0x114u) & 4u) != 0u) {
            if (game_test_object_collision_box_native(object, &probe) != 0u)
                goto player_collision_found;
        }
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &probe, &local_probe);
        point[0] = (uint32)local_probe.vx;
        point[1] = (uint32)local_probe.vy;
        point[2] = (uint32)local_probe.vz;
        mesh = r_u32(descriptor + 8u);
        for (axis = 0u; axis < 3u; ++axis) {
            center = (uint32)r_s16(mesh + axis * 2u);
            extent = (uint32)r_s16(mesh + 8u + axis * 2u);
            magnitude = point[axis] - center;
            if ((sint32)magnitude < 0)
                magnitude = 0u - magnitude;
            if ((sint32)extent < (sint32)magnitude)
                break;
        }
        if (axis != 3u)
            continue;
        child_count = r_s32(mesh + 0x18u);
        node = mesh;
        header = mesh + 0x1Cu;
        for (child = 0u; (sint32)child < child_count; ++child) {
            for (axis = 0u; axis < 3u; ++axis) {
                center = (uint32)r_s16(header + axis * 2u);
                extent = (uint32)r_s16(header + 8u + axis * 2u);
                magnitude = point[axis] - center;
                if ((sint32)magnitude < 0)
                    magnitude = 0u - magnitude;
                if ((sint32)extent < (sint32)magnitude)
                    break;
            }
            if (axis == 3u) {
                offset = r_u32(header + 0x10u);
                node += offset;
                break;
            }
            header += 0x1Cu;
        }
        leaf_count = r_s32(header + 0x18u);
        for (leaf = 0u; (sint32)leaf < leaf_count; ++leaf) {
            for (axis = 0u; axis < 3u; ++axis) {
                center = (uint32)r_s16(node + axis * 2u);
                extent = (uint32)r_s16(node + 8u + axis * 2u);
                magnitude = point[axis] - center;
                if ((sint32)magnitude < 0)
                    magnitude = 0u - magnitude;
                if ((sint32)extent < (sint32)magnitude)
                    break;
            }
            if (axis == 3u) {
                face_count = r_s32(node + 0x14u);
                face_number = 0u;
                while ((sint32)face_number < face_count) {
                    face_index = (uint32)r_s16(node + 0x1Cu + face_number * 2u);
                    face_base = r_u32(descriptor);
                    face = face_base + face_index * 60u;
                    if (game_crosses_face_native(face, &local_probe, &local_origin) != 0u)
                        goto player_collision_found;
                    ++face_number;
                    face_count = r_s32(node + 0x14u);
                }
            }
            offset = r_u32(node + 0x10u);
            node += offset;
        }
    }
    return 0u;
player_collision_found:
    game_resolve_player_collision_native(object, &velocity);
    return 1u;
}


void game_notify_object_contact(uint32 first, uint32 second)
{
    uint32 first_model, second_model, phase, object, other, identity, assigned, previous, flags, callback;
    FUNCTION_MARKER(0x800A18B4, "GAME.OVL");
    first_model = r_u32(first + 4u);
    second_model = r_u32(second + 4u);
    if (first_model == second_model) {
        if ((r_u32(first + 0x114u) & 1u) != 0u)
            return;
    }
    for (phase = 0u; phase < 2u; ++phase) {
        object = phase == 0u ? first : second;
        other = phase == 0u ? second : first;
        w_u32(object + 0x190u, other);
        identity = r_u16(other + 0x24Au);
        assigned = r_u32(object + 0x190u);
        previous = r_u32(object + 0x198u);
        w_u16(object + 0x194u, (uint16)identity);
        if (assigned == previous)
            continue;
        flags = r_u32(object + 0x114u);
        if ((flags & 16u) != 0u)
            continue;
        if ((flags & 0x2000u) != 0u) {
            if ((r_u32(other + 0x114u) & 16u) == 0u)
                continue;
        }
        identity = r_u16(object + 0x194u);
        callback = r_u32(object + 0x58u);
        w_u32(object + 0x198u, assigned);
        w_u16(object + 0x19Cu, (uint16)identity);
        psx_dispatch_guest_call(callback, 2u, object, assigned, 0u);
    }
}


void game_resolve_object_collision(uint32 object, uint32 obstacle)
{
    VECTOR incident, normal, reflected, up, side;
    uint32 value, speed, product;
    FUNCTION_MARKER(0x80099DB4, "GAME.OVL");
    incident.vx = r_s32(object + 0x14u) >> 8;
    incident.vy = r_s32(object + 0x18u) >> 8;
    incident.vz = r_s32(object + 0x1Cu) >> 8;
    vector_normalize(&incident, &incident);
    ApplyMatrixLV((MATRIX *)psx_addr(obstacle + 0x11Cu, 20u),
        (VECTOR *)psx_addr(0x801079B0u, 12u), &normal);
    game_reflect_vector_native(&incident, &normal, &reflected);
    reflected.vx = normal.vx;
    reflected.vy = normal.vy;
    reflected.vz = normal.vz;
    up.vx = r_s16(object + 0x11Eu);
    up.vy = r_s16(object + 0x124u);
    up.vz = r_s16(object + 0x12Au);
    OuterProduct12(&normal, &up, &side);
    vector_normalize(&side, &side);
    OuterProduct12(&normal, &side, &up);
    w_u16(object + 0x120u, (uint16)normal.vx);
    w_u16(object + 0x126u, (uint16)normal.vy);
    w_u16(object + 0x12Cu, (uint16)normal.vz);
    w_u16(object + 0x11Eu, (uint16)up.vx);
    w_u16(object + 0x124u, (uint16)up.vy);
    w_u16(object + 0x12Au, (uint16)up.vz);
    w_u16(object + 0x11Cu, (uint16)side.vx);
    w_u16(object + 0x122u, (uint16)side.vy);
    w_u16(object + 0x128u, (uint16)side.vz);
    matrix_normalize(object + 0x11Cu, 1u);
    speed = r_u32(object + 0xECu);
    if ((sint32)speed < 80000)
        speed = 80000u;
    w_u32(object + 0xECu, 0u);
    w_u32(object + 0xE8u, 0u);
    value = r_u32(object + 8u);
    w_u32(object + 8u, value + (uint32)(normal.vx >> 2));
    value = r_u32(object + 12u);
    w_u32(object + 12u, value + (uint32)(normal.vy >> 2));
    value = r_u32(object + 16u);
    w_u32(object + 16u, value + (uint32)(normal.vz >> 2));
    product = (uint32)reflected.vx * speed;
    w_u32(object + 0x14u, (uint32)((sint32)product >> 12));
    product = (uint32)reflected.vy * speed;
    w_u32(object + 0x18u, (uint32)((sint32)product >> 12));
    product = (uint32)reflected.vz * speed;
    w_u32(object + 0x1Cu, (uint32)((sint32)product >> 12));
    product = (uint32)reflected.vx * speed;
    w_u32(object + 0xFCu, (uint32)((sint32)product >> 12));
    product = (uint32)reflected.vy * speed;
    w_u32(object + 0x100u, (uint32)((sint32)product >> 12));
    product = (uint32)reflected.vz * speed;
    w_u32(object + 0x104u, (uint32)((sint32)product >> 12));
    value = r_u32(object + 0x14u);
    w_u32(object + 0x14u, value + ((uint32)normal.vx << 1));
    value = r_u32(object + 0x18u);
    w_u32(object + 0x18u, value + ((uint32)normal.vy << 1));
    value = r_u32(object + 0x1Cu);
    w_u32(object + 0x1Cu, value + ((uint32)normal.vz << 1));
}


sint32 game_test_object_collision_native(uint32 object, uint32 other, const VECTOR *start, const VECTOR *end)
{
    VECTOR current, previous, local_current, local_previous;
    uint32 player, model, table, descriptor, target_value, source_value, input_value, radius, margin;
    uint32 x, y, z, px, py, pz, scale, shift, magnitude, type, mesh, header, node, offset;
    uint32 point[3], extents[3], axis, child, leaf, face_number, center, extent, face_index, face_base;
    sint32 child_count, leaf_count, face_count;
    FUNCTION_MARKER(0x80099768, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    if (other == player)
        return (sint32)game_check_player_object_collision(object);
    model = r_u32(object + 0x74u);
    table = r_u32(0x80046178u);
    descriptor = r_u32(table + model * 132u + 4u);
    if (descriptor == 0u)
        return -1;
    target_value = r_u32(other + 8u);
    source_value = r_u32(object + 8u);
    input_value = (uint32)start->vx;
    radius = r_u32(descriptor + 16u);
    x = target_value - source_value - input_value;
    target_value = r_u32(other + 12u);
    source_value = r_u32(object + 12u);
    input_value = (uint32)start->vy;
    y = target_value - source_value - input_value;
    target_value = r_u32(other + 16u);
    source_value = r_u32(object + 16u);
    input_value = (uint32)start->vz;
    z = target_value - source_value - input_value;
    target_value = r_u32(other + 8u);
    source_value = r_u32(object + 8u);
    input_value = (uint32)end->vx;
    px = target_value - source_value - input_value;
    target_value = r_u32(other + 12u);
    source_value = r_u32(object + 12u);
    input_value = (uint32)end->vy;
    py = target_value - source_value - input_value;
    target_value = r_u32(other + 16u);
    source_value = r_u32(object + 16u);
    input_value = (uint32)end->vz;
    pz = target_value - source_value - input_value;
    scale = r_u32(object + 0x84u);
    shift = (sint32)scale < 0 ? 0u - scale : scale;
    margin = radius;
    if ((sint32)scale > 0) {
        margin += (uint32)((sint32)5000 >> (shift & 31u));
        x = (uint32)((sint32)x >> (shift & 31u));
        y = (uint32)((sint32)y >> (shift & 31u));
        z = (uint32)((sint32)z >> (shift & 31u));
        px = (uint32)((sint32)px >> (shift & 31u));
        py = (uint32)((sint32)py >> (shift & 31u));
        pz = (uint32)((sint32)pz >> (shift & 31u));
    } else if ((sint32)scale < 0) {
        margin += 5000u << (shift & 31u);
        x <<= shift & 31u;
        y <<= shift & 31u;
        z <<= shift & 31u;
        px <<= shift & 31u;
        py <<= shift & 31u;
        pz <<= shift & 31u;
    }
    current.vx = (sint32)x;
    current.vy = (sint32)y;
    current.vz = (sint32)z;
    previous.vx = (sint32)px;
    previous.vy = (sint32)py;
    previous.vz = (sint32)pz;
    magnitude = (sint32)x < 0 ? 0u - x : x;
    if ((sint32)magnitude < (sint32)margin) {
        magnitude = (sint32)y < 0 ? 0u - y : y;
        if ((sint32)magnitude < (sint32)margin) {
            magnitude = (sint32)z < 0 ? 0u - z : z;
            if ((sint32)magnitude < (sint32)margin) {
                type = r_u32(other);
                if (type == 3u || type == 1u)
                    game_notify_object_contact(object, other);
            }
        }
    }
    magnitude = (sint32)x < 0 ? 0u - x : x;
    if ((sint32)radius < (sint32)magnitude)
        return -1;
    magnitude = (sint32)y < 0 ? 0u - y : y;
    if ((sint32)radius < (sint32)magnitude)
        return -1;
    magnitude = (sint32)z < 0 ? 0u - z : z;
    if ((sint32)radius < (sint32)magnitude)
        return -1;
    if ((r_u32(object + 0x114u) & 4u) != 0u)
        return (sint32)game_test_object_collision_box_native(object, &current);
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &current, &local_current);
    mesh = r_u32(descriptor + 8u);
    point[0] = (uint32)local_current.vx;
    point[1] = (uint32)local_current.vy;
    point[2] = (uint32)local_current.vz;
    for (axis = 0u; axis < 3u; ++axis) {
        center = (uint32)r_s16(mesh + axis * 2u);
        magnitude = point[axis] - center;
        extent = (uint32)r_s16(mesh + 8u + axis * 2u);
        if ((sint32)magnitude < 0)
            magnitude = 0u - magnitude;
        if ((sint32)extent < (sint32)magnitude)
            return 2;
    }
    child_count = r_s32(mesh + 0x18u);
    header = mesh + 0x1Cu;
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &previous, &local_previous);
    for (child = 0u; (sint32)child < child_count; ++child, header += 0x1Cu) {
        for (axis = 0u; axis < 3u; ++axis)
            extents[axis] = (uint32)r_s16(header + 8u + axis * 2u);
        if (r_s32(object + 0x84u) < 4) {
            for (axis = 0u; axis < 3u; ++axis)
                extents[axis] <<= 1;
        }
        for (axis = 0u; axis < 3u; ++axis) {
            center = (uint32)r_s16(header + axis * 2u);
            magnitude = point[axis] - center;
            if ((sint32)magnitude < 0)
                magnitude = 0u - magnitude;
            if ((sint32)extents[axis] < (sint32)magnitude)
                break;
        }
        if (axis != 3u)
            continue;
        offset = r_u32(header + 0x10u);
        leaf_count = r_s32(header + 0x18u);
        node = mesh + offset;
        for (leaf = 0u; (sint32)leaf < leaf_count; ++leaf) {
            for (axis = 0u; axis < 3u; ++axis)
                extents[axis] = (uint32)r_s16(node + 8u + axis * 2u);
            if (r_s32(object + 0x84u) < 4) {
                for (axis = 0u; axis < 3u; ++axis)
                    extents[axis] <<= 1;
            }
            for (axis = 0u; axis < 3u; ++axis) {
                center = (uint32)r_s16(node + axis * 2u);
                magnitude = point[axis] - center;
                if ((sint32)magnitude < 0)
                    magnitude = 0u - magnitude;
                if ((sint32)extents[axis] < (sint32)magnitude)
                    break;
            }
            if (axis == 3u) {
                face_count = r_s32(node + 0x14u);
                face_number = 0u;
                while ((sint32)face_number < face_count) {
                    face_index = (uint32)r_s16(node + 0x1Cu + face_number * 2u);
                    face_base = r_u32(descriptor);
                    if (game_crosses_face_native(face_base + face_index * 60u, &local_current, &local_previous) != 0u) {
                        type = r_u32(other);
                        if (type == 3u || type == 1u)
                            game_resolve_object_collision(other, object);
                        return 1;
                    }
                    face_count = r_s32(node + 0x14u);
                    ++face_number;
                }
            }
            offset = r_u32(node + 0x10u);
            node += offset;
        }
    }
    return 2;
}


uint32 game_check_swept_object_collision(uint32 object, uint32 other)
{
    VECTOR start, end, step;
    uint32 player, x, y, z, value, position, shift, segments, index;
    sint32 distance, result = 0;
    FUNCTION_MARKER(0x8009A050, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    if (other == player)
        return game_check_player_object_collision(object);
    start.vx = 0;
    start.vy = 0;
    start.vz = 0;
    position = r_u32(other + 8u);
    value = r_u32(other + 0x90u);
    x = position - value;
    position = r_u32(other + 12u);
    value = r_u32(other + 0x94u);
    y = position - value;
    position = r_u32(other + 16u);
    value = r_u32(other + 0x98u);
    z = position - value;
    distance = vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    if (distance < 2001) {
        result = game_test_object_collision_native(object, other, &start,
            (const VECTOR *)psx_addr(other + 0x90u, 12u));
        return result == 1;
    }
    if ((uint32)distance - 2001u < 2000u) {
        shift = 1u;
        segments = 2u;
    } else {
        if (distance < 4001)
            return 0u;
        shift = 2u;
        segments = 4u;
    }
    step.vx = (sint32)x >> shift;
    step.vz = (sint32)z >> shift;
    step.vy = (sint32)y >> shift;
    end.vx = step.vx;
    end.vz = step.vz;
    end.vy = step.vy;
    for (index = 0u; index < segments; ++index) {
        result = game_test_object_collision_native(object, other, &start, &end);
        if (result == -1 || result == 1)
            return result == 1;
        start.vx = end.vx;
        start.vy = end.vy;
        start.vz = end.vz;
        end.vx = (sint32)((uint32)end.vx + (uint32)step.vx);
        end.vy = (sint32)((uint32)end.vy + (uint32)step.vy);
        end.vz = (sint32)((uint32)end.vz + (uint32)step.vz);
    }
    return result == 1;
}


void game_draw_camera_overlay_points_native(uint32 count, uint32 *result)
{
    static const uint32 advance_pc[5] = {0x800CF1B0u, 0x800CF268u, 0x800CF2C0u, 0x800CF324u, 0x800CF384u};
    static const uint32 halo_offset[4] = {0xFFFFFFFFu, 1u, 0x10000u, 0xFFFF0000u};
    uint32 heads[3] = {0x800F4028u, 0x80118A88u, 0x80118C88u};
    SVECTOR vertex;
    sint32 screen;
    sint64 checked;
    uint32 bin = 0u, slot, point, packed, depth, seed, random, dim, color, halo_color = 0u;
    uint32 star_number, pixels, pixel, packet, next_packet, phase, value, position, base_color;
    FUNCTION_MARKER(0x800CF05C, "GAME.OVL");
    slot = r_u32(0x800421ACu) + 0x4504u;
    do {
        point = r_u32(heads[0]);
        if (point == 0u)
            goto next_overlay_bin;
        packed = r_u32(point);
        vertex.vz = r_s16(point + 4u);
        vertex.vx = (sint16)packed;
        vertex.vy = (sint16)(packed >> 16);
        depth = (uint32)gte_project_full_depth(&vertex, NULL, NULL) - 0xB00u;
        if ((sint32)depth <= 0)
            goto next_overlay_bin;
        star_number = 32u;
        do {
            seed = r_u32(0x800311BCu);
            random = (uint32)((sint32)seed >> 1) ^ (seed << 3);
            w_u32(0x800311BCu, random);
            dim = (random & 127u) == 0u ? 1u : 0u;
            color = 0x808080u >> dim;
            packed = r_u32(point);
            vertex.vz = r_s16(point + 4u);
            vertex.vx = (sint16)packed;
            vertex.vy = (sint16)(packed >> 16);
            gte_project_full_depth(&vertex, &screen, NULL);
            packet = r_u32(0x80040690u);
            pixels = (star_number & 15u) == 0u ? 5u : 1u;
            for (pixel = 0u; pixel < pixels; ++pixel) {
                if (pixel == 1u) {
                    checked = (sint64)(sint32)star_number + (sint32)(star_number << 8);
                    if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                        xport_mips_overflow_exception(0x800CF20Cu);
                    value = (uint32)checked;
                    checked = (sint64)(sint32)value + (sint32)(star_number << 9);
                    if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                        xport_mips_overflow_exception(0x800CF214u);
                    halo_color = (uint32)checked;
                }
                position = (uint32)screen;
                if (pixel != 0u)
                    position += halo_offset[pixel - 1u];
                w_u32(packet + 4u, pixel == 0u ? color : halo_color);
                w_u8(packet + 7u, 0x6Au);
                w_u8(packet + 3u, 2u);
                w_u32(packet + 8u, position);
                if (pixel == 0u) {
                    checked = (sint64)(sint32)packet + 12;
                    if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                        xport_mips_overflow_exception(advance_pc[pixel]);
                    next_packet = (uint32)checked;
                }
                AddPrim(psx_addr(slot, 4u), psx_addr(packet, 4u));
                if (pixel != 0u) {
                    checked = (sint64)(sint32)packet + 12;
                    if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                        xport_mips_overflow_exception(advance_pc[pixel]);
                    next_packet = (uint32)checked;
                }
                packet = next_packet;
            }
            w_u32(0x80040690u, packet);
            checked = (sint64)(sint32)star_number + 1;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CF390u);
            star_number = (uint32)checked;
            point = r_u32(point + 8u);
        } while (point != 0u);
        for (phase = 1u; phase < 3u; ++phase) {
            point = r_u32(heads[phase]);
            if (point == 0u)
                break;
            base_color = phase == 1u ? 0x202020u : 0x404040u;
            do {
                seed = r_u32(0x800311BCu);
                random = (uint32)((sint32)seed >> 1) ^ (seed << 3);
                w_u32(0x800311BCu, random);
                dim = (random & 127u) == 0u ? 1u : 0u;
                color = base_color >> dim;
                vertex.vx = r_s16(point);
                vertex.vy = r_s16(point + 2u);
                vertex.vz = r_s16(point + 4u);
                if (phase == 1u)
                    vertex.vx = (sint16)(0u - (uint32)(sint32)vertex.vx);
                else
                    vertex.vy = (sint16)(0u - (uint32)(sint32)vertex.vy);
                gte_project_full_depth(&vertex, &screen, NULL);
                packet = r_u32(0x80040690u);
                w_u32(packet + 4u, color);
                w_u8(packet + 7u, 0x6Au);
                w_u8(packet + 3u, 2u);
                w_u32(packet + 8u, (uint32)screen);
                AddPrim(psx_addr(slot, 4u), psx_addr(packet, 4u));
                value = r_u32(0x80040690u);
                checked = (sint64)(sint32)value + 12;
                if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                    xport_mips_overflow_exception(phase == 1u ? 0x800CF4B0u : 0x800CF5D8u);
                w_u32(0x80040690u, (uint32)checked);
                point = r_u32(point + 8u);
            } while (point != 0u);
        }
next_overlay_bin:
        for (phase = 0u; phase < 3u; ++phase) {
            checked = (sint64)(sint32)heads[phase] + 4;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CF5F4u + phase * 4u);
            heads[phase] = (uint32)checked;
        }
        checked = (sint64)(sint32)bin + 1;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CF600u);
        bin = (uint32)checked;
        checked = (sint64)(sint32)bin - (sint32)count;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CF604u);
    } while (checked < 0);
    *result = 0u;
}


void game_separate_object_overlaps(uint32 object)
{
    VECTOR direction;
    uint32 identifiers[3], list, other, model, table, scale, radius, other_radius, total;
    uint32 value, component, x, y, z, distance, numerator, strength, product, dx, dy, dz;
    sint32 denominator;
    FUNCTION_MARKER(0x800CB408, "GAME.OVL");
    identifiers[0] = r_u32(0x80075F74u);
    identifiers[1] = r_u32(0x80075F78u);
    identifiers[2] = r_u32(0x80075F7Cu);
    model = r_u32(object + 0x74u);
    table = r_u32(0x80046178u);
    scale = r_u32(object + 0x84u);
    radius = r_u32(table + model * 132u + 0x6Cu);
    if ((sint32)scale >= 0)
        radius <<= scale & 31u;
    else
        radius = (uint32)((sint32)radius >> ((0u - scale) & 31u));
    for (list = 0u; list < 3u; ++list) {
        other = object_lookup(identifiers[list]);
        while (other != 0u) {
            if ((r_u32(other + 0x114u) & 0x8090u) != 0x10u || other == object)
                goto next_overlapping_object;
            model = r_u32(other + 0x74u);
            table = r_u32(0x80046178u);
            scale = r_u32(other + 0x84u);
            other_radius = r_u32(table + model * 132u + 0x6Cu);
            if ((sint32)scale >= 0)
                other_radius <<= scale & 31u;
            else
                other_radius = (uint32)((sint32)other_radius >> ((0u - scale) & 31u));
            value = r_u32(object + 8u);
            component = r_u32(other + 8u);
            x = value - component;
            value = r_u32(object + 12u);
            component = r_u32(other + 12u);
            y = value - component;
            component = r_u32(other + 16u);
            value = r_u32(object + 16u);
            z = value - component;
            distance = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
            total = radius + other_radius;
            if ((sint32)distance >= (sint32)total)
                goto next_overlapping_object;
            value = r_u32(object + 8u);
            component = r_u32(other + 8u);
            direction.vx = (sint32)(value - component);
            value = r_u32(object + 12u);
            component = r_u32(other + 12u);
            direction.vy = (sint32)(value - component);
            value = r_u32(object + 16u);
            component = r_u32(other + 16u);
            direction.vz = (sint32)(value - component);
            vector_normalize(&direction, &direction);
            if (distance == 0u)
                distance = 1u;
            numerator = distance << 8;
            denominator = (sint32)total;
            if (denominator == 0)
                xport_mips_break(7u);
            if (numerator == 0x80000000u && denominator == -1)
                xport_mips_break(0x5Du);
            strength = 256u - (uint32)((sint32)numerator / denominator);
            product = (uint32)direction.vx * strength;
            dx = (uint32)((sint32)product >> 13);
            product = (uint32)direction.vy * strength;
            dy = (uint32)((sint32)product >> 13);
            product = (uint32)direction.vz * strength;
            dz = (uint32)((sint32)product >> 13);
            value = r_u32(object + 8u);
            w_u32(object + 8u, value + dx);
            value = r_u32(object + 12u);
            w_u32(object + 12u, value + dy);
            value = r_u32(object + 16u);
            w_u32(object + 16u, value + dz);
next_overlapping_object:
            other = r_u32(other + 0x1DCu);
        }
    }
}


void game_draw_camera_streak_layer_a(uint32 mode, uint32 particles, uint32 camera, sint32 intensity)
{
    SVECTOR vertex;
    sint32 screen0, screen1, flags;
    sint64 checked;
    uint32 velocity[3], axis, index, packet, depth, value, product, color, base, slot, offset;
    FUNCTION_MARKER(0x800CBEE0, "GAME.OVL");
    (void)r_u32(0x80040690u);
    (void)r_u32(0x800421ACu);
    packet = r_u32(0x80040690u);
    for (axis = 0u; axis < 3u; ++axis)
        velocity[axis] = r_u32(camera + 0x14u + axis * 4u);
    for (axis = 0u; axis < 3u; ++axis)
        velocity[axis] = (uint32)((sint32)velocity[axis] >> 6);
    for (index = 0u; index < 345u; ++index) {
        vertex.vx = r_s16(particles);
        vertex.vy = r_s16(particles + 2u);
        vertex.vz = r_s16(particles + 4u);
        depth = (uint32)gte_project_full_depth(&vertex, &screen0, &flags);
        if (((uint32)flags & 0x80460000u) != 0u || (sint32)depth <= 0)
            goto next_streak_a;
        vertex.vx = r_s16(particles);
        vertex.vy = r_s16(particles + 2u);
        vertex.vz = r_s16(particles + 4u);
        vertex.vx = (sint16)((uint32)(sint32)vertex.vx + velocity[0]);
        vertex.vy = (sint16)((uint32)(sint32)vertex.vy + velocity[1]);
        vertex.vz = (sint16)((uint32)(sint32)vertex.vz + velocity[2]);
        gte_project_full_depth(&vertex, &screen1, NULL);
        value = (185u - (depth >> 7)) & 255u;
        product = value * (uint32)intensity;
        color = (uint32)((sint32)product >> 8);
        color |= color << 8;
        color |= color << 8;
        checked = (sint64)(sint32)color + 0x52000000;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CC058u);
        color = (uint32)checked;
        (void)r_u32(particles + 8u);
        w_u32(packet, 0x04000000u);
        w_u32(packet + 4u, color);
        w_u32(packet + 8u, (uint32)screen0);
        w_u32(packet + 12u, 0u);
        w_u32(packet + 16u, (uint32)screen1);
        base = r_u32(0x800421ACu);
        if ((sint32)depth < 0x4000) {
            offset = ((depth >> 6) + 64u) * 4u;
            checked = (sint64)(sint32)offset + (sint32)base;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CC0D8u);
        } else {
            offset = ((depth >> 11) + 320u) * 4u;
            checked = (sint64)(sint32)offset + (sint32)base;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CC0B4u);
        }
        slot = (uint32)checked;
        AddPrim(psx_addr(slot, 4u), psx_addr(packet, 4u));
        packet = (packet & 0xFFFFFFu) + 20u;
next_streak_a:
        checked = (sint64)(sint32)particles + 20;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CC118u);
        particles = (uint32)checked;
    }
    w_u32(0x80040690u, packet);
}


void game_draw_mesh_effects_from_camera(uint32 effect, uint32 camera)
{
    SVECTOR vertex;
    sint32 screen, flags, u, v, palette_x, palette_y, width, height, frame, duration;
    sint64 checked;
    uint32 camera_position[3], position[3], relative[3], magnitude[3], axis, shifts, depth, value;
    uint32 texture, offset, palette, page, u0, v0, u1, v1, product, scale, radius, x, y;
    uint32 left, right, top, bottom, packet, color, base, slot, ordering_depth, count;
    FUNCTION_MARKER(0x800D1240, "GAME.OVL");
    if (effect == 0u)
        return;
    for (axis = 0u; axis < 3u; ++axis)
        camera_position[axis] = r_u32(camera + 8u + axis * 4u);
    do {
        for (axis = 0u; axis < 3u; ++axis)
            position[axis] = r_u32(effect + axis * 4u);
        for (axis = 0u; axis < 3u; ++axis) {
            checked = (sint64)(sint32)position[axis] - (sint32)camera_position[axis];
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800D1298u + axis * 4u);
            relative[axis] = (uint32)checked;
        }
        for (axis = 0u; axis < 3u; ++axis) {
            if (relative[axis] == 0x80000000u)
                xport_mips_overflow_exception(0x800D12B8u + axis * 12u);
            magnitude[axis] = (sint32)relative[axis] > 0 ? relative[axis] : 0u - relative[axis];
        }
        if (magnitude[0] > 100000u || magnitude[1] > 100000u || magnitude[2] > 100000u)
            goto age_mesh_effect;
        shifts = 0u;
        while (magnitude[0] > 16383u || magnitude[1] > 16383u || magnitude[2] >= 16383u) {
            magnitude[0] >>= 1;
            magnitude[1] >>= 1;
            magnitude[2] >>= 1;
            ++shifts;
        }
        vertex.vx = (sint16)(relative[0] >> (shifts & 31u));
        vertex.vy = (sint16)(relative[1] >> (shifts & 31u));
        vertex.vz = (sint16)(relative[2] >> (shifts & 31u));
        depth = (uint32)gte_project_full_depth(&vertex, &screen, &flags);
        if (((uint32)flags & 0x80460000u) != 0u)
            goto age_mesh_effect;
        depth <<= shifts & 31u;
        if (depth < 10u)
            goto age_mesh_effect;
        value = r_u16(effect + 0x26u);
        offset = r_u32(effect + 12u);
        checked = (sint64)(sint32)value + (sint32)offset;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800D13A8u);
        offset = (uint32)checked << 4;
        checked = (sint64)(sint32)0x8004EB70u + (sint32)offset;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800D13B0u);
        texture = (uint32)checked;
        u = r_s16(texture);
        v = r_s16(texture + 2u);
        palette_y = r_s16(texture + 6u);
        palette_x = r_s16(texture + 4u);
        palette = ((uint32)palette_y << 6) | ((uint32)(palette_x >> 4) & 63u);
        page = (v > 255 ? 48u : 32u) + ((uint32)u >> 6);
        width = r_s16(texture + 8u);
        height = r_s16(texture + 10u);
        u0 = ((uint32)u & 63u) << 2;
        v0 = (uint32)v & 255u;
        u1 = ((uint32)width << 2) + u0 - 1u;
        v1 = v0 + (uint32)height - 1u;
        scale = r_u16(effect + 0x2Cu);
        product = scale * (uint32)width;
        scale = r_u16(effect + 0x24u);
        product *= scale;
        radius = (product >> 2) / depth;
        if (radius > 48u)
            radius = 48u;
        x = (uint32)(sint32)(sint16)screen;
        y = (uint32)(sint32)(sint16)((uint32)screen >> 16);
        left = x - radius;
        right = x + radius;
        top = y - radius;
        bottom = y + radius;
        packet = r_u32(0x80040690u);
        w_u16(packet + 8u, (uint16)left);
        w_u16(packet + 10u, (uint16)top);
        w_u16(packet + 16u, (uint16)right);
        w_u16(packet + 18u, (uint16)top);
        w_u16(packet + 24u, (uint16)left);
        w_u16(packet + 26u, (uint16)bottom);
        w_u16(packet + 32u, (uint16)right);
        w_u16(packet + 34u, (uint16)bottom);
        w_u8(packet + 12u, (uint8)u0);
        w_u8(packet + 13u, (uint8)v0);
        w_u8(packet + 20u, (uint8)u1);
        w_u8(packet + 21u, (uint8)v0);
        w_u8(packet + 28u, (uint8)u0);
        w_u8(packet + 29u, (uint8)v1);
        w_u8(packet + 36u, (uint8)u1);
        w_u8(packet + 37u, (uint8)v1);
        color = r_u32(effect + 0x20u);
        w_u32(packet + 4u, color);
        w_u8(packet + 7u, 0x2Eu);
        w_u8(packet + 3u, 9u);
        w_u16(packet + 14u, (uint16)palette);
        w_u16(packet + 22u, (uint16)page);
        ordering_depth = depth >= 0x4000u ? depth - 0x4000u : 0u;
        checked = (sint64)(sint32)packet + 40;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800D1508u);
        w_u32(0x80040690u, (uint32)checked);
        base = r_u32(0x800421ACu);
        if (ordering_depth < 0x4000u) {
            offset = ((ordering_depth >> 6) + 64u) * 4u;
            checked = (sint64)(sint32)offset + (sint32)base;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800D1578u);
        } else {
            offset = ((ordering_depth >> 11) + 320u) * 4u;
            checked = (sint64)(sint32)offset + (sint32)base;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800D1554u);
        }
        slot = (uint32)checked;
        AddPrim(psx_addr(slot, 4u), psx_addr(packet, 4u));
age_mesh_effect:
        frame = r_s16(effect + 0x26u);
        duration = r_s16(effect + 0x28u);
        ++frame;
        w_u16(effect + 0x26u, (uint16)frame);
        if (duration <= frame) {
            count = r_u32(0x80109B38u);
            offset = count << 2;
            checked = (sint64)(sint32)0x800FB440u + (sint32)offset;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800D15E8u);
            w_u32((uint32)checked, effect);
            w_u16(0x80109B38u, (uint16)(count + 1u));
        }
        effect = r_u32(effect + 0x18u);
    } while (effect != 0u);
}


void game_draw_descriptor_chain(uint32 descriptors, uint32 camera, uint32 frozen)
{
    SVECTOR vertex;
    sint32 type, initialized, screen, flags;
    sint64 checked;
    uint32 position[3], velocity[3], relative[3], record, index, axis, lifetime, before_shift, color;
    uint32 packet, previous, depth, base, offset, slot, value;
    FUNCTION_MARKER(0x800CB68C, "GAME.OVL");
    packet = r_u32(0x80040690u);
    record = descriptors;
    for (index = 0u; index < 449u; ++index) {
        type = r_s8(record);
        if (type == 0)
            goto next_descriptor;
        for (axis = 0u; axis < 3u; ++axis)
            velocity[axis] = (uint32)r_s16(record + 2u + axis * 2u);
        for (axis = 0u; axis < 3u; ++axis)
            position[axis] = r_u32(record + 8u + axis * 4u);
        lifetime = r_u32(record + 0x18u);
        if ((sint32)frozen <= 0) {
            for (axis = 0u; axis < 3u; ++axis) {
                checked = (sint64)(sint32)position[axis] + (sint32)velocity[axis];
                if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                    xport_mips_overflow_exception(0x800CB70Cu + axis * 4u);
                position[axis] = (uint32)checked;
            }
            for (axis = 0u; axis < 3u; ++axis)
                w_u32(record + 8u + axis * 4u, position[axis]);
            checked = (sint64)(sint32)lifetime - 1;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CB724u);
            lifetime = (uint32)checked;
            w_u32(record + 0x18u, lifetime);
            if ((sint32)lifetime <= 0)
                goto clear_descriptor;
        }
        checked = (sint64)(sint32)lifetime - 1;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CB73Cu);
        before_shift = (uint32)checked;
        color = before_shift << 2;
        if ((sint32)before_shift > 0) {
            checked = (sint64)(sint32)color - 255;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CB748u);
        }
        if ((sint32)before_shift <= 0 || checked > 0) {
            color = type == 1 ? 0u : 255u;
        }
        if (type == 1) {
            w_u8(packet + 6u, (uint8)color);
            color >>= 1;
            w_u8(packet + 5u, (uint8)color);
            w_u8(packet + 4u, (uint8)color);
            w_u8(packet + 14u, (uint8)color);
            color >>= 1;
            w_u8(packet + 13u, (uint8)color);
            w_u8(packet + 12u, (uint8)color);
        } else if (type == 2) {
            w_u8(packet + 4u, (uint8)color);
            w_u8(packet + 5u, (uint8)color);
            w_u8(packet + 6u, 0u);
            w_u8(packet + 12u, (uint8)color);
            w_u8(packet + 13u, 0u);
            w_u8(packet + 14u, 0u);
        } else if (type == 3) {
            color >>= 1;
            w_u8(packet + 6u, (uint8)color);
            color >>= 1;
            w_u8(packet + 5u, (uint8)color);
            w_u8(packet + 4u, (uint8)color);
            w_u8(packet + 14u, (uint8)color);
            color >>= 1;
            w_u8(packet + 13u, (uint8)color);
            w_u8(packet + 12u, (uint8)color);
        }
        w_u8(packet + 7u, 0x52u);
        w_u8(packet + 3u, 4u);
        for (axis = 0u; axis < 3u; ++axis) {
            value = r_u32(camera + 8u + axis * 4u);
            checked = (sint64)(sint32)position[axis] - (sint32)value;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CB810u + axis * 12u);
            relative[axis] = (uint32)((sint32)(uint32)checked >> 4);
        }
        for (axis = 0u; axis < 3u; ++axis) {
            if ((sint32)relative[axis] < -32768 || (sint32)relative[axis] > 32767)
                goto clear_descriptor;
        }
        vertex.vx = (sint16)relative[0];
        vertex.vy = (sint16)relative[1];
        vertex.vz = (sint16)relative[2];
        depth = (uint32)gte_project_full_depth(&vertex, &screen, &flags);
        previous = r_u32(record + 0x14u);
        w_u32(packet + 16u, previous);
        w_u32(packet + 8u, (uint32)screen);
        w_u32(record + 0x14u, (uint32)screen);
        if (((uint32)flags & 0x80460000u) != 0u)
            goto clear_descriptor;
        initialized = r_s8(record + 1u);
        w_u8(record + 1u, 1u);
        if (initialized == 0)
            goto next_descriptor;
        depth <<= 4;
        base = r_u32(0x800421ACu);
        if (depth < 0x4000u) {
            offset = ((depth >> 6) + 64u) * 4u;
            checked = (sint64)(sint32)offset + (sint32)base;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CB958u);
        } else {
            offset = ((depth >> 11) + 320u) * 4u;
            checked = (sint64)(sint32)offset + (sint32)base;
            if (checked < -2147483647LL - 1 || checked > 2147483647LL)
                xport_mips_overflow_exception(0x800CB934u);
        }
        slot = (uint32)checked;
        AddPrim(psx_addr(slot, 4u), psx_addr(packet, 4u));
        checked = (sint64)(sint32)packet + 20;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CB998u);
        packet = (uint32)checked;
        w_u32(0x80040690u, packet);
        goto next_descriptor;
clear_descriptor:
        w_u16(record, 0u);
        w_u8(record + 1u, 0u);
next_descriptor:
        checked = (sint64)(sint32)record + 28;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CB9B4u);
        record = (uint32)checked;
    }
}


void game_draw_camera_streak_layer_b(uint32 mode, uint32 particles, uint32 camera, sint32 intensity)
{
    SVECTOR vertex;
    sint32 screen0, screen1, flags;
    sint64 checked;
    uint32 velocity[3], axis, index, packet, depth, value, product, color, base, slot;
    FUNCTION_MARKER(0x800CC168, "GAME.OVL");
    (void)r_u32(0x80040690u);
    (void)r_u32(0x800421ACu);
    packet = r_u32(0x80040690u);
    base = r_u32(0x800421ACu);
    for (axis = 0u; axis < 3u; ++axis)
        velocity[axis] = r_u32(camera + 0x14u + axis * 4u);
    for (axis = 0u; axis < 3u; ++axis) {
        if (velocity[axis] == 0x80000000u)
            xport_mips_overflow_exception(0x800CC1F4u + axis * 4u);
        velocity[axis] = 0u - velocity[axis];
    }
    for (axis = 0u; axis < 3u; ++axis)
        velocity[axis] = (uint32)((sint32)velocity[axis] >> 7);
    for (index = 0u; index < 345u; ++index) {
        vertex.vx = r_s16(particles);
        vertex.vy = r_s16(particles + 2u);
        vertex.vz = r_s16(particles + 4u);
        depth = (uint32)gte_project_full_depth(&vertex, &screen0, &flags);
        if (((uint32)flags & 0x80460000u) != 0u || (sint32)depth <= 0)
            goto next_streak_b;
        vertex.vx = r_s16(particles);
        vertex.vy = r_s16(particles + 2u);
        vertex.vz = r_s16(particles + 4u);
        vertex.vx = (sint16)((uint32)(sint32)vertex.vx + velocity[0]);
        vertex.vy = (sint16)((uint32)(sint32)vertex.vy + velocity[1]);
        vertex.vz = (sint16)((uint32)(sint32)vertex.vz + velocity[2]);
        gte_project_full_depth(&vertex, &screen1, NULL);
        value = 185u - (depth >> 7);
        if ((sint32)value > 255)
            value = 255u;
        if ((sint32)value < 0)
            value = 0u;
        product = (value & 255u) * (uint32)intensity;
        color = (uint32)((sint32)product >> 8);
        color |= color << 8;
        color |= color << 8;
        checked = (sint64)(sint32)color + 0x52000000;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CC30Cu);
        color = (uint32)checked;
        (void)r_u32(particles + 8u);
        w_u32(packet, 0x04000000u);
        w_u32(packet + 4u, color);
        w_u32(packet + 8u, (uint32)screen0);
        w_u32(packet + 12u, 0u);
        w_u32(packet + 16u, (uint32)screen1);
        slot = base + 0x3E80u;
        AddPrim(psx_addr(slot, 4u), psx_addr(packet, 4u));
        packet = (packet & 0xFFFFFFu) + 20u;
next_streak_b:
        checked = (sint64)(sint32)particles + 20;
        if (checked < -2147483647LL - 1 || checked > 2147483647LL)
            xport_mips_overflow_exception(0x800CC36Cu);
        particles = (uint32)checked;
    }
    w_u32(0x80040690u, packet);
}
