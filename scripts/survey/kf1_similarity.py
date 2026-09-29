#!/usr/bin/env python3
"""Rank KF2 functions by instruction-shape similarity to reconstructed KF1 functions.

Usage: kf1_similarity.py KF2_IMAGE [KF1_IMAGES...]  (default KF1 images: GAME.EXE OPEN.EXE)

Each instruction becomes a token of opcode, funct and registers; j/jal
targets, lui immediates and non-$sp immediates are masked, so address and
constant differences do not hide shared source. Output (TSV to stdout):
kf2_va, kf2_size, kf1_image, kf1_va, kf1_name, kf1_size, ratio, exact_shape.
"""

from __future__ import annotations

import difflib
import struct
import sys
from pathlib import Path

KF1 = Path("/home/sheep/Projects/kings-field")
KF2 = Path(__file__).resolve().parents[2]
KF1_RETAIL = Path("/home/sheep/Projects/kings-field-investigation/extracted/kings-field-japan-retail/disc")
LOADS = {("kf1", "GAME.EXE"): 0x80012000, ("kf1", "OPEN.EXE"): 0x80012000,
         ("kf2", "GAME.EXE"): 0x80011000, ("kf2", "OPEN.EXE"): 0x80011000, ("kf2", "END.EXE"): 0x80011000}


def rows(path: Path, image: str):
    for line in path.read_text().splitlines():
        if line.startswith(image + "\t"):
            yield line.split("\t")


def functions(project: str, image: str):
    root = KF1 if project == "kf1" else KF2
    vendored = {int(r[1], 16) for r in rows(root / "config/retail/functions_vendored.tsv", image)}
    names = {int(r[1], 16): r[2] for r in rows(root / "config/retail/function_identities.tsv", image)}
    for r in rows(root / "config/retail/functions.tsv", image):
        va, size = int(r[1], 16), int(r[3], 16)
        if va not in vendored and size >= 16:
            yield va, size, names.get(va, "")


def token(word: int) -> str:
    op = word >> 26
    rs, rt, rd, sa, funct = (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, (word >> 6) & 31, word & 63
    if op == 0:
        return f"R{funct}:{rs},{rt},{rd},{sa}"
    if op in (2, 3):
        return f"J{op}"
    if op == 0x0F:
        return f"lui:{rt}"
    if op in (1, 4, 5, 6, 7):
        return f"B{op}:{rs},{rt}"
    immediate = word & 0xFFFF
    keep = rs == 29 or op in (0x0C, 0x0D, 0x0E, 0x0A, 0x0B) and immediate < 0x100
    return f"I{op}:{rs},{rt}" + (f"#{immediate}" if keep else "")


def shapes(project: str, image: str):
    root = KF1_RETAIL if project == "kf1" else KF2 / "retail/jp"
    data = (root / image).read_bytes()
    load = LOADS[(project, image)]
    result = []
    for va, size, name in functions(project, image):
        offset = va - load + 0x800
        words = struct.unpack_from(f"<{size // 4}I", data, offset)
        result.append((va, size, name, tuple(token(w) for w in words)))
    return result


def main() -> int:
    kf2_image = sys.argv[1] if len(sys.argv) > 1 else "GAME.EXE"
    kf1_images = sys.argv[2:] or ["GAME.EXE", "OPEN.EXE"]
    targets = shapes("kf2", kf2_image)
    sources = [(image, *item) for image in kf1_images for item in shapes("kf1", image)]
    by_shape = {}
    for image, va, size, name, shape in sources:
        by_shape.setdefault(shape, []).append((image, va, size, name))
    print("kf2_va\tkf2_size\tkf1_image\tkf1_va\tkf1_name\tkf1_size\tratio\texact_shape")
    for va, size, _name, shape in targets:
        if shape in by_shape:
            image, kva, ksize, kname = by_shape[shape][0]
            print(f"{va:#010x}\t{size:#x}\t{image}\t{kva:#010x}\t{kname}\t{ksize:#x}\t1.000\tyes")
            continue
        best = (0.0, None)
        matcher = difflib.SequenceMatcher(autojunk=False)
        matcher.set_seq2(shape)
        for image, kva, ksize, kname, kshape in sources:
            if not 0.6 <= ksize / size <= 1.6:
                continue
            matcher.set_seq1(kshape)
            if matcher.real_quick_ratio() <= best[0] or matcher.quick_ratio() <= best[0]:
                continue
            ratio = matcher.ratio()
            if ratio > best[0]:
                best = (ratio, (image, kva, kname, ksize))
        if best[1] and best[0] >= 0.5:
            image, kva, kname, ksize = best[1]
            print(f"{va:#010x}\t{size:#x}\t{image}\t{kva:#010x}\t{kname}\t{ksize:#x}\t{best[0]:.3f}\tno")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
