#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

enum {
    KF_TMD_DEPTH_SHIFT = 2
};

ADDRESS(0x8002d8b0, 0x40)
void tmd_register(u16 slot, KfTmdHeader *tmd)
{
    game_graphics_runtime.tmd_state.current_asset = game_graphics_runtime.tmd_state.slots[slot] = tmd;
    tmd_prepare_primitive_indices(tmd);
}

ADDRESS(0x8002d8f0, 0x20)
void tmd_set_slot(u16 slot, KfTmdHeader *tmd)
{
    game_graphics_runtime.tmd_state.slots[slot] = tmd;
}

/* Left empty in retail. */
ADDRESS(0x8002d910, 0x8)
void tmd_release_slot(void)
{
}

ADDRESS(0x8002d918, 0x17c)
void func_8002d918(s32 count)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    long perspective;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    remaining = count;
    if (game_graphics_runtime.render_grid.fog_near_distance >= 32000) {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            projected->p2 = 0;
            projected++;
            vertex++;
        }
    } else if (game_graphics_runtime.render_grid.fog_near_distance & 0x8000) {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            perspective -= 800;
            if (perspective < 0) {
                perspective = 0;
            }
            projected->p2 = (u16)perspective << 1;
            projected++;
            vertex++;
        }
    } else {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            if (perspective >= 2800) {
                perspective += (perspective - 2800) * 2;
            }
            projected->p2 = perspective;
            projected++;
            vertex++;
        }
    }
}

ADDRESS(0x8002da94, 0x144)
void func_8002da94(s32 count)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    long perspective;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    remaining = count;
    if (game_graphics_runtime.render_grid.fog_near_distance >= 32000) {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            if (gte_flags != 0x1000) {
                projected->sz = -1;
            }
            projected->p2 = 0;
            projected++;
            vertex++;
        }
    } else {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            if (gte_flags != 0x1000) {
                projected->sz = -1;
            }
            if (perspective >= 2800) {
                perspective += (perspective - 2800) * 2;
            }
            projected->p2 = perspective;
            projected++;
            vertex++;
        }
    }
}

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

#define TMD_VERTEX(base, offset) ((KfScreenVertex *)((base) + (offset)))
#define TMD_XY(vertex) (*(long *)(vertex))

ADDRESS(0x8002ddb4, 0x728)
void func_8002ddb4(u16 object_index, s32 depth_bias, s32 render_mode)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    u32 remaining;
    u16 blend_bits = (u16)render_mode << 5;

    object = tmd_get_object(object_index);
    packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
             (object->primitive_offset + KF_TMD_HEADER_BYTES);
    normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
              (object->normal_offset + KF_TMD_HEADER_BYTES);
    remaining = object->primitive_count;
    while (remaining-- != 0) {
        u8 *vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;
        u32 header = *(u32 *)packet;
        u8 mode = header >> 24;
        KfTmdPrimitive *face;
        KfScreenVertex *va;
        KfScreenVertex *vb;
        KfScreenVertex *vc;
        KfScreenVertex *vd;
        s32 depth;

        packet += KF_TMD_PACKET_HEADER_BYTES;
        face = (KfTmdPrimitive *)packet;
        switch (mode & KF_TMD_MODE_MASK) {
        case KF_TMD_MODE_FT3: {
            POLY_FT3 *prim;

            va = TMD_VERTEX(vertices, face->ft3.vertex0);
            vb = TMD_VERTEX(vertices, face->ft3.vertex1);
            vc = TMD_VERTEX(vertices, face->ft3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (POLY_FT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->clut = face->ft3.clut;
            prim->tpage = (face->ft3.tpage & 0xff9f) | blend_bits;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(u16 *)&prim->u0 = face->ft3.uv0;
            *(u16 *)&prim->u1 = face->ft3.uv1;
            *(u16 *)&prim->u2 = face->ft3.uv2;
            NormalColorDpq((SVECTOR *)(normals + face->ft3.normal),
                           &map_textured_primitive_color,
                           (va->p2 + vb->p2 + vc->p2) / 3,
                           (CVECTOR *)&prim->r0);
            ((u8 *)&prim->tag)[3] = 7;
            prim->code = 0x26;
            depth = (va->sz + vb->sz + vc->sz) / 3;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], prim);
            break;
        }
        case KF_TMD_MODE_GT3: {
            KfGpuGT3 *prim;

            va = TMD_VERTEX(vertices, face->gt3.vertex0);
            vb = TMD_VERTEX(vertices, face->gt3.vertex1);
            vc = TMD_VERTEX(vertices, face->gt3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt3.clut;
            prim->packed.tpage = (face->gt3.tpage & 0xff9f) | blend_bits;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.uv0 = face->gt3.uv0;
            prim->packed.uv1 = face->gt3.uv1;
            prim->packed.uv2 = face->gt3.uv2;
            NormalColorDpq3((SVECTOR *)(normals + face->gt3.normal0),
                            (SVECTOR *)(normals + face->gt3.normal1),
                            (SVECTOR *)(normals + face->gt3.normal2),
                            &map_textured_primitive_color, va->p2,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            ((u8 *)&prim->sdk.tag)[3] = 9;
            prim->sdk.code = 0x36;
            depth = (va->sz + vb->sz + vc->sz) / 3;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], &prim->sdk);
            break;
        }
        case KF_TMD_MODE_GT4: {
            KfGpuGT4 *prim;

            va = TMD_VERTEX(vertices, face->gt4.vertex0);
            vb = TMD_VERTEX(vertices, face->gt4.vertex1);
            vc = TMD_VERTEX(vertices, face->gt4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->gt4.vertex3);
            prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt4.clut;
            prim->packed.tpage = (face->gt4.tpage & 0xff9f) | blend_bits;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.xy3 = TMD_XY(vd);
            prim->packed.uv0 = face->gt4.uv0;
            prim->packed.uv1 = face->gt4.uv1;
            prim->packed.uv2 = face->gt4.uv2;
            prim->packed.uv3 = face->gt4.uv3;
            NormalColorDpq3((SVECTOR *)(normals + face->gt4.normal0),
                            (SVECTOR *)(normals + face->gt4.normal1),
                            (SVECTOR *)(normals + face->gt4.normal2),
                            &map_textured_primitive_color, va->p2,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            NormalColorDpq((SVECTOR *)(normals + face->gt4.normal3),
                           &map_textured_primitive_color, va->p2,
                           &prim->packed.color3);
            ((u8 *)&prim->sdk.tag)[3] = 12;
            prim->sdk.code = 0x3e;
            depth = (va->sz + vb->sz + vc->sz + vd->sz) >> 2;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], &prim->sdk);
            break;
        }
        case KF_TMD_MODE_FT4: {
            POLY_FT4 *prim;

            va = TMD_VERTEX(vertices, face->ft4.vertex0);
            vb = TMD_VERTEX(vertices, face->ft4.vertex1);
            vc = TMD_VERTEX(vertices, face->ft4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->ft4.vertex3);
            prim = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->clut = face->ft4.clut;
            prim->tpage = (face->ft4.tpage & 0xff9f) | blend_bits;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(long *)&prim->x3 = TMD_XY(vd);
            *(u16 *)&prim->u0 = face->ft4.uv0;
            *(u16 *)&prim->u1 = face->ft4.uv1;
            *(u16 *)&prim->u2 = face->ft4.uv2;
            *(u16 *)&prim->u3 = face->ft4.uv3;
            NormalColorDpq((SVECTOR *)(normals + face->ft4.normal),
                           &map_textured_primitive_color,
                           (va->p2 + vb->p2 + vc->p2 + vd->p2) >> 2,
                           (CVECTOR *)&prim->r0);
            ((u8 *)&prim->tag)[3] = 9;
            prim->code = 0x2e;
            depth = (va->sz + vb->sz + vc->sz + vd->sz) >> 2;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], prim);
            break;
        }
        }
        packet += (header >> 6) & 0x3fc;
    }
}

ADDRESS(0x8002e4dc, 0x704)
void func_8002e4dc(u16 object_index, s32 depth_bias)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    u32 header;
    u32 remaining;

    object = tmd_get_object(object_index);
    packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
             (object->primitive_offset + KF_TMD_HEADER_BYTES);
    normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
              (object->normal_offset + KF_TMD_HEADER_BYTES);
    remaining = object->primitive_count;
    while (remaining-- != 0) {
        u8 *vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;
        s32 mode;
        KfTmdPrimitive *face;
        KfScreenVertex *va;
        KfScreenVertex *vb;
        KfScreenVertex *vc;
        KfScreenVertex *vd;
        s32 depth;

        header = *(u32 *)packet;
        mode = header >> 24;
        packet += KF_TMD_PACKET_HEADER_BYTES;
        face = (KfTmdPrimitive *)packet;
        switch (mode & KF_TMD_MODE_MASK) {
        case KF_TMD_MODE_FT3: {
            POLY_FT3 *prim;

            va = TMD_VERTEX(vertices, face->ft3.vertex0);
            vb = TMD_VERTEX(vertices, face->ft3.vertex1);
            vc = TMD_VERTEX(vertices, face->ft3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (POLY_FT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->clut = face->ft3.clut;
            prim->tpage = face->ft3.tpage;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(u16 *)&prim->u0 = face->ft3.uv0;
            *(u16 *)&prim->u1 = face->ft3.uv1;
            *(u16 *)&prim->u2 = face->ft3.uv2;
            NormalColorDpq((SVECTOR *)(normals + face->ft3.normal),
                           &map_textured_primitive_color,
                           (va->p2 + vb->p2 + vc->p2) / 3,
                           (CVECTOR *)&prim->r0);
            ((u8 *)&prim->tag)[3] = 7;
            prim->code = mode;
            depth = (va->sz + vb->sz + vc->sz) / 3;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], prim);
            break;
        }
        case KF_TMD_MODE_GT3: {
            KfGpuGT3 *prim;

            va = TMD_VERTEX(vertices, face->gt3.vertex0);
            vb = TMD_VERTEX(vertices, face->gt3.vertex1);
            vc = TMD_VERTEX(vertices, face->gt3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt3.clut;
            prim->packed.tpage = face->gt3.tpage;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.uv0 = face->gt3.uv0;
            prim->packed.uv1 = face->gt3.uv1;
            prim->packed.uv2 = face->gt3.uv2;
            NormalColorDpq3((SVECTOR *)(normals + face->gt3.normal0),
                            (SVECTOR *)(normals + face->gt3.normal1),
                            (SVECTOR *)(normals + face->gt3.normal2),
                            &map_textured_primitive_color, va->p2,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            ((u8 *)&prim->sdk.tag)[3] = 9;
            prim->sdk.code = mode;
            depth = (va->sz + vb->sz + vc->sz) / 3;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], &prim->sdk);
            break;
        }
        case KF_TMD_MODE_GT4: {
            KfGpuGT4 *prim;

            va = TMD_VERTEX(vertices, face->gt4.vertex0);
            vb = TMD_VERTEX(vertices, face->gt4.vertex1);
            vc = TMD_VERTEX(vertices, face->gt4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->gt4.vertex3);
            prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt4.clut;
            prim->packed.tpage = face->gt4.tpage;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.xy3 = TMD_XY(vd);
            prim->packed.uv0 = face->gt4.uv0;
            prim->packed.uv1 = face->gt4.uv1;
            prim->packed.uv2 = face->gt4.uv2;
            prim->packed.uv3 = face->gt4.uv3;
            NormalColorDpq3((SVECTOR *)(normals + face->gt4.normal0),
                            (SVECTOR *)(normals + face->gt4.normal1),
                            (SVECTOR *)(normals + face->gt4.normal2),
                            &map_textured_primitive_color, va->p2,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            NormalColorDpq((SVECTOR *)(normals + face->gt4.normal3),
                           &map_textured_primitive_color, va->p2,
                           &prim->packed.color3);
            ((u8 *)&prim->sdk.tag)[3] = 12;
            prim->sdk.code = mode;
            depth = (va->sz + vb->sz + vc->sz + vd->sz) >> 2;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], &prim->sdk);
            break;
        }
        case KF_TMD_MODE_FT4: {
            POLY_FT4 *prim;

            va = TMD_VERTEX(vertices, face->ft4.vertex0);
            vb = TMD_VERTEX(vertices, face->ft4.vertex1);
            vc = TMD_VERTEX(vertices, face->ft4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->ft4.vertex3);
            prim = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->clut = face->ft4.clut;
            prim->tpage = face->ft4.tpage;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(long *)&prim->x3 = TMD_XY(vd);
            *(u16 *)&prim->u0 = face->ft4.uv0;
            *(u16 *)&prim->u1 = face->ft4.uv1;
            *(u16 *)&prim->u2 = face->ft4.uv2;
            *(u16 *)&prim->u3 = face->ft4.uv3;
            NormalColorDpq((SVECTOR *)(normals + face->ft4.normal),
                           &map_textured_primitive_color,
                           (va->p2 + vb->p2 + vc->p2 + vd->p2) >> 2,
                           (CVECTOR *)&prim->r0);
            ((u8 *)&prim->tag)[3] = 9;
            prim->code = mode;
            depth = (va->sz + vb->sz + vc->sz + vd->sz) >> 2;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], prim);
            break;
        }
        }
        packet += (header >> 6) & 0x3fc;
    }
}

ADDRESS(0x8002ebe0, 0x5b4)
void func_8002ebe0(u16 object_index, s32 blend_mode, s16 fixed_depth)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    u32 remaining;
    u32 blend_bits;

    object = tmd_get_object(object_index);
    blend_bits = (u32)blend_mode << 5;
    packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
             (object->primitive_offset + KF_TMD_HEADER_BYTES);
    normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
              (object->normal_offset + KF_TMD_HEADER_BYTES);
    remaining = object->primitive_count;
    while (remaining-- != 0) {
        u8 *vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;
        u32 header = *(u32 *)packet;
        u8 mode = header >> 24;
        KfTmdPrimitive *face;
        KfScreenVertex *va;
        KfScreenVertex *vb;
        KfScreenVertex *vc;
        KfScreenVertex *vd;

        packet += KF_TMD_PACKET_HEADER_BYTES;
        face = (KfTmdPrimitive *)packet;
        switch (mode & KF_TMD_MODE_MASK) {
        case KF_TMD_MODE_GT3: {
            KfGpuGT3 *prim;

            va = TMD_VERTEX(vertices, face->gt3.vertex0);
            vb = TMD_VERTEX(vertices, face->gt3.vertex1);
            vc = TMD_VERTEX(vertices, face->gt3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt3.clut;
            prim->packed.tpage = (face->gt3.tpage & 0xff9f) | blend_bits;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.uv0 = face->gt3.uv0;
            prim->packed.uv1 = face->gt3.uv1;
            prim->packed.uv2 = face->gt3.uv2;
            NormalColorCol3((SVECTOR *)(normals + face->gt3.normal0),
                            (SVECTOR *)(normals + face->gt3.normal1),
                            (SVECTOR *)(normals + face->gt3.normal2),
                            &map_textured_primitive_color,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            ((u8 *)&prim->sdk.tag)[3] = 9;
            prim->sdk.code = (mode & 2) | 0x34;
            if (fixed_depth > 0 && fixed_depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[fixed_depth],
                        &prim->sdk);
            break;
        }
        case KF_TMD_MODE_GT4: {
            KfGpuGT4 *prim;

            va = TMD_VERTEX(vertices, face->gt4.vertex0);
            vb = TMD_VERTEX(vertices, face->gt4.vertex1);
            vc = TMD_VERTEX(vertices, face->gt4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->gt4.vertex3);
            prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt4.clut;
            prim->packed.tpage = (face->gt4.tpage & 0xff9f) | blend_bits;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.xy3 = TMD_XY(vd);
            prim->packed.uv0 = face->gt4.uv0;
            prim->packed.uv1 = face->gt4.uv1;
            prim->packed.uv2 = face->gt4.uv2;
            prim->packed.uv3 = face->gt4.uv3;
            NormalColorCol3((SVECTOR *)(normals + face->gt4.normal0),
                            (SVECTOR *)(normals + face->gt4.normal1),
                            (SVECTOR *)(normals + face->gt4.normal2),
                            &map_textured_primitive_color,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            NormalColorCol((SVECTOR *)(normals + face->gt4.normal3),
                           &map_textured_primitive_color,
                           &prim->packed.color3);
            ((u8 *)&prim->sdk.tag)[3] = 12;
            prim->sdk.code = (mode & 2) | 0x3c;
            if (fixed_depth > 0 && fixed_depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[fixed_depth],
                        &prim->sdk);
            break;
        }
        case KF_TMD_MODE_G3: {
            POLY_G3 *prim;

            va = TMD_VERTEX(vertices, face->g3.vertex0);
            vb = TMD_VERTEX(vertices, face->g3.vertex1);
            vc = TMD_VERTEX(vertices, face->g3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (POLY_G3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_G3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            NormalColorCol3((SVECTOR *)(normals + face->g3.normal0),
                            (SVECTOR *)(normals + face->g3.normal1),
                            (SVECTOR *)(normals + face->g3.normal2),
                            &face->g3.color,
                            (CVECTOR *)&prim->r0, (CVECTOR *)&prim->r1,
                            (CVECTOR *)&prim->r2);
            ((u8 *)&prim->tag)[3] = 6;
            prim->code = 0x30;
            if (fixed_depth > 0 && fixed_depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[fixed_depth],
                        prim);
            break;
        }
        case KF_TMD_MODE_G4: {
            POLY_G4 *prim;

            va = TMD_VERTEX(vertices, face->g4.vertex0);
            vb = TMD_VERTEX(vertices, face->g4.vertex1);
            vc = TMD_VERTEX(vertices, face->g4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->g4.vertex3);
            prim = (POLY_G4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_G4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(long *)&prim->x3 = TMD_XY(vd);
            NormalColorCol3((SVECTOR *)(normals + face->g4.normal0),
                            (SVECTOR *)(normals + face->g4.normal1),
                            (SVECTOR *)(normals + face->g4.normal2),
                            &face->g4.color,
                            (CVECTOR *)&prim->r0, (CVECTOR *)&prim->r1,
                            (CVECTOR *)&prim->r2);
            NormalColorCol((SVECTOR *)(normals + face->g4.normal3),
                           &face->g4.color,
                           (CVECTOR *)&prim->r3);
            ((u8 *)&prim->tag)[3] = 8;
            prim->code = 0x38;
            if (fixed_depth > 0 && fixed_depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[fixed_depth],
                        prim);
            break;
        }
        }
        packet += (header >> 6) & 0x3fc;
    }
}
