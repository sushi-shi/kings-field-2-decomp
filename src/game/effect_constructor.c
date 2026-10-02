#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>
#include <psyq/libc.h>


DATA(0x8006d704, 0x4)
u32 effect_trail_next_slot = 0;

DATA(0x8009a5a8, 0x4)
s32 DAT_8009a5a8;

DATA(0x801d9628, 0x900)
KfEffectTrailRow effect_trail_rows[4][24];

RODATA(0x8001249c, 0x1ec)

ADDRESS(0x80040308, 0x13e4)
KfEffectRecord *func_80040308(u8 id, u8 type, u8 kind,
                              const VECTOR *position,
                              const SVECTOR *direction, ...)
{
    KfEffectRecord *record;
    s32 length_squared;
    s32 three_parameter_sound;
    u16 third_parameter;
    /* O32 stacks the fifth argument; optional words follow its home slot. */
    s32 *va = (s32 *)&direction;

    record = effect_pool_find_free();

    if (record == 0) {
        goto finish;
    }
    record->type = type;
    record->kind = kind;
    if (position != 0) {
        record->position = *position;
    }
    record->unknown_0a = 3;
    if (direction != 0) {
        record->direction = *direction;
    } else {
        record->direction.vz = 0;
        record->direction.vy = 0;
        record->direction.vx = 0;
    }
    record->phase = 0;
    record->unknown_06 = id;
    record->scale_z = 0x1000;
    record->scale_y = 0x1000;
    record->scale_x = 0x1000;
    record->rotation.vz = 0;
    record->rotation.vy = 0;
    record->rotation.vx = 0;
    record->unknown_12 = 0;
    record->unknown_05 = 0;
    record->unknown_08 = 1;
    if ((record->type & KF_EFFECT_USE_PLAYER_MAGIC) != 0 &&
        player_state.death_state == 1) {
        record->cooldown = 8;
    } else {
        record->cooldown = 1;
    }
    record->unknown_0c = 0xff;
    record->unknown_09 = 0xff;
    record->updates_remaining = -1;
    length_squared = (s32)record->direction.vx * record->direction.vx +
        (s32)record->direction.vy * record->direction.vy +
        (s32)record->direction.vz * record->direction.vz;
    record->unknown_10 = 0;
    if (length_squared >= 810001) {
        record->unknown_0d = 1;
    } else {
        record->unknown_0d = 0;
    }

    switch (record->kind) {
    case 7:
    case 49:
        effect_pool_initialize_scaled(record, 0x2d, 0x1800);
        record->updates_remaining = 50;
        record->unknown_0d = 1;
        effect_play_spatial_sound(record, 0x23);
        break;
    case 32:
        effect_pool_initialize_scaled(record, 0x21, 0x1800);
        record->updates_remaining = 50;
        break;
    case 4:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0x1f;
        record->render_id = 0x1f;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->updates_remaining = 0x2d;
        record->unknown_3c[4] = 0;
        record->scale_z = 0x32c8;
        record->scale_y = 0x32c8;
        record->scale_x = 0x32c8;
        record->unknown_0d = 1;
        effect_play_spatial_sound(record, 0x20);
        break;
    case 28:
        record->scale_z = 0x800;
        record->scale_y = 0x800;
        record->scale_x = 0x800;
        /* fall through */
    case 1:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x20;
        record->render_id = 0x20;
        record->unknown_3c[4] = 0;
        record->updates_remaining = 70;
        effect_play_spatial_sound(record, 0x1b);
        break;
    case 26:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x24;
        record->render_id = 0x24;
        record->unknown_3c[4] = 0;
        record->updates_remaining = 70;
        record->scale_z = 600;
        record->scale_y = 600;
        record->scale_x = 600;
        break;
    case 27:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x25;
        record->render_id = 0x25;
        record->unknown_3c[4] = 0;
        record->updates_remaining = 70;
        goto zero_scale_27_51_52;
    case 111: {
        u16 value = va[1];
        record->unknown_08 = 0;
        record->updates_remaining = 50;
        *(u16 *)&record->unknown_3c[4] = value;
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
        record->unknown_3c[4] = 0;
        break;
    case 25: {
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0xd;
        record->render_id = 0xd;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->updates_remaining = 45;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->rotation.vz = rand() >> 3;
        record->unknown_3c[4] = 0;
        break;
    }
    case 5: {
        u8 parameter;

        record->unknown_08 = 0;
        parameter = va[1];
        record->updates_remaining = 70;
        record->unknown_3c[5] = parameter;
        effect_play_spatial_sound(record, 0x21);
        break;
    }
    case 105: {
        effect_pool_initialize_scaled(record, 0xe, 0x1000);
        record->rotation.vz = rand();
        record->direction.vx += (rand() >> 9) - 32;
        record->direction.vy += (rand() >> 9) - 32;
        record->direction.vz += (rand() >> 9) - 32;
        record->unknown_3c[4] = va[1];
        record->unknown_3c[5] = va[2];
        *(u16 *)&record->unknown_3c[6] = va[3];
        record->updates_remaining = 70;
        break;
    }
    case 9: {
        effect_pool_initialize_scaled(record, 8, 0x1000);
        record->direction.vx += (rand() >> 8) - 64;
        record->direction.vy += (rand() >> 8) - 64;
        record->direction.vz += (rand() >> 8) - 64;
        record->updates_remaining = 100;
        record->cooldown = 3;
        record->unknown_3c[4] = va[1];
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
        effect_pool_initialize_scaled(record, 8, 0x1000);
        record->unknown_3c[4] = va[1];
        *(u16 *)&record->unknown_3c[6] = va[2];
        record->updates_remaining = 50;
        break;
    }
    case 10: {
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->animation_clip = 0;
        record->unknown_09 = 1;
        record->base_render_id = 0x15;
        record->render_id = 0x15;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_z = 3000;
        record->scale_y = 3000;
        record->scale_x = 3000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->unknown_3c[5] = 0;
        record->updates_remaining = 150;
        effect_play_spatial_sound(record, 0x27);
        break;
    }
    case 6: {
        const SVECTOR *angles;
        KfEffectTrailRow *rows;
        s32 index;
        u32 slot;

        record->unknown_08 = 0;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0;
        record->render_id = 0;
        record->scale_z = 0x1000;
        record->scale_y = 0x1000;
        record->scale_x = 0x1000;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        slot = effect_trail_next_slot;
        rows = effect_trail_rows[slot];
        *(KfEffectTrailRow **)&record->unknown_3c[4] = rows;
        effect_trail_next_slot = (slot + 1) & 3;
        for (index = 23; index != -1; index--, rows++) {
            rows->position = record->position;
            rows->rotation = record->rotation;
        }
        record->unknown_3c[8] = 0;
        record->unknown_3c[9] = 0;
        record->updates_remaining = 150;
        effect_play_spatial_sound(record, 0x22);
        break;
    }
    case 107: {
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0x23;
        record->render_id = 0x23;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
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
        *(u16 *)&record->unknown_3c[4] = va[1];
        effect_play_spatial_sound(record, 0x28);
        break;
    case 122:
        effect_pool_initialize_scaled(record, 0x2f, 0x1000);
        goto initialize_104_122;
    case 104:
        effect_pool_initialize_scaled(record, 0x10, 0x1000);
    initialize_104_122:
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
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        value = va[1];
        *(u16 *)&record->unknown_3c[4] = value / 4;
        break;
    }
    case 118:
    case 119: {
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
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
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x25;
        record->render_id = 0x25;
        record->unknown_3c[4] = 0;
    zero_scale_27_51_52:
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        break;
    case 2: {
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 9;
        record->render_id = 9;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_z = 0;
        record->scale_x = 0;
        *(u16 *)&record->unknown_3c[4] = va[1];
        *(u16 *)&record->unknown_3c[6] = va[2];
        third_parameter = va[3];
        three_parameter_sound = 0x1e;
        goto emit_three_parameter_sound;
    }
    case 20:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        break;
    case 12: {
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->unknown_09 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x22;
        record->render_id = 0x22;
        record->unknown_0c = 0x49;
        record->unknown_10 = 0x1000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->scale_z = 30000;
        record->scale_y = 30000;
        record->scale_x = 30000;
        *(u16 *)&record->unknown_3c[4] = va[2];
        *(u16 *)&record->unknown_3c[6] = va[3];
        *(u16 *)&record->unknown_3c[8] = va[4];
        *(u16 *)&record->unknown_3c[10] = va[5];
        record->updates_remaining = va[6];
        break;
    }
    case 100: {
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x22;
        record->render_id = 0x22;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->rotation.vy += (rand() >> 7) - 128;
        record->rotation.vx += (rand() >> 7) - 128;
        record->rotation.vz = 0;
        effect_play_spatial_sound(record, 0x29);
        break;
    }
    case 42: {
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0x11;
        record->render_id = 0x11;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        record->unknown_3c[4] = va[1];
        break;
    }
    case 113:
    case 115: {
        const SVECTOR *angles;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0xd;
        record->render_id = 0xd;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
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
        u8 parameter;

        effect_pool_initialize_scaled(record, 0x10, 0x1000);
        record->scale_y = 0;
        record->unknown_3c[4] = 0;
        parameter = va[1];
        record->direction.vy = 0;
        record->unknown_3c[5] = parameter;
        break;
    }
    case 45: {
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0x30;
        record->render_id = 0x30;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        effect_play_spatial_sound(record, 0x17);
        *(u16 *)&record->unknown_3c[4] = va[1];
        break;
    }
    case 116:
        record->unknown_08 = 0;
        record->updates_remaining = 20;
        break;
    case 117:
        record->base_render_id = 0x31;
        record->render_id = 0x31;
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->updates_remaining = 50;
        goto initialize_angles_34_35_117;
    case 40: {
        const SVECTOR *angles;

        record->base_render_id = 0xa;
        record->render_id = 0xa;
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
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

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x2c;
        record->render_id = 0x2c;
        record->updates_remaining = 50;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->rotation.vy += (rand() >> 6) - 256;
        random_x = rand();
        record->rotation.vz = 0;
        record->unknown_3c[4] = 0;
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
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->updates_remaining = 45;
    initialize_angles_34_35_117:
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->unknown_3c[4] = 0;
        effect_play_spatial_sound(record, 0x20);
        break;
    }
    case 50:
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        record->unknown_3c[4] = 0;
        effect_play_spatial_sound(record, 0x18);
        break;
    case 101: {
        s32 scale = va[1];
        s32 render_id = va[4];

        effect_pool_initialize_scaled(record, render_id, scale);
        *(u16 *)&record->unknown_3c[4] = *(u16 *)(va + 2);
        record->updates_remaining = *(u16 *)(va + 3);
        *(u16 *)&record->unknown_3c[6] = *(u16 *)(va + 5);
        break;
    }
    case 102: {
        u16 scale;
        s32 volume;
        s16 slot;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0xc;
        record->render_id = 0xc;
        record->scale_y = 0;
        scale = va[1];
        record->scale_z = scale;
        record->scale_x = scale;
        *(s16 *)&record->unknown_3c[4] = va[2];
        if ((s32)(DAT_8009a5a8 - cd_state.frame_count) >= 0) {
            break;
        }
        volume = *(s16 *)&record->unknown_3c[4] / 90;
        DAT_8009a5a8 = cd_state.frame_count + 30;
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
    case 15:
        effect_pool_initialize_fixed(record, 0x19);
        break;
    case 17:
        effect_pool_initialize_fixed(record, 0x1a);
        break;
    case 16:
        record->unknown_08 = 0;
        record->updates_remaining = 8;
        goto play_short_sound;
    case 14:
    case 19:
        record->unknown_08 = 0;
        record->updates_remaining = 16;
    play_short_sound:
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
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_09 = 1;
        record->base_render_id = 0xb;
        record->render_id = 0xb;
        record->unknown_0c = 0x44;
        record->unknown_10 = 0x1000;
        record->scale_z = 0;
        record->scale_y = 0;
        record->scale_x = 0;
        effect_play_spatial_sound(record, 0x1f);
        break;
    case 114: {
        VECTOR candidate_position;

        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->base_render_id = 0x26;
        record->render_id = 0x26;
        record->updates_remaining = 45;
        *(s32 *)&record->unknown_3c[8] = record->position.vy;
        candidate_position.vx = record->position.vx + (rand() >> 5) - 512;
        candidate_position.vz = record->position.vz + (rand() >> 5) - 512;
        if (func_8002b7f8(candidate_position.vx, record->position.vy,
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
        record->unknown_08 = 1;
        record->animation_clip = 0x80;
        record->unknown_0c = 0xff;
        record->render_id = record->base_render_id;
        angles = (const SVECTOR *)va[1];
        record->rotation = *angles;
        record->rotation.vz = 0;
        *(u16 *)&record->unknown_3c[6] = 0;
        break;
    }
    case 23: {
        effect_pool_initialize_scaled(record, 8, 0x400);
        record->direction.vz = 0;
        record->direction.vy = 0;
        record->direction.vx = 0;
        record->phase = 9;
        *(u16 *)&record->unknown_3c[4] = va[1];
        *(u16 *)&record->unknown_3c[6] = va[2];
        third_parameter = va[3];
        three_parameter_sound = 0x26;
    emit_three_parameter_sound:
        *(u16 *)&record->unknown_3c[8] = third_parameter;
        effect_play_spatial_sound(record, three_parameter_sound);
        break;
    }
    case 24:
        record->unknown_08 = 0;
        record->updates_remaining = 70;
        break;
    case 109: {
        effect_pool_initialize_scaled(record, 0xe, 0x400);
        record->updates_remaining = 20;
        record->unknown_3c[4] = va[1];
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
