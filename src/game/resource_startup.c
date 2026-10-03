#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/asset.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/graphics.h>
#include <kf/game/map_object.h>
#include <kf/game/memory.h>
#include <kf/game/player.h>
#include <kf/game/resources.h>
#include <kf/game/tmd.h>

RODATA(0x80011000, 0x53)

enum {
    RESOURCE_ARCHIVE_MO = 0,
    RESOURCE_ARCHIVE_RTMD = 1,
    RESOURCE_ARCHIVE_RTIM = 2,
    RESOURCE_ARCHIVE_TALK = 3,
    RESOURCE_ARCHIVE_VAB = 4,
    RESOURCE_ARCHIVE_FDAT = 5,
    RESOURCE_ARCHIVE_ITEM = 6
};

ADDRESS(0x80015d50, 0x8)
/* The default table serves callback slots with different caller arguments. */
void resource_noop_callback()
{
}

ADDRESS(0x80015d58, 0x27c)
void resource_initialize_game_assets(void)
{
    u8 *source = (u8 *)KF_GAME_RESOURCE_ARENA_BASE;
    u8 *second_value = &state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD];

    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] = 255;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] = 255;
    *second_value = 255;
    state_8017d118.transition_active = 0;
    state_8017d118.transition_phase = 0;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = 0;
    *second_value = 0;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] = 0;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] = 0;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = 0;
    state_8017d118.current_map_region_id = 0;

    cd_archive_open(RESOURCE_ARCHIVE_MO, "COM\\MO.T");
    cd_archive_open(RESOURCE_ARCHIVE_TALK, "COM\\TALK.T");
    cd_archive_open(RESOURCE_ARCHIVE_VAB, "COM\\VAB.T");
    cd_archive_open(RESOURCE_ARCHIVE_FDAT, "COM\\FDAT.T");
    cd_archive_open(RESOURCE_ARCHIVE_RTIM, "COM\\RTIM.T");
    cd_archive_open(RESOURCE_ARCHIVE_RTMD, "COM\\RTMD.T");
    cd_archive_open(RESOURCE_ARCHIVE_ITEM, "COM\\ITEM.T");

    /* The read arena and three copy destinations still lack full source owners. */
    cd_archive_read(RESOURCE_ARCHIVE_FDAT, 0x30, (u_long *)source);
    cd_map_stream_read(RESOURCE_ARCHIVE_FDAT, 0x2f);
    audio_queue_vab_stream(RESOURCE_ARCHIVE_VAB, 0, 0);

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
void resource_run_initial_transition(void)
{
    u8 first;
    u8 second;
    u8 third;
    u8 fourth;
    u8 fifth;

    state_8017d118.transition_active = 1;
    state_8017d118.transition_phase = 0;
    first = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION];
    second = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD];
    third = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM];
    fourth = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB];
    fifth = state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE];
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TMD] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_TIM] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_VAB] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.active_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = KF_RESOURCE_ACTIVE_UNINITIALIZED;
    state_8017d118.transition_offset.x = KF_RESOURCE_OFFSET_NO_SHIFT;
    state_8017d118.transition_offset.z = KF_RESOURCE_OFFSET_NO_SHIFT;
    state_8017d118.transition_offset.y = KF_RESOURCE_OFFSET_NO_SHIFT;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_MAP_REGION] = first;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TMD] = second;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_TIM] = third;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_VAB] = fourth;
    state_8017d118.requested_resource_ids[KF_RESOURCE_SLOT_SEQUENCE] = fifth;
    do {
        cd_request_yield();
        resource_advance_transition();
    } while (state_8017d118.transition_active != 0);
    tmd_set_slot(0, (KfTmdHeader *)0x8012da68);
    state_8017d118.active_table[5]();
}

ADDRESS(0x800160e8, 0x178)
void translate_active_world_positions(s32 dx, s32 dy, s32 dz)
{
    KfEffectRecord *effect = effect_state.records;
    KfMapObject *object;
    KfActor *actor;
    u16 effect_remaining;
    u16 object_remaining;
    u16 actor_remaining;

    player_state.camera_position.vx += dx;
    player_state.camera_position.vz += dz;
    player_state.camera_position.vy += dy;

    effect_remaining = KF_EFFECT_CAPACITY - 1;
    do {
        if (effect->type != KF_EFFECT_SLOT_FREE &&
            (effect->render_flags & 0xc) != 0xc) {
            effect->position.vx += dx;
            effect->position.vz += dz;
            effect->position.vy += dy;
        }
        effect++;
    } while (effect_remaining-- != 0);

    object = map_object_state.objects;
    object_remaining = KF_MAP_OBJECT_CAPACITY - 1;
    do {
        if (object->object_id != KF_MAP_OBJECT_ID_NONE) {
            object->position.vx += dx;
            object->position.vz += dz;
            object->position.vy += dy;
        }
        object++;
    } while (object_remaining-- != 0);

    actor = actor_state.actors;
    actor_remaining = KF_ACTOR_CAPACITY - 1;
    do {
        if (actor->slot_state != KF_ACTOR_SLOT_FREE) {
            actor->position.vx += dx;
            actor->position.vz += dz;
            actor->position.vy += dy;
        }
        actor++;
    } while (actor_remaining-- != 0);
}
