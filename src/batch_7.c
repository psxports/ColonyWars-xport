#include "game_functions.h"
#include <stdlib.h>
#include <string.h>
#include "psx_press.h"
#include "psx_stream.h"
#include "psx_gpu.h"

void draw_set_color_rgb(uint32 red, uint32 green, uint32 blue)
{
    FUNCTION_MARKER(0x80021EC8, "SLUS_005.43");
    w_u8(0x8004617C, red);
    w_u8(0x80033D38, green);
    w_u8(0x80053680, blue);
}

sint32 draw_set_color_packed(sint32 color)
{
    sint32 result;
    FUNCTION_MARKER(0x80021EE8, "SLUS_005.43");
    result = color >> 8;
    w_u8(0x8004617C, color);
    color >>= 16;
    w_u8(0x80033D38, result);
    w_u8(0x80053680, color);
    return result;
}

uint32 camera_set_target_pair(uint32 object, uint32 reference)
{
    uint32 flags;
    FUNCTION_MARKER(0x80024338, "SLUS_005.43");
    flags = r_u32(0x8004EB30);
    w_u32(0x8004EB34, object);
    w_u32(0x8004EB38, reference);
    flags |= 4;
    w_u32(0x8004EB30, flags);
    return flags;
}

uint32 camera_set_offset(uint32 vector, uint32 first_parameter, uint32 second_parameter)
{
    uint32 value;
    FUNCTION_MARKER(0x80024360, "SLUS_005.43");
    value = r_u32(0x8004EB30) | 1;
    w_u32(0x8004EB30, value);
    value = r_u32(vector);
    w_u32(0x8004EB44, value);
    value = r_u32(vector + 4);
    w_u32(0x8004EB48, value);
    value = r_u32(vector + 8);
    w_u32(0x8004EB54, first_parameter);
    w_u32(0x8004EB58, second_parameter);
    w_u32(0x8004EB4C, value);
    return value;
}

uint32 camera_set_look_at_object_offset(uint32 object, uint32 vector)
{
    uint32 value;
    FUNCTION_MARKER(0x800243BC, "SLUS_005.43");
    value = r_u32(0x8004EB30);
    w_u32(0x8004EB3C, object);
    value |= 8;
    w_u32(0x8004EB30, value);
    value = r_u32(vector);
    w_u32(0x8004EB44, value);
    value = r_u32(vector + 4);
    w_u32(0x8004EB48, value);
    value = r_u32(vector + 8);
    w_u32(0x8004EB4C, value);
    return value;
}

uint32 camera_set_look_at_point(uint32 vector)
{
    uint32 flags;
    FUNCTION_MARKER(0x80024454, "SLUS_005.43");
    flags = r_u32(0x8004EB30);
    w_u32(0x8004EB40, vector);
    flags |= 0x10;
    w_u32(0x8004EB30, flags);
    return flags;
}

uint32 camera_set_look_at_object(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x80024474, "SLUS_005.43");
    flags = r_u32(0x8004EB30);
    w_u32(0x8004EB3C, object);
    flags |= 8;
    w_u32(0x8004EB30, flags);
    return flags;
}

uint32 camera_apply_translation(uint32 vector)
{
    FUNCTION_MARKER(0x800244D0, "SLUS_005.43");
    return camera_apply_translation_native((const VECTOR *)psx_addr(vector, 12u));
}

uint32 camera_apply_translation_native(const VECTOR *vector)
{
    uint32 object;
    uint32 value;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 component;
    object = r_u32(0x80040C94);
    value = (uint32)vector->vx;
    x = (uint32)vector->vx;
    y = (uint32)vector->vy;
    z = (uint32)vector->vz;
    w_u32(object + 0xFC, value);
    value = (uint32)vector->vy;
    w_u32(object + 0x100, value);
    value = (uint32)vector->vz;
    w_u32(object + 0x104, value);
    value = r_u32(0x800311E0);
    component = (uint32)vector->vx;
    value += x;
    w_u32(0x800311E0, value);
    value = r_u32(0x800311E4);
    w_u32(object + 0x14, component);
    component = (uint32)vector->vy;
    value += y;
    w_u32(0x800311E4, value);
    value = r_u32(0x800311E8);
    w_u32(object + 0x18, component);
    component = (uint32)vector->vz;
    value += z;
    w_u32(0x800311E8, value);
    w_u32(object + 0x1C, component);
    return value;
}

uint32 camera_request_snap(void)
{
    FUNCTION_MARKER(0x8002455C, "SLUS_005.43");
    w_u32(0x8004EB6C, 1);
    return 1;
}

uint32 object_lookup(uint32 identifier)
{
    FUNCTION_MARKER(0x80024590, "SLUS_005.43");
    if (identifier)
        return r_u32(0x80031200 + (identifier << 3));
    return r_u32(0x8005367C);
}

uint32 object_initialize_pool(void)
{
    uint32 object = 0x80046180;
    uint32 previous = 0;
    uint32 offset;
    FUNCTION_MARKER(0x80024734, "SLUS_005.43");
    w_u32(0x800311F4, 0);
    while (object < 0x8004E52C)
    {
        w_u32(object, 0);
        w_u32(object + 0x1D0, previous);
        previous = object;
        w_u32(object + 0x1D8, 0);
        w_u32(object + 0x1DC, 0);
        object += 0x27C;
        w_u32(object - 0xA8, object);
    }
    w_u32(0x80052E68, 0x80046180);
    w_u32(object + 0x1D0, previous);
    w_u32(object + 0x1D4, 0);
    w_u32(0x80041F50, 0x8004E52C);
    w_u32(0x8005367C, 0);
    w_u32(0x80042164, 0);
    for (offset = 0; offset < 0x50; offset += 8)
    {
        w_u32(0x80031200 + offset, 0);
        w_u32(0x80031204 + offset, 0);
    }
    return 0;
}

uint32 object_advance_counter_twice(uint32 object)
{
    uint32 result;
    FUNCTION_MARKER(0x80024904, "SLUS_005.43");
    result = r_u32(object + 0x20) + 2;
    w_u32(object + 0x20, result);
    return result;
}

uint32 object_advance_counter_pair(uint32 object)
{
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x80024918, "SLUS_005.43");
    first = r_u32(object + 0x20);
    second = r_u32(object + 0x24);
    ++first;
    ++second;
    w_u32(object + 0x20, first);
    w_u32(object + 0x24, second);
    return first;
}

void object_noop_first(void)
{
    FUNCTION_MARKER(0x80024934, "SLUS_005.43");
    /* Original body is JR RA followed by NOP */
}

void object_noop_second(void)
{
    FUNCTION_MARKER(0x8002493C, "SLUS_005.43");
    /* Original body is JR RA followed by NOP */
}

void sound_disable_sample(uint32 index)
{
    FUNCTION_MARKER(0x800267BC, "SLUS_005.43");
    w_u8(0x80033CF4 + index, 0);
}

uint32 stream_reset_all_queues(void)
{
    FUNCTION_MARKER(0x800278D0, "SLUS_005.43");
    w_u32(r_u32(0x80041000), 0);
    w_u32(r_u32(0x80040FFC), 0);
    w_u32(r_u32(0x80041008), 0);
    w_u32(r_u32(0x80041004), 0);
    w_u32(r_u32(0x80040FF8), 2);
    w_u32(0x80040FE4, 7);
    return 7;
}

uint32 stream_cancel_current(void)
{
    uint32 buffer;
    uint32 mode;
    uint32 current;
    uint32 result;
    FUNCTION_MARKER(0x80027A7C, "SLUS_005.43");
    buffer = r_u32(0x80041000);
    w_u32(0x80041EE8, 1);
    w_u32(buffer, 0);
    mode = r_u32(0x80040FE4);
    if (mode - 6 < 2 || mode - 2 < 2)
    {
        current = r_u32(0x80040FF8);
        result = r_u32(0x80040FFC);
        if (current == result)
        {
            w_u32(0x80040FE4, 7);
            return 7;
        }
    }
    else
        result = r_u32(0x80040FFC);
    w_u32(result, 0);
    return result;
}

uint32 heap_coalesce_largest(void)
{
    uint32 block;
    uint32 previous = 0;
    uint32 header;
    uint32 largest = 0;
    uint32 advance;
    FUNCTION_MARKER(0x80029E54, "SLUS_005.43");
    block = r_u32(0x80040F4C);
    header = r_u32(block);
    while ((sint32)header > 0)
    {
        if (header & 1)
        {
            if (previous)
            {
                header = r_u32(previous) + 3 + header;
                block = previous;
                w_u32(previous, header);
            }
            else
                previous = block;
            if ((sint32)largest < (sint32)header)
                largest = header - 1;
        }
        else
            previous = 0;
        advance = (uint32)((sint32)header >> 2) << 2;
        block += advance + 4;
        header = r_u32(block);
    }
    return largest;
}

void render_set_near_depth(uint32 depth)
{
    FUNCTION_MARKER(0x8002C6C4, "SLUS_005.43");
    w_u32(0x800313E0, depth);
}

void render_set_far_depth(uint32 depth)
{
    FUNCTION_MARKER(0x8002C6D4, "SLUS_005.43");
    w_u32(0x800313E4, depth);
}

sint32 vector_cross_fixed(uint32 first, uint32 second, uint32 destination)
{
    uint32 left;
    uint32 right;
    uint32 product;
    uint32 other;
    sint32 result;
    FUNCTION_MARKER(0x8002C818, "SLUS_005.43");
    left = r_u32(first + 4);
    right = r_u32(second + 8);
    product = left * right;
    left = r_u32(first + 8);
    right = r_u32(second + 4);
    other = left * right;
    result = (sint32)(product - other) >> 12;
    w_u32(destination, result);
    left = r_u32(first + 8);
    right = r_u32(second);
    product = left * right;
    left = r_u32(first);
    right = r_u32(second + 8);
    other = left * right;
    result = (sint32)(product - other) >> 12;
    w_u32(destination + 4, result);
    left = r_u32(first);
    right = r_u32(second + 4);
    product = left * right;
    left = r_u32(first + 4);
    right = r_u32(second);
    other = left * right;
    result = (sint32)(product - other) >> 12;
    w_u32(destination + 8, result);
    return result;
}

sint32 vector_multiply_fixed(uint32 first, uint32 second, uint32 destination)
{
    uint32 left;
    uint32 right;
    uint32 product;
    sint32 result;
    FUNCTION_MARKER(0x8002C8B8, "SLUS_005.43");
    left = r_u32(first);
    right = r_u32(second);
    product = left * right;
    result = (sint32)product >> 12;
    w_u32(destination, result);
    left = r_u32(first + 4);
    right = r_u32(second + 4);
    product = left * right;
    result = (sint32)product >> 12;
    w_u32(destination + 4, result);
    left = r_u32(first + 8);
    right = r_u32(second + 8);
    product = left * right;
    result = (sint32)product >> 12;
    w_u32(destination + 8, result);
    return result;
}

sint32 vector_dot_fixed(uint32 first, uint32 second)
{
    uint32 left;
    uint32 right;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 result;
    FUNCTION_MARKER(0x8002C950, "SLUS_005.43");
    left = r_u32(first);
    right = r_u32(second);
    x = left * right;
    left = r_u32(first + 4);
    right = r_u32(second + 4);
    y = left * right;
    left = r_u32(first + 8);
    right = r_u32(second + 8);
    z = left * right;
    result = (uint32)((sint32)x >> 12) + (uint32)((sint32)y >> 12);
    result += (uint32)((sint32)z >> 12);
    return (sint32)result;
}

static sint32 vector_dot_s16_s32_body(uint32 first, uint32 second, const VECTOR *native_second)
{
    uint32 left;
    uint32 right;
    uint32 x;
    uint32 y;
    uint32 z;
    left = (uint32)r_s16(first);
    right = native_second != NULL ? (uint32)native_second->vx : r_u32(second);
    x = left * right;
    left = (uint32)r_s16(first + 2);
    right = native_second != NULL ? (uint32)native_second->vy : r_u32(second + 4);
    y = left * right;
    left = (uint32)r_s16(first + 4);
    right = native_second != NULL ? (uint32)native_second->vz : r_u32(second + 8);
    z = left * right;
    return (sint32)(x + y + z);
}

sint32 vector_dot_s16_s32_raw(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x8002C9EC, "SLUS_005.43");
    return vector_dot_s16_s32_body(first, second, NULL);
}

sint32 vector_dot_s16_s32_native(uint32 first, const VECTOR *second)
{
    return vector_dot_s16_s32_body(first, 0, second);
}

sint32 scalar_abs_wrap(sint32 value)
{
    FUNCTION_MARKER(0x8002E0B0, "SLUS_005.43");
    if (value < 0)
        return (sint32)(0 - (uint32)value);
    return value;
}

sint32 vector_length_approx2(sint32 first, sint32 second)
{
    uint32 maximum;
    uint32 minimum;
    uint32 result;
    FUNCTION_MARKER(0x8002E2C4, "SLUS_005.43");
    if (first < 0)
        first = (sint32)(0 - (uint32)first);
    if (second < 0)
        second = (sint32)(0 - (uint32)second);
    if (first < second)
    {
        maximum = (uint32)second;
        minimum = (uint32)first;
    }
    else
    {
        maximum = (uint32)first;
        minimum = (uint32)second;
    }
    result = (maximum << 4) + (minimum << 2) + minimum;
    return (sint32)result >> 4;
}

sint32 text_measure_width(uint32 text)
{
    uint32 character;
    sint32 glyph;
    uint32 index;
    uint32 font;
    sint32 width;
    uint32 result = 0;
    FUNCTION_MARKER(0x8002EF24, "SLUS_005.43");
    for (;;)
    {
        character = r_u8(text++);
        if (!character)
            break;
        glyph = r_s16(0x8003FC68 + (character << 1));
        font = r_u32(0x80033D2C);
        if (glyph >= 0)
            index = (uint32)glyph + font;
        else
            index = 0xFFFFFFFF;
        width = r_s16(0x8004EB7E + (index << 4));
        if (font == 0x54)
            ++result;
        result += (uint32)width;
    }
    return (sint32)result;
}

sint32 text_measure_width_without_brackets(uint32 text)
{
    uint32 character;
    sint32 glyph;
    uint32 index;
    uint32 result = 0;
    FUNCTION_MARKER(0x8002EFC4, "SLUS_005.43");
    character = r_u8(text);
    while (character)
    {
        if (character != '[' && character != ']')
        {
            glyph = r_s16(0x8003FC68 + (character << 1));
            if (glyph < 0)
                index = 0xFFFFFFFF;
            else
                index = (uint32)glyph + r_u32(0x80033D2C);
            if ((sint32)index < 0)
                result += 4;
            else
                result += (uint32)r_s16(0x8004EB7E + (index << 4));
        }
        ++text;
        character = r_u8(text);
    }
    return (sint32)result;
}

void config_noop(void)
{
    FUNCTION_MARKER(0x8003045C, "SLUS_005.43");
    /* Original body is JR RA followed by NOP */
}

uint32 config_read_line(uint32 source, uint32 destination)
{
    uint32 character;
    FUNCTION_MARKER(0x80030464, "SLUS_005.43");
    do
    {
        character = r_u8(source++);
        w_u8(destination++, character);
    } while (character != 13 && character != 10 && character != 0 && character != 26);
    w_u8(destination - 1, 0);
    character = r_u8(source);
    if (character == 13 || character == 10)
        ++source;
    return source;
}

uint32 config_split_arguments(uint32 line, uint32 *arguments)
{
    uint32 character;
    uint32 count = 0;
    FUNCTION_MARKER(0x800304CC, "SLUS_005.43");
    for (;;)
    {
        character = r_u8(line++);
        if (character == '=')
        {
            w_u8(line - 1, ' ');
            character = ' ';
        }
        if (character == ' ' || character == 9)
            continue;
        if (!character)
            break;
        *arguments++ = line - 1;
        ++count;
        do
        {
            character = r_u8(line++);
            if (character == '=')
            {
                w_u8(line - 1, ' ');
                character = ' ';
            }
        } while (character && character != ' ' && character != 9);
        w_u8(line - 1, 0);
        if (!character)
            break;
    }
    *arguments = 0x80011F04;
    return count;
}

void front_set_movie_limit(const char *path)
{
    const char *tail;
    const char *name;
    size_t length;
    uint32 entry;
    uint32 pointer;
    uint32 index;
    sint32 limit;
    FUNCTION_MARKER(0x800895C8, "FRONT.OVL");
    tail = path + strlen(path) - 8;
    for (index = 0; index < 25; ++index)
    {
        entry = 0x800B727C + index * 8;
        pointer = r_u32(entry);
        name = (const char *)psx_addr(pointer, 12);
        length = strlen(name);
        pointer = r_u32(entry);
        name = (const char *)psx_addr(pointer, 12);
        if (strcmp(tail, name + length - 8) == 0)
        {
            if (r_u32(0x800311C0))
                limit = r_u16(entry + 4);
            else
                limit = r_s16(entry + 4) * 30 / 25;
            w_u16(0x8011329A, limit);
            return;
        }
    }
    w_u16(0x8011329A, 20000);
}

sint32 front_play_movie_with_sound_reset(const char *path, uint32 parameter)
{
    FUNCTION_MARKER(0x8008ABA4, "FRONT.OVL");
    sound_reset_voices();
    w_u32(0x800B7344, parameter);
    w_u32(0x8011320C, 0xFFFFFFFF);
    w_u32(0x800FB808, 0);
    w_u32(0x80113220, 0);
    w_u32(0x8010DA68, VSync(-1));
    w_u8(0x800FB93C, 1);
    w_u32(0x8011321C, 0);
    front_play_movie(path);
    return r_s16(0x800FB808);
}

uint32 front_play_movie_delayed(const char *path)
{
    FUNCTION_MARKER(0x8008AC44, "FRONT.OVL");
    w_u8(0x800FB93C, 1);
    w_u32(0x8011321C, 55);
    return front_play_movie(path);
}

uint32 front_play_movie_preserve_display(const char *path)
{
    FUNCTION_MARKER(0x8008AC7C, "FRONT.OVL");
    w_u8(0x800FB93C, 0);
    w_u32(0x8011321C, 0);
    return front_play_movie(path);
}

void front_clear_vram_regions(uint32 mode)
{
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x8008AF70, "FRONT.OVL");
    rectangle.x = mode ? 320 : 0;
    rectangle.y = 0;
    rectangle.w = mode ? 192 : 512;
    rectangle.h = 256;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    rectangle.x = mode ? 320 : 0;
    rectangle.y = 256;
    rectangle.w = mode ? 192 : 512;
    rectangle.h = 256;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    rectangle.x = 512;
    rectangle.y = 0;
    rectangle.w = 512;
    rectangle.h = 256;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    rectangle.x = 512;
    rectangle.y = 256;
    rectangle.w = 512;
    rectangle.h = 256;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);
}

uint32 front_initialize_video(void)
{
    uint32 pal;
    uint32 x;
    uint32 y;
    FUNCTION_MARKER(0x8008B0BC, "FRONT.OVL");
    ResetCallback();
    InitGeom();
    pal = r_u32(0x800311C0);
    if (pal)
    {
        SetVideoMode(1);
        VSync(30);
        pal = r_u32(0x800311C0);
    }
    if (pal)
    {
        x = r_u32(0x8001056C);
        y = r_u32(0x80010570);
    }
    else
    {
        x = r_u32(0x80010574);
        y = r_u32(0x80010578);
    }
    w_u32(0x80010564, x);
    w_u32(0x80010568, y);
    return x;
}

void front_decode_still(uint32 *bitstream, const PSX_RECT *input, uint32 format)
{
    sint32 height;
    uint32 x, y;
    sint32 width, factor, output_width;
    uint32 vlc_bytes, band_bytes, allocation;
    sint32 right, current, words;
    uint32 next, step;
    PSX_RECT band;
    FUNCTION_MARKER(0x8008ACAC, "FRONT.OVL");
#ifndef NDEBUG
    printf("STILL: starting\n");
#endif
    height = input->h;
    x = (uint16)input->x;
    y = (uint16)input->y;
    width = input->w;
    if (height >= 301)
        w_u16(0x800FB818, r_u32(0x800311C0) ? 512 : 480);
    else
        w_u16(0x800FB818, r_u32(0x800311C0) ? 256 : 240);
    if (format == 1)
    {
        w_u16(0x8010CDA8, 1);
        factor = 3;
    }
    else
    {
        w_u16(0x8010CDA8, 0);
        factor = 2;
    }
    output_width = width * factor / 2;
    w_u32(0x800FB098, 0);
    w_u16(0x800FB0A0, x);
    w_u16(0x800FB0A8, x);
    w_u16(0x800FB0B4, x);
    w_u32(0x800FB0B0, 0);
    w_u32(0x800FB0BC, 0);
    w_u16(0x800FB0A2, y);
    w_u16(0x800FB0A6, height);
    w_u16(0x800FB0AA, y);
    w_u16(0x800FB0AE, height);
    w_u16(0x800FB0B6, y);
    w_u16(0x800FB0B8, factor * 8);
    w_u16(0x800FB0BA, height);
    w_u16(0x800FB0A4, output_width);
    w_u16(0x800FB0AC, output_width);
    vlc_bytes = (uint32)DecDCTBufSize(bitstream) * 5;
    allocation = heap_alloc_back(vlc_bytes);
    w_u32(0x800FB810, allocation);
    w_u32(0x800FB090, allocation);
    w_u32(0x800FB814, allocation);
    w_u32(0x800FB094, allocation);
    band_bytes = (uint32)(factor * height * 8) * 2;
    allocation = heap_alloc_back(band_bytes);
    w_u32(0x800FB7A8, allocation);
    w_u32(0x800FB09C, allocation);
    DecDCTReset(0);
    DecDCToutCallback(NULL);
    DecDCTvlc(bitstream, (uint32 *)psx_addr(r_u32(0x800FB090), vlc_bytes));
    allocation = r_u32(0x800FB090);
    format = r_u16(0x8010CDA8);
    DecDCTin((uint32 *)psx_addr(allocation, vlc_bytes), format);
    right = r_s16(0x800FB0A0);
    right += r_s16(0x800FB0A4);
    current = r_s16(0x800FB0B4);
    if (current < right)
    {
        do
        {
            words = r_s16(0x800FB0B8);
            words = words * r_s16(0x800FB0BA) / 2;
            allocation = r_u32(0x800FB09C);
            DecDCTout((uint32 *)psx_addr(allocation, band_bytes), words);
            DecDCToutSync(0);
            allocation = r_u32(0x800FB09C);
            band.x = r_s16(0x800FB0B4);
            band.y = r_s16(0x800FB0B6);
            band.w = r_s16(0x800FB0B8);
            band.h = r_s16(0x800FB0BA);
            LoadImagePSX(&band, (uint32 *)psx_addr(allocation, band_bytes));
            DrawSync(0);
            next = r_u16(0x800FB0B4);
            step = r_u16(0x800FB0B8);
            width = r_s16(0x800FB0A4);
            next += step;
            w_u16(0x800FB0B4, next);
            right = r_s16(0x800FB0A0) + width;
            current = (sint16)next;
        } while (current < right);
    }
    heap_free(r_u32(0x800FB810));
    heap_free(r_u32(0x800FB7A8));
#ifndef NDEBUG
    printf("STILL: complete\n");
#endif
}

uint32 front_reset_settings(void)
{
    uint32 index;
    sint32 byte;
    FUNCTION_MARKER(0x8008B154, "FRONT.OVL");
    w_u16(0x8005960C, 1);
    w_u16(0x800595FC, 1);
    w_u16(0x800595F8, 1);
    w_u16(0x80059608, 1);
    w_u16(0x8005960E, 1);
    w_u16(0x800595FE, 1);
    w_u16(0x800595FA, 1);
    w_u16(0x8005960A, 1);
    for (index = 0; index < 8; ++index)
    {
        w_u16(0x8005961E + index * 2, 0);
        w_u32(0x80011904 + index * 4, 0);
    }
    w_u32(0x80059630, 0);
    w_u32(0x80011928, 0);
    w_u16(0x80011902, 0);
    w_u32(0x80011924, 0);
    for (index = 0; index < 19; ++index)
    {
        w_u8(0x80010478 + index, 0);
        for (byte = 6; byte >= 0; --byte)
            w_u8(0x80010490 + index * 7 + byte, 0);
    }
    for (byte = 6; byte >= 0; --byte)
        w_u8(0x80010F5C + byte, 0);
    w_u32(0x800F3158, 0);
    w_u32(0x800F3150, 0);
    return front_snapshot_settings(0x80010258);
}

uint32 front_snapshot_settings(uint32 destination)
{
    uint32 source, output, offset, record, index, byte, alignment;
    uint32 values[4];
    FUNCTION_MARKER(0x8009E27C, "FRONT.OVL");
    w_u16(destination, r_u16(0x800595F8));
    w_u16(destination + 2, r_u16(0x800595FA));
    w_u16(destination + 4, r_u16(0x800595FC));
    w_u16(destination + 6, r_u16(0x800595FE));
    w_u16(destination + 8, r_u16(0x8005960C));
    w_u16(destination + 10, r_u16(0x8005960E));
    for (offset = 0; offset < 80; offset += 16)
    {
        for (index = 0; index < 4; ++index)
            values[index] = r_u32(0x800118E0 + offset + index * 4);
        for (index = 0; index < 4; ++index)
            w_u32(destination + 12 + offset + index * 4, values[index]);
    }
    w_u32(destination + 92, r_u32(0x80011930));
    for (record = 0; record < 5; ++record)
    {
        source = 0x800421C0 + record * 48;
        output = destination + 96 + record * 48;
        alignment = (source | output) & 3;
        for (offset = 0; offset < 48; offset += 16)
        {
            for (index = 0; index < 4; ++index)
                values[index] = r_u32(source + offset + index * 4);
            for (index = 0; index < 4; ++index)
            {
                if (alignment)
                {
                    for (byte = 4 - alignment; byte < 4; ++byte)
                        w_u8(output + offset + index * 4 + byte, values[index] >> (byte * 8));
                    for (byte = 0; byte < 4 - alignment; ++byte)
                        w_u8(output + offset + index * 4 + byte, values[index] >> (byte * 8));
                }
                else
                    w_u32(output + offset + index * 4, values[index]);
            }
        }
    }
    w_u32(destination + 0x150, r_u32(0x800311C0));
    for (index = 0; index < 5; ++index)
        w_u32(destination + 0x154 + index * 4, r_u32(0x8001056C + index * 4));
    for (record = 0; record < 19; ++record)
    {
        w_u8(destination + 0x1ED + record, r_u8(0x80010478 + record));
        for (byte = 0; byte < 7; ++byte)
            w_u8(destination + 0x168 + record * 7 + byte, r_u8(0x80010490 + record * 7 + byte));
    }
    for (byte = 0; byte < 7; ++byte)
        w_u8(destination + 0x200 + byte, r_u8(0x80010F5C + byte));
    front_encode_settings_code(destination);
    strcpy((char *)psx_addr(destination + 0x207, 17), (const char *)psx_addr(0x800FB220, 17));
    w_u32(0x80010248, 0);
    return destination + 0x207;
}

uint32 front_encode_settings_code(uint32 settings)
{
    uint32 input = 0x8010D0E8;
    uint32 output = 0x800FB220;
    uint32 remaining = 96;
    uint8 value, packed = 0, bits_read = 0, packed_bits = 0;
    FUNCTION_MARKER(0x800AB578, "FRONT.OVL");
    front_pack_settings_bits(settings);
    front_xor_bytes(0x8010D0E8, 12, 1);
    value = r_u8(input++);
    while (remaining--)
    {
        packed = (uint8)(packed << 1) | (value >> 7);
        value = (uint8)(value << 1);
        ++bits_read;
        ++packed_bits;
        if (bits_read == 8)
        {
            value = r_u8(input++);
            bits_read = 0;
        }
        if (packed_bits == 6)
        {
            w_u8(output++, front_settings_code_character(packed));
            packed_bits = 0;
            packed = 0;
        }
    }
    if (packed_bits)
        w_u8(output++, packed);
    w_u8(output, 0);
    return output;
}

uint32 front_settings_code_character(uint32 code)
{
    FUNCTION_MARKER(0x800AC3A4, "FRONT.OVL");
    return r_u8(0x800BD390 + (uint8)code);
}

void front_xor_bytes(uint32 bytes, uint32 length, uint32 command)
{
    sint32 count = (sint16)length;
    uint8 index = 0, previous = 0x55, value;
    FUNCTION_MARKER(0x800AC1EC, "FRONT.OVL");
    if ((uint8)command == 1)
    {
        while (index < count)
        {
            previous ^= r_u8(bytes);
            w_u8(bytes++, previous);
            ++index;
        }
    }
    else if ((uint8)command == 2)
    {
        while (index < count)
        {
            value = r_u8(bytes);
            w_u8(bytes, r_u8(bytes) ^ previous);
            previous = value;
            ++bytes;
            ++index;
        }
    }
    else
        diagnostic_report(1, 0x80086C70, 0);
}

void front_pack_settings_bits(uint32 settings)
{
    static const uint8 first_shifts[11] = {0, 2, 4, 10, 12, 14, 16, 22, 24, 26, 28};
    static const uint8 second_shifts[8] = {4, 6, 8, 14, 16, 18, 24, 26};
    static const uint8 third_shifts[5] = {4, 6, 16, 18, 24};
    uint32 index, shift, value, packed;
    FUNCTION_MARKER(0x800AAEF8, "FRONT.OVL");
    memset(psx_addr(0x8010D0E8, 12), 0, 12);
    w_u32(0x8010D0E8, r_u32(0x8010D0E8) | 0x3C0);
    w_u32(0x8010D0E8, (r_u32(0x8010D0E8) & 0xFFC3FFFF) | 0x000C0000);
    w_u32(0x8010D0EC, (r_u32(0x8010D0EC) & 0xFFFFFFF0) | 6);
    w_u32(0x8010D0EC, (r_u32(0x8010D0EC) & 0xFFFFC3FF) | 0x1400);
    w_u32(0x8010D0EC, (r_u32(0x8010D0EC) & 0xFF0FFFFF) | 0x00B00000);
    w_u32(0x8010D0F0, (r_u32(0x8010D0F0) & 0xFFFFFFF0) | 7);
    w_u32(0x8010D0F0, (r_u32(0x8010D0F0) & 0xFF0FFFFF) | 0x00E00000);
    w_u8(0x8010D0F1, r_u8(settings + 8));
    for (index = 0; index < 11; ++index)
    {
        value = r_u8(settings + 0x1ED + index) & 3;
        shift = first_shifts[index];
        packed = r_u32(0x8010D0E8);
        w_u32(0x8010D0E8, (packed & ~(3u << shift)) | (value << shift));
    }
    for (index = 0; index < 8; ++index)
    {
        value = r_u8(settings + 0x1F8 + index) & 3;
        shift = second_shifts[index];
        packed = r_u32(0x8010D0EC);
        w_u32(0x8010D0EC, (packed & ~(3u << shift)) | (value << shift));
    }
    value = (r_u8(settings + 0x169) & 3) << 28;
    packed = r_u32(0x8010D0EC);
    w_u32(0x8010D0EC, (packed & 0xCFFFFFFF) | value);
    for (index = 0; index < 5; ++index)
    {
        value = r_u8(settings + 0x16A + index) & 3;
        shift = third_shifts[index];
        packed = r_u32(0x8010D0F0);
        w_u32(0x8010D0F0, (packed & ~(3u << shift)) | (value << shift));
    }
}

uint32 front_decode_settings_code(const char *code)
{
    uint32 output = 0x8010D0E8;
    uint32 remaining = 96;
    uint8 value, packed = 0, bits_read = 0, packed_bits = 0;
    FUNCTION_MARKER(0x800AB75C, "FRONT.OVL");
    if (strlen(code) != 16)
        return 0;
    value = front_settings_code_value((uint8)*code++);
    while (remaining--)
    {
        packed = (uint8)(packed << 1) | ((value & 0x20) >> 5);
        value = (uint8)(value << 1);
        ++bits_read;
        ++packed_bits;
        if (bits_read == 6)
        {
            value = front_settings_code_value((uint8)*code++);
            bits_read = 0;
        }
        if (packed_bits == 8)
        {
            w_u8(output++, packed);
            packed_bits = 0;
            packed = 0;
        }
    }
    front_xor_bytes(0x8010D0E8, 12, 2);
    return front_validate_settings_bits(0x8010D0E8);
}

uint32 front_settings_code_value(uint32 character)
{
    uint32 table = 0x800BD390;
    uint8 index = 0;
    FUNCTION_MARKER(0x800AC3F4, "FRONT.OVL");
    while (r_u8(table))
    {
        if (r_u8(table) == (uint8)character)
            return index;
        ++table;
        ++index;
    }
    return 0xFF;
}

uint32 front_validate_settings_bits(uint32 bits)
{
    FUNCTION_MARKER(0x800AC0C8, "FRONT.OVL");
    if ((r_u32(bits) & 0x3C0) != 0x3C0)
        return 0;
    if ((r_u32(bits) & 0x003C0000) != 0x000C0000)
        return 0;
    if ((r_u32(bits + 4) & 0xF) != 6)
        return 0;
    if ((r_u32(bits + 4) & 0x3C00) != 0x1400)
        return 0;
    if ((r_u32(bits + 4) & 0x00F00000) != 0x00B00000)
        return 0;
    if ((r_u32(bits + 8) & 0xF) != 7)
        return 0;
    if ((r_u32(bits + 8) & 0x00F00000) != 0x00E00000)
        return 0;
    return 1;
}

sint32 front_find_named_code(const char *code)
{
    uint32 index = 0;
    uint32 pointer;
    FUNCTION_MARKER(0x800AC4B8, "FRONT.OVL");
    for (;;)
    {
        pointer = r_u32(0x800BD3E0 + index * 4);
        if (!r_u8(pointer))
            return -1;
        pointer = r_u32(0x800BD3E0 + index * 4);
        if (strcmp(code, (const char *)psx_addr(pointer, 17)) == 0)
            return (sint32)index;
        ++index;
    }
}

uint32 front_apply_settings_code(void)
{
    static const uint8 first_shifts[11] = {0, 2, 4, 10, 12, 14, 16, 22, 24, 26, 28};
    static const uint8 second_shifts[8] = {4, 6, 8, 14, 16, 18, 24, 26};
    static const uint8 third_shifts[5] = {4, 6, 16, 18, 24};
    uint32 index, byte, value;
    FUNCTION_MARKER(0x800AB94C, "FRONT.OVL");
    if (!front_validate_settings_bits(0x8010D0E8))
        return 0;
    for (index = 0; index < 19; ++index)
        for (byte = 0; byte < 7; ++byte)
            w_u8(0x80010490 + index * 7 + byte, 0);
    for (index = 0; index < 8; ++index)
        w_u16(0x8005961E + index * 2, 0);
    w_u32(0x80059630, 0);
    for (index = 0; index < 8; ++index)
        w_u32(0x80011904 + index * 4, 0);
    w_u32(0x80011928, 0);
    w_u32(0x80011924, 0);
    value = r_u8(0x8010D0F1);
    w_u16(0x8005960C, value);
    w_u16(0x800595FC, value);
    w_u16(0x800595F8, value);
    w_u16(0x8005960E, 1);
    w_u16(0x800595FE, 1);
    w_u16(0x800595FA, 1);
    for (index = 0; index < 11; ++index)
        w_u8(0x80010478 + index, (r_u32(0x8010D0E8) >> first_shifts[index]) & 3);
    for (index = 0; index < 8; ++index)
        w_u8(0x80010483 + index, (r_u32(0x8010D0EC) >> second_shifts[index]) & 3);
    w_u8(0x80010F5D, (r_u8(0x8001047B) & 1) || (r_u8(0x8001047C) & 1));
    w_u8(0x80010F5E, (r_u8(0x80010482) & 1) || (r_u8(0x80010484) & 1));
    w_u8(0x80010F5F, (r_u8(0x80010483) & 2) || (r_u8(0x80010484) & 2));
    w_u8(0x80010F60, (r_u8(0x80010486) & 1) || (r_u8(0x80010488) & 1) || (r_u8(0x80010489) & 1));
    w_u8(0x80010F61, (r_u8(0x8001048A) & 1) != 0);
    w_u8(0x80010F62, (r_u8(0x8001048A) & 2) != 0);
    w_u8(0x80010491, (r_u32(0x8010D0EC) >> 28) & 3);
    for (index = 0; index < 5; ++index)
        w_u8(0x80010492 + index, (r_u32(0x8010D0F0) >> third_shifts[index]) & 3);
    front_snapshot_settings(0x80010258);
    if (r_s16(0x8005960C) >= 91)
    {
        w_u8(0x80010518, 0);
        w_u32(0x800F3150, 1);
    }
    else
        w_u32(0x800F3150, 0);
    if ((r_s16(0x800595F8) == 0 && r_s16(0x800595FA) == 0) || r_s16(0x800595F8) == 90)
    {
        w_u16(0x800595FC, 0);
        w_u16(0x800595F8, 0);
        w_u16(0x8005960C, 1);
        w_u16(0x8005960E, 1);
        w_u16(0x800595FE, 1);
        w_u16(0x800595FA, 1);
    }
    return 1;
}

uint32 front_open_card_events(void)
{
    FUNCTION_MARKER(0x8009CE80, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_close_card_events(void)
{
    FUNCTION_MARKER(0x8009D030, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_initialize_card(void)
{
    FUNCTION_MARKER(0x8009E034, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    w_u8(0x800F2D8C, 2);
    w_u8(0x800F2D8D, 2);
    w_u8(0x800F2D8A, 0);
    w_u8(0x800F2D8B, 0);
    w_u32(0x800F315C, 0xFFFFFFFF);
    w_u32(0x800F2F64, 1);
    return 0;
}

uint32 front_shutdown_card(void)
{
    FUNCTION_MARKER(0x8009E084, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

sint32 front_run(void)
{
    uint32 mode, chapter_word, stage_word, first, second;
    uint32 total_a, total_b, total_c, delta_a, delta_b, address;
    sint32 chapter, stage, selected;
    uint32 save_changed = 0;
    FUNCTION_MARKER(0x8008B2D0, "FRONT.OVL");
    front_initialize_video();
    front_initialize_card();
    front_initialize_character_map();
    mode = r_u16(0x80059610);
    if (mode >= 1 && mode <= 4)
    {
        chapter_word = r_u16(0x800595F8);
        delta_a = (uint32)r_s16(0x8005961E);
        total_a = r_u32(0x80011904);
        stage_word = r_u16(0x800595FA);
        total_a += delta_a;
        w_u16(0x80059608, chapter_word);
        w_u16(0x8005960A, stage_word);
        w_u32(0x80011904, total_a);
        delta_a = (uint32)r_s16(0x80059620);
        total_a = r_u32(0x80011908);
        delta_b = (uint32)r_s16(0x80059622);
        total_b = r_u32(0x8001190C);
        total_a += delta_a;
        delta_a = (uint32)r_s16(0x80059624);
        total_b += delta_b;
        w_u32(0x80011908, total_a);
        total_a = r_u32(0x80011910);
        delta_b = (uint32)r_s16(0x80059626);
        w_u32(0x8001190C, total_b);
        total_b = r_u32(0x80011914);
        total_a += delta_a;
        total_b += delta_b;
        delta_a = (uint32)r_s16(0x80059628);
        delta_b = (uint32)r_s16(0x8005962A);
        w_u32(0x80011914, total_b);
        total_b = r_u32(0x8001191C);
        w_u32(0x80011910, total_a);
        total_a = r_u32(0x80011918);
        total_b += delta_b;
        delta_b = (uint32)r_s16(0x8005962C);
        total_a += delta_a;
        w_u32(0x80011918, total_a);
        total_a = r_u32(0x80011920);
        w_u32(0x8001191C, total_b);
        total_b = r_u32(0x80011928);
        total_c = r_u32(0x80059630);
        total_a += delta_b;
        total_b += total_c;
        w_u32(0x80011920, total_a);
        w_u32(0x80011928, total_b);
    }
    if (r_s16(0x80059610) == 2 && r_s16(0x800595FC) == 95 &&
        (r_u8(0x8001047F) & 2) && (r_u8(0x80010487) & 2))
        w_u16(0x800595FC, 96);
    mode = (uint32)r_s16(0x80059610);
    w_u32(0x800F3154, 0);
    if (mode == 0)
    {
        w_u16(0x8005960C, 1);
        w_u16(0x8005960E, 1);
        front_snapshot_settings(0x80010258);
        goto run_menu;
    }
    if (mode == 8)
    {
        front_reset_settings();
        goto run_menu;
    }
    if (mode == 7)
    {
        stage_word = r_u16(0x8005960E);
        chapter_word = r_u16(0x8005960C);
        w_u16(0x800595FE, stage_word);
        w_u16(0x800595FA, stage_word);
        selected = r_s16(0x80011930);
        w_u16(0x800595FC, chapter_word);
        w_u16(0x800595F8, chapter_word);
        if (selected == 1)
            w_u32(0x800F3158, r_u32(0x8003113C) == 0xFFFFFFFF ? 23 : 37);
        else
            w_u32(0x800F3158, 0);
        goto run_menu;
    }
    w_u32(0x800F3158, 38);
    first = r_u32(0x80010248);
    chapter = r_s16(0x800595F8);
    w_u32(0x80010248, first + 1);
    if (chapter == 0)
    {
        stage_word = r_u16(0x800595FA);
        w_u16(0x800595FC, 0);
        w_u32(0x800F3158, 35);
        w_u16(0x800595FE, stage_word + 1);
        if (mode == 5)
        {
            w_u8(0x80010490 + (uint32)(sint32)(sint16)stage_word, 1);
            if (r_s16(0x800595FA) == 6)
            {
                first = r_u16(0x8005960C);
                w_u32(0x800F3158, mode);
                if (first - 1 >= 89)
                {
                    w_u16(0x8005960C, 1);
                    w_u16(0x8005960E, 1);
                }
                first = r_u16(0x8005960C);
                second = r_u16(0x8005960E);
                w_u16(0x800595FC, first);
                w_u16(0x800595FE, second);
            }
        }
    }
    else if (chapter < 90)
    {
        address = 0x80010490 + (uint32)chapter * 7;
        stage = r_s16(0x800595FA);
        address += (uint32)stage;
        w_u8(address, r_u8(address) | (mode == 2 ? 2 : 1));
        selected = r_s16(0x800595FC);
        chapter = r_s16(0x800595F8);
        if (selected != chapter)
        {
            mode = (uint32)r_s16(0x80059610);
            save_changed = 1;
            address = 0x80010478 + (uint32)chapter;
            w_u8(address, r_u8(address) | (mode == 2 ? 2 : 1));
        }
        if (r_s16(0x80011902) >= 2)
        {
            w_u16(0x800595FC, 90);
            w_u16(0x80011902, 0);
        }
        first = r_u16(0x800595FC);
        second = r_u16(0x800595FE);
        w_u16(0x8005960C, first);
        w_u16(0x8005960E, second);
    }
    if (r_s16(0x800595FC) == 90)
    {
        w_u32(0x800F3158, 41);
        w_u32(0x800F3154, 1);
    }
    stage_word = r_u16(0x800595FE);
    chapter_word = r_u16(0x800595FC);
    w_u16(0x800595FA, stage_word);
    w_u16(0x800595F8, chapter_word);
    if (save_changed == 1)
        front_snapshot_settings(0x80010258);
run_menu:
    front_reset_code_text();
    w_u32(0x800F3148, 1);
    sound_reset_voices();
    front_menu_loop();
    front_shutdown_card();
    render_reset();
    return r_s16(0x800F3148);
}

sint32 front_character_glyph(uint32 character)
{
    uint32 table, index, value;
    FUNCTION_MARKER(0x8009683C, "FRONT.OVL");
    character &= 0xFF;
    if (character >= 0xC0 && character <= 0xD6)
        return character - 0x6C;
    if (character >= 0xD8 && character <= 0xDD)
        return character - 0x6D;
    if (character >= 0xE0 && character <= 0xF6)
        return character - 0x6F;
    if (character >= 0xF8 && character <= 0xFD)
        return character - 0x70;
    if (character == 0xFF)
        return 0x8E;
    if (character == 0xDF)
        return 0x8F;
    if (character >= 'A' && character <= 'Z')
        return character - 'A';
    if (character >= 'a' && character <= 'z')
        return character - 0x47;
    if (character >= '0' && character <= '9')
        return character + 4;
    if (character == ' ')
        return -1;
    table = 0x800BC778;
    value = r_u8(table);
    if (!value)
        return -2;
    index = 0;
    while (character != value)
    {
        ++table;
        value = r_u8(table);
        ++index;
        if (!value)
            break;
    }
    if (r_u8(table))
        return index + 0x3E;
    return -2;
}

uint32 front_initialize_character_map(void)
{
    uint32 character;
    FUNCTION_MARKER(0x80096AB4, "FRONT.OVL");
    for (character = 0; character < 256; ++character)
        w_u32(0x8010D168 + character * 4, front_character_glyph(character));
    return 0;
}

uint32 front_reset_code_text(void)
{
    FUNCTION_MARKER(0x800AAEB8, "FRONT.OVL");
    strcpy((char *)psx_addr(0x800FB220, 17), (const char *)psx_addr(0x8001045F, 17));
    return 0x800FB220;
}

void front_free_text_panel(uint32 panel)
{
    uint32 index, lines, line;
    FUNCTION_MARKER(0x80097FF8, "FRONT.OVL");
    if (!panel)
        return;
    lines = r_u32(panel + 0x2C);
    if (lines)
    {
        index = 0;
        if (r_s32(panel + 0x1C) > 0)
        {
            do
            {
                lines = r_u32(panel + 0x2C);
                line = r_u32(lines + index * 4);
                if (line)
                    heap_free(line);
                ++index;
            } while ((sint32)index < r_s32(panel + 0x1C));
        }
        heap_free(r_u32(panel + 0x2C));
    }
    heap_free(panel);
}

uint32 front_create_text_panel(const PSX_RECT *rectangle, uint32 style, uint32 color)
{
    uint32 panel, lines, line, slot, index;
    sint32 height, line_height, count;
    uint32 failed = 0;
    FUNCTION_MARKER(0x80098094, "FRONT.OVL");
    panel = heap_alloc_back(0x30);
    if (!panel)
        return 0;
    w_u32(panel, (sint32)rectangle->x);
    w_u32(panel + 4, (sint32)rectangle->y);
    w_u32(panel + 8, (sint32)rectangle->w);
    height = rectangle->h;
    w_u32(panel + 0x14, color);
    w_u32(panel + 0x24, 0);
    w_u32(panel + 0x28, 0);
    w_u32(panel + 0x10, style);
    w_u32(panel + 0xC, height);
    line_height = r_s16(0x8004EB7A + (style << 4));
    height = r_s32(panel + 0xC);
    ++line_height;
    if (!line_height)
        abort();
    count = height / line_height;
    w_u32(panel + 0x20, line_height);
    w_u32(panel + 0x18, count);
    w_u32(panel + 0x1C, (uint32)count << 1);
    lines = heap_alloc_back((uint32)count << 3);
    w_u32(panel + 0x2C, lines);
    if (!lines)
        failed = 1;
    else if (r_s32(panel + 0x1C) > 0)
    {
        index = 0;
        do
        {
            line = heap_alloc_back(50);
            lines = r_u32(panel + 0x2C);
            slot = lines + index * 4;
            w_u32(slot, line);
            lines = r_u32(panel + 0x2C);
            line = r_u32(lines + index * 4);
            if (!line)
                failed = 1;
            else
                w_u8(line, 0);
            ++index;
        } while ((sint32)index < r_s32(panel + 0x1C));
    }
    if (failed == 1)
    {
        front_free_text_panel(panel);
        return 0;
    }
    return panel;
}

void front_menu_loop(void)
{
    char path[32];
    PSX_RECT rectangle;
    uint32 allocation, first, second, third, pointer, object, target;
    uint32 old_buttons, buttons, controller, panel, voice, flags;
    uint32 source_a, source_b, words[3], offset, index;
    sint32 chapter, next_screen, idle_frames, ready, text_y;
    sint32 selected, cycle;
    FUNCTION_MARKER(0x8008E504, "FRONT.OVL");
    w_u32(0x80053670, 0);
    allocation = heap_alloc_back(0x22000);
    w_u32(0x800FB0EC, allocation);
    resource_load_into("FEGFX.INF", 0x8004EB70);
    resource_load_into("FEGFX.BOT", r_u32(0x800FB0EC));
    allocation = resource_load_sector_index("DATABASE.INF", "DATABASE.PAK");
    w_u32(0x800F708C, allocation);
    allocation = resource_load_sector_index("MISSIONS.INF", "MISSIONS.PAK");
    w_u32(0x800FB0E8, allocation);
    memset(psx_addr(0x800FB748, 1), 0, 1);
    w_u32(0x800313D0, 0);
    w_u32(0x800F313C, 0);
    w_u32(0x800F3140, 0);
    w_u32(0x800F3148, 1);
    stream_init();
    if (!r_u8(0x80010518))
    {
        front_intro();
        w_u32(0x800BD370, 0);
    }
    else
        w_u32(0x800BD370, 1);
    chapter = r_s16(0x800595F8);
    if (chapter >= 91)
    {
        pointer = r_u32(0x800B71F4 + (uint32)chapter * 4);
        front_play_movie_with_sound_reset((const char *)psx_addr(pointer, 16), 30);
        VSync(0);
        SetDispMask(0);
        chapter = r_s16(0x800595F8);
        w_u8(0x80010F02 + (uint32)chapter, 1);
    }
    front_initialize_display();
    front_initialize_screens();
    first = r_u32(0x800F3158);
    second = r_u32(0x800595F8);
    w_u16(0x80114264, first);
    if (!second || r_s16(0x800595F8) == 90)
    {
        w_u16(0x800595FC, 0);
        w_u16(0x800595F8, 0);
        w_u16(0x8005960C, 1);
        w_u16(0x8005960E, 1);
        w_u16(0x800595FE, 1);
        w_u16(0x800595FA, 1);
    }
    if (r_s16(0x800595F8) >= 91)
    {
        front_snapshot_settings(0x80010258);
        w_u8(0x80010518, 0);
        w_u32(0x800F3150, 1);
    }
    else
    {
        chapter = r_s16(0x800595FC);
        selected = r_s16(0x800595FE);
        sprintf(path, "%d_%d.CW", chapter, selected);
        if (resource_exists(path) == 1)
        {
            strcpy((char *)psx_addr(0x80010518, sizeof(path)), path);
            first = r_u16(0x800595FC);
            second = r_u16(0x800595FE);
            w_u16(0x800595F8, first);
            w_u16(0x800595FA, second);
        }
        else
        {
            first = r_u32(0x8008673C);
            second = (uint32)r_s16(0x80086740);
            third = (uint32)r_s8(0x80086742);
            w_u32(0x80010518, first);
            w_u16(0x8001051C, second);
            w_u8(0x8001051E, third);
            w_u16(0x800595FC, 1);
            w_u16(0x800595F8, 1);
            w_u16(0x800595FE, 1);
            w_u16(0x800595FA, 1);
        }
    }
    w_u8(0x800FB10C, 1);
    front_initialize_layers();
    front_configure_layer(0, 0, 0xA9);
    front_configure_layer(1, 3, 0xA8);
    stream_init();
    w_u32(0x800F3080, 1);
    w_u16(0x800F312A, 1);
    w_u16(0x800F3114, 1);
    w_u16(0x800F3116, 1);
    w_u16(0x800F3118, 1);
    rectangle.x = 424;
    rectangle.y = 42;
    rectangle.w = 194;
    w_u32(0x800F30F4, 0);
    w_u16(0x800F311A, 0);
    rectangle.h = 38;
    panel = front_create_text_panel(&rectangle, 0, 0x505060);
    w_u32(0x800F312C, panel);
    w_u16(0x800F311C, 0);
    first = VSync(-1);
    second = r_u32(0x800311C0);
    w_u32(0x800F3124, first);
    w_u32(0x800F3120, 0);
    w_u16(0x800F3128, second ? 50 : 60);
    first = r_u32(0x80042970);
    second = r_u32(0x800313A0);
    voice = sound_reserve_voice(first, 50, 15, 15, 256, second);
    first = r_u32(0x80042970);
    second = r_u32(0x800313A0);
    w_u32(0x800F3130, voice);
    voice = sound_reserve_voice(first, 50, 15, 15, 137, second);
    w_u32(0x800F3134, voice);
    w_u32(0x800FB870, 253);
    front_initialize_render_buffers(300, 2000);
    front_configure_background(8, 7);
    front_initialize_background();
    w_u32(0x8010D0E0, 0);
    w_u32(0x800FB110, 0);
    w_u16(0x801132A4, 0);
    w_u16(0x800B737C, 0);
    w_u32(0x800313D0, 0);
    w_u32(0x800F314C, 1);
    VSyncCallbackPSX(0x8008DA54);
    disc_wait_stream();
    VSync(0);
    while (r_u8(0x800FB10C) == 1)
    {
        if (r_u32(0x800F3150) == 1 && r_s16(0x80114264) == 26 && !r_u32(0x800BD424))
            w_u16(0x80114264, 36);
        front_select_screen(r_s16(0x80114264));
        if (r_u8(0x8010D0DE) == 1)
        {
            target = r_u32(0x800FB114);
            psx_dispatch_guest_call(target, 0, 0, 0, 0);
        }
        object = r_u32(0x800FB110);
        while (object)
        {
            target = r_u32(object + 0x1C);
            psx_dispatch_guest_call(target, 1, object, 0, 0);
            object = r_u32(object + 0x10);
        }
        pointer = r_u32(0x8010D0E0);
        object = r_u32(0x800FB110);
        target = r_u32(0x800FB234);
        flags = r_u8(0x8010D160);
        w_u8(0x800B7384, 0);
        w_u32(0x8010D0E0, object);
        w_u32(0x800FB938, target);
        w_u8(0x800B7384, flags);
        ready = 0;
        if (pointer)
            front_free_screen_list(pointer);
        idle_frames = 0;
        w_u16(0x801132A4, 0);
        w_u16(0x800B737C, 0);
        w_u32(0x800F314C, 0);
        do
        {
            if (idle_frames == 2)
            {
                target = r_u32(0x80113200);
                psx_dispatch_guest_call(target, 0, 0, 0, 0);
            }
            if (!r_s16(0x800B737C) && r_u8(0x800FB10C) == 1 && ready == 1 && r_u8(0x800FB0F0))
            {
                if (idle_frames < 2)
                    ++idle_frames;
            }
            else
                idle_frames = 0;
            first = VSync(-1);
            w_u32(0x800F30F4, first);
            VSync(0);
            ResetGraph(1);
            PutDrawEnv((DRAWENV *)psx_addr(0x800FB878, sizeof(DRAWENV)));
            front_swap_render_buffers();
            front_update_screen();
            w_u32(0x800FB870, 253);
            if (r_u32(0x80040FE8) != 12)
            {
                sound_stop_voice(r_u32(0x800F3130));
                sound_tick();
                sound_stop_voice(r_u32(0x800F3134));
                sound_tick();
                disc_prompt();
                first = r_u32(0x80042970);
                second = r_u32(0x800313A0);
                voice = sound_reserve_voice(first, 50, 15, 15, 256, second);
                first = r_u32(0x80042970);
                second = r_u32(0x800313A0);
                w_u32(0x800F3130, voice);
                voice = sound_reserve_voice(first, 50, 15, 15, 137, second);
                w_u32(0x800F3134, voice);
            }
            front_render_begin();
            front_draw_text_panel(r_u32(0x800F312C));
            w_u32(0x800FB870, 0);
            front_update_connection_label();
            text_y = r_u32(0x800311C0) ? 472 : 440;
            if (r_u8(0x800F3084))
                front_draw_colored_text(0x800F3084, 50, text_y, 0x808080);
            object = r_u32(0x8010D0E0);
            if (object)
                front_draw_screen_list(object);
            front_draw_background();
            if (!r_s16(0x800B737C) && r_u32(0x80040FE8) == 12)
            {
                object = r_u32(0x8010D0E0);
                ready = 1;
                while (object)
                {
                    if (r_s16(object + 0x2C) == 1)
                        ready = 0;
                    object = r_u32(object + 0x10);
                }
                if (ready == 1)
                {
                    next_screen = front_next_screen();
                    if ((sint16)next_screen != -1)
                    {
                        w_u16(0x80114264, next_screen);
                        w_u16(0x800B737C, 1);
                    }
                }
            }
            if (r_u8(0x8010D160) == 1)
            {
                target = r_u32(0x800FB234);
                psx_dispatch_guest_call(target, 0, 0, 0, 0);
            }
            if (r_s16(0x800B737C) == 1 || !r_u8(0x800FB10C))
                w_u16(0x801132A4, 1);
            object = r_u32(0x8010D0E0);
            while (object)
            {
                target = r_u32(object + 0x20);
                psx_dispatch_guest_call(target, 1, object, 0, 0);
                if (r_s16(object + 0x2C) != 2)
                    w_u16(0x801132A4, 0);
                object = r_u32(object + 0x10);
            }
            old_buttons = r_u32(0x8004616C);
            w_u32(0x80053678, old_buttons);
            buttons = pad_update(0);
            old_buttons = r_u32(0x80053678);
            controller = r_u32(0x80040FE8);
            w_u32(0x8004616C, buttons);
            w_u32(0x80053684, (buttons ^ old_buttons) & buttons);
            if (controller != 12)
            {
                w_u32(0x8004616C, 0);
                w_u32(0x80053684, 0);
            }
        } while (!r_s16(0x801132A4));
        flags = r_u8(0x8010D0F4);
        w_u32(0x800F314C, 1);
        if (flags == 1)
        {
            target = r_u32(0x800FB118);
            psx_dispatch_guest_call(target, 0, 0, 0, 0);
        }
        object = r_u32(0x800FB110);
        while (object)
        {
            target = r_u32(object + 0x24);
            psx_dispatch_guest_call(target, 1, object, 0, 0);
            object = r_u32(object + 0x10);
        }
        if (!r_s16(0x80114264) && r_u32(0x800F3140))
        {
            selected = r_s32(0x800313C8);
            if (selected == 4)
            {
                front_play_intro_from_menu();
                w_u8(0x80010518, 0);
                w_u16(0x800595F8, 1);
                w_u16(0x800595FA, 1);
            }
            else if (selected == 3)
            {
                w_u32(0x800F3148, 7);
                w_u16(0x8001192C, 0);
                w_u16(0x8001192E, 0);
                w_u8(0x800FB10C, 0);
            }
            else
            {
                first = r_u32(0x800313CC) + 1;
                w_u16(0x8001192C, 0);
                w_u16(0x8001192E, 0);
                w_u8(0x800FB10C, 0);
                w_u32(0x800F3148, 6);
                cycle = (sint32)first % 5;
                w_u32(0x800313CC, cycle);
                w_u32(0x8003113C, cycle);
            }
            first = r_u32(0x800313C8);
            w_u32(0x800F3140, 0);
            w_u32(0x800313D0, 0);
            if (first)
                w_u32(0x800313C8, first + 1);
            if (r_s32(0x800313C8) >= 5)
                w_u32(0x800313C8, 1);
            w_u16(0x80011930, 0);
        }
    }
    front_close_display();
    front_free_screen_list(r_u32(0x800FB110));
    front_close_background();
    front_close_render_buffers();
    front_free_text_panel(r_u32(0x800F312C));
    allocation = r_u32(0x800F708C);
    source_a = r_u32(0x800FB0E8);
    source_b = r_u32(0x800FB0E8);
    for (offset = 0; offset < 24; offset += 12)
    {
        for (index = 0; index < 3; ++index)
            words[index] = r_u32(source_a + 0x1B8 + offset + index * 4);
        for (index = 0; index < 3; ++index)
            w_u32(0x800105B8 + offset + index * 4, words[index]);
    }
    for (offset = 0; offset < 24; offset += 12)
    {
        for (index = 0; index < 3; ++index)
            words[index] = r_u32(source_b + 0x158 + offset + index * 4);
        for (index = 0; index < 3; ++index)
            w_u32(0x800105A0 + offset + index * 4, words[index]);
    }
    heap_free(allocation);
    allocation = r_u32(0x800FB0E8);
    if (allocation != r_u32(0x800F708C))
        heap_free(allocation);
    stream_stop();
    stream_cancel_current();
    for (index = 0; index < 20; ++index)
        VSync(0);
    VSync(0);
    VSync(0);
    VSyncCallbackPSX(0);
    sound_stop_voice(r_u32(0x800F3130));
    sound_tick();
    sound_stop_voice(r_u32(0x800F3134));
    sound_tick();
    sound_tick();
    sound_tick();
    w_u32(0x80041EF0, 0);
}

uint32 front_initialize_layers(void)
{
    FUNCTION_MARKER(0x800B25D0, "FRONT.OVL");
    w_u8(0x800FB922, 128);
    w_u8(0x800FB921, 128);
    w_u8(0x800FB920, 128);
    w_u32(0x800FB934, 15);
    w_u32(0x800FB8FC, 0);
    w_u32(0x800FB8F8, 0);
    w_u8(0x800FB926, 0);
    w_u8(0x800FB925, 0);
    w_u8(0x800FB924, 0);
    w_u8(0x800FB92A, 0);
    w_u8(0x800FB929, 0);
    w_u8(0x800FB928, 0);
    w_u32(0x800FB92C, 0);
    w_u32(0x800FB930, 0);
    w_u32(0x800FB914, 0);
    w_u32(0x800FB910, 0);
    w_u32(0x800FB91C, 0);
    w_u32(0x800FB918, 0);
    w_u32(0x800BE638, 0x800F29E4);
    return 0x800F29E4;
}

uint32 front_configure_layer(uint32 layer, uint32 mode, uint32 value)
{
    uint32 offset = layer ? 4 : 0;
    uint32 previous, current;
    FUNCTION_MARKER(0x800B2680, "FRONT.OVL");
    previous = r_u32(0x800FB910 + offset);
    if (previous != 0xFFFFFFFF)
        w_u32(0x800FB900 + offset, previous);
    current = r_u32(0x800FB8F8 + offset);
    if (value != current)
        w_u32(0x800FB8F0 + offset, value);
    if (previous == mode)
        w_u32(0x800FB910 + offset, mode);
    w_u32(0x800FB908 + offset, mode);
    return current;
}

void front_free_screen_list(uint32 list)
{
    uint32 type = 28;
    uint32 kind, next, old;
    FUNCTION_MARKER(0x8008D418, "FRONT.OVL");
    while (list)
    {
        kind = (uint32)(sint32)(sint16)(type - 19);
        switch (kind)
        {
        case 0:
        case 2:
        case 5:
        case 6:
        case 9:
        case 11:
            old = list;
            type = r_u16(list);
            next = r_u32(list + 4);
            heap_free(old);
            list = next;
            break;
        default:
            diagnostic_report(100, 0x80086668, 0);
            break;
        }
    }
}

uint32 front_initialize_screens(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8008D4C0, "FRONT.OVL");
    for (index = 0; index < 44; ++index)
        w_u16(0x800FB7A6 - index * 2, 31);
    return 0x800FB74E;
}

void front_render_begin(void)
{
    uint32 pal, channel, offset, texture, pending, mode, requested, table;
    uint32 fade, step, component, intensity, product, colors[3], rgba;
    uint32 pointer, x, width;
    sint32 denominator, half_width, label_y, sprite_y;
    uint32 changed;
    FUNCTION_MARKER(0x800B2748, "FRONT.OVL");
    pal = r_u32(0x800311C0);
    w_u32(0x800BE638, 0x800F29E4);
    sprite_y = pal == 1 ? 453 : 421;
    label_y = pal == 1 ? 448 : 416;
    fade = r_u32(0x800FB92C);
    w_u32(0x800FB870, 83);
    for (channel = 0; channel < 2; ++channel)
    {
        offset = channel * 4;
        if (channel)
            fade = r_u32(0x800FB930);
        if (!fade)
        {
            texture = r_u32(0x800FB8F0 + offset);
            mode = r_u32(0x800FB908 + offset);
            w_u32(0x800FB8F8 + offset, texture);
            w_u32(0x800FB910 + offset, mode);
            if ((sint32)mode >= 0)
            {
                table = r_u32(0x800BE638);
                pointer = r_u32(table + mode * 4);
                width = front_text_width(pointer);
                half_width = (sint32)width / 2;
                w_u32(0x800FB918 + offset, (channel ? 459u : 351u) - (uint32)half_width);
            }
        }
        texture = r_u32(0x800FB8F8 + offset);
        pending = r_u32(0x800FB8F0 + offset);
        changed = texture != pending;
        if (!changed)
        {
            mode = r_u32(0x800FB910 + offset);
            requested = r_u32(0x800FB908 + offset);
            changed = mode != requested;
            if (!changed)
                changed = r_u32(0x800BE638) != 0x800F29E4;
        }
        if (changed)
        {
            fade = r_u32(0x800FB92C + offset);
            intensity = r_u8(0x800FB920);
            step = fade - 1;
            product = step * intensity;
            denominator = r_s32(0x800FB934);
            if (!denominator || (denominator == -1 && product == 0x80000000))
                abort();
            colors[0] = (sint32)product / denominator;
            for (component = 1; component < 3; ++component)
            {
                intensity = r_u8(0x800FB920 + component);
                product = step * intensity;
                if (!denominator || (denominator == -1 && product == 0x80000000))
                    abort();
                colors[component] = (sint32)product / denominator;
            }
            w_u32(0x800FB92C + offset, step);
            w_u8(0x800FB924 + offset, colors[0]);
            w_u8(0x800FB925 + offset, colors[1]);
            w_u8(0x800FB926 + offset, colors[2]);
        }
        else
        {
            fade = r_u32(0x800FB92C + offset);
            denominator = r_s32(0x800FB934);
            if ((sint32)fade < denominator && texture != 0xFFFFFFFF && mode != 0xFFFFFFFF)
            {
                step = fade + 1;
                for (component = 0; component < 3; ++component)
                {
                    intensity = r_u8(0x800FB920 + component);
                    product = step * intensity;
                    if (!denominator || (denominator == -1 && product == 0x80000000))
                        abort();
                    colors[component] = (sint32)product / denominator;
                }
                w_u32(0x800FB92C + offset, step);
                w_u8(0x800FB924 + offset, colors[0]);
                w_u8(0x800FB925 + offset, colors[1]);
                w_u8(0x800FB926 + offset, colors[2]);
            }
        }
    }
    for (channel = 0; channel < 2; ++channel)
    {
        offset = channel * 4;
        texture = r_u32(0x800FB8F8 + offset);
        if (texture != 0xFFFFFFFF)
        {
            rgba = r_u8(0x800FB924 + offset);
            rgba |= (uint32)r_u8(0x800FB925 + offset) << 8;
            rgba |= (uint32)r_u8(0x800FB926 + offset) << 16;
            rgba |= (uint32)r_u8(0x800FB927 + offset) << 24;
            front_draw_sprite(texture, channel ? 517 : 292, sprite_y, rgba);
        }
        mode = r_u32(0x800FB910 + offset);
        if (mode != 0xFFFFFFFF)
        {
            x = r_u32(0x800FB918 + offset);
            table = r_u32(0x800BE638);
            rgba = r_u32(0x800FB924 + offset);
            pointer = r_u32(table + mode * 4);
            front_draw_colored_text(pointer, x, label_y, rgba);
        }
    }
}

typedef struct FRONT_MOVIE_PATH
{
    const char *original_name;
    const char *runtime_path;
} FRONT_MOVIE_PATH;

static const FRONT_MOVIE_PATH front_movie_paths[] = {
    {"ACT/ACT1.STR", "DATA/1/ACT/ACT1.STR"},
    {"ACT/ACT10.STR", "DATA/2/ACT/ACT10.STR"},
    {"ACT/ACT11.STR", "DATA/2/ACT/ACT11.STR"},
    {"ACT/ACT12.STR", "DATA/2/ACT/ACT12.STR"},
    {"ACT/ACT13.STR", "DATA/2/ACT/ACT13.STR"},
    {"ACT/ACT14.STR", "DATA/2/ACT/ACT14.STR"},
    {"ACT/ACT15.STR", "DATA/2/ACT/ACT15.STR"},
    {"ACT/ACT16.STR", "DATA/2/ACT/ACT16.STR"},
    {"ACT/ACT17.STR", "DATA/2/ACT/ACT17.STR"},
    {"ACT/ACT18.STR", "DATA/2/ACT/ACT18.STR"},
    {"ACT/ACT2.STR", "DATA/1/ACT/ACT2.STR"},
    {"ACT/ACT3.STR", "DATA/1/ACT/ACT3.STR"},
    {"ACT/ACT4.STR", "DATA/1/ACT/ACT4.STR"},
    {"ACT/ACT5.STR", "DATA/1/ACT/ACT5.STR"},
    {"ACT/ACT6.STR", "DATA/1/ACT/ACT6.STR"},
    {"ACT/ACT7.STR", "DATA/1/ACT/ACT7.STR"},
    {"ACT/ACT8.STR", "DATA/1/ACT/ACT8.STR"},
    {"ACT/ACT9.STR", "DATA/2/ACT/ACT9.STR"},
    {"END/END1.STR", "DATA/1/END/END1.STR"},
    {"END/END2.STR", "DATA/2/END/END2.STR"},
    {"END/END3.STR", "DATA/2/END/END3.STR"},
    {"END/END4.STR", "DATA/2/END/END4.STR"},
    {"END/END5.STR", "DATA/2/END/END5.STR"},
    {"END/END6.STR", "DATA/2/END/END6.STR"},
    {"INTRO.STR", "DATA/1/INTRO.STR"},
    {"LOGO.STR", "DATA/1/LOGO.STR"},
};

const char *game_movie_resolve_path(const char *path)
{
    char name[64];
    size_t length, index;
    if (path == NULL)
        return NULL;
    while (*path == '/' || *path == '\\')
        ++path;
    length = strcspn(path, ";");
    if (length == 0 || length >= sizeof(name))
        return NULL;
    if (path[length] != 0 && strcmp(path + length, ";1") != 0)
        return NULL;
    for (index = 0; index < length; ++index)
        name[index] = path[index] == '\\' ? '/' : path[index];
    name[length] = 0;
    for (index = 0; index < sizeof(front_movie_paths) / sizeof(front_movie_paths[0]); ++index)
        if (strcmp(name, front_movie_paths[index].original_name) == 0)
            return front_movie_paths[index].runtime_path;
    return NULL;
}

static void front_movie_build_presentation(XportFmvPresentation *presentation)
{
    memset(presentation, 0, sizeof(*presentation));
    presentation->width = 320;
    presentation->height = 240;
    presentation->video_y = 24 + (r_u32(0x800311C0) != 0 ? 8 : 4);
    presentation->screen_x = (sint16)r_u32(0x80010564);
    presentation->screen_y = (sint16)r_u32(0x80010568);
}

typedef struct FRONT_MOVIE_PRESERVED_PAGES
{
    uint32 *pixels;
    uint32 restored;
} FRONT_MOVIE_PRESERVED_PAGES;

static sint32 front_movie_save_pages(FRONT_MOVIE_PRESERVED_PAGES *state, uint32 *pixels)
{
    PSX_RECT rectangle = {0, 0, 480, 496};
    state->pixels = pixels;
    state->restored = 0;
    return StoreImage(&rectangle, pixels) == 0;
}

static sint32 front_movie_restore_background(void *context, uint32 source_number)
{
    FRONT_MOVIE_PRESERVED_PAGES *state = (FRONT_MOVIE_PRESERVED_PAGES *)context;
    PSX_RECT rectangle = {0, 0, 480, 240};
    if (state->restored != 0)
        return 1;
    if (LoadImagePSX(&rectangle, state->pixels) < 0)
        return 0;
    rectangle.y = 240;
    if (LoadImagePSX(&rectangle, state->pixels + 256 * 240) < 0)
        return 0;
    state->restored = 1;
    return 1;
}

static sint32 front_movie_restore_page_layout(FRONT_MOVIE_PRESERVED_PAGES *state)
{
    PSX_RECT rectangle = {0, 0, 480, 240};
    if (state->restored == 0)
        return 1;
    if (StoreImage(&rectangle, state->pixels) < 0)
        return 0;
    rectangle.y = 240;
    if (StoreImage(&rectangle, state->pixels + 256 * 240) < 0)
        return 0;
    rectangle.y = 0;
    if (LoadImagePSX(&rectangle, state->pixels) < 0)
        return 0;
    rectangle.y = 256;
    if (LoadImagePSX(&rectangle, state->pixels + 256 * 240) < 0)
        return 0;
    rectangle.y = 240;
    rectangle.h = 16;
    return LoadImagePSX(&rectangle, state->pixels + 240 * 240) == 0;
}

static void front_movie_clear_after_playback(uint32 height)
{
    PSX_RECT rectangle;
    SetDispMask(0);
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 640;
    rectangle.h = 256;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    rectangle.x = 0;
    rectangle.y = 256;
    rectangle.w = 640;
    rectangle.h = (sint16)height;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);
}

typedef struct FRONT_MOVIE_CONTEXT
{
    FRONT_MOVIE_PRESERVED_PAGES pages;
    DISPENV last_display;
    uint32 presented;
    uint32 source_started;
} FRONT_MOVIE_CONTEXT;

static sint32 front_movie_source_frame(void *context, uint32 source_number)
{
    FRONT_MOVIE_CONTEXT *state = (FRONT_MOVIE_CONTEXT *)context;
    if (state->pages.pixels != NULL && !front_movie_restore_background(&state->pages, source_number))
        return 0;
    if (state->source_started == 0) {
        w_u32(0x80113204, VSync(-1));
        state->source_started = 1;
    }
    w_u16(0x80113298, source_number);
    return 1;
}

static sint32 front_movie_capture_display(void *context)
{
    FRONT_MOVIE_CONTEXT *state = (FRONT_MOVIE_CONTEXT *)context;
    if (xport_fmv_ready()) {
        if (!gpu_get_display_env(&state->last_display))
            return 0;
        state->presented = 1;
    }
    return 1;
}

static sint32 front_movie_finish(void *context, uint32 step)
{
    return front_movie_capture_display(context) ? 0 : -1;
}

uint32 front_play_movie(const char *path)
{
    FRONT_MOVIE_CONTEXT state;
    XportFmvPresentation presentation;
    const char *runtime_path = game_movie_resolve_path(path);
    uint32 clear_height = r_u32(0x800311C0) != 0 ? 256 : 224;
    uint32 final_frame;
    sint32 limit, result;
    if (runtime_path == NULL)
        return 0;
    memset(&state, 0, sizeof(state));
    front_set_movie_limit(path);
    limit = r_s16(0x8011329A);
    final_frame = limit < 0 ? 1 : (uint32)limit + 1;
    w_u16(0x8010CDA8, 1);
    w_u32(0x8010DA6C, 0);
    w_u16(0x80113298, 0);
    front_movie_build_presentation(&presentation);
    presentation.context = &state;
    presentation.frame = front_movie_source_frame;
    presentation.tick = front_movie_capture_display;
    presentation.finish = front_movie_finish;
    presentation.controllers = 1;
    presentation.skip_buttons = PADstart | PADRdown;
    if (r_u8(0x800FB93C) == 0) {
        state.pages.pixels = (uint32 *)malloc(119040 * sizeof(uint32));
        if (state.pages.pixels == NULL)
            return 0;
        DrawSync(0);
        if (!front_movie_save_pages(&state.pages, state.pages.pixels)) {
            free(state.pages.pixels);
            return 0;
        }
    }
    result = xport_fmv_play_presented(runtime_path, final_frame, 0, &presentation);
    if (!game_mount_stream_source())
        result = -1;
    if (state.pages.pixels != NULL) {
        if (!front_movie_restore_page_layout(&state.pages))
            result = -1;
        free(state.pages.pixels);
    }
    if (state.presented != 0) {
        if (state.last_display.disp.y == 240)
            state.last_display.disp.y = 256;
        PutDispEnv(&state.last_display);
        SetDispMask(1);
    }
    if (r_u8(0x800FB93C) != 0 && state.source_started != 0)
        front_movie_clear_after_playback(clear_height);
    if (result < 0)
        return 0;
    w_u32(0x8010DA6C, result == 2 ? 2 : 1);
    return 1;
}
