# GAME actor target and initialization campaign

This GAME.EXE campaign follows the shared `actor_state` object at `0x8016b600`,
the 0x7c-byte actor array, and its 0x78-byte target-group records. The active
actor pointer is at state offset `+0x93ac`; the active group is at `+0x93a8`.
The group array begins at `+0x60e0`; the loop at `8003f7ec` bounds it to forty
0x78-byte records. The following 0x2000 bytes remain opaque.
The candidate's byte zero is a type code; its full object and owner remain open.

All exact rows below were compiled with `probe-gcc257-o2-g0`, compared with the
retail GAME.EXE object, and reported as strict `100.000000000%` by
`kf sema --image game match`. `kf try` listing identity alone is not used as
closure. The source code uses C, with address-derived names where semantics
remain unresolved. Psy-Q `rand`, `SquareRoot0`, and vector APIs are callees,
not reconstructed game functions.

| GAME VA | Verdict | Retail evidence and remaining work |
| --- | --- | --- |
| `80038dc4` | exact | Copies group-indexed fields into an actor; no calls. |
| `80038e38` | exact | Initializes actor fields, calls group copy, `rand`, and placement helper `8002b73c`. |
| `80038f20` | exact | Scans 200 actors and calls unresolved callback-table slot 19 through `state_8017d118.active_table`, then sets the home position. The source-backed listing was certified strict `208/208`; the indirect destination remains unresolved. |
| `80038ff0` | exact | Resets placement fields, calls home-position and group initializers. |
| `800390d0` | exact | Sets a target pointer and current/previous target-type bytes. |
| `80039108` | WIP | Candidate scorer: 0x4c0 bytes, computed dispatch and unresolved indirect control, with many outgoing references. Candidate kind and dispatch-table ownership need recovery. |
| `800395c8` | exact | Scans sixteen candidates, retains the highest signed score and calls the target setter when needed. |
| `800396c4` | exact | Measures actor-to-player X/Z distance and passes it to the best-target scan. |
| `80039710` | exact | Finds a candidate type in sixteen non-null group slots. |
| `80039758` | exact | Uses actor byte `+2` as a group index, finds a target type and applies the setter. |
| `800397a8` | exact | Clears current actor target, sets byte `+0x0f` to `0xff`, then reselects by player distance. |
| `800397d8` | exact | Sets current actor byte `+0x0c` and clears phase unless the input is `0xff`; byte meaning remains unresolved. |
| `80039804` | exact | Same byte/phase update only when the value changes; byte meaning remains unresolved. |
| `8003983c` | WIP | 0x31c-byte actor lifecycle update with collision/distance, random choice, and target-selection branches. Shared group flag and geometry field meanings remain open. |
| `80039c14` | exact | Fixed-point quadratic helper called eight times by `80039c94`; parameter meanings remain unresolved. |
| `80039c94` | WIP, source-backed, 50.3% focused | 0x684 bytes/72 retail blocks; typed actor/group curves, target selection, player training, vector math, and one unresolved `jalr`. Retail uses target-group byte +2 as the unlinked motion divisor; current C has 71 blocks and 46/46 branches. |
| `8003a318` | WIP, `97.5%` focused listing; strict pending | Typed 200-actor magic scan calls distance helpers and `80039c94`. Retail halfword loads prove `falloff` and `effect_flags` are `u16`; delaying amount extraction and actor-base setup until after the flag branch yields 26/26 CFG blocks and 13/13 branches. Only a two-register argument-load swap remains. |
| `8003a614` | WIP | 0x164 bytes, distance/angle tests and `800248a8`; caller and geometry types need recovery. |
| `8003a778` | exact | Scans active actors for the best target by distance and angular score; strict `100.000000000%`. |
| `8003a9f4` | exact | 0x168-byte actor-distance scan with the target-type filter; `kf sema addr` reports strict `100.000000000%` in the typed contiguous unit. |
| `8003ab5c` | exact | 0x158-byte sibling scan without that filter; `kf sema match` reports strict `100.000000000%`, and the current focused listing remains SAME. |
| `8003acb4` | exact | Binds active actor/group and optional linked actor/group; clears all five state fields for null actor. |
| `8003ae50` | WIP | 0x4ec bytes/58 blocks, terrain queries, angle math, square roots and trigonometry; actor motion fields remain open. |
| `8003b33c` | exact, player owner | Terrain-linked actor motion with four calls to `8002b9d4`; strict `100.000000000%` in the consolidated actor-motion collision unit. |
| `8003b520` | exact, player owner | Actor trajectory helper calling `80015bc8`; strict `100.000000000%` in the consolidated actor-motion collision unit. |

The game-owned verdict is supported by actor/group BSS references, actor field
offsets, and confirmed game callers. The large WIP bodies are left in their
address-derived inventories instead of making C claims from adjacency alone.
Retail `jalr` targets remain unresolved until their table/value chains are
proved.

## Connected placement, motion, and behavior wave

The following twenty-five GAME functions were selected through confirmed
actor/terrain calls and shared actor data. `exact` means a strict per-function
objdiff report of `100.000000000%`; WIP rows remain unclaimed unless a C
probe and its measured residue are stated. The terrain query state around
`0x801e8d40` has no established owner, so its consumers keep their
address-derived identities. Direct `rand` and sound calls use Psy-Q and the
game audio API; no body in this wave is classified as vendored.

| GAME VA | Verdict | Retail evidence and open issue |
| --- | --- | --- |
| `8002b604` | WIP | Terrain query wrapper calls `8002a988` then `8002aaa4`; result loads the unresolved terrain state at `801e8d50`. |
| `8002b67c` | WIP | Converts X/Z into a terrain cell, sets terrain globals, calls `8002aaa4`; shared `801e8d40` state and terrain record extent need ownership. |
| `8002b73c` | WIP | Bounded 80-by-80 terrain-grid edit called by actor initialization and behavior; grid datum at `801c7540` needs a typed owner. |
| `8002b9d4` | WIP | Multi-mode terrain/motion query calls actor distance probes `8003a9f4` and `8003ab5c`; terrain globals and stack-argument contract remain open. |
| `80038d04` | WIP | Actor home placement calls `8002b67c`, clamps Y, and reads the terrain result at `801e8d4c`; that datum remains unowned. |
| `8003b5bc` | exact | Writes `0x60` to current actor byte `+0x0d`; BSS HILO target is `actor_state+0x93ac`. |
| `8003b5d0` | WIP | Terrain/collision placement body calls `8002b604`, `8002b9d4`, `800248a8`; large branch family and terrain result contract remain open. |
| `8003b9a4` | exact, player owner | Actor motion with `SquareRoot0`, value approaches, and `8003ae50`; strict `100.000000000%` in the consolidated actor-motion unit. |
| `8003bae4` | exact, player owner | Actor forward-vector movement using angle/vector helpers and `8003ae50`; strict `100.000000000%`. |
| `8003bba0` | exact | Accelerates bounded actor yaw velocity, snaps on crossing, and calls shortest/modular angle helpers. |
| `8003bcd0` | exact | Current-actor wrapper forwards angle/range arguments to `8003bba0` and movement arguments to `8003bae4`; parameter meanings remain provisional. |
| `8003bd40` | WIP | Calls X/Z angle, tolerance, and `8003bcd0`; target geometry layout remains open. |
| `8003be38` | exact, player owner | Three-axis forward-vector movement and terrain query; strict `100.000000000%`. |
| `8003bf74` | exact | Current-actor Euler-angle wrapper calls `8003bba0`, `angle_approach`, and `8003be38`; parameter meanings remain provisional. |
| `8003c000` | WIP | Vector transform helper used by actor behavior and effect dispatch; calls `80034344` and `vector_rotate_yxz`, with cross-system owner unresolved. |
| `8003c10c` | WIP | Vector rotation helper called by effect dispatch and `8003247c`; actor/effect ownership remains unresolved. |
| `8003c220` | WIP | Animation phase and state transition code calls the three matched animation/byte helpers; state meaning remains open. |
| `8003c3e0` | WIP | Actor vector/distance helper called repeatedly by `8003c614`; geometry and return contract remain open. |
| `8003c614` | WIP | `0xa70`-byte actor behavior body with confirmed pool and initializer calls, and an unresolved jump table/indirect jump. |
| `8003d084` | WIP, 91.600000000% strict | Actor signed byte `+0x4b` controls a clamped random sound note offset; CFG and calls agree, but GCC reassociates the final `-2` across the shift (`kf try`: 92.6% listing similarity). |
| `8003d0e8` | exact | Actor target byte `+4` selects range/default spatial sound calls; both paths use `8003d084`. |
| `8003d184` | WIP | `0x248c`-byte actor behavior dispatch calls movement, target, animation, sound, and terrain helpers; two `jalr` sites and jump tables remain unresolved. |
| `8003f610` | exact | Iterates 200 actor records, binds each, updates active actors, and follows flagged groups to map-object positions; strict 100%. |
| `8003f7ec` | WIP, 85.862070000% strict | Fixes up sixteen signed byte-offset target references in each of at most forty groups; CFG and referents agree, with two instruction-order residues (`kf try`: 93.8% listing similarity). |
| `8003f860` | WIP | Loads sixteen-byte actor entries into the 200-slot actor pool, copies group defaults, and sets home position; source record layout and caller contract remain open. |

## Connected resource, placement, and map-object survey

These twenty-five GAME functions were inspected through the actor-group loader's
confirmed calls, terrain queries, and map-resource dependencies. Three already
have strict exact claims in other campaigns; they are recorded here as evidence,
not claimed by this actor campaign. The remaining functions retain WIP identities.
The resource offsets at `801b2244` and `801b5a44` have no admitted BSS binding;
their common containing allocation and extent must be proved before a C datum is
claimed. An indirect `jr` or `jalr` below remains unresolved.

| GAME VA | Verdict | Retail evidence and open issue |
| --- | --- | --- |
| `800160e8` | WIP | Called by map loader `80016820`; writes several `801a` and `8017` state words. The shared load-state record needs an owner. |
| `80016820` | WIP | Map-load coordinator calls CD reads, actor group fixup `8003f7ec`, actor load `8003f860`, map expansion `80034818`, map decoder `80035894`, and resource decode `800489ac`; contains unresolved indirect control. |
| `800248a8` | WIP | Actor/terrain callers reach a player-combat body with `rand`, eight damage-component calls, and an unresolved indirect dispatch; player state is separately owned. |
| `8002a988` | WIP | Terrain setup writes the unowned `801e8d40` query state and reads a grid near `801c`; collision owner handles the helper. |
| `8002aaa4` | WIP | Large terrain-query switch with unresolved indirect branch and many writes to `801e` state; query record and jump table remain open. |
| `8002b7f8` | exact, collision owner | Calls `8002a988` and `8002aaa4`; strict `100.000000000%` in `game.collision_probe_offset`. |
| `8002b874` | WIP | Uses `801e` terrain query and `8017` actor/group state; shared terrain-result layout remains open. |
| `80034f90` | WIP, map owner | Map-placement transform calls Psy-Q `rcos` and `rsin`; downstream of `80035894`. |
| `80035194` | WIP, map owner | Map placement decoder reads `801c` data and is called twice by `80035894`; record layout remains open. |
| `80035590` | WIP, map owner | Clears object flags and motion halfwords and sets three scale halfwords to `0x1000`; used by map/resource decoders. |
| `800356ac` | WIP, map owner | Map placement helper reads `801a` and `801c` state; called by `80035894`. |
| `80035894` | WIP, map owner | `0x7e4`-byte map decoder called by `80016820`; uses placement helpers, terrain edit, `memset`, and an unresolved indirect jump. |
| `80036078` | WIP, map owner | Proximity helper reads `8017`/`8018` state and calls `vector_distance_to_point`; object identity remains open. |
| `800363bc` | exact, map owner | Starts a map-object action when idle; strict `100.000000000%` in `game.map_object`. |
| `800363dc` | exact, map owner | Acquires from the map-object effect pool; strict `100.000000000%` in `game.map_object`. |
| `80036464` | WIP, map owner | Uses pool acquire, object reset, `rand`, and action start; has an unresolved jump-table dispatch. |
| `800365d8` | WIP, map owner | Uses pool acquire and reset, `rand`, `rsin`, `rcos`, and action start; placement argument meanings remain open. |
| `800366fc` | WIP, map owner | Scans map-object state at `8017`; effect ownership and object fields need review. |
| `80040308` | WIP, effect owner | `0x13e4`-byte effect dispatch uses pool, sound, `rand`, and collision probe `8002b7f8`; jump table unresolved. |
| `800483d8` | WIP | Expands ten `u16` offsets from `801b5a44` into caller pointer slots, with `0xffff` as null and base `801b2244`. BSS object extent is unbound. |
| `80048428` | WIP | Adds a base pointer to field `+8` of selected variable-length records at `801b2244`; record bounds and BSS owner remain open. |
| `80048498` | WIP | Inverse of `800483d8`: compresses ten caller pointers into `u16` offsets at `801b5a44`, with null as `0xffff`. |
| `800484e4` | WIP | Inverse of `80048428`: subtracts the base from field `+8` in selected variable-length records. |
| `80048554` | WIP | Serializes actor/group and map-object state into a 3152-byte stack buffer, converts resource offsets, then allocates/copies the selected resource block; complete format is open. |
| `800489ac` | WIP | Reads a selected resource block, updates actor and map-object records, and dispatches through a 16-way jump table; resource format and table owner remain open. |

## Actor-connected vector and interpolation wave

This fourth set follows proven calls from actor scoring, behavior, and motion
into shared fixed-point geometry, then records the direct caller boundary.
The six small helpers at `154fc`–`1586c` and `15ce0` are game C with strict
`100.000000000%` objdiff results. The similarly shaped Psy-Q `rand`, `rsin`,
`rcos`, and `SquareRoot0` functions remain library callees; these helper bodies
are absent from the supplied SDK headers and vendored inventory. The KF1
vector-math source supplied a comparison for adjacent angle functions, while
the KF2 instructions and call sites determine the new signatures. The larger
direct callers are recorded as boundaries, not claimed across other owners.

| GAME VA | Verdict | Retail evidence and open issue |
| --- | --- | --- |
| `800154fc` | exact | Converts signed X/Y/Z displacement to Euler pitch/yaw through `vector_xz_to_angle` and fixed X/Z length; twelve proven callers include actor behavior. |
| `80015574` | exact | Directed interval overlap predicate called by actor candidate scorer `80039108`. |
| `80015698` | WIP | Three-axis bounded distance query calls `SquareRoot0` and returns an unusual `0xff676981` sentinel on failure; called twice by actor motion `8003a318`. Sentinel meaning and height contract remain open. |
| `800157ac` | exact | Scales the sum of two Psy-Q `rand` results; six calls from actor target scoring. |
| `800157f8` | exact | Scales the centered sum of two `rand` results; actor behavior `8003d184` is a proven caller. |
| `8001584c` | exact | Q12 scalar interpolation; three calls each from actor behavior `8003c614` and four from `8003d184`, among cross-system callers. |
| `8001586c` | exact | Shortest-path wrapped Q12 angle interpolation; two calls from actor motion `8003c3e0`. |
| `800158b4` | WIP, 98.000000000% strict | Interpolates nine signed halfword deltas into unsigned halfwords; remaining difference is register allocation and pointer-increment scheduling. The collision and graphics callers are proven. |
| `80015918` | WIP | `0x2b0`-byte fixed-point trajectory solver with multiple divisions and `SquareRoot0`; called by actor behavior `8003c614` and `80015bc8`. |
| `80015bc8` | exact, player owner | Actor trajectory wrapper combines X/Z length, fixed-point solver, `rcos`, and `rsin`; strict `100.000000000%`. |
| `80015ce0` | exact | Adds a scaled `SVECTOR` to a `VECTOR`; proven actor behavior calls at `8003c5d0` and `8003cfd0`. |
| `80024498` | WIP, player owner | Player combat/trajectory body calls `800154fc`, vector helpers, `rand`, and an unresolved indirect branch. |
| `80024ca4` | WIP, player owner | Player motion/combat caller of bounded distance `80015698`, `800253ac`, and `800248a8`. |
| `8002665c` | WIP, player/menu owner | `0xbd0`-byte caller of displacement-to-Euler `800154fc`; broad owner and output fields remain open. |
| `8002985c` | WIP, map/collision owner | `0x9dc`-byte, two-fragment body calls both scalar and angle interpolation, terrain/collision helpers, and has unresolved indirect control. |
| `8002bdbc` | exact, collision owner | Calls nine-halfword interpolation and Q12 scalar interpolation; strict `100.000000000%` in `game.collision_rows`. |
| `80031850` | WIP, graphics owner | `0x53c`-byte draw/transform body calls both matrix and scalar interpolation alongside Psy-Q graphics helpers. |
| `80036b68` | WIP, map owner | Calls map-object reset and sound, `vector_rotate_yxz`, and three Q12 scalar interpolations. |
| `8004195c` | WIP, effect owner | `0x1b8`-byte effect caller of displacement-to-Euler `800154fc`; effect fields remain open. |
| `80041b14` | WIP, effect owner | Calls fixed three-axis length, scalar interpolation, and `8004177c`; effect owner needs field typing. |
| `8004212c` | WIP, effect owner | Calls centered random scalar `800157f8`, effect dispatch `80040308`, and `rand`. |
| `80045fd4` | exact | Interpolates three position words and two angle halfwords through the matched Q12 helpers into a partial scene-pose view; strict `100.000000000%` in the later scene wave. |
| `8004678c` | WIP | `0xc54`-byte scene/gameplay controller calls bounded distance, map/actor functions, and interpolation wrapper `80045fd4`; two indirect transfers remain unresolved. |
| `800474c4` | WIP | Calls collision rows, three scalar interpolations, and graphics helpers; object owner remains open. |
| `800475d8` | WIP | `0x6c0`-byte scene controller calls map-object pool/reset, interpolation wrapper and angle interpolation; record and callback ownership remain open. |

## Scene-event and actor phase wave

This fifth set follows the scene controller at `4678c`, its actor-phase and
map-object adapters, and the direct geometry, memory, and SDK callees used by
the same path. It contains 25 inspected functions. Three newly reconstructed
game helpers are strict exact; the actor phase helper remains a source-backed
WIP. The two trigonometric callees are verified Psy-Q library functions, not
game-source claims. The scene/event records around `801b2144` and the counter
table at `800aa5e8` still lack a proven owner and extent.

| GAME VA | Verdict | Retail evidence and open issue |
| --- | --- | --- |
| `80045e5c` | WIP | Computes sine/cosine and calls terrain query `8002b604`; the terrain result at `801e8d44` has no modeled owner. |
| `80045f20` | WIP | Rotates a local vector with `vector_rotate_yxz`, then adds `player_state.camera_position` at `801985a8`. |
| `80045fd4` | exact | Optional three-word position and two-angle Q12 interpolation into `KfScenePoseView`; reviewed five call relocs and strict `100.000000000%`. |
| `800460a0` | WIP, 99.268295000% strict | Sets actor byte `+0x0c` and animation phase, advances even steps toward the target while calling `800335a0`; call graph and CFG agree, but step and half-step saved registers are interchanged. |
| `80046144` | exact | Finds an `f2` marker in a target candidate's byte stream, or returns fallback byte `+0x10`; strict `100.000000000%`. |
| `800461a0` | WIP | Scans `f1`/`fe` stream controls and updates target-candidate bytes `+0x10` and `+0x13`; its `801b2144` state remains unowned. |
| `800462bc` | WIP | Dispatches candidate type `0x70` through a jump table, calling `461a0`, `46144`, and `460a0`; table format and `801b2144` state remain open. |
| `80046700` | exact | Acquires a map object from the effect range, stores its pool-relative slot in event byte `+0x39`, and initializes object fields; reviewed pool BSS referent and strict `100.000000000%`. |
| `8004678c` | WIP | `0xc54`-byte scene controller calls actor, map-object, pose, and geometry helpers; two indirect transfers and event object extent remain unresolved. |
| `800473e0` | WIP | Tests and decrements a byte in the `800aa5e8` counter table; its outside-load storage extent and owner remain unproved. |
| `80047434` | WIP | Uses the same counter table and notification path, with unresolved indirect control. |
| `800474c4` | WIP | Combines collision rows, three scalar interpolations, and frame calls; destination record remains open. |
| `800475d8` | WIP | Scene loop calls map-object, pose, angle, and controller helpers with unresolved event record ownership. |
| `80047c98` | WIP | Connects terrain trigonometry, actor distance/phase, scene controllers, and resource dispatch; one indirect target remains unresolved. |
| `800482f8` | WIP | Clears event state through `repeat_store_word` and initializes arena blocks; `801b2144` storage extent remains open. |
| `80015104` | exact, math owner | `vector_rotate_yxz`, proven callee of `45f20`; strict `100.000000000%` in `game.vector_math`. |
| `80017228` | exact, memory owner | `repeat_store_word`, direct callee of `482f8`; allocator worker matched it at strict `100.000000000%` in `game.memory`. |
| `800175e8` | exact, memory owner | `memory_arena_initialize_blocks`, direct callee of `482f8`; strict `100.000000000%` in `game.memory`. |
| `8002b604` | WIP | Calls terrain probes `2a988` and `2aaa4`, then returns the unmodeled result word at `801e8d50`; called by `45e5c`. |
| `800335a0` | WIP | `0x3f4`-byte frame/update callee of `460a0` and scene controllers; four-argument signature and ownership remain open. |
| `80034e10` | WIP, menu owner | TIM transition loader called by `462bc`; source-backed strict result is `99.114586000%`. |
| `80036190` | WIP, map owner | Typed map-object proximity/facing search called by `4678c`; source-backed strict result is below exact. |
| `800368b4` | WIP, map owner | Dispatches map-object action through a 21-entry jump table and may clear byte `+0x38`; indirect target set remains unresolved. |
| `8005dda4` | vendored | `rsin` is an exact Psy-Q 3.0 `LIBGTE.LIB` archive match; called by `45e5c`. |
| `8005deac` | vendored | `rcos` is an exact Psy-Q 3.0 `LIBGTE.LIB` archive match; called by `45e5c`. |

## Actor-event controller call boundary

This sixth set follows confirmed direct calls from `462bc`, `4678c`, `475d8`,
and `47c98` into the game-owned frame, menu, player, audio, CD, resource, and
math functions they drive. All 25 rows have a current verdict. The four-byte
graphics setter is a new strict exact reconstruction. The event controller's
menu choices are still large unclaimed bodies, and the scene-coordinate BSS
read by `36ad8` is still unbound.

| GAME VA | Verdict | Retail evidence and open issue |
| --- | --- | --- |
| `80013f84` | exact, audio owner | Spatial-range sound playback, directly called by `4678c`; strict `100.000000000%`. |
| `800144b8` | WIP, audio owner | VAB service called by `4678c` and `475d8`; source-backed strict result `94.873420000%`. |
| `80014864` | exact, math owner | Signed value approach used by `475d8`; strict `100.000000000%`. |
| `80014ac0` | exact, math owner | Angle-to-forward-XZ helper used by `4678c`; strict `100.000000000%`. |
| `80015148` | exact, math owner | Scales an XZ vector by a signed halfword; direct `4678c` callee, strict `100.000000000%`. |
| `800152ac` | exact, math owner | Angle-tolerance predicate used by `475d8` and `47c98`; strict `100.000000000%`. |
| `80015318` | exact, math owner | XZ displacement-to-angle helper called by `47c98`; strict `100.000000000%`. |
| `80016260` | WIP | `0x55c`-byte scene transition body calls audio sequence, resource `48554`, CD yield, and global reset `16820`; its event/resource state remains unmodeled. |
| `80016f4c` | WIP, CD owner | Stream service used by `4678c` and `475d8`; handles critical section, image loading, and CD state with no source claim yet. |
| `8001779c` | exact, memory owner | `cd_request_yield`, direct callee of `4678c` and `16260`; strict `100.000000000%`. |
| `80017c2c` | exact, CD owner | Waits for CD request idleness in the `4678c` path; strict `100.000000000%`. |
| `8001ceb8` | WIP, menu boundary | `0x178`-byte two-choice screen called by `462bc`, with menu frame, window, input, and choice callbacks; no source claim. |
| `8001d6a8` | WIP, menu boundary | `0x228`-byte list screen called by `462bc`; item model, repeated draw/present loop, and input behavior need source reconstruction. |
| `8001d8d0` | WIP, menu boundary | `0x394`-byte list screen called by `462bc`, sharing menu list/render flow with `1d6a8`; no source claim. |
| `8001dc64` | WIP, menu boundary | `0x16c`-byte window/choice screen called by `462bc`; no source claim. |
| `80023984` | exact, player owner | Recalculates combat stats after the `4678c` event path; strict `100.000000000%`. |
| `800251f0` | exact, player owner | Clears player motion in the `462bc` and `4678c` paths; strict `100.000000000%`. |
| `80028fa8` | exact, player owner | Releases a transition pool around `462bc` dispatch; strict `100.000000000%`. |
| `800293d4` | exact, player owner | Enters reaction state from `47c98`; strict `100.000000000%`. |
| `800314d4` | exact | Sets graphics runtime bytes `+0x14cc1` through `+0x14cc4`; four confirmed callers include `4678c`, and reviewed BSS relocs yield strict `100.000000000%`. |
| `800321d8` | WIP, resource owner | Queues TMD read from `4678c`; source-backed strict result `98.435900000%` with unresolved resource allocation boundary. |
| `80036ad8` | WIP | Checks whether `player_state.camera_position` at `801985a8` falls in a caller-provided rectangle. |
| `80036e24` | WIP, audio owner | Frame loop squares phase for RGB, services VAB/CD, and renders camera pose; source-backed strict result `98.863640000%`, saved-register assignment residue. |
| `80045e18` | exact, audio owner | Fixed sound `0x40` wrapper called by `4678c`; strict `100.000000000%`. |
| `80045e3c` | exact, audio owner | Sound-at-volume-100 wrapper called by `4678c`; strict `100.000000000%`. |

## Animated map object and scene reset wave

This seventh set follows the `36ed4` scene controller's animated map-object
path, and the `16820` and `48554` reset/resource path reached by event
transitions. All 25 inspected functions below are game-owned; SDK calls stay
outside this count. The animated vertex-to-world helper is newly strict exact.
The unresolved bodies cross model/keyframe data, terrain scratch storage, and
effect dispatch, so those ownership boundaries remain WIP.

| GAME VA | Verdict | Retail evidence and open issue |
| --- | --- | --- |
| `80034344` | WIP | Resolves model/keyframe data through `resource_registry_get`, `animation_select_keyframe`, and sparse-vertex lookup; directly supplies local vertex to `369b8`. |
| `800369b8` | exact | Gets a model vertex, applies map-object Q12 scale, YXZ rotation, and object position; reviewed two call relocs and strict `100.000000000%`. |
| `80036ed4` | WIP | `0x1df4`-byte scene/map-object controller calls `369b8` twice and spans many object actions; original case ownership and indirect state remain open. |
| `800140dc` | exact, audio owner | Two-argument sound playback called by the `36ed4` scene path; strict `100.000000000%`. |
| `800147a0` | exact, math owner | Angle approach called by `36ed4`; strict `100.000000000%`. |
| `80014a08` | exact, math owner | Five-argument angle-velocity step called by `36ed4`; the corrected early return matches strict `100.000000000%`. |
| `80014e84` | exact, math owner | Y rotation matrix setter called by `36ed4`; strict `100.000000000%`. |
| `80025234` | exact, player owner | Synchronizes player position to map height in the `36ed4` path; strict `100.000000000%`. |
| `8002b9d4` | WIP, collision owner | `0x244`-byte terrain probe called by `36ed4`; uses `801e` scratch words and actor/player distance helpers with no complete storage model. |
| `80035504` | exact, map owner | Plays spatial sound at a map object's position in the `36ed4` path; strict `100.000000000%`. |
| `800355d8` | exact, map owner | Sets a map-object property from `36ed4`; strict `100.000000000%`. |
| `8003fb94` | WIP, source-backed | Dispatches actor/effect targeting from `36ed4` with a 13-argument call to `39c94`; typed source is strict `87.791046000%`. |
| `80041e0c` | WIP, effect owner | Reads effect state at `801e` and calls effect dispatcher `40308`; source and storage owner remain open. |
| `80013ae4` | exact, audio owner | Starts sequence playback from `16260`/`16820` reset flow; strict `100.000000000%`. |
| `800146d0` | exact, audio owner | Queues VAB stream during `16820` reset; strict `100.000000000%`. |
| `80016ee0` | exact, CD owner | Queues map stream read during `16820` reset; strict `100.000000000%`. |
| `800171c8` | exact, memory owner | Copies resource words for `16820` and `48554`; strict `100.000000000%`. |
| `8001777c` | exact, memory owner | Frees memory during `16820` reset; strict `100.000000000%`. |
| `80017ef4` | exact, CD owner | Queues archive entry read from `16820`; strict `100.000000000%`. |
| `800346b0` | exact, graphics owner | Releases a graphics pool record during `16820`; strict `100.000000000%`. |
| `80031fa0` | exact, resource owner | Resolves model resource registry data for `34344`; strict `100.000000000%`. |
| `80033b34` | exact, animation owner | Selects the keyframe used by `34344`; strict `100.000000000%`. |
| `80033ff4` | exact, animation owner | Finds a sparse vertex used by `34344`; strict `100.000000000%`. |
| `80017608` | WIP, memory owner | Arena block allocator called by `48554`; source-backed strict result `96.934784000%`. |
| `800176c0` | exact, memory owner | Releases an arena block from `48554`; strict `100.000000000%`. |

## Sparse animation and render call boundary

This eighth set follows the animated model vertex path from `34344` into the
neighboring sparse animation functions, then follows `34070` into its confirmed
render callers and TMD callees. All 25 functions are game-owned. The existing
17 exact units remain strict exact in the refreshed GAME report. The two
unclaimed interpolation bodies use a packed delta stream and a Psy-Q
`ScaleMatrix` call over a temporary whose complete source type is not yet
proved, so they remain WIP rather than speculative C claims.

| GAME VA | Verdict | Retail evidence and open issue |
| --- | --- | --- |
| `80033bfc` | exact, animation owner | Expands sparse vertex records for `34070`; strict `100.000000000%`. |
| `80033cc0` | exact, animation owner | Decodes sparse vertex updates for `34070`; strict `100.000000000%`. |
| `80033d3c` | WIP | Applies packed vertex deltas, skip records, and `ScaleMatrix` scaling; called by `34070`, but temporary matrix/vector source type remains open. |
| `80034070` | WIP | Coordinates a pooled animated vertex buffer, selects keyframes, decodes sparse vertices, and calls `33d3c`; animation record/data extents remain open. |
| `800345e4` | exact, asset owner | Reads selected TMD object vertex count in the neighboring animation/pool band; strict `100.000000000%`. |
| `80034644` | exact, pool owner | Resets the animation/graphics pool used by `34070`; strict `100.000000000%`. |
| `80034674` | exact, pool owner | Marks a pool record allocated in the same record family; strict `100.000000000%`. |
| `800346f8` | exact, pool owner | Releases all pool records when `34070` allocation fails; strict `100.000000000%`. |
| `80034764` | exact, pool owner | Releases stale records in the shared pool; strict `100.000000000%`. |
| `800347d0` | exact, pool owner | Allocates the pool record requested by `34070`; strict `100.000000000%`. |
| `80031024` | exact, render owner | Walks 14 initialized 36-byte model rows and a full-width `0xff` sentinel at `80066888`, sets GTE matrices, and calls `34070`; strict `100.000000000%` in `game.render_map_cell`. |
| `800316c8` | WIP, render boundary | `0x188`-byte caller of `34070` and TMD projection mode helpers; no source claim. |
| `80031d8c` | WIP, render boundary | `0x214`-byte caller of `34070`, TMD projection, and matrix helpers; no source claim. |
| `8001771c` | exact, memory owner | Checked allocator used by `34070` for the vertex buffer; strict `100.000000000%`. |
| `8002d4a8` | exact, display owner | Sets current TMD vertices after `34070` interpolates them; strict `100.000000000%`. |
| `8002d4b8` | exact, display owner | Selects TMD object vertices for `34070` and its render callers; strict `100.000000000%`. |
| `80033afc` | exact, asset owner | Selects asset registry entry for `34070` and its render callers; strict `100.000000000%`. |
| `8002d0a4` | exact, display owner | Sets near fog distance in the `31024`/`316c8` render path; strict `100.000000000%`. |
| `8002d484` | exact, display owner | Gets current TMD object for the render callers; strict `100.000000000%`. |
| `8002d918` | exact, graphics owner | TMD projection mode helper called by `316c8`; strict `100.000000000%`. |
| `8002dbd8` | exact, graphics owner | Transforms TMD vertices for `31024`; strict `100.000000000%`. |
| `8002e4dc` | WIP, render boundary | `0x704`-byte projected-model caller shared by `31024` and `316c8`; data/primitive ownership remains open. |
| `80014f64` | exact, math owner | Builds YXZ rotation matrix for `31d8c`; strict `100.000000000%`. |
| `8002dd28` | exact, graphics owner | Projects transformed TMD vertices for `31d8c`; strict `100.000000000%`. |
| `8002ebe0` | WIP, render boundary | `0x5b4`-byte downstream model-draw caller of `31d8c`; primitive and GTE state remain open. |

## Map-grid scan and render-frame call boundary

This ninth set follows the confirmed `335a0` frame calls through the map-grid
scan and renderer, then the directly reached lighting, animation, and TMD
helpers. All 25 entries are GAME functions; Psy-Q matrix and color routines
called by them are excluded. The three contiguous map-cell claims now share
`game.render_map_cell`. The typed render-grid state at graphics `+0x14e1c`
contains the signed scan origin, fog threshold, and 24-by-24 layer masks;
retail derives the mask base by adding 12 to that state's address. This
restored an exact byte and relocation match for `30f5c`, while the two earlier
functions retain their source-backed WIP verdicts.

| GAME VA | Verdict | Retail evidence and open issue |
| --- | --- | --- |
| `80030c18` | WIP, source-backed | Renders a five-byte map-cell layer through TMD and collision-light rows; strict `96.521736000%`, with entry setup order residue. |
| `80030de4` | WIP, source-backed | Uses 800-byte map rows and two five-byte layers, relative camera position, and calls `30c18`; strict `89.521280000%`, with matching 10/10 CFG blocks and remaining byte-mask/instruction scheduling differences. |
| `80030f5c` | exact | Selects TMD slot zero and scans a 24-by-24 mask window, calling `30de4` for active in-range cells; strict `100.000000000%` after contiguous state typing and source merge. |
| `80031024` | exact, render owner | Walks 14 initialized 36-byte model rows and a full-width `0xff` sentinel at `80066888`, sets GTE matrices, and calls `34070`; strict `100.000000000%` in `game.render_map_cell`. |
| `800335a0` | WIP | Frame caller orders display, map-grid scan, placed-object update, graphics overlays, and present; large body and loaded display state remain unclaimed. |
| `8002c670` | WIP | Establishes signed scan origin at graphics `+0x14e1c/+0x14e20`, then calls mask builders `2c290` and `2c424`; wider scene state remains open. |
| `8002c290` | WIP, source-backed | Mask-cell update called eight times from `2c670`; typed `801b5a70` scan state is config-only because source BSS placement remains unproved. Strict `71.326740000%`, 15/15 CFG blocks. |
| `8002c424` | WIP | Paired mask builder repeatedly called from `2c670`; the same `801b5a70` state owner is unresolved. |
| `8002cf40` | WIP, effect owner | Eight-row graphics update at `+0x14cd4` calls Psy-Q `LoadImage`; typed source exists at strict `92.910110000%`. |
| `800312f4` | exact, graphics owner | Sliding-panel renderer called by `335a0`; strict `100.000000000%`. |
| `80031384` | exact, graphics owner | Paired sliding-panel renderer called by `335a0`; strict `100.000000000%`. |
| `80031414` | exact, graphics owner | Graphics color draw helper reached by `335a0`; strict `100.000000000%`. |
| `800314fc` | exact, collision owner | Draws the adjacent collision channel in the `335a0` frame path; strict `100.000000000%`. |
| `800316c8` | WIP | Scene TMD renderer called by `335a0`, invokes animation `34070` and projected-model draw `2e4dc`; matrix calls are SDK-owned. |
| `8003247c` | WIP | Large map-placed/resource update after `30f5c`, calls mask queries, renderer `31850`, and actor/effect helper `3c10c`; control/data ownership remains broad. |
| `80033140` | exact, notification owner | Draws notification rows from `335a0`; strict `100.000000000%`. |
| `80033284` | exact, notification owner | Notification enqueue helper reached by `335a0`; strict `100.000000000%`. |
| `80034674` | exact, pool owner | Marks a graphics pool record allocated before the scene render; strict `100.000000000%`. |
| `80034764` | exact, pool owner | Releases stale graphics pool records after present; strict `100.000000000%`. |
| `8002d4f4` | exact, display owner | Camera/TMD setup before the `335a0` frame path; strict `100.000000000%`. |
| `8002f808` | WIP, map render owner | Prepared map primitive path called by `30c18`; source-backed strict `62.168440000%`. |
| `8002ff5c` | WIP, map render owner | Builds a prepared TMD primitive buffer for `30c18`; parent owns reconstruction. |
| `8002ddb4` | WIP | `31850` projected TMD draw with four `NormalClip` paths, depth color, and primitive insertion; SDK calls identified, game packet source remains open. |
| `8002e4dc` | WIP | Projected-model draw shared by `31024`/`316c8`, with four clipped packet paths and SDK color calls; primitive owner remains open. |
| `80034070` | WIP | Pooled animation vertex interpolation used by `31024`/`316c8`; packed sparse deltas and temporary buffer type remain open. |

## Render-model rows, mask state, and map placement

This tenth set follows the confirmed `335a0` render-frame calls, the shared
`80066888` model rows, the `3247c` map scan, and the two callers of `34f90`.
Every entry is a GAME function; Psy-Q `rcos`, `rsin`, matrix, and color APIs
remain library boundaries. `render_model_rows` now owns exactly `0x21c` loaded
bytes: 14 live 36-byte rows plus one complete 36-byte `0xff` sentinel. The
adjacent `80066aa4..80066ab4` bytes remain separate. Retail direct stores from
`2c670` establish the observed 32-byte `801b5a70` scan state, but its source
allocation class is unresolved, so it remains a typed config-only BSS symbol.

| GAME VA | Verdict | Confirmed relationship and result |
| --- | --- | --- |
| `8002c290` | WIP, source-backed | Eight calls from `2c670`; checks the two occupancy layers and updates one mask byte. Strict `71.326740000%`; 15/15 CFG blocks, source BSS owner open. |
| `8002c424` | WIP | Paired border-mask expansion from `2c670`; six arguments, shared scan state, no source claim. |
| `8002c670` | WIP | Initializes `801b5a70` scan cursor and calls `2c290`/`2c424`; 0x7bc-byte body and scan-boundary state still open. |
| `8002cf40` | WIP, effect owner | `335a0` callee updates floor-item graphics; source-backed strict `92.910110000%`. |
| `8002d4f4` | exact, display owner | Sets the view matrix, position, and map-cell origin before `335a0`; strict `100.000000000%`. |
| `8002d918` | exact, graphics owner | TMD projection path called by `316c8`; strict `100.000000000%`. |
| `80030f5c` | exact | Mask-window scan called by `335a0`; strict `100.000000000%` after contiguous source merge. |
| `80031024` | exact | Renders the typed `80066888` model rows, calling `34070` and GTE SDK routines; strict `100.000000000%`. |
| `80031414` | exact, graphics owner | Color-byte draw helper called by `335a0`; strict `100.000000000%`. |
| `800314fc` | exact, collision owner | Collision-channel draw helper called by `335a0`; strict `100.000000000%`. |
| `800316c8` | WIP | `335a0` scene renderer selects a lighting row and calls animation `34070`; its `80198560` state is within the typed player runtime. |
| `80031850` | WIP | `3247c` calls this 0x53c-byte scene renderer five times; its matrix, lighting, animation, and packet paths remain open. |
| `80031d8c` | WIP | `3247c` event renderer and `34070` animation caller; no source claim. |
| `80031fa0` | exact, resource owner | Resource registry lookup used by the `3247c`/`34344` paths; strict `100.000000000%`. |
| `800320b0` | WIP, map owner | Radius mask helper called by `3247c`; source-backed strict `73.959180000%`. |
| `80032174` | WIP, map owner | Map-cell visibility query called by `3247c`; source-backed strict `93.600000000%`. |
| `8003247c` | WIP | 0xb70-byte placed-map/resource update calls `31850`, mask helpers, and actor helper `3c10c`; `80063dcc` data identity remains open. |
| `80033284` | exact, notification owner | Notification display-phase update called by `335a0`; strict `100.000000000%`. |
| `800335a0` | WIP | Frame coordinator calls `2c670`, `31024`, `30f5c`, `3247c`, graphics overlays, and present; its loop and transient scene fields remain unclaimed. |
| `80034070` | WIP | Animated vertex-buffer path called by `31024`/`316c8`; sparse-delta buffer type remains open. |
| `80034344` | WIP | Animated vertex query reached from `369b8`, `3c000`, and event callers; combines sparse lookup and Q12 interpolation, no source claim. |
| `80034f90` | WIP, source-backed | Rotates 10-byte placement-pattern offsets into the 80-by-88 two-layer occupancy grid; six proven calls from `35894`/`36ed4`. Strict `97.868220000%`, 14/14 CFG blocks, remaining register/order residue. |
| `80035194` | WIP, source-backed | Adjacent grid-copy helper with eight proven calls from `35894`/`36ed4`; nine-argument C copies selected fields across rotated 80-column occupancy rows. Strict `86.086365000%`; 51/49 CFG blocks and frame/setup differences remain. |
| `80035894` | WIP | Calls `34f90` twice and `35194` twice with patterns in the `8006787c` loaded band; this band's owner remains unresolved. |
| `80036ed4` | WIP | Calls `34f90` four times and `35194` six times in its map/object setup path; the large 0x1df4-byte body remains unclaimed. |

## Map-object placement and action graph

This eleventh set follows proven calls from the `35894` placement controller,
`36ed4` map-object controller, and `4678c` event controller into the shared
396-record `map_object_state` pool. Every row is a GAME function. SDK
`rand`, `rsin`, and `rcos` stay library calls. The contiguous `365d8`–`366fc`
source now models the pool's sequence counter and action-byte updates. The
scatter spawn and action query are exact as individual functions; the action
update still differs in switch ordering and object-pointer induction. Both
control jump tables at `8001187c` and `800118c4` now have RODATA claims.
Their unit-level text addends remain WIP because preceding functions compile
four and twenty bytes longer than retail, respectively.

| GAME VA | Verdict | Confirmed relationship and result |
| --- | --- | --- |
| `8002b604` | exact | Three calls from `36ed4`; calls `2a988` with Y minus 1280 and `2aaa4`, then reads the provisional collision-cache result in startup BSS. Strict `100.000000000%` in the collision-height wrapper unit. |
| `8002b73c` | WIP, source-backed | Occupancy adjustment called by `35894`; strict `98.404260000%`, with codegen residue. |
| `800314d4` | exact | `36ed4` calls the graphics color setter; strict `100.000000000%`. |
| `8003247c` | WIP, no claim | Map-resource controller calls the `36ad8` map-boundary predicate. |
| `80034f90` | WIP, source-backed | Pattern placement called six times by `35894`/`36ed4`; strict `97.868220000%`. |
| `80035194` | WIP, source-backed | Rotated occupancy rectangle copy called eight times by `35894`/`36ed4`; strict `86.086365000%`. |
| `80035590` | exact | Resets a pool object before both `36464` and `365d8` initialize it; strict `100.000000000%`. |
| `800355d8` | exact | Object property setter used throughout `36ed4`; strict `100.000000000%`. |
| `800356ac` | WIP, source-backed | Map-cell marker setter used by `35894`/`36ed4`; strict `75.573770000%`. |
| `80035894` | WIP, no claim | Placement controller calls `34f90` twice and `35194` twice and reads `8006787c` pattern rows. |
| `80036078` | exact | Map-object collision query shares the typed 24-byte template pool; strict `100.000000000%`. |
| `80036190` | WIP, source-backed | Map-object collision/facing query called through the event/object graph; strict `89.784170000%`. |
| `800363bc` | exact | Idle-action setter called by both spawn helpers and `36ed4`; strict `100.000000000%`. |
| `800363dc` | exact | Reuses a free or oldest map-object slot for `36464` and `365d8`; strict `100.000000000%`. |
| `80036464` | WIP, source-backed | Typed effect spawn calls pool acquire, reset, `rand`, and idle-action setter through a 17-entry template-kind switch. Strict `95.322580000%`; the switch table's first text addend differs by four bytes because preceding `36190` remains WIP in the contiguous unit. |
| `800365d8` | exact | Scatters an object by a 600-unit `rsin`/`rcos` radius, advances pool counter at `+0x873e`, then starts action `0x62`; strict `100.000000000%`. |
| `800366fc` | WIP, source-backed | 396-object action-byte and timer update called twice by `36ed4` and once by `4678c`; strict `37.336365000%`, 37/38 CFG blocks. |
| `800368b4` | exact function, WIP unit data | Event controller calls a five-outcome map-object action/marker query. Its 21-entry switch table at `800118c4` is owned by the contiguous scatter unit; strict function score is `100.000000000%`, while the table's text addend differs by twenty bytes because preceding `366fc` remains WIP. |
| `800369b8` | exact | Object animation vertex-to-world helper called twice by `36ed4`; strict `100.000000000%`. |
| `80036ad8` | WIP, source-backed by another owner | `3247c`, `36ed4`, and `4678c` call a map-boundary predicate now modeled with `player_state.camera_position`; source is in the contiguous vertex-world unit. |
| `80036b68` | exact | Two `36ed4` calls pass pairs of 8-byte offsets from unclassified `8006d6e4` rows. Typed source interpolates a target map object's rotated position and action progress; strict `100.000000000%`. The loaded row owner remains open. |
| `80036e24` | WIP, audio owner | CD/frame service called twice by `36ed4`; strict `98.863640000%`, with saved-register assignment residue. |
| `80036ed4` | WIP, later sourced | Large 0x1df4-byte controller calls the placement/copy helpers, object pool helpers, audio, collision, and frame color setter; see the latest dispatcher verdict below. |
| `8003fb94` | WIP, source-backed | Effect helper directly called by `36ed4` at `383c8`; typed effect and actor views are strict `87.791046000%`. |
| `8004678c` | WIP, no claim | Event controller calls `366fc`, `368b4`, and `36ad8`; event action state still broad. |

## Event controller, collision probe, and save-state call graph

This twelfth set follows proven calls from `4678c`, `475d8`, and `47c98` into
map-object actions, camera pose, event counters, and save-state conversion.
The exact `45e5c` terrain probe joins the two adjacent audio wrappers in one
unit and uses the existing startup-cleared BSS view for its collision-cache
pointer. The pointer's target record remains unresolved. The strict verdicts
below are per function; `368b4` is exact even though its containing unit has
a switch-table addend residue from preceding WIP `366fc`.

| GAME VA | Verdict | Call-connected evidence or remaining boundary |
| --- | --- | --- |
| `8001586c` | exact | `475d8` interpolates wrapped angles; strict 100%. |
| `8001bcfc` | WIP, no source | `47c98` calls this 0x26c-byte scene helper; record owner remains open. |
| `80028fa8` | exact | `47c98` transitions player state and releases pooled records; strict 100%. |
| `800293d4` | exact | `47c98` enters player reaction state; strict 100%. |
| `8002a988` | WIP, no source | `489ac` queries map-cell height; collision-cache and fallback row owner remain open. |
| `8002b604` | exact | `45e5c` invokes the five-argument collision wrapper; strict 100%. |
| `80036464` | WIP, source-backed | Map-object effect spawn is 95.32258% strict; its 17-entry switch table has a prior-function text addend residue. |
| `800368b4` | exact function, WIP unit data | `4678c` calls the action/marker query three times; strict function score 100%, table addend differs by 0x14. |
| `80045e5c` | exact | `4678c` and `47c98` call the 800-unit terrain probe; strict 100% with existing collision-cache BSS view. |
| `80045f20` | exact | `475d8` projects event pose into world coordinates; strict 100%. |
| `80045fd4` | exact | `475d8` interpolates scene pose; strict 100%. |
| `800460a0` | WIP, source-backed | `462bc` seeks animation phase; strict 99.268295%, residual codegen difference. |
| `80046144` | exact | `462bc` scans message markers; strict 100%. |
| `800461a0` | WIP, source-backed | `462bc` uses the adjoining message-stream helper; strict 65.60564%. |
| `800462bc` | WIP, no source | `47c98` calls the 0x444-byte event dispatcher; its indirect branch and event record remain unresolved. |
| `80046700` | exact | `4678c` uses the event map-object spawn helper; strict 100%. |
| `8004678c` | WIP, no source | Controller calls `368b4`, `366fc`, `36190`, sound/CD and notification services. |
| `800473e0` | exact | `4678c` decrements event counters; strict 100%. |
| `80047434` | exact | `475d8` increments and notifies on event counters; strict 100%. |
| `800474c4` | exact | `47c98` interpolates three collision channels; strict 100%. |
| `800475d8` | WIP, no source | 0x6c0-byte controller calls pose interpolation, event counters, PadRead and CD services. |
| `80047c98` | WIP, no source | Controller joins terrain probe, scene pose, actor probe and save serializer through an unresolved callback. |
| `800483a8` | exact | Event callback slot 04 wrapper; strict 100%, indirect target remains unresolved. |
| `800483d8` | WIP, source-backed | Offset-to-pointer conversion is 89% strict; same six-block CFG, constant/base register assignment differs. |
| `80048428` | exact | `48554` adjusts allocated arena owner pointers; strict 100%. |
| `80048498` | WIP, source-backed | Pointer-to-offset conversion is 88.68421% strict; same six-block CFG, constant/base register assignment differs. |
| `800484e4` | exact | `48554` reverses arena pointer relocation; strict 100%. |
| `80048554` | WIP, no source | Save serializer calls four event-state conversion helpers; packed record and switch owner remain open. |
| `800489ac` | WIP, no source | Save decoder calls map-object reset and map-cell height lookup; packed record remains open. |

## Actor and effect dispatch continuation

This thirteenth set follows `36ed4` and the effect update dispatcher into the
actor/magic call graph. The new `3fb94` C body has matching CFG and referents,
but its entry argument storage and flag lifetime remain different. Three
contiguous effect helpers are strict-exact: `4212c` joins `41e94`, while
`42298` and `42424` precede the exact `424f0` scatter helper. The latter pair
shares a three-halfword vector at `801c7068`; its complete allocation and
defining TU remain unproved, so the address-derived BSS identity stays
config-only even though both function bodies match. Each verdict below is for
GAME.EXE and uses the strict objdiff result when source exists.

| GAME VA | Verdict | Evidence and remaining boundary |
| --- | --- | --- |
| `800152ac` | exact | Actor magic dispatch uses the exact angular tolerance helper. |
| `80015318` | exact | Actor magic dispatch and the frame actor scan call the exact XZ angle helper. |
| `800157f8` | exact | `4212c` uses the exact randomized fixed scalar before scaling the spawn direction. |
| `800248a8` | WIP, no source | `3fb94` forwards mode `0x80` damage values to this player helper. |
| `80039c94` | WIP, source-backed | `3fb94` forwards mode `0x10` actor magic values here after its angle gate; the contiguous actor curve unit now owns its body. |
| `8003f610` | exact | Main-loop actor scan binds and updates the pool, reads `player_state.camera_position` at `801985a8`, and copies typed map-object positions; strict 100%. |
| `8003fa68` | WIP, source-backed | Both collision probes in `42298` call this effect helper; strict `70.733330000%`. |
| `8003fb94` | WIP, source-backed | Typed two-mode actor/player magic dispatch, strict `87.791046000%`; 11/11 CFG blocks and 6/6 branches match. The record-type flag transform now matches retail; entry register allocation and mode-mask scheduling differ. |
| `8003fdac` | exact | Selects player magic or fixed power for the adjoining effect emitters. |
| `8003fdd0` | exact | Packages current effect/magic records and calls `3fb94`. |
| `8003feb0` | exact | Computes an offset spawn position and calls `3fdd0`. |
| `8003ff18` | exact | Sends current effect damage and actor magic to the respective helpers. |
| `800400c0` | exact | Builds a rotated effect offset for `401b4` and effect spawning. |
| `800401b4` | exact | Adds the current effect position to the rotated offset from `400c0`. |
| `80040220` | exact | Finds a free effect record in the 128-slot pool. |
| `80040264` | exact | Initializes a scaled effect record. |
| `800402a4` | exact | Initializes a fixed effect record. |
| `80040308` | WIP, no source | Large effect constructor called by `4212c`, `41e94`, and `424f0`; its variadic record layout remains open. |
| `800416ec` | exact | Rotates an effect offset before the motion-probe family. |
| `8004177c` | exact | Effect motion probe in the dispatch call graph. |
| `8004195c` | exact | Effect aim-and-move helper in the dispatch call graph. |
| `80041b14` | exact | Effect target-motion helper in the dispatch call graph. |
| `80041cd0` | exact | Effect scale step immediately before the spawning helpers. |
| `80041d7c` | exact | Spawns an effect with the loaded zero-direction vector. |
| `80041e94` | exact, strict `100.000000000%` | Effect motion spawn now uses the evidenced eighth fixed O32 argument; the earlier stack-argument residue is resolved. |
| `8004212c` | exact | Randomized spawn within 500 units of the provisional collision-cache lower bound; strict `100.000000000%`. |
| `80042298` | exact function, BSS owner WIP | Moves the current effect, probes collisions twice, writes the provisional motion vector; strict `100.000000000%`. |
| `80042424` | exact function, BSS owner WIP | Rewinds effect position using that vector or the current direction; strict `100.000000000%`. |
| `800424f0` | exact | Existing scatter emitter preserved at strict `100.000000000%` after the contiguous merge. |
| `80042650` | WIP, effect owner | Large update dispatcher directly calls `4212c`, `42298`, and `42424`; source work is owned by the effect campaign. |

## Actor movement direction continuation

This ten-function GAME.EXE batch follows the proven calls from the actor
behavior dispatcher into its movement-direction helper and the helper's
geometry calls. `8003c3e0` now has typed C in the contiguous actor-group unit.
Its direct calls and internal jumps have reviewed relocation rows. The source
matches all 23 CFG blocks and 11 branches, but remains strict
`96.361700000%` WIP; its direction read is `player_state+0xe8`, and the C
differs in stack allocation and arithmetic scheduling.
The three earlier functions in the same unit remain strict exact. The
dispatcher has an unresolved indirect jump, so it has no source claim.

| GAME VA | Verdict | Evidence and remaining boundary |
| --- | --- | --- |
| `80015034` | exact | Pitch/yaw-to-forward-vector helper called by `3c3e0`. |
| `80015188` | exact | Q12 short-vector scaling called by `3c3e0`. |
| `800154a8` | exact | Three-axis distance helper called by the movement loop. |
| `800154fc` | exact | Produces movement pitch/yaw from the target delta. |
| `8001586c` | exact | Wrap-aware angle interpolation called twice per movement step. |
| `8003c000` | exact | Animated actor vertex offset and rotation; preserved in the expanded unit. |
| `8003c10c` | exact | Actor/group position selection; preserved in the expanded unit. |
| `8003c220` | exact | Actor animation phase and motion selection; preserved in the expanded unit. |
| `8003c3e0` | WIP, strict `96.361700000%` | Typed eight-argument movement loop has matching CFG and call set; direction is a player-state field and codegen residue remains. |
| `8003c614` | WIP, no source | Eleven direct calls to `3c3e0`; switch-table indirect jump and full dispatcher owner remain open. |

## Effect record dispatch and pool continuation

This exactly ten-function GAME.EXE batch follows the current effect record from
the collision probe through actor/player magic dispatch, offset construction,
and pool allocation. Retail control flow, direct calls, data references,
strings, source history, and adjacent claims were checked before the source
edit. The `3fb94` source now transforms the packed record-type bits at the
same branch as retail and retains matching 11-block/6-branch control flow.
The remaining strict difference starts in its argument setup and mode-mask
instruction schedule. The earlier `3fa68` probe remains effect-campaign-owned;
no source change was made to it. The extra contiguous `402a4` function in the
unit stays exact but is outside this ten-function verdict set.

| GAME VA | Final verdict | Confirmed connection or residue |
| --- | --- | --- |
| `8003fa68` | WIP, strict `70.733330000%` | Collision probe called twice by `42298`; retail has 14 CFG blocks versus 15 compiled, with repeated call arms folded in C. Effect campaign owns the source. |
| `8003fb94` | WIP, strict `87.791046000%` | Effect actor/player magic gate called by `36ed4` and `3fdd0`; referents, call set, 11 CFG blocks, and 6 branches match. Entry instruction order remains different. |
| `8003fdac` | exact, strict `100.000000000%` | Chooses current player magic or the fixed effect power for adjoining emitters. |
| `8003fdd0` | exact, strict `100.000000000%` | Packages current effect and magic records, then calls `3fb94`. |
| `8003feb0` | exact, strict `100.000000000%` | Builds an offset spawn position and calls `3fdd0`. |
| `8003ff18` | exact, strict `100.000000000%` | Sends current effect damage and actor magic to their respective helpers. |
| `800400c0` | exact, strict `100.000000000%` | Builds a rotated effect offset for `401b4` and spawning. |
| `800401b4` | exact, strict `100.000000000%` | Adds the current effect position to the rotated offset from `400c0`. |
| `80040220` | exact, strict `100.000000000%` | Finds a free effect record in the 128-slot pool. |
| `80040264` | exact, strict `100.000000000%` | Initializes a scaled effect record. |

## Actor target and behavior follow-up

This exactly ten-function GAME.EXE batch follows confirmed actor targeting,
damage, movement, and group-fixup calls. The retail disassembly, CFG, direct
callers and callees, data references, adjacent claims, and existing source were
inspected for each address. The new C for `3a318` and `3a778` is in the
contiguous `actor_player_damage` unit. Its pre-existing `3a614` body remains
strict `96.910110000%` WIP. A focused identical listing for `3a778` was
confirmed by a fresh strict report with 141/141 GAME target units relinking.
Indirect branch targets below remain unresolved.

| GAME VA | Final verdict | Confirmed connection or residue |
| --- | --- | --- |
| `80039108` | WIP, no source | Actor candidate scorer called by `39640`; 59 CFG blocks and two indirect jumps require dispatch-table and candidate-record ownership. |
| `8003983c` | WIP, historical strict `99.075380000%` fuzzy | Typed lifecycle update called by `3f610`; the direct call set, player/actor referents, 37 CFG blocks, 24 branches, and 13 return frontiers align. A later focused carve reviewed twelve actor/player BSS relocation pairs and corrected the byte read from player-state +0x10a to retail-proven +0x0a. The focused listing still swaps the actor pointer and constant-one saved registers; current strict score has not been refreshed. |
| `80039c94` | WIP, source-backed, 50.3% focused | Actor magic recipient called by `3a318`, `3fb94`, player damage, and effect dispatch; its 72-block retail body includes an unresolved indirect call. |
| `8003a318` | WIP, `97.5%` focused listing; strict pending | Typed 200-actor magic scan matches the observed distance-helper and `39c94` calls. Retail `lhu` at stack arguments 5 and 16 establishes `u16` falloff/effect flags; the source now computes amount and actor base after the flag branch. Focused CFG is 26/26 blocks and 13/13 branches; only the two argument temporary registers differ. |
| `8003a778` | exact, strict `100.000000000%` | Typed best-target scan retains retail angle wrapping, range test, random variation, and loop schedule; five confirmed callers include player and effect paths. |
| `8003ae50` | WIP, source-backed, 99.2% focused | Actor movement helper called by the matched motion family; typed terrain, angle, `SquareRoot0`, and sine/cosine paths span 58 matching CFG blocks. Six adjacent actor-animation listings remain SAME. |
| `8003c3e0` | WIP, strict `96.361700000%` | Typed actor movement loop is called eleven times by `3c614`; 23 CFG blocks, 11 branches, and direct call set agree, with stack/arithmetic codegen residue. |
| `8003c614` | WIP, no source | Actor behavior dispatcher calls `3c3e0` eleven times; its 45-block CFG has an unresolved indirect jump. |
| `8003d184` | WIP, no source | Higher actor behavior dispatcher calls animation, collision, and movement helpers; 410 blocks and three indirect jumps require dispatch ownership. |
| `8003f7ec` | WIP, strict `85.862070000%` | Fixes up 16 target pointers in each active 0x78-byte group; CFG and referents agree, with two remaining instruction-order differences. |

## Actor-linked placement and event continuation

These ten distinct GAME.EXE functions follow the proven angle/movement calls,
the map-cell and placed-object path, and the actor/event callbacks. Retail
boundaries, CFGs, callers, callees, data references, adjacent claims, and
existing source were checked before changing `14a08`. Its retail early-return
delay slot computes `current - target`; the previous C returned `velocity`.
That semantic correction makes the entire 12-function matrix unit strict
exact. The large `35894` and `36ed4` controllers retain unresolved indirect
branches and candidate table references, so neither has a C claim.

| GAME VA | Final verdict | Confirmed connection or residue |
| --- | --- | --- |
| `80014a08` | exact, strict `100.000000000%` | Angle-velocity step called by actor/map-object controller `36ed4`; corrected early return to retail `current - target`, preserving all eleven adjacent exact matrix functions. |
| `800158b4` | WIP, strict `98.000000000%` | Nine-halfword Q12 interpolation called by collision rows and model renderer `31850`; CFG and result agree, with pointer/index register and increment-order residue. |
| `80030de4` | WIP, strict `89.521280000%` | Draws either or both occupancy-cell layers for exact caller `30f5c`; ten CFG blocks and six branches agree, while byte-load and position arithmetic scheduling differs. |
| `80035894` | WIP, no source | Map placement controller calls `34f90` and `35194` twice each; 64 CFG blocks, two indirect jumps, and the loaded `8006787c` pattern-band owner remain open. |
| `80036464` | WIP, strict `95.322580000%` | Typed map-object effect spawn calls pool acquire, reset, and `rand`; function register scheduling differs, and the contiguous unit's switch-table text addend remains four bytes off because preceding `36190` is WIP. |
| `800366fc` | WIP, strict `37.336365000%` | Updates marker/action fields across 396 map objects; proven calls from `36ed4` twice and event controller `4678c`. Retail/compiled CFG is 37/38 blocks with switch-arm order differences. |
| `80036ed4` | WIP, later sourced | Placed-object/actor controller directly calls `14a08`, `366fc`, map-cell placement, audio, and collision helpers; its 329-block CFG has indirect control. See the latest dispatcher verdict below. |
| `8003a614` | WIP, historical strict `96.910110000%`; current focused `81.2%` | Actor-to-player radial damage gate called five times by `3d184`; distance, angle, and damage call set agrees, while player-state load/register scheduling differs. The historical strict score predates the current source comparison. |
| `8003d084` | WIP, strict `91.600000000%` | Clamps the signed actor sound offset, then combines it with `rand`; five CFG blocks and both branches agree. GCC reassociates the final `-2` around the shift. |
| `800460a0` | WIP, strict `99.268295000%` | Actor animation phase seek called three times by event dispatcher `462bc`; six CFG blocks, two branches, calls, and referents agree, with only step/half-step saved-register assignment different. |

## Render-frame actor and map continuation

These ten distinct GAME.EXE functions are linked by the frame driver, its
render/map calls, the adjacent animation morph path, and the effect/save state
that shares those actor records. Retail disassembly, CFG, callers, callees,
data references, adjacent claims, and current source were checked for every
address. The only source change was in `335a0`: HP and MP digit extraction now
reuses one signed remainder local, matching the retail register lifetime. Its
focused listing is SAME and its strict comparison is 100%, with all 143 GAME
target units relinking. The strict command still exits on three independent,
previously recorded RODATA addend divergences in other units.

| GAME VA | Final verdict | Confirmed connection or remaining boundary |
| --- | --- | --- |
| `8002c670` | WIP, no source | Frame-driver call into 11-block mask dispatcher; 26 direct calls are decoded, but its seven-pair initialized table reads into the unclassified `8006787c` band. Table extent and source owner are unproved. |
| `800311b0` | WIP, strict `92.148150000%` | Textured quad builder; 8/8 CFG blocks, 5/5 branches, and referents agree. Retail uses a 36-byte frame versus compiled 32, with saved-register and packet-store scheduling differences. |
| `80031850` | WIP, strict `84.650750000%` | World-model renderer called by `3247c`; 40/40 CFG blocks, 16/16 branches. The apparent branch-arm call swap in the diff is a layout inversion, while retail confirms the source's two calls on their respective matrix conditions. Register and address scheduling differ. |
| `80031d8c` | WIP, strict `94.654140000%` | Animated-object renderer called by `3247c`; 7/7 CFG blocks and 2/2 branches. Retail retains the final draw argument in a saved register where compiled C reloads it. |
| `8003247c` | WIP, no source | Large placed-object/render controller calls `31850` and `31d8c`; 96 CFG blocks and its record/packet state need a complete typed owner before a truthful source claim. |
| `800335a0` | exact, strict `100.000000000%` | Frame driver calls the collision-mask dispatcher, map/render controllers, notification, display, CD, and pool helpers. Shared HP/MP remainder lifetime produces the retail listing. |
| `80033d3c` | WIP, strict `95.218390000%` | Morph accumulator adjacent to two exact sparse-animation functions. Its 17 CFG blocks and 10 branches agree, but loop/exit layout remains different; the neighboring exacts are preserved. |
| `80041e94` | exact, strict `100.000000000%` | Effect motion source adjacent to exact `4212c`; the eighth fixed O32 argument now matches retail, resolving the prior stack-argument residue. |
| `80048554` | WIP, no source | Save serializer has 43 CFG blocks and one unresolved indirect call; callback-table ownership and field schema remain open. |
| `80048d24` | WIP, no source | Save-state copier/decoder has 16 CFG blocks and a 0x5b8-byte field-copy body; destination save-record owner and complete layout remain unmodeled. |

## Main-loop, projected-model, and actor terrain continuation

This exactly ten-function GAME.EXE batch follows the main loop into resource
loading and scene transitions, and the frame renderer into three adjacent
projected-model packet builders. The actor terrain motion function is reached
from the actor behavior dispatcher and uses the existing actor and collision
cache views. Retail disassembly, CFG, direct callers/callees, data referents,
adjacent claims, and the current source state were inspected for every entry.
`3b5d0` now has typed C appended to the contiguous actor-motion collision
unit; its direct call and BSS relocation pairs were reviewed. Focused
comparison preserves the three preceding SAME listings. Fresh strict
comparison gives `3b5d0` `75.669390000%` with 40/40 CFG blocks and 21/21
branches, and preserves all three adjacent functions at 100%; 142/142 GAME
target units relink. The strict command still stops on the three known,
unrelated RODATA addends elsewhere.

| GAME VA | Final verdict | Confirmed connection or remaining boundary |
| --- | --- | --- |
| `8001369c` | WIP, no source | Main loop calls `15d58`, `15fd4`, `16820`, and exact frame driver `335a0`; nine startup clears include known complete BSS owners, while transition and resource boundaries still require a shared source model. Five CFG blocks, 48 outgoing references. |
| `80015d58` | WIP, no source | Main-loop resource initializer opens seven CD archives, loads a map stream and VAB, copies nine length-prefixed blocks, and initializes TMD assets. The `80011000` archive-name region and two destination extents remain source-owner questions. One straight CFG block. |
| `80015fd4` | WIP, no source | Main-loop scene wait saves five callback-state bytes, sets transition values, repeatedly calls CD yield and `16820`, then invokes callback slot 5. The fixed TMD pointer `8012da68` is outside the modeled load region and its origin remains unresolved; the indirect target is not inferred. Three CFG blocks. |
| `80016260` | WIP, no source | Actor/map-event callers and player transition code enter a 67-block resource/sequence state machine; it directly calls save serializer `48554` and scene transition `16820`. Its callback and BSS ownership remain incomplete. |
| `80016820` | WIP, no source | Called by main loop, `15fd4`, and `16260`; phase dispatcher calls actor target fixup, actor/map loaders, map placement, CD and audio helpers. The 62-block CFG has an indirect jump and call, and reachability is not complete. |
| `8002ddb4` | WIP, no source | World-model packet builder uses four `NormalClip` paths, depth color calls, and `AddPrim`; 44-block CFG. Primitive packet layout and ownership are not fully reconstructed. |
| `8002e4dc` | WIP, no source | Sibling projected-model packet builder with four `NormalClip` paths and `AddPrim`; 44-block CFG. Its packet layout shares the open `2ddb4` boundary. |
| `8002ebe0` | WIP, no source | Downstream animated-model draw caller of `31d8c`; 26-block CFG with `NormalClip`, color calls, and `AddPrim`. GTE/packet source types remain incomplete. |
| `8002ff5c` | WIP, no source | Prepared TMD primitive-buffer builder used by the map-cell renderer; its 0xcbc-byte body uses fourteen direct `resource_copy_words` calls and unresolved compact packet layout. Eleven CFG blocks. |
| `8003b5d0` | WIP, strict `75.669390000%` | Current-actor four-state terrain motion uses exact collision helpers `2b604`/`2b9d4`, collision cache, and the actor group byte; typed source has the retail call set, 40/40 CFG blocks, and 21/21 branches. State dispatch and saved-register/step-block layout differ, with no adjacent exact regression. |

## Main-loop audio and player collision continuation

This exactly ten-function GAME.EXE batch follows the main loop's sound and
floor-item initialization, the arena service reached by resource loading, and
the player collision/action controller. Each function was checked against its
retail extent, disassembly, CFG where available, direct callers and callees,
strings, existing source, and the current strict report. The `22300` C source
previously passed cue 16 for both 17 and 18. Retail passes the incoming cue in
those two cases. Correcting that argument makes all three functions in
`game.menu_sound_cue` exact: `22300` is strict 100% (148/148 bytes), its two
neighbors remain exact, and 143/143 GAME units relink. The global check still
stops on the three previously recorded unrelated RODATA addends.

| GAME VA | Final verdict | Confirmed connection or remaining boundary |
| --- | --- | --- |
| `800139c4` | WIP, no source | Main-loop sound initializer calls eight Sony `Ss*` APIs, clears VAB and voice slots, and seeds seven stream buffers. The workspaces at `8009a6a0`, `80198640`, and `80165a68` have no proved source owner or complete extent. |
| `800144b8` | WIP, strict `94.873420000%` | Typed VAB CD service has the retail 11/11 CFG blocks, 5/5 branches, call set, and referents. GCC retains `-1` in a saved register where retail retains `1`, changing constant loads and store scheduling; no semantic source correction is evidenced. |
| `800155a4` | WIP, strict `87.950820000%` | Seven-argument vector-distance helper has the retail horizontal and vertical interval tests and `SquareRoot0` call. Its vertical branch layout is 13 retail versus 14 compiled blocks, with 9 versus 8 branches; the kept C preserves the observed comparisons and signed widths. |
| `80017608` | WIP, strict `99.782610000%` | Arena allocator calls find, compact, and retry, then splits a block when the 12-byte-header-adjusted remainder is at least 2060. All 8 CFG blocks and 4 branches agree; only the intermediate/result register choice across two arithmetic instructions differs. The other 56 functions in the contiguous CD/memory unit retain SAME focused listings. |
| `80022300` | exact, strict `100.000000000%` | Menu sound cue dispatch passes 16, 17, or 18 to `audio_key_on` as retail does, then calls `SsSeqCalledTbyT`; cue 13 takes the two-`VSync` stereo path. Corrected C matches the retail seven-block CFG and all 148 bytes. |
| `800279cc` | WIP, no source | Player collision branch calls the height/shape probes, death and sound helpers, `rsin`, and collision bounds. Its 0x5ac-byte body depends on the provisional collision-cache extent and shared player state; no complete source model is yet supported. |
| `80027f78` | WIP, no source | Sibling collision branch calls `2b9d4` three times, death, two-component length, and collision bounds. Its loop updates player reaction motion and probes the still-provisional collision-cache result at `801d8d50`; ownership remains WIP. |
| `80028998` | WIP, no source | Player action controller calls the magic-cost gate, weapon attack, and event controller; its 0x528-byte flow spans player flags and unproved dispatch-table referents. |
| `8002985c` | WIP, no source | Player update has 113 candidate outgoing references, including the sibling collision and action functions, but its retail function is fragmented and CFG/reachability cannot yet be validated. Candidate calls were not promoted to proven. |
| `8002ce68` | WIP, strict `65.851850000%` | Main loop calls the seven-argument floor-item GPU capture five times. The typed C has the four retail calls and 5/5 CFG blocks; GCC saves three stack arguments early and uses a 56-byte frame where retail reloads them later with a 40-byte frame. Adjacent `2ce2c` and `2cf40` remain SAME. |

## Menu and card controller continuation

This exactly ten-function GAME.EXE batch follows the top menu through its
item-list and memory-card controllers. Each target was checked against its
retail extent, disassembly and CFG, direct callers and callees, strings,
adjacent claims, and source history. For item action `18f8c`, retail reads the
current HP before clearing player-state field `+0x54`, then adds 15. The C
source had reversed those first two memory operations. A narrow source
correction makes all three functions in `game.menu_glyph_rows` identical in
focused listings and strict 100%; 144/144 GAME target units relink. The global
check still stops on the three established, unrelated RODATA addends.

| GAME VA | Final verdict | Confirmed connection or remaining boundary |
| --- | --- | --- |
| `8001876c` | WIP, strict `96.366460000%` | Seven-choice top-menu controller calls the item and card flows, including `1a898`, `1b2dc`, and `21c8c`. The 34-block CFG and call set agree; saved-register selection and return scheduling differ. Exact adjacent `189f0` remains intact. |
| `80018f8c` | exact, strict `100.000000000%` | Item IDs 71–80 adjust HP/MP and other player fields. In case 75, reading HP before clearing field `+0x54` reproduces retail's load/store order and shared HP-add tail; adjacent row selectors `18d08` and `18dec` remain exact, as does the 3360-byte loaded glyph table. |
| `8001930c` | WIP, no source | Called by `18ac8`; loads archive entry 6, uploads a TIM, emits two `POLY_FT4` packets, waits for pad input, then frees the allocation. Its current-packet references and archive resource lifetime need a complete source model. |
| `8001a4f0` | WIP, no source | Item-list controller has a 2440-byte stack record and calls exact row selector `18d08`, row appender `199d0`, list init, item-model load, and preview helpers. The copied row at `800649ec` is only a candidate datum; its initialized owner and list lifetime remain open. |
| `8001a898` | WIP, no source | Top-menu choice 4 enters a 3240-byte stack-backed equipment list. It calls exact exclusion selector `18dec`, list init, model load/release, preview, and frame helpers; full row/model lifetime remains unresolved. |
| `8001ac80` | WIP, no source | Card-choice caller `1aa9c` enters a 1056-byte stack-backed directory/list flow with card start/stop, row builder, panel, and input calls. The card-entry structure and source lifetime remain incomplete. |
| `8001b2dc` | WIP, no source | Top-menu choice 6 draws a seven-row selection with pad, cue, and two-frame redraw calls. Retail saves and restores six bytes at player-state `+0xc7..+0xcc`; a truthful contiguous field view is not yet modeled. |
| `8001b554` | WIP, no source | Main loop calls this card startup and temporary-file probe; it drives panel, input, and display-state transitions before calling `1b834`. The 800-byte stack state and directory-record lifecycle remain unresolved. |
| `8001b834` | WIP, no source | Card controller `1b554` calls this directory scan, row builder, list, preview, and save-reader flow. Its 1056-byte stack state depends on the unresolved 40-byte card-entry model. |
| `80021c8c` | WIP, strict `99.956985000%` | Enter-menu display mode has the exact seven-block CFG, calls, and referents, but retail allocates a 32-byte frame while current C allocates 24; no source-level local supports padding it. Exact adjacent `21e00` remains intact. |

## Menu packet, card choice, and directory continuation

These ten non-exact GAME.EXE functions follow card choice through menu window,
sprite, glyph, number, and directory helpers. For each, retail disassembly,
CFG, direct callers/callees, string references, current source, and strict
report were reviewed. Focused rebuilds identify the first meaningful
differences below. No seventh-batch source change was retained: a trial that
expressed `1f8b8` as a `while` loop changed its CFG from 42/42 blocks and
18/18 branches to 42/40 and 18/19, so the original humane source was restored
and rechecked. Existing exact siblings were preserved. The scores below are
the current strict report, not focused listing percentages.

| GAME VA | Final verdict | Confirmed connection or remaining residue |
| --- | --- | --- |
| `8001aa9c` | WIP, strict `97.305786000%` | Two-choice card controller calls `1ac80` and `1b14c`, then menu frame/window and audio fade helpers. The current source preserves the call set; result-check block placement and register lifetimes differ. |
| `8001bf68` | WIP, strict `97.123890000%` | Card probe/format/write flow calls the card formatter and file writer. The current C repeats dialog dimensions where retail retains two values in saved registers; exact adjacent `1c12c` and glyph-suffix data remain intact. |
| `8001f8b8` | WIP, strict `94.364640000%` | Menu preview choice has the same 42 CFG blocks and 18 branches as retail. The result-exit block and label-kind 2 branch use a different layout, and initial argument/register assignment differs. A loop-form trial worsened CFG and was reverted. |
| `8001fb8c` | WIP, strict `99.787880000%` | Window renderer has matching 10/10 CFG blocks and 6/6 branches; retail allocates a 48-byte frame and current C 40 bytes. No real stack object supports fabricated padding. |
| `80020d20` | WIP, strict `99.440680000%` | Animated sprite blit emits the same `POLY_FT4` fields and primitive-buffer calls. Retail computes left X as `(position.x - 8) - width`; current GCC groups `width + 8` before subtracting. The source expression already states the retail order. |
| `80020ef8` | WIP, strict `99.394490000%` | Fixed-CLUT sprite blit has the corresponding seven-pixel left margin and matching packet/call set; only the same subtraction grouping differs in the focused listing. |
| `800210ac` | WIP, strict `99.669040000%` | Glyph-string renderer's packet references and loop agree. Retail uses a 56-byte stack frame versus current 48, with a texture-byte store scheduled differently; no supported source correction remains. |
| `80021510` | WIP, strict `98.619570000%` | Number-glyph renderer uses the same packet fields and current-poly referent, with repeated byte-load and pointer-load register/order differences. |
| `80022058` | WIP, strict `95.740000000%` | Number formatter keeps style prefixes/suffixes and decimal digit division; retail has 36 CFG blocks and an eight-byte frame, current C 37 blocks and no frame. The existing source expresses the observed style and zero-stop behavior without padding. |
| `800226ec` | WIP, strict `93.605040000%` | Card directory scanner counts files, orders fifteen `DIRENTRY` records, and returns the size threshold. Retail and C have 13/13 CFG blocks and 7/7 branches; early slot-digit loads differ `lb`/`lbu` and scheduling. Exact `22b48` within the four-function unit remains intact. |

## Menu item codes and map-object animation continuation

This exactly ten-function GAME.EXE batch follows menu item glyph selection,
card wait, collision bounds, map placement, and actor animation. Each target
was inspected for retail extent, CFG, direct callers/callees, strings, current
source, and strict report. Focused recompilation preserved the existing exact
neighbors. No eighth-batch source change was retained: the remaining
differences are either unproved source/data ownership or the codegen/control
residues below. Scores are strict objdiff values from the shared report.

| GAME VA | Final verdict | Confirmed connection or remaining residue |
| --- | --- | --- |
| `8001d340` | WIP, strict `93.103450000%` | Primary item-code selector has 4/4 CFG blocks and 2/2 branches. The sole focused instruction-order difference moves the final `sll` of a 240-byte row stride across a load-image base-address setup. Its table owner remains shared with adjacent menu glyph code. |
| `8001d654` | WIP, strict `90.476190000%` | Secondary item-code selector has the same 4/4 CFG and 2/2 branches; the same row-stride/address instruction ordering remains. Both selector data/table neighbors were preserved. |
| `80023178` | WIP, strict `93.529410000%` | Card-wait glyph-code writer calls event and card helpers. Focused comparison retains matching control flow but different quotient/remainder register assignments and byte-store scheduling; three adjacent memory-card helpers remain exact. |
| `80023384` | WIP, strict `50.744186000%` | Player collision-bounds helper compares provisional collision-cache words with player position and starts death if bounds cross. Retail and C differ in arithmetic grouping, load schedule, and saved-register use; the cache's interior owner remains provisional. |
| `800349bc` | WIP, strict `92.342960000%` | Menu transition emits four textured quads and polls the pad. The 14/14 CFG blocks and 8/8 branches agree, but the loop exit/return arm and constant lifetimes differ; exact `34e10` and TIM neighbor remain intact. |
| `80034f90` | WIP, strict `97.868220000%` | Map-cell pattern placer rotates ten-byte occupancy records using `rcos`/`rsin`. Its 14/14 CFG blocks, 7/7 branches, calls, and referents agree; saved-register assignment and one address calculation order differ. Adjacent `35194` remains WIP with its prior source. |
| `800356ac` | WIP, strict `75.573770000%` | Map-object cell marker writes one of two five-byte occupancy layers and resets object scale. The 9/9 CFG blocks and 4/4 branches agree, while row/column address multiplication schedules differ; five adjacent map-object helpers remain exact. |
| `80036464` | WIP, strict `95.322580000%` | Map-object effect spawn preserves typed pool/reset calls; argument saved-register and halfword-store order differ. The containing map-object switch table still has the previously known text-addend divergence. |
| `800366fc` | WIP, strict `37.336365000%` | Dispatcher scans 396 map objects and handles action-specific marker changes. Focused source and retail differ in branch/field-update layout; exact `365d8` and `368b4` bodies are preserved, while the containing switch-table addend remains WIP. |
| `800460a0` | WIP, strict `99.268295000%` | Actor animation phase seek has matching 6/6 CFG blocks, 2/2 branches, calls, and referents. Only the saved-register assignment of the even step versus half-step differs; no supported semantic correction was found. |

## Menu list and card flow continuation

This exactly ten-function GAME.EXE batch follows proven top-menu and card-list
calls. Retail extent, CFG, direct callers/callees, strings, adjacent claims,
and source history were inspected for each address. The controller at `19834`
passes `effect_state.magic_records` to the existing exact 26-byte selector:
this identifies its byte-zero menu filter and `+0x16` MP-cost payload as
fields of `KfMagicRecord`, rather than a separate menu record. The shared
type, selector, and player-equipment reset now use that owner; inventory and
the existing selector/equipment focused listings remain exact. The new
controller subsequently reached strict 100% after correcting its local menu
view: the pointer at `+0x30` addresses 20 signed values, while 20 byte indices
are a separate stack array. The adjacent selector stays exact.

| GAME VA | Final verdict | Confirmed connection or remaining boundary |
| --- | --- | --- |
| `80018ac8` | WIP, unclaimed | Top-menu item-list controller calls exact glyph selector `18d08`, preview model load/release, input, and two-frame redraw. Its full list/model workspace remains unresolved. |
| `80019834` | exact, strict `100.000000000%` | Magic list controller selects records 14–19, initializes a six-row list, and handles input and redraw. Corrected values/index array ownership resolves the earlier pointer-setup residue; exact adjacent `199d0` remains intact. |
| `80019ac4` | WIP, unclaimed | Copies a 200-byte initialized record from `80064910`, calls exact row builder `19ce4`, then dispatches selection branches. The copied table and full list workspace lack a proved owner. |
| `80019ed4` | WIP, unclaimed | Menu equipment controller dispatches through unresolved indirect choices and calls player equipment setters. Indirect target/table ownership remains open. |
| `8001a2f4` | WIP, unclaimed | List controller calls exact 26-byte row selector and writes player menu selection byte `+0x97`. Copied row at `800649ec` and list workspace remain unresolved. |
| `8001bcfc` | WIP, unclaimed | Card list controller prepares directory rows and calls card probe, card row builder, input, and frame helpers. Its 1016-byte stack record and card-entry lifetime need a complete type. |
| `8001d030` | WIP, unclaimed | Primary item-list controller uses code translator `1d340` and item preview model helpers. Loaded range `80065950` has no proved table extent or source owner. |
| `8001d3b4` | WIP, unclaimed | Paired item-list controller uses secondary translator `1d654`, exact exclusion selector, and preview model helpers. Its scratch/list record owner remains open. |
| `8001d6a8` | WIP, unclaimed | Card/item list controller filters rows, loads a preview model, then draws and reads input. Its 3232-byte workspace and list/model state are not yet fully typed. |
| `8001e94c` | WIP, unclaimed | Numeric menu renderer repeats string and number glyph calls against initialized suffix rows at `80064a00` onward. The suffix-table boundary and full numeric render state remain unresolved. |

## Menu preview, card directory, and event-stream continuation

This exactly ten-function GAME.EXE batch follows proven calls among the
item-list controllers, card-directory readers, render/map paths, and event
stream. Retail extent, CFG, callers/callees, strings, source history, and
current match state were checked for every function. Focused rebuilds of
the three source-owned units below preserved their exact neighbors. The
`461a0` source already has the correct marker/fallback behavior; reversing
two real pointer declarations did not change the observed register-choice
residue and was reverted. No tenth-batch source edit was retained.

| GAME VA | Final verdict | Confirmed connection or remaining boundary |
| --- | --- | --- |
| `8001d8d0` | WIP, unclaimed | Item-list renderer calls exact glyph selector `18d08`, primary item-code translator `1d340`, preview-model helpers, and input renderer. Its selected-record source at `80065aec` and complete list workspace are not proved. |
| `8001ddd0` | WIP, unclaimed | Primary item-list controller calls `18d08`, `1d340`, list init, model load, input, and frame helpers. The list/model state needs a complete typed owner. |
| `8001e0a8` | WIP, unclaimed | Paired item-list controller calls secondary translator `1d654` and the same preview/input flow. The indexed list-state record remains unresolved. |
| `8001e484` | WIP, unclaimed | Shared input/model-preview controller is called by the list handlers and updates preview rotation/translation state. Its 66-block decision flow and input record need reconstruction. |
| `800228c8` | WIP, strict `85.156250000%` | Card save-reader parses experience, level, and slot digits from a 640-byte header. Its 24/24 CFG blocks and 13/13 branches agree, but initial signed-byte loads and digit-decoding register/order differ; exact card-directory neighbors remain intact. |
| `80022b74` | WIP, strict `93.666664000%` | Card payload reader calls libc file I/O, byte-sum, and payload handlers. The source-owned card unit keeps exact adjacent functions; direct return/input register scheduling remains non-exact. |
| `8002f5b0` | WIP, strict `95.833336000%` | Map renderer calls `NormalClip`, color/depth helpers, and `AddPrim`. Its 13/13 CFG blocks and 7/7 branches agree; saved-register assignment and one arithmetic schedule differ. Exact `render_enqueue_map` remains intact. |
| `80035194` | WIP, strict `89.595450000%` | Map-cell pattern placement shares records with `34f90` and uses its 51-block, 26-branch control structure. Retail has a 40-byte frame versus current C's 32 bytes; the source lacks a proved extra stack object, so no padding was added. |
| `800461a0` | WIP, strict `99.126760000%` | Event marker search has matching 14/14 CFG blocks, 5/5 branches, and exact adjacent `46144`. The remaining local pointer assignments use `a0`/`a1` in the opposite roles; a declaration-order trial did not resolve them. |
| `800462bc` | WIP, unclaimed | Event-stream dispatcher calls `461a0` and the phase helper `460a0`, then branches through a 16-case indirect table at candidate `80012890`. That table's owner/targets and the complete stream-state contract remain unproved. |

## Render-map and actor-group continuation

This exactly ten-function GAME.EXE batch follows typed map-cell rendering,
world-model lighting, resource visibility, and the actor-position callers of
the effect dispatcher. Retail disassembly, CFG, direct calls, data references,
adjacent claims, and source history were inspected for each address. Focused
rebuilds preserved exact neighbors in the render-map, resource, player, and
actor-group units. Source trials for the two player leaves did not improve the
retail control form and were reverted. No eleventh-batch source edit was kept.
The percentages below are strict objdiff results; the much higher focused
listing scores are not closure evidence.

| GAME VA | Final verdict | Confirmed connection or remaining residue |
| --- | --- | --- |
| `8002897c` | WIP, strict `88.571430000%` | The 0x1c-byte player interval leaf has retail `slti`/`xori` bounds, but the compiler assigns the condition and return to opposite registers; two truthful C control forms did not resolve it. |
| `80029624` | WIP, strict `62.448980000%` | The reaction phase/ramp helper retains the player-state halfword accesses and 20/20 CFG blocks with 9/9 branches. Signedness and common-tail trials worsened the listing and were reverted. Exact reaction siblings remain intact. |
| `80030c18` | WIP, strict `96.521736000%` | Map-cell renderer has 11/11 blocks, 6/6 branches, calls, and referents; four entry instructions load the shape byte and prepare `SetRotMatrix` in a different order. |
| `80030de4` | WIP, strict `89.521280000%` | Draws either or both occupancy layers for exact `30f5c`. All 10 blocks and six branches agree; byte-load and position arithmetic scheduling differ. Exact `30f5c`/`31024` remain intact. |
| `800311b0` | WIP, strict `92.148150000%` | Textured quad writer has 8/8 blocks, 5/5 branches, packet stores, and `AddPrim`; the semitransparency packet store moves across a branch delay slot and saved-register selection differs. |
| `80031850` | WIP, strict `84.710450000%` | World-model renderer's 40 blocks and 16 branches use the proven map-cell lighting and TMD calls. A depth-transform call is placed in the opposite corresponding CFG block, and address/register schedules differ; the observed branch still selects the same operation. |
| `80031d8c` | WIP, strict `94.654140000%` | Animated-object renderer has 7/7 blocks, 2/2 branches, and the same lighting/TMD calls. Retail retains the render-data argument in `s7`; compiled C reloads it from the stack and saves one fewer register. |
| `80032174` | WIP, strict `93.600000000%` | Resource visibility checks two map-cell coordinates with 5/5 blocks and 3/3 branches. Return-value and condition registers differ; exact resource siblings remain intact. |
| `8003c3e0` | WIP, strict `96.524826000%` | Actor-group position loop is called eleven times by `3c614` and once by effect dispatch. Calls, `player_state+0xe8` referent, 23/23 blocks, and 11/11 branches agree; retail uses an 80-byte frame and compiled C 88 bytes, with argument-load/register differences. The three adjacent group-position functions remain exact. |
| `8003d184` | WIP, unclaimed | The 0x248c-byte actor behavior dispatcher reaches animation, collision, and movement helpers through a large switch and unresolved indirect control. Its complete dispatch table and source boundary are not yet proved. |

## Collision, resource, and actor dispatch continuation

This exactly ten-function GAME.EXE batch follows the frame driver's proven
collision and render calls, the resource-range helpers called by that renderer,
and the map/actor/effect dispatch chain. Each address was checked against its
retail extent, disassembly and CFG, callers, callees, strings, current match
state, and adjacent source claims. A source-local trial moved a redundant
`pending = 0` assignment into the sparse-animation flush branch; it did not
resolve the differing branch topology and was discarded. Focused rebuilds
preserved exact neighbors in the resource and sparse-animation units. No
twelfth-batch source change was retained, and no unproved initialized table
or indirect callee was claimed.

| GAME VA | Final verdict | Confirmed connection or remaining boundary |
| --- | --- | --- |
| `8002c670` | WIP, unclaimed | Frame-driver collision-mask dispatcher calls `rcos`/`rsin`, occupancy and mask helpers, then eight scan/update pairs. Its apparent signed-halfword table beginning at `80067874` has no proved complete extent or owning source. |
| `800320b0` | WIP, strict `73.959180000%` | Two calls from `3247c` scan a 24×24 radius around a map-cell position. Retail and C have 10/10 blocks and 6/6 branches, while coordinate/address register assignment and loop setup differ. Exact resource siblings stay exact. |
| `800321d8` | WIP, strict `98.435900000%` | TMD queue helper has 3/3 blocks, one branch, and matching calls. Retail forms fixed arena address `8009b0a0` by signed `lui/addiu`; the provisional C literal emits `lui/ori`. The address's source mechanism is still unproved. |
| `80032274` | WIP, strict `90.933334000%` | Two frame-renderer calls update VAB stream slots over a range. All 13 blocks and seven branches agree; the compiler saves one extra register and places the audio queue call/loop increment differently. |
| `8003247c` | WIP, unclaimed | The 0xb70-byte scene renderer, called by exact frame driver `335a0`, reaches resource-range, world-model, animated-object, and map-mask helpers. Its 96-block body and shared render state need a complete source model. |
| `80033d3c` | WIP, strict `95.218390000%` | Animation morph accumulator is called by exact cache updater `34070`. Three `ScaleMatrix` calls, 17/17 blocks, and 10/10 branches are present; the skip/pending-flush branch lands at a different common tail. The two adjacent sparse-animation functions remain exact. |
| `80035894` | WIP, unclaimed | Placement controller called by `16820` invokes `34f90`/`35194` and collision probes. Its 64-block flow has an indirect jump and candidate pattern-band references near `8006787c`, without a proved table owner. |
| `80036ed4` | WIP, later sourced | Main-loop map-object controller calls placement, actor/collision, spatial sound, and angle helpers. Its 329 blocks include unresolved indirect jumps; see the latest dispatcher verdict below. |
| `8003c614` | WIP, unclaimed | Actor behavior controller, called by `3d184`, invokes exact actor-position siblings, `3c3e0`, and effect constructor `40308`. Its 45-block switch includes an unresolved indirect jump/table. |
| `80040308` | WIP, unclaimed | Large effect constructor reached by player equipment, actor behavior, and effect dispatch. Its 116-block body has an indirect jump and candidate `8006d704` table references; complete effect-data ownership is unproved. |

## Arena, menu, rendering, and actor animation continuation

This exactly ten-function GAME.EXE batch follows the arena users, menu display,
map renderer, and actor animation or spatial-sound calls. Retail disassembly,
CFG, callers, callees, strings, match state, and adjacent source claims were
checked for every address. The KF1 sibling was inspected: its actor animation
and map-render routines corroborate the broad roles, while its arena allocator
has a different structure and supplies no source-level fix for this GAME
allocator. Focused rebuilds preserved the exact neighbors below. A source-only
switch-arm ordering trial for `3b5d0` changed the CFG away from retail and was
discarded. No source or configuration edit was retained. Percentages are strict
objdiff results, not focused listing scores.

| GAME VA | Final verdict | Confirmed connection or remaining residue |
| --- | --- | --- |
| `80017608` | WIP, strict `99.782610000%` | Arena block allocator has matching 8/8 CFG blocks and 4/4 branches. Only the register chosen for `available - 12` differs in two instructions; the existing source has real `available` and `remainder` locals. Other CD-memory claims remain exact. |
| `80021c8c` | WIP, strict `99.956985000%` | Menu display-state saver has 7/7 blocks and 3/3 branches. Retail reserves a 32-byte frame, compiled C 24 bytes; no additional stack object is evidenced. Exact adjacent `21e00` is preserved. |
| `8002ce68` | WIP, strict `65.851850000%` | Floor-item free-slot helper has 5/5 blocks and 2/2 branches, but saves incoming stack arguments early and uses a 56-byte frame versus retail's 40 bytes. Adjacent `2ce2c` and `2cf40` remain exact. |
| `8002f5b0` | WIP, strict `95.833336000%` | Map renderer has 13/13 blocks, 7/7 branches, and the same clip, color, depth, and packet calls; saved-register and arithmetic schedules differ. Exact `render_enqueue_map` remains intact. |
| `8002f808` | WIP, strict `63.362473000%` | Companion map renderer has 51/51 blocks but 34 compiled versus 35 retail branches, a 112-byte versus 168-byte frame, and different return and packet-call ordering. Its source needs a stronger semantic model before further matching. |
| `800349bc` | WIP, strict `92.342960000%` | Menu transition has 14/14 blocks and 8/8 branches; retail reserves 72 stack bytes versus compiled C's 64, with different register and constant scheduling. Adjacent TIM upload and `34e10` remain exact. |
| `8003b5d0` | WIP, strict `75.669390000%` | Actor motion/collision switch has 40/40 blocks and 21/21 branches, but switch-arm and return-frontier order differ. Reordering source cases to the retail comparison order produced only 39 blocks and was discarded. Three adjacent actor-motion functions remain exact. |
| `8003d084` | WIP, strict `91.600000000%` | Actor spatial-sound pitch offset has 5/5 blocks and 2/2 branches. Retail forms `(clamped_offset - 2) + scaled_random`; compiled C reassociates the same expression. Exact caller `3d0e8` remains intact. |
| `8003f7ec` | WIP, strict `85.862070000%` | Actor target-group fixup has 9/9 blocks and 4/4 branches with the typed group base. Two constant initializations and commutative address arithmetic use different instruction order. Exact adjacent `3f610` and `3f860` are preserved. |
| `800460a0` | WIP, strict `99.268295000%` | Actor animation phase seeker has 6/6 blocks and 2/2 branches; only the saved-register choice for even step and half-step differs. Source-order and width trials have no independently supported correction. |

## Main-loop resource and actor-dispatch continuation

This exactly ten-function GAME.EXE batch follows the direct startup calls from
`game_main_loop`, its resource and map phase calls, and the actor selection and
behavior calls reached from those phases. Retail extents, disassembly/CFG,
callers, callees, strings, data references, current match state, and adjacent
source ownership were checked for each function. The KF1 `game.c`,
`resources.c`, and `actor_behavior.c` routines were used as source-shape
comparators; they do not prove KF2 data ownership or dispatch targets.

The one retained source change appends `game_main_loop` to the contiguous
`game.main` unit. Its nine startup clear sizes use complete typed BSS objects,
and its direct call sequence, camera arguments, and loop agree with retail.
Two fixed addresses remain unresolved source mechanisms. Focused comparison
preserves the adjacent `main` listing as SAME. Strict matching relinked all
147 GAME target units, though global edge-check still reports three previously
known unrelated `.rodata` addend divergences. A full pinned build produced
PSX.EXE; the overlays retain their known unresolved link symbols.

| GAME VA | Final verdict | Confirmed connection or remaining boundary |
| --- | --- | --- |
| `8001369c` | WIP, strict `99.675530000%` | Main loop has 5/5 CFG blocks, 2/2 branches, the direct calls and typed BSS clear extents. Retail forms fixed arena pointer `8009b0a0` with signed `lui/addiu`; the truthful C literal emits `lui/ori`. The arena source mechanism and one-word post-player-state flag at `80198630` remain unproved. Adjacent `main` stays exact. |
| `80015d58` | WIP, unclaimed | One-block common/map resource loader opens seven `COM` archives and copies nine length-prefixed payloads. Several destination and source owners remain unproved, including the provisional collision-cache interior and addresses outside the load image. |
| `80015fd4` | WIP, unclaimed | Three-block transition setup saves five callback-state bytes, yields through the CD service, and invokes callback-table slot five indirectly. Fixed TMD slot `8012da68` has no proved complete owner or source mechanism. |
| `80016260` | WIP, unclaimed | The 67-block transition state machine shares callback and resource phase state with `15fd4`; its payload boundaries and indirect callback targets remain unresolved. |
| `80016820` | WIP, unclaimed | The 62-block main-loop phase controller calls map, actor, and resource setup. Its indirect phase jump and candidate table at `80011058` do not yet establish a source-owned dispatch table. |
| `8002ff5c` | WIP, unclaimed | The exact map-cell renderer calls this 0xcbc-byte prepared-TMD packet builder. It has an 11-block CFG, a 1248-byte frame, and fourteen `resource_copy_words` calls; the packed packet schema and complete argument contract remain unproved. |
| `80039108` | WIP, unclaimed | Actor candidate scorer is called by target selection. Its 59 blocks use a 131-row candidate jump table at `80011cd8` with ten unique in-function targets; the indirect callback and table owner remain unresolved. |
| `80039c94` | WIP, source-backed, 50.3% focused | The 72-block combat helper is reached by actor, player, and effect paths and repeatedly calls `39c14`. Callback slot 18 remains unresolved; current C has 71 blocks and 46/46 branches. |
| `8003ae50` | WIP, source-backed, 99.2% focused | Motion/collision helper called by exact actor-motion routines takes a halfword motion vector and flags. Its C body models KF2 retry paths; the 58/58 blocks and 34/34 branches agree, with angle-register and mask-scheduling residue. |
| `8003c614` | WIP, unclaimed | The 45-block behavior dispatcher is called by `3d184`, calls exact actor-position siblings and `3c3e0`, and branches through an unresolved candidate switch table at `80011ee8`. |

## Card reader, map renderer, and animation follow-up

This exactly ten-function GAME.EXE batch revisited current strict WIPs whose
retail callers and adjacent source claims were already established. Retail
instruction widths and delay slots were checked against the current C for the
card readers, floor-item uploader, map-cell copier, and animation seeker.
Both card-reader digit loops load source bytes with `lb`, while the present C
compiles `lbu`. Changing the header and local byte views to signed types did
not change the pinned GCC listing; the trial was reverted. The map-cell
copier's 40-byte frame holds eight saved registers, so there is no evidence
for an extra local object. No source or configuration edit was retained, and
the current strict scores below remain the final verdicts. The exact adjacent
card-format, floor-item, map-cell, map-render, TIM, and animation claims were
preserved.

| GAME VA | Final verdict | First substantive residue or boundary |
| --- | --- | --- |
| `800228c8` | WIP, strict `85.156250000%` | Card save reader parses six experience and two level digits from a 640-byte header. CFG 24/24 and branches 13/13 agree; retail copies signed-loaded bytes through one halfword temporary, while the compiled loop uses unsigned byte loads and different loop-register lifetimes. Signed byte-view trials did not alter codegen. |
| `80022b74` | WIP, strict `93.666664000%` | Card payload reader has 9/9 blocks, 4/4 branches, and matching file, checksum, and load calls. Retail retains the slot in a separate saved register and has an 80-byte frame; current C uses a 72-byte frame and schedules the retry condition differently. |
| `8002ce68` | WIP, strict `65.851850000%` | Floor-item image uploader preserves the seven-argument call behavior and 5/5 blocks, but the compiler saves later stack arguments early and uses a 56-byte frame against retail's 40 bytes. Exact `2ce2c` and `2cf40` remain intact. |
| `8002f5b0` | WIP, strict `95.833336000%` | Map renderer has 13/13 blocks, 7/7 branches, and the same clip, color/depth, and packet calls; saved-register choice and an arithmetic schedule differ. Exact `render_enqueue_map` remains intact. |
| `8002f808` | WIP, strict `63.362473000%` | Companion map renderer has 51/51 blocks but 34 compiled versus 35 retail branches and a 112-byte versus 168-byte frame. Its return and packet-call ordering need a stronger source model. |
| `80030c18` | WIP, strict `96.521736000%` | Cell-object renderer has 11/11 blocks, 6/6 branches, and matching calls and referents. Four independent entry-setup instructions differ in order; exact `30f5c` and `31024` remain intact. |
| `80030de4` | WIP, strict `89.521280000%` | Two-layer map-cell drawer has 10/10 blocks and 6/6 branches. Its byte-load and position arithmetic scheduling differ, while exact `30f5c`/`31024` remain intact. |
| `800349bc` | WIP, strict `92.342960000%` | Menu transition has 14/14 blocks and 8/8 branches; retail's frame is 72 bytes versus current C's 64, with different register and constant lifetimes. Adjacent TIM upload and `34e10` remain exact. |
| `80035194` | WIP, strict `89.595450000%` | Rotated occupancy rectangle copier has 51/51 blocks and 26/26 branches. Retail saves eight registers in a 40-byte frame; compiled C uses fewer in a 32-byte frame and differs in field-mask and row/column register lifetimes. Its map-pattern sibling remains WIP. |
| `800460a0` | WIP, strict `99.268295000%` | Animation phase seeker has 6/6 blocks, 2/2 branches, and matching calls/referents. Only the saved-register assignment of the even step and half-step remains; no independently supported source correction was found. |

## Actor animation and target continuation after merge

This ten-function GAME.EXE batch is confined to actor animation, target,
group-position, and their proven motion/behavior calls. Current retail CFG,
call and referent evidence was checked against the merged source. Focused
`kf try` comparisons kept exact siblings exact. Moving a common yaw-scale
assignment, duplicating sparse-morph reset assignments, reversing a target
pointer addition, and reordering phase-seeker local declarations either
worsened or did not change the focused listing; all trials were reverted.
No source or configuration edit was retained.

| GAME VA | Final verdict | Evidence and remaining boundary |
| --- | --- | --- |
| `80015918` | WIP, strict `95.494190000%` | Fixed-point trajectory solver has 41/41 blocks and 24/24 branches; the arithmetic result register differs from the first discriminant subtraction onward. Exact adjacent trajectory caller `15bc8` and vector helper `15ce0` remain intact. |
| `80033d3c` | WIP, strict `95.218390000%` | Sparse morph accumulator has 17/17 blocks, 10/10 branches and three `ScaleMatrix` calls. The pending-flush skip branches to a different common tail; an explicit per-branch reset trial reduced the compiled CFG to 16 blocks and was reverted. Both preceding sparse-animation claims remain exact. |
| `80039108` | WIP, unclaimed | Actor candidate scorer is called by the target selector. Its 59-block body uses a 131-row candidate jump table at `80011cd8` with ten unique internal targets; the indirect callback and table owner remain unresolved. |
| `80039c94` | WIP, source-backed, 50.3% focused | Actor combat/magic recipient has 72 retail blocks and eight calls to exact `39c14`; current C has 71 blocks and 46/46 branches. Its indirect callback remains unresolved. |
| `8003ae50` | WIP, source-backed, 99.2% focused | Motion/collision helper reached by exact actor-motion functions has 58/58 CFG blocks and 34/34 branches, with matching terrain, trigonometric, angle, and `SquareRoot0` calls. Only angle-register assignment and mask scheduling differ in the focused listing. |
| `8003c3e0` | WIP, strict `99.645390000%` | Group-position solver has 23/23 blocks, 11/11 branches, matching calls and referents, and three exact siblings. Only the yaw-error and scaled-yaw register assignments differ; consolidating the scale assignment lowered focused similarity and was reverted. |
| `8003c614` | WIP, unclaimed | Actor behavior dispatcher is called by `3d184` and calls group-position helper `3c3e0` eleven times. Its 45-block switch retains an unresolved indirect jump and candidate table at `80011ee8`. |
| `8003d184` | WIP, unclaimed | Higher actor behavior dispatcher reaches animation, collision, movement, and sound helpers. Its 0x248c-byte body has multiple indirect control sites and lacks a proved complete dispatch-table/source owner. |
| `8003f7ec` | WIP, strict `85.862070000%` | Target-group fixup has 9/9 blocks and 4/4 branches with typed group ownership. Focused listing differs only in one constant initialization order and commutative pointer addition; reversing the C addition emitted identical bytes. Exact adjacent `3f610` and `3f860` remain intact. |
| `800460a0` | WIP, strict `99.268295000%` | Animation phase seeker has 6/6 blocks and 2/2 branches, with only even-step versus half-step saved registers swapped. Reordering the two local declarations emitted identical bytes. |

The remaining disjoint actor-owned source WIPs in this neighborhood were
checked as a small follow-up. `8003983c` is strict `99.075380000%`: its 37/37
blocks, 24/24 branches, calls, and referents agree, while saved-register
assignment differs from the first actor-state load. `8003bd40` is strict
`86.532260000%`: 9/9 blocks and 5/5 branches agree, but actor-position load
order and angle/reference register lifetimes differ; six neighboring motion
functions remain exact. `8003b5d0` is strict `75.669390000%`: 40/40 blocks and
21/21 branches agree, but state-dispatch and collision-return paths use a
different layout. An explicit layer-selection branch changed the first branch
shape but lowered focused similarity and was reverted. The source bodies in
other nearby non-exact units belong to the player, effect, or audio campaigns.

The actor-side damage pair was checked against retail calls, CFG, and focused
listings after the player attenuation correction. `8003a318` remains strict
`77.020940000%` WIP: its 26 blocks and 13 branches now match retail, but its
16-argument loop uses a 192-byte frame against retail's 200 bytes, with
different stack-argument scheduling. `8003a614` remains strict
`96.910110000%` WIP: its 6 blocks, 3 branches, and direct calls match, while
retail independently forms the later player-camera Z address and the compiler
reuses an earlier base. Exact `8003a778` was SAME in the focused comparison.
No source correction supported by independent evidence was retained.

For `8003b5d0`, reversing both equivalent collision-layer ternaries matched
retail's initial branch direction and raised focused listing similarity from
65.1% to 66.5%, but did not resolve the state-dispatch topology. The change
was reverted because it only steered code generation. Moving state `0x20`
ahead of state `0` in the C switch was also reverted without comparison when
the focused target object became unavailable during concurrent target work.
The three contiguous preceding functions remain exact in the last completed
focused comparison.

The later sparse-morph control-flow correction reached focused SAME for all
three claims in `game.animation_sparse_vertices`. Retail's skip with no
pending vertices preserves the already-reset delta pointer and jumps to the
shared group tail. A real flush or completed three-vertex batch resets that
pointer; a partial batch only increments its pending count. Expressing these
paths in C removed the earlier skip-tail and partial-batch branch differences.
`80033d3c` has a byte-identical focused listing, with `80033bfc` and
`80033cc0` still SAME. Broad strict matching was not run under the current
quick-build constraint, so this is a focused exact verdict pending strict
certification.

The contiguous `8003ae50` actor X/Z motion helper is now source-owned after
retail review of its three actor-motion callers, collision/floor callees,
cache references, and 58-block control flow. The C body models proposed
motion as a local `VECTOR`, the two floor probes, obstacle-angle steering,
axis and diagonal retries, and optional halfword motion writeback. Retail
evidence corrected the floor-height comparison, fixed-point negation before
the right shift, and the unscaled second-axis retry. A seven-function focused
carve and `kf try` preserve the six preceding actor-animation listings as
SAME; `8003ae50` remains WIP at 99.2% listing similarity. Its remaining
printed difference is the angle adjustment temporary (`a1` versus retail
`v0`) and the independent angle-mask scheduling before the length products.
No broad strict report was run under the quick-build constraint.

## Target-group dispatch and connected helpers

This 30-function GAME campaign follows the target-group lookup, actor magic,
motion, and trajectory calls. `kf try --no-flow` supplies focused listing
verdicts; SAME rows retain their previously established exact source, but no
new broad strict certification was run. Three decoded dispatcher table-base
HI16/LO16 pairs are reviewed. The indexed case words and original table/TU
ownership remain candidates; one-VA carves still withhold case-path relocs.

| GAME VA | Final focused verdict | Remaining evidence boundary |
| --- | --- | --- |
| `80039108` | WIP, unclaimed | 131-row switch; 28 case-path relocs withheld. |
| `8003983c` | WIP, 87.0% | Actor pointer and constant/chance use swapped saved registers; call and field model retained. |
| `80039c94` | WIP, 50.3% | Source-backed 72/71 CFG blocks, 46/46 branches; retail motion divisor is target-group byte +2 unless a linked actor supplies its group index. Its signed `< 240` gate is now reflected in a widened local; callback slot 18 remains unresolved. |
| `8003c614` | WIP, unclaimed | 123-row switch; 67 case-path relocs withheld. |
| `8003d184` | WIP, unclaimed | 241-row switch; 249 case-path relocs withheld. |
| `80015918` | WIP, 85.2% | Arithmetic-result registers differ in the trajectory discriminant. |
| `80015bc8` | SAME | Trajectory caller retains exact listing. |
| `80015ce0` | SAME | Scaled-vector helper retains exact listing. |
| `8003a318` | WIP, 97.5% | Two O32 argument temporaries differ. |
| `8003a614` | WIP, 81.2% | Player camera loads reuse a base register instead of retail's repeated pairs; no overlapping global was introduced. |
| `8003a778` | SAME | Actor target scan retains exact listing. |
| `8003a9f4` | SAME | Actor-distance collision scan retains exact listing. |
| `8003ab5c` | SAME | Sibling actor-distance scan retains exact listing. |
| `8003ae50` | WIP, 99.2% | Steering-angle temporary and independent angle-mask scheduling differ; six preceding claims remain SAME. |
| `800157ac` | SAME | Random-scalar helper retains exact listing. |
| `800157f8` | SAME | Adjacent random-scalar helper retains exact listing. |
| `8003b9a4` | SAME | Actor motion helper retains exact listing. |
| `8003bae4` | SAME | Actor motion helper retains exact listing. |
| `8003bba0` | SAME | Actor turn helper retains exact listing. |
| `8003bcd0` | SAME | Actor motion wrapper retains exact listing. |
| `8003bd40` | WIP, 68.2% | Entry setup and arithmetic registers differ, with identical direct call family. |
| `8003be38` | SAME | Actor vertical-motion helper retains exact listing. |
| `8003bf74` | SAME | Actor angle wrapper retains exact listing. |
| `800395c8` | SAME | Best-target selector retains exact listing. |
| `800396c4` | SAME | Player-distance target selector retains exact listing. |
| `80039710` | SAME | Group target-type search retains exact listing. |
| `80039758` | SAME | Own-group target selector retains exact listing. |
| `800390d0` | SAME | Target setter retains exact listing. |
| `800397a8` | SAME | Target reset/reselection retains exact listing. |
| `8003f7ec` | WIP, 93.8% | Group-pointer fixup has two scheduling/commutative instruction differences; adjacent update/loader claims remain SAME. |

The retail guards constrain the three switch spans without proving their
source owner: `39108` indexes `(type - 2) <= 0x82`, `3c614` indexes
`(selector - 1) <= 0x7a`, and `3d184` indexes `type <= 0xf0`. At `80011cd8`,
131 consecutive words contain ten distinct
in-function targets of `80039108`; the next word at `80011ee4` is zero. At
`80011ee8`, 123 words contain 19 distinct targets of `8003c614`, followed by
a zero word at `800120d4`. At `800120d8`, 241 words contain 28 distinct
targets of `8003d184`; the next word at `8001249c` is `80040698`. These are
decoded value ranges, not yet source-owned `RODATA` claims or promoted case
relocations.
The dominant default targets occupy 114, 100, and 212 entries respectively;
the remaining indexed cases include type `132` in `39108`, selector `123` in
`3c614`, and type `240` in `3d184`. The last case in each range therefore
matters even though most high entries share the default target.
The separate `jalr` sites also have traceable pointer chains without known
ultimate callees: `39108+0x45c` loads `state_8017d118.active_table[16]` and
passes its candidate and distance in `a0/a1`; `3d184+0x45c` loads slot 19
with the actor in `a0`; `3d184+0x223c` loads slot 17. These are callback
table indices, not resolved target identities.
The three direct `3d184` call sites to `3c614` at `3ee54`, `3ee9c`, and
`3eec0` show a wider ABI than the old four-live-register seed: `a0` is a
zero-extended byte from `actor->target` +0x0c, `a1` is a zero-extended
halfword from that target +0x18, and `a2` is a mode value (`-1` or `-2` on
two paths). The first
two calls also write O32 stack arguments at `sp+0x10` and `sp+0x14`.
At `3ee54`, the stream halfwords passed in `a3`, `sp+0x10`, and
`sp+0x14` are explicitly sign-extended, with a stream pointer in
`sp+0x18`. At `3ee9c`, `a3` and both stack halfwords remain zero-extended.
The `3eec0` path instead forms `a3` from the stream cursor and writes a
signed actor halfword to `sp+0x10`. These are distinct observed call forms,
not a single proved high-level parameter type.
The target pointer remains in `s4` throughout `3d184`; its furthest direct
read is `lhu` at +0x2a (`3e1ac`). The pointed record therefore covers at
least 0x2c bytes, beyond the current 0x16-byte common-prefix
`KfTargetCandidate` view, but its stride and complete extent remain unproved.
The meaning of `a3` and the stack values varies by script path and remains
unresolved; no source signature or identity was promoted from this alone.
Another `3d184` case at `3e0c0` calls the source-backed actor-to-player
damage gate `3a614` after `actor_animation_crossed_phase`. Its four register
arguments are `0`, then target bytes +0x0e, +0x0f, and +0x10; O32 arguments
5–8 are target halfwords +0x12, +0x14, +0x16, then byte +0x11. Retail
`3a614` loads all four later arguments with `lhu`, so the byte is promoted
for the call. This links a compact target-record parameter group to an
observed damage action without establishing the whole target record layout.
The first group is reused at `3e1a4`, `3e344`, and `3e6b8`; `3e1a4` ORs
`0x80` into its final byte argument. The `3e200` call uses a parallel
group at target offsets +0x1c..+0x25, with byte arguments at +0x1c..+0x1f
and three halfwords at +0x20/+0x22/+0x24. These direct reads prove two
packed damage-parameter spans, but not the enclosing record stride.
At `16820+0x358`, `resource_copy_words` copies 0xcb0 words to
`actor_state.target_groups` and then calls `3f7ec`. Its 0x32c0-byte copy
covers 40 group records (0x12c0 bytes) and the following 0x2000-byte
`actor_state.unknown_73a0` tail exactly. This establishes one archive-loaded
owner span and the fixup order, while leaving candidate stride and the tail's
internal layout unresolved.

## Actor animation and group control pass after e75f424

This 29-function focused pass follows the actor animation, movement, damage,
group-position, and target-state calls. `SAME` means an identical focused
listing in the current source; no new broad strict report was run. The only
source-only probe in this pass expressed the group-fixup pointer addition with
the operands reversed, then as an integer base plus relative offset; GCC
emitted the same two-instruction residue for both, so the tracked source was
left unchanged.

| GAME VA | Focused verdict | Evidence boundary |
| --- | --- | --- |
| `8003983c` | WIP 87.0% | Actor pointer and constant-one saved registers swap; calls and branch targets align. |
| `80039c14` | SAME | Exact curve leaf remains the control for `39c94`. |
| `80039c94` | WIP 50.3% | Retail-backed signed motion divisor retained; callback slot 18 and one CFG block remain open. |
| `8003a318` | WIP 97.5% | Two O32 argument-load temporaries differ. |
| `8003a614` | WIP 81.2% | Player camera references compile through one reused base, unlike retail's repeated pairs. |
| `8003a778` | SAME | Actor target scan stays identical. |
| `8003a9f4` | SAME; prior strict 100% | Filtered actor-distance scan stays identical. |
| `8003ab5c` | SAME; prior strict 100% | Unfiltered actor-distance scan stays identical; the available strict report predates the later `3ae50` append. |
| `8003acb4` | SAME | Current-actor binding stays identical. |
| `8003ad90` | SAME | Wrapped animation advance stays identical. |
| `8003adc4` | SAME | Clamped animation advance stays identical. |
| `8003ae20` | SAME | Crossed-phase test stays identical. |
| `8003ae50` | WIP 99.2% | 58/58 CFG blocks; angle result temporary and mask scheduling differ. |
| `8003b9a4` | SAME | Motion step helper stays identical. |
| `8003bae4` | SAME | Forward-vector movement stays identical. |
| `8003bba0` | SAME | Actor turn helper stays identical. |
| `8003bcd0` | SAME | Motion wrapper stays identical. |
| `8003bd40` | WIP 68.2% | Identical direct call family; setup and arithmetic registers differ. |
| `8003be38` | SAME | Vertical-motion helper stays identical. |
| `8003bf74` | SAME | Motion-angle wrapper stays identical. |
| `8003c000` | SAME | Group-position sibling stays identical. |
| `8003c10c` | SAME | Group-position sibling stays identical. |
| `8003c220` | SAME | Group-position sibling stays identical. |
| `8003c3e0` | WIP 95.4% | Yaw-error and scaled-yaw temporaries differ; CFG and calls align. |
| `8003d084` | WIP 92.6% | GCC reassociates the final sound-note `-2` around scaled random. |
| `8003d0e8` | SAME | Actor spatial-sound caller stays identical. |
| `8003f610` | SAME | Actor-frame scan stays identical. |
| `8003f7ec` | WIP 93.8% | Sentinel load order and commutative pointer addition differ. |
| `8003f860` | SAME | Actor-record loader stays identical. |

The `8003b5d0` vertical-motion sibling was also inspected as an adjacent
control-flow lead but is outside this 29-function count. Its focused listing
is WIP 65.1% with 40/40 CFG blocks and 21/21 branches; retail and C choose
different switch paths, and the current collision-cache view is still a
provisional interior of `bss_801c7540`. No source change was retained.

Two source-only arithmetic probes for `8003d084` left its residue unresolved.
Swapping the final addends still compiled at 92.6%; subtracting two from the
clamped offset before `rand()` fell to 90.6% because GCC filled the call delay
slot that retail leaves empty. The exact `8003d0e8` caller stayed identical,
and the tracked C source was not changed.

## Archive-loaded target record consumer pass

This 27-function pass follows the `16820` group-payload copy into the
`3f7ec` fixup, actor target selection, animation dispatch, and event
save/restore. Verdicts are current focused listings where source exists;
`WIP, unclaimed` means the retail function has no C body. Existing exact
sources were used as controls, not changed.

| GAME VA | Verdict | Target-record evidence |
| --- | --- | --- |
| `16820` | WIP, unclaimed | Copies 0xcb0 words to target groups, fixes pointers, loads actors, then restores saved state. |
| `38dc4` | SAME | Copies group defaults into actor fields. |
| `390d0` | SAME | Assigns actor target pointer and type. |
| `39108` | WIP, unclaimed | Scores candidate types through a bounded 131-word switch table; calls unresolved callback slot 16 on one path. |
| `395c8` | SAME | Scores up to 16 group target references. |
| `39710` | SAME | Searches the same 16 pointer slots by candidate type. |
| `39758` | SAME | Selects a typed target in the actor's group. |
| `397a8` | SAME | Clears actor target and reselects. |
| `3983c` | WIP 87.0% | Lifecycle path reads selected target; saved actor and sentinel registers differ. |
| `39c14` | SAME | Curve leaf called by actor target effects. |
| `39c94` | WIP 50.3% | Reads group curve halfwords and selected candidate; callback remains indirect. |
| `3c000` | SAME | Group-position control. |
| `3c10c` | SAME | Group-position control. |
| `3c220` | SAME | Group-position control. |
| `3c3e0` | WIP 95.4% | Group-position yaw temporaries differ. |
| `3c614` | WIP, unclaimed | Receives candidate-derived mode and position operands from three `3d184` calls; bounded 123-word switch table remains unowned. |
| `3d084` | WIP 92.6% | Target sound note arithmetic associates differently. |
| `3d0e8` | SAME | Reads selected target sound byte and calls `3d084`. |
| `3d184` | WIP, unclaimed | Actor dispatcher reads candidate through +0x2a; full record extent and indirect callback targets remain open. |
| `3f610` | SAME | Actor frame scan calls `3d184`. |
| `3f7ec` | WIP 93.8% | Fixes 16 relative target offsets per active group against the tail base. |
| `3f860` | SAME | Loads actor records and group defaults after fixup. |
| `46144` | SAME | Searches candidate byte stream for a marker. |
| `461a0` | WIP 86.6% | Byte-stream cursor registers differ; candidate stride remains unproved. |
| `462bc` | WIP 90.5% | Dispatches candidate event opcodes, preserving indirect callbacks. |
| `48554` | SAME | Saves type-0x70 candidate bytes +0x10/+0x13. |
| `489ac` | WIP 95.1% | Restores those two candidate bytes; sentinel/base registers differ. |

The save/restore pair confirms two mutable bytes in the candidate prefix,
but never indexes candidates by a record stride. `3d184` proves a minimum
pointed extent of 0x2c, while the 0x2000-byte archive tail proves only the
owner boundary. Neither observation supports a full candidate structure
size or count.
Two `lhu` reads at `3d184+0x74` and `+0x78` use candidate +0x0a as one
unsigned halfword, so the common-prefix view now types that field as `u16`;
its meaning remains unknown.
The same dispatcher reads candidate +0x0c and +0x16 both as `lhu` and as
`lbu` on different cases. Those overlaps are deliberate byte views of packed
halfwords in retail, not evidence for separate records. The current prefix
fields remain provisional until their per-case meanings are known.
One extended case also uses signed `lh` at +0x24 and +0x26 (`3e298` and
`3e274`), while other cases read those offsets unsigned. The tail therefore
needs case-aware signed and unsigned views before a complete C type is safe.

## Actor dispatcher callee pass after a169114

This 28-function pass follows the direct actor-control calls in `3d184` and
the group target fixup that prepares its candidate pointers. The retained
motion correction is the equivalent collision-layer comparison in `3b5d0`:
`layer == 0 ? 1 : 2` emits retail's initial `bnez` branch and raises its
focused listing from 65.1% to 66.5%. All three contiguous exact siblings
remain `SAME`. Moving the independent switch cases in a source-only probe
did not improve that function and was discarded.
Retail also branches back to the same successful vertical-step block from two
collision responses. Two equivalent source-only `goto` probes shared that
block and improved the listing to 67.1%, but changed the switch layout from
40/40 to 40/39 CFG blocks and added a saved register. Both probes were
discarded; the retained 66.5% source preserves the retail CFG count.
The KF1 `actor_behavior.c` vertical-state switch likewise shares `fall` and
`land` labels, a source-shape analogy only: its collision and gravity paths
are different and do not identify the KF2 function's original source.

| GAME VA | Focused verdict | Dispatcher connection |
| --- | --- | --- |
| `38d04` | SAME | Home-position setter reached through lifecycle setup. |
| `38efc` | SAME | Sets lifecycle and home position. |
| `38f20` | SAME | Scans lifecycle-one actor records. |
| `39710` | SAME | Finds a candidate by type. |
| `39758` | SAME | Selects the actor's own-group target. |
| `397a8` | SAME | Clears and reselects target. |
| `397d8` | SAME | Sets actor animation byte +0x0c. |
| `39804` | SAME | Sets that byte only when changed. |
| `3ad90` | SAME | Wrapped animation advance. |
| `3adc4` | SAME | Clamped animation advance. |
| `3ae20` | SAME | Detects a crossed animation phase. |
| `3ae50` | WIP 99.2% | Actor motion/collision callee; angle mask scheduling differs. |
| `3b33c` | SAME | Actor motion collision helper. |
| `3b520` | SAME | Actor trajectory helper. |
| `3b5bc` | SAME | Sets current actor movement state. |
| `3b5d0` | WIP 66.5% | Dispatcher calls it at `3f5c0`; branch direction now agrees, but switch and register lifetimes differ. |
| `3b9a4` | SAME | Decays and applies actor motion. |
| `3bae4` | SAME | Approaches forward-vector motion. |
| `3bba0` | SAME | Bounded yaw turn. |
| `3bcd0` | SAME | Current-actor motion wrapper. |
| `3bd40` | WIP 68.2% | Target geometry and motion dispatch remain open. |
| `3be38` | SAME | Vertical-motion helper. |
| `3bf74` | SAME | Motion-angle wrapper. |
| `3c000` | SAME | Group-relative position helper. |
| `3c220` | SAME | Animation event group dispatcher. |
| `3c614` | WIP, unclaimed | Three direct `3d184` calls use candidate-derived arguments and O32 stack values. |
| `3d184` | WIP, unclaimed | Main actor action switch consumes packed target records. |
| `3f7ec` | WIP 93.8% | Converts archive-relative target offsets to pointers before actor dispatch. |

The caller, CFG, data-reference, relocation, adjacency, and source-history
review for `3b5d0` is in `/tmp/actor_b5d0_*` for this local pass; its only
external proven caller is `3d184+0x243c`. Retail reads the collision layer
at `bss_801c7540+0x1180a`; the complete cache object boundary is still
provisional, so no new overlapping global was introduced.
The same retail body uses `lbu 5(s2)` for the group's post-step increment
and `lbu 6(s2)` for the second argument to `248a8`. The complete 0x78-byte
group extent was already proved, so its opaque +0x01..+0x08 span is now
split narrowly around those two byte fields; no semantic field names were
inferred. Exact group-copy, curve-leaf, and actor-loader controls remain
identical in focused comparison.
With the archive copy boundary proved, `3f7ec` now takes its byte-pointer
base directly from `actor_state.unknown_73a0`, the beginning of that loaded
tail. This is the same address as `target_groups + 40`; focused comparison
still reports 93.8% with only sentinel scheduling and commutative `addu`
operand order different, while `3f610` and `3f860` remain `SAME`.

## Actor target and motion focused pass after 39dd6fd

This 28-function pass follows the target-selection, animation, movement,
group-position, and group-fixup calls into actor dispatch. Each verdict is a
current focused listing; `SAME` is an identical listing, not a new broad
strict certification. No source-only trial below was retained.

| GAME VA | Verdict | First unresolved difference or control role |
| --- | --- | --- |
| `3983c` | WIP 87.0% | Actor, sentinel, and chance saved registers exchange roles; call and branch targets agree. |
| `39c14` | SAME | Fixed-curve leaf control for `39c94`. |
| `39c94` | WIP 50.3% | Initial stack argument schedule and register lifetimes differ; recipient callback remains indirect. |
| `3a9f4` | SAME | Filtered actor-distance scan. |
| `3ab5c` | SAME | Unfiltered actor-distance scan. |
| `3acb4` | SAME | Current-actor binding. |
| `3ad90` | SAME | Wrapped animation advance. |
| `3adc4` | SAME | Clamped animation advance. |
| `3ae20` | SAME | Animation phase crossing. |
| `3ae50` | WIP 99.2% | Angle mask and temporary schedule differ in two listing regions. |
| `3b33c` | SAME | Collision motion helper. |
| `3b520` | SAME | Trajectory setup. |
| `3b5bc` | SAME | Movement-state setter. |
| `3b5d0` | WIP 66.5% | Vertical-state dispatch differs after the same initial collision setup. |
| `3b9a4` | SAME | Actor motion step. |
| `3bae4` | SAME | Forward-motion approach. |
| `3bba0` | SAME | Bounded yaw turn. |
| `3bcd0` | SAME | Motion wrapper. |
| `3bd40` | WIP 68.2% | Current-actor position load order and argument register lifetimes differ. |
| `3be38` | SAME | Vertical-motion helper. |
| `3bf74` | SAME | Motion-angle wrapper. |
| `3c000` | SAME | Actor group-space position control. |
| `3c10c` | SAME | Actor group-position mode control. |
| `3c220` | SAME | Animation event group dispatcher. |
| `3c3e0` | WIP 95.4% | Yaw-error temporary uses `v1` rather than retail `a2`. |
| `3f610` | SAME | Actor frame scan and dispatcher caller. |
| `3f7ec` | WIP 93.8% | Sentinel load timing and commutative pointer addition remain. |
| `3f860` | SAME | Actor record loader after group fixup. |

Retail `3b5d0` checks vertical state `0x20` first, whereas the current
compiler orders the switch around `0x10`. A source-only special-case check
for `0x20` improved its listing to 68.0% and retained 40/40 CFG blocks, but
the branch landed one block later than retail. A plain if-chain lost two CFG
blocks; adding the shared collision-success tail lost one. These trials did
not prove the original source shape and were discarded. A source-only
`3c3e0` rewrite that computed the yaw shift once fell from 95.4% to 90.4%:
retail does execute the shift in both branch arms. Promoting `3bd40`'s angle
parameter to `s32` with an explicit `s16` use compiled identically at 68.2%,
so its existing narrower source signature was preserved.
Changing `3c3e0`'s masked yaw error to `u16` fell to 84.1% and selected
unsigned halfword loads and `sltiu`, unlike retail's `lh` and `slti`.
Using `s16` for that temporary also fell to 84.1% because of extra
extension, so the retained `s32` expression remains the closest truthful
model of the observed operations.
Changing `3ae50`'s obstacle-angle temporary to `u16` fell from 99.2% to
72.9%; the retained signed integer temporary is supported by the listing.
Default focused comparison keeps `3bd40` at 9/9 CFG blocks and 5/5 branches
despite its 68.2% register and instruction-order residue.

## Actor damage and behavior focused pass after 39dd6fd

This 30-function pass follows actor radial damage and behavior calls through
animation, motion, group position, and target fixup. Source-backed verdicts
are current focused listings. The `3c614` and `3d184` bodies remain unclaimed:
their retail indirect switches and candidate table ownership do not yet
support a complete C translation. No source-only probe in this pass was kept.
Default focused flow shows `39c94` at 72 retail versus 71 compiled CFG
blocks, with the same 46 branches and seven incoming return edges; its first
listed successor difference is the shared return block's renumbering.

| GAME VA | Verdict | Connection or residue |
| --- | --- | --- |
| `3a318` | WIP 97.5% | Actor radial-damage loop; two entry stack-argument temporaries exchange registers. |
| `3a614` | WIP 81.2% | Actor-to-player damage gate; compiler reuses a camera-position base where retail reloads two HI16/LO16 pairs. |
| `3a778` | SAME | Actor target scan and damage caller control. |
| `3a9f4` | SAME | Filtered actor-distance scan. |
| `3ab5c` | SAME | Unfiltered actor-distance scan. |
| `3acb4` | SAME | Current-actor binding. |
| `3ad90` | SAME | Wrapped animation advance. |
| `3adc4` | SAME | Clamped animation advance. |
| `3ae20` | SAME | Crossed-phase test. |
| `3ae50` | WIP 99.2% | Motion collision retry; angle mask schedule remains. |
| `3b33c` | SAME | Collision motion helper. |
| `3b520` | SAME | Trajectory setup. |
| `3b5bc` | SAME | Vertical-motion state setter. |
| `3b5d0` | WIP 66.5% | Vertical-state collision body retains retail 40/40 CFG blocks and 21/21 branches. |
| `3b9a4` | SAME | Motion step. |
| `3bae4` | SAME | Forward-motion approach. |
| `3bba0` | SAME | Bounded yaw turn. |
| `3bcd0` | SAME | Motion wrapper. |
| `3bd40` | WIP 68.2% | Motion-to-point caller; position-load schedule and argument registers differ. |
| `3be38` | SAME | Vertical-motion helper. |
| `3bf74` | SAME | Motion-angle wrapper. |
| `3c000` | SAME | Actor group-space position helper. |
| `3c10c` | SAME | Group-position mode helper. |
| `3c220` | SAME | Animation event group dispatcher. |
| `3c3e0` | WIP 95.4% | Yaw-error temporary register differs. |
| `3c614` | WIP, unclaimed | Three direct `3d184` calls; 123-word candidate switch at `80011ee8` and unresolved indirect jump at `3c7c0`. |
| `3d084` | Exact 100% | Centered random pitch jitter now matches the retail instruction and relocation listing. |
| `3d0e8` | SAME | Actor target sound wrapper. |
| `3d184` | WIP, unclaimed | Behavior dispatcher calls damage, movement, and group-position helpers; complete table and target-record layout unproved. |
| `3f7ec` | WIP 93.8% | Archive-loaded group target fixup; two instruction-order differences remain. |

Retail `3c614` has three proven direct calls from `3d184` and a 45-block
reachable CFG with an unresolved indirect jump. The 123 candidate pointer
rows at `80011ee8` do not promote the switch owner or the indirect branch to
proven control flow. Its complete source claim remains deferred. In `3a318`,
retail and C both load the halfword falloff and word amount/flags from their
O32 stack slots; only the selected `a0`/`v1` temporaries differ. `3a614`'s
extra retail camera-position relocations cannot be modeled as overlapping
interior globals. The exact `3a778` sibling was preserved.
Default focused comparison reports matching 26/26 CFG blocks and 13/13
branches for `3a318`, and 6/6 blocks and 3/3 branches for `3a614`.
Removing redundant `u16` falloff casts or spelling the amount/flags formal
as `u32` left `3a318` at the same 97.5% listing, so neither probe was kept.
Moving the masked amount initialization ahead of the damage-position flag
check fell to 84.6%; retail supports the existing source order.
For `3d084`, retail calls `rand`, multiplies by five, computes the clamped
offset minus two, then shifts the random product and adds. Expressing the
random term as centered pitch jitter, `offset + (scaled_random - 2)`, retains
the same range and emits the retail order. Focused comparison is 2/2 SAME
for `3d084` and its `3d0e8` neighbor; isolated direct objdiff reports 100%
for both (100/100 and 156/156 bytes), and all seven ordered `.text`
relocations agree. The `3d0e8` wrapper is called directly by the behavior
dispatcher at `3d184+0x1b0`; the pair remains in that actor call graph.
Moving the offset subtraction into a post-`rand` local
statement instead filled retail's empty call delay slot and fell to 90.6%.
Retail `3b5d0` reads the vertical-motion halfword unsigned on its successful
increment path, while other actor consumers read the same storage signed.
An explicit `(u16)` view on its three increments compiled identically at
66.5%; the shared signed field and existing source were left intact.
The only proven external direct caller of `3a318` is effect dispatcher
`3ff18+0x174`: it forwards falloff and packed amount/flags from its fifth
and sixth arguments, preserving the callee's
observed halfword and word loads without a new parameter identity.
For `3ae50`, fusing the obstacle-angle choice and wrap into one C assignment
emits the `andi` at retail's earlier point but adds an extra register move;
the source-only listing falls from 99.2% to 91.7%. The retained two-step C
spelling preserves the exact neighboring animation claims.

## Actor fixed-curve and target-scan continuation

The preceding 30-function verdict table remains current for unchanged actor
units. The connected fixed-curve pair has these focused verdicts:

| GAME VA | Verdict | Retail evidence and remaining difference |
| --- | --- | --- |
| `39c14` | SAME | Eight direct calls from `39c94`; exact curve-leaf control remains intact. |
| `39c94` | WIP 62.1% | The candidate scan now snapshots byte +2 before `rand`, advances the reference pointer in the null-test path, and checks its 16-entry count against `-1`. The linked-actor path reads target-group byte +2 as the motion divisor. Retail and C still have 72/71 CFG blocks and 46/46 branches; callback slot 18 remains unresolved. |

The retail scan loads candidate byte +2 once into a saved register before
calling `rand`; the earlier C expression read it again after the call. Retail
also advances the reference pointer in the null-test branch delay slot and
decrements its 15-based counter before comparing with `-1`. These source
corrections raise the focused listing from 50.3% to 59.2%. At
`39c94+0x498` through `+0x4d0`, retail uses the linked actor's group index
to select a 120-byte target group and loads its byte +2. The earlier C used
the group index itself as the divisor; correcting that lookup raises the
focused listing to 62.1%. The exact `39c14` sibling stays SAME. A source-only
`if/else` spelling of `3ae50`'s obstacle angle fell to 90.7%, so the
retained 99.2% source and its six exact animation neighbors remain unchanged.
The direct retail and pinned-compiler objects both contain 417 instructions
for `39c94`; equal body length does not resolve its 72/71 CFG split or the
remaining register and stack-allocation differences.
The KF1 `actor_apply_damage` counterpart accumulates component damage before
health and player-credit handling, which supports the broad source shape;
its five-component record does not establish KF2's eight-component field
names or callback identity.
Effect-owned `3fb94`, a direct caller of `39c94`, has since been certified
strict exact by isolated direct objdiff: 536/536 function bytes and all 36
ordered relocations in its complete unit match. Its older WIP rows above
record earlier checkpoints, not the current verdict.

### Newly sourced GAME object and target dispatchers

| GAME VA | Verdict | Evidence and limit |
| --- | --- | --- |
| `36944` | exact | The 396-object scatter wrapper is 116/116 bytes exact in direct objdiff; its contiguous map-object unit is 4/4 focused SAME. Its input source has no proved static caller, so the address-derived identity remains. |
| `39108` | WIP | The candidate scorer now has a complete C control path and a 131-word bounded switch table at `80011cd8`, all targeting this function. One-VA delink has zero withheld references. Retail's early zero return and current-target return before score doubling are restored; direct objdiff reports 91.414474% fuzzy text with 1216 retail versus 1196 compiled bytes. Focused CFG remains 59/56 blocks and 37/33 branches. The retail and compiled 524-byte tables have the same ten-target equivalence pattern across all 131 entries; their case-code offsets differ. The callback through active slot 16 remains indirect, and the candidate's complete record stride is unresolved. |
| `36ed4` | WIP, all primary arms sourced | The 396-object traversal, current object/template pointers, all 23 bounded primary action IDs (2, 3, 4, 5, 8, 9, 15, 16, 17, 18, 19, 22, 34, 81, 83, 84, 88, 89, 96, 97, 98, 224, 225), and fallback callback have retail-backed C paths. The three adjacent dispatch tables occupy `8001191c..80011cd7`. Action 4 rotates a linked object and tests a GTE-transformed offset against collision; action 19 grows a linked object's scale under the owned `game_counter_bytes[0x4c]` random gate; action 34 relocates the player camera. Action 81 follows four timer paths and latches an effect call; actions 83 and 89 follow decoded subaction tables; action 84 uses the owned `map_object_cell_patterns` at `80067890`. Actions 96–98 read the existing owner-relative `KF_COLLISION_CACHE_LAYER` view, without a new interior global; the cache subrecord boundary is still provisional. Nested C switches now emit all 238 retail table relocations, and the 239-word table target-group pattern has zero differing indices. Replacing the action-4 aggregate `SVECTOR` initializer with the three stack halfword stores proved at `80037518..80037524` removes an extra eight-byte constant, so compiled and retail `.rodata` are both 956 bytes. Retail's matching sparse timer dispatch for actions 2 and 3 is now expressed as two inner switches. Eleven missing HI16/LO16 pairs were added from raw signed-low instructions: six map-object pool refs, three collision-cache layer reads, one event-state byte, and one game counter byte. Retail action-3 `func_80035194` calls load the two map-object bytes with `lbu`; the shared spawn-byte view and `u8` field now preserve those widths at all four calls. Action 4 now uses the retail unsigned masked-angle comparison and the typed low/high spawn-byte view; its high-byte read compiles as the observed `lbu`. Focused comparison remains DIFF (27.6% listing); direct objdiff `.text` is 6.9942617% fuzzy, 7668 retail versus 7476 compiled bytes. The low fuzzy score reflects a different switch-case code layout and does not prove semantic completeness. One-VA delink has zero withheld relocs; no strict exact claim is made. |

The `39108` scorer reads candidate halfwords at offsets `+0x16` and `+0x1a`.
`KfTargetCandidate` therefore describes an observed 0x1c-byte prefix only;
neither these reads nor the switch table prove the allocation stride. For
`36ed4`, the retail first loop has 24 unique primary dispatch targets, of
which 201 table entries select the fallback callback. All 23 concrete action
IDs now have C paths, while subaction semantics and the compiled table layout
remain WIP. Actions 96–98 read the provisional collision-cache interior at
`801d8d4a` through the complete `bss_801c7540` owner. The cache subrecord
boundary remains WIP; the controller defines no overlapping global.
