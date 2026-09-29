#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <psyq/kernel.h>

RODATA(0x80011120, 0x6)

/* Nonzero when formatting card slot 1 fails. */
ADDRESS(0x80022b48, 0x2c)
s32 memory_card_format(void)
{
    return format("bu00:") != 1;
}
