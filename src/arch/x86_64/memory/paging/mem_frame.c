#include "mem_frame.h"

#include <kernel/stivale/stivale_tags.h>
#include <kernel/stivale/terminal.h>
#include "../Bitmap.h"
#include <stdbool.h>
#include "../memory.h"
#include <stdint.h>
#include <kernel/format.h>
#include "../Bitmap.h"
#include <kernel/utility/kernelLog/logger.h>


#define PAGE_SIZE 4096

// Holds RAM information (total, free, used) in bytes
static size_t total_ram_size = 0;
static size_t used_ram_size = 0;
static size_t reserved_ram_size = 0;

// Holds current index in the page frame bitmap to optimize paging
size_t current_index = 0;
// The page frame allocator bitmap
struct Bitmap bitmap;

bool initialized = false;

bool SetPageFrame(uint64_t index, bool state)
{
    if (bitmap_get(&bitmap, index) == state) return true;
    return bitmap_set(&bitmap, index, state);
}

void LockPages(void* address, uint64_t count)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;

    for (uint64_t i = 0; i < index + count; i++)
    {
        if (SetPageFrame(index, true))
        {
            used_ram_size += PAGE_SIZE;
        }
    }
}

void FreePages(void* address, uint64_t count)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;

    for (uint64_t i = 0; i < index + count; i++)
    {
        if (SetPageFrame(i, false))
        {
            used_ram_size -= PAGE_SIZE;

            if (current_index > i)
            {
                current_index = i;
            }
        }   
    }
}

void ReservePages(void* address, uint64_t count)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;

    for (uint64_t i = 0; i < index + count; i++)
    {
        if (SetPageFrame(i, true))
        {
            reserved_ram_size += PAGE_SIZE;
        }
    }
}

void UnreservePages(void* address, uint64_t count)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;

    for (uint64_t i = 0; i < index + count; i++)
    {
        if (SetPageFrame(i, false))
        {
            reserved_ram_size -= PAGE_SIZE;

            if (current_index > i)
            {
                current_index = i;
            }
        }
    }
}

void* RequestPage()
{
    for (; current_index < bitmap_size(&bitmap); current_index++)
    {
        if (bitmap_get(&bitmap, current_index) == true) continue; // Page is not free so we continue
        // We found a free page!!!!
        void* ret = (void*)(current_index * PAGE_SIZE);
        current_index++;
        LockPages(ret, 1); // Lock the page

        return ret;
    }

    // todo: pagefile on disk so the system doesn't crash after memory is full

    return NULL; // We didn't find a free page :-(
}

void PageFrameInitialize()
{
    struct MemoryMap* memorymap = GetTags()->mmap;

    if (initialized)
    {
        terminal_printstr("WARNING: Frame allocator has already been initialized. Stop.");
        return;
    }

    

    struct mmapEntry* largest_entry = NULL;

    for (size_t i = 0; i < memorymap->total_entries_num; i++)
    {
        struct mmapEntry* mmap_entry = memorymap->entries + i;

        // Skip over unusable memory sections
        if (mmap_entry->type != MEM_TYPE_USABLE) continue;
        
        total_ram_size += mmap_entry->num_pages * PAGE_SIZE;

        if (largest_entry == NULL || mmap_entry->num_pages > largest_entry->num_pages)
        {
            largest_entry = mmap_entry;
        }

    }

    size_t bitmapSize = total_ram_size / PAGE_SIZE / SCALE;


    bitmap_init(&bitmap, largest_entry->address, bitmapSize);

    used_ram_size += bitmapSize;

    // Reserve all pages
    ReservePages(0, total_ram_size / PAGE_SIZE);

    for (size_t i = 0; i < memorymap->total_entries_num; i++)
    {
        struct mmapEntry* entry = memorymap->entries + i;

        if (entry->type != MEM_TYPE_USABLE) continue;

        size_t size = entry->num_pages * PAGE_SIZE;

        UnreservePages(entry->address, size / PAGE_SIZE);
    }

    // Lock the bitmap

    size_t bitmap_pages = NEAREST_PAGE(bitmap.size);

    LockPages(bitmap.buffer, bitmap_pages);

    size_t kernelSize = (size_t)&_KernelEnd - (size_t)&_KernelStart;
    size_t kernelPages = NEAREST_PAGE(kernelSize);

    LockPages(KERNEL_PHYSICAL_ADDR(&_KernelStart), kernelPages);

    ReservePages(0, 256);

    kernelLogSuccess("SUCCESS: Initialized page frame allocator");
}

size_t GetReservedRAMSize()
{
    return reserved_ram_size;
}

size_t GetTotalRAMSize()
{
    return total_ram_size;
}

size_t GetUsedRAMSize()
{
    return used_ram_size;
}