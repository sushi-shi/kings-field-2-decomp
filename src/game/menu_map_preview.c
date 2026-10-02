#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/memory.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/game/resources.h>
#include <psyq/pad.h>
#include <psyq/sdk.h>

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
    map_offset = state_8017d118.unknown_09[0] + 480;
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

        func_800217f0(0x36, 0xe, 0xd4, 0xd4, 2, 2);
        menu_present_frame();
    }

    input_wait_release();
    while (PadRead(1) == 0) {}
    input_wait_release();
    memory_free(image);
}
