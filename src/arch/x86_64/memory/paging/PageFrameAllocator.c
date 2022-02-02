#include "PageFrameAllocator.h"
#include "../../../../kernel/stivale/terminal.h"
#include "../../../../kernel/format.h"

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

static size_t current_index = 0;

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

    pageframe_reserve(0, total_mem_size / 4096);

    for (size_t i = 0; i < total_entries; i++)
    {
        struct mmapEntry* mmap_entry = memorymap->entries + i;

        if (mmap_entry->type != MEM_TYPE_USABLE)
        {
            continue;
        }

        size_t reserved_size = mmap_entry->num_pages * 4096;

        pageframe_unreserve(mmap_entry->address, reserved_size / 4096);
    }

    terminal_printstr("Total RAM: ");
    terminal_printstr(uint64ToString((uint64_t)total_mem_size));
    terminal_printstr("\n");
}

bool pageframe_edit(uint64_t index, bool state)
{
    if (bitmap_get(&pageframebitmap, index) == state)
    {
        // already in requested state
        return true;
    }

    return bitmap_set(&pageframebitmap, index, state);
}

void pageframe_lock(void* physicalAddress, size_t pages)
{
    uint64_t start = (uint64_t)physicalAddress / 4096;

    for (uint64_t i = start; i < start + pages; i++)
    {
        if (pageframe_edit(i, true))
        {
            used_mem_size += 4096;
        }
    }
}

void pageframe_unlock(void* physicalAddress, size_t pages)
{
    uint64_t start = (uint64_t)physicalAddress / 4096;

    for (uint64_t i = start; i < start + pages; i++)
    {
        if (pageframe_edit(i, false))
        {
            if (used_mem_size >= 4096)
            {
                used_mem_size -= 4096;
            }

            if (current_index > i)
            {
                current_index = i;
            }
        }
    }
}

void pageframe_reserve(void* physicalAddress, size_t pages)
{
    uint64_t start = (uint64_t)physicalAddress / 4096;

    for (uint64_t i = start; i < start + pages; i++)
    {
        if (pageframe_edit(i, true))
        {
            reserved_mem_size += 4096;
        }
    }
}

void pageframe_unreserve(void* physicalAddress, size_t pages)
{
    uint64_t start = (uint64_t)physicalAddress / 4096;

    for (uint64_t i = start; i < start + pages; i++)
    {
        if (pageframe_edit(i, false))
        {
            if (reserved_mem_size >= 4096)
            {
                reserved_mem_size -= 4096;
            }

            if (current_index > i)
            {
                current_index = i;
            }
        }
    }
}

size_t get_total_memory_size()
{
    return total_mem_size;
}

size_t get_used_memory_size()
{
    return used_mem_size;
}

size_t get_reserved_memory_size()
{
    return reserved_mem_size;
}
