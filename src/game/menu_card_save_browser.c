#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/kernel.h>

ADDRESS(0x8001bcfc, 0x26c)
void func_8001bcfc(void)
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
    func_800226ec(entries, &matching_count);
    count = func_8001af30(entries, glyph_rows[0].codes,
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
    func_80022300(16);
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
            func_80022300(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8001fc94(&menu, 9);
            menu_present_frame();
        }
    }

    if (result != -1)
        func_8001bf68(slot_ids[result]);
    memory_card_stop();
    menu_exit_display_state(0);
}
