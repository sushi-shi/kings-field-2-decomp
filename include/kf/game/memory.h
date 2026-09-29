#ifndef KF_GAME_MEMORY_H
#define KF_GAME_MEMORY_H
#include <kf/lib/types.h>

/* Heap wrapper at 0x8001771c: malloc, or NULL outside the 2 MiB RAM window. */
void *memory_allocate_checked(u32 size);
/* Word-rounded allocation through memory_allocate_checked. */
void *memory_allocate(u32 size);

/* Both return the first source element not copied. */
const u32 *resource_copy_words(u32 *destination, const u32 *source, u32 word_count);
const u16 *resource_copy_halfwords(u16 *destination, const u16 *source, u32 halfword_count);

#endif
