#include <kf/game/callback.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <psyq/pad.h>
#include <kf/game/event_counter.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/memory.h>
#include <kf/game/resources.h>
#include <psyq/sdk.h>
#include <kf/game/effect.h>

RODATA(0x80011098, 0x1c)

DATA(0x80064c30, 0xb40)
KfMenuGlyphRow menu_glyph_rows[120] = {
    {{4111, 4101, 45, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{11, 56, 45, 19, 14, 45, 4115, -1, 0, 0, 0, 0}},
    {{20, 1, 19, 14, 45, 4115, -1, 0, 0, 0, 0, 0}},
    {{34, 45, 21, 39, 4103, 12, 15, 45, -1, 0, 0, 0}},
    {{4121, 19, 42, 25, 39, 30, 45, -1, 0, 0, 0, 0}},
    {{4121, 12, 15, 45, 4115, 14, 45, 4115, -1, 0, 0, 0}},
    {{7, 43, 13, 39, 19, 0, 53, 7, 12, -1, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{27, 43, 1, 32, 14, 45, 4115, -1, 0, 0, 0, 0}},
    {{160, 161, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{12, 8217, 1, 4111, 45, -1, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 12, 4123, 43, 45, 4115, -1, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 129, -1, 0, 0, 0}},
    {{32, 45, 39, 40, 1, 19, 14, 45, 4115, -1, 0, 0}},
    {{4111, 45, 7, 57, 12, 43, 1, 35, 45, -1, 0, 0}},
    {{4125, 2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 42, 4121, 43, 12, 19, -1, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 0, 39, 30, 12, 7, -1, 0, 0, 0, 0}},
    {{20, 1, 19, 28, 42, 32, -1, 0, 0, 0, 0, 0}},
    {{4103, 43, 45, 19, 28, 42, 32, -1, 0, 0, 0, 0}},
    {{4123, 40, 53, 4115, 7, 40, 2, 39, -1, 0, 0, 0}},
    {{162, 163, 88, 150, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 150, -1, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{4123, 43, 12, 19, 8219, 43, 45, 19, -1, 0, 0, 0}},
    {{20, 1, 19, 8219, 43, 45, 19, -1, 0, 0, 0, 0}},
    {{0, 1, 12, 0, 45, 30, 45, -1, 0, 0, 0, 0}},
    {{211, 212, 88, 157, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 157, -1, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{43, 4106, 45, 11, 45, 42, 4115, -1, 0, 0, 0, 0}},
    {{40, 45, 4107, 11, 45, 42, 4115, -1, 0, 0, 0, 0}},
    {{32, 45, 39, 4101, 45, 4115, -1, 0, 0, 0, 0, 0}},
    {{7, 41, 12, 15, 42, 4101, 45, 4115, -1, 0, 0, 0}},
    {{12, 5, 42, 11, 45, 42, 4115, -1, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 124, -1, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 0, 39, 4103, 44, 45, 4123, -1, 0, 0, 0}},
    {{12, 19, 45, 39, 25, 39, 4115, -1, 0, 0, 0, 0}},
    {{11, 42, 4121, 45, 0, 45, 32, -1, 0, 0, 0, 0}},
    {{4114, 45, 34, 39, 25, 39, 4115, -1, 0, 0, 0, 0}},
    {{42, 1, 20, 12, 4103, 44, 45, 4123, -1, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 0, 39, 4123, 45, 17, -1, 0, 0, 0, 0}},
    {{43, 53, 4103, 4101, 45, 4111, 45, -1, 0, 0, 0, 0}},
    {{11, 42, 4121, 45, 4123, 45, 17, -1, 0, 0, 0, 0}},
    {{4114, 12, 2, 52, 45, 5, 45, -1, 0, 0, 0, 0}},
    {{42, 1, 20, 12, 4123, 45, 17, -1, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{204, 88, 127, 156, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{11, 45, 12, 88, 177, -1, 0, 0, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 127, 156, -1, 0, 0}},
    {{168, 205, 88, 158, 156, -1, 0, 0, 0, 0, 0, 0}},
    {{246, 81, 247, 248, 249, 88, 250, 156, -1, 0, 0, 0}},
    {{251, 83, 252, 88, 120, 253, 72, -1, 0, 0, 0, 0}},
    {{189, 254, 88, 158, 156, -1, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{165, 166, 88, 168, 169, -1, 0, 0, 0, 0, 0, 0}},
    {{141, 142, 159, 105, 88, 168, 169, -1, 0, 0, 0, 0}},
    {{181, 149, 88, 168, 169, -1, 0, 0, 0, 0, 0, 0}},
    {{6, 39, 5, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{4123, 40, 53, 4115, 12, 19, 45, 39, -1, 0, 0, 0}},
    {{32, 45, 39, 12, 19, 45, 39, -1, 0, 0, 0, 0}},
    {{4098, 48, 45, 4111, 1, 19, -1, 0, 0, 0, 0, 0}},
    {{143, 144, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{136, 145, 75, 144, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{148, 149, 144, 88, 151, -1, 0, 0, 0, 0, 0, 0}},
    {{146, 147, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{146, 147, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{146, 147, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{146, 147, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{136, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{141, 142, 88, 173, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{11, 45, 12, 88, 196, -1, 0, 0, 0, 0, 0, 0}},
    {{171, 88, 172, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{174, 175, 88, 176, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{11, 45, 12, 88, 191, -1, 0, 0, 0, 0, 0, 0}},
    {{199, 120, 88, 200, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{188, 88, 111, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{74, 74, 99, 70, 88, 167, -1, 0, 0, 0, 0, 0}},
    {{190, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{141, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{187, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{188, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{189, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{4111, 45, 7, 7, 41, 12, 15, 42, -1, 0, 0, 0}},
    {{141, 142, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{141, 142, 88, 69, 72, 104, -1, 0, 0, 0, 0, 0}},
    {{31, 11, 37, 2, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{3, 42, 27, 88, 164, -1, 0, 0, 0, 0, 0, 0}},
    {{165, 166, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{194, 195, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{178, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{182, 183, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{201, 149, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{148, 88, 186, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{120, 192, 193, 88, 164, -1, 0, 0, 0, 0, 0, 0}},
    {{210, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{209, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{184, 185, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{223, 110, 88, 127, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{152, 88, 4104, 45, 19, -1, 0, 0, 0, 0, 0, 0}},
    {{153, 88, 4104, 45, 19, -1, 0, 0, 0, 0, 0, 0}},
    {{154, 155, 88, 4104, 45, 19, -1, 0, 0, 0, 0, 0}},
    {{152, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{153, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{154, 155, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{141, 142, 88, 135, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{3, 42, 27, 88, 135, -1, 0, 0, 0, 0, 0, 0}},
    {{40, 12, 19, 4104, 45, 19, -1, 0, 0, 0, 0, 0}},
};

DATA(0x80065770, 0x1e0)
KfMenuGlyphRow menu_glyph_rows_extra[20] = {
    {{2, 52, 45, 15, 45, 27, 52, 45, 42, -1, 0, 0}},
    {{12, 19, 45, 39, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 45, 12, 2, 51, 45, 4123, -1, 0, 0, 0, 0}},
    {{33, 18, 4, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{2, 49, 39, 4115, 5, 53, 15, 45, -1, 0, 0, 0}},
    {{0, 1, 12, 12, 19, 45, 32, -1, 0, 0, 0, 0}},
    {{27, 41, 45, 4108, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{27, 48, 1, 0, 45, 4125, 45, 42, -1, 0, 0, 0}},
    {{27, 48, 1, 0, 45, 2, 52, 45, 42, -1, 0, 0}},
    {{27, 48, 1, 0, 45, 12, 19, 45, 32, -1, 0, 0}},
    {{27, 43, 1, 32, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{40, 1, 19, 21, 39, 4103, 4125, 42, 19, -1, 0, 0}},
    {{27, 40, 53, 11, 55, -1, 0, 0, 0, 0, 0, 0}},
    {{11, 45, 12, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{4114, 49, 12, 8221, 1, 4110, 39, -1, 0, 0, 0, 0}},
    {{43, 4107, 12, 19, 27, 48, 1, 0, -1, 0, 0, 0}},
    {{0, 45, 12, 26, 45, 42, -1, 0, 0, 0, 0, 0}},
    {{31, 10, 1, 42, 11, 45, 42, 4115, -1, 0, 0, 0}},
    {{40, 1, 19, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{4123, 43, 12, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
};

DATA(0x80065950, 0x2d0)
u8 menu_item_mask_pages[6][120] = {
    {
        0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
};

ADDRESS(0x8001876c, 0x284)
s32 menu_run_root_controller(void)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 choice_result;
    s32 frame;
    u32 buttons;

    menu_enter_display_state(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_player_status();
        menu_draw_window(0, 8, cursor, confirmed);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            choice_result = menu_item_selection_controller();
            goto selection_result;
        case 1:
            choice_result = menu_choose_magic_action();
            goto selection_result;
        case 2:
            menu_equipment_list_controller();
            break;
        case 3:
            menu_show_combat_attributes();
            break;
        case 4:
            menu_item_use_controller();
            break;
        case 5:
            choice_result = menu_run_card_choice();
selection_result:
            result = choice_result;
            if (choice_result == -1)
                result = -99;
            break;
        case 6:
            menu_options_controller();
            break;
        }

        if (result != -99)
            break;

        cursor = menu_poll_choice_input(cursor, 7, &selection, &confirmed, &result);
        buttons = PadRead(1);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if ((buttons & PADR1) != 0 && (buttons & PADL1) != 0)
                menu_draw_location_number();
            menu_draw_player_status();
            menu_draw_window(0, 8, cursor, confirmed);
            menu_present_frame();
        }
    }

    if (result == -1)
        menu_play_sound_cue(0);
    if (result == -3)
        menu_exit_display_state(1);
    else
        menu_exit_display_state(0);
    if (result != -1 && result != -3 && (result & 0x1000) != 0) {
        player_select_magic_action(result & 0xfff);
        result = -1;
    }
    return result;
}

ADDRESS(0x800189f0, 0xd8)
void menu_draw_location_number(void)
{
    KfMenuGlyphString row;
    s32 camera_x;
    s32 camera_z;
    s32 grid_z;
    s32 prefix;
    u16 map_layer;
    s32 value;

    row.position.x = 252;
    row.position.y = 205;
    camera_x = player_state.camera_position.vx;
    camera_z = player_state.camera_position.vz;
    map_layer = player_state.map_layer_index;
    grid_z = camera_z >> 11;
    prefix = state_8017d118.current_map_region_id * 100000
           + map_layer * 10000
           + (camera_x >> 11) * 100;
    value = prefix + grid_z;
    menu_format_number(value, 6, 1, 0, row.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &row);
}

ADDRESS(0x80018ac8, 0x240)
s32 menu_item_selection_controller(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[54];
    u8 values[56];
    u8 indices[56];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    u8 selected_item;

    count = menu_collect_masked_item_rows(game_counter_bytes, rows, values, indices, 67, 119);
    menu_list_init(&menu.list, 0, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = 12;
    if (menu.list.entry_count != 0
        && menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return -1;

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 0, 1, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }

        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &mode, &result);
        selected_item = indices[menu.list.selected_index];
        if (mode == 1) {
            menu_play_sound_cue(17);
            if (selected_item == 67 || selected_item == 68 || selected_item == 69) {
                menu_release_item_model();
                menu_show_map_preview(selected_item);
                menu_play_sound_cue(18);
                if (menu_load_item_model(selected_item) != 0)
                    return -1;
                mode = 0;
            }
        }
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 1);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if ((u32)(result - 71) < 10)
        menu_apply_item_effect(result);
    return result;
}

ADDRESS(0x80018d08, 0xe4)
s32 menu_collect_masked_item_rows(const u8 *mask, KfMenuGlyphRow *rows,
    u8 *values, u8 *indices, s32 first, s32 last)
{
    const KfMenuGlyphRow *glyph = &menu_glyph_rows[first];
    const u8 *entry = mask + first;
    s32 index;
    s32 count = 0;

    last++;
    index = first;
    while (index < last) {
        if (*entry != 0) {
            *rows++ = *glyph;
            *values = *entry;
            *indices = index;
            values++;
            indices++;
            count++;
        }
        entry++;
        glyph++;
        index++;
    }
    return count;
}

ADDRESS(0x80018dec, 0x1a0)
s32 menu_collect_available_item_rows(const u8 *mask, KfMenuGlyphRow *rows,
    u8 *values, u8 *indices, s32 first, s32 last)
{
    const KfMenuGlyphRow *glyph = &menu_glyph_rows[first];
    const u8 *entry = mask + first;
    s32 index;
    s32 count = 0;

    last++;
    index = first;
    while (index < last) {
        if (*entry != 0) {
            *values = *entry;
            if (index == player_state.equipped_weapon_id
                || index == player_state.equipped_head_id
                || index == player_state.equipped_body_id
                || index == player_state.equipped_arm_id
                || index == player_state.equipped_leg_id
                || index == player_state.equipped_shield_id
                || index == player_state.equipped_accessory_id
                || index == player_state.equipped_extra_id
                || index == player_state.secondary_item_shortcut_id)
                --*values;
            if (*values != 0) {
                *rows = *glyph;
                *indices = index;
                rows++;
                values++;
                indices++;
                count++;
            }
        }
        entry++;
        glyph++;
        index++;
    }
    return count;
}

ADDRESS(0x80018f8c, 0x2b4)
void menu_apply_item_effect(s32 item_id)
{
    u16 current_hp;

    if (game_counter_bytes[item_id] == 0)
        return;

    if (item_id == 71) {
        if (player_state.paralysis_timer > 0)
            player_state.paralysis_timer = 0;
        if (player_state.slow_timer >= 65)
            player_state.slow_timer = 64;
        player_cap_curse_strength();
        player_cap_darkness_phase();
    } else if (item_id == 72) {
        player_state.vitals.current_mp += 40;
    } else if (item_id == 73) {
        player_state.base_magic++;
        player_recalculate_combat_stats();
    } else if (item_id == 74) {
        player_state.vitals.current_hp += 40;
    } else if (item_id == 75) {
        current_hp = player_state.vitals.current_hp;
        player_state.poison_timer = 0;
        player_state.vitals.current_hp = current_hp + 15;
    } else if (item_id == 76) {
        player_state.vitals.current_hp = player_state.vitals.maximum_hp;
        player_state.vitals.current_mp = player_state.vitals.maximum_mp;
        player_clear_and_cap_status_effects();
    } else if (item_id == 77) {
        player_state.vitals.current_hp += 100;
    } else if (item_id == 78) {
        player_state.vitals.current_mp += 50;
    }

    if (item_id == 79) {
        player_clear_and_cap_status_effects();
    } else if (item_id == 80) {
        player_state.vitals.current_hp += 100;
        player_state.vitals.current_mp += 50;
        player_clear_and_cap_status_effects();
    }

    if (player_state.vitals.current_hp > player_state.vitals.maximum_hp)
        player_state.vitals.current_hp = player_state.vitals.maximum_hp;
    if (player_state.vitals.current_mp > player_state.vitals.maximum_mp)
        player_state.vitals.current_mp = player_state.vitals.maximum_mp;

    game_counter_bytes[item_id]--;
    if ((u32)(item_id - 77) < 2 || (u32)(item_id - 79) < 2)
        game_counter_bytes[0x52]++;
    menu_play_sound_cue(13);
}

ADDRESS(0x80019240, 0x6c)
void player_clear_and_cap_status_effects(void)
{
    if (player_state.paralysis_timer > 0)
        player_state.paralysis_timer = 0;
    if (player_state.slow_timer > 64)
        player_state.slow_timer = 64;
    player_state.poison_timer = 0;
    player_cap_curse_strength();
    player_cap_darkness_phase();
}

ADDRESS(0x800192ac, 0x30)
void player_cap_darkness_phase(void)
{
    if (player_state.darkness_phase > 64) {
        player_state.darkness_phase = 64;
        player_state.darkness_phase_limit = 0;
    }
}

ADDRESS(0x800192dc, 0x30)
void player_cap_curse_strength(void)
{
    if (player_state.curse_strength > 64) {
        player_state.curse_strength = 64;
        player_state.curse_phase_limit = 0;
    }
}

ADDRESS(0x8001930c, 0x528)
void menu_show_map_preview(s32 menu_code)
{
    u32 entry;
    u32 map_index;
    u32 map_offset;
    u8 *image;
    s32 frame;
    s32 facing_tile;
    s32 u0;

    map_index = (menu_code - 0x43) & 0xff;
    map_offset = state_8017d118.current_map_region_id + 480;
    entry = map_index * 8 + map_offset;
    image = memory_allocate(cd_archive_entry_extent(6, entry, 0));
    cd_archive_read(6, entry, (u_long *)image);
    tim_upload_images(image);

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();

        primitive_buffer_begin_poly_ft4();
        setRGB0(current_poly_ft4, 0x7f, 0x7f, 0x7f);
        SetSemiTrans((void *)current_poly_ft4, 1);
        current_poly_ft4->tpage = 0x1f;
        current_poly_ft4->clut = 0x7fe4;
        setXYWH(current_poly_ft4, 0x3c, 0x14, 200, 200);
        setUVWH(current_poly_ft4, 0, 0, 200, 200);
        primitive_buffer_commit_poly_ft4(10);

        primitive_buffer_begin_poly_ft4();
        setRGB0(current_poly_ft4, 0x7f, 0x7f, 0x7f);
        SetSemiTrans((void *)current_poly_ft4, 1);
        current_poly_ft4->tpage = 0x1c;
        current_poly_ft4->clut = 0x7d25;
        setXYWH(current_poly_ft4,
            player_state.camera_position.vx / 819 + 52,
            212 - player_state.camera_position.vz / 819,
            15, 15);

        facing_tile = ((player_state.camera_rotation.angles[1] & 0xfff) + 256) >> 9;
        if (facing_tile == 8)
            facing_tile = 0;
        u0 = facing_tile * 16 - 128;
        setUVWH(current_poly_ft4, u0, 0x90, 15, 15);
        primitive_buffer_commit_poly_ft4(9);

        menu_draw_nine_slice_panel(0x36, 0xe, 0xd4, 0xd4, 2, 2);
        menu_present_frame();
    }

    input_wait_release();
    while (PadRead(1) == 0) {}
    input_wait_release();
    memory_free(image);
}

ADDRESS(0x80019834, 0x19c)
s32 menu_choose_magic_action(void)
{
    KfMagicMenuList menu;
    KfMenuGlyphRow rows[20];
    s32 values[20];
    u8 indices[20];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    s32 frame;

    count = menu_collect_available_magic_rows(effect_state.magic_records, rows, values, indices, 14, 19);
    menu_list_init(&menu.list, 0, 1);
    menu.list.entry_count = count;
    menu.list.visible_rows = 6;
    menu.rows = rows;
    menu.values = values;
    menu.list.list_y = 0x83;
    menu.list.glyphs_per_entry = 12;

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 0, 2, 0xff);
            if (result == -1)
                result = -99;
            else
                result = indices[menu.list.selected_index];
        }

        if (result != -99)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 2);
            menu_present_frame();
        }
    }

    if (result != -1)
        result |= 0x1000;
    return result;
}

ADDRESS(0x800199d0, 0xf4)
s32 menu_collect_available_magic_rows(const KfMagicRecord *records,
    KfMenuGlyphRow *rows, s32 *values, u8 *indices, s32 first, s32 last)
{
    const KfMenuGlyphRow *glyph = &menu_glyph_rows_extra[first];
    const KfMagicRecord *entry = &records[first];
    s32 count = 0;
    s32 index;

    last++;
    index = first;
    while (index < last) {
        if (entry->menu_available == 1) {
            *rows++ = *glyph;
            *values = entry->mp_cost;
            *indices = index;
            values++;
            indices++;
            count++;
        }
        entry++;
        glyph++;
        index++;
    }
    return count;
}
