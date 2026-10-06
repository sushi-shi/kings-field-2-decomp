#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/effect.h>
#include <kf/game/collision_cache.h>
#include <kf/lib/null.h>
#include <kf/lib/math.h>
#include <kf/game/animation.h>
#include <kf/game/actor.h>
#include <kf/game/player.h>
#include <psyq/libc.h>
#include <kf/game/asset.h>

DATA(0x8006d704, 0x4, ".sdata")
u32 effect_trail_next_slot = 0;

DATA(0x8006d708, 0x8, ".sdata")
static SVECTOR effect_zero_direction = {0, 0, 0, 0};

DATA(0x8009a5a8, 0x4, ".bss")
s32 effect_kind102_sound_cooldown_frame;

DATA(0x8019b6a8, 0x2a8c, ".bss")
KfEffectState effect_state;

DATA(0x801c7068, 0x8, ".bss")
SVECTOR effect_collision_motion_step;

DATA(0x801d9628, 0x900, ".bss")
KfEffectTrailRow effect_trail_rows[4][24];

RODATA(0x8001249c, 0x3f4)

enum {
    EFFECT_SPATIAL_VOLUME = 110,
    EFFECT_SPATIAL_MAX_DISTANCE = 0x6d60,
    EFFECT_SPATIAL_ATTENUATION_DISTANCE = 0x7148,
    EFFECT_COLLISION_HEIGHT_MASK = 0xfff,
    EFFECT_COLLISION_TARGET_MASK = 7,
    EFFECT_COLLISION_TARGET_PLAYER = 1,
    EFFECT_COLLISION_TARGET_ACTORS = 2,
    EFFECT_COLLISION_TARGET_BOTH = 3,
    EFFECT_COLLISION_TARGET_SHAPES_ONLY = 4,
    EFFECT_IMPACT_HOLD_ACTOR_ANIMATION = 0x10000,
    EFFECT_IMPACT_COUNTS_AS_PHYSICAL = 0x20000,
    EFFECT_ANIMATION_PHASE_MASK = KF_FIXED12_ONE - 1
};

ADDRESS(0x8003fa2c, 0x3c)
KfAudioPlaybackResult effect_play_spatial_sound(
    KfEffectRecord *effect, s32 sound)
{
    return audio_play_spatial_range(sound, &effect->position,
        EFFECT_SPATIAL_VOLUME, EFFECT_SPATIAL_MAX_DISTANCE,
        EFFECT_SPATIAL_ATTENUATION_DISTANCE, 0);
}

ADDRESS(0x8003fa68, 0x12c)
s32 effect_probe_collision_by_type(const VECTOR *position, s32 radius,
    s32 height_flags)
{
    KfEffectRecord *record = effect_state.current_record;
    s32 y;
    if (record->cooldown == 0) {
        y = position->vy + ((height_flags & EFFECT_COLLISION_HEIGHT_MASK) >> 1);
        switch (record->type & EFFECT_COLLISION_TARGET_MASK) {
        case EFFECT_COLLISION_TARGET_ACTORS:
            return collision_query_world(position->vx, y, position->vz, radius,
                height_flags, KF_COLLISION_QUERY_SHAPES | KF_COLLISION_QUERY_ACTORS |
                                  KF_COLLISION_QUERY_MAP_OBJECTS);
        case EFFECT_COLLISION_TARGET_PLAYER:
            return collision_query_world(position->vx, y, position->vz, radius,
                height_flags, KF_COLLISION_QUERY_SHAPES | KF_COLLISION_QUERY_MAP_OBJECTS |
                                  KF_COLLISION_QUERY_PLAYER);
        case EFFECT_COLLISION_TARGET_BOTH:
            return collision_query_world(position->vx, y, position->vz, radius,
                height_flags, KF_COLLISION_QUERY_SHAPES | KF_COLLISION_QUERY_ACTORS |
                                  KF_COLLISION_QUERY_MAP_OBJECTS | KF_COLLISION_QUERY_PLAYER);
        case EFFECT_COLLISION_TARGET_SHAPES_ONLY:
            return collision_query_world(position->vx, y, position->vz, radius,
                height_flags, KF_COLLISION_QUERY_SHAPES);
        }
    } else {
        record->cooldown--;
        return 0;
    }
}

enum { EFFECT_FIXED_MAGIC_POWER = 5 };

ADDRESS(0x8003fb94, 0x218)
void effect_dispatch_magic_impact(s32 kind, s32 record_type, s32 radius, u16 power,
                   u8 damage_multiplier_tenths, u16 magic_06, u16 magic_08, u16 magic_0a,
                   u16 magic_04, u16 magic_0c, u16 magic_0e, u16 magic_10,
                   u16 magic_12, u16 magic_14, const VECTOR *position)
{
    s32 options = kind & 0xf0000;
    kind &= ~0xf0000;

    if (kind == KF_COLLISION_HIT_PLAYER) {
        player_apply_damage(magic_06, magic_08, magic_0a, magic_04,
                      magic_0c, magic_0e, magic_10, magic_12,
                      magic_14, radius, damage_multiplier_tenths, position);
    } else if (kind == KF_COLLISION_HIT_ACTOR) {
        s32 actor_index = KF_COLLISION_CACHE_ACTOR_INDEX;
        KfActor *actor = &actor_state.actors[actor_index];
        KfTargetGroup *group = &actor_state.target_groups[actor->group_index];

        if (position != NULL) {
            s32 angle = vector_xz_to_angle(
                actor->position.vx - position->vx,
                actor->position.vz - position->vz);
            if (!angle_within_tolerance(actor->rotation.y, angle + 0x800,
                                        group->actor_facing_tolerance)) {
                return;
            }
        }

        record_type &= 0x30;
        if (options & EFFECT_IMPACT_COUNTS_AS_PHYSICAL) {
            record_type |= 1;
        } else {
            record_type |= 2;
        }
        actor_apply_magic_to_actor(KF_COLLISION_CACHE_ACTOR_INDEX, power, magic_06,
                      magic_08, magic_0a, magic_0c, magic_0e, magic_10,
                      magic_12, magic_14, radius, record_type, position);
        if (options & EFFECT_IMPACT_HOLD_ACTOR_ANIMATION) {
            actor->flags |= KF_ACTOR_FLAG_EFFECT_ANIMATION_HOLD;
        }
    }
}

ADDRESS(0x8003fdac, 0x24)
int effect_magic_power(KfEffectRecord *effect)
{
    if ((effect->type & KF_EFFECT_USE_PLAYER_MAGIC) != 0) {
        return player_state.magic;
    }
    return EFFECT_FIXED_MAGIC_POWER;
}


ADDRESS(0x8003fdd0, 0xe0)
void effect_apply_current_magic(s32 kind, s32 radius, const VECTOR *position)
{
    KfEffectRecord *record = effect_state.current_record;
    const KfMagicRecord *magic = effect_state.current_magic;
    u16 power = effect_magic_power(record);

    effect_dispatch_magic_impact(kind, record->type, radius, power, record->damage_multiplier_tenths,
                  magic->damage_components[0], magic->damage_components[1], magic->damage_components[2],
                  magic->player_status_flags, magic->damage_components[3], magic->damage_components[4],
                  magic->damage_components[5], magic->damage_components[6], magic->damage_components[7],
                  position);
}

ADDRESS(0x8003feb0, 0x68)
void effect_apply_current_magic_backstep(s32 kind)
{
    const KfEffectRecord *record = effect_state.current_record;
    VECTOR position;

    position.vx = record->position.vx - (record->direction.vx << 3);
    position.vy = record->position.vy - (record->direction.vy << 3);
    position.vz = record->position.vz - (record->direction.vz << 3);
    effect_apply_current_magic(kind, 5000, &position);
}


ADDRESS(0x8003ff18, 0x1a8)
void effect_apply_radial_magic_damage(VECTOR *position, s32 start, s32 end,
                                      s32 arg3, s32 arg4, s32 arg5)
{
    KfEffectRecord *record = effect_state.current_record;
    const KfMagicRecord *magic = effect_state.current_magic;

    if (record->type & EFFECT_COLLISION_TARGET_PLAYER) {
        player_apply_radial_damage(position, start, end, arg3, arg4,
                      magic->damage_components[0], magic->damage_components[1], magic->damage_components[2],
                      magic->player_status_flags, magic->damage_components[3], magic->damage_components[4],
                      magic->damage_components[5], magic->damage_components[6], magic->damage_components[7],
                      arg5, record->damage_multiplier_tenths);
    }
    if (record->type & EFFECT_COLLISION_TARGET_ACTORS) {
        u16 power = effect_magic_power(record);

        actor_apply_area_magic(position, start, end, arg3, arg4,
                      power, magic->damage_components[0],
                      magic->damage_components[1], magic->damage_components[2], magic->damage_components[3],
                      magic->damage_components[4], magic->damage_components[5], magic->damage_components[6],
                      magic->damage_components[7], arg5, (record->type & 0x30) | 2);
    }
}


ADDRESS(0x800400c0, 0xf4)
void effect_sample_rotated_vertex(KfEffectRecord *record, s32 mode, VECTOR *output, const SVECTOR *scale)
{
    struct KfEulerAngles angles;
    SVECTOR offset;

    animation_sample_vertex(record->render_id + 40, record->animation_clip,
                  record->animation_phase_q12, mode, &offset);
    offset.vx = offset.vx * scale->vx >> KF_FIXED12_BITS;
    offset.vy = offset.vy * scale->vy >> KF_FIXED12_BITS;
    offset.vz = offset.vz * scale->vz >> KF_FIXED12_BITS;

    angles.x = record->rotation.vx;
    angles.y = record->rotation.vy + KF_ANGLE_HALF_TURN;
    angles.z = record->rotation.vz;
    vector_rotate_yxz(&angles, &offset, output);
}

ADDRESS(0x800401b4, 0x6c)
void effect_sample_world_vertex(KfEffectRecord *record, s32 mode, VECTOR *position, const SVECTOR *scale)
{
    effect_sample_rotated_vertex(record, mode, position, scale);
    addVector(position, &record->position);
}


ADDRESS(0x80040220, 0x44)
KfEffectRecord *effect_pool_find_free(void)
{
    KfEffectRecord *record = effect_state.records;
    u16 i = KF_EFFECT_CAPACITY;

    do {
        if (record->type == KF_EFFECT_SLOT_FREE) {
            return record;
        }
        record++;
    } while (--i != 0);
    return NULL;
}

ADDRESS(0x80040264, 0x40)
void effect_pool_initialize_scaled(KfEffectRecord *record, u8 render_id, u16 scale)
{
    record->render_flags = 5;
    record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
    record->base_render_id = render_id;
    record->render_id = render_id;
    record->render_queue_mode = 1;
    record->lighting_override_index = 0x43;
    record->lighting_blend_q12 = 0x1000;
    record->scale_z = scale;
    record->scale_y = scale;
    record->scale_x = scale;
}

ADDRESS(0x800402a4, 0x64)
void effect_pool_initialize_fixed(KfEffectRecord *record, u8 render_id)
{
    record->render_flags = 14;
    record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
    record->base_render_id = render_id;
    record->render_id = render_id;
    record->render_queue_mode = 1;
    record->lighting_override_index = 0x44;
    record->lighting_blend_q12 = 0x1000;
    record->scale_y = 0x100;
    record->scale_z = 0x100;
    record->scale_x = 0x100;
    record->position.vx = 160;
    record->position.vy = 120;
    record->rotation.vz = 0;
    record->rotation.vy = 0;
    record->rotation.vx = 0;
    record->position.vz = 0;
}

ADDRESS(0x80040308, 0x13e4)
KfEffectRecord *effect_construct_record(u8 damage_multiplier_tenths, u8 type, u8 kind,
                              const VECTOR *position,
                              const SVECTOR *direction, ...)
{
    KfEffectRecord *record;
    s32 length_squared;
    u16 third_parameter;
    /* O32 stacks the fifth argument; optional words follow its home slot. */
    s32 *va = (s32 *)&direction;

    record = effect_pool_find_free();

    if (record == NULL) {
        goto finish;
    }
    record->type = type;
    record->kind = kind;
    if (position != NULL) {
        record->position = *position;
    }
    record->map_layer_mask = 3;
    if (direction != NULL) {
        record->direction = *direction;
    } else {
        record->direction.vz = 0;
        record->direction.vy = 0;
        record->direction.vx = 0;
    }
    record->phase = 0;
    record->damage_multiplier_tenths = damage_multiplier_tenths;
    record->scale_z = 0x1000;
    record->scale_y = 0x1000;
    record->scale_x = 0x1000;
    record->rotation.vz = 0;
    record->rotation.vy = 0;
    record->rotation.vx = 0;
    record->animation_phase_q12 = 0;
    record->unknown_05 = 0;
    record->render_flags = 1;
    if ((record->type & KF_EFFECT_USE_PLAYER_MAGIC) != 0 &&
        player_state.death_state == 1) {
        record->cooldown = 8;
    } else {
        record->cooldown = 1;
    }
    record->lighting_override_index = 0xff;
    record->render_queue_mode = 0xff;
    record->updates_remaining = -1;
    length_squared = (s32)record->direction.vx * record->direction.vx +
        (s32)record->direction.vy * record->direction.vy +
        (s32)record->direction.vz * record->direction.vz;
    record->lighting_blend_q12 = 0;
    if (length_squared >= 810001) {
        record->midpoint_collision_enabled = 1;
    } else {
        record->midpoint_collision_enabled = 0;
    }

    switch (record->kind) {
    case 7:
    case 49:
        effect_pool_initialize_scaled(record, 0x2d, 0x1800);
        record->updates_remaining = 50;
        record->midpoint_collision_enabled = 1;
        effect_play_spatial_sound(record, 0x23);
        break;
    case 32:
        effect_pool_initialize_scaled(record, 0x21, 0x1800);
        record->updates_remaining = 50;
        break;
    case 4:
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0x1f;
        record->render_id = 0x1f;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->updates_remaining = 0x2d;
        record->cache_tail.payload.collision_latch.impact_handled = 0;
        record->scale_z = 0x32c8;
        record->scale_y = 0x32c8;
        record->scale_x = 0x32c8;
        record->midpoint_collision_enabled = 1;
        effect_play_spatial_sound(record, 0x20);
        break;
    case 28:
        record->scale_z = 0x800;
        record->scale_y = 0x800;
        record->scale_x = 0x800;
        /* fall through */
    case 1:
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x20;
        record->render_id = 0x20;
        record->cache_tail.payload.kind1.collision_stage = 0;
        record->updates_remaining = 70;
        effect_play_spatial_sound(record, 0x1b);
        break;
    case 26:
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x24;
        record->render_id = 0x24;
        record->cache_tail.payload.kind1.collision_stage = 0;
        record->updates_remaining = 70;
        record->scale_z = 600;
        record->scale_y = 600;
        record->scale_x = 600;
        record->cache_tail.payload.raw[0] = 0;
        break;
    case 27:
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x25;
        record->render_id = 0x25;
        record->cache_tail.payload.raw[0] = 0;
        record->updates_remaining = 70;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        break;
    case 111: {
        u16 value = va[1];
        record->render_flags = 0;
        record->updates_remaining = 50;
        record->cache_tail.payload.kind111.actor_index = value;
        effect_play_spatial_sound(record, 0x21);
        break;
    }
    case 13:
        effect_pool_initialize_scaled(record, 0x21, 0x1000);
        record->updates_remaining = 50;
        effect_play_spatial_sound(record, 0x2a);
        break;
    case 0:
        effect_pool_initialize_scaled(record, 0xe, 0x200);
        record->updates_remaining = 50;
        record->cache_tail.payload.collision_latch.impact_handled = 0;
        break;
    case 25: {
        const SVECTOR *angles;

        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0xd;
        record->render_id = 0xd;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->updates_remaining = 45;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->rotation.vz = rand() >> KF_RANDOM_ANGLE_SHIFT;
        record->cache_tail.payload.collision_latch.impact_handled = 0;
        break;
    }
    case 5: {
        KfEffectKind5Fanout *fanout = &record->cache_tail.payload.kind5;
        u8 parameter;

        record->render_flags = 0;
        parameter = va[1];
        record->updates_remaining = 70;
        fanout->actor_index = parameter;
        effect_play_spatial_sound(record, 0x21);
        break;
    }
    case 105: {
        KfEffectKind105Attachment *attachment =
            &record->cache_tail.payload.kind105;

        effect_pool_initialize_scaled(record, 0xe, 0x1000);
        record->rotation.vz = rand();
        record->direction.vx += (rand() >> 9) - 32;
        record->direction.vy += (rand() >> 9) - 32;
        record->direction.vz += (rand() >> 9) - 32;
        attachment->parent_index = va[1];
        attachment->actor_index = va[2];
        attachment->vertex_index = va[3];
        record->updates_remaining = 70;
        break;
    }
    case 9: {
        KfEffectKind9Target *target = &record->cache_tail.payload.kind9;

        effect_pool_initialize_scaled(record, 8, 0x1000);
        record->direction.vx += (rand() >> 8) - 64;
        record->direction.vy += (rand() >> 8) - 64;
        record->direction.vz += (rand() >> 8) - 64;
        target->actor_index = va[1];
        record->updates_remaining = 100;
        record->cooldown = 3;
        effect_play_spatial_sound(record, 0x26);
        break;
    }
    case 53:
        effect_pool_initialize_scaled(record, 0x21, 0x2000);
        goto randomize_33_53;
    case 33:
        effect_pool_initialize_scaled(record, 0x21, 0x1000);
    randomize_33_53: {
        s32 random_z;

        record->direction.vx += (rand() >> 8) - 64;
        record->direction.vy += (rand() >> 8) - 64;
        random_z = rand();
        record->updates_remaining = 100;
        record->cooldown = 3;
        record->direction.vz += (random_z >> 8) - 64;
        effect_play_spatial_sound(record, 0x21);
        break;
    }
    case 106:
        effect_pool_initialize_scaled(record, 8, 0x2000);
        record->direction.vy -= 100;
        effect_play_spatial_sound(record, 0x24);
        record->updates_remaining = 100;
        break;
    case 8: {
        KfEffectKind8State *kind8 =
            &record->cache_tail.payload.kind8;

        effect_pool_initialize_scaled(record, 8, 0x1000);
        kind8->parent_index = va[1];
        kind8->vertical_step = va[2];
        record->updates_remaining = 50;
        break;
    }
    case 10: {
        KfEffectKind10Targeting *targeting =
            &record->cache_tail.payload.kind10;
        const SVECTOR *angles;

        record->render_flags = 1;
        record->animation_clip = 0;
        record->render_queue_mode = 1;
        record->base_render_id = 0x15;
        record->render_id = 0x15;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->scale_z = 3000;
        record->scale_y = 3000;
        record->scale_x = 3000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        targeting->emissions_remaining = 0;
        record->updates_remaining = 150;
        effect_play_spatial_sound(record, 0x27);
        break;
    }
    case 6: {
        const SVECTOR *angles;
        KfEffectTrailRow *rows;
        s32 index;
        u32 slot;

        record->render_flags = 0;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0;
        record->render_id = 0;
        record->scale_z = 0x1000;
        record->scale_y = 0x1000;
        record->scale_x = 0x1000;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        slot = effect_trail_next_slot;
        rows = record->cache_tail.payload.trail.rows = effect_trail_rows[slot];
        effect_trail_next_slot = (slot + 1) & 3;
        for (index = 23; index != -1; index--, rows++) {
            rows->position = record->position;
            rows->rotation = record->rotation;
        }
        record->cache_tail.payload.trail.frame_index = 0;
        record->cache_tail.payload.trail.phase_counter = 0;
        record->updates_remaining = 150;
        effect_play_spatial_sound(record, 0x22);
        break;
    }
    case 107: {
        const SVECTOR *angles;

        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0x23;
        record->render_id = 0x23;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->scale_z = 0x1000;
        record->scale_y = 0x1000;
        record->scale_x = 0x1000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        break;
    }
    case 121:
        effect_pool_initialize_scaled(record, 0x2e, 0x1000);
        goto initialize_103_121;
    case 103:
        effect_pool_initialize_scaled(record, 0xf, 0x1000);
    initialize_103_121:
        record->updates_remaining = 100;
        record->cache_tail.payload.kind103.remaining = va[1];
        effect_play_spatial_sound(record, 0x28);
        break;
    case 122:
        effect_pool_initialize_scaled(record, 0x2f, 0x1000);
        record->scale_y = va[1];
        record->updates_remaining = 15;
        break;
    case 104:
        effect_pool_initialize_scaled(record, 0x10, 0x1000);
        record->scale_y = va[1];
        record->updates_remaining = 15;
        break;
    case 54: {
        s32 value;
        s32 render_id;

        render_id = 0x30;
        goto initialize_11_54;
    case 11:
        render_id = 0x11;
    initialize_11_54:

        record->base_render_id = render_id;
        record->render_id = render_id;
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        value = va[1];
        record->cache_tail.payload.scale_step_argument.scale_step = value / 4;
        break;
    }
    case 118:
    case 119: {
        const SVECTOR *angles;

        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x13;
        record->render_id = 0x13;
        record->updates_remaining = 0x23;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        effect_play_spatial_sound(record, 0x20);
        break;
    }
    case 51:
    case 52:
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x25;
        record->render_id = 0x25;
        record->cache_tail.payload.raw[0] = 0;
    zero_scale_27_51_52:
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        break;
    case 2: {
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 9;
        record->render_id = 9;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->scale_z = 0;
        record->scale_x = 0;
        record->cache_tail.payload.kind2.max_scale = va[1];
        record->cache_tail.payload.kind2.scale_step = va[2];
        third_parameter = va[3];
        record->cache_tail.payload.kind2.radial_damage_parameter = third_parameter;
        effect_play_spatial_sound(record, 0x1e);
        break;
    }
    case 20:
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        break;
    case 12: {
        KfEffectKind12Aim *aim =
            &record->cache_tail.payload.kind12;
        const SVECTOR *angles;

        record->render_flags = 1;
        record->render_queue_mode = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x22;
        record->render_id = 0x22;
        record->lighting_override_index = 0x49;
        record->lighting_blend_q12 = 0x1000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->scale_z = 30000;
        record->scale_y = 30000;
        record->scale_x = 30000;
        aim->max_length = va[2];
        aim->scale = va[3];
        aim->turn_step = va[4];
        aim->close_scale = va[5];
        record->updates_remaining = va[6];
        break;
    }
    case 100: {
        const SVECTOR *angles;

        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x22;
        record->render_id = 0x22;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->rotation.vy += (rand() >> 7) - 128;
        record->rotation.vx += (rand() >> 7) - 128;
        record->rotation.vz = 0;
        effect_play_spatial_sound(record, 0x29);
        break;
    }
    case 42: {
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0x11;
        record->render_id = 0x11;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        record->cache_tail.payload.kind42.ticks_remaining = va[1];
        break;
    }
    case 113:
    case 115: {
        const SVECTOR *angles;

        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0xd;
        record->render_id = 0xd;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->updates_remaining = 45;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->scale_z = 5000;
        record->scale_y = 5000;
        record->scale_x = 5000;
        effect_play_spatial_sound(record, 0x29);
        break;
    }
    case 46: {
        KfEffectKind46State *kind46 = &record->cache_tail.payload.kind46;
        u8 parameter;

        effect_pool_initialize_scaled(record, 0x10, 0x1000);
        record->scale_y = 0;
        kind46->phase = 0;
        parameter = va[1];
        record->direction.vy = 0;
        kind46->linked_effect_index = parameter;
        break;
    }
    case 45: {
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0x30;
        record->render_id = 0x30;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        effect_play_spatial_sound(record, 0x17);
        record->cache_tail.payload.scale_step_argument.scale_step = va[1];
        break;
    }
    case 116:
        record->render_flags = 0;
        record->updates_remaining = 20;
        break;
    case 117:
        record->base_render_id = 0x31;
        record->render_id = 0x31;
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->updates_remaining = 50;
        goto initialize_angles_34_35_117;
    case 40: {
        const SVECTOR *angles;

        record->base_render_id = 0xa;
        record->render_id = 0xa;
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->updates_remaining = 50;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        effect_play_spatial_sound(record, 0x32);
        break;
    }
    case 38:
    case 39: {
        const SVECTOR *angles;
        s32 random_x;

        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x2c;
        record->render_id = 0x2c;
        record->updates_remaining = 50;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->rotation.vy += (rand() >> 6) - 256;
        random_x = rand();
        record->rotation.vz = 0;
        record->cache_tail.payload.raw[0] = 0;
        record->scale_z = 0x2000;
        record->scale_y = 0x2000;
        record->scale_x = 0x2000;
        record->rotation.vx += (random_x >> 6) - 256;
        effect_play_spatial_sound(record, 0x20);
        break;
    }
    case 34: {
        const SVECTOR *angles;
        s32 render_id;

        render_id = 0x28;
        goto initialize_34_35;
    case 35:
        render_id = 0x29;
    initialize_34_35:
        record->base_render_id = render_id;
        record->render_id = render_id;
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->updates_remaining = 45;
    initialize_angles_34_35_117:
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->cache_tail.payload.raw[0] = 0;
        effect_play_spatial_sound(record, 0x20);
        break;
    }
    case 50:
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        record->cache_tail.payload.kind50.stage = 0;
        effect_play_spatial_sound(record, 0x18);
        break;
    case 101: {
        KfEffectKind101Motion *motion =
            &record->cache_tail.payload.kind101;
        s32 scale = va[1];
        s32 render_id = va[4];

        effect_pool_initialize_scaled(record, render_id, scale);
        motion->scale_step = *(u16 *)(va + 2);
        record->updates_remaining = *(u16 *)(va + 3);
        motion->vertical_step = *(u16 *)(va + 5);
        break;
    }
    case 102: {
        u16 scale;
        s32 volume;
        s16 slot;

        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0xc;
        record->render_id = 0xc;
        record->scale_y = 0;
        scale = va[1];
        record->scale_z = scale;
        record->scale_x = scale;
        record->cache_tail.payload.kind102.amplitude = va[2];
        if ((s32)(effect_kind102_sound_cooldown_frame - cd_state.frame_count) >= 0) {
            break;
        }
        volume = record->cache_tail.payload.kind102.amplitude / 90;
        effect_kind102_sound_cooldown_frame = cd_state.frame_count + 30;
        if (volume >= 128) {
            volume = 127;
        }
        slot = audio_state.voices.params[236].vab_slot_index;
        if (slot != -1 && audio_state.vab_slots[slot].vab_id != -1 &&
            audio_state.vab_slots[slot].vab_id != 0xfe) {
            audio_play_spatial_range(0xec, &record->position, volume, 28000,
                                     0x7148, 0);
            break;
        }
        slot = audio_state.voices.params[239].vab_slot_index;
        if (slot != -1 && audio_state.vab_slots[slot].vab_id != -1 &&
            audio_state.vab_slots[slot].vab_id != 0xfe) {
            audio_play_spatial_range(0xef, &record->position, volume, 28000,
                                     0x7148, 0);
        }
        break;
    }
    case KF_EFFECT_KIND_DEFENSE_BOOST:
        effect_pool_initialize_fixed(record, 0x19);
        break;
    case KF_EFFECT_KIND_ATTACK_BOOST:
        effect_pool_initialize_fixed(record, 0x1a);
        break;
    case 16:
        record->render_flags = 0;
        record->updates_remaining = 8;
        audio_play_sound(0x2b, 120);
        break;
    case 14:
    case 19:
        record->render_flags = 0;
        record->updates_remaining = 16;
        audio_play_sound(0x2b, 120);
        break;
    case 22:
        effect_pool_initialize_scaled(record, 0x1e, 0x1000);
        record->cooldown = 3;
        record->updates_remaining = 30;
        record->direction.vx += (rand() >> 9) - 32;
        record->direction.vy += (rand() >> 9) - 32;
        record->direction.vz += (rand() >> 9) - 32;
        break;
    case 3:
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->render_queue_mode = 1;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->lighting_override_index = 0x44;
        record->lighting_blend_q12 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        effect_play_spatial_sound(record, 0x1f);
        break;
    case 114: {
        VECTOR candidate_position;

        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->base_render_id = 0x26;
        record->render_id = 0x26;
        record->updates_remaining = 45;
        record->cache_tail.payload.kind114.origin_y = record->position.vy;
        candidate_position.vx = record->position.vx + (rand() >> 5) - 512;
        candidate_position.vz = record->position.vz + (rand() >> 5) - 512;
        if (collision_query_shapes_with_layer_sample(candidate_position.vx, record->position.vy,
                          candidate_position.vz, 10, 10) != 0) {
            candidate_position.vx = record->position.vx;
            candidate_position.vz = record->position.vz;
        }
        record->position.vx = candidate_position.vx - 2730;
        record->position.vz = candidate_position.vz - 2730;
        record->direction.vx = 100;
        record->direction.vz = 100;
        record->position.vy -= 16384;
        record->direction.vy = 600;
        record->scale_z = 0x4000;
        record->scale_y = 0x4000;
        record->scale_x = 0x4000;
        break;
    }
    case 48: {
        const SVECTOR *angles;
        s32 render_id;

        render_id = 0x2a;
        goto setup_render_id;
    case 47:
        render_id = 0x2b;
        goto setup_render_id;
    case 30:
        render_id = 0x1d;
        goto setup_render_id;
    case 29:
    case 31:
        render_id = 0x1c;
    setup_render_id:
        record->base_render_id = render_id;
        record->render_flags = 1;
        record->animation_clip = KF_EFFECT_STATIC_OBJECT_ZERO;
        record->lighting_override_index = 0xff;
        record->render_id = record->base_render_id;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->rotation.vz = 0;
        record->cache_tail.payload.ballistic.age = 0;
        break;
    }
    case 23: {
        effect_pool_initialize_scaled(record, 8, 0x400);
        record->direction.vz = 0;
        record->direction.vy = 0;
        record->direction.vx = 0;
        record->phase = 9;
        record->cache_tail.payload.kind23.actor_index = va[1];
        record->cache_tail.payload.kind23.vertex_index = va[2];
        third_parameter = va[3];
        record->cache_tail.payload.kind23.release_animation_phase = third_parameter;
        effect_play_spatial_sound(record, 0x26);
        break;
    }
    case 24:
        record->render_flags = 0;
        record->updates_remaining = 70;
        break;
    case 109: {
        KfEffectKind109Target *target = &record->cache_tail.payload.kind109;

        effect_pool_initialize_scaled(record, 0xe, 0x400);
        record->updates_remaining = 20;
        target->effect_index = va[1];
        record->direction.vx += (rand() >> 8) - 64;
        record->direction.vy += (rand() >> 8) - 64;
        record->direction.vz += (rand() >> 8) - 64;
        break;
    }
    case 120:
        effect_pool_initialize_scaled(record, 50, 0);
        record->updates_remaining = 100;
        if (rand() < 4096) {
            effect_play_spatial_sound(record, 0x21);
        }
        break;
    /* The remaining kinds reach the retail table's free-slot sentinel.
     * The table dispatch itself remains indirect. */
    default:
        record->type = KF_EFFECT_SLOT_FREE;
        break;
    }
finish:
    return record;
}

ADDRESS(0x800416ec, 0x90)
void effect_rotate_scale_offset_y(const SVECTOR *offset, VECTOR *output, s16 angle, s32 scale)
{
    SVECTOR scaled;
    SVECTOR rotation;
    MATRIX matrix;

    setVector(&scaled,
              (offset->vx * scale) >> KF_FIXED12_BITS,
              0,
              (offset->vz * scale) >> KF_FIXED12_BITS);
    setVector(&rotation, 0, angle, 0);
    RotMatrix(&rotation, &matrix);
    ApplyMatrix(&matrix, &scaled, output);
}

ADDRESS(0x8004177c, 0x1e0)
s32 effect_move_probe(s32 scale, s32 max_length, s32 probe_radius,
                      s32 probe_height_flags, SVECTOR *motion)
{
    KfEffectRecord *record = effect_state.current_record;
    s32 length;

    vector3s_scale_shift12(scale, motion);
    addVector(&record->direction, motion);
    length = fixed_vector3_length(record->direction.vx, record->direction.vy,
                                  record->direction.vz);
    if (length > max_length) {
        record->direction.vx = record->direction.vx * max_length / length;
        record->direction.vy = record->direction.vy * max_length / length;
        record->direction.vz = record->direction.vz * max_length / length;
    }
    addVector(&record->position, &record->direction);
    if (probe_height_flags == -1) {
        return 0;
    }
    return -!!effect_probe_collision_by_type(&record->position, probe_radius, probe_height_flags);
}

ADDRESS(0x8004195c, 0x1b8)
s32 effect_aim_and_move(s32 max_length, s32 scale, s32 turn_step,
                        s32 probe_radius, s32 probe_height_flags, s32 proximity,
                        s32 close_scale, s32 target_filter)
{
    KfEffectRecord *record = effect_state.current_record;
    VECTOR target_position;
    struct KfEulerAngles target_angles;
    SVECTOR motion;
    s32 distance;
    KfActor *target;

    if (!(record->type & KF_EFFECT_USE_PLAYER_MAGIC)) {
        goto player_target;
    }
    target = actor_find_best_in_cone(&record->position, record->rotation.vy,
                           record->rotation.vx, 24000, target_filter,
                           target_filter, &distance, 0);
    if (target == NULL) {
        goto move;
    }
    target_position.vx = target->position.vx;
    target_position.vy = target->position.vy - (target->collision_height >> 1);
    target_position.vz = target->position.vz;

aim:
    vector_displacement_to_pitch_yaw(target_position.vx - record->position.vx,
                  target_position.vy - record->position.vy,
                  target_position.vz - record->position.vz,
                  &target_angles);
    record->rotation.vx = angle_approach(record->rotation.vx,
                                        target_angles.x, turn_step);
    record->rotation.vy = angle_approach(record->rotation.vy,
                                        target_angles.y, turn_step);
    goto move;

player_target:
    distance = player_distance_to_point_in_cone(
        &record->position, record->rotation.vy, 24000, 0x1000);
    if (distance != -1) {
        target_position.vx = player_state.camera_position.vx;
        target_position.vy = player_state.camera_position.vy - KF_PLAYER_CAMERA_EYE_OFFSET;
        target_position.vz = player_state.camera_position.vz;
        goto aim;
    }

move:
    pitch_yaw_to_forward_vector((const struct KfEulerAngles *)&record->rotation,
                                &motion);
    if (distance >= 0 && distance <= proximity) {
        scale = close_scale;
    }
    return effect_move_probe(scale, max_length, probe_radius, probe_height_flags, &motion);
}

ADDRESS(0x80041b14, 0x1bc)
s32 effect_target_motion(const VECTOR *target, s32 max_length, s32 scale,
                         s32 settle_distance, s32 min_distance, s32 probe_radius,
                         s32 probe_height_flags)
{
    KfEffectRecord *record = effect_state.current_record;
    VECTOR delta;
    SVECTOR motion;
    s32 length;

    delta.vx = target->vx - record->position.vx;
    delta.vy = target->vy - record->position.vy;
    delta.vz = target->vz - record->position.vz;
    length = fixed_vector3_length(delta.vx, delta.vy, delta.vz);
    if (length <= min_distance) {
        return -2;
    }
    if (length <= settle_distance) {
        record->direction.vx = fixed_lerp_q12(0, record->direction.vx, 0xc00);
        record->direction.vy = fixed_lerp_q12(0, record->direction.vy, 0xc00);
        record->direction.vz = fixed_lerp_q12(0, record->direction.vz, 0xc00);
    }
    motion.vx = (delta.vx << KF_FIXED12_BITS) / length;
    motion.vy = (delta.vy << KF_FIXED12_BITS) / length;
    motion.vz = (delta.vz << KF_FIXED12_BITS) / length;
    return effect_move_probe(scale, max_length, probe_radius, probe_height_flags, &motion);
}

ADDRESS(0x80041cd0, 0xac)
void effect_scale_step(s32 multiplier, s32 limit, s32 increment, s32 arg3, s32 arg5)
{
    KfEffectRecord *record = effect_state.current_record;
    s32 scaled_size;

    record->scale_y = record->scale_z = record->scale_x += increment;
    scaled_size = (multiplier * (s16)record->scale_x) >> 12;
    effect_apply_radial_magic_damage(&record->position,
                  scaled_size - ((multiplier * increment) >> 12),
                  scaled_size - 1, arg5, arg3, 0x1000);
    if ((s16)record->scale_x >= limit) {
        record->type = KF_EFFECT_SLOT_FREE;
    }
}

ADDRESS(0x80041d7c, 0x90)
void effect_spawn_zero_direction(KfEffectRecord *record, s32 mode)
{
    VECTOR position;
    SVECTOR direction;
    KfEffectRecord *spawned;

    effect_sample_world_vertex(record, mode, &position, (const SVECTOR *)&record->scale_x);
    direction.vx = record->rotation.vx;
    direction.vy = record->rotation.vy + (rand() >> 7) - 128;
    direction.vz = record->rotation.vz;
    spawned = effect_construct_record(10, record->type, 100, &position,
                            &effect_zero_direction, &direction);
    if (spawned != NULL) {
        spawned->phase = 2;
    }
}

ADDRESS(0x80041e0c, 0x88)
s32 effect_spawn_at_lower_bound(const VECTOR *position, s32 arg1, s32 arg2,
                                s32 vertical_window)
{
    s32 lower_bound = KF_COLLISION_CACHE_LOWER_BOUND;
    VECTOR spawn_position;
    SVECTOR direction;

    if (position->vy < lower_bound) {
        return 0;
    }
    if (position->vy > vertical_window + lower_bound) {
        return 0;
    }

    spawn_position.vx = position->vx;
    spawn_position.vz = position->vz;
    spawn_position.vy = lower_bound;
    effect_construct_record(10, 0, 0x66, &spawn_position, &direction, arg1, arg2);
    return 1;
}

ADDRESS(0x80041e94, 0x298)
void effect_spawn_motion(KfEffectRecord *record, s32 position_mode,
                   s32 motion_mode, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, ...)
{
    s32 *args;
    SVECTOR motion;
    VECTOR position_delta;
    VECTOR transformed;

    /* O32 places the remaining effect-mode operands after arg7. */
    args = &arg7;
    if (position_mode == -1) {
        position_delta.vx = position_delta.vy = position_delta.vz = 0;
    } else {
        effect_sample_rotated_vertex(record, position_mode, &position_delta,
                      (const SVECTOR *)&record->scale_x);
    }

    switch (motion_mode) {
    case -1:
        effect_sample_rotated_vertex(record, args[2], &transformed,
                      (const SVECTOR *)args[3]);
        copyVector(&motion, &transformed);
        motion_mode = args[1];
        break;
    case -2:
        motion.vx = (rand() * args[1] >> 15) + (u16)args[2];
        motion.vy = (rand() * args[3] >> 15) + (u16)args[4];
        motion.vz = (rand() * args[5] >> 15) + (u16)args[6];
        goto spawn;
    case -3:
        motion.vx = motion.vy = motion.vz = 0;
        motion_mode = args[1];
        break;
    default:
        pitch_yaw_to_forward_vector((const struct KfEulerAngles *)&record->rotation,
                                    &motion);
        vector3s_scale_shift12(30, &motion);
        motion.vx = -(u16)motion.vx;
        motion.vy = -(u16)motion.vy;
        motion.vz = -(u16)motion.vz;
        break;
    }

    motion.vx = (u16)motion.vx + ((record->direction.vx * motion_mode) >> 12);
    motion.vy = (u16)motion.vy + ((record->direction.vy * motion_mode) >> 12);
    motion.vz = (u16)motion.vz + ((record->direction.vz * motion_mode) >> 12);

spawn:
    addVector(&position_delta, &record->position);
    effect_construct_record(10, 0, 101, &position_delta, &motion,
                  arg3, arg4, arg5, arg6, arg7);
}

ADDRESS(0x8004212c, 0x16c)
s32 effect_scatter_lower_bound(const VECTOR *origin, s32 count, s32 spread,
                  s32 scale_x, s32 scale_z, s32 variation)
{
    s32 lower_bound = KF_COLLISION_CACHE_LOWER_BOUND;
    s32 offset_x = 0;
    s32 offset_z = 0;

    if (origin->vy >= lower_bound) {
        if (origin->vy > lower_bound + 500) {
            return 0;
        }
        count--;
        if (count != -1) {
            do {
            VECTOR position;
            SVECTOR direction;
            s32 magnitude;

            position.vx = origin->vx + offset_x;
            position.vz = origin->vz + offset_z;
            position.vy = KF_COLLISION_CACHE_LOWER_BOUND;
            magnitude = random_centered_triangular_scaled(variation) + 4096;
            effect_construct_record(10, 0, 0x66, &position, &direction,
                          scale_x * magnitude >> 12,
                          scale_z * magnitude >> 12);

            count--;
            offset_x = (rand() * spread >> 14) - spread;
            offset_z = (rand() * spread >> 14) - spread;
            } while (count != -1);
        }
        return 1;
    }
    return 0;
}

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
        if (record->cache_tail.payload.ballistic.age == 0) {
            audio_play_spatial_range(5, &record->position, 110,
                                     28000, 29000, 0);
        }
        age = (u16)record->cache_tail.payload.ballistic.age + 1;
        record->cache_tail.payload.ballistic.age = age;
        prior_y = record->position.vy;
        projected.vy = record->cache_tail.payload.ballistic.origin_y +
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
            effect_apply_current_magic_backstep(collision | EFFECT_IMPACT_COUNTS_AS_PHYSICAL);
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
            collision = effect_collision_step(180, KF_COLLISION_HEIGHT_CHECK_FLOOR, -300);
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
            const KfEffectKind23Attachment *attachment = &record->cache_tail.payload.kind23;
            KfActor *actor = &actor_state.actors[attachment->actor_index];
            VECTOR vertex_offset;
            VECTOR actor_position;
            VECTOR old_position;
            VECTOR *position;

            if (actor->lifecycle != 1 || actor->target_type != 25) {
                record->type = KF_EFFECT_SLOT_FREE;
                break;
            }
            actor_sample_rotated_animation_vertex(actor, attachment->vertex_index,
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
            if (actor->animation_phase >= attachment->release_animation_phase) {
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
        if (record->cache_tail.payload.collision_latch.impact_handled == 0) {
            record->cache_tail.payload.collision_latch.impact_handled = 1;
            effect_apply_current_magic_backstep(collision);
        }
        goto kind4_rotate;
    kind4_zero_collision:
        record->cache_tail.payload.collision_latch.impact_handled = 0;
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
            if (record->cache_tail.payload.collision_latch.impact_handled == 0) {
                record->cache_tail.payload.collision_latch.impact_handled = 1;
                effect_apply_current_magic_backstep(collision);
            }
        } else {
            record->cache_tail.payload.collision_latch.impact_handled = 0;
        }
        effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        break;
    }
    case 42:
        if (record->cache_tail.payload.kind42.ticks_remaining == 0) {
            effect_scale_step(0x3800, 0x1f8, 0x46, 0x800, 0x8000);
        } else {
            record->cache_tail.payload.kind42.ticks_remaining--;
        }
        break;
    case 115:
        collision = effect_aim_and_move(0x258, 0x28, 0x24, 0xb4,
                                  0x168, 0x1000, 0x104, 0x800);
        if (collision == -1 || record->updates_remaining < 2) {
            effect_collision_backtrack();
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, NULL, 0);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, NULL, 1);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, NULL, 2);
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
                           &record->position, NULL, 0);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, NULL, 1);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, NULL, 2);
            effect_play_spatial_sound(record, 0x17);
            record->type = KF_EFFECT_SLOT_FREE;
        } else {
            effect_spawn_at_lower_bound(&record->position, 0x2000, 0x2000, 500);
        }
        break;
    case 46: {
        KfEffectRecord *selected =
            &effect_state.records[record->cache_tail.payload.kind46.linked_effect_index];
        s32 height;
        s32 age;

        record->position.vx += record->direction.vx;
        record->position.vz += record->direction.vz;
        switch (record->cache_tail.payload.kind46.phase) {
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
                record->cache_tail.payload.kind46.phase = 1;
                record->cache_tail.payload.kind46.age_q12 = 0;
                record->scale_threshold.interpolation_start_y = record->scale_y;
            }
            break;
        }
        case 1: {
            record->scale_y = fixed_lerp_q12(
                record->scale_threshold.interpolation_start_y, 0,
                (s16)record->cache_tail.payload.kind46.age_q12);
            age = record->cache_tail.payload.kind46.age_q12 + 512;
            record->cache_tail.payload.kind46.age_q12 = age;
            if ((s16)age >= 4096) {
                record->type = KF_EFFECT_SLOT_FREE;
            }
            break;
        }
        }
        break;
    }
    case 45:
        effect_scale_step(0x4000, record->cache_tail.payload.scale_step_argument.scale_step,
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
        effect_construct_record(10, record->type, 0x2d, &record->position, NULL, 0x1a4);
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
                           &actor_state.actors[actor_index].position, NULL,
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
                if (record->cache_tail.payload.raw[0] == 0) {
                    effect_apply_current_magic_backstep(flags);
                } else {
                    record->cache_tail.payload.raw[0] = 1;
                }
            } else {
                record->cache_tail.payload.raw[0] = 0;
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

        switch (record->cache_tail.payload.kind50.stage) {
        case 0:
            step = (u16)record->scale_z + 64;
            record->scale_z = step;
            record->scale_y = step;
            record->scale_x = step;
            player_sample_weapon_world_vertex(0, &record->position);
            if ((s16)record->scale_x >= 256) {
                record->cache_tail.payload.kind50.stage = 1;
                player_probe_view_target_and_vectors(1000, NULL, &record->direction, &distance);
            }
            break;
        case 1:
            if (effect_collision_step(512, KF_COLLISION_HEIGHT_CHECK_FLOOR | 0x200, 0) != 0) {
                record->cache_tail.payload.kind50.stage = 2;
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
        phase = record->cache_tail.payload.kind1.collision_stage;
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
            if (record->cache_tail.payload.kind1.collision_stage == 1) {
                record->cache_tail.payload.kind1.collision_stage = 2;
                record->render_queue_mode = 1;
                record->lighting_override_index = 0x42;
                record->lighting_blend_q12 = 0x400;
                break;
            }
            effect_collision_backtrack();
            record->cache_tail.payload.kind1.collision_stage = 1;
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
        switch ((s8)record->cache_tail.payload.raw[0]) {
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

        if (record->cache_tail.payload.kind111.actor_index == 0xff) {
            spawn_position.vx = record->position.vx;
            spawn_position.vy = record->position.vy;
            spawn_position.vz = record->position.vz;
            spawn_direction.vz = 0;
            spawn_direction.vy = 0;
            spawn_direction.vx = 0;
            spread = 1000;
        } else {
            const KfActor *actor =
                &actor_state.actors[record->cache_tail.payload.kind111.actor_index];

            spread = actor->collision_radius;
            spawn_position.vx = actor->position.vx;
            spawn_position.vz = actor->position.vz;
            spawn_position.vy = actor->position.vy;
            spawn_direction = actor->motion.vector;
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
            if (record->cache_tail.payload.collision_latch.impact_handled == 0 && rand() < 3000) {
                effect_apply_current_magic_backstep(collision);
            }
            if (collision & 0x10) {
                record->cache_tail.payload.collision_latch.impact_handled = 1;
            }
            if (collision & 5) {
                if (initial_phase == 1) {
                    record->type = KF_EFFECT_SLOT_FREE;
                } else {
                    s32 collision_height = KF_COLLISION_CACHE_RESULT;
                    record->phase = 1;
                    record->direction.vy = -200;
                    record->position.vy = collision_height;
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
        collision = effect_collision_step(50, KF_COLLISION_HEIGHT_CHECK_FLOOR, -300);
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
            u16 count = record->cache_tail.payload.kind103.remaining;
            record->cache_tail.payload.kind103.remaining = count - 1;
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

        effect_scale_step(0x3800, record->cache_tail.payload.scale_step_argument.scale_step,
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
                           &record->position, NULL);
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    }
    case 2:
        step = (u16)record->scale_x + record->cache_tail.payload.kind2.scale_step;
        record->scale_x = step;
        record->scale_z = step;
        if ((s16)record->scale_x >= 0x300) {
            VECTOR elevated;

            elevated.vx = record->position.vx;
            elevated.vy = record->position.vy + 1000;
            elevated.vz = record->position.vz;
            effect_apply_radial_magic_damage(&elevated,
                           ((s16)record->scale_x -
                            (s16)record->cache_tail.payload.kind2.scale_step) * 4,
                           (s16)record->scale_x * 4 - 1,
                           2000, 0x1000,
                           (s16)record->cache_tail.payload.kind2.radial_damage_parameter);
        }
        if ((s16)record->scale_x > (s16)record->cache_tail.payload.kind2.max_scale) {
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
                NULL, &record->rotation);

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
            record->cache_tail.payload.kind12.max_length,
            record->cache_tail.payload.kind12.scale,
            record->cache_tail.payload.kind12.turn_step,
            0xa0,
            0, 6000, record->cache_tail.payload.kind12.close_scale, 0x800);
        if (collision != -1) {
            goto kind12_collision;
        }
        {
            KfEffectRecord *child;

            effect_play_spatial_sound(record, 0x18);
            child = effect_construct_record(10, record->type | 3, 12,
                                  &record->position, NULL,
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
            if (record->cache_tail.payload.kind5.actor_index != 0xff) {
                goto kind5_actor_count;
            }
        kind5_default_count:
            count = 16;
            step_size = 0;
            record->updates_remaining = 1;
            goto kind5_count_ready;
        kind5_actor_count:
            actor = &actor_state.actors[record->cache_tail.payload.kind5.actor_index];
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
            record->cache_tail.payload.kind5.children_remaining = count;
            record->cache_tail.payload.kind5.initial_child_count = count;
            for (index = count - 1; index != -1; index--) {
                effect_construct_record(10, record->type, 105,
                               &record->position, &record->direction,
                               effect_state.current_index,
                               record->cache_tail.payload.kind5.actor_index, progress >> 12);
                progress += step_size;
            }
            record->phase = 1;
            break;
        }
        case 1: {
            const KfActor *actor;
            KfEffectRecord *child;

            if (record->updates_remaining >= 2 &&
                record->cache_tail.payload.kind5.children_remaining != 0) {
                break;
            }
            matches = 0;
            child = effect_state.records;
            for (index = KF_EFFECT_CAPACITY - 1; index != -1;
                 child++, index--) {
                const KfEffectKind105Attachment *attachment =
                    &child->cache_tail.payload.kind105;

                if (child->type == KF_EFFECT_SLOT_FREE ||
                    child->kind != 105 ||
                    attachment->parent_index != effect_state.current_index) {
                    continue;
                }
                if (child->phase == 1) {
                    child->updates_remaining = 1;
                    matches++;
                } else {
                    child->phase = 2;
                }
            }
            actor = &actor_state.actors[record->cache_tail.payload.kind5.actor_index];
            if (record->cache_tail.payload.kind5.initial_child_count != 0) {
                step = (u16)effect_magic_power(record);
                actor_apply_magic_to_actor(record->cache_tail.payload.kind5.actor_index, step,
                              magic->damage_components[0], magic->damage_components[1],
                              magic->damage_components[2], magic->damage_components[3],
                              magic->damage_components[4], magic->damage_components[5],
                              magic->damage_components[6], magic->damage_components[7],
                              (matches << 12) / record->cache_tail.payload.kind5.initial_child_count,
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
        if (record->cache_tail.payload.kind105.actor_index == 0xff) {
            goto kind105_collision;
        }
        actor = &actor_state.actors[record->cache_tail.payload.kind105.actor_index];

        actor_sample_rotated_animation_vertex(actor,
                      record->cache_tail.payload.kind105.vertex_index,
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
                &effect_state.records[record->cache_tail.payload.kind105.parent_index];
            KfEffectKind5Fanout *linked_fanout = &linked->cache_tail.payload.kind5;
            if (linked_fanout->children_remaining != 0) {
                --linked_fanout->children_remaining;
            }
            record->phase = 1;
        } else {
            if (collision == -1 &&
                (KF_COLLISION_CACHE_FLAGS & 0xf) != 0) {
                KfEffectRecord *linked =
                    &effect_state.records[record->cache_tail.payload.kind105.parent_index];
                KfEffectKind5Fanout *linked_fanout = &linked->cache_tail.payload.kind5;
                if (linked_fanout->children_remaining != 0) {
                    --linked_fanout->children_remaining;
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
        const KfEffectKind9Target *target =
            &record->cache_tail.payload.kind9;
        u8 actor_index = target->actor_index;

        if (actor_index == KF_EFFECT_KIND9_TARGET_PLAYER) {
            VECTOR target;

            target.vx = player_state.camera_position.vx;
            target.vy = player_state.camera_position.vy - KF_PLAYER_CAMERA_EYE_OFFSET;
            target.vz = player_state.camera_position.vz;
            collision = effect_target_motion(&target, 400, 60,
                                       3000, 0, 10, KF_COLLISION_HEIGHT_CHECK_FLOOR);
            if (collision != -1) {
                goto kind9_no_collision;
            }
        } else if (actor_index != KF_EFFECT_KIND9_TARGET_NONE) {
            VECTOR target;
            const KfActor *actor = &actor_state.actors[actor_index];

            target.vx = actor->position.vx;
            target.vy = actor->position.vy - (actor->collision_height >> 1);
            target.vz = actor->position.vz;
            collision = effect_target_motion(&target, 600, 50,
                                       0, 0, 10, KF_COLLISION_HEIGHT_CHECK_FLOOR);
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
        collision = effect_collision_step(10, KF_COLLISION_HEIGHT_CHECK_FLOOR, 0);
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
        target.vy = player_state.camera_position.vy - KF_PLAYER_CAMERA_EYE_OFFSET;
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
        collision = effect_collision_step(140, KF_COLLISION_HEIGHT_CHECK_FLOOR, -300);
        if (collision != 0) {
            record->position.vx -= record->direction.vx;
            record->position.vy -= record->direction.vy;
            record->position.vz -= record->direction.vz;
            effect_spawn_radial_ring(1, 0, -400, 60);
            effect_spawn_radial_ring(6, 60, -330, 56);
            effect_spawn_radial_ring(8, 140, -170, 40);
            effect_play_spatial_sound(record, 0x25);
            record->cache_tail.payload.kind106.children_remaining = 15;
            record->phase = 1;
            record->render_flags = 0;
        } else {
            effect_spawn_motion(record, -1, -3, 6000, -800,
                           6, 8, 0, -1024);
        }
        break;
    kind106_phase1:
        if (record->cache_tail.payload.kind106.children_remaining == 0) {
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
            const KfEffectKind8State *kind8 =
                &record->cache_tail.payload.kind8;
            VECTOR next;
            s32 first_collision;

            record->direction.vy = (u16)record->direction.vy +
                                   kind8->vertical_step;
            next.vx = record->position.vx + (s16)record->direction.vx;
            next.vy = record->position.vy + (s16)record->direction.vy;
            next.vz = record->position.vz + (s16)record->direction.vz;
            first_collision = effect_probe_collision_by_type(&next, 140, KF_COLLISION_HEIGHT_CHECK_FLOOR);
            if (first_collision != 0 && (first_collision & 0xf) != 0) {
                next.vx = record->position.vx;
                next.vz = record->position.vz;
                if ((effect_probe_collision_by_type(&next, 140, KF_COLLISION_HEIGHT_CHECK_FLOOR) & 0xf) == 0) {
                    goto kind8_reset_axes;
                }
                if ((s16)record->direction.vy < 0) {
                    record->direction.vy = 0;
                    next.vy = record->position.vy;
                } else {
                    KfEffectRecord *parent =
                        &effect_state.records[kind8->parent_index];

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
                        record->scale_threshold.next_probe_phase =
                            ((rand() * 1500) >> 15) + 256;
                        record->updates_remaining = 100;
                        break;
                    }
                    if (parent->cache_tail.payload.kind106.children_remaining != 0) {
                        parent->cache_tail.payload.kind106.children_remaining--;
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
            if (KF_COLLISION_CACHE_LAYER == 0) {
                record->map_layer_mask = 1;
            } else {
                record->map_layer_mask = 2;
            }
            record->rotation.vz = ((u16)record->rotation.vz + 300) & KF_ANGLE_WRAP_MASK;
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
            const KfEffectKind8State *kind8 =
                &record->cache_tail.payload.kind8;
            KfEffectRecord *parent =
                &effect_state.records[kind8->parent_index];
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
            if ((s16)record->scale_z >= record->scale_threshold.next_probe_phase) {
                s32 collision_kind;

                record->scale_threshold.next_probe_phase += 2048;
                collision_kind = collision_query_world(
                    record->position.vx, record->position.vy,
                    record->position.vz, 256, (s16)record->scale_y, 144);
                if (collision_kind != 0) {
                    effect_apply_current_magic(collision_kind, 5000, NULL);
                }
            }
            if ((s16)record->scale_z >= 4096) {
                record->type = KF_EFFECT_SLOT_FREE;
                if (parent->cache_tail.payload.kind106.children_remaining != 0) {
                    parent->cache_tail.payload.kind106.children_remaining--;
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
        KfEffectKind10Targeting *targeting =
            &record->cache_tail.payload.kind10;

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
        collision = effect_collision_step(250, KF_COLLISION_HEIGHT_CHECK_FLOOR, 0);
        if (collision != 0 || record->updates_remaining < 2) {
            KfEffectRecord *child;

            effect_scatter_lower_bound(&record->position, 8, 400,
                           0x2000, 0x8000, 0x400);
            child = effect_construct_record(10, record->type | 3, 10,
                                  &record->position, NULL,
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
        record->animation_phase_q12 = ((u16)record->animation_phase_q12 + 128)
                                      & EFFECT_ANIMATION_PHASE_MASK;
        if (targeting->emissions_remaining != 0) {
            VECTOR origin;
            VECTOR target;
            SVECTOR direction;
            const KfActor *actor;

            effect_sample_world_vertex(record, 0, &origin,
                           (const SVECTOR *)&record->scale_x);
            actor = &actor_state.actors[targeting->actor_index];
            target.vx = actor->position.vx;
            target.vy = actor->position.vy - (actor->collision_height >> 1);
            target.vz = actor->position.vz;
            vector_direction_scaled(&origin, &target, 800, &direction);
            effect_construct_record(10, record->type, 7, &origin, &direction);
            targeting->emissions_remaining--;
        } else if (rand() < 2048) {
            s32 distance;
            KfActor *actor = actor_find_best_in_cone(
                &record->position, record->rotation.vy,
                record->rotation.vx, 25000, 800, 800,
                &distance, 512);

            if (actor != NULL) {
                targeting->emissions_remaining = 5;
                targeting->actor_index = actor - actor_state.actors;
            }
        }
        if (rand() < 1024) {
            s32 distance;

            if (actor_find_best_in_cone(&record->position, record->rotation.vy,
                               record->rotation.vx, 30000, 800, 800,
                               &distance, 512) != NULL) {
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
            KfEffectTrailChildLink *link;
            u8 parent_index;

            for (index = 0; index < 8; index++) {
                child = effect_construct_record(
                    10, 0, 107, &record->position, &record->direction,
                    &record->rotation, index);

                if (index == 0) {
                    child->render_id = 0x17;
                }
                parent_index = effect_state.current_index;
                link = &child->cache_tail.payload.trail_child;
                link->lag_index = index;
                link->parent_index = parent_index;
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
                record->cache_tail.payload.trail.phase_counter = 24;
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

                    effect_apply_current_magic(EFFECT_IMPACT_HOLD_ACTOR_ANIMATION |
                                               KF_COLLISION_HIT_ACTOR, 5000, NULL);
                    actor_index = *(u8 *)&KF_COLLISION_CACHE_ACTOR_INDEX;
                    record->cache_tail.payload.trail.actor_index = actor_index;
                    actor = &actor_state.actors[record->cache_tail.payload.trail.actor_index];
                    if (actor->target_type == 2 || actor->target_type == 3) {
                        record->render_flags = 1;
                        record->render_id = 22;
                        record->phase = 2;
                        record->cache_tail.payload.trail.phase_counter = 0;
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
                KfEffectTrailRow *rows = record->cache_tail.payload.trail.rows;
                KfEffectTrailRow *row;
                u8 frame = record->cache_tail.payload.trail.frame_index + 1;
                s32 angle = record->cache_tail.payload.trail.phase_counter << 4;

                record->cache_tail.payload.trail.frame_index = frame;
                if (frame >= 24) {
                    record->cache_tail.payload.trail.frame_index = 0;
                }
                row = &rows[record->cache_tail.payload.trail.frame_index];
                record->cache_tail.payload.trail.phase_counter += 8;
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
            KfActor *actor = &actor_state.actors[record->cache_tail.payload.trail.actor_index];
            VECTOR scratch;
            VECTOR *position;
            u8 frame = record->cache_tail.payload.trail.frame_index + 1;
            s32 actor_extent;

            actor->flags |= KF_ACTOR_FLAG_EFFECT_ANIMATION_HOLD;
            actor_extent = actor->collision_radius;
            record->cache_tail.payload.trail.frame_index = frame;
            if (frame >= 24) {
                record->cache_tail.payload.trail.frame_index = 0;
            }
            position = actor_resolve_group_position(actor, &scratch);
            record->position = *position;
            record->position.vy -= actor->collision_height >> 1;
            if (record->cache_tail.payload.trail.phase_counter < 17) {
                s32 scale = fixed_lerp_q12(
                    0, actor_extent, record->cache_tail.payload.trail.phase_counter << 9);

                record->scale_z = scale;
                record->scale_x = scale;
                record->scale_y = fixed_lerp_q12(
                    0, actor->collision_height,
                    record->cache_tail.payload.trail.phase_counter * 350);
            } else if (record->cache_tail.payload.trail.phase_counter >= 60) {
                record->phase = 4;
                break;
            }
            {
                KfEffectTrailRow *rows = record->cache_tail.payload.trail.rows;
                KfEffectTrailRow *row = &rows[record->cache_tail.payload.trail.frame_index];
                s32 radius = ((s32)actor_extent * 25 << 8) >> 12;
                s32 angle = (s16)record->rotation.pad;

                record->cache_tail.payload.trail.phase_counter++;
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
            u8 frame = ++record->cache_tail.payload.trail.frame_index;

            if (frame >= 24) {
                record->cache_tail.payload.trail.frame_index = 0;
            }
            if (record->cache_tail.payload.trail.phase_counter != 0) {
                record->cache_tail.payload.trail.phase_counter--;
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
        KfActor *actor = &actor_state.actors[record->cache_tail.payload.trail.actor_index];

        actor->flags &= ~KF_ACTOR_FLAG_EFFECT_ANIMATION_HOLD;
        record->type = KF_EFFECT_SLOT_FREE;
        break;
    }
    }
    case 107: {
        KfEffectRecord *selected = &effect_state.records[
            record->cache_tail.payload.trail_child.parent_index];
        const KfEffectTrailRow *snapshot;
        s32 frame_index;

        if (initial_phase == 0 && selected->phase >= 3) {
            record->phase = 1;
            record->updates_remaining =
                record->cache_tail.payload.trail_child.lag_index * 3;
        }
        if (selected->phase == 4) {
            record->type = KF_EFFECT_SLOT_FREE;
            break;
        }
        frame_index = selected->cache_tail.payload.trail.frame_index -
                      record->cache_tail.payload.trail_child.lag_index * 3;
        if (frame_index < 0) {
            frame_index += 24;
        }
        snapshot = &selected->cache_tail.payload.trail.rows[frame_index];
        record->position = snapshot->position;
        record->rotation = snapshot->rotation;
        break;
    }
    case 101: {
        const KfEffectKind101Motion *motion =
            &record->cache_tail.payload.kind101;

        record->direction.vy = (u16)record->direction.vy +
                               motion->vertical_step;
        record->position.vx += record->direction.vx;
        record->position.vz += record->direction.vz;
        step = record->scale_x + motion->scale_step;
        record->scale_x = step;
        record->scale_z = step;
        record->scale_y = step;
        record->position.vy += record->direction.vy;
        break;
    }
    case 102: {
        record->phase++;
        record->scale_y = (rsin(record->phase << 8) *
                           record->cache_tail.payload.kind102.amplitude) >> 12;
        if (record->phase >= 8) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        break;
    }
    case KF_EFFECT_KIND_DEFENSE_BOOST:
        if (player_state.defense_boost_timer == 0) {
            record->type = KF_EFFECT_SLOT_FREE;
        }
        record->rotation.vy += 128;
        break;
    case KF_EFFECT_KIND_ATTACK_BOOST:
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
        collision = effect_collision_step(100, KF_COLLISION_HEIGHT_CHECK_FLOOR, -300);
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
            collision_probe_floor_height(record->position.vx,
                          record->cache_tail.payload.kind114.origin_y,
                          record->position.vz, 0, 0);
            if (record->position.vy < KF_COLLISION_CACHE_RESULT) {
                goto kind114_particles;
            }
            record->position.vy = KF_COLLISION_CACHE_RESULT;
        }
        effect_construct_record(10, record->type | 3, 3, &record->position, NULL, 0);
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
        target.vy = player_state.camera_position.vy - KF_PLAYER_CAMERA_EYE_OFFSET;
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
    case 109: {
        const KfEffectKind109Target *target =
            &record->cache_tail.payload.kind109;

        effect_target_motion(
            &effect_state.records[target->effect_index].position,
            500, 15, -1, 0, 0, -1);
        break;
    }
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
                           &record->position, NULL, 0);
            effect_construct_record(10, record->type | 3, 0x2a,
                           &record->position, NULL, 1);
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

ADDRESS(0x80045cc0, 0x30)
void effect_pool_reset(void)
{
    KfEffectRecord *record = effect_state.records;
    u16 i;

    for (i = 0; i < KF_EFFECT_CAPACITY; i++) {
        record->type = KF_EFFECT_SLOT_FREE;
        record++;
    }
}

ADDRESS(0x80045cf0, 0x2c)
void magic_load_records(const KfMagicRecord *records)
{
    const u32 *source = (const u32 *)records;
    u32 *destination = (u32 *)effect_state.magic_records;
    s32 count;

    for (count = sizeof effect_state.magic_records / sizeof *source; count != 0; count--) {
        *destination++ = *source++;
    }
}


ADDRESS(0x80045d1c, 0xfc)
void effect_pool_sweep(void)
{
    KfEffectRecord *record = effect_state.records;

    effect_state.current_index = 0;
    do {
        if (record->type != KF_EFFECT_SLOT_FREE) {
            effect_state.current_record = record;
            effect_state.current_magic = &effect_state.magic_records[record->kind];
            if (record->updates_remaining != -1) {
                if (--record->updates_remaining == -1) {
                    record->type = KF_EFFECT_SLOT_FREE;
                } else {
                    effect_update_dispatch();
                }
            } else {
                effect_update_dispatch();
            }
        }
        effect_state.current_index++;
        record++;
    } while (effect_state.current_index < KF_EFFECT_CAPACITY);
}
