#include <kf/lib/address.h>
#include <kf/lib/types.h>
#include <kf/game/audio.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/audio.h>
#include <psyq/cd.h>
#include <psyq/kernel.h>
#include <psyq/libc.h>
#include <psyq/pad.h>
typedef char kf_card_directory_entry_size[sizeof(struct DIRENTRY) == 40 ? 1 : -1];

enum {
    CARD_CHOICE_EXIT = -2,
    CARD_WRITE_IO_FAILURE = 1,
    CARD_MENU_ROW_CAPACITY = KF_CARD_DIRECTORY_CAPACITY / KF_CARD_FILE_BLOCKS + 1,
    CARD_MENU_VISIBLE_ROWS = 6,
    CARD_MENU_LIST_Y = 0x83,
    CARD_PROBE_TEMPORARY_FILE_CREATE_FAILURE = 2,
    CARD_MENU_NO_PREVIEW_ITEM = 0xff,
    CARD_MENU_NO_LEVEL = 0xff,
    CARD_MENU_NEW_SLOT = 0xff
};

ADDRESS(0x8001aa9c, 0x1e4)
s32 menu_run_card_choice(void)
{
    KfMenuGlyphString labels[2];
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 selection = KF_MENU_SELECTION_NONE;
    s32 frame;
    s32 volume;

    for (;;) {
        if (selection != KF_MENU_SELECTION_NONE || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        switch (selection) {
        case 0:
            result = menu_card_load_browser();
            if (result == 0)
                result = KF_MENU_RESULT_GAME_LOADED;
            break;
        case 1:
            result = menu_prompt_two_option();
            if (result == 0)
                result = CARD_CHOICE_EXIT;
            break;
        }

        if (selection != KF_MENU_SELECTION_NONE && result == KF_MENU_RESULT_CANCELLED)
            result = KF_MENU_RESULT_PENDING;

        if (result != KF_MENU_RESULT_PENDING)
            break;

        cursor = menu_poll_choice_input(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(1, 3, cursor, confirmed);
            menu_present_frame();
        }
    }

    if (result == CARD_CHOICE_EXIT) {
        u32 cd_result;

        CdControl(CdlStop, 0, (u8 *)&cd_result);
        volume = 60;
        menu_prepare_card_exit_rows(labels);
        for (;;) {
            menu_show_dialog_panel(5, labels, 2, 70, 87, 178, 66, 2, 0);
            if (volume > 0) {
                volume--;
                SsSeqSetVol(audio_state.sequence_id, volume, volume);
            }
        }
    }
    return result;
}

ADDRESS(0x8001ac80, 0x2b0)
s32 menu_card_load_browser(void)
{
    struct DIRENTRY entries[KF_CARD_DIRECTORY_CAPACITY];
    KfCardMenuList menu;
    KfCardSlotGlyphRow glyph_rows[CARD_MENU_ROW_CAPACITY];
    s32 experience_values[CARD_MENU_ROW_CAPACITY];
    u8 levels[CARD_MENU_ROW_CAPACITY];
    s32 slot_ids[CARD_MENU_ROW_CAPACITY];
    KfMenuGlyphString dialog_rows[3];
    s32 matching_count;
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 read_result;
    s32 frame;

    menu_prepare_card_browser_rows(dialog_rows);
    menu_show_dialog_panel(4, dialog_rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
    memory_card_scan_save_entries(entries, &matching_count);
    count = menu_card_build_slot_rows(entries, glyph_rows[0].codes,
        experience_values, levels, slot_ids);
    menu_list_init(&menu.list, 1, 0);
    menu.list.visible_rows = CARD_MENU_VISIBLE_ROWS;
    menu.list.list_y = CARD_MENU_LIST_Y;
    menu.list.entry_count = count;
    menu.rows = glyph_rows;
    menu.levels = levels;
    menu.experience_values = experience_values;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 6, 8, CARD_MENU_NO_PREVIEW_ITEM);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = slot_ids[menu.list.selected_index];
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 8);
            menu_present_frame();
        }
    }

    if (result != KF_MENU_RESULT_CANCELLED) {
        menu_prepare_card_read_row(dialog_rows);
        menu_show_dialog_panel(4, dialog_rows, 1, 70, 87, 178, 66, 2, 0);
        read_result = memory_card_read_slot(result);
        if (read_result != 0) {
            menu_prepare_card_read_failure_rows(dialog_rows, read_result);
            menu_show_dialog_panel(4, dialog_rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
            result = KF_MENU_RESULT_CANCELLED;
        } else {
            result = 0;
        }
    }
    memory_card_stop();
    return result;
}

ADDRESS(0x8001af30, 0x100)
s32 menu_card_build_slot_rows(const struct DIRENTRY *card_entries, s16 *glyph_rows,
    s32 *experience_values, u8 *levels, s32 *slot_ids)
{
    s32 count = 0;
    s32 index;
    s32 experience;
    s32 level;
    s32 slot_id;

    for (index = 0; index < KF_CARD_DIRECTORY_CAPACITY; index++) {
        if (memory_card_read_slot_summary(card_entries->name,
            &experience, &level, &slot_id) == 0) {
            *glyph_rows++ = 0x1012;
            *glyph_rows++ = 0x2d;
            *glyph_rows++ = 0xf;
            *glyph_rows++ = slot_id + 229;
            *glyph_rows = -1;
            glyph_rows += 6;
            *experience_values++ = experience;
            *levels++ = level;
            *slot_ids++ = slot_id;
            count++;
        }
        ++card_entries;
    }
    return count;
}

ADDRESS(0x8001b030, 0x11c)
void menu_show_dialog_panel(s32 panel, const KfMenuGlyphString *rows, s32 count,
    s32 detail0, s32 detail1, s32 detail2, s32 detail3, s32 detail4,
    s32 detail5)
{
    s32 frame = 0;
    const KfMenuGlyphString *title = &menu_window_layouts[1].rows[panel];
    const KfMenuGlyphString *current;
    s32 row;

    do {
        menu_frame_begin();
        if (panel < 6) {
            menu_blit_sprite_translucent(&menu_sprite_defs[5], &title->position);
            menu_draw_string(&menu_sprite_defs[1], title);
        }
        current = rows;
        for (row = 0; row < count; row++, current++)
            menu_draw_string(&menu_sprite_defs[1], current);
        menu_draw_nine_slice_panel(detail0, detail1, detail2, detail3, detail4, detail5);
        menu_present_frame();
        frame++;
    } while (frame < 2);
}

ADDRESS(0x8001b14c, 0x190)
s32 menu_prompt_two_option(void)
{
    KfMenuGlyphString labels[2];
    s32 choice;
    s32 selected;
    s32 result;
    s32 current;
    s32 frame;
    s32 cursor_frame;

    current = 0;
    choice = 0;
    result = KF_MENU_RESULT_PENDING;
    selected = KF_MENU_SELECTION_NONE;
    labels[0].position.x = 101;
    labels[0].position.y = 123;
    labels[0].glyphs.codes[0] = 89;
    labels[0].glyphs.codes[1] = 65;
    labels[0].glyphs.codes[2] = -1;
    labels[1].position.x = 101;
    labels[1].position.y = 149;
    labels[1].glyphs.codes[0] = 65;
    labels[1].glyphs.codes[1] = 65;
    labels[1].glyphs.codes[2] = 67;
    labels[1].glyphs.codes[3] = -1;

    for (;;) {
        if (selected != KF_MENU_SELECTION_NONE || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        switch (selected) {
        case 0:
            result = 0;
            break;
        case 1:
            result = -1;
            break;
        }

        if (result != KF_MENU_RESULT_PENDING)
            break;
        current = menu_poll_choice_input(current, 1, &selected, &choice, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            cursor_frame = menu_cursor_animation_frame;
            menu_cursor_animation_frame = 0;
            menu_draw_window(1, 3, 1, 1);
            menu_cursor_animation_frame = cursor_frame;
            menu_draw_two_option(&labels[0], &labels[1], current, choice);
            menu_present_frame();
        }
    }
    return result;
}
enum {
    KF_MENU_OPTION_COUNT = 6,
    KF_MENU_OPTION_CANCEL_ROW = 6
};

ADDRESS(0x8001b2dc, 0x278)
void menu_options_controller(void)
{
    u8 selected[KF_MENU_OPTION_COUNT];
    KfMenuGlyphString labels[2];
    s32 choice = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 last_row = KF_MENU_OPTION_COUNT;
    s32 confirmed;
    s32 frame;
    u32 buttons;

    labels[0].position.x = 180;
    labels[0].position.y = 45;
    labels[0].glyphs.codes[0] = 240;
    labels[0].glyphs.codes[1] = 241;
    labels[0].glyphs.codes[2] = KF_MENU_TEXT_END;
    labels[1].position.x = 254;
    labels[1].position.y = 45;
    labels[1].glyphs.codes[0] = 242;
    labels[1].glyphs.codes[1] = 243;
    labels[1].glyphs.codes[2] = 244;
    labels[1].glyphs.codes[3] = KF_MENU_TEXT_END;

    selected[0] = player_state.audio_effects_enabled;
    selected[1] = player_state.audio_music_enabled;
    selected[2] = player_state.hud_gauges_enabled;
    selected[3] = player_state.compass_enabled;
    selected[4] = player_state.item_preview_enabled;
    selected[5] = player_state.walking_bob_enabled;

    for (;;) {
        if (result != KF_MENU_RESULT_PENDING) {
            input_wait_release();
            break;
        }

        input_wait_brief_release();
        buttons = input_read_mark_active();
        confirmed = 0;
        if (buttons & PADLup) {
            menu_cursor_animation_direction = 0;
            menu_play_sound_cue(16);
            if (choice != 0)
                choice--;
            else
                choice = last_row;
        } else if (buttons & PADLdown) {
            menu_cursor_animation_direction = 0;
            menu_play_sound_cue(16);
            if (choice != last_row)
                choice++;
            else
                choice = 0;
        } else if ((buttons & PADRright) || (buttons & PADLright)
            || (buttons & PADLleft)) {
            if (choice < last_row) {
                menu_play_sound_cue(17);
                confirmed = 1;
                selected[choice] = selected[choice] == 0;
            } else if (buttons & PADRright) {
                menu_play_sound_cue(17);
                result = -1;
                confirmed = 1;
            }
        } else if (buttons & PADRdown) {
            menu_play_sound_cue(18);
            result = -1;
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(2, 7, choice, confirmed);
            menu_draw_options_rows(&labels[0], &labels[1], selected);
            menu_present_frame();
        }
    }

    player_state.audio_effects_enabled = selected[0];
    player_state.audio_music_enabled = selected[1];
    player_state.hud_gauges_enabled = selected[2];
    player_state.compass_enabled = selected[3];
    player_state.item_preview_enabled = selected[4];
    player_state.walking_bob_enabled = selected[5];
}

ADDRESS(0x8001b554, 0x2e0)
s32 menu_card_browser(void)
{
    struct DIRENTRY entries[KF_CARD_DIRECTORY_CAPACITY];
    KfMenuGlyphString rows[4];
    s32 matching_count;
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 selection = KF_MENU_SELECTION_NONE;
    s32 card_full;
    s32 probe;
    s32 buttons;
    s32 frame;

    menu_enter_display_state(0);
    menu_prepare_card_browser_rows(rows);
    menu_show_dialog_panel(9, rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
    probe = memory_card_probe_temporary_file();
    if (probe != 0 && probe != CARD_PROBE_TEMPORARY_FILE_CREATE_FAILURE) {
        menu_build_card_probe_error_rows(rows);
        menu_show_dialog_panel(9, rows, 3, 50, 87, 220, 66, 2, 0);
        input_wait_release();
        while (PadRead(1) == 0) {}
        input_wait_release();
        goto no_file;
    }

    card_full = memory_card_scan_save_entries(entries, &matching_count);
    if (matching_count == 0) {
        if (card_full == 1) {
            menu_build_card_full_rows(rows);
            menu_show_dialog_panel(9, rows, 4, 70, 87, 178, 96, 2, 0);
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
        if (selection != KF_MENU_SELECTION_NONE)
            input_wait_release();
        switch (selection) {
        case 0:
            result = -1;
            break;
        case 1:
            result = menu_card_load_slot_browser();
            if (result == KF_MENU_RESULT_CANCELLED) {
                result = KF_MENU_RESULT_PENDING;
                selection = KF_MENU_SELECTION_NONE;
            }
            break;
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        input_wait_brief_release();
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
    struct DIRENTRY entries[KF_CARD_DIRECTORY_CAPACITY];
    KfCardMenuList menu;
    KfCardSlotGlyphRow glyph_rows[CARD_MENU_ROW_CAPACITY];
    s32 experience_values[CARD_MENU_ROW_CAPACITY];
    u8 levels[CARD_MENU_ROW_CAPACITY];
    s32 slot_ids[CARD_MENU_ROW_CAPACITY];
    KfMenuGlyphString dialog_rows[3];
    s32 matching_count;
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 read_result;
    s32 frame;

    memory_card_scan_save_entries(entries, &matching_count);
    count = menu_card_build_slot_rows(entries, glyph_rows[0].codes,
        experience_values, levels, slot_ids);
    menu_list_init(&menu.list, 1, 0);
    menu.list.visible_rows = CARD_MENU_VISIBLE_ROWS;
    menu.list.list_y = CARD_MENU_LIST_Y;
    menu.list.entry_count = count;
    menu.rows = glyph_rows;
    menu.levels = levels;
    menu.experience_values = experience_values;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 6, 8, CARD_MENU_NO_PREVIEW_ITEM);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = slot_ids[menu.list.selected_index];
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 8);
            menu_present_frame();
        }
    }

    if (result != -1) {
        menu_prepare_card_read_row(dialog_rows);
        menu_show_dialog_panel(9, dialog_rows, 1, 70, 87, 178, 66, 2, 0);
        read_result = memory_card_read_slot(result);
        if (read_result != 0) {
            menu_prepare_card_read_failure_rows(dialog_rows, read_result);
            menu_show_dialog_panel(9, dialog_rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
        }
    }
    return result;
}

ADDRESS(0x8001ba80, 0x114)
void menu_build_card_probe_error_rows(KfMenuGlyphString *row)
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
void menu_build_card_full_rows(KfMenuGlyphString *row)
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

ADDRESS(0x8001bcfc, 0x26c)
void menu_card_save_browser(void)
{
    struct DIRENTRY entries[KF_CARD_DIRECTORY_CAPACITY];
    KfCardMenuList menu;
    KfCardSlotGlyphRow glyph_rows[CARD_MENU_ROW_CAPACITY];
    s32 experience_values[CARD_MENU_ROW_CAPACITY];
    u8 levels[CARD_MENU_ROW_CAPACITY];
    s32 slot_ids[CARD_MENU_ROW_CAPACITY];
    KfMenuGlyphString dialog_rows[2];
    s32 matching_count;
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 frame;

    menu_enter_display_state(1);
    menu_prepare_card_browser_rows(dialog_rows);
    menu_draw_card_dialog_rows(dialog_rows, 2, 70, 87, 178, 66, 2, 0);
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
    levels[count] = CARD_MENU_NO_LEVEL;
    slot_ids[count] = CARD_MENU_NEW_SLOT;
    count++;

    menu_list_init(&menu.list, 1, 3);
    menu.list.visible_rows = CARD_MENU_VISIBLE_ROWS;
    menu.list.list_y = CARD_MENU_LIST_Y;
    menu.list.entry_count = count;
    menu.rows = glyph_rows;
    menu.levels = levels;
    menu.experience_values = experience_values;
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();
        if (mode == 1) {
            result = menu_preview_choice(&menu, 7, 9, CARD_MENU_NO_PREVIEW_ITEM);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = menu.list.selected_index;
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 9);
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
        if (probe != CARD_PROBE_TEMPORARY_FILE_CREATE_FAILURE) {
            menu_prepare_card_io_error_rows(rows);
            menu_draw_card_dialog_rows(rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            goto wait_release;
        }
        if (menu_confirm_card_format(1) == 0) {
            memory_card_format();
            goto write_file;
        }
        menu_prepare_card_format_declined_rows(rows);
        menu_draw_card_dialog_rows(rows, 3, 70, 87, 192, 66, 2, 0);
        input_wait_release();
        while (PadRead(1) == 0) {}
        goto wait_release;
    }

write_file:
    menu_prepare_card_write_rows(rows);
    menu_draw_card_dialog_rows(rows, 2, 70, 87, 178, 66, 2, 0);
    result = memory_card_write_slot(slot);
    if (result == 0)
        return;
    if (result == CARD_WRITE_IO_FAILURE)
        menu_prepare_card_io_error_rows(rows);
    else
        menu_prepare_card_write_full_rows(rows);
    menu_draw_card_dialog_rows(rows, 3, 70, 87, 178, 81, 2, 0);
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
    s32 result = KF_MENU_RESULT_PENDING;
    s32 selection = KF_MENU_SELECTION_NONE;
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
        if (selection != KF_MENU_SELECTION_NONE || result != KF_MENU_RESULT_PENDING)
            input_wait_release();
        switch (selection) {
        case 0:
            result = 0;
            break;
        case 1:
            result = -1;
            break;
        }
        if (result != KF_MENU_RESULT_PENDING)
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
                menu_draw_nine_slice_panel(70, 92, 220, 81, 2, 0);
            }
            menu_present_frame();
        }
    }
    return result;
}

DATA(0x80064af0, 0x140)
KfMenuLabelSuffix menu_label_suffixes[16] = {
    {{33, 34, 41, 45, 5, 45, 4115, 88, -1, 0}},
    {{16, 51, 53, 7, 109, 75, 82, 65, 94, 76}},
    {{13, 45, 4123, 4178, 70, 94, 77, 103, -1, 0}},
    {{33, 34, 41, 45, 5, 45, 4115, 109, -1, 0}},
    {{16, 51, 53, 7, 75, 82, 71, 4175, 74, 65}},
    {{33, 34, 41, 45, 5, 45, 4115, 4165, -1, 0}},
    {{27, 52, 45, 30, 53, 19, -1, 0, 0, 0}},
    {{74, 107, 82, 65, 94, 77, 103, -1, 0, 0}},
    {{75, 94, 76, 69, -1, 0, 0, 0, 0, 0}},
    {{109, 75, 84, 65, 83, -1, 0, 0, 0, 0}},
    {{44, 45, 4115, 4178, 70, 94, 77, 103, -1, 0}},
    {{13, 45, 4123, 75, 82, 65, 94, 76, -1, 0}},
    {{267, 268, 4165, 79, 105, 94, 77, 103, -1, 0}},
    {{44, 45, 4115, 75, 82, 65, 94, 76, -1, 0}},
    {{269, 270, 109, 86, 65, 82, -1, 0, 0, 0}},
    {{161, 271, 109, 263, 59, 82, 71, 4175, 74, 65}}
};

ADDRESS(0x8001c550, 0xdc)
void menu_prepare_card_browser_rows(KfMenuGlyphString *row)
{
    row->position.x = 90;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[0].codes,
        sizeof menu_label_suffixes[0]);
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[1].codes,
        sizeof menu_label_suffixes[1]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001c62c, 0x144)
void menu_prepare_card_io_error_rows(KfMenuGlyphString *row)
{
    row->position.x = 90;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[2].codes,
        sizeof menu_label_suffixes[2]);
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[3].codes,
        sizeof menu_label_suffixes[3]);
    row++;
    row->position.x = 90;
    row->position.y = 135;
    memcpy(row->glyphs.codes, menu_label_suffixes[4].codes,
        sizeof menu_label_suffixes[4]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001c770, 0x140)
void menu_prepare_card_format_declined_rows(KfMenuGlyphString *row)
{
    row->position.x = 90;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[6].codes,
        sizeof menu_label_suffixes[6]);
    row++;
    row->position.x = 174;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[9].codes,
        sizeof menu_label_suffixes[9]);
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[2].codes,
        sizeof menu_label_suffixes[2]);
}

ADDRESS(0x8001c8b0, 0x144)
void menu_prepare_card_write_full_rows(KfMenuGlyphString *row)
{
    row->position.x = 102;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[2].codes,
        sizeof menu_label_suffixes[2]);
    row++;
    row->position.x = 102;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[0].codes,
        sizeof menu_label_suffixes[0]);
    row++;
    row->position.x = 102;
    row->position.y = 135;
    memcpy(row->glyphs.codes, menu_label_suffixes[12].codes,
        sizeof menu_label_suffixes[12]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001c9f4, 0xe0)
void menu_prepare_card_write_rows(KfMenuGlyphString *row)
{
    row->position.x = 102;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[0].codes,
        sizeof menu_label_suffixes[0]);
    row->glyphs.codes[7] = 85;
    row++;
    row->position.x = 102;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[11].codes,
        sizeof menu_label_suffixes[11]);
}

ADDRESS(0x8001cad4, 0x70)
void menu_prepare_card_read_row(KfMenuGlyphString *row)
{
    row->position.x = 102;
    row->position.y = 112;
    memcpy(row->glyphs.codes, menu_label_suffixes[13].codes,
        sizeof menu_label_suffixes[13]);
}

ADDRESS(0x8001cb44, 0x190)
void menu_prepare_card_read_failure_rows(KfMenuGlyphString *row, s32 kind)
{
    row->position.x = 90;
    row->position.y = 105;
    if (kind == 1) {
        memcpy(row->glyphs.codes, menu_label_suffixes[10].codes,
            sizeof menu_label_suffixes[10]);
    } else {
        row->glyphs.codes[0] = 0x1012;
        row->glyphs.codes[1] = 45;
        row->glyphs.codes[2] = 15;
        row->glyphs.codes[3] = 3;
        row->glyphs.codes[4] = 40;
        row->glyphs.codes[5] = 45;
        row->glyphs.codes[6] = -1;
    }
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[3].codes,
        sizeof menu_label_suffixes[3]);
    row++;
    row->position.x = 90;
    row->position.y = 135;
    memcpy(row->glyphs.codes, menu_label_suffixes[4].codes,
        sizeof menu_label_suffixes[4]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001ccd4, 0xdc)
void menu_prepare_card_exit_rows(KfMenuGlyphString *row)
{
    row->position.x = 90;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[14].codes,
        sizeof menu_label_suffixes[14]);
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[15].codes,
        sizeof menu_label_suffixes[15]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001cdb0, 0x108)
void menu_draw_card_dialog_rows(const KfMenuGlyphString *rows, s32 count,
    s32 x, s32 y, s32 width, s32 height, s32 overlap_x, s32 overlap_y)
{
    const KfMenuGlyphString *current;
    s32 frame;
    s32 row;

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_blit_sprite_translucent(&menu_sprite_defs[5],
            &menu_window_layouts[1].rows[3].position);
        menu_draw_string(&menu_sprite_defs[1], &menu_window_layouts[1].rows[3]);
        current = rows;
        for (row = 0; row < count; row++, current++)
            menu_draw_string(&menu_sprite_defs[1], current);
        menu_draw_nine_slice_panel(x, y, width, height, overlap_x, overlap_y);
        menu_present_frame();
    }
}
