# GAME menu render-mode and packet follow-up

This batch rechecked ten non-exact GAME functions connected through item-code
tables, card dialogs, glyph drawing, and TMD preview rendering. Each target
received the six `kf sema --image game` evidence queries (address, block
disassembly, incoming references, outgoing references, strings, and match).
The King's Field I menu and TMD sources were compared for source shape; its
window renderer has an extra backdrop call, and its number formatter has no
GAME II style argument, so neither body can simply be transplanted.

| GAME address | Retail/source finding | Final strict verdict |
| --- | --- | --- |
| `0x8001d340` | The primary 6-by-120 halfword table is 1440/1440 exact. Four CFG blocks agree; the last stride shift is ordered around table-base setup differently. | **WIP, 93.10345%** |
| `0x8001d654` | The secondary 5-by-120 halfword table is 1200/1200 exact. The same stride/base instruction ordering remains with four matching blocks. | **WIP, 90.47619%** |
| `0x8001bf68` | Card probe, optional format, result dialog, and slot write calls agree. Retail retains a dialog-width value in a saved register on the late path. Two temporary source probes sharing the overlap or width constants compiled farther away (81.9% and 79.0% focused similarity), so neither was retained. | **WIP, 97.12389%** |
| `0x8001f8b8` | Fifteen callers and the forwarded call to `0x8001fc94` establish that its third argument is an integer render mode, not a pointer. Retail compares the mode arithmetically; callers pass `2` and `4`. The source, local caller declarations, and identities now use `s32 render_mode`. CFG remains 42/42 blocks and 18/18 branches; block order and saved-register assignment differ. | **WIP, 94.36464%** |
| `0x8001fb8c` | Window title and row calls agree. Retail reserves 48 stack bytes versus 40 compiled bytes; no live object supports padding. | **WIP, 99.78788%** |
| `0x800210ac` | Glyph and kana-mark packet operations agree across eight blocks. Retail has a 56-byte frame versus 48 compiled bytes and a UV register-choice residue. | **WIP, 99.66904%** |
| `0x80021510` | Numeric atlas column selection and packet calls agree across seven blocks. Entry and atlas-U load ordering differ. | **WIP, 98.61957%** |
| `0x80022058` | Decimal style/padding behavior matches the caller contract. Retail has an eight-byte frame and 36 CFG blocks; current C is a leaf with 37 blocks. | **WIP, 95.74000%** |
| `0x8002d5dc` | The typed packet parser follows the King's Field I switch shape with GAME II word-width counts. Its 29-entry switch table is 116 bytes in both objects, but the first target addend is `+0x80` retail versus `+0x84` compiled after an extra entry move. | **WIP, 96.13260%** |
| `0x80023178` | Card-label EXP and LV digit loops have 19 matching CFG blocks and the correct byte stores. Retail assigns the label to `a3` and first quotient to `a0`; current C assigns them differently without a proved source-level correction. Three adjacent card helpers remain exact. | **WIP, 93.52941%** |

The retained ABI correction is type-only at O32. Focused listings for its exact
callers `0x80019834` and `0x8001a2f4` remained SAME. A strict GAME refresh
relinked 146/146 units and kept 388/456 scored functions exact. Global closure
still stops at three unrelated TMD/map-object `.rodata` addends. The required
full `kf build` built PSX; GAME, OPEN, and END retain their established first
unresolved link symbols `InitCARD`, `malloc`, and `display_buffers`. No
repository tests, banking, or commit were performed.
