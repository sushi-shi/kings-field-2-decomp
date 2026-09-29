#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>

enum {
    KF_TMD_DEPTH_SHIFT = 2
};

ADDRESS(0x8002dbd8, 0xa8)
void tmd_transform_vertices(s32 count)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    VECTOR transformed;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    for (remaining = count - 1; remaining != -1; remaining--) {
        RotTrans(vertex, &transformed, &gte_flags);
        projected->x = transformed.vx;
        projected->y = transformed.vy;
        projected->p2 = transformed.vz;
        projected->sz = transformed.vz >> KF_TMD_DEPTH_SHIFT;
        projected++;
        vertex++;
    }
}

ADDRESS(0x8002dc80, 0xa8)
void tmd_transform_vertices_depth(s32 count, s16 depth)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    VECTOR transformed;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    for (remaining = count - 1; remaining != -1; remaining--) {
        RotTrans(vertex, &transformed, &gte_flags);
        projected->x = transformed.vx;
        projected->y = transformed.vy;
        projected->p2 = transformed.vz;
        projected->sz = depth;
        projected++;
        vertex++;
    }
}

ADDRESS(0x8002dd28, 0x8c)
void tmd_project_vertices(s32 count)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    long perspective;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    for (remaining = count - 1; remaining != -1; remaining--) {
        projected->sz = RotTransPers(vertex, (long *)projected, &perspective, &gte_flags);
        projected->p2 = 0;
        projected++;
        vertex++;
    }
}
