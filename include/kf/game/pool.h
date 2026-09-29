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

struct KfMorphObject;

typedef struct KfPoolRecord {
    s16 state;
    u16 asset_index;
    u16 clip_index;
    u16 keyframe_index;
    struct KfMorphObject *rest_morph;
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

#endif
