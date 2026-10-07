import tempfile
import unittest
from pathlib import Path

from scripts.kf.common_order import Common, bucket, exported_bss, placements

HEADER = "image\tva\tsize\tname\tscope\tstorage\tdatatype\towner\tconfidence\tevidence\tnote\n"


class CommonOrderTests(unittest.TestCase):
    def test_bucket_counts_the_length_byte(self) -> None:
        # The KF1 PSYLINK decode: PadIdentifier precedes abc only when the
        # length joins the byte sum.
        self.assertEqual(bucket("abc"), (3 + 97 + 98 + 99) & 511)
        self.assertLess(bucket("PadIdentifier"), bucket("abc"))

    def test_retail_sdk_anchors_bound_each_game_request(self) -> None:
        anchors = [Common(0x100, 8, "_svm_orev2", "LIBSND VMANAGER.OBJ"),
                   Common(0x200, 8, "_spu_keyoff_bit", "LIBSPU S_INI.OBJ")]
        game = [Common(0x108, 4, "ab", "game"), Common(0x110, 4, "player_state", "game")]
        rows, disordered = placements(sorted(anchors + game, key=lambda c: c.va))
        self.assertEqual(disordered, [])
        self.assertEqual([(row.lower, row.upper) for row in rows],
                         [(bucket("_svm_orev2"), bucket("_spu_keyoff_bit"))] * 2)
        self.assertEqual([row.consistent for row in rows],
                         [bucket("_svm_orev2") <= bucket("ab") <= bucket("_spu_keyoff_bit"),
                          bucket("_svm_orev2") <= bucket("player_state") <= bucket("_spu_keyoff_bit")])

    def test_sdk_order_break_and_fixed_statics_are_reported_or_skipped(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            config = Path(directory)
            rows = [
                ("GAME.EXE", "0x10", "0x8", "stack", "static", "bss", "u8[8]", "LIBETC INTR.OBJ"),
                ("GAME.EXE", "0x20", "0x8", "_svm_okof2", "global", "bss", "u8[8]", "LIBSND VMANAGER.OBJ"),
                ("GAME.EXE", "0x28", "0x4", "early", "global", "bss", "s32", "main"),
                ("GAME.EXE", "0x30", "0x8", "_svm_okon1", "global", "bss", "u8[8]", "LIBSND VMANAGER.OBJ"),
                ("GAME.EXE", "0x08", "0x4", "before_run", "global", "bss", "s32", "main"),
            ]
            (config / "data_identities.tsv").write_text(
                HEADER + "".join("\t".join(row + ("supported", "e", "n")) + "\n" for row in rows))
            commons = exported_bss("GAME.EXE", config)
            self.assertEqual([c.name for c in commons], ["_svm_okof2", "early", "_svm_okon1"])
            rows_out, disordered = placements(commons)
            self.assertEqual([c.name for c in disordered], ["_svm_okon1"])
            self.assertEqual(rows_out[0].common.name, "early")


if __name__ == "__main__":
    unittest.main()
