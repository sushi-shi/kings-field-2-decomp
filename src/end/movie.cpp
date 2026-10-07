#include <kf/lib/null.h>
#include <kf/lib/display.h>
#include <kf/end/ending.h>
#include <kf/lib/audio.h>
#include <kf/lib/movie_stream.h>
#include <psyq/libc.h>

enum {
    ENDING_FADE_FRAME = 1336,
    ENDING_LAST_FRAME = 2136,
    ENDING_MASTER_VOLUME = 127,
    ENDING_SEQUENCE_VOLUME = 64,
    ENDING_MDEC_16BIT_MODE = 2
};

void ending_play_movie(void)
{
    CdlFILE file;
    u_char mode;
    int volume = ENDING_SEQUENCE_VOLUME;

    SsSetMVol(ENDING_MASTER_VOLUME, ENDING_MASTER_VOLUME);
    SsSeqSetVol(audio_sequence_id, ENDING_SEQUENCE_VOLUME, ENDING_SEQUENCE_VOLUME);
    SsSeqPlay(audio_sequence_id, SSPLAY_PLAY, 1);
    if (CdSearchFile(&file, "\\OP\\ED.S;1") == NULL) {
        printf("\n__ file not found");
        return;
    }
    SsSetMVol(ENDING_MASTER_VOLUME, ENDING_MASTER_VOLUME);
    SsSeqSetVol(audio_sequence_id, ENDING_SEQUENCE_VOLUME, ENDING_SEQUENCE_VOLUME);
    SsSeqPlay(audio_sequence_id, SSPLAY_PLAY, SSPLAY_INFINITY);
    strSetDefDecEnv();
    strInit(&file.pos);
    strNextVlc();
    display_current = &display_buffers[0];
    do {
        display_begin_frame();
        DecDCTin(dec.vlcbuf[dec.vlcid], ENDING_MDEC_16BIT_MODE);
        DecDCTout((u_long *)dec.imgbuf, dec.slice.w * dec.slice.h / 2);
        strNextVlc();
        strSync(&dec);
        display_present_frame();
        if (Rewind_Switch == KF_TRUE) {
            break;
        }
        if (StrFrame >= ENDING_FADE_FRAME && volume > 0) {
            volume--;
            SsSeqSetVol(audio_sequence_id, volume, volume);
            if (volume == 0) {
                SsSeqStop(audio_sequence_id);
                SsSeqSetVol(audio_sequence_id, ENDING_SEQUENCE_VOLUME, ENDING_SEQUENCE_VOLUME);
                SsSeqPlay(audio_sequence_id, SSPLAY_PLAY, 1);
            }
        }
    } while (StrFrame < ENDING_LAST_FRAME);
    CdStop();
    for (;;) {
    }

    mode = CdlModeSpeed;
    CdControlB(CdlSetmode, &mode, NULL);
    DecDCToutCallback(NULL);
    CdDataCallback(NULL);
    CdReadyCallback(NULL);
    CdControlB(CdlPause, NULL, NULL);
    SetDefDrawEnv(&display_buffers[0].draw, 0, 0, 320, KF_DISPLAY_HEIGHT);
    SetDefDrawEnv(&display_buffers[1].draw, 0, KF_DISPLAY_HEIGHT, 320, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&display_buffers[0].disp, 0, KF_DISPLAY_HEIGHT, 320, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&display_buffers[1].disp, 0, 0, 320, KF_DISPLAY_HEIGHT);
    display_current = &display_buffers[0];
    SsSeqStop(audio_sequence_id);
}
