#pragma once
#include <stdbool.h>
#include <stdint.h>

struct PageDirectoryEntry
{
    bool present : 1;
    bool readWrite : 1;
    bool userSupervisor : 1;
    bool writeThrough : 1;
    bool cacheDisabled : 1;
    bool accessed : 1;
    bool ignore0 : 1;
    bool largerPages : 1;
    bool ignore1 : 1;
    
    uint8_t available : 3;
    uint64_t address : 52;
};

struct PageTable
{
    struct PageDirectoryEntry entries[512];
}__attribute__((aligned(0x1000)));