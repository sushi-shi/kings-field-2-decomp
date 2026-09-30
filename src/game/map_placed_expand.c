#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/map_placed.h>
#include <psyq/libc.h>

extern s32 func_8002b67c(s32 layer, s32 x, s32 z, s32 radius, s32 height);

enum { KF_MAP_PLACED_REGION_SHIFT = 11, KF_MAP_PLACED_RANDOM_SHIFT = 15 };

ADDRESS(0x80034818, 0x134)
void func_80034818(const KfMapPlacedSource *sources)
{
    KfMapPlacedEntry *entry = game_graphics_runtime.map_placed_entries;
    s32 remaining;

    for (remaining = KF_MAP_PLACED_ENTRY_COUNT - 1; remaining != -1; --remaining) {
        if (sources->id != 0xffff) {
            entry->id = sources->id;
            entry->layer = sources->layer;
            entry->frame_count = sources->frame_count;
            entry->frame_period = sources->frame_period;
            entry->position.vx = (sources->region_x << KF_MAP_PLACED_REGION_SHIFT) + sources->local_x;
            entry->position.vz = (sources->region_z << KF_MAP_PLACED_REGION_SHIFT) + sources->local_z;
            entry->position.vy = func_8002b67c(entry->layer, entry->position.vx, entry->position.vz, 0, 0)
                + sources->height_offset;
            entry->frame_index = (rand() * entry->frame_count) >> KF_MAP_PLACED_RANDOM_SHIFT;
        } else {
            entry->id = 0xffff;
        }
        entry++;
        sources++;
    }
}
