#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/event_state.h>
#include <kf/game/resources.h>
#include <psyq/kernel.h>

extern void func_80048554(s32 save_slot);

DATA(0x8017d118, 0x1c)
KfState8017d118 state_8017d118;

ADDRESS(0x80016260, 0x55c)
void func_80016260(u8 first, u8 second, u8 third, u8 fourth,
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

    if ((u8)first == 255) {
        current_first = state_8017d118.values_04[0];
        prior_first = state_8017d118.values_10[0];
        if ((u8)second == 255) {
            current_second = state_8017d118.values_04[1];
            prior_second = state_8017d118.values_10[1];
        } else {
            current_second = (u8)second;
            prior_second = (u8)second;
        }
        if ((u8)third == 255) {
            current_third = state_8017d118.values_04[2];
            prior_third = state_8017d118.values_10[2];
        } else {
            current_third = (u8)third;
            prior_third = (u8)third;
        }
        if ((u8)fourth == 255) {
            current_fourth = state_8017d118.values_04[3];
            prior_fourth = state_8017d118.values_10[3];
        } else {
            current_fourth = (u8)fourth;
            prior_fourth = (u8)fourth;
        }
        if (fifth == 255) {
            current_fifth = state_8017d118.values_04[4];
            prior_fifth = state_8017d118.values_10[4];
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
        current_second = state_8017d118.values_04[1];
        current_third = state_8017d118.values_04[2];
        current_fourth = state_8017d118.values_04[3];
        current_fifth = state_8017d118.values_04[4];
    }

    if (state_8017d118.transition_active == 0) {
        if (state_8017d118.values_04[0] == current_first &&
            state_8017d118.values_04[1] == current_second &&
            state_8017d118.values_04[2] == current_third &&
            state_8017d118.values_04[3] == current_fourth &&
            state_8017d118.values_04[4] == current_fifth) {
            return;
        }
    } else {
        goto handle_active;
    }

apply:
    if (state_8017d118.values_04[0] != 99 &&
        state_8017d118.values_04[0] != current_first) {
        func_80048554(state_8017d118.values_04[0]);
    }
    if (event_state.control.fields.unknown_04[0] < current_first) {
        event_state.control.fields.unknown_04[0] = current_first;
    }
    state_8017d118.transition_active = 1;
    state_8017d118.transition_phase = 0;
    state_8017d118.values_10[0] = (u8)first;
    state_8017d118.values_10[1] = (u8)second;
    state_8017d118.values_10[2] = (u8)third;
    state_8017d118.values_10[3] = (u8)fourth;
    state_8017d118.values_10[4] = fifth;
    state_8017d118.values_17[0] = offset_x;
    state_8017d118.values_17[1] = offset_z;
    state_8017d118.unknown_15 = 0;
    state_8017d118.values_17[2] = offset_y;
    if ((u8)second == 255) {
        state_8017d118.flag_16 = 0;
    } else {
        state_8017d118.flag_16 = 1;
    }
    return;

handle_active:
    if ((state_8017d118.transition_active != 1 ||
         state_8017d118.values_10[0] == prior_first) &&
        state_8017d118.values_10[1] == prior_second &&
        state_8017d118.values_10[2] == prior_third &&
        state_8017d118.values_10[3] == prior_fourth &&
        state_8017d118.values_10[4] == prior_fifth) {
        return;
    }
    if (state_8017d118.values_10[0] == 255 &&
        state_8017d118.values_04[0] == current_first) return;
    if (state_8017d118.values_10[1] == 255 &&
        state_8017d118.values_04[1] == (u8)second) return;
    if (state_8017d118.values_10[2] == 255 &&
        state_8017d118.values_04[2] == (u8)third) return;
    if (state_8017d118.values_10[3] == 255 &&
        state_8017d118.values_04[3] == (u8)fourth) return;
    if (state_8017d118.values_10[4] == 255 &&
        state_8017d118.values_04[4] == fifth) return;

    if ((state_8017d118.values_10[0] != 255 && (u8)first == 255) ||
        (state_8017d118.values_10[1] != 255 && (u8)second == 255) ||
        (state_8017d118.values_10[2] != 255 && (u8)third == 255) ||
        (state_8017d118.values_10[3] != 255 && (u8)fourth == 255) ||
        (state_8017d118.values_10[4] != 255 && fifth == 255)) {
        while (state_8017d118.transition_active != 0) {
            cd_request_yield();
            func_80016820();
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
    if (state_8017d118.unknown_15 != 0 && (u8)first != 255 &&
        offset_x == 127) {
        offset_x = -state_8017d118.values_17[0];
        offset_z = -state_8017d118.values_17[1];
        offset_y = -state_8017d118.values_17[2];
    }
    goto apply;
}
