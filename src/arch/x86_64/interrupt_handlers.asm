; todo make proper interrupts stub

extern pageFaultHandler
extern doubleFaultHandler
extern gpFaultHandler
extern divisionByZeroHandler
extern keyboardInterruptHandler

int_pageFault:
    call pageFaultHandler
    iretq
GLOBAL int_pageFault
int_doubleFault:
    call doubleFaultHandler
    iretq
GLOBAL int_doubleFault
int_gpFault:
    call gpFaultHandler
    iretq
GLOBAL int_gpFault
int_divisionByZero:
    call divisionByZeroHandler
    iretq
GLOBAL int_divisionByZero
int_keyboard:
    call keyboardInterruptHandler
    iretq
GLOBAL int_keyboard