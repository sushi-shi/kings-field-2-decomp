#include <kf/lib/address.h>
#include <kf/lib/types.h>
#include <kf/game/audio.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/audio.h>
#include <psyq/cd.h>
#include <psyq/kernel.h>
#include <psyq/pad.h>

typedef char kf_card_directory_entry_size[sizeof(struct DIRENTRY) == 40 ? 1 : -1];

ADDRESS(0x8001aa9c, 0x1e4)
s32 func_8001aa9c(void)
{
    KfMenuGlyphString labels[2];
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;
    s32 volume;

    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            result = menu_card_load_browser();
            if (result == 0)
                result = -3;
            break;
        case 1:
            result = menu_prompt_two_option();
            if (result == 0)
                result = -2;
            break;
        }

        if (selection != -1 && result == -1)
            result = -99;

        if (result != -99)
            break;

        cursor = menu_poll_choice_input(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(1, 3, cursor, confirmed);
            menu_present_frame();
        }
    }

    if (result == -2) {
        u32 cd_result;

        CdControl(CdlStop, 0, (u8 *)&cd_result);
        volume = 60;
        func_8001ccd4(labels);
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

        menu_prepare_card_browser_rows(dialog_rows);
    menu_show_dialog_panel(4, dialog_rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
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
            menu_render_list(&menu, 8);
            menu_present_frame();
        }
    }

    if (result != -1) {
        menu_prepare_card_read_row(dialog_rows);
        menu_show_dialog_panel(4, dialog_rows, 1, 70, 87, 178, 66, 2, 0);
        read_result = memory_card_read_slot(result);
        if (read_result != 0) {
            menu_prepare_card_read_failure_rows(dialog_rows, read_result);
            menu_show_dialog_panel(4, dialog_rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
            result = -1;
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

    for (index = 0; index < 15; index++) {
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
