#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/graphics.h>
#include <kf/game/pool.h>
#include <psyq/libc.h>

/*
 * The twelve-entry pool that caches per-instance vertex allocations across
 * frames. Records advance free -> live and are marked stale each frame so the
 * render pass can revalidate them before pool_release_stale frees the rest.
 */
ADDRESS(0x80034644, 0x30)
void pool_reset(void)
{
    KfPoolRecord *record = game_graphics_runtime.pool_records;
    u16 records_left = KF_ANIMATION_CACHE_CAPACITY;

    do {
        record->state = KF_ANIMATION_CACHE_FREE;
        record->cached_vertices = NULL;
        record++;
    } while (--records_left != 0);
}

ADDRESS(0x80034674, 0x3c)
void pool_mark_allocated(void)
{
    KfPoolRecord *record = game_graphics_runtime.pool_records;
    u16 records_left = KF_ANIMATION_CACHE_CAPACITY;

    do {
        if (record->state != KF_ANIMATION_CACHE_FREE) {
            record->state = KF_ANIMATION_CACHE_STALE;
        }
        record++;
    } while (--records_left != 0);
}

ADDRESS(0x800346b0, 0x48)
void pool_record_release(KfPoolRecord *record)
{
    record->state = KF_ANIMATION_CACHE_FREE;
    *record->owner_slot = NULL;
    if (record->cached_vertices != NULL) {
        free(record->cached_vertices);
        record->cached_vertices = NULL;
    }
}

ADDRESS(0x800346f8, 0x6c)
void pool_release_all(void)
{
    KfPoolRecord *record = game_graphics_runtime.pool_records;
    s16 records_left;

    for (records_left = KF_ANIMATION_CACHE_CAPACITY - 1; records_left != -1; records_left--) {
        if (record->state != KF_ANIMATION_CACHE_FREE) {
            pool_record_release(record);
        }
        record++;
    }
}

ADDRESS(0x80034764, 0x6c)
void pool_release_stale(void)
{
    KfPoolRecord *record = game_graphics_runtime.pool_records;
    u16 records_left = KF_ANIMATION_CACHE_CAPACITY;

    do {
        if (record->state == KF_ANIMATION_CACHE_STALE) {
            pool_record_release(record);
        }
        record++;
    } while (--records_left != 0);
}

ADDRESS(0x800347d0, 0x48)
KfPoolRecord *pool_allocate(void)
{
    KfPoolRecord *record = game_graphics_runtime.pool_records;
    u16 records_left = KF_ANIMATION_CACHE_CAPACITY;

    do {
        if (record->state == KF_ANIMATION_CACHE_FREE) {
            record->clip_index = KF_ANIMATION_CLIP_NONE;
            return record;
        }
        record++;
    } while (--records_left != 0);
    return NULL;
}
