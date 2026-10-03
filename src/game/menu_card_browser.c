#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/kernel.h>
#include <psyq/libc.h>
#include <psyq/pad.h>

ADDRESS(0x8001b554, 0x2e0)
s32 menu_card_browser(void)
{
    struct DIRENTRY entries[15];
    KfMenuGlyphString rows[4];
    s32 matching_count;
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 card_full;
    s32 probe;
    s32 buttons;
    s32 frame;

    menu_enter_display_state(0);
    func_8001c550(rows);
    func_8001b030(9, rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
    probe = memory_card_probe_temporary_file();
    if (probe != 0 && probe != 2) {
        func_8001ba80(rows);
        func_8001b030(9, rows, 3, 50, 87, 220, 66, 2, 0);
        input_wait_release();
        while (PadRead(1) == 0) {}
        input_wait_release();
        goto no_file;
    }

    card_full = memory_card_scan_save_entries(entries, &matching_count);
    if (matching_count == 0) {
        if (card_full == 1) {
            func_8001bb94(rows);
            func_8001b030(9, rows, 4, 70, 87, 178, 96, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
        }
no_file:
        memory_card_stop();
        menu_exit_display_state(0);
        return -1;
    }

    menu_play_sound_cue(16);
    input_wait_release();
    for (;;) {
        if (selection != -1)
            input_wait_release();
        switch (selection) {
        case 0:
            result = -1;
            break;
        case 1:
            result = menu_card_load_slot_browser();
            if (result == -1) {
                result = -99;
                selection = -1;
            }
            break;
        }
        if (result != -99)
            break;

        func_800223cc();
        buttons = input_read_mark_active();
        if ((buttons & PADLup) || (buttons & PADLdown)) {
            menu_cursor_animation_direction = 0;
            menu_play_sound_cue(16);
            if (cursor == 0)
                cursor = 1;
            else
                cursor = 0;
        } else if (buttons & PADRright) {
            menu_play_sound_cue(17);
            confirmed = 1;
            selection = cursor;
        }
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(6, 2, cursor, confirmed);
            menu_present_frame();
        }
    }
    memory_card_stop();
    menu_exit_display_state(0);
    return result;
}

ADDRESS(0x8001b834, 0x24c)
s32 menu_card_load_slot_browser(void)
{
    struct DIRENTRY entries[15];
    KfCardMenuList menu;
    KfCardSlotGlyphRow glyph_rows[8];
    s32 experience_values[8];
    u8 levels[8];
    s32 slot_ids[8];
    KfMenuGlyphString dialog_rows[3];
    s32 matching_count;
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 read_result;
    s32 frame;

    memory_card_scan_save_entries(entries, &matching_count);
    count = menu_card_build_slot_rows(entries, glyph_rows[0].codes,
        experience_values, levels, slot_ids);
    menu_list_init(&menu.list, 1, 0);
    menu.list.visible_rows = 6;
    menu.list.list_y = 0x83;
    menu.list.entry_count = count;
    menu.rows = glyph_rows;
    menu.values = levels;
    menu.codes = experience_values;

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 6, 8, 0xff);
            if (result == -1)
                result = -99;
            else
                result = slot_ids[menu.list.selected_index];
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8001fc94(&menu, 8);
            menu_present_frame();
        }
    }

    if (result != -1) {
        func_8001cad4(dialog_rows);
        func_8001b030(9, dialog_rows, 1, 70, 87, 178, 66, 2, 0);
        read_result = memory_card_read_slot(result);
        if (read_result != 0) {
            func_8001cb44(dialog_rows, read_result);
            func_8001b030(9, dialog_rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
        }
    }
    return result;
}

ADDRESS(0x8001ba80, 0x114)
void func_8001ba80(KfMenuGlyphString *row)
{
    row->position.x = 70;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[5].codes, sizeof(KfMenuLabelSuffix));
    row->glyphs.codes[8] = 84;
    row->glyphs.codes[9] = 65;
    row->glyphs.codes[10] = 83;
    row->glyphs.codes[11] = -1;
    row++;
    row->position.x = 70;
    row->position.y = 120;
    row->glyphs.codes[0] = 0x1008;
    row->glyphs.codes[1] = 45;
    row->glyphs.codes[2] = 32;
    row->glyphs.codes[3] = 88;
    row->glyphs.codes[4] = 13;
    row->glyphs.codes[5] = 45;
    row->glyphs.codes[6] = 0x101b;
    row->glyphs.codes[7] = 0x1045;
    row->glyphs.codes[8] = -1;
    row++;
    row->position.x = 182;
    row->position.y = 120;
    row->glyphs.codes[0] = 0x1052;
    row->glyphs.codes[1] = 70;
    row->glyphs.codes[2] = 94;
    row->glyphs.codes[3] = 77;
    row->glyphs.codes[4] = 103;
    row->glyphs.codes[5] = -1;
}

ADDRESS(0x8001bb94, 0x168)
void func_8001bb94(KfMenuGlyphString *row)
{
    row->position.x = 104;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[5].codes, sizeof(KfMenuLabelSuffix));
    row++;
    row->position.x = 104;
    row->position.y = 120;
    row->glyphs.codes[0] = 65;
    row->glyphs.codes[1] = 59;
    row->glyphs.codes[2] = 0x2059;
    row->glyphs.codes[3] = 65;
    row->glyphs.codes[4] = 0x1052;
    row->glyphs.codes[5] = 76;
    row->glyphs.codes[6] = -1;
    row++;
    row->position.x = 104;
    row->position.y = 135;
    row->glyphs.codes[0] = 73;
    row->glyphs.codes[1] = 88;
    row->glyphs.codes[2] = 5;
    row->glyphs.codes[3] = 45;
    row->glyphs.codes[4] = 0x1013;
    row->glyphs.codes[5] = 85;
    row->glyphs.codes[6] = 89;
    row->glyphs.codes[7] = -1;
    row++;
    row->position.x = 104;
    row->position.y = 150;
    memcpy(row->glyphs.codes, menu_label_suffixes[2].codes, sizeof(KfMenuLabelSuffix));
}
