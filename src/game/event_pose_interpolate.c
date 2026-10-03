#include <kf/game/event_stream.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <kf/lib/math.h>

ADDRESS(0x80045f20, 0xb4)
void scene_position_from_camera_offset(s32 x, s32 y, s32 z, s32 pitch, s32 yaw,
                   s32 height_offset, s32 z_offset, VECTOR *output)
{
    SVECTOR offset;
    struct KfEulerAngles angles;
    s32 y_position;
    s32 camera_y;

    offset.vx = x;
    offset.vy = y + height_offset;
    offset.vz = z + z_offset;
    angles.x = -pitch;
    angles.y = yaw;
    angles.z = 0;
    vector_rotate_yxz(&angles, &offset, output);
    output->vx += player_state.camera_position.vx;
    y_position = output->vy - 1600;
    camera_y = height_offset + player_state.camera_position.vy;
    output->vy = y_position + camera_y;
    output->vz += player_state.camera_position.vz;
}

ADDRESS(0x80045fd4, 0xcc)
void scene_pose_interpolate(
    KfScenePoseView *destination, const VECTOR *start_position,
    const VECTOR *end_position, const SVECTOR *start_angles,
    const SVECTOR *end_angles, s32 fraction)
{
    if (start_position != 0) {
        destination->position_x = fixed_lerp_q12(start_position->vx, end_position->vx, fraction);
        destination->position_y = fixed_lerp_q12(start_position->vy, end_position->vy, fraction);
        destination->position_z = fixed_lerp_q12(start_position->vz, end_position->vz, fraction);
    }

    if (start_angles != 0) {
        destination->angle_x = angle_lerp_shortest_q12(start_angles->vx, end_angles->vx, fraction);
        destination->angle_z = angle_lerp_shortest_q12(start_angles->vz, end_angles->vz, fraction);
    }
}
