#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdint.h>

// Define our musical notes and their frequency(Hz)
#define AS2 233
#define B2	245
#define F3	349
#define FS3	370
#define A3	440
#define AS3	466
#define C4	523
#define CS4	554
#define D4	587
#define DS4	622
#define E4	659
#define F4	698

typedef struct {
	uint16_t *melody;
	uint32_t musicTime;
	uint16_t note;
} music_t;

void initMusic(music_t *music);
void updateMusic(music_t *music, uint32_t deltaTime, uint16_t level);

#endif /* MUSIC_H_ */
