#include "gdt.h"
#include "memory/memory.h"

struct tss
{
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb_offset;
} tss;

__attribute__((aligned(0x1000))) struct GDT DefaultGDT = {

    {0}, // Null descriptor

    {

        .limit0 = 0xffff,
        .base0 = 0,
        .base1 = 0,
        .access = 0b10011010,
        .limit1_flags = 0,
        .base2 = 0,
    },

    {
        .limit0 = 0xffff,
        .base0 = 0,
        .base1 = 0,
        .access = 0b10010010,
        .limit1_flags = 0,
        .base2 = 0,
    },

    {
        .limit0 = 0xffff,
        .base0 = 0,
        .base1 = 0,
        .access = 0b10011010,
        .limit1_flags = 0b11001111,
        .base2 = 0,
    },
    {
        .limit0 = 0xffff,
        .base0 = 0,
        .base1 = 0,
        .access = 0b10010010,
        .limit1_flags = 0b11001111,
        .base2 = 0,
    },
    {
        .limit0 = 0,
        .base0 = 0,
        .base1 = 0,
        .access = 0b10011010,
        .limit1_flags = 0b00100000,
        .base2 = 0,
    },
    {
        .limit0 = 0,
        .base0 = 0,
        .base1 = 0,
        .access = 0b10010010,
        .limit1_flags = 0,
        .base2 = 0,
    },

    {0},

    {
        .limit0 = 0,
        .base0 = 0,
        .base1 = 0,
        .access = 0b11110010,
        .limit1_flags = 0,
        .base2 = 0,
    },
    {
        .limit0 = 0,
        .base0 = 0,
        .base1 = 0,
        .access = 0b11111010,
        .limit1_flags = 0b00100000,
        .base2 = 0,
    },

    // TSS Upper
    {
        .limit0 = 0,
        .base0 = 0,
        .base1 = 0,
        .access = 0b10001001,
        .limit1_flags = 0b10100000,
        .base2 = 0,
    },

    {0}, // TSS lower


};
void init_tss()
{
    memset((void *)&tss, 0, sizeof(tss));

    uint64_t tss_base = ((uint64_t)&tss);

    DefaultGDT.TssLower.base0 = tss_base & 0xffff;
    DefaultGDT.TssLower.base1 = (tss_base >> 16) & 0xff;
    DefaultGDT.TssLower.base2 = (tss_base >> 24) & 0xff;

    DefaultGDT.TssLower.limit0 = sizeof(tss);

    DefaultGDT.TssUpper.limit0 = (tss_base >> 32) & 0xffff;
    DefaultGDT.TssUpper.base0 = (tss_base >> 48) & 0xffff;
}