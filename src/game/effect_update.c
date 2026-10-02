#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/lib/math.h>
#include <kf/game/animation.h>
#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>

enum { EFFECT_FIXED_MAGIC_POWER = 5 };

ADDRESS(0x8003fb94, 0x218)
void func_8003fb94(s32 kind, s32 record_type, s32 radius, u16 power,
                   u8 record_id, u16 magic_06, u16 magic_08, u16 magic_0a,
                   u16 magic_04, u16 magic_0c, u16 magic_0e, u16 magic_10,
                   u16 magic_12, u16 magic_14, const VECTOR *position)
{
    s32 options = kind & 0xf0000;
    kind &= ~0xf0000;

    if (kind == 0x80) {
        func_800248a8(magic_06, magic_08, magic_0a, magic_04,
                      magic_0c, magic_0e, magic_10, magic_12,
                      magic_14, radius, record_id, position);
    } else if (kind == 0x10) {
        s32 actor_index = KF_COLLISION_CACHE_ACTOR_INDEX;
        KfActor *actor = &actor_state.actors[actor_index];
        KfTargetGroup *group = &actor_state.target_groups[actor->group_index];

        if (position != NULL) {
            s32 angle = vector_xz_to_angle(
                actor->position.vx - position->vx,
                actor->position.vz - position->vz);
            if (!angle_within_tolerance(actor->rotation.y, angle + 0x800,
                                        group->unknown_18)) {
                return;
            }
        }

        record_type &= 0x30;
        if (options & 0x20000) {
            record_type |= 1;
        } else {
            record_type |= 2;
        }
        func_80039c94(KF_COLLISION_CACHE_ACTOR_INDEX, power, magic_06,
                      magic_08, magic_0a, magic_0c, magic_0e, magic_10,
                      magic_12, magic_14, radius, record_type, position);
        if (options & 0x10000) {
            actor->unknown_28 |= 0x800;
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
void func_8003fdd0(s32 kind, s32 radius, const VECTOR *position)
{
    KfEffectRecord *record = effect_state.current_record;
    const KfMagicRecord *magic = effect_state.current_magic;
    u16 power = effect_magic_power(record);

    func_8003fb94(kind, record->type, radius, power, record->unknown_06,
                  magic->unknown_06, magic->unknown_08, magic->unknown_0a,
                  magic->unknown_04, magic->unknown_0c, magic->unknown_0e,
                  magic->unknown_10, magic->unknown_12, magic->unknown_14,
                  position);
}

ADDRESS(0x8003feb0, 0x68)
void func_8003feb0(s32 kind)
{
    const KfEffectRecord *record = effect_state.current_record;
    VECTOR position;

    position.vx = record->position.vx - (record->direction.vx << 3);
    position.vy = record->position.vy - (record->direction.vy << 3);
    position.vz = record->position.vz - (record->direction.vz << 3);
    func_8003fdd0(kind, 5000, &position);
}


ADDRESS(0x8003ff18, 0x1a8)
void func_8003ff18(VECTOR *position, s32 start, s32 end, s32 arg3, s32 arg4, s32 arg5)
{
    KfEffectRecord *record = effect_state.current_record;
    const KfMagicRecord *magic = effect_state.current_magic;

    if (record->type & 1) {
        func_80024ca4(position, start, end, arg3, arg4,
                      magic->unknown_06, magic->unknown_08, magic->unknown_0a,
                      magic->unknown_04, magic->unknown_0c, magic->unknown_0e,
                      magic->unknown_10, magic->unknown_12, magic->unknown_14,
                      arg5, record->unknown_06);
    }
    if (record->type & 2) {
        u16 power = effect_magic_power(record);

        func_8003a318(position, start, end, arg3, arg4,
                      power, magic->unknown_06,
                      magic->unknown_08, magic->unknown_0a, magic->unknown_0c,
                      magic->unknown_0e, magic->unknown_10, magic->unknown_12,
                      magic->unknown_14, arg5, (record->type & 0x30) | 2);
    }
}


ADDRESS(0x800400c0, 0xf4)
void func_800400c0(KfEffectRecord *record, s32 mode, VECTOR *output, const SVECTOR *scale)
{
    struct KfEulerAngles angles;
    SVECTOR offset;

    func_80034344(record->render_id + 40, record->animation_clip,
                  record->unknown_12, mode, &offset);
    offset.vx = offset.vx * scale->vx >> KF_FIXED12_BITS;
    offset.vy = offset.vy * scale->vy >> KF_FIXED12_BITS;
    offset.vz = offset.vz * scale->vz >> KF_FIXED12_BITS;

    angles.x = record->rotation.vx;
    angles.y = record->rotation.vy + KF_ANGLE_HALF_TURN;
    angles.z = record->rotation.vz;
    vector_rotate_yxz(&angles, &offset, output);
}

ADDRESS(0x800401b4, 0x6c)
void func_800401b4(KfEffectRecord *record, s32 mode, VECTOR *position, const SVECTOR *scale)
{
    func_800400c0(record, mode, position, scale);
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
    record->unknown_08 = 5;
    record->animation_clip = 0x80;
    record->base_render_id = render_id;
    record->render_id = render_id;
    record->unknown_09 = 1;
    record->unknown_0c = 0x43;
    record->unknown_10 = 0x1000;
    record->scale_z = scale;
    record->scale_y = scale;
    record->scale_x = scale;
}

ADDRESS(0x800402a4, 0x64)
void effect_pool_initialize_fixed(KfEffectRecord *record, u8 render_id)
{
    record->unknown_08 = 14;
    record->animation_clip = 0x80;
    record->base_render_id = render_id;
    record->render_id = render_id;
    record->unknown_09 = 1;
    record->unknown_0c = 0x44;
    record->unknown_10 = 0x1000;
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
