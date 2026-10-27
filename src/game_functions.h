#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "xport.h"
#include "psx.h"
#include "psx_spu.h"

/* Guest addresses remain integers at game boundaries */
void front_build_model_bounds(uint32 model, uint32 bounds);
uint32 front_create_model_object(sint32 model_index, uint32 x, uint32 y, uint32 z, uint32 type);
uint32 front_init_model_lighting(void);
uint32 front_init_model_preview(void);
uint32 front_start_model_preview(void);
void front_draw_preview_model(uint32 object);
uint32 front_rotate_preview_model(uint32 object);
uint32 front_update_preview_camera(uint32 object);
uint32 front_reset_menu_packet_lists(void);
uint32 front_allocate_cached_text(uint32 packet_capacity, uint32 text_capacity);
uint32 front_create_cached_text(uint32 text, uint32 x, uint32 y, uint32 color);
void front_load_preview_configuration(void);
uint32 front_initialize_ship_view(void);
uint32 front_select_preview_configuration(void);
uint32 front_allocate_model_tables(void);
uint32 front_copy_view_arguments(uint32 destination, const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_begin_ship(void);
uint32 front_view_end_ship(void);
uint32 front_view_set_bundle(const uint32 *arguments, uint32 first);
uint32 front_view_set_position(const uint32 *arguments, uint32 first);
uint32 front_view_set_rotation(const uint32 *arguments, uint32 first);
uint32 front_view_set_length(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_parse_view_configuration(uint32 source, uint32 size);
uint32 front_load_preview_models(uint32 slot);
uint32 front_copy_tmd(uint32 source, uint32 destination, sint32 size);
uint32 front_parse_model_bundle(uint32 source, uint32 first_model);
uint32 front_load_model_bundle(const char *name);
uint32 front_load_slot_bundle(uint32 slot);
void front_viewer_error(void);
void front_apply_object_rotation(uint32 object);
uint32 front_update_viewer_objects(void);
uint32 front_draw_viewer_objects(void);
uint32 front_draw_selected_mesh(void);
uint32 front_draw_cached_text(uint32 identifier);
void front_draw_text_native(const char *text, uint32 x, uint32 y, uint32 color, sint32 blend);
void front_draw_wide_text_native(const char *text, uint32 x, uint32 y, uint32 color);
void front_draw_ship_statistics(void);
void front_draw_ship_details(void);
void front_draw_colored_text_native(const char *text, uint32 x, uint32 y, uint32 color);
void front_draw_distance_scales(void);
void front_draw_viewer_backgrounds(void);
void front_update_ship_view(void);
void front_update_ship_comparison(void);
void front_free_preview_models(void);
void front_select_ship_view(void);
void front_select_ship_comparison(void);
uint32 front_request_next_ship(void);
uint32 front_request_previous_ship(void);
void front_update_ship_screen(void);
void front_draw_viewer_controls(void);
void front_draw_previous_ship_arrow(uint32 x, uint32 y, uint32 active);
void front_draw_next_ship_arrow(uint32 x, uint32 y, uint32 active);
void front_free_cached_text(void);
uint32 front_close_ship_screen(void);
void front_draw_control_segment(uint32 x, uint32 y, uint32 angle, uint32 trail);
uint32 front_wide_text_width(uint32 text);
void front_draw_single_page_text_native(const char *text, uint32 x, uint32 y, uint32 color);
void front_scroll_text_left(uint32 panel);
void front_scroll_text_right(uint32 panel);
void front_scroll_text_reverse(uint32 panel);
void front_reset_text_emphasis(void);
uint32 front_text_width_without_brackets(uint32 text);
uint32 front_scan_text_emphasis(uint32 text);
void front_draw_justified_text(uint32 text, uint32 layout);
uint32 front_draw_emphasized_text_native(uint32 text, uint32 *layout);
uint32 front_draw_emphasized_text_clip_top_native(uint32 text, uint32 *layout, sint32 trim);
uint32 front_draw_emphasized_text_clip_bottom_native(uint32 text, uint32 *layout, sint32 height);
uint32 front_draw_emphasized_text(uint32 text, uint32 layout);
uint32 front_draw_emphasized_text_clip_top(uint32 text, uint32 layout, sint32 trim);
uint32 front_draw_emphasized_text_clip_bottom(uint32 text, uint32 layout, sint32 height);
void front_parse_campaign_filename(uint32 text);
void front_select_initial_campaign_file(void);
void front_return_without_effects(void);
void front_texture_cache_idle(void);
void front_initialize_text_texture_cache(uint32 x, uint32 y, sint32 row_height);
void front_render_text_texture_cache(void);
void front_cache_text_texture(uint32 record);
void front_screen_slide_idle(void);
uint32 front_wait_event_bank_a(void);
uint32 front_wait_event_bank_a_secondary(void);
uint32 front_wait_event_bank_b(void);
uint32 front_poll_event_bank_b(void);
uint32 front_poll_event_bank_a(void);
uint32 front_query_card_state(uint32 port);
uint32 front_wait_card_check_event(void);
uint32 front_request_card_load(uint32 port);
uint32 front_refresh_card_status(uint32 port);
uint32 front_erase_card(sint32 channel);
void front_encode_card_title(const char *text, uint8 *encoded);
void front_build_save_header(uint8 *header, char *name, uint32 blocks);
uint32 front_read_card_directory(uint32 port);
uint32 front_create_card_file(const char *path, uint32 blocks);
uint32 front_write_card_file(const uint8 *header, const char *path, const uint8 *data);
uint32 front_read_card_file(uint32 port, uint32 slot, uint8 *data, sint32 capacity);
uint32 front_write_card_blocks(uint32 port, const char *name, const uint8 *data, uint32 length);
uint32 front_snapshot_basic_settings(uint32 destination);
uint32 front_prepare_card_monitor(void);
uint32 front_update_card_monitor(void);
void front_run_card_monitor(uint32 enabled);
void front_apply_display_origin(void);
void front_restore_settings(uint32 settings);
uint32 front_store_vertical_adjustment(void);
void front_append_card_draw_mode(void);
void front_draw_card_icon(uint32 port, uint32 slot, uint32 x, uint32 y, uint32 color, uint32 frame);
void front_draw_card_icon_large(uint32 port, uint32 slot, uint32 x, uint32 y, uint32 color, uint32 frame);
void front_draw_card_entry(uint32 port, uint32 slot, uint32 x, uint32 y);
void front_draw_card_directory(void);
void front_draw_card_menu(void);
void front_update_card_selection(void);
void front_initialize_load_selection(void);
uint32 front_request_card_setup(void);
uint32 front_initialize_character_picker(uint32 texture, const PSX_RECT *rectangle, uint32 characters);
uint32 front_initialize_save_menu(void);
uint32 front_load_selected_settings(uint32 destination);
void front_update_load_confirmation(void);
void front_draw_text_entry_frame(uint32 texture, const PSX_RECT *rectangle, uint32 color);
void front_format_selected_card(void);
uint32 front_update_character_picker(void);
void front_update_save_menu(void);
void front_initialize_card_operation(void);
void front_update_card_copy_menu(void);
uint32 front_copy_selected_card_file(void);
void front_reset_card_action(void);
void front_update_card_selection_for_copy(void);
void front_update_card_selection_for_browse(void);
void front_commit_loaded_settings(void);
sint32 front_erase_selected_card_file(void);
void front_update_card_delete_menu(void);
uint32 front_erase_selected_card_media(void);
uint32 front_probe_selected_card_media(void);
/* Pending original sector probe with unresolved buffer bytes */
uint32 front_probe_card_sector(sint32 channel);
void front_draw_embossed_line(const sint16 *points, uint32 color, uint32 depth);
void front_assign_control_button_labels(uint32 diagram, uint32 buttons, uint32 action);
void front_assign_control_axis_labels(uint32 diagram, uint32 mode, uint32 index);
void front_prepare_control_diagram(uint32 diagram, uint32 bindings);
void front_draw_control_diagram(uint32 diagram, uint32 bindings);
void front_apply_control_preset(void);
/* Pending complete save owner at FRONT DD90, including DD98 */
uint32 front_save_named_settings(uint32 name, uint32 port, uint32 data);
/* Pending complete display-mode bodies at FRONT A6F04 and A729C */
void front_select_pal_display(void);
void front_select_ntsc_display(void);
uint32 front_view_set_type(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_name(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_role(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_allegiance(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_cruise_speed(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_drive_speed(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_acceleration(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_turn_rate(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_hull(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_shield(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_sensor(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_hardpoints(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_weapons_grids(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_weapons_bays(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_primary_1(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_primary_2(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_primary_3(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_primary_4(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_primary_5(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_primary_6(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_secondary_1(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_secondary_2(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_secondary_3(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_secondary_4(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_secondary_5(const uint32 *arguments, uint32 first, sint32 count);
uint32 front_view_set_secondary_6(const uint32 *arguments, uint32 first, sint32 count);

typedef struct TEX_INFO
{
    uint32 mode;
    PSX_RECT pixel;
    uint32 pixel_data;
    PSX_RECT clut;
    uint32 clut_data;
} TEX_INFO;

uint32 front_relocate_tmd_texture(uint32 model, const TEX_INFO *info, uint32 texture_index);

/* Delegate TIM decoding with guest-address outputs */
sint32 texture_decode_tim_native(uint32 source, TEX_INFO *info);

void texture_load_status(void);
void scene_present_fade(void);
sint32 scene_stop_loading(void);
void scene_free_resources(void);
void scene_suspend_loading(void);
uint32 scene_resume_loading(void);
sint32 message_init(void);
sint32 message_add(char *text, uint32 style, uint32 duration);
sint32 palette_save_all(void);
sint32 palette_register(uint32 code);
uint32 palette_find_remap(uint32 code);
sint32 mesh_find_face_pair(uint32 faces, sint32 count, uint32 first, uint32 second, sint32 skip);
sint32 mesh_build_adjacency(uint32 mesh);
uint32 mesh_import_primitives(uint32 source);
sint32 vector_length_approx(sint32 x, sint32 y, sint32 z);
sint32 triangle_max_edge(uint32 first, uint32 second, uint32 third);
sint32 quad_max_edge(uint32 first, uint32 second, uint32 third, uint32 fourth);
/* Pending shared SDK API, no game-local implementation */
void mesh_render_fixed(uint32 mesh);
void mesh_render_fixed_alternate(uint32 mesh);
uint32 mesh_render_adaptive(uint32 mesh, uint32 mode);
uint32 mesh_render_adaptive_alternate(uint32 mesh, uint32 mode);
void mesh_render_adaptive_single(uint32 mesh);
void render_emit_mesh_lit(uint32 mesh);
void render_subdivide_triangle_medium(uint32 mesh, uint32 face);
void render_subdivide_triangle_large(uint32 mesh, uint32 face);
void render_subdivide_quad_medium(uint32 mesh, uint32 face);
void render_subdivide_quad_large(uint32 mesh, uint32 face);
void render_emit_mesh_overlay(uint32 mesh);
uint32 render_emit_mesh_wire(uint32 mesh, uint32 color);
uint32 render_emit_mesh_wire_front(uint32 mesh, uint32 color);
void render_emit_mesh_depth(uint32 mesh);
void object_draw_mesh(uint32 object);
sint32 mesh_rebuild_normals(uint32 mesh);
sint32 vector_normalize(VECTOR *source, VECTOR *destination);
void texture_load_page(uint32 page, const char *name);
sint32 texture_read_info(uint32 source, TEX_INFO *info);
sint32 texture_upload(uint32 source, TEX_INFO *info, uint32 mode);
sint32 heap_init(void);
uint32 heap_free_bytes(void);
uint32 heap_alloc_front(uint32 size);
uint32 heap_alloc_back(uint32 size);
void heap_free(uint32 address);
void heap_report_failure(uint32 size);
void diagnostic_report(uint32 code, intptr message, intptr detail);
uint32 data_swap_endian(uint32 value);
sint32 sound_set_mode(sint32 mode);
sint32 sound_resume(void);
uint32 sound_set_master(uint32 volume);
sint32 sound_enable_sample(uint32 index);
void sound_set_level(uint32 level);
sint32 resource_clear_flags(void);
uint32 game_init_defaults(void);
sint32 game_init_services(void);
uint32 resource_load_file(const char *name);
uint32 disc_load_file(const char *name);
uint32 disc_load_named_file_back(const char *name);
sint32 disc_check_lid(void);
sint32 sound_stop_all(void);
sint32 disc_read_sectors(const CdlLOC *position, uint32 sectors, uint32 destination);
sint32 disc_read_error(void);
sint32 disc_prompt(void);
sint32 game_validate_data_disc(void);
sint32 game_extracted_file_size(const char *name, uint32 *length);
sint32 game_mount_stream_source(void);
sint32 disc_wait_valid(void);
sint32 disc_event_fatal(void);
void disc_init_events(void);
uint32 disc_toc_raw_ticks(uint32 position);
sint32 disc_init_directory(sint32 toc_only);
sint32 disc_find_directory(const char *name, uint32 parent, uint32 *directory_number, uint32 *file_table);
const char *disc_next_component(const char *path);
sint32 disc_find_file(const char *name, uint32 *sector_size);
uint32 path_uppercase_ascii(char *path);
sint32 disc_file_seek_step(uint32 handle, uint32 offset);
sint32 disc_file_seek_span(uint32 handle, uint32 sectors);
uint32 disc_load_file_back(const char *name);
void disc_set_cd_volume(sint16 volume);
char *path_build_data_filename(char *destination, const char *name);
sint32 disc_file_exists(const char *name);
uint32 render_select_buffer(uint32 entries);
void render_draw_complete(void);
void render_check_lists(void);
uint32 render_draw_centered_sprite(uint32 index, uint32 x, uint32 y);
uint32 render_draw_line(uint32 x0, uint32 y0, uint32 x1, uint32 y1);
uint32 render_draw_sprite(uint32 x, uint32 y, uint32 texture, uint32 palette, uint32 flags);
void disc_set_event_result(uint32 result);
uint32 disc_poll_event_result(void);
uint32 disc_sector_position(uint32 sector, CdlLOC *position);
sint32 disc_read_pair(uint32 sector, uint32 destination);
uint32 disc_count_files(uint32 sector, uint32 buffer);
uint32 disc_build_file_table(uint32 sector, uint32 buffer);
uint32 disc_build_directory_tables(uint32 records, uint32 buffer);
void resource_init_index(void);
void overlay_check_heap(void);
sint32 overlay_run(sint32 (*entry)(void), uint32 allocation);
uint32 overlay_load(const char *name, sint32 load_scene);
sint32 level_load_callback(sint32 mode);
char *resource_make_path(char *destination, const char *name);
sint32 pad_init_config(void);
sint32 sound_reset_voices(void);
sint32 sound_load_samples(uint32 buffer);
uint32 resource_load_into(const char *name, uint32 destination);
/* Pending native-buffer ABI adapter for the same resource loader */
uint32 resource_load_into_native(const char *name, void *destination, size_t capacity);
uint32 vector_pack_xy(uint32 vector);
uint32 mesh_extract_fragment(sint32 selected_count, uint32 selected_faces, uint32 source_mesh);
uint32 mesh_build_bounds(uint32 mesh);
sint32 mesh_load_fragments(uint32 chunk, uint32 output, uint32 source_mesh);
sint32 sound_transfer_vag(uint32 data, uint32 second_data);
sint32 sound_set_voice_volume(uint32 voice, uint32 volume);
sint32 sound_update_voice(uint32 voice);
sint32 sound_update_voice_mode1(uint32 voice);
sint32 sound_update_voice_mode2(uint32 voice);
sint32 sound_update_voice_mode3(uint32 voice);
sint32 sound_update_voice_mode4(uint32 voice);
uint32 sound_find_voice(void);
sint32 input_append_rec(uint32 value);
sint32 pad_scale_axis(sint32 center, sint32 dead_zone, sint32 limit, uint32 sample);
sint32 sound_play_sample(uint32 sample);
sint32 sound_play_sample_pitch(uint32 sample, uint32 pitch);
sint32 sound_start_voice(sint32 sample, uint32 volume, sint32 left, sint32 right, uint32 pitch, uint32 flags);
sint32 table_clear_column(uint32 column);
sint32 table_fill_rows(uint32 first, sint32 end, uint32 value, uint32 column);
sint32 table_max_rows(uint32 first, sint32 count, uint32 column);
sint32 table_find_rows(sint32 count, sint32 height, sint32 column);
uint32 stream_stop(void);
sint32 disc_wait_stream(void);
sint32 stream_init(void);
sint32 game_init_options(void);
uint32 render_alloc_buffers(void);
void render_free_buffers(void);
sint32 font_init(void);
sint32 matrix_set_identity(uint32 matrix);
sint32 matrix_set_identity_native(MATRIX *matrix);
void resource_free_bundle(uint32 bundle);
sint32 render_reset(void);
uint32 resource_file_size(const char *name);
uint32 resource_load_front(const char *name);
uint32 disc_file_size(const char *name);
sint32 video_init_front(void);
uint32 resource_find_entry(const char *name);
sint32 resource_exists(const char *name);
uint32 pad_set_axis(sint32 axis, sint32 value, uint32 pad);
sint32 table_column_empty(uint32 column);
uint32 tex_alloc_region(sint32 width, sint32 height);
sint32 tex_reset_regions(void);
sint32 pad_update(uint32 pad);
sint32 pad_update_replay(uint32 pad);
uint32 input_next_rec(sint32 cancel);
MATRIX *matrix_rotate_x(MATRIX *destination, MATRIX *source, uint32 angle);
MATRIX *matrix_rotate_y(MATRIX *destination, MATRIX *source, uint32 angle);
MATRIX *matrix_rotate_z(MATRIX *destination, MATRIX *source, uint32 angle);
uint32 overlay_load_resource(const char *name, sint32 load_scene);
uint32 resource_read_file(const char *name, uint32 destination);
uint32 render_set_width(uint32 width);
uint32 render_set_clear_color(uint32 red, uint32 green, uint32 blue);
void render_present_frame(void);
uint32 render_swap_buffers(void);
sint32 sound_init_loops(void);
sint32 sound_stop_loops(void);
sint32 sound_stop_voice(uint32 voice);
uint32 model_load_bundle(uint32 name, uint32 type, uint32 flags);
void camera_reset_follow(void);
void camera_set_target(uint32 object);
uint32 camera_set_follow_distance(uint32 distance, uint32 smooth);
uint32 camera_set_position(uint32 vector);
uint32 camera_set_position_native(const VECTOR *vector);
uint32 object_init(uint32 object);
sint32 render_set_semi_trans(sint32 mode);
void render_init_lights(void);
uint32 scene_start_hangar(void);
uint32 scene_start_stars(void);
void gte_save_data(uint32 destination);
void gte_restore_data(uint32 source);
void scene_hangar_frame(void);
void scene_stars_frame(void);
void scene_status_disabled(void);
void scene_hangar_draw(void);
uint32 scene_hangar_update(void);
uint32 camera_enable_flag20(void);
void camera_set_up(uint32 x, uint32 y, uint32 z);
void sound_set_voice_pitch(uint32 voice, uint32 pitch);
uint32 scene_stars_draw(void);
uint32 render_insert_primitive(sint32 depth, uint32 packet);
void scene_stars_update(void);
uint32 render_init_view(uint32 width, uint32 height, uint32 distance);
sint32 sound_reserve_voice(sint32 sample, uint32 volume, sint32 left, sint32 right, uint32 pitch, uint32 flags);
uint32 light_set_color(uint32 light, uint32 red, uint32 green, uint32 blue);
uint32 light_set_direction(uint32 light, uint32 x, uint32 y, uint32 z);

uint32 render_set_blend_mode(uint32 blend, sint32 depth);

sint32 texture_upload_at(uint32 source, sint16 x, sint16 y, sint16 clut_x, sint16 clut_y);

sint32 math_scale_cos(uint32 scale, uint32 angle);

sint32 math_scale_sin(uint32 scale, uint32 angle);

uint32 light_set_angles(sint32 index, uint32 red, uint32 green, uint32 blue, uint32 azimuth, uint32 elevation);

uint32 light_set_direction(uint32 index, uint32 x, uint32 y, uint32 z);

uint32 light_set_color(uint32 index, uint32 red, uint32 green, uint32 blue);

uint32 render_init_buffers(void);

uint32 light_set_scaled_color(uint32 index, uint32 red, uint32 green, uint32 blue);

void render_set_fog_far(sint32 distance, sint32 projection);

uint32 camera_build_axes(VECTOR *forward_input, VECTOR *up_input);

uint32 camera_follow_target(void);
uint32 camera_apply_vertical_scale(void);

uint32 object_list_prepend(uint32 object, uint32 list);

uint32 object_list_remove(uint32 object, uint32 list);

uint32 object_link_chain(uint32 object);

uint32 object_unlink_chain(uint32 object);

uint32 camera_update_modes(void);

uint32 object_allocate(uint32 list);

uint32 object_flush_removed(void);

sint32 sound_init_heap(void);

uint32 sound_init(uint32 mode);

sint32 sound_mute_voices(void);

sint32 sound_restore_voices(void);

void sound_tick(void);

sint32 sound_start_voice_at(sint32 sample, uint32 volume, sint32 left, sint32 right, uint32 pitch, uint32 flags, uint32 voice);

void sound_init_level(uint32 variant, uint32 level);

sint32 sound_offset_to_pan(sint32 offset);

sint32 sound_play_at_listener(uint32 position, uint32 sample, uint32 unused, uint32 listener);

sint32 sound_play_at_camera(uint32 position, uint32 sample, uint32 pitch);

sint32 sound_play_distant_at_camera(uint32 position, uint32 sample, uint32 pitch);

sint32 sound_play_quiet_at_listener(uint32 position, uint32 sample, uint32 unused, uint32 listener);

sint32 sound_play_sample_full(uint32 sample);

uint32 resource_load_sector_index(const char *index_name, const char *data_name);

uint32 stream_clear_queue(void);

sint32 stream_enqueue(uint32 asset, uint32 value, const char *text);

uint32 stream_request(uint32 asset, sint32 priority, uint32 value, const char *text);

uint32 stream_request_segments(uint32 position, uint32 sectors, uint32 asset, uint32 segments, sint32 priority, uint32 value, const char *text);

sint32 disc_draw_notice(uint32 image);

/* Pending shared SDK environment getters */

sint32 disc_set_filter(uint32 channel);
sint32 disc_start_stream(uint32 position, sint32 mode);

sint32 pad_initialize(void);

void object_check_model_address(uint32 object);

sint32 angle_cosine(uint32 angle);
sint32 angle_sine(uint32 angle);
sint32 matrix_from_trigonometry(uint32 matrix, uint32 sx, uint32 cx, uint32 sy, uint32 cy, uint32 sz, uint32 cz);
sint32 matrix_from_angles(uint32 matrix, uint32 transform);

sint32 matrix_shift(uint32 source, uint32 destination, uint32 shift);
sint32 matrix_shift_native(const MATRIX *source, MATRIX *destination, uint32 shift);
uint32 matrix_rotate_x_before(uint32 destination, uint32 source, uint32 angle);
uint32 matrix_rotate_y_before(uint32 destination, uint32 source, uint32 angle);
uint32 matrix_rotate_z_before(uint32 destination, uint32 source, uint32 angle);
uint32 matrix_apply_vector(uint32 destination, uint32 matrix, uint32 vector);

uint32 camera_from_transform(uint32 transform);
uint32 camera_from_direction(uint32 object);

uint32 camera_look_at_object_point(uint32 object);

uint32 matrix_copy_rotation(uint32 destination, uint32 source);
sint32 vector_subtract_native(VECTOR *first, VECTOR *second, VECTOR *output);
sint32 vector_dot_native(VECTOR *first, VECTOR *second);
sint32 vector_magnitude(uint32 vector);
sint32 vector_make_unit(uint32 source, uint32 destination);
sint32 vector_project_native(VECTOR *first, VECTOR *axis, VECTOR *output);
sint32 matrix_orthonormalize(uint32 matrix);

void matrix_normalize(uint32 matrix, uint32 technique);
sint32 angle_atan2(sint32 y, sint32 x);
sint32 object_angles_from_basis(uint32 object, uint32 basis);
uint32 object_camera_relative(uint32 object, uint32 destination, uint32 unused, uint32 reference);
sint32 object_select_model(uint32 object, sint32 distance);

sint32 object_draw_transformed(uint32 object);

sint32 object_basis_from_vectors(VECTOR *forward, VECTOR *up, uint32 object);
sint32 matrix_basis_from_vectors(VECTOR *forward, VECTOR *up, uint32 matrix);
sint32 integer_sign(sint32 value);
sint32 integer_shift_away(sint32 value, uint32 shift);
sint32 random_between(sint32 first, sint32 second);
sint32 square_root_refined(sint32 value);

sint32 random_signed_between(sint32 first, sint32 second);

void font_emit_packets(uint32 ordering_table, const char *text, uint32 position, uint32 palette);
uint32 font_draw(const char *text, uint32 x, uint32 y);
sint32 font_center_x(const char *text);
uint32 font_draw_current(const char *text);
uint32 font_draw_centered(const char *text, uint32 y);
uint32 font_draw_integer(sint32 value);

uint32 font_draw_outline(const char *text, uint32 x, uint32 y, uint32 radius);
uint32 font_draw_centered_outline(const char *text, uint32 y, sint32 radius);

sint32 font_draw_justified(const char *text, uint32 layout);
sint32 font_draw_justified_native(const char *text, uint32 layout[6]);

void mesh_project_vertices(uint32 vertices, uint32 screen, uint32 depth, sint32 count);

sint32 render_test_textured_triangle(uint32 output, uint32 depths, uint32 screen, uint32 face, uint32 short_layout);
uint32 render_emit_gouraud_triangle(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena);
sint32 string_table_find(const char *text, uint32 table);
sint32 string_to_integer(const char *text);
sint32 video_clear_vram(uint32 left, uint32 right);

sint32 system_enter_critical(void);
void system_exit_critical(void);
uint32 system_pad_init(uint32 first, uint32 first_size, uint32 second, uint32 second_size);
uint32 system_pad_start(void);
uint32 system_pad_stop(void);
uint32 system_pad_init_legacy(uint32 mode, uint32 destination);

uint32 system_card_init(uint32 mode);
uint32 system_card_start_bios(void);
uint32 system_card_stop(void);

void system_run_initializers(void);
sint32 system_cd_mix(uint32 volume);
sint32 system_cd_get_sector(void *destination, sint32 words);
sint32 system_cd_position_to_sector(CdlLOC *position);
sint32 system_stream_clear_disabled(void);
sint32 system_read_counter_one(void);
sint32 system_reset_counter_one(void);
void system_set_projection_offset(sint32 x, sint32 y);
void system_set_projection_distance(sint32 distance);
void system_spu_init(void);

void system_spu_quit(void);
uint32 system_spu_write(const void *source, uint32 bytes);
sint32 system_set_video_mode(sint32 mode);
sint32 system_get_video_mode(void);

uint32 render_emit_textured_gouraud_triangle(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 shade_flags, uint32 primitive_code);

sint32 render_test_textured_quad(uint32 output, uint32 depths, uint32 screen, uint32 face, uint32 *fourth_xy, uint32 short_layout);
uint32 render_emit_textured_gouraud_quad(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 shade_flags, uint32 primitive_code, uint32 fourth_xy);
uint32 render_emit_gouraud_quad(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 fourth_xy);

sint32 render_test_flat_quad(uint32 output, uint32 depths, uint32 screen, uint32 face, uint32 *fourth_xy, uint32 short_layout);

sint32 render_test_flat_triangle(uint32 output, uint32 depths, uint32 screen, uint32 face, uint32 short_layout);
uint32 render_emit_textured_flat_triangle(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 shade_flags, uint32 primitive_code);
uint32 render_emit_textured_flat_quad(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 shade_flags, uint32 primitive_code, uint32 fourth_xy);
uint32 render_emit_flat_triangle(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena);
uint32 render_emit_flat_quad(uint32 face, uint32 normals, uint32 ordering_table, uint32 arena, uint32 fourth_xy);
void scene_clear_transition(void);
void system_card_start(void);
sint32 sound_set_voice_left(uint32 voice, uint32 level);
sint32 sound_set_voice_right(uint32 voice, uint32 level);
sint32 sound_select_mode(uint32 mode);
uint32 texture_place_info(TEX_INFO *info, uint32 mode);
uint32 camera_dispatch(void);
uint32 stream_request_position(uint32 position, uint32 sectors, uint32 asset);
sint32 stream_enqueue_segments(uint32 position, uint32 sectors, uint32 asset, uint32 segments, uint32 value, const char *text);
uint32 system_get_odd_field(void);
uint32 render_ellipse_fan(uint32 x, uint32 y, uint32 radius, uint32 color, uint32 depth);
uint32 render_set_full_height(uint32 width);
uint32 render_set_reduced_height(uint32 width);
uint32 light_scale_scene(uint32 scale);
uint32 object_extract_angles(uint32 destination, uint32 object);
uint32 render_submit_frame(void);
uint32 stream_update(void);
uint32 object_update_all(void);
uint32 object_draw_visible(uint32 object);
void object_draw_primitives(uint32 object);
void render_emit_mesh_primitives(uint32 mesh);
sint32 system_set_graph_reverse(sint32 reverse);
sint32 system_set_graph_queue(sint32 enabled);
uint32 system_stop_callbacks(void);
intptr system_vsync_callbacks(sint32 index, PsxInterruptCallback callback);
uint32 system_restart_callbacks(void);
sint32 system_cd_read_break(void);
uint32 system_pad_disable(void);
sint32 system_stream_back_location(CdlLOC *location);
void system_stream_set_mask(uint32 mask, uint32 start, uint32 end);
uint32 game_main_loop(void);
uint32 game_entry(void);
/* Original 8005B8E8 guarded initializer differs from 8005B960 */
/* Original overlay entrypoints outside the frozen main-image selection */
sint32 disc_file_open(char *name);
sint32 sound_free_sample(uint32 sample);
void sound_shutdown(void);
uint32 system_spu_read_blocks(void *destination, uint32 address_units, uint32 blocks);
/* Original SDK patch dependencies pending native owner integration */
void system_card_restore_patch(void);
void card_initialize(uint32 mode);
void card_stop(void);
/* Pending restore-only API in the existing BIOS card owner */

sint32 scene_clear_slot_flags(void);
sint32 vector_average_s16(uint32 destination, uint32 first, uint32 second);
sint32 vector_average_u8_pair(uint32 destination, uint32 first, uint32 second);

uint32 disc_get_toc(void);
uint32 disc_file_size_handle(uint32 handle);
uint32 disc_file_start_sector(uint32 handle);
sint32 disc_name_matches(uint32 name, uint32 candidate);
uint32 disc_file_close(uint32 handle);

void draw_set_color_rgb(uint32 red, uint32 green, uint32 blue);
sint32 draw_set_color_packed(sint32 color);
uint32 camera_set_target_pair(uint32 object, uint32 reference);
uint32 camera_set_offset(uint32 vector, uint32 first_parameter, uint32 second_parameter);
uint32 camera_set_look_at_object_offset(uint32 object, uint32 vector);
uint32 camera_set_look_at_point(uint32 vector);
uint32 camera_set_look_at_object(uint32 object);

uint32 camera_apply_translation(uint32 vector);
uint32 camera_apply_translation_native(const VECTOR *vector);
uint32 camera_request_snap(void);
uint32 object_lookup(uint32 identifier);
uint32 object_initialize_pool(void);
uint32 object_advance_counter_twice(uint32 object);
uint32 object_advance_counter_pair(uint32 object);
void object_noop_first(void);
void object_noop_second(void);
void sound_disable_sample(uint32 index);

uint32 stream_reset_all_queues(void);
uint32 stream_cancel_current(void);
uint32 heap_coalesce_largest(void);
void render_set_near_depth(uint32 depth);
void render_set_far_depth(uint32 depth);
sint32 vector_cross_fixed(uint32 first, uint32 second, uint32 destination);
sint32 vector_multiply_fixed(uint32 first, uint32 second, uint32 destination);
sint32 vector_dot_fixed(uint32 first, uint32 second);
sint32 vector_dot_s16_s32_raw(uint32 first, uint32 second);
sint32 vector_dot_s16_s32_native(uint32 first, const VECTOR *second);

sint32 scalar_abs_wrap(sint32 value);
sint32 vector_length_approx2(sint32 first, sint32 second);
sint32 text_measure_width(uint32 text);
sint32 text_measure_width_without_brackets(uint32 text);
void config_noop(void);
uint32 config_read_line(uint32 source, uint32 destination);
uint32 config_split_arguments(uint32 line, uint32 *arguments);

void front_set_movie_limit(const char *path);
const char *game_movie_resolve_path(const char *path);
uint32 front_play_movie(const char *path);
sint32 front_play_movie_with_sound_reset(const char *path, uint32 parameter);
uint32 front_play_movie_delayed(const char *path);
uint32 front_play_movie_preserve_display(const char *path);

void front_clear_vram_regions(uint32 mode);

uint32 front_initialize_video(void);
void front_decode_still(uint32 *bitstream, const PSX_RECT *input, uint32 format);
uint32 front_reset_settings(void);
uint32 front_snapshot_settings(uint32 destination);
uint32 front_encode_settings_code(uint32 settings);
void front_pack_settings_bits(uint32 settings);
void front_xor_bytes(uint32 bytes, uint32 length, uint32 command);
uint32 front_settings_code_character(uint32 code);
uint32 front_decode_settings_code(const char *code);
uint32 front_settings_code_value(uint32 character);
uint32 front_validate_settings_bits(uint32 bits);
sint32 front_find_named_code(const char *code);
uint32 front_apply_settings_code(void);
uint32 front_open_card_events(void);
uint32 front_close_card_events(void);
uint32 front_initialize_card(void);
uint32 front_shutdown_card(void);
uint32 _bu_init(void);
sint32 front_run(void);
uint32 front_initialize_character_map(void);
uint32 front_reset_code_text(void);
void front_menu_loop(void);
sint32 front_character_glyph(uint32 character);
void front_free_text_panel(uint32 panel);
uint32 front_create_text_panel(const PSX_RECT *rectangle, uint32 style, uint32 color);
void front_intro(void);
void front_initialize_display(void);
uint32 front_initialize_screens(void);
uint32 front_initialize_layers(void);
uint32 front_configure_layer(uint32 layer, uint32 mode, uint32 value);
void front_initialize_render_buffers(uint32 entries, uint32 capacity);
void front_configure_background(uint32 first, uint32 second);
void front_initialize_background(void);
void front_select_screen(sint32 screen);
void front_free_screen_list(uint32 list);
uint32 front_swap_render_buffers(void);
void front_update_screen(void);
void front_render_begin(void);
uint32 front_text_width(uint32 text);
uint32 front_text_width_native(const char *text);
void front_draw_sprite(uint32 texture, uint32 x, uint32 y, uint32 color);
void front_vsync(void);
uint32 front_load_screen_action(sint32 screen);
uint32 front_descriptor_enter(uint32 descriptor);
uint32 front_descriptor_idle(uint32 descriptor);
uint32 front_descriptor_update(uint32 descriptor);
uint32 front_descriptor_leave(uint32 descriptor);
uint32 front_descriptor_skip(uint32 descriptor);
uint32 front_descriptor_strings(uint32 descriptor);
uint32 front_descriptor_actions(uint32 descriptor);
uint32 front_descriptor_sprite(uint32 descriptor);
uint32 front_descriptor_highlight_sprite(uint32 descriptor);
uint32 front_descriptor_string(uint32 descriptor);
uint32 front_descriptor_highlight_string(uint32 descriptor);
uint32 front_descriptor_button(uint32 descriptor);
uint32 front_descriptor_highlight_button(uint32 descriptor);
uint32 front_descriptor_rectangle(uint32 descriptor);
uint32 front_descriptor_highlight_rectangle(uint32 descriptor);
uint32 front_descriptor_begin_pane(uint32 descriptor);
uint32 front_descriptor_end_pane(uint32 descriptor);
uint32 front_descriptor_font(uint32 descriptor);
void front_append_screen_link(uint32 type, uint32 link);
void front_draw_wide_text(uint32 text, uint32 x, uint32 y, uint32 color);
void front_draw_sprite_node(uint32 node);
void front_draw_button_node(uint32 node);
void front_draw_rectangle_node(uint32 node);
void front_draw_string_node(uint32 node);
sint32 front_wide_character_glyph(uint32 character);
void front_draw_text_panel(uint32 panel);
void front_update_connection_label(void);
void front_draw_colored_text(uint32 text, uint32 x, uint32 y, uint32 color);
void front_draw_screen_list(uint32 list);
void front_draw_background(void);
sint32 front_next_screen(void);
void front_play_intro_from_menu(void);
void front_close_display(void);
void front_close_background(void);
void front_close_render_buffers(void);
void front_set_ordering_table_size(uint32 entries);
sint32 front_allocate_render_buffer(uint32 buffer, uint32 capacity);
void front_free_render_buffer(uint32 buffer);
void front_draw_surface_sprite(uint32 sprite);
void front_set_surface_texture_page(void);
uint32 front_screen_bounds(uint32 screen, PSX_RECT *bounds);
uint32 front_allocate_screen_state(uint32 screen);
uint32 front_initialize_screen_slide(uint32 screen);
void front_push_text_line(uint32 panel, uint32 text);
void front_update_screen_slide(uint32 screen);
void front_free_screen_slide(uint32 screen);
uint32 front_initialize_string_fade(uint32 screen);
void front_update_string_fade(uint32 screen);
void front_free_string_fade(uint32 screen);
void front_prepare_card_icons(void);
void front_set_card_texture_layout(uint32 x, uint32 y, uint32 clut_x, uint32 clut_y);
void front_upload_card_icon(uint32 port, uint32 slot);
void front_clear_menu_transition(void);
void front_initialize_glyph_widths(void);
void front_draw_text_clip_top(uint32 text, uint32 x, uint32 y, uint32 color, uint32 trim);
void front_draw_text_clip_bottom(uint32 text, uint32 x, uint32 y, uint32 color, uint32 height);
uint32 front_configure_background_light(uint32 layer, uint32 x, uint32 y, uint32 color, uint32 falloff);
void front_store_screen_action(sint32 screen, sint32 action);
void front_present_still(uint32 *bitstream, uint32 format);
void front_show_startup_image(void);
void front_play_menu_movie(const char *path, uint32 parameter);
sint32 front_prompt_movie_disc(const char *path);
void front_show_still_beea8(void);
void front_show_still_eac00(void);
void front_noop_b838(void);
void front_noop_b840(void);
uint32 front_reset_action_word(void);
uint32 front_find_pane(sint32 action);
uint32 front_update_menu_frame(void);
void front_noop_f37c(void);
uint32 front_leave_menu(void);
sint32 front_list_files(const char *suffix, uint32 *files, sint32 capacity);
uint32 front_replace_text_byte(uint32 text, uint32 from, uint32 to);
void front_uppercase_text(uint32 text);

void front_select_control_viewer_device(void);
uint32 front_update_control_viewer_objects(void);
uint32 front_update_control_viewer_animation(void);
void front_initialize_control_preset_menu(void);
void front_update_control_preset_menu(void);
void front_enable_control_model_draw(uint32 object);
void front_create_control_viewer_objects(void);
uint32 front_prepare_control_model_bundle(const char *name);
void front_initialize_control_viewer(void);
void front_update_control_viewer_menu(void);
void front_release_control_viewer(void);
void front_initialize_control_axis_calibration(void);
void front_update_control_axis_calibration(void);
void front_apply_control_axis_calibration(void);
uint32 front_prepare_control_axis_marker(uint32 marker, uint32 index);
uint32 front_prepare_control_axis_marker_native(uint8 *marker, uint32 index);
void front_draw_control_axis_mode(void);
void front_draw_centered_control_label(uint32 y, uint32 index, uint32 color);
uint32 front_control_calibration_buttons(void);
void front_draw_control_axis_marker(uint32 marker, uint32 phase);
void front_draw_control_axis_marker_native(const uint8 *marker, uint32 phase);
void front_commit_control_axis_calibration(void);
uint32 front_retain_control_viewer(void);
void front_reset_mission_briefing(void);
void front_initialize_mission_briefing(void);
void front_update_mission_briefing(void);
void front_select_mission_briefing_text(void);
void front_draw_mission_briefing(void);
void front_initialize_password_menu(void);
void front_update_password_menu(void);
void front_noop_5700(void);
void front_noop_68a0(void);
void front_noop_6bb0(void);
void front_clear_password_confirmation(void);
void front_apply_named_code(sint32 index);
void front_apply_confirmed_password(void);
void front_commit_screen_position(void);
void front_initialize_screen_position_menu(void);
void front_update_screen_position_menu(void);
void front_snapshot_display_options(void);
void front_initialize_display_confirmation(void);
void front_restore_display_options(void);
void front_initialize_sound_options_menu(void);
void front_request_options_audio(void);
void front_draw_justified_text_native(const char *text, const uint32 *layout);
void front_update_display_confirmation(void);
void front_update_sound_options_menu(void);
void front_draw_volume_bar(uint32 texture, const PSX_RECT *rectangle, uint32 color, sint32 progress);
void front_draw_left_frame(uint32 texture, const PSX_RECT *rectangle, uint32 color);
void front_draw_middle_frame(uint32 texture, const PSX_RECT *rectangle, uint32 color);
void front_draw_right_frame(uint32 texture, const PSX_RECT *rectangle, uint32 color);
void front_snapshot_sound_options(void);
void front_restore_sound_options(void);
void front_initialize_display_options_menu(void);
void front_update_display_options_menu(void);
void front_commit_display_options(void);
uint32 front_confirm_display_options(void);
void front_play_requested_intro(void);
uint32 front_request_intro(void);
void front_request_mode_five(void);
void front_request_mode_six(void);
void front_draw_gradient_rectangle(const PSX_RECT *rectangle, const uint8 *colors, uint32 blend, uint32 depth);
void front_draw_blended_rectangle(const PSX_RECT *rectangle, uint32 color, uint32 blend, uint32 depth);
void front_draw_bottom_red_rectangle(void);
void front_request_mode_seven(void);
void front_initialize_diagnostic_overlay(void);
void front_update_diagnostic_overlay(void);
void front_restore_diagnostic_overlay(void);
uint32 front_wrap_briefing_text(uint32 text, uint32 panel);
void front_play_numbered_movie(sint32 number);
void front_initialize_briefing_view(void);
void front_draw_briefing_view(void);
uint32 front_update_campaign_access_flags(void);
uint32 front_configure_preview_lighting(uint32 object, uint32 bright);
void front_draw_preview_primitives(uint32 object);
uint32 front_create_preview_camera(void);
sint32 front_find_celestial_name(const char *name);
void front_initialize_celestial_overview(void);
uint32 front_index_celestial_systems(void);
uint32 front_load_celestial_systems(void);
uint32 front_finish_selected_body_view(void);
uint32 front_play_system_audio_first(void);
uint32 front_play_system_audio_second(void);
uint32 front_play_system_audio_third(void);
uint32 front_play_body_audio_first(void);
uint32 front_play_body_audio_second(void);
uint32 front_play_body_audio_third(void);
uint32 front_stop_system_audio(void);
void front_initialize_system_database(void);
void front_initialize_bar_transitions(uint32 duration);
void front_reset_bar_transition(uint32 bar, uint32 target);
uint32 front_start_bar_transition(uint32 bar, uint32 target);
uint32 front_append_bar_transition(uint32 target, const uint8 *color);
uint32 front_prepare_bar_scale(void);
sint32 front_bar_transition_extent(void);
void front_update_bar_scale(void);
void front_draw_bar_transition_labels(uint32 index);
uint32 front_update_bar_transition(uint32 index);
void front_draw_bar_transitions(uint32 list);
void front_initialize_ranking_statistics(void);
uint32 front_campaign_rank(void);
void front_initialize_rank_models(uint32 rank);
void front_initialize_rank_lighting(void);
void front_draw_ranking_statistics(void);
void front_draw_rank_models(void);
void front_initialize_mission_statistics(void);
void front_draw_mission_statistics(void);
uint32 front_initialize_database_audio(void);
void front_update_database_audio_timer(void);
void front_free_database_audio(void);
void front_select_database_mission(void);
void front_initialize_database_mission_list(void);
void front_draw_database_mission_list(void);
void front_database_mission_idle(void);
void front_link_campaign_node(uint32 node);
uint32 front_request_complete_campaign_map(void);
uint32 front_initialize_campaign_map(void);
void front_draw_campaign_map(void);
uint32 front_select_campaign_mission(void);
void front_finish_campaign_map(void);
void front_play_selected_campaign_movie(void);
uint32 front_save_campaign_selection(void);
void front_initialize_mission_results(void);
void front_draw_mission_results(void);
void front_free_mission_results(void);
uint32 game_copy_player_view_transform(void);
uint32 game_draw_player_view(void);
void game_free_player_view(void);
uint32 game_mark_object_for_removal(uint32 object);
uint32 game_initialize_player_objects(void);
uint32 game_create_object(uint32 model, uint32 x, uint32 y, uint32 z, uint32 kind);
uint32 game_update_object_removal(uint32 object);
void game_update_player(uint32 object);
void game_dispatch_destruction_shape(uint32 object);
void game_update_player_control_mode(void);
void game_fire_auxiliary_weapon(uint32 slot);
void game_fire_weapon_profile_16(uint32 slot);
uint32 game_target_obstructed(uint32 target);
void game_fail_current_mission(void);
sint32 game_request_radio_message(uint32 asset, uint32 text);
void game_evaluate_target_occlusion(uint32 player, uint32 object, uint32 mode);
uint32 game_remove_objects_by_type(uint32 type);
void game_launch_escape_pod_sequence(void);
void game_update_player_control_mode_1(void);
void game_update_player_control_mode_2(void);
void game_update_player_control_mode_3(void);
void game_update_player_control_mode_4(void);
void game_update_player_control_mode_5(void);
void game_update_exit_request(void);
VECTOR *game_apply_transposed_matrix_native(const MATRIX *matrix, VECTOR *input, VECTOR *output);
void game_attach_object_trail(uint32 object, uint32 type);
void game_process_camera_vector(uint32 position, uint32 parameter);
void game_emit_radial_effect(uint32 x, uint32 y, uint32 z, uint32 size, uint32 color);
uint32 game_emit_vector_effect_native(uint32 position, VECTOR *direction, uint32 color, uint32 size);
uint32 game_set_screen_tint(uint32 red, uint32 green, uint32 blue);
uint32 game_create_weapon_projectile(uint32 owner, uint32 target, uint32 offset, uint32 mode);
void game_attach_object_light(uint32 object, uint32 type, uint32 color, uint32 intensity, uint32 parameter);
void game_emit_wide_particles(uint32 count, uint32 x, uint32 y, uint32 z, uint32 vx, uint32 vy, uint32 vz);
void game_emit_narrow_particles(uint32 count, uint32 x, uint32 y, uint32 z, uint32 vx, uint32 vy, uint32 vz);
uint32 game_get_object_forward(uint32 object, uint32 destination);
VECTOR *game_apply_transposed_short_matrix_native(const MATRIX *matrix, SVECTOR *input, VECTOR *output);
uint32 game_position_at_object_offset(uint32 object, uint32 offset, uint32 destination);
void game_emit_weapon_profile_16(void);
uint32 game_create_profile_16_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_effect(uint32 owner, uint32 target, uint32 offset, uint32 profile);
uint32 game_create_profile_effect_native(uint32 owner, uint32 target, SVECTOR *offset, uint32 profile);
void game_split_projectile(uint32 projectile);
void game_update_homing_projectile(uint32 projectile);
void game_update_speed_gated_projectile(uint32 projectile);
uint32 game_remove_fragmenting_projectile(uint32 projectile);
sint32 game_decrement_nonnegative(sint32 value);
sint32 game_wrap_signed_angle(uint32 angle);
sint32 game_set_acceleration_from_angles(uint32 object, uint32 scale);
sint32 game_reduce_without_sign_change(sint32 value, uint32 step);
uint32 game_canonicalize_object_angles(uint32 object);
sint32 game_set_acceleration_from_basis(uint32 object, uint32 scale);
uint32 game_step_toward_angle(uint32 target, uint32 current, sint32 limit);
uint32 game_create_view_controller(void);
uint32 game_cycle_view_mode(void);
void game_draw_screen_flash(void);
void game_update_view_controller(uint32 object);
uint32 game_poll_frame_input(void);
void game_initialize_weapon_defaults(void);
sint32 game_measure_mesh_radius(uint32 mesh);
void game_measure_model_axis_bounds(uint32 model);
void game_initialize_model_bounds(uint32 model);
uint32 game_relocate_tmd_meshes(uint32 data);
void game_set_mesh_flat_colors(uint32 mesh, uint32 red, uint32 green, uint32 blue);
uint32 game_load_tmd_model(const char *name);
uint32 game_load_resource_bundle(const char *name);
uint32 game_import_resource_bundle(uint32 data, uint32 first);
uint32 game_copy_tmd_meshes(uint32 source, uint32 destination, sint32 size);
uint32 game_relocate_collision_data(uint32 data);
uint32 game_relocate_tmd_texture(uint32 model, const TEX_INFO *info, uint32 texture_index);
void game_bind_model_name(uint32 name, uint32 first);
uint32 game_find_model_name(uint32 name);
uint32 game_uppercase_text(char *text);
uint32 game_get_variable_address(uint32 name);
uint32 game_initialize_model_name_cursor(void);
sint32 game_config_set_condition(const uint32 *arguments, uint32 first);
void game_config_load_tmd(const uint32 *arguments, uint32 first);
void game_load_bound_bundle(uint32 name);
void game_config_load_bundle(const uint32 *arguments, uint32 first);
void game_stop_config_parse(void);
void game_scale_mesh(uint32 mesh, uint32 scale);
void game_config_scale_models(const uint32 *arguments, uint32 first);
void game_config_set_variable(const uint32 *arguments, uint32 first);
void game_config_set_pal_variable(const uint32 *arguments, uint32 first);
void game_config_raise_pal_variable(const uint32 *arguments, uint32 first);
void game_config_lower_pal_variable(const uint32 *arguments, uint32 first);
void game_load_configuration(const char *name);
void game_stop_scene_stream(void);
void game_free_effect_payloads(void);
void game_finish_demo_input(void);
void game_reset_demo_selection(void);
void game_free_optional_scene_buffer(void);
void game_free_scene_buffer_banks(void);
void game_free_scene_buffers(void);
void game_free_special_mode_buffers(void);
void game_stop_auxiliary_scene_voices(void);
void game_stop_scene_voices(void);
void game_free_scene_models(void);
void game_shutdown_scene(void);
void game_emit_drifting_particles(sint32 count, uint32 x, uint32 y, uint32 z);
void game_update_line_effects(uint32 effect);
void game_update_particle_effects(uint32 effect);
uint32 game_emit_vector_effect_at_native(const VECTOR *position, VECTOR *direction, uint32 color, uint32 size);
void game_draw_camera_effects(uint32 effect);
void game_collide_scene_effects(void);
void game_update_scene_effects(void);
void game_update_effect_chain(uint32 effect);
void game_expire_model_effect(uint32 effect);
void game_finish_effect_updates(void);
void game_draw_active_effect_slots(void);
void game_update_camera_particles(void);
void game_process_camera_particle_field(uint32 position, uint32 colors);
void game_emit_object_spark_native(uint32 object, const VECTOR *offset, uint32 size, uint32 color);
void game_emit_object_spark(uint32 object, uint32 offset, uint32 size, uint32 color);
void game_emit_camera_particle_native(const VECTOR *position, const VECTOR *velocity, uint32 color, uint32 size);
void game_update_scene_particle_effects(void);
void game_update_object_particle_slots(void);
void game_update_object_spark_slots(void);
void game_draw_camera_particle_layers(void);
void game_update_camera_streaks(uint32 particles, uint32 camera);
void game_draw_camera_streak_layer_a(uint32 mode, uint32 particles, uint32 camera, sint32 intensity);
void game_draw_camera_streak_layer_b(uint32 mode, uint32 particles, uint32 camera, sint32 intensity);
void game_draw_camera_overlay(void);
void game_draw_camera_overlay_points_native(uint32 count, uint32 *result);
void game_update_object_trails(void);
void game_update_selected_object_link(void);
void game_process_pause_request(void);
void game_mute_scene_effect_voices(void);
void game_suspend_scene_engine_audio(void);
void game_initialize_scene_engine_voices(void);
void game_initialize_scene_effect_voices(void);
void game_initialize_pause_grid(void);
void game_prepare_pause_scene(void);
void game_run_pause_menu(void);
uint32 game_count_pause_menu_entries(void);
void game_initialize_pause_selection(uint32 menu);
void game_draw_pause_frame(uint32 mode);
void game_pause_menu_leave(void);
void game_begin_pause_menu_transition(void);
void game_pause_menu_enter(void);
void game_move_pause_selection(uint32 input, uint32 menu);
void game_update_pause_selection(void);
void game_handle_pause_menu_input(void);
void game_swap_pause_menu_geometry(void);
void game_swap_pause_menu_return_geometry(void);
void game_request_pause_mission_exit(void);
void game_draw_pause_grid_background(void);
void game_draw_pause_ship_preview(void);
void game_draw_pause_heading_for_menu(uint32 menu);
void game_draw_pause_heading(void);
void game_draw_pause_warp_grid_values(uint32 fade, uint32 base_red, uint32 texture_depth);
void game_draw_pause_warp_grid(void);
void game_draw_pause_options(void);
void game_draw_pause_options_for_menu(uint32 menu);
void game_draw_pause_option(uint32 x, uint32 y, uint32 selected, uint32 entry);
void game_draw_pause_panel(void);
void game_draw_pause_panel_for_menu(uint32 menu);
void game_draw_attached_glows(void);
void game_draw_glow_rays(uint32 x, uint32 y, uint32 size, uint32 depth, uint32 color);
void game_draw_partial_rotating_mesh(uint32 phase);
void game_draw_message_cursor(uint32 position[2], const uint32 dimensions[2], uint32 color);
void game_draw_message_queue(void);
void game_draw_expanding_bursts(void);
void game_draw_burst_core(uint32 x, uint32 y, uint32 size, uint32 depth);
void game_normalize_direction_native(VECTOR *output, VECTOR *input);
uint32 game_probe_target_path_native(uint32 source, uint32 distance, const VECTOR *origin, const VECTOR *direction);
void game_choose_target_vertex_native(uint32 source, uint32 target, SVECTOR *output);
uint32 game_is_weapon_target_in_range(uint32 source, uint32 target, uint32 range);
void game_update_weapon_state(void);
void game_update_and_draw_scene(void);
void game_get_target_vertex_vector_native(uint32 source, uint32 target, VECTOR *output);
void game_get_target_vertex_vector(uint32 source, uint32 target, uint32 output);
void game_draw_muzzle_glow_native(const VECTOR *position);
void game_draw_muzzle_glow_for_camera_native(const VECTOR *position, uint32 camera);
uint32 game_create_aimed_projectile(uint32 source, uint32 target, uint32 mode);
void game_draw_paired_glows(void);
uint32 game_reset_player_ring_limits(void);
void game_reset_object_ring_limits(void);
void game_process_ring_crossing(uint32 object, uint32 direction);
uint32 game_crosses_face_native(uint32 face, const VECTOR *start, const VECTOR *end);
uint32 game_test_ring_crossing(uint32 object, uint32 target);
void game_update_training_rings(void);
void game_set_ring_glow_vertex(uint32 object, uint32 index, uint32 color, uint32 output);
void game_update_ring_glow(uint32 object);
void game_emit_ring_fragments(uint32 object, uint32 position, uint32 mode);
uint32 game_count_objects_by_category(uint32 identifier, uint32 category);
sint32 game_play_condition_message(uint32 event);
uint32 game_check_object_count_message(uint32 event);
uint32 game_check_ring_count_message(uint32 event);
uint32 game_test_object_collision_box_native(uint32 object, VECTOR *position);
void game_set_collision_rotation_native(uint32 object, const VECTOR *position, const VECTOR *direction,
    const VECTOR *velocity, uint32 strength);
void game_resolve_player_collision_native(uint32 object, const VECTOR *velocity);
uint32 game_check_player_object_collision(uint32 object);
void game_notify_object_contact(uint32 first, uint32 second);
void game_resolve_object_collision(uint32 object, uint32 obstacle);
sint32 game_test_object_collision_native(uint32 object, uint32 other, const VECTOR *start, const VECTOR *end);
uint32 game_check_swept_object_collision(uint32 object, uint32 other);
void game_separate_object_overlaps(uint32 object);
void game_draw_pause_menu(void);
void game_refresh_mission_state(void);
sint32 game_update_nearby_effect_audio(void);
void game_draw_environment_mesh(void);
void game_update_effect_audio_channel(void);
void game_update_first_nearby_effect_audio(void);
void game_update_second_nearby_effect_audio(void);
void game_draw_queued_scene_messages(void);
void game_update_timed_point_effects(uint32 emit_particles);
void game_draw_timed_point_effects(uint32 mode, uint32 effects, uint32 count, uint32 camera);
void game_emit_timed_point_effect(uint32 position, uint32 size);
uint32 game_initialize_mesh_effect_pool(void);
sint32 game_bind_mesh_effect_texture(uint32 texture, uint32 slot);
void game_draw_mesh_effect_quad(uint32 first, uint32 second, uint32 third, uint32 fourth, uint32 texture, uint32 color, uint32 transparent);
void game_draw_object_link_native(uint32 first_object, uint32 second_object, VECTOR *first_offset, VECTOR *second_offset, uint32 distance, uint32 mode, uint32 power);
void game_start_damage_state(void);
void game_update_damage_state(uint32 object);
void game_finish_damage_state(uint32 object);
uint32 game_create_temporary_camera(uint32 owner, uint32 duration);
void game_update_temporary_camera(uint32 object);
uint32 game_start_timed_view_effect(uint32 magnitude, uint32 duration);
void game_update_timed_view_effect(uint32 object);
void game_apply_player_damage_native(uint32 damage, const VECTOR *position);
void game_apply_player_damage_from_state_native(uint32 damage, const VECTOR *position, uint32 player);
void game_handle_player_projectile_hit(uint32 object, uint32 position, uint32 unused, uint32 other, uint32 projectile, uint32 kind, uint32 damage_profile);
void game_emit_hit_flash(uint32 x, uint32 y, uint32 z);
void game_reset_object_collision_links(void);
void game_collide_scene_object_groups(void);
void game_collide_object_groups(uint32 head, uint32 object, sint32 range, uint32 mode, uint32 same_group);
void game_resolve_special_mission_collision(uint32 first, uint32 second);
void game_record_destroyed_object(uint32 object, uint32 attacker, uint32 projectile);
void game_finish_special_mission_collision(uint32 object);
sint32 game_test_model_segment_native(uint32 object, const VECTOR *point, VECTOR *segment, uint32 output);
void game_dispatch_object_hit(uint32 target, uint32 object, uint32 position, uint32 vector, uint32 other, uint32 projectile, uint32 kind, uint32 profile);
void game_draw_scene_descriptor_chain(void);
void game_draw_descriptor_chain(uint32 descriptors, uint32 camera, uint32 mode);
void game_unlink_mesh_effect_type(uint32 effect);
void game_process_mesh_effect_removals(void);
void game_draw_mesh_effect_chain(uint32 effect);
void game_draw_mesh_effects_from_camera(uint32 effect, uint32 camera);
void game_draw_scene_mesh_effects(void);
void game_draw_active_groups(void);
void game_draw_view_by_mode(uint32 view, uint32 index);
void game_finish_scene_objects(void);
sint32 game_restore_scene_draw_color(void);
void game_collide_effects_with_object(uint32 effect, uint32 object);
void game_resolve_sphere_effect_hit(uint32 object, uint32 effect);
void game_resolve_model_effect_hit(uint32 object, uint32 effect);
void game_apply_effect_impulse(uint32 object, uint32 effect, uint32 velocity, uint32 strength);
void game_collide_effect_chains(uint32 effect, uint32 other);
uint32 matrix_copy_rotation_native(uint32 destination, const MATRIX *source);
uint32 matrix_copy_rotation_to_native(MATRIX *destination, uint32 source);
uint32 game_find_nearest_object(uint32 object, uint32 filter, uint32 group, uint32 excluded_flags);
void game_rotate_object_basis(uint32 object);
void game_check_object_collisions(uint32 object);
void game_update_tractor_beam(uint32 object);
void game_apply_tractor_beam(uint32 target, uint32 object, uint32 beam);
uint32 game_update_scene_objects(void);
void game_draw_tractor_beam_native(VECTOR *points, uint32 unused);
void game_draw_tractor_curve_native(const VECTOR *points, uint32 camera);
void game_draw_jittered_curve_native(VECTOR *points, uint32 camera, uint32 jitter_shift);
void game_draw_jittered_beam_native(VECTOR *points, uint32 unused, uint32 jitter_shift);
void game_emit_point_sparks(uint32 count, uint32 x, uint32 y, uint32 z);
void game_draw_projectile_arcs(uint32 object);
void game_draw_projectile_sparks(uint32 object);
void game_draw_model_arcs(uint32 object, uint32 first_index, uint32 second_index);
void game_update_object_fade_out(uint32 controller);
void game_update_object_fade_in(uint32 object);
void game_draw_fading_object(uint32 object);
void game_draw_scene_object(uint32 object);
void game_accumulate_effect_audio(uint32 position, uint32 intensity);
void game_accumulate_effect_audio_from_camera(uint32 position, uint32 intensity, uint32 camera);
void game_draw_object_details(uint32 object);
void game_draw_attached_record(uint32 object, uint32 record, uint32 mode);
void game_draw_attached_record_small(uint32 object, uint32 record);
uint32 game_allocate_effect(uint32 type, uint32 owner);
void game_mark_effect_for_removal(uint32 effect);
void game_unlink_effect_type(uint32 effect);
void game_process_effect_removals(void);
void game_fire_weapon_profile_12(uint32 slot);
void game_emit_weapon_profile_12(void);
void game_fire_weapon_profile_18(uint32 slot);
void game_emit_weapon_profile_18(void);
void game_fire_weapon_profile_10(uint32 slot);
void game_emit_weapon_profile_10(void);
void game_fire_weapon_profile_1a(uint32 slot);
void game_emit_weapon_profile_1a(void);
void game_fire_weapon_profile_14(uint32 slot);
void game_emit_weapon_profile_14(void);
uint32 game_create_profile_12_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_10_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_14_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_18_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_1a_effect(uint32 owner, uint32 target, uint32 offset);
void game_fire_weapon_profile_02(uint32 slot);
void game_fire_mission_weapon(uint32 slot);
void game_fire_weapon_profile_06(uint32 slot);
void game_fire_weapon_profile_04(uint32 slot);
void game_fire_weapon_profile_00(uint32 slot);
void game_fire_weapon_profile_0c(uint32 slot);
void game_fire_weapon_profile_0e(uint32 slot);
void game_fire_weapon_profile_0a(uint32 slot);
void game_fire_weapon_profile_20(uint32 slot);
uint32 game_create_mission_projectile(uint32 owner, uint32 target, uint32 offset, uint32 mode);
uint32 game_create_profile_06_projectile(uint32 owner, uint32 target, uint32 offset, uint32 mode);
uint32 game_create_profile_0c_projectile(uint32 owner, uint32 offset);
void game_detach_object_trails(uint32 object);
void game_destroy_weapon_projectile(uint32 projectile);
void game_destroy_mission_projectile(uint32 projectile);
void game_emit_projectile_impact(uint32 x, uint32 y, uint32 z);
void game_emit_projectile_fragments(uint32 position, uint32 object, sint32 count, uint32 mode);
void game_emit_reflected_particles(uint32 count, uint32 x, uint32 y, uint32 z, uint32 vx, uint32 vy, uint32 vz);
sint32 game_reflect_vector_native(const VECTOR *input, const VECTOR *normal, VECTOR *output);
void game_emit_fragment_flash(uint32 x, uint32 y, uint32 z);
uint32 game_allocate_flash(uint32 type);
void game_collide_mission_projectile(uint32 projectile);
void game_queue_hud_message(uint32 text);
void game_emit_sized_fragment_flash(uint32 position, uint32 size);
void game_attach_position_light(uint32 object, uint32 position, uint32 color, uint32 intensity, uint32 parameter);
void game_update_reprogrammed_target(uint32 object);
void game_update_object_state(uint32 object);
uint32 game_update_object_movement(uint32 object);
sint32 game_limit_signed_step(sint32 value, sint32 limit);
MATRIX *game_multiply_transposed_right_native(MATRIX *left, const MATRIX *right, MATRIX *output);
uint32 game_integrate_projectile(uint32 object);
void game_update_weapon_projectile(uint32 projectile);
void game_update_profile_0c_projectile(uint32 projectile);
void game_update_expiring_projectile(uint32 projectile);
void game_update_auxiliary_projectile(uint32 projectile);
uint32 front_initialize_selected_body_view(void);
void front_restore_selected_body_names(void);
void front_update_selected_system_menu(void);
void front_draw_selected_system_menu(void);
void front_draw_system_selection_link(uint32 row_spacing);
void front_update_system_body_menu(void);
uint32 front_retain_system_database(void);
uint32 front_free_system_database(void);
uint32 front_restore_system_database_title(void);
uint32 front_close_system_body_menu(void);
void front_initialize_system_body_menu(void);
void front_update_system_database(void);
void front_draw_system_database(void);
void front_copy_selected_system_name(void);
void front_initialize_selected_celestial_system(void);
uint32 front_count_celestial_descendants(uint32 node);
void front_draw_celestial_ring(uint32 ring);
void front_draw_celestial_marker(const sint16 *point, uint32 depth, uint32 color_index);
void front_initialize_celestial_ring(uint32 ring, uint32 radius, uint32 color, uint32 scale, uint32 phase, uint32 definition);
void front_draw_gradient_embossed_line(const sint16 *points, uint32 first_color, uint32 second_color, uint32 depth);
void front_draw_briefing_heap_diagnostics(void);
uint32 front_retain_briefing_view(void);
void front_free_briefing_view(void);
uint32 front_leave_briefing_view(void);
void front_draw_scaled_text_native(const char *text, const PSX_RECT *rectangle, uint32 color, uint32 blend);
void front_draw_scaled_text(uint32 text, uint32 rectangle, uint32 color, uint32 blend);

uint32 game_initialize_player_weapon_slots(void);
uint32 game_initialize_weapon_profile_14(uint32 slot);
uint32 game_initialize_weapon_profile_12(uint32 slot);
uint32 game_initialize_weapon_profile_16(uint32 slot);
uint32 game_initialize_weapon_profile_10(uint32 slot);
uint32 game_initialize_weapon_profile_1a(uint32 slot);
uint32 game_initialize_weapon_profile_18(uint32 slot);
uint32 game_initialize_weapon_profile_04(uint32 slot);
uint32 game_initialize_weapon_profile_02(uint32 slot);
uint32 game_initialize_weapon_profile_06(uint32 slot);
uint32 game_initialize_weapon_profile_00(uint32 slot);
uint32 game_initialize_weapon_profile_0a(uint32 slot);
uint32 game_initialize_weapon_profile_0e(uint32 slot);
uint32 game_initialize_weapon_profile_0c(uint32 slot);
uint32 game_initialize_mission_weapon_slot(uint32 slot);
uint32 game_initialize_weapon_profile_20(uint32 slot);

void game_update_model_transition(uint32 object);
sint32 game_count_active_standard_objects(void);
void game_initialize_model_table(void);
void game_canonicalize_angle_pair(uint32 first_angle, uint32 second_angle);
void game_set_object_rotation(uint32 object, uint32 yaw, uint32 pitch, uint32 roll);
void game_fire_antishield_projectile(uint32 object);
void game_initialize_object_profile_29a(uint32 object);
uint32 game_create_object_link_native(uint32 source, uint32 target, const VECTOR *origin, const VECTOR *first_offset, const VECTOR *second_offset);
uint32 game_release_object_link(uint32 object);
void game_draw_radial_glow_native(const VECTOR *position, uint32 radius, uint32 color, uint32 perspective);
sint32 game_choose_object_link_offsets_native(uint32 source, uint32 target, VECTOR *first_output, VECTOR *second_output);
void game_update_link_audio_native(const VECTOR *position);
void game_emit_sized_fragment_flash_native(const VECTOR *position, uint32 size);
uint32 game_apply_object_link_contact_native(uint32 object, uint32 record, const VECTOR *step);
void game_apply_sensor_damage(uint32 object, sint32 damage);
void game_reset_object_control_link(uint32 object);
void game_draw_object_link_segments_native(uint32 record, const VECTOR *start, const VECTOR *step, sint32 count, const VECTOR *cross);
void game_update_object_link(uint32 object);
void game_update_automatic_object_links(uint32 object);
void game_update_object_automatic_weapons(uint32 object);
void game_update_temporary_sensor_damage(uint32 object);
void game_update_sensor_recovery(uint32 controller);
void game_update_permanent_sensor_damage(uint32 object);
void game_fire_target_decoy(uint32 object);
void game_try_launch_target_decoy(uint32 object);
uint32 game_set_relative_target_angles(uint32 object, uint32 x, uint32 y, uint32 z);
void game_update_timed_maneuver(uint32 object);
void game_begin_yaw_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 yaw, uint32 pitch, uint32 update);
void game_begin_roll_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 roll, uint32 pitch, uint32 update);
void game_update_target_maneuver(uint32 object);
uint32 game_update_maneuver_bias(uint32 object);
uint32 game_update_maneuver_bias_from_heading(uint32 object, uint32 heading);
void game_update_target_maneuver_angles(uint32 object);
void game_update_cached_target_tracking(uint32 object);
void game_update_target_weapons(uint32 object);
uint32 game_create_plasma_torpedo(uint32 owner, uint32 target, uint32 offset);
void game_try_fire_plasma_torpedo(uint32 object);
void game_try_fire_burst_torpedo(uint32 object);
void game_try_fire_antishield_torpedo(uint32 object);
void game_try_fire_rear_lasers(uint32 object);
void game_try_fire_ki_plasma_lasers(uint32 object);
void game_try_fire_scatter_lasers(uint32 object);
void game_try_fire_emp_lasers(uint32 object);
void game_try_fire_primary_lasers(uint32 object);
void game_try_fire_stun_missile(uint32 object);
void game_try_fire_antishield_missile(uint32 object);
void game_try_fire_motion_missile(uint32 object);
void game_try_fire_tracker_missile(uint32 object);
uint32 game_create_antishield_missile(uint32 owner, uint32 target, uint32 offset, uint32 trail);
void game_synchronize_missile_cooldowns(uint32 object);
uint32 game_create_stun_missile(uint32 object);

void game_try_fire_hull_lasers(uint32 object);
void game_try_fire_antishield_lasers(uint32 object);
uint32 game_create_emp_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_hull_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_antishield_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_rear_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_scatter_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_ki_plasma_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_model_effect(uint32 position, sint32 model_index);
void game_clear_trail_slots(void);
uint32 game_draw_laser_projectile(uint32 object);
void game_bind_overlay_effect_textures(void);
uint32 game_pack_palette_color(sint32 red, sint32 green, sint32 blue);
sint32 game_upload_palette_row(uint32 x, uint32 y, uint32 pixels);
sint32 game_upload_palette_row_native(uint32 x, uint32 y, const uint16 *pixels);
uint32 game_find_model_name_native(const char *name);
void game_bind_model_name_native(const char *name, uint32 first);
void game_initialize_head_models_and_palettes(void);
void game_emit_model_fragments(uint32 position, sint32 count, uint32 mode);
void game_handle_effect_contact(uint32 effect, uint32 other);
uint32 game_draw_horizontal_reticle(uint32 packet);
void game_draw_burst_effect(uint32 object);
void game_draw_pulsing_burst_effect(uint32 object);
void game_draw_projected_ring(sint32 x, sint32 y, sint32 radius, uint32 center_color, uint32 edge_color);
void game_draw_position_glow(uint32 position, uint32 color, uint32 size);
sint32 anim_play_movie(uint32 path);
sint32 anim_play_default_movie(void);
sint32 anim_play_movie_with_skip_delay(uint32 path);
sint32 anim_play_movie_without_clear(uint32 path);
void game_draw_orbiting_projectile_effect(uint32 object);
void game_begin_mesh_effect_sequence(uint32 object);
void game_initialize_radial_mesh(uint32 x, uint32 y, uint32 z);
void game_orient_radial_mesh(uint32 x, uint32 y, uint32 z);
void game_clear_model_binding_slots(void);
void game_clear_scene_resource_state(void);
void matrix_rotate_x_before_native(MATRIX *destination, MATRIX *source, uint32 angle);
void matrix_rotate_y_before_native(MATRIX *destination, MATRIX *source, uint32 angle);
void matrix_rotate_z_before_native(MATRIX *destination, MATRIX *source, uint32 angle);
void game_apply_control_rotation(uint32 matrix, uint32 yaw, uint32 pitch, uint32 roll);
void game_update_flight_controls(uint32 object);
void game_update_engine_audio(uint32 controls);
uint32 game_get_controller_state(void);
void game_initialize_direction_indicator(void);
void game_config_select_ship(const uint32 *arguments, uint32 first);
void game_load_bound_bundle_native(const char *name);
void game_config_set_ship_model(const uint32 *arguments, uint32 first);
void game_config_set_ship_type(const uint32 *arguments, uint32 first);
void game_update_stream_callback(void);

void game_initialize_effect_pool(void);
void game_clear_scene_effect_flags(void);
void game_clear_scene_link_slots(void);
void game_clear_auxiliary_effect_slots(void);
void game_clear_particle_records(void);
void game_clear_ship_effect_counters(void);
void game_initialize_local_starfield(void);
void game_initialize_spherical_starfield(void);
void game_insert_spherical_star(uint32 x, uint32 y, uint32 z, uint32 elevation, uint32 azimuth);

void game_load_scene_texture_page(void);
void game_parse_level_numbers(uint32 text);
void game_initialize_fragment_slots(void);
void game_initialize_scene_sky(void);
void game_generate_scene_sky(uint32 seed, uint32 color, uint32 second_color, uint32 third_color, uint32 fourth_color);
void game_update_combat_maneuver(uint32 object);

void game_initialize_sky_geometry(void);
void game_initialize_sky_resources(void);

void game_position_attached_effect(uint32 object, uint32 effect, uint32 position, uint32 anchor, uint32 distance);
void game_position_scene_effects(void);
void game_initialize_blob_effect_resources(void);

void game_measure_level_model_extent(void);
void game_create_script_controller(void);
void game_create_script_event_controller(void);
void game_reset_level_script_state(void);

void game_validate_level_script(void);
void game_initialize_level_script(void);
void game_load_level_script(uint32 name);

void game_config_set_unique_ship_name(const uint32 *arguments, uint32 first);
void game_config_set_ship_intelligence(const uint32 *arguments, uint32 first);
void game_config_set_ship_shield(const uint32 *arguments, uint32 first);
void game_config_set_ship_velocity(const uint32 *arguments, uint32 first);
void game_config_set_ship_sensor(const uint32 *arguments, uint32 first);
void game_config_set_ship_hull(const uint32 *arguments, uint32 first);
void game_config_set_ship_scale(const uint32 *arguments, uint32 first);
void game_config_set_ship_size(const uint32 *arguments, uint32 first);
void game_config_fire(void);
void game_config_objective_text(void);

void game_config_select_task_list(const uint32 *arguments, uint32 first);
void game_config_append_task_id(const uint32 *arguments, uint32 first);
void game_config_begin_task_commands(void);
sint32 game_lookup_script_task(uint32 text, uint32 table);
uint32 game_encode_script_task(const uint32 *arguments, uint32 first, uint32 destination);
void game_config_append_task_command(const uint32 *arguments, uint32 first);

void game_rotate_player_y(uint32 angle);
void game_rotate_player_x(uint32 angle);
void game_rotate_player_z(uint32 angle);
void game_load_asteroid_bundles(void);
void game_config_create_asteroid_controller(const uint32 *arguments, uint32 first);
uint32 game_find_ship_start_callback(uint32 ship);
void game_assign_ship_task_list(uint32 object, uint32 list);
void game_config_create_ship(const uint32 *arguments, uint32 first);
uint32 game_spawn_script_ship(uint32 ship, uint32 x, uint32 y, uint32 z, uint32 yaw, uint32 pitch, uint32 roll);

void game_config_add_waypoint(const uint32 *arguments, uint32 first);
void game_config_create_training_hoop(const uint32 *arguments, uint32 first);
void game_config_set_player_launch(const uint32 *arguments, uint32 first);
void game_config_bind_ship_name(const uint32 *arguments, uint32 first);
void game_config_add_ship_name(const uint32 *arguments, uint32 first);

void game_config_set_hoop_path(const uint32 *arguments, uint32 first);
void game_config_add_jumpgate_objective(void);
void game_config_add_drone_objective(void);

void game_config_destroy_ship_objective(const uint32 *arguments, uint32 first);
void game_config_time_greater_objective(const uint32 *arguments, uint32 first);
void game_config_time_less_objective(const uint32 *arguments, uint32 first);

void game_config_ship_finished_objective(const uint32 *arguments, uint32 first);
void game_config_ships_destroyed_objective(const uint32 *arguments, uint32 first);
void game_config_ships_docked_objective(const uint32 *arguments, uint32 first);
void game_config_ship_disabled_objective(const uint32 *arguments, uint32 first);

void game_config_navy_defeated_objective(void);
void game_config_league_defeated_objective(void);
void game_config_planet_destroyed_objective(void);

void game_config_add_objective(const uint32 *arguments, uint32 first);
void game_config_set_mission_ending(const uint32 *arguments, uint32 first);
void game_config_set_scene_boundary(const uint32 *arguments, uint32 first);
void game_config_create_ship_boundary(const uint32 *arguments, uint32 first);

uint32 game_find_message_ship(uint32 text);
void game_config_add_message(const uint32 *arguments, uint32 first);
void game_config_set_token_mode(const uint32 *arguments, uint32 first);
void game_config_no_navy_fighters_objective(void);

void game_config_create_asteroid_belt(const uint32 *arguments, uint32 first);
void game_initialize_asteroid_belt(uint32 controller);
uint32 game_create_belt_asteroid(uint32 controller);

uint32 game_create_model_effect_native(const VECTOR *position, sint32 model_index);

void game_config_create_mine_field(const uint32 *arguments, uint32 first);

void game_exit_level_script(void);
void game_config_create_planet(const uint32 *arguments, uint32 first);
void game_apply_script_ship_properties(uint32 object);
void game_tick_script_ship_start(uint32 object);
void game_begin_script_ship_task(uint32 object);
void game_script_ship_attack_error(uint32 object);
void game_script_ship_avoid_error(uint32 object);
void game_script_ship_death_error(uint32 object);
void game_tick_script_events(void);
uint32 game_script_event_message(uint32 event);
uint32 game_script_event_mission_success(uint32 event);
uint32 game_script_event_mission_failure(uint32 event);
uint32 game_script_event_category_below(uint32 event);
uint32 game_script_event_ship_count(uint32 event);
uint32 game_script_event_category_above(uint32 event);
uint32 game_script_event_ship_reenabled(uint32 event);
uint32 game_script_event_ship_counter(uint32 event);
uint32 game_script_event_ship_health_below(uint32 event);
uint32 game_script_event_ship_population_below(uint32 event);
uint32 game_script_event_ship_distance_below(uint32 event);
uint32 game_script_event_ship_signal(uint32 event);
uint32 game_script_event_ship_flag(uint32 event);
uint32 game_script_event_ship_secondary_count(uint32 event);
uint32 game_script_event_ship_age(uint32 event);
uint32 game_count_script_ship_objects(uint32 identifier, uint32 type);
uint32 game_find_script_ship_health_below(uint32 identifier, uint32 threshold);
uint32 game_find_script_ship_near(uint32 object, uint32 identifier, uint32 range, uint32 flags);
uint32 game_find_script_ship_near_with_distance_input(uint32 object, uint32 identifier, uint32 range, uint32 flags, uint32 *last_z_magnitude);
uint32 game_script_event_elapsed_with_ship(uint32 event);
uint32 game_script_event_ship_primary_depleted(uint32 event);
uint32 game_script_event_ship_flag_count(uint32 event);
uint32 game_script_event_elapsed(uint32 event);
uint32 game_script_event_ship_ratio_reached(uint32 event);
uint32 game_script_event_ship_idle_time(uint32 event);
uint32 game_script_event_type_seven_count(uint32 event);
uint32 game_script_event_ship_task_flag_count(uint32 event);
uint32 game_script_event_ship_slot_flag(uint32 event);
uint32 game_script_event_category_flag_count(uint32 event);
uint32 game_script_event_ship_count_with_type_three(uint32 event);
void game_initialize_training_controller(void);
uint32 game_create_gradient_strip_set(uint32 first, uint32 second, uint32 third);
void game_initialize_configured_objects(void);
void game_load_player_models(uint32 player_type);
void game_initialize_demo_input(uint32 recording);
void game_load_demo_input(uint32 level_name);
void game_begin_input_recording(void);
uint32 game_initialize_level_scene(void);
void game_initialize_scene_grid(void);
void game_initialize_credits_controller(void);
void game_fill_credits_records(void);
void game_draw_demo_input_status(void);
void game_tick_credits_controller(uint32 object);
uint32 game_update_and_draw_credits(void);

uint32 game_get_ship_task_parameter(uint32 task, uint32 parameter);
uint32 game_validate_ship_task_target(uint32 object);
uint32 game_ship_task_skip(void);
uint32 game_dispatch_ship_task_native(uint32 target, uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_01(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_02(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_03(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_04(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_05(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_06(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_07(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_08(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_09(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_10(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_11(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_12(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_13(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_14(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_15(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_16(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_17(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_18(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_19(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_20(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_21(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_22(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_23(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_24(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_25(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_26(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_27(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_28(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_29(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_30(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_31(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_32(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_33(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_34(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_35(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_36(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_37(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_38(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_39(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_40(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_41(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_42(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_43(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_44(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_45(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_46(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_47(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_48(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_49(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_50(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_51(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_52(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_53(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_54(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_55(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_56(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_57(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_58(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);
uint32 game_ship_task_59(uint32 object, uint32 task, uint32 ordinal, uint32 *cursor);

uint32 game_find_nearest_type_one(uint32 object, uint32 excluded_flags);
uint32 game_find_nearest_type_three(uint32 object, uint32 excluded_flags);
void game_begin_ship_task_approach(uint32 object, uint32 range);
uint32 game_skip_ship_task(uint32 object, uint32 ordinal);
uint32 game_call_ship_death_callback(uint32 object);

void game_begin_timed_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 update);
uint32 game_adjust_maneuver_offsets(uint32 yaw, uint32 pitch);
void game_steer_to_target_offset(uint32 object, uint32 target, uint32 offset, uint32 mode);

void game_update_ship_task_approach(uint32 object);
void game_check_ship_task_approach_target(uint32 object);
void game_tick_timed_maneuver(uint32 object);
uint32 game_validate_ship_attack_target(uint32 object);

uint32 game_select_ship_task_target(uint32 object, uint32 selection, uint32 group, uint32 identifier, uint32 excluded_flags);
void game_report_tractor_capture(uint32 object, uint32 target);

uint32 game_count_matching_active_ship_objects(uint32 reference);
uint32 game_select_carrier_launch_bay(uint32 object);
void game_initialize_ship_launch(uint32 ship, uint32 carrier, uint32 bay);
void game_position_ship_at_carrier_offset(uint32 carrier, uint32 offset, uint32 ship);
uint32 game_find_free_carrier_bay_near_target(uint32 carrier, uint32 target);
uint32 game_find_nearby_opposing_ship(uint32 object, uint32 range);
void game_update_deferred_ship_launch(uint32 object);
uint32 game_collect_matching_ships(uint32 identifier, uint32 output, uint32 category, uint32 excluded_flags);
uint32 game_create_escape_pod(uint32 carrier);
void game_reposition_cloaked_ship(uint32 object);
void game_update_actor_flag_delay(uint32 object);
uint32 game_find_nearest_object_in_range(uint32 object, uint32 category, uint32 range, uint32 excluded_flags);
void game_stop_and_launch_escape_pods(uint32 object);
void game_begin_ship_allegiance_change(uint32 object);
void game_replace_ship_for_category(uint32 object, uint32 category);
void game_tick_ship_task_delay(uint32 object);
void game_move_ship_and_tick_task_delay(uint32 object);
void game_update_ship_task_wait(uint32 object);
void game_begin_ship_offset_follow_maneuver(uint32 object);
void game_update_ship_retreat_task(uint32 object);
void game_update_ship_offset_follow_maneuver(uint32 object);
void game_update_changed_ship_threats(void);
void game_update_escape_pod_movement(uint32 object);
void game_update_carrier_ship_launch(uint32 object);
void game_draw_carrier_launch_bay(uint32 carrier, uint32 bay);
void game_draw_launch_bay_edge_native(const VECTOR *normal, const SVECTOR *first, const SVECTOR *second, sint32 depth, sint32 progress);
void game_update_carrier_ship_docking(uint32 object);
uint32 game_find_active_ship_by_identifier(uint32 identifier, uint32 category);
uint32 game_distance_to_nearest_large_ship(uint32 position);
void game_update_distant_rotating_model(uint32 object);
void game_update_belt_asteroid_spawner(uint32 controller);
void game_process_object_list_contacts(uint32 head, uint32 object, sint32 range);
void game_emit_velocity_fragments(uint32 position, uint32 velocity, uint32 count);
uint32 game_create_moonbase_grid_controller(uint32 object);


uint32 game_distance_to_nearest_large_ship_native(const uint32 position[3]);
void game_create_controller_behind_player(void);
void game_emit_object_debris(uint32 object);
void game_finalize_ship_removal(uint32 object);
void game_update_delayed_ship_behavior(uint32 object);

void game_draw_and_select_target(uint32 object);

void game_update_tractor_pull(uint32 object);

void game_update_ship_point_approach(uint32 object);

void game_update_tractor_bay_retrieval(uint32 object);

sint32 game_allocate_warp_effect(uint32 position, uint32 matrix, uint32 object);
sint32 game_allocate_warp_effect_native(const uint32 position[3], uint32 matrix, uint32 object);

void game_update_ship_warp_departure(uint32 object);

sint32 game_allocate_arrival_warp_effect(uint32 position, uint32 matrix, uint32 object);
sint32 game_allocate_arrival_warp_effect_native(const VECTOR *position, const MATRIX *matrix, uint32 object);

void game_update_jumpgate_ship_controller(uint32 object);

void game_draw_position_glow_native(const uint32 position[3], uint32 color, uint32 size);

void game_emit_model_breakup(uint32 object);
void game_update_ship_entry_placement(uint32 object);

void game_update_ship_timed_departure(uint32 object);
void game_replace_group_member_states(uint32 object, uint32 from_state, uint32 to_state);

void game_update_ship_formation(uint32 object);

void game_spawn_approaching_asteroid(uint32 controller);

void game_update_ship_random_point(uint32 object);
void game_update_warphole_lifetime(uint32 object);

void game_draw_large_warp_glow(uint32 position, uint32 radius, uint32 color, uint32 phase);
void game_draw_large_warp_glow_native(const VECTOR *position, uint32 radius, uint32 color, uint32 phase);
void game_update_drifting_object(uint32 object);

sint32 sound_play_at_listener_native(const VECTOR *position, uint32 sample, uint32 unused, uint32 listener);
void game_handle_ship_hit_native(uint32 object, VECTOR *position, VECTOR *vector, uint32 other, uint32 projectile, uint32 kind, uint32 profile);

void game_update_warphole_impact(uint32 object);

void game_draw_target_beam_native(VECTOR *position, VECTOR *direction, uint32 phase);
void game_begin_object_camera_sequence(uint32 object, uint32 target);
void game_update_target_beam_sequence(uint32 object);

uint32 game_remove_other_objects_of_type(uint32 type, uint32 except);

void game_begin_target_effect_camera(uint32 object, uint32 target);
void game_draw_target_effect_field(uint32 phase);
sint32 game_normalize_vector_exact_in_place(uint32 vector);
sint32 game_build_effect_palette_variants(void);
void game_draw_annulus(uint32 descriptor);
void game_draw_annulus_native(uint32 first_radius, uint32 second_radius, uint32 first_color, uint32 second_color);
void game_draw_camera_annulus(uint32 first_radius, uint32 second_radius, uint32 first_color, uint32 second_color);
sint32 game_draw_effect_quad_mesh(uint32 variant);
void game_draw_visible_quad_mesh_effect(uint32 object);
sint32 game_emit_long_lived_particle(uint32 position, uint32 velocity, uint32 color, uint32 size);
sint32 game_expire_attached_effects(uint32 owner);
void game_draw_attached_triangle(uint32 object, uint32 record);
void game_draw_engine_effects(uint32 object);
uint32 game_update_following_roll_maneuver(uint32 object);
sint32 game_begin_following_roll_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 update);
uint32 game_update_spinning_maneuver(uint32 object);
void game_begin_spinning_maneuver(uint32 object, uint32 ticks, uint32 completion, uint32 yaw, uint32 pitch, uint32 update);
uint32 game_update_four_phase_turn_maneuver(uint32 object);
sint32 game_begin_four_phase_turn_maneuver(uint32 object, uint32 completion);
uint32 game_update_roll_transition_maneuver(uint32 object);
sint32 game_begin_roll_transition_maneuver(uint32 object, uint32 completion);
void game_start_ai_mode_maneuver(uint32 object, uint32 update, uint32 ticks);
sint32 game_update_pitch_alignment_timer(uint32 object);
void game_draw_projected_fan(sint32 x, sint32 y, sint32 radius, uint32 center_color, uint32 edge_color);
void game_draw_twisted_ring(uint32 descriptor, uint32 first_angle_offset, uint32 second_angle_offset);

void game_draw_target_effect_view(uint32 object);

void game_begin_mission_completion(uint32 player);
void game_update_mission_script_controller(uint32 object);

uint32 game_script_condition_ship_missing(uint32 operand);
uint32 game_script_condition_elapsed_time(uint32 operand);

uint32 game_script_condition_elapsed_time_unsigned(uint32 operand);
uint32 game_script_condition_primary_count_zero(void);
uint32 game_script_condition_secondary_count_zero(void);
uint32 game_script_condition_special_state_one(uint32 operand);
uint32 game_script_condition_destroyed_count(uint32 operand);

uint32 game_script_condition_secondary_event_count(uint32 operand);
uint32 game_script_condition_mode_five(void);
uint32 game_script_condition_mode_six(void);
uint32 game_script_condition_disabled_count(uint32 operand);
sint32 game_script_condition_status(void);
uint32 game_script_condition_no_enabled_objects(void);

uint32 game_script_condition_rings_complete(uint32 operand);

void game_update_mission_completion_delay(uint32 object);
void game_begin_docking_completion(uint32 identifier);
void game_update_docking_camera(uint32 object);
void game_return_without_effects(void);
void game_begin_mission_completion_sequence_one(void);
void game_begin_mission_completion_sequence_two(void);

uint32 game_create_docking_sequence_object(uint32 owner, uint32 side, uint32 up, uint32 forward, uint32 yaw);
void game_update_docking_sequence_object(uint32 object);

uint32 game_request_stream_preset_one(void);
uint32 game_request_stream_preset_two(void);
uint32 game_request_stream_preset_three(void);

void game_adjust_display_position(void);
void game_reserved_leaf(void);

void game_restore_pause_setting_values(void);
void game_draw_pause_settings(void);
void game_leave_pause_settings(void);
void game_accept_pause_settings(void);
void game_snapshot_pause_settings(void);
void game_draw_pause_setting_row(uint32 index, uint32 x, uint32 y, uint32 selected);

void game_update_pause_settings(void);

void game_resume_from_pause_settings(void);
void game_draw_pause_resource_wait(void);
void game_start_pause_resource_wait(void);
void game_cancel_pause_resource_wait(void);
void game_release_resource_job_buffer(void);
uint32 game_poll_resource_job(void);
void game_begin_resource_job(void);

uint32 game_parse_briefing_text(uint32 text, uint32 state);

void game_draw_projected_light_occlusion(uint32 position, uint32 index);
void game_draw_projected_glow(sint32 x, sint32 y, sint32 radius, uint32 center_color, uint32 edge_color);

void game_draw_red_light_occlusion(uint32 position, uint32 index);
void game_draw_violet_light_occlusion(uint32 position, uint32 index);

void game_update_warp_grid(uint32 index);

uint32 game_script_increment_data_count(void);
void game_update_object_mission_boundary(uint32 object);

uint32 game_intersect_segment_with_face(uint32 face, uint32 start, uint32 end);
sint32 game_transform_vector_by_transposed_basis(uint32 matrix, uint32 input, uint32 output);

uint32 game_reset_clock_after_600_ticks(void);

uint32 game_copy_player_adjusted_basis_components(uint32 source, uint32 destination);
uint32 game_get_buffer_f4f80(void);

uint32 game_emit_object_world_spark_native(uint32 object, const VECTOR *position, uint32 color, uint32 size);

void game_emit_projectile_fragments_native(const VECTOR *position, uint32 object, sint32 count, VECTOR *mode);

void game_emit_model_fragments_native(const VECTOR *position, sint32 count, uint32 mode);

void game_emit_velocity_fragments_native(const VECTOR *position, const VECTOR *velocity, uint32 count);

uint32 game_emit_flash_bca38(uint32 x, uint32 y, uint32 z);

void game_update_boundary_autopilot(uint32 object);

void game_return_7a8dc(void);
void game_update_player_end_delay(uint32 object);
void game_return_7a9cc(void);
void game_return_7a9d4(void);
uint32 game_prepare_mission_completion_state(void);
uint32 game_initialize_status_slot_type14(uint32 index);

uint32 game_create_hull_laser_native(uint32 owner, uint32 target, SVECTOR *offset);

void game_update_rotating_laser_object(uint32 object);
void game_return_7e34c(void);

void game_update_player_end_sequence(uint32 object);

sint32 game_run_scene(void);
void game_initialize_particle_tables(void);
uint32 game_initialize_curve_tables(void);

uint32 game_update_target_aim_statistics(uint32 object, uint32 target);
uint32 game_transform_bounded_object_point(uint32 unused, uint32 object, uint32 output);

void game_emit_slow_particles(uint32 count, uint32 x, uint32 y, uint32 z, uint32 vx, uint32 vy, uint32 vz);
uint32 game_clear_particle_activity(void);
uint32 game_append_flash_reference(uint32 reference);

uint32 game_draw_twisted_particle_trail(uint32 trail, uint32 color_shift);

uint32 game_draw_target_lock_reticle(void);

void game_draw_radar(uint32 alternate_origin, uint32 rotation_matrix);

sint32 game_project_camera_point_native(const VECTOR *point, uint32 screen, sint32 *depth);

void game_handle_debris_hit(uint32 object, uint32 position, uint32 velocity, uint32 other);

void game_update_debris_fragment(uint32 object);
void game_emit_debris_expiration_effects(uint32 object);

void game_draw_eight_triangle_effect(uint32 x, uint32 y, sint32 radius, sint32 depth);

void game_draw_debris_fragment(uint32 object);

void game_draw_flight_hud(uint32 cockpit_rotation, uint32 cockpit_transform, uint32 view_rotation);

void game_update_and_draw_streak_list(uint32 record, uint32 camera);
void game_draw_weighted_curve(uint32 points, uint32 camera);
uint32 game_finish_object_action(uint32 object);
uint32 game_update_object_removal_delay(uint32 object);
void game_begin_object_destruction(uint32 object);
void game_remove_object_and_resources(uint32 object);
void game_free_object_auxiliary_buffer(uint32 object);
uint32 game_reset_object_control_state(uint32 object);
uint32 game_allocate_object_state_22c(uint32 object);
uint32 game_initialize_object_control(uint32 object);
uint32 game_initialize_object_control_alternate(uint32 object);
void game_configure_object_subsystems(uint32 object, uint32 configuration);
void game_initialize_object_variant_4498(uint32 object);
void game_initialize_object_variant_4568(uint32 object);
void game_initialize_object_variant_4690(uint32 object);
void game_initialize_object_variant_4768(uint32 object);
void game_initialize_object_variant_4844(uint32 object);
void game_initialize_object_variant_493c(uint32 object);
void game_initialize_object_variant_4a5c(uint32 object);
void game_initialize_object_variant_4b58(uint32 object);
void game_initialize_one_attached_effect(uint32 object, uint32 vertex_a, uint32 vertex_b, uint32 width, uint32 length, uint32 color_a, uint32 color_b, uint32 offset);
void game_initialize_two_attached_effects(uint32 object, uint32 first_a, uint32 first_b, uint32 first_width, uint32 first_length, uint32 first_color_a, uint32 first_color_b, uint32 first_offset, uint32 second_a, uint32 second_b, uint32 second_width, uint32 second_length, uint32 second_color_a, uint32 second_color_b, uint32 second_offset);
void game_initialize_three_attached_effects(uint32 object, uint32 first_a, uint32 first_b, uint32 first_width, uint32 first_length, uint32 first_color_a, uint32 first_color_b, uint32 first_offset, uint32 second_a, uint32 second_b, uint32 second_width, uint32 second_length, uint32 second_color_a, uint32 second_color_b, uint32 second_offset, uint32 third_a, uint32 third_b, uint32 third_width, uint32 third_length, uint32 third_color_a, uint32 third_color_b, uint32 third_offset);
void game_initialize_four_attached_effects(uint32 object, uint32 first_a, uint32 first_b, uint32 first_width, uint32 first_length, uint32 first_color_a, uint32 first_color_b, uint32 first_offset, uint32 second_a, uint32 second_b, uint32 second_width, uint32 second_length, uint32 second_color_a, uint32 second_color_b, uint32 second_offset, uint32 third_a, uint32 third_b, uint32 third_width, uint32 third_length, uint32 third_color_a, uint32 third_color_b, uint32 third_offset, uint32 fourth_a, uint32 fourth_b, uint32 fourth_width, uint32 fourth_length, uint32 fourth_color_a, uint32 fourth_color_b, uint32 fourth_offset);

void game_initialize_object_variant_4c48(uint32 object);

void game_initialize_object_variant_4d5c(uint32 object);

void game_initialize_object_variant_4e74(uint32 object);

void game_initialize_object_variant_4f64(uint32 object);

void game_initialize_object_variant_5054(uint32 object);

uint32 game_initialize_object_variant_5144(uint32 object);

void game_initialize_object_variant_51d4(uint32 object);

void game_initialize_object_variant_52a8(uint32 object);

void game_initialize_object_state_204(uint32 object);

void game_initialize_object_variant_538c(uint32 object);

void game_initialize_carrier_bay_geometry(uint32 object, uint32 configuration);
void game_initialize_carrier_bay_geometry_native(uint32 object, const uint32 *configuration);

void game_add_attack_run(uint32 object, uint32 first_vertex, uint32 last_vertex, uint32 number);

uint32 game_create_linked_object_part(uint32 parent, uint32 previous, uint32 index);
void game_attach_linked_part_effect(uint32 previous, uint32 part, uint32 index);

void game_create_two_linked_object_parts(uint32 parent);

void game_initialize_object_variant_5a90(uint32 object);

void game_initialize_object_variant_5c3c(uint32 object);

void game_initialize_object_variant_5dd8(uint32 object);

void game_initialize_object_variant_5f8c(uint32 object);

void game_initialize_object_variant_6130(uint32 object);

void game_initialize_object_variant_6274(uint32 object);

void game_initialize_object_variant_6450(uint32 object);

void game_initialize_object_variant_662c(uint32 object);

void game_initialize_object_variant_682c(uint32 object);

void game_initialize_object_variant_6a50(uint32 object);

void game_initialize_object_variant_6c2c(uint32 object);

void game_initialize_object_variant_6dc4(uint32 object);

void game_initialize_object_variant_6fc4(uint32 object);

void game_initialize_object_variant_71ec(uint32 object);

void game_initialize_object_variant_73c8(uint32 object);

void game_initialize_object_variant_7554(uint32 object);

void game_initialize_object_variant_76e0(uint32 object);

void game_initialize_object_variant_786c(uint32 object);

void game_initialize_object_variant_79f8(uint32 object);

void game_initialize_object_variant_7b84(uint32 object);

void game_initialize_object_variant_7d10(uint32 object);

void game_initialize_object_variant_7e9c(uint32 object);

void game_initialize_object_variant_8210(uint32 object);

void game_initialize_object_variant_8410(uint32 object);

void game_initialize_object_variant_85c4(uint32 object);

void game_initialize_object_variant_8768(uint32 object);

void game_update_attached_model_object(uint32 object);

void game_handle_attached_model_hit_native(uint32 object, VECTOR *position, VECTOR *vector, uint32 other, uint32 projectile, uint32 kind, uint32 profile);

void game_initialize_object_variant_8944(uint32 object);

void game_initialize_object_variant_8c50(uint32 object);

void game_initialize_object_variant_8e2c(uint32 object);

void game_initialize_object_variant_9030(uint32 object);
void game_initialize_object_variant_923c(uint32 object);
void game_initialize_object_variant_9418(uint32 object);
void game_initialize_object_variant_95b4(uint32 object);
void game_initialize_object_variant_9760(uint32 object);
void game_initialize_object_variant_98ec(uint32 object);
void game_initialize_object_variant_9a78(uint32 object);
void game_initialize_object_variant_9c04(uint32 object);
uint32 game_reset_target_tracking_cache(uint32 object);
void game_begin_target_tracking(uint32 object);
void game_start_target_tracking_maneuver(uint32 object);
void game_update_target_collision_avoidance(uint32 object);
void game_begin_alternate_target_tracking(uint32 object);
uint32 game_copy_attack_run_to_control(uint32 owner, uint32 object, uint32 run);
void game_release_linked_part_effects(uint32 object);
void game_update_large_target_attack(uint32 object);
void game_update_linked_part_constraint(uint32 first, uint32 second, uint32 first_offset, uint32 second_offset, uint32 amplitude);
void game_update_timed_rotating_part(uint32 object);
uint32 game_integrate_linked_part_velocity(uint32 object);
void game_update_linked_part_parent_velocity(uint32 object);
void game_begin_linked_part_breakup(uint32 object);
void game_remove_expired_linked_part(uint32 object);
sint32 game_set_voice_and_overlay_volume(uint32 voice, uint32 volume);
uint32 game_initialize_large_effect_object(uint32 object);
void game_initialize_object_variant_e1e44(uint32 object);
void game_initialize_object_variant_e1fdc(uint32 object);
void game_initialize_asteroid_fragment(uint32 object);
void game_break_asteroid_fragment(uint32 object);
uint32 game_update_asteroid_fragment_motion(uint32 object);
uint32 game_subtract_camera_drift(uint32 value, uint32 amount);
void game_update_transition_camera(uint32 object);
void game_begin_transition_warp_effect(uint32 object, uint32 mode);
void game_finish_transition_with_warp(uint32 object);
void game_create_transition_camera(uint32 parent);
void game_update_transition_approach(uint32 object);
void game_update_transition_warp_controller(uint32 object);
void game_attach_player_to_transition_warp(uint32 object);
void game_update_transition_departure(uint32 object);
void game_reset_transition_object_limits(void);
void game_reset_transition_list_limits(uint32 list);
void game_update_transition_delay(uint32 object);
void game_resume_paused_transition_object(uint32 object);
void game_fire_equipped_mole_missile(uint32 object);
void game_update_flight_training(uint32 object);
void game_announce_training_targets(sint32 count);
void game_update_shield_target_training(uint32 object);
void game_update_missile_target_training(uint32 object);
uint32 front_finish_mode_transition(uint32 object);
void game_render_nearby_patch(uint32 descriptor);
void game_render_interpolated_patch(uint32 descriptor, uint32 camera);
void game_update_target_tracking_ai(uint32 object);

void front_draw_secondary_briefing_text(void);

#endif
