#include <kf/lib/address.h>
#include <kf/lib/overlay.h>
#include <kf/game/game.h>
#include <psyq/cd.h>
#include <psyq/kernel.h>
#include <psyq/pad.h>

/*
 * GCC inserts the `__main` hook call for a function named main; the SDK
 * start routine tail-calls here. The heap runs from the end of .bss to the
 * stack reserved below the top of RAM.
 */
ADDRESS(0x80013634, 0x68)
void main(void)
{
    InitHeap(BSS_END, OVERLAY_STACK_BOTTOM - (u32)BSS_END);
    CdInit();
    PadInit(0);
    InitCARD(1);
    ChangeClearPAD(0);
    ExitCriticalSection();
    game_main_loop();
}
