#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/event_counter.h>
#include <kf/game/graphics.h>
#include <kf/game/notify.h>
#include <kf/lib/math.h>

DATA(0x8009a5e8, 0x78)
u8 game_counter_bytes[0x78];

ADDRESS(0x800473e0, 0x54)
s32 game_counter_decrement(s32 index)
{
    if (game_counter_bytes[index] != 0) {
        game_counter_bytes[index]--;
        return 0;
    }

    return 1;
}

ADDRESS(0x80047434, 0x90)
s32 game_counter_increment(s32 index)
{
    if (game_counter_bytes[index] < 99) {
        game_counter_bytes[index]++;
        state_8017d118.active_table[6]();
        return 0;
    }

    notify_enqueue(0x12);
    return 1;
}

ADDRESS(0x800474c4, 0x114)
void func_800474c4(s32 step, s32 first, s32 second, s32 third,
                   s32 target_first, s32 target_second, s32 target_third)
{
    s32 fraction = 0;

    do {
        s32 current_first;
        s32 current_second;
        s32 current_third;

        func_8002bc18();
        current_first = func_8001584c(first, target_first, fraction);
        current_second = func_8001584c(second, target_second, fraction);
        current_third = func_8001584c(third, target_third, fraction);
        func_80031634(current_first, current_second, current_third, 0x800);
        func_800335a0(0, 0);
        fraction += step;
    } while (fraction < 4096);

    func_8002bc18();
    func_80031634(target_first, target_second, target_third, 0x800);
    func_800335a0(0, 0);
}
