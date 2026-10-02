# GAME SDK pointer band: 25 address-only targets

These 25 `GAME.EXE` load-image word positions at `0x8006d4d0..0x8006d5e0`
are a bounded pointer/storage audit. The words are decoded from the
hash-pinned retail image. A pointer-shaped word in linked data is only a
`mips32_candidate` until its original relocation and containing object
are independently proved; the same applies to candidate HI16/LO16
references into this band.

| GAME address | Retail word | Direct evidence and verdict |
| --- | --- | --- |
| `0x8006d4d0` | `0x80013378` | Targets the retail `$Id: intr.c,v 1.52 1995/03/14 12:20:44 hakama Exp $` bytes; LIBETC source context, object boundary WIP. |
| `0x8006d4d4` | `0` | No curated direct reference; no independent object inferred. |
| `0x8006d4d8` | `0x8005eb68` | Targets exact LIBETC `setIntCallback`. |
| `0x8006d4dc` | `0x8005e948` | Targets exact LIBETC `intInit`. |
| `0x8006d4e0` | `0x8005ec14` | Inside exact LIBETC INTR object text; individual function identity unresolved. |
| `0x8006d4e4` | `0` | No curated direct reference. |
| `0x8006d4e8` | `0` | Two candidate HI/LO references inside exact LIBETC functions. |
| `0x8006d50c` | `0` | No curated direct reference. |
| `0x8006d510` | `0` | One exact LIBETC and one unclassified candidate HI/LO reference. |
| `0x8006d514` | `0` | Two unclassified candidate HI/LO references. |
| `0x8006d518` | `0` | No curated direct reference. |
| `0x8006d528` | `0` | No curated direct reference. |
| `0x8006d52c` | `0` | Two candidate HI/LO references inside exact LIBETC functions. |
| `0x8006d568` | `0x1f801070` | One exact LIBAPI and one unclassified candidate HI/LO reference. |
| `0x8006d56c` | `0x1f801100` | Three exact LIBAPI and one unclassified candidate HI/LO reference. |
| `0x8006d5bc` | `0x8006d588` | Reached by the candidate raw pointer word at `0x8006d8c4`; nested data relationship, extent WIP. |
| `0x8006d5c0` | `0x80061188` | Points into exact LIBGPU text. |
| `0x8006d5c4` | `0x800611ac` | Points into exact LIBGPU text. |
| `0x8006d5c8` | `0x8006089c` | Inside exact LIBGPU SYS object text; individual function identity unresolved. |
| `0x8006d5cc` | `0x80061060` | Points into exact LIBGPU text. |
| `0x8006d5d0` | `0x800610b8` | Points into exact LIBGPU text. |
| `0x8006d5d4` | `0x80061104` | Points into exact LIBGPU text. |
| `0x8006d5d8` | `0x80060d94` | Inside exact LIBGPU SYS object text; individual function identity unresolved. |
| `0x8006d5dc` | `0x80060ae4` | Inside exact LIBGPU SYS object text; individual function identity unresolved. |
| `0x8006d5e0` | `0x80061398` | Points into exact LIBGPU text. |

The pinned `LIBETC.LIB` INTR member itself establishes the first range:
its `.data` emits a 24-byte fragment with pointer patches at +0 to its
52-byte `.rdata` revision string, and at +8/+0xc/+0x10 to its exact
`.text` base `0x8005e848` plus `0x320`/`0x100`/`0x3cc`. The four linked
retail words agree exactly. A later 44-byte zero fragment resumes the
same archive `.data` section; the complete section is `0x44` bytes,
but that does not prove one original C global. The six Ghidra four-byte
data rows covering only the proved first `0x18` fragment were
consolidated into one address-only `DAT_8006d4d0` inventory row of
type `u32[6]`. Its original C object boundaries and linkage stay WIP;
the adjacent 44-byte continuation retains its existing rows.

All nine later function pointers lie
within the exact LIBGPU SYS object text (`0x8005f4a0..0x80061a58`),
even where a per-function vendored identity is not curated. Those exact
text-section matches do not prove the defining data object or its extent;
the pointer table continues beyond `0x8006d5e0`.
The nested `0x8006d8c4` to `0x8006d5bc` relationship does not prove
either raw-word candidate is an original relocation. A safe one-VA
carve of exact vendored `ResetCallback` `0x8005e848` loaded the revised
manifest and withheld no function or relocation; its direct pair to
`DAT_8006d894` remained admitted. That focused carve does not validate
the candidate raw pointer at `0x8006d894` into the consolidated range.
No game source, relocation status, or original TU was changed.
