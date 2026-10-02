# GAME LIBGPU OTAG small-data records

The pinned Psy-Q 3.0 `LIBGPU.LIB/OTAG` member emits four successive
`.sdata` Code records. Each independently matches `GAME.EXE` retail:

| GAME extent | Bytes | Contents |
| --- | ---: | --- |
| `0x8006d97c..0x8006d9c2` | 71 | Primitive names from `S16` and `T16` through `FT3` and `F3`, including NUL and alignment bytes |
| `0x8006d9c3..0x8006d9c9` | 7 | Leading zero, `BF`, and `?` labels |
| `0x8006d9ca..0x8006d9d1` | 8 | Leading zeros, newline, and `%5d:` format string |
| `0x8006d9d2..0x8006d9dc` | 11 | Leading zeros, `-%s`, and `-%s\n` format strings |

The three zeros at `0x8006d979..0x8006d97b` align the OTAG records
after the separately pinned TMD label pool. The following three bytes
at `0x8006d9dd..0x8006d9df` are `43 50 45` (`CPE`), outside the
97-byte archived OTAG section; they remain unclassified load-tail
residue before the page-rounded BSS view at `0x8006d9e0`.

The 24 existing candidate raw-word relocations in the already curated
OTAG `.data` record at `0x8006d5fc..0x8006d65b` point into the first
name record, matching all 24 archive section patches. Two candidate
reachable-code pairs in exact vendored `get_p_name` point to `BF` and
`?`. A focused safe one-VA carve of `get_p_name` `0x80063a80` withheld
no function or relocation and resolves those pairs as offsets `+1`
and `+5` into `DAT_8006d9c3`. Three later instruction-word pairs
point into the format-string records. Their relocation tier, and the
24 raw-word tiers, remain candidate. Four address-derived identities
record the exact Code extents without inferring the original C
declarations, linkage, or defining source file.
