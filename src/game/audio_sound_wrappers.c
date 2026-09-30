#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/player.h>
#include <kf/lib/math.h>
#include <psyq/sdk.h>

extern s32 func_8002b604(s32 x, s32 y, s32 z, s32 radius, s32 height);

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
s32 func_80045e5c(const VECTOR *position, const struct KfEulerAngles *angles)
{
    s32 x = position->vx - ((rsin(angles->y) * 800) >> 12);
    s32 z = position->vz + ((rcos(angles->y) * 800) >> 12);
    u8 *shape;

    func_8002b604(x, position->vy, z, 800, 1700);
    /* This pointer is inside the startup-cleared BSS; its target is WIP. */
    shape = *(u8 **)((u8 *)&bss_801c7540 + 0x11804);
    return *shape == 0x20;
}
