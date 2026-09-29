from __future__ import annotations

import hashlib
import importlib.util
import sys
import tempfile
import unittest
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "scripts" / "create-toolchain.py"
SPEC = importlib.util.spec_from_file_location("create_toolchain", SCRIPT)
assert SPEC is not None and SPEC.loader is not None
module = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = module
SPEC.loader.exec_module(module)


class ToolchainTests(unittest.TestCase):
    def test_one_source_medium_is_active(self) -> None:
        self.assertEqual(set(module.MEDIA), {"psyq-3.0"})

    def test_sha256_file(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "sample.bin"
            path.write_bytes(b"King's Field\0")
            self.assertEqual(module.sha256_file(path), hashlib.sha256(path.read_bytes()).hexdigest())

    def test_hash_mismatch_is_fatal(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "medium.img"
            path.write_bytes(b"wrong")
            with self.assertRaisesRegex(RuntimeError, "hash mismatch"):
                module.verify_hash(path, "0" * 64, "test medium")

    def test_casefold_resolution(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            target = root / "ISA BOARD" / "PsxLib" / "LIB"
            target.mkdir(parents=True)
            self.assertEqual(module.resolve_casefold(root, "isa board/PSXLIB/lib"), target)

    def test_manifest_is_sorted_and_reproducible(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            stage = root / "stage"
            (stage / "psyq" / "bin").mkdir(parents=True)
            (stage / "psyq" / "bin" / "PSYLINK.EXE").write_bytes(b"link")
            (stage / "psyq" / "bin" / "CCPSX.EXE").write_bytes(b"compile")
            module.write_manifest(stage)
            first = (stage / "MANIFEST.tsv").read_text()
            module.write_manifest(stage)
            second = (stage / "MANIFEST.tsv").read_text()
            self.assertEqual(first, second)
            self.assertLess(first.index("CCPSX.EXE"), first.index("PSYLINK.EXE"))

    def test_stage_kit_preserves_the_complete_tree(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / "source"
            stage = Path(directory) / "stage"
            for relative in ("BIN", "COMPILER", "INCLUDE/SYS", "LIB", "SAMPLE/ETC"):
                (root / relative).mkdir(parents=True)
            for name in ("2MBYTE.OBJ", "8MBYTE.OBJ", "LIBAPI.LIB", "NONE2.OBJ"):
                (root / "LIB" / name).write_bytes(b"LNK\x02" + name.encode())
            (root / "BIN" / "LIBSN.LIB").write_bytes(b"fileserver libsn")
            (root / "SAMPLE/ETC/marker.txt").write_text("complete media tree")
            for name in ("CC1PSX", "CPPPSX", "CC1PSX.EXE", "CPPPSX.EXE", "README.GNU"):
                (root / "COMPILER" / name).write_bytes(name.encode())

            self.assertEqual(module.find_kit_root(Path(directory)), root)
            module.stage_kit(root, stage)

            for name in ("2MBYTE.OBJ", "8MBYTE.OBJ", "LIBAPI.LIB", "NONE2.OBJ"):
                self.assertEqual(
                    (stage / "psyq-3.0/LIB" / name).read_bytes(),
                    (root / "LIB" / name).read_bytes(),
                )
            self.assertEqual(
                (stage / "psyq-3.0/BIN/LIBSN.LIB").read_bytes(), b"fileserver libsn"
            )
            self.assertEqual(
                (stage / "psyq-3.0/SAMPLE/ETC/marker.txt").read_text(),
                "complete media tree",
            )
            self.assertFalse((stage / "compilers").exists())

    def test_kit_gates_cover_every_linker_input(self) -> None:
        for library in ("LIBSN", "LIBCD", "LIBSND", "LIBSPU", "LIBGTE", "LIBGPU",
                        "LIBETC", "LIBAPI", "LIBPRESS"):
            self.assertIn(f"LIB/{library}.LIB", module.EXPECTED_KIT_FILES)
        self.assertIn("LIB/NONE2.OBJ", module.EXPECTED_KIT_FILES)


if __name__ == "__main__":
    unittest.main()
