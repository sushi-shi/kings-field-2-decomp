#include <kf/lib/address.h>
#include <kf/open/opening.h>
#include <kf/lib/audio.h>
#include <kf/lib/cd_file.h>
#include <kf/lib/display.h>
#include <kf/lib/overlay.h>
#include <psyq/cd.h>
#include <psyq/kernel.h>
#include <psyq/libc.h>
#include <psyq/pad.h>

/* Heap placement literals; the original definitions are unresolved. */
#define OPENING_HEAP_BASE ((u_long *)0x80100000)
#define OPENING_HEAP_BYTES 0xf8000

enum {
    OPENING_DATA_BYTES = 0x96000,
    OPENING_LOAD_ATTEMPTS = 3,
    OPENING_VAB_HEADER_BYTES = 0x1a20,
    OPENING_TITLE_SEQUENCE_BYTES = 0x670,
    OPENING_MOVIE_SEQUENCE_BYTES = 0x1c68,
    /* The title demo plays the movie after this many idle frames. */
    OPENING_IDLE_FRAMES = 931,
    OPENING_MASTER_VOLUME = 127,
    OPENING_SEQUENCE_VOLUME = 64,
    /* opening_fade_out darkens by FADE_STEP per frame up to FADE_END; the
     * prompt stops pulsing at PROMPT_STEADY_LEVEL. */
    OPENING_FADE_STEP = 2,
    OPENING_FADE_END = 96,
    OPENING_PROMPT_STEADY_LEVEL = 32,
    OPENING_CLOSE_FRAMES = 3,
    OPENING_CLOSE_OT_DEPTH = 200
};

DATA(0x8003db88, 0x4)
u8 *overlay_next_request = (u8 *)0x800102f0;
DATA(0x8003db8c, 0x5)
char opening_data_file[5] = "OP.D";

DATA(0x800a6410, 0x4)
u8 *opening_data;
DATA(0x800ac540, 0x1)
u8 display_fade_level;

ADDRESS(0x80011ac0, 0x4dc)
void main(void)
{
    RECT rect;
    s32 prompt_mode = 0;
    s32 idle_frames = 0;
    s32 state;
    s32 choice;
    s32 frame;

    ResetCallback();
    InitHeap(OPENING_HEAP_BASE, OPENING_HEAP_BYTES);
    CdInit();
    PadInit(0);
    ExitCriticalSection();
    audio_vab_header = (u8 *)malloc(OPENING_VAB_HEADER_BYTES);
    audio_title_sequence_data = (u8 *)malloc(OPENING_TITLE_SEQUENCE_BYTES);
    audio_movie_sequence_data = (u8 *)malloc(OPENING_MOVIE_SEQUENCE_BYTES);
    audio_initialize();
    opening_load_data();
    display_initialize();
    display_current = &display_buffers[0];
    PutDrawEnv(&display_current->draw);
    PutDispEnv(&display_current->disp);
    display_current = &display_buffers[1];
    PutDrawEnv(&display_current->draw);
    PutDispEnv(&display_current->disp);
    state = 0;
    opening_open_audio();
restart:
    SsSetMVol(OPENING_MASTER_VOLUME, OPENING_MASTER_VOLUME);
    SsSeqSetVol(audio_title_sequence_id, OPENING_SEQUENCE_VOLUME, OPENING_SEQUENCE_VOLUME);
    SsSeqPlay(audio_title_sequence_id, SSPLAY_PLAY, 1);
    for (;;) {
        display_begin_frame();
        if (state == 0) {
            if (opening_draw_title(KF_TITLE_ANIMATE) == 1) {
                state = 1;
            }
        } else {
            opening_draw_title(KF_TITLE_SHOW);
        }
        if (state == 1 && opening_draw_banner(KF_TITLE_ANIMATE) == 1) {
            break;
        }
        if (PadRead(1) != 0) {
            break;
        }
        display_present_frame();
    }
    while (PadRead(1) != 0) {
    }
    for (;;) {
        display_begin_frame();
        opening_draw_title(KF_TITLE_SHOW);
        opening_draw_banner(KF_TITLE_SHOW);
        choice = opening_poll_pad(&prompt_mode, &idle_frames);
        if (choice == KF_OPENING_START_GAME) {
            *overlay_next_request = KF_OVERLAY_GAME;
            opening_fade_out(prompt_mode);
            break;
        }
        if (choice == KF_OPENING_PLAY_MOVIE) {
            opening_fade_out(prompt_mode);
            opening_play_movie();
            *overlay_next_request = KF_OVERLAY_GAME;
            break;
        }
        if (idle_frames >= OPENING_IDLE_FRAMES) {
            opening_fade_out(prompt_mode);
            state = 0;
            opening_draw_title(KF_TITLE_RESET);
            opening_draw_banner(KF_TITLE_RESET);
            idle_frames = 0;
            opening_play_movie();
            rect.x = rect.y = 0;
            rect.w = 640;
            rect.h = 480;
            ClearImage(&rect, 0, 0, 0);
            goto restart;
        }
        opening_draw_prompt(prompt_mode);
        display_present_frame();
    }
    SetDefDrawEnv(&display_buffers[0].draw, 0, 0, 320, KF_DISPLAY_HEIGHT);
    SetDefDrawEnv(&display_buffers[1].draw, 0, KF_DISPLAY_HEIGHT, 320, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&display_buffers[0].disp, 0, KF_DISPLAY_HEIGHT, 320, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&display_buffers[1].disp, 0, 0, 320, KF_DISPLAY_HEIGHT);
    display_buffers[0].draw.isbg = display_buffers[1].draw.isbg = 1;
    display_current = &display_buffers[0];
    PutDrawEnv(&display_current->draw);
    PutDispEnv(&display_current->disp);
    display_current = &display_buffers[1];
    PutDrawEnv(&display_current->draw);
    PutDispEnv(&display_current->disp);
    for (frame = 0; frame < OPENING_CLOSE_FRAMES; frame++) {
        display_begin_frame();
        primitive_buffer_begin_poly_ft4();
        setTPage(current_poly_ft4, 0, 1, 960, 256);
        current_poly_ft4->clut = getClut(576, 511);
        setXYWH(current_poly_ft4, 32, 0, 255, KF_DISPLAY_HEIGHT);
        setUVWH(current_poly_ft4, 0, 0, 255, KF_DISPLAY_HEIGHT);
        primitive_buffer_commit_poly_ft4(OPENING_CLOSE_OT_DEPTH);
        display_present_frame();
    }
    SsSeqClose(audio_title_sequence_id);
    SsSeqClose(audio_movie_sequence_id);
    SsVabClose(audio_vab_id);
    SsEnd();
    PadStop();
    ResetGraph(3);
}

/* Darkens the title screen while fading the title music out. */
ADDRESS(0x80011f9c, 0xc4)
void opening_fade_out(s32 prompt_mode)
{
    s32 level = 0;

    do {
        display_begin_frame();
        display_fade_level = level;
        opening_draw_title(KF_TITLE_SHOW);
        opening_draw_banner(KF_TITLE_SHOW);
        opening_draw_prompt(prompt_mode);
        display_present_frame();
        if (level == OPENING_PROMPT_STEADY_LEVEL) {
            prompt_mode = KF_TITLE_RESET;
        }
        if (level * 2 < OPENING_MASTER_VOLUME) {
            SsSetMVol(OPENING_MASTER_VOLUME - level * 2, OPENING_MASTER_VOLUME - level * 2);
        }
        level += OPENING_FADE_STEP;
    } while (level < OPENING_FADE_END);
    SsSeqStop(audio_title_sequence_id);
    SsSeqSetVol(audio_title_sequence_id, 0, 0);
    SsSetMVol(0, 0);
}

ADDRESS(0x80012060, 0x68)
void opening_load_data(void)
{
    s32 attempt;

    opening_data = (u8 *)malloc(OPENING_DATA_BYTES);
    for (attempt = OPENING_LOAD_ATTEMPTS - 1; attempt != -1; attempt--) {
        if (cd_file_load_into((u_long *)opening_data, opening_data_file) == KF_CD_LOADED) {
            break;
        }
    }
}
