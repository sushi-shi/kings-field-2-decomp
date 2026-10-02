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

enum {
    KF_MENU_CURSOR_COLOR = 0x40,
    KF_MENU_CURSOR_LEFT_MARGIN = 8,
    KF_MENU_CURSOR_TEXTURE_PAGE_BASE = 500,
    KF_MENU_CURSOR_CLUT_BASE = 36
};

ADDRESS(0x80020d20, 0x1d8)
void menu_blit_sprite(const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    s32 frame = menu_cursor_animation_frame;

    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4,
        KF_MENU_CURSOR_COLOR, KF_MENU_CURSOR_COLOR, KF_MENU_CURSOR_COLOR);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = ((frame + KF_MENU_CURSOR_TEXTURE_PAGE_BASE) << 6)
        + KF_MENU_CURSOR_CLUT_BASE;
    setXYWH(current_poly_ft4,
        position->x - (sprite->width + KF_MENU_CURSOR_LEFT_MARGIN),
        position->y, sprite->width, sprite->height);
    setUVWH(current_poly_ft4,
        sprite->u + (frame << 4), sprite->v, sprite->width, sprite->height);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}

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
