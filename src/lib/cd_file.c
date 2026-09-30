#include <kf/lib/address.h>
#include <kf/lib/cd_file.h>
#include <psyq/cd.h>
#include <psyq/libc.h>

enum {
    KF_CD_SECTOR_BYTES = 0x800,
    KF_CD_SECTOR_WORDS = 512,
    KF_CD_SECTOR_SHIFT = 11
};

DATA(0x8003dbb0, 0x5)
char cd_path_prefix[5] = "\\OP\\";
DATA(0x8003dbb8, 0x3)
char cd_version_suffix[3] = ";1";

/* Reads \OP\<relative_path>;1 sector by sector into DESTINATION. */
ADDRESS(0x80013284, 0x15c)
int cd_file_load_into(u_long *destination, const char *relative_path)
{
    char path[40];
    CdlFILE file;
    CdlLOC start;
    u_char mode;
    s32 sectors;

    start.minute = 0;
    start.second = 2;
    start.sector = 0;
    start.track = 0;
    memcpy(path, cd_path_prefix, sizeof cd_path_prefix);
    strcat(path, relative_path);
    strcat(path, cd_version_suffix);
    if (CdSearchFile(&file, path) == 0) {
        return KF_CD_NOT_FOUND;
    }
    mode = CdlModeSpeed;
    sectors = (file.size + KF_CD_SECTOR_BYTES - 1) >> KF_CD_SECTOR_SHIFT;
    CdControl(CdlSetmode, &mode, 0);
    CdControl(CdlReadN, (u_char *)&file.pos, 0);
    while (--sectors != -1) {
        if (CdReady(0, 0) != CdlDataReady) {
            CdControl(CdlSetloc, (u_char *)&start, 0);
            CdPause();
            return KF_CD_READ_FAILED;
        }
        CdGetSector(destination, KF_CD_SECTOR_WORDS);
        destination += KF_CD_SECTOR_WORDS;
    }
    CdControl(CdlSetloc, (u_char *)&start, 0);
    CdPause();
    return KF_CD_LOADED;
}
