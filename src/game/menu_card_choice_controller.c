#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/menu.h>
#include <psyq/audio.h>
#include <psyq/cd.h>

ADDRESS(0x8001aa9c, 0x1e4)
s32 func_8001aa9c(void)
{
    KfMenuGlyphString labels[2];
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;
    s32 volume;

    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            result = func_8001ac80();
            if (result == 0)
                result = -3;
            break;
        case 1:
            result = func_8001b14c();
            if (result == 0)
                result = -2;
            break;
        }

        if (selection != -1 && result == -1)
            result = -99;

        if (result != -99)
            break;

        cursor = menu_poll_choice_input(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(1, 3, cursor, confirmed);
            menu_present_frame();
        }
    }

    if (result == -2) {
        u32 cd_result;

        CdControl(CdlStop, 0, (u8 *)&cd_result);
        volume = 60;
        func_8001ccd4(labels);
        for (;;) {
            func_8001b030(5, labels, 2, 70, 87, 178, 66, 2, 0);
            if (volume > 0) {
                volume--;
                SsSeqSetVol(audio_state.sequence_id, volume, volume);
            }
        }
    }
    return result;
}
