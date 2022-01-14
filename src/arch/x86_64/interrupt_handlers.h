#pragma once
#include "../../kernel/terminal.h"

void pageFaultHandler();
void doubleFaultHandler();
void gpFaultHandler();

void InitializeExceptionHandlers();
