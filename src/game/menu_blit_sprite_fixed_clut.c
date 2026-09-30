#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

enum {
    KF_MENU_FIXED_CURSOR_COLOR = 0x38,
    KF_MENU_FIXED_CURSOR_CLUT = 0x7d24,
    KF_MENU_FIXED_CURSOR_LEFT_MARGIN = 7
};

ADDRESS(0x80020ef8, 0x1b4)
void menu_blit_sprite_fixed_clut(
    const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4,
        KF_MENU_FIXED_CURSOR_COLOR,
        KF_MENU_FIXED_CURSOR_COLOR,
        KF_MENU_FIXED_CURSOR_COLOR);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = KF_MENU_FIXED_CURSOR_CLUT;
    setXYWH(current_poly_ft4,
        position->x - (sprite->width + KF_MENU_FIXED_CURSOR_LEFT_MARGIN),
        position->y, sprite->width, sprite->height);
    setUVWH(current_poly_ft4,
        sprite->u, sprite->v, sprite->width, sprite->height);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}
