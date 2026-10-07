#include <kf/end/ending.h>
#include <kf/lib/audio.h>
#include <kf/lib/cd_file.h>
#include <kf/lib/display.h>
#include <kf/lib/overlay.h>
#include <psyq/cd.h>
#include <psyq/kernel.h>
#include <psyq/libc.h>
#include <psyq/pad.h>

#define ENDING_HEAP_BASE 0
#define ENDING_HEAP_BYTES 0xf8000

enum {
    ENDING_DATA_BYTES = 0x7d000,
    ENDING_LOAD_ATTEMPTS = 10
};

u8 *overlay_next_request = &kf_psx_overlay_request;

char ending_data_file[5] = "ED.D";

u8 *ending_data;

extern "C" void main(void)
{

    RECT rect;

    ResetCallback();
    InitHeap((void *)ENDING_HEAP_BASE, ENDING_HEAP_BYTES);
    CdInit();
    PadInit(0);
    ExitCriticalSection();
    audio_initialize();
    ending_load_data();
    display_initialize();
    display_current = &display_buffers[0];
    PutDrawEnv(&display_current->draw);
    PutDispEnv(&display_current->disp);
    display_current = &display_buffers[1];
    PutDrawEnv(&display_current->draw);
    PutDispEnv(&display_current->disp);
    ending_open_audio();
    if (*overlay_next_request == KF_OVERLAY_END) {
        ending_play_movie();
    }
}

void ending_load_data(void)
{
    s32 attempt;

    ending_data = (u8 *)malloc(ENDING_DATA_BYTES);
    for (attempt = ENDING_LOAD_ATTEMPTS - 1; attempt != -1; attempt--) {
        if (cd_file_load_into((u_long *)ending_data, ending_data_file) == KF_CD_LOADED) {
            break;
        }
    }
}
