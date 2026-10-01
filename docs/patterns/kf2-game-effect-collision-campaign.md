# GAME effect and collision campaign

This ledger records strict objdiff verdicts for the confirmed effect/collision
call graph. Addresses are GAME.EXE-qualified. `Exact` means matched code and
data are 100% in `build/objdiff/report.json`; listing-only `kf try` is not the
closure criterion. Source remains C under the pinned GCC 2.5.7 probe, whose
historical attribution is open. The related King's Field I collision query is
a different grid/pool implementation, so it is not used as a byte template for
the KF2 80-row collision structures.

## Effect pilot

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| 0x8003fa68 | WIP | Effect collision probe, 70.73% semantic first pass; call/CFG differences. |
| 0x8003fb94 | WIP | Damage path and ownership still unresolved. |
| 0x8003fdac | Exact | `effect_magic_power`, 0x24 bytes. |
| 0x8003fdd0 | Exact | Effect magic spawn, 0xe0 bytes. |
| 0x8003feb0 | Exact | Effect offset spawn, 0x68 bytes. |
| 0x8003ff18 | Exact | Effect emit, 0x1a8 bytes. |
| 0x800400c0 | Exact | Effect transform, 0xf4 bytes. |
| 0x800401b4 | Exact | Effect position, 0x6c bytes. |
| 0x80040220 | Exact | Pool free-slot search, 0x44 bytes. |
| 0x80040264 | Exact | Scaled pool initializer, 0x40 bytes. |
| 0x800402a4 | Exact | Fixed pool initializer, 0x64 bytes. |
| 0x80040308 | WIP | Large constructor; semantic/data-owner model incomplete. |
| 0x800416ec | Exact | Rotate and scale offset Y, 0x90 bytes; manually carved boundary. |
| 0x8004177c | Exact | Move/probe, 0x1e0 bytes. |
| 0x8004195c | Exact | Aim/move, 0x1b8 bytes. |
| 0x80041b14 | Exact | Target motion, 0x1bc bytes. |
| 0x80041cd0 | Exact | Scale step, 0xac bytes. |
| 0x80041d7c | Exact | Zero-direction spawn, 0x90 code plus eight initialized bytes at 0x8006d708. |
| 0x80041e0c | Exact | Height-window spawn is 136/136 strict exact, using the provisional startup-BSS collision lower-bound view without defining an overlapping global. |
| 0x80041e94 | Exact | Fresh isolated direct objdiff confirms all 664 code bytes; the earlier 92.65% report is stale. |
| 0x8004212c | Exact | Fresh isolated direct objdiff preserves all 364 code bytes while the signed BSS bound remains a provisional interior view. |
| 0x80042298 | WIP | BSS halfwords 0x801c7068–0x801c706c need an owner. |
| 0x80042424 | WIP | Same unresolved halfword group. |
| 0x800424f0 | Exact | Scatter, 0x160 bytes. |
| 0x80042650 | WIP | Large dispatcher with incomplete control/data model. |
| 0x80045cc0 | Exact | Effect pool reset, 0x30 bytes. |
| 0x80045cf0 | Exact | Magic record load, 0x2c bytes; manually carved boundary. |
| 0x80045d1c | Exact | Pool sweep, 0xfc bytes. |

## Connected collision campaign

This batch follows direct calls from the effect probe and shared collision-row
data. `External` denotes a function already owned by another campaign; it is
included to preserve the call-graph verdict without claiming its source.

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| 0x8001584c | External exact | Actor math owner; scalar Q12 interpolation is 100%. |
| 0x800158b4 | External WIP | Actor math owner; nine-halfword interpolation is 98% fuzzy. |
| 0x8002a988 | WIP | Collision grid query core; source and table owner unresolved. |
| 0x8002aaa4 | WIP | Large collision grid core with many candidate internal branches. |
| 0x8002b604 | WIP | Returns signed BSS bound at 0x801d8d50; enclosing owner unresolved. |
| 0x8002b67c | WIP | Map-grid and 0x801d8d4a–50 BSS owner unresolved. |
| 0x8002b73c | WIP | Typed occupancy-area source has the retail CFG and BSS referent; `kf try` is 75.5% listing with a register/row-pointer schedule residue. |
| 0x8002b7f8 | Exact | Height-adjusted collision wrapper, 124 bytes. |
| 0x8002b874 | WIP | Reads player/object state with unresolved data-field families. |
| 0x8002b9d4 | WIP | Collision dispatcher calls the geometry probes below; no source claim. |
| 0x8002bc18 | Exact | Initializes 80 rows, 292 code bytes and 3,520 initialized bytes. |
| 0x8002bd3c | Exact | Rotates 80 row matrices, 128 bytes. |
| 0x8002bdbc | Exact | Flagged row interpolation, 224 bytes. |
| 0x8002be9c | Exact | Updates 62 rows except index 38, 156 bytes. |
| 0x8002bf38 | Exact | Filter payload wrapper, 116 bytes. |
| 0x8002bfac | Exact | Clears 24×24 map-cell masks, 40 bytes. |
| 0x8002bfd4 | WIP | Typed segment rasterizer source has eleven branches and the 24×24 mask referents, but differs in loop instruction order (`kf try` 16.0% listing). |
| 0x8002c170 | WIP | Typed two-state row scan has matching CFG; 67.9% listing similarity with register/order residue. |
| 0x8002c1d4 | Exact | Fills between first/last matching cells, 188 bytes; shares a WIP unit with 0x8002c170. |
| 0x8002c290 | WIP | Reads unresolved 0x801b5a control group. |
| 0x8002c424 | WIP | Mask/line rasterizer, no source claim. |
| 0x8002c670 | WIP | Large collision-mask dispatcher, no source claim. |
| 0x80036078 | Exact | Scans 396 map objects with typed radius/height; 280 bytes. |
| 0x8003a9f4 | WIP | Actor-pool geometry probe; actor field family incomplete. |
| 0x8003ab5c | WIP | Companion actor-pool geometry probe; actor field family incomplete. |

The 80×44-byte mutable default table begins at 0x80066ab4 and ends at the next
datum, 0x80067874. Retail has nonzero entries only at indices 64–74; startup
can replace the table from an archive. The runtime's corresponding 80×104-byte
rows begin at `game_graphics_runtime+0x1506c`. The default angle is loaded as
unsigned, whereas the runtime angle is interpreted as signed by interpolation.
Those width facts recover the exact copy order in 0x8002bc18.

The pinned assembler emits section-relative `.data` relocations for locally
defined initialized data, including exported globals. `cd_path_prefix` and
`cd_version_suffix` in `game.cd_archive` independently confirm the same rule;
external data references retain named symbols. The delinker rule covers local
and exported same-module load data while leaving the separate static-BSS rule
unchanged. A durable control is present in `tests/test_delink.py`; repository
tests were not run during this concurrent campaign.

## Continued confirmed-call graph

The next batch follows proven callers of the occupancy writer, collision
dispatcher, actor probes and map-object query, plus the channel transition that
resets the same collision rows. `External` means another worker owns that source.
Large unclaimed functions are WIP triage verdicts, not speculative C claims.

| GAME VA | Verdict | Evidence or blocker |
| --- | --- | --- |
| 0x80016820 | WIP | 0x6b4-byte gameplay caller of the occupancy writer; broader control/data owner unresolved. |
| 0x80025a18 | WIP | 0x918-byte player caller of the exact height-adjusted collision wrapper; player state path unresolved. |
| 0x8002665c | WIP | 0xbd0-byte caller of the actor collision probe; large control/data owner unresolved. |
| 0x800274ec | WIP | 0x43c-byte caller of collision dispatcher and object-state probe; shares provisional collision/bounds state. |
| 0x800279cc | WIP | 0x5ac-byte caller of collision dispatcher and object-state probe; shares provisional collision/bounds state. |
| 0x80027f78 | WIP | 0x2ac-byte collision-dispatcher caller; unresolved same state and larger player flow. |
| 0x800293d4 | External exact | Player-owned damage-reaction helper, 0x54 bytes at 100%. |
| 0x80031634 | Exact | Three Q12-scaled collision-channel accumulations; 148/148 code bytes at 100%. |
| 0x800316c8 | WIP | 0x188-byte graphics setup reads a collision row, map cell and unresolved 0x801a state before SDK matrix calls. |
| 0x800335a0 | WIP | Frame-service caller of the collision mask dispatcher; several initialized data owners remain separate. |
| 0x80035894 | External WIP | Parent-owned map-object band; proven caller of occupancy writer. |
| 0x80036ed4 | WIP | 0x1df4-byte actor/map dispatcher calls collision dispatcher and occupancy writer; incomplete control/data model. |
| 0x80038edc | External exact | Actor-owned group initializer calls occupancy writer; source already exact. |
| 0x8003983c | WIP | 0x31c-byte actor-state caller of occupancy writer and actor collision probe. |
| 0x8003ae50 | WIP | 0x4ec-byte caller of collision dispatcher and object-state probe; actor field family incomplete. |
| 0x8003b33c | External exact | Player owner matched the 0x1e4-byte collision-dispatcher caller at strict 100% using actor-state +0x93a4. |
| 0x8003b5d0 | WIP | 0x3d4-byte collision-dispatcher caller with unresolved actor control flow. |
| 0x8003be38 | WIP | 0x13c-byte collision-dispatcher caller; argument/state model still incomplete. |
| 0x8003d184 | WIP | 0x248c-byte gameplay dispatcher calls occupancy writer and collision dispatcher; indirect/data ownership unresolved. |
| 0x80045e5c | Strict 100% | The 0xb4-byte event/collision probe calls `rsin`, `rcos`, and 0x8002b604, then tests the provisional collision pointer at 0x801d8d44; source is exact despite the unresolved enclosing BSS extent. |
| 0x80045f20 | Focused `SAME` (strict refresh pending) | Shared scene-pose helper has an identical focused listing; paired 0x80045fd4 is also focused `SAME`. |
| 0x800462bc | WIP, focused 90.5% | Provisional typed actor-target bytecode interpreter has matching CFG and bounded 16-entry table; action-handler load-delay schedule differs. |
| 0x800474c4 | Exact | Seven-argument three-channel transition, 276/276 code bytes at 100%; nine direct call relocations reviewed. |
| 0x800475d8 | Focused WIP, 89.4% | Source-backed variadic ABI, signed quarter-depth, flag lifetime, and shared outbound/return pose buffers reproduce the 120-byte frame; one CFG block and register residues remain. |
| 0x80047c98 | WIP | 0x660-byte interaction dispatcher calls actor probe, map-object selector and channel transition; unproven data/indirect owner. |

At 0x801d8d40..0x801d8d68, direct users establish a collision pointer,
vertical bounds, and result words. That address range overlaps the provisional
`equipment_records[20]` view inside the complete startup-cleared BSS. No
verified equipment index bound establishes a non-overlapping boundary, so the
collision dispatcher and its callers remain WIP; no second global or asserted
union owner was introduced.

## Floor-item and frame graphics continuation

This batch follows the `func_800335a0` frame-service call graph, the eight
24-byte floor items initialized by `display_reset`, and the textured-quad
callers. `External` means another campaign owns the source. Strict exact
verdicts below come from a fresh objdiff report; WIP percentages are fuzzy
diagnostics only. No source claim was added for a large dispatcher just because
it is adjacent.

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| 0x8002bfac | Exact retained | Clears the map-cell mask consumed in this frame path. |
| 0x8002c670 | WIP retained | Mask dispatcher called from 0x800335a0; larger raster data/control model unresolved. |
| 0x8002ce2c | Exact | Scans eight floor items for the 0xff free marker; 60/60 code bytes at 100%. |
| 0x8002ce68 | WIP | Typed seven-argument GPU capture uses the free slot, `memory_allocate`, `StoreImage`, `DrawSync`; CFG and calls agree, 65.85% fuzzy, with early stack-argument register allocation differing from retail. |
| 0x8002cf40 | Exact | Eight-item image rotation uses two `LoadImage` calls; the scroll offset wraps at rectangle height, and the typed record loop matches 356/356 bytes strictly. |
| 0x8002d0a4 | External exact | Display owner; fog distance setter called by 0x80031024 and 0x800316c8. |
| 0x8002d248 | External exact | Display reset establishes all eight free markers. |
| 0x8002d32c | External exact | Display frame begin called by 0x800335a0. |
| 0x8002d3c4 | External exact | Display frame present called by 0x800335a0. |
| 0x8002d4f4 | External exact | Display owner; camera transform called at frame-service entry. |
| 0x8002d918 | External exact | TMD projection owner; called by 0x800316c8. |
| 0x8002e4dc | WIP | 0x704-byte textured TMD primitive renderer with `NormalClip`, lighting and `AddPrim`; no source claim. |
| 0x80030f5c | External WIP | Parent-owned 24×24 map scan setup, called by the frame service. |
| 0x80031024 | WIP | 0x18c-byte model draw path uses an initialized record at 0x80066888, collision rows, TMD calls and matrix state; table ownership/fields incomplete. |
| 0x800311b0 | WIP | Typed 15-argument textured quad uses absolute right/bottom, SDK `POLY_FT4` and `AddPrim`; CFG/referents agree, 92.15% fuzzy with primitive-setup register/schedule residue. |
| 0x800312f4 | External exact | Sliding-panel owner; reads `player_state+0x120`, 144/144 bytes. |
| 0x80031384 | External exact | Sliding-panel owner; reads `player_state+0x124`, 144/144 bytes. |
| 0x80031414 | Exact | Draws the graphics color-byte overlay through 0x800311b0; 192/192 bytes at 100%. |
| 0x800314d4 | External exact | Actor owner; sets four color-control bytes. |
| 0x800314fc | Exact | Draws the three-channel collision overlay through 0x800311b0; 312/312 bytes at 100%. |
| 0x80031634 | Exact retained | Three-channel accumulator, 148/148 bytes. |
| 0x800316c8 | WIP | 0x188-byte collision-row render setup uses player/map state, SDK matrices and a large model draw call; no source claim. |
| 0x8003247c | WIP | 0xb70-byte frame-service child with 27 proven calls and broad state; no source claim. |
| 0x80033284 | WIP | 0x300-byte frame-service child with 41 outgoing references; no source claim. |
| 0x800335a0 | WIP | 0x3f4-byte caller ties the mask, floor-item, graphics, and display paths to many initialized data rows; no source claim. |
| 0x80034070 | WIP | 0x2d4-byte draw callee shared by 0x80031024 and 0x800316c8; object/data owner still incomplete. |

`KfFloorItem` remains a single 24-byte runtime object: kind and timer bytes,
a GPU `RECT` at +6, and pixel buffer pointer at +16. This is supported by the
free-slot scan, capture, image rotation, and display reset; no overlapping
interior global was created. The two exact quad callers depend on the helper's
retail 15-argument ABI even while its own instruction schedule remains WIP.

## Actor-group effect-dispatch graph

This 25-function audit follows proven calls from the actor/effect dispatcher at
0x8003c614 and its animation, geometry, spawning, and spatial-sound paths.
`External` means an adjacent campaign owns the source. Exact verdicts are from
the fresh strict objdiff report after the contiguous effect-unit consolidation.

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| 0x80013f50 | External exact | Default-range spatial sound, 52/52 bytes; direct dispatcher call. |
| 0x80015034 | External exact | Pitch/yaw vector conversion, 208/208 bytes; called by 0x3c3e0. |
| 0x80015188 | External exact | Q12 vector scaling, 92/92 bytes; called by 0x3c3e0. |
| 0x80015468 | External exact | Two-axis length, 64/64 bytes; direct dispatcher call. |
| 0x800154a8 | External exact | Three-axis length, 84/84 bytes; called by 0x3c3e0. |
| 0x800154fc | External exact | Vector-to-angle helper, 120/120 bytes; called by 0x3c3e0 and dispatcher. |
| 0x8001584c | External exact | Q12 scalar interpolation, 32/32 bytes; direct dispatcher call. |
| 0x8001586c | External exact | Angular interpolation, 72/72 bytes; called twice by 0x3c3e0. |
| 0x80015918 | WIP | Unclaimed dispatcher callee; function ABI and source owner not yet established. |
| 0x80015ce0 | External exact | Vector scaled-add, 112/112 bytes; called by 0x3c3e0. |
| 0x80034344 | WIP | Unclaimed animation-vertex query used by exact 0x3c000. |
| 0x80038cc8 | External exact | Actor free-slot search, 60/60 bytes; direct dispatcher call. |
| 0x80038e38 | External exact | Group actor initialization, 196/196 bytes; direct dispatcher call. |
| 0x80039758 | External exact | Selects target type in actor's own group, 80/80 bytes; direct dispatcher call. |
| 0x800397d8 | External exact | Sets actor animation byte, 44/44 bytes; called by 0x3c220. |
| 0x80039804 | External exact | Changes actor animation byte if needed, 56/56 bytes; called by 0x3c220. |
| 0x8003c000 | Exact | Animated actor vertex transform, 268/268 bytes; signed fallback offsets and rotation are typed. |
| 0x8003c10c | Exact | Actor/group position selector, 276/276 bytes; group offsets at +0xc/+0xe/+0x10. |
| 0x8003c220 | Exact | Signed-motion animation selection, 448/448 bytes; 32/32 CFG blocks and 21/21 branches. |
| 0x8003c3e0 | WIP | 0x234-byte animated movement helper with seven proven geometry/math calls; the formerly misdecoded 0x801a85b8 reference is `player_state+0xe8` at 0x801985b8. |
| 0x8003c614 | WIP | 0xa70-byte effect/actor dispatcher; indirect jump table at 0x80011ee8 is only a candidate data owner. |
| 0x8003d084 | External WIP | Actor sound-note helper is 91.6% fuzzy in the strict report. |
| 0x8003d0e8 | External exact | Actor spatial-sound wrapper, 156/156 bytes. |
| 0x8003d184 | WIP | 0x248c-byte caller of 0x3c220 and 0x3c614; large dispatch/data model remains open. |
| 0x8003f7ec | External WIP | Group-target fixup has correct 116-byte extent but is 85.86% fuzzy. |

The nine adjacent effect functions from 0x8003fdac through 0x80040308 now share
one source unit and remain strict 1372/1372 code bytes. The reset, magic-record
load, and pool sweep at 0x80045cc0 through 0x80045e18 also share one unit and
remain strict 344/344. This removes seven GAME modules while preserving every
previously exact function and each original address claim. The actor-group trio
at 0x8003c000 through 0x8003c220 is strict 992/992 bytes; the following
0x8003c3e0 body remains WIP. Global `kf match`
still exits at the existing known-reference ownership closure, with one
relocation pair and one relocation site crossing owners; the fresh objdiff
report itself shows all three units at 100% code and data.

## Archive actor records and effect targets

This 27-function continuation follows the exact actor-group state into the
archive actor loader, and follows the exact effect emit/pool functions through
their confirmed callers and target-selection callees. Some WIP nodes were
revisited because the new 16-byte archive-record layout and strict reports
provide stronger boundaries; a revisited node is not counted as a new match.

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| 0x80016820 | WIP | Archive caller passes the section payload at +4 to 0x3f860; wider transition and indirect control remain unresolved. |
| 0x800152ac | External exact | Angle tolerance helper, 60/60 bytes; called by actor target update. |
| 0x80015318 | External exact | Horizontal vector angle, 336/336 bytes; called by actor target update. |
| 0x800155a4 | External WIP | Five-argument point-distance helper has 87.95% fuzzy code in the strict report. |
| 0x80024034 | External exact | Physical-power training increment, 152/152 bytes; called by 0x39c94. |
| 0x800240cc | External exact | Magic training increment, 152/152 bytes; called by 0x39c94. |
| 0x80024164 | External exact | Experience update, 544/544 bytes; called by 0x39c94. |
| 0x80024ca4 | External exact | Player-side radial effect damage wrapper called by 0x3ff18; player owner reconstructed its 16-argument ABI and matched 560/560 bytes. |
| 0x80025878 | External exact | Player attack-target caller of 0x3a778, 416/416 bytes. |
| 0x80026330 | External exact | Player weapon-transform caller of effect dispatch, 308/308 bytes. |
| 0x80038d04 | External exact | Actor home-position setter, 192/192 bytes; called by new 0x3f860. |
| 0x80038dc4 | External exact | Actor group-default copier, 116/116 bytes; called by new 0x3f860. |
| 0x800390d0 | External exact | Actor target setter, 56/56 bytes; called by 0x39c94. |
| 0x80039710 | External exact | Group target search, 72/72 bytes; called by 0x39c94. |
| 0x80039c14 | External exact | Fixed-point actor curve, 128/128 bytes; called eight times by 0x39c94. |
| 0x80039c94 | WIP | 0x684-byte actor target update; an indirect call and broad player/actor state remain unowned. |
| 0x8003a318 | WIP | 0x2fc-byte actor-side effect emitter called directly by exact 0x3ff18; no source claim. |
| 0x8003a778 | WIP | 0x27c-byte scan for nearest valid actor target, called by player/effect paths; angle and distance call set proved, source not claimed. |
| 0x8003a9f4 | WIP | 0x168-byte actor geometry probe called by effect dispatcher; actor field family incomplete. |
| 0x8003ab5c | WIP | 0x158-byte companion geometry probe called by effect dispatcher; actor field family incomplete. |
| 0x8003f7ec | WIP retained | Group-target offset fixup is 85.86% fuzzy; CFG/referent match, two instruction-order/operand-order residues. |
| 0x8003f860 | Exact | New 200-record archive actor loader, 460/460 bytes, appended to the preceding WIP unit without a new module. |
| 0x8003fa68 | WIP retained | Effect collision probe remains 70.73% fuzzy in the strict report. |
| 0x8003fb94 | WIP retained | 0x218-byte effect damage caller; full player/data owner is not established. |
| 0x80040308 | WIP retained | 0x13e4-byte effect constructor calls the exact pool initializers; jump/data model remains incomplete. |
| 0x80041e94 | WIP retained | 0x298-byte motion spawn has correct CFG and call set; 92.65% strict fuzzy code with argument/register schedule residue. |
| 0x80042650 | WIP retained | 0x3670-byte effect dispatcher calls the exact emit/spawn and actor-group helpers; incomplete indirect/data ownership. |

The archive actor loader copies each 16-byte record into one 0x7c-byte actor
slot, then applies group defaults and home position. Its +0x09 group byte and
+0x1c halfword uses refine the existing 0x78-byte target-group type without
changing its extent. The two actor-state BSS pairs and four direct J/JAL sites
in the loader were reviewed against retail words; the function is strict
100% in the fresh objdiff report. The preceding fixup function remains WIP in
the same contiguous unit.

## Animation vertex cache and effect callers

This 28-function continuation follows proven calls from the render cell,
weapon, actor-group, and effect-update paths into the animation vertex cache.
`External` means another campaign owns the source. Strict verdicts are from
the refreshed objdiff report; unclaimed functions remain WIP. Earlier WIP
entries for 0x80034070 and 0x80034344 are superseded below.

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| 0x80026330 | External exact | Weapon transform calls the animated-vertex query; 308/308 bytes. |
| 0x8002665c | WIP | Wider weapon caller of the animated-vertex query; no source claim. |
| 0x8002d4a8 | External exact | Current TMD vertex-pointer setter called by the cache; 16/16 bytes. |
| 0x8002d4b8 | External exact | Object vertex-pointer selector called by the cache; 60/60 bytes. |
| 0x80031024 | External exact | Render-cell caller of the cache updater; 396/396 bytes. |
| 0x800316c8 | WIP | Render caller of the cache updater; no source claim. |
| 0x80031850 | WIP | Render caller of the cache updater; no source claim. |
| 0x80031d8c | WIP 94.65414% | Typed animation/cache renderer in `render_animated_object.c`; seven-block CFG, call set, and referents align, but saved-register/frame lifetime differs from retail. |
| 0x80031fa0 | External exact | Asset registry lookup used by the animated-vertex query; 104/104 bytes. |
| 0x80033afc | External exact | Asset selection used by the cache; 56/56 bytes. |
| 0x80033b34 | External exact | Animation keyframe selection shared by both new functions; 200/200 bytes. |
| 0x80033bfc | External exact | Sparse vertex expansion used by the cache; 196/196 bytes. |
| 0x80033cc0 | External exact | Sparse vertex decode used by the cache; 124/124 bytes. |
| 0x80033d3c | WIP source | Sparse morph accumulator has a typed, three-ScaleMatrix semantic source, 93.9% focused listing and 95.21839% strict fuzzy; same call set and 10/10 branches, with a CFG layout and delay-slot residue. |
| 0x80033ff4 | External exact | Sparse vertex lookup used by the animated-vertex query; 124/124 bytes. |
| 0x80034070 | Exact | Cache update, allocation, sparse decode, interpolation, and vertex selection; 724/724 bytes. |
| 0x80034344 | Exact | Copies a base TMD vertex or interpolates a sparse animated vertex; 672/672 bytes. |
| 0x800345e4 | Exact retained | Adjacent asset vertex-count query, 96/96 bytes. |
| 0x80034644 | External exact | Cache pool reset, 48/48 bytes. |
| 0x80034674 | External exact | Pool allocation-state setter, 60/60 bytes. |
| 0x800346b0 | External exact | Record release called by the cache, 72/72 bytes. |
| 0x800346f8 | External exact | Pool-wide release called by the cache, 108/108 bytes. |
| 0x80034764 | External exact | Stale-record release, 108/108 bytes. |
| 0x800347d0 | External exact | Pool allocation called by the cache, 72/72 bytes. |
| 0x80034818 | External exact | Related map placed-object expansion uses the same animation assets; 308/308 bytes. |
| 0x800369b8 | External exact | Map-object vertex transform calls the animated-vertex query; 288/288 bytes. |
| 0x8003c000 | Exact retained | Actor-group vertex transform calls the animated-vertex query; 268/268 bytes. |
| 0x800400c0 | Exact retained | Effect update calls the animated-vertex query; 244/244 bytes. |

The contiguous 0x80034070–0x80034644 unit is 1492/1492 code bytes and all
three function listings are identical. The asset header's +4 word is the
zero/nonzero animation branch input; +0xc is a word offset into animation
data, with its precise meaning provisional. The pool record's +8 word is an
asset-relative morph offset, not a pointer until the asset base is added.
The render destination at graphics runtime +0x12a54 remains opaque: its
1000-vector extent and owner need independent consumer evidence before a
shared typed field is justified. Global strict matching still reports the
known-reference closure separately from these three exact function results.
The adjacent morph accumulator now shares the sparse-decoder unit; both
preceding exact decoder listings remain identical.

## Camera cell, map motion, and effect dispatch

This 28-function audit follows the new camera-cell query's proven render,
map-script, and event callers, then their actor and effect callees. Exact
statuses use the fresh strict objdiff report. External sources are owned by
the map, actor, player, or audio campaigns; WIP rows have no exact claim.

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| 0x80015104 | External exact | Map-object motion calls the rotated-vector helper; 68/68 bytes. |
| 0x80015318 | External exact | Actor update calls the horizontal angle helper; 336/336 bytes. |
| 0x8001584c | External exact | Map-object motion calls fixed-point interpolation three times; 32/32 bytes. |
| 0x8002b604 | External WIP | Collision height helper called by 0x45e5c is 99.333336% fuzzy; collision cache extent stays provisional. |
| 0x8002b67c | External WIP | Paired collision cache helper is 94.895836% fuzzy; +0x11800 BSS ownership remains provisional. |
| 0x8003247c | WIP | Render caller of the camera-cell query has no source claim. |
| 0x80035504 | External exact | Spatial sound called by map-object motion; 48/48 bytes. |
| 0x80035590 | External exact | Map-object reset called by motion; 72/72 bytes. |
| 0x800369b8 | Exact retained | Animated map-object vertex transform, 288/288 bytes. |
| 0x80036ad8 | Exact | New camera-cell rectangle and height-window query, 144/144 bytes; five call arguments and three player-camera relocations reviewed. |
| 0x80036b68 | External exact | Adjacent map-object motion helper, 700/700 bytes. |
| 0x80036e24 | External WIP | Frame/CD service helper is 98.86364% fuzzy in the strict report. |
| 0x80036ed4 | WIP | Wide map script calls 0x36ad8 and 0x36b68; indirect/data graph remains open. |
| 0x80038f20 | Strict 100% | The 200-record actor scan invokes callback slot 19 through `state_8017d118.active_table`, then advances the matching actor's lifecycle; 208/208 bytes exact, dynamic callback target remains unresolved. |
| 0x800396c4 | External exact | Actor target-distance selector called by 0x3f610; 76/76 bytes. |
| 0x8003acb4 | External exact | Actor bind helper called by 0x3f610; 220/220 bytes. |
| 0x8003d184 | WIP | Wide actor dispatcher called by 0x3f610; no source claim. |
| 0x8003f610 | WIP | Frame actor scan has confirmed bind/target/angle calls, but its wider state and dispatcher paths are unclaimed. |
| 0x80041e0c | Exact | Effect constructor caller is strict 136/136 exact; its signed collision bound at 0x801d8d58 remains a provisional interior view, not a separate global. |
| 0x80041e94 | WIP source | Motion spawn preserves semantics and call set but is 92.650604% strict fuzzy. |
| 0x8004212c | WIP | Randomized effect spawn calls the constructor and reads the same provisional 0x801d8d58 bound. |
| 0x80042298 | WIP | Paired collision probes call 0x3fa68; BSS halfwords 0x801c7068–0x801c706c still lack an owner. |
| 0x80042424 | WIP | Effect dispatcher helper adjusts the current record's position, but also reads those unowned 0x801c7068–0x801c706c halfwords. |
| 0x80042650 | WIP | Main effect dispatcher calls the compact effect/motion helpers; broad indirect and state ownership remains open. |
| 0x80045e5c | Strict 100% | The trigonometric event/collision probe is exact in `game.audio_sound_wrappers`; the collision-cache BSS owner remains provisional. |
| 0x80045f20 | External exact | Adjacent event pose interpolation, 180/180 bytes. |
| 0x8004678c | WIP | Event dispatcher calls 0x36ad8, 0x38f20 and 0x45e5c; no source claim. |
| 0x80047c98 | WIP, focused 85.1% | Event caller of 0x45e5c; source claims the controller and retains its indirect callback. |

The two-function 0x800369b8–0x80036b68 unit is strict 432/432 bytes, with
both listings identical. Its new leaf checks camera X/Z after a signed
right shift by 11 and accepts a height sentinel of 0x8000; otherwise it
compares camera Y against height +2048 and height -3200. Three reviewed
HI16/LO16 pairs now target the existing player-state camera fields. The
collision cache and the three halfwords before player weapon records remain
unowned, so the related dispatcher nodes are deliberately WIP.

## Ten-function render and resource batch

This bounded batch covers ten GAME functions from the frame renderer's proven
calls and the asset/map helpers it reaches. Scores below are from the strict
objdiff report after the render-frame source and relocation refresh. An exact
listing is 100%; a complete CFG or a high fuzzy score does not change a WIP
verdict. Earlier preliminary rows above are superseded for these addresses.

| GAME VA | Verdict | Evidence and remaining work |
| --- | --- | --- |
| 0x800311b0 | WIP, 92.14815% | Typed textured quad has the retail call set, referents, and 8/8 CFG blocks; primitive setup register and instruction order still differ. |
| 0x80031634 | Exact, 100% | Three Q12-scaled collision-channel accumulations; 148/148 code bytes. |
| 0x800316c8 | Exact, 100% | Player weapon render setup and collision-row consumer; 392/392 code bytes. |
| 0x80031d8c | WIP, 94.65414% | Typed animated-object renderer has matching call set, referents, and seven-block CFG; saved-register and frame lifetimes differ. |
| 0x8003247c | WIP, no source claim | The 0xb70-byte frame child has 27 decoded direct calls, including five to 0x80031850. Its 0x80063dcc identity `MATRIX` has 32 retail bytes and two reviewed address references; broader actor, map, and effect record ownership remains unresolved. |
| 0x80033d3c | WIP, 95.21839% | Sparse morph accumulation uses the typed asset/pool model and three `ScaleMatrix` calls; focused CFG has 17/17 blocks and 10/10 branches, but branch layout and delay-slot order differ. |
| 0x80034070 | Exact, 100% | Asset cache update, sparse decode, and interpolation; 724/724 code bytes. |
| 0x80034344 | Exact, 100% | Base or animated TMD vertex selection; 672/672 code bytes. |
| 0x800345e4 | Exact, 100% | Adjacent asset vertex-count query; 96/96 code bytes. |
| 0x80036ad8 | Exact, 100% | Map-object camera-cell and height-window query; 144/144 code bytes. |

The separate frame-driver claims are WIP: 0x80031850 is 84.65075% strict
(40/40 CFG blocks, 16/16 branches), and 0x800335a0 is 99.70356% strict
with its initialized four-byte yaw accumulator matching exactly. The latter
has only HP/MP division temporary-register ordering left in the focused
listing; no unsupported source manipulation was made to force that schedule.

## Ten non-exact floor, frame, and effect functions

This next bounded batch began with ten non-exact or unclaimed GAME functions
connected by the frame driver's floor-item work, its model rendering, or
effect/event calls. Strict scores come from the refreshed objdiff report;
unclaimed functions have no percentage. Earlier WIP rows above are
superseded where a new exact result is recorded here.

| GAME VA | Verdict | Evidence and remaining work |
| --- | --- | --- |
| 0x8002ce68 | WIP, 65.85185% | The seven-argument GPU capture has the retail four-call set and 5/5 CFG blocks; the pinned C object retains extra saved registers for stack arguments. Five `game_main_loop` call sites pass all seven arguments. |
| 0x8002cf40 | Exact, 100% | Eight-record floor-image scroll now wraps at `RECT.h`, as proved by retail `lh` at item +0xc; a typed record loop and two `LoadImage` calls match 356/356 bytes. |
| 0x80031850 | WIP, 84.65075% | World-model renderer has reviewed player/BSS referents and 40/40 CFG blocks with 16/16 branches; entry register, stack lifetime, and pointer-computation order remain different. |
| 0x800335a0 | WIP, 99.70356% | Frame driver has matching calls, 48/48 CFG blocks and 28/28 branches; only HP/MP division temporary-register assignments differ. Its initialized four-byte yaw accumulator matches exactly. |
| 0x80036ed4 | WIP, no source claim | The 0x1df4-byte map-script dispatcher calls 0x80041e0c and 0x80036ad8; its indirect jump through 0x8001191c and the candidate pointer table have unresolved ownership. |
| 0x8003fa68 | WIP, 70.73333% | Current-effect cooldown gate and four case-specific collision modes are decoded; retail emits four calls while the probe compiler folds them to one, giving 14 retail versus 15 compiled CFG blocks. A case-local-return experiment did not improve the shape and was reverted. |
| 0x80040308 | WIP, no source claim | The 0x13e4-byte effect constructor uses exact pool initializers, but its indirect jump through candidate table 0x8001249c and associated record/data owners remain unresolved. |
| 0x80041e0c | Exact, 100% | New 136-byte height-window spawn uses the existing provisional collision-bound interior view and calls the constructor with kind 0x66; its adjacent 0x80041d7c listing remains exact. |
| 0x80041e94 | WIP, 92.650604% | Motion spawn has matching 14/14 CFG blocks, 5/5 branches and call set; remaining differences begin with saved-register and stack-argument lifetimes. Adjacent 0x8004212c remains exact. |
| 0x80047c98 | WIP, focused 85.1% | The 0x660-byte event/effect controller calls the collision probe at 0x80045e5c, event-counter interpolation and map-object selection, then an unresolved indirect callback near its tail. Its source claim is provisional. |

The exact-count movement in this batch is two functions, 0x8002cf40 and
0x80041e0c. Neither source introduces an overlapping definition for the
provisional collision cache. Strict GAME target relink verified 139/139 units;
the global known-reference check still stops at three unrelated TMD/map-object
`.rodata` addend differences and unresolved data/control ownership.

## Ten map-pattern, render, and event functions

This fresh ten-function GAME batch follows the frame renderer through map-cell
placement and the effect dispatcher into event handling. Each function had a
retail address/disassembly/CFG, incoming and outgoing xref, string, and match
pass. None is attributed to the supplied vendored archives. Strict scores are
from the refreshed GAME objdiff report; unclaimed bodies have no score.

| GAME VA | Verdict | Evidence and first unresolved difference |
| --- | --- | --- |
| 0x800311b0 | WIP, 92.14815% | Typed `POLY_FT4` construction has matching 8/8 CFG blocks and 5/5 branches. Retail saves one extra argument register and orders primitive mode/texture stores differently from the probe. |
| 0x80031d8c | WIP, 94.65414% | Animated-object renderer has the same seven CFG blocks and two branches. Retail saves the late render-data argument in `s7`; the probe reloads it from the stack, with different entry scheduling. |
| 0x8003247c | WIP, unclaimed | The 0xb70-byte frame child has 27 direct calls and 50 outgoing references, including five world-model renders and the 32-byte identity matrix. Its mixed map/actor/render state is not yet a complete typed owner. |
| 0x80033d3c | WIP, 95.21839% | Sparse morph accumulator keeps its three `ScaleMatrix` calls and exact neighbors. Both listings have 17 blocks and 10 branches, but retail shares a far loop tail after the skip flush where the probe uses a nearer exit. |
| 0x80034f90 | WIP, 97.86822% | Typed two-layer map-cell pattern writer has matching 14 blocks and seven branches. The first differences are parameter/saved-register assignment and one independent cell-address instruction order. |
| 0x80035194 | WIP, 89.59545% | Explicit rotation case 3 and source-before-destination lighting-byte reads raised strict similarity from 86.086365%; the CFG now matches 51/51 blocks and 26/26 branches. Remaining differences start with stack/saved-register assignment and inner-loop mask scheduling. |
| 0x80042650 | WIP, unclaimed | The 0x3670-byte effect dispatcher has many directly decoded calls but its indirect dispatch/data-table owner and several record views remain unresolved; no body was claimed. |
| 0x800462bc | WIP, unclaimed | Event bytecode interpreter has a candidate 16-entry jump table at 0x80012890, an unresolved callback, and proven menu/map/frame calls. Table and callback ownership are not proved. |
| 0x8004678c | WIP, unclaimed | Event controller calls exact map-object, effect, audio, and frame helpers; the candidate jump table at 0x800128d0 and a later indirect callback remain unresolved. |
| 0x800475d8 | Focused WIP, 89.4% | Source models the variadic spawn ID, paired pose transitions, `PadRead`, CD services, and frame draws; one CFG block and instruction-order residues remain. |

The map-pattern source remains one contiguous two-function unit. A proposed
early row-pointer increment preserved semantics but gave a worse focused
listing without independent source evidence, so it was reverted. GAME target
relink verified 141/141 units. The global edge check still reports only the
three previously recorded unrelated `.rodata` addend differences.

## Ten TMD-render and player-magic functions

This ten-function GAME batch follows the TMD renderer and player magic/effect
call graph. Each address was checked against retail disassembly and CFG,
incoming and outgoing references, strings, adjacent function boundaries,
current match state, and the vendored census. The nine large or incompletely
owned bodies remain unclaimed rather than asserting uncertain data/table
ownership. Their WIP verdicts have no objdiff percentage.

| GAME VA | Verdict | Evidence and remaining work |
| --- | --- | --- |
| 0x80026498 | Exact, 100% | The 452-byte player magic dispatcher matches both code and its 13-word switch table. It uses the 26-byte magic record's `mp_cost`, the player MP and selected-record fields, a typed three-component direction, and direct calls to the player magic/weapon effect helpers. |
| 0x8002665c | WIP, unclaimed | The 0xbd0-byte caller selects magic/weapon actions and reaches exact 0x26498, animated vertices at 0x34344, and effect construction at 0x40308. Its broader player and effect state branches still need a complete source model. |
| 0x8002722c | WIP, unclaimed | The 0x2c0-byte magic-cost and state gate reads magic record +0x16 and player MP, but later indirect control through two candidate tables has no proved table owner. |
| 0x800274ec | WIP, unclaimed | The 0x43c-byte collision/angle loop directly calls 0x2b9d4, `rcos`, `rsin`, and `vector_xz_to_angle`; the surrounding collision-cache extent remains provisional. |
| 0x80028998 | WIP, unclaimed | The 0x528-byte player action controller calls 0x2722c, player weapon attack, and an event controller. Its state flags and event path span several owners. |
| 0x8002985c | WIP, unclaimed | This 0x9dc-byte fragmented routine has 113 outgoing candidate references and no reviewed direct-call set; function extent/control and data ownership must be resolved first. |
| 0x8002ddb4 | WIP, unclaimed | The 0x728-byte textured TMD renderer reaches `tmd_get_object`, `NormalClip`, `NormalColorDpq*`, and `AddPrim`; packet and object-state ownership remain incomplete. |
| 0x8002e4dc | WIP, unclaimed | The adjacent 0x704-byte textured TMD variant has the same proven GPU/GTE call family, but its distinct branches and packet fields need a source model. |
| 0x8002ebe0 | WIP, unclaimed | The 0x5b4-byte colored TMD variant reaches `NormalColorCol*` and `AddPrim`; its material and packet views are not yet complete. |
| 0x8002ff5c | WIP, unclaimed | The 0xcbc-byte map render helper has 13 direct `resource_copy_words` calls and feeds the existing map renderer; the copied data extents and state owner remain unresolved. |

The 0x26498 source claims only its proven 0x80011260..0x80011293 switch
table. The separate 13-byte effect-ID table at 0x800667e8 remains a
config-only, address-derived external because its storage owner is not proved.
After the exact source correction, the switch table's `.text` addend matches
retail. GAME target relink verified 143/143 units; the global known-reference
check still stops at three pre-existing TMD/map-object `.rodata` addend
differences and unresolved ownership, with no new divergence from this unit.

## Ten actor-effect and player-collision functions

This later GAME batch revisits ten still non-exact functions in the player
effect, actor movement, and collision graph after explicit source handoffs.
Retail disassembly and CFG, incoming/outgoing calls, candidate tables, current
source, and vendored attribution were checked for each address. Strict scores
for the four existing source claims come from the refreshed GAME objdiff
report; unclaimed functions have no percentage.

| GAME VA | Verdict | Evidence and remaining work |
| --- | --- | --- |
| 0x80025a18 | WIP, unclaimed | The 0x918-byte player effect dispatcher has 99 CFG blocks, direct calls to the exact 0x25878 helper and effect constructor, and an unresolved indirect jump through candidate `DAT_80011188`. The switch/data owner remains open. |
| 0x800279cc | WIP, unclaimed | This 0x5ac-byte main collision branch calls exact height/sound wrappers, damage response, and collision probes. The shared cache beginning at BSS +0x11800 still overlaps a provisional equipment extent. |
| 0x80027f78 | WIP, unclaimed | The 0x2ac-byte sibling branch calls the collision probe three times, then death, fixed X/Z length, and bounds helpers. It also depends on the unresolved cache extent. |
| 0x8003c614 | WIP, unclaimed | The 0xa70-byte actor/effect dispatcher has 45 CFG blocks and eleven calls to 0x3c3e0; an indirect jump through candidate `DAT_80011ee8` lacks a proved table owner. |
| 0x8003d184 | WIP, unclaimed | The 0x248c-byte actor behavior dispatcher has 410 CFG blocks, three unresolved indirect jumps, and a wide proven animation/collision/effect call graph. Its complete dispatch and data owners remain open. |
| 0x8003fa68 | WIP, 70.73333% | The typed cooldown gate and four collision modes match retail referents, but the probe compiler folds four retail calls into a shared arm: 14 retail versus 15 compiled CFG blocks. No source trick was retained. |
| 0x80040308 | WIP, unclaimed | The 0x13e4-byte effect constructor calls the exact pool initializers and sound helpers; its 116-block CFG includes an unresolved indirect jump through the candidate 0x8001249c table. |
| 0x8003c3e0 | WIP, 96.524826% | Typed eight-argument actor movement has 23/23 CFG blocks, eleven branches, and matching direct calls. Naming the target Y+1600 intermediate brought the retail load/operand order into the focused listing. The first remaining differences are frame/register assignment and the pitch argument's lifetime; the three prior functions remain exact. |
| 0x8003d084 | WIP, 91.60000% | Signed actor byte +0x4b clamps the note offset; 5/5 blocks and the `rand` call agree. The probe reassociates the final `-2` across the shift. An early-subtraction experiment changed the call delay slot and was reverted; adjacent 0x3d0e8 remains exact. |
| 0x800460a0 | WIP, 99.268295% | The actor animation seek has 6/6 blocks, two branches, and exact calls/referents; only step and half-step saved registers are exchanged. Width and expression-order probes worsened or retained the residue and were reverted. |

No candidate table or overlapping BSS symbol was defined for these WIP
dispatchers. The 0x3c3e0 source correction preserves the existing typed actor
and player layouts and its three strict-exact neighbors.

## Ten actor math, damage, and effect WIPs

This GAME batch revisits ten still non-exact source claims after actor and
player owners released their bodies. Each has a retail disassembly/CFG,
incoming/outgoing reference, string, source, and current-match dossier under
`/tmp/kf2-batch7-0x*.txt`; none is attributed to a supplied vendored archive.
The listed percentages are strict objdiff scores, not `--loose` scores.

| GAME VA | Verdict | Evidence and first unresolved difference |
| --- | --- | --- |
| 0x800158b4 | WIP, 98.00000% | The nine-halfword interpolation has exact loads, widths, arithmetic, and 3/3 CFG blocks. Pointer/index temporary assignment and the two independent pointer increments are ordered differently. An explicit-increment source experiment changed retail `lhu`/extension selection and was reverted; two adjacent math functions remain exact. |
| 0x80015918 | WIP, 95.49419% | The trajectory solver keeps 41/41 CFG blocks, 24 branches, `SquareRoot0`, signed divisions, and exact adjacent 0x15bc8/15ce0. Ordering its mode-zero branch like retail improved the focused listing from 76.3% to 85.2%; strict score moved from 95.75581% to 95.49419%. The first remaining mismatch is the discriminant result register, followed by midpoint/time register assignment. The independently evidenced branch structure was retained. |
| 0x8003a318 | WIP, 73.59162% | The 16-argument radial actor-damage caller has its unsigned attenuation correction and matching direct calls, but retail uses a 200-byte frame and 26 CFG blocks versus the probe's 192-byte frame and 25 blocks. Exact neighbor 0x3a778 remains untouched. |
| 0x8003a614 | WIP, 96.91011% | Actor-to-player damage gate has 6/6 blocks and matching distance, angle, and damage calls. Retail forms some camera fields from separate absolute loads; the probe reuses a saved player-state base and assigns scale temporaries to different saved registers. |
| 0x8003a9f4 | WIP, 89.04444% | Actor proximity scan has matching call/field set but 13 retail versus 12 compiled blocks; the alternate-position arm and stack-argument lifetime differ. Four following animation helpers stay exact. |
| 0x8003ab5c | WIP, 88.53488% | Sibling proximity scan omits the target-type exclusion as retail does, but has 12 retail versus 11 compiled blocks and the same alternate-position/register-lifetime residue. |
| 0x8003bd40 | WIP, 86.53226% | Actor horizontal steering has 9/9 blocks and five branches; first differences are independent actor-coordinate load/subtract order and angle/limit saved-register assignments. Six adjacent motion helpers remain exact. |
| 0x8003f7ec | WIP, 85.86207% | Group-target pointer fixup has 9/9 blocks and correct 40×16 offset walk. The two listing residues are sentinel constant setup order and `addu` operand order; reversing the C pointer addition did not change the object and was reverted. Adjacent scan/load functions remain exact. |
| 0x8003fb94 | WIP, 87.791046% | Fifteen-argument effect precursor has 11/11 CFG blocks, six branches, and the expected effect/actor calls; the first difference is prologue saves and argument/mask scheduling. Its nine contiguous update helpers remain exact. |
| 0x80041e94 | **Exact, 100% direct objdiff** | A fresh isolated pinned compile matches all 664 function bytes; the contiguous 0x4212c sibling also remains exact. The whole 1,028-byte `.text` and all 22 ordered relocations match the safe retail module. The earlier 92.650604% report was stale. |

No data or relocation owner was changed for this batch. The source changes
retained here are the 0x15918 mode-branch shape and the earlier 0x3c3e0
target-Y intermediate; experiments without retail support were reverted.

## Ten player, event, and menu-transition functions

This GAME batch follows player collision bounds through event control and the
menu transition. Each address has retail disassembly/CFG, xref, string,
adjacent-boundary, source-history, vendored, and current-match evidence under
`/tmp/kf2-newten-0x*.txt`. Percentages below are strict objdiff results.

| GAME VA | Verdict | Evidence and first unresolved difference |
| --- | --- | --- |
| 0x80023384 | WIP, 50.744186% | The collision-bound calculation and direct calls are identified, but the compiler reuses the player-state base and reassociates the height arithmetic. Retail independently loads three player fields and adds 1600 before subtraction. The BSS bounds overlap a provisional equipment extent, so no new owner was asserted. |
| 0x8002897c | WIP, 88.57143% | The signed inclusive 71–80 predicate has 3/3 CFG blocks. Retail assigns the upper-bound flag to `v1` and the result to `v0`; the probe exchanges them and adds a final move. Natural local and condition-order probes did not close the residue. |
| 0x80029624 | WIP, 62.44898% | Paired u16 phase counters and signed/division paths retain 20/20 CFG blocks. The return/sentinel join and stores are scheduled differently; the fifteen adjacent player-reaction claims remain exact. |
| 0x800349bc | WIP, 92.34296% | Four fade quads retain 14/14 CFG blocks and eight branches. Retail keeps `state` on a 72-byte frame; the probe uses a saved register and a 64-byte frame. An independently weaker local-name probe was reverted. |
| 0x80034e10 | **Exact, 100%** | The two scratch-buffer increments and typed primitive-buffer boundaries now emit the retail order. `tim_upload_images` in the same source remains exact. |
| 0x800461a0 | WIP, 99.12676% | The explicit `0xf1`/`0xfe` branches and shared fallback join restore 14/14 blocks and 5/5 branches. Only the two record-cursor register assignments remain exchanged. The adjacent marker helper remains exact. |
| 0x80047c98 | WIP, focused 85.1% | The 0x660-byte event controller has a provisional C claim and reviewed direct map/effect/menu calls; its final indirect call remains unresolved. |
| 0x800482f8 | **Exact, 100%** | A checked 0x0a-byte event-control sentinel view at +0x2c and post-clear pointer setup reproduce all three halfword stores. All six event-state functions remain exact. |
| 0x80048554 | **Exact, 100%** | The 0x458-byte serializer follows the retail target-group terminator, reads the low byte of the 16-bit object ID for save packets, and releases one loaded saved-block pointer. Direct objdiff confirms `.text` 1112/1112 and `.rodata` 660/660; the table's indirect edges remain candidate. |
| 0x800489ac | Focused WIP, 95.1% | Source owns the 0x378-byte decoder and bounded 16-word opcode table. Sentinel streams, packed map packets, and calls align; two initial loop register assignments remain different. |

The event sentinel has checked header and inventory rows. No candidate table
or overlapping BSS global was defined. GAME target relink verified 144/144
units; the global known-reference check still stops at the three established
unrelated TMD/map-object `.rodata` addends.

## Ten effect-dispatch, resource, and event functions

The largest GAME function, `effect_update_dispatch` at `0x80042650`, is the
primary target of this batch. Retail disassembly/CFG, all incoming and outgoing
xrefs, strings, adjacent boundaries, KF1 source, current source history,
vendored attribution, and the existing match were checked for each address.
The dispatcher and five large controllers have no source claim, so they have
no objdiff percentage. The four existing source scores are from the latest
strict GAME report, after the layout-identical effect phase-field refinement.

| GAME VA | Verdict | Evidence and first unresolved difference |
| --- | --- | --- |
| 0x80042650 | WIP, unclaimed | The 0x3670-byte effect updater has 446 navigator blocks, two indirect switches, 206 decoded direct calls, and a single proven caller in `effect_pool_sweep`. Its 123-entry kind and five-entry phase tables have decoded targets but only candidate pointer relocations. Several arms read a provisional collision-cache BSS region that overlaps an unsupported equipment extent. Ghidra also drops ten retail blocks and invents an argument to `rand()`. A complete truthful C body and data owner remain necessary before objdiff can define a source divergence. |
| 0x800320b0 | WIP, 73.95918% | The 24×24 map-cell mask OR has 10/10 CFG blocks, six branches, and matching graphics-state referents. Retail computes Z/row bounds then X, keeping the radius and mask in different registers; the probe reorders independent coordinate loads and retains different loop induction. A row-before-column source probe improved focused listing but not the retail setup order, so it was not retained. |
| 0x80032274 | WIP, 90.933334% | The VAB range updater has 13/13 blocks, seven branches, and the exact audio call. Retail recomputes the eight-byte slot index each iteration in a 48-byte frame; the probe retains an induction offset and extra saved register in 56 bytes. |
| 0x80035894 | WIP, unclaimed | The 0x7e4-byte map-placement controller has one proven incoming call and eleven direct outgoing calls. Its placement/occupancy path and candidate pattern table near 0x80067874 need a non-overlapping data owner before a source claim. |
| 0x80036190 | WIP, 89.78417% | The map-object interaction query has 15/15 blocks, eight branches, and matching GTE/distance/angle calls. The first divergence is the initial bound branch and return frontier; natural shared-return, single-loop-bound, and signed-halfword-index probes lowered focused similarity and were reverted. Two following map-object helpers remain exact. |
| 0x80036e24 | WIP, 98.86364% | Five-block frame/CD service loop has all five direct calls, two branches, and exact referents. Only the three saved assignments for mode, endpoint, and step are cyclically exchanged; no artificial local was introduced to force registers. |
| 0x8004678c | WIP, unclaimed | The 0xc54-byte scene/effect controller has all 96 direct call/jump words and supported event/player/callback-state address pairs reviewed. Its bounded 35-row command table and CD-loaded callback table still lack proved source ownership; the switch `jr` and final `jalr` remain indirect. |
| 0x800475d8 | Focused WIP, 89.4% | The 0x6c0-byte controller has one fixed map-object argument and a variadic spawn ID, a 120-byte frame, and source-backed pose-buffer reuse; one CFG block differs. |
| 0x80048d24 | WIP, unclaimed | The 0x5b8-byte serializer has no decoded direct calls; it writes a large runtime payload whose disjoint field extents lack a complete owner. |
| 0x800492dc | WIP, unclaimed | The paired 0x5e0-byte deserializer has three data references but no direct calls; its payload schema and exact source field widths remain open. |

The dispatcher evidence supports `KfEffectRecord.phase` at +0x07. Kind zero
uses record +0x40 as a collision latch, while kind six stores a parent index
there in eight spawned children, so that tail remains an opaque variant. The
phase name and checked structure row preserve the exact two-function
zero-direction-spawn unit. No candidate switch-table pointer or overlapping
BSS word was promoted to a source definition. The latest strict pass verifies
146/146 GAME target relinks; global closure retains only the three previously
reported unrelated `.rodata` addend mismatches.

## Ten map, model, resource, and effect render functions

This GAME batch follows the map-cell renderer through the frame's model and
resource callees and the adjacent effect collision probe. Retail disassembly,
CFG, incoming/outgoing references, adjacent claims, source, relocation/data
owners, vendored attribution, and the current strict report were inspected for
each function; navigator captures are under `/tmp/kf2-ten-0x800*.txt`. No
source or inventory change was retained from the focused experiments below.
The percentages are strict objdiff scores from the latest available GAME
report, with exact sibling listings checked by focused compilation.

| GAME VA | Verdict | Evidence and first unresolved difference |
| --- | --- | --- |
| 0x80030c18 | WIP, 96.521736% | The typed map-cell draw has 11/11 blocks, six branches, and matching GTE/TMD calls. Focused listing differs only in the first view-matrix address setup, orientation-byte load, and flags-register assignment order. Moving the orientation assignment across `SetRotMatrix` or introducing a view pointer worsened the schedule; both probes were discarded. Exact 0x30f5c/31024 siblings remain unchanged. |
| 0x80030de4 | WIP, 89.52128% | The two-layer map-cell traversal has 10/10 blocks and six branches. Retail loads the lower-layer elevation before joining the common Y/draw tail and schedules the X/Z arithmetic differently. A natural shared-tail rewrite retained the CFG but worsened the listing, so the typed original remains. |
| 0x800311b0 | WIP, 92.14815% | The typed `POLY_FT4` constructor has 8/8 blocks, five branches, and the retail packet writes and `AddPrim` call. Retail preserves one extra argument in `s2` and orders mode/texture stores differently; no source-supported argument or packet-layout change was found. |
| 0x80031850 | WIP, 84.71045% | World-model render has 40/40 blocks, 16 branches, matching direct calls and the corrected three-argument 0x2ddb4 call. Retail forms a persistent graphics-state base and orders map-cell coordinate arithmetic differently. Reversing the equivalent world-matrix branch arms in a temporary probe still left the first register and address-formation divergences; no edit was retained. |
| 0x80031d8c | WIP, 94.65414% | Animated-object render has 7/7 blocks, two branches, matching TMD calls and record referents. Retail retains the final render-data argument in `s7`; the probe reloads it from the caller stack, changing frame saves and entry scheduling. |
| 0x80032174 | WIP, 93.60000% | Map-cell visibility is a 0x64-byte leaf with 5/5 blocks, three branches, and two validated graphics-state reads; its sole proven caller is 0x3247c. The focused object assigns Z comparisons to `v0` rather than retail `v1` and adds a final result move. Natural early returns created an extra block, so they were reverted. |
| 0x800321d8 | WIP, 98.43590% | Resource TMD queue has matching 3/3 blocks, one branch, calls, and registry referents. Only the fixed arena boundary `0x8009b0a0` differs: retail uses carry-adjusted `lui/addiu` while the current literal emits `lui/ori`. Its source owner remains unresolved; no artificial address expression or overlapping global was introduced. Four exact resource siblings remain exact. |
| 0x8003247c | WIP, unclaimed | The 0xb70-byte map/resource controller has a proven caller at 0x335a0, 27 direct calls, and 50 outgoing references, including five 0x31850 calls and the exact range updaters. The 32-byte identity matrix at 0x80063dcc is typed and reviewed, but the full 96-block object/render state and data ownership are not yet established for a truthful C claim. |
| 0x80033d3c | WIP, 95.21839% | Sparse morph accumulation uses three `ScaleMatrix` calls, 17/17 blocks, ten branches, and a typed asset/pool view. Retail shares a far tail after skip flush while the probe exits by a nearer branch; two contiguous sparse helpers remain exact. |
| 0x8003fa68 | WIP, 70.73333% | Effect collision gate has the proven cooldown/type field reads and four case-specific 0x2b9d4 calls, each with its distinct mode. The compiler folds those calls into a shared arm, leaving 14 retail versus 15 compiled blocks. No call suppression or synthetic side effect was used to force separate arms. |

The render/resource functions are game-owned, not Psy-Q library bodies: their
validated addresses read project graphics, map, asset, or effect state and
their direct calls link the same GAME call graph. The unclaimed frame
controller and unresolved arena literal remain explicitly WIP. No repository
tests, banking, or commit were performed for this batch.

## Ten vector, selection, collision, TMD, and map-object functions

This GAME batch follows the main-loop collision calls into floor-item allocation,
TMD drawing, and map-object placement. Retail disassembly, CFG, incoming and
outgoing references, adjacent claims, source history, data/relocation owners,
vendored attribution, and strict status were inspected for all ten addresses;
navigator captures are under `/tmp/kf2-next-0x800*.txt`. Percentages are strict
objdiff results. The focused-exact vector and menu changes and the supported
TMD width correction were retained; other speculative probes were reverted.

| GAME VA | Verdict | Evidence and first unresolved difference |
| --- | --- | --- |
| 0x8001369c | WIP, unclaimed | The 0x2f0-byte main loop is called directly by `main` and has 48 outgoing references, including nine startup clears, five floor-item allocations, effect-pool sweeping, and frame rendering. Several cleared BSS extents still lack complete owners, so no startup-state source claim was made. |
| 0x800155a4 | **Exact, 100%** | The vector-distance leaf now shares retail's sentinel return and horizontal-distance path, preserving the lower-height early branch. Focused and strict comparisons are exact. |
| 0x80019834 | **Exact, 100%** | The menu's +0x30 pointer targets the stack's `s32 values[20]`, while separate byte indices select the result. Correcting that field preserves the adjacent exact 0x199d0 selector; both functions are strict exact. |
| 0x8002c670 | WIP, unclaimed | The 0x7bc-byte collision-mask dispatcher calls the exact scan/update helpers and reads seven apparent signed-halfword pairs beginning at 0x80067874. That address directly follows the collision-defaults datum, but its complete initialized extent and owner remain unproved; no overlapping table was defined. |
| 0x8002ce68 | WIP, 65.85185% | The seven-argument floor-item capture has the exact free-slot, allocation, `StoreImage`, and `DrawSync` calls and 5/5 CFG blocks. Retail saves s0–s4 in 40 bytes and loads late caller-stack arguments after allocation; the probe retains s0–s8 in 56 bytes. A narrow `u8 kind` probe worsened the listing and was reverted. The two exact sibling functions remain unchanged. |
| 0x8002ddb4 | WIP, 97.34716% | The TMD primitive renderer has matching packet/referent semantics, but retail has 44 blocks and 28 branches versus 42 and 26 in the probe; two depth/loop guards remain structurally different. Eight sibling TMD functions remain exact. |
| 0x8002e4dc | WIP, 96.587975% | The sibling renderer has the same 44/42 block and 28/26 branch gap, with distinct stack and register lifetimes around its packet loop. The shared TMD exact siblings remain intact. |
| 0x8002ebe0 | WIP, 95.27945% | Retail retains `blend_mode << 5` as a full word and stores it with `sw`; changing the C local from a prematurely truncated halfword to `u32` follows that evidence. The focused listing improves, but strict score falls slightly while one CFG block/branch gap remains. The truthful full-width source is retained. |
| 0x80036464 | WIP, 95.32258% | Map-object effect spawn has 12/12 CFG blocks, three branches, and matching field/call semantics. The first remaining differences assign object ID and height offset to opposite saved registers and schedule one store/call delay differently. Its unit also has an independent switch-table `.rodata` addend issue. |
| 0x800366fc | WIP, 37.336365% | Map-object scatter has 37 retail versus 38 probe blocks and 25 branches. The first substantial mismatch uses the object action byte through different base pointers, followed by case-arm joins; the unit's switch-table addend also differs. The exact 0x368b4 sibling remains untouched. |

The strict target after the TMD correction relinked 145/145 GAME units. The
global known-reference check still stops at the three previously reported
unrelated TMD/map-object `.rodata` addends. The required full `kf build` after
retained source changes built PSX and reached only the established unresolved
InitCARD/malloc/display_buffers links in GAME/OPEN/END; no module-cap failure
appeared. No repository tests, banking, or commit were performed.

## Ten menu-selection and player-damage functions

This GAME batch follows the main menu's item and magic selections into item
preview and player damage. Retail disassembly/CFG, both xref directions,
strings, adjacent boundaries, source history, data/relocation ownership,
vendored attribution, and the current strict report were checked for every
address. Navigator captures are under `/tmp/kf2-menu-damage-0x800*.txt`.
The two item controllers use the checked 52-byte `KfItemMenuList`; they keep
the quantity-byte array in `menu.values` and pass a separate item-index array
to the selection helper. The retail test reloads the stored byte-sized entry
count, which was decisive for both exact listings.

| GAME VA | Final verdict | Evidence and remaining difference |
| --- | --- | --- |
| 0x8001876c | WIP, 96.36646% | The seven-choice menu controller has 34/34 blocks and 14/14 branches. Retail saves the -1 and -99 sentinels in the opposite two saved registers and reloads the final result differently; its exact 0x189f0 sibling remains intact. |
| 0x80018ac8 | **Exact, 100%** | The 0x240-byte item-selection controller, including model load/release, special-item TIM preview, two-frame draw, and item-result path, is source-owned and strict exact. |
| 0x8001930c | WIP, unclaimed | The 0x528-byte TIM viewer has one proven caller, 17 decoded direct calls, archive allocation/read/upload, two FT4 packet builds, frame/pad loop, and resource release. Its packet-state lifetime and large local record still lack a complete C model. |
| 0x80019ac4 | WIP, 99.88971% | Equipment-list source and its 200 initialized label bytes match. CFG is 21/21 blocks and 11/11 branches; only the selected-choice load and two comparisons use `v1` in the probe versus retail `a0`. |
| 0x80019ed4 | WIP, unclaimed | The 0x420-byte equipment-row controller selects two glyph ranges through a nine-way table and dispatches equipped item slots through another indirect table. Both pointer tables remain candidate owners; no fabricated source table was defined. |
| 0x8001a2f4 | **Exact, 100%** | The magic-list controller and its eight initialized glyph-prefix bytes match strictly; the exact 0x19834/0x199d0 selection neighbors remain exact. |
| 0x8001a898 | **Exact, 100%** | The 0x204-byte equipment-item list uses the exact equipped-count selector, model preview, two-frame loop, and confirmed stock decrement. |
| 0x8002083c | WIP, 99.65882% | The typed matrix and TMD-preview calls match, but retail reserves 64 more stack bytes than the four evidenced `MATRIX` locals require. The adjacent 0x20748/0x20990 functions remain exact. |
| 0x80024498 | WIP, unclaimed | The 0x34c-byte radial-damage dispatcher has 53 CFG blocks and an eight-entry indirect table at 0x80011128, with all decoded targets inside this body. Its complete switch-table source owner and branch-arm data model remain open. |
| 0x800248a8 | WIP, unclaimed | The 0x3fc-byte damage-component dispatcher has 36 blocks, eight direct calls to exact `player_calculate_damage_component`, a caller-supported 12-argument ABI, and a seven-word internal table at 0x80011148. The table owner and full arm semantics remain WIP. |

The two new exact controllers required 34 directly decoded `mips26`
relocations and three reviewed HI16/LO16 references to `game_counter_bytes`.
Strict comparison verified 149/149 GAME target-unit relinks. The global
known-reference check still stops at the three established unrelated
TMD/map-object `.rodata` addends. After retained edits, full `kf build` built
PSX; GAME/OPEN/END retain their existing first unresolved InitCARD, malloc,
and display_buffers links. No repository tests, banking, or commit were run.

## Effect and event source pass after the merged master

The GAME effect and event ten were checked against retail disassembly/CFG,
incoming and outgoing xrefs, strings, current identities, relocation evidence,
source history, and the current match report. Fresh navigator captures are
under `/tmp/effect2026_0x800*.txt`. This pass used focused `kf try` only; the
strict report predates the retained source correction below.

| GAME VA | Current verdict | Decisive evidence or residue |
| --- | --- | --- |
| 0x8003fa68 | WIP, last strict 70.73333% | Four separate retail collision calls are folded into one shared call in the source probe. `if`/`else if` and reordered-case probes did not restore those call sites; both were discarded. |
| 0x8003fb94 | Focused `SAME` (strict refresh pending) | Updating `kind` in place after extracting option bits matches the retail prologue and mask schedule; all ten listings in `game.effect_update` are identical. |
| 0x80040308 | WIP, unclaimed | The 0x13e4-byte constructor has a bounded 123-word table with all entries inside its body, now curated as one datum. The indirect edges remain candidate and no source-owned RODATA or complete collision-cache model exists. |
| 0x80042650 | WIP, unclaimed | The 0x3670-byte dispatcher has two decoded indirect switches, but the in-body pointer-table relocations and collision-cache owner remain provisional. |
| 0x8004678c | WIP, unclaimed | Both direct callers pass the player camera position, a rotation view, and an integer command; the 0xc54-byte controller ends by forwarding those to active callback slot two. Its 35-way command switch and callback targets remain indirect. |
| 0x800475d8 | Focused WIP, 89.4% | One fixed `KfMapObject *` plus a variadic spawned ID reproduces the four argument homes and duplicate first-argument store. The two interpolation phases reuse position/angle buffers; one CFG block remains different. |
| 0x80047c98 | WIP, focused 85.1% | The 0x660-byte paired controller has a provisional C claim and unresolved indirect callback; CFG has 85 retail versus 83 compiled blocks. |
| 0x80048554 | **Exact, 100%** | The 3,072-byte payload, sentinel-terminated target-group scan, low-byte object IDs, 165-entry switch, and arena calls emit identical `.text` (1112/1112) and `.rodata` (660/660) under direct objdiff. In-body table edges remain candidate. |
| 0x800489ac | Focused WIP, 95.1% | A source claim now models the sentinel-delimited actor/group streams and 16-entry map opcode switch. The table's in-body pointer edges remain candidate. |
| 0x80048d24 | WIP, unclaimed | The 0x5b8-byte save walker is a direct-copy field sequence, but the large destination payload and live-state field family need a common owner. |

The in-place `kind` update is semantically equivalent to masking a separate
mode local; it leaves the option bits available to the later actor branch.
`kf try --unit game.effect_update` reports 10/10 identical listings, including
the nine previously exact neighboring claims. No broad match, repository
tests, full build, banking, or commit were run in this pass.

The constructor's kind bound at `0x800404a8..b0`, four-byte index, table base
`0x8001249c`, and indirect jump at `0x800404d0` establish exactly 123 entries
through `0x80012687`; all raw words target its body, and the following word is
zero before the separate dispatcher table. The event interpreter at
`0x800462bc` similarly bounds opcodes `0xf0..0xff`, indexes 16 words at
`0x80012890..cf`, and jumps to in-body handlers. Both table extents now have
one curated data identity each; the indirect control-flow relocations stay
candidate until source objects validate them. The paired save-state walkers at
`0x80048d24` and `0x800492dc` still need a shared payload schema before C
ownership can be claimed.

Three more event switch-table extents are fixed by retail bounds and pointer
loads. `0x8004678c` subtracts 82 from its command, bounds the result to
0..34, and indexes 35 words at `0x800128d0..0x8001295b` (21 distinct in-body
targets). `0x80048554` bounds a record-kind byte to 0..164 and indexes 165
words at `0x80012960..0x80012bf3` (five distinct in-body targets).
`0x800489ac` subtracts `0xf0` from an opcode, bounds it to 0..15, and indexes
16 words at `0x80012bf8..0x80012c37` (eight distinct in-body targets). The
zero words at `0x8001295c` and `0x80012bf4`, and the string following
`0x80012c38`, remain separate. The three ranges now each have one curated
pointer-table identity, while their in-body pointer relocations remain
candidate until source-owned RODATA validates them.

The event callbacks have a supported pointer owner even though their runtime
targets remain indirect. Retail `0x80046474..88` loads
`state_8017d118.active_table[4]` and calls it with the event object and the
stream byte. `0x80047374..88` calls slot 2 with three event operands, and
`0x800482b0..c4` calls slot 0 with the local position and event argument.
Those three `jalr` instructions do not prove which function each table slot
contains at runtime; their target edges remain unresolved. The existing
`KfState8017d118` layout checks establish the pointer field at +0x0c, so no
new callback global or guessed direct callee is needed.

The fixed resource arena address `0x8009b0a0` is formed by the same signed-low
`lui 0x800a; addiu -0x4f60` pair at exactly three raw GAME sites:
`game_main_loop` at `0x8001389c`, the unclaimed resource initializer at
`0x80015d64`, and `resource_tmd_queue_read` at `0x80032200`. The initializer
first reads an archive stream into that buffer and walks its length-prefixed
records. Startup later initializes a 0x5f000-byte block arena there; the TMD
queue allocates from it. That RAM range ends at `0x800fa0a0`, before the
separate display primitive memory at `0x800fba58` and within the 2 MiB main
RAM policy. A temporary extern-symbol probe emits the retail `lui/addiu`
shape with HI16/LO16 relocations, whereas the current integer literal emits
`lui/ori`. No original symbol, complete BSS extent, or defining TU has been
proved, so no global or arithmetic replacement was retained.

## Event target byte stream at 0x800462bc

The interpreter now has a separate, provisional C unit. Its argument is a
`KfActor *`: retail reads the actor's group index at +2, multiplies it by the
0x78-byte target-group stride, and loads the first target pointer at group
offset +0x38. A non-null candidate of type 0x70 supplies the
byte stream. The sixteen cases for opcodes 0xf0..0xff are bounded by the
retail unsigned subtraction and use the curated 0x80012890 table. The indirect
slot-4 callback remains indirect; the target candidate's still-opaque payload
bytes are accessed through its existing typed owner.

The latest focused `kf try` compares at 90.5% similarity, with 46/46 CFG
blocks, 21/21 branches, and matching return frontiers. Ordering the 0xf9
handler next to the shared 0xf0/0xf8 rewind handler and reusing the single
rewind operand follows the retail load sequence. Expressing the saved byte
offset as `cursor - candidate->bytes` removed the pointer-algebra residue and
states the object relationship directly. The first remaining body difference
is an action-handler load-delay scheduling instruction. This is **WIP, not
strict exact**.
Thirty-one direct `j`/`jal` words were individually checked
against retail instructions and admitted as reviewed relocations, as were
twelve HI16/LO16 references into established actor, event, callback, and
counter owners and the one RODATA-base pair. The sixteen in-body table pointer
words and runtime `jalr` target remain candidate. No repository test, full
build, broad match, banking, or commit was run for this unit.

## Event map-object controller at 0x800475d8

The two decoded direct calls from 0x80047c98 pass existing map-object records.
Retail 0x800475d8 uses its first argument as a `KfMapObject *`; when null, it
acquires one from the effect pool, resets it, and writes the second argument
to the object's +6 halfword. The third and fourth O32 arguments have no body
uses. The first C pass used a two-argument signature, but an exact variadic
control and a focused ABI probe support one fixed `KfMapObject *` plus a
variadic spawned ID. The acquire call reads its sequence from
`map_object_state.unknown_873e` at 0x8017d10e, and the later 24-byte template
walk starts at `map_object_state.templates` at 0x801749d0. Two camera-relative
reads at 0x801984fc and 0x801985c0 belong to `player_state` +0x2c and +0xf0.
These references identify existing owners but do not yet explain the full
pose/CD/pad branch family in the 0x6c0-byte body.
All 38 in-body direct `j`/`jal` words were checked against retail opcode and
target and promoted to reviewed control-flow relocations. The original six
signed-low HI16/LO16 pairs for map-object/player-state addresses and six more
raw-decoded player-rotation/game-counter pairs now point to their existing BSS
owners. A focused one-VA safe delink admitted every row without withheld
relocations. The separate C claim is a truthful 89.4% focused WIP with
55 retail versus 54 compiled CFG blocks, 29/29 branches, and 1/1 returns.
One-fixed-argument `va_arg` access reproduces all four pre-frame argument-home
stores and the repeated first-argument store. Restricting the reward flag to
the return phase yields the retail 120-byte frame; a signed 16-bit pitch and
swapping the source/destination roles of two pose-buffer pairs eliminate
larger call and copy divergences. The first remaining difference is the
template pointer's saved-register assignment (`s1` retail versus `s0`
compiled), followed by one return-path block caused by flag scheduling.
Writing the signed quarter-depth as direct C division now reproduces retail's
`lh`/`move`/`negu`/`bgez` sequence and removes that prior instruction-order
residue. No unsupported register steering was kept. The pose
offsets at template +0xc/+0xe use a shared layout-identical signed view;
existing map-object exact controls remain focused SAME.

The paired 0x80047c98 dispatcher has a direct call from 0x80029014 with
`&player_state.camera_position` and
`&player_state.camera_rotation_target`. Its entry copies the three position
words to a stack `VECTOR`, raises Y by 500, and repeatedly reads the rotation
view's signed +2 halfword. This supports
`void func_80047c98(const VECTOR *, const KfPlayerViewRotation *)` in the
function inventory. The tail calls active callback-table slot zero with the
temporary position and rotation pointer; the runtime callee remains indirect.
Retail 0x800482b0–0x800482c8 loads that slot through
`state_8017d118.active_table` at BSS +0x0c and passes the stack `VECTOR`
at sp+0x20 in a0 and the rotation pointer in a1. The resource-transition
driver assigns either the 32-row initialized no-op table at 0x80063e00 or
an outside-load address 0x8019e138 to the active pointer. That address is four
bytes after the startup-cleared `effect_state` extent ending at 0x8019e134,
and an earlier transition phase passes the same address as the destination of
`cd_archive_queue_read(5, 3 * state_8017d118.values_10[0] + 2, ...)`.
The callback storage is therefore populated from a CD archive entry, but
there is no curated object binding or complete extent for it.
Neither assignment resolves
the indirect callback to a particular callee.
Its 48 direct `j`/`jal` words were checked against raw retail opcode/target and
promoted to reviewed control-flow rows. Twelve raw signed-low pairs identify
the player, actor, map-object, event-counter, event-state, and callback-state
references. Focused
safe delinking of this one VA reported zero withheld relocations; the terminal
`jalr` target remains unresolved.

The separate 0x80047c98 C claim is focused 85.1% WIP. It copies only the
three retail-read position words, adds 500 to Y, uses one actor/map index,
and holds the map-object pool and template bases across the scan. Its frame
is now the retail 88 bytes, and the focused target admits all twelve reviewed
address pairs with zero withheld relocations. Kind 5 bypasses the angle gate
used by kinds 8 and 0x16, and successful kind-3 checks exit immediately;
these retail-backed corrections preserve 55/55 branches and two return
frontiers. Reordering the case bodies to match their retail instruction order,
reloading the kind-3 state byte where retail does, and preserving the linked
object's read/reset order raised the focused listing from 57.1% to 85.1%.
The current CFG has 85 retail versus 83 compiled blocks. The first difference
is rotation-pointer and sentinel saved-register assignment and a zero-index
move scheduled after the optional actor call rather than in the retail branch
delay slot. The remaining case-5 control shape is WIP. Retail
0x80047f58..0x80047f7c confirms kinds 0xa5 and 0xff
converge on the same guarded notification handler, so their combined C cases
remain semantically correct despite the differing decision-tree layout. The
CD-loaded active-table slot remains an indirect call, with no
invented callback owner. The kind-0x20 path treats the +0x40 record pointer as
an unresolved byte view and accesses only its retail-proven byte +1; it does
not assert that the pointed object is a full map-object record. Retail loads
that pointer after calling 0x800293d4, so the source preserves the same order.
The kinds 0x0d/0x14 use a named, layout-identical halfword view of tail bytes
+0x38/+0x39 for their retail `lhu`; existing byte paths and the twelve-byte
tail remain unchanged. Focused 0x47c98 is 85.1%; map-object reset
preserves five exact listings, while its separate cell-marker and init-record
functions retain their prior WIP verdicts.
Both calls to the WIP varargs 0x800475d8 callee pass a non-null map-object
pointer in `a0`: the kind-0x40 path sets it in the `jal` delay slot, and the
linked-object path sets it on its preceding branch delay slot. Neither call
sets an optional argument; that callee consumes `va_arg` only on its null-
object spawn path. The one-argument source calls preserve the retail ABI.

The earlier scene controller at 0x8004678c has direct callers at 0x80028a98
and 0x80029108. Both supply `&player_state.camera_position`,
`&player_state.camera_rotation_target`, and an integer command. Its tail
loads `state_8017d118.active_table[2]` and forwards those same three
arguments through `jalr` at 0x80047388. This supports the typed function
inventory signature; the loaded slot's concrete target remains unresolved.
Five raw HI16/LO16 pairs at 0x80046f00/10/20/30/40 choose consecutive
eight-byte initialized lists at 0x800679a0..0x800679c7. Raw words show
`0xff`-terminated byte IDs. The shared loop at 0x80046f58 multiplies each ID
by 26, indexes `effect_state.magic_records` at 0x8019b6a8, tests
`menu_available` at record +0, and sets it to one if clear. Its signed-low
0x80046f4c/0x80046f50 address pair now validates against the existing
`effect_state` BSS owner. The next distinct
candidate datum begins at 0x800679c8. This supports a bounded five-list
magic-unlock family. On the first newly enabled ID, the path calls
`map_object_effect_pool_acquire(0x15e, 10, ...)`, resets that object, stores
the current scene command as its object ID, and enters the shared pose/update
loop; already enabled IDs continue scanning until the `0xff` terminator.
The acquire call's third argument is the previously established
`map_object_state` +0x873e pool sequence; its 0x80046f9c/0x80046fa0
signed-low pair is reviewed without changing the BSS owner.
Raw words in the existing 35-entry command table at
0x800128d0 map command values 0x5a..0x5e to those five handler blocks; the
five lists partition IDs 0..19 exactly once, in four-ID groups
`[7,8,9,10]`, `[14,15,0,13]`, `[16,1,2,3]`, `[4,17,5,6]`, and
`[18,19,11,12]`; each has a `0xff` terminator. The
five eight-byte extents are now curated in `data.tsv` and
`data_identities.tsv`, with a nonoverlapping gap starting at 0x800679c8.
The five literal `lui`/`addiu` pairs are individually reviewed in
`relocs.tsv` and appear as validated address references after a safe
one-VA delink; the command jump and terminal callback remain indirect.
The table-pointer reachability tier and original data TU remain candidate, so
no initialized source datum is claimed yet.

The remaining raw table words bound commands 0x52..0x74. Distinct handlers
serve 0x52 and 0x54..0x59; 0x53, 0x5f..0x62, 0x69, and 0x6e point to the
common exit at 0x80047370. Commands 0x63..0x66, 0x68, and 0x6a..0x6d point
to 0x800467f8, while 0x67 points to 0x80046cb8. Commands 0x6f..0x74 point
to six short setup blocks at 0x800469fc/46a04/46a0c/468d4/468dc/468e4.
These 35 rows contain 21 distinct in-body target addresses, checked against
all retail words at GAME file offset 0x20d0. This is a raw pointer inventory,
not a promotion of the `jr`
successors.
The table-base `lui`/`addiu` at 0x800467dc/0x800467e0 is reviewed against
the bounded 0x800128d0 owner and passes a safe one-VA delink. Its 35 pointer
rows remain candidate indirect edges. Four decoded direct `j` instructions
at 0x80046f08/18/28/38 converge at 0x80046f4c and are separately reviewed;
the fifth selector falls through. They do not resolve the command-table `jr`.
The final callback through `active_table[2]` also has two distinct possible
table sources. GAME 0x80016820 stores the initialized 32-entry
`callback_default_table` at 0x80063e00 into `active_table` during its initial
phase; later it passes 0x8019e138 to `cd_archive_queue_read` and stores that
same CD destination into `active_table`. The latter address is four bytes
past the proven `effect_state` extent, but its allocation, full extent, and
callback contents are unproved. Neither phase identifies the concrete callee
of 0x8004678c's terminal `jalr`.
The same active pointer is indexed at least through slot 19: retail
0x80038fac loads a callback at pointer +0x4c before its own indirect call.
Thus any active table used on that path must provide at least 20 four-byte
entries (0x50 bytes). It is not yet proved that the CD-loaded table is active
on that path; neither its complete allocation size nor a slot-to-callee map
is established.
All 66 direct `jal` sites in this controller were checked against their raw
MIPS-26 words and named with curated function identities, except the SDK
`rsin` target at 0x8005dda4, which retains its vendored attribution. The 22
previously reviewed sites and 44 newly reviewed sites are control-flow rows.
All 30 direct `j` sites were also checked against their raw MIPS-26 words;
their targets remain inside the function. The four previously reviewed and 26
newly reviewed rows pass a safe GAME one-VA delink with zero withheld
relocations or functions. The switch-table `jr` and final `jalr` remain
indirect, with no promoted callback target.
Seven independently decoded `lui`/signed-low pairs at 0x800467bc,
46888/468a4/468bc, and 46988/469a4/469b8 address the existing
`event_state` BSS owner. Five use the state word at +0; two construct indexed
control-byte bases at +4 and +6. The safe one-VA delink admits all seven;
they do not imply a new overlapping global.
Six further raw pairs at 0x80046a10/46a24/46a40/46a80/46aa0/46ab4
identify the already modeled callback state (+4, +0, +4), event control
(+4, +6), and player state (+0x18) during the transition branch. They too
pass safe one-VA delinking without a new data owner.
Two tail stores at 0x80047318/1c and 0x80047354/58 use `lui at,0x801a`
with signed low halfwords 0x853c and 0x853e. They write 0x384 to existing
`player_state` signed halfwords +0x6c and +0x6e after separate event-counter
calls. The raw pairs pass safe one-VA delinking; the halfwords remain
address-derived status fields until their complete gameplay semantics are
proved.
A further adjacent-pair audit of this controller reviewed 23 omitted raw
HI16/LO16 pairs: thirteen resolve within `player_state`, six within
`event_state`, three within `state_8017d118`, and one at
0x80046ecc/46ed0 loads `audio_state.listener_position.vy` at +0x14. Each raw
opcode, shared register, and complete owner extent was checked; safe one-VA
delinking retains zero withheld rows. No new global or narrower object extent
was inferred.

The following save-offset pair, 0x80048554 and 0x800489ac, each uses its
first argument as a four-word stack-table index. The former receives a byte
from callback state +4 at the 0x47c98 call site and serializes actor/target
indices into an arena payload; the latter restores that state and is directly
called by 0x16820. Both inventories now carry a conservative
`void (s32 save_slot)` ABI. Their 165-word and 16-word switch tables are
bounded, and their raw table-base HI16/LO16 pairs are reviewed. The eighteen
and fourteen direct `j`/`jal` words were checked individually against retail
and promoted to reviewed control-flow rows. Focused safe delinking admits all
of these relocations with zero withheld rows. In-body table pointer targets
remain candidate for indirect reachability. Both functions have since gained
separate C source claims.

Both functions now have separate C and RODATA claims. The 0x800489ac decoder's
focused listing is 95.1%: its call set, four reviewed BSS address pairs,
sequential packet reads, and map-object update path align, leaving the two
actor/group sentinel-loop register assignments. Focused CFG has 23/23 blocks
and 7/7 branches; indirect switch reachability remains incomplete. A
temporary natural `while ((index = *stream++) != 0xff)` spelling for the first
sentinel loop lowered the focused listing to 88.3% and changed its branch
layout, so the retained source stays at 95.1%.
The 0x80048554 serializer's
focused listing now reports `SAME` after correcting three retail-backed facts:
target-group `0xff` ends the group scan, map save packets load the low byte of
the 16-bit object ID separately from the halfword template index, and the
saved block pointer is loaded once before release. Its 165-word retail table
has five distinct in-body destinations, including 16 tail-byte kinds and two
two-byte kinds. The 3,072-byte stack payload and actor, target-group,
map-object, and arena address pairs remain source-backed. Indirect switch
reachability is still candidate. Focused `kf try` reports `SAME`, and a direct
objdiff check confirms `.text` 1112/1112 plus `.rodata` 660/660 at 100%.
The five new BSS pairs and both function bodies passed focused one-VA safe
delinking with zero withheld relocations. The table pointers still have only
candidate indirect-edge status; the decoder remains WIP despite its high fuzzy
score, while the serializer's code and table bytes now match exactly.
