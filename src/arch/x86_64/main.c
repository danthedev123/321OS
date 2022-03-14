#include "../arch/x86_64/interrupts.h"
#include "../arch/x86_64/interrupt_handlers.h"
#include "../arch/x86_64/idt.h"
#include "../arch/x86_64/io/io.h"
#include "../arch/x86_64/gdt.h"
#include "../arch/x86_64/memory/paging/mem_frame.h"
#include <kernel/stivale/stivale2.h>
#include <kernel/stivale/stivale_tags.h>
#include <kernel/stivale/terminal.h>
#include <kernel/drivers/serial/serial.h>
#include <kernel/utility/kernelLog/logger.h>
#include <kernel/format.h>
#include <kernel/lib/string.h>
#include <kernel/drivers/sound/pcspeaker.h>

void kernel_main(struct stivale2_struct* stivale2_struct)
{
    InitializeSerial();

    
    serial_writestring("  ____ ___  __  ____   _____ ");
    serial_writestring("\r\n");
    serial_writestring(" |___ \\__ \\/_ |/ __ \\ / ____|");
    serial_writestring("\r\n");
    serial_writestring("   __) | ) || | |  | | (___  ");
    serial_writestring("\r\n");
    serial_writestring("  |__ < / / | | |  | |\\___ \\ ");
    serial_writestring("\r\n");
    serial_writestring("  ___) / /_ | | |__| |____) |");
    serial_writestring("\r\n");
    serial_writestring(" |____/____||_|\\____/|_____/ ");
    serial_writestring("\r\n\r\n");

    struct GDTDescriptor gdtDescriptor;

    init_tss();

    gdtDescriptor.Size = sizeof(struct GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;

    LoadGDT(&gdtDescriptor);

    kernelLogSuccess("GDT loaded successfully");

    serial_writestring("    - GDT Offset = ");
    serial_writestring(hexToString((uint64_t)gdtDescriptor.Offset));
    serial_writestring("\r\n");
    serial_writestring("    - GDT Limit   = ");
    serial_writestring(hexToString((uint64_t)gdtDescriptor.Size));
    serial_writestring("\r\n");

    stivale_init(stivale2_struct);

    InitInterrupts();
    InitIDT();

    RemapPIC();

    InitializeExceptionHandlers();

    kernelLogSuccess("Interrupts initialized successfully");

    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    CreateHandler((void*)keyboardInterruptHandler, 0x21, IDT_TA_InterruptGate, 0x28);

    PageFrameInitialize();

    asm ("sti");

    kernelLogSuccess("Kernel initialized successfully");

    serial_writestring("Running PC speaker test beep");
    play_sound(1000);

    pcspeaker_quiet();


    while(1)
    {
        asm("hlt");
        
    }
}