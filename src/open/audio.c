#include <kf/lib/address.h>
#include <kf/open/opening.h>
#include <kf/lib/audio.h>
#include <psyq/libc.h>

/* OP.D holds the title TIM images, the VAB header and body, then the title
 * and movie sequences. */
enum {
    OPENING_IMAGE_BYTES = 0x497cc,
    OPENING_VAB_HEADER_BYTES = 0x1a20,
    OPENING_VAB_BODY_BYTES = 0x34ca0,
    OPENING_TITLE_SEQUENCE_BYTES = 0x670,
    OPENING_MOVIE_SEQUENCE_BYTES = 0x1c68
};

ADDRESS(0x80012270, 0x2f0)
void opening_open_audio(void)
{
    u8 *cursor = opening_data;

    tim_upload_images(cursor);
    cursor += OPENING_IMAGE_BYTES;
    memcpy(audio_vab_header, cursor, OPENING_VAB_HEADER_BYTES);
    cursor += OPENING_VAB_HEADER_BYTES;
    audio_vab_id = SsVabOpenHead(audio_vab_header, -1);
    if (audio_vab_id != -1 && SsVabTransBody(cursor, audio_vab_id) == audio_vab_id) {
        SsVabTransCompleted(SS_WAIT_COMPLETED);
    }
    cursor += OPENING_VAB_BODY_BYTES;
    memcpy(audio_title_sequence_data, cursor, OPENING_TITLE_SEQUENCE_BYTES);
    cursor += OPENING_TITLE_SEQUENCE_BYTES;
    memcpy(audio_movie_sequence_data, cursor, OPENING_MOVIE_SEQUENCE_BYTES);
    audio_title_sequence_id = SsSeqOpen((u_long *)audio_title_sequence_data, audio_vab_id);
    audio_movie_sequence_id = SsSeqOpen((u_long *)audio_movie_sequence_data, audio_vab_id);
    DrawSync(0);
    free(opening_data);
}
