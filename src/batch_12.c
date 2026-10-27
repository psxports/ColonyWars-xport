#include "game_functions.h"
#include "psx_gpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void front_build_save_header(uint8 *header, char *name, uint32 blocks)
{
    char title[40];
    uint32 frames;
    uint32 index;
    uint32 frame;
    uint16 color;

    FUNCTION_MARKER(0x8009D7B0, "FRONT.OVL");
    header[0] = 'S';
    header[1] = 'C';
    frames = r_u8(0x800F2DC0u);
    header[3] = (uint8)blocks;
    header[2] = (uint8)(frames + 16u);
    if ((sint32)((uint32)strlen(name) + 12u) >= 32)
        name[19] = 0;
    sprintf(title, "%s%s", "COLONY WARS:", name);
    for (index = 0; index < 32u; ++index) {
        header[4u + index * 2u] = 0;
        header[5u + index * 2u] = 0;
    }
    front_encode_card_title(title, header + 4);
    for (index = 0; index < 28u; ++index)
        header[0x44u + index] = 0;
    if (r_u8(0x800F2DC0u) == 0u)
        return;
    for (index = 0; index < 16u; ++index) {
        color = r_u16(0x800F2DC2u + index * 2u);
        header[0x60u + index * 2u] = (uint8)color;
        header[0x61u + index * 2u] = (uint8)(color >> 8);
    }
    if (r_u8(0x800F2DC0u) == 0u)
        return;
    frame = 0u;
    do {
        for (index = 0; index < 128u; ++index)
            header[0x80u + frame * 128u + index] = r_u8(0x800F2DE2u + frame * 128u + index);
        ++frame;
    } while (frame < r_u8(0x800F2DC0u));
}

uint32 front_read_card_directory(uint32 port)
{
    FUNCTION_MARKER(0x8009D97C, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    w_u8(0x800F2D8C + port, 2);
    w_u8(0x800F2D8A + port, 0);
    w_u32(0x800F2F78 + port * 4, 0);
    return 0;
}

uint32 front_create_card_file(const char *path, uint32 blocks)
{
    FUNCTION_MARKER(0x8009DCAC, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_write_card_file(const uint8 *header, const char *path, const uint8 *data)
{
    FUNCTION_MARKER(0x8009DCF4, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_read_card_file(uint32 port, uint32 slot, uint8 *data, sint32 capacity)
{
    FUNCTION_MARKER(0x8009DE58, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_write_card_blocks(uint32 port, const char *name, const uint8 *data, uint32 length)
{
    FUNCTION_MARKER(0x8009DF54, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

uint32 front_snapshot_basic_settings(uint32 destination)
{
    uint32 record;
    uint32 offset;
    uint32 index;
    uint32 byte;
    uint32 output;
    uint32 alignment;
    uint32 values[4];
    uint32 value;
    FUNCTION_MARKER(0x8009E0E8, "FRONT.OVL");
    for (index = 0; index < 7u; ++index)
        w_u16(destination + 0x20u + index * 2u, r_u16(0x800118F4u + index * 2u));
    for (record = 0; record < 5u; ++record) {
        output = destination + 0x60u + record * 48u;
        alignment = output & 3u;
        for (offset = 0; offset < 48u; offset += 16u) {
            for (index = 0; index < 4u; ++index)
                values[index] = r_u32(0x800421C0u + record * 48u + offset + index * 4u);
            for (index = 0; index < 4u; ++index) {
                if (alignment != 0u) {
                    for (byte = 4u - alignment; byte < 4u; ++byte)
                        w_u8(output + offset + index * 4u + byte, values[index] >> (byte * 8u));
                    for (byte = 0; byte < 4u - alignment; ++byte)
                        w_u8(output + offset + index * 4u + byte, values[index] >> (byte * 8u));
                } else {
                    w_u32(output + offset + index * 4u, values[index]);
                }
            }
        }
    }
    w_u32(destination + 0x150u, r_u32(0x800311C0u));
    for (index = 0; index < 5u; ++index) {
        value = r_u32(0x8001056Cu + index * 4u);
        w_u32(destination + 0x154u + index * 4u, value);
    }
    return value;
}

uint32 front_prepare_card_monitor(void)
{
    FUNCTION_MARKER(0x8009E880, "FRONT.OVL");
    stream_clear_queue();
    stream_cancel_current();
    front_configure_layer(1u, 3u, 0xA8u);
    front_prepare_card_icons();
    w_u32(0x8010D13Cu, 8u);
    w_u32(0x8010D138u, 30u);
    w_u32(0x8010D134u, 30u);
    w_u32(0x8010D118u, 0u);
    w_u32(0x8010D11Cu, 0u);
    w_u32(0x8010D120u, 0u);
    w_u32(0x8010D12Cu, 0u);
    w_u32(0x8010D130u, 0u);
    w_u32(0x8010D15Cu, 0x800F2704u);
    w_u32(0x8010D124u, 0u);
    w_u32(0x8010D128u, 0u);
    front_set_card_texture_layout(0x290u, 0u, 0x280u, 0xE2u);
    w_u8(0x800F2D8Cu, 2u);
    w_u8(0x800F2D8Du, 2u);
    w_u8(0x800F2D8Au, 0u);
    w_u8(0x800F2D8Bu, 0u);
    return 1u;
}

uint32 front_update_card_monitor(void)
{
    uint32 frame;
    FUNCTION_MARKER(0x8009E960, "FRONT.OVL");
    if (r_u8(0x800F2D8Au) == 1u) {
        if (front_read_card_directory(0u) == 1u)
            w_u8(0x800F2D8Au, 0u);
    } else if (r_u8(0x800F2D8Bu) == 1u) {
        if (front_read_card_directory(1u) == 1u)
            w_u8(0x800F2D8Bu, 0u);
    } else {
        frame = r_u32(0x800F2D38u);
        if ((frame & 31u) == 0u)
            front_refresh_card_status(0u);
        else if (((frame + 16u) & 31u) == 0u)
            front_refresh_card_status(1u);
    }
    frame = r_u32(0x800F2D38u) + 1u;
    w_u32(0x800F2D38u, frame);
    return frame;
}

void front_run_card_monitor(uint32 enabled)
{
    FUNCTION_MARKER(0x8009E0A4, "FRONT.OVL");
    if (enabled == 0u)
        return;
    front_initialize_card();
    front_prepare_card_monitor();
    for (;;) {
        VSync(0);
        front_update_card_monitor();
    }
}

void front_apply_display_origin(void)
{
    DISPENV environment;
    uint32 x;
    uint32 y;
    FUNCTION_MARKER(0x800A6E94, "FRONT.OVL");
    gpu_get_display_env(&environment);
    x = r_u32(0x80010564u);
    y = r_u32(0x80010568u);
    environment.screen.x = (sint16)x;
    environment.screen.y = (sint16)y;
    w_u16(0x800FB8E0u, x);
    w_u16(0x800FB8E2u, y);
    PutDispEnv(&environment);
}

void front_restore_settings(uint32 settings)
{
    uint32 index;
    uint32 record;
    uint32 offset;
    uint32 byte;
    uint32 source;
    uint32 destination;
    uint32 values[4];
    uint32 current_mode;
    uint32 requested_mode;
    uint32 pal_x;
    uint32 pal_y;
    uint32 ntsc_x;
    uint32 ntsc_y;
    FUNCTION_MARKER(0x8009E50C, "FRONT.OVL");
    w_u16(0x800595F8u, r_u16(settings));
    w_u16(0x800595FAu, r_u16(settings + 2u));
    w_u16(0x800595FCu, r_u16(settings + 4u));
    w_u16(0x800595FEu, r_u16(settings + 6u));
    w_u16(0x8005960Cu, r_u16(settings + 8u));
    w_u16(0x8005960Eu, r_u16(settings + 10u));
    for (offset = 0; offset < 80u; offset += 16u) {
        for (index = 0; index < 4u; ++index)
            values[index] = r_u32(settings + 12u + offset + index * 4u);
        for (index = 0; index < 4u; ++index)
            w_u32(0x800118E0u + offset + index * 4u, values[index]);
    }
    w_u32(0x80011930u, r_u32(settings + 92u));
    for (record = 0; record < 5u; ++record) {
        source = settings + 0x60u + record * 48u;
        destination = 0x800421C0u + record * 48u;
        for (offset = 0; offset < 48u; offset += 16u) {
            for (index = 0; index < 4u; ++index)
                values[index] = r_u32(source + offset + index * 4u);
            for (index = 0; index < 4u; ++index) {
                w_u32(destination + offset + index * 4u, values[index]);
                if ((source & 3u) != 0u)
                    w_u32(destination + offset + index * 4u, values[index]);
            }
        }
    }
    pal_x = r_u32(settings + 0x154u);
    current_mode = r_u32(0x800311C0u);
    w_u32(0x8001056Cu, pal_x);
    pal_y = r_u32(settings + 0x158u);
    w_u32(0x80010570u, pal_y);
    ntsc_x = r_u32(settings + 0x15Cu);
    w_u32(0x80010574u, ntsc_x);
    ntsc_y = r_u32(settings + 0x160u);
    w_u32(0x80010578u, ntsc_y);
    requested_mode = r_u32(settings + 0x150u);
    if (current_mode != requested_mode) {
        if (requested_mode != 0u)
            front_select_pal_display();
        else
            front_select_ntsc_display();
    } else {
        if (current_mode != 0u) {
            w_u32(0x80010564u, pal_x);
            w_u32(0x80010568u, pal_y);
        } else {
            w_u32(0x80010564u, ntsc_x);
            w_u32(0x80010568u, ntsc_y);
        }
        front_apply_display_origin();
    }
    w_u32(0x8001057Cu, r_u32(settings + 0x164u));
    for (record = 0; record < 19u; ++record) {
        w_u8(0x80010478u + record, r_u8(settings + 0x1EDu + record));
        for (byte = 0; byte < 7u; ++byte)
            w_u8(0x80010490u + record * 7u + byte, r_u8(settings + 0x168u + record * 7u + byte));
    }
    for (byte = 0; byte < 7u; ++byte)
        w_u8(0x80010F5Cu + byte, r_u8(settings + 0x200u + byte));
    if (r_s16(0x800595F8u) >= 91) {
        w_u8(0x80010518u, 0u);
        w_u32(0x800F3150u, 1u);
    } else {
        w_u32(0x800F3150u, 0u);
    }
    if (r_u32(0x800595F8u) == 0u || r_s16(0x800595F8u) == 90) {
        w_u16(0x800595FCu, 0u);
        w_u16(0x800595F8u, 0u);
        w_u16(0x8005960Cu, 1u);
        w_u16(0x8005960Eu, 1u);
        w_u16(0x800595FEu, 1u);
        w_u16(0x800595FAu, 1u);
    }
}

uint32 front_store_vertical_adjustment(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800A6EE4, "FRONT.OVL");
    value = (uint32)r_s16(0x8010D0DCu) - 31u;
    w_u32(0x8001057Cu, value);
    return value;
}

void front_append_card_draw_mode(void)
{
    uint32 packet;
    uint32 page;
    uint32 table;
    FUNCTION_MARKER(0x8009EA38, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    page = GetTPage(0, 1, r_s32(0x800F2F68u), r_s32(0x800F2F6Cu));
    SetDrawMode((DR_MODE *)psx_addr(packet, 12u), 0, 0, (uint16)page, NULL);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 12u));
    w_u32(0x801132B8u, packet + 12u);
}

void front_draw_card_icon(uint32 port, uint32 slot, uint32 x, uint32 y, uint32 color, uint32 frame)
{
    uint32 packet;
    uint32 atlas_x;
    uint32 clut;
    uint32 table;
    FUNCTION_MARKER(0x8009EAB8, "FRONT.OVL");
    frame &= 255u;
    if (frame >= 3u)
        frame = 0u;
    packet = r_u32(0x801132B8u);
    w_u8(packet + 3u, 4u);
    w_u8(packet + 7u, 0x64u);
    w_u16(packet + 8u, x);
    w_u16(packet + 10u, y);
    atlas_x = r_u32(0x800F2F68u);
    w_u8(packet + 13u, slot << 4);
    w_u16(packet + 16u, 16u);
    w_u16(packet + 18u, 16u);
    w_u8(packet + 12u, ((atlas_x & 63u) << 2) + (frame << 4) + port * 48u);
    clut = GetClut(r_s32(0x800F2F70u), (sint32)(port * 15u + r_u32(0x800F2F74u) + slot));
    w_u16(packet + 14u, clut);
    w_u8(packet + 4u, color >> 16);
    w_u8(packet + 5u, color >> 8);
    w_u8(packet + 6u, color);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 20u));
    w_u32(0x801132B8u, packet + 20u);
}

void front_draw_card_icon_large(uint32 port, uint32 slot, uint32 x, uint32 y, uint32 color, uint32 frame)
{
    uint32 packet;
    uint32 atlas_x;
    uint32 uv_y;
    uint32 uv_y_next;
    uint32 uv_y_last;
    uint32 uv_x;
    uint32 uv_x_next;
    uint32 uv_x_last;
    uint32 clut;
    uint32 page;
    uint32 table;
    FUNCTION_MARKER(0x8009EBF4, "FRONT.OVL");
    frame &= 255u;
    if (frame >= 3u)
        frame = 0u;
    packet = r_u32(0x801132B8u);
    w_u8(packet + 3u, 9u);
    w_u8(packet + 7u, 0x2Cu);
    w_u16(packet + 8u, x);
    w_u16(packet + 24u, x);
    w_u16(packet + 10u, y);
    w_u16(packet + 16u, x + 32u);
    w_u16(packet + 18u, y);
    w_u16(packet + 26u, y + 32u);
    w_u16(packet + 32u, x + 32u);
    w_u16(packet + 34u, y + 32u);
    atlas_x = r_u32(0x800F2F68u);
    w_u8(packet + 13u, slot << 4);
    uv_y = r_u8(packet + 13u);
    uv_y_next = r_u8(packet + 13u);
    w_u8(packet + 12u, ((atlas_x & 63u) << 2) + (frame << 4) + port * 48u);
    uv_y_last = r_u8(packet + 13u);
    w_u8(packet + 21u, uv_y);
    uv_x = r_u8(packet + 12u);
    w_u8(packet + 29u, uv_y_next + 15u);
    uv_x_next = r_u8(packet + 12u);
    w_u8(packet + 37u, uv_y_last + 15u);
    uv_x_last = r_u8(packet + 12u);
    w_u8(packet + 28u, uv_x);
    w_u8(packet + 20u, uv_x_next + 15u);
    w_u8(packet + 36u, uv_x_last + 15u);
    clut = GetClut(r_s32(0x800F2F70u), (sint32)(port * 15u + r_u32(0x800F2F74u) + slot));
    w_u16(packet + 14u, clut);
    page = GetTPage(0, 1, r_s32(0x800F2F68u), r_s32(0x800F2F6Cu));
    w_u16(packet + 22u, page);
    w_u8(packet + 4u, color >> 16);
    w_u8(packet + 5u, color >> 8);
    w_u8(packet + 6u, color);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 40u));
    w_u32(0x801132B8u, packet + 40u);
}

void front_draw_card_entry(uint32 port, uint32 slot, uint32 x, uint32 y)
{
    uint32 header = 0x800F7090u + (port << 13) + (slot << 9);
    uint32 icon;
    sint32 frame;
    FUNCTION_MARKER(0x8009EDA4, "FRONT.OVL");
    icon = r_u8(header + 2u);
    if (icon != 16u) {
        frame = (r_s32(0x8010D12Cu) / 10) % (sint32)(icon - 16u);
        front_draw_card_icon_large(port, slot, x - 8u, y - 8u, 0x808080u, (uint8)frame);
        front_append_card_draw_mode();
    }
    if ((r_u16(header + 4u) & 128u) != 0u)
        front_draw_wide_text(header + 4u, x + 32u, y, 0x707070u);
    else
        front_draw_colored_text(header + 4u, x + 32u, y, 0x707070u);
}

static const char *front_card_format_pointer(uint32 address)
{
    uint32 length = 0u;
    while (r_u8(address + length) != 0u)
        ++length;
    return (const char *)psx_addr(address, (size_t)length + 1u);
}

void front_draw_card_directory(void)
{
    char text[48];
    uint32 port;
    uint32 x;
    uint32 column;
    uint32 color;
    uint32 state;
    uint32 labels;
    uint32 header;
    uint32 slot;
    uint32 shown;
    uint32 y;
    uint32 mode;
    uint32 blocks;
    uint32 icon;
    uint32 selected_port;
    uint32 ticks;
    sint32 frame;
    FUNCTION_MARKER(0x8009EEFC, "FRONT.OVL");
    ticks = r_u32(0x8010D12Cu);
    w_u32(0x800FB870u, 0x53u);
    w_u32(0x8010D12Cu, ticks + 1u);
    for (port = 0u; port < 2u; ++port) {
        column = port * 195u;
        x = column + 30u;
        color = 0x808080u;
        if (r_u32(0x8010D118u) != 0u)
            color = r_u32(0x8010D11Cu) == port ? 0x800080u : 0x606060u;
        state = r_u8(0x800F2D8Cu + port);
        if (state < 2u) {
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels), x, 90u, 0x80u);
            continue;
        }
        if (state == 2u) {
            mode = r_u32(0x8010D118u);
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 4u), x, 90u, mode != 0u ? 0x606060u : 0x808080u);
            continue;
        }
        if (state == 4u) {
            if (r_u32(0x8010D118u) == 3u)
                color = 0x606060u;
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 8u), x, 90u, color);
            continue;
        }
        if (state != 3u)
            continue;
        if (r_u32(0x8010D118u) != 1u) {
            blocks = r_u32(0x800F2D90u + port * 4u);
            labels = r_u32(0x8010D15Cu);
            if (blocks == 0u)
                sprintf(text, front_card_format_pointer(r_u32(labels + 0x14u)));
            else if (blocks == 1u)
                sprintf(text, front_card_format_pointer(r_u32(labels + 0x18u)));
            else
                sprintf(text, "%s%d%s", front_card_format_pointer(r_u32(labels + 0x1Cu)), (sint32)r_u32(0x800F2D90u + port * 4u), front_card_format_pointer(r_u32(labels + 0x20u)));
            if (r_u32(0x8010D11Cu) == port || r_u32(0x8010D118u) == 0u)
                front_draw_colored_text_native(text, x, 360u, 0x80808080u);
            else
                front_draw_colored_text_native(text, x, 360u, 0x60606060u);
        }
        if (r_u32(0x8010D118u) == 1u && r_u32(0x800F2F78u + port * 4u) == 0u) {
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 0xA4u), x, 90u, 0x60606060u);
            continue;
        }
        slot = 0u;
        shown = 0u;
        header = 0x800F7090u + (port << 13);
        if (r_u8(header) == 'E') {
            mode = r_u32(0x8010D118u);
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + (mode == 2u ? 0x10u : 0xCu)), x, 90u, color);
            continue;
        }
        do {
            y = 90u + shown * 18u;
            mode = r_u32(0x8010D118u);
            color = 0x808080u;
            if (mode != 0u) {
                selected_port = r_u32(0x8010D11Cu);
                if (selected_port != port || (mode != 2u && r_u32(0x800F2D90u + selected_port * 4u) == 15u))
                    color = 0x606060u;
                else if (r_u32(0x8010D120u) == slot && mode != 4u)
                    color = 0x800080u;
            }
            if (r_u32(0x8010D118u) != 1u || r_u8(0x800F2F80u + port * 15u + slot) == 1u) {
                icon = r_u8(header + 2u);
                if (icon != 16u) {
                    frame = (r_s32(0x8010D12Cu) / 10) % (sint32)(icon - 16u);
                    front_draw_card_icon(port, slot, column + 30u, y, color, (uint8)frame);
                }
                front_append_card_draw_mode();
                blocks = r_u8(header + 3u);
                labels = r_u32(0x8010D15Cu);
                if (blocks == 1u)
                    sprintf(text, front_card_format_pointer(r_u32(labels + 0x24u)));
                else
                    sprintf(text, "%s%d%s", front_card_format_pointer(r_u32(labels + 0x28u)), (sint32)blocks, front_card_format_pointer(r_u32(labels + 0x2Cu)));
                front_draw_single_page_text_native(text, column + 62u, y, color);
                ++shown;
            }
            header += 512u;
            ++slot;
        } while (r_u8(header) != 'E');
        if (r_u32(0x8010D118u) == 2u && r_s32(0x800F2D90u + port * 4u) > 0) {
            color = 0x606060u;
            if (r_u32(0x8010D11Cu) == port)
                color = r_u32(0x8010D120u) == slot ? 0x800080u : 0x808080u;
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 0x10u), x, 90u + shown * 18u, color);
        }
    }
    selected_port = r_u32(0x8010D11Cu);
    if (r_u8(0x800F2D8Cu + selected_port) == 3u) {
        mode = r_u32(0x8010D118u);
        if (mode != 0u && mode != 4u) {
            slot = r_u32(0x8010D120u);
            header = 0x800F7090u + (selected_port << 13) + (slot << 9);
            if (r_u8(header) != 'E' && (mode != 1u || r_u8(0x800F2F80u + selected_port * 15u + slot) != 0u))
                front_draw_card_entry(selected_port, slot, 30u, 386u);
        }
    }
}

void front_draw_card_menu(void)
{
    uint32 port;
    uint32 state;
    uint32 cards = 0u;
    uint32 own_files = 0u;
    uint32 free_blocks = 30u;
    uint32 labels;
    FUNCTION_MARKER(0x8009F608, "FRONT.OVL");
    front_configure_layer(0u, 0u, 0xA9u);
    front_configure_layer(1u, 3u, 0xA8u);
    for (port = 0u; port < 2u; ++port) {
        state = r_u8(0x800F2D8Cu + port);
        if (state == 3u) {
            ++cards;
            own_files += r_u32(0x800F2F78u + port * 4u);
            free_blocks = free_blocks - 15u + r_u32(0x800F2D90u + port * 4u);
        } else if (state == 4u) {
            ++cards;
        }
    }
    w_u8(0x800F2FA8u, (sint32)own_files > 0 ? 0u : 1u);
    w_u8(0x800F2FA9u, (sint32)cards > 0 ? 0u : 1u);
    w_u8(0x800F2FAAu, free_blocks - 1u < 29u && cards == 2u ? 0u : 1u);
    w_u8(0x800F2FABu, (sint32)free_blocks < 30 && (sint32)cards > 0 ? 0u : 1u);
    labels = r_u32(0x8010D15Cu);
    w_u32(0x800FB870u, 0xFDu);
    front_draw_colored_text(r_u32(labels + 0xCCu), 46u, 64u, 0x808080u);
    front_draw_card_directory();
}

void front_update_card_selection(void)
{
    uint32 buttons;
    uint32 repeat;
    uint32 moved = 0u;
    uint32 port;
    uint32 slot;
    uint32 previous;
    uint32 mode;
    uint32 state;
    uint32 own_flags;
    uint32 header;
    uint32 free_blocks;
    FUNCTION_MARKER(0x8009F7B0, "FRONT.OVL");
    buttons = r_u32(0x80053684u);
    if (buttons == 0u) {
        repeat = r_u32(0x8010D134u) - 1u;
        w_u32(0x8010D134u, repeat);
        if (repeat == 0u) {
            repeat = r_u32(0x8010D13Cu);
            buttons = r_u32(0x8004616Cu);
            w_u32(0x8010D134u, repeat);
        }
    } else {
        w_u32(0x8010D134u, r_u32(0x8010D138u));
    }
    if ((buttons & 0x1000u) != 0u) {
        mode = r_u32(0x8010D118u);
        if (mode != 1u || r_u32(0x800F2F78u + r_u32(0x8010D11Cu) * 4u) != 0u) {
            slot = r_u32(0x8010D120u) - 1u;
            w_u32(0x8010D120u, slot);
            moved = 1u;
            if ((sint32)slot < 0)
                w_u32(0x8010D120u, 15u);
        }
    } else if ((buttons & 0x4000u) != 0u) {
        mode = r_u32(0x8010D118u);
        if (mode != 1u || r_u32(0x800F2F78u + r_u32(0x8010D11Cu) * 4u) != 0u) {
            previous = r_u32(0x8010D120u);
            slot = previous + 1u;
            w_u32(0x8010D120u, slot);
            moved = 1u;
            port = r_u32(0x8010D11Cu);
            header = 0x800F7090u + (port << 13);
            if (mode == 2u) {
                if ((r_u8(header + (slot << 9)) == 'E' && r_u32(0x800F2D90u + port * 4u) == 0u) || r_u8(header + (previous << 9)) == 'E')
                    w_u32(0x8010D120u, 0u);
            } else if (r_u8(header + (slot << 9)) == 'E') {
                w_u32(0x8010D120u, 0u);
            }
            if (r_u32(0x8010D118u) == 1u) {
                port = r_u32(0x8010D11Cu);
                previous = r_u32(0x8010D120u);
                own_flags = 0x800F2F80u + port * 15u;
                if (r_u8(own_flags + previous) == 0u)
                    w_u32(0x8010D120u, previous + 1u);
                slot = r_u32(0x8010D120u);
                if (slot != previous && r_u8(own_flags + slot) == 0u) {
                    header = 0x800F7090u + (r_u32(0x8010D11Cu) << 13);
                    for (;;) {
                        slot = r_u32(0x8010D120u) + 1u;
                        w_u32(0x8010D120u, slot);
                        if (r_u8(header + (slot << 9)) == 'E')
                            w_u32(0x8010D120u, 0u);
                        slot = r_u32(0x8010D120u);
                        if (slot == previous || r_u8(own_flags + slot) != 0u)
                            break;
                    }
                }
            }
        }
    } else {
        port = r_u32(0x8010D11Cu);
        if (port == 0u && (buttons & 0x2000u) != 0u) {
            moved = 1u;
            w_u32(0x8010D11Cu, 1u);
        } else if (r_u32(0x8010D11Cu) == 1u && (buttons & 0x8000u) != 0u) {
            moved = 1u;
            w_u32(0x8010D11Cu, 0u);
        }
    }
    if (r_u32(0x8010D118u) == 1u) {
        port = r_u32(0x8010D11Cu);
        if (r_u32(0x800F2F78u + port * 4u) == 0u) {
            w_u32(0x8010D11Cu, port == 0u ? 1u : 0u);
            port = r_u32(0x8010D11Cu);
            if (r_u32(0x800F2F78u + port * 4u) == 0u)
                w_u32(0x8010D11Cu, 0u);
        }
    }
    port = r_u32(0x8010D11Cu);
    state = r_u8(0x800F2D8Cu + port);
    if (state == 2u || (r_u32(0x8010D118u) != 2u && (r_u32(0x800F2D90u + port * 4u) == 15u || state != 3u))) {
        port = r_u32(0x8010D11Cu);
        w_u32(0x8010D11Cu, port == 0u ? 1u : 0u);
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F2D8Cu + port) == 2u || (r_u32(0x8010D118u) != 2u && r_u32(0x800F2D90u + port * 4u) == 15u))
            w_u32(0x8010D11Cu, 0u);
    }
    if (r_u32(0x8010D118u) == 1u) {
        previous = r_u32(0x8010D120u);
        port = r_u32(0x8010D11Cu);
        header = 0x800F7090u + (port << 13);
        if (r_u8(header + (previous << 9)) == 'E' || r_u8(0x800F2F80u + port * 15u + previous) == 0u) {
            slot = previous - 1u;
            w_u32(0x8010D120u, slot);
            if ((sint32)slot < 0)
                w_u32(0x8010D120u, 15u);
        }
        port = r_u32(0x8010D11Cu);
        own_flags = 0x800F2F80u + port * 15u;
        slot = r_u32(0x8010D120u);
        if (slot != previous) {
            header = 0x800F7090u + (port << 13);
            for (;;) {
                if (r_u8(header + (slot << 9)) != 'E' && r_u8(own_flags + r_u32(0x8010D120u)) != 0u)
                    break;
                slot = r_u32(0x8010D120u) - 1u;
                w_u32(0x8010D120u, slot);
                if ((sint32)slot < 0)
                    w_u32(0x8010D120u, 15u);
                slot = r_u32(0x8010D120u);
                if (slot == previous)
                    break;
                header = 0x800F7090u + (r_u32(0x8010D11Cu) << 13);
            }
        }
    } else {
        slot = r_u32(0x8010D120u);
        if ((sint32)slot > 0) {
            port = r_u32(0x8010D11Cu);
            header = 0x800F7090u + (port << 13);
            if (r_u8(header + (slot << 9)) == 'E') {
                free_blocks = 0x800F2D90u + port * 4u;
                for (;;) {
                    if (r_s32(free_blocks) > 0 && r_u32(0x8010D118u) == 2u) {
                        slot = r_u32(0x8010D120u) - 1u;
                        port = r_u32(0x8010D11Cu);
                        if (r_u8(0x800F7090u + (port << 13) + (slot << 9)) != 'E')
                            break;
                    }
                    slot = r_u32(0x8010D120u) - 1u;
                    w_u32(0x8010D120u, slot);
                    if ((sint32)slot <= 0)
                        break;
                    header = 0x800F7090u + (r_u32(0x8010D11Cu) << 13);
                    if (r_u8(header + (slot << 9)) != 'E')
                        break;
                }
            }
        }
    }
    if (moved != 0u) {
        sound_play_sample_pitch(10u, 128u);
        sound_play_sample_pitch(10u, 128u);
    }
    front_draw_card_directory();
}

void front_initialize_load_selection(void)
{
    uint32 count;
    uint32 port;
    uint32 flags;
    uint32 slot;
    uint32 headers;
    FUNCTION_MARKER(0x8009FF04, "FRONT.OVL");
    front_configure_layer(1u, 1u, 0xA8u);
    count = r_u32(0x800F2F78u);
    w_u32(0x8010D130u, 0u);
    w_u16(0x800F3114u, 0u);
    w_u32(0x8010D11Cu, 0u);
    if (count == 0u)
        w_u32(0x8010D11Cu, 1u);
    port = r_u32(0x8010D11Cu);
    if (r_u32(0x800F2F78u + port * 4u) == 0u) {
        w_u32(0x8010D11Cu, 0u);
        return;
    }
    flags = 0x800F2F80u + port * 15u;
    w_u32(0x8010D120u, 0u);
    if (r_u8(flags) == 0u)
        w_u32(0x8010D120u, 1u);
    slot = r_u32(0x8010D120u);
    if (slot == 0u || r_u8(flags + slot) != 0u)
        return;
    headers = 0x800F7090u + (r_u32(0x8010D11Cu) << 13);
    do {
        slot = r_u32(0x8010D120u) + 1u;
        w_u32(0x8010D120u, slot);
        if (r_u8(headers + (slot << 9)) == 'E')
            w_u32(0x8010D120u, 0u);
        slot = r_u32(0x8010D120u);
        if (slot == 0u)
            break;
    } while (r_u8(flags + r_u32(0x8010D120u)) == 0u);
}

uint32 front_request_card_setup(void)
{
    FUNCTION_MARKER(0x800A0474, "FRONT.OVL");
    w_u32(0x8010D128u, 1u);
    return 1u;
}

uint32 front_initialize_character_picker(uint32 texture, const PSX_RECT *rectangle, uint32 characters)
{
    uint32 length = 0u;
    uint32 atlas;
    uint32 width;
    sint32 doubled_width;
    sint32 columns;
    sint32 columns16;
    sint32 numerator;
    uint32 rows;
    FUNCTION_MARKER(0x800A5A34, "FRONT.OVL");
    w_u32(0x800FB0C8u, texture);
    w_u16(0x800FB0DEu, 0u);
    w_u32(0x800FB0CCu, characters);
    while (r_u8(characters + length) != 0u)
        ++length;
    w_u16(0x800FB0D0u, length);
    w_u16(0x800FB0DAu, (uint16)rectangle->x);
    texture = r_u32(0x800FB0C8u);
    w_u16(0x800FB0DCu, (uint16)rectangle->y);
    atlas = 0x8004EB70u + (texture << 4);
    width = (uint32)r_s16(atlas + 8u) << ((2u - (uint32)r_s16(atlas + 12u)) & 31u);
    w_u16(0x800FB0D6u, width);
    w_u16(0x800FB0D8u, r_u16(atlas + 10u));
    doubled_width = (sint32)(sint16)width * 2;
    if (doubled_width == 0)
        abort();
    columns = rectangle->w / doubled_width;
    columns16 = (sint16)columns;
    numerator = (sint32)(sint16)length + columns16 - 1;
    if (columns16 == 0)
        abort();
    rows = (uint32)(numerator / columns16);
    w_u32(0x800FB0C0u, 0x808080u);
    w_u16(0x800FB0E4u, 30u);
    w_u16(0x800FB0E0u, 30u);
    w_u32(0x800FB0C4u, 0x404040u);
    w_u16(0x800FB0E2u, 8u);
    w_u16(0x800FB0D2u, (uint16)columns);
    w_u16(0x800FB0D4u, rows);
    return rows;
}

uint32 front_initialize_save_menu(void)
{
    PSX_RECT rectangle;
    uint32 first_changes;
    uint32 second_changes;
    sint32 existing_save;
    FUNCTION_MARKER(0x800A0488, "FRONT.OVL");
    if (r_u32(0x8010D128u) == 1u) {
        front_prepare_card_monitor();
        w_u32(0x8010D128u, 0u);
    }
    front_configure_layer(1u, 1u, 0xA8u);
    rectangle.x = 100;
    rectangle.y = 200;
    rectangle.w = 250;
    rectangle.h = 20;
    front_initialize_character_picker(0xFDu, &rectangle, 0x800CC7CCu);
    first_changes = r_u32(0x800F2D98u);
    second_changes = r_u32(0x800F2D9Cu);
    existing_save = r_s32(0x80010248u);
    w_u32(0x8010D130u, 0u);
    w_u8(0x8010D150u, 0u);
    w_u32(0x8010D140u, 0u);
    w_u16(0x800F3114u, 0u);
    w_u32(0x8010D148u, first_changes);
    w_u32(0x8010D14Cu, second_changes);
    if (existing_save > 0)
        w_u32(0x8010D130u, 8u);
    return 8u;
}

uint32 front_load_selected_settings(uint32 destination)
{
    uint32 slot;
    uint32 port;
    uint32 bytes;
    uint32 buffer;
    uint32 settings;
    uint32 result;
    uint32 offset;
    uint32 index;
    uint32 values[4];
    FUNCTION_MARKER(0x800A26A0, "FRONT.OVL");
    slot = r_u32(0x8010D120u);
    port = r_u32(0x8010D11Cu);
    bytes = r_u32(0x80110490u + slot * 40u + port * 640u);
    buffer = heap_alloc_back(bytes);
    if (buffer != 0u) {
        port = r_u32(0x8010D11Cu);
        slot = r_u32(0x8010D120u);
        result = front_read_card_file(port, slot, (uint8 *)psx_addr(buffer, bytes), (sint32)bytes) & 255u;
        w_u32(0x8010D124u, result);
        settings = buffer + 512u;
        if (result != 0u) {
            front_encode_settings_code(settings);
            if (strcmp(front_card_format_pointer(0x800FB220u), front_card_format_pointer(buffer + 0x407u)) == 0) {
                for (offset = 0u; offset < 528u; offset += 16u) {
                    for (index = 0u; index < 4u; ++index)
                        values[index] = r_u32(settings + offset + index * 4u);
                    for (index = 0u; index < 4u; ++index)
                        w_u32(destination + offset + index * 4u, values[index]);
                }
                for (index = 0u; index < 3u; ++index)
                    values[index] = r_u32(settings + 528u + index * 4u);
                for (index = 0u; index < 3u; ++index)
                    w_u32(destination + 528u + index * 4u, values[index]);
            } else {
                w_u32(0x8010D124u, 0u);
                front_encode_settings_code(destination);
            }
        }
        heap_free(buffer);
    }
    return r_u32(0x8010D124u);
}

void front_update_load_confirmation(void)
{
    char text[104];
    uint32 state;
    uint32 port;
    uint32 slot;
    uint32 labels;
    uint32 timer;
    uint32 width;
    uint32 title;
    sint32 stage;
    FUNCTION_MARKER(0x800A005C, "FRONT.OVL");
    state = r_u32(0x8010D130u);
    if (state == 0u) {
        front_configure_layer(0u, 0u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        w_u32(0x8010D118u, 1u);
        front_update_card_selection();
        w_u32(0x8010D118u, 0u);
        if ((r_u32(0x80053684u) & 0x40u) == 0u)
            return;
        port = r_u32(0x8010D11Cu);
        slot = r_u32(0x8010D120u);
        if (r_u8(0x800F2F80u + port * 15u + slot) != 1u || r_u8(0x800F2D8Cu + port) != 3u)
            return;
        sound_play_sample_pitch(11u, 128u);
        sound_play_sample_pitch(11u, 128u);
        w_u16(0x800F3118u, 0u);
        w_u32(0x8010D130u, 1u);
        w_u32(0x800F2D3Cu, 18u);
    } else if (state == 1u) {
        front_configure_layer(0u, 0xFFFFFFFFu, 0xA9u);
        front_configure_layer(1u, 0xFFFFFFFFu, 0xA8u);
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        front_draw_colored_text(r_u32(labels + 0xA8u), 44u, 136u, 0x808080u);
        timer = r_u32(0x800F2D3Cu);
        if (timer == 0u) {
            if (front_load_selected_settings(0x8010D578u) == 1u) {
                w_u32(0x8010D130u, 2u);
                w_u8(0x800F2FA8u, 0u);
                timer = 3u;
            } else {
                w_u16(0x800F3118u, 1u);
                w_u32(0x8010D130u, 3u);
                timer = 120u;
            }
            w_u32(0x800F2D3Cu, timer);
            timer = r_u32(0x800F2D3Cu);
        }
        w_u32(0x800F2D3Cu, timer - 1u);
    } else if (state == 2u) {
        front_configure_layer(0u, 4u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        stage = r_s16(0x8010D580u);
        w_u32(0x800FB870u, 0xFDu);
        if (stage < 90)
            title = r_u32(0x800F2468u + ((uint32)stage << 2));
        else
            title = r_u32(0x800F2350u + ((uint32)stage << 2));
        front_draw_colored_text(title, 46u, 64u, 0x808080u);
        labels = r_u32(0x8010D15Cu);
        sprintf(text, "%s(", front_card_format_pointer(r_u32(labels + 0xACu)));
        front_draw_colored_text_native(text, 44u, 136u, 0x808080u);
        width = front_text_width_native(text);
        port = r_u32(0x8010D11Cu);
        slot = r_u32(0x8010D120u);
        title = 0x800F7094u + (port << 13) + (slot << 9);
        front_draw_wide_text(title, width + 44u, 136u, 0x808080u);
        port = r_u32(0x8010D11Cu);
        slot = r_u32(0x8010D120u);
        title = 0x800F7094u + (port << 13) + (slot << 9);
        width += front_wide_text_width(title);
        front_draw_colored_text(0x80086B50u, width + 44u, 136u, 0x808080u);
        timer = r_u32(0x800F2D3Cu);
        if (timer == 0u) {
            w_u16(0x800F3118u, 1u);
            w_u16(0x800F3114u, 1u);
            timer = r_u32(0x800F2D3Cu);
        }
        if ((sint32)timer >= 0)
            w_u32(0x800F2D3Cu, timer - 1u);
    } else if (state == 3u) {
        front_configure_layer(0u, 5u, 0xA9u);
        timer = r_u32(0x800F2D3Cu);
        w_u32(0x800FB870u, 0xFDu);
        timer -= 1u;
        w_u32(0x800F2D3Cu, timer);
        if (timer == 0u || (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            w_u32(0x8010D130u, 0u);
            front_configure_layer(0u, 0u, 0xA9u);
            front_configure_layer(1u, 1u, 0xA8u);
        }
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + 0xB0u), 44u, 136u, 0x808080u);
    }
}

void front_draw_text_entry_frame(uint32 texture, const PSX_RECT *rectangle, uint32 color)
{
    uint32 packet;
    uint32 first;
    uint32 middle;
    uint32 last;
    uint32 mode_packet;
    uint32 table;
    uint32 value;
    uint32 other;
    uint32 bottom;
    uint32 shift;
    uint32 clut;
    uint32 page;
    uint32 width;
    sint32 depth;
    FUNCTION_MARKER(0x800A63C0, "FRONT.OVL");
    packet = r_u32(0x801132B8u);
    w_u8(packet + 3u, 4u);
    w_u8(packet + 7u, 0x64u);
    w_u8(packet + 23u, 4u);
    w_u8(packet + 27u, 0x64u);
    w_u8(packet + 43u, 9u);
    w_u8(packet + 47u, 0x2Cu);
    first = 0x8004EB70u + (texture << 4);
    middle = first + 16u;
    last = first + 32u;
    value = (uint32)rectangle->x;
    other = (uint32)rectangle->y;
    width = (uint32)rectangle->w;
    depth = r_s16(first + 12u);
    shift = (2u - (uint32)depth) & 31u;
    w_u16(packet + 8u, value);
    w_u16(packet + 10u, other);
    w_u8(packet + 12u, (r_u16(first) & 63u) << shift);
    w_u8(packet + 13u, r_u16(first + 2u));
    w_u16(packet + 16u, (uint32)r_s16(first + 8u) << shift);
    w_u16(packet + 18u, r_u16(first + 10u));
    clut = GetClut(r_s16(first + 4u), r_s16(first + 6u));
    w_u16(packet + 14u, clut);
    w_u8(packet + 32u, (r_u16(last) & 63u) << shift);
    w_u8(packet + 33u, r_u16(last + 2u));
    value = (uint32)r_s16(last + 8u) << shift;
    other = r_u16(packet + 10u);
    w_u16(packet + 36u, value);
    w_u16(packet + 38u, r_u16(last + 10u));
    value = r_u16(packet + 8u);
    bottom = (uint32)r_s16(last + 8u) << shift;
    w_u16(packet + 30u, other);
    w_u16(packet + 28u, value + width - bottom);
    clut = GetClut(r_s16(last + 4u), r_s16(last + 6u));
    value = r_u16(packet + 8u);
    other = r_u16(packet + 16u);
    bottom = r_u16(packet + 30u);
    w_u16(packet + 48u, value + other);
    value = r_u16(packet + 10u);
    other = r_u16(packet + 28u);
    width = r_u16(packet + 48u);
    w_u16(packet + 34u, clut);
    w_u16(packet + 50u, value);
    value = r_u16(packet + 50u);
    w_u16(packet + 58u, bottom);
    w_u16(packet + 56u, other);
    w_u16(packet + 64u, width);
    w_u16(packet + 66u, value + r_u16(middle + 10u));
    value = r_u16(packet + 56u);
    other = r_u16(packet + 66u);
    w_u16(packet + 72u, value);
    w_u16(packet + 74u, other);
    w_u8(packet + 52u, (r_u16(middle) & 63u) << shift);
    value = r_u8(packet + 52u) + 1u;
    other = r_u16(middle + 2u);
    w_u8(packet + 60u, value);
    w_u8(packet + 53u, other);
    if ((value & 255u) == 0u)
        w_u8(packet + 60u, 255u);
    value = r_u8(packet + 53u);
    other = r_u8(packet + 52u);
    w_u8(packet + 61u, value);
    value = r_u8(packet + 53u);
    w_u8(packet + 68u, other);
    w_u8(packet + 69u, value + r_u8(middle + 10u));
    value = r_u8(packet + 60u);
    other = r_u8(packet + 69u);
    w_u8(packet + 76u, value);
    w_u8(packet + 77u, other);
    clut = GetClut(r_s16(middle + 4u), r_s16(middle + 6u));
    w_u16(packet + 54u, clut);
    page = GetTPage(depth, 1, r_s16(middle), r_s16(middle + 2u));
    w_u16(packet + 62u, page);
    w_u8(packet + 4u, color);
    w_u8(packet + 5u, color >> 8);
    w_u8(packet + 24u, color);
    w_u8(packet + 25u, color >> 8);
    w_u8(packet + 44u, color);
    w_u8(packet + 45u, color >> 8);
    w_u8(packet + 6u, color >> 16);
    w_u8(packet + 26u, color >> 16);
    w_u8(packet + 46u, color >> 16);
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, packet + 80u);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 20u));
    mode_packet = r_u32(0x801132B8u);
    page = GetTPage(depth, 1, r_s16(first), r_s16(first + 2u));
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 1, (uint16)page, NULL);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(mode_packet, 12u));
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, mode_packet + 12u);
    AddPrim(psx_addr(table, 4u), psx_addr(packet + 20u, 20u));
    mode_packet = r_u32(0x801132B8u);
    page = GetTPage(depth, 1, r_s16(last), r_s16(last + 2u));
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 1, (uint16)page, NULL);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(mode_packet, 12u));
    table = r_u32(0x801132BCu);
    w_u32(0x801132B8u, mode_packet + 12u);
    AddPrim(psx_addr(table, 4u), psx_addr(packet + 40u, 40u));
}

void front_format_selected_card(void)
{
    FUNCTION_MARKER(0x800A29D4, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
}

uint32 front_update_character_picker(void)
{
    uint32 buttons;
    uint32 value;
    uint32 columns;
    uint32 moved = 0u;
    uint32 x;
    uint32 y;
    uint32 start_x;
    uint32 right;
    uint32 step_x;
    uint32 step_y;
    uint32 characters;
    uint32 character;
    uint32 index = 0u;
    uint32 glyph;
    uint32 atlas;
    uint32 shift;
    uint32 packet;
    uint32 table;
    uint32 color;
    uint32 selected_color;
    uint32 ordinary_color;
    uint32 selected_character;
    uint32 clut;
    uint16 page = 0u;
    uint16 previous_page = 0u;
    sint16 space_index = -1;
    sint16 space_x = 0;
    sint16 space_y = 0;
    sint32 page_x;
    sint32 page_y;
    sint32 depth;
    FUNCTION_MARKER(0x800A5BA4, "FRONT.OVL");
    buttons = r_u32(0x80053684u);
    if (buttons == 0u) {
        value = r_u16(0x800FB0E0u) - 1u;
        w_u16(0x800FB0E0u, value);
        if ((uint16)value == 0u) {
            buttons = r_u32(0x8004616Cu);
            w_u16(0x800FB0E0u, r_u16(0x800FB0E2u));
        }
    } else {
        w_u16(0x800FB0E0u, r_u16(0x800FB0E4u));
    }
    if ((buttons & 0x8000u) != 0u) {
        value = r_u16(0x800FB0DEu) - 1u;
        w_u16(0x800FB0DEu, value);
        moved = 1u;
        if ((sint16)value < 0)
            w_u16(0x800FB0DEu, r_u16(0x800FB0D0u) + 1u);
    }
    if ((buttons & 0x2000u) != 0u) {
        value = r_u16(0x800FB0DEu) + 1u;
        w_u16(0x800FB0DEu, value);
        moved = 1u;
        if ((sint32)(sint16)value >= r_s16(0x800FB0D0u) + 2)
            w_u16(0x800FB0DEu, 0u);
    }
    if ((buttons & 0x1000u) != 0u) {
        value = r_u16(0x800FB0DEu);
        columns = r_u16(0x800FB0D2u);
        value -= columns;
        w_u16(0x800FB0DEu, value);
        moved = 1u;
        if ((sint16)value < 0) {
            value += (uint32)((sint32)(sint16)columns * r_s16(0x800FB0D4u));
            w_u16(0x800FB0DEu, value);
            if ((sint32)(sint16)value >= r_s16(0x800FB0D0u) + 2)
                w_u16(0x800FB0DEu, value - columns);
        }
    }
    if ((buttons & 0x4000u) != 0u) {
        value = r_u16(0x800FB0DEu);
        columns = r_u16(0x800FB0D2u);
        value += columns;
        w_u16(0x800FB0DEu, value);
        moved = 1u;
        if ((sint32)(sint16)value >= r_s16(0x800FB0D0u) + 2) {
            value -= (uint32)((sint32)(sint16)columns * (r_s16(0x800FB0D4u) - 1));
            w_u16(0x800FB0DEu, value);
            if ((sint16)value >= (sint16)columns)
                w_u16(0x800FB0DEu, value - columns);
        }
    }
    if (moved != 0u) {
        sound_play_sample_pitch(10u, 128u);
        sound_play_sample_pitch(10u, 128u);
    }
    x = (uint32)r_s16(0x800FB0DAu);
    y = (uint32)r_s16(0x800FB0DCu);
    selected_color = r_u32(0x800FB0C0u);
    step_x = (uint32)r_s16(0x800FB0D6u) << 1;
    columns = (uint32)r_s16(0x800FB0D2u);
    characters = r_u32(0x800FB0CCu);
    ordinary_color = r_u32(0x800FB0C4u);
    value = r_u32(0x800FB0C8u);
    step_y = (uint32)r_s16(0x800FB0D8u) * 3u;
    w_u32(0x800FB870u, value);
    character = r_u8(characters);
    step_y = (uint32)((sint32)step_y >> 1);
    start_x = x;
    right = x + columns * step_x - 1u;
    while (character != 0u) {
        glyph = r_u32(0x8010D168u + character * 4u);
        if ((sint32)glyph >= 0) {
            atlas = 0x8004EB70u + ((glyph + r_u32(0x800FB870u)) << 4);
            depth = r_s16(atlas + 12u);
            shift = (2u - (uint32)depth) & 31u;
            page_x = (r_s16(atlas) / 64) * 64;
            page_y = (r_s16(atlas + 2u) / 256) * 256;
            page = GetTPage(depth, 1, page_x, page_y);
            if (previous_page == 0u)
                previous_page = page;
            if (previous_page != page) {
                packet = r_u32(0x801132B8u);
                value = previous_page;
                previous_page = page;
                SetDrawMode((DR_MODE *)psx_addr(packet, 12u), 0, 1, (uint16)value, NULL);
                table = r_u32(0x801132BCu);
                AddPrim(psx_addr(table, 4u), psx_addr(packet, 12u));
                w_u32(0x801132B8u, packet + 12u);
            }
            packet = r_u32(0x801132B8u);
            w_u8(packet + 3u, 4u);
            w_u8(packet + 7u, 0x64u);
            w_u16(packet + 8u, x);
            w_u16(packet + 10u, y);
            w_u8(packet + 12u, ((uint32)r_s16(atlas) - (uint32)page_x) << shift);
            w_u8(packet + 13u, r_u8(atlas + 2u) - (uint32)page_y);
            w_u16(packet + 16u, (uint32)r_s16(atlas + 8u) << shift);
            w_u16(packet + 18u, r_u16(atlas + 10u));
            clut = GetClut(r_s16(atlas + 4u), r_s16(atlas + 6u));
            w_u16(packet + 14u, clut);
            color = (uint32)r_s16(0x800FB0DEu) == index ? selected_color : ordinary_color;
            w_u8(packet + 4u, color);
            w_u8(packet + 5u, color >> 8);
            w_u8(packet + 6u, color >> 16);
            w_u8(packet + 7u, r_u8(packet + 7u) | 2u);
            table = r_u32(0x801132BCu);
            x += step_x;
            AddPrim(psx_addr(table, 4u), psx_addr(packet, 20u));
            if ((sint32)right < (sint32)x) {
                x = start_x;
                y += step_y;
            }
            w_u32(0x801132B8u, packet + 20u);
        } else if (glyph == 0xFFFFFFFFu) {
            space_index = (sint16)index;
            space_x = (sint16)(x + (uint32)((r_s16(0x800FB0D6u) - 15) >> 1));
            space_y = (sint16)(y + (uint32)((r_s16(0x800FB0D8u) - 5) >> 1));
            x += step_x;
            if ((sint32)right < (sint32)x) {
                x = start_x;
                y += step_y;
            }
        }
        ++characters;
        character = r_u8(characters);
        ++index;
    }
    packet = r_u32(0x801132B8u);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12u), 0, 1, page, NULL);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table, 4u), psx_addr(packet, 12u));
    value = (uint32)r_s16(0x800FB0D6u);
    w_u32(0x801132B8u, packet + 12u);
    columns = (uint32)r_s16(0x800FB0DEu);
    characters = r_u32(0x800FB0CCu);
    x += (uint32)((sint32)(value - 15u) >> 1);
    value = (uint32)r_s16(0x800FB0D8u);
    selected_character = r_u8(characters + columns);
    w_u32(0x800FB870u, 0u);
    y += (uint32)((sint32)(value - 5u) >> 1);
    if (space_index != -1) {
        if (columns == (uint32)(sint32)space_index) {
            front_draw_colored_text(0x80086C00u, (uint32)(sint32)space_x, (uint32)(sint32)space_y, selected_color);
            selected_character = 32u;
        } else {
            front_draw_colored_text(0x80086C00u, (uint32)(sint32)space_x, (uint32)(sint32)space_y, ordinary_color);
        }
    }
    if ((uint32)r_s16(0x800FB0DEu) == index) {
        front_draw_colored_text(0x80086C04u, x, y, selected_color);
        selected_character = 8u;
    } else {
        front_draw_colored_text(0x80086C04u, x, y, ordinary_color);
    }
    x += step_x;
    ++index;
    if ((sint32)right < (sint32)x) {
        x = start_x;
        y += step_y;
    }
    if ((uint32)r_s16(0x800FB0DEu) == index) {
        front_draw_colored_text(0x80086C08u, x, y, selected_color);
        selected_character = 13u;
    } else {
        front_draw_colored_text(0x80086C08u, x, y, ordinary_color);
    }
    if ((buttons & 0x40u) == 0u)
        return 0u;
    sound_play_sample_pitch(11u, 128u);
    sound_play_sample_pitch(11u, 128u);
    return selected_character;
}

void front_update_save_menu(void)
{
    char text[144];
    PSX_RECT rectangle;
    const char *saved_name;
    uint32 labels;
    uint32 title;
    uint32 state;
    uint32 timer;
    uint32 port;
    uint32 slot;
    uint32 entry;
    uint32 value;
    uint32 other;
    uint32 character;
    uint32 length;
    sint32 stage;
    FUNCTION_MARKER(0x800A0568, "FRONT.OVL");
    stage = r_s16(0x8005960Cu);
    w_u32(0x800FB870u, 0xFDu);
    title = r_u32((stage < 90 ? 0x800F2468u : 0x800F2350u) + ((uint32)stage << 2));
    front_draw_colored_text(title, 46u, 64u, 0x808080u);
    if (r_u32(0x8010D130u) == 8u) {
        front_configure_layer(0u, 5u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        value = r_u32(0x80053684u);
        w_u32(0x800FB870u, 0xFDu);
        if ((value & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            value = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, value & ~0x40u);
        } else {
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 0xBCu), 44u, 136u, 0x808080u);
            value = r_u32(0x80010248u);
            labels = r_u32(0x8010D15Cu);
            sprintf(text, front_card_format_pointer(r_u32(labels + (value == 1u ? 0xC4u : 0xC0u))), (sint32)value);
            front_draw_colored_text_native(text, 44u, 156u, 0x808080u);
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 0xC8u), 44u, 176u, 0x808080u);
        }
    }
    if (r_u32(0x8010D130u) != 0u) {
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F2D8Cu + port) == 2u || r_u32(0x8010D148u + port * 4u) != r_u32(0x800F2D98u + port * 4u)) {
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 0xFFFFFFFFu);
            w_u32(0x800F2D40u, 120u);
        }
    }
    other = r_u32(0x800F2D9Cu);
    value = r_u32(0x800F2D98u);
    w_u32(0x8010D14Cu, other);
    state = r_u32(0x8010D130u);
    w_u32(0x8010D148u, value);
    if (state == 0xFFFFFFFFu) {
        w_u32(0x800BC80Cu, 0u);
        front_configure_layer(0u, 5u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        timer = r_u32(0x800F2D40u);
        w_u32(0x800FB870u, 0xFDu);
        timer -= 1u;
        w_u32(0x800F2D40u, timer);
        if (timer == 0u || (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            value = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, value & ~0x40u);
        } else {
            port = r_u32(0x8010D11Cu);
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + (port == 0u ? 0x40u : 0x44u)), 44u, 136u, 0x808080u);
        }
    } else if (state == 5u) {
        front_configure_layer(0u, 5u, 0xA9u);
        timer = r_u32(0x800F2D40u);
        w_u32(0x800FB870u, 0xFDu);
        timer -= 1u;
        w_u32(0x800F2D40u, timer);
        if (timer == 0u || (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            w_u32(0x8010D130u, 2u);
            w_u32(0x80053684u, r_u32(0x80053684u) & ~0x40u);
            front_configure_layer(0u, 0u, 0xA9u);
        } else {
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 0x54u), 44u, 136u, 0x808080u);
            port = r_u32(0x8010D11Cu);
            slot = r_u32(0x8010D144u);
            front_draw_card_entry(port, slot, 52u, 166u);
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 0x58u), 44u, 194u, 0x808080u);
        }
    }
    if (r_u32(0x8010D130u) == 6u) {
        front_configure_layer(0u, 5u, 0xA9u);
        timer = r_u32(0x800F2D40u);
        w_u32(0x800FB870u, 0xFDu);
        timer -= 1u;
        w_u32(0x800F2D40u, timer);
        if (timer == 0u || (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            value = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, value & ~0x40u);
            front_configure_layer(0u, 0u, 0xA9u);
        } else {
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + 0x60u), 44u, 136u, 0x808080u);
        }
    }
    state = r_u32(0x8010D130u);
    if (state == 0u) {
        front_configure_layer(0u, 0u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        w_u32(0x8010D118u, 2u);
        front_update_card_selection();
        w_u32(0x8010D118u, 0u);
        if ((r_u32(0x80053684u) & 0x40u) != 0u) {
            port = r_u32(0x8010D11Cu);
            value = r_u8(0x800F2D8Cu + port);
            if (value >= 2u && value != 2u) {
                sound_play_sample_pitch(11u, 128u);
                sound_play_sample_pitch(11u, 128u);
                w_u32(0x8010D130u, 1u);
            }
        }
    } else if (state == 1u) {
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F2D8Cu + port) == 4u) {
            if (r_u32(0x800BC80Cu) == 0u) {
                w_u32(0x800BC80Cu, 1u);
                front_configure_layer(0u, 2u, 0xA7u);
            }
            port = r_u32(0x8010D11Cu);
            w_u16(0x800F3118u, 0u);
            w_u32(0x800FB870u, 0xFDu);
            labels = r_u32(0x8010D15Cu);
            front_draw_colored_text(r_u32(labels + (port == 0u ? 0x30u : 0x34u)), 44u, 136u, 0x808080u);
            if ((r_u32(0x80053684u) & 0x80u) != 0u) {
                sound_play_sample_pitch(11u, 128u);
                sound_play_sample_pitch(11u, 128u);
                w_u32(0x8010D130u, 3u);
                w_u32(0x800F2D40u, 18u);
                w_u32(0x800BC80Cu, 0u);
                front_configure_layer(0u, 0u, 0xA9u);
            }
            if ((r_u32(0x80053684u) & 0x10u) != 0u) {
                sound_play_sample_pitch(11u, 128u);
                sound_play_sample_pitch(11u, 128u);
                w_u16(0x800F3118u, 1u);
                w_u32(0x8010D130u, 0u);
                w_u32(0x800BC80Cu, 0u);
                front_configure_layer(0u, 0u, 0xA9u);
            }
        } else {
            slot = r_u32(0x8010D120u);
            port = r_u32(0x8010D11Cu);
            if (r_u8(0x800F7090u + (port << 13) + (slot << 9)) == 'E') {
                w_u32(0x8010D130u, 2u);
            } else {
                front_configure_layer(0u, 4u, 0xA7u);
                w_u16(0x800F3118u, 0u);
                w_u32(0x800FB870u, 0xFDu);
                value = r_u32(0x8010D12Cu);
                labels = r_u32(0x8010D15Cu);
                w_u32(0x8010D12Cu, value + 1u);
                front_draw_colored_text(r_u32(labels + 0x38u), 44u, 136u, 0x808080u);
                front_draw_card_entry(r_u32(0x8010D11Cu), r_u32(0x8010D120u), 52u, 166u);
                labels = r_u32(0x8010D15Cu);
                front_draw_colored_text(r_u32(labels + 0x3Cu), 44u, 194u, 0x808080u);
                if ((r_u32(0x80053684u) & 0x80u) != 0u) {
                    sound_play_sample_pitch(11u, 128u);
                    sound_play_sample_pitch(11u, 128u);
                    w_u16(0x800F3118u, 1u);
                    w_u32(0x8010D130u, 2u);
                    port = r_u32(0x8010D11Cu);
                    slot = r_u32(0x8010D120u);
                    if (r_u8(0x800F2F80u + port * 15u + slot) == 1u) {
                        port = r_u32(0x8010D11Cu);
                        slot = r_u32(0x8010D120u);
                        entry = 0x80110478u + port * 640u + slot * 40u;
                        saved_name = front_card_format_pointer(entry + 12u);
                        strcpy((char *)psx_addr(0x8010D150u, strlen(saved_name) + 1u), saved_name);
                        length = 0u;
                        while (r_u8(0x8010D150u + length) != 0u)
                            ++length;
                        w_u32(0x8010D140u, length);
                    }
                }
                if ((r_u32(0x80053684u) & 0x10u) != 0u) {
                    sound_play_sample_pitch(11u, 128u);
                    sound_play_sample_pitch(11u, 128u);
                    w_u16(0x800F3118u, 1u);
                    w_u32(0x8010D130u, 0u);
                    front_configure_layer(0u, 0u, 0xA9u);
                }
            }
        }
    }
    state = r_u32(0x8010D130u);
    if (state == 3u) {
        front_configure_layer(0u, 0xFFFFFFFFu, 0xA9u);
        front_configure_layer(1u, 0xFFFFFFFFu, 0xA8u);
        port = r_u32(0x8010D11Cu);
        w_u32(0x800FB870u, 0xFDu);
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + (port == 0u ? 0x48u : 0x4Cu)), 44u, 136u, 0x808080u);
        timer = r_u32(0x800F2D40u);
        if ((sint32)timer > 0) {
            w_u32(0x800F2D40u, timer - 1u);
            return;
        }
        front_refresh_card_status(r_u32(0x8010D11Cu));
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F2D8Cu + port) == 2u || r_u32(0x8010D148u + port * 4u) != r_u32(0x800F2D98u + port * 4u)) {
            value = r_u32(0x800F2D98u);
            other = r_u32(0x800F2D9Cu);
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 0xFFFFFFFFu);
            w_u32(0x800F2D40u, 120u);
            w_u32(0x8010D148u, value);
            w_u32(0x8010D14Cu, other);
        } else {
            front_format_selected_card();
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 2u);
        }
    } else if (state == 2u) {
        front_configure_layer(0u, 4u, 0xA9u);
        w_u16(0x800F3118u, 1u);
        character = front_update_character_picker();
        if ((character & 255u) == 8u) {
            length = r_u32(0x8010D140u);
            if ((sint32)length > 0) {
                --length;
                w_u32(0x8010D140u, length);
                w_u8(0x8010D150u + length, 0u);
            }
        } else if ((character & 255u) == 13u) {
            w_u16(0x800F3118u, 0u);
            w_u32(0x8010D130u, 4u);
            w_u32(0x800F2D40u, 18u);
        } else if ((character & 255u) != 0u) {
            length = r_u32(0x8010D140u);
            if ((sint32)length < 8) {
                w_u8(0x8010D150u + length, character);
                length = r_u32(0x8010D140u) + 1u;
                w_u32(0x8010D140u, length);
                w_u8(0x8010D150u + length, 0u);
            }
        }
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        rectangle.x = 40;
        rectangle.y = 130;
        rectangle.w = 120;
        rectangle.h = 10;
        value = front_text_width(r_u32(labels + 0x50u));
        labels = r_u32(0x8010D15Cu);
        rectangle.w = (sint16)(value + 14u);
        front_draw_colored_text(r_u32(labels + 0x50u), 48u, 134u, 0x808080u);
        front_draw_colored_text(0x8010D150u, (uint32)(sint32)rectangle.w + 44u, 136u, 0x808080u);
        front_draw_text_entry_frame(440u, &rectangle, 0x808080u);
    } else if (state == 4u) {
        if (r_u32(0x800F2D40u) == 18u) {
            for (value = 21u; value != 0u; --value)
                text[value - 1u] = 0;
            memcpy(text, "BASLUS-00543", 13u);
            strncat(text, (const char *)psx_addr(0x8010D150u, 8u), 8u);
            for (slot = 0u; slot < 15u; ++slot) {
                if (slot != r_u32(0x8010D120u)) {
                    port = r_u32(0x8010D11Cu);
                    entry = 0x80110478u + slot * 40u + port * 640u;
                    if (strncmp(text, (const char *)psx_addr(entry, 20u), 20u) == 0) {
                        w_u32(0x8010D130u, 5u);
                        w_u32(0x800F2D40u, 120u);
                        w_u32(0x8010D144u, slot);
                        w_u16(0x800F3118u, 1u);
                        front_configure_layer(0u, 1u, 0xA8u);
                    }
                }
            }
        }
        if (r_u32(0x8010D130u) != 4u)
            return;
        front_configure_layer(1u, 0xFFFFFFFFu, 0xA8u);
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        front_draw_colored_text(r_u32(labels + 0x5Cu), 44u, 136u, 0x808080u);
        timer = r_u32(0x800F2D40u);
        if ((sint32)timer > 0) {
            w_u32(0x800F2D40u, timer - 1u);
            return;
        }
        front_refresh_card_status(r_u32(0x8010D11Cu));
        port = r_u32(0x8010D11Cu);
        if (r_u8(0x800F2D8Cu + port) == 2u || r_u32(0x8010D148u + port * 4u) != r_u32(0x800F2D98u + port * 4u)) {
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 0xFFFFFFFFu);
            value = r_u32(0x800F2D98u);
            other = r_u32(0x800F2D9Cu);
            w_u32(0x800F2D40u, 120u);
            w_u32(0x8010D148u, value);
            w_u32(0x8010D14Cu, other);
            return;
        }
        slot = r_u32(0x8010D120u);
        if (r_u8(0x800F7090u + (port << 13) + (slot << 9)) != 'E') {
            entry = 0x80110478u + port * 640u + slot * 40u;
            sprintf(text, "bu%d0:%s", (sint32)port, front_card_format_pointer(entry));
            /* Memory card deletion is disabled */
        }
        front_snapshot_basic_settings(0x80010258u);
        port = r_u32(0x8010D11Cu);
        if ((front_save_named_settings(0x8010D150u, port, 0x80010258u) & 255u) == 1u) {
            w_u8(0x800F2FA8u, 0u);
            w_u32(0x8010D130u, 7u);
            w_u32(0x800F2D40u, 3u);
        } else {
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 6u);
            w_u32(0x800F2D40u, 120u);
        }
    } else if (state == 7u) {
        front_configure_layer(0u, 5u, 0xA9u);
        front_configure_layer(1u, 3u, 0xA8u);
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        sprintf(text, "%s(%s%s).", front_card_format_pointer(r_u32(labels + 0x64u)), "COLONY WARS:", front_card_format_pointer(0x8010D150u));
        front_draw_colored_text_native(text, 44u, 136u, 0x808080u);
        timer = r_u32(0x800F2D40u);
        if (timer == 0u) {
            port = r_u32(0x8010D11Cu);
            w_u8(0x800F2D8Cu + port, 0u);
            port = r_u32(0x8010D11Cu);
            w_u8(0x800F2D8Au + port, 1u);
            w_u16(0x800F3118u, 1u);
            timer = r_u32(0x800F2D40u);
        }
        if ((sint32)timer <= 0 && (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            value = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, value & ~0x40u);
            front_configure_layer(0u, 0u, 0xA9u);
        }
        timer = r_u32(0x800F2D40u);
        if ((sint32)timer >= 0)
            w_u32(0x800F2D40u, timer - 1u);
    }
}

void front_initialize_card_operation(void)
{
    uint32 first_changes;
    uint32 second_changes;
    FUNCTION_MARKER(0x800A1580, "FRONT.OVL");
    front_configure_layer(1u, 1u, 0xA8u);
    first_changes = r_u32(0x800F2D98u);
    second_changes = r_u32(0x800F2D9Cu);
    w_u32(0x8010D130u, 0u);
    w_u16(0x800F3114u, 0u);
    w_u32(0x8010D148u, first_changes);
    w_u32(0x8010D14Cu, second_changes);
}

void front_update_card_copy_menu(void)
{
    uint32 state;
    uint32 port;
    uint32 destination;
    uint32 slot;
    uint32 status;
    uint32 timer;
    uint32 labels;
    uint32 value;
    uint32 label;
    FUNCTION_MARKER(0x800A15D8, "FRONT.OVL");
    state = r_u32(0x8010D130u);
    if (state == 0u) {
        front_configure_layer(0u, 0u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        w_u32(0x8010D118u, 3u);
        front_update_card_selection();
        w_u32(0x8010D118u, 0u);
        if ((r_u32(0x80053684u) & 0x40u) == 0u)
            return;
        port = r_u32(0x8010D11Cu);
        status = r_u8(0x800F2D8Cu + port);
        if (status != 3u)
            return;
        sound_play_sample_pitch(11u, 128u);
        sound_play_sample_pitch(11u, 128u);
        port = r_u32(0x8010D11Cu);
        destination = port == 0u ? 1u : 0u;
        value = r_u8(0x800F2D8Cu + destination);
        if (value == 4u) {
            w_u16(0x800F3118u, 0u);
            w_u32(0x8010D130u, 6u);
            w_u32(0x800F2D44u, 120u);
        } else if (value != status) {
            w_u32(0x8010D130u, 5u);
            w_u32(0x800F2D44u, 120u);
        } else {
            slot = r_u32(0x8010D120u);
            value = r_u8(0x800F7093u + (port << 13) + (slot << 9));
            if (r_s32(0x800F2D90u + destination * 4u) < (sint32)value) {
                w_u32(0x8010D130u, 4u);
                w_u32(0x800F2D44u, 120u);
            } else {
                w_u16(0x800F3118u, 0u);
                w_u32(0x8010D130u, 1u);
                w_u32(0x800F2D44u, 18u);
            }
        }
    } else if (state == 1u) {
        front_configure_layer(0u, 0xFFFFFFFFu, 0xA9u);
        front_configure_layer(1u, 0xFFFFFFFFu, 0xA8u);
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        front_draw_colored_text(r_u32(labels + 0x68u), 44u, 136u, 0x808080u);
        front_draw_card_entry(r_u32(0x8010D11Cu), r_u32(0x8010D120u), 52u, 166u);
        port = r_u32(0x8010D11Cu);
        w_u32(0x800FB870u, 0xFDu);
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + (port == 0u ? 0x6Cu : 0x70u)), 44u, 194u, 0x808080u);
        timer = r_u32(0x800F2D44u);
        if (timer == 0u) {
            if (front_copy_selected_card_file() == 1u) {
                w_u32(0x8010D130u, 2u);
                w_u8(0x800F2FA8u, 0u);
                timer = 18u;
            } else {
                w_u16(0x800F3118u, 1u);
                w_u32(0x8010D130u, 3u);
                timer = 120u;
            }
            w_u32(0x800F2D44u, timer);
            timer = r_u32(0x800F2D44u);
        }
        w_u32(0x800F2D44u, timer - 1u);
    } else if (state == 3u) {
        front_configure_layer(0u, 5u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        timer = r_u32(0x800F2D44u);
        w_u32(0x800FB870u, 0xFDu);
        timer -= 1u;
        w_u32(0x800F2D44u, timer);
        if (timer == 0u || (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            value = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, value & ~0x40u);
        }
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + 0x54u), 44u, 136u, 0x808080u);
        front_draw_card_entry(r_u32(0x8010D11Cu), r_u32(0x8010D120u), 52u, 166u);
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + 0x58u), 44u, 194u, 0x808080u);
    } else if (state == 4u || state == 5u) {
        front_configure_layer(0u, 5u, 0xA9u);
        front_configure_layer(1u, 1u, 0xA8u);
        w_u32(0x800FB870u, 0xFDu);
        port = r_u32(0x8010D11Cu);
        timer = r_u32(0x800F2D44u);
        destination = port == 0u ? 1u : 0u;
        if (timer == 0u || (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            value = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, value & ~0x40u);
        }
        if (state == 4u) {
            if (r_u32(0x800F2D90u + destination * 4u) == 0u)
                label = destination == 0u ? 0x78u : 0x7Cu;
            else
                label = destination == 0u ? 0xB4u : 0xB8u;
        } else {
            label = destination == 0u ? 0x80u : 0x84u;
        }
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + label), 44u, 194u, 0x808080u);
        w_u32(0x800F2D44u, r_u32(0x800F2D44u) - 1u);
    } else if (state == 6u) {
        front_configure_layer(0u, 2u, 0xA7u);
        front_configure_layer(1u, 1u, 0xA8u);
        port = r_u32(0x8010D11Cu);
        w_u16(0x800F3118u, 0u);
        w_u32(0x800FB870u, 0xFDu);
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + (port == 0u ? 0x34u : 0x30u)), 44u, 136u, 0x808080u);
        if ((r_u32(0x80053684u) & 0x80u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            w_u32(0x8010D130u, 7u);
            w_u16(0x800F3118u, 0u);
            w_u32(0x800F2D44u, 18u);
        }
        if ((r_u32(0x80053684u) & 0x10u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 0u);
        }
    } else if (state == 7u) {
        front_configure_layer(0u, 0xFFFFFFFFu, 0xA9u);
        front_configure_layer(1u, 0xFFFFFFFFu, 0xA8u);
        port = r_u32(0x8010D11Cu);
        w_u32(0x800FB870u, 0xFDu);
        destination = port == 0u ? 1u : 0u;
        labels = r_u32(0x8010D15Cu);
        front_draw_colored_text(r_u32(labels + (destination == 0u ? 0x48u : 0x4Cu)), 44u, 136u, 0x808080u);
        timer = r_u32(0x800F2D44u);
        if ((sint32)timer > 0) {
            w_u32(0x800F2D44u, timer - 1u);
            return;
        }
        front_refresh_card_status(destination);
        if (r_u8(0x800F2D8Cu + destination) != 4u) {
            w_u16(0x800F3118u, 1u);
            w_u32(0x8010D130u, 0u);
            return;
        }
        port = r_u32(0x8010D11Cu);
        w_u32(0x8010D11Cu, port == 0u ? 1u : 0u);
        front_format_selected_card();
        w_u32(0x8010D11Cu, port);
        w_u32(0x8010D130u, 1u);
    } else if (state == 2u) {
        front_configure_layer(0u, 5u, 0xA9u);
        front_configure_layer(1u, 3u, 0xA8u);
        labels = r_u32(0x8010D15Cu);
        w_u32(0x800FB870u, 0xFDu);
        front_draw_colored_text(r_u32(labels + 0x74u), 44u, 136u, 0x808080u);
        timer = r_u32(0x800F2D44u);
        if (timer == 0u) {
            port = r_u32(0x8010D11Cu);
            destination = port == 0u ? 1u : 0u;
            w_u8(0x800F2D8Au + destination, 1u);
            w_u8(0x800F2D8Cu + destination, 0u);
            w_u16(0x800F3118u, 1u);
            timer = r_u32(0x800F2D44u);
        }
        if ((sint32)timer <= 0 && (r_u32(0x80053684u) & 0x40u) != 0u) {
            sound_play_sample_pitch(11u, 128u);
            sound_play_sample_pitch(11u, 128u);
            value = r_u32(0x80053684u);
            w_u32(0x8010D130u, 0u);
            w_u32(0x80053684u, value & ~0x40u);
        }
        timer = r_u32(0x800F2D44u);
        if ((sint32)timer >= 0)
            w_u32(0x800F2D44u, timer - 1u);
    }
}
