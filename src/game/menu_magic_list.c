#include <kf/lib/address.h>
#include <kf/game/effect.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

extern s32 func_8001f8b8(void *list_state, s32 label_kind,
    s32 render_mode, s32 item_id);

extern s16 menu_row_prefix_649ec[4];

ADDRESS(0x8001a2f4, 0x1fc)
void func_8001a2f4(void)
{
    KfMagicMenuList menu;
    KfMenuGlyphRow rows[20];
    s32 values[20];
    u8 indices[20];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 frame;

    count = func_800199d0(effect_state.magic_records, rows, values, indices, 0, 13);
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
            result = func_8001f8b8(&menu, 5, 4, 0xff);
            if (result == -1)
                result = -99;
            else
                result = indices[menu.list.selected_index];
        }

        if (result != -99)
            break;

        func_8001e484(&menu.list, 0, &mode, &result);
        if (mode == 1)
            func_80022300(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8001fc94(&menu, 4);
            menu_present_frame();
        }
    }

    if (result != -1)
        player_set_unknown_97(result);
}

DATA(0x800649ec, 0x8)
s16 menu_row_prefix_649ec[4] = {89, 4172, 76, -1};
