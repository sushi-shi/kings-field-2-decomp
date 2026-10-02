#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/collision_cache.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/map_object.h>
#include <kf/lib/address.h>
#include <psyq/sdk.h>


RODATA(0x80012960, 0x2d8)



DATA(0x801b2140, 0x3918)
KfEventState event_state;

ADDRESS(0x800482f8, 0xb0)
void event_state_initialize(void)
{
    u16 *offset;
    s32 index;
    KfEventControlSentinels *sentinels;

    repeat_store_word(event_state.control.clear_words, 0, 0x40);
    repeat_store_word(event_state.arena.clear_words, 0, 0xe00);
    sentinels = &event_state.control.fields.sentinels;
    sentinels->unknown_08 = 0xffff;
    sentinels->unknown_04 = 0xffff;
    sentinels->unknown_00 = 0xffff;
    memory_arena_initialize_blocks(&event_state.arena.first_block, 0x3800);
    offset = event_state.saved_offsets;
    for (index = KF_EVENT_SAVED_SLOT_COUNT - 1; index != -1; index--) {
        *offset++ = 0xffff;
    }
    repeat_store_word((u32 *)game_counter_bytes, 0, 0x1e);
    game_counter_bytes[0] = 1;
}

ADDRESS(0x800483a8, 0x30)
void callback_invoke_slot_04_zero(void)
{
    state_8017d118.active_table[1](0);
}

ADDRESS(0x800483d8, 0x50)
void event_saved_offsets_decode(u8 **pointers)
{
    u16 *offset = event_state.saved_offsets;
    s32 index = KF_EVENT_SAVED_SLOT_COUNT - 1;
    u16 absent = 0xffff;

    for (; index != -1; index--) {
        u16 value = *offset++;
        if (value == absent) {
            *pointers = 0;
        } else {
            *pointers = value + event_state.arena.bytes;
        }
        pointers++;
    }
}

ADDRESS(0x80048428, 0x70)
void event_arena_owner_pointers_add_delta(s32 delta)
{
    KfMemoryBlock *block = &event_state.arena.first_block;

    if (block->kind != 0xff) {
        do {
            s32 kind = block->kind;
            u32 step;
            if (kind < 4) {
                if (kind != 0) {
                    block->owner = (u8 **)((u8 *)block->owner + delta);
                }
            }
            step = block->size + sizeof(*block);
            block = (KfMemoryBlock *)((u8 *)block + step);
        } while (block->kind != 0xff);
    }
}

ADDRESS(0x80048498, 0x4c)
void event_saved_offsets_encode(u8 **pointers)
{
    u16 *offset = event_state.saved_offsets;
    s32 index;

    for (index = KF_EVENT_SAVED_SLOT_COUNT - 1; index != -1; index--) {
        u8 *value = *pointers;
        pointers++;
        if (value == 0) {
            *offset = 0xffff;
        } else {
            *offset = (u16)(value - event_state.arena.bytes);
        }
        offset++;
    }
}

ADDRESS(0x800484e4, 0x70)
void event_arena_owner_pointers_subtract_delta(s32 delta)
{
    KfMemoryBlock *block = &event_state.arena.first_block;

    if (block->kind != 0xff) {
        do {
            s32 kind = block->kind;
            u32 step;
            if (kind < 4) {
                if (kind != 0) {
                    block->owner = (u8 **)((u8 *)block->owner - delta);
                }
            }
            step = block->size + sizeof(*block);
            block = (KfMemoryBlock *)((u8 *)block + step);
        } while (block->kind != 0xff);
    }
}



ADDRESS(0x80048554, 0x458)
void event_world_state_save_slot(s32 save_slot)
{
    u8 *saved[KF_EVENT_SAVED_SLOT_COUNT];
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
            *write++ = candidate->word_10.bytes.fallback_offset;
            *write++ = candidate->word_12.bytes.marker_state;
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

    event_saved_offsets_decode(saved);
    event_arena_owner_pointers_add_delta((s32)saved);
    block = saved[save_slot];
    if (block != 0) {
        memory_block_release(block);
    }
    size = (write - payload + 3) & ~3;
    block = memory_arena_allocate_block(&event_state.arena.first_block,
                                        size, &saved[save_slot]);
    if (block != 0) {
        resource_copy_words((u32 *)block, (const u32 *)payload, size >> 2);
        event_saved_offsets_encode(saved);
        event_arena_owner_pointers_subtract_delta((s32)saved);
    }
}



ADDRESS(0x800489ac, 0x378)
void event_world_state_restore_slot(s32 save_slot)
{
    u8 *saved[KF_EVENT_SAVED_SLOT_COUNT];
    u8 *stream;
    s32 index;
    KfActor *actors;
    KfTargetGroup *groups;
    KfMapObject *object;

    event_saved_offsets_decode(saved);
    stream = saved[save_slot];
    if (stream == 0) {
        return;
    }

    actors = actor_state.actors;
    for (;;) {
        s32 actor_index = *stream++;
        if (actor_index == 0xff) {
            break;
        }
        actors[actor_index].lifecycle = *stream++;
    }

    groups = actor_state.target_groups;
    for (;;) {
        s32 group_index = *stream++;
        KfTargetCandidate *candidate;
        if (group_index == 0xff) {
            break;
        }
        candidate = groups[group_index].targets[0].pointer;
        candidate->word_10.bytes.fallback_offset = *stream++;
        candidate->word_12.bytes.marker_state = *stream++;
    }

    object = map_object_state.objects;
    for (index = 0; index < KF_MAP_OBJECT_CAPACITY; object++, index++) {
        u8 opcode = *stream++;
        u16 x;
        u16 y;
        u16 z;

        switch (opcode - 0xf0) {
        case 15:
            object->object_id = 0xff;
            break;
        case 4:
            object->tail.fields.unknown_38 = *stream++;
            object->tail.fields.unknown_39 = *stream++;
            break;
        case 0: {
            s32 x_high;
            s32 z_high;
            s32 y_high;
            s32 angle;

            map_object_reset(object);
            object->action = 0x60;
            object->object_id = *stream++;
            x = *stream++;
            x_high = *stream++;
            z = *stream++;
            z_high = *stream++;
            y = *stream++;
            y_high = *stream++;
            angle = *stream++;
            x |= x_high << 8;
            z |= z_high << 8;
            y |= y_high << 8;
            object->rotation.vz = 0x400;
            object->rotation.vy = angle << 4;
apply_position:
            object->position.vx = x << 2;
            object->position.vz = z << 2;
            object->position.vy = (s16)y;
            object->action_timer = 0x63;
            func_8002a988(object->position.vx, object->position.vy,
                           object->position.vz);
            object->unknown_00 = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
            object->tail.fields.unknown_38 = 0xff;
            continue;
        }
        case 1:
            map_object_reset(object);
            object->action = 0x61;
            object->object_id = *stream++;
            x = *stream++;
            x |= *stream++ << 8;
            z = *stream++;
            z |= *stream++ << 8;
            y = *stream++;
            y |= *stream++ << 8;
            goto apply_position;
        case 2:
            map_object_reset(object);
            object->action = 0x62;
            object->object_id = *stream++;
            x = *stream++;
            x |= *stream++ << 8;
            z = *stream++;
            z |= *stream++ << 8;
            y = *stream++;
            y |= *stream++ << 8;
            object->tail.fields.unknown_3a.value = (*stream << 2) + (rand() >> 13);
            stream++;
            if (object->object_id == 0x67) {
                object->rotation.vx = 0x400;
            }
            goto apply_position;
        case 3:
            map_object_reset(object);
            object->action = 0x70;
            object->object_id = *stream++;
            /* The next byte is also the body of opcode 0xfd. */
        case 13:
            object->tail.fields.unknown_38 = *stream++;
            break;
        default:
            break;
        }
    }
}
