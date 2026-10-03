#include <kf/lib/address.h>
#include <kf/game/collision_cache.h>

DATA(0x800667fc, 0xa)
KfMapOccupancyCell collision_default_cell = {
    {{0xff, 0, 0, 0x3f, 0}, {0xff, 0, 0, 0x3f, 0}}
};

#define COLLISION_CACHE_CELL KF_COLLISION_CACHE_CELL
#define COLLISION_CACHE_LAYER KF_COLLISION_CACHE_LAYER
#define COLLISION_CACHE_HEIGHT KF_COLLISION_CACHE_HEIGHT

ADDRESS(0x8002a988, 0x11c)
s32 collision_sample_map_cell_layer(s32 x, s32 y, s32 z)
{
    KfMapOccupancyCell *cell;
    s32 elevation;

    if ((u32)x <= 0x27fff && (u32)z <= 0x27fff) {
        u16 height;

        cell = &bss_801c7540.map_cells[z >> 11][x >> 11];
        height = (u16)-(y >> 7);
        if (cell->layer[0].elevation > cell->layer[1].elevation) {
            if (height < cell->layer[0].elevation && cell->layer[1].elevation != 0) {
                goto second_layer;
            }
            goto first_layer;
        }
        goto compare_second;
first_layer:
        COLLISION_CACHE_LAYER = 0;
        elevation = -(s32)cell->layer[0].elevation;
        goto scale_height;
second_layer:
        COLLISION_CACHE_LAYER = 5;
        elevation = -(s32)cell->layer[1].elevation;
scale_height:
        COLLISION_CACHE_HEIGHT = elevation * 128;
        goto selected;
compare_second:
        if (height < cell->layer[1].elevation && cell->layer[0].elevation != 0) {
            goto first_layer;
        }
        goto second_layer;
selected:
    } else {
        cell = &collision_default_cell;
        COLLISION_CACHE_LAYER = 0;
        COLLISION_CACHE_HEIGHT = 0;
    }

    COLLISION_CACHE_CELL = cell;
    return COLLISION_CACHE_HEIGHT;
}
