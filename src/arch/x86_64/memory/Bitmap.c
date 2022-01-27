#include "Bitmap.h"

bool Get(struct Bitmap* bitmap, uint64_t index)
{
    if (index > bitmap->size * 8) return 0;

    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;

    if ((bitmap->buffer[byteIndex] & bitIndexer) > 0)
    {
        return 1;
    }

    return 0;
}

bool Set(struct Bitmap* bitmap, uint64_t index, bool value)
{
    if (index > bitmap->size * 8) return 0;

    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;

    bitmap->buffer[byteIndex] &= ~bitIndexer;

    if (value)
    {
        bitmap->buffer[byteIndex] |= bitIndexer;
    }

    return 1;
}