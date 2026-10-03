#ifndef KF_GAME_EVENT_STREAM_H
#define KF_GAME_EVENT_STREAM_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

struct KfActor;
struct KfMapObject;

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
