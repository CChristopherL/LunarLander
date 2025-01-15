#include "stm32f30x_conf.h"
#include "30010_io.h"

#include "music.h"
#include "buzzer.h"

void initMusic() {
	initBuzzer();
	setFreq(0);
}

void updateMusic(uint32_t *runtime) {
	// Initialize note and melody
	static uint16_t note = 0;
	static const uint16_t melody[64] = {B2,0,FS3,F4,AS3,DS4,AS3,CS4,AS3,CS4,DS4,B2,B2,B2,0,0,B2,0,FS3,F4,AS3,DS4,AS3,CS4,AS3,CS4,DS4,B2,B2,B2,0,0,AS2,0,F3,E4,A3,D4,A3,C4,A3,C4,D4,AS2,AS2,AS2,0,0,AS2,0,F3,E4,A3,D4,A3,C4,A3,C4,D4,AS2,AS2,AS2,0,0,};

	// Change buzzer frequency based on runtime
	uint16_t newNote = melody[(*runtime >> 4) & 63];
	if (newNote != note) {
		note = newNote;
		setFreq(note);
	}
}
