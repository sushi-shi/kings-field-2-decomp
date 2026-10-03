#include <kf/lib/address.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

/* The templates copied here use four, five, or six 16-bit glyph codes. */
static inline void menu_copy_prefix8(s16 *destination, const s16 *source)
{
    memcpy(destination, source, 8);
}

static inline void menu_copy_prefix10(s16 *destination, const s16 *source)
{
    memcpy(destination, source, 10);
}

static inline void menu_copy_prefix12(s16 *destination, const s16 *source)
{
    memcpy(destination, source, 12);
}

DATA(0x80064a00, 0xf0)
KfMenuLabelSuffix menu_header_labels[12] = {
    {{130, 131, 132, -1, 0, 0, 0, 0, 0, 0}},
    {{43, 4124, 42, -1, 0, 0, 0, 0, 0, 0}},
    {{137, 138, 139, -1, 0, 0, 0, 0, 0, 0}},
    {{122, 208, 139, -1, 0, 0, 0, 0, 0, 0}},
    {{133, 134, -1, 0, 0, 0, 0, 0, 0}},
    {{8217, 40, 40, 1, 4108, -1, 0, 0, 0, 0}},
    {{255, 255, 5, 45, 12, -1, 0, 0, 0, 0}},
    {{255, 255, 12, 44, 45, -1, 0, 0, 0, 0}},
    {{255, 8221, 1, 4110, 39, -1, 0, 0, 0, 0}},
    {{255, 255, 4111, 45, 7, -1, 0, 0, 0, 0}},
    {{255, 255, 255, 197, 198, -1, 0, 0, 0, 0}},
    {{4105, 45, 42, 4115, -1, 0, 0, 0, 0}},
};

ADDRESS(0x8001e94c, 0x6bc)
void menu_draw_player_status(void)
{
    KfMenuGlyphString heading;
    KfMenuGlyphString amount;
    s32 row_step = 23;

    heading.position.x = 168;
    heading.position.y = 30;
    menu_copy_prefix8(heading.glyphs.codes, menu_header_labels[0].codes);
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 77;
    amount.position.y = heading.position.y;
    menu_format_number(player_state.experience, 7, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.position.y += row_step;
    menu_copy_prefix8(heading.glyphs.codes, menu_header_labels[1].codes);
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.y += row_step;
    menu_format_number(player_state.level, 7, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.glyphs.codes[0] = 227;
    heading.glyphs.codes[1] = 229;
    heading.glyphs.codes[2] = -1;
    heading.position.y += row_step;
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 63;
    amount.position.y += row_step;
    menu_format_number(player_state.vitals.current_hp, 4, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);
    amount.glyphs.codes[0] = 20;
    amount.glyphs.codes[1] = -1;
    amount.position.x += 28;
    menu_draw_number(&menu_sprite_defs[0], &amount);
    amount.position.x += 7;
    menu_format_number(player_state.vitals.maximum_hp, 4, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.glyphs.codes[0] = 228;
    heading.position.y += row_step;
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 63;
    amount.position.y += row_step;
    menu_format_number(player_state.vitals.current_mp, 4, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);
    amount.glyphs.codes[0] = 20;
    amount.glyphs.codes[1] = -1;
    amount.position.x += 28;
    menu_draw_number(&menu_sprite_defs[0], &amount);
    amount.position.x += 7;
    menu_format_number(player_state.vitals.maximum_mp, 4, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.glyphs.codes[0] = 140;
    heading.glyphs.codes[1] = 139;
    heading.glyphs.codes[2] = -1;
    heading.position.y += row_step;
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 84;
    amount.position.y = heading.position.y;
    menu_format_number(player_state.physical_power, 6, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.glyphs.codes[0] = 120;
    heading.position.y += row_step;
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 84;
    amount.position.y = heading.position.y;
    menu_format_number(player_state.magic, 6, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.position.y += row_step;
    menu_copy_prefix8(heading.glyphs.codes, menu_header_labels[4].codes);
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 56;
    amount.position.y += row_step;
    if (player_state.unknown_60 != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[5].codes);
    else if (player_state.curse_strength != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[6].codes);
    else if (player_state.unknown_5e != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[7].codes);
    else if (player_state.unknown_54 != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[8].codes);
    else if (player_state.unknown_5a != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[9].codes);
    else
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[10].codes);
    menu_draw_string(&menu_sprite_defs[1], &amount);

    heading.position.y += row_step;
    menu_copy_prefix10(heading.glyphs.codes, menu_header_labels[11].codes);
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 77;
    amount.position.y += row_step;
    menu_format_number(player_state.gold, 7, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);
    menu_draw_nine_slice_panel(161, 14, 140, 205, 1, 2);
}

ADDRESS(0x8001f008, 0x790)
void menu_draw_combat_attributes(void)
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
