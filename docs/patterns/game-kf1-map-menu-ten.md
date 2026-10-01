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
| `80035894` | DIFF 98.7% | Placement loader stores two initial fields around a load-delay slot in different order, then assigns a temporary register differently. KF1 confirms the placement/effect role, not this instruction schedule. |
| `80036190` | DIFF 99.3% | Interaction probe has the same 15 blocks and calls; only an angle-call load/move order differs. Two same-unit controls remain SAME. |
| `80036464` | DIFF 92.8% | Effect spawner uses different saved-register assignments for the object ID and height offset; ordered calls and KF1-supported pool/sequence flow agree. |
| `80036ed4` | DIFF 82.6% | Large action update has 329 retail versus 327 compiled CFG blocks, with 180/180 branches. First substantive difference is the action timer's register/condition schedule; prior natural action-case probes were reverted. |
| `80021c8c` | DIFF 97.2% | Retail reserves 32 stack bytes, compiled C 24. All remaining instructions agree in the focused listing; adjacent `21e00` stays SAME. |
| `8001fb8c` | DIFF 83.5% | Retail reserves 48 stack bytes, compiled C 40. Body instructions and references agree; the difference repeats at saved-register restores. |
| `800228c8` | DIFF 67.6% | Retail uses signed title-byte reads and a two-byte offset walk. Its 24 blocks and 13 branches agree with the current typed card-reader model; a prior natural offset-walk probe regressed and was removed. |

The two menu frame residues provide no evidence for a live local or changed
signature. KF1's menu renderer is structurally different and does not justify
stack padding. None of the ten gained an independently supported C change or
a new strict 100% match. Existing exact siblings and shared source were left
untouched; no repository tests or full build were run.
