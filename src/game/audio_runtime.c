#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/cd.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>
#include <kf/lib/audio.h>
#include <kf/lib/math.h>
#include <psyq/audio.h>
#include <psyq/kernel.h>

/* SDK-required 2-by-1 sequence workspace; original allocation extent is WIP. */
DATA(0x8009a6a0, 0x158)
char audio_sequence_table[SS_SEQ_TABSIZ * KF_AUDIO_SEQUENCE_CAPACITY * KF_AUDIO_TRACKS_PER_SEQUENCE];

/* Five pooled VAB headers and the dedicated stream-slot-6 header use 0x1000 bytes each. */
DATA(0x80164a68, 0x6000)
static u8 audio_vab_stream_buffers[6][0x1000];
typedef char kf_audio_vab_stream_buffers_size[
    sizeof(audio_vab_stream_buffers) == 0x6000 ? 1 : -1];

DATA(0x80194e30, 0x2800)
static u8 audio_main_vab_header_buffer[0x2800];

DATA(0x80197630, 0xe9c)
KfGameAudioState audio_state;

DATA(0x80198640, 0x3000)
static u8 audio_sequence_buffer[0x3000];

ADDRESS(0x800139c4, 0x120)
void audio_initialize_runtime(void)
{
    KfAudioVabSlot *vab_slot;
    KfAudioVoiceHandle *voice;
    KfAudioVabStreamSlot *stream_slot;
    u8 *stream_buffer;
    s32 index;

    SsInit();
    SsSetMVol(0, 0);
    SsSetTableSize(audio_sequence_table, KF_AUDIO_SEQUENCE_CAPACITY,
        KF_AUDIO_TRACKS_PER_SEQUENCE);
    SsSetTickMode(1);
    SsStart2();
    SsUtSetReverbType(4);
    SsUtReverbOn();
    SsUtSetReverbDepth(0x28, 0x28);

    audio_state.sequence_buffer = (u_long *)audio_sequence_buffer;
    audio_state.sequence_active = 0;
    audio_state.sequence_ready = 0;
    vab_slot = audio_state.vab_slots;
    index = 129;
    do {
        vab_slot->vab_id = -1;
        vab_slot->stream_slot = 0;
        vab_slot++;
        index--;
    } while (index != -1);

    voice = audio_state.voices.handles;
    index = 9;
    do {
        voice->voice_id = -1;
        voice++;
        index--;
    } while (index != -1);

    stream_slot = audio_state.vab_stream_slots;
    index = 0;
    stream_buffer = audio_vab_stream_buffers[1];
    for (; index < 7; index++) {
        stream_slot->state = 0;
        stream_slot->buffer = stream_buffer;
        stream_slot++;
        stream_buffer += 0x1000;
    }
    audio_state.vab_stream_slots[5].buffer = audio_main_vab_header_buffer;
    audio_state.vab_stream_slots[6].buffer = audio_vab_stream_buffers[0];
}

ADDRESS(0x80013ae4, 0x98)
void audio_start_sequence(void)
{
    if (player_state.audio_music_enabled != 0 && audio_state.sequence_ready != 0) {
        audio_state.sequence_id = SsSeqOpen(
            audio_state.sequence_buffer, audio_state.vab_slots[1].vab_id);
        SsSeqSetVol(audio_state.sequence_id, 0x3c, 0x3c);
        SsSeqPlay(audio_state.sequence_id, 1, 0);
        audio_state.sequence_active = 1;
        SsSetMVol(0x7f, 0x7f);
    }
}

ADDRESS(0x80013b7c, 0x58)
void audio_stop_sequence(void)
{
    if (audio_state.sequence_active == 1) {
        SsSeqStop(audio_state.sequence_id);
        SsSeqClose(audio_state.sequence_id);
        audio_state.sequence_active = 0;
    }
}

ADDRESS(0x80013bd4, 0xb8)
void audio_shutdown(void)
{
    KfAudioVabSlot *slot;
    s32 index;

    SsSetMVol(0, 0);
    if (audio_state.sequence_active == 1) {
        SsSeqSetVol(audio_state.sequence_id, 0, 0);
        SsSeqStop(audio_state.sequence_id);
        SsSeqClose(audio_state.sequence_id);
    }
    slot = audio_state.vab_slots;
    index = 129;
    do {
        if (slot->vab_id != -1) {
            SsVabClose(slot->vab_id);
        }
        slot++;
        index--;
    } while (index != -1);
    SsEnd();
}

ADDRESS(0x80013c8c, 0x2c4)
KfAudioPlaybackResult audio_play_spatial(
    s32 sound, const VECTOR *position, s32 volume, s32 max_distance,
    s32 attenuation_distance, s32 note_offset)
{
    s32 delta_x = (position->vx - audio_state.listener_position.vx) >> 3;
    s32 delta_y = (position->vy - audio_state.listener_position.vy) >> 3;
    s32 delta_z = (position->vz - audio_state.listener_position.vz) >> 3;
    s32 attenuation = SquareRoot0(delta_x * delta_x + delta_y * delta_y +
                                  delta_z * delta_z) << 3;
    s32 level;
    s32 angle;
    s32 listener_angle;
    s32 left;
    s32 right;
    s32 alternate_pan = sound & 0x8000;
    sound &= 0xfff;

    if (attenuation >= max_distance) {
        return KF_AUDIO_NOT_PLAYED;
    }

    attenuation = ((attenuation_distance - attenuation) << 7) /
                  attenuation_distance;
    level = (attenuation * volume) >> 7;
    collision_sample_map_cell_layer(position->vx, position->vy, position->vz);
    if ((u16)KF_COLLISION_CACHE_LAYER != audio_state.listener_layer) {
        level = (attenuation * volume) >> 8;
    }
    if (level < 20) {
        return KF_AUDIO_NOT_PLAYED;
    }
    if (level >= 128) {
        level = 127;
    }

    angle = vector_xz_to_angle(
        position->vx - audio_state.listener_position.vx,
        position->vz - audio_state.listener_position.vz);
    listener_angle = audio_state.listener_rotation.vy - 1024;
    angle = (angle - listener_angle) & 0xfff;
    if (angle >= 2048) {
        angle = 4096 - angle;
    }
    angle >>= 1;
    if (attenuation >= 64 && !alternate_pan) {
        angle = (((angle - 512) * (256 - 2 * attenuation)) >> 7) + 512;
    }

    left = (level * rsin(angle)) / 0xd48;
    if (left >= 128) {
        left = 127;
    }
    right = (level * rcos(angle)) / 0xd48;
    if (right >= 128) {
        right = 127;
    }
    audio_key_on(sound, left, right, note_offset);
    return KF_AUDIO_PLAYED;
}

ADDRESS(0x80013f50, 0x34)
KfAudioPlaybackResult audio_play_spatial_default_range(
    s32 sound, const VECTOR *position, s16 volume, s32 note_offset)
{
    return audio_play_spatial(sound, position, volume, KF_AUDIO_DEFAULT_MAX_DISTANCE,
        KF_AUDIO_DEFAULT_ATTENUATION_DISTANCE, note_offset);
}

ADDRESS(0x80013f84, 0x34)
KfAudioPlaybackResult audio_play_spatial_range(
    s32 sound, const VECTOR *position, s16 volume, s32 max_distance,
    s32 attenuation_distance, s32 note_offset)
{
    return audio_play_spatial(
        sound, position, volume, max_distance, attenuation_distance, note_offset);
}

ADDRESS(0x80013fb8, 0x78)
void audio_key_off_handle(KfAudioVoiceHandle *handle)
{
    KfGameAudioState *state;
    KfAudioVoiceParams *voice = &audio_state.voices.params[(u8)handle->sound_id];
    KfAudioVabSlot *vab;

    state = &audio_state;
    vab = &state->vab_slots[voice->vab_slot_index];

    if (vab->vab_id != -1 && vab->vab_id != 0xfe) {
        SsUtKeyOff(handle->voice_id, vab->vab_id, voice->program, voice->tone, voice->note);
    }
}

ADDRESS(0x80014030, 0xac)
void audio_update_listener(const VECTOR *position, const SVECTOR *rotation)
{
    if (position != 0) {
        audio_state.listener_position = *position;
        collision_sample_map_cell_layer(position->vx, position->vy, position->vz);
        audio_state.listener_layer = KF_COLLISION_CACHE_LAYER;
    }
    if (rotation != 0) {
        audio_state.listener_rotation = *rotation;
    }
}

ADDRESS(0x800140dc, 0x24)
void audio_play_sound(s32 sound, s32 volume)
{
    audio_key_on(sound, volume, volume, 0);
}

ADDRESS(0x80014100, 0x64)
void audio_refresh_voice_handles(void)
{
    u8 status[24];
    KfAudioVoiceHandle *handle;
    s32 index;

    SpuGetAllKeysStatus(status);
    handle = audio_state.voices.handles;
    index = 9;
    do {
        if (handle->voice_id != -1 && status[handle->voice_id] == 0) {
            handle->voice_id = -1;
        }
        handle++;
        index--;
    } while (index != -1);
}

ADDRESS(0x80014164, 0x114)
KfAudioVoiceHandle *audio_allocate_voice_handle(s32 sound_id)
{
    KfAudioVoiceHandle *handle;
    KfAudioVoiceHandle *oldest;
    KfAudioVoiceState *voices;
    KfAudioVoiceParams *params;
    s32 lowest_age;
    s32 index;

    audio_refresh_voice_handles();
    handle = audio_state.voices.handles;
    index = 9;
    do {
        if (handle->voice_id == -1) {
            return handle;
        }
        handle++;
        index--;
    } while (index != -1);

    handle = audio_state.voices.handles;
    index = 9;
    do {
        if (handle->sound_id == sound_id) {
            audio_key_off_handle(handle);
            handle->voice_id = -1;
            return handle;
        }
        handle++;
        index--;
    } while (index != -1);

    lowest_age = 0x10000;
    voices = &audio_state.voices;
    handle = voices->handles;
    index = 9;
    params = voices->params;
    do {
        u16 age = params[(u8)handle->sound_id].age;

        if (age < lowest_age) {
            lowest_age = age;
            oldest = handle;
        }
        handle++;
        index--;
    } while (index != -1);
    audio_key_off_handle(oldest);
    oldest->voice_id = -1;
    return oldest;
}

ADDRESS(0x80014278, 0x11c)
void audio_key_on(s32 sound, s32 left_volume, s32 right_volume, s32 note_offset)
{
    KfAudioVoiceParams *voice;
    KfAudioVabSlot *vab;
    KfAudioVoiceHandle *handle;

    if (sound == 0xff || player_state.audio_effects_enabled == 0) {
        return;
    }
    voice = &audio_state.voices.params[(u8)sound];
    if (voice->vab_slot_index == -1) {
        return;
    }
    vab = &audio_state.vab_slots[voice->vab_slot_index];
    if (vab->vab_id == -1 || vab->vab_id == 0xfe) {
        return;
    }
    handle = audio_allocate_voice_handle(sound);
    note_offset += voice->note;
    if (note_offset < 0) {
        note_offset = 0;
    } else if (note_offset > 0x7f) {
        note_offset = 0x7f;
    }
    handle->voice_id = SsUtKeyOn(vab->vab_id, voice->program, voice->tone,
        note_offset, 0, left_volume, right_volume);
    handle->sound_id = sound;
}

ADDRESS(0x80014394, 0x124)
void audio_vab_stream_callback(KfCdRequest *request)
{
    KfAudioVabSlot *vab_slot;

    switch (request->payload.vab.phase) {
    case 0:
        vab_slot = &audio_state.vab_slots[request->payload.vab.slot_index];
        if (cd_sectors_corrupt(
                (u32 *)request->payload.vab.stream_state.vab_stream_slot->buffer,
                request->sector_count)) {
            request->phase = 0;
            CdSeekP(&request->initial_location);
            return;
        }
        if (vab_slot->vab_id != -1) {
            SsVabClose(vab_slot->vab_id);
        }
        vab_slot->vab_id = SsVabOpenHead(
            request->payload.vab.stream_state.vab_stream_slot->buffer, -1);
        if (vab_slot->vab_id == -1) {
            cd_request_advance(request);
            return;
        }
        cd_location_add(&request->location, request->sector_count, &request->location);
        request->payload.vab.phase = 1;
        request->phase = 0;
        request->destination = (u_long *)cd_stream_work_buffer;
        request->sector_count = 0x12;
        CdSeekP(&request->location);
        break;
    case 1:
        request->payload.vab.phase = 2;
        break;
    }
}

ADDRESS(0x800144b8, 0x13c)
void cd_request_service_vab(void)
{
    KfCdRequest *request;
    KfAudioVabSlot *vab_slot;
    CdlLOC *location;
    s16 result;

    EnterCriticalSection();
    request = cd_state.current;
    if (request->kind == KF_CD_REQUEST_VAB_READ && request->payload.vab.phase == 2) {
        ExitCriticalSection();
        location = &request->location;
        vab_slot = &audio_state.vab_slots[request->payload.vab.slot_index];
        for (;;) {
            result = SsVabTransBodyPartly(request->destination, 0x9000, vab_slot->vab_id);
            if (result != -1) {
                break;
            }
            SsVabClose(vab_slot->vab_id);
            cd_request_advance(request);
        }
        if (result == -2) {
            request->phase = 0;
            request->payload.vab.phase = 1;
            cd_location_add(location, request->sector_count, location);
            CdSeekP(location);
            SsVabTransCompleted(1);
        } else if (result == vab_slot->vab_id) {
            cd_request_advance(request);
            SsVabTransCompleted(1);
            request->payload.vab.stream_state.vab_stream_slot->state = 1;
            request->sector_count = 0;
        }
    } else {
        ExitCriticalSection();
    }
}

ADDRESS(0x800145f4, 0xdc)
KfAudioVabStreamSlot *audio_acquire_vab_stream_slot(void)
{
    KfAudioVabStreamSlot *stream_slot;
    KfAudioVabSlot *vab_slot;
    s32 index;

    stream_slot = audio_state.vab_stream_slots;
    index = 4;
    do {
        if (stream_slot->state == 0) {
            return stream_slot;
        }
        index--;
        stream_slot++;
    } while (index != -1);

    stream_slot = audio_state.vab_stream_slots;
    index = 4;
    do {
        if (stream_slot->state == 2) {
            vab_slot = audio_state.vab_slots;
            index = 129;
            do {
                if (vab_slot->vab_id != -1 && vab_slot->stream_slot == stream_slot) {
                    SsVabClose(vab_slot->vab_id);
                    vab_slot->vab_id = -1;
                    vab_slot->stream_slot = 0;
                    break;
                }
                index--;
                vab_slot++;
            } while (index != -1);
            stream_slot->state = 0;
            return stream_slot;
        }
        index--;
        stream_slot++;
    } while (index != -1);
    return 0;
}

ADDRESS(0x800146d0, 0xd0)
void audio_queue_vab_stream(s32 archive_slot, s32 entry, s32 vab_slot_index)
{
    KfAudioVabSlot *vab_slot;
    KfAudioVabStreamSlot *stream_slot;
    KfCdRequest *request;

    vab_slot = &audio_state.vab_slots[vab_slot_index];
    switch (vab_slot_index) {
    case 0:
        stream_slot = &audio_state.vab_stream_slots[6];
        break;
    case 1:
        stream_slot = &audio_state.vab_stream_slots[5];
        break;
    default:
        stream_slot = audio_acquire_vab_stream_slot();
        if (stream_slot == 0) {
            return;
        }
        vab_slot->vab_id = 0xfe;
        break;
    }
    vab_slot->stream_slot = stream_slot;
    stream_slot->state = 3;
    request = cd_state.tail;
    cd_request_wait_done(request);
    request->payload.vab.phase = 0;
    request->payload.vab.stream_state.vab_stream_slot = stream_slot;
    request->payload.vab.slot_index = vab_slot_index;
    cd_archive_queue_stream_read(
        archive_slot, entry * 2, (u_long *)stream_slot->buffer,
        audio_vab_stream_callback);
}
