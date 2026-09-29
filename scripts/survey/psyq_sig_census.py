#!/usr/bin/env python3
"""Match every Psy-Q object signature (ghidra_psx_ldr corpus) against PS-X EXEs.

Each signature is a whole object ``.text`` with relocated bytes wildcarded.
Psy-Q links whole objects, so an unmodified library object appears as one
contiguous match.  For every match location we record *which SDK versions'*
signature for that object also matches there: the intersection is the version
window that object pins.

Output: TSV rows ``exe  va  size  fixed  lib  obj  versions`` on stdout.
"""

from __future__ import annotations

import json
import re
import struct
import sys
from pathlib import Path

MIN_FIXED = 24  # fixed (non-wildcard) bytes required to trust a match


def load_exe(path: Path) -> tuple[int, bytes]:
    data = path.read_bytes()
    if data[:8] != b"PS-X EXE":
        raise SystemExit(f"{path}: not a PS-X EXE")
    load = struct.unpack_from("<I", data, 0x18)[0]
    return load, data[0x800:]


def compile_sig(sig: str) -> tuple[re.Pattern[bytes], int, int]:
    toks = sig.split()
    parts = []
    fixed = 0
    for tok in toks:
        if tok == "??":
            parts.append(b".")
        else:
            parts.append(re.escape(bytes([int(tok, 16)])))
            fixed += 1
    return re.compile(b"".join(parts), re.DOTALL), len(toks), fixed


def load_corpus(root: Path):
    corpus = []  # (version, lib, obj, pattern, size, fixed)
    for vdir in sorted(p for p in root.iterdir() if p.is_dir() and p.name.isdigit()):
        for jf in sorted(vdir.glob("*.json")):
            lib = jf.name.removesuffix(".json")
            for obj in json.loads(jf.read_text()):
                sig = obj.get("sig", "")
                if not sig.strip():
                    continue
                pat, size, fixed = compile_sig(sig)
                if fixed < MIN_FIXED:
                    continue
                corpus.append((vdir.name, lib, obj["name"], pat, size, fixed))
    return corpus


def main() -> int:
    root = Path(sys.argv[1])
    exes = [Path(p) for p in sys.argv[2:]]
    corpus = load_corpus(root)
    print(f"# corpus objects: {len(corpus)}", file=sys.stderr)
    print("exe\tva\tsize\tfixed\tlib\tobj\tversions")
    for exe in exes:
        load, payload = load_exe(exe)
        hits: dict[tuple[int, str, str], dict] = {}
        for version, lib, obj, pat, size, fixed in corpus:
            for m in pat.finditer(payload):
                off = m.start()
                if off & 3:
                    continue
                key = (off, lib, obj)
                h = hits.setdefault(key, {"size": size, "fixed": fixed, "versions": []})
                h["versions"].append(version)
                h["size"] = max(h["size"], size)
                h["fixed"] = max(h["fixed"], fixed)
        for (off, lib, obj), h in sorted(hits.items()):
            print(
                f"{exe}\t0x{load + off:08x}\t{h['size']}\t{h['fixed']}\t{lib}\t{obj}\t"
                + ",".join(h["versions"])
            )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
