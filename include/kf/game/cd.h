#ifndef KF_GAME_CD_H
#define KF_GAME_CD_H
#include <kf/lib/types.h>
#include <psyq/cd.h>

/* GAME.EXE CD layer: whole-sector file reads and the sector-indexed
 * CD\COM\*.T archives. */
enum {
    KF_CD_SECTOR_BYTES = 0x800,
    KF_CD_SECTOR_SHIFT = 11,
    KF_CD_SECTOR_WORDS = KF_CD_SECTOR_BYTES / 4,
    KF_CD_ARCHIVE_SLOTS = 8,
    /* Seed of the per-entry archive checksum held in each entry's last word. */
    KF_CD_CHECKSUM_SEED = 0x12345678
};

/* cd_report_error codes. */
enum {
    KF_CD_ERROR_SEARCH = 0,
    KF_CD_ERROR_READ = 1
};

/* Location and byte size of a file, the leading fields of CdlFILE. */
typedef struct {
    CdlLOC pos;
    u_long size;
} KfCdExtent;

/* Open .T archive: sector 0's `count + 1` sector offsets and the file extent. */
typedef struct {
    u16 *sector_offsets;
    KfCdExtent extent;
} KfCdArchive;

typedef char kf_cd_archive_size[sizeof(KfCdArchive) == 12 ? 1 : -1];

extern char cd_path_prefix[5];
extern char cd_version_suffix[3];
extern KfCdArchive cd_archives[KF_CD_ARCHIVE_SLOTS];
extern long cd_error_event;
extern long cd_complete_event;
extern long cd_data_ready_event;

s32 cd_bcd_to_int(u8 bcd);
u32 cd_int_to_bcd(u8 value);
u32 cd_location_to_sector(CdlLOC *location);
void cd_sector_to_location(CdlLOC *location, u32 sector);
void cd_location_add(CdlLOC *base, u32 sector_offset, CdlLOC *result);
void cd_request_wait_idle(void);
s32 cd_sectors_corrupt(u32 *data, s32 sector_count);
u32 cd_archive_entry_size(u16 slot, u16 entry);
void cd_report_error(s32 code);
void cd_read_sectors(CdlLOC *location, u_long *destination, s32 sector_count);
u8 *cd_extent_load(KfCdExtent *extent);
void cd_extent_read_into(u_long *destination, KfCdExtent *extent, u32 size);
void cd_archive_read(u16 slot, u16 entry, u_long *destination);
u8 *cd_file_load(const char *name);
s32 cd_file_load_into(u_long *destination, const char *name, u32 size);
void cd_archive_open(u16 slot, const char *name);

#endif
