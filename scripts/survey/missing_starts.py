#!/usr/bin/env python3
"""List function starts after `jr ra` + delay slot that the census lacks.

Usage: missing_starts.py IMAGE START END
"""
import struct
import sys
from pathlib import Path

image, start, end = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
data = (Path("retail/jp") / image).read_bytes()
base = 0x80011000


def word(address):
    offset = address - base + 0x800
    return struct.unpack_from("<I", data, offset)[0]


known = set()
for table in ("functions.tsv", "functions_vendored.tsv"):
    for line in Path("config/retail", table).read_text().splitlines():
        fields = line.split("\t")
        if fields[0] == image:
            known.add(int(fields[1], 16))
address = start
while address < end:
    if word(address) == 0x03E00008:
        candidate = address + 8
        while word(candidate) == 0:
            candidate += 4
        if candidate not in known and candidate < end:
            print(f"{candidate:#010x}\t{word(candidate):08x}")
    address += 4
