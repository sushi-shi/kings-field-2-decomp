#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/notification_quad.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <kf/game/render_model.h>

ADDRESS(0x80033584, 0x1c)
void display_toggle_buffer_index(void)
{
    game_graphics_runtime.display_state.buffer_index =
        game_graphics_runtime.display_state.buffer_index == 0;
}

ADDRESS(0x800335a0, 0x3f4)
void render_game_frame(const VECTOR *position, const SVECTOR *rotation)
{
    s32 remainder;
    s32 hp_hundreds;
    s32 hp_tens;
    s32 hp_ones;
    s32 mp_hundreds;
    s32 mp_tens;
    s32 mp_ones;
    s32 attack_width;
    s32 magic_width;
    s32 yaw_delta;
    u8 row_state;

    display_set_view_transform(position, rotation);
    floor_item_update_textures();
    notification_update();
    build_camera_map_cell_layer_masks();
    display_begin_frame();
    pool_mark_allocated();
    render_player_weapon();

    render_model_rows[0].state = player_state.compass_enabled;
    row_state = player_state.hud_gauges_enabled;
    render_model_rows[13].state = row_state;
    render_model_rows[12].state = row_state;
    render_model_rows[11].state = row_state;
    render_model_rows[10].state = row_state;
    render_model_rows[9].state = row_state;
    render_model_rows[8].state = row_state;
    render_model_rows[7].state = row_state;
    render_model_rows[6].state = row_state;
    render_model_rows[5].state = row_state;
    render_model_rows[4].state = row_state;
    render_model_rows[3].state = row_state;
    render_model_rows[2].state = row_state;
    render_model_rows[1].state = row_state;

    yaw_delta = render_model_yaw_smoothing_accumulator +
                angle_shortest_delta(render_model_rows[0].rotation.vy,
                                     game_graphics_runtime.render_state.view_rotation.vy);
    render_model_yaw_smoothing_accumulator = yaw_delta;
    if (yaw_delta > 0) {
        render_model_yaw_smoothing_accumulator = yaw_delta - ((yaw_delta + 7) >> 3);
    } else if (yaw_delta < 0) {
        render_model_yaw_smoothing_accumulator = yaw_delta - ((yaw_delta - 7) >> 3);
    }

    remainder = player_state.vitals.current_hp % 1000;
    hp_hundreds = remainder / 100;
    hp_tens = (remainder % 100) / 10;
    hp_ones = remainder % 10;
    remainder = player_state.vitals.current_mp % 1000;
    mp_hundreds = remainder / 100;
    mp_tens = (remainder % 100) / 10;
    mp_ones = remainder % 10;
    attack_width = player_state.attack_charge_current * 204 / 5000;
    magic_width = player_state.magic_charge * 204 / 5000;

    render_model_rows[0].rotation.vy +=
        render_model_yaw_smoothing_accumulator >> 6;
    render_model_rows[0].rotation.vx = game_graphics_runtime.render_state.view_rotation.vx;
    render_model_rows[3].asset_id = hp_hundreds + 3;
    render_model_rows[4].asset_id = hp_tens + 3;
    render_model_rows[5].asset_id = hp_ones + 3;
    render_model_rows[6].asset_id = mp_hundreds + 3;
    render_model_rows[7].asset_id = mp_tens + 3;
    render_model_rows[8].asset_id = mp_ones + 3;
    render_model_rows[9].scale.vx = attack_width;
    render_model_rows[10].scale.vx = magic_width;

    render_active_model_rows();
    notification_draw();
    render_map_cell_window();
    render_scene_and_update_resources();
    render_sliding_panel_primary();
    render_sliding_panel_secondary();
    render_color_overlay();
    render_accumulated_color_overlay();
    display_present_frame();
    cd_wait_two_vsyncs();
    pool_release_stale();
}

DATA(0x8006d6d4, 0x4)
s32 render_model_yaw_smoothing_accumulator = 0;
