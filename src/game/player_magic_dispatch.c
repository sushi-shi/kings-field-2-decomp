#include <kf/lib/address.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

extern void func_80026330(s32 mode, VECTOR *output);
extern void func_80025a18();

RODATA(0x80011260, 0x34)

ADDRESS(0x80026498, 0x1c4)
void func_80026498(s32 magic_id, s32 consume_mp, s32 effect_parameter)
{
    KfMagicRecord *record = &effect_state.magic_records[magic_id];
    VECTOR position;

    if (player_state.vitals.current_mp < record->mp_cost) {
        return;
    }
    player_state.selected_magic_record = record;
    if (consume_mp != 0) {
        player_state.magic_charge = 0;
        player_state.vitals.current_mp -= record->mp_cost;
    }

    switch (magic_id - 38) {
    case 1:
        func_80026330(DAT_800667e8.effect_ids[effect_parameter], &position);
        func_80025a18(magic_id, &position);
        break;
    case 11:
    case 12:
        func_80026330(0, &position);
        func_80025a18(magic_id, &position);
        break;
    case 2:
        effect_parameter <<= 9;
        player_state.unknown_118.vx = rcos(effect_parameter) >> 3;
        player_state.unknown_118.vy = rsin(effect_parameter) >> 3;
        player_state.unknown_118.vz = 600;
        func_80025a18(magic_id);
        break;
    case 0:
        for (effect_parameter = 0; effect_parameter < 4095; effect_parameter += 684) {
            player_state.unknown_118.vx = rcos(effect_parameter) >> 3;
            player_state.unknown_118.vy = rsin(effect_parameter) >> 3;
            player_state.unknown_118.vz = 400;
            func_80025a18(magic_id);
        }
        break;
    default:
        player_state.unknown_118.vx = 200;
        player_state.unknown_118.vy = 200;
        player_state.unknown_118.vz = 400;
        func_80025a18(magic_id);
        break;
    }
}
