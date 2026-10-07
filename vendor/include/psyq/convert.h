extern "C" {
#ifndef KF_PSYQ_CONVERT_H
#define KF_PSYQ_CONVERT_H

#define atoi atoi_unprototyped
#include <CONVERT.H>
#undef atoi

extern int atoi(const char *string);

#endif

}
