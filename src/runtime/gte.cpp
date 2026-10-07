#include "runtime.h"

#include <kf/psx/sdk.h>

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>

// Geometry library calls evaluated on the host with the coprocessor's
// fixed-point stages: Q12 matrices, 16-bit intermediate saturation, the
// seeded reciprocal used by perspective division and depth-cue interpolation.
// The sine and square-root samples and the reciprocal, projection, lighting and
// matrix arithmetic are carried over from the King's Field port.

namespace kf::psx {
namespace {
using std::int64_t;

struct Geometry {
    MATRIX rotation, light, color;
    s32 back[3], far[3];
    s32 offset_x, offset_y; // Q16
    u16 screen;
    s16 depth_a;
    s32 depth_b;
};
Geometry gte;

constexpr std::array<s16, 1024> quarter_sine = {
    0, 6, 13, 19, 25, 31, 38, 44, 50, 57, 63, 69, 75, 82, 88, 94,
    101, 107, 113, 119, 126, 132, 138, 144, 151, 157, 163, 170, 176, 182, 188, 195,
    201, 207, 214, 220, 226, 232, 239, 245, 251, 257, 264, 270, 276, 283, 289, 295,
    301, 308, 314, 320, 326, 333, 339, 345, 351, 358, 364, 370, 376, 383, 389, 395,
    401, 408, 414, 420, 426, 433, 439, 445, 451, 458, 464, 470, 476, 483, 489, 495,
    501, 508, 514, 520, 526, 533, 539, 545, 551, 557, 564, 570, 576, 582, 589, 595,
    601, 607, 613, 620, 626, 632, 638, 644, 651, 657, 663, 669, 675, 682, 688, 694,
    700, 706, 713, 719, 725, 731, 737, 744, 750, 756, 762, 768, 774, 781, 787, 793,
    799, 805, 811, 818, 824, 830, 836, 842, 848, 854, 861, 867, 873, 879, 885, 891,
    897, 904, 910, 916, 922, 928, 934, 940, 946, 953, 959, 965, 971, 977, 983, 989,
    995, 1001, 1007, 1014, 1020, 1026, 1032, 1038, 1044, 1050, 1056, 1062, 1068, 1074, 1080, 1086,
    1092, 1099, 1105, 1111, 1117, 1123, 1129, 1135, 1141, 1147, 1153, 1159, 1165, 1171, 1177, 1183,
    1189, 1195, 1201, 1207, 1213, 1219, 1225, 1231, 1237, 1243, 1249, 1255, 1261, 1267, 1273, 1279,
    1285, 1291, 1297, 1303, 1309, 1315, 1321, 1327, 1332, 1338, 1344, 1350, 1356, 1362, 1368, 1374,
    1380, 1386, 1392, 1398, 1404, 1409, 1415, 1421, 1427, 1433, 1439, 1445, 1451, 1457, 1462, 1468,
    1474, 1480, 1486, 1492, 1498, 1503, 1509, 1515, 1521, 1527, 1533, 1538, 1544, 1550, 1556, 1562,
    1567, 1573, 1579, 1585, 1591, 1596, 1602, 1608, 1614, 1620, 1625, 1631, 1637, 1643, 1648, 1654,
    1660, 1666, 1671, 1677, 1683, 1689, 1694, 1700, 1706, 1711, 1717, 1723, 1729, 1734, 1740, 1746,
    1751, 1757, 1763, 1768, 1774, 1780, 1785, 1791, 1797, 1802, 1808, 1813, 1819, 1825, 1830, 1836,
    1842, 1847, 1853, 1858, 1864, 1870, 1875, 1881, 1886, 1892, 1898, 1903, 1909, 1914, 1920, 1925,
    1931, 1936, 1942, 1947, 1953, 1958, 1964, 1970, 1975, 1981, 1986, 1992, 1997, 2002, 2008, 2013,
    2019, 2024, 2030, 2035, 2041, 2046, 2052, 2057, 2062, 2068, 2073, 2079, 2084, 2090, 2095, 2100,
    2106, 2111, 2117, 2122, 2127, 2133, 2138, 2143, 2149, 2154, 2159, 2165, 2170, 2175, 2181, 2186,
    2191, 2197, 2202, 2207, 2213, 2218, 2223, 2228, 2234, 2239, 2244, 2249, 2255, 2260, 2265, 2270,
    2276, 2281, 2286, 2291, 2296, 2302, 2307, 2312, 2317, 2322, 2328, 2333, 2338, 2343, 2348, 2353,
    2359, 2364, 2369, 2374, 2379, 2384, 2389, 2394, 2399, 2405, 2410, 2415, 2420, 2425, 2430, 2435,
    2440, 2445, 2450, 2455, 2460, 2465, 2470, 2475, 2480, 2485, 2490, 2495, 2500, 2505, 2510, 2515,
    2520, 2525, 2530, 2535, 2540, 2545, 2550, 2555, 2559, 2564, 2569, 2574, 2579, 2584, 2589, 2594,
    2598, 2603, 2608, 2613, 2618, 2623, 2628, 2632, 2637, 2642, 2647, 2652, 2656, 2661, 2666, 2671,
    2675, 2680, 2685, 2690, 2694, 2699, 2704, 2709, 2713, 2718, 2723, 2727, 2732, 2737, 2741, 2746,
    2751, 2755, 2760, 2765, 2769, 2774, 2779, 2783, 2788, 2792, 2797, 2802, 2806, 2811, 2815, 2820,
    2824, 2829, 2833, 2838, 2843, 2847, 2852, 2856, 2861, 2865, 2870, 2874, 2878, 2883, 2887, 2892,
    2896, 2901, 2905, 2910, 2914, 2918, 2923, 2927, 2932, 2936, 2940, 2945, 2949, 2953, 2958, 2962,
    2967, 2971, 2975, 2979, 2984, 2988, 2992, 2997, 3001, 3005, 3009, 3014, 3018, 3022, 3026, 3031,
    3035, 3039, 3043, 3048, 3052, 3056, 3060, 3064, 3068, 3073, 3077, 3081, 3085, 3089, 3093, 3097,
    3102, 3106, 3110, 3114, 3118, 3122, 3126, 3130, 3134, 3138, 3142, 3146, 3150, 3154, 3158, 3162,
    3166, 3170, 3174, 3178, 3182, 3186, 3190, 3194, 3198, 3202, 3206, 3210, 3214, 3217, 3221, 3225,
    3229, 3233, 3237, 3241, 3244, 3248, 3252, 3256, 3260, 3264, 3267, 3271, 3275, 3279, 3282, 3286,
    3290, 3294, 3297, 3301, 3305, 3309, 3312, 3316, 3320, 3323, 3327, 3331, 3334, 3338, 3342, 3345,
    3349, 3352, 3356, 3360, 3363, 3367, 3370, 3374, 3378, 3381, 3385, 3388, 3392, 3395, 3399, 3402,
    3406, 3409, 3413, 3416, 3420, 3423, 3426, 3430, 3433, 3437, 3440, 3444, 3447, 3450, 3454, 3457,
    3461, 3464, 3467, 3471, 3474, 3477, 3481, 3484, 3487, 3490, 3494, 3497, 3500, 3504, 3507, 3510,
    3513, 3516, 3520, 3523, 3526, 3529, 3532, 3536, 3539, 3542, 3545, 3548, 3551, 3555, 3558, 3561,
    3564, 3567, 3570, 3573, 3576, 3579, 3582, 3585, 3588, 3591, 3594, 3597, 3600, 3603, 3606, 3609,
    3612, 3615, 3618, 3621, 3624, 3627, 3630, 3633, 3636, 3639, 3642, 3644, 3647, 3650, 3653, 3656,
    3659, 3661, 3664, 3667, 3670, 3673, 3675, 3678, 3681, 3684, 3686, 3689, 3692, 3695, 3697, 3700,
    3703, 3705, 3708, 3711, 3713, 3716, 3719, 3721, 3724, 3727, 3729, 3732, 3734, 3737, 3739, 3742,
    3745, 3747, 3750, 3752, 3755, 3757, 3760, 3762, 3765, 3767, 3770, 3772, 3775, 3777, 3779, 3782,
    3784, 3787, 3789, 3791, 3794, 3796, 3798, 3801, 3803, 3805, 3808, 3810, 3812, 3815, 3817, 3819,
    3822, 3824, 3826, 3828, 3831, 3833, 3835, 3837, 3839, 3842, 3844, 3846, 3848, 3850, 3852, 3854,
    3857, 3859, 3861, 3863, 3865, 3867, 3869, 3871, 3873, 3875, 3877, 3879, 3881, 3883, 3885, 3887,
    3889, 3891, 3893, 3895, 3897, 3899, 3901, 3903, 3905, 3907, 3909, 3910, 3912, 3914, 3916, 3918,
    3920, 3921, 3923, 3925, 3927, 3929, 3930, 3932, 3934, 3936, 3937, 3939, 3941, 3943, 3944, 3946,
    3948, 3949, 3951, 3953, 3954, 3956, 3958, 3959, 3961, 3962, 3964, 3965, 3967, 3969, 3970, 3972,
    3973, 3975, 3976, 3978, 3979, 3981, 3982, 3984, 3985, 3987, 3988, 3989, 3991, 3992, 3994, 3995,
    3996, 3998, 3999, 4001, 4002, 4003, 4005, 4006, 4007, 4008, 4010, 4011, 4012, 4014, 4015, 4016,
    4017, 4019, 4020, 4021, 4022, 4023, 4024, 4026, 4027, 4028, 4029, 4030, 4031, 4032, 4034, 4035,
    4036, 4037, 4038, 4039, 4040, 4041, 4042, 4043, 4044, 4045, 4046, 4047, 4048, 4049, 4050, 4051,
    4052, 4053, 4053, 4054, 4055, 4056, 4057, 4058, 4059, 4060, 4060, 4061, 4062, 4063, 4064, 4064,
    4065, 4066, 4067, 4067, 4068, 4069, 4070, 4070, 4071, 4072, 4072, 4073, 4074, 4074, 4075, 4076,
    4076, 4077, 4077, 4078, 4079, 4079, 4080, 4080, 4081, 4081, 4082, 4082, 4083, 4083, 4084, 4084,
    4085, 4085, 4086, 4086, 4087, 4087, 4088, 4088, 4088, 4089, 4089, 4089, 4090, 4090, 4090, 4091,
    4091, 4091, 4092, 4092, 4092, 4092, 4093, 4093, 4093, 4093, 4094, 4094, 4094, 4094, 4094, 4095,
    4095, 4095, 4095, 4095, 4095, 4095, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
};

constexpr std::array<u16, 192> roots = {
    4096, 4127, 4159, 4190, 4222, 4252, 4283, 4314, 4344, 4374, 4404, 4434, 4463, 4492, 4521, 4550,
    4579, 4608, 4636, 4664, 4692, 4720, 4748, 4775, 4802, 4830, 4857, 4884, 4910, 4937, 4964, 4990,
    5016, 5042, 5068, 5094, 5120, 5145, 5170, 5196, 5221, 5246, 5271, 5296, 5320, 5345, 5369, 5394,
    5418, 5442, 5466, 5490, 5514, 5538, 5561, 5585, 5608, 5632, 5655, 5678, 5701, 5724, 5747, 5769,
    5792, 5815, 5837, 5860, 5882, 5904, 5926, 5948, 5970, 5992, 6014, 6036, 6058, 6079, 6101, 6122,
    6144, 6165, 6186, 6207, 6228, 6249, 6270, 6291, 6312, 6333, 6353, 6374, 6394, 6415, 6435, 6456,
    6476, 6496, 6516, 6536, 6556, 6576, 6596, 6616, 6636, 6656, 6675, 6695, 6714, 6734, 6753, 6773,
    6792, 6811, 6830, 6850, 6869, 6888, 6907, 6926, 6945, 6963, 6982, 7001, 7020, 7038, 7057, 7075,
    7094, 7112, 7131, 7149, 7168, 7186, 7204, 7222, 7240, 7258, 7276, 7294, 7312, 7330, 7348, 7366,
    7384, 7401, 7419, 7437, 7454, 7472, 7489, 7507, 7524, 7542, 7559, 7576, 7594, 7611, 7628, 7645,
    7662, 7680, 7697, 7714, 7731, 7747, 7764, 7781, 7798, 7815, 7832, 7848, 7865, 7882, 7898, 7915,
    7931, 7948, 7964, 7981, 7997, 8014, 8030, 8046, 8062, 8079, 8095, 8111, 8127, 8143, 8159, 8175,
    };

s32 sine(s32 angle) {
    const u32 magnitude = (angle < 0 ? 0u - static_cast<u32>(angle) : static_cast<u32>(angle)) & 4095;
    const u32 offset = magnitude & 1023;
    s32 value = 0;
    switch (magnitude >> 10) {
    case 0: value = quarter_sine[offset]; break;
    case 1: value = quarter_sine[1023 - offset]; break;
    case 2: value = -quarter_sine[offset]; break;
    default: value = -quarter_sine[1023 - offset]; break;
    }
    return angle < 0 ? -value : value;
}

s32 cosine(s32 angle) {
    const u32 magnitude = (angle < 0 ? 0u - static_cast<u32>(angle) : static_cast<u32>(angle)) & 4095;
    const u32 offset = magnitude & 1023;
    switch (magnitude >> 10) {
    case 0: return quarter_sine[1023 - offset];
    case 1: return -quarter_sine[offset];
    case 2: return -quarter_sine[1023 - offset];
    default: return quarter_sine[offset];
    }
}

u32 square_root_scaled(u32 value, unsigned result_shift) {
    if (value == 0)
        return 0;
    const unsigned leading = std::countl_zero(value) & ~1u;
    const u32 mantissa = leading >= 24 ? value << (leading - 24) : value >> (24 - leading);
    const unsigned exponent = (31 - leading) / 2;
    return (u32(roots[mantissa - 64]) << exponent) >> result_shift;
}

s16 saturate16(int64_t value) {
    return static_cast<s16>(std::clamp<int64_t>(value, -32768, 32767));
}

u32 projection_scale_q16(u16 distance, u16 depth) {
    constexpr u32 maximum_scale = 0x1ffff;
    if (u32(distance) >= u32(depth) * 2)
        return maximum_scale;
    static constexpr auto seeds = [] {
        std::array<u8, 257> table{};
        for (u32 index = 0; index < table.size(); ++index) {
            const s32 seed = static_cast<s32>((0x40000u / (index + 0x100u) + 1) / 2) - 0x101;
            table[index] = static_cast<u8>(std::max(seed, 0));
        }
        return table;
    }();
    const unsigned shift = std::countl_zero(depth);
    const u32 normalized = u32(depth) << shift;
    const u32 seed = seeds[(normalized - 0x7fc0) >> 7] + 0x101;
    const u32 correction = (0x2000080 - normalized * seed) >> 8;
    const u32 reciprocal = (0x80 + correction * seed) >> 8;
    const uint64_t scaled = uint64_t(u32(distance) << shift) * reciprocal;
    return static_cast<u32>(std::min<uint64_t>((scaled + 0x8000) >> 16, maximum_scale));
}

std::array<s32, 3> rotate(const MATRIX &matrix, const SVECTOR &vector) {
    std::array<s32, 3> result;
    for (int row = 0; row < 3; ++row)
        result[row] = static_cast<s32>((int64_t(matrix.m[row][0]) * vector.vx + int64_t(matrix.m[row][1]) * vector.vy +
                                        int64_t(matrix.m[row][2]) * vector.vz) >> 12);
    return result;
}

void multiply(const MATRIX &left, const MATRIX &right, MATRIX &output) {
    short result[3][3];
    for (int row = 0; row < 3; ++row)
        for (int column = 0; column < 3; ++column)
            result[row][column] = saturate16((int64_t(left.m[row][0]) * right.m[0][column] +
                                              int64_t(left.m[row][1]) * right.m[1][column] +
                                              int64_t(left.m[row][2]) * right.m[2][column]) >> 12);
    for (int row = 0; row < 3; ++row)
        for (int column = 0; column < 3; ++column)
            output.m[row][column] = result[row][column];
}

u8 depth_cue_channel(int64_t color_q16, s32 far_color, s16 depth_cue) {
    const int64_t far_q16 = int64_t(static_cast<s32>(static_cast<u32>(far_color) << 4)) * 4096;
    const auto difference = std::clamp<int64_t>((far_q16 - color_q16) >> 12, -32768, 32767);
    const int64_t result_q4 = (color_q16 + difference * depth_cue) >> 12;
    return static_cast<u8>(std::clamp<int64_t>(result_q4 >> 4, 0, 255));
}

CVECTOR light_normal(const SVECTOR &normal, const CVECTOR &base, s16 depth_cue) {
    std::array<s32, 3> illumination;
    const auto dots = rotate(gte.light, normal);
    for (int row = 0; row < 3; ++row)
        illumination[row] = std::clamp<s32>(dots[row], 0, 32767);
    const u8 tint[3] = {base.r, base.g, base.b};
    u8 color[3];
    for (int channel = 0; channel < 3; ++channel) {
        int64_t dot = int64_t(static_cast<s32>(static_cast<u32>(gte.back[channel]) << 4)) * 4096;
        for (int light = 0; light < 3; ++light)
            dot += int64_t(gte.color.m[channel][light]) * illumination[light];
        const auto intensity_q4 = std::clamp<int64_t>(dot >> 12, 0, 32767);
        color[channel] = depth_cue_channel(int64_t(tint[channel]) * intensity_q4 * 16, gte.far[channel], depth_cue);
    }
    return {color[0], color[1], color[2], base.cd};
}
}
}

using namespace kf;
using namespace kf::psx;

extern "C" {
void InitGeom(void) {
    gte = {};
    gte.offset_x = 160 << 16;
    gte.offset_y = 120 << 16;
    gte.screen = 1000;
}
void SetGeomOffset(long ofx, long ofy) {
    gte.offset_x = static_cast<s32>(static_cast<u32>(ofx) << 16);
    gte.offset_y = static_cast<s32>(static_cast<u32>(ofy) << 16);
}
void SetGeomScreen(long h) { gte.screen = static_cast<u16>(h); }
void SetRotMatrix(MATRIX *m) { gte.rotation = *m; }
void SetTransMatrix(MATRIX *m) {
    for (int i = 0; i < 3; ++i)
        gte.rotation.t[i] = m->t[i];
}
void SetLightMatrix(MATRIX *m) { gte.light = *m; }
void SetColorMatrix(MATRIX *m) { gte.color = *m; }
void SetBackColor(long r, long g, long b) {
    gte.back[0] = static_cast<s32>(r);
    gte.back[1] = static_cast<s32>(g);
    gte.back[2] = static_cast<s32>(b);
}
void SetFarColor(long r, long g, long b) {
    gte.far[0] = static_cast<s32>(r);
    gte.far[1] = static_cast<s32>(g);
    gte.far[2] = static_cast<s32>(b);
}
void SetFogNear(long a, long h) {
    // Wrapped numerator, signed division and a 16-bit coefficient, as retail.
    const s32 numerator = static_cast<s32>(static_cast<u32>(a) * (0u - 320u));
    gte.depth_a = static_cast<s16>(h ? numerator / static_cast<s32>(h) : 0);
    gte.depth_b = 0x01400000;
}

long RotTransPers(SVECTOR *v0, long *sxy, long *p, long *flag) {
    const auto rotated = rotate(gte.rotation, *v0);
    s32 camera[3];
    for (int i = 0; i < 3; ++i)
        camera[i] = static_cast<s32>(static_cast<u32>(rotated[i]) + static_cast<u32>(gte.rotation.t[i]));
    const u16 depth = static_cast<u16>(std::clamp<s32>(camera[2], 0, 65535));
    const u32 scale = projection_scale_q16(gte.screen, depth);
    const auto screen = [scale](s32 value, s32 offset) {
        const int64_t position = int64_t(saturate16(value)) * scale + offset;
        return static_cast<s16>(std::clamp<int64_t>(position >> 16, -1024, 1023));
    };
    const s16 x = screen(camera[0], gte.offset_x), y = screen(camera[1], gte.offset_y);
    *sxy = static_cast<long>((static_cast<u32>(static_cast<u16>(y)) << 16) | static_cast<u16>(x));
    const int64_t cue = (int64_t(scale) * gte.depth_a + gte.depth_b) >> 12;
    *p = static_cast<long>(std::clamp<int64_t>(cue, 0, 4096));
    *flag = 0;
    return depth >> 2;
}

void RotTrans(SVECTOR *v0, VECTOR *v1, long *flag) {
    const auto rotated = rotate(gte.rotation, *v0);
    v1->vx = static_cast<s32>(static_cast<u32>(rotated[0]) + static_cast<u32>(gte.rotation.t[0]));
    v1->vy = static_cast<s32>(static_cast<u32>(rotated[1]) + static_cast<u32>(gte.rotation.t[1]));
    v1->vz = static_cast<s32>(static_cast<u32>(rotated[2]) + static_cast<u32>(gte.rotation.t[2]));
    *flag = 0;
}

long NormalClip(long sxy0, long sxy1, long sxy2) {
    const auto x = [](long v) { return int64_t(static_cast<s16>(v & 0xffff)); };
    const auto y = [](long v) { return int64_t(static_cast<s16>((static_cast<u32>(v) >> 16) & 0xffff)); };
    return static_cast<long>(static_cast<s32>(x(sxy0) * y(sxy1) + x(sxy1) * y(sxy2) + x(sxy2) * y(sxy0) -
                                              x(sxy0) * y(sxy2) - x(sxy1) * y(sxy0) - x(sxy2) * y(sxy1)));
}

void DpqColor(CVECTOR *v0, long p, CVECTOR *v1) {
    const auto cue = static_cast<s16>(p);
    *v1 = {depth_cue_channel(int64_t(v0->r) << 16, gte.far[0], cue),
           depth_cue_channel(int64_t(v0->g) << 16, gte.far[1], cue),
           depth_cue_channel(int64_t(v0->b) << 16, gte.far[2], cue), v0->cd};
}

void NormalColorCol(SVECTOR *v0, CVECTOR *v1, CVECTOR *v2) { *v2 = light_normal(*v0, *v1, 0); }
void NormalColorCol3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3, CVECTOR *v4, CVECTOR *v5, CVECTOR *v6) {
    *v4 = light_normal(*v0, *v3, 0);
    *v5 = light_normal(*v1, *v3, 0);
    *v6 = light_normal(*v2, *v3, 0);
}
void NormalColorDpq(SVECTOR *v0, CVECTOR *v1, long p, CVECTOR *v2) {
    *v2 = light_normal(*v0, *v1, static_cast<s16>(p));
}
void NormalColorDpq3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3, long p, CVECTOR *v4, CVECTOR *v5,
                     CVECTOR *v6) {
    *v4 = light_normal(*v0, *v3, static_cast<s16>(p));
    *v5 = light_normal(*v1, *v3, static_cast<s16>(p));
    *v6 = light_normal(*v2, *v3, static_cast<s16>(p));
}

MATRIX *ScaleMatrix(MATRIX *m, VECTOR *v) {
    const s32 axes[3] = {static_cast<s32>(v->vx), static_cast<s32>(v->vy), static_cast<s32>(v->vz)};
    for (int row = 0; row < 3; ++row)
        for (int column = 0; column < 3; ++column) {
            const u32 product = static_cast<u32>(static_cast<s32>(m->m[row][column])) * static_cast<u32>(axes[column]);
            m->m[row][column] = static_cast<short>(static_cast<s32>(product) >> 12);
        }
    return m;
}
MATRIX *MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *m2) {
    multiply(*m0, *m1, *m2);
    return m2;
}
MATRIX *MulMatrix(MATRIX *m0, MATRIX *m1) {
    multiply(*m0, *m1, *m0);
    return m0;
}
MATRIX *MulMatrix2(MATRIX *m0, MATRIX *m1) {
    multiply(*m0, *m1, *m1);
    return m1;
}

MATRIX *RotMatrix(SVECTOR *r, MATRIX *m) {
    const s32 cx = cosine(r->vx), cy = cosine(r->vy), cz = cosine(r->vz);
    const s32 sx = sine(r->vx), sy = sine(r->vy), sz = sine(r->vz);
    const s32 negative_cz_sy = (-cz * sy) >> 12;
    const s32 negative_sz_sy = (-sz * sy) >> 12;
    m->m[0][0] = static_cast<short>((cz * cy) >> 12);
    m->m[0][1] = static_cast<short>((-sz * cy) >> 12);
    m->m[0][2] = static_cast<short>(sy);
    m->m[1][0] = static_cast<short>(((sz * cx) >> 12) - ((negative_cz_sy * sx) >> 12));
    m->m[1][1] = static_cast<short>(((cz * cx) >> 12) + ((negative_sz_sy * sx) >> 12));
    m->m[1][2] = static_cast<short>((-cy * sx) >> 12);
    m->m[2][0] = static_cast<short>(((sz * sx) >> 12) + ((negative_cz_sy * cx) >> 12));
    m->m[2][1] = static_cast<short>(((cz * sx) >> 12) - ((negative_sz_sy * cx) >> 12));
    m->m[2][2] = static_cast<short>((cy * cx) >> 12);
    return m;
}

VECTOR *ApplyMatrix(MATRIX *m, SVECTOR *v0, VECTOR *v1) {
    const auto rotated = rotate(*m, *v0);
    v1->vx = rotated[0];
    v1->vy = rotated[1];
    v1->vz = rotated[2];
    return v1;
}

void InitClip(EVECTOR *, long, long, long, long, long) {}

long Clip3FTP(SVECTOR *, SVECTOR *, SVECTOR *, short *, short *, short *, EVECTOR **) {
    warn_once("Clip3FTP (near-plane clipping)");
    return 0;
}

long Clip4FTP(SVECTOR *, SVECTOR *, SVECTOR *, SVECTOR *, short *, short *, short *, short *, EVECTOR **) {
    warn_once("Clip4FTP (near-plane clipping)");
    return 0;
}

int rsin(int a) { return sine(a); }
int rcos(int a) { return cosine(a); }

int catan(int a) {
    // Approximation pending the library's table-driven arctangent.
    return static_cast<int>(std::lround(std::atan(a / 4096.0) * 2048.0 / 3.14159265358979323846));
}

long SquareRoot0(long a) { return static_cast<long>(square_root_scaled(static_cast<u32>(a), 12)); }
long SquareRoot12(long a) { return static_cast<long>(square_root_scaled(static_cast<u32>(a), 6)); }
}
