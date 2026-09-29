# SDK and source-to-EXE toolchain

The repository has one active historical SDK: the complete hash-pinned Psy-Q
PS-X Development System Release 3.0 kit (Archive.org
`psyq_psx_toolchain_april_08_1994/psx.zip`; files dated 1995-04-08 despite the
item title). It is staged without files from another SDK, compiler disk, or
assembler archive. Its libraries contain exactly the five Sony RCS revisions
embedded in the retail SLPS-00069 executables (`intr.c` 1.52, `pad.c` 1.33,
`vsync.c` 1.7, `sys.c` 1.67, `s_crwa.c` 1.8). That proves the runtime-library
snapshot, not the compiler, assembler, or linker FromSoftware used. See the
[attribution survey](kf2-toolchain-and-lineage.md).

The kit contains CCPSX 1.10, PSYLINK 1.29, PSYLIB 1.04, ASMPSX 1.21, ASPSX 2.08,
CPE2X 1.3, the general `LIB/` archives and startup objects, and its
`INCLUDE/` headers. `NONE2.OBJ`, `2MBYTE.OBJ` and `8MBYTE.OBJ` are
byte-identical to the Release 2.5 H2000 copies used by the SLPS-00017 project.
`LIB/LIBSN.LIB` is the startup archive; `BIN/LIBSN.LIB` is the larger fileserver
variant.

The kit's `COMPILER/` holds an extensionless GCC 2.4.1 frontend pair and a
`.EXE` GCC 2.6.0 pair, byte-identical to the Release 2.5 pair. Preserving both
is part of preserving this one distribution; neither pair is staged as a second
SDK.

| Environment variable | Psy-Q 3.0 path |
| --- | --- |
| `PSYQ_SDK` | Root of the complete kit tree (`psyq-3.0/`) |
| `PSYQ_BIN` | `BIN` |
| `PSYQ_INCLUDE` | `include-lf/` beside the kit: `INCLUDE` with CRLF converted to LF |
| `PSYQ_LIB` | `LIB` |
| `PSYQ_COMPILER` | `COMPILER` |

The 3.0 headers use DOS CRLF line endings, which the Linux-hosted GCC 2.5.7
preprocessor does not accept inside backslash-continued macros. The stager
therefore derives `include-lf/`, identical except for line endings, and
`PSYQ_INCLUDE` points there; the original `INCLUDE/` stays byte-identical.

The executable chain still uses the separately hash-pinned ASPSX 1.07
(`PSYQ_ASPSX`) inherited from the SLPS-00017 setup, alongside the native GCC
probe selected by each unit's profile. The kit's ASPSX 2.08 carries no
software-key strings but is not yet wired in. PSYLINK, CPE2X, headers,
libraries, and overlay startup come from the preserved kit. Build reports
distinguish those provenances; none of this proves the exact historical tool
versions.

The Psy-Q 3.0 archives are linked unchanged. The SLPS-00017 project's LIBETC
interrupt-return correction is bound to the Release 2.5 archive and to a
runtime failure not reproduced here, so it is not applied.

Classic additionally uses the original GNU GCC 2.5.7 `stdarg.h` and `va-mips.h`,
packaged separately from the Sony SDK. `PSYQ_C_INCLUDE` points to their Nix
store directory. `compile_classic` supplies `__GNUC__=2`, `__GNUC_MINOR__=5`,
`__mips__` and `__MIPSEL__`, which the standalone preprocessor does not define.
The ordinary matching compiler path keeps its existing preprocessing contract.

There is one source compilation path: CPPPSX -> CC1PSX -> ASPSX -> native
Psy-Q OBJ. `kf build` passes those objects and the SDK libraries to PSYLINK,
then runs CPE2X without rewriting its output. `kf analyze` and `kf try` use
the same compiler and assembler, and read the resulting native objects into
ELF views for objdiff. No maspsx or GNU assembler processes game source.
Debug metadata is enabled in the compiler and assembler so private symbols
remain inspectable. It is part of the shared probe flags, not a second
comparison build configuration.

GNU MIPS binutils, psy-k and objdiff are inspection tools. They neither lay
out nor emit the candidate executables. The KF1 compiler scheduling evidence
is in [`patterns/gcc257-epilogue-and-scheduling.md`](patterns/gcc257-epilogue-and-scheduling.md);
SLPS-00069 JP/US game code shows the same return-slot epilogue class.

The Ghidra extension's Psy-Q 3.00 signatures are also an analysis corpus.
Vendored-function inventory code may use them to propose a name after the
Psy-Q 3.0 object search. Those JSON signatures cannot override exact object
evidence and are not SDK inputs.

The initializer verifies and stages the SDK directly:

1. `flake.nix` fetches the one Psy-Q 3.0 kit by immutable SHA-256.
2. `scripts/create-toolchain.py` verifies its tools, compilers and every
   linker-input `.LIB`/`.OBJ`, then copies the complete extracted tree under
   `psyq-3.0/`.
3. A deterministic manifest records every staged file.

No generated SDK binary, retail game image, or executable belongs in Git.

## Analysis tools

`nix develop` provides the single historical SDK and the separate analysis
programs above, plus Ghidra/PyGhidra, DOSBox, little-endian MIPS GNU
binutils, psy-k, disc-image utilities, objdiff, and the normal
C/C++/Python build tools. Ghidra plugin packaging is documented separately in
[`ghidra.md`](ghidra.md).

Splat, Rabbitizer, and spimdisasm use the separately locked Python environment
because the latter two are not available in the pinned Nixpkgs revision. The
shell wrappers keep both setup and execution on that project lock:

```sh
kf-python-sync
splat --help
```

The first command is the one-time `uv sync --frozen --no-install-project`
step; `splat` delegates to `uv run --frozen` thereafter.
