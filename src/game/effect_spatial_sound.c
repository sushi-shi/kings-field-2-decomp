#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/effect.h>

enum {
    EFFECT_SPATIAL_VOLUME = 110,
    EFFECT_SPATIAL_MAX_DISTANCE = 0x6d60,
    EFFECT_SPATIAL_ATTENUATION_DISTANCE = 0x7148
};

ADDRESS(0x8003fa2c, 0x3c)
KfAudioPlaybackResult effect_play_spatial_sound(
    KfEffectRecord *effect, s32 sound)
{
    return audio_play_spatial_range(sound, &effect->position,
        EFFECT_SPATIAL_VOLUME, EFFECT_SPATIAL_MAX_DISTANCE,
        EFFECT_SPATIAL_ATTENUATION_DISTANCE, 0);
}
