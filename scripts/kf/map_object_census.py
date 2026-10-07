"""Census of GAME map-object templates, models, placements and actor flags.

The layouts follow the reconstructed GAME loaders:

- `resource_initialize_game_assets` copies `FDAT.T` entry 48 chunk 0 into
  `map_object_state.templates` (320 rows of 24 bytes, byte 0 the operation);
- `resource_advance_transition` reads map region `r` from `FDAT.T` entries
  `3r` (cells), `3r + 1` (40 target groups + candidate blob, 200 actor load
  records, 350 placements, placed sources) and `3r + 2` (the map callback
  table, 32 pointers into code linked at `0x8019e138`);
- `render_scene_and_update_resources` streams object `n`'s model from `MO.T`
  entry `0x80 + n` (a `KfAssetHeader` whose `tmd_data_offset` locates a TMD);
- placement bytes 16..23 are copied to object bytes `0x38..0x3f`.

TMD mode bytes are GPU primitive codes: bit 2 textured, bit 1
semi-transparent, bit 4 Gouraud, bit 3 quad; flag bit 0 disables lighting.
The callback scan only reports which `ori/addiu $rt, $zero, imm` operation
immediates occur in callback slots 8 (placement init) and 9 (update) up to
their first `jr $ra`; it does not interpret the code.
"""

from __future__ import annotations

import argparse
import json
from collections import Counter
from dataclasses import dataclass, field
from pathlib import Path
import struct
from typing import Sequence

SECTOR = 2048
TEMPLATE_ENTRY = 48
TEMPLATE_COUNT = 320
TEMPLATE_BYTES = 24
PLACEMENT_BYTES = 24
GROUP_COUNT = 40
GROUP_BYTES = 0x78
GROUP_FLAGS_OFFSET = 0x34
ACTOR_RECORD_BYTES = 16
MODEL_FIRST_ENTRY = 0x80
CALLBACK_BASE = 0x8019E138
CALLBACK_SLOTS = 32
PLACEMENT_NONE = 0xFFFF
JR_RA = 0x03E00008
TMD_KINDS = {0x20: "F3", 0x24: "FT3", 0x28: "F4", 0x2C: "FT4",
             0x30: "G3", 0x34: "GT3", 0x38: "G4", 0x3C: "GT4"}


def read_archive(data: bytes) -> list[bytes]:
    """Split a `CD/COM/*.T` sector archive into its entries."""
    count = struct.unpack_from("<H", data)[0]
    offsets = struct.unpack_from(f"<{count + 1}H", data, 2)
    return [data[offsets[i] * SECTOR:offsets[i + 1] * SECTOR] for i in range(count)]


def read_chunks(entry: bytes, count: int) -> list[bytes]:
    """Return the first `count` u32-length-prefixed chunks (short if truncated)."""
    out: list[bytes] = []
    position = 0
    while len(out) < count and position + 4 <= len(entry):
        size = struct.unpack_from("<I", entry, position)[0]
        if size > len(entry) - position - 4:
            break
        out.append(entry[position + 4:position + 4 + size])
        position += 4 + size
    return out


@dataclass
class ModelSummary:
    present: bool
    byte_size: int = 0
    objects: int = 0
    vertices: int = 0
    primitives: dict[str, int] = field(default_factory=dict)
    textured: int = 0
    untextured: int = 0
    semi_transparent: int = 0
    gouraud: int = 0
    unlit: int = 0
    texture_pages: dict[int, int] = field(default_factory=dict)


def summarize_model(asset: bytes) -> ModelSummary:
    """Count primitive kinds of the TMD inside one MO asset."""
    if len(asset) < 12:
        return ModelSummary(present=False)
    byte_size, _animation, tmd_offset = struct.unpack_from("<3I", asset)
    if byte_size < 5 or tmd_offset + 12 > len(asset):
        return ModelSummary(present=False, byte_size=byte_size)
    summary = ModelSummary(present=True, byte_size=byte_size)
    _tmd_id, _flags, summary.objects = struct.unpack_from("<3I", asset, tmd_offset)
    kinds: Counter[str] = Counter()
    pages: Counter[int] = Counter()
    for index in range(summary.objects):
        record = struct.unpack_from("<6Ii", asset, tmd_offset + 12 + index * 28)
        summary.vertices += record[1]
        packet = tmd_offset + 12 + record[4]
        for _ in range(record[5]):
            _olen, ilen, flag, mode = struct.unpack_from("<4B", asset, packet)
            kinds[TMD_KINDS.get(mode & 0xFD, f"mode_{mode:02x}")] += 1
            if mode & 0x04:
                summary.textured += 1
                pages[struct.unpack_from("<H", asset, packet + 10)[0]] += 1
            else:
                summary.untextured += 1
            summary.semi_transparent += bool(mode & 0x02)
            summary.gouraud += bool(mode & 0x10)
            summary.unlit += bool(flag & 0x01)
            packet += 4 + ilen * 4
    summary.primitives = dict(sorted(kinds.items()))
    summary.texture_pages = dict(sorted(pages.items()))
    return summary


@dataclass(frozen=True)
class Template:
    object_id: int
    operation: int
    kind: int
    vab_resource_index: int
    collision_flags: int
    collision_radius: int
    interaction_radius: int
    interaction_height: int
    initial_render_depth_offset: int
    params: bytes


def parse_templates(fdat: Sequence[bytes]) -> list[Template]:
    table = read_chunks(fdat[TEMPLATE_ENTRY], 1)[0]
    if len(table) != TEMPLATE_COUNT * TEMPLATE_BYTES:
        raise ValueError(f"template chunk is {len(table)} bytes")
    templates = []
    for object_id in range(TEMPLATE_COUNT):
        row = table[object_id * TEMPLATE_BYTES:(object_id + 1) * TEMPLATE_BYTES]
        fields = struct.unpack_from("<4B3Hh", row)
        templates.append(Template(object_id, *fields, params=row[12:]))
    return templates


@dataclass(frozen=True)
class Placement:
    region: int
    row: int
    layer_mask: int
    cell_x: int
    cell_z: int
    rotation_y: int
    height: int
    object_bytes_38: bytes


def map_regions(fdat: Sequence[bytes]) -> list[int]:
    return [r for r in range(len(fdat) // 3) if r * 3 + 1 < TEMPLATE_ENTRY and fdat[r * 3 + 1]]


def parse_placements(fdat: Sequence[bytes], region: int) -> dict[int, list[Placement]]:
    rows = read_chunks(fdat[region * 3 + 1], 3)[2]
    found: dict[int, list[Placement]] = {}
    for row in range(len(rows) // PLACEMENT_BYTES):
        base = row * PLACEMENT_BYTES
        layer, cell_z, cell_x, _pad, object_id, rotation, _lz, _lx, height = struct.unpack_from(
            "<4BH4h", rows, base)
        if object_id == PLACEMENT_NONE:
            continue
        found.setdefault(object_id, []).append(Placement(
            region, row, layer, cell_x, cell_z, rotation, height, rows[base + 16:base + 24]))
    return found


def callback_table(code: bytes) -> tuple[int, ...]:
    return struct.unpack_from(f"<{CALLBACK_SLOTS}I", code)


def callback_operation_immediates(code: bytes, entry: int, operations: set[int]) -> list[int]:
    """Operation immediates loaded from $zero before the callback's first `jr $ra`."""
    offset = entry - CALLBACK_BASE
    if not 0 <= offset < len(code):
        return []
    seen: list[int] = []
    while offset + 8 <= len(code):
        word = struct.unpack_from("<I", code, offset)[0]
        opcode, rs, immediate = word >> 26, (word >> 21) & 31, word & 0xFFFF
        if opcode in (0x09, 0x0D) and rs == 0 and immediate in operations and immediate not in seen:
            seen.append(immediate)
        if word == JR_RA:
            break
        offset += 4
    return seen


def is_noop(code: bytes, entry: int) -> bool:
    offset = entry - CALLBACK_BASE
    return 0 <= offset <= len(code) - 8 and code[offset:offset + 8] == struct.pack("<2I", JR_RA, 0)


def actor_flag_groups(fdat: Sequence[bytes], region: int, mask: int) -> list[dict]:
    chunks = read_chunks(fdat[region * 3 + 1], 2)
    groups, records = chunks[0], chunks[1]
    placed: Counter[int] = Counter()
    parents: dict[int, Counter[tuple[int, int]]] = {}
    for index in range(len(records) // ACTOR_RECORD_BYTES):
        base = index * ACTOR_RECORD_BYTES
        slot, group = records[base], records[base + 1]
        if slot == 0xFF:
            continue
        placed[group] += 1
        linked = struct.unpack_from("<H", records, base + 10)[0]
        if linked < len(records) // ACTOR_RECORD_BYTES:
            parent_group = records[linked * ACTOR_RECORD_BYTES + 1]
            parents.setdefault(group, Counter())[(parent_group, groups[parent_group * GROUP_BYTES])] += 1
    found = []
    for group in range(GROUP_COUNT):
        row = groups[group * GROUP_BYTES:(group + 1) * GROUP_BYTES]
        flags = struct.unpack_from("<I", row, GROUP_FLAGS_OFFSET)[0]
        if row[0] != 0xFF and flags & mask:
            found.append({
                "region": region, "group": group, "definition": row[0], "flags": flags,
                "placed": placed[group],
                "linked_parents": [{"group": g, "definition": d, "count": n}
                                   for (g, d), n in sorted(parents.get(group, Counter()).items())],
            })
    return found


def census(disc_dir: Path, operations: Sequence[int], actor_mask: int | None) -> dict:
    com = disc_dir / "CD/COM"
    fdat = read_archive((com / "FDAT.T").read_bytes())
    models = read_archive((com / "MO.T").read_bytes())
    templates = parse_templates(fdat)
    regions = map_regions(fdat)
    placements: dict[int, list[Placement]] = {}
    for region in regions:
        for object_id, rows in parse_placements(fdat, region).items():
            placements.setdefault(object_id, []).extend(rows)
    wanted = set(operations)
    callbacks = {}
    for region in regions:
        code = fdat[region * 3 + 2]
        table = callback_table(code)
        callbacks[region] = {
            "table_in_workspace": all(0 <= t - CALLBACK_BASE < len(code) for t in table),
            **{f"slot_{slot}": {
                "entry": f"0x{table[slot]:08x}", "noop": is_noop(code, table[slot]),
                "operation_immediates": callback_operation_immediates(code, table[slot], wanted)}
               for slot in (8, 9)},
        }
    report: dict = {"operations": {}, "callbacks": callbacks}
    for operation in operations:
        rows = []
        for template in templates:
            if template.operation != operation:
                continue
            placed = placements.get(template.object_id, [])
            rows.append({
                "object_id": template.object_id,
                "template": {
                    "kind": template.kind, "vab_resource_index": template.vab_resource_index,
                    "collision_flags": template.collision_flags,
                    "collision_radius": template.collision_radius,
                    "interaction_radius": template.interaction_radius,
                    "interaction_height": template.interaction_height,
                    "initial_render_depth_offset": template.initial_render_depth_offset,
                    "params": template.params.hex(),
                },
                "model": summarize_model(models[MODEL_FIRST_ENTRY + template.object_id]).__dict__,
                "placements": [{
                    "region": p.region, "row": p.row, "layer_mask": p.layer_mask,
                    "cell": [p.cell_x, p.cell_z], "rotation_y": p.rotation_y, "height": p.height,
                    "object_bytes_38": p.object_bytes_38.hex()} for p in placed],
            })
        report["operations"][operation] = rows
    if actor_mask is not None:
        report["actor_flag_groups"] = [row for region in regions
                                       for row in actor_flag_groups(fdat, region, actor_mask)]
    return report


def render_text(report: dict) -> str:
    lines = []
    for operation, rows in report["operations"].items():
        lines.append(f"operation {operation}: {len(rows)} template(s)")
        for row in rows:
            t, m = row["template"], row["model"]
            lines.append(
                f"  object {row['object_id']}: collision_radius {t['collision_radius']} "
                f"interaction {t['interaction_radius']}x{t['interaction_height']} "
                f"flags 0x{t['collision_flags']:02x} vab {t['vab_resource_index']} "
                f"depth {t['initial_render_depth_offset']} params {t['params']}")
            if m["present"]:
                lines.append(
                    f"    model: {m['objects']} object(s), {m['vertices']} vertices, "
                    f"{m['primitives']}, textured {m['textured']}, untextured {m['untextured']}, "
                    f"semi-transparent {m['semi_transparent']}, unlit {m['unlit']}")
            else:
                lines.append("    model: none")
            regions = Counter(p["region"] for p in row["placements"])
            layers = Counter(p["layer_mask"] for p in row["placements"])
            lines.append(f"    placements: {len(row['placements'])} by region {dict(regions)} "
                         f"layer masks {dict(layers)}")
            for p in row["placements"]:
                lines.append(f"      region {p['region']} row {p['row']} layer {p['layer_mask']} "
                             f"cell {p['cell']} yaw {p['rotation_y']} height {p['height']} "
                             f"bytes38 {p['object_bytes_38']}")
    lines.append("map callbacks (slot: operation immediates before the first jr $ra):")
    for region, slots in report["callbacks"].items():
        parts = [f"{name} {info['entry']}{' noop' if info['noop'] else ''} {info['operation_immediates']}"
                 for name, info in slots.items() if name.startswith("slot_")]
        stale = "" if slots["table_in_workspace"] else " (table outside the 0x8019e138 workspace)"
        lines.append(f"  region {region}{stale}: " + "; ".join(parts))
    for row in report.get("actor_flag_groups", []):
        lines.append(f"actor group: region {row['region']} group {row['group']} definition "
                     f"{row['definition']} flags 0x{row['flags']:x} placed {row['placed']} "
                     f"linked parents {row['linked_parents']}")
    return "\n".join(lines)


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--disc-dir", type=Path,
                        help="extracted SLPS-00069 disc directory holding CD/COM "
                             "(default: the configured retail directory)")
    parser.add_argument("--operations", default="0,11,15,17,33,48,80,81,95,160,161,162,163,164,165",
                        help="comma-separated template operations to report")
    parser.add_argument("--actor-flag", type=lambda value: int(value, 0),
                        help="also list target groups whose initial_actor_flags have this mask")
    parser.add_argument("--json", action="store_true", help="emit JSON instead of text")
    args = parser.parse_args(argv)
    disc_dir = args.disc_dir
    if disc_dir is None:
        from scripts.kf.local_config import configured_retail_dir
        disc_dir = configured_retail_dir(None)
    if not (disc_dir / "CD/COM").is_dir():
        raise SystemExit(f"{disc_dir}/CD/COM: missing; pass --disc-dir with the extracted disc")
    operations = [int(value, 0) for value in args.operations.split(",") if value]
    report = census(disc_dir, operations, args.actor_flag)
    print(json.dumps(report, indent=1) if args.json else render_text(report))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
