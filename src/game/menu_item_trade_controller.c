#include <kf/lib/address.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>


ADDRESS(0x8001d8d0, 0x394)
void func_8001d8d0(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    u8 *counters = game_counter_bytes;
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;

    menu_enter_display_state(1);
    count = func_80018d08(menu_item_mask_pages[4], rows, values, indices,
        0, 119);
    menu_fill_item_counts_and_prices(counters, values, codes, indices, 0, count, 4);
    menu_list_init(&menu.list, 5, 1);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    selected_item = indices[menu.list.selected_index];
    if (menu_load_item_model(selected_item) != 0)
        return;

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        if (menu.list.entry_count != 0)
            func_8002083c(selected_item);
        func_8001fc94(&menu, 15);
        menu_present_frame();
    }
    func_80022300(16);
    input_wait_release();

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 9, 15, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            cost = (s32)menu.codes[menu.list.selected_index] * DAT_8006d694;
            if (counters[96] < cost
                    || (selected_item == 117
                        ? counters[117] + DAT_8006d694 * 10 >= 100
                        : counters[selected_item] + DAT_8006d694 >= 100)) {
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
            func_8001fc94(&menu, 15);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * DAT_8006d694;
        counters[96] -= cost;
        if (result == 117)
            counters[117] += DAT_8006d694 * 10;
        else
            counters[result] += (u8)DAT_8006d694;
    }
    menu_exit_display_state(0);
}
