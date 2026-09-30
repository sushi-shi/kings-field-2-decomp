#include <kf/lib/address.h>
#include <kf/game/animation.h>

enum { KF_ANIMATION_SPARSE_SKIP = -32768 };

ADDRESS(0x80033ff4, 0x7c)
const s16 *animation_find_sparse_vertex(const s16 *encoded, s32 vertex_index)
{
    s32 remaining = *encoded;
    s32 current = 0;

    encoded++;
    for (--remaining; remaining != -1; --remaining) {
        s16 value = *encoded;

        if (value == KF_ANIMATION_SPARSE_SKIP) {
            encoded++;
            current += *encoded++;
            if (vertex_index < current) {
                return 0;
            }
        } else {
            if (current == vertex_index) {
                return encoded;
            }
            current++;
            encoded += 3;
        }
    }
    return 0;
}
