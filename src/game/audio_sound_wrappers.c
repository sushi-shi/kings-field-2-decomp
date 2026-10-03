#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>
#include <kf/lib/math.h>
#include <psyq/sdk.h>

ADDRESS(0x80045e18, 0x24)
void audio_play_sound_64(void)
{
    audio_play_sound(0x40, 100);
}

ADDRESS(0x80045e3c, 0x20)
void audio_play_sound_at_volume_100(s32 sound)
{
    audio_play_sound(sound, 100);
}

ADDRESS(0x80045e5c, 0xb4)
s32 collision_probe_forward_shape_0x20(const VECTOR *position, const struct KfEulerAngles *angles)
{
    s32 x = position->vx - ((rsin(angles->y) * 800) >> 12);
    s32 z = position->vz + ((rcos(angles->y) * 800) >> 12);
    u8 *shape;

    collision_probe_floor_height(x, position->vy, z, 800, 1700);
    shape = KF_COLLISION_CACHE_SHAPE;
    return *shape == 0x20;
}
