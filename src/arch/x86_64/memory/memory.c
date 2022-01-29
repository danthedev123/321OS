#include "memory.h"

uint64_t GetTotalMemSize(struct multiboot_tag *memmap_tag)
{
    multiboot_memory_map_t* mmap;

    // In bytes
    uint64_t totalMemSize = 0;

    for (mmap = ((struct multiboot_tag_mmap *)memmap_tag)->entries;
         (multiboot_uint8_t *)mmap < (multiboot_uint8_t *)memmap_tag + memmap_tag->size;
         mmap = (multiboot_memory_map_t *)((unsigned long)mmap + ((struct multiboot_tag_mmap *)memmap_tag)->entry_size))
    {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            totalMemSize += mmap->len;
        };
    }

    return totalMemSize;
};

void memset(void* start, uint8_t value, uint64_t num)
{
    for (uint64_t i = 0; i < num; i++)
    {
        *(uint8_t*)((uint64_t)start+i) = value;
    }
}