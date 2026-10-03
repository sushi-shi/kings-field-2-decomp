#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/pad.h>
#include <psyq/libc.h>
#include <kf/game/graphics.h>
#include <kf/game/cd.h>
#include <kf/game/memory.h>
#include <kf/game/tmd.h>
#include <kf/game/audio.h>
#include <psyq/audio.h>
#include <psyq/sdk.h>
#include <kf/game/pool.h>

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

DATA(0x80063e80, 0xf0)
KfMenuSpriteDef menu_sprite_defs[KF_MENU_SPRITE_COUNT] = {
    {0x1d, 0x7f64, 240, 0, 7, 14},
    {0x1d, 0x7f64, 0, 0, 15, 15},
    {0x1c, 0x7ee4, 128, 128, 14, 14},
    {0x1e, 0x7fa4, 160, 0, 54, 24},
    {0x1e, 0x7fa4, 160, 32, 54, 24},
    {0x1e, 0x7fa4, 0, 0, 124, 24},
    {0x1e, 0x7fa4, 0, 32, 124, 24},
    {0x1e, 0x7fa4, 0, 64, 236, 6},
    {0x1e, 0x7fa4, 0, 72, 236, 14},
    {0x1e, 0x7fa4, 0, 104, 236, 6},
    {0x1e, 0x7fa4, 0, 88, 236, 14},
    {0x1e, 0x7fa4, 0, 120, 33, 33},
    {0x1e, 0x7fa4, 40, 120, 28, 33},
    {0x1e, 0x7fa4, 72, 120, 33, 33},
    {0x1e, 0x7fa4, 0, 160, 33, 28},
    {0x1e, 0x7fa4, 40, 160, 28, 28},
    {0x1e, 0x7fa4, 72, 160, 33, 28},
    {0x1e, 0x7fa4, 0, 192, 33, 33},
    {0x1e, 0x7fa4, 40, 192, 28, 33},
    {0x1e, 0x7fa4, 72, 192, 33, 33},
};

DATA(0x80063f70, 0x9a0)
KfMenuWindowLayout menu_window_layouts[KF_MENU_WINDOW_COUNT] = {
    {
        {{0, 0}, {{-1}}},
        {
            {{31, 19}, {{0, 1, 18, 32, 109, 114, 66, -1}}},
            {{31, 45}, {{120, 121, 109, 114, 66, -1}}},
            {{31, 71}, {{112, 113, -1}}},
            {{31, 97}, {{137, 138, 57, 122, 208, -1}}},
            {{31, 123}, {{117, 82, 106, -1}}},
            {{31, 149}, {{11, 12, 18, 32, -1}}},
            {{31, 175}, {{4, 8219, 11, 56, 39, -1}}},
            {{31, 201}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{11, 12, 18, 32, -1}}},
        {
            {{31, 45}, {{44, 45, 4115, -1}}},
            {{31, 71}, {{4104, 45, 32, 109, 99, 97, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
            {{17, 19}, {{13, 45, 4123, -1}}},
            {{17, 19}, {{44, 45, 4115, -1}}},
            {{17, 19}, {{4104, 45, 32, 109, 99, 97, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{4, 8219, 11, 56, 39, -1}}},
        {
            {{31, 45}, {{213, 214, 215, -1}}},
            {{31, 71}, {{215, 216, -1}}},
            {{31, 97}, {{133, 134, 217, 218, -1}}},
            {{31, 123}, {{219, 220, 217, 218, -1}}},
            {{31, 149}, {{0, 1, 18, 32, 217, 218, -1}}},
            {{31, 175}, {{238, 239, 213, 214, -1}}},
            {{31, 201}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{116, 66, 57, 115, 106, -1}}},
        {
            {{31, 45}, {{116, 66, -1}}},
            {{31, 71}, {{115, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{164, 206, -1}}},
        {
            {{31, 45}, {{116, 66, -1}}},
            {{31, 71}, {{256, 257, 109, 207, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{0, -1}}},
        {
            {{31, 45}, {{258, 259, -1}}},
            {{31, 71}, {{141, 142, 206, -1}}},
        },
    },
    {
        {{0, 0}, {{-1}}},
        {
            {{98, 94}, {{89, 4171, 97, 106, -1}}},
            {{98, 120}, {{44, 45, 4115, 76, 106, -1}}},
        },
    },
    {0},
};

enum {
    KF_MENU_UPLOAD_SECOND_BUFFER_Y = 240,
    KF_MENU_CURSOR_FRAME_COUNT = 8
};

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

DATA(0x80065c20, 0x5a0)
u16 menu_item_code_primary[6][120] = {
    {
        50000, 50000, 50000, 990, 50000, 2850, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 1830, 50000, 50000, 50000, 50000, 50000, 680, 2950,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 780,
        50000, 50000, 50000, 50000, 50000, 50000, 1200, 3400, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 320,
        50000, 50000, 23, 48, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
    {
        50000, 180, 640, 50000, 50000, 50000, 4300, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 200, 780, 50000,
        50000, 50000, 50000, 50000, 330, 950, 50000, 50000, 50000, 50000, 150, 830,
        50000, 50000, 50000, 50000, 50000, 270, 50000, 3900, 50000, 50000, 50000, 350,
        1800, 4000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 330,
        650, 17700, 20, 40, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
    {
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 180, 50000,
        2380, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 8250, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 6300, 50000, 50000, 50000,
        50000, 50000, 18, 33, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 4800, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
    {
        50000, 50000, 50000, 50000, 50000, 3250, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 2020, 50000, 50000, 50000, 50000, 50000, 50000, 3600,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 1000,
        50000, 19800, 50000, 50000, 50000, 50000, 50000, 3400, 50000, 50000, 50000, 50000,
        1600, 4200, 50000, 23800, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 28, 52, 2800, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        15000, 50000, 50000, 2800, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
    {
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 2, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 1, 50000, 50000,
    },
    {
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 8350, 1530,
        13500, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
};

DATA(0x800661c0, 0x4b0)
u16 menu_item_code_secondary[5][120] = {
    {
        70, 120, 520, 750, 2000, 2100, 2700, 0, 0, 13500, 12800, 13000,
        15000, 26000, 28500, 30000, 1750, 2800, 0, 0, 0, 130, 550, 2000,
        14800, 16500, 23500, 0, 130, 570, 18500, 14500, 25000, 0, 50, 500,
        1300, 18000, 14300, 28000, 0, 150, 880, 1750, 12750, 21000, 0, 210,
        1500, 3500, 13300, 20000, 0, 1880, 2500, 8230, 9100, 11000, 6200, 3350,
        0, 0, 0, 0, 0, 0, 0, 4700, 3700, 6280, 0, 250,
        600, 4830, 8, 13, 1600, 700, 700, 700, 700, 700, 700, 3280,
        1030, 6000, 3300, 1800, 40000, 36500, 8500, 8500, 8500, 8500, 8500, 13500,
        530, 25, 0, 4500, 6500, 9750, 100, 280, 2400, 11000, 6000, 900,
        1900, 18000, 0, 2400, 2400, 2400, 1800, 1800, 1800, 20, 130, 0,
    },
    {
        80, 130, 450, 800, 1900, 1900, 2100, 0, 0, 7300, 8500, 13600,
        14300, 24800, 24800, 25000, 1630, 3200, 0, 0, 0, 140, 600, 2000,
        17000, 16000, 20000, 0, 230, 480, 12700, 14800, 20000, 0, 80, 450,
        1430, 15100, 14300, 20000, 0, 180, 1000, 2850, 11800, 21000, 0, 210,
        1500, 3000, 12700, 19800, 0, 1430, 1300, 7850, 8770, 11200, 7300, 3280,
        0, 0, 0, 0, 0, 0, 0, 4800, 3300, 6300, 0, 250,
        580, 4300, 12, 16, 1800, 800, 800, 800, 800, 800, 800, 2200,
        950, 5800, 2800, 1800, 38000, 3200, 7000, 7000, 7000, 7000, 8270, 13500,
        550, 26, 0, 350, 5220, 8500, 200, 350, 2000, 8640, 5880, 950,
        1880, 18000, 0, 2400, 2400, 2400, 1800, 1800, 1800, 20, 130, 0,
    },
    {
        80, 140, 480, 550, 1450, 1300, 1730, 0, 0, 4700, 7000, 7800,
        8150, 8500, 8500, 8330, 1780, 3500, 0, 0, 0, 160, 580, 2150,
        9200, 6380, 7700, 0, 250, 600, 4500, 4850, 7600, 0, 100, 580,
        1500, 4000, 3000, 5500, 0, 200, 650, 2230, 1200, 4700, 0, 180,
        1230, 3550, 1250, 5100, 0, 1700, 500, 5200, 1380, 1500, 5500, 3600,
        0, 0, 0, 0, 0, 0, 0, 3200, 4500, 1200, 0, 250,
        580, 2450, 13, 25, 2000, 950, 950, 950, 950, 950, 950, 1100,
        370, 1500, 1850, 600, 1330, 5110, 5800, 5800, 5800, 5800, 5980, 4800,
        500, 25, 0, 10, 4830, 6150, 550, 500, 1800, 3500, 4300, 1020,
        2530, 8320, 0, 2250, 2250, 2250, 1700, 1700, 1700, 20, 130, 0,
    },
    {
        60, 150, 600, 880, 1600, 1780, 2550, 0, 0, 8500, 11000, 11500,
        16500, 22000, 25300, 28800, 1700, 2800, 0, 0, 0, 200, 480, 2000,
        12200, 14300, 19800, 0, 100, 350, 13800, 15000, 21000, 0, 50, 370,
        1150, 16200, 14300, 22000, 0, 150, 730, 1630, 12750, 17700, 0, 250,
        1340, 3300, 13300, 16000, 0, 1570, 2500, 9650, 9100, 12000, 6200, 3350,
        0, 0, 0, 0, 0, 0, 0, 4700, 5200, 7850, 0, 250,
        435, 5120, 5, 10, 1600, 530, 530, 530, 530, 530, 530, 8200,
        950, 6650, 3300, 1800, 36000, 3850, 8500, 8500, 8500, 8500, 8500, 22750,
        530, 25, 0, 5000, 6500, 10800, 100, 250, 2400, 11000, 6350, 850,
        1760, 18000, 0, 2400, 2400, 2400, 1800, 1800, 1800, 20, 130, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 5000, 6500, 11300, 760, 580, 3800, 16200, 7800, 1400,
        10500, 22900, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
};

DATA(0x8006d68c, 0x4)
s32 menu_item_model_allocation_pending = 0;

DATA(0x8006d690, 0x4)
s32 input_press_pending = 0;

DATA(0x8006d694, 0x4)
s32 menu_item_quantity = 1;

DATA(0x8006d698, 0x4)
s32 menu_cursor_animation_frame = 0;

DATA(0x8006d69c, 0x4)
s32 menu_cursor_animation_direction = 0;

DATA(0x8006d9e0, 0x4)
POLY_FT4 *current_poly_ft4;

DATA(0x8006d9e8, 0x1)
u8 menu_saved_music_enabled;

DATA(0x8006d9f0, 0x4)
u_long *menu_frame_upload_pixels;

DATA(0x8006d9f8, 0x8)
RECT menu_frame_upload_rect;

DATA(0x8006da00, 0x8)
SVECTOR menu_item_preview_translation = {0};

DATA(0x8006da08, 0x8)
SVECTOR menu_item_preview_rotation = {0};

DATA(0x8006da10, 0x4)
s32 menu_item_preview_rotation_step = 0;

DATA(0x8006dbe8, 0x18)
KfPrimitiveBuffer menu_saved_primitive_buffers[KF_DISPLAY_BUFFER_COUNT];

ADDRESS(0x8001ceb8, 0x178)
void menu_item_buy_sell_controller(s32 kind)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;

    menu_enter_display_state(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_window(3, 3, cursor, confirmed);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();
    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            menu_item_buy_controller(kind);
            break;
        case 1:
            menu_item_sell_controller(kind);
            break;
        }

        if (result != -99)
            break;
        cursor = menu_poll_choice_input(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(3, 3, cursor, confirmed);
            menu_present_frame();
        }
    }
    menu_exit_display_state(0);
}

ADDRESS(0x8001d030, 0x310)
void menu_item_buy_controller(s32 kind)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;
    u8 *counters = game_counter_bytes;

    if (counters[0x33] == 0)
        menu_item_mask_pages[3][51] = 1;
    else
        menu_item_mask_pages[3][51] = 0;
    count = menu_collect_masked_item_rows(menu_item_mask_pages[kind], rows, values, indices,
        0, 119);
    menu_fill_item_counts_and_prices(counters, values, codes, indices, 0, count, kind);
    menu_list_init(&menu.list, 3, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 3, 10, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (player_state.gold < (u32)((s32)menu.codes[menu.list.selected_index]
                    * menu_item_quantity)
                    || counters[selected_item] + menu_item_quantity >= 100) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 10);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        player_state.gold -= cost;
        counters[result] += (u8)menu_item_quantity;
    }
}

ADDRESS(0x8001d340, 0x74)
void menu_fill_item_counts_and_prices(const u8 *source, u8 *counts, u32 *prices,
    const u8 *indices, s32 first, s32 last, s32 group)
{
    const u16 *page;

    if (first < last) {
        const u16 (*pages)[120] = menu_item_code_primary;
        page = pages[group];
        do {
            *counts++ = source[*indices];
            *prices++ = page[*indices++];
            first++;
        } while (first < last);
    }
}

ADDRESS(0x8001d3b4, 0x2a0)
void menu_item_sell_controller(s32 kind)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[120];
    u8 values[120];
    u32 codes[120];
    u8 indices[120];
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 payment;
    u8 selected_item;

    count = menu_collect_available_item_rows(game_counter_bytes, rows, values, indices, 0, 119);
    menu_fill_item_prices(codes, indices, 0, count, kind);
    menu_list_init(&menu.list, 3, 1);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu.list.entry_count != 0
            && menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 4, 11, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (values[menu.list.selected_index] < menu_item_quantity) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 11);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        payment = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        player_state.gold += payment;
        game_counter_bytes[result] -= (u8)menu_item_quantity;
    }
}

ADDRESS(0x8001d654, 0x54)
void menu_fill_item_prices(u32 *prices, const u8 *indices, s32 first, s32 last, s32 group)
{
    const u16 *page;

    if (first < last) {
        const u16 (*pages)[120] = menu_item_code_secondary;
        page = pages[group];
        do {
            *prices++ = page[*indices++];
            first++;
        } while (first < last);
    }
}

ADDRESS(0x8001d6a8, 0x228)
s32 menu_choose_inventory_item(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[120];
    u8 values[120];
    u8 indices[120];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    u8 selected_item;
    s32 frame;

    menu_enter_display_state(1);
    count = menu_collect_masked_item_rows(game_counter_bytes, rows, values, indices, 0, 119);
    menu_list_init(&menu.list, 5, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = 12;
    selected_item = indices[menu.list.selected_index];
    if (menu_load_item_model(selected_item) != 0)
        return -1;

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        if (menu.list.entry_count != 0)
            menu_update_item_preview((u8)selected_item);
        menu_render_list(&menu, 12);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 8, 12, (u8)selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &mode, &result);
        selected_item = indices[menu.list.selected_index];
        if (mode == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview((u8)selected_item);
            menu_render_list(&menu, 12);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    menu_exit_display_state(0);
    return result;
}
ADDRESS(0x8001d8d0, 0x394)
void menu_item_trade_controller(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    u8 *counters = game_counter_bytes;
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;

    menu_enter_display_state(1);
    count = menu_collect_masked_item_rows(menu_item_mask_pages[4], rows, values, indices,
        0, 119);
    menu_fill_item_counts_and_prices(counters, values, codes, indices, 0, count, 4);
    menu_list_init(&menu.list, 5, 1);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    selected_item = indices[menu.list.selected_index];
    if (menu_load_item_model(selected_item) != 0)
        return;

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        if (menu.list.entry_count != 0)
            menu_update_item_preview(selected_item);
        menu_render_list(&menu, 15);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 9, 15, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
            if (counters[96] < cost
                    || (selected_item == 117
                        ? counters[117] + menu_item_quantity * 10 >= 100
                        : counters[selected_item] + menu_item_quantity >= 100)) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 15);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        counters[96] -= cost;
        if (result == 117)
            counters[117] += menu_item_quantity * 10;
        else
            counters[result] += (u8)menu_item_quantity;
    }
    menu_exit_display_state(0);
}
ADDRESS(0x8001dc64, 0x16c)
void menu_item_stock_choice_controller(void)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;

    menu_enter_display_state(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_window(4, 3, cursor, confirmed);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();
    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            menu_buy_masked_stock_items();
            break;
        case 1:
            menu_buy_owned_items();
            break;
        }

        if (result != -99)
            break;
        cursor = menu_poll_choice_input(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(4, 3, cursor, confirmed);
            menu_present_frame();
        }
    }
    menu_exit_display_state(0);
}

ADDRESS(0x8001ddd0, 0x2d8)
void menu_buy_masked_stock_items(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    u8 *counters = game_counter_bytes;
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;

    count = menu_collect_masked_item_rows(menu_item_mask_pages[5], rows, values, indices,
        0, 119);
    menu_fill_item_counts_and_prices(counters, values, codes, indices, 0, count, 5);
    menu_list_init(&menu.list, 4, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 3, 13, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (player_state.gold < (u32)((s32)menu.codes[menu.list.selected_index]
                    * menu_item_quantity)
                    || counters[selected_item] + menu_item_quantity >= 100) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 13);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        player_state.gold -= cost;
        counters[result] += (u8)menu_item_quantity;
    }
}

ADDRESS(0x8001e0a8, 0x2d0)
void menu_buy_owned_items(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    u8 *counters = game_counter_bytes;
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;

    count = menu_collect_masked_item_rows(counters, rows, values, indices, 99, 109);
    menu_fill_item_prices(codes, indices, 0, count, 4);
    menu_list_init(&menu.list, 4, 1);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 10, 14, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (player_state.gold < (u32)((s32)menu.codes[menu.list.selected_index]
                    * menu_item_quantity)
                    || counters[selected_item] + menu_item_quantity >= 100) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 14);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        player_state.gold -= cost;
        counters[result] += (u8)menu_item_quantity;
    }
}
ADDRESS(0x8001e378, 0x10c)
s32 menu_poll_choice_input(s32 index, s32 last, s32 *selection, s32 *confirmed,
    s32 *cancelled)
{
    u32 buttons;

    *selection = -1;
    *confirmed = 0;
    input_wait_brief_release();
    buttons = input_read_mark_active();
    if (buttons & PADLup) {
        menu_cursor_animation_direction = 0;
        menu_play_sound_cue(16);
        if (index != 0)
            index--;
        else
            index = last;
    } else if (buttons & PADLdown) {
        menu_cursor_animation_direction = 0;
        menu_play_sound_cue(16);
        if (index != last)
            index++;
        else
            index = 0;
    } else if (buttons & PADRright) {
        menu_play_sound_cue(17);
        *confirmed = 1;
        if (index < last)
            *selection = index;
        else
            *cancelled = -1;
    } else if (buttons & PADRdown) {
        menu_play_sound_cue(18);
        *cancelled = -1;
    }
    return index;
}

ADDRESS(0x8001e484, 0x4c8)
u32 menu_update_list_input(KfMenuList *list, const u8 *item_ids,
    s32 *selection, s32 *result)
{
    u32 buttons;

    *selection = 0;
    input_wait_brief_release();
    buttons = input_read_mark_active();

    if (list->entry_count == 0) {
        if (buttons != 0) {
            menu_play_sound_cue(18);
            *result = -1;
        }
    } else if (buttons & PADLup) {
        menu_play_sound_cue(16);
        if (list->selected_index != 0) {
            list->selected_index--;
            if (list->cursor_row == 0)
                list->scroll_offset--;
            else
                list->cursor_row--;
        } else {
            list->selected_index = list->entry_count - 1;
            if (list->entry_count < list->visible_rows) {
                list->scroll_offset = 0;
                list->cursor_row = list->entry_count - 1;
            } else {
                list->scroll_offset = list->entry_count - list->visible_rows;
                list->cursor_row = list->visible_rows - 1;
            }
        }
        if (item_ids != 0 && menu_load_item_model(item_ids[list->selected_index]) != 0)
            *result = -1;
    } else if (buttons & PADLdown) {
        menu_play_sound_cue(16);
        if (list->selected_index < list->entry_count - 1) {
            list->selected_index++;
            if (list->cursor_row == list->visible_rows - 1)
                list->scroll_offset++;
            else
                list->cursor_row++;
        } else {
            list->selected_index = 0;
            list->scroll_offset = 0;
            list->cursor_row = 0;
        }
        if (item_ids != 0 && menu_load_item_model(item_ids[list->selected_index]) != 0)
            *result = -1;
    } else if (buttons & PADLright) {
        if (menu_item_quantity < 99) {
            menu_play_sound_cue(16);
            menu_item_quantity++;
        }
    } else if (buttons & PADLleft) {
        if (menu_item_quantity > 1) {
            menu_play_sound_cue(16);
            menu_item_quantity--;
        }
    } else if (buttons & PADRright) {
        *selection = 1;
    } else if (buttons & PADRdown) {
        menu_play_sound_cue(18);
        *result = -1;
    }

    if (buttons & PADselect) {
        if (buttons & PADR1) {
            input_press_pending = 0;
            menu_item_preview_rotation.vx += 16;
        }
        if (buttons & PADR2) {
            input_press_pending = 0;
            menu_item_preview_rotation.vx -= 16;
        }
        if (buttons & PADL1) {
            input_press_pending = 0;
            menu_item_preview_rotation.vz += 16;
        }
        if (buttons & PADL2) {
            input_press_pending = 0;
            menu_item_preview_rotation.vz -= 16;
        }
        if (buttons & PADRup) {
            input_press_pending = 0;
            menu_item_preview_rotation_step++;
        }
        if (buttons & PADRleft) {
            input_press_pending = 0;
            menu_item_preview_rotation_step--;
        }
    }

    if (buttons & PADstart) {
        if (buttons & PADR1) {
            input_press_pending = 0;
            menu_item_preview_translation.vx += 16;
        }
        if (buttons & PADR2) {
            input_press_pending = 0;
            menu_item_preview_translation.vx -= 16;
        }
        if (buttons & PADL1) {
            input_press_pending = 0;
            menu_item_preview_translation.vy += 16;
        }
        if (buttons & PADL2) {
            input_press_pending = 0;
            menu_item_preview_translation.vy -= 16;
        }
        if (buttons & PADRup) {
            input_press_pending = 0;
            menu_item_preview_translation.vz += 16;
        }
        if (buttons & PADRleft) {
            if (menu_item_preview_translation.vz > 500)
                menu_item_preview_translation.vz -= 16;
            input_press_pending = 0;
        }
    }

    return buttons;
}

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
    if (player_state.paralysis_timer != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[5].codes);
    else if (player_state.curse_strength != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[6].codes);
    else if (player_state.slow_timer != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[7].codes);
    else if (player_state.poison_timer != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[8].codes);
    else if (player_state.darkness_phase != 0)
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

ADDRESS(0x8001f798, 0x120)
void menu_draw_options_rows(KfMenuGlyphString *left, KfMenuGlyphString *right,
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

ADDRESS(0x8001f8b8, 0x2d4)
s32 menu_preview_choice(void *list_state, s32 label_kind,
    s32 render_mode, s32 item_id)
{
    KfMenuGlyphString labels[2];
    s32 choice = 0;
    s32 result = -99;
    s32 confirmed;
    s32 frame;
    u32 buttons;

    labels[0].position.x = menu_window_layouts[1].rows[0].position.x;
    labels[0].position.y = menu_window_layouts[1].rows[0].position.y;
    labels[1].position.x = menu_window_layouts[1].rows[1].position.x;
    labels[1].position.y = menu_window_layouts[1].rows[1].position.y;

    if (label_kind == 0) {
        labels[0].glyphs.codes[0] = 114;
        labels[0].glyphs.codes[1] = 66;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 1) {
        labels[0].glyphs.codes[0] = 117;
        labels[0].glyphs.codes[1] = 82;
        labels[0].glyphs.codes[2] = 106;
        labels[0].glyphs.codes[3] = -1;
    } else if (label_kind == 2) {
        labels[0].glyphs.codes[0] = 89;
        labels[0].glyphs.codes[1] = 65;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 3) {
        labels[0].glyphs.codes[0] = 116;
        labels[0].glyphs.codes[1] = 66;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 4) {
        labels[0].glyphs.codes[0] = 115;
        labels[0].glyphs.codes[1] = 106;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 5) {
        labels[0].glyphs.codes[0] = 112;
        labels[0].glyphs.codes[1] = 113;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 6) {
        labels[0].glyphs.codes[0] = 44;
        labels[0].glyphs.codes[1] = 45;
        labels[0].glyphs.codes[2] = 0x1013;
        labels[0].glyphs.codes[3] = -1;
    } else if (label_kind == 7) {
        labels[0].glyphs.codes[0] = 13;
        labels[0].glyphs.codes[1] = 45;
        labels[0].glyphs.codes[2] = 0x101b;
        labels[0].glyphs.codes[3] = -1;
    } else if (label_kind == 8) {
        labels[0].glyphs.codes[0] = 0x104;
        labels[0].glyphs.codes[1] = 71;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 9) {
        labels[0].glyphs.codes[0] = 0x105;
        labels[0].glyphs.codes[1] = 0x106;
        labels[0].glyphs.codes[2] = -1;
    } else {
        labels[0].glyphs.codes[0] = 207;
        labels[0].glyphs.codes[1] = 106;
        labels[0].glyphs.codes[2] = -1;
    }

    if (label_kind == 2) {
        labels[1].glyphs.codes[0] = 65;
        labels[1].glyphs.codes[1] = 65;
        labels[1].glyphs.codes[2] = 67;
        goto labels_ready;
    finished:
        input_wait_release();
        return result;
    } else {
        labels[1].glyphs.codes[0] = 99;
        labels[1].glyphs.codes[1] = 97;
        labels[1].glyphs.codes[2] = 106;
    }
labels_ready:
    labels[1].glyphs.codes[3] = -1;

    for (;;) {
        if (result != -99) {
            goto finished;
        }
        input_wait_brief_release();
        buttons = input_read_mark_active();
        confirmed = 0;
        if ((buttons & PADLup) || (buttons & PADLdown)) {
            menu_cursor_animation_direction = 0;
            menu_play_sound_cue(16);
            if (choice != 0)
                choice = 0;
            else
                choice = 1;
        } else if (buttons & PADRright) {
            menu_play_sound_cue(17);
            confirmed = 1;
            result = -choice;
        } else if (buttons & PADRdown) {
            menu_play_sound_cue(18);
            result = -1;
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_update_item_preview((u8)item_id);
            menu_render_list(list_state, render_mode);
            menu_draw_two_option(&labels[0], &labels[1], choice, confirmed);
            menu_present_frame();
        }
    }
}

ADDRESS(0x8001fb8c, 0x108)
void menu_draw_window(s32 window_kind, s32 count, s32 highlight, s32 confirmation)
{
    const KfMenuWindowLayout *layout = &menu_window_layouts[window_kind];
    const KfMenuGlyphString *row = &layout->rows[0];
    s32 index;

    if (layout->title.position.x != 0) {
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &layout->title.position);
        menu_draw_string(&menu_sprite_defs[1], &layout->title);
    }
    if (count > 0) {
        index = 0;
        do {
            if (index == highlight && confirmation == KF_MENU_CONFIRM_REQUESTED)
                menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_CONFIRMED_ROW], &row->position);
            else
                menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &row->position);
            if (index == highlight)
                menu_blit_sprite(&menu_sprite_defs[2], &row->position);
            menu_draw_string(&menu_sprite_defs[1], row);
            row++;
            index++;
        } while (index < count);
    }
}
ADDRESS(0x8001fc94, 0xab4)
void menu_render_list(const void *list_state, s32 render_mode)
{
    const KfMenuRenderList *view = list_state;
    const KfMenuList *list = &view->list;
    const KfMenuSpriteDef *sprite;
    const s16 *row_glyphs = view->row_glyphs;
    const s16 *detail_codes = (const s16 *)view->detail_rows;
    const s32 *number_values = view->number_values;
    const u8 *byte_values = view->byte_values;
    KfMenuGlyphString text;
    s16 *out_codes;
    s32 row;
    s32 code;
    s32 value;
    s32 y;

    if (list->title.position.x != 0) {
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &list->title.position);
        menu_draw_string(&menu_sprite_defs[1], &list->title);
    }

    detail_codes += list->scroll_offset * 10;
    row_glyphs += list->scroll_offset * list->glyphs_per_entry;
    number_values += list->scroll_offset;
    byte_values += list->scroll_offset;

    {
        for (row = 0; row < list->visible_rows && row < list->entry_count; row++) {
            text.position.x = list->list_x + 5;
            text.position.y = list->list_y + 5 + row * 14;
            out_codes = text.glyphs.codes;
            for (code = 0; code < list->glyphs_per_entry; code++)
                *out_codes++ = *row_glyphs++;
            menu_draw_string(&menu_sprite_defs[1], &text);

            if (render_mode == 3) {
                text.position.x += 84;
                out_codes = text.glyphs.codes;
                for (code = 0; code < 10; code++)
                    *out_codes++ = *detail_codes++;
                *out_codes = -1;
                menu_draw_string(&menu_sprite_defs[1], &text);
            }

            if (((u32)render_mode - 10u) < 6u && render_mode != 12) {
                value = *number_values;
                text.position.x += 140;
                if (render_mode == 15)
                    menu_format_number(value, 6, 0, 6, text.glyphs.codes);
                else
                    menu_format_number(value, 6, 0, 2, text.glyphs.codes);
                menu_draw_number(&menu_sprite_defs[0], &text);
                number_values++;
            }

            if (render_mode == 2 || render_mode == 4) {
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 189;
                    menu_format_number(value, 3, 0, 3, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }

            if (((u32)render_mode - 8u) < 2u) {
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 98;
                    menu_format_number(value, 7, 0, 4, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x += 91;
                    menu_format_number(value, 3, 0, 5, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }

            if (render_mode != 2 && render_mode != 3 && render_mode != 4
                && ((u32)render_mode - 8u) >= 2u && render_mode != 16) {
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x = list->list_x + 208;
                    menu_format_number(value, 2, 0, 1, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }
            if (render_mode == 16) {
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x += 203;
                    menu_format_number(value, 2, 0, 1, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 189;
                    menu_format_number(value, 3, 0, 3, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }
        }
    }

    sprite = &menu_sprite_defs[KF_MENU_SPRITE_LIST_TOP];
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 255, 255, 255);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4, list->list_x, list->list_y,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v,
        sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);

    row = 0;
    if (row < list->visible_rows) {
        y = 0;
        do {
            sprite = row == list->cursor_row
                ? &menu_sprite_defs[KF_MENU_SPRITE_LIST_SELECTED_ROW]
                : &menu_sprite_defs[KF_MENU_SPRITE_LIST_ROW];
            row++;
            primitive_buffer_begin_poly_ft4();
            setRGB0(current_poly_ft4, 255, 255, 255);
            current_poly_ft4->tpage = sprite->tpage;
            current_poly_ft4->clut = sprite->clut;
            setXYWH(current_poly_ft4, list->list_x, list->list_y + y + 5,
                sprite->width, sprite->height);
            setUVWH(current_poly_ft4, sprite->u, sprite->v,
                sprite->width, sprite->height);
            SetSemiTrans((void *)current_poly_ft4, 1);
            primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
            y += 14;
        } while (row < list->visible_rows);
    }

    sprite = &menu_sprite_defs[KF_MENU_SPRITE_LIST_BOTTOM];
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 255, 255, 255);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4, list->list_x,
        list->list_y + list->visible_rows * 14 + 5,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v,
        sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);

    if (((u32)render_mode - 10u) < 2u || ((u32)render_mode - 13u) < 2u)
        menu_draw_status_counters(1);
    else if (render_mode == 15)
        menu_draw_status_counters(3);
    if (((u32)render_mode - 8u) < 2u)
        menu_render_list_mode_8_9_noop();
}
enum { KF_PREVIEW_ANGLE_MASK = 0xfff };

ADDRESS(0x80020748, 0xf4)
void menu_draw_two_option(const KfMenuGlyphString *accept_label,
    const KfMenuGlyphString *decline_label, s32 selected_choice, s32 confirmation)
{
    if (selected_choice == KF_MENU_CHOICE_ACCEPT) {
        menu_blit_sprite(&menu_sprite_defs[KF_MENU_SPRITE_SELECTION_CURSOR],
            &accept_label->position);
    } else {
        menu_blit_sprite(&menu_sprite_defs[KF_MENU_SPRITE_SELECTION_CURSOR],
            &decline_label->position);
    }
    if (confirmation == KF_MENU_CONFIRM_REQUESTED) {
        if (selected_choice == KF_MENU_CHOICE_ACCEPT) {
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_HIGHLIGHT],
                &accept_label->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
                &decline_label->position);
        } else {
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
                &accept_label->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_HIGHLIGHT],
                &decline_label->position);
        }
    } else {
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
            &accept_label->position);
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
            &decline_label->position);
    }
    menu_draw_string(&menu_sprite_defs[KF_MENU_SPRITE_GLYPH_ATLAS], accept_label);
    menu_draw_string(&menu_sprite_defs[KF_MENU_SPRITE_GLYPH_ATLAS], decline_label);
}

ADDRESS(0x8002083c, 0x154)
void menu_update_item_preview(s32 item_id)
{
    MATRIX rotation;
    MATRIX light;
    MATRIX lit;
    MATRIX color;

    if (player_state.item_preview_enabled == 0 || (u8)item_id == 0xff) {
        return;
    }

    rotation.t[0] = menu_item_preview_translation.vx;
    rotation.t[1] = menu_item_preview_translation.vy;
    rotation.t[2] = menu_item_preview_translation.vz;
    menu_item_preview_rotation.vy += menu_item_preview_rotation_step;
    menu_item_preview_rotation.vx &= KF_PREVIEW_ANGLE_MASK;
    menu_item_preview_rotation.vy &= KF_PREVIEW_ANGLE_MASK;
    menu_item_preview_rotation.vz &= KF_PREVIEW_ANGLE_MASK;
    RotMatrix(&menu_item_preview_rotation, &rotation);

    light.m[0][0] = 0x800;
    light.m[0][1] = 0x800;
    light.m[0][2] = -0x800;
    light.m[1][0] = 0;
    light.m[1][1] = 0;
    light.m[1][2] = -0x800;
    light.m[2][0] = 0x1000;
    light.m[2][1] = 0;
    light.m[2][2] = -0x800;
    MulMatrix0(&light, &rotation, &lit);
    SetLightMatrix(&lit);

    color.m[0][0] = 0x1000;
    color.m[0][1] = 0x1000;
    color.m[0][2] = 0x1000;
    color.m[1][0] = 0x1000;
    color.m[1][1] = 0x1000;
    color.m[1][2] = 0x1000;
    color.m[2][0] = 0x1000;
    color.m[2][1] = 0x1000;
    color.m[2][2] = 0x1000;
    SetColorMatrix(&color);
    SetRotMatrix(&rotation);
    SetTransMatrix(&rotation);
    menu_render_item_model();
    current_poly_ft4 = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
}

ADDRESS(0x80020990, 0x1c0)
void menu_draw_status_counters(s32 kind)
{
    KfMenuGlyphString heading;
    KfMenuGlyphString amount;

    heading.position.x = 183;
    heading.position.y = 33;
    if (kind == 3) {
        heading.glyphs.codes[0] = 7;
        heading.glyphs.codes[1] = 41;
        heading.glyphs.codes[2] = 12;
        heading.glyphs.codes[3] = 15;
        heading.glyphs.codes[4] = 42;
        heading.glyphs.codes[5] = KF_MENU_TEXT_END;
    } else {
        heading.glyphs.codes[0] = 221;
        heading.glyphs.codes[1] = 222;
        heading.glyphs.codes[2] = 209;
        heading.glyphs.codes[3] = KF_MENU_TEXT_END;
    }
    menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &heading.position);
    menu_draw_string(&menu_sprite_defs[1], &heading);

    amount.position.x = heading.position.x + 56;
    amount.position.y = heading.position.y;
    if (kind == 3)
        menu_format_number(game_counter_bytes[0x60], 7, 0, 6, amount.glyphs.codes);
    else
        menu_format_number(player_state.gold, 7, 0, 2, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    if (kind != 2) {
        heading.position.x = 183;
        heading.position.y = 61;
        heading.glyphs.codes[0] = 202;
        heading.glyphs.codes[1] = 203;
        heading.glyphs.codes[2] = KF_MENU_TEXT_END;
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &heading.position);
        menu_draw_string(&menu_sprite_defs[1], &heading);
        amount.position.x = heading.position.x + 91;
        amount.position.y = heading.position.y;
        menu_format_number(menu_item_quantity, 2, 0, 1, amount.glyphs.codes);
        menu_draw_number(&menu_sprite_defs[0], &amount);
    }
}
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
static inline void menu_begin_text_glyph(
    const KfMenuSpriteDef *font, const KfMenuGlyphString *string, s32 x_offset)
{
    primitive_buffer_begin_poly_ft4();
    current_poly_ft4->tpage = font->tpage;
    current_poly_ft4->clut = font->clut;
    setXYWH(current_poly_ft4, string->position.x + x_offset,
        string->position.y, font->width, font->height);
}

ADDRESS(0x800210ac, 0x464)
void menu_draw_string(const KfMenuSpriteDef *font, const KfMenuGlyphString *string)
{
    const s16 *code = string->glyphs.codes;
    s32 i;
    s32 x_offset;

    for (i = 0; *code != KF_MENU_TEXT_END; code++, i++) {
        u32 glyph;

        x_offset = i * KF_MENU_GLYPH_ADVANCE;
        menu_begin_text_glyph(font, string, x_offset);
        glyph = *code & KF_MENU_TEXT_GLYPH_MASK;
        setUVWH(current_poly_ft4,
            (glyph % KF_MENU_FONT_COLUMNS) * KF_MENU_FONT_CELL_WIDTH,
            (glyph / KF_MENU_FONT_COLUMNS) * KF_MENU_FONT_CELL_HEIGHT,
            font->width, font->height);
        primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);

        if (*code & KF_MENU_TEXT_DAKUTEN) {
            menu_begin_text_glyph(font, string, x_offset);
            setUVWH(current_poly_ft4, KF_MENU_DAKUTEN_U, KF_MENU_KANA_MARK_V,
                font->width, font->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        }

        if (*code & KF_MENU_TEXT_HANDAKUTEN) {
            menu_begin_text_glyph(font, string, x_offset);
            setUVWH(current_poly_ft4, KF_MENU_HANDAKUTEN_U, KF_MENU_KANA_MARK_V,
                font->width, font->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        }
    }
}

ADDRESS(0x80021510, 0x2e0)
void menu_draw_number(const KfMenuSpriteDef *font, const KfMenuGlyphString *string)
{
    const s16 *code;
    s32 x_offset;

    code = string->glyphs.codes;
    if (*code == KF_MENU_TEXT_END) {
        return;
    }
    x_offset = 0;
    do {
        primitive_buffer_begin_poly_ft4();
        current_poly_ft4->tpage = font->tpage;
        current_poly_ft4->clut = font->clut;
        setXYWH(current_poly_ft4,
            string->position.x + x_offset, string->position.y,
            font->width, font->height);
        if (*code < KF_MENU_NUMBER_COLUMN_ROWS) {
            setUVWH(current_poly_ft4,
                font->u, *code * KF_MENU_FONT_CELL_HEIGHT,
                font->width, font->height);
        } else {
            setUVWH(current_poly_ft4,
                (u8)(font->u + KF_MENU_NUMBER_COLUMN_WIDTH),
                (*code - KF_MENU_NUMBER_COLUMN_ROWS) * KF_MENU_FONT_CELL_HEIGHT,
                font->width, font->height);
        }
        primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        code++;
        x_offset += KF_MENU_DIGIT_ADVANCE;
    } while (*code != KF_MENU_TEXT_END);
}
ADDRESS(0x800217f0, 0x270)
void menu_draw_nine_slice_panel(s32 x, s32 y, s32 width, s32 height,
    s32 overlap_x, s32 overlap_y)
{
    const KfMenuSpriteDef *tile;
    u16 clut;
    s32 row;
    s32 column;
    s32 tile_index;
    s32 draw_x;
    s32 draw_y;
    s32 draw_width;
    s32 draw_height;

    width -= 94;
    height -= 94;
    tile_index = 9;
    tile = &menu_sprite_defs[11];
    draw_y = y;
    for (row = 0; row < 3; row++) {
        draw_height = tile->height;
        if (row == 1) {
            draw_y += 33;
            draw_height += height + overlap_y;
        } else if (row == 2) {
            draw_y += 28 + height;
        }
        draw_x = x;
        for (column = 0; column < 3; column++) {
            draw_width = tile->width;
            if (column == 1) {
                draw_x += 33;
                draw_width += width + overlap_x;
            } else if (column == 2) {
                draw_x += 28 + width;
            }
            primitive_buffer_begin_poly_ft4();
            setRGB0(current_poly_ft4, 0xff, 0xff, 0xff);
            SetSemiTrans((void *)current_poly_ft4, 1);
            current_poly_ft4->tpage = tile->tpage;
            clut = tile->clut;
            setXYWH(current_poly_ft4, draw_x, draw_y, draw_width, draw_height);
            current_poly_ft4->clut = clut;
            setUVWH(current_poly_ft4, tile->u, tile->v, tile->width, tile->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
            tile_index++;
            tile = &menu_sprite_defs[tile_index + 2];
        }
    }
}

ADDRESS(0x80021a60, 0x8)
void menu_render_list_mode_8_9_noop(void)
{
}

ADDRESS(0x80021a68, 0x178)
void menu_frame_begin(void)
{
    game_graphics_runtime.display_state.buffer_index =
        game_graphics_runtime.display_state.buffer_index == 0;
    game_graphics_runtime.display_state.primitive_buffer =
        &game_graphics_runtime.display_state.primitive_buffers[
            game_graphics_runtime.display_state.buffer_index];
    game_graphics_runtime.display_state.ordering_table =
        game_graphics_runtime.display_state.ordering_tables[
            game_graphics_runtime.display_state.buffer_index].entries;
    ClearOTagR(game_graphics_runtime.display_state.ordering_table,
        KF_GAME_ORDERING_TABLE_LENGTH);
    game_graphics_runtime.display_state.primitive_buffer->cursor =
        game_graphics_runtime.display_state.primitive_buffer->start;
    current_poly_ft4 = (POLY_FT4 *)
        game_graphics_runtime.display_state.primitive_buffer->cursor;

    if (game_graphics_runtime.display_state.buffer_index == 1) {
        menu_frame_upload_rect.x = 0;
        menu_frame_upload_rect.y = KF_MENU_UPLOAD_SECOND_BUFFER_Y;
    } else {
        menu_frame_upload_rect.x = 0;
        menu_frame_upload_rect.y = 0;
    }

    if (menu_cursor_animation_direction == 0) {
        menu_cursor_animation_frame++;
    } else if (menu_cursor_animation_direction == 1) {
        menu_cursor_animation_frame--;
    }
    if (menu_cursor_animation_frame >= KF_MENU_CURSOR_FRAME_COUNT) {
        menu_cursor_animation_frame = KF_MENU_CURSOR_FRAME_COUNT - 1;
        menu_cursor_animation_direction = 1;
    }
    if (menu_cursor_animation_frame < 0) {
        menu_cursor_animation_frame = 0;
        menu_cursor_animation_direction = -1;
    }
}

ADDRESS(0x80021be0, 0xac)
void menu_present_frame(void)
{
    DrawSync(0);
    VSync(0);
    PutDrawEnv(&game_graphics_runtime.display_draw_environments[
        game_graphics_runtime.display_state.buffer_index]);
    PutDispEnv(&game_graphics_runtime.display_disp_environments[
        game_graphics_runtime.display_state.buffer_index]);
    LoadImage(&menu_frame_upload_rect, menu_frame_upload_pixels);
    DrawOTag(game_graphics_runtime.display_state.ordering_table
        + (KF_GAME_ORDERING_TABLE_LENGTH - 1));
}

ADDRESS(0x80021c8c, 0x174)
void menu_enter_display_state(s32 mode)
{
    pool_release_all();
    game_graphics_runtime.display_draw_environments[0].isbg = 0;
    game_graphics_runtime.display_draw_environments[0].dfe = 0;
    game_graphics_runtime.display_draw_environments[1].isbg = 0;
    game_graphics_runtime.display_draw_environments[1].dfe = 0;

    menu_saved_primitive_buffers[0] =
        game_graphics_runtime.display_state.primitive_buffers[0];
    menu_saved_primitive_buffers[1] =
        game_graphics_runtime.display_state.primitive_buffers[1];
    game_graphics_runtime.display_state.primitive_buffers[0].end =
        game_graphics_runtime.display_state.primitive_buffers[0].start + 0x6400;
    game_graphics_runtime.display_state.primitive_buffers[1].start =
        game_graphics_runtime.display_state.primitive_buffers[0].end;
    game_graphics_runtime.display_state.primitive_buffers[1].end =
        game_graphics_runtime.display_state.primitive_buffers[1].start + 0x6400;
    menu_frame_upload_pixels = (u_long *)
        game_graphics_runtime.display_state.primitive_buffers[1].end;

    if (game_graphics_runtime.display_state.buffer_index == 1) {
        menu_frame_upload_rect.x = 0;
        menu_frame_upload_rect.y = 240;
    } else {
        menu_frame_upload_rect.x = 0;
        menu_frame_upload_rect.y = 0;
    }
    menu_frame_upload_rect.w = 320;
    menu_frame_upload_rect.h = 240;
    StoreImage(&menu_frame_upload_rect, menu_frame_upload_pixels);
    DrawSync(0);

    menu_saved_music_enabled = player_state.audio_music_enabled;
    if (player_state.audio_music_enabled == 1 && audio_state.sequence_active == 1)
        SsSeqPause(audio_state.sequence_id);
}

ADDRESS(0x80021e00, 0x110)
void menu_exit_display_state(s32 stop_sequence)
{
    u8 music_enabled;

    game_graphics_runtime.display_state.primitive_buffers[0] =
        menu_saved_primitive_buffers[0];
    game_graphics_runtime.display_state.primitive_buffers[1] =
        menu_saved_primitive_buffers[1];
    game_graphics_runtime.display_draw_environments[0].isbg = 1;
    game_graphics_runtime.display_draw_environments[0].dfe = 1;
    game_graphics_runtime.display_draw_environments[1].isbg = 1;
    game_graphics_runtime.display_draw_environments[1].dfe = 1;

    if (stop_sequence == 1) {
        audio_stop_sequence();
    } else {
        music_enabled = player_state.audio_music_enabled;
        if (music_enabled != menu_saved_music_enabled) {
            if (music_enabled == 0)
                audio_stop_sequence();
            else
                audio_start_sequence();
        } else if (music_enabled == 1 && audio_state.sequence_active == 1) {
            SsSeqReplay(audio_state.sequence_id);
        }
    }
}

enum {
    KF_MENU_PRIMITIVE_BRIGHTNESS = 0x68
};

ADDRESS(0x80021f10, 0x50)
void primitive_buffer_begin_poly_ft4(void)
{
    SetPolyFT4(current_poly_ft4);
    setRGB0(current_poly_ft4,
        KF_MENU_PRIMITIVE_BRIGHTNESS,
        KF_MENU_PRIMITIVE_BRIGHTNESS,
        KF_MENU_PRIMITIVE_BRIGHTNESS);
}

ADDRESS(0x80021f60, 0x50)
void primitive_buffer_commit_poly_ft4(s32 depth)
{
    AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], current_poly_ft4);
    current_poly_ft4++;
    game_graphics_runtime.display_state.primitive_buffer->cursor = (u8 *)current_poly_ft4;
}

ADDRESS(0x80021fb0, 0xa8)
void menu_list_init(KfMenuList *list, s32 window_kind, s32 row)
{
    KfMenuGlyphString *source;
    s16 *title_codes;
    s16 *source_codes;
    s32 i;

    list->title.position.x = 17;
    list->title.position.y = 19;
    title_codes = list->title.glyphs.codes;
    source = &menu_window_layouts[window_kind].rows[row];
    source_codes = source->glyphs.codes;
    for (i = 0; i < KF_MENU_LIST_TITLE_COPY_GLYPHS; i++) {
        *title_codes++ = *source_codes++;
    }
    list->list_x = 0x2a;
    list->list_y = 0x9f;
    list->entry_count = 0;
    list->visible_rows = 4;
    list->scroll_offset = 0;
    list->selected_index = 0;
    list->cursor_row = 0;
    list->glyphs_per_entry = 10;
}

enum {
    KF_MENU_FORMAT_BLANK = 10,
    KF_MENU_FORMAT_STYLE_SINGLE_PREFIX = 1,
    KF_MENU_FORMAT_STYLE_TRAILING_13 = 2,
    KF_MENU_FORMAT_STYLE_PAIR_15_16 = 3,
    KF_MENU_FORMAT_STYLE_TRIPLE_12_18_16 = 4,
    KF_MENU_FORMAT_STYLE_PAIR_14_17 = 5,
    KF_MENU_FORMAT_STYLE_TRAILING_11 = 6
};

ADDRESS(0x80022058, 0x190)
void menu_format_number(s32 value, s32 count, s32 padding_mode, s32 style, s16 *out)
{
    s32 i;
    s16 blank;
    s16 *cursor;

    if ((u32)(style - 1) < 2 || style == KF_MENU_FORMAT_STYLE_TRAILING_11) {
        count++;
    } else if (style == KF_MENU_FORMAT_STYLE_PAIR_15_16
        || style == KF_MENU_FORMAT_STYLE_PAIR_14_17) {
        count += 2;
    } else if (style == KF_MENU_FORMAT_STYLE_TRIPLE_12_18_16) {
        count += 3;
    }

    i = 0;
    blank = padding_mode == 0 ? KF_MENU_FORMAT_BLANK : 0;
    if (count > 0) {
        cursor = out;
        do {
            *cursor++ = blank;
            i++;
        } while (i < count);
    }
    out[count] = KF_MENU_TEXT_END;

    if (style == KF_MENU_FORMAT_STYLE_SINGLE_PREFIX) {
        out[0] = 19;
    } else if (style == KF_MENU_FORMAT_STYLE_TRAILING_13) {
        out[count - 1] = 13;
        count--;
    } else if (style == KF_MENU_FORMAT_STYLE_PAIR_15_16) {
        out[0] = 15;
        out[1] = 16;
    } else if (style == KF_MENU_FORMAT_STYLE_TRIPLE_12_18_16) {
        out[0] = 12;
        out[1] = 18;
        out[2] = 16;
    } else if (style == KF_MENU_FORMAT_STYLE_PAIR_14_17) {
        out[0] = 14;
        out[1] = 17;
    } else if (style == KF_MENU_FORMAT_STYLE_TRAILING_11) {
        out[count - 1] = 11;
        count--;
    }

    for (i = count - 1; i >= 0; i--) {
        out[i] = value % 10;
        value /= 10;
        if (value == 0) {
            i = -1;
        }
    }
}

ADDRESS(0x800221e8, 0xd4)
s32 menu_load_item_model(u8 item_id)
{
    u8 *allocation;

    menu_item_quantity = 1;
    if (player_state.item_preview_enabled == 0)
        return 0;

    menu_release_item_model();
    if (item_id != 0xff) {
        allocation = memory_allocate(cd_archive_entry_extent(6, item_id, 0));
        cd_archive_read(6, item_id, (u_long *)allocation);
        tmd_register(KF_TMD_SLOT_MENU_ITEM, (KfTmdHeader *)allocation);
        menu_item_model_allocation_pending = KF_MENU_MODEL_ALLOCATED;
    }
    setVector(&menu_item_preview_translation, 0, 0, 1000);
    setVector(&menu_item_preview_rotation, 0, 0, 0);
    menu_item_preview_rotation_step = 4;
    return 0;
}

ADDRESS(0x800222bc, 0x44)
void menu_release_item_model(void)
{
    if (menu_item_model_allocation_pending == KF_MENU_MODEL_ALLOCATED) {
        memory_free((u8 *)game_graphics_runtime.tmd_state.slots[KF_TMD_SLOT_MENU_ITEM]);
        menu_item_model_allocation_pending = KF_MENU_MODEL_RELEASED;
    }
}
ADDRESS(0x80022300, 0x94)
void menu_play_sound_cue(s32 cue)
{
    if (cue == 16) {
        audio_key_on(16, 48, 48, 0);
        SsSeqCalledTbyT();
    } else if ((u32)cue - 17u < 2u) {
        audio_key_on(cue, 48, 48, 0);
        SsSeqCalledTbyT();
    } else if (cue == 13) {
        audio_key_on(13, 0, 58, 0);
        VSync(0);
        VSync(0);
        audio_key_on(13, 58, 0, 0);
    }
}

ADDRESS(0x80022394, 0x38)
u32 input_read_mark_active(void)
{
    u32 buttons = PadRead(1);
    if (buttons != 0) {
        input_press_pending = 1;
    }
    return buttons;
}

ADDRESS(0x800223cc, 0x6c)
void input_wait_brief_release(void)
{
    s32 polls;

    if (input_press_pending == 1) {
        input_press_pending = 0;
        for (polls = 0; PadRead(1) != 0;) {
            if (polls++ < 6) {
                VSync(0);
            } else {
                menu_cursor_animation_frame = 0;
                break;
            }
        }
    }
}
