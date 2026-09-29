#include <kf/lib/address.h>
#include <kf/end/ending.h>
#include <kf/lib/audio.h>

/* ED.D (262932 bytes) holds the VAB header, the VAB body, then the sequence,
 * which runs to the end of the file. */
enum {
    ENDING_VAB_HEADER_BYTES = 0x1c20,
    ENDING_VAB_BODY_BYTES = 0x3c170,
    ENDING_SEQUENCE_BYTES = 0x2584
};

ADDRESS(0x80011cec, 0xc0)
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
    audio_sequence_data = cursor;
    /* Unused advance past the last section, as OPEN's loader walks every
     * section; retail reloads audio_sequence_data because of it. */
    cursor += ENDING_SEQUENCE_BYTES;
    audio_sequence_id = SsSeqOpen((u_long *)audio_sequence_data, audio_vab_id);
    DrawSync(0);
}
