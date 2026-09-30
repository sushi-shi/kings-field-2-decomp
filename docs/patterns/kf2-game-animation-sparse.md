# GAME animation clip and sparse vertices

These functions form a related GAME.EXE campaign through the calls at
`0x80034180`, `0x800341ec`, `0x8003421c`, and `0x800344a4`/`0x800344fc`.
The clip selector also has a King's Field I source counterpart. The four
source-owned functions below have strict objdiff `100.000000000%` matches.

| GAME VA | Verdict | Retail behavior |
| --- | --- | --- |
| `0x80033b34` | Exact | Returns a keyframe pointer and writes its index and fixed-12 blend fraction. It reads the clip table at asset +16 and reverses a fraction for nonzero keyframe direction. |
| `0x80033bfc` | Exact | Expands encoded vertex positions, copying an eight-byte vertex stride from a base array for `-32768` runs. |
| `0x80033cc0` | Exact | Applies the same sparse stream to existing vertices; `-32768` advances the destination by a signed vertex count. |
| `0x80033d3c` | WIP | The adjacent morph accumulator calls SDK `ScaleMatrix` and accumulates vertex deltas. Its vector and matrix ownership still need a complete source model. |
| `0x80033ff4` | Exact | Looks up one literal vertex in a sparse stream; an index inside a skipped run returns null. |
| `0x800345e4` | Exact | Reads the vertex count from a TMD object under a registered asset. Indices below `0x80` select object zero; other indices select their low seven bits. |

The stream begins with a signed 16-bit record count. The exact probe source
decrements that count before testing against `-1`, matching both the entry
and loop-back branch schedule. In the lookup, loading the count before
incrementing the stream pointer reproduces the retail entry delay slot.
These are observed source shapes under `probe-gcc257-o2-g0`, not proof of the
historical compiler settings. The selector's return pointer is undefined for
an empty clip in the retail instruction stream; no synthetic fallback was
added to the C source.

The TMD vertex-count helper uses the SDK TMD object layout: a 12-byte header,
28-byte object records, and the vertex-count field at object +4. Its internal
`j` at `0x80034620` targets the return block at `0x8003463c`; reviewing that
target as an `R_MIPS_26` referent removed the target-object ambiguity without
altering the C access to the TMD record.
