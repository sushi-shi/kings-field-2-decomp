#ifndef KF_GAME_EVENT_STATE_H
#define KF_GAME_EVENT_STATE_H

#include <kf/lib/types.h>
#include <kf/game/memory.h>
#include <psyq/sdk.h>

struct KfPlayerViewRotation;

enum { KF_EVENT_SAVED_SLOT_COUNT = 10 };

/* The startup clear and the event initializer bound one BSS object. The
 * control bytes are still mostly unclassified; the arena and saved offset
 * table are used together by the save/restore routines. */
typedef struct KfEventControlObjectSlot {
    u16 object_index;
    u8 resource_id;
    u8 unknown_03;
} KfEventControlObjectSlot;
typedef char kf_event_control_object_slot_size[
    sizeof(KfEventControlObjectSlot) == 4 ? 1 : -1];
typedef char kf_event_control_object_slot_resource_offset[
    (u32)&((KfEventControlObjectSlot *)0)->resource_id == 2 ? 1 : -1];

typedef struct KfEventControlFields {
    u8 unknown_04[0x28];
    KfEventControlObjectSlot object_slots[3];
    u8 unknown_34[0x0b];
    u8 stream_actor_definition_id;
} KfEventControlFields;
typedef char kf_event_control_fields_size[
    sizeof(KfEventControlFields) == 0x40 ? 1 : -1];
typedef char kf_event_control_object_slots_offset[
    (u32)&((KfEventControlFields *)0)->object_slots == 0x28 ? 1 : -1];
typedef char kf_event_control_last_slot_resource_offset[
    (u32)&((KfEventControlFields *)0)->object_slots[2].resource_id == 0x32 ? 1 : -1];
typedef char kf_event_control_stream_actor_definition_offset[
    (u32)&((KfEventControlFields *)0)->stream_actor_definition_id == 0x3f ? 1 : -1];

typedef union KfEventControl {
    u32 clear_words[0x40];
    u8 bytes[0x100];
    KfEventControlFields fields;
} KfEventControl;

typedef union KfEventArena {
    u32 clear_words[0xe00];
    KfMemoryBlock first_block;
    u8 bytes[0x3800];
} KfEventArena;

typedef struct KfEventState {
    u32 state_word;
    KfEventControl control;
    KfEventArena arena;
    u16 saved_offsets[KF_EVENT_SAVED_SLOT_COUNT];
} KfEventState;

typedef char kf_event_state_size[sizeof(KfEventState) == 0x3918 ? 1 : -1];
typedef char kf_event_state_control_offset[
    (u32)&((KfEventState *)0)->control == 0x04 ? 1 : -1];
typedef char kf_event_state_arena_offset[
    (u32)&((KfEventState *)0)->arena == 0x104 ? 1 : -1];
typedef char kf_event_state_saved_offsets_offset[
    (u32)&((KfEventState *)0)->saved_offsets == 0x3904 ? 1 : -1];

extern KfEventState event_state;

void event_scene_command_dispatch(const VECTOR *position,
                                  const struct KfPlayerViewRotation *rotation,
                                  s32 command);
void event_world_dispatch_interaction(const VECTOR *position,
                                      const struct KfPlayerViewRotation *rotation);
void event_state_initialize(void);
void event_saved_offsets_decode(u8 **pointers);
void event_arena_owner_pointers_add_delta(s32 delta);
void event_saved_offsets_encode(u8 **pointers);
void event_arena_owner_pointers_subtract_delta(s32 delta);
void event_world_state_save_slot(s32 save_slot);
void event_world_state_restore_slot(s32 save_slot);

#endif
