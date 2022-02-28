#pragma once
#include "stivale2.h"
#include "../../../arch/x86_64/memory/memory.h"

struct stivale_tags
{
    struct stivale2_struct_tag_terminal* term_tag;
    struct stivale2_struct_tag_memmap* memory_map_tag;
    struct stivale2_struct_tag_framebuffer* fb;
    struct MemoryMap* mmap;
    struct stivale2_struct_tag_rsdp* rsdp;
};

struct stivale_tags* GetTags();
struct stivale_tags* InitializeTagsStructure(struct stivale2_struct* stivale2_struct);
void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id);
void stivale_init(struct stivale2_struct* stivale_struct);