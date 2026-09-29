# King's Field II: SDK, compiler, function census, and regional lineage

First-pass evidence for the three King's Field II builds. Everything here is
reproducible from `scripts/` against the executables in `retail/` (ignored;
copied from `kings-field-investigation/extracted/`). Function counts are
machine seeds, not a curated inventory.

| Key | Serial | Boot EXE | Role |
| --- | --- | --- | --- |
| `jp` | SLPS-00069 | `PSX.EXE` | Japanese original, primary target |
| `us` | SLUS-00158 | `SLUS_001.58` | US localization ("King's Field") |
| `eu` | SCES-00510 | `SCES_005.10` | PAL En/Fr/De localization ("King's Field") |

All three discs use a 2 KiB boot loader at `0x80010000` plus three overlays
that share one window at `0x80011000`: `OPEN.EXE`, `GAME.EXE` and `END.EXE`.

## 1. Sony runtime library (SDK)

### Direct RCS witnesses

| Build | `$Id$` strings in OPEN/GAME/END |
| --- | --- |
| jp, us | `intr.c` 1.52 1995/03/14, `pad.c` 1.33 1995/03/14, `vsync.c` 1.7 1995/03/14, `sys.c` 1.67 1995/03/13, `s_crwa.c` 1.8 1995/03/11 |
| eu | `bios.c` 1.71 1995/12/01, `intr.c` 1.73 1995/11/10, `sys.c` 1.116 1995/12/01 |

JP and US link the identical March 1995 runtime snapshot. EU was relinked
against a runtime from December 1995.

### Object-signature census (`scripts/psyq_sig_census.py`)

Every object signature from the `ghidra_psx_ldr` Psy-Q corpus was searched,
with relocation bytes wildcarded. The corpus covers SDK versions 2.6, 3.0, 3.3,
3.4, 3.5, 3.6.10, 3.6.11, 3.7 and 4.0 through 4.7. The table shows the share
of matched library bytes by version. Only objects of at least 64 bytes count.

| Library | jp/us GAME | eu GAME |
| --- | --- | --- |
| LIBCD | **3.0 100%** (2.6 30%) | **3.4 99%** (3.3 47%) |
| LIBETC | **3.0 100%** | **3.4 100%** (3.3 95%) |
| LIBGPU | **3.0 100%** (2.6 80%) | **3.4 99%** (3.3 33%) |
| LIBGTE | **3.0 92%** | 3.3 88% / 3.4 86% |
| LIBSND | **3.0 100%** | **3.4 100%** |
| LIBSPU | **3.0 100%** | **3.4 90%** |
| LIBPRESS (OPEN/END) | none matched | **3.4 100%** |

- **JP/US: Psy-Q / Runtime Library 3.0.** This is proven. The archived 3.0 kit
  (below) contains exactly the five RCS revisions listed above.
- **EU: Runtime Library 3.4.** This is strong. A few GTE objects match only
  3.3 or 3.5+ signatures. That could be real mixing (for example a GTE
  library from another drop) or corpus splitting noise. It needs the actual
  3.4 `.LIB` files to settle.
- `MALLOC.OBJ` matches the 2.6/3.4 signature in every build. `PSX.EXE` holds
  only `2MBYTE.OBJ`-style startup, which is identical across all versions.
- JP/US `OPEN.EXE` and `END.EXE` contain Sony code that the 3.0 corpus does not
  cover, presumably the MDEC/STR movie path. It lies inside the SDK span and is
  counted as SDK below.

## 2. Compiler and assembler

A PS-X EXE carries no compiler banner, so attribution comes from codegen
fingerprints (`scripts/codegen_fingerprints.py`). They are split between
signature-matched Sony bytes and everything else.

| Fingerprint (game code) | KF1 SLPS-00017 | jp | us | eu |
| --- | --- | --- | --- | --- |
| Framed epilogue: `addiu sp` in the `jr ra` delay slot | 370/375 | 252/256 | 252/256 | **0/152** |
| Framed epilogue: `addiu sp` before `jr ra`, `nop` slot | 3/375 | 3/256 | 3/256 | **152/152** |
| `$gp`-relative loads/stores (small data) | 1 | 0 | 0 | **408** |
| `div` with `break 7` / `break 6` traps | yes | yes | yes | yes |
| `$fp` frame setup (`-O0` sign) | 0 | 0 | 0 | 0 |

(EU counts are over Ghidra-seeded game functions only. The Sony runtime
objects that still use the old epilogue are excluded.)

- **JP/US game code** was built with the same compiler class as KF1: an
  optimizing GCC that places the stack restore in the return delay slot, with
  `-G0` (no small data). The KF1 probe campaign showed that GCC 2.6.0 cannot
  emit that epilogue, while 2.4.1 and 2.5.7 do. The Psy-Q 3.0 kit ships both
  `CC1PSX` 2.4.1 and `CC1PSX.EXE` 2.6.0, byte-identical to the Release 2.5
  pair already in the KF1 toolchain. So **GCC 2.4.1 from the kit is the
  natural candidate**, with ASPSX 2.08, CCPSX 1.10 and PSYLINK 1.29. This is
  not yet proven by a compile probe.
- **EU game code was recompiled, not relinked.** No framed EU game function
  keeps the old epilogue, and `$gp` small data appears (`-G` > 0). The
  contemporary SN compiler is `CC1PSX.EXE` **GCC 2.7.2.SN.1** (dated
  1995-12-20). It ships with ASPSX 2.34 (1996-02-20) and CCPSX 1.18
  (1996-02-14). It's a candidate, not proven: GCC 2.6.x would also produce
  the new epilogue.

## 3. Function counts (Ghidra 12 + ghidra_psx_ldr seed)

Psy-Q links game objects before the libraries, so everything from the load
address up to the first matched Sony object is game code. The SDK span runs
from there to the last Sony object.

| Image | jp game | us game | eu game | jp SDK span | eu SDK span |
| --- | ---: | ---: | ---: | ---: | ---: |
| boot EXE | 1 | 1 | 1 | ~5 | ~5 |
| `OPEN.EXE` | 26 (8.4 KB) | 27 (10.7 KB) | 28 (9.0 KB) | 282 (63 KB) | 682 (92 KB) |
| `GAME.EXE` | **488 (217 KB)** | 489 (220 KB) | 494 (227 KB) | 330 (70 KB) | 776 (104 KB) |
| `END.EXE` | 17 (2.9 KB) | 14 (2.1 KB) | 14 (2.1 KB) | 262 (61 KB) | 501 (51 KB) |
| **Total game** | **532** | **531** | **537** | | |

- The reconstruction target is about 530 game functions and 230 KB of game
  code, almost all in `GAME.EXE`. `OPEN.EXE` and `END.EXE` are thin drivers
  around Sony movie and audio code. KF1's curated inventory, for comparison,
  has 935 GAME and 665 OPEN rows including SDK.
- The SDK-span counts are **not comparable** between JP and EU. Ghidra applied
  its 3.4 signatures to EU and split those libraries into named functions, but
  it recognized almost none of the 3.0 objects. Use bytes, not counts, for the
  SDK.
- KF1 found customized, version-skewed Sony code (LIBSND `SEQREAD`) inside its
  "game" span. The same could be true here and needs a skew-tolerant pass
  before the game count is final.

## 4. Similarity between the three builds

`scripts/function_similarity.py` compares game functions. **exact** means
identical once `jal`/`%hi`/`%lo`/`$gp` fields are masked. **shape** keeps only
opcodes and ignores registers, immediates and nops, so it survives a
recompile. "Similar" means the best 4-gram Jaccard score is at least 0.5.

`GAME.EXE`, JP as the reference (488 functions, 217 KB):

| Pair | exact identical | exact similar | shape identical | shape similar | unmatched (shape) |
| --- | --- | --- | --- | --- | --- |
| jp to us | **420** (155 KB) | 41 (43 KB) | 432 | 43 | 13 (7 KB) |
| jp to eu | 82 (7 KB) | 102 (52 KB) | 86 | 148 (114 KB) | 254 (96 KB) |
| jp to KF1 | 32 (2 KB) | 17 (3 KB) | 45 | 25 (6 KB) | 418 (208 KB) |

Best shape score per JP function, by share of bytes:

| Score | jp to eu | jp to KF1 |
| --- | ---: | ---: |
| ≥ 0.7 | 14% | 2.5% |
| 0.5–0.7 | 42% | 1.7% |
| 0.3–0.5 | 29% | 6.8% |
| < 0.3 | 15% | 89% |

`OPEN.EXE` jp to us: 17 of 26 identical. jp to eu: 2 identical, 23 unmatched.
`END.EXE` jp to us: 8 of 17 identical. jp to eu: 2 identical, 15 unmatched.

- **US is a localized relink of the JP code.** It has the same compiler and
  SDK, about 86% of functions are byte-identical after masking, and most of the
  rest are near-identical (text and layout edits). Mostly shared JP/US source
  is realistic.
- **EU is the same game source recompiled with a newer compiler and SDK,** plus
  PAL and multi-language changes. The structure survives (85% of JP bytes have
  a shape partner at ≥ 0.3), but almost nothing is byte-identical. It is a
  separate matching target with its own compiler profile.
- **KF2 is largely new code, not a KF1 derivative.** Only about 45 small
  helpers survive unchanged from KF1. The KF1 source tree helps with engine
  concepts, formats and SDK handling, not with direct function reuse.

## 5. What is available on archive.org

| Needed for | Item / file | Contents | Status |
| --- | --- | --- | --- |
| jp/us SDK + tools | [`psyq_psx_toolchain_april_08_1994`](https://archive.org/details/psyq_psx_toolchain_april_08_1994) `psx.zip` (48.8 MB; SHA-256 `416241637cdb0273b6bd936b0ab5627c8ba7225ee2b98066cf3b50fb09e57899`) | Psy-Q PS-X Development System Release 3.0; files dated **1995-04-08** despite the item title. `LIB/*.LIB`, `INCLUDE`, `CC1PSX` 2.4.1, `CC1PSX.EXE` 2.6.0, ASPSX 2.08, CCPSX 1.10, PSYLINK 1.29, CPE2X | **Downloaded. RCS revisions match JP/US exactly.** ASPSX 2.08 has no software-key strings (untested under DOSBox). |
| jp/us SDK (Sony CD) | `ps1_sdks` "Programmer Tool - Runtime Library Version 3.0 (Japan) DTL-S2180" (68 MB) | Official Sony 3.0 CD | Available, not downloaded; should hold the same libraries |
| eu SDK | Runtime Library **3.4** | — | **Not found on archive.org.** `ps1_sdks` has 3.3 (DTL-S2190) and 3.5 (DTL-S2300) as neighbours. The Ghidra corpus has 3.4 signatures, but those are not `.LIB` files |
| eu compiler | [`psyq-sdk`](https://archive.org/details/psyq-sdk) `PSYQ_SDK.zip`, member `psyq/psyq/CC1PSX.EXE` | GCC **2.7.2.SN.1** (1995-12-20), ASPSX 2.34, CCPSX 1.18, CC1PLPSX | Members downloaded individually |
| later tools | same zip, `psyq/bin/` | GCC 2.95.2 (1999), SDevTC ASPSX 2.86 | Too late for any KF2 build |
| linker/librarian only | `ps1_sdks` `PSY_ExecutablesFrom1995.zip` | PSYLINK (1995-12-21), PSYLIB (1995-11-08) | Possible EU-era linker witness |
| GCC 2.6.0 disk | `ps1_sdks` "GNU C Compiler Version 2.60" | GCC 2.6.0 (1994-10-20) | Already used by KF1 |

Downloaded witnesses live outside the repository
(not under version control). They are proprietary: hash them and keep them
external; don't commit them.

## 6. Next steps to prove the compiler

1. Run the 3.0 kit's `CC1PSX` 2.4.1 and ASPSX 2.08 under DOSBox, using the go32
   stub transplant from the KF1 notes. Compile small JP leaf functions and
   compare against the KF1 `cc1psx-257` rebuild. First choose functions that
   are byte-identical between JP and US.
2. Do the same for EU with GCC 2.7.2.SN.1, sweeping `-G` (for example `-G8`)
   and `-O2`, and compare with the 2.6.0 binary.
3. Rerun the signature census against the real 3.0 `.LIB` files through psy-k
   (KF1's `fid_census.py`) to settle JP's unmatched movie-path objects. Then run
   a skew-tolerant pass over the game span.
