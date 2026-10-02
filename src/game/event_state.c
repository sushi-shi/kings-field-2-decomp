#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>

DATA(0x801b2140, 0x3918)
KfEventState event_state;

ADDRESS(0x800482f8, 0xb0)
void func_800482f8(void)
{
    u16 *offset;
    s32 index;
    KfEventControlSentinels *sentinels;

    repeat_store_word(event_state.control.clear_words, 0, 0x40);
    repeat_store_word(event_state.arena.clear_words, 0, 0xe00);
    sentinels = &event_state.control.fields.sentinels;
    sentinels->unknown_08 = 0xffff;
    sentinels->unknown_04 = 0xffff;
    sentinels->unknown_00 = 0xffff;
    memory_arena_initialize_blocks(&event_state.arena.first_block, 0x3800);
    offset = event_state.saved_offsets;
    for (index = KF_EVENT_SAVED_SLOT_COUNT - 1; index != -1; index--) {
        *offset++ = 0xffff;
    }
    repeat_store_word((u32 *)game_counter_bytes, 0, 0x1e);
    game_counter_bytes[0] = 1;
}

ADDRESS(0x800483a8, 0x30)
void callback_invoke_slot_04_zero(void)
{
    state_8017d118.active_table[1](0);
}

ADDRESS(0x800483d8, 0x50)
void func_800483d8(u8 **pointers)
{
    u16 *offset = event_state.saved_offsets;
    s32 index = KF_EVENT_SAVED_SLOT_COUNT - 1;
    u16 absent = 0xffff;

    for (; index != -1; index--) {
        u16 value = *offset++;
        if (value == absent) {
            *pointers = 0;
        } else {
            *pointers = value + event_state.arena.bytes;
        }
        pointers++;
    }
}

ADDRESS(0x80048428, 0x70)
void func_80048428(s32 delta)
{
    KfMemoryBlock *block = &event_state.arena.first_block;

    if (block->kind != 0xff) {
        do {
            s32 kind = block->kind;
            u32 step;
            if (kind < 4) {
                if (kind != 0) {
                    block->owner = (u8 **)((u8 *)block->owner + delta);
                }
            }
            step = block->size + sizeof(*block);
            block = (KfMemoryBlock *)((u8 *)block + step);
        } while (block->kind != 0xff);
    }
}

ADDRESS(0x80048498, 0x4c)
void func_80048498(u8 **pointers)
{
    u16 *offset = event_state.saved_offsets;
    s32 index;

    for (index = KF_EVENT_SAVED_SLOT_COUNT - 1; index != -1; index--) {
        u8 *value = *pointers;
        pointers++;
        if (value == 0) {
            *offset = 0xffff;
        } else {
            *offset = (u16)(value - event_state.arena.bytes);
        }
        offset++;
    }
}

ADDRESS(0x800484e4, 0x70)
void func_800484e4(s32 delta)
{
    KfMemoryBlock *block = &event_state.arena.first_block;

    if (block->kind != 0xff) {
        do {
            s32 kind = block->kind;
            u32 step;
            if (kind < 4) {
                if (kind != 0) {
                    block->owner = (u8 **)((u8 *)block->owner - delta);
                }
            }
            step = block->size + sizeof(*block);
            block = (KfMemoryBlock *)((u8 *)block + step);
        } while (block->kind != 0xff);
    }
}
