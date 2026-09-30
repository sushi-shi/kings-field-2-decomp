#include <kf/lib/address.h>
#include <kf/game/callback.h>

ADDRESS(0x800167bc, 0x14)
void resource_transition_set_phase_1(void)
{
    state_8017d118.transition_phase = 1;
}

ADDRESS(0x800167d0, 0x14)
void resource_transition_set_phase_3(void)
{
    state_8017d118.transition_phase = 3;
}

ADDRESS(0x800167e4, 0x14)
void resource_transition_set_phase_2(void)
{
    state_8017d118.transition_phase = 2;
}

ADDRESS(0x800167f8, 0x14)
void resource_transition_set_phase_4(void)
{
    state_8017d118.transition_phase = 4;
}

ADDRESS(0x8001680c, 0x14)
void resource_transition_set_phase_6(void)
{
    state_8017d118.transition_phase = 6;
}
