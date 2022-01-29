
#include "terminal.h"
#include "../arch/x86_64/interrupts.h"
#include "../arch/x86_64/interrupt_handlers.h"
#include "../arch/x86_64/idt.h"
#include "../arch/x86_64/io/io.h"
#include "../arch/x86_64/gdt.h"
#include "../arch/x86_64/memory/memory.h"
#include "../arch/x86_64/memory/Bitmap.h"
#include "../arch/x86_64/memory/paging/PageFrameAllocator.h"
#include "multiboot.h"

#include "format.h"

uint8_t testBuffer[20];

void kernel_main(unsigned long addr)
{
    struct GDTDescriptor gdtDescriptor;

    gdtDescriptor.Size = sizeof(struct GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    InitInterrupts();
    InitIDT();

    terminal_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);

    terminal_clear();

    RemapPIC();

    terminal_printstr("Hello World!\nHello from a new line\n");

    InitializeExceptionHandlers();

    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    CreateHandler((void*)keyboardInterruptHandler, 0x21, IDT_TA_InterruptGate, 0x08);

    asm("sti");

    unsigned int size;

    size = *(unsigned int*)addr;

    terminal_printstr("\n");

    terminal_printstr("MBI size: 0x");

    terminal_printstr(hexToString((uint64_t)size));

    terminal_printstr("\n");

    struct multiboot_tag* tag;

    static uint64_t memSize = 0;


    for (tag = (struct multiboot_tag *) (addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
        terminal_printstr("Detected tag of type: ");
        terminal_printstr(hexToString((uint64_t)tag->type));
        terminal_printstr("\n");

        switch (tag->type)
        {
            case MULTIBOOT_TAG_TYPE_MMAP:
                terminal_printstr("Successfully located memory map tag\n");

                uint64_t memorySize = GetTotalMemSize(tag); // bytes

                terminal_printstr("Total mem size: ");

                terminal_printstr(uint64ToString(memorySize));

                terminal_printstr(" bytes\n");

                multiboot_memory_map_t* mmap;


                for (mmap = ((struct multiboot_tag_mmap *)tag)->entries;
                    (multiboot_uint8_t *)mmap < (multiboot_uint8_t *)tag + tag->size;
                    mmap = (multiboot_memory_map_t *)((unsigned long)mmap + ((struct multiboot_tag_mmap *)tag)->entry_size))
                {
                    terminal_printstr("section ");
                    terminal_printstr("Base Addr: ");
                    terminal_printstr(hexToString(mmap->addr));
                    terminal_printstr(" ");
                    terminal_printstr("Length: ");
                    terminal_printstr(hexToString(mmap->len));
                    terminal_printstr(" ");
                    terminal_printstr("Type: ");
                    if (mmap->type == 1)
                    {
                        terminal_printstr("Available");
                    }
                    else
                    {
                        terminal_printstr("Not available");
                    }
                    terminal_printstr("\n");
                }
                ParseMultibootMemMap(tag);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                struct multiboot_tag_basic_meminfo* t;

                terminal_printstr("Found basic mem info\n");
                terminal_printstr("Mem lower: ");

                terminal_printstr(hexToString((uint64_t)t->mem_lower));

                terminal_printstr("\n");

                terminal_printstr("Mem upper: ");

                terminal_printstr(hexToString((uint64_t)t->mem_upper));

                terminal_printstr("\n");
        }
    }

    while(1);
}
