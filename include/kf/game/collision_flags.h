#ifndef KF_GAME_COLLISION_FLAGS_H
#define KF_GAME_COLLISION_FLAGS_H

#include <kf/lib/types.h>
#include <kf/lib/enum.h>

enum class KfCollisionQuery : s32 {
    KF_COLLISION_QUERY_NONE = 0,
    KF_COLLISION_QUERY_SHAPES = 0x01,
    KF_COLLISION_QUERY_LAYER_FLAG_40 = 0x02,
    KF_COLLISION_QUERY_ACTORS = 0x10,
    KF_COLLISION_QUERY_MAP_OBJECTS = 0x20,
    KF_COLLISION_QUERY_ACTORS_INCLUDE_TYPE3 = 0x40,
    KF_COLLISION_QUERY_PLAYER = 0x80
}; using enum KfCollisionQuery;
constexpr KfCollisionQuery operator|(KfCollisionQuery lhs, KfCollisionQuery rhs)
    { return static_cast<KfCollisionQuery>(static_cast<s32>(lhs) | static_cast<s32>(rhs)); }
    constexpr KfCollisionQuery operator&(KfCollisionQuery lhs, KfCollisionQuery rhs)
    { return static_cast<KfCollisionQuery>(static_cast<s32>(lhs) & static_cast<s32>(rhs)); }
    constexpr KfCollisionQuery operator^(KfCollisionQuery lhs, KfCollisionQuery rhs)
    { return static_cast<KfCollisionQuery>(static_cast<s32>(lhs) ^ static_cast<s32>(rhs)); }
    constexpr KfCollisionQuery operator~(KfCollisionQuery value)
    { return static_cast<KfCollisionQuery>(~static_cast<s32>(value)); }
    inline KfCollisionQuery& operator|=(KfCollisionQuery& lhs, KfCollisionQuery rhs) { return lhs = lhs | rhs; }
    inline KfCollisionQuery& operator&=(KfCollisionQuery& lhs, KfCollisionQuery rhs) { return lhs = lhs & rhs; }
    inline KfCollisionQuery& operator^=(KfCollisionQuery& lhs, KfCollisionQuery rhs) { return lhs = lhs ^ rhs; }

enum class KfCollisionHitFlags : u32 {
    KF_COLLISION_HIT_NONE = 0,
    KF_COLLISION_HIT_AXIS = 0x01,
    KF_COLLISION_HIT_DIAGONAL = 0x02,
    KF_COLLISION_HIT_FLOOR = 0x04,
    KF_COLLISION_HIT_HEIGHT_LIMIT = 0x08,
    KF_COLLISION_HIT_SHAPE_MASK = 0x0f,
    KF_COLLISION_HIT_ACTOR = 0x10,
    KF_COLLISION_HIT_MAP_OBJECT = 0x20,
    KF_COLLISION_HIT_PLAYER = 0x80,

    KF_COLLISION_HIT_LEDGE = 0x100,
    KF_COLLISION_IMPACT_HOLD_ACTOR_ANIMATION = 0x10000,
    KF_COLLISION_IMPACT_COUNTS_AS_PHYSICAL = 0x20000,
    KF_COLLISION_IMPACT_OPTION_MASK = 0xf0000
}; using enum KfCollisionHitFlags;
constexpr KfCollisionHitFlags operator|(KfCollisionHitFlags lhs, KfCollisionHitFlags rhs)
    { return static_cast<KfCollisionHitFlags>(static_cast<u32>(lhs) | static_cast<u32>(rhs)); }
    constexpr KfCollisionHitFlags operator&(KfCollisionHitFlags lhs, KfCollisionHitFlags rhs)
    { return static_cast<KfCollisionHitFlags>(static_cast<u32>(lhs) & static_cast<u32>(rhs)); }
    constexpr KfCollisionHitFlags operator^(KfCollisionHitFlags lhs, KfCollisionHitFlags rhs)
    { return static_cast<KfCollisionHitFlags>(static_cast<u32>(lhs) ^ static_cast<u32>(rhs)); }
    constexpr KfCollisionHitFlags operator~(KfCollisionHitFlags value)
    { return static_cast<KfCollisionHitFlags>(~static_cast<u32>(value)); }
    inline KfCollisionHitFlags& operator|=(KfCollisionHitFlags& lhs, KfCollisionHitFlags rhs) { return lhs = lhs | rhs; }
    inline KfCollisionHitFlags& operator&=(KfCollisionHitFlags& lhs, KfCollisionHitFlags rhs) { return lhs = lhs & rhs; }
    inline KfCollisionHitFlags& operator^=(KfCollisionHitFlags& lhs, KfCollisionHitFlags rhs) { return lhs = lhs ^ rhs; }

#endif
