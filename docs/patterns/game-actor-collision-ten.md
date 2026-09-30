# GAME actor and collision ten

These ten previously non-exact GAME functions connect the collision dispatcher
to actor proximity, motion, targeting, and spatial sound. Each was checked
against retail extent, disassembly, CFG, incoming and outgoing references,
strings, source history, vendored inventory, and current match state. None has
supported Sony/Psy-Q archive attribution.

| GAME address | Decisive retail evidence | Final verdict |
| --- | --- | --- |
| `0x8002aaa4` | The 0xb60-byte collision dispatcher is called by the height wrappers and branches indirectly through candidate table `DAT_8001134c`. | **WIP, unclaimed**; the table owner and indirect targets remain unproved. |
| `0x80039108` | The 0x4c0-byte actor scorer calls geometry, angle, random, and target helpers and branches through candidate table `DAT_80011cd8`. | **WIP, unclaimed**; its scorer cases and table owner remain unresolved. |
| `0x80039c94` | The 0x684-byte player/actor combat helper calls `func_80039c14` repeatedly, then training, target, and angle helpers. | **WIP, unclaimed**; its damage/state field family is incomplete. |
| `0x8003a9f4` | Scans 200 actors, excludes inactive/target type 3/current/masked actors, and tests an alternate Y position for flagged actors. | **Exact, 360/360 bytes strict**; branch-local distance queries reproduce retail's shared call setup. |
| `0x8003ab5c` | Companion scan omits the target-type-3 exclusion but keeps the alternate-position collision query. | **Exact, 344/344 bytes strict**; the same branch-local source form matches. |
| `0x8003ae50` | The 0x4ec-byte actor collision response calls the five-channel dispatcher, height probe, collision snapshot, angle, sine/cosine, and square root helpers. | **WIP, unclaimed**; complete actor response state and branch joins need source evidence. |
| `0x8003b5d0` | Drives actor vertical motion through floor, rising, falling, and trajectory cases with collision and damage calls. | **WIP, 75.66939% strict**; call set and 40/40 CFG blocks agree, while switch order, branch joins, and cache-field scheduling differ. |
| `0x8003c3e0` | Repeatedly steers actor pitch/yaw toward a target Y offset of 1600 and advances a position vector. | **WIP, 96.524826% strict**; 23/23 CFG blocks and direct calls agree; frame and pitch-argument lifetimes differ. |
| `0x8003c614` | The 0xa70-byte actor/effect dispatcher calls vector, animation, spatial sound, and effect helpers and contains an indirect jump. | **WIP, unclaimed**; candidate table `DAT_80011ee8` lacks a complete owner. |
| `0x8003d084` | Clamps a signed actor byte at +`0x4b` to ±12 and adds a scaled `rand` result to its note offset. | **WIP, 91.6% strict**; only the final offset subtraction/shift schedule differs, with 5/5 CFG blocks and the `rand` call aligned. |

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
