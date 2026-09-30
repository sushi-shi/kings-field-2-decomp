# GAME card and CD follow-up ten

These ten GAME functions link card selection and payload writing to the CD
stream/archive service. Each was checked against retail disassembly, CFG,
callers, callees, strings, data references, current source and match state,
adjacent claims, and the vendored inventory. None is a supported vendored body.

| GAME address | Decisive retail evidence | Final verdict |
| --- | --- | --- |
| `0x8001b834` | A 1056-byte card-menu frame enumerates files, builds rows, polls selection, and calls the card-read path. | **WIP, unclaimed**; its ordered directory and row workspace need typed source ownership. |
| `0x8001bcfc` | A 1016-byte card-menu frame calls display-state setup, label templates, item preview, and the card-format flow. | **WIP, unclaimed**; the list/row stack layout and return paths are not fully recovered. |
| `0x8001d8d0` | A 1328-byte menu controller reads primary item codes and calls menu input, preview, and redraw functions. | **WIP, unclaimed**; the table slice at 0x80065b30 is still only a candidate interior owner and the stack row layout is incomplete. |
| `0x8001ddd0` | The paired 1328-byte controller reads primary item codes and calls the same input/preview family. | **WIP, unclaimed**; its 0x80065ba8 table slice and branch-dependent row workspace remain provisional. |
| `0x8001e0a8` | Another menu controller reads secondary item codes, uses the same input helper, and draws two menu frames. | **WIP, unclaimed**; its 1320-byte stack workspace and menu data boundary need source recovery. |
| `0x8001e484` | Shared menu-input service reads `KfMenuList` bytes at +0x1e..+0x22, moves its cursor, loads item models, and plays UI cues; sixteen direct callers are proven. | **WIP, unclaimed**; the 0x4c8-byte branch graph and repeated 0x8006d694 data reference still need a complete owner and source model. |
| `0x80022ca0` | Card-save writer prepares a header, formats player experience/level, captures three images, copies a save payload, checksums it, then writes `bu00:` data. | **WIP, unclaimed**; the 1424-byte stack layout, card-header byte owner at 0x8006d6a4, and save payload call must be modeled together. |
| `0x800144b8` | CD VAB service retries `SsVabTransBodyPartly`, closes failed VABs, advances sectors, and marks the stream slot complete. | **WIP, 94.87342% strict**; 11/11 CFG blocks and all calls agree, but retail retains phase value `1` in `s3` while the probe retains the `-1` sentinel. |
| `0x80016f4c` | CD image-stream service checks the request kind, uploads image rectangles, advances chunks, and seeks/retries through CD control. | **Exact, 100% strict** in the current `game.cd_memory` unit; its prior fuzzy score was stale after shared CD type refinements. |
| `0x800184d0` | Archive open builds a `\\CD\\<name>;1` path, searches its extent, reads sector zero, allocates the offset table, and copies halfwords. | **Exact, 100% strict** in the current `game.cd_memory` unit; its prior stack-frame residue disappeared under the current shared type state. |

The seven card/menu functions remain unclaimed where complete object or data
boundaries are missing. No source or inventory identity was invented for them.
Focused `kf try game.cd_memory` now has 56/57 identical listings, with only
`0x80017608` differing; a fresh strict pass certified the two exact functions
above and relinked 145/145 GAME target units. `0x800144b8` retains its prior
strict WIP score. Global edge-check still stops on the three known, unrelated
TMD/map-object `.rodata` addends. No shared source or config changed in this
batch. The preceding full build already covered the shared CD header state;
no repository tests, bank, or commit were run.
