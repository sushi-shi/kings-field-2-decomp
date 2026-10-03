#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/libc.h>


ADDRESS(0x8001a2f4, 0x1fc)
void menu_choose_primary_magic_shortcut(void)
{
    KfMagicMenuList menu;
    KfMenuGlyphRow rows[20];
    s32 values[20];
    u8 indices[20];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 frame;

    count = menu_collect_available_magic_rows(effect_state.magic_records, rows, values, indices, 0, 13);
    memcpy(rows[count].codes, menu_row_prefix_649ec, sizeof menu_row_prefix_649ec);
    values[count] = -1;
    indices[count] = 0xff;
    count++;

    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = count;
    menu.list.visible_rows = 6;
    menu.rows = rows;
    menu.values = values;
    menu.list.list_y = 0x83;
    menu.list.glyphs_per_entry = 12;

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 5, 4, 0xff);
            if (result == -1)
                result = -99;
            else
                result = indices[menu.list.selected_index];
        }

        if (result != -99)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 4);
            menu_present_frame();
        }
    }

    if (result != -1)
        player_set_primary_magic_shortcut_id(result);
}

ADDRESS(0x8001a4f0, 0x30c)
void menu_item_magic_controller(void)
{
    KfMenuRenderList menu;
    KfMenuGlyphRow rows[74];
    u8 counts[74];
    s32 numbers[74];
    u8 item_ids[74];
    u8 magic_ids[74];
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 index;
    s32 frame;
    u8 selected_item;
    u8 saved_count_60;
    u8 saved_count_61;

    for (index = 0; index < 74; index++) {
        counts[index] = 0xff;
        numbers[index] = -1;
        item_ids[index] = 0xff;
        magic_ids[index] = 0xff;
    }

    saved_count_60 = game_counter_bytes[0x60];
    saved_count_61 = game_counter_bytes[0x61];
    game_counter_bytes[0x60] = 0;
    game_counter_bytes[0x61] = 0;
    count = menu_collect_masked_item_rows(game_counter_bytes, rows, counts, item_ids, 70, 116);
    game_counter_bytes[0x60] = saved_count_60;
    game_counter_bytes[0x61] = saved_count_61;

    count += menu_collect_available_magic_rows(effect_state.magic_records, &rows[count],
        &numbers[count], &magic_ids[count], 0, 19);
    memcpy(rows[count].codes, menu_row_prefix_649ec, sizeof menu_row_prefix_649ec);
    count++;

    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = count;
    menu.row_glyphs = rows[0].codes;
    menu.byte_values = counts;
    menu.number_values = numbers;
    menu.list.glyphs_per_entry = 12;

    if (menu.list.entry_count != 0
        && menu_load_item_model(item_ids[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 5, 16, selected_item);
            if (result == -1)
                result = -99;
            else
                result = menu.list.selected_index;
        }

        if (result != -99)
            break;

        menu_update_list_input(&menu.list, item_ids, &selection, &result);
        selected_item = item_ids[menu.list.selected_index];
        if (selection == 1)
            menu_play_sound_cue(17);

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 16);
            menu_present_frame();
        }
    }

    if (result != -1) {
        player_set_secondary_magic_shortcut_id(magic_ids[result]);
        player_set_secondary_item_shortcut_id(item_ids[result]);
    }
}

DATA(0x800649ec, 0x8)
s16 menu_row_prefix_649ec[4] = {89, 4172, 76, -1};
