#include "game_functions.h"

void game_emit_timed_point_effect(uint32 position, uint32 size)
{
    FUNCTION_MARKER(0x800C2468, "GAME.OVL");
    game_emit_sized_fragment_flash(position, size);
}

uint32 game_initialize_mesh_effect_pool(void)
{
    uint32 node, previous, table, index;
    FUNCTION_MARKER(0x800BB944, "GAME.OVL");
    node = 0x80107A08u;
    previous = 0u;
    while (node < 0x801088A8u) {
        w_u32(node + 0x14u, previous);
        previous = node;
        w_u32(node + 0x18u, 0u);
        w_u32(node + 0x1Cu, 0u);
        w_u32(node + 0x10u, node + 48u);
        node += 48u;
    }
    w_u32(node + 0x14u, previous);
    w_u32(node + 0x10u, 0u);
    w_u32(0x801094B0u, 0x80107A08u);
    w_u32(0x80108A48u, 0u);
    table = 0x800FB404u;
    for (index = 0u; index < 10u; ++index) {
        w_u32(table, 0u);
        table -= 4u;
    }
    w_u32(0x80108A2Cu, 0u);
    w_u32(0x80126D60u, 0u);
    return 0x80107A08u;
}

sint32 game_bind_mesh_effect_texture(uint32 texture, uint32 slot)
{
    uint32 source, destination, value;
    sint32 result;
    FUNCTION_MARKER(0x800BB828, "GAME.OVL");
    source = 0x8004EB70u + (texture << 4);
    destination = slot << 2;
    value = (uint32)r_s16(source);
    w_u32(0x800F3C70u + destination, value);
    value = (uint32)r_s16(source + 2u);
    w_u32(0x800F3D18u + destination, value);
    value = (uint32)r_s16(source + 8u) << 2;
    w_u32(0x800F3BE8u + destination, value);
    value = (uint32)r_s16(source + 10u);
    w_u32(0x800F32F0u + destination, value);
    value = r_u32(0x800F3C70u + destination);
    w_u32(0x80126AA0u + destination, (value & 63u) << 2);
    value = r_u8(0x800F3D18u + destination);
    w_u32(0x80126B20u + destination, value);
    value = (uint32)r_s16(source + 4u);
    w_u32(0x800F31C8u + destination, value);
    result = r_s16(source + 6u);
    w_u32(0x800F3248u + destination, (uint32)result);
    return result;
}

void game_draw_mesh_effect_quad(uint32 first, uint32 second, uint32 third, uint32 fourth, uint32 texture, uint32 color, uint32 transparent)
{
    uint32 packet, offset, value, other, page_x, page_y, palette_x, palette_y, ordering;
    sint32 projection, flags;
    POLY_FT4 *primitive;
    FUNCTION_MARKER(0x800BB600, "GAME.OVL");
    packet = r_u32(0x80040690u);
    w_u32(0x80040690u, packet + 40u);
    w_u32(packet + 4u, color);
    RotTransPers((SVECTOR *)psx_addr(first, 8u), (sint32 *)psx_addr(0x800FA268u, 4u), &projection, &flags);
    value = r_u32(0x800FA268u);
    w_u32(packet + 8u, value);
    RotTransPers((SVECTOR *)psx_addr(second, 8u), (sint32 *)psx_addr(0x800FA268u, 4u), &projection, &flags);
    value = r_u32(0x800FA268u);
    w_u32(packet + 16u, value);
    RotTransPers((SVECTOR *)psx_addr(third, 8u), (sint32 *)psx_addr(0x800FA268u, 4u), &projection, &flags);
    value = r_u32(0x800FA268u);
    w_u32(packet + 24u, value);
    RotTransPers((SVECTOR *)psx_addr(fourth, 8u), (sint32 *)psx_addr(0x800FA268u, 4u), &projection, &flags);
    value = r_u32(0x800FA268u);
    w_u32(packet + 32u, value);
    primitive = (POLY_FT4 *)psx_addr(packet, 40u);
    SetPolyFT4(primitive);
    offset = texture << 2;
    value = r_u32(0x80126AA0u + offset);
    w_u8(packet + 12u, (uint8)value);
    value = r_u32(0x80126B20u + offset);
    w_u8(packet + 13u, (uint8)value);
    value = r_u32(0x800F3BE8u + offset);
    other = r_u32(0x80126AA0u + offset);
    w_u8(packet + 20u, (uint8)(other + value - 1u));
    value = r_u32(0x80126B20u + offset);
    w_u8(packet + 21u, (uint8)value);
    value = r_u32(0x80126AA0u + offset);
    w_u8(packet + 28u, (uint8)value);
    value = r_u32(0x800F32F0u + offset);
    other = r_u32(0x80126B20u + offset);
    w_u8(packet + 29u, (uint8)(other + value - 1u));
    value = r_u32(0x800F3BE8u + offset);
    other = r_u32(0x80126AA0u + offset);
    w_u8(packet + 36u, (uint8)(other + value - 1u));
    value = r_u32(0x800F32F0u + offset);
    other = r_u32(0x80126B20u + offset);
    w_u8(packet + 37u, (uint8)(other + value - 1u));
    page_x = r_u32(0x800F3C70u + offset);
    page_y = r_u32(0x800F3D18u + offset);
    value = GetTPage(0, 1, (sint32)page_x, (sint32)page_y);
    w_u16(packet + 22u, (uint16)value);
    palette_x = r_u32(0x800F31C8u + offset);
    palette_y = r_u32(0x800F3248u + offset);
    value = GetClut((sint32)palette_x, (sint32)palette_y);
    w_u16(packet + 14u, (uint16)value);
    SetSemiTrans(primitive, transparent);
    ordering = r_u32(0x8004608Cu);
    AddPrim(psx_addr(ordering + 112u, 4u), primitive);
}


sint32 game_update_nearby_effect_audio(void)
{
    uint32 effect, camera, x, y, z, value, distance, current, voice, alert, flags, text, volume;
    sint32 type;
    FUNCTION_MARKER(0x800E19E4, "GAME.OVL");
    effect = r_u32(0x80124928u);
    w_u32(0x8011A0DCu, 0x7FFFFFFFu);
    w_u32(0x8011A0E0u, 0x7FFFFFFFu);
    w_u32(0x800F4EB8u, 0u);
    w_u32(0x800F4EF8u, 0u);
    while (effect != 0u) {
        if (r_s16(effect + 0x46u) != 0) {
            type = r_s16(effect + 0x3Cu);
            if (type != 0 && type != 100 && type != 105) {
                camera = r_u32(0x80040C94u);
                x = r_u32(effect);
                value = r_u32(camera + 8u);
                x = (uint32)((sint32)(x - value) >> 2);
                y = r_u32(effect + 4u);
                value = r_u32(camera + 12u);
                y = (uint32)((sint32)(y - value) >> 2);
                z = r_u32(effect + 8u);
                value = r_u32(camera + 16u);
                z = (uint32)((sint32)(z - value) >> 2);
                value = (sint32)x < 0 ? 0u - x : x;
                if ((sint32)value >= 16768)
                    goto nearby_effect_next;
                value = (sint32)y < 0 ? 0u - y : y;
                if ((sint32)value >= 16768)
                    goto nearby_effect_next;
                value = (sint32)z < 0 ? 0u - z : z;
                if ((sint32)value >= 16768)
                    goto nearby_effect_next;
                distance = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z) << 2;
                current = r_u32(0x8011A0DCu);
                if ((sint32)distance < (sint32)current) {
                    w_u32(0x8011A0DCu, distance);
                    w_u32(0x800F4EB8u, effect);
                } else {
                    current = r_u32(0x8011A0E0u);
                    if ((sint32)distance < (sint32)current) {
                        w_u32(0x8011A0E0u, distance);
                        w_u32(0x800F4EF8u, effect);
                    }
                }
            }
        }
    nearby_effect_next:
        effect = r_u32(effect + 0x48u);
    }
    if (r_u32(0x800F159Cu) == 0u) {
        voice = r_u32(0x800FD4F8u);
        sound_set_voice_volume(voice, 0u);
    }
    game_update_effect_audio_channel();
    game_update_first_nearby_effect_audio();
    game_update_second_nearby_effect_audio();
    alert = r_u32(0x800ECE68u);
    w_u32(0x80123F50u, 0u);
    w_u32(0x80123F54u, 0u);
    w_u32(0x80123F58u, 0u);
    w_u32(0x801089C8u, 0u);
    w_u32(0x800F279Cu, 0x7FFFFFFFu);
    w_u32(0x800F27A4u, 0x7FFFFFFFu);
    w_u32(0x800F2798u, 0u);
    w_u32(0x800F27A0u, 0u);
    w_u32(0x800F159Cu, 0u);
    if (alert != 0u && (r_u32(0x800311B8u) & 3u) < 2u && r_u32(0x800EC884u) == 0u) {
        flags = r_u32(0x800311C0u);
        w_u8(0x8004617Cu, 255u);
        w_u8(0x80033D38u, 48u);
        w_u8(0x80053680u, 48u);
        w_u32(0x80053688u, 203u);
        w_u32(0x80033D2Cu, 0u);
        w_u32(0x8005368Cu, 220u);
        if (flags != 0u)
            w_u32(0x8005368Cu, 233u);
        text = r_u32(0x800F245Cu);
        font_draw_current((const char *)psx_addr(text, 9u));
        voice = r_u32(0x800F3D14u);
        volume = r_u8(0x800118FCu);
    } else {
        voice = r_u32(0x800F3D14u);
        volume = 0u;
    }
    return sound_set_voice_volume(voice, volume);
}


void game_draw_environment_mesh(void)
{
    MATRIX rotation;
    sint32 screen[3], depths[3], flags, projection, last_depth;
    uint32 batch, depth_base, index, row, column, rows, next_row, next_column;
    uint32 first, second, third, fourth, first_color, second_color, third_color, fourth_color;
    uint32 packet, value, ordering;
    POLY_G4 *primitive;
    FUNCTION_MARKER(0x800E3C58, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    MulMatrix0((MATRIX *)psx_addr(0x80035060u, 20u),
        (MATRIX *)psx_addr(0x80123FE8u, 20u), &rotation);
    SetRotMatrix(&rotation);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    depth_base = r_u32(0x800FD718u);
    for (batch = 0u; batch < 341u; ++batch) {
        gte_project3_full_depth((const SVECTOR *)psx_addr(0x800F8210u + batch * 24u, 24u), screen, depths, &flags);
        for (index = 0u; index < 3u; ++index)
            w_u32(0x80033D50u + batch * 12u + index * 4u, (uint32)screen[index]);
        for (index = 0u; index < 3u; ++index)
            w_u16(depth_base + batch * 6u + index * 2u, (uint16)depths[index]);
        w_u16(0x80059660u + batch * 2u, (uint16)((uint32)flags >> 12));
    }
    last_depth = RotTransPers((SVECTOR *)psx_addr(0x800FA208u, 8u),
        (sint32 *)psx_addr(0x80034D4Cu, 4u), &projection, &flags);
    depth_base = r_u32(0x800FD718u);
    w_u16(depth_base + 0x7FEu, (uint16)((uint32)last_depth << 2));
    rows = r_s16(0x800595F2u) != 0 ? 23u : 31u;
    for (row = 0u; row < rows; ++row) {
        next_row = (row + 1u) & 31u;
        for (column = 0u; column < 32u; ++column) {
            first = row * 32u + column;
            depth_base = r_u32(0x800FD718u);
            if (r_s16(depth_base + first * 2u) < 2401)
                continue;
            next_column = (column + 1u) & 31u;
            second = row * 32u + next_column;
            third = next_row * 32u + column;
            fourth = next_row * 32u + next_column;
            second_color = r_u32(0x80109B50u + second * 4u);
            first_color = r_u32(0x80109B50u + first * 4u);
            third_color = r_u32(0x80109B50u + third * 4u);
            fourth_color = r_u32(0x80109B50u + fourth * 4u);
            if ((sint32)(first_color | second_color | third_color | fourth_color) <= 0)
                continue;
            packet = r_u32(0x80040690u);
            w_u32(0x80040690u, packet + 36u);
            value = r_u32(0x80033D50u + first * 4u);
            w_u32(packet + 8u, value);
            value = r_u32(0x80033D50u + second * 4u);
            w_u32(packet + 16u, value);
            value = r_u32(0x80033D50u + third * 4u);
            w_u32(packet + 24u, value);
            value = r_u32(0x80033D50u + fourth * 4u);
            w_u32(packet + 4u, first_color);
            w_u32(packet + 12u, second_color);
            w_u32(packet + 20u, third_color);
            w_u32(packet + 28u, fourth_color);
            w_u32(packet + 32u, value);
            primitive = (POLY_G4 *)psx_addr(packet, 36u);
            SetPolyG4(primitive);
            ordering = r_u32(0x8004608Cu);
            AddPrim(psx_addr(ordering + 0x45ACu, 4u), primitive);
        }
    }
}


void game_update_effect_audio_channel(void)
{
    VECTOR position;
    uint32 player, voice, volume, sum, numerator, normalized_x, normalized_z, level, value;
    sint32 distance, length, master;
    FUNCTION_MARKER(0x800E07FC, "GAME.OVL");
    if (r_u32(0x801089C8u) == 0u)
        goto effect_audio_mute;
    position.vx = r_s32(0x80123F50u);
    position.vy = r_s32(0x80123F54u);
    player = r_u32(0x80040FDCu);
    position.vz = r_s32(0x80123F58u);
    ApplyMatrixLV((MATRIX *)psx_addr(player + 0x11Cu, 20u), &position, &position);
    if (position.vx > 65535 || position.vy > 65535 || position.vz > 65535)
        goto effect_audio_mute;
    distance = vector_length_approx(position.vx, position.vy, position.vz);
    if (distance == 0) {
        voice = r_u32(0x800FD720u);
        master = r_s16(0x800118FCu);
        sound_set_voice_left(voice, 15u);
        voice = r_u32(0x80108A28u);
        sound_set_voice_left(voice, 15u);
        volume = (uint32)master & 255u;
    } else {
        sum = (uint32)position.vx * (uint32)position.vx;
        sum += (uint32)position.vy * (uint32)position.vy;
        sum += (uint32)position.vz * (uint32)position.vz;
        length = (sint32)SquareRoot0((sint32)sum);
        if (length == 0)
            length = 1;
        numerator = (uint32)position.vx << 12;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        normalized_x = (uint32)((sint64)(sint32)numerator / length);
        numerator = (uint32)position.vz << 12;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        normalized_z = (uint32)((sint64)(sint32)numerator / length);
        level = (uint32)sound_offset_to_pan((sint32)normalized_x) & 255u;
        voice = r_u32(0x800FD720u);
        sound_set_voice_left(voice, level);
        voice = r_u32(0x80108A28u);
        sound_set_voice_left(voice, level);
        level = (uint32)sound_offset_to_pan((sint32)normalized_z) & 255u;
        voice = r_u32(0x800FD720u);
        sound_set_voice_right(voice, level);
        voice = r_u32(0x80108A28u);
        sound_set_voice_right(voice, level);
        if (r_s32(0x801089C8u) >= 256)
            w_u32(0x801089C8u, 255u);
        master = r_s16(0x800118FCu);
        value = r_u32(0x801089C8u);
        value = (uint32)master * value;
        volume = (uint32)((sint32)value >> 8) & 255u;
    }
    voice = r_u32(0x800FD720u);
    sound_set_voice_volume(voice, volume);
    voice = r_u32(0x80108A28u);
    sound_set_voice_volume(voice, volume);
    return;
 effect_audio_mute:
    voice = r_u32(0x800FD720u);
    sound_set_voice_volume(voice, 0u);
    voice = r_u32(0x80108A28u);
    sound_set_voice_volume(voice, 0u);
}


void game_update_first_nearby_effect_audio(void)
{
    VECTOR position;
    uint32 channel, target_slot, voice_slot, camera, target, player, value, old, cx, cy, cz;
    uint32 dx, dy, dz, sum, previous_distance, current_distance, pitch, voice, volume;
    uint32 numerator, normalized_x, normalized_z, level, gain;
    sint32 distance, length, master;
    FUNCTION_MARKER(0x800E0ABC, "GAME.OVL");
    for (channel = 0u; channel < 2u; ++channel) {
        voice_slot = 0x800F4E98u + channel * 4u;
        volume = 0u;
        if (r_s32(0x800F279Cu + channel * 8u) > 32768)
            goto nearby_audio_volume;
        if (r_u32(0x800EC884u) != 0u)
            goto nearby_audio_volume;
        target_slot = 0x800F2798u + channel * 8u;
        camera = r_u32(0x80040C94u);
        value = r_u32(camera + 8u);
        old = r_u32(camera + 0xFCu);
        cx = value - old;
        value = r_u32(camera + 12u);
        old = r_u32(camera + 0x100u);
        cy = value - old;
        value = r_u32(camera + 16u);
        old = r_u32(camera + 0x104u);
        target = r_u32(target_slot);
        cz = value - old;
        value = r_u32(target + 8u);
        old = r_u32(target + 0xFCu);
        dx = (uint32)((sint32)(value - old - cx) >> 1);
        value = r_u32(target + 12u);
        old = r_u32(target + 0x100u);
        dy = (uint32)((sint32)(value - old - cy) >> 1);
        value = r_u32(target + 16u);
        old = r_u32(target + 0x104u);
        dz = (uint32)((sint32)(value - old - cz) >> 1);
        sum = dx * dx + dy * dy + dz * dz;
        previous_distance = SquareRoot0((sint32)sum);
        target = r_u32(target_slot);
        camera = r_u32(0x80040C94u);
        value = r_u32(target + 8u);
        old = r_u32(camera + 8u);
        dx = (uint32)((sint32)(value - old) >> 1);
        value = r_u32(target + 12u);
        old = r_u32(camera + 12u);
        dy = (uint32)((sint32)(value - old) >> 1);
        old = r_u32(camera + 16u);
        value = r_u32(target + 16u);
        dz = (uint32)((sint32)(value - old) >> 1);
        sum = dx * dx + dy * dy + dz * dz;
        current_distance = SquareRoot0((sint32)sum);
        pitch = (uint32)((sint32)(previous_distance - current_distance) >> 4) + 200u;
        if ((sint32)pitch < 10)
            pitch = 10u;
        voice = r_u32(voice_slot);
        sound_set_voice_pitch(voice, pitch);
        player = r_u32(0x80040FDCu);
        position.vx = (sint32)dx >> 1;
        position.vz = (sint32)dz >> 1;
        position.vy = (sint32)dy >> 1;
        ApplyMatrixLV((MATRIX *)psx_addr(player + 0x11Cu, 20u), &position, &position);
        distance = vector_length_approx(position.vx, position.vy, position.vz);
        w_u32(0x80046174u, (uint32)distance);
        if (distance == 0)
            goto nearby_audio_center;
        sum = (uint32)position.vx * (uint32)position.vx;
        sum += (uint32)position.vy * (uint32)position.vy;
        sum += (uint32)position.vz * (uint32)position.vz;
        length = (sint32)SquareRoot0((sint32)sum);
        if (length == 0)
            length = 1;
        numerator = (uint32)position.vx << 12;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        normalized_x = (uint32)((sint64)(sint32)numerator / length);
        distance = r_s32(0x80046174u);
        numerator = (uint32)position.vz << 12;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        normalized_z = (uint32)((sint64)(sint32)numerator / length);
        if (distance == 0)
            goto nearby_audio_center;
        level = (uint32)sound_offset_to_pan((sint32)normalized_x) & 255u;
        voice = r_u32(voice_slot);
        sound_set_voice_left(voice, level);
        distance = r_s32(0x80046174u);
        (void)distance;
        level = (uint32)sound_offset_to_pan((sint32)normalized_z) & 255u;
        voice = r_u32(voice_slot);
        sound_set_voice_right(voice, level);
        gain = 32768u - (current_distance << 1);
        if ((sint32)gain < 0)
            gain = 0u;
        gain = (uint32)((sint32)gain >> 8);
        if ((sint32)gain >= 256)
            gain = 255u;
        master = r_s16(0x800118FCu);
        value = (uint32)master * gain;
        value = (uint32)((sint32)value >> 8);
        value *= 170u;
        volume = (uint32)((sint32)value >> 8) & 255u;
        goto nearby_audio_volume;
    nearby_audio_center:
        master = r_s16(0x800118FCu);
        voice = r_u32(voice_slot);
        value = (uint32)master * 85u;
        volume = (uint32)((sint32)value >> 7) & 255u;
        sound_set_voice_left(voice, 15u);
    nearby_audio_volume:
        voice = r_u32(voice_slot);
        sound_set_voice_volume(voice, volume);
    }
}

void game_update_second_nearby_effect_audio(void)
{
    VECTOR position;
    uint32 channel, target_slot, voice_slot, camera, target, player, value, old, cx, cy, cz;
    uint32 dx, dy, dz, sum, previous_distance, current_distance, pitch, voice, volume;
    uint32 numerator, normalized_x, normalized_z, level, gain;
    sint32 distance, length, master;
    FUNCTION_MARKER(0x800E1250, "GAME.OVL");
    for (channel = 0u; channel < 2u; ++channel) {
        voice_slot = 0x801193F0u + channel * 4u;
        volume = 0u;
        if (r_s32(0x8011A0DCu + channel * 4u) > 32768)
            goto nearby_audio_volume;
        if (r_u32(0x800EC884u) != 0u)
            goto nearby_audio_volume;
        target_slot = 0x800F4EB8u + channel * 64u;
        camera = r_u32(0x80040C94u);
        value = r_u32(camera + 8u);
        old = r_u32(camera + 0xFCu);
        cx = value - old;
        value = r_u32(camera + 12u);
        old = r_u32(camera + 0x100u);
        cy = value - old;
        value = r_u32(camera + 16u);
        old = r_u32(camera + 0x104u);
        target = r_u32(target_slot);
        cz = value - old;
        value = r_u32(target);
        old = r_u32(target + 0x20u);
        dx = (uint32)((sint32)(value - old - cx) >> 1);
        value = r_u32(target + 4u);
        old = r_u32(target + 0x24u);
        dy = (uint32)((sint32)(value - old - cy) >> 1);
        value = r_u32(target + 8u);
        old = r_u32(target + 0x28u);
        dz = (uint32)((sint32)(value - old - cz) >> 1);
        sum = dx * dx + dy * dy + dz * dz;
        previous_distance = SquareRoot0((sint32)sum);
        target = r_u32(target_slot);
        camera = r_u32(0x80040C94u);
        value = r_u32(target);
        old = r_u32(camera + 8u);
        dx = (uint32)((sint32)(value - old) >> 1);
        value = r_u32(target + 4u);
        old = r_u32(camera + 12u);
        dy = (uint32)((sint32)(value - old) >> 1);
        old = r_u32(camera + 16u);
        value = r_u32(target + 8u);
        dz = (uint32)((sint32)(value - old) >> 1);
        sum = dx * dx + dy * dy + dz * dz;
        current_distance = SquareRoot0((sint32)sum);
        pitch = (uint32)((sint32)(previous_distance - current_distance) >> 3) + 200u;
        if ((sint32)pitch < 10)
            pitch = 10u;
        voice = r_u32(voice_slot);
        sound_set_voice_pitch(voice, pitch);
        player = r_u32(0x80040FDCu);
        position.vx = (sint32)dx >> 1;
        position.vz = (sint32)dz >> 1;
        position.vy = (sint32)dy >> 1;
        ApplyMatrixLV((MATRIX *)psx_addr(player + 0x11Cu, 20u), &position, &position);
        distance = vector_length_approx(position.vx, position.vy, position.vz);
        w_u32(0x80046174u, (uint32)distance);
        if (distance == 0)
            goto nearby_audio_center;
        sum = (uint32)position.vx * (uint32)position.vx;
        sum += (uint32)position.vy * (uint32)position.vy;
        sum += (uint32)position.vz * (uint32)position.vz;
        length = (sint32)SquareRoot0((sint32)sum);
        if (length == 0)
            length = 1;
        numerator = (uint32)position.vx << 12;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        normalized_x = (uint32)((sint64)(sint32)numerator / length);
        distance = r_s32(0x80046174u);
        numerator = (uint32)position.vz << 12;
        if (length == -1 && numerator == 0x80000000u)
            xport_mips_break(0x5Du);
        normalized_z = (uint32)((sint64)(sint32)numerator / length);
        if (distance == 0)
            goto nearby_audio_center;
        level = (uint32)sound_offset_to_pan((sint32)normalized_x) & 255u;
        voice = r_u32(voice_slot);
        sound_set_voice_left(voice, level);
        distance = r_s32(0x80046174u);
        (void)distance;
        level = (uint32)sound_offset_to_pan((sint32)normalized_z) & 255u;
        voice = r_u32(voice_slot);
        sound_set_voice_right(voice, level);
        gain = 32768u - (current_distance << 1);
        if ((sint32)gain < 0)
            gain = 0u;
        gain = (uint32)((sint32)gain >> 8);
        if ((sint32)gain >= 256)
            gain = 255u;
        master = r_s16(0x800118FCu);
        value = (uint32)master * gain;
        value = (uint32)((sint32)value >> 8);
        value *= 170u;
        volume = (uint32)((sint32)value >> 8) & 255u;
        goto nearby_audio_volume;
    nearby_audio_center:
        master = r_s16(0x800118FCu);
        voice = r_u32(voice_slot);
        value = (uint32)master * 85u;
        volume = (uint32)((sint32)value >> 7) & 255u;
        sound_set_voice_left(voice, 15u);
    nearby_audio_volume:
        voice = r_u32(voice_slot);
        sound_set_voice_volume(voice, volume);
    }
}


void game_mute_scene_effect_voices(void)
{
    static const uint32 voices[8] = {0x800FD720u, 0x80108A28u, 0x800F4E98u, 0x800F4E9Cu,
        0x801193F0u, 0x801193F4u, 0x800FD4F8u, 0x800F3D14u};
    uint32 index, voice;
    FUNCTION_MARKER(0x800E1CCC, "GAME.OVL");
    for (index = 0u; index < 8u; ++index) {
        voice = r_u32(voices[index]);
        sound_set_voice_volume(voice, 0u);
    }
}


void game_suspend_scene_engine_audio(void)
{
    uint32 voice, first, second;
    FUNCTION_MARKER(0x800DFFE8, "GAME.OVL");
    voice = r_u32(0x800F8100u);
    first = r_u32(0x80108A20u);
    second = r_u32(0x8010AC48u);
    w_u32(0x8010890Cu, first);
    w_u32(0x80109B40u, second);
    sound_set_voice_volume(voice, 0u);
    voice = r_u32(0x800FB438u);
    sound_set_voice_volume(voice, 0u);
    sound_tick();
}


void game_initialize_scene_engine_voices(void)
{
    sint32 sample, result;
    FUNCTION_MARKER(0x800DFF1C, "GAME.OVL");
    sample = r_s32(0x8004297Cu);
    result = sound_reserve_voice(sample, 0u, 15, 15, 256u, 0u);
    sample = r_s32(0x8004298Cu);
    w_u32(0x800F8100u, (uint32)result);
    result = sound_reserve_voice(sample, 0u, 15, 15, 256u, 0u);
    w_u32(0x800FB438u, (uint32)result);
    w_u32(0x80108A20u, 0u);
    w_u32(0x8010AC48u, 0u);
}


void game_initialize_scene_effect_voices(void)
{
    static const uint32 samples[8] = {0x80042988u, 0x80042988u, 0x80042958u, 0x80042958u,
        0x8004295Cu, 0x8004295Cu, 0x80042998u, 0x800429B0u};
    static const uint32 voices[8] = {0x800FD720u, 0x80108A28u, 0x800F4E98u, 0x800F4E9Cu,
        0x801193F0u, 0x801193F4u, 0x800FD4F8u, 0x800F3D14u};
    uint32 index, flags;
    sint32 sample, result = 0;
    FUNCTION_MARKER(0x800E02A4, "GAME.OVL");
    for (index = 0u; index < 8u; ++index) {
        sample = r_s32(samples[index]);
        flags = index < 6u ? r_u32(0x800313A0u) : 0u;
        if (index != 0u)
            w_u32(voices[index - 1u], (uint32)result);
        result = sound_reserve_voice(sample, 0u, 15, 15, index < 6u ? 128u : 255u, flags);
    }
    w_u32(voices[7], (uint32)result);
    game_initialize_scene_engine_voices();
    w_u32(0x80123F50u, 0u);
    w_u32(0x80123F54u, 0u);
    w_u32(0x80123F58u, 0u);
    w_u32(0x801089C8u, 0u);
}


void game_initialize_pause_grid(void)
{
    uint32 row, column, offset, x, y;
    FUNCTION_MARKER(0x8008A2A4, "GAME.OVL");
    y = 0u - 32u;
    for (row = 0u; row < 18u; ++row) {
        x = 0u - 42u;
        for (column = 0u; column < 11u; ++column) {
            offset = row * 88u + column * 8u;
            w_u32(0x800F2B74u + offset, x);
            x += 32u;
            w_u32(0x800F2B70u + offset, y);
        }
        y += 32u;
    }
}


void game_process_pause_request(void)
{
    uint32 input, owner, saved_mode, state, offset, x, y, z, extra, sample_flags, camera;
    uint32 velocity_x, velocity_y, velocity_z, finished, voice;
    sint32 sample, first_voice, second_voice;
    FUNCTION_MARKER(0x800879FC, "GAME.OVL");
    input = r_u32(0x80031138u);
    w_u32(0x800FB5C0u, 0u);
    if ((input & 3u) != 0u || r_u32(0x800EEF54u) != 0u)
        return;
    if (r_u8(0x800313D4u) == 0u && r_u32(0x80040FE8u) == 12u && (r_u32(0x80053684u) & 0x800u) == 0u)
        return;
    game_mute_scene_effect_voices();
    game_suspend_scene_engine_audio();
    sound_reset_voices();
    w_u32(0x800FB5C0u, 1u);
    disc_wait_valid();
    w_u32(0x800FB5C0u, 2u);
    light_set_scaled_color(0u, 128u, 128u, 128u);
    light_set_scaled_color(1u, 128u, 128u, 128u);
    light_set_scaled_color(2u, 128u, 128u, 128u);
    owner = r_u32(0x8003FE74u);
    w_u32(0x800FB5C0u, 3u);
    saved_mode = r_u32(owner + 0x84u);
    w_u32(0x800F3D10u, 0u);
    w_u32(0x8001020Cu, 1u);
    w_u32(0x80053684u, 0u);
    w_u32(owner + 0x84u, 0xFFFFFFFEu);
    state = r_u32(0x80040FE4u);
    w_u32(0x80040FF0u, 1u);
    while (state != 8u) {
        VSync(0);
        state = r_u32(0x80040FE4u);
    }
    w_u32(0x800FB5C0u, 4u);
    for (offset = 0u; offset < 0xF10u; offset += 16u) {
        x = r_u32(0x80040FE0u + offset);
        y = r_u32(0x80040FE4u + offset);
        z = r_u32(0x80040FE8u + offset);
        extra = r_u32(0x80040FECu + offset);
        w_u32(0x800FA468u + offset, x);
        w_u32(0x800FA46Cu + offset, y);
        w_u32(0x800FA470u + offset, z);
        w_u32(0x800FA474u + offset, extra);
    }
    x = r_u32(0x80041EF0u);
    y = r_u32(0x80041EF4u);
    w_u32(0x800FB378u, x);
    w_u32(0x800FB37Cu, y);
    stream_clear_queue();
    stream_cancel_current();
    state = r_u32(0x80040FE4u);
    w_u32(0x800FB5C0u, 5u);
    w_u32(0x80040FF0u, 0u);
    while (state == 8u) {
        VSync(0);
        state = r_u32(0x80040FE4u);
    }
    sample = r_s32(0x80042970u);
    sample_flags = r_u32(0x800313A0u);
    w_u32(0x800FB5C0u, 6u);
    first_voice = sound_reserve_voice(sample, 50u, 15, 15, 256u, sample_flags);
    sample = r_s32(0x80042970u);
    sample_flags = r_u32(0x800313A0u);
    w_u32(0x800ECF18u, (uint32)first_voice);
    second_voice = sound_reserve_voice(sample, 50u, 15, 15, 137u, sample_flags);
    camera = r_u32(0x80040C94u);
    velocity_x = r_u32(camera + 0x14u);
    velocity_y = r_u32(camera + 0x18u);
    w_u32(0x800ECF1Cu, (uint32)second_voice);
    velocity_z = r_u32(camera + 0x1Cu);
    w_u32(0x800FB5C0u, 7u);
    w_u32(camera + 0x14u, 0u);
    w_u32(camera + 0x18u, 0u);
    w_u32(camera + 0x1Cu, 0u);
    game_prepare_pause_scene();
    game_initialize_pause_grid();
    game_run_pause_menu();
    voice = r_u32(0x800ECF18u);
    sound_stop_voice(voice);
    sound_tick();
    voice = r_u32(0x800ECF1Cu);
    sound_stop_voice(voice);
    game_initialize_scene_effect_voices();
    finished = r_u32(0x800ECEA8u);
    w_u32(0x8001020Cu, 0u);
    if (finished == 0u) {
        camera = r_u32(0x80040C94u);
        owner = r_u32(0x8003FE74u);
        w_u32(camera + 0x14u, velocity_x);
        w_u32(camera + 0x18u, velocity_y);
        w_u32(camera + 0x1Cu, velocity_z);
        w_u32(owner + 0x84u, saved_mode);
        light_set_color(0u, 0u, 0u, 0u);
        light_set_color(1u, 0u, 0u, 0u);
        light_set_color(2u, 0u, 0u, 0u);
        game_refresh_mission_state();
    }
    state = r_u32(0x80040FE4u);
    w_u32(0x80040FF0u, 1u);
    while (state != 8u) {
        VSync(0);
        state = r_u32(0x80040FE4u);
    }
    for (offset = 0u; offset < 0xF10u; offset += 16u) {
        x = r_u32(0x800FA468u + offset);
        y = r_u32(0x800FA46Cu + offset);
        z = r_u32(0x800FA470u + offset);
        extra = r_u32(0x800FA474u + offset);
        w_u32(0x80040FE0u + offset, x);
        w_u32(0x80040FE4u + offset, y);
        w_u32(0x80040FE8u + offset, z);
        w_u32(0x80040FECu + offset, extra);
    }
    x = r_u32(0x800FB378u);
    y = r_u32(0x800FB37Cu);
    w_u32(0x80041EF0u, x);
    w_u32(0x80041EF4u, y);
    w_u8(0x800313D4u, 0u);
    w_u32(0x80040FF0u, 0u);
}


void game_prepare_pause_scene(void)
{
    uint32 x, y, z, radius, row, column, latitude, longitude, vertex, cosine, sine, product, circle;
    FUNCTION_MARKER(0x8008B874, "GAME.OVL");
    w_u32(0x8011F120u, 0u);
    w_u32(0x8011F124u, 0u);
    w_u32(0x8011F128u, 0u);
    matrix_set_identity_native((MATRIX *)psx_addr(0x8011A3C0u, 18u));
    radius = 0x3624u;
    w_u32(0x800F4010u, 0xFFFFDCD8u);
    w_u32(0x800F4014u, 0u);
    w_u32(0x800F4018u, 0xA7F8u);
    matrix_set_identity_native((MATRIX *)psx_addr(0x800F3CF0u, 18u));
    x = (uint32)r_s16(0x8011A3CCu);
    y = (uint32)r_s16(0x8011A3CEu);
    z = (uint32)r_s16(0x8011A3D0u);
    light_set_direction(0u, x, y, z);
    if (r_u32(0x800311C0u) != 0u)
        radius = 0x2F60u;
    latitude = 0u - 1024u;
    for (row = 0u; row < 16u; ++row) {
        longitude = 0u;
        for (column = 0u; column < 16u; ++column) {
            cosine = (uint32)rcos((sint32)latitude);
            sine = (uint32)rsin((sint32)longitude);
            product = radius * cosine;
            circle = (uint32)((sint32)product >> 12);
            product = circle * sine;
            vertex = 0x800F33E0u + row * 128u + column * 8u;
            w_u16(vertex, (uint16)((sint32)product >> 12));
            sine = (uint32)rsin((sint32)latitude);
            product = radius * sine;
            w_u16(vertex + 2u, (uint16)((sint32)product >> 12));
            cosine = (uint32)rcos((sint32)latitude);
            sine = (uint32)rcos((sint32)longitude);
            product = radius * cosine;
            circle = (uint32)((sint32)product >> 12);
            product = circle * sine;
            w_u16(vertex + 4u, (uint16)((sint32)product >> 12));
            longitude += 256u;
        }
        latitude += 136u;
    }
}


void game_refresh_mission_state(void)
{
    MATRIX identity;
    uint32 color, red, green, blue, index, offset, x, y, z, row, column;
    sint32 mission, count, mode;
    FUNCTION_MARKER(0x80092F5C, "GAME.OVL");
    mission = r_s16(0x800595F8u);
    w_u32(0x800311D0u, 0u);
    w_u32(0x800311D4u, 0u);
    w_u32(0x800311D8u, 0u);
    if ((uint32)mission <= 2u)
        color = 0x800ED440u;
    else if ((uint32)mission <= 4u)
        color = 0x800ED444u;
    else if ((uint32)mission <= 8u)
        color = 0x800ED448u;
    else if ((uint32)mission <= 12u)
        color = 0x800ED44Cu;
    else
        color = 0x800ED450u;
    red = r_u8(color);
    green = r_u8(color + 1u);
    blue = r_u8(color + 2u);
    SetBackColor((sint32)red, (sint32)green, (sint32)blue);
    red = r_u8(color);
    green = r_u8(color + 1u);
    blue = r_u8(color + 2u);
    w_u8(0x80042900u, (uint8)red);
    w_u8(0x80042901u, (uint8)green);
    w_u8(0x80042902u, (uint8)blue);
    for (row = 0u; row < 3u; ++row) {
        for (column = 0u; column < 3u; ++column)
            identity.m[row][column] = row == column ? 4096 : 0;
    }
    SetLightMatrix(&identity);
    count = r_s16(0x800595F0u);
    index = 0u;
    while ((sint32)index < count) {
        offset = index * 28u;
        mode = r_s16(0x80123F90u + offset);
        color = mode == 2 ? 0x800ED458u : (mode == 3 ? 0x800ED45Cu : 0x800ED454u);
        red = r_u8(color);
        green = r_u8(color + 1u);
        blue = r_u8(color + 2u);
        light_set_color(index, red, green, blue);
        x = (uint32)r_s16(0x80123F78u + offset);
        y = (uint32)r_s16(0x80123F7Au + offset);
        z = (uint32)r_s16(0x80123F7Cu + offset);
        light_set_direction(index, 0u - x, 0u - y, 0u - z);
        count = r_s16(0x800595F0u);
        ++index;
    }
    render_set_clear_color(0u, 0u, 0u);
}


uint32 game_count_pause_menu_entries(void)
{
    uint32 menu, count, value;
    FUNCTION_MARKER(0x80087F4C, "GAME.OVL");
    menu = r_u32(0x80124C00u);
    count = 0u;
    do {
        value = r_u32(menu + 0x30u + count * 28u);
        ++count;
    } while (value != 0xFFFFFFFFu);
    menu = r_u32(0x80124C00u);
    w_u32(menu + 4u, count - 1u);
    return count - 1u;
}


void game_initialize_pause_selection(uint32 menu)
{
    uint32 index, next, count;
    FUNCTION_MARKER(0x80087F9C, "GAME.OVL");
    w_u32(menu, 0u);
    for (;;) {
        index = r_u32(menu);
        if (r_u8(menu + index * 28u + 0x48u) == 1u)
            return;
        next = index + 1u;
        count = r_u32(menu + 4u) - 1u;
        w_u32(menu, next);
        if ((sint32)count < (sint32)next)
            diagnostic_report(666u, 0x80074678u, 0x80074690u);
    }
}


void game_run_pause_menu(void)
{
    uint32 menu, callback, flags, voice, sample_flags, value, old, current, text;
    uint32 x, y, z, next, next_depth, current_depth, latch;
    sint32 sample, first_voice, second_voice, amount;
    FUNCTION_MARKER(0x80088044, "GAME.OVL");
    w_u32(0x80124C00u, 0x800ECFB4u);
    w_u32(0x80108A44u, 32u);
    w_u32(0x800FB3BCu, 0u);
    game_initialize_pause_selection(0x800ECFB4u);
    for (;;) {
        w_u32(0x800F3D10u, 0u);
        w_u32(0x80124C48u, 0u);
        latch = 0u;
        game_count_pause_menu_entries();
        menu = r_u32(0x80124C00u);
        callback = r_u32(menu + 0x20u);
        psx_dispatch_guest_call(callback, 0u, 0u, 0u, 0u);
        flags = r_u32(0x80108A44u);
        w_u32(0x800FA26Cu, 0u);
        w_u32(0x800ECF14u, 0u);
        w_u32(0x80108A44u, flags ^ 4u);
        sound_play_sample(7u);
        flags = r_u32(0x80108A44u);
        w_u32(0x800ECED8u, 0u);
        while ((flags & 4u) != 0u) {
            if (r_u32(0x80040FE8u) != 12u) {
                voice = r_u32(0x800ECF18u);
                sound_stop_voice(voice);
                sound_tick();
                voice = r_u32(0x800ECF1Cu);
                sound_stop_voice(voice);
                disc_prompt();
                sample = r_s32(0x80042970u);
                sample_flags = r_u32(0x800313A0u);
                first_voice = sound_reserve_voice(sample, 50u, 15, 15, 256u, sample_flags);
                sample = r_s32(0x80042970u);
                sample_flags = r_u32(0x800313A0u);
                w_u32(0x800ECF18u, (uint32)first_voice);
                second_voice = sound_reserve_voice(sample, 50u, 15, 15, 137u, sample_flags);
                w_u32(0x800ECF1Cu, (uint32)second_voice);
            }
            x = (uint32)r_s16(0x8011A3CCu);
            y = (uint32)r_s16(0x8011A3CEu);
            z = (uint32)r_s16(0x8011A3D0u);
            light_set_direction(0u, x, y, z);
            value = r_u32(0x8004616Cu);
            w_u32(0x80053678u, value);
            current = (uint32)pad_update(0u);
            old = r_u32(0x80053678u);
            w_u32(0x8004616Cu, current);
            w_u32(0x80053684u, (old ^ current) & current);
            render_select_buffer(0x1160u);
            if (r_u8(0x8004EA28u) != 0u) {
                value = r_u32(0x800F3D10u);
                if (((value >> 5) & 1u) != 0u) {
                    flags = r_u32(0x800311C0u);
                    w_u32(0x80053688u, 39u);
                    w_u32(0x8005368Cu, 31u);
                    if (flags != 0u)
                        w_u32(0x8005368Cu, 32u);
                    text = r_u32(0x800F24C4u);
                    w_u8(0x8004617Cu, 128u);
                    w_u8(0x80033D38u, 128u);
                    w_u8(0x80053680u, 128u);
                    w_u32(0x80033D2Cu, 0u);
                    font_draw_current((const char *)psx_addr(text, 19u));
                }
            }
            menu = r_u32(0x80124C00u);
            callback = r_u32(menu + 0x28u);
            psx_dispatch_guest_call(callback, 0u, 0u, 0u, 0u);
            menu = r_u32(0x80124C00u);
            value = r_u8(menu + 8u);
            game_draw_pause_frame(value);
            system_read_counter_one();
            if (r_u32(0x800FB3BCu) != 0u) {
                menu = r_u32(0x80124C00u);
                callback = r_u32(menu + 0x24u);
                psx_dispatch_guest_call(callback, 0u, 0u, 0u, 0u);
            }
            menu = r_u32(0x80124C00u);
            if (r_u32(menu + 0x10u) == 0u) {
                flags = r_u32(0x80108A44u);
                if ((flags & 8u) != 0u) {
                    w_u32(0x800FB3BCu, 1u);
                    w_u32(0x80108A44u, flags ^ 12u);
                    flags = r_u32(0x80108A44u);
                }
                if ((flags & 16u) != 0u) {
                    w_u32(0x800FB3BCu, 1u);
                    w_u32(0x80108A44u, flags ^ 16u);
                }
            } else {
                flags = r_u32(0x80108A44u);
                if ((flags & 8u) != 0u) {
                    if (latch == 0u) {
                        next = r_u32(0x80124C48u);
                        if (next != 0x800ECFB4u || r_u32(next) != 0u || r_u32(0x800ECEA8u) != 0u) {
                            sound_play_sample(7u);
                            ++latch;
                        }
                    }
                    flags = r_u32(0x80108A44u);
                    if ((flags & 8u) != 0u) {
                        amount = r_s32(0x800ECF38u);
                        if (amount > 0) {
                            w_u32(0x800ECF38u, (uint32)amount - 1u);
                            x = r_u32(0x800ECF24u);
                            y = r_u32(0x800ECF34u);
                            w_u32(0x800ECF24u, x - 4u);
                            w_u32(0x800ECF34u, y - 4u);
                        }
                        goto pause_frame_done;
                    }
                }
                amount = r_s32(0x800ECF38u);
                if (amount < 20) {
                    w_u32(0x800ECF38u, (uint32)amount + 1u);
                    x = r_u32(0x800ECF24u);
                    y = r_u32(0x800ECF34u);
                    w_u32(0x800ECF24u, x + 4u);
                    w_u32(0x800ECF34u, y + 4u);
                }
            }
        pause_frame_done:
            value = r_u32(0x800F3D10u);
            flags = r_u32(0x80108A44u);
            w_u32(0x800F3D10u, value + 1u);
        }
        menu = r_u32(0x80124C00u);
        callback = r_u32(menu + 0x2Cu);
        psx_dispatch_guest_call(callback, 0u, 0u, 0u, 0u);
        next = r_u32(0x80124C48u);
        if (next == 0u)
            return;
        menu = r_u32(0x80124C00u);
        next_depth = r_u8(next + 9u);
        current_depth = r_u8(menu + 9u);
        if (next_depth >= current_depth)
            game_initialize_pause_selection(next);
        next = r_u32(0x80124C48u);
        w_u32(0x80124C00u, next);
    }
}


void game_draw_pause_frame(uint32 mode)
{
    uint32 volume, voice, frame, wave, pan, owner, packet, slot, result;
    uint16 page;
    FUNCTION_MARKER(0x80089D90, "GAME.OVL");
    render_check_lists();
    sound_tick();
    volume = (uint32)r_s16(0x800118FCu);
    voice = r_u32(0x800ECF18u);
    if ((sint32)volume < 0)
        volume += 3u;
    sound_set_voice_volume(voice, (volume >> 2) & 255u);
    volume = (uint32)r_s16(0x800118FCu);
    voice = r_u32(0x800ECF1Cu);
    if ((sint32)volume < 0)
        volume += 3u;
    sound_set_voice_volume(voice, (volume >> 2) & 255u);
    frame = r_u32(0x800F3D10u);
    wave = (uint32)rsin((sint32)(frame << 5));
    pan = (uint32)((sint32)(wave * 15u) >> 12) + 15u;
    voice = r_u32(0x800ECF18u);
    sound_set_voice_right(voice, pan & 255u);
    frame = r_u32(0x800F3D10u);
    wave = (uint32)rcos((sint32)(frame << 5));
    pan = (uint32)((sint32)(wave * 15u) >> 12) + 15u;
    voice = r_u32(0x800ECF18u);
    sound_set_voice_left(voice, pan & 255u);
    frame = r_u32(0x800F3D10u);
    wave = (uint32)rcos((sint32)(frame << 6));
    pan = (uint32)((sint32)(wave * 15u) >> 12) + 15u;
    voice = r_u32(0x800ECF1Cu);
    sound_set_voice_right(voice, pan & 255u);
    frame = r_u32(0x800F3D10u);
    wave = (uint32)rsin((sint32)(frame << 6));
    pan = (uint32)((sint32)(wave * 15u) >> 12) + 15u;
    voice = r_u32(0x800ECF1Cu);
    sound_set_voice_left(voice, pan & 255u);
    result = (uint32)VSync(1);
    w_u32(0x80042178u, result);
    DrawSync(0);
    VSync(mode == 30u ? 2 : 0);
    owner = r_u32(0x8004608Cu);
    PutDrawEnv((DRAWENV *)psx_addr(owner, sizeof(DRAWENV)));
    owner = r_u32(0x8004608Cu);
    PutDispEnv((DISPENV *)psx_addr(owner + 0x5Cu, sizeof(DISPENV)));
    packet = r_u32(0x80040690u);
    page = GetTPage(0, 1, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(packet, sizeof(DR_MODE)), 1, 1, page, NULL);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x45A8u, 4u), psx_addr(packet, 4u));
    slot = r_u32(0x80034D78u);
    owner = r_u32(0x8004608Cu);
    w_u32(0x80040690u, packet + 12u);
    DrawOTag((uint32 *)psx_addr(owner + (slot << 2) + 0x70u, 4u));
    result = (uint32)VSync(1);
    w_u32(0x8004EAF0u, result);
    DrawSyncCallback(render_draw_complete);
}


void game_pause_menu_leave(void)
{
    FUNCTION_MARKER(0x80087F3C, "GAME.OVL");
}


void game_begin_pause_menu_transition(void)
{
    uint32 menu, geometry, flags, values[8];
    FUNCTION_MARKER(0x8008ADBC, "GAME.OVL");
    menu = r_u32(0x80124C00u);
    w_u32(0x800FB3BCu, 0u);
    w_u32(menu + 0x10u, 7u);
    geometry = r_u32(menu + 0x1Cu);
    flags = r_u32(0x80108A44u) | 16u;
    values[0] = r_u32(geometry);
    values[1] = r_u32(geometry + 4u);
    values[2] = r_u32(geometry + 8u);
    values[3] = r_u32(geometry + 12u);
    values[4] = r_u32(geometry);
    values[5] = r_u32(geometry + 4u);
    w_u32(0x80108A44u, flags);
    w_u32(geometry + 24u, values[0]);
    w_u32(geometry + 28u, values[1]);
    w_u32(geometry + 32u, values[2]);
    w_u32(geometry + 36u, values[3]);
    w_u32(geometry + 16u, values[4]);
    w_u32(geometry + 20u, values[5]);
    if ((flags & 32u) == 0u)
        return;
    values[0] = r_u32(0x800ECF3Cu);
    values[1] = r_u32(0x800ECF40u);
    values[2] = r_u32(0x800ECF44u);
    values[3] = r_u32(0x800ECF48u);
    values[4] = r_u32(0x800ECF64u);
    values[5] = r_u32(0x800ECF68u);
    values[6] = r_u32(0x800ECF6Cu);
    values[7] = r_u32(0x800ECF70u);
    w_u32(0x80108A44u, flags ^ 32u);
    w_u32(0x800ECF54u, values[0]);
    w_u32(0x800ECF58u, values[1]);
    w_u32(0x800ECF5Cu, values[2]);
    w_u32(0x800ECF60u, values[3]);
    w_u32(0x800ECF4Cu, values[0]);
    w_u32(0x800ECF50u, values[1]);
    w_u32(0x800ECF7Cu, values[4]);
    w_u32(0x800ECF80u, values[5]);
    w_u32(0x800ECF84u, values[6]);
    w_u32(0x800ECF88u, values[7]);
    w_u32(0x800ECF74u, values[4]);
    w_u32(0x800ECF78u, values[5]);
}


void game_pause_menu_enter(void)
{
    FUNCTION_MARKER(0x8008AED8, "GAME.OVL");
    game_begin_pause_menu_transition();
}


void game_move_pause_selection(uint32 input, uint32 menu)
{
    uint32 initial, found, index, enabled, current, start, bound;
    FUNCTION_MARKER(0x80089FCC, "GAME.OVL");
    initial = r_u32(menu);
    found = 0u;
    if ((input & 0x1000u) != 0u) {
        do {
            index = r_u32(menu);
            if ((sint32)index > 0)
                w_u32(menu, index - 1u);
            else if (index == 0u) {
                index = r_u32(menu + 4u);
                w_u32(menu, index - 1u);
            }
            index = r_u32(menu);
            enabled = r_u8(menu + index * 28u + 0x48u);
            if (enabled == 1u || index == initial)
                found = 1u;
        } while (found == 0u);
        current = r_u32(0x80124C00u);
        index = r_u32(current);
        if (initial != index)
            sound_play_sample_pitch(10u, 128u);
    }
    input = r_u32(0x80053684u);
    if ((input & 0x4000u) != 0u) {
        current = r_u32(0x80124C00u);
        start = r_u32(current);
        if (found == 0u) {
            menu = current;
            bound = r_u32(menu + 4u) - 1u;
            do {
                index = r_u32(menu);
                if ((sint32)index < (sint32)bound)
                    w_u32(menu, index + 1u);
                else if (index == bound)
                    w_u32(menu, 0u);
                index = r_u32(menu);
                enabled = r_u8(menu + index * 28u + 0x48u);
                if (enabled == 1u || index == start)
                    found = 1u;
            } while (found == 0u);
            current = r_u32(0x80124C00u);
        }
        index = r_u32(current);
        if (initial != index)
            sound_play_sample_pitch(10u, 128u);
    }
}


void game_update_pause_selection(void)
{
    uint32 input, menu;
    FUNCTION_MARKER(0x80089FBC, "GAME.OVL");
    input = r_u32(0x80053684u);
    menu = r_u32(0x80124C00u);
    game_move_pause_selection(input, menu);
}


void game_handle_pause_menu_input(void)
{
    uint32 input, menu, index, entry, flags, callback, next;
    FUNCTION_MARKER(0x8008A160, "GAME.OVL");
    game_update_pause_selection();
    input = r_u32(0x80053684u);
    if ((input & 0x40u) != 0u)
        goto pause_confirm;
    if ((input & 0x10u) == 0u)
        return;
    menu = r_u32(0x80124C00u);
    if (menu == 0x800ED290u) {
    pause_confirm:
        menu = r_u32(0x80124C00u);
        index = r_u32(menu);
        w_u32(0x800FB3BCu, 0u);
        w_u32(menu + 0x10u, 4u);
        entry = menu + index * 28u + 0x30u;
        flags = r_u32(0x80108A44u);
        callback = r_u32(entry);
        w_u32(0x80108A44u, flags | 8u);
        psx_dispatch_guest_call(callback, 1, menu, 0u, 0u);
        next = r_u32(entry + 8u);
        w_u32(0x800ECF14u, 1u);
        w_u32(0x80124C48u, next);
        sound_play_sample_pitch(11u, 128u);
    } else if (menu != 0x800ECFB4u) {
        index = r_u32(menu);
        w_u32(0x800FB3BCu, 0u);
        w_u32(menu + 0x10u, 4u);
        entry = menu + index * 28u + 0x30u;
        flags = r_u32(0x80108A44u);
        callback = r_u32(entry + 4u);
        w_u32(0x80108A44u, flags | 8u);
        psx_dispatch_guest_call(callback, 1, menu, 0u, 0u);
        next = r_u32(entry + 12u);
        w_u32(0x80124C48u, next);
        sound_play_sample_pitch(11u, 128u);
    }
}


void game_swap_pause_menu_geometry(void)
{
    uint32 menu, geometry, values[6];
    FUNCTION_MARKER(0x8008AFE4, "GAME.OVL");
    menu = r_u32(0x80124C00u);
    geometry = r_u32(menu + 0x1Cu);
    values[0] = r_u32(geometry + 8u);
    values[1] = r_u32(geometry + 12u);
    values[2] = r_u32(geometry);
    values[3] = r_u32(geometry + 4u);
    values[4] = r_u32(geometry + 8u);
    values[5] = r_u32(geometry + 12u);
    w_u32(geometry + 24u, values[0]);
    w_u32(geometry + 28u, values[1]);
    w_u32(geometry + 32u, values[2]);
    w_u32(geometry + 36u, values[3]);
    w_u32(geometry + 16u, values[4]);
    w_u32(geometry + 20u, values[5]);
}


void game_swap_pause_menu_return_geometry(void)
{
    uint32 menu, geometry, values[6];
    FUNCTION_MARKER(0x8008B11C, "GAME.OVL");
    menu = r_u32(0x80124C00u);
    geometry = r_u32(menu + 0x1Cu);
    values[0] = r_u32(geometry + 8u);
    values[1] = r_u32(geometry + 12u);
    values[2] = r_u32(geometry);
    values[3] = r_u32(geometry + 4u);
    values[4] = r_u32(geometry + 8u);
    values[5] = r_u32(geometry + 12u);
    w_u32(geometry + 24u, values[0]);
    w_u32(geometry + 28u, values[1]);
    w_u32(geometry + 32u, values[2]);
    w_u32(geometry + 36u, values[3]);
    w_u32(geometry + 16u, values[4]);
    w_u32(geometry + 20u, values[5]);
}


void game_request_pause_mission_exit(void)
{
    uint32 menu, geometry, values[6], state;
    FUNCTION_MARKER(0x8008B080, "GAME.OVL");
    menu = r_u32(0x80124C00u);
    geometry = r_u32(menu + 0x1Cu);
    values[0] = r_u32(geometry + 8u);
    values[1] = r_u32(geometry + 12u);
    values[2] = r_u32(geometry);
    values[3] = r_u32(geometry + 4u);
    values[4] = r_u32(geometry + 8u);
    values[5] = r_u32(geometry + 12u);
    w_u32(geometry + 24u, values[0]);
    w_u32(geometry + 28u, values[1]);
    w_u32(geometry + 32u, values[2]);
    w_u32(geometry + 36u, values[3]);
    w_u32(geometry + 16u, values[4]);
    w_u32(geometry + 20u, values[5]);
    state = r_u32(0x800F18ACu);
    if (state == 1u)
        w_u16(0x80059610u, 6u);
    else if (r_s16(0x80059610u) != 8)
        w_u16(0x80059610u, 3u);
    w_u32(0x800ECEA8u, 3u);
}


void game_draw_pause_grid_background(void)
{
    VECTOR position;
    SVECTOR angles;
    MATRIX rotation;
    sint32 screen[3], depths[3], flags, projection, last_depth;
    sint32 row_x, row_y, row_z, texture_depth, texture_x, texture_y;
    uint32 x, camera_x, camera_y, camera_z, y, z, magnitude_x, magnitude_y, magnitude_z;
    uint32 shift, batch, index, row, column, vertex, neighbor, packet, cursor, owner, value;
    uint16 page;
    FUNCTION_MARKER(0x8008BA28, "GAME.OVL");
    x = r_u32(0x800F4010u);
    camera_x = r_u32(0x8011F120u);
    camera_y = r_u32(0x8011F124u);
    camera_z = r_u32(0x8011F128u);
    position.vx = (sint32)(x - camera_x);
    y = r_u32(0x800F4014u);
    z = r_u32(0x800F4018u);
    position.vy = (sint32)(y - camera_y);
    position.vz = (sint32)(z - camera_z);
    ApplyMatrixLV((MATRIX *)psx_addr(0x8011A3C0u, 20u), &position, &position);
    shift = 0u;
    for (;;) {
        x = (uint32)position.vx;
        y = (uint32)position.vy;
        z = (uint32)position.vz;
        magnitude_x = (sint32)x < 0 ? 0u - x : x;
        magnitude_y = (sint32)y < 0 ? 0u - y : y;
        magnitude_z = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)magnitude_x < 16385 && (sint32)magnitude_y < 16385 && (sint32)magnitude_z < 16385)
            break;
        ++shift;
        position.vx >>= 1;
        position.vy >>= 1;
        position.vz >>= 1;
    }
    matrix_copy_rotation(0x80035060u, 0x8011A3C0u);
    row_x = r_s16(0x80035060u) * 8;
    row_y = r_s16(0x80035062u) * 8;
    row_z = r_s16(0x80035064u) * 8;
    w_u16(0x80035060u, (uint16)(row_x / 5));
    w_u16(0x80035062u, (uint16)(row_y / 5));
    w_u16(0x80035064u, (uint16)(row_z / 5));
    angles.vx = 4;
    angles.vy = 0;
    angles.vz = 4;
    RotMatrix(&angles, &rotation);
    MulMatrix0((MATRIX *)psx_addr(0x800F3CF0u, 20u), &rotation,
        (MATRIX *)psx_addr(0x8004EAF8u, 20u));
    matrix_copy_rotation(0x800F3CF0u, 0x8004EAF8u);
    matrix_normalize(0x800F3CF0u, 1u);
    matrix_shift(0x8004EAF8u, 0x80042188u, shift);
    MulMatrix0((MATRIX *)psx_addr(0x80035060u, 20u),
        (MATRIX *)psx_addr(0x80042188u, 20u), (MATRIX *)psx_addr(0x8004EAF8u, 20u));
    rotation.t[0] = position.vx;
    rotation.t[1] = position.vy;
    rotation.t[2] = position.vz;
    SetRotMatrix((MATRIX *)psx_addr(0x8004EAF8u, 20u));
    SetTransMatrix(&rotation);
    for (batch = 0u; batch < 85u; ++batch) {
        gte_project3_full_depth((const SVECTOR *)psx_addr(0x800F33E0u + batch * 24u, 24u), screen, depths, &flags);
        for (index = 0u; index < 3u; ++index)
            w_u32(0x800F4258u + batch * 12u + index * 4u, (uint32)screen[index]);
        for (index = 0u; index < 3u; ++index)
            w_u16(0x8011A1A8u + batch * 6u + index * 2u, (uint16)depths[index]);
        w_u16(0x800F2598u + batch * 2u, (uint16)((uint32)flags >> 12));
    }
    last_depth = RotTransPers((SVECTOR *)psx_addr(0x800F3BD8u, 8u),
        (sint32 *)psx_addr(0x800F4654u, 4u), &projection, &flags);
    w_u16(0x8011A3A6u, (uint16)((uint32)last_depth << 2));
    for (row = 0u; row < 16u; ++row) {
        for (column = 0u; column < 16u; ++column) {
            vertex = row * 16u + column;
            if (r_s16(0x8011A1A8u + vertex * 2u) < 101)
                continue;
            packet = r_u32(0x80040690u);
            w_u32(packet + 4u, 0x2000u);
            value = r_u16(0x800F4258u + vertex * 4u);
            w_u16(packet + 8u, (uint16)value);
            value = r_u16(0x800F425Au + vertex * 4u);
            w_u16(packet + 10u, (uint16)value);
            neighbor = row * 16u + ((column + 1u) & 15u);
            value = r_u16(0x800F4258u + neighbor * 4u);
            w_u16(packet + 12u, (uint16)value);
            value = r_u16(0x800F425Au + neighbor * 4u);
            w_u16(packet + 14u, (uint16)value);
            setLineF2((LINE_F2 *)psx_addr(packet, 16u));
            cursor = r_u32(0x80040690u);
            SetSemiTrans((LINE_F2 *)psx_addr(cursor, 16u), 1);
            owner = r_u32(0x8004608Cu);
            cursor = r_u32(0x80040690u);
            AddPrim(psx_addr(owner + 0x7Cu, 4u), psx_addr(cursor, 4u));
            cursor = r_u32(0x80040690u);
            packet = cursor + 64u;
            w_u32(0x80040690u, packet);
            if (row + 1u >= 16u)
                continue;
            w_u32(cursor + 68u, 0x2000u);
            value = r_u16(0x800F4258u + vertex * 4u);
            w_u16(cursor + 72u, (uint16)value);
            value = r_u16(0x800F425Au + vertex * 4u);
            w_u16(cursor + 74u, (uint16)value);
            neighbor = (row + 1u) * 16u + column;
            value = r_u16(0x800F4258u + neighbor * 4u);
            w_u16(cursor + 76u, (uint16)value);
            value = r_u16(0x800F425Au + neighbor * 4u);
            w_u16(cursor + 78u, (uint16)value);
            setLineF2((LINE_F2 *)psx_addr(packet, 16u));
            cursor = r_u32(0x80040690u);
            SetSemiTrans((LINE_F2 *)psx_addr(cursor, 16u), 1);
            owner = r_u32(0x8004608Cu);
            cursor = r_u32(0x80040690u);
            AddPrim(psx_addr(owner + 0x7Cu, 4u), psx_addr(cursor, 4u));
            cursor = r_u32(0x80040690u);
            w_u32(0x80040690u, cursor + 64u);
        }
    }
    texture_depth = r_s16(0x8005050Cu);
    texture_x = r_s16(0x80050500u);
    texture_y = r_s16(0x80050502u);
    page = GetTPage(texture_depth, 1, texture_x, texture_y);
    packet = r_u32(0x80040690u);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12u), 0, 1, page, NULL);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x7Cu, 4u), psx_addr(packet, 4u));
    w_u32(0x80040690u, packet + 12u);
    game_draw_pause_ship_preview();
}


void game_draw_pause_ship_preview(void)
{
    VECTOR offset, position;
    MATRIX translation;
    uint32 model_index, scale, x, y, z, camera, magnitude_x, magnitude_y, magnitude_z;
    uint32 shift, products[9], index, flags, previous_flags, object, model_table, resource, table_entry;
    sint32 model;
    FUNCTION_MARKER(0x8008BF84, "GAME.OVL");
    offset.vx = r_s32(0x80074694u);
    offset.vy = r_s32(0x80074698u);
    offset.vz = r_s32(0x8007469Cu);
    offset.pad = r_s32(0x800746A0u);
    model_index = r_u32(0x800ECBD0u);
    scale = 0u;
    switch (model_index) {
    case 0u:
    case 6u:
        scale = 0xB00u;
        break;
    case 1u:
    case 2u:
        scale = 0x1300u;
        break;
    case 3u:
        scale = 0xC00u;
        break;
    case 4u:
        scale = 0xD00u;
        break;
    case 5u:
        offset.vz = -7000;
        scale = 0xE00u;
        break;
    case 7u:
    case 8u:
    case 9u:
        scale = 0x1500u;
        break;
    }
    if (r_u32(0x800311C0u) != 0u)
        scale -= (uint32)((sint32)scale >> 3);
    ApplyMatrixLV((MATRIX *)psx_addr(0x800F3CF0u, 20u), &offset, &offset);
    x = r_u32(0x800F4010u);
    camera = r_u32(0x8011F120u);
    position.vx = (sint32)(x - camera + (uint32)offset.vx);
    y = r_u32(0x800F4014u);
    camera = r_u32(0x8011F124u);
    position.vy = (sint32)(y - camera + (uint32)offset.vy);
    z = r_u32(0x800F4018u);
    camera = r_u32(0x8011F128u);
    position.vz = (sint32)(z - camera + (uint32)offset.vz);
    ApplyMatrixLV((MATRIX *)psx_addr(0x8011A3C0u, 20u), &position, &position);
    shift = 0u;
    for (;;) {
        x = (uint32)position.vx;
        y = (uint32)position.vy;
        z = (uint32)position.vz;
        magnitude_x = (sint32)x < 0 ? 0u - x : x;
        magnitude_y = (sint32)y < 0 ? 0u - y : y;
        magnitude_z = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)magnitude_x < 16385 && (sint32)magnitude_y < 16385 && (sint32)magnitude_z < 16385)
            break;
        ++shift;
        position.vx >>= 1;
        position.vy >>= 1;
        position.vz >>= 1;
    }
    translation.t[0] = position.vx;
    translation.t[1] = position.vy;
    translation.t[2] = position.vz;
    matrix_shift(0x800F3CF0u, 0x8004EAF8u, shift);
    for (index = 0u; index < 8u; ++index)
        products[index] = (uint32)r_s16(0x8004EAF8u + index * 2u) * scale;
    w_u16(0x8004EAF8u, (uint16)((sint32)products[0] >> 12));
    w_u16(0x8004EAFAu, (uint16)((sint32)products[1] >> 12));
    products[8] = (uint32)r_s16(0x8004EB08u) * scale;
    for (index = 2u; index < 9u; ++index)
        w_u16(0x8004EAF8u + index * 2u, (uint16)((sint32)products[index] >> 12));
    MulMatrix0((MATRIX *)psx_addr(0x80035060u, 20u),
        (MATRIX *)psx_addr(0x8004EAF8u, 20u), (MATRIX *)psx_addr(0x80042188u, 20u));
    MulMatrix0((MATRIX *)psx_addr(0x80041F28u, 20u),
        (MATRIX *)psx_addr(0x800F3CF0u, 20u), (MATRIX *)psx_addr(0x80034D80u, 20u));
    SetRotMatrix((MATRIX *)psx_addr(0x80042188u, 20u));
    SetLightMatrix((MATRIX *)psx_addr(0x80034D80u, 18u));
    SetTransMatrix(&translation);
    camera = r_u32(0x80040FDCu);
    flags = r_u32(camera + 0x70u);
    w_u32(0x800313D8u, 0xFFFFFFFFu);
    w_u32(0x80052838u, (flags & 4u) != 0u ? 0x01000000u : 0u);
    if (r_u32(0x8004EB2Cu) != 0u) {
        flags = r_u32(0x80052838u);
        w_u32(0x80052838u, flags | 0x02000000u);
    }
    object = r_u32(0x8003FE74u);
    model_index = r_u32(0x800ECBD0u);
    camera = r_u32(0x80040FDCu);
    flags = r_u32(camera + 0x70u);
    previous_flags = r_u32(camera + 0x70u);
    table_entry = 0x800ECBECu + model_index * 44u;
    model = r_s16(table_entry);
    w_u32(0x8004EB18u, flags & 2u);
    w_u32(object + 0x74u, (uint32)model);
    model = r_s16(table_entry);
    model_table = r_u32(0x80046178u);
    resource = r_u32(model_table + (uint32)model * 132u);
    w_u32(0x8004EB2Cu, (previous_flags >> 12) & 2u);
    w_u32(object + 0x64u, resource);
    object_draw_primitives(object);
}


void game_draw_pause_heading_for_menu(uint32 menu)
{
    uint32 geometry, region, anchor_x, anchor_y, title, top, text, length, sprite, packet, tile, owner;
    uint32 shift, value, inset, height, blue, target, origin, delta, old_x, old_y, endpoint, current;
    uint32 magnitude, remaining;
    sint32 texture_depth, texture_x, texture_y, clut_x, clut_y, step;
    uint16 clut, page;
    FUNCTION_MARKER(0x80089918, "GAME.OVL");
    geometry = r_u32(menu + 0x14u);
    region = r_u32(0x800311C0u);
    anchor_y = r_u32(geometry + 0x14u);
    anchor_x = r_u32(geometry + 0x10u);
    if (region != 0u)
        anchor_y += (uint32)((sint32)anchor_y >> 4);
    w_u8(0x8004617Cu, 112u);
    w_u8(0x80033D38u, 32u);
    w_u32(0x80033D2Cu, 0u);
    title = r_u32(menu + 0xCu);
    w_u32(0x80053688u, 40u);
    top = r_u32(0x800ECF04u);
    w_u8(0x80053680u, 0u);
    text = r_u32(0x800F24C4u + title * 4u);
    w_u32(0x8005368Cu, anchor_y + 39u - top);
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    font_draw_current((const char *)psx_addr(text, (size_t)length + 1u));
    sprite = r_u32(0x80040690u);
    w_u32(0x80040690u, sprite + 36u);
    shift = 2u - (uint32)r_s16(0x800504FCu);
    SetSprt((SPRT *)psx_addr(sprite, 20u));
    value = r_u16(0x800504F0u);
    w_u8(sprite + 12u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(0x800504F2u);
    w_u8(sprite + 13u, (uint8)value);
    top = r_u32(0x800ECF04u);
    inset = r_u32(0x800ECEFCu);
    w_u16(sprite + 8u, 267u);
    w_u16(sprite + 10u, (uint16)(anchor_y - (top - inset)));
    value = r_u16(0x800504FEu);
    w_u16(sprite + 16u, (uint16)value);
    value = r_u16(0x800504FAu);
    w_u16(sprite + 18u, (uint16)value);
    clut_x = r_s16(0x800504F4u);
    clut_y = r_s16(0x800504F6u);
    clut = GetClut(clut_x, clut_y);
    w_u8(sprite + 4u, 128u);
    w_u8(sprite + 5u, 128u);
    w_u8(sprite + 6u, 128u);
    owner = r_u32(0x8004608Cu);
    w_u16(sprite + 14u, clut);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(sprite, 4u));
    texture_depth = r_s16(0x800504FCu);
    texture_x = r_s16(0x800504F0u);
    texture_y = r_s16(0x800504F2u);
    page = GetTPage(texture_depth, 1, texture_x, texture_y);
    packet = r_u32(0x80040690u);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12u), 0, 1, page, NULL);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(packet, 4u));
    w_u32(0x80040690u, packet + 12u);
    tile = sprite + 20u;
    w_u8(tile + 3u, 3u);
    w_u8(tile + 7u, 96u);
    value = r_u8(0x800ECEE0u);
    w_u8(tile + 4u, (uint8)value);
    value = r_u8(0x800ECEE1u);
    w_u8(tile + 5u, (uint8)value);
    blue = r_u8(0x800ECEE2u);
    w_u16(tile + 8u, 33u);
    w_u16(tile + 12u, 234u);
    w_u8(tile + 6u, (uint8)blue);
    top = r_u32(0x800ECF04u);
    owner = r_u32(0x8004608Cu);
    inset = r_u32(0x800ECEFCu);
    height = r_u32(0x800ECF00u);
    w_u16(tile + 10u, (uint16)(anchor_y - (top - inset)));
    w_u16(tile + 14u, (uint16)height);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(tile, 4u));
    tile = r_u32(0x80040690u);
    w_u32(0x80040690u, tile + 16u);
    w_u8(tile + 3u, 3u);
    w_u8(tile + 7u, 96u);
    value = r_u8(0x800ECEDCu);
    w_u8(tile + 4u, (uint8)value);
    value = r_u8(0x800ECEDDu);
    w_u8(tile + 5u, (uint8)value);
    blue = r_u8(0x800ECEDEu);
    w_u16(tile + 8u, 0u);
    w_u16(tile + 12u, (uint16)(anchor_x + 512u));
    w_u8(tile + 6u, (uint8)blue);
    owner = r_u32(0x8004608Cu);
    top = r_u32(0x800ECF04u);
    w_u16(tile + 10u, (uint16)(anchor_y - top));
    w_u16(tile + 14u, (uint16)top);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(tile, 4u));
    menu = r_u32(0x80124C00u);
    if ((r_u32(menu + 0x10u) & 1u) == 0u)
        return;
    target = r_u32(geometry + 8u);
    origin = r_u32(geometry);
    delta = target - origin;
    magnitude = (sint32)delta < 0 ? 0u - delta : delta;
    old_x = r_u32(geometry + 0x10u);
    endpoint = r_u32(geometry + 0x20u);
    old_y = r_u32(geometry + 0x14u);
    remaining = endpoint - old_x;
    if ((sint32)remaining < 0)
        remaining = 0u - remaining;
    step = (sint32)magnitude / 15;
    if ((sint32)remaining < step)
        w_u32(geometry + 0x10u, target);
    else
        w_u32(geometry + 0x10u, old_x + (uint32)((sint32)delta / 15));
    target = r_u32(geometry + 12u);
    origin = r_u32(geometry + 4u);
    delta = target - origin;
    magnitude = (sint32)delta < 0 ? 0u - delta : delta;
    endpoint = r_u32(geometry + 0x24u);
    current = r_u32(geometry + 0x14u);
    remaining = endpoint - current;
    if ((sint32)remaining < 0)
        remaining = 0u - remaining;
    step = (sint32)magnitude / 15;
    if ((sint32)remaining < step)
        w_u32(geometry + 0x14u, target);
    else
        w_u32(geometry + 0x14u, current + (uint32)((sint32)delta / 15));
    if (r_u32(geometry + 0x10u) != old_x)
        return;
    if (r_u32(geometry + 0x14u) != old_y)
        return;
    menu = r_u32(0x80124C00u);
    value = r_u32(menu + 0x10u);
    w_u32(menu + 0x10u, value ^ 1u);
}


void game_draw_pause_heading(void)
{
    uint32 menu;
    FUNCTION_MARKER(0x80089910, "GAME.OVL");
    menu = r_u32(0x80124C00u);
    game_draw_pause_heading_for_menu(menu);
}


void game_draw_pause_warp_grid_values(uint32 fade, uint32 base_red, uint32 texture_depth)
{
    uint32 base[3], first[3], second[3], shift, product, row, column, index, grid, grid_x, grid_y;
    uint32 scaled_x, shifted_x, squared, falloff, other_falloff, color, next_color, right_color;
    uint32 packet, value, width, saved_v, next_v, clut_x, clut_y, texture_x, texture_y, depth;
    uint32 coordinate_x, coordinate_y, frame, fresh_y, blue, saved_x, saved_y, owner;
    uint16 clut, page;
    FUNCTION_MARKER(0x8008A324, "GAME.OVL");
    shift = 2u - texture_depth;
    product = fade * base_red;
    base[0] = (uint32)((sint32)product / 32);
    value = r_u32(0x800ECED0u);
    product = fade * value;
    base[1] = (uint32)((sint32)product / 32);
    value = r_u32(0x800ECED4u);
    product = fade * value;
    base[2] = (uint32)((sint32)product / 32);
    for (column = 0u; column < 11u; ++column) {
        for (row = 0u; row < 18u; ++row) {
            grid = 0x800F2B70u + row * 88u + column * 8u;
            grid_x = r_u32(grid);
            scaled_x = (uint32)((sint32)(grid_x * 160u) >> 8);
            grid_y = r_u32(grid + 4u);
            squared = scaled_x * scaled_x + grid_y * grid_y;
            falloff = 176u - (uint32)((sint32)squared >> 8);
            if ((sint32)falloff < 0)
                falloff = 0u;
            shifted_x = (uint32)((sint32)((grid_x - 512u) * 160u) >> 8);
            squared = shifted_x * shifted_x;
            for (index = 0u; index < 3u; ++index) {
                value = r_u32(0x800ECF20u + index * 4u);
                product = value * falloff;
                first[index] = (uint32)((sint32)product >> 8);
            }
            value = grid_y - 245u;
            squared += value * value;
            other_falloff = 176u - (uint32)((sint32)squared >> 8);
            if ((sint32)other_falloff < 0)
                other_falloff = 0u;
            for (index = 0u; index < 3u; ++index) {
                value = r_u32(0x800ECF2Cu + index * 4u);
                product = value * other_falloff;
                second[index] = (uint32)((sint32)product >> 8);
            }
            color = 0x800FD1C8u + row * 44u + column * 4u;
            w_u8(color, (uint8)(base[0] + first[0] + second[0]));
            w_u8(color + 1u, (uint8)(base[1] + first[1] + second[1]));
            w_u8(color + 2u, (uint8)(base[2] + first[2] + second[2]));
        }
    }
    for (column = 0u; column < 11u; ++column) {
        for (row = 0u; row < 18u; ++row) {
            packet = r_u32(0x80040690u);
            SetPolyGT4((POLY_GT4 *)psx_addr(packet, 52u));
            value = r_u16(0x8004FA80u);
            w_u8(packet + 12u, (uint8)((value & 63u) << (shift & 31u)));
            value = r_u16(0x8004FA82u);
            w_u8(packet + 13u, (uint8)value);
            value = r_u8(packet + 12u);
            width = r_u8(0x8004FA88u);
            w_u8(packet + 36u, (uint8)value);
            value = r_u8(packet + 12u);
            value += width * 2u;
            saved_v = r_u8(packet + 13u);
            w_u8(packet + 24u, (uint8)(value - 1u));
            next_v = r_u8(packet + 13u);
            w_u8(packet + 25u, (uint8)saved_v);
            value = r_u8(0x8004FA8Au);
            w_u8(packet + 37u, (uint8)(next_v + value));
            value = r_u8(packet + 24u);
            next_v = r_u8(packet + 37u);
            w_u8(packet + 48u, (uint8)value);
            w_u8(packet + 49u, (uint8)next_v);
            clut_x = (uint32)r_s16(0x8004FA84u);
            clut_y = (uint32)r_s16(0x8004FA86u);
            clut = GetClut((sint32)clut_x, (sint32)clut_y);
            w_u16(packet + 14u, clut);
            depth = (uint32)r_s16(0x8004FA8Cu);
            texture_x = (uint32)r_s16(0x8004FA80u);
            texture_y = (uint32)r_s16(0x8004FA82u);
            page = GetTPage((sint32)depth, 0, (sint32)texture_x, (sint32)texture_y);
            w_u16(packet + 26u, page);
            grid = 0x800F2B70u + row * 88u + column * 8u;
            coordinate_x = r_u32(grid);
            frame = r_u32(0x800F3D10u);
            coordinate_y = r_u32(grid + 4u);
            if ((frame & 1u) != 0u) {
                w_u32(grid + 4u, coordinate_y - 1u);
                fresh_y = r_u32(grid + 4u);
                if ((sint32)(column * 32u) >= (sint32)fresh_y)
                    w_u32(grid + 4u, (column + 1u) * 32u);
            }
            color = 0x800FD1C8u + row * 44u + column * 4u;
            w_u16(packet + 8u, (uint16)coordinate_x);
            w_u16(packet + 10u, (uint16)coordinate_y);
            value = r_u8(color);
            w_u8(packet + 4u, (uint8)value);
            value = r_u8(color + 1u);
            w_u8(packet + 5u, (uint8)value);
            blue = r_u8(color + 2u);
            saved_y = r_u16(packet + 10u);
            w_u16(packet + 20u, (uint16)(coordinate_x + 32u));
            w_u16(packet + 22u, (uint16)saved_y);
            w_u8(packet + 6u, (uint8)blue);
            next_color = color + 44u;
            value = r_u8(next_color);
            w_u8(packet + 16u, (uint8)value);
            value = r_u8(next_color + 1u);
            w_u8(packet + 17u, (uint8)value);
            saved_x = r_u16(packet + 8u);
            blue = r_u8(next_color + 2u);
            w_u16(packet + 32u, (uint16)saved_x);
            w_u16(packet + 34u, (uint16)(coordinate_y + 32u));
            w_u8(packet + 18u, (uint8)blue);
            right_color = color + 4u;
            value = r_u8(right_color);
            saved_x = r_u16(packet + 20u);
            w_u8(packet + 28u, (uint8)value);
            value = r_u8(right_color + 1u);
            w_u8(packet + 29u, (uint8)value);
            blue = r_u8(right_color + 2u);
            saved_y = r_u16(packet + 34u);
            w_u16(packet + 44u, (uint16)saved_x);
            w_u16(packet + 46u, (uint16)saved_y);
            w_u8(packet + 30u, (uint8)blue);
            next_color = right_color + 44u;
            value = r_u8(next_color);
            w_u8(packet + 40u, (uint8)value);
            value = r_u8(next_color + 1u);
            w_u8(packet + 41u, (uint8)value);
            value = r_u8(next_color + 2u);
            w_u8(packet + 42u, (uint8)value);
            owner = r_u32(0x8004608Cu);
            AddPrim(psx_addr(owner + 0x3EF0u, 4u), psx_addr(packet, 4u));
            value = r_u32(0x80040690u);
            w_u32(0x80040690u, value + 52u);
        }
    }
    value = r_u32(0x80108A4Cu) + 1u;
    w_u32(0x80108A4Cu, value);
    if ((sint32)value >= 23)
        w_u32(0x80108A4Cu, 22u);
}


void game_draw_pause_warp_grid(void)
{
    uint32 fade, base_red, texture_depth;
    FUNCTION_MARKER(0x8008A30C, "GAME.OVL");
    fade = r_u32(0x80108A4Cu);
    base_red = r_u32(0x800ECECCu);
    texture_depth = (uint32)r_s16(0x8004FA8Cu);
    game_draw_pause_warp_grid_values(fade, base_red, texture_depth);
}


void game_draw_pause_menu(void)
{
    FUNCTION_MARKER(0x8008A8E8, "GAME.OVL");
    game_draw_pause_grid_background();
    game_draw_pause_options();
    game_draw_pause_panel();
    game_draw_pause_heading();
    game_draw_pause_warp_grid();
}


void game_draw_pause_options_for_menu(uint32 menu)
{
    uint32 count, geometry, arena, anchor_x, shift, value, owner, packet, second_sprite, quad;
    uint32 u, v, width, saved_u, saved_v, first_width, second_width, x, text, length;
    uint32 entry, index, row_offset, selected, target, origin, delta, magnitude, old_x, old_y, remaining, current;
    sint32 depth, texture_x, texture_y, clut_x, clut_y, step;
    uint16 clut, page;
    FUNCTION_MARKER(0x8008895C, "GAME.OVL");
    count = r_u32(menu + 4u);
    geometry = r_u32(menu + 0x1Cu);
    if (count == 0u)
        return;
    arena = r_u32(0x80040690u);
    anchor_x = r_u32(geometry + 0x10u);
    w_u32(0x80040690u, arena + 80u);
    SetSprt((SPRT *)psx_addr(arena, 20u));
    depth = r_s16(0x8005050Cu);
    value = r_u16(0x80050500u);
    shift = 2u - (uint32)depth;
    w_u8(arena + 12u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(0x80050502u);
    w_u16(arena + 8u, (uint16)anchor_x);
    w_u16(arena + 10u, 26u);
    w_u8(arena + 13u, (uint8)value);
    value = r_u16(0x8005050Eu);
    w_u16(arena + 16u, (uint16)value);
    value = r_u16(0x8005050Au);
    w_u16(arena + 18u, (uint16)value);
    clut_x = r_s16(0x80050504u);
    clut_y = r_s16(0x80050506u);
    clut = GetClut(clut_x, clut_y);
    w_u8(arena + 4u, 128u);
    w_u8(arena + 5u, 128u);
    w_u8(arena + 6u, 128u);
    owner = r_u32(0x8004608Cu);
    w_u16(arena + 14u, clut);
    AddPrim(psx_addr(owner + 0x70u, 4u), psx_addr(arena, 4u));
    depth = r_s16(0x8005050Cu);
    texture_x = r_s16(0x80050500u);
    texture_y = r_s16(0x80050502u);
    page = GetTPage(depth, 1, texture_x, texture_y);
    packet = r_u32(0x80040690u);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12u), 0, 1, page, NULL);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x70u, 4u), psx_addr(packet, 4u));
    w_u32(0x80040690u, packet + 12u);
    second_sprite = arena + 20u;
    SetSprt((SPRT *)psx_addr(second_sprite, 20u));
    depth = r_s16(0x8005052Cu);
    value = r_u16(0x80050520u);
    shift = 2u - (uint32)depth;
    w_u8(arena + 32u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(0x80050522u);
    w_u8(arena + 33u, (uint8)value);
    width = r_u16(0x8005052Eu);
    w_u16(arena + 30u, 26u);
    w_u16(arena + 28u, (uint16)(anchor_x - (width - 131u)));
    value = r_u16(0x8005052Eu);
    w_u16(arena + 36u, (uint16)value);
    value = r_u16(0x8005052Au);
    w_u16(arena + 38u, (uint16)value);
    clut_x = r_s16(0x80050524u);
    clut_y = r_s16(0x80050526u);
    clut = GetClut(clut_x, clut_y);
    w_u8(arena + 24u, 128u);
    w_u8(arena + 25u, 128u);
    w_u8(arena + 26u, 128u);
    owner = r_u32(0x8004608Cu);
    w_u16(arena + 34u, clut);
    AddPrim(psx_addr(owner + 0x70u, 4u), psx_addr(second_sprite, 4u));
    depth = r_s16(0x8005052Cu);
    texture_x = r_s16(0x80050520u);
    texture_y = r_s16(0x80050522u);
    page = GetTPage(depth, 1, texture_x, texture_y);
    packet = r_u32(0x80040690u);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12u), 0, 1, page, NULL);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x70u, 4u), psx_addr(packet, 4u));
    w_u32(0x80040690u, packet + 12u);
    quad = arena + 40u;
    SetPolyFT4((POLY_FT4 *)psx_addr(quad, 40u));
    value = r_u16(0x80050510u);
    w_u8(arena + 52u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(0x80050512u);
    w_u8(arena + 53u, (uint8)value);
    u = r_u8(arena + 52u);
    width = r_u8(0x8005051Eu);
    v = r_u8(arena + 53u);
    w_u8(arena + 68u, (uint8)u);
    u = r_u8(arena + 52u);
    w_u8(arena + 61u, (uint8)v);
    w_u8(arena + 60u, (uint8)(u + width - 1u));
    v = r_u8(arena + 53u);
    value = r_u8(0x8005051Au);
    saved_u = r_u8(arena + 60u);
    w_u8(arena + 69u, (uint8)(v + value));
    first_width = r_u16(arena + 16u);
    saved_v = r_u8(arena + 69u);
    w_u16(arena + 50u, 26u);
    w_u8(arena + 76u, (uint8)saved_u);
    w_u16(arena + 48u, (uint16)(first_width + anchor_x));
    w_u8(arena + 77u, (uint8)saved_v);
    first_width = r_u16(0x8005050Eu);
    second_width = r_u16(0x8005052Eu);
    x = r_u16(arena + 48u);
    w_u16(arena + 58u, 26u);
    w_u16(arena + 64u, (uint16)x);
    x = r_u16(arena + 48u);
    w_u16(arena + 56u, (uint16)(x + 131u - first_width - second_width));
    value = r_u16(0x8005051Au);
    w_u16(arena + 66u, (uint16)(value + 26u));
    x = r_u16(arena + 56u);
    value = r_u16(arena + 66u);
    w_u16(arena + 72u, (uint16)x);
    w_u16(arena + 74u, (uint16)value);
    clut_x = r_s16(0x80050514u);
    clut_y = r_s16(0x80050516u);
    clut = GetClut(clut_x, clut_y);
    w_u16(arena + 54u, clut);
    depth = r_s16(0x8005051Cu);
    texture_x = r_s16(0x80050510u);
    texture_y = r_s16(0x80050512u);
    page = GetTPage(depth, 0, texture_x, texture_y);
    value = r_u32(0x800ECF08u);
    w_u8(arena + 44u, (uint8)value);
    value = r_u32(0x800ECF0Cu);
    w_u8(arena + 45u, (uint8)value);
    value = r_u32(0x800ECF10u);
    w_u8(arena + 46u, (uint8)value);
    owner = r_u32(0x8004608Cu);
    w_u16(arena + 62u, page);
    AddPrim(psx_addr(owner + 0x70u, 4u), psx_addr(quad, 4u));
    text = r_u32(0x800F24F4u);
    w_u8(0x8004617Cu, 64u);
    w_u8(0x80033D38u, 64u);
    w_u8(0x80053680u, 64u);
    w_u32(0x80033D2Cu, 242u);
    w_u32(0x80053688u, anchor_x + 74u);
    w_u32(0x8005368Cu, 34u);
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    font_draw_current((const char *)psx_addr(text, (size_t)length + 1u));
    menu = r_u32(0x80124C00u);
    w_u32(0x80033D2Cu, 84u);
    count = r_u32(menu + 4u);
    entry = menu + 0x30u;
    index = 0u;
    row_offset = 0u;
    if ((sint32)count > 0) {
        do {
            menu = r_u32(0x80124C00u);
            selected = r_u32(menu);
            game_draw_pause_option(anchor_x, 62u + row_offset, index == selected, entry);
            menu = r_u32(0x80124C00u);
            count = r_u32(menu + 4u);
            ++index;
            entry += 28u;
            row_offset += 16u;
        } while ((sint32)index < (sint32)count);
    }
    menu = r_u32(0x80124C00u);
    if ((r_u32(menu + 0x10u) & 4u) == 0u)
        return;
    target = r_u32(geometry + 0x20u);
    origin = r_u32(geometry + 0x18u);
    delta = target - origin;
    magnitude = (sint32)delta < 0 ? 0u - delta : delta;
    old_x = r_u32(geometry + 0x10u);
    old_y = r_u32(geometry + 0x14u);
    remaining = target - old_x;
    if ((sint32)remaining < 0)
        remaining = 0u - remaining;
    step = (sint32)magnitude / 15;
    if ((sint32)remaining < step)
        w_u32(geometry + 0x10u, target);
    else
        w_u32(geometry + 0x10u, old_x + (uint32)((sint32)delta / 15));
    target = r_u32(geometry + 0x24u);
    origin = r_u32(geometry + 0x1Cu);
    delta = target - origin;
    magnitude = (sint32)delta < 0 ? 0u - delta : delta;
    current = r_u32(geometry + 0x14u);
    remaining = target - current;
    if ((sint32)remaining < 0)
        remaining = 0u - remaining;
    step = (sint32)magnitude / 15;
    if ((sint32)remaining < step)
        w_u32(geometry + 0x14u, target);
    else
        w_u32(geometry + 0x14u, current + (uint32)((sint32)delta / 15));
    if (r_u32(geometry + 0x10u) != old_x)
        return;
    if (r_u32(geometry + 0x14u) != old_y)
        return;
    menu = r_u32(0x80124C00u);
    value = r_u32(menu + 0x10u);
    w_u32(menu + 0x10u, value ^ 4u);
}
