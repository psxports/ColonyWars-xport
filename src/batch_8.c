#include "game_functions.h"
#include <stdlib.h>
#include <string.h>

uint32 front_text_width(uint32 text)
{
    uint32 length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    return front_text_width_native((const char *)psx_addr(text, (size_t)length + 1u));
}

uint32 front_text_width_native(const char *text)
{
    uint32 character, width = 0, glyph, bank, metric;
    FUNCTION_MARKER(0x80096B04, "FRONT.OVL");
    character = (uint8)*text;
    while (character)
    {
        glyph = r_u32(0x8010D168 + character * 4);
        if ((sint32)glyph < 0)
            width += 4;
        else
        {
            bank = r_u32(0x800FB870);
            metric = 0x8004EB7E + ((bank + glyph) << 4);
            width += (uint32)r_s16(metric);
        }
        character = (uint8)*++text;
    }
    return width;
}

void front_vsync(void)
{
    uint32 timer, state, pointer, voice, product, level;
    sint32 volume, wave;
    FUNCTION_MARKER(0x8008DA54, "FRONT.OVL");
    if (r_u32(0x800F314C) != 1 || r_u32(0x80040FE8) != 12)
        return;
    if (!r_u32(0x800F3140))
    {
        pointer = r_u32(0x80041004);
        if (!r_u32(pointer) && !r_s16(0x80114264))
        {
            timer = r_u32(0x800313D0);
            state = r_u32(0x800313C8);
            ++timer;
            w_u32(0x800313D0, timer);
            if ((sint32)state > 0 && (sint32)timer >= 601)
                w_u32(0x800F3140, 1);
            else if (r_s32(0x800313D0) >= 1801)
            {
                w_u32(0x800F3140, 1);
                w_u32(0x800313C8, 1);
            }
        }
    }
    sound_tick();
    volume = r_s16(0x800118FC);
    if (volume < 0)
        volume += 3;
    level = ((uint32)volume >> 2) & 0xFF;
    voice = r_u32(0x800F3130);
    sound_set_voice_volume(voice, level);
    volume = r_s16(0x800118FC);
    if (volume < 0)
        volume += 3;
    level = ((uint32)volume >> 2) & 0xFF;
    voice = r_u32(0x800F3134);
    sound_set_voice_volume(voice, level);
    wave = rsin((sint32)(r_u32(0x800F30F4) << 5));
    product = (uint32)wave * 15;
    level = (((sint32)product >> 12) + 15) & 0xFF;
    voice = r_u32(0x800F3130);
    sound_set_voice_right(voice, level);
    wave = rcos((sint32)(r_u32(0x800F30F4) << 5));
    product = (uint32)wave * 15;
    level = (((sint32)product >> 12) + 15) & 0xFF;
    voice = r_u32(0x800F3130);
    sound_set_voice_left(voice, level);
    wave = rcos((sint32)(r_u32(0x800F30F4) << 6));
    product = (uint32)wave * 15;
    level = (((sint32)product >> 12) + 15) & 0xFF;
    voice = r_u32(0x800F3134);
    sound_set_voice_right(voice, level);
    wave = rsin((sint32)(r_u32(0x800F30F4) << 6));
    product = (uint32)wave * 15;
    level = (((sint32)product >> 12) + 15) & 0xFF;
    voice = r_u32(0x800F3134);
    sound_set_voice_left(voice, level);
}

void front_select_screen(sint32 screen)
{
    uint32 index, descriptor, type, kind, in_pane;
    sint32 signed_type;
    FUNCTION_MARKER(0x8008B85C, "FRONT.OVL");
    screen = game_frontend_route_screen(screen);
    w_u16(0x80114264, (uint16)screen);
    for (index = 0; index < 15; ++index)
        w_u8(0x800F2FB6 - index, 0);
    w_u32(0x801132A0, 0);
    screen = (sint16)screen;
    w_u16(0x800F311C, 0);
    descriptor = r_u32(0x800BB79C + (uint32)screen * 4);
    w_u32(0x800FB110, 0);
    w_u32(0x800F2FA0, 0);
    w_u16(0x80113226, 27);
    w_u8(0x80113214, 0);
    w_u8(0x80113224, 0);
    w_u8(0x8010CF40, 0);
    w_u8(0x800FB0F0, 0);
    w_u8(0x8010D160, 0);
    w_u8(0x8010D0DE, 0);
    front_load_screen_action(screen);
    for (;;)
    {
        signed_type = r_s16(descriptor);
        type = r_u16(descriptor);
        if (signed_type == 65)
            break;
        in_pane = r_u8(0x8010CF40);
        if (in_pane)
            type = r_u16(descriptor);
        kind = (uint32)(sint32)(sint16)(type - 12);
        if (!in_pane)
        {
            switch (kind)
            {
            case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 16:
                break;
            case 17:
                diagnostic_report(100, 0x80086378, 0);
                /* Fall through */
            case 18:
                diagnostic_report(100, 0x80086394, 0);
                /* Fall through */
            case 7:
                diagnostic_report(100, 0x800863AC, 0);
                /* Fall through */
            case 8:
                diagnostic_report(100, 0x800863C4, 0);
                /* Fall through */
            case 13:
                diagnostic_report(100, 0x800863E0, 0);
                /* Fall through */
            case 14:
                diagnostic_report(100, 0x800863FC, 0);
                /* Fall through */
            case 9:
                diagnostic_report(100, 0x80086418, 0);
                /* Fall through */
            case 10:
                diagnostic_report(100, 0x80086430, 0);
                /* Fall through */
            case 11:
                diagnostic_report(100, 0x8008644C, 0);
                /* Fall through */
            case 12:
                diagnostic_report(100, 0x80086464, 0);
                kind = 5;
                break;
            default:
                diagnostic_report(100, 0x80086480, 0);
                continue;
            }
        }
        else if (kind == 16)
        {
            diagnostic_report(100, 0x8008649C, 0);
            kind = 17;
        }
        switch (kind)
        {
        case 0: descriptor = front_descriptor_enter(descriptor); break;
        case 1: descriptor = front_descriptor_idle(descriptor); break;
        case 2: descriptor = front_descriptor_update(descriptor); break;
        case 3: descriptor = front_descriptor_leave(descriptor); break;
        case 4: descriptor = front_descriptor_skip(descriptor); break;
        case 5: descriptor = front_descriptor_strings(descriptor); break;
        case 6: descriptor = front_descriptor_actions(descriptor); break;
        case 7: descriptor = front_descriptor_sprite(descriptor); break;
        case 8: descriptor = front_descriptor_highlight_sprite(descriptor); break;
        case 9: descriptor = front_descriptor_string(descriptor); break;
        case 10: descriptor = front_descriptor_highlight_string(descriptor); break;
        case 11: descriptor = front_descriptor_button(descriptor); break;
        case 12: descriptor = front_descriptor_highlight_button(descriptor); break;
        case 13: descriptor = front_descriptor_rectangle(descriptor); break;
        case 14: descriptor = front_descriptor_highlight_rectangle(descriptor); break;
        case 16: descriptor = front_descriptor_begin_pane(descriptor); break;
        case 17: descriptor = front_descriptor_end_pane(descriptor); break;
        case 18: descriptor = front_descriptor_font(descriptor); break;
        default:
            diagnostic_report(100, 0x800864B0, 0);
            break;
        }
    }
    if (!r_u8(0x80113224))
        diagnostic_report(100, 0x800864CC, 0);
    if (!r_u8(0x80113214))
        diagnostic_report(100, 0x800864E8, 0);
}

uint32 front_load_screen_action(sint32 screen)
{
    uint32 action;
    FUNCTION_MARKER(0x8008D508, "FRONT.OVL");
    action = r_u16(0x800FB750 + (uint32)(sint32)(sint16)screen * 2);
    w_u16(0x8010D0DC, action);
    return action;
}

uint32 front_descriptor_end_pane(uint32 descriptor)
{
    FUNCTION_MARKER(0x8008BEB4, "FRONT.OVL");
    w_u8(0x8010CF40, 0);
    return descriptor + 2;
}

uint32 front_descriptor_font(uint32 descriptor)
{
    uint32 link, font;
    FUNCTION_MARKER(0x8008BEC4, "FRONT.OVL");
    link = heap_alloc_back(24);
    font = r_u16(descriptor + 2);
    w_u16(link + 0x16, font);
    front_append_screen_link(30, link);
    return descriptor + 4;
}

uint32 front_descriptor_strings(uint32 descriptor)
{
    uint32 language, table, index, strings;
    FUNCTION_MARKER(0x8008C58C, "FRONT.OVL");
    language = r_u32(0x8001057C);
    index = (uint32)r_s16(descriptor + 2);
    table = r_u32(0x800BB8FC + language * 4);
    strings = r_u32(table + index * 4);
    w_u8(0x80113224, 1);
    w_u32(0x800FB0F4, strings);
    return descriptor + 4;
}

uint32 front_descriptor_actions(uint32 descriptor)
{
    uint32 index, actions;
    FUNCTION_MARKER(0x8008C5D4, "FRONT.OVL");
    index = (uint32)r_s16(descriptor + 2);
    actions = r_u32(0x800BB900 + index * 4);
    w_u8(0x80113214, 1);
    w_u32(0x8010DA70, actions);
    return descriptor + 4;
}

uint32 front_descriptor_enter(uint32 descriptor)
{
    uint32 index, callback;
    FUNCTION_MARKER(0x8008C60C, "FRONT.OVL");
    index = (uint32)r_s16(descriptor + 2);
    callback = r_u32(0x800BB9B0 + index * 4);
    w_u8(0x8010D0DE, 1);
    w_u32(0x800FB114, callback);
    return descriptor + 4;
}

uint32 front_descriptor_update(uint32 descriptor)
{
    uint32 index, callback;
    FUNCTION_MARKER(0x8008C644, "FRONT.OVL");
    index = (uint32)r_s16(descriptor + 2);
    callback = r_u32(0x800BBA74 + index * 4);
    w_u8(0x8010D160, 1);
    w_u32(0x800FB234, callback);
    return descriptor + 4;
}

uint32 front_descriptor_idle(uint32 descriptor)
{
    uint32 index, callback;
    FUNCTION_MARKER(0x8008C67C, "FRONT.OVL");
    index = (uint32)r_s16(descriptor + 2);
    callback = r_u32(0x800BBA3C + index * 4);
    w_u8(0x800FB0F0, 1);
    w_u32(0x80113200, callback);
    return descriptor + 4;
}

uint32 front_descriptor_leave(uint32 descriptor)
{
    uint32 index, callback;
    FUNCTION_MARKER(0x8008C6B4, "FRONT.OVL");
    index = (uint32)r_s16(descriptor + 2);
    callback = r_u32(0x800BBAFC + index * 4);
    w_u8(0x8010D0F4, 1);
    w_u32(0x800FB118, callback);
    return descriptor + 4;
}

uint32 front_descriptor_skip(uint32 descriptor)
{
    FUNCTION_MARKER(0x8008C6EC, "FRONT.OVL");
    return descriptor + 10;
}

void front_append_screen_link(uint32 type, uint32 link)
{
    uint32 previous_type, previous, pane, kind;
    FUNCTION_MARKER(0x8008BC24, "FRONT.OVL");
    previous_type = r_u16(0x80113226);
    kind = (uint32)(sint32)(sint16)(previous_type - 19);
    w_u16(link + 0x14, type);
    switch (kind)
    {
    case 0: case 2: case 5: case 6: case 9: case 11:
        previous = r_u32(0x800F2FA0);
        w_u32(previous + 4, link);
        w_u16(previous, type);
        break;
    case 8:
        break;
    default:
        diagnostic_report(100, 0x800865A4, 0);
        break;
    }
    kind = (uint32)(sint32)(sint16)(type - 19);
    switch (kind)
    {
    case 9: case 11:
        previous = r_u32(0x800F2FA0);
        previous_type = r_u16(0x80113226);
        w_u32(link + 4, 0);
        w_u16(link, 27);
        w_u32(link + 0xC, previous);
        w_u16(link + 8, previous_type);
        break;
    case 0: case 2: case 5: case 6:
        previous = r_u32(0x800F2FA0);
        previous_type = r_u16(0x80113226);
        pane = r_u32(0x801132A0);
        w_u32(link + 4, 0);
        w_u16(link, 27);
        w_u32(link + 0xC, previous);
        w_u16(link + 8, previous_type);
        w_u32(link + 0x10, pane);
        break;
    default:
        diagnostic_report(101, 0x800865B8, 0);
        break;
    }
    w_u16(0x80113226, type);
    w_u32(0x800F2FA0, link);
}

uint32 front_descriptor_begin_pane(uint32 descriptor)
{
    uint32 pane, previous, value, y, pal, index, callback;
    FUNCTION_MARKER(0x8008BD74, "FRONT.OVL");
    w_u8(0x8010CF40, 1);
    pane = heap_alloc_back(48);
    previous = r_u32(0x800FB110);
    if (!previous)
        w_u32(0x800FB110, pane);
    value = r_u16(descriptor + 2);
    w_u16(pane + 0x18, value);
    y = r_u16(descriptor + 4);
    pal = r_u32(0x800311C0);
    w_u16(pane + 0x1A, y);
    if (pal && (sint16)y >= 401)
        w_u16(pane + 0x1A, y + 32);
    value = r_u16(descriptor + 6);
    w_u16(pane + 0x16, value);
    index = (uint32)r_s16(descriptor + 8);
    callback = r_u32(0x800BBB78 + index * 4);
    w_u32(pane + 0x1C, callback);
    index = (uint32)r_s16(descriptor + 8);
    callback = r_u32(0x800BBB8C + index * 4);
    w_u32(pane + 0x20, callback);
    index = (uint32)r_s16(descriptor + 8);
    callback = r_u32(0x800BBBA0 + index * 4);
    w_u32(pane + 0x28, 0);
    w_u16(pane + 0x2C, 2);
    w_u32(pane + 0x24, callback);
    front_append_screen_link(28, pane);
    previous = r_u32(0x801132A0);
    w_u32(pane + 0x10, 0);
    if (previous)
        w_u32(previous + 0x10, pane);
    w_u32(0x801132A0, pane);
    return descriptor + 10;
}

uint32 front_descriptor_sprite(uint32 descriptor)
{
    uint32 link, value, palette, color;
    FUNCTION_MARKER(0x8008BF08, "FRONT.OVL");
    link = heap_alloc_back(44);
    w_u16(link + 0x18, r_u16(descriptor + 2));
    w_u16(link + 0x1A, r_u16(descriptor + 4));
    value = r_u16(descriptor + 6);
    w_u16(link + 0x16, 0xFFFF);
    w_u16(link + 0x28, value);
    w_u16(link + 0x1C, r_u16(descriptor + 8));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 10) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u16(link + 0x20, color >> 16);
    w_u16(link + 0x1E, color);
    front_append_screen_link(19, link);
    return descriptor + 12;
}

uint32 front_descriptor_highlight_sprite(uint32 descriptor)
{
    uint32 link, palette, color;
    FUNCTION_MARKER(0x8008BFB0, "FRONT.OVL");
    link = heap_alloc_back(44);
    w_u16(link + 0x18, r_u16(descriptor + 4));
    w_u16(link + 0x1A, r_u16(descriptor + 6));
    w_u16(link + 0x28, r_u16(descriptor + 8));
    w_u16(link + 0x16, r_u16(descriptor + 2));
    w_u16(link + 0x1C, r_u16(descriptor + 10));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 12) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u16(link + 0x20, color >> 16);
    w_u16(link + 0x1E, color);
    w_u16(link + 0x22, r_u16(descriptor + 14));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 16) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u32(link + 0x24, color);
    w_u32(link + 0x24, color);
    front_append_screen_link(19, link);
    return descriptor + 18;
}

uint32 front_descriptor_button(uint32 descriptor)
{
    uint32 link, value, palette, color;
    FUNCTION_MARKER(0x8008C084, "FRONT.OVL");
    link = heap_alloc_back(44);
    w_u16(link + 0x18, r_u16(descriptor + 4));
    w_u16(link + 0x1A, r_u16(descriptor + 6));
    w_u16(link + 0x2A, r_u16(descriptor + 2));
    value = r_u16(descriptor + 8);
    w_u16(link + 0x16, 0xFFFF);
    w_u16(link + 0x28, value);
    w_u16(link + 0x1C, r_u16(descriptor + 10));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 12) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u16(link + 0x20, color >> 16);
    w_u16(link + 0x1E, color);
    front_append_screen_link(24, link);
    return descriptor + 14;
}

uint32 front_descriptor_highlight_button(uint32 descriptor)
{
    uint32 link, palette, color;
    FUNCTION_MARKER(0x8008C138, "FRONT.OVL");
    link = heap_alloc_back(44);
    w_u16(link + 0x18, r_u16(descriptor + 6));
    w_u16(link + 0x1A, r_u16(descriptor + 8));
    w_u16(link + 0x2A, r_u16(descriptor + 2));
    w_u16(link + 0x28, r_u16(descriptor + 10));
    w_u16(link + 0x16, r_u16(descriptor + 4));
    w_u16(link + 0x1C, r_u16(descriptor + 12));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 14) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u16(link + 0x20, color >> 16);
    w_u16(link + 0x1E, color);
    w_u16(link + 0x22, r_u16(descriptor + 16));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 18) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u32(link + 0x24, color);
    w_u32(link + 0x24, color);
    front_append_screen_link(24, link);
    return descriptor + 20;
}

uint32 front_descriptor_rectangle(uint32 descriptor)
{
    uint32 link, value, palette, color;
    FUNCTION_MARKER(0x8008C218, "FRONT.OVL");
    link = heap_alloc_back(44);
    w_u16(link + 0x18, r_u16(descriptor + 2));
    w_u16(link + 0x1A, r_u16(descriptor + 4));
    w_u16(link + 0x28, r_u16(descriptor + 8));
    value = r_u16(descriptor + 6);
    w_u16(link + 0x16, 0xFFFF);
    w_u16(link + 0x2A, value);
    w_u16(link + 0x1C, r_u16(descriptor + 10));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 12) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u16(link + 0x20, color >> 16);
    w_u16(link + 0x1E, color);
    front_append_screen_link(25, link);
    return descriptor + 14;
}

uint32 front_descriptor_highlight_rectangle(uint32 descriptor)
{
    uint32 link, palette, color;
    FUNCTION_MARKER(0x8008C2CC, "FRONT.OVL");
    link = heap_alloc_back(44);
    w_u16(link + 0x18, r_u16(descriptor + 4));
    w_u16(link + 0x1A, r_u16(descriptor + 6));
    w_u16(link + 0x28, r_u16(descriptor + 10));
    w_u16(link + 0x2A, r_u16(descriptor + 8));
    w_u16(link + 0x16, r_u16(descriptor + 2));
    w_u16(link + 0x1C, r_u16(descriptor + 12));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 14) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u16(link + 0x20, color >> 16);
    w_u16(link + 0x1E, color);
    w_u16(link + 0x22, r_u16(descriptor + 16));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 18) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u32(link + 0x24, color);
    w_u32(link + 0x24, color);
    front_append_screen_link(25, link);
    return descriptor + 20;
}

uint32 front_descriptor_string(uint32 descriptor)
{
    uint32 link, text, table, palette, color;
    sint32 identifier;
    FUNCTION_MARKER(0x8008C3AC, "FRONT.OVL");
    link = heap_alloc_back(44);
    w_u16(link + 0x18, r_u16(descriptor + 2));
    w_u16(link + 0x1A, r_u16(descriptor + 4));
    identifier = r_s16(descriptor + 6);
    if (identifier == 47)
        text = 0x800B734C;
    else
    {
        table = r_u32(0x800FB0F4);
        text = r_u32(table + (uint32)identifier * 4 - 0xC4);
    }
    w_u32(link + 0x28, text);
    w_u16(link + 0x16, 0xFFFF);
    w_u16(link + 0x1C, r_u16(descriptor + 8));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 10) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u16(link + 0x20, color >> 16);
    w_u16(link + 0x1E, color);
    front_append_screen_link(21, link);
    return descriptor + 12;
}

uint32 front_descriptor_highlight_string(uint32 descriptor)
{
    uint32 link, text, table, palette, color;
    sint32 identifier;
    FUNCTION_MARKER(0x8008C488, "FRONT.OVL");
    link = heap_alloc_back(44);
    w_u16(link + 0x18, r_u16(descriptor + 4));
    w_u16(link + 0x1A, r_u16(descriptor + 6));
    identifier = r_s16(descriptor + 8);
    if (identifier == 47)
        text = 0x800B734C;
    else
    {
        table = r_u32(0x800FB0F4);
        text = r_u32(table + (uint32)identifier * 4 - 0xC4);
    }
    w_u32(link + 0x28, text);
    w_u16(link + 0x16, r_u16(descriptor + 2));
    w_u16(link + 0x1C, r_u16(descriptor + 10));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 12) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u16(link + 0x20, color >> 16);
    w_u16(link + 0x1E, color);
    w_u16(link + 0x22, r_u16(descriptor + 14));
    palette = 0x800B7380 + (uint32)r_s16(descriptor + 16) * 4;
    (void)r_u32(palette);
    color = r_u32(palette);
    w_u32(link + 0x24, color);
    w_u32(link + 0x24, color);
    front_append_screen_link(21, link);
    return descriptor + 18;
}

void front_draw_sprite(uint32 texture, uint32 x, uint32 y, uint32 color)
{
    uint32 atlas = 0x8004EB70 + texture * 16;
    uint32 packet, shift, value, width, height, ordering_table;
    sint32 depth, clut_x, clut_y, page_x, page_y;
    uint16 clut, page;
    FUNCTION_MARKER(0x800B2458, "FRONT.OVL");
    packet = r_u32(0x801132B8);
    depth = r_s16(atlas + 12);
    w_u8(packet + 3, 4);
    w_u8(packet + 7, 0x64);
    value = r_u16(atlas);
    shift = (2u - (uint32)depth) & 31;
    w_u8(packet + 12, (value & 0x3F) << shift);
    w_u8(packet + 13, r_u16(atlas + 2));
    value = (uint32)r_s16(atlas + 8);
    w_u16(packet + 16, value << shift);
    width = r_u16(packet + 16);
    height = r_u16(atlas + 10);
    w_u16(packet + 18, height);
    height = r_u16(packet + 18);
    w_u16(packet + 8, x - (uint32)((sint16)width / 2));
    w_u16(packet + 10, y - (uint32)((sint16)height / 2));
    clut_x = r_s16(atlas + 4);
    clut_y = r_s16(atlas + 6);
    clut = GetClut(clut_x, clut_y);
    w_u16(packet + 14, clut);
    w_u8(packet + 4, color);
    w_u8(packet + 5, color >> 8);
    w_u8(packet + 6, color >> 16);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 20));
    packet += 20;
    w_u32(0x801132B8, packet);
    page_x = r_s16(atlas);
    page_y = r_s16(atlas + 2);
    page = GetTPage(depth, 1, page_x, page_y);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, page, NULL);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

void front_draw_colored_text(uint32 text, uint32 x, uint32 y, uint32 color)
{
    uint32 length = 0;
    if (r_u8(text) & 128)
    {
        while (r_u16(text + length))
            length += 2;
        front_draw_colored_text_native((const char *)psx_addr(text, (size_t)length + 2), x, y, color);
    }
    else
    {
        while (r_u8(text + length))
            ++length;
        front_draw_colored_text_native((const char *)psx_addr(text, (size_t)length + 1), x, y, color);
    }
}

void front_draw_colored_text_native(const char *text, uint32 x, uint32 y, uint32 color)
{
    uint32 character, glyph, bank, atlas, page = 0, last_page = 0;
    uint32 packet, ordering_table, shift, atlas_x, atlas_y, value, opcode;
    sint32 depth, graph_type, previous_page, clut_x, clut_y;
    uint16 clut;
    FUNCTION_MARKER(0x800976E0, "FRONT.OVL");
    character = (uint8)*text;
    if (character & 0x80)
    {
        front_draw_wide_text_native(text, x, y, color);
        return;
    }
    if (character)
    {
        character = (uint8)*text;
        do
        {
            glyph = r_u32(0x8010D168 + character * 4);
            if ((sint32)glyph < 0)
                x += 4;
            else
            {
                bank = r_u32(0x800FB870);
                atlas = 0x8004EB70 + ((bank + glyph) << 4);
                depth = r_s16(atlas + 12);
                graph_type = GetGraphType();
                shift = (2u - (uint32)depth) & 31;
                if (graph_type == 1 || GetGraphType() == 2)
                {
                    atlas_y = r_u16(atlas + 2);
                    atlas_x = r_u16(atlas);
                    page = (((uint32)depth & 3) << 9) | ((atlas_y & 0x300) >> 3) |
                           0x80 | ((atlas_x & 0x3FF) >> 6);
                }
                else
                {
                    atlas_y = r_u16(atlas + 2);
                    atlas_x = r_u16(atlas);
                    page = (((uint32)depth & 3) << 7) | ((atlas_y >> 4) & 0x10) |
                           0x20 | ((atlas_x & 0x3FF) >> 6) | ((atlas_y & 0x200) << 2);
                }
                previous_page = (sint16)last_page;
                if (!(uint16)last_page)
                {
                    last_page = page;
                    previous_page = (sint16)last_page;
                }
                if ((uint32)previous_page != page)
                {
                    packet = r_u32(0x801132B8);
                    last_page = page;
                    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)previous_page, NULL);
                    ordering_table = r_u32(0x801132BC);
                    AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 12));
                    w_u32(0x801132B8, packet + 12);
                }
                packet = r_u32(0x801132B8);
                w_u8(packet + 3, 4);
                w_u8(packet + 7, 0x64);
                w_u16(packet + 8, x);
                w_u16(packet + 10, y);
                value = r_u16(atlas);
                w_u8(packet + 12, (value & 0x3F) << shift);
                w_u8(packet + 13, r_u16(atlas + 2));
                w_u16(packet + 16, r_u16(atlas + 14));
                w_u16(packet + 18, r_u16(atlas + 10));
                clut_x = r_s16(atlas + 4);
                clut_y = r_s16(atlas + 6);
                clut = GetClut(clut_x, clut_y);
                w_u16(packet + 14, clut);
                opcode = r_u8(packet + 7);
                w_u8(packet + 4, color);
                w_u8(packet + 5, color >> 8);
                w_u8(packet + 7, opcode | 2);
                w_u8(packet + 6, color >> 16);
                ordering_table = r_u32(0x801132BC);
                AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 20));
                value = (uint32)r_s16(atlas + 14);
                w_u32(0x801132B8, packet + 20);
                x += value;
            }
            character = (uint8)*++text;
        } while (character);
    }
    packet = r_u32(0x801132B8);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)page, NULL);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

void front_draw_screen_list(uint32 list)
{
    uint32 node, type, kind;
    FUNCTION_MARKER(0x8008D2E0, "FRONT.OVL");
    node = r_u32(list + 4);
    type = r_u16(list);
    while (node)
    {
        kind = (uint32)(sint32)(sint16)(type - 19);
        switch (kind)
        {
        case 11:
            w_u32(0x800FB870, (uint32)r_s16(node + 0x16));
            break;
        case 9:
            break;
        case 0:
            front_draw_sprite_node(node);
            break;
        case 5:
            front_draw_button_node(node);
            break;
        case 6:
            front_draw_rectangle_node(node);
            break;
        case 2:
            front_draw_string_node(node);
            break;
        case 8:
            diagnostic_report(100, 0x80086628, 0);
            continue;
        default:
            diagnostic_report(102, 0x800865B8, 0);
            continue;
        }
        type = r_u16(node);
        node = r_u32(node + 4);
    }
}

void front_draw_rectangle_node(uint32 node)
{
    uint32 packet, state, color, high, low, gray = 0;
    uint32 parent, local, origin, ordering_table;
    sint32 action, selected, blend;
    FUNCTION_MARKER(0x8008CFB4, "FRONT.OVL");
    action = r_s16(node + 0x16);
    if (action > 0 && r_u8(0x800F2F89 + (uint32)action) == 2)
        return;
    packet = r_u32(0x801132B8);
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x60);
    action = r_s16(node + 0x16);
    if (action > 0)
    {
        state = r_u8(0x800F2F89 + (uint32)action);
        gray = state == 1;
    }
    if (gray)
    {
        high = r_u32(node + 0x20);
        low = r_u32(node + 0x1C);
        color = (high << 16) | (low >> 16);
        color = (color & 0xFF000000) | 0x141414;
    }
    else
    {
        action = r_s16(node + 0x16);
        selected = r_s16(0x8010D0DC);
        if (action == selected)
        {
            (void)r_u32(node + 0x24);
            color = r_u32(node + 0x24);
            blend = r_s16(node + 0x22);
        }
        else
        {
            high = r_u32(node + 0x20);
            low = r_u32(node + 0x1C);
            color = (high << 16) | (low >> 16);
            blend = r_s16(node + 0x1C);
        }
        if (blend == 1)
            w_u8(packet + 7, r_u8(packet + 7) | 2);
    }
    w_u8(packet + 4, color);
    w_u8(packet + 5, color >> 8);
    w_u8(packet + 6, color >> 16);
    parent = r_u32(node + 0x10);
    local = r_u16(node + 0x18);
    origin = r_u16(parent + 0x18);
    w_u16(packet + 8, origin + local);
    parent = r_u32(node + 0x10);
    local = r_u16(node + 0x1A);
    origin = r_u16(parent + 0x1A);
    w_u16(packet + 10, origin + local);
    w_u16(packet + 12, r_u16(node + 0x2A));
    w_u16(packet + 14, r_u16(node + 0x28));
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table + 4, 4), psx_addr(packet, 16));
    w_u32(0x801132B8, packet + 16);
}

void front_draw_sprite_node(uint32 node)
{
    uint32 atlas, packet, state, color, high, low, gray = 0;
    uint32 shift, parent, local, origin, value, ordering_table;
    sint32 action, selected, blend, depth, clut_x, clut_y, page_x, page_y;
    uint16 clut, page;
    FUNCTION_MARKER(0x8008CD3C, "FRONT.OVL");
    action = r_s16(node + 0x16);
    if (action > 0 && r_u8(0x800F2F89 + (uint32)action) == 2)
        return;
    value = (uint32)r_s16(node + 0x28);
    packet = r_u32(0x801132B8);
    atlas = 0x8004EB70 + (value << 4);
    depth = r_s16(atlas + 12);
    w_u8(packet + 3, 4);
    w_u8(packet + 7, 0x64);
    action = r_s16(node + 0x16);
    shift = (2u - (uint32)depth) & 31;
    if (action > 0)
    {
        state = r_u8(0x800F2F89 + (uint32)action);
        if (state == 1)
            gray = 1;
        else
            action = r_s16(node + 0x16);
    }
    if (gray)
    {
        high = r_u32(node + 0x20);
        low = r_u32(node + 0x1C);
        color = (high << 16) | (low >> 16);
        color = (color & 0xFF000000) | 0x141414;
    }
    else
    {
        selected = r_s16(0x8010D0DC);
        if (action == selected)
        {
            (void)r_u32(node + 0x24);
            color = r_u32(node + 0x24);
            blend = r_s16(node + 0x22);
        }
        else
        {
            high = r_u32(node + 0x20);
            low = r_u32(node + 0x1C);
            color = (high << 16) | (low >> 16);
            blend = r_s16(node + 0x1C);
        }
        if (blend == 1)
            w_u8(packet + 7, r_u8(packet + 7) | 2);
    }
    parent = r_u32(node + 0x10);
    local = r_u16(node + 0x18);
    origin = r_u16(parent + 0x18);
    w_u16(packet + 8, origin + local);
    parent = r_u32(node + 0x10);
    local = r_u16(node + 0x1A);
    origin = r_u16(parent + 0x1A);
    w_u16(packet + 10, origin + local);
    value = r_u16(atlas);
    w_u8(packet + 12, (value & 0x3F) << shift);
    w_u8(packet + 13, r_u16(atlas + 2));
    value = (uint32)r_s16(atlas + 8);
    w_u16(packet + 16, value << shift);
    w_u16(packet + 18, r_u16(atlas + 10));
    clut_x = r_s16(atlas + 4);
    clut_y = r_s16(atlas + 6);
    clut = GetClut(clut_x, clut_y);
    w_u8(packet + 6, color >> 16);
    w_u16(packet + 14, clut);
    w_u8(packet + 4, color);
    w_u8(packet + 5, color >> 8);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table + 4, 4), psx_addr(packet, 20));
    packet += 20;
    w_u32(0x801132B8, packet);
    page_x = r_s16(atlas);
    page_y = r_s16(atlas + 2);
    page = GetTPage(depth, 1, page_x, page_y);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, page, NULL);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table + 4, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

void front_draw_string_node(uint32 node)
{
    uint32 state, color, high, low, bank, text, parent;
    uint32 gray = 0, smaller = 0;
    sint32 action, selected, local_x, local_y, parent_x, parent_y;
    const uint8 *native_text;
    size_t span, ram_offset;
    FUNCTION_MARKER(0x8008D170, "FRONT.OVL");
    action = r_s16(node + 0x16);
    if (action > 0)
    {
        state = r_u8(0x800F2F89 + (uint32)action);
        if (state == 2)
            return;
        gray = state == 1;
    }
    if (gray)
    {
        high = r_u32(node + 0x20);
        low = r_u32(node + 0x1C);
        color = ((high << 16) | (low >> 16)) & 0xFF000000;
        color |= 0x141414;
    }
    else
    {
        action = r_s16(node + 0x16);
        selected = r_s16(0x8010D0DC);
        if (action == selected)
        {
            (void)r_u32(node + 0x24);
            color = r_u32(node + 0x24);
        }
        else
        {
            high = r_u32(node + 0x20);
            low = r_u32(node + 0x1C);
            color = (high << 16) | (low >> 16);
        }
    }
    bank = r_u32(0x800FB870);
    if (bank == 340)
    {
        text = r_u32(node + 0x28);
        native_text = (const uint8 *)psx_addr(text, 1);
        ram_offset = (size_t)((uintptr_t)native_text - (uintptr_t)DRAM);
        if (ram_offset >= PSX_DRAM_SIZE)
            abort();
        span = PSX_DRAM_SIZE - ram_offset;
        native_text = (const uint8 *)psx_addr(text, span);
        if (strlen((const char *)native_text) >= 16)
        {
            w_u32(0x800FB870, 170);
            smaller = 1;
        }
    }
    text = r_u32(node + 0x28);
    parent = r_u32(node + 0x10);
    local_x = r_s16(node + 0x18);
    local_y = r_s16(node + 0x1A);
    parent_x = r_s16(parent + 0x18);
    parent_y = r_s16(parent + 0x1A);
    front_draw_colored_text(text, (uint32)(parent_x + local_x), (uint32)(parent_y + local_y), color);
    if (smaller)
        w_u32(0x800FB870, bank);
}

void front_draw_button_node(uint32 node)
{
    uint32 left, right, middle, left_atlas, right_atlas, middle_atlas, atlas_base;
    uint32 color, high, low, gray = 0, flag_offset, state, shift, value, other;
    uint32 parent, origin, local, ordering_table, mode_packet, next_packet;
    sint32 action, selected, blend, second_blend, depth, clut_x, clut_y, page_x, page_y;
    uint16 clut, page;
    FUNCTION_MARKER(0x8008C6F4, "FRONT.OVL");
    action = r_s16(node + 0x16);
    if (action > 0 && r_u8(0x800F2F89 + (uint32)action) == 2)
        return;
    left = r_u32(0x801132B8);
    right = left + 20;
    middle = left + 40;
    w_u8(left + 7, 0x64);
    w_u8(right + 7, 0x64);
    w_u8(middle + 3, 9);
    w_u8(left + 3, 4);
    w_u8(right + 3, 4);
    w_u8(middle + 7, 0x2C);
    action = r_s16(node + 0x16);
    if (action > 0)
    {
        state = r_u8(0x800F2F89 + (uint32)action);
        gray = state == 1;
    }
    if (gray)
    {
        high = r_u32(node + 0x20);
        low = r_u32(node + 0x1C);
        color = (((high << 16) | (low >> 16)) & 0xFF000000) | 0x141414;
        atlas_base = 0x8004EBA0;
    }
    else
    {
        action = r_s16(node + 0x16);
        selected = r_s16(0x8010D0DC);
        if (action == selected)
        {
            (void)r_u32(node + 0x24);
            color = r_u32(node + 0x24);
            flag_offset = 0x22;
            atlas_base = 0x8004EB70;
        }
        else
        {
            high = r_u32(node + 0x20);
            low = r_u32(node + 0x1C);
            color = (high << 16) | (low >> 16);
            flag_offset = 0x1C;
            atlas_base = action == -1 ? 0x8004EB70 : 0x8004EBA0;
        }
        blend = r_s16(node + flag_offset);
        if (blend == 1)
        {
            w_u8(left + 7, r_u8(left + 7) | 2);
            second_blend = r_s16(node + flag_offset);
            if (second_blend == blend)
            {
                w_u8(right + 7, r_u8(right + 7) | 2);
                blend = r_s16(node + flag_offset);
                if (blend == second_blend)
                    w_u8(middle + 7, r_u8(middle + 7) | 2);
            }
        }
    }
    value = (uint32)r_s16(node + 0x28);
    left_atlas = atlas_base + (value << 4);
    middle_atlas = left_atlas + 16;
    right_atlas = left_atlas + 32;
    depth = r_s16(left_atlas + 12);
    parent = r_u32(node + 0x10);
    origin = r_u16(parent + 0x18);
    local = r_u16(node + 0x18);
    w_u16(left + 8, origin + local);
    parent = r_u32(node + 0x10);
    local = r_u16(node + 0x1A);
    origin = r_u16(parent + 0x1A);
    w_u16(left + 10, origin + local);
    value = r_u16(left_atlas);
    shift = (2u - (uint32)depth) & 31;
    w_u8(left + 12, (value & 0x3F) << shift);
    w_u8(left + 13, r_u16(left_atlas + 2));
    value = (uint32)r_s16(left_atlas + 8);
    w_u16(left + 16, value << shift);
    w_u16(left + 18, r_u16(left_atlas + 10));
    clut_x = r_s16(left_atlas + 4);
    clut_y = r_s16(left_atlas + 6);
    clut = GetClut(clut_x, clut_y);
    w_u16(left + 14, clut);
    value = r_u16(right_atlas);
    w_u8(right + 12, (value & 0x3F) << shift);
    w_u8(right + 13, r_u16(right_atlas + 2));
    value = (uint32)r_s16(right_atlas + 8);
    w_u16(right + 16, value << shift);
    w_u16(right + 18, r_u16(right_atlas + 10));
    value = r_u16(left + 8);
    other = r_u16(node + 0x2A);
    local = r_u16(right + 16);
    w_u16(right + 8, value + other - local);
    w_u16(right + 10, r_u16(left + 10));
    clut_x = r_s16(right_atlas + 4);
    clut_y = r_s16(right_atlas + 6);
    clut = GetClut(clut_x, clut_y);
    w_u16(right + 14, clut);
    value = r_u16(left + 8);
    other = r_u16(left + 16);
    w_u16(middle + 8, value + other);
    w_u16(middle + 10, r_u16(left + 10));
    w_u16(middle + 16, r_u16(right + 8));
    value = r_u16(middle + 8);
    other = r_u16(right + 10);
    w_u16(middle + 24, value);
    value = r_u16(middle + 10);
    w_u16(middle + 18, other);
    other = r_u16(middle_atlas + 10);
    w_u16(middle + 26, value + other);
    value = r_u16(middle + 16);
    other = r_u16(middle + 26);
    w_u16(middle + 32, value);
    w_u16(middle + 34, other);
    value = r_u16(middle_atlas);
    w_u8(middle + 12, (value & 0x3F) << shift);
    value = r_u8(middle + 12);
    other = r_u16(middle_atlas + 2);
    ++value;
    w_u8(middle + 20, value);
    w_u8(middle + 13, other);
    if (!(value & 0xFF))
        w_u8(middle + 20, 0xFF);
    value = r_u8(middle + 13);
    other = r_u8(middle + 12);
    w_u8(middle + 21, value);
    value = r_u8(middle + 13);
    w_u8(middle + 28, other);
    other = r_u8(middle_atlas + 10);
    w_u8(middle + 29, value + other);
    value = r_u8(middle + 20);
    other = r_u8(middle + 29);
    w_u8(middle + 36, value);
    w_u8(middle + 37, other);
    clut_x = r_s16(middle_atlas + 4);
    clut_y = r_s16(middle_atlas + 6);
    clut = GetClut(clut_x, clut_y);
    w_u16(middle + 14, clut);
    page_x = r_s16(middle_atlas);
    page_y = r_s16(middle_atlas + 2);
    page = GetTPage(depth, 1, page_x, page_y);
    w_u16(middle + 22, page);
    w_u8(left + 4, color);
    w_u8(left + 6, color >> 16);
    w_u8(left + 5, color >> 8);
    w_u8(right + 4, color);
    w_u8(right + 6, color >> 16);
    w_u8(right + 5, color >> 8);
    w_u8(middle + 4, color);
    w_u8(middle + 5, color >> 8);
    w_u8(middle + 6, color >> 16);
    ordering_table = r_u32(0x801132BC);
    w_u32(0x801132B8, middle + 40);
    AddPrim(psx_addr(ordering_table + 4, 4), psx_addr(left, 20));
    page_x = r_s16(left_atlas);
    page_y = r_s16(left_atlas + 2);
    mode_packet = r_u32(0x801132B8);
    page = GetTPage(depth, 1, page_x, page_y);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12), 0, 1, page, NULL);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table + 4, 4), psx_addr(mode_packet, 12));
    next_packet = mode_packet + 12;
    ordering_table = r_u32(0x801132BC);
    w_u32(0x801132B8, next_packet);
    AddPrim(psx_addr(ordering_table + 4, 4), psx_addr(right, 20));
    page_x = r_s16(right_atlas);
    page_y = r_s16(right_atlas + 2);
    mode_packet = r_u32(0x801132B8);
    page = GetTPage(depth, 1, page_x, page_y);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12), 0, 1, page, NULL);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table + 4, 4), psx_addr(mode_packet, 12));
    next_packet = mode_packet + 12;
    ordering_table = r_u32(0x801132BC);
    w_u32(0x801132B8, next_packet);
    AddPrim(psx_addr(ordering_table + 4, 4), psx_addr(middle, 40));
}

sint32 front_wide_character_glyph(uint32 character)
{
    uint32 lead = character & 0xFF;
    uint32 trail = (character >> 8) & 0xFF;
    uint32 table, index, value;
    sint32 present;
    FUNCTION_MARKER(0x80096990, "FRONT.OVL");
    if (lead == 0x82)
    {
        if (trail >= 0x60 && trail < 0x7A)
            return trail - 0x60;
        if (trail >= 0x4F && trail < 0x59)
            return trail - 0x1B;
        if (trail >= 0x81 && trail < 0x9B)
            return trail - 0x67;
    }
    else if (lead == 0x81)
    {
        if (trail == 0x40)
            return -1;
    }
    else
        return -2;
    table = 0x800BC794;
    present = r_s16(table);
    value = r_u16(table);
    if (!present)
        return -2;
    index = 0;
    while ((sint16)character != (sint16)value)
    {
        table += 2;
        present = r_s16(table);
        value = r_u16(table);
        ++index;
        if (!present)
            break;
    }
    present = r_s16(table);
    index += 62;
    return present ? (sint16)index : -2;
}

uint32 front_swap_render_buffers(void)
{
    uint32 count, active, inactive, next_count, packets;
    FUNCTION_MARKER(0x8009B9EC, "FRONT.OVL");
    count = r_u32(0x801132A8);
    active = r_u32(0x801132BC);
    DrawOTag((uint32 *)psx_addr(active + (count << 2), 4));
    inactive = r_u32(0x801132C0);
    active = r_u32(0x801132BC);
    next_count = r_u32(0x801132AC);
    w_u32(0x801132BC, inactive);
    inactive = r_u32(0x801132BC);
    w_u32(0x801132B4, 0);
    w_u32(0x801132A8, next_count);
    w_u32(0x801132C0, active);
    packets = r_u32(inactive + 0x7CC);
    ++next_count;
    w_u32(0x801132B8, packets);
    ClearOTagR((uint32 *)psx_addr(inactive, (sint32)next_count > 0 ? next_count * 4 : 4), (sint32)next_count);
    return r_u32(0x801132BC);
}

void front_set_ordering_table_size(uint32 entries)
{
    FUNCTION_MARKER(0x8009BA60, "FRONT.OVL");
    w_u32(0x801132AC, entries - 1);
}

sint32 front_allocate_render_buffer(uint32 buffer, uint32 capacity)
{
    uint32 packets;
    FUNCTION_MARKER(0x8009BA74, "FRONT.OVL");
    packets = heap_alloc_back((((capacity << 1) + capacity) * 4 + capacity) * 4);
    w_u32(buffer + 0x7CC, packets);
    return packets != 0;
}

void front_initialize_render_buffers(uint32 entries, uint32 capacity)
{
    uint32 count, buffer;
    FUNCTION_MARKER(0x8009BAB8, "FRONT.OVL");
    w_u32(0x801132B0, capacity);
    w_u32(0x801132A8, entries - 1);
    w_u32(0x801132AC, entries - 1);
    if (!front_allocate_render_buffer(0x801132C4, capacity))
        diagnostic_report(501, 0x80086AD0, 0);
    if (!front_allocate_render_buffer(0x80113A94, capacity))
        diagnostic_report(501, 0x80086AD0, 0);
    count = r_u32(0x801132A8);
    w_u32(0x801132BC, 0x801132C4);
    w_u32(0x801132C0, 0x80113A94);
    ++count;
    ClearOTagR((uint32 *)psx_addr(0x801132C4, (sint32)count > 0 ? count * 4 : 4), (sint32)count);
    count = r_u32(0x801132A8);
    buffer = r_u32(0x801132C0);
    ++count;
    ClearOTagR((uint32 *)psx_addr(buffer, (sint32)count > 0 ? count * 4 : 4), (sint32)count);
    front_swap_render_buffers();
    front_swap_render_buffers();
}

void front_free_render_buffer(uint32 buffer)
{
    FUNCTION_MARKER(0x8009BBA4, "FRONT.OVL");
    heap_free(r_u32(buffer + 0x7CC));
    w_u32(buffer + 0x7CC, 0);
}

void front_close_render_buffers(void)
{
    FUNCTION_MARKER(0x8009BBD8, "FRONT.OVL");
    w_u32(0x801132B0, 0);
    w_u32(0x801132A8, 0);
    front_free_render_buffer(0x801132C4);
    front_free_render_buffer(0x80113A94);
}

void front_close_background(void)
{
    FUNCTION_MARKER(0x8009A844, "FRONT.OVL");
    heap_free(r_u32(0x800FB85C));
    heap_free(r_u32(0x800FB860));
    heap_free(r_u32(0x800FB864));
    heap_free(r_u32(0x800FB868));
}

void front_draw_surface_sprite(uint32 sprite)
{
    uint32 packet, value, origin, ordering_table;
    FUNCTION_MARKER(0x8009B884, "FRONT.OVL");
    if (r_s32(sprite + 12) <= 0)
        return;
    packet = r_u32(0x801132B8);
    SetSprt((SPRT *)psx_addr(packet, 20));
    value = r_u32(sprite + 4);
    w_u16(packet + 8, value);
    value = r_u32(sprite + 8);
    w_u16(packet + 10, value);
    value = r_u32(sprite + 12);
    origin = r_u32(0x8010DB78);
    w_u8(packet + 12, value - origin);
    value = r_u32(sprite + 16);
    origin = r_u32(0x8010DB7C);
    w_u8(packet + 13, value - origin);
    value = r_u32(sprite + 20);
    w_u16(packet + 16, value);
    value = r_u32(sprite + 24);
    w_u16(packet + 18, value);
    value = r_u32(sprite + 28);
    w_u8(packet + 4, value);
    w_u8(packet + 5, value >> 8);
    w_u8(packet + 6, value >> 16);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 20));
    w_u32(0x801132B8, packet + 20);
}

void front_set_surface_texture_page(void)
{
    uint32 x, y, packet, ordering_table;
    uint16 page;
    FUNCTION_MARKER(0x8009B96C, "FRONT.OVL");
    x = r_u32(0x8010DB78);
    y = r_u32(0x8010DB7C);
    packet = r_u32(0x801132B8);
    page = GetTPage(2, 1, (sint32)x, (sint32)y);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 0, page, NULL);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

uint32 front_screen_bounds(uint32 screen, PSX_RECT *bounds)
{
    uint32 node, width, height;
    sint32 value;
    FUNCTION_MARKER(0x8009BC1C, "FRONT.OVL");
    bounds->x = 10000;
    bounds->y = 10000;
    bounds->w = -10000;
    bounds->h = -10000;
    node = r_u32(screen + 4);
    while (node)
    {
        if (r_s16(node + 20) == 28)
            break;
        if (r_s16(node + 22) != -1)
        {
            value = r_s16(node + 24);
            if (value < bounds->x)
                bounds->x = (sint16)value;
            value = r_s16(node + 26);
            if (value < bounds->y)
                bounds->y = (sint16)value;
            value = r_s16(node + 24);
            if (bounds->w < value)
                bounds->w = (sint16)value;
            value = r_s16(node + 26);
            if (bounds->h < value)
                bounds->h = (sint16)value;
        }
        node = r_u32(node + 4);
    }
    width = (uint32)(uint16)bounds->w - (uint32)(uint16)bounds->x;
    height = (uint32)(uint16)bounds->h - (uint32)(uint16)bounds->y;
    bounds->w = (sint16)width;
    bounds->h = (sint16)height;
    return width;
}

uint32 front_allocate_screen_state(uint32 screen)
{
    uint32 state, offset, value;
    PSX_RECT bounds;
    FUNCTION_MARKER(0x8009BD1C, "FRONT.OVL");
    state = heap_alloc_back(0x24C);
    w_u32(screen + 40, state);
    if (!state)
        return 0;
    offset = 0x248;
    for (;;)
    {
        w_u16(state + offset, 0);
        if (offset == 0x50)
            break;
        offset -= 0x24;
    }
    front_screen_bounds(screen, &bounds);
    value = (uint32)r_s16(screen + 24) << 16;
    w_u32(state, value);
    value = (uint32)r_s16(screen + 26) << 16;
    w_u32(state, value);
    value = (uint32)(sint32)bounds.x << 16;
    w_u32(state + 8, value);
    value = (uint32)(sint32)bounds.y << 16;
    w_u32(state + 12, value);
    return value;
}

uint32 front_initialize_screen_slide(uint32 screen)
{
    uint32 state, value, start, end, difference, high, panel;
    sint64 product;
    FUNCTION_MARKER(0x8009BDC8, "FRONT.OVL");
    w_u32(0x800F3138, screen);
    state = heap_alloc_back(40);
    w_u32(screen + 40, state);
    if (state)
    {
        value = (uint32)r_s16(screen + 24) << 16;
        w_u32(state + 8, value);
        value = (uint32)r_s16(screen + 26);
        start = r_u32(state + 8);
        w_u32(state, 0x02A80000);
        end = r_u32(state);
        difference = start - end;
        value <<= 16;
        product = (sint64)(sint32)difference * (sint64)(sint32)0x88888889;
        w_u32(state + 12, value);
        value = r_u32(state + 12);
        w_u32(state + 4, value);
        value = r_u32(state);
        end = r_u32(state + 4);
        w_u32(state + 36, 0);
        w_u32(state + 24, value);
        w_u32(state + 28, end);
        value = (uint32)r_s16(state + 26);
        high = (uint32)((uint64)product >> 32);
        high += difference;
        high = (uint32)((sint32)high >> 3);
        high -= (uint32)((sint32)difference >> 31);
        w_u32(state + 32, high);
        w_u16(screen + 24, value);
        value = (uint32)r_s16(state + 30);
        w_u16(screen + 26, value);
    }
    value = 1;
    w_u16(screen + 44, value);
    panel = r_u32(0x800F312C);
    if (panel)
    {
        value = (uint32)r_s16(screen + 24) + 4;
        w_u32(panel, value);
        value = (uint32)r_s16(screen + 26);
        panel = r_u32(0x800F312C);
        value += 2;
        w_u32(panel + 4, value);
    }
    return value;
}

void front_push_text_line(uint32 panel, uint32 text)
{
    uint32 next, count, numerator, visible, slot, lines, destination;
    size_t offset, span, length;
    const char *native_text;
    FUNCTION_MARKER(0x8009822C, "FRONT.OVL");
    if (!panel)
        return;
    numerator = r_u32(panel + 40) + 1;
    count = r_u32(panel + 28);
    if (!count || (count == 0xFFFFFFFF && numerator == 0x80000000))
        abort();
    next = (uint32)((sint32)numerator % (sint32)count);
    visible = r_u32(panel + 24);
    count = r_u32(panel + 28);
    numerator = visible + next;
    if (!count || (count == 0xFFFFFFFF && numerator == 0x80000000))
        abort();
    slot = (uint32)((sint32)numerator % (sint32)count);
    w_u32(panel + 40, next);
    native_text = (const char *)psx_addr(text, 1);
    offset = (size_t)((uintptr_t)native_text - (uintptr_t)DRAM);
    if (offset >= PSX_DRAM_SIZE)
        abort();
    span = PSX_DRAM_SIZE - offset;
    native_text = (const char *)psx_addr(text, span);
    length = strlen(native_text);
    if (length >= 51)
    {
        w_u8(text + 49, 0);
        length = 49;
    }
    lines = r_u32(panel + 44);
    destination = r_u32(lines + (slot << 2));
    strcpy((char *)psx_addr(destination, length + 1), native_text);
}

void front_update_screen_slide(uint32 screen)
{
    uint32 state, axis, current, target, step, difference, distance, speed;
    uint32 offset, component, value, amount, limit, panel, strings, text;
    uint32 red, green, blue;
    sint32 previous, phase;
    FUNCTION_MARKER(0x8009BECC, "FRONT.OVL");
    state = r_u32(screen + 40);
    if (!state)
        w_u16(screen + 44, r_s16(0x800B737C) == 1 ? 2 : 0);
    else if (r_s16(0x800B737C) == 1)
    {
        w_u16(screen + 44, 2);
        current = r_u32(state + 24);
        target = r_u32(state + 8);
        if (current == target)
            sound_play_sample(7);
        for (axis = 0; axis < 8; axis += 4)
        {
            current = r_u32(state + 24 + axis);
            target = r_u32(state + axis);
            step = r_u32(state + 32 + axis);
            difference = current - target;
            distance = (sint32)difference < 0 ? 0 - difference : difference;
            speed = (sint32)step < 0 ? 0 - step : step;
            if ((sint32)speed < (sint32)distance)
            {
                w_u32(state + 24 + axis, current - step);
                w_u16(screen + 44, 1);
            }
            else
                w_u32(state + 24 + axis, target);
        }
        if (r_s16(screen + 44) == 1)
        {
            for (offset = 0; offset < 48; offset += 16)
            {
                if (r_s16(0x800FB828 + offset) == -1)
                    continue;
                for (component = 0; component < 3; ++component)
                {
                    value = r_u8(0x800FB830 + offset + component);
                    amount = r_u8(0x800FB82D + offset + component);
                    difference = value - amount;
                    w_u8(0x800FB830 + offset + component, (sint32)difference < 0 ? 0 : difference);
                }
            }
        }
        value = (uint32)r_s16(state + 26);
        w_u16(screen + 24, value);
        value = (uint32)r_s16(state + 30);
        w_u16(screen + 26, value);
    }
    else
    {
        previous = r_u16(screen + 44);
        w_u16(screen + 44, 0);
        current = r_u32(state + 24);
        target = r_u32(state);
        if (current == target)
            sound_play_sample(7);
        for (axis = 0; axis < 8; axis += 4)
        {
            current = r_u32(state + 24 + axis);
            target = r_u32(state + 8 + axis);
            step = r_u32(state + 32 + axis);
            difference = current - target;
            distance = (sint32)difference < 0 ? 0 - difference : difference;
            speed = (sint32)step < 0 ? 0 - step : step;
            if ((sint32)speed < (sint32)distance)
            {
                w_u32(state + 24 + axis, current + step);
                w_u16(screen + 44, 1);
            }
            else
                w_u32(state + 24 + axis, target);
        }
        if (r_s16(screen + 44) == 1)
        {
            for (offset = 0; offset < 48; offset += 16)
            {
                if (r_s16(0x800FB828 + offset) == -1)
                    continue;
                for (component = 0; component < 3; ++component)
                {
                    value = r_u8(0x800FB830 + offset + component);
                    amount = r_u8(0x800FB82D + offset + component);
                    limit = r_u8(0x800FB82A + offset + component);
                    value += amount;
                    w_u8(0x800FB830 + offset + component, limit < value ? limit : value);
                }
            }
        }
        value = (uint32)r_s16(state + 26);
        phase = r_s16(screen + 44);
        w_u16(screen + 24, value);
        value = (uint32)r_s16(state + 30);
        w_u16(screen + 26, value);
        if ((sint16)previous != phase)
        {
            for (offset = 0; offset < 48; offset += 16)
            {
                if (r_s16(0x800FB828 + offset) == -1)
                    continue;
                red = r_u8(0x800FB82A + offset);
                green = r_u8(0x800FB82B + offset);
                blue = r_u8(0x800FB82C + offset);
                w_u8(0x800FB830 + offset, red);
                w_u8(0x800FB831 + offset, green);
                w_u8(0x800FB832 + offset, blue);
            }
            if (r_s16(0x800F312A) == 1)
            {
                strings = r_u32(0x800FB0F4);
                panel = r_u32(0x800F312C);
                text = r_u32(strings);
                front_push_text_line(panel, text);
            }
        }
    }
    panel = r_u32(0x800F312C);
    if (panel)
    {
        value = (uint32)r_s16(screen + 24);
        panel = r_u32(0x800F312C);
        w_u32(panel, value + 4);
        value = (uint32)r_s16(screen + 26);
        panel = r_u32(0x800F312C);
        w_u32(panel + 4, value + 2);
    }
    if (r_s16(screen + 44) == 2)
    {
        for (offset = 0; offset < 48; offset += 16)
        {
            if (r_s16(0x800FB828 + offset) == -1)
                continue;
            w_u8(0x800FB832 + offset, 0);
            w_u8(0x800FB831 + offset, 0);
            w_u8(0x800FB830 + offset, 0);
        }
    }
}

void front_free_screen_slide(uint32 screen)
{
    FUNCTION_MARKER(0x8009C518, "FRONT.OVL");
    heap_free(r_u32(screen + 40));
    w_u32(screen + 40, 0);
    w_u32(0x800F3138, 0);
}

uint32 front_initialize_string_fade(uint32 screen)
{
    uint32 state, node, count, table, saved, color, offset, component, value;
    sint32 type;
    FUNCTION_MARKER(0x8009C554, "FRONT.OVL");
    state = heap_alloc_back(16);
    w_u32(screen + 40, state);
    if (state)
    {
        w_u32(state + 4, r_s16(0x800F312A) == 1 ? 0 : 15);
        w_u32(state, 0);
        node = r_u32(screen + 4);
        while (node)
        {
            type = r_s16(node + 20);
            if (type == 28)
                break;
            if (type == 21)
            {
                count = r_u32(state);
                w_u32(state, count + 1);
            }
            node = r_u32(node + 4);
        }
        count = r_u32(state);
        table = heap_alloc_back(count << 2);
        count = r_u32(state);
        w_u32(state + 8, table);
        saved = heap_alloc_back(count << 2);
        w_u32(state + 12, saved);
        node = r_u32(screen + 4);
        offset = 0;
        while (node)
        {
            type = r_s16(node + 20);
            if (type == 28)
                break;
            if (type == 21)
            {
                table = r_u32(state + 8);
                w_u32(table + offset, node + 30);
                for (component = 0; component < 3; ++component)
                {
                    table = r_u32(state + 8);
                    color = r_u32(table + offset);
                    saved = r_u32(state + 12);
                    value = r_u8(color + component);
                    w_u8(saved + offset + component, value);
                }
                if (r_s16(0x800F312A) == 1)
                {
                    for (component = 0; component < 3; ++component)
                    {
                        table = r_u32(state + 8);
                        color = r_u32(table + offset);
                        w_u8(color + component, 0);
                    }
                }
                else
                {
                    for (component = 0; component < 3; ++component)
                    {
                        table = r_u32(state + 8);
                        color = r_u32(table + offset);
                        value = r_u8(color + component);
                        w_u8(color + component, value);
                    }
                }
                offset += 4;
            }
            node = r_u32(node + 4);
        }
    }
    w_u16(screen + 44, 1);
    return 1;
}

void front_update_string_fade(uint32 screen)
{
    uint32 state, factor, index, component, saved, value, low, high, table, color;
    sint64 product;
    FUNCTION_MARKER(0x8009C7A0, "FRONT.OVL");
    state = r_u32(screen + 40);
    if (!state)
    {
        w_u16(screen + 44, r_s16(0x800B737C) == 1 ? 2 : 0);
        return;
    }
    if (r_s16(0x800B737C) == 1 && r_s16(0x800F312A) == 1)
    {
        factor = r_u32(state + 4);
        if ((sint32)factor > 0)
        {
            w_u32(state + 4, factor - 1);
            w_u16(screen + 44, 1);
        }
        else
            w_u16(screen + 44, 2);
    }
    else
    {
        factor = r_u32(state + 4);
        if ((sint32)factor < 15)
        {
            w_u32(state + 4, factor + 1);
            w_u16(screen + 44, 1);
        }
        else
            w_u16(screen + 44, r_s16(0x800B737C) == 1 ? 2 : 0);
    }
    if (r_s32(state) <= 0)
        return;
    index = 0;
    do
    {
        for (component = 0; component < 3; ++component)
        {
            saved = r_u32(state + 12);
            factor = r_u32(state + 4);
            value = r_u8(saved + (index << 2) + component);
            low = factor * value;
            product = (sint64)(sint32)low * (sint64)(sint32)0x88888889;
            table = r_u32(state + 8);
            color = r_u32(table + (index << 2));
            high = (uint32)((uint64)product >> 32);
            high += low;
            high = (uint32)((sint32)high >> 3);
            high -= (uint32)((sint32)low >> 31);
            w_u8(color + component, high);
        }
        ++index;
    } while ((sint32)index < r_s32(state));
}

void front_free_string_fade(uint32 screen)
{
    uint32 state;
    FUNCTION_MARKER(0x8009C968, "FRONT.OVL");
    state = r_u32(screen + 40);
    if (state)
    {
        heap_free(r_u32(state + 8));
        heap_free(r_u32(state + 12));
        heap_free(r_u32(screen + 40));
    }
    w_u32(screen + 40, 0);
}

void front_prepare_card_icons(void)
{
    TEX_INFO info;
    uint32 image, source, pixels, destination, block, count, index, address, old;
    uint32 words[4];
    FUNCTION_MARKER(0x8009C9CC, "FRONT.OVL");
    w_u8(0x800F2DC0, 3);
    for (image = 0; image < 3; ++image)
    {
        texture_read_info(0x800BEC60 + image * 0xC4, &info);
        if (!image)
        {
            source = info.clut_data;
            pixels = info.pixel_data;
            destination = 0x800F2DC2;
            for (block = 0; block < 8; block += 3)
            {
                count = 8 - block;
                if (count > 3)
                    count = 3;
                for (index = 0; index < count; ++index)
                {
                    (void)r_u32(source + (block + index) * 4);
                    words[index] = r_u32(source + (block + index) * 4);
                }
                for (index = 0; index < count; ++index)
                {
                    address = destination + (block + index) * 4;
                    old = r_u32(address + 2);
                    w_u32(address + 2, (old & 0xFFFF0000) | (words[index] >> 16));
                    old = r_u32(address - 2);
                    w_u32(address - 2, (old & 0xFFFF) | (words[index] << 16));
                }
            }
            source = pixels;
        }
        else
            source = info.pixel_data;
        destination = 0x800F2DE2 + image * 128;
        for (block = 0; block < 128; block += 16)
        {
            for (index = 0; index < 4; ++index)
            {
                (void)r_u32(source + block + index * 4);
                words[index] = r_u32(source + block + index * 4);
            }
            for (index = 0; index < 4; ++index)
            {
                address = destination + block + index * 4;
                old = r_u32(address + 2);
                w_u32(address + 2, (old & 0xFFFF0000) | (words[index] >> 16));
                old = r_u32(address - 2);
                w_u32(address - 2, (old & 0xFFFF) | (words[index] << 16));
            }
        }
    }
}

void front_set_card_texture_layout(uint32 x, uint32 y, uint32 clut_x, uint32 clut_y)
{
    FUNCTION_MARKER(0x8009CCD8, "FRONT.OVL");
    w_u32(0x800F2F68, x);
    w_u32(0x800F2F6C, y);
    w_u32(0x800F2F70, clut_x);
    w_u32(0x800F2F74, clut_y);
}

void front_upload_card_icon(uint32 port, uint32 slot)
{
    PSX_RECT rectangle;
    uint32 x, y, source, frame, palette_y, palette_x;
    sint32 frames;
    FUNCTION_MARKER(0x8009CD00, "FRONT.OVL");
    if (!r_u32(0x800F2F64))
        return;
    rectangle.w = 4;
    rectangle.h = 16;
    x = r_u32(0x800F2F68);
    rectangle.x = (sint16)(x + (((port << 1) + port) << 2));
    y = r_u32(0x800F2F6C);
    rectangle.y = (sint16)(y + (slot << 4));
    source = (port << 13) + (slot << 9);
    frames = (sint32)r_u8(0x800F7092 + source) - 16;
    if (frames >= 4)
        frames = 3;
    if (frames > 0)
    {
        source = 0x800F7090 + (slot << 9) + (port << 13);
        for (frame = 0; frame < (uint32)frames; ++frame)
        {
            DrawSync(0);
            LoadImagePSX(&rectangle, (uint32 *)psx_addr(source + 128 + frame * 128, 128));
            rectangle.x = (sint16)((uint16)rectangle.x + 4);
        }
    }
    rectangle.w = 16;
    rectangle.h = 1;
    palette_y = r_u32(0x800F2F74);
    palette_x = r_u32(0x800F2F70);
    rectangle.x = (sint16)palette_x;
    rectangle.y = (sint16)(palette_y + slot + (port << 4) - port);
    DrawSync(0);
    source = 0x800F7090 + (slot << 9) + (port << 13) + 96;
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(source, 32));
}

void front_clear_menu_transition(void)
{
    FUNCTION_MARKER(0x800A5708, "FRONT.OVL");
    w_u16(0x800F311A, 0);
}

void front_update_screen(void)
{
    uint32 pointer, timer, cycle, level, voice, product;
    sint32 volume, wave;
    FUNCTION_MARKER(0x8008D864, "FRONT.OVL");
    if (!r_u32(0x800F3140))
    {
        pointer = r_u32(0x80041004);
        if (!r_u32(pointer) && !r_s16(0x80114264))
        {
            timer = r_u32(0x800313D0);
            cycle = r_u32(0x800313C8);
            ++timer;
            w_u32(0x800313D0, timer);
            if ((sint32)cycle > 0 && (sint32)timer >= 601)
            {
                w_u32(0x800F3140, 1);
                front_clear_menu_transition();
            }
            else if (r_s32(0x800313D0) >= 1801)
            {
                w_u32(0x800F3140, 1);
                front_clear_menu_transition();
                w_u32(0x800313C8, 1);
            }
        }
    }
    stream_update();
    sound_tick();
    volume = r_s16(0x800118FC);
    if (volume < 0)
        volume += 3;
    level = ((uint32)volume >> 2) & 0xFF;
    voice = r_u32(0x800F3130);
    sound_set_voice_volume(voice, level);
    volume = r_s16(0x800118FC);
    if (volume < 0)
        volume += 3;
    level = ((uint32)volume >> 2) & 0xFF;
    voice = r_u32(0x800F3134);
    sound_set_voice_volume(voice, level);
    wave = rsin((sint32)(r_u32(0x800F30F4) << 5));
    product = (uint32)wave * 15;
    level = (((sint32)product >> 12) + 15) & 0xFF;
    voice = r_u32(0x800F3130);
    sound_set_voice_right(voice, level);
    wave = rcos((sint32)(r_u32(0x800F30F4) << 5));
    product = (uint32)wave * 15;
    level = (((sint32)product >> 12) + 15) & 0xFF;
    voice = r_u32(0x800F3130);
    sound_set_voice_left(voice, level);
    wave = rcos((sint32)(r_u32(0x800F30F4) << 6));
    product = (uint32)wave * 15;
    level = (((sint32)product >> 12) + 15) & 0xFF;
    voice = r_u32(0x800F3134);
    sound_set_voice_right(voice, level);
    wave = rsin((sint32)(r_u32(0x800F30F4) << 6));
    product = (uint32)wave * 15;
    level = (((sint32)product >> 12) + 15) & 0xFF;
    voice = r_u32(0x800F3134);
    sound_set_voice_left(voice, level);
}

void front_close_display(void)
{
    uint32 count, table;
    FUNCTION_MARKER(0x8008E480, "FRONT.OVL");
    VSync(0);
    ResetGraph(1);
    PutDrawEnv((DRAWENV *)psx_addr(0x800FB878, 92));
    count = r_u32(0x801132A8);
    table = r_u32(0x801132BC);
    DrawOTag((uint32 *)psx_addr(table + (count << 2), 4));
    front_update_screen();
    DrawSync(0);
    VSync(0);
    front_swap_render_buffers();
    front_update_screen();
    DrawSync(0);
}

void front_initialize_display(void)
{
    uint32 pal, x, y, source, logo, size;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x8008E27C, "FRONT.OVL");
    SetDispMask(0);
    SetDefDrawEnv((DRAWENV *)psx_addr(0x800FB878, 92), 0, 0, 640, 512);
    pal = r_u32(0x800311C0);
    if (pal)
    {
        SetDefDispEnv((DISPENV *)psx_addr(0x800FB8D8, 20), 0, 0, 640, 512);
        w_u16(0x800FB8E6, 256);
    }
    else
        SetDefDispEnv((DISPENV *)psx_addr(0x800FB8D8, 20), 0, 0, 640, 480);
    w_u8(0x800FB8E8, 1);
    w_u8(0x800FB890, 1);
    w_u8(0x800FB88F, 1);
    pal = r_u32(0x800311C0);
    x = r_u32(0x80010564);
    y = r_u32(0x80010568);
    w_u8(0x800FB88E, 0);
    w_u8(0x800FB891, 0);
    w_u8(0x800FB892, 0);
    w_u8(0x800FB893, 0);
    w_u8(0x800FB8EA, pal);
    w_u16(0x800FB8E0, x);
    w_u16(0x800FB8E2, y);
    PutDrawEnv((DRAWENV *)psx_addr(0x800FB878, 92));
    PutDispEnv((DISPENV *)psx_addr(0x800FB8D8, 20));
    front_clear_vram_regions(0);
    w_u8(0x800FB88F, 0);
    PutDrawEnv((DRAWENV *)psx_addr(0x800FB878, 92));
    front_initialize_glyph_widths();
    source = r_u32(0x800FB0EC);
    rectangle.x = 640;
    rectangle.y = 256;
    rectangle.w = 272;
    rectangle.h = 256;
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(source, 272 * 256 * 2));
    DrawSync(0);
    heap_free(r_u32(0x800FB0EC));
    size = resource_file_size("CWLOGO.TIM");
    logo = heap_alloc_front(size);
    resource_load_into("CWLOGO.TIM", logo);
    texture_upload_at(logo, 912, 256, 896, 506);
    texture_decode_tim_native(logo + 4, (TEX_INFO *)psx_addr(0x800F30F8, 28));
    heap_free(logo);
    SetDispMask(1);
    VSync(0);
}
