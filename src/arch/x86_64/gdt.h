#pragma once
#include <stdint.h>

struct GDTDescriptor
{
    uint16_t Size;
    uint64_t Offset;
}__attribute__((packed));

struct GDTEntry
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access;
    uint8_t limit1_flags;
    uint8_t base2;
}__attribute__((packed));

struct GDT
{
    struct GDTEntry null;
    struct GDTEntry _16bitCode;
    struct GDTEntry _16bitData;
    struct GDTEntry _32bitCode;
    struct GDTEntry _32bitData;
    struct GDTEntry _64bitCode;
    struct GDTEntry _64bitData;
    struct GDTEntry UserNull;
    struct GDTEntry UserData;
    struct GDTEntry UserCode;
    struct GDTEntry TssLower;
    struct GDTEntry TssUpper;
}__attribute__((packed))
__attribute__((aligned(0x1000)));

extern struct GDT DefaultGDT;

extern void LoadGDT(struct GDTDescriptor* gdtDescriptor);

void init_tss();