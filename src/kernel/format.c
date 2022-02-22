#include <kernel/format.h>


char hexToStroutput[128];

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
        hexToStroutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexToStroutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexToStroutput[size + 1] = 0;

    return hexToStroutput;
}

char* itoa(int val, int base){

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)
    {
        buf[i] = "0123456789abcdef"[val % base];

    }

    return &buf[i+1];

}