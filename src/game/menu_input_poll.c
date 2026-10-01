#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/pad.h>

ADDRESS(0x8001e378, 0x10c)
s32 func_8001e378(s32 index, s32 last, s32 *selection, s32 *confirmed,
    s32 *cancelled)
{
    u32 buttons;

    *selection = -1;
    *confirmed = 0;
    func_800223cc();
    buttons = input_read_mark_active();
    if (buttons & PADLup) {
        menu_cursor_animation_direction = 0;
        func_80022300(16);
        if (index != 0)
            index--;
        else
            index = last;
    } else if (buttons & PADLdown) {
        menu_cursor_animation_direction = 0;
        func_80022300(16);
        if (index != last)
            index++;
        else
            index = 0;
    } else if (buttons & PADRright) {
        func_80022300(17);
        *confirmed = 1;
        if (index < last)
            *selection = index;
        else
            *cancelled = -1;
    } else if (buttons & PADRdown) {
        func_80022300(18);
        *cancelled = -1;
    }
    return index;
}
