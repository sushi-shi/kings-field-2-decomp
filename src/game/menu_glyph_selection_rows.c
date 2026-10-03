#include <kf/lib/address.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>

ADDRESS(0x80019ce4, 0x1f0)
void func_80019ce4(KfMenuLabelSuffix *rows)
{
    u8 selected[10];
    u8 *entry;
    s32 i;

    selected[0] = player_state.equipped_weapon_id;
    selected[1] = player_state.primary_magic_shortcut_id;
    selected[2] = player_state.equipped_arm_id;
    selected[3] = player_state.equipped_head_id;
    selected[4] = player_state.equipped_body_id;
    selected[5] = player_state.equipped_leg_id;
    selected[6] = player_state.equipped_shield_id;
    selected[7] = player_state.equipped_accessory_id;
    selected[8] = player_state.equipped_extra_id;
    if (player_state.secondary_magic_shortcut_id == 0xff)
        selected[9] = player_state.secondary_item_shortcut_id;
    else
        selected[9] = player_state.secondary_magic_shortcut_id;

    entry = selected;
    for (i = 0; i < 10; rows++, i++, entry++) {
        u32 id = *entry;

        if (id == 0xff)
            goto missing;
        if (i == 1)
            goto extra;
        if (i != 9)
            goto base;
        if (player_state.secondary_magic_shortcut_id == 0xff)
            goto base;
    extra:
        *rows = *(const KfMenuLabelSuffix *)menu_glyph_rows_extra[id].codes;
        goto next;
    base:
        *rows = *(const KfMenuLabelSuffix *)menu_glyph_rows[id].codes;
        goto next;
    missing:
        rows->codes[0] = -1;
    next:;
    }
}
