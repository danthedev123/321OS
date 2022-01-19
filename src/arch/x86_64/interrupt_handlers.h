#pragma once
#include "../../kernel/terminal.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void pageFaultHandler();
void doubleFaultHandler();
void gpFaultHandler();
void keyboardInterruptHandler();

void InitializeExceptionHandlers();

void RemapPIC();