#include <kernel/drivers/keyboard/keyboard.h>
#include <kernel/bool.h>
#include <kernel/stivale/terminal.h>

#define ShiftLeft 0x2A
#define ShiftRight 0x36
#define Enter 0x1C
#define Space 0x39

bool isLeftShiftCurrentlyPressed;
bool isRightShiftCurrentlyPressed;

const char ASCIITable[] = {
    0, 0, '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '-', '=', 0, 0,
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '[', ']',
    0, 0, 'a', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';',
    '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',',
    '.', '/', 0, '*',
    0, ' '
};

char ScancodeToChar(uint8_t scancode, bool uppercase)
{
    if (scancode > 58) return 0;

    if (uppercase)
    {
        return ASCIITable[scancode] - 32;
    }

    return ASCIITable[scancode];
}

void HandleKeyboardPress(uint8_t scancode)
{
    switch (scancode)
    {
        case ShiftLeft:
            isLeftShiftCurrentlyPressed = true;
            break;
        case ShiftRight:
            isRightShiftCurrentlyPressed = true;
            break;
        case ShiftLeft + 0x80:
            isLeftShiftCurrentlyPressed = false;
            break;
        case ShiftRight + 0x80:
            isRightShiftCurrentlyPressed = false;
            break;
        case Enter:
            terminal_putchar('\n');
            break;
        case Space:
            terminal_printstr(" ");
            break;
        default:
            terminal_putchar(ScancodeToChar(scancode, isLeftShiftCurrentlyPressed | isRightShiftCurrentlyPressed));
            break;
    }
}