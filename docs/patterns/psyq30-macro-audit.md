# Psy-Q 3.0 retail-header macro audit

This audit uses the pinned 1995-04-08 Psy-Q 3.0 `include-lf` tree, not a later
SDK manual or an inferred header. The [header hashes](psyq30-macro-header-hashes.tsv)
and [macro index](psyq30-macro-definitions.tsv) identify every definition by
header, physical line, name, function/object form, parameters, replacement
hash, and surrounding preprocessor conditions. The inventory tool writes
complete replacement text to `build/sdk-macro-audit/definitions.tsv` locally;
it is not copied into this public source tree. Inactive branches
and repeated definitions remain separate rows: 1,067 `#define` occurrences
across all 37 headers, of which 173 are function-like and 894 object-like.

The [inventory script](../../scripts/kf/sdk_macro_inventory.py) scans all 163
`src/**/*.c` files and all 56 `.h` files under `include/` and
`vendor/include/`, preserving line positions while masking comments and
strings. Its frozen [219-file source ledger](psyq30-macro-source-files.tsv)
includes a SHA-256 for each file; the associated [name mentions](psyq30-macro-uses.tsv)
and [SDK field writes](psyq30-macro-field-writes.tsv) preserve every lexical
hit. The final retained-source snapshot has 252 non-preprocessor name mentions,
11 preprocessor mentions, 157 source-local definitions, and 646 writes to
fields named by a function-like SDK macro. The pre-substitution C scan had
668 such writes. A file with zero hits is still present in the ledger. Header
hits include an `abs` declaration and a locally defined varargs helper, so
these are lexical candidates rather than proven SDK expansions. The
[preprocessor mentions](psyq30-macro-preprocessor-uses.tsv) and
[local definitions](psyq30-macro-local-definitions.tsv) expose `WAIT_TIME`,
`NULL`, and the locally defined varargs forms. The
[definition decisions](psyq30-macro-definition-decisions.tsv),
[file decisions](psyq30-macro-file-decisions.tsv),
[name-site decisions](psyq30-macro-name-site-decisions.tsv), and
[field-site decisions](psyq30-macro-field-site-decisions.tsv) contain one row
for each of the 1,067 definitions, 219 files, 252 name sites, and 646
field-write sites. Literal collisions without referent or header-use evidence
remain unapplied. This is a complete lexical screen, while a byte-identical
expansion is evidence of
compatibility rather than proof of the original author's macro spelling.

Regenerate the census inside `nix develop` with
`python -m scripts.kf.sdk_macro_inventory --include "$PSYQ_INCLUDE" --source src --source include --source vendor/include --output build/sdk-macro-audit`,
then `python -m scripts.kf.sdk_macro_review --census build/sdk-macro-audit --output build/sdk-macro-audit`.
The source ledger hashes make a stale scan visible after later edits.

## What the headers provide

| Header/family | Function-like definitions | Application boundary |
| --- | ---: | --- |
| `LIBGPU.H` | 74 | Vector, RECT, packet field, ordering-tag, page/palette, and primitive setup macros. |
| `LIBGTE.H` | 28 | Inside `#ifdef ASSEMBLER`; unavailable as C source macros. |
| `LIBCD.H` | 12 | BCD arithmetic and fixed-argument `CdControl` wrappers. |
| `R3000.H` | 12 | Segment/address conversion and predicates; no matching source operation was established. |
| `CTYPE.H` | 15 | Character-classification table expressions; no source candidate. |
| Other headers | 32 | `abs`, assertions, varargs, FS internals, SPU aliases, and utility forms. |

For the non-packet expression families, the source screen included operand
shapes as well as names. No C source accesses the `FS.H` circular-buffer or
device-I/O members, the `_ctype_` table, scratchpad base `0x1f800000`, or
SPU transfer-mode APIs; there is no segment-conversion bitmask operation to
substitute with the `R3000.H` forms. Source CD control commands were checked
against every fixed-argument `LIBCD.H` wrapper: the remaining direct calls
use commands without wrappers or pass a result pointer that a wrapper would
drop. The only same-variable two-bound clamps are the two `limitRange` trials
below. Bit-shift screens found one manually packed CLUT calculation; its
`getClut` trial is below. The source uses the SDK `AddPrim` API rather than
the distinct lowercase `addPrim` tag-manipulation macro.

The remaining object-like definitions are largely SDK register names, status
bits, constants, header guards, and API flags. A raw value is not a source
identity. After stripping header comments and redundant parentheses, 518
numeric object-like definitions reduce to 104 distinct values; 69 of those
values also occur as source numeric tokens,
often with several unrelated SDK names. The value `0x80000000`, for example,
has several SDK definitions and is also a RAM boundary. None was applied by
number alone. `WAIT_TIME` in `src/lib/movie_stream.c` duplicates the SDK value
but may belong to the tutorial source, so its local definition remains.

## Application and rejection decisions

The [per-site vector ledger](psyq30-macro-vector-trials.tsv) records all 46
remaining contiguous X/Y/Z assignment groups found after the first retained
edits. Five additive groups have scalar or incrementing RHS expressions and
cannot use `addVector`. Of 41 isolated alternative-source trials, 34
`setVector` substitutions preserved an already exact listing; four WIP
listings retained their displayed result; one WIP `copyVector` listing retained
its displayed result; the middle magic-dispatch `setVector` changed an exact
listing; and one trial could not complete the native object conversion. The
last case is `effect_spawn_zero_direction.c:17`: compilation reached the
`SIZES` LNK object, whose symbol-name decoding raised a non-ASCII error. It
was attempted, is unresolved, and was not applied. The byte-identical
alternative-source trials were left as explicit assignments unless the SDK
form also improved the source model: retail bytes alone cannot distinguish
the two C spellings.

The following directly corresponding SDK forms were retained after focused
retail comparison:

- `addVector` for position/direction accumulation in GAME actor, effect, and
  map-object functions; `copyVector` for full X/Y/Z copies in collision,
  sparse animation, rendering, effect spawning, and quarter-turn code.
- `setVector` for an actor-group output and the two menu model preview values;
  the exact functions and their same-unit siblings retained their listings.
- `setRECT` for the floor-item upload rectangle and the CD image request's
  four source words, preserving their retail store widths and order.
- `CdSeekL`, `CdSeekP`, `CdPause`, and `CdStop` where the original call already
  had the wrapper's fixed zero result argument. `strKickCD` was checked in
  both OPEN and END. `CdControl(CdlStop, ..., &cd_result)` remains a direct
  call because the wrapper discards that result pointer.
- `itob` in the exact `cd_int_to_bcd` helper, and `setClut`/`setTPage` for
  GAME fade-packet assignments that already called `GetClut`/`GetTPage`.
  Those page/palette wrappers retain the SDK calls; lowercase `getClut` and
  `getTPage` are different call-free forms.

Rejected trials remain explicit assignments. `btoi` changed the exact
`cd_bcd_to_int` instructions: retail shifts/masks the BCD byte, whereas the
SDK macro spells division and remainder. `setVector` in the middle
`player_magic_dispatch` case changed an exact function. `setRECT` in OPEN
`main` changed an exact function. Individual `setXYWH` and `setUVWH` trials
for all four GAME fade quads left the already WIP listing unchanged, but the
combined eight-site form changed code generation; no quad rewrite was kept.
`setXYWH` and `setUVWH` changed the exact notification-quad listing, so its
chained store order remains. The textured-quad UV trial remained non-exact.
TMD packet rows use packed word/halfword transfers and cannot be replaced by
macros that emit separate coordinate or UV stores. Reordered X/Z/Y writes,
partial vectors, and intervening effects are likewise not direct macro sites.

The [packet-tag trial ledger](psyq30-macro-packet-tag-trials.tsv) records all
17 paired direct tag-length/code writes in `render_map.c` and
`tmd_pipeline.c`. Replacing each pair in isolation with `setlen` and
`setcode` left the complete `kf try --context 0` listing identical, including
instruction and ordered relocation lines. Six constant-code cases also had
identical complete listings with the corresponding `setPoly*` macro, plus
`setSemiTrans` where the code bit requires it; see the
[primitive setup trials](psyq30-macro-primitive-setup-trials.tsv). The
original spelling remains underdetermined, so these compatible alternatives
were not installed over the current source.

The [other focused trials](psyq30-macro-other-trials.tsv) cover both remaining
adjacent `w`/`h` assignments (`setWH` produced identical listings), two full
clamps (`limitRange` changed their listings), the exact `abs` caller
(`ABS.H` changed its listing), and a packed menu CLUT calculation
(`getClut` changed its exact listing). Direct `clut`/`tpage` copies were not replaced
with `setClut`/`setTPage`: those SDK forms call `GetClut`/`GetTPage`, while the
source sites transfer an existing value or use lowercase call-free `getClut`.
The source-wide field ledger also records partial rectangle writes, scalar
vector updates, reordered stores, and packed TMD transfers separately.

This pass did not reconstruct C inline helpers. A direct search of all 37
pinned headers found no C `inline`/`__inline` definition and no `static`
function body; the only `static` declaration is the `_ctype_` data array.
The word “inline” appears only in a comment on the `pollhost()` macro. The
28 `LIBGTE.H` forms under `ASSEMBLER` are macros, not evidence of C inline
use. Three current project C helpers are spelled `static inline` (one in
`include/kf/lib/math.h`, two in GAME source); that WIP spelling does not
establish a retail SDK inline family. A later inline pass would need evidence
outside these retail headers.

## Verification boundary

The retained source substitutions were compared with `kf try` unit objects.
Strict `kf match` then reported GAME 391/461, OPEN 24/25, and END 16/16
scored functions exact, preserving the pre-audit exact counts. GAME relinked
149/149 targets, OPEN 10/10, and END 8/8. The global match gate still stops
at known incomplete data/reference closure, including three unrelated GAME
`.rodata` addends. The required full `kf build` produced PSX; GAME, OPEN, and
END retain their existing unresolved links (`InitCARD`, `malloc`, and
`display_buffers` respectively). `ruff check` on the inventory script and
`git diff --check` passed. Repository tests were deliberately not run, per the
user's direction. No macro substitution was banked merely from a loose or
listing-only score.
