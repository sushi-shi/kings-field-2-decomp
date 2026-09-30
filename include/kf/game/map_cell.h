#ifndef KF_GAME_MAP_CELL_H
#define KF_GAME_MAP_CELL_H

#include <kf/game/player.h>

void render_map_cell_object(const KfMapCellShape *shape, SVECTOR *position,
                            u32 flags);

#endif
