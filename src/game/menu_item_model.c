#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/cd.h>
#include <kf/game/memory.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/game/tmd.h>


ADDRESS(0x800221e8, 0xd4)
s32 menu_load_item_model(u8 item_id)
{
    u8 *allocation;

    DAT_8006d694 = 1;
    if (player_state.unknown_c9[2] == 0)
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
