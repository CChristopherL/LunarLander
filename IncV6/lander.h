#ifndef LANDER_H_
#define LANDER_H_

#include "math.h"
#include "explosion.h"
#include "particle.h"
#include "bullet.h"
#include "gameController.h"
#include <stdint.h>

typedef struct {
	uint16_t (*image)[8][24];
	vector_t (*imageOffsets)[8];
	vector_t imageSize;
	uint8_t dockingSize;

	vector_t position;
	vector_t velocity;
	int32_t direction;
	int16_t fuel;
	int8_t lives;
	int16_t score;
	uint8_t landed;

	explosion_t explosions[2];

	vector_t (*boostParticleOffsets)[8];
	uint8_t boostParticleDebounce;
	uint8_t boostParticleCount;
	particle_t boostParticles[6];

	vector_t (*bulletOffsets)[8];
	uint8_t bulletDebounce;
	uint8_t bulletCount;
	uint8_t bulletType;
	bullet_t bullets[6];

} lander_t;

void initLander(lander_t *lander, int16_t fuel, int8_t lives, int16_t score, uint8_t dockingSize);
void explodeLander(lander_t *lander);
void updateLander(lander_t *lander, uint32_t deltaTime, uint8_t joystickState);

#endif /* LANDER_H_ */
