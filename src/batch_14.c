#include "game_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void front_update_diagnostic_overlay(void)
{
    PSX_RECT rectangle, strip;
    sint32 state, time, duration, red, green, blue, first, second;
    sint32 base[4], delta[4], step, position, intensity, index;
    uint32 color, depth, word0, word1, value, dimension;
    uint32 source, destination;
    FUNCTION_MARKER(0x800AA298, "FRONT.OVL");
    state = r_s16(0x8010D82Eu);
    w_u32(0x800FB870u, 0x154u);
    switch (state) {
    case 0:
        first = r_u8(0x8010D856u);
        red = r_u8(0x8010D852u);
        time = r_s16(0x8010D830u);
        first = time * (first - red);
        duration = r_s16(0x8010D832u);
        if (duration == 0)
            abort();
        delta[0] = first / duration;
        green = r_u8(0x8010D853u);
        first = r_u8(0x8010D857u);
        delta[1] = time * (first - green) / duration;
        blue = r_u8(0x8010D854u);
        first = r_u8(0x8010D858u);
        delta[2] = time * (first - blue) / duration;
        depth = r_u32(0x801132A8u);
        red += delta[0];
        green += delta[1];
        blue += delta[2];
        color = (uint8)red | ((uint32)(uint8)green << 8) | ((uint32)(uint8)blue << 16);
        w_u8(0x8010D84Eu, red);
        w_u8(0x8010D84Fu, green);
        w_u8(0x8010D850u, blue);
        front_draw_blended_rectangle((const PSX_RECT *)psx_addr(0x8010D836u, 8u), color, 1u, depth);
        color = r_u8(0x8010D84Eu);
        depth = r_u32(0x801132A8u);
        color |= (uint32)r_u8(0x8010D84Fu) << 8;
        color |= (uint32)r_u8(0x8010D850u) << 16;
        front_draw_blended_rectangle((const PSX_RECT *)psx_addr(0x8010D836u, 8u), color, 2u, depth);
        duration = r_s16(0x8010D832u);
        time = r_s16(0x8010D830u);
        if (duration == time) {
            w_u16(0x8010D82Eu, 1u);
            w_u16(0x8010D830u, 0u);
            word0 = (r_u32(0x8010D838u) << 16) | (r_u32(0x8010D834u) >> 16);
            word1 = (r_u32(0x8010D83Cu) << 16) | (r_u32(0x8010D838u) >> 16);
            value = r_u32(0x8010D840u);
            w_u32(0x8010D840u, (value & 0xFFFF0000u) | (word0 >> 16));
            value = r_u32(0x8010D83Cu);
            w_u32(0x8010D83Cu, (value & 0xFFFFu) | (word0 << 16));
            value = r_u32(0x8010D844u);
            w_u32(0x8010D844u, (value & 0xFFFF0000u) | (word1 >> 16));
            value = r_u32(0x8010D840u);
            w_u32(0x8010D840u, (value & 0xFFFFu) | (word1 << 16));
            first = r_s16(0x8010D83Au);
            second = r_s16(0x8010D85Au);
            dimension = r_u16(0x8010DA62u);
            first = (first - second) / 2;
            value = r_u16(0x8010D836u);
            w_u16(0x8010D84Au, second);
            w_u16(0x8010D84Cu, dimension);
            w_u16(0x8010D846u, value + (uint32)first);
            first = (sint16)dimension / 2;
            value = r_u16(0x8010D838u);
            w_u16(0x8010D848u, value - (uint32)first);
        }
        break;
    case 1:
        first = r_s16(0x8010D846u);
        base[0] = r_s16(0x8010D83Eu);
        time = r_s16(0x8010D830u);
        first = time * (first - base[0]);
        duration = r_s16(0x8010D832u);
        if (duration == 0)
            abort();
        delta[0] = first / duration;
        for (index = 1; index < 4; ++index) {
            base[index] = r_s16(0x8010D83Eu + (uint32)index * 2u);
            first = r_s16(0x8010D846u + (uint32)index * 2u);
            delta[index] = time * (first - base[index]) / duration;
        }
        for (index = 0; index < 4; ++index)
            w_u16(0x8010D836u + (uint32)index * 2u, (uint32)base[index] + (uint32)delta[index]);
        if (duration == time)
            w_u16(0x8010D82Eu, 2u);
        /* Fall through to the text and noise pass */
    case 2:
        color = r_u8(0x8010D84Eu);
        color |= (uint32)r_u8(0x8010D84Fu) << 8;
        color |= (uint32)r_u8(0x8010D850u) << 16;
        front_draw_scaled_text(0x8010D86Eu, 0x8010D836u, color, 1u);
        color = r_u8(0x8010D84Eu);
        color |= (uint32)r_u8(0x8010D84Fu) << 8;
        color |= (uint32)r_u8(0x8010D850u) << 16;
        front_draw_scaled_text(0x8010D86Eu, 0x8010D836u, color, 2u);
        intensity = r_s16(0x8010D834u);
        red = (uint8)random_between(intensity, intensity * 3);
        green = (uint8)random_between(intensity, intensity * 3);
        blue = (uint8)random_between(intensity, intensity * 3);
        depth = r_u32(0x801132A8u);
        rectangle.w = 640;
        rectangle.x = 0;
        color = (uint32)red | ((uint32)green << 8) | ((uint32)blue << 16);
        rectangle.y = (sint16)r_u16(0x8010D838u);
        rectangle.h = (sint16)r_u16(0x8010D83Cu);
        front_draw_blended_rectangle(&rectangle, color, 2u, depth);
        break;
    case 4:
        duration = r_s16(0x8010D832u);
        if (duration == 0)
            abort();
        step = 1280 / duration;
        time = r_s16(0x8010D830u);
        position = time * 640 / duration;
        dimension = r_u16(0x8010DA62u);
        rectangle.h = (sint16)dimension;
        rectangle.y = (sint16)r_u16(0x8010D848u);
        position += step + 1;
        rectangle.x = (sint16)position;
        rectangle.w = (sint16)(640u - (uint32)position);
        front_draw_blended_rectangle(&rectangle, 0xFFFFFFu, 2u, 0u);
        value = (uint16)rectangle.x;
        rectangle.w = 1;
        rectangle.x = (sint16)(value - 1u);
        color = r_u8(0x8010D856u);
        color |= (uint32)r_u8(0x8010D857u) << 8;
        color |= (uint32)r_u8(0x8010D858u) << 16;
        front_draw_scaled_text(0x8010D86Eu, 0x8010D846u, color, 1u);
        color = r_u8(0x8010D856u);
        color |= (uint32)r_u8(0x8010D857u) << 8;
        color |= (uint32)r_u8(0x8010D858u) << 16;
        front_draw_scaled_text(0x8010D86Eu, 0x8010D846u, color, 2u);
        strip.x = 0;
        strip.y = (sint16)r_u16(0x8010D848u);
        strip.w = rectangle.x;
        strip.h = (sint16)r_u16(0x8010D84Cu);
        for (index = 0; index <= step; ++index) {
            intensity = r_s16(0x8010D834u);
            red = (uint8)random_between(intensity, intensity * 3);
            intensity = r_s16(0x8010D834u);
            green = (uint8)random_between(intensity, intensity * 3);
            intensity = r_s16(0x8010D834u);
            blue = (uint8)random_between(intensity, intensity * 3);
            depth = r_u32(0x801132A8u);
            color = (uint32)red | ((uint32)green << 8) | ((uint32)blue << 16);
            front_draw_blended_rectangle(&strip, color, 2u, depth);
            depth = r_u32(0x801132A8u);
            front_draw_blended_rectangle(&rectangle, 0xFFFFFFu, 1u, depth);
            rectangle.x = (sint16)((uint16)rectangle.x - 1u);
        }
        time = r_s16(0x8010D830u);
        duration = r_s16(0x8010D832u);
        if (time == duration) {
            w_u16(0x8010D82Eu, 2u);
            source = 0x8010D846u;
            destination = 0x8010D836u;
            word0 = (r_u32(source + 2u) << 16) | (r_u32(source - 2u) >> 16);
            word1 = (r_u32(source + 6u) << 16) | (r_u32(source + 2u) >> 16);
            value = r_u32(destination + 2u);
            w_u32(destination + 2u, (value & 0xFFFF0000u) | (word0 >> 16));
            value = r_u32(destination - 2u);
            w_u32(destination - 2u, (value & 0xFFFFu) | (word0 << 16));
            value = r_u32(destination + 6u);
            w_u32(destination + 6u, (value & 0xFFFF0000u) | (word1 >> 16));
            value = r_u32(destination + 2u);
            w_u32(destination + 2u, (value & 0xFFFFu) | (word1 << 16));
            word0 = (r_u32(0x8010D858u) << 16) | (r_u32(0x8010D854u) >> 16);
            value = r_u32(0x8010D850u);
            w_u32(0x8010D850u, (value & 0xFFFF0000u) | (word0 >> 16));
            value = r_u32(0x8010D84Cu);
            w_u32(0x8010D84Cu, (value & 0xFFFFu) | (word0 << 16));
            intensity = r_s16(0x8010D834u);
            w_u16(0x8010D834u, (uint32)(intensity * step));
        }
        break;
    case 5:
        color = r_u8(0x8010D84Eu);
        color |= (uint32)r_u8(0x8010D84Fu) << 8;
        color |= (uint32)r_u8(0x8010D850u) << 16;
        front_draw_scaled_text(0x8010D86Eu, 0x8010D836u, color, 1u);
        color = r_u8(0x8010D84Eu);
        color |= (uint32)r_u8(0x8010D84Fu) << 8;
        color |= (uint32)r_u8(0x8010D850u) << 16;
        front_draw_scaled_text(0x8010D86Eu, 0x8010D836u, color, 2u);
        break;
    default:
        break;
    }
    front_draw_bottom_red_rectangle();
    value = r_u16(0x8010D830u);
    w_u16(0x8010D830u, value + 1u);
}

void front_initialize_diagnostic_overlay(void)
{
    PSX_RECT rectangle;
    uint32 table, panel, depth, source, destination, offset, words[4];
    uint32 stutter, timeouts, width, adjusted;
    uint16 dimension;
    sint32 half;
    FUNCTION_MARKER(0x800A9F64, "FRONT.OVL");
    table = r_u32(0x800FB0F4u);
    panel = r_u32(0x800F312Cu);
    front_push_text_line(panel, r_u32(table));
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 640;
    rectangle.h = 480;
    depth = r_u32(0x801132A8u);
    w_u16(0x8010D82Cu, depth);
    front_set_ordering_table_size(10u);
    DrawSync(0);
    front_draw_blended_rectangle(&rectangle, 0xFFFFFFu, 2u, 0u);
    PutDrawEnv((DRAWENV *)psx_addr(0x800FB878u, sizeof(DRAWENV)));
    front_swap_render_buffers();
    DrawSync(0);
    front_draw_blended_rectangle(&rectangle, 0xFFFFFFu, 2u, 0u);
    PutDrawEnv((DRAWENV *)psx_addr(0x800FB878u, sizeof(DRAWENV)));
    front_swap_render_buffers();
    DrawSync(0);
    source = 0x800FB878u;
    destination = 0x8010D7D0u;
    for (offset = 0u; offset < 80u; offset += 16u) {
        words[0] = r_u32(source + offset);
        words[1] = r_u32(source + offset + 4u);
        words[2] = r_u32(source + offset + 8u);
        words[3] = r_u32(source + offset + 12u);
        w_u32(destination + offset, words[0]);
        w_u32(destination + offset + 4u, words[1]);
        w_u32(destination + offset + 8u, words[2]);
        w_u32(destination + offset + 12u, words[3]);
    }
    words[0] = r_u32(source + 80u);
    words[1] = r_u32(source + 84u);
    words[2] = r_u32(source + 88u);
    w_u32(destination + 80u, words[0]);
    w_u32(destination + 84u, words[1]);
    w_u32(destination + 88u, words[2]);
    stutter = r_u32(0x80113228u);
    timeouts = r_u32(0x80113220u);
    w_u8(0x800FB890u, 0u);
    w_u32(0x800FB870u, 0x154u);
    sprintf((char *)psx_addr(0x8010D86Eu, 41u), "stutter %d timeouts %d",
        (sint32)stutter, (sint32)timeouts);
    width = front_text_width(0x8010D86Eu);
    adjusted = 640u - (uint32)(sint32)(sint16)width;
    adjusted += adjusted >> 31;
    w_u16(0x8010D846u, adjusted >> 1);
    dimension = r_u16(0x800500BAu);
    w_u16(0x8010D83Au, 640u);
    w_u16(0x8010D83Cu, 2u);
    w_u16(0x8010D85Au, width);
    w_u16(0x8010D84Au, width);
    w_u8(0x8010D853u, 64u);
    w_u8(0x8010D854u, 64u);
    w_u8(0x8010D856u, 64u);
    w_u8(0x8010D857u, 64u);
    w_u8(0x8010D858u, 128u);
    w_u16(0x8010D832u, 60u);
    w_u16(0x8010D82Eu, 0u);
    w_u16(0x8010D836u, 0u);
    w_u16(0x8010D848u, 220u);
    w_u8(0x8010D84Eu, 0u);
    w_u8(0x8010D84Fu, 0u);
    w_u8(0x8010D850u, 0u);
    w_u8(0x8010D852u, 0u);
    w_u16(0x8010D830u, 0u);
    w_u16(0x8010D834u, 4u);
    half = ((sint32)(sint16)dimension + (dimension >> 15)) >> 1;
    w_u16(0x8010DA62u, dimension);
    w_u16(0x8010D838u, (uint32)half + 219u);
    w_u16(0x8010D84Cu, dimension);
}

void front_request_mode_seven(void)
{
    FUNCTION_MARKER(0x800A9F14, "FRONT.OVL");
    w_u32(0x800F3148u, 7u);
    w_u16(0x8001192Cu, 0u);
    w_u16(0x8001192Eu, 0u);
    w_u32(0x800313C8u, 0u);
    w_u32(0x8003113Cu, 0xFFFFFFFFu);
    front_leave_menu();
}

void front_draw_scaled_text_native(const char *text, const PSX_RECT *rectangle, uint32 color, uint32 blend)
{
    sint32 measured;
    sint32 depth;
    sint32 page_x;
    sint32 page_y;
    sint32 product;
    sint32 scaled;
    uint32 advance = 0u;
    uint32 character;
    uint32 glyph;
    uint32 atlas;
    uint32 packet;
    uint32 shift;
    uint32 span;
    uint32 u;
    uint32 v;
    uint32 first;
    uint32 second;
    uint32 right;
    uint32 bottom;
    uint32 height;
    uint32 opcode;
    uint16 page;
    uint16 clut;
    FUNCTION_MARKER(0x800A9BB0, "FRONT.OVL");
    measured = (sint16)front_text_width_native(text);
    character = (uint8)*text;
    while (character != 0u) {
        glyph = r_u32(0x8010D168u + (character << 2));
        if ((sint32)glyph < 0) {
            product = (sint32)rectangle->w * 4;
            if (measured == 0)
                abort();
            advance += (uint32)(product / measured);
        } else {
            atlas = 0x8004EB70u + ((glyph + r_u32(0x800FB870u)) << 4);
            depth = r_s16(atlas + 12u);
            page_x = (r_s16(atlas) / 64) * 64;
            shift = (2u - (uint32)depth) & 31u;
            page_y = (r_s16(atlas + 2u) / 256) * 256;
            page = GetTPage(depth, (sint16)blend, page_x, page_y);
            packet = r_u32(0x801132B8u);
            SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40u));
            w_u16(packet + 22u, page);
            w_u16(packet + 8u, (uint16)rectangle->x + advance);
            w_u16(packet + 10u, (uint16)rectangle->y);
            w_u8(packet + 12u, ((uint32)r_s16(atlas) - (uint32)page_x) << shift);
            w_u8(packet + 13u, r_u8(atlas + 2u) - (uint32)page_y);
            span = ((uint32)r_s16(atlas + 8u) << shift) - 1u;
            product = (sint32)rectangle->w * (sint32)(sint16)span;
            if (measured == 0)
                abort();
            scaled = product / measured;
            u = r_u8(packet + 12u);
            height = r_u16(atlas + 10u);
            first = r_u16(packet + 10u);
            second = r_u16(packet + 8u);
            w_u16(packet + 18u, first);
            w_u16(packet + 24u, second);
            second = r_u16(packet + 8u);
            first = r_u16(packet + 10u);
            w_u16(packet + 16u, second + (uint32)scaled);
            bottom = (uint16)rectangle->h;
            second = r_u8(packet + 12u);
            v = r_u8(packet + 13u);
            height -= 1u;
            w_u8(packet + 28u, u);
            w_u8(packet + 21u, v);
            right = r_u16(packet + 16u);
            w_u16(packet + 26u, first + bottom);
            bottom = r_u16(packet + 26u);
            w_u8(packet + 20u, second + span);
            first = r_u8(packet + 13u);
            second = r_u8(packet + 20u);
            w_u8(packet + 29u, first + height);
            w_u16(packet + 32u, right);
            w_u16(packet + 34u, bottom);
            w_u8(packet + 36u, second);
            w_u8(packet + 37u, r_u8(packet + 29u));
            clut = GetClut(r_s16(atlas + 4u), r_s16(atlas + 6u));
            w_u16(packet + 14u, clut);
            opcode = r_u8(packet + 7u);
            w_u8(packet + 4u, color);
            w_u8(packet + 5u, color >> 8);
            w_u8(packet + 7u, opcode | 2u);
            w_u8(packet + 6u, color >> 16);
            AddPrim(psx_addr(r_u32(0x801132BCu) + 4u, 4u), psx_addr(packet, 40u));
            product = (sint32)rectangle->w * r_s16(atlas + 14u);
            if (measured == 0)
                abort();
            scaled = product / measured;
            w_u32(0x801132B8u, packet + 40u);
            advance += (uint32)scaled;
        }
        character = (uint8)*++text;
    }
}

void front_draw_scaled_text(uint32 text, uint32 rectangle, uint32 color, uint32 blend)
{
    uint32 length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    front_draw_scaled_text_native((const char *)psx_addr(text, (size_t)length + 1u), (const PSX_RECT *)psx_addr(rectangle, 8u), color, blend);
}

void front_draw_gradient_rectangle(const PSX_RECT *rectangle, const uint8 *colors, uint32 blend, uint32 depth)
{
    uint32 packet;
    uint32 table;
    uint32 mode_packet;
    uint32 vertex;
    uint32 channel;
    uint32 y;
    uint32 height;
    uint32 opcode;
    uint16 page;
    FUNCTION_MARKER(0x800A97BC, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    w_u8(packet + 3u, 8u);
    w_u8(packet + 7u, 0x38u);
    for (vertex = 0u; vertex < 4u; ++vertex) {
        for (channel = 0u; channel < 3u; ++channel)
            w_u8(packet + 4u + vertex * 8u + channel, colors[vertex * 4u + channel]);
    }
    w_u16(packet + 8u, (uint16)rectangle->x);
    w_u16(packet + 10u, (uint16)rectangle->y);
    w_u16(packet + 16u, (uint16)rectangle->x + (uint32)(uint16)rectangle->w);
    w_u16(packet + 18u, (uint16)rectangle->y);
    w_u16(packet + 24u, (uint16)rectangle->x);
    w_u16(packet + 26u, (uint16)rectangle->y + (uint32)(uint16)rectangle->h);
    w_u16(packet + 32u, (uint16)rectangle->x + (uint32)(uint16)rectangle->w);
    y = (uint16)rectangle->y;
    opcode = r_u8(packet + 7u);
    height = (uint16)rectangle->h;
    w_u8(packet + 7u, opcode | 2u);
    w_u16(packet + 34u, y + height);
    table = r_u32(0x801132BCu) + (depth << 2);
    mode_packet = packet + 36u;
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 36u));
    w_u32(0x801132B8u, mode_packet);
    page = GetTPage(0, (sint16)blend, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 0, page, NULL);
    table = r_u32(0x801132BCu) + (depth << 2);
    AddPrim(psx_addr(table, 4u), psx_addr(mode_packet, 12u));
    w_u32(0x801132B8u, mode_packet + 12u);
}

void front_draw_blended_rectangle(const PSX_RECT *rectangle, uint32 color, uint32 blend, uint32 depth)
{
    uint32 packet;
    uint32 table;
    uint32 mode_packet;
    uint32 mode = blend;
    uint16 page;
    FUNCTION_MARKER(0x800A998C, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    w_u8(packet + 3u, 3u);
    w_u8(packet + 7u, 0x60u);
    w_u8(packet + 4u, color);
    w_u8(packet + 5u, color >> 8);
    w_u8(packet + 6u, color >> 16);
    w_u16(packet + 8u, (uint16)rectangle->x);
    w_u16(packet + 10u, (uint16)rectangle->y);
    w_u16(packet + 12u, (uint16)rectangle->w);
    w_u16(packet + 14u, (uint16)rectangle->h);
    if ((sint16)blend < 4)
        w_u8(packet + 7u, r_u8(packet + 7u) | 2u);
    else
        mode = 0u;
    mode_packet = packet + 16u;
    table = r_u32(0x801132BCu) + (depth << 2);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 16u));
    w_u32(0x801132B8u, mode_packet);
    page = GetTPage(0, (sint16)mode, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 0, page, NULL);
    table = r_u32(0x801132BCu) + (depth << 2);
    AddPrim(psx_addr(table, 4u), psx_addr(mode_packet, 12u));
    w_u32(0x801132B8u, mode_packet + 12u);
}

void front_draw_bottom_red_rectangle(void)
{
    uint32 packet;
    uint32 mode_packet;
    uint32 depth;
    uint32 table;
    uint16 page;
    FUNCTION_MARKER(0x800A9AC4, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    w_u8(packet + 3u, 3u);
    w_u8(packet + 7u, 0x60u);
    w_u8(packet + 4u, 100u);
    w_u16(packet + 10u, 400u);
    w_u16(packet + 12u, 640u);
    w_u8(packet + 5u, 0u);
    w_u8(packet + 6u, 0u);
    w_u16(packet + 8u, 0u);
    w_u16(packet + 14u, 80u);
    mode_packet = packet + 16u;
    depth = r_u32(0x801132A8u);
    table = r_u32(0x801132BCu) + (depth << 2);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 16u));
    w_u32(0x801132B8u, mode_packet);
    page = GetTPage(0, 0, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 0, page, NULL);
    depth = r_u32(0x801132A8u);
    table = r_u32(0x801132BCu) + (depth << 2);
    AddPrim(psx_addr(table, 4u), psx_addr(mode_packet, 12u));
    w_u32(0x801132B8u, mode_packet + 12u);
}

void front_commit_display_options(void)
{
    uint32 first;
    uint32 second;
    uint32 third;
    FUNCTION_MARKER(0x800A9364, "FRONT.OVL");
    first = r_u32(0x800F6F9Cu);
    second = r_u32(0x800F6FACu);
    third = r_u32(0x800F6FB0u);
    w_u16(0x800118F4u, first);
    w_u16(0x800118F8u, second);
    w_u16(0x800118F6u, third);
}

uint32 front_confirm_display_options(void)
{
    FUNCTION_MARKER(0x800A939C, "FRONT.OVL");
    w_u16(0x800F3114u, 1u);
    w_u16(0x800F3116u, 1u);
    return 1u;
}

void front_play_requested_intro(void)
{
    FUNCTION_MARKER(0x800A96E4, "FRONT.OVL");
    if (r_u32(0x800BD3DCu) != 0u) {
        front_play_intro_from_menu();
        w_u32(0x800BD3DCu, 0u);
    }
}

uint32 front_request_intro(void)
{
    FUNCTION_MARKER(0x800A9710, "FRONT.OVL");
    w_u32(0x800BD3DCu, 1u);
    return 1u;
}

void front_request_mode_five(void)
{
    FUNCTION_MARKER(0x800A9724, "FRONT.OVL");
    w_u32(0x800F3148u, 5u);
    w_u8(0x800FB748u, 1u);
    w_u16(0x80011930u, 1u);
    front_clear_menu_transition();
}

void front_request_mode_six(void)
{
    sint32 screen;
    FUNCTION_MARKER(0x800A9768, "FRONT.OVL");
    w_u32(0x800F3148u, 6u);
    screen = r_s16(0x8010D0DCu);
    w_u32(0x800313C8u, 0u);
    w_u16(0x80011930u, 1u);
    w_u32(0x8003113Cu, (uint32)(screen - 31));
    front_leave_menu();
}

void front_update_display_options_menu(void)
{
    PSX_RECT rectangle;
    sint32 option;
    uint32 row;
    uint32 choice;
    uint32 choices;
    uint32 setting;
    uint32 color;
    uint32 value;
    uint32 text_x;
    uint32 label;
    uint32 expected;
    uint32 texture;
    FUNCTION_MARKER(0x800A8BF4, "FRONT.OVL");
    option = r_s16(0x8010D0DCu) - 31;
    rectangle.w = 100;
    rectangle.h = 0;
    for (row = 0u; row < 3u; ++row) {
        setting = row == 0u ? 0x800F6F9Cu : row == 1u ? 0x800F6FB0u : 0x800F6FACu;
        color = 0x404040u;
        if (option == (sint32)row) {
            color = 0x808080u;
            if (row == 0u) {
                if ((r_u32(0x80053684u) & 0x8000u) != 0u) {
                    value = r_u32(setting);
                    if ((sint32)value > 0)
                        w_u32(setting, value - 1u);
                }
                if ((r_u32(0x80053684u) & 0x2000u) != 0u) {
                    value = r_u32(setting);
                    if ((sint32)value < 2)
                        w_u32(setting, value + 1u);
                }
            } else {
                if ((r_u32(0x8004616Cu) & 0x8000u) != 0u)
                    w_u32(setting, 1u);
                if ((r_u32(0x8004616Cu) & 0x2000u) != 0u)
                    w_u32(setting, 0u);
            }
        }
        w_u32(0x800FB870u, 0x154u);
        rectangle.x = 40;
        rectangle.y = (sint16)(100u + row * 50u);
        front_draw_colored_text(r_u32(r_u32(0x800F6F98u) + row * 4u), 40u, (uint32)(sint32)rectangle.y, color);
        choices = row == 0u ? 3u : 2u;
        for (choice = 0u; choice < choices; ++choice) {
            text_x = r_u32((row == 0u ? 0x800F6FA0u : 0x800F6FB4u) + choice * 4u);
            if (choice == 0u) {
                w_u32(0x800FB870u, 0xFDu);
                rectangle.y = (sint16)((uint16)rectangle.y + 15u);
            } else {
                rectangle.x = (sint16)(40u + choice * 125u);
            }
            if (row == 0u)
                label = choice == 0u ? 0x14u : choice == 1u ? 0x10u : 0x18u;
            else
                label = choice == 0u ? 0x1Cu : 0x20u;
            front_draw_colored_text(r_u32(r_u32(0x800F6F98u) + label), text_x, (uint32)((sint32)rectangle.y + 4), color);
            expected = row == 0u ? choice : 1u - choice;
            value = r_u32(setting);
            if (choice == 0u) {
                texture = value == expected ? 0x1B5u : 0x1B8u;
                front_draw_left_frame(texture, &rectangle, color);
            } else {
                texture = value == expected ? 0x1AFu : 0x1B2u;
                if (row == 0u && choice == 1u)
                    front_draw_middle_frame(texture, &rectangle, color);
                else
                    front_draw_right_frame(texture, &rectangle, color);
            }
        }
    }
}

void front_snapshot_sound_options(void)
{
    uint32 music;
    uint32 effects;
    uint32 voice;
    uint32 output;
    FUNCTION_MARKER(0x800A8860, "FRONT.OVL");
    music = (uint32)r_s16(0x800118FAu);
    effects = (uint32)r_s16(0x800118FCu);
    voice = (uint32)r_s16(0x800118FEu);
    output = (uint32)r_s16(0x80011900u);
    w_u32(0x800F2D5Cu, music);
    w_u32(0x800F2D60u, effects);
    w_u32(0x800F2D64u, voice);
    w_u32(0x800F2D68u, output);
}

void front_restore_sound_options(void)
{
    uint32 music;
    uint32 effects;
    uint32 voice;
    uint32 current;
    uint32 output;
    uint32 sample;
    uint32 flags;
    sint32 reserved;
    FUNCTION_MARKER(0x800A88A8, "FRONT.OVL");
    stream_cancel_current();
    music = r_u32(0x800F2D5Cu);
    effects = r_u32(0x800F2D60u);
    voice = r_u32(0x800F2D64u);
    current = (uint32)r_s16(0x80011900u);
    output = r_u32(0x800F2D68u);
    w_u16(0x800118FAu, music);
    w_u16(0x800118FCu, effects);
    w_u16(0x800118FEu, voice);
    if (current == output)
        return;
    sound_stop_voice(r_u32(0x800F3130u));
    sound_tick();
    sound_stop_voice(r_u32(0x800F3134u));
    sound_tick();
    output = r_u32(0x800F2D68u);
    w_u16(0x80011900u, output);
    sound_select_mode(output & 255u);
    sample = r_u32(0x80042970u);
    flags = r_u32(0x800313A0u);
    reserved = sound_reserve_voice((sint32)sample, 50u, 15, 15, 0x100u, flags);
    sample = r_u32(0x80042970u);
    flags = r_u32(0x800313A0u);
    w_u32(0x800F3130u, (uint32)reserved);
    reserved = sound_reserve_voice((sint32)sample, 50u, 15, 15, 0x89u, flags);
    w_u32(0x800F3134u, (uint32)reserved);
}

void front_initialize_display_options_menu(void)
{
    static const uint32 offsets[7] = {0x14u, 0x10u, 0x18u, 0x1Cu, 0x20u, 0x24u, 0x28u};
    static const uint32 destinations[7] = {0x800F6FA0u, 0x800F6FA4u, 0x800F6FA8u, 0x800F6FB4u, 0x800F6FB8u, 0x800F6FC0u, 0x800F6FC4u};
    static const uint32 centers[7] = {90u, 215u, 340u, 90u, 215u, 90u, 215u};
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 mode;
    uint32 labels;
    uint32 width;
    uint32 half_width;
    uint32 index;
    FUNCTION_MARKER(0x800A89D0, "FRONT.OVL");
    front_configure_layer(0u, 0u, 0xA9u);
    front_configure_layer(1u, 1u, 0xA8u);
    first = (uint32)r_s16(0x800118F4u);
    second = (uint32)r_s16(0x800118F8u);
    third = (uint32)r_s16(0x800118F6u);
    mode = r_u32(0x800311C0u);
    w_u32(0x800F6F98u, 0x800F28F8u);
    labels = r_u32(0x800F6F98u);
    w_u32(0x800F6FDCu, 0u);
    w_u32(0x800FB870u, 0xFDu);
    w_u32(0x800F6F9Cu, first);
    w_u32(0x800F6FACu, second);
    w_u32(0x800F6FB0u, third);
    w_u32(0x800F6FBCu, mode);
    for (index = 0u; index < 7u; ++index) {
        width = front_text_width(r_u32(labels + offsets[index]));
        half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
        labels = r_u32(0x800F6F98u);
        w_u32(destinations[index], centers[index] - half_width);
    }
    labels = r_u32(0x800F6F98u);
    w_u32(0x800FB870u, 0x53u);
    width = front_text_width(r_u32(labels + 0x2Cu));
    half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
    w_u32(0x800F6FC8u, 215u - half_width);
}

void front_draw_volume_bar(uint32 texture, const PSX_RECT *rectangle, uint32 color, sint32 progress)
{
    uint32 total;
    uint32 product;
    uint32 filled;
    uint32 x;
    uint32 y;
    uint32 left_width;
    uint32 right_width = 0u;
    uint32 middle_width = 0u;
    uint32 capacity;
    uint32 section;
    uint32 part;
    uint32 atlas;
    uint32 packet;
    uint32 mode_packet;
    uint32 table;
    uint32 span;
    uint32 first;
    uint32 second;
    uint32 value;
    uint32 shift;
    uint32 offset;
    sint32 depth;
    uint16 clut;
    uint16 page;
    FUNCTION_MARKER(0x800B1C10, "FRONT.OVL");
    total = (uint32)(sint32)rectangle->w;
    product = (uint32)progress * (total - 4u);
    x = (uint32)(sint32)rectangle->x;
    y = (uint32)(sint32)rectangle->y;
    filled = (uint32)((sint32)product / 1000);
    if ((sint32)filled > 0)
        filled += 2u;
    if ((sint32)filled >= (sint32)(total - 2u))
        filled = total;
    atlas = 0x8004EB70u + (texture << 4);
    depth = r_s16(atlas + 12u);
    shift = (2u - (uint32)depth) & 31u;
    left_width = (uint32)r_s16(atlas + 8u) << shift;
    for (section = 0u; section < 3u; ++section) {
        if (section == 1u) {
            total -= left_width;
            filled -= left_width;
            right_width = (uint32)r_s16(0x8004EB90u + (texture << 4) + 8u) << shift;
            middle_width = total - right_width;
        } else if (section == 2u) {
            filled -= middle_width;
        }
        capacity = section == 0u ? left_width : section == 1u ? middle_width : right_width;
        for (part = 0u; part < 2u; ++part) {
            if (part == 0u ? (sint32)filled <= 0 : (sint32)filled >= (sint32)capacity)
                continue;
            atlas = 0x8004EB70u + (texture << 4) + section * 16u + part * 48u;
            packet = r_u32(0x801132B8u);
            if (section != 1u) {
                SetSprt((SPRT *)psx_addr(packet, 20u));
                w_u16(packet + 8u, x);
                w_u16(packet + 10u, y);
                offset = part == 0u || (section == 2u && (sint32)filled <= 0) ? 0u : filled;
                w_u8(packet + 12u, ((r_u16(atlas) & 63u) << shift) + offset);
                value = r_u16(atlas + 2u);
                if (part == 0u) {
                    w_u8(packet + 13u, value);
                    span = (sint32)filled < (sint32)capacity ? filled : (uint32)r_s16(atlas + 8u) << shift;
                    w_u16(packet + 16u, span);
                } else if (section == 0u) {
                    w_u16(packet + 16u, capacity - filled);
                    w_u8(packet + 13u, value);
                } else {
                    w_u8(packet + 13u, value);
                    span = (sint32)filled > 0 ? capacity - filled : capacity;
                    w_u16(packet + 16u, span);
                }
                w_u16(packet + 18u, r_u16(atlas + 10u));
                mode_packet = packet + 20u;
                clut = GetClut(r_s16(atlas + 4u), r_s16(atlas + 6u));
                w_u16(packet + 14u, clut);
                if (section != 2u || part == 0u)
                    value = (uint32)r_s16(packet + 16u);
                w_u8(packet + 4u, color);
                w_u8(packet + 5u, color >> 8);
                w_u8(packet + 6u, color >> 16);
                table = r_u32(0x801132BCu);
                if (section != 2u || part == 0u)
                    x += value;
                AddPrim(psx_addr(table, 4u), psx_addr(packet, 20u));
                page = GetTPage(depth, 1, r_s16(atlas), r_s16(atlas + 2u));
                SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 1, page, NULL);
                table = r_u32(0x801132BCu);
                AddPrim(psx_addr(table, 4u), psx_addr(mode_packet, 12u));
                w_u32(0x801132B8u, mode_packet + 12u);
            } else {
                SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40u));
                w_u16(packet + 8u, x);
                w_u16(packet + 10u, y);
                if (part == 0u)
                    span = (sint32)filled < (sint32)capacity ? filled : capacity;
                else
                    span = (sint32)filled > 0 ? capacity - filled : capacity;
                w_u16(packet + 16u, x + span);
                first = r_u16(packet + 8u);
                w_u16(packet + 24u, first);
                second = r_u16(packet + 10u);
                w_u16(packet + 18u, y);
                w_u16(packet + 26u, second + r_u16(atlas + 10u));
                first = r_u16(packet + 16u);
                second = r_u16(packet + 26u);
                w_u16(packet + 32u, first);
                w_u16(packet + 34u, second);
                w_u8(packet + 12u, (r_u16(atlas) & 63u) << shift);
                value = r_u8(packet + 12u) + 1u;
                first = r_u16(atlas + 2u);
                w_u8(packet + 20u, value);
                w_u8(packet + 13u, first);
                if ((uint8)value == 0u)
                    w_u8(packet + 20u, 255u);
                first = r_u8(packet + 13u);
                second = r_u8(packet + 12u);
                w_u8(packet + 21u, first);
                first = r_u8(packet + 13u);
                w_u8(packet + 28u, second);
                w_u8(packet + 29u, first + r_u8(atlas + 10u));
                first = r_u8(packet + 20u);
                second = r_u8(packet + 29u);
                w_u8(packet + 36u, first);
                w_u8(packet + 37u, second);
                clut = GetClut(r_s16(atlas + 4u), r_s16(atlas + 6u));
                w_u16(packet + 14u, clut);
                page = GetTPage(depth, 1, r_s16(atlas), r_s16(atlas + 2u));
                w_u16(packet + 22u, page);
                first = (uint32)r_s16(packet + 16u);
                second = (uint32)r_s16(packet + 8u);
                w_u8(packet + 4u, color);
                w_u8(packet + 5u, color >> 8);
                x += first - second;
                w_u8(packet + 6u, color >> 16);
                table = r_u32(0x801132BCu);
                AddPrim(psx_addr(table, 4u), psx_addr(packet, 40u));
                w_u32(0x801132B8u, packet + 40u);
            }
        }
    }
}

void front_draw_left_frame(uint32 texture, const PSX_RECT *rectangle, uint32 color)
{
    uint32 packet;
    uint32 quad;
    uint32 atlas;
    uint32 right_atlas;
    uint32 x;
    uint32 y;
    uint32 width;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 value;
    uint32 shift;
    uint32 table;
    uint32 mode_packet;
    sint32 depth;
    sint32 atlas_x;
    sint32 atlas_y;
    uint16 clut;
    uint16 page;
    FUNCTION_MARKER(0x800B1694, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    SetSprt((SPRT *)psx_addr(packet, 20u));
    quad = packet + 20u;
    SetPolyFT4((POLY_FT4 *)psx_addr(quad, 40u));
    atlas = 0x8004EB70u + (texture << 4);
    x = (uint32)(sint32)rectangle->x;
    y = (uint32)(sint32)rectangle->y;
    width = (uint32)(sint32)rectangle->w;
    depth = r_s16(atlas + 12u);
    shift = (2u - (uint32)depth) & 31u;
    w_u16(packet + 8u, x);
    w_u16(packet + 10u, y);
    w_u8(packet + 12u, (r_u16(atlas) & 63u) << shift);
    w_u8(packet + 13u, r_u16(atlas + 2u));
    w_u16(packet + 16u, (uint32)r_s16(atlas + 8u) << shift);
    w_u16(packet + 18u, r_u16(atlas + 10u));
    right_atlas = atlas + 16u;
    clut = GetClut(r_s16(atlas + 4u), r_s16(atlas + 6u));
    first = r_u16(packet + 8u);
    second = r_u16(packet + 16u);
    w_u16(packet + 14u, clut);
    third = r_u16(packet + 10u);
    w_u16(quad + 8u, first + second);
    first = r_u16(quad + 8u);
    w_u16(quad + 10u, third);
    second = r_u16(quad + 10u);
    w_u16(quad + 16u, x + width);
    w_u16(quad + 18u, y);
    w_u16(quad + 24u, first);
    w_u16(quad + 26u, second + r_u16(right_atlas + 10u));
    first = r_u16(quad + 16u);
    second = r_u16(quad + 26u);
    w_u16(quad + 32u, first);
    w_u16(quad + 34u, second);
    w_u8(quad + 12u, (r_u16(right_atlas) & 63u) << shift);
    value = r_u8(quad + 12u) + 1u;
    first = r_u16(right_atlas + 2u);
    w_u8(quad + 20u, value);
    w_u8(quad + 13u, first);
    if ((uint8)value == 0u)
        w_u8(quad + 20u, 255u);
    first = r_u8(quad + 13u);
    second = r_u8(quad + 12u);
    w_u8(quad + 21u, first);
    first = r_u8(quad + 13u);
    w_u8(quad + 28u, second);
    w_u8(quad + 29u, first + r_u8(right_atlas + 10u));
    first = r_u8(quad + 20u);
    second = r_u8(quad + 29u);
    w_u8(quad + 36u, first);
    w_u8(quad + 37u, second);
    clut = GetClut(r_s16(right_atlas + 4u), r_s16(right_atlas + 6u));
    w_u16(quad + 14u, clut);
    page = GetTPage(depth, 1, r_s16(right_atlas), r_s16(right_atlas + 2u));
    w_u16(quad + 22u, page);
    w_u8(packet + 4u, color);
    w_u8(packet + 5u, color >> 8);
    w_u8(packet + 6u, color >> 16);
    w_u8(quad + 4u, color);
    w_u8(quad + 5u, color >> 8);
    w_u8(quad + 6u, color >> 16);
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, packet + 60u);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 20u));
    atlas_x = r_s16(atlas);
    atlas_y = r_s16(atlas + 2u);
    mode_packet = r_u32(0x801132B8u);
    page = GetTPage(depth, 1, atlas_x, atlas_y);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 1, page, NULL);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(mode_packet, 12u));
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, mode_packet + 12u);
    AddPrim(psx_addr(table, 4u), psx_addr(quad, 40u));
}

void front_draw_right_frame(uint32 texture, const PSX_RECT *rectangle, uint32 color)
{
    uint32 packet;
    uint32 quad;
    uint32 atlas;
    uint32 left_atlas;
    uint32 x;
    uint32 y;
    uint32 width;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 value;
    uint32 shift;
    uint32 table;
    uint32 mode_packet;
    sint32 depth;
    sint32 atlas_x;
    sint32 atlas_y;
    uint16 clut;
    uint16 page;
    FUNCTION_MARKER(0x800B195C, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    SetSprt((SPRT *)psx_addr(packet, 20u));
    quad = packet + 20u;
    SetPolyFT4((POLY_FT4 *)psx_addr(quad, 40u));
    atlas = 0x8004EB90u + (texture << 4);
    depth = r_s16(atlas + 12u);
    first = r_u16(atlas);
    x = (uint32)(sint32)rectangle->x;
    y = (uint32)(sint32)rectangle->y;
    width = (uint32)(sint32)rectangle->w;
    shift = (2u - (uint32)depth) & 31u;
    w_u8(packet + 12u, (first & 63u) << shift);
    w_u8(packet + 13u, r_u16(atlas + 2u));
    w_u16(packet + 16u, (uint32)r_s16(atlas + 8u) << shift);
    w_u16(packet + 18u, r_u16(atlas + 10u));
    value = (uint32)r_s16(atlas + 8u) << shift;
    w_u16(packet + 10u, y);
    w_u16(packet + 8u, x + width - value);
    left_atlas = atlas - 16u;
    clut = GetClut(r_s16(atlas + 4u), r_s16(atlas + 6u));
    first = r_u16(packet + 8u);
    second = r_u16(packet + 10u);
    w_u16(quad + 8u, x);
    third = r_u16(quad + 8u);
    w_u16(packet + 14u, clut);
    w_u16(quad + 10u, y);
    value = r_u16(quad + 10u);
    w_u16(quad + 16u, first);
    w_u16(quad + 18u, second);
    w_u16(quad + 24u, third);
    w_u16(quad + 26u, value + r_u16(left_atlas + 10u));
    first = r_u16(quad + 16u);
    second = r_u16(quad + 26u);
    w_u16(quad + 32u, first);
    w_u16(quad + 34u, second);
    w_u8(quad + 12u, (r_u16(left_atlas) & 63u) << shift);
    value = r_u8(quad + 12u) + 1u;
    first = r_u16(left_atlas + 2u);
    w_u8(quad + 20u, value);
    w_u8(quad + 13u, first);
    if ((uint8)value == 0u)
        w_u8(quad + 20u, 255u);
    first = r_u8(quad + 13u);
    second = r_u8(quad + 12u);
    w_u8(quad + 21u, first);
    first = r_u8(quad + 13u);
    w_u8(quad + 28u, second);
    w_u8(quad + 29u, first + r_u8(left_atlas + 10u));
    first = r_u8(quad + 20u);
    second = r_u8(quad + 29u);
    w_u8(quad + 36u, first);
    w_u8(quad + 37u, second);
    clut = GetClut(r_s16(left_atlas + 4u), r_s16(left_atlas + 6u));
    w_u16(quad + 14u, clut);
    page = GetTPage(depth, 1, r_s16(left_atlas), r_s16(left_atlas + 2u));
    w_u16(quad + 22u, page);
    w_u8(packet + 4u, color);
    w_u8(packet + 5u, color >> 8);
    w_u8(quad + 4u, color);
    w_u8(packet + 6u, color >> 16);
    w_u8(quad + 5u, color >> 8);
    w_u8(quad + 6u, color >> 16);
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, packet + 60u);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 20u));
    atlas_x = r_s16(atlas);
    atlas_y = r_s16(atlas + 2u);
    mode_packet = r_u32(0x801132B8u);
    page = GetTPage(depth, 1, atlas_x, atlas_y);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 1, page, NULL);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(mode_packet, 12u));
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, mode_packet + 12u);
    AddPrim(psx_addr(table, 4u), psx_addr(quad, 40u));
}

void front_draw_middle_frame(uint32 texture, const PSX_RECT *rectangle, uint32 color)
{
    uint32 packet;
    uint32 atlas;
    uint32 x;
    uint32 y;
    uint32 width;
    uint32 first;
    uint32 second;
    uint32 value;
    uint32 shift;
    uint32 table;
    sint32 depth;
    uint16 clut;
    uint16 page;
    FUNCTION_MARKER(0x800B14F8, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40u));
    atlas = 0x8004EB80u + (texture << 4);
    x = (uint32)(sint32)rectangle->x;
    y = (uint32)(sint32)rectangle->y;
    width = (uint32)(sint32)rectangle->w;
    depth = r_s16(atlas + 12u);
    w_u16(packet + 8u, x);
    first = r_u16(packet + 8u);
    w_u16(packet + 10u, y);
    w_u16(packet + 16u, x + width);
    second = r_u16(packet + 10u);
    w_u16(packet + 18u, y);
    w_u16(packet + 24u, first);
    w_u16(packet + 26u, second + r_u16(atlas + 10u));
    second = r_u16(packet + 26u);
    first = r_u16(packet + 16u);
    w_u16(packet + 34u, second);
    w_u16(packet + 32u, first);
    shift = (2u - (uint32)depth) & 31u;
    w_u8(packet + 12u, (r_u16(atlas) & 63u) << shift);
    value = r_u8(packet + 12u) + 1u;
    first = r_u16(atlas + 2u);
    w_u8(packet + 20u, value);
    w_u8(packet + 13u, first);
    if ((uint8)value == 0u)
        w_u8(packet + 20u, 255u);
    first = r_u8(packet + 13u);
    second = r_u8(packet + 12u);
    w_u8(packet + 21u, first);
    first = r_u8(packet + 13u);
    w_u8(packet + 28u, second);
    w_u8(packet + 29u, first + r_u8(atlas + 10u));
    first = r_u8(packet + 20u);
    second = r_u8(packet + 29u);
    w_u8(packet + 36u, first);
    w_u8(packet + 37u, second);
    clut = GetClut(r_s16(atlas + 4u), r_s16(atlas + 6u));
    w_u16(packet + 14u, clut);
    page = GetTPage(depth, 1, r_s16(atlas), r_s16(atlas + 2u));
    w_u16(packet + 22u, page);
    w_u8(packet + 4u, color);
    w_u8(packet + 5u, color >> 8);
    w_u8(packet + 6u, color >> 16);
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, packet + 40u);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 40u));
}

void front_restore_diagnostic_overlay(void)
{
    uint32 source, destination, offset, words[4];
    sint32 entries;
    uint32 pass;
    FUNCTION_MARKER(0x800AADE0, "FRONT.OVL");
    entries = r_s16(0x8010D82Cu);
    front_set_ordering_table_size((uint32)entries);
    source = 0x8010D7D0u;
    destination = 0x800FB878u;
    for (offset = 0u; offset < 80u; offset += 16u) {
        words[0] = r_u32(source + offset);
        words[1] = r_u32(source + offset + 4u);
        words[2] = r_u32(source + offset + 8u);
        words[3] = r_u32(source + offset + 12u);
        w_u32(destination + offset, words[0]);
        w_u32(destination + offset + 4u, words[1]);
        w_u32(destination + offset + 8u, words[2]);
        w_u32(destination + offset + 12u, words[3]);
    }
    words[0] = r_u32(source + 80u);
    words[1] = r_u32(source + 84u);
    words[2] = r_u32(source + 88u);
    w_u32(destination + 80u, words[0]);
    w_u32(destination + 84u, words[1]);
    w_u32(destination + 88u, words[2]);
    for (pass = 0u; pass < 3u; ++pass) {
        DrawSync(0);
        PutDrawEnv((DRAWENV *)psx_addr(0x800FB878u, sizeof(DRAWENV)));
        front_swap_render_buffers();
    }
}

uint32 front_wrap_briefing_text(uint32 text, uint32 panel)
{
    uint32 end, search, count, line, length, total, hard_end, span;
    uint32 value, character;
    const char *native_text, *found;
    FUNCTION_MARKER(0x800AC668, "FRONT.OVL");
    total = 0u;
    hard_end = 0u;
    w_u32(panel + 8u, 0u);
    while (r_u8(text) != 0u) {
        span = 0u;
        while (r_u8(text + span) != 0u)
            ++span;
        native_text = (const char *)psx_addr(text, span + 1u);
        found = strchr(native_text, 13);
        end = found ? text + (uint32)(found - native_text) : 0u;
        count = r_u32(panel + 8u);
        w_u32(panel + 0x994u + (count << 2), 0xFFFFFFFEu);
        if (end != 0u && (sint32)(end - text) < 39) {
            hard_end = 1u;
        } else {
            end = text;
            search = text;
            while (search != 0u && (sint32)(search - text) < 38) {
                end = search;
                found = strchr(native_text + (uint32)(search - text) + 1u, ' ');
                search = found ? text + (uint32)(found - native_text) : 0u;
                count = r_u32(panel + 8u);
                value = r_u32(panel + 0x994u + (count << 2));
                w_u32(panel + 0x994u + (count << 2), value + 1u);
            }
            if (end == text) {
                hard_end = 1u;
                end = text + (uint32)strlen(native_text);
            }
        }
        length = end - text;
        count = r_u32(panel + 8u);
        line = panel + 0x34u + count * 44u;
        if (length != 0u)
            strncpy((char *)psx_addr(line, length), (const char *)psx_addr(text, length), length);
        count = r_u32(panel + 8u);
        total += length;
        w_u8(panel + 0x34u + count * 44u + length, 0u);
        if (hard_end != 0u) {
            count = r_u32(panel + 8u);
            w_u32(panel + 0x8CCu + (count << 2), 0u);
        } else {
            count = r_u32(panel + 8u);
            value = front_text_width_without_brackets(panel + 0x34u + count * 44u);
            count = r_u32(panel + 8u);
            w_u32(panel + 0x8CCu + (count << 2), 344u - value);
        }
        hard_end = 0u;
        text = end + 1u;
        character = r_u8(text);
        while (character == 13u || character == 10u) {
            ++text;
            character = r_u8(text);
        }
        count = r_u32(panel + 8u) + 1u;
        w_u32(panel + 8u, count);
        if ((sint32)count >= 50)
            break;
    }
    return total;
}

void front_play_numbered_movie(sint32 number)
{
    char path[32];
    FUNCTION_MARKER(0x800AC880, "FRONT.OVL");
    if (number < 20)
        sprintf(path, "\\ACT\\ACT%d.STR;1", number);
    else
        sprintf(path, "\\END\\END%d.STR;1", number - 20);
    front_play_menu_movie(path, 30u);
}

void front_initialize_briefing_view(void)
{
    char name[24];
    char *extension;
    uint32 total, value, first, second, length, source, base, record;
    uint32 index, size, buffer, words[3], offset;
    sint32 divisor, scaled, numerator;
    FUNCTION_MARKER(0x800AC8D8, "FRONT.OVL");
    total = 0u;
    front_configure_layer(0u, 0u, 0xA9u);
    front_configure_layer(1u, 3u, 0xA8u);
    value = r_u32(0x800BD3F8u);
    w_u32(0x800BD408u, 100u);
    w_u32(0x800BD404u, 1u);
    w_u32(0x800BD40Cu, 0u);
    if (value == 0u) {
        if (r_u32(0x800BD424u) == 0u) {
            if (r_s16(0x8005960Cu) == 0) {
                w_u16(0x8005960Cu, 1u);
                w_u16(0x8005960Eu, 1u);
            }
            first = r_u16(0x8005960Cu);
            second = r_u16(0x8005960Eu);
            w_u16(0x800595F8u, first);
            w_u16(0x800595FAu, second);
            sprintf((char *)psx_addr(0x80010518u, 17u), "%d_%d.CW",
                (sint32)(sint16)first, (sint32)(sint16)second);
        }
        if (r_u32(0x800F313Cu) == 0u && r_s16(0x800118F6u) == 1) {
            first = (uint32)(sint32)r_s16(0x800595F8u);
            if ((sint32)first > 0 && r_s16(0x800595FAu) == 1)
                front_play_numbered_movie((sint32)first);
            w_u32(0x800F313Cu, 1u);
        }
        length = 0u;
        while (r_u8(0x80010518u + length) != 0u)
            ++length;
        strcpy(name, (const char *)psx_addr(0x80010518u, length + 1u));
        extension = strchr(name, '.');
        if (extension != NULL)
            *extension = 0;
        strcat(name, ".BRF");
        size = disc_file_size(name);
        buffer = heap_alloc_back(size + 4u);
        w_u32(0x800BD3FCu, buffer);
        if (buffer != 0u) {
            resource_load_into(name, buffer);
            buffer = r_u32(0x800BD3FCu);
            w_u8(buffer + size, 0u);
            buffer = r_u32(0x800BD3FCu);
            w_u8(buffer + size + 1u, 0u);
            buffer = r_u32(0x800BD3FCu);
            w_u32(0x800FB870u, 0xFDu);
            total = front_wrap_briefing_text(buffer, 0x800BD3F8u);
            w_u32(0x800BD404u, 1u);
        } else {
            w_u32(0x800BD400u, 0u);
        }
        first = (uint32)(sint32)r_s16(0x800595FAu);
        second = (uint32)(sint32)r_s16(0x800595F8u);
        w_u32(0x8003119Cu, 320u);
        w_u32(0x800F6F98u, 8u);
        index = (first - 1u) * 3u + second * 18u;
        value = r_u8(0x800BE04Cu + index);
        first = r_u8(0x800BE04Du + index);
        w_u32(0x800BD420u, first);
        if (first == 0u)
            w_u32(0x800BD420u, value);
        else
            w_u32(0x800BD420u, first + 9u);
        first = r_u32(0x800BD420u);
        w_u32(0x800BD420u, first * 3u);
        source = r_u32(0x800BE1B4u + (value << 2));
        length = 0u;
        while (r_u8(source + length) != 0u)
            ++length;
        front_prepare_control_model_bundle((const char *)psx_addr(source, length + 1u));
        first = (uint32)(sint32)r_s16(0x800595FAu);
        w_u16(0x8001192Cu, value);
        second = (uint32)(sint32)r_s16(0x800595F8u);
        index = (first - 1u) * 3u + second * 18u;
        w_u16(0x8001192Eu, r_u8(0x800BE04Eu + index));
        disc_wait_stream();
        front_create_control_viewer_objects();
        base = r_u32(0x80046178u);
        w_u32(0x800F6FCCu, 320u);
        w_u32(0x800F6FD0u, 300u);
        source = r_u32(base);
        w_u32(0x800BC6E4u, 250u);
        value = mesh_import_primitives(source);
        second = (uint32)(sint32)r_s16(0x800595F8u);
        w_u32(0x800BC61Cu, value);
        if ((sint32)second > 0 || r_s16(0x800595FAu) > 0) {
            w_u32(0x800BD428u, 0u);
            disc_wait_stream();
            VSync(0);
            second = (uint32)(sint32)r_s16(0x800595F8u);
            first = (uint32)(sint32)r_s16(0x800595FAu);
            index = r_u32(0x800BDE50u + second * 24u + first * 4u);
            base = r_u32(0x800F708Cu);
            record = base + index * 24u;
            first = r_u32(record + 20u);
            second = r_u32(record + 28u);
            stream_request_segments(record + 16u, first, second, 0x800118FEu, 1, 0u, NULL);
            second = (uint32)(sint32)r_s16(0x800595F8u);
            first = (uint32)(sint32)r_s16(0x800595FAu);
            index = r_u32(0x800BDE50u + second * 24u + first * 4u);
            base = r_u32(0x800F708Cu);
            record = base + index * 24u;
            for (offset = 0u; offset < 24u; offset += 12u) {
                words[0] = r_u32(record + 8u + offset);
                words[1] = r_u32(record + 12u + offset);
                words[2] = r_u32(record + 16u + offset);
                w_u32(0x800105D0u + offset, words[0]);
                w_u32(0x800105D4u + offset, words[1]);
                w_u32(0x800105D8u + offset, words[2]);
            }
        }
        if ((sint32)total > 0) {
            second = (uint32)(sint32)r_s16(0x800595F8u);
            first = (uint32)(sint32)r_s16(0x800595FAu);
            index = r_u32(0x800BDE50u + second * 24u + first * 4u);
            base = r_u32(0x800F708Cu);
            record = base + index * 24u;
            value = r_u32(0x800311C0u);
            divisor = r_s32(record + 28u);
            divisor /= value != 0u ? 6 : 5;
            if (divisor == 0)
                abort();
            scaled = 0x01900000 / divisor;
            second = (uint32)(sint32)r_s16(0x800595F8u);
            w_u32(0x800BD418u, (uint32)scaled);
            first = (uint32)(sint32)r_s16(0x800595FAu);
            index = r_u32(0x800BDE50u + second * 24u + first * 4u);
            base = r_u32(0x800F708Cu);
            record = base + index * 24u;
            value = r_u32(record + 28u);
            first = r_u32(0x800311C0u);
            divisor = (sint32)(value - 600u);
            divisor /= first != 0u ? 6 : 5;
            numerator = (sint32)((uint32)divisor << 8);
            divisor = numerator / (sint32)total;
            if (divisor == 0)
                abort();
            w_u32(0x800BD410u, (uint32)(65536 / divisor));
        } else {
            w_u32(0x800BD410u, 50u);
            w_u32(0x800BD418u, 50u);
        }
        front_reset_menu_packet_lists();
    } else {
        w_u32(0x800BD3F8u, 0u);
    }
    if (r_s16(0x800595F8u) > 0 || r_s16(0x800595FAu) > 0) {
        VSync(0);
        second = (uint32)(sint32)r_s16(0x800595F8u);
        first = (uint32)(sint32)r_s16(0x800595FAu);
        index = r_u32(0x800BDE50u + second * 24u + first * 4u);
        base = r_u32(0x800F708Cu);
        record = base + index * 24u;
        first = r_u32(record + 20u);
        second = r_u32(record + 28u);
        stream_request_segments(record + 16u, first, second, 0x800118FEu, 1, 0u, NULL);
    }
    w_u32(0x800BD41Cu, 0u);
    w_u32(0x800BD414u, 0u);
    value = heap_free_bytes();
    w_u32(0x80113208u, value);
    value = heap_coalesce_largest();
    first = (uint32)(sint32)r_s16(0x800595F8u);
    w_u32(0x800FB7F8u, value);
    if (first == 0u) {
        w_u8(0x80010210u, 0u);
    } else {
        second = (uint32)(sint32)r_s16(0x800595FAu);
        source = r_u32(0x800F24B8u + first * 24u + second * 4u);
        length = 0u;
        while (r_u8(source + length) != 0u)
            ++length;
        strcpy((char *)psx_addr(0x80010210u, length + 1u), (const char *)psx_addr(source, length + 1u));
    }
}

void front_draw_briefing_heap_diagnostics(void)
{
    char text[32];
    FUNCTION_MARKER(0x800ADA4C, "FRONT.OVL");
    sprintf(text, "amount:%d", r_s32(0x80113208u));
    front_draw_colored_text_native(text, 320u, 240u, 0x808080u);
    sprintf(text, "amount:%d", r_s32(0x800FB7F8u));
    front_draw_colored_text_native(text, 320u, 280u, 0x808080u);
}

uint32 front_retain_briefing_view(void)
{
    FUNCTION_MARKER(0x800ADBE8, "FRONT.OVL");
    w_u32(0x800BD3F8u, 1u);
    return stream_stop();
}

void front_free_briefing_view(void)
{
    uint32 retained, pointer;
    FUNCTION_MARKER(0x800ADC1C, "FRONT.OVL");
    retained = r_u32(0x800BD3F8u);
    if (retained != 0u)
        return;
    stream_stop();
    pointer = r_u32(0x800FB874u);
    if (pointer != 0u)
        heap_free(pointer);
    pointer = r_u32(0x800BD3FCu);
    if (pointer != 0u)
        heap_free(pointer);
    w_u32(0x800FB874u, 0u);
    w_u32(0x800BD3FCu, 0u);
    w_u32(0x800BD424u, 0u);
    object_update_all();
    object_initialize_pool();
    front_free_preview_models();
}

uint32 front_leave_briefing_view(void)
{
    uint32 pointer;
    FUNCTION_MARKER(0x800ADCB0, "FRONT.OVL");
    stream_stop();
    pointer = r_u32(0x800FB874u);
    if (pointer != 0u)
        heap_free(pointer);
    pointer = r_u32(0x800BD3FCu);
    if (pointer != 0u)
        heap_free(pointer);
    w_u32(0x800FB874u, 0u);
    w_u32(0x800BD3FCu, 0u);
    return front_leave_menu();
}

void front_draw_briefing_view(void)
{
    uint32 layout[6];
    sint16 points[4];
    uint32 scroll, row, limit, text, last, cursor, result, packet, table;
    uint32 value, object, header, shift, next, link, captured_table;
    uint32 first, second;
    FUNCTION_MARKER(0x800AD0C8, "FRONT.OVL");
    scroll = 0u;
    if (r_u32(0x80040FE4u) == 6u || r_s16(0x800118FEu) == 0)
        w_u32(0x800BD428u, 1u);
    w_u32(0x800FB870u, 0xFDu);
    value = r_u32(0x800BE1DCu);
    first = (uint32)(sint32)r_s16(0x800595F8u);
    w_u32(0x800BE1DCu, value + 1u);
    if (first != 0u) {
        second = (uint32)(sint32)r_s16(0x800595FAu);
        text = r_u32(0x800F24B8u + first * 24u + second * 4u);
        front_draw_colored_text(text, 46u, 64u, 0x808080u);
    }
    front_reset_text_emphasis();
    row = 1u;
    layout[2] = 0x505060u;
    layout[1] = r_u32(0x800BD408u);
    limit = r_u32(0x800BD400u);
    text = 0x800BD458u;
    while ((sint32)row < (sint32)limit) {
        last = r_u32(0x800BD404u);
        if ((sint32)last < (sint32)row)
            break;
        layout[0] = 46u;
        cursor = 0u;
        if (layout[1] - 100u < 80u) {
            layout[3] = r_u32(0x800BDCC4u + row * 4u);
            layout[4] = r_u32(0x800BDD8Cu + row * 4u);
            layout[5] = 38u;
            if (row == last) {
                layout[5] = r_u32(0x800BD40Cu);
                result = front_draw_emphasized_text_native(text, layout);
                if ((sint32)result > 0) {
                    value = r_u32(0x800BD404u);
                    w_u32(0x800BD40Cu, 0u);
                    w_u32(0x800BD404u, value + 1u);
                }
                cursor = r_u32(0x800BE1DCu) & 2u;
            } else {
                front_draw_emphasized_text_native(text, layout);
            }
            if ((sint32)layout[1] >= 161)
                scroll = 1u;
        } else if ((sint32)layout[1] < 100 && (sint32)(layout[1] + 20u) > 100) {
            first = 100u - layout[1];
            layout[3] = r_u32(0x800BDCC4u + row * 4u);
            layout[4] = r_u32(0x800BDD8Cu + row * 4u);
            layout[5] = 38u;
            front_draw_emphasized_text_clip_top_native(text, layout, (sint32)first);
        } else if (layout[1] - 180u < 20u) {
            layout[3] = r_u32(0x800BDCC4u + row * 4u);
            scroll = 1u;
            layout[4] = r_u32(0x800BDD8Cu + row * 4u);
            last = r_u32(0x800BD404u);
            layout[5] = 38u;
            first = 200u - layout[1];
            if (row == last) {
                layout[5] = r_u32(0x800BD40Cu);
                result = front_draw_emphasized_text_clip_bottom_native(text, layout, (sint32)first);
                if ((sint32)result > 0) {
                    value = r_u32(0x800BD404u);
                    w_u32(0x800BD40Cu, 0u);
                    w_u32(0x800BD404u, value + 1u);
                }
                cursor = r_u32(0x800BE1DCu) & 2u;
            } else {
                front_draw_emphasized_text_clip_bottom_native(text, layout, (sint32)first);
            }
        } else {
            front_scan_text_emphasis(text);
        }
        if (cursor != 0u) {
            packet = r_u32(0x801132B8u);
            w_u8(packet + 3u, 3u);
            w_u8(packet + 7u, 0x60u);
            w_u16(packet + 8u, layout[0]);
            w_u16(packet + 12u, 10u);
            w_u16(packet + 14u, 20u);
            w_u8(packet + 4u, 255u);
            w_u8(packet + 5u, 255u);
            w_u8(packet + 6u, 255u);
            w_u16(packet + 10u, layout[1]);
            table = r_u32(0x801132BCu);
            AddPrim(psx_addr(table + 4u, 4u), psx_addr(packet, 16u));
            w_u32(0x801132B8u, packet + 16u);
        }
        text += 44u;
        ++row;
        layout[1] += 20u;
    }
    if (r_u32(0x800BD428u) == 1u) {
        value = r_u32(0x800BD414u);
        first = r_u32(0x800BD410u);
        value += first;
        w_u32(0x800BD414u, value);
        if ((sint32)value >= 256) {
            value -= 256u;
            first = r_u32(0x800BD40Cu);
            w_u32(0x800BD414u, value);
            w_u32(0x800BD40Cu, first + 1u);
        }
    }
    if (scroll != 0u && GetODE() != 0) {
        value = r_u32(0x800BD408u);
        w_u32(0x800BD408u, value - 2u);
    }
    w_u32(0x8001024Cu, 320u);
    w_u32(0x80010250u, 240u);
    SetGeomOffset(320, 240);
    SetGeomScreen(r_s32(0x800BC6E4u));
    captured_table = r_u32(0x801132BCu);
    table = r_u32(0x801132BCu);
    object = r_u32(0x800F6F9Cu);
    link = r_u32(table + 0x7CCu);
    value = r_u32(object + 32u);
    packet = r_u32(0x801132B8u);
    w_u32(object + 32u, value + 2u);
    object = r_u32(0x800F6F9Cu);
    value = r_u32(object + 36u);
    w_u32(object + 36u, value + 2u);
    object = r_u32(0x800F6F9Cu);
    value = r_u32(object + 40u);
    w_u32(object + 40u, value + 1u);
    object = r_u32(0x800F6F9Cu);
    w_u32(0x800421ACu, captured_table);
    w_u32(0x80040690u, packet);
    w_u32(0x8004217Cu, link);
    matrix_set_identity(object + 0x11Cu);
    header = r_u32(0x800BC6E0u);
    object = r_u32(0x800F6F9Cu);
    shift = 0u - (uint32)(sint32)r_s16(header + 4u);
    value = (uint32)(sint32)r_s16(object + 0x11Cu);
    w_u16(object + 0x11Cu, (uint32)((sint32)value >> (shift & 31u)));
    object = r_u32(0x800F6F9Cu);
    shift = 0u - (uint32)(sint32)r_s16(header + 4u);
    value = (uint32)(sint32)r_s16(object + 0x124u);
    w_u16(object + 0x124u, (uint32)((sint32)value >> (shift & 31u)));
    object = r_u32(0x800F6F9Cu);
    shift = 0u - (uint32)(sint32)r_s16(header + 4u);
    value = (uint32)(sint32)r_s16(object + 0x12Cu);
    w_u16(object + 0x12Cu, (uint32)((sint32)value >> (shift & 31u)));
    object = r_u32(0x800F6F9Cu);
    front_apply_object_rotation(object);
    object = r_u32(0x800F6F9Cu);
    value = r_u32(object + 8u);
    w_u32(object + 0x130u, value);
    object = r_u32(0x800F6F9Cu);
    header = r_u32(0x800BC6E0u);
    value = r_u32(object + 12u);
    w_u32(object + 0x134u, value);
    value = r_u16(header + 6u);
    object = r_u32(0x800F6F9Cu);
    w_u32(object + 0x138u, (uint32)((sint32)(sint16)value >> 1));
    object = r_u32(0x800F6F9Cu);
    SetRotMatrix((MATRIX *)psx_addr(object + 0x11Cu, sizeof(MATRIX)));
    object = r_u32(0x800F6F9Cu);
    SetTransMatrix((MATRIX *)psx_addr(object + 0x11Cu, sizeof(MATRIX)));
    value = r_u32(0x800BC61Cu);
    w_u32(0x80053670u, 0u);
    mesh_render_adaptive(value, 0x82008u);
    packet = r_u32(0x80040690u);
    first = r_u32(0x80031198u);
    second = r_u32(0x800311B4u);
    w_u32(0x801132B8u, packet);
    w_u32(0x8001024Cu, first);
    w_u32(0x80010250u, second);
    SetGeomOffset((sint32)first, (sint32)second);
    SetGeomScreen(r_s32(0x800313F8u));
    front_draw_viewer_controls();
    packet = r_u32(0x801132B8u);
    w_u8(packet + 3u, 3u);
    w_u8(packet + 7u, 0x60u);
    w_u16(packet + 8u, 100u);
    w_u16(packet + 10u, 370u);
    value = (uint32)(sint32)r_s16(0x800BD41Eu);
    w_u16(packet + 12u, value);
    first = r_u32(0x800BD428u);
    if (first != 0u) {
        if ((sint32)value >= 400) {
            w_u16(packet + 12u, 400u);
        } else {
            value = r_u32(0x800BD41Cu);
            first = r_u32(0x800BD418u);
            w_u32(0x800BD41Cu, value + first);
        }
    }
    w_u16(packet + 14u, 20u);
    w_u8(packet + 4u, 48u);
    value = r_u8(packet + 7u);
    w_u8(packet + 5u, 96u);
    w_u8(packet + 6u, 96u);
    w_u8(packet + 7u, value | 2u);
    table = r_u32(0x801132BCu);
    next = packet + 16u;
    AddPrim(psx_addr(table + 4u, 4u), psx_addr(packet, 16u));
    points[0] = 96;
    points[1] = 365;
    points[2] = 504;
    points[3] = 365;
    w_u32(0x801132B8u, next);
    front_draw_embossed_line(points, 0xC0C060u, 1u);
    points[1] = 395;
    points[3] = 395;
    front_draw_embossed_line(points, 0xC0C060u, 1u);
    points[0] = 95;
    points[2] = 95;
    points[1] = 365;
    front_draw_embossed_line(points, 0xC0C060u, 1u);
    points[0] = 505;
    points[2] = 505;
    points[1] = 365;
    points[3] = 395;
    front_draw_embossed_line(points, 0xC0C060u, 1u);
    value = r_u32(0x800BD420u);
    w_u32(0x800FB870u, 0x53u);
    text = r_u32(0x800F295Cu + (value << 2));
    second = 350u;
    if (r_u8(text) != 0u) {
        front_draw_colored_text(text, 95u, 350u, 0x404040u);
        second = 335u;
    }
    value = r_u32(0x800BD420u);
    text = r_u32(0x800F2958u + (value << 2));
    if (r_u8(text) != 0u) {
        front_draw_colored_text(text, 95u, second, 0x404040u);
        second -= 15u;
    }
    value = r_u32(0x800BD420u);
    text = r_u32(0x800F2954u + (value << 2));
    front_draw_colored_text(text, 95u, second, 0x404040u);
}

uint32 front_update_campaign_access_flags(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800AE208, "FRONT.OVL");
    value = r_u8(0x800104A6u);
    w_u8(0x8010D56Cu, 1u);
    w_u8(0x8010D56Du, value != 0u || r_s16(0x800595F8u) == 3);
    w_u8(0x8010D56Eu, r_u8(0x800104D0u) != 0u || r_s16(0x800595F8u) == 9);
    w_u8(0x8010D56Fu, r_u8(0x800104B4u) != 0u || r_s16(0x800595F8u) == 5);
    w_u8(0x8010D570u, r_u8(0x800104ECu) != 0u || r_s16(0x800595F8u) == 13);
    return 1u;
}

uint32 front_configure_preview_lighting(uint32 object, uint32 bright)
{
    MATRIX light;
    uint32 x, z;
    FUNCTION_MARKER(0x800AE318, "FRONT.OVL");
    light.m[0][0] = 4096;
    light.m[1][0] = 0;
    light.m[2][0] = 0;
    light.m[0][1] = 0;
    light.m[1][1] = 4096;
    light.m[2][1] = 0;
    light.m[0][2] = 0;
    light.m[1][2] = 0;
    light.m[2][2] = 4096;
    SetLightMatrix(&light);
    if (bright == 0u)
        SetBackColor(48, 48, 48);
    else
        SetBackColor(128, 128, 128);
    light_set_color(0u, 255u, 255u, 255u);
    light_set_color(1u, 0u, 0u, 0u);
    light_set_color(2u, 0u, 0u, 0u);
    x = r_u32(object + 8u);
    z = r_u32(object + 16u);
    return light_set_direction(0u, x, 0u, z);
}

void front_draw_preview_primitives(uint32 object)
{
    FUNCTION_MARKER(0x800AE3F0, "FRONT.OVL");
    object_draw_primitives(object);
}

uint32 front_create_preview_camera(void)
{
    uint32 object, value;
    FUNCTION_MARKER(0x800AE410, "FRONT.OVL");
    object_update_all();
    object_initialize_pool();
    object = front_create_model_object(1, 0u, 0u, 0u, 0u);
    w_u32(object + 0x40u, 0x8002493Cu);
    value = r_u32(0x800FBA0Cu);
    w_u32(object + 8u, 0u - value);
    value = r_u32(0x800FBA10u);
    w_u32(object + 12u, 0u - value);
    value = r_u32(0x800FBA14u);
    w_u32(0x80040FDCu, object);
    w_u32(object + 0xFCu, 0u);
    w_u32(object + 0x100u, 0u);
    w_u32(object + 0x104u, 0u);
    w_u32(object + 0x20u, 0u);
    w_u32(object + 0x24u, 0u);
    w_u32(object + 0x28u, 0u);
    w_u32(object + 16u, 0u - value);
    matrix_set_identity(object + 0x11Cu);
    front_apply_object_rotation(object);
    value = r_u32(0x80040FDCu);
    w_u32(0x80040C94u, value);
    return value;
}

void front_draw_gradient_embossed_line(const sint16 *points, uint32 first_color, uint32 second_color, uint32 depth)
{
    uint32 packet, table, stage, dx, dy, value, bucket;
    sint16 first_y, second_y;
    FUNCTION_MARKER(0x800AE824, "FRONT.OVL");
    first_y = points[1];
    second_y = points[3];
    dx = first_y == second_y ? 0u : 1u;
    dy = first_y == second_y ? 1u : 0u;
    packet = r_u32(0x801132B8u);
    bucket = depth << 2;
    for (stage = 0u; stage < 3u; ++stage) {
        w_u8(packet + 3u, 4u);
        w_u8(packet + 7u, 0x50u);
        if (stage == 2u) {
            w_u8(packet + 12u, (uint8)second_color >> 1);
            w_u8(packet + 13u, (uint8)(second_color >> 8) >> 1);
            w_u8(packet + 14u, (uint8)(second_color >> 16) >> 1);
            w_u8(packet + 4u, (uint8)first_color >> 1);
            w_u8(packet + 5u, (uint8)(first_color >> 8) >> 1);
            w_u8(packet + 6u, (uint8)(first_color >> 16) >> 1);
        } else if (stage == 1u) {
            w_u8(packet + 4u, (uint8)first_color >> 1);
            w_u8(packet + 5u, (uint8)(first_color >> 8) >> 1);
            w_u8(packet + 6u, (uint8)(first_color >> 16) >> 1);
            w_u8(packet + 12u, (uint8)second_color >> 1);
            w_u8(packet + 13u, (uint8)(second_color >> 8) >> 1);
            w_u8(packet + 14u, (uint8)(second_color >> 16) >> 1);
        } else {
            w_u8(packet + 4u, first_color);
            w_u8(packet + 5u, first_color >> 8);
            w_u8(packet + 6u, first_color >> 16);
            w_u8(packet + 12u, second_color);
            w_u8(packet + 13u, second_color >> 8);
            w_u8(packet + 14u, second_color >> 16);
        }
        if (stage == 1u) {
            w_u16(packet + 8u, (uint32)(uint16)points[0] - dx);
            w_u16(packet + 10u, (uint32)(uint16)points[1] - dy);
            w_u16(packet + 16u, (uint32)(uint16)points[2] - dx);
            value = r_u8(packet + 7u);
            second_y = points[3];
            w_u8(packet + 7u, value | 2u);
            w_u16(packet + 18u, (uint32)(uint16)second_y - dy);
        } else if (stage == 2u) {
            w_u16(packet + 8u, (uint32)(uint16)points[0] + dx);
            w_u16(packet + 10u, (uint32)(uint16)points[1] + dy);
            w_u16(packet + 16u, (uint32)(uint16)points[2] + dx);
            value = r_u8(packet + 7u);
            second_y = points[3];
            w_u8(packet + 7u, value | 2u);
            w_u16(packet + 18u, (uint32)(uint16)second_y + dy);
        } else {
            w_u16(packet + 8u, (uint16)points[0]);
            w_u16(packet + 10u, (uint16)points[1]);
            w_u16(packet + 16u, (uint16)points[2]);
            value = r_u8(packet + 7u);
            second_y = points[3];
            w_u8(packet + 7u, value | 2u);
            w_u16(packet + 18u, (uint16)second_y);
        }
        table = r_u32(0x801132BCu);
        AddPrim(psx_addr(table + bucket, 4u), psx_addr(packet, 20u));
        packet += 20u;
    }
    w_u32(0x801132B8u, packet);
}

sint32 front_find_celestial_name(const char *name)
{
    uint32 entry, index, text, length;
    FUNCTION_MARKER(0x800AEAE8, "FRONT.OVL");
    entry = 0x800BE594u;
    text = r_u32(entry);
    index = 0u;
    if (r_u8(text) == 0u)
        return -4;
    for (;;) {
        text = r_u32(entry);
        length = 0u;
        while (r_u8(text + length) != 0u)
            ++length;
        if (strcmp((const char *)psx_addr(text, length + 1u), name) == 0)
            break;
        entry += 4u;
        text = r_u32(entry);
        length = r_u8(text);
        ++index;
        if (length == 0u)
            break;
    }
    text = r_u32(0x800BE594u + (index << 2));
    if (r_u8(text) != 0u)
        return (sint32)(index - 3u);
    return -4;
}

void front_initialize_celestial_ring(uint32 ring, uint32 radius, uint32 color, uint32 scale, uint32 phase, uint32 definition)
{
    MATRIX temporary;
    uint32 value, index, angle, step, packet, vertex, screen, next_screen;
    uint32 first, second, count, table, entry, name, length, object, camera, models;
    sint32 segments, coefficient, product, depth, projection, flags, match;
    sint64 wide_product;
    const char *native_name;
    FUNCTION_MARKER(0x800AEB9C, "FRONT.OVL");
    value = radius - 400u;
    wide_product = (sint64)(sint32)value * (sint32)0xEA0EA0EBu;
    first = (uint32)((uint64)wide_product >> 32) + value;
    segments = (sint32)first >> 6;
    segments -= (sint32)value >> 31;
    segments += 10;
    if (segments >= 32)
        segments = 31;
    if (segments < 10)
        segments = 10;
    step = 4096u / (uint32)segments;
    angle = 0u;
    packet = ring;
    vertex = ring;
    for (index = 0u; index < (uint32)segments; ++index) {
        w_u8(packet + 0x207u, 3u);
        w_u8(packet + 0x20Bu, 0x40u);
        w_u8(packet + 0x208u, color);
        w_u8(packet + 0x209u, color >> 8);
        w_u8(packet + 0x217u, 3u);
        w_u8(packet + 0x21Bu, 0x40u);
        w_u8(packet + 0x20Au, color >> 16);
        w_u8(packet + 0x218u, (uint8)color >> 1);
        w_u8(packet + 0x219u, (uint8)(color >> 8) >> 1);
        w_u8(packet + 0x227u, 3u);
        w_u8(packet + 0x22Bu, 0x40u);
        w_u8(packet + 0x21Au, (uint8)(color >> 16) >> 1);
        w_u8(packet + 0x228u, (uint8)color >> 1);
        w_u8(packet + 0x229u, (uint8)(color >> 8) >> 1);
        w_u8(packet + 0x7D7u, 3u);
        w_u8(packet + 0x7DBu, 0x40u);
        w_u8(packet + 0x22Au, (uint8)(color >> 16) >> 1);
        w_u8(packet + 0x7D8u, color);
        w_u8(packet + 0x7D9u, color >> 8);
        w_u8(packet + 0x7E7u, 3u);
        w_u8(packet + 0x7DAu, color >> 16);
        w_u8(packet + 0x7EBu, 0x40u);
        w_u8(packet + 0x7E8u, (uint8)color >> 1);
        w_u8(packet + 0x7E9u, (uint8)(color >> 8) >> 1);
        w_u8(packet + 0x7F7u, 3u);
        w_u8(packet + 0x7FBu, 0x40u);
        w_u8(packet + 0x7EAu, (uint8)(color >> 16) >> 1);
        w_u8(packet + 0x7F8u, (uint8)color >> 1);
        w_u8(packet + 0x7F9u, (uint8)(color >> 8) >> 1);
        w_u8(packet + 0x7FAu, (uint8)(color >> 16) >> 1);
        coefficient = rsin((sint32)angle);
        product = (sint32)(radius * (uint32)coefficient);
        w_u16(vertex + 4u, (uint32)(product / 4096));
        coefficient = rcos((sint32)angle);
        product = (sint32)(radius * (uint32)coefficient);
        w_u16(vertex + 8u, (uint32)(product / 4096));
        w_u16(vertex + 6u, 0u);
        vertex += 8u;
        packet += 48u;
        angle += step;
    }
    vertex = ring + (uint32)segments * 8u;
    w_u16(vertex + 4u, r_u16(ring + 4u));
    w_u16(vertex + 8u, r_u16(ring + 8u));
    w_u16(vertex + 6u, r_u16(ring + 6u));
    w_u32(ring, (uint32)segments);
    w_u32(ring + 0xDB8u, 0u);
    value = scale * scale * 25u;
    w_u32(ring + 0xDA8u, value);
    if ((sint32)value >= 0x1801)
        w_u32(ring + 0xDA8u, 0x1800u);
    if (radius == 0u)
        phase = 0u;
    w_u32(ring + 0xDB0u, phase);
    w_u32(ring + 0xDACu, phase * 1000u);
    w_u32(ring + 0xDB4u, radius);
    matrix_set_identity(0x800FB9F8u);
    value = r_u16(0x8010CD70u);
    matrix_rotate_x(&temporary, (MATRIX *)psx_addr(0x800FB9F8u, 20u), value & 0xFFFu);
    value = r_u32(ring + 0xDACu);
    matrix_rotate_y((MATRIX *)psx_addr(0x800FB9F8u, 20u), &temporary,
        (uint32)((sint32)value % 4096));
    SetRotMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)));
    SetTransMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)));
    count = r_u32(ring);
    index = 0u;
    vertex = ring + 4u;
    screen = ring + 0x104u;
    while ((sint32)index < (sint32)count) {
        depth = RotTransPers((SVECTOR *)psx_addr(vertex, 8u),
            (sint32 *)psx_addr(screen, 4u), &projection, &flags);
        w_u32(ring + 0x184u + index * 4u, (uint32)depth);
        vertex += 8u;
        screen += 4u;
        count = r_u32(ring);
        ++index;
    }
    first = r_u32(ring + 0x184u);
    w_u32(ring + 0x184u + (count << 2), first);
    count = r_u32(ring);
    first = r_u16(ring + 0x104u);
    w_u16(ring + 0x104u + (count << 2), first);
    count = r_u32(ring);
    first = r_u16(ring + 0x106u);
    w_u16(ring + 0x106u + (count << 2), first);
    count = r_u32(ring);
    index = 0u;
    while ((sint32)index < (sint32)count) {
        packet = ring + index * 48u;
        screen = ring + index * 4u;
        next_screen = ring + (index + 1u) * 4u;
        first = r_u16(screen + 0x104u);
        w_u16(packet + 0x20Cu, first);
        w_u16(packet + 0x22Cu, first);
        w_u16(packet + 0x21Cu, first);
        first = r_u16(next_screen + 0x104u);
        w_u16(packet + 0x210u, first);
        w_u16(packet + 0x230u, first);
        w_u16(packet + 0x220u, first);
        first = r_u16(screen + 0x106u);
        w_u16(packet + 0x20Eu, first);
        first = r_u16(next_screen + 0x106u);
        second = r_u16(packet + 0x20Eu);
        w_u16(packet + 0x212u, first);
        first = r_u16(packet + 0x20Eu);
        w_u16(packet + 0x22Eu, second + 1u);
        second = r_u16(packet + 0x212u);
        w_u16(packet + 0x21Eu, first - 1u);
        first = r_u16(packet + 0x212u);
        w_u16(packet + 0x232u, second + 1u);
        w_u16(packet + 0x222u, first - 1u);
        first = r_u16(screen + 0x104u);
        w_u16(packet + 0x7DCu, first);
        w_u16(packet + 0x7FCu, first);
        w_u16(packet + 0x7ECu, first);
        first = r_u16(next_screen + 0x104u);
        w_u16(packet + 0x7E0u, first);
        w_u16(packet + 0x800u, first);
        w_u16(packet + 0x7F0u, first);
        first = r_u16(screen + 0x106u);
        w_u16(packet + 0x7DEu, first);
        first = r_u16(packet + 0x7DEu);
        second = r_u16(next_screen + 0x106u);
        w_u16(packet + 0x7E2u, second);
        w_u16(packet + 0x7EEu, first - 1u);
        first = r_u16(packet + 0x7E2u);
        second = r_u16(packet + 0x7E2u);
        w_u16(packet + 0x7F2u, first - 1u);
        first = r_u16(packet + 0x7DEu);
        w_u16(packet + 0x802u, second + 1u);
        w_u16(packet + 0x7FEu, first + 1u);
        count = r_u32(ring);
        ++index;
    }
    table = 0x800BE594u;
    entry = table;
    name = r_u32(table);
    index = 0u;
    if (r_u8(name) != 0u) {
        for (;;) {
            name = r_u32(entry);
            length = 0u;
            while (r_u8(name + length) != 0u)
                ++length;
            native_name = (const char *)psx_addr(name, length + 1u);
            length = 0u;
            while (r_u8(definition + 24u + length) != 0u)
                ++length;
            if (strcmp(native_name, (const char *)psx_addr(definition + 24u, length + 1u)) == 0)
                break;
            entry += 4u;
            name = r_u32(entry);
            value = r_u8(name);
            ++index;
            if (value == 0u)
                break;
        }
        name = r_u32(table + (index << 2));
        match = r_u8(name) != 0u ? (sint32)(index - 3u) : -4;
    } else {
        match = -4;
    }
    w_u32(ring + 0xDBCu, (uint32)match);
    if (match < 0) {
        value = definition + (match == -4 ? 24u : 4u);
        w_u32(ring + 0xDC0u, value);
        w_u32(ring + 0xDA4u, 0u);
        return;
    }
    w_u32(ring + 0xDC0u, definition + 4u);
    object = front_create_model_object(0, 0u, 0u, 0u, 0u);
    value = r_u32(0x800311B8u);
    w_u32(object + 0x3Cu, 0x8002D2BCu);
    w_u32(0x800BC6CCu, 0u);
    w_u32(object + 0x38u, 0x800AE3F0u);
    w_u32(0x800FB80Cu, value);
    matrix_set_identity(object + 0x11Cu);
    camera = r_u32(0x80040FDCu);
    w_u32(object + 8u, 0u);
    w_u32(object + 12u, 0u);
    w_u32(object + 16u, 0u);
    w_u32(object + 32u, 0u);
    w_u32(object + 36u, 0u);
    w_u32(object + 40u, 0u);
    product = r_s16(camera + 0x128u) * 75;
    w_u32(object + 0xFCu, (uint32)(product >> 12));
    product = r_s16(camera + 0x12Au) * 75;
    w_u32(object + 0x100u, (uint32)(product >> 12));
    product = r_s16(camera + 0x12Cu);
    w_u32(object + 0x40u, 0x8002493Cu);
    w_u32(object + 0x84u, 0xFFFFFFFCu);
    product *= 75;
    w_u32(object + 0x104u, (uint32)(product >> 12));
    value = r_u32(ring + 0xDA8u);
    w_u16(object + 0x24Cu, 360u);
    w_u32(object + 0xA8u, value);
    models = r_u32(0x80046178u);
    w_u32(object + 0xACu, 0u);
    w_u32(object + 0xB0u, 0u);
    value = r_u32(models + (uint32)match * 132u);
    w_u32(object + 0x64u, value);
    value = r_u32(object + 0x64u);
    w_u32(object + 0x68u, 0u);
    if (value == 0u)
        w_u32(object + 0x64u, r_u32(models));
    w_u32(ring + 0xDA4u, object);
}
