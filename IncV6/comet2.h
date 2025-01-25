#ifndef COMET2_H_
#define COMET2_H_

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
} comet2_t;

void initComet2(comet2_t *comet, vector_t position, vector_t velocity);
void breakComet2(comet2_t *comet);
void updateComet2(comet2_t *comet, uint32_t deltaTime);

#endif /* COMET2_H_ */
