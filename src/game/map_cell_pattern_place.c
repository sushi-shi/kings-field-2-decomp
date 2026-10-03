#include <kf/lib/address.h>
#include <kf/game/map_cell_pattern.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

enum {
    KF_PATTERN_END = 0xff,
    KF_PATTERN_SKIP_BYTE = 0xfe,
    KF_PATTERN_SKIP_OBJECT = 0xff,
    KF_PATTERN_LAYER_FLAG_MASK = 0x7f,
    KF_PATTERN_MODE_FIRST_LAYER = 1
};

ADDRESS(0x80034f90, 0x204)
void map_cell_apply_rotated_pattern(s32 mode, s32 world_x, s32 world_z, s32 angle,
                   const KfMapCellPattern *patterns, s32 variant_index,
                   s32 layer_flag)
{
    s32 cell_origin_x = world_x >> 11;
    s32 cell_origin_z = world_z >> 11;
    s32 cosine = rcos(angle);
    s32 sine = rsin(angle);
    s32 first_layer_offset = ((mode & 0xff) == KF_PATTERN_MODE_FIRST_LAYER)
                                 ? 0 : sizeof(KfMapOccupancyLayer);
    s32 second_layer_offset = first_layer_offset == 0
                                  ? sizeof(KfMapOccupancyLayer) : 0;

    while (patterns->variant[0].first_unknown_03 != KF_PATTERN_END) {
        s32 local_x = patterns->offset_x;
        s32 local_z = patterns->offset_z;
        s32 cell_x = ((local_x * cosine - local_z * sine) >> 12) +
                     cell_origin_x;
        s32 cell_z = ((local_z * cosine + local_x * sine) >> 12) +
                     cell_origin_z;
        KfMapOccupancyCell *cell = &bss_801c7540.map_cells[cell_z][cell_x];
        const KfMapCellPatternVariant *variant = &patterns->variant[variant_index];
        KfMapOccupancyLayer *first_layer =
            (KfMapOccupancyLayer *)((u8 *)cell + first_layer_offset);
        KfMapOccupancyLayer *second_layer =
            (KfMapOccupancyLayer *)((u8 *)cell + second_layer_offset);

        if (variant->first_unknown_03 != KF_PATTERN_SKIP_BYTE) {
            first_layer->unknown_03 = variant->first_unknown_03;
        }
        if (variant->second_unknown_03 != KF_PATTERN_SKIP_BYTE) {
            second_layer->unknown_03 = variant->second_unknown_03;
        }
        if (variant->first_object_index != KF_PATTERN_SKIP_OBJECT) {
            first_layer->object_index = variant->first_object_index;
        }
        if (variant->second_object_index != KF_PATTERN_SKIP_OBJECT) {
            second_layer->object_index = variant->second_object_index;
        }
        if (layer_flag != KF_PATTERN_END) {
            first_layer->lighting_index =
                (first_layer->lighting_index & KF_PATTERN_LAYER_FLAG_MASK) |
                layer_flag;
            second_layer->lighting_index =
                (second_layer->lighting_index & KF_PATTERN_LAYER_FLAG_MASK) |
                layer_flag;
        }
        patterns++;
    }
}

ADDRESS(0x80035194, 0x370)
void map_cell_copy_rotated_fields(u32 layer_select, s32 source_x, s32 source_z,
                   s32 destination_x, s32 destination_z,
                   s32 width, s32 height, s32 rotation, u32 field_mask)
{
    KfMapOccupancyCell *source_row;
    KfMapOccupancyCell *destination_row;
    s32 inner_step;
    s32 row_step;
    s32 quarter_turns;
    s32 rows_remaining;

    if (width == 0xff) {
        return;
    }
    quarter_turns = -(rotation >> 10) & 3;
    switch (quarter_turns) {
    case 0:
        inner_step = 1;
        row_step = 80;
        break;
    case 1:
        inner_step = -80;
        row_step = 1;
        destination_z += width - 1;
        break;
    case 2:
        inner_step = -1;
        row_step = -80;
        destination_x += width - 1;
        destination_z += height - 1;
        break;
    case 3:
        inner_step = 80;
        row_step = -1;
        destination_x += height - 1;
        break;
    }
    source_row = &bss_801c7540.map_cells[source_z][source_x];
    destination_row = &bss_801c7540.map_cells[destination_z][destination_x];
    rows_remaining = height - 1;
    if (height == 0) {
        return;
    }
    do {
        KfMapOccupancyCell *source = source_row;
        KfMapOccupancyCell *destination = destination_row;
        s32 columns_remaining;
        source_row += 80;
        destination_row += row_step;
        columns_remaining = width - 1;
        if (columns_remaining != -1) {
            do {
            if (layer_select & 1) {
                if (field_mask & 1) {
                    destination->layer[0].object_index = source->layer[0].object_index;
                }
                if (field_mask & 2) {
                    destination->layer[0].elevation = source->layer[0].elevation;
                }
                if (field_mask & 4) {
                    destination->layer[0].quarter_turns =
                        (destination->layer[0].quarter_turns & 0xfc) |
                        ((source->layer[0].quarter_turns + quarter_turns) & 3);
                }
                if (field_mask & 8) {
                    destination->layer[0].unknown_03 = source->layer[0].unknown_03;
                }
                if (field_mask & 0x10) {
                    destination->layer[0].lighting_index =
                        (source->layer[0].lighting_index & 0x3f) |
                        (destination->layer[0].lighting_index & 0xc0);
                }
                if (field_mask & 0x20) {
                    destination->layer[0].lighting_index =
                        (source->layer[0].lighting_index & 0x40) |
                        (destination->layer[0].lighting_index & 0xbf);
                }
                if (field_mask & 0x40) {
                    destination->layer[0].lighting_index =
                        (source->layer[0].lighting_index & 0x80) |
                        (destination->layer[0].lighting_index & 0x7f);
                }
            }
            if (layer_select & 2) {
                if (field_mask & 1) {
                    destination->layer[1].object_index = source->layer[1].object_index;
                }
                if (field_mask & 2) {
                    destination->layer[1].elevation = source->layer[1].elevation;
                }
                if (field_mask & 4) {
                    destination->layer[1].quarter_turns =
                        (destination->layer[1].quarter_turns & 0xfc) |
                        ((source->layer[1].quarter_turns + quarter_turns) & 3);
                }
                if (field_mask & 8) {
                    destination->layer[1].unknown_03 = source->layer[1].unknown_03;
                }
                if (field_mask & 0x10) {
                    destination->layer[1].lighting_index =
                        (source->layer[1].lighting_index & 0x3f) |
                        (destination->layer[1].lighting_index & 0xc0);
                }
                if (field_mask & 0x20) {
                    destination->layer[1].lighting_index =
                        (source->layer[1].lighting_index & 0x40) |
                        (destination->layer[1].lighting_index & 0xbf);
                }
                if (field_mask & 0x40) {
                    destination->layer[1].lighting_index =
                        (source->layer[1].lighting_index & 0x80) |
                        (destination->layer[1].lighting_index & 0x7f);
                }
            }
                source++;
                destination += inner_step;
                columns_remaining--;
            } while (columns_remaining != -1);
        }
        rows_remaining--;
    } while (rows_remaining != -1);
}
