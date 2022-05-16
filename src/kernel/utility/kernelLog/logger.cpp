#include <kernel/stivale/terminal.h>
#include <kernel/drivers/serial/serial.h>


void kernelLogSuccess(const char* str)
{
    // [ (GREEN) OK (RESET TO DEFAULT) ] 
    terminal_printstr("[ ");
    terminal_printstr("\033[32;m");
    terminal_printstr("OK");
    terminal_printstr("\033[0;m");
    terminal_printstr(" ] ");
    terminal_printstr(str);
    terminal_printstr("\n");

    serial_writestring(" ");
    serial_writestring("success");
    serial_writestring("  ");
    serial_writestring(str);
    serial_writestring("\r\n");

}

void kernelLogFail(const char* str)
{
    terminal_printstr("[ ");
    terminal_printstr("\033[31;m");
    terminal_printstr("FAILED");
    terminal_printstr("\033[0;m");
    terminal_printstr(" ] ");
    terminal_printstr(str);
    terminal_printstr("\n"); 

    serial_writestring(" ");
    serial_writestring("fail");
    serial_writestring("  ");
    serial_writestring(str);
    serial_writestring("\r\n");
}