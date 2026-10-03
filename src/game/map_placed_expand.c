#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_placed.h>
#include <psyq/libc.h>

enum { KF_MAP_PLACED_REGION_SHIFT = 11, KF_MAP_PLACED_RANDOM_SHIFT = 15 };

ADDRESS(0x80034818, 0x134)
void map_placed_expand_sources(const KfMapPlacedSource *sources)
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
            entry->position.vy = collision_sample_map_layer_height(entry->layer, entry->position.vx, entry->position.vz, 0, 0)
                + sources->height_offset;
            entry->frame_index = (rand() * entry->frame_count) >> KF_MAP_PLACED_RANDOM_SHIFT;
        } else {
            entry->id = 0xffff;
        }
        entry++;
        sources++;
    }
}
