#include <kf/lib/address.h>
#include <kf/game/effect.h>

ADDRESS(0x80045cc0, 0x30)
void effect_pool_reset(void)
{
    KfEffectRecord *record = effect_state.records;
    u16 i;

    for (i = 0; i < KF_EFFECT_CAPACITY; i++) {
        record->type = KF_EFFECT_SLOT_FREE;
        record++;
    }
}

ADDRESS(0x80045cf0, 0x2c)
void magic_load_records(const KfMagicRecord *records)
{
    const u32 *source = (const u32 *)records;
    u32 *destination = (u32 *)effect_state.magic_records;
    s32 count;

    for (count = sizeof effect_state.magic_records / sizeof *source; count != 0; count--) {
        *destination++ = *source++;
    }
}


ADDRESS(0x80045d1c, 0xfc)
void effect_pool_sweep(void)
{
    KfEffectRecord *record = effect_state.records;

    effect_state.current_index = 0;
    do {
        if (record->type != KF_EFFECT_SLOT_FREE) {
            effect_state.current_record = record;
            effect_state.current_magic = &effect_state.magic_records[record->kind];
            if (record->updates_remaining != -1) {
                if (--record->updates_remaining == -1) {
                    record->type = KF_EFFECT_SLOT_FREE;
                } else {
                    effect_update_dispatch();
                }
            } else {
                effect_update_dispatch();
            }
        }
        effect_state.current_index++;
        record++;
    } while (effect_state.current_index < KF_EFFECT_CAPACITY);
}
