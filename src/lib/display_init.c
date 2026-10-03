#include <kf/lib/address.h>
#include <kf/lib/display.h>

enum {
    DISPLAY_PROJECTION_DISTANCE = 320,
    DISPLAY_DITHERING_ENABLED = 1,
    DISPLAY_OUTPUT_ENABLED = 1
};

/* OPEN clears a 640-pixel frame to black; END keeps its 320-pixel frame. */
#ifdef KF_END
#define DISPLAY_WIDTH 320
#define DISPLAY_CLEARS_BACKGROUND 0
#define DISPLAY_BACKGROUND_LEVEL 255
#else
#define DISPLAY_WIDTH 640
#define DISPLAY_CLEARS_BACKGROUND 1
#define DISPLAY_BACKGROUND_LEVEL 0
#endif

DATA_AT("OPEN", 0x800a5ad0, 0x4)
DATA_AT("END", 0x800a2898, 0x4)
KfDisplayBuffer *display_current;

DATA_AT("OPEN", 0x800a6438, 0x20e8)
DATA_AT("END", 0x800a31f8, 0x20e8)
KfDisplayBuffer display_buffers[2];

DATA_AT("OPEN", 0x800a8540, 0x4000)
DATA_AT("END", 0x800a52f8, 0x4000)
u8 display_primitives[2][KF_PRIMITIVE_BUFFER_BYTES];

ADDRESS_AT("OPEN", 0x800120c8, 0x13c)
ADDRESS_AT("END", 0x80011b40, 0x140)
void display_initialize(void)
{
    ResetGraph(0);
    SetGraphDebug(0);
    InitGeom();
    SetGeomOffset(DISPLAY_WIDTH / 2, KF_DISPLAY_HEIGHT / 2);
    SetGeomScreen(DISPLAY_PROJECTION_DISTANCE);
    SetBackColor(DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL);
    SetDefDrawEnv(&display_buffers[0].draw, 0, 0, DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDrawEnv(&display_buffers[1].draw, 0, KF_DISPLAY_HEIGHT, DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&display_buffers[0].disp, 0, KF_DISPLAY_HEIGHT, DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&display_buffers[1].disp, 0, 0, DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    display_buffers[0].draw.dtd = display_buffers[1].draw.dtd = DISPLAY_DITHERING_ENABLED;
    display_buffers[0].draw.isbg = display_buffers[1].draw.isbg = DISPLAY_CLEARS_BACKGROUND;
    setRGB0(&display_buffers[0].draw,
        DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL);
    setRGB0(&display_buffers[1].draw,
        DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL);
    SetDispMask(DISPLAY_OUTPUT_ENABLED);
    display_buffers[0].primitives = (POLY_FT4 *)display_primitives[0];
    display_buffers[1].primitives = (POLY_FT4 *)display_primitives[1];
}
