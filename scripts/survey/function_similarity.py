#!/usr/bin/env python3
"""Compare functions between PS-X EXE builds with relocation-aware masking.

Input: a function list (va, size) per image, e.g. the Ghidra seed TSVs.
Each function's words are normalized so that link-time values do not count:

* ``j``/``jal`` keep only the opcode;
* ``lui`` keeps opcode+rt, and any later immediate that uses a register most
  recently loaded by ``lui`` (the paired %lo) is masked;
* ``$gp``-relative immediates are masked (small-data addresses).

Branch displacements, stack offsets, struct offsets, registers and constants
stay significant.  Two functions are *identical* when their normalized words
are equal; otherwise the best *similar* partner is found by 4-gram Jaccard over
the normalized words (only among functions of comparable size).
"""

from __future__ import annotations

import csv
import hashlib
import struct
import sys
from collections import defaultdict
from pathlib import Path

GP = 28


def load_payload(path: Path) -> tuple[int, bytes]:
    data = path.read_bytes()
    return struct.unpack_from("<I", data, 0x18)[0], data[0x800:]


def shape(words: list[int]) -> tuple[int, ...]:
    """Opcode/function-code only: insensitive to registers, immediates and
    link-time values, so a recompiled function keeps most of its shape."""
    out = []
    for x in words:
        op = x >> 26
        if x == 0:
            continue  # nops come and go with scheduling
        if op == 0:
            out.append(x & 0x3F)
        elif op == 1:
            out.append(0x100 | ((x >> 16) & 31))
        elif op in (0x10, 0x12):
            out.append((op << 8) | ((x >> 21) & 31))
        else:
            out.append(op << 8)
    return tuple(out)


SHAPE = False


def normalize(words: list[int]) -> tuple[int, ...]:
    if SHAPE:
        return shape(words)
    lui_regs: set[int] = set()
    out = []
    for x in words:
        op = x >> 26
        rs = (x >> 21) & 31
        rt = (x >> 16) & 31
        if op in (2, 3):
            out.append(op << 26)
            continue
        if op == 0x0F:
            lui_regs.add(rt)
            out.append(x & 0xFFFF0000)
            continue
        itype_imm = op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E) or 0x20 <= op <= 0x2E or op in (0x32, 0x3A)
        if itype_imm and (rs in lui_regs or rs == GP):
            out.append(x & 0xFFFF0000)
        else:
            out.append(x)
        # A write to rt (ALU-immediate or load) or rd (R-type) ends the %hi
        # pairing for that register; later immediates on it are real offsets.
        if 0x08 <= op <= 0x0E or 0x20 <= op <= 0x26:
            lui_regs.discard(rt)
        elif op == 0:
            lui_regs.discard((x >> 11) & 31)
    return tuple(out)


def read_functions(tsv: Path, exe: Path, minimum: int = 8):
    base, payload = load_payload(exe)
    funcs = []
    with tsv.open() as fh:
        rows = list(csv.DictReader((line for line in fh if not line.startswith("#")), delimiter="\t"))
    for r in rows:
        va = int(r["va"], 16)
        size = int(r["size"], 0)
        off = va - base
        if size < minimum or off < 0 or off + size > len(payload):
            continue
        words = list(struct.unpack_from(f"<{size // 4}I", payload, off))
        norm = normalize(words)
        grams = {norm[i : i + 4] for i in range(max(1, len(norm) - 3))}
        funcs.append({
            "va": va, "size": size, "name": r.get("name", ""),
            "hash": hashlib.sha1(repr(norm).encode()).hexdigest(),
            "grams": grams,
        })
    return funcs


def compare(a: list[dict], b: list[dict], threshold: float = 0.5):
    bh = defaultdict(list)
    for f in b:
        bh[f["hash"]].append(f)
    identical = [f for f in a if f["hash"] in bh]
    rest_a = [f for f in a if f["hash"] not in bh]
    ahashes = {f["hash"] for f in a}
    rest_b = [f for f in b if f["hash"] not in ahashes]
    similar = []
    for f in rest_a:
        best, score = None, 0.0
        for g in rest_b:
            if not (0.6 * f["size"] <= g["size"] <= 1.67 * f["size"]):
                continue
            inter = len(f["grams"] & g["grams"])
            if not inter:
                continue
            s = inter / len(f["grams"] | g["grams"])
            if s > score:
                best, score = g, s
        if best is not None and score >= threshold:
            similar.append((f, best, score))
    return identical, similar, rest_a


def main() -> int:
    # usage: function_similarity.py LABEL=TSV:EXE LABEL=TSV:EXE
    global SHAPE
    args = sys.argv[1:]
    if args and args[0] == "--shape":
        SHAPE = True
        args = args[1:]
    specs = []
    for arg in args:
        label, rest = arg.split("=", 1)
        tsv, exe = rest.split(":", 1)
        specs.append((label, read_functions(Path(tsv), Path(exe))))
    print("left\tright\tleft_funcs\tleft_bytes\tidentical\tidentical_bytes\tsimilar>=0.5\tsimilar_bytes\tunmatched\tunmatched_bytes")
    for i, (la, fa) in enumerate(specs):
        for lb, fb in specs[i + 1 :]:
            ident, sim, _ = compare(fa, fb)
            sim_ids = {id(f) for f, _, _ in sim}
            idents = {id(f) for f in ident}
            un = [f for f in fa if id(f) not in sim_ids and id(f) not in idents]
            tb = sum(f["size"] for f in fa)
            print(
                f"{la}\t{lb}\t{len(fa)}\t{tb}\t{len(ident)}\t{sum(f['size'] for f in ident)}\t"
                f"{len(sim)}\t{sum(f['size'] for f, _, _ in sim)}\t{len(un)}\t{sum(f['size'] for f in un)}"
            )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
