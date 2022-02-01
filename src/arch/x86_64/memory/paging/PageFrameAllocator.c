#include "PageFrameAllocator.h"

// Total memory size in the system stored in bytes
static size_t total_mem_size = 0;
// Amount of memory in bytes that has been used
static size_t used_mem_size = 0;
// Amount of memory in bytes that is reserved
static size_t reserved_mem_size = 0;
// Bitmap of memory space
// 1 = in use
// 0 = free
static struct Bitmap pageframebitmap;

void InitializePageFrameAllocator()
{
    struct MemoryMap* memorymap = GetTags()->mmap;
    size_t total_entries = memorymap->total_entries_num;
    struct mmapEntry* largest_entry = NULL;

    // Find the largest memory region
    for (size_t i = 0; i < total_entries; i++)
    {
        struct mmapEntry* memorymap_entry = memorymap->entries + i;

        if (memorymap_entry->type == MEM_TYPE_UNUSABLE && (largest_entry == NULL || memorymap_entry->num_pages > largest_entry->num_pages))
        {
            largest_entry = memorymap_entry;
        }

        total_mem_size += memorymap_entry->num_pages * 4096;
    }

    bitmap_init(&pageframebitmap, largest_entry->address, total_mem_size / 4096 / SCALE);
}