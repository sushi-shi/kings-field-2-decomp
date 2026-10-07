"""Join the pinned SDK macro census to source-site evidence.

The output is a coverage ledger, not a claim that matching literals prove
original macro spelling. Unresolved semantic sites remain explicit.
"""

from __future__ import annotations

import argparse
import collections
import csv
import re
from pathlib import Path

from scripts.kf.sdk_macro_inventory import code_without_comments_and_strings


NUMBER = re.compile(r"(?<![A-Za-z_0-9])(?:0[xX][0-9a-fA-F]+|[0-9]+)[uUlL]*(?![A-Za-z_0-9])")
SIMPLE_NUMBER = re.compile(r"^(-?(?:0[xX][0-9a-fA-F]+|[0-9]+))[uUlL]*$")
FIELD = re.compile(r"->([A-Za-z_]\w*)")


def read_tsv(path: Path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def trial_rows(census: Path, name: str):
    local = census / f"{name}.tsv"
    return read_tsv(local) if local.exists() else []


def number_value(spelling: str) -> int:
    core = re.sub(r"[uUlL]+$", "", spelling)
    if core.startswith("-"):
        return -number_value(core[1:])
    if core.startswith(("0x", "0X")):
        return int(core, 16)
    if len(core) > 1 and core.startswith("0"):
        try:
            return int(core, 8)
        except ValueError:
            return int(core, 10)
    return int(core, 10)


def simple_number(replacement: str) -> str | None:
    text = re.sub(r"/\*.*?\*/", "", replacement, flags=re.DOTALL).strip()
    while text.startswith("(") and text.endswith(")"):
        text = text[1:-1].strip()
    found = SIMPLE_NUMBER.fullmatch(text)
    return found.group(1) if found else None


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--census", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    definitions = read_tsv(args.census / "definitions.tsv")
    uses = read_tsv(args.census / "uses.tsv")
    preprocessor = read_tsv(args.census / "preprocessor_uses.tsv")
    fields = read_tsv(args.census / "field_writes.tsv")
    files = read_tsv(args.census / "files.tsv")
    use_count = collections.Counter(row["name"] for row in uses)
    pp_count = collections.Counter(row["name"] for row in preprocessor)
    field_count = collections.Counter(row["field"] for row in fields)
    source_numbers = collections.Counter()
    numbers_by_file = {}
    for row in files:
        source = code_without_comments_and_strings(Path(row["source"]).read_text())
        found = collections.Counter(number_value(m.group()) for m in NUMBER.finditer(source))
        numbers_by_file[row["source"]] = found
        source_numbers.update(found)

    args.output.mkdir(parents=True, exist_ok=True)
    with (args.output / "definition_decisions.tsv").open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("header", "line", "name", "kind", "source_name_sites",
                         "preprocessor_sites", "field_write_candidates", "literal_collisions",
                         "decision"))
        for row in definitions:
            name = row["name"]
            field_sites = sum(field_count[f] for f in set(FIELD.findall(row["replacement"])))
            numeric = simple_number(row["replacement"])
            literal_sites = source_numbers[number_value(numeric)] if numeric else 0
            if row["header"] == "LIBGTE.H" and row["kind"] == "function":
                decision = "assembler_only"
            elif use_count[name]:
                decision = "source_name_present_see_site_ledger"
            elif pp_count[name]:
                decision = "preprocessor_or_local_shadow"
            elif row["kind"] == "function" and field_sites:
                decision = "field_shape_candidate_see_site_ledger"
            elif row["kind"] == "object" and literal_sites:
                decision = "literal_collision_no_macro_provenance"
            else:
                decision = "no_lexical_candidate"
            writer.writerow((row["header"], row["line"], name, row["kind"],
                             use_count[name], pp_count[name], field_sites,
                             literal_sites, decision))

    use_by_file = collections.Counter(row["source"] for row in uses)
    pp_by_file = collections.Counter(row["source"] for row in preprocessor)
    field_by_file = collections.Counter(row["source"] for row in fields)
    values = {number_value(number) for row in definitions
              if row["kind"] == "object"
              if (number := simple_number(row["replacement"]))}
    with (args.output / "file_decisions.tsv").open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source", "sha256", "source_name_sites", "preprocessor_sites",
                         "field_write_candidates", "numeric_literal_collisions", "decision"))
        for row in files:
            path = row["source"]
            numeric_sites = sum(count for value, count in numbers_by_file[path].items()
                                if value in values)
            decision = "see_name_and_field_site_ledgers" if (use_by_file[path] or field_by_file[path]) else (
                "literal_only_no_macro_provenance" if numeric_sites else "no_lexical_candidate")
            writer.writerow((path, row["sha256"], use_by_file[path], pp_by_file[path],
                             field_by_file[path], numeric_sites, decision))
    vector_trials = trial_rows(args.census, "vector_trials")
    vector_starts = {(row["source"], int(row["line"])): row for row in vector_trials}
    packet_trials = trial_rows(args.census, "packet_tag_trials")
    packet_lines = {(row["source"], int(row["line"]) + 1) for row in packet_trials}
    with (args.output / "name_site_decisions.tsv").open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source", "line", "name", "form", "decision"))
        for row in uses:
            name, path = row["name"], row["source"]
            if name == "abs" and path == "vendor/include/psyq/libc.h":
                decision = "function_declaration_not_macro_expansion"
            elif name == "abs":
                decision = "ABS_H_macro_trial_changed_exact_listing"
            elif name in {"va_start", "va_arg", "va_end", "__va_rounded_size"}:
                decision = "source_local_stdarg_macro"
            elif name == "WAIT_TIME" and path == "src/lib/movie_stream_state.inc":
                decision = "source_local_macro_shadow"
            else:
                decision = "macro_or_SDK_constant_present_as_spelled"
            writer.writerow((path, row["line"], name, row["form"], decision))
    with (args.output / "field_site_decisions.tsv").open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source", "line", "field", "candidate_family", "decision"))
        for row in fields:
            path, line, field = row["source"], int(row["line"]), row["field"]
            if field in {"vx", "vy", "vz"}:
                trial = next((vector_starts[(path, line - offset)]
                              for offset in (0, 1, 2)
                              if (path, line - offset) in vector_starts), None)
                family = "setVector/copyVector/addVector"
                if trial:
                    decision = "isolated_trial_" + trial["macro"]
                elif path.endswith("world_translate.c"):
                    decision = "scalar_component_increments_no_vector_operand"
                elif path.endswith("effect_scatter.c") and 49 <= line <= 59:
                    decision = "subtraction_not_addVector"
                else:
                    decision = "partial_reordered_or_interleaved_vector_writes"
            elif field == "code":
                family = "setcode/setPoly"
                decision = "full_listing_identical_trial" if (path, line) in packet_lines else "packet_code_review"
            elif field in {"x0", "x1", "x2", "x3", "y0", "y1", "y2", "y3",
                           "u0", "u1", "u2", "u3", "v0", "v1", "v2", "v3"}:
                family = "setXY/setUV"
                if path.endswith("tmd_pipeline.c"):
                    decision = "packed_word_or_halfword_transfer"
                elif path.endswith("notification_quad.c"):
                    decision = "trial_changed_exact_listing"
                elif path.endswith("menu_transition.c"):
                    decision = "single_site_trials_unchanged_combined_trial_changed"
                else:
                    decision = "quad_trial_nonexact_or_store_order_differs"
            elif field in {"clut", "tpage"}:
                family = "setClut/setTPage"
                if (field == "clut" and path.endswith("menu_render_primitives.c")
                        and 418 <= line < 495):
                    decision = "getClut_trial_changed_exact_listing"
                else:
                    decision = "direct_value_or_packed_copy_not_GetClut_or_GetTPage_call"
            elif field in {"x", "y", "w", "h"}:
                family = "setRECT/setWH"
                if field in {"w", "h"} and path.endswith((
                        "floor_item_find_free.c", "menu_display_state.c")) and line in {36, 37, 38, 39}:
                    decision = "setWH_full_listing_identical_trial"
                elif path.endswith("open/main.c"):
                    decision = "setRECT_trial_changed_exact_listing"
                else:
                    decision = "no_contiguous_four_field_RECT_shape_or_partial_update"
            else:
                family = "primitive pad"
                decision = "non_primitive_field"
            writer.writerow((path, line, field, family, decision))
    print(f"{len(definitions)} definition rows, {len(files)} file rows, "
          f"{len(uses)} name rows, and {len(fields)} field rows classified")


if __name__ == "__main__":
    main()
