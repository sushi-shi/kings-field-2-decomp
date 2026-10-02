#include <kf/lib/address.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>


ADDRESS(0x80036e24, 0xb0)
void func_80036e24(s32 mode, s32 phase, s32 last_phase, s32 step)
{
    VECTOR position;
    SVECTOR angles;

    for (;;) {
        s32 brightness = (phase * phase) >> 16;

        if (brightness >= 256) {
            brightness = 255;
        }
        func_800314d4(mode, brightness, brightness, brightness);
        cd_request_service_vab();
        cd_request_service_stream();
        player_get_camera_pose(&position, &angles);
        func_800335a0(&position, &angles);
        if (phase == last_phase) {
            break;
        }
        phase += step;
    }
}
