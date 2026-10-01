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
owner and its candidate relocation rows remain unvalidated. The current
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
Focused comparison is **WIP, 44.4% listing**: retail/source have 72/71 CFG
blocks, 46/46 branches, and the same seven incoming return edges. The first
real structural residue is one block, followed by broad register and stack
allocation differences. The exact `80039c14` sibling remains SAME. The
slot-18 callback target and several actor field meanings remain unresolved.

## Connected actor-behavior dispatch tables

The behavior controller `func_8003c614` tests unsigned `mode - 1 <= 0x7a`
before indexing `0x80011ee8`. This proves 123 indexed words through
`0x800120d0` (0x1ec bytes). The retail words name 19 distinct internal
targets between `0x8003c7c8` and `0x8003d060`; 100 select the common tail
at `0x8003d060`. `0x800120d4` is outside this indexed extent, before the next
table. Only 23 mode values select nondefault targets, from the sparse set
`0x01`, `0x02`, `0x04`, `0x07`, `0x09`, `0x0c`, `0x16..0x18`, `0x1a..0x1d`,
`0x1f..0x21`, `0x28`, `0x6c`, `0x6e`, `0x70`, and `0x78`, `0x79`, `0x7b`.
The controller remains unclaimed because its complete mode and record contract
is unresolved.

Focused one-VA carving withholds 68 control relocations after the indirect
switch in `8003c614`, and 249 after the larger switch in `8003d184`.
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
