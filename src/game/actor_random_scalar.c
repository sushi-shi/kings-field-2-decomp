#include <kf/lib/address.h>
#include <kf/lib/types.h>
#include <psyq/libc.h>

ADDRESS(0x800157ac, 0x4c)
s32 func_800157ac(s32 amplitude)
{
    s32 first = rand();
    s32 second = rand();

    return ((first + second) * amplitude) >> 15;
}

ADDRESS(0x800157f8, 0x54)
s32 func_800157f8(s32 amplitude)
{
    s32 first = rand();
    s32 second = rand();

    return ((first + second - 0x8000) * amplitude) >> 15;
}
