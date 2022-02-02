#pragma once
#include "../memory.h"
#include "../../../../kernel/stivale/stivale_tags.h"
#include "../Bitmap.h"

// Reads the stivale memory map and intializes the page frame allocator
void InitializePageFrameAllocator();
void pageframe_reserve(void* physicalAddress, size_t pages);
void pageframe_unreserve(void* physicalAddress, size_t pages);