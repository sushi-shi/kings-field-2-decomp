#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/card.h>
#include <psyq/kernel.h>
#include <psyq/pad.h>

DATA(0x8006d6a0, 0x1)
u8 memory_card_loaded_slot = 0;

/* LIBAPI's HwCARD, EvSpIOE, EvSpTIMOUT, EvSpNEW, EvSpERROR and EvMdNOINTR,
 * which the Psy-Q 3.0 kit's headers do not define. */
#define CARD_EVENT_CLASS 0xf4000001
enum {
    CARD_EVENT_SPEC_IO_END = 0x0004,
    CARD_EVENT_SPEC_TIMEOUT = 0x0100,
    CARD_EVENT_SPEC_NEW_DEVICE = 0x2000,
    CARD_EVENT_SPEC_ERROR = 0x8000,
    CARD_EVENT_MODE_NO_INTERRUPT = 0x2000
};

ADDRESS(0x80022438, 0x30)
void input_wait_release(void)
{
    input_idle_counter = 0;
    while (PadRead(1) != 0) {
    }
}

ADDRESS(0x80022468, 0xe8)
void memory_card_initialize(void)
{
    memory_card_io_end_event = OpenEvent(CARD_EVENT_CLASS, CARD_EVENT_SPEC_IO_END,
        CARD_EVENT_MODE_NO_INTERRUPT, NULL);
    memory_card_timeout_event = OpenEvent(CARD_EVENT_CLASS, CARD_EVENT_SPEC_TIMEOUT,
        CARD_EVENT_MODE_NO_INTERRUPT, NULL);
    memory_card_new_device_event = OpenEvent(CARD_EVENT_CLASS, CARD_EVENT_SPEC_NEW_DEVICE,
        CARD_EVENT_MODE_NO_INTERRUPT, NULL);
    memory_card_error_event = OpenEvent(CARD_EVENT_CLASS, CARD_EVENT_SPEC_ERROR,
        CARD_EVENT_MODE_NO_INTERRUPT, NULL);
    EnableEvent(memory_card_io_end_event);
    EnableEvent(memory_card_timeout_event);
    EnableEvent(memory_card_new_device_event);
    EnableEvent(memory_card_error_event);
    memory_card_buffer = memory_card_buffer_storage;
}

ADDRESS(0x80022550, 0x60)
void memory_card_shutdown_events(void)
{
    CloseEvent(memory_card_io_end_event);
    CloseEvent(memory_card_timeout_event);
    CloseEvent(memory_card_new_device_event);
    CloseEvent(memory_card_error_event);
    StopCARD();
}

ADDRESS(0x800225b0, 0x28)
void memory_card_start(void)
{
    StartCARD();
    _bu_init();
}

ADDRESS(0x800225d8, 0x28)
void memory_card_stop(void)
{
    StopCARD();
    PadInit(0);
}
