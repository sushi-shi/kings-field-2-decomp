#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
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
    case 109:
        func_80041b14(
            &effect_state.records[record->unknown_3c[4]].position,
            500, 15, -1, 0, 0, -1);
        break;
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
