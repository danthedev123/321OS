#pragma once
#include "../../kernel/terminal.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI 0x20

struct interrupt_frame;

__attribute__((interrupt, noreturn)) void pageFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt, noreturn)) void doubleFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt, noreturn)) void gpFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void keyboardInterruptHandler(struct interrupt_frame* frame);

void InitializeExceptionHandlers();

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();