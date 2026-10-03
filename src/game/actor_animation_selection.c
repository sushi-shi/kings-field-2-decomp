#include <kf/lib/address.h>
#include <kf/game/actor.h>

ADDRESS(0x800397d8, 0x2c)
void actor_set_animation(u8 animation_id)
{
    if (animation_id != 0xff) {
        KfActor *actor = actor_state.current;

        actor->animation_id = animation_id;
        actor->animation_phase = 0;
    }
}

ADDRESS(0x80039804, 0x38)
void actor_set_animation_if_changed(u8 animation_id)
{
    KfActor *actor = actor_state.current;

    if (animation_id != 0xff && actor->animation_id != animation_id) {
        actor->animation_id = animation_id;
        actor->animation_phase = 0;
    }
}
