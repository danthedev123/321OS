
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
#include <kernel/drivers/serial/serial.h>

#include <kernel/format.h>

void kernel_main(struct stivale2_struct* stivale2_struct)
{
    struct GDTDescriptor gdtDescriptor;

    init_tss();

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

    InitializeSerial();
 
    while(1);
}