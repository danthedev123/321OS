#include "interrupt_handlers.h"

void pageFaultHandler()
{
    terminal_printstr("panic: Detected page fault");
    while(1);
}