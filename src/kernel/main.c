
#include "terminal.h"
#include "../arch/x86_64/interrupts.h"
#include "../arch/x86_64/interrupt_handlers.h"
#include "../arch/x86_64/idt.h"

void kernel_main()
{
    InitInterrupts();
    InitIDT();


    //void CreateHandler(void* handlerFunction, uint8_t entryOffset, uint8_t type_attr, uint8_t sel)

    // Make the page fault exception handler



    terminal_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);

    terminal_clear();

    terminal_printstr("Hello World!\nHello from a new line\n");

    InitializeExceptionHandlers();

    int* test = (int*)0x80000000000;
    *test = 2;
}