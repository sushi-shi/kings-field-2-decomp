#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/player.h>
#include <psyq/kernel.h>

ADDRESS(0x80023178, 0x110)
void func_80023178(KfCardHeader *header, s32 slot_glyph)
{
    s32 experience = player_state.experience;
    s32 level = player_state.level;
    s32 digit;
    s32 index;

    header->title[30] = 0x82;
    header->title[31] = slot_glyph + 0x4f;
    for (index = 0; index < 6; index++) {
        digit = experience % 10;
        experience /= 10;
        header->title[(25 - index) * 2] = 0x82;
        header->title[1 + (25 - index) * 2] = digit + 0x4f;
        if (experience == 0)
            index = 6;
    }

    for (index = 0; index < 2; index++) {
        digit = level % 10;
        level /= 10;
        header->title[(30 - index) * 2] = 0x82;
        header->title[1 + (30 - index) * 2] = digit + 0x4f;
        if (level == 0)
            index = 2;
    }
}

ADDRESS(0x80023288, 0x24)
u32 memory_card_payload_byte_sum(const u8 *payload)
{
    u32 sum = 0;
    s32 index;

    for (index = KF_CARD_PAYLOAD_BYTES - 1; index >= 0;) {
        sum += *payload;
        index--;
        payload++;
    }
    return sum;
}

ADDRESS(0x800232ac, 0x80)
s32 memory_card_wait_event(void)
{
    for (;;) {
        if (TestEvent(memory_card_io_end_event) == 1) {
            return KF_CARD_EVENT_IO_END;
        }
        if (TestEvent(memory_card_timeout_event) == 1) {
            return KF_CARD_EVENT_TIMEOUT;
        }
        if (TestEvent(memory_card_new_device_event) == 1) {
            return KF_CARD_EVENT_NEW_DEVICE;
        }
        if (TestEvent(memory_card_error_event) == 1) {
            return KF_CARD_EVENT_ERROR;
        }
    }
}

ADDRESS(0x8002332c, 0x58)
void memory_card_clear_events(void)
{
    TestEvent(memory_card_io_end_event);
    TestEvent(memory_card_timeout_event);
    TestEvent(memory_card_new_device_event);
    TestEvent(memory_card_error_event);
}
