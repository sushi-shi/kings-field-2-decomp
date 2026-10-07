#include <kf/lib/display.h>

enum {
    DISPLAY_PROJECTION_DISTANCE = 320,
    DISPLAY_DITHERING_ENABLED = 1,
    DISPLAY_OUTPUT_ENABLED = 1
};

#ifdef KF_END
#define DISPLAY_WIDTH 320
#define DISPLAY_CLEARS_BACKGROUND 0
#define DISPLAY_BACKGROUND_LEVEL 255
#else
#define DISPLAY_WIDTH 640
#define DISPLAY_CLEARS_BACKGROUND 1
#define DISPLAY_BACKGROUND_LEVEL 0
#endif

KfDisplayBuffer *display_current;

KfDisplayBuffer display_buffers[2];

u8 display_primitives[2][KF_PRIMITIVE_BUFFER_BYTES];

void display_initialize(void)
{
    ResetGraph(KF_GPU_RESET_FULL);
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
