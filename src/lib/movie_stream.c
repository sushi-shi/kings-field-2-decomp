#include <kf/lib/address.h>
#include <kf/lib/types.h>
#include <kf/lib/movie_stream.h>

/*
 * The tutorial's earlier revision as linked here: one decode buffer, 16-bit
 * output and a global DECENV. OPEN.EXE and END.EXE link the same bodies; only
 * the last frame differs.
 */

#define WAIT_TIME 0x800000
#define RING_SIZE 32
#define SCR_WIDTH 320
#define SCR_HEIGHT 240
#define SLICE_WIDTH_PIXELS 16
#define VLC_BUFFER_HEIGHT 256

/* STR sector header as read here: the Psy-Q 3.0 StHEADER stops after
 * `height`, but the code also compares the next two words (named headm and
 * headv by later SDK headers) with the first two words of the frame data. */
typedef struct {
    StHEADER header;
    u_long headm;
    u_long headv;
} StrSectorHeader;

DATA_AT("OPEN", 0x8003dec0, 0x4)
DATA_AT("END", 0x8003ace0, 0x4)
int Rewind_Switch = 0;

DATA_AT("OPEN", 0x8003dec8, 0x4)
DATA_AT("END", 0x8003ace8, 0x4)
long StrFrame = 0;

DATA_AT("OPEN", 0x8003e058, 0x28000)
DATA_AT("END", 0x8003ae50, 0x28000)
u_long vlcbuf0[SCR_WIDTH / 2 * VLC_BUFFER_HEIGHT];

DATA_AT("OPEN", 0x80066058, 0x28000)
DATA_AT("END", 0x80062e50, 0x28000)
u_long vlcbuf1[SCR_WIDTH / 2 * VLC_BUFFER_HEIGHT];

DATA_AT("OPEN", 0x8008e058, 0x3c00)
DATA_AT("END", 0x8008ae50, 0x3c00)
u_short imgbuf[7680];

DATA_AT("OPEN", 0x80091c58, 0x10000)
DATA_AT("END", 0x8008ea50, 0x10000)
u_long Ring_Buff[RING_SIZE * SECTOR_SIZE];

DATA_AT("OPEN", 0x800a1c58, 0x30)
DATA_AT("END", 0x8009ea50, 0x30)
DECENV dec;


ADDRESS_AT("OPEN", 0x8001383c, 0xbc)
ADDRESS_AT("END", 0x800121ec, 0xbc)
void strSetDefDecEnv(void)
{
    dec.vlcbuf[0] = vlcbuf0;
    dec.vlcbuf[1] = vlcbuf1;
    dec.vlcid = 0;
    dec.imgbuf = imgbuf;
    dec.rectid = 0;
    dec.isdone = 0;
    setRECT(&dec.rect[0], 0, 0, SCR_WIDTH, SCR_HEIGHT);
    setRECT(&dec.rect[1], 0, SCR_HEIGHT, SCR_WIDTH, SCR_HEIGHT);
    setRECT(&dec.slice, 0, 0, SLICE_WIDTH_PIXELS, SCR_HEIGHT);
}

ADDRESS_AT("OPEN", 0x800138f8, 0x74)
ADDRESS_AT("END", 0x800122a8, 0x74)
void strInit(CdlLOC *loc)
{
    DecDCTReset(0);
    Rewind_Switch = 0;
    DecDCToutCallback(strCallback);
    StSetRing(Ring_Buff, RING_SIZE);
    StSetStream(0, 1, 0x0fffffff, 0, 0);
    strKickCD(loc);
}

/* DecDCTout completion callback: store the decoded slice, then start the
 * next slice or finish the frame and flip the destination rectangle. */
ADDRESS_AT("OPEN", 0x8001396c, 0x10c)
ADDRESS_AT("END", 0x8001231c, 0x10c)
void strCallback(void)
{
    LoadImage(&dec.slice, (u_long *)dec.imgbuf);
    dec.slice.x += dec.slice.w;
    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
        DecDCTout((u_long *)dec.imgbuf, dec.slice.w * dec.slice.h / 2);
    } else {
        dec.isdone = 1;
        dec.rectid = dec.rectid ? 0 : 1;
        dec.slice.x = dec.rect[dec.rectid].x;
        dec.slice.y = dec.rect[dec.rectid].y;
    }
}

ADDRESS_AT("OPEN", 0x80013a78, 0x94)
ADDRESS_AT("END", 0x80012428, 0x94)
int strNextVlc(void)
{
    int cnt = WAIT_TIME;
    u_long *next;

    while ((next = strNext(&dec)) == 0) {
        if (--cnt == 0) {
            return -1;
        }
    }
    dec.vlcid = dec.vlcid ? 0 : 1;
    DecDCTvlc(next, dec.vlcbuf[dec.vlcid]);
    StFreeRing(next);
    return 0;
}

/* The environment argument is unused; the caller passes the global. */
ADDRESS_AT("OPEN", 0x80013b0c, 0xb0)
ADDRESS_AT("END", 0x800124bc, 0xb0)
u_long *strNext(DECENV *env)
{
    u_long *addr;
    StrSectorHeader *sector;
    int cnt = WAIT_TIME;

    while (StGetNext(&addr, (u_long **)&sector)) {
        if (--cnt == 0) {
            return 0;
        }
    }
    if (addr[0] != sector->headm || addr[1] != sector->headv) {
        StFreeRing(addr);
        return 0;
    }
    StrFrame = sector->header.frameCount;
    if (sector->header.frameCount >= MOVIE_END_FRAME) {
        Rewind_Switch = 1;
    }
    return addr;
}

ADDRESS_AT("OPEN", 0x80013bbc, 0x74)
ADDRESS_AT("END", 0x8001256c, 0x74)
void strSync(DECENV *env)
{
    u_long cnt = WAIT_TIME;

    while (env->isdone == 0) {
        if (--cnt == 0) {
            env->isdone = 1;
            env->rectid = env->rectid ? 0 : 1;
            env->slice.x = env->rect[env->rectid].x;
            env->slice.y = env->rect[env->rectid].y;
        }
    }
    env->isdone = 0;
}

ADDRESS_AT("OPEN", 0x80013c30, 0x48)
ADDRESS_AT("END", 0x800125e0, 0x48)
void strKickCD(CdlLOC *loc)
{
    while (CdSeekL(loc) == 0) {
    }
    while (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT | CdlModeSF) == 0) {
    }
}
