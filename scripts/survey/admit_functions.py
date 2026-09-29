#!/usr/bin/env python3
"""Admit carved functions and set reviewed identities from a JSON spec.

Usage: admit_functions.py SPEC.json

SPEC = {"image": "GAME.EXE", "owner": "vector_math", "evidence": "...",
        "functions": {"0x800151e4": {"size": "0x40", "name": "...",
                      "return": "void", "parameters": "s16 scale;s32 *vector"}}}

A function missing from functions.tsv is added as a manual carve; every listed
function gets its identity row (created if absent). Rows stay sorted.
"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from scripts.kf.retail import IMAGE_LAYOUTS, IMAGE_ORDER  # noqa: E402

CONFIG = Path("config/retail")


def load(path):
    lines = path.read_text().splitlines()
    comments = [line for line in lines if line.startswith("#")]
    body = [line.split("\t") for line in lines if not line.startswith("#")]
    return comments, body[0], body[1:]


def save(path, comments, header, rows):
    rows.sort(key=lambda row: (IMAGE_ORDER[row[0]], int(row[1], 16)))
    path.write_text("\n".join(comments + ["\t".join(header)] + ["\t".join(r) for r in rows]) + "\n")


spec = json.loads(Path(sys.argv[1]).read_text())
image = spec["image"]
layout = IMAGE_LAYOUTS[image]
functions = {int(va, 16): item for va, item in spec["functions"].items()}

comments, header, rows = load(CONFIG / "functions.tsv")
present = {int(row[1], 16) for row in rows if row[0] == image}
added = 0
for va, item in functions.items():
    if va in present:
        continue
    size = item["size"]
    values = dict(image=image, va=f"0x{va:08x}", file_offset=hex(va - layout.load_address + 0x800),
                  size=size, body_size=size, fragments="1", kind="function",
                  confidence="mips-frame-carve", provenance=f"manual:{spec['owner']}",
                  note=item.get("carve_note", "bounded by the preceding jr ra delay slot and the next start"))
    row = [values.get(column, "") for column in header]
    rows.append(row)
    added += 1
save(CONFIG / "functions.tsv", comments, header, rows)

comments, header, rows = load(CONFIG / "function_identities.tsv")
index = {int(row[1], 16): row for row in rows if row[0] == image}
for va, item in functions.items():
    row = index.get(va)
    if row is None:
        row = [""] * len(header)
        row[0], row[1] = image, f"0x{va:08x}"
        row[header.index("signature_confidence")] = "candidate"
        rows.append(row)
    name = item["name"]
    owner, _, action = name.partition("_")
    values = dict(name=name, owner=item.get("owner", ""), action=item.get("action", ""),
                  return_type=item.get("return", "void"), parameters=item.get("parameters", ""),
                  name_confidence="supported", evidence=spec.get("evidence", "asm;cfg;callers;callees"),
                  note=item.get("note", spec.get("note", "")))
    for key, value in values.items():
        row[header.index(key)] = value
save(CONFIG / "function_identities.tsv", comments, header, rows)
print(f"{image}: census added={added} identities={len(functions)}")
