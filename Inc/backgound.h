#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "math.h"

void initBackground(uint32_t *seed);
uint16_t getBackgroundASCIICharacter(vector_t position, uint32_t seed);
uint16_t* getBackgroundASCIIImage(vector_t position, vector_t size, uint32_t *seed, uint16_t *asciiImage, uint8_t padding);
void destroyBackgroundASCIIImage(uint16_t *backgroundASCIIImage);

#endif /* BACKGROUND_H_ */
