#include "game_functions.h"

void game_draw_pause_options(void)
{
    uint32 menu;
    FUNCTION_MARKER(0x80088954, "GAME.OVL");
    menu = r_u32(0x80124C00u);
    game_draw_pause_options_for_menu(menu);
}


void game_draw_pause_panel_for_menu(uint32 menu)
{
    uint32 geometry, region, anchor_x, anchor_y, left_end, packet, mode_packet, value, blue, width;
    uint32 owner, shift, index, text, length, target, origin, delta, magnitude, old_x, old_y, endpoint, remaining, current;
    sint32 depth, texture_x, texture_y, clut_x, clut_y, step;
    uint16 clut, page;
    FUNCTION_MARKER(0x80088F98, "GAME.OVL");
    geometry = r_u32(menu + 0x18u);
    region = r_u32(0x800311C0u);
    anchor_y = r_u32(geometry + 0x14u);
    anchor_x = r_u32(geometry + 0x10u);
    if (region != 0u)
        anchor_y += (uint32)((sint32)anchor_y >> 4);
    packet = r_u32(0x80040690u);
    w_u32(0x80040690u, packet + 16u);
    w_u8(packet + 3u, 3u);
    w_u8(packet + 7u, 96u);
    value = r_u8(0x800ECEDCu);
    w_u8(packet + 4u, (uint8)value);
    value = r_u8(0x800ECEDDu);
    w_u8(packet + 5u, (uint8)value);
    blue = r_u8(0x800ECEDEu);
    left_end = anchor_x + 151u;
    w_u16(packet + 8u, (uint16)anchor_x);
    w_u16(packet + 10u, (uint16)anchor_y);
    w_u8(packet + 6u, (uint8)blue);
    owner = r_u32(0x8004608Cu);
    w_u16(packet + 12u, (uint16)left_end);
    value = r_u16(0x8005053Au);
    w_u16(packet + 14u, (uint16)value);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(packet, 4u));
    packet = r_u32(0x80040690u);
    w_u32(0x80040690u, packet + 20u);
    SetSprt((SPRT *)psx_addr(packet, 20u));
    depth = r_s16(0x8005053Cu);
    value = r_u16(0x80050530u);
    shift = 2u - (uint32)depth;
    w_u8(packet + 12u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(0x80050532u);
    w_u16(packet + 10u, (uint16)anchor_y);
    w_u16(packet + 8u, (uint16)left_end);
    w_u8(packet + 13u, (uint8)value);
    value = r_u16(0x8005053Eu);
    w_u16(packet + 16u, (uint16)value);
    value = r_u16(0x8005053Au);
    w_u16(packet + 18u, (uint16)value);
    clut_x = r_s16(0x80050534u);
    clut_y = r_s16(0x80050536u);
    clut = GetClut(clut_x, clut_y);
    w_u8(packet + 4u, 128u);
    w_u8(packet + 5u, 128u);
    w_u8(packet + 6u, 128u);
    owner = r_u32(0x8004608Cu);
    w_u16(packet + 14u, clut);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(packet, 4u));
    depth = r_s16(0x8005053Cu);
    texture_x = r_s16(0x80050530u);
    texture_y = r_s16(0x80050532u);
    mode_packet = r_u32(0x80040690u);
    page = GetTPage(depth, 1, texture_x, texture_y);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 1, page, NULL);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(mode_packet, 4u));
    packet = mode_packet + 12u;
    w_u32(0x80040690u, packet + 16u);
    w_u8(packet + 3u, 3u);
    w_u8(packet + 7u, 96u);
    for (index = 0u; index < 3u; ++index) {
        value = r_u8(0x800ECEE0u + index);
        w_u8(packet + 4u + index, (uint8)value);
    }
    width = r_u16(0x8005053Eu);
    owner = r_u32(0x8004608Cu);
    w_u16(packet + 10u, (uint16)anchor_y);
    w_u16(packet + 12u, 103u);
    w_u16(packet + 14u, 11u);
    w_u16(packet + 8u, (uint16)(anchor_x + width + 151u));
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(packet, 4u));
    packet = r_u32(0x80040690u);
    w_u32(0x80040690u, packet + 16u);
    w_u8(packet + 3u, 3u);
    w_u8(packet + 7u, 96u);
    for (index = 0u; index < 3u; ++index) {
        value = r_u8(0x800ECEE0u + index);
        w_u8(packet + 4u + index, (uint8)value);
    }
    width = r_u16(0x8005053Eu);
    owner = r_u32(0x8004608Cu);
    w_u16(packet + 10u, (uint16)anchor_y);
    w_u16(packet + 12u, 103u);
    w_u16(packet + 14u, 11u);
    w_u16(packet + 8u, (uint16)(anchor_x + width + 269u));
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(packet, 4u));
    packet = r_u32(0x80040690u);
    w_u32(0x80040690u, packet + 20u);
    SetSprt((SPRT *)psx_addr(packet, 20u));
    depth = r_s16(0x8005054Cu);
    value = r_u16(0x80050540u);
    shift = 2u - (uint32)depth;
    w_u8(packet + 12u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(0x80050542u);
    w_u8(packet + 13u, (uint8)value);
    width = r_u16(0x8005053Eu);
    w_u16(packet + 10u, (uint16)anchor_y);
    w_u16(packet + 8u, (uint16)(anchor_x + width + 372u));
    value = r_u16(0x8005054Eu);
    w_u16(packet + 16u, (uint16)value);
    value = r_u16(0x8005054Au);
    w_u16(packet + 18u, (uint16)value);
    clut_x = r_s16(0x80050544u);
    clut_y = r_s16(0x80050546u);
    clut = GetClut(clut_x, clut_y);
    w_u8(packet + 4u, 128u);
    w_u8(packet + 5u, 128u);
    w_u8(packet + 6u, 128u);
    owner = r_u32(0x8004608Cu);
    w_u16(packet + 14u, clut);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(packet, 4u));
    depth = r_s16(0x8005054Cu);
    texture_x = r_s16(0x80050540u);
    texture_y = r_s16(0x80050542u);
    mode_packet = r_u32(0x80040690u);
    page = GetTPage(depth, 1, texture_x, texture_y);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 1, page, NULL);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(mode_packet, 4u));
    packet = mode_packet + 12u;
    w_u32(0x80040690u, packet + 16u);
    w_u8(packet + 3u, 3u);
    w_u8(packet + 7u, 96u);
    for (index = 0u; index < 3u; ++index) {
        value = r_u8(0x800ECEDCu + index);
        w_u8(packet + 4u + index, (uint8)value);
    }
    width = r_u16(0x8005053Eu);
    w_u16(packet + 10u, (uint16)anchor_y);
    w_u16(packet + 8u, (uint16)(anchor_x + width * 2u + 372u));
    width = r_u16(0x8005053Eu);
    owner = r_u32(0x8004608Cu);
    w_u16(packet + 12u, (uint16)(512u - (anchor_x + width * 2u + 372u)));
    value = r_u16(0x8005053Au);
    w_u16(packet + 14u, (uint16)value);
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(packet, 4u));
    packet = r_u32(0x80040690u);
    w_u32(0x80040690u, packet + 16u);
    w_u8(packet + 3u, 3u);
    w_u8(packet + 7u, 96u);
    value = r_u8(0x800ECEDCu);
    w_u8(packet + 4u, (uint8)value);
    value = r_u8(0x800ECEDDu);
    w_u8(packet + 5u, (uint8)value);
    blue = r_u8(0x800ECEDEu);
    w_u16(packet + 8u, (uint16)anchor_x);
    w_u8(packet + 6u, (uint8)blue);
    value = r_u16(0x8005053Au);
    owner = r_u32(0x8004608Cu);
    w_u16(packet + 12u, (uint16)(anchor_x + 512u));
    w_u16(packet + 14u, 66u);
    w_u16(packet + 10u, (uint16)(anchor_y + value - 1u));
    AddPrim(psx_addr(owner + 0x84u, 4u), psx_addr(packet, 4u));
    depth = r_s16(0x8005058Cu);
    packet = r_u32(0x80040690u);
    shift = 2u - (uint32)depth;
    w_u32(0x80040690u, packet + 20u);
    SetSprt((SPRT *)psx_addr(packet, 20u));
    value = r_u16(0x80050580u);
    w_u8(packet + 12u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(0x80050582u);
    w_u16(packet + 8u, (uint16)(anchor_x + 167u));
    w_u16(packet + 10u, (uint16)(anchor_y + 2u));
    w_u8(packet + 13u, (uint8)value);
    value = r_u16(0x8005058Eu);
    w_u16(packet + 16u, (uint16)value);
    value = r_u16(0x8005058Au);
    w_u16(packet + 18u, (uint16)value);
    clut_x = r_s16(0x80050584u);
    clut_y = r_s16(0x80050586u);
    clut = GetClut(clut_x, clut_y);
    w_u8(packet + 4u, 128u);
    w_u8(packet + 5u, 128u);
    w_u8(packet + 6u, 128u);
    owner = r_u32(0x8004608Cu);
    w_u16(packet + 14u, clut);
    AddPrim(psx_addr(owner + 0x80u, 4u), psx_addr(packet, 4u));
    text = r_u32(0x800F24C8u);
    w_u32(0x80033D2Cu, 84u);
    w_u8(0x8004617Cu, 128u);
    w_u8(0x80033D38u, 128u);
    w_u8(0x80053680u, 128u);
    w_u32(0x80053688u, anchor_x + 205u);
    w_u32(0x8005368Cu, anchor_y);
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    font_draw_current((const char *)psx_addr(text, (size_t)length + 1u));
    depth = r_s16(0x8005057Cu);
    packet = r_u32(0x80040690u);
    shift = 2u - (uint32)depth;
    w_u32(0x80040690u, packet + 20u);
    SetSprt((SPRT *)psx_addr(packet, 20u));
    value = r_u16(0x80050570u);
    w_u8(packet + 12u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(0x80050572u);
    w_u8(packet + 13u, (uint8)value);
    width = r_u16(0x8005053Eu);
    w_u16(packet + 10u, (uint16)(anchor_y + 1u));
    w_u16(packet + 8u, (uint16)(width + left_end + 208u));
    value = r_u16(0x8005057Eu);
    w_u16(packet + 16u, (uint16)value);
    value = r_u16(0x8005057Au);
    w_u16(packet + 18u, (uint16)value);
    clut_x = r_s16(0x80050574u);
    clut_y = r_s16(0x80050576u);
    clut = GetClut(clut_x, clut_y);
    menu = r_u32(0x80124C00u);
    w_u16(packet + 14u, clut);
    value = menu == 0x800ECFB4u ? 0u : 128u;
    w_u8(packet + 4u, (uint8)value);
    w_u8(packet + 5u, (uint8)value);
    w_u8(packet + 6u, (uint8)value);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x80u, 4u), psx_addr(packet, 4u));
    menu = r_u32(0x80124C00u);
    w_u32(0x80033D2Cu, 84u);
    w_u8(0x8004617Cu, 128u);
    w_u8(0x80033D38u, 128u);
    w_u8(0x80053680u, 128u);
    if (menu != 0x800ECFB4u) {
        text = r_u32(0x800F24CCu);
        w_u32(0x80053688u, anchor_x + 310u);
        w_u32(0x8005368Cu, anchor_y);
        length = 0u;
        while (r_u8(text + length) != 0u)
            ++length;
        font_draw_current((const char *)psx_addr(text, (size_t)length + 1u));
    }
    depth = r_s16(0x8005057Cu);
    texture_x = r_s16(0x80050570u);
    texture_y = r_s16(0x80050572u);
    mode_packet = r_u32(0x80040690u);
    page = GetTPage(depth, 1, texture_x, texture_y);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, 12u), 0, 1, page, NULL);
    owner = r_u32(0x8004608Cu);
    AddPrim(psx_addr(owner + 0x80u, 4u), psx_addr(mode_packet, 4u));
    menu = r_u32(0x80124C00u);
    value = r_u32(menu + 0x10u);
    w_u32(0x80040690u, mode_packet + 12u);
    if ((value & 2u) == 0u)
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
    w_u32(menu + 0x10u, value ^ 2u);
}


void game_draw_pause_panel(void)
{
    uint32 menu;
    FUNCTION_MARKER(0x80088F90, "GAME.OVL");
    menu = r_u32(0x80124C00u);
    game_draw_pause_panel_for_menu(menu);
}


void game_draw_pause_option(uint32 x, uint32 y, uint32 selected, uint32 entry)
{
    uint32 left_texture, middle_texture, right_texture, title, enabled, shade, value, text, length;
    uint32 arena, right_sprite, quad, shift, width, u, v, saved_u, saved_v, first_width, second_width, coordinate, owner;
    sint32 depth, clut_x, clut_y, texture_x, texture_y;
    uint16 clut, page;
    FUNCTION_MARKER(0x80088550, "GAME.OVL");
    shade = 128u;
    title = r_u32(entry + 0x10u);
    if (selected != 0u) {
        left_texture = 0x8004F9F0u;
        middle_texture = left_texture + 16u;
        right_texture = left_texture + 64u;
        w_u8(0x8004617Cu, 128u);
        w_u8(0x80033D38u, 128u);
        w_u8(0x80053680u, 128u);
    } else {
        left_texture = 0x8004F9C0u;
        middle_texture = left_texture + 16u;
        right_texture = left_texture + 96u;
        w_u8(0x8004617Cu, 112u);
        w_u8(0x80033D38u, 32u);
        w_u8(0x80053680u, 0u);
    }
    enabled = r_u8(entry + 0x18u);
    if (enabled == 0u) {
        shade = 18u;
        w_u8(0x8004617Cu, 18u);
        w_u8(0x80033D38u, 18u);
        w_u8(0x80053680u, 18u);
        w_u32(0x800ECF08u, 18u);
        w_u32(0x800ECF0Cu, 18u);
        w_u32(0x800ECF10u, 18u);
    }
    text = r_u32(0x800F24C4u + title * 4u);
    arena = r_u32(0x80040690u);
    w_u32(0x80053688u, x + 4u);
    w_u32(0x8005368Cu, y + 1u);
    w_u32(0x80040690u, arena + 80u);
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    font_draw_current((const char *)psx_addr(text, (size_t)length + 1u));
    SetSprt((SPRT *)psx_addr(arena, 20u));
    depth = r_s16(left_texture + 12u);
    value = r_u16(left_texture);
    shift = 2u - (uint32)depth;
    w_u8(arena + 12u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(left_texture + 2u);
    w_u16(arena + 10u, (uint16)y);
    w_u16(arena + 8u, (uint16)x);
    w_u8(arena + 13u, (uint8)value);
    value = r_u16(left_texture + 14u);
    w_u16(arena + 16u, (uint16)value);
    value = r_u16(left_texture + 10u);
    w_u16(arena + 18u, (uint16)value);
    clut_x = r_s16(left_texture + 4u);
    clut_y = r_s16(left_texture + 6u);
    clut = GetClut(clut_x, clut_y);
    right_sprite = arena + 20u;
    w_u8(arena + 4u, (uint8)shade);
    w_u8(arena + 6u, (uint8)shade);
    w_u8(arena + 5u, (uint8)shade);
    owner = r_u32(0x8004608Cu);
    w_u16(arena + 14u, clut);
    AddPrim(psx_addr(owner + 0x70u, 4u), psx_addr(arena, 4u));
    SetSprt((SPRT *)psx_addr(right_sprite, 20u));
    depth = r_s16(right_texture + 12u);
    value = r_u16(right_texture);
    shift = 2u - (uint32)depth;
    w_u8(arena + 32u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(right_texture + 2u);
    w_u8(arena + 33u, (uint8)value);
    width = r_u16(right_texture + 14u);
    w_u16(arena + 30u, (uint16)y);
    w_u16(arena + 28u, (uint16)(x - (width - 131u)));
    value = r_u16(right_texture + 14u);
    w_u16(arena + 36u, (uint16)value);
    value = r_u16(right_texture + 10u);
    w_u16(arena + 38u, (uint16)value);
    clut_x = r_s16(right_texture + 4u);
    clut_y = r_s16(right_texture + 6u);
    clut = GetClut(clut_x, clut_y);
    w_u8(arena + 24u, (uint8)shade);
    w_u8(arena + 26u, (uint8)shade);
    w_u8(arena + 25u, (uint8)shade);
    owner = r_u32(0x8004608Cu);
    w_u16(arena + 34u, clut);
    AddPrim(psx_addr(owner + 0x70u, 4u), psx_addr(right_sprite, 4u));
    quad = arena + 40u;
    SetPolyFT4((POLY_FT4 *)psx_addr(quad, 40u));
    value = r_u16(middle_texture);
    w_u8(arena + 52u, (uint8)((value & 63u) << (shift & 31u)));
    value = r_u16(middle_texture + 2u);
    w_u8(arena + 53u, (uint8)value);
    u = r_u8(arena + 52u);
    width = r_u8(middle_texture + 14u);
    v = r_u8(arena + 53u);
    w_u8(arena + 68u, (uint8)u);
    u = r_u8(arena + 52u);
    w_u8(arena + 61u, (uint8)v);
    w_u8(arena + 60u, (uint8)(u + width - 1u));
    v = r_u8(arena + 53u);
    value = r_u8(middle_texture + 10u);
    saved_u = r_u8(arena + 60u);
    w_u8(arena + 69u, (uint8)(v + value));
    first_width = r_u16(arena + 16u);
    saved_v = r_u8(arena + 69u);
    w_u16(arena + 48u, (uint16)(first_width + x));
    w_u16(arena + 50u, (uint16)y);
    w_u8(arena + 76u, (uint8)saved_u);
    w_u8(arena + 77u, (uint8)saved_v);
    first_width = r_u16(left_texture + 14u);
    second_width = r_u16(right_texture + 14u);
    coordinate = r_u16(arena + 48u);
    w_u16(arena + 58u, (uint16)y);
    w_u16(arena + 64u, (uint16)coordinate);
    coordinate = r_u16(arena + 48u);
    w_u16(arena + 56u, (uint16)(coordinate + 131u - first_width - second_width));
    value = r_u16(middle_texture + 10u);
    w_u16(arena + 66u, (uint16)(value + y));
    coordinate = r_u16(arena + 56u);
    value = r_u16(arena + 66u);
    w_u16(arena + 72u, (uint16)coordinate);
    w_u16(arena + 74u, (uint16)value);
    clut_x = r_s16(middle_texture + 4u);
    clut_y = r_s16(middle_texture + 6u);
    clut = GetClut(clut_x, clut_y);
    w_u16(arena + 54u, clut);
    depth = r_s16(middle_texture + 12u);
    texture_x = r_s16(middle_texture);
    texture_y = r_s16(middle_texture + 2u);
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
    w_u32(0x800ECF08u, 144u);
    w_u32(0x800ECF0Cu, 144u);
    w_u32(0x800ECF10u, 144u);
}


void game_draw_attached_glows(void)
{
    VECTOR position;
    SVECTOR point;
    sint32 screen, projection, flags, depth;
    uint32 slot, owner, record, camera, x, y, z, value, shift, magnitude_x, magnitude_y, magnitude_z;
    uint32 age, lifetime, intensity, packed, red, green, blue, color, scaled_depth, size, screen_x, screen_y, draw_depth;
    FUNCTION_MARKER(0x8008D0CC, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    for (slot = 0u; slot < 64u; ++slot) {
        owner = r_u32(0x80124030u + slot * 36u);
        if (owner == 0u)
            continue;
        record = 0x80124020u + slot * 36u;
        if (r_u32(owner + 0x118u) != record)
            continue;
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
        do {
            ApplyMatrixLV((MATRIX *)psx_addr(owner + 0x11Cu, 20u),
                (VECTOR *)psx_addr(record, 12u), &position);
            x = r_u32(owner + 8u);
            camera = r_u32(0x80040C94u);
            value = r_u32(camera + 8u);
            position.vx = (sint32)((uint32)position.vx + x - value);
            y = r_u32(owner + 12u);
            value = r_u32(camera + 12u);
            position.vy = (sint32)((uint32)position.vy + y - value);
            z = r_u32(owner + 16u);
            value = r_u32(camera + 16u);
            position.vz = (sint32)((uint32)position.vz + z - value);
            shift = 2u;
            for (;;) {
                x = (uint32)position.vx;
                y = (uint32)position.vy;
                z = (uint32)position.vz;
                magnitude_x = (sint32)x < 0 ? 0u - x : x;
                magnitude_y = (sint32)y < 0 ? 0u - y : y;
                magnitude_z = (sint32)z < 0 ? 0u - z : z;
                if ((sint32)magnitude_x < 0x3FAD && (sint32)magnitude_y < 0x3FAD && (sint32)magnitude_z < 0x3FAD)
                    break;
                ++shift;
                position.vx >>= 1;
                position.vy >>= 1;
                position.vz >>= 1;
            }
            point.vx = (sint16)position.vx;
            point.vy = (sint16)position.vy;
            point.vz = (sint16)position.vz;
            SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
            depth = RotTransPers(&point, &screen, &projection, &flags);
            lifetime = r_u32(record + 0x18u);
            age = r_u32(record + 0x14u);
            intensity = 255u - ((lifetime - age) << 4);
            if (depth != 0 && (sint32)intensity > 0) {
                packed = r_u32(record + 0x1Cu);
                red = ((packed & 255u) * intensity) >> 8;
                green = ((((packed >> 8) & 255u) * intensity) >> 8) << 8;
                blue = ((((packed >> 16) & 255u) * intensity) >> 8) << 16;
                color = red + green + blue;
                scaled_depth = (uint32)depth << ((shift - 2u) & 31u);
                if (scaled_depth >= 0x7D01u)
                    size = 40u;
                else {
                    if (scaled_depth == 0u)
                        xport_mips_break(7u);
                    size = 64000u / scaled_depth;
                }
                if ((sint32)size < 40)
                    size = 40u;
                if ((sint32)size >= 225)
                    size = 224u;
                screen_x = (uint32)(sint32)(sint16)((uint32)screen & 65535u);
                screen_y = (uint32)(sint32)(sint16)((uint32)screen >> 16);
                draw_depth = ((uint32)depth - 16u) << (shift & 31u);
                if ((sint32)size >= 41)
                    render_ellipse_fan(screen_x, screen_y, size >> 3, color, draw_depth);
                game_draw_glow_rays(screen_x, screen_y, size << 1, draw_depth, color);
            }
            age = r_u32(record + 0x14u) - 1u;
            w_u32(record + 0x14u, age);
            if ((sint32)age < 0) {
                lifetime = r_u32(record + 0x18u);
                w_u32(record + 0x14u, lifetime);
            }
            record = r_u32(record + 0x20u);
        } while (record != 0u);
    }
}


void game_draw_glow_rays(uint32 x, uint32 y, uint32 size, uint32 depth, uint32 color)
{
    uint32 horizontal, vertical, narrow, ray, packet, outer_x, outer_y;
    FUNCTION_MARKER(0x800BC378, "GAME.OVL");
    if ((sint32)size < 2)
        size = 2u;
    horizontal = (uint32)((sint32)size >> 3);
    vertical = (uint32)((sint32)size >> 4);
    narrow = (uint32)((sint32)size >> 5);
    for (ray = 0u; ray < 4u; ++ray) {
        if (ray == 0u) {
            outer_x = x + horizontal;
            outer_y = y - vertical;
        } else if (ray == 1u) {
            outer_x = x - horizontal;
            outer_y = y + vertical;
        } else if (ray == 2u) {
            outer_x = x - vertical;
            outer_y = y - narrow;
        } else {
            outer_x = x + vertical;
            outer_y = y + narrow;
        }
        packet = r_u32(0x80040690u);
        w_u32(0x80040690u, packet + 20u);
        w_u32(packet + 4u, 0u);
        if (ray == 3u)
            w_u32(packet + 12u, color);
        w_u16(packet + 8u, (uint16)outer_x);
        w_u16(packet + 10u, (uint16)outer_y);
        w_u16(packet + 16u, (uint16)x);
        w_u16(packet + 18u, (uint16)y);
        if (ray != 3u)
            w_u32(packet + 12u, color);
        setLineG2((LINE_G2 *)psx_addr(packet, 20u));
        SetSemiTrans((LINE_G2 *)psx_addr(packet, 20u), 1);
        render_insert_primitive((sint32)depth, packet);
    }
}


void game_draw_partial_rotating_mesh(uint32 phase)
{
    SVECTOR angles;
    MATRIX rotation, view;
    VECTOR translation;
    sint32 screen, projection, flags, depth;
    uint32 frame, x, y, z, row, column, vertex, first, second, third, fourth, next_row;
    uint32 packet_base, packet, owner, value;
    FUNCTION_MARKER(0x80090434, "GAME.OVL");
    angles.vx = 256;
    frame = r_u32(0x800311B8u);
    angles.vy = 128;
    angles.vz = (sint16)(frame << 1);
    if ((sint32)phase >= 16)
        phase = 30u - phase;
    if ((sint32)phase <= 0)
        return;
    RotMatrix(&angles, &rotation);
    MulMatrix0((MATRIX *)psx_addr(0x80035060u, 20u),
        (MATRIX *)psx_addr(0x800F4E78u, 20u), &view);
    MulMatrix0(&view, &rotation, &view);
    x = r_u32(0x800ED3ACu);
    y = r_u32(0x800ED3B0u);
    translation.vx = (sint32)(x * 300u) >> 8;
    translation.vy = (sint32)(y * 300u) >> 8;
    z = r_u32(0x800ED3B4u);
    translation.vz = (sint32)(z * 300u) >> 8;
    ApplyMatrixLV((MATRIX *)psx_addr(0x80035060u, 20u), &translation, &translation);
    w_u32(0x80034D6Cu, (uint32)translation.vx);
    w_u32(0x80034D70u, (uint32)translation.vy);
    w_u32(0x80034D74u, (uint32)translation.vz);
    SetRotMatrix(&view);
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    angles.vx = 0;
    angles.vy = 0;
    angles.vz = 0;
    depth = RotTransPers(&angles, &screen, &projection, &flags);
    if (depth < 32)
        return;
    for (row = 0u; row < 16u; ++row) {
        for (column = 0u; column <= phase; ++column) {
            vertex = 0x800F4658u + row * 128u + column * 8u;
            RotTransPers((SVECTOR *)psx_addr(vertex, 8u),
                (sint32 *)psx_addr(0x80033D50u + row * 64u + column * 4u, 4u), &projection, &flags);
        }
    }
    for (row = 0u; row < 16u; ++row) {
        next_row = (row + 1u) & 15u;
        for (column = 0u; column < phase; ++column) {
            first = row * 16u + column;
            second = next_row * 16u + column;
            third = first + 1u;
            fourth = second + 1u;
            frame = r_u32(0x800311B8u);
            packet_base = r_u32(0x80123F60u);
            packet = packet_base + row * 0x680u + (column + 15u - phase) * 104u + (frame & 1u) * 52u;
            value = r_u16(0x80033D50u + first * 4u);
            w_u16(packet + 8u, (uint16)value);
            value = r_u16(0x80033D52u + first * 4u);
            owner = r_u32(0x8004608Cu);
            w_u16(packet + 10u, (uint16)value);
            value = r_u16(0x80033D50u + second * 4u);
            w_u16(packet + 20u, (uint16)value);
            value = r_u16(0x80033D52u + second * 4u);
            w_u16(packet + 22u, (uint16)value);
            value = r_u16(0x80033D50u + third * 4u);
            w_u16(packet + 32u, (uint16)value);
            value = r_u16(0x80033D52u + third * 4u);
            w_u16(packet + 34u, (uint16)value);
            value = r_u16(0x80033D50u + fourth * 4u);
            w_u16(packet + 44u, (uint16)value);
            value = r_u16(0x80033D52u + fourth * 4u);
            w_u16(packet + 46u, (uint16)value);
            AddPrim(psx_addr(owner + 0x45A8u, 4u), psx_addr(packet, 4u));
        }
    }
}


void game_draw_message_cursor(uint32 position[2], const uint32 dimensions[2], uint32 color)
{
    uint32 packet, x, y, extent;
    FUNCTION_MARKER(0x80086028, "GAME.OVL");
    packet = r_u32(0x80040690u);
    w_u32(0x80040690u, packet + 24u);
    w_u32(packet + 4u, color);
    if (r_u32(0x800311C0u) != 0u) {
        y = position[1];
        position[1] = y + (uint32)((sint32)y >> 4);
    }
    x = position[0];
    w_u16(packet + 8u, (uint16)x);
    y = position[1];
    w_u16(packet + 10u, (uint16)y);
    x = position[0];
    extent = dimensions[0];
    w_u16(packet + 12u, (uint16)(x + extent));
    y = position[1];
    w_u16(packet + 14u, (uint16)y);
    x = position[0];
    w_u16(packet + 16u, (uint16)x);
    y = position[1];
    extent = dimensions[1];
    w_u16(packet + 18u, (uint16)(y + extent));
    x = position[0];
    extent = dimensions[0];
    w_u16(packet + 20u, (uint16)(x + extent));
    y = position[1];
    extent = dimensions[1];
    w_u16(packet + 22u, (uint16)(y + extent));
    SetPolyF4((POLY_F4 *)psx_addr(packet, 24u));
    render_insert_primitive(10, packet);
}


void game_draw_message_queue(void)
{
    uint32 offset, count, slot, base, timer, line_y, value, color, red, green, blue, region;
    uint32 text_length, index, saved, address, frame, accumulator, rate, fraction, head, remaining, next_timer;
    uint32 position[2], dimensions[2], cursor_color;
    sint32 length, revealed, age, width;
    FUNCTION_MARKER(0x800859F4, "GAME.OVL");
    index = (uint32)r_s16(0x80031126u);
    offset = index * 288u;
    if (r_s16(0x80042552u + offset) == 0)
        return;
    w_u32(0x80033D2Cu, 84u);
    count = 0u;
    for (slot = 0u; slot < 4u; ++slot) {
        length = r_s16(0x80042552u + slot * 288u);
        if (length != 0) {
            revealed = r_s16(0x80042554u + slot * 288u);
            if (revealed >= length)
                ++count;
        }
    }
    if (count == (uint32)r_s16(0x80031128u))
        --count;
    age = r_s32(0x800311B0u);
    value = r_u32(0x80031120u);
    w_u32(0x8011F130u, value);
    if (age < 200) {
        region = r_u32(0x800311C0u);
        w_u32(0x8011F130u, region * 10u + 142u);
    }
    timer = 0x80042550u + offset;
    base = 0x80042440u + offset;
    line_y = r_u32(0x8011F130u) - count * 10u;
    for (slot = 0u; slot < 4u; ++slot) {
        length = r_s16(0x80042552u + offset);
        if (length == 0)
            return;
        age = r_s16(timer);
        if (age < 151) {
            color = r_u32(0x8004254Cu + offset);
            blue = (uint32)r_s16(0x8004254Eu + offset);
            age = r_s16(timer);
            w_u8(0x8004617Cu, (uint8)color);
            color = r_u32(0x8004254Cu + offset);
            w_u8(0x80053680u, (uint8)blue);
            w_u8(0x80033D38u, (uint8)((sint32)color >> 8));
            if (age < 151)
                goto message_reveal;
        }
        age = r_s16(timer);
        color = r_u32(0x8004254Cu + offset);
        if (age >= 154) {
            red = (color & 255u) >> 1;
            green = (color >> 9) & 127u;
            blue = (color >> 17) & 127u;
        } else {
            red = (color & 255u) + 17u;
            green = ((color >> 8) & 255u) + 17u;
            blue = ((color >> 16) & 255u) + 17u;
            if (red >= 256u)
                red = 255u;
            if (green >= 256u)
                green = 255u;
            if (blue >= 256u)
                blue = 255u;
        }
        if (red == 0u && green == 0u && blue == 0u) {
            remaining = r_u16(0x80031128u);
            head = r_u16(0x80031126u);
            w_u16(0x80042552u + offset, 0u);
            ++head;
            w_u16(0x80031126u, (uint16)head);
            w_u16(0x80031128u, (uint16)(remaining - 1u));
            if ((sint16)head >= 4)
                w_u16(0x80031126u, 0u);
            goto message_advance;
        }
        w_u8(0x8004617Cu, (uint8)red);
        w_u8(0x80033D38u, (uint8)green);
        w_u8(0x80053680u, (uint8)blue);
        width = text_measure_width(base);
        value = (uint32)((sint32)(512u - (uint32)width) >> 1) - 18u;
        region = r_u32(0x800311C0u);
        w_u32(0x80053688u, value);
        w_u32(0x8005368Cu, line_y);
        if (region != 0u)
            w_u32(0x8005368Cu, line_y + (uint32)((sint32)line_y >> 4));
        text_length = 0u;
        while (r_u8(base + text_length) != 0u)
            ++text_length;
        font_draw_current((const char *)psx_addr(base, (size_t)text_length + 1u));
        color = red + (green << 8) + (blue << 16);
        w_u32(0x8004254Cu + offset, color);
        goto message_advance;
    message_reveal:
        revealed = r_s16(0x80042554u + offset);
        length = r_s16(0x80042552u + offset);
        if (revealed < length) {
            width = text_measure_width(base);
            value = (uint32)((sint32)(512u - (uint32)width) >> 1) - 18u;
            region = r_u32(0x800311C0u);
            w_u32(0x80053688u, value);
            w_u32(0x8005368Cu, line_y);
            if (region != 0u)
                w_u32(0x8005368Cu, line_y + (uint32)((sint32)line_y >> 4));
            revealed = r_s16(0x80042554u + offset);
            address = base + (uint32)revealed;
            saved = r_u8(address);
            w_u8(address, 0u);
            text_length = 0u;
            while (r_u8(base + text_length) != 0u)
                ++text_length;
            font_draw_current((const char *)psx_addr(base, (size_t)text_length + 1u));
            revealed = r_s16(0x80042554u + offset);
            w_u8(base + (uint32)revealed, (uint8)saved);
            frame = r_u32(0x800311B8u);
            if (frame % 3u != 0u) {
                value = r_u32(0x8003142Cu);
                position[0] = value + 4u;
                position[1] = line_y + 1u;
                dimensions[0] = 6u;
                dimensions[1] = 8u;
                cursor_color = r_u32(0x8004254Cu + offset);
                game_draw_message_cursor(position, dimensions, cursor_color);
            }
            accumulator = r_u32(0x8004255Cu + offset);
            rate = r_u32(0x80042558u + offset);
            accumulator += rate;
            w_u32(0x8004255Cu + offset, accumulator);
            index = r_u16(0x80042554u + offset);
            fraction = r_u8(0x8004255Cu + offset);
            w_u16(0x80042554u + offset, (uint16)(index + (uint32)((sint32)accumulator >> 8)));
            revealed = r_s16(0x80042554u + offset);
            length = r_s16(0x80042552u + offset);
            w_u32(0x8004255Cu + offset, fraction);
            if (revealed >= length)
                w_u16(timer, 0u);
            return;
        }
        width = text_measure_width(base);
        value = (uint32)((sint32)(512u - (uint32)width) >> 1) - 18u;
        region = r_u32(0x800311C0u);
        w_u32(0x80053688u, value);
        w_u32(0x8005368Cu, line_y);
        if (region != 0u)
            w_u32(0x8005368Cu, line_y + (uint32)((sint32)line_y >> 4));
        text_length = 0u;
        while (r_u8(base + text_length) != 0u)
            ++text_length;
        font_draw_current((const char *)psx_addr(base, (size_t)text_length + 1u));
    message_advance:
        value = r_u16(timer);
        next_timer = timer + 288u;
        offset += 288u;
        w_u16(base + 0x110u, (uint16)(value + 1u));
        base += 288u;
        timer = next_timer;
        if ((sint32)timer >= (sint32)0x800429D0u) {
            timer = 0x80042550u;
            base = 0x80042440u;
            offset = 0u;
        }
        line_y += 10u;
    }
}


void game_draw_expanding_bursts(void)
{
    SVECTOR point;
    sint32 screen, projection, flags, depth;
    uint32 slot, offset, first_record, second_record, camera, x, y, z, relative_x, relative_y, relative_z;
    uint32 random, velocity[3], index, shift, magnitude_x, magnitude_y, magnitude_z, scaled_depth, radius, factor, mode;
    uint32 red, green, blue, life, color, draw_depth, center_x, center_y, angle, next_angle, packet, wave, value;
    uint32 growth, decay, outer_width, outer_height;
    FUNCTION_MARKER(0x800BCD68, "GAME.OVL");
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    for (slot = 0u; slot < 10u; ++slot) {
        offset = slot * 48u;
        first_record = 0x801077D0u + offset;
        second_record = 0x800FD518u + offset;
        if (r_s32(first_record + 0x24u) > 0) {
            x = r_u32(first_record);
            camera = r_u32(0x80040C94u);
            y = r_u32(first_record + 4u);
            z = r_u32(first_record + 8u);
            value = r_u32(camera + 8u);
            relative_x = x - value;
            value = r_u32(camera + 12u);
            relative_y = y - value;
            value = r_u32(camera + 16u);
            relative_z = z - value;
            mode = r_u32(first_record + 0x2Cu);
            if (mode != 0u) {
                x = r_u32(first_record);
                y = r_u32(first_record + 4u);
                random = r_u32(0x800EF50Cu);
                z = r_u32(first_record + 8u);
                for (index = 0u; index < 3u; ++index) {
                    random = (uint32)((sint32)random >> 1) ^ (random << 3);
                    w_u32(0x800EF50Cu, random);
                    velocity[index] = (random & 8191u) - 4096u;
                }
                game_emit_wide_particles(8u, x, y, z, velocity[0], velocity[1], velocity[2]);
                x = r_u32(first_record);
                y = r_u32(first_record + 4u);
                random = r_u32(0x800EF50Cu);
                z = r_u32(first_record + 8u);
                for (index = 0u; index < 3u; ++index) {
                    random = (uint32)((sint32)random >> 1) ^ (random << 3);
                    w_u32(0x800EF50Cu, random);
                    velocity[index] = (random & 8191u) - 4096u;
                }
                game_emit_narrow_particles(8u, x, y, z, velocity[0], velocity[1], velocity[2]);
            } else {
                x = r_u32(first_record);
                y = r_u32(first_record + 4u);
                z = r_u32(first_record + 8u);
                game_emit_wide_particles(8u, x, y, z, 0u, 0u, 0u);
                x = r_u32(first_record);
                y = r_u32(first_record + 4u);
                z = r_u32(first_record + 8u);
                game_emit_narrow_particles(8u, x, y, z, 0u, 0u, 0u);
            }
            shift = 0u;
            for (;;) {
                magnitude_x = (sint32)relative_x < 0 ? 0u - relative_x : relative_x;
                magnitude_y = (sint32)relative_y < 0 ? 0u - relative_y : relative_y;
                magnitude_z = (sint32)relative_z < 0 ? 0u - relative_z : relative_z;
                if ((sint32)magnitude_x < 0x3FFD && (sint32)magnitude_y < 0x3FFD && (sint32)magnitude_z < 0x3FFD)
                    break;
                ++shift;
                relative_x = (uint32)((sint32)relative_x >> 1);
                relative_y = (uint32)((sint32)relative_y >> 1);
                relative_z = (uint32)((sint32)relative_z >> 1);
            }
            point.vx = (sint16)relative_x;
            point.vy = (sint16)relative_y;
            point.vz = (sint16)relative_z;
            depth = RotTransPers(&point, &screen, &projection, &flags);
            scaled_depth = (uint32)depth << (shift & 31u);
            if (scaled_depth >= 33u) {
                radius = 64000u / scaled_depth;
                if (radius == 0u)
                    radius = 1u;
                factor = r_u32(first_record + 0x10u);
                radius *= factor;
                mode = r_u32(first_record + 0x2Cu);
                radius >>= (8u - mode) & 31u;
                draw_depth = (scaled_depth - 32u) << 2;
                if (radius >= 361u)
                    radius = 360u;
                red = r_u32(first_record + 0x18u);
                life = r_u32(first_record + 0x24u);
                red = (uint32)((sint32)(red * life) >> 8);
                green = r_u32(first_record + 0x1Cu);
                green = (uint32)((sint32)(green * life) >> 8);
                blue = r_u32(first_record + 0x20u);
                blue = (uint32)((sint32)(blue * life) >> 8);
                color = red + (green << 8) + (blue << 16);
                center_x = (uint32)screen & 65535u;
                center_y = (uint32)screen >> 16;
                game_draw_burst_core((uint32)(sint32)(sint16)center_x,
                    (uint32)(sint32)(sint16)center_y, radius, draw_depth);
                for (angle = 0u; angle < 4096u; angle += 128u) {
                    next_angle = angle + 128u;
                    packet = r_u32(0x80040690u);
                    w_u32(0x80040690u, packet + 36u);
                    w_u32(packet + 4u, 0u);
                    w_u32(packet + 12u, 0u);
                    w_u32(packet + 20u, color);
                    w_u32(packet + 28u, color);
                    wave = (uint32)rsin((sint32)angle);
                    w_u16(packet + 8u, (uint16)(center_x + ((radius * wave) >> 13)));
                    wave = (uint32)rcos((sint32)angle);
                    w_u16(packet + 10u, (uint16)(center_y + ((radius * wave) >> 14)));
                    wave = (uint32)rsin((sint32)next_angle);
                    w_u16(packet + 16u, (uint16)(center_x + ((radius * wave) >> 13)));
                    wave = (uint32)rcos((sint32)next_angle);
                    w_u16(packet + 18u, (uint16)(center_y + ((radius * wave) >> 14)));
                    wave = (uint32)rsin((sint32)angle);
                    w_u16(packet + 24u, (uint16)(center_x + ((radius * wave) >> 12)));
                    wave = (uint32)rcos((sint32)angle);
                    w_u16(packet + 26u, (uint16)(center_y + ((radius * wave) >> 13)));
                    wave = (uint32)rsin((sint32)next_angle);
                    w_u16(packet + 32u, (uint16)(center_x + ((radius * wave) >> 12)));
                    wave = (uint32)rcos((sint32)next_angle);
                    w_u16(packet + 34u, (uint16)(center_y + ((radius * wave) >> 13)));
                    SetPolyG4((POLY_G4 *)psx_addr(packet, 36u));
                    SetSemiTrans((POLY_G4 *)psx_addr(packet, 36u), 1);
                    render_insert_primitive((sint32)draw_depth, packet);
                }
            }
            value = r_u32(first_record + 0x10u);
            growth = r_u32(first_record + 0x14u);
            life = r_u32(first_record + 0x24u);
            decay = r_u32(first_record + 0x28u);
            w_u32(first_record + 0x10u, value + growth);
            w_u32(first_record + 0x24u, life - decay);
        }
        if (r_s32(second_record + 0x24u) > 0) {
            x = r_u32(second_record);
            camera = r_u32(0x80040C94u);
            y = r_u32(second_record + 4u);
            z = r_u32(second_record + 8u);
            value = r_u32(camera + 8u);
            relative_x = (uint32)((sint32)(x - value) >> 4);
            value = r_u32(camera + 12u);
            relative_y = (uint32)((sint32)(y - value) >> 4);
            value = r_u32(camera + 16u);
            relative_z = (uint32)((sint32)(z - value) >> 4);
            point.vx = (sint16)relative_x;
            point.vy = (sint16)relative_y;
            point.vz = (sint16)relative_z;
            depth = RotTransPers(&point, &screen, &projection, &flags);
            scaled_depth = (uint32)depth << 4;
            if (scaled_depth >= 33u) {
                radius = 64000u / scaled_depth;
                if (radius == 0u)
                    radius = 1u;
                factor = r_u32(second_record + 0x10u);
                radius = (radius * factor) >> 8;
                if (radius >= 361u)
                    radius = 360u;
                draw_depth = (uint32)depth << 6;
                life = r_u32(second_record + 0x24u);
                factor = life << 1;
                outer_width = radius + (radius >> 1);
                outer_height = radius << 1;
                if (factor >= 256u)
                    factor = 255u;
                red = r_u32(second_record + 0x18u);
                red = (red * factor) >> 8;
                green = r_u32(second_record + 0x1Cu);
                green = (green * factor) >> 8;
                blue = r_u32(second_record + 0x20u);
                blue = (blue * factor) >> 8;
                color = red + (green << 8) + (blue << 16);
                center_x = (uint32)screen & 65535u;
                center_y = (uint32)screen >> 16;
                for (angle = 0u; angle < 4096u; angle += 128u) {
                    next_angle = angle + 128u;
                    packet = r_u32(0x80040690u);
                    w_u32(0x80040690u, packet + 28u);
                    w_u32(packet + 4u, 0u);
                    w_u32(packet + 12u, 0u);
                    w_u32(packet + 20u, color);
                    wave = (uint32)rsin((sint32)angle);
                    w_u16(packet + 8u, (uint16)(center_x + ((outer_width * wave) >> 12)));
                    wave = (uint32)rcos((sint32)angle);
                    w_u16(packet + 10u, (uint16)(center_y + ((outer_width * wave) >> 13)));
                    wave = (uint32)rsin((sint32)next_angle);
                    w_u16(packet + 16u, (uint16)(center_x + ((outer_width * wave) >> 12)));
                    wave = (uint32)rcos((sint32)next_angle);
                    w_u16(packet + 18u, (uint16)(center_y + ((outer_width * wave) >> 13)));
                    w_u16(packet + 24u, (uint16)center_x);
                    w_u16(packet + 26u, (uint16)center_y);
                    SetPolyG3((POLY_G3 *)psx_addr(packet, 28u));
                    SetSemiTrans((POLY_G3 *)psx_addr(packet, 28u), 1);
                    render_insert_primitive((sint32)draw_depth, packet);
                    packet = r_u32(0x80040690u);
                    w_u32(0x80040690u, packet + 28u);
                    w_u32(packet + 4u, 0u);
                    w_u32(packet + 12u, 0u);
                    w_u32(packet + 20u, color);
                    wave = (uint32)rsin((sint32)angle);
                    w_u16(packet + 8u, (uint16)(center_x + ((outer_height * wave) >> 12)));
                    wave = (uint32)rcos((sint32)angle);
                    w_u16(packet + 10u, (uint16)(center_y + ((radius * wave) >> 13)));
                    wave = (uint32)rsin((sint32)next_angle);
                    w_u16(packet + 16u, (uint16)(center_x + ((outer_height * wave) >> 12)));
                    wave = (uint32)rcos((sint32)next_angle);
                    w_u16(packet + 18u, (uint16)(center_y + ((radius * wave) >> 13)));
                    w_u16(packet + 24u, (uint16)center_x);
                    w_u16(packet + 26u, (uint16)center_y);
                    SetPolyG3((POLY_G3 *)psx_addr(packet, 28u));
                    SetSemiTrans((POLY_G3 *)psx_addr(packet, 28u), 1);
                    render_insert_primitive((sint32)draw_depth, packet);
                }
            }
            growth = r_u32(second_record + 0x14u);
            value = r_u32(second_record + 0x10u);
            w_u32(second_record + 0x10u, value + growth);
            decay = r_u32(second_record + 0x28u);
            life = r_u32(second_record + 0x24u);
            w_u32(second_record + 0x24u, life - decay);
        }
    }
}


void game_draw_burst_core(uint32 x, uint32 y, uint32 size, uint32 depth)
{
    uint32 half, quarter, random, next, offset_x, offset_y, long_x, long_y, thin_x, thin_y, middle;
    uint32 triangle, packet;
    FUNCTION_MARKER(0x800BBE9C, "GAME.OVL");
    if ((sint32)size < 2)
        size = 2u;
    half = (uint32)((sint32)size >> 1);
    if (depth != 0x114Fu) {
        random = r_u32(0x800EF50Cu);
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        offset_x = (next & 32767u) % half;
        random = (uint32)((sint32)next >> 1) ^ (next << 3);
        offset_y = (random & 32767u) % half;
        quarter = (uint32)((sint32)size >> 2);
        w_u32(0x800EF50Cu, random);
        x += offset_x - quarter;
        y += offset_y - quarter;
    }
    long_x = (uint32)((sint32)size >> 3);
    long_y = (uint32)((sint32)size >> 4);
    thin_y = (uint32)((sint32)size >> 7);
    thin_x = (uint32)((sint32)size >> 6);
    middle = (uint32)((sint32)size >> 5);
    for (triangle = 0u; triangle < 8u; ++triangle) {
        packet = r_u32(0x80040690u);
        w_u32(0x80040690u, packet + 28u);
        if (triangle == 0u)
            w_u32(packet + 20u, 0x00204080u);
        w_u32(packet + 4u, 0u);
        w_u32(packet + 12u, 0u);
        switch (triangle) {
        case 0u:
            w_u16(packet + 8u, (uint16)(x + long_x));
            w_u16(packet + 10u, (uint16)(y - long_y));
            w_u16(packet + 16u, (uint16)x);
            w_u16(packet + 24u, (uint16)x);
            w_u16(packet + 26u, (uint16)y);
            w_u16(packet + 18u, (uint16)(y - thin_y));
            break;
        case 1u:
            w_u32(packet + 20u, 0x00204080u);
            w_u16(packet + 10u, (uint16)(y - long_y));
            w_u16(packet + 8u, (uint16)(x + long_x));
            w_u16(packet + 18u, (uint16)y);
            w_u16(packet + 24u, (uint16)x);
            w_u16(packet + 26u, (uint16)y);
            w_u16(packet + 16u, (uint16)(x + thin_x));
            break;
        case 2u:
            w_u16(packet + 8u, (uint16)(x - long_x));
            w_u32(packet + 20u, 0x00204080u);
            w_u16(packet + 10u, (uint16)(y + long_y));
            w_u16(packet + 18u, (uint16)y);
            w_u16(packet + 24u, (uint16)x);
            w_u16(packet + 26u, (uint16)y);
            w_u16(packet + 16u, (uint16)(x - thin_x));
            break;
        case 3u:
            w_u16(packet + 8u, (uint16)(x - long_x));
            w_u16(packet + 10u, (uint16)(y + long_y));
            w_u16(packet + 16u, (uint16)x);
            w_u32(packet + 20u, 0x00204080u);
            w_u16(packet + 24u, (uint16)x);
            w_u16(packet + 26u, (uint16)y);
            w_u16(packet + 18u, (uint16)(y + thin_y));
            break;
        case 4u:
            w_u32(packet + 20u, 0x00204080u);
            w_u16(packet + 10u, (uint16)(y - middle));
            w_u16(packet + 16u, (uint16)x);
            w_u16(packet + 8u, (uint16)(x - long_y));
            w_u16(packet + 24u, (uint16)x);
            w_u16(packet + 26u, (uint16)y);
            w_u16(packet + 18u, (uint16)(y - thin_y));
            break;
        case 5u:
            w_u16(packet + 8u, (uint16)(x - long_y));
            w_u16(packet + 10u, (uint16)(y - middle));
            w_u32(packet + 20u, 0x00204080u);
            w_u16(packet + 18u, (uint16)y);
            w_u16(packet + 24u, (uint16)x);
            w_u16(packet + 26u, (uint16)y);
            w_u16(packet + 16u, (uint16)(x - thin_x));
            break;
        case 6u:
            w_u32(packet + 20u, 0x00204080u);
            w_u16(packet + 10u, (uint16)(y + middle));
            w_u16(packet + 16u, (uint16)x);
            w_u16(packet + 8u, (uint16)(x + long_y));
            w_u16(packet + 24u, (uint16)x);
            w_u16(packet + 26u, (uint16)y);
            w_u16(packet + 18u, (uint16)(y + thin_y));
            break;
        case 7u:
            w_u32(packet + 20u, 0x00204080u);
            w_u16(packet + 10u, (uint16)(y + middle));
            w_u16(packet + 8u, (uint16)(x + long_y));
            w_u16(packet + 18u, (uint16)y);
            w_u16(packet + 24u, (uint16)x);
            w_u16(packet + 26u, (uint16)y);
            w_u16(packet + 16u, (uint16)(x + thin_x));
            break;
        }
        SetPolyG3((POLY_G3 *)psx_addr(packet, 28u));
        SetSemiTrans((POLY_G3 *)psx_addr(packet, 28u), 1);
        render_insert_primitive((sint32)depth, packet);
    }
}


void game_normalize_direction_native(VECTOR *output, VECTOR *input)
{
    uint32 x, y, z, magnitude_x, magnitude_y, magnitude_z, sum;
    sint32 length;
    FUNCTION_MARKER(0x800AE92C, "GAME.OVL");
    for (;;) {
        x = (uint32)input->vx;
        y = (uint32)input->vy;
        z = (uint32)input->vz;
        magnitude_x = (sint32)x < 0 ? 0u - x : x;
        magnitude_y = (sint32)y < 0 ? 0u - y : y;
        magnitude_z = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)magnitude_x <= 32768 && (sint32)magnitude_y <= 32768 && (sint32)magnitude_z <= 32768)
            break;
        input->vy >>= 1;
        input->vx >>= 1;
        input->vz >>= 1;
    }
    x = (uint32)input->vx;
    y = (uint32)input->vy;
    z = (uint32)input->vz;
    sum = x * x + y * y + z * z;
    length = (sint32)SquareRoot0((sint32)sum);
    if (length <= 0)
        length = 1;
    output->vx = (sint32)((uint32)input->vx << 12) / length;
    output->vy = (sint32)((uint32)input->vy << 12) / length;
    output->vz = (sint32)((uint32)input->vz << 12) / length;
}


uint32 game_probe_target_path_native(uint32 source, uint32 distance, const VECTOR *origin, const VECTOR *direction)
{
    VECTOR relative, transformed;
    uint32 list, object, flags, model, table, metadata, bound, sample, along, product, x, y, z, scale;
    uint32 relative_x, relative_y, relative_z, magnitude_x, magnitude_y, magnitude_z;
    FUNCTION_MARKER(0x800AF4C8, "GAME.OVL");
    for (list = 0u; list < 2u; ++list) {
        object = object_lookup(list == 0u ? 3u : 1u);
        while (object != 0u) {
            flags = r_u32(object + 0x114u);
            if ((flags & 16u) != 0u && object != source) {
                model = r_u32(object + 0x74u);
                table = r_u32(0x80046178u);
                metadata = r_u32(table + model * 132u + 4u);
                bound = r_u32(metadata + 16u);
                (void)r_u32(metadata + 8u);
                along = 0u;
                for (sample = 0u; sample < 5u; ++sample) {
                    product = (uint32)direction->vx * along;
                    x = (uint32)origin->vx + (uint32)((sint32)product >> 12);
                    product = (uint32)direction->vy * along;
                    y = (uint32)origin->vy + (uint32)((sint32)product >> 12);
                    product = (uint32)direction->vz * along;
                    z = (uint32)origin->vz + (uint32)((sint32)product >> 12);
                    relative_x = r_u32(object + 8u);
                    scale = r_u32(object + 0x84u);
                    relative_x = (uint32)((sint32)(relative_x - x) >> (scale & 31u));
                    magnitude_x = (sint32)relative_x < 0 ? 0u - relative_x : relative_x;
                    relative_y = r_u32(object + 12u);
                    scale = r_u32(object + 0x84u);
                    relative_y = (uint32)((sint32)(relative_y - y) >> (scale & 31u));
                    relative_z = r_u32(object + 16u);
                    scale = r_u32(object + 0x84u);
                    relative_z = (uint32)((sint32)(relative_z - z) >> (scale & 31u));
                    if ((sint32)magnitude_x < (sint32)bound) {
                        magnitude_y = (sint32)relative_y < 0 ? 0u - relative_y : relative_y;
                        if ((sint32)magnitude_y < (sint32)bound) {
                            magnitude_z = (sint32)relative_z < 0 ? 0u - relative_z : relative_z;
                            if ((sint32)magnitude_z < (sint32)bound) {
                                relative.vx = (sint32)relative_x;
                                relative.vy = (sint32)relative_y;
                                relative.vz = (sint32)relative_z;
                                game_apply_transposed_matrix_native((const MATRIX *)psx_addr(source + 0x11Cu, 18u),
                                    &relative, &transformed);
                            }
                        }
                    }
                    along += 0x88B8u;
                }
            }
            object = r_u32(object + 0x1DCu);
        }
    }
    return 1u;
}


void game_choose_target_vertex_native(uint32 source, uint32 target, SVECTOR *output)
{
    VECTOR difference, local;
    uint32 model, source_x, table, target_x, descriptor, value, target_value, source_value, scale;
    uint32 axis, longitudinal, angular_y, angular_x, extent, column, angular, numerator, choice, frame;
    uint32 mesh, vertices, vertex, vertex_index, coordinate;
    sint32 lower, upper, denominator;
    FUNCTION_MARKER(0x800AF120, "GAME.OVL");
    model = r_u32(source + 0x74u);
    source_x = r_u32(source + 8u);
    table = r_u32(0x80046178u);
    target_x = r_u32(target + 8u);
    descriptor = r_u32(table + model * 132u + 8u);
    difference.vx = (sint32)(target_x - source_x);
    target_value = r_u32(target + 12u);
    source_value = r_u32(source + 12u);
    difference.vy = (sint32)(target_value - source_value);
    target_value = r_u32(target + 16u);
    source_value = r_u32(source + 16u);
    difference.vz = (sint32)(target_value - source_value);
    game_apply_transposed_matrix_native((const MATRIX *)psx_addr(source + 0x11Cu, 18u), &difference, &local);
    scale = r_u32(source + 0x84u);
    if ((sint32)scale > 0) {
        local.vx >>= scale & 31u;
        scale = r_u32(source + 0x84u);
        local.vy >>= scale & 31u;
        scale = r_u32(source + 0x84u);
        local.vz >>= scale & 31u;
    } else if ((sint32)scale < 0) {
        local.vx = (sint32)((uint32)local.vx << ((0u - scale) & 31u));
        scale = r_u32(source + 0x84u);
        if ((sint32)scale < 0)
            scale = 0u - scale;
        local.vy = (sint32)((uint32)local.vy << (scale & 31u));
        scale = r_u32(source + 0x84u);
        if ((sint32)scale < 0)
            scale = 0u - scale;
        local.vz = (sint32)((uint32)local.vz << (scale & 31u));
    }
    axis = r_u32(descriptor);
    if (axis == 0u) {
        longitudinal = (uint32)local.vx;
        angular_x = (uint32)local.vy;
        angular_y = (uint32)local.vz;
    } else if (axis == 1u) {
        longitudinal = (uint32)local.vy;
        angular_x = (uint32)local.vx;
        angular_y = (uint32)local.vz;
    } else {
        longitudinal = (uint32)local.vz;
        angular_x = (uint32)local.vy;
        angular_y = (uint32)local.vx;
    }
    extent = r_u32(descriptor + 4u);
    lower = (sint32)(0u - extent) >> 1;
    upper = (sint32)extent >> 1;
    if ((sint32)longitudinal <= lower)
        column = 0u;
    else if ((sint32)longitudinal >= upper)
        column = 15u;
    else {
        if ((sint32)longitudinal < 0) {
            value = 0u - (uint32)upper - longitudinal;
            if ((sint32)value < 0)
                value = 0u - value;
        } else if ((sint32)longitudinal > 0)
            value = (uint32)upper + longitudinal;
        else
            value = (uint32)upper;
        denominator = r_s32(descriptor + 4u);
        numerator = value << 4;
        if (denominator == 0)
            xport_mips_break(7u);
        if (numerator == 0x80000000u && denominator == -1)
            xport_mips_break(0x5Du);
        column = (uint32)((sint32)numerator / denominator);
        if ((sint32)column >= 16)
            column = 15u;
        if ((sint32)column < 0)
            column = 0u;
    }
    angular = (uint32)(ratan2((sint32)angular_y, (sint32)angular_x) >> 8);
    if ((sint32)angular < 0)
        angular += 15u;
    choice = (uint32)random_between(0, 2) & 255u;
    frame = r_u32(0x800F2398u);
    if (choice == 0u)
        angular += (frame & 1u) != 0u ? 0u - 3u : 3u;
    else if (choice == 1u)
        column += (frame & 1u) != 0u ? 0u - 3u : 3u;
    else {
        value = (frame & 1u) != 0u ? 0u - 4u : 4u;
        column += value;
        angular += value;
    }
    if ((sint32)angular < 0)
        angular = 0u;
    if ((sint32)angular >= 16)
        angular = 15u;
    if ((sint32)column < 0)
        column = 0u;
    if ((sint32)column >= 16)
        column = 15u;
    model = r_u32(source + 0x74u);
    table = r_u32(0x80046178u);
    mesh = r_u32(table + model * 132u);
    vertex_index = (uint32)r_s16(descriptor + column * 34u + angular * 2u + 8u);
    vertices = r_u32(mesh);
    vertex = vertices + (vertex_index << 3);
    coordinate = r_u16(vertex);
    output->vx = (sint16)coordinate;
    coordinate = r_u16(vertex + 2u);
    output->vy = (sint16)coordinate;
    frame = r_u32(0x800F2398u);
    coordinate = r_u16(vertex + 4u);
    w_u32(0x800F2398u, frame + 1u);
    output->vz = (sint16)coordinate;
}


uint32 game_is_weapon_target_in_range(uint32 source, uint32 target, uint32 range)
{
    SVECTOR vertex;
    VECTOR origin, difference, direction;
    uint32 scale, value, position, x, y, z, distance, model, table, radius, limit;
    FUNCTION_MARKER(0x800EC57C, "GAME.OVL");
    game_choose_target_vertex_native(source, target, &vertex);
    ApplyMatrix((MATRIX *)psx_addr(source + 0x11Cu, 20u), &vertex, &origin);
    scale = r_u32(source + 0x84u);
    position = r_u32(source + 8u);
    origin.vx = (sint32)(((uint32)origin.vx << (scale & 31u)) + position);
    scale = r_u32(source + 0x84u);
    position = r_u32(source + 12u);
    origin.vy = (sint32)(((uint32)origin.vy << (scale & 31u)) + position);
    scale = r_u32(source + 0x84u);
    position = r_u32(source + 16u);
    origin.vz = (sint32)(((uint32)origin.vz << (scale & 31u)) + position);
    x = r_u32(target + 8u) - (uint32)origin.vx;
    difference.vx = (sint32)x;
    y = r_u32(target + 12u) - (uint32)origin.vy;
    difference.vy = (sint32)y;
    z = r_u32(target + 16u) - (uint32)origin.vz;
    difference.vz = (sint32)z;
    distance = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
    scale = r_u32(source + 0x84u);
    w_u32(0x80046174u, distance);
    model = r_u32(source + 0x74u);
    table = r_u32(0x80046178u);
    radius = r_u32(table + model * 132u + 0x6Cu);
    if ((sint32)scale > 0)
        value = radius << (scale & 31u);
    else {
        if ((sint32)scale < 0)
            scale = 0u - scale;
        value = (uint32)((sint32)radius >> (scale & 31u));
    }
    w_u32(0x8011F130u, range + value);
    distance = r_u32(0x80046174u);
    limit = r_u32(0x8011F130u);
    if ((sint32)limit < (sint32)distance)
        return 0u;
    game_normalize_direction_native(&direction, &difference);
    distance = r_u32(0x80046174u);
    return game_probe_target_path_native(source, distance, &origin, &direction) != 0u;
}


void game_update_weapon_state(void)
{
    uint32 region, frame, periodic, refreshed = 0u, list, object, player, block, record, index;
    uint32 timer, age, mode, count, limit, value, control, candidate, candidate_object, flags;
    uint32 scan_index, weapon_index, weapon, assigned, identity, best, minimum, chosen, load;
    sint32 rotation, rotation_limit;
    FUNCTION_MARKER(0x800EA5EC, "GAME.OVL");
    region = r_u32(0x800311C0u);
    periodic = 0u;
    if (region != 0u) {
        frame = r_u32(0x800311B8u);
        if ((frame & 3u) == 0u)
            periodic = 1u;
    }
    w_u32(0x800F2B68u, periodic);
    for (list = 0u; list < 2u; ++list) {
        object = object_lookup(list == 0u ? 1u : 3u);
        player = r_u32(0x80040FDCu);
        if (object == player)
            object = r_u32(object + 0x1D4u);
        while (object != 0u) {
            if (r_u32(object) == 0u)
                goto next_weapon_object;
            block = r_u32(object + 0x110u);
            if (block == 0u)
                goto next_weapon_object;
            for (index = 0u; index < 6u; ++index) {
                record = block + index * 0x30u;
                if (r_u8(record) == 0u)
                    continue;
                timer = r_u32(record + 0x24u);
                periodic = r_u32(0x800F2B68u);
                mode = r_u32(record + 0x2Cu);
                timer -= periodic;
                frame = r_u32(0x800311B8u);
                w_u32(record + 0x24u, timer);
                age = frame - timer;
                if (mode == 1u) {
                    if (age == 1u) {
                        count = r_u32(record + 0x28u);
                        w_u32(record + 0x2Cu, 0u);
                        w_u32(record + 0x28u, count + 1u);
                    }
                } else {
                    limit = r_u32(record + 0x1Cu);
                    if ((sint32)age >= (sint32)limit) {
                        w_u32(record + 0x28u, 0u);
                        w_u32(record + 0x2Cu, 1u);
                    } else {
                        count = r_u32(record + 0x28u);
                        limit = r_u32(record + 0x18u);
                        if ((sint32)count < (sint32)limit) {
                            limit = r_u32(record + 0x20u);
                            if ((sint32)age >= (sint32)limit)
                                w_u32(record + 0x2Cu, 1u);
                        }
                    }
                }
            }
            for (index = 0u; index < 9u; ++index) {
                frame = r_u32(0x800311B8u);
                record = block + 0x120u + index * 0x2Cu;
                if (r_u8(record) == 0u)
                    continue;
                mode = r_u32(record + 0x28u);
                if (mode == 1u) {
                    timer = r_u32(record + 0x24u);
                    if (frame - timer == 1u) {
                        count = r_u32(record + 0x1Cu);
                        w_u32(record + 0x28u, 0u);
                        w_u32(record + 0x20u, 0u);
                        w_u32(record + 0x1Cu, count + 1u);
                    }
                    continue;
                }
                count = r_u32(record + 0x1Cu);
                limit = r_u32(record + 0x10u);
                if ((sint32)count >= (sint32)limit)
                    continue;
                rotation = r_s16(object + 0x25Cu);
                rotation_limit = r_s16(record + 0x18u);
                if (rotation < 0)
                    rotation = -rotation;
                value = (uint32)(rotation_limit < rotation);
                if (value == 0u) {
                    rotation = r_s16(object + 0x25Eu);
                    rotation_limit = r_s16(record + 0x1Au);
                    if (rotation < 0)
                        rotation = -rotation;
                    value = (uint32)(rotation_limit < rotation);
                }
                if (value != 0u && index != 8u)
                    w_u32(record + 0x20u, 0u);
                else {
                    value = r_u32(object + 0x230u);
                    limit = r_u32(record + 0x14u);
                    if ((sint32)limit < (sint32)value)
                        w_u32(record + 0x20u, 0u);
                    else {
                        count = r_u32(record + 0x20u);
                        w_u32(record + 0x20u, count + 1u);
                    }
                }
                count = r_u32(record + 0x20u);
                limit = r_u32(record + 12u);
                if ((sint32)count >= (sint32)limit) {
                    timer = r_u32(record + 0x24u);
                    limit = r_u32(record + 8u);
                    if (frame - timer >= limit)
                        w_u32(record + 0x28u, 1u);
                }
            }
            value = r_u8(block + 0x46Fu);
            control = block + 0x2ACu;
            if (value == 0u)
                goto weapon_control_done;
            frame = r_u32(0x800311B8u);
            timer = r_u32(block + 0x468u);
            if (frame - timer >= 60u) {
                w_u32(block + 0x468u, frame);
                refreshed = 1u;
                for (index = 0u; index < 10u; ++index) {
                    candidate = control + 0x140u + index * 12u;
                    w_u32(candidate, 0u);
                    w_u16(candidate + 4u, 0u);
                    w_u8(candidate + 8u, 0u);
                    w_u8(candidate + 9u, 1u);
                }
                w_u8(control + 0x1C2u, 0u);
                w_u8(control + 0x1C0u, 0u);
                identity = (uint32)r_s16(0x80059612u);
                value = r_u32(object);
                flags = 0u;
                if (value == identity)
                    flags = r_u32(object + 0x114u);
                if (value != identity || (flags & 0x10000000u) != 0u) {
                    if (r_u32(0x800EDD20u) == 0u) {
                        player = r_u32(0x80040FDCu);
                        limit = r_u32(control + 0x1B8u);
                        if (game_is_weapon_target_in_range(object, player, limit) != 0u) {
                            count = r_u8(control + 0x1C0u);
                            player = r_u32(0x80040FDCu);
                            w_u32(control + 0x140u + count * 12u, player);
                            count = r_u8(control + 0x1C0u);
                            identity = r_u16(player + 0x24Au);
                            w_u16(control + 0x144u + count * 12u, (uint16)identity);
                            count = r_u8(control + 0x1C0u);
                            w_u8(control + 0x149u + count * 12u, 0u);
                            count = r_u8(control + 0x1C0u);
                            w_u8(control + 0x1C0u, (uint8)(count + 1u));
                        }
                    }
                }
                value = r_u32(object);
                candidate_object = object_lookup(value == 3u ? 1u : 3u);
                while (candidate_object != 0u) {
                    flags = r_u32(candidate_object + 0x114u);
                    if ((flags & 0x14u) == 0u && r_u32(candidate_object) != 0u) {
                        player = r_u32(0x80040FDCu);
                        if (candidate_object != player) {
                            limit = r_u32(control + 0x1B8u);
                            if (game_is_weapon_target_in_range(object, candidate_object, limit) != 0u) {
                                count = r_u8(control + 0x1C0u);
                                w_u32(control + 0x140u + count * 12u, candidate_object);
                                count = r_u8(control + 0x1C0u);
                                identity = r_u16(candidate_object + 0x24Au);
                                w_u16(control + 0x144u + count * 12u, (uint16)identity);
                                count = r_u8(control + 0x1C0u);
                                w_u8(control + 0x149u + count * 12u, 0u);
                                count = r_u8(control + 0x1C0u);
                                count = (count + 1u) & 255u;
                                w_u8(control + 0x1C0u, (uint8)count);
                                if (count >= 9u)
                                    break;
                            }
                        }
                    }
                    candidate_object = r_u32(candidate_object + 0x1DCu);
                }
                count = r_u8(control + 0x1C4u);
                for (index = 0u, record = control; index < count; ++index, record += 32u) {
                    if (r_u8(record) != 0u)
                        w_u32(record + 12u, 0u);
                    w_u8(record + 0x1Du, 0u);
                    count = r_u8(control + 0x1C4u);
                }
            }
            count = r_u8(control + 0x1C0u);
            if (count >= 10u)
                diagnostic_report(count, (intptr)0x80076BC0u, (intptr)0x80076BD0u);
            count = r_u8(control + 0x1C0u);
            scan_index = 0u;
            candidate = control + 0x140u;
            if (count != 0u) {
                do {
                    candidate_object = r_u32(candidate);
                    if (r_u32(candidate_object) == 0u)
                        w_u8(candidate + 9u, 1u);
                    count = r_u8(control + 0x1C0u);
                    ++scan_index;
                    candidate += 12u;
                } while (scan_index < count);
                count = r_u8(control + 0x1C0u);
            }
            if (count == 0u)
                goto weapon_control_done;
            count = r_u8(control + 0x1C4u);
            for (weapon_index = 0u, weapon = control; weapon_index < count; ++weapon_index, weapon += 32u) {
                if (r_u8(weapon) == 0u)
                    goto next_control_weapon;
                assigned = r_u32(weapon + 12u);
                if (assigned != 0u) {
                    if (refreshed == 0u && r_u32(assigned) != 0u) {
                        identity = r_u16(assigned + 0x24Au);
                        value = r_u16(weapon + 0x10u);
                        if (identity == value) {
                            frame = r_u32(0x800311B8u);
                            timer = r_u32(weapon + 0x18u);
                            limit = r_u16(weapon + 2u);
                            if ((sint32)(frame - timer) < (sint32)limit)
                                goto maintain_control_weapon;
                        }
                    }
                    assigned = r_u32(weapon + 12u);
                    if (assigned != 0u) {
                        count = r_u8(control + 0x1C0u);
                        scan_index = 0u;
                        candidate = control + 0x140u;
                        while (scan_index < count) {
                            candidate_object = r_u32(candidate);
                            assigned = r_u32(weapon + 12u);
                            if (candidate_object == assigned) {
                                load = r_u8(candidate + 8u);
                                if (load != 0u)
                                    w_u8(candidate + 8u, (uint8)(load - 1u));
                            }
                            count = r_u8(control + 0x1C0u);
                            ++scan_index;
                            candidate += 12u;
                        }
                        assigned = r_u32(weapon + 12u);
                        player = r_u32(0x80040FDCu);
                        if (assigned == player) {
                            load = r_u8(control + 0x1C2u);
                            w_u8(control + 0x1C2u, (uint8)(load - 1u));
                        }
                        w_u32(weapon + 12u, 0u);
                    }
                }
                candidate_object = r_u32(control + 0x140u);
                player = r_u32(0x80040FDCu);
                if (candidate_object == player) {
                    count = r_u8(control + 0x1C0u);
                    value = (uint32)(count == 1u);
                    if (value == 0u) {
                        load = r_u8(control + 0x1C2u);
                        limit = r_u8(control + 0x1C1u);
                        value = (uint32)(load < limit);
                    }
                    if (value != 0u) {
                        candidate = control + scan_index * 12u;
                        w_u32(weapon + 12u, candidate_object);
                        load = r_u8(candidate + 0x148u);
                        w_u8(candidate + 0x148u, (uint8)(load + 1u));
                    }
                }
                assigned = r_u32(weapon + 12u);
                if (assigned == 0u) {
                    minimum = 4u;
                    candidate_object = r_u32(control + 0x140u);
                    player = r_u32(0x80040FDCu);
                    count = r_u8(control + 0x1C0u);
                    scan_index = (uint32)(candidate_object == player);
                    best = 0xFFFFFFFFu;
                    candidate = control + scan_index * 12u;
                    while (scan_index < count) {
                        load = r_u8(candidate + 0x148u);
                        if (load <= minimum) {
                            minimum = load;
                            best = scan_index;
                        }
                        ++scan_index;
                        candidate += 12u;
                    }
                    chosen = (uint32)(sint32)(sint8)best;
                    candidate = control + chosen * 12u;
                    load = r_u8(candidate + 0x148u);
                    if (load < 3u && chosen != 0xFFFFFFFFu) {
                        candidate_object = r_u32(candidate + 0x140u);
                        w_u32(weapon + 12u, candidate_object);
                        load = r_u8(candidate + 0x148u);
                        w_u8(candidate + 0x148u, (uint8)(load + 1u));
                    }
                    assigned = r_u32(weapon + 12u);
                    if (assigned == 0u)
                        goto clear_control_weapon;
                }
                identity = r_u16(assigned + 0x24Au);
                w_u16(weapon + 0x10u, (uint16)identity);
                value = (uint32)random_between(5, 15);
                frame = r_u32(0x800311B8u);
                w_u32(weapon + 0x18u, frame + value);
                assigned = r_u32(weapon + 12u);
                player = r_u32(0x80040FDCu);
                if (assigned == player) {
                    load = r_u8(control + 0x1C2u);
                    w_u8(control + 0x1C2u, (uint8)(load + 1u));
                }
maintain_control_weapon:
                assigned = r_u32(weapon + 12u);
                if (assigned == 0u)
                    goto clear_control_weapon;
                frame = r_u32(0x800311B8u);
                timer = r_u32(weapon + 0x14u);
                mode = r_u8(weapon + 0x1Du);
                age = frame - timer;
                if (mode == 1u) {
                    if (age == 1u) {
                        count = r_u8(weapon + 0x1Cu);
                        w_u8(weapon + 0x1Cu, (uint8)(count + 1u));
                    }
                    goto clear_control_weapon;
                }
                limit = r_u16(weapon + 4u);
                if ((sint32)age >= (sint32)limit) {
                    w_u8(weapon + 0x1Cu, 0u);
                    w_u8(weapon + 0x1Du, 1u);
                } else {
                    count = r_u8(weapon + 0x1Cu);
                    limit = r_u8(weapon + 8u);
                    if (count < limit) {
                        limit = r_u16(weapon + 6u);
                        if ((sint32)age >= (sint32)limit)
                            w_u8(weapon + 0x1Du, 1u);
                    }
                }
                goto next_control_weapon;
clear_control_weapon:
                w_u8(weapon + 0x1Du, 0u);
next_control_weapon:
                count = r_u8(control + 0x1C4u);
            }
            if (refreshed != 0u)
                refreshed = 0u;
weapon_control_done:
            if (r_u32(block + 0x474u) != 0u) {
                mode = r_u32(block + 0x480u);
                if (mode == 1u) {
                    timer = r_u32(block + 0x47Cu);
                    frame = r_u32(0x800311B8u);
                    if (frame < timer)
                        w_u32(block + 0x480u, 0u);
                } else {
                    timer = r_u32(block + 0x47Cu);
                    frame = r_u32(0x800311B8u);
                    value = timer - frame;
                    w_u32(0x8011F130u, value);
                    if ((sint32)value <= 0)
                        w_u32(block + 0x480u, 1u);
                }
            }
next_weapon_object:
            object = r_u32(object + 0x1DCu);
            player = r_u32(0x80040FDCu);
            if (object == player)
                object = r_u32(object + 0x1D4u);
        }
    }
    value = r_u32(0x800F2398u);
    w_u32(0x800F2398u, value + 1u);
}


void game_update_and_draw_scene(void)
{
    uint32 flags, player, value, phase, index, view;
    sint32 count, step;
    FUNCTION_MARKER(0x80082D40, "GAME.OVL");
    if (r_s16(0x8005961Cu) != 0 && r_u32(0x80031134u) == 0u)
        game_process_pause_request();
    if (r_u32(0x800ECEA8u) != 0u)
        return;
    step = r_s16(0x800ECEBCu);
    value = r_u32(0x80059630u);
    w_u32(0x80059630u, value + (uint32)step);
    render_select_buffer(0x1160u);
    game_reset_object_collision_links();
    if (r_u32(0x80031134u) == 0u)
        game_collide_scene_object_groups();
    game_update_weapon_state();
    game_update_scene_objects();
    game_update_scene_effects();
    if (r_u32(0x80031134u) == 0u) {
        flags = r_u32(0x80053684u);
        if ((flags & 0x100u) != 0u) {
            player = r_u32(0x80040FDCu);
            if (r_u32(player) == 0x66u && r_s16(0x8005961Cu) != 0)
                game_cycle_view_mode();
        }
    }
    game_draw_camera_particle_layers();
    game_update_object_trails();
    game_draw_scene_descriptor_chain();
    game_draw_expanding_bursts();
    game_draw_screen_flash();
    game_draw_active_effect_slots();
    game_draw_scene_mesh_effects();
    count = r_s16(0x800595F0u);
    index = 0u;
    view = 0x80123F78u;
    while ((sint32)index < count) {
        game_draw_view_by_mode(view, index);
        count = r_s16(0x800595F0u);
        ++index;
        view += 0x1Cu;
    }
    game_update_timed_point_effects(1u);
    game_draw_environment_mesh();
    game_draw_active_groups();
    game_draw_attached_glows();
    game_update_scene_particle_effects();
    game_update_selected_object_link();
    game_draw_message_queue();
    game_draw_queued_scene_messages();
    phase = r_u32(0x800ED394u);
    if (phase != 0u)
        game_draw_partial_rotating_mesh(phase);
    game_update_nearby_effect_audio();
    game_restore_scene_draw_color();
    game_finish_scene_objects();
    game_finish_effect_updates();
    render_submit_frame();
    system_read_counter_one();
}


void game_get_target_vertex_vector_native(uint32 source, uint32 target, VECTOR *output)
{
    SVECTOR vertex;
    FUNCTION_MARKER(0x80084CF4, "GAME.OVL");
    if (r_u32(source + 0x110u) == 0u) {
        output->vx = 0;
        output->vy = 0;
        output->vz = 0;
        return;
    }
    game_choose_target_vertex_native(source, target, &vertex);
    output->vx = vertex.vx;
    output->vy = vertex.vy;
    output->vz = vertex.vz;
}


void game_get_target_vertex_vector(uint32 source, uint32 target, uint32 output)
{
    VECTOR result;
    game_get_target_vertex_vector_native(source, target, &result);
    w_u32(output, (uint32)result.vx);
    w_u32(output + 4u, (uint32)result.vy);
    w_u32(output + 8u, (uint32)result.vz);
}


void game_draw_muzzle_glow_native(const VECTOR *position)
{
    uint32 camera;
    FUNCTION_MARKER(0x800AEB3C, "GAME.OVL");
    camera = r_u32(0x80040C94u);
    game_draw_muzzle_glow_for_camera_native(position, camera);
}


void game_draw_muzzle_glow_for_camera_native(const VECTOR *position, uint32 camera)
{
    SVECTOR vertex;
    sint32 screen, projection, flags;
    uint32 x, y, z, ax, ay, az, shifts = 0u, depth, radius;
    FUNCTION_MARKER(0x800AEB44, "GAME.OVL");
    x = (uint32)position->vx - r_u32(camera + 8u);
    y = (uint32)position->vy - r_u32(camera + 12u);
    z = (uint32)position->vz - r_u32(camera + 16u);
    for (;;) {
        ax = (sint32)x < 0 ? 0u - x : x;
        ay = (sint32)y < 0 ? 0u - y : y;
        az = (sint32)z < 0 ? 0u - z : z;
        if ((sint32)ax < 16385 && (sint32)ay < 16385 && (sint32)az < 16385)
            break;
        ++shifts;
        x = (uint32)((sint32)x >> 1);
        y = (uint32)((sint32)y >> 1);
        z = (uint32)((sint32)z >> 1);
    }
    w_u32(0x80034D6Cu, 0u);
    w_u32(0x80034D70u, 0u);
    w_u32(0x80034D74u, 0u);
    vertex.vx = (sint16)x;
    vertex.vy = (sint16)y;
    vertex.vz = (sint16)z;
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, 32u));
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
    depth = (uint32)RotTransPers(&vertex, &screen, &projection, &flags);
    if (depth == 0u)
        return;
    depth <<= ((shifts & 255u) + 2u) & 31u;
    depth >>= 4;
    if (depth == 0u)
        depth = 1u;
    radius = 64000u / depth;
    if ((sint32)radius < 16)
        radius = 16u;
    if ((sint32)radius >= 129)
        radius = 128u;
    x = (uint32)(sint32)(sint16)screen;
    y = (uint32)(sint32)(sint16)((uint32)screen >> 16);
    render_ellipse_fan(x, y, radius, 0x2080u, 0u);
    x = (uint32)(sint32)(sint16)screen;
    y = (uint32)(sint32)(sint16)((uint32)screen >> 16);
    render_ellipse_fan(x, y, radius >> 1, 0x8888u, 0u);
}


uint32 game_create_aimed_projectile(uint32 source, uint32 target, uint32 mode)
{
    SVECTOR vertex;
    VECTOR origin, difference, direction;
    uint32 scale, position, x, y, z, player, distance, quotient, value, product, type, effect, speed;
    uint32 model, table, radius, numerator, velocity, next_velocity, multiplier;
    sint32 denominator;
    FUNCTION_MARKER(0x800AED2C, "GAME.OVL");
    game_choose_target_vertex_native(source, target, &vertex);
    ApplyMatrix((MATRIX *)psx_addr(source + 0x11Cu, 20u), &vertex, &origin);
    scale = r_u32(source + 0x84u);
    if ((sint32)scale < 0) {
        scale = 0u - scale;
        origin.vx >>= scale & 31u;
        origin.vz >>= scale & 31u;
        origin.vy >>= scale & 31u;
    } else if ((sint32)scale > 0) {
        origin.vx = (sint32)((uint32)origin.vx << (scale & 31u));
        origin.vz = (sint32)((uint32)origin.vz << (scale & 31u));
        origin.vy = (sint32)((uint32)origin.vy << (scale & 31u));
    }
    position = r_u32(source + 8u);
    origin.vx = (sint32)((uint32)origin.vx + position);
    position = r_u32(source + 12u);
    origin.vy = (sint32)((uint32)origin.vy + position);
    position = r_u32(source + 16u);
    origin.vz = (sint32)((uint32)origin.vz + position);
    x = r_u32(target + 8u) - (uint32)origin.vx;
    difference.vx = (sint32)x;
    y = r_u32(target + 12u) - (uint32)origin.vy;
    difference.vy = (sint32)y;
    z = r_u32(target + 16u) - (uint32)origin.vz;
    player = r_u32(0x80040FDCu);
    difference.vz = (sint32)z;
    if (target == player) {
        distance = (uint32)vector_length_approx((sint32)x, (sint32)y, (sint32)z);
        denominator = r_s32(0x800EF108u);
        if (denominator == 0)
            xport_mips_break(7u);
        if (distance == 0x80000000u && denominator == -1)
            xport_mips_break(0x5Du);
        quotient = (uint32)((sint32)distance / denominator);
        value = (uint32)(r_s32(target + 0x14u) >> 8);
        difference.vx = (sint32)((uint32)difference.vx + value * quotient);
        value = (uint32)(r_s32(target + 0x18u) >> 8);
        difference.vy = (sint32)((uint32)difference.vy + value * quotient);
        value = (uint32)(r_s32(target + 0x1Cu) >> 8);
        product = value * quotient;
        w_u32(0x80046174u, distance);
        difference.vz = (sint32)((uint32)difference.vz + product);
    } else {
        value = (uint32)random_between(0, 0xA28);
        difference.vx = (sint32)((uint32)difference.vx + value);
        value = (uint32)random_between(0, 0xA28);
        difference.vy = (sint32)((uint32)difference.vy + value);
        value = (uint32)random_between(0, 0xA28);
        difference.vz = (sint32)((uint32)difference.vz + value);
    }
    game_normalize_direction_native(&direction, &difference);
    type = r_u32(0x800EF154u);
    effect = game_allocate_effect(type, source);
    value = (uint32)direction.vx;
    speed = r_u32(0x800EF108u);
    product = value * speed;
    w_u32(effect + 0x20u, (uint32)((sint32)product >> 12));
    product = (uint32)direction.vy * speed;
    w_u32(effect + 0x24u, (uint32)((sint32)product >> 12));
    product = (uint32)direction.vz * speed;
    w_u32(effect + 0x28u, (uint32)((sint32)product >> 12));
    w_u32(effect, (uint32)origin.vx);
    w_u32(effect + 4u, (uint32)origin.vy);
    w_u32(effect + 8u, (uint32)origin.vz);
    value = r_u32(0x800EF158u);
    w_u16(effect + 0x3Cu, (uint16)value);
    if (mode != 0u)
        w_u16(effect + 0x46u, 0u);
    else {
        value = r_u16(effect + 0x46u);
        w_u16(effect + 0x46u, (uint16)(value | 1u));
    }
    model = r_u32(source + 0x74u);
    table = r_u32(0x80046178u);
    radius = r_u32(table + model * 132u + 0x6Cu);
    scale = r_u32(source + 0x84u);
    numerator = (radius << (scale & 31u)) + 0x320C8u;
    denominator = r_s32(0x800EF108u);
    if (denominator == 0)
        xport_mips_break(7u);
    if (numerator == 0x80000000u && denominator == -1)
        xport_mips_break(0x5Du);
    quotient = (uint32)((sint32)numerator / denominator);
    w_u32(effect + 0x2Cu, numerator);
    w_u32(effect + 0x2Cu, quotient);
    velocity = r_u32(effect + 0x20u);
    multiplier = r_u32(0x800EF160u);
    product = velocity * multiplier;
    next_velocity = r_u32(effect + 0x24u);
    w_u16(effect + 0x36u, (uint16)product);
    multiplier = r_u32(0x800EF160u);
    product = next_velocity * multiplier;
    next_velocity = r_u32(effect + 0x28u);
    w_u16(effect + 0x38u, (uint16)product);
    multiplier = r_u32(0x800EF160u);
    product = next_velocity * multiplier;
    w_u16(effect + 0x3Au, (uint16)product);
    value = r_u32(source);
    w_u32(effect + 0x14u, value == 3u ? 0x800EF3F4u : 0x800EF374u);
    w_u32(effect + 0x1Cu, 4u);
    w_u32(effect + 0x68u, 0x800EEC1Cu);
    game_draw_muzzle_glow_native(&origin);
    return effect;
}
