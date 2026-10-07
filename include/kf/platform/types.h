#ifndef KF_PLATFORM_TYPES_H
#define KF_PLATFORM_TYPES_H

#include <cstdint>

// Fixed-width names for host code. They live in namespace kf so that host
// translation units can also see the game's own (long-based) u32/s32 names.
namespace kf {
using s8 = std::int8_t;
using u8 = std::uint8_t;
using s16 = std::int16_t;
using u16 = std::uint16_t;
using s32 = std::int32_t;
using u32 = std::uint32_t;
}

#endif // KF_PLATFORM_TYPES_H
