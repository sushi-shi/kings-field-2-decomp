"""Independent config-data import, complete-object comparison and closure gates."""

import unittest


from scripts.kf import config_data


SDK_HEADER = ("Header : LNK version 2\n"
              "16 : Section symbol number 3 '.data' in group 0 alignment 8\n6 : Switch to section 3\n")
SDK_EXPORT = "12 : XDEF symbol number 4 'values' at offset 0 in section 3\n"


class ConfigDataTests(unittest.TestCase):
    def test_sdk_parser_accumulates_whole_section_and_preserves_export(self):
        listing = SDK_HEADER + "2 : Code 2 bytes\n\n0000: 01 02\n2 : Code 2 bytes\n0000: 03 04\n" + SDK_EXPORT
        self.assertEqual(config_data.parse_sdk_section(listing, '.data'),
                         config_data.SdkSection(bytes((1, 2, 3, 4)), 4, (("values", 0),), 8))

    def test_sdk_alignment_tags_are_not_byte_counts_or_arbitrary_halves(self):
        listing = SDK_HEADER + "2 : Code 3 bytes\n0000: 01 02 03\n" + SDK_EXPORT
        for tag, alignment in ((2, 1), (4, 2), (8, 4), (16, 16)):
            with self.subTest(tag=tag):
                section = config_data.parse_sdk_section(listing.replace('alignment 8', f'alignment {tag}'), '.data')
                self.assertEqual((section.alignment, section.lnk_alignment), (alignment, tag))
                self.assertEqual(section.data, b'\x01\x02\x03')
        for tag in (0, 1, 3, 6, 12, 24, 32, 64, 128, 256):
            with self.subTest(tag=tag), self.assertRaisesRegex(ValueError, 'alignment tag'):
                config_data.parse_sdk_section(listing.replace('alignment 8', f'alignment {tag}'), '.data')

    def test_sdk_alignment_interpretation_requires_one_known_lnk_version(self):
        listing = SDK_HEADER + "2 : Code 3 bytes\n0000: 01 02 03\n" + SDK_EXPORT
        for header in ('', 'Header : LNK version 1\n', 'Header : LNK version 3\n',
                       'Header : LNK version 2\nHeader : LNK version 2\n'):
            with self.subTest(header=header), self.assertRaisesRegex(ValueError, 'single LNK v2'):
                config_data.parse_sdk_section(listing.replace('Header : LNK version 2\n', header), '.data')

    def test_sdk_parser_never_discards_patches_reservations_or_unknown_records(self):
        for record in ("10 : Patch type 2 at offset 0 with $0", "8 : Uninitialized 4 bytes",
                       "62 : Unknown allocation operation", "18 : Local symbol 'private' in section 3 at offset 0"):
            listing = SDK_HEADER + "2 : Code 4 bytes\n0000: 00 00 00 00\n" + record + "\n" + SDK_EXPORT
            with self.subTest(record=record), self.assertRaises(ValueError):
                config_data.parse_sdk_section(listing, '.data')

    def test_sdk_parser_rejects_missing_disordered_truncated_and_extra_bytes(self):
        for body in ("", "2 : Code 4 bytes\n0000: 00 00\n", "2 : Code 4 bytes\n0002: 00 00 00 00\n",
                     "2 : Code 4 bytes\n0000: 00 00 00 00 00\n",
                     "2 : Code 4 bytes\n0000: 00 00 00 00\n0004: 00 00 00 00\n"):
            with self.subTest(body=body), self.assertRaises(ValueError):
                config_data.parse_sdk_section(SDK_HEADER + body + SDK_EXPORT, '.data')

    def test_sdk_text_patches_referencing_data_do_not_become_data_patches(self):
        listing = (SDK_HEADER + "2 : Code 4 bytes\n0000: 00 00 00 00\n6 : Switch to section 2\n"
                   + "10 : Patch type 82 at offset 0 with (sectbase(3)+$0)\n" + SDK_EXPORT)
        self.assertEqual(len(config_data.parse_sdk_section(listing, '.data').data), 4)


if __name__ == '__main__':
    unittest.main()
