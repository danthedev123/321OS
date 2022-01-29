#include "PageFrameAllocator.h"
#include <stdint.h>
#include <bool.h>

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;

uint64_t memEnd = 0x0;

bool Initialized = false;

void InitializePageFrameAllocator(struct multiboot_tag* mmap_tag)
{
    if (Initialized) return;

    Initialized = true;

    multiboot_memory_map_t* mmap;

    struct multiboot_tag_mmap* tag = (struct multiboot_tag_mmap*)mmap_tag;

    for (mmap = tag->entries;
        (multiboot_uint8_t*)mmap < (multiboot_uint8_t*)mmap_tag + mmap_tag->size;
        mmap = (multiboot_memory_map_t*) ((unsigned long) mmap
        + tag->entry_size))
    {
        memEnd += (uint64_t)mmap->len;
    }


}