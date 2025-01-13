#include "music.h"
#include "buzzer.h"
#include "timer.h"

uint8_t melody = 0;
uint16_t note = 0;
uint16_t beats = 0;

const uint16_t melodies[2][512][2] = {
	{ // Main Menu Melody
		{B2, 4}, // Note frequency, beats in length
		{0, 4},
		{FS3, 4},
		{F4, 4},
		{AS3, 4},
		{DS4, 4},
		{AS3, 4},
		{CS4, 4},
		{AS3, 4},
		{CS4, 4},
		{DS4, 4},
		{B2, 12},
		{0, 8},

		{B2, 4},
		{0, 4},
		{FS3, 4},
		{F4, 4},
		{AS3, 4},
		{DS4, 4},
		{AS3, 4},
		{CS4, 4},
		{AS3, 4},
		{CS4, 4},
		{DS4, 4},
		{B2, 12},
		{0, 8},

		{AS2, 4},
		{0, 4},
		{F3, 4},
		{E4, 4},
		{A3, 4},
		{D4, 4},
		{A3, 4},
		{C4, 4},
		{A3, 4},
		{C4, 4},
		{D4, 4},
		{AS2, 12},
		{0, 8},

		{AS2, 4},
		{0, 4},
		{F3, 4},
		{E4, 4},
		{A3, 4},
		{D4, 4},
		{A3, 4},
		{C4, 4},
		{A3, 4},
		{C4, 4},
		{D4, 4},
		{AS2, 12},
		{0, 8},

		{B2, 4},
		{0, 4},
		{FS3, 4},
		{F4, 4},
		{AS3, 4},
		{DS4, 4},
		{AS3, 4},
		{CS4, 4},
		{AS3, 4},
		{CS4, 4},
		{DS4, 4},
		{B2, 12},
		{0, 8}
	},

	{ // In-game Melody
			{0, 4} // Note Frequency, beats in length
	}
};

void initMusic() {
	initBuzzer();
	initTimer15();
	setMelody(0);
}

void setMelody(uint8_t melodyId) {
	beats = melodies[melodyId][0][1];
	note = 0;
}

void TIM1_BRK_TIM15_IRQHandler() {
	//TODO: Loop melody if the array has run out of notes
	if (beats == 0) {
		note++;
		beats = melodies[melody][note][1];

	}
	setFreq(melodies[melody][note][0]);
	beats--;

	TIM15->SR &= ~0x0001; // Clear interrupt bit
}
