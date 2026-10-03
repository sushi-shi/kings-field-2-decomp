#ifndef KF_GAME_TMD_PACKETS_H
#define KF_GAME_TMD_PACKETS_H

#include <kf/lib/types.h>
#include <kf/game/tmd.h>
#include <psyq/sdk.h>

/* Prepared TMD packet indices are byte offsets into projected vertices. */
enum {
    KF_TMD_PACKET_HEADER_BYTES = 4,
    KF_TMD_WORD_BYTES = 4,
    KF_TMD_INPUT_LENGTH_TO_BYTES_SHIFT = 6,
    KF_TMD_PACKET_BODY_BYTES_MASK = 0x3fc,
    KF_TMD_VECTOR_OFFSET_SHIFT = 3,
    KF_TMD_MODE_F3 = 0x20,
    KF_TMD_MODE_FT3 = 0x24,
    KF_TMD_MODE_F4 = 0x28,
    KF_TMD_MODE_FT4 = 0x2c,
    KF_TMD_MODE_G3 = 0x30,
    KF_TMD_MODE_GT3 = 0x34,
    KF_TMD_MODE_G4 = 0x38,
    KF_TMD_MODE_GT4 = 0x3c,
    KF_TMD_MODE_MASK = 0xfd,
    KF_MAP_OT_DEPTH_BIAS = 240,
    KF_MAP_OT_DEPTH_LIMIT = 8192,
    KF_MAP_CELL_PREPARED_BYTES = 4096,
    KF_MAP_CELL_PREPARED_PAYLOAD_BYTES = 4056
};

#define TMD_PACKET_BODY(packet) ((packet) + KF_TMD_PACKET_HEADER_BYTES)
#define TMD_PACKET_BODY_BYTES(header_word) \
    (((header_word) >> KF_TMD_INPUT_LENGTH_TO_BYTES_SHIFT) & \
     KF_TMD_PACKET_BODY_BYTES_MASK)
#define tmd_packet_kind(word) (((word) >> 24) & KF_TMD_MODE_MASK)

typedef union KfTmdPacketHeader {
    u32 word;
    struct {
        u8 output_length;
        u8 input_length;
        u8 flag;
        u8 mode;
    } bytes;
} KfTmdPacketHeader;

typedef char kf_tmd_packet_header_size[
    sizeof(KfTmdPacketHeader) == KF_TMD_PACKET_HEADER_BYTES ? 1 : -1];
typedef char kf_tmd_packet_input_length_offset[
    (u32)&((KfTmdPacketHeader *)0)->bytes.input_length == 1 ? 1 : -1];
typedef char kf_tmd_packet_mode_offset[
    (u32)&((KfTmdPacketHeader *)0)->bytes.mode == 3 ? 1 : -1];

/* The cell renderer supplies a complete temporary TMD to its packet helper. */
typedef struct KfTmdPreparedAsset {
    KfTmdHeader header;
    KfTmdObject object;
    u8 payload[KF_MAP_CELL_PREPARED_PAYLOAD_BYTES];
} KfTmdPreparedAsset;

typedef char kf_tmd_prepared_asset_size[
    sizeof(KfTmdPreparedAsset) == KF_MAP_CELL_PREPARED_BYTES ? 1 : -1];
typedef char kf_tmd_prepared_object_offset[
    (u32)&((KfTmdPreparedAsset *)0)->object == KF_TMD_HEADER_BYTES ? 1 : -1];
typedef char kf_tmd_prepared_payload_offset[
    (u32)&((KfTmdPreparedAsset *)0)->payload == 0x28 ? 1 : -1];

typedef struct KfTmdFt3 {
    u16 uv0;
    u16 clut;
    u16 uv1;
    u16 tpage;
    u16 uv2;
    u16 pad;
    u16 normal;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
} KfTmdFt3;

typedef struct KfTmdFt4 {
    u16 uv0;
    u16 clut;
    u16 uv1;
    u16 tpage;
    u16 uv2;
    u16 pad0;
    u16 uv3;
    u16 pad1;
    u16 normal;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
    u16 pad2;
} KfTmdFt4;

/* The first subdivided FT4 child writes its two trailing index pairs as words. */
typedef struct KfTmdFt4PackedIndices {
    u16 uv0;
    u16 clut;
    u16 uv1;
    u16 tpage;
    u16 uv2;
    u16 pad0;
    u16 uv3;
    u16 pad1;
    u16 normal;
    u16 vertex0;
    u32 vertex1_vertex2;
    u32 vertex3_pad2;
} KfTmdFt4PackedIndices;

/* On-disk primitive bodies. Each index is a halfword until the preparation
 * pass converts it to a byte offset into the projected-vector array. */
typedef struct KfTmdF3 {
    u8 color[4];
    u16 normal;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
} KfTmdF3;

typedef struct KfTmdG3 {
    CVECTOR color;
    u16 normal0;
    u16 vertex0;
    u16 normal1;
    u16 vertex1;
    u16 normal2;
    u16 vertex2;
} KfTmdG3;

typedef struct KfTmdF4 {
    u8 color[4];
    u16 normal;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
    u16 pad;
} KfTmdF4;

typedef struct KfTmdG4 {
    CVECTOR color;
    u16 normal0;
    u16 vertex0;
    u16 normal1;
    u16 vertex1;
    u16 normal2;
    u16 vertex2;
    u16 normal3;
    u16 vertex3;
} KfTmdG4;

typedef struct KfTmdGt3 {
    u16 uv0;
    u16 clut;
    u16 uv1;
    u16 tpage;
    u16 uv2;
    u16 pad;
    u16 normal0;
    u16 vertex0;
    u16 normal1;
    u16 vertex1;
    u16 normal2;
    u16 vertex2;
} KfTmdGt3;

typedef struct KfTmdGt4 {
    u16 uv0;
    u16 clut;
    u16 uv1;
    u16 tpage;
    u16 uv2;
    u16 pad0;
    u16 uv3;
    u16 pad1;
    u16 normal0;
    u16 vertex0;
    u16 normal1;
    u16 vertex1;
    u16 normal2;
    u16 vertex2;
    u16 normal3;
    u16 vertex3;
} KfTmdGt4;

typedef union KfTmdPrimitive {
    KfTmdF3 f3;
    KfTmdG3 g3;
    KfTmdFt3 ft3;
    KfTmdGt3 gt3;
    KfTmdF4 f4;
    KfTmdG4 g4;
    KfTmdFt4 ft4;
    KfTmdGt4 gt4;
} KfTmdPrimitive;

typedef char kf_tmd_f3_size[sizeof(KfTmdF3) == 12 ? 1 : -1];
typedef char kf_tmd_g3_size[sizeof(KfTmdG3) == 16 ? 1 : -1];
typedef char kf_tmd_f4_size[sizeof(KfTmdF4) == 16 ? 1 : -1];
typedef char kf_tmd_g4_size[sizeof(KfTmdG4) == 20 ? 1 : -1];
typedef char kf_tmd_gt3_size[sizeof(KfTmdGt3) == 24 ? 1 : -1];
typedef char kf_tmd_gt4_size[sizeof(KfTmdGt4) == 32 ? 1 : -1];
typedef char kf_tmd_f3_normal_offset[
    (u32)&((KfTmdF3 *)0)->normal == 4 ? 1 : -1];
typedef char kf_tmd_g3_last_vertex_offset[
    (u32)&((KfTmdG3 *)0)->vertex2 == 14 ? 1 : -1];
typedef char kf_tmd_f4_last_vertex_offset[
    (u32)&((KfTmdF4 *)0)->vertex3 == 12 ? 1 : -1];
typedef char kf_tmd_g4_last_vertex_offset[
    (u32)&((KfTmdG4 *)0)->vertex3 == 18 ? 1 : -1];
typedef char kf_tmd_gt3_normal_offset[
    (u32)&((KfTmdGt3 *)0)->normal0 == 12 ? 1 : -1];
typedef char kf_tmd_gt4_normal_offset[
    (u32)&((KfTmdGt4 *)0)->normal0 == 16 ? 1 : -1];

typedef struct KfGpuGT3Packed {
        u_long tag;
        CVECTOR color0;
        long xy0;
        u16 uv0;
        u16 clut;
        CVECTOR color1;
        long xy1;
        u16 uv1;
        u16 tpage;
        CVECTOR color2;
        long xy2;
        u16 uv2;
        u16 pad;
} KfGpuGT3Packed;

typedef union KfGpuGT3 {
    POLY_GT3 sdk;
    KfGpuGT3Packed packed;
} KfGpuGT3;

typedef struct KfGpuGT4Packed {
        u_long tag;
        CVECTOR color0;
        long xy0;
        u16 uv0;
        u16 clut;
        CVECTOR color1;
        long xy1;
        u16 uv1;
        u16 tpage;
        CVECTOR color2;
        long xy2;
        u16 uv2;
        u16 pad0;
        CVECTOR color3;
        long xy3;
        u16 uv3;
        u16 pad1;
} KfGpuGT4Packed;

typedef union KfGpuGT4 {
    POLY_GT4 sdk;
    KfGpuGT4Packed packed;
} KfGpuGT4;

typedef char kf_tmd_ft3_size[sizeof(KfTmdFt3) == 20 ? 1 : -1];
typedef char kf_tmd_ft4_size[sizeof(KfTmdFt4) == 28 ? 1 : -1];
typedef char kf_tmd_ft4_packed_indices_size[
    sizeof(KfTmdFt4PackedIndices) == sizeof(KfTmdFt4) ? 1 : -1];
typedef char kf_tmd_ft4_packed_indices_offset[
    (u32)&((KfTmdFt4PackedIndices *)0)->vertex1_vertex2 == 20 ? 1 : -1];
typedef char kf_tmd_ft3_normal_offset[
    (u32)&((KfTmdFt3 *)0)->normal == 12 ? 1 : -1];
typedef char kf_tmd_ft4_normal_offset[
    (u32)&((KfTmdFt4 *)0)->normal == 16 ? 1 : -1];
typedef char kf_tmd_ft4_last_vertex_offset[
    (u32)&((KfTmdFt4 *)0)->vertex3 == 24 ? 1 : -1];
typedef char kf_gpu_gt3_size[sizeof(KfGpuGT3) == 40 ? 1 : -1];
typedef char kf_gpu_gt4_size[sizeof(KfGpuGT4) == 52 ? 1 : -1];
typedef char kf_gpu_gt3_color2_offset[
    (u32)&((KfGpuGT3 *)0)->packed.color2 == 28 ? 1 : -1];
typedef char kf_gpu_gt4_color3_offset[
    (u32)&((KfGpuGT4 *)0)->packed.color3 == 40 ? 1 : -1];
typedef char kf_gpu_gt4_last_uv_offset[
    (u32)&((KfGpuGT4 *)0)->packed.uv3 == 48 ? 1 : -1];

extern CVECTOR map_textured_primitive_color;
void render_enqueue_blended_tmd(u16 object_index, s32 depth_bias, s32 render_mode);
void render_enqueue_textured_tmd(u16 object_index, s32 depth_bias);
void render_enqueue_tmd_fixed_depth(u16 object_index, s32 blend_mode, s16 fixed_depth);
void render_enqueue_map(u16 object_index);
void render_enqueue_clipped_tmd_polygon(s32 vertex_count, SVECTOR *normal, u16 clut, u16 tpage,
                   u32 mode, s32 depth_bias);
void render_enqueue_tmd_with_clipping(u16 object_index, s32 depth_bias,
                   KfTmdPreparedAsset *prepared_asset);
void tmd_prepare_subdivided_object(KfTmdHeader *asset, s32 object_index,
                   KfTmdPreparedAsset *prepared_asset);

#endif
