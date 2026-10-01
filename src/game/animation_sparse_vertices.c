#include <kf/lib/address.h>
#include <kf/game/animation.h>

enum { KF_ANIMATION_SPARSE_SKIP = -32768 };

ADDRESS(0x80033bfc, 0xc4)
void animation_expand_sparse_vertices(SVECTOR *vertices, const SVECTOR *base,
                                      const s16 *encoded)
{
    s32 remaining = *encoded++;

    for (--remaining; remaining != -1; --remaining) {
        u16 value = *encoded++;

        if ((s16)value == KF_ANIMATION_SPARSE_SKIP) {
            s32 copy_count = *encoded++;

            for (--copy_count; copy_count != -1; --copy_count) {
                copyVector(vertices, base);
                vertices++;
                base++;
            }
        } else {
            vertices->vx = value;
            vertices->vy = *encoded++;
            vertices->vz = *encoded++;
            vertices++;
            base++;
        }
    }
}

ADDRESS(0x80033cc0, 0x7c)
void animation_decode_sparse_vertices(SVECTOR *vertices, const s16 *encoded)
{
    s32 remaining = *encoded++;

    for (--remaining; remaining != -1; --remaining) {
        u16 value = *encoded++;

        if ((s16)value == KF_ANIMATION_SPARSE_SKIP) {
            vertices += *encoded++;
        } else {
            vertices->vx = value;
            vertices->vy = *encoded++;
            vertices->vz = *encoded++;
            vertices++;
        }
    }
}

ADDRESS(0x80033d3c, 0x2b8)
void func_80033d3c(SVECTOR *vertices, const s16 *encoded, s32 blend_fraction)
{
    MATRIX deltas;
    VECTOR scale;
    SVECTOR *group_start;
    SVECTOR *cursor;
    s32 pending;
    s32 remaining;
    s16 *delta_write;

    scale.vz = blend_fraction;
    scale.vy = blend_fraction;
    scale.vx = blend_fraction;
    remaining = *encoded;
    cursor = vertices;
    encoded++;
    pending = 0;
    group_start = cursor;
    delta_write = &deltas.m[0][0];

    for (--remaining; remaining != -1; --remaining) {
        s16 value = *encoded++;

        if (value == KF_ANIMATION_SPARSE_SKIP) {
            cursor += *encoded++;
            if (pending != 0) {
                s16 *scaled;

                ScaleMatrix(&deltas, &scale);
                scaled = &deltas.m[0][0];
                for (--pending; pending != -1; --pending) {
                    group_start->vx += *scaled++;
                    group_start->vy += *scaled++;
                    group_start->vz += *scaled++;
                    group_start++;
                }
                pending = 0;
                delta_write = &deltas.m[0][0];
            }
            group_start = cursor;
        } else {
            *delta_write++ = value - cursor->vx;
            *delta_write++ = *encoded++ - cursor->vy;
            *delta_write++ = *encoded++ - cursor->vz;
            cursor++;

            if (pending != 2) {
                goto increment_pending;
            }
            ScaleMatrix(&deltas, &scale);
            group_start[0].vx += deltas.m[0][0];
            group_start[0].vy += deltas.m[0][1];
            group_start[0].vz += deltas.m[0][2];
            group_start[1].vx += deltas.m[1][0];
            group_start[1].vy += deltas.m[1][1];
            group_start[1].vz += deltas.m[1][2];
            group_start[2].vx += deltas.m[2][0];
            pending = 0;
            group_start[2].vy += deltas.m[2][1];
            group_start[2].vz += deltas.m[2][2];
            delta_write = &deltas.m[0][0];
            group_start = cursor;
            goto next_vertex;

        increment_pending:
            ++pending;
        }
    next_vertex:
        ;
    }

    if (pending != 0) {
        s16 *scaled;

        ScaleMatrix(&deltas, &scale);
        scaled = &deltas.m[0][0];
        for (--pending; pending != -1; --pending) {
            group_start->vx += *scaled++;
            group_start->vy += *scaled++;
            group_start->vz += *scaled++;
            group_start++;
        }
    }
}
