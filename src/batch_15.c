#include "game_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void front_draw_celestial_marker(const sint16 *point, uint32 depth, uint32 color_index)
{
    uint32 color, packet, value, first, second, shift, table;
    sint32 format;
    uint16 page, clut;
    FUNCTION_MARKER(0x800AF2C8, "FRONT.OVL");
    color = r_u32(0x800BE624u + (color_index << 2));
    packet = r_u32(0x801132B8u);
    format = r_s16(0x800508ECu);
    w_u8(packet + 3u, 9u);
    w_u8(packet + 7u, 0x2Cu);
    w_u8(packet + 4u, color);
    w_u8(packet + 5u, color >> 8);
    w_u8(packet + 6u, color >> 16);
    w_u16(packet + 8u, (uint32)(uint16)point[0] - 12u);
    w_u16(packet + 10u, (uint32)(uint16)point[1] - 12u);
    w_u16(packet + 16u, (uint32)(uint16)point[0] + 12u);
    w_u16(packet + 18u, (uint32)(uint16)point[1] - 12u);
    w_u16(packet + 24u, (uint32)(uint16)point[0] - 12u);
    w_u16(packet + 26u, (uint32)(uint16)point[1] + 12u);
    w_u16(packet + 32u, (uint32)(uint16)point[0] + 12u);
    w_u16(packet + 34u, (uint32)(uint16)point[1] + 12u);
    shift = (2u - (uint32)format) & 31u;
    page = GetTPage(format, 1, r_s16(0x800508E0u), r_s16(0x800508E2u));
    w_u16(packet + 22u, page);
    clut = GetClut(r_s16(0x800508E4u), r_s16(0x800508E6u));
    value = r_u8(packet + 7u);
    w_u16(packet + 14u, clut);
    w_u8(packet + 7u, value | 2u);
    w_u8(packet + 12u, (r_u16(0x800508E0u) & 63u) << shift);
    w_u8(packet + 13u, r_u16(0x800508E2u));
    value = r_u8(packet + 12u);
    first = (uint32)(sint32)r_s16(0x800508E8u);
    w_u8(packet + 28u, value);
    value = r_u8(packet + 12u);
    value += first << shift;
    first = r_u8(packet + 13u);
    w_u8(packet + 20u, value - 1u);
    value = r_u8(packet + 13u);
    w_u8(packet + 21u, first);
    first = r_u8(0x800508EAu);
    w_u8(packet + 29u, value + first - 1u);
    first = r_u8(packet + 20u);
    second = r_u8(packet + 29u);
    w_u8(packet + 36u, first);
    w_u8(packet + 37u, second);
    table = r_u32(0x801132BCu);
    AddPrim(psx_addr(table + (depth << 2), 4u), psx_addr(packet, 40u));
    w_u32(0x801132B8u, packet + 40u);
}

void front_draw_celestial_ring(uint32 ring)
{
    MATRIX temporary;
    VECTOR rotated;
    sint16 point[2];
    sint32 angle, screen_xy, projection, flags, depth;
    uint32 value, object, table, captured_table, link, packet, radius;
    uint32 first, second, callback, index, count, bank;
    FUNCTION_MARKER(0x800AF4E0, "FRONT.OVL");
    w_u16(0x8003506Eu, 0u);
    w_u16(0x8003506Cu, 0u);
    w_u16(0x8003506Au, 0u);
    w_u16(0x80035066u, 0u);
    w_u16(0x80035064u, 0u);
    w_u16(0x80035062u, 0u);
    w_u16(0x80035070u, 512u);
    w_u16(0x80035068u, 512u);
    w_u16(0x80035060u, 512u);
    w_u32(0x80035074u, 0u);
    w_u32(0x80035078u, 0u);
    w_u32(0x8003507Cu, 0u);
    matrix_set_identity(0x800FB9F8u);
    value = r_u16(0x8010CD70u);
    matrix_rotate_x(&temporary, (MATRIX *)psx_addr(0x800FB9F8u, 20u), value & 0xFFFu);
    angle = r_s32(ring + 0xDACu) / 4096;
    matrix_rotate_y((MATRIX *)psx_addr(0x800FB9F8u, 20u), &temporary, (uint32)(angle % 4096));
    value = r_u32(ring + 0xDACu);
    first = r_u32(ring + 0xDB0u);
    object = r_u32(ring + 0xDA4u);
    w_u32(ring + 0xDACu, value + first);
    if (object != 0u) {
        ApplyMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)),
            (SVECTOR *)psx_addr(ring + 4u, 8u), &rotated);
        object = r_u32(ring + 0xDA4u);
        w_u32(object + 8u, (uint32)rotated.vx);
        object = r_u32(ring + 0xDA4u);
        w_u32(object + 12u, (uint32)rotated.vy);
        object = r_u32(ring + 0xDA4u);
        w_u32(object + 16u, (uint32)rotated.vz);
        table = r_u32(0x801132BCu);
        captured_table = r_u32(0x801132BCu);
        radius = r_u32(ring + 0xDB4u);
        link = r_u32(table + 0x7CCu);
        packet = r_u32(0x801132B8u);
        w_u32(0x800421ACu, captured_table);
        w_u32(0x80040690u, packet);
        w_u32(0x8004217Cu, link);
        object = r_u32(ring + 0xDA4u);
        front_configure_preview_lighting(object, radius == 0u ? 1u : 0u);
        object = r_u32(ring + 0xDA4u);
        w_u16(object + 0x12Au, 0u);
        w_u16(object + 0x128u, 0u);
        w_u16(object + 0x126u, 0u);
        w_u16(object + 0x122u, 0u);
        w_u16(object + 0x120u, 0u);
        w_u16(object + 0x11Eu, 0u);
        object = r_u32(ring + 0xDA4u);
        value = r_u16(ring + 0xDA8u);
        w_u16(object + 0x12Cu, value);
        w_u16(object + 0x124u, value);
        w_u16(object + 0x11Cu, value);
        object = r_u32(ring + 0xDA4u);
        front_apply_object_rotation(object);
        object = r_u32(ring + 0xDA4u);
        callback = r_u32(object + 0x3Cu);
        psx_dispatch_guest_call(callback, 1, object, 0u, 0u);
        object = r_u32(ring + 0xDA4u);
        value = r_u32(object + 32u);
        w_u32(object + 44u, value);
        object = r_u32(ring + 0xDA4u);
        value = r_u32(object + 36u);
        w_u32(object + 48u, value);
        object = r_u32(ring + 0xDA4u);
        packet = r_u32(0x80040690u);
        value = r_u32(object + 40u);
        w_u32(object + 52u, value);
        w_u32(0x801132B8u, packet);
        w_u32(0x800FB870u, 0x53u);
        SetRotMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)));
        SetTransMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)));
        RotTransPers((SVECTOR *)psx_addr(ring + 4u, 8u), &screen_xy, &projection, &flags);
        point[0] = (sint16)screen_xy;
        point[1] = (sint16)((uint32)screen_xy >> 16);
    } else {
        w_u32(0x800FB870u, 0x53u);
        SetRotMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)));
        SetTransMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)));
        depth = RotTransPers((SVECTOR *)psx_addr(ring + 4u, 8u), &screen_xy, &projection, &flags);
        point[0] = (sint16)screen_xy;
        point[1] = (sint16)((uint32)screen_xy >> 16);
        value = r_u32(ring + 0xDBCu);
        front_draw_celestial_marker(point, (uint32)(depth / 32), 0u - value);
    }
    value = r_u32(ring + 0xDC0u);
    front_draw_colored_text(value, (uint32)(sint32)point[0], (uint32)(sint32)point[1], 0x808080u);
    value = r_u32(ring + 0xDB8u);
    bank = value == 0u ? 0x204u : 0x7D4u;
    count = r_u32(ring);
    index = 0u;
    while ((sint32)index < (sint32)count) {
        first = r_u32(ring + 0x184u + index * 4u);
        second = r_u32(ring + 0x188u + index * 4u);
        depth = (sint32)(first + second) / 32;
        packet = ring + bank + index * 48u;
        table = r_u32(0x801132BCu);
        AddPrim(psx_addr(table + ((uint32)depth << 2), 4u), psx_addr(packet, 16u));
        count = r_u32(ring);
        ++index;
    }
    w_u32(ring + 0xDB8u, value == 0u ? 1u : 0u);
}

uint32 front_count_celestial_descendants(uint32 node)
{
    uint32 offset, processed, count, child;
    FUNCTION_MARKER(0x800AF8F8, "FRONT.OVL");
    count = r_u32(node + 0x4Cu);
    processed = 0u;
    offset = 0u;
    if ((sint32)count > 0) {
        offset = 1u;
        do {
            child = node + offset * 80u;
            if (r_s32(child + 0x4Cu) > 0)
                offset += front_count_celestial_descendants(child);
            count = r_u32(node + 0x4Cu);
            ++processed;
            ++offset;
        } while ((sint32)processed < (sint32)count);
        --offset;
    }
    return offset;
}

void front_initialize_celestial_overview(void)
{
    uint32 max_size, max_period, base, node, value, root, size;
    uint32 period, phase, radius, index, rings, ring, count, offset, scale;
    FUNCTION_MARKER(0x800AF98C, "FRONT.OVL");
    w_u16(0x8010CD70u, 400u);
    w_u32(0x8010CD68u, 0u);
    w_u16(0x8010CD72u, 0xFA88u);
    max_size = 1u;
    matrix_set_identity(0x800FB9F8u);
    w_u32(0x800FBA0Cu, 0xFFFFFA88u);
    w_u32(0x800FBA10u, 0xFFFFFE16u);
    w_u32(0x800FBA14u, 8100u);
    max_period = 1u;
    front_create_preview_camera();
    w_u32(0x8001024Cu, 320u);
    w_u32(0x80010250u, 240u);
    SetGeomOffset(320, 240);
    SetGeomScreen(400);
    node = r_u32(0x800FB9F4u);
    if (r_u8(node) != 'F') {
        do {
            value = r_u32(node + 0x2Cu);
            if ((sint32)max_size < (sint32)value)
                max_size = value;
            value = r_u32(node + 0x30u);
            if ((sint32)max_period < (sint32)value)
                max_period = value;
            node += 80u;
        } while (r_u8(node) != 'F');
    }
    max_period = SquareRoot0((sint32)((max_period + 1u) << 12));
    if ((sint32)max_size <= 65535) {
        root = SquareRoot0((sint32)(max_size << 12));
        max_size = (uint32)((sint32)root / 4096);
    } else {
        root = SquareRoot0((sint32)max_size);
        max_size = (uint32)((sint32)root / 64);
    }
    rings = 0u;
    base = r_u32(0x800FB9F4u);
    index = 0u;
    ring = 0x800FBA18u;
    if (r_u8(base) != 'F') {
        do {
            offset = index * 80u;
            node = base + offset;
            value = r_u32(node + 0x30u);
            root = SquareRoot0((sint32)(value << 12));
            period = (uint32)((sint32)root / 4096);
            base = r_u32(0x800FB9F4u);
            value = r_u32(base + offset + 0x2Cu);
            ++period;
            if ((sint32)value <= 65535) {
                root = SquareRoot0((sint32)(value << 12));
                size = (uint32)((sint32)root / 4096);
            } else {
                root = SquareRoot0((sint32)value);
                size = (uint32)((sint32)root / 64);
            }
            if (period == 0u || (period == 0xFFFFFFFFu && max_period == 0x80000000u))
                abort();
            phase = (uint32)((sint32)max_period / (sint32)period);
            value = size << 12;
            if (max_size == 0u || (max_size == 0xFFFFFFFFu && value == 0x80000000u))
                abort();
            radius = (uint32)((sint32)value / (sint32)max_size);
            base = r_u32(0x800FB9F4u);
            node = base + offset;
            base = r_u32(0x800FB9F4u);
            scale = r_u32(base + offset + 0x48u);
            front_initialize_celestial_ring(ring, radius, 0x206060u, scale, phase, node);
            base = r_u32(0x800FB9F4u);
            node = base + offset;
            if (r_s32(node + 0x4Cu) > 0 && r_s32(node + 0x2Cu) > 0) {
                base = r_u32(0x800FB9F4u);
                index += front_count_celestial_descendants(base + offset);
            }
            ++index;
            ring += 0xDC4u;
            base = r_u32(0x800FB9F4u);
            node = base + index * 80u;
            value = r_u8(node);
            ++rings;
        } while (value != 'F');
    }
    count = rings;
    w_u32(0x8010CD68u, count);
}

void front_initialize_selected_celestial_system(void)
{
    uint32 max_size, max_period, selected, other, value, root, node;
    uint32 count, processed, index, radius, phase, scale, scale_base;
    uint32 period, size, ring, rings, offset, definition;
    FUNCTION_MARKER(0x800AFD54, "FRONT.OVL");
    w_u16(0x8010CD70u, 400u);
    w_u32(0x8010CD68u, 0u);
    w_u16(0x8010CD72u, 0xFA88u);
    max_size = 1u;
    matrix_set_identity(0x800FB9F8u);
    w_u32(0x800FBA0Cu, 0xFFFFFA88u);
    w_u32(0x800FBA10u, 0xFFFFFE16u);
    w_u32(0x800FBA14u, 8100u);
    max_period = 1u;
    front_create_preview_camera();
    w_u32(0x8001024Cu, 320u);
    w_u32(0x80010250u, 240u);
    SetGeomOffset(320, 240);
    SetGeomScreen(400);
    selected = r_u32(0x800FB978u);
    count = r_u32(selected + 0x4Cu);
    processed = 0u;
    if ((sint32)count > 0) {
        other = r_u32(0x800FB9F4u) + 80u;
        node = selected + 80u;
        do {
            value = r_u32(node + 0x2Cu);
            if ((sint32)max_size < (sint32)value)
                max_size = value;
            value = r_u32(other + 0x30u);
            if ((sint32)max_period < (sint32)value)
                max_period = r_u32(node + 0x30u);
            other += 80u;
            ++processed;
            node += 80u;
        } while ((sint32)processed < (sint32)count);
    }
    max_period = SquareRoot0((sint32)((max_period + 1u) << 12));
    if ((sint32)max_size <= 65535) {
        root = SquareRoot0((sint32)(max_size << 12));
        max_size = (uint32)((sint32)root / 4096);
    } else {
        root = SquareRoot0((sint32)max_size);
        max_size = (uint32)((sint32)root / 64);
    }
    selected = r_u32(0x800FB978u);
    definition = r_u32(0x800FB978u);
    value = r_u32(selected + 0x48u);
    scale_base = (uint32)((sint32)((value + 1u) << 8) / 20);
    front_initialize_celestial_ring(0x800FBA18u, 0u, 0x206060u, 20u, 0u, definition);
    value = r_u32(0x800FC7BCu);
    w_u32(value + 0xD4u, 0x00FFFFFFu);
    index = 1u;
    selected = r_u32(0x800FB978u);
    rings = 1u;
    count = r_u32(selected + 0x4Cu);
    processed = 0u;
    ring = 0x800FC7DCu;
    if ((sint32)count > 0) {
        do {
            offset = index * 80u;
            value = r_u32(selected + offset + 0x30u);
            root = SquareRoot0((sint32)(value << 12));
            period = (uint32)((sint32)root / 4096);
            selected = r_u32(0x800FB978u);
            value = r_u32(selected + offset + 0x2Cu);
            ++period;
            if ((sint32)value <= 65535) {
                root = SquareRoot0((sint32)(value << 12));
                size = (uint32)((sint32)root / 4096);
            } else {
                root = SquareRoot0((sint32)value);
                size = (uint32)((sint32)root / 64);
            }
            value = size << 12;
            if (max_size == 0u || (max_size == 0xFFFFFFFFu && value == 0x80000000u))
                abort();
            radius = (uint32)((sint32)value / (sint32)max_size);
            if (period == 0u || (period == 0xFFFFFFFFu && max_period == 0x80000000u))
                abort();
            phase = (uint32)((sint32)max_period / (sint32)period);
            selected = r_u32(0x800FB978u);
            value = r_u32(selected + offset + 0x48u);
            value = (value + 1u) << 8;
            if (scale_base == 0u || (scale_base == 0xFFFFFFFFu && value == 0x80000000u))
                abort();
            scale = (uint32)((sint32)value / (sint32)scale_base);
            selected = r_u32(0x800FB978u);
            definition = selected + offset;
            front_initialize_celestial_ring(ring, radius, 0x206060u, scale, phase, definition);
            selected = r_u32(0x800FB978u);
            w_u32(0x800FB97Cu + processed * 4u, selected + offset);
            selected = r_u32(0x800FB978u);
            w_u32(0x800FB954u, rings);
            node = selected + offset;
            if (r_s32(node + 0x4Cu) > 0 && r_s32(node + 0x2Cu) > 0) {
                selected = r_u32(0x800FB978u);
                index += front_count_celestial_descendants(selected + offset);
            }
            ++index;
            ring += 0xDC4u;
            selected = r_u32(0x800FB978u);
            count = r_u32(selected + 0x4Cu);
            ++processed;
            ++rings;
        } while ((sint32)processed < (sint32)count);
    }
    w_u32(0x8010CD68u, rings);
}

uint32 front_index_celestial_systems(void)
{
    uint32 fresh, address, count, processed, offset, value, base, index;
    FUNCTION_MARKER(0x800B023C, "FRONT.OVL");
    fresh = 1u;
    w_u32(0x800FB94Cu, 0u);
    address = 0x800F2FB6u;
    count = 15u;
    do {
        w_u8(address, 2u);
        --address;
        --count;
    } while (count != 0u);
    count = r_u32(0x800FB950u);
    processed = 0u;
    offset = 0u;
    while ((sint32)processed < (sint32)count) {
        if (fresh != 0u) {
            index = r_u32(0x800FB94Cu);
            base = r_u32(0x800FB948u);
            w_u32(0x800FB9CCu + (index << 2), base + offset);
            index = r_u32(0x800FB94Cu);
            w_u8(0x800F2FA8u + index, 0u);
            index = r_u32(0x800FB94Cu);
            fresh = 0u;
            w_u32(0x800FB94Cu, index + 1u);
        }
        base = r_u32(0x800FB948u);
        value = r_u8(base + offset);
        if (value == 'F')
            fresh = 1u;
        count = r_u32(0x800FB950u);
        ++processed;
        offset += 80u;
    }
    value = r_u32(0x800FB9CCu);
    w_u32(0x800FB9F4u, value);
    return value;
}

uint32 front_load_celestial_systems(void)
{
    uint32 bytes, buffer;
    FUNCTION_MARKER(0x800B033C, "FRONT.OVL");
    w_u32(0x800FB9F4u, 0u);
    w_u32(0x800FB9CCu, 0u);
    bytes = disc_file_size("ALL.SLR");
    w_u32(0x800FB950u, bytes / 80u);
    buffer = heap_alloc_back(bytes);
    w_u32(0x800FB948u, buffer);
    front_update_campaign_access_flags();
    buffer = r_u32(0x800FB948u);
    resource_load_into("ALL.SLR", buffer);
    return front_index_celestial_systems();
}

void front_initialize_system_database(void)
{
    uint32 table, destination, words[3], word, address, offset, index;
    uint32 aligned, shift, value, record, base, sectors, asset;
    FUNCTION_MARKER(0x800B03E0, "FRONT.OVL");
    table = r_u32(0x800FB0F4u);
    w_u32(0x800313DCu, 3u);
    w_u16(0x8003506Eu, 0u);
    w_u16(0x8003506Cu, 0u);
    w_u16(0x8003506Au, 0u);
    w_u16(0x80035066u, 0u);
    w_u16(0x80035064u, 0u);
    w_u16(0x80035062u, 0u);
    w_u16(0x80035070u, 512u);
    w_u16(0x80035068u, 512u);
    w_u16(0x80035060u, 512u);
    w_u32(0x80035074u, 0u);
    w_u32(0x80035078u, 0u);
    w_u32(0x8003507Cu, 0u);
    destination = r_u32(table);
    words[0] = r_u32(0x80086EDCu);
    words[1] = r_u32(0x80086EE0u);
    words[2] = r_u32(0x80086EE4u);
    for (index = 0u; index < 4u; ++index) {
        word = index < 3u ? words[index] : r_u32(0x80086EE8u);
        address = destination + (index << 2);
        offset = address & 3u;
        if (offset == 0u) {
            w_u32(address, word);
            w_u32(address, word);
        } else {
            aligned = (address + 3u) & ~3u;
            shift = ((address + 3u) & 3u) * 8u;
            value = r_u32(aligned);
            w_u32(aligned, (value & (0xFFFFFF00u << shift)) | (word >> (24u - shift)));
            aligned = address & ~3u;
            shift = offset * 8u;
            value = r_u32(aligned);
            w_u32(aligned, (value & (0x00FFFFFFu >> (24u - shift))) | (word << shift));
        }
    }
    if (r_u32(0x800BE620u) != 0u) {
        w_u32(0x800BE620u, 0u);
        w_u16(0x8010CD74u, 0u);
    }
    if (r_s16(0x8010CD74u) != 0) {
        w_u16(0x8010CD74u, 0u);
        front_index_celestial_systems();
    } else {
        front_prepare_control_model_bundle("GAVPLAN.BND");
        front_load_celestial_systems();
        front_reset_menu_packet_lists();
        disc_wait_stream();
        VSync(0);
        if (r_u32(0x800FB9F4u) != 0u) {
            front_initialize_celestial_overview();
            record = r_u32(0x800BE578u);
            w_u32(0x8010CD7Cu, record);
            value = r_u32(record);
            if ((sint32)value >= 0) {
                base = r_u32(0x800F708Cu);
                record = base + value * 24u;
                sectors = r_u32(record + 20u);
                asset = r_u32(record + 28u);
                stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
            }
        }
    }
    if (r_u8(0x80011935u) == 0u) {
        for (index = 0u; index < 5u; ++index) {
            if (r_u8(0x8010D56Cu + index) == 0u)
                w_u8(0x800F2FA8u + index, 1u);
        }
    }
}

void front_update_system_database(void)
{
    uint32 selection, current, selected, offset, record, value, base;
    uint32 sectors, asset, index, count, ring;
    FUNCTION_MARKER(0x800B0638, "FRONT.OVL");
    selection = (uint32)(sint32)r_s16(0x8010D0DCu);
    current = r_u32(0x800FB9F4u);
    offset = selection << 2;
    selected = r_u32(0x800FB950u + offset);
    if (current != selected) {
        w_u32(0x800FB9F4u, selected);
        record = r_u32(0x800BE4FCu + offset);
        w_u32(0x8010CD7Cu, record);
        record = r_u32(0x8010CD7Cu);
        value = r_u32(record);
        if ((sint32)value >= 0) {
            base = r_u32(0x800F708Cu);
            record = base + value * 24u;
            sectors = r_u32(record + 20u);
            asset = r_u32(record + 28u);
            stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
        }
        if (r_u32(0x800FB9F4u) == 0u) {
            value = r_u32(0x8010CD6Cu);
            w_u32(0x8010CD6Cu, value + 1u);
            front_draw_viewer_controls();
            return;
        }
        front_initialize_celestial_overview();
    }
    if (r_u32(0x800FB9F4u) != 0u) {
        camera_dispatch();
        count = r_u32(0x8010CD68u);
        index = 0u;
        ring = 0x800FBA18u;
        while ((sint32)index < (sint32)count) {
            front_draw_celestial_ring(ring);
            count = r_u32(0x8010CD68u);
            ++index;
            ring += 0xDC4u;
        }
    }
    value = r_u32(0x8010CD6Cu);
    w_u32(0x8010CD6Cu, value + 1u);
    front_draw_viewer_controls();
}

void front_draw_system_database(void)
{
    uint32 value, count, index, ring;
    FUNCTION_MARKER(0x800B078C, "FRONT.OVL");
    if (r_u32(0x800FB9F4u) != 0u) {
        camera_dispatch();
        count = r_u32(0x8010CD68u);
        index = 0u;
        ring = 0x800FBA18u;
        while ((sint32)index < (sint32)count) {
            front_draw_celestial_ring(ring);
            count = r_u32(0x8010CD68u);
            ++index;
            ring += 0xDC4u;
        }
    }
    value = r_u32(0x8010CD6Cu);
    w_u32(0x8010CD6Cu, value + 1u);
    front_draw_viewer_controls();
}

void front_copy_selected_system_name(void)
{
    uint32 selection, table, source, length;
    FUNCTION_MARKER(0x800B084C, "FRONT.OVL");
    w_u16(0x8010CD74u, 1u);
    w_u16(0x8010CD76u, 1u);
    selection = (uint32)(sint32)r_s16(0x8010D0DCu);
    table = r_u32(0x800FB0F4u);
    source = r_u32(table + (selection << 2) - 0x78u);
    length = 0u;
    while (r_u8(source + length) != 0u)
        ++length;
    strcpy((char *)psx_addr(0x800FB958u, length + 1u), (const char *)psx_addr(source, length + 1u));
    front_uppercase_text(0x800FB958u);
}

uint32 front_retain_system_database(void)
{
    FUNCTION_MARKER(0x800B0890, "FRONT.OVL");
    w_u16(0x8010CD74u, 1u);
    w_u16(0x8010CD76u, 0u);
    return 1u;
}

uint32 front_free_system_database(void)
{
    uint32 buffer;
    FUNCTION_MARKER(0x800B08AC, "FRONT.OVL");
    if (r_s16(0x8010CD74u) == 0) {
        object_update_all();
        object_initialize_pool();
        front_free_preview_models();
        buffer = r_u32(0x800FB948u);
        heap_free(buffer);
    }
    return stream_stop();
}

uint32 front_restore_system_database_title(void)
{
    uint32 table, destination, length;
    FUNCTION_MARKER(0x800B0908, "FRONT.OVL");
    table = r_u32(0x800FB0F4u);
    w_u16(0x8010CD78u, 0u);
    destination = r_u32(table);
    length = 0u;
    while (r_u8(0x800FB958u + length) != 0u)
        ++length;
    strcpy((char *)psx_addr(destination, length + 1u), (const char *)psx_addr(0x800FB958u, length + 1u));
    return destination;
}

uint32 front_close_system_body_menu(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800B093C, "FRONT.OVL");
    value = (uint32)(sint32)r_s16(0x8010CD78u);
    if (value == 0u)
        return stream_stop();
    w_u16(0x8010CD78u, 0u);
    return value;
}

void front_initialize_system_body_menu(void)
{
    uint32 head, address, count, length, table, destination, source;
    uint32 index, processed, offset, node;
    FUNCTION_MARKER(0x800B0988, "FRONT.OVL");
    head = r_u32(0x800FB9F4u);
    w_u16(0x8010CD7Au, 0u);
    if (head != 0u && r_s16(0x8010CD76u) == 0) {
        front_initialize_celestial_overview();
        w_u16(0x8010CD76u, 0u);
    }
    address = 0x800F2FB6u;
    count = 15u;
    do {
        w_u8(address, 2u);
        --address;
        --count;
    } while (count != 0u);
    table = r_u32(0x800FB0F4u);
    destination = r_u32(table);
    length = 0u;
    while (r_u8(0x800FB958u + length) != 0u)
        ++length;
    strcpy((char *)psx_addr(destination, length + 1u), (const char *)psx_addr(0x800FB958u, length + 1u));
    head = r_u32(0x800FB9F4u);
    index = 1u;
    processed = 0u;
    if (head == 0u)
        return;
    if (r_u8(head + 80u) != 'F') {
        do {
            table = r_u32(0x800FB0F4u);
            head = r_u32(0x800FB9F4u);
            destination = r_u32(table + processed * 4u + 4u);
            offset = index * 80u;
            source = head + offset + 4u;
            length = 0u;
            while (r_u8(source + length) != 0u)
                ++length;
            strcpy((char *)psx_addr(destination, length + 1u), (const char *)psx_addr(source, length + 1u));
            w_u8(0x800F2FA8u + processed, 0u);
            head = r_u32(0x800FB9F4u);
            w_u32(0x800FB97Cu + processed * 4u, head + offset);
            head = r_u32(0x800FB9F4u);
            node = head + offset;
            if (r_s32(node + 0x4Cu) > 0 && r_s32(node + 0x2Cu) > 0) {
                head = r_u32(0x800FB9F4u);
                index += front_count_celestial_descendants(head + offset);
            }
            ++index;
            head = r_u32(0x800FB9F4u);
            node = head + index * 80u;
            count = r_u8(node);
            ++processed;
        } while (count != 'F');
    }
    head = r_u32(0x800FB9F4u);
    source = head + 4u;
    length = 0u;
    while (r_u8(source + length) != 0u)
        ++length;
    if (strcmp((const char *)psx_addr(source, length + 1u), "Helios") == 0) {
        --processed;
        w_u8(0x800F2FA8u + processed, 2u);
    }
    w_u32(0x800FB954u, processed);
}

void front_draw_system_selection_link(uint32 row_spacing)
{
    MATRIX temporary;
    sint16 points[6];
    sint32 screen_xy, projection, flags, angle, left, projected_x;
    uint32 selection, ring, value, panel, top, row_offset;
    FUNCTION_MARKER(0x800B0B60, "FRONT.OVL");
    selection = (uint32)(sint32)r_s16(0x8010D0DCu);
    ring = 0x800E1D20u + selection * 0xDC4u;
    matrix_set_identity(0x800FB9F8u);
    value = r_u16(0x8010CD70u);
    matrix_rotate_x(&temporary, (MATRIX *)psx_addr(0x800FB9F8u, 20u), value & 0xFFFu);
    angle = r_s32(ring + 0xDACu) / 4096;
    matrix_rotate_y((MATRIX *)psx_addr(0x800FB9F8u, 20u), &temporary, (uint32)(angle % 4096));
    SetRotMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)));
    SetTransMatrix((MATRIX *)psx_addr(0x800FB9F8u, sizeof(MATRIX)));
    RotTransPers((SVECTOR *)psx_addr(ring + 4u, 8u), &screen_xy, &projection, &flags);
    selection = (uint32)(sint32)r_s16(0x8010D0DCu);
    row_offset = row_spacing * (selection - 31u);
    panel = r_u32(0x800F3138u);
    value = r_u16(panel + 24u);
    points[0] = (sint16)value;
    top = r_u16(panel + 26u);
    top += row_offset + 74u;
    projected_x = (sint16)screen_xy;
    left = (sint16)value;
    points[1] = (sint16)top;
    if (left < projected_x) {
        points[2] = (sint16)projected_x;
        points[3] = (sint16)top;
        points[4] = (sint16)screen_xy;
        points[5] = (sint16)((uint32)screen_xy >> 16);
        front_draw_embossed_line(points, 0x808080u, 0u);
        front_draw_embossed_line(points + 2, 0x808080u, 0u);
    }
}

void front_update_system_body_menu(void)
{
    uint32 count, index, ring, selection, record, selected, value;
    FUNCTION_MARKER(0x800B0D74, "FRONT.OVL");
    if (r_u32(0x800FB9F4u) != 0u) {
        camera_dispatch();
        count = r_u32(0x8010CD68u);
        if ((sint32)count > 0) {
            index = 0u;
            ring = 0x800FBA18u;
            do {
                front_draw_celestial_ring(ring);
                count = r_u32(0x8010CD68u);
                ++index;
                ring += 0xDC4u;
            } while ((sint32)index < (sint32)count);
            count = r_u32(0x8010CD68u);
        }
    } else {
        count = r_u32(0x8010CD68u);
    }
    if ((sint32)count >= 2) {
        selection = (uint32)(sint32)r_s16(0x8010D0DCu);
        record = r_u32(0x8010CD7Cu);
        selected = r_u32(0x800FB900u + (selection << 2));
        w_u32(0x8010CD80u, record + selection * 20u - 600u);
        w_u32(0x800FB978u, selected);
        front_draw_system_selection_link(30u);
    } else {
        w_u32(0x800FB978u, 0u);
    }
    value = r_u32(0x8010CD6Cu);
    w_u32(0x8010CD6Cu, value + 1u);
    front_draw_viewer_controls();
}

uint32 front_initialize_selected_body_view(void)
{
    uint32 table, node, destination, source, length, value;
    FUNCTION_MARKER(0x800B0E84, "FRONT.OVL");
    front_configure_layer(0u, 0u, 0xA9u);
    if (r_s16(0x8010CD7Au) == 0) {
        table = r_u32(0x800FB0F4u);
        node = r_u32(0x800FB978u);
        destination = r_u32(table);
        source = node + 4u;
        length = 0u;
        while (r_u8(source + length) != 0u)
            ++length;
        strcpy((char *)psx_addr(destination, length + 1u), (const char *)psx_addr(source, length + 1u));
        table = r_u32(0x800FB0F4u);
        destination = r_u32(table);
        front_uppercase_text(destination);
        front_initialize_selected_celestial_system();
    }
    node = r_u32(0x800FB978u);
    w_u16(0x8010CD7Au, 1u);
    value = r_u32(node + 0x4Cu);
    if (value == 0u)
        w_u8(0x800F2FABu, 2u);
    return 2u;
}

void front_restore_selected_body_names(void)
{
    uint32 address, count, table, node, destination, source, length, index;
    FUNCTION_MARKER(0x800B0F30, "FRONT.OVL");
    front_configure_layer(0u, 0xFFFFFFFFu, 0xA9u);
    w_u16(0x8010CD78u, 0u);
    w_u16(0x800F3116u, 0u);
    front_configure_layer(0u, 0xFFFFFFFFu, 0xA9u);
    address = 0x800F2FB6u;
    count = 15u;
    do {
        w_u8(address, 2u);
        --address;
        --count;
    } while (count != 0u);
    table = r_u32(0x800FB0F4u);
    node = r_u32(0x800FB978u);
    destination = r_u32(table);
    source = node + 4u;
    length = 0u;
    while (r_u8(source + length) != 0u)
        ++length;
    strcpy((char *)psx_addr(destination, length + 1u), (const char *)psx_addr(source, length + 1u));
    count = r_u32(0x800FB954u);
    index = 0u;
    address = 0x800FB97Cu;
    while ((sint32)index < (sint32)count) {
        node = r_u32(address);
        address += 4u;
        table = r_u32(0x800FB0F4u);
        destination = r_u32(table + index * 4u + 4u);
        source = node + 4u;
        length = 0u;
        while (r_u8(source + length) != 0u)
            ++length;
        strcpy((char *)psx_addr(destination, length + 1u), (const char *)psx_addr(source, length + 1u));
        w_u8(0x800F2FA8u + index, 0u);
        count = r_u32(0x800FB954u);
        ++index;
    }
}

void front_update_selected_system_menu(void)
{
    uint32 count, index, ring, value;
    FUNCTION_MARKER(0x800B1034, "FRONT.OVL");
    if (r_u32(0x800FB978u) != 0u) {
        camera_dispatch();
        count = r_u32(0x8010CD68u);
        if ((sint32)count > 0) {
            index = 0u;
            ring = 0x800FBA18u;
            do {
                front_draw_celestial_ring(ring);
                count = r_u32(0x8010CD68u);
                ++index;
                ring += 0xDC4u;
            } while ((sint32)index < (sint32)count);
            count = r_u32(0x8010CD68u);
        }
    } else {
        count = r_u32(0x8010CD68u);
    }
    if ((sint32)count >= 2)
        front_draw_system_selection_link(20u);
    value = r_u32(0x8010CD6Cu);
    w_u32(0x8010CD6Cu, value + 1u);
    front_draw_viewer_controls();
}

void front_draw_selected_system_menu(void)
{
    uint32 count, index, ring, value;
    FUNCTION_MARKER(0x800B10F4, "FRONT.OVL");
    if (r_u32(0x800FB978u) != 0u) {
        camera_dispatch();
        count = r_u32(0x8010CD68u);
        index = 0u;
        ring = 0x800FBA18u;
        while ((sint32)index < (sint32)count) {
            front_draw_celestial_ring(ring);
            count = r_u32(0x8010CD68u);
            ++index;
            ring += 0xDC4u;
        }
    }
    value = r_u32(0x8010CD6Cu);
    w_u32(0x8010CD6Cu, value + 1u);
    front_draw_viewer_controls();
}

uint32 front_finish_selected_body_view(void)
{
    uint32 mode, value;
    FUNCTION_MARKER(0x800B1190, "FRONT.OVL");
    mode = r_u32(0x800FB900u);
    front_configure_layer(0u, mode, 0xA9u);
    value = (uint32)(sint32)r_s16(0x8010CD78u);
    if (value == 0u)
        return stream_stop();
    w_u16(0x8010CD78u, 0u);
    return value;
}

uint32 front_play_system_audio_first(void)
{
    uint32 descriptor, value, base, record, sectors, asset;
    FUNCTION_MARKER(0x800B11F0, "FRONT.OVL");
    descriptor = r_u32(0x8010CD7Cu);
    value = r_u32(descriptor + 4u);
    if ((sint32)value >= 0) {
        base = r_u32(0x800F708Cu);
        record = base + value * 24u;
        sectors = r_u32(record + 20u);
        asset = r_u32(record + 28u);
        stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
        w_u16(0x8010CD78u, 1u);
    }
    return 1u;
}

uint32 front_play_system_audio_second(void)
{
    uint32 descriptor, value, base, record, sectors, asset;
    FUNCTION_MARKER(0x800B126C, "FRONT.OVL");
    descriptor = r_u32(0x8010CD7Cu);
    value = r_u32(descriptor + 8u);
    if ((sint32)value >= 0) {
        base = r_u32(0x800F708Cu);
        record = base + value * 24u;
        sectors = r_u32(record + 20u);
        asset = r_u32(record + 28u);
        stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
        w_u16(0x8010CD78u, 1u);
    }
    return 1u;
}

uint32 front_play_system_audio_third(void)
{
    uint32 descriptor, value, base, record, sectors, asset;
    FUNCTION_MARKER(0x800B12E8, "FRONT.OVL");
    descriptor = r_u32(0x8010CD7Cu);
    value = r_u32(descriptor + 8u);
    if ((sint32)value >= 0) {
        value = r_u32(descriptor + 16u);
        base = r_u32(0x800F708Cu);
        record = base + value * 24u;
        sectors = r_u32(record + 20u);
        asset = r_u32(record + 28u);
        stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
        w_u16(0x8010CD78u, 1u);
    }
    return 1u;
}

uint32 front_play_body_audio_first(void)
{
    uint32 descriptor, value, base, record, sectors, asset;
    FUNCTION_MARKER(0x800B1368, "FRONT.OVL");
    descriptor = r_u32(0x8010CD80u);
    value = r_u32(descriptor + 4u);
    if ((sint32)value >= 0) {
        base = r_u32(0x800F708Cu);
        record = base + value * 24u;
        sectors = r_u32(record + 20u);
        asset = r_u32(record + 28u);
        stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
        w_u16(0x8010CD78u, 1u);
    }
    return 1u;
}

uint32 front_play_body_audio_second(void)
{
    uint32 descriptor, value, base, record, sectors, asset;
    FUNCTION_MARKER(0x800B13E4, "FRONT.OVL");
    descriptor = r_u32(0x8010CD80u);
    value = r_u32(descriptor + 8u);
    if ((sint32)value >= 0) {
        base = r_u32(0x800F708Cu);
        record = base + value * 24u;
        sectors = r_u32(record + 20u);
        asset = r_u32(record + 28u);
        stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
        w_u16(0x8010CD78u, 1u);
    }
    return 1u;
}

uint32 front_play_body_audio_third(void)
{
    uint32 descriptor, value, base, record, sectors, asset;
    FUNCTION_MARKER(0x800B1460, "FRONT.OVL");
    descriptor = r_u32(0x8010CD80u);
    value = r_u32(descriptor + 8u);
    if ((sint32)value >= 0) {
        value = r_u32(descriptor + 16u);
        base = r_u32(0x800F708Cu);
        record = base + value * 24u;
        sectors = r_u32(record + 20u);
        asset = r_u32(record + 28u);
        stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
        w_u16(0x8010CD78u, 1u);
    }
    return 1u;
}

uint32 front_stop_system_audio(void)
{
    FUNCTION_MARKER(0x800B14D8, "FRONT.OVL");
    return stream_stop();
}

void front_initialize_bar_transitions(uint32 duration)
{
    sint32 divisor, step;
    uint32 index, bar;
    FUNCTION_MARKER(0x800B2EB4, "FRONT.OVL");
    divisor = (sint16)duration;
    if (divisor == 0)
        abort();
    step = 4096 / divisor;
    w_u16(0x8010DA7Cu, duration);
    w_u16(0x8010DA78u, 0u);
    w_u16(0x8010DA82u, 0u);
    w_u16(0x8010DA80u, (uint32)step);
    w_u16(0x8010DA7Eu, (uint32)step);
    for (index = 0u; index < 10u; ++index) {
        bar = 0x8010DA84u + index * 24u;
        w_u8(bar + 17u, 0u);
        w_u8(bar + 16u, 0u);
    }
}

void front_reset_bar_transition(uint32 bar, uint32 target)
{
    FUNCTION_MARKER(0x800B2F70, "FRONT.OVL");
    w_u16(bar + 4u, 0u);
    w_u16(bar + 8u, 0u);
    w_u16(bar + 10u, 0u);
    w_u16(bar + 6u, target);
}

uint32 front_start_bar_transition(uint32 bar, uint32 target)
{
    FUNCTION_MARKER(0x800B2F84, "FRONT.OVL");
    front_reset_bar_transition(bar, (uint32)(sint32)(sint16)target);
    w_u8(bar + 16u, 1u);
    w_u16(bar + 8u, target);
    return 1u;
}

uint32 front_append_bar_transition(uint32 target, const uint8 *color)
{
    uint32 count, current, previous, value, blue;
    sint32 last;
    FUNCTION_MARKER(0x800B2FCC, "FRONT.OVL");
    count = r_u16(0x8010DA78u);
    last = (sint16)(count - 1u);
    if (last < 0)
        diagnostic_report(0u, (intptr)0x80086F00u, (intptr)0x80086F14u);
    current = 0x8010DA84u + (uint32)(sint32)(sint16)count * 24u;
    previous = 0x8010DA84u + (uint32)last * 24u;
    front_reset_bar_transition(current, (uint32)(sint32)(sint16)target);
    value = r_u16(previous + 6u);
    w_u16(current + 4u, value);
    value = r_u16(previous);
    w_u16(current, value);
    value = r_u16(previous + 2u);
    w_u16(current + 2u, value);
    w_u8(current + 12u, color[0]);
    w_u8(current + 13u, color[1]);
    blue = color[2];
    w_u32(current + 20u, 0u);
    w_u8(current + 17u, 0u);
    w_u8(current + 14u, blue);
    w_u8(previous + 17u, 1u);
    count = r_u16(0x8010DA78u) + 1u;
    w_u16(0x8010DA78u, count);
    return count;
}

uint32 front_prepare_bar_scale(void)
{
    uint32 total, maximum, index, bar, value;
    sint32 count;
    FUNCTION_MARKER(0x800B30EC, "FRONT.OVL");
    total = 0u;
    maximum = 0u;
    count = r_s16(0x8010DA78u);
    bar = 0x8010DA84u;
    for (index = 0u; (sint32)index < count; ++index) {
        if (r_u8(bar + 16u) == 0u)
            total += r_u16(bar + 6u);
        if (r_u8(bar + 17u) == 0u) {
            value = (uint32)(sint32)(sint16)total;
            if (maximum < value)
                maximum = value;
            total = 0u;
        }
        bar += 24u;
    }
    w_u16(0x8010DA7Au, 4096u);
    return 4096u;
}

sint32 front_bar_transition_extent(void)
{
    uint32 total, maximum, index, bar, numerator;
    sint32 count, divisor, value;
    FUNCTION_MARKER(0x800B3184, "FRONT.OVL");
    total = 0u;
    maximum = 0u;
    count = r_s16(0x8010DA78u);
    bar = 0x8010DA84u;
    for (index = 0u; (sint32)index < count; ++index) {
        if (r_u8(bar + 16u) == 0u) {
            value = r_s16(bar + 8u);
            divisor = r_s16(0x8010DA80u);
            numerator = (uint32)value << 12;
            if (divisor == 0)
                abort();
            total += (uint32)((sint32)numerator / divisor);
        }
        if (r_u8(bar + 17u) == 0u) {
            if ((sint16)maximum < (sint16)total)
                maximum = total;
            total = 0u;
        }
        bar += 24u;
    }
    return (sint16)maximum;
}

void front_update_bar_scale(void)
{
    uint32 current, target;
    sint32 extent, value, step, difference, distance;
    FUNCTION_MARKER(0x800B3258, "FRONT.OVL");
    extent = (sint16)front_bar_transition_extent();
    if (extent >= 400) {
        value = r_s16(0x8010DA80u);
        step = value / 100;
        w_u16(0x8010DA7Eu, (uint32)value << 1);
        w_u16(0x8010DA82u, (uint32)step);
    }
    target = r_u16(0x8010DA7Eu);
    current = r_u16(0x8010DA80u);
    step = r_s16(0x8010DA82u);
    difference = (sint16)(target - current);
    distance = difference < 0 ? -difference : difference;
    if (distance < step) {
        w_u16(0x8010DA80u, target);
    } else {
        if (difference < 0)
            w_u16(0x8010DA80u, current - (uint32)step);
        if (difference > 0) {
            current = r_u16(0x8010DA80u);
            w_u16(0x8010DA80u, current + (uint32)step);
        }
    }
}

void front_draw_bar_transition_labels(uint32 index)
{
    uint32 bar, numerator, text, x, y, width, previous, number;
    sint32 selected, divisor, extent;
    char label[8];
    FUNCTION_MARKER(0x800B3354, "FRONT.OVL");
    selected = (sint16)index;
    bar = 0x8010DA84u + (uint32)selected * 24u;
    numerator = (uint32)(r_s16(bar + 8u) + r_s16(bar + 4u)) << 12;
    divisor = r_s16(0x8010DA80u);
    numerator += (uint32)r_s16(bar + 10u);
    if (divisor == 0)
        abort();
    extent = (sint32)numerator / divisor;
    if (r_u8(bar + 16u) == 1u) {
        width = front_text_width(r_u32(bar + 20u));
        text = r_u32(bar + 20u);
        x = (uint32)r_s16(bar) - width - 6u;
        y = (uint32)r_s16(bar + 2u);
        front_draw_colored_text(text, x, y, 0x808080u);
        return;
    }
    previous = 0u;
    if (selected > 0 && r_u8(bar - 24u + 17u) == 1u)
        previous = bar - 24u;
    number = r_u16(bar + 8u);
    if (previous != 0u)
        number += r_u16(previous + 8u);
    sprintf(label, "%d", (sint32)(sint16)number);
    if (r_u8(bar + 17u) == 1u) {
        width = front_text_width_native(label);
        x = (uint32)(sint16)extent + (uint32)r_s16(bar) - (uint32)((sint32)width >> 1);
        y = (uint32)r_s16(bar + 2u) - 14u;
    } else {
        x = (uint32)(sint16)extent + (uint32)r_s16(bar);
        y = (uint32)r_s16(bar + 2u) + 2u;
    }
    front_draw_colored_text_native(label, x, y, 0x808080u);
    text = r_u32(bar + 20u);
    if (text != 0u) {
        y = (uint32)r_s16(bar + 2u) + 18u;
        x = (uint32)r_s16(bar);
        front_draw_colored_text(text, x, y, 0x808080u);
    }
}

uint32 front_update_bar_transition(uint32 index)
{
    uint32 bar, previous, value;
    sint32 selected, current, target, scale, change, whole, fraction;
    FUNCTION_MARKER(0x800B352C, "FRONT.OVL");
    selected = (sint16)index;
    bar = 0x8010DA84u + (uint32)selected * 24u;
    previous = 0u;
    if (selected > 0 && r_u8(bar - 24u + 17u) == 1u)
        previous = bar - 24u;
    if (previous != 0u) {
        current = r_s16(previous + 8u);
        target = r_s16(previous + 6u);
        if (target != current)
            return 0u;
        w_u16(bar + 4u, (uint32)current);
    }
    target = r_s16(0x8010DA7Eu);
    current = r_s16(0x8010DA80u);
    if (target != current)
        return 1u;
    target = r_s16(bar + 6u);
    current = r_s16(bar + 8u);
    scale = r_s16(0x8010DA7Au);
    change = (sint32)((uint32)(target - current) << 12);
    if (change < -scale)
        change = -scale;
    if (change > scale)
        change = scale;
    whole = change / 4096;
    w_u16(bar + 8u, (uint32)current + (uint32)whole);
    value = r_u16(bar + 10u) + (uint32)(change - whole * 4096);
    w_u16(bar + 10u, value);
    fraction = (sint16)value;
    if (fraction >= 4096) {
        whole = fraction / 4096;
        fraction = r_s16(bar + 10u);
        value = r_u16(bar + 8u);
        w_u16(bar + 8u, value + (uint32)whole);
        w_u16(bar + 10u, (uint32)(fraction - (fraction / 4096) * 4096));
    }
    return 1u;
}

void front_draw_bar_transitions(uint32 list)
{
    uint32 index, bar, packet, numerator, value;
    sint32 divisor, quotient;
    uint16 x[7], y[7];
    FUNCTION_MARKER(0x800B3698, "FRONT.OVL");
    w_u32(0x800FB870u, 0x53u);
    front_update_bar_scale();
    index = 0u;
    if (r_s16(0x8010DA78u) <= 0)
        return;
    do {
        if ((sint16)front_update_bar_transition((uint32)(sint32)(sint16)index) != 0) {
            front_draw_bar_transition_labels((uint32)(sint32)(sint16)index);
            bar = list + 12u + (uint32)(sint32)(sint16)index * 24u;
            if (r_u8(bar + 16u) == 0u) {
                numerator = (uint32)r_s16(bar + 4u) << 12;
                divisor = r_s16(0x8010DA80u);
                if (divisor == 0)
                    abort();
                quotient = (sint32)numerator / divisor;
                x[0] = (uint16)(r_u16(bar) + (uint32)quotient - 6u);
                numerator = (uint32)(r_s16(bar + 4u) + r_s16(bar + 8u)) << 12;
                numerator += (uint32)r_s16(bar + 10u);
                quotient = (sint32)numerator / divisor;
                x[1] = (uint16)(r_u16(bar) + (uint32)quotient - 6u);
                numerator = (uint32)r_s16(bar + 4u) << 12;
                quotient = (sint32)numerator / divisor;
                x[2] = (uint16)(r_u16(bar) + (uint32)quotient);
                numerator = (uint32)(r_s16(bar + 4u) + r_s16(bar + 8u)) << 12;
                numerator += (uint32)r_s16(bar + 10u);
                quotient = (sint32)numerator / divisor;
                x[3] = (uint16)(r_u16(bar) + (uint32)quotient);
            } else {
                x[0] = (uint16)(r_u16(bar) + r_u16(bar + 4u) - 6u);
                value = r_u16(bar) + r_u16(bar + 4u);
                x[1] = (uint16)(r_u16(bar + 8u) + value - 6u);
                x[2] = (uint16)(r_u16(bar) + r_u16(bar + 4u));
                value = r_u16(bar) + r_u16(bar + 4u);
                x[3] = (uint16)(r_u16(bar + 8u) + value);
            }
            x[4] = x[0];
            x[5] = x[2];
            x[6] = x[3];
            value = r_u16(bar + 2u);
            y[0] = (uint16)(value - 6u);
            y[1] = y[0];
            y[2] = (uint16)r_u16(bar + 2u);
            y[3] = y[2];
            y[4] = (uint16)(value + 10u);
            value = r_u16(bar + 2u);
            packet = r_u32(0x801132B8u);
            y[5] = (uint16)(value + 16u);
            y[6] = y[5];
            w_u8(packet + 3u, 5u);
            w_u8(packet + 7u, 0x28u);
            w_u8(packet + 4u, r_u8(bar + 12u));
            w_u8(packet + 5u, r_u8(bar + 13u));
            w_u8(packet + 6u, r_u8(bar + 14u));
            w_u16(packet + 8u, x[2]);
            w_u16(packet + 10u, y[2]);
            w_u16(packet + 12u, x[3]);
            w_u16(packet + 14u, y[3]);
            w_u16(packet + 16u, x[5]);
            w_u16(packet + 18u, y[5]);
            w_u16(packet + 20u, x[6]);
            w_u16(packet + 22u, y[6]);
            AddPrim(psx_addr(r_u32(0x801132BCu), 4u), psx_addr(packet, 24u));
            packet += 24u;
            w_u8(packet + 3u, 5u);
            w_u8(packet + 7u, 0x28u);
            w_u8(packet + 4u, r_u8(bar + 12u) >> 1);
            w_u8(packet + 5u, r_u8(bar + 13u) >> 1);
            w_u8(packet + 6u, r_u8(bar + 14u) >> 1);
            w_u16(packet + 8u, x[0]);
            w_u16(packet + 10u, y[0]);
            w_u16(packet + 12u, x[1]);
            w_u16(packet + 14u, y[1]);
            w_u16(packet + 16u, x[2]);
            w_u16(packet + 18u, y[2]);
            w_u16(packet + 20u, x[3]);
            w_u16(packet + 22u, y[3]);
            AddPrim(psx_addr(r_u32(0x801132BCu), 4u), psx_addr(packet, 24u));
            packet += 24u;
            w_u8(packet + 3u, 5u);
            w_u8(packet + 7u, 0x28u);
            w_u8(packet + 4u, (r_u8(bar + 12u) * 2u) / 3u);
            w_u8(packet + 5u, (r_u8(bar + 13u) * 2u) / 3u);
            w_u8(packet + 6u, (r_u8(bar + 14u) * 2u) / 3u);
            w_u16(packet + 8u, x[0]);
            w_u16(packet + 10u, y[0]);
            w_u16(packet + 12u, x[2]);
            w_u16(packet + 14u, y[2]);
            w_u16(packet + 16u, x[4]);
            w_u16(packet + 18u, y[4]);
            w_u16(packet + 20u, x[5]);
            w_u16(packet + 22u, y[5]);
            AddPrim(psx_addr(r_u32(0x801132BCu), 4u), psx_addr(packet, 24u));
            packet += 24u;
            w_u32(0x801132B8u, packet);
        }
        ++index;
    } while ((sint16)index < r_s16(0x8010DA78u));
}

void front_initialize_ranking_statistics(void)
{
    uint32 table, destination, source, length, models, mesh, rank, bar, number, count;
    uint32 first_width, second_width, third_width, maximum;
    uint8 color[3];
    FUNCTION_MARKER(0x800B3B90, "FRONT.OVL");
    number = r_u32(0x800F3150u);
    table = r_u32(0x800FB0F4u);
    destination = r_u32(table + 4u);
    source = r_u32(table + (number == 1u ? 8u : 12u));
    length = 0u;
    while (r_u8(source + length) != 0u)
        ++length;
    strcpy((char *)psx_addr(destination, (size_t)length + 1u), (const char *)psx_addr(source, (size_t)length + 1u));
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
    w_u32(0x800FB870u, 83u);
    w_u32(0x80110980u, 0x800F29FCu);
    w_u32(0x80110984u, 1u);
    front_initialize_bar_transitions(4u);
    bar = 0x8010DA84u + (uint32)r_s16(0x8010DA78u) * 24u;
    number = r_u32(0x80011914u);
    w_u16(bar, 100u);
    w_u16(bar + 2u, 225u);
    w_u8(bar + 12u, 208u);
    w_u8(bar + 13u, 64u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    w_u32(bar + 20u, r_u32(table + 4u));
    front_reset_bar_transition(bar, (uint32)(sint32)(sint16)number);
    color[0] = 0u;
    color[1] = 0u;
    color[2] = 255u;
    count = r_u16(0x8010DA78u);
    number = r_u32(0x80011904u) - r_u32(0x80011914u);
    w_u16(0x8010DA78u, count + 1u);
    front_append_bar_transition((uint32)(sint32)(sint16)number, color);
    bar = 0x8010DA84u + (uint32)r_s16(0x8010DA78u) * 24u;
    number = r_u32(0x80011918u);
    w_u16(bar, 100u);
    w_u16(bar + 2u, 275u);
    w_u8(bar + 12u, 208u);
    w_u8(bar + 13u, 64u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    w_u32(bar + 20u, r_u32(table));
    front_reset_bar_transition(bar, (uint32)(sint32)(sint16)number);
    color[0] = 0u;
    color[1] = 0u;
    color[2] = 255u;
    count = r_u16(0x8010DA78u);
    number = r_u32(0x80011908u) - r_u32(0x80011918u);
    w_u16(0x8010DA78u, count + 1u);
    front_append_bar_transition((uint32)(sint32)(sint16)number, color);
    bar = 0x8010DA84u + (uint32)r_s16(0x8010DA78u) * 24u;
    number = r_u32(0x8001190Cu);
    w_u16(bar, 100u);
    w_u16(bar + 2u, 325u);
    w_u8(bar + 12u, 0u);
    w_u8(bar + 13u, 128u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    w_u32(bar + 20u, r_u32(table + 12u));
    front_reset_bar_transition(bar, (uint32)(sint32)(sint16)number);
    count = r_u16(0x8010DA78u) + 1u;
    number = r_u32(0x80011910u);
    bar = 0x8010DA84u + (uint32)(sint32)(sint16)count * 24u;
    w_u16(0x8010DA78u, count);
    w_u16(bar, 100u);
    w_u16(bar + 2u, 375u);
    w_u8(bar + 12u, 0u);
    w_u8(bar + 13u, 128u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    w_u32(bar + 20u, r_u32(table + 8u));
    front_reset_bar_transition(bar, (uint32)(sint32)(sint16)number);
    count = r_u16(0x8010DA78u) + 1u;
    bar = 0x8010DA84u + (uint32)(sint32)(sint16)count * 24u;
    w_u16(0x8010DA78u, count);
    w_u8(bar + 12u, 208u);
    w_u8(bar + 13u, 64u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    source = r_u32(table + 56u);
    w_u32(bar + 20u, source);
    number = front_text_width(source);
    w_u16(bar, number + 106u);
    w_u16(bar + 2u, 185u);
    front_start_bar_transition(bar, 15u);
    count = r_u16(0x8010DA78u) + 1u;
    w_u16(0x8010DA78u, count);
    front_prepare_bar_scale();
    table = r_u32(0x80110980u);
    w_u32(0x800FB870u, 253u);
    first_width = front_text_width(r_u32(table + 36u));
    table = r_u32(0x80110980u);
    second_width = front_text_width(r_u32(table + 40u));
    maximum = (sint32)first_width < (sint32)second_width ? second_width : first_width;
    table = r_u32(0x80110980u);
    third_width = front_text_width(r_u32(table + 44u));
    if ((sint32)maximum < (sint32)third_width)
        maximum = third_width;
    maximum += 46u;
    w_u32(0x801131F0u, maximum - first_width);
    w_u32(0x801131F4u, maximum - second_width);
    w_u32(0x801131F8u, maximum - third_width);
    w_u32(0x801131FCu, maximum);
}

void front_initialize_rank_lighting(void)
{
    MATRIX light;
    FUNCTION_MARKER(0x800B6A90, "FRONT.OVL");
    light.m[0][0] = 4096;
    light.m[1][0] = 0;
    light.m[2][0] = 0;
    light.m[0][1] = 0;
    light.m[1][1] = 4096;
    light.m[2][1] = 0;
    light.m[0][2] = 0;
    light.m[1][2] = 0;
    light.m[2][2] = 4096;
    SetLightMatrix(&light);
    SetBackColor(32, 32, 32);
    light_set_color(0u, 0u, 0u, 0u);
    light_set_direction(0u, 0u, 1000u, 1000u);
    light_set_color(1u, 64u, 64u, 64u);
    light_set_direction(1u, 1000u, (uint32)-1000, 1000u);
    light_set_color(2u, 0u, 0u, 0u);
}

uint32 front_campaign_rank(void)
{
    uint32 row, column, address, flags;
    sint32 first, second, rank;
    FUNCTION_MARKER(0x800B6B4C, "FRONT.OVL");
    first = 0;
    second = 0;
    for (row = 0u; row < 18u; ++row) {
        address = 0x80010498u + row * 7u;
        for (column = 0u; column < 6u; ++column) {
            flags = r_u8(address + column);
            if ((flags & 2u) != 0u)
                ++first;
            else if ((flags & 1u) != 0u)
                ++second;
        }
    }
    rank = (first - second) / 4 + 1;
    if (rank <= 0)
        rank = 1;
    if (rank >= 9)
        rank = 8;
    return (uint32)rank;
}

void front_initialize_rank_models(uint32 rank)
{
    uint32 horizontal, vertical, depth, first_mesh, second_mesh, count, row, record;
    FUNCTION_MARKER(0x800B6DD8, "FRONT.OVL");
    horizontal = 0u;
    vertical = 0u;
    depth = 0u;
    w_u32(0x800FB21Cu, rank);
    if ((sint32)rank >= 5) {
        rank -= 4u;
        horizontal = 3240u;
        vertical = (uint32)-1688;
        depth = (uint32)-4000;
    }
    w_u32(0x800FB144u, vertical - 1512u);
    w_u32(0x800FB148u, depth + 28000u);
    first_mesh = r_u32(0x800FB120u);
    second_mesh = r_u32(0x800FB128u);
    w_u32(0x800FB140u, 0u);
    w_u32(0x800FB14Cu, 512u);
    w_u32(0x800FB154u, horizontal);
    w_u32(0x800FB158u, (uint32)-1512);
    w_u32(0x800FB15Cu, 28000u);
    w_u32(0x800FB160u, 512u);
    w_u32(0x800FB150u, first_mesh);
    w_u32(0x800FB164u, second_mesh);
    count = 2u;
    if (horizontal != 0u) {
        count = 3u;
        first_mesh = r_u32(0x800FB124u);
        w_u32(0x800FB168u, 0u - horizontal);
        w_u32(0x800FB16Cu, (uint32)-1512);
        w_u32(0x800FB170u, 28000u);
        w_u32(0x800FB174u, 512u);
        w_u32(0x800FB178u, first_mesh);
    }
    vertical = 316u;
    depth = 32000u;
    record = 0x800FB140u + count * 20u;
    for (row = 1u; (sint32)row < (sint32)rank; ++row) {
        w_u32(record, horizontal);
        w_u32(record + 4u, vertical);
        w_u32(record + 8u, depth);
        w_u32(record + 12u, 512u);
        first_mesh = r_u32(0x800FB130u);
        ++count;
        w_u32(record + 16u, first_mesh);
        record += 20u;
        if (horizontal != 0u) {
            w_u32(record, 0u - horizontal);
            w_u32(record + 4u, vertical);
            w_u32(record + 8u, depth);
            w_u32(record + 12u, 512u);
            first_mesh = r_u32(0x800FB12Cu);
            ++count;
            w_u32(record + 16u, first_mesh);
            record += 20u;
        }
        vertical += 1416u;
        depth += 4000u;
    }
    w_u32(0x800FB13Cu, count);
}

void front_draw_ranking_statistics(void)
{
    uint32 table, text, width, rank, length;
    sint32 seconds, minutes, hours, value;
    char line[104];
    FUNCTION_MARKER(0x800B4050, "FRONT.OVL");
    table = r_u32(0x80110980u);
    w_u32(0x800FB870u, 253u);
    front_draw_colored_text(r_u32(table + 48u), 46u, 95u, 0x404040u);
    table = r_u32(0x80110980u);
    width = front_text_width(r_u32(table + 48u));
    rank = r_u32(0x800FB21Cu);
    text = r_u32(0x800F2A38u + (rank << 2));
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    front_draw_text_native((const char *)psx_addr(text, (size_t)length + 1u), width + 50u, 95u, 0x404040u, 1);
    front_draw_rank_models();
    seconds = r_s32(0x80011928u) / 300;
    minutes = seconds / 60;
    hours = minutes / 60;
    table = r_u32(0x80110980u);
    text = r_u32(table + 36u);
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    sprintf(line, "%s : %d%d:%d%d:%d%d", (const char *)psx_addr(text, (size_t)length + 1u), hours / 10, hours % 10, (minutes - hours * 60) / 10, minutes % 10, (seconds - minutes * 60) / 10, seconds % 10);
    front_draw_colored_text_native(line, 46u, 120u, 0x404040u);
    table = r_u32(0x80110980u);
    value = r_s32(0x80011924u);
    text = r_u32(table + 32u);
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    sprintf(line, "%s %d", (const char *)psx_addr(text, (size_t)length + 1u), value);
    front_draw_colored_text_native(line, 46u, 140u, 0x404040u);
    if (r_u32(0x80110984u) == 1u)
        front_draw_bar_transitions(0x8010DA78u);
}

void front_draw_rank_models(void)
{
    uint32 table, active_table, tag, packet, count, index, record, value, offset_x, offset_y;
    MATRIX identity = {0};
    MATRIX transform;
    FUNCTION_MARKER(0x800B6FAC, "FRONT.OVL");
    w_u32(0x8001024Cu, 320u);
    w_u32(0x80010250u, 240u);
    system_set_projection_offset(320, 240);
    system_set_projection_distance(r_s32(0x800BC6E4u));
    render_set_semi_trans(1);
    table = r_u32(0x801132BCu);
    active_table = r_u32(0x801132BCu);
    tag = r_u32(table + 0x7CCu);
    packet = r_u32(0x801132B8u);
    w_u32(0x800421ACu, active_table + 400u);
    count = r_u32(0x800FB13Cu);
    w_u32(0x80040690u, packet);
    w_u32(0x8004217Cu, tag);
    if ((sint32)count > 0) {
        index = 0u;
        record = 0x800FB140u;
        do {
            value = r_u32(record + 8u);
            if ((sint32)value >= 8001)
                w_u32(record + 8u, value - 200u);
            value = r_u32(record + 8u);
            if ((sint32)value < 8512) {
                value = r_u32(record + 12u);
                if ((sint32)value > 0)
                    w_u32(record + 12u, value - 64u);
            }
            matrix_set_identity_native(&identity);
            matrix_rotate_x(&transform, &identity, r_u32(record + 12u));
            transform.t[0] = r_s32(record);
            transform.t[1] = r_s32(record + 4u);
            transform.t[2] = r_s32(record + 8u);
            SetRotMatrix(&transform);
            SetTransMatrix(&transform);
            value = r_u32(record + 16u);
            ++index;
            mesh_render_fixed_alternate(value);
            count = r_u32(0x800FB13Cu);
            record += 20u;
        } while ((sint32)index < (sint32)count);
    }
    render_set_semi_trans(0);
    packet = r_u32(0x80040690u);
    offset_x = r_u32(0x80031198u);
    offset_y = r_u32(0x800311B4u);
    w_u32(0x801132B8u, packet);
    w_u32(0x8001024Cu, offset_x);
    w_u32(0x80010250u, offset_y);
    system_set_projection_offset((sint32)offset_x, (sint32)offset_y);
    system_set_projection_distance(r_s32(0x800313F8u));
}

void front_initialize_mission_statistics(void)
{
    uint32 bar, table, number, count, text, width;
    uint8 color[3];
    FUNCTION_MARKER(0x800B42AC, "FRONT.OVL");
    w_u32(0x800FB870u, 83u);
    w_u32(0x80110980u, 0x800F29FCu);
    w_u32(0x80110984u, 1u);
    front_initialize_bar_transitions(40u);
    bar = 0x8010DA84u + (uint32)r_s16(0x8010DA78u) * 24u;
    number = (uint32)r_s16(0x80059626u);
    w_u16(bar, 100u);
    w_u16(bar + 2u, 225u);
    w_u8(bar + 12u, 208u);
    w_u8(bar + 13u, 64u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    w_u32(bar + 20u, r_u32(table + 4u));
    front_reset_bar_transition(bar, number);
    color[0] = 0u;
    color[1] = 0u;
    color[2] = 255u;
    count = r_u16(0x8010DA78u);
    number = (uint32)r_s16(0x8005961Eu) - (uint32)r_s16(0x80059626u);
    w_u16(0x8010DA78u, count + 1u);
    front_append_bar_transition((uint32)(sint32)(sint16)number, color);
    bar = 0x8010DA84u + (uint32)r_s16(0x8010DA78u) * 24u;
    number = (uint32)r_s16(0x80059628u);
    w_u16(bar, 100u);
    w_u16(bar + 2u, 275u);
    w_u8(bar + 12u, 208u);
    w_u8(bar + 13u, 64u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    w_u32(bar + 20u, r_u32(table));
    front_reset_bar_transition(bar, number);
    color[0] = 0u;
    color[1] = 0u;
    color[2] = 255u;
    count = r_u16(0x8010DA78u);
    number = (uint32)r_s16(0x80059620u) - (uint32)r_s16(0x80059628u);
    w_u16(0x8010DA78u, count + 1u);
    front_append_bar_transition((uint32)(sint32)(sint16)number, color);
    bar = 0x8010DA84u + (uint32)r_s16(0x8010DA78u) * 24u;
    number = (uint32)r_s16(0x80059622u);
    w_u16(bar, 100u);
    w_u16(bar + 2u, 325u);
    w_u8(bar + 12u, 0u);
    w_u8(bar + 13u, 128u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    w_u32(bar + 20u, r_u32(table + 12u));
    front_reset_bar_transition(bar, number);
    count = r_u16(0x8010DA78u) + 1u;
    number = (uint32)r_s16(0x80059624u);
    bar = 0x8010DA84u + (uint32)(sint32)(sint16)count * 24u;
    w_u16(0x8010DA78u, count);
    w_u16(bar, 100u);
    w_u16(bar + 2u, 375u);
    w_u8(bar + 12u, 0u);
    w_u8(bar + 13u, 128u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    w_u32(bar + 20u, r_u32(table + 8u));
    front_reset_bar_transition(bar, number);
    count = r_u16(0x8010DA78u) + 1u;
    bar = 0x8010DA84u + (uint32)(sint32)(sint16)count * 24u;
    w_u16(0x8010DA78u, count);
    w_u8(bar + 12u, 208u);
    w_u8(bar + 13u, 64u);
    w_u8(bar + 14u, 0u);
    table = r_u32(0x80110980u);
    text = r_u32(table + 56u);
    w_u32(bar + 20u, text);
    width = front_text_width(text);
    w_u16(bar, width + 106u);
    w_u16(bar + 2u, 185u);
    front_start_bar_transition(bar, 15u);
    count = r_u16(0x8010DA78u) + 1u;
    w_u16(0x8010DA78u, count);
    front_prepare_bar_scale();
    table = r_u32(0x80110980u);
    w_u32(0x800FB870u, 253u);
    width = front_text_width(r_u32(table + 36u));
    w_u32(0x801131F0u, 46u);
    w_u32(0x801131FCu, width + 46u);
}

void front_draw_mission_statistics(void)
{
    uint32 table, text, width, rank, length, x, format, format_length;
    sint32 seconds, minutes, hours;
    char line[104];
    FUNCTION_MARKER(0x800B45D8, "FRONT.OVL");
    table = r_u32(0x80110980u);
    w_u32(0x800FB870u, 253u);
    front_draw_colored_text(r_u32(table + 48u), 46u, 95u, 0x404040u);
    table = r_u32(0x80110980u);
    width = front_text_width(r_u32(table + 48u));
    rank = r_u32(0x800FB21Cu);
    text = r_u32(0x800F2A38u + (rank << 2));
    length = 0u;
    while (r_u8(text + length) != 0u)
        ++length;
    front_draw_text_native((const char *)psx_addr(text, (size_t)length + 1u), width + 50u, 95u, 0x404040u, 1);
    table = r_u32(0x80110980u);
    x = r_u32(0x801131F0u);
    front_draw_colored_text(r_u32(table + 36u), x, 120u, 0x404040u);
    seconds = r_s32(0x80059630u) / 300;
    minutes = seconds / 60;
    hours = minutes / 60;
    sprintf(line, " : %d%d:%d%d:%d%d", hours / 10, hours % 10, (minutes - hours * 60) / 10, minutes % 10, (seconds - minutes * 60) / 10, seconds % 10);
    x = r_u32(0x801131FCu);
    front_draw_colored_text_native(line, x, 120u, 0x404040u);
    if (r_s16(0x800595F8u) >= 2 && r_s16(0x800595FAu) == 1) {
        table = r_u32(0x80110980u);
        format = r_u32(table + 52u);
        format_length = 0u;
        while (r_u8(format + format_length) != 0u)
            ++format_length;
        length = 0u;
        while (r_u8(0x800FB220u + length) != 0u)
            ++length;
        sprintf(line, (const char *)psx_addr(format, (size_t)format_length + 1u), (const char *)psx_addr(0x800FB220u, (size_t)length + 1u));
        front_draw_colored_text_native(line, 46u, 152u, 0x404040u);
    }
    if (r_u32(0x80110984u) == 1u)
        front_draw_bar_transitions(0x8010DA78u);
}

uint32 front_initialize_database_audio(void)
{
    uint32 saved, position, sector, end_sector, sectors, database;
    CdlFILE file;
    FUNCTION_MARKER(0x800B4868, "FRONT.OVL");
    stream_init();
    saved = (uint32)r_s16(0x800118FCu);
    w_u16(0x800118FCu, 0u);
    w_u32(0x8010CDACu, 0u);
    w_u32(0x8011329Cu, saved);
    if (CdSearchFile(&file, "\\INTRO.STX;1") != NULL) {
        position = (uint32)file.pos.minute | ((uint32)file.pos.second << 8) | ((uint32)file.pos.sector << 16) | ((uint32)file.pos.track << 24);
        w_u32(0x80110978u, position);
        sector = (uint32)system_cd_position_to_sector(&file.pos);
        w_u32(0x800FB800u, sector);
        end_sector = sector + (file.size >> 11);
        w_u32(0x800FB804u, end_sector);
        CdIntToPos((sint32)end_sector, (CdlLOC *)psx_addr(0x8011097Cu, sizeof(CdlLOC)));
        sectors = file.size >> 10;
        w_u32(0x80113210u, sectors);
        stream_request_position(0x80110978u, 1u, sectors);
    } else {
#ifndef NDEBUG
        printf("no xa file\n");
#endif
    }
    database = resource_load_sector_index("DATABASE.INF", "DATABASE.PAK");
    w_u32(0x800F708Cu, database);
    return database;
}

void front_update_database_audio_timer(void)
{
    uint32 value, timer, deadline;
    FUNCTION_MARKER(0x800B496C, "FRONT.OVL");
    if ((r_u32(0x80053684u) & 0x100u) != 0u) {
        value = r_u32(0x800BE644u) == 0u ? 1u : 0u;
        w_u32(0x800BE644u, value);
        w_u32(0x80040FF0u, value);
    }
    if (r_u32(0x800BE644u) != 0u)
        return;
    if (r_u32(r_u32(0x80041004u)) != 0u)
        return;
    timer = r_u32(0x8010CDACu);
    deadline = r_u32(0x800BE640u);
    w_u32(0x8010CDACu, timer + 1u);
    if (timer == deadline)
        w_u32(0x800BE640u, timer + 240u);
}

void front_free_database_audio(void)
{
    uint32 saved;
    FUNCTION_MARKER(0x800B4A08, "FRONT.OVL");
    stream_reset_all_queues();
    heap_free(r_u32(0x800F708Cu));
    saved = r_u32(0x8011329Cu);
    w_u16(0x800118FCu, saved);
}

void front_select_database_mission(void)
{
    sint32 selected;
    char name[16];
    FUNCTION_MARKER(0x800B4A4C, "FRONT.OVL");
    selected = r_s16(0x8010D0DCu) - 30;
    sprintf(name, "0_%d.CW", selected);
    memcpy(psx_addr(0x80010518u, strlen(name) + 1u), name, strlen(name) + 1u);
    w_u16(0x800595F8u, 0u);
    w_u16(0x800595FAu, (uint32)selected);
    w_u32(0x800BD424u, 1u);
    front_clear_menu_transition();
}

void front_initialize_database_mission_list(void)
{
    uint32 index, value;
    FUNCTION_MARKER(0x800B4ABC, "FRONT.OVL");
    value = r_u8(0x80011935u);
    if (value == 0u) {
        for (index = 0u; index < 6u; ++index)
            w_u8(0x800F2FADu - index, 1u);
        for (index = 0u; index < 6u; ++index) {
            value = r_u8(0x80010491u + index);
            w_u8(0x800F2FA8u + index, 0u);
            if (value == 0u)
                break;
        }
    }
    for (index = 0u; index < 6u; ++index) {
        if (r_u8(0x80010491u + index) == 0u) {
            w_u16(0x8010D0DCu, index + 31u);
            break;
        }
    }
    front_reset_mission_briefing();
}

void front_draw_database_mission_list(void)
{
    uint32 selected, previous, asset, record, sectors;
    FUNCTION_MARKER(0x800B4B84, "FRONT.OVL");
    selected = (uint32)(sint32)r_s16(0x8010D0DCu) - 31u;
    previous = r_u32(0x800F3168u);
    if (selected != previous) {
        w_u32(0x800F3168u, selected);
        asset = r_u32(0x800BDE54u + (selected << 2));
        record = r_u32(0x800F708Cu) + asset * 24u;
        sectors = r_u32(record + 20u);
        asset = r_u32(record + 28u);
        stream_request_segments(record + 16u, sectors, asset, 0x800118FEu, 1, 0u, NULL);
    }
    w_u32(0x800FB870u, 253u);
    front_draw_colored_text(0x800F31A0u + selected * 2652u, 46u, 64u, 0x808080u);
    front_reset_text_emphasis();
    w_u32(0x800FB870u, 83u);
}

void front_database_mission_idle(void)
{
    FUNCTION_MARKER(0x800B4C90, "FRONT.OVL");
}

void front_link_campaign_node(uint32 node)
{
    uint32 visits, level, maximum, type, first, second, candidate, neighbors[2];
    uint32 index, neighbor, children;
    FUNCTION_MARKER(0x800B4C98, "FRONT.OVL");
    if (node == 0u)
        return;
    visits = r_u32(node + 32u);
    if (visits == 0u)
        return;
    level = r_u32(node + 24u);
    w_u32(node + 32u, visits + 1u);
    maximum = r_u32(0x800FB730u);
    type = r_u32(node + 28u);
    if ((sint32)maximum < (sint32)level)
        w_u32(0x800FB730u, level);
    first = r_u8(0x800BE6B4u + (type << 1));
    if (first >= 90u)
        first -= 70u;
    second = r_u8(0x800BE6B5u + (type << 1));
    if (second >= 90u)
        second -= 70u;
    neighbors[0] = 0u;
    if (first - 1u < 99u) {
        candidate = 0x800FB2A8u + first * 40u;
        if (r_u32(candidate + 32u) != 0u)
            neighbors[0] = candidate;
        else if (type == 18u && r_u32(0x800FB6D8u) != 0u)
            neighbors[0] = 0x800FB6B8u;
    }
    neighbors[1] = 0u;
    if (second - 1u < 99u) {
        candidate = 0x800FB2A8u + second * 40u;
        if (r_u32(candidate + 32u) != 0u)
            neighbors[1] = candidate;
    }
    for (index = 0u; index < 2u; ++index) {
        neighbor = neighbors[index];
        w_u32(node + (index == 0u ? 4u : 0u), neighbor);
        if (neighbor != 0u) {
            if (r_u32(neighbor + 8u) != node && r_u32(neighbor + 12u) != node && r_u32(neighbor + 16u) != node) {
                children = r_u32(neighbor + 20u);
                w_u32(neighbor + 8u + (children << 2), node);
                children = r_u32(neighbor + 20u);
                if ((sint32)children < 3)
                    w_u32(neighbor + 20u, children + 1u);
            }
            level = r_u32(node + 24u) + 1u;
            maximum = r_u32(neighbor + 24u);
            if ((sint32)maximum < (sint32)level)
                w_u32(neighbor + 24u, level);
        }
    }
    front_link_campaign_node(neighbors[0]);
    front_link_campaign_node(neighbors[1]);
}
