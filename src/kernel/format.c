#include "format.h"

char output[128];

char output16[128];
char* hexToString16(uint16_t value)
{
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        output16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        output16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    output16[size + 1] = 0;
    return output16;
}

char* hexToString(uint64_t value)
{
    uint64_t* valPtr = &value;

    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;

    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        output[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        output[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    output[size + 1] = 0;

    return output;
}

