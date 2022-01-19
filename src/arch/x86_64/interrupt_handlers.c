#include "interrupt_handlers.h"
#include "idt.h"
#include "interrupts.h"
#include "io/io.h"

void pageFaultHandler()
{
    terminal_printstr("panic: detected page fault");
    while(1);
}

void doubleFaultHandler()
{
    terminal_printstr("panic: detected double fault");
    while(1);
}

void gpFaultHandler()
{
    terminal_printstr("panic: detected general protection fault");
    while(1);
    
}

void InitializeExceptionHandlers()
{
    CreateHandler((void*)pageFaultHandler, 0xE, IDT_TA_TrapGate, 0x08);
    CreateHandler((void*)doubleFaultHandler, 0x8, IDT_TA_TrapGate, 0x08);
    CreateHandler((void*)gpFaultHandler, 0xD, IDT_TA_TrapGate, 0x08);

    terminal_printstr("SUCCESS: Initialized exception handlers");

}

void keyboardInterruptHandler()
{
    terminal_printstr("p");
}

void RemapPIC()
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);

    terminal_printstr("\nSUCCESS: PIC Initialized successfully");
}