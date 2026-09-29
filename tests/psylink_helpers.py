"""Shared helpers for the native PSYLINK smoke tests."""

from __future__ import annotations

import struct
import subprocess
from pathlib import Path


def run(arguments: list[str], *, cwd: Path, env: dict[str, str] | None = None) -> None:
    process = subprocess.run(
        arguments,
        cwd=cwd,
        env=env,
        capture_output=True,
        text=True,
        check=False,
        timeout=30,
    )
    if process.returncode:
        raise RuntimeError(
            f"command failed ({process.returncode}): {' '.join(arguments)}\n"
            f"{process.stderr or process.stdout}"
        )


def symbol_address(path: Path, name: str, *, case_sensitive: bool = False) -> int:
    data = path.read_bytes()
    encoded = (name if case_sensitive else name.upper()).encode("ascii")
    marker = bytes((2, len(encoded))) + encoded
    offsets = []
    cursor = 0
    while (offset := data.find(marker, cursor)) >= 0:
        if offset >= 4:
            offsets.append(struct.unpack_from("<I", data, offset - 4)[0])
        cursor = offset + len(marker)
    if len(offsets) != 1:
        raise RuntimeError(f"{path}: expected one {name} symbol, found {offsets}")
    return offsets[0]


def assert_link_succeeded(path: Path) -> None:
    text = path.read_text(encoding="ascii", errors="replace")
    if "Linking completed." not in text or "0 error(s)" not in text:
        raise RuntimeError(f"{path}: PSYLINK did not succeed:\n{text}")
