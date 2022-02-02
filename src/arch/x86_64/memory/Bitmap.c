#include "Bitmap.h"
#include "memory.h"

void bitmap_init(struct Bitmap* bitmap, void* buffer, size_t size)
{
    bitmap->buffer = buffer;
    bitmap->size = size;

    memset(bitmap->buffer, 0, size);
}

bool bitmap_get(struct Bitmap* bitmap, size_t index)
{
    uint64_t byte_index = index / SCALE;
    uint8_t bit_index = index % SCALE;
    uint8_t bit_indexer = 0b10000000 >> bit_index;

    if ((bitmap->buffer[byte_index] & bit_indexer) != 0)
    {
        return true;
    }
    return false;
}

void bitmap_set(struct Bitmap* bitmap, size_t index, bool value)
{

	uint64_t byte_index = index / SCALE;
	uint8_t bit_index = index % SCALE;
	uint8_t bit_selector = 0b10000000 >> bit_index;

	if (value)
    {
		bitmap->buffer[byte_index] |= bit_selector;		// force the bit on
	}
    else
    {
		bitmap->buffer[byte_index] &= ~bit_selector;	// force the bit off
	}
}