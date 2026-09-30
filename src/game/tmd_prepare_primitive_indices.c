#include <kf/lib/address.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

RODATA(0x80011410, 0x74)

ADDRESS(0x8002d5dc, 0x2d4)
void tmd_prepare_primitive_indices(KfTmdHeader *tmd)
{
    KfTmdObject *object;
    u8 *packet;
    u8 *body;
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
            body = TMD_PACKET_BODY(packet);
            header.word = *(u32 *)packet;
            packet = body + header.bytes.input_length * KF_TMD_WORD_BYTES;
            primitive = (KfTmdPrimitive *)body;
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
