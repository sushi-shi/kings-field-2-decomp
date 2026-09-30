#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <psyq/kernel.h>

RODATA(0x80011104, 0x1a)

ADDRESS(0x80022600, 0xec)
s32 memory_card_probe_temporary_file(void)
{
    char path[26] = "bu00:BISLPS-00069TEMP    ";
    s32 status;
    s32 handle;

    memory_card_clear_events();
    _card_info(0);
    status = memory_card_wait_event();
    if (status != KF_CARD_EVENT_NEW_DEVICE && status != KF_CARD_EVENT_IO_END) {
        return status;
    }
    handle = open(path, 0x200);
    close(handle);
    delete(path);
    return (handle == -1) << 1;
}
