#include <kf/lib/address.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>

enum {
    FRAME_COLOR_LEVELS = 256,
    FRAME_COLOR_MAX = FRAME_COLOR_LEVELS - 1
};

ADDRESS(0x80036e24, 0xb0)
void render_frames_with_color_overlay(s32 mode, s32 phase, s32 last_phase,
    s32 step)
{
    VECTOR position;
    SVECTOR angles;

    for (;;) {
        s32 brightness = (phase * phase) >> 16;

        if (brightness >= FRAME_COLOR_LEVELS) {
            brightness = FRAME_COLOR_MAX;
        }
        render_set_color_overlay(mode, brightness, brightness, brightness);
        cd_request_service_vab();
        cd_request_service_stream();
        player_get_camera_pose(&position, &angles);
        render_game_frame(&position, &angles);
        if (phase == last_phase) {
            break;
        }
        phase += step;
    }
}
