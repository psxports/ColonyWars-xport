#include "game_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint32 front_request_complete_campaign_map(void)
{
    FUNCTION_MARKER(0x800B4EDC, "FRONT.OVL");
    w_u8(0x800FB748u, 1u);
    return front_initialize_campaign_map();
}

uint32 front_initialize_campaign_map(void)
{
    uint32 value, index, record, active, destination, source, length, table, neighbor, ordinate, level, selected;
    sint32 divisor, spacing;
    FUNCTION_MARKER(0x800B4F08, "FRONT.OVL");
    front_configure_layer(1u, 3u, 0xA8u);
    w_u32(0x800FB740u, 0u);
    w_u8(0x800FB745u, 8u);
    w_u8(0x800FB744u, 30u);
    w_u8(0x800FB746u, 30u);
    w_u32(0x800FB238u, 0x800F2CE8u);
    if (r_u8(0x80010F62u) == 1u || r_u8(0x800FB748u) == 1u) {
        w_u32(0x800FB738u, 1u);
        w_u32(0x800FB734u, 1u);
    } else {
        w_u32(0x800FB738u, 0u);
        w_u32(0x800FB734u, 0u);
    }
    w_u32(0x800FB73Cu, 1u);
    w_u32(0x800FB730u, 1u);
    w_u32(0x800FB2F0u, 1u);
    w_u32(0x800FB2ECu, 1u);
    w_u32(0x800FB2D0u, 0u);
    w_u32(0x800FB2D4u, 0u);
    w_u32(0x800FB2D8u, 0u);
    w_u32(0x800FB2DCu, 0u);
    w_u32(0x800FB2E0u, 0u);
    w_u32(0x800FB2E4u, 0u);
    w_u32(0x800FB2E8u, 0u);
    w_u16(0x800FB240u, 0xFFFFu);
    w_u16(0x800FB242u, 0xFFFFu);
    for (index = 2u; index < 27u; ++index) {
        active = 0u;
        if (index < 19u) {
            if (r_u8(0x80010478u + index) != 0u || r_u32(0x800FB738u) == 1u)
                active = 1u;
        } else if (index >= 21u) {
            if (r_u8(0x80010F48u + index) == 1u || r_u32(0x800FB738u) == 1u) {
                active = 1u;
                w_u32(0x800FB2C8u + index * 40u, active);
                w_u32(0x800FB734u, 1u);
            }
        }
        if (!(index >= 21u && active == 1u))
            w_u32(0x800FB2C8u + index * 40u, active);
        record = 0x800FB2A8u + index * 40u;
        w_u32(record + 28u, index);
        w_u32(record, 0u);
        w_u32(record + 4u, 0u);
        w_u32(record + 8u, 0u);
        w_u32(record + 12u, 0u);
        w_u32(record + 16u, 0u);
        w_u32(record + 20u, 0u);
        w_u32(record + 24u, 0u);
        w_u16(0x800FB23Cu + index * 4u, 0xFFFFu);
        w_u16(0x800FB23Eu + index * 4u, 0xFFFFu);
    }
    front_link_campaign_node(0x800FB2D0u);
    divisor = r_s32(0x800FB730u);
    if (divisor == 0)
        abort();
    spacing = 450 / divisor;
    if (spacing >= 151)
        spacing = 150;
    for (index = 0u; index < 27u; ++index) {
        record = 0x800FB2A8u + index * 40u;
        if (r_s32(record + 32u) >= 2) {
            ordinate = r_u32(0x800BE6ECu + index * 4u) + 246u;
            w_u16(0x800FB23Eu + index * 4u, ordinate);
            level = r_u32(record + 24u);
            w_u16(0x800FB23Cu + index * 4u, level * (uint32)spacing + 36u);
            if (index < 20u) {
                if (r_u32(record + 4u) == 0u) {
                    neighbor = r_u8(0x800BE6B4u + index * 2u);
                    if (neighbor >= 91u)
                        neighbor -= 70u;
                    ordinate = r_u32(0x800BE6ECu + neighbor * 4u) + 246u;
                    w_u16(0x800FB23Eu + neighbor * 4u, ordinate);
                    level = r_u32(record + 24u) + 1u;
                    w_u16(0x800FB23Cu + neighbor * 4u, level * (uint32)spacing + 36u);
                }
                if (r_u32(record) == 0u) {
                    neighbor = r_u8(0x800BE6B5u + index * 2u);
                    if (neighbor >= 90u)
                        neighbor -= 70u;
                    value = r_u8(0x800BE6B5u + neighbor * 2u);
                    ordinate = r_u32(0x800BE6ECu + value * 4u) + 246u;
                    w_u16(0x800FB23Eu + neighbor * 4u, ordinate);
                    level = r_u32(0x800FB2C0u + neighbor * 40u) + 1u;
                    w_u16(0x800FB23Cu + neighbor * 4u, level * (uint32)spacing + 36u);
                }
            }
        }
    }
    w_u8(0x800F2FA9u, 2u);
    w_u8(0x800F2FAAu, 2u);
    w_u8(0x800F2FABu, 2u);
    w_u8(0x800F2FACu, 2u);
    w_u8(0x800F2FADu, 2u);
    w_u8(0x800F2FAEu, 2u);
    if (r_u32(0x800FB738u) == 1u) {
        index = 0u;
        selected = r_u32(0x800FB73Cu);
        value = r_u32(0x800BE648u + (selected << 2));
        while ((sint32)index < (sint32)value) {
            w_u8(0x800F2FA9u + index, 0u);
            selected = r_u32(0x800FB73Cu);
            value = r_u32(0x800BE648u + (selected << 2));
            ++index;
        }
        for (index = 0u; index < 6u; ++index) {
            table = r_u32(0x800FB0F4u);
            destination = r_u32(table + 8u + index * 4u);
            source = r_u32(table + 36u + index * 4u);
            length = 0u;
            while (r_u8(source + length) != 0u)
                ++length;
            strcpy((char *)psx_addr(destination, (size_t)length + 1u), (const char *)psx_addr(source, (size_t)length + 1u));
        }
    } else if (r_u32(0x800FB734u) == 1u) {
        table = r_u32(0x800FB0F4u);
        destination = r_u32(table + 8u);
        source = r_u32(table + 32u);
        length = 0u;
        while (r_u8(source + length) != 0u)
            ++length;
        strcpy((char *)psx_addr(destination, (size_t)length + 1u), (const char *)psx_addr(source, (size_t)length + 1u));
        w_u8(0x800F2FA9u, 0u);
    }
    selected = r_u32(0x800FB73Cu);
    table = r_u32(0x800FB0F4u);
    destination = r_u32(table + 4u);
    source = r_u32(table + ((sint32)selected >= 21 ? 64u : 60u));
    length = 0u;
    while (r_u8(source + length) != 0u)
        ++length;
    strcpy((char *)psx_addr(destination, (size_t)length + 1u), (const char *)psx_addr(source, (size_t)length + 1u));
    return destination;
}

void front_draw_campaign_map(void)
{
    uint32 input, value, selected, previous, index, table, destination, source, length, count;
    uint32 normal_color, link_color, color, record, neighbor, type, bright, draw;
    uint32 text, bank, menu;
    sint32 direction;
    sint16 points[4];
    FUNCTION_MARKER(0x800B55F0, "FRONT.OVL");
    input = r_u32(0x80053684u);
    if (input == 0u) {
        value = r_u8(0x800FB746u) - 1u;
        w_u8(0x800FB746u, value);
        if ((uint8)value == 0u) {
            value = r_u8(0x800FB745u);
            input = r_u32(0x8004616Cu);
            w_u8(0x800FB746u, value);
        }
    } else {
        value = r_u8(0x800FB744u);
        w_u8(0x800FB746u, value);
    }
    direction = 0;
    if (r_u32(0x800FB734u) == 1u) {
        if ((input & 0x8000u) != 0u)
            direction = -1;
        else if ((input & 0x2000u) != 0u)
            direction = 1;
    }
    if (direction != 0) {
        sound_play_sample_pitch(10u, 128u);
        sound_play_sample_pitch(10u, 128u);
        previous = r_u32(0x800FB73Cu);
        do {
            selected = r_u32(0x800FB73Cu) + (uint32)direction;
            w_u32(0x800FB73Cu, selected);
            if (direction < 0) {
                if ((sint32)selected <= 0)
                    w_u32(0x800FB73Cu, 26u);
            } else if ((sint32)selected >= 27) {
                w_u32(0x800FB73Cu, 1u);
            }
            selected = r_u32(0x800FB73Cu);
            value = r_u32(0x800FB2C8u + selected * 40u);
        } while ((sint32)value < 2 && selected != previous);
        if (r_u32(0x800FB738u) == 1u) {
            selected = r_u32(0x800FB73Cu);
            w_u8(0x800F2FA9u, 2u);
            w_u8(0x800F2FAAu, 2u);
            w_u8(0x800F2FABu, 2u);
            w_u8(0x800F2FACu, 2u);
            w_u8(0x800F2FADu, 2u);
            w_u8(0x800F2FAEu, 2u);
            count = r_u32(0x800BE648u + (selected << 2));
            index = 0u;
            while ((sint32)index < (sint32)count) {
                w_u8(0x800F2FA9u + index, 0u);
                selected = r_u32(0x800FB73Cu);
                count = r_u32(0x800BE648u + (selected << 2));
                ++index;
            }
        } else if (r_u32(0x800FB734u) == 1u) {
            selected = r_u32(0x800FB73Cu);
            value = r_u32(0x800BE648u + (selected << 2));
            w_u8(0x800F2FA9u, value != 0u ? 0u : 2u);
        }
        selected = r_u32(0x800FB73Cu);
        table = r_u32(0x800FB0F4u);
        destination = r_u32(table + 4u);
        source = r_u32(table + ((sint32)selected >= 21 ? 64u : 60u));
        length = 0u;
        while (r_u8(source + length) != 0u)
            ++length;
        strcpy((char *)psx_addr(destination, (size_t)length + 1u), (const char *)psx_addr(source, (size_t)length + 1u));
    }
    if (r_u32(0x800FB734u) == 1u) {
        normal_color = 0x404040u;
        link_color = 0x404040u;
    } else {
        normal_color = 0x808080u;
        link_color = 0xF0F0F0u;
    }
    w_u32(0x800FB870u, 83u);
    for (index = 1u; index < 27u; ++index) {
        bank = r_u32(0x800BE758u + index * 4u);
        w_u32(0x800FB870u, bank);
        record = 0x800FB2A8u + index * 40u;
        if (r_s32(record + 32u) < 2)
            continue;
        color = normal_color;
        if (r_u32(0x800FB734u) == 1u && index == r_u32(0x800FB73Cu))
            color = 0x808080u;
        front_draw_text_native("A", (uint32)r_s16(0x800FB23Cu + index * 4u) - 12u, (uint32)r_s16(0x800FB23Eu + index * 4u) - 12u, color, 0);
        points[0] = (sint16)r_u16(0x800FB23Cu + index * 4u);
        points[1] = (sint16)r_u16(0x800FB23Eu + index * 4u);
        bright = 0u;
        if (index != 25u || r_u32(0x800FB694u) != 0u) {
            if (index == 25u && r_u8(0x80010F62u) == 1u)
                bright = 1u;
            else if (r_u32(record + 4u) != 0u && (r_u8(0x80010478u + index) & 2u) != 0u)
                bright = 1u;
        }
        draw = 0u;
        color = link_color;
        if (bright != 0u) {
            draw = 1u;
            color = 0xF0F0F0u;
        } else if (r_u32(0x800FB738u) == 1u && r_u32(record + 4u) != 0u) {
            draw = 1u;
        } else if (index < 20u) {
            draw = 2u;
        }
        if (draw == 1u) {
            neighbor = r_u32(record + 4u);
            type = r_u32(neighbor + 28u);
            points[2] = (sint16)r_u16(0x800FB23Cu + (type << 2));
            neighbor = r_u32(record + 4u);
            type = r_u32(neighbor + 28u);
            points[3] = (sint16)r_u16(0x800FB23Eu + (type << 2));
            front_draw_embossed_line(points, color, 1u);
        } else if (draw == 2u) {
            neighbor = r_u8(0x800BE6B4u + index * 2u);
            if (neighbor >= 90u)
                neighbor -= 70u;
            points[2] = (sint16)r_u16(0x800FB23Cu + (neighbor << 2));
            points[3] = (sint16)r_u16(0x800FB23Eu + (neighbor << 2));
            front_draw_gradient_embossed_line(points, link_color, 0u, 1u);
        }
        draw = 0u;
        color = link_color;
        neighbor = r_u32(record);
        if (neighbor != 0u && (r_u8(0x80010478u + index) & 1u) != 0u) {
            draw = 1u;
            color = 0xF0F0F0u;
        } else if (r_u32(0x800FB738u) == 1u && r_u32(record) != 0u) {
            draw = 1u;
        } else if (index != 1u && index < 20u) {
            draw = 2u;
        }
        if (draw == 1u) {
            neighbor = r_u32(record);
            type = r_u32(neighbor + 28u);
            points[2] = (sint16)r_u16(0x800FB23Cu + (type << 2));
            neighbor = r_u32(record);
            type = r_u32(neighbor + 28u);
            points[3] = (sint16)r_u16(0x800FB23Eu + (type << 2));
            front_draw_embossed_line(points, color, 1u);
        } else if (draw == 2u) {
            neighbor = r_u8(0x800BE6B5u + index * 2u);
            if (neighbor >= 90u)
                neighbor -= 70u;
            points[2] = (sint16)r_u16(0x800FB23Cu + (neighbor << 2));
            if (points[2] < points[0])
                points[2] = (sint16)((uint32)(sint32)points[0] + 24u);
            points[3] = (sint16)(r_u32(0x800BE6ECu + (neighbor << 2)) + 246u);
            front_draw_gradient_embossed_line(points, link_color, 0u, 1u);
        }
    }
    selected = r_u32(0x800FB73Cu);
    w_u32(0x800FB870u, 253u);
    front_draw_colored_text(r_u32(0x800F2468u + (selected << 2)), 46u, 64u, 0x808080u);
    menu = (uint32)r_s16(0x8010D0DCu);
    if ((sint32)menu >= 32 && r_u32(0x800FB738u) == 1u) {
        selected = r_u32(0x800FB73Cu);
        text = r_u32(0x800F24BCu + selected * 24u + (menu << 2) - 128u);
        front_draw_colored_text(text, 46u, 90u, 0x808080u);
    } else if (r_u32(0x800FB734u) == 1u && r_u32(0x800FB738u) == 0u) {
        table = r_u32(0x800FB238u);
        front_draw_colored_text(r_u32(table + 20u), 46u, 90u, 0x808080u);
        table = r_u32(0x800FB238u);
        front_draw_colored_text(r_u32(table + 24u), 46u, 110u, 0x808080u);
        table = r_u32(0x800FB238u);
        w_u32(0x800FB870u, 83u);
        front_draw_colored_text(r_u32(table + 28u), 46u, 140u, 0x808080u);
        table = r_u32(0x800FB238u);
        front_draw_colored_text(r_u32(table + 32u), 46u, 155u, 0x808080u);
        w_u32(0x800FB870u, 253u);
    }
    selected = r_u32(0x800FB73Cu);
    if ((sint32)selected < 20) {
        selected = r_u32(0x800FB73Cu);
        bank = r_u32(0x800BE758u + (selected << 2));
        table = r_u32(0x800FB238u);
        text = r_u32(table + (bank << 2) - 0x744u);
        front_draw_colored_text(text, 46u, 390u, 0x808080u);
    }
}

uint32 front_select_campaign_mission(void)
{
    uint32 selected, mission, saved, state;
    char name[24];
    FUNCTION_MARKER(0x800B62E0, "FRONT.OVL");
    selected = r_u32(0x800FB73Cu);
    mission = r_u16(0x8010D0DCu) - 31u;
    w_u16(0x800595FCu, selected);
    w_u16(0x8005960Cu, selected);
    w_u16(0x800595F8u, selected);
    w_u16(0x800595FEu, mission);
    w_u16(0x8005960Eu, mission);
    w_u16(0x800595FAu, mission);
    sprintf(name, "%d_%d.CW", (sint32)(sint16)selected, (sint32)(sint16)mission);
    memcpy(psx_addr(0x80010518u, strlen(name) + 1u), name, strlen(name) + 1u);
    if (resource_exists(name) == 0) {
        w_u16(0x800595FCu, 1u);
        w_u16(0x8005960Cu, 1u);
        w_u16(0x800595F8u, 1u);
        w_u16(0x800595FEu, 1u);
        w_u16(0x8005960Eu, 1u);
        w_u16(0x800595FAu, 1u);
        sprintf(name, "%d_%d.CW", 1, 1);
        memcpy(psx_addr(0x80010518u, strlen(name) + 1u), name, strlen(name) + 1u);
    }
    selected = r_u32(0x800FB73Cu);
    saved = r_u32(0x800FB740u);
    w_u32(0x800F3150u, 0u);
    w_u32(0x800F313Cu, selected == saved ? 1u : 0u);
    state = r_u32(0x800595F8u);
    w_u32(0x800FB740u, 0u);
    if (state == 0x10001u)
        return front_reset_settings();
    return state;
}

void front_finish_campaign_map(void)
{
    sint32 selected;
    FUNCTION_MARKER(0x800B643C, "FRONT.OVL");
    selected = r_s32(0x800FB740u);
    if (selected > 0) {
        front_play_numbered_movie(selected);
        w_u32(0x800FB740u, 0u);
    } else {
        w_u8(0x800FB748u, 0u);
    }
}

void front_play_selected_campaign_movie(void)
{
    sint32 selected;
    FUNCTION_MARKER(0x800B648C, "FRONT.OVL");
    selected = r_s32(0x800FB740u);
    if (selected > 0)
        front_play_numbered_movie(selected);
    w_u32(0x800FB740u, 0u);
}

uint32 front_save_campaign_selection(void)
{
    uint32 selected;
    FUNCTION_MARKER(0x800B64D0, "FRONT.OVL");
    selected = r_u32(0x800FB73Cu);
    w_u32(0x800FB740u, selected);
    return selected;
}

void front_initialize_mission_results(void)
{
    uint32 table, destination, source, length, clear_secondary, models, mesh, rank;
    FUNCTION_MARKER(0x800B6BEC, "FRONT.OVL");
    front_configure_layer(0u, 5u, 0xA9u);
    front_configure_layer(1u, 3u, 0xA8u);
    w_u8(0x800F2FA9u, r_s16(0x800595F8u) >= 2 && r_s16(0x800595FAu) == 1 ? 0u : 2u);
    clear_secondary = 0u;
    if (r_u32(0x800F3150u) == 1u) {
        table = r_u32(0x800FB0F4u);
        destination = r_u32(table + 4u);
        source = r_u32(table + 12u);
    } else if (r_u32(0x800F3154u) == 1u) {
        table = r_u32(0x800FB0F4u);
        destination = r_u32(table + 4u);
        source = r_u32(table + 20u);
        clear_secondary = 1u;
    } else {
        table = r_u32(0x800FB0F4u);
        destination = r_u32(table + 4u);
        source = r_u32(table + 16u);
    }
    length = 0u;
    while (r_u8(source + length) != 0u)
        ++length;
    strcpy((char *)psx_addr(destination, (size_t)length + 1u), (const char *)psx_addr(source, (size_t)length + 1u));
    if (clear_secondary != 0u)
        w_u32(0x800F3154u, 0u);
    w_u32(0x8003119Cu, 320u);
    w_u32(0x800F6F98u, 8u);
    front_prepare_control_model_bundle("RANKING.BND");
    models = r_u32(0x80046178u);
    mesh = mesh_import_primitives(r_u32(models));
    models = r_u32(0x80046178u);
    w_u32(0x800FB120u, mesh);
    mesh = mesh_import_primitives(r_u32(models + 132u));
    models = r_u32(0x80046178u);
    w_u32(0x800FB12Cu, mesh);
    mesh = mesh_import_primitives(r_u32(models + 264u));
    models = r_u32(0x80046178u);
    w_u32(0x800FB130u, mesh);
    mesh = mesh_import_primitives(r_u32(models + 396u));
    models = r_u32(0x80046178u);
    w_u32(0x800FB124u, mesh);
    mesh = mesh_import_primitives(r_u32(models + 528u));
    w_u32(0x800FB128u, mesh);
    rank = front_campaign_rank();
    front_initialize_rank_models(rank);
    w_u32(0x800BC6E4u, 250u);
    front_initialize_rank_lighting();
    front_initialize_mission_statistics();
}

void front_draw_mission_results(void)
{
    uint32 group, mission, text;
    FUNCTION_MARKER(0x800B7168, "FRONT.OVL");
    group = (uint32)r_s16(0x80059608u);
    w_u32(0x800FB870u, 253u);
    mission = (uint32)r_s16(0x8005960Au);
    text = r_u32(0x800F24BCu + group * 24u + (mission << 2) - 4u);
    front_draw_colored_text(text, 46u, 64u, 0x808080u);
    front_draw_rank_models();
    front_draw_mission_statistics();
}

void front_free_mission_results(void)
{
    FUNCTION_MARKER(0x800B71E4, "FRONT.OVL");
    front_free_preview_models();
    resource_free_bundle(r_u32(0x800FB120u));
    resource_free_bundle(r_u32(0x800FB124u));
    resource_free_bundle(r_u32(0x800FB128u));
    resource_free_bundle(r_u32(0x800FB12Cu));
    resource_free_bundle(r_u32(0x800FB130u));
    heap_free(r_u32(0x800BC6D0u));
    w_u32(0x800BC6D0u, 0u);
}

uint32 game_copy_player_view_transform(void)
{
    uint32 player, view, value;
    FUNCTION_MARKER(0x80078C00, "GAME.OVL");
    player = r_u32(0x80040FDCu);
    view = r_u32(0x8003FE74u);
    value = r_u16(player + 0x11Cu);
    w_u16(view + 0x11Cu, value);
    value = r_u16(player + 0x122u);
    w_u16(view + 0x11Eu, value);
    value = r_u16(player + 0x128u);
    w_u16(view + 0x120u, value);
    value = r_u16(player + 0x11Eu);
    w_u16(view + 0x122u, value);
    value = r_u16(player + 0x124u);
    w_u16(view + 0x124u, value);
    value = r_u16(player + 0x12Au);
    w_u16(view + 0x126u, value);
    value = r_u16(player + 0x120u);
    w_u16(view + 0x128u, value);
    value = r_u16(player + 0x126u);
    w_u16(view + 0x12Au, value);
    value = r_u16(player + 0x12Cu);
    w_u16(view + 0x12Cu, value);
    value = r_u32(player + 8u);
    w_u32(view + 8u, value);
    value = r_u32(player + 12u);
    w_u32(view + 12u, value);
    value = r_u32(player + 16u);
    w_u32(view + 16u, value);
    return value;
}

uint32 game_draw_player_view(void)
{
    uint32 player, view, value, selected, definition, type, models, mesh, result;
    FUNCTION_MARKER(0x80078A2C, "GAME.OVL");
    if (r_s32(0x800313DCu) < 4 || r_u32(0x800EC868u) != 0u)
        return 1u;
    if (r_u32(0x800EC884u) == 0u) {
        player = r_u32(0x80040FDCu);
        view = r_u32(0x8003FE74u);
        value = r_u16(player + 0x11Cu);
        w_u16(view + 0x11Cu, value);
        value = r_u16(player + 0x122u);
        w_u16(view + 0x11Eu, value);
        value = r_u16(player + 0x128u);
        w_u16(view + 0x120u, value);
        value = r_u16(player + 0x11Eu);
        w_u16(view + 0x122u, value);
        value = r_u16(player + 0x124u);
        w_u16(view + 0x124u, value);
        value = r_u16(player + 0x12Au);
        w_u16(view + 0x126u, value);
        value = r_u16(player + 0x120u);
        w_u16(view + 0x128u, value);
        value = r_u16(player + 0x126u);
        w_u16(view + 0x12Au, value);
        value = r_u16(player + 0x12Cu);
        w_u16(view + 0x12Cu, value);
        value = r_u32(player + 8u);
        w_u32(view + 8u, value);
        value = r_u32(player + 12u);
        w_u32(view + 12u, value);
        value = r_u32(player + 16u);
        w_u32(view + 16u, value);
    }
    selected = r_u32(0x800ECBD0u);
    view = r_u32(0x8003FE74u);
    definition = 0x800ECBECu + selected * 44u;
    type = (uint32)r_s16(definition);
    w_u32(view + 0x74u, type);
    type = (uint32)r_s16(definition);
    models = r_u32(0x80046178u);
    mesh = r_u32(models + type * 132u);
    w_u32(view + 0x38u, 0x8002B8FCu);
    w_u32(view + 0x64u, mesh);
    result = object_draw_visible(view);
    if (r_s16(0x8005961Cu) != 0) {
        player = r_u32(0x80040FDCu);
        if (r_u32(player + 0x40u) != 0x8007ADB8u) {
            view = r_u32(0x8003FE74u);
            value = r_u32(player + 0xECu);
            w_u32(view + 0xECu, value);
        }
    }
    game_draw_engine_effects(r_u32(0x8003FE74u));
    return result;
}

void game_free_player_view(void)
{
    uint32 view, emitter;
    FUNCTION_MARKER(0x80078ED4, "GAME.OVL");
    view = r_u32(0x8003FE74u);
    emitter = r_u32(view + 0x1E8u);
    if (emitter != 0u)
        heap_free(emitter);
    view = r_u32(0x8003FE74u);
    emitter = r_u32(view + 0x1ECu);
    w_u32(view + 0x1E8u, 0u);
    if (emitter != 0u)
        heap_free(emitter);
    view = r_u32(0x8003FE74u);
    emitter = r_u32(view + 0x1F0u);
    w_u32(view + 0x1ECu, 0u);
    if (emitter != 0u)
        heap_free(emitter);
    view = r_u32(0x8003FE74u);
    w_u32(view + 0x1F0u, 0u);
    game_mark_object_for_removal(view);
}

uint32 game_mark_object_for_removal(uint32 object)
{
    uint32 state, count, offset;
    FUNCTION_MARKER(0x800ADFC0, "GAME.OVL");
    state = r_u32(object);
    w_u16(object + 0x24Au, 0u);
    if (state != 0u) {
        w_u32(object + 0x9Cu, 1u);
        w_u32(object, 0u);
        w_u32(object + 0x40u, 0x800ADF8Cu);
        return 0x800ADF8Cu;
    }
    count = r_u32(0x800311F8u);
    offset = count << 2;
    ++count;
    w_u32(0x800311F8u, count);
    w_u32(0x80046090u + offset, object);
    if (count >= 55u)
        diagnostic_report(1200u, (intptr)0x80075E8Cu, (intptr)0x80075EA4u);
    return offset;
}

uint32 game_initialize_player_objects(void)
{
    uint32 state, kind, player, view, identity_player, value, emitter;
    sint32 mode;
    FUNCTION_MARKER(0x80078CA0, "GAME.OVL");
    state = r_u32(0x800595F8u);
    w_u16(0x80059612u, state == 0x30007u ? 3u : 1u);
    kind = (uint32)r_s16(0x80059612u);
    player = game_create_object(1u, 0u, 0u, 0u, kind);
    w_u32(0x800EDD20u, 0u);
    w_u32(0x80040FDCu, player);
    view = game_create_object(0u, 0u, 0u, 0u, 0u);
    w_u32(0x8003FE74u, view);
    w_u32(view + 0x168u, 0u);
    w_u32(view + 0x84u, (uint32)-3);
    w_u32(player, 102u);
    w_u32(player + 4u, 102u);
    identity_player = r_u32(0x80040FDCu);
    w_u32(player + 0x114u, 8192u);
    w_u32(player + 0x44u, 0x80079E5Cu);
    w_u32(player + 0x40u, 0x80078F5Cu);
    w_u32(player + 0x3Cu, 0x80078A24u);
    w_u32(player + 0x84u, 0u);
    w_u32(player + 0xA0u, 1u);
    w_u32(player + 0xFCu, 0u);
    w_u32(player + 0x100u, 0u);
    w_u32(player + 0x104u, 0u);
    w_u32(player + 0x78u, 0u);
    w_u32(player + 0x7Cu, 0u);
    w_u32(player + 0x80u, 0u);
    w_u32(player + 0xD8u, 0u);
    w_u32(player + 0xDCu, 0u);
    w_u32(player + 0x48u, 0x80078ECCu);
    value = r_u32(0x800118F0u);
    w_u32(player + 0x238u, value);
    value = r_u32(0x800118ECu);
    w_u32(player + 0x1E4u, 0x800EEC5Cu);
    w_u32(player + 0x234u, value);
    matrix_set_identity(identity_player + 0x11Cu);
    player = r_u32(0x80040FDCu);
    w_u32(player + 0x20u, 0u);
    w_u32(player + 0x24u, 0u);
    w_u32(player + 0x28u, 0u);
    matrix_copy_rotation(0x80035060u, player + 0x11Cu);
    mode = r_s16(0x800118F4u);
    if (mode == 0) {
        w_u32(0x800313DCu, 3u);
        w_u32(0x800ECE70u, 1u);
    } else if (mode == 1) {
        w_u32(0x800313DCu, 3u);
        w_u32(0x800ECE70u, 0u);
    } else {
        player = r_u32(0x80040FDCu);
        w_u32(0x800313DCu, 4u);
        w_u32(0x800ECE70u, 0u);
        w_u32(0x8004EB1Cu, player);
    }
    emitter = heap_alloc_back(36u);
    view = r_u32(0x8003FE74u);
    w_u32(view + 0x1E8u, emitter);
    emitter = heap_alloc_back(36u);
    view = r_u32(0x8003FE74u);
    w_u32(view + 0x1ECu, emitter);
    emitter = heap_alloc_back(36u);
    view = r_u32(0x8003FE74u);
    w_u32(view + 0x1F0u, emitter);
    return emitter;
}

uint32 game_create_object(uint32 model, uint32 x, uint32 y, uint32 z, uint32 kind)
{
    uint32 pool, object, models, mesh, clock, motion, serial, index;
    FUNCTION_MARKER(0x800ADD70, "GAME.OVL");
    pool = kind - 1u < 9u ? kind & 255u : 0u;
    object = object_allocate(pool);
    models = r_u32(0x80046178u);
    w_u32(object, kind);
    w_u32(object + 0xA4u, 0u);
    w_u32(object + 0x74u, model);
    clock = r_u32(0x800311B8u);
    mesh = r_u32(models + model * 132u);
    w_u32(object + 0x90u, x);
    w_u32(object + 8u, x);
    w_u32(object + 0x94u, y);
    w_u32(object + 12u, y);
    w_u32(object + 0x98u, z);
    w_u32(object + 16u, z);
    w_u32(object + 0x20u, 0u);
    w_u32(object + 0x24u, 0u);
    w_u32(object + 0x28u, 0u);
    w_u32(object + 0x2Cu, 0xFFFFFFFFu);
    w_u32(object + 0x30u, 0xFFFFFFFFu);
    w_u32(object + 0x34u, 0xFFFFFFFFu);
    w_u32(object + 0xFCu, 0u);
    w_u32(object + 0x14u, 0u);
    w_u32(object + 0x100u, 0u);
    w_u32(object + 0x18u, 0u);
    w_u32(object + 0x104u, 0u);
    w_u32(object + 0x1Cu, 0u);
    w_u32(object + 0x15Cu, 0u);
    w_u32(object + 0xF8u, clock);
    w_u32(object + 0x64u, mesh);
    if ((sint32)model > 0) {
        w_u32(object + 0x3Cu, 0x8002D2BCu);
        w_u32(object + 0x38u, 0x8002B8FCu);
    } else {
        w_u32(object + 0x3Cu, 0x8002493Cu);
        w_u32(object + 0x38u, 0x8002493Cu);
    }
    w_u8(object + 0x270u, 0u);
    motion = r_u32(0x800313E4u);
    w_u32(object + 0xD4u, 0x808080u);
    w_u32(object + 0x40u, 0x8002493Cu);
    w_u32(object + 0x60u, 0x8002493Cu);
    w_u32(object + 0x44u, 0x8002493Cu);
    w_u32(object + 0x50u, 0x8002493Cu);
    w_u32(object + 0x4Cu, 0x8002493Cu);
    w_u32(object + 0x5Cu, 0x8002493Cu);
    w_u32(object + 0x48u, 0x800ADFC0u);
    w_u32(object + 0x70u, 10u);
    w_u32(object + 0x110u, 0u);
    w_u32(object + 0x114u, 0u);
    w_u32(object + 0x68u, 0u);
    w_u32(object + 0x22Cu, 0u);
    w_u32(object + 0xA0u, 0u);
    w_u32(object + 0x1E0u, 0u);
    w_u32(object + 0x168u, 0u);
    w_u32(object + 0x160u, 0u);
    w_u32(object + 0x178u, 0u);
    w_u32(object + 0x1B0u, 0u);
    w_u32(object + 0x1A0u, 0u);
    w_u32(object + 0x198u, 0u);
    w_u32(object + 0x84u, 0u);
    for (index = 0u; index < 6u; ++index)
        w_u16(object + 0x250u + index * 2u, 0u);
    w_u32(object + 0x1E4u, 0u);
    w_u32(object + 0x6Cu, 0u);
    w_u32(object + 0xD8u, motion);
    w_u32(object + 0x20Cu, 0u);
    w_u8(object + 0x26Cu, 1u);
    w_u8(object + 0x26Bu, 1u);
    w_u8(object + 0x274u, 0u);
    serial = r_u16(0x80031254u);
    w_u32(object + 0xA0u, 1u);
    w_u32(object + 4u, 0u);
    w_u32(object + 0xDCu, 0u);
    w_u32(object + 0x278u, 0u);
    w_u16(object + 0x24Cu, 0u);
    w_u32(object + 0x118u, 0u);
    w_u16(object + 0x24Au, serial);
    w_u16(0x80031254u, serial + 1u);
    return object;
}

uint32 game_update_object_removal(uint32 object)
{
    uint32 remaining;
    FUNCTION_MARKER(0x800ADF8C, "GAME.OVL");
    remaining = r_u32(object + 0x9Cu) - 1u;
    w_u32(object + 0x9Cu, remaining);
    if ((sint32)remaining <= 0)
        return game_mark_object_for_removal(object);
    return remaining;
}

uint32 game_initialize_player_weapon_slots(void)
{
    uint32 selected, definition, slot, count, base, primary, secondary, packed;
    FUNCTION_MARKER(0x80078054, "GAME.OVL");
    selected = r_u32(0x800ECBD0u);
    w_u32(0x80127038u, 0u);
    w_u32(0x8012703Cu, 0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    packed = (uint32)r_s16(definition + 20u);
    w_u32(0x800ECBCCu, 0u);
    if (packed != 0u) {
        game_initialize_weapon_profile_14(0u);
        slot = r_u32(0x80127038u);
        w_u32(0x80119C58u + slot * 52u, 50u);
        count = r_u32(0x80127038u);
        w_u32(0x80127038u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    packed = (uint32)r_s16(definition + 0x12u);
    if (packed != 0u) {
        slot = r_u32(0x80127038u);
        game_initialize_weapon_profile_12(slot);
        slot = r_u32(0x80127038u);
        w_u32(0x80119C58u + slot * 52u, 50u);
        count = r_u32(0x80127038u);
        w_u32(0x80127038u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    packed = (uint32)r_s16(definition + 0x16u);
    if (packed != 0u) {
        slot = r_u32(0x80127038u);
        game_initialize_weapon_profile_16(slot);
        slot = r_u32(0x80127038u);
        w_u32(0x80119C58u + slot * 52u, 50u);
        count = r_u32(0x80127038u);
        w_u32(0x800ECBCCu, slot);
        w_u32(0x80127038u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    packed = (uint32)r_s16(definition + 0x10u);
    if (packed != 0u) {
        slot = r_u32(0x80127038u);
        game_initialize_weapon_profile_10(slot);
        slot = r_u32(0x80127038u);
        w_u32(0x80119C58u + slot * 52u, 50u);
        count = r_u32(0x80127038u);
        w_u32(0x80127038u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    packed = (uint32)r_s16(definition + 0x1Au);
    if (packed != 0u) {
        slot = r_u32(0x80127038u);
        game_initialize_weapon_profile_1a(slot);
        slot = r_u32(0x80127038u);
        w_u32(0x80119C58u + slot * 52u, 50u);
        count = r_u32(0x80127038u);
        w_u32(0x80127038u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    packed = (uint32)r_s16(definition + 0x18u);
    if (packed != 0u) {
        slot = r_u32(0x80127038u);
        game_initialize_weapon_profile_18(slot);
        slot = r_u32(0x80127038u);
        w_u32(0x80119C58u + slot * 52u, 50u);
        count = r_u32(0x80127038u);
        w_u32(0x80127038u, count + 1u);
    }
    primary = r_u32(0x80127038u);
    selected = r_u32(0x800ECBD0u);
    w_u32(0x80127040u, 0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    w_u32(0x80127044u, primary);
    if (r_s16(definition + 0x4u) != 0) {
        game_initialize_weapon_profile_04(primary);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 2u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    if (r_s16(definition + 0x2u) != 0) {
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        game_initialize_weapon_profile_02(base + count);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 2u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    if (r_s16(definition + 0x6u) != 0) {
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        game_initialize_weapon_profile_06(base + count);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 2u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    if (r_s16(definition + 0x0u) != 0) {
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        game_initialize_weapon_profile_00(base + count);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 2u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    if (r_s16(definition + 0xAu) != 0) {
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        game_initialize_weapon_profile_0a(base + count);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 1u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    if (r_s16(definition + 0xEu) != 0) {
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        game_initialize_weapon_profile_0e(base + count);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 1u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    if (r_s16(definition + 0xCu) != 0) {
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        game_initialize_weapon_profile_0c(base + count);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 1u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    packed = r_u32(0x800595F8u);
    if (packed == 0x10004u || packed == 0x1000Bu) {
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        game_initialize_mission_weapon_slot(base + count);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 9999u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    if (r_s16(definition + 0x20u) != 0) {
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        game_initialize_weapon_profile_20(base + count);
        base = r_u32(0x80127044u);
        count = r_u32(0x80127040u);
        slot = base + count;
        w_u32(0x80119C58u + slot * 52u, 50u);
        count = r_u32(0x80127040u);
        w_u32(0x80127040u, count + 1u);
    }
    primary = r_u32(0x80127044u);
    selected = r_u32(0x8012703Cu);
    secondary = r_u32(0x80127040u);
    w_u32(0x800EC878u, primary);
    w_u32(0x800EF4CCu, primary);
    slot = primary + secondary;
    w_u32(0x800EC874u, selected);
    w_u32(0x800EF4B8u, selected);
    w_u32(0x800ECBC8u, slot);
    w_u32(0x80119C58u + slot * 52u, 5u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_14(uint32 slot)
{
    uint32 selected, definition, record, value, first_parameter, second_parameter;
    sint32 variant;
    FUNCTION_MARKER(0x8007D8E0, "GAME.OVL");
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    variant = r_s16(definition + 20u);
    first_parameter = variant == 1 ? 15u : variant == 2 ? 20u : 35u;
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    variant = r_s16(definition + 20u);
    second_parameter = variant == 2 ? 30u : 35u;
    record = 0x80119C30u + slot * 52u;
    value = r_u32(0x800ECDD4u);
    w_u32(record, value);
    value = r_u32(0x800ECDD8u);
    w_u32(record + 4u, value);
    value = r_u32(0x800ECDDCu);
    w_u32(record + 8u, value);
    if (r_u8(0x80011937u) != 0u)
        w_u32(record + 12u, 0u);
    else
        w_u32(record + 12u, first_parameter);
    w_u32(record + 20u, 1600u);
    w_u32(record + 44u, 0x8007E8F4u);
    w_u32(record + 16u, second_parameter);
    w_u16(record + 48u, 0u);
    w_u16(record + 50u, 0u);
    w_u32(record + 24u, 0u);
    w_u32(record + 32u, 0u);
    w_u32(record + 28u, 0u);
    w_u32(record + 36u, 2u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_12(uint32 slot)
{
    uint32 selected, definition, record, value, first_parameter, second_parameter;
    sint32 variant;
    FUNCTION_MARKER(0x8007D568, "GAME.OVL");
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    variant = r_s16(definition + 18u);
    first_parameter = variant == 1 ? 15u : variant == 2 ? 20u : 35u;
    selected = r_u32(0x800ECBD0u);
    definition = r_u32(0x800ECBF8u + selected * 44u);
    variant = r_s16(definition + 18u);
    second_parameter = variant == 2 ? 30u : 35u;
    record = 0x80119C30u + slot * 52u;
    value = r_u32(0x800ECDC0u);
    w_u32(record, value);
    value = r_u32(0x800ECDC4u);
    w_u32(record + 4u, value);
    value = r_u32(0x800ECDC8u);
    w_u32(record + 8u, value);
    if (r_u8(0x80011937u) != 0u)
        w_u32(record + 12u, 0u);
    else
        w_u32(record + 12u, first_parameter);
    w_u32(record + 20u, 1600u);
    w_u32(record + 44u, 0x8007E354u);
    w_u32(record + 16u, second_parameter);
    w_u16(record + 48u, 0u);
    w_u16(record + 50u, 0u);
    w_u32(record + 24u, 0u);
    w_u32(record + 32u, 0u);
    w_u32(record + 28u, 0u);
    w_u32(record + 36u, 1u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_16(uint32 slot)
{
    uint32 record, value, parameter;
    FUNCTION_MARKER(0x8007D770, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    value = r_u32(0x800ECDC0u);
    w_u32(record, value);
    value = r_u32(0x800ECDC4u);
    w_u32(record + 4u, value);
    value = r_u32(0x800ECDC8u);
    w_u32(record + 8u, value);
    if (r_u8(0x80011937u) != 0u) {
        w_u32(record + 12u, 0u);
    } else {
        value = r_u32(0x800ECDCCu);
        w_u32(record + 12u, value);
    }
    parameter = r_u32(0x800ECDD0u);
    w_u32(record + 20u, 1600u);
    w_u32(record + 44u, 0x8007E474u);
    w_u16(record + 48u, 0u);
    w_u16(record + 50u, 0u);
    w_u32(record + 24u, 0u);
    w_u32(record + 32u, 0u);
    w_u32(record + 28u, 0u);
    w_u32(record + 36u, 1u);
    w_u32(record + 16u, parameter);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_10(uint32 slot)
{
    uint32 record, value;
    FUNCTION_MARKER(0x8007DAE8, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    value = r_u32(0x800ECDC0u);
    w_u32(record, value);
    value = r_u32(0x800ECDC4u);
    w_u32(record + 4u, value);
    value = r_u32(0x800ECDC8u);
    w_u32(record + 8u, value);
    if (r_u8(0x80011937u) != 0u)
        w_u32(record + 12u, 0u);
    else
        w_u32(record + 12u, 30u);
    w_u32(record + 16u, 30u);
    w_u32(record + 20u, 1600u);
    w_u32(record + 44u, 0x8007E6B4u);
    w_u16(record + 48u, 0u);
    w_u16(record + 50u, 0u);
    w_u32(record + 24u, 0u);
    w_u32(record + 32u, 0u);
    w_u32(record + 28u, 0u);
    w_u32(record + 36u, 4u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_1a(uint32 slot)
{
    uint32 record, value;
    FUNCTION_MARKER(0x8007DC48, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    value = r_u32(0x800ECDC0u);
    w_u32(record, value);
    value = r_u32(0x800ECDC4u);
    w_u32(record + 4u, value);
    value = r_u32(0x800ECDC8u);
    w_u32(record + 8u, value);
    if (r_u8(0x80011937u) != 0u)
        w_u32(record + 12u, 0u);
    else
        w_u32(record + 12u, 30u);
    w_u32(record + 16u, 30u);
    w_u32(record + 20u, 1600u);
    w_u32(record + 44u, 0x8007E7D4u);
    w_u16(record + 48u, 0u);
    w_u16(record + 50u, 0u);
    w_u32(record + 24u, 0u);
    w_u32(record + 32u, 0u);
    w_u32(record + 28u, 0u);
    w_u32(record + 36u, 3u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_18(uint32 slot)
{
    uint32 record, value;
    FUNCTION_MARKER(0x8007DDA8, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    value = r_u32(0x800ECDC0u);
    w_u32(record, value);
    value = (uint32)(r_s32(0x800ECDC4u) >> 1);
    w_u32(record + 4u, value);
    value = r_u32(0x800ECDC8u);
    w_u32(record + 8u, value);
    if (r_u8(0x80011937u) != 0u)
        w_u32(record + 12u, 0u);
    else
        w_u32(record + 12u, 25u);
    w_u32(record + 16u, 25u);
    w_u32(record + 20u, 1600u);
    w_u32(record + 44u, 0x8007E594u);
    w_u16(record + 48u, 0u);
    w_u16(record + 50u, 0u);
    w_u32(record + 24u, 0u);
    w_u32(record + 32u, 0u);
    w_u32(record + 28u, 0u);
    w_u32(record + 36u, 5u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_02(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007DF0C, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007EA14u);
    w_u16(record + 48u, 1u);
    w_u32(record + 36u, 10u);
    w_u16(record + 50u, 1u);
    return slot * 52u;
}

uint32 game_initialize_mission_weapon_slot(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007DF78, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007EA8Cu);
    w_u16(record + 48u, 2u);
    w_u32(record + 36u, 15u);
    w_u16(record + 50u, 1u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_04(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007DFE8, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007EB80u);
    w_u16(record + 48u, 1u);
    w_u32(record + 36u, 16u);
    w_u16(record + 50u, 1u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_06(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007E054, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007EB08u);
    w_u16(record + 48u, 1u);
    w_u32(record + 36u, 8u);
    w_u16(record + 50u, 1u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_00(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007E0C0, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007EBF8u);
    w_u16(record + 48u, 1u);
    w_u32(record + 36u, 9u);
    w_u16(record + 50u, 1u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_0e(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007E12C, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007ED04u);
    w_u16(record + 48u, 2u);
    w_u32(record + 36u, 11u);
    w_u16(record + 50u, 2u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_0a(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007E198, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007EDE4u);
    w_u16(record + 48u, 2u);
    w_u32(record + 36u, 12u);
    w_u16(record + 50u, 2u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_0c(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007E204, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007EC90u);
    w_u16(record + 48u, 2u);
    w_u32(record + 36u, 13u);
    w_u16(record + 50u, 2u);
    return slot * 52u;
}

uint32 game_initialize_weapon_profile_20(uint32 slot)
{
    uint32 record;
    FUNCTION_MARKER(0x8007E270, "GAME.OVL");
    record = 0x80119C30u + slot * 52u;
    w_u32(record + 44u, 0x8007EEC4u);
    w_u16(record + 48u, 3u);
    w_u32(record + 36u, 6u);
    w_u16(record + 50u, 1u);
    return slot * 52u;
}

void game_update_player(uint32 object)
{
    uint32 value, player, view, callback, difference, previous, index, selected, excluded;
    uint32 count, base, end, record, amount, rate, region, scale, state, target, fire;
    uint32 stationary, offset, elapsed;
    sint32 kind;
    FUNCTION_MARKER(0x80078F64, "GAME.OVL");
    if (r_u8(0x80011934u) != 0u) {
        player = r_u32(0x80040FDCu);
        w_u32(player + 0x238u, 10000u);
        w_u32(player + 0x234u, 10000u);
    }
    value = r_u32(0x800EC888u);
    if ((sint32)value > 0) {
        --value;
        w_u32(0x800EC888u, value);
        if (value == 0u) {
            view = r_u32(0x8003FE74u);
            w_u32(0x800EF4E4u, 0u);
            game_dispatch_destruction_shape(view);
            player = r_u32(0x80040FDCu);
            w_u32(0x800EC868u, 1u);
            w_u32(player + 0x9Cu, 180u);
            w_u32(player + 0x40u, 0x8007A8E4u);
        }
    }
    if (r_u32(0x800EC884u) != 0u)
        game_update_player_control_mode();
    callback = r_u32(0x800ECEACu);
    psx_dispatch_guest_call(callback, 1, object, 0u, 0u);
    if (r_u32(0x800EC868u) != 0u || r_s32(0x800EC888u) > 0 || r_u32(0x800EC884u) != 0u)
        return;
    stationary = 1u;
    for (offset = 8u; offset <= 16u; offset += 4u) {
        value = r_u32(object + offset);
        previous = r_u32(object + offset + 0x88u);
        difference = value - previous;
        if ((sint32)difference < 0)
            difference = 0u - difference;
        if ((sint32)difference >= 61) {
            stationary = 0u;
            break;
        }
    }
    if (stationary == 0u)
        w_u32(object + 0xD8u, 0u);
    else
        w_u32(object + 0xD8u, r_u32(object + 0xD8u) + 1u);
    value = r_u32(object + 0xDCu);
    if ((sint32)value > 0)
        w_u32(object + 0xDCu, value - 1u);
    do {
        if (r_u8(0x80040F2Fu) != 0u) {
            count = r_u32(0x80127038u);
            if ((sint32)count > 0) {
                value = r_u32(0x800EC874u);
                state = r_u32(0x800EF4A8u);
                selected = value + 1u;
                w_u32(0x800EC874u, selected);
                if (state != 2u)
                    w_u32(0x800EF4A8u, 2u);
                base = r_u32(0x8012703Cu);
                if ((sint32)selected >= (sint32)(base + count))
                    w_u32(0x800EC874u, base);
                sound_play_sample(18u);
                w_u32(0x800EF4DCu, 15u);
            }
        }
        selected = r_u32(0x800EC874u);
        excluded = r_u32(0x800ECBCCu);
    } while (selected == excluded && selected != 0u);
    if (r_u8(0x80040F31u) != 0u) {
        count = r_u32(0x80127040u);
        if (count != 0u) {
            state = r_u32(0x800EF4BCu);
            w_u32(0x800EF4E0u, 0u);
            if (state - 4u < 2u || state == 6u || r_u32(0x800EF4E4u) != 0u) {
                target = r_u32(0x800EF4E4u);
                w_u32(0x800EF4BCu, 3u);
                w_u32(0x800EC8A0u, 0u);
                if (target != 0u) {
                    value = r_u32(target + 0x114u);
                    if ((value & 0x40000u) != 0u)
                        w_u32(target + 0x114u, value ^ 0x40000u);
                    w_u32(0x800EF4E4u, 0u);
                }
            } else {
                base = r_u32(0x80127044u);
                selected = r_u32(0x800EC878u);
                end = base + count;
                do {
                    value = r_u32(0x800EC878u) + 1u;
                    w_u32(0x800EC878u, value);
                    if ((sint32)value >= (sint32)end)
                        w_u32(0x800EC878u, base);
                    index = r_u32(0x800EC878u);
                    if (r_u32(0x80119C58u + index * 52u) != 0u) {
                        sound_play_sample(18u);
                        w_u32(0x800EF4BCu, 2u);
                        break;
                    }
                } while (selected != index);
            }
        }
    }
    index = r_u32(0x8012703Cu);
    count = r_u32(0x80127038u);
    if ((sint32)index < (sint32)(index + count)) {
        record = 0x80119C30u + index * 52u;
        region = r_u32(0x800311C0u);
        scale = r_u32(0x800F2B68u);
        selected = r_u32(0x800EC874u);
        do {
            w_u32(record + 32u, r_u32(record + 32u) - 1u);
            if (region != 0u)
                w_u32(record + 32u, r_u32(record + 32u) - scale);
            if (r_s32(record + 32u) < 0)
                w_u32(record + 32u, 0u);
            if (index != selected) {
                amount = r_u32(record + 28u);
                if ((sint32)amount > 0) {
                    rate = r_u32(record + 16u);
                    amount -= rate;
                    w_u32(record + 28u, amount);
                    if (region != 0u && scale != 0u)
                        w_u32(record + 28u, amount - r_u32(record + 16u));
                }
                if (r_s32(record + 28u) < 0)
                    w_u32(record + 28u, 0u);
            }
            base = r_u32(0x8012703Cu);
            count = r_u32(0x80127038u);
            ++index;
            record += 52u;
        } while ((sint32)index < (sint32)(base + count));
    }
    fire = r_u8(0x80040F29u);
    if (fire == 0u)
        w_u32(0x800EF4DCu, 0u);
    value = r_u32(0x800EF4DCu);
    if ((sint32)value > 0)
        w_u32(0x800EF4DCu, value - 1u);
    if (r_s32(0x80127038u) > 0 && r_u32(0x800EF4DCu) == 0u) {
        if (fire != 0u && r_u32(0x800311DCu) != 1u) {
            selected = r_u32(0x800EC874u);
            callback = r_u32(0x80119C5Cu + selected * 52u);
            psx_dispatch_guest_call(callback, 1, selected, 0u, 0u);
        } else {
            selected = r_u32(0x800EC874u);
            record = 0x80119C30u + selected * 52u;
            if (r_u32(record + 24u) == 1u) {
                value = r_u32(record + 32u);
                w_u32(record + 24u, 0u);
                if (value == 0u) {
                    value = r_u32(record);
                    scale = r_u32(0x800F2B68u);
                    w_u32(record + 32u, value - scale);
                }
            } else {
                amount = r_u32(record + 28u);
                if ((sint32)amount > 0) {
                    rate = r_u32(record + 16u);
                    region = r_u32(0x800311C0u);
                    amount -= rate;
                    w_u32(record + 28u, amount);
                    if (region != 0u && r_u32(0x800F2B68u) != 0u)
                        w_u32(record + 28u, amount - r_u32(record + 16u));
                }
                selected = r_u32(0x800EC874u);
                record = 0x80119C30u + selected * 52u;
                if (r_s32(record + 28u) < 0)
                    w_u32(record + 28u, 0u);
            }
        }
    }
    selected = r_u32(0x800EC878u);
    record = 0x80119C30u + selected * 52u;
    value = r_u32(record + 32u);
    if ((sint32)value > 0)
        w_u32(record + 32u, value - 1u);
    if (r_s32(0x80127040u) <= 0)
        goto secondary_finished;
    if (r_u32(0x800311DCu) == 1u) {
        if (r_u8(0x80040F32u) != 0u) {
            selected = r_u32(0x800ECBC8u);
            if (r_u32(0x80119C58u + selected * 52u) != 0u) {
                game_fire_auxiliary_weapon(selected);
                if (r_u8(0x80011936u) == 0u) {
                    selected = r_u32(0x800ECBC8u);
                    record = 0x80119C30u + selected * 52u;
                    w_u32(record + 40u, r_u32(record + 40u) - 1u);
                }
            }
        }
        if (r_u8(0x80040F30u) != 0u)
            game_fire_weapon_profile_16(r_u32(0x800ECBCCu));
        goto secondary_finished;
    }
    if (r_u8(0x80040F32u) == 0u)
        goto secondary_finished;
    state = r_u32(0x800EF4BCu);
    if (state == 4u || state - 5u < 2u)
        goto charged_secondary;
    selected = r_u32(0x800EC878u);
    record = 0x80119C30u + selected * 52u;
    if (r_s16(record + 50u) == 0 && r_u32(record + 32u) == 0u) {
        if (r_s32(record + 40u) <= 0 || game_target_obstructed(r_u32(0x800EF4E0u)) != 0u)
            goto secondary_finished;
        selected = r_u32(0x800EC878u);
        callback = r_u32(0x80119C5Cu + selected * 52u);
        psx_dispatch_guest_call(callback, 1, selected, 0u, 0u);
        if (r_u8(0x80011936u) == 0u) {
            selected = r_u32(0x800EC878u);
            record = 0x80119C30u + selected * 52u;
            w_u32(record + 40u, r_u32(record + 40u) - 1u);
        }
        goto secondary_finished;
    }
    selected = r_u32(0x800EC878u);
    record = 0x80119C30u + selected * 52u;
    kind = r_s16(record + 50u);
    if ((kind == 1 || kind == 2) && r_s32(record + 40u) > 0) {
        w_u32(0x800EF4CCu, selected);
        w_u32(0x800EF4BCu, kind == 1 ? 4u : 6u);
        sound_play_sample(19u);
    }
    goto secondary_finished;

charged_secondary:
    if (r_u8(0x80040F32u) == 0u)
        goto secondary_finished;
    state = r_u32(0x800EF4BCu);
    fire = state == 5u;
    if (state == 6u) {
        value = r_u32(0x800EC8A0u);
        count = r_u32(0x800EC89Cu);
        fire = (sint32)count < (sint32)value;
    }
    if (fire != 0u) {
        selected = r_u32(0x800EC878u);
        w_u32(0x800EF4BCu, 3u);
        callback = r_u32(0x80119C5Cu + selected * 52u);
        goto fire_charged_secondary;
    }
    if (r_u8(0x80040F32u) == 0u)
        goto secondary_finished;
    state = r_u32(0x800EF4BCu);
    if (state != 4u)
        goto secondary_state_captured;
    selected = r_u32(0x800EF4CCu);
    if (r_s16(0x80119C60u + selected * 52u) == 3)
        goto secondary_finished;
    if (game_target_obstructed(r_u32(0x800EF4E0u)) != 0u)
        goto secondary_finished;
    selected = r_u32(0x800EC878u);
    callback = r_u32(0x80119C5Cu + selected * 52u);
    w_u32(0x800EF4BCu, 3u);
    w_u32(0x800EF4E0u, 0u);

fire_charged_secondary:
    psx_dispatch_guest_call(callback, 1, selected, 0u, 0u);
    if (r_u8(0x80011936u) == 0u) {
        selected = r_u32(0x800EC878u);
        record = 0x80119C30u + selected * 52u;
        w_u32(record + 40u, r_u32(record + 40u) - 1u);
    }
    w_u32(0x800EF48Cu, 512u);
    w_u32(0x800EC8A0u, 0u);

secondary_finished:
    state = r_u32(0x800EF4BCu);
secondary_state_captured:
    if (state == 6u)
        w_u32(0x800EC8A0u, r_u32(0x800EC8A0u) + 1u);
    else
        w_u32(0x800EC8A0u, 0u);
    value = r_u32(0x800EEB24u);
    if (value == 0u)
        return;
    if (r_u32(0x800595F8u) == 0x30007u) {
        elapsed = r_u32(0x800311B8u) - value;
        if (elapsed < 181u || r_u32(r_u32(0x80041004u)) != 0u)
            return;
        state = r_u32(0x800EEB28u);
        w_u32(0x800EC870u, 1u);
        w_u32(0x800EC86Cu, state == 1u ? state : 0u);
        (void)r_u32(0x80040FDCu);
        game_fail_current_mission();
    } else {
        elapsed = r_u32(0x800311B8u) - value;
        if (elapsed == 900u) {
            game_request_radio_message(36u, r_u32(0x800F23CCu));
        } else if (elapsed == 1800u) {
            (void)r_u32(0x80040FDCu);
            w_u32(0x800EC870u, 1u);
            w_u32(0x800EC86Cu, 1u);
            game_fail_current_mission();
        }
    }
}

uint32 game_target_obstructed(uint32 target)
{
    uint32 player, target_x, target_y, target_z, player_x, player_y, player_z, object, distance;
    sint32 target_distance;
    FUNCTION_MARKER(0x80079D30, "GAME.OVL");
    if (target == 0u)
        return 0u;
    player = r_u32(0x80040FDCu);
    target_x = r_u32(target + 8u);
    target_y = r_u32(target + 12u);
    player_x = r_u32(player + 8u);
    player_y = r_u32(player + 12u);
    target_z = r_u32(target + 16u);
    player_z = r_u32(player + 16u);
    target_distance = vector_length_approx((sint32)(target_x - player_x),
        (sint32)(target_y - player_y), (sint32)(target_z - player_z));
    object = object_lookup(1u);
    while (object != 0u) {
        if ((r_u32(object + 0x114u) & 0x10u) != 0u) {
            player = r_u32(0x80040FDCu);
            game_evaluate_target_occlusion(player, object, 0u);
            distance = r_u32(0x80046174u);
            if ((sint32)distance <= 199999 && (sint32)distance < target_distance
                && r_s32(0x80052E6Cu) < 560 && r_s32(0x80053674u) < 560) {
                sound_play_sample(17u);
                return 1u;
            }
        }
        object = r_u32(object + 0x1DCu);
    }
    return 0u;
}

sint32 game_request_radio_message(uint32 asset, uint32 text)
{
    uint32 record, sectors, data, length;
    const char *native_text = NULL;
    FUNCTION_MARKER(0x800877B4, "GAME.OVL");
    record = r_u32(0x800F2B08u) + asset * 24u;
    sectors = r_u32(record + 20u);
    data = r_u32(record + 28u);
    if (text != 0u) {
        length = 0u;
        while (r_u8(text + length) != 0u)
            ++length;
        native_text = (const char *)psx_addr(text, (size_t)length + 1u);
    }
    return stream_enqueue_segments(record + 16u, sectors, data, 0x800118FEu, 0u, native_text);
}

void game_fail_current_mission(void)
{
    uint32 count, player;
    sint32 group;
    FUNCTION_MARKER(0x8007A7CC, "GAME.OVL");
    if (r_s16(0x80059610u) != 8)
        w_u16(0x80059610u, 4u);
    w_u16(0x800595F6u, 1u);
    count = r_u16(0x80059624u);
    group = r_s16(0x800595F8u);
    w_u16(0x8005961Cu, 0u);
    w_u32(0x800595C8u, 1u);
    w_u16(0x80059624u, (uint16)(count + 1u));
    if (group == 1) {
        count = r_u16(0x80011902u) + 1u;
        w_u16(0x80011902u, (uint16)count);
        if ((sint16)count >= 2)
            w_u32(0x800EC86Cu, (uint32)group);
    }
    player = r_u32(0x80040FDCu);
    if (r_u32(player) == 102u) {
        w_u32(player + 0x44u, 0x8002493Cu);
        count = r_u32(0x80011924u) + 1u;
        w_u32(0x80011924u, count);
        game_remove_objects_by_type(104u);
        stream_clear_queue();
        message_init();
        game_launch_escape_pod_sequence();
        w_u32(0x800EC88Cu, 0u);
    }
}

uint32 game_remove_objects_by_type(uint32 type)
{
    uint32 object, next, object_type, callback;
    FUNCTION_MARKER(0x800ADC78, "GAME.OVL");
    object = r_u32(0x8005367Cu);
    while (object != 0u) {
        object_type = r_u32(object);
        next = r_u32(object + 0x1D4u);
        if (object_type == type) {
            callback = r_u32(object + 0x48u);
            psx_dispatch_guest_call(callback, 1u, object, 0u, 0u);
        }
        object = next;
    }
    return 0u;
}

void game_update_player_control_mode(void)
{
    uint32 mode;
    FUNCTION_MARKER(0x8007CCD4, "GAME.OVL");
    mode = r_u32(0x800EC884u);
    switch (mode) {
    case 1u:
        game_update_player_control_mode_1();
        break;
    case 2u:
        game_update_player_control_mode_2();
        break;
    case 3u:
        game_update_player_control_mode_3();
        break;
    case 4u:
        w_u16(0x800595F4u, 1u);
        game_update_player_control_mode_4();
        break;
    case 5u:
        w_u16(0x800595F4u, 1u);
        game_update_player_control_mode_5();
        break;
    default:
        break;
    }
    game_update_exit_request();
}

void game_update_player_control_mode_2(void)
{
    FUNCTION_MARKER(0x8007C724, "GAME.OVL");
    /* Original body is only jr ra and its nop delay */
}

void game_update_exit_request(void)
{
    uint32 start, current;
    FUNCTION_MARKER(0x8008776C, "GAME.OVL");
    start = r_u32(0x800EEF14u);
    current = r_u32(0x800311B8u);
    if (start + 150u < current && (r_u32(0x80053684u) & 0x840u) != 0u)
        w_u32(0x800ECEA8u, 3u);
}

VECTOR *game_apply_transposed_matrix_native(const MATRIX *matrix, VECTOR *input, VECTOR *output)
{
    MATRIX transposed;
    FUNCTION_MARKER(0x80097B60, "GAME.OVL");
    transposed.m[0][0] = matrix->m[0][0];
    transposed.m[0][1] = matrix->m[1][0];
    transposed.m[0][2] = matrix->m[2][0];
    transposed.m[1][0] = matrix->m[0][1];
    transposed.m[1][1] = matrix->m[1][1];
    transposed.m[1][2] = matrix->m[2][1];
    transposed.m[2][0] = matrix->m[0][2];
    transposed.m[2][1] = matrix->m[1][2];
    transposed.m[2][2] = matrix->m[2][2];
    return ApplyMatrixLV(&transposed, input, output);
}

void game_evaluate_target_occlusion(uint32 player, uint32 object, uint32 mode)
{
    VECTOR difference, transformed;
    uint32 x, y, z, other_x, other_y, other_z, distance, current, angle, vertical, depth, horizontal;
    FUNCTION_MARKER(0x80083780, "GAME.OVL");
    other_x = r_u32(object + 8u);
    x = r_u32(player + 8u);
    difference.vx = (sint32)(other_x - x);
    other_y = r_u32(object + 12u);
    y = r_u32(player + 12u);
    difference.vy = (sint32)(other_y - y);
    other_z = r_u32(object + 16u);
    z = r_u32(player + 16u);
    difference.vz = (sint32)(other_z - z);
    distance = (uint32)vector_length_approx(difference.vx, difference.vy, difference.vz);
    current = r_u32(0x80040FDCu);
    w_u32(0x80046174u, distance);
    if (player == current)
        ApplyMatrixLV((MATRIX *)psx_addr(player + 0x11Cu, 20u), &difference, &transformed);
    else
        game_apply_transposed_matrix_native((const MATRIX *)psx_addr(player + 0x11Cu, 18u), &difference, &transformed);
    angle = (uint32)ratan2(transformed.vx, transformed.vz);
    vertical = (uint32)transformed.vy;
    depth = (uint32)transformed.vz;
    w_u32(0x80033D1Cu, angle);
    w_u32(0x80033D18u, angle);
    if ((sint32)depth < 0)
        depth = 0u - depth;
    angle = (uint32)ratan2((sint32)(0u - vertical), (sint32)depth);
    w_u32(0x80033D30u, angle);
    w_u32(0x80033D20u, angle);
    if ((mode << 16) != 0u) {
        difference.vx = (sint32)(0u - (uint32)difference.vx);
        difference.vy = (sint32)(0u - (uint32)difference.vy);
        current = r_u32(0x80040FDCu);
        difference.vz = (sint32)(0u - (uint32)difference.vz);
        if (object == current)
            ApplyMatrixLV((MATRIX *)psx_addr(object + 0x11Cu, 20u), &difference, &transformed);
        else
            game_apply_transposed_matrix_native((const MATRIX *)psx_addr(object + 0x11Cu, 18u), &difference, &transformed);
        angle = (uint32)ratan2(transformed.vx, transformed.vz);
        vertical = (uint32)transformed.vy;
        depth = (uint32)transformed.vz;
        w_u32(0x80040C98u, angle);
        if ((sint32)depth < 0)
            depth = 0u - depth;
        angle = (uint32)ratan2((sint32)(0u - vertical), (sint32)depth);
        horizontal = r_u32(0x80040C98u);
        w_u32(0x80040F1Cu, angle);
        if ((sint32)angle < 0)
            angle = 0u - angle;
        w_u32(0x80033D34u, angle);
        if ((sint32)horizontal < 0)
            horizontal = 0u - horizontal;
        w_u32(0x80033D24u, horizontal);
    }
    horizontal = r_u32(0x80033D18u);
    vertical = r_u32(0x80033D20u);
    if ((sint32)horizontal < 0)
        horizontal = 0u - horizontal;
    if ((sint32)vertical < 0)
        vertical = 0u - vertical;
    w_u32(0x80052E6Cu, horizontal);
    w_u32(0x80053674u, vertical);
}

void game_update_player_control_mode_1(void)
{
    VECTOR up, translation;
    uint32 object, speed, px, py, pz, x, y, z, product_x, product_y, product_z, timer, state;
    sint32 increment;
    FUNCTION_MARKER(0x8007C2D4, "GAME.OVL");
    object = r_u32(0x8003FE74u);
    up.vx = r_s16(object + 0x11Eu);
    up.vy = r_s16(object + 0x124u);
    up.vz = r_s16(object + 0x12Au);
    object_basis_from_vectors((VECTOR *)psx_addr(0x8010AB70u, 12u), &up, object);
    x = r_u32(0x8010AB70u);
    speed = r_u32(0x800EC87Cu);
    product_x = x * speed;
    y = r_u32(0x8010AB74u);
    product_y = y * speed;
    z = r_u32(0x8010AB78u);
    product_z = z * speed;
    px = r_u32(0x800FB410u) + (uint32)((sint32)product_x >> 12);
    w_u32(0x800FB410u, px);
    py = r_u32(0x800FB414u) + (uint32)((sint32)product_y >> 12);
    w_u32(0x800FB414u, py);
    pz = r_u32(0x800FB418u);
    object = r_u32(0x8003FE74u);
    pz += (uint32)((sint32)product_z >> 12);
    w_u32(0x800FB418u, pz);
    w_u32(object + 8u, px);
    w_u32(object + 12u, r_u32(0x800FB414u));
    w_u32(object + 16u, r_u32(0x800FB418u));
    product_x = r_u32(0x8010AB70u) * speed;
    product_y = r_u32(0x8010AB74u) * speed;
    product_z = r_u32(0x8010AB78u) * speed;
    translation.vx = (sint32)product_x >> 13;
    translation.vy = (sint32)product_y >> 13;
    translation.vz = (sint32)product_z >> 13;
    camera_apply_translation_native(&translation);
    camera_set_up(0u - (uint32)up.vx, 0u - (uint32)up.vy, 0u - (uint32)up.vz);
    timer = r_u32(0x80108A40u) - 1u;
    w_u32(0x80108A40u, timer);
    if (timer == 75u)
        game_attach_object_trail(r_u32(0x8003FE74u), 2u);
    if (r_s32(0x80108A40u) <= 0) {
        state = r_u32(0x800F32C8u);
        w_u32(0x800ECEA8u, 3u);
        if (state == 0u)
            w_u32(0x800313DCu, 3u);
        if (state == 1u)
            w_u32(0x800313DCu, 4u);
    }
    speed = r_u32(0x800EC87Cu);
    increment = (sint32)speed >> 2;
    if (increment <= 0)
        increment = 1;
    speed += (uint32)increment;
    w_u32(0x800EC87Cu, speed);
    if ((sint32)speed >= 2049)
        w_u32(0x800EC87Cu, 2048u);
}

void game_update_player_control_mode_3(void)
{
    VECTOR up;
    uint32 x, y, z, speed, lateral, product_x, product_y, product_z, lateral_x, lateral_y, lateral_z;
    uint32 object, px, py, pz, timer, state;
    FUNCTION_MARKER(0x8007C504, "GAME.OVL");
    x = r_u32(0x8010AB70u);
    speed = r_u32(0x800EC87Cu);
    product_x = x * speed;
    y = r_u32(0x8010AB74u);
    product_y = y * speed;
    z = r_u32(0x8010AB78u);
    product_z = z * speed;
    lateral = r_u32(0x800EC880u);
    lateral_x = x * lateral;
    object = r_u32(0x8003FE74u);
    up.vx = r_s16(object + 0x11Eu);
    up.vy = r_s16(object + 0x124u);
    up.vz = r_s16(object + 0x12Au);
    lateral_y = y * lateral;
    px = r_u32(0x800FB410u) + (uint32)((sint32)product_x >> 12);
    w_u32(0x800FB410u, px);
    py = r_u32(0x800FB414u) + (uint32)((sint32)product_y >> 12);
    w_u32(0x800FB414u, py);
    pz = r_u32(0x800FB418u) + (uint32)((sint32)product_z >> 12);
    lateral_z = z * lateral;
    w_u32(0x800FB418u, pz);
    x = r_u32(0x8010AB58u) + (uint32)((sint32)lateral_x >> 12);
    w_u32(0x8010AB58u, x);
    y = r_u32(0x8010AB5Cu) + (uint32)((sint32)lateral_y >> 12);
    w_u32(0x8010AB5Cu, y);
    z = r_u32(0x8010AB60u) + (uint32)((sint32)lateral_z >> 12);
    w_u32(0x8010AB60u, z);
    w_u32(object + 8u, px);
    py = r_u32(0x800FB414u);
    w_u32(0x800EC87Cu, speed + 5u);
    w_u32(object + 12u, py);
    timer = r_u32(0x80108A40u);
    pz = r_u32(0x800FB418u);
    w_u32(object + 16u, pz);
    if ((sint32)timer >= 361) {
        w_u32(0x800EC880u, lateral + 5u);
    } else if (r_u32(0x800311C0u) != 0u) {
        if ((sint32)lateral > 0)
            w_u32(0x800EC880u, lateral - 3u);
    } else if ((sint32)lateral > 0) {
        w_u32(0x800EC880u, lateral - 5u);
    }
    timer = r_u32(0x80108A40u) - 1u;
    w_u32(0x80108A40u, timer);
    camera_set_up(0u - (uint32)up.vx, 0u - (uint32)up.vy, 0u - (uint32)up.vz);
    if (r_s32(0x80108A40u) <= 0) {
        state = r_u32(0x800F32C8u);
        w_u32(0x800ECEA8u, 3u);
        if (state == 0u)
            w_u32(0x800313DCu, 3u);
        if (state == 1u)
            w_u32(0x800313DCu, 4u);
    }
}

void game_update_player_control_mode_4(void)
{
    VECTOR direction;
    uint32 object, target, x, y, z, dx, dy, dz, ax, ay, az, count, value;
    FUNCTION_MARKER(0x8007C72C, "GAME.OVL");
    object = r_u32(0x80123F44u);
    x = r_u32(object + 8u);
    w_u32(object + 0x234u, 255u);
    w_u32(0x80118A70u, x - 20000u);
    w_u32(0x80118A74u, r_u32(object + 12u) - 20000u);
    z = r_u32(object + 16u);
    target = r_u32(0x80123F68u);
    w_u32(0x80118A78u, z - 20000u);
    x = r_u32(target + 8u);
    dx = x - r_u32(object + 8u);
    y = r_u32(target + 12u);
    y -= r_u32(object + 12u);
    dy = y;
    z = r_u32(target + 16u);
    dz = z - r_u32(object + 16u);
    for (;;) {
        ax = (sint32)dx < 0 ? 0u - dx : dx;
        ay = (sint32)dy < 0 ? 0u - dy : dy;
        az = (sint32)dz < 0 ? 0u - dz : dz;
        if ((sint32)ax < 16384 && (sint32)ay < 16384 && (sint32)az < 16384)
            break;
        dx = (uint32)((sint32)dx >> 1);
        dy = (uint32)((sint32)dy >> 1);
        dz = (uint32)((sint32)dz >> 1);
    }
    direction.vx = (sint32)dx;
    direction.vy = (sint32)dy;
    direction.vz = (sint32)dz;
    vector_normalize(&direction, &direction);
    x = r_u32(0x8010AB58u);
    y = (uint32)direction.vy;
    x += (uint32)(direction.vx >> 5);
    w_u32(0x8010AB58u, x);
    y = (uint32)((sint32)y >> 5);
    value = r_u32(0x8010AB5Cu);
    z = (uint32)direction.vz;
    count = r_u32(0x800ECDB8u);
    w_u32(0x8010AB5Cu, value + y);
    value = r_u32(0x8010AB60u) + (uint32)((sint32)z >> 5);
    w_u32(0x8010AB60u, value);
    if ((sint32)count > 0) {
        ++count;
        w_u32(0x800ECDB8u, count);
        game_process_camera_vector(0x800ECDA8u, count);
        value = r_u32(0x800ECDB8u) << 5;
        if ((sint32)value >= 20001)
            w_u32(0x800ECEA8u, 3u);
    }
    camera_set_position(0x80118A70u);
    camera_set_up(0u, 0xFFFFF000u, 0u);
}

void game_update_player_control_mode_5(void)
{
    VECTOR direction;
    uint32 object, target, x, y, z, dx, dy, dz, ax, ay, az, count, value, random, next, component;
    FUNCTION_MARKER(0x8007C938, "GAME.OVL");
    object = r_u32(0x80123F44u);
    x = r_u32(object + 8u);
    w_u32(object + 0x234u, 255u);
    w_u32(0x80118A70u, x - 100000u);
    w_u32(0x80118A74u, r_u32(object + 12u) - 100000u);
    z = r_u32(object + 16u);
    target = r_u32(0x80123F68u);
    w_u32(0x80118A78u, z - 100000u);
    x = r_u32(target + 8u);
    dx = x - r_u32(object + 8u);
    y = r_u32(target + 12u);
    dy = y - r_u32(object + 12u);
    count = r_u32(0x800ECDBCu);
    z = r_u32(target + 16u);
    dz = z - r_u32(object + 16u);
    ++count;
    w_u32(0x800ECDBCu, count);
    if ((sint32)count < 150) {
        for (;;) {
            ax = (sint32)dx < 0 ? 0u - dx : dx;
            ay = (sint32)dy < 0 ? 0u - dy : dy;
            az = (sint32)dz < 0 ? 0u - dz : dz;
            if ((sint32)ax < 16384 && (sint32)ay < 16384 && (sint32)az < 16384)
                break;
            dx = (uint32)((sint32)dx >> 1);
            dy = (uint32)((sint32)dy >> 1);
            dz = (uint32)((sint32)dz >> 1);
        }
        direction.vx = (sint32)dx;
        direction.vy = (sint32)dy;
        direction.vz = (sint32)dz;
        vector_normalize(&direction, &direction);
        w_u32(0x80034D6Cu, 0u);
        x = (uint32)direction.vx;
        w_u32(0x80034D70u, 0u);
        w_u32(0x80034D74u, 0u);
        value = r_u32(0x8010AB58u);
        y = (uint32)direction.vy;
        w_u32(0x8010AB58u, value + (x << 1));
        value = r_u32(0x8010AB5Cu);
        z = (uint32)direction.vz;
        w_u32(0x8010AB5Cu, value + (y << 1));
        value = r_u32(0x8010AB60u);
        random = r_u32(0x800EF50Cu);
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        w_u32(0x8010AB60u, value + (z << 1));
        direction.vx = (sint32)((next & 1023u) - 512u);
        w_u32(0x800EF50Cu, next);
        random = (uint32)((sint32)next >> 1) ^ (next << 3);
        direction.vy = (sint32)((random & 1023u) - 512u);
        w_u32(0x800EF50Cu, random);
        next = (uint32)((sint32)random >> 1) ^ (random << 3);
        w_u32(0x800EF50Cu, next);
        direction.vz = (sint32)((next & 1023u) - 512u);
        SetRotMatrix((MATRIX *)psx_addr(0x80034D58u, 20u));
        SetTransMatrix((MATRIX *)psx_addr(0x80034D58u, sizeof(MATRIX)));
        x = r_u32(0x8010AB58u);
        y = r_u32(0x8010AB5Cu);
        z = r_u32(0x8010AB60u);
        game_emit_radial_effect(x, y, z, 10000u, 0x80E080u);
        game_emit_vector_effect_native(0x8010AB58u, &direction, 0x206020u, 9000u);
    } else {
        w_u32(0x800ECDB8u, r_u32(0x800ECDB8u) + 1u);
    }
    if (r_u32(0x800ECDBCu) == 150u) {
        sound_play_sample_pitch(4u, 128u);
        game_set_screen_tint(40u, 128u, 40u);
    }
    object = r_u32(0x80040C94u);
    value = r_u32(0x8010AB58u);
    component = r_u32(object + 8u);
    (void)((sint32)(value - component) >> 2);
    value = r_u32(0x8010AB5Cu);
    component = r_u32(object + 12u);
    (void)((sint32)(value - component) >> 2);
    value = r_u32(0x8010AB60u);
    component = r_u32(object + 16u);
    count = r_u32(0x800ECDB8u);
    (void)((sint32)(value - component) >> 2);
    if ((sint32)count > 0 && (sint32)count >= 251)
        w_u32(0x800ECEA8u, 3u);
    camera_set_position(0x80118A70u);
    camera_set_up(0u, 0xFFFFF000u, 0u);
}
