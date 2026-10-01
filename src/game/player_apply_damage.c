#include <kf/lib/address.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

extern void func_80024498(const VECTOR *origin, s32 damage, s32 reaction_flags);

enum {
    KF_PLAYER_STATUS_GUARD_CHANCE = 16384,
    KF_PLAYER_POISON_ROLL_SCALE = 100,
    KF_PLAYER_POISON_ROLL_SHIFT = 15
};

RODATA(0x80011148, 0x1c)

ADDRESS(0x800248a8, 0x3fc)
void func_800248a8(u16 damage0, u16 damage1, u16 damage2, u16 status_flags,
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
    func_80024498(origin, damage_loss, flags);
}
