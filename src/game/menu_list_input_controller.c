#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/pad.h>


ADDRESS(0x8001e484, 0x4c8)
u32 func_8001e484(KfMenuList *list, const u8 *item_ids,
    s32 *selection, s32 *result)
{
    u32 buttons;

    *selection = 0;
    func_800223cc();
    buttons = input_read_mark_active();

    if (list->entry_count == 0) {
        if (buttons != 0) {
            func_80022300(18);
            *result = -1;
        }
    } else if (buttons & PADLup) {
        func_80022300(16);
        if (list->selected_index != 0) {
            list->selected_index--;
            if (list->cursor_row == 0)
                list->scroll_offset--;
            else
                list->cursor_row--;
        } else {
            list->selected_index = list->entry_count - 1;
            if (list->entry_count < list->visible_rows) {
                list->scroll_offset = 0;
                list->cursor_row = list->entry_count - 1;
            } else {
                list->scroll_offset = list->entry_count - list->visible_rows;
                list->cursor_row = list->visible_rows - 1;
            }
        }
        if (item_ids != 0 && menu_load_item_model(item_ids[list->selected_index]) != 0)
            *result = -1;
    } else if (buttons & PADLdown) {
        func_80022300(16);
        if (list->selected_index < list->entry_count - 1) {
            list->selected_index++;
            if (list->cursor_row == list->visible_rows - 1)
                list->scroll_offset++;
            else
                list->cursor_row++;
        } else {
            list->selected_index = 0;
            list->scroll_offset = 0;
            list->cursor_row = 0;
        }
        if (item_ids != 0 && menu_load_item_model(item_ids[list->selected_index]) != 0)
            *result = -1;
    } else if (buttons & PADLright) {
        if (DAT_8006d694 < 99) {
            func_80022300(16);
            DAT_8006d694++;
        }
    } else if (buttons & PADLleft) {
        if (DAT_8006d694 > 1) {
            func_80022300(16);
            DAT_8006d694--;
        }
    } else if (buttons & PADRright) {
        *selection = 1;
    } else if (buttons & PADRdown) {
        func_80022300(18);
        *result = -1;
    }

    if (buttons & PADselect) {
        if (buttons & PADR1) {
            input_idle_counter = 0;
            menu_item_preview_rotation.vx += 16;
        }
        if (buttons & PADR2) {
            input_idle_counter = 0;
            menu_item_preview_rotation.vx -= 16;
        }
        if (buttons & PADL1) {
            input_idle_counter = 0;
            menu_item_preview_rotation.vz += 16;
        }
        if (buttons & PADL2) {
            input_idle_counter = 0;
            menu_item_preview_rotation.vz -= 16;
        }
        if (buttons & PADRup) {
            input_idle_counter = 0;
            menu_item_preview_rotation_step++;
        }
        if (buttons & PADRleft) {
            input_idle_counter = 0;
            menu_item_preview_rotation_step--;
        }
    }

    if (buttons & PADstart) {
        if (buttons & PADR1) {
            input_idle_counter = 0;
            menu_item_preview_translation.vx += 16;
        }
        if (buttons & PADR2) {
            input_idle_counter = 0;
            menu_item_preview_translation.vx -= 16;
        }
        if (buttons & PADL1) {
            input_idle_counter = 0;
            menu_item_preview_translation.vy += 16;
        }
        if (buttons & PADL2) {
            input_idle_counter = 0;
            menu_item_preview_translation.vy -= 16;
        }
        if (buttons & PADRup) {
            input_idle_counter = 0;
            menu_item_preview_translation.vz += 16;
        }
        if (buttons & PADRleft) {
            if (menu_item_preview_translation.vz > 500)
                menu_item_preview_translation.vz -= 16;
            input_idle_counter = 0;
        }
    }

    return buttons;
}
