#pragma once
#include <stdint.h>

struct PageMapIndexer
{
    uint64_t PDP_i;
    uint64_t PD_i;
    uint64_t PT_i;
    uint64_t P_i;
};


void InitializePageMapIndexer(struct PageMapIndexer* pmi, uint64_t virtualAddress);

