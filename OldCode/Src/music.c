#include "music.h"

uint16_t note = 0;
uint16_t beats = 0;

const uint16_t melodies[64] = {B2,0,FS3,F4,AS3,DS4,AS3,CS4,AS3,CS4,DS4,B2,B2,B2,0,0,B2,0,FS3,F4,AS3,DS4,AS3,CS4,AS3,CS4,DS4,B2,B2,B2,0,0,AS2,0,F3,E4,A3,D4,A3,C4,A3,C4,D4,AS2,AS2,AS2,0,0,AS2,0,F3,E4,A3,D4,A3,C4,A3,C4,D4,AS2,AS2,AS2,0,0,};

void initMusic() {
	initBuzzer();
	initTimer15();
	setMelody(0);
}

void setMelody() {
	beats = 4;
	note = 0;
}

void TIM1_BRK_TIM15_IRQHandler() {
	//TODO: Loop melody if the array has run out of notes

	if (beats == 0) {
		note++;
		beats = 4;
	}
	if (note >	 63) {
			note = 0;
		}

	setFreq(melodies[note]);
	beats--;

	TIM15->SR &= ~0x0001; // Clear interrupt bit
}
