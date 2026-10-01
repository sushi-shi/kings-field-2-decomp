# GAME render-frame packet chain

This 26-function pass follows the confirmed calls from the frame driver through
map cells, textured panels, animated models, resource flags, and the TIM image
transition. The percentages are from the last available strict report, which
predates some focused source changes. `SAME` below means a fresh focused listing
comparison only; it is not a new strict result. Collision, map, and audio bodies
are read-only controls where another worker owns the source.

| GAME VA | Verdict | Evidence or remaining difference |
| --- | --- | --- |
| `0x80030c18` | WIP; focused 98.7% | Retail and C both use a 4176-byte frame with the 4096-byte prepared TMD asset. Only an independent quarter-turn byte load and flags argument move swap order. The analogous KF1 map-cell renderer computes orientation before matrix setup; it does not establish a different KF2 local or type. |
| `0x80030de4` | Focused SAME; old strict 89.52128% | Two occupancy layers and view-relative coordinates match. Strict certification remains unavailable in this focused-only pass. |
| `0x80030f5c` | Exact, 100% | 24-by-24 render-grid scan control. |
| `0x80031024` | Exact, 100% | Typed model-row renderer control. |
| `0x800311b0` | WIP, 92.14815% | Fifteen-argument textured FT4 helper has correct calls and packet writes. Retail saves one more register and places a code-byte store in a branch delay slot. |
| `0x800312f4` | Exact, 100% | First sliding-panel caller. |
| `0x80031384` | Exact, 100% | Second sliding-panel caller. |
| `0x80031414` | Exact, 100% | Color-byte panel draw caller. |
| `0x800314d4` | Exact, 100% | Color-byte setter. |
| `0x800314fc` | Exact, 100% | Collision-overlay draw; collision-owned control. |
| `0x80031634` | Exact, 100% | Three-channel collision accumulator; collision-owned control. |
| `0x800316c8` | Exact, 100% | Player-weapon render caller. |
| `0x80031850` | WIP, 84.71045% in last strict report | World-model renderer has typed map-cell references and the confirmed TMD call family; current focused listing still differs in register allocation and path layout. Retail reads the stack halfword for `relative.vy` even when the world matrix is null; the caller at `0x80032e38` takes that path with render mode `0x14`. The source leaves that local uninitialized on this path as retail does, pending source provenance. |
| `0x80031d8c` | WIP, 94.65414% | Animated-object renderer has a 120-byte frame and the right calls. Retail hoists blend mode from the stack to `s7`; the probe reloads it and saves one fewer register. |
| `0x80032040` | Exact, 100% | Single-cell layer-mask lookup control. |
| `0x800320b0` | WIP, 73.95918% | Radius layer-mask scan remains in the shared resource runtime; map-owned source is read-only here. |
| `0x80032174` | WIP, 93.6% | View-cell rectangle check remains in the shared resource runtime; map-owned source is read-only here. |
| `0x800321d8` | WIP, 98.43590% | TMD archive queue uses a fixed arena address. Retail `lui/addiu` versus C literal `lui/ori` persists because arena binding and extent are not proved. |
| `0x80032274` | WIP, 90.933334% | VAB range update is audio-owned and read-only here. |
| `0x80032364` | Exact, 100% | TMD registry range-update control. |
| `0x8003247c` | Unclaimed WIP | Dispatcher reaches 200 typed actors, 128 placed-object records, TMD/VAB range updates, and model draws. The first flag region at `sp+88` is initially cleared for 128 bytes and passed to a 128-entry TMD update at registry index `0x80`; it is later cleared for 320 bytes and passed to a 320-entry update at index `0x100`. The second region at `sp+408` is 64 bytes in each VAB update. Middle resource ownership remains incomplete. |
| `0x800335a0` | Exact, 100% | Frame driver that calls the dispatcher and render paths. |
| `0x80033994` | Exact, 100% | Menu-model TMD setup control. |
| `0x8003494c` | Exact, 100% | TIM CLUT/pixel upload control. |
| `0x800349bc` | WIP, 92.34296% | Four textured fade quads and pad polling agree; retail spills return state in a 72-byte frame versus the probe's 64-byte frame without a proved extra local. |
| `0x80034e10` | Exact, 100% | Archive TIM/VRAM transition control. |

The clipped-result color at `EVECTOR.rgb` (`+0x1c`) is a four-byte `CVECTOR`
destination for `DpqColor`. Psy-Q 3.0's complete 44-byte `EVECTOR` also matches
every other consumed clipped-vertex offset, so it replaces the former partial
game-side view. Its GT3 packet transfer remains a word copy because retail
uses `lw`/`sw`. Focused `game.render_map` keeps `0x8002f194` SAME and the
existing `0x8002f5b0`/`0x8002f808` WIP listings unchanged after that type
refinement. No source change was retained for a register-order or frame-size
residue without independent ownership evidence.
A focused revisit of GAME `0x8002f808` rebuilt `game.render_map` against the
selected retail body and checked its callers, calls, data references, and
strings. The current listing has 51/51 CFG blocks, 35/35 branches, and the
retail call set, but remains 58.9% similar; the first differences include a
168-byte retail frame versus a 120-byte probe frame and the primitive-count
reload schedule. The clipping-window macros now add their unsigned bias after
casting the signed delta, giving defined 32-bit wrap for the retail `addiu`
checks. A focused rebuild emits a byte-identical listing for the whole unit,
including the exact `render_enqueue_map` sibling. The function remains WIP;
the frame and register differences have no supported source correction.
