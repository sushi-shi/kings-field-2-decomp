# GAME target-candidate scorer pilot

`func_80039108` is an unclaimed 0x4c0-byte GAME function at `0x80039108`.
Its only proven direct caller is `actor_select_best_target`. The candidate
signature is `s32 func_80039108(KfTargetCandidate *target, s32 player_distance)`;
the first two arguments and signed result use are supported by that caller and
the body, while the full candidate-record field family remains incomplete.

Retail reads `target->type`, returns zero for `0xff`, then bounds-checks
`type - 2` against `0x82` and indexes words at `0x80011cd8`. The bounds check
proves 131 indexed words at `0x80011cd8..0x80011ee0` (0x20c bytes). Reading
every retail word yields ten unique instruction targets, all inside this
function; 114 rows select the default block at `0x8003953c`. The indexed
extent and decoded targets are proven retail facts. The table's original source
owner and its candidate relocation rows remain unvalidated. The following
word at `0x80011ee4` is zero before the next dispatch table at `0x80011ee8`;
its padding owner is unproved. The current
`data.tsv` and identity inventories still split the words into address-derived
pointer rows; a source reconstruction needs one unit RODATA claim after
ownership and delinker validation.

The nondefault target types are `0x02`, `0x03`, `0x04`, `0x05`, `0x09`, `0x0b`,
`0x0d`, `0x12`, `0x13`, `0x14`, `0x16`, `0x17`, `0x18`, `0x19`, `0x1b`, `0x70`,
and `0x84`. Repeated rows share cases; this list identifies dispatch shape,
not semantic names for the target kinds.

Several cases load an unsigned halfword at candidate-relative `+0x1a`, beyond
the current 0x16-byte `KfTargetCandidate` common-prefix view. Other cases read
both bytes and halfwords at `+0x0c..+0x10`. This supports type-specific or
larger candidate payloads, but does not yet prove one fixed record stride.
The `0x70` row returns `-1` directly; other named rows reach distance, facing,
or random-score gates before the shared return. These are case behaviors, not
confirmed gameplay labels.

The body loads `actor_state+0x93ac` as the current actor context and reads
`player_state+0xd8` position fields in several case paths. Direct calls include
`func_800157ac`, `rand`, `vector_xz_to_angle`, `angle_within_tolerance`, and
`func_80015574`. A separate `jalr` at `0x80039564` loads
`state_8017d118.active_table` from `0x8017d124`, then a pointer at table byte
offset `0x40` (slot 16). It passes the candidate in `$a0` and distance in
`$a1`; the callback target remains unresolved. The actor damage dispatcher at
`0x80039c94` uses the same active table's byte offset `0x48` (slot 18) for its
own unresolved `jalr`. That call presents an actor pointer in `$a0`, a scaled
value in `$a1`, two more register arguments, and six caller-stack words; the
destination and its source-level signature remain unproved.

No strings are referenced. The case paths perform distance checks, angle tests,
random gating, and score selection, with a shared return at `0x800395ac`.

Final verdict: **WIP, unclaimed**. There is no source object or strict score.
The next concrete ownership step is validation of the switch table's source
owner and a typed candidate-record model. The indirect branch's row values
are decoded, while the callback's destination is still unknown.

A one-VA focused carve of `80039108` uses only five curated relocation rows
and withholds 28 case-path MIPS26 calls and jumps after the indirect switch;
the delinker marks them `non-reachable-code-channel`. Source matching requires
the switch table and those case edges to be curated together. This does not
promote the indirect callback destination to a known function.

The adjacent magic recipient `80039c94` is a more tractable source pilot:
its focused carve withholds no relocations and its two sourced callers pass
thirteen arguments. Retail masks the actor index to sixteen bits, selects an
actor from the 200-entry pool, and may follow signed actor `+0x22` when the
slot state is three. It then loads the actor's group and reads eight unsigned
halfwords at group `+0x20..+0x2f`, passing each in order to the exact curve
helper `80039c14`. The shared group type now models that span as
`unknown_20[8]` without assigning an unsupported gameplay name. The summed
curve result is capped at `0x68db7`, scaled by a caller halfword and 5000,
then passed to active callback-table slot 18 with ten observed arguments.
That callback target remains unresolved. Later paths select actor targets,
award player training or experience, and build motion toward a supplied
position. A contiguous source claim now follows the exact curve helper in
`actor_fixed_curve.c`; its direct call set and three reviewed BSS relocation
pairs are represented, while the decompiler candidate remains only a guide.
KF1's `combat_calculate_damage_component` and `actor_apply_damage` show a
comparable squared-difference curve and training/experience sequence. They
support the source shape, but do not identify the KF2 callback or original TU.
Retail `0x8003a170` loads byte +2 from the selected target group for the
unlinked motion divisor; the linked branch instead loads the linked actor's
group index. Retail compares the zero-extended byte using signed `slti`, so
the C source holds the loaded value in an `s32` motion divisor.
Focused comparison is **WIP, 50.3% listing**: retail/source have 72/71 CFG
blocks, 46/46 branches, and the same seven incoming return edges. The first
real structural residue is one block, followed by broad register and stack
allocation differences. The exact `80039c14` sibling remains SAME. The
slot-18 callback target and several actor field meanings remain unresolved.

## Connected actor-behavior dispatch tables

The behavior controller `func_8003c614` tests unsigned `mode - 1 <= 0x7a`
before indexing `0x80011ee8`. This proves 123 indexed words through
`0x800120d0` (0x1ec bytes). The retail words name 19 distinct internal
targets between `0x8003c7c8` and `0x8003d060`; 100 select the common tail
at `0x8003d060`. `0x800120d4` is a zero word outside this indexed extent,
before the next table at `0x800120d8`; its source-level padding owner is
unproved. Only 23 mode values select nondefault targets, from the sparse set
`0x01`, `0x02`, `0x04`, `0x07`, `0x09`, `0x0c`, `0x16..0x18`, `0x1a..0x1d`,
`0x1f..0x21`, `0x28`, `0x6c`, `0x6e`, `0x70`, and `0x78`, `0x79`, `0x7b`.
The controller remains unclaimed because its complete mode and record contract
is unresolved.

Each table base has one decoded incoming address reference, from its owning
dispatcher. The three `lui/addiu` pairs at `80039158`, `8003c7ac`, and
`8003d350` are reviewed target references, but the indexed words remain
candidate table data without a source claim. Focused one-VA carving withholds
67 control relocations after the indirect switch in `8003c614`, and 249 after
the larger switch in `8003d184`.
These counts are an evidence gap in the current curated model, not evidence
that the case bodies are dead code.

The larger actor update dispatcher `func_8003d184` reads actor byte `+0x0e`,
accepts values through `0xf0`, and indexes 241 words at
`0x800120d8..0x80012498` (0x3c4 bytes). All decoded words point inside its
`0x8003d184..0x8003f610` body, with 28 distinct targets; 212 select
`0x8003f3ac`. The next word at `0x8001249c` begins an independently modeled
effect table. Nondefault actor-byte values are `0x00..0x05`, `0x09..0x1e`,
and `0xf0`; `0x0c`/`0x10` and `0x0d`/`0x11` each share a target. Its two
`jalr` sites at `0x8003d5e0` and `0x8003f3c0` load
`state_8017d118.active_table` slots 19 and 17 respectively; neither callback
destination is proved. Slot 19 receives the current actor pointer in `$a0`;
slot 17's call site does not freshly populate argument registers. These indexed
extents and row values are retail facts,
but neither table has a validated original source owner or source claim.

## Current GAME source comparison

The earlier unclaimed verdict above predates the current
`game.actor_candidate_score` source and its 131-word `RODATA` claim. The
current candidate record has a checked halfword/byte union at `+0x0c`.
Retail case `0x19` loads the high byte with `lbu` at `0x80039430`, then
shifts it four places for `angle_within_tolerance`. The prior C expression
shifted the full halfword and compiled as `lhu` plus `srl`; using
`word_0c.bytes.high` emits `lbu a2,13(s0)` at compiled offset `+0x2f4`.
The same union preserves halfword reads in the distance cases and the low
byte read in the facing case.

An isolated direct objdiff of the same 1,216-byte target shows the source-only
old halfword expression at **85.72369%** strict `.text` and the retained byte
expression at **89.0625%**. Its 524-byte `.rodata` remains 37.309162%; the
case-layout difference still changes jump-table targets. Focused `kf try`
remains DIFF, 35.1% listing: the first mismatch is saved-register assignment
after the prologue, and later switch-case placement differs. The direct call
set and reviewed address pairs are unchanged. The indirect callback-table
slot 16 and switch `jr` remain unresolved as before. No exact claim follows
from the improved strict score.

The affected `game.actor_fixed_curve` exact helper `0x80039c14` remains
focused SAME; its sibling `0x80039c94` remains WIP at 62.1% focused.
`game.event_target_stream` remains WIP at 90.5% focused. The actor behavior
dispatcher uses the same typed `+0x0c` view and is still WIP at 18.2% focused.

The connected 26-function target/caller/control pass below used focused
per-unit builds after the shared field migration. “Exact control” means the
prior strict report was 100% and the current focused listing, including
ordered relocations, is SAME. Scores on WIP rows are current focused listing
similarity except for the scorer's direct strict result above.

| GAME VA | Verdict |
| --- | --- |
| `0x80038cc8` | Exact control, SAME. |
| `0x80038d04` | Exact control, SAME. |
| `0x80038dc4` | Exact control, SAME. |
| `0x80038e38` | Exact control, SAME. |
| `0x80038efc` | Exact control, SAME. |
| `0x80038f20` | Exact control, SAME. |
| `0x80038ff0` | Exact control, SAME. |
| `0x80039048` | Exact control, SAME. |
| `0x80039080` | Exact control, SAME. |
| `0x800390d0` | Exact control, SAME. |
| `0x80039108` | WIP, 89.0625% strict `.text`; 35.1% focused listing. |
| `0x800395c8` | Exact caller control, SAME. |
| `0x800396c4` | Exact control, SAME. |
| `0x80039710` | Exact control, SAME. |
| `0x80039758` | Exact control, SAME. |
| `0x800397a8` | Exact control, SAME. |
| `0x800397d8` | Exact control, SAME. |
| `0x80039804` | Exact control, SAME. |
| `0x8003983c` | WIP, 87.0% focused. |
| `0x80039b58` | WIP, 79.2% focused. |
| `0x80039c14` | Exact sibling control, SAME. |
| `0x80039c94` | WIP, 62.1% focused. |
| `0x8003f610` | Exact sibling control, SAME. |
| `0x8003f7ec` | WIP, 93.8% focused; sentinel setup and `addu` operand order. |
| `0x8003f860` | Exact sibling control, SAME. |
| `0x800462bc` | WIP, 90.5% focused; first instruction residue is byte-load timing. |

For `0x80039b58`, retail keeps both the actor base and an actor-state field
pointer live through its 200-slot scan. A source-only trial with an explicit
second pointer preserved behavior but worsened focused similarity from 79.2%
to 67.3%, and the compiler still did not use retail's `lbu -7(s1)` group-index
form. It was discarded; the retained source has one actor cursor. For
`0x80039c94`, the eight direct curve calls and typed stack halfwords already
match the retail call set; its 72/71 block residue remains at the final
linked-actor target-type guard. There is no supported source correction from
this review.
