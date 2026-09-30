#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/audio.h>
#include <kf/game/player.h>

enum {
    COLLISION_CACHE_DEPTH_OFFSET = 0x11810,
    COLLISION_DEPTH_ARM_HEIGHT = 200,
    COLLISION_DEPTH_DEATH_LIMIT = 32000
};

ADDRESS(0x80027928, 0x60)
void func_80027928(void)
{
    /* Cache ownership at this offset remains provisional. */
    if (player_state.unknown_13a >= COLLISION_DEPTH_ARM_HEIGHT
        && (*(const s32 *)((const u8 *)&bss_801c7540
                           + COLLISION_CACHE_DEPTH_OFFSET)
            - player_state.camera_position.vy) > COLLISION_DEPTH_DEATH_LIMIT) {
        player_death_begin(NULL);
        player_state.unknown_d1[4] = 1;
    }
}

ADDRESS(0x80027988, 0x44)
void func_80027988(s32 magnitude)
{
    s32 volume = magnitude;

    if (volume >= 320) {
        volume -= 320;
        if (volume >= 897) {
            volume = 896;
        }
        audio_play_sound(12, (volume >> 3) + 32);
    }
}
