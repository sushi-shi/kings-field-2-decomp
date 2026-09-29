"""Run the complete GAME retail/C/Rust resource-codec comparison census.

Every family runs its full shipped corpus and synthetic controls. There is no
case/event limit in this aggregate entry point. Results retain the explicit
provider and I/O boundaries documented by each constituent oracle.
"""

from __future__ import annotations

import argparse
from importlib import import_module
from pathlib import Path
from typing import Sequence

from scripts.kf.rust_codec import build_driver


ORACLES = ("sector_archive",)


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--no-rebuild", action="store_true", help="explicitly reuse existing C objects"
    )
    parser.add_argument("--retail-dir", type=Path, help="extracted disc directory")
    args = parser.parse_args(argv)
    driver = build_driver()
    forwarded = ["--rust-driver", str(driver)]
    if args.no_rebuild:
        forwarded.append("--no-rebuild")
    if args.retail_dir:
        forwarded += ["--retail-dir", str(args.retail_dir)]
    for name in ORACLES:
        print(f"[codec-oracle] checking {name}", flush=True)
        result = import_module(f"scripts.kf.{name}_oracle").main(list(forwarded))
        if result != 0:
            print(f"[codec-oracle] FAIL: {name}", flush=True)
            return 1
    print(f"[codec-oracle] PASS: all {len(ORACLES)} complete comparison suites", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
