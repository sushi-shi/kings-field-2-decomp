#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/cd.h>
#include <kf/game/game.h>
#include <psyq/pad.h>
#include <psyq/sdk.h>

enum {
    KF_GPU_RESET_KEEP_DISPLAY = 3
};

ADDRESS(0x8001398c, 0x38)
void game_shutdown(void)
{
    audio_shutdown();
    cd_close_events();
    PadStop();
    ResetGraph(KF_GPU_RESET_KEEP_DISPLAY);
}
