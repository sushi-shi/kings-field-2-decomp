#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

RODATA(0x80011128, 0x3c)

enum {
    KF_PLAYER_DAMAGE_DEATH_STATE = 0x11,
    KF_PLAYER_DAMAGE_ORIGIN_HEIGHT = 850,
    KF_PLAYER_DAMAGE_MOTION_MIN = 80,
    KF_PLAYER_DAMAGE_MOTION_MAX = 300,
    KF_PLAYER_DAMAGE_STRONG_MAX = 600,
    KF_PLAYER_DAMAGE_VERTICAL_LIMIT = 200,
    KF_PLAYER_DAMAGE_DURATION_BASE = 1300,
    KF_PLAYER_DAMAGE_DURATION_MIN = 70
};

ADDRESS(0x80024498, 0x34c)
void player_apply_damage_reaction(const VECTOR *origin, s32 damage, s32 reaction_flags)
{
    struct KfEulerAngles angles;
    SVECTOR direction;
    s32 remaining;
    s32 intensity;
    s32 magnitude;
    s16 duration;
    s32 direction_index;
    s32 origin_height;

    if (player_state.death_state == KF_PLAYER_DAMAGE_DEATH_STATE || damage == 0) {
        return;
    }
    remaining = player_state.vitals.current_hp - damage;
    if (remaining <= 0) {
        remaining = 0;
    }
    intensity = (damage << 11) / player_state.vitals.maximum_hp;
    player_state.vitals.current_hp = remaining;

    if (origin != NULL) {
        magnitude = intensity >> 2;
        origin_height = origin->vy + KF_PLAYER_DAMAGE_ORIGIN_HEIGHT;
        func_800154fc(
            player_state.camera_position.vx - origin->vx,
            player_state.camera_position.vy - origin_height,
            player_state.camera_position.vz - origin->vz, &angles);
        if (magnitude < KF_PLAYER_DAMAGE_MOTION_MIN) {
            magnitude = KF_PLAYER_DAMAGE_MOTION_MIN;
        }
        if (reaction_flags & 0x40) {
            magnitude *= 5;
            if (magnitude > KF_PLAYER_DAMAGE_STRONG_MAX) {
                magnitude = KF_PLAYER_DAMAGE_STRONG_MAX;
            }
        } else if (magnitude > KF_PLAYER_DAMAGE_MOTION_MAX) {
            magnitude = KF_PLAYER_DAMAGE_MOTION_MAX;
        }
        pitch_yaw_to_forward_vector(&angles, &direction);
        vector3s_scale_shift12(magnitude, &direction);
        if (direction.vy > KF_PLAYER_DAMAGE_VERTICAL_LIMIT) {
            direction.vy = KF_PLAYER_DAMAGE_VERTICAL_LIMIT;
        } else if (direction.vy < -KF_PLAYER_DAMAGE_VERTICAL_LIMIT) {
            direction.vy = -KF_PLAYER_DAMAGE_VERTICAL_LIMIT;
        }
    } else {
        direction.vz = 0;
        direction.vy = 0;
        direction.vx = 0;
    }

    duration = (KF_PLAYER_DAMAGE_DURATION_BASE - (intensity >> 1)) >> 2;
    if ((s16)duration < KF_PLAYER_DAMAGE_DURATION_MIN + 1) {
        duration = KF_PLAYER_DAMAGE_DURATION_MIN;
    }
    if (remaining == 0) {
        player_death_begin(&direction);
        player_state.unknown_13c = 3500;
        player_state.unknown_13e = duration;
        return;
    }
    if (origin == NULL) {
        player_state.unknown_13c = 3500;
        player_state.unknown_13e = duration;
        return;
    }

    intensity = SquareRoot0(intensity << 2);
    if (intensity < 16) {
        intensity = 16;
    } else if (intensity > 42) {
        intensity = 42;
    }
    if (reaction_flags & 0x40) {
        intensity <<= 1;
    }
    if (reaction_flags & 0x80) {
        angles.x = 0;
        if (rand() >= 16385) {
            angles.z = intensity;
        } else {
            angles.z = -intensity;
        }
        angles.y = 0;
        func_800294f8((const SVECTOR *)&direction, (const SVECTOR *)&angles, (s16)duration);
        return;
    }

    direction_index = ((angles.y - player_state.camera_rotation.angles[1] + 256) >> 9) & 7;
    switch (direction_index) {
    case 0:
        angles.x = -intensity;
        angles.z = 0;
        break;
    case 1:
        angles.x = -intensity;
        angles.z = intensity;
        break;
    case 2:
        angles.x = 0;
        angles.z = intensity;
        break;
    case 3:
        angles.x = intensity;
        angles.z = intensity;
        break;
    case 4:
        angles.x = intensity;
        angles.z = 0;
        break;
    case 5:
        angles.x = intensity;
        angles.z = -intensity;
        break;
    case 6:
        angles.x = 0;
        angles.z = -intensity;
        break;
    case 7:
        angles.x = -intensity;
        angles.z = -intensity;
        break;
    default:
        break;
    }
    angles.y = 0;
    func_80029464((const SVECTOR *)&direction, (const SVECTOR *)&angles, (s16)duration);
}

enum {
    KF_PLAYER_STATUS_CAP = 64,
    KF_PLAYER_STATUS_FIRST = 1,
    KF_PLAYER_STATUS_SECOND = 2,
    KF_PLAYER_STATUS_THIRD = 4
};

ADDRESS(0x800247e4, 0xc4)
void player_cap_status_components(u32 mask)
{
    if (mask & KF_PLAYER_STATUS_FIRST) {
        if (player_state.curse_strength >= KF_PLAYER_STATUS_CAP + 1) {
            player_state.curse_strength = KF_PLAYER_STATUS_CAP;
        }
        player_state.unknown_58 = 0;
    }
    if (mask & KF_PLAYER_STATUS_SECOND) {
        if (player_state.unknown_5a >= KF_PLAYER_STATUS_CAP + 1) {
            player_state.unknown_5a = KF_PLAYER_STATUS_CAP;
        }
        player_state.unknown_5c = 0;
    }
    if (mask & (KF_PLAYER_STATUS_FIRST | KF_PLAYER_STATUS_SECOND)) {
        player_state.unknown_54 = 0;
    }
    if (mask & (KF_PLAYER_STATUS_FIRST | KF_PLAYER_STATUS_THIRD)) {
        if (player_state.unknown_5e >= KF_PLAYER_STATUS_CAP + 1) {
            player_state.unknown_5e = KF_PLAYER_STATUS_CAP;
        }
    }
    if (mask & KF_PLAYER_STATUS_THIRD) {
        player_state.unknown_60 = 0;
    }
}

enum {
    KF_PLAYER_STATUS_GUARD_CHANCE = 16384,
    KF_PLAYER_POISON_ROLL_SCALE = 100,
    KF_PLAYER_POISON_ROLL_SHIFT = 15
};

ADDRESS(0x800248a8, 0x3fc)
void player_apply_damage(u16 damage0, u16 damage1, u16 damage2, u16 status_flags,
                   u16 damage3, u16 damage4, u16 damage5, u16 damage6,
                   u16 damage7, u16 scale_q16, u16 multiplier_tenths,
                   const VECTOR *origin)
{
    s32 total;
    s32 mp_loss;
    s32 damage_loss;
    s32 timer_58;
    s32 timer_5c;
    s32 timer_60;
    s32 timer_5e;
    s32 timer_54;
    u16 flags = status_flags;

    if (player_state.unknown_a0 != 0) {
        return;
    }
    if (player_state.equipped_accessory_id == 0x36
        || player_state.equipped_extra_id == 0x36) {
        if (rand() < KF_PLAYER_STATUS_GUARD_CHANCE) {
            flags &= 0xfff8;
        }
    }
    if (player_state.equipped_accessory_id == 0x3a
        || player_state.equipped_extra_id == 0x3a) {
        timer_58 = 300;
        timer_5c = 250;
        timer_54 = 300;
        timer_60 = 100;
        timer_5e = 300;
    } else {
        timer_58 = 600;
        timer_5c = 500;
        timer_54 = 600;
        timer_60 = 200;
        timer_5e = 600;
    }

    switch ((flags & 0xf) - 1) {
    case 0:
        player_state.unknown_58 = timer_58;
        player_state.curse_strength = 1;
        player_recalculate_combat_stats();
        break;
    case 1:
        player_state.unknown_5c = timer_5c;
        break;
    case 2:
        if (player_state.equipped_accessory_id == 0x35
            || player_state.equipped_extra_id == 0x35) {
            if (rand() < KF_PLAYER_STATUS_GUARD_CHANCE) {
                break;
            }
        }
        if (player_state.combat_components[3]
            < ((rand() * KF_PLAYER_POISON_ROLL_SCALE) >> KF_PLAYER_POISON_ROLL_SHIFT)) {
            player_state.unknown_54 = timer_54;
        }
        break;
    case 3:
        player_state.unknown_60 = timer_60;
        break;
    case 4:
        player_state.unknown_5e = timer_5e;
        break;
    case 6:
        player_state.unknown_54 = 0;
        break;
    case 5:
        mp_loss = player_state.vitals.maximum_mp / 6;
        if (mp_loss < player_state.vitals.current_mp) {
            player_state.vitals.current_mp -= mp_loss;
        } else {
            player_state.vitals.current_mp = 0;
        }
        break;
    }

    total = player_calculate_damage_component(
        player_state.physical_power, player_state.combat_components[0], damage0);
    total += player_calculate_damage_component(
        player_state.physical_power, player_state.combat_components[1], damage1);
    total += player_calculate_damage_component(
        player_state.physical_power, player_state.combat_components[2], damage2);
    total += player_calculate_damage_component(
        player_state.physical_power, player_state.combat_components[4], damage3);
    total += player_calculate_damage_component(
        player_state.physical_power, player_state.combat_components[5], damage4);
    total += player_calculate_damage_component(
        player_state.physical_power, player_state.combat_components[6], damage5);
    total += player_calculate_damage_component(
        player_state.physical_power, player_state.combat_components[7], damage6);
    total += player_calculate_damage_component(
        player_state.physical_power, player_state.combat_components[8], damage7);
    total = (scale_q16 * total + 0x8000) >> 16;
    damage_loss = multiplier_tenths * total / 10;
    player_apply_damage_reaction(origin, damage_loss, flags);
}

enum {
    KF_RADIAL_MODE_PLAYER = 0x8000,
    KF_RADIAL_MODE_PLAYER_ABOVE = 0x8001,
    KF_RADIAL_REACH_OFFSET = 800,
    KF_RADIAL_OUTSIDE_REACH = -9999999
};

ADDRESS(0x80024ca4, 0x230)
void player_apply_radial_damage(VECTOR *position, s32 start, s32 end, s32 mode,
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
        distance = vector_distance_between_with_reach(position, end, &player_state.camera_position,
                                  KF_RADIAL_REACH_OFFSET, KF_PLAYER_HEIGHT);
    } else if (mode == KF_RADIAL_MODE_PLAYER_ABOVE) {
        if (position->vy < player_state.camera_position.vy - KF_PLAYER_HEIGHT) {
            distance = KF_RADIAL_OUTSIDE_REACH;
        } else {
            distance = vector_distance_between_with_reach(position, end,
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

    player_apply_damage(damage0, damage1, damage2, damage3,
                  damage4, damage5, damage6, damage7,
                  damage8, attenuation, record_id, reaction_origin);
}
