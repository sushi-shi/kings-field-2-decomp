# GAME SDK-to-card storage boundary: 25-target audit

This `GAME.EXE` review follows 25 relocation target addresses from
`0x8006db30` through `0x8006dc00`. Twenty have curated data identities;
five (`0x8006dbc8`, `0x8006dbd0`, `0x8006dbd8`, `0x8006dbe0`,
`0x8006dbf4`) are interior or still uncurated targets. A target is not an
independent object merely because a `lui`/low instruction names it. All
reference counts below come from the curated relocation *candidate* inventory;
`exact SDK` refers only to the referring text's Psy-Q archive match in
`functions_vendored.tsv`. The load-image candidates through `0x8006dbc0`
contain zero at their listed address, which does not settle allocation size.

| GAME target | Refs | Direct evidence | Owner verdict |
| --- | ---: | --- | --- |
| `0x8006db30` | 4 | Unclassified `0x8004c460..0x8004cce4` sites near LIBCD text. | Address-only, probable SDK context; no archive/object proof. |
| `0x8006db40` | 15 | All in exact `LIBSPU.LIB` text. | Exclude from game source claims. |
| `0x8006db48` | 7 | Six in exact `LIBSPU.LIB`, one at unclassified `0x8004dd40`. | No game claim; archive boundary WIP. |
| `0x8006db50` | 4 | All in exact `LIBSPU.LIB` text. | Exclude from game source claims. |
| `0x8006db58` | 3 | All in exact `LIBSPU.LIB` text. | Exclude from game source claims. |
| `0x8006db60` | 4 | Two in exact `LIBSPU.LIB`, two unclassified `0x8004db60/0x8004de10`. | No game claim; archive boundary WIP. |
| `0x8006db68` | 4 | Two in exact `LIBSPU.LIB`, two unclassified `0x8004dd6c/0x8004ddd0`. | No game claim; archive boundary WIP. |
| `0x8006db70` | 2 | One in exact `LIBSPU.LIB`, one unclassified `0x8004dd74`. | No game claim. |
| `0x8006db78` | 4 | Two in exact `LIBSPU.LIB`, two unclassified `0x8004dd94/0x8004dde0`. | No game claim. |
| `0x8006db80` | 7 | Four in exact `SpuInitMalloc`/`SpuMalloc`/allocation helpers; three unclassified `0x8004fdfc..0x80050164`. | No game claim. |
| `0x8006db88` | 25 | Fourteen in exact `LIBSPU.LIB` malloc/free helpers; eleven unclassified `0x8004fdb4..0x80050514`. | No game claim; do not split the unknown allocation. |
| `0x8006db90` | 3 | Exact `LIBSND.LIB` `SsEnd` and `_SsStart`. | Exclude from game source claims. |
| `0x8006db98` | 2 | Exact `LIBSND.LIB` `SpuVmFlush`. | Exclude from game source claims. |
| `0x8006dba0` | 5 | Two exact `LIBETC.LIB` `VBLNKInit`/`VBLNKStop`, three unclassified `0x8005e71c..0x8005e788`. | No game claim; boundary WIP. |
| `0x8006dba8` | 3 | Exact `LIBETC.LIB` `PadInit`/`PadRead`. | Exclude from game source claims. |
| `0x8006dbb0` | 1 | Exact `LIBETC.LIB` `PadInit`. | Exclude from game source claims. |
| `0x8006dbb8` | 2 | Exact `LIBGPU.LIB` text. | Exclude from game source claims. |
| `0x8006dbc0` | 4 | Exact `LIBGPU.LIB` `OpenTIM`/`ReadTIM`. | Exclude from game source claims. |
| `0x8006dbc8` | 2 | Unclassified `0x80061af8/0x80061b4c`, immediately after the matched TIM readers. | Uncurated target; original SDK object/extent candidate. |
| `0x8006dbd0` | 2 | Unclassified `0x80061ae4/0x80061b54`. | Uncurated SDK-adjacent target; no owner promotion. |
| `0x8006dbd8` | 4 | Unclassified `0x80061af0..0x80061c34`. | Uncurated SDK-adjacent target; no owner promotion. |
| `0x8006dbe0` | 1 | Unclassified store at `0x80061b08`. | Uncurated SDK-adjacent target; no owner promotion. |
| `0x8006dbe8` | 2 | Reviewed GAME `0x80021c8c`/`0x80021e00` references to the beginning of two 12-byte saved primitive-buffer records. | Existing 24-byte BSS claim in `menu_display_state.c` supported; original TU WIP. |
| `0x8006dbf4` | 2 | Reviewed GAME `0x80021c8c`/`0x80021e00` references to `0x8006dbe8 + 12`. | Interior of the same 24-byte BSS object, no new identity. |
| `0x8006dc00` | 1 | Reviewed `memory_card_initialize` stores the buffer's address. | Existing `0x4000`-byte BSS definition in `memory_card_events.c` supported; only the first `0x400` bytes appear in padded retail load-file coverage. |

The first eighteen identity rows remain library-owned or library-adjacent
address-only candidates. The four uncurated `0x8006dbc8..0x8006dbe0`
targets stay unresolved at the end of the SDK-like run. The defensible game
source boundary starts at the separately proven saved-buffer object
`0x8006dbe8`; neither the nearby zero bytes nor a Ghidra split can move it.

The five reviewed GAME direct pairs at `0x80021cc4/cc8`, `0x80021cf0/cf4`,
`0x80021e08/e0c`, `0x80021e34/e38`, and `0x80022530/534` were decoded
from the raw retail words. Each is `lui 0x8007` plus a signed-low `addiu`,
and resolves respectively to `0x8006dbe8`, `0x8006dbf4`, `0x8006dbe8`,
`0x8006dbf4`, and `0x8006dc00`. This confirms the two-record 12-byte stride
and the separate card-buffer base without promoting nearby candidate pairs.

Two isolated strict controls support the existing game claims without
settling their original translation units: `game.menu_display_state` has
`func_80021e00` at 100% and `func_80021c8c` at 99.956985% text, while its
48-byte aggregate `.bss` scores 0%; `game.memory_card_events` has five of
five function texts at 100% and its one-byte `.data` at 100%. Its 16 KiB
card buffer has no separately comparable BSS section in that focused target
object, so the runtime allocation mechanism remains open. No source,
identity, relocation, or vendored-function rows were changed.

Three additional adjacent initialized-data controls were compiled and
strictly compared in isolation: `game.cd_memory` has 56/57 exact function
texts (only `memory_arena_allocate_block` at `0x80017608` remains 99.78261%),
with `.data` 11/11 bytes and `.rodata` 33/33 bytes exact but aggregate
`.bss` 772 bytes at 0%; `game.menu_frame_begin` has two of two texts and
8/8 initialized bytes exact, with its four-byte BSS at 0%; and
`game.menu_item_model` has two of two texts and 28/28 initialized bytes
exact. These are unit-local controls, not full-image allocation proof.
