#!/usr/bin/env python3
"""Plain objdump listing of IMAGE between START and END (KF2 overlays at 0x80011000)."""
import subprocess
import sys
import tempfile
from pathlib import Path

image, start, end = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
base = 0x80011000 if image != "PSX.EXE" else 0x80010000
data = (Path("retail/jp") / image).read_bytes()
offset = start - base + 0x800
with tempfile.NamedTemporaryFile(suffix=".bin") as blob:
    blob.write(data[offset:offset + end - start])
    blob.flush()
    out = subprocess.run(
        ["mipsel-linux-gnu-objdump", "-D", "-b", "binary", "-m", "mips:3000",
         "--adjust-vma", hex(start), blob.name],
        capture_output=True, text=True, check=True,
    ).stdout
print(out.split("<.data>:\n", 1)[1])
