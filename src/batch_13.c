#include "game_functions.h"
#include "psx_gpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void front_request_options_audio(void)
{
    uint32 root;
    uint32 buffer;
    uint32 sectors;
    uint32 asset;
    uint32 position;
    uint32 destination;
    uint32 word;
    uint32 shift;
    uint32 mask;
    uint32 result;
    FUNCTION_MARKER(0x800A7C3C, "FRONT.OVL");
    root = r_u32(0x800FB0E8u);
    buffer = r_u32(0x80041000u);
    sectors = r_u32(root + 0x1C4u);
    asset = r_u32(root + 0x1CCu);
    w_u32(buffer, 0u);
    w_u32(r_u32(0x80041000u) + 4u, 0u);
    buffer = r_u32(0x80041000u);
    position = root + 0x1C0u;
    shift = ((position + 3u) & 3u) * 8u;
    word = r_u32((position + 3u) & 0xFFFFFFFCu) << (24u - shift);
    shift = (position & 3u) * 8u;
    mask = 0xFFFFFFFFu >> shift;
    word = (word & ~mask) | (r_u32(position & 0xFFFFFFFCu) >> shift);
    destination = buffer + 8u;
    shift = ((destination + 3u) & 3u) * 8u;
    mask = 0xFFFFFFFFu >> (24u - shift);
    w_u32((destination + 3u) & 0xFFFFFFFCu,
        (r_u32((destination + 3u) & 0xFFFFFFFCu) & ~mask) | (word >> (24u - shift)));
    shift = (destination & 3u) * 8u;
    mask = 0xFFFFFFFFu << shift;
    w_u32(destination & 0xFFFFFFFCu,
        (r_u32(destination & 0xFFFFFFFCu) & ~mask) | (word << shift));
    result = (uint32)CdPosToInt((CdlLOC *)psx_addr(position, sizeof(CdlLOC)));
    w_u32(r_u32(0x80041000u) + 28u, result);
    w_u32(r_u32(0x80041000u) + 12u, sectors);
    w_u32(r_u32(0x80041000u) + 20u, 0x800118FEu);
    w_u32(r_u32(0x80041000u) + 24u, 75u);
    w_u32(r_u32(0x80041000u) + 32u, asset);
    w_u32(r_u32(0x80041000u) + 36u, 0u);
    w_u32(r_u32(0x80041000u) + 40u, 0u);
    w_u8(r_u32(0x80041000u) + 48u, 0u);
    w_u32(r_u32(0x80041000u) + 44u, 0u);
    buffer = r_u32(0x80041000u);
    w_u32(0x80041EE8u, 0u);
    w_u32(buffer, 3u);
    result = r_u32(0x80040FF8u);
    if (result == 0u || r_u32(result) == 0u)
        w_u32(0x80040FE4u, 4u);
}

static const char *front_display_option_string(uint32 text)
{
    uint32 length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    return (const char *)psx_addr(text, (size_t)length + 1u);
}

void front_update_display_confirmation(void)
{
    char text[104];
    uint32 layout[5];
    uint32 mode;
    uint32 labels;
    uint32 first;
    uint32 second;
    uint32 width;
    uint32 half_width;
    uint32 current_width;
    uint32 choice;
    uint32 offset;
    uint32 menu;
    uint32 table;
    uint32 callback;
    uint32 value;
    sint32 seconds;
    sint32 screen;
    FUNCTION_MARKER(0x800A76E8, "FRONT.OVL");
    mode = r_u32(0x800311C0u);
    seconds = r_s32(0x800F6FE0u) / (mode != 0u ? 50 : 60);
    labels = r_u32(0x800F6F98u);
    w_u32(0x800FB870u, 0x53u);
    second = r_u32(labels + 0x34u);
    first = r_u32(labels + 0x30u);
    sprintf(text, "%s%d%s", front_display_option_string(first), seconds, front_display_option_string(second));
    width = r_u32(0x800F6FC8u);
    half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
    layout[0] = 320u - half_width;
    layout[1] = 200u;
    layout[2] = 0x808080u;
    current_width = front_text_width_native(text);
    layout[3] = r_u32(0x800F6FC8u) - current_width;
    layout[4] = 10u;
    front_draw_justified_text_native(text, layout);
    choice = r_u16(0x8010D0DCu) - 31u;
    offset = (uint32)((sint32)(sint16)choice * 8);
    if ((r_u32(0x80053684u) & 0x80u) != 0u) {
        sound_play_sample_pitch(11u, 128u);
        sound_play_sample_pitch(11u, 128u);
        menu = r_u32(0x8010DA70u);
        table = r_u32(menu + 0x10u);
        callback = r_u32(table + offset + 4u);
        psx_dispatch_guest_call(callback, 0u, 0u, 0u, 0u);
        front_store_screen_action(r_s16(0x80114264u), r_s16(0x8010D0DCu));
        table = r_u32(r_u32(0x8010DA70u) + 0x10u);
        screen = r_s16(0x80114264u);
        value = r_u8(table + offset);
        w_u16(0x800F312Au, (uint32)(screen != (sint32)value));
        menu = r_u32(0x8010DA70u);
        w_u16(0x800F3114u, 1u);
        w_u16(0x800F3116u, 1u);
        w_u16(0x800F3118u, 1u);
        value = r_u8(r_u32(menu + 0x10u) + offset);
        w_u16(0x800B737Cu, 1u);
        w_u32(0x800F6FE8u, 1u);
        w_u16(0x80114264u, value);
    } else if (r_u32(0x800F6FE0u) == 0u) {
        sound_play_sample_pitch(11u, 128u);
        sound_play_sample_pitch(11u, 128u);
        front_store_screen_action(r_s16(0x80114264u), 31);
        menu = r_u32(0x8010DA70u);
        screen = r_s16(0x80114264u);
        value = r_u8(menu + 0x14u);
        w_u16(0x800F312Au, (uint32)(screen != (sint32)value));
        menu = r_u32(0x8010DA70u);
        w_u16(0x800F3114u, 1u);
        w_u16(0x800F3116u, 1u);
        w_u16(0x800F3118u, 1u);
        value = r_u8(menu + 0x14u);
        w_u16(0x800B737Cu, 1u);
        w_u16(0x80114264u, value);
    }
    w_u32(0x800F6FE0u, r_u32(0x800F6FE0u) - 1u);
}

void front_update_sound_options_menu(void)
{
    PSX_RECT rectangle;
    sint32 option;
    sint32 progress;
    sint32 voice;
    uint32 color;
    uint32 value;
    uint32 counter;
    uint32 base;
    uint32 mode;
    uint32 direction;
    uint32 sample;
    uint32 flags;
    uint32 texture;
    uint32 text_x;
    FUNCTION_MARKER(0x800A7D94, "FRONT.OVL");
    option = r_s16(0x8010D0DCu) - 31;
    w_u32(0x800FB870u, 0x154u);
    color = 0x404040u;
    if (option == 0) {
        if (r_u32(0x800BD3D4u) != 0u) {
            base = r_u32(0x800FB0E8u);
            stream_request_position(base + 0x160u, r_u32(base + 0x164u), r_u32(base + 0x16Cu));
        }
        color = 0x808080u;
        if ((r_u32(0x8004616Cu) & 0x8000u) != 0u) {
            value = r_u32(0x800F2D6Cu);
            if ((sint32)value > 0)
                w_u32(0x800F2D6Cu, value - 1u);
        }
        if ((r_u32(0x8004616Cu) & 0x2000u) != 0u) {
            value = r_u32(0x800F2D6Cu);
            if ((sint32)value < 255)
                w_u32(0x800F2D6Cu, value + 1u);
        }
    }
    rectangle.x = 40;
    rectangle.y = 115;
    rectangle.w = 350;
    rectangle.h = 0;
    front_draw_colored_text(r_u32(r_u32(0x800F2D58u)), 40u, 100u, color);
    progress = (sint32)(r_u32(0x800F2D6Cu) * 1000u) / 255;
    front_draw_volume_bar(0x1B5u, &rectangle, color, progress);
    color = 0x404040u;
    if (option == 1) {
        if (r_u32(0x800BD3D4u) != 1u)
            stream_cancel_current();
        color = 0x808080u;
        if ((r_u32(0x8004616Cu) & 0x8000u) != 0u) {
            value = r_u32(0x800F2D70u);
            if ((sint32)value > 0) {
                counter = r_u32(0x800BD3D8u);
                w_u32(0x800F2D70u, value - 1u);
                w_u32(0x800BD3D8u, counter + 1u);
            }
        }
        if ((r_u32(0x8004616Cu) & 0x2000u) != 0u) {
            value = r_u32(0x800F2D70u);
            if ((sint32)value < 255) {
                counter = r_u32(0x800BD3D8u);
                w_u32(0x800F2D70u, value + 1u);
                w_u32(0x800BD3D8u, counter + 1u);
            }
        }
        if (r_u32(0x800BD3D8u) == 15u) {
            sound_play_sample_pitch(10u, 128u);
            sound_play_sample_pitch(10u, 128u);
            sound_play_sample_pitch(10u, 128u);
            sound_play_sample_pitch(10u, 128u);
            w_u32(0x800BD3D8u, 0u);
        }
    }
    rectangle.x = 40;
    rectangle.y = 165;
    front_draw_colored_text(r_u32(r_u32(0x800F2D58u) + 4u), 40u, 150u, color);
    progress = (sint32)(r_u32(0x800F2D70u) * 1000u) / 255;
    front_draw_volume_bar(0x1B5u, &rectangle, color, progress);
    color = 0x404040u;
    if (option == 2) {
        if (r_u32(0x800BD3D4u) != 2u)
            front_request_options_audio();
        color = 0x808080u;
        if ((r_u32(0x8004616Cu) & 0x8000u) != 0u) {
            value = r_u32(0x800F2D74u);
            if ((sint32)value > 0)
                w_u32(0x800F2D74u, value - 1u);
        }
        if ((r_u32(0x8004616Cu) & 0x2000u) != 0u) {
            value = r_u32(0x800F2D74u);
            if ((sint32)value < 255)
                w_u32(0x800F2D74u, value + 1u);
        }
    }
    rectangle.x = 40;
    rectangle.y = 215;
    front_draw_colored_text(r_u32(r_u32(0x800F2D58u) + 8u), 40u, 200u, color);
    progress = (sint32)(r_u32(0x800F2D74u) * 1000u) / 255;
    front_draw_volume_bar(0x1B5u, &rectangle, color, progress);
    color = 0x404040u;
    if (option == 3) {
        if (r_u32(0x800BD3D4u) != 3u)
            stream_cancel_current();
        color = 0x808080u;
        for (direction = 0u; direction < 2u; ++direction) {
            if ((r_u32(0x80053684u) & (direction == 0u ? 0x8000u : 0x2000u)) == 0u)
                continue;
            value = r_u32(0x800F2D78u);
            if (direction == 0u ? (sint32)value <= 0 : (sint32)value >= 2)
                continue;
            base = r_u32(0x800F3130u);
            w_u32(0x800F2D78u, direction == 0u ? value - 1u : value + 1u);
            sound_stop_voice(base);
            sound_tick();
            sound_stop_voice(r_u32(0x800F3134u));
            sound_tick();
            value = r_u32(0x800F2D78u);
            if (value <= 2u) {
                mode = 1u << value;
                w_u16(0x80011900u, mode);
                sound_select_mode(mode);
            }
            sample = r_u32(0x80042970u);
            flags = r_u32(0x800313A0u);
            voice = sound_reserve_voice((sint32)sample, 50u, 15, 15, 0x100u, flags);
            sample = r_u32(0x80042970u);
            flags = r_u32(0x800313A0u);
            w_u32(0x800F3130u, (uint32)voice);
            voice = sound_reserve_voice((sint32)sample, 50u, 15, 15, 0x89u, flags);
            w_u32(0x800F3134u, (uint32)voice);
            sound_play_sample_pitch(10u, 128u);
            sound_play_sample_pitch(10u, 128u);
        }
    }
    w_u32(0x800FB870u, 0x154u);
    rectangle.y = 250;
    rectangle.x = 40;
    rectangle.w = 100;
    front_draw_colored_text(r_u32(r_u32(0x800F2D58u) + 12u), 40u, 250u, color);
    text_x = r_u32(0x800F2D7Cu);
    w_u32(0x800FB870u, 0xFDu);
    rectangle.y = (sint16)((uint16)rectangle.y + 15u);
    front_draw_colored_text(r_u32(r_u32(0x800F2D58u) + 0x14u), text_x, (uint32)((sint32)rectangle.y + 4), color);
    texture = r_u32(0x800F2D78u) == 0u ? 0x1B5u : 0x1B8u;
    front_draw_left_frame(texture, &rectangle, color);
    text_x = r_u32(0x800F2D80u);
    rectangle.x = 165;
    front_draw_colored_text(r_u32(r_u32(0x800F2D58u) + 0x18u), text_x, (uint32)((sint32)rectangle.y + 4), color);
    texture = r_u32(0x800F2D78u) == 1u ? 0x1AFu : 0x1B2u;
    front_draw_middle_frame(texture, &rectangle, color);
    text_x = r_u32(0x800F2D84u);
    rectangle.x = 290;
    front_draw_colored_text(r_u32(r_u32(0x800F2D58u) + 0x1Cu), text_x, (uint32)((sint32)rectangle.y + 4), color);
    texture = r_u32(0x800F2D78u) == 2u ? 0x1AFu : 0x1B2u;
    front_draw_right_frame(texture, &rectangle, color);
    value = r_u32(0x800F2D6Cu) << 7;
    base = r_u32(0x800F2D70u);
    w_u16(0x800118FAu, value);
    sound_set_level(base);
    value = r_u32(0x800F2D74u);
    w_u32(0x800BD3D4u, (uint32)option);
    w_u16(0x800118FEu, value << 7);
}

void front_snapshot_display_options(void)
{
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 mode;
    uint32 selected_mode;
    FUNCTION_MARKER(0x800A7608, "FRONT.OVL");
    first = r_u32(0x800F6F9Cu);
    second = r_u32(0x800F6FACu);
    third = r_u32(0x800F6FB0u);
    mode = r_u32(0x800311C0u);
    selected_mode = r_u32(0x800F6FBCu);
    w_u16(0x800118F4u, first);
    w_u16(0x800118F8u, second);
    w_u16(0x800118F6u, third);
    w_u32(0x800F6FE4u, mode);
    if (selected_mode != 0u)
        front_select_pal_display();
    else
        front_select_ntsc_display();
    w_u32(0x800F6FE8u, 0u);
}

void front_initialize_display_confirmation(void)
{
    char text[104];
    uint32 mode;
    uint32 labels;
    uint32 first;
    uint32 second;
    uint32 width;
    FUNCTION_MARKER(0x800A7670, "FRONT.OVL");
    mode = r_u32(0x800311C0u);
    w_u16(0x800F3114u, 0u);
    w_u32(0x800F6FE0u, mode != 0u ? 750u : 900u);
    labels = r_u32(0x800F6F98u);
    w_u32(0x800FB870u, 0x53u);
    first = r_u32(labels + 0x30u);
    second = r_u32(labels + 0x34u);
    sprintf(text, "%s000%s", front_display_option_string(first), front_display_option_string(second));
    width = front_text_width_native(text);
    w_u32(0x800F6FC8u, width);
}

void front_restore_display_options(void)
{
    FUNCTION_MARKER(0x800A79D8, "FRONT.OVL");
    front_configure_layer(0u, 0u, 0xA9u);
    if (r_u32(0x800F6FE8u) == 0u) {
        if (r_u32(0x800F6FE4u) != 0u)
            front_select_pal_display();
        else
            front_select_ntsc_display();
    }
}

void front_initialize_sound_options_menu(void)
{
    sint32 music;
    sint32 effects;
    sint32 voice;
    sint32 output;
    uint32 labels;
    uint32 width;
    uint32 half_width;
    uint32 base;
    sint32 screen;
    FUNCTION_MARKER(0x800A7A40, "FRONT.OVL");
    front_configure_layer(1u, 1u, 0xA8u);
    stream_clear_queue();
    w_u32(0x800F2D58u, 0x800F2934u);
    music = r_s16(0x800118FAu);
    effects = r_s16(0x800118FCu);
    voice = r_s16(0x800118FEu);
    output = r_s16(0x80011900u);
    w_u32(0x800F2D5Cu, (uint32)music);
    w_u32(0x800F2D60u, (uint32)effects);
    w_u32(0x800F2D64u, (uint32)voice);
    w_u32(0x800F2D68u, (uint32)output);
    w_u32(0x800F2D6Cu, (uint32)(music / 128));
    w_u32(0x800F2D70u, (uint32)effects);
    w_u32(0x800F2D74u, (uint32)(voice / 128));
    if (output == 1)
        w_u32(0x800F2D78u, 0u);
    else if (output == 2)
        w_u32(0x800F2D78u, 1u);
    else if (output == 4)
        w_u32(0x800F2D78u, 2u);
    labels = r_u32(0x800F2D58u);
    w_u32(0x800FB870u, 0xFDu);
    width = front_text_width(r_u32(labels + 0x14u));
    half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
    labels = r_u32(0x800F2D58u);
    w_u32(0x800F2D7Cu, 90u - half_width);
    width = front_text_width(r_u32(labels + 0x18u));
    half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
    labels = r_u32(0x800F2D58u);
    w_u32(0x800F2D80u, 215u - half_width);
    width = front_text_width(r_u32(labels + 0x1Cu));
    half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
    w_u32(0x800F2D84u, 340u - half_width);
    disc_wait_stream();
    screen = r_s16(0x8010D0DCu);
    if (screen == 31) {
        base = r_u32(0x800FB0E8u);
        stream_request_position(base + 0x160u, r_u32(base + 0x164u), r_u32(base + 0x16Cu));
    } else if (screen == 33) {
        front_request_options_audio();
    }
}

void front_select_pal_display(void)
{
    DISPENV current;
    DISPENV saved;
    PSX_RECT rectangle;
    uint32 x;
    uint32 y;
    uint32 previous_x;
    uint32 previous_y;
    uint32 counter;
    uint32 total;
    uint32 mode;
    uint32 object;
    uint32 word;
    uint32 index;
    sint32 elapsed;
    sint32 divisor;
    sint32 quotient;
    sint32 position;
    FUNCTION_MARKER(0x800A6F04, "FRONT.OVL");
    if (r_u32(0x800311C0u) != 0u)
        return;
    x = r_u32(0x80010564u);
    y = r_u32(0x80010568u);
    previous_x = r_u32(0x8001056Cu);
    previous_y = r_u32(0x80010570u);
    w_u32(0x80010574u, x);
    w_u32(0x80010578u, y);
    w_u32(0x80010564u, previous_x);
    w_u32(0x80010568u, previous_y);
    if (!gpu_get_display_env(&current))
        abort();
    VSync(0);
    SetDispMask(0);
    for (index = 0u; index < 3u; ++index) {
        PutDrawEnv((DRAWENV *)psx_addr(0x800FB878u, sizeof(DRAWENV)));
        front_swap_render_buffers();
        DrawSync(0);
        VSync(0);
    }
    saved = current;
    current.screen.h = 240;
    PutDispEnv(&current);
    VSync(0);
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 640;
    rectangle.h = 512;
    ClearImage(&rectangle, 0u, 0u, 0u);
    DrawSync(0);
    VSync(0);
    rectangle.x = 0;
    rectangle.y = 511;
    rectangle.w = 640;
    rectangle.h = 2;
    ClearImage(&rectangle, 0u, 0u, 0u);
    DrawSync(0);
    VSync(0);
    VSync(0);
    counter = (uint32)VSync(-1);
    elapsed = (sint32)(counter - r_u32(0x800F3124u));
    divisor = r_s16(0x800F3128u);
    if (divisor == 0 || (elapsed == (sint32)0x80000000u && divisor == -1))
        abort();
    quotient = elapsed / divisor;
    total = r_u32(0x800F3120u);
    w_u32(0x800F30F4u, counter);
    w_u32(0x800F3124u, counter);
    w_u16(0x800F3128u, 50u);
    w_u32(0x800311C0u, 1u);
    w_u32(0x800F3120u, total + (uint32)quotient);
    system_set_video_mode(1);
    VSync(0);
    x = r_u32(0x80010564u);
    y = r_u32(0x80010568u);
    mode = r_u32(0x800311C0u);
    saved.screen.h = 256;
    saved.disp.h = 512;
    saved.screen.x = (sint16)x;
    saved.screen.y = (sint16)y;
    saved.pad0 = (uint8)mode;
    PutDispEnv(&saved);
    for (index = 0u; index < 20u; index += 4u) {
        memcpy(&word, (const uint8 *)&saved + index, 4u);
        w_u32(0x800FB8D8u + index, word);
    }
    object = r_u32(0x8010D0E0u);
    for (index = 0u; index < 30u; ++index)
        VSync(0);
    front_initialize_background();
    while (object != 0u) {
        position = r_s16(object + 0x1Au);
        if (position > 400)
            w_u16(object + 0x1Au, (uint32)position + 32u);
        object = r_u32(object + 0x10u);
    }
    SetDispMask(1);
}

void front_select_ntsc_display(void)
{
    DISPENV current;
    DISPENV saved;
    PSX_RECT rectangle;
    uint32 x;
    uint32 y;
    uint32 previous_x;
    uint32 previous_y;
    uint32 counter;
    uint32 total;
    uint32 mode;
    uint32 object;
    uint32 word;
    uint32 index;
    sint32 elapsed;
    sint32 divisor;
    sint32 quotient;
    sint32 position;
    FUNCTION_MARKER(0x800A729C, "FRONT.OVL");
    if (r_u32(0x800311C0u) == 0u)
        return;
    x = r_u32(0x80010564u);
    y = r_u32(0x80010568u);
    previous_x = r_u32(0x80010574u);
    previous_y = r_u32(0x80010578u);
    w_u32(0x8001056Cu, x);
    w_u32(0x80010570u, y);
    w_u32(0x80010564u, previous_x);
    w_u32(0x80010568u, previous_y);
    if (!gpu_get_display_env(&current))
        abort();
    VSync(0);
    SetDispMask(0);
    for (index = 0u; index < 3u; ++index) {
        PutDrawEnv((DRAWENV *)psx_addr(0x800FB878u, sizeof(DRAWENV)));
        front_swap_render_buffers();
        DrawSync(0);
        VSync(0);
    }
    saved = current;
    current.screen.h = 240;
    PutDispEnv(&current);
    VSync(0);
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 640;
    rectangle.h = 512;
    ClearImage(&rectangle, 0u, 0u, 0u);
    DrawSync(0);
    VSync(0);
    VSync(0);
    counter = (uint32)VSync(-1);
    elapsed = (sint32)(counter - r_u32(0x800F3124u));
    divisor = r_s16(0x800F3128u);
    if (divisor == 0 || (elapsed == (sint32)0x80000000u && divisor == -1))
        abort();
    quotient = elapsed / divisor;
    total = r_u32(0x800F3120u);
    w_u32(0x800F30F4u, counter);
    w_u32(0x800F3124u, counter);
    w_u32(0x800311C0u, 0u);
    w_u16(0x800F3128u, 60u);
    w_u32(0x800F3120u, total + (uint32)quotient);
    system_set_video_mode(0);
    VSync(0);
    x = r_u32(0x80010564u);
    y = r_u32(0x80010568u);
    mode = r_u32(0x800311C0u);
    saved.screen.x = (sint16)x;
    saved.screen.y = (sint16)y;
    saved.pad0 = (uint8)mode;
    PutDispEnv(&saved);
    for (index = 0u; index < 20u; index += 4u) {
        memcpy(&word, (const uint8 *)&saved + index, 4u);
        w_u32(0x800FB8D8u + index, word);
    }
    for (index = 0u; index < 30u; ++index)
        VSync(0);
    front_initialize_background();
    object = r_u32(0x8010D0E0u);
    while (object != 0u) {
        position = r_s16(object + 0x1Au);
        if (position > 400)
            w_u16(object + 0x1Au, (uint32)position - 32u);
        object = r_u32(object + 0x10u);
    }
    SetDispMask(1);
}

void front_noop_5700(void)
{
    FUNCTION_MARKER(0x800A5700, "FRONT.OVL");
}

void front_noop_68a0(void)
{
    FUNCTION_MARKER(0x800A68A0, "FRONT.OVL");
}

void front_noop_6bb0(void)
{
    FUNCTION_MARKER(0x800A6BB0, "FRONT.OVL");
}

void front_clear_password_confirmation(void)
{
    FUNCTION_MARKER(0x800A6BB8, "FRONT.OVL");
    w_u16(0x800F3114u, 0u);
}

void front_apply_named_code(sint32 index)
{
    FUNCTION_MARKER(0x800AC58C, "FRONT.OVL");
    switch ((uint32)index) {
    case 0u:
        w_u8(0x80011934u, 1u);
        break;
    case 1u:
        w_u8(0x80011935u, 1u);
        break;
    case 2u:
        w_u8(0x80011936u, 1u);
        break;
    case 3u:
        w_u8(0x80011937u, 1u);
        break;
    case 4u:
        w_u8(0x80011934u, 0u);
        w_u8(0x80011935u, 0u);
        w_u8(0x80011936u, 0u);
        w_u8(0x80011937u, 0u);
        break;
    default:
        break;
    }
}

void front_apply_confirmed_password(void)
{
    sint32 index;
    FUNCTION_MARKER(0x800A6BC8, "FRONT.OVL");
    index = r_s16(0x800FB7EAu);
    if (index >= 0)
        front_apply_named_code(index);
    else
        front_apply_settings_code();
}

void front_commit_screen_position(void)
{
    uint32 x;
    uint32 y;
    uint32 mode;
    FUNCTION_MARKER(0x800A6C04, "FRONT.OVL");
    x = r_u32(0x800F6FCCu);
    y = r_u32(0x800F6FD0u);
    mode = r_u32(0x800311C0u);
    w_u32(0x80010564u, x);
    w_u32(0x80010568u, y);
    if (mode != 0u) {
        w_u32(0x8001056Cu, x);
        w_u32(0x80010570u, y);
    } else {
        w_u32(0x80010574u, x);
        w_u32(0x80010578u, y);
    }
}

void front_initialize_screen_position_menu(void)
{
    uint32 x;
    uint32 y;
    uint32 labels;
    uint32 width;
    uint32 half_width;
    FUNCTION_MARKER(0x800A6C64, "FRONT.OVL");
    front_configure_layer(0u, 4u, 0xA9u);
    front_configure_layer(1u, 1u, 0xA8u);
    w_u16(0x800F6FD8u, 30u);
    w_u16(0x800F6FD4u, 30u);
    x = r_u32(0x80010564u);
    y = r_u32(0x80010568u);
    labels = r_u32(0x800F6F98u);
    w_u16(0x800F6FD6u, 8u);
    w_u32(0x800FB870u, 0x53u);
    w_u32(0x800F6FCCu, x);
    w_u32(0x800F6FD0u, y);
    width = front_text_width(r_u32(labels + 0x38u));
    half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
    w_u32(0x800F6FC8u, 320u - half_width);
}

void front_update_screen_position_menu(void)
{
    DISPENV environment;
    uint32 labels;
    uint32 text_x;
    uint32 buttons;
    uint32 repeat;
    uint32 value;
    FUNCTION_MARKER(0x800A6D18, "FRONT.OVL");
    if (!gpu_get_display_env(&environment))
        abort();
    labels = r_u32(0x800F6F98u);
    text_x = r_u32(0x800F6FC8u);
    w_u32(0x800FB870u, 0x53u);
    front_draw_colored_text(r_u32(labels + 0x38u), text_x, 200u, 0x808080u);
    buttons = r_u32(0x80053684u);
    repeat = buttons;
    if (buttons == 0u) {
        value = r_u16(0x800F6FD4u) - 1u;
        w_u16(0x800F6FD4u, value);
        repeat = 0u;
        if ((uint16)value == 0u) {
            value = r_u16(0x800F6FD6u);
            repeat = r_u32(0x8004616Cu);
            w_u16(0x800F6FD4u, value);
        }
    } else {
        w_u16(0x800F6FD4u, r_u16(0x800F6FD8u));
    }
    if ((repeat & 0x8000u) != 0u) {
        value = r_u32(0x800F6FCCu);
        if ((sint32)value > 0)
            w_u32(0x800F6FCCu, value - 1u);
    }
    if ((repeat & 0x2000u) != 0u) {
        value = r_u32(0x800F6FCCu);
        if ((sint32)value < 40)
            w_u32(0x800F6FCCu, value + 1u);
    }
    if ((repeat & 0x1000u) != 0u) {
        value = r_u32(0x800F6FD0u);
        if ((sint32)value > 0)
            w_u32(0x800F6FD0u, value - 1u);
    }
    if ((repeat & 0x4000u) != 0u) {
        value = r_u32(0x800F6FD0u);
        if ((sint32)value < 32)
            w_u32(0x800F6FD0u, value + 1u);
    }
    environment.screen.x = (sint16)r_u32(0x800F6FCCu);
    environment.screen.y = (sint16)r_u32(0x800F6FD0u);
    PutDispEnv(&environment);
}

static const char *front_password_string(void)
{
    uint32 length = 0u;
    while (r_u8(0x800FB7D0u + length) != 0u)
        ++length;
    return (const char *)psx_addr(0x800FB7D0u, (size_t)length + 1u);
}

void front_initialize_password_menu(void)
{
    PSX_RECT rectangle;
    uint32 length;
    uint32 result;
    uint32 value;
    FUNCTION_MARKER(0x800A6760, "FRONT.OVL");
    front_configure_layer(1u, 1u, 0xA8u);
    if (r_s16(0x800F3114u) != 0) {
        length = 0u;
        while (r_u8(0x800FB220u + length) != 0u)
            ++length;
        strcpy((char *)psx_addr(0x800FB7D0u, (size_t)length + 1u), (const char *)psx_addr(0x800FB220u, (size_t)length + 1u));
    }
    if (strcmp(front_password_string(), "lZaampyIODg?Pt7e") == 0)
        w_u8(0x800FB7D0u, 0u);
    w_u32(0x800FB7F4u, 0x800F289Cu);
    w_u16(0x800FB7EAu, 0xFFFFu);
    length = (uint32)strlen(front_password_string());
    w_u16(0x800FB7E8u, length);
    rectangle.x = 80;
    rectangle.y = 200;
    rectangle.w = 280;
    rectangle.h = 20;
    front_initialize_character_picker(0xFDu, &rectangle, 0x800BD390u);
    result = front_decode_settings_code(front_password_string());
    w_u16(0x800FB7ECu, result);
    if ((sint16)result == 1) {
        value = r_u8(0x8010D0F1u);
        w_u8(0x800F2FA9u, 0u);
        w_u16(0x800FB7F0u, value);
    } else {
        w_u8(0x800F2FA9u, 1u);
    }
    w_u16(0x800FB7EEu, r_u16(0x800F3114u));
}

void front_update_password_menu(void)
{
    PSX_RECT rectangle;
    uint32 labels;
    uint32 key;
    uint32 value;
    uint32 result;
    uint32 text;
    sint32 length;
    sint32 named;
    sint32 index;
    FUNCTION_MARKER(0x800A68A8, "FRONT.OVL");
    labels = r_u32(0x800FB7F4u);
    w_u32(0x800FB870u, 0xFDu);
    rectangle.x = 40;
    rectangle.y = 130;
    rectangle.w = 120;
    rectangle.h = 10;
    front_draw_colored_text(r_u32(labels), 48u, 134u, 0x808080u);
    front_draw_colored_text(0x800FB7D0u, 164u, 136u, 0x808080u);
    front_draw_text_entry_frame(0x1B8u, &rectangle, 0x808080u);
    if (r_s16(0x800FB7EEu) == 0) {
        key = front_update_character_picker();
        if ((uint8)key == 8u) {
            length = r_s16(0x800FB7E8u);
            if (length <= 0)
                return;
            value = (uint32)length - 1u;
        } else if ((uint8)key == 13u) {
            w_u16(0x800F3114u, 1u);
            w_u16(0x800FB7EEu, 1u);
            value = (uint32)strlen(front_password_string());
            strcpy((char *)psx_addr(0x800BD378u, (size_t)value + 1u), front_password_string());
            named = front_find_named_code(front_password_string());
            w_u16(0x800FB7EAu, (uint32)named);
            if ((sint16)named == -1) {
                result = front_decode_settings_code(front_password_string());
                value = r_u8(0x8010D0F1u);
                w_u16(0x800FB7ECu, result);
                w_u16(0x800FB7F0u, value);
            } else {
                w_u16(0x800FB7ECu, 1u);
            }
            w_u8(0x800F2FA9u, r_s16(0x800FB7ECu) == 1 ? 0u : 1u);
            return;
        } else {
            if ((uint8)key == 0u)
                return;
            length = r_s16(0x800FB7E8u);
            if (length >= 22)
                return;
            w_u8(0x800FB7D0u + (uint32)length, key);
            value = r_u16(0x800FB7E8u) + 1u;
        }
        w_u16(0x800FB7E8u, value);
        w_u8(0x800FB7D0u + (uint32)(sint32)(sint16)value, 0u);
        return;
    }
    if (r_s16(0x800FB7ECu) == 1) {
        named = r_s16(0x800FB7EAu);
        if (named == -1) {
            index = r_s16(0x800FB7F0u);
            if (index < 90)
                text = r_u32(0x800F2468u + (uint32)index * 4u);
            else
                text = r_u32(0x800F2350u + (uint32)index * 4u);
        } else {
            text = r_u32(0x800F2D0Cu + (uint32)named * 4u);
        }
    } else {
        text = r_u32(r_u32(0x800FB7F4u) + 8u);
    }
    front_draw_colored_text(text, 46u, 64u, 0x808080u);
}

void front_draw_mission_briefing(void)
{
    uint32 packet;
    uint32 next;
    uint32 size;
    uint32 height;
    uint32 width;
    uint32 remaining;
    uint32 table;
    uint32 step;
    sint32 current;
    sint32 maximum;
    uint16 clut;
    uint16 page;
    FUNCTION_MARKER(0x800A5718, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    SetSprt((SPRT *)psx_addr(packet, 20u));
    w_u16(packet + 8u, 20u);
    size = r_u16(0x800F3160u);
    w_u8(packet + 12u, 64u);
    w_u16(packet + 10u, size + 120u);
    w_u8(packet + 13u, r_u16(0x800F3160u));
    width = r_u16(0x800F3100u) << 2;
    w_u16(packet + 16u, width > 191u ? 191u : width);
    size = r_u16(0x800F3160u);
    height = r_u16(0x800F3102u);
    w_u16(packet + 18u, height - size * 2u);
    clut = GetClut(896, 506);
    w_u16(packet + 14u, clut);
    SetShadeTex((SPRT *)psx_addr(packet, 20u), 1);
    next = packet + 20u;
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 20u));
    w_u32(0x801132B8u, next);
    page = GetTPage(0, 1, 912, 256);
    SetDrawMode((DR_MODE *)psx_addr(next, 12u), 0, 1, page, NULL);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(next, 12u));
    next += 12u;
    w_u32(0x801132B8u, next);
    if (width > 191u) {
        packet = next;
        SetSprt((SPRT *)psx_addr(packet, 20u));
        w_u16(packet + 8u, 211u);
        size = r_u16(0x800F3160u);
        w_u8(packet + 12u, 0u);
        w_u16(packet + 10u, size + 120u);
        remaining = width - 192u;
        w_u8(packet + 13u, r_u16(0x800F3160u));
        w_u16(packet + 16u, remaining < 256u ? remaining : 255u);
        size = r_u16(0x800F3160u);
        height = r_u16(0x800F3102u);
        w_u16(packet + 18u, height - size * 2u);
        clut = GetClut(896, 506);
        w_u16(packet + 14u, clut);
        SetShadeTex((SPRT *)psx_addr(packet, 20u), 1);
        next = packet + 20u;
        table = r_u32(0x801132BCu);
        AddPrim(psx_addr(table, 4u), psx_addr(packet, 20u));
        w_u32(0x801132B8u, next);
        page = GetTPage(0, 1, 960, 256);
        SetDrawMode((DR_MODE *)psx_addr(next, 12u), 0, 1, page, NULL);
        table = r_u32(0x801132BCu);
        AddPrim(psx_addr(table, 4u), psx_addr(next, 12u));
        next += 12u;
        w_u32(0x801132B8u, next);
    }
    if (r_s16(0x800F311Au) == 1) {
        current = r_s16(0x800F3160u);
        if (current > 0) {
            step = r_u16(0x800F3162u);
            w_u16(0x800F3160u, (uint32)current - step);
            current = r_s16(0x800F3160u);
        }
        if (current < 0)
            w_u16(0x800F3160u, 0u);
    } else {
        current = r_s16(0x800F3160u);
        maximum = r_u16(0x800F3102u) >> 1;
        if (current < maximum) {
            step = r_u16(0x800F3164u);
            w_u16(0x800F3160u, (uint32)current + step);
        }
        current = r_s16(0x800F3160u);
        if (maximum < current)
            w_u16(0x800F3160u, (uint32)maximum);
    }
}

void front_commit_control_axis_calibration(void)
{
    FUNCTION_MARKER(0x800A52C8, "FRONT.OVL");
    front_apply_control_axis_calibration();
}

uint32 front_retain_control_viewer(void)
{
    FUNCTION_MARKER(0x800A52E8, "FRONT.OVL");
    w_u8(0x800BC810u, 1u);
    return 1u;
}

void front_reset_mission_briefing(void)
{
    uint32 state;
    uint32 size;
    sint32 transition;
    FUNCTION_MARKER(0x800A5638, "FRONT.OVL");
    state = r_u32(0x800BD374u);
    if (state == 0u)
        front_configure_layer(1u, 3u, 0xA8u);
    else
        w_u32(0x800BD374u, 0u);
    front_configure_layer(0u, 0u, 0xA9u);
    transition = r_s16(0x800F311Au);
    w_u32(0x800BD370u, 1u);
    if (transition == 0) {
        size = r_u16(0x800F3102u);
        w_u16(0x800F311Au, 1u);
        w_u16(0x800F3160u, size >> 1);
        w_u16(0x800F3162u, size / 40u);
        w_u16(0x800F3164u, size / 15u);
    }
}

void front_initialize_mission_briefing(void)
{
    uint32 state;
    uint32 record;
    uint32 base;
    FUNCTION_MARKER(0x800A5380, "FRONT.OVL");
    w_u16(0x800F3118u, 0u);
    front_configure_layer(1u, 0xFFFFFFFFu, 0xA8u);
    state = r_u32(0x800BD370u);
    w_u32(0x800BD374u, 1u);
    w_u32(0x80113218u, state);
    if (state == 0u) {
        w_u32(0x800FB0F8u, r_u32(0x800311C0u) != 0u ? 550u : 660u);
        if (r_s16(0x8005960Cu) < 2 && r_s16(0x8005960Eu) < 2)
            record = 0x1C0u;
        else
            record = 0x610u;
        base = r_u32(0x800FB0E8u);
        stream_request_segments(base + record, r_u32(base + record + 4u), r_u32(base + record + 12u), 0x800118FEu, 1, 0u, NULL);
    }
    if (r_u32(0x80011928u) == 0u)
        w_u8(0x800F2FABu, 1u);
    if (r_u8(0x80011935u) == 0u)
        w_u8(0x800F2FACu, 2u);
    front_reset_mission_briefing();
}

void front_update_mission_briefing(void)
{
    uint32 timer;
    uint32 base;
    FUNCTION_MARKER(0x800A54CC, "FRONT.OVL");
    if (r_u32(0x80113218u) == 0u) {
        timer = r_u32(0x800FB0F8u);
        if ((sint32)timer > 0) {
            --timer;
            w_u32(0x800FB0F8u, timer);
            if (timer == 0u) {
                base = r_u32(0x800FB0E8u);
                stream_request_segments(base + 0x5F8u, r_u32(base + 0x5FCu), r_u32(base + 0x604u), 0x800118FEu, 1, 0u, NULL);
            }
        }
    }
    front_draw_mission_briefing();
}

void front_select_mission_briefing_text(void)
{
    uint32 table;
    uint32 source;
    uint32 destination;
    uint32 length;
    uint32 offset;
    sint32 mode;
    FUNCTION_MARKER(0x800A5548, "FRONT.OVL");
    if (r_s16(0x8005960Cu) >= 91)
        w_u32(0x800F3150u, 1u);
    if (r_u32(0x800F3150u) == 1u) {
        offset = 0x24u;
    } else {
        mode = r_s16(0x800595F8u);
        if (mode >= 2 || (mode == 1 && r_s16(0x800595FAu) >= 2))
            offset = 0x20u;
        else
            offset = 0x1Cu;
    }
    table = r_u32(0x800FB0F4u);
    destination = r_u32(table + 4u);
    source = r_u32(table + offset);
    length = 0u;
    while (r_u8(source + length) != 0u)
        ++length;
    strcpy((char *)psx_addr(destination, (size_t)length + 1u), (const char *)psx_addr(source, (size_t)length + 1u));
    front_reset_mission_briefing();
    if (r_u32(0x80011928u) == 0u)
        w_u8(0x800F2FA9u, 1u);
}

static void front_draw_control_axis_marker_owner(const uint8 *marker, uint32 guest_marker, uint32 phase)
{
    uint32 factor;
    uint32 red;
    uint32 green;
    uint32 blue;
    uint32 packet;
    uint32 atlas;
    uint32 shift;
    uint32 width;
    uint32 height;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    uint32 table;
    sint32 depth;
    uint16 clut;
    uint16 page;
    FUNCTION_MARKER(0x800A4734, "FRONT.OVL");
    if (r_u8(0x8011328Eu) == 1u)
        return;
    factor = phase & 63u;
    if (factor > 32u)
        factor = 64u - factor;
    red = ((marker ? marker[0] : r_u8(guest_marker)) * factor) >> 5;
    green = ((marker ? marker[1] : r_u8(guest_marker + 1u)) * factor) >> 5;
    blue = ((marker ? marker[2] : r_u8(guest_marker + 2u)) * factor) >> 5;
    packet = r_u32(0x801132B8u);
    atlas = 0x8004EB70u + ((uint32)(sint16)(marker ? xport_load_le16(marker + 8u) : r_u16(guest_marker + 8u)) << 4);
    depth = r_s16(atlas + 12u);
    width = (uint32)r_s16(atlas + 8u);
    height = (uint32)r_s16(atlas + 10u) - 1u;
    shift = (2u - (uint32)depth) & 31u;
    width = (width << shift) - 1u;
    SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40u));
    w_u8(packet + 7u, r_u8(packet + 7u) | 2u);
    w_u16(packet + 8u, marker ? xport_load_le16(marker + 4u) : r_u16(guest_marker + 4u));
    w_u16(packet + 10u, marker ? xport_load_le16(marker + 6u) : r_u16(guest_marker + 6u));
    w_u16(packet + 16u, (marker ? xport_load_le16(marker + 4u) : r_u16(guest_marker + 4u)) + width);
    w_u16(packet + 18u, marker ? xport_load_le16(marker + 6u) : r_u16(guest_marker + 6u));
    w_u16(packet + 24u, marker ? xport_load_le16(marker + 4u) : r_u16(guest_marker + 4u));
    w_u16(packet + 26u, (marker ? xport_load_le16(marker + 6u) : r_u16(guest_marker + 6u)) + height);
    w_u16(packet + 32u, (marker ? xport_load_le16(marker + 4u) : r_u16(guest_marker + 4u)) + width);
    w_u16(packet + 34u, (marker ? xport_load_le16(marker + 6u) : r_u16(guest_marker + 6u)) + height);
    w_u8(packet + 12u, (r_u16(atlas) & 63u) << shift);
    w_u8(packet + 13u, r_u16(atlas + 2u));
    first = r_u8(packet + 12u);
    second = r_u8(packet + 13u);
    w_u8(packet + 20u, first + width);
    first = r_u8(packet + 12u);
    w_u8(packet + 29u, second + height);
    second = r_u8(packet + 13u);
    third = r_u8(packet + 20u);
    fourth = r_u8(packet + 29u);
    w_u8(packet + 28u, first);
    w_u8(packet + 21u, second);
    w_u8(packet + 36u, third);
    w_u8(packet + 37u, fourth);
    if ((marker ? marker[10] : r_u8(guest_marker + 10u)) != 0u) {
        first = r_u8(packet + 12u);
        second = r_u8(packet + 20u);
        third = r_u8(packet + 36u);
        w_u8(packet + 20u, first);
        fourth = r_u8(packet + 28u);
        w_u8(packet + 12u, second);
        w_u8(packet + 28u, third);
        w_u8(packet + 36u, fourth);
    }
    if ((marker ? marker[11] : r_u8(guest_marker + 11u)) != 0u) {
        first = r_u8(packet + 13u);
        second = r_u8(packet + 29u);
        third = r_u8(packet + 37u);
        w_u8(packet + 29u, first);
        fourth = r_u8(packet + 21u);
        w_u8(packet + 13u, second);
        w_u8(packet + 21u, third);
        w_u8(packet + 37u, fourth);
    }
    clut = GetClut(r_s16(atlas + 4u), r_s16(atlas + 6u));
    w_u16(packet + 14u, clut);
    page = GetTPage(depth, 1, r_s16(atlas), r_s16(atlas + 2u));
    w_u16(packet + 22u, page);
    w_u8(packet + 4u, red);
    w_u8(packet + 5u, green);
    w_u8(packet + 6u, blue);
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, packet + 40u);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 40u));
}

void front_draw_control_axis_marker(uint32 marker, uint32 phase)
{
    front_draw_control_axis_marker_owner(NULL, marker, phase);
}

void front_draw_control_axis_marker_native(const uint8 *marker, uint32 phase)
{
    front_draw_control_axis_marker_owner(marker, 0u, phase);
}

static uint32 front_prepare_control_axis_marker_owner(uint8 *marker, uint32 guest_marker, uint32 index)
{
    uint32 mode;
    uint32 type;
    uint32 table = 0u;
    uint32 source = 0u;
    uint32 offset = 0u;
    uint32 value;
    FUNCTION_MARKER(0x800A49FC, "FRONT.OVL");
    mode = r_u8(0x8011327Fu);
    if (mode <= 2u) {
        type = r_u8(0x8004EA29u);
        if (mode == 0u) {
            if (type == 0x53u)
                table = 0x800BD304u;
            else if (type == 0x73u)
                table = 0x800BD2FCu;
        } else if (mode == 1u) {
            if (type == 0x53u)
                table = 0x800BD32Cu;
            else if (type == 0x73u)
                table = 0x800BD30Cu;
        } else {
            if (type == 0x53u)
                table = 0x800BD354u;
            else if (type == 0x73u)
                table = 0x800BD34Cu;
        }
        if (table != 0u)
            source = table + (index & 255u) * 8u;
    }
    if (r_u8(0x8011327Eu) == 1u)
        offset = r_u16(source + 4u);
    if (marker) {
        marker[2] = 128u;
        marker[1] = 128u;
        marker[0] = 0u;
        xport_store_le16(marker + 8u, 0x1E3u);
    } else {
        w_u8(guest_marker + 2u, 128u);
        w_u8(guest_marker + 1u, 128u);
        w_u8(guest_marker, 0u);
        w_u16(guest_marker + 8u, 0x1E3u);
    }
    value = r_u16(source) + offset;
    if (marker)
        xport_store_le16(marker + 4u, value);
    else
        w_u16(guest_marker + 4u, value);
    value = r_u16(source + 2u);
    if (marker)
        xport_store_le16(marker + 6u, value);
    else
        w_u16(guest_marker + 6u, value);
    value = r_u8(source + 6u);
    if (marker)
        marker[10] = (uint8)value;
    else
        w_u8(guest_marker + 10u, value);
    value = r_u8(source + 7u);
    if (marker)
        marker[11] = (uint8)value;
    else
        w_u8(guest_marker + 11u, value);
    return value;
}

uint32 front_prepare_control_axis_marker(uint32 marker, uint32 index)
{
    return front_prepare_control_axis_marker_owner(NULL, marker, index);
}

uint32 front_prepare_control_axis_marker_native(uint8 *marker, uint32 index)
{
    return front_prepare_control_axis_marker_owner(marker, 0u, index);
}

void front_draw_control_axis_mode(void)
{
    uint32 type;
    uint32 mode;
    uint32 offset;
    FUNCTION_MARKER(0x800A4B64, "FRONT.OVL");
    type = r_u8(0x8004EA29u);
    w_u32(0x800FB870u, 0xFDu);
    if (type != 0x53u && type != 0x73u)
        return;
    mode = r_u8(0x8011327Eu);
    if (mode > 1u)
        return;
    offset = type == 0x53u ? 0x44u : 0x3Cu;
    front_draw_colored_text(r_u32(r_u32(0x80113270u) + offset + mode * 4u), 46u, 100u, 0x808080u);
}

void front_draw_centered_control_label(uint32 y, uint32 index, uint32 color)
{
    uint32 offset;
    uint32 text;
    uint32 width;
    uint32 half_width;
    FUNCTION_MARKER(0x800A4C6C, "FRONT.OVL");
    offset = (uint32)((sint32)(sint16)index * 4);
    w_u32(0x800FB870u, 0xFDu);
    text = r_u32(r_u32(0x80113270u) + offset);
    width = front_text_width(text);
    text = r_u32(r_u32(0x80113270u) + offset);
    half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
    front_draw_colored_text(text, 320u - half_width, (uint32)(sint32)(sint16)y, (uint32)(sint32)(sint16)color);
}

uint32 front_control_calibration_buttons(void)
{
    uint32 type;
    FUNCTION_MARKER(0x800A4D04, "FRONT.OVL");
    type = r_u8(0x8004EA29u);
    if (type == 0x73u)
        return r_u32(0x80053684u) & 15u;
    if (type == 0x53u)
        return r_u32(0x80053684u) & 0x40u;
    return 0u;
}

void front_initialize_control_axis_calibration(void)
{
    uint32 bindings;
    uint32 type;
    sint32 preset;
    FUNCTION_MARKER(0x800A4440, "FRONT.OVL");
    front_configure_layer(0u, 0xFFFFFFFFu, 0xA9u);
    front_configure_layer(1u, 1u, 0xA8u);
    bindings = r_u32(0x800313C4u);
    type = r_u8(0x8004EA29u);
    w_u16(0x800F3116u, 0u);
    w_u8(0x8011328Eu, 0u);
    w_u8(0x8011327Fu, 1u);
    w_u8(0x80113280u, 0u);
    w_u8(0x80113287u, 0u);
    w_u8(0x8011327Eu, 0u);
    w_u32(0x80113268u, bindings);
    if (type == 0x53u) {
        preset = r_s16(bindings + 0x2Eu);
        if (preset == 0 || preset == 2)
            w_u8(0x8011327Eu, 1u);
    }
}

void front_apply_control_axis_calibration(void)
{
    uint32 limit;
    uint32 group;
    uint32 record;
    uint32 destination;
    uint32 x;
    uint32 y;
    sint32 span_x;
    sint32 span_y;
    sint32 extent;
    FUNCTION_MARKER(0x800A44F0, "FRONT.OVL");
    limit = r_u8(0x8004EA29u) == 0x53u ? 16u : 25u;
    for (group = 0u; group < 2u; ++group) {
        record = 0x80113280u + group * 7u;
        if (r_u8(record) == 0u)
            continue;
        x = r_u8(record + 3u);
        y = r_u8(record + 4u);
        extent = (sint32)r_u8(record + 5u) - (sint32)x;
        if (extent < 0)
            extent = -extent;
        span_x = (sint32)limit - (sint32)x;
        if (span_x < 0)
            span_x = -span_x;
        if ((uint8)extent < (uint8)span_x)
            span_x = extent;
        span_y = (sint32)limit - (sint32)y;
        extent = (sint32)r_u8(record + 6u) - (sint32)y;
        if (span_y < 0)
            span_y = -span_y;
        if (extent < 0)
            extent = -extent;
        if ((uint8)extent < (uint8)span_y)
            span_y = extent;
        destination = group == 0u ? 0x27u : 0x1Fu;
        w_u8(r_u32(0x800313C4u) + destination + 1u, limit);
        w_u8(r_u32(0x800313C4u) + destination, x);
        w_u8(r_u32(0x800313C4u) + destination + 2u, (uint32)span_x);
        w_u8(r_u32(0x800313C4u) + destination + 5u, limit);
        w_u8(r_u32(0x800313C4u) + destination + 4u, y);
        w_u8(r_u32(0x800313C4u) + destination + 6u, (uint32)span_y);
    }
}

void front_initialize_control_preset_menu(void)
{
    uint32 bindings;
    uint32 preset;
    FUNCTION_MARKER(0x800A3AE0, "FRONT.OVL");
    front_configure_layer(0u, 4u, 0xA9u);
    front_configure_layer(1u, 1u, 0xA8u);
    bindings = r_u32(0x800313C4u);
    w_u16(0x8011327Cu, 0u);
    w_u8(0x8011328Eu, 0u);
    preset = r_u16(bindings + 0x2Eu);
    w_u8(0x800313D4u, 0u);
    w_u16(0x80113276u, preset);
}

void front_enable_control_model_draw(uint32 object)
{
    FUNCTION_MARKER(0x800A3B44, "FRONT.OVL");
    w_u32(object + 0x3Cu, 0x8002D2BCu);
    w_u16(object + 0x24Cu, 0u);
}

void front_create_control_viewer_objects(void)
{
    uint32 object;
    uint32 camera;
    uint32 value;
    uint32 z;
    FUNCTION_MARKER(0x800A3B58, "FRONT.OVL");
    object_update_all();
    object_initialize_pool();
    front_init_model_lighting();
    object = front_create_model_object(1, 0u, 0u, 0u, 0u);
    matrix_set_identity(object + 0x11Cu);
    w_u32(0x80040FDCu, object);
    w_u32(object + 0x40u, 0x8008FC10u);
    w_u32(object + 8u, 0u);
    w_u32(object + 0xCu, 0u);
    w_u32(object + 0x10u, 0u);
    w_u32(0x80040C94u, object);
    object = front_create_model_object(0, 0u, 0u, 0u, 0u);
    value = r_u32(0x800311B8u);
    w_u32(object + 0x3Cu, 0x8002493Cu);
    w_u32(object + 0x68u, 0u);
    w_u32(0x800BC6CCu, 0u);
    w_u32(object + 0x38u, 0x80090144u);
    w_u32(0x800FB80Cu, value);
    matrix_set_identity(object + 0x11Cu);
    camera = r_u32(0x80040FDCu);
    w_u32(object + 0x10u, 7500u);
    w_u32(object + 8u, 0u);
    w_u32(object + 0xCu, 0u);
    w_u32(object + 0x20u, 0u);
    w_u32(object + 0x24u, 0x200u);
    value = (uint32)r_s16(camera + 0x128u) * 75u;
    w_u32(object + 0xFCu, (uint32)((sint32)value >> 12));
    value = (uint32)r_s16(camera + 0x12Au) * 75u;
    w_u32(object + 0x100u, (uint32)((sint32)value >> 12));
    z = (uint32)r_s16(camera + 0x12Cu);
    w_u32(object + 0x40u, 0x800A3B44u);
    w_u32(object + 0x84u, 0xFFFFFFFCu);
    w_u32(object + 0xA8u, 0u);
    w_u32(object + 0xACu, 0u);
    w_u32(object + 0xB0u, 0u);
    w_u16(object + 0x24Cu, 360u);
    value = z * 75u;
    w_u32(object + 0x104u, (uint32)((sint32)value >> 12));
    w_u32(0x800F6F9Cu, object);
}

uint32 front_prepare_control_model_bundle(const char *name)
{
    uint32 row;
    sint32 column;
    uint32 models;
    uint32 result;
    FUNCTION_MARKER(0x800AE4E0, "FRONT.OVL");
    w_u32(0x80031454u, 0u);
    for (row = 0u; row < 64u; ++row) {
        for (column = 31; column >= 0; --column)
            w_u16(0x80057B50u + row * 64u + (uint32)column * 2u, 0u);
    }
    for (column = 0; column < 6; ++column)
        w_u16(0x80042946u + (uint32)column * 2u, 640u);
    for (column = 0; column < 10; ++column) {
        table_clear_column((uint32)column);
        table_clear_column((uint32)column + 16u);
    }
    for (column = 21; column <= 27; ++column)
        table_clear_column((uint32)column);
    front_allocate_model_tables();
    models = r_u32(0x80046178u);
    for (column = 54; column >= 0; --column)
        w_u32(models + (uint32)column * 132u, 0u);
    result = front_load_model_bundle(name);
    w_u32(0x800BC6D8u, result);
    return result;
}

void front_initialize_control_viewer(void)
{
    uint32 type;
    FUNCTION_MARKER(0x800A3CE0, "FRONT.OVL");
    front_configure_layer(0u, 0u, 0xA9u);
    front_configure_layer(1u, 3u, 0xA8u);
    if (r_u8(0x800BC810u) == 0u) {
        w_u32(0x8003119Cu, 320u);
        w_u32(0x800F6F98u, 8u);
        front_prepare_control_model_bundle("PADS.BND");
        front_create_control_viewer_objects();
        w_u32(0x800F6FD0u, 300u);
        w_u16(0x8011327Au, 400u);
        w_u32(0x800F6FCCu, 320u);
        w_u32(0x80113230u, 0u);
        w_u32(0x800BC6E4u, 1000u);
    }
    type = r_u8(0x8004EA29u);
    w_u8(0x800BC810u, 0u);
    w_u8(0x800F2FA9u, type == 0x53u || type == 0x73u ? 0u : 1u);
    w_u32(0x80113270u, 0x800F2818u);
    front_select_control_viewer_device();
}

void front_release_control_viewer(void)
{
    uint32 retained;
    FUNCTION_MARKER(0x800A3E08, "FRONT.OVL");
    retained = r_u8(0x800BC810u);
    if (retained == 0u) {
        object_update_all();
        object_initialize_pool();
        front_free_preview_models();
    }
}

uint32 front_update_control_viewer_objects(void)
{
    uint32 object;
    uint32 value;
    uint32 rotated;
    uint32 callback;
    FUNCTION_MARKER(0x800A3E3C, "FRONT.OVL");
    object = r_u32(0x8005367Cu);
    while (object != 0u) {
        value = r_u8(object + 0x26Cu) - 1u;
        w_u8(object + 0x26Cu, value);
        if ((value & 255u) == 0u) {
            rotated = 0u;
            callback = r_u32(object + 0x40u);
            psx_dispatch_guest_call(callback, 1u, object, 0u, 0u);
            w_u8(object + 0x26Cu, r_u8(object + 0x26Bu));
            if ((r_u32(object + 0x70u) & 0x100000u) != 0u) {
                if (r_u32(object + 0x20u) != 0u || r_u32(object + 0x24u) != 0u || r_u32(object + 0x28u) != 0u) {
                    front_apply_object_rotation(object);
                    rotated = 1u;
                    w_u32(object + 0x28u, 0u);
                    w_u32(object + 0x24u, 0u);
                    w_u32(object + 0x20u, 0u);
                }
                value = r_u16(object + 0x24Cu) - 1u;
                w_u16(object + 0x24Cu, value);
                if ((sint16)value <= 0 && rotated == 1u) {
                    matrix_normalize(object + 0x11Cu, 1u);
                    value = (uint32)random_between(30, 40);
                    w_u16(object + 0x24Cu, value);
                }
            } else if (r_u32(object + 0x20u) != r_u32(object + 0x2Cu) || r_u32(object + 0x24u) != r_u32(object + 0x30u) || r_u32(object + 0x28u) != r_u32(object + 0x34u)) {
                matrix_set_identity(object + 0x11Cu);
                front_apply_object_rotation(object);
            }
        }
        object = r_u32(object + 0x1D4u);
    }
    return object_flush_removed();
}

uint32 front_update_control_viewer_animation(void)
{
    sint32 state;
    uint32 object;
    uint32 value;
    uint32 row;
    uint32 column;
    uint32 element;
    sint32 delta;
    sint32 divisor;
    sint32 numerator;
    FUNCTION_MARKER(0x800A3418, "FRONT.OVL");
    state = r_s16(0x80113278u);
    if (state == 2) {
        object = r_u32(0x800F6F9Cu);
        value = r_u32(object + 0x20u);
        delta = (sint32)(0x1199u - value) / 10;
        w_u32(object + 0x20u, value + (uint32)delta);
        object = r_u32(0x800F6F9Cu);
        if (r_s32(object + 0x20u) >= 4096)
            w_u32(object + 0x20u, 4096u);
        value = r_u32(0x800BC6E4u);
        delta = (sint32)(0x55Fu - value) / 10;
        value += (uint32)delta;
        w_u32(0x800BC6E4u, value);
        if ((sint32)value >= 1250)
            w_u32(0x800BC6E4u, 1250u);
        object = r_u32(0x800F6F9Cu);
        if (r_u32(object + 0x20u) == 4096u && r_u32(0x800BC6E4u) == 1250u) {
            w_u16(0x80113278u, 3u);
            w_u32(0x800F2D50u, 15u);
        }
    } else if (state == 3) {
        value = r_u32(0x800F2D50u);
        w_u32(0x800F2D50u, value - 1u);
        if ((sint32)value <= 0)
            w_u16(0x80113278u, 4u);
    } else if (state == 4) {
        value = r_u32(0x800BC6E4u) - 16u;
        w_u32(0x800BC6E4u, value);
        if ((sint32)value < 1001) {
            w_u32(0x800BC6E4u, 1000u);
            w_u16(0x80113278u, 5u);
        }
    } else if (state == 5) {
        value = r_u16(0x8011327Au) - 26u;
        w_u16(0x8011327Au, value);
        if ((sint16)value <= 0) {
            w_u16(0x8011327Au, 0u);
            w_u16(0x80113278u, 0u);
        }
    }
    front_update_control_viewer_objects();
    object = r_u32(0x800F6F9Cu);
    if (r_u32(object + 0xA8u) == 0u) {
        matrix_set_identity(r_u32(0x800F6F9Cu) + 0x11Cu);
        front_apply_object_rotation(r_u32(0x800F6F9Cu));
        divisor = r_s32(0x800BD35Cu);
        for (row = 0u; row < 3u; ++row) {
            for (column = 0u; column < 3u; ++column) {
                element = r_u32(0x800F6F9Cu) + 0x11Cu + row * 6u + column * 2u;
                numerator = (sint32)((uint32)r_s16(element) << 12);
                if (divisor == 0)
                    abort();
                w_u16(element, (uint32)(numerator / divisor));
            }
        }
    }
    camera_dispatch();
    w_u16(0x8003506Eu, 0u);
    w_u16(0x8003506Cu, 0u);
    w_u16(0x8003506Au, 0u);
    w_u16(0x80035066u, 0u);
    w_u16(0x80035064u, 0u);
    w_u16(0x80035062u, 0u);
    w_u16(0x80035070u, 0x200u);
    w_u16(0x80035068u, 0x200u);
    w_u16(0x80035060u, 0x200u);
    w_u32(0x80035074u, 0u);
    w_u32(0x80035078u, 0u);
    w_u32(0x8003507Cu, 0u);
    return front_draw_viewer_objects();
}

void front_select_control_viewer_device(void)
{
    uint32 object;
    uint32 selected;
    uint32 active;
    uint32 type;
    FUNCTION_MARKER(0x800A31A8, "FRONT.OVL");
    if (r_u8(0x8004EA28u) != 0u) {
        object = r_u32(0x800F6F9Cu);
        w_u16(0x80113274u, 0xFFFFu);
        w_u16(0x80113278u, 1u);
        w_u32(0x80113230u, 0u);
        w_u32(object + 0xA8u, 0xFFFFFFFFu);
        w_u16(0x8011327Au, 400u);
        return;
    }
    selected = r_u32(0x80113230u);
    active = r_u32(0x800313C4u);
    if (selected == active)
        return;
    w_u32(0x80113230u, active);
    type = r_u8(0x8004EA29u);
    w_u16(0x80113278u, 2u);
    w_u32(0x800BC6E4u, 250u);
    w_u16(0x8011327Au, 400u);
    w_u16(0x80113274u, type);
    if (active != 0u)
        w_u16(0x80113276u, r_u16(r_u32(0x80113230u) + 0x2Eu));
    w_u32(r_u32(0x800F6F9Cu) + 0x20u, 0u);
    type = r_u8(0x8004EA29u);
    if (type == 0x41u) {
        w_u32(r_u32(0x800F6F9Cu) + 0xA8u, 1u);
        w_u32(r_u32(0x800F6F9Cu) + 0x24u, 0x500u);
        w_u32(r_u32(0x800F6F9Cu) + 0x10u, 0x1E37u);
        w_u32(r_u32(0x800F6F9Cu) + 0x0Cu, 0u);
        w_u32(0x80113264u, 0x800BC81Cu);
        w_u32(0x8011326Cu, 0x800BCB7Cu);
    } else if (type == 0x53u) {
        w_u32(r_u32(0x800F6F9Cu) + 0xA8u, 0u);
        w_u32(r_u32(0x800F6F9Cu) + 0x24u, 0x200u);
        w_u32(r_u32(0x800F6F9Cu) + 0x10u, 0x2AF8u);
        w_u32(r_u32(0x800F6F9Cu) + 0x0Cu, 20u);
        w_u32(0x80113264u, 0x800BCA5Cu);
        w_u32(0x8011326Cu, 0x800BD07Cu);
    } else if (type == 0x73u) {
        w_u32(r_u32(0x800F6F9Cu) + 0xA8u, 2u);
        w_u32(r_u32(0x800F6F9Cu) + 0x24u, 0x500u);
        w_u32(r_u32(0x800F6F9Cu) + 0x10u, 0x1E37u);
        w_u32(r_u32(0x800F6F9Cu) + 0x0Cu, 0u);
        w_u32(0x80113264u, 0x800BC93Cu);
        w_u32(0x8011326Cu, 0x800BCDFCu);
    } else {
        object = r_u32(0x800F6F9Cu);
        w_u32(0x80113264u, 0u);
        w_u32(0x8011326Cu, 0u);
        w_u32(object + 0xA8u, 0xFFFFFFFFu);
    }
}

uint32 front_copy_selected_card_file(void)
{
    FUNCTION_MARKER(0x800A27F0, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

void front_reset_card_action(void)
{
    uint32 first_changes;
    uint32 second_changes;
    FUNCTION_MARKER(0x800A1EE0, "FRONT.OVL");
    front_configure_layer(1u, 1u, 0xA8u);
    first_changes = r_u32(0x800F2D98u);
    second_changes = r_u32(0x800F2D9Cu);
    w_u32(0x8010D130u, 0u);
    w_u16(0x800F3114u, 0u);
    w_u32(0x8010D148u, first_changes);
    w_u32(0x8010D14Cu, second_changes);
}

void front_update_card_selection_for_copy(void)
{
    FUNCTION_MARKER(0x800A25D4, "FRONT.OVL");
    w_u32(0x8010D118u, 3u);
    front_update_card_selection();
    w_u32(0x8010D118u, 0u);
}

void front_update_card_selection_for_browse(void)
{
    FUNCTION_MARKER(0x800A260C, "FRONT.OVL");
    w_u32(0x8010D118u, 4u);
    front_update_card_selection();
    w_u32(0x8010D118u, 0u);
}

void front_commit_loaded_settings(void)
{
    FUNCTION_MARKER(0x800A2644, "FRONT.OVL");
    if (r_u32(0x8010D124u) != 0u) {
        front_restore_settings(0x8010D578u);
        front_snapshot_settings(0x80010258u);
    }
    w_u32(0x8010D124u, 0u);
}

sint32 front_erase_selected_card_file(void)
{
    FUNCTION_MARKER(0x800A296C, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

void front_update_card_delete_menu(void)
{
    uint32 state;
    uint32 port;
    uint32 slot;
    uint32 timer;
    uint32 labels;
    uint32 buttons;
    uint32 first_changes;
    uint32 second_changes;
    FUNCTION_MARKER(0x800A1F38, "FRONT.OVL");
    if (r_u32(0x8010D130u) != 0u) {
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F2D8Cu + port) == 2u || r_u32(0x8010D148u + port * 4u) != r_u32(0x800F2D98u + port * 4u)) {
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 0xFFFFFFFFu);
            w_u32(0x800F2D48u, 120u);
        }
    }
    second_changes = r_u32(0x800F2D9Cu);
    first_changes = r_u32(0x800F2D98u);
    w_u32(0x8010D14Cu, second_changes);
    state = r_u32(0x8010D130u);
    w_u32(0x8010D148u, first_changes);
    if (state == 0xFFFFFFFFu) {
        front_configure_layer(0u, 5u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        timer = r_u32(0x800F2D48u);
        w_u32(0x800FB870u, 0xFDu);
        timer -= 1u;
        w_u32(0x800F2D48u, timer);
        if (timer == 0u || (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            buttons = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, buttons & ~0x40u);
        } else {
            port = r_u32(0x8010D11Cu);
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + (port == 0u ? 0x40u : 0x44u)), 44u, 136u, 0x808080u);
        }
    }
    state = r_u32(0x8010D130u);
    if (state == 0u) {
        front_configure_layer(0u, 0u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        w_u32(0x8010D118u, 3u);
        front_update_card_selection();
        buttons = r_u32(0x80053684u);
        w_u32(0x8010D118u, 0u);
        if ((buttons & 0x40u) == 0u)
            return;
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F2D8Cu + port) != 3u)
            return;
        slot = r_u32(0x8010D120u);
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F7090u + (port << 13) + (slot << 9)) == 'E')
            return;
        sound_play_sample_pitch(11u, 128u);
        sound_play_sample_pitch(11u, 128u);
        w_u16(0x800F3118u, 0u);
        w_u32(0x8010D130u, 1u);
        w_u32(0x800F2D48u, 18u);
    } else if (state == 1u) {
        front_configure_layer(0u, 0u, 0xA7u);
        front_configure_layer(1u, 1u, 0xA8u);
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        front_draw_colored_text(r_u32(labels + 0x88u), 44u, 136u, 0x808080u);
        front_draw_card_entry(r_u32(0x8010D11Cu), r_u32(0x8010D120u), 52u, 166u);
        port = r_u32(0x8010D11Cu);
        w_u32(0x800FB870u, 0xFDu);
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + (port == 0u ? 0x90u : 0x94u)), 44u, 194u, 0x808080u);
        buttons = r_u32(0x80053684u);
        if ((buttons & 0x80u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            w_u32(0x8010D130u, 2u);
            w_u32(0x800F2D48u, 18u);
        } else if ((buttons & 0x10u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 0u);
        }
    } else if (state == 2u) {
        front_configure_layer(0u, 0xFFFFFFFFu, 0xA9u);
        front_configure_layer(1u, 0xFFFFFFFFu, 0xA8u);
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        front_draw_colored_text(r_u32(labels + 0x8Cu), 44u, 136u, 0x808080u);
        front_draw_card_entry(r_u32(0x8010D11Cu), r_u32(0x8010D120u), 52u, 166u);
        port = r_u32(0x8010D11Cu);
        w_u32(0x800FB870u, 0xFDu);
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + (port == 0u ? 0x98u : 0x9Cu)), 44u, 194u, 0x808080u);
        timer = r_u32(0x800F2D48u);
        if (timer != 0u) {
            w_u32(0x800F2D48u, timer - 1u);
            return;
        }
        front_refresh_card_status(r_u32(0x8010D11Cu));
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F2D8Cu + port) == 2u || r_u32(0x8010D148u + port * 4u) != r_u32(0x800F2D98u + port * 4u)) {
            first_changes = r_u32(0x800F2D98u);
            second_changes = r_u32(0x800F2D9Cu);
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 0xFFFFFFFFu);
            w_u32(0x800F2D48u, 120u);
            w_u32(0x8010D148u, first_changes);
            w_u32(0x8010D14Cu, second_changes);
        } else {
            front_erase_selected_card_file();
            w_u32(0x8010D130u, 3u);
            w_u8(0x800F2FA8u, 0u);
            w_u32(0x800F2D48u, 18u);
        }
        w_u32(0x800F2D48u, r_u32(0x800F2D48u) - 1u);
    } else if (state == 3u) {
        front_configure_layer(0u, 5u, 0xA9u);
        front_configure_layer(1u, 3u, 0xA8u);
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        front_draw_colored_text(r_u32(labels + 0xA0u), 44u, 136u, 0x808080u);
        timer = r_u32(0x800F2D48u);
        if (timer == 0u) {
            port = r_u32(0x8010D11Cu);
            w_u8(0x800F2D8Cu + port, 0u);
            port = r_u32(0x8010D11Cu);
            w_u8(0x800F2D8Au + port, 1u);
            w_u16(0x800F3118u, 1u);
            timer = r_u32(0x800F2D48u);
        }
        if ((sint32)timer <= 0 && (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            buttons = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, buttons & ~0x40u);
        }
        timer = r_u32(0x800F2D48u);
        if ((sint32)timer >= 0)
            w_u32(0x800F2D48u, timer - 1u);
    }
}

uint32 front_erase_selected_card_media(void)
{
    FUNCTION_MARKER(0x800A2A8C, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    w_u8(0x800F2D8C + r_u32(0x8010D11C), 2);
    return 2;
}

uint32 front_probe_selected_card_media(void)
{
    FUNCTION_MARKER(0x800A2AD0, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    w_u8(0x800F2D8C + r_u32(0x8010D11C), 2);
    return 2;
}

void front_draw_embossed_line(const sint16 *points, uint32 color, uint32 depth)
{
    uint32 packet;
    uint32 table;
    uint32 line;
    uint32 x_offset;
    uint32 y_offset;
    uint32 red;
    uint32 green;
    uint32 blue;
    FUNCTION_MARKER(0x800AE600, "FRONT.OVL");
    x_offset = points[1] == points[3] ? 0u : 1u;
    y_offset = points[1] == points[3] ? 1u : 0u;
    packet = r_u32(0x801132B8u);
    for (line = 0u; line < 3u; ++line) {
        w_u8(packet + 3u, 3u);
        w_u8(packet + 7u, 0x40u);
        red = (uint8)color;
        green = (uint8)(color >> 8);
        blue = (uint8)(color >> 16);
        if (line != 0u) {
            red >>= 1;
            green >>= 1;
            blue >>= 1;
        }
        w_u8(packet + 4u, red);
        w_u8(packet + 5u, green);
        w_u8(packet + 6u, blue);
        if (line == 0u) {
            w_u16(packet + 8u, (uint16)points[0]);
            w_u16(packet + 10u, (uint16)points[1]);
            w_u16(packet + 12u, (uint16)points[2]);
            w_u16(packet + 14u, (uint16)points[3]);
        } else if (line == 1u) {
            w_u16(packet + 8u, (uint16)points[0] - x_offset);
            w_u16(packet + 10u, (uint16)points[1] - y_offset);
            w_u16(packet + 12u, (uint16)points[2] - x_offset);
            w_u16(packet + 14u, (uint16)points[3] - y_offset);
        } else {
            w_u16(packet + 8u, (uint16)points[0] + x_offset);
            w_u16(packet + 10u, (uint16)points[1] + y_offset);
            w_u16(packet + 12u, (uint16)points[2] + x_offset);
            w_u16(packet + 14u, (uint16)points[3] + y_offset);
        }
        table = r_u32(0x801132BCu) + (depth << 2);
        AddPrim(psx_addr(table, 4u), psx_addr(packet, 16u));
        packet += 16u;
    }
    w_u32(0x801132B8u, packet);
}

void front_assign_control_button_labels(uint32 diagram, uint32 buttons, uint32 action)
{
    uint32 record = diagram;
    uint32 index = 0u;
    uint32 label;
    uint32 alternate;
    FUNCTION_MARKER(0x800A3FAC, "FRONT.OVL");
    buttons &= 0xFFFFu;
    if (buttons == 0u)
        return;
    alternate = (uint16)(action - 12u) < 2u;
    label = 0x800F27D4u + (uint32)((sint32)(sint16)action * 4);
    do {
        if ((buttons & 1u) != 0u) {
            if (index < 8u && alternate != 0u) {
                record += 320u;
                index += 16u;
            }
            w_u32(record + 12u, r_u32(label));
        }
        buttons = (buttons & 0xFFFFu) >> 1;
        record += 20u;
        ++index;
    } while (buttons != 0u);
}

void front_assign_control_axis_labels(uint32 diagram, uint32 mode, uint32 index)
{
    uint32 labels;
    uint32 record;
    FUNCTION_MARKER(0x800A4030, "FRONT.OVL");
    switch (mode) {
    case 1u:
        labels = 0x800F27DCu;
        break;
    case 2u:
        labels = 0x800F27D4u;
        break;
    case 3u:
        labels = 0x800F27E4u;
        break;
    case 4u:
        record = diagram + index * 20u;
        w_u32(record + 12u, r_u32(0x800F27F0u));
        w_u32(record + 32u, r_u32(0x800F27ECu));
        return;
    default:
        return;
    }
    record = diagram + index * 20u;
    w_u32(record + 12u, r_u32(labels));
    w_u32(record + 32u, r_u32(labels + 4u));
}

void front_prepare_control_diagram(uint32 diagram, uint32 bindings)
{
    sint32 index;
    uint32 mode;
    FUNCTION_MARKER(0x800A4120, "FRONT.OVL");
    for (index = 31; index >= 0; --index)
        w_u32(diagram + (uint32)index * 20u + 12u, 0u);
    for (index = 0; index < 15; ++index)
        front_assign_control_button_labels(diagram, r_u16(bindings + (uint32)index * 2u), (uint32)index);
    front_assign_control_button_labels(diagram, 0x100u, 15u);
    front_assign_control_button_labels(diagram, 0x800u, 16u);
    for (index = 0; index < 4; ++index) {
        mode = r_u8(bindings + 0x1Eu + (uint32)index * 4u);
        if (mode != 0u)
            front_assign_control_axis_labels(diagram, mode, 24u + (uint32)index * 2u);
    }
}

void front_draw_control_diagram(uint32 diagram, uint32 bindings)
{
    sint16 points[6];
    sint32 index;
    uint32 record;
    uint32 text;
    uint32 alignment;
    uint32 width;
    uint32 half_width;
    uint32 x = 0u;
    uint32 y = 0u;
    uint32 offset;
    FUNCTION_MARKER(0x800A2B0C, "FRONT.OVL");
    front_prepare_control_diagram(diagram, bindings);
    for (index = 31; index >= 0; --index) {
        record = diagram + (uint32)index * 20u;
        text = r_u32(record + 12u);
        if (text == 0u)
            continue;
        alignment = r_u8(record + 16u);
        if (alignment == 0u) {
            x = (uint32)r_s16(record + 8u);
            y = (uint32)r_s16(record + 10u) - 5u;
        } else if (alignment == 1u) {
            width = front_text_width(text);
            x = (uint32)r_s16(record + 8u) - width;
            y = (uint32)r_s16(record + 10u) - 5u;
        } else if (alignment < 6u) {
            width = front_text_width(text);
            half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
            if (alignment == 5u) {
                x = 320u - half_width;
                y = 171u;
            } else {
                x = (uint32)r_s16(record + 8u) - half_width;
                y = (uint32)r_s16(record + 10u);
                if (alignment == 2u)
                    y -= 5u;
                else if (alignment == 3u)
                    y += 5u;
                else
                    y -= 10u;
            }
        }
        points[1] = (sint16)r_u16(record + 2u);
        points[3] = (sint16)r_u16(record + 6u);
        points[5] = (sint16)r_u16(record + 10u);
        if (r_s16(record) >= 320) {
            offset = (uint32)r_s16(0x8011327Au);
            points[0] = (sint16)((uint32)r_s16(record) + offset);
            points[2] = (sint16)(r_u16(record + 4u) + offset);
            x += offset;
            points[4] = (sint16)(r_u16(record + 8u) + offset);
        } else {
            offset = (uint32)r_s16(0x8011327Au);
            points[0] = (sint16)((uint32)r_s16(record) - offset);
            points[2] = (sint16)(r_u16(record + 4u) - offset);
            x -= offset;
            points[4] = (sint16)(r_u16(record + 8u) - offset);
        }
        front_draw_embossed_line(points + 2, 0xFF8080u, 1u);
        front_draw_embossed_line(points, 0xFF8080u, 1u);
        front_draw_colored_text(r_u32(record + 12u), x, y, 0x808080u);
    }
}

void front_apply_control_preset(void)
{
    uint32 table;
    uint32 type;
    uint32 bindings;
    uint32 destination;
    uint32 preset;
    uint32 source;
    uint32 offset;
    uint32 value;
    FUNCTION_MARKER(0x800A2E28, "FRONT.OVL");
    if (r_u32(0x800313C4u) != r_u32(0x80113230u))
        return;
    if (r_u8(0x8004EA28u) != 0u)
        return;
    type = r_u8(0x8004EA29u);
    if (type == 0x41u)
        table = 0x800BC81Cu;
    else if (type == 0x53u)
        table = 0x800BCA5Cu;
    else if (type == 0x73u)
        table = 0x800BC93Cu;
    else
        return;
    bindings = r_u32(0x800313C4u);
    w_u16(bindings + 0x2Eu, r_u16(0x80113276u));
    for (offset = 0u; offset < 30u; offset += 2u) {
        preset = (uint32)r_s16(0x80113276u);
        value = r_u16(table + preset * 48u + offset);
        w_u16(bindings + offset, value);
    }
    for (offset = 0x1Eu; offset <= 0x2Au; offset += 4u) {
        preset = (uint32)r_s16(0x80113276u);
        source = table + preset * 48u + offset;
        destination = offset == 0x1Eu ? bindings : r_u32(0x800313C4u);
        value = r_u8(source);
        w_u8(destination + offset, value);
    }
}
