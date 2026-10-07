#ifndef KF_AUDIO_CODEC_RESULT_H
#define KF_AUDIO_CODEC_RESULT_H

#include <kf/platform/types.h>

namespace kf {
enum class KfCodecResult : s32 {
    KF_CODEC_OK = 0,
    KF_CODEC_END = 1,
    KF_CODEC_INVALID = 2,
    KF_CODEC_OUTPUT_FULL = 3
};
using enum KfCodecResult;
}

#endif // KF_AUDIO_CODEC_RESULT_H
