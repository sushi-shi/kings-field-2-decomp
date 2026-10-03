#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>

ADDRESS(0x8001a7fc, 0x9c)
void menu_show_combat_attributes(void)
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
            menu_play_sound_cue(18);
            current = -1;
        }
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8001f008();
            menu_present_frame();
        }
    }
}
