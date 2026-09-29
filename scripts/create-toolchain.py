#!/usr/bin/env python3
"""Stage and verify the hash-pinned Psy-Q 3.0 (1995-04-08) SDK media."""

from __future__ import annotations

import argparse
import hashlib
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


PROJECT_DIR = Path(
    os.environ.get("KINGS_FIELD_DIR", Path(__file__).resolve().parent.parent)
).resolve()

# King's Field II (Japan) release date, used to normalize staged metadata.
RELEASE_EPOCH = 806284800


@dataclass(frozen=True)
class Medium:
    env: str
    filename: str
    sha256: str
    url: str


MEDIA = {
    "psyq-3.0": Medium(
        env="PSYQ_KIT_ZIP",
        filename="psx.zip",
        sha256="416241637cdb0273b6bd936b0ab5627c8ba7225ee2b98066cf3b50fb09e57899",
        url="https://archive.org/download/psyq_psx_toolchain_april_08_1994/psx.zip",
    ),
}

# These gates bind the staged SDK to the exact files used by the attribution
# investigation.  The five Sony RCS revisions embedded in the retail JP/US
# executables (intr.c 1.52, pad.c 1.33, vsync.c 1.7, sys.c 1.67, s_crwa.c 1.8)
# occur in these libraries.  That proves the runtime-library snapshot, not the
# compiler, assembler or linker that FromSoftware used.
EXPECTED_KIT_FILES = {
    "BIN/CCPSX.EXE": "3520b27140e7a2c7d82580190f06e25269fe04c50ca9c6187fad268e6f8b6435",
    "BIN/ASPSX.EXE": "e368c1032ee1ea51bb55e55abdffbf7cb86852b89ba72360266c13b4327e2623",
    "BIN/PSYLINK.EXE": "c50518655cd965bfc899098563e34ba7447a43a43cec64bae1104dae4b314bdd",
    "BIN/PSYLIB.EXE": "fdae3dea7cfd0891a4862e845398a7b028a51cede0d0f0f852d269e8a877bd98",
    "BIN/ASMPSX.EXE": "8804da268575232018071e15287124ab8891b3cbf070b2b316c276de6d06b445",
    "BIN/CPE2X.EXE": "8ee3df02d30d9269bba8c570d69f3c9d2b59aff98af0fbf796367526bc02ef20",
    "COMPILER/CC1PSX": "d164b281bd6c815abb06a38ca21f1a4f75d5c6a63c2b3a41128def58afee9548",
    "COMPILER/CPPPSX": "38687a01d58dcd3c373d7487e50d1c634cd3cf6c3137f3aa92304aa05bdf17e1",
    "COMPILER/CC1PSX.EXE": "e65635ec539f2b9c6db6f2b350c1fd300a26fdd0a264b3f2defefa9c2d0d6dd3",
    "COMPILER/CPPPSX.EXE": "97b42264dffafcf15ca3e6348d49a537f13d958d2d66292eeb1a1c155fe0c217",
    "LIB/2MBYTE.OBJ": "b4ed8b2e76bc841f7a81132906d846877aefa8f6233fb738600c8b1ebd032a46",
    "LIB/8MBYTE.OBJ": "0c0ccb91c6d059a88b5ec5ac8f34c65a717c636a911247c31faa0df4ae2e599e",
    "LIB/CARD.OBJ": "4ab0873cddbf26d99aded93f8b654c861f44409cee408b3bff7026fc59665387",
    "LIB/LIBAPI.LIB": "0bf086cba5646d7634b1037b6ff449391cc7e5259796a10bf7e849a9f68da3fd",
    "LIB/LIBC.LIB": "f75c0938d65467879232bbad3389687fb4a4faecb5ec9605e3c8d7c4d9133145",
    "LIB/LIBC2.LIB": "892ee2d9cdf7caf02cf2b6df4aeabacdb7e0e6c7159b32f4225b7ffb2b503ac2",
    "LIB/LIBCARD.LIB": "c2d0d1d4cde7ed2d9d88124438ad65b276cccc7de643df241413b0bcd8888ec7",
    "LIB/LIBCD.LIB": "22437a70d950830553e7439f42cfe08deaeded0c82dcddbc0614821a509a31df",
    "LIB/LIBETC.LIB": "a42c2c758275a2c4b35a629494a531c6f9be79ac5e2da01ecdf8c4e642cce26b",
    "LIB/LIBGPU.LIB": "198a165ddc6ef4c01ad9567e37998730b51ed5411aba5939a86a54dbe9d8996d",
    "LIB/LIBGS.LIB": "c7dde3f379b80de7d8088f91cda2d02801628a99fc712c98e0ba3c70eb0b9004",
    "LIB/LIBGTE.LIB": "cea7ea666904a237d9d925b57ca2223696c86a2938c1dccec5c91c9779c4fbb7",
    "LIB/LIBMATH.LIB": "65ce94ccbda45e579e70124c7098c4b3c95c05e0c804494e5462cf9b62c77b65",
    "LIB/LIBPRESS.LIB": "5267abd8f2f7e0eb1701954b21274f5df8b966c5d9caa5187ebb4c2b2800014c",
    "LIB/LIBSN.LIB": "46e8f89dadf8abfc24466f153ce90e10820898d14069d732156668e9cc6f3b69",
    "LIB/LIBSND.LIB": "85c9c5d15226f689e6bd00002911e2aad6a1064c0dc7d5ce6b69dcb554cd09d4",
    "LIB/LIBSPU.LIB": "41c47817707f0207390e7ca5c9750aca1c9c543981a5ad5ed717e7c92e278bd5",
    "LIB/NONE.OBJ": "8ba3c04a73aca73f4fffea12c2ff5bdf18286919ddf1ce0e81af5f4f3c4756dc",
    "LIB/NONE2.OBJ": "38662381b57cdbebb8d083842fd23914bad1d4c66303beb871f2f5248c38774c",
}

TOOL_VERSION_MARKERS = {
    "CCPSX.EXE": "CCPSX version 1.10",
    # Spelling/case follows the strings embedded in the original binaries.
    "PSYLINK.EXE": "PSYLINK version 1.29",
    "PSYLIB.EXE": "PsyLib version 1.04",
    "ASMPSX.EXE": "Psy-Q PSX version 1.21",
    "ASPSX.EXE": "Psy-Q ASPSX version 2.08",
    "CPE2X.EXE": "CPE2X Ver1.3",
}


def log(message: str) -> None:
    print(f"[sdk] {message}", flush=True)


def run(command: list[str], **kwargs: object) -> subprocess.CompletedProcess[bytes]:
    log("+ " + " ".join(command))
    return subprocess.run(command, check=True, **kwargs)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def verify_hash(path: Path, expected: str, label: str) -> None:
    actual = sha256_file(path)
    if actual != expected:
        raise RuntimeError(
            f"{label} hash mismatch: expected {expected}, got {actual} ({path})"
        )


def resolve_casefold(root: Path, relative: str) -> Path:
    """Resolve an archive member without assuming host case semantics."""

    current = root
    for part in Path(relative).parts:
        if not current.is_dir():
            raise FileNotFoundError(relative)
        candidates = [
            child for child in current.iterdir() if child.name.casefold() == part.casefold()
        ]
        if len(candidates) != 1:
            raise FileNotFoundError(
                f"expected one case-insensitive match for {part!r} below {current}, "
                f"found {len(candidates)}"
            )
        current = candidates[0]
    return current


def find_kit_root(extracted: Path) -> Path:
    """Find the directory containing the kit's `BIN`, `COMPILER` and `LIB`."""

    candidates = [extracted, *sorted(p for p in extracted.rglob("*") if p.is_dir())]
    for candidate in candidates:
        children = {p.name.casefold() for p in candidate.iterdir()} if candidate.is_dir() else set()
        if {"bin", "compiler", "lib", "include"} <= children:
            return candidate
    raise RuntimeError("Psy-Q 3.0 root (BIN + COMPILER + LIB + INCLUDE) was not found")


def extract_archive(source: Path, destination: Path) -> None:
    destination.mkdir(parents=True, exist_ok=True)
    if source.suffix.casefold() == ".rar":
        run(
            [
                "unar",
                "-quiet",
                "-force-overwrite",
                "-output-directory",
                str(destination),
                str(source),
            ]
        )
        return
    run(
        ["7z", "x", "-y", f"-o{destination}", str(source)],
        stdout=subprocess.DEVNULL,
    )


def copy_tree(source: Path, destination: Path) -> None:
    if not source.is_dir():
        raise RuntimeError(f"required directory is absent: {source}")
    shutil.copytree(source, destination, dirs_exist_ok=True)


def verify_kit(root: Path) -> None:
    for relative, digest in EXPECTED_KIT_FILES.items():
        verify_hash(resolve_casefold(root, relative), digest, relative)

    bin_dir = resolve_casefold(root, "BIN")
    for filename, marker in TOOL_VERSION_MARKERS.items():
        data = resolve_casefold(bin_dir, filename).read_bytes()
        if marker.encode("ascii") not in data:
            raise RuntimeError(f"{filename} does not contain version marker {marker!r}")

    for relative, marker in (
        ("COMPILER/CC1PSX", b"2.4.1"),
        ("COMPILER/CPPPSX", b"2.4.1"),
        ("COMPILER/CC1PSX.EXE", b"2.6.0"),
        ("COMPILER/CPPPSX.EXE", b"2.6.0"),
    ):
        if marker not in resolve_casefold(root, relative).read_bytes():
            raise RuntimeError(f"{relative} does not contain compiler marker {marker!r}")


def stage_kit(root: Path, stage: Path) -> None:
    log("staging the complete Psy-Q 3.0 kit tree")
    copy_tree(root, stage / "psyq-3.0")


def stage_include_lf(stage: Path) -> None:
    """Derive LF-terminated copies of the kit headers for the host preprocessor.

    The 3.0 headers use DOS CRLF endings. The DOS toolchain read them natively,
    but the Linux-hosted GCC 2.5.7 preprocessor treats a backslash followed by
    a carriage return as no line continuation, which breaks multi-line SDK
    macros. Only line endings change; `psyq-3.0/INCLUDE` stays byte-identical.
    """
    source = resolve_casefold(stage / "psyq-3.0", "INCLUDE")
    target = stage / "include-lf"
    for path in sorted(p for p in source.rglob("*") if p.is_file()):
        destination = target / path.relative_to(source)
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(path.read_bytes().replace(b"\r\n", b"\n"))


def write_attribution(stage: Path) -> None:
    text = """# Psy-Q 3.0 SDK baseline

`psyq-3.0/` is the complete `psx/` tree of the one hash-pinned Psy-Q PS-X
Development System Release 3.0 kit (Archive.org
`psyq_psx_toolchain_april_08_1994/psx.zip`; files dated 1995-04-08 despite the
item title). It is preserved as a coherent SDK baseline rather than being
combined with tools or files from other distributions.

Its libraries contain exactly the five Sony RCS revisions embedded in the
retail King's Field II JP/US executables. That proves the runtime-library
snapshot only.

The kit contains CCPSX 1.10, PSYLINK 1.29, PSYLIB 1.04, ASMPSX 1.21, ASPSX
2.08, CPE2X 1.3 and a compiler directory. That directory holds the extensionless
GCC 2.4.1 pair and the `.EXE` GCC 2.6.0 pair, byte-identical to the Psy-Q
Release 2.5 pair used by the King's Field (SLPS-00017) project.

`include-lf/` is derived from `psyq-3.0/INCLUDE` with CRLF line endings
converted to LF for the Linux-hosted preprocessor; no other byte changes.

The build supplies its native C compiler and replacement assemblers separately.
The original SDK binaries remain proprietary; this output must not be committed
to Git.
"""
    (stage / "ATTRIBUTION.md").write_text(text, encoding="utf-8", newline="\n")

    rows = ["id\tenvironment\tsha256\turl"]
    for identifier, medium in MEDIA.items():
        rows.append(f"{identifier}\t{medium.env}\t{medium.sha256}\t{medium.url}")
    (stage / "SOURCE_MEDIA.tsv").write_text("\n".join(rows) + "\n", encoding="utf-8")


def normalize_stage(stage: Path) -> None:
    for path in sorted(stage.rglob("*")):
        if path.is_symlink():
            raise RuntimeError(f"staged SDK must not contain symlinks: {path}")
        mode = 0o755 if path.is_dir() else 0o644
        path.chmod(mode)
        os.utime(path, (RELEASE_EPOCH, RELEASE_EPOCH), follow_symlinks=False)
    stage.chmod(0o755)
    os.utime(stage, (RELEASE_EPOCH, RELEASE_EPOCH), follow_symlinks=False)


def write_manifest(stage: Path) -> None:
    manifest = stage / "MANIFEST.tsv"
    rows = ["path\tbytes\tsha256"]
    for path in sorted(p for p in stage.rglob("*") if p.is_file() and p != manifest):
        relative = path.relative_to(stage).as_posix()
        rows.append(f"{relative}\t{path.stat().st_size}\t{sha256_file(path)}")
    manifest.write_text("\n".join(rows) + "\n", encoding="utf-8", newline="\n")


def medium_from_environment(medium: Medium) -> Path:
    value = os.environ.get(medium.env)
    if not value:
        raise RuntimeError(
            f"{medium.env} is unset; use `nix develop` or set the path to the "
            "pinned source medium"
        )
    path = Path(value)
    if not path.is_file():
        raise RuntimeError(f"{medium.env} does not name a file: {path}")
    verify_hash(path, medium.sha256, medium.filename)
    return path


def build(stage: Path, work: Path) -> None:
    kit = medium_from_environment(MEDIA["psyq-3.0"])

    kit_extract = work / "psyq-3.0"
    extract_archive(kit, kit_extract)

    kit_root = find_kit_root(kit_extract)
    verify_kit(kit_root)
    stage_kit(kit_root, stage)
    stage_include_lf(stage)
    write_attribution(stage)
    write_manifest(stage)
    normalize_stage(stage)


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--stage-dir",
        type=Path,
        default=PROJECT_DIR / "build" / "sdk",
        help="stage directly here (default: build/sdk)",
    )
    parser.add_argument("--work-dir", type=Path, help="temporary extraction directory")
    parser.add_argument("--keep-work", action="store_true", help="retain temporary extraction files")
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(sys.argv[1:] if argv is None else argv)
    owned_work = args.work_dir is None
    work = (
        Path(tempfile.mkdtemp(prefix="kings-field-sdk-"))
        if owned_work
        else args.work_dir.resolve()
    )
    stage = args.stage_dir.resolve()

    if stage.exists() and any(stage.iterdir()):
        raise RuntimeError(f"refusing to merge into non-empty stage directory: {stage}")
    stage.mkdir(parents=True, exist_ok=True)
    work.mkdir(parents=True, exist_ok=True)

    log(f"work:  {work}")
    log(f"stage: {stage}")
    try:
        build(stage, work)
        log("SDK initialization complete")
    finally:
        if owned_work and not args.keep_work:
            shutil.rmtree(work, ignore_errors=True)
        elif args.keep_work:
            log(f"kept work directory: {work}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
