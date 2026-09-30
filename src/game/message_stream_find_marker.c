#include <kf/game/actor.h>
#include <kf/game/event_state.h>
#include <kf/lib/address.h>

ADDRESS(0x80046144, 0x5c)
u8 func_80046144(const KfTargetCandidate *candidate, u8 marker)
{
    const u8 *cursor = candidate->bytes;

    for (;;) {
        s32 code = *cursor++;

        if (code == 0xf2) {
            goto marker_record;
        }
        if (code == 0xff) {
            return candidate->fallback_offset;
        }
        continue;

marker_record:
        if (*cursor == marker) {
            const u8 *base = &candidate->marker_state;
            return cursor - base;
        }
        cursor++;
    }
}

ADDRESS(0x800461a0, 0x11c)
u8 *func_800461a0(KfActor *actor)
{
    KfTargetCandidate *candidate =
        actor_state.target_groups[actor->group_index].targets[0].pointer;
    u8 *cursor = candidate->bytes;
    u8 *marker = cursor + 3;

    for (;;) {
        u8 code = *cursor;

        if (code == 0xf1) {
            goto marker_record;
        }
        if (code != 0xfe) {
            continue;
        }
        if (candidate->fallback_offset == 0) {
            cursor++;
            candidate->fallback_offset = cursor - candidate->bytes;
            return cursor;
        }
use_fallback:
        return candidate->bytes + candidate->fallback_offset;

marker_record:
        if (event_state.control.bytes[cursor[1]] == cursor[2]) {
            u8 offset = func_80046144(candidate, *marker);
            if (candidate->fallback_offset < offset) {
                candidate->fallback_offset = offset;
            }
            event_state.control.bytes[0x3f] = actor->unknown_01;
            candidate->marker_state = 0;
            goto use_fallback;
        }
        marker += 4;
        cursor += 4;
    }
}
