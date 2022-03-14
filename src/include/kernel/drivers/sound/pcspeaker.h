// Driver for generic IBM PC speaker
#include <stdint.h>
#include "../../../../arch/x86_64/io/io.h"

// Plays a sound with the given frequency.
// @params:
// nFrequence: Frequency of the sound to play.
void play_sound(uint32_t nFrequence);
void pcspeaker_quiet();