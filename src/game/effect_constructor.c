#include <kf/lib/address.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>

RODATA(0x8001249c, 0x1ec)

ADDRESS(0x80040308, 0x13e4)
KfEffectRecord *func_80040308(u8 id, u8 type, u8 kind,
                              const VECTOR *position,
                              const SVECTOR *direction, ...)
{
    KfEffectRecord *record = effect_pool_find_free();
    s32 length_squared;

    if (record == 0) {
        return 0;
    }
    record->type = type;
    record->kind = kind;
    if (position != 0) {
        record->position = *position;
    }
    record->unknown_0a = 3;
    if (direction == 0) {
        record->direction.vx = 0;
        record->direction.vy = 0;
        record->direction.vz = 0;
    } else {
        record->direction = *direction;
    }
    record->phase = 0;
    record->unknown_06 = id;
    record->scale_x = 0x1000;
    record->scale_y = 0x1000;
    record->scale_z = 0x1000;
    record->rotation.vx = 0;
    record->rotation.vy = 0;
    record->rotation.vz = 0;
    record->unknown_08 = 1;
    record->unknown_05 = 0;
    record->cooldown = 1;
    if ((type & KF_EFFECT_USE_PLAYER_MAGIC) != 0 &&
        player_state.death_state == 1) {
        record->cooldown = 8;
    }
    record->unknown_10 = 0;
    record->unknown_0c = 0xff;
    record->unknown_09 = 0xff;
    record->updates_remaining = -1;
    length_squared = (s32)record->direction.vx * record->direction.vx +
        (s32)record->direction.vy * record->direction.vy +
        (s32)record->direction.vz * record->direction.vz;
    record->unknown_0d = length_squared >= 810001;

    switch (kind) {
    case 0:
        effect_pool_initialize_scaled(record, 0xe, 0x200);
        record->updates_remaining = 50;
        record->unknown_3c[4] = 0;
        break;
    case 3:
        record->unknown_08 = 1;
        record->unknown_09 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_x = 0;
        record->scale_y = 0;
        record->scale_z = 0;
        effect_play_spatial_sound(record, 0x1f);
        break;
    case 4:
        record->unknown_09 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x1f;
        record->render_id = 0x1f;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->updates_remaining = 0x2d;
        record->scale_x = 0x32c8;
        record->scale_y = 0x32c8;
        record->scale_z = 0x32c8;
        record->unknown_0d = 1;
        effect_play_spatial_sound(record, 0x20);
        break;
    /* The remaining kinds and their O32 trailing operands are not yet
     * reconstructed. The 123-word table and its indirect dispatch remain
     * distinct from proven direct calls. */
    }
    return record;
}
