#include <string.h>
#include <errno.h>

#include "game_functions.h"
#include "native_dispatch.h"

sint32 heap_init(void)
{
    uint32 base;
    uint32 header;
    FUNCTION_MARKER(0x80029B54, "SLUS_005.43");
    base = r_u32(0x80031154);
    header = ((0x801FF5F8 - base) & 0xFFFFFFFC) | 1;
    w_u32(0x80040F4C, base);
    w_u32(base, header);
    w_u32(base + (header & 0xFFFFFFFC) + 4, 0xFFFFFFFE);
    return -2;
}

uint32 heap_free_bytes(void)
{
    uint32 block;
    uint32 header;
    uint32 total = 0;
    FUNCTION_MARKER(0x80029E00, "SLUS_005.43");
    block = r_u32(0x80040F4C);
    header = r_u32(block);
    while ((sint32)header > 0)
    {
        if (header & 1)
            total = total - 1 + header;
        block += (header & 0xFFFFFFFC) + 4;
        header = r_u32(block);
    }
    return total;
}

uint32 heap_alloc_front(uint32 size)
{
    uint32 best = 0;
    sint32 best_header = 0x7FFFFFFF;
    uint32 block;
    uint32 header;
    uint32 needed;
    FUNCTION_MARKER(0x80029C40, "SLUS_005.43");
    size = (size + 3) & 0xFFFFFFFC;
    block = r_u32(0x80040F4C);
    needed = size + 1;
    w_u32(0x800313B4, r_u32(0x800313B4) + 1);
    for (;;)
    {
        header = r_u32(block);
        if ((sint32)header <= 0)
            break;
        if (header & 1)
        {
            if (header == needed)
            {
                w_u32(block, size);
                return block + 4;
            }
            if ((sint32)needed < (sint32)header && (sint32)header < best_header)
            {
                best = block;
                best_header = (sint32)header;
            }
        }
        block += (header & 0xFFFFFFFC) + 4;
    }
    if (best)
    {
        w_u32(best, size);
        w_u32(best + size + 4, (uint32)best_header - size - 4);
        return best + 4;
    }
    heap_report_failure(0);
    return 0;
}

uint32 heap_alloc_back(uint32 size)
{
    uint32 best = 0;
    sint32 best_header = 0x7FFFFFFF;
    uint32 block;
    uint32 header;
    uint32 needed;
    uint32 remainder;
    FUNCTION_MARKER(0x80029D1C, "SLUS_005.43");
    size = (size + 3) & 0xFFFFFFFC;
    block = r_u32(0x80040F4C);
    needed = size + 1;
    w_u32(0x800313B4, r_u32(0x800313B4) + 1);
    for (;;)
    {
        header = r_u32(block);
        if ((sint32)header <= 0)
            break;
        if (header & 1)
        {
            if (header == needed)
            {
                w_u32(block, size);
                return block + 4;
            }
            if ((sint32)needed < (sint32)header && (sint32)header < best_header)
            {
                best = block;
                best_header = (sint32)header;
            }
        }
        block += (header & 0xFFFFFFFC) + 4;
    }
    if (best)
    {
        remainder = (uint32)best_header - size - 4;
        w_u32(best, remainder);
        block = best + (remainder & 0xFFFFFFFC) + 4;
        w_u32(block, size);
        return block + 4;
    }
    heap_report_failure(0);
    return 0;
}

void heap_free(uint32 address)
{
    uint32 prev = 0;
    uint32 block;
    uint32 header;
    FUNCTION_MARKER(0x80029B9C, "SLUS_005.43");
    if (!address)
        return;
    w_u32(address - 4, r_u32(address - 4) | 1);
    block = r_u32(0x80040F4C);
    w_u32(0x800313B4, r_u32(0x800313B4) - 1);
    header = r_u32(block);
    while ((sint32)header > 0)
    {
        if (header & 1)
        {
            if (prev)
            {
                header += r_u32(prev) + 3;
                block = prev;
                w_u32(prev, header);
            }
            else
                prev = block;
        }
        else
            prev = 0;
        block += (header & 0xFFFFFFFC) + 4;
        header = r_u32(block);
    }
}

void heap_report_failure(uint32 size)
{
    FUNCTION_MARKER(0x80029B18, "SLUS_005.43");
    if (size + 1 < 2)
        diagnostic_report(200, 0x80011E6C, 0x80011E80);
}

void diagnostic_report(uint32 code, intptr message, intptr detail)
{
    FUNCTION_MARKER(0x8001F9A4, "SLUS_005.43");
    /* Original release body only spills arguments and returns */
}

uint32 data_swap_endian(uint32 value)
{
    FUNCTION_MARKER(0x80026500, "SLUS_005.43");
    return (value >> 24) | ((value >> 8) & 0xFF00) |
        ((value & 0xFF00) << 8) | (value << 24);
}

sint32 sound_set_mode(sint32 mode)
{
    FUNCTION_MARKER(0x80027624, "SLUS_005.43");
    if (mode == 2 || mode == 1)
    {
        w_u32(0x800313A0, 0);
        return 1;
    }
    if (mode < 3)
        return 1;
    if (mode == 4)
        w_u32(0x800313A0, 2);
    return 4;
}

sint32 sound_resume(void)
{
    FUNCTION_MARKER(0x80024C98, "SLUS_005.43");
    if (r_u8(0x80031258))
        w_u8(0x8003125C, 1);
    return 1;
}

uint32 sound_set_master(uint32 volume)
{
    uint32 enabled;
    FUNCTION_MARKER(0x80024BF0, "SLUS_005.43");
    enabled = r_u8(0x80031258);
    if (enabled)
        w_u8(0x8003125B, volume);
    return enabled;
}

sint32 sound_enable_sample(uint32 index)
{
    FUNCTION_MARKER(0x800267A0, "SLUS_005.43");
    w_u8(0x80033CF4 + index, 1);
    return 1;
}

void sound_set_level(uint32 level)
{
    FUNCTION_MARKER(0x80027604, "SLUS_005.43");
    w_u16(0x800118FC, level);
    w_u32(0x80041F48, (sint32)(level << 16) >> 18);
}

sint32 resource_clear_flags(void)
{
    uint32 entry;
    sint32 index;
    FUNCTION_MARKER(0x800101D0, "SLUS_005.43");
    for (entry = 0; entry < 19; ++entry)
        for (index = 6; index >= 0; --index)
            w_u8(0x80010490 + entry * 7 + (uint32)index, 0);
    return 0;
}

uint32 game_init_defaults(void)
{
    uint32 word_a;
    uint32 word_b;
    uint32 result;
    FUNCTION_MARKER(0x80010030, "SLUS_005.43");
    word_a = r_u8(0x80010000);
    w_u16(0x800595F8, 0);
    w_u16(0x800595FC, 0);
    w_u16(0x800595FA, 0);
    w_u16(0x800595FE, 0);
    w_u16(0x8005960C, 0);
    w_u16(0x8005960E, 0);
    w_u8(0x80010518, word_a);

    /* Original SWL/SWR pairs merge into aligned destination words */
    word_a = r_u32(0x80010004);
    word_b = r_u32(0x80010008);
    w_u32(0x80010528, (r_u32(0x80010528) & 0xFF000000) | (word_a >> 8));
    w_u32(0x80010524, (r_u32(0x80010524) & 0x00FFFFFF) | (word_a << 24));
    w_u32(0x8001052C, (r_u32(0x8001052C) & 0xFF000000) | (word_b >> 8));
    w_u32(0x80010528, (r_u32(0x80010528) & 0x00FFFFFF) | (word_b << 24));

    word_a = r_u32(0x8001000C);
    word_b = r_u32(0x80010010);
    w_u32(0x80010538, (r_u32(0x80010538) & 0xFFFF0000) | (word_a >> 16));
    w_u32(0x80010534, (r_u32(0x80010534) & 0x0000FFFF) | (word_a << 16));
    w_u32(0x8001053C, (r_u32(0x8001053C) & 0xFFFF0000) | (word_b >> 16));
    w_u32(0x80010538, (r_u32(0x80010538) & 0x0000FFFF) | (word_b << 16));

    word_a = r_u32(0x80010014);
    word_b = r_u32(0x80010018);
    w_u32(0x80010548, (r_u32(0x80010548) & 0xFFFFFF00) | (word_a >> 24));
    w_u32(0x80010544, (r_u32(0x80010544) & 0x000000FF) | (word_a << 8));
    w_u32(0x8001054C, (r_u32(0x8001054C) & 0xFFFFFF00) | (word_b >> 24));
    w_u32(0x80010548, (r_u32(0x80010548) & 0x000000FF) | (word_b << 8));
    word_a = r_u32(0x8001001C);
    w_u32(0x80010550, (r_u32(0x80010550) & 0xFFFFFF00) | (word_a >> 24));
    w_u32(0x8001054C, (r_u32(0x8001054C) & 0x000000FF) | (word_a << 8));

    word_a = r_u32(0x80010020);
    word_b = r_u32(0x80010024);
    w_u32(0x80010554, word_a);
    w_u32(0x80010558, word_b);
    word_a = r_u32(0x80010028);
    word_b = (uint32)r_s8(0x8001002C);
    w_u32(0x8001055C, word_a);
    w_u8(0x80010560, word_b);

    result = r_u32(0x800311C0);
    w_u32(0x8001056C, 8);
    w_u32(0x80010570, 20);
    w_u32(0x80010574, 0);
    w_u32(0x80010578, 0);
    if (result)
    {
        w_u32(0x80010564, 8);
        w_u32(0x80010568, 20);
    }
    else
    {
        w_u32(0x80010564, 0);
        w_u32(0x80010568, 0);
    }
    w_u32(0x8001057C, 0);
    return result;
}

sint32 game_init_services(void)
{
    FUNCTION_MARKER(0x8001F440, "SLUS_005.43");
    ResetCallback();
    InitGeom();
    CdInit();
    game_init_defaults();
    return heap_init();
}

uint32 resource_load_file(const char *name)
{
    FUNCTION_MARKER(0x800216A4, "SLUS_005.43");
    return disc_load_file(name);
}

sint32 disc_check_lid(void)
{
    FUNCTION_MARKER(0x80028440, "SLUS_005.43");
    /* Extracted-file access has no physical drive lid */
    return 0;
}

sint32 sound_stop_all(void)
{
    uint32 offset;
    FUNCTION_MARKER(0x80024CBC, "SLUS_005.43");
    if (!r_u8(0x80031258))
        return 0;
    SpuSetKey(0, 0xFFFFFF);
    for (offset = 0; offset < 24 * 28; offset += 28)
    {
        w_u32(0x800336F0 + offset, 0);
        w_u32(0x800336F4 + offset, 0);
        w_u32(0x80033994 + offset, 0);
        w_u8(0x800339A4 + offset, 255);
        w_u8(0x800339A5 + offset, 255);
    }
    w_u8(0x8003125C, 0);
    return 0;
}

sint32 disc_read_sectors(const CdlLOC *position, uint32 sectors, uint32 destination)
{
    sint32 done = 0;
    sint32 attempt;
    sint32 remaining;
    sint32 ticks;
    FUNCTION_MARKER(0x80020FD8, "SLUS_005.43");
    while (!done)
    {
        attempt = 0;
        while (!done && attempt < 15)
        {
            ticks = 0;
            if (CdControlB(2, (uint8 *)position, 0))
            {
                VSync(0);
                if (CdRead((sint32)sectors, (uint32 *)psx_addr(destination, (size_t)sectors * 2048), 128))
                {
                    VSync(0);
                    remaining = (sint32)sectors;
                    while ((uint32)remaining == sectors && ticks < 1750)
                    {
                        remaining = CdReadSync(1, 0);
                        ++ticks;
                        VSync(0);
                        if (ticks == 1750)
                        {
                            CdControlF(1, 0);
                            if (!(CdStatus() & 0x10))
                                ticks = 0;
                        }
                    }
                    if ((uint32)remaining < sectors)
                        ticks = 0;
                    while (remaining > 0 && ticks < 300)
                    {
                        remaining = CdReadSync(1, 0);
                        VSync(0);
                        if ((uint32)remaining == sectors)
                            ++ticks;
                    }
                    if (!remaining)
                        done = 1;
                }
            }
            if (!done)
                ++attempt;
        }
        if (!done)
            disc_read_error();
    }
    return done;
}

sint32 disc_read_error(void)
{
    FUNCTION_MARKER(0x80028C54, "SLUS_005.43");
    if (!disc_check_lid())
        w_u32(0x80040FE8, 21);
    return disc_prompt();
}

sint32 overlay_run(sint32 (*entry)(void), uint32 allocation)
{
    sint16 result;
    FUNCTION_MARKER(0x8001F8BC, "SLUS_005.43");
    w_u32(0x8004EAB8, heap_free_bytes());
    w_u32(0x8004EAC0, r_u32(0x800313B4));
    result = (sint16)entry();
    w_u32(0x8004EABC, heap_free_bytes());
    w_u32(0x8004EAC4, r_u32(0x800313B4));
    heap_free(allocation);
    return result;
}

uint32 overlay_load(const char *name, sint32 load_scene)
{
    char path[32];
    uint32 free_before;
    uint32 count_before;
    uint32 allocation;
    FUNCTION_MARKER(0x8001F4DC, "SLUS_005.43");
    strcpy(path, name);
    strcat(path, ".OVL");
    free_before = heap_free_bytes();
    count_before = r_u32(0x800313B4);
    if (load_scene)
        level_load_callback(r_s16(0x8001192E) >= 9 ? 10 : 0);
    w_u32(0x8004EAC8, free_before - heap_free_bytes());
    w_u32(0x8004EACC, r_u32(0x800313B4) - count_before);
    allocation = resource_load_file(path);
    if (allocation != 0)
        game_select_native_overlay(name);
    return allocation;
}

char *resource_make_path(char *destination, const char *name)
{
    FUNCTION_MARKER(0x80020F5C, "SLUS_005.43");
    if (*name != '\\')
        *destination++ = '\\';
    return strcpy(destination, name);
}

sint32 pad_init_config(void)
{
    uint32 index = 0;
    uint32 base = 0x800421C0;
    uint32 type;
    FUNCTION_MARKER(0x80029EE4, "SLUS_005.43");
    memset(psx_addr(base, 240), 0, 240);
    while (r_u8(0x800313BC + index) != 255)
    {
        type = r_u8(0x800313BC + index);
        if (type == 'S')
        {
            w_u16(base, 0);
            w_u16(base + 2, 0);
            w_u16(base + 4, 0);
            w_u16(base + 6, 0);
            w_u16(base + 8, 0x8000);
            w_u16(base + 10, 0x2000);
            w_u16(base + 12, 2);
            w_u16(base + 14, 8);
            w_u16(base + 18, 128);
            w_u16(base + 22, 64);
            w_u16(base + 16, 16);
            w_u16(base + 20, 32);
            w_u16(base + 24, 0x5000);
            w_u16(base + 26, 0x5000);
            w_u16(base + 28, 0);
            w_u8(base + 30, 2);
            w_u8(base + 34, 1);
            w_u8(base + 38, 0);
            w_u8(base + 42, 0);
            w_u8(base + 43, 127);
            w_u8(base + 39, 127);
            w_u8(base + 35, 127);
            w_u8(base + 31, 127);
            w_u8(base + 44, 16);
            w_u8(base + 40, 16);
            w_u8(base + 36, 16);
            w_u8(base + 32, 16);
            w_u8(base + 45, 127);
            w_u8(base + 41, 127);
            w_u8(base + 37, 127);
            w_u8(base + 33, 127);
        }
        else if (type == 'A')
        {
            w_u16(base, 0x8000);
            w_u16(base + 2, 0x2000);
            w_u16(base + 4, 0x1000);
            w_u16(base + 6, 0x4000);
            w_u16(base + 8, 1);
            w_u16(base + 10, 2);
            w_u16(base + 12, 4);
            w_u16(base + 14, 8);
            w_u16(base + 18, 64);
            w_u16(base + 22, 32);
            w_u16(base + 16, 128);
            w_u16(base + 20, 16);
            w_u16(base + 24, 1);
            w_u16(base + 26, 2);
            w_u16(base + 28, 0);
        }
        else if (type == 's')
        {
            w_u16(base, 0);
            w_u16(base + 2, 0);
            w_u16(base + 4, 0);
            w_u16(base + 6, 0);
            w_u16(base + 8, 1);
            w_u16(base + 10, 2);
            w_u16(base + 12, 4);
            w_u16(base + 14, 8);
            w_u16(base + 16, 32);
            w_u16(base + 18, 64);
            w_u16(base + 20, 16);
            w_u16(base + 22, 128);
            w_u16(base + 24, 1);
            w_u16(base + 26, 2);
            w_u16(base + 28, 0);
            w_u8(base + 30, 0);
            w_u8(base + 34, 0);
            w_u8(base + 38, 2);
            w_u8(base + 42, 1);
            w_u8(base + 43, 127);
            w_u8(base + 39, 127);
            w_u8(base + 35, 127);
            w_u8(base + 31, 127);
            w_u8(base + 44, 25);
            w_u8(base + 40, 25);
            w_u8(base + 36, 25);
            w_u8(base + 32, 25);
            w_u8(base + 45, 127);
            w_u8(base + 41, 127);
            w_u8(base + 37, 127);
            w_u8(base + 33, 127);
        }
        else
            diagnostic_report(501, 0x80011E84, 0);
        ++index;
        base += 48;
    }
    return 255;
}

sint32 sound_reset_voices(void)
{
    uint32 offset;
    sint32 i;
    FUNCTION_MARKER(0x800267D4, "SLUS_005.43");
    for (offset = 0; offset < 24 * 28; offset += 28)
    {
        w_u32(0x800336F0 + offset, 0);
        w_u32(0x800336F4 + offset, 0);
        w_u32(0x80033994 + offset, 0);
        w_u8(0x800339A4 + offset, 255);
        w_u8(0x800339A5 + offset, 255);
    }
    SpuSetKey(0, 0xFFFFFF);
    w_u32(0x8003131C, 0);
    for (i = 0; i < 4; ++i)
    {
        sound_tick();
        VSync(0);
    }
    return 0;
}

sint32 sound_load_samples(uint32 buffer)
{
    uint32 index;
    uint32 size;
    uint32 b1;
    uint32 b2;
    uint32 b3;
    sint32 sample;
    FUNCTION_MARKER(0x80026898, "SLUS_005.43");
    resource_load_into("VAG.WAD", buffer);
    for (index = 0; index < 30; ++index)
    {
        buffer += 12;
        size = r_u8(buffer++);
        b1 = r_u8(buffer++);
        b2 = r_u8(buffer++);
        b3 = r_u8(buffer++);
        size += (b1 << 8) + (b2 << 16) + (b3 << 24);
        sample = sound_transfer_vag(buffer, 0);
        w_u32(0x80042958 + index * 4, sample);
        if (sample < 0)
        {
#ifndef NDEBUG
            printf((const char *)psx_addr(0x80011DC0, 42));
#endif
        }
        buffer += size;
    }
    return 0;
}

sint32 sound_set_voice_volume(uint32 voice, uint32 volume)
{
    uint32 enabled;
    uint32 product;
    uint32 scaled;
    FUNCTION_MARKER(0x80025AC8, "SLUS_005.43");
    enabled = r_u8(0x80031258);
    if (!enabled)
        return 0;
    if (voice == 0xFFFFFFFF)
        return 0;
    if (voice >= 24)
        return (sint32)(voice << 3);
    volume &= 255;
    if (!(r_u32(0x80033994 + voice * 28) & 2))
        return (sint32)volume;
    product = volume * r_u8(0x8003125B);
    scaled = (uint32)(((uint64)product * 0x80808081) >> 32) >> 7;
    w_u8(0x800339A6 + voice * 28, scaled);
    return sound_update_voice(voice);
}

sint32 sound_update_voice(uint32 voice)
{
    uint32 mode;
    FUNCTION_MARKER(0x80025D1C, "SLUS_005.43");
    mode = r_u8(0x80033C30);
    switch (mode)
    {
    case 1:
        return sound_update_voice_mode1(voice);
    case 2:
        return sound_update_voice_mode2(voice);
    case 3:
        return sound_update_voice_mode3(voice);
    case 4:
        return sound_update_voice_mode4(voice);
    default:
        return mode < 3 ? 1 : 4;
    }
}

uint32 sound_find_voice(void)
{
    uint32 voice;
    FUNCTION_MARKER(0x80026528, "SLUS_005.43");
    for (;;)
    {
        voice = r_u32(0x8003131C) + 1;
        w_u32(0x8003131C, voice);
        if ((sint32)voice >= 24)
            w_u32(0x8003131C, 0);
        voice = r_u32(0x8003131C);
        if (r_u32(0x80033994 + voice * 28) & 2)
            continue;
        if (r_u8(0x80033CF4 + voice))
            continue;
        return r_u32(0x8003131C);
    }
}

sint32 input_append_rec(uint32 value)
{
    uint32 index;
    uint32 buffer;
    uint32 count;
    FUNCTION_MARKER(0x80015534, "SLUS_005.43");
    index = r_u16(0x80033D28);
    buffer = r_u32(0x8003112C);
    w_u16(0x80033D28, index + 1);
    count = r_u32(0x800428F8);
    w_u16(buffer + index * 2, value);
    ++count;
    w_u32(0x800428F8, count);
    if (count == 3600)
        w_u32(0x8004EB24, 1);
    return 1;
}

sint32 pad_scale_axis(sint32 center, sint32 dead_zone, sint32 limit, uint32 sample)
{
    uint32 delta;
    uint32 magnitude;
    uint32 negative_limit;
    sint32 divisor;
    sint32 numerator;
    FUNCTION_MARKER(0x8002A620, "SLUS_005.43");
    delta = (sample & 255) - (uint32)center;
    magnitude = (sint32)delta < 0 ? 0 - delta : delta;
    if ((sint32)magnitude < dead_zone)
        delta = 0;
    if ((sint32)delta > 0)
    {
        delta -= (uint32)dead_zone;
        if (limit < (sint32)delta)
            delta = (uint32)limit;
    }
    else if ((sint32)delta < 0)
    {
        negative_limit = 0 - (uint32)limit;
        delta += (uint32)dead_zone;
        if ((sint32)delta < (sint32)negative_limit)
            delta = negative_limit;
    }
    numerator = (sint32)(delta << 7);
    if (limit == dead_zone)
        return numerator;
    divisor = (sint32)((uint32)limit - (uint32)dead_zone);
    if (divisor == -1 && numerator == (sint32)0x80000000)
        trap(0x5D);
    return (sint32)((sint64)numerator / divisor);
}

sint32 sound_play_sample(uint32 sample)
{
    uint32 volume;
    uint32 flags;
    sint32 sound;
    FUNCTION_MARKER(0x800274F0, "SLUS_005.43");
    volume = r_u16(0x800118FC);
    flags = r_u32(0x800313A0);
    sound = r_s32(0x80042958 + (sample & 255) * 4);
    return sound_start_voice(sound, (volume >> 2) & 255, 15, 15, 256, flags);
}

sint32 sound_play_sample_pitch(uint32 sample, uint32 pitch)
{
    uint32 volume;
    uint32 flags;
    sint32 sound;
    FUNCTION_MARKER(0x800275A8, "SLUS_005.43");
    volume = r_u16(0x800118FC);
    flags = r_u32(0x800313A0);
    sound = r_s32(0x80042958 + (sample & 255) * 4);
    return sound_start_voice(sound, (volume >> 2) & 255, 15, 15, pitch, flags);
}

sint32 table_clear_column(uint32 column)
{
    uint32 row;
    FUNCTION_MARKER(0x80030BF4, "SLUS_005.43");
    for (row = 0; row < 64; ++row)
        w_u16(0x80057B50 + row * 64 + column * 2, 255);
    return 0;
}

sint32 table_fill_rows(uint32 first, sint32 end, uint32 value, uint32 column)
{
    uint32 row_address;
    FUNCTION_MARKER(0x80030C28, "SLUS_005.43");
    if ((sint32)first >= end)
        return (sint32)(first << 6);
    row_address = 0x80057B50 + (first << 6);
    do
    {
        w_u16(row_address + column * 2, value);
        ++first;
        row_address += 64;
    } while ((sint32)first < end);
    return 0;
}

sint32 table_max_rows(uint32 first, sint32 count, uint32 column)
{
    uint32 row;
    sint32 maximum = 0;
    sint32 value;
    FUNCTION_MARKER(0x80030A30, "SLUS_005.43");
    for (row = 0; (sint32)row < count; ++row)
    {
        value = r_s16(0x80057B50 + ((first + row) << 6) + column * 2);
        if (maximum < value)
            maximum = value;
    }
    return maximum;
}

sint32 table_find_rows(sint32 count, sint32 height, sint32 column)
{
    uint32 row = 0;
    uint32 offset;
    uint32 span;
    sint32 ceiling = column < 16 ? 256 : 242;
    sint32 last = (sint32)(64 - (uint32)count);
    sint32 value;
    sint32 failed;
    FUNCTION_MARKER(0x80030968, "SLUS_005.43");
    if (last < 0)
        return -1;
    offset = (uint32)column * 2;
    for (;;)
    {
        value = r_s16(0x80057B50 + row * 64 + offset);
        if (ceiling - value >= height)
        {
            failed = 0;
            span = 0;
            while ((sint32)span < count)
            {
                value = r_s16(0x80057B50 + ((row + span) << 6) + offset);
                ++span;
                if (ceiling - value < height)
                {
                    failed = 1;
                    break;
                }
            }
            if (!failed)
                return (sint32)row;
        }
        ++row;
        if (last < (sint32)row)
            return -1;
    }
}

uint32 stream_stop(void)
{
    uint32 current;
    uint32 pending;
    uint32 state;
    uint32 result;
    uint32 buffer;
    FUNCTION_MARKER(0x80027934, "SLUS_005.43");
    buffer = r_u32(0x80041008);
    w_u32(0x80041EE4, 1);
    w_u32(buffer, 0);
    w_u8(r_u32(0x80041008) + 48, 0);
    w_u8(r_u32(0x80041004) + 48, 0);
    current = r_u32(0x80040FF8);
    pending = r_u32(0x80041004);
    if (current == pending)
    {
        state = r_u32(0x80040FE4);
        if (state - 6 < 2 || state - 2 < 2)
        {
            w_u32(0x80040FE4, 7);
            return 7;
        }
        result = r_u32(0x80040FFC);
        if (result)
            w_u32(current, 0);
        return result;
    }
    if (r_u32(0x80040FE4) == 7)
        w_u32(0x80040FE4, 6);
    w_u32(pending, 0);
    return 6;
}

sint32 disc_wait_stream(void)
{
    FUNCTION_MARKER(0x80027764, "SLUS_005.43");
    CdDataCallback(NULL);
    CdReadyCallback(NULL);
    return game_mount_stream_source();
}

sint32 stream_init(void)
{
    FUNCTION_MARKER(0x800277D0, "SLUS_005.43");
    disc_wait_valid();
    w_u32(0x80040FF0, 0);
    w_u32(0x80041EEC, 0);
    w_u32(0x80041EF0, 0);
    w_u32(0x80041EDC, 0);
    w_u32(0x80041EE0, 0);
    w_u32(0x800414FC, 0);
    w_u32(0x8004100C, 0);
    w_u32(0x80041148, 0);
    w_u32(0x80041284, 0);
    w_u32(0x800413C0, 0);
    w_u32(0x80040FFC, 0x8004100C);
    w_u32(0x80041000, 0x80041148);
    w_u32(0x80041008, 0x800413C0);
    w_u32(0x80041004, 0x80041284);
    w_u32(0x80041EE4, 0);
    w_u32(0x80041EE8, 0);
    w_u32(0x80040FF8, 0);
    w_u32(0x80041EF4, 0);
    w_u32(0x80040FE4, 0);
    w_u32(0x80040FE8, 12);
    SsSetSerialVol(0, 0, 0);
    w_u32(0x80040FE0, r_u32(0x800311C0) ? 6 : 5);
    return disc_wait_stream();
}

sint32 game_init_options(void)
{
    sint32 level;
    FUNCTION_MARKER(0x8001F9D0, "SLUS_005.43");
    w_u32(0x800118E0, 1);
    w_u16(0x800118FA, 24575);
    w_u16(0x800118FC, 191);
    w_u16(0x800118FE, 24575);
    w_u16(0x800118F6, 1);
    w_u16(0x800118F8, 1);
    w_u16(0x80011900, 2);
    w_u16(0x800118F4, 0);
    w_u32(0x80011904, 0);
    w_u32(0x80011908, 0);
    w_u32(0x8001190C, 0);
    w_u32(0x80011910, 0);
    w_u32(0x80011914, 0);
    w_u32(0x80011918, 0);
    w_u32(0x8001191C, 0);
    w_u32(0x80011920, 0);
    w_u32(0x80011924, 0);
    w_u32(0x80011928, 0);
    w_u16(0x80011902, 0);
    w_u16(0x8001192C, 0);
    resource_clear_flags();
    level = r_s16(0x800118FC);
    sound_set_level((uint32)level);
    return level;
}

uint32 render_alloc_buffers(void)
{
    uint32 buffer;
    uint32 owner;
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x80021CF4, "SLUS_005.43");
    w_u32(0x80042170, 0x80035080);
    buffer = heap_alloc_back(0x32800);
    owner = r_u32(0x80042170);
    w_u32(owner + 0x45F0, buffer);
    first = r_u32(owner + 0x45F0);
    second = buffer + 0x19400;
    w_u32(owner + 0x8BE4, second);
    w_u32(first + 0x18FFC, 0x12345678);
    w_u32(r_u32(owner + 0x8BE4) + 0x18FFC, 0x12345678);
    return second;
}

void render_free_buffers(void)
{
    FUNCTION_MARKER(0x80021D80, "SLUS_005.43");
    heap_free(r_u32(r_u32(0x80042170) + 0x45F0));
}

sint32 font_init(void)
{
    uint32 character;
    uint32 punctuation;
    uint32 i;
    sint32 glyph;
    SPRT *sprite;
    FUNCTION_MARKER(0x8002E3EC, "SLUS_005.43");
    for (character = 0; character < 255; ++character)
    {
        if (character >= 192 && character < 215)
            glyph = character - 108;
        else if (character >= 216 && character < 222)
            glyph = character - 109;
        else if (character == 223)
            glyph = 143;
        else if (character >= 224 && character < 247)
            glyph = character - 111;
        else if (character >= 248 && character < 254)
            glyph = character - 112;
        else if (character >= 'A' && character <= 'Z')
            glyph = character - 'A';
        else if (character >= 'a' && character <= 'z')
            glyph = character - 71;
        else if (character == ' ')
            glyph = -1;
        else if (character >= '0' && character <= '9')
            glyph = character + 4;
        else
        {
            punctuation = 0;
            while (r_u8(0x80031414 + punctuation))
            {
                if (character == r_u8(0x80031414 + punctuation))
                    break;
                ++punctuation;
            }
            glyph = r_u8(0x80031414 + punctuation) ? (sint32)(punctuation + 62) : -1;
        }
        w_u16(0x8003FC68 + character * 2, glyph);
    }
    for (i = 0; i < 350; ++i)
    {
        sprite = (SPRT *)psx_addr(0x800429D8 + i * 40, sizeof(SPRT));
        setSprt(sprite);
        setSemiTrans(sprite, 1);
        sprite = (SPRT *)psx_addr(0x800429EC + i * 40, sizeof(SPRT));
        setSprt(sprite);
        setSemiTrans(sprite, 1);
    }
    return 0;
}

sint32 matrix_set_identity(uint32 matrix)
{
    return matrix_set_identity_native((MATRIX *)psx_addr(matrix, 18u));
}

sint32 matrix_set_identity_native(MATRIX *matrix)
{
    FUNCTION_MARKER(0x8002C0CC, "SLUS_005.43");
    matrix->m[0][0] = 4096;
    matrix->m[1][0] = 0;
    matrix->m[2][0] = 0;
    matrix->m[0][1] = 0;
    matrix->m[1][1] = 4096;
    matrix->m[2][1] = 0;
    matrix->m[0][2] = 0;
    matrix->m[1][2] = 0;
    matrix->m[2][2] = 4096;
    return 4096;
}

void resource_free_bundle(uint32 bundle)
{
    FUNCTION_MARKER(0x8001577C, "SLUS_005.43");
    if (bundle)
    {
        heap_free(r_u32(bundle + 16));
        heap_free(r_u32(bundle));
        heap_free(r_u32(bundle + 8));
        heap_free(bundle);
    }
}

sint32 render_reset(void)
{
    FUNCTION_MARKER(0x8001F970, "SLUS_005.43");
    return ResetGraph(3);
}

uint32 resource_file_size(const char *name)
{
    FUNCTION_MARKER(0x8002173C, "SLUS_005.43");
    return disc_file_size(name);
}

sint32 video_init_front(void)
{
    DISPENV environment;
    FUNCTION_MARKER(0x8001EDC0, "SLUS_005.43");
    VSync(0);
    SetDispMask(0);
    ResetGraph(0);
    if (r_u32(0x800311C0))
        system_set_video_mode(1);
    SetDefDispEnv(&environment, 0, 0, 640, 480);
    environment.pad0 = r_u8(0x800311C0);
    environment.isinter = 1;
    environment.isrgb24 = 0;
    VSync(0);
    PutDispEnv(&environment);
    return VSync(30);
}

uint32 resource_find_entry(const char *name)
{
    uint32 index = 0;
    uint32 entry;
    FUNCTION_MARKER(0x80020DB4, "SLUS_005.43");
    for (;;)
    {
        entry = r_u32(0x800421A8) + (index << 5);
        if (!r_u8(entry))
            break;
        entry = r_u32(0x800421A8) + (index << 5);
        if (!strcmp(name, (const char *)psx_addr(entry, 32)))
            return index;
        ++index;
    }
    diagnostic_report(444, 0x80011B74, (intptr)name);
    return 0;
}

sint32 resource_exists(const char *name)
{
    sint32 found = 0;
    uint32 entry;
    FUNCTION_MARKER(0x80021278, "SLUS_005.43");
    entry = r_u32(0x800421A8);
    while (r_u8(entry) && !found)
    {
        found = strcmp(name, (const char *)psx_addr(entry, 32)) == 0;
        entry += 32;
    }
    return found;
}

uint32 disc_file_size(const char *name)
{
    size_t archive_size;
    uint32 index;
    uint32 next;
    uint32 cur;
    uint32 end;
    uint32 start;
    uint32 length;
    FUNCTION_MARKER(0x80021568, "SLUS_005.43");
    if (r_u32(0x8003117C) == 1)
    {
        if (!resource_exists(name))
        {
            errno = ENOENT;
            return 0;
        }
        index = resource_find_entry(name);
        next = r_u32(0x800421A8) + (index << 5) + 32;
        cur = r_u32(0x800421A8) + (index << 5);
        end = r_u32(next + 16);
        start = r_u32(cur + 16);
        if (!xport_file_size("DATA/1/GAME.RSC", &archive_size))
            return 0;
        if (end < start || end > archive_size)
        {
            errno = EINVAL;
            return 0;
        }
        return end - start;
    }
    if (game_extracted_file_size(name, &length))
        return length;
    diagnostic_report(100, 0x80011BCC, (intptr)name);
    return 0;
}

static uint32 resource_load_into_buffer(const char *name, uint32 destination, uint8 *native, size_t capacity)
{
    FILE *file;
    long length;
    uint32 index;
    uint32 next;
    uint32 cur;
    uint32 total;
    uint32 first;
    uint32 offset;
    uint32 copied;
    uint32 middle;
    uint32 read;
    uint32 end;
    int error;
    if (r_u32(0x80040FE8) != 12)
    {
        w_u32(0x80040FE8, 12);
        w_u32(0x80040FE4, 4);
    }
    if (r_u32(0x8003117C) != 1)
    {
        errno = EINVAL;
        return 0;
    }
    if (!resource_exists(name))
    {
        errno = ENOENT;
        return 0;
    }
    index = resource_find_entry(name);
    next = r_u32(0x800421A8) + (index << 5) + 32;
    cur = r_u32(0x800421A8) + (index << 5);
    end = r_u32(next + 16);
    offset = r_u32(cur + 16);
    file = xport_fopen("DATA/1/GAME.RSC", "rb");
    if (!file)
        return 0;
    if (fseek(file, 0, SEEK_END) || (length = ftell(file)) < 0)
    {
        error = errno;
        fclose(file);
        errno = error ? error : EIO;
        return 0;
    }
    if (end < offset || (uint64)end > (uint64)length)
    {
        fclose(file);
        errno = EINVAL;
        return 0;
    }
    if (fseek(file, (long)offset, SEEK_SET))
    {
        error = errno;
        fclose(file);
        errno = error ? error : EIO;
        return 0;
    }
    total = end - offset;
    if (native && total > capacity)
    {
        fclose(file);
        errno = ERANGE;
        return 0;
    }
    w_u32(0x80031194, offset & 0x7FF);
    first = 2048 - r_u32(0x80031194);
    if (total < first)
        first = total;
    errno = 0;
    copied = (uint32)xport_fread(native ? native + destination : psx_addr(destination, first), 1, first, file);
    if (copied != first)
        goto read_error;
    destination += copied;
    middle = (total - copied) / 2048 * 2048;
    VSync(0);
    if (middle)
    {
        read = (uint32)xport_fread(native ? native + destination : psx_addr(destination, middle), 1, middle, file);
        destination += read;
        copied += read;
        if (read != middle)
            goto read_error;
    }
    VSync(0);
    if (total != copied)
    {
        read = (uint32)xport_fread(native ? native + destination : psx_addr(destination, total - copied), 1, total - copied, file);
        copied += read;
        if (copied != total)
            goto read_error;
    }
    fclose(file);
    CdDataCallback(0);
    CdReadyCallback(0);
    return total;

read_error:
    error = errno;
    fclose(file);
    CdDataCallback(0);
    CdReadyCallback(0);
    errno = error ? error : EIO;
    return copied;
}

uint32 resource_load_into(const char *name, uint32 destination)
{
    FUNCTION_MARKER(0x80021784, "SLUS_005.43");
    return resource_load_into_buffer(name, destination, NULL, 0);
}

uint32 resource_load_into_native(const char *name, void *destination, size_t capacity)
{
    if (!destination)
    {
        errno = EINVAL;
        return 0;
    }
    return resource_load_into_buffer(name, 0, (uint8 *)destination, capacity);
}

uint32 pad_set_axis(sint32 axis, sint32 value, uint32 pad)
{
    uint32 offset;
    uint32 positive;
    uint32 negative;
    FUNCTION_MARKER(0x8002A480, "SLUS_005.43");
    switch (axis)
    {
    case 1:
        negative = 2;
        positive = 3;
        break;
    case 2:
        negative = 0;
        positive = 1;
        break;
    case 3:
        negative = 4;
        positive = 5;
        break;
    case 4:
        negative = 7;
        positive = 6;
        break;
    default:
        return axis < 3 ? 1 : 4;
    }
    if (!value)
        return pad * 5;
    offset = pad * 21;
    if (value < 0)
        w_u8(0x80040F20 + offset + negative, 0 - (uint32)value);
    else
        w_u8(0x80040F20 + offset + positive, value);
    return offset;
}

sint32 table_column_empty(uint32 column)
{
    uint32 row;
    FUNCTION_MARKER(0x80030754, "SLUS_005.43");
    for (row = 0; row < 64; ++row)
        if (r_s16(0x80057B50 + row * 64 + column * 2))
            return 0;
    return 1;
}

uint32 tex_alloc_region(sint32 width, sint32 height)
{
    uint32 column;
    sint32 row;
    sint32 y;
    sint32 pages;
    sint32 i;
    FUNCTION_MARKER(0x80030794, "SLUS_005.43");
    if (width < 64)
    {
        for (column = 0; column < 32; ++column)
        {
            row = table_find_rows(width, height, (sint32)column);
            if (row != -1)
            {
                y = table_max_rows((uint32)row, width, column);
                table_fill_rows((uint32)row, (sint32)((uint32)row + (uint32)width), (uint32)y + (uint32)height, column);
                return ((uint32)row + ((column & 15) << 6)) |
                    (((uint32)y + ((column >> 4) << 8)) << 16);
            }
        }
        return 0xFFFFFFFF;
    }
    for (column = 0; column < 15; ++column)
    {
        if (table_column_empty(column) && table_column_empty(column + 1))
        {
            y = table_max_rows(0, width, column);
            table_fill_rows(0, width, (uint32)y + (uint32)height, column);
            return ((column & 15) << 6) | ((column / 16) << 16);
        }
    }
    column = 16;
    for (;;)
    {
        if (table_column_empty(column) && table_column_empty(column + 1))
            break;
        if (++column >= 31)
            return 0xFFFFFFFF;
    }
    pages = (sint32)((uint32)width + 63) >> 6;
    for (i = 0; i < pages; ++i)
        table_clear_column(column + (uint32)i);
    return ((column & 15) << 6) | ((column / 16) << 24);
}

sint32 tex_reset_regions(void)
{
    uint32 row;
    sint32 column;
    uint32 address;
    uint32 level;
    FUNCTION_MARKER(0x80030A94, "SLUS_005.43");
    w_u32(0x80031454, 0);
    for (row = 0; row < 64; ++row)
        for (column = 31; column >= 0; --column)
            w_u16(0x80057B50 + row * 64 + (uint32)column * 2, 0);
    address = 0x80042950;
    for (column = 32; column >= 0; --column)
    {
        w_u16(address, 512);
        address -= 2;
    }
    for (column = 0; column < 8; ++column)
    {
        table_clear_column(column);
        table_clear_column(column + 16);
    }
    table_clear_column(21);
    table_clear_column(22);
    table_clear_column(23);
    table_clear_column(24);
    table_clear_column(25);
    table_clear_column(26);
    if (r_s16(0x800595F8) == 2)
        return table_clear_column(27);
    level = r_u32(0x800595F8);
    switch (level)
    {
    case 0x10003:
    case 0x20003:
    case 0x40006:
    case 0x30007:
    case 0x10009:
    case 0x50009:
    case 0x4000B:
    case 0x1000D:
    case 0x20011:
        return table_clear_column(27);
    default:
        return 0x20011;
    }
}

sint32 pad_update(uint32 pad)
{
    uint32 first_pad;
    uint32 second_pad;
    uint32 source;
    uint32 a;
    uint32 b;
    uint32 c;
    uint32 d;
    uint32 base;
    uint32 output;
    uint32 type;
    uint32 candidate;
    uint32 index;
    uint32 config;
    uint32 buttons = 0;
    uint32 prev;
    sint32 axis0;
    sint32 axis1;
    sint32 axis2;
    sint32 axis3;
    FUNCTION_MARKER(0x8002A6C8, "SLUS_005.43");
    first_pad = r_u8(0x800313D5);
    if (r_u8(0x8004EA28 + first_pad * 32))
        w_u8(0x800313D4, 1);
    source = 0x8004EA28 + first_pad * 32;
    second_pad = r_u8(0x800313D6);
    a = r_u32(source);
    b = r_u32(source + 4);
    c = r_u32(source + 8);
    d = r_u32(source + 12);
    w_u32(0x8004EA28, a);
    w_u32(0x8004EA2C, b);
    w_u32(0x8004EA30, c);
    w_u32(0x8004EA34, d);
    a = r_u32(source + 16);
    b = r_u32(source + 20);
    c = r_u32(source + 24);
    d = r_u32(source + 28);
    w_u32(0x8004EA38, a);
    w_u32(0x8004EA3C, b);
    w_u32(0x8004EA40, c);
    w_u32(0x8004EA44, d);
    source = 0x8004EA28 + second_pad * 32;
    a = r_u32(source);
    b = r_u32(source + 4);
    c = r_u32(source + 8);
    w_u32(0x8004EA48, a);
    w_u32(0x8004EA4C, b);
    w_u32(0x8004EA50, c);
    a = r_u32(source + 12);
    b = r_u32(source + 16);
    c = r_u32(source + 20);
    w_u32(0x8004EA54, a);
    w_u32(0x8004EA58, b);
    w_u32(0x8004EA5C, c);
    a = r_u32(source + 24);
    b = r_u32(source + 28);
    w_u32(0x8004EA60, a);
    w_u32(0x8004EA64, b);

    source = 0x8004EAE0 + r_u8(0x800313D5) * 4;
    a = r_u32(0x8004EAE0);
    w_u32(source, a);
    w_u32(source, a);
    source = 0x8004EAE0 + r_u8(0x800313D6) * 4;
    a = r_u32(0x8004EAE4);
    w_u32(source, a);
    w_u32(source, a);

    base = 0x8004EA28 + pad * 32;
    if (!r_u8(base) && r_u16(base + 2) != 0xFFFF)
    {
        w_u32(0x800313D0, 0);
        w_u32(0x800313C8, 0);
    }
    if (r_u32(0x80031138) & 3)
        return pad_update_replay(pad);
    output = 0x80040F20 + pad * 21;
    w_u8(output + 7, 0);
    w_u8(output + 6, 0);
    w_u8(output + 5, 0);
    w_u8(output + 4, 0);
    w_u8(output + 3, 0);
    w_u8(output + 2, 0);
    w_u8(output + 1, 0);
    w_u8(output, 0);
    if (r_u8(base))
        return 0;
    type = r_u8(base + 1);
    if (type != r_u8(0x800313C1))
    {
        w_u8(0x800313C1, type);
        candidate = r_u8(0x800313BC);
        index = 0;
        if (candidate != 255)
        {
            while (candidate != type)
            {
                candidate = r_u8(0x800313BC + index + 1);
                ++index;
                if (candidate == 255)
                    break;
            }
            candidate = r_u8(0x800313BC + index);
        }
        w_u32(0x800313C4, candidate == 255 ? 0 : 0x800421C0 + index * 48);
    }
    type = r_u8(base + 1);
    if (type == 'S' || type == 's' || type == 'A')
    {
        a = r_u8(base + 2);
        b = r_u8(base + 3);
        buttons = ~((a << 8) | b);
    }
    config = r_u32(0x800313C4);
    if (!config)
        return (sint16)buttons;

    if (buttons & r_u16(config))
        w_u8(output, 127);
    if (buttons & r_u16(r_u32(0x800313C4) + 2))
        w_u8(output + 1, 127);
    if (buttons & r_u16(r_u32(0x800313C4) + 4))
        w_u8(output + 2, 127);
    if (buttons & r_u16(r_u32(0x800313C4) + 6))
        w_u8(output + 3, 127);
    if (buttons & r_u16(r_u32(0x800313C4) + 8))
        w_u8(output + 4, 127);
    if (buttons & r_u16(r_u32(0x800313C4) + 10))
        w_u8(output + 5, 127);
    if (buttons & r_u16(r_u32(0x800313C4) + 12))
        w_u8(output + 6, 127);
    if (buttons & r_u16(r_u32(0x800313C4) + 14))
        w_u8(output + 7, 127);

    if (buttons & r_u16(r_u32(0x800313C4) + 16))
    {
        prev = r_u8(output + 8);
        w_u8(output + 8, 127);
        w_u8(output + 15, prev ^ 127);
    }
    else
    {
        w_u8(output + 15, 0);
        w_u8(output + 8, 0);
    }
    if (buttons & r_u16(r_u32(0x800313C4) + 18))
    {
        prev = r_u8(output + 9);
        w_u8(output + 9, 127);
        w_u8(output + 16, prev ^ 127);
    }
    else
    {
        w_u8(output + 16, 0);
        w_u8(output + 9, 0);
    }
    if (buttons & r_u16(r_u32(0x800313C4) + 20))
    {
        prev = r_u8(output + 10);
        w_u8(output + 10, 127);
        w_u8(output + 17, prev ^ 127);
    }
    else
    {
        w_u8(output + 17, 0);
        w_u8(output + 10, 0);
    }
    if (buttons & r_u16(r_u32(0x800313C4) + 22))
    {
        prev = r_u8(output + 11);
        w_u8(output + 11, 127);
        w_u8(output + 18, prev ^ 127);
    }
    else
    {
        w_u8(output + 18, 0);
        w_u8(output + 11, 0);
    }
    if (buttons & r_u16(r_u32(0x800313C4) + 24))
    {
        prev = r_u8(output + 12);
        w_u8(output + 12, 127);
        w_u8(output + 19, prev ^ 127);
    }
    else
    {
        w_u8(output + 19, 0);
        w_u8(output + 12, 0);
    }
    if (buttons & r_u16(r_u32(0x800313C4) + 26))
    {
        prev = r_u8(output + 13);
        w_u8(output + 13, 127);
        w_u8(output + 20, prev ^ 127);
    }
    else
    {
        w_u8(output + 20, 0);
        w_u8(output + 13, 0);
    }
    if (buttons & r_u16(r_u32(0x800313C4) + 28))
        w_u8(output + 14, 127);
    type = r_u8(base + 1);
    if (type == 'S' || type == 's')
    {
        config = r_u32(0x800313C4);
        a = r_u8(base + 4);
        b = r_u8(config + 31);
        c = r_u8(config + 32);
        d = r_u8(config + 33);
        axis0 = pad_scale_axis(b, c, d, a);
        config = r_u32(0x800313C4);
        a = r_u8(base + 5);
        b = r_u8(config + 35);
        c = r_u8(config + 36);
        d = r_u8(config + 37);
        axis1 = pad_scale_axis(b, c, d, a);
        config = r_u32(0x800313C4);
        a = r_u8(base + 6);
        b = r_u8(config + 39);
        c = r_u8(config + 40);
        d = r_u8(config + 41);
        axis2 = pad_scale_axis(b, c, d, a);
        config = r_u32(0x800313C4);
        a = r_u8(base + 7);
        b = r_u8(config + 43);
        c = r_u8(config + 44);
        d = r_u8(config + 45);
        axis3 = pad_scale_axis(b, c, d, a);
        pad_set_axis(r_u8(r_u32(0x800313C4) + 30), axis0, pad);
        pad_set_axis(r_u8(r_u32(0x800313C4) + 34), axis1, pad);
        pad_set_axis(r_u8(r_u32(0x800313C4) + 38), axis2, pad);
        pad_set_axis(r_u8(r_u32(0x800313C4) + 42), axis3, pad);
    }
    return (sint16)buttons;
}

uint32 overlay_load_resource(const char *name, sint32 load_scene)
{
    char path[32];
    uint32 sectors;
    uint32 size;
    uint32 loaded;
    uint32 allocation;
    uint32 free_before;
    uint32 count_before;
    FUNCTION_MARKER(0x8001F5F4, "SLUS_005.43");
    strcpy(path, name);
    strcat(path, ".OVL");
    size = resource_file_size(path);
    sectors = (uint32)((sint32)(size + 2047) / 2048);
    allocation = heap_alloc_front(sectors << 11);
    free_before = heap_free_bytes();
    count_before = r_u32(0x800313B4);
    if (load_scene)
        level_load_callback(r_s16(0x8001192E) >= 9 ? 10 : 0);
    w_u32(0x8004EAC8, free_before - heap_free_bytes());
    w_u32(0x8004EACC, r_u32(0x800313B4) - count_before);
    loaded = resource_read_file(path, allocation);
    if (loaded == size && loaded != 0)
        game_select_native_overlay(name);
    return allocation;
}

uint32 resource_read_file(const char *name, uint32 destination)
{
    FUNCTION_MARKER(0x800216EC, "SLUS_005.43");
    return resource_load_into(name, destination);
}

MATRIX *matrix_rotate_x(MATRIX *destination, MATRIX *source, uint32 angle)
{
    MATRIX rotation = {0};
    sint16 c;
    sint16 s;
    FUNCTION_MARKER(0x8002BCC4, "SLUS_005.43");
    angle &= 0xFFF;
    c = (sint16)rcos(angle);
    s = (sint16)rsin(angle);
    rotation.m[0][0] = 4096;
    rotation.m[0][1] = 0;
    rotation.m[0][2] = 0;
    rotation.m[1][0] = 0;
    rotation.m[1][1] = c;
    rotation.m[1][2] = -s;
    rotation.m[2][0] = 0;
    rotation.m[2][1] = s;
    rotation.m[2][2] = c;
    MulMatrix0(source, &rotation, destination);
    return destination;
}

MATRIX *matrix_rotate_y(MATRIX *destination, MATRIX *source, uint32 angle)
{
    MATRIX rotation = {0};
    sint16 c;
    sint16 s;
    FUNCTION_MARKER(0x8002BE70, "SLUS_005.43");
    angle &= 0xFFF;
    c = (sint16)rcos(angle);
    s = (sint16)rsin(angle);
    rotation.m[0][2] = -s;
    rotation.m[0][0] = c;
    rotation.m[0][1] = 0;
    rotation.m[1][0] = 0;
    rotation.m[1][1] = 4096;
    rotation.m[1][2] = 0;
    rotation.m[2][0] = s;
    rotation.m[2][1] = 0;
    rotation.m[2][2] = c;
    MulMatrix0(source, &rotation, destination);
    return destination;
}

MATRIX *matrix_rotate_z(MATRIX *destination, MATRIX *source, uint32 angle)
{
    MATRIX rotation = {0};
    sint16 c;
    sint16 s;
    FUNCTION_MARKER(0x8002BF88, "SLUS_005.43");
    angle &= 0xFFF;
    c = (sint16)rcos(angle);
    s = (sint16)rsin(angle);
    rotation.m[0][1] = s;
    rotation.m[1][0] = -s;
    rotation.m[0][0] = c;
    rotation.m[0][2] = 0;
    rotation.m[1][1] = c;
    rotation.m[1][2] = 0;
    rotation.m[2][0] = 0;
    rotation.m[2][1] = 0;
    rotation.m[2][2] = 4096;
    MulMatrix0(source, &rotation, destination);
    return destination;
}

uint32 input_next_rec(sint32 cancel)
{
    uint32 count;
    uint32 index;
    uint32 buffer;
    uint32 result;
    FUNCTION_MARKER(0x800154C8, "SLUS_005.43");
    if (cancel)
    {
        w_u32(0x8004EB24, 1);
        return 0;
    }
    count = r_u32(0x800428F8);
    if ((sint32)count >= 2640)
    {
        w_u32(0x8004EB24, 1);
        return 0;
    }
    index = r_u16(0x80033D28);
    buffer = r_u32(0x8003112C);
    w_u32(0x800428F8, count + 1);
    result = r_u16(buffer + index * 2);
    w_u16(0x80033D28, index + 1);
    return result;
}

sint32 pad_update_replay(uint32 pad)
{
    uint32 mode;
    uint32 sample = 0;
    uint32 type;
    uint32 buttons = 0;
    uint32 output;
    uint32 prev;
    uint32 center;
    uint32 dead_zone;
    uint32 limit;
    uint32 analog;
    sint32 axis0;
    sint32 axis1;
    sint32 axis2;
    sint32 axis3;
    FUNCTION_MARKER(0x8002B124, "SLUS_005.43");
    mode = r_u32(0x80031138);
    if (mode & 2)
    {
        sample = r_u16(0x8004EA2A + pad * 32);
        type = r_u8(0x8004EA29 + pad * 32);
        if (type == 'S' || type == 's' || type == 'A')
            buttons = ~(((sample & 255) << 8) | (sample >> 8));
        sample = (uint16)input_next_rec((sint16)buttons);
        buttons = 0;
    }
    else if (mode & 1)
    {
        sample = r_u16(0x8004EA2A + pad * 32);
        input_append_rec(sample);
    }
    output = 0x80040F20 + pad * 21;
    w_u8(output + 13, 0);
    w_u8(output + 12, 0);
    w_u8(output + 7, 0);
    w_u8(output + 6, 0);
    w_u8(output + 5, 0);
    w_u8(output + 4, 0);
    w_u8(output + 3, 0);
    w_u8(output + 2, 0);
    w_u8(output + 1, 0);
    w_u8(output, 0);
    if (r_u8(0x80040F50))
        return 0;
    type = r_u8(0x80040F51);
    if (type == 'S' || type == 's' || type == 'A')
        buttons = ~(((sample & 255) << 8) | (sample >> 8));
    if (buttons & r_u16(0x800428C8))
        w_u8(output, 127);
    if (buttons & r_u16(0x800428CA))
        w_u8(output + 1, 127);
    if (buttons & r_u16(0x800428CC))
        w_u8(output + 2, 127);
    if (buttons & r_u16(0x800428CE))
        w_u8(output + 3, 127);
    if (buttons & r_u16(0x800428D0))
        w_u8(output + 4, 127);
    if (buttons & r_u16(0x800428D2))
        w_u8(output + 5, 127);
    if (buttons & r_u16(0x800428D4))
        w_u8(output + 6, 127);
    if (buttons & r_u16(0x800428D6))
        w_u8(output + 7, 127);
    if (buttons & r_u16(0x800428D8))
    {
        prev = r_u8(output + 8);
        w_u8(output + 8, 127);
        w_u8(output + 15, prev ^ 127);
    }
    else
    {
        w_u8(output + 15, 0);
        w_u8(output + 8, 0);
    }
    if (buttons & r_u16(0x800428DA))
    {
        prev = r_u8(output + 9);
        w_u8(output + 9, 127);
        w_u8(output + 16, prev ^ 127);
    }
    else
    {
        w_u8(output + 16, 0);
        w_u8(output + 9, 0);
    }
    if (buttons & r_u16(0x800428DC))
    {
        prev = r_u8(output + 10);
        w_u8(output + 10, 127);
        w_u8(output + 17, prev ^ 127);
    }
    else
    {
        w_u8(output + 17, 0);
        w_u8(output + 10, 0);
    }
    if (buttons & r_u16(0x800428DE))
    {
        prev = r_u8(output + 11);
        w_u8(output + 11, 127);
        w_u8(output + 18, prev ^ 127);
    }
    else
    {
        w_u8(output + 18, 0);
        w_u8(output + 11, 0);
    }
    if (buttons & r_u16(0x800428E0))
        w_u8(output + 12, 127);
    if (buttons & r_u16(0x800428E2))
        w_u8(output + 13, 127);
    if (buttons & r_u16(0x800428E4))
        w_u8(output + 14, 127);
    type = r_u8(0x80040F51);
    if (type == 'S' || type == 's')
    {
        center = r_u8(0x800428E7);
        dead_zone = r_u8(0x800428E8);
        limit = r_u8(0x800428E9);
        analog = r_u8(0x80040F54);
        axis0 = pad_scale_axis(center, dead_zone, limit, analog);
        center = r_u8(0x800428EB);
        dead_zone = r_u8(0x800428EC);
        limit = r_u8(0x800428ED);
        analog = r_u8(0x80040F55);
        axis1 = pad_scale_axis(center, dead_zone, limit, analog);
        center = r_u8(0x800428EF);
        dead_zone = r_u8(0x800428F0);
        limit = r_u8(0x800428F1);
        analog = r_u8(0x80040F56);
        axis2 = pad_scale_axis(center, dead_zone, limit, analog);
        center = r_u8(0x800428F3);
        dead_zone = r_u8(0x800428F4);
        limit = r_u8(0x800428F5);
        analog = r_u8(0x80040F57);
        axis3 = pad_scale_axis(center, dead_zone, limit, analog);
        pad_set_axis(r_u8(0x800428E6), axis0, pad);
        pad_set_axis(r_u8(0x800428EA), axis1, pad);
        pad_set_axis(r_u8(0x800428EE), axis2, pad);
        pad_set_axis(r_u8(0x800428F2), axis3, pad);
    }
    return (sint16)buttons;
}

sint32 level_load_callback(sint32 mode)
{
    uint32 owner;
    uint32 base;
    uint32 model;
    uint32 index;
    uint32 random;
    FUNCTION_MARKER(0x800149BC, "SLUS_005.43");
    SetDispMask(0);
    render_set_width(320);
    render_set_clear_color(0, 0, 0);
    SetDispMask(0);
    w_u32(0x800310A8, 0);
    base = r_u32(0x80042170);
    owner = r_u32(0x8004608C);
    if (owner == r_u32(0x80042170))
        base += 0x45F4;
    w_u32(0x8004608C, base);
    owner = r_u32(0x8004608C);
    ClearOTagR((uint32 *)psx_addr(owner + 112, 4448 * 4), 4448);
    w_u32(0x80034D78, 4447);
    w_u32(0x800520FC, 0xFFFFFFFF);
    w_u32(0x800421AC, r_u32(0x8004608C) + 116);
    base = r_u32(0x8004608C);
    w_u32(0x80040690, r_u32(base + 0x45F0));
    w_u32(0x8004217C, r_u32(0x80040690));
    render_present_frame();
    render_swap_buffers();
    render_present_frame();
    render_swap_buffers();
    sound_init_loops();
    if (!mode)
    {
        model = model_load_bundle(0x800119C4, 6, 464);
        w_u32(0x80040FB8, model);
        index = (uint32)(sint32)r_s16(0x8001192C);
        model = model_load_bundle(r_u32(0x8003107C + (index << 2)), 5, 448);
        w_u32(0x80059658, model);
        camera_reset_follow();
        camera_set_target(0x80034DE0);
        camera_set_follow_distance(50000, 0);
        r_u32(0x80040C94);
        camera_update_modes();
        camera_set_position(0x800310C0);
        object_init(0x80034DE0);
        object_init(0x80040798);
        w_u32(0x80034E44, r_u32(0x80059658));
        w_u32(0x80034E18, 0x8001690C);
        w_u32(0x80034E64, 0xFFFFFFFB);
        w_u32(0x80034DE8, 0);
        w_u32(0x80034DEC, 2500);
        w_u32(0x80034DF0, 0);
        w_u32(0x800407FC, r_u32(0x80040FB8));
        w_u32(0x800407D0, 0x8001690C);
        w_u32(0x8004081C, 0xFFFFFFFF);
        w_u32(0x800407A0, 0);
        w_u32(0x800407A4, 500);
        w_u32(0x800407A8, 0);
        w_u32(0x80031114, 0);
        w_u32(0x80031118, 20);
        w_u32(0x8003111C, 45);
        render_set_semi_trans(0);
        render_init_lights();
        scene_start_hangar();
        w_u32(0x800310A4, 1);
    }
    else if (mode == 10)
    {
        index = (uint32)(sint32)r_s16(0x8001192C);
        model = model_load_bundle(r_u32(0x8003107C + (index << 2)), 5, 448);
        w_u32(0x80059658, model);
        camera_reset_follow();
        camera_set_target(0x80034DE0);
        camera_set_follow_distance(50000, 0);
        r_u32(0x80040C94);
        camera_update_modes();
        camera_set_position(0x800310C0);
        object_init(0x80034DE0);
        w_u32(0x80034E44, r_u32(0x80059658));
        w_u32(0x80034E18, 0x8001690C);
        w_u32(0x80034E64, 0xFFFFFFFD);
        w_u32(0x80059E60, 0x800510F8);
        for (index = 0; index < 135; ++index)
        {
            random = randPSX();
            base = r_u32(0x80059E60) + (index << 3);
            w_u16(base, (random & 0x3FFF) - 0x2000);
            random = randPSX();
            base = r_u32(0x80059E60) + (index << 3);
            w_u16(base + 2, (random & 0x1FFF) - 2048);
            random = randPSX();
            base = r_u32(0x80059E60) + (index << 3);
            w_u16(base + 4, (random & 0x3FFF) - 0x2000);
        }
        camera_set_target(0x80034DE0);
        render_init_lights();
        scene_start_stars();
        w_u32(0x800310A4, 1);
    }
#ifndef NDEBUG
    return printf("LOAD_CALLBACK : callback started\n");
#else
    return 33;
#endif
}

void camera_reset_follow(void)
{
    FUNCTION_MARKER(0x80024308, "SLUS_005.43");
    w_u32(0x8004EB30, 0);
    w_u32(0x8004EB54, 0);
    w_u32(0x8004EB58, 0);
}

void camera_set_target(uint32 object)
{
    FUNCTION_MARKER(0x80024328, "SLUS_005.43");
    w_u32(0x8004EB34, object);
}

uint32 camera_set_follow_distance(uint32 distance, uint32 smooth)
{
    uint32 flags;
    FUNCTION_MARKER(0x80024410, "SLUS_005.43");
    flags = r_u32(0x8004EB30);
    w_u32(0x8004EB64, distance);
    w_u32(0x8004EB68, smooth);
    flags |= 2;
    w_u32(0x8004EB30, flags);
    return flags;
}

static uint32 camera_set_position_body(uint32 x, uint32 y, uint32 z)
{
    uint32 camera;
    camera = r_u32(0x80040C94);
    w_u32(0x800311E0, x);
    w_u32(0x800311E4, y);
    w_u32(0x800311E8, z);
    w_u32(camera + 252, 0);
    w_u32(camera + 256, 0);
    w_u32(camera + 260, 0);
    return camera;
}

uint32 camera_set_position(uint32 vector)
{
    uint32 x, y, z;
    FUNCTION_MARKER(0x80024494, "SLUS_005.43");
    x = r_u32(vector);
    y = r_u32(vector + 4);
    z = r_u32(vector + 8);
    return camera_set_position_body(x, y, z);
}

uint32 camera_set_position_native(const VECTOR *vector)
{
    uint32 x, y, z;
    x = (uint32)vector->vx;
    y = (uint32)vector->vy;
    z = (uint32)vector->vz;
    return camera_set_position_body(x, y, z);
}

uint32 render_set_width(uint32 width)
{
    uint32 active;
    uint32 standard;
    FUNCTION_MARKER(0x80022B18, "SLUS_005.43");
    active = r_u32(0x800310A4);
    if (active)
        return active;
    standard = r_u32(0x800311C0);
    w_u32(0x8003119C, width);
    w_u32(0x800311B0, standard ? 180 : 150);
    width = r_u32(0x8003119C);
    standard = r_u32(0x800311B0);
    return render_init_view(width, standard, 200);
}

uint32 render_set_clear_color(uint32 red, uint32 green, uint32 blue)
{
    uint32 base;
    FUNCTION_MARKER(0x80022A3C, "SLUS_005.43");
    base = r_u32(0x800310A4);
    if (base)
        return base;
    base = r_u32(0x80042170);
    w_u8(base + 25, red);
    base = r_u32(0x80042170);
    w_u8(base + 26, green);
    base = r_u32(0x80042170);
    w_u8(base + 27, blue);
    base = r_u32(0x80042170);
    w_u8(base + 17933, red);
    base = r_u32(0x80042170);
    w_u8(base + 17934, green);
    base = r_u32(0x80042170);
    w_u8(base + 17935, blue);
    return base;
}
