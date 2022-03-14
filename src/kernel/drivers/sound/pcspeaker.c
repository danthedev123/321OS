// Driver for generic IBM PC speaker
#include <stdint.h>
#include <kernel/drivers/sound/pcspeaker.h>

// Plays a sound with the given frequency.
// @params:
// nFrequence: Frequency of the sound to play.
void play_sound(uint32_t nFrequence)
{
	uint32_t Div;
	uint8_t tmp;

	// Set the PIT to the desired frequency
	Div = 1193180 / nFrequence;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t)(Div));
	outb(0x42, (uint8_t)(Div >> 8));

	// And play the sound using the PC speaker
	tmp = inb(0x61);
	if (tmp != (tmp | 3))
	{
		outb(0x61, tmp | 3);
	}
}

// Turns off PC speaker sound
void pcspeaker_quiet()
{
	uint8_t tmp = inb(0x61) & 0xFC;

	outb(0x61, tmp);
}
