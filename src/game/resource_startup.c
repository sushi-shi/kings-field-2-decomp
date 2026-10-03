#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/asset.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/graphics.h>
#include <kf/game/map_object.h>
#include <kf/game/memory.h>
#include <kf/game/player.h>
#include <kf/game/resources.h>
#include <kf/game/event_state.h>
#include <psyq/kernel.h>
#include <kf/game/tmd.h>

RODATA(0x80011000, 0x53)

ADDRESS(0x80015d50, 0x8)
/* The default table serves callback slots with different caller arguments. */
void resource_noop_callback()
{
}

ADDRESS(0x80015d58, 0x27c)
void resource_initialize_game_assets(void)
{
    u8 *source = (u8 *)KF_GAME_RESOURCE_ARENA_BASE;
    u8 *second_value = &state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD];

    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] = 255;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] = 255;
    *second_value = 255;
    state_8017d118.transition_active = 0;
    state_8017d118.transition_phase = 0;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = 0;
    *second_value = 0;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] = 0;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] = 0;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = 0;
    state_8017d118.current_map_region_id = 0;

    cd_archive_open(KF_RESOURCE_ARCHIVE_MO, "COM\\MO.T");
    cd_archive_open(KF_RESOURCE_ARCHIVE_TALK, "COM\\TALK.T");
    cd_archive_open(KF_RESOURCE_ARCHIVE_VAB, "COM\\VAB.T");
    cd_archive_open(KF_RESOURCE_ARCHIVE_FDAT, "COM\\FDAT.T");
    cd_archive_open(KF_RESOURCE_ARCHIVE_RTIM, "COM\\RTIM.T");
    cd_archive_open(KF_RESOURCE_ARCHIVE_RTMD, "COM\\RTMD.T");
    cd_archive_open(KF_RESOURCE_ARCHIVE_ITEM, "COM\\ITEM.T");

    /* The read arena and three copy destinations still lack full source owners. */
    cd_archive_read(KF_RESOURCE_ARCHIVE_FDAT, 0x30, (u_long *)source);
    cd_map_stream_read(KF_RESOURCE_ARCHIVE_FDAT, 0x2f);
    audio_queue_vab_stream(KF_RESOURCE_ARCHIVE_VAB, 0, 0);

    resource_copy_words((u32 *)&map_object_state, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)player_weapon_records, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)0x801d8d88, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)player_level_growth_table, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)&effect_state, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)collision_default_rows, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)audio_state.voices.params, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)0x800fa0d0, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    asset_registry_load_tmd_archive(0, (u8 *)0x800fa0d0);
    resource_copy_words((u32 *)0x800855a0, (u32 *)(source + 4), *(u32 *)source >> 2);
    asset_registry_load_tmd_archive(0x28, (u8 *)0x800855a0);
    game_initialize_session();
}

ADDRESS(0x80015fd4, 0x114)
void resource_run_initial_transition(void)
{
    u8 first;
    u8 second;
    u8 third;
    u8 fourth;
    u8 fifth;

    state_8017d118.transition_active = 1;
    state_8017d118.transition_phase = 0;
    first = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION];
    second = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD];
    third = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM];
    fourth = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB];
    fifth = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.transition_offset.x = KF_RESOURCE_OFFSET_NO_SHIFT;
    state_8017d118.transition_offset.z = KF_RESOURCE_OFFSET_NO_SHIFT;
    state_8017d118.transition_offset.y = KF_RESOURCE_OFFSET_NO_SHIFT;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = first;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] = second;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] = third;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] = fourth;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = fifth;
    do {
        cd_request_yield();
        resource_advance_transition();
    } while (state_8017d118.transition_active != 0);
    tmd_set_slot(0, (KfTmdHeader *)0x8012da68);
    state_8017d118.active_table[5]();
}

ADDRESS(0x800160e8, 0x178)
void translate_active_world_positions(s32 dx, s32 dy, s32 dz)
{
    KfEffectRecord *effect = effect_state.records;
    KfMapObject *object;
    KfActor *actor;
    u16 effect_remaining;
    u16 object_remaining;
    u16 actor_remaining;

    player_state.camera_position.vx += dx;
    player_state.camera_position.vz += dz;
    player_state.camera_position.vy += dy;

    effect_remaining = KF_EFFECT_CAPACITY - 1;
    do {
        if (effect->type != KF_EFFECT_SLOT_FREE &&
            (effect->render_flags & KF_EFFECT_RENDER_TRANSFORM_MASK) !=
                KF_EFFECT_RENDER_SCREEN_SPACE) {
            effect->position.vx += dx;
            effect->position.vz += dz;
            effect->position.vy += dy;
        }
        effect++;
    } while (effect_remaining-- != 0);

    object = map_object_state.objects;
    object_remaining = KF_MAP_OBJECT_CAPACITY - 1;
    do {
        if (object->object_id != KF_MAP_OBJECT_ID_NONE) {
            object->position.vx += dx;
            object->position.vz += dz;
            object->position.vy += dy;
        }
        object++;
    } while (object_remaining-- != 0);

    actor = actor_state.actors;
    actor_remaining = KF_ACTOR_CAPACITY - 1;
    do {
        if (actor->slot_state != KF_ACTOR_SLOT_FREE) {
            actor->position.vx += dx;
            actor->position.vz += dz;
            actor->position.vy += dy;
        }
        actor++;
    } while (actor_remaining-- != 0);
}

DATA(0x8017d118, 0x1c)
KfState8017d118 state_8017d118;

ADDRESS(0x80016260, 0x55c)
void resource_request_transition(u8 map_region_id, u8 tmd_id, u8 tim_id, u8 vab_id,
                    u8 sequence_id, s8 offset_x, s8 offset_z, s8 offset_y)
{
    u8 current_map_region_id;
    u8 current_tmd_id;
    u8 current_tim_id;
    u8 current_vab_id;
    u8 current_sequence_id;
    u8 prior_map_region_id;
    u8 prior_tmd_id;
    u8 prior_tim_id;
    u8 prior_vab_id;
    u8 prior_sequence_id;

    if (sequence_id == KF_RESOURCE_REQUEST_START_SEQUENCE) {
        if (audio_state.sequence_active == 0) {
            audio_start_sequence();
        }
        return;
    }

    if (map_region_id == KF_RESOURCE_REQUEST_KEEP) {
        current_map_region_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION];
        prior_map_region_id = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION];
        if (tmd_id == KF_RESOURCE_REQUEST_KEEP) {
            current_tmd_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD];
            prior_tmd_id = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD];
        } else {
            current_tmd_id = tmd_id;
            prior_tmd_id = tmd_id;
        }
        if (tim_id == KF_RESOURCE_REQUEST_KEEP) {
            current_tim_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM];
            prior_tim_id = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM];
        } else {
            current_tim_id = tim_id;
            prior_tim_id = tim_id;
        }
        if (vab_id == KF_RESOURCE_REQUEST_KEEP) {
            current_vab_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB];
            prior_vab_id = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB];
        } else {
            current_vab_id = vab_id;
            prior_vab_id = vab_id;
        }
        if (sequence_id == KF_RESOURCE_REQUEST_KEEP) {
            current_sequence_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
            prior_sequence_id = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
        } else {
            current_sequence_id = sequence_id;
            prior_sequence_id = sequence_id;
        }
    } else {
        current_map_region_id = map_region_id;
        prior_map_region_id = map_region_id;
        prior_tmd_id = map_region_id;
        prior_tim_id = map_region_id;
        prior_vab_id = map_region_id;
        prior_sequence_id = map_region_id;
        current_tmd_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD];
        current_tim_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM];
        current_vab_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB];
        current_sequence_id = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
    }

    if (state_8017d118.transition_active != 0) {
        goto handle_active;
    }
    if (state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == current_map_region_id &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD] == current_tmd_id &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] == current_tim_id &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] == current_vab_id &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] == current_sequence_id) {
        return;
    }

apply:
    if (state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] != KF_RESOURCE_ACTIVE_UNINITIALIZED &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] != current_map_region_id) {
        event_world_state_save_slot(state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION]);
    }
    if (event_state.control.fields.highest_requested_map_region_id < current_map_region_id) {
        event_state.control.fields.highest_requested_map_region_id = current_map_region_id;
    }
    state_8017d118.transition_active = 1;
    state_8017d118.transition_phase = 0;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = map_region_id;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] = tmd_id;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] = tim_id;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] = vab_id;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = sequence_id;
    state_8017d118.transition_offset.x = offset_x;
    state_8017d118.transition_offset.z = offset_z;
    state_8017d118.world_shift_applied = 0;
    state_8017d118.transition_offset.y = offset_y;
    if (tmd_id == KF_RESOURCE_REQUEST_KEEP) {
        state_8017d118.tmd_object_limit_active = 0;
    } else {
        state_8017d118.tmd_object_limit_active = 1;
    }
    return;

handle_active:
    if ((state_8017d118.transition_active != 1 ||
         state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == prior_map_region_id) &&
        state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] == prior_tmd_id &&
        state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] == prior_tim_id &&
        state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] == prior_vab_id &&
        state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] == prior_sequence_id) {
        return;
    }
    if ((state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == current_map_region_id) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD] == tmd_id) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] == tim_id) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] == vab_id) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] == sequence_id)) {
        return;
    }

    if ((state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] != KF_RESOURCE_REQUEST_KEEP &&
         map_region_id == KF_RESOURCE_REQUEST_KEEP) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] != KF_RESOURCE_REQUEST_KEEP &&
         tmd_id == KF_RESOURCE_REQUEST_KEEP) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] != KF_RESOURCE_REQUEST_KEEP &&
         tim_id == KF_RESOURCE_REQUEST_KEEP) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] != KF_RESOURCE_REQUEST_KEEP &&
         vab_id == KF_RESOURCE_REQUEST_KEEP) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] != KF_RESOURCE_REQUEST_KEEP &&
         sequence_id == KF_RESOURCE_REQUEST_KEEP)) {
        while (state_8017d118.transition_active != 0) {
            cd_request_yield();
            resource_advance_transition();
        }
    } else {
        do {
            EnterCriticalSection();
            if (state_8017d118.transition_phase != KF_RESOURCE_TRANSITION_PHASE_PENDING_IO) break;
            ExitCriticalSection();
            cd_request_yield();
        } while (1);
    }
    ExitCriticalSection();
    if (state_8017d118.world_shift_applied != 0 && map_region_id != KF_RESOURCE_REQUEST_KEEP &&
        offset_x == KF_RESOURCE_OFFSET_NO_SHIFT) {
        offset_x = -state_8017d118.transition_offset.x;
        offset_y = -state_8017d118.transition_offset.y;
        offset_z = -state_8017d118.transition_offset.z;
    }
    goto apply;
}
