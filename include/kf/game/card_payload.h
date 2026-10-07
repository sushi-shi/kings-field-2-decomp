#ifndef KF_GAME_CARD_PAYLOAD_H
#define KF_GAME_CARD_PAYLOAD_H

#include <kf/game/card.h>
#include <kf/game/player.h>

enum { KF_CARD_SAVE_ROTATION_OFFSET = 14820 };

/* The payload's odd byte offsets are part of the on-card format. The
 * unassigned bytes are left zero by the card writer. */
typedef struct KfCardPlayerSnapshot {
    s32 experience;
    s32 next_level_experience;
    u32 gold;
    u16 map_layer_index;
    KfPlayerVitals vitals;
    u16 base_physical_power;
    u16 base_magic;
    u16 physical_power_training;
    u16 magic_training;
    s16 poison_timer;
    s16 curse_strength;
    u16 curse_phase_limit;
    s16 darkness_phase;
    u16 darkness_phase_limit;
    s16 slow_timer;
    s16 paralysis_timer;
    s16 defense_boost_timer;
    s16 attack_boost_timer;
    s16 magic_tint_phase;
    s16 magic_tint_phase_limit;
    s16 map_marker_visual_effect_timer;
    s16 full_mp_timer;
    s16 magic_boost_timer;
    u8 unused_3a[8];
    u8 level;
    u8 unknown_09;
    KF_ENUM_STORAGE(KfObjectId, u8) equipped_ids[7];
    KfEffectKind primary_magic_shortcut_id;
    KfEffectKind secondary_magic_shortcut_id;
    KF_ENUM_STORAGE(KfObjectId, u8) secondary_item_shortcut_id;
    KF_ENUM_STORAGE(KfObjectId, u8) equipped_weapon_id;
    u8 audio_effects_enabled;
    u8 audio_music_enabled;
    u8 hud_gauges_enabled;
    u8 compass_enabled;
    u8 item_preview_enabled;
    u8 walking_bob_enabled;
} KfCardPlayerSnapshot;

typedef struct KfCardSavePayload {
    u8 active_resource_ids[5];
    u8 event_control[0x100];
    u8 event_arena[0x3800];
    u8 unused_3905;
    u16 saved_event_offsets[10];
    u8 game_counters[0x78];
    u8 magic_menu_available[64];
    u8 unused_39d2[2];
    VECTOR camera_position;
    KfPlayerViewRotation camera_rotation_target;
    KfCardPlayerSnapshot player;
    u8 unused_tail[444];
} KfCardSavePayload;

typedef char kf_card_snapshot_level_offset[
    (u32)&((KfCardPlayerSnapshot *)0)->level == 66 ? 1 : -1];
typedef char kf_card_snapshot_size[sizeof(KfCardPlayerSnapshot) == 88 ? 1 : -1];
typedef char kf_card_save_control_offset[
    (u32)&((KfCardSavePayload *)0)->event_control == 5 ? 1 : -1];
typedef char kf_card_save_arena_offset[
    (u32)&((KfCardSavePayload *)0)->event_arena == 261 ? 1 : -1];
typedef char kf_card_save_offsets_offset[
    (u32)&((KfCardSavePayload *)0)->saved_event_offsets == 14598 ? 1 : -1];
typedef char kf_card_save_counters_offset[
    (u32)&((KfCardSavePayload *)0)->game_counters == 14618 ? 1 : -1];
typedef char kf_card_save_magic_offset[
    (u32)&((KfCardSavePayload *)0)->magic_menu_available == 14738 ? 1 : -1];
typedef char kf_card_save_position_offset[
    (u32)&((KfCardSavePayload *)0)->camera_position == 14804 ? 1 : -1];
typedef char kf_card_save_rotation_offset[
    (u32)&((KfCardSavePayload *)0)->camera_rotation_target ==
    KF_CARD_SAVE_ROTATION_OFFSET ? 1 : -1];
typedef char kf_card_save_player_offset[
    (u32)&((KfCardSavePayload *)0)->player == 14828 ? 1 : -1];
typedef char kf_card_save_payload_size[
    sizeof(KfCardSavePayload) == KF_CARD_PAYLOAD_BYTES ? 1 : -1];

#endif
