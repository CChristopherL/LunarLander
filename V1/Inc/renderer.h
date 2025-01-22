#ifndef RENDERER_H_
#define RENDERER_H_

#include <stdint.h>
#include <stdio.h>
#include "math.h"

#define ESC 0x1B

void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size);
void drawASCIIImageWithBackground(uint16_t *asciiImage, vector_t position, vector_t size);

#endif /* RENDERER_H_ */
