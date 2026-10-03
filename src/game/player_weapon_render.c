#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

ADDRESS(0x800316c8, 0x188)
void render_player_weapon(void)
{
    s32 cell_x;
    s32 cell_z;
    u8 *layer;
    KfCollisionRow *row;
    KfWeaponRecordGame *weapon;
    KfTmdObject *object;
    MATRIX model;

    if (player_state.weapon_attack_phase == -1) {
        return;
    }

    cell_z = player_state.camera_position.vz >> 11;
    cell_x = player_state.camera_position.vx >> 11;
    layer = &bss_801c7540.map_cells[0][0].layer[0].lighting_index;
    row = &game_graphics_runtime.collision_rows[
        layer[cell_x * sizeof(KfMapOccupancyCell) +
              cell_z * sizeof(bss_801c7540.map_cells[0]) +
              player_state.map_layer_index] & 0x3f];
    SetColorMatrix((MATRIX *)&row->motion);
    SetLightMatrix((MATRIX *)&row->rotations[0]);
    fog_set_near(row->filter.angle);
    SetBackColor(row->filter.kinds.types[0], row->filter.kinds.types[1],
                 row->filter.kinds.types[2]);

    weapon = player_state.equipped_weapon_record;
    model.t[0] = (s16)weapon->position_offset_x;
    model.t[1] = (s16)weapon->position_offset_y;
    model.t[2] = (s16)weapon->position_offset_z;
    RotMatrix((SVECTOR *)&weapon->rotation_offset_x, &model);
    SetRotMatrix(&model);
    SetTransMatrix(&model);
    asset_registry_select(0x20);
    object = tmd_get_object(0);
    if (animation_prepare_asset_vertices(&player_state.weapon_animation_cache, 0x20,
                      player_state.weapon_attack_mode,
                      player_state.weapon_attack_phase,
                      object->vertex_count) != 0) {
        tmd_project_vertices_with_fog(object->vertex_count);
        render_enqueue_textured_tmd(0, 100);
    }
}
