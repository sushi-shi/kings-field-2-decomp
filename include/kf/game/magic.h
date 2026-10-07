#ifndef KF_GAME_MAGIC_H
#define KF_GAME_MAGIC_H

#include <kf/lib/types.h>
#include <kf/lib/enum.h>

/*
 * Spell and effect identities share one namespace (KF1 KfEffectKind): the
 * magic shortcuts, queued magic action and weapon magic IDs are passed
 * unchanged to effect_construct_record, whose record kind indexes
 * magic_records for kinds below KF_MAGIC_RECORD_COUNT. Kinds from 100 are
 * child or variant effects without a magic row; actor_dispatch_group_effect
 * also accepts 108, 110, 112 and 123, which spawn actors or remap to another
 * kind instead of constructing themselves. Members without behavioural
 * evidence keep their decimal encoding as a WIP name.
 */
KF_ENUM_BEGIN(KfEffectKind, u8)
    KF_EFFECT_KIND_0 = 0,
    KF_EFFECT_KIND_1 = 1,
    KF_EFFECT_KIND_2 = 2,
    KF_EFFECT_KIND_3 = 3,
    KF_EFFECT_KIND_4 = 4,
    KF_EFFECT_KIND_5 = 5,
    KF_EFFECT_KIND_6 = 6,
    KF_EFFECT_KIND_7 = 7,
    KF_EFFECT_KIND_8 = 8,
    KF_EFFECT_KIND_9 = 9,
    KF_EFFECT_KIND_10 = 10,
    KF_EFFECT_KIND_11 = 11,
    KF_EFFECT_KIND_12 = 12,
    KF_EFFECT_KIND_13 = 13,
    KF_EFFECT_KIND_14 = 14,
    KF_EFFECT_KIND_DEFENSE_BOOST = 15,
    KF_EFFECT_KIND_16 = 16,
    KF_EFFECT_KIND_ATTACK_BOOST = 17,
    KF_EFFECT_KIND_18 = 18,
    KF_EFFECT_KIND_19 = 19,
    KF_EFFECT_KIND_20 = 20,
    KF_EFFECT_KIND_22 = 22,
    KF_EFFECT_KIND_23 = 23,
    KF_EFFECT_KIND_24 = 24,
    KF_EFFECT_KIND_25 = 25,
    KF_EFFECT_KIND_26 = 26,
    KF_EFFECT_KIND_27 = 27,
    KF_EFFECT_KIND_28 = 28,
    KF_EFFECT_KIND_29 = 29,
    KF_EFFECT_KIND_30 = 30,
    KF_EFFECT_KIND_31 = 31,
    KF_EFFECT_KIND_32 = 32,
    KF_EFFECT_KIND_33 = 33,
    KF_EFFECT_KIND_34 = 34,
    KF_EFFECT_KIND_35 = 35,
    KF_EFFECT_KIND_38 = 38,
    KF_EFFECT_KIND_39 = 39,
    KF_EFFECT_KIND_40 = 40,
    KF_EFFECT_KIND_42 = 42,
    KF_EFFECT_KIND_43 = 43,
    KF_EFFECT_KIND_44 = 44,
    KF_EFFECT_KIND_45 = 45,
    KF_EFFECT_KIND_46 = 46,
    KF_EFFECT_KIND_47 = 47,
    KF_EFFECT_KIND_48 = 48,
    KF_EFFECT_KIND_49 = 49,
    KF_EFFECT_KIND_50 = 50,
    KF_EFFECT_KIND_51 = 51,
    KF_EFFECT_KIND_52 = 52,
    KF_EFFECT_KIND_53 = 53,
    KF_EFFECT_KIND_54 = 54,
    KF_EFFECT_KIND_100 = 100,
    KF_EFFECT_KIND_101 = 101,
    KF_EFFECT_KIND_102 = 102,
    KF_EFFECT_KIND_103 = 103,
    KF_EFFECT_KIND_104 = 104,
    KF_EFFECT_KIND_105 = 105,
    KF_EFFECT_KIND_106 = 106,
    KF_EFFECT_KIND_107 = 107,
    KF_EFFECT_KIND_108 = 108,
    KF_EFFECT_KIND_109 = 109,
    KF_EFFECT_KIND_110 = 110,
    KF_EFFECT_KIND_111 = 111,
    KF_EFFECT_KIND_112 = 112,
    KF_EFFECT_KIND_113 = 113,
    KF_EFFECT_KIND_114 = 114,
    KF_EFFECT_KIND_115 = 115,
    KF_EFFECT_KIND_116 = 116,
    KF_EFFECT_KIND_117 = 117,
    KF_EFFECT_KIND_118 = 118,
    KF_EFFECT_KIND_119 = 119,
    KF_EFFECT_KIND_120 = 120,
    KF_EFFECT_KIND_121 = 121,
    KF_EFFECT_KIND_122 = 122,
    KF_EFFECT_KIND_123 = 123,
    KF_MAGIC_NONE = 0xff
KF_ENUM_END(KfEffectKind)

#endif
