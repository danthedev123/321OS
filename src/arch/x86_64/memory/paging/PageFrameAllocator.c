#include "PageFrameAllocator.h"
#include <stdint.h>
#include <bool.h>
#include <stddef.h>
#include "../Bitmap.h"
#include "../memory.h"
#include "../../../../kernel/terminal.h"
#include "../../../../kernel/format.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;

struct Bitmap bitmap; // page frame allocator bitmap

bool Initialized = false;

void InitializePageFrameAllocator(struct multiboot_tag* tag)
{
    if (Initialized) return;

    Initialized = true;

    multiboot_memory_map_t* mmap;

    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;

    struct multiboot_tag_mmap* mmap_tag = (struct multiboot_tag_mmap*)tag;

    for (mmap = ((struct multiboot_tag_mmap *)mmap_tag)->entries;
         (multiboot_uint8_t *)mmap < (multiboot_uint8_t *)mmap_tag + mmap_tag->size;
         mmap = (multiboot_memory_map_t *)((unsigned long)mmap + ((struct multiboot_tag_mmap *)mmap_tag)->entry_size))
    {
        uintptr_t ptr = (uintptr_t)mmap->addr;

        void* memAddr = (void*)ptr;

        largestFreeMemSeg = memAddr;


        largestFreeMemSegSize = mmap->len;
    }
    uint64_t memSize = GetTotalMemSize(tag);

    uint64_t bitmapSize = memSize / 4096 / 8 + 1;

    bitmap_init(&bitmap, &largestFreeMemSeg, largestFreeMemSegSize);

    for (int i = 0; i < bitmapSize; i++)
    {
        *(uint8_t*)(bitmap.buffer + i) = 0;
    }
}