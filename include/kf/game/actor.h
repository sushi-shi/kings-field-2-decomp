#ifndef KF_GAME_ACTOR_H
#define KF_GAME_ACTOR_H

#include <kf/lib/bool.h>
#include <kf/lib/math.h>
#include <kf/lib/types.h>
#include <psyq/sdk.h>

enum {
    KF_ACTOR_ANIMATION_PHASE_PERIOD = 0x1000,
    KF_ACTOR_ANIMATION_PHASE_MAX = KF_ACTOR_ANIMATION_PHASE_PERIOD - 1,
    KF_ACTOR_CAPACITY = 200,
    KF_ACTOR_DYNAMIC_START = 190,
    KF_ACTOR_DYNAMIC_COUNT = KF_ACTOR_CAPACITY - KF_ACTOR_DYNAMIC_START,
    KF_ACTOR_SLOT_FREE = 0xff,
    KF_ACTOR_LIFECYCLE_DORMANT = 0
};

/* This is the observed prefix; complete extent and stride remain under study. */
typedef struct KfTargetCandidate {
    u8 type;
    u8 unknown_01[3];
    u8 unknown_04;
    u8 unknown_05[3];
    u16 unknown_08;
    u16 unknown_0a;
    u16 unknown_0c;
    u16 unknown_0e;
    u8 fallback_offset;
    u8 unknown_11;
    u8 unknown_12;
    u8 marker_state;
    u8 bytes[2];
    u16 unknown_16;
    u8 unknown_18[2];
    u16 unknown_1a;
} KfTargetCandidate;
typedef char kf_target_candidate_view_size[sizeof(KfTargetCandidate) == 0x1c ? 1 : -1];
typedef char kf_target_candidate_fallback_offset[(u32)&((KfTargetCandidate *)0)->fallback_offset == 0x10 ? 1 : -1];
typedef char kf_target_candidate_unknown_08_offset[(u32)&((KfTargetCandidate *)0)->unknown_08 == 0x08 ? 1 : -1];
typedef char kf_target_candidate_unknown_0a_offset[(u32)&((KfTargetCandidate *)0)->unknown_0a == 0x0a ? 1 : -1];
typedef char kf_target_candidate_unknown_0c_offset[(u32)&((KfTargetCandidate *)0)->unknown_0c == 0x0c ? 1 : -1];
typedef char kf_target_candidate_unknown_0e_offset[(u32)&((KfTargetCandidate *)0)->unknown_0e == 0x0e ? 1 : -1];
typedef char kf_target_candidate_unknown_11_offset[(u32)&((KfTargetCandidate *)0)->unknown_11 == 0x11 ? 1 : -1];
typedef char kf_target_candidate_unknown_12_offset[(u32)&((KfTargetCandidate *)0)->unknown_12 == 0x12 ? 1 : -1];
typedef char kf_target_candidate_marker_state_offset[(u32)&((KfTargetCandidate *)0)->marker_state == 0x13 ? 1 : -1];
typedef char kf_target_candidate_bytes_offset[(u32)&((KfTargetCandidate *)0)->bytes == 0x14 ? 1 : -1];

/* Group slots hold byte offsets until actor_fixup_group_targets runs. */
typedef union KfTargetReference {
    s32 relative_offset;
    KfTargetCandidate *pointer;
} KfTargetReference;
typedef char kf_target_reference_size[sizeof(KfTargetReference) == 4 ? 1 : -1];

typedef struct KfTargetGroup {
    u8 unknown_00;
    u8 unknown_01[4];
    u8 unknown_05;
    u8 unknown_06;
    u8 unknown_07[2];
    u8 unknown_09;
    u8 unknown_0a[2];
    s16 unknown_0c;
    s16 unknown_0e;
    s16 unknown_10;
    u16 unknown_12;
    u16 unknown_14;
    u16 unknown_16;
    u16 unknown_18;
    u16 unknown_1a;
    u16 unknown_1c;
    u16 unknown_1e;
    u16 unknown_20[8];
    u8 unknown_30[2];
    u16 unknown_32;
    u32 unknown_34;
    KfTargetReference targets[16];
} KfTargetGroup;
typedef char kf_target_group_size[sizeof(KfTargetGroup) == 0x78 ? 1 : -1];
typedef char kf_target_group_byte_05_offset[(u32)&((KfTargetGroup *)0)->unknown_05 == 0x05 ? 1 : -1];
typedef char kf_target_group_byte_06_offset[(u32)&((KfTargetGroup *)0)->unknown_06 == 0x06 ? 1 : -1];
typedef char kf_target_group_offset_x[
    (u32)&((KfTargetGroup *)0)->unknown_0c == 0x0c ? 1 : -1];
typedef char kf_target_group_curve_offset[
    (u32)&((KfTargetGroup *)0)->unknown_20 == 0x20 ? 1 : -1];

typedef struct KfActorHalfword4aBytes {
    u8 low;
    s8 high;
} KfActorHalfword4aBytes;
typedef char kf_actor_halfword_4a_bytes_size[
    sizeof(KfActorHalfword4aBytes) == 2 ? 1 : -1];

typedef union KfActorHalfword4a {
    u16 value;
    KfActorHalfword4aBytes bytes;
} KfActorHalfword4a;
typedef char kf_actor_halfword_4a_size[sizeof(KfActorHalfword4a) == 2 ? 1 : -1];

/* The 0x7c stride and these fields are fixed by the actor pool and phase
 * helpers. Other fields remain open. */
typedef struct KfActor {
    u8 slot_state;
    u8 unknown_01;
    u8 group_index;
    u8 unknown_03;
    u8 unknown_04;
    u8 unknown_05;
    u8 unknown_06;
    u8 unknown_07[2];
    u8 lifecycle;
    u8 unknown_0a[2];
    u8 unknown_0c;
    u8 unknown_0d;
    u8 target_type;
    u8 unknown_0f;
    u8 previous_target_type;
    u8 unknown_11;
    u8 unknown_12;
    u8 unknown_13;
    u8 unknown_14;
    u8 unknown_15;
    s16 unknown_16;
    u16 animation_phase;
    u16 unknown_1a;
    u16 unknown_1c;
    u16 unknown_1e;
    u16 unknown_20;
    s16 unknown_22;
    s16 unknown_24;
    s16 unknown_26;
    u32 unknown_28;
    VECTOR position;
    s32 unknown_3c;
    struct KfEulerAngles rotation;
    u8 unknown_46[2];
    u16 unknown_48;
    KfActorHalfword4a unknown_4a;
    u16 unknown_4c;
    u8 unknown_4e[2];
    s16 unknown_50;
    s16 unknown_52;
    s16 unknown_54;
    u8 unknown_56[2];
    u16 unknown_58;
    u8 unknown_5a[2];
    struct KfPoolRecord *animation_cache;
    KfTargetCandidate *target;
    u8 unknown_64[2];
    s16 animation_step;
    s16 unknown_68;
    s16 unknown_6a;
    s16 unknown_6c;
    u8 unknown_6e[2];
    s16 unknown_70;
    u8 unknown_72[0x0a];
} KfActor;

typedef char kf_actor_size[sizeof(KfActor) == 0x7c ? 1 : -1];
typedef char kf_actor_phase_offset[(u32)&((KfActor *)0)->animation_phase == 0x18 ? 1 : -1];
typedef char kf_actor_position_offset[(u32)&((KfActor *)0)->position == 0x2c ? 1 : -1];
typedef char kf_actor_previous_y_offset[(u32)&((KfActor *)0)->unknown_3c == 0x3c ? 1 : -1];
typedef char kf_actor_home_offset_x[(u32)&((KfActor *)0)->unknown_24 == 0x24 ? 1 : -1];
typedef char kf_actor_home_offset_y[(u32)&((KfActor *)0)->unknown_26 == 0x26 ? 1 : -1];
typedef char kf_actor_motion_x_offset[(u32)&((KfActor *)0)->unknown_50 == 0x50 ? 1 : -1];
typedef char kf_actor_motion_z_offset[(u32)&((KfActor *)0)->unknown_54 == 0x54 ? 1 : -1];
typedef char kf_actor_cache_offset[(u32)&((KfActor *)0)->animation_cache == 0x5c ? 1 : -1];
typedef char kf_actor_target_offset[(u32)&((KfActor *)0)->target == 0x60 ? 1 : -1];
typedef char kf_actor_step_offset[(u32)&((KfActor *)0)->animation_step == 0x66 ? 1 : -1];
typedef char kf_actor_motion_result_68_offset[(u32)&((KfActor *)0)->unknown_68 == 0x68 ? 1 : -1];
typedef char kf_actor_motion_result_6a_offset[(u32)&((KfActor *)0)->unknown_6a == 0x6a ? 1 : -1];
typedef char kf_actor_motion_result_6c_offset[(u32)&((KfActor *)0)->unknown_6c == 0x6c ? 1 : -1];
typedef char kf_actor_unknown_70_offset[(u32)&((KfActor *)0)->unknown_70 == 0x70 ? 1 : -1];

/* The startup clear bounds this runtime; the two trailer writes and actor
 * array are fixed by actor_pool_clear. */
typedef struct KfActorStateGame {
    KfActor actors[KF_ACTOR_CAPACITY];
    KfTargetGroup target_groups[40];
    /* 0x16820 loads the groups and this opaque tail as one 0x32c0-byte span;
     * 0x3f7ec fixes group target offsets after the copy. */
    u8 unknown_73a0[0x2000];
    u8 unknown_93a0;
    u8 unknown_93a1[3];
    s32 unknown_93a4;
    KfTargetGroup *active_group;
    KfActor *current;
    KfTargetGroup *other_group;
    KfActor *other_actor;
    s32 unknown_93b8;
    u32 current_group_index;
    u32 active_actor_count;
    u32 unknown_93c4;
    KfActor *actor_93c8;
} KfActorStateGame;

typedef char kf_actor_state_size[sizeof(KfActorStateGame) == 0x93cc ? 1 : -1];
typedef char kf_actor_state_groups_offset[(u32)&((KfActorStateGame *)0)->target_groups == 0x60e0 ? 1 : -1];
typedef char kf_actor_state_active_group_offset[(u32)&((KfActorStateGame *)0)->active_group == 0x93a8 ? 1 : -1];
typedef char kf_actor_state_current_offset[(u32)&((KfActorStateGame *)0)->current == 0x93ac ? 1 : -1];
typedef char kf_actor_state_unknown_93a4_offset[
    (u32)&((KfActorStateGame *)0)->unknown_93a4 == 0x93a4 ? 1 : -1];
typedef char kf_actor_state_unknown_93b8_offset[
    (u32)&((KfActorStateGame *)0)->unknown_93b8 == 0x93b8 ? 1 : -1];
typedef char kf_actor_state_active_actor_count_offset[
    (u32)&((KfActorStateGame *)0)->active_actor_count == 0x93c0 ? 1 : -1];

extern KfActorStateGame actor_state;

KfActor *actor_pool_find_free(void);
void actor_set_home_position(KfActor *actor);
void actor_set_lifecycle_and_home_position(KfActor *actor);
void actor_pool_clear(void);
void actor_set_target(KfActor *actor, KfTargetCandidate *target);
void actor_copy_group_defaults(KfActor *actor);
void actor_initialize_from_group(KfActor *actor);
void actor_prepare_and_initialize(KfActor *actor);
void actor_bind_current(KfActor *actor);
void actor_fixup_group_targets(void);
KfTargetCandidate *actor_find_target_of_type(const KfTargetGroup *group, u8 type);
u8 func_80046144(const KfTargetCandidate *candidate, u8 marker);
void actor_select_target_type_in_own_group(KfActor *actor, u8 type);
void actor_select_best_target(s32 player_distance);
s32 func_80039108(KfTargetCandidate *target, s32 player_distance);
void actor_select_target_for_player_distance(void);
VECTOR *func_8003c10c(KfActor *actor, VECTOR *output);
s32 func_8003c000(KfActor *actor, s32 vertex_index, VECTOR *output);
void actor_reset_target_and_reselect(void);
void func_800397d8(u8 value);
void func_80039804(u8 value);
void actor_advance_animation_wrapped(KfActor *actor, s16 delta);
void actor_advance_animation_clamped(KfActor *actor, s16 delta);
KfBool32 actor_animation_crossed_phase(const KfActor *actor, u16 phase);

#endif
