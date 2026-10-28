#include "game_functions.h"
#include <stdio.h>

void game_finish_demo_input(void)
{
    FUNCTION_MARKER(0x8009A534, "GAME.OVL");
    // Original routine only returns
    return;
}

void game_reset_demo_selection(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x8009AAA8, "GAME.OVL");
    flags = r_u32(0x80031138u);
    if ((flags & 3u) == 0u)
        return;
    flags = r_u32(0x80031138u);
    if ((flags & 1u) != 0u)
        game_finish_demo_input();
    if ((flags & 3u) != 0u)
        heap_free(r_u32(0x8003112Cu));
    w_u16(0x800595FAu, 20u);
    w_u16(0x800595F8u, 20u);
    w_u16(0x800595FEu, 1u);
    w_u16(0x800595FCu, 1u);
    sprintf((char *)psx_addr(0x80010518u, 7u), "1_1.CW");
}

void game_free_optional_scene_buffer(void)
{
    FUNCTION_MARKER(0x800911DC, "GAME.OVL");
    if (r_u32(0x800ED3A8u) != 0u)
        heap_free(r_u32(0x80123F60u));
}

void game_free_scene_buffer_banks(void)
{
    uint32 bank, index;
    FUNCTION_MARKER(0x800E2DD0, "GAME.OVL");
    for (bank = 0u; bank < 2u; ++bank) {
        for (index = 0u; index < 5u; ++index)
            heap_free(r_u32(0x801193C8u + bank * 20u + index * 4u));
    }
}

void game_free_scene_buffers(void)
{
    FUNCTION_MARKER(0x800E2E48, "GAME.OVL");
    game_free_optional_scene_buffer();
    game_free_scene_buffer_banks();
}

void game_free_special_mode_buffers(void)
{
    FUNCTION_MARKER(0x800EA598, "GAME.OVL");
    if (r_u32(0x80031134u) == 0u)
        return;
    heap_free(r_u32(0x8011EFE8u));
    heap_free(r_u32(0x8011A3B4u));
    heap_free(r_u32(0x80118A84u));
}

void game_stop_auxiliary_scene_voices(void)
{
    FUNCTION_MARKER(0x800DFFA0, "GAME.OVL");
    sound_stop_voice(r_u32(0x800F8100u));
    sound_tick();
    sound_stop_voice(r_u32(0x800FB438u));
    sound_tick();
}

void game_stop_scene_voices(void)
{
    FUNCTION_MARKER(0x800E1D64, "GAME.OVL");
    sound_stop_voice(r_u32(0x800FD720u));
    sound_stop_voice(r_u32(0x80108A28u));
    sound_tick();
    sound_stop_voice(r_u32(0x800F4E98u));
    sound_tick();
    sound_stop_voice(r_u32(0x800F4E9Cu));
    sound_tick();
    sound_stop_voice(r_u32(0x801193F0u));
    sound_tick();
    sound_stop_voice(r_u32(0x801193F4u));
    sound_tick();
    sound_stop_voice(r_u32(0x800FD4F8u));
    sound_tick();
    sound_stop_voice(r_u32(0x800F3D14u));
    sound_tick();
    game_stop_auxiliary_scene_voices();
    sound_reset_voices();
}

void game_free_scene_models(void)
{
    uint32 index, offset, table, slot, mesh, fragment, fragment_offset, header, allocation;
    sint32 count, fragments;
    FUNCTION_MARKER(0x8008314C, "GAME.OVL");
    count = r_s32(0x800ECEB8u);
    index = 0u;
    offset = 0u;
    if (count > 0) {
        do {
            table = r_u32(0x80046178u);
            slot = table + offset;
            if (r_u32(slot + 0x74u) == 0u) {
                heap_free(r_u32(slot) - 12u);
            } else {
                mesh = r_u32(slot);
                resource_free_bundle(mesh);
                table = r_u32(0x80046178u);
                fragments = r_s32(table + offset + 0x70u);
                fragment = 0u;
                if (fragments > 0) {
                    do {
                        fragment_offset = fragment << 2;
                        mesh = r_u32(table + offset + fragment_offset + 12u);
                        ++fragment;
                        resource_free_bundle(mesh);
                        table = r_u32(0x80046178u);
                        header = r_u32(table + offset + fragment_offset + 0x3Cu);
                        heap_free(r_u32(header + 8u));
                        table = r_u32(0x80046178u);
                        header = r_u32(table + offset + fragment_offset + 0x3Cu);
                        heap_free(header);
                        table = r_u32(0x80046178u);
                        fragments = r_s32(table + offset + 0x70u);
                    } while ((sint32)fragment < fragments);
                }
            }
            table = r_u32(0x80046178u);
            slot = table + offset;
            if (r_s16(slot + 0x80u) != 0) {
                allocation = r_u32(slot + 4u);
                if (allocation != 0u)
                    heap_free(allocation);
            }
            table = r_u32(0x80046178u);
            allocation = r_u32(table + offset + 8u);
            if (allocation != 0u)
                heap_free(allocation);
            count = r_s32(0x800ECEB8u);
            ++index;
            offset += 132u;
        } while ((sint32)index < count);
    }
    table = r_u32(0x80046178u);
    w_u32(0x800ECEB8u, 0u);
    heap_free(table);
}

void game_shutdown_scene(void)
{
    uint32 chapter, mission;
    sint32 mode;
    FUNCTION_MARKER(0x80082FE8, "GAME.OVL");
    game_stop_scene_voices();
    game_free_scene_buffers();
    game_free_effect_payloads();
    game_reset_demo_selection();
    object_update_all();
    object_initialize_pool();
    game_free_special_mode_buffers();
    game_stop_scene_stream();
    game_free_scene_models();
    render_reset();
    if (r_s16(0x800595FCu) == 0) {
        chapter = r_u16(0x80059600u);
        mission = r_u16(0x80059602u);
        w_u16(0x800595FCu, (uint16)chapter);
        w_u16(0x800595FEu, (uint16)mission);
    }
    if (r_s16(0x800595F8u) == 1) {
        mode = r_s16(0x80059610u);
        if (mode == 1 || mode == 3)
            w_u16(0x80011902u, (uint16)(r_u16(0x80011902u) + 1u));
    }
}

void game_emit_drifting_particles(sint32 count, uint32 x, uint32 y, uint32 z)
{
    uint32 index, base, offset, record, state, first, second, third, next;
    FUNCTION_MARKER(0x800AF848, "GAME.OVL");
    while (count > 0) {
        --count;
        index = r_u32(0x800EF10Cu);
        base = r_u32(0x80123F64u);
        state = r_u32(0x800EF50Cu);
        offset = index * 28u;
        record = base + offset;
        first = (uint32)((sint32)state >> 1) ^ (state << 3);
        next = index + 1u;
        w_u32(0x800EF50Cu, first);
        second = (uint32)((sint32)first >> 1) ^ (first << 3);
        w_u16(record + 2u, (uint16)((first & 127u) - 64u));
        third = (uint32)((sint32)second >> 1) ^ (second << 3);
        w_u32(0x800EF50Cu, second);
        w_u16(record + 4u, (uint16)((second & 127u) - 64u));
        w_u32(0x800EF50Cu, third);
        next = (uint32)((sint32)next % 450);
        w_u32(0x800EF10Cu, next);
        w_u8(record, 3u);
        base = r_u32(0x80123F64u);
        w_u32(record + 8u, x);
        w_u32(record + 12u, y);
        w_u32(record + 16u, z);
        w_u16(record + 6u, (uint16)((third & 127u) - 64u));
        w_u32(record + 24u, 64u);
        w_u8(base + offset + 1u, 0u);
    }
}

void game_update_line_effects(uint32 effect)
{
    SVECTOR vertices[3];
    sint32 screen[3], depth[3], flags;
    uint32 x, y, z, vx, vy, vz, camera, rx, ry, rz, ex, ey, ez;
    uint32 ax, ay, az, shifts, packet, color, ordering_depth, lifetime;
    FUNCTION_MARKER(0x800B03E8, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    while (effect != 0u) {
        if (r_s16(effect + 0x3Cu) != 0) {
            x = r_u32(effect);
            vx = r_u32(effect + 0x20u);
            vy = r_u32(effect + 0x24u);
            vz = r_u32(effect + 0x28u);
            w_u32(effect, x + vx);
            y = r_u32(effect + 4u);
            z = r_u32(effect + 8u);
            camera = r_u32(0x80040C94u);
            x = r_u32(effect);
            w_u32(effect + 4u, y + vy);
            w_u32(effect + 8u, z + vz);
            rx = x - r_u32(camera + 8u);
            y = r_u32(effect + 4u);
            ry = y - r_u32(camera + 12u);
            z = r_u32(effect + 8u);
            rz = z - r_u32(camera + 16u);
            ex = rx - (uint32)(sint32)r_s16(effect + 0x36u);
            ey = ry - (uint32)(sint32)r_s16(effect + 0x38u);
            ez = rz - (uint32)(sint32)r_s16(effect + 0x3Au);
            ax = (sint32)rx < 0 ? 0u - rx : rx;
            ay = (sint32)ry < 0 ? 0u - ry : ry;
            az = (sint32)rz < 0 ? 0u - rz : rz;
            if ((sint32)ax <= 100000 && (sint32)ay <= 100000 && (sint32)az <= 100000) {
                shifts = 0u;
                while ((sint32)ax >= 16385 || (sint32)ay >= 16385 || (sint32)az >= 16385) {
                    ++shifts;
                    rx = (uint32)((sint32)rx >> 1);
                    ry = (uint32)((sint32)ry >> 1);
                    rz = (uint32)((sint32)rz >> 1);
                    ex = (uint32)((sint32)ex >> 1);
                    ey = (uint32)((sint32)ey >> 1);
                    ez = (uint32)((sint32)ez >> 1);
                    ax = (sint32)rx < 0 ? 0u - rx : rx;
                    ay = (sint32)ry < 0 ? 0u - ry : ry;
                    az = (sint32)rz < 0 ? 0u - rz : rz;
                }
                vertices[0].vx = (sint16)rx;
                vertices[0].vy = (sint16)ry;
                vertices[0].vz = (sint16)rz;
                vertices[1].vx = (sint16)ex;
                vertices[1].vy = (sint16)ey;
                vertices[1].vz = (sint16)ez;
                vertices[2].vx = (sint16)ex;
                vertices[2].vy = (sint16)ey;
                vertices[2].vz = (sint16)ez;
                gte_project3_full_depth(vertices, screen, depth, &flags);
                ordering_depth = (uint32)(depth[2] >> 2) << ((shifts + 2u) & 31u);
                if (((uint32)flags & 0xFFFF0000u) == 0u) {
                    packet = r_u32(0x80040690u);
                    color = r_u32(effect + 12u);
                    w_u32(0x80040690u, packet + 20u);
                    w_u32(packet + 4u, color);
                    w_u32(packet + 12u, 0u);
                    w_u32(packet + 8u, (uint32)screen[0]);
                    w_u32(packet + 16u, (uint32)screen[2]);
                    setLineG2(psx_addr(packet, 20u));
                    setSemiTrans(psx_addr(packet, 20u), 1);
                    render_insert_primitive((sint32)ordering_depth, packet);
                }
                if ((r_u32(0x800311B8u) & 1u) != 0u) {
                    x = r_u32(effect);
                    y = r_u32(effect + 4u);
                    z = r_u32(effect + 8u);
                    game_emit_drifting_particles(1, x, y, z);
                }
            }
        }
        lifetime = r_u32(effect + 0x2Cu) - 1u;
        w_u32(effect + 0x2Cu, lifetime);
        if ((sint32)lifetime <= 0)
            game_mark_effect_for_removal(effect);
        effect = r_u32(effect + 0x50u);
    }
}

void game_update_particle_effects(uint32 effect)
{
    VECTOR position, direction;
    uint32 first, second, state, rng_x, rng_y, rng_z, x, y, z, vx, vy, vz, lifetime;
    FUNCTION_MARKER(0x800B0748, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    while (effect != 0u) {
        if (r_s16(effect + 0x3Cu) != 0) {
            first = r_u32(effect + 12u);
            second = r_u32(effect + 16u);
            w_u16(effect + 0x42u, 1u);
            x = r_u32(effect);
            vy = r_u32(effect + 0x24u);
            w_u32(effect + 16u, first);
            vx = r_u32(effect + 0x20u);
            w_u32(effect + 12u, second);
            y = r_u32(effect + 4u);
            w_u32(effect, x + vx);
            z = r_u32(effect + 8u);
            w_u32(effect + 4u, y + vy);
            vz = r_u32(effect + 0x28u);
            position.vx = r_s32(effect);
            w_u32(effect + 8u, z + vz);
            state = r_u32(0x800EF50Cu);
            position.vy = r_s32(effect + 4u);
            rng_x = (uint32)((sint32)state >> 1) ^ (state << 3);
            rng_y = (uint32)((sint32)rng_x >> 1) ^ (rng_x << 3);
            rng_z = (uint32)((sint32)rng_y >> 1) ^ (rng_y << 3);
            position.vz = r_s32(effect + 8u);
            vx = r_u32(effect + 0x20u);
            w_u32(0x800EF50Cu, rng_x);
            w_u32(0x800EF50Cu, rng_y);
            w_u32(0x800EF50Cu, rng_z);
            direction.vx = (sint32)((rng_x & 63u) - 31u + (uint32)((sint32)vx >> 2));
            vy = r_u32(effect + 0x24u);
            direction.vy = (sint32)((rng_y & 63u) - 31u + (uint32)((sint32)vy >> 2));
            vz = r_u32(effect + 0x28u);
            direction.vz = (sint32)((rng_z & 63u) - 31u + (uint32)((sint32)vz >> 2));
            game_emit_radial_effect((uint32)position.vx, (uint32)position.vy, (uint32)position.vz, 256u, 0x402450u);
            if ((r_u32(0x800311B8u) & 1u) != 0u)
                game_emit_vector_effect_at_native(&position, &direction, 0x400030u, 512u);
        }
        lifetime = r_u32(effect + 0x2Cu) - 1u;
        w_u32(effect + 0x2Cu, lifetime);
        if ((sint32)lifetime <= 0)
            game_mark_effect_for_removal(effect);
        effect = r_u32(effect + 0x50u);
    }
}

void game_draw_camera_effects(uint32 effect)
{
    uint32 camera;
    FUNCTION_MARKER(0x800B0928, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    camera = r_u32(0x80040C94u);
    game_update_and_draw_streak_list(effect, camera);
}

void game_collide_scene_effects(void)
{
    static const uint32 groups[4] = {3u, 1u, 6u, 7u};
    uint32 group, object, effect, head;
    FUNCTION_MARKER(0x800B2194, "GAME.OVL");
    for (group = 0u; group < 4u; ++group) {
        object = object_lookup(groups[group]);
        while (object != 0u) {
            head = r_u32(0x80124928u);
            game_collide_effects_with_object(head, object);
            object = r_u32(object + 0x1DCu);
        }
    }
    effect = r_u32(0x800F337Cu);
    while (effect != 0u) {
        if ((r_u16(effect + 0x46u) & 1u) != 0u) {
            head = r_u32(0x800F3384u);
            game_collide_effect_chains(effect, head);
        }
        effect = r_u32(effect + 0x50u);
    }
}

void game_update_scene_effects(void)
{
    uint32 head;
    FUNCTION_MARKER(0x800B20E8, "GAME.OVL");
    head = r_u32(0x800F3374u);
    w_u32(0x800EF114u, 0u);
    game_update_line_effects(head);
    head = r_u32(0x800F3378u);
    game_draw_camera_effects(head);
    head = r_u32(0x800F3384u);
    game_draw_camera_effects(head);
    head = r_u32(0x800F3380u);
    game_update_particle_effects(head);
    if (r_u32(0x80031134u) == 0u)
        game_collide_scene_effects();
}

uint32 game_find_nearest_object(uint32 object, uint32 filter, uint32 group, uint32 excluded_flags)
{
    uint32 candidate, best, flags, x, y, z, px, py, pz;
    sint32 minimum, distance;
    FUNCTION_MARKER(0x800AE50C, "GAME.OVL");
    minimum = 0x07FFFFFF;
    best = 0u;
    candidate = object_lookup(group);
    while (candidate != 0u) {
        flags = r_u32(candidate + 0x114u);
        if (((flags & 16u) != 0u && filter == 0u) || ((flags & 16u) == 0u && filter == 1u)) {
            x = r_u32(candidate + 8u);
            px = r_u32(object + 8u);
            y = r_u32(candidate + 12u);
            py = r_u32(object + 12u);
            pz = r_u32(object + 16u);
            z = r_u32(candidate + 16u);
            distance = vector_length_approx((sint32)(x - px), (sint32)(y - py), (sint32)(pz - z));
            w_u32(0x80046174u, (uint32)distance);
            if (distance < minimum) {
                flags = r_u32(candidate + 0x114u);
                if ((flags & 0x8000u) == 0u && (flags & excluded_flags) == 0u) {
                    minimum = distance;
                    best = candidate;
                }
            }
        }
        candidate = r_u32(candidate + 0x1DCu);
    }
    return best;
}

void game_rotate_object_basis(uint32 object)
{
    MATRIX first, second;
    MATRIX *basis;
    uint32 yaw, pitch, roll;
    FUNCTION_MARKER(0x8009294C, "GAME.OVL");
    yaw = r_u32(object + 0x20u);
    if (yaw != 0u) {
        pitch = r_u32(object + 0x24u);
        if (pitch != 0u) {
            roll = r_u32(object + 0x28u);
            basis = (MATRIX *)psx_addr(object + 0x11Cu, 20u);
            matrix_rotate_y(&first, basis, 0u - yaw);
            if (roll != 0u) {
                matrix_rotate_x(&second, &first, r_u32(object + 0x24u));
                matrix_rotate_z(basis, &second, 0u - r_u32(object + 0x28u));
            } else {
                matrix_rotate_x(basis, &first, r_u32(object + 0x24u));
            }
        } else {
            roll = r_u32(object + 0x28u);
            basis = (MATRIX *)psx_addr(object + 0x11Cu, 20u);
            matrix_rotate_y(&first, basis, 0u - yaw);
            if (roll != 0u)
                matrix_rotate_z(basis, &first, 0u - r_u32(object + 0x28u));
            else
                matrix_copy_rotation_native(object + 0x11Cu, &first);
        }
    } else {
        pitch = r_u32(object + 0x24u);
        if (pitch != 0u) {
            roll = r_u32(object + 0x28u);
            basis = (MATRIX *)psx_addr(object + 0x11Cu, 20u);
            matrix_rotate_x(&first, basis, pitch);
            if (roll != 0u)
                matrix_rotate_z(basis, &first, 0u - r_u32(object + 0x28u));
            else
                matrix_copy_rotation_native(object + 0x11Cu, &first);
        } else {
            roll = r_u32(object + 0x28u);
            if (roll != 0u) {
                basis = (MATRIX *)psx_addr(object + 0x11Cu, 20u);
                matrix_rotate_z(&first, basis, 0u - roll);
                matrix_copy_rotation_native(object + 0x11Cu, &first);
            }
        }
    }
}

void game_check_object_collisions(uint32 object)
{
    uint32 kind, head;
    FUNCTION_MARKER(0x800A1680, "GAME.OVL");
    kind = r_u32(object);
    if (kind == 3u || kind == 1u) {
        head = object_lookup(kind == 3u ? 2u : 4u);
        game_process_object_list_contacts(head, object, 2000);
        head = object_lookup(5u);
        game_process_object_list_contacts(head, object, 2000);
    } else if (kind == 102u) {
        head = object_lookup(4u);
        game_process_object_list_contacts(head, object, 2000);
        head = object_lookup(2u);
        game_process_object_list_contacts(head, object, 2000);
    } else if (kind == 106u) {
        head = object_lookup(5u);
        game_process_object_list_contacts(head, object, 2000);
    }
}

void game_update_tractor_beam(uint32 object)
{
    uint32 beam, target;
    FUNCTION_MARKER(0x800A612C, "GAME.OVL");
    beam = r_u32(object + 0x208u);
    if (beam == 0u)
        diagnostic_report(r_u32(object + 4u), 0x80075D30u, 0x80075D4Cu);
    target = r_u32(beam + 0x38u);
    if (r_u32(target) == 0u)
        w_u32(object + 0x114u, r_u32(object + 0x114u) ^ 0x20000u);
    else
        game_apply_tractor_beam(target, object, beam);
}

uint32 game_update_scene_objects(void)
{
    uint32 object, value, mode, callback, x, y, z, rotated, timer, normalize;
    uint32 kind, tracking, target, distance;
    sint32 category;
    FUNCTION_MARKER(0x800AD924, "GAME.OVL");
    object = r_u32(0x8005367Cu);
    while (object != 0u) {
        value = r_u8(object + 0x26Cu);
        mode = r_u8(object + 0x270u);
        w_u8(object + 0x26Cu, (uint8)(value - 1u));
        rotated = 0u;
        if (mode == 1u)
            game_update_ship_formation(object);
        if (r_u8(object + 0x26Cu) == 0u) {
            x = r_u32(object + 8u);
            y = r_u32(object + 12u);
            z = r_u32(object + 16u);
            callback = r_u32(object + 0x40u);
            w_u32(object + 0x90u, x);
            w_u32(object + 0x94u, y);
            w_u32(object + 0x98u, z);
            psx_dispatch_guest_call(callback, 1u, object, 0u, 0u);
            value = r_u8(object + 0x26Bu);
            w_u8(object + 0x26Cu, (uint8)value);
            if ((r_u32(object + 0x70u) & 0x100000u) != 0u) {
                if (r_u32(object + 0x20u) != 0u || r_u32(object + 0x24u) != 0u || r_u32(object + 0x28u) != 0u) {
                    game_rotate_object_basis(object);
                    rotated = 1u;
                    w_u32(object + 0x28u, 0u);
                    w_u32(object + 0x24u, 0u);
                    w_u32(object + 0x20u, 0u);
                }
                timer = r_u16(object + 0x24Cu) - 1u;
                w_u16(object + 0x24Cu, (uint16)timer);
                normalize = (sint16)timer <= 0 && rotated == 1u;
                if (normalize == 0u) {
                    value = r_u32(0x800311B8u);
                    if (value < 5u && r_u32(0x800311C0u) != 0u)
                        normalize = 1u;
                }
                if (normalize != 0u) {
                    matrix_normalize(object + 0x11Cu, 1u);
                    value = (uint32)random_between(30, 40);
                    w_u16(object + 0x24Cu, (uint16)value);
                }
            } else {
                value = r_u32(object + 0x20u);
                x = r_u32(object + 0x2Cu);
                normalize = value != x;
                if (normalize == 0u) {
                    value = r_u32(object + 0x24u);
                    x = r_u32(object + 0x30u);
                    normalize = value != x;
                }
                if (normalize == 0u) {
                    value = r_u32(object + 0x28u);
                    x = r_u32(object + 0x34u);
                    normalize = value != x;
                }
                if (normalize != 0u) {
                    matrix_set_identity_native((MATRIX *)psx_addr(object + 0x11Cu, 18u));
                    game_rotate_object_basis(object);
                }
            }
        }
        if (r_s32(object + 4u) > 0 && r_u32(object) != 0u) {
            game_check_object_collisions(object);
            if ((r_u32(object + 0x114u) & 0x20000u) != 0u)
                game_update_tractor_beam(object);
            kind = r_u32(object + 4u);
            category = r_s16(0x800536BCu + kind * 60u);
            if (category == 2) {
                if ((r_u32(object + 0x114u) & 0xC000u) == 0u) {
                    callback = r_u32(object + 0x60u);
                    psx_dispatch_guest_call(callback, 1u, object, 0u, 0u);
                }
            } else if (r_u32(object) == 3u) {
                tracking = r_u32(object + 0x278u);
                if (tracking != 0u) {
                    value = r_u16(tracking + 16u) - 1u;
                    w_u16(tracking + 16u, (uint16)value);
                    tracking = r_u32(object + 0x278u);
                    if (r_s16(tracking + 16u) <= 0) {
                        value = (uint32)random_between(30, 60);
                        tracking = r_u32(object + 0x278u);
                        w_u16(tracking + 16u, (uint16)value);
                        target = game_find_nearest_object(object, 0u, 3u, 0u);
                        if (target != 0u) {
                            game_evaluate_target_occlusion(object, target, 0u);
                            distance = r_u32(0x80046174u);
                            if ((sint32)distance <= 199999) {
                                tracking = r_u32(object + 0x278u);
                                w_u32(tracking, target);
                                tracking = r_u32(object + 0x278u);
                                w_u32(tracking + 8u, distance);
                                tracking = r_u32(object + 0x278u);
                                value = r_u32(0x80033D1Cu);
                                w_u16(tracking + 12u, (uint16)value);
                                tracking = r_u32(object + 0x278u);
                                value = r_u32(0x80033D30u);
                                w_u16(tracking + 14u, (uint16)value);
                            } else {
                                tracking = r_u32(object + 0x278u);
                                w_u32(tracking, 0u);
                            }
                        } else {
                            tracking = r_u32(object + 0x278u);
                            w_u32(tracking, 0u);
                        }
                    }
                }
            }
        }
        object = r_u32(object + 0x1D4u);
    }
    return object_flush_removed();
}

void game_draw_tractor_beam_native(VECTOR *points, uint32 unused)
{
    uint32 camera, x, y, z, component;
    sint32 distance;
    FUNCTION_MARKER(0x800CA428, "GAME.OVL");
    camera = r_u32(0x80040C94u);
    x = (uint32)points[0].vx;
    component = r_u32(camera + 8u);
    x -= component;
    y = (uint32)points[0].vy;
    component = r_u32(camera + 12u);
    y -= component;
    z = (uint32)points[0].vz;
    component = r_u32(camera + 16u);
    z -= component;
    distance = vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    if (distance > 500000)
        return;
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    camera = r_u32(0x80040C94u);
    game_draw_tractor_curve_native(points, camera);
}

void game_apply_tractor_beam(uint32 target, uint32 object, uint32 beam)
{
    VECTOR points[4], offset, difference;
    uint32 player, phase, product, component, value, nx, ny, nz, numerator;
    sint32 length, amplitude, limit;
    FUNCTION_MARKER(0x800853F4, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    if (target == player)
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(target + 0x11Cu, 18u),
            (VECTOR *)psx_addr(beam + 32u, 12u), &offset);
    else
        ApplyMatrixLV((MATRIX *)psx_addr(target + 0x11Cu, 20u),
            (VECTOR *)psx_addr(beam + 32u, 12u), &offset);
    points[0].vx = (sint32)(r_u32(target + 8u) + (uint32)offset.vx);
    points[0].vy = (sint32)(r_u32(target + 12u) + (uint32)offset.vy);
    value = r_u32(target + 16u);
    player = r_u32(0x80040FDCu);
    points[0].vz = (sint32)(value + (uint32)offset.vz);
    if (object == player)
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u),
            (VECTOR *)psx_addr(beam + 16u, 12u), &offset);
    else
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u),
            (VECTOR *)psx_addr(beam + 16u, 12u), &offset);
    points[3].vx = (sint32)(r_u32(object + 8u) + (uint32)offset.vx);
    difference.vx = (sint32)((uint32)points[3].vx - (uint32)points[0].vx);
    points[3].vy = (sint32)(r_u32(object + 12u) + (uint32)offset.vy);
    difference.vy = (sint32)((uint32)points[3].vy - (uint32)points[0].vy);
    points[3].vz = (sint32)(r_u32(object + 16u) + (uint32)offset.vz);
    difference.vz = (sint32)((uint32)points[3].vz - (uint32)points[0].vz);
    length = vector_length_approx(difference.vx, difference.vy, difference.vz);
    if (length == 0)
        length = 1;
    amplitude = length >> 2;
    if (amplitude >= 16001)
        amplitude = 16000;
    phase = r_u32(0x800311B8u) << 6;
    product = (uint32)amplitude * (uint32)rsin((sint32)phase);
    component = (uint32)((sint32)((uint32)difference.vx * 20u) >> 6);
    points[1].vx = (sint32)((uint32)points[0].vx + component + (uint32)((sint32)product >> 14));
    phase = r_u32(0x800311B8u) << 7;
    product = (uint32)amplitude * (uint32)rcos((sint32)phase);
    component = (uint32)((sint32)((uint32)difference.vy * 20u) >> 6);
    points[1].vy = (sint32)((uint32)points[0].vy + component - (uint32)((sint32)product >> 14));
    phase = (0u - r_u32(0x800311B8u)) << 8;
    product = (uint32)amplitude * (uint32)rsin((sint32)phase);
    component = (uint32)((sint32)((uint32)difference.vz * 20u) >> 6);
    points[1].vz = (sint32)((uint32)points[0].vz + component + (uint32)((sint32)product >> 14));
    phase = r_u32(0x800311B8u) << 8;
    product = (uint32)amplitude * (uint32)rsin((sint32)phase);
    component = (uint32)((sint32)((uint32)difference.vx * 40u) >> 6);
    points[2].vx = (sint32)((uint32)points[0].vx + component - (uint32)((sint32)product >> 14));
    phase = r_u32(0x800311B8u) << 7;
    product = (uint32)amplitude * (uint32)rcos((sint32)phase);
    component = (uint32)((sint32)((uint32)difference.vy * 40u) >> 6);
    points[2].vy = (sint32)((uint32)points[0].vy + component + (uint32)((sint32)product >> 14));
    phase = (0u - r_u32(0x800311B8u)) << 6;
    product = (uint32)amplitude * (uint32)rsin((sint32)phase);
    component = (uint32)((sint32)((uint32)difference.vz * 40u) >> 6);
    points[2].vz = (sint32)((uint32)points[0].vz + component - (uint32)((sint32)product >> 14));
    game_draw_tractor_beam_native(points, 0u);
    if (r_s16(beam + 0x40u) != 0)
        return;
    numerator = (uint32)difference.vx << 12;
    if (length == -1 && numerator == 0x80000000u)
        xport_mips_break(0x5Du);
    nx = (uint32)((sint64)(sint32)numerator / length);
    numerator = (uint32)difference.vy << 12;
    if (length == -1 && numerator == 0x80000000u)
        xport_mips_break(0x5Du);
    ny = (uint32)((sint64)(sint32)numerator / length);
    numerator = (uint32)difference.vz << 12;
    if (length == -1 && numerator == 0x80000000u)
        xport_mips_break(0x5Du);
    nz = (uint32)((sint64)(sint32)numerator / length);
    value = r_u32(beam + 0x34u);
    if ((sint32)value < (sint32)((uint32)length + 3000u)) {
        limit = (r_u32(target + 0x114u) & 16u) != 0u ? 2200 : 1400;
        if (limit < length)
            length = limit;
        product = nx * (uint32)length;
        value = r_u32(target + 0x14u);
        w_u32(target + 0x14u, value + (uint32)((sint32)product >> 12));
        product = ny * (uint32)length;
        value = r_u32(target + 0x18u);
        w_u32(target + 0x18u, value + (uint32)((sint32)product >> 12));
        product = nz * (uint32)length;
        value = r_u32(target + 0x1Cu);
        w_u32(target + 0x1Cu, value + (uint32)((sint32)product >> 12));
    }
    game_update_object_movement(target);
}

void game_finish_effect_updates(void)
{
    uint32 effect;
    FUNCTION_MARKER(0x800B2164, "GAME.OVL");
    effect = r_u32(0x800F337Cu);
    game_update_effect_chain(effect);
    game_process_effect_removals();
}


void game_update_effect_chain(uint32 effect)
{
    MATRIX first, second, model, light;
    VECTOR relative, transformed;
    uint32 x, y, z, vx, vy, vz, payload, camera, value, yaw, pitch, roll;
    uint32 index, table, base, shift, extra_shift, bound, horizontal, vertical, depth, distance;
    sint32 scale, maximum;
    FUNCTION_MARKER(0x800B0994, "GAME.OVL");
    w_u32(0x8010AB84u, 0u);
    if (effect == 0u)
        return;
    while (effect != 0u) {
        x = r_u32(effect);
        vx = r_u32(effect + 0x20u);
        y = r_u32(effect + 4u);
        vy = r_u32(effect + 0x24u);
        w_u16(effect + 0x42u, 0u);
        w_u32(effect, x + vx);
        z = r_u32(effect + 8u);
        w_u32(effect + 4u, y + vy);
        vz = r_u32(effect + 0x28u);
        payload = r_u32(effect + 0x6Cu);
        w_u32(effect + 8u, z + vz);
        value = r_u16(payload + 0x34u);
        w_u16(payload + 0x2Eu, (uint16)value);
        payload = r_u32(effect + 0x6Cu);
        value = r_u16(payload + 0x36u);
        w_u16(payload + 0x30u, (uint16)value);
        payload = r_u32(effect + 0x6Cu);
        value = r_u16(payload + 0x38u);
        w_u16(payload + 0x32u, (uint16)value);
        payload = r_u32(effect + 0x6Cu);
        extra_shift = 0u;
        yaw = (uint32)r_s16(payload + 0x2Eu);
        w_u32(0x8010AB84u, effect);
        index = 0u;
        if (yaw == 0u && r_u32(payload + 0x30u) == 0u)
            goto effect_camera_relative;
        if (yaw != 0u) {
            pitch = (uint32)r_s16(payload + 0x30u);
            if (pitch != 0u) {
                roll = (uint32)r_s16(payload + 0x32u);
                matrix_rotate_y(&first, (MATRIX *)psx_addr(payload + 4u, 20u), 0u - yaw);
                payload = r_u32(effect + 0x6Cu);
                pitch = (uint32)r_s16(payload + 0x30u);
                if (roll != 0u) {
                    matrix_rotate_x(&second, &first, pitch);
                    payload = r_u32(effect + 0x6Cu);
                    roll = (uint32)r_s16(payload + 0x32u);
                    matrix_rotate_z((MATRIX *)psx_addr(payload + 4u, 20u), &second, 0u - roll);
                } else {
                    matrix_rotate_x((MATRIX *)psx_addr(payload + 4u, 20u), &first, pitch);
                }
            } else {
                roll = (uint32)r_s16(payload + 0x32u);
                matrix_rotate_y(&first, (MATRIX *)psx_addr(payload + 4u, 20u), 0u - yaw);
                payload = r_u32(effect + 0x6Cu);
                if (roll != 0u) {
                    roll = (uint32)r_s16(payload + 0x32u);
                    matrix_rotate_z((MATRIX *)psx_addr(payload + 4u, 20u), &first, 0u - roll);
                } else {
                    matrix_copy_rotation_native(payload + 4u, &first);
                }
            }
        } else {
            pitch = (uint32)r_s16(payload + 0x30u);
            if (pitch != 0u) {
                roll = (uint32)r_s16(payload + 0x32u);
                matrix_rotate_x(&first, (MATRIX *)psx_addr(payload + 4u, 20u), pitch);
                payload = r_u32(effect + 0x6Cu);
                if (roll != 0u) {
                    roll = (uint32)r_s16(payload + 0x32u);
                    matrix_rotate_z((MATRIX *)psx_addr(payload + 4u, 20u), &first, 0u - roll);
                } else {
                    matrix_copy_rotation_native(payload + 4u, &first);
                }
            } else {
                roll = (uint32)r_s16(payload + 0x32u);
                if (roll != 0u) {
                    matrix_rotate_z(&first, (MATRIX *)psx_addr(payload + 4u, 20u), 0u - roll);
                    payload = r_u32(effect + 0x6Cu);
                    matrix_copy_rotation_native(payload + 4u, &first);
                }
            }
        }
        payload = r_u32(effect + 0x6Cu);
        value = r_u16(payload + 0x2Cu) - 1u;
        w_u16(payload + 0x2Cu, (uint16)value);
        payload = r_u32(effect + 0x6Cu);
        if (r_s16(payload + 0x2Cu) <= 0) {
            matrix_normalize(payload + 4u, 1u);
            payload = r_u32(effect + 0x6Cu);
            w_u16(payload + 0x2Cu, 150u);
        }
    effect_camera_relative:
        camera = r_u32(0x80040C94u);
        x = r_u32(effect);
        value = r_u32(camera + 8u);
        w_u32(0x800313D8u, 0u);
        w_u32(0x80052838u, 0u);
        w_u32(0x8004EB2Cu, 0u);
        w_u32(0x80052E64u, 0x808080u);
        x -= value;
        relative.vx = (sint32)x;
        y = r_u32(effect + 4u);
        value = r_u32(camera + 12u);
        if ((sint32)x < 0)
            x = 0u - x;
        w_u32(0x8004EB18u, 2u);
        y -= value;
        relative.vy = (sint32)y;
        z = r_u32(effect + 8u);
        value = r_u32(camera + 16u);
        if ((sint32)y < 0)
            y = 0u - y;
        z -= value;
        relative.vz = (sint32)z;
        if ((sint32)z < 0)
            z = 0u - z;
        maximum = (sint32)x;
        if (maximum < (sint32)y)
            maximum = (sint32)y;
        if (maximum < (sint32)z)
            maximum = (sint32)z;
        payload = r_u32(effect + 0x6Cu);
        table = r_u32(payload);
        if (table != 0u) {
            for (;;) {
                if (r_s8(table + 4u) == -1)
                    goto effect_expiry;
                value = r_u32(table);
                table += 8u;
                if ((sint32)value >= maximum)
                    break;
                ++index;
            }
            payload = r_u32(effect + 0x6Cu);
            value = r_u32(payload + 0x24u) + index;
            base = r_u32(0x80046178u);
            value = r_u32(base + value * 132u);
            w_u32(payload + 0x28u, value);
        }
        scale = r_s16(effect + 0x34u);
        if (scale > 0) {
            shift = (uint32)r_s16(effect + 0x34u) & 31u;
            relative.vx >>= shift;
            shift = (uint32)r_s16(effect + 0x34u) & 31u;
            relative.vy >>= shift;
            shift = (uint32)r_s16(effect + 0x34u) & 31u;
            w_u32(0x80053670u, (uint32)scale);
            relative.vz >>= shift;
        } else {
            w_u32(0x80053670u, 0u);
        }
        x = (uint32)relative.vx;
        y = (uint32)relative.vy;
        z = (uint32)relative.vz;
        if ((sint32)x < 0)
            x = 0u - x;
        if ((sint32)y < 0)
            y = 0u - y;
        if ((sint32)z < 0)
            z = 0u - z;
        distance = (uint32)((sint32)(x | y | z) >> 14);
        while (distance != 0u) {
            distance = (uint32)((sint32)distance >> 1);
            value = r_u32(0x80053670u);
            w_u32(0x80053670u, value + 1u);
            ++extra_shift;
        }
        if (extra_shift != 0u) {
            relative.vx >>= extra_shift & 31u;
            relative.vz >>= extra_shift & 31u;
            relative.vy >>= extra_shift & 31u;
        }
        ApplyMatrixLV((MATRIX *)psx_addr(0x80035060u, 20u), &relative, &transformed);
        shift = r_u32(0x80053670u) & 31u;
        x = (uint32)transformed.vx << shift;
        z = (uint32)transformed.vz << shift;
        y = (uint32)transformed.vy << shift;
        scale = r_s16(effect + 0x34u);
        payload = r_u32(effect + 0x6Cu);
        value = r_u32(payload + 0x24u);
        base = r_u32(0x80046178u);
        bound = r_u32(base + value * 132u + 0x6Cu);
        if (scale > 0) {
            bound <<= (uint32)scale & 31u;
            depth = (sint32)z > 0 ? z + 1000u : 0u - z - 1000u;
            horizontal = (uint32)((sint32)depth >> 10) * 3u << 9;
            vertical = (uint32)((sint32)depth >> 1);
        } else {
            value = (uint32)scale;
            if (scale < 0)
                value = 0u - value;
            extra_shift += value;
            bound = (uint32)((sint32)bound >> ((0u - (uint32)scale) & 31u)) << 10;
            depth = (sint32)z > 0 ? z + 1000u : 0u - z - 1000u;
            horizontal = (uint32)((sint32)((depth * 3u) << 9) >> 10);
            vertical = (uint32)((sint32)depth >> 1);
        }
        distance = (sint32)x < 0 ? 0u - x : x;
        distance = (sint32)z > 0 ? distance - horizontal : distance + horizontal;
        if (scale > 0) {
            if ((sint32)distance < 0)
                distance += 1023u;
            distance = (uint32)((sint32)distance >> 10);
        }
        distance *= 724u;
        if ((sint32)bound < (sint32)distance)
            goto effect_expiry;
        distance = (sint32)y < 0 ? 0u - y : y;
        distance = (sint32)z > 0 ? distance - vertical : distance + vertical;
        if (scale > 0) {
            if ((sint32)distance < 0)
                distance += 1023u;
            distance = (uint32)((sint32)distance >> 10);
        }
        distance *= 724u;
        if ((sint32)bound < (sint32)distance)
            goto effect_expiry;
        payload = r_u32(effect + 0x6Cu);
        matrix_copy_rotation_to_native(&model, payload + 4u);
        matrix_shift_native(&model, &first, extra_shift);
        MulMatrix0((MATRIX *)psx_addr(0x80035060u, 20u), &first, &second);
        MulMatrix0((MATRIX *)psx_addr(0x80041F28u, 20u), &model, &light);
        SetRotMatrix(&second);
        SetLightMatrix(&light);
        w_u32(0x80034D6Cu, (uint32)transformed.vx);
        w_u32(0x80034D70u, (uint32)transformed.vy);
        w_u32(0x80034D74u, (uint32)transformed.vz);
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
        payload = r_u32(effect + 0x6Cu);
        w_u16(effect + 0x42u, 1u);
        render_emit_mesh_primitives(r_u32(payload + 0x28u));
    effect_expiry:
        value = r_u32(effect + 0x2Cu) - 1u;
        w_u32(effect + 0x2Cu, value);
        if ((sint32)value <= 0)
            game_expire_model_effect(effect);
        effect = r_u32(effect + 0x50u);
    }
    w_u32(0x8010AB84u, 0u);
}

void game_expire_model_effect(uint32 effect)
{
    uint32 player, x, y, z, px, py, pz, value;
    sint32 type, distance;
    FUNCTION_MARKER(0x800B11B8, "GAME.OVL");
    type = r_s16(effect + 0x3Cu);
    if (type == 107) {
        value = r_u32(0x800595CCu);
        w_u32(0x800595CCu, value - 1u);
    } else if (type == 100) {
        player = r_u32(0x80040FDCu);
        x = r_u32(effect);
        y = r_u32(effect + 4u);
        px = r_u32(player + 8u);
        z = r_u32(effect + 8u);
        py = r_u32(player + 12u);
        pz = r_u32(player + 16u);
        distance = vector_length_approx((sint32)(px - x), (sint32)(py - y), (sint32)(pz - z));
        w_u32(0x80046174u, (uint32)distance);
        if (distance <= 200000)
            return;
        if (r_s32(effect + 0x30u) >= distance) {
            w_u32(effect + 0x30u, (uint32)distance);
            w_u32(effect + 0x2Cu, 20u);
            return;
        }
        value = r_u32(0x800595CCu);
        w_u32(0x800595CCu, value - 1u);
    }
    game_mark_effect_for_removal(effect);
}

void game_finish_scene_objects(void)
{
    uint32 object, callback, yaw, pitch, roll;
    FUNCTION_MARKER(0x800AE054, "GAME.OVL");
    object = r_u32(0x8005367Cu);
    while (object != 0u) {
        callback = r_u32(object + 0x3Cu);
        psx_dispatch_guest_call(callback, 1u, object, 0u, 0u);
        yaw = r_u32(object + 0x20u);
        pitch = r_u32(object + 0x24u);
        roll = r_u32(object + 0x28u);
        w_u32(object + 0x2Cu, yaw);
        w_u32(object + 0x30u, pitch);
        w_u32(object + 0x34u, roll);
        object = r_u32(object + 0x1D4u);
    }
}

sint32 game_restore_scene_draw_color(void)
{
    sint32 color;
    FUNCTION_MARKER(0x80092AD4, "GAME.OVL");
    color = r_s32(0x800ECE6Cu);
    w_u8(0x8004617Cu, 255u);
    w_u8(0x80033D38u, 0u);
    w_u8(0x80053680u, 0u);
    w_u32(0x80033D2Cu, 242u);
    return draw_set_color_packed(color);
}


void game_draw_scene_descriptor_chain(void)
{
    uint32 descriptors, camera;
    FUNCTION_MARKER(0x800AFF30, "GAME.OVL");
    if (r_u32(0x800EC884u) != 0u)
        return;
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    descriptors = r_u32(0x80123F64u);
    camera = r_u32(0x80040C94u);
    game_draw_descriptor_chain(descriptors, camera, 0u);
}


void game_unlink_mesh_effect_type(uint32 effect)
{
    uint32 previous, next, type;
    FUNCTION_MARKER(0x800BBBF0, "GAME.OVL");
    previous = r_u32(effect + 0x1Cu);
    if (previous != 0u) {
        next = r_u32(effect + 0x18u);
        if (next != 0u) {
            w_u32(previous + 0x18u, next);
            next = r_u32(effect + 0x18u);
            previous = r_u32(effect + 0x1Cu);
            w_u32(next + 0x1Cu, previous);
        } else {
            w_u32(previous + 0x18u, 0u);
        }
    } else {
        next = r_u32(effect + 0x18u);
        if (next != 0u) {
            w_u32(next + 0x1Cu, 0u);
            type = (uint32)r_s16(effect + 0x2Au);
            next = r_u32(effect + 0x18u);
            w_u32(0x800FB3E0u + (type << 2), next);
        } else {
            type = (uint32)r_s16(effect + 0x2Au);
            w_u32(0x800FB3E0u + (type << 2), 0u);
        }
    }
    w_u32(effect + 0x18u, 0u);
    w_u32(effect + 0x1Cu, 0u);
}


void game_process_mesh_effect_removals(void)
{
    uint32 index, effect, previous, next, count, free_head;
    FUNCTION_MARKER(0x800BBAD0, "GAME.OVL");
    count = r_u32(0x80109B38u);
    index = 0u;
    if ((sint32)count <= 0)
        return;
    do {
        effect = r_u32(0x800FB440u + ((index & 0xFFFFu) << 2));
        previous = r_u32(effect + 0x14u);
        if (previous != 0u) {
            next = r_u32(effect + 0x10u);
            if (next != 0u) {
                w_u32(previous + 0x10u, next);
                next = r_u32(effect + 0x10u);
                previous = r_u32(effect + 0x14u);
                w_u32(next + 0x14u, previous);
            } else {
                w_u32(previous + 0x10u, 0u);
            }
        } else {
            next = r_u32(effect + 0x10u);
            if (next != 0u) {
                w_u32(next + 0x14u, 0u);
                next = r_u32(effect + 0x10u);
                w_u32(0x80108A48u, next);
            } else {
                w_u32(0x80108A48u, 0u);
            }
        }
        count = r_u32(0x80126D60u);
        w_u32(0x80126D60u, count - 1u);
        game_unlink_mesh_effect_type(effect);
        free_head = r_u32(0x801094B0u);
        count = r_u32(0x80109B38u);
        ++index;
        w_u32(0x801094B0u, effect);
        w_u32(effect + 0x10u, free_head);
        w_u32(free_head + 0x14u, effect);
    } while ((sint32)(index & 0xFFFFu) < (sint32)count);
}


void game_draw_mesh_effect_chain(uint32 effect)
{
    uint32 camera;
    FUNCTION_MARKER(0x800BBCF8, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    camera = r_u32(0x80040C94u);
    game_draw_mesh_effects_from_camera(effect, camera);
}


void game_draw_scene_mesh_effects(void)
{
    uint32 effect;
    FUNCTION_MARKER(0x800BBCC0, "GAME.OVL");
    effect = r_u32(0x800FB3E0u);
    w_u32(0x80109B38u, 0u);
    game_draw_mesh_effect_chain(effect);
    game_process_mesh_effect_removals();
}


void game_draw_active_groups(void)
{
    uint32 index;
    FUNCTION_MARKER(0x800903D0, "GAME.OVL");
    for (index = 0u; index < 2u; ++index) {
        if (r_u32(0x8010ACDCu + index * 0x5C48u) != 0u)
            game_update_warp_grid(index);
    }
}


void game_draw_view_by_mode(uint32 view, uint32 index)
{
    sint32 mode;
    FUNCTION_MARKER(0x8008E510, "GAME.OVL");
    mode = r_s16(0x80123F90u + index * 28u);
    if (mode == 2)
        game_draw_red_light_occlusion(view, index);
    else if (mode == 3)
        game_draw_violet_light_occlusion(view, index);
    else
        game_draw_projected_light_occlusion(view, index);
}


void game_reset_object_collision_links(void)
{
    uint32 groups[3], index, object;
    FUNCTION_MARKER(0x800A27A0, "GAME.OVL");
    groups[0] = r_u32(0x80075D08u);
    groups[1] = r_u32(0x80075D0Cu);
    groups[2] = r_u32(0x80075D10u);
    w_u32(0x800ECE68u, 0u);
    for (index = 0u; index < 3u; ++index) {
        object = object_lookup(groups[index]);
        while (object != 0u) {
            w_u32(object + 0x190u, 0u);
            object = r_u32(object + 0x1DCu);
        }
    }
}


void game_collide_scene_object_groups(void)
{
    static const uint32 object_groups[3] = {3u, 3u, 1u};
    static const uint32 target_groups[3] = {1u, 3u, 1u};
    uint32 pass, object, head, model, base, radius, scale, range, player;
    FUNCTION_MARKER(0x800A20D0, "GAME.OVL");
    for (pass = 0u; pass < 3u; ++pass) {
        object = object_lookup(object_groups[pass]);
        while (object != 0u) {
            head = object_lookup(target_groups[pass]);
            model = r_u32(object + 0x74u);
            base = r_u32(0x80046178u);
            radius = r_u32(base + model * 132u + 0x6Cu);
            scale = r_u32(object + 0x84u);
            w_u32(0x80046174u, radius);
            if ((sint32)scale < 0) {
                radius = (uint32)((sint32)radius >> ((0u - scale) & 31u));
                w_u32(0x80046174u, radius);
            }
            range = r_u32(0x80046174u);
            if ((sint32)range >= 1001)
                w_u32(0x80046174u, 1000u);
            if (r_u32(object + 0xA0u) != 0u) {
                if ((r_u32(object + 0x114u) & 0x8000u) == 0u) {
                    range = r_u32(0x80046174u);
                    game_collide_object_groups(head, object, (sint32)range, 1u, pass == 0u ? 0u : 1u);
                }
            }
            object = r_u32(object + 0x1DCu);
        }
    }
    for (pass = 0u; pass < 2u; ++pass) {
        object = object_lookup(pass == 0u ? 3u : 1u);
        while (object != 0u) {
            head = object_lookup(6u);
            if (r_u32(object + 0xA0u) != 0u) {
                if ((r_u32(object + 0x114u) & 0x8000u) == 0u)
                    game_collide_object_groups(head, object, 4000, 0u, 0u);
            }
            object = r_u32(object + 0x1DCu);
        }
    }
    head = object_lookup(7u);
    player = r_u32(0x80040FDCu);
    game_collide_object_groups(head, player, 4000, 1u, 0u);
}


void game_draw_active_effect_slots(void)
{
    uint32 index, offset, object, parameter;
    FUNCTION_MARKER(0x800DF51C, "GAME.OVL");
    for (index = 0u; index < 32u; ++index) {
        offset = index * 0x34u;
        if (r_u8(0x801094E8u + offset) != 0u) {
            object = r_u32(0x801094E4u + offset);
            parameter = r_u32(0x801094E0u + offset);
            game_update_linked_part_constraint(object, parameter, 0x801094C8u + offset,
                0x801094B8u + offset, 2048u);
        }
    }
}


void game_update_camera_particles(void)
{
    uint32 camera;
    FUNCTION_MARKER(0x800C7684, "GAME.OVL");
    camera = r_u32(0x80040C94u);
    game_process_camera_particle_field(camera + 8u, 0x8011A3A8u);
}


void game_emit_object_spark_native(uint32 object, const VECTOR *offset, uint32 size, uint32 color)
{
    uint32 seed, first, second, third, slot, base, dx, dy, dz, value, next, quotient;
    sint32 divisor;
    FUNCTION_MARKER(0x800C77E0, "GAME.OVL");
    seed = r_u32(0x800EF50Cu);
    divisor = (sint32)(size << 3);
    first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    if (divisor == 0)
        xport_mips_break(7u);
    dx = (uint32)((sint32)(first & 0x7FFFu) % divisor);
    second = (uint32)((sint32)first >> 1) ^ (first << 3);
    dy = (uint32)((sint32)(second & 0x7FFFu) % divisor);
    third = (uint32)((sint32)second >> 1) ^ (second << 3);
    dz = (uint32)((sint32)(third & 0x7FFFu) % divisor);
    slot = r_u32(0x800EF4F8u);
    base = 0x800FD728u + slot * 48u;
    w_u32(base, object);
    w_u32(base + 0x24u, size);
    w_u32(base + 0x28u, color);
    w_u32(base + 0x2Cu, 0u);
    value = (uint32)offset->vx;
    next = slot + 1u;
    w_u32(base + 4u, value);
    value = (uint32)offset->vy;
    size <<= 2;
    w_u32(0x800EF50Cu, first);
    w_u32(0x800EF50Cu, second);
    w_u32(0x800EF50Cu, third);
    w_u32(base + 8u, value);
    value = (uint32)offset->vz;
    w_u32(base + 12u, value);
    w_u32(base + 0x14u, dx - size);
    w_u32(base + 0x18u, dy - size);
    w_u32(base + 0x1Cu, dz - size);
    value = (sint32)next < 0 ? slot + 64u : next;
    quotient = (uint32)((sint32)value >> 6) << 6;
    w_u32(0x800EF4F8u, next - quotient);
}

void game_emit_object_spark(uint32 object, uint32 offset, uint32 size, uint32 color)
{
    game_emit_object_spark_native(object, (const VECTOR *)psx_addr(offset, 12u), size, color);
}


void game_update_scene_particle_effects(void)
{
    VECTOR offset;
    uint32 player, phase, value;
    sint32 glow;
    FUNCTION_MARKER(0x800C803C, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    game_update_camera_particles();
    game_update_object_particle_slots();
    game_update_object_spark_slots();
    if (r_u32(0x800EC884u) != 0u)
        return;
    if (r_u32(0x800EF4BCu) == 6u) {
        if (r_s32(0x800313DCu) >= 4) {
            player = r_u32(0x80040FDCu);
            offset.vx = 0;
            offset.vy = 0;
            offset.vz = 1024;
            game_emit_object_spark_native(player, &offset, 512u, 0xFF2070u);
            player = r_u32(0x80040FDCu);
            game_emit_object_spark_native(player, &offset, 512u, 0xFF2070u);
        }
    }
    if (r_s16(0x800595F6u) != 0)
        return;
    if (r_u32(0x800EC868u) != 0u)
        return;
    player = r_u32(0x80040FDCu);
    if (r_s32(player + 0x238u) >= 20)
        return;
    if (r_s32(0x800313DCu) >= 4)
        return;
    phase = r_u32(0x800311B8u) << 5;
    value = (uint32)rsin((sint32)phase) << 5;
    glow = (sint32)((uint32)((sint32)value >> 12) + 33u);
    if (r_s32(0x800ECE9Cu) < glow)
        w_u32(0x800ECE9Cu, (uint32)glow);
}


void game_draw_camera_particle_layers(void)
{
    uint32 camera, delta, camera_after;
    sint32 x, y, z, speed, intensity, high;
    sint64 product;
    FUNCTION_MARKER(0x8008C6D0, "GAME.OVL");
    if (r_u32(0x8001020Cu) == 0u) {
        camera = r_u32(0x80040C94u);
        game_update_camera_streaks(0x80116560u, camera);
    }
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    camera = r_u32(0x80040C94u);
    x = r_s32(camera + 0x14u) >> 8;
    y = r_s32(camera + 0x18u) >> 8;
    z = r_s32(camera + 0x1Cu) >> 8;
    speed = vector_length_approx(x, y, z);
    if (speed == 0)
        speed = 1;
    if (speed < 1280) {
        delta = 1280u - (uint32)speed;
        product = (sint64)(sint32)delta * 0x66666667;
        high = (sint32)(product >> 32);
        camera_after = r_u32(0x80040C94u);
        intensity = (sint32)((uint32)(high >> 1) - (uint32)((sint32)delta >> 31));
        game_draw_camera_streak_layer_a(0u, 0x80116560u, camera_after, intensity);
        camera_after = r_u32(0x80040C94u);
        game_draw_camera_streak_layer_b(0u, 0x80116560u, camera_after, intensity);
    }
    game_draw_camera_overlay();
}


void game_emit_camera_particle_native(const VECTOR *position, const VECTOR *velocity, uint32 color, uint32 size)
{
    uint32 seed, random, slot, base, remainder, value, next, quotient;
    sint32 divisor;
    FUNCTION_MARKER(0x800C74E8, "GAME.OVL");
    seed = r_u32(0x800EF50Cu);
    divisor = (sint32)size >> 1;
    random = (uint32)((sint32)seed >> 1) ^ (seed << 3);
    if (divisor == 0)
        xport_mips_break(7u);
    remainder = (uint32)((sint32)(random & 0x7FFFu) % divisor);
    slot = r_u32(0x800EF4F4u);
    base = 0x800FB5C8u + slot * 56u;
    w_u32(base + 0x10u, 200u);
    w_u32(base + 0x1Cu, color & 255u);
    w_u32(base + 0x20u, (color >> 8) & 255u);
    w_u32(base + 0x24u, (color >> 16) & 255u);
    w_u32(base + 0x18u, size + remainder);
    value = (uint32)position->vx;
    w_u32(base, value);
    value = (uint32)position->vy;
    w_u32(base + 4u, value);
    value = (uint32)position->vz;
    w_u32(base + 8u, value);
    value = (uint32)velocity->vx;
    next = slot + 1u;
    w_u32(0x800EF50Cu, random);
    w_u32(base + 0x28u, value);
    value = (uint32)velocity->vy;
    w_u32(base + 0x2Cu, value);
    value = (uint32)velocity->vz;
    w_u32(base + 0x14u, 10u);
    w_u32(base + 0x30u, value);
    value = (sint32)next < 0 ? slot + 128u : next;
    quotient = (uint32)((sint32)value >> 7) << 7;
    w_u32(0x800EF4F4u, next - quotient);
}


void game_update_object_particle_slots(void)
{
    VECTOR position, velocity;
    uint32 index, slot, object, sum, numerator, seed, first, second, third, component;
    sint32 length;
    FUNCTION_MARKER(0x800C7A18, "GAME.OVL");
    for (index = 0u; index < 16u; ++index) {
        slot = 0x80118090u + index * 28u;
        if (r_s32(slot + 0x14u) <= 0)
            continue;
        object = r_u32(slot);
        ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u),
            (VECTOR *)psx_addr(slot + 4u, 12u), &position);
        sum = (uint32)position.vx * (uint32)position.vx;
        sum += (uint32)position.vy * (uint32)position.vy;
        sum += (uint32)position.vz * (uint32)position.vz;
        length = (sint32)SquareRoot0((sint32)sum);
        if (length == 0)
            length = 1;
        numerator = (uint32)position.vx << 7;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        velocity.vx = (sint32)(uint32)((sint64)(sint32)numerator / length);
        numerator = (uint32)position.vy << 7;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        velocity.vy = (sint32)(uint32)((sint64)(sint32)numerator / length);
        numerator = (uint32)position.vz << 7;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        velocity.vz = (sint32)(uint32)((sint64)(sint32)numerator / length);
        seed = r_u32(0x800EF50Cu);
        first = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        velocity.vx = (sint32)((uint32)velocity.vx - 32u + (first & 63u));
        second = (uint32)((sint32)first >> 1) ^ (first << 3);
        velocity.vy = (sint32)((uint32)velocity.vy - 32u + (second & 63u));
        third = (uint32)((sint32)second >> 1) ^ (second << 3);
        velocity.vz = (sint32)((uint32)velocity.vz - 32u + (third & 63u));
        object = r_u32(slot);
        w_u32(0x800EF50Cu, first);
        component = r_u32(object + 8u);
        position.vx = (sint32)((uint32)position.vx + component);
        object = r_u32(slot);
        w_u32(0x800EF50Cu, second);
        w_u32(0x800EF50Cu, third);
        component = r_u32(object + 12u);
        position.vy = (sint32)((uint32)position.vy + component);
        object = r_u32(slot);
        component = r_u32(object + 16u);
        position.vz = (sint32)((uint32)position.vz + component);
        game_emit_camera_particle_native(&position, &velocity, 0x104080u, 600u);
        game_emit_narrow_particles(3u, (uint32)position.vx, (uint32)position.vy,
            (uint32)position.vz, (uint32)velocity.vx, (uint32)velocity.vy, (uint32)velocity.vz);
    }
}


void game_update_object_spark_slots(void)
{
    VECTOR relative, position;
    uint32 index, base, x, y, z, value, object, scale, vx, vy, vz, color, timer, red, green, blue, player, size;
    FUNCTION_MARKER(0x800C7C98, "GAME.OVL");
    for (index = 0u; index < 64u; ++index) {
        base = 0x800FD728u + index * 48u;
        if (r_s32(base + 0x24u) <= 0)
            continue;
        value = r_u32(base + 0x14u);
        x = r_u32(base + 4u);
        x += value;
        value = r_u32(base + 0x18u);
        y = r_u32(base + 8u);
        y += value;
        value = r_u32(base + 0x1Cu);
        z = r_u32(base + 12u);
        z += value;
        object = r_u32(base);
        scale = r_u32(object + 0x84u);
        if ((sint32)scale > 0) {
            x <<= scale & 31u;
            object = r_u32(base);
            scale = r_u32(object + 0x84u);
            y <<= scale & 31u;
            object = r_u32(base);
            scale = r_u32(object + 0x84u);
            z <<= scale & 31u;
        }
        relative.vx = (sint32)x;
        relative.vy = (sint32)y;
        relative.vz = (sint32)z;
        vx = r_u32(base + 0x14u);
        vy = r_u32(base + 0x18u);
        w_u32(base + 0x14u, vx - (uint32)((sint32)vx >> 2));
        vz = r_u32(base + 0x1Cu);
        w_u32(base + 0x18u, vy - (uint32)((sint32)vy >> 2));
        w_u32(base + 0x1Cu, vz - (uint32)((sint32)vz >> 2));
        color = r_u32(base + 0x28u);
        timer = r_u32(base + 0x2Cu);
        red = (uint32)((sint32)((color & 255u) * timer) >> 8);
        green = (uint32)((sint32)(((color >> 8) & 255u) * timer) >> 8) << 8;
        blue = (uint32)((sint32)(((color >> 16) & 255u) * timer) >> 8) << 16;
        object = r_u32(base);
        player = r_u32(0x80040FDCu);
        color = red + green + blue;
        if (object == player)
            game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &relative, &position);
        else
            ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u), &relative, &position);
        object = r_u32(base);
        position.vx = (sint32)((uint32)position.vx + r_u32(object + 8u));
        object = r_u32(base);
        position.vy = (sint32)((uint32)position.vy + r_u32(object + 12u));
        object = r_u32(base);
        position.vz = (sint32)((uint32)position.vz + r_u32(object + 16u));
        SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
        object = r_u32(base);
        player = r_u32(0x80040FDCu);
        size = r_u32(base + 0x24u);
        if (object != player)
            size <<= 2;
        game_emit_radial_effect((uint32)position.vx, (uint32)position.vy, (uint32)position.vz, size, color);
        timer = r_u32(base + 0x2Cu) + 20u;
        w_u32(base + 0x2Cu, timer);
        if ((sint32)timer >= 256)
            w_u32(base + 0x24u, 0u);
    }
}


void game_update_camera_streaks(uint32 particles, uint32 camera)
{
    uint32 index, point, shift, x, y, z;
    uint32 vx, vy, vz;
    FUNCTION_MARKER(0x800CBD88, "GAME.OVL");
    vx = (uint32)(r_s32(camera + 0x14u) >> 8);
    vy = (uint32)(r_s32(camera + 0x18u) >> 8);
    vz = (uint32)(r_s32(camera + 0x1Cu) >> 8);
    for (index = 0u; index < 345u; ++index) {
        point = particles + index * 20u;
        shift = r_u16(point + 0x10u) & 31u;
        x = (uint32)r_s16(point);
        y = (uint32)r_s16(point + 2u);
        z = (uint32)r_s16(point + 4u);
        x -= vx << shift;
        y -= vy << shift;
        z -= vz << shift;
        if ((sint32)(x - 16384u) > 0)
            x -= 32768u;
        else if ((sint32)(x + 16384u) <= 0)
            x += 32768u;
        if ((sint32)(y - 16384u) > 0)
            y -= 32768u;
        else if ((sint32)(y + 16384u) <= 0)
            y += 32768u;
        if ((sint32)(z - 16384u) > 0)
            z -= 32768u;
        else if ((sint32)(z + 16384u) <= 0)
            z += 32768u;
        w_u16(point, (uint16)x);
        w_u16(point + 2u, (uint16)y);
        w_u16(point + 4u, (uint16)z);
    }
}


void game_draw_camera_overlay(void)
{
    uint32 result;
    FUNCTION_MARKER(0x8008CC7C, "GAME.OVL");
    result = 0u;
    game_draw_camera_overlay_points_native(128u, &result);
}


void game_update_object_trails(void)
{
    uint32 slot, offset, ring, owner, value, color, mode;
    sint32 next;
    FUNCTION_MARKER(0x800B34B4, "GAME.OVL");
    for (slot = 0u; slot < 20u; ++slot) {
        offset = slot * 300u;
        if (r_s32(0x80124D68u + offset) <= 0)
            continue;
        ring = r_u32(0x80124D6Cu + offset);
        owner = r_u32(0x80124D70u + offset);
        value = r_u32(owner + 8u);
        w_u32(0x80124C50u + offset + ring * 28u, value);
        ring = r_u32(0x80124D6Cu + offset);
        owner = r_u32(0x80124D70u + offset);
        value = r_u32(owner + 12u);
        w_u32(0x80124C54u + offset + ring * 28u, value);
        ring = r_u32(0x80124D6Cu + offset);
        owner = r_u32(0x80124D70u + offset);
        value = r_u32(owner + 16u);
        w_u32(0x80124C58u + offset + ring * 28u, value);
        next = (sint32)(r_u32(0x80124D6Cu + offset) + 1u);
        w_u32(0x80124D6Cu + offset, (uint32)(next % 10));
        color = r_u32(0x80124D74u + offset);
        if ((sint32)color < 255)
            w_u32(0x80124D74u + offset, color + 128u);
        color = r_u32(0x80124D74u + offset);
        if ((sint32)color >= 256)
            w_u32(0x80124D74u + offset, 255u);
        ring = r_u32(0x80124D6Cu + offset);
        color = r_u32(0x80124D74u + offset);
        w_u32(0x80124C68u + offset + ring * 28u, color);
        mode = r_u32(0x80124D68u + offset);
        if (mode == 1u)
            game_draw_twisted_particle_trail(0x80124C50u + offset, 0u);
        else if (mode == 2u)
            game_draw_twisted_particle_trail(0x80124C50u + offset, 16u);
        else if (mode == 3u)
            game_draw_twisted_particle_trail(0x80124C50u + offset, 8u);
        ring = r_u32(0x80124D6Cu + offset);
        w_u32(0x80124C64u + offset + ring * 28u, 1u);
    }
    value = r_u32(0x800EF514u);
    w_u32(0x800EF514u, value + 1u);
}


void game_update_selected_object_link(void)
{
    VECTOR first, second;
    uint32 object, flags, mission, player, distance, target, step;
    sint32 power, selected;
    FUNCTION_MARKER(0x800CB254, "GAME.OVL");
    if (r_u32(0x800EF4E4u) != 0u) {
        first.vx = 0;
        first.vy = r_s32(0x800313DCu) < 4 ? 2048 : 256;
        object = r_u32(0x800EF4E4u);
        first.vz = 0;
        second.vx = 0;
        second.vy = 0;
        second.vz = 0;
        power = (sint32)(r_u32(object + 0x84u) + 3u);
        flags = r_u32(object + 0x114u);
        if ((flags & 0x40400000u) != 0u) {
            power >>= 2;
        } else {
            mission = r_u32(0x800595F8u);
            if (mission == 0x2000Bu && r_u32(object + 4u) == 5u) {
                power >>= 2;
            } else {
                selected = r_s16(0x80059614u);
                if (r_u32(object + 4u) == (uint32)selected)
                    power >>= 2;
            }
        }
        if (power < 0)
            power = 0;
        w_u32(0x80123F40u, 15000u << (((uint32)power << 1) & 31u));
        player = r_u32(0x80040FDCu);
        object = r_u32(0x800EF4E4u);
        distance = r_u32(0x800EF554u);
        game_draw_object_link_native(player, object, &first, &second, distance, 2u, (uint32)power);
        distance = r_u32(0x800EF554u);
        target = r_u32(0x80123F40u);
        if ((sint32)target < (sint32)distance) {
            step = distance - 512u;
            w_u32(0x800EF554u, step);
            if ((sint32)step < (sint32)target)
                w_u32(0x800EF554u, target);
        } else if ((sint32)distance < (sint32)target) {
            step = distance + 512u;
            w_u32(0x800EF554u, step);
            if ((sint32)target < (sint32)step)
                w_u32(0x800EF554u, target);
        }
        object = r_u32(0x800EF4E4u);
        if ((r_u32(object + 0x114u) & 16u) != 0u)
            game_separate_object_overlaps(object);
    }
    step = r_u32(0x800EF550u);
    w_u32(0x800EF550u, (step + 1u) & 15u);
}


static uint32 game_particle_add_checked(uint32 first, uint32 second, uint32 pc)
{
    uint32 result = first + second;
    if (((~(first ^ second)) & (first ^ result) & 0x80000000u) != 0u)
        xport_mips_overflow_exception(pc);
    return result;
}

static uint32 game_particle_sub_checked(uint32 first, uint32 second, uint32 pc)
{
    uint32 result = first - second;
    if (((first ^ second) & (first ^ result) & 0x80000000u) != 0u)
        xport_mips_overflow_exception(pc);
    return result;
}

void game_process_camera_particle_field(uint32 position, uint32 colors)
{
    SVECTOR vertex;
    uint16 texture[6];
    uint32 index, particle, life, decay, x, y, z, vx, vy, vz, red, green, blue, color;
    uint32 camera_x, camera_y, camera_z, shifts, check_x, check_y, check_z, size, depth, divisor;
    uint32 numerator, half_size, left_top, right_top, left_bottom, right_bottom, packet, ordering;
    uint32 value, tag, head, pointer;
    sint32 screen, quotient;
    FUNCTION_MARKER(0x800CBA0C, "GAME.OVL");
    for (index = 0u; index < 128u; ++index) {
        particle = 0x800FB5C8u + index * 56u;
        life = r_u32(particle + 0x10u);
        decay = r_u32(particle + 0x14u);
        life = game_particle_sub_checked(life, decay, 0x800CBA5Cu);
        w_u32(particle + 0x10u, life);
        if ((sint32)life <= 0)
            continue;
        x = r_u32(particle);
        y = r_u32(particle + 4u);
        z = r_u32(particle + 8u);
        vx = r_u32(particle + 0x28u);
        vy = r_u32(particle + 0x2Cu);
        vz = r_u32(particle + 0x30u);
        x = game_particle_add_checked(x, vx, 0x800CBA84u);
        y = game_particle_add_checked(y, vy, 0x800CBA88u);
        z = game_particle_add_checked(z, vz, 0x800CBA8Cu);
        w_u32(particle, x);
        w_u32(particle + 4u, y);
        w_u32(particle + 8u, z);
        red = r_u32(particle + 0x1Cu);
        green = r_u32(particle + 0x20u);
        blue = r_u32(particle + 0x24u);
        red = (red * life) >> 8;
        green = ((green * life) >> 8) << 8;
        blue = ((blue * life) >> 8) << 16;
        color = game_particle_add_checked(red, green, 0x800CBAD4u);
        color = game_particle_add_checked(color, blue, 0x800CBAD8u);
        camera_x = r_u32(position);
        camera_y = r_u32(position + 4u);
        camera_z = r_u32(position + 8u);
        x = game_particle_sub_checked(x, camera_x, 0x800CBAE8u);
        y = game_particle_sub_checked(y, camera_y, 0x800CBAECu);
        z = game_particle_sub_checked(z, camera_z, 0x800CBAF0u);
        shifts = 0u;
        for (;;) {
            check_x = game_particle_add_checked(x, 0xFFFFC000u, 0x800CBAF8u);
            check_y = game_particle_add_checked(y, 0xFFFFC000u, 0x800CBB00u);
            if ((sint32)check_x > 0)
                goto particle_reduce_coordinates;
            check_z = game_particle_add_checked(z, 0xFFFFC000u, 0x800CBB08u);
            if ((sint32)check_y > 0)
                goto particle_reduce_coordinates;
            check_x = game_particle_add_checked(x, 16384u, 0x800CBB10u);
            if ((sint32)check_z > 0)
                goto particle_reduce_coordinates;
            check_y = game_particle_add_checked(y, 16384u, 0x800CBB18u);
            if ((sint32)check_x < 0)
                goto particle_reduce_coordinates;
            check_z = game_particle_add_checked(z, 16384u, 0x800CBB20u);
            if ((sint32)check_y < 0 || (sint32)check_z < 0)
                goto particle_reduce_coordinates;
            break;
        particle_reduce_coordinates:
            ++shifts;
            x = (uint32)((sint32)x >> 1);
            y = (uint32)((sint32)y >> 1);
            z = (uint32)((sint32)z >> 1);
        }
        size = r_u32(particle + 0x18u);
        vertex.vx = (sint16)x;
        vertex.vy = (sint16)y;
        vertex.vz = (sint16)z;
        depth = (uint32)gte_project_full_depth(&vertex, &screen, NULL) << (shifts & 31u);
        if ((sint32)depth <= 0)
            continue;
        numerator = size * 64000u;
        depth = game_particle_add_checked(depth, 2u, 0x800CBB9Cu);
        divisor = (uint32)((sint32)depth >> 1);
        if (divisor == 0u)
            xport_mips_break(7u);
        if (divisor == 0xFFFFFFFFu && numerator == 0x80000000u)
            xport_mips_break(6u);
        quotient = (sint32)(uint32)((sint64)(sint32)numerator / (sint32)divisor);
        size = (uint32)(quotient >> 9);
        depth = game_particle_add_checked(depth, 0xFFFFE000u, 0x800CBBD4u);
        if ((sint32)depth <= 0)
            depth = 1u;
        value = game_particle_add_checked(size, 0xFFFFFFC0u, 0x800CBBE4u);
        if ((sint32)value >= 0)
            size = 64u;
        half_size = (uint32)((sint32)size >> 1) << 16;
        left_top = game_particle_sub_checked((uint32)screen, size, 0x800CBC04u);
        left_top = game_particle_sub_checked(left_top, half_size, 0x800CBC08u);
        right_top = game_particle_add_checked((uint32)screen, size, 0x800CBC0Cu);
        right_top = game_particle_sub_checked(right_top, half_size, 0x800CBC10u);
        left_bottom = game_particle_sub_checked((uint32)screen, size, 0x800CBC14u);
        left_bottom = game_particle_add_checked(left_bottom, half_size, 0x800CBC18u);
        right_bottom = game_particle_add_checked((uint32)screen, size, 0x800CBC1Cu);
        right_bottom = game_particle_add_checked(right_bottom, half_size, 0x800CBC20u);
        texture[0] = r_u16(colors);
        texture[1] = r_u16(colors + 2u);
        texture[2] = r_u16(colors + 4u);
        texture[3] = r_u16(colors + 6u);
        texture[4] = r_u16(colors + 8u);
        texture[5] = r_u16(colors + 10u);
        packet = r_u32(0x80040690u);
        w_u32(packet + 4u, color);
        w_u8(packet + 7u, 0x2Eu);
        w_u8(packet + 3u, 9u);
        w_u32(packet + 8u, left_top);
        w_u32(packet + 16u, right_top);
        w_u32(packet + 24u, left_bottom);
        w_u32(packet + 32u, right_bottom);
        w_u16(packet + 12u, texture[0]);
        w_u16(packet + 20u, texture[1]);
        w_u16(packet + 28u, texture[2]);
        w_u16(packet + 36u, texture[3]);
        w_u16(packet + 14u, texture[4]);
        w_u16(packet + 22u, texture[5]);
        ordering = r_u32(0x800421ACu);
        if ((sint32)depth >= 16384) {
            value = game_particle_sub_checked(depth, 0x3FFFFEu, 0x800CBCA8u);
            if ((sint32)value > 0)
                depth = 0x3FFFFEu;
            value = game_particle_add_checked(depth >> 11, 320u, 0x800CBCC0u);
            ordering = game_particle_add_checked(value << 2, ordering, 0x800CBCC8u);
        } else {
            if ((sint32)depth < 0)
                depth = 0u;
            value = game_particle_add_checked(depth >> 6, 64u, 0x800CBCE4u);
            ordering = game_particle_add_checked(value << 2, ordering, 0x800CBCECu);
        }
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000u) | (head & 0xFFFFFFu));
        head = r_u32(ordering);
        w_u32(ordering, (head & 0xFF000000u) | (packet & 0xFFFFFFu));
        pointer = r_u32(0x80040690u);
        pointer = game_particle_add_checked(pointer, 40u, 0x800CBD34u);
        w_u32(0x80040690u, pointer);
    }
}


void game_draw_queued_scene_messages(void)
{
    uint32 head, offset, record, timer, processed, count, value, next;
    sint32 initial_count;
    uint32 y;
    FUNCTION_MARKER(0x800875A0, "GAME.OVL");
    if (r_u32(0x800EC884u) != 0u)
        return;
    if (r_u32(0x800595C8u) != 0u)
        return;
    processed = 0u;
    head = (uint32)r_s16(0x800F3C6Cu);
    w_u32(0x80033D2Cu, 242u);
    initial_count = r_s16(0x800F3C68u);
    w_u8(0x8004617Cu, 128u);
    w_u8(0x80033D38u, 128u);
    w_u8(0x80053680u, 128u);
    y = 105u - (uint32)initial_count * 6u;
    if (initial_count <= 0)
        return;
    offset = head * 122u;
    do {
        record = 0x801268B0u + offset;
        timer = 0x80126928u + offset;
        value = r_u32(0x800311C0u);
        w_u32(0x80053688u, 10u);
        w_u32(0x8005368Cu, y);
        if (value != 0u)
            w_u32(0x8005368Cu, y + (uint32)((sint32)y >> 4));
        font_draw_current((const char *)psx_addr(record, 120u));
        value = r_u16(timer) - 1u;
        w_u16(record + 120u, (uint16)value);
        if (r_s16(timer) <= 0) {
            --processed;
            count = r_u16(0x800F3C68u);
            next = r_u16(0x800F3C6Cu) + 1u;
            --count;
            w_u16(0x800F3C6Cu, (uint16)next);
            w_u16(0x800F3C68u, (uint16)count);
            if ((sint16)(uint16)next >= 4)
                w_u16(0x800F3C6Cu, 0u);
        }
        y += 6u;
        ++processed;
        ++head;
        offset += 122u;
        if ((sint32)head >= 4) {
            head = 0u;
            offset = 0u;
        }
        initial_count = r_s16(0x800F3C68u);
    } while ((sint32)processed < initial_count);
}


void game_update_timed_point_effects(uint32 emit_particles)
{
    uint32 index, slot, seed, state, mask, parameter, vx, vy, vz, x, y, z;
    sint32 lifetime;
    FUNCTION_MARKER(0x800C2488, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    parameter = r_u32(0x80040C94u);
    game_draw_timed_point_effects(1u, 0x8011A3E8u, 256u, parameter);
    if (emit_particles == 0u)
        return;
    for (index = 0u; index < 256u; ++index) {
        slot = 0x8011A3E8u + index * 76u;
        lifetime = r_s16(slot);
        if (lifetime <= 0)
            continue;
        if (lifetime >= 60) {
            seed = r_u32(0x800EF50Cu);
            state = (uint32)((sint32)seed >> 1) ^ (seed << 3);
            w_u32(0x800EF50Cu, state);
            if ((state & 15u) == 0u)
                goto timed_point_emit;
        }
        if (r_s16(slot) != 1)
            continue;
        seed = r_u32(0x800EF50Cu);
        state = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        w_u32(0x800EF50Cu, state);
        if ((state & 31u) != 31u)
            continue;
    timed_point_emit:
        seed = r_u32(0x800EF50Cu);
        mask = (uint32)r_s16(slot + 6u);
        state = (uint32)((sint32)seed >> 1) ^ (seed << 3);
        parameter = (state & (mask & 0x7FFFu)) + 500u;
        w_u32(0x800EF50Cu, state);
        game_emit_timed_point_effect(slot + 0x28u, parameter);
        vx = (uint32)r_s16(slot + 0x38u) << 2;
        vy = (uint32)r_s16(slot + 0x3Au) << 2;
        vz = (uint32)r_s16(slot + 0x3Cu) << 2;
        x = r_u32(slot + 0x28u);
        y = r_u32(slot + 0x2Cu);
        z = r_u32(slot + 0x30u);
        game_emit_narrow_particles(1u, x, y, z, vx, vy, vz);
    }
}
