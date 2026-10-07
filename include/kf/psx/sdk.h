#ifndef KF_PSX_SDK_H
#define KF_PSX_SDK_H

// Port-owned declarations of the PlayStation library interface used by the
// original game sources. Layouts follow the documented PlayStation formats so
// static layout checks in the game headers still hold on ILP32 targets. These
// declarations are a transitional bring-up seam; the implementations live in
// src/psx and translate to the native platform, renderer and audio layers.
//
// This header is self-contained: it is included by freestanding game
// translation units and by hosted runtime translation units alike.

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------- geometry
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { u_char r, g, b, cd; } CVECTOR;
typedef struct { short vx, vy; } DVECTOR;
typedef struct {
    SVECTOR v;
    VECTOR sxyz;
    DVECTOR sxy;
    CVECTOR rgb;
    short txuv, pad;
    long chx, chy;
} EVECTOR;

#define ONE 4096

void InitGeom(void);
void SetGeomOffset(long ofx, long ofy);
void SetGeomScreen(long h);
void SetRotMatrix(MATRIX *m);
void SetTransMatrix(MATRIX *m);
void SetLightMatrix(MATRIX *m);
void SetColorMatrix(MATRIX *m);
void SetBackColor(long rbk, long gbk, long bbk);
void SetFarColor(long rfc, long gfc, long bfc);
void SetFogNear(long a, long h);
long RotTransPers(SVECTOR *v0, long *sxy, long *p, long *flag);
void RotTrans(SVECTOR *v0, VECTOR *v1, long *flag);
long NormalClip(long sxy0, long sxy1, long sxy2);
void DpqColor(CVECTOR *v0, long p, CVECTOR *v1);
void NormalColorCol(SVECTOR *v0, CVECTOR *v1, CVECTOR *v2);
void NormalColorCol3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3,
                     CVECTOR *v4, CVECTOR *v5, CVECTOR *v6);
void NormalColorDpq(SVECTOR *v0, CVECTOR *v1, long p, CVECTOR *v2);
void NormalColorDpq3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3,
                     long p, CVECTOR *v4, CVECTOR *v5, CVECTOR *v6);
MATRIX *ScaleMatrix(MATRIX *m, VECTOR *v);
MATRIX *MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *m2);
MATRIX *MulMatrix(MATRIX *m0, MATRIX *m1);
MATRIX *MulMatrix2(MATRIX *m0, MATRIX *m1);
MATRIX *RotMatrix(SVECTOR *r, MATRIX *m);
VECTOR *ApplyMatrix(MATRIX *m, SVECTOR *v0, VECTOR *v1);
void InitClip(EVECTOR *evbfad, long hw, long vw, long h, long near_z, long far_z);
long Clip3FTP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2,
              short *uv0, short *uv1, short *uv2, EVECTOR **evmx);
long Clip4FTP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3,
              short *uv0, short *uv1, short *uv2, short *uv3, EVECTOR **evmx);
int rsin(int a);
int rcos(int a);
int catan(int a);
long SquareRoot0(long a);
long SquareRoot12(long a);

// ----------------------------------------------------------------- graphics
typedef struct { short x, y, w, h; } RECT;
typedef struct { u_long tag; u_long code[15]; } DR_ENV;
typedef struct {
    RECT clip;
    short ofs[2];
    RECT tw;
    u_short tpage;
    u_char dtd, dfe, isbg, r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV;
typedef struct {
    RECT disp;
    RECT screen;
    u_char isinter, isrgb24, pad0, pad1;
} DISPENV;

// The low 24 tag bits link primitives. On the host they hold an ordering
// handle owned by the runtime, never a truncated pointer.
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
    u_char r0, g0, b0, code;
} P_TAG;

typedef struct { u_long tag; u_char r0, g0, b0, code; short x0, y0, x1, y1, x2, y2; } POLY_F3;
typedef struct { u_long tag; u_char r0, g0, b0, code; short x0, y0, x1, y1, x2, y2, x3, y3; } POLY_F4;
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0; u_char u0, v0; u_short clut;
    short x1, y1; u_char u1, v1; u_short tpage;
    short x2, y2; u_char u2, v2; u_short pad1;
} POLY_FT3;
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0; u_char u0, v0; u_short clut;
    short x1, y1; u_char u1, v1; u_short tpage;
    short x2, y2; u_char u2, v2; u_short pad1;
    short x3, y3; u_char u3, v3; u_short pad2;
} POLY_FT4;
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code; short x0, y0;
    u_char r1, g1, b1, pad1; short x1, y1;
    u_char r2, g2, b2, pad2; short x2, y2;
} POLY_G3;
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code; short x0, y0;
    u_char r1, g1, b1, pad1; short x1, y1;
    u_char r2, g2, b2, pad2; short x2, y2;
    u_char r3, g3, b3, pad3; short x3, y3;
} POLY_G4;
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code; short x0, y0; u_char u0, v0; u_short clut;
    u_char r1, g1, b1, p1; short x1, y1; u_char u1, v1; u_short tpage;
    u_char r2, g2, b2, p2; short x2, y2; u_char u2, v2; u_short pad2;
} POLY_GT3;
typedef struct {
    u_long tag;
    u_char r0, g0, b0, code; short x0, y0; u_char u0, v0; u_short clut;
    u_char r1, g1, b1, p1; short x1, y1; u_char u1, v1; u_short tpage;
    u_char r2, g2, b2, p2; short x2, y2; u_char u2, v2; u_short pad2;
    u_char r3, g3, b3, p3; short x3, y3; u_char u3, v3; u_short pad3;
} POLY_GT4;
typedef struct { u_long tag; u_char r0, g0, b0, code; short x0, y0; u_char u0, v0; u_short clut; short w, h; } SPRT;
typedef struct { u_long tag; u_char r0, g0, b0, code; short x0, y0; short w, h; } TILE;
typedef struct { u_long tag; u_long code[2]; } DR_MODE;

typedef struct {
    u_long mode;
    RECT *crect;
    u_long *caddr;
    RECT *prect;
    u_long *paddr;
} TIM_IMAGE;

#define setVector(v, _x, _y, _z) (v)->vx = (_x), (v)->vy = (_y), (v)->vz = (_z)
#define copyVector(v0, v1) (v0)->vx = (v1)->vx, (v0)->vy = (v1)->vy, (v0)->vz = (v1)->vz
#define addVector(v0, v1) (v0)->vx += (v1)->vx, (v0)->vy += (v1)->vy, (v0)->vz += (v1)->vz
#define setRECT(r, _x, _y, _w, _h) (r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h)

#define setlen(p, _len) (((P_TAG *)(p))->len = (u_char)(_len))
#define setcode(p, _code) (((P_TAG *)(p))->code = (u_char)(_code))
#define getlen(p) (u_char)(((P_TAG *)(p))->len)
#define getcode(p) (u_char)(((P_TAG *)(p))->code)

#define getTPage(tp, abr, x, y) \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6))
#define getClut(x, y) (((y) << 6) | (((x) >> 4) & 0x3f))
#define setTPage(p, tp, abr, x, y) ((p)->tpage = GetTPage(tp, abr, x, y))
#define setClut(p, x, y) ((p)->clut = GetClut(x, y))
#define setRGB0(p, _r0, _g0, _b0) (p)->r0 = (_r0), (p)->g0 = (_g0), (p)->b0 = (_b0)
#define setXYWH(p, _x0, _y0, _w, _h) \
    (p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x0) + (_w), (p)->y1 = (_y0), \
    (p)->x2 = (_x0), (p)->y2 = (_y0) + (_h), (p)->x3 = (_x0) + (_w), (p)->y3 = (_y0) + (_h)
#define setUVWH(p, _u0, _v0, _w, _h) \
    (p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u0) + (_w), (p)->v1 = (_v0), \
    (p)->u2 = (_u0), (p)->v2 = (_v0) + (_h), (p)->u3 = (_u0) + (_w), (p)->v3 = (_v0) + (_h)
#define setSemiTrans(p, abe) \
    ((abe) ? setcode(p, getcode(p) | 0x02) : setcode(p, getcode(p) & ~0x02))
#define setPolyF3(p) setlen(p, 4), setcode(p, 0x20)
#define setPolyFT3(p) setlen(p, 7), setcode(p, 0x24)
#define setPolyG3(p) setlen(p, 6), setcode(p, 0x30)
#define setPolyGT3(p) setlen(p, 9), setcode(p, 0x34)
#define setPolyF4(p) setlen(p, 5), setcode(p, 0x28)
#define setPolyFT4(p) setlen(p, 9), setcode(p, 0x2c)
#define setPolyG4(p) setlen(p, 8), setcode(p, 0x38)
#define setPolyGT4(p) setlen(p, 12), setcode(p, 0x3c)

int ResetGraph(int mode);
int SetGraphDebug(int level);
void SetDispMask(int mask);
int DrawSync(int mode);
DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
DRAWENV *PutDrawEnv(DRAWENV *env);
DISPENV *PutDispEnv(DISPENV *env);
u_long *ClearOTag(u_long *ot, int n);
u_long *ClearOTagR(u_long *ot, int n);
void AddPrim(void *ot, void *p);
void DrawOTag(u_long *p);
int ClearImage(RECT *rect, u_char r, u_char g, u_char b);
int LoadImage(RECT *rect, u_long *p);
int StoreImage(RECT *rect, u_long *p);
int MoveImage(RECT *rect, int x, int y);
int OpenTIM(u_long *addr);
TIM_IMAGE *ReadTIM(TIM_IMAGE *timimg);
u_short GetTPage(int tp, int abr, int x, int y);
u_short GetClut(int x, int y);
void SetPolyFT4(POLY_FT4 *p);
void SetSemiTrans(void *p, int abe);

// -------------------------------------------------------- vsync and pads
#define PADLup (1 << 12)
#define PADLdown (1 << 14)
#define PADLleft (1 << 15)
#define PADLright (1 << 13)
#define PADRup (1 << 4)
#define PADRdown (1 << 6)
#define PADRleft (1 << 7)
#define PADRright (1 << 5)
#define PADL1 (1 << 2)
#define PADL2 (1 << 0)
#define PADR1 (1 << 3)
#define PADR2 (1 << 1)
#define PADstart (1 << 11)
#define PADselect (1 << 8)

int VSync(int mode);
void ResetCallback(void);

// ------------------------------------------------------------------ CD-ROM
typedef struct { u_char minute, second, sector, track; } CdlLOC;
typedef struct {
    CdlLOC pos;
    u_long size;
    char name[16];
} CdlFILE;
typedef struct {
    u_short id;
    u_short type;
    u_short secCount;
    u_short nSectors;
    u_long frameCount;
    u_long frameSize;
    u_short width;
    u_short height;
} StHEADER;

#define CdlModeStream 0x100
#define CdlModeSpeed 0x80
#define CdlModeRT 0x40
#define CdlModeSize1 0x20
#define CdlModeSize0 0x10
#define CdlModeSF 0x08
#define CdlNop 0x01
#define CdlSetloc 0x02
#define CdlPlay 0x03
#define CdlReadN 0x06
#define CdlStandby 0x07
#define CdlStop 0x08
#define CdlPause 0x09
#define CdlSetmode 0x0e
#define CdlSeekL 0x15
#define CdlSeekP 0x16
#define CdlReadS 0x1b
#define CdlNoIntr 0x00
#define CdlDataReady 0x01
#define CdlComplete 0x02
#define CdlDataEnd 0x04
#define CdlDiskError 0x05

#define btoi(b) ((b) / 16 * 10 + (b) % 16)
#define itob(i) ((i) / 10 * 16 + (i) % 10)
#define CdSeekL(p) CdControl(CdlSeekL, (u_char *)(p), 0)
#define CdSeekP(p) CdControl(CdlSeekP, (u_char *)(p), 0)
#define CdPause() CdControl(CdlPause, 0, 0)
#define CdStop() CdControl(CdlStop, 0, 0)
#define SECTOR_SIZE 512
#define HEADER_SIZE 8

void CdInit(void);
CdlFILE *CdSearchFile(CdlFILE *fp, char *name);
int CdControl(u_char com, u_char *param, u_char *result);
int CdControlB(u_char com, u_char *param, u_char *result);
int CdReady(int mode, u_char *result);
int CdGetSector(void *madr, int size);
int CdRead(int sectors, u_long *buf, int mode);
int CdRead2(long mode);
int CdReadSync(int mode, u_char *result);
u_long CdReadyCallback(void (*func)());
int CdDataCallback(void (*func)());
void StSetRing(u_long *ring_addr, u_long ring_size);
void StSetStream(u_long mode, u_long start_frame, u_long end_frame,
                 void (*func1)(), void (*func2)());
u_long StFreeRing(u_long *base);
u_long StGetNext(u_long **addr, u_long **header);

// ---------------------------------------------------------- image decoder
void DecDCTReset(int mode);
void DecDCTvlc(u_long *bs, u_long *buf);
void DecDCTin(u_long *buf, int mode);
void DecDCTout(u_long *buf, int size);
int DecDCToutCallback(void (*func)());

// ------------------------------------------------------------------ sound
#define SSPLAY_INFINITY 0
#define SSPLAY_PAUSE 0
#define SSPLAY_PLAY 1
#define SS_TICK60 1
#define SS_WAIT_COMPLETED 1
#define SS_REV_TYPE_STUDIO_C 4
#define SS_REV_TYPE_HALL 5
#define SS_SEQ_TABSIZ 172

void SsInit(void);
void SsSetTickMode(long mode);
void SsSetTableSize(char *table, short s_max, short t_max);
void SsStart2(void);
void SsEnd(void);
short SsUtSetReverbType(short type);
void SsUtReverbOn(void);
void SsUtSetReverbDepth(short left, short right);
void SsSetMVol(short left, short right);
short SsSeqOpen(unsigned long *addr, short vab_id);
void SsSeqPlay(short seq_access_num, char play_mode, short l_count);
void SsSeqStop(short seq_access_num);
void SsSeqClose(short seq_access_num);
void SsSeqSetVol(short seq_access_num, short voll, short volr);
void SsSeqPause(short seq_access_num);
void SsSeqReplay(short seq_access_num);
void SsSeqCalledTbyT(void);
short SsVabOpenHead(unsigned char *addr, short vab_id);
short SsVabTransBody(unsigned char *addr, short vab_id);
short SsVabTransBodyPartly(unsigned char *addr, unsigned long bufsize, short vab_id);
short SsVabTransCompleted(short immediate_flag);
void SsVabClose(short vab_id);
short SsUtKeyOn(short vab_id, short prog, short tone, short note, short fine,
                short voll, short volr);
short SsUtKeyOff(short voice, short vab_id, short prog, short tone, short note);
void SpuGetAllKeysStatus(char *status);

// ------------------------------------------------------- kernel and BIOS
struct EXEC {
    unsigned long pc0;
    unsigned long gp0;
    unsigned long t_addr;
    unsigned long t_size;
    unsigned long d_addr;
    unsigned long d_size;
    unsigned long b_addr;
    unsigned long b_size;
    unsigned long s_addr;
    unsigned long s_size;
    unsigned long sp, fp, gp, ret, base;
};

struct DIRENTRY {
    char name[20];
    long attr;
    long size;
    struct DIRENTRY *next;
    long head;
    char system[4];
};

long OpenEvent(unsigned long descriptor, long spec, long mode, void (*handler)(void));
long EnableEvent(long event);
long DisableEvent(long event);
long TestEvent(long event);
long UnDeliverEvent(long event);
long CloseEvent(long event);
void EnterCriticalSection(void);
void ExitCriticalSection(void);
void InitHeap(void *head, long size);
void InitCARD(long pad_enable);
long StartCARD(void);
long StopCARD(void);
void ChangeClearPAD(long value);
void _bu_init(void);
long _card_info(long channel);
long _96_init(void);
long _96_remove(void);
long Load(const char *name, struct EXEC *header);
long Exec(struct EXEC *header, long argc, char **argv);
void SetMem(long megabytes);

// Reports, once, that the port skipped retail behavior it cannot run yet.
void kf_psx_note_unported(const char *what);

// PSX.EXE's overlay request byte (retail 0x800102f0), shared by every program.
extern u_char kf_psx_overlay_request;

u_long PadInit(long identifier);
#ifdef __cplusplus
u_long PadRead(long identifier = 0);
#else
u_long PadRead(long identifier);
#endif
void PadStop(void);

#ifdef __cplusplus
}
#endif

#endif // KF_PSX_SDK_H
