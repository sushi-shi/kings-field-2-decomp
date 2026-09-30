#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>

extern void func_8001ddd0(void);
extern void func_8001e0a8(void);

ADDRESS(0x8001dc64, 0x16c)
void func_8001dc64(void)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;

    func_80021c8c(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_window(4, 3, cursor, confirmed);
        menu_present_frame();
    }
    func_80022300(16);
    input_wait_release();
    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            func_8001ddd0();
            break;
        case 1:
            func_8001e0a8();
            break;
        }

        if (result != -99)
            break;
        cursor = func_8001e378(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(4, 3, cursor, confirmed);
            menu_present_frame();
        }
    }
    func_80021e00(0);
}
