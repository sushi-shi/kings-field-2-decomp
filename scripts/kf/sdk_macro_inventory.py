"""Inventory every define in a pinned Psy-Q include tree and its source uses.

This is a lexical census, not evidence that a macro produced retail code.
Definitions in inactive preprocessor branches remain visible on purpose.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import re
from pathlib import Path


DEFINE = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)(.*)$")
TOKEN = re.compile(r"\b[A-Za-z_]\w*\b")


def logical_lines(path: Path):
    lines = path.read_text(encoding="latin1").splitlines()
    index = 0
    while index < len(lines):
        start = index + 1
        parts = [lines[index]]
        while parts[-1].rstrip().endswith("\\") and index + 1 < len(lines):
            index += 1
            parts.append(lines[index])
        yield start, "\n".join(parts)
        index += 1


def definitions(include: Path):
    rows = []
    hashes = []
    for path in sorted(include.rglob("*.H")):
        relative = path.relative_to(include).as_posix()
        hashes.append((relative, hashlib.sha256(path.read_bytes()).hexdigest()))
        conditions = []
        for line, logical in logical_lines(path):
            stripped = logical.lstrip()
            directive = re.match(r"#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)", stripped)
            if directive:
                kind, rest = directive.groups()
                if kind in {"if", "ifdef", "ifndef"}:
                    conditions.append(f"{kind} {rest.strip()}")
                elif kind == "endif" and conditions:
                    conditions.pop()
                elif conditions:
                    conditions[-1] = f"{kind} {rest.strip()}".strip()
                continue
            physical = logical.split("\n")
            found = DEFINE.match(physical[0])
            if not found:
                continue
            name, tail = found.groups()
            if len(physical) > 1:
                tail += "\n" + "\n".join(physical[1:])
            function_like = tail.startswith("(")
            params = ""
            replacement = tail.strip()
            if function_like:
                end = tail.find(")")
                if end >= 0:
                    params = tail[1:end]
                    replacement = tail[end + 1 :].strip()
            rows.append(
                (relative, line, name, "function" if function_like else "object",
                 params, replacement.replace("\n", "\\n"), " > ".join(conditions) or "(unconditional)")
            )
    return rows, hashes


def code_without_comments_and_strings(source: str) -> str:
    # Preserve length/newlines so match offsets map to original source lines.
    out = list(source)
    index = 0
    state = "code"
    while index < len(source):
        char = source[index]
        next_char = source[index + 1] if index + 1 < len(source) else ""
        if state == "code":
            if char == "/" and next_char == "/":
                state = "line"
                out[index] = out[index + 1] = " "
                index += 2
                continue
            if char == "/" and next_char == "*":
                state = "block"
                out[index] = out[index + 1] = " "
                index += 2
                continue
            if char in {'"', "'"}:
                state = char
                out[index] = " "
        elif state == "line":
            if char == "\n":
                state = "code"
            else:
                out[index] = " "
        elif state == "block":
            if char == "*" and next_char == "/":
                out[index] = out[index + 1] = " "
                state = "code"
                index += 2
                continue
            if char != "\n":
                out[index] = " "
        else:
            if char == "\\" and next_char:
                out[index] = " "
                if next_char != "\n":
                    out[index + 1] = " "
                index += 2
                continue
            if char == state:
                state = "code"
            if char != "\n":
                out[index] = " "
        index += 1
    return "".join(out)


def source_sites(source_roots: list[Path], macros):
    names = {row[2] for row in macros}
    functions = {row[2] for row in macros if row[3] == "function"}
    fields_in_macros = sorted({field for row in macros if row[3] == "function"
                               for field in re.findall(r"->([A-Za-z_]\w*)", row[5])},
                              key=lambda field: (-len(field), field))
    field_write = re.compile(
        r"(?:\.|->)\s*(" + "|".join(map(re.escape, fields_in_macros))
        + r")\s*(?:[+*/%&|^-]?=)"
    )
    uses = []
    preprocessor_uses = []
    local_definitions = []
    fields = []
    files = []
    paths = sorted({path for root in source_roots
                    for suffix in ("*.c", "*.h") for path in root.rglob(suffix)})
    for path in paths:
        source = path.read_text(encoding="utf-8")
        clean = code_without_comments_and_strings(source)
        relative = path.as_posix()
        file_uses = 0
        file_fields = 0
        directive_lines = {}
        for line, logical in logical_lines(path):
            found = re.match(r"\s*#\s*(\w+)\b", logical)
            if not found:
                continue
            kind = found.group(1)
            for offset in range(logical.count("\n") + 1):
                directive_lines[line + offset] = kind
            local = DEFINE.match(logical.split("\n")[0]) if kind == "define" else None
            if local:
                local_definitions.append((relative, line, local.group(1),
                                          "function" if local.group(2).startswith("(") else "object",
                                          "sdk_name" if local.group(1) in names else "source_only"))
        for found in TOKEN.finditer(clean):
            name = found.group()
            if name not in names:
                continue
            line = clean.count("\n", 0, found.start()) + 1
            is_call = bool(re.match(r"\s*\(", clean[found.end() :]))
            if name in functions and not is_call:
                if line not in directive_lines:
                    continue
            row = (relative, line, name, "call" if is_call else "token")
            if line in directive_lines:
                preprocessor_uses.append((*row, directive_lines[line]))
            else:
                uses.append(row)
                file_uses += 1
        for found in field_write.finditer(clean):
            line = clean.count("\n", 0, found.start()) + 1
            fields.append((relative, line, found.group(1)))
            file_fields += 1
        files.append((relative, len(source.splitlines()), hashlib.sha256(source.encode()).hexdigest(),
                      file_uses, file_fields, "lexically_scanned"))
    return uses, preprocessor_uses, local_definitions, fields, files


def write_tsv(path: Path, heading, rows):
    with path.open("w", newline="", encoding="utf-8") as output:
        writer = csv.writer(output, delimiter="\t", lineterminator="\n")
        writer.writerow(heading)
        writer.writerows(rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--include", type=Path, required=True)
    parser.add_argument("--source", type=Path, action="append",
                        help="source root to scan (repeat for src, include, vendor/include)")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    macros, hashes = definitions(args.include)
    uses, preprocessor_uses, local_definitions, fields, files = source_sites(
        args.source or [Path("src")], macros)
    write_tsv(args.output / "definitions.tsv",
              ("header", "line", "name", "kind", "parameters", "replacement", "condition"), macros)
    public_index = [(*row[:5], hashlib.sha256(row[5].encode("latin1")).hexdigest(), row[6])
                    for row in macros]
    write_tsv(args.output / "macro_index.tsv",
              ("header", "line", "name", "kind", "parameters", "replacement_sha256",
               "condition"), public_index)
    write_tsv(args.output / "header_hashes.tsv", ("header", "sha256"), hashes)
    write_tsv(args.output / "uses.tsv", ("source", "line", "name", "form"), uses)
    write_tsv(args.output / "preprocessor_uses.tsv",
              ("source", "line", "name", "form", "directive"), preprocessor_uses)
    write_tsv(args.output / "local_definitions.tsv",
              ("source", "line", "name", "kind", "sdk_overlap"), local_definitions)
    write_tsv(args.output / "field_writes.tsv", ("source", "line", "field"), fields)
    write_tsv(args.output / "files.tsv",
              ("source", "lines", "sha256", "macro_mentions", "field_writes", "scan_status"), files)
    print(f"{len(hashes)} headers; {len(macros)} definitions; "
          f"{len(uses)} source mentions; {len(preprocessor_uses)} preprocessor mentions; "
          f"{len(local_definitions)} local definitions; {len(fields)} field writes; "
          f"{len(files)} C/header files")


if __name__ == "__main__":
    main()
