#pragma once
#include <stdint.h>

void InitInterrupts();
void CreateHandler(void* handlerFunction, uint8_t entryOffset, uint8_t type_attr, uint8_t sel);
void InitIDT();