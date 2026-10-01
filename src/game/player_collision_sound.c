#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/audio.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

extern s32 func_8002b604(s32 x, s32 y, s32 z, s32 radius, s32 height);
extern s32 func_8002b9d4(s32 x, s32 y, s32 z, s32 radius, s32 height, s32 mode);
extern void func_8002b874(void);
extern void func_80024498(const VECTOR *origin, s32 damage, s32 reaction_flags);
extern void func_80023384(void);

enum {
    COLLISION_CACHE_DEPTH_OFFSET = 0x11810,
    COLLISION_DEPTH_ARM_HEIGHT = 200,
    COLLISION_DEPTH_DEATH_LIMIT = 32000
};

ADDRESS(0x80027928, 0x60)
void func_80027928(void)
{
    /* Cache ownership at this offset remains provisional. */
    if (player_state.unknown_13a >= COLLISION_DEPTH_ARM_HEIGHT
        && (*(const s32 *)((const u8 *)&bss_801c7540
                           + COLLISION_CACHE_DEPTH_OFFSET)
            - player_state.camera_position.vy) > COLLISION_DEPTH_DEATH_LIMIT) {
        player_death_begin(NULL);
        player_state.unknown_d1[4] = 1;
    }
}

ADDRESS(0x80027988, 0x44)
void func_80027988(s32 magnitude)
{
    s32 volume = magnitude;

    if (volume >= 320) {
        volume -= 320;
        if (volume >= 897) {
            volume = 896;
        }
        audio_play_sound(12, (volume >> 3) + 32);
    }
}

ADDRESS(0x800279cc, 0x5ac)
void func_800279cc(void)
{
    s32 next_y;
    s32 height_difference;
    s32 collision_flags;
    s32 impact;
    s32 bob;
    s16 movement_speed;

    func_8002b604(player_state.camera_position.vx,
                  player_state.camera_position.vy,
                  player_state.camera_position.vz, 800, 1700);
    player_state.unknown_ea = 0;

    switch (player_state.unknown_d0) {
    case 0:
        break;

    case 0x10:
        func_80027928();
        player_state.camera_position.vy += player_state.unknown_13a;
        player_state.unknown_ea = player_state.unknown_13a;
        player_state.unknown_13a += 40;
        if (KF_COLLISION_CACHE_RESULT + 100 < player_state.camera_position.vy) {
            player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
            player_state.unknown_d0 = 0;
        }
        goto finish;

    case 0x20:
        func_80027928();
        player_state.camera_position.vy += player_state.unknown_13a;
        if (player_state.camera_position.vy <= KF_COLLISION_CACHE_RESULT
            || player_state.unknown_13a >= 0) {
            if (KF_COLLISION_CACHE_RESULT
                < player_state.camera_position.vy - player_state.unknown_13a) {
                player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
            }
            player_state.unknown_d0 = 0;
        }
        player_state.unknown_ea = player_state.unknown_13a;
        player_state.unknown_13a += 5;
        goto finish;

    case 0x40:
        func_80027928();
        next_y = player_state.camera_position.vy + player_state.unknown_13a;
        collision_flags = func_8002b9d4(player_state.camera_position.vx, next_y,
                                         player_state.camera_position.vz, 800, 1700, 0x31);
        if (collision_flags == 0) {
            player_state.unknown_ea = player_state.unknown_13a;
            player_state.unknown_13a += 40;
            player_state.unknown_110[0] = player_state.unknown_13a >> 1;
            player_state.camera_position.vy = next_y;
            goto finish;
        }
        if (player_state.unknown_13a < 0) {
            player_state.unknown_13a = 0;
            goto finish;
        }
        func_80027988(player_state.unknown_13a);
        if (player_state.unknown_13a >= 480) {
            impact = (player_state.unknown_13a * player_state.unknown_13a) >> 12;
            func_80024498(NULL, (impact * impact * impact) / 0x1ccf0, 0);
        }
        if ((collision_flags & 4) != 0) {
            player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
        } else {
            func_8002b874();
            next_y = KF_COLLISION_CACHE_POSITION.vy
                   - KF_COLLISION_CACHE_INTERACTION_HEIGHT - 1;
            if (func_8002b9d4(player_state.camera_position.vx, next_y,
                               player_state.camera_position.vz, 800, 1700, 0x31) == 0) {
                player_state.camera_position.vy = next_y;
            }
        }
        player_state.unknown_138 = 1;
        player_state.unknown_d0 = 0x50;
        /* Enter the landing response in the same frame. */
        goto landing;

    case 0x50:
landing:
        if (player_state.unknown_138 > 0) {
            player_state.unknown_138 += player_state.unknown_13a >> 2;
        }
        player_state.unknown_13a -= 100;
        if (player_state.unknown_110[0] > 0) {
            player_state.unknown_110[0] += player_state.unknown_13a > 0 ? 10 : -30;
        }
        if (player_state.unknown_138 <= 0
            && player_state.unknown_110[0] <= 0) {
            player_state.unknown_d0 = 0;
            player_state.unknown_13a = 0;
            player_state.unknown_138 = 0;
            player_state.unknown_110[0] = 0;
        }
        break;

    default:
        goto finish;
    }

    height_difference = KF_COLLISION_CACHE_RESULT - player_state.camera_position.vy;
    if (height_difference < 0) {
        if (height_difference >= -256) {
            if (height_difference < -128) {
                player_state.camera_position.vy -= 128;
            } else {
                player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
            }
            goto finish;
        }
        if (height_difference >= -512) {
            player_state.camera_position.vy -= 256;
            goto finish;
        }
        movement_speed = player_state.movement_speed.signed_value;
        player_state.unknown_d0 = 0x20;
        player_state.unknown_13a = movement_speed > 200 ? -300 : -150;
    } else {
        if (height_difference <= 0) {
            goto finish;
        }
        if (func_8002b9d4(player_state.camera_position.vx,
                           player_state.camera_position.vy + 1,
                           player_state.camera_position.vz, 800, 1700, 0x31) != 0) {
            goto finish;
        }
        if (height_difference <= 256) {
            if (height_difference >= 129) {
                player_state.camera_position.vy += 128;
            } else {
                player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
            }
            goto finish;
        }
        if (height_difference <= 512) {
            player_state.camera_position.vy += 256;
            goto finish;
        }
        player_state.unknown_d0 = height_difference > 1024 ? 0x40 : 0x10;
        player_state.unknown_13a = 40;
    }
    player_state.unknown_138 = 0;
    player_state.unknown_110[0] = 0;

finish:
    if (player_state.unknown_d0 == 0) {
        if (player_state.unknown_c9[3] != 0) {
            player_state.unknown_136 =
                (player_state.unknown_136 + player_state.movement_speed.unsigned_value) & 0xfff;
            bob = rsin(player_state.unknown_136) >> 5;
            if (bob < 0) {
                bob = -bob;
            }
            player_state.unknown_134 = bob - (bob >> 2);
        } else {
            player_state.unknown_134 = 0;
        }
    }
    func_80023384();
}
