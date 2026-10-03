#include <kf/lib/address.h>
#include <kf/game/animation.h>
#include <kf/game/asset.h>
#include <kf/game/graphics.h>
#include <kf/game/memory.h>
#include <kf/game/tmd.h>

enum { KF_ANIMATION_BLEND_ONE = 0x1000, KF_ANIMATION_BLEND_SHIFT = 12 };

ADDRESS(0x80033b34, 0xc8)
KfAnimKeyframe *animation_select_keyframe(KfAssetHeader *asset, s32 clip_index, s32 phase,
                                          s32 *keyframe_index, u32 *blend_fraction)
{
    u32 *clip_table = (u32 *)((u8 *)asset + asset->clip_table_offset);
    KfAnimClip *clip = (KfAnimClip *)((u8 *)asset + clip_table[clip_index]);
    u32 *offsets = clip->keyframe_offsets;
    s32 remaining = clip->keyframe_count;
    s32 index = 0;
    s32 phase_end = 0;
    s32 phase_start = 0;
    u32 fraction;
    KfAnimKeyframe *keyframe;

    for (--remaining; remaining != -1; --remaining) {
        keyframe = (KfAnimKeyframe *)((u8 *)asset + *offsets++);

        phase_end += keyframe->duration;
        if (phase < phase_end) {
            fraction = ((u32)(phase - phase_start) << KF_ANIMATION_BLEND_SHIFT)
                / keyframe->duration;
            if (keyframe->reverse != 0) {
                fraction = KF_ANIMATION_BLEND_ONE - fraction;
            }
            goto selected;
        }
        phase_start = phase_end;
        index++;
    }
    index--;
    fraction = KF_ANIMATION_BLEND_ONE;

selected:
    *keyframe_index = index;
    *blend_fraction = fraction;
    return keyframe;
}

enum { KF_ANIMATION_SPARSE_SKIP = -32768 };

ADDRESS(0x80033bfc, 0xc4)
void animation_expand_sparse_vertices(SVECTOR *vertices, const SVECTOR *base,
                                      const s16 *encoded)
{
    s32 remaining = *encoded++;

    for (--remaining; remaining != -1; --remaining) {
        u16 value = *encoded++;

        if ((s16)value == KF_ANIMATION_SPARSE_SKIP) {
            s32 copy_count = *encoded++;

            for (--copy_count; copy_count != -1; --copy_count) {
                copyVector(vertices, base);
                vertices++;
                base++;
            }
        } else {
            vertices->vx = value;
            vertices->vy = *encoded++;
            vertices->vz = *encoded++;
            vertices++;
            base++;
        }
    }
}

ADDRESS(0x80033cc0, 0x7c)
void animation_decode_sparse_vertices(SVECTOR *vertices, const s16 *encoded)
{
    s32 remaining = *encoded++;

    for (--remaining; remaining != -1; --remaining) {
        u16 value = *encoded++;

        if ((s16)value == KF_ANIMATION_SPARSE_SKIP) {
            vertices += *encoded++;
        } else {
            vertices->vx = value;
            vertices->vy = *encoded++;
            vertices->vz = *encoded++;
            vertices++;
        }
    }
}

ADDRESS(0x80033d3c, 0x2b8)
void animation_apply_sparse_morph(SVECTOR *vertices, const s16 *encoded, s32 blend_fraction)
{
    MATRIX deltas;
    VECTOR scale;
    SVECTOR *group_start;
    SVECTOR *cursor;
    s32 pending;
    s32 remaining;
    s16 *delta_write;

    scale.vz = blend_fraction;
    scale.vy = blend_fraction;
    scale.vx = blend_fraction;
    remaining = *encoded;
    cursor = vertices;
    encoded++;
    pending = 0;
    group_start = cursor;
    delta_write = &deltas.m[0][0];

    for (--remaining; remaining != -1; --remaining) {
        s16 value = *encoded++;

        if (value == KF_ANIMATION_SPARSE_SKIP) {
            cursor += *encoded++;
            if (pending != 0) {
                s16 *scaled;

                ScaleMatrix(&deltas, &scale);
                scaled = &deltas.m[0][0];
                for (--pending; pending != -1; --pending) {
                    group_start->vx += *scaled++;
                    group_start->vy += *scaled++;
                    group_start->vz += *scaled++;
                    group_start++;
                }
                pending = 0;
                delta_write = &deltas.m[0][0];
            }
            group_start = cursor;
        } else {
            *delta_write++ = value - cursor->vx;
            *delta_write++ = *encoded++ - cursor->vy;
            *delta_write++ = *encoded++ - cursor->vz;
            cursor++;

            if (pending == 2) {
                ScaleMatrix(&deltas, &scale);
                group_start[0].vx += deltas.m[0][0];
                group_start[0].vy += deltas.m[0][1];
                group_start[0].vz += deltas.m[0][2];
                group_start[1].vx += deltas.m[1][0];
                group_start[1].vy += deltas.m[1][1];
                group_start[1].vz += deltas.m[1][2];
                group_start[2].vx += deltas.m[2][0];
                pending = 0;
                group_start[2].vy += deltas.m[2][1];
                group_start[2].vz += deltas.m[2][2];
                delta_write = &deltas.m[0][0];
                group_start = cursor;
            } else {
                ++pending;
            }
        }
    }

    if (pending != 0) {
        s16 *scaled;

        ScaleMatrix(&deltas, &scale);
        scaled = &deltas.m[0][0];
        for (--pending; pending != -1; --pending) {
            group_start->vx += *scaled++;
            group_start->vy += *scaled++;
            group_start->vz += *scaled++;
            group_start++;
        }
    }
}

ADDRESS(0x80033ff4, 0x7c)
const s16 *animation_find_sparse_vertex(const s16 *encoded, s32 vertex_index)
{
    s32 remaining = *encoded;
    s32 current = 0;

    encoded++;
    for (--remaining; remaining != -1; --remaining) {
        s16 value = *encoded;

        if (value == KF_ANIMATION_SPARSE_SKIP) {
            encoded++;
            current += *encoded++;
            if (vertex_index < current) {
                return 0;
            }
        } else {
            if (current == vertex_index) {
                return encoded;
            }
            current++;
            encoded += 3;
        }
    }
    return 0;
}

enum { KF_ASSET_OBJECT_SELECT_BIT = 0x80, KF_ASSET_OBJECT_INDEX_MASK = 0x7f };

ADDRESS(0x80034070, 0x2d4)
s32 animation_prepare_asset_vertices(KfPoolRecord **owner_slot, s32 asset_index, s32 clip,
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
        morph_offsets = (u32 *)((u8 *)asset + asset->morph_offsets_offset);
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

    {
        u16 copy_count = vertex_count;
        const u32 *source = (const u32 *)record->cached_vertices;
        u32 *destination = (u32 *)game_graphics_runtime.animation_vertex_scratch;

        do {
            *destination++ = *source++;
            *destination++ = *source++;
        } while (--copy_count != 0);
    }
    animation_apply_sparse_morph(game_graphics_runtime.animation_vertex_scratch,
                   (const s16 *)((u8 *)asset + record->rest_morph_offset),
                   blend_fraction);
    tmd_set_current_vertices(game_graphics_runtime.animation_vertex_scratch);
    record->state = KF_ANIMATION_CACHE_LIVE;
    return (s32)record;
}

ADDRESS(0x80034344, 0x2a0)
s32 animation_sample_vertex(s32 asset_index, s32 clip, s32 phase, s32 vertex_index,
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
    morph_offsets = (u32 *)((u8 *)asset + asset->morph_offsets_offset);
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
