#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

ADDRESS(0x80020b50, 0x1d0)
void menu_blit_sprite_translucent(
    const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 0xff, 0xff, 0xff);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4,
        position->x - KF_MENU_TRANSLUCENT_SPRITE_OFFSET,
        position->y - KF_MENU_TRANSLUCENT_SPRITE_OFFSET,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v, sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}
