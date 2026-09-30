#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/player.h>

extern s32 func_8002b67c(s32 layer, s32 x, s32 z, s32 radius, s32 height);

enum {
    ACTOR_HOME_CELL_SHIFT = 11,
    ACTOR_COLLISION_CACHE_HEIGHT_OFFSET = 0x1180c
};

ADDRESS(0x80038d04, 0xc0)
void actor_set_home_position(KfActor *actor)
{
    actor->position.vx = (actor->unknown_07[1] << ACTOR_HOME_CELL_SHIFT)
                       + actor->unknown_24;
    actor->position.vz = (actor->unknown_07[0] << ACTOR_HOME_CELL_SHIFT)
                       + actor->unknown_22;
    actor->position.vy = func_8002b67c(actor->unknown_06,
                                        actor->position.vx,
                                        actor->position.vz,
                                        actor->unknown_1c,
                                        actor->unknown_1e);
    if (actor->position.vy >= 0) {
        actor->position.vy = 0;
    }
    if (actor->unknown_28 & 0x400) {
        actor->position.vy = *(s32 *)((u8 *)&bss_801c7540
                                       + ACTOR_COLLISION_CACHE_HEIGHT_OFFSET);
    }
    if (!(actor->unknown_28 & 0x10)) {
        actor->position.vy += actor->unknown_26;
    }
}
