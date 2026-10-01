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
| `0x8001d030` | `func_8001d030` | DIFF 96.6%; price/quantity load and register order |
| `0x8001d340` | `func_8001d340` | SAME |
| `0x8001d3b4` | `func_8001d3b4` | SAME |
| `0x8001d654` | `func_8001d654` | SAME |
| `0x8001d6a8` | `func_8001d6a8` | SAME |
| `0x8001d8d0` | `func_8001d8d0` | SAME |
| `0x8001dc64` | `func_8001dc64` | SAME |
| `0x8001ddd0` | `func_8001ddd0` | DIFF 96.2%; same purchase-expression residue |
| `0x8001e0a8` | `func_8001e0a8` | DIFF 96.2%; same purchase-expression residue |
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
| `0x80021510` | `menu_draw_number` | DIFF 93.1%; font width/height load order in two UV paths |
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
field-write groups in its source. A normal focused probe retains DIFF 43.2%,
with 14/14 CFG blocks and 8/8 branches; adjacent `tim_upload_images`
(`0x8003494c`) and `func_80034e10` remain SAME. Retail spills the fade state
on a 72-byte frame, while the current 64-byte probe keeps it in a saved
register. The macro substitution changes source expression without claiming
an exact-match gain.

## Connected item, list, and display pass

A focused GAME pass followed confirmed menu-list calls and shared sprite,
primitive-buffer, and display state through 25 functions from `0x8001d030`
to `0x80021fb0`. Fifteen listings were SAME and ten remained DIFF. These
are focused `kf try --no-flow` verdicts, not strict banking claims; the
frame unit's adjacent `0x80021a60` also rebuilt SAME but is outside this
25-function count.

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
