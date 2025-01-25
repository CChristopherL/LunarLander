#ifndef BULLET_H_
#define BULLET_H_

#include "math.h"
#include <stdint.h>

typedef struct {
	uint16_t (*image)[1];
	vector_t imageSize;
	vector_t position;
	vector_t velocity;
	vector_t shadowPosition;
	uint32_t frame;
} bullet_t;

void initBullet(bullet_t *bullet, vector_t position, vector_t velocity);
void updateBullet(bullet_t *bullet, uint32_t deltaTime);

#endif /* BULLET_H_ */
