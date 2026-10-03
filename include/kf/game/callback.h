#ifndef KF_GAME_CALLBACK_H
#define KF_GAME_CALLBACK_H

#include <kf/lib/types.h>

/* The initialized table has 32 function-pointer rows; the BSS table's extent
 * remains open. Callback signatures are resolved at their individual uses. */
typedef void (*KfCallback)();

/* Startup clears this 0x1c-byte runtime state. Its active table pointer is
 * replaced by both an initialized table and a BSS table. */
typedef struct KfState8017d118 {
    s16 transition_active;
    s16 transition_phase;
    u8 active_resource_ids[5];
    u8 unknown_09[3];
    KfCallback *active_table;
    u8 requested_resource_ids[5];
    u8 world_shift_applied;
    u8 flag_16;
    s8 transition_offset_xzy[3];
    s16 sequence_fade_volume;
} KfState8017d118;

typedef char kf_state_8017d118_size[sizeof(KfState8017d118) == 0x1c ? 1 : -1];
typedef char kf_state_8017d118_table_offset[(u32)&((KfState8017d118 *)0)->active_table == 0xc ? 1 : -1];
typedef char kf_state_8017d118_active_resource_ids_offset[(u32)&((KfState8017d118 *)0)->active_resource_ids == 0x04 ? 1 : -1];
typedef char kf_state_8017d118_requested_resource_ids_offset[(u32)&((KfState8017d118 *)0)->requested_resource_ids == 0x10 ? 1 : -1];
typedef char kf_state_8017d118_world_shift_applied_offset[(u32)&((KfState8017d118 *)0)->world_shift_applied == 0x15 ? 1 : -1];
typedef char kf_state_8017d118_flag_16_offset[(u32)&((KfState8017d118 *)0)->flag_16 == 0x16 ? 1 : -1];
typedef char kf_state_8017d118_transition_offset_xzy_offset[(u32)&((KfState8017d118 *)0)->transition_offset_xzy == 0x17 ? 1 : -1];
typedef char kf_state_8017d118_sequence_fade_volume_offset[(u32)&((KfState8017d118 *)0)->sequence_fade_volume == 0x1a ? 1 : -1];

extern KfState8017d118 state_8017d118;
extern KfCallback callback_default_table[32];

void callback_invoke_slot_04_zero(void);
void resource_noop_callback();
void resource_transition_set_phase_1(void);
void resource_transition_set_phase_2(void);
void resource_transition_set_phase_3(void);
void resource_transition_set_phase_4(void);
void resource_transition_set_phase_6(void);

#endif
