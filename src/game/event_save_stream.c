#include <kf/game/actor.h>
#include <kf/game/event_state.h>
#include <kf/game/map_object.h>
#include <kf/lib/address.h>

RODATA(0x80012960, 0x294)

ADDRESS(0x80048554, 0x458)
void func_80048554(s32 save_slot)
{
    u8 *saved[10];
    u8 payload[3072];
    u8 *write = payload;
    KfActor *actor = actor_state.actors;
    KfTargetGroup *group;
    KfMapObject *object;
    s32 index;
    s32 size;
    u8 *block;

    for (index = 0; index < KF_ACTOR_CAPACITY; actor++, index++) {
        if (actor->slot_state != 0xff && actor->slot_state == 1) {
            *write++ = index;
            if (actor->lifecycle == 3) {
                *write = 3;
            } else {
                *write = 0;
            }
            write++;
        }
    }
    *write++ = 0xff;

    group = actor_state.target_groups;
    for (index = 0; index < 40; group++, index++) {
        KfTargetCandidate *candidate;
        if (group->unknown_00 == 0xff) {
            break;
        }
        candidate = group->targets[0].pointer;
        if (candidate != 0 && candidate->type == 0x70) {
            *write++ = index;
            *write++ = candidate->fallback_offset;
            *write++ = candidate->marker_state;
        }
    }
    *write++ = 0xff;

    object = map_object_state.objects;
    for (index = 0; index < KF_MAP_OBJECT_CAPACITY; object++, index++) {
        s32 object_id = object->object_id;
        s32 kind;
        if (object_id == 0xff) {
            *write++ = 0xff;
            continue;
        }
        kind = map_object_state.templates[object_id].collision_kind;
        /* Save packets carry the low byte of the 16-bit template ID. */
        switch (kind) {
        case 64:
            switch (object->action) {
            case 0x60:
                *write++ = 0xf0;
                *write++ = *(const u8 *)&object->object_id;
                *write++ = (u32)object->position.vx >> 2;
                *write++ = (u32)object->position.vx >> 10;
                *write++ = (u32)object->position.vz >> 2;
                *write++ = (u32)object->position.vz >> 10;
                *write++ = object->position.vy;
                *write++ = (u32)object->position.vy >> 8;
                *write++ = object->rotation.vy >> 4;
                break;
            case 0x61:
                *write++ = 0xf1;
                *write++ = *(const u8 *)&object->object_id;
                *write++ = (u32)object->position.vx >> 2;
                *write++ = (u32)object->position.vx >> 10;
                *write++ = (u32)object->position.vz >> 2;
                *write++ = (u32)object->position.vz >> 10;
                *write++ = object->position.vy;
                *write++ = (u32)object->position.vy >> 8;
                break;
            case 0x62:
                *write++ = 0xf2;
                *write++ = *(const u8 *)&object->object_id;
                *write++ = (u32)object->position.vx >> 2;
                *write++ = (u32)object->position.vx >> 10;
                *write++ = (u32)object->position.vz >> 2;
                *write++ = (u32)object->position.vz >> 10;
                *write++ = object->position.vy;
                *write++ = (u32)object->position.vy >> 8;
                *write++ = object->tail.fields.unknown_3a.value >> 2;
                break;
            case 0x70:
                *write++ = 0xf3;
                *write++ = *(const u8 *)&object->object_id;
                *write++ = object->tail.fields.unknown_38;
                break;
            default:
                *write++ = 0xfd;
                *write++ = object->tail.fields.unknown_38;
                break;
            }
            break;
        case 83:
            if (object->tail.fields.unknown_38 < 2) {
                break;
            }
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 8:
        case 22:
        case 80:
        case 81:
        case 84:
        case 88:
        case 95:
        case 160:
        case 161:
        case 163:
        case 164:
            *write++ = 0xfd;
            *write++ = object->tail.fields.unknown_38;
            break;
        case 15:
        case 17:
            *write++ = 0xf4;
            *write++ = object->tail.fields.unknown_38;
            *write++ = object->tail.fields.unknown_39;
            break;
        default:
            *write++ = 0xfe;
            break;
        }
    }

    func_800483d8(saved);
    func_80048428((s32)saved);
    block = saved[save_slot];
    if (block != 0) {
        memory_block_release(block);
    }
    size = (write - payload + 3) & ~3;
    block = memory_arena_allocate_block(&event_state.arena.first_block,
                                        size, &saved[save_slot]);
    if (block != 0) {
        resource_copy_words((u32 *)block, (const u32 *)payload, size >> 2);
        func_80048498(saved);
        func_800484e4((s32)saved);
    }
}
