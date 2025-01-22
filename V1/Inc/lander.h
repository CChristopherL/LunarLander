#ifndef LANDER_H_
#define LANDER_H_

#include "math.h"
#include "explosion.h"
#include <stdint.h>

typedef struct {
	vector_t position;
	vector_t velocity;
	int32_t direction;
	int16_t fuel;
	int8_t lives;
	int16_t score;
	uint8_t exploding;
	explosion_t explosion;
	uint8_t landed;
} lander_t;

void initLander(lander_t *lander, int16_t fuel, int8_t lives, int16_t score);
void updateLander(lander_t *lander, uint32_t deltaTime, uint8_t joystickState, uint16_t level, uint32_t seed);

#endif /* LANDER_H_ */
