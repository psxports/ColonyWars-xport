#include "game_functions.h"
#include <string.h>

enum { GAME_SAVE_PAYLOAD = 0x207, GAME_SAVE_HEADER = 16, GAME_SAVE_BYTES = GAME_SAVE_HEADER + GAME_SAVE_PAYLOAD };

typedef struct GAME_SAVE_SPAN
{
    uint32 offset;
    uint32 address;
    uint32 bytes;
} GAME_SAVE_SPAN;

/* Preserve the original settings layout without its memory-card header or password */
static const GAME_SAVE_SPAN save_spans[] = {
    {0, 0x800595F8, 8}, {8, 0x8005960C, 4},
    {12, 0x800118E0, 80}, {92, 0x80011930, 4},
    {96, 0x800421C0, 240}, {0x150, 0x800311C0, 4},
    {0x154, 0x8001056C, 20}, {0x168, 0x80010490, 133},
    {0x1ED, 0x80010478, 19}, {0x200, 0x80010F5C, 7}
};
static uint8 startup_save[GAME_SAVE_BYTES];
static uint8 saved_payload[GAME_SAVE_PAYLOAD];
static int startup_save_ready;
static int saved_payload_known;

static uint32 game_save_word(const uint8 *bytes)
{
    return (uint32)bytes[0] | ((uint32)bytes[1] << 8) | ((uint32)bytes[2] << 16) | ((uint32)bytes[3] << 24);
}

static void game_save_put_word(uint8 *bytes, uint32 value)
{
    uint32 index;
    for (index = 0; index < 4; ++index)
        bytes[index] = (uint8)(value >> (index * 8));
}

static uint32 game_save_checksum(const uint8 *bytes)
{
    uint32 value = 2166136261u;
    uint32 index;
    for (index = 0; index < GAME_SAVE_PAYLOAD; ++index)
        value = (value ^ bytes[index]) * 16777619;
    return value;
}

static void game_save_capture(uint8 *payload)
{
    uint32 span, byte, chapter;
    memset(payload, 0, GAME_SAVE_PAYLOAD);
    for (span = 0; span < sizeof(save_spans) / sizeof(save_spans[0]); ++span)
        for (byte = 0; byte < save_spans[span].bytes; ++byte)
            payload[save_spans[span].offset + byte] = r_u8(save_spans[span].address + byte);
    chapter = (uint32)payload[0] | ((uint32)payload[1] << 8);
    if (chapter == 0 || chapter == 20 || chapter == 90) {
        /* Training and demo selections must not replace the campaign checkpoint */
        if (saved_payload_known)
            memcpy(payload, saved_payload, 12);
        else {
            memcpy(payload, payload + 8, 4);
            memcpy(payload + 4, payload + 8, 4);
        }
    }
}

static int game_save_valid(const uint8 *record, sint32 bytes)
{
    uint32 index, chapter, mission;
    const uint8 *payload = record + GAME_SAVE_HEADER;
    if (bytes != GAME_SAVE_BYTES || memcmp(record, "CWSV", 4) != 0 ||
        game_save_word(record + 4) != 1 || game_save_word(record + 8) != GAME_SAVE_PAYLOAD ||
        game_save_word(record + 12) != game_save_checksum(payload))
        return 0;
    for (index = 0; index < 12; index += 4) {
        chapter = (uint32)payload[index] | ((uint32)payload[index + 1] << 8);
        mission = (uint32)payload[index + 2] | ((uint32)payload[index + 3] << 8);
        if (chapter > 96 || mission > 7)
            return 0;
    }
    return game_save_word(payload + 0x150) <= 1 && game_save_word(payload + 0x164) < 5;
}

void game_load_startup_save(void)
{
    sint32 bytes;
    startup_save_ready = 0;
    saved_payload_known = 0;
    bytes = xport_load(startup_save, sizeof(startup_save));
    if (game_save_valid(startup_save, bytes))
        startup_save_ready = 1;
}

int game_restore_startup_save(void)
{
    uint32 byte;
    if (!startup_save_ready)
        return 0;
    startup_save_ready = 0;
    for (byte = 0; byte < GAME_SAVE_PAYLOAD; ++byte)
        w_u8(0x80010258 + byte, startup_save[GAME_SAVE_HEADER + byte]);
    front_restore_settings(0x80010258);
    game_save_capture(saved_payload);
    saved_payload_known = 1;
    return 1;
}

static sint32 game_save_commit(const uint8 *payload)
{
    uint8 record[GAME_SAVE_BYTES];
    sint32 result;
    if (!saved_payload_known) {
        memcpy(saved_payload, payload, GAME_SAVE_PAYLOAD);
        saved_payload_known = 1;
        return 0;
    }
    if (memcmp(saved_payload, payload, GAME_SAVE_PAYLOAD) == 0)
        return 0;
    memcpy(record, "CWSV", 4);
    game_save_put_word(record + 4, 1);
    game_save_put_word(record + 8, GAME_SAVE_PAYLOAD);
    game_save_put_word(record + 12, game_save_checksum(payload));
    memcpy(record + GAME_SAVE_HEADER, payload, GAME_SAVE_PAYLOAD);
    result = xport_save(record, sizeof(record));
    if (result == 1)
        memcpy(saved_payload, payload, GAME_SAVE_PAYLOAD);
    return result;
}

sint32 game_autosave(void)
{
    uint8 payload[GAME_SAVE_PAYLOAD];
    game_save_capture(payload);
    return game_save_commit(payload);
}

sint32 game_autosave_options(void)
{
    uint8 payload[GAME_SAVE_PAYLOAD];
    uint32 byte;
    if (!saved_payload_known)
        return 0;
    memcpy(payload, saved_payload, sizeof(payload));
    /* Pause options preserve the last committed campaign and mission statistics */
    for (byte = 0; byte < 36; ++byte)
        payload[12 + byte] = r_u8(0x800118E0 + byte);
    for (byte = 0; byte < 240; ++byte)
        payload[96 + byte] = r_u8(0x800421C0 + byte);
    for (byte = 0; byte < 20; ++byte)
        payload[0x154 + byte] = r_u8(0x8001056C + byte);
    return game_save_commit(payload);
}

sint32 game_frontend_route_screen(sint32 screen)
{
    if (screen >= 8 && screen <= 16)
        return 1;
    if (screen == 42)
        return 38;
    return screen;
}

void game_frontend_menu_ready(void)
{
    sint32 screen = r_s16(0x80114264);
    uint32 hidden = screen == 1 ? 34 : screen == 38 ? 32 : 0;
    uint32 pane, node, action;
    if (hidden != 0) {
        w_u8(0x800F2F89 + hidden, 2);
        if (r_u16(0x8010D0DC) == hidden)
            w_u16(0x8010D0DC, 31);
    }
    if (screen == 1) {
        pane = r_u32(0x800FB110);
        while (pane != 0) {
            node = r_u32(pane + 4);
            while (node != 0) {
                action = r_u16(node + 0x16);
                if (action == 35 || action == 36)
                    w_u16(node + 0x1A, r_u16(node + 0x1A) - 30);
                node = r_u32(node + 4);
            }
            pane = r_u32(pane + 0x10);
        }
    }
}


void game_wait_hangar_departure(void)
{
    uint32 previous, current;
    uint32 minimum = r_u32(0x800311C0) ? 300 : 360;
    if (r_u32(0x80046170) != 0x800145FC)
        return;
    previous = (uint32)pad_update(0);
    while (r_s32(0x800310B0) < (sint32)minimum) {
        VSync(0);
        current = (uint32)pad_update(0);
        if ((current & ~previous & (0x40 | 0x800)) != 0)
            break;
        previous = current;
    }
}
