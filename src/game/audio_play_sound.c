#include <kf/lib/address.h>
#include <kf/game/audio.h>

ADDRESS(0x800140dc, 0x24)
void audio_play_sound(s32 sound, s32 volume)
{
    audio_key_on(sound, volume, volume, 0);
}
