#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

extern s16 menu_row_prefix_649ec[4];
extern s32 func_8001f8b8(void *list_state, s32 label_kind,
    s32 render_mode, s32 item_id);
extern u32 func_8001e484(KfMenuList *list, const u8 *item_ids,
    s32 *selection, s32 *result);
extern void func_8001fc94(const KfMenuRenderList *list, s32 render_mode);

ADDRESS(0x8001a4f0, 0x30c)
void func_8001a4f0(void)
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
    count = func_80018d08(game_counter_bytes, rows, counts, item_ids, 70, 116);
    game_counter_bytes[0x60] = saved_count_60;
    game_counter_bytes[0x61] = saved_count_61;

    count += func_800199d0(effect_state.magic_records, &rows[count],
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
            result = func_8001f8b8(&menu, 5, 16, selected_item);
            if (result == -1)
                result = -99;
            else
                result = menu.list.selected_index;
        }

        if (result != -99)
            break;

        func_8001e484(&menu.list, item_ids, &selection, &result);
        selected_item = item_ids[menu.list.selected_index];
        if (selection == 1)
            func_80022300(17);

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                func_8002083c(selected_item);
            func_8001fc94(&menu, 16);
            menu_present_frame();
        }
    }

    if (result != -1) {
        player_set_unknown_98(magic_ids[result]);
        player_set_unknown_99(item_ids[result]);
    }
}
