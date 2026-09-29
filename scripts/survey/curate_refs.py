#!/usr/bin/env python3
"""Curate data identities and HI/LO relocation rows for one function range.

Usage: curate_refs.py SPEC.json

SPEC = {
  "image": "OPEN.EXE", "range": ["0x800120c8", "0x80012204"],
  "owner": "display", "evidence": "asm;xrefs",
  "objects": {"display_buffers": ["0x800a6438", "0x20e8", "KfDisplayBuffer[2]", "note"]},
  "code": {"strCallback": "0x8001396c"},         # optional function-address referents
  "referents": {"display_current": ["0x800a2898", "0x4"]}  # admitted objects: relocations only
}

For every LUI/low pair inside the range whose target falls in a listed object
(or equals a listed code address), the matching relocs.tsv row is marked
reviewed (or added when the seed census lacked it). Identities covering a listed
object's extent are replaced by one identity for that object. Storage is `bss`
when the object extends past the image's load end, else `load`. Rows keep
KF1's schema; nothing else in config/retail is touched.
"""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from scripts.kf.retail import IMAGE_LAYOUTS, IMAGE_ORDER  # noqa: E402

CONFIG = Path("config/retail")


def load(path: Path):
    lines = path.read_text().splitlines()
    comments = [line for line in lines if line.startswith("#")]
    body = [line for line in lines if not line.startswith("#")]
    return comments, body[0].split("\t"), [line.split("\t") for line in body[1:]]


def save(path: Path, comments, header, rows, key=1) -> None:
    rows.sort(key=lambda r: (IMAGE_ORDER[r[0]], int(r[key], 16)))
    path.write_text("\n".join(comments + ["\t".join(header)] + ["\t".join(r) for r in rows]) + "\n")


def main() -> int:
    spec = json.loads(Path(sys.argv[1]).read_text())
    image = spec["image"]
    layout = IMAGE_LAYOUTS[image]
    start, end = (int(v, 16) for v in spec["range"])
    objects = {name: (int(v[0], 16), int(v[1], 16), v[2], v[3]) for name, v in spec.get("objects", {}).items()}
    code = {int(v, 16): name for name, v in spec.get("code", {}).items()}
    referents = {name: (int(v[0], 16), int(v[1], 16)) for name, v in spec.get("referents", {}).items()}

    def owner(address: int, include_referents: bool = True):
        for name, (va, size, _, _) in objects.items():
            if va <= address < va + size:
                return name, va
        if include_referents:
            for name, (va, size) in referents.items():
                if va <= address < va + size:
                    return name, va
        return None, None

    # identities
    comments, header, rows = load(CONFIG / "data_identities.tsv")
    kept = [r for r in rows if not (r[0] == image and owner(int(r[1], 16), False)[0])]
    for name, (va, size, datatype, note) in objects.items():
        row = [""] * len(header)
        storage = "bss" if va + size > layout.load_end else "load"
        values = dict(image=image, va=f"0x{va:08x}", size=hex(size), name=name, scope="global",
                      storage=storage, datatype=datatype, owner=spec.get("owner", ""),
                      confidence="supported", evidence=spec.get("evidence", "asm;xrefs"), note=note)
        for key, value in values.items():
            row[header.index(key)] = value
        kept.append(row)
    save(CONFIG / "data_identities.tsv", comments, header, kept)

    # relocations
    comments, header, rows = load(CONFIG / "relocs.tsv")
    by_site = {(r[0], r[1]): r for r in rows}
    pairs = subprocess.run(
        [sys.executable, str(Path(__file__).with_name("hilo_pairs.py")),
         f"retail/jp/{image}", hex(start), hex(end)],
        capture_output=True, text=True, check=True,
    ).stdout.splitlines()
    added = reviewed = 0
    for line in pairs:
        lui, low, opcode, target_text = line.split("\t")
        target = int(target_text, 16)
        name, base = owner(target)
        if name is None and target not in code:
            continue
        va, size = ((objects.get(name) or referents[name])[:2]) if name else (target, 1)
        region = "bss" if (name and va + size > layout.load_end) else "load"
        target_name = code.get(target) or (name if target == base else "")
        row = by_site.get((image, lui))
        if row is None:
            row = [""] * len(header)
            values = dict(image=image, site_va=lui,
                          site_file_offset=hex(int(lui, 16) - layout.load_address + 0x800),
                          paired_site_va=low, kind="mips_hi16_lo16", channel="reachable-code",
                          target_va=target_text, opcode=f"lui+{opcode}")
            for key, value in values.items():
                row[header.index(key)] = value
            rows.append(row)
            by_site[(image, lui)] = row
            added += 1
        else:
            reviewed += 1
        row[header.index("target_region")] = region
        row[header.index("target_name")] = target_name
        row[header.index("confidence")] = "paired-reviewed"
        row[header.index("status")] = "reviewed"
        row[header.index("provenance")] = f"manual:{spec.get('owner', 'curate_refs')}"
    save(CONFIG / "relocs.tsv", comments, header, rows)
    print(f"{image}: identities={len(objects)} relocations added={added} reviewed={reviewed}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
