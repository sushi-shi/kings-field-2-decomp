#include <kf/lib/address.h>
#include <kf/lib/types.h>

ADDRESS(0x8002897c, 0x1c)
s32 func_8002897c(s32 value)
{
    s32 result = 0;
    if (value < 81) {
        result = value >= 71;
    }
    return result;
}
