#ifndef KF_GAME_EVENT_STREAM_H
#define KF_GAME_EVENT_STREAM_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

struct KfActor;
struct KfMapObject;

/* Partial view used by scene interpolation; the enclosing object extent is open. */
typedef struct KfScenePoseView {
    u8 unknown_00[0x14];
    s32 position_x;
    s32 position_y;
    s32 position_z;
    u8 unknown_20[4];
    s16 angle_x;
    u8 unknown_26[2];
    s16 angle_z;
    u8 unknown_2a[2];
} KfScenePoseView;

typedef char kf_scene_pose_position_offset[(u32)&((KfScenePoseView *)0)->position_x == 0x14 ? 1 : -1];
typedef char kf_scene_pose_angle_x_offset[(u32)&((KfScenePoseView *)0)->angle_x == 0x24 ? 1 : -1];
typedef char kf_scene_pose_angle_z_offset[(u32)&((KfScenePoseView *)0)->angle_z == 0x28 ? 1 : -1];

/* Partial event-object view: the caller also uses fields outside this range. */
typedef struct KfEventObjectView {
    u8 unknown_00[0x39];
    u8 effect_object_index;
} KfEventObjectView;

typedef char kf_event_object_effect_index_offset[
    (u32)&((KfEventObjectView *)0)->effect_object_index == 0x39 ? 1 : -1];

void event_spawn_effect_object(KfEventObjectView *event, s32 object_id);
void scene_position_from_camera_offset(s32 x, s32 y, s32 z, s32 pitch, s32 yaw,
                   s32 height_offset, s32 z_offset, VECTOR *output);
void scene_pose_interpolate(KfScenePoseView *destination,
                   const VECTOR *start_position, const VECTOR *end_position,
                   const SVECTOR *start_angles, const SVECTOR *end_angles,
                   s32 fraction);
void func_800462bc(struct KfActor *actor);
void func_800475d8(struct KfMapObject *object, ...);

#endif
