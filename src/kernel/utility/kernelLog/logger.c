#include <kernel/stivale/terminal.h>


void kernelLogSuccess(char* str)
{
    // [ (GREEN) OK (RESET TO DEFAULT) ] 
    terminal_printstr("[ ");
    terminal_printstr("\033[32;m");
    terminal_printstr("OK");
    terminal_printstr("\033[0;m");
    terminal_printstr(" ] ");
    terminal_printstr(str);
    terminal_printstr("\n");
}

void kernelLogFail(char* str)
{
    terminal_printstr("[ ");
    terminal_printstr("\033[31;m");
    terminal_printstr("FAILED");
    terminal_printstr("\033[0;m");
    terminal_printstr(" ] ");
    terminal_printstr(str);
    terminal_printstr("\n"); 
}