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

typedef struct KfTargetCandidateWord0cBytes {
    u8 low;
    u8 high;
} KfTargetCandidateWord0cBytes;
typedef char kf_target_candidate_word0c_bytes_size[
    sizeof(KfTargetCandidateWord0cBytes) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord0c {
    u16 value;
    KfTargetCandidateWord0cBytes bytes;
} KfTargetCandidateWord0c;
typedef char kf_target_candidate_word0c_size[
    sizeof(KfTargetCandidateWord0c) == 2 ? 1 : -1];
typedef char kf_target_candidate_word0c_high_offset[
    (u32)&((KfTargetCandidateWord0c *)0)->bytes.high == 1 ? 1 : -1];

typedef struct KfTargetCandidateWord0eBytes {
    u8 low;
    u8 high;
} KfTargetCandidateWord0eBytes;
typedef char kf_target_candidate_word0e_bytes_size[
    sizeof(KfTargetCandidateWord0eBytes) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord0e {
    u16 value;
    KfTargetCandidateWord0eBytes bytes;
} KfTargetCandidateWord0e;
typedef char kf_target_candidate_word0e_size[
    sizeof(KfTargetCandidateWord0e) == 2 ? 1 : -1];

typedef struct KfTargetCandidateWord10Bytes {
    u8 fallback_offset;
    u8 unknown_11;
} KfTargetCandidateWord10Bytes;
typedef char kf_target_candidate_word10_bytes_size[
    sizeof(KfTargetCandidateWord10Bytes) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord10 {
    u16 value;
    KfTargetCandidateWord10Bytes bytes;
} KfTargetCandidateWord10;
typedef char kf_target_candidate_word10_size[
    sizeof(KfTargetCandidateWord10) == 2 ? 1 : -1];

typedef struct KfTargetCandidateWord12Bytes {
    u8 unknown_12;
    u8 marker_state;
} KfTargetCandidateWord12Bytes;
typedef char kf_target_candidate_word12_bytes_size[
    sizeof(KfTargetCandidateWord12Bytes) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord12 {
    u16 value;
    KfTargetCandidateWord12Bytes bytes;
} KfTargetCandidateWord12;
typedef char kf_target_candidate_word12_size[
    sizeof(KfTargetCandidateWord12) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord14 {
    u16 value;
    u8 bytes[2];
} KfTargetCandidateWord14;
typedef char kf_target_candidate_word14_size[
    sizeof(KfTargetCandidateWord14) == 2 ? 1 : -1];

typedef struct KfTargetCandidateWord16Bytes {
    u8 low;
    u8 high;
} KfTargetCandidateWord16Bytes;
typedef char kf_target_candidate_word16_bytes_size[
    sizeof(KfTargetCandidateWord16Bytes) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord16 {
    u16 value;
    KfTargetCandidateWord16Bytes bytes;
} KfTargetCandidateWord16;
typedef char kf_target_candidate_word16_size[
    sizeof(KfTargetCandidateWord16) == 2 ? 1 : -1];

typedef struct KfTargetCandidateWord18Bytes {
    u8 low;
    u8 high;
} KfTargetCandidateWord18Bytes;
typedef char kf_target_candidate_word18_bytes_size[
    sizeof(KfTargetCandidateWord18Bytes) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord18 {
    u16 value;
    KfTargetCandidateWord18Bytes bytes;
} KfTargetCandidateWord18;
typedef char kf_target_candidate_word18_size[
    sizeof(KfTargetCandidateWord18) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord1a {
    u16 value;
    u8 bytes[2];
} KfTargetCandidateWord1a;
typedef char kf_target_candidate_word1a_size[
    sizeof(KfTargetCandidateWord1a) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord1c {
    u16 value;
    u8 bytes[2];
} KfTargetCandidateWord1c;
typedef char kf_target_candidate_word1c_size[
    sizeof(KfTargetCandidateWord1c) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord1e {
    u16 value;
    u8 bytes[2];
} KfTargetCandidateWord1e;
typedef char kf_target_candidate_word1e_size[
    sizeof(KfTargetCandidateWord1e) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord26 {
    u16 unsigned_value;
    s16 signed_value;
} KfTargetCandidateWord26;
typedef char kf_target_candidate_word26_size[
    sizeof(KfTargetCandidateWord26) == 2 ? 1 : -1];

typedef union KfTargetCandidateWord24 {
    u16 unsigned_value;
    s16 signed_value;
} KfTargetCandidateWord24;
typedef char kf_target_candidate_word24_size[
    sizeof(KfTargetCandidateWord24) == 2 ? 1 : -1];

/* This is the observed prefix; complete extent and stride remain under study. */
typedef struct KfTargetCandidate {
    u8 type;
    u8 unknown_01[3];
    u8 sound_code;
    u8 unknown_05[3];
    u16 animation_step;
    u16 sound_trigger;
    KfTargetCandidateWord0c word_0c;
    KfTargetCandidateWord0e word_0e;
    KfTargetCandidateWord10 word_10;
    KfTargetCandidateWord12 word_12;
    KfTargetCandidateWord14 word_14;
    KfTargetCandidateWord16 word_16;
    KfTargetCandidateWord18 word_18;
    KfTargetCandidateWord1a word_1a;
    KfTargetCandidateWord1c word_1c;
    KfTargetCandidateWord1e word_1e;
    u16 unknown_20;
    u16 unknown_22;
    KfTargetCandidateWord24 word_24;
    KfTargetCandidateWord26 word_26;
    u16 unknown_28;
    u16 unknown_2a;
} KfTargetCandidate;
typedef char kf_target_candidate_view_size[sizeof(KfTargetCandidate) == 0x2c ? 1 : -1];
typedef char kf_target_candidate_sound_code_offset[(u32)&((KfTargetCandidate *)0)->sound_code == 4 ? 1 : -1];
typedef char kf_target_candidate_word_10_offset[(u32)&((KfTargetCandidate *)0)->word_10 == 0x10 ? 1 : -1];
typedef char kf_target_candidate_fallback_offset[(u32)&((KfTargetCandidateWord10Bytes *)0)->fallback_offset == 0 ? 1 : -1];
typedef char kf_target_candidate_animation_step_offset[(u32)&((KfTargetCandidate *)0)->animation_step == 0x08 ? 1 : -1];
typedef char kf_target_candidate_sound_trigger_offset[(u32)&((KfTargetCandidate *)0)->sound_trigger == 0x0a ? 1 : -1];
typedef char kf_target_candidate_word_0c_offset[(u32)&((KfTargetCandidate *)0)->word_0c == 0x0c ? 1 : -1];
typedef char kf_target_candidate_word_0e_offset[(u32)&((KfTargetCandidate *)0)->word_0e == 0x0e ? 1 : -1];
typedef char kf_target_candidate_word_0e_low_offset[(u32)&((KfTargetCandidate *)0)->word_0e.bytes.low == 0x0e ? 1 : -1];
typedef char kf_target_candidate_unknown_11_offset[(u32)&((KfTargetCandidateWord10Bytes *)0)->unknown_11 == 1 ? 1 : -1];
typedef char kf_target_candidate_unknown_11_total_offset[(u32)&((KfTargetCandidate *)0)->word_10.bytes.unknown_11 == 0x11 ? 1 : -1];
typedef char kf_target_candidate_word_12_offset[(u32)&((KfTargetCandidate *)0)->word_12 == 0x12 ? 1 : -1];
typedef char kf_target_candidate_marker_state_offset[(u32)&((KfTargetCandidateWord12Bytes *)0)->marker_state == 1 ? 1 : -1];
typedef char kf_target_candidate_marker_state_total_offset[(u32)&((KfTargetCandidate *)0)->word_12.bytes.marker_state == 0x13 ? 1 : -1];
typedef char kf_target_candidate_word_14_offset[(u32)&((KfTargetCandidate *)0)->word_14 == 0x14 ? 1 : -1];
typedef char kf_target_candidate_stream_bytes_offset[(u32)&((KfTargetCandidate *)0)->word_14.bytes == 0x14 ? 1 : -1];
typedef char kf_target_candidate_word_16_offset[(u32)&((KfTargetCandidate *)0)->word_16 == 0x16 ? 1 : -1];
typedef char kf_target_candidate_word_16_high_offset[(u32)&((KfTargetCandidate *)0)->word_16.bytes.high == 0x17 ? 1 : -1];
typedef char kf_target_candidate_word_18_offset[(u32)&((KfTargetCandidate *)0)->word_18 == 0x18 ? 1 : -1];
typedef char kf_target_candidate_word_18_high_offset[(u32)&((KfTargetCandidate *)0)->word_18.bytes.high == 0x19 ? 1 : -1];
typedef char kf_target_candidate_word_1a_offset[(u32)&((KfTargetCandidate *)0)->word_1a == 0x1a ? 1 : -1];
typedef char kf_target_candidate_word_1c_offset[(u32)&((KfTargetCandidate *)0)->word_1c == 0x1c ? 1 : -1];
typedef char kf_target_candidate_word_1e_offset[(u32)&((KfTargetCandidate *)0)->word_1e == 0x1e ? 1 : -1];
typedef char kf_target_candidate_unknown_20_offset[(u32)&((KfTargetCandidate *)0)->unknown_20 == 0x20 ? 1 : -1];
typedef char kf_target_candidate_unknown_22_offset[(u32)&((KfTargetCandidate *)0)->unknown_22 == 0x22 ? 1 : -1];
typedef char kf_target_candidate_word_24_offset[(u32)&((KfTargetCandidate *)0)->word_24 == 0x24 ? 1 : -1];
typedef char kf_target_candidate_word_26_offset[(u32)&((KfTargetCandidate *)0)->word_26 == 0x26 ? 1 : -1];
typedef char kf_target_candidate_unknown_28_offset[(u32)&((KfTargetCandidate *)0)->unknown_28 == 0x28 ? 1 : -1];
typedef char kf_target_candidate_unknown_2a_offset[(u32)&((KfTargetCandidate *)0)->unknown_2a == 0x2a ? 1 : -1];

/* Type 25 reads a variable halfword stream after this proved prefix. Its
 * complete allocation and record stride are not established. */
typedef struct KfTargetCandidateAction25 {
    u8 type;
    u8 unknown_01[3];
    u8 unknown_04;
    u8 unknown_05[3];
    u16 unknown_08;
    u16 unknown_0a;
    KfTargetCandidateWord0c word_0c;
    KfTargetCandidateWord0e word_0e;
    KfTargetCandidateWord10 word_10;
    KfTargetCandidateWord12 word_12;
    KfTargetCandidateWord14 word_14;
    KfTargetCandidateWord16 word_16;
    KfTargetCandidateWord18 word_18;
    u16 stream[1]; /* first word of a variable-length archive payload */
} KfTargetCandidateAction25;
typedef char kf_target_candidate_action25_prefix_size[
    sizeof(KfTargetCandidateAction25) == 0x1c ? 1 : -1];
typedef char kf_target_candidate_action25_stream_offset[
    (u32)&((KfTargetCandidateAction25 *)0)->stream == 0x1a ? 1 : -1];

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
    u16 unknown_30;
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
typedef char kf_target_group_unknown_30_offset[
    (u32)&((KfTargetGroup *)0)->unknown_30 == 0x30 ? 1 : -1];

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

typedef struct KfActorState70Bytes {
    u8 low;
    u8 high;
} KfActorState70Bytes;
typedef char kf_actor_state_70_bytes_size[
    sizeof(KfActorState70Bytes) == 2 ? 1 : -1];

typedef union KfActorState70 {
    s16 signed_state;
    KfActorState70Bytes bytes;
} KfActorState70;
typedef char kf_actor_state_70_size[sizeof(KfActorState70) == 2 ? 1 : -1];

typedef struct KfActorOrientation {
    struct KfEulerAngles rotation;
    u8 unknown_46[2];
} KfActorOrientation;
typedef char kf_actor_orientation_size[sizeof(KfActorOrientation) == 8 ? 1 : -1];

typedef struct KfActorTail72Motion {
    struct KfEulerAngles angles;
    s16 baseline;
} KfActorTail72Motion;
typedef char kf_actor_tail_72_motion_size[
    sizeof(KfActorTail72Motion) == 8 ? 1 : -1];

typedef struct KfActorTail72Script {
    u16 word_index;
    u16 unknown_74;
} KfActorTail72Script;
typedef char kf_actor_tail_72_script_size[
    sizeof(KfActorTail72Script) == 4 ? 1 : -1];

/* Action-specific tail storage overlaps a vector, signed state, and script words. */
typedef union KfActorTail72 {
    s16 signed_state;
    u16 unsigned_state;
    struct KfEulerAngles angles;
    SVECTOR direction;
    KfActorTail72Motion motion;
    KfActorTail72Script script;
} KfActorTail72;
typedef char kf_actor_tail_72_size[sizeof(KfActorTail72) == 8 ? 1 : -1];
typedef char kf_actor_tail_72_baseline_offset[
    (u32)&((KfActorTail72 *)0)->motion.baseline == 6 ? 1 : -1];

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
    s16 unknown_64;
    s16 animation_step;
    s16 unknown_68;
    s16 unknown_6a;
    s16 unknown_6c;
    u8 unknown_6e[2];
    KfActorState70 state_70;
    KfActorTail72 tail_72;
    u8 unknown_7a[2];
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
typedef char kf_actor_unknown_64_offset[(u32)&((KfActor *)0)->unknown_64 == 0x64 ? 1 : -1];
typedef char kf_actor_step_offset[(u32)&((KfActor *)0)->animation_step == 0x66 ? 1 : -1];
typedef char kf_actor_motion_result_68_offset[(u32)&((KfActor *)0)->unknown_68 == 0x68 ? 1 : -1];
typedef char kf_actor_motion_result_6a_offset[(u32)&((KfActor *)0)->unknown_6a == 0x6a ? 1 : -1];
typedef char kf_actor_motion_result_6c_offset[(u32)&((KfActor *)0)->unknown_6c == 0x6c ? 1 : -1];
typedef char kf_actor_state_70_offset[(u32)&((KfActor *)0)->state_70 == 0x70 ? 1 : -1];
typedef char kf_actor_state_71_offset[(u32)&((KfActor *)0)->state_70.bytes.high == 0x71 ? 1 : -1];
typedef char kf_actor_tail_72_offset[(u32)&((KfActor *)0)->tail_72 == 0x72 ? 1 : -1];
typedef char kf_actor_tail_72_signed_offset[(u32)&((KfActor *)0)->tail_72.signed_state == 0x72 ? 1 : -1];
typedef char kf_actor_tail_72_unsigned_offset[(u32)&((KfActor *)0)->tail_72.unsigned_state == 0x72 ? 1 : -1];
typedef char kf_actor_tail_72_angles_offset[(u32)&((KfActor *)0)->tail_72.angles == 0x72 ? 1 : -1];
typedef char kf_actor_tail_72_script_index_offset[(u32)&((KfActor *)0)->tail_72.script.word_index == 0x72 ? 1 : -1];
typedef char kf_actor_tail_72_script_word_74_offset[(u32)&((KfActor *)0)->tail_72.script.unknown_74 == 0x74 ? 1 : -1];
typedef char kf_actor_unknown_78_offset[(u32)&((KfActor *)0)->tail_72.motion.baseline == 0x78 ? 1 : -1];
typedef char kf_actor_unknown_7a_offset[(u32)&((KfActor *)0)->unknown_7a == 0x7a ? 1 : -1];

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
