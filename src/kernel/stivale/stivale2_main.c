#include "stivale2_main.h"
#include <stdint.h>
#include "../stivale2.h"
#include <stddef.h>
#include "stivale_tags.h"
#include <bool.h>

bool Tags_Initialized = false;

static struct stivale_tags stivale2_tags = {0};

struct stivale_tags* GetTagsStructure(struct stivale2_struct* stivale2_struct)
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
 