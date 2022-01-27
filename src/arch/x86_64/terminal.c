#include "../../kernel/terminal.h"

#include <stddef.h>

const static size_t VGA_NUM_COLS = 80;
const static size_t VGA_NUM_ROWS = 25;

struct Character
{
    uint8_t character;
    uint8_t color;
};

struct Character* buffer = (struct Character*)0xb8000;
size_t col = 0;
size_t row = 0;
// Define default color
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row)
{
    struct Character empty = (struct Character)
    {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < VGA_NUM_COLS; col++) {
        buffer[col + VGA_NUM_COLS * row] = empty;
    }
}

void terminal_clear()
{
    for (size_t i = 0; i < VGA_NUM_ROWS; i++)
    {
        clear_row(i);
    }
}

void terminal_newline()
{
    col = 0;

    if (row < VGA_NUM_ROWS - 1)
    {
        row++;
        return;
    }

    for (size_t row = 1; row < VGA_NUM_ROWS; row++)
    {
        for (size_t col = 0; col < VGA_NUM_COLS; col++)
        {
            struct Character character = buffer[col + VGA_NUM_COLS * row];
            buffer[col + VGA_NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row(VGA_NUM_ROWS - 1);
}

void terminal_putchar(char character)
{
    if (character == '\n')
    {
        terminal_newline();
        return;
    }

    if (col > VGA_NUM_COLS)
    {
        terminal_newline();
    }

    buffer[col + VGA_NUM_COLS * row] = (struct Character)
    {
        character: (uint8_t) character,
        color: color
    };

    col++;
}

void terminal_printstr(char* str)
{
    for (size_t i = 0; 1; i++)
    { 
        char character = (uint8_t) str[i];

        if (character ==
        '\0')
        {
            return;
        }

        terminal_putchar(character);
    }
}

void terminal_set_color(uint8_t foreground, uint8_t background)
{
    color = foreground + (background << 4);
}