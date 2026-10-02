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
| 0x8003fb94 | Exact | Fresh isolated direct objdiff confirms 536/536 code bytes and matching ordered relocations inside the ten-function effect-update unit. |
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
| 0x80042298 | Exact | Direct strict objdiff and fresh focused comparison confirm the first function in the three-function scatter unit; the provisional 0x801c7068 `SVECTOR` owner is now defined in that source. |
| 0x80042424 | Exact | Direct strict objdiff and fresh focused comparison confirm the companion scatter function with the same provisional shared `SVECTOR` owner. |
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
| 0x80047c98 | WIP, focused 85.7% | The 0x660-byte interaction dispatcher now sends proven template kinds 3 and 4 to one handler; it calls actor probe, map-object selector and channel transition, while its callback remains indirect. |

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
| 0x8002ff5c | WIP, unclaimed | The 0xcbc-byte map render helper has 14 direct `resource_copy_words` calls and feeds the existing map renderer; the copied data extents and state owner remain unresolved. |

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
| 0x800158b4 | **Exact, 100% strict** | Separate source, target, and destination pointers with source-before-target evaluation emit the retail nine-halfword load and increment schedule. The later [trajectory campaign](kf2-game-player-actor-trajectory.md) records strict 100/100 closure; a fresh focused build keeps this and the two preceding leaves `SAME`. |
| 0x80015918 | WIP, 95.49419% | The trajectory solver keeps 41/41 CFG blocks, 24 branches, `SquareRoot0`, signed divisions, and exact adjacent 0x15bc8/15ce0. Ordering its mode-zero branch like retail improved the focused listing from 76.3% to 85.2%; strict score moved from 95.75581% to 95.49419%. The first remaining mismatch is the discriminant result register, followed by midpoint/time register assignment. An explicit in-place discriminant accumulation changed earlier multiplication/save scheduling and reduced the focused listing to 67.2%, so it was reverted. The independently evidenced branch structure was retained. |
| 0x8003a318 | WIP, 99.86911% strict | The 16-argument radial actor-damage caller now has the supported unsigned attenuation, matching calls, and 26/26 CFG blocks. The recorded strict report leaves two incoming stack-argument temporary registers exchanged; the [near-exact audit](kf2-game-root-near-exact-verdicts.md) found no supported signature, call, CFG, or referent correction. Exact neighbor 0x3a778 remains untouched. |
| 0x8003a614 | WIP, 96.91011% | Actor-to-player damage gate has 6/6 blocks and matching distance, angle, and damage calls. Retail forms some camera fields from separate absolute loads; the probe reuses a saved player-state base and assigns scale temporaries to different saved registers. |
| 0x8003a9f4 | Exact, 100% | Branch-local distance queries now reproduce the shared retail call setup; 360/360 strict text bytes match, as detailed in `game-actor-collision-ten.md`. The older WIP rows above are historical. |
| 0x8003ab5c | Exact, 100% | The companion scan omits the target-type exclusion and matches 344/344 strict text bytes; see `game-actor-collision-ten.md`. The older WIP rows above are historical. |
| 0x8003bd40 | WIP, 86.53226% | Actor horizontal steering has 9/9 blocks and five branches; first differences are independent actor-coordinate load/subtract order and angle/limit saved-register assignments. Six adjacent motion helpers remain exact. |
| 0x8003f7ec | WIP, 85.86207% | Group-target pointer fixup has 9/9 blocks and correct 40×16 offset walk. The two listing residues are sentinel constant setup order and `addu` operand order; reversing the C pointer addition did not change the object and was reverted. Adjacent scan/load functions remain exact. |
| 0x8003fb94 | Exact, 100% | Updating `kind` in place after extracting option bits matches the retail prologue and mask schedule; direct objdiff confirms 536/536 text bytes and matching ordered relocations. The older WIP row above is historical. |
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
| 0x80036464 | WIP, 95.32258% | Map-object effect spawn has 12/12 CFG blocks, three branches, and matching field/call semantics. The first remaining differences assign object ID and height offset to opposite saved registers and schedule one store/call delay differently. Its 17 switch-table R_MIPS_32 rows have identical case grouping but a uniform +4 addend shift caused by the preceding 0x80036190 body compiling 560 bytes versus retail's 556; this is an upstream size residue, not an independent table-owner issue. |
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
| 0x8003fb94 | Exact, 100% direct objdiff | Updating `kind` in place after extracting option bits matches the retail prologue and mask schedule. All ten functions in `game.effect_update` are 100%; the whole unit is 1908/1908 `.text` bytes with 36/36 ordered relocations identical. |
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

The constructor at 0x80040308 now has 133 raw-reviewed direct-control rows:
69 `jal` calls and 64 in-body `j` words. Its bounded 123-entry switch table
at 0x8001249c is constructed by the reviewed `lui/addiu` pair at
0x800404bc/0x800404c0. A safe one-VA carve admits 135 relocations with two
withheld candidate pairs and no withheld functions. The two remaining pairs
load and store DAT_8006d704, an initialized-zero index used modulo four to
select 576-byte-spaced destinations from 0x801d9628. Only this constructor
references the datum in the current xref inventory, and the destination has
no admitted binding. Both source owners remain WIP; the switch `jr` remains
indirect. A read-only raw constructor scan through GAME code finds one
`lui/addiu` pair for 0x801d9628, at 0x80040a50/0x80040a54. This confirms
the constructor is the only direct base reference found in that code span;
it does not establish the buffer's allocation or complete extent.
The constructor advances its ring index with `& 3`, scales each destination
by 576 bytes, and copies 24 rows of 24 bytes each. The updater indexes the
stored slot pointer by a modulo-24 frame and writes 24-byte row fields. This
bounds the accessed region to a candidate 4 × 24 × 24 = 0x900 bytes through
0x801d9f28. `display_frame_cleared_word` ends at 0x801d9614, leaving an
unclaimed 0x14-byte gap before the effect buffer base. Neither that gap nor
the buffer's original defining TU is proved, so no overlapping BSS datum or
source definition has been added for it.

A direct branch audit of 0x80047c98 found that template collision kinds 3
and 4 enter the same state handler in retail. The C source had omitted kind
4; sharing the case label preserves that behavior and raises its focused
listing from 85.1% to 85.7%. The compiled CFG remains 83 versus 85 retail
blocks, with 55/55 branches. Its first remaining divergence is the
object-index zero assignment in the angle-test branch delay slot; the nested
linked-object case also differs in branch layout. A source-only nested
conditional probe for that latter case produced the same listing and was
discarded. No callback target was inferred from the terminal `jalr`.

The raw 0x8004678c command table was re-read directly from GAME.EXE at file
offset 0x20d0. Command bytes 0x52..0x74 map to 35 in-body labels. The
0x63..0x6e band mostly shares 0x800467f8, except 0x67 enters 0x80046cb8
and 0x69/0x6e enter the common exit at 0x80047370. Commands 0x6f..0x74
enter six adjacent labels at 0x800469fc, 0x80046a04, 0x80046a0c,
0x800468d4, 0x800468dc, and 0x800468e4. These are retail pointer values,
not promoted indirect CFG edges; the 0x800467f0 `jr` and later callback
`jalr` remain unresolved.

The 0x8003fa68 effect collision probe remains focused 57.5% WIP. Retail has
four separate direct 0x8002b9d4 call sites, one per effect type 1..4; the
current C keeps those four case-specific calls but the probe compiler merges
their common argument setup and call. A temporary direct-return spelling
still merged them and lowered the focused listing to 53.6%, so it was
discarded. The original call separation is source/compiler attribution still
to resolve; no artificial side effects were added.

A fresh raw audit locates the four proven calls at `0x8003faf8`,
`0x8003fb1c`, `0x8003fb40`, and `0x8003fb64`. Quick off-tree compiles of
unchanged source with GCC 2.5.7 `-O2` and GCC 2.6.0 `-O1` still emit only one
`jal` to `func_8002b9d4`. Neither simple compiler-version nor optimization
level change closes the 14-retail/15-candidate-block CFG difference. The
function remains WIP; no source edit is supported by these probes.

For masked effect type 0, retail reaches the common return with `$v0=1`;
types 5..7 reach it with `$v0=4`. Those values are leftover switch-comparison
immediates, not explicit return assignments. No proved caller reaches those
types in this probe: the known type-0 constructor paths for kinds 101 and
0x66, and type-0x10 paths for kinds 15 and 17, do not call it. A temporary C
trial assigning those values explicitly introduced a new jump table/RODATA
and lowered focused similarity from 57.5% to 44.2%; it was discarded.

Current 29-function effect/event checkpoint verdicts (strict means direct
objdiff or previously reviewed strict report, with current focused controls
where noted):

| GAME address | Verdict |
| --- | --- |
| 0x8003fa68 | WIP, focused 57.5%; four retail collision calls merge in compiled source. |
| 0x8003fb94 | Exact, direct 536/536 bytes. |
| 0x8003fdac | Exact, direct effect-update unit. |
| 0x8003fdd0 | Exact, direct effect-update unit. |
| 0x8003feb0 | Exact, direct effect-update unit. |
| 0x8003ff18 | Exact, direct effect-update unit. |
| 0x800400c0 | Exact, direct effect-update unit. |
| 0x800401b4 | Exact, direct effect-update unit. |
| 0x80040220 | Exact, direct effect-update unit. |
| 0x80040264 | Exact, direct effect-update unit. |
| 0x800402a4 | Exact, direct effect-update unit. |
| 0x80040308 | WIP, source unclaimed; two constructor data pairs withheld. |
| 0x80041e94 | Exact, direct 664/664 bytes. |
| 0x8004212c | Exact, direct 364/364 bytes. |
| 0x800462bc | WIP, focused 90.5%; action load/branch ordering. |
| 0x8004678c | WIP, source unclaimed; command-table `jr` and callback `jalr` indirect. |
| 0x800473e0 | Exact, reviewed strict event-counter unit. |
| 0x80047434 | Exact, reviewed strict event-counter unit. |
| 0x800474c4 | Exact, reviewed strict event-counter unit. |
| 0x800475d8 | WIP, focused 89.4%; one CFG block and register-order residue. |
| 0x80047c98 | WIP, focused 85.7%; 85/83 CFG blocks after kind-4 correction. |
| 0x800482f8 | Exact, reviewed strict event-state unit; current focused SAME. |
| 0x800483a8 | Exact, reviewed strict event-state unit; current focused SAME. |
| 0x800483d8 | Exact, reviewed strict event-state unit; current focused SAME. |
| 0x80048428 | Exact, reviewed strict event-state unit; current focused SAME. |
| 0x80048498 | Exact, reviewed strict event-state unit; current focused SAME. |
| 0x800484e4 | Exact, reviewed strict event-state unit; current focused SAME. |
| 0x80048554 | Exact, direct 1112/1112 text and 660/660 rodata; current focused SAME. |
| 0x800489ac | WIP, focused 95.1%; actor/group sentinel-loop register assignments. |

The source-backed first pass of the GAME 0x80040308 constructor now handles
additional switch arms while remaining WIP. Kinds 2 and 23 each store three
unsigned halfwords at effect-record +0x40, +0x42, and +0x44 from their
trailing O32 arguments. Other kinds access +0x40 and +0x41 as separate bytes,
so these writes are retained through the provisional `unknown_3c` tail view.
The complete variant-specific field family and record-tail semantics are not
yet proved; the three halfwords do not justify a shared named field layout.
The table entry for kind 12 points to 0x80040d20. That arm copies the caller's
SVECTOR, sets all three scales to 30000, writes four `u16` tail values at
+0x40/+0x42/+0x44/+0x46, and takes the update count from the next trailing
argument. This source-backed arm compiles, but the constructor remains focused
9.2% WIP; the lower score was retained because the added reads and writes are
directly decoded. A fresh comparison of all 123 raw table words against C
case labels shows 55 entries route to the shared free-slot sentinel. Of the
68 remaining kinds, 66 now have source arms. Kind 6 still depends on the
unowned DAT_8006d704/0x801d9628 copy buffer, and kind 102 depends on the
unowned 0x8009a5a8 timer. The low aggregate score cannot be attributed to
missing case coverage alone; arm ordering, source structure, and referents
still require first-divergence review.
The adjacent actor-group source calls this kind with an angle pointer, four
halfword-sized values, and an update-count value in that same order; that
caller is itself WIP and is supporting context rather than ABI proof alone.

Draft PR #3 five-function continuation, GAME focused verdicts (none are
strict exact):

| Address | Verdict | Remaining evidence gap |
| --- | --- | --- |
| 0x8003c614 | 22.7% WIP | Branch-local script argument reads now use retail halfword/word widths; frame and local lifetime still diverge. |
| 0x8003d184 | 3.6% WIP | Target-state 1 continuation now has its active-group probe, timer refresh, and animation call; the large indirect switch remains incomplete. |
| 0x80040308 | 11.4% WIP | Only kinds 6 and 102 lack source arms; their buffers are unowned, and the broader source/code shape still diverges. |
| 0x80042650 | 8.0% focused WIP | Additional decoded motion, collision, scale, healing, and spawn arms are source-claimed; most of the 123-entry switch and two indirect dispatches remain unresolved. |
| 0x8004678c | 16.4% WIP | Command 0x55 now has the direct actor/map-object search path; other commands and the terminal callback value chain remain unresolved. |

For 0x8003c614, retail's argument-slot cursor starts at sp+200 (the saved
position mode) and advances to sp+212 only for modes -1/-2. The mode -1 arm
reads the three following caller slots with `lhu`; mode -2 reads full words.
The default position mode calls 0x8003c000 without eagerly loading a
variadic value. Kind 0x17 and the actor-spawn kinds load their pointer from
the next caller slot only when reached, while kind 0x7b reads the following
word at cursor+8. Moving the C reads into these branches corrects their
meaning and compiles at 22.7% focused WIP, versus the earlier 23.0% probe.
Reading each mode -1 value as `u16` from its 32-bit O32 slot restores the
three retail `lhu` opcodes at the corresponding absolute caller slots. The
focused score stays 22.7% WIP because the compiler still uses a 184-byte
frame versus retail's 192-byte frame and retains different local lifetimes;
neither residue justifies a synthetic local.

In 0x8003d184, target-state 1 at 0x8003d694 first sets phase 0xf1 and
selects the target byte when the actor's phase flag is clear. Otherwise it
calls the direct 0x8003bcd0 helper with the signed actor halfword at +0x64,
target halfwords +0x0c/+0x0e, active-group bytes +3/+4, and constant 5.
A nonzero result, or a zero result followed by `(rand() >> 5)` below the
target's +0x10 byte, refreshes the signed +0x64 halfword from
`rand() >> 3`. The arm then calls `actor_advance_animation_wrapped` with
target +0x08. These raw-backed additions compile and retain 3.6% focused
WIP; most other target states remain unmodeled.

Event command 0x55 enters 0x80047204. It selects side 1 or 2 from the
player halfword at +0x128, then calls 0x8003a778 with the event position,
rotation halfwords +2/+0, bounds `(8000, 500, 500)`, a distance output, and
-1. A returned actor whose byte +3 matches the side loads archive slot 6
and entry `actor+1+240`. Otherwise retail scans all 396 map objects in
0x44-byte steps, requiring object ID 0xe2 and extra byte +0x40 equal to the
side. Its direct 0x80036ad8 probe uses position X/Z shifted by 11, tail
bytes +0x38/+0x39, and 0x8000; a nonzero result loads slot 6 and entry
`tail halfword +0x3a + 510`. Either path sets `event_state.state_word=1`.
These typed, raw-backed calls raise focused 0x8004678c similarity from
14.8% to 16.4% WIP. Focused CFG grows from 52 to 64 compiled blocks against
108 retail blocks, and from 26 to 33 compiled branches against 51 retail
branches. The final `active_table[2]` call remains indirect.

Commands 0x5a..0x5e load five separate address-derived eight-byte lists at
0x800679a0, a8, b0, b8, and c0. Their raw bytes are respectively
`07 08 09 0a ff 00 00 00`, `0e 0f 00 0d ff 00 00 00`,
`10 01 02 03 ff 00 00 00`, `04 11 05 06 ff 00 00 00`, and
`12 13 0b 0c ff ff ff ff`. The shared loop stops at 0xff and indexes the
26-byte magic-record stride. The distinct final padding and absent source
definition keep all five DATA owners unresolved; no source arm or overlapping
global was claimed from these lists.

Command 0x59 in the GAME 0x8004678c switch points to 0x80046e14. It scans the
396 `map_object_state.objects` records at their proved 0x44-byte stride,
accepts object IDs 82/83 and 90..96, and uses exact `func_80015698` to keep
the closest nonnegative distance below 999999. If a record is found, retail
copies its X/Z position, weights its Y by four times the difference from
`audio_state.listener_position.vy`, and calls `audio_play_spatial_range`
with sound 0x8009, volume 0x6e, reach 25000, attenuation 29000, and note 0.
The typed C arm raises focused similarity to 14.8% WIP. The overall CFG still
has 108 retail versus 52 compiled blocks and 51 versus 26 branches because
other command arms remain unsourced. The terminal active-table callback stays
indirect.

The constructor's kind-114 table entry points to 0x80041458. It stores the
original Y word at record +0x44, probes two randomized X/Z coordinates with
the existing 0x8002b7f8 collision helper (radius and height 10), restores
original X/Z on a nonzero collision result, then applies the decoded fixed
position, rotation, and scale values. This word use overlaps the kind-12
halfword view of +0x44/+0x46 and confirms that the tail needs variant-specific
typing. The new arm compiles; focused similarity was 9.0% WIP, retained for
the raw-backed behavior despite a lower interim score. Spelling the initial
direction-present branch first then matches retail's `beqz`-to-zero path and
raises the focused result to 9.1%; the first remaining divergence is still the
72-byte retail versus 48-byte compiled frame and saved-register assignment.
Retail keeps a single trailing-O32-argument pointer live from the constructor
entry (`s1 = sp+88`) across switch arms. Replacing per-arm `va_start` calls
with one function-wide `va_list`, balanced on both returns, is also the
natural source model for this one call's variadic arguments. A temporary
source-only probe raised focused similarity from 9.1% to 10.1%; the retained
source reproduces that result. The compiled frame grows to 56 bytes and now
saves six `s` registers, versus retail's 72 bytes and six `s` registers; its
first remaining register assignment still differs.
The word store is `lw v1,24(s0)` at 0x80041474 followed by `sw v1,68(s0)`
at 0x80041484. Offset 68 is `unknown_3c[8]` in the 72-byte shared record;
the store ends exactly at its boundary. A shared union field-path change would
touch existing exact consumers of `unknown_3c`, so this remains a localized
variant view until a complete tail family is recovered.

The constructor entry's decoded stores initialize direction Z/Y/X,
scale Z/Y/X, and rotation Z/Y/X in that order. Retail clears the record's
halfword +0x12 before the magic-type cooldown decision, then clears +0x10
during the direction-length calculation; the earlier source had cleared
+0x10 twice and omitted +0x12. Its type gate also reloads the stored record
byte. The corrected C uses those fields and a single cooldown assignment.
The 123-entry kind table sends 55 kinds to the free-slot sentinel, so the
source now represents that with a switch `default`; kinds 6 and 102 remain
explicit WIP arms because their source-owned buffers are unproved. Focused
similarity rises from 10.1% to 11.1% while preserving the provisional
indirect table edge. Retail branches to distinct stores of 1 or 0 at record
+0x0d after comparing the signed squared direction length against 810001;
spelling those two outcomes explicitly raises the retained focused result
to 11.4% WIP.

GAME update kind 16 enters 0x80045680 in the bounded kind table. When the
effect's signed update count is four, it adds 60 to
`player_state.vitals.current_hp` and caps the result at `maximum_hp`. Every
visit passes `rsin(updates_remaining << 8)` as arg5 to the existing
`func_8002bf38(0xe6, 0xc8, 0xa0, 0x59d8, value)` helper. These widths,
constants, and direct call are decoded in retail. The focused source raises
0x80042650 from 1.3% to 2.6% WIP; the rest of the large dispatcher remains
unresolved.

Kind 22 enters 0x800458e0. Retail uses `lhu` at record rotation Y +0x36,
adds 10, then calls `func_80042298(100, 0x80000000, -300)`; a nonzero result
is sent to `func_8003feb0` before marking the effect slot free. The matching
source uses an unsigned halfword read and raises focused similarity to 2.8%
WIP. The table's indirect dispatch remains indirect.

Kind 109 enters 0x80045b6c. Retail reads byte +0x40 of the current record,
multiplies it by the proved 72-byte effect-record stride, and adds the
`effect_state.records[0].position` base at 0x8019bd3c. It passes that typed
target position to `func_80041b14` with the seven decoded arguments
`(500, 15, -1, 0, 0, -1)`. This arm compiles and raises the focused update
dispatcher to 3.3% WIP without creating a separate interior global.

Kind 20 enters 0x80043cb8 and calls `func_80041cd0(0x4000, 0x100, 0x20,
0x400, 0x8000)` through the shared 0x80044f30 tail, then adds 64 to the
halfword at record +0x26 (`rotation.vz`). The raw table pointer, direct call,
stack fifth argument, and halfword store support this C arm. Focused
comparison remains 3.3% WIP because most of the 0x3670-byte body is still
unreconstructed; no exact control regressed.

Kinds 1 and 28 enter 0x80043374 and 0x8004336c, selecting radii 500 and
250 before a shared arm. The byte at record +0x40 is its phase: phase 2
increments signed halfword +0x10 by 256 and frees the slot at 4096; phase 1
first adds 13 to direction Y. The arm adds 200 to rotation X, probes
`func_80042298(radius, radius * 2, 250)`, and on a nonzero result reports
the collision. Phase 1 then advances to phase 2 with the decoded +0x09,
+0x0c, and +0x10 values; other nonzero collisions call `func_80042424`,
enter phase 1, and reset direction Z/X/Y to 0/0/-100. The continuing path
calls `func_80041e0c` on the record position with `(0x4000, 0x4000, 500)`.
These direct edges and stores compile at 3.2% focused WIP versus the prior
3.3%; the small aggregate decline does not override the retail-backed arm.

Kind 14 enters 0x80045700. At update count 12 it clears the signed player
halfword at +0x54. It builds a stack VECTOR from two `rand` calls (X is
`rand() >> 5` minus 512, Y is `rand() >> 8` plus 200, Z is 0x400) and a zero
SVECTOR, then calls the proved constructor with type 0, kind 101 and the five
O32 payload words `(700, -30, 10, 14, -10)`. Retail immediately writes bytes
3 and 14 at the returned record's +0x0a and +0x08, then calls
`func_8002bf38(160, 180, 220, 18000, rsin(updates_remaining << 7))`.
The bounded arm compiles and raises the focused dispatcher verdict from 3.2%
to 5.1% WIP. Kind 19 enters 0x800457c4. At count 12 it adds 150 to the
player's current HP, caps at maximum HP, and calls
`player_cap_status_components(7)`; it then spawns three kind-101 records
with the same random position and zero direction pattern, but render ID 18,
and calls `func_8002bf38(240, 240, 160, 18000,
rsin(updates_remaining << 7))`. The count loop, five constructor payload
words, returned-record byte writes, and direct call are decoded in retail.
The focused dispatcher result rises to 6.9% WIP. The remaining kinds and
both indirect switches are still open.

Kind 116 enters 0x80042f20. Retail forms a midpoint by adding signed
half-distance from each direction halfword to the current position, then
advances the record position by the full signed direction. It calls the
directly identified `func_8002b7f8` at the midpoint and, only if clear, at
the advanced endpoint with radius 5 and height 10. A nonzero result from
either probe frees the slot, after which retail reloads the possibly changed
record type and calls the constructor for kind 0x2d with the advanced
position and payload 0x1a4. The new C emits the observed `lhu; sll 16; sra
17` midpoint pattern and both direct calls in the focused listing. The
whole-function fuzzy result moves from 6.9% to 6.6% WIP because switch
layout and most other arms remain incomplete; the decoded source is retained.

Kinds 11 and 54 share entry 0x80043af8. They pass the signed halfword at
record +0x40 to `func_80041cd0(0x3800, value, 0x80, 0x400, 0x8000)`, then
build a random SVECTOR in retail X/Z/Y store order from three `rand` calls.
The arm constructs kind 101 at the current position with payload
`(0xc00, -128, 15, 18, 10)` and adds 64 to rotation Z at the shared tail
0x80044f38. Focused compilation raises the full dispatcher to 7.6% WIP;
its switch tables and omitted arms remain the first large divergences.

Kinds 26 and 27 share entry 0x80043460. Retail writes effect type 0x21,
adds 100 to rotation Z, and calls `func_8003ff18` with the current position,
zero start, signed scale X, 0x8000, 0x400, and 0x1000. It then writes type
0x24 and branches on the signed byte at record +0x40: zero probes
`func_80042298(100, 200, 0)` and clears all three direction halfwords on
collision; one subtracts 512 from scale Z and frees the slot when the signed
result is nonpositive. Other byte values leave it active. This direct arm
compiles at the same 7.6% aggregate focused WIP score; the instruction
pattern and call order are retained from retail evidence.

Kinds 25, 34, and 35 enter a shared collision arm at 0x80042b98 or
0x80042b90. Kind 25 probes `func_80042298(250, 100, -30)`; 34/35 use the
same call with a zero third argument. On nonzero collision, low four result
bits free the slot, while a zero +0x40 phase byte is set to one and reports
the collision once through `func_8003feb0`. No collision clears that byte.
Every path calls `func_80041e0c(&position, 0x2000, 0x2000, 500)` at the
shared tail 0x80043450. This direct arm compiles; the aggregate focused
result moves from 7.6% to 7.4% WIP as the still-incomplete switch layout
changes.

Kinds 118 and 119 enter 0x80043bd4 and 0x80043bdc, selecting constructor
kind 0x33 or 0x34 before a shared collision check. Retail probes
`func_80042298(140, 0, -200)` and, on a nonzero result, calls the kind
selector with type `record->type | 3`, current position and a null direction,
then frees the original record. Their source compiles at 7.4% aggregate WIP;
the indirect outer dispatch is still provisional.

Kinds 51 and 52 enter 0x80043b98 and 0x80043bb0, respectively. Both call
`func_80041cd0` with multiplier 0x1000, argument four 0x800 and final
0x8000; kind 51 passes limit/increment 0x400/0x80, while kind 52 passes
0x800/0x100. Kind 40 enters 0x800430d4 and probes
`func_80042298(100, 200, 0)`. A collision reports its result and frees the
slot; otherwise it calls `func_80041e0c(&position, 0x2000, 0x2000, 500)`.
All three arms have decoded table entries and direct call targets. The
focused dispatcher comparison remains WIP at 7.8%; the retail 224-byte
frame and much of its 123-entry switch are still absent from this source.

Kind 42 enters 0x80042bfc. Retail reads the signed phase byte at record
+0x40; zero calls `func_80041cd0(0x3800, 0x1f8, 0x46, 0x800, 0x8000)`,
while a nonzero byte decrements in place. The direct table entry and byte
width are decoded. Adding that branch lowers the aggregate focused score
from 7.8% to 7.4% WIP as the incomplete outer switch changes layout; the
source-backed behavior is retained.

Kind 45 enters 0x80042f08 and calls that same scale helper with
`(0x4000, (s16)record_tail_40, 0x46, 0x800, 0x8000)`. Retail uses `lh`
for the +0x40 argument; the tail remains a provisional variant-specific
view. The focused dispatcher still compiles at 7.4% WIP.

Kind 115 enters 0x80042c30. Its direct aim call uses eight decoded O32
arguments `(0x258, 0x28, 0x24, 0xb4, 0x168, 0x1000, 0x104, 0x800)`.
When that call returns -1 or the signed update count is below 2, retail
calls the effect-position helper, spawns three kind-0x2a effects at the
current position with null direction and trailing values 0, 1, and 2,
plays spatial sound 0x18, and frees the original slot. The other path
calls `func_80041e0c(&position, 0x2000, 0x2000, 500)`. These are decoded
direct edges, with no indirect target inferred. The new source compiles
at 8.2% focused WIP; most outer-switch arms remain absent.
Kind 113 enters 0x80042d14. Retail probes `func_80042298(180, 360, 0)`;
a nonzero result or signed update count below 2 takes three kind-0x2a
constructor calls with trailing values 0/1/2, plays spatial sound 0x17,
then frees the original slot. Otherwise it calls
`func_80041e0c(&position, 0x2000, 0x2000, 500)`. It does not call the
position helper used by kind 115. This decoded arm compiles at 8.1% focused
WIP, a small aggregate decline from 8.2% while the outer switch is
incomplete.

Kind 117 enters 0x80043008 and advances all three position components by
their signed direction halfwords. Retail probes actors at that position
with Y raised by 5000, radius 100, and height 10000 through
`func_8003a9f4`. Its nonnegative result indexes the 0x7c-byte actor pool;
the base `0x8016b62c` is `actor_state.actors[0].position`, so the
constructor receives that actor position with trailing 0x4ec. Both paths
then call `func_80041e0c` on the elevated local position with
`(0x2000, 0x7fff, 10000)`. No new global or indirect target is inferred.
Focused compilation is 8.0% WIP after this arm; the complete actor-pool
referent is source-backed despite aggregate score movement.

Kind 106 enters 0x80044748. With phase zero, retail adds 20 to the
direction Y halfword and calls `func_80042298(140, 0x80000000, -300)`.
A nonzero result subtracts all three signed direction components from the
position, calls `func_800424f0` with `(1, 0, -400, 60)`,
`(6, 60, -330, 56)`, and `(8, 140, -170, 40)` in that order, plays
spatial sound 0x25, and sets the tail byte at +0x40 to 15, phase to 1,
and byte +0x08 to zero. The zero-result path calls the nine-argument
`func_80041e94(record, -1, -3, 6000, -800, 6, 8, 0, -1024)`.
In phase one, a zero tail byte frees the slot; other phases return.
This complete decoded arm raises the focused aggregate listing from
8.0% to 8.1% WIP; it does not prove the outer indirect switch's
source ownership.

Kind 120 enters 0x80045bb0. Retail increases direction Y by 20 and, while
signed scale X is below 0x1000, increases scale Z by 0x200 and copies that
halfword to X and Y. A first `rand() < 400` proceeds directly to the
completion path; otherwise it probes `func_80042298(180, 0, -300)` and
only collision bits `& 5` proceed, after copying the provisional collision
cache result word at 0x801d8d50 to position Y. The completion path runs a
second `rand() < 8192`; on success it spawns two kind-0x2a effects with
trailing operands zero and one and plays spatial sound 0x18. Both completion
outcomes free the slot, while the nonmatching collision path leaves it
active. Every path adds 2700 to rotation Z. Source uses the existing
`KF_COLLISION_CACHE_RESULT` interior view; the complete BSS owner remains
provisional. Focused aggregate listing is 8.0% WIP after this arm, a small
score decline despite the directly decoded branch and call structure.

Kind 50 enters 0x80043264 and switches on the signed tail byte at +0x40.
Phase zero adds 64 to scale Z and copies it to Y/X, calls
`func_80026330(0, &position)`, and once signed scale X reaches 256 sets
phase one and calls `func_80025878(1000, NULL, &direction, &distance)`.
Phase one probes `func_80042298(512, 0x80000200, 0)`; a nonzero result
sets phase two and calls `func_8003ff18` with the decoded six arguments.
Phase two frees the slot when signed scale X reaches 512, then still
increments and copies the three scales. Other phase values return.
The focused aggregate listing is 7.7% WIP after this complete decoded
arm; the score decline does not contradict its direct call and branch
evidence while most of the dispatcher remains absent.

Kinds 29, 30, 31, 47, and 48 share the phase-zero path beginning at
0x800426b0/0x800426b8. Kinds 30/47 use trajectory factor 5; the others
use 10. A zero halfword at record +0x42 first plays spatial sound 5.
Retail increments that halfword as signed age, projects X/Z from the
direction vector, and projects Y from the signed +0x40 origin plus
direction-Y times age and `(factor * age * age) >> 1`. It probes the
projected point with `func_8003fa68(..., 20, 20)` and, only on a zero
result, probes the midpoint with the previous position. It writes only
the three position words before `func_80041e0c`. A nonzero collision
reports `collision | 0x20000` and frees the slot. Otherwise retail sets
byte +0x0a from the provisional collision-cache layer and calls
`func_800154fc` with direction X, actual Y displacement, direction Z,
and the record rotation. Nonzero phases return. These variant-specific
tail halfwords are kept as a provisional view of the shared record, not
a global layout assertion. The focused aggregate listing remains 7.7%
WIP after all five directly decoded entries.

Kinds 38 and 39 enter 0x80043130/0x8004310c. Kind 39 with a signed update
count below 45 uses the eight-argument `func_8004195c` probe; otherwise
it adds 10 to direction Y and uses `func_80042298(100, 0, 0)`. Kind 38
uses that collision probe without the Y increment. A triggering result
reads the provisional cache flags at 0x801d8d60: bit 0x10 emits one
collision report on the first transition of tail byte +0x40 to one,
while its absence clears that byte. Retail then calls the 14-argument
`func_80041e94` twelve times, reloads the cache flags, and frees the
slot if their low nibble is nonzero. Every path finishes with
`func_80041e0c(&position, 0x2000, 0x2000, 500)`. The second flag read
is kept separate because those calls may change the cache. Focused
aggregate listing rises from 7.7% to 7.9% WIP after these two paths;
the cache's complete BSS layout remains provisional.

Kind 107 enters 0x800454a8 and uses the unsigned tail byte at +0x40 to
index the proved 72-byte effect-record pool. When current phase is zero and
the selected record's phase is at least three, it sets phase one and stores
three times current tail byte +0x41 as the update count. Selected phase four
frees the slot. Otherwise selected tail byte +0x44 minus three times current
tail byte +0x41, adjusted upward by 24 when negative, indexes 24-byte
snapshots through the selected record's +0x40 pointer. Retail copies the
snapshot's 16-byte VECTOR and eight-byte SVECTOR into current position and
rotation. A temporary tail-pointer view keeps the source honest while the
pointed allocation and complete variant layout remain unproved. Focused
aggregate similarity is 7.7% WIP; no global owner was asserted.

Kinds 33 and 53 enter the shared path at 0x800445e8/0x800445e0 with motion
scales 3800 and 7600. Retail copies three camera-position words to a local
target, subtracts 1600 from Y, calls `func_80041b14` with seven arguments,
then calls `func_80041e0c` on the effect position. A `-1` result reports
the provisional collision-cache flags, emits twelve identical 14-argument
`func_80041e94` calls, and frees the effect slot. Both result paths finish
with a nine-argument `func_80041e94`; its fifth argument is the signed
quotient of the negated motion scale divided by 48. The source uses only
the three camera-position components retail loads and leaves the cache's
complete owner provisional. Focused comparison compiles at 8.0% similarity;
the whole dispatcher remains WIP, with an earlier prologue and incomplete
switch-table divergence.

Kind 24 enters 0x80045a58. It copies the three camera-position words into a
local target with Y reduced by 1600, then calls `func_80041b14` with
`(300, 40, 2000, 0, 10, 0)`. A `-1` result reports the provisional cache
flags, emits twelve 14-argument `func_80041e94` calls with the same fixed
range operands and kind 14, then frees the slot. Otherwise a random value
below 16384 creates kind 0x6d at the record position, passing the rotation
pointer and `effect_state.current_index` as distinct O32 stack arguments.
This raw-backed path compiles; focused aggregate similarity is 8.2% WIP.

Kind 114 enters 0x8004593c and advances the three position words by the
signed direction halfwords. It tests the new position through
`func_8002b9d4(x, y, z, 10, 10, 176)`. A nonzero result spawns kind 3 and
frees the current slot. The kind-114 constructor directly writes its original
position Y to the +0x44 tail word; `func_8002b604` probes with that saved Y.
The subsequent cache-result comparison either
clamps position Y and takes the same spawn/free path, or emits two
`func_80041e94` calls with a random angular operand and leaves the slot
active. The complete +0x44 tail family and collision-cache owner are still
provisional. This source-backed arm compiles at 8.0% aggregate focused
similarity, down from 8.2%; the lower whole-function score does not disprove
its decoded calls, widths, or control paths.

Kind 46 enters 0x80042dc8. It advances position X/Z by signed direction
halfwords and uses signed tail bytes +0x40/+0x41 as phase and selected
effect-record index. In phase zero, retail calls `func_8002b9d4` with the
current position, radius 10, signed scale Y, and mode 0x30; it reports that
result, probes floor height at the selected record's Y, then sets position Y
from the provisional cache result. The scale-Y halfword becomes the signed
difference from the provisional cache height limit, capped at 32767. If the
selected record is free, it enters phase one, clears tail age +0x42, and
saves the scale in the record's +0x32 halfword. Phase one interpolates that
saved scale toward zero with `func_8001584c`, advances age by 512, and frees
the slot at signed age 4096. The source keeps byte-array casts for the
variant-specific tail until a complete shared layout is supported. Focused
aggregate similarity is 7.9% WIP; the earlier whole-function score fell by
0.1 point while the decoded control and call paths were retained.

Kind 111 enters 0x80043508. The signed halfword at effect tail +0x40 is an
actor index except for sentinel 0xff. The sentinel takes the current effect
position, zeroes the first three direction halfwords, and uses spread 1000;
otherwise retail indexes the 124-byte actor pool and copies actor position,
the eight-byte motion view at actor +0x50, and unsigned halfword spread at
actor +0x1c. Two `rand` calls perturb X/Z by `(rand * spread >> 14) - spread`,
Y drops by 2000, and `func_80040308` creates kind zero with the local
position and direction. This first-pass C compiles at 7.8% aggregate
similarity, still WIP because the dispatcher has incomplete arms and an
earlier prologue/table mismatch; no new actor field ownership was asserted.

Kind 5 enters 0x80043f44. Phase zero chooses a child count from
`asset_vertex_count(actor->unknown_01 + 128, actor->unknown_0c)` using the
unsigned actor index at effect tail +0x41; sentinel 0xff or a zero vertex
count instead yields 16 children, zero progress step, and one remaining
update. Counts above 32 clamp to 32 and derive a fractional step from the
original vertex count. The constructor loop emits kind 105, passing the
effect's direction, `effect_state.current_index`, actor index, and accumulated
fraction as distinct O32 operands; it then enters phase one. Phase one
waits while updates remain and tail halfword +0x42 is nonzero, then scans
all 128 effect records for active kind-105 children whose tail +0x40 byte
matches `current_index`. It sets phase-one children's update count to one
and counts them, or advances other matching children to phase two. A
nonzero parent child-count byte invokes `effect_magic_power` and passes the
typed current-magic halfwords +6..+0x14, scaled child count, effect flags,
and indexed actor position to `func_80039c94`; the parent is then freed.
Both the pool scan and magic-record owner are established by existing
complete objects. This source-backed first pass compiles at 7.0% focused
aggregate similarity, down from 7.7% while the overall switch remains
incomplete; it is WIP, not a match.

Kind 23 enters 0x80042934. Phase nine uses the signed tail halfword +0x40
as an actor index, requires actor lifecycle 1 and target type 25, and obtains
a vertex offset at the signed tail +0x42 index through `func_8003c000`.
It adds that offset to the actor's resolved position, grows all three scales
by 512 up to 0x1800, derives direction halfwords from the old and new
position words, and invokes `func_80041e94` with the decoded mode and
motion operands. Once actor animation phase reaches the unsigned tail
halfword +0x44, it calls `func_8003c3e0` with the existing player-state
+0xe8 origin view, resets phase to zero, and sets 50 remaining updates.
Other phases first invoke `func_80041e94` with mode 0x100 and then enter
the same collision/growth path as kinds 7/49. No new player or actor owner
was asserted. This complete raw-backed path compiles at 8.4% focused
aggregate similarity, up from 7.0%, and remains WIP.

Kind 105 enters 0x800441cc. It advances rotation Z by 800. Phase two
decreases the X/Y scales by 128, increases direction Y by 5, and uses
`func_80042298(100, 0, 0)` to free the slot when the low collision-result
nibble is set. Earlier phases use tail byte +0x41 as an actor index, with
0xff taking that collision path. For a valid actor, the signed tail
halfword +0x42 selects `func_8003c000`'s vertex offset; the source combines
that with `func_8003c10c`'s actor position. Phase zero probes it through
`func_80041b14(300, 50, 500, 150, 10, 0)`. A -2 result enters phase one
and decrements actor halfword +0x42 if nonzero; a -1 result with the
provisional collision-cache FLAGS low nibble set also decrements that
halfword and frees the child. Otherwise it waits. Phase one copies the
three meaningful position words and, when fewer than two updates remain,
enters phase two, chooses a random lifetime, and halves the vertex-offset
components into direction. Retail additionally copies the fourth local
VECTOR word, but this path has no defining store for that word; its source
value remains unresolved and the C leaves the destination pad unchanged.
The focused aggregate comparison compiles at 8.2% WIP, down from 8.4%;
the raw calls, branch conditions, and typed pool references are retained.

Kind 12 enters 0x80043cd0 and branches on phase. Phase 101 increments to
102. Phase 100 constructs another kind-12 record, sets its phase to 101,
then joins phase 102 in resetting the parent's render ID, scales, type bits,
and phase to 110. Phase 110 invokes `func_80041cd0` with the decoded
0x3800/0x31f/0x46/0x400/0x8000 arguments and advances rotation Y by 64.
Other phases play sound 0x29 on their first update, increment phase, and
call `func_8004195c` with the four signed tail halfwords +0x40..+0x46,
zero, 6000, that last halfword again, and 0x800. A -1 result plays sound
0x18 and takes the same child/reset path; otherwise the record moves through
`func_80041e0c`, increases rotation Z by 128, and calls `func_80041e94`
with the decoded mode operands. The direct child call supplies a null fixed
direction parameter and `&record->rotation` as its first variadic angle
pointer; constructor reads of later variadic slots have no visible writer
at this call site, so their values remain unresolved. This first-pass arm
compiles at 8.4% aggregate focused similarity, up from 8.2%, and is WIP.

Kind 9 enters 0x8004441c. Tail byte +0x40 selects a target: 0xfe builds
a local point from `player_state.camera_position` with Y lowered by 1600;
0xff increases direction Y by 10 and uses `func_80042298(10, 0x80000000,
0)`; other values index the actor pool, subtract half of actor halfword
+0x1e from actor position Y, and call `func_80041b14` with the decoded
600/50/0/0/10/0x80000000 arguments. The player point uses
400/60/3000/0/10/0x80000000. A nonzero collision result for sentinel
0xff, or a -1 motion result for the other targets, reports the provisional
collision-cache FLAGS word through `func_8003feb0`, emits twelve identical
`func_80041e94` spread calls, emits one `func_8004212c` scatter, and frees
the slot. Otherwise it calls `func_80041e94` with the decoded -1/-3 mode
and -80/6/8/0/0x400 operands. All branch outcomes and direct calls have a
source-backed path; the local VECTOR pad is unused by the target helper in
the visible call contract. The aggregate focused comparison compiles at
8.7% WIP, up from 8.4%.

Kind six now has its complete phase-zero path in C. Retail's five-entry phase
table selects 0x80044f8c at phase zero; that path loops eight times,
constructing kind-107 children with the parent's position, direction, and
rotation. It stores the current effect index at each child +0x40 and the
loop ordinal at +0x41, uses render ID 0x17 for child zero and 0x18 for
child seven, then sets the parent phase to one. The first variadic pointer
is the rotation, matching the current constructor case-107 reader. The
phase-one signed update-count test is also in C: a count below three sets
phase three, count -1, and tail +0x45 to 24. Phase three increments the
+0x44 frame modulo 24; while the +0x45 byte is nonzero
it decrements that byte, otherwise it clears bit 0x800 on the actor indexed
by +0x46 and frees the effect. Phase four calls `func_80041e94` 32 times
with the observed argument slots, then performs the same actor-bit clear and
free. The remaining phase-one branch now makes the decoded eight-operand
`func_8004195c(250,25,32,200,0,0x400,100,0x800)` probe. A -1 result
sets the signed update count to -1. When cache flags are 0x10, it calls
`func_8003fdd0`, reads the low byte of the cache actor index, and for actor
target type two or three selects phase two, render ID 22, and a yaw toward
that actor. Other results select phase three with the 24-tick tail timer.
The phase-two body runs immediately after a successful actor handoff in
retail. Its proved prefix now also runs in C: set actor flag 0x800, advance
the 24-frame counter, copy the position returned by `func_8003c10c`,
subtract half the actor +0x1e halfword, and select phase four once the
tail +0x45 count reaches 60. The nonnegative probe result advances the
24-row index and writes position X/Z/Y and rotation X/Y/Z through the
record's +0x40 trail pointer. The 24-byte row shape is proved by the
constructor's full VECTOR/SVECTOR copy. Phase two now also writes the
same row family, using the actor +0x1c width for its radial scale and
angle increment while advancing the +0x45 byte;
the table `jr` and pointer-word relocations remain candidate-tier. Focused
similarity is 10.0% in the current aggregate; this is source-backed WIP
with no exact claim.

Kind eight's apparent 0x801abd28 referent was a signed-low arithmetic
mistake: `lui 0x801a; addiu -17112` resolves to 0x8019bd28, exactly
`effect_state.records` after its 0x680-byte magic-record prefix. Its
72-byte index stride and constructor kind-eight +0x40 parent-index write
confirm the linked-record view; no new global is needed. Phase zero is now
in C: it integrates direction into a local position, makes the two
`func_8003fa68` collision probes, transitions to a parent-linked orbit when
`func_80041e0c` returns zero, or decrements the parent's tail counter and
frees the child. The ordinary movement path copies XYZ back, sets the
cache-layer-dependent byte, advances the rotation, and calls `func_80041e94`.
Phase one is also in C: it builds a forward X/Z pair from direction +0x34,
positions the effect relative to the linked record, interpolates direction
+0x38, advances scale +0x30, and queries the collision helper after the
signed threshold at +0x32. Its zero/nonzero collision result may call
`func_8003fdd0`; reaching scale 4096 frees the child and decrements the
linked record's +0x40 counter. A random branch emits the 14-argument
`func_80041e94` motion. Phase-zero-only focused similarity was 13.5%,
and the complete phase-zero/one arm reaches 15.2% WIP. The source follows
the corrected retail referent and call paths without claiming exactness.

Kind 10 enters 0x80044c7c. Phase one calls `func_80041cd0` with
0x4000/0x800/75/0x400/0x8000 and advances rotation Y by 64; phase five
resets the render ID, scale, update count, and type bits before taking that
path. Phase zero checks `func_80042298(250, 0x80000000, 0)` and remaining
updates. A collision or fewer than two updates emits a scattered kind-10
child with null direction and the current rotation as its first variadic
angle pointer, sets the child's phase to two, plays sound 0x17, and resets
the parent. Otherwise rotation Z advances by 128 modulo 4096. An active
tail +0x41 countdown derives an origin through `func_800401b4`, selects
the actor at tail +0x40, computes an 800-unit direction to that actor's
height-adjusted position, emits kind seven, and decrements the countdown.
With no countdown, a 1/16 random branch seeks an actor within 25000 via
`func_8003a778` and stores its pool index/countdown. A separate 1/32
branch seeks an actor within 30000 and, if found, emits two
`func_80041d7c` modes. Other phases increment phase. The caller-side
constructor argument count is limited to the raw stores; later variadic
reads in the WIP constructor remain unresolved. This source-backed arm
compiles at 13.6% aggregate focused similarity, up from 8.7%, and is WIP.

Kinds 103 and 121 share the 0x80043700 handler. Their phase-two path now
subtracts 128 from the unsigned scale halfword at +0x2c, mirrors the result
to +0x2e, and frees the effect when the signed result is nonpositive.
Their phase-zero path calls `func_80042298(50,0x80000000,-300)`, handles a
nonzero low collision nibble by clearing X/Z motion, decrements the signed
tail +0x40 countdown, and emits four kind-101 children with three independently
randomized direction halfwords. Retail stores all three halfwords of that
child direction before each constructor call. Phase one now retains its
collision call, the nonzero low-nibble `func_80042424`/`func_8002b604`
pair, signed direction smoothing, 70-unit vertical decrement, and the
four-child branch when the lesser of two provisional collision-cache words
minus the record Y is below 7000. A nonzero low collision nibble or a
height difference of at least 7000 instead builds a stack VECTOR with
the record X/Z and selected cache Y, sets phase two, constructs kind
104/122 and then kind two with the decoded type, distance and scale
arguments, plays sound 0x17, and still emits the four kind-101 children.
Retail has no visible write to the separate stack SVECTOR passed to both
constructors on this entry; C leaves its contents unspecified. Other phases
remain partial. Focused aggregate similarity is 9.8% WIP after this arm;
the lower score retains directly decoded branches, widths and calls.

Kinds 104 and 122 share 0x800439a8. Their phases three and later now have
the raw-backed path in C: animation clip receives `(phase & 1) - 128`,
phases below nine compute the distance from the effect to the player camera,
and distances below 32001 drive `func_8002bf38` with the observed sine
attenuation and RGB/distance arguments. Each such phase advances afterward.
Phases below three now take the directly decoded pair of `SquareRoot12`
calls on the scale-Y cubic expression, shift the result by three, and
construct kind 11 for kind 104 or kind 54 for kind 122 before joining that
same animation/distance path. Retail passes `sp+88` as the constructor's
direction vector with no visible write on this entry; the C keeps a local
SVECTOR without inventing its contents. This is a remaining value-origin
uncertainty rather than a missing call. The aggregate focused
similarity after this arm was 9.9% WIP.

Kind 100 now has the decoded 0x80043e2c control path in C. For phases 4–70
it calls `func_8004195c` with the observed eight operands; a nonnegative
result advances the effect position, rotation Z and motion. Other phases
below 100 raise direction Y by ten, call `func_80042298(100,0,0)`, and on a
zero result advance position. The failure/phase-100 path constructs kind 20,
plays sound 0x18, frees the record and increments phase. Retail passes
`sp+88` as the constructor's direction pointer on that path but has no
visible write to the local on this function entry; C retains a stack
SVECTOR without inventing its value. The constructor reads its three
halfwords, so the origin of those values remains a real WIP uncertainty.
Focused aggregate similarity is 15.1%, compared with 15.2% before this arm.

The raw 123-word kind table has 55 entries that jump straight to the common
return and 68 active entries; the current C names all 68 active kinds,
with kinds 6, 103/121, and 104/122 limited to proven phases. Kind 100's
constructor direction has an unproved stack value. The dispatcher remains
WIP at 9.8% aggregate focused similarity. A comparison with flow enabled
previously reported CFG comparison unavailable while retail direct J/JAL
rows were still candidate-tier and the compiled kind switch had an unresolved
indirect jump. A raw audit now confirms all 334 in-body direct J/JAL sites:
their decoded opcodes and encoded targets match the curated rows. The 302
remaining candidates were promoted to reviewed after the earlier 32. A
safe one-VA carve admits all 334 unique direct `R_MIPS_26` rows; its two
standalone/module objects report 672 relocations in aggregate. The sole
withheld row is the table-base HI/LO candidate at 0x80044f70/74. Both
table-base pairs retain candidate ownership,
and indirect dispatch targets remain unresolved. No refreshed CFG or strict
source verdict follows from the direct-edge promotion alone.

Kind 6 phase two now follows the raw 0x80045254–0x800452bc scale branch:
for tail byte +0x45 below 17, two `func_8001584c` calls interpolate from
zero to actor halfword +0x1e with fractions `counter << 9` and
`counter * 350`, storing effect scale X/Z and Y respectively. At 60 or
more, phase becomes four. While the counter is below 60, the same phase
then advances the angle in record rotation padding by `100000 / actor
width`, writes a sinusoidal XYZ position around the actor and a rotation
into the row indexed by tail +0x44, and increments tail +0x45. Phase one's
nonnegative-probe path also writes that proven VECTOR/SVECTOR row. These
accesses use the saved +0x40 pointer; they do not define the four-bank
buffer at 0x801d9628 or its candidate index global. Focused dispatcher
similarity after the kind-six pass was 10.0% WIP; the lower score retained
directly decoded field widths, row writes, and calls. Shared `game.effect_reset` and
`game.effect_scatter` focused controls remain 3/3 listing-SAME each.
KF1's exact ground-branch visual source also spells X/Z offsets as
`rsin`/`rcos(angle)` times a radial distance shifted by 12, which is a
useful source-shape comparison for the KF2 row calculation. The KF2 raw
0x80045338–0x80045388 instructions independently establish that expression;
KF1 has no corresponding four-bank trail allocation here.

Kind zero's collision branch now uses the existing
`KF_COLLISION_CACHE_RESULT` owner: raw `lw` at 0x800436d0 resolves to
`bss_801c7540+0x11810` (0x801d8d50). When collision bits `& 5` are set,
retail frees a phase-one record; otherwise it selects phase one, sets
direction Y to -200, and copies that cache result to position Y. This
replaces an obsolete owner-unknown comment without adding an overlapping
BSS definition. The focused aggregate after that bounded edit was
10.0% WIP; the current dispatcher verdict is 9.8%.

A fresh focused constructor comparison keeps GAME 0x80040308 at 11.4% WIP.
Its first divergence is the prologue: retail reserves 72 stack bytes and
sets the variadic cursor at sp+88 after calling `effect_pool_find_free`,
while the current source reserves 56 bytes and sets its cursor at sp+76
before that call. The following optional 16-byte position copy uses the
same load/store widths and order, albeit different saved registers. The
missing kind-6 and kind-102 storage owners prevent treating the frame
residue as an attributable compiler problem; no frame-padding source was
added.
KF1's strict-exact constructor is a source-shape control: its five named
arguments and `s32 *` variadic cursor put the cursor assignment in the
pool-find call delay slot, as KF2 retail does with a frame 16 bytes larger.
Moving the KF2 `va_start` after `effect_pool_find_free` alone lowered its
focused score from 11.4% to 10.5% and changed the null branch; the trial
was reverted. Neither the cursor origin nor the larger frame establishes a
different ABI or an original source ordering by itself.

The complete `effect_state` object at 0x8019b6a8 now has its single source
definition in `effect_reset.c` (`KfEffectState`, 0x2a8c bytes). The object
extent is bounded by the startup 0xaa3-word clear and the 128-record,
72-byte pool scan after its 0x680-byte magic-record prefix. Focused
`game.effect_reset` comparison keeps `effect_pool_reset`,
`magic_load_records`, and `effect_pool_sweep` 3/3 listing-SAME. This
source-owner claim removes the unresolved `effect_state` native-link name
(GAME diagnostics 644/14 distinct names to 556/13 in the shared link audit),
but the probe emits a 0x2a90 COMMON allocation for the retail 0x2a8c BSS
extent. The four-byte allocation residue is unresolved; no field or padding
was changed to mask it, and strict data/link closure is not claimed.

## Focused effect-graph continuation

This disjoint 25-function pass recompiled each listed unit after the shared
effect-record and BSS refinements. `SAME` here is a focused listing verdict;
it does not replace a direct strict objdiff check. The exact controls retain
their earlier strict evidence where recorded above. No source correction was
retained in the collision probe or constructor during this pass.

| GAME VA | Current focused verdict |
| --- | --- |
| 0x8003fa2c | SAME; strict certification not repeated here |
| 0x8003fa68 | WIP, 57.5%; four retail collision calls compile as one |
| 0x8003fb94 | SAME; prior strict exact |
| 0x8003fdac | SAME; prior strict exact |
| 0x8003fdd0 | SAME; prior strict exact |
| 0x8003feb0 | SAME; prior strict exact |
| 0x8003ff18 | SAME; prior strict exact |
| 0x800400c0 | SAME; prior strict exact |
| 0x800401b4 | SAME; prior strict exact |
| 0x80040220 | SAME; prior strict exact |
| 0x80040264 | SAME; prior strict exact |
| 0x800402a4 | SAME; prior strict exact |
| 0x80040308 | WIP, 11.4%; retail 72-byte frame versus probe 56-byte frame |
| 0x800416ec | SAME; prior strict exact |
| 0x8004177c | SAME; prior strict exact |
| 0x8004195c | SAME; prior strict exact |
| 0x80041b14 | SAME; prior strict exact |
| 0x80041cd0 | SAME; prior strict exact |
| 0x80041d7c | SAME; prior strict exact |
| 0x80041e0c | SAME; prior strict exact |
| 0x80041e94 | SAME; prior direct strict exact |
| 0x8004212c | SAME; prior direct strict exact |
| 0x80042298 | SAME; prior direct strict exact in the three-function scatter unit |
| 0x80042424 | SAME; prior direct strict exact in the three-function scatter unit |
| 0x800424f0 | SAME; prior direct strict exact in the three-function scatter unit |

Retail `0x80040308` constructs an optional-argument cursor at the fifth
named argument's stack slot, matching a source-shape lead from KF1. An
isolated manual-cursor source trial moved the probe cursor to that slot but
left focused similarity at 11.4%; moving the assignment after pool allocation
lowered it to 10.5%. Both trials stayed in temporary files. Changing the
null-allocation return from literal zero to the known-null record pointer
also emitted the same 11.4% listing and was reverted. The first retained
constructor divergence remains the frame and saved-register allocation.

Kind 102's word at 0x8009a5a8 has only two direct GAME references, its load
and frame-count-plus-30 store inside the constructor. Other words from
0x8009a5a0 through 0x8009a5e0 are used by identified Sony malloc and
Psy-Q CD code, immediately before `game_counter_bytes` at 0x8009a5e8.
That mixed neighborhood does not prove an enclosing game object or a source
definition for the timer, so its owner remains unresolved.

## Render and sparse-animation follow-up

This ten-function call-connected batch supersedes older render and sparse-morph
scores above. Each source was rebuilt against its current GAME carve. The five
former WIPs below have isolated direct objdiff verdicts; the five adjacent
controls were also checked with focused `kf try`. In particular, the two
previously reported frame/sparse WIPs now compare exactly without a source
change in this pass.

| GAME VA | Current verdict |
| --- | --- |
| 0x800311b0 | WIP, direct objdiff 92.14815% (324 bytes); retail saves an additional argument register and orders polygon setup stores differently. |
| 0x80031850 | WIP, direct objdiff 95.0% (1340 bytes); retaining a typed row pointer before the column index reproduces retail's row-first address calculation. Entry register assignment and later branch scheduling still differ. |
| 0x80031d8c | WIP, direct objdiff 94.65414% (532 bytes); retail saves the final render-data argument in `s7`, while the probe reloads it from the caller stack. |
| 0x800335a0 | Exact, direct objdiff 100% (1012 bytes); frame-render sibling 0x80033584 is also exact. |
| 0x80033afc | Focused SAME; earlier strict exact asset-registry control. |
| 0x80033b34 | Focused SAME; earlier strict exact keyframe control. |
| 0x80033bfc | Exact, direct objdiff 100% (196 bytes). |
| 0x80033cc0 | Exact, direct objdiff 100% (124 bytes). |
| 0x80033d3c | Exact, direct objdiff 100% (696 bytes); its three `ScaleMatrix` calls and loop tail now match the current retail carve. |
| 0x80033ff4 | Focused SAME; earlier strict exact sparse-find control. |

## Frame-resource transition continuation

This eleven-function batch follows the frame renderer through its map-cell,
resource, and image-transition controls. Each verdict below comes from an
isolated source compile and direct objdiff against the current GAME carve.
The resource unit also contains `0x80032274`, an audio-owned WIP that was
compiled but left untouched.

| GAME VA | Current verdict |
| --- | --- |
| 0x80030c18 | WIP, 96.521736% (460 bytes); an early orientation-byte load and flags move exchange order. |
| 0x80030de4 | Exact, 100% (376 bytes); supersedes the old 89.52128% strict report. |
| 0x80030f5c | Exact, 100% (200 bytes); grid-scan control. |
| 0x80032040 | Exact, 100% (112 bytes); single-cell layer-mask control. |
| 0x800320b0 | WIP, 73.95918% (196 bytes); row and column calculations and loop register allocation diverge. |
| 0x80032174 | WIP, 93.6% (100 bytes); comparisons and branches align, but the result and scratch register assignments differ. |
| 0x800321d8 | WIP, 98.4359% (156 bytes); the unowned `0x8009b0a0` arena literal still emits `lui/ori` rather than retail's relocatable `lui/addiu`. |
| 0x80032364 | Exact, 100% (280 bytes); TMD range-update control. |
| 0x8003494c | Exact, 100% (112 bytes); TIM upload control. |
| 0x800349bc | WIP, 96.31408% (1108 bytes); retail spills its return state in a 72-byte frame, while the probe keeps it in a register in a 64-byte frame. |
| 0x80034e10 | Exact, 100% (384 bytes); image-transition control. |

A temporary, typed `0x800320b0` probe calculated the z-row pointer before
the x-column expression, matching retail's broad arithmetic order. Focused
listing similarity rose from 26.9% to 40.4%, but direct objdiff fell from
73.95918% to 67.85714%. The declaration order alone does not establish the
original source and no change was retained. KF1's map-cell renderer uses
similar SDK matrix setup, but does not resolve the KF2 scratch-register
residue at `0x80030c18`.

## Clipped-map render follow-up

The contiguous `render_map.c` unit was rebuilt after shared type and
relocation refinements. Isolated direct objdiff gives `0x8002f194` 100%
(1052 bytes), `0x8002f5b0` 95.833336% (600 bytes), and `0x8002f808`
91.17697% (1876 bytes). The latter verdict supersedes an older 63.362473%
report. The clipped-triangle path still differs where retail transfers the
third vertex UV before subsequent color words, while the probe moves that
independent transfer later. The alternate prepared path still allocates a
168-byte retail frame versus the probe's 120-byte frame; no complete
additional local object or source definition is proved. Focused CFG has
13/13 blocks and 7/7 branches for the clipped-triangle path, and 51/51
blocks and 35/35 branches for the alternate prepared path, with known
successors aligned by block order. Its direct
`NormalClip`, `NormalColorCol`, `DpqColor`, `Clip4FTP`, `Clip3FTP`, and
clipped-triangle calls retain their reviewed referents. No source edit was
retained for register or stack-placement differences alone.

The latest focused rebuild still reports `0x8002f194` SAME, with
`0x8002f5b0` at 82.6% and `0x8002f808` at 58.9% listing similarity; these
focused numbers do not supersede the direct strict scores above. Retail reads
the FT4 vertex indices in 0/1/2/3 order as the source spells them, whereas
the compiler schedules the independent reads 0/2/1/3. Retail also writes the
third clipped-triangle UV before the colors, matching C statement order but
not the probe schedule. Neither observation warrants a source reorder.

## TMD preparation and subdivision continuation

This thirteen-function connected batch follows the prepared-object call from
`0x80030c18` into `0x8002ff5c` and the adjacent TMD packet walkers. Fresh
focused builds below retain the prior direct objdiff verdicts; `SAME` is a
listing result, not a new strict certificate.

| GAME VA | Current verdict |
| --- | --- |
| 0x8002d5dc | Focused SAME; prior direct strict 100% index-control helper. |
| 0x8002d8b0 | Focused SAME; prior strict exact TMD register helper. |
| 0x8002d8f0 | Focused SAME; prior strict exact TMD slot helper. |
| 0x8002d910 | Focused SAME; prior strict exact TMD release helper. |
| 0x8002d918 | Focused SAME; prior strict exact projection helper. |
| 0x8002da94 | Focused SAME; prior strict exact vertex helper. |
| 0x8002dbd8 | Focused SAME; prior strict exact transform helper. |
| 0x8002dc80 | Focused SAME; prior strict exact depth-transform helper. |
| 0x8002dd28 | Focused SAME; prior strict exact projection helper. |
| 0x8002ddb4 | WIP, 93.6% focused; prior direct 97.34716%, CFG 44/42 blocks. |
| 0x8002e4dc | WIP, 92.0% focused; prior direct 97.2784%, CFG 44/42 blocks. |
| 0x8002ebe0 | WIP, 88.7% focused; prior direct 95.27945%, CFG 26/25 blocks. |
| 0x8002ff5c | WIP, 12.0% focused; prior direct 51.50675%, CFG 11/11 blocks. |

The `0x8002ff5c` retail body has a 1248-byte frame, a shared packet-word
scratch at `sp+48`, and a 128-element `SVECTOR` midpoint workspace beginning
at `sp+64`; the C body preserves those extents and all fourteen
`resource_copy_words` call sites, but still allocates 1360 bytes. The raw
entry duplicates its packet-header load before the FT4/FT3 dispatch; the
source origin of that second load is not established, so no redundant read
was added. KF1 has no corresponding midpoint subdivider source. In
`0x8002ebe0`, temporary source-only trials mutating the blend argument in
place and widening the fixed-depth parameter with explicit `s16` casts both
compiled to the unchanged 88.7% listing. They were discarded; neither trial
established the original signature or prevented GCC from hoisting the
equivalent depth guard ahead of the four packet arms. A separate temporary
probe nested the positive-depth test around the triangle insertion in
`0x8002ddb4` and `0x8002e4dc`; both listings and their 44/42-block CFG gaps
were unchanged. KF1's TMD renderer uses a different ordering-table depth
policy, so its source is a shape lead only. No tracked TMD source edit was
retained.

The subdivider also stores halfwords to stack scratch and rereads individual
bytes before writing packet index fields. A temporary `WRITE_INDEX` variant
used a local halfword with byte views in place of direct shifts. It reduced
the candidate frame from 1360 to 1344 bytes, but focused listing similarity
fell from 12.0% to 7.6%; the 1248-byte retail frame and source-local scratch
layout remain unproved. The variant was not retained.

Another temporary probe built the first FT4 vertex-index words through a
two-halfword union, matching the retail `sh`/`sh`/`lw` scratch sequence in
shape. It left the focused listing at 11.9% versus the 12.0% baseline and
did not resolve the entry/frame divergence; it was also discarded.

## Effect constructor ownership handoff

The earlier `0x80040308` 11.4% focused verdict above is historical. The
separate actor-group callee pass retained retail-backed constructor case-body
ordering: its new focused score is 27.9%, and isolated strict `.text` is
69.41712% (5092 retail bytes versus 4456 compiled), with 116 retail versus
103 compiled CFG blocks. Kind 6 iterates 24 entries of 24 bytes in each of
four ring slots at `0x801d9628`, using the `DAT_8006d704` counter; the
complete storage owner and kind-102 Sony-runtime-adjacent timer owner remain
unproved. The constructor stays WIP, and this handoff leaves its source with
the actor-group worker.

## Effect constructor switch-entry follow-up

A later isolated GAME `game.effect_constructor` rebuild and direct strict
comparison puts `0x80040308` at 70.960724% `.text` (5,092 retail bytes),
with a 27.8% focused listing, still WIP. The raw 123-word switch table has
62 distinct in-body targets.
The previous C merged separate retail entries for kinds 11/54, 14/16/19,
27/51/52, 29/30/31/47/48, 33/53, 34/35/117, 103/121, and 104/122.
Raw case-entry instructions show the distinct immediate assignments,
initializer calls, and joins; the source now spells those entry-local paths
before their common tails. Its compiled table has 61 distinct targets, and
all 123 entries preserve the retail target-equivalence relation except the
pair 6/102. Those two source arms still share an unresolved no-op body while
retail enters separate buffer-copy and timer paths. The `0x801d9628`
workspace, `DAT_8006d704` index, and `0x8009a5a8` timer remain unowned; no
source storage claim or table-entry workaround was added.
Retail has 69 direct `jal` instructions in this body versus 67 compiled:
the unmatched target counts are one `audio_play_spatial_range` and one
`effect_play_spatial_sound`. The first direct instruction difference remains
the 72-byte retail frame versus the 56-byte compiled frame; prior
manual-cursor probes did not establish a safe source correction for it.
The adjacent `game.effect_update` unit was rebuilt as a focused control;
all ten functions, including both pool initializers, remain direct strict
100%.

## Constructor kind-6 trail ownership

The raw kind-6 block at `0x800409e4..0x80040aec` establishes the previously
missing trail path. Its `0x80040a48/4c` and `0x80040a78/7c` signed-low pairs
read and write the initialized-zero word `DAT_8006d704`, incrementing it
modulo four. The `0x80040a50/54` signed-low pair names the destination base
`0x801d9628`. A selected slot is 576 bytes, and the block copies 24
position/rotation rows of 24 bytes from the effect record, then clears two
trailer bytes, sets 150 updates, and calls `effect_play_spatial_sound` with
sound `0x22`. The updater reads the stored row pointer through record +0x40.

The source now models that four-slot trail and its index with address-derived
identities. The BSS claim covers the observed minimum `4 * 24 * 24 = 0x900`
bytes; original allocation extent and defining TU remain candidate. The two
index pairs and one base pair were checked against the raw words and admitted
by a safe one-VA GAME carve with zero withheld relocations. A focused rebuild
improves the constructor listing from 27.8% to 32.4%; fresh isolated strict
objdiff improves `.text` from 70.960724% to 76.343285% (5,092 retail bytes,
4,700 candidate bytes), with the four initialized data bytes exact. The
123-entry `.rodata` table remains WIP because kind 102 still has an unmodeled
entry, and the retail 72-byte versus probe 56-byte frame remains the first
divergence. No exact claim is made.

A related focused GAME effect-motion pass then rebuilt
`effect_rotate_scale_offset_y` (`0x800416ec`), `effect_move_probe`
(`0x8004177c`), `effect_aim_and_move` (`0x8004195c`), `effect_target_motion`
(`0x80041b14`), and both functions in `effect_spawn_zero_direction`
(`0x80041d7c`, `0x80041e0c`). Direct strict comparison reports 100% for
all six. Along with the ten exact `effect_update` functions above, this
16-function connected control batch required no source edits.

## Effect updater call and table audit

A fresh focused rebuild of `game.effect_update_dispatch` leaves GAME
`0x80042650` WIP: retail text is 13,936 bytes and the compiled body is
13,924 bytes. Moving the self-contained ballistic cases 29/31/48 and
30/47 to the switch start follows the first retail table targets: their
retail offsets are `+96` and `+104`, versus `+104` and `+112` in the probe,
an eight-byte difference after the probe's two extra saved-register stores.
Retail then enters kinds 7/49 at `+520`, 13/32 at `+612`, and kind 23 at
`+740`. The kind-23 non-phase-nine arm at `+1168` calls `func_80041e94`
and explicitly jumps back to the kind-7 shared path at `+520`. The source
now puts these cases in the same order and spells the shared jump directly,
preserving the existing behavior and argument values. The next retail
targets are kind 4 at `+1220`, kinds 34/35 at `+1344`, and kind 25 at
`+1352`; their self-contained C groups now follow kind 23 in that order.
Retail next enters kind 42 at `+1452`, kind 115 at `+1504`, and kind 113 at
`+1732`; these three independent groups were moved next, with their bodies
and calls unchanged. Kinds 46, 45, 116, and 117 then follow retail at
`+1912`, `+2232`, `+2256`, and `+2488`. Their independent C arms now appear
in that order too. The following retail targets are kind 40 at `+2692`,
kinds 39/38 at `+2748/+2784`, kind 50 at `+3092`, and kinds 28/1 at
`+3356/+3364`. Those self-contained C groups now follow in that order.
Kinds 26/27 at `+3600` and kind 111 at `+3768` then precede kind 0 at
`+4052` in retail; their unchanged C arms now sit in the **outer kind
switch** before kind 0. An earlier text move accidentally nested them in
kind 50's phase switch and changed dispatch semantics; that probe was
discarded despite compiling. The next independent retail groups are kinds
103/121 at `+4272`, 104/122 at `+4952`, and 11/54 at `+5288`.
Moving their unchanged arms after outer kind 0 yields candidate targets
`+4176`, `+4848`, and `+5188` in the same physical order. Focused listing
similarity rose to 32.9%. The following independent retail groups are kinds
51/52 at `+5448/+5472` and 118/119 at `+5508/+5516`. Moving their unchanged
arms next yields candidate targets `+5360/+5384` and `+5420/+5428`, in the
same order. Retail kind 2 at `+5576` is followed by kinds 20, 12, and 100
at `+5736`, `+5760`, and `+6108`. Their independent C arms now follow kind 2;
candidate targets are `+5592`, `+5608`, and `+5944` in the same order.
The next retail groups, kinds 5, 105, and 9, also now follow kind 100 in
that order. Kinds 53/33, 106, 8, 10, and 6 follow kind 9 in the retail
table; their C arms now follow in that order too. All moved bodies and calls
were unchanged. Kinds 107, 101, and 102 follow kind 6 in retail and now
follow it in C. The next retail groups are kinds 15, 17, 16, 14, 19, 22,
and 3; those independent C arms now follow in that order. Retail's remaining
distinct kind targets follow as 114, 24, 109, and 120 before the default.
The kind-114 arm now precedes kind 24 in C, matching the raw pointer-target
order. Both retail and candidate have the same physical order for all 59
distinct classes in the 123-row main kind table. Each reordered group ends
in a `break` or an explicit transfer; the shared kind-118/119, kind-53/33,
and kind-20 paths retain their explicit `goto` joins. Focused listing
similarity is now 47.4% (13.0% before these moves), and direct strict text
was 74.016360% against the prior target object (13,936 retail bytes versus
13,924 candidate bytes). An earlier four-case
move temporarily lowered strict text from 20.613089% to 17.417624% despite
aligning the preceding target order. The function remains WIP. Both 516-byte
`.rodata` sections contain 128 `R_MIPS_32` pointer rows. Canonicalizing their
in-body target addends by first-occurrence class gives the same class at all
128 indices, with 64 distinct classes on each side. Raw `.rodata` similarity
is now 24.902723% (14.883268% before the case moves): body-offset changes
lower this byte metric,
with no evidence for a missing table entry. Retail allocates 224 stack bytes
and saves five `$s` registers, while this probe allocates 192 bytes and saves
seven. Individual case bodies and CFG still diverge. No additional live stack object is
established by the frame difference.

Retail and the current compiled body each have 27 direct calls to
`func_80040308`; the source contains 27 constructor call expressions. In an
earlier case order, the compiler emitted only 26 calls: for kind 114, retail
kept a separate kind-3 call at body offset `+13208`, while that candidate set
`a2 = 3` in the delay slot at `+12664` of a jump to `+13412`, sharing the
constructor call at `+13416` with a later dynamic-kind path. Moving kind 114
to its raw-supported physical position restored the separate compiled call.
A fresh direct strict comparison
keeps all ten `game.effect_update` functions and all three `game.effect_reset`
functions, including sole caller `effect_pool_sweep`, at 100%. The ballistic,
growth, and retail case-order moves retain their call arms and pointer table.
Other off-tree
phase-case ordering probes were discarded because their source-level joins
are not yet established.

The previous off-tree move of kinds 103/121, 104/122, and 11/54 used the
semantically invalid nested-switch base; it lost two pointer rows and was
discarded. Repeating those moves from the repaired outer switch keeps all
128 pointer rows and the 74.016360% text / 24.902723% data WIP verdict
above. Each of the 64 target equivalence classes is preserved across all
128 pointer indices, including the five secondary phase-table rows. The
retail and current compiled bodies each have 27 constructor calls to
`func_80040308`, as detailed above. Fresh focused compilation and isolated
strict comparison leave all ten `game.effect_update` and three
`game.effect_reset` helper function bodies at 100% text; the reset unit's BSS
remains a separate data WIP. The dispatcher itself remains WIP.

The kind-23 actor update passes a player-space origin to
`func_8003c3e0`. Retail `0x80042abc/0x80042ac0` forms `0x801985a8`, or
`player_state + 0xd8`, immediately before that call. The former source used
an incompatible `VECTOR` cast of `player_state.unknown_e8` at `+0xe8`.
It now passes the typed `player_state.camera_position` at the observed
`+0xd8`; the focused body still compiles. On the same refreshed narrow
target object, isolated strict text changes from 74.044780% before this
source correction to 74.047646% after it. This is a referent correction,
not a codegen experiment. The target was refreshed with 66 independently
decoded BSS HI16/LO16 pairs and the phase-table base pair, all admitted by
the safe delinker with zero withheld rows. The raw kind-6 phase-table selector at
`0x80044f60..0x80044f88` bounds its unsigned phase to 0..4, loads one of
five rows at `0x8001287c..0x8001288c`, and jumps to five addresses inside
the dispatcher. The table identities now name the dispatcher's RODATA owner;
the bounded base pair was promoted from candidate to reviewed evidence.

Kinds 103/121 previously excluded phase values above two before the collision
probe. Retail starts at `0x80043700` by checking only for phase two: that
phase takes the fade path, while every other phase reaches the collision
call at `0x8004373c`. Phase zero then takes the timer path, phase one may
take the direction update, and values above two continue to the random-child
loop. The C now uses that early phase-two exit and restricts direction updates
to phase one, preserving the raw phase-three-plus path. On the same refreshed
target, isolated strict text rises from 74.047646% to 74.537600%; the
candidate body shrinks from 13,924 to 13,916 bytes. The 516-byte RODATA
still has 128 pointer rows and all 64 target classes in the same physical
order; raw RODATA similarity rises to 25.389105%. Focused listing remains
47.4%, with 446/459 retail/candidate CFG blocks and 217/222 branches, so
the function remains WIP.

Kind 9 has two distinct retail collision-result branches: the actor/player
path checks the `func_80041b14` result at `0x800444d8`, and the special
`0xff` path checks `func_80042298` at `0x8004459c`. Both choose the shared
impact or non-impact arms without re-reading the actor index. The former C
combined those results in one predicate after the calls. Moving each check
immediately after its call follows the raw control flow and keeps the impact
and non-impact behavior. The kind-9 group now has five conditional branches
and six direct calls on both sides. Isolated strict text rises from 74.537600%
to 74.726460%, with a 13,888-byte candidate body; RODATA similarity rises
from 25.389105% to 29.863813%. Focused listing is 47.5%, CFG blocks are
446/456 and branches 217/221, and all 128 pointer rows still preserve the
64 retail target classes. The function remains WIP.

In kind 105, retail `0x800443a0..0x800443bc` loads four words from the local
`VECTOR` at stack `+88..+100` and stores all four to the record position at
`+20..+32`. The prior C copied only X/Y/Z, omitting the fourth pad word.
Assigning the complete `VECTOR` reproduces that four-word copy in the
focused compiler; no defining write to the local pad is visible on this
path, so its source value remains unresolved. Isolated strict text improves
from 74.726460% to 74.813150% with the same 13,888-byte candidate and
29.863813% RODATA result. The global CFG and all 128 pointer classes remain
as above; exact closure is not claimed.

Two off-tree shared-join spellings were discarded. Replacing kind 12's
`reset` branch with an explicit label gave 74.324340% strict text versus
74.537600% on the same then-current base, and six candidate conditional
branches in that group versus seven in retail;
source syntax for that join remains unproved. Replacing the kind-103/121
transition state with direct labels gave 74.535880% strict text versus
74.726460% on the same current base and did not establish the original
source form. Both
trials kept the table rows, but the retained C better preserves the available
source evidence and direct text result.

Kind 24's constructor call passes a pointer in stack argument five. Retail
forms `record + 0x34` at `0x80045b3c`, which is the record's `direction`;
the C previously passed `rotation` at `+0x24`. Correcting the referent makes
the focused compiler use `addiu ...,record,52` in the same call arm. On the
same curated target, strict text moves from 74.813150% to 74.813430%; the
13,888-byte candidate and 29.863813% RODATA result remain unchanged.

Kind 120's retail control flow at `0x80045be0..0x80045c24` branches from
the first random check directly to the spawn arm, or saves the collision
result in `$s0` and tests both zero and masked flags before joining the
rotation tail. The prior C kept a `finish` Boolean, which made the candidate
retain the random result in `$s0` and collapse the collision tests. A direct
early exit to the rotation label preserves the same effects and produces the
retail two-branch collision topology, including the `move s0,v0` and
`andi v0,s0,5` delay-slot sequence. Isolated strict text rises from
74.813430% to 75.147250% on the same 13,888-byte candidate; RODATA stays
29.863813%. Focused similarity is 47.8%, with CFG blocks 446/456 and
branches 217/221. All 128 pointer relocation rows remain at the same offsets
and retain the 64 retail pairwise target classes. The updater remains WIP.

Kind 6's phase-three timer expiry and phase-four emission loop converge on
one retail actor cleanup block at `0x80045474`: it indexes the actor, clears
bit `0x800` in `unknown_28`, then frees the effect. The C had duplicated that
cleanup in both phase arms, causing four candidate `actor_state+0x28`
relocation pairs where retail uses one `actor_state` base pair for the shared
tail. Routing both arms to a common cleanup label reproduces the shared
control flow and the base-plus-field access. Candidate actor-state HI16/LO16
pair count falls from 15 to the retail 12. Against the same target, isolated
strict text rises from 75.147250% to 75.994545%, and candidate text shrinks
from 13,888 to 13,804 bytes. RODATA similarity falls from 29.863813% to
20.719845% as the code addresses move, but all 128 rows still preserve the
64 retail target equivalence classes. This remains a WIP match.

The two kind-105 collision outcomes decrement a linked effect record, not
the actor's rotation. Retail `0x80044300..0x80044334` and
`0x80044364..0x8004439c` each index `effect_state.records` with the byte at
`record+0x40`, then test and decrement the signed halfword at linked-record
`+0x42`. The previous C used `actor->rotation.y` in both arms. Each arm now
forms its own typed effect-record pointer before accessing the WIP tail
halfword. This restores both missing `effect_state+0x680` relocation pairs
and their field offsets. Strict text rises from 75.994545% to 76.364810%,
candidate text grows from 13,804 to 13,868 bytes, and RODATA rises from
20.719845% to 27.918287%. The actor's role in the earlier position helper
calls remains unchanged.

In kind 5, retail forms the actor-record base pointer at `0x80044104..0x80044110`
before checking the nonzero effect count, then passes its position field to
`func_80039c94`. The prior C formed `&actor_state.actors[index].position`
only inside that call, giving an extra `actor_state+0x2c` relocation rather
than the retail base relocation and a different branch schedule. Keeping a
typed actor pointer across the count check reproduces the base addend and
raises strict text from 76.364810% to 76.560850%; candidate text is 13,872
bytes and RODATA remains 27.918287%. The current candidate and retail now
have identical multisets of named direct-call targets and named data
HI16/LO16 low addends. At this checkpoint candidate `.rel.text` has 467 rows
against retail 470, with the three-row difference confined to internal jump
relocations. The 128 pointer rows and 64 target equivalence classes remain
unchanged.

Kind 6 phase three increments its frame byte at retail `0x800453c0`, stores
the incremented byte at `0x800453cc`, then checks the truncated value against
24 and resets it to zero only on wrap. The former C selected the stored value
with a conditional expression, delaying the first store and adding a jump.
Using an increment followed by a wrap check makes the focused compiler emit
the retail local instruction sequence, including the initial store and no
extra jump. Isolated strict text falls from 76.560850% to 76.418200% because
the shortened arm shifts later code; RODATA remains 27.918287%, candidate
text becomes 13,868 bytes, and named referents stay identical. The current
candidate has 466 `.rel.text` rows versus retail 470, all four missing rows
being internal jump relocations. The source-backed local correction is kept;
the updater remains WIP.

The kind-6 phase-four 32-iteration loop compares its decremented index to
`-1` in retail at `0x80045464..0x80045470`; the C used a nonnegative test,
which emitted `bgez`. Spelling the finite loop's `index != -1` condition
emits the retail `li -1; bne` and delay-slot constant, with unchanged loop
behavior. Strict text rises from 76.418200% to 76.855340% on the same target;
candidate text is 13,872 bytes and RODATA remains 27.918287%.

The earlier growth arm and kind 13 converge on the same retail collision
block at `0x800428d8`, where `func_80042424` and `func_8003feb0` run before
the growth update at `0x80042908`. The C previously kept those shared labels
at the end of the function, so both candidate arms jumped to a far tail.
Moving the labels into the kind-13 physical region makes the focused object
emit both calls there and improves the ordered named call/data relocation
sequence from 259 to 261 matching entries out of 274 under an order-preserving
alignment. All 128 table rows retain their exact relocation offsets and
symbol class sequence, and all 64 target classes keep the same physical
order; the named referent/addend multiset remains identical to retail.
Isolated strict text rises from 76.855340% to 77.307690% on the same
13,872-byte candidate. RODATA similarity falls to 17.996109% as the pointer
targets shift. The dispatcher remains WIP.

Within that local growth block, retail kind 13 branches from nonzero phase
at `0x800428b4`, then branches on a zero collision result at `0x800428d0`
past the shared collision calls to the helper path at `0x800428f4`.
Separating those paths with labels reproduces the same local order and
conditional-branch polarity in the focused object. Strict text changes from
77.307690% to 77.267800% as later code shifts four bytes, while RODATA
rises from 17.996109% to 32.587547%; candidate text is 13,868 bytes.
The 128 ordered pointer relocations, 64 class identities and physical class
order, and named call/data referent multiset are preserved. This raw-backed
CFG correction is retained despite the small aggregate text decrease.

The phase snapshot originates from `lbu` at the dispatcher entry. Keeping
that zero-extended value in an `s32` local, rather than narrowing it back to
`u8`, removes repeated candidate `andi 0xff` operations at the growth arms
while preserving the entry load and all phase values. Focused text shrinks
from 13,868 to 13,832 bytes; isolated strict text rises from 77.267800% to
77.692310%, with RODATA at 29.766537%. All 128 ordered pointer rows, 64
class identities and physical order, and the named call/data referent
multiset remain aligned with retail. The updater remains WIP.

The shared scale tail belongs just before kind 6 in retail: its
`func_80041cd0` call at `0x80044f30` is followed by the common rotation-Y
increment at `0x80044f38..0x80044f48`. The C formerly placed this tail
after every switch case. Moving the label into the preceding kind-10 region
retains all incoming paths and places the call and rotation update at the
same physical point in the candidate. The ordered named call/data alignment
improves from 261 to 262 of 274 entries, while all 128 ordered pointer rows,
64 classes and their physical order, and the named referent multiset remain
unchanged. Isolated strict text changes from 77.692310% to 77.613950% and
RODATA from 29.766537% to 19.649805% as later addresses move; candidate
text is 13,828 bytes. The raw-supported call placement is retained.

The other shared tail, `func_80041e94`, sits at retail `0x80044aa0..0x80044ab0`
between kind 8's phase-zero and phase-one code. It has two decoded incoming
paths, from kind 12 at `0x80043de8` and from kind 8 at `0x80044a68`.
The C had placed this common spawn call after every switch arm. Splitting
kind 8's phase dispatch around a local spawn label preserves those paths
and places the call before phase one, as in retail. The candidate call is now
at body `+0x23b4`, against retail `+0x2454`; ordered named call/data alignment
improves from 262 to 263 of 274 entries. Isolated strict text rises from
77.613950% to 78.411020%, RODATA from 19.649805% to 30.642023%, and
candidate text shrinks from 13,828 to 13,824 bytes. All 128 pointer-row
offsets/symbols, 64 target classes and their physical order, and the named
referent multiset remain aligned; the updater is still WIP.

Kind 100's two phase-range paths had the correct calls but the opposite
physical order. Retail branches at `0x80043e3c` into the collision probe
at `0x80043ea4`, leaving the direction increment and `func_80042298`
path as fallthrough. A local label now expresses that control flow while
preserving the old phase and miss behavior. The candidate places those calls
in retail order. A focused rebuild and isolated strict comparison raise
updater text from 78.411020% to 79.511765%; RODATA stays 30.642023% and
candidate text stays 13,824 bytes. All 128 pointer rows and 64 target
classes retain their pairwise and physical order, and the named call/data
referent multiset remains exact. The updater remains WIP.

Kind 12's retail phase dispatch has direct paths: phase 101 increments and
exits, 100 enters the child spawn at `0x80043d90`, 102 enters the reset at
`0x80043dbc`, and 110 enters the shared scale setup at `0x80043d08`.
The prior C used a temporary reset flag, and its compiled object emitted a
constant condition after the child spawn. Explicit local reset/scale labels
model the decoded paths without that redundant condition. The focused
candidate shrinks from 13,824 to 13,792 text bytes. Isolated strict text
changes from 79.511765% to 79.430540%, and RODATA from 30.642023% to
24.027237%; the address movement affects global alignment, so the
source-backed control-flow correction is retained. All 128 pointer rows,
64 pairwise target classes and their physical order, and the named call/data
referent multiset remain aligned with retail. The updater remains WIP.

Within that kind-12 dispatch, retail branches on a nonnegative collision
result at `0x80043d7c` to the `func_80041e0c` path at `0x80043df0`.
The miss path falls through to sound and child creation, then the reset and
scale setup precede the collision-success body in physical order. A local
collision label now gives the C that same order. The candidate emits the
same branch polarity and block sequence; isolated strict text rises from
79.430540% to 79.590700%, with RODATA unchanged at 24.027237% and text
still 13,792 bytes. The 128 pointer rows, 64 target classes and their
physical order, and named referent multiset remain exact.

Retail has a shared phase-increment block at `0x80044f4c`, immediately
after the shared scale/rotation tail. Decoded incoming edges include kind
12 phase 101 at `0x80043cd4` and three kind-100 paths at `0x80043e78`,
`0x80043ee8`, and `0x80043f3c`. Routing those four source paths to a
common label after the scale tail gives the candidate the same local
instruction sequence (`lbu`, load-delay `nop`, `addiu`, `j`, delay-slot
`sb`) and physical order. The candidate block is currently at body
`+0x2828`, versus retail `+0x28fc`; other incoming retail paths still need
review. Focused candidate text shrinks from 13,792 to 13,776 bytes,
isolated strict text rises from 79.590700% to 79.954930%, and RODATA
rises from 24.027237% to 28.696499%. All 128 pointer rows, 64 target
classes and their physical order, and named referent multiset remain exact.

The other five decoded incoming edges to that shared increment are three
kind-104/122 paths at `0x80043a34`, `0x80043a78`, and `0x80043af0`, plus
two kind-10 phase-dispatch paths at `0x80044c98` and `0x80044cac`.
Those source paths now use the common label as well. Focused text shrinks
from 13,776 to 13,752 bytes; isolated strict text rises from 79.954930%
to 80.417046%, and RODATA from 28.696499% to 29.280155%. All 128
pointer rows, 64 target classes and their physical order, and the named
referent multiset remain exact. The updater is still WIP.

The kind-29/31/48 age-zero check at retail `0x800426c4` uses `lh` from
record tail `+0x42`; the next age arithmetic reload at `0x800426f0`
uses `lhu` from the same address. A direct signed-halfword view for the
zero check gives the candidate the retail `lh` opcode while preserving the
later unsigned reload. The focused object remains 13,752 text bytes;
isolated strict text rises from 80.417046% to 80.432840%, with RODATA
unchanged at 29.280155%. This is an instruction-width correction, not a
new object or field owner.

The same ballistic arm computes the projected Y expression before storing
projected X and Z in retail. The C had assigned X first, making the probe
load X inputs before the Y multiply chain. Putting the Y expression first
preserves the same arithmetic and gives the candidate the retail ordering
from the age reload through both multiplies, then the X/Z stores. Focused
candidate text stays 13,752 bytes; isolated strict text rises from
80.432840% to 80.707520%, with RODATA unchanged at 29.280155%. Pointer
class and named-referent controls remain exact.

The age value itself is a signed 16-bit local. Declaring it `s16` lets the
probe retain the unsigned halfword reload for the storage increment, then
sign-extend once for the Y multiply chain and delay the halfword store to
the same point after the three multiplies as retail. The candidate's local
instruction sequence is now the retail sequence shifted eight body bytes
by the preceding entry code; register allocation and the larger retail
frame still differ. Focused candidate text remains 13,752 bytes, while
isolated strict text rises from 80.707520% to 80.915610% and RODATA stays
29.280155%. The 128 pointer rows, 64 class relations and physical order,
and named referent multiset remain exact.

The growth arms for kinds 7/49 and 13/32 copied the entry phase into a
`shared_growth_phase` local that was never assigned another value. Using
`initial_phase` directly preserves every path and removes the candidate's
extra register move before the kind-7 phase test. Its branch delay slot
now contains the same `slti phase,3` as retail at `0x8004285c`.
Candidate text shrinks from 13,752 to 13,744 bytes. The physical shift
moves isolated strict text from 80.915610% to 80.854770% and RODATA from
29.280155% to 23.735409%; the raw-supported source simplification is
retained. All 128 pointer rows, 64 class relations and physical order,
and the named referent multiset remain exact.

Kind 23 takes its four-word `old_position` snapshot after both
`func_8003c000` and `func_8003c10c` return. Retail copies the four words
at body `+0x34c..+0x368`, then updates the live record position from the
returned pointer. The C had initialized `old_position` before either
call, which was an observable ordering difference if those calls touch the
record. Moving the snapshot after the calls gives the candidate the same
local load/call/copy/update sequence through body `+0x394`. Its stack
offsets still differ because the retail and candidate frames differ.
Focused candidate text is 13,748 bytes; isolated strict text rises from
80.854770% to 81.348740%, and RODATA from 23.735409% to 29.571985%.
All 128 pointer rows, 64 class relations and physical order, and named
referent multiset remain exact.

Kind 23's scale update also uses the record field as the intermediate:
retail stores the incremented X halfword before the signed cap check,
conditionally overwrites X with `0x1800`, then reloads X once and stores
that value to Z followed by Y. The prior C capped a local value before
any X store. Updating X first and using a chained Y/Z assignment yields
the retail local instruction order from body `+0x394` through `+0x3f4`;
the field store before the check is a real source-order distinction.
Focused candidate text is 13,756 bytes, isolated strict text 81.480770%
(up from 81.348740%) and RODATA 27.626460%. The 128 pointer rows,
64 target classes and physical order, and named referent multiset remain
exact. The updater is still WIP.

Kind 4 contained a real axis error: retail increments rotation Y at record
offset `+0x26`, whereas the C incremented rotation Z at `+0x28`. The
source now uses `rotation.vy`. Retail also branches on zero collision to
the clear-flag path at body `+0x514`, leaving the hit path and optional
`func_8003feb0` call as fallthrough before the common rotation/update
tail. A local zero-collision label gives the candidate the same branch
polarity and complete instruction sequence over body `+0x4c4..+0x53c`
apart from saved-register allocation and global return targets. Focused
candidate text is 13,760 bytes; isolated strict text rises from
81.480770% to 81.508896%, and RODATA from 27.626460% to 30.642023%.
All 128 pointer rows, 64 class relations and physical order, and named
referent multiset remain exact.

Kind 46's phase byte at record `+0x40` dispatches through two explicit
checks in retail: `beqz` at body `+0x7b4` to phase zero, then comparison
with one at `+0x7bc..+0x7c0` to the phase-one arm; other values return.
The prior C `if`/`else if` compiled phase zero as fallthrough and skipped
the second check on that route. Expressing the two arms as an inner
two-case `switch` produces the retail check order and branch polarity.
Focused candidate text is 13,768 bytes; isolated strict text rises from
81.508896% to 81.912740%, while RODATA moves from 30.642023% to
26.945526% as later case addresses shift. All 128 pointer rows,
64 class relations and physical order, and named referent multiset remain
exact. The phase-zero height/cap body still differs and remains WIP.

For kind 46 phase zero, retail stores the collision-cache result to
position Y, reloads that global result, subtracts the height-limit global,
then branches to the `0x7fff` cap store only when the signed height is
above the cap. The C now reads the result global for the subtraction and
spells the uncapped assignment as the first branch arm. The pinned probe
still folds the two result loads into one, so the data-load residue remains;
it now emits the retail `bnez` polarity and height-first/cap-second
store order. Candidate text is 13,772 bytes. Isolated strict text moves
from 81.912740% to 81.752870% as code addresses shift, while RODATA
rises from 26.945526% to 32.295720%. The 128 pointer rows, 64 class
relations and physical order, and named referent multiset remain exact.

Kind 117's elevated point contains only three copied position components.
Retail body `+0x9ec..+0xa08` stores X, adds 5000 to Y, and stores Z; it
does not copy the fourth `VECTOR` word. Replacing the whole-structure copy
with those three field assignments removes the candidate's extra fourth
word load/store and reproduces the local instruction order, apart from
the frame offset and saved-record register. Isolated strict text rises
from 81.752870% to 82.632610%; RODATA is 31.712063% after address shifts.

Kinds 39 and 38 share a collision-response body, but retail places the
kind-38 `func_80042298` call before the kind-39 `func_8004195c` call. The
kind-39 age check branches into the later spawn call at body `+0xb00`;
the kind-38 call branches to the shared response on a nonzero result and
otherwise jumps to the shared finish. The spawn call compares its raw
return directly against `-1`, then falls into the response only on equality.
The earlier C materialized Boolean trigger values and placed the spawn
call first. Two labels now express the retail control flow without an
intermediate Boolean; the candidate reproduces the local branch/call
sequence at `+0xaac..+0xb2c` except for the preceding address shift.
Focused candidate text is 13,760 bytes. Isolated strict text rises from
82.632610% to 83.171070%; RODATA is 30.739300%. All 128 pointer rows,
64 class relations and physical order, and the 272 named referents remain
exact. The overall CFG remains WIP at 446/451 blocks and 217/220 branches.

The same kind-38/39 response loop terminates on `count == -1` in retail:
after decrementing the counter, it loads `-1` and uses `bne` to repeat,
with the next argument setup in the delay slot. Spelling the C loop as
`count != -1` reproduces those four local instructions. It adds four bytes
and shifts later addresses, so isolated strict text moves from 83.171070%
to 83.045920% while RODATA stays 30.739300%; the source is retained on
the direct loop-control evidence. Pointer classes and named referents remain
exact.

Kinds 26/27 had another axis error: retail body `+0xe24..+0xe30` reads and
writes rotation Y at record `+0x26` when adding 100, while C used rotation
Z at `+0x28`. The source now updates Y. After the constructor call, retail
also checks phase zero first, phase one second, then returns for other
values. An inner two-case switch reproduces that branch topology, including
the kind-zero branch and the explicit other-value exit. Focused candidate
text is 13,772 bytes; isolated strict text rises from 83.045920% to
83.438286%, RODATA rises from 30.739300% to 32.392998%, and internal jump
relocations rise from 465 to 466 against the target's 470. All pointer
classes and named referents remain exact. The phase-one arithmetic still
differs: target uses `addiu -512`, while this probe hoists `-512` into a
register and uses `addu`.

Kind 0 had a third axis error: retail body `+0xfd4..+0xfe8` increments
direction Y at record `+0x36` by 20, while C incremented direction Z at
`+0x38`. The scale growth reads `scale_z` with `lhu` at `+0xfec`, so the
source now uses the unsigned halfword before adding 256. These changes
restore both address and load width; isolated strict text rises from
83.438286% to 83.524970%, with RODATA unchanged at 32.392998%. The 128
pointer rows/classes/order and 272 named referents remain exact.

Kinds 103/121 phase two use the stored X scale as the cap decision in
retail: body `+0x10cc` writes X, then `lh` rereads X at `+0x10d0` before
the positive test, while the Y store sits in that test's delay slot. The
prior C tested an untruncated local. Reading the record field after its
write now reproduces this local store/reload/branch order and preserves
the signed halfword decision. The pinned probe still chooses a register
constant plus `addu` for `-128` instead of retail `addiu -128`. Isolated
strict text shifts from 83.524970% to 83.521240%; RODATA is unchanged at
32.392998%. Pointer classes and named referents remain exact.

The kinds 103/121 transition arm also had a source-order mismatch. In
retail, a nonzero collision with low flags and nonzero prior phase calls
`func_80042424`, probes the collision height, computes the lower bound,
and jumps directly to the spawn block. Phase one can reach the same spawn
block when its height distance reaches 7000; the phase-zero countdown is
physically after that block and then joins the random-spawn loop. The old
C used a temporary transition flag, placed the phase-zero countdown before
the spawn block, and duplicated the height calculation in the compiled
path. A direct label for the spawn block preserves the two distinct
`func_80042424` calls and matches the retail branch/call order through the
collision split; it also shares the calculated distance with both spawn
calls. Isolated strict text rises from 83.521240% to 84.252010%, while
RODATA moves to 31.517510% as later case addresses shift. Candidate text
is 13,744 bytes. All 128 pointer rows, 64 class relationships/order, and
272 named referents remain exact; this arm remains WIP in register and
height-selection scheduling.

The following four-iteration random-spawn loop also terminates on
`index == -1`: retail loads `-1` and uses `beq` at body `+0x1344..+0x1348`,
then jumps back to its `rand` call. Changing the C condition from
`index >= 0` to `index != -1` reproduces this loop tail. Isolated strict
text rises from 84.252010% to 84.561424%, RODATA to 33.754864%, and
candidate text is 13,748 bytes. Pointer classes and named referents remain
exact.

Kind 2 passed the wrong position to `func_8003ff18`. Retail body
`+0x15f0..+0x1614` builds a three-component stack point with the record's
X and Z but Y raised by 1000, then passes that point as the first
argument. The old C passed `&record->position` directly. The scale step
also reads unsigned halfwords at record `+0x2c` and `+0x42`; the C had
signed casts. A local elevated point and unsigned step reproduce the
retail load/store and call-argument sequence, including no fourth-vector
copy. Isolated strict text rises from 84.561424% to 84.804535%, RODATA
to 34.046690%, and candidate text is 13,788 bytes. All 128 pointer rows,
64 target classes/order, and 272 named referents remain exact.

Kind 12's phase predispatch is a balanced comparison tree in retail:
phase 101 is checked first, then the remaining values split at 102 before
checking 100, 102, and 110. Replacing the flat C `if` chain with a switch
over the same phase values produces the same first split and ordered case
checks. Moving the shared scale label immediately after the switch gives
phase 110 the retail `bne` to default, with the scale setup as fallthrough;
the reset path jumps back to that shared label. The candidate now matches
the dispatch control sequence through the default entry, aside from an
extra register move in the first delay slot. Isolated strict text rises
from 84.804535% to 85.272385%, RODATA moves from 34.046690% to
32.490273%, text is 13,816 bytes, and internal jump relocations are 468
versus retail 470. All pointer rows, classes, physical order, and named
referents remain exact.

At the kind-12 default entry, retail reloads the phase byte from the
record (`lbu` at body `+0x16d0`) before testing for the initial sound;
the switch's saved phase value is not reused. Reading `record->phase`
there removes the candidate's saved-value test and gives the same
load-delay check. Isolated strict text rises from 85.272385% to
85.323770%, RODATA to 33.657590%, with 13,820 text bytes and unchanged
pointer/referent controls.

Kind 100's collision-probe result also chooses blocks in a physical order
the old C obscured. Retail branches on a result other than `-1` at body
`+0x1860` to the collision-success block, leaving the child-spawn miss
block as fallthrough; success then performs the update and effect call.
Putting an explicit success label after the miss block reproduces that
branch polarity and the call order from the probe through the shared
phase-increment exit. Isolated strict text rises from 85.323770% to
86.077210%; RODATA stays 33.657590% and candidate text stays 13,820
bytes. The 128 pointer rows/classes/order and 272 named referents remain
exact.

Kind 5's phase dispatch checks zero, then one, then exits on other values
at retail body `+0x18f4..+0x1908`. The old C compiled phase zero as
fallthrough and hoisted actor index from record `+0x41` before that check.
Retail instead loads the index inside phase zero and again inside each
child-spawn iteration. An inner phase switch with direct record reads
reproduces those branch and load sites. The child loop counts down from
`count - 1` to `-1`; retail decrements its loop counter and compares
against `-1` around `+0x198c..+0x1994` and `+0x19c8..+0x19dc`.
That form restores the loop tail and all iterations. Isolated strict text
rises from 86.077210% to 86.827780%, RODATA is 33.560310%, candidate
text is 13,808 bytes, and `.rel.text` is 469 versus retail 470. All
128 pointer rows/classes/order and 272 named referents remain exact.

In kind 5's phase-one magic call, retail truncates the return from
`effect_magic_power` before passing it as `func_80039c94`'s `u16` power
argument (`andi a1,v0,0xffff` at body `+0x1b50`). The sibling effect
callers also store this return in `u16 power`. An explicit conversion in
the dispatcher restores that instruction. Isolated strict text rises to
86.878010%; RODATA remains 33.560310%, and all 128 pointer rows,
classes, physical order, and 272 named referents remain exact.

Kind 105 first updates rotation Z, then tests phase two before loading its
actor index at record `+0x41`; retail has the phase branch at `+0x1b90`
and the `lbu` at `+0x1b98`. Moving the C actor-index read into the
non-phase-two arm restores that order. Isolated strict text reaches
87.102180%. RODATA is 31.906614% after compiler layout movement; all
128 pointer rows/classes/physical order and named referents remain exact.

Retail kind 105 places its phase-two scale update after the actor position
calculation and before the shared collision check (`+0x1c18..+0x1c34`).
It then dispatches the actor path through explicit phase-zero and phase-one
tests at `+0x1c60..+0x1c70`; other phases leave without copying the
position. Source labels for these raw branch targets restore that order and
the missing nonzero-phase exit. Isolated strict text reaches 88.363950%;
RODATA is 31.322958%, candidate text is 13,832 bytes, and `.rel.text`
now has 470 rows versus retail 470. The 128 pointer rows/classes/order
and 272 named referents remain exact. The unit is still WIP.

Kind 5's phase-one scan uses two ascending 72-byte record pointers and a
counter from 127 down to `-1` in retail (`+0x1a14..+0x1a9c`). A pointer
walk with a descending iteration count recovers the same scan topology;
the pool remains 128 records. With the pointer increment before the counter
decrement, isolated strict text reaches 88.415900%. RODATA is 28.696499%
after layout movement; `.rel.text` remains 470/470, and all 128 pointer
rows/classes/order and 272 named referents remain exact.

The kind-5 child constructor receives record direction at `+0x34`, not
rotation at `+0x24`: retail computes the fifth-argument pointer with
`addiu s4,s2,52` at body `+0x1998`. Correcting the C pointer changes the
candidate immediate from 36 to 52 without disturbing the strict score or
the exact pointer/referent controls.

Kind 9's repeated `func_80041e94` emission begins with count 11 and
decrements to `-1` in retail (`+0x1e98`, `+0x1ef4..+0x1efc`). The C loop
now expresses the same twelve iterations and countdown, raising isolated
strict text from 88.415900% to 88.596725%; RODATA and exact pointer/
referent controls are unchanged.

Kind 9 tests the actor byte against `0xfe` first, then `0xff` (`+0x1dcc`
and `+0x1e18`); the player-camera branch precedes the indexed-actor
branch. The `0xff` path begins only after the impact-emission block at
`+0x1f30` and rejoins that block on a nonzero proximity result. The C
branches and labels now preserve that order and the same three outcomes.
Isolated strict text rises to 89.237080%; RODATA is 15.953307% after
layout movement, and candidate text is 13,828 bytes. Its `.rel.text` has
469 rows versus 470 retail, while all 128 pointer rows/classes/order and
272 named referents remain exact.

Kind 106 tests phase zero and one before entering its phase-zero body at
retail `+0x20f8..+0x2110`; the prior C put the phase-one test after that
body. The explicit phase dispatch restores the retail branch sequence and
default exit. Isolated strict text reaches 89.479620%; RODATA is
15.758755%, candidate text is 13,836 bytes, and `.rel.text` returns to
470/470. All 128 pointer rows/classes/order and 272 named referents
remain exact.

Kind 8's second collision probe clears direction Z and X when its low
nibble is zero. Retail stores to record `+0x38` and `+0x34` at body
`+0x23d4..+0x23d8`; the prior C cleared Y and X. The corrected C emits
the two exact halfword offsets. Strict text remains 89.479620% until the
reset block's physical placement is recovered, and the exact pointer/
referent controls are unchanged.

The zero-nibble kind-8 reset block sits after the parent effect handling
and immediately before the shared position copy in retail. A branch to
that later block restores the `beqz` form after the second collision call
and the paired X/Z stores before the copy. Isolated strict text is
89.480194%, RODATA rises to 32.782100%, candidate text is 13,832 bytes,
and `.rel.text` is 469/470; the 128 pointer rows/classes/order and 272
named referents remain exact.

Kind 10's phase switch dispatches 1, 0, and 5 before its phase-zero body.
Retail checks phase one first, then branches on `< 2` to distinguish phase
zero, and checks phase five on the other side (`+0x262c..+0x2664`). The
phase-five reset sits after the phase-zero child spawn and before the
ordinary phase-zero continuation; the phase-one helper call sits after
both. The C now uses those explicit branches, so each path retains its
original behavior and physical order. Focused compilation and isolated
strict comparison give 90.758320% text, 15.953307% RODATA, candidate
text 13,828 bytes, and `.rel.text` 470/470. All 128 pointer rows/classes/
order and 272 named referents remain exact; the unit is still WIP.

The kind-10 countdown path calls `func_800401b4` before reading the
record's tail actor index `+0x40` and resolving `actor_state`. Keeping the
pointer assignment after that call restores the retail load order around
`+0x274c..+0x2774`. Isolated strict text rises to 91.067740%; RODATA
stays 15.953307%, candidate text remains 13,828 bytes, `.rel.text` is
470/470, and all 128 pointer rows/classes/order and 272 named referents
remain exact.

Kind 6 creates exactly eight children and writes render ID `0x18` to the
last child only after the loop. Retail has the loop branch at `+0x2990`
and the final `sb` at `+0x2998`, with the child pointer still live. Moving
that write out of the C loop restores the same topology. Isolated strict
text reaches 91.069176% and RODATA 33.463036%; candidate text is 13,812
bytes, `.rel.text` remains 470/470, and the pointer/referent controls
remain exact.

Within kind 6's child loop, retail loads `effect_state.current_index`
before storing the child's own index at `+0x41`, then stores the loaded
parent index at child `+0x40` (`+0x2978..+0x2988`). The C now snapshots
that parent index before both child stores. This restores the raw load/
store order; isolated strict text is 91.058266% after local code layout
movement, RODATA remains 33.463036%, and exact pointer/referent controls
are unchanged.

Kind 6 phase one writes its incremented frame byte at record `+0x44`
before comparing it with 24, then overwrites that byte with zero only on
wrap (`+0x2ac8..+0x2aec`). The prior conditional assignment delayed the
first write until after the comparison. The two-step C update restores the
retail store/`andi`/branch sequence. Isolated strict text reaches
91.274680% and RODATA 35.019455%; candidate text is 13,808 bytes,
`.rel.text` is 469/470, with exact pointer classes and named referents.

The phase-two frame update uses the same store-before-wrap sequence at
retail `+0x2ba0..+0x2bc4`. Expressing that sequence in C restores the
store, `andi`, unsigned limit check, and conditional zero store before
`func_8003c10c`. Isolated strict text reaches 91.502300%; RODATA is
33.463036%, candidate text is 13,800 bytes, and `.rel.text` is 468/470.
All 128 pointer rows/classes/order and 272 named referents remain exact.

In kind 6 phase two, retail saves the actor's unsigned halfword at `+0x1c`
before `func_8003c10c` (`+0x2ba4`). That saved extent supplies the first
`func_8001584c` call, the rotation divisor, and the radius calculation
(`+0x2c28`, `+0x2c74`, `+0x2ca0`). By contrast, the Y adjustment and second
interpolation reload actor `+0x1e` after the position call (`+0x2bec`,
`+0x2c44`). The C now keeps these fields separate and retains the `+0x1c`
snapshot across the call. Isolated strict text is 91.643230%, RODATA
33.463036%, candidate text 13,796 bytes, and `.rel.text` 468/470. All 128
ordered pointer rows/classes and 272 named referents remain exact.

The phase-two count branches at `+0x2c04..+0x2c24` exit immediately after
setting phase four when the count reaches 60. When it is below 17, the two
scale calls fall directly into the radius update; counts 17–59 join that
update without a second count check. The C now follows those three paths.
The focused build compiles with 446/451 CFG blocks, 217/217 branches, and
one return on each side. Isolated strict text reaches 91.877720%, RODATA
32.782100%, candidate text 13,780 bytes, `.rel.text` 468/470, and the
ordered pointer/referent controls remain exact. Signed member and array-view
probes for rotation `pad` did not alter the unsigned load plus explicit
sign-extension residue and were discarded.

The saved `+0x1c` actor halfword is promoted to an `s32` local after its
unsigned load, so its full zero-extended value survives across the scale
calls into the division and radius arithmetic. The probe no longer inserts
an extra `andi 0xffff` before division. Isolated strict text is 91.962685%,
RODATA 13.229572% after body-offset shifts, candidate text 13,776 bytes,
and `.rel.text` 468/470. The focused CFG/branch counts and exact ordered
pointer/referent controls are unchanged; the different radius shift sequence
remains unattributed codegen residue.

In the ballistic kinds 29/31/48 and 30/47, retail's collision-cache layer
selection at body `+0x1cc..+0x1e4` starts with layer two, branches over a
layer-one override when the cache halfword is nonzero, then writes the result
to record `+0x0a`. An explicit local and zero test recover the exact retail
`bnez` polarity and delay-slot constant without changing the two outcomes.
The focused build has 446/451 CFG blocks and 217/217 branches; isolated
strict text is 91.976750%, RODATA 13.229572%, candidate text 13,776 bytes,
and `.rel.text` 468/470. All 128 ordered table pointer classes and 272
named referents remain exact.

Kind 114's two-emission path counts from one down through zero in retail:
the less-than-cache-result branch initializes the loop register to one at
`+0x3378`, decrements it in the `rand` delay slot at `+0x33ac`, and exits
after the second `func_80041e94` when it equals minus one at `+0x33f4`.
The C now uses that countdown loop. The probe recovers the loop decrement,
minus-one sentinel check, and branch sequence. Focused CFG remains 446/451
blocks with 217/217 branches and one return each. Isolated strict text is
92.104770%, RODATA 13.229572%, candidate text 13,776 bytes, and `.rel.text`
468/470; all 128 ordered table pointer classes and 272 named referents
remain exact.

The kind-114 collision call passes `10` in `$a3`, then stores `10` and
`176` in the two O32 stack argument slots at body `+0x331c..+0x333c`.
The prior source reversed those stack values. It now spells the raw call
`func_8002b9d4(x, y, z, 10, 10, 176)`, correcting a behavior-affecting
argument mismatch. Focused compilation and exact pointer/referent controls
hold; isolated strict text is 92.105340% with the same 13,776-byte text,
13.229572% RODATA, and 468/470 `.rel.text` rows.

The two missing text relocation rows are internal `R_MIPS_26` jumps:
retail has 128 and the probe has 126. Local HI16 and LO16 counts both
match two apiece, while the 272 named call/data referents remain exact.

The kind-38/39 collision response resolves one of those jumps and a
behavioral error. With cache flag `0x10` set, retail tail byte `+0x40`
zero calls `func_8003feb0` then jumps past the `+0x40 = 1` store; a
nonzero byte takes the branch to that store (`+0xb54..+0xb88`). The prior
C wrote one on both paths. The corrected branch preserves the raw call and
store conditions. Focused compilation has 446/452 CFG blocks, 217/217
branches, and one return each. Isolated strict text is 92.334100%, RODATA
21.887160%, candidate text 13,784 bytes, and `.rel.text` 469/470: local
`R_MIPS_26` jumps are 127 candidate versus 128 retail. All 128 ordered
pointer rows/classes and 272 named referents remain exact.

In kinds 103/121, the spawned branch calls `effect_play_spatial_sound`
at body `+0x1294` and jumps directly to the four-child loop at `+0x12d0`
(`j` at `+0x129c`, loop count three in its delay slot). The prior C fell
through the nonspawn phase-zero tail update, which could decrement tail
`+0x40` and change phase on a spawn. An explicit jump now preserves the
retail paths. Focused compilation has 446/452 CFG blocks, 217/217
branches, and one return each. Isolated strict text is 92.384610%,
RODATA 27.529182%, candidate text 13,792 bytes, and `.rel.text` 470/470.
All 128 ordered table pointer classes and 272 named referents remain exact.

On the nonspawn phase-zero path, retail loads tail halfword `+0x40` as
unsigned, calculates its decrement, shifts the original value for a signed
test, and stores the decrement in the `bgtz` delay slot before an optional
phase-one store (`+0x12ac..+0x12c8`). The C now snapshots that unsigned
halfword, writes `count - 1`, and tests the saved count as signed. The
compiled sequence matches this local retail order. Focused CFG improves to
446/451 blocks with 217/217 branches; isolated strict text is 92.604190%,
RODATA 28.015564%, candidate text 13,788 bytes, and `.rel.text` 470/470.
The 128 ordered table pointer classes and 272 named referents remain exact.

The two kind-103/121 collision-bound paths write their selected lower Y
bound to stack `+0x5c`; later stores fill X and Z at `+0x58` and `+0x60`
before passing that VECTOR to both constructors. The C now stores the bound
in `spawn_position.vy` at selection time instead of carrying an independent
scalar and assigning the vector member just before the calls. Focused CFG
and branches stay 446/451 and 217/217. Isolated strict text rises to
92.663315%, RODATA to 32.101166%, candidate text is 13,796 bytes,
and `.rel.text` is 470/470; table and referent controls remain exact.

Retail's two lower-bound selections each branch and write exactly one of
the two cache words into `spawn_position.vy` (`+0x1158..+0x1170` and
`+0x11d8..+0x11f0`). Explicit if/else stores in C recover those local
`beqz`, `j` with store delay slot, and alternate store sequences. Focused
CFG is 446/453 with 217/217 branches and one return each. Isolated strict
text rises to 92.750000%; RODATA is 16.634241% after body-offset shifts,
candidate text 13,812 bytes, and `.rel.text` 472/470. The two extra local
`R_MIPS_26` rows are a remaining cross-body shape difference; all 128
ordered table classes and 272 named referents are still exact.
Only 9 of the 128 pointer addends are byte-exact at this intermediate body
layout; the remaining values point to shifted case bodies while preserving
the 64 target-equivalence classes and their order.

Kind 6 phase zero falls through after creating eight children: retail sets
phase one at body `+0x29a0` and immediately reads `updates_remaining` for
the phase-one update, with no intervening jump. The previous C `break`
delayed that update until the next dispatch and introduced an extra local
`R_MIPS_26`. The raw-supported fallthrough now compiles with 446/450 CFG
blocks, 217/217 branches, and one return each. Isolated strict text is
92.823770%, RODATA 35.116730%, candidate text 13,812 bytes, and
`.rel.text` 471/470 (129 versus 128 local jumps). All 128 ordered pointer
rows/classes and 272 named referents remain exact.

Kind 5 phase zero has one shared default count path. Retail branches on
actor tail byte `0xff` to count 16, step zero, and one update; after
`asset_vertex_count`, a zero count branches backward to that same default
block (`+0x190c..+0x195c`). Nonzero counts below 33 use step `0x1000`,
and larger counts divide the shifted count before capping it at 32. The
C now spells that shared path and its actor branch explicitly. Focused
compilation remains 446/450 CFG blocks and 217/217 branches. Isolated
strict text reaches 93.141500%; RODATA remains 35.116730%, candidate
text 13,812 bytes, and `.rel.text` 471/470. The 128 pointer rows/classes/
order and 272 named referents remain exact.

Kind 6 phase one records the collision actor index at `record+0x46`, then
reloads that byte before indexing `actor_state` (retail body `+0x2a44`
and `+0x2a48`). Indexing through the stored record member in C restores
that `sb`/`lbu` value chain. Focused compilation reports 446/450 CFG
blocks and 217/217 branches. Isolated strict text reaches 93.246840%;
RODATA is 33.754864%, candidate text 13,820 bytes, and `.rel.text`
471/470. All 128 ordered table pointer classes and 272 named referents
remain exact.

On the kind 6 actor-capture path, retail stores the negative actor-facing
angle in record halfword `+0x2a`, the same `rotation.pad` field read by
phase two. The previous C wrote `rotation.vy` at `+0x26`; correcting the
field makes the local store target exact. Focused CFG remains 446/450
with 217/217 branches. Isolated strict text is 93.247130%, RODATA
33.754864%, text 13,820 bytes, and `.rel.text` 471/470. All ordered
table-pointer classes and named referents remain exact.

The kind 6 low-count branch sets phase three, remaining updates `-1`,
and frame count 24 at body `+0x29b8..+0x29d0`. A failed collision-cache
flag check branches back to `+0x29bc`, and a captured actor of the wrong
target type branches to `+0x29b8`; these paths share the same transition.
The C now spells that common exit. Focused CFG is 446/448 blocks with
217/217 branches. Isolated strict text reaches 93.606480%, RODATA
35.116730%, text 13,808 bytes, and `.rel.text` 470/470. All 128
ordered pointer classes and 272 named referents remain exact.

## Kind-102 audio parameter identity

The two kind-102 signed-low loads use `lui 0x801a` followed by `lh`
with negative immediates. Carry-adjusted resolution gives GAME
`0x801983cc` and `0x801983ea`, not addresses in the `0x801a83xx`
range. Both lie inside the owned `audio_state` BSS object, at offsets
`+0xd9c` and `+0xdba`. Its sound parameter array starts at `+0x464`,
and each `KfAudioVoiceParams` record is ten bytes: those loads are the
signed `vab_slot_index` halfwords of entries 236 and 239. Existing audio
callers index this array with a `u8` sound ID, and resource startup copies
loaded words into its first entry. A complete 256-record array ends at
`+0xe64`, exactly where the independently checked VAB stream slots begin.
The shared `audio.h` view now models that full extent instead of a ten-entry
prefix and opaque tail. The original source spelling and meaning of these
two sound IDs remain unknown; no separate global was created.

Focused rebuilds after the layout correction retain all 15 exact
`game.audio_runtime` functions, all five exact transition-phase callbacks,
and the prior WIP strict verdicts for the two audio-runtime, two startup,
and one transition-step functions. No address constructor or body was
changed to protect a score.

## Constructor kind-102 and stack-object follow-up

The constructor now has a distinct kind-102 arm after kind 101, following
the retail switch target order. Its two signed audio halfword reads use the
typed parameter entries above. The separate `0x8009a5a8` cooldown word has
exactly two surveyed direct raw references: the constructor loads it at
`0x80041240`, compares it as signed against `cd_state.frame_count`, then
stores the frame count plus 30 at `0x80041298`. Seven relocations were
reviewed, with no overlapping direct access found in the surveyed map. Its
defining source mechanism remains candidate because the address sits in a
mixed Sony/CD BSS neighborhood. The source keeps an address-derived identity
rather than claiming a new audio-state member.

For kind 114, retail stores random X and Z coordinates at stack offsets
`+24` and `+32`, leaving `+28` between them. This supports one 16-byte
`VECTOR` local, now used in source instead of scalar X/Z locals. Focused
similarity rises from 35.3% to 37.8%, and direct strict text from 82.501175%
to 83.979576% (5,092 retail bytes versus 5,072 compiled). Both bodies now
use 72-byte frames, 116 CFG blocks, 22 branches, and 69 direct calls. The
text and switch table have 157 and 123 relocation rows respectively; all
123 switch entries preserve the retail pairwise target equivalence across 62
distinct classes. The constructor remains WIP because instruction order and
other source-shape differences persist; exact closure is not claimed.

A later source-order pass put all 62 constructor switch target classes in the
same sorted physical order as retail. It moved the self-contained case groups
51/52, 53/33, 121/103, 122/104, 54/11, 16/14/19, 48/47/30/29/31, and
117/34/35; the kind-4 raw path also proved and restored a missing
`unknown_3c[4] = 0` store. Focused similarity is now 39.6%, and direct strict
text is 88.789474% (5,076 candidate versus 5,092 retail bytes), up from
83.979576%. It remains WIP.

An off-tree KF1-style `s32 *` varargs cursor at `&direction` placed the probe
cursor at retail stack offset `+88`, but strict text fell to 88.48154% and
the candidate grew to 5,100 bytes. Moving the cursor assignment after the
pool find fell to 88.296936% and 5,104 bytes. Both retained the wrong
saved-register allocation. These probes were discarded; the cursor shape is
not established by the stack address alone.

The constructor's switch now reads `record->kind` after assigning it. Raw
retail loads the discriminant with `lbu v1,1(record)` at that point, and the
focused compiler emits the same load. On the same existing target, this
source-backed change moves focused listing from 39.6% to 39.8% and RODATA
from 6.707317% to 28.760162%; isolated strict text moves from 70.829540%
to 70.502750%, with 116/116 CFG blocks and 22/22 branches unchanged.
The lower intermediate text score does not falsify the raw discriminant
evidence; the constructor remains WIP.

The next focused constructor pass restored the kind-4 byte store at record
offset `+8`, which retail emits before the clip and offset-`+9` stores at
`0x80040528..0x80040538`. Seventeen equal-value scale triples now store the
halfwords at offsets `+48`, `+46`, then `+44`, following the repeated raw
retail order. The isolated strict `.text` result is 88.933230% of 5,092
retail bytes, versus 88.840530% before these edits; `.rodata` rises from
14.532520% to 32.926830%. The constructor's 123 ordered jump-table
relocations still form the same 62 target-equivalence classes as retail,
with no pairwise class conflict; only seven body-offset addends currently
match exactly. Focused CFG remains 116/116 blocks and 22/22 branches.
An off-tree fifth-slot cursor probe reproduced `sp+88` but lowered the
focused listing from 39.8% to 39.6%; the stack address alone does not prove
the historical variadic source spelling, so that probe was discarded.

The constructor allocation-failure path now joins the final record return.
Retail branches from body `+0x3c` to `+0x13bc`, moves the null record into
`v0`, then enters the shared epilogue. The former early `return 0` sent the
probe straight to the epilogue with an immediate zero in the branch delay
slot. The source `goto finish` recovers the branch target, nop delay slot,
and 26/26 known return frontiers while preserving 116/116 CFG blocks and
22/22 branches. Isolated strict `.text` is 88.886090% and `.rodata` is
32.926830%; the small text decrease from the previous 88.933230% follows
body-offset shifts. All 123 table rows still form the same 62 target
classes in order, with seven exact addends. This remains a WIP.
