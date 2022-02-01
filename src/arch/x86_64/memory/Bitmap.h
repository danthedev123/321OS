#pragma once
#include <bool.h>
#include <stdint.h>
#include <stddef.h>

#define SCALE sizeof(size_t)

struct Bitmap
{
    uint8_t* buffer;
    size_t size;
};

void bitmap_init(struct Bitmap* bitmap, void* buffer, size_t size);

bool bitmap_get(struct Bitmap* bitmap, size_t index);
void bitmap_set(struct Bitmap* bitmap, size_t index, bool value);