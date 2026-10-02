#include <kf/lib/address.h>
#include <kf/lib/types.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/menu.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>

ADDRESS(0x8002897c, 0x1c)
s32 func_8002897c(s32 value)
{
    s32 result = 0;
    if (value < 81) {
        result = value >= 71;
    }
    return result;
}

ADDRESS(0x80028998, 0x528)
void func_80028998(void)
{
    const u16 *attack_mask;
    s32 charge_gain;
    u8 timer;

    if (player_state.weapon_magic_shots_configured == 0
        && (player_state.flags_140.word & 0x00800080) == 0x80
        && player_state.weapon_magic_shots_remaining == 0) {
        func_8002722c(player_state.unknown_97);
    }

    if ((player_state.flags_140.word & 0x08000800) == 0x800) {
        if (player_state.unknown_98 != 0xff) {
            func_8002722c(player_state.unknown_98);
        }
        if (player_state.unknown_99 != 0xff) {
            if (game_counter_bytes[player_state.unknown_99] != 0) {
                if (func_8002897c(player_state.unknown_99) != 0) {
                    func_80018f8c(player_state.unknown_99);
                } else {
                    event_scene_command_dispatch(&player_state.camera_position,
                                  &player_state.camera_rotation_target,
                                  player_state.unknown_99);
                }
            } else {
                notify_enqueue(20);
            }
        }
    }

    if (player_state.unknown_d1[0] != 0xff) {
        timer = player_state.unknown_d1[3] - 1;
        player_state.unknown_d1[3] = timer;
        if (timer == 0) {
            func_80025a18(player_state.unknown_d1[0]);
            timer = player_state.unknown_d1[1] - 1;
            player_state.unknown_d1[1] = timer;
            if (timer == 0) {
                player_state.unknown_d1[0] = 0xff;
            } else {
                player_state.unknown_d1[3] = player_state.unknown_d1[2];
            }
        }
    }

    if ((player_state.flags_140.word & 0x00200020) == 0x00200020
        && player_state.equipped_shield_id != 50) {
        if (player_state.unknown_0c[1] != 0) {
            player_state.unknown_0c[1]--;
        }
        player_state.weapon_charge_delay = 1;
        player_state.attack_charge_current -= 500;
        if ((s16)player_state.attack_charge_current <= 0) {
            player_state.attack_charge_current = 0;
            player_state.weapon_charge_delay = 40;
        }
        player_state.magic_charge -= 500;
        if ((s16)player_state.magic_charge <= 0) {
            player_state.magic_charge = 0;
        }
        player_state.damage_scale -= 128;
        if (player_state.damage_scale < 1024) {
            player_state.damage_scale = 1024;
        }
        return;
    }

    if (player_state.selected_magic_record == 0) {
        player_state.magic_charge += player_charge_gain_for_rank(player_state.magic, 0);
    } else {
        charge_gain = player_charge_gain_for_rank(player_state.magic,
                                    player_state.selected_magic_record->unknown_01[0]);
        if (player_state.equipped_head_id == 24) {
            charge_gain >>= 1;
        }
        player_state.magic_charge += charge_gain;
    }
    if (player_state.magic_charge > 5000) {
        player_state.magic_charge = 5000;
    }
    player_state.damage_scale += 128;
    if (player_state.damage_scale > 4096) {
        player_state.damage_scale = 4096;
    }
    player_state.unknown_0c[1] = 1;

    if (player_state.weapon_magic_shots_remaining != 0) {
        player_state.weapon_magic_shots_remaining--;
    } else {
        player_state.unknown_78 = DAT_800667e8.attack_masks;
    }

    if ((player_state.flags_140.low & 0xb0) != 0
        && (player_state.flags_140.halves.high & 0xb0) == 0
        && player_state.equipped_weapon_record->unknown_24 != 0) {
        if (player_has_power_and_magic_60() == 0) {
            goto cancel_weapon_attack;
        }
        attack_mask = player_state.unknown_78;
        if ((player_state.flags_140.low & attack_mask[0]) == 0) {
            goto cancel_weapon_attack;
        }
        if (attack_mask == DAT_800667e8.attack_masks
            && (player_state.attack_charge_current != 5000
                || player_state.magic_charge != 5000)) {
            goto cancel_weapon_attack;
        }
        player_state.unknown_78 = attack_mask + 1;
        if (attack_mask[1] == 0xffff) {
            player_begin_weapon_attack(1);
            return;
        }
        player_state.weapon_magic_shots_remaining = 20;
        goto after_weapon_attack;
cancel_weapon_attack:
        player_state.weapon_magic_shots_remaining = 0;
    }

after_weapon_attack:
    if (player_state.weapon_charge_delay == 0
        && player_state.weapon_magic_shots_remaining == 0
        && (player_state.flags_140.word & 0x00100010) == 0x10) {
        player_begin_weapon_attack(0);
    }
}
