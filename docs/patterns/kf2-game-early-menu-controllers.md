# GAME early-menu controller pass

This 25-function GAME campaign follows proven calls from `0x8001876c`
through list, choice, and memory-card menus. For every address, I inspected
`kf sema --image game` address, block disassembly, xrefs, callees, strings,
and current match state. Call connectivity defines the campaign, not the
original translation-unit boundaries. No member was identified as vendored.

| Address | Retail evidence | Verdict |
| --- | --- | --- |
| `0x8001876c` | 34-block menu controller calls frame/window/input helpers and `0x800189f0` | WIP: controller state and indirect jump |
| `0x800189f0` | draws the player's location-derived number using menu glyph row | **exact, 216/216 bytes** |
| `0x80018ac8` | item-menu controller calls list init, item-model load, and input handlers | WIP: list/model lifetime |
| `0x80018d08` | table selector used by the item and list controllers | WIP: `0x80064c30` loaded-table extent |
| `0x80018dec` | table selector used by `0x8001a898` | WIP: same loaded-table owner |
| `0x80018f8c` | player menu calls four clamp/recalculate helpers | WIP: larger controller and state transitions |
| `0x80019240` | clamps three signed paired player values | **exact, 108/108 bytes** |
| `0x800192ac` | clamps one signed paired player value | **exact, 48/48 bytes** |
| `0x800192dc` | clamps one signed paired player value | **exact, 48/48 bytes** |
| `0x8001930c` | archive/TIM menu setup calls both primitive-buffer helpers | WIP: resource lifetime and stack record |
| `0x80019834` | list controller calls `0x800199d0`, list init, and frame renderer | WIP: output-record ownership |
| `0x800199d0` | copies selected 24-byte glyph rows from `0x80065770` | WIP: loaded source table extent |
| `0x80019ac4` | controller calls `0x80019ce4` and two list branches | WIP: record/table ownership |
| `0x80019ce4` | prepares menu rows from two 24-byte loaded tables | WIP: source table extents |
| `0x80019ed4` | selection controller with direct menu calls and indirect dispatch | WIP: dispatch target and state |
| `0x8001a2f4` | list controller calls `0x800199d0` | WIP: copied-row owner |
| `0x8001a4f0` | item/list controller calls both table helpers | WIP: record and model lifetime |
| `0x8001a7fc` | waits on input, cues sound, redraws twice | **existing exact, 156/156 bytes** |
| `0x8001a898` | menu loop calls `0x80018dec`, list init, and item-model load | WIP: table/model owner |
| `0x8001aa9c` | card-menu controller calls choice screen and card flow | WIP: caller state |
| `0x8001ac80` | card startup, row builder, panel, and input calls | WIP: card record model |
| `0x8001af30` | probes fifteen 40-byte card entries, appending glyph and value streams | **existing exact, 256/256 bytes** |
| `0x8001b030` | draws a two-frame card panel with six detail arguments | **existing exact, 284/284 bytes** |
| `0x8001b14c` | polls a two-choice card prompt, draws two glyph labels for two frames | **exact, 400/400 bytes** |
| `0x8001b2dc` | input/frame controller with sound cues and cursor state | WIP: state and animation flow |

The four new strict functions use typed `player_state` and menu glyph fields.
The location renderer's seven direct call/data references and the clamp
helpers' ten player/call references were reviewed. Focused `kf try` listings
were identical; strict `kf match` reported 100% and 166/166 GAME target
relinks. The full command still exits at repository-wide known-reference data
ownership closure, not at these units.

The `0x8001b14c` source is merged with adjacent exact `0x8001b030` in one
contiguous card-panel unit. Its two 28-byte glyph strings are initialized by
field, preserving the retail stack shape without an unsupported `.rodata`
copy. Candidate target relocations for its nine direct calls/jumps and three
cursor-frame pairs were reviewed against decoded instructions. Focused `kf try`
reported both listings SAME; strict objdiff reports 684/684 code bytes and
2/2 functions exact, with 162/162 GAME target relinks. The global check still
exits at known-reference data ownership closure.
