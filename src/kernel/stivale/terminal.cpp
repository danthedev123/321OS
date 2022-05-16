#include <kernel/stivale/terminal.h>
#include <kernel/stivale/stivale_tags.h>
#include <stddef.h>
#include <stdarg.h>

void (*stivale2_write)(const char* buf, size_t size) = NULL;

void terminal_initialize()
{
    struct stivale2_struct_tag_terminal* terminal_tag = GetTags()->term_tag;

    if (terminal_tag == NULL)
    {
        while(1)
        {
            // Halt execution
            asm("hlt");
        }
    }

    void* stivale_write_ptr = (void*)terminal_tag->term_write;
    stivale2_write = (void(*)(const char*,size_t))(stivale_write_ptr);
}

void terminal_delete()
{
    stivale2_write = NULL;
}

void terminal_putchar(char chr)
{
    if (stivale2_write != NULL)
    {
        stivale2_write(&chr, 1);
    }
}

void terminal_printstr(const char* str)
{
    for (size_t t = 0; str[t]; t++)
    {
        terminal_putchar(str[t]);
    }
}
