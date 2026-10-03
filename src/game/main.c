#include <kf/lib/address.h>
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

DATA(0x80198630, 0x4)
u32 DAT_80198630;

/*
 * GCC inserts the `__main` hook call for a function named main; the SDK
 * start routine tail-calls here. The heap runs from the end of .bss to the
 * stack reserved below the top of RAM.
 */
ADDRESS(0x80013634, 0x68)
void main(void)
{
    InitHeap(BSS_END, OVERLAY_STACK_BOTTOM - (u32)BSS_END);
    CdInit();
    PadInit(0);
    InitCARD(1);
    ChangeClearPAD(0);
    ExitCriticalSection();
    game_main_loop();
}

ADDRESS(0x8001369c, 0x2f0)
void game_main_loop(void)
{
    VECTOR camera_position;
    SVECTOR camera_rotation;

    repeat_store_word((u32 *)&state_8017d118, 0,
        sizeof state_8017d118 / sizeof(u32));
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
    func_800139c4();
    map_object_pool_reset();
    actor_pool_clear();
    effect_pool_reset();
    func_80015d58();
    event_state_initialize();
    memory_card_initialize();
    reset_collision_rows_and_overlay();
    game_graphics_runtime.collision_rotation_dirty = 1;
    refresh_collision_row_rotations();
    SsSetMVol(0x7f, 0x7f);
    if (menu_card_browser() != -1) {
        player_restore_equipment_effects();
    }
    func_80015fd4();
    player_sync_position_to_map();

    floor_item_capture_image(0x140, 0x100, 0, 1, 1, 0x40, 0x40);
    floor_item_capture_image(0x198, 0x1c0, 0, 1, 1, 0x20, 0x20);
    floor_item_capture_image(0x1a0, 0x1c0, 0, 4, 1, 0x20, 0x20);
    floor_item_capture_image(0x150, 0x140, 0, 2, 1, 0x40, 0x40);
    floor_item_capture_image(0x150, 0x100, 0, 4, 1, 0x40, 0x40);

    /* The fixed arena base and exit word have unresolved original owners. */
    memory_arena_initialize_blocks(KF_GAME_RESOURCE_ARENA_BASE,
                                   KF_GAME_RESOURCE_ARENA_CAPACITY);
    func_80036e24(0x82, 0x1000, 0, -128);
    DAT_80198630 = 0;

    do {
        reset_collision_rows_and_overlay();
        func_80036ed4();
        func_8002985c();
        actor_update_frame();
        effect_pool_sweep();
        player_state.unknown_09[1] = 0;
        callback_invoke_slot_04_zero();
        func_80016820();
        player_get_camera_pose(&camera_position, &camera_rotation);
        audio_update_listener(&camera_position, &camera_rotation);
        refresh_collision_row_rotations();
        cd_request_service_stream();
        cd_request_service_vab();
        func_800335a0(&camera_position, &camera_rotation);
    } while (DAT_80198630 != 1);

    game_shutdown();
    /* PSX.EXE owns the fixed next-overlay mailbox. */
    *(u8 *)0x800102f0 = KF_OVERLAY_END;
}
