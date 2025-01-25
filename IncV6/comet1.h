#ifndef COMET1_H_
#define COMET1_H_

#include "math.h"
#include "debris.h"
#include <stdint.h>

typedef struct {
	uint16_t (*image)[128];
	vector_t imageSize;
	vector_t position;
	vector_t velocity;
	uint32_t frame;
	uint8_t broken;
	debris_t debris1;
	debris_t debris2;
	debris_t debris3;
} comet1_t;

void initComet1(comet1_t *comet, vector_t position, vector_t velocity);
void breakComet1(comet1_t *comet);
void updateComet1(comet1_t *comet, uint32_t deltaTime);

#endif /* COMET1_H_ */
