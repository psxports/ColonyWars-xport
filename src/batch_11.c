#include "game_functions.h"
#include <stdlib.h>
#include <string.h>

void front_update_ship_comparison(void)
{
    uint32 state;
    uint32 selected;
    uint32 value;
    uint32 index;
    uint32 entry;
    uint32 sectors;
    uint32 asset;
    FUNCTION_MARKER(0x8009456C, "FRONT.OVL");
    state = r_u32(0x800BC620);
    selected = r_u32(0x800F6FA0);
    if (state == 1)
    {
        value = r_u32(0x800F6FC0) + 11;
        w_u32(0x800F6FC0, value);
        if ((sint32)value > 160)
            w_u32(0x800F6FC0, 160);
        value = r_u32(0x800F6FF8) + 11;
        w_u32(0x800F6FF8, value);
        if ((sint32)value > 160)
            w_u32(0x800F6FF8, 160);
        value = r_u32(0x800F6FC0);
        if (value == 160 && r_u32(0x800F6FF8) == value)
        {
            w_u32(0x800BC620, 0);
            for (index = 0; index < 2; ++index)
            {
                value = r_u8(0x800BC4B4 + (selected << 1) + index);
                entry = r_u32(0x800F708C) + value * 24;
                sectors = r_u32(entry + 0x14);
                asset = r_u32(entry + 0x1C);
                stream_enqueue_segments(entry + 0x10, sectors, asset, 0x800118FE, 0,
                                        (const char *)psx_addr(0x8008690C, 1));
            }
        }
    }
    if (r_s16(0x800B737C) == 1 || r_u32(0x800BC620) - 5 < 2)
    {
        value = r_u32(0x800F6FC0) - 12;
        w_u32(0x800F6FC0, value);
        if ((sint32)value < 0)
            w_u32(0x800F6FC0, 0);
        value = r_u32(0x800F6FF8) - 12;
        w_u32(0x800F6FF8, value);
        if ((sint32)value < 0)
            w_u32(0x800F6FF8, 0);
    }
    state = r_u32(0x800BC620);
    if (state < 2 || state - 2 < 2 || state - 5 < 2)
    {
        front_update_viewer_objects();
        camera_dispatch();
        if (r_s32(0x800F6F98) > 0)
            front_draw_viewer_objects();
    }
    w_u32(0x800FB870, 83);
    front_draw_ship_statistics();
    front_draw_distance_scales();
    front_draw_viewer_backgrounds();
}

void front_free_preview_models(void)
{
    uint32 index = 0;
    uint32 offset = 0;
    uint32 record;
    uint32 address;
    FUNCTION_MARKER(0x80095A70, "FRONT.OVL");
    if (r_s32(0x800BC6DC) > 0)
    {
        do
        {
            record = r_u32(0x80046178) + offset;
            heap_free(r_u32(record) - 12);
            record = r_u32(0x80046178) + offset;
            address = r_u32(record + 4);
            if (address)
                heap_free(address);
            ++index;
            offset += 132;
        } while ((sint32)index < r_s32(0x800BC6DC));
    }
    address = r_u32(0x800BC61C);
    w_u32(0x800BC6DC, 0);
    resource_free_bundle(address);
    address = r_u32(0x80046178);
    w_u32(0x800BC61C, 0);
    heap_free(address);
    heap_free(r_u32(0x800BC6E0));
}

static uint32 front_ship_is_available(uint32 slot)
{
    uint32 index = r_u32(0x800F700C) + slot;
    sint32 stage = r_s16(0x8005960C);
    uint32 offset = index << 2;
    uint32 ship_stage = r_u8(0x800BC524 + offset);
    if ((sint32)ship_stage < stage)
        return 1;
    if ((sint32)ship_stage == stage && (sint32)r_u8(0x800BC525 + offset) < r_s16(0x8005960E))
        return 1;
    if (!ship_stage && !r_u8(0x800BC525 + offset))
        return 1;
    if ((sint32)ship_stage == stage && (sint32)r_u8(0x800BC525 + offset) == r_s16(0x8005960E) &&
        !r_u8(0x800BC527 + offset))
        return 1;
    if (r_u8(0x80011935) != 1)
        return 0;
    index = r_u32(0x800F700C) + slot;
    return r_u8(0x800BC524 + (index << 2)) < 20;
}

void front_select_ship_view(void)
{
    uint32 state;
    uint32 buttons;
    uint32 selected;
    uint32 model;
    uint32 object;
    uint32 bounds;
    uint32 bounds_offset;
    uint32 record;
    uint32 numerator;
    uint32 denominator;
    uint32 quotient;
    uint32 target;
    uint32 difference;
    uint32 step;
    uint32 magnitude;
    uint32 value;
    FUNCTION_MARKER(0x800947D4, "FRONT.OVL");
    state = r_u32(0x800BC620);
    if (state < 2 || state - 2 < 2)
    {
        buttons = r_u32(0x80053684);
        if (buttons & 0x8000)
        {
            w_u32(0x800BC624, 10);
            w_u32(0x800BC620, 5);
            stream_clear_queue();
        }
        else if ((buttons & 0x2000) || r_s32(0x8010D0D8) >= 13)
        {
            w_u32(0x800BC624, 10);
            w_u32(0x800BC620, 6);
            stream_clear_queue();
        }
    }
    state = r_u32(0x800BC620);
    if (state - 7 < 2)
    {
        if (r_s32(0x800F2D20) >= 10)
        {
            if (front_ship_is_available(0) && r_u32(0x800F2D20) != 11)
            {
                SsSetSerialVol(0, 0, 0);
                front_free_preview_models();
                selected = r_u32(0x800F700C);
                w_u32(0x8010D0D8, selected);
                w_u32(0x800F6FA0, selected);
                front_load_preview_models(0);
                value = r_u32(0x800BC6D8);
                w_u32(0x800F6FDC, 0xFFFFFFFE);
                w_u32(0x800F6FA4, value);
                front_init_model_preview();
                disc_wait_stream();
                VSync(0);
                VSync(0);
                selected = r_u32(0x800F6FA0);
                object = r_u32(0x800F6F9C);
                model = r_u32(0x800F6FA4);
                value = r_u8(0x800BC526 + (selected << 2));
                w_u32(object + 0xA8, value);
                bounds_offset = model * 48;
                bounds = r_u32(0x800BC6E0) + bounds_offset;
                numerator = r_u32(bounds + 12) * 250;
                denominator = r_u32(bounds + 8) << 2;
                if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
                    abort();
                quotient = (uint32)((sint32)numerator / (sint32)denominator);
                record = r_u32(0x800BC6D0) + selected * 1148;
                denominator = r_u32(record + 0x40);
                numerator = quotient << 13;
                if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
                    abort();
                target = (uint32)((sint32)numerator / (sint32)denominator);
                difference = target - r_u32(0x800F6FAC);
                w_u32(0x800F6FA8, 0);
                w_u32(0x800F6FB0, target);
                step = (uint32)((sint32)difference / 60);
                w_u32(0x800F6FB4, step);
                magnitude = (sint32)step < 0 ? 0 - step : step;
                if ((sint32)magnitude > 0)
                    sound_play_sample(8);
                if (r_s16(0x800B737C) != 1)
                {
                    bounds = r_u32(0x800BC6E0) + bounds_offset;
                    value = (uint32)r_s16(bounds + 0x16);
                    w_u32(0x800F6F98, 8);
                    w_u32(0x800F6FA8, value);
                }
                w_u32(0x800BC624, 9);
                w_u16(0x800FB828, 9);
                value = r_u32(0x800F6FCC);
                w_u32(0x800BC620, 1);
                w_u8(0x800FB942, 0);
                w_u8(0x800FB941, 0);
                w_u8(0x800FB940, 0);
                w_u32(0x800F7010, 0);
                w_u32(0x800F6FC0, 0);
                w_u16(0x800FB838, 0xFFFF);
                w_u16(0x800FB848, 0xFFFF);
                w_u32(0x800F6FC4, value + 60);
            }
            else
            {
                value = r_u32(0x800F700C) + r_u32(0x800F7010);
                w_u32(0x800F700C, value);
                if ((sint32)value >= 13)
                    w_u32(0x800F700C, 0);
                value = r_u32(0x800F700C);
                if ((sint32)value < 0)
                    w_u32(0x800F700C, 12);
            }
            w_u32(0x800F2D20, 0);
        }
        value = r_u32(0x800F2D20) + 1;
        w_u32(0x800F2D20, value);
    }
    else if (r_s16(0x800B737C) == 1 || state - 5 < 2)
    {
        value = r_u32(0x800F6F98) - 1;
        w_u32(0x800F6F98, value);
        if (value == 0xFFFFFFF6)
        {
            if (state == 5)
            {
                w_u32(0x800BC620, 7);
                w_u32(0x800F7010, 0xFFFFFFFF);
            }
            if (r_u32(0x800BC620) == 6)
            {
                w_u32(0x800BC620, 8);
                w_u32(0x800F7010, 1);
            }
            w_u32(0x800F2D20, 11);
        }
    }
    else
    {
        value = r_u32(0x800F6F98);
        if ((sint32)value < 8)
            w_u32(0x800F6F98, value + 1);
    }
    if (r_s16(0x800B737C) == 1)
        w_u8(0x800BC628, 0);
}

void front_select_ship_comparison(void)
{
    uint32 state;
    uint32 buttons;
    uint32 selected;
    uint32 loaded = 0;
    uint32 value;
    uint32 slot;
    uint32 offset;
    uint32 model;
    uint32 object;
    uint32 bounds;
    uint32 bounds_offset;
    uint32 record;
    uint32 numerator;
    uint32 denominator;
    uint32 quotient;
    uint32 target;
    uint32 difference;
    uint32 step;
    uint32 magnitude;
    uint32 count;
    FUNCTION_MARKER(0x80094D94, "FRONT.OVL");
    state = r_u32(0x800BC620);
    if (state < 2 || state - 2 < 2)
    {
        buttons = r_u32(0x80053684);
        if (buttons & 0x8000)
        {
            w_u32(0x800BC624, 10);
            w_u32(0x800BC620, 5);
            stream_clear_queue();
        }
        else if ((buttons & 0x2000) || r_s32(0x8010D0D8) < 13)
        {
            w_u32(0x800BC624, 10);
            w_u32(0x800BC620, 6);
            stream_clear_queue();
        }
    }
    state = r_u32(0x800BC620);
    if (state - 7 < 2)
    {
        if (r_s32(0x800F2D24) >= 10)
        {
            w_u16(0x800FB828, 0xFFFF);
            w_u16(0x800FB838, 0xFFFF);
            w_u16(0x800FB848, 0xFFFF);
            if (front_ship_is_available(0) && r_u32(0x800F2D24) != 11)
            {
                selected = r_u32(0x800F700C);
                w_u32(0x8010D0D8, selected);
                w_u32(0x800F6FA0, selected);
                w_u32(0x800F6FD8, selected + 1);
                loaded = 1;
                front_free_preview_models();
                front_load_preview_models(0);
                value = r_u32(0x800BC6D8);
                w_u16(0x800FB838, 9);
                w_u32(0x800F6FA4, value);
            }
            if (front_ship_is_available(1) && r_u32(0x800F2D24) != 11)
            {
                selected = r_u32(0x800F700C);
                w_u32(0x8010D0D8, selected);
                w_u32(0x800F6FD8, selected + 1);
                if (!loaded)
                {
                    w_u32(0x800F6FA0, selected);
                    w_u32(0x800F6FA4, 0xFFFFFFFF);
                    front_free_preview_models();
                    front_load_preview_models(1);
                    value = r_u32(0x800BC6D8);
                }
                else
                    value = front_load_slot_bundle(1);
                w_u32(0x800F6FDC, value);
                w_u16(0x800FB848, 9);
                loaded = 1;
            }
            if (!loaded)
            {
                value = r_u32(0x800F700C);
                if ((sint32)value < 13)
                    value = 13;
                else
                    value += r_u32(0x800F7010) << 1;
                w_u32(0x800F700C, value);
                count = r_u32(0x800BC6C8);
                value = r_u32(0x800F700C);
                if ((sint32)value >= (sint32)(count - 1))
                    w_u32(0x800F700C, 13);
                value = r_u32(0x800F700C);
                if ((sint32)value < 13)
                    w_u32(0x800F700C, count - 2);
            }
            else
            {
                front_init_model_preview();
                disc_wait_stream();
                VSync(0);
                VSync(0);
                for (slot = 0; slot < 2; ++slot)
                {
                    offset = slot * 56;
                    model = r_u32(0x800F6FA4 + offset);
                    if ((sint32)model >= 0)
                    {
                        selected = r_u32(0x800F6FA0 + offset);
                        object = r_u32(0x800F6F9C + offset);
                        value = r_u8(0x800BC526 + (selected << 2));
                        w_u32(object + 0xA8, value);
                        bounds_offset = model * 48;
                        bounds = r_u32(0x800BC6E0) + bounds_offset;
                        numerator = r_u32(bounds + 12) * 250;
                        denominator = r_u32(bounds + 8) << 2;
                        if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
                            abort();
                        quotient = (uint32)((sint32)numerator / (sint32)denominator);
                        record = r_u32(0x800BC6D0) + selected * 1148;
                        denominator = r_u32(record + 0x40);
                        numerator = quotient << 13;
                        if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
                            abort();
                        target = (uint32)((sint32)numerator / (sint32)denominator);
                        difference = target - r_u32(0x800F6FAC + offset);
                        w_u32(0x800F6FA8 + offset, 0);
                        w_u32(0x800F6FB0 + offset, target);
                        step = (uint32)((sint32)difference / 60);
                        w_u32(0x800F6FB4 + offset, step);
                        w_u8(0x800F6FCA + offset, 255);
                        w_u8(0x800F6FC9 + offset, 255);
                        value = r_u32(0x800F6FB4 + offset);
                        w_u8(0x800F6FC8 + offset, 255);
                        magnitude = (sint32)value < 0 ? 0 - value : value;
                        if ((sint32)magnitude > 0)
                            sound_play_sample(8);
                        if (r_s16(0x800B737C) != 1)
                        {
                            bounds = r_u32(0x800BC6E0) + bounds_offset;
                            value = (uint32)r_s16(bounds + 0x16);
                            w_u32(0x800F6FA8 + offset, value);
                            w_u32(0x800F6F98, 0);
                        }
                        selected = r_u32(0x8010D0D8) + slot;
                        record = r_u32(0x800BC6D0) + selected * 1148;
                        bounds = r_u32(0x800BC6E0) + bounds_offset;
                        value = (uint32)r_s16(bounds + 6);
                        w_u32(record + 0x30, value);
                    }
                    w_u32(0x800F6FC0 + offset, 0);
                    w_u32(0x800F6FC4 + offset, 160);
                }
                w_u32(0x800F6FCC, 100);
                w_u32(0x800F6FD0, 185);
                w_u32(0x800F7004, 540);
                w_u32(0x800F7008, 310);
                w_u32(0x800BC620, 1);
                w_u32(0x800BC624, 9);
                w_u8(0x800FB942, 0);
                w_u8(0x800FB941, 0);
                w_u8(0x800FB940, 0);
                w_u32(0x800F7010, 0);
            }
            w_u32(0x800F2D24, 0);
        }
        value = r_u32(0x800F2D24) + 1;
        w_u32(0x800F2D24, value);
    }
    else if (r_s16(0x800B737C) == 1 || state - 5 < 2)
    {
        value = r_u32(0x800F6F98) - 1;
        w_u32(0x800F6F98, value);
        if (value == 0xFFFFFFF6)
        {
            if (state == 5)
            {
                w_u32(0x800BC620, 7);
                w_u32(0x800F7010, 0xFFFFFFFF);
            }
            if (r_u32(0x800BC620) == 6)
            {
                w_u32(0x800BC620, 8);
                w_u32(0x800F7010, 1);
            }
            w_u32(0x800F2D24, 11);
        }
    }
    else
    {
        value = r_u32(0x800F6F98);
        if ((sint32)value < 8)
            w_u32(0x800F6F98, value + 1);
    }
    if (r_s16(0x800B737C) == 1)
        w_u8(0x800BC628, 0);
}

uint32 front_request_next_ship(void)
{
    FUNCTION_MARKER(0x80095B48, "FRONT.OVL");
    w_u32(0x800F7010, 1);
    w_u8(0x800BC628, 1);
    return 1;
}

uint32 front_request_previous_ship(void)
{
    FUNCTION_MARKER(0x80095B68, "FRONT.OVL");
    w_u32(0x800F7010, 0xFFFFFFFF);
    w_u8(0x800BC628, 1);
    return 1;
}

void front_update_ship_screen(void)
{
    uint32 state;
    uint32 slot;
    uint32 component;
    uint32 offset;
    uint32 value;
    uint32 step;
    uint32 target;
    FUNCTION_MARKER(0x8009565C, "FRONT.OVL");
    if (r_s16(0x800B737C))
        w_u32(0x800BC624, 11);
    else
    {
        state = r_u32(0x800BC624);
        if (state == 9)
        {
            for (slot = 0; slot < 3; ++slot)
            {
                offset = slot << 4;
                if (r_s16(0x800FB828 + offset) == -1)
                    continue;
                for (component = 0; component < 3; ++component)
                {
                    value = r_u8(0x800FB830 + offset + component);
                    step = r_u8(0x800FB82D + offset + component);
                    target = r_u8(0x800FB82A + offset + component);
                    value += step;
                    w_u8(0x800FB830 + offset + component, value > target ? target : value);
                }
            }
            if (r_u16(0x800FB830) == r_u16(0x800FB82A) &&
                r_u8(0x800FB832) == r_u8(0x800FB82C))
                w_u32(0x800BC624, 11);
        }
        else if (state == 10)
        {
            for (slot = 0; slot < 3; ++slot)
            {
                offset = slot << 4;
                if (r_s16(0x800FB828 + offset) == -1)
                    continue;
                for (component = 0; component < 3; ++component)
                {
                    value = r_u8(0x800FB830 + offset + component);
                    step = r_u8(0x800FB82D + offset + component);
                    value -= step;
                    w_u8(0x800FB830 + offset + component, (sint32)value < 0 ? 0 : value);
                }
            }
        }
    }
    if (r_s32(0x800F700C) < 13)
        front_update_ship_view();
    else
        front_update_ship_comparison();
    if (r_s16(0x8010D0DC) == 31)
        front_select_ship_view();
    else
        front_select_ship_comparison();
    front_draw_viewer_controls();
    state = r_u32(0x800BC620);
    front_draw_previous_ship_arrow(90, 364, state == 7 || state == 5);
    state = r_u32(0x800BC620);
    front_draw_next_ship_arrow(158, 364, state == 8 || state == 6);
}

void front_free_cached_text(void)
{
    uint32 first;
    FUNCTION_MARKER(0x800B6588, "FRONT.OVL");
    first = r_u32(0x8010D0F8);
    w_u32(0x8010D110, 0);
    w_u32(0x8010D108, 0);
    w_u32(0x8010D10C, 0);
    w_u32(0x8010D104, 0);
    heap_free(first);
    heap_free(r_u32(0x8010D0FC));
    heap_free(r_u32(0x8010D114));
}

uint32 front_close_ship_screen(void)
{
    FUNCTION_MARKER(0x80095B88, "FRONT.OVL");
    front_free_cached_text();
    w_u16(0x800F3116, 1);
    w_u8(0x800BC628, 0);
    object_update_all();
    object_initialize_pool();
    front_free_preview_models();
    heap_free(r_u32(0x800BC6D0));
    w_u32(0x800BC6D0, 0);
    front_configure_background_light(0, 320, 240, 0, 9);
    front_configure_background_light(1, 40, 40, 0x105010, 9);
    front_configure_background_light(2, 600, 440, 0x501010, 9);
    return stream_clear_queue();
}

void front_draw_viewer_controls(void)
{
    uint32 index;
    uint32 angle;
    uint32 state;
    uint32 speed;
    uint32 phase;
    uint32 history[4];
    FUNCTION_MARKER(0x800ADD68, "FRONT.OVL");
    for (index = 0; index < 4; ++index)
    {
        angle = r_u32(0x800FB7B8 + index * 4);
        front_draw_control_segment(50, 380, angle, 1);
        angle = r_u32(0x800FB7B8 + index * 4);
        front_draw_control_segment(50, 380, angle + 2048, 1);
    }
    angle = r_u32(0x800FB7C8);
    front_draw_control_segment(50, 380, angle, 0);
    angle = r_u32(0x800FB7C8);
    front_draw_control_segment(50, 380, angle + 2048, 0);
    state = r_u32(0x80040FE4);
    if (state == 6 || state == 2)
    {
        speed = r_u32(0x800FB7B4);
        if ((sint32)speed < 70)
            w_u32(0x800FB7B4, speed + 1);
    }
    else
    {
        speed = r_u32(0x800FB7B4);
        if ((sint32)speed > 0)
            w_u32(0x800FB7B4, speed - 1);
    }
    phase = r_u32(0x800FB7B0);
    speed = r_u32(0x800FB7B4);
    for (index = 0; index < 4; ++index)
        history[index] = r_u32(0x800FB7BC + index * 4);
    phase += speed;
    w_u32(0x800FB7B0, phase);
    for (index = 0; index < 4; ++index)
        w_u32(0x800FB7B8 + index * 4, history[index]);
    w_u32(0x800FB7C8, phase);
}

void front_draw_control_segment(uint32 x, uint32 y, uint32 angle, uint32 trail)
{
    uint32 packet;
    uint32 width;
    uint32 height;
    uint32 depth;
    uint32 cosine;
    uint32 sine;
    uint32 px[4];
    uint32 py[4];
    uint32 vertex_x[4];
    uint32 vertex_y[4];
    uint32 index;
    uint32 sum;
    uint32 difference;
    uint32 shift;
    uint32 value;
    uint32 table;
    uint16 page;
    uint16 clut;
    FUNCTION_MARKER(0x800ADEC8, "FRONT.OVL");
    packet = r_u32(0x801132B8);
    width = (uint32)r_s16(0x8005096E);
    height = (uint32)r_s16(0x8005096A);
    depth = (uint32)r_s16(0x8005096C);
    px[0] = 0;
    px[1] = 0 - width;
    px[2] = 0;
    px[3] = 0 - width;
    py[0] = 0xFFFFFFE0;
    py[1] = 0xFFFFFFE0;
    py[2] = height - 32;
    py[3] = height - 32;
    shift = (2 - depth) & 31;
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 0x2C);
    w_u8(packet + 4, trail ? 8 : 24);
    w_u8(packet + 5, trail ? 16 : 48);
    w_u8(packet + 6, trail ? 16 : 48);
    cosine = (uint32)rcos((sint32)angle);
    sine = (uint32)rsin((sint32)angle);
    for (index = 0; index < 4; ++index)
    {
        sum = py[index] * cosine + px[index] * sine;
        difference = px[index] * cosine - py[index] * sine;
        vertex_x[index] = x - (uint32)((sint32)sum >> 12);
        vertex_y[index] = y + (uint32)((sint32)difference >> 12);
    }
    w_u16(packet + 8, vertex_x[0]);
    w_u16(packet + 10, vertex_y[0]);
    w_u16(packet + 16, vertex_x[1]);
    w_u16(packet + 24, vertex_x[2]);
    w_u16(packet + 26, vertex_y[2]);
    w_u16(packet + 32, vertex_x[3]);
    w_u16(packet + 34, vertex_y[3]);
    w_u16(packet + 18, vertex_y[1]);
    page = GetTPage((sint32)depth, 1, r_s16(0x80050960), r_s16(0x80050962));
    w_u16(packet + 22, page);
    clut = GetClut(r_s16(0x80050964), r_s16(0x80050966));
    value = r_u8(packet + 7);
    w_u16(packet + 14, clut);
    w_u8(packet + 7, value | 2);
    value = r_u16(0x80050960);
    w_u8(packet + 12, (value & 63) << shift);
    w_u8(packet + 13, r_u16(0x80050962));
    value = r_u8(packet + 12);
    width = (uint32)r_s16(0x80050968);
    w_u8(packet + 28, value);
    value = r_u8(packet + 12);
    value += width << shift;
    height = r_u8(packet + 13);
    w_u8(packet + 20, value - 1);
    value = r_u8(packet + 13);
    w_u8(packet + 21, height);
    height = r_u8(0x8005096A);
    w_u8(packet + 29, value + height - 1);
    value = r_u8(packet + 20);
    height = r_u8(packet + 29);
    w_u8(packet + 36, value);
    w_u8(packet + 37, height);
    table = r_u32(0x801132BC) + 4;
    AddPrim(psx_addr(table, 4), psx_addr(packet, 40));
    w_u32(0x801132B8, packet + 40);
}

static void front_draw_ship_arrow(uint32 x, uint32 y, uint32 active, uint32 phase_address, uint32 reverse)
{
    uint32 phase;
    sint32 height;
    sint32 period;
    uint32 packet;
    uint32 depth;
    uint32 width;
    uint32 shift;
    uint32 strip;
    uint32 bytes;
    uint32 edge_x;
    uint32 edge_y;
    uint32 saved_x;
    uint32 saved_y;
    uint32 value;
    uint32 texture_width;
    uint32 red;
    uint32 cyan;
    uint32 table;
    uint32 x1;
    uint32 y1;
    uint32 x2;
    uint32 y2;
    uint32 x3;
    uint32 y3;
    uint32 u1;
    uint32 v1;
    uint32 u2;
    uint32 v2;
    uint32 u3;
    uint32 v3;
    uint16 page;
    uint16 clut;
    height = r_s16(0x8005099E);
    phase = r_u32(phase_address) + active;
    period = (sint16)(height / 3);
    if (!period || (phase == 0x80000000 && period == -1))
        abort();
    phase = (uint32)((sint32)phase % period);
    packet = r_u32(0x801132B8);
    depth = (uint32)r_s16(0x8005099C);
    width = (uint32)(height >> 2);
    w_u32(phase_address, phase);
    shift = (2 - depth) & 31;
    red = active == 1 ? 64 : 32;
    cyan = active == 1 ? 128 : 64;
    for (strip = 0; strip < 3; ++strip)
    {
        if (strip < 2)
        {
            bytes = 52;
            SetPolyGT4((POLY_GT4 *)psx_addr(packet, 52));
            if (!strip)
            {
                w_u8(packet + 4, 0);
                w_u8(packet + 5, 0);
                w_u8(packet + 6, 0);
                w_u8(packet + 28, 0);
                w_u8(packet + 29, 0);
                w_u8(packet + 30, 0);
                w_u8(packet + 16, red);
                w_u8(packet + 17, cyan);
                w_u8(packet + 18, cyan);
                w_u8(packet + 40, red);
                w_u8(packet + 41, cyan);
                w_u8(packet + 42, cyan);
            }
            else
            {
                w_u8(packet + 16, 0);
                w_u8(packet + 17, 0);
                w_u8(packet + 18, 0);
                w_u8(packet + 40, 0);
                w_u8(packet + 41, 0);
                w_u8(packet + 42, 0);
                w_u8(packet + 4, red);
                w_u8(packet + 5, cyan);
                w_u8(packet + 6, cyan);
                w_u8(packet + 28, red);
                w_u8(packet + 29, cyan);
                w_u8(packet + 30, cyan);
            }
            x1 = 20; y1 = 22; x2 = 32; y2 = 34; x3 = 44; y3 = 46;
            u1 = 24; v1 = 25; u2 = 36; v2 = 37; u3 = 48; v3 = 49;
            edge_x = strip ? x + width + width : x;
        }
        else
        {
            bytes = 40;
            SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
            w_u8(packet + 4, red);
            w_u8(packet + 5, cyan);
            w_u8(packet + 6, cyan);
            x1 = 16; y1 = 18; x2 = 24; y2 = 26; x3 = 32; y3 = 34;
            u1 = 20; v1 = 21; u2 = 28; v2 = 29; u3 = 36; v3 = 37;
            edge_x = x + width;
        }
        w_u16(packet + 8, edge_x);
        edge_x = r_u16(packet + 8);
        w_u16(packet + 10, y);
        saved_y = r_u16(packet + 10);
        saved_x = r_u16(packet + 8);
        edge_x += width;
        if (strip == 1)
            --edge_x;
        w_u16(packet + x1, edge_x);
        edge_y = r_u16(packet + 10);
        w_u16(packet + y1, saved_y);
        w_u16(packet + x2, saved_x);
        value = r_u16(0x8005099A);
        edge_y += value - 1;
        edge_x = r_u16(packet + x1);
        w_u16(packet + y2, edge_y);
        edge_y = r_u16(packet + y2);
        w_u16(packet + x3, edge_x);
        w_u16(packet + y3, edge_y);
        page = GetTPage((sint32)depth, 1, r_s16(0x80050990), r_s16(0x80050992));
        w_u16(packet + (strip < 2 ? 26 : 22), page);
        clut = GetClut(r_s16(0x80050994), r_s16(0x80050996));
        w_u16(packet + 14, clut);
        if ((!reverse && strip == 1) || (reverse && strip == 0))
        {
            texture_width = (uint32)(r_s16(0x8005099E) / 3);
            value = r_u16(0x80050990);
            phase = r_u32(phase_address);
            value = ((value & 63) << shift) + phase + width + width - texture_width;
        }
        else
        {
            value = r_u16(0x80050990);
            phase = r_u32(phase_address);
            value = ((value & 63) << shift) + phase;
            if (strip == 2)
                value += width - (reverse ? 1 : 0);
        }
        if (!reverse)
        {
            w_u8(packet + 12, value);
            value = r_u16(0x80050992);
            saved_x = r_u8(packet + 12);
            w_u8(packet + 13, value);
            value = r_u8(packet + 12);
            saved_y = r_u8(packet + 13);
            w_u8(packet + u2, saved_x);
            value += width - (strip == 1 ? 1 : 0);
            w_u8(packet + u1, value);
            w_u8(packet + v1, saved_y);
        }
        else
        {
            w_u8(packet + u1, value);
            if (strip == 1)
            {
                value = r_u16(0x80050992);
                w_u8(packet + 13, value);
                value = r_u8(packet + u1);
                value += width - 1;
            }
            else
            {
                value = r_u8(packet + u1);
                saved_y = r_u16(0x80050992);
                value += width + (strip == 2 ? 1 : 0);
                w_u8(packet + 13, saved_y);
            }
            w_u8(packet + 12, value);
            saved_y = r_u8(packet + 13);
            saved_x = r_u8(packet + 12);
            w_u8(packet + v1, saved_y);
            w_u8(packet + u2, saved_x);
        }
        height = r_u8(0x8005099A);
        value = r_u8(packet + u1);
        w_u8(packet + u3, value);
        value = r_u8(packet + 13);
        w_u8(packet + v2, value + (uint32)height - 1);
        value = r_u8(packet + 7);
        saved_y = r_u8(packet + v2);
        w_u8(packet + 7, value | 2);
        w_u8(packet + v3, saved_y);
        table = r_u32(0x801132BC);
        AddPrim(psx_addr(table, 4), psx_addr(packet, bytes));
        packet += bytes;
        w_u32(0x801132B8, packet);
    }
}

void front_draw_previous_ship_arrow(uint32 x, uint32 y, uint32 active)
{
    FUNCTION_MARKER(0x80095D90, "FRONT.OVL");
    front_draw_ship_arrow(x, y, active, 0x800F2D2C, 0);
}

void front_draw_next_ship_arrow(uint32 x, uint32 y, uint32 active)
{
    FUNCTION_MARKER(0x800962AC, "FRONT.OVL");
    front_draw_ship_arrow(x, y, active, 0x800F2D30, 1);
}

uint32 front_wide_text_width(uint32 text)
{
    uint32 width = 0;
    uint32 bank;
    sint32 character;
    sint32 glyph;
    FUNCTION_MARKER(0x80096B84, "FRONT.OVL");
    character = r_s16(text);
    while (character)
    {
        glyph = front_wide_character_glyph((uint32)character);
        if (glyph >= 0)
        {
            bank = r_u32(0x800FB870);
            width += (uint32)r_s16(0x8004EB7E + (((uint32)glyph + bank) << 4));
        }
        else
            width += glyph == -1 ? 4 : 16;
        text += 2;
        character = r_s16(text);
    }
    return width;
}

void front_draw_single_page_text_native(const char *text, uint32 x, uint32 y, uint32 color)
{
    uint32 character;
    uint32 glyph;
    uint32 atlas;
    uint32 page = 0;
    uint32 cached_page = 0;
    uint32 packet;
    uint32 table;
    uint32 shift;
    uint32 atlas_x;
    uint32 atlas_y;
    uint32 value;
    uint32 opcode;
    sint32 depth;
    sint32 graph_type;
    uint16 clut;
    FUNCTION_MARKER(0x80097440, "FRONT.OVL");
    character = (uint8)*text;
    if (character & 128)
    {
        front_draw_wide_text_native(text, x, y, color);
        return;
    }
    if (character)
    {
        character = (uint8)*text;
        do
        {
            glyph = r_u32(0x8010D168 + (character << 2));
            if ((sint32)glyph < 0)
                x += 4;
            else
            {
                value = r_u32(0x800FB870);
                atlas = 0x8004EB70 + ((glyph + value) << 4);
                depth = r_s16(atlas + 12);
                shift = (2 - (uint32)depth) & 31;
                if (!(uint16)cached_page)
                {
                    graph_type = GetGraphType();
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
                    cached_page = page;
                }
                packet = r_u32(0x801132B8);
                w_u8(packet + 3, 4);
                w_u8(packet + 7, 0x64);
                w_u16(packet + 8, x);
                w_u16(packet + 10, y);
                value = r_u16(atlas);
                w_u8(packet + 12, (value & 63) << shift);
                w_u8(packet + 13, r_u16(atlas + 2));
                w_u16(packet + 16, r_u16(atlas + 14));
                w_u16(packet + 18, r_u16(atlas + 10));
                clut = GetClut(r_s16(atlas + 4), r_s16(atlas + 6));
                w_u16(packet + 14, clut);
                opcode = r_u8(packet + 7);
                w_u8(packet + 4, color);
                w_u8(packet + 5, color >> 8);
                w_u8(packet + 7, opcode | 2);
                w_u8(packet + 6, color >> 16);
                table = r_u32(0x801132BC);
                AddPrim(psx_addr(table, 4), psx_addr(packet, 20));
                value = (uint32)r_s16(atlas + 14);
                w_u32(0x801132B8, packet + 20);
                x += value;
            }
            character = (uint8)*++text;
        } while (character);
    }
    packet = r_u32(0x801132B8);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)page, NULL);
    table = r_u32(0x801132BC);
    AddPrim(psx_addr(table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

static uint32 front_text_quotient(uint32 numerator, uint32 denominator)
{
    if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
        abort();
    return (uint32)((sint32)numerator / (sint32)denominator);
}

static uint32 front_text_remainder(uint32 numerator, uint32 denominator)
{
    if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
        abort();
    return (uint32)((sint32)numerator % (sint32)denominator);
}

static void front_draw_scrolling_text_panel(uint32 panel, uint32 right_aligned, uint32 reverse)
{
    uint32 count;
    uint32 spacing;
    uint32 scroll;
    uint32 numerator;
    uint32 denominator;
    uint32 index;
    uint32 row;
    uint32 trim;
    uint32 x;
    uint32 y;
    uint32 color;
    uint32 bottom;
    uint32 next_y;
    uint32 height;
    uint32 lines;
    uint32 text;
    uint32 width;
    count = r_u32(panel + 28);
    if (!count)
        return;
    w_u32(0x800FB870, r_u32(panel + 16));
    if (!panel)
        return;
    if (!reverse || (r_u32(0x800F30F4) & 1))
    {
        lines = r_u32(panel + 40);
        spacing = r_u32(panel + 32);
        scroll = r_u32(panel + 36);
        if (lines * spacing != scroll)
        {
            count = r_u32(panel + 28);
            denominator = count * spacing;
            w_u32(panel + 36, front_text_remainder(scroll + 1, denominator));
        }
    }
    if (!reverse)
    {
        scroll = r_u32(panel + 36);
        spacing = r_u32(panel + 32);
        row = front_text_quotient(scroll, spacing);
        trim = front_text_remainder(scroll, spacing);
        count = r_u32(panel + 28);
        index = front_text_remainder(row, count);
        x = r_u32(panel);
        color = r_u32(panel + 20);
        y = r_u32(panel + 4);
        if (right_aligned)
        {
            width = r_u32(panel + 8);
            y -= trim;
            lines = r_u32(panel + 44);
            text = r_u32(lines + (index << 2));
            x += width;
            width = front_text_width(text);
            lines = r_u32(panel + 44);
            text = r_u32(lines + (index << 2));
            front_draw_text_clip_top(text, x - width, y, color, trim);
        }
        else
        {
            lines = r_u32(panel + 44);
            y -= trim;
            text = r_u32(lines + (index << 2));
            front_draw_text_clip_top(text, x, y, color, trim);
        }
    }
    else
    {
        count = r_u32(panel + 28);
        spacing = r_u32(panel + 32);
        scroll = r_u32(panel + 36);
        height = r_u32(panel + 12);
        numerator = scroll + (count + 1) * spacing - height;
        trim = front_text_remainder(numerator, spacing);
        numerator = scroll + spacing - trim;
        row = front_text_quotient(numerator, spacing);
        index = front_text_remainder(row, count);
        color = r_u32(panel + 20);
        x = r_u32(panel);
        y = r_u32(panel + 4);
        lines = r_u32(panel + 44);
        y -= trim;
        text = r_u32(lines + (index << 2));
        front_draw_text_clip_top(text, x, y, color, trim);
    }
    for (;;)
    {
        spacing = r_u32(panel + 32);
        bottom = r_u32(panel + 4);
        height = r_u32(panel + 12);
        y += spacing;
        next_y = y + spacing;
        bottom += height;
        ++index;
        if ((sint32)bottom < (sint32)next_y)
            break;
        count = r_u32(panel + 28);
        row = front_text_remainder(index, count);
        lines = r_u32(panel + 44);
        text = r_u32(lines + (row << 2));
        if (right_aligned)
        {
            width = front_text_width(text);
            count = r_u32(panel + 28);
            row = front_text_remainder(index, count);
            lines = r_u32(panel + 44);
            text = r_u32(lines + (row << 2));
            front_draw_colored_text(text, x - width, y, color);
        }
        else
            front_draw_colored_text(text, x, y, color);
    }
    bottom = r_u32(panel + 4);
    height = r_u32(panel + 12);
    bottom += height;
    if ((sint32)y < (sint32)bottom)
    {
        if (right_aligned)
        {
            count = r_u32(panel + 28);
            row = front_text_remainder(index, count);
            scroll = r_u32(panel + 36);
            spacing = r_u32(panel + 32);
            trim = front_text_remainder(scroll + height, spacing);
            lines = r_u32(panel + 44);
            text = r_u32(lines + (row << 2));
            width = front_text_width(text);
            count = r_u32(panel + 28);
            row = front_text_remainder(index, count);
            lines = r_u32(panel + 44);
            text = r_u32(lines + (row << 2));
            front_draw_text_clip_bottom(text, x - width, y, color, trim);
        }
        else
        {
            scroll = r_u32(panel + 36);
            spacing = r_u32(panel + 32);
            trim = front_text_remainder(reverse ? scroll : scroll + height, spacing);
            count = r_u32(panel + 28);
            row = front_text_remainder(index, count);
            lines = r_u32(panel + 44);
            text = r_u32(lines + (row << 2));
            front_draw_text_clip_bottom(text, x, y, color, trim);
        }
    }
}

void front_scroll_text_left(uint32 panel)
{
    FUNCTION_MARKER(0x8009830C, "FRONT.OVL");
    front_draw_scrolling_text_panel(panel, 0, 0);
}

void front_scroll_text_right(uint32 panel)
{
    FUNCTION_MARKER(0x800985AC, "FRONT.OVL");
    front_draw_scrolling_text_panel(panel, 1, 0);
}

void front_scroll_text_reverse(uint32 panel)
{
    FUNCTION_MARKER(0x80098914, "FRONT.OVL");
    front_draw_scrolling_text_panel(panel, 0, 1);
}

void front_reset_text_emphasis(void)
{
    FUNCTION_MARKER(0x80099360, "FRONT.OVL");
    w_u32(0x8010D568, 0);
}

uint32 front_text_width_without_brackets(uint32 text)
{
    uint32 width = 0;
    uint32 character;
    uint32 glyph;
    uint32 bank;
    FUNCTION_MARKER(0x80099370, "FRONT.OVL");
    character = r_u8(text);
    while (character)
    {
        if (character != '[' && character != ']')
        {
            glyph = r_u32(0x8010D168 + (character << 2));
            if ((sint32)glyph < 0)
                width += 4;
            else
            {
                bank = r_u32(0x800FB870);
                width += (uint32)r_s16(0x8004EB7E + ((glyph + bank) << 4));
            }
        }
        character = r_u8(++text);
    }
    return width;
}

uint32 front_scan_text_emphasis(uint32 text)
{
    uint32 character;
    FUNCTION_MARKER(0x80099408, "FRONT.OVL");
    character = r_u8(text);
    while (character)
    {
        if (character == '[')
            w_u32(0x8010D568, 1);
        else if (character == ']')
            w_u32(0x8010D568, 0);
        character = r_u8(++text);
    }
    return 0;
}

void front_draw_justified_text_native(const char *text, const uint32 *layout)
{
    uint32 x;
    uint32 y;
    uint32 color;
    uint32 remaining;
    uint32 increment;
    uint32 accumulated = 0;
    uint32 previous_accumulated = 0;
    uint32 extra;
    uint32 character;
    uint32 glyph;
    uint32 bank;
    uint32 atlas;
    uint32 page = 0;
    uint32 cached_page = 0;
    uint32 packet;
    uint32 table;
    uint32 shift;
    uint32 atlas_x;
    uint32 atlas_y;
    uint32 value;
    sint32 previous;
    sint32 depth;
    sint32 graph_type;
    sint32 clut_x;
    sint32 clut_y;
    uint16 clut;
    FUNCTION_MARKER(0x80098FB8, "FRONT.OVL");
    x = layout[0];
    remaining = layout[4];
    y = layout[1];
    color = layout[2];
    increment = 0;
    if ((sint32)remaining > 0)
        increment = front_text_quotient(layout[3] << 16, remaining);
    character = (uint8)*text;
    while (character)
    {
        glyph = r_u32(0x8010D168 + (character << 2));
        if ((sint32)glyph < 0)
        {
            if (glyph == 0xFFFFFFFF)
            {
                --remaining;
                if (!remaining)
                {
                    extra = layout[3];
                    x += 4 + extra - (uint32)((sint32)accumulated >> 16);
                    accumulated = extra << 16;
                }
                else
                {
                    accumulated += increment;
                    x += 4 + (uint32)((sint32)accumulated >> 16) -
                         (uint32)((sint32)previous_accumulated >> 16);
                    previous_accumulated = accumulated;
                }
            }
            else
                x += 4;
        }
        else
        {
            bank = r_u32(0x800FB870);
            atlas = 0x8004EB70 + ((glyph + bank) << 4);
            depth = r_s16(atlas + 12);
            graph_type = GetGraphType();
            shift = (2 - (uint32)depth) & 31;
            if (graph_type == 1 || GetGraphType() == 2)
            {
                atlas_y = r_u16(atlas + 2);
                atlas_x = r_u16(atlas);
                page = (((uint32)depth & 3) << 9) | 0x80 | ((atlas_y & 0x300) >> 3) | ((atlas_x & 0x3FF) >> 6);
            }
            else
            {
                atlas_y = r_u16(atlas + 2);
                atlas_x = r_u16(atlas);
                page = (((uint32)depth & 3) << 7) | 0x20 | ((atlas_y >> 4) & 0x10) | ((atlas_x & 0x3FF) >> 6) | ((atlas_y & 0x200) << 2);
            }
            previous = (sint16)cached_page;
            if (!(uint16)cached_page)
            {
                cached_page = page;
                previous = (sint16)cached_page;
            }
            if ((uint32)previous != page)
            {
                packet = r_u32(0x801132B8);
                cached_page = page;
                SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)previous, NULL);
                table = r_u32(0x801132BC);
                AddPrim(psx_addr(table, 4), psx_addr(packet, 12));
                w_u32(0x801132B8, packet + 12);
            }
            packet = r_u32(0x801132B8);
            w_u8(packet + 3, 4);
            w_u8(packet + 7, 0x64);
            w_u16(packet + 8, x);
            w_u16(packet + 10, y);
            value = r_u16(atlas);
            w_u8(packet + 12, (value & 63) << shift);
            w_u8(packet + 13, r_u16(atlas + 2));
            w_u16(packet + 16, r_u16(atlas + 14));
            w_u16(packet + 18, r_u16(atlas + 10));
            clut_x = r_s16(atlas + 4);
            clut_y = r_s16(atlas + 6);
            clut = GetClut(clut_x, clut_y);
            w_u16(packet + 14, clut);
            value = r_u8(packet + 7);
            w_u8(packet + 4, color);
            w_u8(packet + 5, color >> 8);
            w_u8(packet + 7, value | 2);
            w_u8(packet + 6, color >> 16);
            table = r_u32(0x801132BC);
            AddPrim(psx_addr(table, 4), psx_addr(packet, 20));
            value = (uint32)r_s16(atlas + 14);
            w_u32(0x801132B8, packet + 20);
            x += value;
        }
        character = (uint8)*++text;
    }
    packet = r_u32(0x801132B8);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)page, NULL);
    table = r_u32(0x801132BC);
    AddPrim(psx_addr(table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

void front_draw_justified_text(uint32 text, uint32 layout)
{
    uint32 length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    front_draw_justified_text_native((const char *)psx_addr(text, (size_t)length + 1u), (const uint32 *)psx_addr(layout, 20u));
}

static uint32 front_draw_emphasis_run(uint32 text, uint32 *layout, uint32 clipping, sint32 extent)
{
    uint32 budget;
    uint32 x;
    uint32 y;
    uint32 remaining;
    uint32 color;
    uint32 reference;
    uint32 increment = 0;
    uint32 accumulated = 0;
    uint32 previous_accumulated = 0;
    uint32 extra;
    uint32 red;
    uint32 green;
    uint32 blue;
    uint32 character;
    uint32 glyph;
    uint32 atlas;
    uint32 value;
    uint32 packet;
    uint32 mode_packet;
    uint32 table;
    uint32 page = 0;
    uint16 cached_page = 0;
    sint32 previous;
    sint32 glyph_height;
    sint32 depth;
    sint32 graph_type;
    uint32 shift;
    uint32 atlas_x;
    uint32 atlas_y;
    uint16 clut;
    if (clipping == 1)
    {
        budget = layout[5];
        x = layout[0];
        y = layout[1];
        if (extent < 0)
        {
            y -= (uint32)extent;
            extent = 0;
        }
        reference = r_u32(0x8010D26C);
        value = r_u32(0x800FB870);
        glyph_height = r_s16(0x8004EB7A + ((reference + value) << 4));
        if (glyph_height < extent)
            return 0;
        remaining = layout[4];
        color = layout[2];
    }
    else
    {
        if (clipping == 2)
        {
            if (extent <= 0)
                return 0;
            reference = r_u32(0x8010D26C);
            value = r_u32(0x800FB870);
            glyph_height = r_s16(0x8004EB7A + ((reference + value) << 4));
            if (glyph_height < extent)
                extent = glyph_height;
        }
        budget = layout[5];
        x = layout[0];
        remaining = layout[4];
        y = layout[1];
        color = layout[2];
    }
    if ((sint32)remaining > 0)
        increment = front_text_quotient(layout[3] << 16, remaining);
    if (r_u32(0x8010D568))
    {
        red = (uint8)(color << 1);
        green = (uint8)((color >> 8) << 1);
        blue = (uint8)((color >> 16) << 1);
    }
    else
    {
        red = (uint8)color;
        green = (uint8)(color >> 8);
        blue = (uint8)(color >> 16);
    }
    character = r_u8(text);
    packet = r_u32(0x801132B8);
    while (character && (sint32)budget > 0)
    {
        if (character == '[')
        {
            red = (uint8)(color << 1);
            green = (uint8)((color >> 8) << 1);
            blue = (uint8)((color >> 16) << 1);
            w_u32(0x8010D568, 1);
        }
        else if (character == ']')
        {
            red = (uint8)color;
            green = (uint8)(color >> 8);
            blue = (uint8)(color >> 16);
            w_u32(0x8010D568, 0);
        }
        else
        {
            character = r_u8(text);
            glyph = r_u32(0x8010D168 + (character << 2));
            if ((sint32)glyph < 0)
            {
                if (glyph == 0xFFFFFFFF)
                {
                    --remaining;
                    if (!remaining)
                    {
                        extra = layout[3];
                        x += 4 + extra - (uint32)((sint32)accumulated >> 16);
                        accumulated = extra << 16;
                    }
                    else
                    {
                        accumulated += increment;
                        x += 4 + (uint32)((sint32)accumulated >> 16) -
                             (uint32)((sint32)previous_accumulated >> 16);
                        previous_accumulated = accumulated;
                    }
                }
                else
                    x += 4;
            }
            else
            {
                value = r_u32(0x800FB870);
                atlas = 0x8004EB70 + ((glyph + value) << 4);
                depth = r_s16(atlas + 12);
                page = 2;
                shift = (2 - (uint32)depth) & 31;
                graph_type = GetGraphType();
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
                if (!cached_page)
                    cached_page = (uint16)page;
                previous = (sint16)cached_page;
                if ((uint32)previous != page)
                {
                    mode_packet = packet;
                    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12), 0, 1, (uint16)previous, NULL);
                    table = r_u32(0x801132BC);
                    packet += 12;
                    cached_page = (uint16)page;
                    AddPrim(psx_addr(table + 4, 4), psx_addr(mode_packet, 12));
                }
                w_u8(packet + 3, 4);
                w_u8(packet + 7, 0x64);
                w_u16(packet + 8, x);
                w_u16(packet + 10, clipping == 1 ? y + (uint32)extent : y);
                value = r_u16(atlas);
                w_u8(packet + 12, (value & 63) << shift);
                if (clipping == 1)
                    w_u8(packet + 13, r_u8(atlas + 2) + (uint32)extent);
                else
                    w_u8(packet + 13, r_u16(atlas + 2));
                value = r_u16(atlas + 14);
                if (clipping == 2)
                {
                    w_u16(packet + 18, (uint32)extent);
                    w_u16(packet + 16, value);
                }
                else
                {
                    w_u16(packet + 16, value);
                    value = r_u16(atlas + 10);
                    if (clipping == 1)
                        value -= (uint32)extent;
                    w_u16(packet + 18, value);
                }
                clut = GetClut(r_s16(atlas + 4), r_s16(atlas + 6));
                w_u16(packet + 14, clut);
                w_u8(packet + 4, red);
                w_u8(packet + 5, green);
                w_u8(packet + 6, blue);
                table = r_u32(0x801132BC);
                AddPrim(psx_addr(table + 4, 4), psx_addr(packet, 20));
                value = (uint32)r_s16(atlas + 14);
                packet += 20;
                x += value;
            }
            --budget;
        }
        character = r_u8(++text);
    }
    mode_packet = packet;
    layout[0] = x;
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12), 0, 1, (uint16)page, NULL);
    table = r_u32(0x801132BC);
    AddPrim(psx_addr(table + 4, 4), psx_addr(mode_packet, 12));
    w_u32(0x801132B8, mode_packet + 12);
    return budget;
}

uint32 front_draw_emphasized_text_native(uint32 text, uint32 *layout)
{
    FUNCTION_MARKER(0x80099470, "FRONT.OVL");
    return front_draw_emphasis_run(text, layout, 0, 0);
}

uint32 front_draw_emphasized_text_clip_top_native(uint32 text, uint32 *layout, sint32 trim)
{
    FUNCTION_MARKER(0x80099928, "FRONT.OVL");
    return front_draw_emphasis_run(text, layout, 1, trim);
}

uint32 front_draw_emphasized_text_clip_bottom_native(uint32 text, uint32 *layout, sint32 height)
{
    FUNCTION_MARKER(0x80099E38, "FRONT.OVL");
    return front_draw_emphasis_run(text, layout, 2, height);
}

uint32 front_draw_emphasized_text(uint32 text, uint32 layout)
{
    return front_draw_emphasized_text_native(text, (uint32 *)psx_addr(layout, 24u));
}

uint32 front_draw_emphasized_text_clip_top(uint32 text, uint32 layout, sint32 trim)
{
    return front_draw_emphasized_text_clip_top_native(text, (uint32 *)psx_addr(layout, 24u), trim);
}

uint32 front_draw_emphasized_text_clip_bottom(uint32 text, uint32 layout, sint32 height)
{
    return front_draw_emphasized_text_clip_bottom_native(text,
        height > 0 ? (uint32 *)psx_addr(layout, 24u) : NULL, height);
}

void front_parse_campaign_filename(uint32 text)
{
    char filename[24];
    char *separator;
    char *mission;
    uint32 length = 0;
    sint32 value;
    FUNCTION_MARKER(0x8009A354, "FRONT.OVL");
    while (r_u8(text + length))
        ++length;
    strcpy(filename, (const char *)psx_addr(text, (size_t)length + 1));
    w_u16(0x800595FC, 0);
    w_u16(0x800595F8, 0);
    w_u16(0x800595FE, 0);
    w_u16(0x800595FA, 0);
    separator = strchr(filename, '_');
    if (separator)
    {
        *separator = 0;
        value = atoi(filename);
        mission = separator + 1;
        w_u16(0x800595FC, (uint32)value);
        w_u16(0x800595F8, (uint32)value);
        separator = strchr(mission, '.');
        if (separator)
        {
            *separator = 0;
            value = atoi(mission);
            w_u16(0x800595FE, (uint32)value);
            w_u16(0x800595FA, (uint32)value);
        }
    }
    if (r_u16(0x800595F8) >= 19)
    {
        w_u16(0x800595FC, 0);
        w_u16(0x800595F8, 0);
    }
    if (r_u16(0x800595FA) >= 7)
    {
        w_u16(0x800595FE, 0);
        w_u16(0x800595FA, 0);
    }
}

void front_texture_cache_idle(void)
{
    FUNCTION_MARKER(0x8009B3A0, "FRONT.OVL");
    /* Original function only returns */
}

void front_initialize_text_texture_cache(uint32 x, uint32 y, sint32 row_height)
{
    uint32 rows;
    uint32 index = 0;
    uint32 entry = 0x8010DB94;
    FUNCTION_MARKER(0x8009B3A8, "FRONT.OVL");
    rows = front_text_quotient(256, (uint32)row_height);
    w_u32(0x8010DB78, x);
    w_u32(0x8010DB80, 256);
    w_u32(0x8010DB84, 256);
    w_u32(0x8010DB8C, 256);
    w_u32(0x8010DB7C, y);
    w_u32(0x8010DB90, (uint32)row_height);
    w_u32(0x8010DC64, 512);
    w_u32(0x8010DC68, 0);
    w_u32(0x8010DB88, rows);
    if ((sint32)rows > 0)
    {
        do
        {
            w_u32(entry, 0);
            rows = r_u32(0x8010DB88);
            ++index;
            entry += 4;
        } while ((sint32)index < (sint32)rows);
    }
    ClearOTagR((uint32 *)psx_addr(0x8011046C, 4), 1);
}

void front_render_text_texture_cache(void)
{
    DRAWENV saved;
    DRAWENV target;
    uint32 x;
    uint32 y;
    uint32 width;
    uint32 height;
    uint32 reference;
    uint32 bank;
    uint32 atlas;
    sint32 depth;
    FUNCTION_MARKER(0x8009B478, "FRONT.OVL");
    GetDrawEnv(&saved);
    x = r_u32(0x8010DB78);
    y = r_u32(0x8010DB7C);
    width = r_u32(0x8010DB80);
    height = r_u32(0x8010DB84);
    SetDefDrawEnv(&target, (sint32)x, (sint32)y, (sint32)width, (sint32)height);
    reference = r_u32(0x8010D26C);
    bank = r_u32(0x800FB870);
    target.isbg = 1;
    target.dfe = 1;
    target.dtd = 0;
    atlas = 0x8004EB70 + ((reference + bank) << 4);
    depth = r_s16(atlas + 12);
    target.tpage = GetTPage(depth, 1, r_s16(atlas), r_s16(atlas + 2));
    target.r0 = 0;
    target.g0 = 0;
    target.b0 = 0;
    PutDrawEnv(&target);
    DrawOTag((uint32 *)psx_addr(0x8011046C, 4));
    DrawSync(0);
    PutDrawEnv(&saved);
}

void front_cache_text_texture(uint32 record)
{
    uint32 selected = 0xFFFFFFFF;
    uint32 text;
    uint32 length;
    uint32 used;
    uint32 capacity;
    uint32 width;
    uint32 index;
    uint32 count;
    uint32 maximum;
    uint32 row;
    uint32 value;
    uint32 origin;
    uint32 packet;
    uint32 character;
    uint32 glyph;
    uint32 atlas;
    uint32 shift;
    uint32 opcode;
    uint16 clut;
    FUNCTION_MARKER(0x8009B56C, "FRONT.OVL");
    text = r_u32(record);
    length = 0;
    while (r_u8(text + length))
        ++length;
    length = (uint32)strlen((const char *)psx_addr(text, (size_t)length + 1));
    used = r_u32(0x8010DC68);
    capacity = r_u32(0x8010DC64);
    if (used + length <= capacity)
    {
        width = front_text_width(r_u32(record));
        count = r_u32(0x8010DB88);
        if ((sint32)count > 0)
        {
            row = 0x8010DB94;
            maximum = r_u32(0x8010DB8C);
            count = r_u32(0x8010DB88);
            index = 0;
            do
            {
                value = r_u32(row) + width;
                if ((sint32)value < (sint32)maximum)
                {
                    selected = index;
                    break;
                }
                ++index;
                row += 4;
            } while ((sint32)index < (sint32)count);
        }
    }
    if (selected == 0xFFFFFFFF)
    {
        w_u32(record + 12, selected);
        w_u32(record + 16, selected);
        return;
    }
    row = 0x8010DB94 + (selected << 2);
    value = r_u32(row);
    origin = r_u32(0x8010DB78);
    w_u32(record + 12, value + origin);
    value = r_u32(0x8010DB90);
    text = r_u32(record);
    origin = r_u32(0x8010DB7C);
    w_u32(record + 20, width);
    w_u32(record + 16, value * selected + origin);
    value = r_u32(0x8010DB90);
    w_u32(record + 24, value);
    used = r_u32(0x8010DC68);
    packet = 0x8010DC6C + used * 20;
    length = 0;
    while (r_u8(text + length))
        ++length;
    length = (uint32)strlen((const char *)psx_addr(text, (size_t)length + 1));
    used = r_u32(0x8010DC68);
    w_u32(0x8010DC68, used + length);
    character = r_u8(text);
    while (character)
    {
        glyph = r_u32(0x8010D168 + (character << 2));
        if ((sint32)glyph < 0)
            w_u32(row, r_u32(row) + 4);
        else
        {
            value = r_u32(0x800FB870);
            atlas = 0x8004EB70 + ((glyph + value) << 4);
            value = (uint32)r_s16(atlas + 12);
            shift = (2 - value) & 31;
            SetSprt((SPRT *)psx_addr(packet, 20));
            w_u16(packet + 8, r_u32(row));
            value = r_u32(0x8010DB90);
            w_u16(packet + 10, value * selected);
            value = r_u16(atlas);
            w_u8(packet + 12, (value & 63) << shift);
            w_u8(packet + 13, r_u16(atlas + 2));
            value = (uint32)r_s16(atlas + 8);
            w_u16(packet + 16, value << shift);
            w_u16(packet + 18, r_u16(atlas + 10));
            clut = GetClut(r_s16(atlas + 4), r_s16(atlas + 6));
            opcode = r_u8(packet + 7);
            w_u8(packet + 4, 128);
            w_u16(packet + 14, clut);
            w_u8(packet + 5, 128);
            w_u8(packet + 6, 128);
            w_u8(packet + 7, opcode | 2);
            AddPrim(psx_addr(0x8011046C, 4), psx_addr(packet, 20));
            value = (uint32)r_s16(atlas + 14);
            origin = r_u32(row);
            packet += 20;
            w_u32(row, origin + value);
        }
        character = r_u8(++text);
    }
    value = r_u32(row) + 1;
    w_u32(row, value & 0xFFFE);
}

void front_screen_slide_idle(void)
{
    FUNCTION_MARKER(0x8009BDC0, "FRONT.OVL");
    /* Original function only returns */
}

uint32 front_wait_event_bank_a(void)
{
    FUNCTION_MARKER(0x8009D0C8, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 2;
}

uint32 front_wait_event_bank_a_secondary(void)
{
    FUNCTION_MARKER(0x8009D14C, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 2;
}

uint32 front_wait_event_bank_b(void)
{
    FUNCTION_MARKER(0x8009D1D0, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 2;
}

uint32 front_poll_event_bank_b(void)
{
    FUNCTION_MARKER(0x8009D25C, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_poll_event_bank_a(void)
{
    FUNCTION_MARKER(0x8009D2B4, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_query_card_state(uint32 port)
{
    FUNCTION_MARKER(0x8009D548, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 2;
}

uint32 front_wait_card_check_event(void)
{
    FUNCTION_MARKER(0x8009D624, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 2;
}

uint32 front_request_card_load(uint32 port)
{
    FUNCTION_MARKER(0x8009D6A8, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_refresh_card_status(uint32 port)
{
    FUNCTION_MARKER(0x8009D304, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    w_u8(0x800F2D8C + port, 2);
    w_u8(0x800F2D8A + port, 0);
    return 1;
}

uint32 front_erase_card(sint32 channel)
{
    FUNCTION_MARKER(0x8009D460, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

void front_encode_card_title(const char *text, uint8 *encoded)
{
    uint32 lead = 0;
    uint32 trail = 0;
    uint32 character;
    FUNCTION_MARKER(0x8009D6E4, "FRONT.OVL");
    character = (uint8)*text;
    while (character)
    {
        if ((character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9'))
        {
            trail = character + 31;
            lead = 0x82;
        }
        else if (character >= 'a' && character <= 'z')
        {
            trail = (uint8)*text + 32;
            lead = 0x82;
        }
        else if (character == ' ' || character == ':')
        {
            trail = character == ' ' ? 0x40 : 0x46;
            lead = 0x81;
        }
        encoded[0] = (uint8)lead;
        encoded[1] = (uint8)trail;
        encoded += 2;
        character = (uint8)*++text;
    }
    encoded[0] = 0;
    encoded[1] = 0;
}
