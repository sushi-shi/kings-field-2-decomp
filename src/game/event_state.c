#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/card.h>
#include <kf/game/card_payload.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/map_object.h>
#include <kf/lib/address.h>
#include <psyq/libc.h>
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
            collision_sample_map_cell_layer(object->position.vx, object->position.vy,
                           object->position.vz);
            object->unknown_00 = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
            object->tail.fields.unknown_38 = 0xff;
            break;
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

ADDRESS(0x80048d24, 0x5b8)
void card_payload_capture_game_state(u8 *buffer)
{
    KfCardSavePayload *payload = (KfCardSavePayload *)buffer;
    KfCardPlayerSnapshot *saved = &payload->player;
    u8 *magic_flag = payload->magic_menu_available;
    KfMagicRecord *record = effect_state.magic_records;
    s32 i;

    memcpy(buffer, state_8017d118.values_04, 5);
    memcpy(&payload->camera_position, &player_state.camera_position, 16);
    memcpy(buffer + KF_CARD_SAVE_ROTATION_OFFSET,
        &player_state.camera_rotation_target, 8);

    saved->experience = player_state.experience;
    saved->next_level_experience = player_state.next_level_experience;
    saved->gold = player_state.gold;
    saved->map_layer_index = player_state.map_layer_index;
    saved->vitals.maximum_hp = player_state.vitals.maximum_hp;
    saved->vitals.current_hp = player_state.vitals.current_hp;
    saved->vitals.maximum_mp = player_state.vitals.maximum_mp;
    saved->vitals.current_mp = player_state.vitals.current_mp;
    saved->base_physical_power = player_state.base_physical_power;
    saved->base_magic = player_state.base_magic;
    saved->physical_power_training = player_state.physical_power_training;
    saved->magic_training = player_state.magic_training;
    saved->poison_timer = player_state.poison_timer;
    saved->curse_strength = player_state.curse_strength;
    saved->unknown_58 = player_state.unknown_58;
    saved->darkness_phase = player_state.darkness_phase;
    saved->unknown_5c = player_state.unknown_5c;
    saved->slow_timer = player_state.slow_timer;
    saved->paralysis_timer = player_state.paralysis_timer;
    saved->defense_boost_timer = player_state.defense_boost_timer;
    saved->attack_boost_timer = player_state.attack_boost_timer;
    saved->unknown_66 = player_state.unknown_66;
    saved->unknown_68 = player_state.unknown_68;
    saved->unknown_6a = player_state.unknown_6a;
    saved->full_mp_timer = player_state.full_mp_timer;
    saved->magic_boost_timer = player_state.magic_boost_timer;
    saved->level = player_state.level;
    saved->unknown_09 = player_state.unknown_09[0];
    saved->equipped_ids[0] = player_state.equipped_head_id;
    saved->equipped_ids[1] = player_state.equipped_body_id;
    saved->equipped_ids[2] = player_state.equipped_arm_id;
    saved->equipped_ids[3] = player_state.equipped_leg_id;
    saved->equipped_ids[4] = player_state.equipped_shield_id;
    saved->equipped_ids[5] = player_state.equipped_accessory_id;
    saved->equipped_ids[6] = player_state.equipped_extra_id;
    saved->primary_magic_shortcut_id = player_state.primary_magic_shortcut_id;
    saved->secondary_magic_shortcut_id = player_state.secondary_magic_shortcut_id;
    saved->secondary_item_shortcut_id = player_state.secondary_item_shortcut_id;
    saved->equipped_weapon_id = player_state.equipped_weapon_id;
    saved->audio_effects_enabled = player_state.audio_effects_enabled;
    saved->audio_music_enabled = player_state.audio_music_enabled;
    saved->unknown_c9[0] = player_state.unknown_c9[0];
    saved->unknown_c9[1] = player_state.unknown_c9[1];
    saved->unknown_c9[2] = player_state.unknown_c9[2];
    saved->unknown_c9[3] = player_state.unknown_c9[3];

    for (i = 63; i != -1; --i) {
        *magic_flag++ = record->menu_available;
        ++record;
    }
    memcpy(payload->game_counters, game_counter_bytes,
        sizeof payload->game_counters);
    memcpy(payload->event_control, event_state.control.bytes,
        sizeof payload->event_control);
    memcpy(payload->event_arena, event_state.arena.bytes,
        sizeof payload->event_arena);
    memcpy(payload->saved_event_offsets, event_state.saved_offsets,
        sizeof payload->saved_event_offsets);
}

ADDRESS(0x800492dc, 0x5e0)
void card_payload_restore_game_state(const u8 *buffer)
{
    const KfCardSavePayload *payload = (const KfCardSavePayload *)buffer;
    const KfCardPlayerSnapshot *saved = &payload->player;
    const u8 *magic_flag = payload->magic_menu_available;
    KfMagicRecord *record = effect_state.magic_records;
    s32 i;

    memcpy(state_8017d118.values_04, buffer, 5);
    state_8017d118.values_04[1] = state_8017d118.values_04[0];
    state_8017d118.values_04[2] = state_8017d118.values_04[0];
    state_8017d118.values_04[3] = state_8017d118.values_04[0];
    state_8017d118.values_04[4] = state_8017d118.values_04[0];
    memcpy(&player_state.camera_position, &payload->camera_position, 16);
    memcpy(&player_state.camera_rotation_target,
        buffer + KF_CARD_SAVE_ROTATION_OFFSET, 8);

    player_state.experience = saved->experience;
    player_state.next_level_experience = saved->next_level_experience;
    player_state.gold = saved->gold;
    player_state.map_layer_index = saved->map_layer_index;
    player_state.vitals.maximum_hp = saved->vitals.maximum_hp;
    player_state.vitals.current_hp = saved->vitals.current_hp;
    player_state.vitals.maximum_mp = saved->vitals.maximum_mp;
    player_state.vitals.current_mp = saved->vitals.current_mp;
    player_state.base_physical_power = saved->base_physical_power;
    player_state.base_magic = saved->base_magic;
    player_state.physical_power_training = saved->physical_power_training;
    player_state.magic_training = saved->magic_training;
    player_state.poison_timer = saved->poison_timer;
    player_state.curse_strength = saved->curse_strength;
    player_state.unknown_58 = saved->unknown_58;
    player_state.darkness_phase = saved->darkness_phase;
    player_state.unknown_5c = saved->unknown_5c;
    player_state.slow_timer = saved->slow_timer;
    player_state.paralysis_timer = saved->paralysis_timer;
    player_state.defense_boost_timer = saved->defense_boost_timer;
    player_state.attack_boost_timer = saved->attack_boost_timer;
    player_state.unknown_66 = saved->unknown_66;
    player_state.unknown_68 = saved->unknown_68;
    player_state.unknown_6a = saved->unknown_6a;
    player_state.full_mp_timer = saved->full_mp_timer;
    player_state.magic_boost_timer = saved->magic_boost_timer;
    player_state.level = saved->level;
    player_state.unknown_09[0] = saved->unknown_09;
    player_state.equipped_head_id = saved->equipped_ids[0];
    player_state.equipped_body_id = saved->equipped_ids[1];
    player_state.equipped_arm_id = saved->equipped_ids[2];
    player_state.equipped_leg_id = saved->equipped_ids[3];
    player_state.equipped_shield_id = saved->equipped_ids[4];
    player_state.equipped_accessory_id = saved->equipped_ids[5];
    player_state.equipped_extra_id = saved->equipped_ids[6];
    player_state.primary_magic_shortcut_id = saved->primary_magic_shortcut_id;
    player_state.secondary_magic_shortcut_id = saved->secondary_magic_shortcut_id;
    player_state.secondary_item_shortcut_id = saved->secondary_item_shortcut_id;
    player_state.equipped_weapon_id = saved->equipped_weapon_id;
    player_state.audio_effects_enabled = saved->audio_effects_enabled;
    player_state.audio_music_enabled = saved->audio_music_enabled;
    player_state.unknown_c9[0] = saved->unknown_c9[0];
    player_state.unknown_c9[1] = saved->unknown_c9[1];
    player_state.unknown_c9[2] = saved->unknown_c9[2];
    player_state.unknown_c9[3] = saved->unknown_c9[3];

    for (i = 63; i != -1; --i) {
        record->menu_available = *magic_flag++;
        ++record;
    }
    memcpy(game_counter_bytes, payload->game_counters,
        sizeof payload->game_counters);
    memcpy(event_state.control.bytes, payload->event_control,
        sizeof payload->event_control);
    memcpy(event_state.arena.bytes, payload->event_arena,
        sizeof payload->event_arena);
    memcpy(event_state.saved_offsets, payload->saved_event_offsets,
        sizeof payload->saved_event_offsets);
}
