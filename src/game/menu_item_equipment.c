#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>


ADDRESS(0x8001a898, 0x204)
void func_8001a898(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[120];
    u8 values[120];
    u8 indices[120];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    u8 selected_item;

    count = menu_collect_available_item_rows(game_counter_bytes, rows, values, indices, 0, 119);
    menu_list_init(&menu.list, 0, 4);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = 12;
    if (menu.list.entry_count != 0
        && menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 1, 7,
                indices[menu.list.selected_index]);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }

        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &mode, &result);
        selected_item = indices[menu.list.selected_index];
        if (mode == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            func_8001fc94(&menu, 7);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1)
        game_counter_bytes[result]--;
}
