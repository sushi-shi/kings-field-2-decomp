"""SDATA() small-data literal claims: validation, packing and referents.

Under cc1 -G8, GCC 2.5.7 sends every constant of at most eight bytes to
.sdata (config/mips/mips.h SELECT_SECTION), so a short string literal is an
anonymous .sdata member that ASPSX addresses through the section symbol. A
unit claims that run with SDATA(va, size); the delinker packs it among the
unit's named .sdata claims in address order and never exports a symbol for it.
"""

from __future__ import annotations

import struct
import unittest
from pathlib import Path

from scripts.kf.delink import (
    SDATA_LITERAL_SYMBOL,
    Catalog,
    Datum,
    Function,
    Module,
    _apply_relocation,
    _module_object,
    decode_hi_lo_target,
)
from scripts.kf.manifest import Profile, _sdata_range
from scripts.kf.model import DataClaim, RodataClaim
from scripts.kf.mips_elf import MipsRelocation
from tests.test_delink import elf_sections, elf_symbols


def profile(small_data: int) -> Profile:
    return Profile('probe', 'c', 'gcc257-native', 'O2', small_data, '1.07', ())


class SdataRangeValidationTests(unittest.TestCase):
    source = Path('src/game/unit.c')

    def validate(self, claims, small_data=8, data=(), rodata=None):
        return _sdata_range(self.source, 'GAME.EXE', profile(small_data), claims,
                            data, [], rodata)

    def test_one_word_aligned_range_under_a_small_data_profile(self):
        claim = RodataClaim(0x8006D6A4, 0xB, 3)
        self.assertEqual(self.validate((claim,)), (0x8006D6A4, 0xB))
        self.assertIsNone(self.validate(()))

    def test_rejects_g0_misalignment_overlap_and_duplicates(self):
        claim = RodataClaim(0x8006D6A4, 0xB, 3)
        cases = {
            'g0': dict(claims=(claim,), small_data=0),
            'unaligned': dict(claims=(RodataClaim(0x8006D6A6, 2, 3),)),
            'two': dict(claims=(claim, RodataClaim(0x8006D6B0, 4, 4))),
            'named': dict(claims=(claim,),
                          data=(DataClaim(0x8006D6A8, 4, 'named', 5, '.sdata'),)),
            'rodata': dict(claims=(claim,), rodata=(0x8006D6A0, 8)),
        }
        for name, case in cases.items():
            with self.subTest(name), self.assertRaises(ValueError):
                self.validate(**case)


class SdataLiteralModuleTests(unittest.TestCase):
    def test_code_reference_resolves_to_the_literal_working_symbol(self):
        function = Function('GAME.EXE', 0x80022700, 8, 8, 1, 'scan', 'test', 'test')
        catalog = Catalog(functions={'GAME.EXE': (function,)},
                          function_starts={'GAME.EXE': {function.va: function}},
                          data={'GAME.EXE': ()})
        blob = bytearray(struct.pack('<2I', 0x3C028007, 0x8042D6A5))  # lui; lb +1
        row = {
            'image': 'GAME.EXE', 'site_va': '0x80022700', 'paired_site_va': '0x80022704',
            'kind': 'mips_hi16_lo16', 'channel': 'reachable-code', 'target_va': '0x8006d6a5',
            'target_region': 'load', 'target_name': '', 'opcode': 'lui+lb',
            'confidence': 'paired-reviewed', 'status': 'reviewed',
        }
        relocations, _used = _apply_relocation(blob, function, row, catalog, 'safe',
                                               None, (0x8006D6A4, 0xB))
        self.assertEqual({item.symbol for item in relocations}, {SDATA_LITERAL_SYMBOL})
        self.assertEqual(decode_hi_lo_target(*struct.unpack('<2I', blob)), 1)

    def test_literals_pack_after_named_sdata_and_statics_use_sbss_offsets(self):
        function = Function('GAME.EXE', 0x80010000, 24, 24, 1, 'only', 'test', 'test')
        words = (0x3C020000, 0x80420001, 0x3C030000, 0xA0630000, 0x3C040000, 0x8C840000)
        carved = {function.va: (struct.pack('<6I', *words), [
            MipsRelocation(0, 'R_MIPS_HI16', SDATA_LITERAL_SYMBOL),
            MipsRelocation(4, 'R_MIPS_LO16', SDATA_LITERAL_SYMBOL),
            MipsRelocation(8, 'R_MIPS_HI16', 'loaded_slot'),
            MipsRelocation(12, 'R_MIPS_LO16', 'loaded_slot'),
            MipsRelocation(16, 'R_MIPS_HI16', 'second_event'),
            MipsRelocation(20, 'R_MIPS_LO16', 'second_event'),
        ])}
        module = Module('GAME.EXE', 'game.card', 'card', (function.va,), (
            Datum(0x8006D6A0, 1, 'loaded_slot', 'load', 'global', '.sdata'),
            Datum(0x8006DA18, 4, 'first_event', 'bss', 'static', '.sbss', 8),
            Datum(0x8006DA20, 4, 'second_event', 'bss', 'static', '.sbss', 8),
        ), sdata=(0x8006D6A4, 0xB))
        blobs = {0x8006D6A0: (b'\0', []), 0x8006D6A4: (b' \0\0\0bu00:*\0', [])}
        retail = {0x8006D6A1: b'\0\0\0'}
        built = _module_object(module, {function.va: function}, carved, blobs,
                               load_padding=lambda va, size: retail[va][:size])
        self.assertEqual([item.symbol for item in built.relocations],
                         ['.sdata', '.sdata', '.sdata', '.sdata', '.sbss', '.sbss'])
        sections = elf_sections(built.data)
        text = built.data[sections['.text'][4]:sections['.text'][4] + sections['.text'][5]]
        self.assertEqual(decode_hi_lo_target(*struct.unpack_from('<2I', text)), 5)
        self.assertEqual(decode_hi_lo_target(*struct.unpack_from('<2I', text, 8)), 0)
        self.assertEqual(decode_hi_lo_target(*struct.unpack_from('<2I', text, 16)), 8)
        sdata = built.data[sections['.sdata'][4]:sections['.sdata'][4] + sections['.sdata'][5]]
        self.assertEqual(sdata, b'\0\0\0\0 \0\0\0bu00:*\0')
        self.assertEqual(sections['.sbss'][5], 16)
        names = {name for name, _symbol in elf_symbols(built.data)}
        self.assertNotIn(SDATA_LITERAL_SYMBOL, names)
        self.assertIn('loaded_slot', names)


if __name__ == '__main__':
    unittest.main()
