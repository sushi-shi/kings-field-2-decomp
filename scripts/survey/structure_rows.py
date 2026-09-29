#!/usr/bin/env python3
"""Append inventory rows for header structures missing from structures.tsv.

Usage: structure_rows.py EVIDENCE

Layouts come from the same header parser that validates the inventory, so
sizes and offsets cannot drift from the C declarations. Fields named
unknown_* are recorded as opaque; the rest as supported with EVIDENCE.
Existing rows are never rewritten. Review notes before committing.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from scripts.kf.inventory import _header_structure_layouts  # noqa: E402

evidence = sys.argv[1]
config = Path("config/retail")


def load(path):
    lines = path.read_text().splitlines()
    comments = [line for line in lines if line.startswith("#")]
    body = [line for line in lines if not line.startswith("#") and line.strip()]
    return comments, body[0], [line.split("\t") for line in body[1:]]


layouts = _header_structure_layouts()
s_comments, s_header, s_rows = load(config / "structures.tsv")
f_comments, f_header, f_rows = load(config / "structure_fields.tsv")
known = {row[0] for row in s_rows}
added = []
for name, layout in layouts.items():
    if name in known:
        continue
    s_rows.append([name, f"{layout.size:#04x}", "supported", evidence, "layout from the reconstructed header"])
    for field in layout.fields:
        opaque = field.name.startswith("unknown_")
        f_rows.append([name, f"{field.offset:#04x}", f"{field.size:#04x}", field.name, field.datatype,
                       "opaque" if opaque else "supported", evidence,
                       "not yet modelled" if opaque else "member used by reconstructed code"])
    added.append(name)
s_rows.sort(key=lambda row: row[0])
f_rows.sort(key=lambda row: (row[0], int(row[1], 16)))
(config / "structures.tsv").write_text("\n".join(s_comments + [s_header] + ["\t".join(r) for r in s_rows]) + "\n")
(config / "structure_fields.tsv").write_text("\n".join(f_comments + [f_header] + ["\t".join(r) for r in f_rows]) + "\n")
print("added:", ", ".join(added) or "none")
