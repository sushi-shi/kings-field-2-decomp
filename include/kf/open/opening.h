#ifndef KF_OPEN_OPENING_H
#define KF_OPEN_OPENING_H
#include <kf/lib/bool.h>
#include <kf/lib/types.h>
#include <kf/lib/enum.h>
#include <sys/types.h>

enum class KfTitleMode : s32 {
    KF_TITLE_ANIMATE = 0,
    KF_TITLE_SHOW = 1,
    KF_TITLE_RESET = 2
}; using enum KfTitleMode;

enum class KfOpeningChoice : s32 {
    KF_OPENING_NO_CHOICE = 0,
    KF_OPENING_START_GAME = 1,
    KF_OPENING_PLAY_MOVIE = 2
}; using enum KfOpeningChoice;

enum class KfTitlePhase : s32 {
    KF_TITLE_PHASE_FADE_IN = 0,
    KF_TITLE_PHASE_SETTLE = 1
}; using enum KfTitlePhase;

enum class KfPromptPhase : s32 {
    KF_PROMPT_PHASE_BRIGHTEN = 0,
    KF_PROMPT_PHASE_DIM = 1
}; using enum KfPromptPhase;

enum class KfOpeningState : s32 {
    KF_OPENING_STATE_TITLE_FADE_IN = 0,
    KF_OPENING_STATE_BANNER_FADE_IN = 1
}; using enum KfOpeningState;

enum {
    KF_OPENING_VAB_HEADER_BYTES = 0x1a20,
    KF_OPENING_TITLE_SEQUENCE_BYTES = 0x670,
    KF_OPENING_MOVIE_SEQUENCE_BYTES = 0x1c68,
    KF_OPENING_SEQUENCE_VOLUME = 64
};

extern u8 *opening_data;
extern char opening_data_file[5];
extern u_long *audio_title_sequence_data;
extern u_long *audio_movie_sequence_data;
extern short audio_title_sequence_id;
extern short audio_movie_sequence_id;

void opening_fade_out(KfTitleMode prompt_mode);
void opening_load_data(void);
void opening_open_audio(void);
b32 opening_draw_title(KfTitleMode mode);
b32 opening_draw_banner(KfTitleMode mode);
void opening_draw_prompt(KfTitleMode mode);
KfOpeningChoice opening_poll_pad(const KfTitleMode *prompt_mode, s32 *idle_frames);
void primitive_buffer_begin_poly_ft4(void);
void primitive_buffer_commit_poly_ft4(s32 depth);
void tim_upload_images(u8 *tim_data);
void audio_play_voice(
    s16 vab_id, s16 program, s16 tone, s16 note, s16 left_volume, s16 right_volume);
void opening_play_movie(void);

#endif
