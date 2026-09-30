#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>
#include <psyq/sdk.h>

enum {
    KF_MENU_MODEL_BACK_COLOR = 60,
    KF_MENU_MODEL_GEOM_SCREEN = 200,
    KF_MENU_MODEL_FOG_NEAR = 0x59d8
};

extern void func_8002d918(s32 vertex_count);
extern void func_8002e4dc(s32 object_index, s32 depth_bias);

ADDRESS(0x80033994, 0x68)
void func_80033994(void)
{
    SetBackColor(KF_MENU_MODEL_BACK_COLOR, KF_MENU_MODEL_BACK_COLOR, KF_MENU_MODEL_BACK_COLOR);
    SetGeomScreen(KF_MENU_MODEL_GEOM_SCREEN);
    tmd_select(KF_TMD_SLOT_MENU_ITEM);
    tmd_select_object_vertices(0);
    fog_set_near(KF_MENU_MODEL_FOG_NEAR);
    func_8002d918(tmd_get_object(0)->vertex_count);
    func_8002e4dc(0, 0);
}
