#include <kf/lib/display.h>

POLY_FT4 *current_poly_ft4;

void display_begin_frame(void)
{
    display_current = (display_current == &display_buffers[0]) ? &display_buffers[1] : &display_buffers[0];
    ClearOTag(display_current->ordering_table, KF_ORDERING_TABLE_LENGTH);
#ifdef KF_OPEN
    display_fade_level = 0;
#endif
    current_poly_ft4 = display_current->primitives;
}

void display_present_frame(void)
{
    DrawSync(0);
    VSync(0);
    PutDrawEnv(&display_current->draw);
    PutDispEnv(&display_current->disp);
    DrawOTag(display_current->ordering_table);
}
