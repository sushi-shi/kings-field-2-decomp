#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/cd.h>
#include <kf/game/memory.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/game/tmd.h>
#include <kf/game/audio.h>
#include <kf/game/card.h>
#include <psyq/audio.h>
#include <psyq/pad.h>

DATA(0x8006d68c, 0x4)
s32 menu_item_model_allocation_pending = 0;

DATA(0x8006d690, 0x4)
s32 input_press_pending = 0;

DATA(0x8006d694, 0x4)
s32 menu_item_quantity = 1;

DATA(0x8006da00, 0x8)
SVECTOR menu_item_preview_translation = {0};

DATA(0x8006da08, 0x8)
SVECTOR menu_item_preview_rotation = {0};

DATA(0x8006da10, 0x4)
s32 menu_item_preview_rotation_step = 0;

ADDRESS(0x800221e8, 0xd4)
s32 menu_load_item_model(u8 item_id)
{
    u8 *allocation;

    menu_item_quantity = 1;
    if (player_state.item_preview_enabled == 0)
        return 0;

    menu_release_item_model();
    if (item_id != 0xff) {
        allocation = memory_allocate(cd_archive_entry_extent(6, item_id, 0));
        cd_archive_read(6, item_id, (u_long *)allocation);
        tmd_register(KF_TMD_SLOT_MENU_ITEM, (KfTmdHeader *)allocation);
        menu_item_model_allocation_pending = KF_MENU_MODEL_ALLOCATED;
    }
    setVector(&menu_item_preview_translation, 0, 0, 1000);
    setVector(&menu_item_preview_rotation, 0, 0, 0);
    menu_item_preview_rotation_step = 4;
    return 0;
}

ADDRESS(0x800222bc, 0x44)
void menu_release_item_model(void)
{
    if (menu_item_model_allocation_pending == KF_MENU_MODEL_ALLOCATED) {
        memory_free((u8 *)game_graphics_runtime.tmd_state.slots[KF_TMD_SLOT_MENU_ITEM]);
        menu_item_model_allocation_pending = KF_MENU_MODEL_RELEASED;
    }
}
ADDRESS(0x80022300, 0x94)
void menu_play_sound_cue(s32 cue)
{
    if (cue == 16) {
        audio_key_on(16, 48, 48, 0);
        SsSeqCalledTbyT();
    } else if ((u32)cue - 17u < 2u) {
        audio_key_on(cue, 48, 48, 0);
        SsSeqCalledTbyT();
    } else if (cue == 13) {
        audio_key_on(13, 0, 58, 0);
        VSync(0);
        VSync(0);
        audio_key_on(13, 58, 0, 0);
    }
}

ADDRESS(0x80022394, 0x38)
u32 input_read_mark_active(void)
{
    u32 buttons = PadRead(1);
    if (buttons != 0) {
        input_press_pending = 1;
    }
    return buttons;
}

ADDRESS(0x800223cc, 0x6c)
void input_wait_brief_release(void)
{
    s32 polls;

    if (input_press_pending == 1) {
        input_press_pending = 0;
        for (polls = 0; PadRead(1) != 0;) {
            if (polls++ < 6) {
                VSync(0);
            } else {
                menu_cursor_animation_frame = 0;
                break;
            }
        }
    }
}
