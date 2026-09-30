#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>

extern void func_8001f008(void);

ADDRESS(0x8001a7fc, 0x9c)
void func_8001a7fc(void)
{
    s32 current = -99;
    s32 previous = -99;
    s32 frame;

    for (;;) {
        if (current != previous) {
            input_wait_release();
            return;
        }
        func_800223cc();
        if (input_read_mark_active()) {
            func_80022300(18);
            current = -1;
        }
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8001f008();
            menu_present_frame();
        }
    }
}
