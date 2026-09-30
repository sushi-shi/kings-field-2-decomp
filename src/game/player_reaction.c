#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>

s32 func_8001876c(void);
s32 func_8002897c(s32 value);
void func_80028fa8(void);
void func_800335a0(s32 mode, s32 argument);
void func_8004678c(const VECTOR *position, const KfPlayerViewRotation *rotation, s32 value);
void func_80047c98(const VECTOR *position, const KfPlayerViewRotation *rotation);
void func_8002360c(s32 first, s32 second, s32 third, s32 fourth, s32 fifth, s32 sixth);
void func_800291ec(KfMapObject *object);

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
    player_state.unknown_108[2] = 0;
    player_state.unknown_108[1] = 0;
    player_state.unknown_108[0] = 0;
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
    player_state.camera_rotation_target.angles[0] += player_state.unknown_108[0];
    player_state.camera_rotation_target.angles[1] += player_state.unknown_108[1];
    player_state.camera_rotation_target.angles[2] += player_state.unknown_108[2];
    player_state.unknown_108[0] = 0;
    player_state.unknown_108[1] = 0;
    player_state.unknown_108[2] = 0;

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
    s32 result;

    if (secondary != NULL) {
        other = *secondary;
    }
    if (current == 0 && other == 0) {
        return -1;
    }
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
