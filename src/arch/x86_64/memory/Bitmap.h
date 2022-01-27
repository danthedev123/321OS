#pragma once
#include "../../../kernel/bool.h"
#include <stdint.h>
#include <stddef.h>

struct Bitmap
{
    size_t size;
    uint8_t* buffer;
};

bool Get(struct Bitmap* bitmap, uint64_t index);
bool Set(struct Bitmap* bitmap, uint64_t index, bool value);