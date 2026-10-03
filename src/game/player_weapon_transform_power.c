#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/animation.h>
#include <kf/game/player.h>

ADDRESS(0x80026330, 0x134)
void player_sample_weapon_world_vertex(s32 vertex_index, VECTOR *output)
{
    SVECTOR offset;
    struct KfEulerAngles angles;

    angles.x = player_state.camera_rotation.angles[0]
             - player_state.equipped_weapon_record->rotation_offset_x;
    angles.y = player_state.camera_rotation.angles[1]
             - player_state.equipped_weapon_record->rotation_offset_y;
    angles.z = player_state.camera_rotation.angles[2]
             + player_state.equipped_weapon_record->rotation_offset_z;
    animation_sample_vertex(32, player_state.weapon_attack_mode,
                  player_state.weapon_attack_phase, vertex_index, &offset);
    offset.vx -= player_state.equipped_weapon_record->position_offset_x;
    offset.vy += player_state.equipped_weapon_record->position_offset_y;
    offset.vz -= player_state.equipped_weapon_record->position_offset_z;
    vector_rotate_yxz(&angles, &offset, output);
    output->vx += player_state.camera_position.vx;
    output->vz += player_state.camera_position.vz;
    {
        s32 y = output->vy - 1600;
        s32 camera_y = player_state.camera_vertical_offset + player_state.camera_position.vy
                     + player_state.landing_vertical_offset;
        output->vy = y + camera_y;
    }
}

enum { PLAYER_SPECIAL_ATTACK_POWER_MINIMUM = 60 };

ADDRESS(0x80026464, 0x34)
s32 player_has_power_and_magic_60(void)
{
    return player_state.physical_power >= PLAYER_SPECIAL_ATTACK_POWER_MINIMUM
        && player_state.magic >= PLAYER_SPECIAL_ATTACK_POWER_MINIMUM;
}
