#include <kf/open/opening.h>
#include <kf/lib/audio.h>
#include <psyq/libc.h>

enum {
    OPENING_IMAGE_BYTES = 0x497cc,
    OPENING_VAB_BODY_BYTES = 0x34ca0
};

u_long *audio_title_sequence_data;

u_long *audio_movie_sequence_data;

short audio_vab_id;

u8 *audio_vab_header;

short audio_title_sequence_id;

short audio_movie_sequence_id;

void opening_open_audio(void)
{
    u8 *cursor = opening_data;

    tim_upload_images(cursor);
    cursor += OPENING_IMAGE_BYTES;
    memcpy((void *)audio_vab_header, (const void *)cursor, KF_OPENING_VAB_HEADER_BYTES);
    cursor += KF_OPENING_VAB_HEADER_BYTES;
    audio_vab_id = SsVabOpenHead(audio_vab_header, -1);
    if (audio_vab_id != -1 && SsVabTransBody(cursor, audio_vab_id) == audio_vab_id) {
        SsVabTransCompleted(SS_WAIT_COMPLETED);
    }
    cursor += OPENING_VAB_BODY_BYTES;
    memcpy((void *)audio_title_sequence_data, (const void *)cursor, KF_OPENING_TITLE_SEQUENCE_BYTES);
    cursor += KF_OPENING_TITLE_SEQUENCE_BYTES;
    memcpy((void *)audio_movie_sequence_data, (const void *)cursor, KF_OPENING_MOVIE_SEQUENCE_BYTES);
    audio_title_sequence_id = SsSeqOpen(audio_title_sequence_data, audio_vab_id);
    audio_movie_sequence_id = SsSeqOpen(audio_movie_sequence_data, audio_vab_id);
    DrawSync(0);
    free((void *)opening_data);
}
