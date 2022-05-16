#include "../../../arch/x86_64/io/io.h"
#include <stddef.h>

#define COM1_PORT 0x3f8

int is_transmit_empty()
{
    return inb(COM1_PORT + 5) & 0x20;
}

void InitializeSerial()
{
    outb(COM1_PORT + 1, 0x00);
    outb(COM1_PORT + 3, 0x80);
    outb(COM1_PORT, 0x03);
    outb(COM1_PORT + 1, 0x00);
    outb(COM1_PORT + 3, 0x03);
    outb(COM1_PORT + 2, 0xC7);
    outb(COM1_PORT + 4, 0x0B);
    
}

void serial_putc(char c)
{
    while (is_transmit_empty() == 0)
    {

    }

    outb(COM1_PORT, c);
}

void serial_writestring(const char* str)
{
    for (size_t i = 0; str[i]; i++)
    {
        serial_putc(str[i]);
    }
}