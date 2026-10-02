# GAME menu UI focused verdicts

This related batch follows the menu selection, glyph, input, status, and sprite
families through their shared calls and data. Each row was rebuilt with a
focused `kf try --unit ... --context 0 --no-flow` probe. `SAME` means an
identical focused listing; strict `kf match` closure was not run in this pass.
The source models and curated retail evidence, rather than a fuzzy score,
remain the criteria for retaining changes.

| GAME address | Function | Focused verdict |
| --- | --- | --- |
| `0x8001876c` | `func_8001876c` | DIFF 90.8%; result-stack and return-path instruction residue |
| `0x800189f0` | `func_800189f0` | SAME |
| `0x80018ac8` | `func_80018ac8` | SAME |
| `0x80018d08` | `func_80018d08` | SAME |
| `0x80018dec` | `func_80018dec` | SAME |
| `0x80018f8c` | `func_80018f8c` | SAME |
| `0x80019240` | `func_80019240` | SAME |
| `0x800192ac` | `func_800192ac` | SAME |
| `0x800192dc` | `func_800192dc` | SAME |
| `0x80019834` | `func_80019834` | SAME |
| `0x800199d0` | `func_800199d0` | SAME |
| `0x80019ac4` | `func_80019ac4` | SAME |
| `0x80019ce4` | `func_80019ce4` | SAME |
| `0x80019ed4` | `func_80019ed4` | SAME |
| `0x8001a4f0` | `func_8001a4f0` | DIFF 96.0%; initializer-loop register choice only |
| `0x8001a898` | `func_8001a898` | SAME |
| `0x8001c550` | `func_8001c550` | SAME |
| `0x8001c62c` | `func_8001c62c` | SAME |
| `0x8001c770` | `func_8001c770` | SAME |
| `0x8001c8b0` | `func_8001c8b0` | SAME |
| `0x8001c9f4` | `func_8001c9f4` | SAME |
| `0x8001cad4` | `func_8001cad4` | SAME |
| `0x8001cb44` | `func_8001cb44` | SAME |
| `0x8001ccd4` | `func_8001ccd4` | SAME |
| `0x8001cdb0` | `func_8001cdb0` | SAME |
| `0x8001ceb8` | `func_8001ceb8` | SAME |
| `0x8001d030` | `func_8001d030` | SAME; 100% isolated direct objdiff |
| `0x8001d340` | `func_8001d340` | SAME |
| `0x8001d3b4` | `func_8001d3b4` | SAME |
| `0x8001d654` | `func_8001d654` | SAME |
| `0x8001d6a8` | `func_8001d6a8` | SAME |
| `0x8001d8d0` | `func_8001d8d0` | SAME |
| `0x8001dc64` | `func_8001dc64` | SAME |
| `0x8001ddd0` | `func_8001ddd0` | SAME; 100% isolated direct objdiff |
| `0x8001e0a8` | `func_8001e0a8` | SAME; 100% isolated direct objdiff |
| `0x8001e378` | `func_8001e378` | SAME |
| `0x8001e484` | `func_8001e484` | SAME |
| `0x8001e94c` | `func_8001e94c` | SAME |
| `0x8001f008` | `func_8001f008` | SAME |
| `0x8001f798` | `func_8001f798` | SAME |
| `0x8001f8b8` | `func_8001f8b8` | DIFF 84.8%; loop exit and input-wait block placement |
| `0x8001fb8c` | `menu_draw_window` | DIFF 83.5%; target 48-byte versus probe 40-byte frame; other instructions align |
| `0x80020b50` | `menu_blit_sprite_translucent` | SAME |
| `0x80020d20` | `menu_blit_sprite` | SAME |
| `0x80020ef8` | `menu_blit_sprite_fixed_clut` | SAME |
| `0x800210ac` | `menu_draw_string` | DIFF 90.4%; target 56-byte versus probe 48-byte frame and glyph UV register choice |
| `0x80021510` | `menu_draw_number` | SAME; 100% isolated direct objdiff |
| `0x800217f0` | `func_800217f0` | SAME |
| `0x80021a60` | `func_80021a60` | SAME |
| `0x80021a68` | `menu_frame_begin` | SAME |
| `0x80021be0` | `menu_present_frame` | SAME |
| `0x80023570` | `player_restore_equipment_effects` | SAME |
| `0x8002360c` | `func_8002360c` | SAME |
| `0x80023814` | `func_80023814` | SAME |
| `0x80023868` | `player_add_equipment_bonuses` | SAME |
| `0x80023984` | `player_recalculate_combat_stats` | SAME |
| `0x80024034` | `player_increment_physical_power_training` | SAME |
| `0x800240cc` | `player_increment_magic_training` | SAME |
| `0x80024164` | `player_add_experience` | SAME |
| `0x80024384` | `player_calculate_damage_component` | SAME |
| `0x80024448` | `player_adjust_hp_unclamped` | SAME |
| `0x800311b0` | `func_800311b0` | DIFF 57.5% focused; packet-code store and saved-register placement |
| `0x800312f4` | `func_800312f4` | SAME |
| `0x80031384` | `func_80031384` | SAME |
| `0x80031414` | `func_80031414` | SAME |
| `0x800314d4` | `func_800314d4` | SAME |
| `0x800314fc` | `func_800314fc` | SAME |
| `0x80032fec` | `notification_draw_quad` | SAME |
| `0x80033140` | `notification_draw` | SAME |
| `0x800331d0` | `notify_enqueue` | SAME |
| `0x80033274` | `notification_digit_set_v` | SAME |
| `0x80033284` | `func_80033284` | SAME |
| `0x800335a0` | `func_800335a0` | SAME |
| `0x80033994` | `func_80033994` | SAME |
| `0x800473e0` | `func_800473e0` | SAME |
| `0x80047434` | `func_80047434` | SAME |
| `0x800474c4` | `func_800474c4` | SAME |
| `0x800475d8` | `func_800475d8` | DIFF 89.4%; effect-campaign source correction retained, still WIP |

A fresh 30-function focused menu, frame, and notification sweep after the
`menu_sprite_defs` inventory identity sync gave 27 SAME listings and the three
retained WIP results above: `menu_draw_window` 83.5%, `menu_draw_string`
90.4%, and `menu_draw_number` 93.1%. This is a listing probe, not strict
closure. The notification quad's two rectangle groups were also trialed with
the pinned Psy-Q `setXYWH` and `setUVWH` macros; the exact drawing helper
fell to 74.0% and both substitutions were reverted. The retail store order
supports the existing chained assignments, not those macro spellings.

The later 21-function label/item/input batch gave 18 SAME listings and the
three purchase-controller DIFF results shown above. Retail's 20-byte label
template copies use unaligned word load/store pairs. Replacing nineteen
casted struct assignments across eight label builders with 20-byte `memcpy`
from the template code arrays removes incompatible typed-pointer access; all
ten functions in the shared label unit remain focused SAME. Direct objdiff
also reports 100.0% for all ten label-unit function symbols, with retail
body sizes of 220, 324, 320, 324, 224, 112, 400, 220, 264, and 376 bytes
in address order; the whole unit's `.text` is 2,784/2,784 bytes and its
`.data` is 320/320 bytes, both at 100%. The three
purchase controllers have the same price/quantity load and temporary-register
residue, with their call sets and known CFG shapes already accounted for.

A subsequent 26-function graphics and notification call-family sweep gave
24 focused SAME listings and two retained DIFF results. The four direct
textured-quad callers and its color setter stayed SAME, as did the five
notification functions, render-frame caller, ten player-core functions, and
three event-counter functions. The textured-quad helper's packet fields,
referents, call set, and 8/8 CFG blocks, 5/5 branches, and 1/1 returns agree
with retail. The focused listing is DIFF 57.5%, while the earlier strict report
recorded 92.14815%;
these metrics use different comparisons. Replacing conditional
`setSemiTrans` with the equivalent direct `quad->code = 0x2e` in a source-only
probe left the focused listing unchanged, so it was discarded. The pinned
Psy-Q 3.0 `setPolyFT4` macro expands to length 9 and code `0x2c` stores;
`setSemiTrans` ORs `0x02` into the code, giving the conditional `0x2e` store
seen in retail. Splitting `setPolyFT4` into separate authentic `setlen` and
`setcode` calls also left the listing unchanged. Retail first saves an extra `s2` and
moves the coordinate arguments into saved registers earlier; later it places
the base-code store in a branch delay slot. Those differences have no
supported source correction at present. The event
map-object controller is now focused DIFF 89.4% after its separate
effect-campaign source correction and is owned by that campaign; no source
edit was made in the textured-quad family.

For `menu_draw_number`, hoisting the second atlas-column U value into a
typed byte local reduced the focused listing to 86.0%; reversing its
commutative addition left the 93.1% listing unchanged. Both trial expressions
were reverted. Its remaining two U/width load-order differences and one
prologue move position lack a source-level correction from current evidence.
A later `s32` second-column-U local likewise hoisted the U value and reduced
the focused result to 86.2%; that probe was reverted too.

A later focused 28-function sweep of the glyph-row, input, status,
notification, and transition family gave 25 SAME listings and three retained
DIFF results: `0x8001876c` 90.8%, `0x8001930c` 80.2%, and `0x800349bc`
43.2%. The two additional exact controls in that sweep were
`menu_magic_list` `0x8001a2f4` and `menu_simple_loop` `0x8001a7fc`;
`tim_upload_images` and `func_80034e10` stayed SAME in the transition unit.
No new C source change was retained from this sweep. These are focused
listings, not strict closure.
The adjacent menu-model render helper `0x80033994` also rebuilt SAME in a
separate focused probe; its calls select the menu item TMD and draw object
zero through the shared model path.
The [inline-helper probe ledger](kf2-game-inline-helper-probes.md) records
the controlled macro and out-of-line spelling comparisons for the exact
status, attribute, and notification functions and the WIP glyph renderer.

The early item selector `0x80018ac8` and equipment selector `0x8001a898`
remain SAME after the shared list-input prototype cleanup. The mixed
item/magic controller `0x8001a4f0` remains DIFF 96.0%: its 22 CFG blocks and
12 branches agree, and the only displayed differences are register choices
for the fixed-count initializer's index and constants. A natural `do/while`
spelling emitted the same 96.0% listing as the retained `for` loop, so the
source stayed with the simpler form.

The separate archive-image preview at `0x8001930c` remains DIFF 80.2% in a
normal focused `kf try`. SDK `setXYWH` and `setUVWH` macros express the two
rectangles without changing the listing. Its calls, referents, 47 CFG blocks,
and 27 branches align; the remaining observable differences start with a
64-byte versus 56-byte frame and the archive-entry arithmetic schedule.
The arithmetic expression has been restored after a left-associative probe
failed to improve the instruction order. A separate archive-base local
lowered the focused listing to 79.6% by changing the initial registers;
that expression was also reverted. No compiler mechanism is inferred from
these residues.

The related fade transition at `0x800349bc` draws four `POLY_FT4` rectangles.
The pinned Psy-Q 3.0 `setXYWH` and `setUVWH` macros replace eight equivalent
field-write groups in its source. A current focused probe retains DIFF 44.8%,
with 14/14 CFG blocks and 8/8 branches; adjacent `tim_upload_images`
(`0x8003494c`) and `func_80034e10` remain SAME. Retail spills the fade state
on a 72-byte frame, while the current 64-byte probe keeps it in a saved
register. The macro substitution changes source expression without claiming
an exact-match gain.
GCC 2.6.0 raises the fade body's focused similarity to 75.9% with either
`-mcpu=r2000` or `-mcpu=r3000`, but changes both exact sibling listings;
GCC 2.5.7 without its current CPU flag also changes the exact `0x34e10`
sibling. No profile change is supported for this contiguous unit.

## Connected item, list, and display pass

A focused GAME pass followed confirmed menu-list calls and shared sprite,
primitive-buffer, and display state through 25 functions from `0x8001d030`
to `0x80021fb0`. Fifteen listings were SAME and ten remained DIFF. These
are focused `kf try --no-flow` verdicts, not strict banking claims; the
frame unit's adjacent `0x80021a60` also rebuilt SAME but is outside this
25-function count. The three purchase-controller DIFF rows in this historical
pass were later closed by the connected label and item-list follow-on below.

| Address | Function or role | Focused verdict |
| --- | --- | --- |
| `0x8001d030` | primary item controller | DIFF 96.6% |
| `0x8001ddd0` | stock item list | DIFF 96.2% |
| `0x8001e0a8` | secondary stock item list | DIFF 96.2% |
| `0x8001e484` | list input | SAME |
| `0x8001e94c` | status values | SAME |
| `0x8001f798` | paired row helper | SAME |
| `0x8001f8b8` | preview choice | DIFF 84.8% |
| `0x8001fb8c` | window drawing | DIFF 83.5% |
| `0x8001fc94` | list renderer | DIFF 97.7% |
| `0x80020748` | two-option drawing | SAME |
| `0x8002083c` | item-model preview | DIFF 73.1% |
| `0x80020990` | value heading | SAME |
| `0x80020b50` | translucent sprite | SAME |
| `0x80020d20` | sprite | SAME |
| `0x80020ef8` | fixed-CLUT sprite | SAME |
| `0x800210ac` | glyph string | DIFF 90.4% |
| `0x80021510` | numeric glyph string | DIFF 93.1% |
| `0x800217f0` | nine-slice panel | SAME |
| `0x80021a68` | frame begin | SAME |
| `0x80021be0` | frame present | SAME |
| `0x80021c8c` | display entry | DIFF 97.2% |
| `0x80021e00` | display exit | SAME |
| `0x80021f10` | primitive begin | SAME |
| `0x80021f60` | primitive commit | SAME |
| `0x80021fb0` | list initialization | SAME |

For the list renderer, retail and source agree on the 19 direct incoming
calls, menu sprite and primitive referents, numeric formatting calls, and
mode-dependent row widths. The first focused difference is register and
placement order for the scroll offset and card-column condition; subsequent
differences include a row-counter register and one branch delay slot. The
previous normal focused comparison agreed on 55/55 CFG blocks and 33/33
branches. Moving the card-column expression into the row loop had already
reduced similarity and was discarded. The two-option unit's preview body
has a 64-byte retail frame surplus over its four live `MATRIX` locals;
despite its lower focused listing score, direct objdiff previously gave
99.65882%. The display-entry function similarly has an unexplained 32-byte
retail frame versus a 24-byte source frame. There is no supported source
object for those unused frame bytes, so this pass retained the current C
and did not disturb the exact siblings.

Ten further direct callers and helpers also retained focused SAME listings:
`0x8001cdb0`, `0x8001ceb8`, `0x8001d340`, `0x8001d3b4`, `0x8001d654`,
`0x8001d6a8`, `0x8001d8d0`, `0x8001dc64`, `0x8001e378`, and `0x8001f008`.
The first displayed difference in the primary item controller at
`0x8001d030` is the order of loading the selected price and current gold
and their temporary registers; the price multiplication, unsigned funds
comparison, and subsequent quantity condition still agree. Neither that
ordering nor the exact helper controls supports changing the represented
table, signedness, or purchase semantics.

Focused first-difference checks across the ten WIPs show the three purchase
controllers sharing the same price/gold load-order and register residue.
`menu_draw_window` and display entry differ only in stack frame size and
saved offsets. The item-model preview likewise differs only by a 64-byte
stack displacement, which affects each live matrix slot. The glyph renderer
has an eight-byte frame surplus in retail plus glyph-index register order;
the numeric glyph renderer differs at its initial font-pointer move and the
two second-column U/width loads. Preview choice first changes saved-register
assignment and schedules four window-layout loads around its result sentinel.
The renderer's first difference remains the scroll/card-mode calculation
described above. Calls, referents, and retail-visible types provide no new
source-backed correction for these residues.

A further ten direct callees remained focused SAME: glyph-row builders
`0x80018d08`, `0x80018dec`, `0x80018f8c`; model load/release
`0x800221e8`, `0x800222bc`; menu cue and pad helpers `0x80022300`,
`0x80022394`, `0x800223cc`; and input release/event initialization
`0x80022438`, `0x80022468`. The other three functions in the shared
memory-card event unit also rebuilt SAME as adjacent controls. This
rules out a newly broken direct callee as the cause of the ten renderer
and controller differences above.

## Early menu controller and strict-object audit

The next GAME pass followed the root menu controller through its item,
equipment, magic, status, and TIM-view branches. Of the 19 functions in
`0x8001876c`–`0x8001a898`, sixteen retained focused SAME listings and three
remain DIFF. Six direct callees outside that address band also retained
SAME, making the connected 25-function pass 22 SAME and three DIFF.

| Address | Focused verdict | Address | Focused verdict |
| --- | --- | --- | --- |
| `0x8001876c` | DIFF 90.8% | `0x800189f0` | SAME |
| `0x80018ac8` | SAME | `0x80018d08` | SAME |
| `0x80018dec` | SAME | `0x80018f8c` | SAME |
| `0x80019240` | SAME | `0x800192ac` | SAME |
| `0x800192dc` | SAME | `0x8001930c` | DIFF 80.2% |
| `0x80019834` | SAME | `0x800199d0` | SAME |
| `0x80019ac4` | SAME | `0x80019ce4` | SAME |
| `0x80019ed4` | SAME | `0x8001a2f4` | SAME |
| `0x8001a4f0` | DIFF 96.0% | `0x8001a7fc` | SAME |
| `0x8001a898` | SAME | `0x8001e484` | SAME |
| `0x800221e8` | SAME | `0x800222bc` | SAME |
| `0x80022300` | SAME | `0x80022394` | SAME |
| `0x800223cc` | SAME | | |

The mixed item/magic controller `0x8001a4f0` matches its 22 CFG blocks,
12 branches, calls, data references, and 2,440-byte frame. Its remaining
focused difference is register choice in the 74-row initializer. Reusing
the `count` local as that loop counter reduced similarity from 96.0% to
83.4% and changed later saved-register assignments, so the source-only
probe was discarded. The TIM preview `0x8001930c` preserves its archive,
allocation, TIM upload, packet, frame, pad, and release calls and its
47-block/27-branch CFG. Reversing the two archive-index addition operands
in a source-only probe reduced similarity from 80.2% to 79.9%; the
current expression remains. Its first differences are the 64-byte retail
versus 56-byte source frame and initial archive-index register schedule.
For `0x8001876c`, the first difference swaps the two saved registers used
for `-1` and `-99`; later the source probe carries a result across the exit
condition where retail reloads it, leaving the same direct call set but a
different branch schedule. No source-backed state or ABI change was retained.

A later focused rebuild of the inclusive `0x8001876c`–`0x8001a898` band
confirmed its 19 claims: sixteen SAME and the same three DIFF results above.
Source-identical compiler probes in temporary objects found that GCC 2.5.7
with `-mcpu=r3000` kept the WIP focused scores at 90.8%, 80.2%, and 96.0%
for `0x1876c`, `0x1930c`, and `0x1a4f0`; GCC 2.6.0 reduced them to 83.0%,
59.2%, and 74.4%. The exact `0x189f0` sibling also fell to 53.0% under
GCC 2.6.0. No profile change was retained. A typed `u8` preview-page local
left `0x1930c` at 80.2% and was discarded.

A bounded audit rebuilt focused-SAME objects in `/tmp` with their pinned unit
profiles and compared each to its carved retail module through isolated
native objdiff reports. The old shared strict report had marked three
functions WIP and omitted three; all six now prove exact. Raw ordered
relocation listings also match for every row.

| GAME address | Unit | Isolated strict result | Old report |
| --- | --- | --- | --- |
| `0x80019ac4` | `game.menu_equipment_list` | `.text` 544/544, `.data` 200/200 | 99.88971% |
| `0x8001d340` | `game.menu_item_code_primary` | `.text` 116/116, `.data` 1440/1440 | 93.10345% |
| `0x8001d3b4` | `game.menu_item_sell_controller` | `.text` 672/672 | absent |
| `0x8001d654` | `game.menu_item_code_secondary` | `.text` 84/84; unit `.text` 636/636, `.data` 1200/1200 | 90.47619% |
| `0x8001d8d0` | `game.menu_item_trade_controller` | `.text` 916/916 | absent |
| `0x8001e484` | `game.menu_list_input_controller` | `.text` 1224/1224 | absent |

The secondary code unit's exact `0x8001d6a8` sibling occupies the other
552 code bytes. The primary code unit has two ordered text relocations,
and the secondary has 26; their candidate and retail relocation lists are
identical. No unit above claims `.rodata`. These isolated object findings
are strict evidence for the listed functions, without a broad match run or
banking.

The status and attribute renderers were absent from the old shared report
but already had direct exact evidence. Fresh isolated objects reconfirm
`0x8001e94c` at 1,724/1,724 text and 240/240 data bytes, and
`0x8001f008` at 1,936/1,936 text bytes. Both ordered relocation lists
match their retail targets. These are confirmations, separate from the six
newly recognized exact functions above.

## Connected label and item-list follow-on

A fresh focused GAME sweep followed the label builders into the primary and
secondary item controllers, their code translators, list input, and status
display. All 23 functions in `0x8001c550`–`0x8001f008` have a retained
SAME listing after the purchase-controller correction below. These are
quick focused `kf try` results; the three changed controllers also passed
normal focused comparisons with flow clues. The ten label
functions also have separate direct objdiff proof for their whole unit:
`.text` 2,784/2,784 bytes and `.data` 320/320 bytes.

| GAME address | Function or role | Focused verdict |
| --- | --- | --- |
| `0x8001c550` | label builder 1 | SAME |
| `0x8001c62c` | label builder 2 | SAME |
| `0x8001c770` | label builder 3 | SAME |
| `0x8001c8b0` | label builder 4 | SAME |
| `0x8001c9f4` | label builder 5 | SAME |
| `0x8001cad4` | label builder 6 | SAME |
| `0x8001cb44` | typed label selector | SAME |
| `0x8001ccd4` | label builder 8 | SAME |
| `0x8001cdb0` | row layout | SAME |
| `0x8001ceb8` | item-category dispatcher | SAME |
| `0x8001d030` | primary item controller | SAME; 100% isolated direct objdiff |
| `0x8001d340` | primary code translator | SAME |
| `0x8001d3b4` | sell controller | SAME |
| `0x8001d654` | secondary code translator | SAME |
| `0x8001d6a8` | secondary code helper | SAME |
| `0x8001d8d0` | trade controller | SAME |
| `0x8001dc64` | stock-list dispatcher | SAME |
| `0x8001ddd0` | primary stock list | SAME; 100% isolated direct objdiff |
| `0x8001e0a8` | secondary stock list | SAME; 100% isolated direct objdiff |
| `0x8001e378` | input poll | SAME |
| `0x8001e484` | list input | SAME |
| `0x8001e94c` | status display | SAME |
| `0x8001f008` | attribute display | SAME |

The three previously DIFF functions have confirmed incoming calls from `0x8001ceb8`
to the primary controller and from `0x8001dc64` to both stock lists. They
have no retail string references. Before correction, their first focused difference was the
same price/gold load order and temporary-register assignment: the probe
loads the selected price stack value before retail does, then changes the
`DAT_8006d694` multiplier register, `mult` operand order, and unsigned funds
comparison register. The purchase path, direct call sets, and relevant
referents remained represented in source. The same residue across all three
does not establish a different item table, signedness, or purchase rule.
Inlining the one-use purchase cost into the funds condition, with the price
as the left multiplication operand, expresses the same calculation and now
matches the retail load order and `mult` operands in all three. Final normal
focused builds are SAME; isolated native objdiff confirms exact `.text`
sections of 784/784, 728/728, and 720/720 bytes respectively. All three
ordered relocation lists match the carved retail objects. The later cost
assignment for the completed purchase is unchanged.

## Renderer, sprite, and frame call-graph extension

The item-list controllers' direct rendering, model, buffer, and cue helpers
form a 25-function follow-on in `0x8001f798`–`0x800223cc`, excluding the
separately owned preview-choice and number-formatter functions. Twenty
current focused listings are SAME and five are DIFF. Existing exact sprite,
buffer, model, and cue siblings remained stable after the item-controller
edits. The five WIPs had fresh image-qualified retail block disassembly,
callers, callees, strings, and match-state review; none has a retail string
reference.

| GAME address | Role | Focused verdict |
| --- | --- | --- |
| `0x8001f798` | paired menu rows | SAME |
| `0x8001fb8c` | window drawing | DIFF 83.5% |
| `0x8001fc94` | item/card list renderer | DIFF 98.6% |
| `0x80020748` | two-option drawing | SAME |
| `0x8002083c` | item-model preview | DIFF 73.1% |
| `0x80020990` | value heading | SAME |
| `0x80020b50` | translucent sprite | SAME |
| `0x80020d20` | sprite | SAME |
| `0x80020ef8` | fixed-CLUT sprite | SAME |
| `0x800210ac` | glyph string | DIFF 90.4% |
| `0x80021510` | numeric glyph string | SAME; 100% isolated direct objdiff |
| `0x800217f0` | nine-slice panel | SAME |
| `0x80021a60` | frame stub | SAME |
| `0x80021a68` | frame begin | SAME |
| `0x80021be0` | frame present | SAME |
| `0x80021c8c` | display entry | DIFF 97.2% |
| `0x80021e00` | display exit | SAME |
| `0x80021f10` | primitive begin | SAME |
| `0x80021f60` | primitive commit | SAME |
| `0x80021fb0` | list initialization | SAME |
| `0x800221e8` | item-model load | SAME |
| `0x800222bc` | item-model release | SAME |
| `0x80022300` | menu cue | SAME |
| `0x80022394` | input activation | SAME |
| `0x800223cc` | input release | SAME |

For `menu_draw_number`, the retail second-column U computation adds seven
to the font's byte U before adding glyph width. Casting that expression to
`u8` at the Psy-Q packet boundary expresses the stored coordinate's width
and fixes both UV paths. The pinned Psy-Q 3.0 `LIBGPU.H` `setUVWH` macro
adds width for the `u1` and `u3` packet writes, and `POLY_FT4` stores
each U coordinate as `u_char`; this supports the byte boundary independently
of the score. Initializing the code pointer before checking its
first halfword fixes the one remaining argument-save placement. Normal
focused comparison is SAME, and isolated native objdiff proves all 736
text bytes with an identical raw ordered relocation listing. A guard-only
rewrite left the 99.5% intermediate listing unchanged; a while-loop form
moved the code-pointer calculation too early and was discarded.

For the other five WIPs, direct call sets and referents remain supported.
The list renderer still first differs at the scroll/card-mode calculation;
its 55 blocks and 33 branches agree with retail. The window and display
entry helpers retain unexplained eight-byte frame surpluses, while the
item-model preview has a 64-byte surplus over its four live `MATRIX`
objects. The glyph-string renderer retains a frame surplus and glyph-UV
register choices. A single controlled GCC 2.6.0 probe for window drawing
fell to 63.7% isolated objdiff, compared with the existing GCC 2.5.7
probe's 99.8%; no compiler-profile change was retained. There is no
supported source object for the surplus frame bytes, so these five sources
were left unchanged. A `u16` glyph-index local left the string renderer's
focused listing at 90.4%, and swapping independent scroll-pointer updates
left the list renderer at 97.7%; both source-only probes were discarded.
An explicit prechecked `do` loop moved the card-mode calculation behind
the retail bounds checks but changed other loop instructions, reducing the
renderer to 92.2%; it was discarded. Explicit byte casts on the glyph
renderer UV coordinates left its 90.4% listing unchanged and were also
discarded.
Two lower-panel row-initialization probes, one with an explicit positive-row
guard and `do` loop and one retaining the `for` loop, reduced the list
renderer to 95.9% and shrank its otherwise correct 128-byte frame to 120
bytes. A guard around the existing `for` loop retained the frame but reduced
the listing to 97.4%. None of these forms was retained.

Fresh five-unit isolated native objdiff confirms current strict per-function
scores of 99.78788% (`0x8001fb8c`), 98.44964% (`0x8001fc94`),
99.65882% (`0x8002083c`), 99.66904% (`0x800210ac`), and 99.956985%
(`0x80021c8c`). The two-option and display-exit siblings remain 100%.
All five units now have identical raw ordered relocation listings. The list
renderer previously had all 201 relocation types and symbols in the same
order, with downstream offsets shifted by four bytes after a body
instruction gap; its retained lower-panel loop removed that gap. The loop
increments the row counter before starting each tile packet, as retail does
in the call delay slot and as the KF1 sibling's source expresses. Normal
focused similarity improves from 97.7% to 98.6% and the correct 128-byte
frame remains. Isolated objdiff's fuzzy percentage moves slightly downward
from 98.581024% to 98.44964%; the source-backed loop is retained because
its instruction order and relocation sites are closer to retail.
Applying the KF1-style prechecked `do` loop to the main text rows was a
separate source-only probe: it repurposed the retail sentinel register,
shrunk the frame to 120 bytes, and reduced the focused listing to 92.4%.
The main row loop remains in its prior source form.
Spelling the card-column predicate as `render_mode == 8 || render_mode == 9`
left the retained 98.6% listing unchanged, so that source-only probe was
discarded too.

Source-identical alternate-profile probes on six further UI units produced
no strict closure. GCC 2.5.7 with `-mcpu=r3000` reproduced the current
focused listings and preserved exact siblings. GCC 2.6.0 reduced the window,
list, option-preview, display-entry, and glyph-string listing similarities;
it also changed exact siblings in the option and display-state units. Its
fade-transition body improved from 44.8% to 75.9% focused similarity, but
both exact neighboring functions regressed. These are compiler probes, not
evidence to split a source unit or change its profile.

## Bounded relocation and data audit

The existing carved GAME targets and source objects rebuilt in `/tmp` were
compared without regenerating the shared target registry. The six WIP units
below have identical ordered relocation types and symbol names. Relocation
site offsets differ only where the instruction stream already differs; the
two rows marked `sites SAME` have byte-identical ordered relocation rows.
The only initialized side section in these units is the 28-byte root-menu
switch table, which also matches byte for byte.

| GAME function | Target/source relocations | Site offsets | Initialized side data |
| --- | ---: | --- | --- |
| `0x8001876c` | 55/55 | DIFF after controller schedule | `.rodata` 28/28 SAME |
| `0x8001930c` | 69/69 | DIFF from first graphics-state address | none |
| `0x8001a4f0` | 28/28 | SAME | none |
| `0x800311b0` | 7/7 | DIFF from first graphics-state address | none |
| `0x800349bc` | 96/96 | DIFF after fade prologue | none |
| `0x80036e24` | 5/5 | SAME | none |

No missing relocation or initialized datum explains these six non-exact
functions. The separate resource transition `0x80016820` still references
unowned RAM workspaces at `0x8019e138` and `0x8012da68`; their known uses do
not establish complete extents or a source allocation mechanism, so no data
claim was added.
The frame-service `0x36e24` remains DIFF 87.8% in a fresh focused listing:
its five calls and two branches agree, while the mode, endpoint, and step
arguments occupy cyclically different saved registers.

## Connected item/magic controller and direct-helper strict audit

An isolated fresh compile and direct objdiff comparison of 22 GAME menu units
covering 36 functions around `0x8001a4f0` and its direct helpers gives **30/36
strict exact**. All initialized `.data` and `.rodata` sections owned by these
units also compare at 100%. The six non-exact functions remain WIP:

| Function | Strict objdiff | First unresolved difference |
| --- | ---: | --- |
| `0x8001876c` | 96.36646% | `-1`/`-99` saved-register choice and later result reload/branch schedule |
| `0x8001930c` | 98.26363% | Retail 64-byte frame and saved `s7` versus probe 56-byte frame |
| `0x8001a4f0` | 99.74359% | 74-row initializer index and constant registers |
| `0x8001f8b8` | 94.36464% | Argument register retention and input-release exit-block placement |
| `0x8001fc94` | 98.44964% | Card-mode row-width and lower-panel scheduling |
| `0x8002083c` | 99.65882% | Retail reserves 64 more stack bytes than four live `MATRIX` locals explain |

The strict-exact functions in this audit are `0x800189f0`, `0x80018ac8`,
`0x80018d08`, `0x80018dec`, `0x80018f8c`, `0x80019240`, `0x800192ac`,
`0x800192dc`, `0x80019834`, `0x800199d0`, `0x80019ac4`, `0x80019ce4`,
`0x80019ed4`, `0x8001a2f4`, `0x8001a7fc`, `0x8001a898`, `0x8001e484`,
`0x80020748`, `0x80020990`, `0x80021a60`, `0x80021a68`, `0x80021be0`,
`0x80021f10`, `0x80021f60`, `0x80021fb0`, `0x800221e8`, `0x800222bc`,
`0x80022300`, `0x80022394`, and `0x800223cc`. Reusing the row-count local
for the `0x8001a4f0` initializer loop reduced its focused listing from 96.0%
to 83.4% and shifted later saved-register roles; the probe was reverted.
No source edit was retained from this audit.

KF1 master `src/game/menu_runtime.c` (`5a3469b3`) supplies an analogous
`menu_draw_window` with indexed `layout->rows[row]` access. That expression
was tried in KF2 `0x8001fb8c`, but the focused listing fell from 83.5% to
33.1%: the probe recomputed row offsets and changed the loop, while KF2
retail increments a row pointer by 28 bytes in the branch delay slot. The
KF2 pointer-walk source and the exact 0xa90-byte initialized menu table stay
unchanged.

KF1 `menu_draw_string` in the same `5a3469b3` source uses a signed glyph
local. The corresponding KF2 signed-local probe fell from 90.4% to 68.9%
focused similarity because GCC emitted signed remainder/division correction
branches throughout the atlas packet writes. KF2 retail uses the unsigned
mask/shift sequence, so the unsigned local remains.

## Text, list, and transition follow-up

Fresh isolated compiles and direct objdiff compared ten connected GAME text,
list, and transition functions. Three listings are strict exact; seven retain
the following WIP verdicts. The transition's initialized `menu_transition_rect`
and the window renderer's two initialized tables were preserved unchanged.

| Function | Strict objdiff | Final verdict |
| --- | ---: | --- |
| `0x8001a4f0` item/magic controller | 99.74359% | Initializer-loop index and constant-register residue; 22/22 CFG blocks agree |
| `0x8001f8b8` preview choice | 94.36464% | Argument register assignment and input-release block placement; 42/42 CFG blocks agree |
| `0x8001fb8c` window draw | 99.78788% | Retail reserves 48 stack bytes versus 40 in the probe; row pointer walk remains supported |
| `0x8001fc94` list renderer | 99.70803% | Retail loads RGB constant `0xff` before zeroing the row Y offset at `0x8002029c`/`0x800202a0`; probe reverses those two instructions, with all later listing and referents aligned |
| `0x800210ac` string draw | 99.66904% | Retail reserves 56 stack bytes versus 48 and chooses the opposite glyph UV temporary register |
| `0x80021510` number draw | **100%** | Exact 736-byte text-helper control |
| `0x80022058` number formatter | 97.39% | Retail reserves eight stack bytes; the fifth argument consequently loads at `24(sp)` rather than `16(sp)`, while the digit and style operations otherwise align |
| `0x8003494c` TIM upload | **100%** | Exact 112-byte transition control |
| `0x800349bc` fade transition | 96.31408% | The mutable pad state spills to retail stack and quad constants occupy different saved registers; geometry, calls, and phase behavior agree |
| `0x80034e10` transition loader | **100%** | Exact 384-byte transition control |

KF1 master `src/game/menu_runtime.c` at `5a3469b3` has an exact
`menu_list_render` with the same `row = 0; if (row < visible_rows) { yoff = 0;
do { ... } }` row-loop structure used here. KF2 retail's sole remaining list
difference is the order of the independent color and Y-offset setup; this
cross-game source does not justify inserting a carrier local. KF1's number
formatter has no KF2 style argument or style branches. A focused KF2 trial
changing the blank-glyph temporary from `s16` to KF1's `s32` reduced listing
similarity from 78.5% to 69.1%, added instructions, and changed delay-slot
scheduling. That trial was reverted. KF1 `display_play_transition.c` is an
image fade, but has a different packet/loop contract than KF2's four-quad
menu transition, so no source shape was transferred. No C edit was retained.

A fresh isolated `0x8001fc94` control remains 99.70803% strict over 2,740
bytes. Moving the existing lower-row `y = 0` initializer outside its
visibility guard left the values unchanged but lowered strict text to
99.49927%. The retail color/Y instruction order is therefore still an
unattributed scheduling residue; the guarded source form is retained.

## Adjacent loaded menu scalars

GAME `0x8006d690` is a four-byte initialized-zero input latch: 16 validated
references include `input_read_mark_active` setting it after a nonzero pad
read and `func_800223cc` clearing it before the release wait. GAME
`0x8006d694` is a separate four-byte initialized-one menu quantity: 24
validated references include `menu_load_item_model` resetting it and the
bounded 1–99 list-input increment/decrement. The two nonoverlapping words now
have `DATA` definitions in those respective current C units. Their original
allocation and defining translation units remain unknown.

After a focused safe carve admitted all five affected functions with no
withheld relocations, fresh isolated GCC 2.5.7 objects directly compared at
strict 100% for the two `menu_item_model` functions, three `menu_sound_cue`
functions, and both units' `.data` sections (28 and 4 bytes). The individual
four-byte symbols also compare at 100%.

Three already-defined load objects had stale blank current-C owner fields:
`menu_sprite_defs` (`0x80063e80`, 240 bytes) and `menu_window_layouts`
(`0x80063f70`, 2464 bytes) are in `menu_draw_window.c`, and
`menu_transition_rect` (`0x8006d6dc`, eight bytes) is in `menu_transition.c`.
Their original translation-unit boundaries are still unknown. Focused safe
carves had no withheld rows; isolated objdiff gives each symbol and both
units' `.data` sections 100%. The window body remains at its prior
99.78788% strict frame-size residue. Transition's two exact neighbors remain
100%, while the fade body retains its prior 96.31408% WIP verdict.

## Fresh twelve-function menu control (2026-10-02)

Eight menu units were recompiled individually with their complete manifest GCC
2.5.7 `-O2 -G0 -mcpu=r2000` profile and compared by isolated strict objdiff
against their existing safe-delink targets. The already-current preview choice
object was included as a ninth unit. All nine units preserve the exact
target/candidate `.rel.text` row counts (48, 69, 28, 54, 7, 19, 80, 38, and
36 respectively). Three functions are strict exact; nine remain WIP:

| GAME function | Fresh strict | Verdict |
| --- | ---: | --- |
| `0x8001876c` location number | 96.36646% | Result/sentinel register and reload schedule; fields and control retained |
| `0x800189f0` location sibling | **100%** | Exact |
| `0x8001930c` map preview | 98.26363% | Retail saves `s7` in a 64-byte frame; candidate uses 56 bytes |
| `0x8001a4f0` item/magic controller | 99.74359% | Initializer index/constant registers differ |
| `0x8001b554` card browser | 98.478264% | Probe result stays in `v0` rather than retail `a0` |
| `0x8001f8b8` preview choice | 99.14365% | Eight remaining raw words have register/order differences; its older 94.36464% report is stale |
| `0x8001fb8c` window draw | 99.78788% | Retail 48-byte versus candidate 40-byte frame; row walk retained |
| `0x80020748` two-option draw | **100%** | Exact |
| `0x8002083c` two-option preview | 99.65882% | Retail reserves 64 more stack bytes than the modeled live matrices require |
| `0x80020990` two-option sibling | **100%** | Exact |
| `0x800210ac` string draw | 99.66904% | Frame and UV temporary register placement |
| `0x80022058` number format | 97.39% | Retail reserves eight extra stack bytes; digit and style operations align |

An off-tree, source-equivalent preview-choice control moved the exit from the
existing label to a natural `while (result == -99)` loop followed by
`input_wait_release()`. With the complete unit profile it fell from
**99.14365%** to **94.91713%** strict and from 36 to 34 `.rel.text` rows,
so the tracked source is unchanged. These comparisons supplied no new field,
call, or referent correction; no menu C or identity edit was retained.

## Menu item controller exact control

Ten further menu-item units were compiled individually with the same complete
manifest profile and compared by isolated strict objdiff. All eleven function
claims are **100% exact**, with equal target/candidate text relocation counts:
`0x80018ac8` selection (22/22), `0x80019ed4` equipment category (56/56,
plus 18/18 RODATA relocations), `0x8001a898` equipment (18/18), `0x8001d030`
primary (38/38), `0x8001d340` item code (2/2), `0x8001d3b4` sell (29/29),
`0x8001d654` and `0x8001d6a8` secondary code (26/26 for the unit),
`0x8001d8d0` trade (39/39), `0x8001ddd0` stock (31/31), and `0x8001e0a8`
secondary stock (29/29). `game.menu_item_model` was also refreshed separately:
both load/release functions are exact with 32/32 unit text relocations. This
screen retained no source or inventory edit.

The related status and attribute renderers were also checked from current
sources with the full manifest profile. `game.menu_status_render` at
`0x8001e94c` and `game.menu_attribute_render` at `0x8001f008` are each
**100% strict exact**, with 123/123 and 165/165 target/candidate text
relocations. Older exploratory notes that call them unclaimed are stale.

A separate ten-unit card/menu input control covers eleven more functions.
`0x8001aa9c`, `0x8001ac80`, `0x8001b834`, `0x8001bcfc`, `0x8001c12c`,
`0x8001a7fc`, `0x8001e378`, `0x8001e484`, `0x8001dc64`, and the
`menu_blit_sprite_translucent` entry are **strict exact**. The remaining
`0x8001bf68` card-format flow is **97.12389%** strict with its known probe
status register and later write-path scheduling difference. All ten units
have equal target/candidate text relocation counts; the card-format unit has
76/76. No source-backed call, field, or control correction emerged, and no C
or inventory edit was retained.

Ten menu row/list/card units add 18 function claims in another current-source
strict control. Seventeen are exact: `0x80018d08`, `0x80018dec`,
`0x80018f8c`, `0x80019240`, `0x800192ac`, `0x800192dc`, `0x80019834`,
`0x800199d0`, `0x80019ac4`, `0x80019ce4`, `0x8001a2f4`, `0x8001af30`,
`0x8001b030`, `0x8001b14c`, `0x8001b2dc`, `0x8001ba80`, and
`0x8001bb94`. The card browser `0x8001b554` remains **98.478264%** WIP
with its previously documented result-register residue. All ten units have
equal target/candidate text relocation counts; their row and card data
identities did not need changes. In particular, older reports naming
`0x8001b2dc` WIP are stale relative to this fresh strict check.

Ten visual/primitive units add 15 current-source strict verdicts. Fourteen
functions are exact: `0x8001f798`, `menu_draw_two_option`, `0x80020990`,
`menu_blit_sprite`, `menu_blit_sprite_fixed_clut`, `menu_draw_number`,
`0x800217f0`, `0x80021a60`, `menu_frame_begin`, `menu_present_frame`,
`menu_blit_sprite_translucent`, `primitive_buffer_begin_poly_ft4`,
`primitive_buffer_commit_poly_ft4`, and `menu_list_init`. The sole WIP is
`0x8002083c` at **99.65882%** strict, with its already documented 64-byte
retail/candidate frame difference and no proved extra live object. All ten
units have equal target/candidate text relocation counts; the two-option unit
has 80/80. Earlier `menu_draw_number` WIP scores are stale for the current
source. No C or inventory change was retained.
