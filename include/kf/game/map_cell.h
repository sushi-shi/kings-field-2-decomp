#ifndef KF_GAME_MAP_CELL_H
#define KF_GAME_MAP_CELL_H

#include <kf/game/player.h>
#include <kf/game/render_types.h>

enum {
    KF_MAP_CELL_POSITION_SHIFT = 11,
    KF_MAP_CELL_ELEVATION_SHIFT = 7,
    KF_MAP_CELL_NO_OBJECT_INDEX = 0xff
};

void render_map_cell_object(const KfMapCellShape *shape, SVECTOR *position,
                            KfMapLayerMask flags);
s32 collision_sample_map_layer_height(KfMapLayerMask layer, s32 x, s32 z, s32 radius, s32 height);
void map_cell_add_layer_occupancy(s32 x, s32 z, s32 radius, s32 amount);

#endif
