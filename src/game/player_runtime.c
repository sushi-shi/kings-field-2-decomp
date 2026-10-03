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
#include <kf/game/actor.h>
#include <kf/game/animation.h>
#include <kf/game/collision_cache.h>
#include <kf/game/asset.h>
#include <kf/game/map_cell.h>
#include <psyq/libc.h>
#include <stdarg.h>



typedef union KfPlayerMagicSpawnRecord {
    SVECTOR offset;
    struct {
        s16 x;
        s16 y;
        s16 z;
        s16 effect_kind;
    } fields;
} KfPlayerMagicSpawnRecord;

typedef char kf_player_magic_spawn_record_size[
    sizeof(KfPlayerMagicSpawnRecord) == 8 ? 1 : -1];
typedef char kf_player_magic_spawn_effect_kind_offset[
    (u32)&((KfPlayerMagicSpawnRecord *)0)->fields.effect_kind == 6 ? 1 : -1];


DATA(0x800667a0, 0x28)
KfPlayerMagicSpawnRecord player_magic_spawn_records[5] = {
    {{0, 0, 100, 0}},
    {{-1000, 0, 0, 46}},
    {{-2000, 0, -2000, 46}},
    {{1000, 0, 0, 46}},
    {{2000, 0, -2000, 46}}
};

DATA(0x800667c8, 0x20)
SVECTOR player_magic_square_offsets[4] = {
    {-400, -400, 200, 0},
    {-400, 400, 200, 0},
    {400, -400, 200, 0},
    {400, 400, 200, 0}
};

DATA(0x800667e8, 0x14)
KfPlayerMagicIdSequence player_magic_id_sequence = {
    {39, 40, 60, 66, 84, 86, 39, 40, 60, 66, 84, 86},
    {0x20, 0x10, 0x80, 0xffff}
};


DATA(0x800758f0, 0x4b0)
KfPlayerLevelGrowth player_level_growth_table[KF_PLAYER_LEVEL_GROWTH_COUNT];

DATA(0x80075da0, 0xc000)
static KfWeaponAssetBuffer player_weapon_asset_buffer;


DATA(0x801984d0, 0x160)
KfPlayerState player_state;

DATA(0x801c7078, 0x4c8)
KfWeaponRecordGame player_weapon_records[18];

DATA(0x801c7540, 0x11844)
KfBss801c7540 bss_801c7540;




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


RODATA(0x80011128, 0x134)

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

ADDRESS(0x80024ed4, 0x78)
void player_get_camera_pose(VECTOR *position, SVECTOR *angles)
{
    position->vx = player_state.camera_position.vx;
    position->vz = player_state.camera_position.vz;
    position->vy = player_state.camera_vertical_offset + player_state.camera_position.vy
                 + player_state.landing_vertical_offset - KF_PLAYER_CAMERA_EYE_OFFSET;
    angles->vx = player_state.camera_rotation.angles[0];
    angles->vy = player_state.camera_rotation.angles[1];
    angles->vz = player_state.camera_rotation.angles[2];
}

ADDRESS(0x80024f4c, 0xb8)
void player_reset_status(void)
{
    player_state.magic_boost_timer = 0;
    player_state.full_mp_timer = 0;
    player_state.map_marker_visual_effect_timer = 0;
    player_state.magic_tint_phase_limit = 0;
    player_state.magic_tint_phase = 0;
    player_state.attack_boost_timer = 0;
    player_state.defense_boost_timer = 0;
    player_state.paralysis_timer = 0;
    player_state.slow_timer = 0;
    player_state.darkness_phase_limit = 0;
    player_state.darkness_phase = 0;
    player_state.curse_phase_limit = 0;
    player_state.curse_strength = 0;
    player_state.poison_timer = 0;
    player_state.fatal_fall_latch = 0;
    player_state.vitals.current_hp = player_state.vitals.maximum_hp;
    player_state.vitals.current_mp = player_state.vitals.maximum_mp;
    player_reset_view();
}

enum {
    PLAYER_INITIAL_LEVEL = 1,
    PLAYER_INITIAL_CAMERA_PITCH = 2500,
    PLAYER_INITIAL_CAMERA_X = 0x11800,
    PLAYER_INITIAL_CAMERA_Y = -12800,
    PLAYER_INITIAL_CAMERA_Z = 0x18000,
    PLAYER_INITIAL_MAP_LAYER = 5
};

ADDRESS(0x80025004, 0x180)
void player_initialize_state(void)
{
    s32 i;
    KfMagicRecord *entry;

    player_state.experience = 0;
    player_state.level = PLAYER_INITIAL_LEVEL;
    player_state.gold = 0;
    player_state.damage_red_overlay_scale = 0;
    player_state.damage_red_overlay_decay = 0;
    player_state.equipped_head_id = KF_EQUIPMENT_NONE;
    player_state.equipped_body_id = KF_EQUIPMENT_NONE;
    player_state.equipped_leg_id = KF_EQUIPMENT_NONE;
    player_state.equipped_shield_id = KF_EQUIPMENT_NONE;
    player_state.equipped_arm_id = KF_EQUIPMENT_NONE;
    player_state.equipped_accessory_id = KF_EQUIPMENT_NONE;
    player_state.equipped_extra_id = KF_EQUIPMENT_NONE;
    player_state.vitals.maximum_hp = player_level_growth_table[0].maximum_hp;
    player_state.vitals.maximum_mp = player_level_growth_table[0].maximum_mp;
    player_state.base_physical_power = player_level_growth_table[0].physical_power_step;
    player_state.base_magic = player_level_growth_table[0].magic_step;
    player_state.next_level_experience = player_level_growth_table[0].experience_threshold;
    player_set_equipment_slot(KF_EQUIPMENT_NONE, KF_EQUIPMENT_NONE);
    player_state.selected_magic_record = NULL;
    player_set_primary_magic_shortcut_id(KF_EQUIPMENT_NONE);
    player_equip_weapon(0);
    player_set_secondary_magic_shortcut_id(KF_EQUIPMENT_NONE);
    player_set_secondary_item_shortcut_id(KF_EQUIPMENT_NONE);
    player_state.camera_rotation_target.angles[0] = 0;
    player_state.camera_rotation_target.angles[1] = PLAYER_INITIAL_CAMERA_PITCH;
    player_state.camera_rotation_target.angles[2] = 0;
    player_state.camera_position.vy = PLAYER_INITIAL_CAMERA_Y;
    player_state.camera_position.vx = PLAYER_INITIAL_CAMERA_X;
    player_state.camera_position.vz = PLAYER_INITIAL_CAMERA_Z;
    player_state.map_layer_index = PLAYER_INITIAL_MAP_LAYER;
    entry = effect_state.magic_records;
    for (i = KF_MAGIC_RECORD_COUNT - 1; i != -1; i--) {
        entry->menu_available = 0;
        entry++;
    }
    player_reset_status();
}

ADDRESS(0x80025184, 0x6c)
void game_initialize_session(void)
{
    player_state.weapon_asset_buffer = (KfAssetHeader *)&player_weapon_asset_buffer;
    player_initialize_state();
    player_state.audio_effects_enabled = 1;
    player_state.audio_music_enabled = 1;
    player_state.hud_gauges_enabled = 1;
    player_state.compass_enabled = 1;
    player_state.item_preview_enabled = 1;
    player_state.walking_bob_enabled = 1;
    player_state.force_actor_lifecycle_refresh = 0;
}

ADDRESS(0x800251f0, 0x44)
void player_clear_motion(void)
{
    player_state.yaw_step = 0;
    player_state.pitch_step = 0;
    player_state.movement_speed.unsigned_value = 0;
    player_state.forward_velocity = 0;
    player_state.strafe_velocity = 0;
    player_state.flags_140.low &= KF_PLAYER_MOTION_FLAGS_KEPT;
}

ADDRESS(0x80025234, 0xb0)
void player_sync_position_to_map(void)
{
    s32 layer;

    player_state.equipment_effect_ticks = 0;
    layer = 2;
    if (player_state.map_layer_index == 0) {
        layer = 1;
    }
    player_state.camera_position.vy =
        collision_sample_map_layer_height(layer, player_state.camera_position.vx,
                      player_state.camera_position.vz,
                      KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT);
    player_update_collision_bounds();
    player_state.force_actor_lifecycle_refresh = 1;
    player_state.vertical_motion_state = KF_PLAYER_VERTICAL_GROUNDED;
    player_state.vertical_velocity = 0;
    player_state.death_state = KF_PLAYER_REACTION_NORMAL;
    player_clear_motion();
    map_cell_add_layer_occupancy(player_state.camera_position.vx,
                  player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS, 1);
}

ADDRESS(0x800252e4, 0xc8)
s32 player_distance_to_point_in_cone(
    const VECTOR *point, s16 facing, s32 max_distance, s32 angle_tolerance)
{
    s32 distance;
    s16 delta;

    distance = player_distance_to_point(point->vx, KF_DISTANCE_IGNORE_HEIGHT, point->vz, max_distance, 0);
    if (distance != KF_DISTANCE_NONE) {
        delta = (vector_xz_to_angle(
                     player_state.camera_position.vx - point->vx,
                     player_state.camera_position.vz - point->vz)
                 - facing) & KF_ANGLE_WRAP_MASK;
        delta = angle_error_magnitude(delta);
        if (angle_tolerance < delta) {
            distance = KF_DISTANCE_NONE;
        }
    }
    return distance;
}

ADDRESS(0x800253ac, 0x50)
s32 player_distance_to_point(
    s32 point_x, s32 point_y, s32 point_z, s32 max_distance, s32 point_height)
{
    return vector_distance_to_point(
        &player_state.camera_position, point_x, point_y, point_z, max_distance,
        KF_PLAYER_HEIGHT, point_height);
}



ADDRESS(0x800253fc, 0x10)
void player_set_primary_magic_shortcut_id(u8 value)
{
    player_state.primary_magic_shortcut_id = value;
}

ADDRESS(0x8002540c, 0x28)
void player_set_secondary_magic_shortcut_id(u8 value)
{
    player_state.secondary_magic_shortcut_id = value;
    if (value != KF_EQUIPMENT_NONE) {
        player_state.secondary_item_shortcut_id = KF_EQUIPMENT_NONE;
    }
}

ADDRESS(0x80025434, 0x28)
void player_set_secondary_item_shortcut_id(u8 value)
{
    player_state.secondary_item_shortcut_id = value;
    if (value != KF_EQUIPMENT_NONE) {
        player_state.secondary_magic_shortcut_id = KF_EQUIPMENT_NONE;
    }
}

ADDRESS(0x8002545c, 0x240)
void player_set_equipment_slot(u8 item_id, u8 slot)
{
    switch (slot) {
    case KF_EQUIPMENT_SLOT_HEAD:
        player_state.equipped_head_id = item_id;
        break;
    case KF_EQUIPMENT_SLOT_BODY:
        player_state.equipped_body_id = item_id;
        break;
    case KF_EQUIPMENT_SLOT_LEG:
        player_state.equipped_leg_id = item_id;
        break;
    case KF_EQUIPMENT_SLOT_SHIELD:
        player_state.equipped_shield_id = item_id;
        break;
    case KF_EQUIPMENT_SLOT_ARM:
        player_state.equipped_arm_id = item_id;
        break;
    case KF_EQUIPMENT_SLOT_ACCESSORY:
        player_state.equipped_accessory_id = item_id;
        break;
    case KF_EQUIPMENT_SLOT_EXTRA:
        player_state.equipped_extra_id = item_id;
        break;
    }

    if (player_state.equipped_head_id != KF_EQUIPMENT_NONE) {
        player_state.equipped_head_record = &bss_801c7540.equipment_records[player_state.equipped_head_id];
    } else {
        player_state.equipped_head_record = NULL;
    }
    if (player_state.equipped_body_id != KF_EQUIPMENT_NONE) {
        player_state.equipped_body_record = &bss_801c7540.equipment_records[player_state.equipped_body_id];
    } else {
        player_state.equipped_body_record = NULL;
    }
    if (player_state.equipped_leg_id != KF_EQUIPMENT_NONE) {
        player_state.equipped_leg_record = &bss_801c7540.equipment_records[player_state.equipped_leg_id];
    } else {
        player_state.equipped_leg_record = NULL;
    }
    if (player_state.equipped_shield_id != KF_EQUIPMENT_NONE) {
        player_state.equipped_shield_record = &bss_801c7540.equipment_records[player_state.equipped_shield_id];
    } else {
        player_state.equipped_shield_record = NULL;
    }
    if (player_state.equipped_arm_id != KF_EQUIPMENT_NONE) {
        player_state.equipped_arm_record = &bss_801c7540.equipment_records[player_state.equipped_arm_id];
    } else {
        player_state.equipped_arm_record = NULL;
    }
    if (player_state.equipped_accessory_id != KF_EQUIPMENT_NONE) {
        player_state.equipped_accessory_record = &bss_801c7540.equipment_records[player_state.equipped_accessory_id];
    } else {
        player_state.equipped_accessory_record = NULL;
    }
    if (player_state.equipped_extra_id != KF_EQUIPMENT_NONE) {
        player_state.equipped_extra_record = &bss_801c7540.equipment_records[player_state.equipped_extra_id];
    } else {
        player_state.equipped_extra_record = NULL;
    }
    player_recalculate_combat_stats();
}

enum {
    PLAYER_WEAPON_CHARGE_DELAY_UPDATES = 10,
    PLAYER_WEAPON_ARCHIVE_SLOT = 5,
    PLAYER_WEAPON_ARCHIVE_FIRST_ENTRY = 49
};

ADDRESS(0x8002569c, 0xb8)
void player_equip_weapon(u8 weapon_id)
{
    player_state.attack_charge_current = 0;
    player_state.attack_charge_committed = 0;
    player_state.weapon_charge_delay = PLAYER_WEAPON_CHARGE_DELAY_UPDATES;
    player_state.equipped_weapon_id = weapon_id;
    if (weapon_id != KF_EQUIPMENT_NONE) {
        player_state.equipped_weapon_record = &player_weapon_records[weapon_id];
        cd_archive_read(PLAYER_WEAPON_ARCHIVE_SLOT,
                        weapon_id + PLAYER_WEAPON_ARCHIVE_FIRST_ENTRY,
                        (u_long *)player_state.weapon_asset_buffer);
        asset_registry_set(KF_PLAYER_WEAPON_ASSET_INDEX,
                           player_state.weapon_asset_buffer);
    }
    player_state.weapon_attack_phase = KF_WEAPON_ATTACK_INACTIVE;
    player_state.weapon_animation_cache = NULL;
    player_state.weapon_magic_shots_remaining = 0;
    player_state.weapon_guard_active = 0;
    player_recalculate_combat_stats();
}

ADDRESS(0x80025754, 0x124)
void player_begin_weapon_attack(s32 mode)
{
    if (player_state.weapon_attack_phase != KF_WEAPON_ATTACK_INACTIVE
        || player_state.equipped_weapon_id == KF_EQUIPMENT_NONE
        || player_state.paralysis_timer != 0) {
        return;
    }

    player_state.weapon_attack_mode = mode;
    player_state.weapon_attack_phase = 0;
    if (mode == 0) {
        player_state.weapon_attack_window = player_state.equipped_weapon_record->normal_attack_end_phase;
        player_state.weapon_next_sound_phase = player_state.equipped_weapon_record->normal_attack_sound_phase;
    } else {
        player_state.weapon_attack_window = player_state.equipped_weapon_record->alternate_attack_window_start;
        player_state.weapon_next_sound_phase = player_state.equipped_weapon_record->alternate_attack_sound_start_phase;
    }
    player_state.attack_charge_committed = player_state.attack_charge_current;
    if (player_state.attack_charge_current == KF_PLAYER_CHARGE_FULL
        && player_state.magic_charge == KF_PLAYER_CHARGE_FULL) {
        player_state.weapon_attack_fully_charged = 1;
        player_state.weapon_magic_shots_configured = player_state.equipped_weapon_record->magic_shots;
    } else {
        player_state.weapon_attack_fully_charged = 0;
    }
    player_state.attack_charge_current = 0;
    player_state.unknown_9c[0] = 0;
}

ADDRESS(0x80025878, 0x1a0)
KfActor *player_probe_view_target_and_vectors(s32 scale, VECTOR *position, SVECTOR *direction,
                       s32 *distance)
{
    struct KfEulerAngles angles;
    KfActor *actor;
    KfTargetCandidate *target;
    s32 height;

    if (position != 0) {
        angles.x = -player_state.camera_rotation.angles[0];
        angles.y = player_state.camera_rotation.angles[1];
        angles.z = player_state.camera_rotation.angles[2];
        vector_rotate_yxz(&angles, &player_state.magic_origin_offset, position);
        position->vx += player_state.camera_position.vx;
        height = position->vy - KF_PLAYER_CAMERA_EYE_OFFSET;
        position->vy = height + player_state.camera_position.vy;
        position->vz += player_state.camera_position.vz;
    }

    actor = actor_find_best_in_cone(&player_state.camera_position,
                          (s16)player_state.camera_rotation.angles[1],
                          (s16)player_state.camera_rotation.angles[0], 0x55f0,
                          0x200, 0x200, distance, 0);
    actor_state.actor_93c8 = actor;
    if (actor != 0) {
        target = actor_find_target_of_type(&actor_state.target_groups[actor->group_index],
                                           0x82);
        if (target != 0 && !(actor->flags & KF_ACTOR_FLAG_BLOCK_PLAYER_TARGETING)) {
            actor_set_target(actor, target);
        }
    }

    if (direction != 0) {
        angles.x = player_state.camera_rotation.angles[0];
        angles.y = player_state.camera_rotation.angles[1];
        angles.z = player_state.camera_rotation.angles[2];
        pitch_yaw_to_forward_vector(&angles, direction);
        vector3s_scale_shift12(scale, direction);
    }
    return actor;
}

ADDRESS(0x80025a18, 0x918)
void player_dispatch_magic_effect(s32 effect_id, ...)
{
    VECTOR position;
    SVECTOR direction;
    s32 distance;
    s32 adjusted_distance;
    KfActor *actor;
    KfEffectRecord *effect;
    s32 i;
    s32 kind;
    s32 rotation_scale;
    s32 target_scale;
    s32 simple_scale;
    s32 case3_z;
    va_list arguments;
    const VECTOR *override_position;
    va_start(arguments, effect_id);

    switch (effect_id) {
    case 7:
        simple_scale = 1000;
simple_probe:
        player_probe_view_target_and_vectors(simple_scale, &position, &direction, &distance);
emit_simple_effect:
        effect_construct_record(10, 0x12, effect_id, &position, &direction);
        break;
    case 2:
        effect_construct_record(10, 0x13, effect_id, &player_state.camera_position,
                       0, 0x1000, 0x100, 0x1000);
        break;
    case 3:
        actor = player_probe_view_target_and_vectors(5000, &position, &direction, &distance);
        if (actor == 0) {
            position.vx += direction.vx;
            position.vy = player_state.camera_position.vy;
            case3_z = position.vz + direction.vz;
            goto case3_store_z;
        }
        position.vx = ((s32)actor->motion.vector.vx << 14) / 600 + actor->position.vx;
        position.vy = ((s32)actor->motion.vector.vy << 14) / 600 + actor->position.vy;
        position.vz = ((s32)actor->motion.vector.vz << 14) / 600 + actor->position.vz;
        if (collision_query_shapes_with_layer_sample(position.vx, position.vy, position.vz, 10, 10)) {
            position.vx = actor->position.vx;
            position.vy = actor->position.vy;
            case3_z = actor->position.vz;
            goto case3_store_z;
        }
        goto case3_emit;
case3_store_z:
        position.vz = case3_z;
case3_emit:
        effect_construct_record(10, 0x12, 0x72, &position, 0);
        break;
    case 0:
        actor = player_probe_view_target_and_vectors(5000, &position, &direction, &distance);
        if (actor == 0) {
            kind = 255;
        } else {
            kind = actor - actor_state.actors;
        }
        position.vx += direction.vx;
        position.vz += direction.vz;
        effect_construct_record(10, 0x12, 0x6f, &position, 0, kind);
        break;
    case 13: {
        const SVECTOR *sequence = player_magic_square_offsets;
        for (i = 3; i != -1; i--) {
            player_state.magic_origin_offset = *sequence;
            player_probe_view_target_and_vectors(800, &position, &direction, &distance);
            sequence++;
            direction.vx += -32 + (rand() >> 9);
            direction.vy += -32 + (rand() >> 9);
            direction.vz += -32 + (rand() >> 9);
            effect_construct_record(10, 0x12, effect_id, &position, &direction);
        }
        break;
    }
    case 51:
        effect_id = 0x76;
        goto simple_effect;
    case 52:
        effect_id = 0x77;
        goto simple_effect;
    case 4:
simple_effect:
        rotation_scale = 700;
probe_rotation_effect:
        player_probe_view_target_and_vectors(rotation_scale, &position, &direction, &distance);
        effect_construct_record(10, 0x12, effect_id, &position, &direction,
                       &player_state.camera_rotation);
        break;
    case 11:
        player_probe_view_target_and_vectors(600, &position, &direction, &adjusted_distance);
        if (adjusted_distance != -1) {
            adjusted_distance = adjusted_distance / 600 - 8;
            if (adjusted_distance < 2) {
                adjusted_distance = 2;
            }
        } else {
            adjusted_distance = 10;
        }
        direction.vy = 0;
        effect_construct_record(10, 0x12, 0x67, &position, &direction, adjusted_distance);
        break;
    case 5:
        target_scale = 200;
select_actor_effect:
        actor = player_probe_view_target_and_vectors(target_scale, &position, &direction, &distance);
        if (actor == 0) {
            kind = 255;
        } else {
            kind = actor - actor_state.actors;
        }
        effect_construct_record(10, 0x12, effect_id, &position, &direction, kind);
        break;
    case 9:
        target_scale = 500;
        goto select_actor_effect;
    case 8:
        player_probe_view_target_and_vectors(700, &position, &direction, &distance);
        effect_construct_record(10, 0x12, 0x6a, &position, &direction,
                       &player_state.camera_rotation);
        break;
    case 10:
        rotation_scale = 300;
        goto probe_rotation_effect;
    case 6:
        rotation_scale = 250;
        goto probe_rotation_effect;
    case 12: {
        s16 old_yaw = player_state.camera_rotation.angles[1];
        player_state.camera_rotation.angles[1] -=
            (u16)player_state.magic_origin_offset.vx * 2;
        player_probe_view_target_and_vectors(150, &position, &direction, &distance);
        player_state.magic_origin_offset.vx += 100;
        player_state.camera_rotation.angles[1] = old_yaw;
        effect_construct_record(10, 0x12, effect_id, &position, &direction,
                       &player_state.camera_rotation, 600, 60, 128, 140, 160);
        break;
    }
    case 1:
        simple_scale = 500;
        goto simple_probe;
    case 43:
        effect_id = 0x73;
        goto sequence_effect;
    case 42:
        effect_id = 0x71;
sequence_effect: {
        const KfPlayerMagicSpawnRecord *record = player_magic_spawn_records;
        player_state.magic_origin_offset = record->offset;
        player_probe_view_target_and_vectors(600, &position, &direction, &distance);
        effect = effect_construct_record(10, 0x12, effect_id,
                               &position, &direction, &player_state.camera_rotation);
        if (effect != 0) {
            s32 index = effect - effect_state.records;
            record++;
            for (i = 3; i != -1; i--) {
                player_state.magic_origin_offset = record->offset;
                player_probe_view_target_and_vectors(600, &position, &direction, &distance);
                effect_construct_record(10, 0x12, record->fields.effect_kind,
                                        &position, &direction, index);
                record++;
            }
        }
        break;
    }
    case 44:
        player_probe_view_target_and_vectors(1000, &position, &direction, &distance);
        effect_id = 0x75;
        goto emit_rotation_effect;
    case 45:
        player_probe_view_target_and_vectors(1000, &position, &direction, &distance);
        effect_id = 0x74;
        goto emit_simple_effect;
    case 40:
        rotation_scale = 1000;
        goto probe_rotation_effect;
    case 39:
        player_probe_view_target_and_vectors(50, 0, &direction, &distance);
        override_position = va_arg(arguments, const VECTOR *);
        position = *override_position;
        goto emit_rotation_effect;
    case 49: {
        const VECTOR *override_position;
        /* Cases 49 and 50 omit the rotation argument. */
        player_probe_view_target_and_vectors(550, 0, &direction, &distance);
        override_position = va_arg(arguments, const VECTOR *);
        position = *override_position;
        goto emit_simple_effect;
    }
    case 50: {
        const VECTOR *override_position;
        override_position = va_arg(arguments, const VECTOR *);
        position = *override_position;
        goto emit_simple_effect;
    }
    case 34:
    case 35:
    case 38:
        player_probe_view_target_and_vectors(900, &position, &direction, &distance);
        goto emit_rotation_effect;
emit_rotation_effect:
        effect_construct_record(10, 0x12, effect_id, &position, &direction,
                       &player_state.camera_rotation);
        break;
    case 15:
        effect_construct_record(10, 0x10, 15, &player_state.camera_position,
                       &direction);
        player_state.defense_boost_timer = 900;
        player_recalculate_combat_stats();
        break;
    case 17:
        effect_construct_record(10, 0x10, 17, &player_state.camera_position,
                       &direction);
        player_state.attack_boost_timer = 900;
        player_recalculate_combat_stats();
        break;
    case 14:
        effect_construct_record(10, 0x10, 14, &player_state.camera_position,
                       &direction);
        break;
    case 16:
        effect_construct_record(10, 0x10, 16, &player_state.camera_position,
                       &direction);
        break;
    case 19:
        effect_construct_record(10, 0x10, 19, &player_state.camera_position,
                       &direction);
        break;
    default:
        break;
    }
    va_end(arguments);
}
