#ifndef KF_GAME_EVENT_STREAM_H
#define KF_GAME_EVENT_STREAM_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

struct KfActor;
struct KfMapObject;

enum {
    KF_EVENT_STREAM_REWIND_MARKER = 0xf0,
    KF_EVENT_STREAM_CONDITIONAL_MARKER = 0xf1,
    KF_EVENT_STREAM_MARKER_RECORD = 0xf2,
    KF_EVENT_STREAM_START = 0xfe,
    KF_EVENT_STREAM_END = 0xff
};

enum {
    KF_EVENT_POST_STREAM_MENU_MASK = 0xf0,
    KF_EVENT_POST_STREAM_BUY_SELL_CHOICE_MASK = 0x0f,
    KF_EVENT_POST_STREAM_BUY_SELL = 0,
    KF_EVENT_POST_STREAM_STOCK_CHOICE = 0x10,
    KF_EVENT_POST_STREAM_TRADE = 0x20,
    KF_EVENT_POST_STREAM_INVENTORY_CHOICE = 0x30
};

void event_spawn_effect_object(struct KfMapObject *event, s32 object_id);
void scene_position_from_camera_offset(s32 x, s32 y, s32 z, s32 pitch, s32 yaw,
                   s32 height_offset, s32 z_offset, VECTOR *output);
void scene_pose_interpolate(struct KfMapObject *destination,
                   const VECTOR *start_position, const VECTOR *end_position,
                   const SVECTOR *start_angles, const SVECTOR *end_angles,
                   s32 fraction);
void event_target_stream_execute(struct KfActor *actor);
void event_map_object_interact(struct KfMapObject *object, ...);

#endif
