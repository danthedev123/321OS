#include "page_frame.h"
#include "../../../../kernel/stivale/stivale_tags.h"
#include "../../../../kernel/stivale/terminal.h"


// Total RAM in the system, stored in bytes
static size_t total_ram_size = 0;
// Total RAM size used by the page bitmap or allocated pages
static size_t used_ram_size = 0;
// Total RAM size that is reserved
static size_t reserved_ram_size = 0;

void ReadStivaleMemoryMap()
{
    struct MemoryMap* memoryMap = GetTags()->memory_map_tag;
    size_t num_entries = memoryMap->total_entries_num;

    void* largest_region = NULL;

    for (size_t i = 0; i < num_entries; i++)
    {
        terminal_printstr("Found entry!");   
    }
}