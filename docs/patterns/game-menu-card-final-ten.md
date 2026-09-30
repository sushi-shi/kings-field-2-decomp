# GAME menu and card final ten

This ten-function pass follows menu item-code translation, card-choice and
preview control, glyph rendering, directory scanning, and save-label writing.
Each function has a C source claim. Retail disassembly, CFG, proven call and
data references, neighboring claims, KF1 analogues where present, and current
source and match state were reviewed. All ten are game-owned; their Sony calls
are SDK boundaries, not evidence that the enclosing functions are vendored.

| GAME address | Retail/source evidence | Final verdict |
| --- | --- | --- |
| `0x8001b2dc` | Seven-row option controller toggles six player-state bytes and redraws twice per input step; 22/22 CFG blocks agree. | **WIP, 95.601265% strict**; choice-register lifetime and schedule differ. The exact adjacent card-panel functions remain preserved. |
| `0x8001bf68` | Temporary-file probe selects format, error, or save dialog, then writes a slot; all retail call targets agree. | **WIP, 97.12389% strict**; probe-status register and late write-path scheduling differ. Its exact `0x8001c12c` sibling remains preserved. |
| `0x8001d340` | Primary item-code translator indexes the complete six-page, 1,440-byte halfword table; 4/4 CFG blocks agree. | **WIP, 93.10345% strict**; the final page-stride shift is scheduled around the table-base address pair differently. Its initialized table remains 1,440/1,440 exact. |
| `0x8001f8b8` | Preview-choice controller initializes two glyph rows, polls input, and draws two frames; 42/42 CFG blocks agree. | **WIP, 94.36464% strict**; block order and saved-register assignment differ after the proven integer render-mode signature correction. |
| `0x8001fb8c` | Window title and selected rows use the correct sprite/string calls and pointer walk; 10/10 CFG blocks agree. | **WIP, 99.78788% strict**; retail reserves 48 stack bytes, current C 40, with no evidenced extra live object. |
| `0x800210ac` | Glyph-string renderer emits base and kana-mark packets with eight matching CFG blocks. | **WIP, 99.66904% strict**; retail reserves 56 stack bytes, current C 48, plus one UV register choice. |
| `0x80021510` | Number renderer selects one of two glyph-atlas columns and commits each packet; 7/7 CFG blocks agree. | **WIP, 98.61957% strict**; entry delay-slot and atlas-U load order differ. |
| `0x80022058` | Decimal formatter writes padding, style glyphs, and right-to-left digits; retail's zero-quotient path matches the source loop. | **WIP, 95.74% strict**; retail reserves eight stack bytes and has 36 CFG blocks, while current C compiles as a 37-block leaf. |
| `0x800226ec` | Card directory walker uses SDK `DIRENTRY`, the `BISLPS-00069` prefix, fifteen slots, and `firstfile`/`nextfile`; 13/13 CFG blocks agree. | **WIP, 93.60504% strict**; opening `memset` argument/store scheduling and a later pointer-add order differ. Adjacent card-byte data ownership remains provisional. |
| `0x80023178` | Save-label writer encodes player EXP and level in two Shift-JIS decimal loops; 19/19 CFG blocks agree. | **WIP, 93.52941% strict**; retail keeps the label in `a3` and quotient in `a0`, while compiled registers differ. Three adjacent wait/checksum helpers remain exact. |

Focused comparisons of all ten units preserved seven exact neighboring
listings. No source-only probe has established a different semantic model for
the remaining frame or register differences, so no source/config edit was
retained. A fresh `kf match --image game` relinked 149/149 target units and
confirmed every score above; the global edge-check still stops on the three
known, unrelated TMD/map-object `.rodata` addends. The preceding shared full
build produced PSX and retained the established unresolved overlay links.
No repository tests, bank, or commit were run.
