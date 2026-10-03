#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/event_state.h>
#include <kf/game/resources.h>
#include <psyq/kernel.h>

DATA(0x8017d118, 0x1c)
KfState8017d118 state_8017d118;

ADDRESS(0x80016260, 0x55c)
void resource_request_transition(u8 first, u8 second, u8 third, u8 fourth,
                    u8 fifth, s8 offset_x, s8 offset_z, s8 offset_y)
{
    u8 current_first;
    u8 current_second;
    u8 current_third;
    u8 current_fourth;
    u8 current_fifth;
    u8 prior_first;
    u8 prior_second;
    u8 prior_third;
    u8 prior_fourth;
    u8 prior_fifth;

    if (fifth == KF_RESOURCE_REQUEST_START_SEQUENCE) {
        if (audio_state.sequence_active == 0) {
            audio_start_sequence();
        }
        return;
    }

    if (first == KF_RESOURCE_REQUEST_KEEP) {
        current_first = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION];
        prior_first = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION];
        if (second == KF_RESOURCE_REQUEST_KEEP) {
            current_second = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD];
            prior_second = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD];
        } else {
            current_second = second;
            prior_second = second;
        }
        if (third == KF_RESOURCE_REQUEST_KEEP) {
            current_third = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM];
            prior_third = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM];
        } else {
            current_third = third;
            prior_third = third;
        }
        if (fourth == KF_RESOURCE_REQUEST_KEEP) {
            current_fourth = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB];
            prior_fourth = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB];
        } else {
            current_fourth = fourth;
            prior_fourth = fourth;
        }
        if (fifth == KF_RESOURCE_REQUEST_KEEP) {
            current_fifth = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
            prior_fifth = state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
        } else {
            current_fifth = fifth;
            prior_fifth = fifth;
        }
    } else {
        current_first = first;
        prior_first = first;
        prior_second = first;
        prior_third = first;
        prior_fourth = first;
        prior_fifth = first;
        current_second = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD];
        current_third = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM];
        current_fourth = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB];
        current_fifth = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
    }

    if (state_8017d118.transition_active != 0) {
        goto handle_active;
    }
    if (state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == current_first &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD] == current_second &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] == current_third &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] == current_fourth &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] == current_fifth) {
        return;
    }

apply:
    if (state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] != KF_RESOURCE_ACTIVE_UNINITIALIZED &&
        state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] != current_first) {
        event_world_state_save_slot(state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION]);
    }
    if (event_state.control.fields.highest_requested_map_region_id < current_first) {
        event_state.control.fields.highest_requested_map_region_id = current_first;
    }
    state_8017d118.transition_active = 1;
    state_8017d118.transition_phase = 0;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = first;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] = second;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] = third;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] = fourth;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = fifth;
    state_8017d118.transition_offset.x = offset_x;
    state_8017d118.transition_offset.z = offset_z;
    state_8017d118.world_shift_applied = 0;
    state_8017d118.transition_offset.y = offset_y;
    if (second == KF_RESOURCE_REQUEST_KEEP) {
        state_8017d118.tmd_object_limit_active = 0;
    } else {
        state_8017d118.tmd_object_limit_active = 1;
    }
    return;

handle_active:
    if ((state_8017d118.transition_active != 1 ||
         state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == prior_first) &&
        state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] == prior_second &&
        state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] == prior_third &&
        state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] == prior_fourth &&
        state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] == prior_fifth) {
        return;
    }
    if ((state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] == current_first) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD] == second) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] == third) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] == fourth) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] == KF_RESOURCE_REQUEST_KEEP &&
         state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] == fifth)) {
        return;
    }

    if ((state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] != KF_RESOURCE_REQUEST_KEEP &&
         first == KF_RESOURCE_REQUEST_KEEP) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] != KF_RESOURCE_REQUEST_KEEP &&
         second == KF_RESOURCE_REQUEST_KEEP) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] != KF_RESOURCE_REQUEST_KEEP &&
         third == KF_RESOURCE_REQUEST_KEEP) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] != KF_RESOURCE_REQUEST_KEEP &&
         fourth == KF_RESOURCE_REQUEST_KEEP) ||
        (state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] != KF_RESOURCE_REQUEST_KEEP &&
         fifth == KF_RESOURCE_REQUEST_KEEP)) {
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
    if (state_8017d118.world_shift_applied != 0 && first != KF_RESOURCE_REQUEST_KEEP &&
        offset_x == KF_RESOURCE_OFFSET_NO_SHIFT) {
        offset_x = -state_8017d118.transition_offset.x;
        offset_y = -state_8017d118.transition_offset.y;
        offset_z = -state_8017d118.transition_offset.z;
    }
    goto apply;
}
