#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/effect.h>
#include <kf/game/menu.h>


ADDRESS(0x80019834, 0x19c)
s32 menu_choose_magic_action(void)
{
    KfMagicMenuList menu;
    KfMenuGlyphRow rows[20];
    s32 values[20];
    u8 indices[20];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 frame;

    count = menu_collect_available_magic_rows(effect_state.magic_records, rows, values, indices, 14, 19);
    menu_list_init(&menu.list, 0, 1);
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
            result = menu_preview_choice(&menu, 0, 2, 0xff);
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
            menu_render_list(&menu, 2);
            menu_present_frame();
        }
    }

    if (result != -1)
        result |= 0x1000;
    return result;
}

ADDRESS(0x800199d0, 0xf4)
s32 menu_collect_available_magic_rows(const KfMagicRecord *records,
    KfMenuGlyphRow *rows, s32 *values, u8 *indices, s32 first, s32 last)
{
    const KfMenuGlyphRow *glyph = &menu_glyph_rows_extra[first];
    const KfMagicRecord *entry = &records[first];
    s32 count = 0;
    s32 index;

    last++;
    index = first;
    while (index < last) {
        if (entry->menu_available == 1) {
            *rows++ = *glyph;
            *values = entry->mp_cost;
            *indices = index;
            values++;
            indices++;
            count++;
        }
        entry++;
        glyph++;
        index++;
    }
    return count;
}
