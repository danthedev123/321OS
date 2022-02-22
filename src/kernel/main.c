
//#include "terminal.h"
#include "../arch/x86_64/interrupts.h"
#include "../arch/x86_64/interrupt_handlers.h"
#include "../arch/x86_64/idt.h"
#include "../arch/x86_64/io/io.h"
#include "../arch/x86_64/gdt.h"
//#include "../arch/x86_64/memory/memory.h"
#include "../arch/x86_64/memory/Bitmap.h"
//#include "../arch/x86_64/memory/paging/PageFrameAllocator.h"
#include "../arch/x86_64/memory/paging/mem_frame.h"
#include <kernel/stivale/stivale2.h>
#include <kernel/stivale/stivale_tags.h>
#include <kernel/stivale/terminal.h>

#include <kernel/format.h>

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

void kernel_main(struct stivale2_struct* stivale2_struct)
{
    struct GDTDescriptor gdtDescriptor;

    gdtDescriptor.Size = sizeof(struct GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    // Initialize tags struct, terminal and memory map
    stivale_init(stivale2_struct);

    //InitializePageFrameAllocator();


    InitInterrupts();
    InitIDT();

    //terminal_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);


    RemapPIC();

    //terminal_printstr("Hello World!\nHello from a new line\n");

    InitializeExceptionHandlers();

    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    asm("sti");

    CreateHandler((void*)keyboardInterruptHandler, 0x21, IDT_TA_InterruptGate, 0x28);

    PageFrameInitialize();


    while(1);
}