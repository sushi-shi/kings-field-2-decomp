#include <kf/lib/address.h>
#include <kf/game/animation.h>

enum { KF_ANIMATION_BLEND_ONE = 0x1000, KF_ANIMATION_BLEND_SHIFT = 12 };

ADDRESS(0x80033b34, 0xc8)
KfAnimKeyframe *animation_select_keyframe(KfAssetHeader *asset, s32 clip_index, s32 phase,
                                          s32 *keyframe_index, u32 *blend_fraction)
{
    u32 *clip_table = (u32 *)((u8 *)asset + asset->clip_table_offset);
    KfAnimClip *clip = (KfAnimClip *)((u8 *)asset + clip_table[clip_index]);
    u32 *offsets = clip->keyframe_offsets;
    s32 remaining = clip->keyframe_count;
    s32 index = 0;
    s32 phase_end = 0;
    s32 phase_start = 0;
    u32 fraction;
    KfAnimKeyframe *keyframe;

    for (--remaining; remaining != -1; --remaining) {
        keyframe = (KfAnimKeyframe *)((u8 *)asset + *offsets++);

        phase_end += keyframe->duration;
        if (phase < phase_end) {
            fraction = ((u32)(phase - phase_start) << KF_ANIMATION_BLEND_SHIFT)
                / keyframe->duration;
            if (keyframe->reverse != 0) {
                fraction = KF_ANIMATION_BLEND_ONE - fraction;
            }
            goto selected;
        }
        phase_start = phase_end;
        index++;
    }
    index--;
    fraction = KF_ANIMATION_BLEND_ONE;

selected:
    *keyframe_index = index;
    *blend_fraction = fraction;
    return keyframe;
}
