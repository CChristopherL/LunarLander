#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "background.h"
#include "renderer.h"
#include "noise.h"

void initBackground(uint16_t *level, uint32_t *seed) {
	// Display entire background by displaying background divided up into 16x16 chunks
	vector_t chunkPosition = {0, 0};
	vector_t chunkSize = {16, 16};
	for (uint8_t chunk = 0; chunk < 96; chunk++) {
		chunkPosition.x = (chunk & 15) << 4;
		chunkPosition.y = (chunk >> 4) << 4;

		uint16_t *chunkASCIIImage = getBackgroundASCIIImage(chunkPosition, chunkSize);
		drawASCIIImage(chunkASCIIImage, chunkPosition, chunkSize);
		destroyBackgroundASCIIImage(chunkASCIIImage);
	}
}

uint32_t getSeed(uint32_t *seed) {
	static uint32_t *seedPtr = NULL;

	if (seed != NULL) {
		seedPtr = seed;
	}

	return (seedPtr == NULL) ? 0 : *seedPtr;
}

uint16_t getLevel(uint16_t *level) {
	static uint16_t *levelPtr = NULL;

	if (level != NULL) {
		levelPtr = level;
	}

	return (levelPtr == NULL) ? 0 : *levelPtr;
}

int32_t getTerrainHeight(vector_t position) {
	uint16_t seed = getSeed(NULL);
	uint16_t terrainShift = (seed & 511);
	return 87 - ((sinLUT((position.x << 3) + (terrainShift << 4)) - cosLUT(position.x << 2) + (terrainShift << 3)) >> 13);
}

int32_t getTerrainSlope(vector_t position) {
	uint16_t seed = getSeed(NULL);
	uint16_t terrainShift = (seed & 511);
	return -((cosLUT((position.x << 3) + (terrainShift << 4)) + (sinLUT(position.x << 2) << 2) << 3) >> 13);
}

uint16_t canPositionContainLandingPad(vector_t position) {
	uint32_t seed = getSeed(NULL);
	uint16_t level = getLevel(NULL);

	// Return if this position can contain a landing pad
	int32_t terrainHeight = getTerrainHeight(position);
	uint8_t characterId = generateValueNoise(position, 8, seed);
	int32_t terrainSlope = getTerrainSlope(position);
	if (position.y > terrainHeight && characterId > 2 && (terrainSlope < 4) && (terrainSlope > -4) && (position.y < terrainHeight + 2)) {
		return 1;
	}
	return 0;

}

uint16_t getBackgroundASCIICharacter(vector_t position) {
	// Background character types
	static const uint16_t starTypes[9] = {STAR_AST_LIGHT, STAR_AST_DARK, STAR_PLUS_LIGHT, STAR_PLUS_DARK, STAR_TIMES_LIGHT, STAR_TIMES_DARK, STAR_DIV_LIGHT, STAR_DIV_DARK, SPACE};
	static const uint16_t terrainTypes[4][8] = {
		{MEDIUM_GRAY, GRAY, LIGHT_GRAY, BRIGHT_GRAY, LIGHT_GRAY, GRAY, MEDIUM_GRAY, GRAY},
		{ORANGE, RED, DARK_RED, DARK_GRAY, MEDIUM_GRAY, GRAY, LIGHT_GRAY, BRIGHT_GRAY},
		{CYAN, LIGHT_CYAN, CYAN, WHITE, MEDIUM_GRAY, GRAY, LIGHT_GRAY, BRIGHT_GRAY},
		{LIGHT_CYAN, CYAN, BLUE_CYAN, BLUE_GRAY, MEDIUM_GRAY, GRAY, LIGHT_GRAY, BRIGHT_GRAY}
	};

	uint32_t seed = getSeed(NULL);
	uint16_t level = getLevel(NULL);

	// TODO: Look into using flash memory to read recently cached backgrounds instead of recalculating background each frame

	// Wrap x position around screen
	position.x = (position.x < 0) ? -1 * position.x & 511 : position.x & 511;

	// Character will be terrain
	int32_t terrainHeight = getTerrainHeight(position);
	if (position.y > terrainHeight) {
		uint8_t characterId = generateValueNoise(position, 8, seed);

		// Determine if current character can be a landing pad
		if (canPositionContainLandingPad(position)) {
			// If left-neigboring character is a landing pad, then this character should also be a landing pad
			position.x--;
			if (getBackgroundASCIICharacter(position) == LANDING_PAD) {
				return LANDING_PAD;
			}

			// If no left-neighboring landing pad exists, then check if there is room for 5 landing pads to the right
			position.x += 5;
			if ((canPositionContainLandingPad(position) == LANDING_PAD)) {
				return LANDING_PAD; // If there is room for 5 landing pads, make the left most character a landing pad
			}
		}

		// Set the terrain surface to be landing pad if the following are true: the surface color is correct, the slope is nearly flat, and the y-position is on the terrain surface
		if (characterId > 2) {
			int32_t terrainSlope = getTerrainSlope(position);
			if ((terrainSlope < 4) && (terrainSlope > -4) && (position.y < terrainHeight + 2)) {

			}
		}

		return terrainTypes[level][characterId];
	}

	// Character will be atmosphere
	else {
		uint16_t starTypeIndex = generateWhiteNoise(position, 2048, seed);
		if (starTypeIndex < 8) {
			return starTypes[starTypeIndex];
		} else {
			return SPACE;
		}
	}
}

uint16_t* getBackgroundASCIIImage(vector_t position, vector_t size) {
	// Allocate memory to store background ascii image and set default character (no faint(1bit), no blink(1bit), white forground(3bits), black background(3bits), character(8bits))
	uint16_t *backgroundASCIIImage = malloc((size.x * size.y) * sizeof(uint16_t));
	memset(backgroundASCIIImage, SPACE, (size.x * size.y) * sizeof(uint16_t));

	for (uint16_t r = 0; r < size.y; r++) {
		// If character is out of bounds in y-direction, don't bother computing the background
		if (position.y + r < 0 || position.y + r > 95) {
			continue;
		}

		for (uint16_t c = 0; c < size.x; c++) {
			// Only generate background if it is visible and not occluded by the ascii image
			/*if ((asciiImage != NULL) && (r > padding - 1) && (r < size.y - padding) && (c > padding - 1) && (c < size.x - padding)) {
				if (((asciiImage[((r - padding) * (size.x - (padding << 1))) + (c - padding)]) & 0b0000000011111111) != 0b100000) {
					backgroundASCIIImage[r * size.x + c] = 0b0011100000100000;
					continue;
				}
			}*/

			// Generate background character based on position and seed
			uint8_t x = (position.x + c);
			x = (x < 0) ? (x & 255) - 1: x & 255; // Wrap current character position around the screen x-direction
			uint8_t y = position.y + r;
			vector_t backgroundASCIICharacterPosition = {x, y};
			backgroundASCIIImage[r * size.x + c] = getBackgroundASCIICharacter(backgroundASCIICharacterPosition);
		}
	}

	return backgroundASCIIImage;
}

void destroyBackgroundASCIIImage(uint16_t *backgroundASCIIImage) {
	// TODO: Look into using flash memory to cache recently calculated backgrounds
	free(backgroundASCIIImage);
}
