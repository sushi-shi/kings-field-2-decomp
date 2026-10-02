#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>
#include <kf/game/asset.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/map_cell.h>
#include <kf/game/player.h>
#include <psyq/libc.h>
#include <stdarg.h>


DATA(0x800667a0, 0x28)
SVECTOR DAT_800667a0[5] = {
    {0, 0, 100, 0},
    {-1000, 0, 0, 46},
    {-2000, 0, -2000, 46},
    {1000, 0, 0, 46},
    {2000, 0, -2000, 46}
};

DATA(0x800667c8, 0x20)
SVECTOR DAT_800667c8[4] = {
    {-400, -400, 200, 0},
    {-400, 400, 200, 0},
    {400, -400, 200, 0},
    {400, 400, 200, 0}
};

DATA(0x800667e8, 0x14)
KfPlayerMagicIdSequence DAT_800667e8 = {
    {39, 40, 60, 66, 84, 86, 39, 40, 60, 66, 84, 86},
    {0x20, 0x10, 0x80, 0xffff}
};

enum { PLAYER_CAMERA_HEIGHT_OFFSET = 1600 };

ADDRESS(0x80024ed4, 0x78)
void player_get_camera_pose(VECTOR *position, SVECTOR *angles)
{
    position->vx = player_state.camera_position.vx;
    position->vz = player_state.camera_position.vz;
    position->vy = player_state.unknown_134 + player_state.camera_position.vy
                 + player_state.unknown_138 - PLAYER_CAMERA_HEIGHT_OFFSET;
    angles->vx = player_state.camera_rotation.angles[0];
    angles->vy = player_state.camera_rotation.angles[1];
    angles->vz = player_state.camera_rotation.angles[2];
}

ADDRESS(0x80024f4c, 0xb8)
void player_reset_status(void)
{
    player_state.unknown_6e = 0;
    player_state.unknown_6c = 0;
    player_state.unknown_6a = 0;
    player_state.unknown_68 = 0;
    player_state.unknown_66 = 0;
    player_state.unknown_64 = 0;
    player_state.unknown_62 = 0;
    player_state.unknown_60 = 0;
    player_state.unknown_5e = 0;
    player_state.unknown_5c = 0;
    player_state.unknown_5a = 0;
    player_state.unknown_58 = 0;
    player_state.curse_strength = 0;
    player_state.unknown_54 = 0;
    player_state.unknown_ce[7] = 0;
    player_state.vitals.current_hp = player_state.vitals.maximum_hp;
    player_state.vitals.current_mp = player_state.vitals.maximum_mp;
    player_reset_view();
}

DATA(0x80075da0, 0xc000)
static KfWeaponAssetBuffer player_weapon_asset_buffer;

DATA(0x801c7078, 0x4c8)
KfWeaponRecordGame player_weapon_records[18];

DATA(0x801c7540, 0x11844)
KfBss801c7540 bss_801c7540;

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
    player_state.unknown_13c = 0;
    player_state.unknown_13e = 0;
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
    player_set_unknown_97(KF_EQUIPMENT_NONE);
    player_equip_weapon(0);
    player_set_unknown_98(KF_EQUIPMENT_NONE);
    player_set_unknown_99(KF_EQUIPMENT_NONE);
    player_state.camera_rotation_target.angles[0] = 0;
    player_state.camera_rotation_target.angles[1] = PLAYER_INITIAL_CAMERA_PITCH;
    player_state.camera_rotation_target.angles[2] = 0;
    player_state.camera_position.vy = PLAYER_INITIAL_CAMERA_Y;
    player_state.camera_position.vx = PLAYER_INITIAL_CAMERA_X;
    player_state.camera_position.vz = PLAYER_INITIAL_CAMERA_Z;
    player_state.unknown_128 = PLAYER_INITIAL_MAP_LAYER;
    entry = effect_state.magic_records;
    for (i = 63; i != -1; i--) {
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
    player_state.unknown_c9[0] = 1;
    player_state.unknown_c9[1] = 1;
    player_state.unknown_c9[2] = 1;
    player_state.unknown_c9[3] = 1;
    player_state.unknown_09[1] = 0;
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

enum {
    PLAYER_MAP_PROBE_RADIUS = 800,
    PLAYER_MAP_PROBE_HEIGHT = 1700
};

ADDRESS(0x80025234, 0xb0)
void player_sync_position_to_map(void)
{
    s32 layer;

    player_state.equipment_effect_ticks = 0;
    layer = 2;
    if (player_state.unknown_128 == 0) {
        layer = 1;
    }
    player_state.camera_position.vy =
        func_8002b67c(layer, player_state.camera_position.vx,
                      player_state.camera_position.vz,
                      PLAYER_MAP_PROBE_RADIUS, PLAYER_MAP_PROBE_HEIGHT);
    func_80023384();
    player_state.unknown_09[1] = 1;
    player_state.unknown_ce[2] = 0;
    player_state.unknown_13a = 0;
    player_state.death_state = 0;
    player_clear_motion();
    func_8002b73c(player_state.camera_position.vx,
                  player_state.camera_position.vz, PLAYER_MAP_PROBE_RADIUS, 1);
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

RODATA(0x80011168, 0xf4)

ADDRESS(0x800253fc, 0x10)
void player_set_unknown_97(u8 value)
{
    player_state.unknown_97 = value;
}

ADDRESS(0x8002540c, 0x28)
void player_set_unknown_98(u8 value)
{
    player_state.unknown_98 = value;
    if (value != KF_EQUIPMENT_NONE) {
        player_state.unknown_99 = KF_EQUIPMENT_NONE;
    }
}

ADDRESS(0x80025434, 0x28)
void player_set_unknown_99(u8 value)
{
    player_state.unknown_99 = value;
    if (value != KF_EQUIPMENT_NONE) {
        player_state.unknown_98 = KF_EQUIPMENT_NONE;
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
    PLAYER_WEAPON_ARCHIVE_FIRST_ENTRY = 49,
    PLAYER_WEAPON_ASSET_INDEX = 32,
    PLAYER_WEAPON_ATTACK_INACTIVE = -1
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
        asset_registry_set(PLAYER_WEAPON_ASSET_INDEX,
                           player_state.weapon_asset_buffer);
    }
    player_state.weapon_attack_phase = PLAYER_WEAPON_ATTACK_INACTIVE;
    player_state.weapon_animation_cache = NULL;
    player_state.weapon_magic_shots_remaining = 0;
    player_state.unknown_a0 = 0;
    player_recalculate_combat_stats();
}

enum {
    PLAYER_CHARGE_FULL = 5000
};

ADDRESS(0x80025754, 0x124)
void player_begin_weapon_attack(s32 mode)
{
    if (player_state.weapon_attack_phase != PLAYER_WEAPON_ATTACK_INACTIVE
        || player_state.equipped_weapon_id == KF_EQUIPMENT_NONE
        || player_state.unknown_60 != 0) {
        return;
    }

    player_state.weapon_attack_mode = mode;
    player_state.weapon_attack_phase = 0;
    if (mode == 0) {
        player_state.weapon_attack_window = player_state.equipped_weapon_record->unknown_1e;
        player_state.weapon_attack_recovery = player_state.equipped_weapon_record->unknown_2c;
    } else {
        player_state.weapon_attack_window = player_state.equipped_weapon_record->unknown_26;
        player_state.weapon_attack_recovery = player_state.equipped_weapon_record->unknown_2e;
    }
    player_state.attack_charge_committed = player_state.attack_charge_current;
    if (player_state.attack_charge_current == PLAYER_CHARGE_FULL
        && player_state.magic_charge == PLAYER_CHARGE_FULL) {
        player_state.weapon_attack_fully_charged = 1;
        player_state.weapon_magic_shots_configured = player_state.equipped_weapon_record->magic_shots;
    } else {
        player_state.weapon_attack_fully_charged = 0;
    }
    player_state.attack_charge_current = 0;
    player_state.unknown_9c[0] = 0;
}

extern KfActor *func_8003a778(const VECTOR *position, s16 yaw, s16 pitch,
                               s32 max_distance, s32 filter_a, s32 filter_b,
                               s32 *distance, s32 flags);

ADDRESS(0x80025878, 0x1a0)
KfActor *func_80025878(s32 scale, VECTOR *position, SVECTOR *direction,
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
        vector_rotate_yxz(&angles, &player_state.unknown_118, position);
        position->vx += player_state.camera_position.vx;
        height = position->vy - 1600;
        position->vy = height + player_state.camera_position.vy;
        position->vz += player_state.camera_position.vz;
    }

    actor = func_8003a778(&player_state.camera_position,
                          (s16)player_state.camera_rotation.angles[1],
                          (s16)player_state.camera_rotation.angles[0], 0x55f0,
                          0x200, 0x200, distance, 0);
    actor_state.actor_93c8 = actor;
    if (actor != 0) {
        target = actor_find_target_of_type(&actor_state.target_groups[actor->group_index],
                                           0x82);
        if (target != 0 && !(actor->unknown_28 & 0x100)) {
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
void func_80025a18(s32 effect_id, ...)
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
        func_80025878(simple_scale, &position, &direction, &distance);
emit_simple_effect:
        func_80040308(10, 0x12, effect_id, &position, &direction);
        break;
    case 2:
        func_80040308(10, 0x13, effect_id, &player_state.camera_position,
                       0, 0x1000, 0x100, 0x1000);
        break;
    case 3:
        actor = func_80025878(5000, &position, &direction, &distance);
        if (actor == 0) {
            position.vx += direction.vx;
            position.vy = player_state.camera_position.vy;
            case3_z = position.vz + direction.vz;
            goto case3_store_z;
        } else {
            position.vx = ((s32)actor->unknown_50 << 14) / 600 + actor->position.vx;
            position.vy = ((s32)actor->unknown_52 << 14) / 600 + actor->position.vy;
            position.vz = ((s32)actor->unknown_54 << 14) / 600 + actor->position.vz;
            if (func_8002b7f8(position.vx, position.vy, position.vz, 10, 10)) {
                position.vx = actor->position.vx;
                position.vy = actor->position.vy;
                case3_z = actor->position.vz;
                goto case3_store_z;
            }
        }
        goto case3_emit;
case3_store_z:
        position.vz = case3_z;
case3_emit:
        func_80040308(10, 0x12, 0x72, &position, 0);
        break;
    case 0:
        actor = func_80025878(5000, &position, &direction, &distance);
        if (actor == 0) {
            kind = 255;
        } else {
            kind = actor - actor_state.actors;
        }
        position.vx += direction.vx;
        position.vz += direction.vz;
        func_80040308(10, 0x12, 0x6f, &position, 0, kind);
        break;
    case 13: {
        const SVECTOR *sequence = DAT_800667c8;
        for (i = 3; i != -1; i--) {
            player_state.unknown_118 = *sequence;
            func_80025878(800, &position, &direction, &distance);
            sequence++;
            direction.vx += -32 + (rand() >> 9);
            direction.vy += -32 + (rand() >> 9);
            direction.vz += -32 + (rand() >> 9);
            func_80040308(10, 0x12, effect_id, &position, &direction);
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
        func_80025878(rotation_scale, &position, &direction, &distance);
        func_80040308(10, 0x12, effect_id, &position, &direction,
                       &player_state.camera_rotation);
        break;
    case 11:
        func_80025878(600, &position, &direction, &adjusted_distance);
        if (adjusted_distance != -1) {
            adjusted_distance = adjusted_distance / 600 - 8;
            if (adjusted_distance < 2) {
                adjusted_distance = 2;
            }
        } else {
            adjusted_distance = 10;
        }
        direction.vy = 0;
        func_80040308(10, 0x12, 0x67, &position, &direction, adjusted_distance);
        break;
    case 5:
        target_scale = 200;
select_actor_effect:
        actor = func_80025878(target_scale, &position, &direction, &distance);
        if (actor == 0) {
            kind = 255;
        } else {
            kind = actor - actor_state.actors;
        }
        func_80040308(10, 0x12, effect_id, &position, &direction, kind);
        break;
    case 9:
        target_scale = 500;
        goto select_actor_effect;
    case 8:
        func_80025878(700, &position, &direction, &distance);
        func_80040308(10, 0x12, 0x6a, &position, &direction,
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
            (u16)player_state.unknown_118.vx * 2;
        func_80025878(150, &position, &direction, &distance);
        player_state.unknown_118.vx += 100;
        player_state.camera_rotation.angles[1] = old_yaw;
        func_80040308(10, 0x12, effect_id, &position, &direction,
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
        const SVECTOR *record = DAT_800667a0;
        player_state.unknown_118 = *record;
        func_80025878(600, &position, &direction, &distance);
        effect = func_80040308(10, 0x12, effect_id,
                               &position, &direction, &player_state.camera_rotation);
        if (effect != 0) {
            s32 index = effect - effect_state.records;
            record++;
            for (i = 3; i != -1; i--) {
                player_state.unknown_118 = *record;
                func_80025878(600, &position, &direction, &distance);
                func_80040308(10, 0x12, record->pad, &position, &direction, index);
                record++;
            }
        }
        break;
    }
    case 44:
        func_80025878(1000, &position, &direction, &distance);
        effect_id = 0x75;
        goto emit_rotation_effect;
    case 45:
        func_80025878(1000, &position, &direction, &distance);
        effect_id = 0x74;
        goto emit_simple_effect;
    case 40:
        rotation_scale = 1000;
        goto probe_rotation_effect;
    case 39:
        func_80025878(50, 0, &direction, &distance);
        override_position = va_arg(arguments, const VECTOR *);
        position = *override_position;
        goto emit_rotation_effect;
    case 49:
        /* Cases 49 and 50 omit the rotation argument. */
        func_80025878(550, 0, &direction, &distance);
        override_position = va_arg(arguments, const VECTOR *);
        position = *override_position;
        goto emit_simple_effect;
    case 50:
        override_position = va_arg(arguments, const VECTOR *);
        position = *override_position;
        goto emit_simple_effect;
    case 34:
    case 35:
    case 38:
        func_80025878(900, &position, &direction, &distance);
        goto emit_rotation_effect;
emit_rotation_effect:
        func_80040308(10, 0x12, effect_id, &position, &direction,
                       &player_state.camera_rotation);
        break;
    case 15:
        func_80040308(10, 0x10, 15, &player_state.camera_position,
                       &direction);
        player_state.unknown_62 = 900;
        player_recalculate_combat_stats();
        break;
    case 17:
        func_80040308(10, 0x10, 17, &player_state.camera_position,
                       &direction);
        player_state.unknown_64 = 900;
        player_recalculate_combat_stats();
        break;
    case 14:
        func_80040308(10, 0x10, 14, &player_state.camera_position,
                       &direction);
        break;
    case 16:
        func_80040308(10, 0x10, 16, &player_state.camera_position,
                       &direction);
        break;
    case 19:
        func_80040308(10, 0x10, 19, &player_state.camera_position,
                       &direction);
        break;
    default:
        break;
    }
    va_end(arguments);
}
