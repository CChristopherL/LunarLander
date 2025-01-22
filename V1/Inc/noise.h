#ifndef NOISE_H_
#define NOISE_H_

#include <stdlib.h>
#include <stdint.h>
#include "math.h"

uint16_t generateWhiteNoise(int16_t x, int16_t y, uint32_t range, uint32_t seed);
int16_t interpolateLinear(int16_t a, int16_t b, int16_t alpha);
uint8_t generateValueNoise(vector_t position, uint8_t scale, uint32_t seed);

#endif /* NOISE_H_ */
