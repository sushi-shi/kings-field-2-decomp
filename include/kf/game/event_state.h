#ifndef KF_GAME_EVENT_STATE_H
#define KF_GAME_EVENT_STATE_H

#include <kf/lib/types.h>
#include <kf/game/memory.h>

enum { KF_EVENT_SAVED_SLOT_COUNT = 10 };

/* The startup clear and the event initializer bound one BSS object. The
 * control bytes are still mostly unclassified; the arena and saved offset
 * table are used together by the save/restore routines. */
typedef struct KfEventControlSentinels {
    u16 unknown_00;
    u8 unknown_02[2];
    u16 unknown_04;
    u8 unknown_06[2];
    u16 unknown_08;
} KfEventControlSentinels;
typedef char kf_event_control_sentinels_size[
    sizeof(KfEventControlSentinels) == 0x0a ? 1 : -1];
typedef char kf_event_control_sentinels_last_offset[
    (u32)&((KfEventControlSentinels *)0)->unknown_08 == 0x08 ? 1 : -1];

typedef struct KfEventControlFields {
    u8 unknown_04[0x28];
    KfEventControlSentinels sentinels;
} KfEventControlFields;
typedef char kf_event_control_sentinels_offset[
    (u32)&((KfEventControlFields *)0)->sentinels == 0x28 ? 1 : -1];

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

void func_800482f8(void);
void func_800483d8(u8 **pointers);
void func_80048428(s32 delta);
void func_80048498(u8 **pointers);
void func_800484e4(s32 delta);
void func_80048554(s32 save_slot);

#endif
