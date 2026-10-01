#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/kernel.h>
#include <psyq/pad.h>

extern s32 func_800226ec(struct DIRENTRY *entries, s32 *matching_count);
extern s32 func_8001f8b8(void *list_state, s32 label_kind,
    s32 render_mode, s32 item_id);
extern void func_8001cad4(KfMenuGlyphString *rows);
extern void func_8001cb44(KfMenuGlyphString *rows, s32 kind);

ADDRESS(0x8001b834, 0x24c)
s32 func_8001b834(void)
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

    func_800226ec(entries, &matching_count);
    count = func_8001af30(entries, glyph_rows[0].codes,
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
            result = func_8001f8b8(&menu, 6, 8, 0xff);
            if (result == -1)
                result = -99;
            else
                result = slot_ids[menu.list.selected_index];
        }
        if (result != -99)
            break;

        func_8001e484(&menu.list, 0, &mode, &result);
        if (mode == 1)
            func_80022300(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8001fc94(&menu, 8);
            menu_present_frame();
        }
    }

    if (result != -1) {
        func_8001cad4(dialog_rows);
        func_8001b030(9, dialog_rows, 1, 70, 87, 178, 66, 2, 0);
        read_result = func_80022b74(result);
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
