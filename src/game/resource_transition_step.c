#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_object.h>
#include <kf/game/map_placed.h>
#include <kf/game/memory.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <psyq/audio.h>

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
    case 0:
        if (state_8017d118.values_10[0] == 255) {
            goto phase_three;
        }
        state_8017d118.transition_phase = 0xf0;
        object = map_object_state.objects;
        index = KF_MAP_OBJECT_CAPACITY - 1;
        do {
            object->action = 255;
            index--;
            object++;
        } while (index != -1);
        cd_archive_queue_read(5, state_8017d118.values_10[0] * 3,
            (u_long *)cd_stream_work_buffer,
            (KfCdRequestCallback)resource_transition_set_phase_1);
        return;

    case 1:
        buffer = cd_stream_work_buffer;
        state_8017d118.transition_phase = 0xf0;
        /* Retail's default table contains 32 pointers to the no-op callback. */
        state_8017d118.active_table = callback_default_table;
        cd_archive_queue_read(5, state_8017d118.values_10[0] * 3 + 2,
            (u_long *)0x8019e138,
            (KfCdRequestCallback)resource_transition_set_phase_2);
        resource_copy_words((u32 *)&bss_801c7540, (u32 *)(buffer + 4), 0x3e80);
        buffer += *(u32 *)buffer + 4;
        resource_copy_words((u32 *)((u8 *)&bss_801c7540 + 0x10000),
            (u32 *)(buffer + 4), 0x600);
        if (state_8017d118.values_17[0] != 127) {
            state_8017d118.unknown_15 = 1;
            translate_active_world_positions(state_8017d118.values_17[0] << 11,
                -state_8017d118.values_17[2] * 128,
                state_8017d118.values_17[1] << 11);
        }
        state_8017d118.unknown_09[0] = state_8017d118.values_10[0];
        return;

    case 2:
        state_8017d118.transition_phase = 0xf0;
        cd_archive_queue_read(5, state_8017d118.values_10[0] * 3 + 1,
            (u_long *)cd_stream_work_buffer,
            (KfCdRequestCallback)resource_transition_set_phase_3);
        state_8017d118.active_table = (KfCallback *)0x8019e138;
        return;

    case 3:
phase_three:
        if (state_8017d118.values_10[1] != 255) {
            state_8017d118.transition_phase = 0xf0;
            cd_archive_queue_read(1, state_8017d118.values_10[1],
                (u_long *)0x8012da68,
                (KfCdRequestCallback)resource_transition_set_phase_4);
        }
        if (state_8017d118.values_10[0] != 255) {
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
                if (object->tail.fields.unknown_34 != 0) {
                    pool_record_release((KfPoolRecord *)object->tail.fields.unknown_34);
                }
                if (object->action == 0x20) {
                    memory_free((u8 *)object->extra_40.record);
                    object->action = 255;
                }
                object->object_id = 255;
                index--;
                object++;
            } while (index != -1);
            if (state_8017d118.values_17[0] != 127) {
                KfMapOccupancyCell *cell = &bss_801c7540.map_cells[0][0];
                index = 0x1900;
                do {
                    cell->layer[0].quarter_turns &= 3;
                    cell->layer[1].quarter_turns &= 3;
                    index--;
                    cell++;
                } while (index != 0);
                map_cell_add_layer_occupancy(player_state.camera_position.vx,
                    player_state.camera_position.vz, 800, 1);
            }
            resource_copy_words((u32 *)actor_state.target_groups,
                (u32 *)(stream + 4), 0xcb0);
            actor_fixup_group_targets();
            stream += *(u32 *)stream + 4;
            actor_load_records((const struct KfActorLoadRecord *)(stream + 4));
            stream += *(u32 *)stream + 4;
            map_object_initialize_from_placements((KfMapObjectPlacement *)(stream + 4));
            stream += *(u32 *)stream + 4;
            map_placed_expand_sources((KfMapPlacedSource *)(stream + 4));
            event_world_state_restore_slot(state_8017d118.values_10[0]);
            state_8017d118.active_table[5]();
            player_state.unknown_09[1] = 1;
        }
        if (state_8017d118.values_10[1] != 255) return;

    case 4:
        if (state_8017d118.values_10[2] != 255) {
            cd_map_stream_read(2, state_8017d118.values_10[2]);
        }
        state_8017d118.flag_16 = 0;
        if (state_8017d118.values_10[3] != 255 ||
            state_8017d118.values_10[4] != 255) {
            goto begin_phase_five;
        }
        goto complete;

complete:
        if (state_8017d118.values_10[0] != 255)
            state_8017d118.values_04[0] = state_8017d118.values_10[0];
        if (state_8017d118.values_10[1] != 255)
            state_8017d118.values_04[1] = state_8017d118.values_10[1];
        if (state_8017d118.values_10[2] != 255)
            state_8017d118.values_04[2] = state_8017d118.values_10[2];
        if (state_8017d118.values_10[3] != 255)
            state_8017d118.values_04[3] = state_8017d118.values_10[3];
        if (state_8017d118.values_10[4] != 255)
            state_8017d118.values_04[4] = state_8017d118.values_10[4];
        state_8017d118.transition_active = 0;
        return;

begin_phase_five:
        state_8017d118.transition_phase = 5;
        state_8017d118.unknown_1a = 60;
        audio_state.sequence_ready = 0;

    case 5:
        if (audio_state.sequence_active != 0) {
            state_8017d118.unknown_1a -= 2;
            if ((s16)state_8017d118.unknown_1a <= 0) {
                state_8017d118.unknown_1a = 0;
            }
            SsSeqSetVol(audio_state.sequence_id,
                (s16)state_8017d118.unknown_1a,
                (s16)state_8017d118.unknown_1a);
            if (state_8017d118.unknown_1a != 0) return;
            SsSeqStop(audio_state.sequence_id);
            SsSeqClose(audio_state.sequence_id);
            audio_state.sequence_active = 0;
        }
        if (audio_state.vab_slots[1].vab_id != -1) {
            audio_state.vab_slots[1].stream_slot = 0;
        }
        if (state_8017d118.values_10[3] != 255) {
            audio_queue_vab_stream(4, state_8017d118.values_10[3] + 1, 1);
        }
        if (state_8017d118.values_10[4] != 255) {
            cd_archive_queue_read(4,
                state_8017d118.values_10[4] % 100 + 320,
                audio_state.sequence_buffer,
                (KfCdRequestCallback)resource_transition_set_phase_6);
            state_8017d118.transition_phase = 0xf0;
            return;
        }
        goto complete;

    case 6:
        if (state_8017d118.values_10[4] < 100) {
            audio_state.sequence_ready = 1;
            audio_start_sequence();
        }
        goto complete;
    }
}

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
