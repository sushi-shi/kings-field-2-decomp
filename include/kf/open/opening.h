#ifndef KF_OPEN_OPENING_H
#define KF_OPEN_OPENING_H
#include <kf/lib/types.h>
#include <sys/types.h>

/* OPEN.EXE: loads OP.D, runs the title screen and plays the opening movie. */
enum {
    /* Modes of the fading title elements. */
    KF_TITLE_ANIMATE = 0,
    KF_TITLE_SHOW = 1,
    KF_TITLE_RESET = 2
};

enum {
    /* opening_poll_pad results. */
    KF_OPENING_NO_CHOICE = 0,
    KF_OPENING_START_GAME = 1,
    KF_OPENING_PLAY_MOVIE = 2
};

extern u8 *opening_data;
extern char opening_data_file[5];
extern u8 *audio_title_sequence_data;
extern u8 *audio_movie_sequence_data;
extern short audio_title_sequence_id;
extern short audio_movie_sequence_id;

void opening_fade_out(s32 prompt_mode);
void opening_load_data(void);
void opening_open_audio(void);
s32 opening_draw_title(s32 mode);
s32 opening_draw_banner(s32 mode);
void opening_draw_prompt(s32 mode);
s32 opening_poll_pad(s32 *prompt_mode, s32 *idle_frames);
void primitive_buffer_begin_poly_ft4(void);
void primitive_buffer_commit_poly_ft4(s32 depth);
void tim_upload_images(u8 *tim_data);
void audio_play_voice(
    s16 vab_id, s16 program, s16 tone, s16 note, s16 left_volume, s16 right_volume);
void opening_play_movie(void);

#endif
