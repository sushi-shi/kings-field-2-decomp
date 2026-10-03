#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/effect.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>

ADDRESS(0x800160e8, 0x178)
void translate_active_world_positions(s32 dx, s32 dy, s32 dz)
{
    KfEffectRecord *effect = effect_state.records;
    KfMapObject *object;
    KfActor *actor;
    u16 effect_remaining;
    u16 object_remaining;
    u16 actor_remaining;

    player_state.camera_position.vx += dx;
    player_state.camera_position.vz += dz;
    player_state.camera_position.vy += dy;

    effect_remaining = KF_EFFECT_CAPACITY - 1;
    do {
        if (effect->type != KF_EFFECT_SLOT_FREE &&
            (effect->render_flags & 0xc) != 0xc) {
            effect->position.vx += dx;
            effect->position.vz += dz;
            effect->position.vy += dy;
        }
        effect++;
    } while (effect_remaining-- != 0);

    object = map_object_state.objects;
    object_remaining = KF_MAP_OBJECT_CAPACITY - 1;
    do {
        if (object->object_id != 0xff) {
            object->position.vx += dx;
            object->position.vz += dz;
            object->position.vy += dy;
        }
        object++;
    } while (object_remaining-- != 0);

    actor = actor_state.actors;
    actor_remaining = KF_ACTOR_CAPACITY - 1;
    do {
        if (actor->slot_state != KF_ACTOR_SLOT_FREE) {
            actor->position.vx += dx;
            actor->position.vz += dz;
            actor->position.vy += dy;
        }
        actor++;
    } while (actor_remaining-- != 0);
}
