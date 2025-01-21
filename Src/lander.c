#include "lander.h"
#include "renderer.h"
#include "explosion.h"

lander_t initLander(lander_t *lander) {
	vector_t position = {10000, 10000};
	vector_t velocity = {500, 10};
	lander->position = position;
	lander->velocity = velocity;
	lander->direction = 0;
	lander->fuel = 1000;
	lander->lives = 3;
	lander->score = 0;
	lander->exploding = 0;
	lander->explosion;
}

void updateLander(lander_t *lander, uint32_t deltaTime, uint8_t joystickState, uint32_t seed) {
	static const uint16_t landerImage[8][12] = {
		{0x3820,0x38DB,0x3820,0x3820,
		0x38DB,0x3820,0x38DB,0x3820,
		0x3820,0x3820,0x3820,0x3820},

		{0x3820,0x3820,0x3820,0x38DC,
		0x38DC,0x38DF,0x38DB,0x3820,
		0x3820,0x38DF,0x3820,0x3820},

		{0x38DC,0x38DC,0x3820,0x3820,
		0x38DC,0x38DC,0x38DF,0x38DF,
		0x3820,0x3820,0x3820,0x3820},

		{0x3820,0x38DC,0x3820,0x3820,
		0x38DF,0x38DC,0x38DB,0x3820,
		0x3820,0x3820,0x3820,0x38DF},

		{0x3820,0x3820,0x3820,0x3820,
		0x3820,0x38DB,0x3820,0x38DB,
		0x3820,0x3820,0x38DB,0x3820},

		{0x3820,0x3820,0x38DC,0x3820,
		0x3820,0x38DB,0x38DC,0x38DF,
		0x38DF,0x3820,0x3820,0x3820},

		{0x3820,0x3820,0x38DC,0x38DC,
		0x38DF,0x38DF,0x38DC,0x38DC,
		0x3820,0x3820,0x3820,0x3820},

		{0x38DC,0x3820,0x3820,0x3820,
		0x3820,0x38DB,0x38DF,0x38DC,
		0x3820,0x3820,0x38DF,0x3820}
	};

	static const int8_t landerOffsets[8][2] = {
			{0, 0},
			{0, 0},
			{1, 0},
			{0, 0},
			{0, 0},
			{0, 0},
			{-1, 0},
			{0, 0}
	};

	static const vector_t landerImageSize = {4, 3};

	// Joystick left is held - Rotate Counter-clockwise
	if (joystickState & 0b00000100) { // LEFT
		lander->direction -= deltaTime * 2;
		if (lander->direction < 0) {
			lander->direction = 127;
		}
	}

	// Joystick right is held - Rotate clockwise
	if (joystickState & 0b00001000) { // RIGHT
		lander->direction += deltaTime * 2;
		if (lander->direction > 127) {
			lander->direction = 0;
		}
	}

	// Joystick up is held - Boost
	if (joystickState & 0b00000001) { // UP
		// Only boost if lander has fuel
		if (lander->fuel > 0) {
			lander->velocity.x += (sinLUT((lander->direction >> 4) << 6) >> 11) * deltaTime;
			lander->velocity.x = (lander->velocity.x > 500) ? 500 : lander->velocity.x;
			lander->velocity.x = (lander->velocity.x < -500) ? -500 : lander->velocity.x;

			lander->velocity.y -= (cosLUT((lander->direction >> 4) << 6) >> 11) * deltaTime;
			lander->velocity.y = (lander->velocity.y > 500) ? 500 : lander->velocity.y;
			lander->velocity.y = (lander->velocity.y < -500) ? -500 : lander->velocity.y;

			lander->fuel -= deltaTime;
			//TODO: If runtime division is even then spawn fire particle
		} else {
			// TODO: If runtime division / 4 is even then spawn smoke particle
		}
	}

	// Apply velocity
	lander->position.x += lander->velocity.x * deltaTime;
	lander->position.y += lander->velocity.y * deltaTime;

	// Apply gravity
	lander->velocity.y += deltaTime;

	vector_t displayPosition = {(lander->position.x >> 10) + landerOffsets[lander->direction >> 4][0], (lander->position.y >> 10) + landerOffsets[lander->direction >> 4][1]};

	vector_t collisionPosition;
	collisionPosition.y = displayPosition.y + landerImageSize.y - 2;
	collisionPosition.x = displayPosition.x + (landerImageSize.x >> 1);
	if (collisionPosition.y > getTerrainHeight(collisionPosition, seed)) {
		if (((lander->direction >> 7) == 0) && (getBackgroundASCIICharacter(collisionPosition, seed, 0) == 0b0101011111011111) && (lander->velocity.x < 100) && (lander->velocity.x > -100) && (lander->velocity.y < 150) && (lander->velocity.y > 0)) {
			// TODO: Successful landing
			printf("%c[2J TErrain!", ESC);
		} else {
			lander->velocity.x = 0;
			lander->velocity.y = 0;

			if (lander->exploding == 0) {
				lander->exploding = 1;
				initExplosion(&(lander->explosion), lander->position);
			}

			lander->position.x = 10000;
			lander->position.y = 10000;
			lander->velocity.x = 500;
			lander->velocity.y = 10;
			lander->direction = 0;
			lander->fuel = (lander->fuel < 100) ? 100 : lander->fuel;
		}
	}

	if (lander->exploding == 1) {
		lander->exploding = updateExplosion(&(lander->explosion), deltaTime);
	}

	// Convert actual lander position into closest position for display
	drawASCIIImageWithBackground(landerImage[lander->direction >> 4], displayPosition, landerImageSize);
}
