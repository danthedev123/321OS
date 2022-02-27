#include "memory.h"

void memset(void* start, uint8_t value, uint64_t num)
{
    for (uint64_t i = 0; i < num; i++)
    {
        *(uint8_t*)((uint64_t)start+i) = value;
    }
}

void memzero(void* start, uint64_t num)
{
    for (int i = 0; i < num; i++) ((uint8_t*)start)[i] = 0;
}