#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/resources.h>
#include <psyq/pad.h>
#include <psyq/sdk.h>

ADDRESS(0x8003494c, 0x70)
void tim_upload_images(u8 *tim_data)
{
    TIM_IMAGE image;

    OpenTIM((u_long *)tim_data);
    while (ReadTIM(&image) != NULL) {
        if (image.caddr != NULL) {
            LoadImage(image.crect, image.caddr);
        }
        if (image.paddr != NULL) {
            LoadImage(image.prect, image.paddr);
        }
    }
}

#define MENU_FADE_NEXT_QUAD() do { \
    KfPrimitiveBuffer *buffer = game_graphics_runtime.display_state.primitive_buffer; \
    quad = (POLY_FT4 *)buffer->cursor; \
    buffer->cursor += sizeof(POLY_FT4); \
    if (game_graphics_runtime.display_state.primitive_buffer->cursor > \
        game_graphics_runtime.display_state.primitive_buffer->end) \
        goto present; \
    SetPolyFT4(quad); \
} while (0)

ADDRESS(0x800349bc, 0x454)
s32 func_800349bc(s32 level, s32 step)
{
    POLY_FT4 *quad;
    s32 state = -1;
    s32 shade;
    u32 buttons;

    for (;;) {
        display_begin_frame();
        shade = 0x80 - (level >> 1);

        MENU_FADE_NEXT_QUAD();
        quad->x0 = 0;
        quad->y0 = 0;
        quad->x1 = 192;
        quad->y1 = 0;
        quad->x2 = 0;
        quad->y2 = 240;
        quad->x3 = 192;
        quad->y3 = 240;
        quad->u0 = 0;
        quad->v0 = 0;
        quad->u1 = 192;
        quad->v1 = 0;
        quad->u2 = 0;
        quad->v2 = 240;
        quad->u3 = 192;
        quad->v3 = 240;
        quad->clut = 0;
        setTPage(quad, 2, 0, 320, 0);
        setRGB0(quad, shade, shade, shade);
        AddPrim(game_graphics_runtime.display_state.ordering_table + 2, quad);

        MENU_FADE_NEXT_QUAD();
        quad->x0 = 192;
        quad->y0 = 0;
        quad->x1 = 320;
        quad->y1 = 0;
        quad->x2 = 192;
        quad->y2 = 240;
        quad->x3 = 320;
        quad->y3 = 240;
        quad->u0 = 0;
        quad->v0 = 0;
        quad->u1 = 128;
        quad->v1 = 0;
        quad->u2 = 0;
        quad->v2 = 240;
        quad->u3 = 128;
        quad->v3 = 240;
        quad->clut = 0;
        setTPage(quad, 2, 0, 512, 0);
        setRGB0(quad, shade, shade, shade);
        AddPrim(game_graphics_runtime.display_state.ordering_table + 2, quad);

        MENU_FADE_NEXT_QUAD();
        SetSemiTrans(quad, 1);
        quad->x0 = 32;
        quad->y0 = 112;
        quad->x1 = 288;
        quad->y1 = 112;
        quad->x2 = 32;
        quad->y2 = 240;
        quad->x3 = 288;
        quad->y3 = 240;
        quad->u0 = 0;
        quad->v0 = 0;
        quad->u1 = 255;
        quad->v1 = 0;
        quad->u2 = 0;
        quad->v2 = 128;
        quad->u3 = 255;
        quad->v3 = 128;
        setClut(quad, 576, 511);
        setTPage(quad, 0, 1, 960, 256);
        setRGB0(quad, level, level, level);
        AddPrim(game_graphics_runtime.display_state.ordering_table, quad);

        MENU_FADE_NEXT_QUAD();
        SetSemiTrans(quad, 1);
        quad->x0 = 32;
        quad->y0 = 112;
        quad->x1 = 288;
        quad->y1 = 112;
        quad->x2 = 32;
        quad->y2 = 240;
        quad->x3 = 288;
        quad->y3 = 240;
        quad->u0 = 0;
        quad->v0 = 0;
        quad->u1 = 255;
        quad->v1 = 0;
        quad->u2 = 0;
        quad->v2 = 128;
        quad->u3 = 255;
        quad->v3 = 128;
        setClut(quad, 576, 511);
        setTPage(quad, 0, 2, 960, 256);
        setRGB0(quad, level, level, level);
        AddPrim(game_graphics_runtime.display_state.ordering_table + 1, quad);

present:
        DrawSync(0);
        display_present_frame();
        level += step;
        if ((u32)(level - 1) >= 119u) {
            DrawSync(0);
            return state;
        }
        buttons = PadRead(1);
        if (state == -1) {
            if (buttons == 0)
                state = -2;
        } else if (buttons != 0) {
            DrawSync(0);
            return level;
        }
    }
}

#undef MENU_FADE_NEXT_QUAD

ADDRESS(0x80034e10, 0x180)
void func_80034e10(u16 archive_slot, u16 archive_entry)
{
    s32 frame;
    u32 buttons;
    u8 *scratch;

    DrawSync(0);
    cd_archive_read(archive_slot, archive_entry,
        (u_long *)game_graphics_runtime.display_state.asset_load_buffer);
    tim_upload_images(game_graphics_runtime.display_state.asset_load_buffer);
    DrawSync(0);

    scratch = game_graphics_runtime.display_state.primitive_buffers[0].start;
    scratch += KF_GAME_PRIMITIVE_BUFFER_BYTES / 4;
    game_graphics_runtime.display_state.primitive_buffers[0].end = scratch;
    game_graphics_runtime.display_state.primitive_buffers[1].start = scratch;
    scratch += KF_GAME_PRIMITIVE_BUFFER_BYTES / 4;
    game_graphics_runtime.display_state.primitive_buffers[1].end = scratch;
    StoreImage(&menu_transition_rect, (u_long *)scratch);
    DrawSync(0);
    MoveImage(&game_graphics_runtime.display_draw_environments[
            game_graphics_runtime.display_state.buffer_index].clip,
        menu_transition_rect.x, menu_transition_rect.y);
    DrawSync(0);

    frame = func_800349bc(0, 12);
    if (frame < 0) {
        for (;;) {
            buttons = PadRead(1);
            if (frame == -1) {
                if (buttons != 0)
                    continue;
                frame = -2;
                continue;
            }
            if (buttons == 0)
                continue;
            frame = 80;
            break;
        }
    }
    func_800349bc(frame, -12);
    LoadImage(&menu_transition_rect,
        (u_long *)game_graphics_runtime.display_state.primitive_buffers[1].end);
    game_graphics_runtime.display_state.primitive_buffers[0].end =
        game_graphics_runtime.display_state.primitive_buffers[0].start
        + KF_GAME_PRIMITIVE_BUFFER_BYTES;
    game_graphics_runtime.display_state.primitive_buffers[1].start =
        game_graphics_runtime.display_state.primitive_buffers[0].end;
    game_graphics_runtime.display_state.primitive_buffers[1].end =
        game_graphics_runtime.display_state.primitive_buffers[1].start
        + KF_GAME_PRIMITIVE_BUFFER_BYTES;
}
