#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

extern void func_8002d918(s32 vertex_count);

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
    func_8002d918(object->vertex_count);
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
void func_8002f5b0(s32 vertex_count, SVECTOR *normal, u16 clut, u16 tpage,
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
        depth = (first->sxyz.vz + second->sxyz.vz + third->sxyz.vz) / 12
              + depth_bias;
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

#define MAP_OUTSIDE_Y(delta) ((u32)((delta) + 511) >= 1023)
#define MAP_OUTSIDE_X(delta) ((u32)((delta) + 1023) >= 2047)
#define MAP_ORIGINAL_VERTEX(base, offset) ((SVECTOR *)((u8 *)(base) + (offset)))

ADDRESS(0x8002f808, 0x754)
void func_8002f808(u16 object_index, s32 depth_bias,
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
    func_8002da94(object->vertex_count);
    if (prepared_asset != 0) {
        packet = (u8 *)prepared_asset +
            (object->primitive_offset + KF_TMD_HEADER_BYTES);
    } else {
        packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
            (object->primitive_offset + KF_TMD_HEADER_BYTES);
    }
    remaining = object->primitive_count;
    if (remaining != 0) {
        remaining--;
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
                        func_8002f5b0(clipped_count,
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
                        func_8002f5b0(clipped_count,
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
