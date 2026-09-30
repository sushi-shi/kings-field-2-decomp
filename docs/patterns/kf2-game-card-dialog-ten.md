# GAME card dialogs and item-list choices

This pass covers ten previously non-exact GAME functions connected by card-menu
calls, glyph rows, and two-frame choice controllers. Each received the matcher
address, block disassembly, incoming-xref, callee, string, and match-state
queries. No function in this set has a supported vendored attribution. The
larger list handlers remain separate WIP ownership questions.

| Address | Retail evidence | Final verdict |
| --- | --- | --- |
| `0x8001b834` | card-directory scan calls the card-row selector, list initializer, preview panel, and save reader | WIP, unclaimed; complete 40-byte card-entry state is unresolved |
| `0x8001bcfc` | card startup probes a temporary file, enumerates card entries, then calls the format flow and cleanup | WIP, unclaimed; card-list record and lifetime remain unresolved |
| `0x8001bf68` | temporary-file probe selects format/error dialogs, then writes the chosen card slot | **WIP, 97.123890% strict**; probe-status register and late-path schedule differ |
| `0x8001c12c` | glyph suffixes and window-layout rows feed a two-option dialog with pad input | WIP, unclaimed; dialog stack record and full field use remain unresolved |
| `0x8001ceb8` | two-frame choice loop dispatches two item-list branches using the caller's low-nibble kind | **exact, 100% strict** |
| `0x8001d030` | first item-list branch calls the primary code translator and item-model preview | WIP, unclaimed; list record and `0x80065950` data owner remain unresolved |
| `0x8001d3b4` | paired item-list branch calls the secondary translator and item-model preview | WIP, unclaimed; list record and second lookup owner remain unresolved |
| `0x8001d6a8` | script-driven item-list/model renderer returns a selection used by its caller | WIP, unclaimed; list/model state is unresolved |
| `0x8001d8d0` | related item-list/model renderer uses the primary translator and a shared record | WIP, unclaimed; record and indexed table view remain unresolved |
| `0x8001dc64` | two-frame choice loop dispatches the two item-list branches without arguments | **exact, 100% strict** |

`0x8001bf68` now has a C source for all three dialog outcomes, the card-slot
write, and the pad-release loops. The 160-byte frame, ordered calls, geometry
arguments, and 24 decoded direct-call/internal-jump relocations agree with
retail. Its four-row scratch buffer fits the observed 112-byte local region;
the original declaration remains unproven, and no exact claim is made. The
first remaining instruction difference is the probe status kept in `s0`
instead of `v1`; the write-result branch also has an eight-byte scheduling
residue. No extra carrier or assembly was added to force those registers.

The `0x8001ceb8` source extends `game.menu_label_templates` contiguously.
Its two list dispatches, selection pointers, and two-frame draw loops match
retail; all ten function listings in that unit remain exact, including the
nine prior glyph builders/renderer and the initialized suffix table. The
matching no-argument controller at `0x8001dc64` is a separate unit because
intervening list functions remain unclaimed. Both sources exposed that callers
pass mode `1` to `func_80021c8c`; its shared prototype now records the
argument while the callee body remains unchanged. The 34 decoded call/jump
relocations in these two controllers were reviewed before strict comparison.

Focused comparisons reported 10/10 and 1/1 identical listings. The strict
GAME report confirms both new functions at 100% and 139/139 target relinks.
Retail census validation and `git diff --check` pass. The global strict
command still stops at known-reference closure and three unrelated TMD/map
`.rodata` addends. No repository tests, banking, or commit were performed.
