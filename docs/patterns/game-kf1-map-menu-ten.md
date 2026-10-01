# GAME KF1 map-object and menu follow-up

The following ten GAME.EXE source claims were checked against the current
retail disassembly and focused rebuilt listings. The map-object relatives in
KF1 `src/game/map_object.c` and `map_object_pool.c` support the effect-pool,
placement, and action roles, but KF2's template kinds, reset call, and larger
action controller prevent transferring KF1 source or register layout verbatim.
The strict report is stale; percentages in this table are focused listing
similarities, never exact-match claims.

| VA | Focused verdict | Current boundary |
| --- | --- | --- |
| `80015918` | DIFF 85.2% | Trajectory discriminant and selected-time registers differ after the same three calls and 41-block CFG; exact `15bc8` and `15ce0` controls remain SAME. |
| `80023384` | DIFF 63.2% | Retail constructs three independent `player_state` field addresses per bound; compiled C retains a saved base pointer. Death-threshold calls and widths agree. |
| `800144b8` | DIFF 91.8% | VAB service holds phase value 1 versus retry sentinel -1 in the saved register; calls and 11-block CFG agree, and 15 audio siblings remain SAME. |
| `80035894` | **Exact 100%** | Retail loads the template field before storing independent object defaults, then copies an eight-byte placement tail with two loads before two stores. KF1 has the same aggregate-copy source pattern, while KF2 raw bytes establish the actual field offsets. |
| `80036190` | DIFF 99.3% | Interaction probe has the same 15 blocks and calls; only an angle-call load/move order differs. Two same-unit controls remain SAME. |
| `80036464` | DIFF 92.8% | Effect spawner uses different saved-register assignments for the object ID and height offset; ordered calls and KF1-supported pool/sequence flow agree. |
| `80036ed4` | DIFF 82.6% | Large action update has 329 retail versus 327 compiled CFG blocks, with 180/180 branches. First substantive difference is the action timer's register/condition schedule; prior natural action-case probes were reverted. |
| `80021c8c` | DIFF 97.2% | Retail reserves 32 stack bytes, compiled C 24. All remaining instructions agree in the focused listing; adjacent `21e00` stays SAME. |
| `8001fb8c` | DIFF 83.5% | Retail reserves 48 stack bytes, compiled C 40. Body instructions and references agree; the difference repeats at saved-register restores. |
| `800228c8` | DIFF 67.6% | Retail uses signed title-byte reads and a two-byte offset walk. Its 24 blocks and 13 branches agree with the current typed card-reader model; a prior natural offset-walk probe regressed and was removed. |

The two menu frame residues provide no evidence for a live local or changed
signature. KF1's menu renderer is structurally different and does not justify
stack padding. The initial ten-function sweep gained no exact match. Existing
exact siblings were left untouched, and no repository tests were run.

After the checkpoint, the placement loader's raw instructions at
`0x80035984..0x80035990` supported moving the assignments of
`unknown_05` and `unknown_10` after the independent template-field
assignment in C. A focused rebuild improved its listing from 98.7% to
99.3% with no earlier divergences. Isolated direct objdiff of baseline
and changed scratch objects against the same safe retail module moved
strict `.text` from 97.758415% to 98.403960% (2020 bytes); `.data`
270/270 and `.rodata` 1016/1016 bytes remained exact. At that stage the
later load/`memset` schedule was still WIP.

The final source models placement bytes `+0x10..+0x17` as a checked eight-byte
`KfMapObjectTailCopyWords` pair and assigns it to the matching object-tail
span at `+0x38..+0x3f`. Retail at `0x80035a3c..0x80035a48` reads both source
words before storing either; KF1's placement loader uses the analogous
aggregate-copy source shape. This one type/source change made the focused
listing SAME. A fresh isolated direct objdiff reports **100%** for all 2020
`.text` bytes, 270 `.data` bytes, and 1016 `.rodata` bytes. All **308 ordered
relocations** match exactly (54 `.text`, 254 `.rodata`). The six adjacent
`game.map_object_reset` helpers remain SAME, as do the two previously exact
functions in `game.map_object`. The RODATA claim covers the 254-entry switch
table, and the initialized DATA claim is the complete 270-byte
`map_object_cell_patterns` array. No new BSS owner was inferred.

The pinned Psy-Q 3.0 `LIBGPU.H` defines `setVector` as the same three ordered
field assignments used in `80036464`; KF1's analogous effect spawner spells
that macro. Replacing the KF2 assignments with it emitted an identical
92.8% focused listing, including the saved-register and publication-order
residues. The spelling does not prove a KF2 original macro use, so the trial
was reverted. Both same-unit exact helpers stayed SAME.
