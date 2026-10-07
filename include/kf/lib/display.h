#ifndef KF_DISPLAY_H
#define KF_DISPLAY_H
#include <kf/lib/types.h>
#include <kf/lib/gpu.h>
#include <psyq/sdk.h>

/* Double-buffered display state shared by OPEN.EXE and END.EXE. */
enum {
    KF_ORDERING_TABLE_LENGTH = 1024,
    KF_DISPLAY_HEIGHT = 240,
    KF_PRIMITIVE_BUFFER_BYTES = 0x2000
};

typedef struct {
    DRAWENV draw;
    DISPENV disp;
    u_long ordering_table[KF_ORDERING_TABLE_LENGTH];
    POLY_FT4 *primitives;
} KfDisplayBuffer;

typedef char kf_display_buffer_size[sizeof(KfDisplayBuffer) == 0x1074 ? 1 : -1];

extern KfDisplayBuffer display_buffers[2];
extern KfDisplayBuffer *display_current;
extern u8 display_primitives[2][KF_PRIMITIVE_BUFFER_BYTES];
extern POLY_FT4 *current_poly_ft4;
/* OPEN.EXE only: darkening applied to committed quads; cleared per frame. */
extern u8 display_fade_level;

void display_initialize(void);
void display_begin_frame(void);
void display_present_frame(void);

#endif
