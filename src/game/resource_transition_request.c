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

    if (fifth == 200) {
        if (audio_state.sequence_active == 0) {
            audio_start_sequence();
        }
        return;
    }

    if (first == 255) {
        current_first = state_8017d118.active_resource_ids[0];
        prior_first = state_8017d118.requested_resource_ids[0];
        if (second == 255) {
            current_second = state_8017d118.active_resource_ids[1];
            prior_second = state_8017d118.requested_resource_ids[1];
        } else {
            current_second = second;
            prior_second = second;
        }
        if (third == 255) {
            current_third = state_8017d118.active_resource_ids[2];
            prior_third = state_8017d118.requested_resource_ids[2];
        } else {
            current_third = third;
            prior_third = third;
        }
        if (fourth == 255) {
            current_fourth = state_8017d118.active_resource_ids[3];
            prior_fourth = state_8017d118.requested_resource_ids[3];
        } else {
            current_fourth = fourth;
            prior_fourth = fourth;
        }
        if (fifth == 255) {
            current_fifth = state_8017d118.active_resource_ids[4];
            prior_fifth = state_8017d118.requested_resource_ids[4];
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
        current_second = state_8017d118.active_resource_ids[1];
        current_third = state_8017d118.active_resource_ids[2];
        current_fourth = state_8017d118.active_resource_ids[3];
        current_fifth = state_8017d118.active_resource_ids[4];
    }

    if (state_8017d118.transition_active != 0) {
        goto handle_active;
    }
    if (state_8017d118.active_resource_ids[0] == current_first &&
        state_8017d118.active_resource_ids[1] == current_second &&
        state_8017d118.active_resource_ids[2] == current_third &&
        state_8017d118.active_resource_ids[3] == current_fourth &&
        state_8017d118.active_resource_ids[4] == current_fifth) {
        return;
    }

apply:
    if (state_8017d118.active_resource_ids[0] != 99 &&
        state_8017d118.active_resource_ids[0] != current_first) {
        event_world_state_save_slot(state_8017d118.active_resource_ids[0]);
    }
    if (event_state.control.fields.unknown_04[0] < current_first) {
        event_state.control.fields.unknown_04[0] = current_first;
    }
    state_8017d118.transition_active = 1;
    state_8017d118.transition_phase = 0;
    state_8017d118.requested_resource_ids[0] = first;
    state_8017d118.requested_resource_ids[1] = second;
    state_8017d118.requested_resource_ids[2] = third;
    state_8017d118.requested_resource_ids[3] = fourth;
    state_8017d118.requested_resource_ids[4] = fifth;
    state_8017d118.transition_offset_xzy[0] = offset_x;
    state_8017d118.transition_offset_xzy[1] = offset_z;
    state_8017d118.world_shift_applied = 0;
    state_8017d118.transition_offset_xzy[2] = offset_y;
    if (second == 255) {
        state_8017d118.flag_16 = 0;
    } else {
        state_8017d118.flag_16 = 1;
    }
    return;

handle_active:
    if ((state_8017d118.transition_active != 1 ||
         state_8017d118.requested_resource_ids[0] == prior_first) &&
        state_8017d118.requested_resource_ids[1] == prior_second &&
        state_8017d118.requested_resource_ids[2] == prior_third &&
        state_8017d118.requested_resource_ids[3] == prior_fourth &&
        state_8017d118.requested_resource_ids[4] == prior_fifth) {
        return;
    }
    if ((state_8017d118.requested_resource_ids[0] == 255 &&
         state_8017d118.active_resource_ids[0] == current_first) ||
        (state_8017d118.requested_resource_ids[1] == 255 &&
         state_8017d118.active_resource_ids[1] == second) ||
        (state_8017d118.requested_resource_ids[2] == 255 &&
         state_8017d118.active_resource_ids[2] == third) ||
        (state_8017d118.requested_resource_ids[3] == 255 &&
         state_8017d118.active_resource_ids[3] == fourth) ||
        (state_8017d118.requested_resource_ids[4] == 255 &&
         state_8017d118.active_resource_ids[4] == fifth)) {
        return;
    }

    if ((state_8017d118.requested_resource_ids[0] != 255 && first == 255) ||
        (state_8017d118.requested_resource_ids[1] != 255 && second == 255) ||
        (state_8017d118.requested_resource_ids[2] != 255 && third == 255) ||
        (state_8017d118.requested_resource_ids[3] != 255 && fourth == 255) ||
        (state_8017d118.requested_resource_ids[4] != 255 && fifth == 255)) {
        while (state_8017d118.transition_active != 0) {
            cd_request_yield();
            resource_advance_transition();
        }
    } else {
        do {
            EnterCriticalSection();
            if (state_8017d118.transition_phase != 0xf0) break;
            ExitCriticalSection();
            cd_request_yield();
        } while (1);
    }
    ExitCriticalSection();
    if (state_8017d118.world_shift_applied != 0 && first != 255 &&
        offset_x == 127) {
        offset_x = -state_8017d118.transition_offset_xzy[0];
        offset_z = -state_8017d118.transition_offset_xzy[1];
        offset_y = -state_8017d118.transition_offset_xzy[2];
    }
    goto apply;
}
