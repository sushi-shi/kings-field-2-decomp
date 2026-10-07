#include <kf/lib/null.h>
#include <kf/lib/types.h>
#include <kf/lib/movie_stream.h>

#define WAIT_TIME 0x800000
#define RING_SIZE 32
#define SCR_WIDTH 320
#define SCR_HEIGHT 240
#define SLICE_WIDTH_PIXELS 16
#define VLC_BUFFER_HEIGHT 256

typedef struct {
    StHEADER header;
    u_long headm;
    u_long headv;
} StrSectorHeader;

KfBool Rewind_Switch;

long StrFrame;

u_long vlcbuf0[SCR_WIDTH / 2 * VLC_BUFFER_HEIGHT];

u_long vlcbuf1[SCR_WIDTH / 2 * VLC_BUFFER_HEIGHT];

u_short imgbuf[7680];

u_long Ring_Buff[RING_SIZE * SECTOR_SIZE];

DECENV dec;

void strSetDefDecEnv(void)
{
    dec.vlcbuf[0] = vlcbuf0;
    dec.vlcbuf[1] = vlcbuf1;
    dec.vlcid = 0;
    dec.imgbuf = imgbuf;
    dec.rectid = 0;
    dec.isdone = KF_FALSE;
    setRECT(&dec.rect[0], 0, 0, SCR_WIDTH, SCR_HEIGHT);
    setRECT(&dec.rect[1], 0, SCR_HEIGHT, SCR_WIDTH, SCR_HEIGHT);
    setRECT(&dec.slice, 0, 0, SLICE_WIDTH_PIXELS, SCR_HEIGHT);
}

void strInit(CdlLOC *loc)
{
    DecDCTReset(0);
    Rewind_Switch = KF_FALSE;
    DecDCToutCallback(strCallback);
    StSetRing(Ring_Buff, RING_SIZE);
    StSetStream(0, 1, 0x0fffffff, NULL, NULL);
    strKickCD(loc);
}

void strCallback(void)
{
    LoadImage(&dec.slice, (u_long *)dec.imgbuf);
    dec.slice.x += dec.slice.w;
    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
        DecDCTout((u_long *)dec.imgbuf, dec.slice.w * dec.slice.h / 2);
    } else {
        dec.isdone = KF_TRUE;
        dec.rectid = dec.rectid ? 0 : 1;
        dec.slice.x = dec.rect[dec.rectid].x;
        dec.slice.y = dec.rect[dec.rectid].y;
    }
}

int strNextVlc(void)
{
    int cnt = WAIT_TIME;
    u_long *next;

    while ((next = strNext(&dec)) == NULL) {
        if (--cnt == 0) {
            return -1;
        }
    }
    dec.vlcid = dec.vlcid ? 0 : 1;
    DecDCTvlc(next, dec.vlcbuf[dec.vlcid]);
    StFreeRing(next);
    return 0;
}

u_long *strNext(DECENV *env)
{
    u_long *addr;
    StrSectorHeader *sector;
    int cnt = WAIT_TIME;

    while (StGetNext(&addr, (u_long **)&sector)) {
        if (--cnt == 0) {
            return NULL;
        }
    }
    if (addr[0] != sector->headm || addr[1] != sector->headv) {
        StFreeRing(addr);
        return NULL;
    }
    StrFrame = sector->header.frameCount;
    if (sector->header.frameCount >= MOVIE_END_FRAME) {
        Rewind_Switch = KF_TRUE;
    }
    return addr;
}

void strSync(DECENV *env)
{
    u_long cnt = WAIT_TIME;

    while (!env->isdone) {
        if (--cnt == 0) {
            env->isdone = KF_TRUE;
            env->rectid = env->rectid ? 0 : 1;
            env->slice.x = env->rect[env->rectid].x;
            env->slice.y = env->rect[env->rectid].y;
        }
    }
    env->isdone = KF_FALSE;
}

void strKickCD(CdlLOC *loc)
{
    while (CdSeekL(loc) == 0) {
    }
    while (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT | CdlModeSF) == 0) {
    }
}
