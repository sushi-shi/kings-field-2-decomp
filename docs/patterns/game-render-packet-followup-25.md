# GAME TMD and render packet follow-up

This 25-function pass follows the TMD object, prepared-packet, and map-cell
render call graph. The percentages below are from the last strict GAME report;
focused probes are called out separately. Exact neighbors are regression
controls, not evidence that a contiguous unit is the original source file.

| VA | Verdict | Evidence or open difference |
| --- | --- | --- |
| `0x8002d5dc` | WIP, 96.13260% | Typed packet-index preparation has the 17 retail CFG blocks; an extra entry move shifts switch targets by four bytes. |
| `0x8002d8b0` | Exact, 100% | TMD registration control. |
| `0x8002d8f0` | Exact, 100% | TMD slot setter control. |
| `0x8002d910` | Exact, 100% | TMD slot release control. |
| `0x8002d918` | Exact, 100% | Vertex projection control. |
| `0x8002da94` | Exact, 100% | Alternate vertex projection control. |
| `0x8002dbd8` | Exact, 100% | Vertex transform control. |
| `0x8002dc80` | Exact, 100% | Depth transform control. |
| `0x8002dd28` | Exact, 100% | Projected-vertex control. |
| `0x8002ddb4` | WIP, 97.34716% | FT packet walker uses the proved face offsets; dispatch and register schedule differ. |
| `0x8002e4dc` | WIP, 96.587975% | Normal-colored packet walker has the same family of dispatch/codegen residue. |
| `0x8002ebe0` | WIP, 95.27945% | Fixed-depth packet walker has the same family of dispatch/codegen residue. |
| `0x8002f194` | Exact, 100% | Map emitter and its initialized color datum remain a control. |
| `0x8002f5b0` | WIP, 95.833336% | Clipped-triangle fields and calls agree; saved-register assignment and one load/store schedule differ. |
| `0x8002f808` | WIP, 63.362473% | Alternate clipped TMD path has typed FT3/FT4 packet fields; retail uses a 168-byte frame versus the probe's 112 and different packet-control scheduling. No additional live local extent has been proven. |
| `0x80030c18` | WIP, 96.521736% | Focused listing differs at two independent prologue operations; four-unit map-cell check preserves exact controls. |
| `0x80030de4` | WIP in last strict report, 89.52128%; focused SAME | Typed two-layer map-cell helper needs a fresh strict report before exact attribution. |
| `0x80030f5c` | Exact, 100% | Render-grid scan control. |
| `0x80031024` | Exact, 100% | Render-model row control. |
| `0x800316c8` | Exact, 100% | Player-weapon render control. |
| `0x80031850` | WIP, 84.71045% | World-model packet path has the correct 40 CFG blocks and 16 branches; a typed map-cell view lowered focused listing similarity and remains a source-backed correction. |
| `0x80031d8c` | WIP, 94.65414% | Retail retains the sixth blend-mode argument in a saved register; the probe reloads its stack slot. |
| `0x800321d8` | WIP, 98.43590% | Retail forms arena address `0x8009b0a0` with `lui/addiu`, while the fixed C literal emits `lui/ori`; no owning symbol or extent is yet proven. |
| `0x80033994` | Exact, 100% | Menu-model rendering control. |
| `0x800345e4` | Exact, 100% | Asset vertex-count control. |

Both `0x8002f194` and `0x8002f808` now use the existing
`KfTmdPacketHeader` word/byte view: its mode byte selects FT3/FT4 cases and
its input-length byte advances the packet cursor in four-byte words. Focused
builds produced the same listings as the former shift/mask forms, including
the exact `0x8002f194` sibling. Compile-time checks fix the packet header at
four bytes, with input length at byte 1 and mode at byte 3. A temporary
application of that union to
the three large TMD packet walkers enlarged their stack frames and moved
branches; it was discarded. Swapping the FT3/FT4 source cases in the map
emitter similarly worsened its focused listing and was discarded. Neither
experiment identifies a source-level reason for the remaining differences.

The clipped-vertex color at result offset `+0x1c` is now a `CVECTOR` in the
shared record, matching the `DpqColor` output parameter and the typed GT3
packet color destination. The final four-byte packet copies remain explicit
word copies, as retail emits `lw`/`sw` there. This layout-identical change
retains the exact map emitter and both WIP focused listing verdicts.

## Display, panel, frame, and TIM transfer callers

A second 22-function call-chain pass checked the display/TMD setup reached by
the renderers, their screen-quad callers, the frame driver, and the archive
TIM transfer. All addresses below are in GAME. The two large prepared-object
and resource controllers remain unclaimed because their full object/workspace
ownership is still incomplete.

| VA | Verdict | Role or remaining boundary |
| --- | --- | --- |
| `0x8002d0a4` | Exact, 100% | Near-fog setter. |
| `0x8002d0d4` | Exact, 100% | Display initialization. |
| `0x8002d248` | Exact, 100% | Display reset. |
| `0x8002d32c` | Exact, 100% | Begin-frame primitive and OT setup. |
| `0x8002d3c4` | Exact, 100% | Present frame. |
| `0x8002d458` | Exact, 100% | TMD asset selection. |
| `0x8002d484` | Exact, 100% | TMD object lookup. |
| `0x8002d4a8` | Exact, 100% | Current vertex pointer setter. |
| `0x8002d4b8` | Exact, 100% | Object vertex selection. |
| `0x8002d4f4` | Exact, 100% | View matrix and position update. |
| `0x8002ff5c` | Unclaimed WIP | Prepared TMD builder has a 1248-byte frame and a 1024-byte, 128-entry `SVECTOR` midpoint workspace; its copied packet schema is not yet completely modeled. |
| `0x800311b0` | WIP, 92.14815% | Fifteen-argument `POLY_FT4` builder; retail saves one more register and schedules the packet-code store in a branch delay slot. |
| `0x800312f4` | Exact, 100% | First sliding textured panel. |
| `0x80031384` | Exact, 100% | Second sliding textured panel. |
| `0x80031414` | Exact, 100% | Color-byte screen quad draw. |
| `0x800314d4` | Exact, 100% | Color-byte screen quad setter. |
| `0x800314fc` | Exact, 100% | Collision overlay draw, owned by the collision family. Its signed halfword reads coexist with unsigned halfword accumulation at `0x80031634`. |
| `0x8003247c` | Unclaimed WIP | Frame resource/actor dispatcher visits 200 typed `KfActor` and 128 typed `KfMapPlacedEntry` records. Its first loop tests `actor.lifecycle == 1` at `+0x09`, then actor flag word `+0x28` against `0x2000` and `0x80000`. The first TMD range update consumes 128 flag bytes from `sp+88`; the second consumes 320 after a full clear at the same base. Its last loop skips `id == 0xffff`, checks `layer`, draws with `position`, and advances `frame_index` modulo `frame_count` when the frame counter is divisible by `frame_period`; the middle resource state lacks a full owner. |
| `0x800335a0` | Exact, 100% | Frame driver calls the panel, TMD, resource, and display helpers. |
| `0x8003494c` | Exact, 100% | TIM iterator uploads CLUT and pixel rectangles. |
| `0x800349bc` | WIP, 92.34296% | Four fade quads and pad polling agree; retail spills return state in a 72-byte frame, while the current C uses 64 bytes without a proved extra local. |
| `0x80034e10` | Exact, 100% | TIM/archive image transfer and VRAM snapshot. |

The graphics-runtime `+0x14cc6/+0x14cc8/+0x14cca` values are accumulated with
`lhu`/`sh` in `0x80031634` but read with signed `lh` for division in exact
`0x800314fc`. The current unsigned storage with signed read views reflects
both instruction families; a global signed-field change would overstate the
evidence.

The prepared-TMD builder's decoded entry selects source object `index * 0x1c`
and places the destination object at output `+0x0c`, with packets starting at
output `+0x28`. These object/payload offsets now have compile-time checks in
`KfTmdPreparedAsset`; the focused map-cell unit retains its three exact
siblings. The builder accepts both opaque and semitransparent FT4 modes
(`0x2c/0x2e`) and FT3 modes (`0x24/0x26`). Four-way FT4 expansion advances
the packet output by `0x80` bytes and appends five midpoint vertices; FT3
advances by `0x60` bytes and appends three. The sole caller invokes this
builder only when the source has fewer than 16 primitives, bounding its
midpoint list to at most 75 entries inside the 128-entry stack workspace.
Its `0xffff00ff` mask retains
the non-interpolated bits of packed UV words. This establishes
the packet and midpoint extents but not enough ordering/detail for a humane
complete source claim.
