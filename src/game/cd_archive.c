#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/cd.h>
#include <kf/game/memory.h>
#include <psyq/kernel.h>
#include <psyq/libc.h>

enum {
    CD_READ_ATTEMPTS = 3,
    CD_READ_SYNC_POLL = 1,
    CD_PATH_BYTES = 64
};

RODATA(0x80011074, 0x21)

DATA(0x8006d680, 0x5)
char cd_path_prefix[5] = "\\CD\\";
DATA(0x8006d688, 0x3)
char cd_version_suffix[3] = ";1";

ADDRESS(0x800180b4, 0x44)
u32 cd_archive_entry_size(u16 slot, u16 entry)
{
    u16 *offsets = cd_archives[slot].sector_offsets;
    u32 start = offsets[entry];
    u32 end = offsets[entry + 1];

    return (end - start) << KF_CD_SECTOR_SHIFT;
}

/* Error hook left empty in retail. */
ADDRESS(0x800180f8, 0x8)
void cd_report_error(s32 code)
{
}

/* Reads SECTOR_COUNT sectors synchronously, with the request queue idle and
 * its CD events disabled. */
ADDRESS(0x80018100, 0x140)
void cd_read_sectors(CdlLOC *location, u_long *destination, s32 sector_count)
{
    s32 attempt = 0;
    s32 failed = 1;
    s32 result;

    cd_request_wait_idle();
    DisableEvent(cd_error_event);
    DisableEvent(cd_complete_event);
    DisableEvent(cd_data_ready_event);
    CdControl(CdlPause, NULL, NULL);
    for (; attempt < CD_READ_ATTEMPTS; attempt++) {
        CdControl(CdlSetloc, (u_char *)location, NULL);
        CdRead(sector_count, destination, CdlModeSpeed);
        while ((result = CdReadSync(CD_READ_SYNC_POLL, NULL)) > 0) {
        }
        CdControl(CdlPause, NULL, NULL);
        if (result == 0) {
            failed = 0;
            break;
        }
    }
    EnableEvent(cd_error_event);
    EnableEvent(cd_complete_event);
    EnableEvent(cd_data_ready_event);
    if (failed == 1) {
        cd_report_error(KF_CD_ERROR_READ);
    }
}

ADDRESS(0x80018240, 0x58)
u8 *cd_extent_load(KfCdExtent *extent)
{
    s32 size = (extent->size + KF_CD_SECTOR_BYTES - 1) & -KF_CD_SECTOR_BYTES;
    u8 *data = (u8 *)memory_allocate(size);

    cd_read_sectors(&extent->pos, (u_long *)data, size >> KF_CD_SECTOR_SHIFT);
    return data;
}

/* Reads SIZE bytes of the extent, or all of it when SIZE is zero. */
ADDRESS(0x80018298, 0x38)
void cd_extent_read_into(u_long *destination, KfCdExtent *extent, u32 size)
{
    if (size == 0) {
        size = extent->size;
    }
    cd_read_sectors(&extent->pos, destination,
        (size + KF_CD_SECTOR_BYTES - 1) >> KF_CD_SECTOR_SHIFT);
}

/* Reads archive entry ENTRY until its checksum holds. */
ADDRESS(0x800182d0, 0x94)
void cd_archive_read(u16 slot, u16 entry, u_long *destination)
{
    KfCdArchive *archive = &cd_archives[slot];
    u16 *offsets = archive->sector_offsets;
    s32 sector_count = offsets[entry + 1] - offsets[entry];
    CdlLOC location;

    cd_location_add(&archive->extent.pos, offsets[entry], &location);
    do {
        cd_read_sectors(&location, destination, sector_count);
    } while (cd_sectors_corrupt((u32 *)destination, sector_count));
}

/* Loads \CD\<name>;1 into a fresh allocation. */
ADDRESS(0x80018364, 0xb4)
u8 *cd_file_load(const char *name)
{
    char path[CD_PATH_BYTES];
    CdlFILE file;
    u32 size;
    u8 *data;

    memcpy(path, cd_path_prefix, sizeof cd_path_prefix);
    strcat(path, name);
    strcat(path, cd_version_suffix);
    cd_request_wait_idle();
    if (CdSearchFile(&file, path) == NULL) {
        cd_report_error(KF_CD_ERROR_SEARCH);
    }
    size = (file.size + KF_CD_SECTOR_BYTES - 1) & -KF_CD_SECTOR_BYTES;
    data = (u8 *)memory_allocate(size);
    cd_read_sectors(&file.pos, (u_long *)data, size >> KF_CD_SECTOR_SHIFT);
    return data;
}

/* Loads SIZE bytes of \CD\<name>;1, or all of it when SIZE is zero. */
ADDRESS(0x80018418, 0xb8)
s32 cd_file_load_into(u_long *destination, const char *name, u32 size)
{
    char path[CD_PATH_BYTES];
    CdlFILE file;
    /* Unused: the OPEN/END cd_file_load_into seeks back to this location
     * itself; this copy reads through cd_read_sectors but keeps the local,
     * and retail keeps its frame slot. */
    CdlLOC start;

    memcpy(path, cd_path_prefix, sizeof cd_path_prefix);
    strcat(path, name);
    strcat(path, cd_version_suffix);
    cd_request_wait_idle();
    if (CdSearchFile(&file, path) == NULL) {
        cd_report_error(KF_CD_ERROR_SEARCH);
    }
    if (size == 0) {
        size = file.size;
    }
    cd_read_sectors(&file.pos, destination,
        (size + KF_CD_SECTOR_BYTES - 1) >> KF_CD_SECTOR_SHIFT);
    return 0;
}

/* Opens \CD\<name>;1 as archive SLOT: keeps its extent and copies the
 * sector-offset table out of sector 0. */
ADDRESS(0x800184d0, 0xf0)
void cd_archive_open(u16 slot, const char *name)
{
    u16 table[KF_CD_SECTOR_BYTES / 2];
    CdlFILE file;
    char path[CD_PATH_BYTES];
    KfCdArchive *archive = &cd_archives[slot];

    memcpy(path, cd_path_prefix, sizeof cd_path_prefix);
    strcat(path, name);
    strcat(path, cd_version_suffix);
    if (CdSearchFile(&file, path) == NULL) {
        printf("CdSearchFile error  FileName=%s\n", path);
    }
    archive->extent.pos = file.pos;
    archive->extent.size = file.size;
    cd_extent_read_into((u_long *)table, &archive->extent, KF_CD_SECTOR_BYTES);
    archive->sector_offsets = (u16 *)memory_allocate((table[0] + 1) * 2);
    resource_copy_halfwords(archive->sector_offsets, &table[1], table[0] + 1);
}
