#include "game_functions.h"
#include <string.h>

uint32 mesh_extract_fragment(sint32 selected_count, uint32 selected_faces, uint32 source_mesh)
{
    VECTOR first = {0};
    VECTOR second = {0};
    sint32 *components[6] = {&first.vx, &first.vy, &first.vz, &second.vx, &second.vy, &second.vz};
    uint32 sums[3] = {0, 0, 0};
    uint32 center[3];
    uint16 coordinates[3];
    uint32 boundaries;
    uint32 record;
    uint32 extra_faces;
    uint32 triangles;
    uint32 quads;
    uint32 unique_count;
    uint32 mesh;
    uint32 allocation;
    uint32 face;
    uint32 input_face;
    uint32 face_cursor;
    uint32 temporary;
    uint32 vertex;
    uint32 destination;
    uint32 value;
    uint32 color;
    uint32 last_uv;
    uint32 pass;
    uint32 field;
    uint32 corner;
    uint32 axis;
    uint32 side;
    uint32 a;
    uint32 b;
    uint32 vertices;
    uint32 next;
    sint32 boundary_count;
    sint32 index;
    sint32 scan;
    sint32 found;
    sint32 edge;
    sint32 divisor;
    sint32 remainder;
    sint32 left;
    sint32 right;
    FUNCTION_MARKER(0x80016E6C, "SLUS_005.43");
    boundary_count = (sint32)vector_pack_xy(r_u32(0x800421B0));
    boundaries = r_u32(0x800421B0) + 4;
    w_u32(0x800421B0, boundaries);
    extra_faces = 0;
    for (index = 0; index < boundary_count; ++index)
    {
        record = r_u32(0x800421B0);
        value = r_u32(record);
        w_u32(0x800421B0, record + 76);
        extra_faces += value;
    }
    triangles = 0;
    quads = 0;
    unique_count = 0;
    mesh = heap_alloc_back(52);
    w_u32(mesh + 12, 0);
    value = (uint32)selected_count + extra_faces;
    w_u32(mesh + 20, value);
    allocation = heap_alloc_back(value * 60);
    w_u32(mesh + 16, allocation);
    allocation = heap_alloc_back(value << 3);
    w_u32(mesh + 8, allocation);
    face_cursor = r_u32(mesh + 16);
    w_u32(mesh + 32, face_cursor);
    face_cursor += extra_faces * 60;
    for (pass = 0; pass < 2; ++pass)
    {
        if (pass)
        {
            w_u32(mesh + 36, face_cursor);
            w_u32(mesh + 24, triangles + extra_faces);
        }
        for (index = 0; index < selected_count; ++index)
        {
            value = r_u32(selected_faces + (uint32)index * 4);
            input_face = r_u32(source_mesh + 16) + value * 60;
            value = r_u32(input_face);
            if (value != (pass ? 4u : 3u))
                continue;
            w_u32(face_cursor, value);
            for (field = 4; field <= 20; field += 4)
                w_u32(face_cursor + field, r_u32(input_face + field));
            for (field = 24; field <= 34; field += 2)
                w_u16(face_cursor + field, r_u16(input_face + field));
            color = r_u32(input_face + 36);
            w_u16(face_cursor + 40, 0);
            w_u16(face_cursor + 42, 0);
            w_u16(face_cursor + 44, 0);
            w_u16(face_cursor + 46, 0);
            w_u32(face_cursor + 36, color);
            w_u16(face_cursor + 48, r_u16(input_face + 48));
            w_u16(face_cursor + 50, r_u16(input_face + 50));
            w_u16(face_cursor + 52, r_u16(input_face + 52));
            w_u32(face_cursor + 56, r_u32(input_face + 56));
            face_cursor += 60;
            if (pass)
                ++quads;
            else
                ++triangles;
        }
    }
    w_u32(mesh + 28, quads);
    if (quads + triangles != (uint32)selected_count)
    {
#ifndef NDEBUG
        printf("warning!! - inconsistent face count!\n");
#endif
    }
    for (index = 0; index < selected_count; ++index)
    {
        face = r_u32(mesh + 16) + ((uint32)index + extra_faces) * 60;
        for (axis = 0; axis < 3; ++axis)
        {
            vertex = r_u32(face + 20) << 3;
            vertex += r_u32(source_mesh + 8);
            destination = r_u32(mesh + 8) + (uint32)index * 8;
            value = r_u16(vertex + axis * 2);
            w_u16(destination + axis * 2, value);
        }
        w_u32(face + 20, (uint32)index);
    }
    temporary = heap_alloc_back(r_u32(source_mesh + 4) << 3);
    destination = temporary;
    for (index = 0; index < selected_count; ++index)
    {
        face = r_u32(mesh + 16) + ((uint32)index + extra_faces) * 60;
        for (corner = 1; corner <= 4; ++corner)
        {
            if (corner == 4 && r_u32(face) != 4)
                break;
            for (axis = 0; axis < 3; ++axis)
            {
                vertex = r_u32(face + corner * 4) << 3;
                vertex += r_u32(source_mesh);
                coordinates[axis] = r_u16(vertex + axis * 2);
            }
            found = -1;
            for (scan = 0; scan < (sint32)unique_count; ++scan)
            {
                vertex = temporary + (uint32)scan * 8;
                if (r_s16(vertex) == (sint16)coordinates[0] && r_s16(vertex + 2) == (sint16)coordinates[1] && r_s16(vertex + 4) == (sint16)coordinates[2])
                    found = scan;
            }
            if (found < 0)
            {
                w_u32(face + corner * 4, unique_count);
                w_u16(destination, coordinates[0]);
                w_u16(destination + 2, coordinates[1]);
                ++unique_count;
                w_u16(destination + 4, coordinates[2]);
                destination += 8;
            }
            else
                w_u32(face + corner * 4, (uint32)found);
        }
    }
    value = unique_count + (uint32)boundary_count;
    w_u32(mesh + 4, value);
    allocation = heap_alloc_back(value << 3);
    w_u32(mesh, allocation);
    for (index = 0; index < (sint32)unique_count; ++index)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            destination = r_u32(mesh) + (uint32)index * 8;
            value = r_u16(temporary + (uint32)index * 8 + axis * 2);
            w_u16(destination + axis * 2, value);
        }
    }
    heap_free(temporary);
    divisor = 0;
    for (index = 0; index < (sint32)unique_count; ++index)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            vertex = r_u32(mesh) + (uint32)index * 8;
            sums[axis] += (uint32)r_s16(vertex + axis * 2);
        }
        ++divisor;
    }
    for (axis = 0; axis < 3; ++axis)
    {
        if (!divisor)
        {
            trap(7);
            center[axis] = (sint32)sums[axis] < 0 ? 1u : 0xFFFFFFFF;
        }
        else
            center[axis] = (uint32)((sint64)(sint32)sums[axis] / divisor);
    }
    w_u16(mesh + 40, center[0]);
    w_u16(mesh + 42, center[1]);
    w_u16(mesh + 44, center[2]);
    for (index = 0; index < (sint32)unique_count; ++index)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            vertex = r_u32(mesh) + (uint32)index * 8 + axis * 2;
            w_u16(vertex, r_u16(vertex) - center[axis]);
        }
    }
    face_cursor = r_u32(mesh + 16);
    record = boundaries;
    for (index = 0; index < boundary_count; ++index, record += 76)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            destination = r_u32(mesh) + ((uint32)index + unique_count) * 8;
            value = r_u16(record + 68 + axis * 2);
            w_u16(destination + axis * 2, value);
        }
        edge = 0;
        if (r_s32(record) <= 0)
            continue;
        do
        {
            divisor = r_s32(record);
            next = (uint32)edge + 1;
            if (!divisor)
            {
                trap(7);
                remainder = (sint32)next;
            }
            else if (divisor == -1 && next == 0x80000000)
            {
                trap(0x5D);
                remainder = 0;
            }
            else
                remainder = (sint32)((sint64)(sint32)next % divisor);
            face = face_cursor;
            w_u32(face, 3);
            value = (uint32)r_s16(record + 4 + (uint32)edge * 2);
            w_u32(face + 4, value);
            value = (uint32)r_s16(record + 4 + (uint32)remainder * 2);
            w_u32(face + 12, (uint32)index + unique_count);
            w_u32(face + 16, 0xFFFFFFFF);
            w_u32(face + 20, (uint32)edge + (uint32)selected_count);
            w_u32(face + 8, value);
            for (field = 24; field <= 32; field += 2)
            {
                input_face = r_u32(mesh + 16) + extra_faces * 60;
                value = r_u16(input_face + field);
                w_u16(face + field, value);
            }
            input_face = r_u32(mesh + 16) + extra_faces * 60;
            last_uv = r_u16(input_face + 34);
            w_u32(face + 36, 0x2050);
            w_u16(face + 40, 0);
            w_u16(face + 42, 0);
            w_u16(face + 44, 0);
            w_u16(face + 46, 0);
            w_u16(face + 48, 0);
            w_u16(face + 34, last_uv);
            w_u16(face + 50, 0);
            a = r_u32(face + 4);
            w_u16(face + 52, 0);
            w_u32(face + 56, 0);
            for (side = 0; side < 2; ++side)
            {
                for (axis = 0; axis < 3; ++axis)
                {
                    if (side || axis)
                        a = r_u32(face + 4);
                    vertices = r_u32(mesh);
                    b = r_u32(face + (side ? 12 : 8));
                    left = r_s16(vertices + (a << 3) + axis * 2);
                    right = r_s16(vertices + (b << 3) + axis * 2);
                    *components[side * 3 + axis] = left - right;
                }
            }
            vector_normalize(&first, &first);
            vector_normalize(&second, &second);
            OuterProduct12(&first, &second, &first);
            vector_normalize(&first, &first);
            face_cursor += 60;
            edge = (sint32)next;
            value = r_u32(face + 20) << 3;
            value += r_u32(mesh + 8);
            w_u16(value, 0 - (uint32)first.vx);
            value = r_u32(face + 20) << 3;
            value += r_u32(mesh + 8);
            w_u16(value + 2, 0 - (uint32)first.vy);
            value = r_u32(face + 20) << 3;
            value += r_u32(mesh + 8);
            w_u16(value + 4, 0 - (uint32)first.vz);
        } while (edge < r_s32(record));
    }
    w_u32(mesh + 48, extra_faces);
    return mesh;
}

uint32 vector_pack_xy(uint32 vector)
{
    uint32 y;
    uint32 x;
    FUNCTION_MARKER(0x80016E58, "SLUS_005.43");
    y = r_u16(vector + 2);
    x = r_u16(vector);
    return x | (y << 16);
}

uint32 model_load_bundle(uint32 name, uint32 page, uint32 clut_x)
{
    char path[256];
    uint32 palette[256];
    PSX_RECT rectangle;
    uint32 file_size;
    uint32 file;
    uint32 mesh;
    uint32 allocation;
    uint32 count;
    uint32 source;
    uint32 destination;
    uint32 value;
    uint32 component;
    sint32 index;
    FUNCTION_MARKER(0x80016BEC, "SLUS_005.43");
    sprintf(path, "%s.TPG", (const char *)psx_addr(name, 32));
    texture_load_page(page, path);
    sprintf(path, "%s.CLT", (const char *)psx_addr(name, 32));
    resource_load_into_native(path, palette, sizeof(palette));
    rectangle.y = 224;
    rectangle.w = 16;
    rectangle.x = (sint16)clut_x;
    rectangle.h = 32;
    LoadImagePSX(&rectangle, palette);
    DrawSync(0);
    sprintf(path, "%s.MOD", (const char *)psx_addr(name, 32));
    file_size = disc_file_size(path);
    file = heap_alloc_back(file_size);
    resource_load_into(path, file);
    mesh = heap_alloc_back(52);
    value = r_u32(file + 20);
    w_u32(mesh + 20, value);
    value = r_u32(file + 12);
    count = r_u32(mesh + 20);
    w_u32(mesh + 12, value);
    value = r_u32(file + 24);
    w_u32(mesh + 24, value);
    value = r_u32(file + 28);
    w_u32(mesh + 28, value);
    value = r_u32(file + 4);
    w_u32(mesh + 4, value);
    allocation = heap_alloc_back(count * 60);
    count = r_u32(mesh + 4);
    w_u32(mesh + 16, allocation);
    allocation = heap_alloc_back(count << 3);
    count = r_u32(mesh + 20);
    w_u32(mesh, allocation);
    allocation = heap_alloc_back(count << 3);
    source = file + 52;
    destination = r_u32(mesh);
    count = r_u32(mesh + 4);
    w_u32(mesh + 8, allocation);
    index = 0;
    if ((sint32)count > 0)
    {
        do
        {
            value = r_u32(source);
            source += 4;
            ++index;
            w_u32(destination, value);
            destination += 4;
            value = r_u32(source);
            source += 4;
            w_u32(destination, value);
            count = r_u32(mesh + 4);
            destination += 4;
        } while (index < (sint32)count);
    }
    count = r_u32(mesh + 20);
    for (index = 0; index < (sint32)count; ++index)
        source += 8;
    count = r_u32(mesh + 20);
    destination = r_u32(mesh + 16);
    index = 0;
    if ((sint32)count > 0)
    {
        do
        {
            for (component = 0; component < 15; ++component)
            {
                value = r_u32(source);
                source += 4;
                w_u32(destination, value);
                destination += 4;
            }
            count = r_u32(mesh + 20);
            ++index;
        } while (index < (sint32)count);
    }
    allocation = r_u32(mesh + 16);
    count = r_u32(mesh + 24);
    w_u32(mesh + 32, allocation);
    count *= 60;
    allocation = r_u32(mesh + 16);
    w_u32(mesh + 36, allocation + count);
    heap_free(file);
    mesh_rebuild_normals(mesh);
    return mesh;
}

void object_draw_mesh(uint32 object)
{
    FUNCTION_MARKER(0x8001690C, "SLUS_005.43");
    mesh_render_fixed(r_u32(object + 100));
}

sint32 vector_normalize(VECTOR *source, VECTOR *destination)
{
    sint32 *input[3] = {&source->vx, &source->vy, &source->vz};
    sint32 *output[3] = {&destination->vx, &destination->vy, &destination->vz};
    uint32 squared;
    uint32 value;
    uint32 component;
    sint32 length;
    sint32 numerator;
    sint32 result;
    FUNCTION_MARKER(0x800233A4, "SLUS_005.43");
    squared = 0;
    for (component = 0; component < 3; ++component)
    {
        value = (uint32)*input[component];
        squared += value * value;
    }
    length = (sint32)SquareRoot0((sint32)squared);
    if (!length)
        length = 1;
    for (component = 0; component < 3; ++component)
    {
        numerator = (sint32)((uint32)*input[component] << 12);
        if (length == -1 && numerator == (sint32)0x80000000)
            trap(0x5D);
        result = (sint32)((sint64)numerator / length);
        *output[component] = result;
    }
    return result;
}

sint32 mesh_rebuild_normals(uint32 mesh)
{
    VECTOR first = {0};
    VECTOR second = {0};
    sint32 *components[6] = {&first.vx, &first.vy, &first.vz, &second.vx, &second.vy, &second.vz};
    uint32 face;
    uint32 offset;
    uint32 side;
    uint32 axis;
    uint32 a;
    uint32 b;
    uint32 vertices;
    uint32 destination;
    sint32 left;
    sint32 right;
    sint32 index;
    sint32 count;
    FUNCTION_MARKER(0x80016930, "SLUS_005.43");
    count = r_s32(mesh + 20);
    if (count <= 0)
        return count;
    index = 0;
    offset = 0;
    do
    {
        face = r_u32(mesh + 16) + offset;
        a = r_u32(face + 4);
        w_u32(face + 20, (uint32)index);
        for (side = 0; side < 2; ++side)
        {
            for (axis = 0; axis < 3; ++axis)
            {
                if (side || axis)
                    a = r_u32(face + 4);
                vertices = r_u32(mesh);
                b = r_u32(face + (side ? 12 : 8));
                left = r_s16(vertices + (a << 3) + axis * 2);
                right = r_s16(vertices + (b << 3) + axis * 2);
                *components[side * 3 + axis] = left - right;
            }
        }
        vector_normalize(&first, &first);
        vector_normalize(&second, &second);
        OuterProduct12(&first, &second, &first);
        vector_normalize(&first, &first);
        a = r_u32(face + 20) << 3;
        vertices = r_u32(mesh + 8);
        destination = a + vertices;
        w_u16(destination, 0 - (uint32)first.vx);
        a = r_u32(face + 20) << 3;
        vertices = r_u32(mesh + 8);
        destination = a + vertices;
        w_u16(destination + 2, 0 - (uint32)first.vy);
        a = r_u32(face + 20) << 3;
        vertices = r_u32(mesh + 8);
        destination = a + vertices;
        w_u16(destination + 4, 0 - (uint32)first.vz);
        count = r_s32(mesh + 20);
        ++index;
        offset += 60;
    } while (index < count);
    return 0;
}

void texture_load_page(uint32 page, const char *name)
{
    uint32 buffer;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x80016B50, "SLUS_005.43");
    buffer = heap_alloc_back(0x20000);
    resource_load_into(name, buffer);
    rectangle.w = 64;
    rectangle.x = (sint16)((page & 15) << 6);
    rectangle.y = (sint16)(((page & 31) >= 16) << 8);
    rectangle.h = 256;
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(buffer, 64 * 256 * 2));
    DrawSync(0);
    heap_free(buffer);
}

void mesh_render_fixed(uint32 mesh)
{
    sint32 count;
    sint32 groups;
    size_t iterations;
    uint32 vertices;
    FUNCTION_MARKER(0x80016468, "SLUS_005.43");
    count = r_s32(mesh + 4);
    w_u32(0x80046088, 0x80033D50);
    w_u32(0x80041F4C, 0x8003FE80);
    w_u32(0x80042908, 0x1F800000);
    groups = count / 3;
    if (groups * 3 < count)
        ++groups;
    vertices = r_u32(mesh);
    iterations = groups > 0 ? (size_t)groups : 1;
    RotTransPers3N((const SVECTOR *)psx_addr(vertices, iterations * 24), (sint32 *)psx_addr(0x80033D50, iterations * 12), (uint16 *)psx_addr(0x8003FE80, iterations * 6), (uint16 *)psx_addr(0x80059660, iterations * 2), groups);
    render_emit_mesh_lit(mesh);
}

void mesh_render_fixed_alternate(uint32 mesh)
{
    sint32 count;
    sint32 groups;
    size_t iterations;
    uint32 vertices;
    FUNCTION_MARKER(0x8001650C, "SLUS_005.43");
    count = r_s32(mesh + 4);
    w_u32(0x80046088, 0x80033D50);
    w_u32(0x80041F4C, 0x8003FE80);
    w_u32(0x80042908, 0x1F800000);
    groups = count / 3;
    if (groups * 3 < count)
        ++groups;
    vertices = r_u32(mesh);
    iterations = groups > 0 ? (size_t)groups : 1;
    RotTransPers3N((const SVECTOR *)psx_addr(vertices, iterations * 24), (sint32 *)psx_addr(0x80033D50, iterations * 12), (uint16 *)psx_addr(0x8003FE80, iterations * 6), (uint16 *)psx_addr(0x80059660, iterations * 2), groups);
    render_emit_mesh_overlay(mesh);
}

uint32 mesh_render_adaptive(uint32 mesh, uint32 mode)
{
    sint32 count;
    sint32 groups;
    size_t iterations;
    uint32 vertices;
    uint32 screen;
    uint32 depth;
    FUNCTION_MARKER(0x800165B0, "SLUS_005.43");
    count = r_s32(mesh + 4);
    if (count >= 254)
    {
        w_u32(0x80046088, 0x80033D50);
        w_u32(0x80041F4C, 0x1F800000);
    }
    else if (count >= 169)
    {
        w_u32(0x80046088, 0x1F800000);
        w_u32(0x80041F4C, 0x8003FE80);
    }
    else
    {
        w_u32(0x80046088, 0x1F800000);
        w_u32(0x80041F4C, 0x1F8002A8);
    }
    count = r_s32(mesh + 4);
    groups = count / 3;
    if (groups * 3 < count)
        ++groups;
    vertices = r_u32(mesh);
    screen = r_u32(0x80046088);
    depth = r_u32(0x80041F4C);
    iterations = groups > 0 ? (size_t)groups : 1;
    RotTransPers3N((const SVECTOR *)psx_addr(vertices, iterations * 24), (sint32 *)psx_addr(screen, iterations * 12), (uint16 *)psx_addr(depth, iterations * 6), (uint16 *)psx_addr(0x80059660, iterations * 2), groups);
    w_u32(0x80042908, 0x80052878);
    return render_emit_mesh_wire(mesh, mode);
}

uint32 mesh_render_adaptive_alternate(uint32 mesh, uint32 mode)
{
    sint32 count;
    sint32 groups;
    size_t iterations;
    uint32 vertices;
    uint32 screen;
    uint32 depth;
    FUNCTION_MARKER(0x800166D4, "SLUS_005.43");
    count = r_s32(mesh + 4);
    if (count >= 254)
    {
        w_u32(0x80046088, 0x80033D50);
        w_u32(0x80041F4C, 0x1F800000);
    }
    else if (count >= 169)
    {
        w_u32(0x80046088, 0x1F800000);
        w_u32(0x80041F4C, 0x8003FE80);
    }
    else
    {
        w_u32(0x80046088, 0x1F800000);
        w_u32(0x80041F4C, 0x1F8002A8);
    }
    count = r_s32(mesh + 4);
    groups = count / 3;
    if (groups * 3 < count)
        ++groups;
    vertices = r_u32(mesh);
    screen = r_u32(0x80046088);
    depth = r_u32(0x80041F4C);
    iterations = groups > 0 ? (size_t)groups : 1;
    RotTransPers3N((const SVECTOR *)psx_addr(vertices, iterations * 24), (sint32 *)psx_addr(screen, iterations * 12), (uint16 *)psx_addr(depth, iterations * 6), (uint16 *)psx_addr(0x80059660, iterations * 2), groups);
    w_u32(0x80042908, 0x80052878);
    return render_emit_mesh_wire_front(mesh, mode);
}

void mesh_render_adaptive_single(uint32 mesh)
{
    sint32 count;
    sint32 groups;
    size_t iterations;
    uint32 vertices;
    uint32 screen;
    uint32 depth;
    FUNCTION_MARKER(0x800167F8, "SLUS_005.43");
    count = r_s32(mesh + 4);
    if (count >= 254)
    {
        w_u32(0x80046088, 0x80033D50);
        w_u32(0x80041F4C, 0x1F800000);
    }
    else if (count >= 169)
    {
        w_u32(0x80046088, 0x1F800000);
        w_u32(0x80041F4C, 0x8003FE80);
    }
    else
    {
        w_u32(0x80046088, 0x1F800000);
        w_u32(0x80041F4C, 0x1F8002A8);
    }
    count = r_s32(mesh + 4);
    groups = count / 3;
    if (groups * 3 < count)
        ++groups;
    vertices = r_u32(mesh);
    screen = r_u32(0x80046088);
    depth = r_u32(0x80041F4C);
    iterations = groups > 0 ? (size_t)groups : 1;
    RotTransPers3N((const SVECTOR *)psx_addr(vertices, iterations * 24), (sint32 *)psx_addr(screen, iterations * 12), (uint16 *)psx_addr(depth, iterations * 6), (uint16 *)psx_addr(0x80059660, iterations * 2), groups);
    w_u32(0x80042908, 0x80052878);
    render_emit_mesh_depth(mesh);
}

sint32 vector_length_approx(sint32 x, sint32 y, sint32 z)
{
    uint32 ax;
    uint32 ay;
    uint32 az;
    uint32 larger;
    uint32 smaller;
    uint32 value;
    FUNCTION_MARKER(0x8002E310, "SLUS_005.43");
    ax = x < 0 ? 0 - (uint32)x : (uint32)x;
    ay = y < 0 ? 0 - (uint32)y : (uint32)y;
    az = z < 0 ? 0 - (uint32)z : (uint32)z;
    if ((sint32)ax < (sint32)az)
    {
        larger = az;
        smaller = ax;
    }
    else
    {
        larger = ax;
        smaller = az;
    }
    value = (larger << 4) + (smaller << 2) + smaller;
    ax = (uint32)((sint32)value >> 4);
    if ((sint32)ax < (sint32)ay)
    {
        larger = ay;
        smaller = ax;
    }
    else
    {
        larger = ax;
        smaller = ay;
    }
    value = (larger << 4) + (smaller << 2) + smaller;
    return (sint32)value >> 4;
}

sint32 triangle_max_edge(uint32 first, uint32 second, uint32 third)
{
    sint32 x0, x1, y0, y1, z0, z1;
    sint32 a, b, c;
    FUNCTION_MARKER(0x80016248, "SLUS_005.43");
    x0 = r_s16(first);
    x1 = r_s16(second);
    y0 = r_s16(first + 2);
    y1 = r_s16(second + 2);
    z1 = r_s16(second + 4);
    z0 = r_s16(first + 4);
    a = vector_length_approx(x0 - x1, y0 - y1, z0 - z1);
    x0 = r_s16(second);
    x1 = r_s16(third);
    y1 = r_s16(third + 2);
    z0 = r_s16(second + 4);
    z1 = r_s16(third + 4);
    y0 = r_s16(second + 2);
    b = vector_length_approx(x0 - x1, y0 - y1, z0 - z1);
    x0 = r_s16(third);
    x1 = r_s16(first);
    y0 = r_s16(third + 2);
    y1 = r_s16(first + 2);
    z1 = r_s16(first + 4);
    z0 = r_s16(third + 4);
    c = vector_length_approx(x0 - x1, y0 - y1, z0 - z1);
    if (a < b)
        a = b;
    if (a < c)
        a = c;
    return a;
}

sint32 quad_max_edge(uint32 first, uint32 second, uint32 third, uint32 fourth)
{
    sint32 x0, x1, y0, y1, z0, z1;
    sint32 a, b, c, d;
    FUNCTION_MARKER(0x80016334, "SLUS_005.43");
    x0 = r_s16(first);
    x1 = r_s16(second);
    y0 = r_s16(first + 2);
    y1 = r_s16(second + 2);
    z1 = r_s16(second + 4);
    z0 = r_s16(first + 4);
    a = vector_length_approx(x0 - x1, y0 - y1, z0 - z1);
    x0 = r_s16(second);
    x1 = r_s16(third);
    y1 = r_s16(third + 2);
    z0 = r_s16(second + 4);
    z1 = r_s16(third + 4);
    y0 = r_s16(second + 2);
    b = vector_length_approx(x0 - x1, y0 - y1, z0 - z1);
    x0 = r_s16(third);
    x1 = r_s16(fourth);
    y0 = r_s16(third + 2);
    y1 = r_s16(fourth + 2);
    z1 = r_s16(fourth + 4);
    z0 = r_s16(third + 4);
    c = vector_length_approx(x0 - x1, y0 - y1, z0 - z1);
    x0 = r_s16(fourth);
    x1 = r_s16(first);
    y0 = r_s16(fourth + 2);
    y1 = r_s16(first + 2);
    z1 = r_s16(first + 4);
    z0 = r_s16(fourth + 4);
    d = vector_length_approx(x0 - x1, y0 - y1, z0 - z1);
    if (a < b)
        a = b;
    if (a < c)
        a = c;
    if (a < d)
        a = d;
    return a;
}

sint32 message_add(char *text, uint32 style, uint32 duration)
{
    uint32 length;
    uint32 consumed;
    uint32 next;
    uint32 split;
    uint32 product;
    uint32 offset;
    uint32 divisor;
    uint32 frames;
    uint32 color;
    sint32 enabled;
    sint32 previous;
    sint32 numerator;
    sint32 quotient;
    sint32 index;
    uint8 saved;
    char *chunk;
    FUNCTION_MARKER(0x8001507C, "SLUS_005.43");
    enabled = r_s16(0x800118F8);
    if (!enabled)
        return enabled;
    length = (uint32)strlen(text);
    if ((sint16)length >= 76)
    {
        consumed = 0;
        previous = 0;
        for (;;)
        {
            next = consumed + 75;
            consumed = next;
            split = 75;
            if ((sint16)length < (sint16)next)
            {
                split = 75 - (next - length);
                consumed = length;
            }
            product = duration * ((uint32)(sint32)(sint16)consumed - (uint32)previous);
            quotient = (sint32)((sint64)(sint32)product / (sint16)length);
            if ((sint16)consumed >= (sint16)length)
                return message_add(text, style, (uint32)quotient);
            if ((uint8)text[(sint16)split] != 32)
            {
                do
                    --split;
                while ((uint8)text[(sint16)split] != 32);
            }
            chunk = text;
            saved = (uint8)text[(sint16)split];
            text += (sint16)split;
            *text = 0;
            message_add(chunk, style, (uint32)quotient);
            *text = (char)saved;
            previous = (sint16)consumed;
        }
    }
    offset = (uint32)r_s16(0x80031124) * 288;
    sprintf((char *)psx_addr(0x80042440 + offset, (size_t)length + 1), "%s", text);
    offset = (uint32)r_s16(0x80031124) * 288;
    w_u16(0x80042552 + offset, length);
    if (style == 1)
        color = 0x80FF;
    else if (style == 2)
        color = 0x20E020;
    else
    {
        color = 0xFF8020;
        offset = (uint32)r_s16(0x80031124) * 288;
    }
    w_u32(0x8004254C + offset, color);
    offset = (uint32)r_s16(0x80031124) * 288;
    w_u16(0x80042550 + offset, 0);
    w_u16(0x80042554 + offset, 0);
    w_u32(0x8004255C + offset, 0);
    if (duration)
    {
        frames = r_u32(0x800311C0) ? 25 : 30;
        numerator = (sint32)((uint32)r_s16(0x80042552 + offset) << 8);
        divisor = frames * duration;
        if (!divisor)
        {
            trap(7);
            quotient = numerator < 0 ? 1 : -1;
        }
        else
            quotient = (sint32)((sint64)numerator / (sint32)divisor);
        offset = (uint32)r_s16(0x80031124) * 288;
        w_u32(0x80042558 + offset, (uint32)quotient);
    }
    else
        w_u32(0x80042558 + offset, 512);
    index = r_s16(0x80031128);
    if (index < 4)
        w_u16(0x80031128, index + 1);
    else
    {
        index = r_s16(0x80031126) + 1;
        w_u16(0x80031126, index % 4);
    }
    index = (r_s16(0x80031124) + 1) % 4;
    w_u16(0x80031124, index);
    return index;
}

uint32 mesh_import_primitives(uint32 source)
{
    static const uint32 texture_offsets[6] = {6, 10, 4, 8, 12, 16};
    uint32 primitives;
    uint32 vertices;
    uint32 normals;
    uint32 vertex_count;
    uint32 normal_count;
    uint32 primitive_count;
    uint32 mesh;
    uint32 allocation;
    uint32 destination;
    uint32 data;
    uint32 opcode;
    uint32 triangles;
    uint32 quads;
    uint32 written;
    uint32 offset;
    uint32 pass;
    uint32 corners;
    uint32 component;
    uint32 value;
    uint32 index_offset;
    uint32 index_step;
    sint32 index;
    FUNCTION_MARKER(0x80015BF4, "SLUS_005.43");
    primitives = r_u32(source + 16);
    vertices = r_u32(source);
    normals = r_u32(source + 8);
    vertex_count = r_u32(source + 4);
    primitive_count = r_u32(source + 20);
    normal_count = r_u32(source + 12);
    mesh = heap_alloc_back(52);
    w_u16(mesh + 40, 0);
    w_u16(mesh + 42, 0);
    w_u16(mesh + 44, 0);
    w_u32(mesh + 4, vertex_count);
    allocation = heap_alloc_back(vertex_count << 3);
    w_u32(mesh, allocation);
    allocation = heap_alloc_back(primitive_count * 60);
    w_u32(mesh + 16, allocation);
    allocation = heap_alloc_back(normal_count << 3);
    destination = r_u32(mesh + 16);
    w_u32(mesh + 8, allocation);
    w_u32(mesh + 12, 0);
    w_u32(mesh + 32, destination);
    for (index = 0; index < (sint32)vertex_count; ++index)
    {
        for (component = 0; component < 3; ++component)
        {
            destination = r_u32(mesh) + (uint32)index * 8;
            value = r_u16(vertices + (uint32)index * 8 + component * 2);
            w_u16(destination + component * 2, value);
        }
    }
    for (index = 0; index < (sint32)normal_count; ++index)
    {
        for (component = 0; component < 3; ++component)
        {
            destination = r_u32(mesh + 8) + (uint32)index * 8;
            value = r_u16(normals + (uint32)index * 8 + component * 2);
            w_u16(destination + component * 2, value);
        }
    }
    triangles = 0;
    quads = 0;
    data = primitives;
    for (index = 0; index < (sint32)primitive_count; ++index)
    {
        opcode = r_u8(data + 3) & 0xFC;
        if (opcode == 0x24 || opcode == 0x34)
            ++triangles;
        else if (opcode == 0x2C || opcode == 0x3C)
            ++quads;
        data += ((uint32)r_u8(data + 1) << 2) + 4;
    }
    w_u32(mesh + 20, triangles + quads);
    w_u32(mesh + 24, triangles);
    w_u32(mesh + 28, quads);
    offset = 0;
    written = 0;
    for (pass = 0; pass < 2; ++pass)
    {
        if (pass)
        {
            offset = written * 60;
            w_u32(mesh + 36, r_u32(mesh + 16) + offset);
        }
        corners = pass ? 4 : 3;
        data = primitives;
        for (index = 0; index < (sint32)primitive_count; ++index)
        {
            opcode = r_u8(data + 3) & 0xFC;
            if ((!pass && (opcode == 0x24 || opcode == 0x34)) || (pass && (opcode == 0x2C || opcode == 0x3C)))
            {
                destination = r_u32(mesh + 16) + offset;
                w_u32(destination, corners);
                for (component = 0; component < corners + 2; ++component)
                {
                    destination = r_u32(mesh + 16) + offset;
                    value = r_u16(data + texture_offsets[component]);
                    w_u16(destination + 24 + component * 2, value);
                }
                index_offset = pass ? 22 : 18;
                index_step = (opcode == 0x34 || opcode == 0x3C) ? 4 : 2;
                for (component = 0; component < corners; ++component)
                {
                    destination = r_u32(mesh + 16) + offset;
                    value = (uint32)r_s16(data + index_offset + component * index_step);
                    w_u32(destination + 4 + component * 4, value);
                }
                if (!pass)
                {
                    destination = r_u32(mesh + 16) + offset;
                    w_u32(destination + 16, 0xFFFFFFFF);
                }
                destination = r_u32(mesh + 16) + offset;
                value = (uint32)r_s16(data + (pass ? 20 : 16));
                w_u32(destination + 20, value);
                destination = r_u32(mesh + 16) + offset;
                w_u32(destination + 36, 0x808080);
                offset += 60;
                if (!pass)
                    ++written;
            }
            data += ((uint32)r_u8(data + 1) << 2) + 4;
        }
    }
    mesh_build_adjacency(mesh);
    return mesh;
}

sint32 mesh_build_adjacency(uint32 mesh)
{
    static const uint32 triangle_edges[3] = {4, 8, 12};
    static const uint32 quad_edges[4] = {4, 8, 16, 12};
    const uint32 *edges;
    uint32 offset;
    uint32 face;
    uint32 faces;
    uint32 vertices;
    uint32 first;
    uint32 second;
    uint32 edge;
    uint32 axis;
    uint32 corners;
    uint32 a;
    uint32 b;
    uint32 c;
    uint32 d;
    sint32 count;
    sint32 index;
    sint32 neighbor;
    sint32 sum;
    FUNCTION_MARKER(0x80015888, "SLUS_005.43");
    count = r_s32(mesh + 20);
    if (count <= 0)
        return count;
    offset = 0;
    index = 0;
    do
    {
        face = r_u32(mesh + 16) + offset;
        if (r_u32(face) == 3)
        {
            corners = 3;
            edges = triangle_edges;
        }
        else
        {
            corners = 4;
            edges = quad_edges;
        }
        for (edge = 0; edge < corners; ++edge)
        {
            faces = r_u32(mesh + 16);
            count = r_s32(mesh + 20);
            first = r_u32(face + edges[edge]);
            second = r_u32(face + edges[(edge + 1) % corners]);
            neighbor = mesh_find_face_pair(faces, count, first, second, index);
            w_u16(face + 40 + edge * 2, neighbor);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            a = r_u32(face + 4) << 3;
            vertices = r_u32(mesh);
            b = r_u32(face + 8) << 3;
            c = r_u32(face + 12) << 3;
            sum = r_s16(vertices + a + axis * 2);
            sum += r_s16(vertices + b + axis * 2);
            sum += r_s16(vertices + c + axis * 2);
            if (corners == 3)
                sum /= 3;
            else
            {
                d = r_u32(face + 16) << 3;
                sum += r_s16(vertices + d + axis * 2);
                sum >>= 2;
            }
            w_u16(face + 48 + axis * 2, sum);
        }
        count = r_s32(mesh + 20);
        ++index;
        offset += 60;
    } while (index < count);
    return 0;
}

sint32 palette_register(uint32 code)
{
    uint32 offset;
    uint32 counter;
    FUNCTION_MARKER(0x80015660, "SLUS_005.43");
    for (offset = 0; offset < 32 * 0x34; offset += 0x34)
    {
        if (r_u16(0x800521A8 + offset) == (code & 0xFFFF))
            return (sint32)(code & 0xFFFF);
    }
    counter = r_u32(0x8003114C);
    offset = counter * 0x34;
    counter += 1;
    w_u32(0x8003114C, counter);
    w_u16(0x800521A8 + offset, code);
    if ((sint32)counter >= 32)
#ifndef NDEBUG
        return printf("Error:  this model has too many cluts\n");
#else
        return 38;
#endif
    return (sint32)offset;
}

uint32 palette_find_remap(uint32 code)
{
    uint32 index;
    FUNCTION_MARKER(0x80015704, "SLUS_005.43");
    code &= 0xFFFF;
    for (index = 0; index < 32; ++index)
    {
        if (r_u16(0x800521A8 + index * 0x34) == code)
            return ((0x3800 + index * 0x40) | 0x1D) & 0xFFFF;
    }
#ifndef NDEBUG
    printf("clut not found %d\n", (sint32)code);
#endif
    return 0;
}

sint32 mesh_find_face_pair(uint32 faces, sint32 count, uint32 first, uint32 second, sint32 skip)
{
    sint32 index;
    uint32 value;
    FUNCTION_MARKER(0x800157D0, "SLUS_005.43");
    for (index = 0; index < count; ++index, faces += 60)
    {
        value = r_u32(faces + 4);
        if (value != first)
        {
            if (r_u32(faces + 8) != first && r_u32(faces + 12) != first && r_u32(faces + 16) != first)
                continue;
            value = r_u32(faces + 4);
        }
        if (value == second || r_u32(faces + 8) == second || r_u32(faces + 12) == second || r_u32(faces + 16) == second)
        {
            if (index != skip)
                return index;
        }
    }
    return -1;
}

sint32 palette_save_all(void)
{
    uint32 offset;
    uint32 code;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x8001558C, "SLUS_005.43");
    for (offset = 0; offset < 32 * 0x34; offset += 0x34)
    {
        code = r_u16(0x800521A8 + offset);
        rectangle.w = 16;
        rectangle.h = 1;
        rectangle.x = (sint16)((code & 0x3F) << 4);
        rectangle.y = (sint16)(code >> 6);
        StoreImage(&rectangle, (uint32 *)psx_addr(0x800521AC + offset, 32));
        DrawSync(0);
    }
    return 0;
}

void scene_suspend_loading(void)
{
    FUNCTION_MARKER(0x80014EC0, "SLUS_005.43");
    if (!r_u32(0x800310A4))
        return;
    VSyncCallbackPSX(0);
    sound_set_voice_volume(r_u32(0x80052180), 0);
    sound_set_voice_volume(r_u32(0x800510F0), 0);
    sound_set_voice_volume(r_u32(0x800428C0), 0);
    sound_tick();
    sound_tick();
    sound_tick();
    sound_tick();
}

uint32 scene_resume_loading(void)
{
    FUNCTION_MARKER(0x80014F64, "SLUS_005.43");
    if (!r_u32(0x800310A4))
        return 0;
    return VSyncCallbackPSX(r_u32(0x80046170));
}

sint32 message_init(void)
{
    uint32 offset;
    FUNCTION_MARKER(0x80014FB0, "SLUS_005.43");
    w_u16(0x80031124, 0);
    w_u16(0x80031126, 0);
    w_u16(0x80031128, 0);
    for (offset = 0; offset < 4 * 0x120; offset += 0x120)
    {
        sprintf((char *)psx_addr(0x80042440 + offset, 0x10C), " ");
        w_u32(0x8004254C + offset, 0xFF8020);
        w_u16(0x80042554 + offset, 0);
        w_u16(0x80042550 + offset, 0);
        w_u16(0x80042552 + offset, 0);
    }
    return 0;
}

void scene_present_fade(void)
{
    uint32 owner;
    uint32 packet;
    uint32 tile;
    uint32 ot;
    uint16 page;
    FUNCTION_MARKER(0x80012BB4, "SLUS_005.43");
    owner = r_u32(0x8004608C);
    PutDrawEnv((DRAWENV *)psx_addr(owner, sizeof(DRAWENV)));
    owner = r_u32(0x8004608C);
    PutDispEnv((DISPENV *)psx_addr(owner + 92, sizeof(DISPENV)));
    packet = r_u32(0x80040690);
    page = GetTPage(0, 3, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(packet, sizeof(DR_MODE)), 1, 1, page, 0);
    owner = r_u32(0x8004608C);
    AddPrim(psx_addr(owner + 0x45A0, 4), psx_addr(packet, 4));
    packet += 12;
    w_u32(0x80040690, packet);
    tile = r_u32(0x80040690);
    w_u32(0x80040690, r_u32(0x80040690) + 16);
    SetTile((TILE *)psx_addr(tile, sizeof(TILE)));
    w_u8(tile + 4, 32);
    w_u8(tile + 5, 32);
    w_u8(tile + 6, 32);
    w_u16(tile + 8, 0);
    w_u16(tile + 10, 0);
    w_u16(tile + 12, 320);
    w_u16(tile + 14, 256);
    w_u8(tile + 7, r_u8(tile + 7) | 2);
    owner = r_u32(0x8004608C);
    AddPrim(psx_addr(owner + 0x45A4, 4), psx_addr(tile, 4));
    owner = r_u32(0x80042170);
    w_u8(owner + 24, 0);
    owner = r_u32(0x80042170);
    w_u8(owner + 0x45F4 + 24, 0);
    packet = r_u32(0x80040690);
    page = GetTPage(0, 2, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(packet, sizeof(DR_MODE)), 1, 1, page, 0);
    owner = r_u32(0x8004608C);
    AddPrim(psx_addr(owner + 0x45A8, 4), psx_addr(packet, 4));
    packet += 12;
    w_u32(0x80040690, packet);
    ot = (r_u32(0x80034D78) << 2) + 112;
    owner = r_u32(0x8004608C);
    DrawOTag((uint32 *)psx_addr(ot + owner, 4));
}

sint32 scene_stop_loading(void)
{
    FUNCTION_MARKER(0x80012E44, "SLUS_005.43");
    if (!r_u32(0x800310A4))
        return 0;
    w_u32(0x800310A8, 1);
    while (r_s32(0x800310B0) < 3000)
    {
        VSync(0);
    }
    SetDispMask(0);
    sound_stop_loops();
    VSyncCallbackPSX(0);
    VSync(0);
    DrawSync(0);
    w_u32(0x800310A4, 0);
    scene_free_resources();
#ifndef NDEBUG
    return printf("LOAD_CALLBACK : stopped\n");
#else
    return 24;
#endif
}

void scene_free_resources(void)
{
    FUNCTION_MARKER(0x80012F0C, "SLUS_005.43");
    resource_free_bundle(r_u32(0x80059658));
    if (r_u32(0x80046170) == 0x800145FC)
        resource_free_bundle(r_u32(0x80040FB8));
}

sint32 sound_stop_voice(uint32 voice)
{
    uint32 offset;
    uint32 channel_offset;
    uint32 mask;
    sint32 channel;
    FUNCTION_MARKER(0x8002589C, "SLUS_005.43");
    if (!r_u8(0x80031258) || voice == 0xFFFFFFFF)
        return 0;
    if (voice >= 24)
        return (sint32)(voice << 3);
    offset = voice * 28;
    if (!(r_u32(0x80033994 + offset) & 2))
        return 0;
    mask = 0;
    channel = r_s8(0x800339A4 + offset);
    channel_offset = (uint32)channel * 28;
    if (r_u32(0x80033704 + channel_offset) == voice)
    {
        mask = (uint32)1 << ((uint32)channel & 31);
        w_u32(0x800336F0 + channel_offset, 0);
    }
    channel = r_s8(0x800339A5 + offset);
    if (channel >= 0)
    {
        channel_offset = (uint32)channel * 28;
        if (r_u32(0x80033704 + channel_offset) == voice)
        {
            mask |= (uint32)1 << ((uint32)channel & 31);
            w_u32(0x800336F0 + channel_offset, 0);
        }
    }
    SpuSetKey(0, mask);
    w_u32(0x80033994 + offset, 0);
    return (sint32)offset;
}

void sound_set_voice_pitch(uint32 voice, uint32 pitch)
{
    uint32 offset;
    uint32 channel_offset;
    uint32 base_pitch;
    uint32 product;
    sint32 channel;
    SpuVoiceAttr attr = {0};
    FUNCTION_MARKER(0x80025B6C, "SLUS_005.43");
    if (!r_u8(0x80031258) || voice == 0xFFFFFFFF || voice >= 24)
        return;
    offset = voice * 28;
    if (!(r_u32(0x80033994 + offset) & 2))
        return;
    channel = r_s8(0x800339A4 + offset);
    w_u32(0x8003399C + offset, pitch);
    channel_offset = (uint32)channel * 28;
    product = r_u32(0x80033704 + channel_offset);
    base_pitch = r_u32(0x80033990 + offset);
    if (product == voice)
    {
        product = pitch * base_pitch;
        attr.voice = (uint32)1 << ((uint32)channel & 31);
        attr.mask = 0x10;
        if ((sint32)product < 0)
            product += 255;
        attr.pitch = (uint16)((sint32)product >> 8);
        w_u32(0x80033700 + channel_offset, attr.pitch);
        SpuSetVoiceAttr(&attr);
    }
    channel = r_s8(0x800339A5 + offset);
    if (!channel)
        return;
    channel_offset = (uint32)channel * 28;
    if (r_u32(0x80033704 + channel_offset) != voice)
        return;
    product = pitch * base_pitch;
    attr.voice = (uint32)1 << ((uint32)channel & 31);
    attr.mask = 0x10;
    if ((sint32)product < 0)
        product += 255;
    attr.pitch = (uint16)((sint32)product >> 8);
    w_u32(0x80033700 + channel_offset, attr.pitch);
    SpuSetVoiceAttr(&attr);
}

void scene_stars_update(void)
{
    uint32 fade;
    uint32 product;
    uint32 phase;
    uint32 value;
    uint32 index;
    uint32 band;
    uint32 segment;
    uint32 stars;
    uint32 camera;
    uint32 address;
    uint32 packet;
    uint32 color;
    uint32 depth;
    sint32 projected_depth;
    sint32 projection;
    sint32 flags;
    SVECTOR point = {0};
    FUNCTION_MARKER(0x80013A48, "SLUS_005.43");
    if (r_s32(0x800310B0) >= 4 && r_s32(0x800310B0) < 6)
        SetDispMask(1);
    w_u32(0x80031100, r_s16(0x800118FC) >> 2);
    w_u32(0x80031104, r_s16(0x800118FC) >> 2);
    w_u32(0x800310FC, 0);
    w_u32(0x8003110C, 64);
    w_u32(0x80031108, 64);
    fade = 255 - r_u32(0x800310A8);
    product = r_u32(0x80031100) * fade;
    sound_set_voice_volume(r_u32(0x800510F0), (product >> 8) & 255);
    product = r_u32(0x80031104) * fade;
    sound_set_voice_volume(r_u32(0x800428C0), (product >> 8) & 255);
    product = r_u32(0x800310FC) * fade;
    sound_set_voice_volume(r_u32(0x80052180), (product >> 8) & 255);
    sound_set_voice_pitch(r_u32(0x800510F0), r_u32(0x80031108));
    sound_set_voice_pitch(r_u32(0x800428C0), r_u32(0x8003110C));
    sound_tick();
    w_u16(0x800310D0, 0);
    w_u16(0x800310D2, 0);
    w_u16(0x800310D4, 0);
    RotMatrixYXZ((SVECTOR *)psx_addr(0x800310D0, sizeof(SVECTOR)), (MATRIX *)psx_addr(0x80034EFC, sizeof(MATRIX)));
    phase = r_u32(0x800310B8) << 2;
    value = (uint32)rsin((sint32)phase >> 2) << 6;
    w_u32(0x80034DE8, (sint32)value >> 12);
    value = (uint32)rcos((sint32)phase >> 1) << 6;
    w_u32(0x80034DE8, r_u32(0x80034DE8) + (uint32)((sint32)value >> 12));
    value = (uint32)rcos((sint32)phase >> 2) << 6;
    w_u32(0x80034DEC, (sint32)value >> 12);
    value = (uint32)rsin((sint32)phase) << 6;
    w_u32(0x80034DEC, r_u32(0x80034DEC) - (uint32)((sint32)value >> 12));
    value = (uint32)rsin((sint32)phase >> 1) << 6;
    w_u32(0x80034DF0, (sint32)value >> 12);
    value = (uint32)rcos((sint32)phase) << 6;
    w_u32(0x80034DF0, r_u32(0x80034DF0) - (uint32)((sint32)value >> 12));
    value = (uint32)rsin(r_s32(0x800310B8)) * 5000;
    w_u32(0x800310C0, (sint32)value >> 12);
    value = (uint32)rcos(r_s32(0x800310B8)) * 6500;
    w_u32(0x800310C8, (sint32)value >> 12);
    w_u32(0x800310C4, -3000);
    w_u32(0x800310B8, r_u32(0x800310B8) + 15);
    camera_set_position(0x800310C0);
    camera_set_up(0, (uint32)-4096, 0);
    r_u32(0x80040C94);
    camera_update_modes();
    object_draw_transformed(0x80034DE0);
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    for (index = 0; index < 135; ++index)
    {
        band = index >> 6;
        depth = 0;
        for (segment = 0; segment < 2; ++segment)
        {
            stars = r_u32(0x80059E60) + index * 8;
            camera = r_u32(0x80040C94);
            value = (uint32)(r_s32(camera + 8) >> 2);
            point.vx = (sint16)(r_u16(stars) - value);
            stars = r_u32(0x80059E60) + index * 8;
            camera = r_u32(0x80040C94);
            value = (uint32)(r_s32(camera + 12) >> 2);
            point.vy = (sint16)(r_u16(stars + 2) - value);
            stars = r_u32(0x80059E60) + index * 8;
            if (!segment)
            {
                camera = r_u32(0x80040C94);
                value = (uint32)(r_s32(camera + 16) >> 2);
                point.vz = (sint16)(r_u16(stars + 4) - value);
            }
            else
            {
                value = r_u16(stars + 4) + (1024 >> band);
                camera = r_u32(0x80040C94);
                point.vz = (sint16)(value - (uint32)(r_s32(camera + 16) >> 2));
            }
            projected_depth = RotTransPers(&point, (sint32 *)psx_addr(0x80033D50 + segment * 4, 4), &projection, &flags);
            depth = (uint32)projected_depth;
            if (segment)
                depth <<= band;
            if ((sint32)depth <= 0)
                break;
        }
        if ((sint32)depth > 0)
        {
            color = depth >> 9;
            if (color >= 256)
                color = 255;
            color = 255 - color;
            color = (color << 8) + color + (color << 16);
            packet = r_u32(0x80040690);
            w_u32(packet + 4, color);
            packet = r_u32(0x80040690);
            w_u32(packet + 12, 0);
            packet = r_u32(0x80040690);
            w_u16(packet + 8, r_u16(0x80033D50));
            packet = r_u32(0x80040690);
            w_u16(packet + 10, r_u16(0x80033D52));
            packet = r_u32(0x80040690);
            w_u16(packet + 16, r_u16(0x80033D54));
            packet = r_u32(0x80040690);
            w_u16(packet + 18, r_u16(0x80033D56));
            packet = r_u32(0x80040690);
            setLineG2((LINE_G2 *)psx_addr(packet, sizeof(LINE_G2)));
            packet = r_u32(0x80040690);
            SetSemiTrans((LINE_G2 *)psx_addr(packet, sizeof(LINE_G2)), 1);
            render_insert_primitive((sint32)(depth << 4), r_u32(0x80040690));
            w_u32(0x80040690, r_u32(0x80040690) + 20);
        }
        address = r_u32(0x80059E60) + index * 8;
        stars = r_u32(0x80059E60) + index * 8;
        w_u16(address + 4, r_u16(stars + 4) - (512 >> band));
        stars = r_u32(0x80059E60) + index * 8;
        if (r_s16(stars + 4) < -8192)
        {
            address = r_u32(0x80059E60) + index * 8;
            stars = r_u32(0x80059E60) + index * 8;
            w_u16(address + 4, r_u16(stars + 4) + 0x4000);
        }
    }
}

void texture_load_status(void)
{
    uint32 size;
    uint32 buffer;
    TEX_INFO info;
    FUNCTION_MARKER(0x80012188, "SLUS_005.43");
    size = disc_file_size("LOADTEXT.TIM");
    buffer = heap_alloc_back(size);
    resource_load_into("LOADTEXT.TIM", buffer);
    texture_read_info(buffer + 4, &info);
    info.pixel.x = 320;
    info.pixel.y = 32;
    texture_upload(buffer, &info, 1);
    w_u32(0x80041EF8, info.pixel.x);
    w_u32(0x80041F20, info.pixel.y);
    w_u32(0x8003FE78, (r_u32(0x80041EF8) & 0x3F) << 2);
    w_u32(0x80040684, r_u32(0x80041F20) & 255);
    w_u32(0x80040680, ((uint32)(uint16)info.pixel.w << 2) + r_u32(0x8003FE78) - 1);
    w_u32(0x80040688, (uint16)info.pixel.h + r_u32(0x80040684) - 1);
    info.clut.x = 336;
    info.clut.y = 0;
    texture_upload(buffer, &info, 0);
    w_u32(0x80040F70, info.clut.x);
    w_u32(0x80040F74, info.clut.y);
    heap_free(buffer);
}

sint32 sound_stop_loops(void)
{
    FUNCTION_MARKER(0x8001200C, "SLUS_005.43");
    sound_stop_voice(r_u32(0x80052180));
    sound_stop_voice(r_u32(0x800510F0));
    sound_stop_voice(r_u32(0x800428C0));
    return sound_reset_voices();
}

uint32 object_init(uint32 object)
{
    FUNCTION_MARKER(0x80012530, "SLUS_005.43");
    w_u32(object + 8, 0);
    w_u32(object + 12, 0);
    w_u32(object + 16, 0);
    w_u32(object + 0x114, 0);
    w_u32(object + 0x3C, 0x8002DA68);
    w_u32(object + 4, 0);
    w_u32(object + 0xA0, 0);
    w_u32(object + 0x54, 0);
    w_u32(object + 0x58, 0);
    w_u32(object + 0x44, 0);
    w_u32(object + 0x5C, 0);
    w_u32(object + 0x4C, 0);
    w_u32(object + 0x178, 0);
    w_u32(object + 0x170, 0);
    w_u32(object + 0x1A8, 0);
    w_u32(object + 0x20, 0);
    w_u32(object + 0x24, 0);
    w_u32(object + 0x28, 0);
    w_u32(object + 0x214, 0);
    w_u32(object + 0x218, 0);
    w_u32(object + 0x21C, 0);
    w_u16(object + 0x224, 0);
    w_u16(object + 0x226, 0);
    w_u16(object + 0x228, 0);
    w_u32(object + 0xE0, -123);
    w_u32(object + 0x23C, 0);
    w_u32(object + 0x40, 0);
    w_u32(object + 0x9C, 1);
    w_u8(object + 0x26D, 0);
    w_u16(object + 0x264, 0);
    w_u16(object + 0x268, 0);
    w_u32(object + 0x20C, 0);
    w_u32(object + 0x84, -3);
    w_u32(object + 0x70, r_u32(object + 0x70) | 0x100000);
    w_u16(object + 0x24C, 60);
    matrix_set_identity(object + 0x11C);
    w_u32(object + 0x28, 0);
    w_u32(object + 0x24, 0);
    w_u32(object + 0x20, 0);
    w_u32(object + 0x1B8, 0);
    w_u32(object + 0x200, 0);
    w_u32(object + 0x204, 0);
    w_u16(object + 0x1C4, 0);
    w_u32(object + 0x1CC, 0);
    w_u32(object + 0x208, 0);
    w_u8(object + 0x26E, 0);
    w_u8(object + 0x26F, 0);
    w_u8(object + 0x270, 0);
    w_u32(object + 0x210, 0);
    return object;
}

uint32 camera_enable_flag20(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x80024438, "SLUS_005.43");
    flags = r_u32(0x8004EB30) | 0x20;
    w_u32(0x8004EB30, flags);
    return flags;
}

void camera_set_up(uint32 x, uint32 y, uint32 z)
{
    FUNCTION_MARKER(0x80024570, "SLUS_005.43");
    w_u32(0x80033D40, x);
    w_u32(0x80033D44, y);
    w_u32(0x80033D48, z);
}

uint32 scene_hangar_update(void)
{
    uint32 fade;
    uint32 product;
    uint32 intensity;
    uint32 red;
    uint32 green;
    uint32 blue;
    uint32 sine;
    uint32 cosine;
    FUNCTION_MARKER(0x80012F6C, "SLUS_005.43");
    if (r_s32(0x8003110C) < 256)
        w_u32(0x8003110C, r_u32(0x8003110C) + 2);
    else if (r_s32(0x80031108) < 128 && (r_u32(0x800310B0) & 1))
        w_u32(0x80031108, r_u32(0x80031108) + 1);
    if (r_s32(0x800310FC) > 0)
        w_u32(0x800310FC, r_u32(0x800310FC) - 1);
    fade = 255 - r_u32(0x800310A8);
    product = r_u32(0x800310FC) * fade;
    sound_set_voice_volume(r_u32(0x80052180), (product >> 8) & 255);
    product = r_u32(0x80031100) * fade;
    sound_set_voice_volume(r_u32(0x800510F0), (product >> 8) & 255);
    product = r_u32(0x80031104) * fade;
    sound_set_voice_volume(r_u32(0x800428C0), (product >> 8) & 255);
    sound_set_voice_pitch(r_u32(0x800510F0), r_u32(0x80031108));
    sound_set_voice_pitch(r_u32(0x800428C0), r_u32(0x8003110C));
    sound_tick();
    if (r_s32(0x800310B0) >= 4 && r_s32(0x800310B0) < 6)
        SetDispMask(1);
    w_u32(0x800310B8, r_u32(0x800310B8) + 256);
    if (r_s32(0x800310C0) > 0)
    {
        red = r_s16(0x800310D8);
        green = r_s16(0x800310DA);
        blue = r_s16(0x800310DC);
        light_set_color(0, red, green, blue);
        sine = (uint32)rsin(r_s32(0x800310B8)) << 12;
        sine = (uint32)((sint32)sine >> 12);
        cosine = (uint32)rcos(r_s32(0x800310B8)) << 12;
        cosine = (uint32)((sint32)cosine >> 12);
        light_set_direction(0, sine, 0, cosine);
        intensity = 255 - (r_u32(0x800310F4) << 1);
        if ((sint32)intensity < 0)
            intensity = 0;
        w_u32(0x800310F4, (r_u32(0x800310F4) + 10) & 0x1FF);
        product = (uint32)r_s16(0x800310E0) * intensity;
        red = (uint32)((sint32)product >> 8);
        product = (uint32)r_s16(0x800310E2) * intensity;
        green = (uint32)((sint32)product >> 8);
        product = (uint32)r_s16(0x800310E4) * intensity;
        blue = (uint32)((sint32)product >> 8);
        light_set_color(1, red, green, blue);
        light_set_direction(1, 0, 4096, 0);
        intensity = 0;
        product = (uint32)r_s16(0x800310E8) * intensity;
        red = (uint32)((sint32)product >> 8);
        product = (uint32)r_s16(0x800310EA) * intensity;
        green = (uint32)((sint32)product >> 8);
        product = (uint32)r_s16(0x800310EC) * intensity;
        blue = (uint32)((sint32)product >> 8);
        light_set_color(2, red, green, blue);
        light_set_direction(2, 0, 0, 4096);
    }
    else
    {
        light_set_color(0, 0, 0, 0);
        light_set_direction(0, 4096, 0, 0);
        light_set_color(1, 0, 0, 0);
        light_set_direction(1, 0, 4096, 0);
        light_set_color(2, 0, 0, 0);
        light_set_direction(2, 0, 0, 4096);
    }
    cosine = (uint32)rcos(r_s32(0x800310B8) >> 2);
    product = cosine * r_u32(0x8003111C);
    w_u16(0x800310D0, (sint32)product >> 12);
    w_u16(0x800310D2, 0);
    sine = (uint32)rsin(r_s32(0x800310B8) >> 3);
    product = sine * r_u32(0x8003111C);
    w_u16(0x800310D4, (sint32)product >> 12);
    RotMatrixYXZ((SVECTOR *)psx_addr(0x800310D0, sizeof(SVECTOR)), (MATRIX *)psx_addr(0x80034EFC, sizeof(MATRIX)));
    w_u16(0x800310D0, 0);
    w_u16(0x800310D2, -1024);
    w_u16(0x800310D4, 0);
    RotMatrixYXZ((SVECTOR *)psx_addr(0x800310D0, sizeof(SVECTOR)), (MATRIX *)psx_addr(0x800408B4, sizeof(MATRIX)));
    if (r_s32(0x800407A4) < r_s32(0x80034DEC))
    {
        camera_set_target(0x80040798);
        w_u32(0x800310C0, 2000);
        w_u32(0x800310C4, -1250);
        w_u32(0x800310C8, 1000);
    }
    else
    {
        camera_set_target(0x80034DE0);
        if (r_s32(0x800310C0) > 0)
        {
            w_u32(0x800310C0, r_u32(0x800310C0) - 10);
            if (r_u32(0x800310B0) & 1)
                w_u32(0x800310C4, r_u32(0x800310C4) + 1);
            product = (uint32)(r_s32(0x80031114) >> 2);
            w_u32(0x800310C8, r_u32(0x800310C8) + product);
        }
        else
        {
            w_u32(0x80034DF0, r_u32(0x80034DF0) + 50);
            product = r_u32(0x800310C8) + 45;
            w_u32(0x800310C8, product + r_u32(0x80031114));
            if (r_s32(0x8003111C) > 0)
                w_u32(0x8003111C, r_u32(0x8003111C) - 1);
            if (r_s32(0x800310C8) < r_s32(0x80034DF0))
                w_u32(0x800310C8, r_u32(0x800310C8) + 4);
            if (r_s32(0x800310C4) < r_s32(0x80034DEC))
                w_u32(0x800310C4, r_u32(0x800310C4) + 1);
        }
    }
    w_u32(0x80034DF0, r_u32(0x80034DF0) + r_u32(0x80031114));
    w_u32(0x80034DEC, r_u32(0x80034DEC) - r_u32(0x80031118));
    if (r_s32(0x80034DEC) < -500)
    {
        if (r_s32(0x80031118) > 0)
            w_u32(0x80031118, r_u32(0x80031118) - 1);
        if (!(r_u32(0x800310B0) & 3))
            w_u32(0x80031114, r_u32(0x80031114) + 1);
    }
    camera_enable_flag20();
    camera_set_up(0, (uint32)-4096, 0);
    r_u32(0x80040C94);
    camera_update_modes();
    camera_set_position(0x800310C0);
    if (r_s32(0x800310B0) < 400)
        object_draw_transformed(0x80040798);
    return object_draw_transformed(0x80034DE0);
}

void scene_hangar_draw(void)
{
    uint32 index;
    uint32 value;
    uint32 next;
    uint32 color;
    uint32 packet;
    uint32 owner;
    SVECTOR point = {0};
    sint32 screen;
    sint32 projection;
    sint32 flags;
    FUNCTION_MARKER(0x800137D8, "SLUS_005.43");
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, sizeof(MATRIX)));
    w_u32(0x800310B4, 0x56785678);
    for (index = 0; index < 175; ++index)
    {
        value = r_u32(0x800310B4) >> 1;
        next = value ^ (r_u32(0x800310B4) << 3);
        w_u32(0x800310B4, next);
        point.vx = (sint16)((next & 0x7FF) - 1024);
        value = r_u32(0x800310B4) >> 1;
        next = value ^ (r_u32(0x800310B4) << 3);
        w_u32(0x800310B4, next);
        value = (next & 0x3FF) - 512;
        point.vy = (sint16)(((value << 1) + value) >> 1);
        value = r_u32(0x800310B4) >> 1;
        next = value ^ (r_u32(0x800310B4) << 3);
        w_u32(0x800310B4, next);
        point.vz = (sint16)((next & 0x3FF) + 512);
        RotTransPers(&point, &screen, &projection, &flags);
        color = (index >> 2) + 32;
        color = (color << 8) + color + (color << 16);
        packet = r_u32(0x80040690);
        w_u32(packet + 4, color);
        packet = r_u32(0x80040690);
        w_u16(packet + 8, (uint32)screen);
        packet = r_u32(0x80040690);
        w_u16(packet + 10, (uint32)screen >> 16);
        packet = r_u32(0x80040690);
        SetTile1((TILE_1 *)psx_addr(packet, sizeof(TILE_1)));
        owner = r_u32(0x8004608C);
        packet = r_u32(0x80040690);
        AddPrim(psx_addr(owner + 0x45A0, 4), psx_addr(packet, 4));
        w_u32(0x80040690, r_u32(0x80040690) + 12);
    }
}

uint32 render_insert_primitive(sint32 depth, uint32 packet)
{
    uint32 base;
    uint32 slot;
    FUNCTION_MARKER(0x8002F0BC, "SLUS_005.43");
    base = r_u32(0x800421AC);
    if (depth < 0x4000)
    {
        if (depth < 0)
            depth = 0;
        slot = base + (((uint32)depth >> 6) + 64) * 4;
    }
    else
        slot = base + ((((uint32)depth & 0x3FFFFE) >> 11) + 320) * 4;
    AddPrim(psx_addr(slot, 4), psx_addr(packet, 4));
    return r_u32(slot);
}

uint32 scene_stars_draw(void)
{
    uint32 packet;
    uint32 vertex;
    uint32 color;
    uint32 phase;
    FUNCTION_MARKER(0x80014354, "SLUS_005.43");
    packet = r_u32(0x80040690);
    w_u32(0x80040690, r_u32(0x80040690) + 36);
    for (vertex = 0; vertex < 4; ++vertex)
    {
        phase = r_u32(0x800310B8) + vertex * 333;
        color = (uint32)rsin((sint32)phase) << 5;
        w_u8(packet + 4 + vertex * 8, ((sint32)color >> 12) + 32);
        w_u8(packet + 5 + vertex * 8, 0);
        phase = r_u32(0x800310B8) + vertex * 333;
        color = (uint32)rcos((sint32)phase) << 5;
        w_u8(packet + 6 + vertex * 8, ((sint32)color >> 12) + 32);
    }
    w_u16(packet + 8, 0);
    w_u16(packet + 10, 0);
    w_u16(packet + 16, 320);
    w_u16(packet + 18, 0);
    w_u16(packet + 24, 0);
    w_u16(packet + 26, 256);
    w_u16(packet + 32, 320);
    w_u16(packet + 34, 256);
    SetPolyG4((POLY_G4 *)psx_addr(packet, sizeof(POLY_G4)));
    return render_insert_primitive(999999, packet);
}

void scene_hangar_frame(void)
{
    FUNCTION_MARKER(0x800145FC, "SLUS_005.43");
    w_u32(0x80031110, r_u32(0x80031110) ^ 1);
    if (r_u32(0x80053690))
        return;
    w_u32(0x80053690, 1);
    gte_save_data(0x80052100);
    if (r_u32(0x80031110))
        scene_hangar_draw();
    else
    {
        w_u32(0x800310B0, r_u32(0x800310B0) + 1);
        render_present_frame();
        render_swap_buffers();
        scene_hangar_update();
    }
    scene_status_disabled();
    gte_restore_data(0x80052100);
    w_u32(0x80053690, 0);
}

void scene_stars_frame(void)
{
    FUNCTION_MARKER(0x80014704, "SLUS_005.43");
    w_u32(0x80031110, r_u32(0x80031110) ^ 1);
    if (r_u32(0x80053690))
        return;
    w_u32(0x80053690, 1);
    gte_save_data(0x80052100);
    if (r_u32(0x80031110))
        scene_stars_draw();
    else
    {
        w_u32(0x800310B0, r_u32(0x800310B0) + 1);
        render_present_frame();
        render_swap_buffers();
        scene_stars_update();
    }
    scene_status_disabled();
    gte_restore_data(0x80052100);
    w_u32(0x80053690, 0);
}

void scene_status_disabled(void)
{
    FUNCTION_MARKER(0x8001231C, "SLUS_005.43");
    /* Original entry jumps over the entire drawing block */
}

uint32 scene_start_hangar(void)
{
    FUNCTION_MARKER(0x8001480C, "SLUS_005.43");
    w_u32(0x8004EB28, 0);
    w_u32(0x80053690, 0);
    w_u32(0x800310B0, 0);
    w_u32(0x80046170, 0x800145FC);
    w_u32(0x800310C8, 8000);
    w_u32(0x80031100, r_s16(0x800118FC) >> 1);
    w_u32(0x80031104, r_s16(0x800118FC) >> 1);
    w_u32(0x800310FC, r_s16(0x800118FC) >> 1);
    w_u32(0x80031108, 64);
    w_u32(0x8003110C, 64);
    return VSyncCallbackPSX(0x800145FC);
}

uint32 scene_start_stars(void)
{
    FUNCTION_MARKER(0x800148F0, "SLUS_005.43");
    w_u32(0x8004EB28, 0);
    w_u32(0x80053690, 0);
    w_u32(0x800310B0, 0);
    w_u32(0x80046170, 0x80014704);
    w_u32(0x800310C8, 8000);
    w_u32(0x80031100, r_s16(0x800118FC) >> 1);
    w_u32(0x80031104, r_s16(0x800118FC) >> 1);
    w_u32(0x800310FC, 0);
    w_u32(0x80031108, 64);
    w_u32(0x8003110C, 64);
    return VSyncCallbackPSX(0x80014704);
}

void gte_save_data(uint32 destination)
{
    uint32 index;
    FUNCTION_MARKER(0x8001DF64, "SLUS_005.43");
    for (index = 0; index < 32; ++index)
        w_u32(destination + index * 4, xport_gte_read_data(index));
}

void gte_restore_data(uint32 source)
{
    uint32 index;
    FUNCTION_MARKER(0x8001E048, "SLUS_005.43");
    for (index = 0; index < 32; ++index)
        xport_gte_write_data(index, r_u32(source + index * 4));
}

sint32 render_set_semi_trans(sint32 enabled)
{
    uint32 flag;
    FUNCTION_MARKER(0x80016E20, "SLUS_005.43");
    flag = enabled ? 2 : 0;
    w_u32(0x80031148, flag);
    w_u32(0x80031144, flag);
    return 2;
}

uint32 render_swap_buffers(void)
{
    uint32 owner;
    uint32 base;
    uint32 packet;
    FUNCTION_MARKER(0x800127C0, "SLUS_005.43");
    w_u32(0x800310AC, r_u32(0x800310AC) ^ 1);
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
    owner = r_u32(0x8004608C);
    w_u32(0x80040690, r_u32(owner + 0x45F0));
    packet = r_u32(0x80040690);
    w_u32(0x8004217C, packet);
    return packet;
}

sint32 sound_init_loops(void)
{
    sint32 voice;
    FUNCTION_MARKER(0x80011F4C, "SLUS_005.43");
    sound_reset_voices();
    voice = sound_reserve_voice(r_s32(0x80042958), 0, 15, 15, 128, 0);
    w_u32(0x800510F0, voice);
    voice = sound_reserve_voice(r_s32(0x800429B0), 0, 15, 15, 128, 0);
    w_u32(0x80052180, voice);
    voice = sound_reserve_voice(r_s32(0x8004297C), 0, 15, 15, 128, 0);
    w_u32(0x800428C0, voice);
    return voice;
}

void render_init_lights(void)
{
    MATRIX light = {0};
    FUNCTION_MARKER(0x8001206C, "SLUS_005.43");
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
    render_set_clear_color(0, 0, 0);
    SetBackColor(48, 48, 48);
    light_set_color(0, 192, 64, 0);
    light_set_direction(0, 4096, 0, 0);
    light_set_color(1, 192, 64, 0);
    light_set_direction(1, 0, 4096, 0);
    light_set_color(2, 192, 64, 0);
    light_set_direction(2, 0, 0, 4096);
}

void render_present_frame(void)
{
    uint32 owner;
    uint32 mode_packet;
    uint32 tile;
    uint32 color;
    uint32 value;
    uint32 ot;
    uint16 page;
    FUNCTION_MARKER(0x800128B0, "SLUS_005.43");
    owner = r_u32(0x8004608C);
    PutDrawEnv((DRAWENV *)psx_addr(owner, sizeof(DRAWENV)));
    owner = r_u32(0x8004608C);
    PutDispEnv((DISPENV *)psx_addr(owner + 92, sizeof(DISPENV)));
    mode_packet = r_u32(0x80040690);
    page = GetTPage(0, 3, 0, 0);
    SetDrawMode((DR_MODE *)psx_addr(mode_packet, sizeof(DR_MODE)), 1, 1, page, 0);
    owner = r_u32(0x8004608C);
    AddPrim(psx_addr(owner + 0x45A0, 4), psx_addr(mode_packet, 4));
    mode_packet += 12;
    w_u32(0x80040690, mode_packet);
    if (r_s32(0x800310A8) > 0)
    {
        tile = r_u32(0x80040690);
        w_u32(0x80040690, r_u32(0x80040690) + 64);
        value = r_u32(0x800310A8);
        color = (value << 8) + r_u32(0x800310A8);
        color += r_u32(0x800310A8) << 16;
        w_u32(tile + 4, color);
        w_u16(tile + 8, 0);
        w_u16(tile + 10, 0);
        w_u16(tile + 12, 320);
        w_u16(tile + 14, r_u32(0x800311C0) ? 256 : 240);
        SetTile((TILE *)psx_addr(tile, sizeof(TILE)));
        owner = r_u32(0x8004608C);
        AddPrim(psx_addr(owner + 112, 4), psx_addr(tile, 4));
        SetSemiTrans((TILE *)psx_addr(tile, sizeof(TILE)), 1);
        mode_packet = r_u32(0x80040690);
        page = GetTPage(0, 2, 0, 0);
        SetDrawMode((DR_MODE *)psx_addr(mode_packet, sizeof(DR_MODE)), 1, 1, page, 0);
        owner = r_u32(0x8004608C);
        AddPrim(psx_addr(owner + 112, 4), psx_addr(mode_packet, 4));
        mode_packet += 12;
        w_u32(0x80040690, mode_packet);
        w_u32(0x800310A8, r_u32(0x800310A8) + 5);
        if (r_s32(0x800310A8) >= 256)
        {
            w_u32(0x800310B0, 3000);
            w_u32(0x800310A8, 255);
        }
    }
    value = r_u32(0x80034D78);
    owner = r_u32(0x8004608C);
    ot = (value << 2) + 112 + owner;
    DrawOTag((uint32 *)psx_addr(ot, 4));
}

sint32 sound_reserve_voice(sint32 sample, uint32 volume, sint32 left, sint32 right, uint32 pitch, uint32 flags)
{
    uint32 voice;
    uint32 offset;
    uint32 sample_offset;
    uint32 base_pitch;
    uint32 product;
    uint32 scaled;
    SpuVoiceAttr attr = {0};
    FUNCTION_MARKER(0x80025664, "SLUS_005.43");
    if (!r_u8(0x80031258) || !r_u8(0x8003125C))
        return -1;
    voice = sound_find_voice();
    product = (volume & 255) * r_u8(0x8003125B);
    scaled = (uint32)(((uint64)product * 0x80808081) >> 32) >> 7;
    sample_offset = (uint32)sample * 20;
    offset = voice * 28;
    base_pitch = r_u32(0x80032D00 + sample_offset);
    w_u32(0x80033994 + offset, 3);
    product = pitch * base_pitch;
    w_u32(0x80033998 + offset, flags);
    w_u8(0x800339A7 + offset, left);
    w_u8(0x800339A8 + offset, right);
    w_u32(0x8003399C + offset, pitch);
    w_u8(0x800339A4 + offset, voice);
    w_u32(0x800336F0 + offset, 1);
    w_u32(0x800336F4 + offset, 1);
    w_u32(0x80033990 + offset, base_pitch);
    w_u8(0x800339A6 + offset, scaled);
    if ((sint32)product < 0)
        product += 255;
    product = (uint32)((sint32)(product << 8) >> 16);
    w_u32(0x80033700 + offset, product);
    w_u32(0x80033704 + offset, voice);
    attr.voice = (uint32)1 << (voice & 31);
    attr.mask = 0x90;
    attr.addr = r_u32(0x80032CF0 + sample_offset);
    attr.pitch = r_u32(0x80033700 + offset);
    SpuSetVoiceAttr(&attr);
    w_u8(0x800339A5 + offset, 255);
    sound_update_voice(voice);
    return (sint32)voice;
}
