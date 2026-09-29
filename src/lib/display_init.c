#include <kf/lib/address.h>
#include <kf/lib/display.h>

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

ADDRESS_AT("OPEN", 0x800120c8, 0x13c)
ADDRESS_AT("END", 0x80011b40, 0x140)
void display_initialize(void)
{
    ResetGraph(0);
    SetGraphDebug(0);
    InitGeom();
    SetGeomOffset(DISPLAY_WIDTH / 2, KF_DISPLAY_HEIGHT / 2);
    SetGeomScreen(320);
    SetBackColor(DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL);
    SetDefDrawEnv(&display_buffers[0].draw, 0, 0, DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDrawEnv(&display_buffers[1].draw, 0, KF_DISPLAY_HEIGHT, DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&display_buffers[0].disp, 0, KF_DISPLAY_HEIGHT, DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&display_buffers[1].disp, 0, 0, DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    display_buffers[0].draw.dtd = display_buffers[1].draw.dtd = 1;
    display_buffers[0].draw.isbg = display_buffers[1].draw.isbg = DISPLAY_CLEARS_BACKGROUND;
    setRGB0(&display_buffers[0].draw,
        DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL);
    setRGB0(&display_buffers[1].draw,
        DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL, DISPLAY_BACKGROUND_LEVEL);
    SetDispMask(1);
    display_buffers[0].primitives = (POLY_FT4 *)display_primitives[0];
    display_buffers[1].primitives = (POLY_FT4 *)display_primitives[1];
}
