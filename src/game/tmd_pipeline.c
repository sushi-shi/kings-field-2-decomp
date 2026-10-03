#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

enum {
    KF_TMD_DEPTH_SHIFT = 2
};

RODATA(0x80011410, 0x74)

ADDRESS(0x8002d5dc, 0x2d4)
void tmd_prepare_primitive_indices(KfTmdHeader *tmd)
{
    KfTmdObject *object;
    u8 *packet;
    KfTmdPrimitive *primitive;
    u32 objects_left;
    u32 primitives_left;
    KfTmdPacketHeader header;

    objects_left = tmd->object_count;
    object = TMD_OBJECTS(tmd);
    while (--objects_left != (u32)-1) {
        primitives_left = object->primitive_count;
        packet = (u8 *)tmd + (object->primitive_offset + KF_TMD_HEADER_BYTES);
        while (--primitives_left != (u32)-1) {
            primitive = (KfTmdPrimitive *)TMD_PACKET_BODY(packet);
            header.word = *(u32 *)packet;
            packet = (u8 *)primitive + header.bytes.input_length * KF_TMD_WORD_BYTES;
            switch (tmd_packet_kind(header.word)) {
            case KF_TMD_MODE_F3: {
                primitive->f3.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f3.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f3.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f3.normal <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_G3: {
                primitive->g3.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.normal0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.normal1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.normal2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_FT3: {
                primitive->ft3.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft3.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft3.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft3.normal <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_GT3: {
                primitive->gt3.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.normal0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.normal1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.normal2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_F4: {
                primitive->f4.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f4.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f4.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f4.vertex3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f4.normal <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_G4: {
                primitive->g4.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.vertex3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.normal0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.normal1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.normal2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.normal3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_FT4: {
                primitive->ft4.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft4.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft4.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft4.vertex3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft4.normal <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_GT4: {
                primitive->gt4.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.vertex3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.normal0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.normal1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.normal2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.normal3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            }
        }
        object++;
    }
}

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
void tmd_project_vertices_with_fog(s32 count)
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
void tmd_project_vertices_mark_clipped(s32 count)
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
void render_enqueue_blended_tmd(u16 object_index, s32 depth_bias, s32 render_mode)
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
        KfTmdPacketHeader header;
        u8 mode;
        KfTmdPrimitive *face;
        KfScreenVertex *va;
        KfScreenVertex *vb;
        KfScreenVertex *vc;
        KfScreenVertex *vd;
        s32 depth;

        header.word = *(u32 *)packet;
        mode = header.word >> 24;
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
            if ((u32)depth >= KF_MAP_OT_DEPTH_LIMIT)
                break;
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
        packet += TMD_PACKET_BODY_BYTES(header.word);
    }
}

ADDRESS(0x8002e4dc, 0x704)
void render_enqueue_textured_tmd(u16 object_index, s32 depth_bias)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    KfTmdPacketHeader header;
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

        header.word = *(u32 *)packet;
        mode = header.word >> 24;
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
            if ((u32)depth >= KF_MAP_OT_DEPTH_LIMIT)
                break;
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
        packet += TMD_PACKET_BODY_BYTES(header.word);
    }
}

ADDRESS(0x8002ebe0, 0x5b4)
void render_enqueue_tmd_fixed_depth(u16 object_index, s32 blend_mode, s16 fixed_depth)
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
        KfTmdPacketHeader header;
        u8 mode;
        KfTmdPrimitive *face;
        KfScreenVertex *va;
        KfScreenVertex *vb;
        KfScreenVertex *vc;
        KfScreenVertex *vd;
        void *enqueue_prim;

        header.word = *(u32 *)packet;
        mode = header.word >> 24;
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
            enqueue_prim = &prim->sdk;
            goto enqueue;
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
            enqueue_prim = &prim->sdk;
            goto enqueue;
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
            enqueue_prim = prim;
            goto enqueue;
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
            enqueue_prim = prim;
            goto enqueue;
        }
        }
        goto next_packet;
enqueue:
        if (fixed_depth <= 0)
            goto next_packet;
        if ((u32)fixed_depth >= KF_MAP_OT_DEPTH_LIMIT)
            goto next_packet;
        AddPrim(&game_graphics_runtime.display_state.ordering_table[fixed_depth],
                enqueue_prim);
next_packet:
        packet += TMD_PACKET_BODY_BYTES(header.word);
    }
}

DATA(0x8006d6d0, 0x4)
CVECTOR map_textured_primitive_color = {128, 128, 128, 0};

/* The prepared indices in an FT packet are byte offsets into this array. */
#define MAP_VERTEX(base, offset) ((KfScreenVertex *)((u8 *)(base) + (offset)))
#define MAP_XY(vertex) (*(long *)(vertex))

ADDRESS(0x8002f194, 0x41c)
void render_enqueue_map(u16 object_index)
{
    KfTmdObject *object;
    KfTmdPacketHeader header;
    u8 *normals;
    u8 *packet;
    u32 remaining;
    CVECTOR shade;
    KfScreenVertex *va;
    KfScreenVertex *vb;
    KfScreenVertex *vc;
    KfScreenVertex *vd;

    object = tmd_get_object(object_index);
    normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
        (object->normal_offset + KF_TMD_HEADER_BYTES);
    tmd_project_vertices_with_fog(object->vertex_count);
    packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
        (object->primitive_offset + KF_TMD_HEADER_BYTES);
    remaining = object->primitive_count;
    while (remaining-- != 0) {
        u8 *vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;

        header.word = *(u32 *)packet;
        packet += KF_TMD_PACKET_HEADER_BYTES;
        switch (header.bytes.mode & KF_TMD_MODE_MASK) {
        case KF_TMD_MODE_FT4: {
            KfTmdFt4 *face = (KfTmdFt4 *)packet;
            KfGpuGT4 *prim;
            s32 depth;

            va = MAP_VERTEX(vertices, face->vertex0);
            vb = MAP_VERTEX(vertices, face->vertex1);
            vc = MAP_VERTEX(vertices, face->vertex2);
            if (NormalClip(MAP_XY(va), MAP_XY(vb), MAP_XY(vc)) <= 0) {
                break;
            }
            prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            vd = MAP_VERTEX(vertices, face->vertex3);
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end) {
                return;
            }
            prim->packed.clut = face->clut;
            prim->packed.tpage = face->tpage;
            prim->packed.xy0 = MAP_XY(va);
            prim->packed.xy1 = MAP_XY(vb);
            prim->packed.xy2 = MAP_XY(vc);
            prim->packed.xy3 = MAP_XY(vd);
            prim->packed.uv0 = face->uv0;
            prim->packed.uv1 = face->uv1;
            prim->packed.uv2 = face->uv2;
            prim->packed.uv3 = face->uv3;
            NormalColorCol((SVECTOR *)(normals + face->normal),
                           &map_textured_primitive_color, &shade);
            DpqColor(&shade, va->p2, &prim->packed.color0);
            DpqColor(&shade, vb->p2, &prim->packed.color1);
            DpqColor(&shade, vc->p2, &prim->packed.color2);
            DpqColor(&shade, vd->p2, &prim->packed.color3);
            ((u8 *)&prim->sdk.tag)[3] = 0x0c;
            prim->sdk.code = header.bytes.mode | 0x3c;
            depth = ((va->sz + vb->sz + vc->sz + vd->sz) >> 2) +
                KF_MAP_OT_DEPTH_BIAS;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT) {
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth],
                        &prim->sdk);
            }
            break;
        }
        case KF_TMD_MODE_FT3: {
            KfTmdFt3 *face = (KfTmdFt3 *)packet;
            KfGpuGT3 *prim;
            s32 depth;

            va = MAP_VERTEX(vertices, face->vertex0);
            vb = MAP_VERTEX(vertices, face->vertex1);
            vc = MAP_VERTEX(vertices, face->vertex2);
            if (NormalClip(MAP_XY(va), MAP_XY(vb), MAP_XY(vc)) <= 0) {
                break;
            }
            prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end) {
                return;
            }
            prim->packed.clut = face->clut;
            prim->packed.tpage = face->tpage;
            prim->packed.xy0 = MAP_XY(va);
            prim->packed.xy1 = MAP_XY(vb);
            prim->packed.xy2 = MAP_XY(vc);
            prim->packed.uv0 = face->uv0;
            prim->packed.uv1 = face->uv1;
            prim->packed.uv2 = face->uv2;
            NormalColorCol((SVECTOR *)(normals + face->normal),
                           &map_textured_primitive_color, &shade);
            DpqColor(&shade, va->p2, &prim->packed.color0);
            DpqColor(&shade, vb->p2, &prim->packed.color1);
            DpqColor(&shade, vc->p2, &prim->packed.color2);
            ((u8 *)&prim->sdk.tag)[3] = 0x09;
            prim->sdk.code = header.bytes.mode | 0x34;
            depth = (va->sz + vb->sz + vc->sz) / 3 + KF_MAP_OT_DEPTH_BIAS;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT) {
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth],
                        &prim->sdk);
            }
            break;
        }
        }
        packet += header.bytes.input_length * KF_TMD_WORD_BYTES;
    }
}

/* The clipping SDK returns EVECTOR pointers; NormalClip takes packed screen XY. */
#define CLIPPED_MAP_VERTEX(index) \
    (game_graphics_runtime.clip_result_vertices[index])
#define CLIPPED_MAP_XY(vertex) (*(long *)&(vertex)->sxy)

ADDRESS(0x8002f5b0, 0x258)
void render_enqueue_clipped_tmd_polygon(s32 vertex_count, SVECTOR *normal, u16 clut, u16 tpage,
                   u32 mode, s32 depth_bias)
{
    EVECTOR *first;
    EVECTOR *second;
    EVECTOR *third;
    CVECTOR shade;
    CVECTOR first_color;
    u32 packet_code;
    EVECTOR **next;
    s32 triangle_count;

    if (NormalClip(CLIPPED_MAP_XY(CLIPPED_MAP_VERTEX(0)),
                   CLIPPED_MAP_XY(CLIPPED_MAP_VERTEX(1)),
                   CLIPPED_MAP_XY(CLIPPED_MAP_VERTEX(2))) <= 0) {
        return;
    }
    NormalColorCol(normal, &map_textured_primitive_color, &shade);
    next = game_graphics_runtime.clip_result_vertices;
    first = *next++;
    packet_code = mode | 0x34;
    DpqColor(&shade, first->sxyz.pad >> 1, &first_color);
    second = *next++;
    DpqColor(&shade, second->sxyz.pad >> 1, &second->rgb);

    triangle_count = vertex_count - 2;
    goto loop_test;
loop_body: {
        KfGpuGT3 *packet;
        s32 depth;
        s32 summed_depth;

        third = *next++;
        DpqColor(&shade, third->sxyz.pad >> 1, &third->rgb);
        packet = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
        game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
        if (game_graphics_runtime.display_state.primitive_buffer->cursor >
            game_graphics_runtime.display_state.primitive_buffer->end) {
            return;
        }
        packet->packed.clut = clut;
        packet->packed.tpage = tpage;
        packet->packed.xy0 = CLIPPED_MAP_XY(first);
        packet->packed.xy1 = CLIPPED_MAP_XY(second);
        packet->packed.xy2 = CLIPPED_MAP_XY(third);
        packet->packed.uv0 = first->txuv;
        packet->packed.uv1 = second->txuv;
        packet->packed.uv2 = third->txuv;
        *(u32 *)&packet->packed.color0 = *(u32 *)&first_color;
        *(u32 *)&packet->packed.color1 = *(u32 *)&second->rgb;
        *(u32 *)&packet->packed.color2 = *(u32 *)&third->rgb;
        ((u8 *)&packet->sdk.tag)[3] = 9;
        packet->sdk.code = packet_code;
        summed_depth = first->sxyz.vz + second->sxyz.vz + third->sxyz.vz;
        depth = summed_depth / 12 + depth_bias;
        if (depth < 16) {
            depth = 16;
        }
        AddPrim(&game_graphics_runtime.display_state.ordering_table[depth & 0x1fff],
                &packet->sdk);

        second = third;
    }
loop_test:
    if (triangle_count-- > 0) {
        goto loop_body;
    }
}

#define MAP_OUTSIDE_Y(delta) ((u32)(delta) + 511u >= 1023u)
#define MAP_OUTSIDE_X(delta) ((u32)(delta) + 1023u >= 2047u)
#define MAP_ORIGINAL_VERTEX(base, offset) ((SVECTOR *)((u8 *)(base) + (offset)))

ADDRESS(0x8002f808, 0x754)
void render_enqueue_tmd_with_clipping(u16 object_index, s32 depth_bias,
                   KfTmdPreparedAsset *prepared_asset)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    u8 *vertices;
    SVECTOR *original_vertices;
    u32 remaining;
    KfTmdPacketHeader header;
    CVECTOR shade;

    if (prepared_asset != 0) {
        object = &prepared_asset->object;
        normals = (u8 *)prepared_asset +
            (object->normal_offset + KF_TMD_HEADER_BYTES);
        game_graphics_runtime.current_tmd_vertices =
            (SVECTOR *)((u8 *)prepared_asset +
                        (object->vertex_offset + KF_TMD_HEADER_BYTES));
    } else {
        object = tmd_get_object(object_index);
        normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
            (object->normal_offset + KF_TMD_HEADER_BYTES);
    }
    original_vertices = game_graphics_runtime.current_tmd_vertices;
    tmd_project_vertices_mark_clipped(object->vertex_count);
    if (prepared_asset != 0) {
        packet = (u8 *)prepared_asset +
            (object->primitive_offset + KF_TMD_HEADER_BYTES);
    } else {
        packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
            (object->primitive_offset + KF_TMD_HEADER_BYTES);
    }
    remaining = object->primitive_count;
    if (remaining-- != 0) {
        do {
            vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;
            header.word = *(u32 *)packet;
            packet += KF_TMD_PACKET_HEADER_BYTES;
            switch (header.bytes.mode & KF_TMD_MODE_MASK) {
            case KF_TMD_MODE_FT4: {
                KfTmdFt4 *face = (KfTmdFt4 *)packet;
                KfScreenVertex *va = MAP_VERTEX(vertices, face->vertex0);
                KfScreenVertex *vb = MAP_VERTEX(vertices, face->vertex1);
                KfScreenVertex *vc = MAP_VERTEX(vertices, face->vertex2);
                KfScreenVertex *vd = MAP_VERTEX(vertices, face->vertex3);
                s32 dy01;
                s32 dy13;
                s32 dy32;
                s32 dy20;
                s32 dy12;
                s32 dx01;
                s32 dx13;
                s32 dx32;
                s32 dx20;
                s32 dx12;
                s32 clipped_count;
                s32 depth;
                KfGpuGT4 *prim;

                dy01 = va->y - vb->y;
                dy13 = vb->y - vd->y;
                dy32 = vd->y - vc->y;
                dy20 = vc->y - va->y;
                dy12 = vb->y - vc->y;
                dx01 = va->x - vb->x;
                dx13 = vb->x - vd->x;
                dx32 = vd->x - vc->x;
                dx20 = vc->x - va->x;
                dx12 = vb->x - vc->x;
                if (!((s16)(va->sz | vb->sz | vc->sz | vd->sz) == -1 ||
                    MAP_OUTSIDE_Y(dy01) || MAP_OUTSIDE_Y(dy13) ||
                    MAP_OUTSIDE_Y(dy32) || MAP_OUTSIDE_Y(dy20) ||
                    MAP_OUTSIDE_Y(dy12) || MAP_OUTSIDE_X(dx01) ||
                    MAP_OUTSIDE_X(dx13) || MAP_OUTSIDE_X(dx32) ||
                    MAP_OUTSIDE_X(dx20) || MAP_OUTSIDE_X(dx12))) {
                    if (NormalClip(MAP_XY(va), MAP_XY(vb), MAP_XY(vc)) <= 0) {
                        break;
                    }
                    prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
                    game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
                    if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                        game_graphics_runtime.display_state.primitive_buffer->end) {
                        return;
                    }
                    prim->packed.clut = face->clut;
                    prim->packed.tpage = face->tpage;
                    prim->packed.xy0 = MAP_XY(va);
                    prim->packed.xy1 = MAP_XY(vb);
                    prim->packed.xy2 = MAP_XY(vc);
                    prim->packed.xy3 = MAP_XY(vd);
                    prim->packed.uv0 = face->uv0;
                    prim->packed.uv1 = face->uv1;
                    prim->packed.uv2 = face->uv2;
                    prim->packed.uv3 = face->uv3;
                    NormalColorCol((SVECTOR *)(normals + face->normal),
                                   &map_textured_primitive_color, &shade);
                    DpqColor(&shade, va->p2, &prim->packed.color0);
                    DpqColor(&shade, vb->p2, &prim->packed.color1);
                    DpqColor(&shade, vc->p2, &prim->packed.color2);
                    DpqColor(&shade, vd->p2, &prim->packed.color3);
                    ((u8 *)&prim->sdk.tag)[3] = 12;
                    prim->sdk.code = (header.bytes.mode & 2) | 0x3c;
                    depth = ((va->sz + vb->sz + vc->sz + vd->sz) >> 2) + depth_bias;
                    if (depth < 16) {
                        depth = 16;
                    }
                    AddPrim(&game_graphics_runtime.display_state.ordering_table[depth & 0x1fff],
                            &prim->sdk);
                } else {
                    clipped_count = Clip4FTP(MAP_ORIGINAL_VERTEX(original_vertices, face->vertex0),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex1),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex2),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex3),
                                             (short *)&face->uv0,
                                             (short *)&face->uv1,
                                             (short *)&face->uv2,
                                             (short *)&face->uv3,
                                             game_graphics_runtime.clip_result_vertices);
                    if (clipped_count >= 3) {
                        render_enqueue_clipped_tmd_polygon(clipped_count,
                                       (SVECTOR *)(normals + face->normal),
                                       face->clut, face->tpage,
                                       header.bytes.mode & 2, depth_bias);
                    }
                }
                break;
            }
            case KF_TMD_MODE_FT3: {
                KfTmdFt3 *face = (KfTmdFt3 *)packet;
                KfScreenVertex *va = MAP_VERTEX(vertices, face->vertex0);
                KfScreenVertex *vb = MAP_VERTEX(vertices, face->vertex1);
                KfScreenVertex *vc = MAP_VERTEX(vertices, face->vertex2);
                s32 dy01;
                s32 dy12;
                s32 dy20;
                s32 dx01;
                s32 dx12;
                s32 dx20;
                s32 clipped_count;
                s32 depth;
                KfGpuGT3 *prim;

                dy01 = va->y - vb->y;
                dy12 = vb->y - vc->y;
                dy20 = vc->y - va->y;
                dx01 = va->x - vb->x;
                dx12 = vb->x - vc->x;
                dx20 = vc->x - va->x;
                if (!((s16)(va->sz | vb->sz | vc->sz) == -1 ||
                    MAP_OUTSIDE_Y(dy01) || MAP_OUTSIDE_Y(dy12) ||
                    MAP_OUTSIDE_Y(dy20) || MAP_OUTSIDE_X(dx01) ||
                    MAP_OUTSIDE_X(dx12) || MAP_OUTSIDE_X(dx20))) {
                    if (NormalClip(MAP_XY(va), MAP_XY(vb), MAP_XY(vc)) <= 0) {
                        break;
                    }
                    prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
                    game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
                    if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                        game_graphics_runtime.display_state.primitive_buffer->end) {
                        return;
                    }
                    prim->packed.clut = face->clut;
                    prim->packed.tpage = face->tpage;
                    prim->packed.xy0 = MAP_XY(va);
                    prim->packed.xy1 = MAP_XY(vb);
                    prim->packed.xy2 = MAP_XY(vc);
                    prim->packed.uv0 = face->uv0;
                    prim->packed.uv1 = face->uv1;
                    prim->packed.uv2 = face->uv2;
                    NormalColorCol((SVECTOR *)(normals + face->normal),
                                   &map_textured_primitive_color, &shade);
                    DpqColor(&shade, va->p2, &prim->packed.color0);
                    DpqColor(&shade, vb->p2, &prim->packed.color1);
                    DpqColor(&shade, vc->p2, &prim->packed.color2);
                    ((u8 *)&prim->sdk.tag)[3] = 9;
                    prim->sdk.code = (header.bytes.mode & 2) | 0x34;
                    depth = (va->sz + vb->sz + vc->sz) / 3 + depth_bias;
                    if (depth < 16) {
                        depth = 16;
                    }
                    AddPrim(&game_graphics_runtime.display_state.ordering_table[depth & 0x1fff],
                            &prim->sdk);
                } else {
                    clipped_count = Clip3FTP(MAP_ORIGINAL_VERTEX(original_vertices, face->vertex0),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex1),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex2),
                                             (short *)&face->uv0,
                                             (short *)&face->uv1,
                                             (short *)&face->uv2,
                                             game_graphics_runtime.clip_result_vertices);
                    if (clipped_count >= 3) {
                        render_enqueue_clipped_tmd_polygon(clipped_count,
                                       (SVECTOR *)(normals + face->normal),
                                       face->clut, face->tpage,
                                       header.bytes.mode & 2, depth_bias);
                    }
                }
                break;
            }
            }
            packet += header.bytes.input_length * KF_TMD_WORD_BYTES;
        } while (remaining-- != 0);
    }
}
