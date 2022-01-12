#include "interrupt_handlers.h"
#include "idt.h"
#include "interrupts.h"

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
}

void InitializeExceptionHandlers()
{
    CreateHandler((void*)pageFaultHandler, 0xE, IDT_TA_TrapGate, 0x08);
    CreateHandler((void*)doubleFaultHandler, 0x8, IDT_TA_TrapGate, 0x08);
    CreateHandler((void*)gpFaultHandler, 0xD, IDT_TA_TrapGate, 0x08);

    terminal_printstr("SUCCESS: Initialized exception handlers");

}