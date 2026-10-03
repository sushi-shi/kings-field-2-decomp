#ifndef KF_AUDIO_H
#define KF_AUDIO_H
#include <kf/lib/types.h>
#include <psyq/audio.h>

/* Sequence playback state shared by OPEN.EXE and END.EXE. */
enum {
    KF_AUDIO_SEQUENCE_CAPACITY = 2,
    KF_AUDIO_TRACKS_PER_SEQUENCE = 1
};

extern char audio_sequence_table[SS_SEQ_TABSIZ * KF_AUDIO_SEQUENCE_CAPACITY * KF_AUDIO_TRACKS_PER_SEQUENCE];
extern u8 *audio_vab_header;
extern short audio_vab_id;
extern u_long *audio_sequence_data;
extern short audio_sequence_id;

void audio_initialize(void);

#endif
