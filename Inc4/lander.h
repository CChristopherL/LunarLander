#ifndef LANDER_H_
#define LANDER_H_

#include "math.h"
#include "explosion.h"
#include <stdint.h>

typedef struct {
	uint16_t (*image)[12];
	vector_t *offsets;
	vector_t size;
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

#define JOYSTICK_LEFT 0b00000100
#define JOYSTICK_RIGHT 0b00001000
#define JOYSTICK_UP 0b00000001

void initLander(lander_t *lander, int16_t fuel, int8_t lives, int16_t score);
void explodeLander(lander_t *lander);
void updateLander(lander_t *lander, uint32_t deltaTime, uint8_t joystickState);

#endif /* LANDER_H_ */
