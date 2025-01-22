#include "30010_io.h"
#include "stm32f30x_conf.h"

#include "background.h"
#include "renderer.h"
#include "noise.h"



void initBackground(uint16_t *level, uint32_t *seed) {
	// Split background up into chunks and write them to the screen
	vector_t chunkPosition = {0, 0};
	vector_t chunkSize = {16, 16};

	// Create chunks
	for (uint8_t chunk = 0; chunk < 96; chunk++) {
		chunkPosition.x = (chunk & 15) << 4;
		chunkPosition.y = (chunk >> 4) << 4;

		uint16_t *chunkASCIIImage = getBackgroundASCIIImage(chunkPosition, chunkSize, level, seed, NULL, 0);
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

uint16_t getBackgroundASCIICharacter(vector_t position, uint16_t level, uint32_t seed, uint8_t checkingNeighbor) {
	//TODO: Define level colors
	static const starTypes[9] = {STAR_AST_LIGHT, STAR_AST_DARK, STAR_PLUS_LIGHT, STAR_PLUS_DARK, STAR_TIMES_LIGHT, STAR_TIMES_DARK, STAR_DIV_LIGHT, STAR_DIV_DARK, SPACE};
	static const terrainTypes[4][8] = {
			{MEDIUM_GRAY, GRAY, LIGHT_GRAY, BRIGHT_GRAY, LIGHT_GRAY, GRAY, MEDIUM_GRAY, GRAY},
			{ORANGE, RED, DARK_RED, DARK_GRAY, MEDIUM_GRAY, GRAY, LIGHT_GRAY, BRIGHT_GRAY},
			{CYAN, LIGHT_CYAN, CYAN, WHITE, MEDIUM_GRAY, GRAY, LIGHT_GRAY, BRIGHT_GRAY},
			{LIGHT_CYAN, CYAN, BLUE_CYAN, BLUE_GRAY, MEDIUM_GRAY, GRAY, LIGHT_GRAY, BRIGHT_GRAY}
	};

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
					return LANDING_PAD;
				}

				// If neigboring landing pad exists then there must be space for the rest
				position.x--;
				if (getBackgroundASCIICharacter(position, level, seed, 0) == LANDING_PAD) {
					return LANDING_PAD;
				}

				// If no neighboring landing pad exists, then check if there is room for 4+ landing pads
				position.x += 5;
				if ((getBackgroundASCIICharacter(position, level, seed, 1) == LANDING_PAD)) {
					return LANDING_PAD;
				}
			}
		}
		return terrainTypes[level][noiseCharacter];
	}

	// Atmosphere
	else {
		uint16_t starTypeIndex = generateWhiteNoise(position.x, position.y, 2048, seed);
		if (starTypeIndex < 8) {
			return starTypes[starTypeIndex];
		} else {
			return SPACE;
		}
	}
}

uint16_t* getBackgroundASCIIImage(vector_t position, vector_t size, uint16_t *level, uint32_t *seed, uint16_t *asciiImage, uint8_t padding) {
	// Initialize the seed for procedural generation
	static uint32_t *seedPtr = NULL;
	static uint16_t *levelPtr = NULL;
	if (seed != NULL) {
		seedPtr = seed;
	}
	if (level != NULL) {
		levelPtr = level;
	}

	// Allocate memory to store background ascii image and set default character (no faint(1bit), no blink(1bit), white forground(3bits), black background(3bits), ' ' character)
	uint16_t *backgroundASCIIImage = malloc((size.x * size.y) * sizeof(uint16_t));
	memset(backgroundASCIIImage, 0b0011100000100000, (size.x * size.y) * sizeof(uint16_t));

	// Return blank background function if no seed was ever given
	if ((seedPtr == NULL) || (levelPtr == NULL)) {
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
			backgroundASCIIImage[r * size.x + c] = getBackgroundASCIICharacter(backgroundASCIICharacterPosition, *levelPtr, *seedPtr, 0);
		}
	}

	return backgroundASCIIImage;
}

void destroyBackgroundASCIIImage(uint16_t *backgroundASCIIImage) {
	// TODO: Look into using flash memory to cache recently calculated backgrounds
	free(backgroundASCIIImage);
}
