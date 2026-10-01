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
| `0x800475d8` | `func_800475d8` | DIFF 88.3%; existing event-controller CFG and register residue |

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
referents, call set, and 8/8 CFG blocks agree with retail. Its focused
listing is DIFF 57.5%, while the earlier strict report recorded 92.14815%;
these metrics use different comparisons. Replacing conditional
`setSemiTrans` with the equivalent direct `quad->code = 0x2e` in a source-only
probe left the focused listing unchanged, so it was discarded. The event
map-object controller remains focused DIFF 88.3% and is owned by its separate
campaign; no source edit was made in either family.

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
