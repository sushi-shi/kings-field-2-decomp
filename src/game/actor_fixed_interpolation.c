#include <kf/lib/address.h>
#include <kf/lib/math.h>

ADDRESS(0x8001584c, 0x20)
s32 fixed_lerp_q12(s32 start, s32 end, s32 fraction)
{
    return (((end - start) * fraction) >> KF_FIXED12_BITS) + start;
}

ADDRESS(0x8001586c, 0x48)
s32 angle_lerp_shortest_q12(s32 start, s32 end, s32 fraction)
{
    s32 delta = ((s16)end - (s16)start) & KF_ANGLE_WRAP_MASK;

    if (delta > KF_ANGLE_HALF_TURN) {
        delta -= KF_ANGLE_FULL_TURN;
    }
    return (start + ((delta * fraction) >> KF_FIXED12_BITS)) & KF_ANGLE_WRAP_MASK;
}

ADDRESS(0x800158b4, 0x64)
void fixed_lerp_nine_halfwords_q12(const u16 *start, const u16 *end, u16 *output, s16 fraction)
{
    const u16 *source = start;
    const u16 *target = end;
    u16 *destination = output;
    s32 index;

    for (index = 8; index != -1; index--) {
        u16 value = *source++;
        u16 next = *target++;
        s32 delta = (s16)next - (s16)value;

        *destination++ = value + ((delta * fraction) >> KF_FIXED12_BITS);
    }
}
