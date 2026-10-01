#ifndef KF_GAME_MAP_CELL_H
#define KF_GAME_MAP_CELL_H

#include <kf/game/player.h>

void render_map_cell_object(const KfMapCellShape *shape, SVECTOR *position,
                            u32 flags);
s32 func_8002b67c(u8 kind, s32 x, s32 z, s32 radius, s32 height);
void func_8002b73c(s32 x, s32 z, s32 radius, s32 amount);

#endif
