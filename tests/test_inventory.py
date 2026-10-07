from __future__ import annotations

import unittest
from dataclasses import replace
from unittest.mock import patch

from scripts.kf.inventory import (
    _data_access,
    _ghidra_type,
    _header_structure_layouts,
    _structural_data_matches_identity,
    _signature_hints,
    load_data_identities,
    validate,
)
from scripts.kf.paths import REPO, RETAIL_CONFIG
from scripts.kf.retail import parse_int, read_tsv


def _structure_field(structure: str, offset: int) -> tuple[str, str, int]:
    """(name, datatype, size) of one checked layout field from structure_fields.tsv."""
    _, rows = read_tsv(RETAIL_CONFIG / "structure_fields.tsv")
    for row in rows:
        if row["structure"] == structure and parse_int(row["offset"]) == offset:
            return row["name"], row["datatype"], parse_int(row["size"])
    raise AssertionError(f"{structure} has no field at {offset:#x}")


def words(*values: int) -> bytes:
    return b"".join(value.to_bytes(4, "little") for value in values)


class FakeImage:
    def __init__(self, word: int):
        self.word = word

    def u32(self, _va: int) -> int:
        return self.word


class FakeReference:
    def __init__(self, kind: str = "address", paired_site: int | None = 0x1004):
        self.kind = kind
        self.paired_site = paired_site


class InventoryTests(unittest.TestCase):
    def test_bss_identity_can_extend_beyond_final_load_fragment(self) -> None:
        row = load_data_identities(RETAIL_CONFIG)[("GAME.EXE", 0x8006DC00)]
        self.assertTrue(_structural_data_matches_identity(row, "bss", 0x400))
        self.assertFalse(_structural_data_matches_identity(row, "defined", 0x400))
        self.assertFalse(_structural_data_matches_identity(row, "bss", 0x200))
        self.assertFalse(_structural_data_matches_identity(row, "bss", 0x5000))
        self.assertFalse(_structural_data_matches_identity(replace(row, va=row.va - 4), "bss", 0x400))

    def test_enum_storage_typedef_preserves_abi_and_rejects_unknown_types(self) -> None:
        domain = "KF_ENUM_BEGIN(Mode, s16) MODE_FIRST = 1 KF_ENUM_END(Mode)"
        alias = "typedef KF_ENUM_STORAGE(Mode, u32) ModeWord;"
        carrier = "typedef struct Carrier { u8 prefix; ModeWord mode; u8 suffix; } Carrier;"
        source = domain + alias + carrier
        with patch("pathlib.Path.read_text", lambda path:
                   source if path.name == "types.h" else ""):
            layout = _header_structure_layouts()["Carrier"]
        self.assertEqual((layout.size, layout.alignment), (12, 4))
        self.assertEqual([(f.offset, f.size, f.datatype) for f in layout.fields],
                         [(0, 1, "u8"), (4, 4, "ModeWord"), (8, 1, "u8")])
        controls = (
            (source.replace("(Mode, u32)", "(Missing, u32)"), "undeclared enum domain"),
            (source.replace("(Mode, u32)", "(Mode, size_t)"), "unsupported enum storage"),
            (domain + alias + alias + carrier, "duplicate checked type"),
            (domain + "typedef u32 ModeWord;" + carrier, "unknown field type"),
        )
        for source, error in controls:
            with self.subTest(error=error), patch("pathlib.Path.read_text", lambda path:
                                                source if path.name == "types.h" else ""):
                with self.assertRaisesRegex(ValueError, error):
                    _header_structure_layouts()

    def test_storage_macro_requires_a_declared_enum_domain(self) -> None:
        declaration = """
            KF_ENUM_BEGIN(Floor, s32)
                FLOOR_FIRST = 1
            KF_ENUM_END(Floor)
        """
        carrier = "typedef struct Carrier { KF_ENUM_STORAGE(Floor, u8) floor; } Carrier;"
        with patch("pathlib.Path.read_text", lambda path:
                   declaration + carrier if path.name == "types.h" else ""):
            field = _header_structure_layouts()["Carrier"].fields[0]
            self.assertEqual(field.datatype, "KF_ENUM_STORAGE(Floor, u8)")
        with patch("pathlib.Path.read_text", lambda path:
                   carrier if path.name == "types.h" else ""):
            with self.assertRaisesRegex(ValueError, "undeclared enum domain 'Floor'"):
                _header_structure_layouts()

    def test_stored_enum_fields_keep_domain_names_widths_and_alignment(self) -> None:
        source = """
            KF_ENUM_BEGIN(ByteState, u8)
                BYTE_IDLE = 0, BYTE_COUNT = 3
            KF_ENUM_END(ByteState)
            KF_ENUM_BEGIN(HalfState, s16)
                HALF_IDLE = 0
            KF_ENUM_END(HalfState)
            typedef struct EnumCarrier {
                ByteState states[BYTE_COUNT];
                HalfState state;
                u8 tail;
            } EnumCarrier;
        """
        with patch("pathlib.Path.read_text",
                   lambda path: source if path.name == "types.h" else ""):
            carrier = _header_structure_layouts()["EnumCarrier"]
        self.assertEqual((carrier.size, carrier.alignment), (8, 2))
        self.assertEqual([(f.name, f.datatype, f.offset, f.size) for f in carrier.fields], [
            ("states", "ByteState[3]", 0, 3), ("state", "HalfState", 4, 2),
            ("tail", "u8", 6, 1),
        ])

    def test_unknown_enum_storage_is_rejected_before_layout_can_be_trusted(self) -> None:
        source = """
            KF_ENUM_BEGIN(BadState, size_t)
                BAD_IDLE = 0
            KF_ENUM_END(BadState)
            typedef struct BadCarrier { BadState state; } BadCarrier;
        """
        with patch("pathlib.Path.read_text",
                   lambda path: source if path.name == "types.h" else ""):
            with self.assertRaisesRegex(ValueError, "unsupported enum storage"):
                _header_structure_layouts()

    def test_named_union_uses_maximum_extent_and_alignment(self) -> None:
        source = """
            typedef union LayoutUnion { u8 bytes[5]; u32 word; } LayoutUnion;
            typedef struct LayoutCarrier {
                u8 prefix;
                union LayoutUnion payload;
                u8 suffix;
            } LayoutCarrier;
        """
        def header(path):
            return source if path.name == "types.h" else ""

        with patch("pathlib.Path.read_text", header):
            layouts = _header_structure_layouts()
        union = layouts["LayoutUnion"]
        self.assertEqual((union.size, union.alignment), (8, 4))
        self.assertEqual([(f.offset, f.size) for f in union.fields], [(0, 5), (0, 4)])
        carrier = layouts["LayoutCarrier"]
        self.assertEqual((carrier.size, carrier.alignment), (16, 4))
        self.assertEqual([(f.offset, f.size) for f in carrier.fields],
                         [(0, 1), (4, 8), (12, 1)])

    def test_named_union_rejects_unknown_member_type(self) -> None:
        def header(path):
            return ("typedef union BadLayout { Missing value; } BadLayout;"
                    if path.name == "types.h" else "")

        with patch("pathlib.Path.read_text", header):
            with self.assertRaisesRegex(ValueError, "unknown field type 'Missing'"):
                _header_structure_layouts()

    def test_named_array_extents_preserve_physical_layout_and_reject_unknowns(self) -> None:
        declarations = """
            enum { ROWS = 3, COLUMNS = 0x5, EMPTY = 0, EXPRESSION = ROWS + 1 };
            typedef struct ExampleGrid {
                u8 prefix;
                u16 cells[ROWS][COLUMNS];
                u32 suffix;
            } ExampleGrid;
        """

        def read_header(path):
            return declarations if path.name == "types.h" else ""

        with patch("scripts.kf.inventory.Path.read_text", read_header):
            layout = _header_structure_layouts()["ExampleGrid"]
        self.assertEqual((layout.size, layout.alignment), (36, 4))
        self.assertEqual([(f.offset, f.size, f.datatype) for f in layout.fields],
                         [(0, 1, "u8"), (2, 30, "u16[3][5]"), (32, 4, "u32")])
        for bound, error in (("MISSING", "unresolved"), ("EXPRESSION", "unresolved"),
                             ("EMPTY", "nonpositive")):
            with self.subTest(bound=bound):
                broken = declarations.replace("cells[ROWS]", f"cells[{bound}]")
                with patch("scripts.kf.inventory.Path.read_text",
                           lambda path: broken if path.name == "types.h" else ""):
                    with self.assertRaisesRegex(ValueError, error + " array bound"):
                        _header_structure_layouts()

    def test_implicit_enum_array_count_starts_at_zero_and_follows_known_resets(self) -> None:
        declarations = """
            typedef enum TileIndex { FIRST, SECOND, THIRD, FOURTH, TILE_COUNT } TileIndex;
            enum { RESET = 0x7, NEXT, ROW_COUNT };
            typedef struct TileGrid { u16 tiles[ROW_COUNT][TILE_COUNT]; } TileGrid;
        """
        with patch("scripts.kf.inventory.Path.read_text",
                   lambda path: declarations if path.name == "types.h" else ""):
            layout = _header_structure_layouts()["TileGrid"]
        self.assertEqual((layout.size, layout.alignment), (72, 2))
        self.assertEqual(layout.fields[0].datatype, "u16[9][4]")

    def test_unknown_enum_alias_breaks_implicit_counts_without_guessing(self) -> None:
        for initializer in ("MISSING", "ALIAS", "KNOWN + 1"):
            for bound in ("ALIAS", "NEXT"):
                with self.subTest(initializer=initializer, bound=bound):
                    declarations = f"""
                        enum {{ KNOWN = 3, ALIAS = {initializer}, NEXT }};
                        typedef struct UnknownAlias {{ u8 cells[{bound}]; }} UnknownAlias;
                    """
                    with patch("scripts.kf.inventory.Path.read_text",
                               lambda path: declarations if path.name == "types.h" else ""):
                        with self.assertRaisesRegex(ValueError, "unresolved array bound"):
                            _header_structure_layouts()

    def test_implicit_enum_bound_does_not_guess_after_unknown_expression_or_overflow(self) -> None:
        for value in ("EXTERNAL + 1", "0x7fffffff"):
            with self.subTest(value=value):
                declarations = f"""
                    enum {{ FIRST = {value}, UNKNOWN_COUNT, RESET = 2, KNOWN_COUNT }};
                    typedef struct UnknownGrid {{ u8 cells[UNKNOWN_COUNT]; }} UnknownGrid;
                """
                with patch("scripts.kf.inventory.Path.read_text",
                           lambda path: declarations if path.name == "types.h" else ""):
                    with self.assertRaisesRegex(ValueError, "unresolved array bound 'UNKNOWN_COUNT'"):
                        _header_structure_layouts()
                declarations = declarations.replace("cells[UNKNOWN_COUNT]", "cells[KNOWN_COUNT]")
                with patch("scripts.kf.inventory.Path.read_text",
                           lambda path: declarations if path.name == "types.h" else ""):
                    self.assertEqual(_header_structure_layouts()["UnknownGrid"].size, 3)

    def test_union_storage_overlaps_and_rounds_up_for_enclosing_struct(self) -> None:
        declarations = """
            typedef union ExamplePayload {
                u8 bytes[5];
                u32 word;
                u16 pair[2];
            } ExamplePayload;
            typedef struct ExampleEnvelope {
                u8 prefix;
                union ExamplePayload payload;
                u8 suffix;
            } ExampleEnvelope;
            typedef union UninventoriedView {
                u8 bytes[5];
                struct { u16 first; u16 second; } nested;
            } UninventoriedView;
        """

        def read_header(path):
            return declarations if path.name == "types.h" else ""

        with patch("scripts.kf.inventory.Path.read_text", read_header):
            layouts = _header_structure_layouts()
        self.assertEqual(set(layouts), {"ExamplePayload", "ExampleEnvelope"})
        payload = layouts["ExamplePayload"]
        self.assertEqual((payload.size, payload.alignment), (8, 4))
        self.assertEqual([(f.offset, f.size) for f in payload.fields],
                         [(0, 5), (0, 4), (0, 4)])
        envelope = layouts["ExampleEnvelope"]
        self.assertEqual((envelope.size, envelope.alignment), (16, 4))
        self.assertEqual([(f.offset, f.size) for f in envelope.fields],
                         [(0, 1), (4, 8), (12, 1)])

    def test_curated_inventories_cover_the_wip_universe(self) -> None:
        # Every carveable non-vendored function in the four images has a
        # (candidate) identity row; update these counts with each admission.
        counts = validate(RETAIL_CONFIG)
        self.assertEqual(counts["functions"], 564)
        self.assertEqual(counts["signatures_started"], 564)
        self.assertEqual(counts["data"], 770)
        self.assertGreaterEqual(counts["functions_named"], 562)
        self.assertEqual(counts["structures"], 247)

    def test_static_signature_hint_tracks_live_arguments_and_result(self) -> None:
        parameters, result, shape = _signature_hints(words(
            0x8C820000,  # lw v0,0(a0)
            0x00451021,  # addu v0,v0,a1
            0x03E00008,  # jr ra
            0x00000000,
        ))
        self.assertEqual(parameters, "unknown *object;unknown arg1")
        self.assertEqual(result, "unknown")
        self.assertIn("loads=1", shape)

    def test_data_access_uses_low_instruction_opcode(self) -> None:
        reference = FakeReference()
        self.assertEqual(_data_access(FakeImage(0x8C820000), reference), "read")
        self.assertEqual(_data_access(FakeImage(0xAC820000), reference), "write")
        self.assertEqual(
            _data_access(FakeImage(0), FakeReference(kind="pointer")),
            "initializer",
        )

    def test_ghidra_types_are_explicitly_candidate_project_widths(self) -> None:
        self.assertEqual(_ghidra_type("undefined4"), "u32")
        self.assertEqual(_ghidra_type("undefined2 *"), "u16 *")
        self.assertEqual(_ghidra_type("short *"), "s16 *")

    def test_sources_include_semantic_owner_headers_directly(self) -> None:
        self.assertFalse((REPO / "include/kf/semantic_types.h").exists())
        for directory, pattern in (("src", "*.c"), ("include", "*.h")):
            for source in (REPO / directory).rglob(pattern):
                self.assertNotIn("kf/semantic_types.h", source.read_text())

if __name__ == "__main__":
    unittest.main()
