#include "music.h"
#include "buzzer.h"

void initMusic(music_t *music) {
	initBuzzer();
	setFreq(0);

	static uint16_t melody[64] = {B2,0,FS3,F4,AS3,DS4,AS3,CS4,AS3,CS4,DS4,B2,B2,B2,0,0,B2,0,FS3,F4,AS3,DS4,AS3,CS4,AS3,CS4,DS4,B2,B2,B2,0,0,AS2,0,F3,E4,A3,D4,A3,C4,A3,C4,D4,AS2,AS2,AS2,0,0,AS2,0,F3,E4,A3,D4,A3,C4,A3,C4,D4,AS2,AS2,AS2,0,0};


	music->melody = melody;
	music->musicTime = 0;
	music->note = 0;
}

void updateMusic(music_t *music, uint32_t deltaTime, uint16_t level) {
	// Change buzzer frequency based on runtime
	music->musicTime += deltaTime;
	if (music->musicTime > 20 - (level << 1)) {
		music->note = music->note + 1;
		setFreq(music->melody[music->note & 63]);
		music->musicTime = 0;
	}
}
