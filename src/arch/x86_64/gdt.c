#include "gdt.h"
#include "memory/memory.h"

struct tss
{
    uint32_t reserved0; uint64_t rsp0;      uint64_t rsp1;
    uint64_t rsp2;      uint64_t reserved1; uint64_t ist1;
    uint64_t ist2;      uint64_t ist3;      uint64_t ist4;
    uint64_t ist5;      uint64_t ist6;      uint64_t ist7;
    uint64_t reserved2; uint16_t reserved3; uint16_t iopb_offset;
} tss;

__attribute__((aligned(0x1000)))
struct GDT DefaultGDT = {
    {0x0000, 0, 0, 0x00, 0x00, 0}, // Null 0x00
    {0xFFFF, 0, 0, 0x9A, 0x00, 0}, // 16 bit code 0x08
    {0xFFFF, 0, 0, 0x92, 0x00, 0}, // 16 bit data 0x10
    {0xFFFF, 0, 0, 0x9A, 0xCF, 0}, // 32 bit code 0x18
    {0xFFFF, 0, 0, 0x92, 0xCF, 0}, // 32 bit data 0x20
    {0x0000, 0, 0, 0x9A, 0x20, 0}, // 64 bit code 0x28
    {0x0000, 0, 0, 0x92, 0x00, 0}, // 64 bit data 0x30

    {0x0000, 0, 0, 0x00, 0x00, 0}, // User null 0x38
    
    {0x0000, 0, 0, 0xF2, 0x00, 0}, // User data 0x40
    {0x0000, 0, 0, 0xFA, 0x20, 0}, // User code 0x48

    // TSS Lower 0x50
    {0, 0, 0, 0x89, 0xa0, 0},
    // TSS upper 0x58
    {0, 0, 0, 0x00, 0x00, 0},
};

void init_tss()
{
    memset((void*)&tss, 0, sizeof(tss));

    uint64_t tss_base = ((uint64_t)&tss);

    DefaultGDT.TssLower.base0 = tss_base & 0xffff;
    DefaultGDT.TssLower.base1 = (tss_base >> 16) & 0xff;
    DefaultGDT.TssLower.base2 = (tss_base >> 24) & 0xff;

    DefaultGDT.TssLower.limit0 = sizeof(tss);

    DefaultGDT.TssUpper.limit0 = (tss_base >> 32) & 0xffff;
    DefaultGDT.TssUpper.base0 = (tss_base >> 48) & 0xffff;
}