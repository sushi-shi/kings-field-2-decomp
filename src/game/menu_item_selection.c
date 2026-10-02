#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>

ADDRESS(0x80018ac8, 0x240)
s32 func_80018ac8(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[54];
    u8 values[56];
    u8 indices[56];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    u8 selected_item;

    count = func_80018d08(game_counter_bytes, rows, values, indices, 67, 119);
    menu_list_init(&menu.list, 0, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = 12;
    if (menu.list.entry_count != 0
        && menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return -1;

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 0, 1, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }

        if (result != -99)
            break;

        func_8001e484(&menu.list, indices, &mode, &result);
        selected_item = indices[menu.list.selected_index];
        if (mode == 1) {
            func_80022300(17);
            if (selected_item == 67 || selected_item == 68 || selected_item == 69) {
                menu_release_item_model();
                menu_show_map_preview(selected_item);
                func_80022300(18);
                if (menu_load_item_model(selected_item) != 0)
                    return -1;
                mode = 0;
            }
        }
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                func_8002083c(selected_item);
            func_8001fc94(&menu, 1);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if ((u32)(result - 71) < 10)
        func_80018f8c(result);
    return result;
}
