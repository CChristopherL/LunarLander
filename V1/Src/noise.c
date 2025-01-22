#include "noise.h"

// Generate white noise given x, y, seed, and range. Range must be a multiple of 2.
uint16_t generateWhiteNoise(int16_t x, int16_t y, uint32_t range, uint32_t seed) {
    // Large number
    uint32_t noiseValue = (229 * x + 241 * y + 257 * seed);

    // Scramble bits
    noiseValue ^= sinLUT((noiseValue + seed) & 511);
    noiseValue ^= cosLUT((noiseValue ^ seed) & 511);
    noiseValue ^= (seed << 10);
    noiseValue ^= (noiseValue >> 2);
    noiseValue ^= (seed << 4);

	// Return positive number between [0, range - 1] (range has to be a mutiple of two)
	return noiseValue & (range - 1);
}

// Linear interpolation using 12.4 fixed-point arithmetic
int16_t interpolateLinear(int16_t a, int16_t b, int16_t alpha) {
	return a + (((b - a) * alpha) >> 4);
}

uint8_t generateValueNoise(vector_t position, uint8_t scale, uint32_t seed) {
	// Choose positional values for grid
	int16_t gridLeft = position.x / scale;
	int16_t gridTop = position.y / scale;
	int16_t gridRight = gridLeft + 1;
	int16_t gridBottom = gridTop + 1;

	// Current position used for interpolation
	int16_t x = ((position.x % scale) << 4) / scale;
	int16_t y = ((position.y % scale) << 4) / scale;

	// Interpolate grid and return value based on position in grid
	uint8_t whiteNoiseBottomLeft = generateWhiteNoise(gridLeft, gridBottom, 8, seed);
	uint8_t whiteNoiseBottomRight = generateWhiteNoise(gridRight, gridBottom, 8, seed);
	uint8_t whiteNoiseTopLeft = generateWhiteNoise(gridLeft, gridTop, 8, seed);
	uint8_t whiteNoiseTopRight = generateWhiteNoise(gridRight, gridTop, 8, seed);
	int16_t topInterpolated = interpolateLinear(whiteNoiseTopLeft << 4, whiteNoiseTopRight << 4, x);
	int16_t bottomInterpolated = interpolateLinear(whiteNoiseBottomLeft << 4, whiteNoiseBottomRight << 4, x);
	return (interpolateLinear(topInterpolated, bottomInterpolated, y) >> 4);
}
