#ifndef KF_GAME_POOL_H
#define KF_GAME_POOL_H

#include <kf/lib/types.h>
#include <kf/lib/enum.h>
#include <psyq/sdk.h>

enum { KF_ANIMATION_CACHE_CAPACITY = 12 };

enum class KfAnimationCacheState : u8 {
    KF_ANIMATION_CACHE_FREE = 0,
    KF_ANIMATION_CACHE_STALE = 1,
    KF_ANIMATION_CACHE_LIVE = 2
}; using enum KfAnimationCacheState;

enum class KfAnimationClip : u8 {
    KF_ANIMATION_CLIP_FIRST = 0,
    KF_ANIMATION_CLIP_SECOND = 1,
    KF_ANIMATION_CLIP_THIRD = 2,
    KF_ANIMATION_CLIP_STATIC_OBJECT_FIRST = 0x80,
    KF_ANIMATION_CLIP_STATIC_OBJECT_SECOND = 0x81,
    KF_ANIMATION_CLIP_NONE = 0xff
}; using enum KfAnimationClip;

enum { KF_ASSET_OBJECT_INDEX_MASK = 0x7f };

typedef struct KfPoolRecord {
    KfEnumStorage<KfAnimationCacheState, s16> state;
    u16 asset_index;
    KfEnumStorage<KfAnimationClip, u16> clip_index;
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
s32 animation_prepare_asset_vertices(KfPoolRecord **owner_slot, s32 asset_index,
                  KfAnimationClip clip,
                  s32 phase, s32 vertex_count);

#endif
