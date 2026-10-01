# GAME event controller follow-up

Both functions below are sourced C and remain strict WIP. The pinned probe's
focused listings were rebuilt after inspection of each retail body, callers,
callees, referents and neighboring functions. No strings occur in either body.

| Function | Strict / focused verdict | First divergence and retained decision |
| --- | --- | --- |
| `0x800475d8` `func_800475d8` (0x6c0 bytes) | 98.56481% / 89.4% WIP | Retail keeps the selected map template in `$s1`; the probe starts it in `$s0`, changing later saved-register lifetimes and a four-byte join offset. The variadic spawn ID, 120-byte frame, map/player referents, direct call set and paired pose transitions are present. No signature, type or field correction supports forcing that register choice. |
| `0x80047c98` `func_80047c98` (0x660 bytes) | 99.81618% / 97.1% WIP | The retained C places the shared `notify_enqueue(6)` block before linked-object lookup and branches back to it for linked ID `0xff`, matching retail `0x80048150..0x80048190`. Focused CFG is 85/85 blocks and 55/55 branches. Remaining visible differences exchange the view-rotation argument and constant-one saved registers and reverse one commutative `addu` operand order. The final `jalr` callback remains unresolved. |

The caller at `0x80047c98` invokes `0x800475d8` twice with non-null object
pointers. Its other direct calls and typed event/map/player references agree
with retail. These observations do not establish a compiler/register cause or
justify source padding, forced calls, or a guessed indirect target.
