#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/kernel.h>
#include <psyq/libc.h>
#include <psyq/pad.h>

ADDRESS(0x8001bcfc, 0x26c)
void menu_card_save_browser(void)
{
    struct DIRENTRY entries[15];
    KfCardMenuList menu;
    KfCardSlotGlyphRow glyph_rows[8];
    s32 experience_values[8];
    u8 levels[8];
    s32 slot_ids[8];
    KfMenuGlyphString dialog_rows[2];
    s32 matching_count;
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 frame;

    menu_enter_display_state(1);
    func_8001c550(dialog_rows);
    func_8001cdb0(dialog_rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
    memory_card_probe_temporary_file();
    memory_card_scan_save_entries(entries, &matching_count);
    count = menu_card_build_slot_rows(entries, glyph_rows[0].codes,
        experience_values, levels, slot_ids);
    glyph_rows[count].codes[0] = 0xe0;
    glyph_rows[count].codes[1] = 0xe1;
    glyph_rows[count].codes[2] = 0xe2;
    glyph_rows[count].codes[3] = -1;
    experience_values[count] = -1;
    levels[count] = 0xff;
    slot_ids[count] = 0xff;
    count++;

    menu_list_init(&menu.list, 1, 3);
    menu.list.visible_rows = 6;
    menu.list.list_y = 0x83;
    menu.list.entry_count = count;
    menu.rows = glyph_rows;
    menu.values = levels;
    menu.codes = experience_values;
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();
        if (mode == 1) {
            result = menu_preview_choice(&menu, 7, 9, 0xff);
            if (result == -1)
                result = -99;
            else
                result = menu.list.selected_index;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8001fc94(&menu, 9);
            menu_present_frame();
        }
    }

    if (result != -1)
        menu_card_save_slot(slot_ids[result]);
    memory_card_stop();
    menu_exit_display_state(0);
}

ADDRESS(0x8001bf68, 0x1c4)
void menu_card_save_slot(s32 slot)
{
    KfMenuGlyphString rows[4];
    s32 probe = memory_card_probe_temporary_file();
    s32 result;

    if (probe != 0) {
        if (probe != 2) {
            func_8001c62c(rows);
            func_8001cdb0(rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            goto wait_release;
        }
        if (menu_confirm_card_format(1) == 0) {
            memory_card_format();
            goto write_file;
        }
        func_8001c770(rows);
        func_8001cdb0(rows, 3, 70, 87, 192, 66, 2, 0);
        input_wait_release();
        while (PadRead(1) == 0) {}
        goto wait_release;
    }

write_file:
    func_8001c9f4(rows);
    func_8001cdb0(rows, 2, 70, 87, 178, 66, 2, 0);
    result = memory_card_write_slot(slot);
    if (result == 0)
        return;
    if (result == 1)
        func_8001c62c(rows);
    else
        func_8001c8b0(rows);
    func_8001cdb0(rows, 3, 70, 87, 178, 81, 2, 0);
    input_wait_release();
    while (PadRead(1) == 0) {}

wait_release:
    input_wait_release();
}

ADDRESS(0x8001c12c, 0x424)
s32 menu_confirm_card_format(s32 kind)
{
    KfMenuGlyphString labels[7];
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;

    labels[0].position.x = menu_window_layouts[1].rows[0].position.x;
    labels[0].position.y = menu_window_layouts[1].rows[0].position.y;
    labels[0].glyphs.codes[0] = 89;
    labels[0].glyphs.codes[1] = 65;
    labels[0].glyphs.codes[2] = -1;
    labels[1].position.x = menu_window_layouts[1].rows[1].position.x;
    labels[1].position.y = menu_window_layouts[1].rows[1].position.y;
    labels[1].glyphs.codes[0] = 65;
    labels[1].glyphs.codes[1] = 65;
    labels[1].glyphs.codes[2] = 67;
    labels[1].glyphs.codes[3] = -1;

    if (kind == 1) {
        labels[2].position.x = 90;
        labels[2].position.y = 110;
        memcpy(labels[2].glyphs.codes, menu_label_suffixes[5].codes, sizeof(KfMenuLabelSuffix));
        labels[3].position.x = 90;
        labels[3].position.y = 125;
        memcpy(labels[3].glyphs.codes, menu_label_suffixes[6].codes, sizeof(KfMenuLabelSuffix));
        labels[4].position.x = 174;
        labels[4].position.y = 125;
        memcpy(labels[4].glyphs.codes, menu_label_suffixes[7].codes, sizeof(KfMenuLabelSuffix));
        labels[5].position.x = 90;
        labels[5].position.y = 140;
        memcpy(labels[5].glyphs.codes, menu_label_suffixes[6].codes, sizeof(KfMenuLabelSuffix));
        labels[6].position.x = 174;
        labels[6].position.y = 140;
        memcpy(labels[6].glyphs.codes, menu_label_suffixes[8].codes, sizeof(KfMenuLabelSuffix));
    }

    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();
        switch (selection) {
        case 0:
            result = 0;
            break;
        case 1:
            result = -1;
            break;
        }
        if (result != -99)
            break;

        cursor = menu_poll_choice_input(cursor, 1, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_two_option(&labels[0], &labels[1], cursor, confirmed);
            menu_blit_sprite_translucent(&menu_sprite_defs[5],
                &menu_window_layouts[1].rows[3].position);
            menu_draw_string(&menu_sprite_defs[1], &menu_window_layouts[1].rows[3]);
            if (kind == 1) {
                menu_draw_string(&menu_sprite_defs[1], &labels[2]);
                menu_draw_string(&menu_sprite_defs[1], &labels[3]);
                menu_draw_string(&menu_sprite_defs[1], &labels[4]);
                menu_draw_string(&menu_sprite_defs[1], &labels[5]);
                menu_draw_string(&menu_sprite_defs[1], &labels[6]);
                func_800217f0(70, 92, 220, 81, 2, 0);
            }
            menu_present_frame();
        }
    }
    return result;
}
