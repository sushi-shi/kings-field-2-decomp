#ifndef KF_GAME_COLLISION_FLAGS_H
#define KF_GAME_COLLISION_FLAGS_H

#include <kf/lib/types.h>
#include <kf/lib/enum.h>

/* What collision_query_world tests. Actor, map-object and player requests
 * use the bit positions that report those hits in KfCollisionHitFlags. */
KF_ENUM_BEGIN(KfCollisionQuery, s32)
    KF_COLLISION_QUERY_NONE = 0,
    KF_COLLISION_QUERY_SHAPES = 0x01,
    KF_COLLISION_QUERY_LAYER_FLAG_40 = 0x02,
    KF_COLLISION_QUERY_ACTORS = 0x10,
    KF_COLLISION_QUERY_MAP_OBJECTS = 0x20,
    KF_COLLISION_QUERY_ACTORS_INCLUDE_TYPE3 = 0x40,
    KF_COLLISION_QUERY_PLAYER = 0x80
KF_ENUM_END(KfCollisionQuery)
KF_ENUM_FLAGS(KfCollisionQuery, s32)

/* Collision query result word, cached in KfCollisionCache.flags. Shape
 * records report the low nibble; actor, map-object and player hits use their
 * query bits. Effect impacts carry option bits 16-19 in the same word to
 * effect_dispatch_magic_impact. */
KF_ENUM_BEGIN(KfCollisionHitFlags, u32)
    KF_COLLISION_HIT_NONE = 0,
    KF_COLLISION_HIT_AXIS = 0x01,
    KF_COLLISION_HIT_DIAGONAL = 0x02,
    KF_COLLISION_HIT_FLOOR = 0x04,
    KF_COLLISION_HIT_HEIGHT_LIMIT = 0x08,
    KF_COLLISION_HIT_SHAPE_MASK = 0x0f,
    KF_COLLISION_HIT_ACTOR = 0x10,
    KF_COLLISION_HIT_MAP_OBJECT = 0x20,
    KF_COLLISION_HIT_PLAYER = 0x80,
    KF_COLLISION_IMPACT_HOLD_ACTOR_ANIMATION = 0x10000,
    KF_COLLISION_IMPACT_COUNTS_AS_PHYSICAL = 0x20000,
    KF_COLLISION_IMPACT_OPTION_MASK = 0xf0000
KF_ENUM_END(KfCollisionHitFlags)
KF_ENUM_FLAGS(KfCollisionHitFlags, u32)

#endif
