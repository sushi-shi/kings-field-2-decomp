#include <kf/lib/address.h>
#include <kf/game/animation.h>
#include <kf/game/asset.h>
#include <kf/game/graphics.h>
#include <kf/game/memory.h>
#include <kf/game/tmd.h>

enum { KF_ASSET_OBJECT_SELECT_BIT = 0x80, KF_ASSET_OBJECT_INDEX_MASK = 0x7f };

ADDRESS(0x80034070, 0x2d4)
s32 func_80034070(KfPoolRecord **owner_slot, s32 asset_index, s32 clip,
                  s32 phase, s32 vertex_count)
{
    KfAssetHeader *asset = game_graphics_runtime.asset_registry_entries[asset_index];
    KfPoolRecord *record = *owner_slot;
    KfAnimKeyframe *keyframe;
    u32 *morph_offsets;
    u16 *morph_indices;
    u16 remaining;
    s32 keyframe_index;
    u32 blend_fraction;

    if (asset->animation_present == 0) {
        if (record != 0) {
            pool_record_release(record);
        }
        asset_registry_select(asset_index);
        tmd_select_object_vertices(0);
        return 1;
    }

    if (record == 0) {
        record = pool_allocate();
        if (record == 0) {
            return 0;
        }
allocate_vertices:
        record->asset_index = asset_index;
        record->owner_slot = owner_slot;
        for (;;) {
            record->cached_vertices = (SVECTOR *)memory_malloc_checked(vertex_count * sizeof(SVECTOR));
            if (record->cached_vertices != 0) {
                break;
            }
            pool_release_all();
        }
        *owner_slot = record;
    } else if (record->asset_index != asset_index) {
        pool_record_release(record);
        record->clip_index = KF_ANIMATION_CLIP_NONE;
        goto allocate_vertices;
    }

    keyframe = animation_select_keyframe(asset, clip, phase, &keyframe_index,
                                          &blend_fraction);
    if (record->clip_index != clip || record->keyframe_index != keyframe_index) {
        tmd_select_object_vertices(0);
        morph_offsets = (u32 *)((u8 *)asset + asset->unknown_0c);
        remaining = keyframe->morph_count;
        if (remaining != 0) {
            morph_indices = (u16 *)(keyframe + 1);
            animation_expand_sparse_vertices(
                record->cached_vertices, game_graphics_runtime.current_tmd_vertices,
                (const s16 *)((u8 *)asset + morph_offsets[*morph_indices++]));
            for (--remaining; remaining != 0; --remaining) {
                animation_decode_sparse_vertices(
                    record->cached_vertices,
                    (const s16 *)((u8 *)asset + morph_offsets[*morph_indices++]));
            }
        } else {
            const u32 *source = (const u32 *)game_graphics_runtime.current_tmd_vertices;
            u32 *destination = (u32 *)record->cached_vertices;
            u16 copy_count = vertex_count;

            do {
                *destination++ = *source++;
                *destination++ = *source++;
            } while (--copy_count != 0);
        }
        record->rest_morph_offset = morph_offsets[keyframe->rest_index];
    }
    record->clip_index = clip;
    record->keyframe_index = keyframe_index;

    /* The destination begins four bytes into an otherwise unresolved graphics span. */
    {
        u16 copy_count = vertex_count;
        const u32 *source = (const u32 *)record->cached_vertices;
        u32 *destination = (u32 *)&game_graphics_runtime.unknown_12a50[4];

        do {
            *destination++ = *source++;
            *destination++ = *source++;
        } while (--copy_count != 0);
    }
    func_80033d3c((SVECTOR *)&game_graphics_runtime.unknown_12a50[4],
                   (const s16 *)((u8 *)asset + record->rest_morph_offset),
                   blend_fraction);
    tmd_set_current_vertices((SVECTOR *)&game_graphics_runtime.unknown_12a50[4]);
    record->state = KF_ANIMATION_CACHE_LIVE;
    return (s32)record;
}

ADDRESS(0x80034344, 0x2a0)
s32 func_80034344(s32 asset_index, s32 clip, s32 phase, s32 vertex_index,
                  SVECTOR *output)
{
    KfAssetHeader *asset = resource_registry_get(asset_index);
    KfTmdHeader *tmd;
    SVECTOR *vertices;
    KfAnimKeyframe *keyframe;
    SVECTOR vertex;
    u32 *morph_offsets;
    u16 *morph_indices;
    const s16 *encoded;
    s32 remaining;
    s32 keyframe_index;
    u32 blend_fraction;

    if (asset == 0) {
        output->vz = 0;
        output->vy = 0;
        output->vx = 0;
        return 1;
    }

    tmd = (KfTmdHeader *)((u8 *)asset + asset->tmd_data_offset);
    if (clip >= KF_ASSET_OBJECT_SELECT_BIT) {
copy_object_vertex:
        vertices = TMD_OBJECT_VERTICES(tmd, &TMD_OBJECTS(tmd)[clip & KF_ASSET_OBJECT_INDEX_MASK]);
        *output = vertices[vertex_index];
        goto finished;
    }
    if (asset->animation_present == 0) {
        clip = 0;
        goto copy_object_vertex;
    }

    vertices = TMD_OBJECT_VERTICES(tmd, &TMD_OBJECTS(tmd)[0]);
    keyframe = animation_select_keyframe(asset, clip, phase, &keyframe_index,
                                          &blend_fraction);
    vertex = vertices[vertex_index];
    morph_offsets = (u32 *)((u8 *)asset + asset->unknown_0c);
    remaining = keyframe->morph_count;
    morph_indices = (u16 *)(keyframe + 1);
    for (--remaining; remaining != -1; --remaining) {
        encoded = animation_find_sparse_vertex(
            (const s16 *)((u8 *)asset + morph_offsets[*morph_indices++]),
            vertex_index);
        if (encoded != 0) {
            vertex.vx = *encoded++;
            vertex.vy = *encoded++;
            vertex.vz = *encoded;
        }
    }
    encoded = animation_find_sparse_vertex(
        (const s16 *)((u8 *)asset + morph_offsets[keyframe->rest_index]),
        vertex_index);
    if (encoded != 0) {
        vertex.vx = (((*encoded++ - vertex.vx) * (s32)blend_fraction) >> 12) + vertex.vx;
        vertex.vy = (((*encoded - vertex.vy) * (s32)blend_fraction) >> 12) + vertex.vy;
        vertex.vz = (((encoded[1] - vertex.vz) * (s32)blend_fraction) >> 12) + vertex.vz;
    }
    *output = vertex;
finished:
    return 0;
}

ADDRESS(0x800345e4, 0x60)
u32 asset_vertex_count(s32 asset_index, s32 encoded_object_index)
{
    KfAssetHeader *asset = game_graphics_runtime.asset_registry_entries[asset_index];
    KfTmdHeader *tmd;

    if (asset == 0) {
        return 0;
    }
    tmd = (KfTmdHeader *)((u8 *)asset + asset->tmd_data_offset);
    if (encoded_object_index >= KF_ASSET_OBJECT_SELECT_BIT) {
        return TMD_OBJECTS(tmd)[encoded_object_index & KF_ASSET_OBJECT_INDEX_MASK].vertex_count;
    }
    return TMD_OBJECTS(tmd)[0].vertex_count;
}
