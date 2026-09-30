#include <kf/lib/address.h>
#include <kf/game/effect.h>

extern void func_8003ff18(VECTOR *position, s32 start, s32 end, s32 arg5,
                          s32 arg3, s32 fixed_scale);

ADDRESS(0x80041cd0, 0xac)
void func_80041cd0(s32 multiplier, s32 limit, s32 increment, s32 arg3, s32 arg5)
{
    KfEffectRecord *record = effect_state.current_record;
    s32 scaled_size;

    record->scale_y = record->scale_z = record->scale_x += increment;
    scaled_size = (multiplier * (s16)record->scale_x) >> 12;
    func_8003ff18(&record->position,
                  scaled_size - ((multiplier * increment) >> 12),
                  scaled_size - 1, arg5, arg3, 0x1000);
    if ((s16)record->scale_x >= limit) {
        record->type = KF_EFFECT_SLOT_FREE;
    }
}
