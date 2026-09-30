#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>

extern s32 DAT_8006d694;

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
    } else if (buttons & 0x1000) {
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
    } else if (buttons & 0x4000) {
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
    } else if (buttons & 0x2000) {
        if (DAT_8006d694 < 99) {
            func_80022300(16);
            DAT_8006d694++;
        }
    } else if (buttons & 0x8000) {
        if (DAT_8006d694 > 1) {
            func_80022300(16);
            DAT_8006d694--;
        }
    } else if (buttons & 0x20) {
        *selection = 1;
    } else if (buttons & 0x40) {
        func_80022300(18);
        *result = -1;
    }

    if (buttons & 0x100) {
        if (buttons & 8) {
            input_idle_counter = 0;
            menu_item_preview_rotation.vx += 16;
        }
        if (buttons & 2) {
            input_idle_counter = 0;
            menu_item_preview_rotation.vx -= 16;
        }
        if (buttons & 4) {
            input_idle_counter = 0;
            menu_item_preview_rotation.vz += 16;
        }
        if (buttons & 1) {
            input_idle_counter = 0;
            menu_item_preview_rotation.vz -= 16;
        }
        if (buttons & 0x10) {
            input_idle_counter = 0;
            menu_item_preview_rotation_step++;
        }
        if (buttons & 0x80) {
            input_idle_counter = 0;
            menu_item_preview_rotation_step--;
        }
    }

    if (buttons & 0x800) {
        if (buttons & 8) {
            input_idle_counter = 0;
            menu_item_preview_translation.vx += 16;
        }
        if (buttons & 2) {
            input_idle_counter = 0;
            menu_item_preview_translation.vx -= 16;
        }
        if (buttons & 4) {
            input_idle_counter = 0;
            menu_item_preview_translation.vy += 16;
        }
        if (buttons & 1) {
            input_idle_counter = 0;
            menu_item_preview_translation.vy -= 16;
        }
        if (buttons & 0x10) {
            input_idle_counter = 0;
            menu_item_preview_translation.vz += 16;
        }
        if (buttons & 0x80) {
            if (menu_item_preview_translation.vz > 500)
                menu_item_preview_translation.vz -= 16;
            input_idle_counter = 0;
        }
    }

    return buttons;
}
