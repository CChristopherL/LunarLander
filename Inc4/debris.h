#ifndef DEBRIS_H_
#define DEBRIS_H_

#include "math.h"
#include <stdint.h>

typedef struct {
	uint16_t (*image)[5][24];
	vector_t size;
	vector_t position;
	vector_t velocity;
	uint32_t frame;
	uint8_t type;
} debris_t;

void initDebris(debris_t *debris, vector_t position, vector_t velocity, uint8_t type);
uint8_t updateDebris(debris_t *debris, uint32_t deltaTime);

#endif /* DEBRIS_H_ */
