#include "gdt.h"

__attribute__((aligned(0x1000)))
struct GDT DefaultGDT = {
    {0x0000, 0, 0, 0x00, 0x00, 0}, // Null
    {0xFFFF, 0, 0, 0x9A, 0x00, 0}, // 16 bit code
    {0xFFFF, 0, 0, 0x92, 0x00, 0}, // 16 bit data
    {0xFFFF, 0, 0, 0x9A, 0xCF, 0}, // 32 bit code
    {0xFFFF, 0, 0, 0x92, 0xCF, 0}, // 32 bit data
    {0x0000, 0, 0, 0x9A, 0x20, 0}, // 64 bit code
    {0x0000, 0, 0, 0x92, 0x00, 0}, // 64 bit data
    {0x0000, 0, 0, 0xF2, 0x00, 0}, // User data
    {0x0000, 0, 0, 0xFA, 0x20, 0}, // User code
};