/* Generated from current game and reviewed SDK boundary bindings */
sint32 anim_play_default_movie(void);
sint32 anim_play_movie_with_skip_delay(uint32 path);
sint32 anim_play_movie_without_clear(uint32 path);
void front_clear_vram_regions(uint32 mode);
uint32 front_initialize_video(void);
uint32 front_reset_settings(void);
sint32 front_run(void);
void front_noop_b838(void);
void front_noop_b840(void);
uint32 front_reset_action_word(void);
void front_select_screen(sint32 screen);
void front_append_screen_link(uint32 type, uint32 link);
uint32 front_descriptor_begin_pane(uint32 descriptor);
uint32 front_descriptor_end_pane(uint32 descriptor);
uint32 front_descriptor_font(uint32 descriptor);
uint32 front_descriptor_sprite(uint32 descriptor);
uint32 front_descriptor_highlight_sprite(uint32 descriptor);
uint32 front_descriptor_button(uint32 descriptor);
uint32 front_descriptor_highlight_button(uint32 descriptor);
uint32 front_descriptor_rectangle(uint32 descriptor);
uint32 front_descriptor_highlight_rectangle(uint32 descriptor);
uint32 front_descriptor_string(uint32 descriptor);
uint32 front_descriptor_highlight_string(uint32 descriptor);
uint32 front_descriptor_strings(uint32 descriptor);
uint32 front_descriptor_actions(uint32 descriptor);
uint32 front_descriptor_enter(uint32 descriptor);
uint32 front_descriptor_update(uint32 descriptor);
uint32 front_descriptor_idle(uint32 descriptor);
uint32 front_descriptor_leave(uint32 descriptor);
uint32 front_descriptor_skip(uint32 descriptor);
void front_draw_button_node(uint32 node);
void front_draw_sprite_node(uint32 node);
void front_draw_rectangle_node(uint32 node);
void front_draw_string_node(uint32 node);
void front_draw_screen_list(uint32 list);
uint32 front_find_pane(sint32 action);
void front_free_screen_list(uint32 list);
uint32 front_initialize_screens(void);
void front_store_screen_action(sint32 screen, sint32 action);
uint32 front_load_screen_action(sint32 screen);
sint32 front_next_screen(void);
void front_update_screen(void);
void front_vsync(void);
uint32 front_update_menu_frame(void);
void front_show_still_beea8(void);
void front_show_still_eac00(void);
void front_show_startup_image(void);
void front_intro(void);
void front_initialize_display(void);
void front_close_display(void);
void front_menu_loop(void);
void front_select_initial_campaign_file(void);
void front_noop_f37c(void);
uint32 front_leave_menu(void);
uint32 front_replace_text_byte(uint32 text, uint32 from, uint32 to);
void front_uppercase_text(uint32 text);
void front_build_model_bounds(uint32 model, uint32 bounds);
uint32 front_parse_view_configuration(uint32 source, uint32 size);
uint32 front_update_preview_camera(uint32 object);
uint32 front_init_model_lighting(void);
uint32 front_init_model_preview(void);
uint32 front_start_model_preview(void);
void front_draw_preview_model(uint32 object);
uint32 front_rotate_preview_model(uint32 object);
uint32 front_initialize_ship_view(void);
uint32 front_select_preview_configuration(void);
void front_load_preview_configuration(void);
uint32 front_allocate_model_tables(void);
uint32 front_copy_tmd(uint32 source, uint32 destination, sint32 size);
uint32 front_parse_model_bundle(uint32 source, uint32 first_model);
uint32 front_load_slot_bundle(uint32 slot);
uint32 front_load_preview_models(uint32 slot);
uint32 front_view_begin_ship(void);
uint32 front_view_end_ship(void);
void front_viewer_error(void);
void front_apply_object_rotation(uint32 object);
uint32 front_update_viewer_objects(void);
uint32 front_draw_viewer_objects(void);
uint32 front_draw_selected_mesh(void);
void front_draw_ship_statistics(void);
void front_draw_ship_details(void);
void front_draw_distance_scales(void);
void front_draw_viewer_backgrounds(void);
void front_update_ship_view(void);
void front_update_ship_comparison(void);
void front_select_ship_view(void);
void front_select_ship_comparison(void);
void front_update_ship_screen(void);
void front_free_preview_models(void);
uint32 front_request_next_ship(void);
uint32 front_request_previous_ship(void);
uint32 front_close_ship_screen(void);
uint32 front_finish_mode_transition(uint32 object);
void front_draw_previous_ship_arrow(uint32 x, uint32 y, uint32 active);
void front_draw_next_ship_arrow(uint32 x, uint32 y, uint32 active);
void front_initialize_glyph_widths(void);
sint32 front_character_glyph(uint32 character);
sint32 front_wide_character_glyph(uint32 character);
uint32 front_initialize_character_map(void);
uint32 front_wide_text_width(uint32 text);
void front_free_text_panel(uint32 panel);
void front_push_text_line(uint32 panel, uint32 text);
void front_scroll_text_left(uint32 panel);
void front_scroll_text_right(uint32 panel);
void front_scroll_text_reverse(uint32 panel);
void front_draw_text_panel(uint32 panel);
void front_reset_text_emphasis(void);
uint32 front_text_width_without_brackets(uint32 text);
uint32 front_scan_text_emphasis(uint32 text);
void front_parse_campaign_filename(uint32 text);
void front_configure_background(uint32 columns, uint32 rows);
void front_close_background(void);
void front_initialize_background(void);
void front_draw_background(void);
void front_texture_cache_idle(void);
void front_initialize_text_texture_cache(uint32 x, uint32 y, sint32 row_height);
void front_render_text_texture_cache(void);
void front_cache_text_texture(uint32 record);
void front_draw_surface_sprite(uint32 sprite);
void front_set_surface_texture_page(void);
uint32 front_swap_render_buffers(void);
void front_set_ordering_table_size(uint32 entries);
sint32 front_allocate_render_buffer(uint32 buffer, uint32 capacity);
void front_initialize_render_buffers(uint32 entries, uint32 capacity);
void front_free_render_buffer(uint32 buffer);
void front_close_render_buffers(void);
uint32 front_allocate_screen_state(uint32 screen);
void front_screen_slide_idle(void);
uint32 front_initialize_screen_slide(uint32 screen);
void front_update_screen_slide(uint32 screen);
void front_free_screen_slide(uint32 screen);
uint32 front_initialize_string_fade(uint32 screen);
void front_update_string_fade(uint32 screen);
void front_free_string_fade(uint32 screen);
void front_prepare_card_icons(void);
void front_upload_card_icon(uint32 port, uint32 slot);
uint32 front_open_card_events(void);
uint32 front_close_card_events(void);
uint32 front_wait_event_bank_a(void);
uint32 front_wait_event_bank_a_secondary(void);
uint32 front_wait_event_bank_b(void);
uint32 front_poll_event_bank_b(void);
uint32 front_poll_event_bank_a(void);
uint32 front_refresh_card_status(uint32 port);
uint32 front_erase_card(sint32 channel);
uint32 front_probe_card_sector(sint32 channel);
uint32 front_query_card_state(uint32 port);
uint32 front_wait_card_check_event(void);
uint32 front_request_card_load(uint32 port);
uint32 front_read_card_directory(uint32 port);
uint32 front_save_named_settings(uint32 name, uint32 port, uint32 data);
uint32 front_initialize_card(void);
uint32 front_shutdown_card(void);
void front_run_card_monitor(uint32 enabled);
uint32 front_snapshot_basic_settings(uint32 destination);
uint32 front_snapshot_settings(uint32 destination);
void front_restore_settings(uint32 settings);
uint32 front_prepare_card_monitor(void);
uint32 front_update_card_monitor(void);
void front_append_card_draw_mode(void);
void front_draw_card_directory(void);
void front_draw_card_menu(void);
void front_update_card_selection(void);
void front_initialize_load_selection(void);
void front_update_load_confirmation(void);
uint32 front_request_card_setup(void);
uint32 front_initialize_save_menu(void);
void front_update_save_menu(void);
void front_initialize_card_operation(void);
void front_update_card_copy_menu(void);
void front_reset_card_action(void);
void front_update_card_delete_menu(void);
void front_update_card_selection_for_copy(void);
void front_update_card_selection_for_browse(void);
void front_commit_loaded_settings(void);
uint32 front_load_selected_settings(uint32 destination);
void front_return_without_effects(void);
uint32 front_copy_selected_card_file(void);
sint32 front_erase_selected_card_file(void);
void front_format_selected_card(void);
uint32 front_erase_selected_card_media(void);
uint32 front_probe_selected_card_media(void);
void front_draw_control_diagram(uint32 diagram, uint32 bindings);
void front_apply_control_preset(void);
void front_select_control_viewer_device(void);
uint32 front_update_control_viewer_animation(void);
void front_update_control_preset_menu(void);
void front_initialize_control_preset_menu(void);
void front_enable_control_model_draw(uint32 object);
void front_create_control_viewer_objects(void);
void front_initialize_control_viewer(void);
void front_release_control_viewer(void);
uint32 front_update_control_viewer_objects(void);
void front_assign_control_button_labels(uint32 diagram, uint32 buttons, uint32 action);
void front_assign_control_axis_labels(uint32 diagram, uint32 mode, uint32 index);
void front_prepare_control_diagram(uint32 diagram, uint32 bindings);
void front_update_control_viewer_menu(void);
void front_initialize_control_axis_calibration(void);
void front_apply_control_axis_calibration(void);
void front_draw_control_axis_mode(void);
void front_draw_centered_control_label(uint32 y, uint32 index, uint32 color);
uint32 front_control_calibration_buttons(void);
void front_update_control_axis_calibration(void);
void front_commit_control_axis_calibration(void);
uint32 front_retain_control_viewer(void);
void front_update_connection_label(void);
void front_initialize_mission_briefing(void);
void front_update_mission_briefing(void);
void front_select_mission_briefing_text(void);
void front_reset_mission_briefing(void);
void front_noop_5700(void);
void front_clear_menu_transition(void);
void front_draw_mission_briefing(void);
uint32 front_update_character_picker(void);
void front_initialize_password_menu(void);
void front_noop_68a0(void);
void front_update_password_menu(void);
void front_noop_6bb0(void);
void front_clear_password_confirmation(void);
void front_apply_confirmed_password(void);
void front_commit_screen_position(void);
void front_initialize_screen_position_menu(void);
void front_update_screen_position_menu(void);
void front_apply_display_origin(void);
uint32 front_store_vertical_adjustment(void);
void front_select_pal_display(void);
void front_select_ntsc_display(void);
void front_snapshot_display_options(void);
void front_initialize_display_confirmation(void);
void front_update_display_confirmation(void);
void front_restore_display_options(void);
void front_initialize_sound_options_menu(void);
void front_request_options_audio(void);
void front_update_sound_options_menu(void);
void front_snapshot_sound_options(void);
void front_restore_sound_options(void);
void front_initialize_display_options_menu(void);
void front_update_display_options_menu(void);
void front_commit_display_options(void);
uint32 front_confirm_display_options(void);
void front_play_intro_from_menu(void);
void front_play_requested_intro(void);
uint32 front_request_intro(void);
void front_request_mode_five(void);
void front_request_mode_six(void);
void front_draw_bottom_red_rectangle(void);
void front_request_mode_seven(void);
void front_initialize_diagnostic_overlay(void);
void front_update_diagnostic_overlay(void);
void front_restore_diagnostic_overlay(void);
uint32 front_reset_code_text(void);
void front_pack_settings_bits(uint32 settings);
uint32 front_encode_settings_code(uint32 settings);
uint32 front_apply_settings_code(void);
uint32 front_validate_settings_bits(uint32 bits);
void front_xor_bytes(uint32 bytes, uint32 length, uint32 command);
uint32 front_settings_code_character(uint32 code);
uint32 front_settings_code_value(uint32 character);
void front_apply_named_code(sint32 index);
uint32 front_wrap_briefing_text(uint32 text, uint32 panel);
void front_play_numbered_movie(sint32 number);
void front_initialize_briefing_view(void);
void front_draw_briefing_view(void);
void front_draw_briefing_heap_diagnostics(void);
void front_draw_secondary_briefing_text(void);
uint32 front_retain_briefing_view(void);
void front_free_briefing_view(void);
uint32 front_leave_briefing_view(void);
uint32 front_reset_menu_packet_lists(void);
void front_draw_viewer_controls(void);
uint32 front_update_campaign_access_flags(void);
uint32 front_configure_preview_lighting(uint32 object, uint32 bright);
void front_draw_preview_primitives(uint32 object);
uint32 front_create_preview_camera(void);
void front_draw_celestial_ring(uint32 ring);
uint32 front_count_celestial_descendants(uint32 node);
void front_initialize_celestial_overview(void);
void front_initialize_selected_celestial_system(void);
uint32 front_index_celestial_systems(void);
uint32 front_load_celestial_systems(void);
void front_initialize_system_database(void);
void front_update_system_database(void);
void front_draw_system_database(void);
void front_copy_selected_system_name(void);
uint32 front_retain_system_database(void);
uint32 front_free_system_database(void);
uint32 front_restore_system_database_title(void);
uint32 front_close_system_body_menu(void);
void front_initialize_system_body_menu(void);
void front_draw_system_selection_link(uint32 row_spacing);
void front_update_system_body_menu(void);
uint32 front_initialize_selected_body_view(void);
void front_restore_selected_body_names(void);
void front_update_selected_system_menu(void);
void front_draw_selected_system_menu(void);
uint32 front_finish_selected_body_view(void);
uint32 front_play_system_audio_first(void);
uint32 front_play_system_audio_second(void);
uint32 front_play_system_audio_third(void);
uint32 front_play_body_audio_first(void);
uint32 front_play_body_audio_second(void);
uint32 front_play_body_audio_third(void);
uint32 front_stop_system_audio(void);
uint32 front_initialize_layers(void);
uint32 front_configure_layer(uint32 layer, uint32 mode, uint32 value);
void front_render_begin(void);
void front_initialize_bar_transitions(uint32 duration);
void front_reset_bar_transition(uint32 bar, uint32 target);
uint32 front_start_bar_transition(uint32 bar, uint32 target);
uint32 front_prepare_bar_scale(void);
sint32 front_bar_transition_extent(void);
void front_update_bar_scale(void);
void front_draw_bar_transition_labels(uint32 index);
uint32 front_update_bar_transition(uint32 index);
void front_draw_bar_transitions(uint32 list);
void front_initialize_ranking_statistics(void);
void front_draw_ranking_statistics(void);
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
uint32 front_allocate_cached_text(uint32 packet_capacity, uint32 text_capacity);
void front_free_cached_text(void);
uint32 front_draw_cached_text(uint32 identifier);
void front_initialize_rank_lighting(void);
uint32 front_campaign_rank(void);
void front_initialize_mission_results(void);
void front_initialize_rank_models(uint32 rank);
void front_draw_rank_models(void);
void front_draw_mission_results(void);
void front_free_mission_results(void);
void front_draw_ship_statistics(void);
void front_draw_ship_details(void);
void front_update_ship_view(void);
void front_update_ship_comparison(void);
void front_select_ship_view(void);
void front_select_ship_comparison(void);
uint32 front_poll_event_bank_b(void);
uint32 front_poll_event_bank_a(void);
uint32 front_save_named_settings(uint32 name, uint32 port, uint32 data);
void front_free_cached_text(void);
void front_update_mission_briefing(void);
void front_select_mission_briefing_text(void);
void front_update_string_fade(uint32 screen);
uint32 front_update_menu_frame(void);
void front_release_control_viewer(void);
void front_initialize_display_confirmation(void);
void front_update_display_confirmation(void);
void front_play_requested_intro(void);
void front_update_system_database(void);
uint32 front_restore_system_database_title(void);
void front_initialize_system_body_menu(void);
uint32 front_play_system_audio_first(void);
uint32 front_play_system_audio_second(void);
uint32 front_play_system_audio_third(void);
uint32 front_play_body_audio_first(void);
uint32 front_play_body_audio_second(void);
uint32 front_play_body_audio_third(void);
void front_initialize_database_mission_list(void);
void front_draw_database_mission_list(void);
void front_draw_campaign_map(void);
void front_free_briefing_view(void);
void front_update_ship_screen(void);
uint32 front_erase_selected_card_media(void);
uint32 front_probe_selected_card_media(void);
void front_copy_selected_system_name(void);
void front_snapshot_display_options(void);
uint32 game_initialize_player_weapon_slots(void);
uint32 game_draw_player_view(void);
uint32 game_copy_player_view_transform(void);
uint32 game_initialize_player_objects(void);
void game_free_player_view(void);
void game_update_player(uint32 object);
uint32 game_target_obstructed(uint32 target);
void game_start_damage_state(void);
void game_update_damage_state(uint32 object);
void game_finish_damage_state(uint32 object);
uint32 game_create_temporary_camera(uint32 owner, uint32 duration);
void game_update_temporary_camera(uint32 object);
void game_fail_current_mission(void);
void game_return_7a8dc(void);
void game_update_player_end_delay(uint32 object);
void game_return_7a9cc(void);
void game_return_7a9d4(void);
void game_load_player_models(uint32 player_type);
void game_update_boundary_autopilot(uint32 object);
void game_update_rotating_laser_object(uint32 object);
void game_update_player_end_sequence(uint32 object);
void game_launch_escape_pod_sequence(void);
uint32 game_prepare_mission_completion_state(void);
void game_begin_mission_completion_sequence_one(void);
void game_begin_mission_completion_sequence_two(void);
void game_update_player_control_mode_1(void);
void game_update_player_control_mode_3(void);
void game_update_player_control_mode_2(void);
void game_update_player_control_mode_4(void);
void game_update_player_control_mode_5(void);
void game_update_player_control_mode(void);
void game_begin_object_camera_sequence(uint32 object, uint32 target);
void game_begin_target_effect_camera(uint32 object, uint32 target);
void game_scale_mesh(uint32 mesh, uint32 scale);
uint32 game_relocate_tmd_meshes(uint32 data);
uint32 game_copy_tmd_meshes(uint32 source, uint32 destination, sint32 size);
void game_measure_model_axis_bounds(uint32 model);
sint32 game_measure_mesh_radius(uint32 mesh);
uint32 game_initialize_weapon_profile_12(uint32 slot);
uint32 game_initialize_weapon_profile_16(uint32 slot);
uint32 game_initialize_weapon_profile_14(uint32 slot);
uint32 game_initialize_weapon_profile_10(uint32 slot);
uint32 game_initialize_weapon_profile_1a(uint32 slot);
uint32 game_initialize_weapon_profile_18(uint32 slot);
uint32 game_initialize_weapon_profile_02(uint32 slot);
uint32 game_initialize_mission_weapon_slot(uint32 slot);
uint32 game_initialize_weapon_profile_04(uint32 slot);
uint32 game_initialize_weapon_profile_06(uint32 slot);
uint32 game_initialize_weapon_profile_00(uint32 slot);
uint32 game_initialize_weapon_profile_0e(uint32 slot);
uint32 game_initialize_weapon_profile_0a(uint32 slot);
uint32 game_initialize_weapon_profile_0c(uint32 slot);
uint32 game_initialize_weapon_profile_20(uint32 slot);
uint32 game_initialize_status_slot_type14(uint32 index);
void game_return_7e34c(void);
void game_fire_weapon_profile_12(uint32 slot);
void game_fire_weapon_profile_16(uint32 slot);
void game_fire_weapon_profile_18(uint32 slot);
void game_fire_weapon_profile_10(uint32 slot);
void game_fire_weapon_profile_1a(uint32 slot);
void game_fire_weapon_profile_14(uint32 slot);
void game_fire_weapon_profile_02(uint32 slot);
void game_fire_mission_weapon(uint32 slot);
void game_fire_weapon_profile_06(uint32 slot);
void game_fire_weapon_profile_04(uint32 slot);
void game_fire_weapon_profile_00(uint32 slot);
void game_fire_weapon_profile_0c(uint32 slot);
void game_fire_weapon_profile_0e(uint32 slot);
void game_fire_weapon_profile_0a(uint32 slot);
void game_fire_weapon_profile_20(uint32 slot);
void game_fire_auxiliary_weapon(uint32 slot);
void game_emit_weapon_profile_12(void);
void game_emit_weapon_profile_16(void);
void game_emit_weapon_profile_18(void);
void game_emit_weapon_profile_10(void);
void game_emit_weapon_profile_1a(void);
void game_emit_weapon_profile_14(void);
uint32 game_create_profile_0c_projectile(uint32 owner, uint32 offset);
uint32 game_create_plasma_torpedo(uint32 owner, uint32 target, uint32 offset);
void game_fire_antishield_projectile(uint32 object);
uint32 game_create_stun_missile(uint32 object);
void game_fire_target_decoy(uint32 object);
uint32 game_integrate_projectile(uint32 object);
void game_update_weapon_projectile(uint32 projectile);
void game_update_profile_0c_projectile(uint32 projectile);
void game_split_projectile(uint32 projectile);
void game_update_homing_projectile(uint32 projectile);
void game_update_speed_gated_projectile(uint32 projectile);
void game_update_expiring_projectile(uint32 projectile);
void game_update_auxiliary_projectile(uint32 projectile);
void game_destroy_weapon_projectile(uint32 projectile);
uint32 game_remove_fragmenting_projectile(uint32 projectile);
void game_collide_mission_projectile(uint32 projectile);
void game_destroy_mission_projectile(uint32 projectile);
void game_update_reprogrammed_target(uint32 object);
sint32 game_decrement_nonnegative(sint32 value);
sint32 game_wrap_signed_angle(uint32 angle);
uint32 game_update_object_movement(uint32 object);
sint32 game_set_acceleration_from_angles(uint32 object, uint32 scale);
sint32 game_reduce_without_sign_change(sint32 value, uint32 step);
uint32 game_canonicalize_object_angles(uint32 object);
sint32 game_set_acceleration_from_basis(uint32 object, uint32 scale);
void game_update_object_state(uint32 object);
sint32 game_limit_signed_step(sint32 value, sint32 limit);
uint32 game_step_toward_angle(uint32 target, uint32 current, sint32 limit);
void game_update_view_controller(uint32 object);
uint32 game_create_view_controller(void);
uint32 game_cycle_view_mode(void);
void game_draw_screen_flash(void);
uint32 game_set_screen_tint(uint32 red, uint32 green, uint32 blue);
sint32 game_run_scene(void);
uint32 game_poll_frame_input(void);
void game_initialize_model_bounds(uint32 model);
void game_initialize_weapon_defaults(void);
void game_update_and_draw_scene(void);
void game_stop_scene_stream(void);
void game_shutdown_scene(void);
void game_initialize_model_table(void);
void game_free_scene_models(void);
uint32 game_update_target_aim_statistics(uint32 object, uint32 target);
void game_canonicalize_angle_pair(uint32 first_angle, uint32 second_angle);
void game_evaluate_target_occlusion(uint32 player, uint32 object, uint32 mode);
void game_initialize_object_profile_29a(uint32 object);
void game_begin_mesh_effect_sequence(uint32 object);
uint32 game_release_object_link(uint32 object);
void game_update_object_link(uint32 object);
void game_apply_tractor_beam(uint32 target, uint32 object, uint32 beam);
uint32 game_transform_bounded_object_point(uint32 unused, uint32 object, uint32 output);
void game_draw_message_queue(void);
void game_update_docking_camera(uint32 object);
void game_begin_docking_completion(uint32 identifier);
void game_update_docking_sequence_object(uint32 object);
void game_clear_ship_effect_counters(void);
void game_queue_hud_message(uint32 text);
void game_draw_queued_scene_messages(void);
void game_update_exit_request(void);
sint32 game_request_radio_message(uint32 asset, uint32 text);
uint32 game_request_stream_preset_one(void);
uint32 game_request_stream_preset_two(void);
uint32 game_request_stream_preset_three(void);
void game_process_pause_request(void);
void game_adjust_display_position(void);
void game_pause_menu_leave(void);
void game_reserved_leaf(void);
uint32 game_count_pause_menu_entries(void);
void game_initialize_pause_selection(uint32 menu);
void game_run_pause_menu(void);
void game_draw_pause_options(void);
void game_draw_pause_options_for_menu(uint32 menu);
void game_draw_pause_panel(void);
void game_draw_pause_panel_for_menu(uint32 menu);
void game_draw_pause_heading(void);
void game_draw_pause_heading_for_menu(uint32 menu);
void game_draw_pause_frame(uint32 mode);
void game_update_pause_selection(void);
void game_move_pause_selection(uint32 input, uint32 menu);
void game_handle_pause_menu_input(void);
void game_initialize_pause_grid(void);
void game_draw_pause_warp_grid(void);
void game_draw_pause_warp_grid_values(uint32 fade, uint32 base_red, uint32 texture_depth);
void game_draw_pause_menu(void);
void game_restore_pause_setting_values(void);
void game_update_pause_settings(void);
void game_draw_pause_settings(void);
void game_begin_pause_menu_transition(void);
void game_pause_menu_enter(void);
void game_leave_pause_settings(void);
void game_accept_pause_settings(void);
void game_snapshot_pause_settings(void);
void game_swap_pause_menu_geometry(void);
void game_resume_from_pause_settings(void);
void game_request_pause_mission_exit(void);
void game_swap_pause_menu_return_geometry(void);
void game_prepare_pause_scene(void);
void game_draw_pause_grid_background(void);
void game_draw_pause_ship_preview(void);
void game_draw_pause_resource_wait(void);
void game_start_pause_resource_wait(void);
void game_cancel_pause_resource_wait(void);
void game_initialize_local_starfield(void);
void game_draw_camera_particle_layers(void);
void game_initialize_spherical_starfield(void);
void game_draw_camera_overlay(void);
void game_clear_scene_link_slots(void);
void game_draw_attached_glows(void);
void game_draw_projected_light_occlusion(uint32 position, uint32 index);
void game_draw_red_light_occlusion(uint32 position, uint32 index);
void game_draw_violet_light_occlusion(uint32 position, uint32 index);
void game_draw_view_by_mode(uint32 view, uint32 index);
void game_draw_position_glow(uint32 position, uint32 color, uint32 size);
void game_initialize_scene_grid(void);
void game_update_warp_grid(uint32 index);
sint32 game_allocate_arrival_warp_effect(uint32 position, uint32 matrix, uint32 object);
sint32 game_allocate_warp_effect(uint32 position, uint32 matrix, uint32 object);
void game_draw_active_groups(void);
void game_draw_partial_rotating_mesh(uint32 phase);
void game_orient_radial_mesh(uint32 x, uint32 y, uint32 z);
void game_initialize_radial_mesh(uint32 x, uint32 y, uint32 z);
void game_free_optional_scene_buffer(void);
void game_clear_model_binding_slots(void);
uint32 game_import_resource_bundle(uint32 data, uint32 first);
void game_clear_scene_resource_state(void);
void game_update_flight_controls(uint32 object);
void game_rotate_player_x(uint32 angle);
void game_rotate_player_y(uint32 angle);
void game_rotate_player_z(uint32 angle);
void game_rotate_object_basis(uint32 object);
sint32 game_restore_scene_draw_color(void);
uint32 game_get_controller_state(void);
void game_update_stream_callback(void);
uint32 game_initialize_level_scene(void);
void game_initialize_direction_indicator(void);
void game_refresh_mission_state(void);
void game_config_fire(void);
void game_config_add_jumpgate_objective(void);
void game_config_add_drone_objective(void);
uint32 game_script_increment_data_count(void);
void game_config_objective_text(void);
void game_exit_level_script(void);
void game_assign_ship_task_list(uint32 object, uint32 list);
uint32 game_find_ship_start_callback(uint32 ship);
void game_load_asteroid_bundles(void);
void game_config_navy_defeated_objective(void);
void game_config_league_defeated_objective(void);
void game_config_planet_destroyed_objective(void);
void game_config_begin_task_commands(void);
void game_config_no_navy_fighters_objective(void);
void game_update_object_mission_boundary(uint32 object);
void game_load_level_script(uint32 filename);
sint32 game_lookup_script_task(uint32 text, uint32 table);
void game_apply_script_ship_properties(uint32 object);
void game_tick_script_ship_start(uint32 object);
uint32 game_get_ship_task_parameter(uint32 task, uint32 parameter);
void game_script_ship_attack_error(uint32 object);
void game_script_ship_avoid_error(uint32 object);
void game_script_ship_death_error(uint32 object);
void game_load_bound_bundle(uint32 name);
void game_stop_config_parse(void);
uint32 game_find_model_name(uint32 name);
void game_bind_model_name(uint32 name, uint32 first);
uint32 game_get_variable_address(uint32 name);
uint32 game_initialize_model_name_cursor(void);
uint32 game_relocate_collision_data(uint32 data);
uint32 game_intersect_segment_with_face(uint32 face, uint32 start, uint32 end);
sint32 game_transform_vector_by_transposed_basis(uint32 matrix, uint32 input, uint32 output);
uint32 game_check_player_object_collision(uint32 object);
void game_resolve_object_collision(uint32 object, uint32 obstacle);
uint32 game_check_swept_object_collision(uint32 object, uint32 other);
void game_initialize_demo_input(uint32 recording);
void game_begin_input_recording(void);
void game_finish_demo_input(void);
void game_load_demo_input(uint32 level_name);
uint32 game_reset_clock_after_600_ticks(void);
void game_draw_demo_input_status(void);
void game_reset_demo_selection(void);
uint32 game_ship_task_skip(void);
void game_begin_script_ship_task(uint32 object);
uint32 game_skip_ship_task(uint32 object, uint32 ordinal);
void game_reset_object_control_link(uint32 object);
uint32 game_validate_ship_task_target(uint32 object);
uint32 game_validate_ship_attack_target(uint32 object);
uint32 game_call_ship_death_callback(uint32 object);
void game_update_deferred_ship_launch(uint32 object);
void game_update_actor_flag_delay(uint32 object);
void game_report_tractor_capture(uint32 object, uint32 target);
void game_apply_sensor_damage(uint32 object, sint32 damage);
void game_update_temporary_sensor_damage(uint32 object);
void game_update_sensor_recovery(uint32 controller);
void game_update_permanent_sensor_damage(uint32 object);
void game_create_controller_behind_player(void);
uint32 game_distance_to_nearest_large_ship(uint32 position);
void game_record_destroyed_object(uint32 object, uint32 attacker, uint32 projectile);
void game_check_object_collisions(uint32 object);
void game_process_object_list_contacts(uint32 head, uint32 object, sint32 range);
void game_notify_object_contact(uint32 first, uint32 second);
void game_finish_special_mission_collision(uint32 object);
void game_resolve_special_mission_collision(uint32 first, uint32 second);
void game_collide_scene_object_groups(void);
uint32 game_find_active_ship_by_identifier(uint32 identifier, uint32 category);
void game_reset_object_collision_links(void);
void game_begin_ship_allegiance_change(uint32 object);
void game_replace_ship_for_category(uint32 object, uint32 category);
void game_update_model_transition(uint32 object);
sint32 game_count_active_standard_objects(void);
uint32 game_start_timed_view_effect(uint32 magnitude, uint32 duration);
void game_update_timed_view_effect(uint32 object);
void game_emit_model_fragments(uint32 position, sint32 count, uint32 mode);
void game_emit_velocity_fragments(uint32 position, uint32 velocity, uint32 count);
void game_initialize_configured_objects(void);
void game_update_distant_rotating_model(uint32 object);
uint32 game_adjust_maneuver_offsets(uint32 yaw, uint32 pitch);
void game_update_ship_task_approach(uint32 object);
void game_check_ship_task_approach_target(uint32 object);
void game_begin_ship_task_approach(uint32 object, uint32 range);
void game_tick_ship_task_delay(uint32 object);
void game_move_ship_and_tick_task_delay(uint32 object);
void game_update_ship_task_wait(uint32 object);
void game_initialize_ship_launch(uint32 ship, uint32 carrier, uint32 bay);
void game_update_carrier_ship_launch(uint32 object);
void game_update_ship_point_approach(uint32 object);
void game_begin_ship_offset_follow_maneuver(uint32 object);
void game_update_ship_offset_follow_maneuver(uint32 object);
uint32 game_position_at_object_offset(uint32 object, uint32 offset, uint32 destination);
void game_position_ship_at_carrier_offset(uint32 carrier, uint32 offset, uint32 ship);
void game_update_ship_retreat_task(uint32 object);
void game_update_carrier_ship_docking(uint32 object);
uint32 game_find_nearby_opposing_ship(uint32 object, uint32 range);
void game_update_changed_ship_threats(void);
void game_update_escape_pod_movement(uint32 object);
void game_finalize_ship_removal(uint32 object);
void game_update_object_fade_out(uint32 controller);
void game_draw_fading_object(uint32 object);
void game_update_object_fade_in(uint32 object);
void game_update_belt_asteroid_spawner(uint32 controller);
uint32 game_create_belt_asteroid(uint32 controller);
void game_initialize_asteroid_belt(uint32 controller);
void game_stop_and_launch_escape_pods(uint32 object);
uint32 game_create_escape_pod(uint32 carrier);
void game_draw_and_select_target(uint32 object);
void game_update_tractor_beam(uint32 object);
void game_update_tractor_pull(uint32 object);
void game_update_tractor_bay_retrieval(uint32 object);
uint32 game_find_free_carrier_bay_near_target(uint32 carrier, uint32 target);
void game_update_automatic_object_links(uint32 object);
void game_update_ship_warp_departure(uint32 object);
void game_update_jumpgate_ship_controller(uint32 object);
void game_update_delayed_ship_behavior(uint32 object);
void game_update_ship_entry_placement(uint32 object);
void game_update_ship_timed_departure(uint32 object);
void game_replace_group_member_states(uint32 object, uint32 from_state, uint32 to_state);
void game_update_ship_formation(uint32 object);
void game_spawn_approaching_asteroid(uint32 controller);
void game_reposition_cloaked_ship(uint32 object);
void game_update_ship_random_point(uint32 object);
void game_update_warphole_lifetime(uint32 object);
uint32 game_create_moonbase_grid_controller(uint32 object);
void game_update_warphole_impact(uint32 object);
void game_update_drifting_object(uint32 object);
void game_update_target_beam_sequence(uint32 object);
void game_draw_target_effect_view(uint32 object);
uint32 game_select_carrier_launch_bay(uint32 object);
void game_create_script_controller(void);
void game_update_mission_script_controller(uint32 object);
uint32 game_script_condition_ship_missing(uint32 operand);
uint32 game_script_condition_elapsed_time(uint32 operand);
uint32 game_script_condition_elapsed_time_unsigned(uint32 operand);
uint32 game_script_condition_primary_count_zero(void);
uint32 game_script_condition_secondary_count_zero(void);
uint32 game_script_condition_special_state_one(uint32 operand);
uint32 game_script_condition_destroyed_count(uint32 operand);
uint32 game_script_condition_secondary_event_count(uint32 operand);
uint32 game_script_condition_rings_complete(uint32 operand);
uint32 game_script_condition_mode_five(void);
uint32 game_script_condition_mode_six(void);
uint32 game_script_condition_disabled_count(uint32 operand);
sint32 game_script_condition_status(void);
uint32 game_script_condition_no_enabled_objects(void);
void game_reset_level_script_state(void);
void game_begin_mission_completion(uint32 player);
void game_update_mission_completion_delay(uint32 object);
void game_validate_level_script(void);
uint32 game_update_scene_objects(void);
uint32 game_remove_objects_by_type(uint32 type);
uint32 game_remove_other_objects_of_type(uint32 type, uint32 except);
uint32 game_update_object_removal(uint32 object);
uint32 game_mark_object_for_removal(uint32 object);
void game_finish_scene_objects(void);
uint32 game_count_script_ship_objects(uint32 identifier, uint32 type);
uint32 game_count_matching_active_ship_objects(uint32 reference);
uint32 game_find_nearest_type_one(uint32 object, uint32 excluded_flags);
uint32 game_find_nearest_type_three(uint32 object, uint32 excluded_flags);
uint32 game_count_objects_by_category(uint32 identifier, uint32 category);
uint32 game_find_script_ship_health_below(uint32 identifier, uint32 threshold);
uint32 game_copy_player_adjusted_basis_components(uint32 source, uint32 destination);
uint32 game_get_object_forward(uint32 object, uint32 destination);
uint32 game_create_aimed_projectile(uint32 source, uint32 target, uint32 mode);
uint32 game_get_buffer_f4f80(void);
void game_initialize_particle_tables(void);
void game_clear_particle_records(void);
uint32 game_clear_particle_activity(void);
void game_draw_scene_descriptor_chain(void);
void game_initialize_effect_pool(void);
uint32 game_allocate_effect(uint32 type, uint32 owner);
void game_return_without_effects(void);
void game_update_line_effects(uint32 effect);
void game_update_particle_effects(uint32 effect);
void game_draw_camera_effects(uint32 effect);
void game_update_effect_chain(uint32 effect);
void game_expire_model_effect(uint32 effect);
void game_process_effect_removals(void);
void game_unlink_effect_type(uint32 effect);
void game_mark_effect_for_removal(uint32 effect);
void game_free_effect_payloads(void);
uint32 game_create_scatter_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_emp_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_ki_plasma_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_hull_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_rear_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_antishield_laser(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_12_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_16_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_18_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_14_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_10_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_profile_1a_effect(uint32 owner, uint32 target, uint32 offset);
uint32 game_create_model_effect(uint32 position, sint32 model_index);
void game_update_scene_effects(void);
void game_finish_effect_updates(void);
void game_collide_scene_effects(void);
void game_resolve_sphere_effect_hit(uint32 object, uint32 effect);
void game_resolve_model_effect_hit(uint32 object, uint32 effect);
void game_handle_effect_contact(uint32 effect, uint32 other);
void game_clear_trail_slots(void);
void game_attach_object_trail(uint32 object, uint32 type);
uint32 game_draw_twisted_particle_trail(uint32 trail, uint32 color_shift);
void game_update_object_trails(void);
void game_detach_object_trails(uint32 object);
uint32 game_draw_laser_projectile(uint32 object);
void game_draw_orbiting_projectile_effect(uint32 object);
void game_draw_burst_effect(uint32 object);
void game_draw_projectile_sparks(uint32 object);
void game_draw_projectile_arcs(uint32 object);
void game_draw_pulsing_burst_effect(uint32 object);
uint32 game_draw_target_lock_reticle(void);
uint32 game_draw_horizontal_reticle(uint32 packet);
void game_bind_overlay_effect_textures(void);
uint32 game_pack_palette_color(sint32 red, sint32 green, sint32 blue);
sint32 game_upload_palette_row(uint32 x, uint32 y, uint32 pixels);
void game_initialize_head_models_and_palettes(void);
void game_draw_radar(uint32 alternate_origin, uint32 rotation_matrix);
void game_draw_flight_hud(uint32 cockpit_rotation, uint32 cockpit_transform, uint32 view_rotation);
sint32 game_bind_mesh_effect_texture(uint32 texture, uint32 slot);
uint32 game_initialize_mesh_effect_pool(void);
uint32 game_allocate_flash(uint32 type);
void game_process_mesh_effect_removals(void);
void game_unlink_mesh_effect_type(uint32 effect);
uint32 game_append_flash_reference(uint32 reference);
void game_draw_scene_mesh_effects(void);
void game_draw_mesh_effect_chain(uint32 effect);
void game_emit_fragment_flash(uint32 x, uint32 y, uint32 z);
void game_emit_sized_fragment_flash(uint32 position, uint32 size);
void game_clear_auxiliary_effect_slots(void);
uint32 game_emit_flash_bca38(uint32 x, uint32 y, uint32 z);
void game_emit_projectile_impact(uint32 x, uint32 y, uint32 z);
void game_emit_hit_flash(uint32 x, uint32 y, uint32 z);
void game_draw_expanding_bursts(void);
void game_draw_debris_fragment(uint32 object);
void game_emit_debris_expiration_effects(uint32 object);
void game_update_debris_fragment(uint32 object);
void game_emit_model_breakup(uint32 object);
void game_initialize_fragment_slots(void);
void game_emit_object_debris(uint32 object);
void game_dispatch_destruction_shape(uint32 object);
void game_emit_timed_point_effect(uint32 position, uint32 size);
void game_update_timed_point_effects(uint32 emit_particles);
void game_draw_attached_record(uint32 object, uint32 record, uint32 mode);
void game_draw_attached_record_small(uint32 object, uint32 record);
void game_draw_attached_triangle(uint32 object, uint32 record);
void game_draw_object_details(uint32 object);
void game_draw_engine_effects(uint32 object);
void game_draw_visible_quad_mesh_effect(uint32 object);
sint32 game_normalize_vector_exact_in_place(uint32 vector);
sint32 game_build_effect_palette_variants(void);
void game_initialize_blob_effect_resources(void);
void game_update_camera_particles(void);
sint32 game_expire_attached_effects(uint32 owner);
void game_update_object_particle_slots(void);
void game_update_object_spark_slots(void);
void game_update_scene_particle_effects(void);
void game_process_camera_vector(uint32 position, uint32 parameter);
void game_tick_timed_maneuver(uint32 object);
uint32 game_update_following_roll_maneuver(uint32 object);
void game_update_timed_maneuver(uint32 object);
uint32 game_update_spinning_maneuver(uint32 object);
uint32 game_update_four_phase_turn_maneuver(uint32 object);
sint32 game_begin_four_phase_turn_maneuver(uint32 object, uint32 completion);
uint32 game_update_roll_transition_maneuver(uint32 object);
sint32 game_begin_roll_transition_maneuver(uint32 object, uint32 completion);
void game_start_ai_mode_maneuver(uint32 object, uint32 update, uint32 ticks);
sint32 game_update_pitch_alignment_timer(uint32 object);
uint32 game_initialize_curve_tables(void);
void game_render_nearby_patch(uint32 descriptor);
void game_draw_model_arcs(uint32 object, uint32 first_index, uint32 second_index);
void game_update_selected_object_link(void);
void game_separate_object_overlaps(uint32 object);
void game_draw_descriptor_chain(uint32 descriptors, uint32 camera, uint32 frozen);
void game_process_camera_particle_field(uint32 position, uint32 colors);
void game_update_camera_streaks(uint32 particles, uint32 camera);
void game_update_and_draw_streak_list(uint32 record, uint32 camera);
void game_collide_effect_chains(uint32 effect, uint32 other);
void game_collide_effects_with_object(uint32 effect, uint32 object);
void game_draw_twisted_ring(uint32 descriptor, uint32 first_angle_offset, uint32 second_angle_offset);
void game_draw_annulus(uint32 descriptor);
void game_render_interpolated_patch(uint32 descriptor, uint32 camera);
void game_draw_weighted_curve(uint32 points, uint32 camera);
void game_draw_mesh_effects_from_camera(uint32 effect, uint32 camera);
void game_load_scene_texture_page(void);
void game_initialize_level_script(void);
void game_parse_level_numbers(uint32 text);
void game_measure_level_model_extent(void);
void game_draw_scene_object(uint32 object);
void game_draw_carrier_launch_bay(uint32 carrier, uint32 bay);
uint32 game_finish_object_action(uint32 object);
uint32 game_update_object_removal_delay(uint32 object);
void game_begin_object_destruction(uint32 object);
void game_remove_object_and_resources(uint32 object);
uint32 game_reset_object_control_state(uint32 object);
uint32 game_initialize_object_control(uint32 object);
uint32 game_allocate_object_state_22c(uint32 object);
uint32 game_initialize_object_control_alternate(uint32 object);
void game_initialize_object_variant_4498(uint32 object);
void game_initialize_object_variant_4568(uint32 object);
void game_initialize_object_variant_4690(uint32 object);
void game_initialize_object_variant_4768(uint32 object);
void game_initialize_object_variant_4844(uint32 object);
void game_initialize_object_variant_493c(uint32 object);
void game_initialize_object_variant_4a5c(uint32 object);
void game_initialize_object_variant_4b58(uint32 object);
void game_initialize_object_variant_4c48(uint32 object);
void game_initialize_object_variant_4d5c(uint32 object);
void game_initialize_object_variant_4e74(uint32 object);
void game_initialize_object_variant_4f64(uint32 object);
void game_initialize_object_variant_5054(uint32 object);
uint32 game_initialize_object_variant_5144(uint32 object);
void game_initialize_object_variant_51d4(uint32 object);
void game_initialize_object_variant_52a8(uint32 object);
void game_initialize_object_variant_538c(uint32 object);
void game_initialize_object_state_204(uint32 object);
void game_initialize_carrier_bay_geometry(uint32 object, uint32 configuration);
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
void game_initialize_object_variant_8944(uint32 object);
void game_update_attached_model_object(uint32 object);
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
void game_update_target_maneuver(uint32 object);
void game_update_target_maneuver_angles(uint32 object);
void game_begin_target_tracking(uint32 object);
uint32 game_update_maneuver_bias(uint32 object);
uint32 game_update_maneuver_bias_from_heading(uint32 object, uint32 heading);
void game_update_cached_target_tracking(uint32 object);
void game_start_target_tracking_maneuver(uint32 object);
void game_update_target_tracking_ai(uint32 object);
void game_update_target_collision_avoidance(uint32 object);
uint32 game_reset_target_tracking_cache(uint32 object);
void game_begin_alternate_target_tracking(uint32 object);
uint32 game_copy_attack_run_to_control(uint32 owner, uint32 object, uint32 run);
void game_update_large_target_attack(uint32 object);
void game_update_combat_maneuver(uint32 object);
void game_update_target_weapons(uint32 object);
void game_clear_scene_effect_flags(void);
void game_release_linked_part_effects(uint32 object);
void game_attach_linked_part_effect(uint32 previous, uint32 part, uint32 index);
void game_draw_active_effect_slots(void);
void game_update_timed_rotating_part(uint32 object);
uint32 game_integrate_linked_part_velocity(uint32 object);
void game_update_linked_part_parent_velocity(uint32 object);
void game_begin_linked_part_breakup(uint32 object);
void game_remove_expired_linked_part(uint32 object);
uint32 game_create_linked_object_part(uint32 parent, uint32 previous, uint32 index);
void game_create_two_linked_object_parts(uint32 parent);
void game_position_scene_effects(void);
void game_initialize_scene_engine_voices(void);
void game_stop_auxiliary_scene_voices(void);
void game_suspend_scene_engine_audio(void);
sint32 game_set_voice_and_overlay_volume(uint32 voice, uint32 volume);
void game_update_engine_audio(uint32 controls);
void game_initialize_scene_effect_voices(void);
void game_accumulate_effect_audio(uint32 position, uint32 intensity);
void game_accumulate_effect_audio_from_camera(uint32 position, uint32 intensity, uint32 camera);
void game_update_effect_audio_channel(void);
void game_update_first_nearby_effect_audio(void);
void game_update_second_nearby_effect_audio(void);
sint32 game_update_nearby_effect_audio(void);
void game_mute_scene_effect_voices(void);
void game_stop_scene_voices(void);
void game_initialize_object_variant_e1e44(uint32 object);
void game_initialize_object_variant_e1fdc(uint32 object);
uint32 game_initialize_large_effect_object(uint32 object);
void game_initialize_asteroid_fragment(uint32 object);
void game_break_asteroid_fragment(uint32 object);
uint32 game_update_asteroid_fragment_motion(uint32 object);
uint32 game_create_gradient_strip_set(uint32 first, uint32 second, uint32 third);
void game_initialize_sky_geometry(void);
sint32 game_draw_effect_quad_mesh(uint32 variant);
void game_initialize_sky_resources(void);
void game_free_scene_buffer_banks(void);
void game_free_scene_buffers(void);
void game_draw_environment_mesh(void);
void game_draw_target_effect_field(uint32 phase);
void game_initialize_scene_sky(void);
void game_tick_script_events(void);
void game_create_script_event_controller(void);
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
uint32 game_script_event_elapsed_with_ship(uint32 event);
uint32 game_script_event_ship_primary_depleted(uint32 event);
uint32 game_script_event_ship_flag_count(uint32 event);
uint32 game_script_event_elapsed(uint32 event);
uint32 game_script_event_ship_ratio_reached(uint32 event);
uint32 game_script_event_ship_idle_time(uint32 event);
uint32 game_script_event_type_seven_count(uint32 event);
uint32 game_find_message_ship(uint32 text);
sint32 game_play_condition_message(uint32 event);
uint32 game_script_event_ship_task_flag_count(uint32 event);
uint32 game_script_event_ship_slot_flag(uint32 event);
uint32 game_script_event_category_flag_count(uint32 event);
uint32 game_script_event_ship_count_with_type_three(uint32 event);
uint32 game_check_object_count_message(uint32 event);
uint32 game_check_ring_count_message(uint32 event);
void game_emit_ring_fragments(uint32 object, uint32 position, uint32 mode);
void game_process_ring_crossing(uint32 object, uint32 direction);
void game_draw_paired_glows(void);
void game_update_ring_glow(uint32 object);
uint32 game_test_ring_crossing(uint32 object, uint32 target);
void game_finish_transition_with_warp(uint32 object);
uint32 game_subtract_camera_drift(uint32 value, uint32 amount);
void game_update_transition_camera(uint32 object);
void game_create_transition_camera(uint32 parent);
void game_update_transition_approach(uint32 object);
void game_update_transition_warp_controller(uint32 object);
void game_begin_transition_warp_effect(uint32 object, uint32 mode);
void game_attach_player_to_transition_warp(uint32 object);
void game_update_transition_departure(uint32 object);
uint32 game_reset_player_ring_limits(void);
void game_reset_object_ring_limits(void);
void game_reset_transition_object_limits(void);
void game_reset_transition_list_limits(uint32 list);
void game_update_transition_delay(uint32 object);
void game_update_training_rings(void);
void game_resume_paused_transition_object(uint32 object);
void game_update_flight_training(uint32 object);
void game_announce_training_targets(sint32 count);
void game_update_shield_target_training(uint32 object);
void game_update_missile_target_training(uint32 object);
void game_initialize_training_controller(void);
uint32 game_parse_briefing_text(uint32 text, uint32 state);
void game_begin_resource_job(void);
uint32 game_poll_resource_job(void);
void game_release_resource_job_buffer(void);
void game_fill_credits_records(void);
uint32 game_update_and_draw_credits(void);
void game_initialize_credits_controller(void);
void game_tick_credits_controller(uint32 object);
void game_free_special_mode_buffers(void);
void game_update_weapon_state(void);
void game_free_object_auxiliary_buffer(uint32 object);
void game_configure_object_subsystems(uint32 object, uint32 configuration);
void game_try_fire_hull_lasers(uint32 object);
void game_try_fire_rear_lasers(uint32 object);
void game_try_fire_antishield_lasers(uint32 object);
void game_try_fire_primary_lasers(uint32 object);
void game_try_fire_emp_lasers(uint32 object);
void game_try_fire_ki_plasma_lasers(uint32 object);
void game_try_fire_scatter_lasers(uint32 object);
void game_try_fire_tracker_missile(uint32 object);
void game_try_fire_motion_missile(uint32 object);
void game_try_fire_stun_missile(uint32 object);
void game_fire_equipped_mole_missile(uint32 object);
void game_try_fire_antishield_missile(uint32 object);
void game_try_fire_plasma_torpedo(uint32 object);
void game_try_fire_burst_torpedo(uint32 object);
void game_try_fire_antishield_torpedo(uint32 object);
void game_try_launch_target_decoy(uint32 object);
void game_update_object_automatic_weapons(uint32 object);
uint32 game_is_weapon_target_in_range(uint32 source, uint32 target, uint32 range);
void game_synchronize_missile_cooldowns(uint32 object);
void game_update_boundary_autopilot(uint32 object);
void game_update_rotating_laser_object(uint32 object);
uint32 game_request_stream_preset_two(void);
void game_update_flight_controls(uint32 object);
void game_update_object_mission_boundary(uint32 object);
void game_create_controller_behind_player(void);
void game_emit_velocity_fragments(uint32 position, uint32 velocity, uint32 count);
void game_update_warphole_lifetime(uint32 object);
void game_begin_mission_completion(uint32 player);
uint32 game_draw_target_lock_reticle(void);
void game_draw_flight_hud(uint32 cockpit_rotation, uint32 cockpit_transform, uint32 view_rotation);
void game_draw_attached_record_small(uint32 object, uint32 record);
uint32 game_poll_resource_job(void);
void game_update_docking_camera(uint32 object);
void game_update_view_controller(uint32 object);
void game_render_nearby_patch(uint32 descriptor);
uint32 game_draw_horizontal_reticle(uint32 packet);
uint32 game_draw_player_view(void);
void game_free_player_view(void);
void game_update_player(uint32 object);
void game_fire_weapon_profile_20(uint32 slot);
void game_update_stream_callback(void);
void game_tick_credits_controller(uint32 object);
void game_update_mission_script_controller(uint32 object);
void game_update_pause_settings(void);
uint32 game_script_increment_data_count(void);
void game_update_transition_warp_controller(uint32 object);
void game_update_ring_glow(uint32 object);
void game_update_missile_target_training(uint32 object);
uint32 game_init_defaults(void);
sint32 resource_clear_flags(void);
sint32 sound_init_loops(void);
sint32 sound_stop_loops(void);
void render_init_lights(void);
void texture_load_status(void);
void scene_status_disabled(void);
uint32 object_init(uint32 object);
uint32 render_swap_buffers(void);
void render_present_frame(void);
void scene_present_fade(void);
sint32 scene_stop_loading(void);
void scene_free_resources(void);
uint32 scene_hangar_update(void);
void scene_hangar_draw(void);
void scene_stars_update(void);
uint32 scene_stars_draw(void);
void scene_hangar_frame(void);
void scene_stars_frame(void);
uint32 scene_start_hangar(void);
uint32 scene_start_stars(void);
sint32 level_load_callback(sint32 mode);
void scene_suspend_loading(void);
uint32 scene_resume_loading(void);
sint32 message_init(void);
uint32 input_next_rec(sint32 cancel);
sint32 input_append_rec(uint32 value);
sint32 palette_save_all(void);
sint32 scene_clear_slot_flags(void);
sint32 palette_register(uint32 code);
uint32 palette_find_remap(uint32 code);
void resource_free_bundle(uint32 bundle);
sint32 mesh_build_adjacency(uint32 mesh);
uint32 mesh_import_primitives(uint32 source);
sint32 vector_average_s16(uint32 destination, uint32 first, uint32 second);
sint32 vector_average_u8_pair(uint32 destination, uint32 first, uint32 second);
sint32 triangle_max_edge(uint32 first, uint32 second, uint32 third);
void mesh_render_fixed(uint32 mesh);
void mesh_render_fixed_alternate(uint32 mesh);
uint32 mesh_render_adaptive(uint32 mesh, uint32 mode);
uint32 mesh_render_adaptive_alternate(uint32 mesh, uint32 mode);
void mesh_render_adaptive_single(uint32 mesh);
void object_draw_mesh(uint32 object);
sint32 mesh_rebuild_normals(uint32 mesh);
uint32 model_load_bundle(uint32 name, uint32 page, uint32 clut_x);
sint32 render_set_semi_trans(sint32 enabled);
uint32 vector_pack_xy(uint32 vector);
uint32 mesh_extract_fragment(sint32 selected_count, uint32 selected_faces, uint32 source_mesh);
uint32 mesh_build_bounds(uint32 mesh);
sint32 mesh_load_fragments(uint32 chunk, uint32 output, uint32 source_mesh);
void render_emit_mesh_lit(uint32 mesh);
uint32 render_emit_mesh_wire(uint32 mesh, uint32 color);
uint32 render_emit_mesh_wire_front(uint32 mesh, uint32 color);
void render_subdivide_triangle_medium(uint32 mesh, uint32 face);
void render_subdivide_triangle_large(uint32 mesh, uint32 face);
void render_subdivide_quad_medium(uint32 mesh, uint32 face);
void render_subdivide_quad_large(uint32 mesh, uint32 face);
void gte_save_data(uint32 destination);
void gte_restore_data(uint32 source);
void render_emit_mesh_overlay(uint32 mesh);
void render_emit_mesh_depth(uint32 mesh);
sint32 video_init_front(void);
uint32 game_main_loop(void);
void resource_init_index(void);
sint32 game_init_services(void);
uint32 game_entry(void);
void overlay_check_heap(void);
sint32 render_reset(void);
sint32 game_init_options(void);
void scene_clear_transition(void);
void disc_set_event_result(uint32 result);
uint32 disc_poll_event_result(void);
uint32 disc_get_toc(void);
uint32 disc_file_size_handle(uint32 handle);
uint32 disc_file_start_sector(uint32 handle);
sint32 disc_event_fatal(void);
sint32 disc_read_pair(uint32 sector, uint32 destination);
uint32 disc_count_files(uint32 sector, uint32 buffer);
uint32 disc_build_file_table(uint32 sector, uint32 buffer);
sint32 disc_name_matches(uint32 name, uint32 candidate);
uint32 disc_build_directory_tables(uint32 records, uint32 buffer);
void disc_init_events(void);
uint32 disc_toc_raw_ticks(uint32 position);
sint32 disc_init_directory(sint32 toc_only);
sint32 disc_file_seek_step(uint32 handle, uint32 offset);
sint32 disc_file_seek_span(uint32 handle, uint32 sectors);
uint32 disc_file_close(uint32 handle);
void disc_set_cd_volume(sint16 volume);
uint32 render_select_buffer(uint32 entries);
void render_draw_complete(void);
uint32 render_submit_frame(void);
uint32 render_alloc_buffers(void);
void render_free_buffers(void);
void render_check_lists(void);
uint32 render_draw_centered_sprite(uint32 index, uint32 x, uint32 y);
void draw_set_color_rgb(uint32 red, uint32 green, uint32 blue);
sint32 draw_set_color_packed(sint32 color);
uint32 render_set_blend_mode(uint32 blend, sint32 depth);
uint32 render_init_view(uint32 width, uint32 height, uint32 distance);
uint32 render_init_buffers(void);
uint32 render_set_clear_color(uint32 red, uint32 green, uint32 blue);
uint32 render_set_full_height(uint32 width);
uint32 render_set_width(uint32 width);
uint32 render_set_reduced_height(uint32 width);
void render_set_fog_far(sint32 distance, sint32 projection);
uint32 light_scale_scene(uint32 scale);
uint32 camera_follow_target(void);
uint32 camera_update_modes(void);
void camera_reset_follow(void);
void camera_set_target(uint32 object);
uint32 camera_set_target_pair(uint32 object, uint32 reference);
uint32 camera_set_offset(uint32 vector, uint32 first_parameter, uint32 second_parameter);
uint32 camera_set_look_at_object_offset(uint32 object, uint32 vector);
uint32 camera_set_follow_distance(uint32 distance, uint32 smooth);
uint32 camera_enable_flag20(void);
uint32 camera_set_look_at_point(uint32 vector);
uint32 camera_set_look_at_object(uint32 object);
uint32 camera_set_position(uint32 vector);
uint32 camera_apply_translation(uint32 vector);
uint32 camera_request_snap(void);
void camera_set_up(uint32 x, uint32 y, uint32 z);
uint32 object_lookup(uint32 identifier);
uint32 object_list_prepend(uint32 object, uint32 list);
uint32 object_list_remove(uint32 object, uint32 list);
uint32 object_link_chain(uint32 object);
uint32 object_unlink_chain(uint32 object);
uint32 object_initialize_pool(void);
uint32 object_allocate(uint32 list);
uint32 object_advance_counter_twice(uint32 object);
uint32 object_advance_counter_pair(uint32 object);
void object_noop_first(void);
void object_noop_second(void);
uint32 object_flush_removed(void);
uint32 object_update_all(void);
sint32 sound_init_heap(void);
uint32 sound_init(uint32 mode);
uint32 sound_set_master(uint32 volume);
sint32 sound_select_mode(uint32 mode);
void sound_shutdown(void);
sint32 sound_resume(void);
sint32 sound_stop_all(void);
sint32 sound_mute_voices(void);
sint32 sound_restore_voices(void);
sint32 sound_transfer_vag(uint32 data, uint32 second_data);
sint32 sound_free_sample(uint32 sample);
void sound_tick(void);
sint32 sound_stop_voice(uint32 voice);
sint32 sound_set_voice_left(uint32 voice, uint32 level);
sint32 sound_set_voice_right(uint32 voice, uint32 level);
sint32 sound_set_voice_volume(uint32 voice, uint32 volume);
void sound_set_voice_pitch(uint32 voice, uint32 pitch);
sint32 sound_update_voice(uint32 voice);
sint32 sound_update_voice_mode1(uint32 voice);
sint32 sound_update_voice_mode2(uint32 voice);
sint32 sound_update_voice_mode3(uint32 voice);
sint32 sound_update_voice_mode4(uint32 voice);
uint32 data_swap_endian(uint32 value);
uint32 sound_find_voice(void);
sint32 sound_enable_sample(uint32 index);
void sound_disable_sample(uint32 index);
sint32 sound_reset_voices(void);
sint32 sound_load_samples(uint32 buffer);
void sound_init_level(uint32 variant, uint32 level);
sint32 sound_offset_to_pan(sint32 offset);
sint32 sound_play_at_camera(uint32 position, uint32 sample, uint32 pitch);
sint32 sound_play_distant_at_camera(uint32 position, uint32 sample, uint32 pitch);
sint32 sound_play_sample(uint32 sample);
sint32 sound_play_sample_full(uint32 sample);
sint32 sound_play_sample_pitch(uint32 sample, uint32 pitch);
void sound_set_level(uint32 level);
sint32 sound_set_mode(sint32 mode);
sint32 disc_wait_stream(void);
sint32 stream_init(void);
uint32 stream_reset_all_queues(void);
uint32 stream_stop(void);
uint32 stream_clear_queue(void);
uint32 stream_cancel_current(void);
uint32 stream_request_position(uint32 position, uint32 sectors, uint32 asset);
sint32 disc_check_lid(void);
sint32 disc_draw_notice(uint32 image);
sint32 disc_prompt(void);
sint32 disc_wait_valid(void);
sint32 disc_read_error(void);
uint32 stream_update(void);
sint32 disc_set_filter(uint32 channel);
sint32 disc_start_stream(uint32 position, sint32 mode);
void heap_report_failure(uint32 size);
sint32 heap_init(void);
void heap_free(uint32 address);
uint32 heap_alloc_front(uint32 size);
uint32 heap_alloc_back(uint32 size);
uint32 heap_free_bytes(void);
uint32 heap_coalesce_largest(void);
sint32 pad_init_config(void);
sint32 pad_initialize(void);
uint32 pad_set_axis(sint32 axis, sint32 value, uint32 pad);
sint32 pad_update(uint32 pad);
sint32 pad_update_replay(uint32 pad);
void object_check_model_address(uint32 object);
void object_draw_primitives(uint32 object);
sint32 matrix_from_angles(uint32 matrix, uint32 transform);
uint32 matrix_rotate_x_before(uint32 destination, uint32 source, uint32 angle);
sint32 matrix_shift(uint32 source, uint32 destination, uint32 shift);
uint32 matrix_rotate_y_before(uint32 destination, uint32 source, uint32 angle);
uint32 matrix_rotate_z_before(uint32 destination, uint32 source, uint32 angle);
uint32 matrix_apply_vector(uint32 destination, uint32 matrix, uint32 vector);
uint32 camera_from_transform(uint32 transform);
uint32 camera_from_direction(uint32 object);
uint32 camera_look_at_object_point(uint32 object);
uint32 camera_apply_vertical_scale(void);
void render_set_near_depth(uint32 depth);
void render_set_far_depth(uint32 depth);
uint32 camera_dispatch(void);
uint32 matrix_copy_rotation(uint32 destination, uint32 source);
sint32 vector_cross_fixed(uint32 first, uint32 second, uint32 destination);
sint32 vector_multiply_fixed(uint32 first, uint32 second, uint32 destination);
sint32 vector_dot_fixed(uint32 first, uint32 second);
sint32 vector_dot_s16_s32_raw(uint32 first, uint32 second);
sint32 vector_magnitude(uint32 vector);
sint32 vector_make_unit(uint32 source, uint32 destination);
sint32 matrix_orthonormalize(uint32 matrix);
void matrix_normalize(uint32 matrix, uint32 technique);
uint32 object_extract_angles(uint32 destination, uint32 object);
sint32 object_angles_from_basis(uint32 object, uint32 basis);
sint32 object_select_model(uint32 object, sint32 distance);
uint32 object_draw_visible(uint32 object);
sint32 object_draw_transformed(uint32 object);
sint32 integer_shift_away(sint32 value, uint32 shift);
sint32 integer_sign(sint32 value);
sint32 scalar_abs_wrap(sint32 value);
sint32 random_between(sint32 first, sint32 second);
sint32 math_scale_cos(uint32 scale, uint32 angle);
sint32 math_scale_sin(uint32 scale, uint32 angle);
sint32 angle_cosine(uint32 angle);
sint32 angle_sine(uint32 angle);
sint32 angle_atan2(sint32 y, sint32 x);
sint32 square_root_refined(sint32 value);
sint32 vector_length_approx2(sint32 first, sint32 second);
sint32 vector_length_approx(sint32 x, sint32 y, sint32 z);
sint32 random_signed_between(sint32 first, sint32 second);
sint32 font_init(void);
sint32 text_measure_width(uint32 text);
sint32 text_measure_width_without_brackets(uint32 text);
uint32 font_draw_integer(sint32 value);
uint32 render_insert_primitive(sint32 depth, uint32 packet);
void render_emit_mesh_primitives(uint32 mesh);
void render_emit_mesh_primitives(uint32 mesh);
void render_emit_mesh_primitives(uint32 mesh);
void render_emit_mesh_primitives(uint32 mesh);
void render_emit_mesh_primitives(uint32 mesh);
void config_noop(void);
uint32 config_read_line(uint32 source, uint32 destination);
sint32 table_column_empty(uint32 column);
uint32 tex_alloc_region(sint32 width, sint32 height);
sint32 table_find_rows(sint32 count, sint32 height, sint32 column);
sint32 table_max_rows(uint32 first, sint32 count, uint32 column);
sint32 tex_reset_regions(void);
sint32 table_clear_column(uint32 column);
sint32 video_clear_vram(uint32 left, uint32 right);
sint32 system_enter_critical(void);
void system_exit_critical(void);
uint32 system_pad_start(void);
uint32 system_pad_stop(void);
uint32 system_pad_init_legacy(uint32 mode, uint32 destination);
uint32 system_pad_disable(void);
void card_initialize(uint32 mode);
void system_card_start(void);
void card_stop(void);
uint32 system_card_init(uint32 mode);
uint32 system_card_start_bios(void);
uint32 system_card_stop(void);
void system_card_restore_patch(void);
sint32 system_cd_mix(uint32 volume);
void system_stream_set_mask(uint32 mask, uint32 start, uint32 end);
sint32 system_read_counter_one(void);
sint32 system_reset_counter_one(void);
void system_set_projection_offset(sint32 x, sint32 y);
void system_set_projection_distance(sint32 distance);
void system_spu_init(void);
void system_spu_quit(void);
sint32 system_set_video_mode(sint32 mode);
sint32 system_get_video_mode(void);
sint32 system_cd_read_break(void);
sint32 system_set_graph_reverse(sint32 reverse);
sint32 system_set_graph_queue(sint32 enabled);
uint32 system_get_odd_field(void);
uint32 system_stop_callbacks(void);
uint32 system_restart_callbacks(void);
uint32 render_submit_frame(void);
uint32 render_set_full_height(uint32 width);
uint32 render_set_reduced_height(uint32 width);
uint32 light_scale_scene(uint32 scale);
sint32 sound_select_mode(uint32 mode);
void sound_shutdown(void);
sint32 sound_free_sample(uint32 sample);
sint32 sound_set_voice_left(uint32 voice, uint32 level);
sint32 sound_set_voice_right(uint32 voice, uint32 level);
uint32 stream_request_position(uint32 position, uint32 sectors, uint32 asset);
uint32 stream_update(void);
uint32 object_extract_angles(uint32 destination, uint32 object);

static uint32 game_dispatch_main(uint32 target, uint32 count, uint32 a0, uint32 a1, uint32 a2)
{
    switch (target)
    {
    case 0x80010030:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_init_defaults();
    case 0x800101D0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)resource_clear_flags();
    case 0x80011F4C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_init_loops();
    case 0x8001200C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_stop_loops();
    case 0x8001206C:
        if (count > 3) game_unbound_native_call(target, count);
        render_init_lights();
        return 0;
    case 0x80012188:
        if (count > 3) game_unbound_native_call(target, count);
        texture_load_status();
        return 0;
    case 0x8001231C:
        if (count > 3) game_unbound_native_call(target, count);
        scene_status_disabled();
        return 0;
    case 0x80012530:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_init((uint32)a0);
    case 0x800127C0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)render_swap_buffers();
    case 0x800128B0:
        if (count > 3) game_unbound_native_call(target, count);
        render_present_frame();
        return 0;
    case 0x80012BB4:
        if (count > 3) game_unbound_native_call(target, count);
        scene_present_fade();
        return 0;
    case 0x80012E44:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)scene_stop_loading();
    case 0x80012F0C:
        if (count > 3) game_unbound_native_call(target, count);
        scene_free_resources();
        return 0;
    case 0x80012F6C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)scene_hangar_update();
    case 0x800137D8:
        if (count > 3) game_unbound_native_call(target, count);
        scene_hangar_draw();
        return 0;
    case 0x80013A48:
        if (count > 3) game_unbound_native_call(target, count);
        scene_stars_update();
        return 0;
    case 0x80014354:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)scene_stars_draw();
    case 0x800145FC:
        if (count > 3) game_unbound_native_call(target, count);
        scene_hangar_frame();
        return 0;
    case 0x80014704:
        if (count > 3) game_unbound_native_call(target, count);
        scene_stars_frame();
        return 0;
    case 0x8001480C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)scene_start_hangar();
    case 0x800148F0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)scene_start_stars();
    case 0x800149BC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)level_load_callback((sint32)a0);
    case 0x80014EC0:
        if (count > 3) game_unbound_native_call(target, count);
        scene_suspend_loading();
        return 0;
    case 0x80014F64:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)scene_resume_loading();
    case 0x80014FB0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)message_init();
    case 0x800154C8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)input_next_rec((sint32)a0);
    case 0x80015534:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)input_append_rec((uint32)a0);
    case 0x8001558C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)palette_save_all();
    case 0x80015630:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)scene_clear_slot_flags();
    case 0x80015660:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)palette_register((uint32)a0);
    case 0x80015704:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)palette_find_remap((uint32)a0);
    case 0x8001577C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        resource_free_bundle((uint32)a0);
        return 0;
    case 0x80015888:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)mesh_build_adjacency((uint32)a0);
    case 0x80015BF4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)mesh_import_primitives((uint32)a0);
    case 0x800161C8:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_average_s16((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80016214:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_average_u8_pair((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80016248:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)triangle_max_edge((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80016468:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        mesh_render_fixed((uint32)a0);
        return 0;
    case 0x8001650C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        mesh_render_fixed_alternate((uint32)a0);
        return 0;
    case 0x800165B0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)mesh_render_adaptive((uint32)a0, (uint32)a1);
    case 0x800166D4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)mesh_render_adaptive_alternate((uint32)a0, (uint32)a1);
    case 0x800167F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        mesh_render_adaptive_single((uint32)a0);
        return 0;
    case 0x8001690C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        object_draw_mesh((uint32)a0);
        return 0;
    case 0x80016930:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)mesh_rebuild_normals((uint32)a0);
    case 0x80016BEC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)model_load_bundle((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80016E20:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_set_semi_trans((sint32)a0);
    case 0x80016E58:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_pack_xy((uint32)a0);
    case 0x80016E6C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)mesh_extract_fragment((sint32)a0, (uint32)a1, (uint32)a2);
    case 0x80017D10:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)mesh_build_bounds((uint32)a0);
    case 0x80017F78:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)mesh_load_fragments((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80018094:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_emit_mesh_lit((uint32)a0);
        return 0;
    case 0x80018AA4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_emit_mesh_wire((uint32)a0, (uint32)a1);
    case 0x80018F10:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_emit_mesh_wire_front((uint32)a0, (uint32)a1);
    case 0x800192DC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        render_subdivide_triangle_medium((uint32)a0, (uint32)a1);
        return 0;
    case 0x80019AEC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        render_subdivide_triangle_large((uint32)a0, (uint32)a1);
        return 0;
    case 0x8001B614:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        render_subdivide_quad_medium((uint32)a0, (uint32)a1);
        return 0;
    case 0x8001BF44:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        render_subdivide_quad_large((uint32)a0, (uint32)a1);
        return 0;
    case 0x8001DF64:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        gte_save_data((uint32)a0);
        return 0;
    case 0x8001E048:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        gte_restore_data((uint32)a0);
        return 0;
    case 0x8001E12C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_emit_mesh_overlay((uint32)a0);
        return 0;
    case 0x8001E564:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_emit_mesh_depth((uint32)a0);
        return 0;
    case 0x8001EDC0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)video_init_front();
    case 0x8001EE8C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_main_loop();
    case 0x8001F320:
        if (count > 3) game_unbound_native_call(target, count);
        resource_init_index();
        return 0;
    case 0x8001F440:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_init_services();
    case 0x8001F490:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_entry();
    case 0x8001F758:
        if (count > 3) game_unbound_native_call(target, count);
        overlay_check_heap();
        return 0;
    case 0x8001F970:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)render_reset();
    case 0x8001F9D0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_init_options();
    case 0x8001FAD4:
        if (count > 3) game_unbound_native_call(target, count);
        scene_clear_transition();
        return 0;
    case 0x8001FD04:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        disc_set_event_result((uint32)a0);
        return 0;
    case 0x8001FD14:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_poll_event_result();
    case 0x8001FD4C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_get_toc();
    case 0x8001FD5C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_file_size_handle((uint32)a0);
    case 0x8001FD98:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_file_start_sector((uint32)a0);
    case 0x8001FDD4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_event_fatal();
    case 0x8001FF24:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_read_pair((uint32)a0, (uint32)a1);
    case 0x8001FF98:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_count_files((uint32)a0, (uint32)a1);
    case 0x8002009C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_build_file_table((uint32)a0, (uint32)a1);
    case 0x80020228:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_name_matches((uint32)a0, (uint32)a1);
    case 0x80020264:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_build_directory_tables((uint32)a0, (uint32)a1);
    case 0x80020328:
        if (count > 3) game_unbound_native_call(target, count);
        disc_init_events();
        return 0;
    case 0x800203B4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_toc_raw_ticks((uint32)a0);
    case 0x800203F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_init_directory((sint32)a0);
    case 0x80020B00:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_file_seek_step((uint32)a0, (uint32)a1);
    case 0x80020BC0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_file_seek_span((uint32)a0, (uint32)a1);
    case 0x80020C74:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_file_close((uint32)a0);
    case 0x80020D40:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        disc_set_cd_volume((sint16)a0);
        return 0;
    case 0x80021AE8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_select_buffer((uint32)a0);
    case 0x80021B7C:
        if (count > 3) game_unbound_native_call(target, count);
        render_draw_complete();
        return 0;
    case 0x80021BC4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)render_submit_frame();
    case 0x80021CF4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)render_alloc_buffers();
    case 0x80021D80:
        if (count > 3) game_unbound_native_call(target, count);
        render_free_buffers();
        return 0;
    case 0x80021DAC:
        if (count > 3) game_unbound_native_call(target, count);
        render_check_lists();
        return 0;
    case 0x80021E58:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_draw_centered_sprite((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80021EC8:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        draw_set_color_rgb((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x80021EE8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)draw_set_color_packed((sint32)a0);
    case 0x800221A8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_set_blend_mode((uint32)a0, (sint32)a1);
    case 0x800224BC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_init_view((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8002261C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)render_init_buffers();
    case 0x80022A3C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_set_clear_color((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80022AC0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_set_full_height((uint32)a0);
    case 0x80022B18:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_set_width((uint32)a0);
    case 0x80022B80:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_set_reduced_height((uint32)a0);
    case 0x80023268:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        render_set_fog_far((sint32)a0, (sint32)a1);
        return 0;
    case 0x80023290:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)light_scale_scene((uint32)a0);
    case 0x800236B0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_follow_target();
    case 0x80023B34:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_update_modes();
    case 0x80024308:
        if (count > 3) game_unbound_native_call(target, count);
        camera_reset_follow();
        return 0;
    case 0x80024328:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        camera_set_target((uint32)a0);
        return 0;
    case 0x80024338:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_set_target_pair((uint32)a0, (uint32)a1);
    case 0x80024360:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_set_offset((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800243BC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_set_look_at_object_offset((uint32)a0, (uint32)a1);
    case 0x80024410:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_set_follow_distance((uint32)a0, (uint32)a1);
    case 0x80024438:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_enable_flag20();
    case 0x80024454:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_set_look_at_point((uint32)a0);
    case 0x80024474:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_set_look_at_object((uint32)a0);
    case 0x80024494:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_set_position((uint32)a0);
    case 0x800244D0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_apply_translation((uint32)a0);
    case 0x8002455C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_request_snap();
    case 0x80024570:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        camera_set_up((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x80024590:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_lookup((uint32)a0);
    case 0x800245C0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_list_prepend((uint32)a0, (uint32)a1);
    case 0x80024634:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_list_remove((uint32)a0, (uint32)a1);
    case 0x800246A0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_link_chain((uint32)a0);
    case 0x800246DC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_unlink_chain((uint32)a0);
    case 0x80024734:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)object_initialize_pool();
    case 0x800247F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_allocate((uint32)a0);
    case 0x80024904:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_advance_counter_twice((uint32)a0);
    case 0x80024918:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_advance_counter_pair((uint32)a0);
    case 0x80024934:
        if (count > 3) game_unbound_native_call(target, count);
        object_noop_first();
        return 0;
    case 0x8002493C:
        if (count > 3) game_unbound_native_call(target, count);
        object_noop_second();
        return 0;
    case 0x80024944:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)object_flush_removed();
    case 0x80024A08:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)object_update_all();
    case 0x80024A60:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_init_heap();
    case 0x80024A88:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_init((uint32)a0);
    case 0x80024BF0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_set_master((uint32)a0);
    case 0x80024C1C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_select_mode((uint32)a0);
    case 0x80024C5C:
        if (count > 3) game_unbound_native_call(target, count);
        sound_shutdown();
        return 0;
    case 0x80024C98:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_resume();
    case 0x80024CBC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_stop_all();
    case 0x80024D64:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_mute_voices();
    case 0x80024E44:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_restore_voices();
    case 0x80024F08:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_transfer_vag((uint32)a0, (uint32)a1);
    case 0x80025110:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_free_sample((uint32)a0);
    case 0x800251E4:
        if (count > 3) game_unbound_native_call(target, count);
        sound_tick();
        return 0;
    case 0x8002589C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_stop_voice((uint32)a0);
    case 0x800259E0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_set_voice_left((uint32)a0, (uint32)a1);
    case 0x80025A58:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_set_voice_right((uint32)a0, (uint32)a1);
    case 0x80025AC8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_set_voice_volume((uint32)a0, (uint32)a1);
    case 0x80025B6C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        sound_set_voice_pitch((uint32)a0, (uint32)a1);
        return 0;
    case 0x80025D1C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_update_voice((uint32)a0);
    case 0x80025DB4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_update_voice_mode1((uint32)a0);
    case 0x80025E68:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_update_voice_mode2((uint32)a0);
    case 0x80025F9C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_update_voice_mode3((uint32)a0);
    case 0x80026228:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_update_voice_mode4((uint32)a0);
    case 0x80026500:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)data_swap_endian((uint32)a0);
    case 0x80026528:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_find_voice();
    case 0x800267A0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_enable_sample((uint32)a0);
    case 0x800267BC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        sound_disable_sample((uint32)a0);
        return 0;
    case 0x800267D4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_reset_voices();
    case 0x80026898:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_load_samples((uint32)a0);
    case 0x80026978:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        sound_init_level((uint32)a0, (uint32)a1);
        return 0;
    case 0x80026A34:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_offset_to_pan((sint32)a0);
    case 0x80026D28:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_play_at_camera((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80026FC4:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_play_distant_at_camera((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800274F0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_play_sample((uint32)a0);
    case 0x80027550:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_play_sample_full((uint32)a0);
    case 0x800275A8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_play_sample_pitch((uint32)a0, (uint32)a1);
    case 0x80027604:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        sound_set_level((uint32)a0);
        return 0;
    case 0x80027624:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_set_mode((sint32)a0);
    case 0x80027764:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_wait_stream();
    case 0x800277D0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_init();
    case 0x800278D0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_reset_all_queues();
    case 0x80027934:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_stop();
    case 0x80027A08:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_clear_queue();
    case 0x80027A7C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_cancel_current();
    case 0x800282FC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_request_position((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80028440:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_check_lid();
    case 0x8002848C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_draw_notice((uint32)a0);
    case 0x80028628:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_prompt();
    case 0x80028ADC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_wait_valid();
    case 0x80028C54:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_read_error();
    case 0x80028C94:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_update();
    case 0x80029A94:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_set_filter((uint32)a0);
    case 0x80029AD0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)disc_start_stream((uint32)a0, (sint32)a1);
    case 0x80029B18:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        heap_report_failure((uint32)a0);
        return 0;
    case 0x80029B54:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)heap_init();
    case 0x80029B9C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        heap_free((uint32)a0);
        return 0;
    case 0x80029C40:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)heap_alloc_front((uint32)a0);
    case 0x80029D1C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)heap_alloc_back((uint32)a0);
    case 0x80029E00:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)heap_free_bytes();
    case 0x80029E54:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)heap_coalesce_largest();
    case 0x80029EE4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)pad_init_config();
    case 0x8002A340:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)pad_initialize();
    case 0x8002A480:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)pad_set_axis((sint32)a0, (sint32)a1, (uint32)a2);
    case 0x8002A6C8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)pad_update((uint32)a0);
    case 0x8002B124:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)pad_update_replay((uint32)a0);
    case 0x8002B8AC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        object_check_model_address((uint32)a0);
        return 0;
    case 0x8002B8FC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        object_draw_primitives((uint32)a0);
        return 0;
    case 0x8002B948:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)matrix_from_angles((uint32)a0, (uint32)a1);
    case 0x8002BD50:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)matrix_rotate_x_before((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8002BDDC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)matrix_shift((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8002BEFC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)matrix_rotate_y_before((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8002C014:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)matrix_rotate_z_before((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8002C0A0:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)matrix_apply_vector((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8002C0F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_from_transform((uint32)a0);
    case 0x8002C158:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_from_direction((uint32)a0);
    case 0x8002C348:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_look_at_object_point((uint32)a0);
    case 0x8002C548:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_apply_vertical_scale();
    case 0x8002C6C4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_set_near_depth((uint32)a0);
        return 0;
    case 0x8002C6D4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_set_far_depth((uint32)a0);
        return 0;
    case 0x8002C6E4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)camera_dispatch();
    case 0x8002C7AC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)matrix_copy_rotation((uint32)a0, (uint32)a1);
    case 0x8002C818:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_cross_fixed((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8002C8B8:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_multiply_fixed((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8002C950:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_dot_fixed((uint32)a0, (uint32)a1);
    case 0x8002C9EC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_dot_s16_s32_raw((uint32)a0, (uint32)a1);
    case 0x8002CA34:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_magnitude((uint32)a0);
    case 0x8002CA88:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_make_unit((uint32)a0, (uint32)a1);
    case 0x8002CC08:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)matrix_orthonormalize((uint32)a0);
    case 0x8002CDC0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        matrix_normalize((uint32)a0, (uint32)a1);
        return 0;
    case 0x8002CFCC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_extract_angles((uint32)a0, (uint32)a1);
    case 0x8002D0E4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_angles_from_basis((uint32)a0, (uint32)a1);
    case 0x8002D1F8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_select_model((uint32)a0, (sint32)a1);
    case 0x8002D2BC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_draw_visible((uint32)a0);
    case 0x8002DA68:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_draw_transformed((uint32)a0);
    case 0x8002E048:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)integer_shift_away((sint32)a0, (uint32)a1);
    case 0x8002E09C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)integer_sign((sint32)a0);
    case 0x8002E0B0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)scalar_abs_wrap((sint32)a0);
    case 0x8002E0C8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)random_between((sint32)a0, (sint32)a1);
    case 0x8002E188:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)math_scale_cos((uint32)a0, (uint32)a1);
    case 0x8002E1C0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)math_scale_sin((uint32)a0, (uint32)a1);
    case 0x8002E1F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)angle_cosine((uint32)a0);
    case 0x8002E218:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)angle_sine((uint32)a0);
    case 0x8002E238:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)angle_atan2((sint32)a0, (sint32)a1);
    case 0x8002E258:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)square_root_refined((sint32)a0);
    case 0x8002E2C4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_length_approx2((sint32)a0, (sint32)a1);
    case 0x8002E310:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)vector_length_approx((sint32)a0, (sint32)a1, (sint32)a2);
    case 0x8002E390:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)random_signed_between((sint32)a0, (sint32)a1);
    case 0x8002E3EC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)font_init();
    case 0x8002EF24:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)text_measure_width((uint32)a0);
    case 0x8002EFC4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)text_measure_width_without_brackets((uint32)a0);
    case 0x8002F064:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)font_draw_integer((sint32)a0);
    case 0x8002F0BC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_insert_primitive((sint32)a0, (uint32)a1);
    case 0x8002F1E4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_emit_mesh_primitives((uint32)a0);
        return 0;
    case 0x8002FB30:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_emit_mesh_primitives((uint32)a0);
        return 0;
    case 0x8002FBE8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_emit_mesh_primitives((uint32)a0);
        return 0;
    case 0x8002FCA0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_emit_mesh_primitives((uint32)a0);
        return 0;
    case 0x8002FD58:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        render_emit_mesh_primitives((uint32)a0);
        return 0;
    case 0x8003045C:
        if (count > 3) game_unbound_native_call(target, count);
        config_noop();
        return 0;
    case 0x80030464:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)config_read_line((uint32)a0, (uint32)a1);
    case 0x80030754:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)table_column_empty((uint32)a0);
    case 0x80030794:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)tex_alloc_region((sint32)a0, (sint32)a1);
    case 0x80030968:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)table_find_rows((sint32)a0, (sint32)a1, (sint32)a2);
    case 0x80030A30:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)table_max_rows((uint32)a0, (sint32)a1, (uint32)a2);
    case 0x80030A94:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)tex_reset_regions();
    case 0x80030BF4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)table_clear_column((uint32)a0);
    case 0x80030C64:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)video_clear_vram((uint32)a0, (uint32)a1);
    case 0x8005A280:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_enter_critical();
    case 0x8005A290:
        if (count > 3) game_unbound_native_call(target, count);
        system_exit_critical();
        return 0;
    case 0x8005A648:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_pad_start();
    case 0x8005A658:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_pad_stop();
    case 0x8005A668:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)system_pad_init_legacy((uint32)a0, (uint32)a1);
    case 0x8005A6BC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_pad_disable();
    case 0x8005B50C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        card_initialize((uint32)a0);
        return 0;
    case 0x8005B560:
        if (count > 3) game_unbound_native_call(target, count);
        system_card_start();
        return 0;
    case 0x8005B598:
        if (count > 3) game_unbound_native_call(target, count);
        card_stop();
        return 0;
    case 0x8005B5C8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)system_card_init((uint32)a0);
    case 0x8005B5D8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_card_start_bios();
    case 0x8005B5E8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_card_stop();
    case 0x8005B798:
        if (count > 3) game_unbound_native_call(target, count);
        system_card_restore_patch();
        return 0;
    case 0x8005C7E4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)system_cd_mix((uint32)a0);
    case 0x8005F758:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        system_stream_set_mask((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x80062C30:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_read_counter_one();
    case 0x80062C54:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_reset_counter_one();
    case 0x80063BE0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        system_set_projection_offset((sint32)a0, (sint32)a1);
        return 0;
    case 0x80063C00:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        system_set_projection_distance((sint32)a0);
        return 0;
    case 0x8006F398:
        if (count > 3) game_unbound_native_call(target, count);
        system_spu_init();
        return 0;
    case 0x800700FC:
        if (count > 3) game_unbound_native_call(target, count);
        system_spu_quit();
        return 0;
    case 0x80073374:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)system_set_video_mode((sint32)a0);
    case 0x8007338C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_get_video_mode();
    case 0x8005EE74:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_cd_read_break();
    case 0x8006A804:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)system_set_graph_reverse((sint32)a0);
    case 0x8006A97C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)system_set_graph_queue((sint32)a0);
    case 0x8006B8EC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_get_odd_field();
    case 0x80072988:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_stop_callbacks();
    case 0x800729B8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)system_restart_callbacks();
    case 0x80021BBC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)render_submit_frame();
    case 0x80022AB8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_set_full_height((uint32)a0);
    case 0x80022B78:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)render_set_reduced_height((uint32)a0);
    case 0x80023288:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)light_scale_scene((uint32)a0);
    case 0x80024C14:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_select_mode((uint32)a0);
    case 0x80024C54:
        if (count > 3) game_unbound_native_call(target, count);
        sound_shutdown();
        return 0;
    case 0x80025108:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_free_sample((uint32)a0);
    case 0x800259D8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_set_voice_left((uint32)a0, (uint32)a1);
    case 0x80025A50:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)sound_set_voice_right((uint32)a0, (uint32)a1);
    case 0x800282F4:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_request_position((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80028C8C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)stream_update();
    case 0x8002CFC4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)object_extract_angles((uint32)a0, (uint32)a1);
    default:
        game_unbound_native_call(target, count);
        return 0;
    }
}

static uint32 game_dispatch_front(uint32 target, uint32 count, uint32 a0, uint32 a1, uint32 a2)
{
    switch (target)
    {
    case 0x8008AF70:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_clear_vram_regions((uint32)a0);
        return 0;
    case 0x8008B0BC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_video();
    case 0x8008B154:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_reset_settings();
    case 0x8008B2D0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_run();
    case 0x8008B838:
        if (count > 3) game_unbound_native_call(target, count);
        front_noop_b838();
        return 0;
    case 0x8008B840:
        if (count > 3) game_unbound_native_call(target, count);
        front_noop_b840();
        return 0;
    case 0x8008B848:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_reset_action_word();
    case 0x8008B85C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_select_screen((sint32)a0);
        return 0;
    case 0x8008BC24:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_append_screen_link((uint32)a0, (uint32)a1);
        return 0;
    case 0x8008BD74:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_begin_pane((uint32)a0);
    case 0x8008BEB4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_end_pane((uint32)a0);
    case 0x8008BEC4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_font((uint32)a0);
    case 0x8008BF08:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_sprite((uint32)a0);
    case 0x8008BFB0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_highlight_sprite((uint32)a0);
    case 0x8008C084:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_button((uint32)a0);
    case 0x8008C138:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_highlight_button((uint32)a0);
    case 0x8008C218:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_rectangle((uint32)a0);
    case 0x8008C2CC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_highlight_rectangle((uint32)a0);
    case 0x8008C3AC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_string((uint32)a0);
    case 0x8008C488:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_highlight_string((uint32)a0);
    case 0x8008C58C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_strings((uint32)a0);
    case 0x8008C5D4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_actions((uint32)a0);
    case 0x8008C60C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_enter((uint32)a0);
    case 0x8008C644:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_update((uint32)a0);
    case 0x8008C67C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_idle((uint32)a0);
    case 0x8008C6B4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_leave((uint32)a0);
    case 0x8008C6EC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_descriptor_skip((uint32)a0);
    case 0x8008C6F4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_button_node((uint32)a0);
        return 0;
    case 0x8008CD3C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_sprite_node((uint32)a0);
        return 0;
    case 0x8008CFB4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_rectangle_node((uint32)a0);
        return 0;
    case 0x8008D170:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_string_node((uint32)a0);
        return 0;
    case 0x8008D2E0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_screen_list((uint32)a0);
        return 0;
    case 0x8008D3DC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_find_pane((sint32)a0);
    case 0x8008D418:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_free_screen_list((uint32)a0);
        return 0;
    case 0x8008D4C0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_screens();
    case 0x8008D4E8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_store_screen_action((sint32)a0, (sint32)a1);
        return 0;
    case 0x8008D508:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_load_screen_action((sint32)a0);
    case 0x8008D534:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_next_screen();
    case 0x8008D864:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_screen();
        return 0;
    case 0x8008DA54:
        if (count > 3) game_unbound_native_call(target, count);
        front_vsync();
        return 0;
    case 0x8008DC5C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_menu_frame();
    case 0x8008DFE4:
        if (count > 3) game_unbound_native_call(target, count);
        front_show_still_beea8();
        return 0;
    case 0x8008E00C:
        if (count > 3) game_unbound_native_call(target, count);
        front_show_still_eac00();
        return 0;
    case 0x8008E034:
        if (count > 3) game_unbound_native_call(target, count);
        front_show_startup_image();
        return 0;
    case 0x8008E05C:
        if (count > 3) game_unbound_native_call(target, count);
        front_intro();
        return 0;
    case 0x8008E27C:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_display();
        return 0;
    case 0x8008E480:
        if (count > 3) game_unbound_native_call(target, count);
        front_close_display();
        return 0;
    case 0x8008E504:
        if (count > 3) game_unbound_native_call(target, count);
        front_menu_loop();
        return 0;
    case 0x8008F1F4:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_initial_campaign_file();
        return 0;
    case 0x8008F37C:
        if (count > 3) game_unbound_native_call(target, count);
        front_noop_f37c();
        return 0;
    case 0x8008F384:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_leave_menu();
    case 0x8008F4DC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_replace_text_byte((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8008F518:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_uppercase_text((uint32)a0);
        return 0;
    case 0x8008F56C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_build_model_bounds((uint32)a0, (uint32)a1);
        return 0;
    case 0x8008FA94:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_parse_view_configuration((uint32)a0, (uint32)a1);
    case 0x8008FC10:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_preview_camera((uint32)a0);
    case 0x8008FDF8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_init_model_lighting();
    case 0x8008FEC8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_init_model_preview();
    case 0x80090124:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_start_model_preview();
    case 0x80090144:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_preview_model((uint32)a0);
        return 0;
    case 0x80090274:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_rotate_preview_model((uint32)a0);
    case 0x80090338:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_ship_view();
    case 0x80090BD0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_select_preview_configuration();
    case 0x80090C2C:
        if (count > 3) game_unbound_native_call(target, count);
        front_load_preview_configuration();
        return 0;
    case 0x800911EC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_allocate_model_tables();
    case 0x80091254:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_copy_tmd((uint32)a0, (uint32)a1, (sint32)a2);
    case 0x800914BC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_parse_model_bundle((uint32)a0, (uint32)a1);
    case 0x80091740:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_load_slot_bundle((uint32)a0);
    case 0x80091864:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_load_preview_models((uint32)a0);
    case 0x80091AC4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_view_begin_ship();
    case 0x800923B0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_view_end_ship();
    case 0x80092458:
        if (count > 3) game_unbound_native_call(target, count);
        front_viewer_error();
        return 0;
    case 0x80092488:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_apply_object_rotation((uint32)a0);
        return 0;
    case 0x80092610:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_viewer_objects();
    case 0x80092878:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_draw_viewer_objects();
    case 0x80092928:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_draw_selected_mesh();
    case 0x80092A8C:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_ship_statistics();
        return 0;
    case 0x800931C0:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_ship_details();
        return 0;
    case 0x800937D8:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_distance_scales();
        return 0;
    case 0x80093B24:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_viewer_backgrounds();
        return 0;
    case 0x80094038:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_ship_view();
        return 0;
    case 0x8009456C:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_ship_comparison();
        return 0;
    case 0x800947D4:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_ship_view();
        return 0;
    case 0x80094D94:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_ship_comparison();
        return 0;
    case 0x8009565C:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_ship_screen();
        return 0;
    case 0x80095A70:
        if (count > 3) game_unbound_native_call(target, count);
        front_free_preview_models();
        return 0;
    case 0x80095B48:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_request_next_ship();
    case 0x80095B68:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_request_previous_ship();
    case 0x80095B88:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_close_ship_screen();
    case 0x80095D5C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_finish_mode_transition((uint32)a0);
    case 0x80095D90:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        front_draw_previous_ship_arrow((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800962AC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        front_draw_next_ship_arrow((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800967D4:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_glyph_widths();
        return 0;
    case 0x8009683C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_character_glyph((uint32)a0);
    case 0x80096990:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_wide_character_glyph((uint32)a0);
    case 0x80096AB4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_character_map();
    case 0x80096B84:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_wide_text_width((uint32)a0);
    case 0x80097FF8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_free_text_panel((uint32)a0);
        return 0;
    case 0x8009822C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_push_text_line((uint32)a0, (uint32)a1);
        return 0;
    case 0x8009830C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_scroll_text_left((uint32)a0);
        return 0;
    case 0x800985AC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_scroll_text_right((uint32)a0);
        return 0;
    case 0x80098914:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_scroll_text_reverse((uint32)a0);
        return 0;
    case 0x80098C10:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_text_panel((uint32)a0);
        return 0;
    case 0x80099360:
        if (count > 3) game_unbound_native_call(target, count);
        front_reset_text_emphasis();
        return 0;
    case 0x80099370:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_text_width_without_brackets((uint32)a0);
    case 0x80099408:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_scan_text_emphasis((uint32)a0);
    case 0x8009A354:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_parse_campaign_filename((uint32)a0);
        return 0;
    case 0x8009A550:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_configure_background((uint32)a0, (uint32)a1);
        return 0;
    case 0x8009A844:
        if (count > 3) game_unbound_native_call(target, count);
        front_close_background();
        return 0;
    case 0x8009A89C:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_background();
        return 0;
    case 0x8009AF20:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_background();
        return 0;
    case 0x8009B3A0:
        if (count > 3) game_unbound_native_call(target, count);
        front_texture_cache_idle();
        return 0;
    case 0x8009B3A8:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        front_initialize_text_texture_cache((uint32)a0, (uint32)a1, (sint32)a2);
        return 0;
    case 0x8009B478:
        if (count > 3) game_unbound_native_call(target, count);
        front_render_text_texture_cache();
        return 0;
    case 0x8009B56C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_cache_text_texture((uint32)a0);
        return 0;
    case 0x8009B884:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_surface_sprite((uint32)a0);
        return 0;
    case 0x8009B96C:
        if (count > 3) game_unbound_native_call(target, count);
        front_set_surface_texture_page();
        return 0;
    case 0x8009B9EC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_swap_render_buffers();
    case 0x8009BA60:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_set_ordering_table_size((uint32)a0);
        return 0;
    case 0x8009BA74:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_allocate_render_buffer((uint32)a0, (uint32)a1);
    case 0x8009BAB8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_initialize_render_buffers((uint32)a0, (uint32)a1);
        return 0;
    case 0x8009BBA4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_free_render_buffer((uint32)a0);
        return 0;
    case 0x8009BBD8:
        if (count > 3) game_unbound_native_call(target, count);
        front_close_render_buffers();
        return 0;
    case 0x8009BD1C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_allocate_screen_state((uint32)a0);
    case 0x8009BDC0:
        if (count > 3) game_unbound_native_call(target, count);
        front_screen_slide_idle();
        return 0;
    case 0x8009BDC8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_screen_slide((uint32)a0);
    case 0x8009BECC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_update_screen_slide((uint32)a0);
        return 0;
    case 0x8009C518:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_free_screen_slide((uint32)a0);
        return 0;
    case 0x8009C554:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_string_fade((uint32)a0);
    case 0x8009C7A0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_update_string_fade((uint32)a0);
        return 0;
    case 0x8009C968:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_free_string_fade((uint32)a0);
        return 0;
    case 0x8009C9CC:
        if (count > 3) game_unbound_native_call(target, count);
        front_prepare_card_icons();
        return 0;
    case 0x8009CD00:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_upload_card_icon((uint32)a0, (uint32)a1);
        return 0;
    case 0x8009CE80:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_open_card_events();
    case 0x8009D030:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_close_card_events();
    case 0x8009D0C8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_wait_event_bank_a();
    case 0x8009D14C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_wait_event_bank_a_secondary();
    case 0x8009D1D0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_wait_event_bank_b();
    case 0x8009D25C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_poll_event_bank_b();
    case 0x8009D2B4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_poll_event_bank_a();
    case 0x8009D304:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_refresh_card_status((uint32)a0);
    case 0x8009D460:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_erase_card((sint32)a0);
    case 0x8009D4F0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_probe_card_sector((sint32)a0);
    case 0x8009D548:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_query_card_state((uint32)a0);
    case 0x8009D624:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_wait_card_check_event();
    case 0x8009D6A8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_request_card_load((uint32)a0);
    case 0x8009D97C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_read_card_directory((uint32)a0);
    case 0x8009DD98:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_save_named_settings((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8009E034:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_card();
    case 0x8009E084:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_shutdown_card();
    case 0x8009E0A4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_run_card_monitor((uint32)a0);
        return 0;
    case 0x8009E0E8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_snapshot_basic_settings((uint32)a0);
    case 0x8009E27C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_snapshot_settings((uint32)a0);
    case 0x8009E50C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_restore_settings((uint32)a0);
        return 0;
    case 0x8009E880:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_prepare_card_monitor();
    case 0x8009E960:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_card_monitor();
    case 0x8009EA38:
        if (count > 3) game_unbound_native_call(target, count);
        front_append_card_draw_mode();
        return 0;
    case 0x8009EEFC:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_card_directory();
        return 0;
    case 0x8009F608:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_card_menu();
        return 0;
    case 0x8009F7B0:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_card_selection();
        return 0;
    case 0x8009FF04:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_load_selection();
        return 0;
    case 0x800A005C:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_load_confirmation();
        return 0;
    case 0x800A0474:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_request_card_setup();
    case 0x800A0488:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_save_menu();
    case 0x800A0568:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_save_menu();
        return 0;
    case 0x800A1580:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_card_operation();
        return 0;
    case 0x800A15D8:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_card_copy_menu();
        return 0;
    case 0x800A1EE0:
        if (count > 3) game_unbound_native_call(target, count);
        front_reset_card_action();
        return 0;
    case 0x800A1F38:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_card_delete_menu();
        return 0;
    case 0x800A25D4:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_card_selection_for_copy();
        return 0;
    case 0x800A260C:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_card_selection_for_browse();
        return 0;
    case 0x800A2644:
        if (count > 3) game_unbound_native_call(target, count);
        front_commit_loaded_settings();
        return 0;
    case 0x800A26A0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_load_selected_settings((uint32)a0);
    case 0x800A27E8:
        if (count > 3) game_unbound_native_call(target, count);
        front_return_without_effects();
        return 0;
    case 0x800A27F0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_copy_selected_card_file();
    case 0x800A296C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_erase_selected_card_file();
    case 0x800A29D4:
        if (count > 3) game_unbound_native_call(target, count);
        front_format_selected_card();
        return 0;
    case 0x800A2A8C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_erase_selected_card_media();
    case 0x800A2AD0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_probe_selected_card_media();
    case 0x800A2B0C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_draw_control_diagram((uint32)a0, (uint32)a1);
        return 0;
    case 0x800A2E28:
        if (count > 3) game_unbound_native_call(target, count);
        front_apply_control_preset();
        return 0;
    case 0x800A31A8:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_control_viewer_device();
        return 0;
    case 0x800A3418:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_control_viewer_animation();
    case 0x800A3738:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_control_preset_menu();
        return 0;
    case 0x800A3AE0:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_control_preset_menu();
        return 0;
    case 0x800A3B44:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_enable_control_model_draw((uint32)a0);
        return 0;
    case 0x800A3B58:
        if (count > 3) game_unbound_native_call(target, count);
        front_create_control_viewer_objects();
        return 0;
    case 0x800A3CE0:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_control_viewer();
        return 0;
    case 0x800A3E08:
        if (count > 3) game_unbound_native_call(target, count);
        front_release_control_viewer();
        return 0;
    case 0x800A3E3C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_control_viewer_objects();
    case 0x800A3FAC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        front_assign_control_button_labels((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800A4030:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        front_assign_control_axis_labels((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800A4120:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_prepare_control_diagram((uint32)a0, (uint32)a1);
        return 0;
    case 0x800A4224:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_control_viewer_menu();
        return 0;
    case 0x800A4440:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_control_axis_calibration();
        return 0;
    case 0x800A44F0:
        if (count > 3) game_unbound_native_call(target, count);
        front_apply_control_axis_calibration();
        return 0;
    case 0x800A4B64:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_control_axis_mode();
        return 0;
    case 0x800A4C6C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        front_draw_centered_control_label((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800A4D04:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_control_calibration_buttons();
    case 0x800A4D4C:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_control_axis_calibration();
        return 0;
    case 0x800A52C8:
        if (count > 3) game_unbound_native_call(target, count);
        front_commit_control_axis_calibration();
        return 0;
    case 0x800A52E8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_retain_control_viewer();
    case 0x800A52FC:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_connection_label();
        return 0;
    case 0x800A5380:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_mission_briefing();
        return 0;
    case 0x800A54CC:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_mission_briefing();
        return 0;
    case 0x800A5548:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_mission_briefing_text();
        return 0;
    case 0x800A5638:
        if (count > 3) game_unbound_native_call(target, count);
        front_reset_mission_briefing();
        return 0;
    case 0x800A5700:
        if (count > 3) game_unbound_native_call(target, count);
        front_noop_5700();
        return 0;
    case 0x800A5708:
        if (count > 3) game_unbound_native_call(target, count);
        front_clear_menu_transition();
        return 0;
    case 0x800A5718:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_mission_briefing();
        return 0;
    case 0x800A5BA4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_character_picker();
    case 0x800A6760:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_password_menu();
        return 0;
    case 0x800A68A0:
        if (count > 3) game_unbound_native_call(target, count);
        front_noop_68a0();
        return 0;
    case 0x800A68A8:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_password_menu();
        return 0;
    case 0x800A6BB0:
        if (count > 3) game_unbound_native_call(target, count);
        front_noop_6bb0();
        return 0;
    case 0x800A6BB8:
        if (count > 3) game_unbound_native_call(target, count);
        front_clear_password_confirmation();
        return 0;
    case 0x800A6BC8:
        if (count > 3) game_unbound_native_call(target, count);
        front_apply_confirmed_password();
        return 0;
    case 0x800A6C04:
        if (count > 3) game_unbound_native_call(target, count);
        front_commit_screen_position();
        return 0;
    case 0x800A6C64:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_screen_position_menu();
        return 0;
    case 0x800A6D18:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_screen_position_menu();
        return 0;
    case 0x800A6E94:
        if (count > 3) game_unbound_native_call(target, count);
        front_apply_display_origin();
        return 0;
    case 0x800A6EE4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_store_vertical_adjustment();
    case 0x800A6F04:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_pal_display();
        return 0;
    case 0x800A729C:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_ntsc_display();
        return 0;
    case 0x800A7608:
        if (count > 3) game_unbound_native_call(target, count);
        front_snapshot_display_options();
        return 0;
    case 0x800A7670:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_display_confirmation();
        return 0;
    case 0x800A76E8:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_display_confirmation();
        return 0;
    case 0x800A79D8:
        if (count > 3) game_unbound_native_call(target, count);
        front_restore_display_options();
        return 0;
    case 0x800A7A40:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_sound_options_menu();
        return 0;
    case 0x800A7C3C:
        if (count > 3) game_unbound_native_call(target, count);
        front_request_options_audio();
        return 0;
    case 0x800A7D94:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_sound_options_menu();
        return 0;
    case 0x800A8860:
        if (count > 3) game_unbound_native_call(target, count);
        front_snapshot_sound_options();
        return 0;
    case 0x800A88A8:
        if (count > 3) game_unbound_native_call(target, count);
        front_restore_sound_options();
        return 0;
    case 0x800A89D0:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_display_options_menu();
        return 0;
    case 0x800A8BF4:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_display_options_menu();
        return 0;
    case 0x800A9364:
        if (count > 3) game_unbound_native_call(target, count);
        front_commit_display_options();
        return 0;
    case 0x800A939C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_confirm_display_options();
    case 0x800A96B4:
        if (count > 3) game_unbound_native_call(target, count);
        front_play_intro_from_menu();
        return 0;
    case 0x800A96E4:
        if (count > 3) game_unbound_native_call(target, count);
        front_play_requested_intro();
        return 0;
    case 0x800A9710:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_request_intro();
    case 0x800A9724:
        if (count > 3) game_unbound_native_call(target, count);
        front_request_mode_five();
        return 0;
    case 0x800A9768:
        if (count > 3) game_unbound_native_call(target, count);
        front_request_mode_six();
        return 0;
    case 0x800A9AC4:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_bottom_red_rectangle();
        return 0;
    case 0x800A9F14:
        if (count > 3) game_unbound_native_call(target, count);
        front_request_mode_seven();
        return 0;
    case 0x800A9F64:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_diagnostic_overlay();
        return 0;
    case 0x800AA298:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_diagnostic_overlay();
        return 0;
    case 0x800AADE0:
        if (count > 3) game_unbound_native_call(target, count);
        front_restore_diagnostic_overlay();
        return 0;
    case 0x800AAEB8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_reset_code_text();
    case 0x800AAEF8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_pack_settings_bits((uint32)a0);
        return 0;
    case 0x800AB578:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_encode_settings_code((uint32)a0);
    case 0x800AB94C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_apply_settings_code();
    case 0x800AC0C8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_validate_settings_bits((uint32)a0);
    case 0x800AC1EC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        front_xor_bytes((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800AC3A4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_settings_code_character((uint32)a0);
    case 0x800AC3F4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_settings_code_value((uint32)a0);
    case 0x800AC58C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_apply_named_code((sint32)a0);
        return 0;
    case 0x800AC668:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_wrap_briefing_text((uint32)a0, (uint32)a1);
    case 0x800AC880:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_play_numbered_movie((sint32)a0);
        return 0;
    case 0x800AC8D8:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_briefing_view();
        return 0;
    case 0x800AD0C8:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_briefing_view();
        return 0;
    case 0x800ADA4C:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_briefing_heap_diagnostics();
        return 0;
    case 0x800ADACC:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_secondary_briefing_text();
        return 0;
    case 0x800ADBE8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_retain_briefing_view();
    case 0x800ADC1C:
        if (count > 3) game_unbound_native_call(target, count);
        front_free_briefing_view();
        return 0;
    case 0x800ADCB0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_leave_briefing_view();
    case 0x800ADD28:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_reset_menu_packet_lists();
    case 0x800ADD68:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_viewer_controls();
        return 0;
    case 0x800AE208:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_campaign_access_flags();
    case 0x800AE318:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_configure_preview_lighting((uint32)a0, (uint32)a1);
    case 0x800AE3F0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_preview_primitives((uint32)a0);
        return 0;
    case 0x800AE410:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_create_preview_camera();
    case 0x800AF4E0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_celestial_ring((uint32)a0);
        return 0;
    case 0x800AF8F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_count_celestial_descendants((uint32)a0);
    case 0x800AF98C:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_celestial_overview();
        return 0;
    case 0x800AFD54:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_selected_celestial_system();
        return 0;
    case 0x800B023C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_index_celestial_systems();
    case 0x800B033C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_load_celestial_systems();
    case 0x800B03E0:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_system_database();
        return 0;
    case 0x800B0638:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_system_database();
        return 0;
    case 0x800B078C:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_system_database();
        return 0;
    case 0x800B084C:
        if (count > 3) game_unbound_native_call(target, count);
        front_copy_selected_system_name();
        return 0;
    case 0x800B0890:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_retain_system_database();
    case 0x800B08AC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_free_system_database();
    case 0x800B0908:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_restore_system_database_title();
    case 0x800B093C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_close_system_body_menu();
    case 0x800B0988:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_system_body_menu();
        return 0;
    case 0x800B0B60:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_system_selection_link((uint32)a0);
        return 0;
    case 0x800B0D74:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_system_body_menu();
        return 0;
    case 0x800B0E84:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_selected_body_view();
    case 0x800B0F30:
        if (count > 3) game_unbound_native_call(target, count);
        front_restore_selected_body_names();
        return 0;
    case 0x800B1034:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_selected_system_menu();
        return 0;
    case 0x800B10F4:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_selected_system_menu();
        return 0;
    case 0x800B1190:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_finish_selected_body_view();
    case 0x800B11F0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_system_audio_first();
    case 0x800B126C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_system_audio_second();
    case 0x800B12E8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_system_audio_third();
    case 0x800B1368:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_body_audio_first();
    case 0x800B13E4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_body_audio_second();
    case 0x800B1460:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_body_audio_third();
    case 0x800B14D8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_stop_system_audio();
    case 0x800B25D0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_layers();
    case 0x800B2680:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_configure_layer((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B2748:
        if (count > 3) game_unbound_native_call(target, count);
        front_render_begin();
        return 0;
    case 0x800B2EB4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_initialize_bar_transitions((uint32)a0);
        return 0;
    case 0x800B2F70:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        front_reset_bar_transition((uint32)a0, (uint32)a1);
        return 0;
    case 0x800B2F84:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_start_bar_transition((uint32)a0, (uint32)a1);
    case 0x800B30EC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_prepare_bar_scale();
    case 0x800B3184:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_bar_transition_extent();
    case 0x800B3258:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_bar_scale();
        return 0;
    case 0x800B3354:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_bar_transition_labels((uint32)a0);
        return 0;
    case 0x800B352C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_bar_transition((uint32)a0);
    case 0x800B3698:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_draw_bar_transitions((uint32)a0);
        return 0;
    case 0x800B3B90:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_ranking_statistics();
        return 0;
    case 0x800B4050:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_ranking_statistics();
        return 0;
    case 0x800B42AC:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_mission_statistics();
        return 0;
    case 0x800B45D8:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_mission_statistics();
        return 0;
    case 0x800B4868:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_database_audio();
    case 0x800B496C:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_database_audio_timer();
        return 0;
    case 0x800B4A08:
        if (count > 3) game_unbound_native_call(target, count);
        front_free_database_audio();
        return 0;
    case 0x800B4A4C:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_database_mission();
        return 0;
    case 0x800B4ABC:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_database_mission_list();
        return 0;
    case 0x800B4B84:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_database_mission_list();
        return 0;
    case 0x800B4C90:
        if (count > 3) game_unbound_native_call(target, count);
        front_database_mission_idle();
        return 0;
    case 0x800B4C98:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_link_campaign_node((uint32)a0);
        return 0;
    case 0x800B4EDC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_request_complete_campaign_map();
    case 0x800B4F08:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_initialize_campaign_map();
    case 0x800B55F0:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_campaign_map();
        return 0;
    case 0x800B62E0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_select_campaign_mission();
    case 0x800B643C:
        if (count > 3) game_unbound_native_call(target, count);
        front_finish_campaign_map();
        return 0;
    case 0x800B648C:
        if (count > 3) game_unbound_native_call(target, count);
        front_play_selected_campaign_movie();
        return 0;
    case 0x800B64D0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_save_campaign_selection();
    case 0x800B64EC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_allocate_cached_text((uint32)a0, (uint32)a1);
    case 0x800B6588:
        if (count > 3) game_unbound_native_call(target, count);
        front_free_cached_text();
        return 0;
    case 0x800B69AC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_draw_cached_text((uint32)a0);
    case 0x800B6A90:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_rank_lighting();
        return 0;
    case 0x800B6B4C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_campaign_rank();
    case 0x800B6BEC:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_mission_results();
        return 0;
    case 0x800B6DD8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_initialize_rank_models((uint32)a0);
        return 0;
    case 0x800B6FAC:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_rank_models();
        return 0;
    case 0x800B7168:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_mission_results();
        return 0;
    case 0x800B71E4:
        if (count > 3) game_unbound_native_call(target, count);
        front_free_mission_results();
        return 0;
    case 0x80092A84:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_ship_statistics();
        return 0;
    case 0x800931B8:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_ship_details();
        return 0;
    case 0x80094028:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_ship_view();
        return 0;
    case 0x80094564:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_ship_comparison();
        return 0;
    case 0x800947CC:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_ship_view();
        return 0;
    case 0x80094D8C:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_ship_comparison();
        return 0;
    case 0x8009D254:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_poll_event_bank_b();
    case 0x8009D2AC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_poll_event_bank_a();
    case 0x8009DD90:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)front_save_named_settings((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B6580:
        if (count > 3) game_unbound_native_call(target, count);
        front_free_cached_text();
        return 0;
    case 0x800A54C4:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_mission_briefing();
        return 0;
    case 0x800A5540:
        if (count > 3) game_unbound_native_call(target, count);
        front_select_mission_briefing_text();
        return 0;
    case 0x8009C794:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        front_update_string_fade((uint32)a0);
        return 0;
    case 0x8008DC54:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_update_menu_frame();
    case 0x800A3E00:
        if (count > 3) game_unbound_native_call(target, count);
        front_release_control_viewer();
        return 0;
    case 0x800A7668:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_display_confirmation();
        return 0;
    case 0x800A76E0:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_display_confirmation();
        return 0;
    case 0x800A96DC:
        if (count > 3) game_unbound_native_call(target, count);
        front_play_requested_intro();
        return 0;
    case 0x800B0630:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_system_database();
        return 0;
    case 0x800B0900:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_restore_system_database_title();
    case 0x800B0980:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_system_body_menu();
        return 0;
    case 0x800B11E8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_system_audio_first();
    case 0x800B1264:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_system_audio_second();
    case 0x800B12E0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_system_audio_third();
    case 0x800B1360:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_body_audio_first();
    case 0x800B13DC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_body_audio_second();
    case 0x800B1458:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_play_body_audio_third();
    case 0x800B4AB4:
        if (count > 3) game_unbound_native_call(target, count);
        front_initialize_database_mission_list();
        return 0;
    case 0x800B4B7C:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_database_mission_list();
        return 0;
    case 0x800B55E8:
        if (count > 3) game_unbound_native_call(target, count);
        front_draw_campaign_map();
        return 0;
    case 0x800ADC14:
        if (count > 3) game_unbound_native_call(target, count);
        front_free_briefing_view();
        return 0;
    case 0x80095654:
        if (count > 3) game_unbound_native_call(target, count);
        front_update_ship_screen();
        return 0;
    case 0x800A2A84:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_erase_selected_card_media();
    case 0x800A2AC8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)front_probe_selected_card_media();
    case 0x800B0828:
        if (count > 3) game_unbound_native_call(target, count);
        front_copy_selected_system_name();
        return 0;
    case 0x800A75E0:
        if (count > 3) game_unbound_native_call(target, count);
        front_snapshot_display_options();
        return 0;
    default:
        game_unbound_native_call(target, count);
        return 0;
    }
}

static uint32 game_dispatch_game(uint32 target, uint32 count, uint32 a0, uint32 a1, uint32 a2)
{
    switch (target)
    {
    case 0x80078054:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_player_weapon_slots();
    case 0x80078A2C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_player_view();
    case 0x80078C00:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_copy_player_view_transform();
    case 0x80078CA0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_player_objects();
    case 0x80078ED4:
        if (count > 3) game_unbound_native_call(target, count);
        game_free_player_view();
        return 0;
    case 0x80078F64:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_player((uint32)a0);
        return 0;
    case 0x80079D30:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_target_obstructed((uint32)a0);
    case 0x8007A5C8:
        if (count > 3) game_unbound_native_call(target, count);
        game_start_damage_state();
        return 0;
    case 0x8007A644:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_damage_state((uint32)a0);
        return 0;
    case 0x8007A68C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_finish_damage_state((uint32)a0);
        return 0;
    case 0x8007A6BC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_temporary_camera((uint32)a0, (uint32)a1);
    case 0x8007A770:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_temporary_camera((uint32)a0);
        return 0;
    case 0x8007A7CC:
        if (count > 3) game_unbound_native_call(target, count);
        game_fail_current_mission();
        return 0;
    case 0x8007A8DC:
        if (count > 3) game_unbound_native_call(target, count);
        game_return_7a8dc();
        return 0;
    case 0x8007A8E4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_player_end_delay((uint32)a0);
        return 0;
    case 0x8007A9CC:
        if (count > 3) game_unbound_native_call(target, count);
        game_return_7a9cc();
        return 0;
    case 0x8007A9D4:
        if (count > 3) game_unbound_native_call(target, count);
        game_return_7a9d4();
        return 0;
    case 0x8007A9DC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_load_player_models((uint32)a0);
        return 0;
    case 0x8007ADC0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_boundary_autopilot((uint32)a0);
        return 0;
    case 0x8007B05C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_rotating_laser_object((uint32)a0);
        return 0;
    case 0x8007B12C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_player_end_sequence((uint32)a0);
        return 0;
    case 0x8007B840:
        if (count > 3) game_unbound_native_call(target, count);
        game_launch_escape_pod_sequence();
        return 0;
    case 0x8007BD84:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_prepare_mission_completion_state();
    case 0x8007BDB8:
        if (count > 3) game_unbound_native_call(target, count);
        game_begin_mission_completion_sequence_one();
        return 0;
    case 0x8007C0CC:
        if (count > 3) game_unbound_native_call(target, count);
        game_begin_mission_completion_sequence_two();
        return 0;
    case 0x8007C2D4:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_player_control_mode_1();
        return 0;
    case 0x8007C504:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_player_control_mode_3();
        return 0;
    case 0x8007C724:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_player_control_mode_2();
        return 0;
    case 0x8007C72C:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_player_control_mode_4();
        return 0;
    case 0x8007C938:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_player_control_mode_5();
        return 0;
    case 0x8007CCD4:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_player_control_mode();
        return 0;
    case 0x8007CD88:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_begin_object_camera_sequence((uint32)a0, (uint32)a1);
        return 0;
    case 0x8007CEAC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_begin_target_effect_camera((uint32)a0, (uint32)a1);
        return 0;
    case 0x8007D050:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_scale_mesh((uint32)a0, (uint32)a1);
        return 0;
    case 0x8007D0E0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_relocate_tmd_meshes((uint32)a0);
    case 0x8007D150:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_copy_tmd_meshes((uint32)a0, (uint32)a1, (sint32)a2);
    case 0x8007D1EC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_measure_model_axis_bounds((uint32)a0);
        return 0;
    case 0x8007D2FC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_measure_mesh_radius((uint32)a0);
    case 0x8007D568:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_12((uint32)a0);
    case 0x8007D770:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_16((uint32)a0);
    case 0x8007D8E0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_14((uint32)a0);
    case 0x8007DAE8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_10((uint32)a0);
    case 0x8007DC48:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_1a((uint32)a0);
    case 0x8007DDA8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_18((uint32)a0);
    case 0x8007DF0C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_02((uint32)a0);
    case 0x8007DF78:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_mission_weapon_slot((uint32)a0);
    case 0x8007DFE8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_04((uint32)a0);
    case 0x8007E054:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_06((uint32)a0);
    case 0x8007E0C0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_00((uint32)a0);
    case 0x8007E12C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_0e((uint32)a0);
    case 0x8007E198:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_0a((uint32)a0);
    case 0x8007E204:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_0c((uint32)a0);
    case 0x8007E270:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_weapon_profile_20((uint32)a0);
    case 0x8007E2E0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_status_slot_type14((uint32)a0);
    case 0x8007E34C:
        if (count > 3) game_unbound_native_call(target, count);
        game_return_7e34c();
        return 0;
    case 0x8007E354:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_12((uint32)a0);
        return 0;
    case 0x8007E474:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_16((uint32)a0);
        return 0;
    case 0x8007E594:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_18((uint32)a0);
        return 0;
    case 0x8007E6B4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_10((uint32)a0);
        return 0;
    case 0x8007E7D4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_1a((uint32)a0);
        return 0;
    case 0x8007E8F4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_14((uint32)a0);
        return 0;
    case 0x8007EA14:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_02((uint32)a0);
        return 0;
    case 0x8007EA8C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_mission_weapon((uint32)a0);
        return 0;
    case 0x8007EB08:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_06((uint32)a0);
        return 0;
    case 0x8007EB80:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_04((uint32)a0);
        return 0;
    case 0x8007EBF8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_00((uint32)a0);
        return 0;
    case 0x8007EC90:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_0c((uint32)a0);
        return 0;
    case 0x8007ED04:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_0e((uint32)a0);
        return 0;
    case 0x8007EDE4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_0a((uint32)a0);
        return 0;
    case 0x8007EECC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_20((uint32)a0);
        return 0;
    case 0x8007EF74:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_auxiliary_weapon((uint32)a0);
        return 0;
    case 0x8007F0CC:
        if (count > 3) game_unbound_native_call(target, count);
        game_emit_weapon_profile_12();
        return 0;
    case 0x8007F244:
        if (count > 3) game_unbound_native_call(target, count);
        game_emit_weapon_profile_16();
        return 0;
    case 0x8007F2FC:
        if (count > 3) game_unbound_native_call(target, count);
        game_emit_weapon_profile_18();
        return 0;
    case 0x8007F490:
        if (count > 3) game_unbound_native_call(target, count);
        game_emit_weapon_profile_10();
        return 0;
    case 0x8007F60C:
        if (count > 3) game_unbound_native_call(target, count);
        game_emit_weapon_profile_1a();
        return 0;
    case 0x8007F744:
        if (count > 3) game_unbound_native_call(target, count);
        game_emit_weapon_profile_14();
        return 0;
    case 0x8007F8BC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_profile_0c_projectile((uint32)a0, (uint32)a1);
    case 0x80080214:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_plasma_torpedo((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800803C4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_antishield_projectile((uint32)a0);
        return 0;
    case 0x80080464:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_stun_missile((uint32)a0);
    case 0x800804B8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_target_decoy((uint32)a0);
        return 0;
    case 0x800805C0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_integrate_projectile((uint32)a0);
    case 0x8008068C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_weapon_projectile((uint32)a0);
        return 0;
    case 0x800807D8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_profile_0c_projectile((uint32)a0);
        return 0;
    case 0x80080828:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_split_projectile((uint32)a0);
        return 0;
    case 0x80080B8C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_homing_projectile((uint32)a0);
        return 0;
    case 0x80080EF4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_speed_gated_projectile((uint32)a0);
        return 0;
    case 0x80081294:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_expiring_projectile((uint32)a0);
        return 0;
    case 0x800812E0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_auxiliary_projectile((uint32)a0);
        return 0;
    case 0x8008132C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_destroy_weapon_projectile((uint32)a0);
        return 0;
    case 0x80081408:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_remove_fragmenting_projectile((uint32)a0);
    case 0x8008146C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_collide_mission_projectile((uint32)a0);
        return 0;
    case 0x8008157C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_destroy_mission_projectile((uint32)a0);
        return 0;
    case 0x800815AC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_reprogrammed_target((uint32)a0);
        return 0;
    case 0x800815E0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_decrement_nonnegative((sint32)a0);
    case 0x800815F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_wrap_signed_angle((uint32)a0);
    case 0x8008169C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_object_movement((uint32)a0);
    case 0x800818F8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_set_acceleration_from_angles((uint32)a0, (uint32)a1);
    case 0x8008197C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_reduce_without_sign_change((sint32)a0, (uint32)a1);
    case 0x800819EC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_canonicalize_object_angles((uint32)a0);
    case 0x80081AC8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_set_acceleration_from_basis((uint32)a0, (uint32)a1);
    case 0x80081B14:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_object_state((uint32)a0);
        return 0;
    case 0x80081CD8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_limit_signed_step((sint32)a0, (sint32)a1);
    case 0x80081D0C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_step_toward_angle((uint32)a0, (uint32)a1, (sint32)a2);
    case 0x80081D8C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_view_controller((uint32)a0);
        return 0;
    case 0x8008246C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_view_controller();
    case 0x80082530:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_cycle_view_mode();
    case 0x800825DC:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_screen_flash();
        return 0;
    case 0x8008281C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_set_screen_tint((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80082880:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_run_scene();
    case 0x80082A48:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_poll_frame_input();
    case 0x80082C44:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_model_bounds((uint32)a0);
        return 0;
    case 0x80082CCC:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_weapon_defaults();
        return 0;
    case 0x80082D40:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_and_draw_scene();
        return 0;
    case 0x80082F8C:
        if (count > 3) game_unbound_native_call(target, count);
        game_stop_scene_stream();
        return 0;
    case 0x80082FE8:
        if (count > 3) game_unbound_native_call(target, count);
        game_shutdown_scene();
        return 0;
    case 0x800830CC:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_model_table();
        return 0;
    case 0x8008314C:
        if (count > 3) game_unbound_native_call(target, count);
        game_free_scene_models();
        return 0;
    case 0x80083320:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_target_aim_statistics((uint32)a0, (uint32)a1);
    case 0x800834CC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_canonicalize_angle_pair((uint32)a0, (uint32)a1);
        return 0;
    case 0x80083780:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_evaluate_target_occlusion((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x80083B00:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_profile_29a((uint32)a0);
        return 0;
    case 0x80083B38:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_mesh_effect_sequence((uint32)a0);
        return 0;
    case 0x80083F24:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_release_object_link((uint32)a0);
    case 0x80083F5C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_object_link((uint32)a0);
        return 0;
    case 0x800853F4:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_apply_tractor_beam((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x80085848:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_transform_bounded_object_point((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800859F4:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_message_queue();
        return 0;
    case 0x80086124:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_docking_camera((uint32)a0);
        return 0;
    case 0x800868A0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_docking_completion((uint32)a0);
        return 0;
    case 0x80087438:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_docking_sequence_object((uint32)a0);
        return 0;
    case 0x80087458:
        if (count > 3) game_unbound_native_call(target, count);
        game_clear_ship_effect_counters();
        return 0;
    case 0x80087498:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_queue_hud_message((uint32)a0);
        return 0;
    case 0x800875A0:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_queued_scene_messages();
        return 0;
    case 0x8008776C:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_exit_request();
        return 0;
    case 0x800877B4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_request_radio_message((uint32)a0, (uint32)a1);
    case 0x80087808:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_request_stream_preset_one();
    case 0x80087848:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_request_stream_preset_two();
    case 0x800879A0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_request_stream_preset_three();
    case 0x800879FC:
        if (count > 3) game_unbound_native_call(target, count);
        game_process_pause_request();
        return 0;
    case 0x80087E3C:
        if (count > 3) game_unbound_native_call(target, count);
        game_adjust_display_position();
        return 0;
    case 0x80087F3C:
        if (count > 3) game_unbound_native_call(target, count);
        game_pause_menu_leave();
        return 0;
    case 0x80087F44:
        if (count > 3) game_unbound_native_call(target, count);
        game_reserved_leaf();
        return 0;
    case 0x80087F4C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_count_pause_menu_entries();
    case 0x80087F9C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_pause_selection((uint32)a0);
        return 0;
    case 0x80088044:
        if (count > 3) game_unbound_native_call(target, count);
        game_run_pause_menu();
        return 0;
    case 0x80088954:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_options();
        return 0;
    case 0x8008895C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_pause_options_for_menu((uint32)a0);
        return 0;
    case 0x80088F90:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_panel();
        return 0;
    case 0x80088F98:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_pause_panel_for_menu((uint32)a0);
        return 0;
    case 0x80089910:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_heading();
        return 0;
    case 0x80089918:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_pause_heading_for_menu((uint32)a0);
        return 0;
    case 0x80089D90:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_pause_frame((uint32)a0);
        return 0;
    case 0x80089FBC:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_pause_selection();
        return 0;
    case 0x80089FCC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_move_pause_selection((uint32)a0, (uint32)a1);
        return 0;
    case 0x8008A160:
        if (count > 3) game_unbound_native_call(target, count);
        game_handle_pause_menu_input();
        return 0;
    case 0x8008A2A4:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_pause_grid();
        return 0;
    case 0x8008A30C:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_warp_grid();
        return 0;
    case 0x8008A324:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_draw_pause_warp_grid_values((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x8008A8E8:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_menu();
        return 0;
    case 0x8008A928:
        if (count > 3) game_unbound_native_call(target, count);
        game_restore_pause_setting_values();
        return 0;
    case 0x8008A9A0:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_pause_settings();
        return 0;
    case 0x8008AD20:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_settings();
        return 0;
    case 0x8008ADBC:
        if (count > 3) game_unbound_native_call(target, count);
        game_begin_pause_menu_transition();
        return 0;
    case 0x8008AED8:
        if (count > 3) game_unbound_native_call(target, count);
        game_pause_menu_enter();
        return 0;
    case 0x8008AEF8:
        if (count > 3) game_unbound_native_call(target, count);
        game_leave_pause_settings();
        return 0;
    case 0x8008AF5C:
        if (count > 3) game_unbound_native_call(target, count);
        game_accept_pause_settings();
        return 0;
    case 0x8008AF7C:
        if (count > 3) game_unbound_native_call(target, count);
        game_snapshot_pause_settings();
        return 0;
    case 0x8008AFE4:
        if (count > 3) game_unbound_native_call(target, count);
        game_swap_pause_menu_geometry();
        return 0;
    case 0x8008B02C:
        if (count > 3) game_unbound_native_call(target, count);
        game_resume_from_pause_settings();
        return 0;
    case 0x8008B080:
        if (count > 3) game_unbound_native_call(target, count);
        game_request_pause_mission_exit();
        return 0;
    case 0x8008B11C:
        if (count > 3) game_unbound_native_call(target, count);
        game_swap_pause_menu_return_geometry();
        return 0;
    case 0x8008B874:
        if (count > 3) game_unbound_native_call(target, count);
        game_prepare_pause_scene();
        return 0;
    case 0x8008BA28:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_grid_background();
        return 0;
    case 0x8008BF84:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_ship_preview();
        return 0;
    case 0x8008C470:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_pause_resource_wait();
        return 0;
    case 0x8008C56C:
        if (count > 3) game_unbound_native_call(target, count);
        game_start_pause_resource_wait();
        return 0;
    case 0x8008C59C:
        if (count > 3) game_unbound_native_call(target, count);
        game_cancel_pause_resource_wait();
        return 0;
    case 0x8008C5CC:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_local_starfield();
        return 0;
    case 0x8008C6D0:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_camera_particle_layers();
        return 0;
    case 0x8008CB0C:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_spherical_starfield();
        return 0;
    case 0x8008CC7C:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_camera_overlay();
        return 0;
    case 0x8008CCA4:
        if (count > 3) game_unbound_native_call(target, count);
        game_clear_scene_link_slots();
        return 0;
    case 0x8008D0CC:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_attached_glows();
        return 0;
    case 0x8008D410:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_projected_light_occlusion((uint32)a0, (uint32)a1);
        return 0;
    case 0x8008D9B8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_red_light_occlusion((uint32)a0, (uint32)a1);
        return 0;
    case 0x8008DF64:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_violet_light_occlusion((uint32)a0, (uint32)a1);
        return 0;
    case 0x8008E510:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_view_by_mode((uint32)a0, (uint32)a1);
        return 0;
    case 0x8008E590:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_draw_position_glow((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x8008EA60:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_scene_grid();
        return 0;
    case 0x8008F03C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_warp_grid((uint32)a0);
        return 0;
    case 0x8008FEF0:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_allocate_arrival_warp_effect((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x80090160:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_allocate_warp_effect((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800903D0:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_active_groups();
        return 0;
    case 0x80090434:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_partial_rotating_mesh((uint32)a0);
        return 0;
    case 0x800907D0:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_orient_radial_mesh((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x80090A38:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_initialize_radial_mesh((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800911DC:
        if (count > 3) game_unbound_native_call(target, count);
        game_free_optional_scene_buffer();
        return 0;
    case 0x80091210:
        if (count > 3) game_unbound_native_call(target, count);
        game_clear_model_binding_slots();
        return 0;
    case 0x8009125C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_import_resource_bundle((uint32)a0, (uint32)a1);
    case 0x80091BA4:
        if (count > 3) game_unbound_native_call(target, count);
        game_clear_scene_resource_state();
        return 0;
    case 0x80091BDC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_flight_controls((uint32)a0);
        return 0;
    case 0x80092730:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_rotate_player_x((uint32)a0);
        return 0;
    case 0x80092778:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_rotate_player_y((uint32)a0);
        return 0;
    case 0x800927C0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_rotate_player_z((uint32)a0);
        return 0;
    case 0x8009294C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_rotate_object_basis((uint32)a0);
        return 0;
    case 0x80092AD4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_restore_scene_draw_color();
    case 0x80092B24:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_get_controller_state();
    case 0x80092B3C:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_stream_callback();
        return 0;
    case 0x80092B74:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_level_scene();
    case 0x80092EEC:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_direction_indicator();
        return 0;
    case 0x80092F5C:
        if (count > 3) game_unbound_native_call(target, count);
        game_refresh_mission_state();
        return 0;
    case 0x800937A8:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_fire();
        return 0;
    case 0x80093BB0:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_add_jumpgate_objective();
        return 0;
    case 0x80093BCC:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_add_drone_objective();
        return 0;
    case 0x80093C2C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_increment_data_count();
    case 0x80093C48:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_objective_text();
        return 0;
    case 0x80093D78:
        if (count > 3) game_unbound_native_call(target, count);
        game_exit_level_script();
        return 0;
    case 0x80093F50:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_assign_ship_task_list((uint32)a0, (uint32)a1);
        return 0;
    case 0x80094058:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_ship_start_callback((uint32)a0);
    case 0x80094210:
        if (count > 3) game_unbound_native_call(target, count);
        game_load_asteroid_bundles();
        return 0;
    case 0x8009509C:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_navy_defeated_objective();
        return 0;
    case 0x80095118:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_league_defeated_objective();
        return 0;
    case 0x800952DC:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_planet_destroyed_objective();
        return 0;
    case 0x800952F8:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_begin_task_commands();
        return 0;
    case 0x80095CAC:
        if (count > 3) game_unbound_native_call(target, count);
        game_config_no_navy_fighters_objective();
        return 0;
    case 0x80095CD0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_object_mission_boundary((uint32)a0);
        return 0;
    case 0x8009674C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_load_level_script((uint32)a0);
        return 0;
    case 0x80096904:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_lookup_script_task((uint32)a0, (uint32)a1);
    case 0x800969A4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_apply_script_ship_properties((uint32)a0);
        return 0;
    case 0x80096B18:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_tick_script_ship_start((uint32)a0);
        return 0;
    case 0x80096CF0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_get_ship_task_parameter((uint32)a0, (uint32)a1);
    case 0x80097198:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_script_ship_attack_error((uint32)a0);
        return 0;
    case 0x800971CC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_script_ship_avoid_error((uint32)a0);
        return 0;
    case 0x80097200:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_script_ship_death_error((uint32)a0);
        return 0;
    case 0x8009756C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_load_bound_bundle((uint32)a0);
        return 0;
    case 0x800975B4:
        if (count > 3) game_unbound_native_call(target, count);
        game_stop_config_parse();
        return 0;
    case 0x800975C4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_model_name((uint32)a0);
    case 0x80097680:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_bind_model_name((uint32)a0, (uint32)a1);
        return 0;
    case 0x80097740:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_get_variable_address((uint32)a0);
    case 0x80097AB4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_model_name_cursor();
    case 0x80098124:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_relocate_collision_data((uint32)a0);
    case 0x80098140:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_intersect_segment_with_face((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x8009851C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_transform_vector_by_transposed_basis((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800991E8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_check_player_object_collision((uint32)a0);
    case 0x80099DB4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_resolve_object_collision((uint32)a0, (uint32)a1);
        return 0;
    case 0x8009A050:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_check_swept_object_collision((uint32)a0, (uint32)a1);
    case 0x8009A29C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_demo_input((uint32)a0);
        return 0;
    case 0x8009A354:
        if (count > 3) game_unbound_native_call(target, count);
        game_begin_input_recording();
        return 0;
    case 0x8009A534:
        if (count > 3) game_unbound_native_call(target, count);
        game_finish_demo_input();
        return 0;
    case 0x8009A53C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_load_demo_input((uint32)a0);
        return 0;
    case 0x8009A99C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_reset_clock_after_600_ticks();
    case 0x8009A9DC:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_demo_input_status();
        return 0;
    case 0x8009AAA8:
        if (count > 3) game_unbound_native_call(target, count);
        game_reset_demo_selection();
        return 0;
    case 0x8009AB50:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_ship_task_skip();
    case 0x8009AB58:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_script_ship_task((uint32)a0);
        return 0;
    case 0x8009ADB0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_skip_ship_task((uint32)a0, (uint32)a1);
    case 0x8009ADC8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_reset_object_control_link((uint32)a0);
        return 0;
    case 0x8009ADF4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_validate_ship_task_target((uint32)a0);
    case 0x8009AE28:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_validate_ship_attack_target((uint32)a0);
    case 0x8009B200:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_call_ship_death_callback((uint32)a0);
    case 0x8009C6E4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_deferred_ship_launch((uint32)a0);
        return 0;
    case 0x8009D360:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_actor_flag_delay((uint32)a0);
        return 0;
    case 0x8009E618:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_report_tractor_capture((uint32)a0, (uint32)a1);
        return 0;
    case 0x8009F5C4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_apply_sensor_damage((uint32)a0, (sint32)a1);
        return 0;
    case 0x8009F84C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_temporary_sensor_damage((uint32)a0);
        return 0;
    case 0x8009F944:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_sensor_recovery((uint32)a0);
        return 0;
    case 0x8009F9F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_permanent_sensor_damage((uint32)a0);
        return 0;
    case 0x800A1140:
        if (count > 3) game_unbound_native_call(target, count);
        game_create_controller_behind_player();
        return 0;
    case 0x800A138C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_distance_to_nearest_large_ship((uint32)a0);
    case 0x800A1490:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_record_destroyed_object((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800A1680:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_check_object_collisions((uint32)a0);
        return 0;
    case 0x800A1748:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_process_object_list_contacts((uint32)a0, (uint32)a1, (sint32)a2);
        return 0;
    case 0x800A18B4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_notify_object_contact((uint32)a0, (uint32)a1);
        return 0;
    case 0x800A19D4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_finish_special_mission_collision((uint32)a0);
        return 0;
    case 0x800A1A28:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_resolve_special_mission_collision((uint32)a0, (uint32)a1);
        return 0;
    case 0x800A20D0:
        if (count > 3) game_unbound_native_call(target, count);
        game_collide_scene_object_groups();
        return 0;
    case 0x800A268C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_active_ship_by_identifier((uint32)a0, (uint32)a1);
    case 0x800A27A0:
        if (count > 3) game_unbound_native_call(target, count);
        game_reset_object_collision_links();
        return 0;
    case 0x800A2830:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_ship_allegiance_change((uint32)a0);
        return 0;
    case 0x800A28EC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_replace_ship_for_category((uint32)a0, (uint32)a1);
        return 0;
    case 0x800A2A94:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_model_transition((uint32)a0);
        return 0;
    case 0x800A2C4C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_count_active_standard_objects();
    case 0x800A2CC4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_start_timed_view_effect((uint32)a0, (uint32)a1);
    case 0x800A2D3C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_timed_view_effect((uint32)a0);
        return 0;
    case 0x800A2F58:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_emit_model_fragments((uint32)a0, (sint32)a1, (uint32)a2);
        return 0;
    case 0x800A31C0:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_emit_velocity_fragments((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800A337C:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_configured_objects();
        return 0;
    case 0x800A358C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_distant_rotating_model((uint32)a0);
        return 0;
    case 0x800A3684:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_adjust_maneuver_offsets((uint32)a0, (uint32)a1);
    case 0x800A3704:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_task_approach((uint32)a0);
        return 0;
    case 0x800A3760:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_check_ship_task_approach_target((uint32)a0);
        return 0;
    case 0x800A3798:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_begin_ship_task_approach((uint32)a0, (uint32)a1);
        return 0;
    case 0x800A3A70:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_tick_ship_task_delay((uint32)a0);
        return 0;
    case 0x800A3A98:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_move_ship_and_tick_task_delay((uint32)a0);
        return 0;
    case 0x800A3AE0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_task_wait((uint32)a0);
        return 0;
    case 0x800A3B44:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_initialize_ship_launch((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800A3CAC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_carrier_ship_launch((uint32)a0);
        return 0;
    case 0x800A3E38:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_point_approach((uint32)a0);
        return 0;
    case 0x800A457C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_ship_offset_follow_maneuver((uint32)a0);
        return 0;
    case 0x800A461C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_offset_follow_maneuver((uint32)a0);
        return 0;
    case 0x800A47E4:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_position_at_object_offset((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800A487C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_position_ship_at_carrier_offset((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800A48F4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_retreat_task((uint32)a0);
        return 0;
    case 0x800A49A4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_carrier_ship_docking((uint32)a0);
        return 0;
    case 0x800A4D7C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_nearby_opposing_ship((uint32)a0, (uint32)a1);
    case 0x800A4E20:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_changed_ship_threats();
        return 0;
    case 0x800A4F00:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_escape_pod_movement((uint32)a0);
        return 0;
    case 0x800A5030:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_finalize_ship_removal((uint32)a0);
        return 0;
    case 0x800A5138:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_object_fade_out((uint32)a0);
        return 0;
    case 0x800A5264:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_fading_object((uint32)a0);
        return 0;
    case 0x800A52D0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_object_fade_in((uint32)a0);
        return 0;
    case 0x800A5380:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_belt_asteroid_spawner((uint32)a0);
        return 0;
    case 0x800A54F0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_belt_asteroid((uint32)a0);
    case 0x800A5850:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_asteroid_belt((uint32)a0);
        return 0;
    case 0x800A5B5C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_stop_and_launch_escape_pods((uint32)a0);
        return 0;
    case 0x800A5BE4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_escape_pod((uint32)a0);
    case 0x800A5E84:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_and_select_target((uint32)a0);
        return 0;
    case 0x800A612C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_tractor_beam((uint32)a0);
        return 0;
    case 0x800A61B8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_tractor_pull((uint32)a0);
        return 0;
    case 0x800A6478:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_tractor_bay_retrieval((uint32)a0);
        return 0;
    case 0x800A6B08:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_free_carrier_bay_near_target((uint32)a0, (uint32)a1);
    case 0x800A6C18:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_automatic_object_links((uint32)a0);
        return 0;
    case 0x800A7864:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_warp_departure((uint32)a0);
        return 0;
    case 0x800A7CCC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_jumpgate_ship_controller((uint32)a0);
        return 0;
    case 0x800A8400:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_delayed_ship_behavior((uint32)a0);
        return 0;
    case 0x800A853C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_entry_placement((uint32)a0);
        return 0;
    case 0x800A8D44:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_timed_departure((uint32)a0);
        return 0;
    case 0x800A8E70:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_replace_group_member_states((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800A8FA4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_formation((uint32)a0);
        return 0;
    case 0x800AA91C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_spawn_approaching_asteroid((uint32)a0);
        return 0;
    case 0x800AAC84:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_reposition_cloaked_ship((uint32)a0);
        return 0;
    case 0x800AAE44:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ship_random_point((uint32)a0);
        return 0;
    case 0x800AB06C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_warphole_lifetime((uint32)a0);
        return 0;
    case 0x800AB0FC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_moonbase_grid_controller((uint32)a0);
    case 0x800AB270:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_warphole_impact((uint32)a0);
        return 0;
    case 0x800AB8A0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_drifting_object((uint32)a0);
        return 0;
    case 0x800ABA30:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_target_beam_sequence((uint32)a0);
        return 0;
    case 0x800AC008:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_target_effect_view((uint32)a0);
        return 0;
    case 0x800AC560:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_select_carrier_launch_bay((uint32)a0);
    case 0x800AC5BC:
        if (count > 3) game_unbound_native_call(target, count);
        game_create_script_controller();
        return 0;
    case 0x800AC624:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_mission_script_controller((uint32)a0);
        return 0;
    case 0x800AC9B8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_ship_missing((uint32)a0);
    case 0x800ACA0C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_elapsed_time((uint32)a0);
    case 0x800ACB24:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_elapsed_time_unsigned((uint32)a0);
    case 0x800ACB54:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_primary_count_zero();
    case 0x800ACB9C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_secondary_count_zero();
    case 0x800ACBE4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_special_state_one((uint32)a0);
    case 0x800ACC34:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_destroyed_count((uint32)a0);
    case 0x800ACDB0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_secondary_event_count((uint32)a0);
    case 0x800ACF2C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_rings_complete((uint32)a0);
    case 0x800ACFF0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_mode_five();
    case 0x800AD02C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_mode_six();
    case 0x800AD058:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_disabled_count((uint32)a0);
    case 0x800AD200:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_status();
    case 0x800AD210:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_condition_no_enabled_objects();
    case 0x800AD270:
        if (count > 3) game_unbound_native_call(target, count);
        game_reset_level_script_state();
        return 0;
    case 0x800AD528:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_mission_completion((uint32)a0);
        return 0;
    case 0x800AD890:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_mission_completion_delay((uint32)a0);
        return 0;
    case 0x800AD8B8:
        if (count > 3) game_unbound_native_call(target, count);
        game_validate_level_script();
        return 0;
    case 0x800AD924:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_scene_objects();
    case 0x800ADC78:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_remove_objects_by_type((uint32)a0);
    case 0x800ADCE0:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_remove_other_objects_of_type((uint32)a0, (uint32)a1);
    case 0x800ADF8C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_object_removal((uint32)a0);
    case 0x800ADFC0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_mark_object_for_removal((uint32)a0);
    case 0x800AE054:
        if (count > 3) game_unbound_native_call(target, count);
        game_finish_scene_objects();
        return 0;
    case 0x800AE0BC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_count_script_ship_objects((uint32)a0, (uint32)a1);
    case 0x800AE19C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_count_matching_active_ship_objects((uint32)a0);
    case 0x800AE25C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_nearest_type_one((uint32)a0, (uint32)a1);
    case 0x800AE348:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_nearest_type_three((uint32)a0, (uint32)a1);
    case 0x800AE6C8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_count_objects_by_category((uint32)a0, (uint32)a1);
    case 0x800AE76C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_script_ship_health_below((uint32)a0, (uint32)a1);
    case 0x800AE88C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_copy_player_adjusted_basis_components((uint32)a0, (uint32)a1);
    case 0x800AE8DC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_get_object_forward((uint32)a0, (uint32)a1);
    case 0x800AED2C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_aimed_projectile((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800AF6DC:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_get_buffer_f4f80();
    case 0x800AF6EC:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_particle_tables();
        return 0;
    case 0x800AFEA0:
        if (count > 3) game_unbound_native_call(target, count);
        game_clear_particle_records();
        return 0;
    case 0x800AFF00:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_clear_particle_activity();
    case 0x800AFF30:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_scene_descriptor_chain();
        return 0;
    case 0x800AFFA4:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_effect_pool();
        return 0;
    case 0x800B01CC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_allocate_effect((uint32)a0, (uint32)a1);
    case 0x800B03E0:
        if (count > 3) game_unbound_native_call(target, count);
        game_return_without_effects();
        return 0;
    case 0x800B03E8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_line_effects((uint32)a0);
        return 0;
    case 0x800B0748:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_particle_effects((uint32)a0);
        return 0;
    case 0x800B0928:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_camera_effects((uint32)a0);
        return 0;
    case 0x800B0994:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_effect_chain((uint32)a0);
        return 0;
    case 0x800B11B8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_expire_model_effect((uint32)a0);
        return 0;
    case 0x800B12A4:
        if (count > 3) game_unbound_native_call(target, count);
        game_process_effect_removals();
        return 0;
    case 0x800B13DC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_unlink_effect_type((uint32)a0);
        return 0;
    case 0x800B1478:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_mark_effect_for_removal((uint32)a0);
        return 0;
    case 0x800B14BC:
        if (count > 3) game_unbound_native_call(target, count);
        game_free_effect_payloads();
        return 0;
    case 0x800B177C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_scatter_laser((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1884:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_emp_laser((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B18CC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_ki_plasma_laser((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1994:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_hull_laser((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B19D8:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_rear_laser((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1A40:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_antishield_laser((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1A84:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_profile_12_effect((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1AF8:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_profile_16_effect((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1B90:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_profile_18_effect((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1C84:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_profile_14_effect((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1CF8:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_profile_10_effect((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1E68:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_profile_1a_effect((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B1FE4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_model_effect((uint32)a0, (sint32)a1);
    case 0x800B20E8:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_scene_effects();
        return 0;
    case 0x800B2164:
        if (count > 3) game_unbound_native_call(target, count);
        game_finish_effect_updates();
        return 0;
    case 0x800B2194:
        if (count > 3) game_unbound_native_call(target, count);
        game_collide_scene_effects();
        return 0;
    case 0x800B22CC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_resolve_sphere_effect_hit((uint32)a0, (uint32)a1);
        return 0;
    case 0x800B2390:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_resolve_model_effect_hit((uint32)a0, (uint32)a1);
        return 0;
    case 0x800B2520:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_handle_effect_contact((uint32)a0, (uint32)a1);
        return 0;
    case 0x800B2710:
        if (count > 3) game_unbound_native_call(target, count);
        game_clear_trail_slots();
        return 0;
    case 0x800B2738:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_attach_object_trail((uint32)a0, (uint32)a1);
        return 0;
    case 0x800B2820:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_twisted_particle_trail((uint32)a0, (uint32)a1);
    case 0x800B34B4:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_object_trails();
        return 0;
    case 0x800B3764:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_detach_object_trails((uint32)a0);
        return 0;
    case 0x800B37B0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_laser_projectile((uint32)a0);
    case 0x800B37D0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_orbiting_projectile_effect((uint32)a0);
        return 0;
    case 0x800B3C08:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_burst_effect((uint32)a0);
        return 0;
    case 0x800B3E14:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_projectile_sparks((uint32)a0);
        return 0;
    case 0x800B3EE8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_projectile_arcs((uint32)a0);
        return 0;
    case 0x800B4184:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_pulsing_burst_effect((uint32)a0);
        return 0;
    case 0x800B43E0:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_target_lock_reticle();
    case 0x800B5C74:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_horizontal_reticle((uint32)a0);
    case 0x800B5D78:
        if (count > 3) game_unbound_native_call(target, count);
        game_bind_overlay_effect_textures();
        return 0;
    case 0x800B5E30:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_pack_palette_color((sint32)a0, (sint32)a1, (sint32)a2);
    case 0x800B5E5C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_upload_palette_row((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800B5EA0:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_head_models_and_palettes();
        return 0;
    case 0x800B6454:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_radar((uint32)a0, (uint32)a1);
        return 0;
    case 0x800B8E4C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_draw_flight_hud((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800BB828:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_bind_mesh_effect_texture((uint32)a0, (uint32)a1);
    case 0x800BB944:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_mesh_effect_pool();
    case 0x800BB9DC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_allocate_flash((uint32)a0);
    case 0x800BBAD0:
        if (count > 3) game_unbound_native_call(target, count);
        game_process_mesh_effect_removals();
        return 0;
    case 0x800BBBF0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_unlink_mesh_effect_type((uint32)a0);
        return 0;
    case 0x800BBC8C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_append_flash_reference((uint32)a0);
    case 0x800BBCC0:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_scene_mesh_effects();
        return 0;
    case 0x800BBCF8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_mesh_effect_chain((uint32)a0);
        return 0;
    case 0x800BBD64:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_emit_fragment_flash((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800BBE0C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_emit_sized_fragment_flash((uint32)a0, (uint32)a1);
        return 0;
    case 0x800BC9F0:
        if (count > 3) game_unbound_native_call(target, count);
        game_clear_auxiliary_effect_slots();
        return 0;
    case 0x800BCA38:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_emit_flash_bca38((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800BCB48:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_emit_projectile_impact((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800BCC5C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_emit_hit_flash((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800BCD68:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_expanding_bursts();
        return 0;
    case 0x800BD868:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_debris_fragment((uint32)a0);
        return 0;
    case 0x800BE09C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_emit_debris_expiration_effects((uint32)a0);
        return 0;
    case 0x800BF458:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_debris_fragment((uint32)a0);
        return 0;
    case 0x800BF6A4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_emit_model_breakup((uint32)a0);
        return 0;
    case 0x800BFB40:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_fragment_slots();
        return 0;
    case 0x800BFC70:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_emit_object_debris((uint32)a0);
        return 0;
    case 0x800C2428:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_dispatch_destruction_shape((uint32)a0);
        return 0;
    case 0x800C2468:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_emit_timed_point_effect((uint32)a0, (uint32)a1);
        return 0;
    case 0x800C2488:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_timed_point_effects((uint32)a0);
        return 0;
    case 0x800C2638:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_draw_attached_record((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800C4604:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_attached_record_small((uint32)a0, (uint32)a1);
        return 0;
    case 0x800C55A4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_attached_triangle((uint32)a0, (uint32)a1);
        return 0;
    case 0x800C5C20:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_object_details((uint32)a0);
        return 0;
    case 0x800C5E0C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_engine_effects((uint32)a0);
        return 0;
    case 0x800C682C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_visible_quad_mesh_effect((uint32)a0);
        return 0;
    case 0x800C694C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_normalize_vector_exact_in_place((uint32)a0);
    case 0x800C6A60:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_build_effect_palette_variants();
    case 0x800C6BD8:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_blob_effect_resources();
        return 0;
    case 0x800C7684:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_camera_particles();
        return 0;
    case 0x800C79BC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_expire_attached_effects((uint32)a0);
    case 0x800C7A18:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_object_particle_slots();
        return 0;
    case 0x800C7C98:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_object_spark_slots();
        return 0;
    case 0x800C803C:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_scene_particle_effects();
        return 0;
    case 0x800C8C2C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_process_camera_vector((uint32)a0, (uint32)a1);
        return 0;
    case 0x800C9460:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_tick_timed_maneuver((uint32)a0);
        return 0;
    case 0x800C9510:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_following_roll_maneuver((uint32)a0);
    case 0x800C9774:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_timed_maneuver((uint32)a0);
        return 0;
    case 0x800C9868:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_spinning_maneuver((uint32)a0);
    case 0x800C9940:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_four_phase_turn_maneuver((uint32)a0);
    case 0x800C9AEC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_begin_four_phase_turn_maneuver((uint32)a0, (uint32)a1);
    case 0x800C9B84:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_roll_transition_maneuver((uint32)a0);
    case 0x800C9C74:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_begin_roll_transition_maneuver((uint32)a0, (uint32)a1);
    case 0x800C9D50:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_start_ai_mode_maneuver((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800CA060:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_pitch_alignment_timer((uint32)a0);
    case 0x800CA100:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_curve_tables();
    case 0x800CA294:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_render_nearby_patch((uint32)a0);
        return 0;
    case 0x800CABFC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_draw_model_arcs((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800CB254:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_selected_object_link();
        return 0;
    case 0x800CB408:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_separate_object_overlaps((uint32)a0);
        return 0;
    case 0x800CB68C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_draw_descriptor_chain((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800CBA0C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_process_camera_particle_field((uint32)a0, (uint32)a1);
        return 0;
    case 0x800CBD88:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_update_camera_streaks((uint32)a0, (uint32)a1);
        return 0;
    case 0x800CC3BC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_update_and_draw_streak_list((uint32)a0, (uint32)a1);
        return 0;
    case 0x800CD428:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_collide_effect_chains((uint32)a0, (uint32)a1);
        return 0;
    case 0x800CE104:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_collide_effects_with_object((uint32)a0, (uint32)a1);
        return 0;
    case 0x800CE9EC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_draw_twisted_ring((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800CECCC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_annulus((uint32)a0);
        return 0;
    case 0x800CF648:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_render_interpolated_patch((uint32)a0, (uint32)a1);
        return 0;
    case 0x800D02FC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_weighted_curve((uint32)a0, (uint32)a1);
        return 0;
    case 0x800D1240:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_mesh_effects_from_camera((uint32)a0, (uint32)a1);
        return 0;
    case 0x800D164C:
        if (count > 3) game_unbound_native_call(target, count);
        game_load_scene_texture_page();
        return 0;
    case 0x800D1704:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_level_script();
        return 0;
    case 0x800D196C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_parse_level_numbers((uint32)a0);
        return 0;
    case 0x800D1A80:
        if (count > 3) game_unbound_native_call(target, count);
        game_measure_level_model_extent();
        return 0;
    case 0x800D1C3C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_scene_object((uint32)a0);
        return 0;
    case 0x800D25C8:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_carrier_launch_bay((uint32)a0, (uint32)a1);
        return 0;
    case 0x800D332C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_finish_object_action((uint32)a0);
    case 0x800D3384:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_object_removal_delay((uint32)a0);
    case 0x800D33AC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_object_destruction((uint32)a0);
        return 0;
    case 0x800D3478:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_remove_object_and_resources((uint32)a0);
        return 0;
    case 0x800D4060:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_reset_object_control_state((uint32)a0);
    case 0x800D41D8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_object_control((uint32)a0);
    case 0x800D4318:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_allocate_object_state_22c((uint32)a0);
    case 0x800D4358:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_object_control_alternate((uint32)a0);
    case 0x800D4498:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4498((uint32)a0);
        return 0;
    case 0x800D4568:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4568((uint32)a0);
        return 0;
    case 0x800D4690:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4690((uint32)a0);
        return 0;
    case 0x800D4768:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4768((uint32)a0);
        return 0;
    case 0x800D4844:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4844((uint32)a0);
        return 0;
    case 0x800D493C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_493c((uint32)a0);
        return 0;
    case 0x800D4A5C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4a5c((uint32)a0);
        return 0;
    case 0x800D4B58:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4b58((uint32)a0);
        return 0;
    case 0x800D4C48:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4c48((uint32)a0);
        return 0;
    case 0x800D4D5C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4d5c((uint32)a0);
        return 0;
    case 0x800D4E74:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4e74((uint32)a0);
        return 0;
    case 0x800D4F64:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_4f64((uint32)a0);
        return 0;
    case 0x800D5054:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_5054((uint32)a0);
        return 0;
    case 0x800D5144:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_object_variant_5144((uint32)a0);
    case 0x800D51D4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_51d4((uint32)a0);
        return 0;
    case 0x800D52A8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_52a8((uint32)a0);
        return 0;
    case 0x800D538C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_538c((uint32)a0);
        return 0;
    case 0x800D5440:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_state_204((uint32)a0);
        return 0;
    case 0x800D54F4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_initialize_carrier_bay_geometry((uint32)a0, (uint32)a1);
        return 0;
    case 0x800D5A90:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_5a90((uint32)a0);
        return 0;
    case 0x800D5C3C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_5c3c((uint32)a0);
        return 0;
    case 0x800D5DD8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_5dd8((uint32)a0);
        return 0;
    case 0x800D5F8C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_5f8c((uint32)a0);
        return 0;
    case 0x800D6130:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_6130((uint32)a0);
        return 0;
    case 0x800D6274:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_6274((uint32)a0);
        return 0;
    case 0x800D6450:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_6450((uint32)a0);
        return 0;
    case 0x800D662C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_662c((uint32)a0);
        return 0;
    case 0x800D682C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_682c((uint32)a0);
        return 0;
    case 0x800D6A50:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_6a50((uint32)a0);
        return 0;
    case 0x800D6C2C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_6c2c((uint32)a0);
        return 0;
    case 0x800D6DC4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_6dc4((uint32)a0);
        return 0;
    case 0x800D6FC4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_6fc4((uint32)a0);
        return 0;
    case 0x800D71EC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_71ec((uint32)a0);
        return 0;
    case 0x800D73C8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_73c8((uint32)a0);
        return 0;
    case 0x800D7554:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_7554((uint32)a0);
        return 0;
    case 0x800D76E0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_76e0((uint32)a0);
        return 0;
    case 0x800D786C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_786c((uint32)a0);
        return 0;
    case 0x800D79F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_79f8((uint32)a0);
        return 0;
    case 0x800D7B84:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_7b84((uint32)a0);
        return 0;
    case 0x800D7D10:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_7d10((uint32)a0);
        return 0;
    case 0x800D7E9C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_7e9c((uint32)a0);
        return 0;
    case 0x800D8210:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_8210((uint32)a0);
        return 0;
    case 0x800D8410:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_8410((uint32)a0);
        return 0;
    case 0x800D85C4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_85c4((uint32)a0);
        return 0;
    case 0x800D8768:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_8768((uint32)a0);
        return 0;
    case 0x800D8944:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_8944((uint32)a0);
        return 0;
    case 0x800D8B64:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_attached_model_object((uint32)a0);
        return 0;
    case 0x800D8C50:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_8c50((uint32)a0);
        return 0;
    case 0x800D8E2C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_8e2c((uint32)a0);
        return 0;
    case 0x800D9030:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_9030((uint32)a0);
        return 0;
    case 0x800D923C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_923c((uint32)a0);
        return 0;
    case 0x800D9418:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_9418((uint32)a0);
        return 0;
    case 0x800D95B4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_95b4((uint32)a0);
        return 0;
    case 0x800D9760:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_9760((uint32)a0);
        return 0;
    case 0x800D98EC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_98ec((uint32)a0);
        return 0;
    case 0x800D9A78:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_9a78((uint32)a0);
        return 0;
    case 0x800D9C04:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_9c04((uint32)a0);
        return 0;
    case 0x800D9D90:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_target_maneuver((uint32)a0);
        return 0;
    case 0x800DA150:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_target_maneuver_angles((uint32)a0);
        return 0;
    case 0x800DA210:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_target_tracking((uint32)a0);
        return 0;
    case 0x800DA414:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_maneuver_bias((uint32)a0);
    case 0x800DA41C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_maneuver_bias_from_heading((uint32)a0, (uint32)a1);
    case 0x800DA640:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_cached_target_tracking((uint32)a0);
        return 0;
    case 0x800DA870:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_start_target_tracking_maneuver((uint32)a0);
        return 0;
    case 0x800DA8BC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_target_tracking_ai((uint32)a0);
        return 0;
    case 0x800DAFDC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_target_collision_avoidance((uint32)a0);
        return 0;
    case 0x800DB220:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_reset_target_tracking_cache((uint32)a0);
    case 0x800DB30C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_alternate_target_tracking((uint32)a0);
        return 0;
    case 0x800DB430:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_copy_attack_run_to_control((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800DB588:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_large_target_attack((uint32)a0);
        return 0;
    case 0x800DBE20:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_combat_maneuver((uint32)a0);
        return 0;
    case 0x800DE274:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_target_weapons((uint32)a0);
        return 0;
    case 0x800DE6E4:
        if (count > 3) game_unbound_native_call(target, count);
        game_clear_scene_effect_flags();
        return 0;
    case 0x800DE70C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_release_linked_part_effects((uint32)a0);
        return 0;
    case 0x800DE774:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_attach_linked_part_effect((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800DF51C:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_active_effect_slots();
        return 0;
    case 0x800DF5D0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_timed_rotating_part((uint32)a0);
        return 0;
    case 0x800DF638:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_integrate_linked_part_velocity((uint32)a0);
    case 0x800DF704:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_linked_part_parent_velocity((uint32)a0);
        return 0;
    case 0x800DF768:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_linked_part_breakup((uint32)a0);
        return 0;
    case 0x800DFA38:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_remove_expired_linked_part((uint32)a0);
        return 0;
    case 0x800DFA78:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_linked_object_part((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800DFC24:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_create_two_linked_object_parts((uint32)a0);
        return 0;
    case 0x800DFE40:
        if (count > 3) game_unbound_native_call(target, count);
        game_position_scene_effects();
        return 0;
    case 0x800DFF1C:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_scene_engine_voices();
        return 0;
    case 0x800DFFA0:
        if (count > 3) game_unbound_native_call(target, count);
        game_stop_auxiliary_scene_voices();
        return 0;
    case 0x800DFFE8:
        if (count > 3) game_unbound_native_call(target, count);
        game_suspend_scene_engine_audio();
        return 0;
    case 0x800E0058:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_set_voice_and_overlay_volume((uint32)a0, (uint32)a1);
    case 0x800E0090:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_engine_audio((uint32)a0);
        return 0;
    case 0x800E02A4:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_scene_effect_voices();
        return 0;
    case 0x800E0454:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_accumulate_effect_audio((uint32)a0, (uint32)a1);
        return 0;
    case 0x800E045C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_accumulate_effect_audio_from_camera((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800E07FC:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_effect_audio_channel();
        return 0;
    case 0x800E0ABC:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_first_nearby_effect_audio();
        return 0;
    case 0x800E1250:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_second_nearby_effect_audio();
        return 0;
    case 0x800E19E4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_nearby_effect_audio();
    case 0x800E1CCC:
        if (count > 3) game_unbound_native_call(target, count);
        game_mute_scene_effect_voices();
        return 0;
    case 0x800E1D64:
        if (count > 3) game_unbound_native_call(target, count);
        game_stop_scene_voices();
        return 0;
    case 0x800E1E44:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_e1e44((uint32)a0);
        return 0;
    case 0x800E1FDC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_object_variant_e1fdc((uint32)a0);
        return 0;
    case 0x800E21DC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_initialize_large_effect_object((uint32)a0);
    case 0x800E2228:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_initialize_asteroid_fragment((uint32)a0);
        return 0;
    case 0x800E2314:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_break_asteroid_fragment((uint32)a0);
        return 0;
    case 0x800E2590:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_asteroid_fragment_motion((uint32)a0);
    case 0x800E25DC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_create_gradient_strip_set((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800E27A4:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_sky_geometry();
        return 0;
    case 0x800E2BD4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_effect_quad_mesh((uint32)a0);
    case 0x800E2DA8:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_sky_resources();
        return 0;
    case 0x800E2DD0:
        if (count > 3) game_unbound_native_call(target, count);
        game_free_scene_buffer_banks();
        return 0;
    case 0x800E2E48:
        if (count > 3) game_unbound_native_call(target, count);
        game_free_scene_buffers();
        return 0;
    case 0x800E3C58:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_environment_mesh();
        return 0;
    case 0x800E3EA8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_draw_target_effect_field((uint32)a0);
        return 0;
    case 0x800E42AC:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_scene_sky();
        return 0;
    case 0x800E43E0:
        if (count > 3) game_unbound_native_call(target, count);
        game_tick_script_events();
        return 0;
    case 0x800E44B4:
        if (count > 3) game_unbound_native_call(target, count);
        game_create_script_event_controller();
        return 0;
    case 0x800E44F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_message((uint32)a0);
    case 0x800E4518:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_mission_success((uint32)a0);
    case 0x800E4560:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_mission_failure((uint32)a0);
    case 0x800E4690:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_category_below((uint32)a0);
    case 0x800E4720:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_count((uint32)a0);
    case 0x800E477C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_category_above((uint32)a0);
    case 0x800E4818:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_reenabled((uint32)a0);
    case 0x800E491C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_counter((uint32)a0);
    case 0x800E4990:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_health_below((uint32)a0);
    case 0x800E49D8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_population_below((uint32)a0);
    case 0x800E4A48:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_distance_below((uint32)a0);
    case 0x800E4AE8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_signal((uint32)a0);
    case 0x800E4B88:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_flag((uint32)a0);
    case 0x800E4BF8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_secondary_count((uint32)a0);
    case 0x800E4C54:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_age((uint32)a0);
    case 0x800E4D10:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_elapsed_with_ship((uint32)a0);
    case 0x800E4DA4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_primary_depleted((uint32)a0);
    case 0x800E4E10:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_flag_count((uint32)a0);
    case 0x800E4ED4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_elapsed((uint32)a0);
    case 0x800E4F20:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_ratio_reached((uint32)a0);
    case 0x800E5034:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_idle_time((uint32)a0);
    case 0x800E50BC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_type_seven_count((uint32)a0);
    case 0x800E512C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_find_message_ship((uint32)a0);
    case 0x800E51B4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_play_condition_message((uint32)a0);
    case 0x800E52A8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_task_flag_count((uint32)a0);
    case 0x800E536C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_slot_flag((uint32)a0);
    case 0x800E5400:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_category_flag_count((uint32)a0);
    case 0x800E5494:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_event_ship_count_with_type_three((uint32)a0);
    case 0x800E5538:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_check_object_count_message((uint32)a0);
    case 0x800E560C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_check_ring_count_message((uint32)a0);
    case 0x800E56CC:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_emit_ring_fragments((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800E5710:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_process_ring_crossing((uint32)a0, (uint32)a1);
        return 0;
    case 0x800E5970:
        if (count > 3) game_unbound_native_call(target, count);
        game_draw_paired_glows();
        return 0;
    case 0x800E5BD8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ring_glow((uint32)a0);
        return 0;
    case 0x800E5F48:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_test_ring_crossing((uint32)a0, (uint32)a1);
    case 0x800E6264:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_finish_transition_with_warp((uint32)a0);
        return 0;
    case 0x800E62AC:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_subtract_camera_drift((uint32)a0, (uint32)a1);
    case 0x800E62C8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_transition_camera((uint32)a0);
        return 0;
    case 0x800E67B8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_create_transition_camera((uint32)a0);
        return 0;
    case 0x800E683C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_transition_approach((uint32)a0);
        return 0;
    case 0x800E6A2C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_transition_warp_controller((uint32)a0);
        return 0;
    case 0x800E728C:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_begin_transition_warp_effect((uint32)a0, (uint32)a1);
        return 0;
    case 0x800E74A4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_attach_player_to_transition_warp((uint32)a0);
        return 0;
    case 0x800E7730:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_transition_departure((uint32)a0);
        return 0;
    case 0x800E7F44:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_reset_player_ring_limits();
    case 0x800E7F5C:
        if (count > 3) game_unbound_native_call(target, count);
        game_reset_object_ring_limits();
        return 0;
    case 0x800E7FFC:
        if (count > 3) game_unbound_native_call(target, count);
        game_reset_transition_object_limits();
        return 0;
    case 0x800E8084:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_reset_transition_list_limits((uint32)a0);
        return 0;
    case 0x800E80C4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_transition_delay((uint32)a0);
        return 0;
    case 0x800E8114:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_training_rings();
        return 0;
    case 0x800E8200:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_resume_paused_transition_object((uint32)a0);
        return 0;
    case 0x800E8228:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_flight_training((uint32)a0);
        return 0;
    case 0x800E88F8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_announce_training_targets((sint32)a0);
        return 0;
    case 0x800E89D0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_shield_target_training((uint32)a0);
        return 0;
    case 0x800E8C6C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_missile_target_training((uint32)a0);
        return 0;
    case 0x800E8FA8:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_training_controller();
        return 0;
    case 0x800E9208:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_parse_briefing_text((uint32)a0, (uint32)a1);
    case 0x800E9420:
        if (count > 3) game_unbound_native_call(target, count);
        game_begin_resource_job();
        return 0;
    case 0x800E96E4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_poll_resource_job();
    case 0x800E9B94:
        if (count > 3) game_unbound_native_call(target, count);
        game_release_resource_job_buffer();
        return 0;
    case 0x800E9BD8:
        if (count > 3) game_unbound_native_call(target, count);
        game_fill_credits_records();
        return 0;
    case 0x800E9E1C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_update_and_draw_credits();
    case 0x800EA1FC:
        if (count > 3) game_unbound_native_call(target, count);
        game_initialize_credits_controller();
        return 0;
    case 0x800EA418:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_tick_credits_controller((uint32)a0);
        return 0;
    case 0x800EA598:
        if (count > 3) game_unbound_native_call(target, count);
        game_free_special_mode_buffers();
        return 0;
    case 0x800EA5EC:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_weapon_state();
        return 0;
    case 0x800EB138:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_free_object_auxiliary_buffer((uint32)a0);
        return 0;
    case 0x800EB168:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_configure_object_subsystems((uint32)a0, (uint32)a1);
        return 0;
    case 0x800EB8A4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_hull_lasers((uint32)a0);
        return 0;
    case 0x800EB9F0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_rear_lasers((uint32)a0);
        return 0;
    case 0x800EBABC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_antishield_lasers((uint32)a0);
        return 0;
    case 0x800EBC10:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_primary_lasers((uint32)a0);
        return 0;
    case 0x800EBC58:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_emp_lasers((uint32)a0);
        return 0;
    case 0x800EBDAC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_ki_plasma_lasers((uint32)a0);
        return 0;
    case 0x800EBF00:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_scatter_lasers((uint32)a0);
        return 0;
    case 0x800EBFCC:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_tracker_missile((uint32)a0);
        return 0;
    case 0x800EC060:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_motion_missile((uint32)a0);
        return 0;
    case 0x800EC0F4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_stun_missile((uint32)a0);
        return 0;
    case 0x800EC180:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_equipped_mole_missile((uint32)a0);
        return 0;
    case 0x800EC204:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_antishield_missile((uint32)a0);
        return 0;
    case 0x800EC298:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_plasma_torpedo((uint32)a0);
        return 0;
    case 0x800EC31C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_burst_torpedo((uint32)a0);
        return 0;
    case 0x800EC398:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_fire_antishield_torpedo((uint32)a0);
        return 0;
    case 0x800EC414:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_try_launch_target_decoy((uint32)a0);
        return 0;
    case 0x800EC484:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_object_automatic_weapons((uint32)a0);
        return 0;
    case 0x800EC57C:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        return (uint32)game_is_weapon_target_in_range((uint32)a0, (uint32)a1, (uint32)a2);
    case 0x800EC724:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_synchronize_missile_cooldowns((uint32)a0);
        return 0;
    case 0x8007ADB8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_boundary_autopilot((uint32)a0);
        return 0;
    case 0x8007B054:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_rotating_laser_object((uint32)a0);
        return 0;
    case 0x80087840:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_request_stream_preset_two();
    case 0x80091BD4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_flight_controls((uint32)a0);
        return 0;
    case 0x80095CC8:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_object_mission_boundary((uint32)a0);
        return 0;
    case 0x800A1138:
        if (count > 3) game_unbound_native_call(target, count);
        game_create_controller_behind_player();
        return 0;
    case 0x800A31B0:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_emit_velocity_fragments((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800AB064:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_warphole_lifetime((uint32)a0);
        return 0;
    case 0x800AD520:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_begin_mission_completion((uint32)a0);
        return 0;
    case 0x800B43D8:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_target_lock_reticle();
    case 0x800B8E44:
        if (count < 3 || count > 3) game_unbound_native_call(target, count);
        game_draw_flight_hud((uint32)a0, (uint32)a1, (uint32)a2);
        return 0;
    case 0x800C45F4:
        if (count < 2 || count > 3) game_unbound_native_call(target, count);
        game_draw_attached_record_small((uint32)a0, (uint32)a1);
        return 0;
    case 0x800E96D4:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_poll_resource_job();
    case 0x8008611C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_docking_camera((uint32)a0);
        return 0;
    case 0x80081D84:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_view_controller((uint32)a0);
        return 0;
    case 0x800CA28C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_render_nearby_patch((uint32)a0);
        return 0;
    case 0x800B5C6C:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_horizontal_reticle(r_u32(0x80040690));
    case 0x80078A24:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_draw_player_view();
    case 0x80078ECC:
        if (count > 3) game_unbound_native_call(target, count);
        game_free_player_view();
        return 0;
    case 0x80078F5C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_player((uint32)a0);
        return 0;
    case 0x8007EEC4:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_fire_weapon_profile_20((uint32)a0);
        return 0;
    case 0x80092B34:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_stream_callback();
        return 0;
    case 0x800EA410:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_tick_credits_controller((uint32)a0);
        return 0;
    case 0x800AC61C:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_mission_script_controller((uint32)a0);
        return 0;
    case 0x8008A998:
        if (count > 3) game_unbound_native_call(target, count);
        game_update_pause_settings();
        return 0;
    case 0x80093C24:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)game_script_increment_data_count();
    case 0x800E6A24:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_transition_warp_controller((uint32)a0);
        return 0;
    case 0x800E5BD0:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_ring_glow((uint32)a0);
        return 0;
    case 0x800E8C64:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        game_update_missile_target_training((uint32)a0);
        return 0;
    default:
        game_unbound_native_call(target, count);
        return 0;
    }
}

static uint32 game_dispatch_anim(uint32 target, uint32 count, uint32 a0, uint32 a1, uint32 a2)
{
    switch (target)
    {
    case 0x80091824:
        if (count > 3) game_unbound_native_call(target, count);
        return (uint32)anim_play_default_movie();
    case 0x80091860:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)anim_play_movie_with_skip_delay((uint32)a0);
    case 0x80091898:
        if (count < 1 || count > 3) game_unbound_native_call(target, count);
        return (uint32)anim_play_movie_without_clear((uint32)a0);
    default:
        game_unbound_native_call(target, count);
        return 0;
    }
}
