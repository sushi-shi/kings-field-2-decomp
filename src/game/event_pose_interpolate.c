#include <kf/game/event_stream.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>
#include <kf/lib/math.h>

RODATA(0x80012890, 0x40)

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
    KfMapObject *destination, const VECTOR *start_position,
    const VECTOR *end_position, const SVECTOR *start_angles,
    const SVECTOR *end_angles, s32 fraction)
{
    if (start_position != 0) {
        destination->position.vx = fixed_lerp_q12(start_position->vx, end_position->vx, fraction);
        destination->position.vy = fixed_lerp_q12(start_position->vy, end_position->vy, fraction);
        destination->position.vz = fixed_lerp_q12(start_position->vz, end_position->vz, fraction);
    }

    if (start_angles != 0) {
        destination->rotation.vx = angle_lerp_shortest_q12(start_angles->vx, end_angles->vx, fraction);
        destination->rotation.vz = angle_lerp_shortest_q12(start_angles->vz, end_angles->vz, fraction);
    }
}

ADDRESS(0x800460a0, 0xa4)
void actor_animation_seek_phase(KfActor *actor, u8 state, u16 phase, s32 target_phase, s32 phase_step)
{
    s32 step;
    u32 half_step;
    s32 final_phase;

    if ((u16)phase_step == 0) {
        return;
    }

    step = phase_step & 0xfffe;
    half_step = (u32)step >> 1;
    final_phase = target_phase - half_step;
    actor->animation_id = state;
    actor->animation_phase = phase;

    while (!angle_within_tolerance(actor->animation_phase, (u16)final_phase, half_step)) {
        actor->animation_phase = (step + actor->animation_phase) & 0xfff;
        render_game_frame(0, 0);
    }

    actor->animation_phase = final_phase & 0xfff;
    render_game_frame(0, 0);
}

ADDRESS(0x80046144, 0x5c)
u8 event_target_stream_find_marker(const KfTargetCandidate *candidate, u8 marker)
{
    const u8 *cursor = candidate->word_14.bytes;

    for (;;) {
        s32 code = *cursor++;

        if (code == 0xf2) {
            goto marker_record;
        }
        if (code == 0xff) {
            return candidate->word_10.bytes.fallback_offset;
        }
        continue;

marker_record:
        if (*cursor == marker) {
            const u8 *base = &candidate->word_12.bytes.marker_state;
            return cursor - base;
        }
        cursor++;
    }
}

ADDRESS(0x800461a0, 0x11c)
u8 *event_target_stream_resolve_cursor(KfActor *actor)
{
    KfTargetCandidate *candidate =
        actor_state.target_groups[actor->group_index].targets[0].pointer;
    u8 *cursor = candidate->word_14.bytes;
    u8 *marker = cursor + 3;

    for (;;) {
        u8 code = *cursor;

        if (code == 0xf1) {
            goto marker_record;
        }
        if (code != 0xfe) {
            /* Retail retries this byte; the stream must supply a control code. */
            continue;
        }
        if (candidate->word_10.bytes.fallback_offset == 0) {
            cursor++;
            candidate->word_10.bytes.fallback_offset = cursor - candidate->word_14.bytes;
            return cursor;
        }
use_fallback:
        return candidate->word_14.bytes + candidate->word_10.bytes.fallback_offset;

marker_record:
        if (event_state.control.bytes[cursor[1]] == cursor[2]) {
            u8 offset = event_target_stream_find_marker(candidate, *marker);
            if (candidate->word_10.bytes.fallback_offset < offset) {
                candidate->word_10.bytes.fallback_offset = offset;
            }
            event_state.control.fields.stream_actor_definition_id = actor->definition_id;
            candidate->word_12.bytes.marker_state = 0;
            goto use_fallback;
        }
        marker += 4;
        cursor += 4;
    }
}

ADDRESS(0x800462bc, 0x444)
void event_target_stream_execute(KfActor *actor)
{
    KfTargetCandidate *candidate =
        actor_state.target_groups[actor->group_index].targets[0].pointer;
    u8 *cursor;
    s32 repeat;
    s32 restore_state;
    u8 saved_state;
    s32 old_counter;
    s32 choice;

    if (candidate == 0) {
        return;
    }
    if (candidate->type != 0x70) {
        return;
    }
    restore_state = 0;
    repeat = 0;
    if (candidate->word_10.bytes.fallback_offset == 0) {
        event_state.control.fields.stream_actor_definition_id = actor->definition_id;
    }
    cursor = event_target_stream_resolve_cursor(actor);
    if (event_state.control.fields.stream_actor_definition_id != actor->definition_id &&
        candidate->word_12.bytes.marker_state == 1) {
        while (*cursor++ != 0xf0) {
        }
        cursor++;
        candidate->word_10.bytes.fallback_offset = cursor - candidate->word_14.bytes;
        candidate->word_12.bytes.marker_state = 0;
    }

    for (;;) {
        switch (*cursor - 0xf0) {
        case 0:
            candidate->word_12.bytes.marker_state = 1;
            /* The two rewind opcodes share their byte-count operand. */
        case 8:
        {
            u8 count = cursor[1];
            candidate->word_10.bytes.fallback_offset -= count;
            cursor -= count;
            break;
        }
        case 9:
            if (event_state.control.bytes[cursor[1]] == cursor[2]) {
                candidate->word_10.bytes.fallback_offset = event_target_stream_find_marker(candidate, cursor[3]);
                cursor = candidate->word_14.bytes + candidate->word_10.bytes.fallback_offset;
            } else {
                cursor += 4;
                candidate->word_10.bytes.fallback_offset += 4;
            }
            break;
        case 2:
            cursor += 2;
            candidate->word_10.bytes.fallback_offset += 2;
            break;
        case 3:
            goto advance;
        case 4:
            cursor++;
            candidate->word_10.bytes.fallback_offset++;
            state_8017d118.active_table[4](actor, *cursor);
            goto advance;
        case 5:
            cursor++;
            candidate->word_10.bytes.fallback_offset++;
            repeat = *cursor;
            goto advance;
        case 6:
            event_state.control.fields.stream_actor_definition_id = actor->definition_id;
            cursor++;
            candidate->word_10.bytes.fallback_offset++;
            candidate->word_12.bytes.marker_state = 0;
            break;
        case 7:
            event_state.control.bytes[cursor[1]] = cursor[2];
            cursor += 2;
            candidate->word_10.bytes.fallback_offset += 2;
            goto advance;
        case 15:
            goto after_script;
        default:
            goto execute;
        }
        continue;

execute:
        if (restore_state == 0 && candidate->animation_id != 0xff) {
            u16 phase = actor->animation_phase;
            saved_state = actor->animation_id;
            restore_state = 1;
            if (phase != 0) {
                actor_animation_seek_phase(actor, actor->animation_id,
                              phase, 0, actor->animation_step);
            }
            actor_animation_seek_phase(actor, candidate->animation_id, 0, 0xfff,
                          candidate->animation_step);
        }
        menu_show_transition_image(3, candidate->word_0c.value + *cursor);

advance:
        cursor++;
        candidate->word_10.bytes.fallback_offset++;
        if (repeat != 0) {
            repeat--;
            continue;
        }
        break;
    }

after_script:
    old_counter = game_counter_bytes[0x53];
    switch (candidate->word_12.bytes.unknown_12 & 0xf0) {
    case 0:
        player_render_frame_and_release_pool();
        menu_item_buy_sell_controller(candidate->word_12.bytes.unknown_12 & 0xf);
        break;
    case 0x10:
        player_render_frame_and_release_pool();
        menu_item_stock_choice_controller();
        break;
    case 0x20:
        player_render_frame_and_release_pool();
        menu_item_trade_controller();
        break;
    case 0x30:
        player_render_frame_and_release_pool();
        choice = menu_choose_inventory_item();
        if (choice != -1) {
            render_game_frame(0, 0);
            menu_show_transition_image(6, choice + 360);
        }
        break;
    }
    if (game_counter_bytes[0x53] < old_counter) {
        event_state.control.bytes[0x1c] = 1;
    }
    event_state.control.fields.stream_actor_definition_id = actor->definition_id;
    if (restore_state != 0 && candidate->word_10.bytes.unknown_11 != 0xff) {
        actor_animation_seek_phase(actor, candidate->word_10.bytes.unknown_11, 0, 0xfff,
                      candidate->word_0e.value);
        actor->animation_id = saved_state;
    }
    event_state.state_word = 1;
    player_clear_motion();
}
