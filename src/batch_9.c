#include "game_functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void front_initialize_glyph_widths(void)
{
    uint32 offset, width;
    sint32 depth;
    FUNCTION_MARKER(0x800967D4, "FRONT.OVL");
    for (offset = 0; offset < 0x1E40; offset += 16)
    {
        if (!r_s16(0x8004EB7E + offset))
        {
            depth = r_s16(0x8004EB7C + offset);
            width = (uint32)r_s16(0x8004EB78 + offset);
            w_u16(0x8004EB7E + offset, width << ((2 - (uint32)depth) & 31));
        }
    }
}

void front_draw_background(void)
{
    uint32 columns, points, phase, packets, colors, atlas, shift, row, column;
    uint32 offset, component, delta_x, delta_y, square, intensity, light, value;
    uint32 base_index, index, color, packet, word, last_color, x0, x1, y0, y1;
    uint32 u0, u1, v0, v1, table, count, old, inactive;
    sint32 top, bottom, divisor, remainder, y_offset, exponent, depth, width, height;
    uint16 page;
    FUNCTION_MARKER(0x8009AF20, "FRONT.OVL");
    columns = r_u32(0x800FB854);
    points = r_u32(0x800FB864);
    bottom = r_s16(points + (columns << 2) + 6);
    top = r_s16(points + 2);
    phase = r_u32(0x800FB820);
    divisor = bottom - top;
    if (!divisor || (divisor == -1 && phase == 0x80000000))
        abort();
    remainder = (sint32)phase % divisor;
    packets = r_u32(0x800FB85C);
    points = r_u32(0x800FB864);
    colors = r_u32(0x800FB868);
    y_offset = divisor - remainder;
    atlas = 0x8004EB70 + (((phase & 15) + 449) << 4);
    depth = r_s16(atlas + 12);
    shift = (2 - (uint32)depth) & 31;
    if (r_s32(0x800FB858) >= 0)
    {
        row = 0;
        do
        {
            top = r_s16(points + 2);
            columns = r_u32(0x800FB854);
            y0 = (uint32)top + (uint32)y_offset;
            if ((sint32)columns >= 0)
            {
                column = 0;
                do
                {
                    x0 = (uint32)r_s16(points);
                    w_u8(colors, 8);
                    w_u8(colors + 1, 20);
                    w_u8(colors + 2, 16);
                    for (offset = 0; offset < 48; offset += 16)
                    {
                        exponent = r_s16(0x800FB828 + offset);
                        if (exponent == -1)
                            continue;
                        value = (uint32)r_s16(0x800FB824 + offset);
                        delta_x = x0 - value;
                        square = delta_x * delta_x;
                        value = (uint32)r_s16(0x800FB826 + offset);
                        delta_y = y0 - value;
                        square += delta_y * delta_y;
                        intensity = 176 - (uint32)((sint32)square >> ((uint32)exponent & 31));
                        if ((sint32)intensity < 0)
                            intensity = 0;
                        for (component = 0; component < 3; ++component)
                        {
                            light = r_u8(0x800FB830 + offset + component);
                            value = r_u8(colors + component);
                            square = intensity * light;
                            value += (uint32)((sint32)square >> 8);
                            w_u8(colors + component, value);
                        }
                    }
                    points += 4;
                    ++column;
                    columns = r_u32(0x800FB854);
                    colors += 4;
                } while ((sint32)column <= (sint32)columns);
            }
            ++row;
        } while ((sint32)row <= r_s32(0x800FB858));
    }
    if (r_s32(0x800FB858) > 0)
    {
        row = 0;
        do
        {
            columns = r_u32(0x800FB854);
            base_index = row * (columns + 1);
            points = r_u32(0x800FB864);
            top = r_s16(points + (base_index << 2) + 2);
            bottom = r_s16(points + ((base_index + columns) << 2) + 6);
            y0 = (uint32)top + (uint32)y_offset;
            y1 = (uint32)bottom + (uint32)y_offset;
            if ((sint32)columns > 0)
            {
                column = 0;
                do
                {
                    packet = packets;
                    index = (base_index + column) << 2;
                    color = r_u32(0x800FB868);
                    points = r_u32(0x800FB864);
                    color += index;
                    points += index;
                    value = r_u8(color);
                    x0 = (uint32)r_s16(points);
                    x1 = (uint32)r_s16(points + 4);
                    w_u8(packet + 4, value);
                    value = r_u8(color + 1);
                    w_u8(packet + 5, value);
                    value = r_u8(color + 2);
                    color += 4;
                    w_u8(packet + 6, value);
                    word = r_u32(color);
                    w_u32(packet + 16, word);
                    columns = r_u32(0x800FB854);
                    color += columns << 2;
                    word = r_u32(color);
                    w_u32(packet + 28, word);
                    last_color = r_u32(color + 4);
                    ++column;
                    w_u16(packet + 8, x0);
                    w_u16(packet + 10, y0);
                    w_u16(packet + 20, x1);
                    w_u16(packet + 22, y0);
                    w_u16(packet + 32, x0);
                    w_u16(packet + 34, y1);
                    w_u16(packet + 44, x1);
                    w_u16(packet + 46, y1);
                    w_u32(packet + 40, last_color);
                    u0 = r_u16(atlas);
                    v0 = r_u8(atlas + 2);
                    width = r_s16(atlas + 8);
                    height = r_s16(atlas + 10);
                    u0 = (u0 & 63) << shift;
                    u1 = u0 + ((uint32)width << shift) - 1;
                    w_u8(packet + 12, u0);
                    w_u8(packet + 13, v0);
                    w_u8(packet + 24, u1);
                    w_u8(packet + 25, v0);
                    v1 = v0 + (uint32)height;
                    w_u8(packet + 36, u0);
                    w_u8(packet + 37, v1);
                    w_u8(packet + 48, u1);
                    w_u8(packet + 49, v1);
                    top = r_s16(atlas);
                    bottom = r_s16(atlas + 2);
                    page = GetTPage(depth, 1, top, bottom);
                    w_u16(packet + 26, page);
                    count = r_u32(0x801132A8);
                    table = r_u32(0x801132BC);
                    AddPrim(psx_addr(table + (count << 2), 4), psx_addr(packet, 52));
                    columns = r_u32(0x800FB854);
                    packets += 52;
                } while ((sint32)column < (sint32)columns);
            }
            ++row;
        } while ((sint32)row < r_s32(0x800FB858));
    }
    old = r_u32(0x800FB85C);
    phase = r_u32(0x800FB820);
    inactive = r_u32(0x800FB860);
    ++phase;
    w_u32(0x800FB85C, inactive);
    w_u32(0x800FB860, old);
    w_u32(0x800FB820, phase);
}

void front_draw_text_panel(uint32 panel)
{
    uint32 bank, index, line_height, scroll, count, numerator, denominator, clip;
    uint32 color, y, origin, x, right, lines, width, text, remainder, bottom, limit;
    FUNCTION_MARKER(0x80098C10, "FRONT.OVL");
    if (!r_u32(panel + 28))
        return;
    bank = r_u32(panel + 16);
    w_u32(0x800FB870, bank);
    if (!panel)
        return;
    index = r_u32(panel + 40);
    line_height = r_u32(panel + 32);
    numerator = index * line_height;
    scroll = r_u32(panel + 36);
    if (numerator != scroll)
    {
        count = r_u32(panel + 28);
        denominator = count * line_height;
        numerator = scroll + 1;
        if (!denominator || (denominator == 0xFFFFFFFF && numerator == 0x80000000))
            abort();
        w_u32(panel + 36, (uint32)((sint32)numerator % (sint32)denominator));
    }
    count = r_u32(panel + 28);
    line_height = r_u32(panel + 32);
    scroll = r_u32(panel + 36);
    bottom = r_u32(panel + 12);
    numerator = scroll + (count + 1) * line_height - bottom;
    if (!line_height || (line_height == 0xFFFFFFFF && numerator == 0x80000000))
        abort();
    clip = (uint32)((sint32)numerator % (sint32)line_height);
    numerator = scroll + line_height - clip;
    if (!line_height || (line_height == 0xFFFFFFFF && numerator == 0x80000000))
        abort();
    numerator = (uint32)((sint32)numerator / (sint32)line_height);
    if (!count || (count == 0xFFFFFFFF && numerator == 0x80000000))
        abort();
    index = (uint32)((sint32)numerator % (sint32)count);
    color = r_u32(panel + 20);
    origin = r_u32(panel + 4);
    x = r_u32(panel);
    y = origin - clip;
    lines = r_u32(panel + 44);
    width = r_u32(panel + 8);
    text = r_u32(lines + (index << 2));
    right = x + width;
    width = front_text_width(text);
    x = right - width;
    lines = r_u32(panel + 44);
    text = r_u32(lines + (index << 2));
    front_draw_text_clip_top(text, x, y, color, clip);
    for (;;)
    {
        line_height = r_u32(panel + 32);
        origin = r_u32(panel + 4);
        bottom = r_u32(panel + 12);
        y += line_height;
        limit = origin + bottom;
        bottom = y + line_height;
        ++index;
        if ((sint32)limit < (sint32)bottom)
            break;
        count = r_u32(panel + 28);
        if (!count || (count == 0xFFFFFFFF && index == 0x80000000))
            abort();
        remainder = (uint32)((sint32)index % (sint32)count);
        lines = r_u32(panel + 44);
        text = r_u32(lines + (remainder << 2));
        width = front_text_width(text);
        count = r_u32(panel + 28);
        if (!count || (count == 0xFFFFFFFF && index == 0x80000000))
            abort();
        remainder = (uint32)((sint32)index % (sint32)count);
        x = right - width;
        lines = r_u32(panel + 44);
        text = r_u32(lines + (remainder << 2));
        front_draw_colored_text(text, x, y, color);
    }
    origin = r_u32(panel + 4);
    bottom = r_u32(panel + 12);
    if ((sint32)y >= (sint32)(origin + bottom))
        return;
    count = r_u32(panel + 28);
    if (!count || (count == 0xFFFFFFFF && index == 0x80000000))
        abort();
    remainder = (uint32)((sint32)index % (sint32)count);
    line_height = r_u32(panel + 32);
    scroll = r_u32(panel + 36);
    if (!line_height || (line_height == 0xFFFFFFFF && scroll == 0x80000000))
        abort();
    clip = (uint32)((sint32)scroll % (sint32)line_height);
    lines = r_u32(panel + 44);
    text = r_u32(lines + (remainder << 2));
    width = front_text_width(text);
    count = r_u32(panel + 28);
    if (!count || (count == 0xFFFFFFFF && index == 0x80000000))
        abort();
    remainder = (uint32)((sint32)index % (sint32)count);
    x = right - width;
    lines = r_u32(panel + 44);
    text = r_u32(lines + (remainder << 2));
    front_draw_text_clip_bottom(text, x, y, color, clip);
}

void front_draw_text_clip_bottom(uint32 text, uint32 x, uint32 y, uint32 color, uint32 height)
{
    uint32 glyph, bank, atlas, character, packet, page = 0, cached_page = 0;
    uint32 shift, type, atlas_x, atlas_y, value, width, ordering_table, opcode;
    sint32 metric_height, depth, previous_page, clut_x, clut_y;
    uint16 clut;
    FUNCTION_MARKER(0x800979CC, "FRONT.OVL");
    if ((sint32)height <= 0)
        return;
    glyph = r_u32(0x8010D26C);
    bank = r_u32(0x800FB870);
    metric_height = r_s16(0x8004EB7A + ((glyph + bank) << 4));
    if (metric_height < (sint32)height)
        height = (uint32)metric_height;
    character = r_u8(text);
    while (character)
    {
        glyph = r_u32(0x8010D168 + (character << 2));
        if ((sint32)glyph < 0)
            x += 4;
        else
        {
            bank = r_u32(0x800FB870);
            atlas = 0x8004EB70 + ((glyph + bank) << 4);
            depth = r_s16(atlas + 12);
            type = GetGraphType();
            shift = (2 - (uint32)depth) & 31;
            if (type == 1 || GetGraphType() == 2)
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
            previous_page = (sint16)cached_page;
            if (!(uint16)cached_page)
            {
                cached_page = page;
                previous_page = (sint16)cached_page;
            }
            if ((uint32)previous_page != page)
            {
                packet = r_u32(0x801132B8);
                cached_page = page;
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
            w_u8(packet + 12, (value & 63) << shift);
            value = r_u16(atlas + 2);
            w_u8(packet + 13, value);
            width = r_u16(atlas + 14);
            w_u16(packet + 18, height);
            w_u16(packet + 16, width);
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
        character = r_u8(++text);
    }
    packet = r_u32(0x801132B8);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)page, NULL);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

void front_draw_text_clip_top(uint32 text, uint32 x, uint32 y, uint32 color, uint32 trim)
{
    uint32 glyph, bank, atlas, character, packet, page = 0, cached_page = 0;
    uint32 shift, type, atlas_x, atlas_y, value, ordering_table, opcode;
    sint32 metric_height, depth, previous_page, clut_x, clut_y;
    uint16 clut;
    FUNCTION_MARKER(0x80097CD4, "FRONT.OVL");
    if ((sint32)trim < 0)
    {
        y -= trim;
        trim = 0;
    }
    glyph = r_u32(0x8010D26C);
    bank = r_u32(0x800FB870);
    metric_height = r_s16(0x8004EB7A + ((glyph + bank) << 4));
    if (metric_height < (sint32)trim)
        return;
    character = r_u8(text);
    while (character)
    {
        glyph = r_u32(0x8010D168 + (character << 2));
        if ((sint32)glyph < 0)
            x += 4;
        else
        {
            bank = r_u32(0x800FB870);
            atlas = 0x8004EB70 + ((glyph + bank) << 4);
            depth = r_s16(atlas + 12);
            type = GetGraphType();
            shift = (2 - (uint32)depth) & 31;
            if (type == 1 || GetGraphType() == 2)
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
            previous_page = (sint16)cached_page;
            if (!(uint16)cached_page)
            {
                cached_page = page;
                previous_page = (sint16)cached_page;
            }
            if ((uint32)previous_page != page)
            {
                packet = r_u32(0x801132B8);
                cached_page = page;
                SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)previous_page, NULL);
                ordering_table = r_u32(0x801132BC);
                AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 12));
                w_u32(0x801132B8, packet + 12);
            }
            packet = r_u32(0x801132B8);
            w_u8(packet + 3, 4);
            w_u8(packet + 7, 0x64);
            w_u16(packet + 8, x);
            w_u16(packet + 10, y + trim);
            value = r_u16(atlas);
            w_u8(packet + 12, (value & 63) << shift);
            value = r_u16(atlas + 2);
            w_u8(packet + 13, value);
            value = r_u16(atlas + 14);
            w_u16(packet + 16, value);
            value = r_u16(atlas + 10);
            w_u16(packet + 18, value - trim);
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
        character = r_u8(++text);
    }
    packet = r_u32(0x801132B8);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (uint16)page, NULL);
    ordering_table = r_u32(0x801132BC);
    AddPrim(psx_addr(ordering_table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

uint32 front_configure_background_light(uint32 layer, uint32 x, uint32 y, uint32 color, uint32 falloff)
{
    uint32 offset, value;
    FUNCTION_MARKER(0x8009A458, "FRONT.OVL");
    offset = (uint32)((sint32)(layer << 16) >> 12);
    w_u16(0x800FB824 + offset, x);
    w_u16(0x800FB826 + offset, y);
    w_u8(0x800FB82A + offset, color);
    w_u8(0x800FB82B + offset, color >> 8);
    w_u8(0x800FB82C + offset, color >> 16);
    value = (uint32)(((uint64)(color & 255) * 0x4EC4EC4F) >> 34);
    w_u8(0x800FB82D + offset, value);
    value = (uint32)(((uint64)((color >> 8) & 255) * 0x4EC4EC4F) >> 34);
    w_u8(0x800FB82E + offset, value);
    value = (uint32)(((uint64)((color >> 16) & 255) * 0x4EC4EC4F) >> 34);
    w_u16(0x800FB828 + offset, falloff);
    w_u8(0x800FB82F + offset, value);
    return value;
}

void front_configure_background(uint32 columns, uint32 rows)
{
    uint32 count, first, second, points, colors, red, green, blue;
    FUNCTION_MARKER(0x8009A550, "FRONT.OVL");
    count = (columns + 1) * (rows + 1);
    w_u32(0x800FB854, columns);
    w_u32(0x800FB858, rows);
    first = heap_alloc_back(count * 52);
    columns = r_u32(0x800FB854);
    rows = r_u32(0x800FB858);
    count = (columns + 1) * (rows + 1);
    w_u32(0x800FB85C, first);
    second = heap_alloc_back(count * 52);
    columns = r_u32(0x800FB854);
    rows = r_u32(0x800FB858);
    count = (columns + 1) * (rows + 1);
    w_u32(0x800FB860, second);
    points = heap_alloc_back(count << 2);
    columns = r_u32(0x800FB854);
    rows = r_u32(0x800FB858);
    count = (columns + 1) * (rows + 1);
    w_u32(0x800FB864, points);
    colors = heap_alloc_back(count << 2);
    w_u32(0x800FB868, colors);
    front_configure_background_light(0, 320, 240, 0, 9);
    red = r_u8(0x800FB82A);
    green = r_u8(0x800FB82B);
    blue = r_u8(0x800FB82C);
    w_u8(0x800FB830, red);
    w_u8(0x800FB831, green);
    w_u8(0x800FB832, blue);
    front_configure_background_light(1, 40, 40, 0x105010, 9);
    red = r_u8(0x800FB83A);
    green = r_u8(0x800FB83B);
    blue = r_u8(0x800FB83C);
    w_u8(0x800FB840, red);
    w_u8(0x800FB841, green);
    w_u8(0x800FB842, blue);
    front_configure_background_light(2, 600, 440, 0x501010, 9);
    red = r_u8(0x800FB84A);
    green = r_u8(0x800FB84B);
    blue = r_u8(0x800FB84C);
    w_u8(0x800FB850, red);
    w_u8(0x800FB851, green);
    w_u8(0x800FB852, blue);
}

void front_initialize_background(void)
{
    uint32 first, second, pal, span, rows, columns, phase, points, colors, atlas;
    uint32 step, shift, row, column, y_numerator, x_numerator, y, x, y_offset;
    uint32 offset, component, dx, dy, square, intensity, light, value, index, base_index;
    uint32 top, bottom, x0, x1, p0, p1, color, vertex, vertex_offset, u0, u1, v0, v1;
    sint32 exponent, depth, width, height, clut_x, clut_y;
    uint16 clut, page;
    FUNCTION_MARKER(0x8009A89C, "FRONT.OVL");
    first = r_u32(0x800FB85C);
    pal = r_u32(0x800311C0);
    second = r_u32(0x800FB860);
    span = pal ? 480 : 448;
    rows = r_u32(0x800FB858);
    if (!rows)
        abort();
    step = (uint32)((sint32)span / (sint32)rows);
    phase = r_u32(0x800FB820);
    if (!step || (step == 0xFFFFFFFF && phase == 0x80000000))
        abort();
    y_offset = step - (uint32)((sint32)phase % (sint32)step);
    points = r_u32(0x800FB864);
    colors = r_u32(0x800FB868);
    atlas = 0x8004EB70 + (((phase & 15) + 449) << 4);
    depth = r_s16(atlas + 12);
    shift = (2 - (uint32)depth) & 31;
    if ((sint32)rows >= 0)
    {
        row = 0;
        y_numerator = 0;
        do
        {
            rows = r_u32(0x800FB858);
            if (!rows || (rows == 0xFFFFFFFF && y_numerator == 0x80000000))
                abort();
            y = (uint32)((sint32)y_numerator / (sint32)rows) + 2;
            columns = r_u32(0x800FB854);
            if ((sint32)columns >= 0)
            {
                column = 0;
                x_numerator = 0;
                do
                {
                    columns = r_u32(0x800FB854);
                    if (!columns || (columns == 0xFFFFFFFF && x_numerator == 0x80000000))
                        abort();
                    x = (uint32)((sint32)x_numerator / (sint32)columns);
                    w_u16(points + 2, y);
                    w_u16(points, x);
                    w_u8(colors, 8);
                    w_u8(colors + 1, 20);
                    w_u8(colors + 2, 16);
                    for (offset = 0; offset < 48; offset += 16)
                    {
                        exponent = r_s16(0x800FB828 + offset);
                        if (exponent == -1)
                            continue;
                        value = (uint32)r_s16(0x800FB824 + offset);
                        dx = x - value;
                        square = dx * dx;
                        value = (uint32)r_s16(0x800FB826 + offset);
                        dy = y - value;
                        square += dy * dy;
                        intensity = 176 - (uint32)((sint32)square >> ((uint32)exponent & 31));
                        if ((sint32)intensity < 0)
                            intensity = 0;
                        for (component = 0; component < 3; ++component)
                        {
                            light = r_u8(0x800FB830 + offset + component);
                            value = r_u8(colors + component);
                            square = intensity * light;
                            value += (uint32)((sint32)square >> 8);
                            w_u8(colors + component, value);
                        }
                    }
                    points += 4;
                    colors += 4;
                    columns = r_u32(0x800FB854);
                    ++column;
                    x_numerator += 640;
                } while ((sint32)column <= (sint32)columns);
            }
            rows = r_u32(0x800FB858);
            ++row;
            y_numerator += span;
        } while ((sint32)row <= (sint32)rows);
    }
    if (r_s32(0x800FB858) <= 0)
        return;
    row = 0;
    do
    {
        columns = r_u32(0x800FB854);
        base_index = row * (columns + 1);
        points = r_u32(0x800FB864);
        top = (uint32)r_s16(points + (base_index << 2) + 2);
        bottom = (uint32)r_s16(points + ((base_index + columns) << 2) + 6);
        if ((sint32)columns > 0)
        {
            top += y_offset;
            bottom += y_offset;
            column = 0;
            do
            {
                index = (base_index + column) << 2;
                points = r_u32(0x800FB864);
                x0 = (uint32)r_s16(points + index);
                x1 = (uint32)r_s16(points + index + 4);
                p0 = first;
                p1 = second;
                w_u8(p0 + 3, 12);
                w_u8(p0 + 7, 0x3C);
                w_u8(p1 + 3, 12);
                w_u8(p1 + 7, 0x3C);
                colors = r_u32(0x800FB868);
                color = colors + index;
                for (vertex = 0; vertex < 4; ++vertex)
                {
                    if (vertex == 2)
                    {
                        columns = r_u32(0x800FB854);
                        color += columns << 2;
                    }
                    else if (vertex)
                        color += 4;
                    vertex_offset = 4 + vertex * 12;
                    for (component = 0; component < 3; ++component)
                    {
                        value = r_u8(color + component);
                        w_u8(p0 + vertex_offset + component, value);
                    }
                    for (component = 0; component < 3; ++component)
                    {
                        value = r_u8(color + component);
                        w_u8(p1 + vertex_offset + component, value);
                    }
                }
                clut_x = r_s16(atlas + 4);
                clut_y = r_s16(atlas + 6);
                first += 52;
                clut = GetClut(clut_x, clut_y);
                w_u16(p0 + 14, clut);
                clut_x = r_s16(atlas + 4);
                clut_y = r_s16(atlas + 6);
                second += 52;
                clut = GetClut(clut_x, clut_y);
                w_u16(p1 + 14, clut);
                w_u16(p0 + 8, x0);
                w_u16(p0 + 10, top);
                w_u16(p0 + 20, x1);
                w_u16(p0 + 22, top);
                w_u16(p0 + 32, x0);
                w_u16(p0 + 34, bottom);
                w_u16(p0 + 44, x1);
                w_u16(p0 + 46, bottom);
                w_u16(p1 + 8, x0);
                w_u16(p1 + 10, top);
                w_u16(p1 + 20, x1);
                w_u16(p1 + 22, top);
                w_u16(p1 + 32, x0);
                w_u16(p1 + 34, bottom);
                w_u16(p1 + 44, x1);
                w_u16(p1 + 46, bottom);
                clut_x = r_s16(atlas);
                clut_y = r_s16(atlas + 2);
                page = GetTPage(depth, 1, clut_x, clut_y);
                ++column;
                w_u16(p0 + 26, page);
                w_u16(p1 + 26, page);
                u0 = r_u16(atlas);
                v0 = r_u8(atlas + 2);
                width = r_s16(atlas + 8);
                height = r_s16(atlas + 10);
                u0 = (u0 & 63) << shift;
                u1 = u0 + ((uint32)width << shift) - 1;
                v1 = v0 + (uint32)height;
                w_u8(p0 + 12, u0);
                w_u8(p0 + 13, v0);
                w_u8(p0 + 24, u1);
                w_u8(p0 + 25, v0);
                w_u8(p0 + 36, u0);
                w_u8(p0 + 37, v1);
                w_u8(p0 + 48, u1);
                w_u8(p0 + 49, v1);
                w_u8(p1 + 12, u0);
                w_u8(p1 + 13, v0);
                w_u8(p1 + 24, u1);
                w_u8(p1 + 25, v0);
                w_u8(p1 + 36, u0);
                w_u8(p1 + 37, v1);
                w_u8(p1 + 48, u1);
                w_u8(p1 + 49, v1);
                columns = r_u32(0x800FB854);
            } while ((sint32)column < (sint32)columns);
        }
        rows = r_u32(0x800FB858);
        ++row;
    } while ((sint32)row < (sint32)rows);
}

void front_store_screen_action(sint32 screen, sint32 action)
{
    uint32 offset;
    FUNCTION_MARKER(0x8008D4E8, "FRONT.OVL");
    offset = (uint32)((sint32)((uint32)screen << 16) >> 15);
    w_u16(0x800FB750 + offset, (uint32)action);
}

sint32 front_next_screen(void)
{
    uint32 buttons, attempts, links, selection, table, direction, index, record_offset;
    uint32 target, info, forced, pending;
    sint32 enabled, initial, current, screen;
    FUNCTION_MARKER(0x8008D534, "FRONT.OVL");
    enabled = r_s16(0x800F3114);
    buttons = r_u32(0x80053684);
    attempts = 0;
    if (enabled)
    {
        initial = r_s16(0x8010D0DC);
        links = r_u32(0x8010DA70);
        for (;;)
        {
            selection = r_u16(0x8010D0DC);
            index = (uint32)(sint32)(sint16)(selection - 31);
            direction = 0xFFFFFFFF;
            if (buttons & 0x1000)
                direction = 0;
            else if (buttons & 0x4000)
                direction = 4;
            else if (buttons & 0x8000)
                direction = 8;
            else if (buttons & 0x2000)
                direction = 12;
            else if ((sint32)attempts > 0)
                direction = 0;
            if (direction != 0xFFFFFFFF)
            {
                table = r_u32(links + direction);
                selection = r_u8(table + index);
                w_u16(0x8010D0DC, selection);
            }
            current = r_s16(0x8010D0DC);
            ++attempts;
            enabled = r_u8(0x800F2F89 + (uint32)current);
            if (!enabled || (initial == current && (sint32)attempts >= 2))
                break;
        }
        index = (uint32)current - 31;
        if (initial != current)
        {
            sound_play_sample_pitch(10, 128);
            sound_play_sample_pitch(10, 128);
        }
        else if (r_s16(0x800F3116) && (r_u32(0x80053684) & 0x40))
        {
            sound_play_sample_pitch(11, 128);
            sound_play_sample_pitch(11, 128);
            info = r_u32(0x8010DA70);
            table = r_u32(info + 16);
            record_offset = (uint32)((sint32)(index << 16) >> 13);
            target = r_u32(table + record_offset + 4);
            psx_dispatch_guest_call(target, 0, 0, 0, 0);
            screen = r_s16(0x80114264);
            current = r_s16(0x8010D0DC);
            front_store_screen_action(screen, current);
            info = r_u32(0x8010DA70);
            table = r_u32(info + 16);
            screen = r_s16(0x80114264);
            target = r_u8(table + record_offset);
            w_u16(0x800F312A, screen == (sint32)target ? 0 : 1);
            info = r_u32(0x8010DA70);
            w_u16(0x800F3118, 1);
            table = r_u32(info + 16);
            return (sint32)r_u8(table + (uint32)((sint32)(index << 16) >> 13));
        }
    }
    pending = (uint32)r_s16(0x800F3118);
    if (!pending && r_u32(0x800F3140) != 1)
        return -1;
    forced = r_u32(0x800F3144);
    w_u16(0x800F3118, 1);
    if (!forced && !(r_u32(0x80053684) & 0x10) && !r_u32(0x800F3140))
        return -1;
    w_u32(0x800F3148, 1);
    sound_play_sample_pitch(11, 128);
    sound_play_sample_pitch(11, 128);
    screen = r_s16(0x80114264);
    front_store_screen_action(screen, 31);
    info = r_u32(0x8010DA70);
    screen = r_s16(0x80114264);
    target = r_u8(info + 20);
    w_u16(0x800F312A, screen == (sint32)target ? 0 : 1);
    info = r_u32(0x8010DA70);
    w_u16(0x800F3114, 1);
    w_u16(0x800F3116, 1);
    w_u32(0x800F3144, 0);
    return (sint32)r_u8(info + 20);
}

void front_update_connection_label(void)
{
    uint32 state, format_address;
    const char *format;
    char *destination;
    size_t offset, span;
    FUNCTION_MARKER(0x800A52FC, "FRONT.OVL");
    if (!r_u8(0x800FB10C))
        w_u32(0x800F3080, 2);
    state = r_u32(0x800F3080);
    if ((sint32)state < 3)
    {
        state = r_u32(0x800F3080);
        format_address = r_u32(0x800BD364 + (state << 2));
    }
    else
        format_address = r_u32(0x800BD370);
    format = (const char *)psx_addr(format_address, 1);
    offset = (size_t)((uintptr_t)format - (uintptr_t)DRAM);
    if (offset >= PSX_DRAM_SIZE)
        abort();
    span = PSX_DRAM_SIZE - offset;
    format = (const char *)psx_addr(format_address, span);
    destination = (char *)psx_addr(0x800F3084, 1);
    offset = (size_t)((uintptr_t)destination - (uintptr_t)DRAM);
    if (offset >= PSX_DRAM_SIZE)
        abort();
    span = PSX_DRAM_SIZE - offset;
    destination = (char *)psx_addr(0x800F3084, span);
    sprintf(destination, format);
}

void front_present_still(uint32 *bitstream, uint32 format)
{
    PSX_RECT rectangle;
    DISPENV display;
    uint32 pal, x, y;
    FUNCTION_MARKER(0x8008DED0, "FRONT.OVL");
    VSync(0);
    SetDispMask(0);
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 640;
    rectangle.h = 480;
    front_decode_still(bitstream, &rectangle, format);
    pal = r_u32(0x800311C0);
    if (pal)
    {
        SetDefDispEnv(&display, 0, 0, 640, 512);
        w_u16(0x800FB8E6, 256);
    }
    else
        SetDefDispEnv(&display, 0, 0, 640, 480);
    pal = r_u32(0x800311C0);
    display.isinter = 1;
    display.pad0 = (uint8)pal;
    display.isrgb24 = format != 0;
    x = r_u32(0x80010564);
    y = r_u32(0x80010568);
    display.screen.x = (sint16)x;
    display.screen.y = (sint16)y;
    PutDispEnv(&display);
    VSync(0);
    SetDispMask(1);
}

void front_show_startup_image(void)
{
    FUNCTION_MARKER(0x8008E034, "FRONT.OVL");
    front_present_still((uint32 *)psx_addr(0x800E15B8, 0x32CB0), 0);
}

void front_intro(void)
{
    FUNCTION_MARKER(0x8008E05C, "FRONT.OVL");
    SetDispMask(0);
    VSyncCallbackPSX(0);
    w_u32(0x800F314C, 0);
    sound_tick();
    sound_tick();
    sound_tick();
    sound_tick();
    if (r_u8(0x80010554))
    {
        w_u32(0x800B7274, 1);
        w_u32(0x800B7270, 0);
        front_play_movie_with_sound_reset("\\INTRO.STR;1", 30);
        VSync(0);
        SetDispMask(0);
        w_u32(0x800B7274, 0);
        w_u32(0x800B7270, 1);
    }
    w_u8(0x80010554, 0);
}

void front_play_menu_movie(const char *path, uint32 parameter)
{
    DISPENV display;
    DRAWENV draw;
    PSX_RECT rectangle;
    uint32 words[23], index, x, y, sample, flags;
    sint32 voice;
    FUNCTION_MARKER(0x800A93B8, "FRONT.OVL");
    VSync(0);
    ResetGraph(1);
    PutDrawEnv((DRAWENV *)psx_addr(0x800FB878, 92));
    front_swap_render_buffers();
    front_update_screen();
    stream_stop();
    stream_cancel_current();
    for (index = 0; index < 20; ++index)
    {
        VSync(0);
        front_update_screen();
    }
    VSyncCallbackPSX(0);
    sound_stop_voice(r_u32(0x800F3130));
    sound_tick();
    sound_stop_voice(r_u32(0x800F3134));
    sound_tick();
    sound_tick();
    sound_tick();
    front_play_movie_with_sound_reset(path, parameter);
    DrawSync(0);
    SetDispMask(0);
    for (index = 0; index < 5; ++index)
    {
        (void)r_u32(0x800FB8D8 + index * 4);
        words[index] = r_u32(0x800FB8D8 + index * 4);
    }
    memcpy(&display, words, 20);
    for (index = 0; index < 23; ++index)
        words[index] = r_u32(0x800FB878 + index * 4);
    memcpy(&draw, words, 92);
    x = r_u32(0x80010564);
    y = r_u32(0x80010568);
    display.screen.h = 240;
    draw.dfe = 0;
    display.screen.x = (sint16)x;
    display.screen.y = (sint16)y;
    PutDrawEnv(&draw);
    PutDispEnv(&display);
    VSync(0);
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 640;
    rectangle.h = 256;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    VSync(0);
    rectangle.x = 0;
    rectangle.y = 256;
    rectangle.w = 640;
    rectangle.h = 256;
    ClearImage(&rectangle, 0, 0, 0);
    DrawSync(0);
    PutDispEnv((DISPENV *)psx_addr(0x800FB8D8, 20));
    VSync(0);
    SetDispMask(1);
    sample = r_u32(0x80042970);
    flags = r_u32(0x800313A0);
    voice = sound_reserve_voice((sint32)sample, 50, 15, 15, 256, flags);
    sample = r_u32(0x80042970);
    flags = r_u32(0x800313A0);
    w_u32(0x800F3130, (uint32)voice);
    voice = sound_reserve_voice((sint32)sample, 50, 15, 15, 137, flags);
    w_u32(0x800F3134, (uint32)voice);
    VSync(0);
    VSyncCallbackPSX(0x8008DA54);
}

void front_play_intro_from_menu(void)
{
    FUNCTION_MARKER(0x800A96B4, "FRONT.OVL");
    front_play_menu_movie("\\INTRO.STR;1", 30);
}

void front_show_still_beea8(void)
{
    FUNCTION_MARKER(0x8008DFE4, "FRONT.OVL");
    front_present_still((uint32 *)psx_addr(0x800BEEA8, 0x553C0), 1);
}

void front_show_still_eac00(void)
{
    FUNCTION_MARKER(0x8008E00C, "FRONT.OVL");
    front_present_still((uint32 *)psx_addr(0x800EAC00, 0x29668), 0);
}

sint32 front_prompt_movie_disc(const char *path)
{
    FUNCTION_MARKER(0x8008A020, "FRONT.OVL");
    return game_validate_data_disc() ? 0 : -1;
}

void front_noop_b838(void)
{
    FUNCTION_MARKER(0x8008B838, "FRONT.OVL");
    /* Original body only returns */
}

void front_noop_b840(void)
{
    FUNCTION_MARKER(0x8008B840, "FRONT.OVL");
    /* Original body only returns */
}

uint32 front_reset_action_word(void)
{
    FUNCTION_MARKER(0x8008B848, "FRONT.OVL");
    w_u16(0x800FB7A6, 31);
    return 31;
}

uint32 front_find_pane(sint32 action)
{
    uint32 pane;
    sint32 current;
    FUNCTION_MARKER(0x8008D3DC, "FRONT.OVL");
    pane = r_u32(0x800FB110);
    action = (sint16)action;
    current = r_s16(pane + 22);
    while (current != action)
    {
        pane = r_u32(pane + 16);
        current = r_s16(pane + 22);
    }
    return pane;
}

uint32 front_update_menu_frame(void)
{
    uint32 gate, active, inactive, count, packets, root, pane, target;
    uint32 ready, done = 0, previous, buttons, delay;
    sint32 next;
    FUNCTION_MARKER(0x8008DC5C, "FRONT.OVL");
    gate = r_u32(0x800B7378);
    if (gate)
        return gate;
    gate = r_u32(0x800B73A8);
    if (gate)
        return gate;
    inactive = r_u32(0x801132C0);
    active = r_u32(0x801132BC);
    w_u32(0x801132BC, inactive);
    root = r_u32(0x801132BC);
    count = r_u32(0x801132A8);
    w_u32(0x801132B4, 0);
    w_u32(0x801132C0, active);
    packets = r_u32(root + 0x7CC);
    w_u32(0x801132B8, packets);
    ++count;
    ClearOTagR((uint32 *)psx_addr(inactive, (sint32)count > 0 ? count * 4 : 4), (sint32)count);
    root = r_u32(0x8010D0E0);
    if (root)
        front_draw_screen_list(root);
    delay = 0x251A;
    do
    {
        previous = delay;
        --delay;
    } while ((sint32)previous >= 0);
    if (!r_u32(0x800B73A8))
    {
        PutDrawEnv((DRAWENV *)psx_addr(0x800FB878, 92));
        count = r_u32(0x801132A8);
        inactive = r_u32(0x801132C0);
        DrawOTag((uint32 *)psx_addr(inactive + (count << 2), 4));
    }
    if (!r_s16(0x800B737C))
    {
        pane = r_u32(0x8010D0E0);
        ready = 1;
        while (pane)
        {
            if (r_s16(pane + 44) == 1)
                ready = 0;
            pane = r_u32(pane + 16);
        }
        if (ready == 1)
        {
            next = front_next_screen();
            if ((sint16)next != -1)
            {
                w_u16(0x80114264, (uint32)next);
                w_u16(0x800B737C, 1);
            }
        }
    }
    if (r_u8(0x800B7384) == 1)
    {
        target = r_u32(0x800FB938);
        psx_dispatch_guest_call(target, 0, 0, 0, 0);
    }
    if (r_s16(0x800B737C) == 1 || !r_u8(0x800FB10C))
        done = 1;
    pane = r_u32(0x8010D0E0);
    while (pane)
    {
        target = r_u32(pane + 32);
        psx_dispatch_guest_call(target, 1, pane, 0, 0);
        if (r_s16(pane + 44) != 2)
            done = 0;
        pane = r_u32(pane + 16);
    }
    previous = r_u32(0x8004616C);
    w_u16(0x801132A4, done);
    w_u32(0x80053678, previous);
    buttons = (uint32)pad_update(0);
    previous = r_u32(0x80053678);
    w_u32(0x8004616C, buttons);
    w_u32(0x80053684, (buttons ^ previous) & buttons);
    return buttons;
}

void front_noop_f37c(void)
{
    FUNCTION_MARKER(0x8008F37C, "FRONT.OVL");
    /* Original body only returns */
}

uint32 front_leave_menu(void)
{
    uint32 x, y;
    FUNCTION_MARKER(0x8008F384, "FRONT.OVL");
    w_u8(0x800FB10C, 0);
    front_clear_menu_transition();
    if (r_u32(0x800311C0))
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

sint32 front_list_files(const char *suffix, uint32 *files, sint32 capacity)
{
    uint32 entry, count = 0;
    const char *name, *dot;
    size_t offset, span;
    FUNCTION_MARKER(0x8008F3F8, "FRONT.OVL");
    if ((sint32)(uint32)strlen(suffix) >= 4)
    {
#ifndef NDEBUG
        printf("filelist:error invalid suffix");
#endif
        return 0;
    }
    entry = r_u32(0x800421A8);
    while (r_u8(entry))
    {
        name = (const char *)psx_addr(entry, 1);
        offset = (size_t)((uintptr_t)name - (uintptr_t)DRAM);
        if (offset >= PSX_DRAM_SIZE)
            abort();
        span = PSX_DRAM_SIZE - offset;
        name = (const char *)psx_addr(entry, span);
        dot = strchr(name, '.');
        if (dot && !strcmp(dot + 1, suffix))
        {
            if ((sint32)count < capacity)
                files[count] = entry;
            ++count;
        }
        entry += 32;
    }
    return (sint32)count;
}

uint32 front_replace_text_byte(uint32 text, uint32 from, uint32 to)
{
    uint32 value;
    FUNCTION_MARKER(0x8008F4DC, "FRONT.OVL");
    value = r_u8(text);
    from &= 255;
    while (value)
    {
        if (value == from)
            w_u8(text, to);
        value = r_u8(++text);
    }
    return 0;
}

void front_uppercase_text(uint32 text)
{
    uint32 value;
    FUNCTION_MARKER(0x8008F518, "FRONT.OVL");
    value = r_u8(text);
    while (value)
    {
        value = (uint32)toupper((int)value);
        w_u8(text, value);
        value = r_u8(++text);
    }
}

void front_build_model_bounds(uint32 model, uint32 bounds)
{
    sint32 minimum[3] = {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};
    sint32 maximum[3] = {0, 0, 0};
    sint32 candidates[4];
    uint32 descriptor;
    uint32 vertices;
    sint32 count;
    sint32 index;
    uint32 axis;
    sint32 value;
    sint32 extent;
    sint32 min_x;
    sint32 min_y;
    sint32 min_z;
    sint32 max_x;
    sint32 max_z;
    sint32 shift_x;
    sint32 shift_y;
    sint32 shift_z;
    uint32 center;
    uint32 counter;
    uint32 selected;
    uint32 record;
    FUNCTION_MARKER(0x8008F56C, "FRONT.OVL");
    descriptor = r_u32(model);
    count = r_s32(descriptor + 4);
    vertices = r_u32(descriptor);
    for (index = 0; index < count; ++index)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            value = r_s16(vertices + axis * 2);
            if (value < minimum[axis])
                minimum[axis] = value;
            if (value > maximum[axis])
                maximum[axis] = value;
        }
        vertices += 8;
    }
    extent = minimum[0] < 0 ? -minimum[0] : minimum[0];
    value = minimum[1] < 0 ? -minimum[1] : minimum[1];
    w_u32(bounds, model);
    w_u16(bounds + 0x10, minimum[0]);
    w_u16(bounds + 0x12, minimum[1]);
    w_u16(bounds + 0x14, minimum[2]);
    w_u32(model + 0x6C, extent);
    w_u16(model + 0x7A, maximum[0]);
    w_u16(model + 0x7C, maximum[1]);
    w_u16(model + 0x7E, maximum[2]);
    if (extent < value)
        w_u32(model + 0x6C, value);
    candidates[0] = minimum[2];
    candidates[1] = maximum[0];
    candidates[2] = maximum[1];
    candidates[3] = maximum[2];
    for (index = 0; index < 4; ++index)
    {
        extent = r_s32(model + 0x6C);
        value = candidates[index] < 0 ? -candidates[index] : candidates[index];
        if (extent < value)
            w_u32(model + 0x6C, value);
    }
    for (axis = 0; axis < 3; ++axis)
    {
        center = 0 - ((uint32)minimum[axis] + (uint32)maximum[axis]);
        center += center >> 31;
        w_u16(bounds + 0x18 + axis * 2, (sint32)center >> 1);
    }
    max_x = r_s16(model + 0x7A);
    min_x = r_s16(bounds + 0x10);
    min_y = r_s16(bounds + 0x12);
    extent = max_x - min_x;
    if (extent < 0)
        extent = -extent;
    w_u32(bounds + 8, extent);
    value = r_s16(model + 0x7C) - min_y;
    if (value < 0)
        value = -value;
    if (extent < value)
        w_u32(bounds + 8, value);
    max_z = r_s16(model + 0x7E);
    min_z = r_s16(bounds + 0x14);
    extent = r_s32(bounds + 8);
    value = max_z - min_z;
    if (value < 0)
        value = -value;
    if (extent < value)
        w_u32(bounds + 8, value);
    center = r_u32(bounds + 8);
    min_z = r_s16(bounds + 0x14);
    center += center >> 31;
    w_u32(bounds + 8, (sint32)center >> 1);
    max_z = r_s16(model + 0x7E);
    w_u16(bounds + 4, -2);
    center = r_u32(bounds + 8);
    value = max_z - min_z;
    if (value < 0)
        value = -value;
    w_u16(bounds + 6, center);
    w_u32(bounds + 0xC, value);
    if ((sint16)center >= 10001)
    {
        do
        {
            center = (uint32)(sint32)(sint16)r_u16(bounds + 6);
            center += center >> 31;
            counter = r_u16(bounds + 4);
            w_u16(bounds + 6, (sint32)center >> 1);
            value = r_s16(bounds + 6);
            w_u16(bounds + 4, counter - 1);
        } while (value >= 10001);
    }
    shift_x = r_s16(bounds + 4);
    value = r_s16(bounds + 0x18);
    shift_y = r_s16(bounds + 4);
    w_u16(bounds + 0x18, value >> ((0 - (uint32)shift_x) & 31));
    value = r_s16(bounds + 0x1A);
    shift_z = r_s16(bounds + 4);
    w_u16(bounds + 0x1A, value >> ((0 - (uint32)shift_y) & 31));
    value = r_s16(bounds + 0x1C);
    selected = r_u32(0x8010D0D8);
    w_u16(bounds + 0x1C, value >> ((0 - (uint32)shift_z) & 31));
    record = (((selected << 3) + selected) << 5) - selected;
    record = r_u32(0x800BC6D0) + (record << 2);
    value = r_s32(record + 0x40);
    w_u16(bounds + 0x16, 30);
    while (value > 0)
    {
        counter = r_u16(bounds + 0x16);
        value >>= 1;
        w_u16(bounds + 0x16, counter - 3);
    }
}

uint32 front_create_model_object(sint32 model_index, uint32 x, uint32 y, uint32 z, uint32 type)
{
    uint32 object;
    uint32 model;
    uint32 value;
    uint32 bounds;
    sint32 shift;
    uint32 owner;
    uint32 serial;
    FUNCTION_MARKER(0x8008F8D4, "FRONT.OVL");
    object = object_allocate(type - 1 < 9 ? type & 255 : 0);
    model = ((uint32)model_index << 5) + (uint32)model_index;
    model = r_u32(0x80046178) + (model << 2);
    w_u32(object, type);
    w_u32(object + 0xA4, 0);
    w_u32(object + 0x74, model_index);
    value = r_u32(model);
    w_u32(object + 0x64, value);
    value = r_u32(model + 0x74);
    w_u32(object + 0x114, 32);
    w_u32(object + 0x2C, 0xFFFFFFFF);
    w_u32(object + 0x30, 0xFFFFFFFF);
    w_u32(object + 0x34, 0xFFFFFFFF);
    w_u32(object + 0x90, x);
    w_u32(object + 8, x);
    w_u32(object + 0x94, y);
    w_u32(object + 0xC, y);
    w_u32(object + 0x98, z);
    w_u32(object + 0x10, z);
    w_u32(object + 0x20, 0);
    w_u32(object + 0x24, 0);
    w_u32(object + 0x28, 0);
    w_u32(object + 0x6C, 0);
    w_u32(object + 0x68, value);
    w_u32(object + 0x3C, model_index < 2 ? 0x8002493C : 0x8002D2BC);
    w_u32(object + 0xD4, 0x808080);
    bounds = r_u32(0x800BC6E0);
    w_u32(object + 0x38, 0x8002B8FC);
    w_u32(object + 0x40, 0x8002493C);
    w_u32(object + 0x44, 0x8002493C);
    w_u32(object + 0x50, 0x8002493C);
    w_u32(object + 0x4C, 0x8002493C);
    w_u32(object + 0x5C, 0x8002493C);
    w_u32(object + 0x48, 0x8002493C);
    w_u32(object + 0xA0, 0);
    w_u32(object + 0x1E0, 0);
    w_u32(object + 0x168, 0);
    w_u32(object + 0x160, 0);
    w_u32(object + 0x1A0, 0);
    w_u32(object + 0x198, 0);
    w_u32(object + 0x70, 10);
    shift = r_s16(bounds + 4);
    owner = r_u32(0x800313E4);
    w_u8(object + 0x26C, 1);
    w_u8(object + 0x26B, 1);
    serial = r_u16(0x80031254);
    w_u16(object + 0x250, 0);
    w_u16(object + 0x252, 0);
    w_u16(object + 0x254, 0);
    w_u32(object + 0x6C, 0);
    w_u32(object + 4, 0);
    w_u32(object + 0xD8, owner);
    w_u32(object + 0x84, shift);
    w_u16(object + 0x24A, serial);
    w_u16(0x80031254, serial + 1);
    return object;
}

uint32 front_init_model_lighting(void)
{
    MATRIX light = {0};
    FUNCTION_MARKER(0x8008FDF8, "FRONT.OVL");
    light.m[0][0] = 4096;
    light.m[1][1] = 4096;
    light.m[2][2] = 4096;
    SetLightMatrix(&light);
    SetBackColor(96, 96, 96);
    light_set_color(0, 0, 0, 0);
    light_set_direction(0, 0, 1000, 1000);
    light_set_color(1, 128, 128, 128);
    light_set_direction(1, 1000, (uint32)-1000, 1000);
    light_set_color(2, 0, 0, 0);
    return light_set_direction(2, 0, 1000, 1000);
}

uint32 front_init_model_preview(void)
{
    uint32 object;
    uint32 slot;
    uint32 offset;
    sint32 model_index;
    uint32 record_index;
    uint32 record;
    uint32 bounds;
    uint32 base;
    uint32 camera;
    uint32 value;
    uint32 center;
    sint32 shift;
    FUNCTION_MARKER(0x8008FEC8, "FRONT.OVL");
    object_update_all();
    object_initialize_pool();
    front_init_model_lighting();
    object = front_create_model_object(1, 0, 0, 0, 0);
    matrix_set_identity(object + 0x11C);
    w_u32(0x80040FDC, object);
    w_u32(object + 0x40, 0x8008FC10);
    w_u32(object + 8, 0);
    w_u32(object + 0xC, 0);
    w_u32(object + 0x10, 0);
    w_u32(0x80040C94, object);
    for (slot = 0; slot < 2; ++slot)
    {
        offset = slot * 56;
        model_index = r_s32(0x800F6FA4 + offset);
        if (model_index < 0)
            continue;
        record_index = r_u32(0x800F6FA0 + offset);
        object = front_create_model_object(model_index, 0, 0, 0, 0);
        base = r_u32(0x800311B8);
        w_u32(object + 0x3C, 0x8002493C);
        w_u32(0x800BC6CC, 0);
        w_u32(object + 0x38, 0x80090144);
        w_u32(0x800FB80C, base);
        matrix_set_identity(object + 0x11C);
        record = (((record_index << 3) + record_index) << 5) - record_index;
        record = r_u32(0x800BC6D0) + (record << 2);
        bounds = ((uint32)model_index << 1) + (uint32)model_index;
        bounds = r_u32(0x800BC6E0) + (bounds << 4);
        center = (uint32)r_s16(bounds + 0x18);
        value = r_u32(record + 0x28);
        w_u32(object + 8, value + center);
        center = (uint32)r_s16(bounds + 0x1A);
        value = r_u32(record + 0x2C);
        w_u32(object + 0xC, value + center);
        value = (uint32)r_s16(bounds + 6);
        value = (uint32)((sint32)((value << 1) + value) >> 1);
        w_u32(record + 0x30, value);
        center = (uint32)r_s16(bounds + 0x1C);
        camera = r_u32(0x80040FDC);
        w_u32(object + 0x20, 2300);
        w_u32(object + 0x24, 4000);
        w_u32(object + 0x10, value + center);
        value = (uint32)r_s16(camera + 0x128);
        value = (value << 2) + value;
        value -= value << 4;
        w_u32(object + 0xFC, (sint32)value >> 12);
        value = (uint32)r_s16(camera + 0x12A);
        value = (value << 2) + value;
        value -= value << 4;
        w_u32(object + 0x100, (sint32)value >> 12);
        value = (uint32)r_s16(camera + 0x12C);
        w_u32(object + 0x40, 0x80090274);
        value = (value << 2) + value;
        value -= value << 4;
        w_u32(object + 0x104, (sint32)value >> 12);
        shift = r_s16(bounds + 4);
        w_u32(object + 0xA8, 0);
        w_u32(object + 0xAC, 0);
        w_u32(object + 0xB0, slot);
        w_u16(object + 0x24C, 360);
        w_u32(object + 0x84, shift);
        w_u32(0x800F6F9C + offset, object);
    }
    return 0;
}

uint32 front_start_model_preview(void)
{
    FUNCTION_MARKER(0x80090124, "FRONT.OVL");
    return front_init_model_preview();
}

void front_draw_preview_model(uint32 object)
{
    uint32 slot_x;
    uint32 slot_y;
    uint32 x;
    uint32 y;
    uint32 index;
    uint32 base;
    uint32 model;
    uint32 mesh;
    FUNCTION_MARKER(0x80090144, "FRONT.OVL");
    slot_x = r_u32(object + 0xB0);
    slot_y = r_u32(object + 0xB0);
    x = r_u32(0x800F6FCC + slot_x * 56);
    y = r_u32(0x800F6FD0 + slot_y * 56);
    w_u32(0x8001024C, x);
    w_u32(0x80010250, y);
    system_set_projection_offset((sint32)x, (sint32)y);
    system_set_projection_distance(r_s32(0x800BC6E4));
    index = r_u32(object + 0x74);
    index += r_u32(object + 0xA8);
    base = r_u32(0x80046178);
    model = base + (((index << 5) + index) << 2);
    mesh = r_u32(model);
    w_u32(object + 0x64, mesh);
    mesh = r_u32(object + 0x64);
    w_u32(object + 0x68, 0);
    if (!mesh)
    {
        index = r_u32(object + 0x74);
        w_u32(object + 0xA8, 0);
        model = base + (((index << 5) + index) << 2);
        w_u32(object + 0x64, r_u32(model));
    }
    object_draw_primitives(object);
    x = r_u32(0x80031198);
    y = r_u32(0x800311B4);
    w_u32(0x8001024C, x);
    w_u32(0x80010250, y);
    system_set_projection_offset((sint32)x, (sint32)y);
    system_set_projection_distance(r_s32(0x800313F8));
}

uint32 front_rotate_preview_model(uint32 object)
{
    uint32 slot;
    uint32 step;
    uint32 half;
    uint32 x;
    uint32 y;
    uint32 z;
    FUNCTION_MARKER(0x80090274, "FRONT.OVL");
    slot = r_u32(object + 0xB0);
    w_u32(object + 0x3C, 0x8002D2BC);
    w_u16(object + 0x24C, 0);
    step = r_u32(0x800F6FA8 + slot * 56);
    w_u32(0x800BC6CC, 1);
    if (!step)
        return 1;
    x = r_u32(object + 0x20);
    slot = r_u32(object + 0xB0);
    w_u32(object + 0x20, x + step);
    y = r_u32(object + 0x24);
    step = r_u32(0x800F6FA8 + slot * 56);
    slot = r_u32(object + 0xB0);
    z = r_u32(object + 0x28);
    w_u32(object + 0x24, y + step);
    half = r_u32(0x800F6FA8 + slot * 56);
    half += half >> 31;
    half = (uint32)((sint32)half >> 1);
    w_u32(object + 0x28, z + 1 + half);
    return half;
}

uint32 front_update_preview_camera(uint32 object)
{
    uint32 camera;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 factor;
    uint32 value;
    FUNCTION_MARKER(0x8008FC10, "FRONT.OVL");
    camera = r_u32(0x80040FDC);
    w_u32(0x800313DC, 3);
    matrix_copy_rotation(0x80035060, camera + 0x11C);
    if (r_u32(0x8003119C) == 512)
    {
        x = (uint32)(r_s16(0x80035060) * 8 / 5);
        y = (uint32)(r_s16(0x80035062) * 8 / 5);
        z = (uint32)(r_s16(0x80035064) * 8 / 5);
        w_u16(0x80035060, x);
        w_u16(0x80035062, y);
        w_u16(0x80035064, z);
    }
    factor = r_u32(0x800F6F98);
    if ((sint32)factor < 8)
    {
        x = (uint32)r_s16(0x80035066) * factor;
        if ((sint32)x < 0)
            x += 7;
        y = (uint32)r_s16(0x80035068) * factor;
        w_u16(0x80035066, (sint32)x >> 3);
        if ((sint32)y < 0)
            y += 7;
        z = (uint32)r_s16(0x8003506A) * factor;
        w_u16(0x80035068, (sint32)y >> 3);
        if ((sint32)z < 0)
            z += 7;
        w_u16(0x8003506A, (sint32)z >> 3);
    }
    camera = r_u32(0x80040FDC);
    value = (uint32)r_s16(camera + 0x128);
    value = (value << 2) + value;
    value -= value << 4;
    w_u32(object + 0xFC, (sint32)value >> 12);
    value = (uint32)r_s16(camera + 0x12A);
    value = (value << 2) + value;
    value -= value << 4;
    w_u32(object + 0x100, (sint32)value >> 12);
    value = (uint32)r_s16(camera + 0x12C);
    w_u32(0x80040C94, object);
    value = (value << 2) + value;
    value -= value << 4;
    value = (uint32)((sint32)value >> 12);
    w_u32(object + 0x104, value);
    return value;
}

uint32 front_reset_menu_packet_lists(void)
{
    uint32 base;
    FUNCTION_MARKER(0x800ADD28, "FRONT.OVL");
    base = r_u32(0x800FB7B0);
    w_u32(0x800FB7B4, 0);
    w_u32(0x800FB7C8, base);
    w_u32(0x800FB7C4, base);
    w_u32(0x800FB7C0, base);
    w_u32(0x800FB7BC, base);
    w_u32(0x800FB7B8, base);
    return base;
}

uint32 front_allocate_cached_text(uint32 packet_capacity, uint32 text_capacity)
{
    uint32 allocation;
    FUNCTION_MARKER(0x800B64EC, "FRONT.OVL");
    w_u32(0x8010D100, 0);
    w_u32(0x8010D110, 0);
    w_u32(0x8010D108, 0);
    w_u32(0x8010D10C, packet_capacity);
    w_u32(0x8010D104, text_capacity);
    allocation = heap_alloc_back(packet_capacity * 20);
    w_u32(0x8010D0F8, allocation);
    allocation = heap_alloc_back(packet_capacity * 20);
    w_u32(0x8010D0FC, allocation);
    allocation = heap_alloc_back(text_capacity << 3);
    w_u32(0x8010D114, allocation);
    return allocation;
}

uint32 front_create_cached_text(uint32 text, uint32 x, uint32 y, uint32 color)
{
    uint32 identifier;
    uint32 index;
    uint32 descriptor;
    uint32 offset;
    uint32 glyph;
    uint32 atlas;
    uint32 bank;
    uint32 shift;
    uint32 first;
    uint32 second;
    uint32 value;
    uint32 character;
    uint32 page = 0;
    sint32 previous_page = 0;
    sint32 depth;
    sint32 atlas_x;
    sint32 atlas_y;
    FUNCTION_MARKER(0x800B65E8, "FRONT.OVL");
    if (r_u8(text) & 128)
        return r_u32(0x8010D108);
    identifier = r_u32(0x8010D108);
    index = r_u32(0x8010D110);
    w_u32(0x8010D108, identifier + 1);
    descriptor = r_u32(0x8010D114) + (identifier << 3);
    w_u32(descriptor, index);
    w_u32(descriptor + 4, 0);
    character = r_u8(text);
    offset = index * 20;
    while (character)
    {
        glyph = r_u32(0x8010D168 + (character << 2));
        if ((sint32)glyph < 0)
            x += 4;
        else
        {
            bank = r_u32(0x800FB870);
            atlas = 0x8004EB70 + ((glyph + bank) << 4);
            atlas_x = r_s16(atlas);
            depth = r_s16(atlas + 12);
            atlas_y = r_s16(atlas + 2);
            shift = (2 - (uint32)depth) & 31;
            page = GetTPage(depth, 1, atlas_x, atlas_y);
            if (!previous_page)
                previous_page = (sint16)page;
            if (previous_page != (sint16)page)
            {
                ++index;
                first = r_u32(0x8010D0F8) + offset;
                second = r_u32(0x8010D0FC) + offset;
                SetDrawMode((DR_MODE *)psx_addr(first, 12), 0, 1, (uint16)previous_page, NULL);
                SetDrawMode((DR_MODE *)psx_addr(second, 12), 0, 1, (uint16)previous_page, NULL);
                previous_page = (sint16)page;
                offset += 20;
                w_u32(descriptor + 4, r_u32(descriptor + 4) + 1);
            }
            first = r_u32(0x8010D0F8) + offset;
            second = r_u32(0x8010D0FC) + offset;
            w_u8(first + 3, 4);
            w_u8(first + 7, 0x64);
            w_u8(second + 3, 4);
            w_u8(second + 7, 0x64);
            w_u16(first + 8, x);
            w_u16(second + 8, x);
            w_u16(first + 10, y);
            w_u16(second + 10, y);
            value = r_u16(atlas);
            w_u8(first + 12, (value & 63) << shift);
            value = r_u16(atlas);
            w_u8(second + 12, (value & 63) << shift);
            w_u8(first + 13, r_u16(atlas + 2));
            w_u8(second + 13, r_u16(atlas + 2));
            w_u16(first + 16, r_u16(atlas + 14));
            w_u16(second + 16, r_u16(atlas + 14));
            w_u16(first + 18, r_u16(atlas + 10));
            w_u16(second + 18, r_u16(atlas + 10));
            atlas_x = r_s16(atlas + 4);
            atlas_y = r_s16(atlas + 6);
            ++index;
            w_u16(first + 14, GetClut(atlas_x, atlas_y));
            atlas_x = r_s16(atlas + 4);
            atlas_y = r_s16(atlas + 6);
            offset += 20;
            w_u16(second + 14, GetClut(atlas_x, atlas_y));
            w_u8(first + 4, color);
            w_u8(first + 5, color >> 8);
            w_u8(first + 6, color >> 16);
            w_u8(second + 4, color);
            w_u8(second + 5, color >> 8);
            w_u8(second + 6, color >> 16);
            value = (uint32)r_s16(atlas + 14);
            x += value;
            w_u32(descriptor + 4, r_u32(descriptor + 4) + 1);
        }
        character = r_u8(++text);
    }
    first = r_u32(0x8010D0F8) + index * 20;
    second = r_u32(0x8010D0FC) + index * 20;
    ++index;
    SetDrawMode((DR_MODE *)psx_addr(first, 12), 0, 1, (uint16)page, NULL);
    SetDrawMode((DR_MODE *)psx_addr(second, 12), 0, 1, (uint16)page, NULL);
    w_u32(descriptor + 4, r_u32(descriptor + 4) + 1);
    w_u32(0x8010D110, index);
    return identifier;
}

uint32 front_initialize_ship_view(void)
{
    uint32 index;
    uint32 label;
    uint32 width;
    uint32 maximum;
    uint32 target;
    uint32 x;
    uint32 y;
    uint32 spacing;
    uint32 text;
    uint32 value;
    FUNCTION_MARKER(0x80090338, "FRONT.OVL");
    front_configure_layer(0, 0xFFFFFFFF, 169);
    front_reset_menu_packet_lists();
    label = r_u32(0x800F26E0);
    w_u32(0x800FB870, 83);
    width = front_text_width(label);
    w_u32(0x800BC600, width);
    for (index = 1; index < 4; ++index)
    {
        label = r_u32(0x800F26E0 + index * 8);
        width = front_text_width(label);
        maximum = r_u32(0x800BC600);
        if ((sint32)maximum < (sint32)width)
            w_u32(0x800BC600, width);
    }
    label = r_u32(0x800F26E4);
    width = front_text_width(label);
    w_u32(0x800BC604, width);
    for (index = 1; index < 4; ++index)
    {
        label = r_u32(0x800F26E4 + index * 8);
        width = front_text_width(label);
        maximum = r_u32(0x800BC604);
        if ((sint32)maximum < (sint32)width)
            w_u32(0x800BC604, width);
    }
    front_allocate_cached_text(300, 30);
    y = r_u32(0x800BC610);
    spacing = r_u32(0x800BC618);
    x = r_u32(0x800BC60C);
    y += spacing * 4;
    for (index = 0; index < 8; ++index)
    {
        if (index && !(index & 1))
            y += r_u32(0x800BC618);
        label = r_u32(0x800F26E0 + index * 4);
        text = front_create_cached_text(label, x + (index & 1) * 170, y, 0x808080);
        w_u32(0x800F7014 + index * 4, text);
    }
    y = 105;
    for (index = 0; index < 7; ++index)
    {
        if (index)
            y += r_u32(0x800BC618);
        label = r_u32(0x800F26B4 + index * 4);
        width = front_text_width(label);
        if (!index || r_s32(0x800BC608) < (sint32)width)
            w_u32(0x800BC608, width);
        label = r_u32(0x800F26B4 + index * 4);
        text = front_create_cached_text(label, 195, y, 0x808080);
        w_u32(0x800F703C + index * 4, text);
    }
    y = 256;
    for (index = 0; index < 7; ++index)
    {
        if (index)
            y += r_u32(0x800BC618);
        label = r_u32(0x800F26B4 + index * 4);
        text = front_create_cached_text(label, 265, y, 0x808080);
        w_u32(0x800F7064 + index * 4, text);
    }
    w_u16(0x800F3116, 0);
    /* The light helper consumes only the three RGB bytes */
    front_configure_background_light(0, 446, 262, 0x403020, 9);
    front_configure_background_light(1, 40, 40, 0x105010, 9);
    front_configure_background_light(2, 600, 440, 0x501010, 9);
    value = r_u32(0x80086924);
    target = r_u32(0x80086928);
    w_u32(0x800BC62C, value);
    w_u32(0x800BC630, target);
    w_u16(0x800BC634, r_u16(0x8008692C));
    w_u16(0x800FB838, 0xFFFF);
    w_u16(0x800FB848, 0xFFFF);
    front_load_preview_configuration();
    w_u32(0x800BC624, 11);
    w_u8(0x800BC628, 1);
#ifndef CW_EXTRACTED_XA
    while (!CdRead2(456))
    {
    }
#endif
    value = r_u32(0x800BC600);
    width = r_u32(0x800BC608);
    w_u32(0x800BC600, value + 4);
    value = r_u32(0x800BC604);
    w_u32(0x800BC608, width + 4);
    value += 4;
    w_u32(0x800BC604, value);
    return value;
}
