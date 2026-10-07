#include "runtime.h"

#include <kf/psx/sdk.h>

#include <cstring>

// Movie decoder calls. Bitstream decoding is not implemented yet: output slices
// are black, and completion callbacks run synchronously so playback advances.

namespace kf::psx {
namespace {
struct Decoder {
    void (*output_callback)();
    bool in_callback;
    bool pending;
    u_long *output;
    int output_words;
};
Decoder decoder;
}
}

using namespace kf::psx;

extern "C" {
void DecDCTReset(int) { decoder = {}; }

void DecDCTvlc(u_long *, u_long *) {
    warn_once("DecDCTvlc (movie decoding)");
}

void DecDCTin(u_long *, int) {}

void DecDCTout(u_long *buf, int size) {
    decoder.output = buf;
    decoder.output_words = size;
    decoder.pending = true;
    if (decoder.in_callback)
        return;
    // The callback queues the next slice; run until it stops requesting more.
    while (decoder.pending) {
        decoder.pending = false;
        std::memset(decoder.output, 0, static_cast<std::size_t>(decoder.output_words) * 4);
        if (!decoder.output_callback)
            break;
        decoder.in_callback = true;
        decoder.output_callback();
        decoder.in_callback = false;
    }
}

int DecDCToutCallback(void (*func)()) {
    decoder.output_callback = func;
    return 0;
}
}
