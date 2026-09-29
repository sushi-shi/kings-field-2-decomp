#include <kf/lib/address.h>
#include <kf/lib/audio.h>

enum {
    AUDIO_REVERB_DEPTH = 0x20
};

ADDRESS_AT("OPEN", 0x80012204, 0x6c)
ADDRESS_AT("END", 0x80011c80, 0x6c)
void audio_initialize(void)
{
    SsInit();
    SsSetTickMode(SS_TICK60);
    SsSetTableSize(audio_sequence_table, KF_AUDIO_SEQUENCE_CAPACITY, KF_AUDIO_TRACKS_PER_SEQUENCE);
    SsUtSetReverbType(SS_REV_TYPE_HALL);
    SsUtReverbOn();
    SsUtSetReverbDepth(AUDIO_REVERB_DEPTH, AUDIO_REVERB_DEPTH);
    SsStart2();
    SsSetMVol(0, 0);
}
