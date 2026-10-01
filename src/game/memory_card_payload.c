#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/card_payload.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <psyq/libc.h>

ADDRESS(0x80048d24, 0x5b8)
void func_80048d24(u8 *buffer)
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
    saved->unknown_128 = player_state.unknown_128;
    saved->vitals.maximum_hp = player_state.vitals.maximum_hp;
    saved->vitals.current_hp = player_state.vitals.current_hp;
    saved->vitals.maximum_mp = player_state.vitals.maximum_mp;
    saved->vitals.current_mp = player_state.vitals.current_mp;
    saved->base_physical_power = player_state.base_physical_power;
    saved->base_magic = player_state.base_magic;
    saved->physical_power_training = player_state.physical_power_training;
    saved->magic_training = player_state.magic_training;
    saved->unknown_54 = player_state.unknown_54;
    saved->curse_strength = player_state.curse_strength;
    saved->unknown_58 = player_state.unknown_58;
    saved->unknown_5a = player_state.unknown_5a;
    saved->unknown_5c = player_state.unknown_5c;
    saved->unknown_5e = player_state.unknown_5e;
    saved->unknown_60 = player_state.unknown_60;
    saved->unknown_62 = player_state.unknown_62;
    saved->unknown_64 = player_state.unknown_64;
    saved->unknown_66 = player_state.unknown_66;
    saved->unknown_68 = player_state.unknown_68;
    saved->unknown_6a = player_state.unknown_6a;
    saved->unknown_6c = player_state.unknown_6c;
    saved->unknown_6e = player_state.unknown_6e;
    saved->level = player_state.level;
    saved->unknown_09 = player_state.unknown_09[0];
    saved->equipped_ids[0] = player_state.equipped_head_id;
    saved->equipped_ids[1] = player_state.equipped_body_id;
    saved->equipped_ids[2] = player_state.equipped_arm_id;
    saved->equipped_ids[3] = player_state.equipped_leg_id;
    saved->equipped_ids[4] = player_state.equipped_shield_id;
    saved->equipped_ids[5] = player_state.equipped_accessory_id;
    saved->equipped_ids[6] = player_state.equipped_extra_id;
    saved->unknown_97 = player_state.unknown_97;
    saved->unknown_98 = player_state.unknown_98;
    saved->unknown_99 = player_state.unknown_99;
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
void func_800492dc(const u8 *buffer)
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
    player_state.unknown_128 = saved->unknown_128;
    player_state.vitals.maximum_hp = saved->vitals.maximum_hp;
    player_state.vitals.current_hp = saved->vitals.current_hp;
    player_state.vitals.maximum_mp = saved->vitals.maximum_mp;
    player_state.vitals.current_mp = saved->vitals.current_mp;
    player_state.base_physical_power = saved->base_physical_power;
    player_state.base_magic = saved->base_magic;
    player_state.physical_power_training = saved->physical_power_training;
    player_state.magic_training = saved->magic_training;
    player_state.unknown_54 = saved->unknown_54;
    player_state.curse_strength = saved->curse_strength;
    player_state.unknown_58 = saved->unknown_58;
    player_state.unknown_5a = saved->unknown_5a;
    player_state.unknown_5c = saved->unknown_5c;
    player_state.unknown_5e = saved->unknown_5e;
    player_state.unknown_60 = saved->unknown_60;
    player_state.unknown_62 = saved->unknown_62;
    player_state.unknown_64 = saved->unknown_64;
    player_state.unknown_66 = saved->unknown_66;
    player_state.unknown_68 = saved->unknown_68;
    player_state.unknown_6a = saved->unknown_6a;
    player_state.unknown_6c = saved->unknown_6c;
    player_state.unknown_6e = saved->unknown_6e;
    player_state.level = saved->level;
    player_state.unknown_09[0] = saved->unknown_09;
    player_state.equipped_head_id = saved->equipped_ids[0];
    player_state.equipped_body_id = saved->equipped_ids[1];
    player_state.equipped_arm_id = saved->equipped_ids[2];
    player_state.equipped_leg_id = saved->equipped_ids[3];
    player_state.equipped_shield_id = saved->equipped_ids[4];
    player_state.equipped_accessory_id = saved->equipped_ids[5];
    player_state.equipped_extra_id = saved->equipped_ids[6];
    player_state.unknown_97 = saved->unknown_97;
    player_state.unknown_98 = saved->unknown_98;
    player_state.unknown_99 = saved->unknown_99;
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
