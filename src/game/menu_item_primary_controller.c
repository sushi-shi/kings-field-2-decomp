#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>

extern s32 DAT_8006d694;
extern s32 func_8001f8b8(void *list_state, s32 label_kind,
    s32 render_mode, s32 item_id);

ADDRESS(0x8001d030, 0x310)
void func_8001d030(s32 kind)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;
    u8 *counters = game_counter_bytes;

    if (counters[0x33] == 0)
        menu_item_mask_pages[3][51] = 1;
    else
        menu_item_mask_pages[3][51] = 0;
    count = func_80018d08(menu_item_mask_pages[kind], rows, values, indices,
        0, 119);
    func_8001d340(counters, values, codes, indices, 0, count, kind);
    menu_list_init(&menu.list, 3, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = func_8001f8b8(&menu, 3, 10, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        func_8001e484(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (player_state.gold < (u32)((s32)menu.codes[menu.list.selected_index]
                    * DAT_8006d694)
                    || counters[selected_item] + DAT_8006d694 >= 100) {
                func_80022300(18);
                selection = 0;
            } else {
                func_80022300(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                func_8002083c(selected_item);
            func_8001fc94(&menu, 10);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * DAT_8006d694;
        player_state.gold -= cost;
        counters[result] += (u8)DAT_8006d694;
    }
}
