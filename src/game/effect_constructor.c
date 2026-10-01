#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>
#include <psyq/libc.h>
#include <stdarg.h>

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

    if (kind > 122 || kind == 18 || kind == 21 || kind == 36 ||
        kind == 37 || kind == 41 || kind == 43 || kind == 44 ||
        (kind >= 55 && kind <= 99) || kind == 108 || kind == 110 ||
        kind == 112) {
        record->type = KF_EFFECT_SLOT_FREE;
        return record;
    }

    switch (kind) {
    case 0:
        effect_pool_initialize_scaled(record, 0xe, 0x200);
        record->updates_remaining = 50;
        record->unknown_3c[4] = 0;
        break;
    case 1:
    case 28:
        if (kind == 28) {
            record->scale_x = 0x800;
            record->scale_y = 0x800;
            record->scale_z = 0x800;
        }
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x20;
        record->render_id = 0x20;
        record->unknown_3c[4] = 0;
        record->updates_remaining = 70;
        effect_play_spatial_sound(record, 0x1b);
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
    case 5: {
        va_list arguments;

        record->unknown_08 = 0;
        record->updates_remaining = 70;
        va_start(arguments, direction);
        record->unknown_3c[5] = va_arg(arguments, s32);
        va_end(arguments);
        effect_play_spatial_sound(record, 0x21);
        break;
    }
    case 7:
    case 49:
        effect_pool_initialize_scaled(record, 0x2d, 0x1800);
        record->updates_remaining = 50;
        record->unknown_0d = 1;
        effect_play_spatial_sound(record, 0x23);
        break;
    case 8: {
        va_list arguments;

        effect_pool_initialize_scaled(record, 8, 0x1000);
        va_start(arguments, direction);
        record->unknown_3c[4] = va_arg(arguments, s32);
        *(u16 *)&record->unknown_3c[6] = va_arg(arguments, s32);
        va_end(arguments);
        record->updates_remaining = 50;
        break;
    }
    case 11:
    case 54: {
        va_list arguments;
        s32 value;
        s32 render_id = kind == 54 ? 0x30 : 0x11;

        record->base_render_id = render_id;
        record->render_id = render_id;
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_x = 0;
        record->scale_y = 0;
        record->scale_z = 0;
        va_start(arguments, direction);
        value = va_arg(arguments, s32);
        va_end(arguments);
        *(u16 *)&record->unknown_3c[4] = value / 4;
        break;
    }
    case 13:
        effect_pool_initialize_scaled(record, 0x21, 0x1000);
        record->updates_remaining = 50;
        effect_play_spatial_sound(record, 0x2a);
        break;
    case 14:
    case 16:
    case 19:
        record->unknown_08 = 0;
        record->updates_remaining = kind == 16 ? 8 : 16;
        audio_play_sound(0x2b, 120);
        break;
    case 15:
        effect_pool_initialize_fixed(record, 0x19);
        break;
    case 17:
        effect_pool_initialize_fixed(record, 0x1a);
        break;
    case 20:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_x = 0;
        record->scale_y = 0;
        record->scale_z = 0;
        break;
    case 22:
        effect_pool_initialize_scaled(record, 0x1e, 0x1000);
        record->cooldown = 3;
        record->updates_remaining = 30;
        record->direction.vx += (rand() >> 9) - 32;
        record->direction.vy += (rand() >> 9) - 32;
        record->direction.vz += (rand() >> 9) - 32;
        break;
    case 23: {
        va_list arguments;
        u16 first_parameter;
        u16 second_parameter;
        u16 third_parameter;

        va_start(arguments, direction);
        first_parameter = va_arg(arguments, s32);
        second_parameter = va_arg(arguments, s32);
        third_parameter = va_arg(arguments, s32);
        va_end(arguments);
        effect_pool_initialize_scaled(record, 8, 0x400);
        record->direction.vx = 0;
        record->direction.vy = 0;
        record->direction.vz = 0;
        record->phase = 9;
        *(u16 *)&record->unknown_3c[4] = first_parameter;
        *(u16 *)&record->unknown_3c[6] = second_parameter;
        *(u16 *)&record->unknown_3c[8] = third_parameter;
        effect_play_spatial_sound(record, 0x26);
        break;
    }
    case 24:
        record->unknown_08 = 0;
        record->updates_remaining = 70;
        break;
    case 25: {
        va_list arguments;
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0xd;
        record->render_id = 0xd;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->updates_remaining = 45;
        va_start(arguments, direction);
        angles = va_arg(arguments, const SVECTOR *);
        va_end(arguments);
        record->rotation = *angles;
        record->rotation.vz = rand() >> 3;
        record->unknown_3c[4] = 0;
        break;
    }
    case 26:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x24;
        record->render_id = 0x24;
        record->unknown_3c[4] = 0;
        record->updates_remaining = 70;
        record->scale_x = 600;
        record->scale_y = 600;
        record->scale_z = 600;
        break;
    case 27:
    case 51:
    case 52:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x25;
        record->render_id = 0x25;
        record->unknown_3c[4] = 0;
        if (kind == 27) {
            record->updates_remaining = 70;
        }
        record->scale_x = 0;
        record->scale_y = 0;
        record->scale_z = 0;
        break;
    case 29:
    case 30:
    case 31:
    case 47:
    case 48: {
        va_list arguments;
        const SVECTOR *angles;
        s32 render_id;

        if (kind == 47) {
            render_id = 0x2b;
        } else if (kind == 48) {
            render_id = 0x2a;
        } else if (kind == 30) {
            render_id = 0x1d;
        } else {
            render_id = 0x1c;
        }
        record->base_render_id = render_id;
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_0c = 0xff;
        record->render_id = record->base_render_id;
        va_start(arguments, direction);
        angles = va_arg(arguments, const SVECTOR *);
        va_end(arguments);
        record->rotation = *angles;
        record->rotation.vz = 0;
        *(u16 *)&record->unknown_3c[6] = 0;
        break;
    }
    case 32:
        effect_pool_initialize_scaled(record, 0x21, 0x1800);
        record->updates_remaining = 50;
        break;
    case 50:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->scale_x = 0;
        record->scale_y = 0;
        record->scale_z = 0;
        record->unknown_3c[4] = 0;
        effect_play_spatial_sound(record, 0x18);
        break;
    case 101: {
        va_list arguments;
        s32 scale;
        s32 first_value;
        s32 lifetime;
        s32 render_id;
        s32 second_value;

        va_start(arguments, direction);
        scale = va_arg(arguments, s32);
        first_value = va_arg(arguments, s32);
        lifetime = va_arg(arguments, s32);
        render_id = va_arg(arguments, s32);
        second_value = va_arg(arguments, s32);
        va_end(arguments);
        effect_pool_initialize_scaled(record, render_id, scale);
        *(u16 *)&record->unknown_3c[4] = first_value;
        record->updates_remaining = lifetime;
        *(u16 *)&record->unknown_3c[6] = second_value;
        break;
    }
    case 103:
    case 121: {
        va_list arguments;

        effect_pool_initialize_scaled(record, kind == 121 ? 0x2e : 0xf,
                                      0x1000);
        record->updates_remaining = 100;
        va_start(arguments, direction);
        *(u16 *)&record->unknown_3c[4] = va_arg(arguments, s32);
        va_end(arguments);
        effect_play_spatial_sound(record, 0x28);
        break;
    }
    case 104:
    case 122: {
        va_list arguments;

        effect_pool_initialize_scaled(record, kind == 122 ? 0x2f : 0x10,
                                      0x1000);
        va_start(arguments, direction);
        record->scale_y = va_arg(arguments, s32);
        va_end(arguments);
        record->updates_remaining = 15;
        break;
    }
    case 33:
    case 53:
        effect_pool_initialize_scaled(record, 0x21,
                                      kind == 53 ? 0x2000 : 0x1000);
        record->direction.vx += (rand() >> 8) - 64;
        record->direction.vy += (rand() >> 8) - 64;
        record->updates_remaining = 100;
        record->cooldown = 3;
        record->direction.vz += (rand() >> 8) - 64;
        effect_play_spatial_sound(record, 0x21);
        break;
    case 34:
    case 35:
    case 117: {
        va_list arguments;
        const SVECTOR *angles;
        s32 render_id = kind == 117 ? 0x31 : kind == 34 ? 0x28 : 0x29;

        record->base_render_id = render_id;
        record->render_id = render_id;
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->updates_remaining = kind == 117 ? 50 : 45;
        va_start(arguments, direction);
        angles = va_arg(arguments, const SVECTOR *);
        va_end(arguments);
        record->rotation = *angles;
        record->unknown_3c[4] = 0;
        effect_play_spatial_sound(record, 0x20);
        break;
    }
    case 40: {
        va_list arguments;
        const SVECTOR *angles;

        record->base_render_id = 0xa;
        record->render_id = 0xa;
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->updates_remaining = 50;
        va_start(arguments, direction);
        angles = va_arg(arguments, const SVECTOR *);
        va_end(arguments);
        record->rotation = *angles;
        effect_play_spatial_sound(record, 0x32);
        break;
    }
    case 45: {
        va_list arguments;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0x30;
        record->render_id = 0x30;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_x = 0;
        record->scale_y = 0;
        record->scale_z = 0;
        effect_play_spatial_sound(record, 0x17);
        va_start(arguments, direction);
        *(u16 *)&record->unknown_3c[4] = va_arg(arguments, s32);
        va_end(arguments);
        break;
    }
    case 105: {
        va_list arguments;

        effect_pool_initialize_scaled(record, 0xe, 0x1000);
        record->rotation.vz = rand();
        record->direction.vx += (rand() >> 9) - 32;
        record->direction.vy += (rand() >> 9) - 32;
        record->direction.vz += (rand() >> 9) - 32;
        va_start(arguments, direction);
        record->unknown_3c[4] = va_arg(arguments, s32);
        record->unknown_3c[5] = va_arg(arguments, s32);
        *(u16 *)&record->unknown_3c[6] = va_arg(arguments, s32);
        va_end(arguments);
        record->updates_remaining = 70;
        break;
    }
    case 106:
        effect_pool_initialize_scaled(record, 8, 0x2000);
        record->direction.vy -= 100;
        effect_play_spatial_sound(record, 0x24);
        record->updates_remaining = 100;
        break;
    case 109: {
        va_list arguments;

        effect_pool_initialize_scaled(record, 0xe, 0x400);
        record->updates_remaining = 20;
        va_start(arguments, direction);
        record->unknown_3c[4] = va_arg(arguments, s32);
        va_end(arguments);
        record->direction.vx += (rand() >> 8) - 64;
        record->direction.vy += (rand() >> 8) - 64;
        record->direction.vz += (rand() >> 8) - 64;
        break;
    }
    case 111: {
        va_list arguments;

        va_start(arguments, direction);
        record->unknown_08 = 0;
        record->updates_remaining = 50;
        *(u16 *)&record->unknown_3c[4] = va_arg(arguments, s32);
        va_end(arguments);
        effect_play_spatial_sound(record, 0x21);
        break;
    }
    case 116:
        record->unknown_08 = 0;
        record->updates_remaining = 20;
        break;
    case 118:
    case 119: {
        va_list arguments;
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x13;
        record->render_id = 0x13;
        record->updates_remaining = 0x23;
        va_start(arguments, direction);
        angles = va_arg(arguments, const SVECTOR *);
        va_end(arguments);
        record->rotation = *angles;
        effect_play_spatial_sound(record, 0x20);
        break;
    }
    case 120:
        effect_pool_initialize_scaled(record, 50, 0);
        record->updates_remaining = 100;
        if (rand() < 4096) {
            effect_play_spatial_sound(record, 0x21);
        }
        break;
    /* The remaining kinds and their O32 trailing operands are not yet
     * reconstructed. The 123-word table and its indirect dispatch remain
     * distinct from proven direct calls. */
    }
    return record;
}
