#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>

ADDRESS(0x8002d8b0, 0x40)
void tmd_register(u16 slot, KfTmdHeader *tmd)
{
    game_graphics_runtime.tmd_state.current_asset = game_graphics_runtime.tmd_state.slots[slot] = tmd;
    tmd_prepare_primitive_indices(tmd);
}

ADDRESS(0x8002d8f0, 0x20)
void tmd_set_slot(u16 slot, KfTmdHeader *tmd)
{
    game_graphics_runtime.tmd_state.slots[slot] = tmd;
}

/* Left empty in retail. */
ADDRESS(0x8002d910, 0x8)
void tmd_release_slot(void)
{
}
