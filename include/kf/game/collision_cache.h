#ifndef KF_GAME_COLLISION_CACHE_H
#define KF_GAME_COLLISION_CACHE_H

#include <kf/game/player.h>

s32 func_8002a988(s32 x, s32 y, s32 z);

/* Temporary interior view of the complete startup-cleared BSS object. The
 * boundary with the provisional equipment-record view is still unresolved. */
#define KF_COLLISION_CACHE_CELL \
    (*(KfMapOccupancyCell **)((u8 *)&bss_801c7540 + 0x11800))
#define KF_COLLISION_CACHE_SHAPE \
    (*(u8 **)((u8 *)&bss_801c7540 + 0x11804))
#define KF_COLLISION_CACHE_LAYER \
    (*(u16 *)((u8 *)&bss_801c7540 + 0x1180a))
#define KF_COLLISION_CACHE_HEIGHT \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x1180c))
#define KF_COLLISION_CACHE_RESULT \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11810))
/* The cache's complete layout still overlaps the provisional equipment view. */
#define KF_COLLISION_CACHE_LOWER_BOUND \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11818))
#define KF_COLLISION_CACHE_UPPER_BOUND \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x1181c))
#define KF_COLLISION_CACHE_FLAGS \
    (*(u32 *)((u8 *)&bss_801c7540 + 0x11820))
#define KF_COLLISION_CACHE_ACTOR_INDEX \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11824))
#define KF_COLLISION_CACHE_OBJECT_INDEX \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11828))
#define KF_COLLISION_CACHE_POSITION \
    (*(VECTOR *)((u8 *)&bss_801c7540 + 0x11830))
#define KF_COLLISION_CACHE_RADIUS \
    (*(u16 *)((u8 *)&bss_801c7540 + 0x11840))
#define KF_COLLISION_CACHE_INTERACTION_HEIGHT \
    (*(u16 *)((u8 *)&bss_801c7540 + 0x11842))

#endif
