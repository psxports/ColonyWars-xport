#include "game_functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

uint32 mesh_build_bounds(uint32 mesh)
{
    sint32 minimum[3] = {999999, 999999, 999999};
    sint32 maximum[3] = {-999999, -999999, -999999};
    sint32 coordinate[3];
    sint32 radius;
    sint32 magnitude;
    sint32 index;
    uint32 vertex;
    uint32 axis;
    uint32 field;
    uint32 bounds;
    uint32 box;
    uint32 count;
    uint32 value;
    FUNCTION_MARKER(0x80017D10, "SLUS_005.43");
    radius = 0;
    if (r_s32(mesh + 4) > 0)
    {
        vertex = r_u32(mesh);
        index = 0;
        do
        {
            for (axis = 0; axis < 3; ++axis)
            {
                coordinate[axis] = r_s16(vertex + axis * 2);
                if (maximum[axis] < coordinate[axis])
                    maximum[axis] = coordinate[axis];
            }
            for (axis = 0; axis < 3; ++axis)
            {
                if (coordinate[axis] < minimum[axis])
                    minimum[axis] = coordinate[axis];
            }
            for (field = 0; field < 6; ++field)
            {
                magnitude = field < 3 ? maximum[field] : minimum[field - 3];
                if (magnitude < 0)
                    magnitude = (sint32)(0 - (uint32)magnitude);
                if (radius < magnitude)
                    radius = magnitude;
            }
            count = r_u32(mesh + 4);
            ++index;
            vertex += 8;
        } while (index < (sint32)count);
    }
    bounds = heap_alloc_back(20);
    box = heap_alloc_back(28);
    w_u32(bounds, 0);
    count = r_u32(mesh + 20);
    w_u32(bounds + 12, 1);
    value = (uint32)maximum[0] + (uint32)minimum[0];
    value = (uint32)((sint32)value >> 1);
    w_u32(bounds + 8, box);
    w_u32(bounds + 16, (uint32)radius);
    w_u32(bounds + 4, count);
    w_u16(box, value);
    value = (uint32)maximum[1] + (uint32)minimum[1];
    w_u16(box + 2, (sint32)value >> 1);
    value = (uint32)maximum[2] + (uint32)minimum[2];
    w_u16(box + 4, (sint32)value >> 1);
    for (axis = 0; axis < 3; ++axis)
    {
        value = (uint32)maximum[axis] - (uint32)minimum[axis];
        w_u16(box + 8 + axis * 2, (sint32)value >> 1);
    }
    w_u32(box + 16, 0);
    count = r_u32(mesh + 20);
    w_u32(box + 24, 0);
    w_u32(box + 20, count);
    return bounds;
}

sint32 mesh_load_fragments(uint32 chunk, uint32 output, uint32 source_mesh)
{
    sint32 count;
    sint32 index;
    uint32 selected;
    uint32 list;
    uint32 mesh;
    uint32 bounds;
    FUNCTION_MARKER(0x80017F78, "SLUS_005.43");
    if (chunk & 3)
        diagnostic_report(666, 0x80011A74, (intptr)0x80011A90);
    count = (sint32)vector_pack_xy(chunk);
    w_u32(output + 112, (uint32)count);
    chunk += 4;
    for (index = 0; index < count; ++index)
    {
        selected = vector_pack_xy(chunk);
        chunk += 4;
        w_u32(0x80040F78 + (uint32)index * 4, chunk);
        chunk += selected << 2;
    }
    w_u32(0x800421B0, chunk);
    for (index = 0; index < count; ++index)
    {
        list = r_u32(0x80040F78 + (uint32)index * 4);
        selected = vector_pack_xy(list - 4);
        list = r_u32(0x80040F78 + (uint32)index * 4);
        mesh = mesh_extract_fragment((sint32)selected, list, source_mesh);
        w_u32(output + (uint32)index * 4 + 12, mesh);
        bounds = mesh_build_bounds(mesh);
        w_u32(output + (uint32)index * 4 + 60, bounds);
    }
    return count > 0 ? 0 : count;
}

void render_emit_mesh_lit(uint32 mesh)
{
    static const uint32 quad_tests[4][3] = {{0, 1, 3}, {1, 3, 2}, {3, 2, 0}, {2, 0, 1}};
    uint32 face;
    uint32 screen;
    uint32 depth_table;
    uint32 normals;
    uint32 offset[4];
    uint32 point[4];
    uint16 uv[4];
    uint16 clut;
    uint16 page;
    uint32 pass;
    uint32 corners;
    uint32 vertex;
    uint32 test;
    uint32 axis;
    uint32 normal;
    uint32 color;
    uint32 packet;
    uint32 semi;
    uint32 depth;
    uint32 shift;
    uint32 base;
    uint32 slot;
    sint32 counts[2];
    sint32 remaining;
    sint32 z[4];
    sint32 sum;
    sint32 far;
    sint32 minimum;
    sint32 maximum;
    sint32 coordinate;
    sint32 span[2];
    sint32 visible;
    FUNCTION_MARKER(0x80018094, "SLUS_005.43");
    face = r_u32(mesh + 32);
    counts[0] = r_s32(mesh + 24);
    counts[1] = r_s32(mesh + 28);
    screen = r_u32(0x80046088);
    depth_table = r_u32(0x80041F4C);
    normals = r_u32(mesh + 8);
    (void)r_u32(mesh);
    for (pass = 0; pass < 2; ++pass)
    {
        corners = pass ? 4 : 3;
        for (remaining = counts[pass]; remaining > 0; --remaining, face += 60)
        {
            for (vertex = 0; vertex < corners; ++vertex)
                offset[vertex] = r_u32(face + 4 + vertex * 4) << 2;
            for (vertex = 0; vertex < corners; ++vertex)
                z[vertex] = r_s16(depth_table + (offset[vertex] >> 1));
            sum = z[0] + z[1] + z[2];
            if (!pass)
            {
                if (!sum)
                    continue;
                far = ((sum + z[2]) >> 2) - 2048;
            }
            else
            {
                sum += z[3];
                far = (sum >> 2) - 2048;
                if (far < 0 && !sum)
                    continue;
            }
            if (far < 0)
            {
                if (!pass)
                {
                    for (vertex = 0; vertex < 3; ++vertex)
                        point[vertex] = r_u32(screen + offset[vertex]);
                    if (NormalClip((sint32)point[0], (sint32)point[1], (sint32)point[2]) < 0)
                        continue;
                }
                visible = 1;
                for (axis = 0; axis < 2; ++axis)
                {
                    minimum = r_s16(screen + offset[0] + axis * 2);
                    maximum = minimum;
                    for (vertex = 1; vertex < corners; ++vertex)
                    {
                        coordinate = r_s16(screen + offset[vertex] + axis * 2);
                        if (coordinate <= minimum)
                            minimum = coordinate;
                        if (coordinate >= maximum && !(pass && axis == 1 && vertex == 3))
                            maximum = coordinate;
                    }
                    if (maximum < 0 || minimum > (axis ? 240 : 512))
                    {
                        visible = 0;
                        break;
                    }
                    span[axis] = maximum - minimum;
                }
                if (!visible)
                    continue;
                if (!pass)
                {
                    if (span[0] >= 256 || span[1] > 128)
                    {
                        render_subdivide_triangle_large(mesh, face);
                        continue;
                    }
                    if (span[0] >= 128 || span[1] > 64)
                    {
                        render_subdivide_triangle_medium(mesh, face);
                        continue;
                    }
                }
                else
                {
                    if (span[0] >= 256 || span[1] > 128)
                    {
                        visible = 0;
                        for (test = 0; test < 4; ++test)
                        {
                            for (vertex = 0; vertex < 3; ++vertex)
                                point[vertex] = r_u32(screen + offset[quad_tests[test][vertex]]);
                            if (NormalClip((sint32)point[0], (sint32)point[1], (sint32)point[2]) >= 0)
                            {
                                visible = 1;
                                break;
                            }
                        }
                        if (visible)
                            render_subdivide_quad_large(mesh, face);
                        continue;
                    }
                    for (vertex = 0; vertex < 3; ++vertex)
                        point[vertex] = r_u32(screen + offset[vertex]);
                    if (NormalClip((sint32)point[0], (sint32)point[1], (sint32)point[2]) < 0)
                        continue;
                    /* Original thresholds use the packed points */
                    if ((sint32)(point[2] - 128) >= 0 || (sint32)(point[1] - 64) > 0)
                    {
                        render_subdivide_quad_medium(mesh, face);
                        continue;
                    }
                }
            }
            for (vertex = 0; vertex < corners; ++vertex)
                point[vertex] = r_u32(screen + offset[vertex]);
            if (NormalClip((sint32)point[0], (sint32)point[1], (sint32)point[2]) < 0)
                continue;
            normal = (r_u32(face + 20) << 3) + normals;
            color = r_u32(face + 36);
            xport_gte_write_data(0, r_u32(normal));
            xport_gte_write_data(1, r_u32(normal + 4));
            xport_gte_write_data(6, color);
            xport_gte_execute(0x108041B);
            color = xport_gte_read_data(22);
            packet = r_u32(0x80040690);
            semi = r_u32(pass ? 0x80031148 : 0x80031144);
            w_u32(packet + 4, color);
            w_u8(packet + 7, semi + (pass ? 0x2C : 0x24));
            w_u8(packet + 3, pass ? 9 : 7);
            for (vertex = 0; vertex < corners; ++vertex)
                w_u32(packet + 8 + vertex * 8, point[vertex]);
            for (vertex = 0; vertex < corners; ++vertex)
                uv[vertex] = r_u16(face + 28 + vertex * 2);
            for (vertex = 0; vertex < corners; ++vertex)
                w_u16(packet + 12 + vertex * 8, uv[vertex]);
            clut = r_u16(face + 24);
            page = r_u16(face + 26);
            w_u16(packet + 14, clut);
            w_u16(packet + 22, page);
            for (vertex = 0; vertex < corners; ++vertex)
                z[vertex] = r_s16(depth_table + (offset[vertex] >> 1));
            sum = pass ? z[0] : z[0] * 2;
            sum += z[1] + z[2];
            if (pass)
                sum += z[3];
            shift = r_u32(0x80053670);
            depth = ((uint32)sum >> 2) << (shift & 31);
            base = r_u32(0x800421AC);
            if ((sint32)depth < 0x4000)
            {
                if ((sint32)depth < 0)
                    depth = 0;
                slot = base + ((depth >> 6) + 64) * 4;
            }
            else
            {
                if ((sint32)depth > 0x3FFFFE)
                    depth = 0x3FFFFE;
                slot = base + ((depth >> 11) + 320) * 4;
            }
            AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
            w_u32(0x80040690, r_u32(0x80040690) + (pass ? 40 : 32));
        }
    }
}

uint32 render_emit_mesh_wire(uint32 mesh, uint32 color)
{
    uint32 face;
    uint32 counts[2];
    uint32 screen;
    uint32 depth_table;
    uint32 scratch;
    uint32 result;
    uint32 pass;
    uint32 remaining;
    uint32 corners;
    uint32 vertex;
    uint32 offset[4];
    uint32 point[4];
    uint32 packet;
    uint32 saved_color;
    uint32 depth;
    uint32 base;
    uint32 slot;
    sint32 sum;
    FUNCTION_MARKER(0x80018AA4, "SLUS_005.43");
    face = r_u32(mesh + 32);
    counts[0] = r_u32(mesh + 24);
    counts[1] = r_u32(mesh + 28);
    screen = r_u32(0x80046088);
    depth_table = r_u32(0x80041F4C);
    (void)r_u32(mesh + 8);
    (void)r_u32(mesh);
    scratch = r_u32(0x80042908);
    result = scratch;
    w_u32(scratch, color);
    for (pass = 0; pass < 2; ++pass)
    {
        corners = pass ? 4 : 3;
        remaining = counts[pass];
        do
        {
            for (vertex = 0; vertex < corners; ++vertex)
                offset[vertex] = r_u32(face + 4 + vertex * 4) << 2;
            sum = 0;
            for (vertex = 0; vertex < corners; ++vertex)
                sum += r_s16(depth_table + (offset[vertex] >> 1));
            if (sum)
            {
                for (vertex = 0; vertex < corners; ++vertex)
                    point[vertex] = r_u32(screen + offset[vertex]);
                scratch = r_u32(0x80042908);
                saved_color = r_u32(scratch);
                packet = r_u32(0x80040690);
                w_u32(packet + 4, saved_color);
                w_u8(packet + 7, 0x4E);
                w_u8(packet + 3, 6);
                w_u32(packet + 8, point[0]);
                w_u32(packet + 12, point[1]);
                w_u32(packet + 16, point[pass ? 3 : 2]);
                w_u32(packet + 20, point[pass ? 2 : 0]);
                w_u32(packet + 24, 0x55555555);
                depth = 50u << (r_u32(0x80053670) & 31);
                base = r_u32(0x800421AC);
                if ((sint32)depth < 0x4000)
                {
                    if ((sint32)depth < 0)
                        depth = 0;
                    slot = base + ((depth >> 6) + 64) * 4;
                }
                else
                {
                    if ((sint32)depth > 0x3FFFFE)
                        depth = 0x3FFFFE;
                    slot = base + ((depth >> 11) + 320) * 4;
                }
                AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
                result = r_u32(slot);
                w_u32(0x80040690, r_u32(0x80040690) + 28);
                if (pass)
                {
                    packet = r_u32(0x80040690);
                    w_u32(packet + 4, saved_color);
                    w_u8(packet + 7, 0x42);
                    w_u8(packet + 3, 3);
                    w_u32(packet + 8, point[2]);
                    w_u32(packet + 12, point[0]);
                    (void)r_u32(0x80053670);
                    w_u32(0x80040690, r_u32(0x80040690) + 16);
                    /* The original leaves this closing line unlinked */
                    result = 3;
                }
            }
            face += 60;
            --remaining;
        } while ((sint32)remaining > 0);
    }
    return result;
}

uint32 render_emit_mesh_wire_front(uint32 mesh, uint32 color)
{
    uint32 face;
    uint32 counts[2];
    uint32 screen;
    uint32 depth_table;
    uint32 scratch;
    uint32 result;
    uint32 pass;
    uint32 remaining;
    uint32 corners;
    uint32 vertex;
    uint32 offset[4];
    uint32 point[4];
    uint32 packet;
    uint32 saved_color;
    uint32 slot;
    sint32 sum;
    FUNCTION_MARKER(0x80018F10, "SLUS_005.43");
    face = r_u32(mesh + 32);
    counts[0] = r_u32(mesh + 24);
    counts[1] = r_u32(mesh + 28);
    screen = r_u32(0x80046088);
    depth_table = r_u32(0x80041F4C);
    (void)r_u32(mesh + 8);
    (void)r_u32(mesh);
    scratch = r_u32(0x80042908);
    result = scratch;
    w_u32(scratch, color);
    for (pass = 0; pass < 2; ++pass)
    {
        corners = pass ? 4 : 3;
        remaining = counts[pass];
        do
        {
            for (vertex = 0; vertex < corners; ++vertex)
                offset[vertex] = r_u32(face + 4 + vertex * 4) << 2;
            if (!pass)
            {
                sum = 0;
                for (vertex = 0; vertex < 3; ++vertex)
                    sum += r_s16(depth_table + (offset[vertex] >> 1));
                if (!sum)
                    goto next_face;
            }
            for (vertex = 0; vertex < corners; ++vertex)
                point[vertex] = r_u32(screen + offset[vertex]);
            if (NormalClip((sint32)point[0], (sint32)point[1], (sint32)point[2]) < 0)
                goto next_face;
            scratch = r_u32(0x80042908);
            saved_color = r_u32(scratch);
            packet = r_u32(0x80040690);
            w_u32(packet + 4, saved_color);
            w_u8(packet + 7, 0x4E);
            w_u8(packet + 3, 6);
            w_u32(packet + 8, point[0]);
            w_u32(packet + 12, point[1]);
            w_u32(packet + 16, point[pass ? 3 : 2]);
            w_u32(packet + 20, point[pass ? 2 : 0]);
            w_u32(packet + 24, 0x55555555);
            slot = r_u32(0x800421AC);
            AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
            result = r_u32(slot);
            w_u32(0x80040690, r_u32(0x80040690) + 28);
            if (pass)
            {
                packet = r_u32(0x80040690);
                w_u32(packet + 4, saved_color);
                w_u8(packet + 7, 0x42);
                w_u8(packet + 3, 3);
                w_u32(packet + 8, point[2]);
                w_u32(packet + 12, point[0]);
                slot = r_u32(0x800421AC);
                AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
                result = r_u32(slot);
                w_u32(0x80040690, r_u32(0x80040690) + 16);
            }
        next_face:
            face += 60;
            --remaining;
        } while ((sint32)remaining > 0);
    }
    return result;
}



void render_subdivide_triangle_medium(uint32 mesh, uint32 face)
{
    static const uint32 original_slots[3] = {0, 3, 5};
    static const uint32 midpoint_pairs[3][3] = {{0, 3, 1}, {0, 5, 2}, {3, 5, 4}};
    static const uint32 triangles[4][3] = {{0, 1, 2}, {1, 3, 4}, {1, 2, 4}, {2, 4, 5}};
    uint32 scratch;
    uint32 vertices;
    uint32 source[4];
    uint32 words[3];
    sint32 coordinate[6];
    uint32 component[4];
    uint32 index;
    uint32 axis;
    uint32 pair;
    uint32 first;
    uint32 second;
    uint32 destination;
    uint32 normal;
    uint32 color;
    uint16 page;
    uint16 clut;
    uint32 point[3];
    uint16 uv[3];
    uint32 sum;
    uint32 depth;
    uint32 packet;
    uint32 semi;
    uint32 base;
    uint32 slot;
    FUNCTION_MARKER(0x800192DC, "SLUS_005.43");
    (void)r_u32(0x80046088);
    scratch = r_u32(0x80042908);
    for (index = 0; index < 4; ++index)
        source[index] = r_u32(face + 4 + index * 4) << 3;
    vertices = r_u32(mesh);
    for (index = 0; index < 4; ++index)
        source[index] += vertices;
    for (axis = 0; axis < 2; ++axis)
    {
        for (index = 0; index < 3; ++index)
            words[index] = r_u32(source[index] + axis * 4);
        for (index = 0; index < 3; ++index)
            w_u32(scratch + original_slots[index] * 8 + axis * 4, words[index]);
    }
    for (pair = 0; pair < 3; ++pair)
    {
        first = scratch + midpoint_pairs[pair][0] * 8;
        second = scratch + midpoint_pairs[pair][1] * 8;
        destination = scratch + midpoint_pairs[pair][2] * 8;
        for (axis = 0; axis < 3; ++axis)
            coordinate[axis] = r_s16(first + axis * 2);
        for (axis = 0; axis < 3; ++axis)
            coordinate[axis + 3] = r_s16(second + axis * 2);
        for (axis = 0; axis < 3; ++axis)
            w_u16(destination + axis * 2, (coordinate[axis] + coordinate[axis + 3]) >> 1);
    }
    for (index = 0; index < 3; ++index)
        uv[index] = r_u16(face + 28 + index * 2);
    for (index = 0; index < 3; ++index)
        w_u16(scratch + 400 + original_slots[index] * 2, uv[index]);
    for (pair = 0; pair < 3; ++pair)
    {
        first = scratch + 400 + midpoint_pairs[pair][0] * 2;
        second = scratch + 400 + midpoint_pairs[pair][1] * 2;
        destination = scratch + 400 + midpoint_pairs[pair][2] * 2;
        component[0] = r_u8(first);
        component[1] = r_u8(first + 1);
        component[2] = r_u8(second);
        component[3] = r_u8(second + 1);
        w_u8(destination, (component[0] + component[2]) >> 1);
        w_u8(destination + 1, (component[1] + component[3]) >> 1);
    }
    for (index = 0; index < 6; ++index)
    {
        words[0] = r_u32(scratch + index * 8);
        words[1] = r_u32(scratch + index * 8 + 4);
        xport_gte_write_data(0, words[0]);
        xport_gte_write_data(1, words[1]);
        xport_gte_execute(0x180001);
        w_u32(scratch + 200 + index * 4, xport_gte_read_data(14));
        w_u32(scratch + 300 + index * 4, xport_gte_read_data(19));
    }
    page = r_u16(face + 26);
    clut = r_u16(face + 24);
    normal = r_u32(face + 20) << 3;
    normal += r_u32(mesh + 8);
    color = r_u32(face + 36);
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    xport_gte_write_data(6, color);
    xport_gte_execute(0x108041B);
    color = xport_gte_read_data(22);
    for (pair = 0; pair < 4; ++pair)
    {
        for (index = 0; index < 3; ++index)
            words[index] = r_u32(scratch + 300 + triangles[pair][index] * 4);
        sum = words[0] + words[1] + words[2] + words[2];
        if (!sum)
            continue;
        depth = sum >> 2;
        for (index = 0; index < 3; ++index)
            point[index] = r_u32(scratch + 200 + triangles[pair][index] * 4);
        for (index = 0; index < 3; ++index)
            uv[index] = r_u16(scratch + 400 + triangles[pair][index] * 2);
        packet = r_u32(0x80040690);
        semi = r_u32(0x80031144);
        w_u32(packet + 4, color);
        w_u8(packet + 7, semi + 0x24);
        w_u8(packet + 3, 7);
        for (index = 0; index < 3; ++index)
            w_u32(packet + 8 + index * 8, point[index]);
        for (index = 0; index < 3; ++index)
            w_u16(packet + 12 + index * 8, uv[index]);
        w_u16(packet + 14, clut);
        w_u16(packet + 22, page);
        depth <<= r_u32(0x80053670) & 31;
        base = r_u32(0x800421AC);
        if ((sint32)depth < 0x4000)
        {
            if ((sint32)depth < 0)
                depth = 0;
            slot = base + ((depth >> 6) + 64) * 4;
        }
        else
        {
            if ((sint32)depth > 0x3FFFFE)
                depth = 0x3FFFFE;
            slot = base + ((depth >> 11) + 320) * 4;
        }
        AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
        w_u32(0x80040690, r_u32(0x80040690) + 32);
    }
}

void render_subdivide_triangle_large(uint32 mesh, uint32 face)
{
    static const uint32 original_slots[3] = {0, 10, 14};
    static const uint32 midpoint_pairs[12][3] = {
        {0, 10, 3}, {0, 14, 5}, {10, 14, 12}, {0, 3, 1}, {0, 5, 2}, {3, 5, 4},
        {3, 10, 6}, {3, 12, 7}, {10, 12, 11}, {5, 12, 8}, {5, 14, 9}, {12, 14, 13}};
    static const uint32 triangles[16][3] = {
        {0, 1, 2}, {1, 3, 4}, {1, 2, 4}, {2, 4, 5}, {3, 4, 7}, {4, 7, 8},
        {4, 5, 8}, {7, 8, 12}, {3, 6, 7}, {6, 10, 11}, {6, 7, 11}, {7, 11, 12},
        {5, 8, 9}, {8, 9, 13}, {8, 12, 13}, {9, 13, 14}};
    uint32 scratch;
    uint32 vertices;
    uint32 source[4];
    uint32 words[3];
    sint32 coordinate[6];
    uint32 component[4];
    uint32 index;
    uint32 axis;
    uint32 pair;
    uint32 first;
    uint32 second;
    uint32 destination;
    uint32 normal;
    uint32 color;
    uint16 page;
    uint16 clut;
    uint32 point[3];
    uint16 uv[3];
    uint32 sum;
    uint32 depth;
    uint32 packet;
    uint32 semi;
    uint32 base;
    uint32 slot;
    FUNCTION_MARKER(0x80019AEC, "SLUS_005.43");
    (void)r_u32(0x80046088);
    scratch = r_u32(0x80042908);
    for (index = 0; index < 4; ++index)
        source[index] = r_u32(face + 4 + index * 4) << 3;
    vertices = r_u32(mesh);
    for (index = 0; index < 4; ++index)
        source[index] += vertices;
    for (axis = 0; axis < 2; ++axis)
    {
        for (index = 0; index < 3; ++index)
            words[index] = r_u32(source[index] + axis * 4);
        for (index = 0; index < 3; ++index)
            w_u32(scratch + original_slots[index] * 8 + axis * 4, words[index]);
    }
    for (pair = 0; pair < 12; ++pair)
    {
        first = scratch + midpoint_pairs[pair][0] * 8;
        second = scratch + midpoint_pairs[pair][1] * 8;
        destination = scratch + midpoint_pairs[pair][2] * 8;
        for (axis = 0; axis < 3; ++axis)
            coordinate[axis] = r_s16(first + axis * 2);
        for (axis = 0; axis < 3; ++axis)
            coordinate[axis + 3] = r_s16(second + axis * 2);
        for (axis = 0; axis < 3; ++axis)
            w_u16(destination + axis * 2, (coordinate[axis] + coordinate[axis + 3]) >> 1);
    }
    for (index = 0; index < 3; ++index)
        uv[index] = r_u16(face + 28 + index * 2);
    for (index = 0; index < 3; ++index)
        w_u16(scratch + 400 + original_slots[index] * 2, uv[index]);
    for (pair = 0; pair < 12; ++pair)
    {
        first = scratch + 400 + midpoint_pairs[pair][0] * 2;
        second = scratch + 400 + midpoint_pairs[pair][1] * 2;
        destination = scratch + 400 + midpoint_pairs[pair][2] * 2;
        component[0] = r_u8(first);
        component[1] = r_u8(first + 1);
        component[2] = r_u8(second);
        component[3] = r_u8(second + 1);
        w_u8(destination, (component[0] + component[2]) >> 1);
        w_u8(destination + 1, (component[1] + component[3]) >> 1);
    }
    for (index = 0; index < 15; ++index)
    {
        words[0] = r_u32(scratch + index * 8);
        words[1] = r_u32(scratch + index * 8 + 4);
        xport_gte_write_data(0, words[0]);
        xport_gte_write_data(1, words[1]);
        xport_gte_execute(0x180001);
        w_u32(scratch + 200 + index * 4, xport_gte_read_data(14));
        w_u32(scratch + 300 + index * 4, xport_gte_read_data(19));
    }
    page = r_u16(face + 26);
    clut = r_u16(face + 24);
    normal = r_u32(face + 20) << 3;
    normal += r_u32(mesh + 8);
    color = r_u32(face + 36);
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    xport_gte_write_data(6, color);
    xport_gte_execute(0x108041B);
    color = xport_gte_read_data(22);
    for (pair = 0; pair < 16; ++pair)
    {
        if (pair < 9)
        {
            for (index = 0; index < 3; ++index)
                words[index] = r_u32(scratch + 300 + triangles[pair][index] * 4);
            sum = words[0] + words[1] + words[2] + words[2];
            if (!sum)
                continue;
            depth = sum >> 2;
        }
        for (index = 0; index < 3; ++index)
            point[index] = r_u32(scratch + 200 + triangles[pair][index] * 4);
        for (index = 0; index < 3; ++index)
            uv[index] = r_u16(scratch + 400 + triangles[pair][index] * 2);
        if (pair >= 9)
        {
            for (index = 0; index < 3; ++index)
                words[index] = r_u32(scratch + 300 + triangles[pair][index] * 4);
            sum = words[0] + words[1] + words[2] + words[2];
            depth = sum >> 2;
        }
        packet = r_u32(0x80040690);
        semi = r_u32(0x80031144);
        w_u32(packet + 4, color);
        w_u8(packet + 7, semi + 0x24);
        w_u8(packet + 3, 7);
        for (index = 0; index < 3; ++index)
            w_u32(packet + 8 + index * 8, point[index]);
        for (index = 0; index < 3; ++index)
            w_u16(packet + 12 + index * 8, uv[index]);
        w_u16(packet + 14, clut);
        w_u16(packet + 22, page);
        depth <<= r_u32(0x80053670) & 31;
        base = r_u32(0x800421AC);
        if ((sint32)depth < 0x4000)
        {
            if ((sint32)depth < 0)
                depth = 0;
            slot = base + ((depth >> 6) + 64) * 4;
        }
        else
        {
            if ((sint32)depth > 0x3FFFFE)
                depth = 0x3FFFFE;
            slot = base + ((depth >> 11) + 320) * 4;
        }
        AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
        w_u32(0x80040690, r_u32(0x80040690) + 32);
    }
}

void render_subdivide_quad_medium(uint32 mesh, uint32 face)
{
    static const uint32 original_slots[4] = {0, 2, 6, 8};
    static const uint32 midpoint_pairs[5][3] = {{0, 2, 1}, {0, 6, 3}, {2, 8, 5}, {6, 8, 7}, {1, 7, 4}};
    static const uint32 quads[4][4] = {{0, 1, 3, 4}, {1, 2, 4, 5}, {3, 4, 6, 7}, {4, 5, 7, 8}};
    uint32 scratch;
    uint32 vertices;
    uint32 source[4];
    uint32 words[4];
    sint32 coordinate[6];
    uint32 component[4];
    uint32 index;
    uint32 axis;
    uint32 pair;
    uint32 first;
    uint32 second;
    uint32 destination;
    uint32 normal;
    uint32 color;
    uint16 page;
    uint16 clut;
    uint32 point[4];
    uint16 uv[4];
    uint32 sum;
    uint32 depth;
    uint32 packet;
    uint32 semi;
    uint32 base;
    uint32 slot;
    FUNCTION_MARKER(0x8001B614, "SLUS_005.43");
    (void)r_u32(0x80046088);
    scratch = r_u32(0x80042908);
    for (index = 0; index < 4; ++index)
        source[index] = r_u32(face + 4 + index * 4) << 3;
    vertices = r_u32(mesh);
    for (index = 0; index < 4; ++index)
        source[index] += vertices;
    for (axis = 0; axis < 2; ++axis)
    {
        for (index = 0; index < 4; ++index)
            words[index] = r_u32(source[index] + axis * 4);
        for (index = 0; index < 4; ++index)
            w_u32(scratch + original_slots[index] * 8 + axis * 4, words[index]);
    }
    for (pair = 0; pair < 5; ++pair)
    {
        first = scratch + midpoint_pairs[pair][0] * 8;
        second = scratch + midpoint_pairs[pair][1] * 8;
        destination = scratch + midpoint_pairs[pair][2] * 8;
        for (axis = 0; axis < 3; ++axis)
            coordinate[axis] = r_s16(first + axis * 2);
        for (axis = 0; axis < 3; ++axis)
            coordinate[axis + 3] = r_s16(second + axis * 2);
        for (axis = 0; axis < 3; ++axis)
            w_u16(destination + axis * 2, (coordinate[axis] + coordinate[axis + 3]) >> 1);
    }
    for (index = 0; index < 4; ++index)
        uv[index] = r_u16(face + 28 + index * 2);
    for (index = 0; index < 4; ++index)
        w_u16(scratch + 400 + original_slots[index] * 2, uv[index]);
    for (pair = 0; pair < 5; ++pair)
    {
        first = scratch + 400 + midpoint_pairs[pair][0] * 2;
        second = scratch + 400 + midpoint_pairs[pair][1] * 2;
        destination = scratch + 400 + midpoint_pairs[pair][2] * 2;
        component[0] = r_u8(first);
        component[1] = r_u8(first + 1);
        component[2] = r_u8(second);
        component[3] = r_u8(second + 1);
        w_u8(destination, (component[0] + component[2]) >> 1);
        w_u8(destination + 1, (component[1] + component[3]) >> 1);
    }
    for (index = 0; index < 9; ++index)
    {
        words[0] = r_u32(scratch + index * 8);
        words[1] = r_u32(scratch + index * 8 + 4);
        xport_gte_write_data(0, words[0]);
        xport_gte_write_data(1, words[1]);
        xport_gte_execute(0x180001);
        w_u32(scratch + 200 + index * 4, xport_gte_read_data(14));
        w_u32(scratch + 300 + index * 4, xport_gte_read_data(19));
    }
    page = r_u16(face + 26);
    clut = r_u16(face + 24);
    normal = r_u32(face + 20) << 3;
    normal += r_u32(mesh + 8);
    color = r_u32(face + 36);
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    xport_gte_write_data(6, color);
    xport_gte_execute(0x108041B);
    color = xport_gte_read_data(22);
    for (pair = 0; pair < 4; ++pair)
    {
        for (index = 0; index < 4; ++index)
            words[index] = r_u32(scratch + 300 + quads[pair][index] * 4);
        sum = words[0] + words[1] + words[2] + words[3];
        if (!sum)
            continue;
        depth = sum >> 2;
        for (index = 0; index < 4; ++index)
            point[index] = r_u32(scratch + 200 + quads[pair][index] * 4);
        for (index = 0; index < 4; ++index)
            uv[index] = r_u16(scratch + 400 + quads[pair][index] * 2);
        packet = r_u32(0x80040690);
        semi = r_u32(0x80031148);
        w_u32(packet + 4, color);
        w_u8(packet + 7, semi + 0x2C);
        w_u8(packet + 3, 9);
        for (index = 0; index < 4; ++index)
            w_u32(packet + 8 + index * 8, point[index]);
        for (index = 0; index < 4; ++index)
            w_u16(packet + 12 + index * 8, uv[index]);
        w_u16(packet + 14, clut);
        w_u16(packet + 22, page);
        depth <<= r_u32(0x80053670) & 31;
        base = r_u32(0x800421AC);
        if ((sint32)depth < 0x4000)
        {
            if ((sint32)depth < 0)
                depth = 0;
            slot = base + ((depth >> 6) + 64) * 4;
        }
        else
        {
            if ((sint32)depth > 0x3FFFFE)
                depth = 0x3FFFFE;
            slot = base + ((depth >> 11) + 320) * 4;
        }
        AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
        w_u32(0x80040690, r_u32(0x80040690) + 40);
    }
}

void render_emit_mesh_overlay(uint32 mesh)
{
    uint32 face;
    sint32 counts[2];
    uint32 screen;
    uint32 depth_table;
    uint32 normals;
    uint32 pass;
    sint32 remaining;
    uint32 corners;
    uint32 vertex;
    uint32 offset[4];
    uint32 point[4];
    sint32 z[4];
    sint32 sum;
    uint32 normal;
    uint32 color;
    uint32 packet;
    uint32 semi;
    uint16 uv[4];
    uint16 clut;
    uint16 page;
    uint32 slot;
    FUNCTION_MARKER(0x8001E12C, "SLUS_005.43");
    face = r_u32(mesh + 32);
    counts[0] = r_s32(mesh + 24);
    counts[1] = r_s32(mesh + 28);
    screen = r_u32(0x80046088);
    depth_table = r_u32(0x80041F4C);
    normals = r_u32(mesh + 8);
    (void)r_u32(mesh);
    for (pass = 0; pass < 2; ++pass)
    {
        corners = pass ? 4 : 3;
        for (remaining = counts[pass]; remaining > 0; --remaining, face += 60)
        {
            for (vertex = 0; vertex < corners; ++vertex)
                offset[vertex] = r_u32(face + 4 + vertex * 4) << 2;
            for (vertex = 0; vertex < corners; ++vertex)
                z[vertex] = r_s16(depth_table + (offset[vertex] >> 1));
            sum = z[0] + z[1] + z[2];
            if (pass)
                sum += z[3];
            if (!pass && !sum)
                continue;
            for (vertex = 0; vertex < corners; ++vertex)
                point[vertex] = r_u32(screen + offset[vertex]);
            if (NormalClip((sint32)point[0], (sint32)point[1], (sint32)point[2]) < 0)
                continue;
            color = r_u32(face + 36);
            if (!(color >> 24))
            {
                normal = (r_u32(face + 20) << 3) + normals;
                color = r_u32(face + 36);
                xport_gte_write_data(0, r_u32(normal));
                xport_gte_write_data(1, r_u32(normal + 4));
                xport_gte_write_data(6, color);
                xport_gte_execute(0x108041B);
                color = xport_gte_read_data(22);
            }
            packet = r_u32(0x80040690);
            semi = r_u32(pass ? 0x80031148 : 0x80031144);
            w_u32(packet + 4, color);
            w_u8(packet + 7, semi + (pass ? 0x2C : 0x24));
            w_u8(packet + 3, pass ? 9 : 7);
            for (vertex = 0; vertex < corners; ++vertex)
                w_u32(packet + 8 + vertex * 8, point[vertex]);
            for (vertex = 0; vertex < corners; ++vertex)
                uv[vertex] = r_u16(face + 28 + vertex * 2);
            for (vertex = 0; vertex < corners; ++vertex)
                w_u16(packet + 12 + vertex * 8, uv[vertex]);
            clut = r_u16(face + 24);
            page = r_u16(face + 26);
            w_u16(packet + 14, clut);
            w_u16(packet + 22, page);
            slot = r_u32(0x800421AC);
            AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
            w_u32(0x80040690, r_u32(0x80040690) + (pass ? 40 : 32));
        }
    }
}

void render_emit_mesh_depth(uint32 mesh)
{
    uint32 face;
    sint32 counts[2];
    uint32 screen;
    uint32 depth_table;
    uint32 normals;
    uint32 pass;
    sint32 remaining;
    uint32 corners;
    uint32 vertex;
    uint32 offset[4];
    uint32 point[4];
    sint32 z[4];
    sint32 sum;
    uint32 normal;
    uint32 color;
    uint32 packet;
    uint32 semi;
    uint16 uv[4];
    uint16 clut;
    uint16 page;
    uint32 shift;
    uint32 depth;
    uint32 base;
    uint32 slot;
    FUNCTION_MARKER(0x8001E564, "SLUS_005.43");
    face = r_u32(mesh + 32);
    counts[0] = r_s32(mesh + 24);
    counts[1] = r_s32(mesh + 28);
    screen = r_u32(0x80046088);
    depth_table = r_u32(0x80041F4C);
    normals = r_u32(mesh + 8);
    (void)r_u32(mesh);
    for (pass = 0; pass < 2; ++pass)
    {
        corners = pass ? 4 : 3;
        for (remaining = counts[pass]; remaining > 0; --remaining, face += 60)
        {
            for (vertex = 0; vertex < corners; ++vertex)
                offset[vertex] = r_u32(face + 4 + vertex * 4) << 2;
            for (vertex = 0; vertex < corners; ++vertex)
                z[vertex] = r_s16(depth_table + (offset[vertex] >> 1));
            sum = z[0] + z[1] + z[2];
            if (pass)
                sum += z[3];
            if (pass ? sum <= 0 : !sum)
                continue;
            for (vertex = 0; vertex < corners; ++vertex)
                point[vertex] = r_u32(screen + offset[vertex]);
            if (NormalClip((sint32)point[0], (sint32)point[1], (sint32)point[2]) < 0)
                continue;
            normal = (r_u32(face + 20) << 3) + normals;
            color = r_u32(face + 36);
            xport_gte_write_data(0, r_u32(normal));
            xport_gte_write_data(1, r_u32(normal + 4));
            xport_gte_write_data(6, color);
            xport_gte_execute(0x108041B);
            color = xport_gte_read_data(22);
            packet = r_u32(0x80040690);
            semi = r_u32(pass ? 0x80031148 : 0x80031144);
            w_u32(packet + 4, color);
            w_u8(packet + 7, semi + (pass ? 0x2C : 0x24));
            w_u8(packet + 3, pass ? 9 : 7);
            for (vertex = 0; vertex < corners; ++vertex)
                w_u32(packet + 8 + vertex * 8, point[vertex]);
            for (vertex = 0; vertex < corners; ++vertex)
                uv[vertex] = r_u16(face + 28 + vertex * 2);
            for (vertex = 0; vertex < corners; ++vertex)
                w_u16(packet + 12 + vertex * 8, uv[vertex]);
            clut = r_u16(face + 24);
            page = r_u16(face + 26);
            w_u16(packet + 14, clut);
            w_u16(packet + 22, page);
            for (vertex = 0; vertex < corners; ++vertex)
                z[vertex] = r_s16(depth_table + (offset[vertex] >> 1));
            sum = pass ? z[0] : z[0] * 2;
            sum += z[1] + z[2];
            if (pass)
                sum += z[3];
            shift = r_u32(0x80053670);
            depth = ((uint32)sum >> 2) << (shift & 31);
            base = r_u32(0x800421AC);
            if ((sint32)depth < 0x4000)
            {
                if ((sint32)depth < 0)
                    depth = 0;
                slot = base + ((depth >> 6) + 64) * 4;
            }
            else
            {
                if ((sint32)depth > 0x3FFFFE)
                    depth = 0x3FFFFE;
                slot = base + ((depth >> 11) + 320) * 4;
            }
            AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
            w_u32(0x80040690, r_u32(0x80040690) + (pass ? 40 : 32));
        }
    }
}

uint32 resource_load_front(const char *name)
{
    uint32 size;
    uint32 buffer;
    FUNCTION_MARKER(0x8001ED48, "SLUS_005.43");
    size = disc_file_size(name);
    buffer = heap_alloc_front(size);
    if (buffer)
        resource_load_into(name, buffer);
    return buffer;
}

sint32 disc_wait_valid(void)
{
    FUNCTION_MARKER(0x80028ADC, "SLUS_005.43");
    return game_validate_data_disc();
}

void resource_init_index(void)
{
    FILE *file;
    long length;
    uint32 index;
    uint32 entry;
    uint32 count;
    uint32 offset;
    uint32 previous = 0;
    FUNCTION_MARKER(0x8001F320, "SLUS_005.43");
    w_u32(0x8003117C, 0);
    w_u32(0x800421A8, 0x80053B4C);
    w_u8(0x80053B4C, 0);
    /* The resource archive is identical on both discs */
    file = xport_fopen("DATA/1/GAME.RSC", "rb");
    if (!file)
        return;
    if (r_u32(0x80040FE8) != 12)
    {
        w_u32(0x80040FE8, 12);
        w_u32(0x80040FE4, 4);
    }
    if (fseek(file, 0, SEEK_END) || (length = ftell(file)) < 0 || fseek(file, 0, SEEK_SET))
    {
        int error = errno;
        fclose(file);
        errno = error ? error : EIO;
        return;
    }
    if ((uint64)length > 0xFFFFFFFF)
    {
        fclose(file);
        errno = EINVAL;
        return;
    }
    errno = 0;
    if (xport_fread(psx_addr(0x80053B48, 16384), 1, 16384, file) != 16384)
    {
        int error = errno;
        fclose(file);
        w_u8(0x80053B4C, 0);
        errno = error ? error : EIO;
        return;
    }
    fclose(file);
    count = r_u32(0x80053B48) + 1;
    w_u32(0x80053B48, count);
    if (count == 0 || count >= 511)
    {
        diagnostic_report(count, 0x80011AC4, (intptr)0x80011ADC);
        w_u8(0x80053B4C, 0);
        errno = EINVAL;
        return;
    }
    for (index = 0; index < count; ++index)
    {
        entry = 0x80053B4C + index * 32;
        offset = r_u32(entry + 16);
        if (offset < previous || offset > (uint32)length || offset < 4 + count * 32 ||
            (index + 1 == count ? r_u8(entry) != 0 :
             r_u8(entry) == 0 || !memchr(psx_addr(entry, 16), 0, 16)))
        {
            w_u8(0x80053B4C, 0);
            errno = EINVAL;
            return;
        }
        previous = offset;
    }
    w_u32(0x8003117C, 1);
#ifndef NDEBUG
    printf("There are %d files in resource file\n", (sint32)count);
#endif
}

void overlay_check_heap(void)
{
    char detail[104];
    uint32 expected;
    uint32 before;
    uint32 loaded;
    uint32 allocations;
    uint32 after;
    FUNCTION_MARKER(0x8001F758, "SLUS_005.43");
    expected = r_u32(0x8004EAC4);
    expected += r_u32(0x8004EACC);
    before = r_u32(0x8004EAC0);
    if (before != expected)
    {
        before = r_u32(0x8004EAC0);
        allocations = r_u32(0x8004EACC);
        after = r_u32(0x8004EAC4);
        sprintf(detail, "\n%d", (sint32)(after - (before + allocations)));
        diagnostic_report(100, 0x80011B14, (intptr)detail);
        return;
    }
    before = r_u32(0x8004EAB8);
    loaded = r_u32(0x8004EAC8);
    allocations = r_u32(0x8004EACC);
    after = r_u32(0x8004EABC);
    if (after < before + loaded - (allocations << 2))
    {
        diagnostic_report(100, 0x80011B2C, 0);
        return;
    }
    before = r_u32(0x8004EAB8);
    loaded = r_u32(0x8004EAC8);
    allocations = r_u32(0x8004EACC);
    after = r_u32(0x8004EABC);
    if (before + loaded + (allocations << 2) < after)
        diagnostic_report(100, 0x80011B2C, 0);
}

void render_subdivide_quad_large(uint32 mesh, uint32 face)
{
    static const uint32 original_slots[4] = {0, 4, 20, 24};
    static const uint32 midpoint_pairs[21][3] = {
        {0, 4, 2}, {0, 20, 10}, {4, 24, 14}, {20, 24, 22}, {2, 22, 12},
        {0, 2, 1}, {0, 10, 5}, {2, 12, 7}, {10, 12, 11}, {1, 11, 6},
        {2, 4, 3}, {4, 14, 9}, {12, 14, 13}, {3, 13, 8},
        {10, 20, 15}, {12, 22, 17}, {20, 22, 21}, {11, 21, 16},
        {14, 24, 19}, {22, 24, 23}, {13, 23, 18}};
    static const uint32 quads[16][4] = {
        {0, 1, 5, 6}, {1, 2, 6, 7}, {2, 3, 7, 8}, {3, 4, 8, 9},
        {5, 6, 10, 11}, {6, 7, 11, 12}, {7, 8, 12, 13}, {8, 9, 13, 14},
        {10, 11, 15, 16}, {11, 12, 16, 17}, {12, 13, 17, 18}, {13, 14, 18, 19},
        {15, 16, 20, 21}, {16, 17, 21, 22}, {17, 18, 22, 23}, {18, 19, 23, 24}};
    uint32 scratch;
    uint32 vertices;
    uint32 source[4];
    uint32 words[4];
    sint32 coordinate[6];
    uint32 component[4];
    uint32 index;
    uint32 axis;
    uint32 pair;
    uint32 first;
    uint32 second;
    uint32 destination;
    uint32 normal;
    uint32 color;
    uint16 page;
    uint16 clut;
    uint32 point[4];
    uint16 uv[4];
    uint32 sum;
    uint32 depth;
    uint32 packet;
    uint32 semi;
    uint32 base;
    uint32 slot;
    FUNCTION_MARKER(0x8001BF44, "SLUS_005.43");
    (void)r_u32(0x80046088);
    scratch = r_u32(0x80042908);
    for (index = 0; index < 4; ++index)
        source[index] = r_u32(face + 4 + index * 4) << 3;
    vertices = r_u32(mesh);
    for (index = 0; index < 4; ++index)
        source[index] += vertices;
    for (axis = 0; axis < 2; ++axis)
    {
        for (index = 0; index < 4; ++index)
            words[index] = r_u32(source[index] + axis * 4);
        for (index = 0; index < 4; ++index)
            w_u32(scratch + original_slots[index] * 8 + axis * 4, words[index]);
    }
    for (pair = 0; pair < 21; ++pair)
    {
        first = scratch + midpoint_pairs[pair][0] * 8;
        second = scratch + midpoint_pairs[pair][1] * 8;
        destination = scratch + midpoint_pairs[pair][2] * 8;
        for (axis = 0; axis < 3; ++axis)
            coordinate[axis] = r_s16(first + axis * 2);
        for (axis = 0; axis < 3; ++axis)
            coordinate[axis + 3] = r_s16(second + axis * 2);
        for (axis = 0; axis < 3; ++axis)
            w_u16(destination + axis * 2, (coordinate[axis] + coordinate[axis + 3]) >> 1);
    }
    for (index = 0; index < 4; ++index)
        uv[index] = r_u16(face + 28 + index * 2);
    for (index = 0; index < 4; ++index)
        w_u16(scratch + 400 + original_slots[index] * 2, uv[index]);
    for (pair = 0; pair < 21; ++pair)
    {
        first = scratch + 400 + midpoint_pairs[pair][0] * 2;
        second = scratch + 400 + midpoint_pairs[pair][1] * 2;
        destination = scratch + 400 + midpoint_pairs[pair][2] * 2;
        component[0] = r_u8(first);
        component[1] = r_u8(first + 1);
        component[2] = r_u8(second);
        component[3] = r_u8(second + 1);
        w_u8(destination, (component[0] + component[2]) >> 1);
        w_u8(destination + 1, (component[1] + component[3]) >> 1);
    }
    for (index = 0; index < 25; ++index)
    {
        words[0] = r_u32(scratch + index * 8);
        words[1] = r_u32(scratch + index * 8 + 4);
        xport_gte_write_data(0, words[0]);
        xport_gte_write_data(1, words[1]);
        xport_gte_execute(0x180001);
        w_u32(scratch + 200 + index * 4, xport_gte_read_data(14));
        w_u32(scratch + 300 + index * 4, xport_gte_read_data(19));
    }
    page = r_u16(face + 26);
    clut = r_u16(face + 24);
    normal = r_u32(face + 20) << 3;
    normal += r_u32(mesh + 8);
    color = r_u32(face + 36);
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    xport_gte_write_data(6, color);
    xport_gte_execute(0x108041B);
    color = xport_gte_read_data(22);
    for (pair = 0; pair < 16; ++pair)
    {
        for (index = 0; index < 4; ++index)
            words[index] = r_u32(scratch + 300 + quads[pair][index] * 4);
        sum = words[0] + words[1] + words[2] + words[3];
        if (!sum)
            continue;
        depth = sum >> 2;
        for (index = 0; index < 4; ++index)
            point[index] = r_u32(scratch + 200 + quads[pair][index] * 4);
        for (index = 0; index < 4; ++index)
            uv[index] = r_u16(scratch + 400 + quads[pair][index] * 2);
        packet = r_u32(0x80040690);
        semi = r_u32(0x80031148);
        w_u32(packet + 4, color);
        w_u8(packet + 7, semi + 0x2C);
        w_u8(packet + 3, 9);
        for (index = 0; index < 4; ++index)
            w_u32(packet + 8 + index * 8, point[index]);
        for (index = 0; index < 4; ++index)
            w_u16(packet + 12 + index * 8, uv[index]);
        w_u16(packet + 14, clut);
        w_u16(packet + 22, page);
        depth <<= r_u32(0x80053670) & 31;
        base = r_u32(0x800421AC);
        if ((sint32)depth < 0x4000)
        {
            if ((sint32)depth < 0)
                depth = 0;
            slot = base + ((depth >> 6) + 64) * 4;
        }
        else
        {
            if ((sint32)depth > 0x3FFFFE)
                depth = 0x3FFFFE;
            slot = base + ((depth >> 11) + 320) * 4;
        }
        AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
        w_u32(0x80040690, r_u32(0x80040690) + 40);
    }
}

void disc_set_event_result(uint32 result)
{
    FUNCTION_MARKER(0x8001FD04, "SLUS_005.43");
    w_u32(0x80031178, result);
}

uint32 disc_poll_event_result(void)
{
    FUNCTION_MARKER(0x8001FD14, "SLUS_005.43");
    if (TestEvent(r_s32(0x800428C4)))
        return r_u32(0x80031178);
    return 0;
}

uint32 disc_sector_position(uint32 sector, CdlLOC *position)
{
    uint32 minute;
    uint32 minute_tens;
    uint32 second;
    uint32 second_tens;
    uint32 frame;
    uint32 frame_tens;
    FUNCTION_MARKER(0x8001FE0C, "SLUS_005.43");
    sector += r_u32(0x8004068C);
    minute = sector / 4500;
    minute_tens = sector / 45000;
    second = (sector % 4500) / 75;
    second_tens = (sector % 4500) / 750;
    frame = sector % 75;
    frame_tens = frame / 10;
    position->track = 1;
    position->minute = (uint8)((minute_tens << 4) + minute - minute_tens * 10);
    position->second = (uint8)((second_tens << 4) + second - second_tens * 10);
    position->sector = (uint8)((frame_tens << 4) + frame - frame_tens * 10);
    return frame_tens * 10;
}

sint32 disc_read_pair(uint32 sector, uint32 destination)
{
    CdlLOC position;
    FUNCTION_MARKER(0x8001FF24, "SLUS_005.43");
    disc_sector_position(sector, &position);
    disc_set_event_result(0x1000);
    CdControlF(21, (uint8 *)&position);
    while (disc_poll_event_result() != 0x1000)
    {
    }
    CdRead(2, (uint32 *)psx_addr(destination, 4096), 128);
    return CdReadSync(0, 0);
}

uint32 disc_count_files(uint32 sector, uint32 buffer)
{
    uint32 cursor;
    uint32 count;
    uint32 total;
    uint32 window;
    uint32 limit;
    uint32 high;
    uint32 next;
    FUNCTION_MARKER(0x8001FF98, "SLUS_005.43");
    cursor = buffer;
    count = 0;
    total = 0;
    disc_read_pair(sector, buffer);
    window = 4096;
    high = r_u16(buffer + 12);
    limit = r_u16(buffer + 10) | (high << 16);
    for (;;)
    {
        if (!(r_u8(cursor + 25) & 2))
            ++count;
        cursor += r_u8(cursor);
        next = r_u8(cursor);
        total += next;
        if (total >= limit)
            break;
        if (cursor - buffer < 4096)
        {
            if (next)
                continue;
            if (window >= limit)
                break;
        }
        ++sector;
        disc_read_pair(sector, buffer);
        window += 4096;
    }
    return count;
}

uint32 disc_build_file_table(uint32 sector, uint32 buffer)
{
    uint32 cursor;
    uint32 count;
    uint32 table;
    uint32 entry;
    uint32 total;
    uint32 window;
    uint32 limit;
    uint32 high;
    uint32 low;
    uint32 length;
    uint32 next;
    FUNCTION_MARKER(0x8002009C, "SLUS_005.43");
    cursor = buffer;
    total = 0;
    count = disc_count_files(sector, buffer);
    table = heap_alloc_back((count - 1) * 24 + 28);
    if (!table)
        return 0;
    w_u32(table, count);
    disc_read_pair(sector, buffer);
    window = 4096;
    entry = table;
    high = r_u16(buffer + 12);
    limit = r_u16(buffer + 10) | (high << 16);
    for (;;)
    {
        if (!(r_u8(cursor + 25) & 2))
        {
            length = r_u8(cursor + 32);
            memcpy(psx_addr(entry + 4, length), psx_addr(cursor + 33, length), length);
            length = r_u8(cursor + 32);
            w_u8(entry + length + 5, 0);
            high = r_u16(cursor + 12);
            low = r_u16(cursor + 10);
            w_u32(entry + 20, low | (high << 16));
            high = r_u16(cursor + 4);
            low = r_u16(cursor + 2);
            w_u32(entry + 24, low | (high << 16));
            entry += 24;
        }
        cursor += r_u8(cursor);
        next = r_u8(cursor);
        total += next;
        if (total >= limit)
            break;
        if (cursor - buffer < 4096)
        {
            if (next)
                continue;
            if (window >= limit)
                break;
        }
        ++sector;
        disc_read_pair(sector, buffer);
        window += 4096;
    }
    return table;
}

uint32 disc_build_directory_tables(uint32 records, uint32 buffer)
{
    uint32 record;
    uint32 high;
    uint32 sector;
    uint32 length;
    uint16 parent;
    uint32 table;
    FUNCTION_MARKER(0x80020264, "SLUS_005.43");
    record = records + 4;
    while (record - records < r_u32(records))
    {
        high = r_u16(record + 4);
        sector = r_u16(record + 2) | (high << 16);
        length = r_u8(record);
        parent = r_u16(record + 6);
        if (!(record & 3))
        {
            w_u16(record, length);
            w_u16(record + 2, parent);
            table = disc_build_file_table(sector, buffer);
            w_u32(record + 4, table);
        }
        else
        {
            w_u16(record, length);
            table = disc_build_file_table(sector, buffer);
            w_u32(record + 2, table);
            w_u16(record + 6, parent);
        }
        record += length + 8;
        if (length & 1)
            ++record;
    }
    return 0;
}

sint32 disc_event_fatal(void)
{
    FUNCTION_MARKER(0x8001FDD4, "SLUS_005.43");
    diagnostic_report(666, 0x80011B44, (intptr)0x80011B4C);
    exit(-1);
}

void disc_init_events(void)
{
    sint32 handle;
    FUNCTION_MARKER(0x80020328, "SLUS_005.43");
    EnterCriticalSectionPSX();
    handle = OpenEvent(0xF0000003, 0x20, 0x2000, 0);
    w_u32(0x800428C4, (uint32)handle);
    handle = OpenEvent(0xF0000003, 0x8000, 0x1000, disc_event_fatal);
    w_u32(0x800520F8, (uint32)handle);
    EnableEvent(r_s32(0x800428C4));
    EnableEvent(r_s32(0x800520F8));
    ExitCriticalSection();
}

uint32 disc_toc_raw_ticks(uint32 position)
{
    uint32 second;
    uint32 frame;
    uint32 minute;
    FUNCTION_MARKER(0x800203B4, "SLUS_005.43");
    second = r_u8(position + 1);
    frame = r_u8(position + 2);
    minute = r_u8(position);
    return frame + second * 75 + minute * 4500;
}

sint32 disc_init_directory(sint32 toc_only)
{
    uint32 scratch;
    uint32 entry;
    uint32 index;
    uint32 value;
    uint32 lead;
    uint32 size;
    uint32 table;
    sint32 handle;
    sint32 sectors;
    sint32 slot;
    CdlLOC position;
    FUNCTION_MARKER(0x800203F8, "SLUS_005.43");
    scratch = heap_alloc_back(0x1060);
    EnterCriticalSectionPSX();
    handle = OpenEvent(0xF0000003, 0x20, 0x2000, 0);
    w_u32(0x800428C4, (uint32)handle);
    handle = OpenEvent(0xF0000003, 0x8000, 0x1000, disc_event_fatal);
    w_u32(0x800520F8, (uint32)handle);
    EnableEvent(r_s32(0x800428C4));
    entry = 0x800422B0;
    EnableEvent(r_s32(0x800520F8));
    ExitCriticalSection();
    CdGetToc2(1, (CdlLOC *)psx_addr(0x800422B0, 400));
    for (index = 0; index < 100; ++index)
    {
        if (!disc_toc_raw_ticks(entry))
        {
            value = r_u32(0x800422B0);
            w_u32(entry, value);
            break;
        }
        entry += 4;
    }
    if (toc_only)
        return 0;
    value = r_u8(0x800422B5);
    lead = ((value >> 4) * 10 + (value & 15)) * 75;
    value = r_u8(0x800422B6);
    lead += (value >> 4) * 10 + (value & 15);
    w_u32(0x8004068C, lead);
    disc_sector_position(16, &position);
    disc_set_event_result(0x1000);
    CdControlF(21, (uint8 *)&position);
    while (disc_poll_event_result() != 0x1000)
    {
    }
    CdRead(1, (uint32 *)psx_addr(scratch, 2048), 128);
    CdReadSync(0, 0);
    if (r_u8(scratch) != 1)
        return 0;
    size = r_u32(scratch + 132);
    table = heap_alloc_back(size + 4);
    w_u32(0x80031170, table);
    if (!table)
        return 0;
    w_u32(table, size);
    value = r_u32(scratch + 140);
    disc_sector_position(value, &position);
    disc_set_event_result(0x1001);
    CdControlF(21, (uint8 *)&position);
    while (disc_poll_event_result() != 0x1001)
    {
    }
    sectors = (sint32)((uint32)((sint32)size >> 11) + 1);
    CdRead(sectors, (uint32 *)psx_addr(scratch, sectors > 0 ? (size_t)sectors * 2048 : 0), 128);
    CdReadSync(0, 0);
    table = r_u32(0x80031170);
    memcpy(psx_addr(table + 4, size), psx_addr(scratch, size), size);
    table = r_u32(0x80031170);
    disc_build_directory_tables(table, scratch);
    w_u32(0x80031174, 0);
    for (slot = 240; slot >= 0; slot -= 16)
        w_u32(0x80040698 + (uint32)slot, 0);
    heap_free(scratch);
    return 1;
}

sint32 disc_find_directory(const char *name, uint32 parent, uint32 *directory_number, uint32 *file_table)
{
    uint32 root;
    uint32 record;
    uint32 number;
    uint32 length;
    uint32 record_parent;
    uint32 table;
    uint32 index;
    uint32 character;
    sint32 mismatch;
    FUNCTION_MARKER(0x800206A4, "SLUS_005.43");
    root = r_u32(0x80031170);
    record = root + 4;
    number = 0;
    while (record - root < r_u32(root))
    {
        length = r_u16(record);
        if (!(record & 3))
        {
            record_parent = r_u16(record + 2);
            table = r_u32(record + 4);
        }
        else
        {
            table = r_u32(record + 2);
            record_parent = r_u16(record + 6);
        }
        ++number;
        if (record_parent == parent)
        {
            mismatch = 0;
            for (index = 0; index < length; ++index)
            {
                character = r_u8(record + 8 + index);
                if (character != (uint8)name[index])
                {
                    mismatch = 1;
                    break;
                }
            }
            if (!mismatch)
            {
                character = (uint8)name[index];
                if ((character == '\\' || !character) && index == length)
                {
                    *file_table = table;
                    *directory_number = number;
                    return 1;
                }
            }
        }
        record += length + 8;
        if (length & 1)
            ++record;
        root = r_u32(0x80031170);
    }
    return 0;
}

const char *disc_next_component(const char *path)
{
    uint8 character;
    FUNCTION_MARKER(0x800207CC, "SLUS_005.43");
    for (;; ++path)
    {
        character = (uint8)*path;
        if (!character)
            return 0;
        if (character == '\\')
            return path + 1;
    }
}

sint32 disc_find_file(const char *name, uint32 *sector_size)
{
    const char *leaf;
    const char *input;
    uint32 parent;
    uint32 table;
    uint32 entry;
    uint32 offset;
    uint32 matched;
    uint32 character;
    uint32 initial;
    sint32 count;
    sint32 index;
    sint32 equal;
    FUNCTION_MARKER(0x80020804, "SLUS_005.43");
    if ((uint8)*name == '\\')
        ++name;
    if (!disc_next_component(name))
    {
        leaf = name;
        table = r_u32(r_u32(0x80031170) + 8);
    }
    else
    {
        parent = 1;
        for (;;)
        {
            if (!disc_find_directory(name, parent, &parent, &table))
                return 0;
            leaf = disc_next_component(name);
            if (!disc_next_component(leaf))
                break;
            name = leaf;
            if (!(uint8)*name)
                break;
        }
    }
    count = r_s32(table);
    if (count <= 0)
        return 0;
    initial = (uint8)*leaf;
    offset = 4;
    for (index = 0; index < count; ++index, offset += 24)
    {
        entry = table + offset;
        matched = 0;
        equal = 1;
        if (initial)
        {
            input = leaf;
            while (equal)
            {
                character = r_u8(entry + matched);
                if (character != (uint8)*input)
                    equal = 0;
                ++input;
                character = (uint8)*input;
                ++matched;
                if (!character)
                    break;
            }
        }
        if (equal && r_u8(entry + matched) == ';')
        {
            entry = table + (uint32)index * 24;
            sector_size[0] = r_u32(entry + 24);
            sector_size[1] = r_u32(entry + 20);
            return 1;
        }
    }
    return 0;
}

uint32 path_uppercase_ascii(char *path)
{
    uint32 character;
    FUNCTION_MARKER(0x800209E0, "SLUS_005.43");
    character = (uint8)*path;
    while (character)
    {
        if (character - 97 < 26)
            *path = (char)(uint8)(character + 224);
        ++path;
        character = (uint8)*path;
    }
    return (uint32)-97;
}

sint32 disc_file_seek_step(uint32 handle, uint32 offset)
{
    uint32 entry;
    uint32 sector;
    CdlLOC position;
    FUNCTION_MARKER(0x80020B00, "SLUS_005.43");
    entry = 0x800406A4 + ((handle - 1) << 4);
    sector = r_u32(entry) + offset;
    w_u32(entry, sector);
    disc_sector_position(sector, &position);
    TestEvent(r_s32(0x800428C4));
    w_u32(entry, r_u32(entry) + 1);
    if (CdControlF(21, (uint8 *)&position))
        return 1;
    diagnostic_report(666, 0x80011B50, (intptr)0x80011B4C);
    return 0;
}

sint32 disc_file_seek_span(uint32 handle, uint32 sectors)
{
    uint32 entry;
    CdlLOC position;
    FUNCTION_MARKER(0x80020BC0, "SLUS_005.43");
    entry = 0x800406A4 + ((handle - 1) << 4);
    disc_sector_position(r_u32(entry), &position);
    TestEvent(r_s32(0x800428C4));
    w_u32(entry, r_u32(entry) + sectors);
    if (CdControlF(21, (uint8 *)&position))
        return 1;
    diagnostic_report(666, 0x80011B64, (intptr)0x80011B4C);
    return 0;
}

uint32 disc_load_file_back(const char *name)
{
    FUNCTION_MARKER(0x80020CC4, "SLUS_005.43");
    return disc_load_named_file_back(name);
}

void disc_set_cd_volume(sint16 volume)
{
    SpuCommonAttr attribute;
    FUNCTION_MARKER(0x80020D40, "SLUS_005.43");
    attribute.mask = 0x3C0;
    attribute.cd.volume.left = volume;
    attribute.cd.volume.right = volume;
    attribute.cd.reverb = 0;
    attribute.cd.mix = 1;
    SpuSetCommonAttr(&attribute);
}

char *path_build_data_filename(char *destination, const char *name)
{
    const char *leaf;
    const char *separator;
    FUNCTION_MARKER(0x80020EA0, "SLUS_005.43");
    leaf = name;
    separator = strchr(name, '\\');
    while (separator)
    {
        leaf = separator + 1;
        separator = strchr(leaf, '\\');
    }
    strcpy(destination, "..\\DATA\\");
    return strcat(destination, leaf);
}

sint32 disc_file_exists(const char *name)
{
    uint32 length;
    FUNCTION_MARKER(0x80021338, "SLUS_005.43");
    return game_extracted_file_size(name, &length);
}

uint32 render_select_buffer(uint32 entries)
{
    uint32 owner;
    uint32 packet;
    FUNCTION_MARKER(0x80021AE8, "SLUS_005.43");
    owner = r_u32(0x80042170);
    if (r_u32(0x8004608C) == owner)
        owner += 0x45F4;
    w_u32(0x8004608C, owner);
    ClearOTagR((uint32 *)psx_addr(owner + 112, (sint32)entries > 0 ? entries * 4 : 4), (sint32)entries);
    w_u32(0x80034D78, entries - 1);
    owner = r_u32(0x8004608C);
    w_u32(0x800520FC, 0xFFFFFFFF);
    packet = r_u32(owner + 0x45F0);
    w_u32(0x800421AC, owner + 116);
    w_u32(0x80040690, packet);
    w_u32(0x8004217C, packet);
    return owner + 116;
}

void render_draw_complete(void)
{
    uint32 counter;
    FUNCTION_MARKER(0x80021B7C, "SLUS_005.43");
    counter = (uint32)VSync(1);
    w_u32(0x80042180, counter - r_u32(0x8004EAF0));
    DrawSyncCallback(0);
}

void render_check_lists(void)
{
    FUNCTION_MARKER(0x80021DAC, "SLUS_005.43");
    if (r_u32(r_u32(r_u32(0x80042170) + 0x45F0) + 0x18FFC) != 0x12345678)
        diagnostic_report(330, 0x80011BE0, (intptr)0x80011BF8);
    if (r_u32(r_u32(r_u32(0x80042170) + 0x8BE4) + 0x18FFC) != 0x12345678)
        diagnostic_report(330, 0x80011BFC, (intptr)0x80011BF8);
}

uint32 render_draw_centered_sprite(uint32 index, uint32 x, uint32 y)
{
    uint32 texture;
    FUNCTION_MARKER(0x80021E58, "SLUS_005.43");
    texture = 0x8004EB70 + (index << 4);
    x -= (uint32)(r_s16(texture + 14) >> 1);
    y -= (uint32)(r_s16(texture + 10) >> 1);
    return render_draw_sprite(x, y, texture, texture, 1);
}

uint32 render_draw_line(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
    uint32 packet;
    uint32 next;
    FUNCTION_MARKER(0x80021F10, "SLUS_005.43");
    packet = r_u32(0x80040690);
    w_u32(0x80042904, packet);
    setLineF2((LINE_F2 *)psx_addr(packet, sizeof(LINE_F2)));
    packet = r_u32(0x80042904);
    w_u8(packet + 4, r_u8(0x8004617C));
    packet = r_u32(0x80042904);
    w_u8(packet + 5, r_u8(0x80033D38));
    packet = r_u32(0x80042904);
    w_u8(packet + 6, r_u8(0x80053680));
    packet = r_u32(0x80042904);
    w_u16(packet + 8, x0);
    w_u16(packet + 10, y0);
    w_u16(packet + 12, x1);
    w_u16(packet + 14, y1);
    render_insert_primitive(r_s32(0x800520FC), packet);
    next = r_u32(0x80042904) + 16;
    w_u32(0x80042904, next);
    w_u32(0x80040690, next);
    return next;
}

uint32 render_draw_sprite(uint32 x, uint32 y, uint32 texture, uint32 palette, uint32 flags)
{
    uint32 packet;
    uint32 mode_packet;
    uint32 shift;
    uint32 owner;
    sint32 format;
    uint16 clut;
    uint16 page;
    FUNCTION_MARKER(0x80021FF8, "SLUS_005.43");
    packet = r_u32(0x80040690);
    format = r_s16(texture + 12);
    shift = (2 - (uint32)format) & 31;
    SetSprt((SPRT *)psx_addr(packet, sizeof(SPRT)));
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    w_u8(packet + 12, (r_u16(texture) & 63) << shift);
    w_u8(packet + 13, r_u16(texture + 2));
    w_u16(packet + 16, (uint32)r_s16(texture + 8) << shift);
    w_u16(packet + 18, r_u16(texture + 10));
    clut = GetClut(r_s16(palette + 4), r_s16(palette + 6));
    w_u16(packet + 14, clut);
    if (flags & 1)
    {
        w_u8(packet + 4, r_u8(0x8004617C));
        w_u8(packet + 5, r_u8(0x80033D38));
        w_u8(packet + 6, r_u8(0x80053680));
    }
    else
        SetShadeTex((SPRT *)psx_addr(packet, sizeof(SPRT)), 1);
    if (flags & 2)
        w_u8(packet + 7, r_u8(packet + 7) | 2);
    mode_packet = packet + 20;
    owner = r_u32(0x8004608C);
    AddPrim(psx_addr(owner + 112, 4), psx_addr(packet, 4));
    w_u32(0x80040690, mode_packet);
    page = GetTPage(format, 1, r_s16(texture), r_s16(texture + 2));
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, sizeof(DR_MODE)), 0, 1, page, 0);
    owner = r_u32(0x8004608C);
    AddPrim(psx_addr(owner + 112, 4), psx_addr(mode_packet, 4));
    w_u32(0x80040690, mode_packet + 12);
    return r_u32(owner + 112);
}
