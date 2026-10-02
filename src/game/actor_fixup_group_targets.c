#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>

ADDRESS(0x8003f610, 0x1dc)
void func_8003f610(void)
{
    KfActor *actor;

    actor_state.active_actor_count = 0;
    actor = actor_state.actors;
    actor_state.unknown_93b8 = 0;
    do {
        if (actor->slot_state != KF_ACTOR_SLOT_FREE) {
            actor_bind_current(actor);
            if (((actor_state.unknown_93c4 & 3) ==
                 (actor_state.unknown_93b8 & 3)) ||
                player_state.unknown_09[1] != 0 ||
                player_state.death_state == 1) {
                func_8003983c();
            }

            if (actor->lifecycle == 1) {
                if ((actor_state.unknown_93c4 & 3) ==
                    (actor_state.active_actor_count & 3)) {
                    actor_select_target_for_player_distance();
                }
                func_8003d184();
                actor_state.active_actor_count++;
            }

            if ((actor_state.active_group->unknown_34 & 0x10000) != 0) {
                KfMapObject *object =
                    &map_object_state.objects[actor->unknown_22];
                s32 object_z;

                actor->position.vx = object->position.vx;
                actor->position.vy = object->position.vy - 500;
                object_z = object->position.vz;
                actor->position.vz = object_z;
                if (actor->lifecycle == 1) {
                    actor->rotation.y = vector_xz_to_angle(
                        player_state.camera_position.vx - actor->position.vx,
                        player_state.camera_position.vz - object_z);
                }
            }
        }
        actor++;
        actor_state.unknown_93b8++;
    } while (actor_state.unknown_93b8 < KF_ACTOR_CAPACITY);

    actor_state.unknown_93c4++;
    actor_bind_current(NULL);
}

ADDRESS(0x8003f7ec, 0x74)
void actor_fixup_group_targets(void)
{
    KfTargetGroup *group = actor_state.target_groups;
    KfTargetCandidate *base;
    s32 group_index;
    s32 slot_index;
    KfTargetReference *slot;
    s32 empty_offset;

    group_index = 0;
    empty_offset = -1;
    base = (KfTargetCandidate *)actor_state.unknown_73a0;
    while (group_index < 40) {
        if (group->unknown_00 == 0xff) {
            break;
        }
        slot = group->targets;
        for (slot_index = 0; slot_index < 16; slot_index++, slot++) {
            if (slot->relative_offset == empty_offset) {
                slot->pointer = NULL;
            } else {
                slot->pointer = (KfTargetCandidate *)((u8 *)base + slot->relative_offset);
            }
        }
        group_index++;
        group++;
    }
}

/* The archive loader at 0x80016820 advances across 16-byte records. */
typedef struct KfActorLoadRecord {
    u8 slot_state;
    u8 group_index;
    u8 unknown_02;
    u8 cell_z;
    u8 cell_x;
    u8 unknown_05;
    u8 unknown_06;
    u8 unknown_07;
    u16 unknown_08;
    u16 unknown_0a;
    u16 unknown_0c;
    u16 unknown_0e;
} KfActorLoadRecord;
typedef char kf_actor_load_record_size[sizeof(KfActorLoadRecord) == 16 ? 1 : -1];

ADDRESS(0x8003f860, 0x1cc)
void func_8003f860(const KfActorLoadRecord *records)
{
    KfActor *actor = actor_state.actors;
    u16 remaining = KF_ACTOR_CAPACITY - 1;

    do {
        actor->slot_state = records->slot_state;
        if (actor->slot_state != 0xff) {
            const KfTargetGroup *group;

            actor->group_index = records->group_index;
            actor->unknown_04 = 0;
            actor->unknown_05 = records->unknown_02;
            actor->unknown_06 = records->unknown_07;
            actor->unknown_07[0] = records->cell_z;
            actor->unknown_07[1] = records->cell_x;
            actor->unknown_0a[0] = records->unknown_05;
            actor->unknown_0a[1] = records->unknown_06;
            actor->unknown_20 = records->unknown_08;
            actor->unknown_22 = records->unknown_0a;
            actor->unknown_24 = records->unknown_0c;
            actor->unknown_26 = records->unknown_0e;
            actor->lifecycle = 0;
            actor->target_type = 0;
            actor->unknown_0f = 0xff;
            actor->target = NULL;

            group = &actor_state.target_groups[actor->group_index];
            actor_copy_group_defaults(actor);
            actor_set_home_position(actor);
            actor->unknown_15 = group->unknown_09;
            if ((actor->unknown_28 & 0x10) != 0) {
                if (actor->slot_state == 3) {
                    if (actor->unknown_24 == -1) {
                        actor->unknown_24 = group->unknown_1a;
                    }
                    if (actor->unknown_26 == -1) {
                        actor->unknown_26 = group->unknown_1c;
                    }
                } else {
                    actor->slot_state = 4;
                    actor->unknown_26 = 0;
                }
            }
        } else {
            actor->lifecycle = 0;
        }
        records++;
        actor++;
    } while (remaining-- != 0);
}
