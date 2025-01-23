#ifndef COMET_H_
#define COMET_H_

#include "math.h"
#include "debris.h"
#include <stdint.h>

typedef struct {
	uint16_t (*image)[77];
	vector_t size;
	vector_t position;
	vector_t velocity;
	uint32_t frame;
	uint8_t broken;
	debris_t debris1;
	debris_t debris2;
	debris_t debris3;
} comet_t;

void initComet(comet_t *comet, vector_t position, vector_t velocity);
void breakComet(comet_t *comet);
void updateComet(comet_t *comet, uint32_t deltaTime);

#endif /* COMET_H_ */
