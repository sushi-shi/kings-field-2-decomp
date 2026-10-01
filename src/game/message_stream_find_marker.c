#include <kf/game/actor.h>
#include <kf/game/event_state.h>
#include <kf/lib/address.h>

ADDRESS(0x80046144, 0x5c)
u8 func_80046144(const KfTargetCandidate *candidate, u8 marker)
{
    const u8 *cursor = candidate->word_14.bytes;

    for (;;) {
        s32 code = *cursor++;

        if (code == 0xf2) {
            goto marker_record;
        }
        if (code == 0xff) {
            return candidate->word_10.bytes.fallback_offset;
        }
        continue;

marker_record:
        if (*cursor == marker) {
            const u8 *base = &candidate->word_12.bytes.marker_state;
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
    u8 *cursor = candidate->word_14.bytes;
    u8 *marker = cursor + 3;

    for (;;) {
        u8 code = *cursor;

        if (code == 0xf1) {
            goto marker_record;
        }
        if (code != 0xfe) {
            /* Retail retries this byte; the stream must supply a control code. */
            continue;
        }
        if (candidate->word_10.bytes.fallback_offset == 0) {
            cursor++;
            candidate->word_10.bytes.fallback_offset = cursor - candidate->word_14.bytes;
            return cursor;
        }
use_fallback:
        return candidate->word_14.bytes + candidate->word_10.bytes.fallback_offset;

marker_record:
        if (event_state.control.bytes[cursor[1]] == cursor[2]) {
            u8 offset = func_80046144(candidate, *marker);
            if (candidate->word_10.bytes.fallback_offset < offset) {
                candidate->word_10.bytes.fallback_offset = offset;
            }
            event_state.control.bytes[0x3f] = actor->unknown_01;
            candidate->word_12.bytes.marker_state = 0;
            goto use_fallback;
        }
        marker += 4;
        cursor += 4;
    }
}
