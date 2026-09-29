#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <psyq/kernel.h>

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
