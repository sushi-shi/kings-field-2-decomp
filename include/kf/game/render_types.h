#ifndef KF_GAME_RENDER_TYPES_H
#define KF_GAME_RENDER_TYPES_H

#include <kf/lib/types.h>
#include <kf/lib/enum.h>

enum class KfMapLayerMask : u8 {
    KF_MAP_LAYER_NONE = 0,
    KF_MAP_LAYER_FIRST = 1,
    KF_MAP_LAYER_SECOND = 2,
    KF_MAP_LAYER_BOTH = KF_MAP_LAYER_FIRST | KF_MAP_LAYER_SECOND,

    KF_MAP_LAYER_IN_VIEW = 0x20,

    KF_MAP_LAYER_NEAR_PREPARE = 0x40,

    KF_MAP_LAYER_NEAR_CLIPPED = 0x80
}; using enum KfMapLayerMask;
constexpr KfMapLayerMask operator|(KfMapLayerMask lhs, KfMapLayerMask rhs)
    { return static_cast<KfMapLayerMask>(static_cast<u8>(lhs) | static_cast<u8>(rhs)); }
    constexpr KfMapLayerMask operator&(KfMapLayerMask lhs, KfMapLayerMask rhs)
    { return static_cast<KfMapLayerMask>(static_cast<u8>(lhs) & static_cast<u8>(rhs)); }
    constexpr KfMapLayerMask operator^(KfMapLayerMask lhs, KfMapLayerMask rhs)
    { return static_cast<KfMapLayerMask>(static_cast<u8>(lhs) ^ static_cast<u8>(rhs)); }
    constexpr KfMapLayerMask operator~(KfMapLayerMask value)
    { return static_cast<KfMapLayerMask>(~static_cast<u8>(value)); }
    inline KfMapLayerMask& operator|=(KfMapLayerMask& lhs, KfMapLayerMask rhs) { return lhs = lhs | rhs; }
    inline KfMapLayerMask& operator&=(KfMapLayerMask& lhs, KfMapLayerMask rhs) { return lhs = lhs & rhs; }
    inline KfMapLayerMask& operator^=(KfMapLayerMask& lhs, KfMapLayerMask rhs) { return lhs = lhs ^ rhs; }

enum class KfSpriteState : u8 {
    KF_SPRITE_HIDDEN = 0,
    KF_SPRITE_VISIBLE = 1,
    KF_SPRITE_END = 0xff
}; using enum KfSpriteState;

enum class KfColorOverlayControl : u8 {
    KF_COLOR_OVERLAY_NO_FLAGS = 0,
    KF_COLOR_OVERLAY_ADD = 1,
    KF_COLOR_OVERLAY_SUBTRACT = 2,
    KF_COLOR_OVERLAY_BLEND_MASK = 3,
    KF_COLOR_OVERLAY_FRONT = 0x80,
    KF_COLOR_OVERLAY_OFF = 0xff
}; using enum KfColorOverlayControl;
constexpr KfColorOverlayControl operator|(KfColorOverlayControl lhs, KfColorOverlayControl rhs)
    { return static_cast<KfColorOverlayControl>(static_cast<u8>(lhs) | static_cast<u8>(rhs)); }
    constexpr KfColorOverlayControl operator&(KfColorOverlayControl lhs, KfColorOverlayControl rhs)
    { return static_cast<KfColorOverlayControl>(static_cast<u8>(lhs) & static_cast<u8>(rhs)); }
    constexpr KfColorOverlayControl operator^(KfColorOverlayControl lhs, KfColorOverlayControl rhs)
    { return static_cast<KfColorOverlayControl>(static_cast<u8>(lhs) ^ static_cast<u8>(rhs)); }
    constexpr KfColorOverlayControl operator~(KfColorOverlayControl value)
    { return static_cast<KfColorOverlayControl>(~static_cast<u8>(value)); }
    inline KfColorOverlayControl& operator|=(KfColorOverlayControl& lhs, KfColorOverlayControl rhs) { return lhs = lhs | rhs; }
    inline KfColorOverlayControl& operator&=(KfColorOverlayControl& lhs, KfColorOverlayControl rhs) { return lhs = lhs & rhs; }
    inline KfColorOverlayControl& operator^=(KfColorOverlayControl& lhs, KfColorOverlayControl rhs) { return lhs = lhs ^ rhs; }

#define KF_GPU_TPAGE_ABR(mode) (kf_enum_value(mode) << 5)

enum {
    KF_TEXTURED_QUAD_SEMI_TRANSPARENT = 1,
    KF_TEXTURED_QUAD_OPAQUE = 0xff
};

enum {
    KF_LIGHTING_FLAGS_INDEX_MASK = 0x7f,
    KF_LIGHTING_FLAG_REVERSED_LIGHT = 0x80
};

enum class KfRenderQueueMode : u8 {
    KF_RENDER_QUEUE_BLEND_AVERAGE = 0,
    KF_RENDER_QUEUE_BLEND_ADD = 1,
    KF_RENDER_QUEUE_BLEND_SUBTRACT = 2,
    KF_RENDER_QUEUE_BLEND_ADD_QUARTER = 3,
    KF_RENDER_QUEUE_TEXTURED_UNBIASED = 0x80,
    KF_RENDER_QUEUE_CLIPPED = 0xfe,
    KF_RENDER_QUEUE_TEXTURED = 0xff
}; using enum KfRenderQueueMode;

enum class KfLightingIndex : u8 {
    KF_LIGHTING_HUD_COMPASS = 0x40,
    KF_LIGHTING_HUD = 0x41,
    KF_LIGHTING_PRESET_42 = 0x42,
    KF_LIGHTING_SCALED_EFFECT = 0x43,
    KF_LIGHTING_EFFECT = 0x44,
    KF_LIGHTING_MAP_PLACED = 0x46,
    KF_LIGHTING_ACTOR_DEFAULT = 0x47,
    KF_LIGHTING_PRESET_48 = 0x48,
    KF_LIGHTING_PRESET_49 = 0x49,
    KF_LIGHTING_NONE = 0xff
}; using enum KfLightingIndex;

#endif
