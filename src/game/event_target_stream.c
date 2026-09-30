#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>

extern u8 *func_800461a0(KfActor *actor);
extern u8 func_80046144(const KfTargetCandidate *candidate, u8 marker);
extern void func_800460a0(KfActor *actor, u8 state, u16 phase,
                           s32 target_phase, s32 phase_step);
extern void func_80034e10(u16 archive_slot, u16 archive_entry);
extern void func_80028fa8(void);
extern void func_8001ceb8(s32 value);
extern void func_8001dc64(void);
extern void func_8001d8d0(void);
extern s32 func_8001d6a8(void);
extern void func_800335a0(s32 arg0, s32 arg1);

RODATA(0x80012890, 0x40)

ADDRESS(0x800462bc, 0x444)
void func_800462bc(KfActor *actor)
{
    KfTargetCandidate *candidate =
        actor_state.target_groups[actor->group_index].targets[0].pointer;
    u8 *cursor;
    s32 repeat = 0;
    s32 restore_state = 0;
    u8 saved_state = 0;
    s32 old_counter;
    s32 choice;

    if (candidate == 0 || candidate->type != 0x70) {
        return;
    }
    if (candidate->fallback_offset == 0) {
        event_state.control.bytes[0x3f] = actor->unknown_01;
    }
    cursor = func_800461a0(actor);
    if (event_state.control.bytes[0x3f] != actor->unknown_01 &&
        candidate->marker_state == 1) {
        while (*cursor++ != 0xf0) {
        }
        cursor++;
        candidate->fallback_offset = (cursor + 0xec) - (u8 *)candidate;
        candidate->marker_state = 0;
    }

    for (;;) {
        switch (*cursor - 0xf0) {
        case 0:
            candidate->marker_state = 1;
            /* The two rewind opcodes share their byte-count operand. */
        case 8:
            candidate->fallback_offset -= cursor[1];
            cursor -= cursor[1];
            break;
        case 2:
            cursor += 2;
            candidate->fallback_offset += 2;
            break;
        case 3:
            goto advance;
        case 4:
            cursor++;
            candidate->fallback_offset++;
            state_8017d118.active_table[4](actor, *cursor);
            goto advance;
        case 5:
            cursor++;
            candidate->fallback_offset++;
            repeat = *cursor;
            goto advance;
        case 6:
            event_state.control.bytes[0x3f] = actor->unknown_01;
            cursor++;
            candidate->fallback_offset++;
            candidate->marker_state = 0;
            break;
        case 7:
            event_state.control.bytes[cursor[1]] = cursor[2];
            cursor += 2;
            candidate->fallback_offset += 2;
            goto advance;
        case 9:
            if (event_state.control.bytes[cursor[1]] == cursor[2]) {
                candidate->fallback_offset = func_80046144(candidate, cursor[3]);
                cursor = candidate->bytes + candidate->fallback_offset;
            } else {
                cursor += 4;
                candidate->fallback_offset += 4;
            }
            break;
        case 15:
            goto after_script;
        default:
            goto execute;
        }
        continue;

execute:
        if (restore_state == 0 && ((u8 *)candidate)[1] != 0xff) {
            saved_state = actor->unknown_0c;
            restore_state = 1;
            if (actor->animation_phase != 0) {
                func_800460a0(actor, actor->unknown_0c,
                              actor->animation_phase, 0, actor->animation_step);
            }
            func_800460a0(actor, ((u8 *)candidate)[1], 0, 0xfff,
                          *(u16 *)((u8 *)candidate + 8));
        }
        func_80034e10(3, *(u16 *)((u8 *)candidate + 0x0c) + *cursor);

advance:
        cursor++;
        candidate->fallback_offset++;
        if (repeat != 0) {
            repeat--;
            continue;
        }
        break;
    }

after_script:
    old_counter = game_counter_bytes[0x53];
    switch (((u8 *)candidate)[0x12] & 0xf0) {
    case 0:
        func_80028fa8();
        func_8001ceb8(((u8 *)candidate)[0x12] & 0xf);
        break;
    case 0x10:
        func_80028fa8();
        func_8001dc64();
        break;
    case 0x20:
        func_80028fa8();
        func_8001d8d0();
        break;
    case 0x30:
        func_80028fa8();
        choice = func_8001d6a8();
        if (choice != -1) {
            func_800335a0(0, 0);
            func_80034e10(6, choice + 360);
        }
        break;
    }
    if (game_counter_bytes[0x53] < old_counter) {
        event_state.control.bytes[0x1c] = 1;
    }
    event_state.control.bytes[0x3f] = actor->unknown_01;
    if (restore_state != 0 && ((u8 *)candidate)[0x11] != 0xff) {
        func_800460a0(actor, ((u8 *)candidate)[0x11], 0, 0xfff,
                      *(u16 *)((u8 *)candidate + 0x0e));
        actor->unknown_0c = saved_state;
    }
    event_state.state_word = 1;
    player_clear_motion();
}
