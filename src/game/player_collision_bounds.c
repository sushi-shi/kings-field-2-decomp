#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/player.h>

enum {
    COLLISION_CACHE_LOWER_OFFSET = 0x11818,
    COLLISION_CACHE_UPPER_OFFSET = 0x1181c,
    COLLISION_CACHE_BIAS = 1600,
    COLLISION_LOWER_DEATH_LIMIT = -1000
};

ADDRESS(0x80023384, 0xac)
void func_80023384(void)
{
    s32 lower;
    s32 upper;

    /* The collision-cache words overlap a provisionally sized equipment span. */
    lower = *(s32 *)((u8 *)&bss_801c7540 + COLLISION_CACHE_LOWER_OFFSET)
          + COLLISION_CACHE_BIAS;
    lower -= player_state.unknown_134 + player_state.camera_position.vy
           + player_state.unknown_138;
    player_state.unknown_120 = lower;
    if (lower < COLLISION_LOWER_DEATH_LIMIT) {
        player_death_begin(NULL);
    }

    upper = *(s32 *)((u8 *)&bss_801c7540 + COLLISION_CACHE_UPPER_OFFSET)
          + COLLISION_CACHE_BIAS;
    upper -= player_state.unknown_134 + player_state.camera_position.vy
           + player_state.unknown_138;
    player_state.unknown_124 = upper;
    if (upper <= 0) {
        player_death_begin(NULL);
    }
}
