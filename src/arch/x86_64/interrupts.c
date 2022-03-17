
#include "idt.h"
#include "interrupts.h"
#include <kernel/stivale/terminal.h>
#include <kernel/drivers/serial/serial.h>
#include "memory/paging/mem_frame.h"

struct IDTR idtr;

struct IDTDescriptorEntry idt_entries[256];

void InitInterrupts()
{
    idtr.limit = 0x0FFF;
    idtr.offset = (uint64_t)RequestPage();  
}

void CreateHandler(void* handlerFunction, uint8_t entryOffset, uint8_t type_attr, uint8_t sel)
{
    struct IDTDescriptorEntry* newInterrupt = (struct IDTDescriptorEntry*)(idtr.offset + entryOffset * sizeof(struct IDTDescriptorEntry));

    newInterrupt->offset0 = (uint16_t)((uint64_t)handlerFunction & 0x000000000000ffff);
    newInterrupt->offset1 = (uint16_t)(((uint64_t)handlerFunction & 0x00000000ffff0000) >> 16);
    newInterrupt->offset2 = (uint32_t)(((uint64_t)handlerFunction & 0xffffffff00000000) >> 32);
    newInterrupt->type_attr = type_attr;
    newInterrupt->sel = sel;
    newInterrupt->ignore = 0;
}

void InitIDT()
{
    asm("lidt %0" : : "m" (idtr));
}