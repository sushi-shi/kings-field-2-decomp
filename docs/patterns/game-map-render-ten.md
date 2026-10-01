# GAME map-object and render ten-function verdict

These ten functions were checked against their GAME.EXE disassembly, CFG,
incoming and outgoing references, strings, and focused pinned-probe listings.
The sole retained source change is the typed row calculation in
`map_object_set_cell_marker`; an isolated one-unit objdiff reports its
0xf4-byte function and all five siblings at strict 100%.

| Address | Function | Fresh focused verdict | Remaining boundary |
| --- | --- | --- | --- |
| `0x80030c18` | `render_map_cell_object` | WIP, 98.7%; 11/11 CFG blocks, 6/6 branches | Independent orientation load, flag move, and view-matrix address setup differ before the first SDK call. Three siblings remain SAME. |
| `0x80031850` | `func_80031850` | WIP, 94.5%; 40/40 blocks, 16/16 branches | Saved-register assignment for scale, clip, and graphics-state base; one later argument-setup schedule. |
| `0x80031d8c` | `func_80031d8c` | WIP, 79.5%; 7/7 blocks, 2/2 branches | Retail saves the late blend-mode argument in `s7`; the probe reloads it from the caller stack and has a different frame. |
| `0x8003247c` | `func_8003247c` | WIP, 71.0%; 96/96 blocks, 54/54 branches | Retail's stack frame is 768 bytes versus 760 probe bytes. Its free-effect branch jumps to the effect-increment block; the focused B62 successor index differs around the special-draw layout, without proving a semantic edge difference. The frame cause remains unidentified. |
| `0x80034f90` | `func_80034f90` | WIP, 92.5%; 14/14 blocks, 7/7 branches | The same row-major cell address and field writes compile with different input-register and independent-addition schedules. |
| `0x80035194` | `func_80035194` | WIP, 58.8%; 51/51 blocks, 26/26 branches | Source and retail select the same rectangle-copy paths; stack-argument and saved-register allocation differ at entry. |
| `0x800356ac` | `map_object_set_cell_marker` | **Strict exact, 100%**; all six functions in its unit have focused SAME listings | Two typed row pointers reproduce retail's Z-row calculation before X-cell addition in both marker branches. |
| `0x80036190` | `func_80036190` | WIP, 99.3%; 15/15 blocks, 8/8 branches | The only focused difference is the order of an angle-call result move and an independent stack-argument load. Its two adjacent helpers remain SAME. |
| `0x80036464` | `map_object_spawn_effect` | WIP, 92.8%; 12/12 blocks, 3/3 branches | Object ID and height offset occupy opposite saved registers; the preceding function's four-byte size residue shifts switch-table addends. |
| `0x800475d8` | `func_800475d8` | WIP, 89.4%; 55/54 blocks, 29/29 branches | Template-pointer saved register and a return-path join differ; variadic spawned ID and pose/call paths remain source-backed. |

The first pattern-placement function was also compiled with an explicit typed
row pointer. Its focused listing fell from 92.5% to 91.7% without changing
the 14-block CFG, so that probe was reverted. The other eight WIP sources
were left unchanged: the remaining symptoms are not evidence for padding,
forced register use, or altered semantic fields. Verification used focused
`kf try` builds and an isolated strict comparison of the newly exact reset
unit. No repository tests, lint, full build, or broad match was run.

For `0x8003247c`, a pinned isolated candidate decode resolves the focused
CFG clue at effect-loop B62: the compiled B78 at body `+0x96c` is the same
`addiu s0,s0,72` effect increment as retail B79 at `+0x9c4`. Their block
ordinals differ because the special-draw arm is laid out differently. This
does not establish a different free-effect branch condition or a missing
source operation. Retail's actor-position scratch starts at `sp+72`, and its
two 320-byte flag arrays at `sp+88` and `sp+408`; the probe places them at
`sp+64`, `sp+80`, and `sp+400`. The rotation scratch stays at `sp+56`, and
saved registers likewise move eight bytes with the frame. No raw reference
identifies a live object in that eight-byte gap, so the 768/760-byte frame
cause remains open.

## Graphics follow-up (2026-10-02)

Fresh isolated strict objects, retail disassembly, xrefs, and focused listings
confirmed five disjoint graphics functions. `0x80030c18` remains 96.521736%
strict/98.7% focused; its only focused residue is the order of an independent
orientation load and saved flag move. Its three sibling functions and all
540 initialized data bytes remain exact. `0x800311b0` remains 92.148150%
strict/57.5% focused: the pinned Psy-Q `setPolyFT4` store is sound, but retail
puts the packet-code byte store in a semitransparency branch delay slot and
saves another register. `0x80031d8c` remains 94.654140% strict/79.5%
focused; retail retains the late blend-mode argument in `s7`, whereas the
candidate reloads it from the caller stack. No source edit was supported for
these three. `0x80031850` world-model rendering remains 99.298510% strict
and 94.5% focused: retail assigns scale, lighting blend, and the graphics
base to different saved registers, then orders an independent light-matrix
address setup differently; its calls and referents agree. No source edit was
supported there.

Retail `0x8003247c` constructs the camera-position pointer at
`player_state+0xd8` before the map-object loop and retains it in `s8` for
both sound-position reads. The source now establishes the pointer before that
loop instead of inside the sound-action arm. This source-level lifetime is
supported by the raw reads at `0x80032908` and `0x80032930`. The pinned probe
still folds the pointer into two direct global loads, so exact closure remains
open; focused listing improved from 71.0% to 72.6% and isolated strict from
92.0027% to 92.18579%. The 768/760-byte frame delta and effect-loop codegen
order remain. Retail has two ordered `player_state` HI16/LO16 pairs (one
camera base and one vertical component); the candidate still has three
direct-load pairs. The render identity matrix and other initialized data stay
strict exact. Only focused builds and isolated strict comparisons were used;
no tests, lint, full build, broad match, or README edit was run.

The adjacent `tmd_pipeline.c` packet walkers were also rebuilt as one isolated
unit. Its eight preceding helpers remain strict exact. The three WIPs are:

| Function | Strict | Focused | Remaining difference |
| --- | ---: | ---: | --- |
| `0x8002ddb4` | 97.347160% | 93.6% | Retail retains two separate FT3/GT3 positive-depth branches before the shared ordering-table tail; the candidate folds them into that tail. |
| `0x8002e4dc` | 97.293980% | 93.5% | The paired textured walker has the same two-branch depth-layout gap. |
| `0x8002ebe0` | 95.279450% | 88.7% | Retail evaluates signed fixed depth at the packet tail; the candidate precomputes an equivalent range flag at entry and uses a larger frame. |

Moving the first walker's depth temporary into each packet-mode case emitted
an identical focused listing and was reverted. KF1's analogous packet walker
uses related GTE color calls but a different ordering-table depth rule, so it
does not establish an alternate KF2 source spelling.
