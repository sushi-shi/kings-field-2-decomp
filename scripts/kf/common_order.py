"""Audit curated COMMON names against the retail exported-BSS (XBSS) order.

PSYLINK allocates exported COMMON requests after every fixed .bss
contribution, walking its 512 symbol-hash buckets in increasing order. The
bucket of a name is ``(len(name) + sum(name bytes)) & 511``; within one bucket
the most recently interned symbol comes first. The decoded routine and its
full-CPE controls are recorded in the sibling King's Field project
(``docs/patterns/common-allocation-fidelity.md``), and every exported Psy-Q
global in all three King's Field II overlays sits at a non-decreasing bucket.

A game COMMON object therefore reaches its retail address only when its source
spelling lands in the bucket interval its retail SDK neighbours bound. This
report states which curated identities do not. It proposes no names: the
interval is evidence about the original spelling, not a name.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from scripts.kf.paths import RETAIL_CONFIG
from scripts.kf.retail import read_tsv


def bucket(name: str) -> int:
    """PSYLINK's case-sensitive symbol-table bucket for ``name``."""
    encoded = name.encode("latin-1")
    return (len(encoded) + sum(encoded)) & 511


@dataclass(frozen=True)
class Common:
    va: int
    size: int
    name: str
    owner: str

    @property
    def sdk(self) -> bool:
        return self.owner.startswith("LIB")


@dataclass(frozen=True)
class Placement:
    common: Common
    lower: int
    upper: int

    @property
    def consistent(self) -> bool:
        return self.lower <= bucket(self.common.name) <= self.upper


def exported_bss(image: str, config: Path = RETAIL_CONFIG) -> list[Common]:
    """Global BSS identities from the first exported SDK request onward."""
    _fields, rows = read_tsv(config / "data_identities.tsv")
    commons = [Common(int(row["va"], 16), int(row["size"], 16), row["name"], row["owner"])
               for row in rows
               if row["image"] == image and row["storage"] == "bss" and row["scope"] == "global"]
    anchors = [common.va for common in commons if common.sdk]
    if not anchors:
        return []
    start = min(anchors)
    return sorted((common for common in commons if common.va >= start), key=lambda c: c.va)


def placements(commons: list[Common]) -> tuple[list[Placement], list[Common]]:
    """Bucket intervals of game requests, and SDK anchors that break the order."""
    anchors = [common for common in commons if common.sdk]
    disordered = [later for earlier, later in zip(anchors, anchors[1:])
                  if bucket(later.name) < bucket(earlier.name)]
    result = []
    for common in commons:
        if common.sdk:
            continue
        lower = max((bucket(a.name) for a in anchors if a.va < common.va), default=0)
        upper = min((bucket(a.name) for a in anchors if a.va > common.va), default=511)
        result.append(Placement(common, lower, upper))
    return result, disordered


def report(images: tuple[str, ...]) -> int:
    failures = 0
    for image in images:
        commons = exported_bss(image)
        rows, disordered = placements(commons)
        for anchor in disordered:
            print(f"{image}: SDK request {anchor.name}@{anchor.va:#x} breaks the bucket order")
        bad = [row for row in rows if not row.consistent]
        for row in bad:
            common = row.common
            print(f"{image}: {common.name}@{common.va:#x} ({common.owner}) bucket "
                  f"{bucket(common.name)} outside retail interval [{row.lower}, {row.upper}]")
        print(f"[common-order] {image}: {len(rows) - len(bad)}/{len(rows)} game COMMON names "
              f"fit their retail bucket interval; {len(disordered)} SDK order breaks")
        failures += len(bad) + len(disordered)
    return 1 if failures else 0
