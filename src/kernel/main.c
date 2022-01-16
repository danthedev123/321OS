
#include "terminal.h"
#include "../arch/x86_64/interrupts.h"
#include "../arch/x86_64/interrupt_handlers.h"
#include "../arch/x86_64/idt.h"
#include "../arch/x86_64/io/io.h"

void kernel_main()
{
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
}