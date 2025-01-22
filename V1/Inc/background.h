#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "math.h"

// Terrain colors/characters
#define LANDING_PAD 0b0101011111011111
#define BRIGHT_GRAY 0b0011100010110010
#define LIGHT_GRAY 0b0011100010110001
#define GRAY 0b0011100010110000
#define MEDIUM_GRAY 0b1011100010110001
#define DARK_GRAY 0b1011100010110000
#define DARK_RED 0b1000100011011011
#define RED 0b0000100011011011
#define ORANGE 0b0001100110110001
#define WHITE 0b0011111011011011
#define LIGHT_CYAN 0b0011011110110010
#define CYAN 0b0011011111011011
#define BLUE 0b0010011111011011
#define BLUE_CYAN 0b1010011111011011
#define BLUE_GRAY 0b1011110010110010

// Atmosphere colors/characters
#define STAR_AST_LIGHT 0b0011100000101010
#define STAR_AST_DARK 0b1011100000101010
#define STAR_PLUS_LIGHT 0b0011100000101011
#define STAR_PLUS_DARK 0b1011100000101011
#define STAR_TIMES_LIGHT 0b0011100010011110
#define STAR_TIMES_DARK 0b1011100010011110
#define STAR_DIV_LIGHT 0b0011100011110110
#define STAR_DIV_DARK 0b1011100011110110
#define SPACE 0b0011100000100000

void initBackground(uint16_t *level, uint32_t *seed);
int32_t getTerrainHeight(vector_t position, uint32_t seed);
int32_t getTerrainSlope(vector_t position, uint32_t seed);
uint16_t getBackgroundASCIICharacter(vector_t position, uint16_t level, uint32_t seed, uint8_t checkingNeighbor);
uint16_t* getBackgroundASCIIImage(vector_t position, vector_t size, uint16_t *level, uint32_t *seed, uint16_t *asciiImage, uint8_t padding);
void destroyBackgroundASCIIImage(uint16_t *backgroundASCIIImage);

#endif /* BACKGROUND_H_ */
