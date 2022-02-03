#include "Bitmap.h"
#include "memory.h"

void bitmap_init(struct Bitmap* bitmap, void* buffer, size_t size)
{
    bitmap->buffer = buffer;
    bitmap->size = size;

    memset(bitmap->buffer, 0, size);
}

size_t bitmap_size(struct Bitmap* bitmap)
{
    return bitmap->size * SCALE;
}

bool bitmap_get(struct Bitmap* bitmap, size_t index)
{
    if (index > bitmap_size(bitmap))
    {
        return false;
    }
    
    uint64_t byte_index = index / SCALE;
    uint8_t bit_index = index % SCALE;
    uint8_t bit_indexer = 0b10000000 >> bit_index;

    if ((bitmap->buffer[byte_index] & bit_indexer) != 0)
    {
        return true;
    }
    return false;
}

bool bitmap_set(struct Bitmap* bitmap, size_t index, bool value)
{
    if (index > bitmap_size(bitmap))
    {
        return false; // out of range
    }

	uint64_t byte_index = index / SCALE;
	uint8_t bit_index = index % SCALE;
	uint8_t bit_selector = 0b10000000 >> bit_index;

	if (value)
    {
		bitmap->buffer[byte_index] |= bit_selector; // force the bit on
	}
    else
    {
		bitmap->buffer[byte_index] &= ~bit_selector; // force the bit off
	}

    return true;
}