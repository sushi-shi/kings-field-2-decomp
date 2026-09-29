#!/usr/bin/env python3
"""List LUI/low-immediate address pairs inside a PS-X EXE address range.

Usage: hilo_pairs.py EXE START END
Prints: lui_site  low_site  low_opcode  target  (signed-low carry applied;
`ori` is reported separately because it is zero-extended).
"""
import struct
import sys

LOW = {0x09: "addiu", 0x0D: "ori", 0x20: "lb", 0x21: "lh", 0x23: "lw", 0x24: "lbu",
       0x25: "lhu", 0x28: "sb", 0x29: "sh", 0x2B: "sw"}

data = open(sys.argv[1], "rb").read()
base = struct.unpack_from("<I", data, 0x18)[0]
start, end = int(sys.argv[2], 0), int(sys.argv[3], 0)
words = struct.unpack_from(f"<{(end - start) // 4}I", data, 0x800 + start - base)
for i, word in enumerate(words):
    if word >> 26 != 0x0F:
        continue
    reg, high = (word >> 16) & 31, word & 0xFFFF
    for j in range(i + 1, min(i + 8, len(words))):
        low = words[j]
        op = low >> 26
        if op in LOW and (low >> 21) & 31 == reg:
            imm = low & 0xFFFF
            target = (high << 16) + (imm if op == 0x0D else imm - (0x10000 if imm & 0x8000 else 0))
            print(f"0x{start + 4 * i:08x}\t0x{start + 4 * j:08x}\t{LOW[op]}\t0x{target & 0xFFFFFFFF:08x}")
            break
        if (low >> 26) == 0x0F and (low >> 16) & 31 == reg:
            break
