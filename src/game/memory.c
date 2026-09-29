#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/cd.h>
#include <kf/game/memory.h>
#include <psyq/libc.h>
#include <psyq/sdk.h>

enum {
    MEMORY_RAM_BYTES = 0x200000
};

/* KSEG0 base of main RAM, which malloc results must fall within. */
#define MEMORY_RAM_BASE 0x80000000u

#define MEMORY_BLOCK(data) ((KfMemoryBlock *)(data) - 1)

ADDRESS(0x800176c0, 0x20)
void memory_block_release(u8 *data)
{
    memory_arena_free(MEMORY_BLOCK(data));
}

ADDRESS(0x800176e0, 0x8)
void memory_block_set_kind(u8 *data, u8 kind)
{
    MEMORY_BLOCK(data)->kind = kind;
}

ADDRESS(0x800176e8, 0xc)
u8 memory_block_kind(u8 *data)
{
    return MEMORY_BLOCK(data)->kind;
}

ADDRESS(0x800176f4, 0x8)
void memory_block_set_flags(u8 *data, u8 flags)
{
    MEMORY_BLOCK(data)->flags = flags;
}

ADDRESS(0x800176fc, 0xc)
u8 memory_block_flags(u8 *data)
{
    return MEMORY_BLOCK(data)->flags;
}

ADDRESS(0x80017708, 0x8)
void memory_block_set_tag(u8 *data, u16 tag)
{
    MEMORY_BLOCK(data)->tag = tag;
}

ADDRESS(0x80017710, 0xc)
u16 memory_block_tag(u8 *data)
{
    return MEMORY_BLOCK(data)->tag;
}

ADDRESS(0x8001771c, 0x38)
u8 *memory_malloc_checked(u32 size)
{
    u8 *block = malloc(size);

    if ((u32)block + MEMORY_RAM_BASE > MEMORY_RAM_BYTES - 1) {
        return NULL;
    }
    return block;
}

ADDRESS(0x80017754, 0x28)
u8 *memory_allocate(u32 size)
{
    return memory_malloc_checked((size + 3) & ~3);
}

ADDRESS(0x8001777c, 0x20)
void memory_free(u8 *data)
{
    free(data);
}

/* Services the VAB and stream requests at the head of the CD queue, then
 * waits for the GPU and the next VSync. */
ADDRESS(0x8001779c, 0x38)
void cd_request_yield(void)
{
    cd_request_service_vab();
    cd_request_service_stream();
    DrawSync(0);
    VSync(0);
}
