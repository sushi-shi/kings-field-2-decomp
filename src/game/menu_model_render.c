#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>
#include <psyq/sdk.h>

enum {
    KF_MENU_MODEL_BACK_COLOR = 60,
    KF_MENU_MODEL_GEOM_SCREEN = 200,
    KF_MENU_MODEL_FOG_NEAR = 0x59d8
};

ADDRESS(0x80033994, 0x68)
void menu_render_item_model(void)
{
    SetBackColor(KF_MENU_MODEL_BACK_COLOR, KF_MENU_MODEL_BACK_COLOR, KF_MENU_MODEL_BACK_COLOR);
    SetGeomScreen(KF_MENU_MODEL_GEOM_SCREEN);
    tmd_select(KF_TMD_SLOT_MENU_ITEM);
    tmd_select_object_vertices(0);
    fog_set_near(KF_MENU_MODEL_FOG_NEAR);
    tmd_project_vertices_with_fog(tmd_get_object(0)->vertex_count);
    render_enqueue_textured_tmd(0, 0);
}
