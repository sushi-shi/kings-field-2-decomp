# GAME menu scalars: 30 direct relocation reviews

The four GAME globals in this pass already have distinct, source-backed
identities: `input_idle_counter` (`0x8006d690`, current C definition in
`menu_sound_cue.c`), `DAT_8006d694` (`0x8006d694`, current C definition in
`menu_item_model.c`), `menu_cursor_animation_frame` (`0x8006d698`), and
`menu_cursor_animation_direction` (`0x8006d69c`, both in
`menu_frame_begin.c`). Their original translation-unit boundaries remain
unproved. The 30 candidate relocation rows below were all inside the four
bounded, source-claimed GAME functions shown. At every site, raw retail bytes
decode as adjacent `lui 0x8007` plus `lw` or `sw`; the low instruction's
base register equals the `lui` destination, and its signed immediate resolves
to the curated scalar address. No word/byte view or control-flow edge was
inferred from proximity.

| Enclosing function | Target and reviewed `lui` sites | Verdict |
| --- | --- | --- |
| `func_8001e484` | `DAT_8006d694`: `0x8001e674`, `0x8001e694`, `0x8001e6a4`, `0x8001e6bc`, `0x8001e6dc`, `0x8001e6ec` (six direct word reads/writes). | Strict text 100%. |
| `func_8001e484` | `input_idle_counter`: `0x8001e748`, `0x8001e770`, `0x8001e798`, `0x8001e7c0`, `0x8001e7e4`, `0x8001e80c`, `0x8001e840`, `0x8001e868`, `0x8001e890`, `0x8001e8b8`, `0x8001e8e0`, `0x8001e920` (twelve direct word stores). | Strict text 100%; source resets the latch along twelve input paths. |
| `func_80020990` | `DAT_8006d694`: `0x80020b04` (one direct word read). | Strict text 100%; numeric heading reads the quantity. |
| `menu_blit_sprite` `0x80020d20` | `menu_cursor_animation_frame`: `0x80020d38` (one direct word read). | Strict text 100%. |
| `menu_frame_begin` `0x80021a68` | `menu_cursor_animation_frame`: `0x80021b4c`, `0x80021b68`, `0x80021b78`, `0x80021b80`, `0x80021b98`, `0x80021bac`, `0x80021bc0` (seven direct word reads/writes). | Strict text 100%. |
| `menu_frame_begin` `0x80021a68` | `menu_cursor_animation_direction`: `0x80021b38`, `0x80021ba4`, `0x80021bc8` (three direct word reads/writes). | Strict text 100%. |

Each row was changed from candidate/`paired-reachable` to
reviewed/`paired-reviewed`, preserving its image, site, partner, target,
opcode, register, and channel. Four sequential `kf-delink --policy safe`
one-VA carves (`0x8001e484`, `0x80020990`, `0x80020d20`, `0x80021a68`)
withheld no functions or relocations; the per-VA used reports contain all
30 reviewed pairs under the correct identity with zero addend. This does
not promote any neighboring candidate site or infer the original datum TU.

An isolated strict comparison after the carve kept all four affected texts
at 100%. The adjacent `menu_draw_two_option` and `func_80021a60` controls
were also 100%; the unrelated `func_8002083c` remained 99.65882% WIP.
`game.menu_frame_begin` kept 8/8 initialized `.data` bytes exact, while
its four-byte BSS section remained 0% because allocation form is unresolved.
No C source or data identity changed. No repository tests, lint, full build,
broad match, or full-image link were run.
