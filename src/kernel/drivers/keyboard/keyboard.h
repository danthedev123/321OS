#pragma once
#include <stdint.h>
#include <bool.h>

char ScancodeToChar(uint8_t scancode, bool uppercase);
void HandleKeyboardPress(uint8_t scancode);