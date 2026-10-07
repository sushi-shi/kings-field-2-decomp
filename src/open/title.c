#include <kf/lib/address.h>
#include <kf/open/opening.h>
#include <kf/lib/audio.h>
#include <kf/lib/display.h>
#include <psyq/pad.h>

/* Title elements fade in to PEAK_LEVEL, then settle back to REST_LEVEL. */
enum {
    TITLE_FADE_STEP = 6,
    TITLE_PEAK_LEVEL = 144,
    TITLE_REST_LEVEL = 96,
    PROMPT_PULSE_STEP = 3,
    PROMPT_PULSE_HIGH = 101,
    PROMPT_PULSE_LOW = 48,
    TITLE_BACKDROP_LEVEL = 32,
    TITLE_BACKDROP_WIDTH = 640,
    TITLE_BACKDROP_OT_DEPTH = 100,
    TITLE_OT_DEPTH = 200,
    PRIMITIVE_BRIGHTNESS = 64,
    /* Voice played when a title choice is made. */
    CHOICE_VOICE_PROGRAM = 10,
    CHOICE_VOICE_NOTE = 60,
    CHOICE_VOICE_VOLUME = 64,
    TITLE_PHASE_FADE_IN = 0,
    TITLE_PHASE_SETTLE = 1,
    PROMPT_PHASE_BRIGHTEN = 0,
    PROMPT_PHASE_DIM = 1
};

DATA(0x8003db94, 0x4, ".data")
s32 title_level = 0;
DATA(0x8003db98, 0x4, ".data")
s32 title_phase = TITLE_PHASE_FADE_IN;
DATA(0x8003db9c, 0x4, ".data")
s32 banner_level = 0;
DATA(0x8003dba0, 0x4, ".data")
s32 banner_phase = TITLE_PHASE_FADE_IN;
DATA(0x8003dba4, 0x4, ".data")
s32 prompt_level = PROMPT_PULSE_LOW;
DATA(0x8003dba8, 0x4, ".data")
s32 prompt_phase = PROMPT_PHASE_BRIGHTEN;
DATA(0x8003dbac, 0x4, ".data")
u32 pad_previous_buttons = 0;

/* Draws the backdrop and the four title tiles; returns 1 once settled. */
ADDRESS(0x80012560, 0x61c)
s32 opening_draw_title(s32 mode)
{
    s32 settled = 0;

    if (mode == KF_TITLE_SHOW) {
        title_phase = TITLE_PHASE_SETTLE;
        title_level = TITLE_REST_LEVEL;
    } else if (mode == KF_TITLE_RESET) {
        title_level = 0;
        title_phase = TITLE_PHASE_FADE_IN;
        return 0;
    }
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, TITLE_BACKDROP_LEVEL, TITLE_BACKDROP_LEVEL, TITLE_BACKDROP_LEVEL);
    setTPage(current_poly_ft4, 2, 0, 640, 0);
    setXYWH(current_poly_ft4, 0, 0, TITLE_BACKDROP_WIDTH, KF_DISPLAY_HEIGHT);
    setUVWH(current_poly_ft4, 0, 0, 242, 239);
    primitive_buffer_commit_poly_ft4(TITLE_BACKDROP_OT_DEPTH);
    if (title_phase == TITLE_PHASE_FADE_IN) {
        title_level += TITLE_FADE_STEP;
        if (title_level >= TITLE_PEAK_LEVEL) {
            title_phase = TITLE_PHASE_SETTLE;
        }
    } else if (title_level > TITLE_REST_LEVEL) {
        title_level -= TITLE_FADE_STEP;
    } else {
        settled = 1;
    }
    primitive_buffer_begin_poly_ft4();
    setTPage(current_poly_ft4, 2, 1, 640, 256);
    setRGB0(current_poly_ft4, title_level, title_level, title_level);
    setXYWH(current_poly_ft4, 20, 55, 256, 121);
    setUVWH(current_poly_ft4, 0, 0, 255, 121);
    primitive_buffer_commit_poly_ft4(TITLE_OT_DEPTH);
    primitive_buffer_begin_poly_ft4();
    setTPage(current_poly_ft4, 2, 1, 896, 256);
    setRGB0(current_poly_ft4, title_level, title_level, title_level);
    setXYWH(current_poly_ft4, 276, 55, 44, 121);
    setUVWH(current_poly_ft4, 0, 0, 44, 121);
    primitive_buffer_commit_poly_ft4(TITLE_OT_DEPTH);
    primitive_buffer_begin_poly_ft4();
    setTPage(current_poly_ft4, 2, 1, 640, 256);
    setRGB0(current_poly_ft4, title_level, title_level, title_level);
    setXYWH(current_poly_ft4, 320, 55, 256, 121);
    setUVWH(current_poly_ft4, 0, 128, 255, 121);
    primitive_buffer_commit_poly_ft4(TITLE_OT_DEPTH);
    primitive_buffer_begin_poly_ft4();
    setTPage(current_poly_ft4, 2, 1, 896, 256);
    setRGB0(current_poly_ft4, title_level, title_level, title_level);
    setXYWH(current_poly_ft4, 576, 55, 44, 121);
    setUVWH(current_poly_ft4, 0, 128, 44, 121);
    primitive_buffer_commit_poly_ft4(TITLE_OT_DEPTH);
    return settled;
}

/* Draws the banner under the title; returns 1 once settled. */
ADDRESS(0x80012b7c, 0x200)
s32 opening_draw_banner(s32 mode)
{
    s32 settled = 0;

    if (mode == KF_TITLE_SHOW) {
        banner_phase = TITLE_PHASE_SETTLE;
        banner_level = TITLE_REST_LEVEL;
    } else if (mode == KF_TITLE_RESET) {
        banner_level = 0;
        banner_phase = TITLE_PHASE_FADE_IN;
        return 0;
    }
    if (banner_phase == TITLE_PHASE_FADE_IN) {
        banner_level += TITLE_FADE_STEP;
        if (banner_level >= TITLE_PEAK_LEVEL) {
            banner_phase = TITLE_PHASE_SETTLE;
        }
    } else if (banner_level > TITLE_REST_LEVEL) {
        banner_level -= TITLE_FADE_STEP;
    } else {
        settled = 1;
    }
    primitive_buffer_begin_poly_ft4();
    setTPage(current_poly_ft4, 0, 1, 896, 0);
    current_poly_ft4->clut = getClut(0, 500);
    SetSemiTrans((void *)current_poly_ft4, 1);
    setRGB0(current_poly_ft4, banner_level, banner_level, banner_level);
    setXYWH(current_poly_ft4, 160, 180, 320, 16);
    setUVWH(current_poly_ft4, 0, 0, 255, 16);
    primitive_buffer_commit_poly_ft4(TITLE_OT_DEPTH);
    return settled;
}

/* Draws the prompt; KF_TITLE_ANIMATE makes it pulse. */
ADDRESS(0x80012d7c, 0x1c0)
void opening_draw_prompt(s32 mode)
{
    if (prompt_phase == PROMPT_PHASE_BRIGHTEN) {
        prompt_level += PROMPT_PULSE_STEP;
        if (prompt_level >= PROMPT_PULSE_HIGH) {
            prompt_phase = PROMPT_PHASE_DIM;
        }
    } else {
        prompt_level -= PROMPT_PULSE_STEP;
        if (prompt_level < PROMPT_PULSE_LOW) {
            prompt_phase = PROMPT_PHASE_BRIGHTEN;
        }
    }
    primitive_buffer_begin_poly_ft4();
    setTPage(current_poly_ft4, 0, 1, 896, 0);
    current_poly_ft4->clut = getClut(0, 501);
    SetSemiTrans((void *)current_poly_ft4, 1);
    if (mode == KF_TITLE_ANIMATE) {
        setRGB0(current_poly_ft4, prompt_level, prompt_level, prompt_level);
    }
    setXYWH(current_poly_ft4, 287, 204, 64, 12);
    setUVWH(current_poly_ft4, 0, 128, 64, 12);
    primitive_buffer_commit_poly_ft4(TITLE_OT_DEPTH);
}

/* A newly pressed face button or START starts the game; SELECT plays the
 * movie. Counts every polled frame in IDLE_FRAMES. */
ADDRESS(0x80012f3c, 0x170)
s32 opening_poll_pad(s32 *prompt_mode, s32 *idle_frames)
{
    s32 choice = KF_OPENING_NO_CHOICE;
    u32 buttons = PadRead(1);

    if ((buttons & PADRup && !(pad_previous_buttons & PADRup))
        || (buttons & PADRright && !(pad_previous_buttons & PADRright))
        || (buttons & PADRdown && !(pad_previous_buttons & PADRdown))
        || (buttons & PADRleft && !(pad_previous_buttons & PADRleft))
        || (buttons & PADstart && !(pad_previous_buttons & PADstart))) {
        audio_play_voice(audio_vab_id, CHOICE_VOICE_PROGRAM, 0, CHOICE_VOICE_NOTE,
            CHOICE_VOICE_VOLUME, CHOICE_VOICE_VOLUME);
        choice = KF_OPENING_START_GAME;
    } else if (buttons & PADselect && !(pad_previous_buttons & PADselect)) {
        audio_play_voice(audio_vab_id, CHOICE_VOICE_PROGRAM, 0, CHOICE_VOICE_NOTE,
            CHOICE_VOICE_VOLUME, CHOICE_VOICE_VOLUME);
        choice = KF_OPENING_PLAY_MOVIE;
    }
    pad_previous_buttons = buttons;
    (*idle_frames)++;
    return choice;
}

ADDRESS(0x800130ac, 0x50)
void primitive_buffer_begin_poly_ft4(void)
{
    SetPolyFT4(current_poly_ft4);
    setRGB0(current_poly_ft4, PRIMITIVE_BRIGHTNESS, PRIMITIVE_BRIGHTNESS, PRIMITIVE_BRIGHTNESS);
}

/* Darkens the quad by display_fade_level, then links it at DEPTH. */
ADDRESS(0x800130fc, 0xcc)
void primitive_buffer_commit_poly_ft4(s32 depth)
{
    u8 level;

    if (display_fade_level != 0) {
        level = current_poly_ft4->r0;
        if (level < display_fade_level) {
            level = 0;
        } else {
            level -= display_fade_level;
        }
        setRGB0(current_poly_ft4, level, level, level);
    }
    AddPrim((void *)&display_current->ordering_table[depth], (void *)current_poly_ft4);
    current_poly_ft4++;
}
