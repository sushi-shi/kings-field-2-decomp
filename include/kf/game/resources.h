#ifndef KF_GAME_RESOURCES_H
#define KF_GAME_RESOURCES_H

#include <kf/lib/types.h>

void tim_upload_images(u8 *tim_data);
void resource_tmd_read_complete(u8 *data);
void resource_tmd_queue_read(s32 archive_slot, s32 entry, s32 registry_index);
void resource_vab_update_range(s32 archive_slot, s32 entry, s32 vab_slot,
    s32 count, u8 *flags);
void resource_tmd_update_range(s32 archive_slot, s32 entry, s32 registry_index,
    s32 count, u8 *flags);

#endif
