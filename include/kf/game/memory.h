#ifndef KF_GAME_MEMORY_H
#define KF_GAME_MEMORY_H
#include <kf/lib/types.h>

/* Header in front of every block handed out by the GAME arena allocator. */
typedef struct KfMemoryBlock {
    u8 kind;
    u8 flags;
    u16 tag;
    u32 size;
    u8 **owner;
} KfMemoryBlock;

typedef char kf_memory_block_size[sizeof(KfMemoryBlock) == 12 ? 1 : -1];

void memory_arena_free(KfMemoryBlock *block);
void memory_block_release(u8 *data);
void memory_block_set_kind(u8 *data, u8 kind);
u8 memory_block_kind(u8 *data);
void memory_block_set_flags(u8 *data, u8 flags);
u8 memory_block_flags(u8 *data);
void memory_block_set_tag(u8 *data, u16 tag);
u16 memory_block_tag(u8 *data);
/* malloc, or NULL outside the 2 MiB RAM window. */
u8 *memory_malloc_checked(u32 size);
/* Word-rounded allocation through memory_malloc_checked. */
u8 *memory_allocate(u32 size);
void memory_free(u8 *data);

/* Both return the first source element not copied. */
const u32 *resource_copy_words(u32 *destination, const u32 *source, u32 word_count);
const u16 *resource_copy_halfwords(u16 *destination, const u16 *source, u32 halfword_count);

#endif
