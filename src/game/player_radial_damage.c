#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/player.h>

enum {
    KF_RADIAL_MODE_PLAYER = 0x8000,
    KF_RADIAL_MODE_PLAYER_ABOVE = 0x8001,
    KF_RADIAL_REACH_OFFSET = 800,
    KF_RADIAL_OUTSIDE_REACH = -9999999
};

ADDRESS(0x80024ca4, 0x230)
void func_80024ca4(VECTOR *position, s32 start, s32 end, s32 mode,
                   u16 falloff, u16 damage0, u16 damage1, u16 damage2,
                   u16 damage3, u16 damage4, u16 damage5, u16 damage6,
                   u16 damage7, u16 damage8, s32 scale_and_flags, u16 record_id)
{
    const VECTOR *reaction_origin = position;
    s32 distance;
    u16 attenuation;
    u16 base_scale;

    if (scale_and_flags & 0x8000) {
        reaction_origin = 0;
    }
    base_scale = scale_and_flags & 0x7fff;

    if (mode == KF_RADIAL_MODE_PLAYER) {
        distance = func_80015698(position, end, &player_state.camera_position,
                                  KF_RADIAL_REACH_OFFSET, KF_PLAYER_HEIGHT);
    } else if (mode == KF_RADIAL_MODE_PLAYER_ABOVE) {
        if (position->vy < player_state.camera_position.vy - KF_PLAYER_HEIGHT) {
            distance = KF_RADIAL_OUTSIDE_REACH;
        } else {
            distance = func_80015698(position, end,
                                      &player_state.camera_position,
                                      KF_RADIAL_REACH_OFFSET, KF_PLAYER_HEIGHT);
        }
    } else {
        distance = player_distance_to_point(position->vx, position->vy,
                                            position->vz, end, mode);
        if (distance == KF_DISTANCE_NONE) {
            distance = KF_RADIAL_OUTSIDE_REACH;
        }
    }

    if (distance < start) {
        return;
    }

    if (falloff != KF_FIXED12_ONE) {
        u16 ratio = (distance << KF_FIXED12_BITS) / end;
        u16 factor = KF_FIXED12_ONE
                   - ((u32)(ratio * (KF_FIXED12_ONE - falloff)) >> KF_FIXED12_BITS);
        attenuation = (base_scale * factor) >> KF_FIXED12_BITS;
    } else {
        attenuation = base_scale;
    }

    func_800248a8(damage0, damage1, damage2, damage3,
                  damage4, damage5, damage6, damage7,
                  damage8, attenuation, record_id, reaction_origin);
}
