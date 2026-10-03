#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/asset.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

DATA(0x801c7068, 0x8)
SVECTOR effect_collision_motion_step;

RODATA(0x8001268c, 0x204)

ADDRESS(0x80042298, 0x18c)
s32 effect_collision_step(s32 radius, s32 angle, s32 step)
{
    KfEffectRecord *record = effect_state.current_record;
    VECTOR previous;
    s32 result;
    u8 next_kind;

    addVector(&record->position, &record->direction);
    result = effect_probe_collision_by_type(&record->position, radius, angle);
    if (record->midpoint_collision_enabled != 0) {
        effect_collision_motion_step.vx = record->direction.vx >> 1;
        effect_collision_motion_step.vy = record->direction.vy >> 1;
        effect_collision_motion_step.vz = record->direction.vz >> 1;
        if (result == 0) {
            previous.vx = record->position.vx - effect_collision_motion_step.vx;
            previous.vy = record->position.vy - effect_collision_motion_step.vy;
            previous.vz = record->position.vz - effect_collision_motion_step.vz;
            result = effect_probe_collision_by_type(&previous, radius, angle);
            if (result != 0) {
                copyVector(&effect_collision_motion_step, &record->direction);
            }
        }
    }
    next_kind = 2;
    if (KF_COLLISION_CACHE_LAYER == 0) {
        next_kind = 1;
    }
    record->map_layer_mask = next_kind;
    record->rotation.vz = ((u16)record->rotation.vz - step) & KF_ANGLE_WRAP_MASK;
    return result;
}

ADDRESS(0x80042424, 0xcc)
void effect_collision_backtrack(void)
{
    KfEffectRecord *record = effect_state.current_record;

    if (record->midpoint_collision_enabled != 0) {
        record->position.vx -= effect_collision_motion_step.vx;
        record->position.vy -= effect_collision_motion_step.vy;
        record->position.vz -= effect_collision_motion_step.vz;
    } else {
        record->position.vx -= record->direction.vx;
        record->position.vy -= record->direction.vy;
        record->position.vz -= record->direction.vz;
    }
    record->position.vx -= record->direction.vx;
    record->position.vy -= record->direction.vy;
    record->position.vz -= record->direction.vz;
}

ADDRESS(0x800424f0, 0x160)
void effect_spawn_radial_ring(s32 count, s32 radius, s32 vertical_angle, s32 arg3)
{
    KfEffectRecord *record = effect_state.current_record;
    SVECTOR direction;
    VECTOR position;
    s32 angle = rand();
    s32 angle_step = KF_ANGLE_FULL_TURN / count;

    position.vx = record->position.vx - record->direction.vx;
    position.vy = record->position.vy - record->direction.vy;
    position.vz = record->position.vz - record->direction.vz;
    direction.vy = vertical_angle;
    count--;
    while (count != -1) {
        direction.vx = (rcos(angle) * radius) >> KF_FIXED12_BITS;
        direction.vz = (rsin(angle) * radius) >> KF_FIXED12_BITS;
        angle += angle_step;
        count--;
        effect_construct_record(10, record->type | 3, 8, &position, &direction,
                      effect_state.current_index, arg3);
    }
}

ADDRESS(0x80042650, 0x3670)
void effect_update_dispatch(void)
{
    KfEffectRecord *record = effect_state.current_record;
    KfMagicRecord *magic = effect_state.current_magic;
    u32 initial_kind = record->kind;
    s32 initial_phase = record->phase;
    s32 collision;
    s32 step;
    s32 shared_multiplier;
    s32 shared_limit;
    s32 shared_increment;
    s32 shared_position_mode;
    s32 shared_motion_mode;
    s32 shared_motion_scale;
    s32 shared_motion_acceleration;
    s32 shared_motion_count;
    s32 shared_motion_layer;

    switch (initial_kind) {
    case 29:
    case 31:
    case 48: {
        VECTOR projected;
        VECTOR midpoint;
        s16 age;
        s32 prior_y;
        s32 acceleration;

        acceleration = 10;
        goto ballistic_update;
    case 30:
    case 47:
        acceleration = 5;
    ballistic_update:
        if (initial_phase != 0) {
            break;
        }
        /* These kinds overlay the record tail with a Y origin and age. */
        if (*(s16 *)&record->unknown_3c[6] == 0) {
            audio_play_spatial_range(5, &record->position, 110,
                                     28000, 29000, 0);
        }
        age = *(u16 *)&record->unknown_3c[6] + 1;
        *(u16 *)&record->unknown_3c[6] = age;
        prior_y = record->position.vy;
        projected.vy = (s16)*(u16 *)&record->unknown_3c[4] +
                       record->direction.vy * age +
                       ((acceleration * age * age) >> 1);
        projected.vx = record->position.vx + record->direction.vx;
        projected.vz = record->position.vz + record->direction.vz;
        collision = effect_probe_collision_by_type(&projected, 20, 20);
        if (collision == 0) {
            midpoint.vx = (projected.vx + record->position.vx) >> 1;
            midpoint.vy = (projected.vy + record->position.vy) >> 1;
            midpoint.vz = (projected.vz + record->position.vz) >> 1;
            collision = effect_probe_collision_by_type(&midpoint, 20, 20);
        }
        record->position.vx = projected.vx;
        record->position.vy = projected.vy;
        record->position.vz = projected.vz;
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        if (collision != 0) {
            effect_apply_current_magic_backstep(collision | 0x20000);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            u8 layer = 2;
            if (KF_COLLISION_CACHE_LAYER == 0) {
                layer = 1;
            }
            record->map_layer_mask = layer;
            vector_displacement_to_pitch_yaw(record->direction.vx,
                          record->position.vy - prior_y,
                          record->direction.vz,
                          (struct KfEulerAngles *)&record->rotation);
        }
        break;
    }
    case 7:
    case 49: {
    shared_growth_entry:
        if (initial_phase == 0) {
            collision = effect_collision_step(180, (s32)0x80000000, -300);
            if (collision == 0) {
                break;
            }
            effect_scatter_lower_bound(&record->position, 3, 400, 0x2000, 0x2000, 0x400);
            goto shared_growth_collision;
        }
        if (initial_phase >= 3) {
            record->type = KF_EFFECT_SLOT_FREE;
            break;
        }
        goto shared_growth_update;
    }
    case 13:
    case 32: {
        if (initial_phase != 0) {
            goto kind13_nonzero_phase;
        }
        collision = effect_collision_step(180, 0, -300);
        if (collision == 0) {
            goto kind13_no_collision;
        }
    shared_growth_collision:
        effect_collision_backtrack();
        effect_apply_current_magic_backstep(collision);
        record->phase = 1;
        goto shared_growth_update;
    kind13_no_collision:
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        break;
    kind13_nonzero_phase:
        if (initial_phase >= 3) {
            record->type = KF_EFFECT_SLOT_FREE;
            break;
        }
    shared_growth_update:
        record->animation_clip = initial_phase - 128;
        step = record->scale_x + 2048;
        record->scale_x = step;
        record->scale_y = step;
        record->scale_z = step;
        record->phase++;
        break;
    }
    case 23:
        if (initial_phase == 9) {
            KfActor *actor = &actor_state.actors[*(s16 *)&record->unknown_3c[4]];
            VECTOR vertex_offset;
            VECTOR actor_position;
            VECTOR old_position;
            VECTOR *position;

            if (actor->lifecycle != 1 || actor->target_type != 25) {
                record->type = KF_EFFECT_SLOT_FREE;
                break;
            }
            actor_sample_rotated_animation_vertex(actor, *(s16 *)&record->unknown_3c[6],
                          &vertex_offset);
            position = actor_resolve_group_position(actor, &actor_position);
            old_position = record->position;
            record->position.vx = vertex_offset.vx + position->vx;
            record->position.vy = vertex_offset.vy + position->vy;
            record->position.vz = vertex_offset.vz + position->vz;
            record->scale_x = (u16)record->scale_x + 512;
            if ((s16)record->scale_x > 0x1800) {
                record->scale_x = 0x1800;
            }
            record->scale_y = record->scale_z = record->scale_x;
            record->direction.vx = (u16)record->position.vx - (u16)old_position.vx;
            record->direction.vy = (u16)record->position.vy - (u16)old_position.vy;
            record->direction.vz = (u16)record->position.vz - (u16)old_position.vz;
            effect_spawn_motion(record, -1, -700, (s16)record->scale_x,
                           -300, 3, 8, 0);
            if (actor->animation_phase >= *(u16 *)&record->unknown_3c[8]) {
                actor_compute_target_direction(actor, &player_state.camera_position,
                               650, &record->position, &record->direction,
                               -1, 0x400, 5);
                record->phase = 0;
                record->updates_remaining = 50;
            }
            break;
        }
        effect_spawn_motion(record, -1, 0x100, (s16)record->scale_x,
                       -300, 2, 8, 0);
        goto shared_growth_entry;
    case 4:
        collision = effect_collision_step(180, 0, 0);
        if (collision == 0) {
            goto kind4_zero_collision;
        }
        if (collision & 0xf) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        if (record->unknown_3c[4] == 0) {
            record->unknown_3c[4] = 1;
            effect_apply_current_magic_backstep(collision);
        }
        goto kind4_rotate;
    kind4_zero_collision:
        record->unknown_3c[4] = 0;
    kind4_rotate:
        record->rotation.vy += 750;
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        break;
    case 34:
    case 35: {
        s32 vertical_step;

        vertical_step = 0;
        goto collision_kind25_update;
    case 25:
        vertical_step = -30;
    collision_kind25_update:
        collision = effect_collision_step(250, 100, vertical_step);
        if (collision != 0) {
            if (collision & 0xf) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            if (record->unknown_3c[4] == 0) {
                record->unknown_3c[4] = 1;
                effect_apply_current_magic_backstep(collision);
            }
        } else {
            record->unknown_3c[4] = 0;
        }
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        break;
    }
    case 42:
        if ((s8)record->unknown_3c[4] == 0) {
            effect_scale_step(0x3800, 0x1f8, 0x46, 0x800, 0x8000);
        } else {
            record->unknown_3c[4]--;
        }
        break;
    case 115:
        collision = effect_aim_and_move(0x258, 0x28, 0x24, 0xb4,
                                  0x168, 0x1000, 0x104, 0x800);
        if (collision == -1 || record->updates_remaining < 2) {
            effect_collision_backtrack();
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, 0, 0);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, 0, 1);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, 0, 2);
            effect_play_spatial_sound(record, 0x18);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        }
        break;
    case 113:
        collision = effect_collision_step(180, 360, 0);
        if (collision != 0 || record->updates_remaining < 2) {
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, 0, 0);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, 0, 1);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, 0, 2);
            effect_play_spatial_sound(record, 0x17);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        }
        break;
    case 46: {
        KfEffectRecord *selected =
            &effect_state.records[(s8)record->unknown_3c[5]];
        s32 height;
        s32 age;

        record->position.vx += record->direction.vx;
        record->position.vz += record->direction.vz;
        switch ((s8)record->unknown_3c[4]) {
        case 0: {
            collision = collision_query_world(record->position.vx, record->position.vy,
                                      record->position.vz, 10,
                                      (s16)record->scale_y, 0x30);
            effect_apply_current_magic_backstep(collision);
            collision_probe_floor_height(record->position.vx, selected->position.vy,
                          record->position.vz, 0, 0);
            record->position.vy = KF_COLLISION_CACHE_RESULT;
            height = KF_COLLISION_CACHE_RESULT - KF_COLLISION_CACHE_HEIGHT_LIMIT;
            if (height <= 32767) {
                record->scale_y = height;
            } else {
                record->scale_y = 32767;
            }
            if (selected->type == KF_EFFECT_SLOT_FREE) {
                record->unknown_3c[4] = 1;
                *(u16 *)&record->unknown_3c[6] = 0;
                *(u16 *)&record->unknown_32[0] = record->scale_y;
            }
            break;
        }
        case 1: {
            record->scale_y = fixed_lerp_q12(
                (s16)*(u16 *)&record->unknown_32[0], 0,
                (s16)*(u16 *)&record->unknown_3c[6]);
            age = *(u16 *)&record->unknown_3c[6] + 512;
            *(u16 *)&record->unknown_3c[6] = age;
            if ((s16)age >= 4096) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            break;
        }
        }
        break;
    }
    case 45:
        effect_scale_step(0x4000, (s16)*(u16 *)&record->unknown_3c[4],
                       0x46, 0x800, 0x8000);
        break;
    case 116: {
        VECTOR midpoint;

        midpoint.vx = record->position.vx + (record->direction.vx >> 1);
        midpoint.vy = record->position.vy + (record->direction.vy >> 1);
        midpoint.vz = record->position.vz + (record->direction.vz >> 1);
        record->position.vx += record->direction.vx;
        record->position.vy += record->direction.vy;
        record->position.vz += record->direction.vz;
        if (collision_query_shapes_with_layer_sample(midpoint.vx, midpoint.vy, midpoint.vz, 5, 10) ||
            collision_query_shapes_with_layer_sample(record->position.vx, record->position.vy,
                           record->position.vz, 5, 10)) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        effect_construct_record(10, record->type, 0x2d, &record->position, 0, 0x1a4);
        break;
    }
    case 117: {
        VECTOR elevated;
        s32 actor_index;

        record->position.vx += record->direction.vx;
        record->position.vy += record->direction.vy;
        record->position.vz += record->direction.vz;
        elevated.vx = record->position.vx;
        elevated.vy = record->position.vy + 5000;
        elevated.vz = record->position.vz;
        actor_index = actor_find_overlap_excluding_target_type3(elevated.vx, elevated.vy, elevated.vz,
                                    100, 10000);
        if (actor_index != -1) {
            effect_construct_record(10, record->type, 0x2d,
                           &actor_state.actors[actor_index].position, 0,
                           0x4ec);
        }
        effect_spawn_at_lower_bound(&elevated, 0x2000, 0x7fff, 10000);
        break;
    }
    case 40:
        collision = effect_collision_step(100, 200, 0);
        if (collision != 0) {
            effect_apply_current_magic_backstep(collision);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        }
        break;
    case 39: {
        s32 count;
        u32 flags;

        if (record->updates_remaining < 45) {
            goto kind39_spawn;
        }
        record->direction.vy = (u16)record->direction.vy + 10;
    case 38:
        if (effect_collision_step(100, 0, 0) != 0) {
            goto kind38_response;
        }
        goto kind38_finish;
    kind39_spawn:
        if (effect_aim_and_move(0x258, 0x28, 0x24, 0x32,
                          100, 0x1000, 0x104, 0x800) != -1) {
            goto kind38_finish;
        }
    kind38_response:
        {
            flags = KF_COLLISION_CACHE_FLAGS;
            if (flags & 0x10) {
                if (record->unknown_3c[4] == 0) {
                    effect_apply_current_magic_backstep(flags);
                } else {
                    record->unknown_3c[4] = 1;
                }
            } else {
                record->unknown_3c[4] = 0;
            }
            for (count = 11; count != -1; count--) {
                effect_spawn_motion(record, -1, -2, 0xc00, -90, 16, 14, 5,
                               0x200, -256, 0x200, -256, 0x200, -256);
            }
            if (KF_COLLISION_CACHE_FLAGS & 0xf) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
        }
    kind38_finish:
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        break;
    }
    case 50: {
        s32 distance;

        switch ((s8)record->unknown_3c[4]) {
        case 0:
            step = (u16)record->scale_z + 64;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
            player_sample_weapon_world_vertex(0, &record->position);
            if ((s16)record->scale_x >= 256) {
                record->unknown_3c[4] = 1;
                player_probe_view_target_and_vectors(1000, 0, &record->direction, &distance);
            }
            break;
        case 1:
            if (effect_collision_step(512, (s32)0x80000200, 0) != 0) {
                record->unknown_3c[4] = 2;
                effect_apply_radial_magic_damage(&record->position, 0, 0x400,
                               0x8000, 0x1000, 0x1000);
            }
            break;
        case 2:
            if ((s16)record->scale_x >= 512) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            step = (u16)record->scale_z + 64;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
            break;
        }
        break;
    }
    case 28: {
        s32 radius;
        u8 phase;

        radius = 250;
        goto kind_one_update;
    case 1:
        radius = 500;
    kind_one_update:
        phase = record->unknown_3c[4];
        if (phase == 2) {
            record->lighting_blend_q12 += 256;
            if (record->lighting_blend_q12 >= 4096) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            break;
        }
        if (phase == 1) {
            record->direction.vy += 13;
        }
        record->rotation.vx += 200;
        collision = effect_collision_step(radius, radius * 2, 250);
        if (collision != 0) {
            effect_apply_current_magic_backstep(collision);
            if (record->unknown_3c[4] == 1) {
                record->unknown_3c[4] = 2;
                record->render_queue_mode = 1;
                record->lighting_override_index = 0x42;
                record->lighting_blend_q12 = 0x400;
                break;
            }
            effect_collision_backtrack();
            record->unknown_3c[4] = 1;
            record->direction.vz = 0;
            record->direction.vx = 0;
            record->direction.vy = -100;
        }
        effect_spawn_at_lower_bound(&record->position, 0x4000, 0x4000, 500);
        break;
    }
    case 26:
    case 27:
        record->type = 0x21;
        record->rotation.vy += 100;
        effect_apply_radial_magic_damage(&record->position, 0, (s16)record->scale_x,
                       0x8000, 0x400, 0x1000);
        record->type = 0x24;
        switch ((s8)record->unknown_3c[4]) {
        case 0:
            if (effect_collision_step(100, 200, 0) != 0) {
                record->direction.vz = 0;
                record->direction.vy = 0;
                record->direction.vx = 0;
            }
            break;
        case 1:
            record->scale_z = (s16)record->scale_z - 512;
            if ((s16)record->scale_z <= 0) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            break;
        }
        break;
    case 111: {
        VECTOR spawn_position;
        SVECTOR spawn_direction;
        s32 spread;

        if ((s16)*(u16 *)&record->unknown_3c[4] == 0xff) {
            spawn_position.vx = record->position.vx;
            spawn_position.vy = record->position.vy;
            spawn_position.vz = record->position.vz;
            spawn_direction.vz = 0;
            spawn_direction.vy = 0;
            spawn_direction.vx = 0;
            spread = 1000;
        } else {
            const KfActor *actor =
                &actor_state.actors[(s16)*(u16 *)&record->unknown_3c[4]];

            spread = actor->collision_radius;
            spawn_position.vx = actor->position.vx;
            spawn_position.vz = actor->position.vz;
            spawn_position.vy = actor->position.vy;
            spawn_direction = *(const SVECTOR *)&actor->unknown_50;
        }
        spawn_position.vx += ((rand() * spread) >> 14) - spread;
        spawn_position.vz += ((rand() * spread) >> 14) - spread;
        spawn_position.vy -= 2000;
        effect_construct_record(10, record->type | 3, 0,
                       &spawn_position, &spawn_direction);
        break;
    }
    case 0:
        record->direction.vy += 20;
        if ((s16)record->scale_x < 0xc00) {
            step = (u16)record->scale_z + 0x100;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
        }
        collision = effect_collision_step(180, 0, -300);
        if (collision != 0) {
            if (record->unknown_3c[4] == 0 && rand() < 3000) {
                effect_apply_current_magic_backstep(collision);
            }
            if (collision & 0x10) {
                record->unknown_3c[4] = 1;
            }
            if (collision & 5) {
                if (initial_phase == 1) {
                    record->type = KF_EFFECT_SLOT_FREE;
                } else {
                    record->phase = 1;
                    record->direction.vy = -200;
                    record->position.vy = KF_COLLISION_CACHE_RESULT;
                }
            }
        }
        effect_spawn_at_lower_bound(&record->position, 0x400, 0x400, 500);
        record->rotation.vz += 2700;
        break;
    case 103:
    case 121: {
        s32 prior_phase = record->phase;
        s32 index;
        VECTOR spawn_position;
        s32 distance;

        if (prior_phase == 2) {
            record->scale_x = (s16)record->scale_x - 128;
            record->scale_y = record->scale_x;
            if ((s16)record->scale_x <= 0) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            break;
        }
        collision = effect_collision_step(50, (s32)0x80000000, -300);
        if (collision != 0) {
            if (collision & 0xf) {
                if (prior_phase == 0) {
                    effect_collision_backtrack();
                    record->direction.vz = 0;
                    record->direction.vx = 0;
                } else {
                    effect_collision_backtrack();
                    collision_probe_floor_height(record->position.vx,
                                  record->position.vy,
                                  record->position.vz, 50, 0);
                    if (KF_COLLISION_CACHE_RESULT <
                        KF_COLLISION_CACHE_LOWER_BOUND) {
                        spawn_position.vy = KF_COLLISION_CACHE_RESULT;
                    } else {
                        spawn_position.vy = KF_COLLISION_CACHE_LOWER_BOUND;
                    }
                    distance = spawn_position.vy - record->position.vy;
                    goto kind103_spawn;
                }
            }
            record->phase = 1;
        }
        if (prior_phase == 1) {
            record->direction.vy = (u16)record->direction.vy - 70;
            record->direction.vx = fixed_lerp_q12(
                0, (s16)record->direction.vx, 3000);
            record->direction.vz = fixed_lerp_q12(
                0, (s16)record->direction.vz, 3000);
            if (KF_COLLISION_CACHE_RESULT <
                KF_COLLISION_CACHE_LOWER_BOUND) {
                spawn_position.vy = KF_COLLISION_CACHE_RESULT;
            } else {
                spawn_position.vy = KF_COLLISION_CACHE_LOWER_BOUND;
            }
            distance = spawn_position.vy - record->position.vy;
            if (distance >= 7000) {
                goto kind103_spawn;
            }
        }
        goto kind103_after_spawn;
    kind103_spawn: {
        SVECTOR spawn_direction;

        record->phase = 2;
        spawn_position.vx = record->position.vx;
        spawn_position.vz = record->position.vz;
        /* Retail has no visible write to this stack direction. */
        effect_construct_record(10, record->type | 3,
                       initial_kind == 103 ? 104 : 122,
                       &spawn_position, &spawn_direction, distance);
        effect_construct_record(10, record->type, 2,
                       &spawn_position, &spawn_direction,
                       distance >> 1, distance >> 4, 0x800);
        effect_play_spatial_sound(record, 0x17);
        goto kind103_loop;
    }
    kind103_after_spawn:
        if (prior_phase == 0) {
            u16 count = *(u16 *)&record->unknown_3c[4];
            *(u16 *)&record->unknown_3c[4] = count - 1;
            if ((s16)count <= 0) {
                record->phase = 1;
            }
        }
    kind103_loop:
        for (index = 3; index != -1; index--) {
            SVECTOR random_direction;

            random_direction.vx = (rand() >> 7) - 128;
            random_direction.vy = (rand() >> 7) - 128;
            random_direction.vz = (rand() >> 7) - 128;
            effect_construct_record(10, 0, 101, &record->position,
                           &random_direction, 0x800, -128, 5, 18, 0);
        }
        break;
    }
    case 104:
    case 122:
        if (initial_phase < 3) {
            SVECTOR local_direction;
            s32 scale = (s16)record->scale_y;
            s32 root = SquareRoot12(scale * ((scale * scale) >> 12));

            root = SquareRoot12(root);
            /* Retail passes this stack vector without a visible write on
             * this kind entry. */
            effect_construct_record(10, record->type | 3,
                           initial_kind == 104 ? 11 : 54,
                           &record->position, &local_direction, root >> 3);
        }
        {
            s32 distance;

            record->animation_clip = (initial_phase & 1) - 128;
            if (record->phase < 9) {
                distance = fixed_vector3_length(
                    player_state.camera_position.vx - record->position.vx,
                    player_state.camera_position.vy - record->position.vy,
                    player_state.camera_position.vz - record->position.vz);
                if (distance < 32001) {
                    s32 strength = (((rsin(record->phase << 8) >> 1) + 1024) *
                                    (32000 - distance)) / 32000;

                    interpolate_collision_filter_rows(200, 180, 160, 32000, strength);
                }
            }
            goto shared_phase_increment;
        }
    case 11:
    case 54: {
        SVECTOR random_direction;

        effect_scale_step(0x3800, (s16)*(u16 *)&record->unknown_3c[4],
                       0x80, 0x400, 0x8000);
        random_direction.vx = (rand() >> 6) - 256;
        random_direction.vz = (rand() >> 6) - 256;
        random_direction.vy = -(rand() >> 7) - 128;
        effect_construct_record(10, 0, 101, &record->position, &random_direction,
                       0xc00, -128, 15, 18, 10);
        record->rotation.vy += 64;
        break;
    }
    case 51:
        effect_scale_step(0x1000, 0x400, 0x80, 0x800, 0x8000);
        break;
    case 52:
        effect_scale_step(0x1000, 0x800, 0x100, 0x800, 0x8000);
        break;
    case 118: {
        s32 child_kind;

        child_kind = 0x33;
        goto child_impact_update;
    case 119:
        child_kind = 0x34;
    child_impact_update:
        if (effect_collision_step(140, 0, -200) != 0) {
            effect_construct_record(10, record->type | 3,
                           child_kind,
                           &record->position, 0);
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    }
    case 2:
        step = (u16)record->scale_x + *(u16 *)&record->unknown_3c[6];
        record->scale_x = step;
        record->scale_z = step;
        if ((s16)record->scale_x >= 0x300) {
            VECTOR elevated;

            elevated.vx = record->position.vx;
            elevated.vy = record->position.vy + 1000;
            elevated.vz = record->position.vz;
            effect_apply_radial_magic_damage(&elevated,
                           ((s16)record->scale_x -
                            (s16)*(u16 *)&record->unknown_3c[6]) * 4,
                           (s16)record->scale_x * 4 - 1,
                           2000, 0x1000,
                           (s16)*(u16 *)&record->unknown_3c[8]);
        }
        if ((s16)record->scale_x > (s16)*(u16 *)&record->unknown_3c[4]) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 20:
        shared_multiplier = 0x4000;
        shared_limit = 0x100;
        shared_increment = 0x20;
        goto shared_scale_step;
    case 12: {
        s32 prior_phase = initial_phase;

        switch (prior_phase) {
        case 101:
            goto shared_phase_increment;
        case 100: {
            KfEffectRecord *child = effect_construct_record(
                10, record->type | 3, 12, &record->position,
                0, &record->rotation);

            child->phase = 101;
            goto kind12_reset;
        }
        case 102:
            goto kind12_reset;
        case 110:
            goto kind12_scale;
        }
        goto kind12_default;
    kind12_scale:
        shared_multiplier = 0x3800;
        shared_limit = 0x31f;
        shared_increment = 0x46;
        goto shared_scale_step;
    kind12_default:
        if (record->phase == 0) {
            effect_play_spatial_sound(record, 0x29);
        }
        record->phase++;
        collision = effect_aim_and_move(
            *(s16 *)&record->unknown_3c[4],
            *(s16 *)&record->unknown_3c[6],
            *(s16 *)&record->unknown_3c[8],
            0xa0,
            0, 6000, *(s16 *)&record->unknown_3c[10], 0x800);
        if (collision != -1) {
            goto kind12_collision;
        }
        {
            KfEffectRecord *child;

            effect_play_spatial_sound(record, 0x18);
            child = effect_construct_record(10, record->type | 3, 12,
                                  &record->position, 0,
                                  &record->rotation);
            child->phase = 101;
            goto kind12_reset;
        }
    kind12_reset:
        record->render_id = 0x11;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        record->phase = 110;
        record->type |= 3;
        goto kind12_scale;
    kind12_collision:
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        record->rotation.vz = (u16)record->rotation.vz + 128;
        shared_position_mode = 2;
        shared_motion_mode = 0x400;
        shared_motion_scale = 0xc00;
        shared_motion_acceleration = -300;
        shared_motion_count = 5;
        shared_motion_layer = 33;
        goto shared_spawn_motion;
    }
    case 100: {
        SVECTOR local_direction;
        s32 phase = initial_phase;

        if (phase < 100) {
            if ((u32)(phase - 4) < 67) {
                goto kind100_collision;
            }
            record->direction.vy = (u16)record->direction.vy + 10;
            if (effect_collision_step(100, 0, 0) == 0) {
                effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
                goto shared_phase_increment;
            }
            goto kind100_miss;
        kind100_collision:
            if (effect_aim_and_move(600, 30, 64, 100,
                              0, 0x1000, 360, 0x800) != -1) {
                goto kind100_success;
            }
        }
    kind100_miss:
        /* Retail passes this stack local without a visible write on this path. */
        effect_construct_record(10, record->type | 3, 20, &record->position,
                      &local_direction);
        effect_play_spatial_sound(record, 0x18);
        record->type = KF_EFFECT_SLOT_FREE;
        goto shared_phase_increment;
    kind100_success:
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        record->rotation.vz = (u16)record->rotation.vz + 128;
        effect_spawn_motion(record, 5, 0x400, 0x800, -150, 10, 8, 0);
        goto shared_phase_increment;
    }
    case 5: {
        s32 count;
        s32 step_size;
        s32 progress;
        s32 matches;
        s32 index;

        switch (initial_phase) {
        case 0: {
            const KfActor *actor;

            progress = 0;
            if (record->unknown_3c[5] != 0xff) {
                goto kind5_actor_count;
            }
        kind5_default_count:
            count = 16;
            step_size = 0;
            record->updates_remaining = 1;
            goto kind5_count_ready;
        kind5_actor_count:
            actor = &actor_state.actors[record->unknown_3c[5]];
            count = asset_vertex_count(actor->definition_id + 128,
                                       actor->animation_id);
            if (count == 0) {
                goto kind5_default_count;
            }
            if (count < 33) {
                step_size = 0x1000;
                goto kind5_count_ready;
            }
            step_size = (count << 12) / 32;
            count = 32;
        kind5_count_ready:
            *(u16 *)&record->unknown_3c[6] = count;
            record->unknown_3c[4] = count;
            for (index = count - 1; index != -1; index--) {
                effect_construct_record(10, record->type, 105,
                               &record->position, &record->direction,
                               effect_state.current_index,
                               record->unknown_3c[5], progress >> 12);
                progress += step_size;
            }
            record->phase = 1;
            break;
        }
        case 1: {
            const KfActor *actor;
            KfEffectRecord *child;

            if (record->updates_remaining >= 2 &&
                *(s16 *)&record->unknown_3c[6] != 0) {
                break;
            }
            matches = 0;
            child = effect_state.records;
            for (index = KF_EFFECT_CAPACITY - 1; index != -1;
                 child++, index--) {
                if (child->type == KF_EFFECT_SLOT_FREE ||
                    child->kind != 105 ||
                    child->unknown_3c[4] != effect_state.current_index) {
                    continue;
                }
                if (child->phase == 1) {
                    child->updates_remaining = 1;
                    matches++;
                } else {
                    child->phase = 2;
                }
            }
            actor = &actor_state.actors[record->unknown_3c[5]];
            if (record->unknown_3c[4] != 0) {
                step = (u16)effect_magic_power(record);
                actor_apply_magic_to_actor(record->unknown_3c[5], step,
                              magic->damage_components[0], magic->damage_components[1],
                              magic->damage_components[2], magic->damage_components[3],
                              magic->damage_components[4], magic->damage_components[5],
                              magic->damage_components[6], magic->damage_components[7],
                              (matches << 12) / record->unknown_3c[4],
                              (record->type & 0x30) | 2,
                              &actor->position);
            }
            record->type = KF_EFFECT_SLOT_FREE;
            break;
        }
        }
        break;
    }
    case 105: {
        KfActor *actor;
        VECTOR vertex_offset;
        VECTOR actor_position;
        VECTOR *position;
        VECTOR next_position;

        record->rotation.vz = (u16)record->rotation.vz + 800;
        if (initial_phase == 2) {
            goto kind105_phase2;
        }
        if (record->unknown_3c[5] == 0xff) {
            goto kind105_collision;
        }
        actor = &actor_state.actors[record->unknown_3c[5]];

        actor_sample_rotated_animation_vertex(actor, *(s16 *)&record->unknown_3c[6],
                      &vertex_offset);
        position = actor_resolve_group_position(actor, &actor_position);
        next_position.vx = vertex_offset.vx + position->vx;
        next_position.vy = vertex_offset.vy + position->vy;
        next_position.vz = vertex_offset.vz + position->vz;
        goto kind105_actor_phase;
    kind105_phase2:
        step = (u16)record->scale_x - 128;
        record->scale_x = step;
        record->scale_y = step;
        record->direction.vy = (u16)record->direction.vy + 5;
    kind105_collision:
        collision = effect_collision_step(100, 0, 0);
        if (collision != 0 && (collision & 0xf) != 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    kind105_actor_phase:
        if (initial_phase == 0) {
            goto kind105_phase0;
        }
        if (initial_phase == 1) {
            goto kind105_phase1;
        }
        break;
    kind105_phase0:
        collision = effect_target_motion(&next_position, 300, 50,
                                   500, 150, 10, 0);
        if (collision == -2) {
            KfEffectRecord *linked =
                &effect_state.records[record->unknown_3c[4]];
            s16 *linked_count = (s16 *)&linked->unknown_3c[6];
            if (*linked_count != 0) {
                --*linked_count;
            }
            record->phase = 1;
        } else {
            if (collision == -1 &&
                (KF_COLLISION_CACHE_FLAGS & 0xf) != 0) {
                KfEffectRecord *linked =
                    &effect_state.records[record->unknown_3c[4]];
                s16 *linked_count = (s16 *)&linked->unknown_3c[6];
                if (*linked_count != 0) {
                    --*linked_count;
                }
                record->type = KF_EFFECT_SLOT_FREE;
            }
            break;
        }
    kind105_phase1:
        /* Retail copies all four VECTOR words, including the pad;
         * no defining pad store is visible on this path. */
        record->position = next_position;
        if (record->updates_remaining < 2) {
            record->phase = 2;
            record->updates_remaining = (rand() >> 12) + 16;
            record->direction.vx = vertex_offset.vx >> 1;
            record->direction.vy = vertex_offset.vy >> 1;
            record->direction.vz = vertex_offset.vz >> 1;
        }
        break;
    }
    case 9: {
        u8 actor_index = record->unknown_3c[4];

        if (actor_index == 0xfe) {
            VECTOR target;

            target.vx = player_state.camera_position.vx;
            target.vy = player_state.camera_position.vy - 1600;
            target.vz = player_state.camera_position.vz;
            collision = effect_target_motion(&target, 400, 60,
                                       3000, 0, 10, (s32)0x80000000);
            if (collision != -1) {
                goto kind9_no_collision;
            }
        } else if (actor_index != 0xff) {
            VECTOR target;
            const KfActor *actor = &actor_state.actors[actor_index];

            target.vx = actor->position.vx;
            target.vy = actor->position.vy - (actor->collision_height >> 1);
            target.vz = actor->position.vz;
            collision = effect_target_motion(&target, 600, 50,
                                       0, 0, 10, (s32)0x80000000);
            if (collision != -1) {
                goto kind9_no_collision;
            }
        } else {
            goto kind9_unbound;
        }
    kind9_impact:
        {
            s32 index;

            effect_apply_current_magic_backstep(KF_COLLISION_CACHE_FLAGS);
            for (index = 11; index != -1; index--) {
                effect_spawn_motion(record, -1, -2, 0xc00, -90, 16, 8, 5,
                               0x200, -256, 0x200, -256, 0x200, -256);
            }
            effect_scatter_lower_bound(&record->position, 5, 0x400,
                           0x2000, 0x4000, 0x1000);
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    kind9_unbound:
        record->direction.vy = (u16)record->direction.vy + 10;
        collision = effect_collision_step(10, (s32)0x80000000, 0);
        if (collision != 0) {
            goto kind9_impact;
        }
    kind9_no_collision:
        effect_spawn_motion(record, -1, -3, 0xed8, -80,
                       6, 8, 0, 0x400);
        break;
    }
    case 53: {
        VECTOR target;
        s32 motion_scale;
        s32 result;
        s32 count;

        motion_scale = 7600;
        goto kind33_update;
    case 33:
        motion_scale = 3800;
    kind33_update:
        target.vx = player_state.camera_position.vx;
        target.vy = player_state.camera_position.vy - 1600;
        target.vz = player_state.camera_position.vz;
        result = effect_target_motion(&target, 400, 60, 3000, 0, 10, 0);
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        if (result == -1) {
            effect_apply_current_magic_backstep(KF_COLLISION_CACHE_FLAGS);
            for (count = 11; count != -1; count--) {
                effect_spawn_motion(record, -1, -2, 0xc00, -90, 16, 33, 5,
                               0x200, -256, 0x200, -256, 0x200, -256);
            }
            record->type = KF_EFFECT_SLOT_FREE;
        }
        effect_spawn_motion(record, -1, -3, motion_scale,
                       -motion_scale / 48, 6, 33, 0, 0x400);
        break;
    }
    case 106:
        if (initial_phase == 0) {
            goto kind106_phase0;
        }
        if (initial_phase == 1) {
            goto kind106_phase1;
        }
        break;
    kind106_phase0:
        record->direction.vy = (u16)record->direction.vy + 20;
        collision = effect_collision_step(140, (s32)0x80000000, -300);
        if (collision != 0) {
            record->position.vx -= record->direction.vx;
            record->position.vy -= record->direction.vy;
            record->position.vz -= record->direction.vz;
            effect_spawn_radial_ring(1, 0, -400, 60);
            effect_spawn_radial_ring(6, 60, -330, 56);
            effect_spawn_radial_ring(8, 140, -170, 40);
            effect_play_spatial_sound(record, 0x25);
            record->unknown_3c[4] = 15;
            record->phase = 1;
            record->render_flags = 0;
        } else {
            effect_spawn_motion(record, -1, -3, 6000, -800,
                           6, 8, 0, -1024);
        }
        break;
    kind106_phase1:
        if (record->unknown_3c[4] == 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 8:
        if (initial_phase != 0) {
            if (initial_phase == 1) {
                goto kind8_phase_one;
            }
            break;
        }
        {
            VECTOR next;
            s32 first_collision;

            record->direction.vy = (u16)record->direction.vy +
                                   *(u16 *)&record->unknown_3c[6];
            next.vx = record->position.vx + (s16)record->direction.vx;
            next.vy = record->position.vy + (s16)record->direction.vy;
            next.vz = record->position.vz + (s16)record->direction.vz;
            first_collision = effect_probe_collision_by_type(&next, 140, (s32)0x80000000);
            if (first_collision != 0 && (first_collision & 0xf) != 0) {
                next.vx = record->position.vx;
                next.vz = record->position.vz;
                if ((effect_probe_collision_by_type(&next, 140, (s32)0x80000000) & 0xf) == 0) {
                    goto kind8_reset_axes;
                }
                if ((s16)record->direction.vy < 0) {
                    record->direction.vy = 0;
                    next.vy = record->position.vy;
                } else {
                    KfEffectRecord *parent =
                        &effect_state.records[record->unknown_3c[4]];

                    if (effect_spawn_at_lower_bound(&next, 0x2000, 0x2000, 500) == 0) {
                        s32 dx;
                        s32 dz;
                        s32 distance;

                        record->phase = 1;
                        record->render_flags = 9;
                        record->render_id = 20;
                        record->rotation.vz = 0;
                        dx = record->position.vx - parent->position.vx;
                        dz = record->position.vz - parent->position.vz;
                        record->direction.vx = vector_xz_to_angle(dx, dz);
                        distance = fixed_vector2_length(dx, dz);
                        record->direction.vz = distance;
                        record->direction.vy = distance;
                        record->scale_z = 0;
                        record->scale_y = 0;
                        record->scale_x = 0x400;
                        *(u16 *)record->unknown_32 =
                            ((rand() * 1500) >> 15) + 256;
                        record->updates_remaining = 100;
                        break;
                    }
                    if (parent->unknown_3c[4] != 0) {
                        parent->unknown_3c[4]--;
                    }
                    record->type = KF_EFFECT_SLOT_FREE;
                    break;
                }
            }
            goto kind8_apply_next;
        kind8_reset_axes:
            record->direction.vz = 0;
            record->direction.vx = 0;
        kind8_apply_next:
            record->position.vx = next.vx;
            record->position.vy = next.vy;
            record->position.vz = next.vz;
            record->map_layer_mask = KF_COLLISION_CACHE_LAYER != 0 ? 2 : 1;
            record->rotation.vz = ((u16)record->rotation.vz + 300) & 0xfff;
            shared_position_mode = -1;
            shared_motion_mode = 0x400;
            shared_motion_scale = 0x1000;
            shared_motion_acceleration = -500;
            shared_motion_count = 2;
            shared_motion_layer = 8;
            goto shared_spawn_motion;
        }
    shared_spawn_motion:
        effect_spawn_motion(record, shared_position_mode, shared_motion_mode,
                       shared_motion_scale, shared_motion_acceleration,
                       shared_motion_count, shared_motion_layer, 0);
        break;
    kind8_phase_one: {
            KfEffectRecord *parent =
                &effect_state.records[record->unknown_3c[4]];
            struct KfVecXZi forward;

            angle_to_forward_xz((s16)record->direction.vx, &forward);
            record->position.vx = parent->position.vx +
                                  (((s16)record->direction.vz * forward.x) >> 12);
            record->position.vz = parent->position.vz +
                                  (((s16)record->direction.vz * forward.z) >> 12);
            record->direction.vz = fixed_lerp_q12(
                (s16)record->direction.vy, 0, (s16)record->scale_z);
            record->scale_y = ((u32)(rsin((s16)record->scale_z >> 1) * 25)) >> 5;
            record->scale_z = (u16)record->scale_z + 64;
            if ((s16)record->scale_z >= (s16)*(u16 *)record->unknown_32) {
                s32 collision_kind;

                *(u16 *)record->unknown_32 += 2048;
                collision_kind = collision_query_world(
                    record->position.vx, record->position.vy,
                    record->position.vz, 256, (s16)record->scale_y, 144);
                if (collision_kind != 0) {
                    effect_apply_current_magic(collision_kind, 5000, 0);
                }
            }
            if ((s16)record->scale_z >= 4096) {
                record->type = KF_EFFECT_SLOT_FREE;
                if (parent->unknown_3c[4] != 0) {
                    parent->unknown_3c[4]--;
                }
                break;
            }
            if (rand() < 2048) {
                effect_spawn_motion(record, -1, -2, 0x1800, -96,
                               12, 39, -17, 128, -64, 64,
                               -100, 128, -64);
            }
        }
        break;
    case 10: {
        switch (initial_phase) {
        case 0:
            goto kind10_phase0;
        case 1:
            goto kind10_phase1;
        case 5:
            goto kind10_reset;
        default:
            goto shared_phase_increment;
        }
    kind10_phase0:
        collision = effect_collision_step(250, (s32)0x80000000, 0);
        if (collision != 0 || record->updates_remaining < 2) {
            KfEffectRecord *child;

            effect_scatter_lower_bound(&record->position, 8, 400,
                           0x2000, 0x8000, 0x400);
            child = effect_construct_record(10, record->type | 3, 10,
                                  &record->position, 0,
                                  &record->rotation);
            child->phase = 2;
            effect_play_spatial_sound(record, 0x17);
            goto kind10_reset;
        }
        goto kind10_normal;
    kind10_reset:
        record->phase = 1;
        record->render_id = 0xb;
        record->lighting_override_index = 0x44;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        record->updates_remaining = -1;
        record->type |= 3;
        goto kind10_phase1;
    kind10_normal:
        record->animation_phase_q12 = ((u16)record->animation_phase_q12 + 128) & 0xfff;
        if (record->unknown_3c[5] != 0) {
            VECTOR origin;
            VECTOR target;
            SVECTOR direction;
            const KfActor *actor;

            effect_sample_world_vertex(record, 0, &origin,
                           (const SVECTOR *)&record->scale_x);
            actor = &actor_state.actors[record->unknown_3c[4]];
            target.vx = actor->position.vx;
            target.vy = actor->position.vy - (actor->collision_height >> 1);
            target.vz = actor->position.vz;
            vector_direction_scaled(&origin, &target, 800, &direction);
            effect_construct_record(10, record->type, 7, &origin, &direction);
            record->unknown_3c[5]--;
        } else if (rand() < 2048) {
            s32 distance;
            KfActor *actor = actor_find_best_in_cone(
                &record->position, record->rotation.vy,
                record->rotation.vx, 25000, 800, 800,
                &distance, 512);

            if (actor != 0) {
                record->unknown_3c[5] = 5;
                record->unknown_3c[4] = actor - actor_state.actors;
            }
        }
        if (rand() < 1024) {
            s32 distance;

            if (actor_find_best_in_cone(&record->position, record->rotation.vy,
                               record->rotation.vx, 30000, 800, 800,
                               &distance, 512) != 0) {
                effect_spawn_zero_direction(record, 0x2f);
                effect_spawn_zero_direction(record, 0x32);
            }
        }
        break;
    kind10_phase1:
        shared_multiplier = 0x4000;
        shared_limit = 0x800;
        shared_increment = 75;
    shared_scale_step:
        effect_scale_step(shared_multiplier, shared_limit, shared_increment,
                       0x400, 0x8000);
        record->rotation.vy = (u16)record->rotation.vy + 64;
        break;
    }
    shared_phase_increment:
        record->phase++;
        break;
    case 6: {
        switch (initial_phase) {
        case 0: {
            s32 index;
            KfEffectRecord *child;
            u8 parent_index;

            for (index = 0; index < 8; index++) {
                child = effect_construct_record(
                    10, 0, 107, &record->position, &record->direction,
                    &record->rotation, index);

                if (index == 0) {
                    child->render_id = 0x17;
                }
                parent_index = effect_state.current_index;
                child->unknown_3c[5] = index;
                child->unknown_3c[4] = parent_index;
            }
            child->render_id = 0x18;
            record->phase = 1;
        }
        /* The spawn tick also runs the phase-one update. */
        case 1:
            if (record->updates_remaining < 3) {
            kind6_phase3:
                record->phase = 3;
                record->updates_remaining = -1;
                record->unknown_3c[9] = 24;
                break;
            }
            if (effect_aim_and_move(250, 25, 32, 200,
                              0, 0x400, 100, 0x800) == -1) {
                record->updates_remaining = -1;
                if (KF_COLLISION_CACHE_FLAGS != 0x10) {
                    goto kind6_phase3;
                }
                {
                    u8 actor_index;
                    KfActor *actor;

                    effect_apply_current_magic(0x10010, 5000, 0);
                    actor_index = *(u8 *)&KF_COLLISION_CACHE_ACTOR_INDEX;
                    record->unknown_3c[10] = actor_index;
                    actor = &actor_state.actors[record->unknown_3c[10]];
                    if (actor->target_type == 2 || actor->target_type == 3) {
                        record->render_flags = 1;
                        record->render_id = 22;
                        record->phase = 2;
                        record->unknown_3c[9] = 0;
                        record->rotation.vz = 0;
                        record->rotation.vx = 0;
                        record->rotation.pad = -vector_xz_to_angle(
                            record->position.vx - actor->position.vx,
                            record->position.vz - actor->position.vz);
                        goto phase_two;
                    }
                }
                goto kind6_phase3;
            } else {
                KfEffectTrailRow *rows =
                    *(KfEffectTrailRow **)&record->unknown_3c[4];
                KfEffectTrailRow *row;
                u8 frame = record->unknown_3c[8] + 1;
                s32 angle = record->unknown_3c[9] << 4;

                record->unknown_3c[8] = frame;
                if (frame >= 24) {
                    record->unknown_3c[8] = 0;
                }
                row = &rows[record->unknown_3c[8]];
                record->unknown_3c[9] += 8;
                row->position.vx = record->position.vx;
                row->position.vz = record->position.vz;
                row->position.vy = record->position.vy + (rsin(angle) >> 3);
                row->rotation.vy = record->rotation.vy;
                row->rotation.vz = record->rotation.vz;
                row->rotation.vx = record->rotation.vx + (rcos(angle) >> 4);
            }
            break;
        case 2:
        phase_two: {
            KfActor *actor = &actor_state.actors[record->unknown_3c[10]];
            VECTOR scratch;
            VECTOR *position;
            u8 frame = record->unknown_3c[8] + 1;
            s32 actor_extent;

            actor->unknown_28 |= 0x800;
            actor_extent = actor->collision_radius;
            record->unknown_3c[8] = frame;
            if (frame >= 24) {
                record->unknown_3c[8] = 0;
            }
            position = actor_resolve_group_position(actor, &scratch);
            record->position = *position;
            record->position.vy -= actor->collision_height >> 1;
            if (record->unknown_3c[9] < 17) {
                s32 scale = fixed_lerp_q12(
                    0, actor_extent, record->unknown_3c[9] << 9);

                record->scale_z = scale;
                record->scale_x = scale;
                record->scale_y = fixed_lerp_q12(
                    0, actor->collision_height, record->unknown_3c[9] * 350);
            } else if (record->unknown_3c[9] >= 60) {
                record->phase = 4;
                break;
            }
            {
                KfEffectTrailRow *rows =
                    *(KfEffectTrailRow **)&record->unknown_3c[4];
                KfEffectTrailRow *row = &rows[record->unknown_3c[8]];
                s32 radius = ((s32)actor_extent * 25 << 8) >> 12;
                s32 angle = (s16)record->rotation.pad;

                record->unknown_3c[9]++;
                record->rotation.pad = angle + 100000 / actor_extent;
                row->position.vx = record->position.vx +
                                   ((rsin(angle) * radius) >> 12);
                row->position.vz = record->position.vz +
                                   ((rcos(angle) * radius) >> 12);
                row->position.vy = record->position.vy +
                                   (rsin(angle << 1) >> 4);
                row->rotation.vy = -angle - 1024;
                row->rotation.vz = 0;
                row->rotation.vx = rcos(angle << 1) >> 4;
            }
            break;
        }
        case 3: {
            u8 frame = ++record->unknown_3c[8];

            if (frame >= 24) {
                record->unknown_3c[8] = 0;
            }
            if (record->unknown_3c[9] != 0) {
                record->unknown_3c[9]--;
                break;
            }
            goto kind6_release_actor;
        }
        case 4: {
            s32 index;

            for (index = 31; index != -1; index--) {
                effect_spawn_motion(record, -1, -2, 0xc00, -90,
                               16, 14, 5, 0x200, -256, 0x200,
                               -320, 0x200, -256);
            }
            goto kind6_release_actor;
        }
        }
        break;
    kind6_release_actor: {
        KfActor *actor = &actor_state.actors[record->unknown_3c[10]];

        actor->unknown_28 &= ~0x800;
        record->type = KF_EFFECT_SLOT_FREE;
        break;
    }
    }
    case 107: {
        KfEffectRecord *selected =
            &effect_state.records[record->unknown_3c[4]];
        const u8 *snapshots;
        const u8 *snapshot;
        s32 frame_index;

        if (initial_phase == 0 && selected->phase >= 3) {
            record->phase = 1;
            record->updates_remaining = record->unknown_3c[5] * 3;
        }
        if (selected->phase == 4) {
            record->type = KF_EFFECT_SLOT_FREE;
            break;
        }
        frame_index = selected->unknown_3c[8] -
                      record->unknown_3c[5] * 3;
        if (frame_index < 0) {
            frame_index += 24;
        }
        /* The selected variant's +0x40 word points to 24-byte snapshots;
         * its allocation and complete tail layout are not yet owned. */
        snapshots = *(const u8 *const *)&selected->unknown_3c[4];
        snapshot = snapshots + frame_index * 24;
        record->position = *(const VECTOR *)snapshot;
        record->rotation = *(const SVECTOR *)(snapshot + 16);
        break;
    }
    case 101:
        record->direction.vy = (u16)record->direction.vy +
                               *(u16 *)&record->unknown_3c[6];
        record->position.vx += record->direction.vx;
        record->position.vz += record->direction.vz;
        step = record->scale_x + *(u16 *)&record->unknown_3c[4];
        record->scale_x = step;
        record->scale_z = step;
        record->scale_y = step;
        record->position.vy += record->direction.vy;
        break;
    case 102:
        record->phase++;
        record->scale_y = (rsin(record->phase << 8) *
                           (s16)*(u16 *)&record->unknown_3c[4]) >> 12;
        if (record->phase >= 8) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 15:
        if (player_state.defense_boost_timer == 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        record->rotation.vy += 128;
        break;
    case 17:
        if (player_state.attack_boost_timer == 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        record->rotation.vy -= 128;
        break;
    case 16:
        if (record->updates_remaining == 4) {
            player_state.vitals.current_hp += 60;
            if (player_state.vitals.current_hp >
                player_state.vitals.maximum_hp) {
                player_state.vitals.current_hp =
                    player_state.vitals.maximum_hp;
            }
        }
        interpolate_collision_filter_rows(0xe6, 0xc8, 0xa0, 0x59d8,
                       rsin(record->updates_remaining << 8));
        break;
    case 14: {
        VECTOR spawn_position;
        SVECTOR spawn_direction;
        KfEffectRecord *spawned;

        if (record->updates_remaining == 12) {
            player_state.poison_timer = 0;
        }
        spawn_position.vx = (rand() >> 5) - 512;
        spawn_position.vy = (rand() >> 8) + 200;
        spawn_position.vz = 0x400;
        spawn_direction.vz = 0;
        spawn_direction.vx = 0;
        spawn_direction.vy = 0;
        spawned = effect_construct_record(10, 0, 101, &spawn_position,
                                &spawn_direction, 700, -30, 10, 14, -10);
        spawned->map_layer_mask = 3;
        spawned->render_flags = 14;
        interpolate_collision_filter_rows(160, 180, 220, 18000,
                       rsin(record->updates_remaining << 7));
        break;
    }
    case 19: {
        VECTOR spawn_position;
        SVECTOR spawn_direction;
        KfEffectRecord *spawned;
        s32 count;

        if (record->updates_remaining == 12) {
            player_state.vitals.current_hp += 150;
            if (player_state.vitals.current_hp >
                player_state.vitals.maximum_hp) {
                player_state.vitals.current_hp =
                    player_state.vitals.maximum_hp;
            }
            player_cap_status_components(7);
        }
        spawn_direction.vz = 0;
        spawn_direction.vx = 0;
        spawn_direction.vy = 0;
        for (count = 3; count != 0; count--) {
            spawn_position.vx = (rand() >> 5) - 512;
            spawn_position.vy = (rand() >> 8) + 200;
            spawn_position.vz = 0x400;
            spawned = effect_construct_record(10, 0, 101, &spawn_position,
                                    &spawn_direction, 700, -30, 10, 18, -10);
            spawned->map_layer_mask = 3;
            spawned->render_flags = 14;
        }
        interpolate_collision_filter_rows(240, 240, 160, 18000,
                       rsin(record->updates_remaining << 7));
        break;
    }
    case 22:
        record->direction.vy = (u16)record->direction.vy + 10;
        collision = effect_collision_step(100, (s32)0x80000000, -300);
        if (collision != 0) {
            effect_apply_current_magic_backstep(collision);
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    case 3:
        effect_scale_step(0x4000, 0x200, 0x40, 0xc00, 0x8000);
        break;
    case 114: {
        s32 count;

        record->position.vx += record->direction.vx;
        record->position.vy += record->direction.vy;
        record->position.vz += record->direction.vz;
        collision = collision_query_world(record->position.vx, record->position.vy,
                                  record->position.vz, 10, 10, 176);
        if (collision == 0) {
            /* The kind-114 constructor saves its original Y at +0x44.
             * Other effect kinds use this tail differently. */
            collision_probe_floor_height(record->position.vx,
                          *(s32 *)&record->unknown_3c[8],
                          record->position.vz, 0, 0);
            if (record->position.vy < KF_COLLISION_CACHE_RESULT) {
                goto kind114_particles;
            }
            record->position.vy = KF_COLLISION_CACHE_RESULT;
        }
        effect_construct_record(10, record->type | 3, 3, &record->position, 0, 0);
        record->type = KF_EFFECT_SLOT_FREE;
        break;
    kind114_particles:
        for (count = 1; count != -1; count--) {
            effect_spawn_motion(record, (rand() * 20) >> 15,
                           -3, 6000, -200, 5, 39, 0, 0x100);
        }
        break;
    }
    case 24: {
        VECTOR target;
        s32 result;
        s32 count;

        target.vx = player_state.camera_position.vx;
        target.vy = player_state.camera_position.vy - 1600;
        target.vz = player_state.camera_position.vz;
        result = effect_target_motion(&target, 300, 40, 2000, 0, 10, 0);
        if (result == -1) {
            effect_apply_current_magic_backstep(KF_COLLISION_CACHE_FLAGS);
            for (count = 11; count != -1; count--) {
                effect_spawn_motion(record, -1, -2, 0xc00, -90, 16, 14, 5,
                               0x200, -256, 0x200, -256, 0x200, -256);
            }
            record->type = KF_EFFECT_SLOT_FREE;
        } else if (rand() < 16384) {
            effect_construct_record(10, 0, 0x6d, &record->position,
                           &record->direction, effect_state.current_index);
        }
        break;
    }
    case 109:
        effect_target_motion(
            &effect_state.records[record->unknown_3c[4]].position,
            500, 15, -1, 0, 0, -1);
        break;
    case 120: {
        record->direction.vy = (u16)record->direction.vy + 20;
        if ((s16)record->scale_x < 0x1000) {
            step = (u16)record->scale_z + 0x200;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
        }
        if (rand() >= 400) {
            collision = effect_collision_step(180, 0, -300);
            if (collision == 0 || (collision & 5) == 0) {
                goto kind120_rotate;
            }
            record->position.vy = KF_COLLISION_CACHE_RESULT;
        }
        if (rand() < 8192) {
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, 0, 0);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, 0, 1);
            effect_play_spatial_sound(record, 0x18);
        }
        record->type = KF_EFFECT_SLOT_FREE;
    kind120_rotate:
        record->rotation.vz += 2700;
        break;
    }
    }
    return;

}
