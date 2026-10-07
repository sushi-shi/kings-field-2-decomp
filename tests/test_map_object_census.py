"""Map-object census decoders on synthetic archives, chunks, TMDs and callbacks."""

import struct
import unittest

from scripts.kf.map_object_census import (
    CALLBACK_BASE,
    JR_RA,
    SECTOR,
    callback_operation_immediates,
    is_noop,
    read_archive,
    read_chunks,
    summarize_model,
)


def packet(mode: int, flag: int, body_words: int) -> bytes:
    body = bytearray(body_words * 4)
    if mode & 0x04:
        struct.pack_into("<H", body, 6, 0x1234)
    return bytes((0, body_words, flag, mode)) + bytes(body)


class MapObjectCensusTest(unittest.TestCase):
    def test_archive_entries_follow_sector_offsets(self):
        header = struct.pack("<H3H", 2, 1, 2, 2).ljust(SECTOR, b"\0")
        archive = header + b"A" * SECTOR
        self.assertEqual(read_archive(archive), [b"A" * SECTOR, b""])

    def test_chunks_stop_at_count_or_truncation(self):
        entry = struct.pack("<I", 2) + b"ab" + struct.pack("<I", 1) + b"c" + struct.pack("<I", 9)
        self.assertEqual(read_chunks(entry, 1), [b"ab"])
        self.assertEqual(read_chunks(entry, 5), [b"ab", b"c"])

    def test_model_counts_textured_semi_transparent_and_unlit_primitives(self):
        packets = packet(0x24, 0, 3) + packet(0x36, 1, 5) + packet(0x20, 0, 2)
        objects = struct.pack("<6Ii", 0, 7, 0, 0, 28, 3, 0)
        tmd = struct.pack("<3I", 0x41, 0, 1) + objects + packets
        asset = struct.pack("<3I", 12 + len(tmd), 0, 12) + tmd
        model = summarize_model(asset)
        self.assertTrue(model.present)
        self.assertEqual((model.objects, model.vertices), (1, 7))
        self.assertEqual(model.primitives, {"F3": 1, "FT3": 1, "GT3": 1})
        self.assertEqual((model.textured, model.untextured), (2, 1))
        self.assertEqual((model.semi_transparent, model.gouraud, model.unlit), (1, 1, 1))
        self.assertEqual(model.texture_pages, {0x1234: 2})
        self.assertFalse(summarize_model(struct.pack("<3I", 0, 0, 12)).present)

    def test_callback_scan_stops_at_first_return(self):
        ori_v0_a0 = 0x34020000 | 0xA0
        addiu_v1_a5 = 0x24030000 | 0xA5
        ori_v0_a1_after = 0x34020000 | 0xA1
        code = struct.pack("<6I", ori_v0_a0, addiu_v1_a5, JR_RA, 0, ori_v0_a1_after, 0)
        self.assertEqual(
            callback_operation_immediates(code, CALLBACK_BASE, {0xA0, 0xA1, 0xA5}), [0xA0, 0xA5])
        self.assertEqual(callback_operation_immediates(code, CALLBACK_BASE - 4, {0xA0}), [])
        self.assertTrue(is_noop(code, CALLBACK_BASE + 8))
        self.assertFalse(is_noop(code, CALLBACK_BASE))


if __name__ == "__main__":
    unittest.main()
