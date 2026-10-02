# GAME CD stream and archive storage: 26 strict controls

On 2026-10-02, `game.cd_memory` was freshly compiled in isolation with its
current `probe-gcc257-o2-g0` manifest profile and compared directly with the
delinked GAME object using strict `objdiff-cli diff`. This related cohort is
the contiguous 25-function request/archive run from `0x80017804` through
`0x800182d0`, plus the earlier direct `cd_stream_work_buffer` consumer
`cd_map_stream_read`. **All 26 selected functions are strict 100%.** The
whole unit contains 57 function claims: 56 exact and only the unrelated
`memory_arena_allocate_block` at 99.78261% WIP.

| GAME VA | Function | Verdict |
| --- | --- | --- |
| `80016ee0` | `cd_map_stream_read` | 100% |
| `80017804` | `cd_wait_two_vsyncs` | 100% |
| `80017864` | `cd_request_advance` | 100% |
| `800178d0` | `cd_complete_handler` | 100% |
| `80017a1c` | `cd_data_ready_handler` | 100% |
| `80017a98` | `cd_error_handler` | 100% |
| `80017aa0` | `cd_bcd_to_int` | 100% |
| `80017ac0` | `cd_int_to_bcd` | 100% |
| `80017aec` | `cd_location_to_sector` | 100% |
| `80017b6c` | `cd_sector_to_location` | 100% |
| `80017bf0` | `cd_location_add` | 100% |
| `80017c2c` | `cd_request_wait_idle` | 100% |
| `80017ca8` | `cd_request_wait_done` | 100% |
| `80017d00` | `cd_sectors_corrupt` | 100% |
| `80017d54` | `cd_request_enqueue` | 100% |
| `80017e6c` | `cd_archive_entry_extent` | 100% |
| `80017ef4` | `cd_archive_queue_read` | 100% |
| `80017f48` | `cd_archive_queue_read_kind_20` | 100% |
| `80017f9c` | `cd_archive_queue_stream_read` | 100% |
| `80017ff0` | `cd_archive_read_chunked` | 100% |
| `800180b4` | `cd_archive_entry_size` | 100% |
| `800180f8` | `cd_report_error` | 100% |
| `80018100` | `cd_read_sectors` | 100% |
| `80018240` | `cd_extent_load` | 100% |
| `80018298` | `cd_extent_read_into` | 100% |
| `800182d0` | `cd_archive_read` | 100% |

The target and candidate each have 263 relocation tuples in identical order,
including site, type and referent. The unit's 11-byte `.data` matches
exactly: `cd_path_prefix` is five bytes at offset zero and
`cd_version_suffix` is three bytes at offset eight. Its 33-byte `.rodata`
also matches exactly. These full-unit controls support the selected source
calls and data references independently of the unit's remaining allocator
register residue.

The target models contiguous global BSS objects `cd_state` (676 bytes at
GAME `0x801b5d60`) and `cd_archives` (96 bytes at `0x801b6004`), totaling
772 bytes and ending at `0x801b6064`. The typed state contains event and
frame words followed by a 16-record request ring of 40-byte records and its
tail/current pointers; exact request and initialization functions address
those fields and compare against the ring end. Eight 12-byte archive records
account for the complete archive extent. The fresh candidate emits global
COMMON requests of 680 and 96 bytes, with no `.bss` section. The target's
section and exact extents are curated delinker placement, while the candidate
request's four-byte state rounding is a probe property. Neither object
comparison establishes the historical declaration or defining TU.

`cd_stream_work_buffer` is directly constructed at GAME `0x801b6064`,
immediately after the two modeled BSS objects, by exact
`cd_map_stream_read` and by the audio VAB callback. The resource-transition
consumer reads `0x3e80` words from `buffer + 4`, proving a readable prefix
of at least `0xfa04` bytes. Archive reads use `0x800`-byte sectors; a stream
that supplies that whole prefix requires at least `0x10000` bytes of loaded
input after sector rounding. The next independently addressed BSS datum is
at `0x801c7068`, `0x11004` bytes from the work-buffer base. This is a
placement/capacity interval, not a proven complete C array: the archive entry
maximum, source object split and allocation/linker mechanism are unknown.
The existing address-only inventory anchor remains unpromoted. No source,
identity, relocation, or compiler-profile change follows from this audit,
and no linked-EXE equality is claimed.
