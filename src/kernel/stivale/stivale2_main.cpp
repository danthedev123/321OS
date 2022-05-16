#include <stdint.h>
#include <kernel/stivale/stivale2.h>
#include <stddef.h>
#include <kernel/stivale/stivale_tags.h>
#include <stdbool.h>
#include "../../arch/x86_64/memory/memory.h"
#include <kernel/stivale/terminal.h>

static uint8_t stack[8192];

static struct stivale2_header_tag_terminal terminal_hdr_tag =
{
    // stivale2 tags begin with an identifier and a pointer to the next tag in the linked list
    .tag =
    {
        // identifier constant defined in stivale2.h
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        // next = 0 (end of linked list of header tags)
        .next = 0
    },

    .flags = 0,
};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag =
{
    .tag =
    {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&terminal_hdr_tag
    },
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0,
};

__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr =
{
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    // Beginning of linked list
    .tags = (uintptr_t)&framebuffer_hdr_tag
};

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
            .type = static_cast<EntryType>(MEM_TYPE_USABLE ? stivale_mmap_entry->type == STIVALE2_MMAP_USABLE : MEM_TYPE_UNUSABLE)
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
        stivale2_tags.rsdp = (struct stivale2_struct_tag_rsdp*) stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_RSDP_ID);


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
    struct stivale2_tag *current_tag = (struct stivale2_tag *)stivale2_struct->tags;
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
        current_tag = (struct stivale2_tag *)current_tag->next;
    }
}