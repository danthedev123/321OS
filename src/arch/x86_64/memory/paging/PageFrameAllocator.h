#pragma once
#include "../memory.h"
#include "../../../../kernel/stivale/stivale_tags.h"
#include "../Bitmap.h"

// Intializes the page frame allocator
void InitializePageFrameAllocator();
void FreePage(void* address);
void LockPage(void* address);
