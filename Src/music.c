#include "music.h"
#include "buzzer.h"

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

	RCC->APB2ENR |= RCC_APB2Periph_TIM15;
	TIM15->CR1 = 0x0000; // Configure timer 15
	TIM15->ARR = 0x2180; // Set reload value
	TIM15->PSC = 349; // Set prescale value
	TIM15->CR1 |= 0x0001;

	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts

	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt

	setMelody(0);
}

void setMelody(uint8_t newMelodyId) {
	melody = newMelodyId;
	beats = melodies[melody][0][1];
	note = 0;
}

void TIM1_BRK_TIM15_IRQHandler() {
	//TODO: Loop melody if the array has run out of notes
	if (beats == 0) {
		if (++beats > 511) {
			beats = 0;
		}
		beats = melodies[melody][note][1];
	}
	setFreq(melodies[melody][note][0]);
	beats--;

	TIM15->SR &= ~0x0001; // Clear interrupt bit
}
