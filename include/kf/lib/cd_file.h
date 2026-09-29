#ifndef KF_CD_FILE_H
#define KF_CD_FILE_H
#include <kf/lib/types.h>
#include <sys/types.h>

/* Result codes returned by cd_file_load_into. */
enum {
    KF_CD_LOADED = 0,
    KF_CD_NOT_FOUND = 1,
    KF_CD_READ_FAILED = 3
};

extern char cd_path_prefix[5];
extern char cd_version_suffix[3];

int cd_file_load_into(u_long *destination, const char *relative_path);

#endif
