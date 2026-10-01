#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/card.h>
#include <psyq/audio.h>
#include <psyq/pad.h>

DATA(0x8006d690, 0x4)
s32 input_idle_counter = 0;

ADDRESS(0x80022300, 0x94)
void func_80022300(s32 cue)
{
    if (cue == 16) {
        audio_key_on(16, 48, 48, 0);
        SsSeqCalledTbyT();
    } else if ((u32)cue - 17u < 2u) {
        audio_key_on(cue, 48, 48, 0);
        SsSeqCalledTbyT();
    } else if (cue == 13) {
        audio_key_on(13, 0, 58, 0);
        VSync(0);
        VSync(0);
        audio_key_on(13, 58, 0, 0);
    }
}

ADDRESS(0x80022394, 0x38)
u32 input_read_mark_active(void)
{
    u32 buttons = PadRead(1);
    if (buttons != 0) {
        input_idle_counter = 1;
    }
    return buttons;
}

ADDRESS(0x800223cc, 0x6c)
void func_800223cc(void)
{
    s32 polls;

    if (input_idle_counter == 1) {
        input_idle_counter = 0;
        for (polls = 0; PadRead(1) != 0;) {
            if (polls++ < 6) {
                VSync(0);
            } else {
                menu_cursor_animation_frame = 0;
                break;
            }
        }
    }
}
