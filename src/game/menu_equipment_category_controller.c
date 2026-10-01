#include <kf/lib/address.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

extern s16 menu_row_prefix_649ec[4];
extern s32 func_8001f8b8(void *list_state, s32 label_kind,
    s32 render_mode, s32 item_id);
extern u32 func_8001e484(KfMenuList *list, const u8 *item_ids,
    s32 *selection, s32 *result);
extern void func_8001fc94(const KfItemMenuList *list, s32 render_mode);

RODATA(0x800110b8, 0x4c)

ADDRESS(0x80019ed4, 0x420)
void func_80019ed4(s32 category)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[28];
    u8 values[32];
    u8 item_ids[32];
    s32 selection = 0;
    s32 result = -99;
    s32 first;
    s32 last;
    s32 count;
    s32 frame;
    u8 selected_item;
    u8 equipped_id;

    switch (category) {
    case 0:
        first = 0;
        last = 20;
        break;
    case 2:
        first = 34;
        last = 40;
        break;
    case 3:
        first = 21;
        last = 27;
        break;
    case 4:
        first = 28;
        last = 33;
        break;
    case 5:
        first = 41;
        last = 46;
        break;
    case 6:
        first = 47;
        last = 52;
        break;
    case 7:
    case 8:
        equipped_id = category == 7 ? player_state.equipped_extra_id
                                    : player_state.equipped_accessory_id;
        first = 53;
        if (equipped_id != 0xff)
            game_counter_bytes[equipped_id]--;
        last = 59;
        break;
    }

    count = func_80018d08(game_counter_bytes, rows, values, item_ids,
        first, last);
    memcpy(rows[count].codes, menu_row_prefix_649ec, sizeof menu_row_prefix_649ec);
    values[count] = 0xff;
    item_ids[count] = 0xff;
    count++;

    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = 12;

    if (menu.list.entry_count != 0
        && menu_load_item_model(item_ids[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = func_8001f8b8(&menu, 5, 5, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }

        if (result != -99)
            break;

        func_8001e484(&menu.list, item_ids, &selection, &result);
        selected_item = item_ids[menu.list.selected_index];
        if (selection == 1)
            func_80022300(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                func_8002083c(selected_item);
            func_8001fc94(&menu, 5);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        switch (category) {
        case 0:
            player_equip_weapon((u8)result);
            break;
        case 2:
            player_set_equipment_slot((u8)result, 4);
            break;
        case 3:
            player_set_equipment_slot((u8)result, 0);
            break;
        case 4:
            player_set_equipment_slot((u8)result, 1);
            break;
        case 5:
            player_set_equipment_slot((u8)result, 2);
            break;
        case 6:
            player_set_equipment_slot((u8)result, 3);
            break;
        case 7:
            player_set_equipment_slot((u8)result, 5);
            break;
        case 8:
            player_set_equipment_slot((u8)result, 6);
            break;
        }
    }

    if ((u32)(category - 7) < 2 && equipped_id != 0xff)
        game_counter_bytes[equipped_id]++;
}
