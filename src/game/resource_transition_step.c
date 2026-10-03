#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/cd.h>
#include <kf/game/collision_cache.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_object.h>
#include <kf/game/map_placed.h>
#include <kf/game/memory.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <kf/game/resources.h>
#include <psyq/audio.h>

enum {
    RESOURCE_STEP_BEGIN_MAP = 0,
    RESOURCE_STEP_LOAD_MAP_CELLS = 1,
    RESOURCE_STEP_QUEUE_MAP_ACTORS = 2,
    RESOURCE_STEP_LOAD_MAP_ACTORS = 3,
    RESOURCE_STEP_QUEUE_TIM = 4,
    RESOURCE_STEP_FADE_AUDIO = 5,
    RESOURCE_STEP_FINISH_AUDIO = 6,
    RESOURCE_MAP_CELL_QUARTER_TURN_MASK = 3,
    RESOURCE_SEQUENCE_FADE_START_VOLUME = 60,
    RESOURCE_SEQUENCE_FADE_STEP = 2
};

DATA(0x80063e00, 0x80)
KfCallback callback_default_table[32] = {
    resource_noop_callback, resource_noop_callback, resource_noop_callback, resource_noop_callback,
    resource_noop_callback, resource_noop_callback, resource_noop_callback, resource_noop_callback,
    resource_noop_callback, resource_noop_callback, resource_noop_callback, resource_noop_callback,
    resource_noop_callback, resource_noop_callback, resource_noop_callback, resource_noop_callback,
    resource_noop_callback, resource_noop_callback, resource_noop_callback, resource_noop_callback,
    resource_noop_callback, resource_noop_callback, resource_noop_callback, resource_noop_callback,
    resource_noop_callback, resource_noop_callback, resource_noop_callback, resource_noop_callback,
    resource_noop_callback, resource_noop_callback, resource_noop_callback, resource_noop_callback,
};

DATA(0x8019e138, 0x14000)
static u8 resource_callback_table_workspace[0x14000];

ADDRESS(0x800167bc, 0x14)
void resource_transition_set_phase_1(void)
{
    state_8017d118.transition_phase = RESOURCE_STEP_LOAD_MAP_CELLS;
}

ADDRESS(0x800167d0, 0x14)
void resource_transition_set_phase_3(void)
{
    state_8017d118.transition_phase = RESOURCE_STEP_LOAD_MAP_ACTORS;
}

ADDRESS(0x800167e4, 0x14)
void resource_transition_set_phase_2(void)
{
    state_8017d118.transition_phase = RESOURCE_STEP_QUEUE_MAP_ACTORS;
}

ADDRESS(0x800167f8, 0x14)
void resource_transition_set_phase_4(void)
{
    state_8017d118.transition_phase = RESOURCE_STEP_QUEUE_TIM;
}

ADDRESS(0x8001680c, 0x14)
void resource_transition_set_phase_6(void)
{
    state_8017d118.transition_phase = RESOURCE_STEP_FINISH_AUDIO;
}

RODATA(0x80011058, 0x1c)

ADDRESS(0x80016820, 0x6b4)
void resource_advance_transition(void)
{
    u8 *buffer;
    u8 *stream;
    KfActor *actor;
    KfMapObject *object;
    s32 phase;
    s32 index;

    if (state_8017d118.transition_active == 0) {
        return;
    }
    if (state_8017d118.transition_active != 1) {
        return;
    }
    phase = state_8017d118.transition_phase;

    switch (phase) {
    case RESOURCE_STEP_BEGIN_MAP:
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == KF_RESOURCE_REQUEST_KEEP) {
            goto phase_three;
        }
        state_8017d118.transition_phase = KF_RESOURCE_TRANSITION_PHASE_PENDING_IO;
        object = map_object_state.objects;
        index = KF_MAP_OBJECT_CAPACITY - 1;
        do {
            object->action = KF_MAP_OBJECT_ACTION_NONE;
            index--;
            object++;
        } while (index != -1);
        cd_archive_queue_read(KF_RESOURCE_ARCHIVE_FDAT, state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] * 3,
            (u_long *)cd_stream_work_buffer,
            (KfCdRequestCallback)resource_transition_set_phase_1);
        return;

    case RESOURCE_STEP_LOAD_MAP_CELLS:
        buffer = cd_stream_work_buffer;
        state_8017d118.transition_phase = KF_RESOURCE_TRANSITION_PHASE_PENDING_IO;
        /* Retail's default table contains 32 pointers to the no-op callback. */
        state_8017d118.active_table = callback_default_table;
        cd_archive_queue_read(KF_RESOURCE_ARCHIVE_FDAT, state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] * 3 + 2,
            (u_long *)resource_callback_table_workspace,
            (KfCdRequestCallback)resource_transition_set_phase_2);
        resource_copy_words((u32 *)&bss_801c7540, (u32 *)(buffer + 4),
                            sizeof(bss_801c7540.map_cells) / sizeof(u32));
        buffer += *(u32 *)buffer + 4;
        resource_copy_words((u32 *)KF_COLLISION_SHAPE_BANK,
            (u32 *)(buffer + 4), KF_COLLISION_SHAPE_BANK_BYTES / sizeof(u32));
        if (state_8017d118.transition_offset.x != KF_RESOURCE_OFFSET_NO_SHIFT) {
            state_8017d118.world_shift_applied = 1;
            translate_active_world_positions(state_8017d118.transition_offset.x << KF_MAP_CELL_POSITION_SHIFT,
                -state_8017d118.transition_offset.y * 128,
                state_8017d118.transition_offset.z << KF_MAP_CELL_POSITION_SHIFT);
        }
        state_8017d118.current_map_region_id = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION];
        return;

    case RESOURCE_STEP_QUEUE_MAP_ACTORS:
        state_8017d118.transition_phase = KF_RESOURCE_TRANSITION_PHASE_PENDING_IO;
        cd_archive_queue_read(KF_RESOURCE_ARCHIVE_FDAT, state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] * 3 + 1,
            (u_long *)cd_stream_work_buffer,
            (KfCdRequestCallback)resource_transition_set_phase_3);
        state_8017d118.active_table = (KfCallback *)resource_callback_table_workspace;
        return;

    case RESOURCE_STEP_LOAD_MAP_ACTORS:
phase_three:
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] != KF_RESOURCE_REQUEST_KEEP) {
            state_8017d118.transition_phase = KF_RESOURCE_TRANSITION_PHASE_PENDING_IO;
            cd_archive_queue_read(KF_RESOURCE_ARCHIVE_RTMD, state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD],
                (u_long *)resource_tmd_workspace,
                (KfCdRequestCallback)resource_transition_set_phase_4);
        }
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] != KF_RESOURCE_REQUEST_KEEP) {
            stream = cd_stream_work_buffer;
            actor = actor_state.actors;
            index = KF_ACTOR_CAPACITY - 1;
            do {
                if (actor->animation_cache != 0) {
                    pool_record_release(actor->animation_cache);
                }
                actor->slot_state = KF_ACTOR_SLOT_FREE;
                index--;
                actor++;
            } while (index != -1);
            object = map_object_state.objects;
            index = KF_MAP_OBJECT_CAPACITY - 1;
            do {
                if (object->tail.animated.animation_cache != 0) {
                    pool_record_release(object->tail.animated.animation_cache);
                }
                if (object->action == 0x20) {
                    memory_free((u8 *)object->extra_40.record);
                    object->action = KF_MAP_OBJECT_ACTION_NONE;
                }
                object->object_id = KF_MAP_OBJECT_ID_NONE;
                index--;
                object++;
            } while (index != -1);
            if (state_8017d118.transition_offset.x != KF_RESOURCE_OFFSET_NO_SHIFT) {
                KfMapOccupancyCell *cell = &bss_801c7540.map_cells[0][0];
                index = KF_MAP_WORLD_GRID_SIDE * KF_MAP_WORLD_GRID_SIDE;
                do {
                    cell->layer[0].quarter_turns &= RESOURCE_MAP_CELL_QUARTER_TURN_MASK;
                    cell->layer[1].quarter_turns &= RESOURCE_MAP_CELL_QUARTER_TURN_MASK;
                    index--;
                    cell++;
                } while (index != 0);
                map_cell_add_layer_occupancy(player_state.camera_position.vx,
                    player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS, 1);
            }
            resource_copy_words((u32 *)actor_state.target_groups,
                (u32 *)(stream + 4),
                (sizeof(actor_state.target_groups) + sizeof(actor_state.unknown_73a0)) /
                    sizeof(u32));
            actor_fixup_group_targets();
            stream += *(u32 *)stream + 4;
            actor_load_records((const struct KfActorLoadRecord *)(stream + 4));
            stream += *(u32 *)stream + 4;
            map_object_initialize_from_placements((KfMapObjectPlacement *)(stream + 4));
            stream += *(u32 *)stream + 4;
            map_placed_expand_sources((KfMapPlacedSource *)(stream + 4));
            event_world_state_restore_slot(state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION]);
            state_8017d118.active_table[5]();
            player_state.force_actor_lifecycle_refresh = 1;
        }
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] != KF_RESOURCE_REQUEST_KEEP) return;

    case RESOURCE_STEP_QUEUE_TIM:
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] != KF_RESOURCE_REQUEST_KEEP) {
            cd_map_stream_read(KF_RESOURCE_ARCHIVE_RTIM, state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM]);
        }
        state_8017d118.tmd_object_limit_active = 0;
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] != KF_RESOURCE_REQUEST_KEEP ||
            state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] != KF_RESOURCE_REQUEST_KEEP) {
            goto begin_phase_five;
        }
        goto complete;

complete:
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] != KF_RESOURCE_REQUEST_KEEP)
            state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] =
                state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION];
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] != KF_RESOURCE_REQUEST_KEEP)
            state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD] =
                state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD];
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] != KF_RESOURCE_REQUEST_KEEP)
            state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] =
                state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM];
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] != KF_RESOURCE_REQUEST_KEEP)
            state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] =
                state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB];
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] != KF_RESOURCE_REQUEST_KEEP)
            state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] =
                state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
        state_8017d118.transition_active = 0;
        return;

begin_phase_five:
        state_8017d118.transition_phase = RESOURCE_STEP_FADE_AUDIO;
        state_8017d118.sequence_fade_volume = RESOURCE_SEQUENCE_FADE_START_VOLUME;
        audio_state.sequence_ready = 0;

    case RESOURCE_STEP_FADE_AUDIO:
        if (audio_state.sequence_active != 0) {
            state_8017d118.sequence_fade_volume -= RESOURCE_SEQUENCE_FADE_STEP;
            if (state_8017d118.sequence_fade_volume <= 0) {
                state_8017d118.sequence_fade_volume = 0;
            }
            SsSeqSetVol(audio_state.sequence_id,
                state_8017d118.sequence_fade_volume,
                state_8017d118.sequence_fade_volume);
            if (state_8017d118.sequence_fade_volume != 0) return;
            SsSeqStop(audio_state.sequence_id);
            SsSeqClose(audio_state.sequence_id);
            audio_state.sequence_active = 0;
        }
        if (audio_state.vab_slots[1].vab_id != -1) {
            audio_state.vab_slots[1].stream_slot = 0;
        }
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] != KF_RESOURCE_REQUEST_KEEP) {
            audio_queue_vab_stream(KF_RESOURCE_ARCHIVE_VAB, state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] + 1, 1);
        }
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] != KF_RESOURCE_REQUEST_KEEP) {
            cd_archive_queue_read(KF_RESOURCE_ARCHIVE_VAB,
                state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] % 100 + 320,
                audio_state.sequence_buffer,
                (KfCdRequestCallback)resource_transition_set_phase_6);
            state_8017d118.transition_phase = KF_RESOURCE_TRANSITION_PHASE_PENDING_IO;
            return;
        }
        goto complete;

    case RESOURCE_STEP_FINISH_AUDIO:
        if (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] < 100) {
            audio_state.sequence_ready = 1;
            audio_start_sequence();
        }
        goto complete;
    }
}
