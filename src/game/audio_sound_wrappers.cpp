#include <kf/game/audio.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>
#include <kf/lib/math.h>
#include <psyq/sdk.h>

enum { AUDIO_WRAPPER_VOLUME = 100 };

void audio_play_sound_64(void)
{
    audio_play_sound(0x40, AUDIO_WRAPPER_VOLUME);
}

void audio_play_sound_at_volume_100(s32 sound)
{
    audio_play_sound(sound, AUDIO_WRAPPER_VOLUME);
}

b32 collision_probe_forward_shape_0x20(const VECTOR *position, const struct KfEulerAngles *angles)
{
    s32 x = position->vx - ((rsin(angles->y) * KF_PLAYER_COLLISION_RADIUS) >> KF_FIXED12_BITS);
    s32 z = position->vz + ((rcos(angles->y) * KF_PLAYER_COLLISION_RADIUS) >> KF_FIXED12_BITS);
    KfMapOccupancyLayer *selected_layer;

    collision_probe_floor_height(x, position->vy, z,
        KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT);
    selected_layer = KF_COLLISION_CACHE_SHAPE;
    return selected_layer->object_index == 0x20;
}
