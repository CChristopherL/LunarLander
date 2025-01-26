#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "math.h"
#include <stdint.h>

typedef struct {
	uint16_t (*image)[1];
	vector_t size;
	vector_t position;
	vector_t velocity;
	uint32_t frame;
} particle_t;

void initParticle(particle_t *particle, vector_t position, vector_t velocity);
void updateParticle(particle_t *particle, uint32_t deltaTime);

#endif /* PARTICLE_H_ */
