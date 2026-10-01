#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/asset.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

extern s32 func_80042298(s32 radius, s32 angle, s32 step);
extern void func_8003feb0(s32 kind);
extern s32 func_80041e0c(const VECTOR *position, s32 arg1,
                         s32 arg2, s32 vertical_window);
extern void func_80041cd0(s32 multiplier, s32 limit, s32 increment,
                          s32 arg3, s32 arg5);
extern void func_8003ff18(VECTOR *position, s32 start, s32 end,
                          s32 arg3, s32 arg4, s32 arg5);
extern s32 func_8004212c(const VECTOR *origin, s32 count, s32 spread,
                         s32 scale_x, s32 scale_z, s32 variation);
extern void func_80042424(void);
extern void func_8002bf38(u8 first, u8 second, u8 third,
                         s32 angle, u16 value);
extern s32 func_80041b14(const VECTOR *target, s32 max_length, s32 scale,
                         s32 settle_distance, s32 min_distance,
                         s32 probe_radius, s32 probe_angle);
extern s32 func_8002b7f8(s32 x, s32 y, s32 z, s32 radius, s32 height);
extern s32 func_8004195c(s32 max_length, s32 scale, s32 turn_step,
                         s32 probe_radius, s32 probe_angle, s32 proximity,
                         s32 close_scale, s32 target_filter);
extern s32 func_8003a9f4(s32 x, s32 y, s32 z, s32 radius, s32 height);
extern void func_800424f0(s32 count, s32 radius, s32 vertical_angle,
                          s32 arg3);
extern void func_80026330(s32 mode, VECTOR *output);
extern KfActor *func_80025878(s32 scale, VECTOR *position,
                              SVECTOR *direction, s32 *distance);
extern s32 func_8002b9d4(s32 x, s32 y, s32 z, s32 radius,
                          s32 height, s32 mode);
extern s32 func_8002b604(s32 x, s32 y, s32 z, s32 radius, s32 height);
extern s32 func_8001584c(s32 start, s32 end, s32 fraction);
extern void func_80039c94(s32 actor_index, u16 power, u16 magic_06,
                          u16 magic_08, u16 magic_0a, u16 magic_0c,
                          u16 magic_0e, u16 magic_10, u16 magic_12,
                          u16 magic_14, s32 amount, s32 effect_flags,
                          const VECTOR *position);
extern s32 func_8003c3e0(KfActor *actor, const VECTOR *origin, s32 step,
                         const VECTOR *target, SVECTOR *direction,
                         s32 pitch_override, u16 yaw_limit, s32 iterations);

ADDRESS(0x80042650, 0x3670)
void effect_update_dispatch(void)
{
    KfEffectRecord *record = effect_state.current_record;
    s32 collision;
    s32 step;

    switch (record->kind) {
    case 0:
        record->direction.vz += 20;
        if ((s16)record->scale_x < 0xc00) {
            step = (s16)record->scale_z + 0x100;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
        }
        collision = func_80042298(180, 0, -300);
        if (collision != 0) {
            if (record->unknown_3c[4] == 0 && rand() < 3000) {
                func_8003feb0(collision);
            }
            if (collision & 0x10) {
                record->unknown_3c[4] = 1;
            }
            /* The vertical collision-cache word read on this branch has no
             * proved owner; its state update remains WIP. */
        }
        func_80041e0c(&record->position, 0x400, 0x400, 500);
        record->rotation.vz += 2700;
        break;
    case 1:
    case 28: {
        s32 radius = record->kind == 28 ? 250 : 500;
        u8 phase = record->unknown_3c[4];

        if (phase == 2) {
            record->unknown_10 += 256;
            if (record->unknown_10 >= 4096) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            break;
        }
        if (phase == 1) {
            record->direction.vy += 13;
        }
        record->rotation.vx += 200;
        collision = func_80042298(radius, radius * 2, 250);
        if (collision != 0) {
            func_8003feb0(collision);
            if (phase == 1) {
                record->unknown_3c[4] = 2;
                record->unknown_09 = 1;
                record->unknown_0c = 0x42;
                record->unknown_10 = 0x400;
                break;
            }
            func_80042424();
            record->unknown_3c[4] = 1;
            record->direction.vz = 0;
            record->direction.vx = 0;
            record->direction.vy = -100;
        }
        func_80041e0c(&record->position, 0x4000, 0x4000, 500);
        break;
    }
    case 2:
        step = (s16)record->scale_x + (s16)*(u16 *)&record->unknown_3c[6];
        record->scale_x = step;
        record->scale_z = step;
        if ((s16)record->scale_x >= 0x300) {
            func_8003ff18(&record->position,
                           ((s16)record->scale_x -
                            (s16)*(u16 *)&record->unknown_3c[6]) * 4,
                           (s16)record->scale_x * 4 - 1,
                           2000, 0x1000,
                           (s16)*(u16 *)&record->unknown_3c[8]);
        }
        if ((s16)record->scale_x > (s16)*(u16 *)&record->unknown_3c[4]) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 3:
        func_80041cd0(0x4000, 0x200, 0x40, 0xc00, 0x8000);
        break;
    case 4:
        collision = func_80042298(180, 0, 0);
        if (collision == 0) {
            record->unknown_3c[4] = 0;
        } else {
            if (collision & 0xf) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            if (record->unknown_3c[4] == 0) {
                record->unknown_3c[4] = 1;
                func_8003feb0(collision);
            }
        }
        record->rotation.vz += 750;
        func_80041e0c(&record->position, 0x2000, 0x2000, 500);
        break;
    case 5: {
        s32 count;
        s32 step_size;
        s32 progress;
        s32 matches;
        s32 index;
        u8 actor_index = record->unknown_3c[5];

        if (record->phase == 0) {
            progress = 0;
            if (actor_index == 0xff) {
                count = 16;
                step_size = 0;
                record->updates_remaining = 1;
            } else {
                const KfActor *actor = &actor_state.actors[actor_index];

                count = asset_vertex_count(actor->unknown_01 + 128,
                                           actor->unknown_0c);
                if (count == 0) {
                    count = 16;
                    step_size = 0;
                    record->updates_remaining = 1;
                } else if (count < 33) {
                    step_size = 0x1000;
                } else {
                    step_size = (count << 12) / 32;
                    count = 32;
                }
            }
            *(u16 *)&record->unknown_3c[6] = count;
            record->unknown_3c[4] = count;
            for (index = 0; index < count; index++) {
                func_80040308(10, record->type, 105,
                               &record->position, &record->rotation,
                               effect_state.current_index, actor_index,
                               progress >> 12);
                progress += step_size;
            }
            record->phase = 1;
        } else if (record->phase == 1) {
            if (record->updates_remaining >= 2 &&
                *(s16 *)&record->unknown_3c[6] != 0) {
                break;
            }
            matches = 0;
            for (index = 0; index < KF_EFFECT_CAPACITY; index++) {
                KfEffectRecord *child = &effect_state.records[index];

                if (child->type == KF_EFFECT_SLOT_FREE ||
                    child->kind != 105 ||
                    child->unknown_3c[4] != effect_state.current_index) {
                    continue;
                }
                if (child->phase == 1) {
                    child->updates_remaining = 1;
                    matches++;
                } else {
                    child->phase = 2;
                }
            }
            if (record->unknown_3c[4] != 0) {
                KfMagicRecord *magic = effect_state.current_magic;

                step = effect_magic_power(record);
                func_80039c94(actor_index, step,
                              magic->unknown_06, magic->unknown_08,
                              magic->unknown_0a, magic->unknown_0c,
                              magic->unknown_0e, magic->unknown_10,
                              magic->unknown_12, magic->unknown_14,
                              (matches << 12) / record->unknown_3c[4],
                              (record->type & 0x30) | 2,
                              &actor_state.actors[actor_index].position);
            }
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    }
    case 23:
        if (record->phase == 9) {
            KfActor *actor = &actor_state.actors[*(s16 *)&record->unknown_3c[4]];
            VECTOR vertex_offset;
            VECTOR actor_position;
            VECTOR old_position = record->position;
            VECTOR *position;
            s32 scale;

            if (actor->lifecycle != 1 || actor->target_type != 25) {
                record->type = KF_EFFECT_SLOT_FREE;
                break;
            }
            func_8003c000(actor, *(s16 *)&record->unknown_3c[6],
                          &vertex_offset);
            position = func_8003c10c(actor, &actor_position);
            record->position.vx = vertex_offset.vx + position->vx;
            record->position.vy = vertex_offset.vy + position->vy;
            record->position.vz = vertex_offset.vz + position->vz;
            scale = (u16)record->scale_x + 512;
            if ((s16)scale > 0x1800) {
                scale = 0x1800;
            }
            record->scale_x = scale;
            record->scale_y = scale;
            record->scale_z = scale;
            record->direction.vx = (u16)record->position.vx - (u16)old_position.vx;
            record->direction.vy = (u16)record->position.vy - (u16)old_position.vy;
            record->direction.vz = (u16)record->position.vz - (u16)old_position.vz;
            func_80041e94(record, -1, -700, (s16)record->scale_x,
                           -300, 3, 8, 0);
            if (actor->animation_phase >= *(u16 *)&record->unknown_3c[8]) {
                func_8003c3e0(actor, (const VECTOR *)&player_state.unknown_e8,
                               650, &record->position, &record->direction,
                               -1, 0x400, 5);
                record->phase = 0;
                record->updates_remaining = 50;
            }
            break;
        }
        func_80041e94(record, -1, 0x100, (s16)record->scale_x,
                       -300, 2, 8, 0);
        /* Other phases share the collision and growth path with kinds 7/49. */
    case 7:
    case 49: {
        s32 prior_phase = record->phase;

        if (prior_phase == 0) {
            collision = func_80042298(180, (s32)0x80000000, -300);
            if (collision == 0) {
                break;
            }
            func_8004212c(&record->position, 3, 400, 0x2000, 0x2000, 0x400);
            func_80042424();
            func_8003feb0(collision);
            record->phase = 1;
        } else if (prior_phase >= 3) {
            record->type = KF_EFFECT_SLOT_FREE;
            break;
        }
        record->animation_clip = prior_phase - 128;
        step = record->scale_x + 2048;
        record->scale_x = step;
        record->scale_y = step;
        record->scale_z = step;
        record->phase++;
        break;
    }
    case 9: {
        u8 actor_index = record->unknown_3c[4];

        if (actor_index == 0xff) {
            record->direction.vy = (u16)record->direction.vy + 10;
            collision = func_80042298(10, (s32)0x80000000, 0);
        } else {
            VECTOR target;

            if (actor_index == 0xfe) {
                target.vx = player_state.camera_position.vx;
                target.vy = player_state.camera_position.vy - 1600;
                target.vz = player_state.camera_position.vz;
                collision = func_80041b14(&target, 400, 60,
                                           3000, 0, 10, (s32)0x80000000);
            } else {
                const KfActor *actor = &actor_state.actors[actor_index];

                target.vx = actor->position.vx;
                target.vy = actor->position.vy - (actor->unknown_1e >> 1);
                target.vz = actor->position.vz;
                collision = func_80041b14(&target, 600, 50,
                                           0, 0, 10, (s32)0x80000000);
            }
        }
        if ((actor_index == 0xff && collision != 0) ||
            (actor_index != 0xff && collision == -1)) {
            s32 index;

            func_8003feb0(KF_COLLISION_CACHE_FLAGS);
            for (index = 0; index < 12; index++) {
                func_80041e94(record, -1, -2, 0xc00, -90, 16, 8, 5,
                               0x200, -256, 0x200, -256, 0x200, -256);
            }
            func_8004212c(&record->position, 5, 0x400,
                           0x2000, 0x4000, 0x1000);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            func_80041e94(record, -1, -3, 0xed8, -80,
                           6, 8, 0, 0x400);
        }
        break;
    }
    case 11:
    case 54: {
        SVECTOR random_direction;

        func_80041cd0(0x3800, (s16)*(u16 *)&record->unknown_3c[4],
                       0x80, 0x400, 0x8000);
        random_direction.vx = (rand() >> 6) - 256;
        random_direction.vz = (rand() >> 6) - 256;
        random_direction.vy = -(rand() >> 7) - 128;
        func_80040308(10, 0, 101, &record->position, &random_direction,
                       0xc00, -128, 15, 18, 10);
        record->rotation.vz += 64;
        break;
    }
    case 12: {
        s32 prior_phase = record->phase;
        s32 reset = 0;

        if (prior_phase == 101) {
            record->phase++;
            break;
        }
        if (prior_phase == 100) {
            KfEffectRecord *child = func_80040308(
                10, record->type | 3, 12, &record->position,
                0, &record->rotation);

            child->phase = 101;
            reset = 1;
        } else if (prior_phase == 102) {
            reset = 1;
        } else if (prior_phase != 110) {
            if (prior_phase == 0) {
                effect_play_spatial_sound(record, 0x29);
            }
            record->phase++;
            collision = func_8004195c(
                *(s16 *)&record->unknown_3c[4],
                *(s16 *)&record->unknown_3c[6],
                *(s16 *)&record->unknown_3c[8],
                *(s16 *)&record->unknown_3c[10],
                0, 6000, *(s16 *)&record->unknown_3c[10], 0x800);
            if (collision == -1) {
                KfEffectRecord *child;

                effect_play_spatial_sound(record, 0x18);
                child = func_80040308(10, record->type | 3, 12,
                                      &record->position, 0,
                                      &record->rotation);
                child->phase = 101;
                reset = 1;
            } else {
                func_80041e0c(&record->position, 0x2000, 0x2000, 500);
                record->rotation.vz = (u16)record->rotation.vz + 128;
                func_80041e94(record, 2, 0x400, 0xc00, -300,
                               5, 33, 0);
            }
        }
        if (reset) {
            record->render_id = 0x11;
            record->scale_x = 0;
            record->scale_y = 0;
            record->scale_z = 0;
            record->phase = 110;
            record->type |= 3;
        }
        if (reset || prior_phase == 110) {
            func_80041cd0(0x3800, 0x31f, 0x46, 0x400, 0x8000);
            record->rotation.vy = (u16)record->rotation.vy + 64;
        }
        break;
    }
    case 13:
    case 32: {
        s32 prior_phase = record->phase;

        if (prior_phase == 0) {
            collision = func_80042298(180, 0, -300);
            if (collision == 0) {
                func_80041e0c(&record->position, 0x2000, 0x2000, 500);
                break;
            }
            func_80042424();
            func_8003feb0(collision);
            record->phase = 1;
        } else if (prior_phase >= 3) {
            record->type = KF_EFFECT_SLOT_FREE;
            break;
        }
        record->animation_clip = prior_phase - 128;
        step = record->scale_x + 2048;
        record->scale_x = step;
        record->scale_y = step;
        record->scale_z = step;
        record->phase++;
        break;
    }
    case 14: {
        VECTOR spawn_position;
        SVECTOR spawn_direction;
        KfEffectRecord *spawned;

        if (record->updates_remaining == 12) {
            player_state.unknown_54 = 0;
        }
        spawn_position.vx = (rand() >> 5) - 512;
        spawn_position.vy = (rand() >> 8) + 200;
        spawn_position.vz = 0x400;
        spawn_direction.vz = 0;
        spawn_direction.vx = 0;
        spawn_direction.vy = 0;
        spawned = func_80040308(10, 0, 101, &spawn_position,
                                &spawn_direction, 700, -30, 10, 14, -10);
        spawned->unknown_0a = 3;
        spawned->unknown_08 = 14;
        func_8002bf38(160, 180, 220, 18000,
                       rsin(record->updates_remaining << 7));
        break;
    }
    case 15:
        if (player_state.unknown_62 == 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        record->rotation.vy += 128;
        break;
    case 16:
        if (record->updates_remaining == 4) {
            player_state.vitals.current_hp += 60;
            if (player_state.vitals.current_hp >
                player_state.vitals.maximum_hp) {
                player_state.vitals.current_hp =
                    player_state.vitals.maximum_hp;
            }
        }
        func_8002bf38(0xe6, 0xc8, 0xa0, 0x59d8,
                       rsin(record->updates_remaining << 8));
        break;
    case 17:
        if (player_state.unknown_64 == 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        record->rotation.vy -= 128;
        break;
    case 19: {
        VECTOR spawn_position;
        SVECTOR spawn_direction;
        KfEffectRecord *spawned;
        s32 count;

        if (record->updates_remaining == 12) {
            player_state.vitals.current_hp += 150;
            if (player_state.vitals.current_hp >
                player_state.vitals.maximum_hp) {
                player_state.vitals.current_hp =
                    player_state.vitals.maximum_hp;
            }
            player_cap_status_components(7);
        }
        spawn_direction.vz = 0;
        spawn_direction.vx = 0;
        spawn_direction.vy = 0;
        for (count = 3; count != 0; count--) {
            spawn_position.vx = (rand() >> 5) - 512;
            spawn_position.vy = (rand() >> 8) + 200;
            spawn_position.vz = 0x400;
            spawned = func_80040308(10, 0, 101, &spawn_position,
                                    &spawn_direction, 700, -30, 10, 18, -10);
            spawned->unknown_0a = 3;
            spawned->unknown_08 = 14;
        }
        func_8002bf38(240, 240, 160, 18000,
                       rsin(record->updates_remaining << 7));
        break;
    }
    case 20:
        func_80041cd0(0x4000, 0x100, 0x20, 0x400, 0x8000);
        record->rotation.vz += 64;
        break;
    case 22:
        record->rotation.vy = (u16)record->rotation.vy + 10;
        collision = func_80042298(100, (s32)0x80000000, -300);
        if (collision != 0) {
            func_8003feb0(collision);
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 24: {
        VECTOR target;
        s32 result;
        s32 count;

        target.vx = player_state.camera_position.vx;
        target.vy = player_state.camera_position.vy - 1600;
        target.vz = player_state.camera_position.vz;
        result = func_80041b14(&target, 300, 40, 2000, 0, 10, 0);
        if (result == -1) {
            func_8003feb0(KF_COLLISION_CACHE_FLAGS);
            for (count = 11; count >= 0; count--) {
                func_80041e94(record, -1, -2, 0xc00, -90, 16, 14, 5,
                               0x200, -256, 0x200, -256, 0x200, -256);
            }
            record->type = KF_EFFECT_SLOT_FREE;
        } else if (rand() < 16384) {
            func_80040308(10, 0, 0x6d, &record->position,
                           &record->rotation, effect_state.current_index);
        }
        break;
    }
    case 25:
    case 34:
    case 35:
        collision = func_80042298(250, 100, record->kind == 25 ? -30 : 0);
        if (collision != 0) {
            if (collision & 0xf) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            if (record->unknown_3c[4] == 0) {
                record->unknown_3c[4] = 1;
                func_8003feb0(collision);
            }
        } else {
            record->unknown_3c[4] = 0;
        }
        func_80041e0c(&record->position, 0x2000, 0x2000, 500);
        break;
    case 26:
    case 27:
        record->type = 0x21;
        record->rotation.vz += 100;
        func_8003ff18(&record->position, 0, (s16)record->scale_x,
                       0x8000, 0x400, 0x1000);
        record->type = 0x24;
        if ((s8)record->unknown_3c[4] == 0) {
            if (func_80042298(100, 200, 0) != 0) {
                record->direction.vz = 0;
                record->direction.vy = 0;
                record->direction.vx = 0;
            }
        } else if ((s8)record->unknown_3c[4] == 1) {
            record->scale_z -= 512;
            if ((s16)record->scale_z <= 0) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
        }
        break;
    case 29:
    case 30:
    case 31:
    case 47:
    case 48: {
        VECTOR projected;
        VECTOR midpoint;
        s32 age;
        s32 prior_y;
        s32 acceleration;

        if (record->phase != 0) {
            break;
        }
        acceleration = record->kind == 30 || record->kind == 47 ? 5 : 10;
        /* These kinds overlay the record tail with a Y origin and age. */
        if ((s16)*(u16 *)&record->unknown_3c[6] == 0) {
            audio_play_spatial_range(5, &record->position, 110,
                                     28000, 29000, 0);
        }
        age = *(u16 *)&record->unknown_3c[6] + 1;
        *(u16 *)&record->unknown_3c[6] = age;
        age = (s16)age;
        projected.vx = record->position.vx + record->direction.vx;
        prior_y = record->position.vy;
        projected.vy = (s16)*(u16 *)&record->unknown_3c[4] +
                       record->direction.vy * age +
                       ((acceleration * age * age) >> 1);
        projected.vz = record->position.vz + record->direction.vz;
        collision = func_8003fa68(&projected, 20, 20);
        if (collision == 0) {
            midpoint.vx = (projected.vx + record->position.vx) >> 1;
            midpoint.vy = (projected.vy + record->position.vy) >> 1;
            midpoint.vz = (projected.vz + record->position.vz) >> 1;
            collision = func_8003fa68(&midpoint, 20, 20);
        }
        record->position.vx = projected.vx;
        record->position.vy = projected.vy;
        record->position.vz = projected.vz;
        func_80041e0c(&record->position, 0x2000, 0x2000, 500);
        if (collision != 0) {
            func_8003feb0(collision | 0x20000);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            record->unknown_0a = KF_COLLISION_CACHE_LAYER ? 2 : 1;
            func_800154fc(record->direction.vx,
                          record->position.vy - prior_y,
                          record->direction.vz,
                          (struct KfEulerAngles *)&record->rotation);
        }
        break;
    }
    case 33:
    case 53: {
        VECTOR target;
        s32 motion_scale = record->kind == 33 ? 3800 : 7600;
        s32 result;
        s32 count;

        target.vx = player_state.camera_position.vx;
        target.vy = player_state.camera_position.vy - 1600;
        target.vz = player_state.camera_position.vz;
        result = func_80041b14(&target, 400, 60, 3000, 0, 10, 0);
        func_80041e0c(&record->position, 0x2000, 0x2000, 500);
        if (result == -1) {
            func_8003feb0(KF_COLLISION_CACHE_FLAGS);
            for (count = 11; count >= 0; count--) {
                func_80041e94(record, -1, -2, 0xc00, -90, 16, 33, 5,
                               0x200, -256, 0x200, -256, 0x200, -256);
            }
            record->type = KF_EFFECT_SLOT_FREE;
        }
        func_80041e94(record, -1, -3, motion_scale,
                       -motion_scale / 48, 6, 33, 0, 0x400);
        break;
    }
    case 38:
    case 39: {
        s32 trigger;
        s32 count;
        u32 flags;

        if (record->kind == 39 && record->updates_remaining < 45) {
            trigger = func_8004195c(0x258, 0x28, 0x24, 0x32,
                                    100, 0x1000, 0x104, 0x800) == -1;
        } else {
            if (record->kind == 39) {
                record->direction.vy = (u16)record->direction.vy + 10;
            }
            trigger = func_80042298(100, 0, 0) != 0;
        }
        if (trigger) {
            flags = KF_COLLISION_CACHE_FLAGS;
            if (flags & 0x10) {
                if (record->unknown_3c[4] == 0) {
                    func_8003feb0(flags);
                }
                record->unknown_3c[4] = 1;
            } else {
                record->unknown_3c[4] = 0;
            }
            for (count = 11; count >= 0; count--) {
                func_80041e94(record, -1, -2, 0xc00, -90, 16, 14, 5,
                               0x200, -256, 0x200, -256, 0x200, -256);
            }
            if (KF_COLLISION_CACHE_FLAGS & 0xf) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
        }
        func_80041e0c(&record->position, 0x2000, 0x2000, 500);
        break;
    }
    case 40:
        collision = func_80042298(100, 200, 0);
        if (collision != 0) {
            func_8003feb0(collision);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            func_80041e0c(&record->position, 0x2000, 0x2000, 500);
        }
        break;
    case 42:
        if ((s8)record->unknown_3c[4] == 0) {
            func_80041cd0(0x3800, 0x1f8, 0x46, 0x800, 0x8000);
        } else {
            record->unknown_3c[4]--;
        }
        break;
    case 45:
        func_80041cd0(0x4000, (s16)*(u16 *)&record->unknown_3c[4],
                       0x46, 0x800, 0x8000);
        break;
    case 46: {
        KfEffectRecord *selected =
            &effect_state.records[(s8)record->unknown_3c[5]];
        s32 height;
        s32 age;

        record->position.vx += record->direction.vx;
        record->position.vz += record->direction.vz;
        if ((s8)record->unknown_3c[4] == 0) {
            collision = func_8002b9d4(record->position.vx, record->position.vy,
                                      record->position.vz, 10,
                                      (s16)record->scale_y, 0x30);
            func_8003feb0(collision);
            func_8002b604(record->position.vx, selected->position.vy,
                          record->position.vz, 0, 0);
            record->position.vy = KF_COLLISION_CACHE_RESULT;
            height = record->position.vy - KF_COLLISION_CACHE_HEIGHT_LIMIT;
            record->scale_y = height > 32767 ? 32767 : height;
            if (selected->type == KF_EFFECT_SLOT_FREE) {
                record->unknown_3c[4] = 1;
                *(u16 *)&record->unknown_3c[6] = 0;
                *(u16 *)&record->unknown_32[0] = record->scale_y;
            }
        } else if ((s8)record->unknown_3c[4] == 1) {
            record->scale_y = func_8001584c(
                (s16)*(u16 *)&record->unknown_32[0], 0,
                (s16)*(u16 *)&record->unknown_3c[6]);
            age = *(u16 *)&record->unknown_3c[6] + 512;
            *(u16 *)&record->unknown_3c[6] = age;
            if ((s16)age >= 4096) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
        }
        break;
    }
    case 50: {
        s32 distance;

        switch ((s8)record->unknown_3c[4]) {
        case 0:
            step = (u16)record->scale_z + 64;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
            func_80026330(0, &record->position);
            if ((s16)record->scale_x >= 256) {
                record->unknown_3c[4] = 1;
                func_80025878(1000, 0, &record->direction, &distance);
            }
            break;
        case 1:
            if (func_80042298(512, (s32)0x80000200, 0) != 0) {
                record->unknown_3c[4] = 2;
                func_8003ff18(&record->position, 0, 0x400,
                               0x8000, 0x1000, 0x1000);
            }
            break;
        case 2:
            if ((s16)record->scale_x >= 512) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            step = (u16)record->scale_z + 64;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
            break;
        }
        break;
    }
    case 51:
        func_80041cd0(0x1000, 0x400, 0x80, 0x800, 0x8000);
        break;
    case 52:
        func_80041cd0(0x1000, 0x800, 0x100, 0x800, 0x8000);
        break;
    case 101:
        record->direction.vy = (u16)record->direction.vy +
                               *(u16 *)&record->unknown_3c[6];
        record->position.vx += record->direction.vx;
        record->position.vz += record->direction.vz;
        step = record->scale_x + *(u16 *)&record->unknown_3c[4];
        record->scale_x = step;
        record->scale_y = step;
        record->scale_z = step;
        record->position.vy += record->direction.vy;
        break;
    case 102:
        record->phase++;
        record->scale_y = (rsin(record->phase << 8) *
                           (s16)*(u16 *)&record->unknown_3c[4]) >> 12;
        if (record->phase >= 8) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 105: {
        u8 actor_index = record->unknown_3c[5];

        record->rotation.vz = (u16)record->rotation.vz + 800;
        if (record->phase == 2) {
            step = (u16)record->scale_x - 128;
            record->scale_x = step;
            record->scale_y = step;
            record->direction.vy = (u16)record->direction.vy + 5;
        } else if (actor_index != 0xff) {
            KfActor *actor = &actor_state.actors[actor_index];
            VECTOR vertex_offset;
            VECTOR actor_position;
            VECTOR *position;
            VECTOR next_position;

            func_8003c000(actor, *(s16 *)&record->unknown_3c[6],
                          &vertex_offset);
            position = func_8003c10c(actor, &actor_position);
            next_position.vx = vertex_offset.vx + position->vx;
            next_position.vy = vertex_offset.vy + position->vy;
            next_position.vz = vertex_offset.vz + position->vz;
            if (record->phase == 0) {
                collision = func_80041b14(&next_position, 300, 50,
                                           500, 150, 10, 0);
                if (collision == -2) {
                    if (actor->rotation.y != 0) {
                        actor->rotation.y--;
                    }
                    record->phase = 1;
                } else {
                    if (collision == -1 &&
                        (KF_COLLISION_CACHE_FLAGS & 0xf) != 0) {
                        if (actor->rotation.y != 0) {
                            actor->rotation.y--;
                        }
                        record->type = KF_EFFECT_SLOT_FREE;
                    }
                    break;
                }
            }
            /* Retail also copies the local VECTOR pad without a defining
             * store on this path; its source value remains unresolved. */
            record->position.vx = next_position.vx;
            record->position.vy = next_position.vy;
            record->position.vz = next_position.vz;
            if (record->updates_remaining < 2) {
                record->phase = 2;
                record->updates_remaining = (rand() >> 12) + 16;
                record->direction.vx = vertex_offset.vx >> 1;
                record->direction.vy = vertex_offset.vy >> 1;
                record->direction.vz = vertex_offset.vz >> 1;
            }
            break;
        }
        collision = func_80042298(100, 0, 0);
        if (collision != 0 && (collision & 0xf) != 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    }
    case 106:
        if (record->phase == 0) {
            record->direction.vy = (u16)record->direction.vy + 20;
            collision = func_80042298(140, (s32)0x80000000, -300);
            if (collision != 0) {
                record->position.vx -= record->direction.vx;
                record->position.vy -= record->direction.vy;
                record->position.vz -= record->direction.vz;
                func_800424f0(1, 0, -400, 60);
                func_800424f0(6, 60, -330, 56);
                func_800424f0(8, 140, -170, 40);
                effect_play_spatial_sound(record, 0x25);
                record->unknown_3c[4] = 15;
                record->phase = 1;
                record->unknown_08 = 0;
            } else {
                func_80041e94(record, -1, -3, 6000, -800,
                               6, 8, 0, -1024);
            }
        } else if (record->phase == 1 && record->unknown_3c[4] == 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 107: {
        KfEffectRecord *selected =
            &effect_state.records[record->unknown_3c[4]];
        const u8 *snapshots;
        const u8 *snapshot;
        s32 frame_index;

        if (record->phase == 0 && selected->phase >= 3) {
            record->phase = 1;
            record->updates_remaining = record->unknown_3c[5] * 3;
        }
        if (selected->phase == 4) {
            record->type = KF_EFFECT_SLOT_FREE;
            break;
        }
        frame_index = selected->unknown_3c[8] -
                      record->unknown_3c[5] * 3;
        if (frame_index < 0) {
            frame_index += 24;
        }
        /* The selected variant's +0x40 word points to 24-byte snapshots;
         * its allocation and complete tail layout are not yet owned. */
        snapshots = *(const u8 *const *)&selected->unknown_3c[4];
        snapshot = snapshots + frame_index * 24;
        record->position = *(const VECTOR *)snapshot;
        record->rotation = *(const SVECTOR *)(snapshot + 16);
        break;
    }
    case 109:
        func_80041b14(
            &effect_state.records[record->unknown_3c[4]].position,
            500, 15, -1, 0, 0, -1);
        break;
    case 111: {
        VECTOR spawn_position;
        SVECTOR spawn_direction;
        s32 spread;

        if ((s16)*(u16 *)&record->unknown_3c[4] == 0xff) {
            spawn_position.vx = record->position.vx;
            spawn_position.vy = record->position.vy;
            spawn_position.vz = record->position.vz;
            spawn_direction.vx = 0;
            spawn_direction.vy = 0;
            spawn_direction.vz = 0;
            spread = 1000;
        } else {
            const KfActor *actor =
                &actor_state.actors[(s16)*(u16 *)&record->unknown_3c[4]];

            spawn_position.vx = actor->position.vx;
            spawn_position.vy = actor->position.vy;
            spawn_position.vz = actor->position.vz;
            spawn_direction = *(const SVECTOR *)&actor->unknown_50;
            spread = actor->unknown_1c;
        }
        spawn_position.vx += ((rand() * spread) >> 14) - spread;
        spawn_position.vy -= 2000;
        spawn_position.vz += ((rand() * spread) >> 14) - spread;
        func_80040308(10, record->type | 3, 0,
                       &spawn_position, &spawn_direction);
        break;
    }
    case 113:
        collision = func_80042298(180, 360, 0);
        if (collision != 0 || record->updates_remaining < 2) {
            func_80040308(10, record->type | 3, 0x2a,
                           &record->position, 0, 0);
            func_80040308(10, record->type | 3, 0x2a,
                           &record->position, 0, 1);
            func_80040308(10, record->type | 3, 0x2a,
                           &record->position, 0, 2);
            effect_play_spatial_sound(record, 0x17);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            func_80041e0c(&record->position, 0x2000, 0x2000, 500);
        }
        break;
    case 114: {
        s32 count;

        record->position.vx += record->direction.vx;
        record->position.vy += record->direction.vy;
        record->position.vz += record->direction.vz;
        collision = func_8002b9d4(record->position.vx, record->position.vy,
                                  record->position.vz, 10, 176, 10);
        if (collision == 0) {
            /* The kind-114 constructor saves its original Y at +0x44.
             * Other effect kinds use this tail differently. */
            func_8002b604(record->position.vx,
                          *(s32 *)&record->unknown_3c[8],
                          record->position.vz, 0, 0);
            if (record->position.vy < KF_COLLISION_CACHE_RESULT) {
                for (count = 0; count < 2; count++) {
                    func_80041e94(record, (rand() * 20) >> 15,
                                   -3, 6000, -200, 5, 39, 0, 0x100);
                }
                break;
            }
            record->position.vy = KF_COLLISION_CACHE_RESULT;
        }
        func_80040308(10, record->type | 3, 3, &record->position, 0, 0);
        record->type = KF_EFFECT_SLOT_FREE;
        break;
    }
    case 115:
        collision = func_8004195c(0x258, 0x28, 0x24, 0xb4,
                                  0x168, 0x1000, 0x104, 0x800);
        if (collision == -1 || record->updates_remaining < 2) {
            func_80042424();
            func_80040308(10, record->type | 3, 0x2a,
                           &record->position, 0, 0);
            func_80040308(10, record->type | 3, 0x2a,
                           &record->position, 0, 1);
            func_80040308(10, record->type | 3, 0x2a,
                           &record->position, 0, 2);
            effect_play_spatial_sound(record, 0x18);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            func_80041e0c(&record->position, 0x2000, 0x2000, 500);
        }
        break;
    case 116: {
        VECTOR midpoint;

        midpoint.vx = record->position.vx + (record->direction.vx >> 1);
        midpoint.vy = record->position.vy + (record->direction.vy >> 1);
        midpoint.vz = record->position.vz + (record->direction.vz >> 1);
        record->position.vx += record->direction.vx;
        record->position.vy += record->direction.vy;
        record->position.vz += record->direction.vz;
        if (func_8002b7f8(midpoint.vx, midpoint.vy, midpoint.vz, 5, 10) ||
            func_8002b7f8(record->position.vx, record->position.vy,
                           record->position.vz, 5, 10)) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        func_80040308(10, record->type, 0x2d, &record->position, 0, 0x1a4);
        break;
    }
    case 117: {
        VECTOR elevated;
        s32 actor_index;

        record->position.vx += record->direction.vx;
        record->position.vy += record->direction.vy;
        record->position.vz += record->direction.vz;
        elevated = record->position;
        elevated.vy += 5000;
        actor_index = func_8003a9f4(elevated.vx, elevated.vy, elevated.vz,
                                    100, 10000);
        if (actor_index != -1) {
            func_80040308(10, record->type, 0x2d,
                           &actor_state.actors[actor_index].position, 0,
                           0x4ec);
        }
        func_80041e0c(&elevated, 0x2000, 0x7fff, 10000);
        break;
    }
    case 118:
    case 119:
        if (func_80042298(140, 0, -200) != 0) {
            func_80040308(10, record->type | 3,
                           record->kind == 118 ? 0x33 : 0x34,
                           &record->position, 0);
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 120: {
        s32 finish;

        record->direction.vy = (u16)record->direction.vy + 20;
        if ((s16)record->scale_x < 0x1000) {
            step = (u16)record->scale_z + 0x200;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
        }
        finish = rand() < 400;
        if (!finish) {
            collision = func_80042298(180, 0, -300);
            if ((collision & 5) != 0) {
                record->position.vy = KF_COLLISION_CACHE_RESULT;
                finish = 1;
            }
        }
        if (finish) {
            if (rand() < 8192) {
                func_80040308(10, record->type | 3, 0x2a,
                               &record->position, 0, 0);
                func_80040308(10, record->type | 3, 0x2a,
                               &record->position, 0, 1);
                effect_play_spatial_sound(record, 0x18);
            }
            record->type = KF_EFFECT_SLOT_FREE;
        }
        record->rotation.vz += 2700;
        break;
    }
    /* The remaining effect kinds, including two indirect switch dispatches,
     * are not yet reconstructed. Their callback and BSS owners remain open. */
    }
}
