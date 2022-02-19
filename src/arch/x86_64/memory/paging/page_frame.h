#pragma once
#include <stddef.h>

void ReadStivaleMemoryMap();
void AllocatePage();

size_t GetTotalRAMSize();
size_t GetUsedRAMSize();
size_t GetReservedRAMSize();