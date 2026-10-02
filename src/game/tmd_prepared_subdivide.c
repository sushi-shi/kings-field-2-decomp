#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/memory.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

typedef struct KfTmdUvBytes {
    u8 u, v;
} KfTmdUvBytes;
typedef char kf_tmd_uv_bytes_size[sizeof(KfTmdUvBytes) == 2 ? 1 : -1];

typedef struct KfTmdFt4TextureWords {
    KfTmdUvBytes uv0; u16 clut; KfTmdUvBytes uv1; u16 tpage;
    KfTmdUvBytes uv2; u16 pad0; KfTmdUvBytes uv3; u16 pad1;
} KfTmdFt4TextureWords;
typedef char kf_tmd_ft4_texture_words_size[
    sizeof(KfTmdFt4TextureWords) == 16 ? 1 : -1];
typedef char kf_tmd_ft4_texture_uv1_offset[
    (u32)&((KfTmdFt4TextureWords *)0)->uv1 == 4 ? 1 : -1];
typedef char kf_tmd_ft4_texture_uv2_offset[
    (u32)&((KfTmdFt4TextureWords *)0)->uv2 == 8 ? 1 : -1];
typedef char kf_tmd_ft4_texture_uv3_offset[
    (u32)&((KfTmdFt4TextureWords *)0)->uv3 == 12 ? 1 : -1];
typedef KfTmdUvBytes KfUvScratch;
typedef char kf_tmd_uv_scratch_size[sizeof(KfUvScratch) == 2 ? 1 : -1];
#define WRITE_UV_CACHED(field, value) do { \
    ((u8 *)&(field))[0] = (value).u; \
    ((u8 *)&(field))[1] = (value).v; \
} while (0)
#define WRITE_INDEX(field, value) do { \
    ((u8 *)&(field))[0] = (u8)(value); \
    ((u8 *)&(field))[1] = (u8)((value) >> 8); \
} while (0)
#define MID_INDEX(src, n) ((u16)(((src)->vertex_count + (n)) << 3))
#define MID_VECTOR(dst, lhs, rhs) do { \
    (dst)->vx = ((s32)(lhs)->vx + (s32)(rhs)->vx) >> 1; \
    (dst)->vy = ((s32)(lhs)->vy + (s32)(rhs)->vy) >> 1; \
    (dst)->vz = ((s32)(lhs)->vz + (s32)(rhs)->vz) >> 1; \
} while (0)
#define MID_UV_INTO(dst, lhs, rhs) do { \
    (dst).u = ((u32)(lhs).u + (u32)(rhs).u) >> 1; \
    (dst).v = ((u32)(lhs).v + (u32)(rhs).v) >> 1; \
} while (0)

ADDRESS(0x8002ff5c, 0xcbc)
void func_8002ff5c(KfTmdHeader *asset, s32 object_index,
                   KfTmdPreparedAsset *prepared_asset)
{
    u8 *base;
    KfTmdObject *source;
    KfTmdObject *target;
    u8 *source_packet;
    u8 *output_packet;
    union { SVECTOR vector; u32 words[2]; } corners[4];
    KfTmdFt4TextureWords tex;
    SVECTOR midpoints[128];
    SVECTOR *midpoint_end;
    u32 midpoint_count;
    u32 output_packet_bytes;
    u32 remaining;

    midpoint_count = 0;
    output_packet_bytes = 0;
    midpoint_end = midpoints;
    target = &prepared_asset->object;
    output_packet = (u8 *)target;
    target->primitive_offset = sizeof(KfTmdObject);
    output_packet += target->primitive_offset;
    base = (u8 *)asset + KF_TMD_HEADER_BYTES;
    source = &TMD_OBJECTS(asset)[object_index];
    target->primitive_count = source->primitive_count;
    remaining = source->primitive_count;
    source_packet = base + source->primitive_offset;
    while (--remaining != (u32)-1) {
        KfTmdPacketHeader header;
        KfUvScratch uv_ab, uv_ac, uv_ad, uv_cd, uv_bd;
        header.word = *(u32 *)source_packet;
        if (header.bytes.mode == 0x2c || header.bytes.mode == 0x2e) {
            KfTmdFt4 *face = (KfTmdFt4 *)(source_packet + 4);
            KfTmdFt4PackedIndices *first =
                (KfTmdFt4PackedIndices *)(output_packet + 4);
            KfTmdFt4 *second;
            KfTmdFt4 *third;
            KfTmdFt4 *fourth;
            u16 ab, ac, ad, cd, bd;

            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 8);
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex0);
                corners[0].words[0] = vertex[0];
                corners[0].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex1);
                corners[1].words[0] = vertex[0];
                corners[1].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex2);
                corners[2].words[0] = vertex[0];
                corners[2].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex3);
                corners[3].words[0] = vertex[0];
                corners[3].words[1] = vertex[1];
            }
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[1].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[2].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[3].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[2].vector, &corners[3].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[1].vector, &corners[3].vector);
            midpoint_end++;
            resource_copy_words((u32 *)&tex, (u32 *)(source_packet + 4), 4);
            MID_UV_INTO(uv_ab, tex.uv0, tex.uv1);
            MID_UV_INTO(uv_ac, tex.uv0, tex.uv2);
            MID_UV_INTO(uv_ad, tex.uv0, tex.uv3);
            MID_UV_INTO(uv_cd, tex.uv2, tex.uv3);
            MID_UV_INTO(uv_bd, tex.uv1, tex.uv3);
            ab = MID_INDEX(source, midpoint_count);
            ac = MID_INDEX(source, midpoint_count + 1);
            ad = MID_INDEX(source, midpoint_count + 2);
            cd = MID_INDEX(source, midpoint_count + 3);
            bd = MID_INDEX(source, midpoint_count + 4);
            WRITE_UV_CACHED(first->uv1, uv_ab);
            WRITE_UV_CACHED(first->uv2, uv_ac);
            WRITE_UV_CACHED(first->uv3, uv_ad);
            first->vertex1_vertex2 = (u32)ab | ((u32)ac << 16);
            first->vertex3_pad2 = (u32)ad | ((u32)ac << 16);
            output_packet += 32;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 8);
            second = (KfTmdFt4 *)(output_packet + 4);
            WRITE_UV_CACHED(second->uv0, uv_ab);
            WRITE_UV_CACHED(second->uv2, uv_ad);
            WRITE_UV_CACHED(second->uv3, uv_bd);
            WRITE_INDEX(second->vertex0, ab);
            WRITE_INDEX(second->vertex2, ad);
            WRITE_INDEX(second->vertex3, bd);
            output_packet += 32;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 8);
            third = (KfTmdFt4 *)(output_packet + 4);
            WRITE_UV_CACHED(third->uv0, uv_ac);
            WRITE_UV_CACHED(third->uv1, uv_ad);
            WRITE_UV_CACHED(third->uv3, uv_cd);
            WRITE_INDEX(third->vertex0, ac);
            WRITE_INDEX(third->vertex1, ad);
            WRITE_INDEX(third->vertex3, cd);
            output_packet += 32;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 8);
            fourth = (KfTmdFt4 *)(output_packet + 4);
            WRITE_UV_CACHED(fourth->uv0, uv_ad);
            WRITE_UV_CACHED(fourth->uv1, uv_bd);
            WRITE_UV_CACHED(fourth->uv2, uv_cd);
            WRITE_INDEX(fourth->vertex0, ad);
            WRITE_INDEX(fourth->vertex1, bd);
            WRITE_INDEX(fourth->vertex2, cd);
            output_packet += 32;
            output_packet_bytes += 128;
            midpoint_count += 5;
            target->primitive_count += 3;
        } else if (header.bytes.mode == 0x24 || header.bytes.mode == 0x26) {
            KfTmdFt3 *face = (KfTmdFt3 *)(source_packet + 4);
            KfTmdFt3 *first = (KfTmdFt3 *)(output_packet + 4);
            KfTmdFt3 *second;
            KfTmdFt3 *third;
            KfTmdFt3 *fourth;
            u16 ab, ac, bc;

            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 6);
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex0);
                corners[0].words[0] = vertex[0];
                corners[0].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex1);
                corners[1].words[0] = vertex[0];
                corners[1].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex2);
                corners[2].words[0] = vertex[0];
                corners[2].words[1] = vertex[1];
            }
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[1].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[2].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[1].vector, &corners[2].vector);
            midpoint_end++;
            resource_copy_words((u32 *)&tex, (u32 *)(source_packet + 4), 3);
            MID_UV_INTO(uv_ab, tex.uv0, tex.uv1);
            MID_UV_INTO(uv_ac, tex.uv0, tex.uv2);
            MID_UV_INTO(uv_bd, tex.uv1, tex.uv2);
            ab = MID_INDEX(source, midpoint_count);
            ac = MID_INDEX(source, midpoint_count + 1);
            bc = MID_INDEX(source, midpoint_count + 2);
            WRITE_UV_CACHED(first->uv1, uv_ab);
            WRITE_UV_CACHED(first->uv2, uv_ac);
            WRITE_INDEX(first->vertex1, ab);
            WRITE_INDEX(first->vertex2, ac);
            output_packet += 24;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 6);
            second = (KfTmdFt3 *)(output_packet + 4);
            WRITE_UV_CACHED(second->uv0, uv_ab);
            WRITE_UV_CACHED(second->uv2, uv_bd);
            WRITE_INDEX(second->vertex0, ab);
            WRITE_INDEX(second->vertex2, bc);
            output_packet += 24;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 6);
            third = (KfTmdFt3 *)(output_packet + 4);
            WRITE_UV_CACHED(third->uv0, uv_ac);
            WRITE_UV_CACHED(third->uv1, uv_bd);
            WRITE_INDEX(third->vertex0, ac);
            WRITE_INDEX(third->vertex1, bc);
            output_packet += 24;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 6);
            fourth = (KfTmdFt3 *)(output_packet + 4);
            WRITE_UV_CACHED(fourth->uv0, uv_ab);
            WRITE_UV_CACHED(fourth->uv1, uv_bd);
            WRITE_UV_CACHED(fourth->uv2, uv_ac);
            WRITE_INDEX(fourth->vertex0, ab);
            WRITE_INDEX(fourth->vertex1, bc);
            WRITE_INDEX(fourth->vertex2, ac);
            output_packet += 24;
            output_packet_bytes += 96;
            midpoint_count += 3;
            target->primitive_count += 3;
        } else {
            u32 input_words = header.bytes.input_length + 1;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, input_words);
            output_packet += input_words * KF_TMD_WORD_BYTES;
            output_packet_bytes += input_words * KF_TMD_WORD_BYTES;
        }
        header.word = *(u32 *)source_packet;
        source_packet += (header.bytes.input_length + 1) * KF_TMD_WORD_BYTES;
    }
    target->vertex_offset = target->primitive_offset + output_packet_bytes;
    target->vertex_count = source->vertex_count + midpoint_count;
    resource_copy_words((u32 *)output_packet, (u32 *)(base + source->vertex_offset),
                        source->vertex_count * 2);
    output_packet += source->vertex_count * sizeof(SVECTOR);
    resource_copy_words((u32 *)output_packet, (u32 *)midpoints, midpoint_count * 2);
    output_packet += midpoint_count * sizeof(SVECTOR);
    target->normal_offset = target->vertex_offset + target->vertex_count * sizeof(SVECTOR);
    target->normal_count = source->normal_count;
    resource_copy_words((u32 *)output_packet,
                        (u32 *)(base + source->normal_offset), source->normal_count * 2);
}
#undef MID_INDEX
#undef MID_VECTOR
#undef MID_UV_INTO
#undef WRITE_UV_CACHED
#undef WRITE_INDEX
