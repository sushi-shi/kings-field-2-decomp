#include <kf/lib/overlay.h>
#include <kf/game/game.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/card.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/event_state.h>
#include <kf/game/graphics.h>
#include <kf/game/map_object.h>
#include <kf/game/memory.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/game/resources.h>
#include <psyq/audio.h>
#include <psyq/cd.h>
#include <psyq/kernel.h>
#include <psyq/pad.h>
#include <psyq/sdk.h>

enum class KfGameMainState : u32 {
    GAME_MAIN_RUNNING = 0,
    GAME_MAIN_EXIT_REQUESTED = 1
}; using enum KfGameMainState;

KfGameMainState game_main_exit_flag;

enum { GAME_INITIAL_MASTER_VOLUME = 0x7f };

extern "C" void main(void)
{
    InitHeap(NULL, KF_GAME_HEAP_BYTES);
    CdInit();
    PadInit(0);
    InitCARD(1);
    ChangeClearPAD(0);
    ExitCriticalSection();
    game_main_loop();
}

void game_main_loop(void)
{
    VECTOR camera_position;
    SVECTOR camera_rotation;

    repeat_store_word((u32 *)&resource_state, 0,
        sizeof resource_state / sizeof(u32));
    repeat_store_word((u32 *)&game_graphics_runtime, 0,
        sizeof game_graphics_runtime / sizeof(u32));
    repeat_store_word((u32 *)&player_state, 0,
        sizeof player_state / sizeof(u32));
    repeat_store_word((u32 *)&event_state, 0,
        sizeof event_state / sizeof(u32));
    repeat_store_word((u32 *)&bss_801c7540, 0,
        sizeof bss_801c7540 / sizeof(u32));
    repeat_store_word((u32 *)&map_object_state, 0,
        sizeof map_object_state / sizeof(u32));
    repeat_store_word((u32 *)&actor_state, 0,
        sizeof actor_state / sizeof(u32));
    repeat_store_word((u32 *)&effect_state, 0,
        sizeof effect_state / sizeof(u32));
    repeat_store_word((u32 *)&audio_state, 0,
        sizeof audio_state / sizeof(u32));

    cd_initialize();
    display_initialize();
    audio_initialize_runtime();
    map_object_pool_reset();
    actor_pool_clear();
    effect_pool_reset();
    resource_initialize_game_assets();
    event_state_initialize();
    memory_card_initialize();
    reset_collision_rows_and_overlay();
    game_graphics_runtime.collision_rotation_dirty = KF_TRUE;
    refresh_collision_row_rotations();
    SsSetMVol(GAME_INITIAL_MASTER_VOLUME, GAME_INITIAL_MASTER_VOLUME);
    if (menu_card_browser() != -1) {
        player_restore_equipment_effects();
    }
    resource_run_initial_transition();
    player_sync_position_to_map();

    floor_item_capture_image(0x140, 0x100, 0, 1, KF_FLOOR_ITEM_SCROLLING_IMAGE, 0x40, 0x40);
    floor_item_capture_image(0x198, 0x1c0, 0, 1, KF_FLOOR_ITEM_SCROLLING_IMAGE, 0x20, 0x20);
    floor_item_capture_image(0x1a0, 0x1c0, 0, 4, KF_FLOOR_ITEM_SCROLLING_IMAGE, 0x20, 0x20);
    floor_item_capture_image(0x150, 0x140, 0, 2, KF_FLOOR_ITEM_SCROLLING_IMAGE, 0x40, 0x40);
    floor_item_capture_image(0x150, 0x100, 0, 4, KF_FLOOR_ITEM_SCROLLING_IMAGE, 0x40, 0x40);

    memory_arena_initialize_blocks(KF_GAME_RESOURCE_ARENA_BASE,
                                   KF_GAME_RESOURCE_ARENA_CAPACITY);
    render_frames_with_color_overlay(KF_COLOR_OVERLAY_SUBTRACT | KF_COLOR_OVERLAY_FRONT,
                                     0x1000, 0, -128);
    game_main_exit_flag = GAME_MAIN_RUNNING;

    do {
        reset_collision_rows_and_overlay();
        map_object_update_actions();
        player_update_frame();
        actor_update_frame();
        effect_pool_sweep();
        player_state.force_actor_lifecycle_refresh = KF_FALSE;
        callback_invoke_slot_04_zero();
        resource_advance_transition();
        player_get_camera_pose(&camera_position, &camera_rotation);
        audio_update_listener(&camera_position, &camera_rotation);
        refresh_collision_row_rotations();
        cd_request_service_stream();
        cd_request_service_vab();
        render_game_frame(&camera_position, &camera_rotation);
    } while (game_main_exit_flag != GAME_MAIN_EXIT_REQUESTED);

    game_shutdown();

    kf_psx_overlay_request = KF_OVERLAY_END;
}

void game_shutdown(void)
{
    audio_shutdown();
    cd_close_events();
    PadStop();
    ResetGraph(KF_GPU_RESET_KEEP_DISPLAY);
}
