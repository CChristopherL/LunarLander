#ifndef POWERUP_H_
#define POWERUP_H_

#include "math.h"
#include <stdint.h>

typedef struct {
	uint16_t (*image)[45];
	vector_t imageSize;
	vector_t position;
	uint8_t type;
	uint8_t disabled;
} powerup_t;

void initPowerup(powerup_t *powerup, vector_t position, uint8_t type);
void updatePowerup(powerup_t *powerup);

#endif /* POWERUP_H_ */
