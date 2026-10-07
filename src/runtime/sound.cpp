#include "runtime.h"

#include <kf/psx/sdk.h>

#include <cstring>

// Sound library calls. Bank and sequence handles behave as the library's, but
// nothing is audible yet; the audio milestone connects them to the mixer.

namespace kf::psx {
namespace {
short next_vab;
short next_sequence;
}
void sound_update() {}
}

using namespace kf::psx;

extern "C" {
void SsInit(void) {
    next_vab = 0;
    next_sequence = 0;
    warn_once("sound playback");
}
void SsSetTickMode(long) {}
void SsSetTableSize(char *, short, short) {}
void SsStart2(void) {}
void SsEnd(void) {}
short SsUtSetReverbType(short type) { return type; }
void SsUtReverbOn(void) {}
void SsUtSetReverbDepth(short, short) {}
void SsSetMVol(short, short) {}
short SsSeqOpen(unsigned long *, short) { return next_sequence++ & 0x0f; }
void SsSeqPlay(short, char, short) {}
void SsSeqStop(short) {}
void SsSeqClose(short) {}
void SsSeqSetVol(short, short, short) {}
void SsSeqPause(short) {}
void SsSeqReplay(short) {}
void SsSeqCalledTbyT(void) {}
short SsVabOpenHead(unsigned char *, short vab_id) { return vab_id >= 0 ? vab_id : next_vab++ & 0x0f; }
short SsVabTransBody(unsigned char *, short vab_id) { return vab_id; }
short SsVabTransBodyPartly(unsigned char *, unsigned long, short vab_id) { return vab_id; }
short SsVabTransCompleted(short) { return 1; }
void SsVabClose(short) {}
short SsUtKeyOn(short, short, short, short, short, short, short) { return 0; }
short SsUtKeyOff(short, short, short, short, short) { return 0; }
void SpuGetAllKeysStatus(char *status) { std::memset(status, 0, 24); }
}
