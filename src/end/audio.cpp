#include <kf/end/ending.h>
#include <kf/lib/audio.h>

enum {
    ENDING_VAB_HEADER_BYTES = 0x1c20,
    ENDING_VAB_BODY_BYTES = 0x3c170,
    ENDING_SEQUENCE_BYTES = 0x2584
};

u_long *audio_sequence_data;

short audio_vab_id;

u8 *audio_vab_header;

short audio_sequence_id;

void ending_open_audio(void)
{
    u8 *cursor = ending_data;

    audio_vab_header = cursor;
    cursor += ENDING_VAB_HEADER_BYTES;
    audio_vab_id = SsVabOpenHead(audio_vab_header, -1);
    if (audio_vab_id != -1 && SsVabTransBody(cursor, audio_vab_id) == audio_vab_id) {
        SsVabTransCompleted(SS_WAIT_COMPLETED);
    }
    cursor += ENDING_VAB_BODY_BYTES;
    audio_sequence_data = (u_long *)cursor;

    cursor += ENDING_SEQUENCE_BYTES;
    audio_sequence_id = SsSeqOpen(audio_sequence_data, audio_vab_id);
    DrawSync(0);
}
