#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_object.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <psyq/libc.h>
#include <psyq/sdk.h>

DATA(0x8006d6b0, 0x20)
static RECT player_status_texture_rows[4] = {
    {0x240, 0x119, 16, 1},
    {0x240, 0x117, 16, 1},
    {0x240, 0x11a, 16, 1},
    {0x240, 0x118, 16, 1}
};

RODATA(0x80011300, 0x4c)

ADDRESS(0x80028ec0, 0xe8)
void func_80028ec0(void)
{
    player_state.reaction.damage.motion.vx = angle_velocity_step(
        0, (s16)player_state.unknown_100[0], player_state.reaction.damage.motion.vx, 8, 4);
    player_state.reaction.damage.motion.vy = angle_velocity_step(
        0, (s16)player_state.unknown_100[1], player_state.reaction.damage.motion.vy, 8, 4);
    player_state.reaction.damage.motion.vz = angle_velocity_step(
        0, (s16)player_state.unknown_100[2], player_state.reaction.damage.motion.vz, 8, 4);

    player_state.unknown_100[0] += player_state.reaction.damage.motion.vx;
    player_state.unknown_100[1] += player_state.reaction.damage.motion.vy;
    player_state.unknown_100[2] += player_state.reaction.damage.motion.vz;
}

ADDRESS(0x80028fa8, 0x6c)
void func_80028fa8(void)
{
    u8 saved_c9 = player_state.unknown_c9[0];
    u8 saved_ca = player_state.unknown_c9[1];

    player_state.unknown_c9[0] = 0;
    player_state.unknown_c9[1] = 0;
    func_800335a0(0, 0);
    player_state.unknown_c9[0] = saved_c9;
    player_state.unknown_c9[1] = saved_ca;
    pool_release_all();
}

ADDRESS(0x80029014, 0x154)
void func_80029014(void)
{
    s32 value;

    if ((player_state.flags_140.word & 0x00200020) == 0x20) {
        if (player_state.death_state == 1) {
            func_800291ec(&map_object_state.objects[player_state.reaction.view.mode]);
        } else {
            func_80047c98(&player_state.camera_position, &player_state.camera_rotation_target);
        }
    }
    if ((player_state.flags_140.word & 0x00400040) != 0x40
        || player_state.weapon_attack_phase != -1) {
        return;
    }

    func_80028fa8();
    value = func_8001876c();
    if (value >= 0) {
        if (func_8002897c(value) == 0) {
            func_800335a0(0, 0);
            func_8004678c(&player_state.camera_position, &player_state.camera_rotation_target, value);
        }
    } else if (value == -3) {
        s32 resource;
        player_restore_equipment_effects();
        resource = state_8017d118.values_04[0];
        func_8002360c(resource, resource, resource, resource, resource, 255);
    }
    player_clear_motion();
}

ADDRESS(0x80029168, 0x68)
void func_80029168(void)
{
    player_state.death_state = 0;
    player_state.forward_velocity = 0;
    player_state.strafe_velocity = 0;
    player_state.unknown_100[2] = 0;
    player_state.unknown_100[1] = 0;
    player_state.unknown_100[0] = 0;
    player_state.unknown_108.components[2] = 0;
    player_state.unknown_108.components[1] = 0;
    player_state.unknown_108.components[0] = 0;
    player_state.unknown_110[2] = 0;
    player_state.unknown_110[1] = 0;
    player_state.unknown_110[0] = 0;
}

ADDRESS(0x800291d0, 0x1c)
void func_800291d0(u8 mode)
{
    player_state.death_state = 1;
    player_state.reaction.view.mode = mode;
}

struct KfMapObjectRecord40 {
    u8 unknown_00;
    u8 unknown_01;
    u8 unknown_02[0x0a];
    SVECTOR unknown_0c;
    u8 unknown_14[0x24];
    s16 unknown_38;
};

typedef char kf_player_map_object_vector_offset[
    (u32)&((KfMapObjectRecord40 *)0)->unknown_0c == 0x0c ? 1 : -1];
typedef char kf_player_map_object_halfword_offset[
    (u32)&((KfMapObjectRecord40 *)0)->unknown_38 == 0x38 ? 1 : -1];

void func_80029428(const SVECTOR *rotation);

ADDRESS(0x800291ec, 0x1e8)
void func_800291ec(KfMapObject *object)
{
    KfMapObjectRecord40 *record;

    if (player_state.death_state != 1) {
        return;
    }
    if (player_state.reaction.view.mode != object - map_object_state.objects) {
        return;
    }

    record = object->extra_40.record;
    player_state.camera_rotation_target.angles[0] += player_state.unknown_108.components[0];
    player_state.camera_rotation_target.angles[1] += player_state.unknown_108.components[1];
    player_state.camera_rotation_target.angles[2] += player_state.unknown_108.components[2];
    player_state.unknown_108.components[0] = 0;
    player_state.unknown_108.components[1] = 0;
    player_state.unknown_108.components[2] = 0;

    if (record->unknown_01 == 0) {
        SVECTOR rotation;
        rotation.vx = (record->unknown_0c.vx * record->unknown_38) >> 15;
        rotation.vy = ((record->unknown_0c.vy * record->unknown_38) >> 15) + 512;
        rotation.vz = (record->unknown_0c.vz * record->unknown_38) >> 15;
        func_80029428(&rotation);
    } else {
        struct KfVecXZi offset;
        angle_to_forward_xz(object->rotation.vy + 1024, &offset);
        vector2i_scale_shift11(900, &offset);
        player_state.death_state = 5;
        player_state.reaction.position.mode = 0;
        player_state.reaction.position.position.vx = object->position.vx + offset.x;
        player_state.reaction.position.position.vz = object->position.vz + offset.z;
        player_state.reaction.position.position.vy = object->position.vy;
    }
}

ADDRESS(0x800293d4, 0x54)
void func_800293d4(u8 mode)
{
    player_state.death_state = 2;
    player_state.reaction.view.mode = mode;
    player_state.reaction.view.step = 0;
    player_state.reaction.view.rotation = player_state.camera_rotation_target;
}

ADDRESS(0x80029428, 0x3c)
void func_80029428(const SVECTOR *rotation)
{
    player_state.death_state = 3;
    player_state.reaction.damage.rotation = *rotation;
}

ADDRESS(0x80029464, 0x94)
void func_80029464(const SVECTOR *rotation, const SVECTOR *motion, s16 duration)
{
    player_state.death_state = 0x10;
    player_state.reaction.damage.rotation = *rotation;
    player_state.reaction.damage.motion = *motion;
    player_state.unknown_13c = 3500;
    player_state.unknown_13e = duration;
    player_state.unknown_d0 = 0x40;
    player_state.unknown_13a = player_state.reaction.damage.rotation.vy;
}

ADDRESS(0x800294f8, 0x78)
void func_800294f8(const SVECTOR *rotation, const SVECTOR *motion, s16 duration)
{
    player_state.death_state = 0x12;
    player_state.reaction.damage.rotation = *rotation;
    player_state.reaction.damage.motion = *motion;
    player_state.unknown_13c = 3500;
    player_state.unknown_13e = duration;
}

enum {
    KF_PLAYER_DEATH_STATE = 0x11,
    KF_PLAYER_DEATH_SOUND = 0xb,
    KF_PLAYER_DEATH_VOLUME = 110
};

ADDRESS(0x80029570, 0x88)
void player_death_begin(const SVECTOR *rotation)
{
    if (player_state.death_state != KF_PLAYER_DEATH_STATE) {
        player_state.death_state = KF_PLAYER_DEATH_STATE;
        audio_play_sound(KF_PLAYER_DEATH_SOUND, KF_PLAYER_DEATH_VOLUME);
        if (rotation != NULL) {
            player_state.reaction.damage.rotation = *rotation;
        }
        player_state.reaction.damage.motion.vx = 0;
        player_state.unknown_106 = 0;
    }
}

ADDRESS(0x800295f8, 0x2c)
void func_800295f8(void)
{
    if (player_state.death_state == 0) {
        player_state.death_state = 4;
        player_state.reaction.damage.rotation.vx = 0;
    }
}

ADDRESS(0x80029624, 0xc4)
s16 func_80029624(u16 *phase, u16 *secondary, s32 duration, s32 scale)
{
    s16 current = *phase;
    s16 other = 0;
    s16 result;

    if (secondary != NULL) {
        other = *secondary;
    }
    if (current != 0 || other != 0) {
        if (current < other) {
            current++;
        } else {
            other = 0;
            current--;
        }
        if (current < duration) {
            result = current * scale / duration;
        } else {
            result = scale;
        }
        *phase = current;
        if (secondary != NULL) {
            *secondary = other;
        }
        return (s16)result;
    }
    return -1;
}

ADDRESS(0x800296e8, 0x74)
void player_adjust_hp(s32 delta)
{
    s32 value = player_state.vitals.current_hp;

    value += delta;

    if (value <= 0) {
        player_state.vitals.current_hp = 0;
        player_death_begin(NULL);
        return;
    }
    if (player_state.vitals.maximum_hp < value) {
        player_state.vitals.current_hp = player_state.vitals.maximum_hp;
    } else {
        player_state.vitals.current_hp = value;
    }
}

ADDRESS(0x8002975c, 0x58)
void player_adjust_mp(s32 delta)
{
    s32 value = player_state.vitals.current_mp;

    value += delta;

    if (value <= 0) {
        player_state.vitals.current_mp = 0;
        return;
    }
    if (player_state.vitals.maximum_mp < value) {
        player_state.vitals.current_mp = player_state.vitals.maximum_mp;
    } else {
        player_state.vitals.current_mp = value;
    }
}

ADDRESS(0x800297b4, 0xa8)
void func_800297b4(const KfEquipmentRecord *equipment)
{
    if (equipment->hp_regen_interval != 0
        && player_state.equipment_effect_ticks % equipment->hp_regen_interval == 0) {
        player_adjust_hp(1);
    }
    if (equipment->hp_drain_interval != 0
        && player_state.equipment_effect_ticks % equipment->hp_drain_interval == 0) {
        player_adjust_hp(-1);
    }
}

ADDRESS(0x8002985c, 0x112c)
void func_8002985c(void)
{
    s16 value;
    s32 index;
    s32 fraction;
    u8 object_index;
    u8 step;
    KfMapObject *object;

    actor_state.unknown_93a0 = 4;
    func_8002b73c(player_state.camera_position.vx,
                   player_state.camera_position.vz, 800, -1);
    value = func_80029624((u16 *)&player_state.unknown_5a,
                           &player_state.unknown_5c, 64, 0xc00);
    if (value != -1) {
        func_8002bf38(10, 10, 10, 0xef9, value);
    }
    value = func_80029624((u16 *)&player_state.unknown_66,
                           (u16 *)&player_state.unknown_68, 64, 0xe10);
    if (value != -1) {
        func_8002bf38(220, 220, 160, 18000, value);
    }

    if (player_state.unknown_13c != 0) {
        player_state.unknown_13c -= player_state.unknown_13e;
        if (player_state.unknown_13c < 0) {
            player_state.unknown_13c = 0;
        }
        value = player_state.unknown_13c;
        if (value > 4095) {
            value = 4096;
        }
        func_80031634(60, 0, 0, value);
    }

    player_state.flags_140.low = PadRead(1);
    if (player_state.flags_140.low & 0x800) {
        cd_report_error(3);
    }
    if (player_state.flags_140.low & 0x100) {
        player_state.flags_140.low = 0x40;
    }

    player_state.movement_step_limit = 200;
    player_state.turn_step_limit = 28;
    if ((player_state.flags_140.low & 0x5000) == 0) {
        player_state.turn_step_limit = 35;
    }
    if (player_state.unknown_5e != 0) {
        if (player_state.unknown_5e < 64) {
            player_state.unknown_0e += 100;
            if ((s16)player_state.unknown_0e > 0) {
                player_state.unknown_0e = 0;
            }
        } else {
            player_state.unknown_0e -= 100;
            if ((s16)player_state.unknown_0e < -3300) {
                player_state.unknown_0e = -3300;
            }
        }
        player_state.turn_step_limit >>= 1;
        player_state.unknown_5e--;
    } else {
        if (player_state.unknown_0c[1] == 0) {
            player_state.unknown_0e += 800;
            if ((s16)player_state.unknown_0e > 2800) {
                player_state.unknown_0e = 2800;
            }
        } else {
            player_state.unknown_0e -= 800;
            if ((s16)player_state.unknown_0e < 0) {
                player_state.unknown_0e = 0;
            }
        }
    }
    player_state.movement_step_limit +=
        ((s16)player_state.unknown_0e * player_state.movement_step_limit) >> 12;
    if (player_state.unknown_60 != 0) {
        player_state.movement_step_limit = 0;
        player_state.turn_step_limit = 0;
        player_state.unknown_60--;
        if ((player_state.unknown_60 & 3) == 0) {
            func_80031634(60, 30, 0, 0xc00);
        }
    }
    if (player_state.unknown_a0 != 0) {
        player_state.movement_step_limit >>= 1;
        player_state.turn_step_limit >>= 1;
    }

    switch (player_state.death_state) {
    case 0:
        func_80028998();
        player_update_camera_rotation();
        player_update_horizontal_motion();
        goto update_reaction_pose;
    case 1:
        object_index = player_state.reaction.view.mode;
        object = &map_object_state.objects[object_index];
        func_80028998();
        player_update_camera_rotation();
        player_state.unknown_e8 = (s16)object->position.vx
                                - (s16)player_state.camera_position.vx;
        player_state.unknown_ea = (s16)object->position.vy
                                - (s16)player_state.camera_position.vy;
        player_state.unknown_ec = (s16)object->position.vz
                                - (s16)player_state.camera_position.vz;
        player_state.camera_position = object->position;
        player_state.unknown_108.vector = object->rotation;
update_reaction_view:
        func_80029014();
        goto after_reaction;
    case 2:
        object_index = player_state.reaction.view.mode;
        object = &map_object_state.objects[object_index];
        func_80028998();
        fraction = player_state.reaction.view.step << 7;
        player_state.camera_position.vx = func_8001584c(
            player_state.camera_position.vx, object->position.vx, fraction);
        player_state.camera_position.vy = func_8001584c(
            player_state.camera_position.vy, object->position.vy, fraction);
        player_state.camera_position.vz = func_8001584c(
            player_state.camera_position.vz, object->position.vz, fraction);
        player_state.unknown_108.components[0] = func_8001586c(0, object->rotation.vx, fraction);
        player_state.unknown_108.components[1] = func_8001586c(0, object->rotation.vy, fraction);
        player_state.unknown_108.components[2] = func_8001586c(0, object->rotation.vz, fraction);
        player_state.camera_rotation_target.angles[0] = func_8001586c(
            player_state.reaction.view.rotation.angles[0], 0, fraction);
        player_state.camera_rotation_target.angles[1] = func_8001586c(
            player_state.reaction.view.rotation.angles[1], 0, fraction);
        player_state.camera_rotation_target.angles[2] = func_8001586c(
            player_state.reaction.view.rotation.angles[2], 0, fraction);
        step = player_state.reaction.view.step++;
        if (step > 31) {
            func_800291d0(player_state.reaction.view.mode);
        }
        goto after_reaction;
    case 5:
        ++player_state.reaction.position.mode;
        fraction = player_state.reaction.position.mode << 8;
        player_state.camera_position.vx = func_8001584c(
            player_state.camera_position.vx,
            player_state.reaction.position.position.vx, fraction);
        player_state.camera_position.vy = func_8001584c(
            player_state.camera_position.vy,
            player_state.reaction.position.position.vy, fraction);
        player_state.camera_position.vz = func_8001584c(
            player_state.camera_position.vz,
            player_state.reaction.position.position.vz, fraction);
        if (player_state.reaction.position.mode > 15) {
            func_80029168();
        }
        goto after_reaction;
    case 3:
        if (player_move_reaction_with_collision() != 0) {
            func_80029168();
        }
        goto update_reaction_view;
    case 4: {
        u16 angle_phase;

        func_80028998();
        player_update_camera_rotation();
        player_update_horizontal_motion();
        player_state.unknown_134 = 0;
        player_update_vertical_motion();
        player_state.unknown_134 += -256
                                   + (s16)(rcos((s16)player_state.reaction.angle_phase) >> 4);
        player_state.unknown_108.components[2] = rsin((s16)player_state.reaction.angle_phase) >> 6;
        angle_phase = (player_state.reaction.angle_phase + 128) & 0xfff;
        player_state.reaction.angle_phase = angle_phase;
        if (angle_phase == 0) {
            player_state.unknown_108.components[2] = 0;
            func_80029168();
        }
        goto update_reaction_view;
    }
    case 16:
        func_80028998();
        player_update_camera_rotation();
        player_update_horizontal_motion();
        player_state.reaction.damage.rotation.vy = 1;
        player_move_reaction_with_collision();
        func_80028ec0();
        if (player_state.unknown_100[0] == 0
            && player_state.unknown_100[1] == 0
            && player_state.unknown_100[2] == 0) {
            func_80029168();
        }
update_reaction_pose:
        player_update_vertical_motion();
        goto update_reaction_view;
    case 18:
        func_80028ec0();
        if (player_state.unknown_100[0] == 0
            && player_state.unknown_100[1] == 0
            && player_state.unknown_100[2] == 0) {
            func_80029168();
        }
        goto update_reaction_view;
    case 17:
        player_state.vitals.current_hp = 0;
        value = angle_velocity_step(-1024, player_state.unknown_100[0],
                                    player_state.reaction.damage.motion.vx, 8, 4);
        player_state.reaction.damage.motion.vx = value;
        player_state.unknown_100[0] += (value * 3) >> 1;
        value = player_state.unknown_134;
        player_state.unknown_134 = value < 1500 ? value + 500 : 1500;
        player_move_reaction_with_collision();
        player_state.unknown_106++;
        if (player_state.unknown_106 == 31
            && player_state.unknown_120 > -1001
            && player_state.unknown_124 >= 0
            && player_state.unknown_d1[4] == 0
            && (player_state.equipped_accessory_id == 54
                || player_state.equipped_extra_id == 54)
            && game_counter_bytes[0x53] != 0) {
            game_counter_bytes[0x53]--;
            func_80036e24(1, 0, 4096, 512);
            player_reset_status();
            func_80036e24(1, 4096, 0, -512);
        }
        if (player_state.unknown_106 > 31) {
            if (player_state.unknown_106 < 65) {
                s32 shade = func_8001584c(0, 255,
                                            (player_state.unknown_106 - 32) * 128);
                func_800314d4(0x82, shade, shade, shade);
            } else {
                KfEffectRecord *effect = effect_state.records;
                for (index = KF_EFFECT_CAPACITY; index != 0; index--) {
                    effect->type = 0xff;
                    effect++;
                }
                if (game_counter_bytes[0x4c] != 0
                    && (event_state.control.bytes[1] & 8) != 0) {
                    func_80038f20();
                    player_state.camera_rotation_target.angles[0] = 0;
                    player_state.camera_rotation_target.angles[1] = 0xc00;
                    player_state.camera_rotation_target.angles[2] = 0;
                    player_state.camera_position.vy = -0x2480;
                    player_state.camera_position.vx = 0x1e000;
                    player_state.camera_position.vz = 0x22000;
                    player_state.unknown_128 = 5;
                    game_counter_bytes[0x4c]--;
                    func_80048554(state_8017d118.values_04[0]);
                    player_reset_status();
                    func_8002360c(1, 1, 1, 1, 1, 0x43);
                } else {
                    player_initialize_state();
                    func_800482f8();
                    func_8002360c(0, 0, 0, 0, 0, 255);
                }
            }
        }
        goto after_reaction;
    default:
        goto after_reaction;
    }

after_reaction:
    player_state.flags_140.halves.high = player_state.flags_140.low;
    if (player_state.unknown_54 != 0) {
        if (player_state.unknown_54 % 30 == 0) {
            player_state.unknown_13c = 2400;
            player_state.unknown_13e = 60;
            player_adjust_hp_unclamped(-1);
        }
        player_state.unknown_54--;
    }
    if (player_state.unknown_62 != 0 && --player_state.unknown_62 == 0) {
        player_recalculate_combat_stats();
    }
    if (player_state.unknown_64 != 0 && --player_state.unknown_64 == 0) {
        player_recalculate_combat_stats();
    }
    if (player_state.unknown_6c != 0) {
        player_state.unknown_6c--;
        player_state.vitals.current_mp = player_state.vitals.maximum_mp;
        if (player_state.unknown_6c == 0) {
            notify_enqueue(34);
        }
    }
    if (player_state.unknown_6e != 0 && --player_state.unknown_6e == 0) {
        player_recalculate_combat_stats();
        notify_enqueue(34);
    }
    if (player_state.unknown_6a != 0) {
        if (player_state.unknown_6a == 1) {
            MoveImage(&player_status_texture_rows[0], 0x240, 0x103);
            MoveImage(&player_status_texture_rows[2], 0x240, 0x106);
            notify_enqueue(34);
            player_state.unknown_6a = 0;
            func_800357a0(0);
        } else {
            if ((player_state.unknown_6a & 7) == 0) {
                MoveImage(&player_status_texture_rows[1], 0x240, 0x103);
            }
            if ((player_state.unknown_6a & 7) == 4) {
                MoveImage(&player_status_texture_rows[3], 0x240, 0x106);
            }
            if ((player_state.unknown_6a & 7) == 1) {
                func_800357a0(1);
            }
            player_state.unknown_6a--;
        }
    }
    value = func_80029624((u16 *)&player_state.curse_strength,
                           &player_state.unknown_58, 64, 0xc00);
    if (value != -1) {
        if (player_state.curse_strength == 0) {
            player_recalculate_combat_stats();
        }
        func_80031634(0x46, 0x46, 30, value);
    }

    player_state.camera_rotation.angles[0] =
        player_state.camera_rotation_target.angles[0]
        + player_state.unknown_100[0] + player_state.unknown_108.components[0]
        + player_state.unknown_110[0];
    player_state.camera_rotation.angles[1] =
        player_state.camera_rotation_target.angles[1]
        + player_state.unknown_100[1] + player_state.unknown_108.components[1]
        + player_state.unknown_110[1];
    player_state.camera_rotation.angles[2] =
        player_state.camera_rotation_target.angles[2]
        + player_state.unknown_100[2] + player_state.unknown_108.components[2]
        + player_state.unknown_110[2];
    func_8002b73c(player_state.camera_position.vx,
                   player_state.camera_position.vz, 800, 1);
    actor_state.unknown_93a0 = 0;
    index = func_8003a9f4(player_state.camera_position.vx,
                           player_state.camera_position.vy,
                           player_state.camera_position.vz, 1, 1700);
    if (index != -1 && (actor_state.actors[index].unknown_28 & 8) != 0) {
        func_800295f8();
    }
    func_8002665c();
    if (player_state.equipped_weapon_id != 0xff) {
        KfWeaponRecordGame *weapon = player_state.equipped_weapon_record;
        if (weapon->unknown_16 != 0
            && player_state.equipment_effect_ticks % weapon->unknown_16 == 0) {
            player_adjust_hp(1);
        }
        weapon = player_state.equipped_weapon_record;
        if (weapon->unknown_18 != 0
            && player_state.equipment_effect_ticks % weapon->unknown_18 == 0) {
            player_adjust_mp(1);
        }
    }
    if (player_state.equipped_head_id != 0xff) {
        func_800297b4(player_state.equipped_head_record);
    }
    if (player_state.equipped_body_id != 0xff) {
        func_800297b4(player_state.equipped_body_record);
    }
    if (player_state.equipped_arm_id != 0xff) {
        func_800297b4(player_state.equipped_arm_record);
    }
    if (player_state.equipped_leg_id != 0xff) {
        func_800297b4(player_state.equipped_leg_record);
    }
    if (player_state.equipped_shield_id != 0xff) {
        func_800297b4(player_state.equipped_shield_record);
    }
    if (player_state.equipped_accessory_id != 0xff) {
        func_800297b4(player_state.equipped_accessory_record);
    }
    if (player_state.equipped_extra_id != 0xff) {
        func_800297b4(player_state.equipped_extra_record);
    }
    if (player_state.equipped_head_id == 25 && rand() < 36) {
        player_state.unknown_118.vx = 0;
        player_state.unknown_118.vy = -300;
        player_state.unknown_118.vz = 400;
        func_80025a18(11);
    }
    if (player_state.equipped_body_id == 31) {
        func_8002bf38(20, 20, 20, 5000, 0x800);
    }
    player_state.equipment_effect_ticks++;
}
