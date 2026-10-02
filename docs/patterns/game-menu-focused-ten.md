# GAME menu focused ten

Ten GAME menu/card units were compiled off-tree with their pinned manifest
profiles and compared by isolated strict objdiff against the existing
safe-delink target objects. Every unit has equal target/candidate `.rel.text`
row counts; no source, inventory, or profile edit was retained.

Eight functions remain WIP:

| Function | Fresh strict text | Unit `.rel.text`, target/candidate | Existing raw/source limit |
| --- | ---: | ---: | --- |
| `0x8001876c` location menu | 96.36646% | 48/48 | Seven-choice control/calls agree; saved values and return schedule differ. |
| `0x8001930c` map preview | 98.26363% | 69/69 | Archive-entry inputs and call set agree; retail uses a larger frame and different saved image register. |
| `0x8001a4f0` item/magic controller | 99.74359% | 28/28 | Twenty-two-block control agrees; initializer index and constant registers differ. |
| `0x8001b554` card browser | 98.478264% | 38/38 | Probe-result register and guard scheduling remain different. |
| `0x8001bf68` card format | 97.12389% | 76/76 | Probe status and write-path schedules differ with the reviewed call set. |
| `0x8001f8b8` preview choice | 99.14365% | 36/36 | Label writes/calls agree; retained arguments and input exit layout differ. |
| `0x8002083c` two-option menu | 99.65882% | 80/80 | Retail uses a 64-byte frame versus the candidate's smaller frame, with no proved extra live object. |
| `0x80023178` card title writer | 93.52941% | 28/28 | Two signed decimal loops and 19/19 blocks agree; header/quotient register roles differ. |

Nine neighboring functions are already strict **100% exact**: `0x800189f0`,
`0x8001c12c`, `menu_draw_two_option`, `0x80020990`, `0x8001e378`,
`0x8001e484`, and the three wait/checksum/event helpers in
`game.memory_card_wait` after `0x80023178`. This pass did not newly close or
bank a function. None of the eight WIPs gained an evidenced wrong field,
call, or referent from the fresh comparison.
