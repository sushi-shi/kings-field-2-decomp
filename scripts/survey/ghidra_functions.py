#!/usr/bin/env python3
"""Dump Ghidra (ghidra_psx_ldr) auto-analysis function lists for PS-X EXEs.

Usage: ghidra_functions.py OUT_DIR EXE...
Writes OUT_DIR/<region>_<exe>.tsv with: va, size, name, source.
Machine seed only: boundaries are Ghidra's, not reviewed.
"""

from __future__ import annotations

import sys
import tempfile
from pathlib import Path

import pyghidra


def main() -> int:
    out = Path(sys.argv[1])
    out.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="kf2-ghidra-") as root:
        for arg in sys.argv[2:]:
            exe = Path(arg)
            tag = f"{exe.parent.name}_{exe.name}"
            with pyghidra.open_program(
                exe,
                project_location=Path(root) / tag,
                project_name="analysis",
                analyze=True,
            ) as api:
                program = api.getCurrentProgram()
                rows = []
                for fn in program.getFunctionManager().getFunctions(True):
                    body = fn.getBody()
                    rows.append((
                        fn.getEntryPoint().getOffset(),
                        body.getNumAddresses(),
                        fn.getName(),
                        str(fn.getSymbol().getSource()),
                    ))
                with open(out / f"{tag}.tsv", "w") as fh:
                    fh.write("va\tsize\tname\tsource\n")
                    for va, size, name, source in rows:
                        fh.write(f"0x{va:08x}\t{size}\t{name}\t{source}\n")
                print(f"{tag}: {program.getLanguageID()} functions={len(rows)}", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
