# GAME menu and card controller records

This ten-function GAME campaign follows card and item-list callers into the
shared menu glyph, number, preview, and primitive helpers. Each function was
checked through the retail address, disassembly/CFG, incoming and outgoing
references, strings, and current match queries. No function in this set has
Sony/Psy-Q library-body evidence.

| Address | Retail role and ownership boundary | Final verdict |
| --- | --- | --- |
| `0x8001a4f0` | Mixed item/magic list controller uses 74-row glyph, count, value, and ID buffers, then previews and stores the selected entry. | **WIP, 99.74359% strict**; 22/22 CFG blocks, initialization-loop register residue |
| `0x8001ac80` | Card startup and directory/preview flow calls 18 distinct helpers. | **exact, 688/688 code bytes** |
| `0x8001b2dc` | Seven-row option controller copies six player-state flags, toggles a selected flag, and draws paired glyph labels. | **exact, 632/632 code bytes** |
| `0x8001b554` | Card startup, temporary-file probe, directory scan, input loop, and frame rendering. | **WIP, 98.478264% strict**; probe-result register and delay-slot order differ |
| `0x8001d030` | Primary item-list branch uses the `0x80065950` page and `0x80065aeb` interior lookup plus preview/menu helpers. | **exact, 784/784 code bytes** |
| `0x8001d3b4` | Secondary item-list branch uses the paired translator and a large stack workspace. | **exact, 672/672 code bytes** |
| `0x8001d6a8` | Item-model selector has a 3,232-byte frame with typed glyph rows and two 120-byte arrays. | **exact, 552/552 code bytes** |
| `0x8001e94c` | Status/numeric renderer draws glyph suffix rows and player values with menu string/number helpers. | **exact, 1,724/1,724 code and 240/240 owned data bytes** |
| `0x8001f008` | Straight-line paired component renderer reads player attack/combat halfwords and glyph suffix rows 2 and 3. | **exact, 1,936/1,936 code bytes** |
| `0x8001fc94` | Shared numeric/list renderer emits several FT4 packets and calls the exact primitive-buffer pair. | **WIP, 98.44964% strict**; row-count and card-column scheduling differ |

The `0x8001a4f0` source uses the shared 52-byte menu render view and exact
stack extents: 74 glyph rows, two 74-byte ID arrays, one 74-byte count array,
and 74 number words. Retail clears two event counters only during the initial
item selection, appends magic rows, and copies the shared eight-byte row
prefix. The focused target has 22 reviewed direct-control/data relocation
rows and no withheld rows. After correcting the result to a selected row
index, the remaining listing differences are confined to register allocation
in the 74-entry initializer; the source does not add a carrier for them.

The new `0x8001b2dc` source is appended to the contiguous
`game.menu_card_panel` unit. Retail loads `0x80198597..0x8019859c` from
`player_state+0xc7..+0xcc`, then writes those bytes back on exit. The source
uses the established audio option fields and four `unknown_c9` bytes; it does
not define an overlapping BSS object. Its two 28-byte glyph strings and six
local flags account for the observed stack accesses. Twelve decoded HI/LO
pairs were curated to `player_state`, and the direct calls, internal jumps,
and cursor-data pairs in its range were promoted from candidate rows after
instruction review. The initial source probe had a 22-block CFG and matching
calls but reloaded the literal option count where retail kept six in `s4`.
That 95.601265% observation is historical: a fresh focused build and direct
objdiff find this 632-byte function and its two preceding panel siblings
strict exact. No artificial register carrier was added.

Fresh focused builds and direct per-unit objdiff now establish **7/10 strict
exact** in this controller-record survey. The three WIPs are `0x8001a4f0`
(99.74359%), `0x8001b554` (98.478264%), and `0x8001fc94` (98.44964%);
their current differences remain register, delay-slot, or scheduling residues
after the reviewed calls and referents. The exact `0x8001e94c` unit also owns
240/240 correct initialized bytes at `0x80064a00`. No source edit, linked
build, repository test, or banking was done for this recheck.
