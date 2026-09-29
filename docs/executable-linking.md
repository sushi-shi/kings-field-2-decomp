# Source-to-EXE build

Inside `nix develop`, run:

```sh
kf build
```

`kf link` is an alias for the same executable builder. There is one candidate
artifact chain:

```text
C source -> CPPPSX/CC1PSX -> assembly -> ASPSX -> Psy-Q OBJ
Psy-Q OBJ + original SDK OBJ/LIB -> PSYLINK -> CPE -> CPE2X -> PS-X EXE
```

The EXE is the unchanged output of CPE2X. Retail bytes are comparison inputs
only. The build does not synthesize object sections, force addresses from
`DATA()` claims, rewrite the CPE, patch the EXE, or copy missing retail bytes.
A failed phase removes the stale EXE.

The pinned Psy-Q 3.0 kit supplies PSYLINK 1.29, CPE2X 1.3, headers, libraries,
and the overlay `NONE2.OBJ` startup. The active chain still uses the separately
hash-pinned ASPSX 1.07 inherited from the SLPS-00017 setup; the kit's ASPSX
2.08 is not yet wired in. The build report records every tool and input hash. This is
a reproducible source-to-EXE chain, while exact historical compiler and
assembler attribution remains open.

Overlay startup uses `BSS_START` and `BSS_END` from zero-byte boundary
declarations in `config/link/overlay_bounds.asm`. A separately pinned native
ASMPSX 2.34 assembles those declarations because the C assembler lacks named
sections and the kit's ASMPSX 1.21 is not yet validated under DOSBox. PSYLINK
places the labels around `.bss`; their object contains no instructions or
storage. Startup derives its word count and heap size from RAM/stack settings.

Derived ELF objects, delinked retail modules, objdiff projects, and semantic
reports are analysis views. Source compilation in `kf analyze` and `kf try`
uses the same CPPPSX/CC1PSX/ASPSX implementation as `kf build`. The reader
translates the resulting native LNK objects for objdiff; no GNU assembler
recompiles game source and no ELF view is an input to PSYLINK.

ASPSX can erase an in-object data symbol into a section-relative reference.
In SLPS-00017, `floor_entry_cells-2` became `.data+14` when the table started
at `.data+16`.
The reviewed `config/native_reloc_referents.tsv` records the image, function,
exact function-relative HI/LO offsets, data owner, signed addend and evidence
for exceptions that need their named owner in the comparison view. The
SLPS-00069 table has no rows yet.

The reader applies a row only to its exact pair. Both native patches must
refer to the same section and address; the named owner must be uniquely
defined there, and `owner offset + addend` must equal the original native
target. The instruction pair must be LUI plus a sign-extending low operation
using its register. Missing, moved or contradictory pairs fail instead of
choosing another symbol. Identical addresses at other sites remain unchanged.
No assembly-wide spelling search or nearest-owner selection is performed.

This follows Gruntz's explicit referent-table approach on the native-view side.
Retail owner evidence remains independently curated in `config/retail/relocs.tsv`;
the native conversion does not read a retail executable or borrow its bytes.
Tests reconcile each current manifest row with that independent retail record.
The TSV is a direct compile dependency and applied rows are recorded in object
metadata. Only the ELF view's symbol/addend split changes; native LNK objects,
PSYLINK inputs and executable bytes remain unchanged. These rows are reviewed
reconstruction evidence, not proof of historical source spelling.

The shared probe enables native compiler/assembler debug metadata for private
symbols and function records. The source assembly passes unchanged except for
DOS line endings. Native COMMON reservations remain unplaced in the ELF view;
only PSYLINK assigns their executable addresses.

Current source-to-EXE status is distinct from exactness and playability:

| Property | Current status |
| --- | --- |
| PSYLINK emits PSX, GAME, OPEN, and END | Wired through `kf build`; needs at least one source unit per image |
| Candidate artifact path | Direct source-to-EXE with no output rewriting |
| Historical toolchain identity | Open; the usable ASPSX is separately sourced |
| Byte-identical retail images | Not achieved |
| Boot | Not yet attempted for SLPS-00069 |

An SDK mismatch does not by itself prove that an executable is unplayable: a
different library revision may preserve the public API and runtime behavior.
It also cannot establish complete playability, because private object layouts,
data, callbacks, and initialization behavior can differ even when linking
succeeds. The original hash-verified retail disc remains the runtime reference.

### Runtime smoke test

`kf-run-candidate` substitutes linked executables into a copy of the raw retail
Mode 2 disc (`KF_CANDIDATE_IMAGES` selects which ones, for controlled hybrid
tests). The retail BIN must match the SLPS-00069 SHA-256
`ca7d6d616d125c2d7bd7be131e8362eddb0b71cd15a2902dae73453dbe043a31`. Every
changed sector's EDC and P/Q ECC is regenerated; `kf-run-retail` boots the
unmodified disc.

Existing source-to-EXE outputs can be compared again without rebuilding them:

```sh
kf link --compare-only
```

The PSYLINK controls use original Psy-Q objects and libraries to test section
alignment and BSS allocation.

## Layout-tolerant executable comparison

`kf link --compare-only` compares moved byte regions in existing outputs and
refreshes the second generated README block, `executable-score`:

```sh
kf link --compare-only
```

The command accepts `--image psx|game|open|end`. The README keeps one row per image
and a byte-weighted total for the available images. It reads saved reports
only when their candidate hashes match EXEs still on disk and their retail
hashes match the configured retail identities. Missing, changed or failed
outputs receive no cached score. The executable and function blocks share
the same update lock and preserve each other.

The **Data modules exact** column shows complete passing source data owners
out of all compared owners, using the same strict gate as `kf verify data`.
A module counts only when every initialized section, relocation, BSS layout
and retail section placement passes. Exact functions or equal initializer
bytes alone do not make its data exact. Separate config-owned SDK contributions
are excluded; these object checks do not measure the generated EXE's similarity.
`kf analyze`/`kf check` and `kf bank` also refresh this column. Missing comparison
artifacts or objects older than their source, headers, inventories or build
inputs show `—`, independently of executable-score availability.

The report is a content heuristic called `unique-byte-islands-v1`, not a
reimplementation of objdiff or a claim of function identity:

1. Find exact 16-byte windows that occur once in each load area. Coalesce
   overlapping windows with the same displacement, then select the longest
   nonoverlapping runs. Each byte can be assigned at most once in each image.
2. Extend selected runs through equal, unused neighboring bytes. Also keep
   exact file-boundary prefixes and suffixes, so repeated padding does not
   require a unique seed.
3. Join adjacent anchors only when they are neighbors in both images. Each
   intervening gap is at most 128 bytes. A bounded byte-sequence comparison
   accepts gaps with at least 60% similarity, or edits of at most 16 bytes
   per side when the two flanking exact runs supply at least 85% similarity
   across that local span. Unchanged bytes inside an accepted gap earn
   credit; its edited, inserted or deleted bytes do not.
4. Sum equal byte pairs across all selected islands. Report similarity as
   `200 * equal_pairs / (retail_load_bytes + candidate_load_bytes)`. All
   unpaired bytes remain in the denominator. The nonzero score uses the
   same formula with zero-valued bytes removed from both counts.

Islands may move or reorder, but no address, opcode, register, immediate,
constant or data byte is masked. A changed pointer still loses byte credit;
similar pointer encodings do not prove the same referent. Short or repetitive
regions without suitable anchors can remain unpaired even when shared.
The algorithm is conservative and deterministic, rather than an optimal
global alignment or an instruction-semantic comparison.

The island scores cover the complete EXE load areas, including sector
padding, and exclude the 2048-byte headers. The separate fixed-offset
comparison still covers the complete files and is the only executable
equality check. Neither island similarity nor island coverage changes
function scores, banking criteria or the full data-layout gates.

Normal builds write tool commands and input hashes into each image's
`build.json`. They do not load retail EXEs, compare output bytes, or update
the README. Explicit comparison-only runs write `fuzzy-comparison.json` and
`fuzzy-comparison.html`, preserving the build report and executable.
The standalone HTML contains a movement
map, filterable islands with both addresses and sizes, and the largest
unanchored regions. The JSON retains all islands, complementary unanchored
ranges, parameters, counts and input hashes. These generated files stay
under `build/link/{psx,game,open}/` and are not committed.

## Verification and limits

The full rebuild compared 484 functions, including thirteen source-verified vendor functions. Every score is unchanged from the pre-edit snapshot: 460 remain exactly 100%. The game-only totals remain 340/362 GAME, 106/108 OPEN and 1/1 PSX. No new function was banked.

All 22 newly defined initialized objects match their retail extents and raw bytes in the freshly compiled objects. This includes every sound selector, the six light matrices, path arrays and initialized state words; it does not establish their linked positions.

At the time of that experiment, the full repository suite passed all 739 tests
with local retail/build inputs. `ruff check scripts tests`, `git diff --check`
and `nix flake check -L` also passed. Those counts record the retired campaign,
not the current suite.

The executable-island controls cover reordering, small edits, insertions/deletions, duplicate-copy accounting, unrelated content, padding, changed address encodings and malformed EXEs. README controls cover hash freshness, weighted totals, preservation of the function block and repeatable updates. The source-to-EXE build and a subsequent comparison-only pass gave identical island reports for all three KF1 images; the latter left native EXEs and build-provenance reports unchanged. These integration checks are recorded under `build/link/island-audit/`.

The native executable controls check the minimal startup opcodes and
symbolic entry, all four BIOS aliases, initialized zero data, exclusion of an
8192-byte BSS buffer, unchanged original archives, and rejection of unresolved
references or stale output. They do not execute a complete game image.

The existing function oracles now explicitly opt into named-object data bindings where merged modules own data from separate retail regions. Each object keeps its compiled extent and initializer; ambiguous section-relative references, unknown objects, wrong extents and overlapping initialized objects are rejected. The returned program records `data_binding=objects`. This isolates function semantics and does not prove section placement. Default oracle linking still requires a consistent section base. This facility is not used by `kf link`, the delinker or the full data-layout checks.

`kf analyze` still exits unsuccessfully on data layout and incomplete ownership.
With native LNK-derived views it reports 0/1 PSX, 9/20 OPEN and 23/41 GAME
data-owning units matching. Unplaced COMMON reservations remain explicit
failures rather than receiving inferred section offsets. Equal bytes or
unchanged resolved functions do not waive ownership and placement failures.
Earlier closure reports remain under `build/link/overlay-link-audit/`; current
native build commands and input hashes are in each image's `build.json`.
