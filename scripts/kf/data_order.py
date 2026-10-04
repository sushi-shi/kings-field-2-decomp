"""Audit whether claimed initialized sections can belong to the named units.

An object contributes one ordered run to each output section. This checks for
*known* claims from another unit inside that run. Unclaimed bytes still need
independent section/ownership evidence before a TU boundary is proven.
"""

from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass

from scripts.kf.manifest import Manifest
from scripts.kf.paths import RETAIL_CONFIG
from scripts.kf.retail import read_tsv

SECTION_ORDER = {section: rank for rank, section in enumerate(
    (".rdata", ".text", ".data", ".sdata", ".sbss", ".bss")
)}


@dataclass(frozen=True)
class Claim:
    image: str
    section: str
    start: int
    end: int
    unit: str
    name: str


@dataclass(frozen=True)
class Interleaving:
    image: str
    section: str
    unit: str
    before: Claim
    intruder: Claim
    after: Claim


@dataclass(frozen=True)
class Overlap:
    first: Claim
    second: Claim


@dataclass(frozen=True)
class SectionConflict:
    image: str
    section: str
    unit: str
    symbol: str
    va: int
    boundary: int
    expected: str


def _initialized_claims(manifest: Manifest) -> dict[tuple[str, str], list[Claim]]:
    sections: dict[tuple[str, str], list[Claim]] = defaultdict(list)
    for unit in manifest.units:
        for datum in unit.data:
            if datum.section_name in {".data", ".sdata"}:
                sections[(unit.image, datum.section_name)].append(
                    Claim(unit.image, datum.section_name, datum.va, datum.end,
                          unit.unit, datum.symbol)
                )
        if unit.rodata is not None:
            va, size = unit.rodata
            sections[(unit.image, ".rdata")].append(
                Claim(unit.image, ".rdata", va, va + size, unit.unit, "RODATA")
            )
    for claims in sections.values():
        claims.sort(key=lambda claim: (claim.start, claim.end))
    return sections


def find_overlaps(manifest: Manifest) -> tuple[Overlap, ...]:
    """Find overlapping initialized claims, including anonymous RODATA runs."""
    found: list[Overlap] = []
    for claims in _initialized_claims(manifest).values():
        for index, first in enumerate(claims):
            for second in claims[index + 1:]:
                if second.start >= first.end:
                    break
                found.append(Overlap(first, second))
    return tuple(found)


def find_interleavings(manifest: Manifest) -> tuple[Interleaving, ...]:
    """Find foreign initialized claims between two claims from one unit."""
    sections = _initialized_claims(manifest)
    found: list[Interleaving] = []
    for (image, section), claims in sections.items():
        by_unit: dict[str, list[Claim]] = defaultdict(list)
        for claim in claims:
            by_unit[claim.unit].append(claim)
        for unit, owned in by_unit.items():
            for before, after in zip(owned, owned[1:]):
                for intruder in claims:
                    if intruder.unit != unit and before.end <= intruder.start < after.start:
                        found.append(Interleaving(image, section, unit, before,
                                                  intruder, after))
    return tuple(sorted(found, key=lambda item: (
        item.image, item.section, item.before.start, item.intruder.start,
    )))


def find_section_conflicts(manifest: Manifest) -> tuple[SectionConflict, ...]:
    """Find claims beyond the known side of a retail section-boundary bracket."""
    fields, rows = read_tsv(RETAIL_CONFIG / "section_boundaries.tsv")
    if fields != ("image", "left_section", "right_section", "last_left_end",
                  "first_right_start", "evidence", "status"):
        raise ValueError("section_boundaries.tsv: invalid columns")
    by_image: dict[str, list[tuple[str, str, int, int]]] = defaultdict(list)
    for row in rows:
        lower = int(row["last_left_end"], 0)
        upper = int(row["first_right_start"], 0)
        left, right = row["left_section"], row["right_section"]
        if (lower > upper or row["status"] not in {"observed", "candidate"}
                or left not in SECTION_ORDER or right not in SECTION_ORDER
                or SECTION_ORDER[right] != SECTION_ORDER[left] + 1):
            raise ValueError(f"section_boundaries.tsv: invalid boundary {row!r}")
        by_image[row["image"]].append((left, right, lower, upper))
    found: list[SectionConflict] = []
    for unit in manifest.units:
        for datum in unit.data:
            for left, right, lower, upper in by_image[unit.image]:
                if SECTION_ORDER[datum.section_name] <= SECTION_ORDER[left] and datum.end > upper:
                    found.append(SectionConflict(unit.image, datum.section_name, unit.unit,
                                                 datum.symbol, datum.va, upper, right))
                elif SECTION_ORDER[datum.section_name] >= SECTION_ORDER[right] and datum.va < lower:
                    found.append(SectionConflict(unit.image, datum.section_name, unit.unit,
                                                 datum.symbol, datum.va, lower, left))
    return tuple(sorted(found, key=lambda item: (item.image, item.va)))


def report(manifest: Manifest, images: tuple[str, ...] = ()) -> int:
    overlaps = [item for item in find_overlaps(manifest)
                if not images or item.first.image in images]
    for item in overlaps:
        first, second = item.first, item.second
        print(f"{first.image} {first.section}: overlapping claims "
              f"{first.unit} {first.name}@{first.start:#x}..{first.end:#x} and "
              f"{second.unit} {second.name}@{second.start:#x}..{second.end:#x}")
    conflicts = [item for item in find_interleavings(manifest)
                 if not images or item.image in images]
    groups: dict[tuple[str, str, str, Claim, Claim], list[Claim]] = defaultdict(list)
    for item in conflicts:
        groups[(item.image, item.section, item.unit,
                item.before, item.after)].append(item.intruder)
    for (image, section, unit, before, after), intruders in groups.items():
        first = intruders[0]
        print(f"{image} {section}: {unit} {before.name}@{before.start:#x} -> "
              f"{after.name}@{after.start:#x}: {len(intruders)} foreign claims; "
              f"first {first.unit} {first.name}@{first.start:#x}")
    section_conflicts = [item for item in find_section_conflicts(manifest)
                         if not images or item.image in images]
    for item in section_conflicts:
        print(f"{item.image}: {item.unit} {item.symbol}@{item.va:#x} claims "
              f"{item.section} past {item.expected} boundary bracket "
              f"{item.boundary:#x}")
    print(f"[data-order] {len(overlaps)} overlapping claims; "
          f"{len(groups)} interrupted unit runs; "
          f"{len(conflicts)} known foreign claims; "
          f"{len(section_conflicts)} section-boundary conflicts")
    return 1 if overlaps or conflicts or section_conflicts else 0
