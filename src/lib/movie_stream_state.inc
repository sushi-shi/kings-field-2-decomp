/*
 * Private state of the streaming movie tutorial, as linked into each image's
 * movie unit. Retail keeps the two zero words in that unit's .sdata, the flag
 * and frame number in its .sbss and the buffers in fixed .bss, so all of them
 * are statics of one translation unit with the movie player. Include before
 * the player's functions; movie_stream.inc supplies the str* helpers after
 * them.
 */

#define WAIT_TIME 0x800000
#define RING_SIZE 32
#define SCR_WIDTH 320
#define SCR_HEIGHT 240
#define SLICE_WIDTH_PIXELS 16
#define VLC_BUFFER_HEIGHT 256

/* TUTO0.C's movie size; nothing in either retail image reads them. */
DATA_AT("OPEN", 0x8003dbbc, 0x4, ".sdata")
DATA_AT("END", 0x8003aa4c, 0x4, ".sdata")
static int StrWidth = 0;
DATA_AT("OPEN", 0x8003dbc0, 0x4, ".sdata")
DATA_AT("END", 0x8003aa50, 0x4, ".sdata")
static int StrHeight = 0;

DATA_AT("OPEN", 0x8003dec0, 0x4, ".sbss")
DATA_AT("END", 0x8003ace0, 0x4, ".sbss")
static KfBool Rewind_Switch;

DATA_AT("OPEN", 0x8003dec8, 0x4, ".sbss")
DATA_AT("END", 0x8003ace8, 0x4, ".sbss")
static long StrFrame;

DATA_AT("OPEN", 0x8003e058, 0x28000, ".bss")
DATA_AT("END", 0x8003ae50, 0x28000, ".bss")
static u_long vlcbuf0[SCR_WIDTH / 2 * VLC_BUFFER_HEIGHT];

DATA_AT("OPEN", 0x80066058, 0x28000, ".bss")
DATA_AT("END", 0x80062e50, 0x28000, ".bss")
static u_long vlcbuf1[SCR_WIDTH / 2 * VLC_BUFFER_HEIGHT];

DATA_AT("OPEN", 0x8008e058, 0x3c00, ".bss")
DATA_AT("END", 0x8008ae50, 0x3c00, ".bss")
static u_short imgbuf[7680];

DATA_AT("OPEN", 0x80091c58, 0x10000, ".bss")
DATA_AT("END", 0x8008ea50, 0x10000, ".bss")
static u_long Ring_Buff[RING_SIZE * SECTOR_SIZE];

DATA_AT("OPEN", 0x800a1c58, 0x30, ".bss")
DATA_AT("END", 0x8009ea50, 0x30, ".bss")
static DECENV dec;
