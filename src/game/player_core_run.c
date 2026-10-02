#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/graphics.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>
#include <kf/game/resources.h>

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
    if (player_state.unknown_62 != 0) {
        func_80040308(10, 16, 15, &player_state.camera_position, NULL);
    }
    if (player_state.unknown_64 != 0) {
        func_80040308(10, 16, 17, &player_state.camera_position, NULL);
    }
    player_recalculate_combat_stats();
}

ADDRESS(0x8002360c, 0x208)
void func_8002360c(
    s32 first, s32 second, s32 third, s32 fourth, s32 fifth, s32 optional_resource)
{
    cd_request_wait_idle();
    state_8017d118.values_04[0] = 99;
    state_8017d118.values_04[1] = 99;
    state_8017d118.values_04[2] = 99;
    state_8017d118.values_04[3] = 99;
    state_8017d118.values_04[4] = 99;
    func_80016260(first, second, third, 255, 255, 127, 127, 127);
    do {
        cd_request_yield();
        func_80016820();
    } while (state_8017d118.transition_active != 0);
    cd_request_wait_idle();
    DrawSync(0);
    VSync(0);
    DrawSync(0);
    VSync(0);
    if (optional_resource != 255) {
        func_80016260(255, 255, optional_resource, 255, 255, 127, 127, 127);
        do {
            cd_request_yield();
            func_80016820();
        } while (state_8017d118.transition_active != 0);
        cd_request_wait_idle();
        DrawSync(0);
        VSync(0);
        DrawSync(0);
        VSync(0);
    }

    player_sync_position_to_map();
    func_8002bc18();
    func_80036e24(0x82, 0x1000, 0x1000, 0);
    if (game_graphics_runtime.asset_registry_entries[0x181] == 0) {
        resource_tmd_queue_read(0, 0x101, 0x181);
    }
    cd_request_wait_idle();
    func_80036e24(0x82, 0x1000, 0, -128);
    func_80016260(255, 255, 255, fourth, fifth, 127, 127, 127);
}

enum {
    KF_PLAYER_VALUE_SCALE_BITS = 6,
    KF_PLAYER_VALUE_LIMIT = 5000
};

ADDRESS(0x80023814, 0x54)
s32 func_80023814(s32 value, s32 rank)
{
    s32 scaled = ((value << KF_PLAYER_VALUE_SCALE_BITS) / (rank + 1)) + 1;
    if (scaled >= KF_PLAYER_VALUE_LIMIT) {
        return KF_PLAYER_VALUE_LIMIT;
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
    PLAYER_ACCESSORY_FIRST = 0x37,
    PLAYER_ACCESSORY_SECOND = 0x38,
    PLAYER_ACCESSORY_THIRD = 0x39,
    PLAYER_BONUS_OVERFLOW_LIMIT = 0x7fff,
    PLAYER_POWER_CAP_THRESHOLD = 1000
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

    if (player_state.unknown_62 != 0) {
        player_state.combat_components[5] += PLAYER_STATUS_DEFENSE_BONUS;
    }
    if (player_state.unknown_64 != 0) {
        player_state.combat_components[1] += PLAYER_STATUS_POWER_BONUS;
        player_state.combat_components[0] += PLAYER_STATUS_POWER_BONUS;
        player_state.combat_components[2] += PLAYER_STATUS_POWER_BONUS;
    }
    if (player_state.unknown_6e != 0) {
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
    if (player_state.equipped_accessory_id == PLAYER_ACCESSORY_FIRST
        || player_state.equipped_extra_id == PLAYER_ACCESSORY_FIRST) {
        player_state.attack_components[1] += 5;
        player_state.attack_components[2] += 5;
        player_state.attack_components[3] += 22;
        player_state.attack_components[7] += 12;
    }
    if (player_state.equipped_accessory_id == PLAYER_ACCESSORY_SECOND
        || player_state.equipped_extra_id == PLAYER_ACCESSORY_SECOND) {
        player_state.magic += 8;
    }
    if (player_state.equipped_accessory_id == PLAYER_ACCESSORY_THIRD
        || player_state.equipped_extra_id == PLAYER_ACCESSORY_THIRD) {
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
