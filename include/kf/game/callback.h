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
    u8 values_04[5];
    u8 unknown_09[3];
    KfCallback *active_table;
    u8 values_10[5];
    u8 unknown_15;
    u8 flag_16;
    s8 values_17[3];
    s16 unknown_1a;
} KfState8017d118;

typedef char kf_state_8017d118_size[sizeof(KfState8017d118) == 0x1c ? 1 : -1];
typedef char kf_state_8017d118_table_offset[(u32)&((KfState8017d118 *)0)->active_table == 0xc ? 1 : -1];
typedef char kf_state_8017d118_values_10_offset[(u32)&((KfState8017d118 *)0)->values_10 == 0x10 ? 1 : -1];
typedef char kf_state_8017d118_flag_16_offset[(u32)&((KfState8017d118 *)0)->flag_16 == 0x16 ? 1 : -1];
typedef char kf_state_8017d118_values_17_offset[(u32)&((KfState8017d118 *)0)->values_17 == 0x17 ? 1 : -1];
typedef char kf_state_8017d118_unknown_1a_offset[(u32)&((KfState8017d118 *)0)->unknown_1a == 0x1a ? 1 : -1];

extern KfState8017d118 state_8017d118;
extern KfCallback callback_default_table[32];

void callback_invoke_slot_04_zero(void);
void func_80015d50();
void resource_transition_set_phase_1(void);
void resource_transition_set_phase_2(void);
void resource_transition_set_phase_3(void);
void resource_transition_set_phase_4(void);
void resource_transition_set_phase_6(void);

#endif
