#include <kf/lib/address.h>
#include <kf/game/menu.h>

ADDRESS(0x8001f798, 0x120)
void func_8001f798(KfMenuGlyphString *left, KfMenuGlyphString *right,
    const u8 *selected)
{
    s32 left_y = left->position.y;
    s32 right_y = right->position.y;
    s32 row;

    row = 0;
    do {
        if (*selected == 1) {
            menu_blit_sprite_fixed_clut(&menu_sprite_defs[2], &left->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[4], &left->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[3], &right->position);
        } else {
            menu_blit_sprite_fixed_clut(&menu_sprite_defs[2], &right->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[3], &left->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[4], &right->position);
        }
        menu_draw_string(&menu_sprite_defs[1], left);
        menu_draw_string(&menu_sprite_defs[1], right);
        left->position.y += 26;
        right->position.y += 26;
        selected++;
        row++;
    } while (row < 6);
    left->position.y = left_y;
    right->position.y = right_y;
}
