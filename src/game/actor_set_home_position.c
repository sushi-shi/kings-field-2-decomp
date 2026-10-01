#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>
#include <kf/game/map_cell.h>
#include <kf/game/player.h>

enum { ACTOR_HOME_CELL_SHIFT = 11 };

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
        actor->position.vy = KF_COLLISION_CACHE_HEIGHT;
    }
    if (!(actor->unknown_28 & 0x10)) {
        actor->position.vy += actor->unknown_26;
    }
}
