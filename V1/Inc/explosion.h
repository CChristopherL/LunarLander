#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "math.h"
#include <stdint.h>

typedef struct {
	vector_t position;
	uint32_t frame;
} explosion_t;

void initExplosion(explosion_t *explosion, vector_t position);
uint8_t updateExplosion(explosion_t *explosion, uint32_t deltaTime);

#endif /* EXPLOSION_H_ */
