#include <kf/lib/address.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

/* The two column headings copy four 16-bit glyph codes each. */
static inline void menu_copy_prefix8(s16 *destination, const s16 *source)
{
    memcpy(destination, source, 8);
}

ADDRESS(0x8001f008, 0x790)
void func_8001f008(void)
{
    KfMenuGlyphString label;
    KfMenuGlyphString number;

    label.position.x = 24;
    label.position.y = 28;
    menu_copy_prefix8(label.glyphs.codes, menu_header_labels[2].codes);
    menu_draw_string(&menu_sprite_defs[1], &label);

    label.glyphs.codes[0] = 255;
    label.glyphs.codes[1] = 263;
    label.glyphs.codes[2] = 106;
    label.glyphs.codes[3] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.x = label.position.x + 84;
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[0], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 264;
    label.glyphs.codes[2] = 81;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[1], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 265;
    label.glyphs.codes[2] = 76;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[2], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 266;
    label.glyphs.codes[2] = 88;
    label.glyphs.codes[3] = 120;
    label.glyphs.codes[4] = 121;
    label.glyphs.codes[5] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[3], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 170;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[4], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 187;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[5], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 188;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[6], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 141;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[7], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.position.x = 168;
    label.position.y = 28;
    menu_copy_prefix8(label.glyphs.codes, menu_header_labels[3].codes);
    menu_draw_string(&menu_sprite_defs[1], &label);

    label.glyphs.codes[0] = 255;
    label.glyphs.codes[1] = 263;
    label.glyphs.codes[2] = 106;
    label.glyphs.codes[3] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.x = label.position.x + 84;
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[0], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 264;
    label.glyphs.codes[2] = 81;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[1], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 265;
    label.glyphs.codes[2] = 76;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[2], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 136;
    label.glyphs.codes[2] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[3], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 120;
    label.glyphs.codes[2] = 88;
    label.glyphs.codes[3] = 120;
    label.glyphs.codes[4] = 121;
    label.glyphs.codes[5] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[4], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 170;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[5], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 187;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[6], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 188;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[7], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 141;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[8], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    menu_draw_nine_slice_panel(17, 14, 140, 205, 1, 2);
    menu_draw_nine_slice_panel(161, 14, 140, 205, 1, 2);
}
