"""Relocation-aware matching of native Psy-Q objects against PS-X EXEs.

This is deliberately separate from :mod:`scripts.kf.census`: a PS-X EXE has
discarded its linker relocations, whereas an SDK ``.OBJ`` still records the
exact bytes that PSYLINK was allowed to change.  The script asks the audited
``psy-k`` utility to extract and describe Psy-Q libraries, masks those object
relocation fields, and searches a retail executable for the remaining exact
bytes.

The proprietary SDK inputs and the third-party parser are external witnesses;
neither is vendored here.  A typical invocation is::

    python3 -m scripts.kf.toolchain_attribution \
      --psyk /path/to/psy-k-v0.4.0/target/release/psyk \
      --sdk-lib-dir /path/to/PSXLIB/LIB \
      --exe GAME.EXE --exe OPEN.EXE --output matches.tsv

Only relocation-masked exact matches are emitted.  They establish the exact
library member/snapshot represented by the input object, but do not by
themselves prove that game-owned translation units used the same compiler.
"""

from __future__ import annotations

import argparse
import csv
import re
import subprocess
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Iterable, Sequence


SECTION_RE = re.compile(r"Section symbol number ([0-9a-f]+) '([^']+)'")
SWITCH_RE = re.compile(r"Switch to section ([0-9a-f]+)")
CODE_RE = re.compile(r"Code ([0-9]+) bytes")
HEX_RE = re.compile(r"^([0-9a-f]{4,}): ((?:[0-9a-f]{2}(?: |$))+)$")
PATCH_RE = re.compile(r"Patch type ([0-9]+) at offset ([0-9a-f]+)")
RCS_RE = re.compile(rb"\$Id: [^\x00\r\n]+")


@dataclass
class ObjectBlock:
    """One contiguous code/data record from a Psy-Q object."""

    library: str
    module: str
    section_id: int
    section_name: str
    index: int
    data: bytes
    compare_mask: bytearray = field(init=False)
    relocation_count: int = 0
    records: int = 1

    def __post_init__(self) -> None:
        self.compare_mask = bytearray(b"\xff" * len(self.data))

    def mask_patch(self, kind: int, offset: int) -> None:
        """Mask bits that a PSYLINK relocation is permitted to rewrite."""
        if not 0 <= offset <= len(self.data) - 4:
            raise ValueError(
                f"{self.module} block {self.index}: relocation {kind} at "
                f"0x{offset:x} exceeds {len(self.data)}-byte record"
            )
        # PSY-Q/MIPS relocation tags observed in retail SDK objects:
        # 0x4a/74 is R_MIPS_26; 0x52/82 and 0x54/84 are HI16 and LO16.
        # 8/16 are full-word writes.  Conservatively mask a full word for an
        # unknown tag rather than manufacture a false exact match.
        masks = {
            74: (0x00, 0x00, 0x00, 0xFC),
            82: (0x00, 0x00, 0xFF, 0xFF),
            84: (0x00, 0x00, 0xFF, 0xFF),
        }.get(kind, (0x00, 0x00, 0x00, 0x00))
        for index, mask in enumerate(masks, offset):
            self.compare_mask[index] &= mask
        self.relocation_count += 1

    @property
    def compared_bits(self) -> int:
        return sum(value.bit_count() for value in self.compare_mask)

    @property
    def rcs_id(self) -> str:
        match = RCS_RE.search(self.data)
        return "" if match is None else match.group().decode("ascii", "replace")

    def anchor(self) -> tuple[int, bytes] | None:
        """Return the longest fully compared run used to seed exact search."""
        best_start = best_end = start = 0
        for position, mask in enumerate(self.compare_mask):
            if mask != 0xFF:
                if position - start > best_end - best_start:
                    best_start, best_end = start, position
                start = position + 1
        if len(self.compare_mask) - start > best_end - best_start:
            best_start, best_end = start, len(self.compare_mask)
        if best_end - best_start < 8:
            return None
        return best_start, self.data[best_start:best_end]


@dataclass(frozen=True)
class PsxImage:
    path: Path
    load_address: int
    payload: bytes

    @classmethod
    def read(cls, path: Path) -> "PsxImage":
        data = path.read_bytes()
        if len(data) < 0x800 or data[:8] != b"PS-X EXE":
            raise ValueError(f"{path}: not a PS-X EXE")
        load_address = int.from_bytes(data[0x18:0x1C], "little")
        size = int.from_bytes(data[0x1C:0x20], "little")
        if 0x800 + size > len(data):
            raise ValueError(f"{path}: declared payload exceeds file")
        return cls(path, load_address, data[0x800:0x800 + size])


def run_psyk(psyk: Path, *arguments: str, cwd: Path | None = None) -> str:
    result = subprocess.run(
        (str(psyk), *arguments), cwd=cwd, check=True,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
    )
    return result.stdout


def parse_object_listing(library: str, module: str, listing: str) -> list[ObjectBlock]:
    """Parse the stable human listing produced by psy-k 0.4.0."""
    section_names: dict[int, str] = {}
    section_id = -1
    blocks: list[ObjectBlock] = []
    pending_size: int | None = None
    pending_hex = bytearray()

    def finish_code() -> None:
        nonlocal pending_size, pending_hex
        if pending_size is None:
            return
        if len(pending_hex) != pending_size:
            raise ValueError(
                f"{module}: psy-k listed {pending_size} bytes but emitted "
                f"{len(pending_hex)} hexdump bytes"
            )
        blocks.append(ObjectBlock(
            library, module, section_id,
            section_names.get(section_id, f"section_{section_id:x}"),
            len(blocks), bytes(pending_hex),
        ))
        pending_size = None
        pending_hex = bytearray()

    for raw_line in listing.splitlines():
        line = raw_line.strip()
        if match := SECTION_RE.search(line):
            section_names[int(match.group(1), 16)] = match.group(2)
            continue
        if match := SWITCH_RE.search(line):
            finish_code()
            section_id = int(match.group(1), 16)
            continue
        if match := CODE_RE.search(line):
            finish_code()
            pending_size = int(match.group(1))
            continue
        if pending_size is not None and (match := HEX_RE.fullmatch(line)):
            offset = int(match.group(1), 16)
            if offset != len(pending_hex):
                raise ValueError(
                    f"{module}: non-contiguous hexdump at 0x{offset:x}"
                )
            pending_hex.extend(bytes.fromhex(match.group(2)))
            continue
        if match := PATCH_RE.search(line):
            finish_code()
            if not blocks:
                raise ValueError(f"{module}: relocation precedes first code record")
            blocks[-1].mask_patch(int(match.group(1)), int(match.group(2), 16))
    finish_code()
    return blocks


def library_blocks(psyk: Path, library_path: Path) -> list[ObjectBlock]:
    blocks: list[ObjectBlock] = []
    with tempfile.TemporaryDirectory(prefix="kf-psyq-") as directory:
        root = Path(directory)
        run_psyk(psyk, "extract", str(library_path.resolve()), cwd=root)
        for obj in sorted(root.glob("*.OBJ")):
            listing = run_psyk(psyk, "list", "--code", str(obj))
            blocks.extend(parse_object_listing(
                library_path.name, obj.stem, listing,
            ))
    return coalesce_sections(blocks)


def coalesce_sections(blocks: Iterable[ObjectBlock]) -> list[ObjectBlock]:
    """Join one object's interleaved records into linker-contiguous sections.

    Psy-Q objects can switch from ``.text`` to ``.rdata`` and back many times.
    PSYLINK appends the records for each object section in record order, so the
    joined form is both a stronger fingerprint and a useful assertion about
    linker behavior.
    """
    grouped: dict[tuple[str, str, int, str], list[ObjectBlock]] = {}
    for block in blocks:
        key = (block.library, block.module, block.section_id, block.section_name)
        grouped.setdefault(key, []).append(block)
    result: list[ObjectBlock] = []
    for (library, module, section_id, section_name), members in grouped.items():
        joined = ObjectBlock(
            library, module, section_id, section_name, 0,
            b"".join(member.data for member in members),
            records=len(members),
        )
        joined.compare_mask = bytearray().join(
            member.compare_mask for member in members
        )
        joined.relocation_count = sum(
            member.relocation_count for member in members
        )
        result.append(joined)
    return result


def find_block(block: ObjectBlock, image: PsxImage, minimum_bytes: int = 32) -> list[int]:
    if len(block.data) < minimum_bytes or block.compared_bits < minimum_bytes * 4:
        return []
    anchor = block.anchor()
    if anchor is None:
        return []
    anchor_offset, needle = anchor
    matches: list[int] = []
    cursor = 0
    while True:
        found = image.payload.find(needle, cursor)
        if found < 0:
            break
        start = found - anchor_offset
        if 0 <= start <= len(image.payload) - len(block.data):
            candidate = image.payload[start:start + len(block.data)]
            if all(
                (actual & mask) == (expected & mask)
                for actual, expected, mask in zip(
                    candidate, block.data, block.compare_mask, strict=True,
                )
            ):
                matches.append(start)
        cursor = found + 1
    return matches


def match_rows(blocks: Iterable[ObjectBlock], images: Sequence[PsxImage],
               minimum_bytes: int = 32) \
        -> list[dict[str, object]]:
    blocks = list(blocks)
    signatures: dict[tuple[str, bytes, bytes], list[str]] = {}
    for block in blocks:
        normalized = bytes(
            value & mask
            for value, mask in zip(block.data, block.compare_mask, strict=True)
        )
        signature = (block.section_name, normalized, bytes(block.compare_mask))
        signatures.setdefault(signature, []).append(
            f"{block.library}/{block.module}/{block.section_name}"
        )
    rows: list[dict[str, object]] = []
    for block in blocks:
        normalized = bytes(
            value & mask
            for value, mask in zip(block.data, block.compare_mask, strict=True)
        )
        candidates = sorted(signatures[
            (block.section_name, normalized, bytes(block.compare_mask))
        ])
        for image in images:
            matches = find_block(block, image, minimum_bytes)
            for payload_offset in matches:
                if len(candidates) > 1:
                    confidence = "archive_ambiguous"
                elif block.compared_bits >= 1024:
                    confidence = "strong_unique"
                else:
                    confidence = "short_unique"
                rows.append({
                    "library": block.library,
                    "module": block.module,
                    "section": block.section_name,
                    "block": block.index,
                    "records": block.records,
                    "block_size": len(block.data),
                    "compared_bits": block.compared_bits,
                    "relocations_masked": block.relocation_count,
                    "rcs_id": block.rcs_id,
                    "image": image.path.name,
                    "image_occurrences": len(matches),
                    "archive_candidate_count": len(candidates),
                    "archive_candidates": ",".join(candidates),
                    "confidence": confidence,
                    "exe_offset": f"0x{0x800 + payload_offset:x}",
                    "ram_va": f"0x{image.load_address + payload_offset:08x}",
                })
    return sorted(rows, key=lambda row: (
        str(row["image"]), int(str(row["exe_offset"]), 0),
        str(row["library"]), str(row["module"]), int(row["block"]),
    ))


FIELDS = (
    "library", "module", "section", "block", "records", "block_size",
    "compared_bits", "relocations_masked", "rcs_id", "image",
    "image_occurrences", "archive_candidate_count", "archive_candidates",
    "confidence", "exe_offset", "ram_va",
)


def write_tsv(path: Path | None, rows: Sequence[dict[str, object]]) -> None:
    if path is None:
        import sys
        stream = sys.stdout
        close = False
    else:
        path.parent.mkdir(parents=True, exist_ok=True)
        stream = path.open("w", encoding="utf-8", newline="")
        close = True
    try:
        stream.write(
            "# Exact byte matches after masking only relocations retained by "
            "the input Psy-Q objects.\n"
        )
        writer = csv.DictWriter(stream, FIELDS, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    finally:
        if close:
            stream.close()


def parse_args(arguments: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--psyk", type=Path, required=True,
                        help="psy-k 0.4.0 (or compatible) executable")
    parser.add_argument("--sdk-lib-dir", type=Path, required=True,
                        help="directory containing native Psy-Q .LIB files")
    parser.add_argument("--library", action="append", default=[],
                        help="library basename to scan; default: every .LIB")
    parser.add_argument("--exe", action="append", type=Path, required=True,
                        help="PS-X EXE to search; may be repeated")
    parser.add_argument("--output", type=Path,
                        help="output TSV; default: standard output")
    parser.add_argument("--minimum-bytes", type=int, default=32,
                        help="minimum joined object-section size (default: 32)")
    parser.add_argument(
        "--unique-only", action="store_true",
        help="emit only object sections with one match in that executable",
    )
    return parser.parse_args(arguments)


def main(arguments: Sequence[str] | None = None) -> int:
    args = parse_args(arguments)
    names = {name.upper() for name in args.library}
    libraries = sorted(args.sdk_lib_dir.glob("*.LIB"))
    if names:
        libraries = [path for path in libraries if path.name.upper() in names]
    if not libraries:
        raise SystemExit("no selected .LIB files")
    blocks = [
        block for library in libraries
        for block in library_blocks(args.psyk, library)
    ]
    rows = match_rows(
        blocks, [PsxImage.read(path) for path in args.exe], args.minimum_bytes,
    )
    if args.unique_only:
        rows = [row for row in rows if row["image_occurrences"] == 1]
    write_tsv(args.output, rows)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
