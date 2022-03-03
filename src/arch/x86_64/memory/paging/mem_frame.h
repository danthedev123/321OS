#pragma once
#include <stddef.h>


size_t GetTotalRAMSize();
size_t GetUsedRAMSize();
size_t GetReservedRAMSize();

void PageFrameInitialize();

void* RequestPage();