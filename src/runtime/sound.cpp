#include "runtime.h"

#include <kf/audio/sound.h>
#include <kf/psx/sdk.h>

#include <array>
#include <cstring>
#include <vector>

// Sound library calls on the software mixer from the King's Field port. Bank
// and sequence handles keep the library's numbering; voices keep hardware
// numbers so key-off and key-status calls address the same voice.

namespace kf::psx {
namespace {
constexpr std::size_t bank_capacity = 16;
constexpr std::size_t sequence_capacity = 32;
constexpr std::size_t vab_header_fixed_bytes = 0x20 + 128 * 16;
constexpr std::size_t vab_tone_block_bytes = 16 * 32;
constexpr std::size_t vab_sample_table_bytes = 256 * 2;
constexpr std::size_t spu_voice_count = 24;
constexpr short transfer_more_data = -2;

struct Bank {
    bool open;
    std::vector<u8> header;
    std::vector<u8> body;
    std::size_t body_size;
    SoundBank *loaded;
};
struct Sequence {
    MusicSequence *score;
};
std::array<Bank, bank_capacity> banks;
std::array<Sequence, sequence_capacity> sequences;
ReverbPreset reverb_preset = ReverbPreset::Studio;
s16 reverb_depth_left, reverb_depth_right;
bool reverb_on;

u16 le16(const u8 *bytes) { return static_cast<u16>(bytes[0] | (bytes[1] << 8)); }

// Scores hold a reference to their bank, so the mixer keeps it until both go.
void close_bank(Bank &bank) {
    if (bank.loaded)
        sound_bank_release(bank.loaded);
    bank = {};
}

bool finish_bank(Bank &bank) {
    bank.loaded = sound_bank_load(bank.header.data(), bank.header.size(), bank.body.data(), bank.body.size());
    if (!bank.loaded) {
        warn_once("a sound bank that the mixer rejected");
        return false;
    }
    bank.body.clear();
    bank.body.shrink_to_fit();
    return true;
}

void apply_reverb() {
    sound_set_reverb(reverb_preset, reverb_on ? reverb_depth_left : 0, reverb_on ? reverb_depth_right : 0);
}

Bank *find_bank(short id) {
    if (id < 0 || static_cast<std::size_t>(id) >= banks.size() || !banks[id].open)
        return nullptr;
    return &banks[id];
}

MusicSequence *find_score(short id) {
    if (id < 0 || static_cast<std::size_t>(id) >= sequences.size())
        return nullptr;
    return sequences[id].score;
}

void reset_library() {
    for (auto &sequence : sequences) {
        if (sequence.score)
            sound_sequence_release(sequence.score);
        sequence = {};
    }
    for (auto &bank : banks)
        close_bank(bank);
    sound_reset(reverb_preset, 0, 0);
    reverb_on = false;
}
}

void sound_update() { sound_poll(); }
}

using namespace kf;
using namespace kf::psx;

extern "C" {
void SsInit(void) { reset_library(); }
void SsSetTickMode(long) {}
void SsSetTableSize(char *, short, short) {}
void SsStart2(void) {}
void SsEnd(void) {}

short SsUtSetReverbType(short type) {
    reverb_preset = type == SS_REV_TYPE_HALL ? ReverbPreset::Hall : ReverbPreset::Studio;
    apply_reverb();
    return type;
}

void SsUtReverbOn(void) {
    reverb_on = true;
    apply_reverb();
}

void SsUtSetReverbDepth(short left, short right) {
    reverb_depth_left = left;
    reverb_depth_right = right;
    apply_reverb();
}

void SsSetMVol(short left, short right) { sound_master_volume(left, right); }

short SsVabOpenHead(unsigned char *addr, short vab_id) {
    if (std::memcmp(addr, "pBAV", 4) != 0)
        return -1;
    short id = vab_id;
    if (id < 0) {
        for (std::size_t i = 0; i < banks.size(); ++i)
            if (!banks[i].open) {
                id = static_cast<short>(i);
                break;
            }
    }
    if (id < 0 || static_cast<std::size_t>(id) >= banks.size())
        return -1;
    close_bank(banks[id]);
    const std::size_t programs = le16(addr + 0x12);
    const std::size_t samples = le16(addr + 0x16);
    if (programs > 128 || samples > 255)
        return -1;
    const std::size_t header_size = vab_header_fixed_bytes + programs * vab_tone_block_bytes + vab_sample_table_bytes;
    // The body is the sample table's total; the header's file size can be smaller.
    std::size_t body_size = 0;
    const u8 *table = addr + header_size - vab_sample_table_bytes;
    for (std::size_t sample = 1; sample <= samples; ++sample)
        body_size += std::size_t(le16(table + sample * 2)) * 8;
    auto &bank = banks[id];
    bank.open = true;
    bank.header.assign(addr, addr + header_size);
    bank.body_size = body_size;
    return id;
}

short SsVabTransBody(unsigned char *addr, short vab_id) {
    auto *bank = find_bank(vab_id);
    if (!bank)
        return -1;
    bank->body.assign(addr, addr + bank->body_size);
    return finish_bank(*bank) ? vab_id : -1;
}

short SsVabTransBodyPartly(unsigned char *addr, unsigned long bufsize, short vab_id) {
    auto *bank = find_bank(vab_id);
    if (!bank || bank->loaded)
        return -1;
    const std::size_t remaining = bank->body_size - bank->body.size();
    const std::size_t count = remaining < bufsize ? remaining : bufsize;
    bank->body.insert(bank->body.end(), addr, addr + count);
    if (bank->body.size() < bank->body_size)
        return transfer_more_data;
    return finish_bank(*bank) ? vab_id : -1;
}

short SsVabTransCompleted(short) { return 1; }

void SsVabClose(short vab_id) {
    if (auto *bank = find_bank(vab_id))
        close_bank(*bank);
}

short SsSeqOpen(unsigned long *addr, short vab_id) {
    auto *bank = find_bank(vab_id);
    if (!bank || !bank->loaded)
        return -1;
    const auto *bytes = reinterpret_cast<const u8 *>(addr);
    // The library reads the score to its end marker; bound the scan generously.
    constexpr std::size_t maximum_score_bytes = 0x40000;
    for (std::size_t i = 0; i < sequences.size(); ++i) {
        if (sequences[i].score)
            continue;
        sequences[i].score = sound_sequence_load(bytes, maximum_score_bytes, bank->loaded);
        if (!sequences[i].score) {
            warn_once("a sequence that the mixer rejected");
            return -1;
        }
        return static_cast<short>(i);
    }
    return -1;
}

void SsSeqPlay(short seq_access_num, char play_mode, short l_count) {
    auto *score = find_score(seq_access_num);
    if (!score)
        return;
    sound_sequence_play_count(score, l_count > 0 ? static_cast<unsigned>(l_count) : 0u);
    if (play_mode == SSPLAY_PAUSE)
        sound_sequence_pause(score, true);
}

void SsSeqStop(short seq_access_num) { sound_sequence_stop(find_score(seq_access_num)); }

void SsSeqClose(short seq_access_num) {
    if (auto *score = find_score(seq_access_num)) {
        sound_sequence_release(score);
        sequences[seq_access_num] = {};
    }
}

void SsSeqSetVol(short seq_access_num, short voll, short volr) {
    sound_sequence_volume(find_score(seq_access_num), voll, volr);
}

void SsSeqPause(short seq_access_num) { sound_sequence_pause(find_score(seq_access_num), true); }
void SsSeqReplay(short seq_access_num) { sound_sequence_pause(find_score(seq_access_num), false); }
void SsSeqCalledTbyT(void) { sound_poll(); }

short SsUtKeyOn(short vab_id, short prog, short tone, short note, short, short voll, short volr) {
    auto *bank = find_bank(vab_id);
    if (!bank || !bank->loaded)
        return -1;
    const auto voice = sound_voice_play(bank->loaded, prog, tone, note, voll, volr);
    return static_cast<short>(sound_voice_index(voice));
}

short SsUtKeyOff(short voice, short, short, short, short) {
    sound_voice_index_release(voice);
    return 0;
}

void SpuGetAllKeysStatus(char *status) {
    for (std::size_t i = 0; i < spu_voice_count; ++i)
        status[i] = sound_voice_index_active(static_cast<int>(i)) ? 1 : 0;
}
}
