#include <kf/lib/address.h>
#include <kf/lib/math.h>

ADDRESS(0x800154fc, 0x78)
void func_800154fc(s32 x, s32 y, s32 z, struct KfEulerAngles *angles)
{
    angles->y = vector_xz_to_angle(x, z);
    angles->x = -vector_xz_to_angle(y, fixed_vector2_length(x, z)) & KF_ANGLE_WRAP_MASK;
    angles->z = 0;
}

ADDRESS(0x80015574, 0x30)
KfBool func_80015574(s32 first, s32 first_width, s32 second, s32 second_width)
{
    if (second < first) {
        if (second < first - first_width) {
            return 0;
        }
    } else {
        if (first < second - second_width) {
            return 0;
        }
    }
    return 1;
}
