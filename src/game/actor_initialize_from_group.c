#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>
#include <psyq/libc.h>

extern s32 func_8002b73c(s32 x, s32 z, u16 parameter, s32 mode);

ADDRESS(0x80038e38, 0xc4)
void actor_initialize_from_group(KfActor *actor)
{
    actor_copy_group_defaults(actor);
    actor->lifecycle = 1;
    actor->unknown_0c = 0;
    actor->animation_phase = 0;
    actor->unknown_11 = 0;
    actor->unknown_0d = 0;
    actor->target_type = 0;
    actor->unknown_0f = 0xff;
    actor->target = NULL;
    if ((actor->unknown_05 & 1) == 0) {
        actor->rotation.y = rand() >> 3;
    }
    actor->unknown_54 = 0;
    actor->unknown_52 = 0;
    actor->unknown_50 = 0;
    actor->unknown_58 = 0;
    actor->unknown_14 = 0x47;
    actor->unknown_16 = 0x800;
    if (actor->unknown_28 & 0x80) {
        actor->unknown_13 = 1;
    } else {
        actor->unknown_13 = 0xff;
    }
    func_8002b73c(actor->position.vx, actor->position.vz, actor->unknown_1c, 1);
}
