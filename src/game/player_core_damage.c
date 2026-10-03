#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/graphics.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>
#include <kf/game/resources.h>
#include <kf/lib/math.h>
#include <psyq/sdk.h>


DATA(0x800758f0, 0x4b0)
KfPlayerLevelGrowth player_level_growth_table[KF_PLAYER_LEVEL_GROWTH_COUNT];

DATA(0x801984d0, 0x160)
KfPlayerState player_state;

ADDRESS(0x80023570, 0x9c)
void player_restore_equipment_effects(void)
{
    player_set_equipment_slot(KF_EQUIPMENT_NONE, KF_EQUIPMENT_NONE);
    player_equip_weapon(player_state.equipped_weapon_id);
    player_reset_view();
    if (player_state.defense_boost_timer != 0) {
        effect_construct_record(10, KF_EFFECT_USE_PLAYER_MAGIC, KF_EFFECT_KIND_DEFENSE_BOOST,
                                &player_state.camera_position, NULL);
    }
    if (player_state.attack_boost_timer != 0) {
        effect_construct_record(10, KF_EFFECT_USE_PLAYER_MAGIC, KF_EFFECT_KIND_ATTACK_BOOST,
                                &player_state.camera_position, NULL);
    }
    player_recalculate_combat_stats();
}

ADDRESS(0x8002360c, 0x208)
void player_reload_map_resources(
    s32 first, s32 second, s32 third, s32 fourth, s32 fifth, s32 optional_resource)
{
    cd_request_wait_idle();
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    resource_request_transition(first, second, third,
                                KF_RESOURCE_REQUEST_KEEP, KF_RESOURCE_REQUEST_KEEP,
                                KF_RESOURCE_OFFSET_NO_SHIFT, KF_RESOURCE_OFFSET_NO_SHIFT,
                                KF_RESOURCE_OFFSET_NO_SHIFT);
    do {
        cd_request_yield();
        resource_advance_transition();
    } while (state_8017d118.transition_active != 0);
    cd_request_wait_idle();
    DrawSync(0);
    VSync(0);
    DrawSync(0);
    VSync(0);
    if (optional_resource != KF_RESOURCE_REQUEST_KEEP) {
        resource_request_transition(KF_RESOURCE_REQUEST_KEEP, KF_RESOURCE_REQUEST_KEEP,
                                    optional_resource, KF_RESOURCE_REQUEST_KEEP,
                                    KF_RESOURCE_REQUEST_KEEP, KF_RESOURCE_OFFSET_NO_SHIFT,
                                    KF_RESOURCE_OFFSET_NO_SHIFT, KF_RESOURCE_OFFSET_NO_SHIFT);
        do {
            cd_request_yield();
            resource_advance_transition();
        } while (state_8017d118.transition_active != 0);
        cd_request_wait_idle();
        DrawSync(0);
        VSync(0);
        DrawSync(0);
        VSync(0);
    }

    player_sync_position_to_map();
    reset_collision_rows_and_overlay();
    render_frames_with_color_overlay(0x82, 0x1000, 0x1000, 0);
    if (game_graphics_runtime.asset_registry_entries[0x181] == 0) {
        resource_tmd_queue_read(0, 0x101, 0x181);
    }
    cd_request_wait_idle();
    render_frames_with_color_overlay(0x82, 0x1000, 0, -128);
    resource_request_transition(KF_RESOURCE_REQUEST_KEEP, KF_RESOURCE_REQUEST_KEEP,
                                KF_RESOURCE_REQUEST_KEEP, fourth, fifth,
                                KF_RESOURCE_OFFSET_NO_SHIFT, KF_RESOURCE_OFFSET_NO_SHIFT,
                                KF_RESOURCE_OFFSET_NO_SHIFT);
}

enum {
    KF_PLAYER_VALUE_SCALE_BITS = 6
};

ADDRESS(0x80023814, 0x54)
s32 player_charge_gain_for_rank(s32 value, s32 rank)
{
    s32 scaled = ((value << KF_PLAYER_VALUE_SCALE_BITS) / (rank + 1)) + 1;
    if (scaled >= KF_PLAYER_CHARGE_FULL) {
        return KF_PLAYER_CHARGE_FULL;
    }
    return scaled;
}

ADDRESS(0x80023868, 0x11c)
void player_add_equipment_bonuses(s32 item_id)
{
    const KfEquipmentRecord *record;

    if (item_id == KF_EQUIPMENT_NONE) {
        return;
    }
    record = &bss_801c7540.equipment_records[item_id];
    player_state.combat_components[0] += record->bonus_components[0];
    player_state.combat_components[1] += record->bonus_components[1];
    player_state.combat_components[2] += record->bonus_components[2];
    player_state.combat_components[3] += record->bonus_components[3];
    player_state.combat_components[4] += record->bonus_components[4];
    player_state.combat_components[5] += record->bonus_components[5];
    player_state.combat_components[6] += record->bonus_components[6];
    player_state.combat_components[7] += record->bonus_components[7];
    player_state.combat_components[8] += record->bonus_components[8];
}

enum {
    PLAYER_CURSE_POWER_PENALTY = 20,
    PLAYER_STATUS_DEFENSE_BONUS = 50,
    PLAYER_STATUS_POWER_BONUS = 30,
    PLAYER_WEAPON_ATTACK_PENALTY = 10,
    PLAYER_ACCESSORY_ATTACK_BONUS = 0x37,
    PLAYER_ACCESSORY_MAGIC_BONUS = 0x38,
    PLAYER_ACCESSORY_PHYSICAL_POWER_BONUS = 0x39,
    PLAYER_BONUS_OVERFLOW_LIMIT = 0x7fff,
    PLAYER_POWER_CAP_THRESHOLD = KF_PLAYER_POWER_MAX + 1
};

ADDRESS(0x80023984, 0x6b0)
void player_recalculate_combat_stats(void)
{
    const KfWeaponRecordGame *weapon;
    s32 power;

    player_state.attack_components[0] = 0;
    player_state.attack_components[1] = 0;
    player_state.attack_components[2] = 0;
    player_state.attack_components[3] = 0;
    player_state.attack_components[4] = 0;
    player_state.attack_components[5] = 0;
    player_state.attack_components[6] = 0;
    player_state.attack_components[7] = 0;
    player_state.combat_components[0] = 0;
    player_state.combat_components[1] = 0;
    player_state.combat_components[2] = 0;
    player_state.combat_components[3] = 0;
    player_state.combat_components[4] = 0;
    player_state.combat_components[5] = 0;
    player_state.combat_components[6] = 0;
    player_state.combat_components[7] = 0;
    player_state.combat_components[8] = 0;

    player_state.physical_power = player_state.base_physical_power;
    player_state.magic = player_state.base_magic;
    if (player_state.curse_strength != 0) {
        power = player_state.physical_power - PLAYER_CURSE_POWER_PENALTY;
        if (power < 0) {
            power = 0;
        }
        player_state.physical_power = power;
    }

    if (player_state.equipped_weapon_id != KF_EQUIPMENT_NONE) {
        weapon = &player_weapon_records[player_state.equipped_weapon_id];
        player_state.attack_components[0] += weapon->attack_components[0];
        player_state.attack_components[1] += weapon->attack_components[1];
        player_state.attack_components[2] += weapon->attack_components[2];
        player_state.attack_components[3] += weapon->attack_components[3];
        player_state.attack_components[4] += weapon->attack_components[4];
        player_state.attack_components[5] += weapon->attack_components[5];
        player_state.attack_components[6] += weapon->attack_components[6];
        player_state.attack_components[7] += weapon->attack_components[7];
    }

    player_add_equipment_bonuses(player_state.equipped_head_id);
    player_add_equipment_bonuses(player_state.equipped_body_id);
    player_add_equipment_bonuses(player_state.equipped_arm_id);
    player_add_equipment_bonuses(player_state.equipped_leg_id);
    player_add_equipment_bonuses(player_state.equipped_shield_id);
    player_add_equipment_bonuses(player_state.equipped_accessory_id);
    player_add_equipment_bonuses(player_state.equipped_extra_id);

    if (player_state.defense_boost_timer != 0) {
        player_state.combat_components[5] += PLAYER_STATUS_DEFENSE_BONUS;
    }
    if (player_state.attack_boost_timer != 0) {
        player_state.combat_components[1] += PLAYER_STATUS_POWER_BONUS;
        player_state.combat_components[0] += PLAYER_STATUS_POWER_BONUS;
        player_state.combat_components[2] += PLAYER_STATUS_POWER_BONUS;
    }
    if (player_state.magic_boost_timer != 0) {
        player_state.magic += PLAYER_STATUS_POWER_BONUS;
    }

    if (player_state.equipped_weapon_id == 10) {
        player_state.combat_components[1] -= PLAYER_WEAPON_ATTACK_PENALTY;
        player_state.combat_components[2] -= PLAYER_WEAPON_ATTACK_PENALTY;
    }
    if (player_state.equipped_weapon_id == 11) {
        player_state.combat_components[4] -= PLAYER_WEAPON_ATTACK_PENALTY;
        player_state.combat_components[5] -= PLAYER_WEAPON_ATTACK_PENALTY;
        player_state.combat_components[7] -= PLAYER_WEAPON_ATTACK_PENALTY;
        player_state.combat_components[8] -= PLAYER_WEAPON_ATTACK_PENALTY;
    }
    if (player_state.equipped_accessory_id == PLAYER_ACCESSORY_ATTACK_BONUS
        || player_state.equipped_extra_id == PLAYER_ACCESSORY_ATTACK_BONUS) {
        player_state.attack_components[1] += 5;
        player_state.attack_components[2] += 5;
        player_state.attack_components[3] += 22;
        player_state.attack_components[7] += 12;
    }
    if (player_state.equipped_accessory_id == PLAYER_ACCESSORY_MAGIC_BONUS
        || player_state.equipped_extra_id == PLAYER_ACCESSORY_MAGIC_BONUS) {
        player_state.magic += 8;
    }
    if (player_state.equipped_accessory_id == PLAYER_ACCESSORY_PHYSICAL_POWER_BONUS
        || player_state.equipped_extra_id == PLAYER_ACCESSORY_PHYSICAL_POWER_BONUS) {
        player_state.physical_power += 8;
    }

    if (player_state.physical_power > PLAYER_BONUS_OVERFLOW_LIMIT) {
        player_state.physical_power = 0;
    } else if (player_state.physical_power >= PLAYER_POWER_CAP_THRESHOLD) {
        player_state.physical_power = KF_PLAYER_POWER_MAX;
    }
    if (player_state.magic > PLAYER_BONUS_OVERFLOW_LIMIT) {
        player_state.magic = 0;
    } else if (player_state.magic >= PLAYER_POWER_CAP_THRESHOLD) {
        player_state.magic = KF_PLAYER_POWER_MAX;
    }
    if (player_state.attack_components[0] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.attack_components[0] = 0;
    if (player_state.attack_components[1] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.attack_components[1] = 0;
    if (player_state.attack_components[2] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.attack_components[2] = 0;
    if (player_state.attack_components[3] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.attack_components[3] = 0;
    if (player_state.attack_components[4] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.attack_components[4] = 0;
    if (player_state.attack_components[5] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.attack_components[5] = 0;
    if (player_state.attack_components[6] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.attack_components[6] = 0;
    if (player_state.attack_components[7] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.attack_components[7] = 0;
    if (player_state.combat_components[0] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[0] = 0;
    if (player_state.combat_components[1] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[1] = 0;
    if (player_state.combat_components[2] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[2] = 0;
    if (player_state.combat_components[3] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[3] = 0;
    if (player_state.combat_components[4] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[4] = 0;
    if (player_state.combat_components[5] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[5] = 0;
    if (player_state.combat_components[6] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[6] = 0;
    if (player_state.combat_components[7] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[7] = 0;
    if (player_state.combat_components[8] > PLAYER_BONUS_OVERFLOW_LIMIT) player_state.combat_components[8] = 0;
}

ADDRESS(0x80024034, 0x98)
void player_increment_physical_power_training(void)
{
    player_state.physical_power_training++;
    if (player_state.physical_power_training >= KF_PLAYER_TRAINING_POINTS_PER_GAIN) {
        player_state.base_physical_power++;
        player_state.physical_power_training = 0;
        if (player_state.base_physical_power >= KF_PLAYER_POWER_MAX + 1) {
            player_state.base_physical_power = KF_PLAYER_POWER_MAX;
        } else {
            notify_enqueue(KF_NOTIFICATION_PHYSICAL_POWER_INCREASED);
        }
        player_recalculate_combat_stats();
    }
}

ADDRESS(0x800240cc, 0x98)
void player_increment_magic_training(void)
{
    player_state.magic_training++;
    if (player_state.magic_training >= KF_PLAYER_TRAINING_POINTS_PER_GAIN) {
        player_state.base_magic++;
        player_state.magic_training = 0;
        if (player_state.base_magic >= KF_PLAYER_POWER_MAX + 1) {
            player_state.base_magic = KF_PLAYER_POWER_MAX;
        } else {
            notify_enqueue(KF_NOTIFICATION_MAGIC_POWER_INCREASED);
        }
        player_recalculate_combat_stats();
    }
}

ADDRESS(0x80024164, 0x220)
void player_add_experience(s16 amount)
{
    const KfPlayerLevelGrowth *growth;
    u8 level;

    player_state.experience += amount;
    if (player_state.experience > KF_PLAYER_EXPERIENCE_MAX) {
        player_state.experience = KF_PLAYER_EXPERIENCE_MAX;
    }
    while (player_state.experience >= player_state.next_level_experience) {
        level = player_state.level;
        if (player_state.level >= KF_PLAYER_LEVEL_MAX) {
            break;
        }
        player_state.level = level + 1;
        if (level >= KF_PLAYER_LEVEL_GROWTH_COUNT) {
            growth = &player_level_growth_table[KF_PLAYER_LEVEL_GROWTH_COUNT - 1];
            player_state.vitals.maximum_hp +=
                growth->maximum_hp
                - growth[-1].maximum_hp;
            player_state.vitals.maximum_mp +=
                growth->maximum_mp
                - growth[-1].maximum_mp;
            player_state.base_physical_power += growth->physical_power_step;
            player_state.base_magic += growth->magic_step;
            player_state.next_level_experience +=
                growth->experience_threshold
                - growth[-1].experience_threshold;
        } else {
            growth = &player_level_growth_table[level];
            player_state.vitals.maximum_hp = growth->maximum_hp;
            player_state.vitals.maximum_mp = growth->maximum_mp;
            player_state.base_physical_power += growth->physical_power_step;
            player_state.base_magic += growth->magic_step;
            player_state.next_level_experience = growth->experience_threshold;
        }
        if (player_state.vitals.maximum_hp >= KF_PLAYER_VITAL_MAX + 1) {
            player_state.vitals.maximum_hp = KF_PLAYER_VITAL_MAX;
        }
        if (player_state.vitals.maximum_mp >= KF_PLAYER_VITAL_MAX + 1) {
            player_state.vitals.maximum_mp = KF_PLAYER_VITAL_MAX;
        }
        if (player_state.base_physical_power >= KF_PLAYER_POWER_MAX + 1) {
            player_state.base_physical_power = KF_PLAYER_POWER_MAX;
        }
        if (player_state.base_magic >= KF_PLAYER_POWER_MAX + 1) {
            player_state.base_magic = KF_PLAYER_POWER_MAX;
        }
        player_recalculate_combat_stats();
        notify_enqueue(KF_NOTIFICATION_LEVEL_UP);
    }
}

enum {
    KF_DAMAGE_SUBUNITS_PER_HP = 16,
    KF_DAMAGE_POWER_DIVISOR = 5,
    KF_DAMAGE_THRESHOLD_MULTIPLIER = 2
};

ADDRESS(0x80024384, 0xc4)
s32 player_calculate_damage_component(s32 base_power, s32 defense, s32 attack)
{
    s32 threshold = base_power;
    s32 defense_scaled = defense;
    s32 attack_scaled = attack;
    s32 excess;

    threshold = (threshold * player_state.damage_scale) >> 8;
    attack_scaled <<= 4;
    defense_scaled <<= 4;
    if (attack_scaled == 0) {
        return 0;
    }
    threshold += defense_scaled;
    excess = attack_scaled - threshold;
    if (excess < 0) {
        excess = 0;
    }
    if (threshold == 0) {
        threshold = KF_DAMAGE_SUBUNITS_PER_HP;
    }
    return (excess + (attack_scaled * attack_scaled)
                     / (threshold * KF_DAMAGE_THRESHOLD_MULTIPLIER))
           / KF_DAMAGE_POWER_DIVISOR;
}

ADDRESS(0x80024448, 0x50)
void player_adjust_hp_unclamped(s32 delta)
{
    s32 hp = player_state.vitals.current_hp + delta;

    if (hp <= 0) {
        hp = 0;
        player_death_begin(NULL);
    }
    player_state.vitals.current_hp = hp;
}


RODATA(0x80011128, 0x3c)

enum {
    KF_PLAYER_DAMAGE_ORIGIN_HEIGHT = 850,
    KF_PLAYER_DAMAGE_MOTION_MIN = 80,
    KF_PLAYER_DAMAGE_MOTION_MAX = 300,
    KF_PLAYER_DAMAGE_STRONG_MAX = 600,
    KF_PLAYER_DAMAGE_VERTICAL_LIMIT = 200,
    KF_PLAYER_DAMAGE_DURATION_BASE = 1300,
    KF_PLAYER_DAMAGE_DURATION_MIN = 70,
    KF_PLAYER_DAMAGE_REACTION_STRONG = 0x40,
    KF_PLAYER_DAMAGE_REACTION_ROTATION_ONLY = 0x80
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

    if (player_state.death_state == KF_PLAYER_REACTION_DEATH || damage == 0) {
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
        vector_displacement_to_pitch_yaw(
            player_state.camera_position.vx - origin->vx,
            player_state.camera_position.vy - origin_height,
            player_state.camera_position.vz - origin->vz, &angles);
        if (magnitude < KF_PLAYER_DAMAGE_MOTION_MIN) {
            magnitude = KF_PLAYER_DAMAGE_MOTION_MIN;
        }
        if (reaction_flags & KF_PLAYER_DAMAGE_REACTION_STRONG) {
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
        player_state.damage_red_overlay_scale = 3500;
        player_state.damage_red_overlay_decay = duration;
        return;
    }
    if (origin == NULL) {
        player_state.damage_red_overlay_scale = 3500;
        player_state.damage_red_overlay_decay = duration;
        return;
    }

    intensity = SquareRoot0(intensity << 2);
    if (intensity < 16) {
        intensity = 16;
    } else if (intensity > 42) {
        intensity = 42;
    }
    if (reaction_flags & KF_PLAYER_DAMAGE_REACTION_STRONG) {
        intensity <<= 1;
    }
    if (reaction_flags & KF_PLAYER_DAMAGE_REACTION_ROTATION_ONLY) {
        angles.x = 0;
        if (rand() >= 16385) {
            angles.z = intensity;
        } else {
            angles.z = -intensity;
        }
        angles.y = 0;
        player_begin_rotation_only_damage_reaction((const SVECTOR *)&direction, (const SVECTOR *)&angles, (s16)duration);
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
    player_begin_moving_damage_reaction((const SVECTOR *)&direction, (const SVECTOR *)&angles, (s16)duration);
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
        player_state.curse_phase_limit = 0;
    }
    if (mask & KF_PLAYER_STATUS_SECOND) {
        if (player_state.darkness_phase >= KF_PLAYER_STATUS_CAP + 1) {
            player_state.darkness_phase = KF_PLAYER_STATUS_CAP;
        }
        player_state.darkness_phase_limit = 0;
    }
    if (mask & (KF_PLAYER_STATUS_FIRST | KF_PLAYER_STATUS_SECOND)) {
        player_state.poison_timer = 0;
    }
    if (mask & (KF_PLAYER_STATUS_FIRST | KF_PLAYER_STATUS_THIRD)) {
        if (player_state.slow_timer >= KF_PLAYER_STATUS_CAP + 1) {
            player_state.slow_timer = KF_PLAYER_STATUS_CAP;
        }
    }
    if (mask & KF_PLAYER_STATUS_THIRD) {
        player_state.paralysis_timer = 0;
    }
}

enum {
    KF_PLAYER_POISON_GUARD_ACCESSORY_ID = 0x35,
    KF_PLAYER_STATUS_GUARD_ACCESSORY_ID = 0x36,
    KF_PLAYER_STATUS_DURATION_HALVING_ACCESSORY_ID = 0x3a,
    KF_PLAYER_STATUS_GUARD_CHANCE = 16384,
    KF_PLAYER_POISON_ROLL_SCALE = 100,
    KF_PLAYER_POISON_ROLL_SHIFT = 15
};

/* The encoded low-nibble status kind is one greater than this switch index. */
enum {
    PLAYER_DAMAGE_STATUS_CURSE = 0,
    PLAYER_DAMAGE_STATUS_DARKNESS = 1,
    PLAYER_DAMAGE_STATUS_POISON = 2,
    PLAYER_DAMAGE_STATUS_PARALYSIS = 3,
    PLAYER_DAMAGE_STATUS_SLOW = 4,
    PLAYER_DAMAGE_STATUS_MP_DRAIN = 5,
    PLAYER_DAMAGE_STATUS_POISON_CLEAR = 6
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
    s32 curse_phase_limit;
    s32 darkness_phase_limit;
    s32 paralysis_duration;
    s32 slow_duration;
    s32 poison_duration;
    u16 flags = status_flags;

    if (player_state.weapon_guard_active != 0) {
        return;
    }
    if (player_state.equipped_accessory_id == KF_PLAYER_STATUS_GUARD_ACCESSORY_ID
        || player_state.equipped_extra_id == KF_PLAYER_STATUS_GUARD_ACCESSORY_ID) {
        if (rand() < KF_PLAYER_STATUS_GUARD_CHANCE) {
            flags &= 0xfff8;
        }
    }
    if (player_state.equipped_accessory_id == KF_PLAYER_STATUS_DURATION_HALVING_ACCESSORY_ID
        || player_state.equipped_extra_id == KF_PLAYER_STATUS_DURATION_HALVING_ACCESSORY_ID) {
        curse_phase_limit = 300;
        darkness_phase_limit = 250;
        poison_duration = 300;
        paralysis_duration = 100;
        slow_duration = 300;
    } else {
        curse_phase_limit = 600;
        darkness_phase_limit = 500;
        poison_duration = 600;
        paralysis_duration = 200;
        slow_duration = 600;
    }

    switch ((flags & 0xf) - 1) {
    case PLAYER_DAMAGE_STATUS_CURSE:
        player_state.curse_phase_limit = curse_phase_limit;
        player_state.curse_strength = 1;
        player_recalculate_combat_stats();
        break;
    case PLAYER_DAMAGE_STATUS_DARKNESS:
        player_state.darkness_phase_limit = darkness_phase_limit;
        break;
    case PLAYER_DAMAGE_STATUS_POISON:
        if (player_state.equipped_accessory_id == KF_PLAYER_POISON_GUARD_ACCESSORY_ID
            || player_state.equipped_extra_id == KF_PLAYER_POISON_GUARD_ACCESSORY_ID) {
            if (rand() < KF_PLAYER_STATUS_GUARD_CHANCE) {
                break;
            }
        }
        if (player_state.combat_components[KF_PLAYER_COMBAT_POISON_RESISTANCE]
            < ((rand() * KF_PLAYER_POISON_ROLL_SCALE) >> KF_PLAYER_POISON_ROLL_SHIFT)) {
            player_state.poison_timer = poison_duration;
        }
        break;
    case PLAYER_DAMAGE_STATUS_PARALYSIS:
        player_state.paralysis_timer = paralysis_duration;
        break;
    case PLAYER_DAMAGE_STATUS_SLOW:
        player_state.slow_timer = slow_duration;
        break;
    case PLAYER_DAMAGE_STATUS_POISON_CLEAR:
        player_state.poison_timer = 0;
        break;
    case PLAYER_DAMAGE_STATUS_MP_DRAIN:
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
    KF_RADIAL_NO_REACTION_ORIGIN = 0x8000,
    KF_RADIAL_BASE_SCALE_MASK = 0x7fff
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

    if (scale_and_flags & KF_RADIAL_NO_REACTION_ORIGIN) {
        reaction_origin = 0;
    }
    base_scale = scale_and_flags & KF_RADIAL_BASE_SCALE_MASK;

    if (mode == KF_RADIAL_MODE_PLAYER) {
        distance = vector_distance_between_with_reach(position, end, &player_state.camera_position,
                                  KF_RADIAL_REACH_OFFSET, KF_PLAYER_HEIGHT);
    } else if (mode == KF_RADIAL_MODE_PLAYER_ABOVE) {
        if (position->vy < player_state.camera_position.vy - KF_PLAYER_HEIGHT) {
            distance = KF_DISTANCE_OUTSIDE_REACH;
        } else {
            distance = vector_distance_between_with_reach(position, end,
                                      &player_state.camera_position,
                                      KF_RADIAL_REACH_OFFSET, KF_PLAYER_HEIGHT);
        }
    } else {
        distance = player_distance_to_point(position->vx, position->vy,
                                            position->vz, end, mode);
        if (distance == KF_DISTANCE_NONE) {
            distance = KF_DISTANCE_OUTSIDE_REACH;
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
