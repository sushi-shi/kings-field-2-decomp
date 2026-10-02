#ifndef KF_GAME_POOL_H
#define KF_GAME_POOL_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

/* Twelve-entry animation vertex cache and its per-frame lifecycle. */
enum {
    KF_ANIMATION_CACHE_FREE = 0,
    KF_ANIMATION_CACHE_STALE = 1,
    KF_ANIMATION_CACHE_LIVE = 2,
    KF_ANIMATION_CACHE_CAPACITY = 12,
    KF_ANIMATION_CLIP_NONE = 0xff
};

typedef struct KfPoolRecord {
    s16 state;
    u16 asset_index;
    u16 clip_index;
    u16 keyframe_index;
    u32 rest_morph_offset;
    SVECTOR *cached_vertices;
    struct KfPoolRecord **owner_slot;
} KfPoolRecord;

typedef char kf_pool_record_size[sizeof(KfPoolRecord) == 20 ? 1 : -1];

void pool_reset(void);
void pool_mark_allocated(void);
void pool_record_release(KfPoolRecord *record);
void pool_release_all(void);
void pool_release_stale(void);
KfPoolRecord *pool_allocate(void);
s32 animation_prepare_asset_vertices(KfPoolRecord **owner_slot, s32 asset_index, s32 clip,
                  s32 phase, s32 vertex_count);

#endif
