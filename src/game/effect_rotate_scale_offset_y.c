#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/effect.h>

ADDRESS(0x800416ec, 0x90)
void effect_rotate_scale_offset_y(const SVECTOR *offset, VECTOR *output, s16 angle, s32 scale)
{
    SVECTOR scaled;
    SVECTOR rotation;
    MATRIX matrix;

    setVector(&scaled,
              (offset->vx * scale) >> KF_FIXED12_BITS,
              0,
              (offset->vz * scale) >> KF_FIXED12_BITS);
    setVector(&rotation, 0, angle, 0);
    RotMatrix(&rotation, &matrix);
    ApplyMatrix(&matrix, &scaled, output);
}
