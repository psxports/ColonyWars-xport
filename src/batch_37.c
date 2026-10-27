#include "game_functions.h"
#include "psx_gpu.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void front_select_initial_campaign_file(void)
{
    uint32 files[80];
    uint32 first, second, third, index, selected, length;
    sint32 count, found;
    FUNCTION_MARKER(0x8008F1F4, "FRONT.OVL");
    if (r_u8(0x80010518) != 0)
        return;
    found = resource_exists((const char *)psx_addr(0x80086744, 7));
    if (found == 1) {
        first = r_u32(0x80086744);
        second = (uint32)r_s16(0x80086748);
        third = (uint32)r_s8(0x8008674A);
        w_u32(0x80010518, first);
        w_u16(0x8001051C, second);
        w_u8(0x8001051E, third);
        w_u16(0x800595FC, 1);
        w_u16(0x800595F8, 1);
        w_u16(0x800595FE, 1);
        w_u16(0x800595FA, 1);
        return;
    }
    found = resource_exists((const char *)psx_addr(0x8008674C, 9));
    if (found == 1) {
        first = r_u32(0x8008674C);
        second = r_u32(0x80086750);
        third = (uint32)r_s8(0x80086754);
        w_u32(0x80010518, first);
        w_u32(0x8001051C, second);
        w_u8(0x80010520, third);
        w_u16(0x800595FC, 0);
        w_u16(0x800595F8, 0);
        w_u16(0x800595FE, 0);
        w_u16(0x800595FA, 0);
        return;
    }
    count = front_list_files((const char *)psx_addr(0x80086758, 3), files, 80);
    index = 0;
    for (;;) {
        selected = files[index];
        length = 0;
        while (r_u8(selected + length) != 0)
            ++length;
        found = resource_exists((const char *)psx_addr(selected, (size_t)length + 1));
        if (found == 1)
            break;
        if ((sint32)index >= count)
            return;
        if ((sint32)index >= 80)
            break;
        ++index;
    }
    if ((sint32)index >= count || (sint32)index >= 80)
        return;
    selected = files[index];
    length = 0;
    while (r_u8(selected + length) != 0)
        ++length;
    strcpy((char *)psx_addr(0x80010518, (size_t)length + 1), (const char *)psx_addr(selected, (size_t)length + 1));
    front_parse_campaign_filename(0x80010518);
}

void front_update_control_axis_calibration(void)
{
    char text[40];
    uint8 marker[12];
    uint32 ticks, dirty, old_config, config, status, type, offset, label, length;
    uint32 state, index, prompt, mode, x, y, object;
    FUNCTION_MARKER(0x800A4D4C, "FRONT.OVL");
    ticks = r_u16(0x800BD360);
    dirty = r_u8(0x8011328E);
    w_u16(0x800BD360, ticks + 1);
    if (dirty == 0) {
        old_config = r_u32(0x80113268);
        config = r_u32(0x800313C4);
        if (old_config != config || r_u8(0x8004EA28) != 0) {
            w_u8(0x8011328E, 1);
            w_u32(0x80113268, config);
            w_u32(0x800F3144, 1);
        }
    }
    status = r_u8(0x8004EA28);
    if (status == 0 || status == 0xFF) {
        if (status == 0xFF) {
            offset = 0x14;
        } else {
            type = r_u8(0x8004EA29);
            switch (type) {
            case 0x41: offset = 0; break;
            case 0x53: offset = 4; break;
            case 0x73: offset = 8; break;
            case 0x23: offset = 12; break;
            case 0x12: offset = 16; break;
            default: offset = 24; break;
            }
        }
        label = r_u32(r_u32(0x80113270) + offset);
        length = 0;
        while (r_u8(label + length) != 0)
            ++length;
        strcpy(text, (const char *)psx_addr(label, (size_t)length + 1));
    }
    w_u32(0x800FB870, 0xFD);
    front_draw_colored_text_native(text, 46, 64, 0x808080);
    if (r_u8(0x8011328E) == 0) {
        state = r_u8(0x8011327F);
        switch (state) {
        case 0:
            front_draw_control_axis_mode();
            front_prepare_control_axis_marker_native(marker, 0);
            front_draw_control_axis_marker_native(marker, r_u16(0x800BD360));
            type = r_u8(0x8004EA29);
            prompt = type == 0x73 ? 19 : 22;
            front_draw_centered_control_label(154, prompt, (uint32)(sint32)(sint16)0x8080);
            if (front_control_calibration_buttons() != 0) {
                mode = r_u8(0x8011327E);
                if (mode == 0) {
                    x = r_u8(0x8004EA2E); y = r_u8(0x8004EA2F);
                    w_u8(0x80113281, x); w_u8(0x80113282, y);
                }
                if (mode == 1) {
                    x = r_u8(0x8004EA2C); y = r_u8(0x8004EA2D);
                    w_u8(0x80113288, x); w_u8(0x80113289, y);
                }
                w_u8(0x8011327F, 2);
                sound_play_sample_pitch(11, 128);
                sound_play_sample_pitch(11, 128);
            }
            break;
        case 1:
            front_draw_control_axis_mode();
            for (index = 0; index < 4; ++index) {
                front_prepare_control_axis_marker_native(marker, index & 255);
                front_draw_control_axis_marker_native(marker, r_u16(0x800BD360));
            }
            type = r_u8(0x8004EA29);
            prompt = type == 0x73 ? 20 : 23;
            front_draw_centered_control_label(154, prompt, (uint32)(sint32)(sint16)0x8080);
            if (front_control_calibration_buttons() != 0) {
                mode = r_u8(0x8011327E);
                if (mode == 0) {
                    x = r_u8(0x8004EA2E); y = r_u8(0x8004EA2F);
                    w_u8(0x80113283, x); w_u8(0x80113284, y);
                }
                if (mode == 1) {
                    x = r_u8(0x8004EA2C); y = r_u8(0x8004EA2D);
                    w_u8(0x8011328A, x); w_u8(0x8011328B, y);
                }
                w_u8(0x8011327F, 0);
                sound_play_sample_pitch(11, 128);
                sound_play_sample_pitch(11, 128);
            }
            break;
        case 2:
            front_draw_control_axis_mode();
            front_prepare_control_axis_marker_native(marker, 0);
            front_draw_control_axis_marker_native(marker, r_u16(0x800BD360));
            type = r_u8(0x8004EA29);
            prompt = type == 0x73 ? 21 : 24;
            front_draw_centered_control_label(154, prompt, (uint32)(sint32)(sint16)0x8080);
            if (front_control_calibration_buttons() != 0) {
                mode = r_u8(0x8011327E);
                if (mode == 0) {
                    x = r_u8(0x8004EA2E); y = r_u8(0x8004EA2F);
                    w_u8(0x80113280, 1);
                    w_u8(0x80113285, x); w_u8(0x80113286, y);
                }
                if (mode == 1) {
                    x = r_u8(0x8004EA2C); y = r_u8(0x8004EA2D);
                    w_u8(0x80113287, 1);
                    w_u8(0x8011328C, x); w_u8(0x8011328D, y);
                }
                sound_play_sample_pitch(11, 128);
                sound_play_sample_pitch(11, 128);
                config = r_u32(0x800313C4);
                if (r_s16(config + 0x2E) < 3 || r_u8(0x8011327E) == 1) {
                    w_u8(0x8011327F, 3);
                } else {
                    w_u8(0x8011327F, 1);
                    w_u8(0x8011327E, 1);
                }
            }
            break;
        case 3:
            label = r_u32(r_u32(0x80113270) + 0x64);
            front_draw_colored_text(label, 46, 100, 0x808080);
            front_configure_layer(0, 4, 0xA9);
            w_u16(0x800F3116, 1);
            break;
        }
    }
    object = r_u32(0x800F6F9C);
    if (r_s32(object + 0xA8) >= 0)
        front_update_control_viewer_animation();
}

void front_update_control_preset_menu(void)
{
    char text[40];
    uint32 value, status, diagram, bindings, type, offset, table, label, length, object;
    sint32 preset;
    FUNCTION_MARKER(0x800A3738, "FRONT.OVL");
    if (r_u8(0x8011328E) == 0 && r_u8(0x800313D4) != 0) {
        w_u8(0x8011328E, 1);
        w_u8(0x800313D4, 0);
        w_u32(0x800F3144, 1);
    }
    value = r_u32(0x80053684);
    w_u32(0x800FB870, 0x53);
    if ((value & 0x8000) != 0) {
        sound_play_sample_pitch(10, 128);
        sound_play_sample_pitch(10, 128);
        value = r_u16(0x80113276) - 1;
        w_u16(0x80113276, value);
        if ((sint16)value < 0)
            w_u16(0x80113276, 5);
        w_u16(0x8011327C, 0xFFE2);
    }
    if ((r_u32(0x80053684) & 0x2000) != 0) {
        sound_play_sample_pitch(10, 128);
        sound_play_sample_pitch(10, 128);
        value = r_u16(0x80113276) + 1;
        w_u16(0x80113276, value);
        if ((sint16)value >= 6)
            w_u16(0x80113276, 0);
        w_u16(0x8011327C, 30);
    }
    status = r_u8(0x8004EA28);
    w_u32(0x800FB870, 0x53);
    if (status == 0xFF) {
        label = r_u32(r_u32(0x80113270) + 0x14);
        length = 0;
        while (r_u8(label + length) != 0)
            ++length;
        strcpy(text, (const char *)psx_addr(label, (size_t)length + 1));
        object = r_u32(0x800F6F9C);
        w_u32(object + 0xA8, 0xFFFFFFFF);
    } else if (status == 0) {
        preset = r_s16(0x80113276);
        diagram = r_u32(0x8011326C);
        bindings = r_u32(0x80113264);
        front_draw_control_diagram(diagram, bindings + (uint32)preset * 48u);
        type = r_u8(0x8004EA29);
        switch (type) {
        case 0x41: offset = 0; break;
        case 0x53: offset = 4; break;
        case 0x73: offset = 8; break;
        case 0x23: offset = 12; break;
        case 0x12: offset = 16; break;
        default: offset = 24; break;
        }
        table = r_u32(0x80113270);
        if (type == 0x41 || type == 0x53 || type == 0x73) {
            preset = r_s16(0x80113276);
            label = r_u32(table + offset);
            length = 0;
            while (r_u8(label + length) != 0)
                ++length;
            sprintf(text, (const char *)psx_addr(0x80086BB4, 6), (const char *)psx_addr(label, (size_t)length + 1), preset + 1);
        } else {
            label = r_u32(table + offset);
            length = 0;
            while (r_u8(label + length) != 0)
                ++length;
            strcpy(text, (const char *)psx_addr(label, (size_t)length + 1));
        }
    }
    w_u32(0x800FB870, 0xFD);
    front_draw_colored_text_native(text, 46, 64, 0x808080);
    object = r_u32(0x800F6F9C);
    w_u32(0x800FB870, 0x53);
    if (r_s32(object + 0xA8) >= 0 && r_s16(0x80113278) != 1)
        front_update_control_viewer_animation();
    if (r_s16(0x8011327C) < 0) {
        front_draw_previous_ship_arrow(254, 130, 1);
        value = r_u16(0x8011327C);
        w_u16(0x8011327C, value + 1);
    } else {
        front_draw_previous_ship_arrow(254, 130, 0);
    }
    if (r_s16(0x8011327C) > 0) {
        front_draw_next_ship_arrow(322, 130, 1);
        value = r_u16(0x8011327C);
        w_u16(0x8011327C, value - 1);
    } else {
        front_draw_next_ship_arrow(322, 130, 0);
    }
}

void front_update_control_viewer_menu(void)
{
    char text[40];
    uint32 status, type, diagram, offset, label, length, object;
    FUNCTION_MARKER(0x800A4224, "FRONT.OVL");
    w_u32(0x800FB870, 0x53);
    status = r_u8(0x8004EA28);
    if (status == 0xFF) {
        w_u8(0x800F2FA9, 1);
    } else {
        type = r_u8(0x8004EA29);
        w_u8(0x800F2FA9, type == 0x53 || type == 0x73 ? 0 : 1);
    }
    status = r_u8(0x8004EA28);
    diagram = r_u32(0x8011326C);
    if (status == 0xFF) {
        label = r_u32(r_u32(0x80113270) + 0x14);
        length = 0;
        while (r_u8(label + length) != 0)
            ++length;
        strcpy(text, (const char *)psx_addr(label, (size_t)length + 1));
    } else if (status == 0 && diagram != 0) {
        front_draw_control_diagram(diagram, r_u32(0x800313C4));
        type = r_u8(0x8004EA29);
        switch (type) {
        case 0x41: offset = 0; break;
        case 0x53: offset = 4; break;
        case 0x73: offset = 8; break;
        case 0x23: offset = 12; break;
        case 0x12: offset = 16; break;
        default: offset = 24; break;
        }
        label = r_u32(r_u32(0x80113270) + offset);
        length = 0;
        while (r_u8(label + length) != 0)
            ++length;
        strcpy(text, (const char *)psx_addr(label, (size_t)length + 1));
    }
    w_u32(0x800FB870, 0xFD);
    front_draw_colored_text_native(text, 46, 64, 0x808080);
    object = r_u32(0x800F6F9C);
    w_u32(0x800FB870, 0x53);
    if (r_s32(object + 0xA8) >= 0 && r_s16(0x80113278) != 1)
        front_update_control_viewer_animation();
}

static sint32 disc_extracted_file_path(char *path, size_t capacity, const char *name)
{
    size_t length, index;
    if (!name) {
        errno = EINVAL;
        return 0;
    }
    while (*name == '\\' || *name == '/')
        ++name;
    length = strcspn(name, ";");
    if (length == 0 || (name[length] != 0 && strcmp(name + length, ";1") != 0)) {
        errno = ENOENT;
        return 0;
    }
    if (capacity < 8 || length > capacity - 8) {
        errno = ENAMETOOLONG;
        return 0;
    }
    memcpy(path, "DATA/1/", 7);
    for (index = 0; index < length; ++index)
        path[index + 7] = name[index] == '\\' ? '/' : name[index];
    path[length + 7] = 0;
    return 1;
}

sint32 game_extracted_file_size(const char *name, uint32 *length)
{
    char path[132];
    size_t bytes;
    if (!disc_extracted_file_path(path, sizeof(path), name))
        return 0;
    if (!xport_file_size(path, &bytes))
        return 0;
    if (bytes > 0xFFFFFFFF) {
        errno = EFBIG;
        return 0;
    }
    *length = (uint32)bytes;
    return 1;
}

static uint32 disc_load_extracted_file(const char *name, uint32 from_back)
{
    char path[132];
    size_t copied;
    long file_length;
    uint32 capacity, allocation;
    sint32 error;
    FILE *file;
    if (!disc_extracted_file_path(path, sizeof(path), name))
        return 0;
    file = xport_fopen(path, "rb");
    if (!file)
        return 0;
    errno = 0;
    if (fseek(file, 0, SEEK_END) || (file_length = ftell(file)) < 0 || fseek(file, 0, SEEK_SET)) {
        error = errno ? errno : EIO;
        fclose(file);
        errno = error;
        return 0;
    }
    if ((uint64)file_length > 0xFFFFF7FF) {
        fclose(file);
        errno = EFBIG;
        return 0;
    }
    capacity = (((uint32)file_length >> 11) + 1) << 11;
    allocation = from_back ? heap_alloc_back(capacity) : heap_alloc_front(capacity);
    if (allocation == 0) {
        fclose(file);
        errno = ENOMEM;
        return 0;
    }
    errno = 0;
    copied = file_length ? xport_fread(psx_addr(allocation, (size_t)file_length), 1, (size_t)file_length, file) : 0;
    if (copied != (size_t)file_length) {
        error = errno ? errno : EIO;
        fclose(file);
        heap_free(allocation);
        errno = error;
        return 0;
    }
    if (fclose(file) != 0) {
        error = errno ? errno : EIO;
        heap_free(allocation);
        errno = error;
        return 0;
    }
    return allocation;
}

uint32 disc_load_named_file_back(const char *name)
{
    FUNCTION_MARKER(0x800213C0, "SLUS_005.43");
    return disc_load_extracted_file(name, 1);
}

uint32 disc_load_file(const char *name)
{
    FUNCTION_MARKER(0x80021494, "SLUS_005.43");
    return disc_load_extracted_file(name, 0);
}

void front_return_without_effects(void)
{
    FUNCTION_MARKER(0x800A27E8, "FRONT.OVL");
    /* Original body only returns */
}

void game_return_without_effects(void)
{
    FUNCTION_MARKER(0x800B03E0, "GAME.OVL");
    /* Original body only returns */
}

uint32 front_probe_card_sector(sint32 channel)
{
    FUNCTION_MARKER(0x8009D4F0, "FRONT.OVL");
    /* Memory card disabled pending xport_save/load */
    return 0;
}

void game_render_nearby_patch(uint32 descriptor)
{
    uint32 camera = r_u32(0x80040C94);
    uint32 x, y, z;
    FUNCTION_MARKER(0x800CA294, "GAME.OVL");
    x = r_u32(descriptor);
    x -= r_u32(camera + 8);
    y = r_u32(descriptor + 4);
    y -= r_u32(camera + 12);
    z = r_u32(descriptor + 8);
    z -= r_u32(camera + 16);
    if (vector_length_approx((sint32)x, (sint32)y, (sint32)z) > 500000)
        return;
    w_u32(0x80034D6C, 0);
    w_u32(0x80034D70, 0);
    w_u32(0x80034D74, 0);
    SetRotMatrix((MATRIX *)psx_addr(0x80034D58, 20));
    SetTransMatrix((MATRIX *)psx_addr(0x80034D58, 32));
    camera = r_u32(0x80040C94);
    game_render_interpolated_patch(descriptor, camera);
}

/* Canonical callback admission keeps table indices in 0..2 */
void game_update_target_tracking_ai(uint32 object)
{
    sint16 pairs[6], increments[3];
    uint32 words[3], tail, target, callback, state, value, current, limit, kind, ticks;
    sint32 mode, index, choice, distance;
    FUNCTION_MARKER(0x800DA8BC, "GAME.OVL");
    words[0] = r_u32(0x80076694);
    words[1] = r_u32(0x80076698);
    words[2] = r_u32(0x8007669C);
    for (index = 0; index < 3; ++index) {
        pairs[index * 2] = (sint16)words[index];
        pairs[index * 2 + 1] = (sint16)(words[index] >> 16);
    }
    tail = r_u32(0x800766A0);
    increments[0] = (sint16)tail;
    increments[1] = (sint16)(tail >> 16);
    increments[2] = r_s16(0x800766A4);
    callback = r_u32(object + 0x4C);
    target = r_u32(object + 0x178);
    psx_dispatch_guest_call(callback, 1, object, 0, 0);
    choice = random_between(0, 1000);
    state = r_u32(object + 0x22C);
    current = r_u32(state);
    w_u32(0x8011F130, (uint32)choice);
    if ((sint32)current <= 0) {
        if (choice >= 751 && r_s16(state + 8) == 0) {
            w_u32(object + 0x88, 666);
            w_u32(object + 0xB4, 10);
            w_u32(object + 0xB8, 0);
            choice = random_signed_between(50, 120);
            w_u32(object + 0xBC, (uint32)choice);
            if (choice < 0)
                w_u32(object + 0xB4, 0u - r_u32(object + 0xB4));
        } else {
            w_u32(object + 0x88, 667);
        }
        choice = random_between(10, 30);
        state = r_u32(object + 0x22C);
        w_u32(state, (uint32)choice);
    } else {
        w_u32(state, current - 1);
    }
    mode = r_s16(object + 0x262);
    value = r_u32(0x80033D1C);
    w_u16(object + (mode == 0 ? 0x254 : 0x250), value);
    value = r_u32(0x80033D30);
    current = r_u32(object + 0x190);
    w_u16(object + 0x252, value);
    if (current != 0) goto speed_ready;
    distance = r_s32(0x80046174);
    if (distance > 40000) goto maximum_speed;
    if (distance >= 21001) {
        kind = r_u32(object + 4);
        w_u32(object + 0xE8, r_u32(0x8005369C + kind * 60));
        goto speed_ready;
    }
    distance = r_s32(0x80052E6C);
    if (distance >= 801) {
        kind = r_u32(object + 4);
        limit = (uint32)(r_s32(0x8005369C + kind * 60) >> 2);
        current = r_u32(object + 0xE8);
        limit *= 3;
        if ((sint32)current < (sint32)limit)
            w_u32(object + 0xE8, limit);
        kind = r_u32(object + 4);
        mode = r_s16(object + 0x262);
        value = r_u16(0x800536B4 + kind * 60);
        w_u16(object + 0x260, value + (uint16)increments[mode]);
        goto speed_ready;
    }
    if (distance < 240) {
        w_u32(object + 0xE8, r_u32(target + 0xE8));
        goto speed_ready;
    }
maximum_speed:
    kind = r_u32(object + 4);
    w_u32(object + 0xE8, r_u32(0x800536A0 + kind * 60));
speed_ready:
    kind = r_u32(object + 4);
    limit = (uint32)(r_s32(0x8005369C + kind * 60) >> 1);
    current = r_u32(object + 0xE8);
    if ((sint32)current < (sint32)limit)
        w_u32(object + 0xE8, limit);
    game_update_object_state(object);
    game_update_object_movement(object);
    if (r_u32(object + 0x88) == 666) {
        state = r_u32(object + 0x22C);
        current = r_u32(state);
        if ((sint32)current < 5) {
            value = (uint32)game_reduce_without_sign_change(r_s32(object + 0xB8), 10);
            w_u32(object + 0xB8, value);
        } else {
            current = r_u32(object + 0xB8);
            limit = r_u32(object + 0xBC);
            value = (sint32)current < 0 ? 0u - current : current;
            limit = (sint32)limit < 0 ? 0u - limit : limit;
            if ((sint32)value < (sint32)limit)
                w_u32(object + 0xB8, current + r_u32(object + 0xB4));
        }
        value = r_u32(object + 0x28);
        w_u32(object + 0x28, value + r_u32(object + 0xB8));
    }
    state = r_u32(object + 0x22C);
    current = (uint32)(sint32)r_s16(state + 8);
    if (current == 0) {
        choice = r_s32(0x8011F130);
        if (choice >= 14) {
            if (r_s32(0x80052E6C) < 1681) goto maneuver_selection;
            if (r_s32(0x80033D24) >= 280) goto maneuver_selection;
            if (r_s32(0x80053674) >= 280) goto maneuver_selection;
            if (choice >= 25) goto maneuver_selection;
        }
        choice = random_between(30, 90);
        state = r_u32(object + 0x22C);
        w_u16(state + 8, (uint32)choice);
        mode = r_s16(object + 0x262);
    } else {
        w_u16(state + 8, current - 1);
        value = r_u32(object + 0x20);
        current = r_u32(object + 0xAC);
        limit = r_u32(object + 0xB0);
        w_u32(object + 0x20, value + current);
        value = r_u32(object + 0x24);
        current = r_u32(object + 0xA8) - 1;
        w_u32(object + 0x24, value + limit);
        w_u32(object + 0xA8, current);
        if ((sint32)current > 0) goto maneuver_selection;
        mode = r_s16(object + 0x262);
    }
    w_u32(object + 0xA8, 12);
    choice = random_signed_between(pairs[mode * 2], pairs[mode * 2 + 1]);
    mode = r_s16(object + 0x262);
    w_u32(object + 0xAC, (uint32)choice);
    choice = random_signed_between(pairs[mode * 2], pairs[mode * 2 + 1]);
    w_u32(object + 0xB0, (uint32)choice);
maneuver_selection:
    if (r_u32(object + 0x88) == 666) return;
    choice = random_between(0, 100);
    state = r_u32(object + 0x22C);
    current = (uint32)(sint32)r_s16(state + 14);
    w_u32(0x8011F130, (uint32)choice);
    if ((sint32)current < -75 && choice < 15) {
        w_u16(state + 14, 0);
        kind = r_u32(object + 4);
        current = r_u32(0x800536A0 + kind * 60);
        w_u32(object + 0x88, 4);
        w_u32(object + 0xE8, current);
        ticks = (uint32)random_between(60, 150);
        goto start_maneuver;
    }
    state = r_u32(object + 0x22C);
    if (r_s16(state + 12) >= 301 && r_s32(0x80033D24) >= 1601) {
        value = 9;
        goto immediate_maneuver;
    }
    if (r_s32(0x80052E6C) >= 1681 &&
        r_s32(0x80053674) < 240 &&
        r_s32(0x80033D24) >= 1681 &&
        r_s32(0x80046174) >= 26001) {
        if (r_s32(0x8011F130) >= 50) return;
        value = 5;
        goto immediate_maneuver;
    }
    if ((r_u32(object + 0x114) & 0x100000) != 0 &&
        r_s32(0x80052E6C) < 176 && r_s32(0x80053674) < 176) {
        w_u32(object + 0x88, r_s32(0x8011F130) < 40 ? 6 : 1);
        ticks = (uint32)random_between(20, 60);
        goto start_maneuver;
    }
    if (r_s32(0x80052E6C) < 1601) return;
    if (r_s32(0x80053674) >= 160) return;
    if (r_s32(0x80033D24) >= 120) return;
    mode = r_s16(object + 0x262);
    if (mode == 2 && r_s32(0x8011F130) < 8) {
        value = 5;
        goto immediate_maneuver;
    }
    if (mode != 1) return;
    if (r_s32(0x8011F130) >= 4) return;
    value = 5;
immediate_maneuver:
    w_u32(object + 0x88, value);
    ticks = 0;
start_maneuver:
    game_start_ai_mode_maneuver(object, 0x800DA640, ticks);
}

/* Apply the approved cached short-name policy */
static const char *game_planet_argument_text(uint32 text)
{
    uint32 length = 0;
    while (r_u8(text + length) != 0)
        ++length;
    return (const char *)psx_addr(text, (size_t)length + 1);
}

static sint32 game_planet_argument_integer(uint32 text)
{
    return string_to_integer(game_planet_argument_text(text));
}

static sint32 game_planet_divide(sint32 numerator, sint32 denominator)
{
    if (denominator == 0)
        trap(7);
    if (denominator == -1 && numerator == (sint32)0x80000000u)
        trap(0x5D);
    return numerator / denominator;
}

void game_config_create_planet(const uint32 *arguments, uint32 first)
{
    char name[64];
    uint32 count, sun = 0, view, offset, value, axis, model, length;
    sint32 x, y, z, divisor, size, normalized;
    FUNCTION_MARKER(0x80095428, "GAME.OVL");
    count = r_u32(0x800595C0);
    if ((sint32)count >= 7)
        diagnostic_report(count, 0x80075174, 0x80074F80);
    strcpy(name, game_planet_argument_text(arguments[first + 1]));
    if (strcmp(name, "SUNA") == 0)
        sun = 1;
    else if (strcmp(name, "SUNB") == 0)
        sun = 2;
    else if (strcmp(name, "SUNC") == 0)
        sun = 3;
    if (sun != 0) {
        view = 0x80123F78 + (uint32)(sint32)r_s16(0x800595F0) * 28;
        w_u16(view + 24, sun);
        value = (uint32)game_planet_argument_integer(arguments[first + 8]);
        w_u16(view + 26, value);
        value = (uint32)game_planet_argument_integer(arguments[first + 2]);
        w_u32(view + 8, value);
        value = (uint32)game_planet_argument_integer(arguments[first + 3]);
        w_u32(view + 12, value);
        value = (uint32)game_planet_argument_integer(arguments[first + 4]);
        w_u32(view + 16, value);
        x = game_planet_argument_integer(arguments[first + 2]) >> 8;
        y = game_planet_argument_integer(arguments[first + 3]) >> 8;
        z = game_planet_argument_integer(arguments[first + 4]) >> 8;
        divisor = vector_length_approx(x, y, z);
        if (divisor == 0)
            divisor = 1;
        value = 120u << ((uint32)(sint32)r_s16(view + 26) & 31);
        size = game_planet_divide((sint32)value, divisor);
        if (size < 8)
            size = 8;
        if (size >= 32001)
            size = 32000;
        if (size >= 151)
            size = 151;
        else if (size >= 121)
            size = 121;
        else if (size >= 81)
            size = 80;
        w_u16(view + 26, (uint32)size);
        if (size >= 80) {
            sun = (uint32)(sint32)r_s16(view + 24);
            value = (uint32)(sint32)r_s16(view + 26);
            w_u32(0x800F15EC, sun);
            w_u32(0x800F15F0, value);
            w_u32(0x801193FC, (uint32)x);
            w_u32(0x80119400, (uint32)y);
            w_u32(0x80119404, (uint32)z);
        }
        normalized = game_planet_divide((sint32)((uint32)x << 12), divisor);
        w_u16(view, (uint32)normalized);
        normalized = game_planet_divide((sint32)((uint32)y << 12), divisor);
        w_u16(view + 2, (uint32)normalized);
        normalized = game_planet_divide((sint32)((uint32)z << 12), divisor);
        value = r_u32(view + 8);
        w_u16(view + 4, (uint32)normalized);
        if (value == 0 && r_u32(view + 12) == 0 && r_u32(view + 16) == 0) {
            w_u32(view + 8, 0xFFFFF001);
            w_u16(view, 0xFFFFF001);
        }
        value = r_u16(0x800595F0);
        w_u16(0x800595F0, value + 1);
        return;
    }
    if (strcmp(name, "MARS") == 0)
        w_u32(0x800ED70C, r_u32(0x800595C0));
    if (strcmp(name, "ROCK") == 0)
        w_u32(0x800ED710, r_u32(0x800595C0));
    if (strcmp(name, "IRIS") == 0)
        w_u32(0x800ED714, r_u32(0x800595C0));
    model = game_find_model_name(arguments[first + 1]);
    if (model == 0) {
        strcat(name, ".BND");
        game_load_bound_bundle_native(name);
    }
    model = game_find_model_name(arguments[first + 1]);
    offset = r_u32(0x800595C0) * 36;
    w_u32(0x801076E8 + offset, model);
    for (axis = 0; axis < 3; ++axis) {
        value = (uint32)game_planet_argument_integer(arguments[first + 2 + axis]);
        offset = r_u32(0x800595C0) * 36;
        w_u32(0x801076EC + offset + axis * 4, value);
    }
    for (axis = 0; axis < 3; ++axis) {
        value = (uint32)game_planet_argument_integer(arguments[first + 5 + axis]);
        offset = r_u32(0x800595C0) * 36;
        w_u16(0x801076FC + offset + axis * 2, value << 3);
    }
    for (axis = 0; axis < 3; ++axis) {
        value = (uint32)game_planet_argument_integer(arguments[first + 8 + axis]);
        offset = r_u32(0x800595C0) * 36;
        w_u16(0x80107702 + offset + axis * 2, value);
    }
    length = (uint32)strlen(name);
    w_u32(0x8011F130, length);
    /* Use the first-load character rule for cached short names */
    if (name[(sint32)length - (length < 5 ? 1 : 5)] == 'M') {
        offset = r_u32(0x800595C0) * 36;
        w_u16(0x80107708 + offset, 0);
    } else {
        offset = r_u32(0x800595C0) * 36;
        w_u16(0x80107708 + offset, 1);
    }
    value = r_u32(0x800595C0);
    w_u32(0x800595C0, value + 1);
}

/* Omit the three dead SUB traps with explicit user approval */
static uint32 game_patch_add_checked(uint32 first, uint32 second, uint32 pc)
{
    sint64 value = (sint64)(sint32)first + (sint32)second;
    if (value < (-2147483647 - 1) || value > 2147483647)
        xport_mips_overflow_exception(pc);
    return first + second;
}

static uint32 game_patch_subtract_checked(uint32 first, uint32 second, uint32 pc)
{
    sint64 value = (sint64)(sint32)first - (sint32)second;
    if (value < (-2147483647 - 1) || value > 2147483647)
        xport_mips_overflow_exception(pc);
    return first - second;
}

static uint32 game_patch_component(uint32 descriptor, uint32 camera, uint32 axis, const uint32 *weights)
{
    static const uint32 delta_pc[3] = {0x800CF6DC, 0x800CF73C, 0x800CF790};
    static const uint32 camera_pc[3] = {0x800CF70C, 0x800CF760, 0x800CF7B4};
    static const uint32 add_pc[3] = {0x800CF71C, 0x800CF770, 0x800CF7C4};
    uint32 origin, first, second, third, fourth, offset = axis * 4;
    uint32 product_a, product_b, product_c, result;
    if (axis == 0) {
        origin = r_u32(camera + 8);
        first = r_u32(descriptor);
    } else {
        first = r_u32(descriptor + offset);
        origin = r_u32(camera + 8 + offset);
    }
    second = r_u32(descriptor + 16 + offset);
    third = r_u32(descriptor + 32 + offset);
    fourth = r_u32(descriptor + 48 + offset);
    second = game_patch_subtract_checked(second, first, delta_pc[axis]);
    third = game_patch_subtract_checked(third, first, delta_pc[axis] + 4);
    fourth = game_patch_subtract_checked(fourth, first, delta_pc[axis] + 8);
    /* Approved omission is only CF6E8, CF6EC and CF6F0 for axis zero */
    product_a = second * weights[0];
    product_b = third * weights[1];
    product_c = fourth * weights[2];
    result = game_patch_subtract_checked(first, origin, camera_pc[axis]);
    result = game_patch_add_checked(result, (uint32)((sint32)product_a >> 8), add_pc[axis]);
    result = game_patch_add_checked(result, (uint32)((sint32)product_b >> 8), add_pc[axis] + 4);
    return game_patch_add_checked(result, (uint32)((sint32)product_c >> 8), add_pc[axis] + 8);
}

void game_render_interpolated_patch(uint32 descriptor, uint32 camera)
{
    uint32 sample, weights[3], x, y, z, screen, leading, depth;
    uint32 screen_pointer = 0x80033D50, depth_pointer = 0x800510F8;
    uint32 leading_pointer = 0x800F2AC8, packet, table, bucket, difference;
    uint32 first_x, first_y, second_x, second_y;
    sint32 region_x, region_y, region_z;
    FUNCTION_MARKER(0x800CF648, "GAME.OVL");
    for (sample = 0; sample < 16; ++sample) {
        weights[0] = r_u32(0x800F8108 + sample * 4);
        weights[1] = r_u32(0x800F8150 + sample * 4);
        weights[2] = r_u32(0x800F8190 + sample * 4);
        x = game_patch_component(descriptor, camera, 0, weights);
        y = game_patch_component(descriptor, camera, 1, weights);
        z = game_patch_component(descriptor, camera, 2, weights);
        x = (uint32)((sint32)x >> 2);
        y = (uint32)((sint32)y >> 2);
        z = (uint32)((sint32)z >> 2);
        region_x = (((sint32)x >> 15) + 1) >> 1;
        region_y = (((sint32)y >> 15) + 1) >> 1;
        region_z = (((sint32)z >> 15) + 1) >> 1;
        if (region_x + region_y + region_z != 0)
            return;
        xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
        xport_gte_write_data(1, z);
        xport_gte_execute(0x180001);
        screen = xport_gte_read_data(14);
        leading = xport_gte_read_data(31);
        depth = xport_gte_read_data(19);
        w_u32(leading_pointer, leading);
        w_u32(depth_pointer, depth << 4);
        w_u32(screen_pointer, screen);
        screen_pointer += 4;
        depth_pointer += 4;
        leading_pointer += 4;
    }
    screen_pointer = 0x80033D50;
    depth_pointer = 0x800510F8;
    for (sample = 0; sample < 15; ++sample) {
        first_x = (uint32)(sint32)r_s16(screen_pointer);
        first_y = (uint32)(sint32)r_s16(screen_pointer + 2);
        second_x = (uint32)(sint32)r_s16(screen_pointer + 4);
        second_y = (uint32)(sint32)r_s16(screen_pointer + 6);
        depth = r_u32(depth_pointer);
        packet = r_u32(0x80040690);
        w_u32(packet + 4, 0xFF00);
        w_u32(packet + 12, 0xFF00);
        w_u8(packet + 7, 0x50);
        w_u8(packet + 3, 4);
        w_u16(packet + 8, first_x);
        w_u16(packet + 10, first_y);
        w_u16(packet + 16, second_x);
        w_u16(packet + 18, second_y);
        table = r_u32(0x800421AC);
        if ((sint32)depth >= 0x4000) {
            difference = game_patch_subtract_checked(depth, 0x3FFFFE, 0x800CF910);
            if ((sint32)difference > 0)
                depth = 0x3FFFFE;
            bucket = game_patch_add_checked(depth >> 11, 0x140, 0x800CF928);
            bucket = game_patch_add_checked(bucket << 2, table, 0x800CF930);
        } else {
            if ((sint32)depth < 0)
                depth = 0;
            bucket = game_patch_add_checked(depth >> 6, 0x40, 0x800CF94C);
            bucket = game_patch_add_checked(bucket << 2, table, 0x800CF954);
        }
        AddPrim(psx_addr(bucket, 4), psx_addr(packet, 20));
        packet = r_u32(0x80040690);
        packet = game_patch_add_checked(packet, 20, 0x800CF9A0);
        w_u32(0x80040690, packet);
        screen_pointer += 4;
        depth_pointer += 4;
    }
}

/* Use the approved full-row character budget */
void front_draw_secondary_briefing_text(void)
{
    uint32 layout[6];
    uint32 length = 0, row, count, text, extra;
    FUNCTION_MARKER(0x800ADACC, "FRONT.OVL");
    w_u32(0x800FB870, 0xFD);
    while (r_u8(0x800BD42C + length) != 0)
        ++length;
    front_draw_colored_text_native((const char *)psx_addr(0x800BD42C, (size_t)length + 1), 46, 64, 0x808080);
    front_reset_text_emphasis();
    row = 1;
    w_u32(0x800FB870, 0x53);
    count = r_u32(0x800BD400);
    layout[0] = 46;
    layout[1] = 90;
    layout[2] = 0x505060;
    text = 0x800BD458;
    extra = 0x800BDCC8;
    while ((sint32)row < (sint32)count) {
        if (layout[1] - 90 < 110) {
            layout[3] = r_u32(extra);
            layout[4] = r_u32(0x800BDD8C + (row << 2));
            layout[5] = 38;
            front_draw_emphasized_text_native(text, layout);
        }
        text += 44;
        ++row;
        layout[1] += 16;
        extra += 4;
    }
}

/* Keep the game descriptor as guest addresses while xport parses TIM */
sint32 texture_decode_tim_native(uint32 source, TEX_INFO *info)
{
    GpuTimImage decoded;
    size_t available = PSX_DRAM_SIZE - (source & (PSX_DRAM_SIZE - 1));
    const uint8 *bytes = (const uint8 *)psx_addr(source, available);
    if (!gpu_decode_tim_body(bytes, available, &decoded))
        return 0;
    info->mode = decoded.mode;
    if (decoded.mode & 8) {
        info->clut = decoded.clut;
        info->clut_data = source + (uint32)(decoded.clut_data - bytes);
    }
    info->pixel = decoded.pixels;
    info->pixel_data = source + (uint32)(decoded.pixel_data - bytes);
    return 1;
}

void game_dispatch_object_hit(uint32 target, uint32 object, uint32 position, uint32 vector, uint32 other, uint32 projectile, uint32 kind, uint32 profile)
{
    switch (target)
    {
    case 0x8002493C:
        object_noop_second();
        return;
    case 0x80079E5C:
        game_handle_player_projectile_hit(object, position, vector, other, projectile, kind, profile);
        return;
    case 0x8009FA2C:
        game_handle_ship_hit_native(object, (VECTOR *)psx_addr(position, 12), (VECTOR *)psx_addr(vector, 12), other, projectile, kind, profile);
        return;
    case 0x800BF5FC:
        game_handle_debris_hit(object, position, vector, other);
        return;
    case 0x800D8BE4:
        game_handle_attached_model_hit_native(object, (VECTOR *)psx_addr(position, 12), (VECTOR *)psx_addr(vector, 12), other, projectile, kind, profile);
        return;
    case 0x800E56CC:
        game_emit_ring_fragments(object, position, vector);
        return;
    default:
#ifndef NDEBUG
        fprintf(stderr, "Unbound object-hit callback %08X\n", target);
#endif
        abort();
    }
}
