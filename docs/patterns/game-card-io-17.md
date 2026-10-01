# GAME card I/O: seventeen-function focused verdict

This batch covers the card event, temporary-file probe, directory/read/write,
wait/checksum, and payload-transfer units. Retail disassembly, CFG, callers,
callees, strings, data references, reviewed relocations, adjacent functions,
KF1 save-system source, and current match state were checked for the five
non-exact functions. The KF1 `save_system.c` has the card event and payload
framework, but no matching `firstfile`/`nextfile` directory reader or KF2
experience/title decoder; its different save schema is a lead, not a source
identity for those functions.

| GAME address | Current role | Direct strict verdict |
| --- | --- | --- |
| `0x80022438` | input-release wait | exact, 100% |
| `0x80022468` | card-event initialization | exact, 100% |
| `0x80022550` | card-event shutdown | exact, 100% |
| `0x800225b0` | card service start | exact, 100% |
| `0x800225d8` | card service stop | exact, 100% |
| `0x80022600` | temporary-file probe | exact, 100% |
| `0x800226ec` | scan and sort fifteen `DIRENTRY` records | WIP, 93.60504% |
| `0x800228c8` | read header and decode EXP, level, slot | WIP, 85.15625% |
| `0x80022b48` | card format wrapper | exact, 100% |
| `0x80022b74` | read, checksum, restore with retries | WIP, 93.666664% |
| `0x80022ca0` | create and write title, icons, payload | WIP, 95.896774% |
| `0x80023178` | write player digits into Shift-JIS title | WIP, 93.52941% |
| `0x80023288` | payload byte sum | exact, 100% |
| `0x800232ac` | card-event wait | exact, 100% |
| `0x8002332c` | clear card events | exact, 100% |
| `0x80048d24` | serialize payload | exact, 100% |
| `0x800492dc` | deserialize payload | exact, 100% |

The focused direct objdiff across these five units reports **12/17 exact**
functions, 6,884 code bytes, and **337/337 initialized data bytes exact**.
The 0x800226ec and 0x800228c8 retail CFGs agree with current source at 13/13
and 24/24 blocks. Their first real differences are signed byte-load selection
and seed/loop scheduling; the two retail slot-seed bytes at 0x8006d6a4/5
remain separate candidate data identities without a proved original owner.
They are separated from the `memory_card_loaded_slot` byte at 0x8006d6a0
by three unclassified bytes and from the `bu00:*` wildcard at 0x8006d6a8
by two more. All three card functions load each seed byte directly with
`lb`, while only the directory scan addresses the wildcard. This proves
the byte values and uses, but neither one original containing object nor a
defining translation unit; combining them with an adjacent global would
claim more than the retail evidence supports.
The read/retry path at 0x80022b74 has the same nine CFG blocks and referents,
but retail uses an 80-byte frame and another saved-register assignment. The
digit writer at 0x80023178 has 19/19 CFG blocks and the same signed divisions;
its argument and quotient registers differ.

Retail 0x80022ca0 copies the 0x40-byte title from `memory_card_assets`, calls
the digit writer, then loads one 32-byte palette from
`memory_card_assets + 0x20 + slot * 0x20`. The source's
`icon_palette[slot - 1]` denotes that same address because the palette array
begins at offset 0x40. Its card-header fields, three `StoreImage` calls,
0x4000-byte buffer clear, payload serializer, checksum, conditional create,
and final write agree with the raw call sequence. The remaining mismatch
starts in slot-seed initialization, zero-fill, and saved-register scheduling;
the existing `present = 0` source-order probe did not move the compiled
listing. No new source edit is justified by this pass.

This verdict used only five targeted unit rebuilds, direct per-unit objdiff,
and focused `kf try` comparisons. It did not run repository tests, lint, a
full linked build, or a broad match. Earlier unsuccessful source-only probes
and the unresolved two-byte data ownership are detailed in
`kf2-game-menu-card-followup.md`.
