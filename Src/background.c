#include "30010_io.h"
#include "stm32f30x_conf.h"

#include "background.h"
#include "renderer.h"
#include "noise.h"

void initBackground(uint32_t *seed) {
	// Split background up into chunks and write them to the screen
	vector_t chunkPosition = {0, 0};
	vector_t chunkSize = {16, 16};

	// Create chunks
	for (uint8_t chunk = 0; chunk < 96; chunk++) {
		chunkPosition.x = (chunk & 15) << 4;
		chunkPosition.y = (chunk >> 4) << 4;

		uint16_t *chunkASCIIImage = getBackgroundASCIIImage(chunkPosition, chunkSize, seed, NULL, 0);
		drawASCIIImage(chunkASCIIImage, chunkPosition, chunkSize);
		destroyBackgroundASCIIImage(chunkASCIIImage);
	}
}

int32_t getTerrainHeight(vector_t position, uint32_t seed) {
	uint16_t terrainShift = (seed & 511);
	return 87 - ((sinLUT((position.x << 3) + (terrainShift << 4)) - cosLUT(position.x << 2) + (terrainShift << 3)) >> 13);
}

int32_t getTerrainSlope(vector_t position, uint32_t seed) {
	uint16_t terrainShift = (seed & 511);
	return -(8 * cosLUT(8 * position.x + 16 * terrainShift) + 4 * sinLUT(4 * position.x)) >> 13;
}

uint16_t getBackgroundASCIICharacter(vector_t position, uint32_t seed, uint8_t checkingNeighbor) {

	// TODO: Look into using flash memory to read recently cached backgrounds instead of recalculating background each frame
	position.x = (position.x < 0) ? -1 * position.x & 511 : position.x & 511;
	// Terrain
	uint16_t terrainShift = (seed & 511);
	int32_t terrainHeight = getTerrainHeight(position, seed);

	if (position.y > terrainHeight) {
		uint8_t noiseCharacter = generateValueNoise(position, 8, seed);

		// If the terrain is not lava, flat, and has room for at least 4 characters landing pad then draw landing pad
		if (noiseCharacter != 2 && noiseCharacter != 1 && noiseCharacter != 0) {
			int32_t terrainSlope = getTerrainSlope(position, seed);

			if ((terrainSlope < 4) && (terrainSlope > -4) && (position.y < terrainHeight + 2)) {
				if (checkingNeighbor) {
					return 0b0101011111011111;
				}

				// If neigboring landing pad exists then there must be space for the rest
				position.x--;
				if (getBackgroundASCIICharacter(position, seed, 0) == 0b0101011111011111) {
					return 0b0101011111011111;
				}

				// If no neighboring landing pad exists, then check if there is room for 4+ landing pads
				position.x += 5;
				if ((getBackgroundASCIICharacter(position, seed, 1) == 0b0101011111011111)) {
					return 0b0101011111011111;
				}
			}
		}
			//TODO: REMOVE BACKGROUND GRID THINGY
		switch (noiseCharacter) {
			case(7): // White
				//return 0b0011100001000000;
				return 0b0011100010110010;

			case(6): // Light gray
				//return 0b0011100000100110;

				return 0b0011100010110001;

			case(5): // Gray
				//return 0b0011100000100100;
				return 0b0011100010110000;

			case(4): // Dark gray
				//return 0b0011100000100101;
				return 0b1011100010110001;

			case(3): // Dark dark gray
				//return 0b1011100001011000;
				return 0b1011100010110000;

			case(2): // # Dark red
				//return 0b1000100000100011;
				return 0b1000100011011011;

			case(1): // Red
				//return 0b0000100001010111;
				return 0b0000100011011011;

			case(0): // Orange
				//return 0b0001100001001101;
				return 0b0001100110110001;
		}
	}

	// Atmosphere
	else {
		switch(generateWhiteNoise(position.x, position.y, 2048, seed)) {
			case(1):
				return 0b0011100000101010; // Light *

			case(2):
				return 0b1011100000101010; // Dark *

			case(3):
				return 0b0011100000101011; // Light +

			case(4):
				return 0b1011100000101011; // Dark +

			case(5):
				return 0b0011100010011110; // Light ×

			case(6):
				return 0b1011100010011110; // Dark ×

			case(7):
				return 0b0011100011110110; // Light ÷

			case(8):
				return 0b1011100011110110; // Dark ÷

			default:
				return 0b0011100000100000; // ' ' space character
		}
	}
}

uint16_t* getBackgroundASCIIImage(vector_t position, vector_t size, uint32_t *seed, uint16_t *asciiImage, uint8_t padding) {
	// Initialize the seed for procedural generation
	static uint32_t *seedPtr = NULL;
	if (seed != NULL) {
		seedPtr = seed;
	}

	// Allocate memory to store background ascii image and set default character (no faint(1bit), no blink(1bit), white forground(3bits), black background(3bits), ' ' character)
	uint16_t *backgroundASCIIImage = malloc((size.x * size.y) * sizeof(uint16_t));
	memset(backgroundASCIIImage, 0b0011100000100000, (size.x * size.y) * sizeof(uint16_t));

	// Return blank background function if no seed was ever given
	if (seedPtr == NULL) {
		return backgroundASCIIImage;
	}

	// Wrap position around the screen x-direction
	position.x = (position.x < 0) ? (position.x & 255) - 1 : position.x & 255;

	for (uint16_t r = 0; r < size.y; r++) {
		// If character is out of bounds in y-direction, don't bother computing the background
		if (position.y + r < 0 || position.y + r > 95) {
			continue;
		}

		for (uint16_t c = 0; c < size.x; c++) {
			// Only generate background if it is visible and not occluded by the ascii image
			if ((asciiImage != NULL) && (r > padding - 1) && (r < size.y - padding) && (c > padding - 1) && (c < size.x - padding)) {
				if (((asciiImage[((r - padding) * (size.x - (padding << 1))) + (c - padding)]) & 0b0000000011111111) != 0b100000) {
					backgroundASCIIImage[r * size.x + c] = 0b0011100000100000;
					continue;
				}
			}

			// Generate background character based on position and seed
			uint8_t x = (position.x + c);
			x = (x < 0) ? (x & 255) - 1: x & 255; // Wrap current character position around the screen x-direction
			uint8_t y = position.y + r;
			vector_t backgroundASCIICharacterPosition = {x, y};
			backgroundASCIIImage[r * size.x + c] = getBackgroundASCIICharacter(backgroundASCIICharacterPosition, *seedPtr, 0);
		}
	}

	return backgroundASCIIImage;
}

void destroyBackgroundASCIIImage(uint16_t *backgroundASCIIImage) {
	// TODO: Look into using flash memory to cache recently calculated backgrounds
	free(backgroundASCIIImage);
}
