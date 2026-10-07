#ifndef KF_AUDIO_SOUND_H
#define KF_AUDIO_SOUND_H

#include <kf/platform/types.h>

#include <cstddef>

namespace kf {
struct SoundBank;
struct MusicSequence;
using SoundVoice = u32;
inline constexpr SoundVoice no_sound_voice = 0;
enum class ReverbPreset : u8 { Studio, Hall };

bool sound_start();
void sound_shutdown();
void sound_poll();
void sound_set_paused(bool paused);
void sound_reset(ReverbPreset preset, s16 depth_left, s16 depth_right);
// Changes the reverb without releasing banks, sequences or voices.
void sound_set_reverb(ReverbPreset preset, s16 depth_left, s16 depth_right);
void sound_master_volume(s16 left, s16 right);
SoundBank *sound_bank_load(const u8 *header, std::size_t header_size,
    const u8 *body, std::size_t body_size);
void sound_bank_release(SoundBank *bank);
SoundVoice sound_voice_play(SoundBank *bank, s16 program, s16 tone, s16 note, s16 left, s16 right);
void sound_voice_release(SoundVoice voice);
// Hardware voice numbers, as the original key-on and key-status calls use them.
int sound_voice_index(SoundVoice voice);
bool sound_voice_index_active(int index);
void sound_voice_index_release(int index);
void sound_note_play(SoundBank *bank, s16 program, s16 note, s16 left, s16 right);
void sound_note_release(SoundBank *bank, s16 program, s16 note);
MusicSequence *sound_sequence_load(const u8 *data, std::size_t size, SoundBank *bank);
void sound_sequence_play(MusicSequence *sequence);
// Plays a score the given number of times; zero repeats it indefinitely.
void sound_sequence_play_count(MusicSequence *sequence, unsigned count);
void sound_sequence_stop(MusicSequence *sequence);
void sound_sequence_pause(MusicSequence *sequence, bool paused);
void sound_sequence_volume(MusicSequence *sequence, s16 left, s16 right);
void sound_sequence_release(MusicSequence *sequence);
}

#endif // KF_AUDIO_SOUND_H
