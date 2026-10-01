#include <kf/lib/address.h>
#include <kf/game/asset.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/graphics.h>
#include <kf/game/map_object.h>
#include <kf/game/memory.h>
#include <kf/game/player.h>
#include <kf/game/tmd.h>

RODATA(0x80011000, 0x58)

ADDRESS(0x80015d58, 0x27c)
void func_80015d58(void)
{
    u8 *source = (u8 *)KF_GAME_RESOURCE_ARENA_BASE;
    u8 *second_value = &state_8017d118.values_04[1];

    state_8017d118.values_04[3] = 255;
    state_8017d118.values_04[2] = 255;
    *second_value = 255;
    state_8017d118.transition_active = 0;
    state_8017d118.transition_phase = 0;
    state_8017d118.values_04[0] = 0;
    *second_value = 0;
    state_8017d118.values_04[2] = 0;
    state_8017d118.values_04[3] = 0;
    state_8017d118.values_04[4] = 0;
    state_8017d118.unknown_09[0] = 0;

    cd_archive_open(0, "COM\\MO.T");
    cd_archive_open(3, "COM\\TALK.T");
    cd_archive_open(4, "COM\\VAB.T");
    cd_archive_open(5, "COM\\FDAT.T");
    cd_archive_open(2, "COM\\RTIM.T");
    cd_archive_open(1, "COM\\RTMD.T");
    cd_archive_open(6, "COM\\ITEM.T");

    /* The read arena and two copy destinations still lack full source owners. */
    cd_archive_read(5, 0x30, (u_long *)source);
    cd_map_stream_read(5, 0x2f);
    audio_queue_vab_stream(4, 0, 0);

    resource_copy_words((u32 *)&map_object_state, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)player_weapon_records, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)0x801d8d88, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)player_level_growth_table, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)&effect_state, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)collision_default_rows, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)audio_state.voices.params, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    resource_copy_words((u32 *)0x800fa0d0, (u32 *)(source + 4), *(u32 *)source >> 2);
    source += *(u32 *)source + 4;
    asset_registry_load_tmd_archive(0, (u8 *)0x800fa0d0);
    resource_copy_words((u32 *)0x800855a0, (u32 *)(source + 4), *(u32 *)source >> 2);
    asset_registry_load_tmd_archive(0x28, (u8 *)0x800855a0);
    game_initialize_session();
}

ADDRESS(0x80015fd4, 0x114)
void func_80015fd4(void)
{
    u8 first;
    u8 second;
    u8 third;
    u8 fourth;
    u8 fifth;

    state_8017d118.transition_active = 1;
    state_8017d118.transition_phase = 0;
    first = state_8017d118.values_04[0];
    second = state_8017d118.values_04[1];
    third = state_8017d118.values_04[2];
    fourth = state_8017d118.values_04[3];
    fifth = state_8017d118.values_04[4];
    state_8017d118.values_04[0] = 99;
    state_8017d118.values_04[1] = 99;
    state_8017d118.values_04[2] = 99;
    state_8017d118.values_04[3] = 99;
    state_8017d118.values_04[4] = 99;
    state_8017d118.values_17[0] = 127;
    state_8017d118.values_17[1] = 127;
    state_8017d118.values_17[2] = 127;
    state_8017d118.values_10[0] = first;
    state_8017d118.values_10[1] = second;
    state_8017d118.values_10[2] = third;
    state_8017d118.values_10[3] = fourth;
    state_8017d118.values_10[4] = fifth;
    do {
        cd_request_yield();
        func_80016820();
    } while (state_8017d118.transition_active != 0);
    tmd_set_slot(0, (KfTmdHeader *)0x8012da68);
    state_8017d118.active_table[5]();
}
