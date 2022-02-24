#include <stdint.h>
#include <kernel/stivale/stivale2.h>
#include <stddef.h>
#include <kernel/stivale/stivale_tags.h>
#include <stdbool.h>
#include "../../arch/x86_64/memory/memory.h"
#include <kernel/stivale/terminal.h>

bool Tags_Initialized = false;

static struct stivale_tags stivale2_tags = {0};

void stivale_initialize_memorymap(struct stivale2_struct* stivale_struct, struct MemoryMap* mmap)
{
    struct stivale2_struct_tag_memmap* stivale_memmap = (struct stivale2_struct_tag_memmap*) GetTags()->memory_map_tag;

    size_t total_entries = stivale_memmap->entries;

    for (size_t i = 0; i < total_entries; i++)
    {
        struct stivale2_mmap_entry* stivale_mmap_entry = stivale_memmap->memmap + i;
        mmap->entries[i] = (struct mmapEntry)
        {
            .address = (void*)stivale_mmap_entry->base,
            .num_pages = NEAREST_PAGE(stivale_mmap_entry->length),
            // If the memory type is not usable, return unusable
            .type = MEM_TYPE_USABLE ? stivale_mmap_entry->type == STIVALE2_MMAP_USABLE : MEM_TYPE_UNUSABLE
        };

    }

    mmap->total_entries_num = total_entries;
}


void stivale_init(struct stivale2_struct* stivale_struct)
{
    InitializeTagsStructure(stivale_struct);

    terminal_initialize();

    stivale_initialize_memorymap(stivale_struct, stivale2_tags.mmap);

}

struct stivale_tags* InitializeTagsStructure(struct stivale2_struct* stivale2_struct)
{
    if (!Tags_Initialized) // if we have not already created this tags structure
    {
        stivale2_tags.term_tag = (struct stivale2_struct_tag_terminal*) stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);
        stivale2_tags.memory_map_tag = (struct stivale2_struct_tag_memmap*) stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
        stivale2_tags.fb = (struct stivale2_struct_tag_framebuffer*) stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);


        Tags_Initialized = true;

    }

    return &stivale2_tags;
}

struct stivale_tags* GetTags()
{
    // If we have already initialized
    return &stivale2_tags;
}

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        // If the tag pointer is NULL (end of linked list), we did not find
        // the tag. Return NULL to signal this.
        if (current_tag == NULL) {
            return NULL;
        }
 
        // Check whether the identifier matches. If it does, return a pointer
        // to the matching tag.
        if (current_tag->identifier == id) {
            return current_tag;
        }
 
        // Get a pointer to the next tag in the linked list and repeat.
        current_tag = (void *)current_tag->next;
    }
}
 