#!/usr/bin/env python3
"""Count compiler/assembler codegen fingerprints in PS-X EXEs.

Library-matched bytes (from psyq_sig_census output, objects >= 64 bytes) are
counted separately from the remainder ("other"), which is mostly game code.

Fingerprints:
  epilogue  delay slot of each ``jr $ra``: addiu sp / nop / other
  gp        loads/stores based on $gp (small-data -G model)
  fp        ``addu/move $fp,$sp`` frame-pointer setups (typical of -O0)
  div       div/divu followed by an ASPSX/GAS divide-by-zero ``break 7`` trap
  ovf       signed-div overflow ``break 6`` trap
"""

from __future__ import annotations

import csv
import struct
import sys
from collections import Counter
from pathlib import Path

JR_RA = 0x03E00008


def load(path: Path):
    data = path.read_bytes()
    base = struct.unpack_from("<I", data, 0x18)[0]
    payload = data[0x800:]
    words = struct.unpack_from(f"<{len(payload)//4}I", payload)
    return base, words


def lib_mask(hits: Path, exe: str, base: int, n: int) -> list[bool]:
    mask = [False] * n
    with hits.open() as fh:
        for r in csv.DictReader(fh, delimiter="\t"):
            if r["exe"] != exe or int(r["size"]) < 64:
                continue
            start = (int(r["va"], 16) - base) // 4
            for i in range(start, min(n, start + int(r["size"]) // 4)):
                mask[i] = True
    return mask


def main() -> int:
    hits = Path(sys.argv[1])
    print("exe\tpart\twords\tjr_ra\tep_addiu_sp\tep_nop\tep_other\tgp_ls\tfp_setup\tdiv\tdiv_break7\tbreak6")
    for exe in sys.argv[2:]:
        base, w = load(Path(exe))
        mask = lib_mask(hits, exe, base, len(w))
        for part, want in (("sdk", True), ("other", False)):
            c = Counter()
            for i, x in enumerate(w):
                if mask[i] != want:
                    continue
                c["words"] += 1
                op = x >> 26
                if x == JR_RA and i + 1 < len(w):
                    c["jr"] += 1
                    d = w[i + 1]
                    if d >> 16 == 0x27BD:
                        c["ep_sp"] += 1
                    elif d == 0:
                        c["ep_nop"] += 1
                    else:
                        c["ep_other"] += 1
                if op in (0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B) and (x >> 21) & 31 == 28:
                    c["gp"] += 1
                # addu $fp,$sp,$zero  (move fp,sp)
                if x == 0x03A0F021:
                    c["fp"] += 1
                if op == 0 and x & 0x3F in (0x1A, 0x1B):
                    c["div"] += 1
                    window = w[i + 1 : i + 5]
                    if 0x0007000D in window:
                        c["div7"] += 1
                if x == 0x0006000D:
                    c["b6"] += 1
            print(
                f"{exe}\t{part}\t{c['words']}\t{c['jr']}\t{c['ep_sp']}\t{c['ep_nop']}\t"
                f"{c['ep_other']}\t{c['gp']}\t{c['fp']}\t{c['div']}\t{c['div7']}\t{c['b6']}"
            )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
