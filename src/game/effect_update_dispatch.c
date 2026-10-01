#include <kf/lib/address.h>
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
    case 109:
        func_80041b14(
            &effect_state.records[record->unknown_3c[4]].position,
            500, 15, -1, 0, 0, -1);
        break;
    /* The remaining effect kinds, including two indirect switch dispatches,
     * are not yet reconstructed. Their callback and BSS owners remain open. */
    }
}
