#include "game_functions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

uint32 front_select_preview_configuration(void)
{
    uint32 first;
    uint32 second;
    uint32 tail;
    uint32 terminator;
    FUNCTION_MARKER(0x80090BD0, "FRONT.OVL");
    first = r_u32(0x80086930);
    second = r_u32(0x80086934);
    w_u32(0x800BC62C, first);
    w_u32(0x800BC630, second);
    tail = r_u16(0x80086938);
    terminator = r_u8(0x8008693A);
    w_u16(0x800BC634, tail);
    w_u8(0x800BC636, terminator);
    front_load_preview_configuration();
    w_u8(0x800BC628, 1);
    return 1;
}

uint32 front_allocate_model_tables(void)
{
    uint32 models;
    uint32 bounds;
    uint32 index;
    FUNCTION_MARKER(0x800911EC, "FRONT.OVL");
    models = heap_alloc_back(7315);
    w_u32(0x80046178, models);
    bounds = heap_alloc_back(2688);
    models = r_u32(0x80046178);
    w_u32(0x800BC6E0, bounds);
    for (index = 0; index < 55; ++index)
    {
        w_u32(models, 0);
        w_u32(models + 0x74, 0);
        w_u32(models + 4, 0);
        models += 132;
    }
    w_u32(0x800BC6DC, 0);
    return 0;
}

uint32 front_copy_view_arguments(uint32 destination, const uint32 *arguments, uint32 first, sint32 count)
{
    sint32 index;
    uint32 source;
    uint32 length;
    FUNCTION_MARKER(0x800919F8, "FRONT.OVL");
    for (index = 1; index < count; ++index)
    {
        source = arguments[first + (uint32)index];
        length = 0;
        while (r_u8(source + length))
            ++length;
        source = arguments[first + (uint32)index];
        strcpy((char *)psx_addr(destination, (size_t)length + 1), (const char *)psx_addr(source, (size_t)length + 1));
        if (index < count - 1)
        {
            destination += length;
            w_u8(destination++, ' ');
        }
    }
    return 0;
}

static sint32 front_view_integer(uint32 text)
{
    uint32 length = 0;
    while (r_u8(text + length))
        ++length;
    return string_to_integer((const char *)psx_addr(text, (size_t)length + 1));
}

uint32 front_view_begin_ship(void)
{
    uint32 count;
    FUNCTION_MARKER(0x80091AC4, "FRONT.OVL");
    count = r_u32(0x800BC6C8) + 1;
    w_u32(0x800BC6C8, count);
    return count;
}

uint32 front_view_end_ship(void)
{
    uint32 next;
    FUNCTION_MARKER(0x800923B0, "FRONT.OVL");
    next = r_u32(0x800FB104);
    if (next)
    {
        next = r_u32(0x800BC6D4) + 1148;
        w_u32(0x800BC6D4, next);
    }
    return next;
}

uint32 front_view_set_bundle(const uint32 *arguments, uint32 first)
{
    uint32 destination;
    uint32 source;
    uint32 length;
    uint32 enabled;
    FUNCTION_MARKER(0x80091AE4, "FRONT.OVL");
    enabled = r_u32(0x800FB104);
    if (!enabled)
        return enabled;
    destination = r_u32(0x800BC6D4);
    source = arguments[first + 1];
    length = 0;
    while (r_u8(source + length))
        ++length;
    strcpy((char *)psx_addr(destination, (size_t)length + 1), (const char *)psx_addr(source, (size_t)length + 1));
    destination = r_u32(0x800BC6D4);
    front_uppercase_text(destination);
    destination = r_u32(0x800BC6D4);
    length = 0;
    while (r_u8(destination + length))
        ++length;
    strcat((char *)psx_addr(destination, (size_t)length + 5), ".BND");
    return destination;
}

uint32 front_view_set_position(const uint32 *arguments, uint32 first)
{
    uint32 value;
    uint32 record;
    FUNCTION_MARKER(0x80091B58, "FRONT.OVL");
    value = r_u32(0x800FB104);
    if (!value)
        return value;
    value = (uint32)front_view_integer(arguments[first + 1]);
    record = r_u32(0x800BC6D4);
    w_u32(record + 0x28, value);
    value = (uint32)front_view_integer(arguments[first + 2]);
    record = r_u32(0x800BC6D4);
    w_u32(record + 0x2C, value);
    value = (uint32)front_view_integer(arguments[first + 3]);
    record = r_u32(0x800BC6D4);
    w_u32(record + 0x30, value);
    return value;
}

uint32 front_view_set_rotation(const uint32 *arguments, uint32 first)
{
    uint32 value;
    uint32 record;
    FUNCTION_MARKER(0x80091BE0, "FRONT.OVL");
    value = r_u32(0x800FB104);
    if (!value)
        return value;
    value = (uint32)front_view_integer(arguments[first + 1]);
    record = r_u32(0x800BC6D4);
    w_u32(record + 0x34, value);
    value = (uint32)front_view_integer(arguments[first + 2]);
    record = r_u32(0x800BC6D4);
    w_u32(record + 0x38, value);
    value = (uint32)front_view_integer(arguments[first + 3]);
    record = r_u32(0x800BC6D4);
    w_u32(record + 0x3C, value);
    return value;
}

uint32 front_view_set_length(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 text;
    uint32 value;
    uint32 enabled;
    FUNCTION_MARKER(0x800923E4, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    text = record + 0x44;
    if (!enabled)
        return record;
    front_copy_view_arguments(text, arguments, first, count);
    value = (uint32)front_view_integer(text);
    record = r_u32(0x800BC6D4);
    w_u32(record + 0x40, value);
    if (!value)
    {
        value = 1;
        w_u32(record + 0x40, value);
    }
    return value;
}

uint32 front_view_set_type(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091C60, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x6C, arguments, first, count);
    return record;
}

uint32 front_view_set_name(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091CA8, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x94, arguments, first, count);
    return record;
}

uint32 front_view_set_role(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091CF0, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0xBC, arguments, first, count);
    return record;
}

uint32 front_view_set_allegiance(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091D38, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0xE4, arguments, first, count);
    return record;
}

uint32 front_view_set_cruise_speed(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091D80, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x10C, arguments, first, count);
    return record;
}

uint32 front_view_set_drive_speed(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091DC8, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x134, arguments, first, count);
    return record;
}

uint32 front_view_set_acceleration(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091E10, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x15C, arguments, first, count);
    return record;
}

uint32 front_view_set_turn_rate(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091E58, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x184, arguments, first, count);
    return record;
}

uint32 front_view_set_hull(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091EA0, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x1AC, arguments, first, count);
    return record;
}

uint32 front_view_set_shield(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091EE8, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x1D4, arguments, first, count);
    return record;
}

uint32 front_view_set_sensor(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091F30, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x1FC, arguments, first, count);
    return record;
}

uint32 front_view_set_hardpoints(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091F78, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x224, arguments, first, count);
    return record;
}

uint32 front_view_set_weapons_grids(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80091FC0, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x24C, arguments, first, count);
    return record;
}

uint32 front_view_set_weapons_bays(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092008, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x274, arguments, first, count);
    return record;
}

uint32 front_view_set_primary_1(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092050, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x29C, arguments, first, count);
    return record;
}

uint32 front_view_set_primary_2(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092098, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x2C4, arguments, first, count);
    return record;
}

uint32 front_view_set_primary_3(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x800920E0, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x2EC, arguments, first, count);
    return record;
}

uint32 front_view_set_primary_4(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092128, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x314, arguments, first, count);
    return record;
}

uint32 front_view_set_primary_5(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092170, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x33C, arguments, first, count);
    return record;
}

uint32 front_view_set_primary_6(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x800921B8, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x364, arguments, first, count);
    return record;
}

uint32 front_view_set_secondary_1(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092200, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x38C, arguments, first, count);
    return record;
}

uint32 front_view_set_secondary_2(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092248, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x3B4, arguments, first, count);
    return record;
}

uint32 front_view_set_secondary_3(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092290, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x3DC, arguments, first, count);
    return record;
}

uint32 front_view_set_secondary_4(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x800922D8, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x404, arguments, first, count);
    return record;
}

uint32 front_view_set_secondary_5(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092320, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x42C, arguments, first, count);
    return record;
}

uint32 front_view_set_secondary_6(const uint32 *arguments, uint32 first, sint32 count)
{
    uint32 record;
    uint32 enabled;
    FUNCTION_MARKER(0x80092368, "FRONT.OVL");
    record = r_u32(0x800BC6D4);
    enabled = r_u32(0x800FB104);
    if (enabled)
        return front_copy_view_arguments(record + 0x454, arguments, first, count);
    return record;
}

static uint32 front_dispatch_view_command(uint32 target, const uint32 *arguments, uint32 first, sint32 count)
{
    switch (target)
    {
    case 0x80091AC4:
        return front_view_begin_ship();
    case 0x800923B0:
        return front_view_end_ship();
    case 0x80091AE4:
        return front_view_set_bundle(arguments, first);
    case 0x80091B50:
        return front_view_set_position(arguments, first);
    case 0x80091BD8:
        return front_view_set_rotation(arguments, first);
    case 0x800923E4:
        return front_view_set_length(arguments, first, count);
    case 0x80091C60:
        return front_view_set_type(arguments, first, count);
    case 0x80091CA8:
        return front_view_set_name(arguments, first, count);
    case 0x80091CF0:
        return front_view_set_role(arguments, first, count);
    case 0x80091D38:
        return front_view_set_allegiance(arguments, first, count);
    case 0x80091D80:
        return front_view_set_cruise_speed(arguments, first, count);
    case 0x80091DC8:
        return front_view_set_drive_speed(arguments, first, count);
    case 0x80091E10:
        return front_view_set_acceleration(arguments, first, count);
    case 0x80091E58:
        return front_view_set_turn_rate(arguments, first, count);
    case 0x80091EA0:
        return front_view_set_hull(arguments, first, count);
    case 0x80091EE8:
        return front_view_set_shield(arguments, first, count);
    case 0x80091F30:
        return front_view_set_sensor(arguments, first, count);
    case 0x80091F78:
        return front_view_set_hardpoints(arguments, first, count);
    case 0x80091FC0:
        return front_view_set_weapons_grids(arguments, first, count);
    case 0x80092008:
        return front_view_set_weapons_bays(arguments, first, count);
    case 0x80092050:
        return front_view_set_primary_1(arguments, first, count);
    case 0x80092098:
        return front_view_set_primary_2(arguments, first, count);
    case 0x800920E0:
        return front_view_set_primary_3(arguments, first, count);
    case 0x80092128:
        return front_view_set_primary_4(arguments, first, count);
    case 0x80092170:
        return front_view_set_primary_5(arguments, first, count);
    case 0x800921B8:
        return front_view_set_primary_6(arguments, first, count);
    case 0x80092200:
        return front_view_set_secondary_1(arguments, first, count);
    case 0x80092248:
        return front_view_set_secondary_2(arguments, first, count);
    case 0x80092290:
        return front_view_set_secondary_3(arguments, first, count);
    case 0x800922D8:
        return front_view_set_secondary_4(arguments, first, count);
    case 0x80092320:
        return front_view_set_secondary_5(arguments, first, count);
    case 0x80092368:
        return front_view_set_secondary_6(arguments, first, count);
    default:
        abort();
    }
}

uint32 front_parse_view_configuration(uint32 source, uint32 size)
{
    uint32 end;
    uint32 line;
    uint32 arguments[20];
    uint32 length;
    uint32 command;
    sint32 count;
    sint32 kind;
    char detail[128];
    FUNCTION_MARKER(0x8008FA94, "FRONT.OVL");
    end = source + size;
    line = heap_alloc_back(3000);
    if (!line)
        diagnostic_report(900, 0x800868FC, (intptr)0x8008690C);
    do
    {
        source = config_read_line(source, line);
        front_replace_text_byte(line, 9, ' ');
        if (r_u8(line) != ';')
        {
            count = (sint32)config_split_arguments(line, arguments);
            if (count > 0)
            {
                command = arguments[0];
                length = 0;
                while (r_u8(command + length))
                    ++length;
                kind = string_table_find((const char *)psx_addr(command, (size_t)length + 1), 0x800BC638);
                if (kind == -1)
                {
                    strcpy(detail, "ERROR PARSING : ");
                    length = 0;
                    while (r_u8(0x800BC62C + length))
                        ++length;
                    strcat(detail, (const char *)psx_addr(0x800BC62C, (size_t)length + 1));
                    diagnostic_report(1000, (intptr)detail, (intptr)arguments[0]);
                }
                else
                {
                    command = r_u32(0x800BC6E8 + ((uint32)kind << 2));
                    front_dispatch_view_command(command, arguments, 0, count);
                }
            }
        }
    } while (source < end);
    heap_free(line);
    return 1;
}

void front_load_preview_configuration(void)
{
    uint32 reuse;
    uint32 size;
    uint32 loaded;
    uint32 buffer;
    uint32 record;
    uint32 count;
    uint32 index;
    uint32 offset;
    uint32 stage;
    uint32 bounds;
    uint32 model_bounds;
    uint32 numerator;
    uint32 denominator;
    uint32 quotient;
    uint32 target;
    uint32 difference;
    uint32 high;
    uint32 step;
    uint32 speed;
    uint32 magnitude;
    sint32 campaign;
    FUNCTION_MARKER(0x80090C2C, "FRONT.OVL");
    reuse = r_u8(0x800BC628);
    w_u32(0x800BC6E4, 250);
    w_u32(0x8003119C, 320);
    w_u32(0x800F6F98, 8);
    w_u32(0x800F6FB8, 20);
    w_u32(0x800F6FBC, 265);
    w_u32(0x800F6FF0, 460);
    w_u32(0x800F6FF4, 390);
    w_u32(0x800F7010, 0);
    w_u8(0x800BC628, 0);
    if (reuse != 1)
    {
        w_u32(0x8010D0D8, 0);
        w_u32(0x800F700C, 0);
        w_u32(0x800FB104, 0);
        size = disc_file_size((const char *)psx_addr(0x800BC62C, 12));
        if (!size)
        {
            diagnostic_report(666, 0x8008693C, (intptr)0x8008690C);
            return;
        }
        buffer = heap_alloc_back(size);
        w_u32(0x800FB86C, buffer);
        if (!buffer)
        {
            diagnostic_report(666, 0x80086954, (intptr)0x8008690C);
            return;
        }
        loaded = resource_load_into((const char *)psx_addr(0x800BC62C, 12), buffer);
        if (loaded != size)
        {
            diagnostic_report(666, 0x80086980, (intptr)0x8008690C);
            return;
        }
        buffer = r_u32(0x800FB86C);
        w_u32(0x800BC6C8, 0);
        front_parse_view_configuration(buffer, loaded);
        count = r_u32(0x800BC6C8);
        record = heap_alloc_back(count * 1148);
        w_u32(0x800BC6D0, record);
        if (!record)
        {
            diagnostic_report(666, 0x800869A4, (intptr)0x8008690C);
            return;
        }
        count = r_u32(0x800BC6C8);
        w_u32(0x800BC6D4, record);
        index = 0;
        if ((sint32)count > 0)
        {
            do
            {
                record = r_u32(0x800BC6D4);
                memset(psx_addr(record, 1148), 0, 1148);
                record = r_u32(0x800BC6D4);
                count = r_u32(0x800BC6C8);
                ++index;
                w_u32(record + 0x40, 1);
                w_u32(0x800BC6D4, record + 1148);
            } while ((sint32)index < (sint32)count);
        }
        buffer = r_u32(0x800FB86C);
        record = r_u32(0x800BC6D0);
        w_u32(0x800BC6C8, 0);
        w_u32(0x800FB104, 1);
        w_u32(0x800BC6D4, record);
        front_parse_view_configuration(buffer, loaded);
        heap_free(r_u32(0x800FB86C));
        index = r_u32(0x800F700C);
        campaign = r_s16(0x8005960C);
        if ((sint32)r_u8(0x800BC524 + (index << 2)) >= campaign)
        {
            for (;;)
            {
                index = r_u32(0x800F700C);
                offset = index << 2;
                stage = r_u8(0x800BC524 + offset);
                if ((sint32)stage == campaign && (sint32)r_u8(0x800BC525 + offset) < r_s16(0x8005960E))
                    break;
                if (!stage && !r_u8(0x800BC525 + offset))
                    break;
                if ((sint32)stage == campaign && (sint32)r_u8(0x800BC525 + offset) == r_s16(0x8005960E) && !r_u8(0x800BC527 + offset))
                    break;
                if (r_u8(0x80011935) == 1)
                    break;
                index = r_u32(0x800F700C) + 1;
                w_u32(0x800F700C, index);
                stage = r_u8(0x800BC524 + (index << 2));
                if ((sint32)stage < r_s16(0x8005960C))
                    break;
            }
        }
        index = r_u32(0x800F700C);
        w_u32(0x8010D0D8, index);
        w_u32(0x800F6FA0, index);
        front_load_preview_models(0);
        w_u32(0x800F6FA4, 0);
        front_start_model_preview();
        w_u32(0x800F6FAC, 4096);
        w_u32(0x800F7010, 1);
        w_u32(0x800BC620, 1);
        w_u32(0x800BC624, 11);
    }
    bounds = r_u32(0x800BC6E0);
    numerator = r_u32(bounds + 0xC);
    numerator *= 250;
    denominator = r_u32(bounds + 8) << 2;
    if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
        abort();
    quotient = (uint32)((sint32)numerator / (sint32)denominator);
    index = r_u32(0x800F6FA0);
    record = (((index << 3) + index) << 5) - index;
    record = r_u32(0x800BC6D0) + (record << 2);
    denominator = r_u32(record + 0x40);
    numerator = quotient << 13;
    if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
        abort();
    target = (uint32)((sint32)numerator / (sint32)denominator);
    difference = target - r_u32(0x800F6FAC);
    index = r_u32(0x800F6FA4);
    w_u32(0x800F6FB0, target);
    model_bounds = bounds + (((index << 1) + index) << 4);
    high = (uint32)(((sint64)(sint32)difference * (sint32)0x88888889) >> 32);
    step = (uint32)((sint32)(high + difference) >> 5) - (uint32)((sint32)difference >> 31);
    w_u32(0x800F6FB4, step);
    speed = (uint32)r_s16(model_bounds + 0x16);
    w_u32(0x800F6FC0, 0);
    w_u32(0x800F6FC4, 0);
    w_u32(0x800F6FDC, 0xFFFFFFFE);
    w_u32(0x800F6FA8, speed);
    denominator = r_u32(record + 0x40);
    if (!denominator || (numerator == 0x80000000 && denominator == 0xFFFFFFFF))
        abort();
    target = (uint32)((sint32)numerator / (sint32)denominator);
    w_u32(0x800F6FEC, step);
    w_u32(0x800F6FE8, target);
    speed = (uint32)r_s16(model_bounds + 0x16);
    magnitude = (sint32)step < 0 ? 0 - step : step;
    w_u32(0x800F6FF8, 0);
    w_u32(0x800F6FFC, 0);
    w_u32(0x800F6FE0, speed);
    if ((sint32)magnitude > 0)
        sound_play_sample(8);
    w_u16(0x800F3116, 0);
}

uint32 front_copy_tmd(uint32 source, uint32 destination, sint32 size)
{
    uint32 header = destination + 8;
    uint32 first = 0;
    uint32 base;
    uint32 object;
    uint32 count;
    uint32 vertex;
    uint32 primitive;
    uint32 normal;
    uint32 index;
    sint32 word;
    sint32 words;
    FUNCTION_MARKER(0x80091254, "FRONT.OVL");
    words = size >> 2;
    for (word = 0; word < words; ++word)
    {
        vertex = r_u32(source);
        source += 4;
        w_u32(destination, vertex);
        destination += 4;
    }
    count = r_u16(header);
    base = header + 4;
    object = base;
    for (index = 0; index < count; ++index)
    {
        vertex = r_u32(object);
        primitive = r_u32(object + 16);
        w_u32(object, vertex + base);
        normal = r_u32(object + 8);
        w_u32(object + 16, primitive + base);
        w_u32(object + 8, normal + base);
        if (!index)
            first = object;
        object += 28;
    }
    return first;
}

uint32 front_relocate_tmd_texture(uint32 model, const TEX_INFO *info, uint32 texture_index)
{
    uint32 primitive;
    uint32 format;
    uint32 u;
    uint32 v;
    uint32 clut;
    uint32 page;
    uint32 opcode;
    uint32 index;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    sint32 count;
    sint32 tag;
    FUNCTION_MARKER(0x800912F0, "FRONT.OVL");
    primitive = r_u32(model + 16);
    format = info->mode & 3;
    u = ((uint32)(uint16)info->pixel.x & 63) << ((2 - format) & 31);
    v = (uint16)info->pixel.y & 255;
    clut = ((uint32)(sint32)info->clut.y << 6) + (uint32)((sint32)info->clut.x >> 4);
    page = (uint32)((sint32)info->pixel.x >> 6);
    page += (uint32)((sint32)info->pixel.y >> 8) << 4;
    page += format << 7;
    page += r_u32(0x800311C8) << 5;
    count = r_s32(model + 20);
    tag = (sint16)(texture_index | 0x8000);
    for (index = 0; (sint32)index < count; ++index)
    {
        opcode = r_u8(primitive + 3) & 0xFC;
        if ((opcode == 0x24 || opcode == 0x34 || opcode == 0x2C || opcode == 0x3C) && r_s16(primitive + 10) == tag)
        {
            first = r_u8(primitive + 4);
            second = r_u8(primitive + 8);
            w_u16(primitive + 6, clut);
            w_u16(primitive + 10, page);
            w_u8(primitive + 4, first + u);
            third = r_u8(primitive + 12);
            w_u8(primitive + 8, second + u);
            if (opcode == 0x2C || opcode == 0x3C)
            {
                fourth = r_u8(primitive + 16);
                w_u8(primitive + 12, third + u);
                first = r_u8(primitive + 5);
                w_u8(primitive + 16, fourth + u);
                second = r_u8(primitive + 9);
                w_u8(primitive + 5, first + v);
                third = r_u8(primitive + 13);
                w_u8(primitive + 9, second + v);
                fourth = r_u8(primitive + 17);
                w_u8(primitive + 13, third + v);
                w_u8(primitive + 17, fourth + v);
            }
            else
            {
                first = r_u8(primitive + 5);
                w_u8(primitive + 12, third + u);
                second = r_u8(primitive + 9);
                w_u8(primitive + 5, first + v);
                third = r_u8(primitive + 13);
                w_u8(primitive + 9, second + v);
                w_u8(primitive + 13, third + v);
            }
        }
        primitive += ((uint32)r_u8(primitive + 1) << 2) + 4;
    }
    return 1;
}

uint32 front_parse_model_bundle(uint32 source, uint32 first_model)
{
    TEX_INFO info;
    uint32 cursor;
    uint32 end = 0;
    uint32 chunk;
    uint32 kind;
    uint32 models = 0;
    uint32 texture_index = 0;
    uint32 allocation;
    uint32 descriptor;
    uint32 index;
    uint32 model;
    uint32 bounds;
    uint32 offset;
    FUNCTION_MARKER(0x800914BC, "FRONT.OVL");
    if (r_u32(source) == 0x59444F42)
        end = source + r_u32(source + 4) + 8;
    else
        diagnostic_report(120, 0x80086A48, (intptr)0x8008690C);
    cursor = source + 8;
    while (cursor < end)
    {
        chunk = cursor;
        kind = r_u32(chunk);
        cursor += 8;
        if (kind == 0x53444D54)
        {
            texture_index = 0;
            allocation = heap_alloc_back(r_u32(chunk + 4));
            descriptor = front_copy_tmd(cursor, allocation, r_s32(chunk + 4));
            index = first_model + models;
            ++models;
            offset = ((index << 5) + index) << 2;
            model = r_u32(0x80046178) + offset;
            bounds = r_u32(0x800BC6E0) + (((index << 1) + index) << 4);
            w_u32(model, descriptor);
            w_u32(model + 4, 0);
            front_build_model_bounds(model, bounds);
        }
        else if (kind == 0x534D4954)
        {
            texture_read_info(cursor + 4, &info);
            texture_place_info(&info, 2);
            texture_upload(cursor, &info, 2);
            for (index = 0; (sint32)index < (sint32)models; ++index)
            {
                model = first_model + index;
                offset = ((model << 5) + model) << 2;
                model = r_u32(0x80046178) + offset;
                front_relocate_tmd_texture(r_u32(model), &info, texture_index);
            }
            ++texture_index;
        }
        cursor += r_u32(chunk + 4);
    }
    if (!models)
        diagnostic_report(0, 0x80086A60, 0);
    return first_model + models;
}

static void front_bundle_load_error(const char *name)
{
#ifndef NDEBUG
    int error = errno ? errno : EIO;
    fprintf(stderr, "Unable to load model bundle: %s (%s)\n", name, strerror(error));
#endif
    abort();
}

uint32 front_load_model_bundle(const char *name)
{
    uint32 size;
    uint32 buffer;
    uint32 first;
    uint32 next;
    FUNCTION_MARKER(0x800916D0, "FRONT.OVL");
    size = disc_file_size(name);
    if (!size)
        front_bundle_load_error(name);
    buffer = heap_alloc_front(size);
    if (resource_load_into(name, buffer) != size)
    {
        heap_free(buffer);
        front_bundle_load_error(name);
    }
    first = r_u32(0x800BC6DC);
    next = front_parse_model_bundle(buffer, first);
    w_u32(0x800BC6DC, next);
    heap_free(buffer);
    return first;
}

uint32 front_load_slot_bundle(uint32 slot)
{
    uint32 offset;
    uint32 selected;
    uint32 record;
    uint32 size;
    uint32 buffer;
    uint32 first;
    uint32 next;
    uint32 bounds;
    uint32 value;
    sint32 length;
    const char *name;
    FUNCTION_MARKER(0x80091740, "FRONT.OVL");
    offset = slot * 56;
    selected = r_u32(0x800F6FA0 + offset);
    record = r_u32(0x800BC6D0);
    record += ((((selected << 3) + selected) << 5) - selected) << 2;
    name = (const char *)psx_addr(record, 40);
    size = disc_file_size(name);
    if (!size)
        front_bundle_load_error(name);
    buffer = heap_alloc_front(size);
    if (resource_load_into(name, buffer) != size)
    {
        heap_free(buffer);
        front_bundle_load_error(name);
    }
    first = r_u32(0x800BC6DC);
    next = front_parse_model_bundle(buffer, first);
    w_u32(0x800BC6DC, next);
    heap_free(buffer);
    selected = r_u32(0x800F6FA0 + offset);
    record = ((((selected << 3) + selected) << 5) - selected) << 2;
    record += r_u32(0x800BC6D0);
    length = r_s32(record + 0x40);
    bounds = r_u32(0x800BC6E0) + (((first << 1) + first) << 4);
    w_u16(bounds + 0x16, 30);
    while (length > 0)
    {
        value = r_u16(bounds + 0x16);
        length >>= 1;
        w_u16(bounds + 0x16, value - 2);
    }
    return first;
}

uint32 front_load_preview_models(uint32 slot)
{
    uint32 row;
    sint32 column;
    uint32 base;
    uint32 index;
    uint32 first;
    uint32 selected;
    uint32 model;
    uint32 mesh;
    FUNCTION_MARKER(0x80091864, "FRONT.OVL");
    w_u32(0x80031454, 0);
    w_u32(0x800F6FDC, 0xFFFFFFFF);
    w_u32(0x800F6FA4, 0xFFFFFFFF);
    for (row = 0; row < 64; ++row)
    {
        base = 0x80057B50 + row * 64;
        for (column = 31; column >= 0; --column)
            w_u16(base + (uint32)column * 2, 0);
    }
    for (index = 0; index < 6; ++index)
        w_u16(0x80042946 + index * 2, 640);
    for (index = 0; index < 10; ++index)
    {
        table_clear_column(index);
        table_clear_column(index + 16);
    }
    for (index = 21; index < 28; ++index)
        table_clear_column(index);
    front_allocate_model_tables();
    base = r_u32(0x80046178);
    for (index = 0; index < 55; ++index)
    {
        w_u32(base, 0);
        w_u32(base + 0x74, 0);
        base += 132;
    }
    first = front_load_slot_bundle(slot);
    selected = r_u32(0x800F6FA0);
    w_u32(0x800BC6D8, first);
    selected = r_u8(0x800BC526 + (selected << 2));
    model = r_u32(0x80046178) + (((selected << 5) + selected) << 2);
    mesh = mesh_import_primitives(r_u32(model));
    w_u32(0x800BC61C, mesh);
    return 0;
}

void front_viewer_error(void)
{
    FUNCTION_MARKER(0x80092458, "FRONT.OVL");
    diagnostic_report(100, 0x80086A78, (intptr)0x8008690C);
}

static void front_commit_rotation(uint32 destination, const MATRIX *matrix)
{
    uint32 row;
    uint32 column;
    for (row = 0; row < 3; ++row)
        for (column = 0; column < 3; ++column)
            w_u16(destination + row * 6 + column * 2, (uint16)matrix->m[row][column]);
}

void front_apply_object_rotation(uint32 object)
{
    MATRIX first;
    MATRIX second;
    MATRIX *matrix;
    uint32 angle;
    FUNCTION_MARKER(0x80092488, "FRONT.OVL");
    angle = r_u32(object + 0x20);
    matrix = (MATRIX *)psx_addr(object + 0x11C, 20);
    if (angle)
    {
        if (r_u32(object + 0x24))
        {
            if (r_u32(object + 0x28))
            {
                matrix_rotate_y(&first, matrix, 0 - angle);
                matrix_rotate_x(&second, &first, r_u32(object + 0x24));
                matrix_rotate_z(matrix, &second, 0 - r_u32(object + 0x28));
            }
            else
            {
                matrix_rotate_y(&first, matrix, 0 - angle);
                matrix_rotate_x(matrix, &first, r_u32(object + 0x24));
            }
        }
        else
        {
            if (r_u32(object + 0x28))
            {
                matrix_rotate_y(&first, matrix, 0 - angle);
                matrix_rotate_z(matrix, &first, 0 - r_u32(object + 0x28));
            }
            else
            {
                matrix_rotate_y(&first, matrix, 0 - angle);
                front_commit_rotation(object + 0x11C, &first);
            }
        }
    }
    else
    {
        angle = r_u32(object + 0x24);
        if (angle)
        {
            if (r_u32(object + 0x28))
            {
                matrix_rotate_x(&first, matrix, angle);
                matrix_rotate_z(matrix, &first, 0 - r_u32(object + 0x28));
            }
            else
            {
                matrix_rotate_x(&first, matrix, angle);
                front_commit_rotation(object + 0x11C, &first);
            }
        }
        else
        {
            angle = r_u32(object + 0x28);
            if (angle)
            {
                matrix_rotate_z(&first, matrix, 0 - angle);
                front_commit_rotation(object + 0x11C, &first);
            }
        }
    }
}

uint32 front_update_viewer_objects(void)
{
    uint32 object;
    uint32 value;
    uint32 callback;
    uint32 rotated;
    uint32 slot;
    uint32 offset;
    uint32 bounds_offset;
    uint32 bounds;
    uint32 record_index;
    uint32 record;
    uint32 first;
    uint32 second;
    sint32 model_index;
    FUNCTION_MARKER(0x80092610, "FRONT.OVL");
    object = r_u32(0x8005367C);
    while (object)
    {
        value = r_u8(object + 0x26C) - 1;
        w_u8(object + 0x26C, value);
        if (!(value & 255))
        {
            rotated = 0;
            callback = r_u32(object + 0x40);
            psx_dispatch_guest_call(callback, 1, object, 0, 0);
            w_u8(object + 0x26C, r_u8(object + 0x26B));
            if (r_u32(object + 0x70) & 0x100000)
            {
                if (r_u32(object + 0x20) || r_u32(object + 0x24) || r_u32(object + 0x28))
                {
                    front_apply_object_rotation(object);
                    rotated = 1;
                    w_u32(object + 0x28, 0);
                    w_u32(object + 0x24, 0);
                    w_u32(object + 0x20, 0);
                }
                value = r_u16(object + 0x24C) - 1;
                w_u16(object + 0x24C, value);
                if ((sint16)value <= 0 && rotated == 1)
                {
                    matrix_normalize(object + 0x11C, 1);
                    value = (uint32)random_between(30, 40);
                    w_u16(object + 0x24C, value);
                }
            }
            else
            {
                first = r_u32(object + 0x20);
                second = r_u32(object + 0x2C);
                value = first != second;
                if (!value)
                {
                    first = r_u32(object + 0x24);
                    second = r_u32(object + 0x30);
                    value = first != second;
                }
                if (!value)
                {
                    first = r_u32(object + 0x28);
                    second = r_u32(object + 0x34);
                    value = first != second;
                }
                if (value)
                {
                    matrix_set_identity(object + 0x11C);
                    front_apply_object_rotation(object);
                }
            }
        }
        object = r_u32(object + 0x1D4);
    }
    for (slot = 0; slot < 2; ++slot)
    {
        offset = slot * 56;
        model_index = r_s32(0x800F6FA4 + offset);
        if (model_index < 0)
            continue;
        bounds_offset = (((uint32)model_index << 1) + (uint32)model_index) << 4;
        object = r_u32(0x800F6F9C + offset);
        bounds = r_u32(0x800BC6E0);
        record_index = r_u32(0x800F6FA0 + offset);
        bounds += bounds_offset;
        ApplyMatrix((MATRIX *)psx_addr(object + 0x11C, 20), (SVECTOR *)psx_addr(bounds + 0x18, 8), (VECTOR *)psx_addr(bounds + 0x20, 12));
        record = (((record_index << 3) + record_index) << 5) - record_index;
        record = r_u32(0x800BC6D0) + (record << 2);
        bounds = r_u32(0x800BC6E0) + bounds_offset;
        first = r_u32(record + 0x28);
        second = r_u32(bounds + 0x20);
        w_u32(object + 8, first + second);
        first = r_u32(record + 0x2C);
        second = r_u32(bounds + 0x24);
        w_u32(object + 0xC, first + second);
        first = r_u32(record + 0x30);
        second = r_u32(bounds + 0x28);
        w_u32(object + 0x10, first + second);
    }
    return object_flush_removed();
}

uint32 front_draw_viewer_objects(void)
{
    uint32 object;
    uint32 table;
    uint32 active_table;
    uint32 tag;
    uint32 packet;
    uint32 callback;
    uint32 x;
    uint32 y;
    uint32 z;
    FUNCTION_MARKER(0x80092878, "FRONT.OVL");
    object = r_u32(0x8005367C);
    table = r_u32(0x801132BC);
    active_table = r_u32(0x801132BC);
    tag = r_u32(table + 0x7CC);
    packet = r_u32(0x801132B8);
    w_u32(0x800421AC, active_table);
    w_u32(0x80040690, packet);
    w_u32(0x8004217C, tag);
    while (object)
    {
        callback = r_u32(object + 0x3C);
        psx_dispatch_guest_call(callback, 1, object, 0, 0);
        x = r_u32(object + 0x20);
        y = r_u32(object + 0x24);
        z = r_u32(object + 0x28);
        w_u32(object + 0x2C, x);
        w_u32(object + 0x30, y);
        w_u32(object + 0x34, z);
        object = r_u32(object + 0x1D4);
    }
    packet = r_u32(0x80040690);
    w_u32(0x801132B8, packet);
    return packet;
}

uint32 front_draw_selected_mesh(void)
{
    uint32 model_index;
    uint32 bounds;
    uint32 table;
    uint32 tag;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 offset;
    uint32 packet;
    uint32 mesh;
    uint32 mode;
    FUNCTION_MARKER(0x80092928, "FRONT.OVL");
    if (!r_u8(0x800FB940))
        return 0;
    model_index = r_u32(0x800F6FA4);
    w_u32(0x8001024C, 300);
    w_u32(0x80010250, 285);
    system_set_projection_offset(300, 285);
    system_set_projection_distance(r_s32(0x800BC6E4));
    bounds = r_u32(0x800BC6E0) + (((model_index << 1) + model_index) << 4);
    table = r_u32(0x801132BC);
    tag = r_u32(table + 0x7CC);
    x = r_u32(bounds + 0x20);
    w_u32(0x8010CD9C, x);
    y = r_u32(bounds + 0x24);
    table = r_u32(0x801132BC);
    packet = r_u32(0x801132B8);
    w_u32(0x8010CDA0, y);
    offset = r_u32(0x800FB0FC);
    z = r_u32(bounds + 0x28);
    w_u32(0x8004217C, tag);
    w_u32(0x800421AC, table);
    w_u32(0x80040690, packet);
    w_u32(0x8010CDA4, offset + z);
    SetTransMatrix((MATRIX *)psx_addr(0x8010CD88, sizeof(MATRIX)));
    mesh = r_u32(0x800BC61C);
    mode = r_u32(0x800FB940);
    mesh_render_adaptive(mesh, mode);
    packet = r_u32(0x80040690);
    x = r_u32(0x80031198);
    y = r_u32(0x800311B4);
    w_u32(0x801132B8, packet);
    w_u32(0x8001024C, x);
    w_u32(0x80010250, y);
    system_set_projection_offset((sint32)x, (sint32)y);
    system_set_projection_distance(r_s32(0x800313F8));
    return packet;
}

uint32 front_draw_cached_text(uint32 identifier)
{
    uint32 descriptor;
    uint32 index;
    uint32 count;
    uint32 end;
    uint32 offset;
    uint32 bank;
    uint32 packet;
    uint32 table;
    FUNCTION_MARKER(0x800B69AC, "FRONT.OVL");
    offset = identifier << 3;
    descriptor = r_u32(0x8010D114) + offset;
    index = r_u32(descriptor);
    count = r_u32(descriptor + 4);
    end = index + count;
    packet = index * 20;
    while ((sint32)index < (sint32)end)
    {
        bank = r_u32(0x8010D100);
        descriptor = r_u32(0x8010D0F8 + (bank << 2));
        table = r_u32(0x801132BC);
        AddPrim(psx_addr(table, 4), psx_addr(descriptor + packet, 4));
        descriptor = r_u32(0x8010D114) + offset;
        ++index;
        count = r_u32(descriptor);
        end = count + r_u32(descriptor + 4);
        packet += 20;
    }
    bank = r_u32(0x8010D100) ^ 1;
    w_u32(0x8010D100, bank);
    return bank;
}

void front_draw_text_native(const char *text, uint32 x, uint32 y, uint32 color, sint32 blend)
{
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
    FUNCTION_MARKER(0x80097118, "FRONT.OVL");
    character = (uint8)*text;
    if (character & 128)
    {
        front_draw_wide_text_native(text, x, y, color);
        return;
    }
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
            graph_type = GetGraphType();
            shift = (2 - (uint32)depth) & 31;
            if (graph_type == 1 || GetGraphType() == 2)
            {
                atlas_y = r_u16(atlas + 2);
                atlas_x = r_u16(atlas);
                page = (((uint32)depth & 3) << 9) | (((uint32)blend & 3) << 7) | ((atlas_y & 0x300) >> 3) | ((atlas_x & 0x3FF) >> 6);
            }
            else
            {
                atlas_y = r_u16(atlas + 2);
                atlas_x = r_u16(atlas);
                page = (((uint32)depth & 3) << 7) | (((uint32)blend & 3) << 5) | ((atlas_y >> 4) & 0x10) | ((atlas_x & 0x3FF) >> 6) | ((atlas_y & 0x200) << 2);
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
            w_u8(packet + 4, color);
            w_u8(packet + 5, color >> 8);
            w_u8(packet + 6, color >> 16);
            if (blend > 0)
                w_u8(packet + 7, r_u8(packet + 7) | 2);
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

static const char *front_viewer_text_pointer(uint32 text)
{
    uint32 length = 0;
    if (r_u8(text) & 128)
    {
        while (r_u16(text + length))
            length += 2;
        return (const char *)psx_addr(text, (size_t)length + 2);
    }
    while (r_u8(text + length))
        ++length;
    return (const char *)psx_addr(text, (size_t)length + 1);
}

static void front_draw_viewer_string(uint32 text, uint32 x, uint32 y)
{
    front_draw_text_native(front_viewer_text_pointer(text), x, y, 0x808080, 0);
}

static void front_draw_ship_values(uint32 labels, uint32 x, uint32 y)
{
    static const uint32 fields[5] = {0x44, 0x1D4, 0x1AC, 0x1FC, 0x10C};
    static const char *formats[5] = {"%s meters", "%s su", "%s hu", "%s sru", "%s slu"};
    char detail[104];
    uint32 index;
    uint32 text;
    uint32 value_x;
    for (index = 0; index < 5; ++index)
    {
        if (index)
            y += r_u32(0x800BC618);
        front_draw_cached_text(r_u32(labels + index * 4));
        text = r_u32(0x800BC6D4) + fields[index];
        sprintf(detail, formats[index], (const char *)psx_addr(text, 40));
        value_x = x + r_u32(0x800BC608);
        front_draw_text_native(detail, value_x, y, 0x808080, 0);
    }
    text = r_u32(0x800BC6D4);
    if (r_u8(text + 0x24C))
    {
        y += r_u32(0x800BC618);
        front_draw_cached_text(r_u32(labels + 20));
        text = r_u32(0x800BC6D4) + 0x24C;
        value_x = x + r_u32(0x800BC608);
        front_draw_viewer_string(text, value_x, y);
    }
    text = r_u32(0x800BC6D4);
    if (r_u8(text + 0x274))
    {
        y += r_u32(0x800BC618);
        front_draw_cached_text(r_u32(labels + 24));
        text = r_u32(0x800BC6D4) + 0x274;
        value_x = x + r_u32(0x800BC608);
        front_draw_viewer_string(text, value_x, y);
    }
}

void front_draw_ship_statistics(void)
{
    uint32 selected;
    uint32 offset;
    uint32 record;
    uint32 state;
    uint32 text;
    uint32 y;
    FUNCTION_MARKER(0x80092A8C, "FRONT.OVL");
    if (r_s32(0x800F6F98) < 8 && !r_u8(0x800BC628))
        return;
    selected = r_u32(0x800F700C);
    w_u32(0x800FB870, 83);
    offset = (((selected << 3) + selected) << 5) - selected;
    record = r_u32(0x800BC6D0) + (offset << 2);
    w_u32(0x800BC6D4, record);
    front_draw_viewer_string(record + 0x94, 24, 290);
    state = r_u32(0x800BC620);
    if ((state == 0 || state - 5 < 2) && r_s32(0x800F6FA4) >= 0)
    {
        text = r_u32(0x800BC6D4) + 0xE4;
        y = 290 + r_u32(0x800BC618);
        front_draw_viewer_string(text, 24, y);
        front_draw_ship_values(0x800F703C, 195, 105);
    }
    else
    {
        state = r_u32(0x800BC620);
        if (state == 0 || state - 5 < 2)
        {
            text = r_u32(0x800F26D0);
            y = 290 + r_u32(0x800BC618);
            front_draw_viewer_string(text, 24, y);
        }
    }
    selected = r_u32(0x800F700C);
    offset = (((selected << 3) + selected) << 5) - selected;
    record = r_u32(0x800BC6D0) + (offset << 2) + 1148;
    w_u32(0x800BC6D4, record);
    front_draw_viewer_string(record + 0x94, 464, 192);
    state = r_u32(0x800BC620);
    if ((state == 0 || state - 5 < 2) && r_s32(0x800F6FDC) >= 0)
    {
        text = r_u32(0x800BC6D4) + 0xE4;
        y = 192 + r_u32(0x800BC618);
        front_draw_viewer_string(text, 464, y);
        front_draw_ship_values(0x800F7064, 265, 256);
    }
    else
    {
        state = r_u32(0x800BC620);
        if (state == 0 || state - 5 < 2)
        {
            text = r_u32(0x800F26D0);
            y = 192 + r_u32(0x800BC618);
            front_draw_viewer_string(text, 464, y);
        }
    }
}

void front_draw_ship_details(void)
{
    static const uint32 fields[8] = {0x44, 0x10C, 0x1D4, 0x134, 0x1AC, 0x15C, 0x1FC, 0x184};
    static const char *formats[8] = {"%s meters", "%s slu", "%s su", "%s slu", "%s hu", "%s slups", "%s sru", "%s dps"};
    char detail[104];
    uint32 x;
    uint32 y;
    uint32 selected;
    uint32 offset;
    uint32 record;
    uint32 state;
    uint32 index;
    uint32 value_x;
    uint32 text;
    FUNCTION_MARKER(0x800931C0, "FRONT.OVL");
    if (r_s32(0x800F6F98) < 8 && !r_u8(0x800BC628))
        return;
    y = r_u32(0x800BC610);
    selected = r_u32(0x800F700C);
    x = r_u32(0x800BC60C);
    w_u32(0x800FB870, 83);
    offset = (((selected << 3) + selected) << 5) - selected;
    record = r_u32(0x800BC6D0) + (offset << 2);
    w_u32(0x800BC6D4, record);
    front_draw_viewer_string(record + 0xBC, x, y);
    state = r_u32(0x800BC620);
    if (state != 0 && state - 5 >= 2)
        return;
    y += r_u32(0x800BC618);
    text = r_u32(0x800BC6D4) + 0x94;
    front_draw_viewer_string(text, x, y);
    y += r_u32(0x800BC618);
    text = r_u32(0x800BC6D4) + 0xE4;
    front_draw_viewer_string(text, x, y);
    y += r_u32(0x800BC618) << 1;
    for (index = 0; index < 8; ++index)
    {
        if (index && !(index & 1))
            y += r_u32(0x800BC618);
        front_draw_cached_text(r_u32(0x800F7014 + index * 4));
        text = r_u32(0x800BC6D4) + fields[index];
        sprintf(detail, formats[index], (const char *)psx_addr(text, 40));
        if (index & 1)
            value_x = x + r_u32(0x800BC604) + 170;
        else
            value_x = x + r_u32(0x800BC600);
        front_draw_text_native(detail, value_x, y, 0x808080, 0);
    }
    text = r_u32(0x800F2700);
    y += r_u32(0x800BC618) << 1;
    front_draw_viewer_string(text, x, y);
    for (index = 0; index < 6; ++index)
    {
        y += r_u32(0x800BC618);
        text = r_u32(0x800BC6D4) + 0x29C + index * 40;
        front_draw_viewer_string(text, x + 42, y);
        text = r_u32(0x800BC6D4) + 0x38C + index * 40;
        front_draw_viewer_string(text, x + 170, y);
    }
}

void front_draw_distance_scales(void)
{
    char label[104];
    uint32 slot;
    uint32 offset;
    uint32 target;
    uint32 current;
    uint32 step;
    uint32 difference;
    uint32 magnitude;
    uint32 increment;
    uint32 interval;
    uint32 index;
    uint32 position;
    uint32 number;
    uint32 packet;
    uint32 table;
    uint32 x;
    uint32 y;
    uint32 length;
    uint32 height;
    uint32 red;
    uint32 green;
    uint32 blue;
    FUNCTION_MARKER(0x800937D8, "FRONT.OVL");
    w_u32(0x800FB870, 0);
    for (slot = 0; slot < 2; ++slot)
    {
        offset = slot * 56;
        if (r_u32(0x800F6FA4 + offset) == 0xFFFFFFFE)
            continue;
        target = r_u32(0x800F6FB0 + offset);
        current = r_u32(0x800F6FAC + offset);
        step = r_u32(0x800F6FB4 + offset);
        difference = target - current;
        if ((sint32)difference < 0)
            difference = 0 - difference;
        magnitude = step;
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        if ((sint32)difference < (sint32)magnitude)
            w_u32(0x800F6FAC + offset, target);
        else
            w_u32(0x800F6FAC + offset, current + step);
        increment = r_u32(0x800F6FAC + offset);
        interval = 2;
        while ((sint32)increment < 0x6276)
        {
            increment *= 5;
            interval *= 5;
        }
        index = 0;
        position = 0;
        number = 0;
        do
        {
            packet = r_u32(0x801132B8);
            w_u8(packet + 3, 3);
            w_u8(packet + 7, 0x40);
            if (!((sint32)index % 5))
            {
                red = interval == 50 || interval == 10 ? 80 : 0;
                green = interval == 50 ? 0 : 80;
                blue = 0;
                height = 8;
            }
            else
            {
                red = interval == 50 ? 80 : 0;
                green = 80;
                blue = interval == 50 || interval == 10 ? 0 : 80;
                height = 5;
            }
            w_u8(packet + 4, red);
            w_u8(packet + 5, green);
            w_u8(packet + 6, blue);
            x = r_u32(0x800F6FB8 + offset) + (uint32)((sint32)position >> 12);
            w_u16(packet + 8, x);
            y = r_u32(0x800F6FBC + offset);
            w_u16(packet + 12, x);
            w_u16(packet + 10, y);
            y = r_u32(0x800F6FBC + offset);
            w_u16(packet + 14, y + height);
            table = r_u32(0x801132BC);
            AddPrim(psx_addr(table, 4), psx_addr(packet, 16));
            w_u32(0x801132B8, packet + 16);
            if (!((sint32)index % 5))
            {
                sprintf(label, "%d", (sint32)number);
                length = (uint32)strlen(label) * 5;
                length = (uint32)((sint32)(length + (length >> 31)) >> 1);
                y = r_u32(0x800F6FBC + offset) + height + 5;
                front_draw_colored_text_native(label, x - length, y, 0x8000);
            }
            ++index;
            position += increment;
            number += interval;
        } while ((sint32)position <= 0x9FFFF);
    }
}

void front_draw_viewer_backgrounds(void)
{
    uint32 slot;
    uint32 offset;
    uint32 value;
    uint32 red;
    uint32 green;
    uint32 blue;
    uint32 width;
    uint32 height;
    uint32 packet;
    uint32 table;
    uint32 x;
    uint32 y;
    uint32 half_width;
    uint32 page;
    FUNCTION_MARKER(0x80093B24, "FRONT.OVL");
    for (slot = 0; slot < 2; ++slot)
    {
        offset = slot * 56;
        if (r_u32(0x800F6FA4 + offset) == 0xFFFFFFFE)
            continue;
        if (r_s32(0x800F6FC0 + offset) < 160)
        {
            w_u8(0x800F6FCA + offset, 255);
            w_u8(0x800F6FC9 + offset, 255);
            w_u8(0x800F6FC8 + offset, 255);
        }
        else
        {
            value = r_u8(0x800F6FC8 + offset);
            w_u8(0x800F6FC8 + offset, value > 16 ? value - 16 : 0);
            value = r_u8(0x800F6FC9 + offset);
            w_u8(0x800F6FC9 + offset, value > 16 ? value - 16 : 0);
            value = r_u8(0x800F6FCA + offset);
            w_u8(0x800F6FCA + offset, value > 48 ? value - 14 : 0);
        }
        width = r_u32(0x800F6FC4);
        red = r_u8(0x800F6FC8 + offset);
        green = r_u8(0x800F6FC9 + offset);
        blue = r_u8(0x800F6FCA + offset);
        half_width = (uint32)((sint32)width / 2);
        if (red)
        {
            packet = r_u32(0x801132B8);
            w_u8(packet + 3, 3);
            w_u8(packet + 7, 0x60);
            w_u8(packet + 4, red);
            w_u8(packet + 5, green);
            w_u8(packet + 6, blue);
            x = r_u32(0x800F6FCC + offset) - half_width;
            w_u16(packet + 8, x);
            height = r_u32(0x800F6FC0 + offset);
            y = r_u32(0x800F6FD0 + offset) - (uint32)((sint32)height >> 1);
            w_u16(packet + 10, y);
            if (r_s32(0x800F6FC0 + offset) < 8)
                height = r_u32(0x800F6FC0 + offset);
            else
                height = 8;
            w_u16(packet + 12, width);
            w_u16(packet + 14, height);
            w_u8(packet + 7, r_u8(packet + 7) | 2);
            value = r_u32(0x801132A8);
            table = r_u32(0x801132BC) + (value << 2) - 4;
            AddPrim(psx_addr(table, 4), psx_addr(packet, 16));
            packet += 16;
            w_u32(0x801132B8, packet);
            w_u8(packet + 3, 3);
            w_u8(packet + 7, 0x60);
            w_u8(packet + 4, red);
            w_u8(packet + 5, green);
            w_u8(packet + 6, blue);
            if (r_s32(0x800F6FC0 + offset) < 8)
            {
                x = r_u32(0x800F6FCC + offset) - half_width;
                w_u16(packet + 8, x);
                y = r_u32(0x800F6FD0 + offset);
                w_u16(packet + 12, width);
                w_u16(packet + 10, y);
                height = r_u32(0x800F6FC0 + offset);
                w_u16(packet + 14, height);
            }
            else
            {
                x = r_u32(0x800F6FCC + offset) - half_width;
                w_u16(packet + 8, x);
                y = r_u32(0x800F6FD0 + offset);
                height = r_u32(0x800F6FC0 + offset);
                w_u16(packet + 12, width);
                w_u16(packet + 14, 8);
                w_u16(packet + 10, y + (uint32)((sint32)height >> 1) - 8);
            }
            w_u8(packet + 7, r_u8(packet + 7) | 2);
            value = r_u32(0x801132A8);
            table = r_u32(0x801132BC) + (value << 2) - 4;
            AddPrim(psx_addr(table, 4), psx_addr(packet, 16));
            w_u32(0x801132B8, packet + 16);
        }
        packet = r_u32(0x801132B8);
        w_u8(packet + 3, 3);
        w_u8(packet + 4, 16);
        w_u8(packet + 5, 16);
        w_u8(packet + 6, 48);
        w_u8(packet + 7, 0x60);
        x = r_u32(0x800F6FCC + offset) - half_width;
        w_u16(packet + 8, x);
        height = r_u32(0x800F6FC0 + offset);
        y = r_u32(0x800F6FD0 + offset) - (uint32)((sint32)height >> 1);
        value = r_u8(packet + 7);
        w_u16(packet + 12, width);
        w_u16(packet + 10, y);
        height = r_u32(0x800F6FC0 + offset);
        w_u8(packet + 7, value | 2);
        w_u16(packet + 14, height);
        value = r_u32(0x801132A8);
        table = r_u32(0x801132BC) + (value << 2) - 4;
        AddPrim(psx_addr(table, 4), psx_addr(packet, 16));
        w_u32(0x801132B8, packet + 16);
    }
    packet = r_u32(0x801132B8);
    page = GetTPage(0, 0, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 0, (uint16)page, NULL);
    value = r_u32(0x801132A8);
    table = r_u32(0x801132BC) + (value << 2) - 4;
    AddPrim(psx_addr(table, 4), psx_addr(packet, 12));
    w_u32(0x801132B8, packet + 12);
}

void front_update_ship_view(void)
{
    uint32 model;
    uint32 state;
    uint32 selected;
    uint32 bounds;
    uint32 record;
    uint32 record_offset;
    uint32 model_offset;
    uint32 timer;
    uint32 value;
    uint32 movement;
    uint32 index;
    uint32 entry;
    uint32 sectors;
    uint32 asset;
    sint32 extent;
    sint32 half_extent;
    FUNCTION_MARKER(0x80094038, "FRONT.OVL");
    model = r_u32(0x800F6FA4);
    state = r_u32(0x800BC620);
    selected = r_u32(0x800F6FA0);
    if (state == 1)
    {
        w_u32(0x800F6FCC, 300);
        w_u32(0x800F6FD0, 285);
        w_u32(0x800F6FC4, 360);
        bounds = r_u32(0x800BC6E0) + model * 48;
        extent = r_s16(bounds + 6);
        record_offset = selected * 1148;
        record = r_u32(0x800BC6D0) + record_offset;
        value = r_u32(record + 0x30) - (uint32)(sint32)(sint16)(extent / 30);
        w_u32(record + 0x30, value);
        extent = r_s16(bounds + 6);
        w_u8(0x800FB940, 0);
        w_u8(0x800FB941, 0);
        w_u8(0x800FB942, 0);
        half_extent = r_s16(bounds + 6) >> 1;
        w_u32(0x800FB0FC, (uint32)(extent >> 1));
        if (r_s32(record + 0x30) < half_extent)
        {
            w_u32(0x800BC620, 2);
            w_u32(0x800BC774, 15);
        }
    }
    else if (state == 2)
    {
        timer = r_u32(0x800BC774) - 1;
        w_u32(0x800BC774, timer);
        if (!timer)
        {
            w_u32(0x800BC774, 15);
            w_u32(0x800BC620, 3);
            w_u8(0x800FB940, 255);
            w_u8(0x800FB941, 255);
            w_u8(0x800FB942, 255);
        }
    }
    else if (state == 3)
    {
        timer = r_u32(0x800BC774);
        movement = (uint32)((sint32)(timer * 200) / 15);
        w_u32(0x800F6FCC, movement + 100);
        w_u32(0x800F6FC4, movement + 160);
        movement = (uint32)((sint32)(timer * 100) / 15);
        w_u32(0x800F6FD0, movement + 185);
        model_offset = model * 48;
        bounds = r_u32(0x800BC6E0) + model_offset;
        extent = r_s16(bounds + 6);
        record_offset = selected * 1148;
        record = r_u32(0x800BC6D0) + record_offset;
        value = r_u32(record + 0x30) + (uint32)(sint32)(sint16)(extent / 30);
        w_u32(record + 0x30, value);
        extent = r_s16(bounds + 6);
        value = r_u8(0x800FB940);
        w_u32(0x800FB0FC, (uint32)(extent >> 1));
        if (value >= 32)
            w_u8(0x800FB940, value - 15);
        value = r_u8(0x800FB941);
        if (value >= 131)
            w_u8(0x800FB941, value - 15);
        value = r_u8(0x800FB942);
        if (value >= 167)
            w_u8(0x800FB942, value - 6);
        extent = r_s16(bounds + 6);
        if (extent < r_s32(record + 0x30))
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
            bounds = r_u32(0x800BC6E0) + model_offset;
            record = r_u32(0x800BC6D0) + record_offset;
            extent = r_s16(bounds + 6);
            w_u32(record + 0x30, (uint32)extent);
        }
        timer = r_u32(0x800BC774);
        if ((sint32)timer > 0)
            w_u32(0x800BC774, timer - 1);
        value = r_u32(0x800F6FC0) + 11;
        w_u32(0x800F6FC0, value);
        if ((sint32)value > 160)
            w_u32(0x800F6FC0, 160);
    }
    if (r_s16(0x800B737C) == 1 || r_u32(0x800BC620) - 5 < 2)
    {
        value = r_u32(0x800F6FC0) - 12;
        w_u32(0x800F6FC0, value);
        if ((sint32)value < 0)
            w_u32(0x800F6FC0, 0);
    }
    state = r_u32(0x800BC620);
    if (state < 2 || state - 2 < 2 || state - 5 < 2)
    {
        front_update_viewer_objects();
        camera_dispatch();
        if (r_s32(0x800F6F98) > 0)
        {
            front_draw_viewer_objects();
            if (r_s32(0x800F6F98) > 0)
                front_draw_selected_mesh();
        }
    }
    w_u32(0x800FB870, 83);
    front_draw_ship_details();
    front_draw_distance_scales();
    front_draw_viewer_backgrounds();
}
