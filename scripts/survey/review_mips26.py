#!/usr/bin/env python3
"""Review direct j/jal relocation rows inside claimed functions.

Usage: review_mips26.py IMAGE START END OWNER

The delinker withholds a candidate mips26 row whose site its reachability
pass classifies as instruction-word rather than reachable code. For every
row in [START, END) this re-decodes the instruction and marks the row
reviewed only if the opcode matches, the decoded target equals the row's
target, and the target is a census/vendored function start or lies inside
the function that contains the site.
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from scripts.kf.relocations import decode_mips26_target  # noqa: E402
from scripts.kf.retail import IMAGE_LAYOUTS  # noqa: E402

image, start, end, owner = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16), sys.argv[4]
layout = IMAGE_LAYOUTS[image]
data = (Path("retail/jp") / image).read_bytes()
config = Path("config/retail")

starts, extents = set(), []
for table in ("functions.tsv", "functions_vendored.tsv"):
    for line in (config / table).read_text().splitlines():
        fields = line.split("\t")
        if fields[0] == image:
            va = int(fields[1], 16)
            starts.add(va)
            size = int(fields[3 if table == "functions.tsv" else 2], 16)
            extents.append((va, va + size))


def containing(address):
    for low, high in extents:
        if low <= address < high:
            return low, high
    return None


path = config / "relocs.tsv"
lines = path.read_text().splitlines()
header = [line for line in lines if not line.startswith("#")][0].split("\t")
column = {name: index for index, name in enumerate(header)}
reviewed = rejected = 0
out = []
for line in lines:
    fields = line.split("\t")
    if (len(fields) == len(header) and fields[0] == image and fields[column["kind"]] == "mips26"
            and start <= int(fields[1], 16) < end and fields[column["status"]] != "reviewed"):
        site = int(fields[1], 16)
        target = int(fields[column["target_va"]], 16)
        word = struct.unpack_from("<I", data, site - layout.load_address + 0x800)[0]
        opcode = {"j": 2, "jal": 3}.get(fields[column["opcode"]])
        extent = containing(site)
        ok = (opcode == word >> 26 and decode_mips26_target(site, word) == target
              and (target in starts or (extent and extent[0] <= target < extent[1])))
        if ok:
            fields[column["status"]] = "reviewed"
            fields[column["provenance"]] = f"manual:{owner}"
            reviewed += 1
        else:
            rejected += 1
    out.append("\t".join(fields))
path.write_text("\n".join(out) + "\n")
print(f"{image}: mips26 reviewed={reviewed} not-validated={rejected}")
