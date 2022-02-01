#pragma once
#include <stdint.h>
#include <helpers.h>
#include <stddef.h>

#define ROUND_UP_PAGE_SIZE(n) ROUND_UP(n, 4096)
#define NEAREST_PAGE(n) (ROUND_UP_PAGE_SIZE(n) / 4096)

void memset(void* start, uint8_t value, uint64_t num);

enum EntryType
{
    MEM_TYPE_UNUSABLE,
    MEM_TYPE_USABLE
};

struct mmapEntry
{
    void* address;
    size_t num_pages;
    enum EntryType type;
};

struct MemoryMap
{
    struct mmapEntry entries[256];
    size_t total_entries_num;
};