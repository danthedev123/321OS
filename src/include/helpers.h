#pragma once

// Macro to round value [n] to the nearest multiple [m]
#define ROUND_UP(n, m) ((n) % (m) == 0 ? (n) : (n) - (n) % (m) + (m)) 

