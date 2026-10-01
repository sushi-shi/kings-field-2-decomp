# GAME actor and collision ten

These ten related GAME functions connect the collision dispatcher
to actor proximity, motion, targeting, and spatial sound. Each was checked
against retail extent, disassembly, CFG, incoming and outgoing references,
strings, source history, vendored inventory, and current match state. None has
supported Sony/Psy-Q archive attribution.

| GAME address | Decisive retail evidence | Final verdict |
| --- | --- | --- |
| `0x8002aaa4` | The 0xb60-byte collision dispatcher is called by the height wrappers and branches indirectly through candidate table `DAT_8001134c`. | **WIP, unclaimed**; the table owner and indirect targets remain unproved. |
| `0x80039108` | The 0x4c0-byte actor scorer calls geometry, angle, random, and target helpers and branches through candidate table `DAT_80011cd8`. | **WIP, unclaimed**; its scorer cases and table owner remain unresolved. |
| `0x80039c94` | The 0x684-byte player/actor combat helper calls `func_80039c14` eight times, an unresolved slot-18 callback, then training, target, and angle helpers. | **WIP, source claimed, 91.85132% recorded strict**; the focused listing still differs first in stack-argument load/save order and saved-register assignment. |
| `0x8003a9f4` | Scans 200 actors, excludes inactive/target type 3/current/masked actors, and tests an alternate Y position for flagged actors. | **Exact, 360/360 bytes strict**; branch-local distance queries reproduce retail's shared call setup. |
| `0x8003ab5c` | Companion scan omits the target-type-3 exclusion but keeps the alternate-position collision query. | **Exact, 344/344 bytes strict**; the same branch-local source form matches. |
| `0x8003ae50` | The 0x4ec-byte actor collision response calls the five-channel dispatcher, height probe, collision snapshot, angle, sine/cosine, and square root helpers. | **WIP, source claimed, 99.31746% recorded strict**; 58/58 CFG blocks and all direct calls agree. The 99.2% focused listing differs at obstacle-angle register assignment and mask timing; six neighboring functions remain exact. |
| `0x8003b5d0` | Drives actor vertical motion through floor, rising, falling, and trajectory cases with collision and damage calls. | **WIP, 75.66939% strict**; call set and 40/40 CFG blocks agree, while switch order, branch joins, and cache-field scheduling differ. |
| `0x8003c3e0` | Repeatedly steers actor pitch/yaw toward a target Y offset of 1600 and advances a position vector. | **WIP, 99.64539% strict**; 23/23 CFG blocks and direct calls agree; only yaw-error and shifted-numerator registers differ in the focused listing. |
| `0x8003c614` | The 0xa70-byte actor/effect dispatcher calls vector, animation, spatial sound, and effect helpers and contains an indirect jump. | **WIP, source claimed, 23.0% focused**; its candidate table `DAT_80011ee8` and complete dispatch behavior remain unresolved. |
| `0x8003d084` | Clamps a signed actor byte at +`0x4b` to ±12 and adds a scaled `rand` result to its note offset. | **Exact, 100% strict**; the centered random pitch jitter matches the retail listing and ordered relocation, and a fresh focused build keeps both this leaf and its `0x3d0e8` caller `SAME`. |

The two exact actor scans retain typed `VECTOR` paths. Their C source places
the distance query in each branch; the pinned compiler shares the call site
in precisely the retail control flow. A temporary collision-layer predicate
probe aligned `0x8003b5d0`'s two branch directions but left its broader switch
WIP, so the source was not changed. A temporary sound-offset subtraction probe
moved work into the `rand` call delay slot and was also discarded.

Strict GAME matching relinked 145/145 target units. The four adjacent actor
animation functions, three actor-group helpers, three actor-motion helpers,
and the actor spatial-sound wrapper retained their exact listings. Global
edge-check still stops on the three existing unrelated TMD/map-object
`.rodata` addend mismatches. The full `kf build` built PSX; GAME, OPEN, and END
retain the pre-existing unresolved first symbols `InitCARD`, `malloc`, and
`display_buffers`. No repository tests, bank, or commit were run.

A later focused revisit of `0x8003b5d0` kept the source at 66.5% listing
similarity with 40/40 CFG blocks, 21/21 branches, and all three preceding
motion helpers identical. Retail tests motion state `0x20` first and shares
one vertical-step block across three collision outcomes. A temporary explicit
`if` dispatch lowered listing similarity to 62.3%. A temporary shared-step
label aligned the `bnez` after the collision call, but compiled only 39 CFG
blocks with an extra return frontier (67.1% listing). Both experiments were
discarded; the current switch preserves the stronger CFG agreement.

The later `0x80039c94` focused comparison keeps its eight direct curve calls,
the unresolved slot-18 callback, and exact `0x80039c14` sibling. Retail loads
the eleventh stack argument as a halfword, but the existing explicit `u16`
uses already account for that instruction. Changing the formal `amount` type
from `s32` to `u16` in a temporary source enlarged the compiled frame from
168 to 176 bytes and lowered listing similarity from 62.1% to 55.2%; it was
discarded. The baseline has 72 retail versus 71 compiled CFG blocks, with
46/46 branches and seven return-frontier edges.
The one-block CFG difference is at the final linked-actor target-type guard:
retail branches over a redundant `li v0,0x10` and lands directly on the state
store, creating a separate one-instruction block. The probe puts
`move a0,linked` in the guard's delay slot and reaches the `li` before the
same store. Both retain the guarded target-type call and state write; no
source behavior is missing there.
An explicit source `goto` to that state-write join compiled identically to
the existing conditional and was discarded.

The related `0x8003a614` actor-to-player damage gate remains WIP at 81.2%
focused listing similarity. Retail forms camera X and Z through separate
absolute `player_state` pairs, while this source reuses a saved base for Z.
Exact `0x800396c4` uses the same typed `player_state.camera_position` fields
and emits separate pairs, so this difference does not justify splitting the
complete player-state owner into overlapping globals.
