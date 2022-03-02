#pragma once

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI 0x20

struct interrupt_frame;

__attribute__((noreturn)) void pageFaultHandler(struct interrupt_frame* frame);
__attribute__((noreturn)) void doubleFaultHandler(struct interrupt_frame* frame);
__attribute__((noreturn)) void gpFaultHandler(struct interrupt_frame* frame);
__attribute__((noreturn)) void divisionByZeroHandler(struct interrupt_frame* frame);
void keyboardInterruptHandler(struct interrupt_frame* frame);

extern void int_pageFault();
extern void int_pageFault();
extern void int_doubleFault();
extern void int_gpFault();
extern void int_divisionByZero();
extern void int_keyboard();

void InitializeExceptionHandlers();

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();