#include "lander.h"
#include "background.h"

void initLander(lander_t *lander, int16_t fuel, int8_t lives, int16_t score) {
	static uint16_t landerImage[8][12] = {
		{0x0020,0x38DB,0x0020,0x0020,
		0x27DC,0x0020,0x27DC,0x0020,
		0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x38DC,
		0x20DC,0x38DF,0x38DB,0x0020,
		0x0020,0x20DF,0x0020,0x0020},

		{0x20DC,0x38DC,0x0020,0x0020,
		0x20DC,0x38DC,0x38DF,0x38DF,
		0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x20DC,0x0020,0x0020,
		0x20DF,0x38DC,0x38DB,0x0020,
		0x0020,0x0020,0x0020,0x38DF},

		{0x0020,0x0020,0x0020,0x0020,
		0x0020,0x27DF,0x0020,0x27DF,
		0x0020,0x0020,0x38DB,0x0020},

		{0x0020,0x0020,0x20DC,0x0020,
		0x0020,0x38DB,0x38DC,0x20DF,
		0x38DF,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x38DC,0x20DC,
		0x38DF,0x38DF,0x38DC,0x20DC,
		0x0020,0x0020,0x0020,0x0020},

		{0x38DC,0x0020,0x0020,0x0020,
		0x0020,0x38DB,0x38DF,0x20DC,
		0x0020,0x0020,0x20DF,0x0020}
	};
	static vector_t landerOffsets[8] = {
		{0, 0},
		{0, 0},
		{1, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{-1, 0},
		{0, 0}
	};
	static vector_t landerSize = {4, 3};

	lander->image = landerImage;
	lander->offsets = landerOffsets;
	lander->size = landerSize;

	vector_t position = {10000, 10000};
	vector_t velocity = {500, 10};
	lander->position = position;
	lander->velocity = velocity;
	lander->direction = 0;
	lander->fuel = fuel;
	lander->lives = lives;
	lander->score = score;
	lander->exploding = 0;
	lander->landed = 0;
}

void explodeLander(lander_t *lander) {
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

static void updateInput(lander_t *lander, uint32_t deltaTime, uint8_t joystickState) {
	if (joystickState & JOYSTICK_LEFT) {
		// Rotate counter-clockwise
		lander->direction -= (deltaTime << 1);
		if (lander->direction < 0) { // Wrap direction value
			lander->direction = 127;
		}
	}
	if (joystickState & JOYSTICK_RIGHT) {
		// Rotate clockwise
		lander->direction += (deltaTime << 1);
		if (lander->direction > 127) { // Wrap direction value
			lander->direction = 0;
		}
	}
	if (joystickState & JOYSTICK_UP) {
		// Boost if lander has fuel
		if (lander->fuel > 0) {
			lander->fuel -= deltaTime;

			// Update velocity components based on lander orientation
			lander->velocity.x += (sinLUT((lander->direction >> 4) << 6) >> 11) * deltaTime;
			lander->velocity.y -= (cosLUT((lander->direction >> 4) << 6) >> 11) * deltaTime;

			// TODO: Spawn fire boost particle if deltaTime boost particle cooldown
		} else {
			// TODO: Spawn smoke particle if deltaTime boost particle cooldown
		}
	}
}

static void updatePhysics(lander_t *lander, uint32_t deltaTime) {
	lander->velocity.y += deltaTime; // Gravity

	lander->velocity.x = (lander->velocity.x > 500) ? 500 : lander->velocity.x; // Cap velocity
	lander->velocity.x = (lander->velocity.x < -500) ? -500 : lander->velocity.x; // Cap velocity
	lander->velocity.y = (lander->velocity.y > 500) ? 500 : lander->velocity.y; // Cap velocity
	lander->velocity.y = (lander->velocity.y < -500) ? -500 : lander->velocity.y; // Cap velocity

	lander->position.x += lander->velocity.x * deltaTime; // Update position
	lander->position.y += lander->velocity.y * deltaTime;
	lander->position.x = (lander->position.x < 0) ? (lander->position.x & 262143) - 1 : (lander->position.x & 262143); // Wrap x position
}

static void checkCollisions(lander_t *lander, uint32_t deltaTime) {
	vector_t landerCollisionPosition = {(lander->position.x >> 10) + lander->offsets[lander->direction >> 4].x + (lander->size.x >> 1), (lander->position.y >> 10) + lander->offsets[lander->direction >> 4].y + lander->size.y - 1};

	// Terrain collision
	if (landerCollisionPosition.y > getTerrainHeight(landerCollisionPosition)) {
		// If lander landed successfully
		if (((lander->direction >> 7) == 0) && (getBackgroundASCIICharacter(landerCollisionPosition) == LANDING_PAD) && (lander->velocity.x < 100) && (lander->velocity.x > -100) && (lander->velocity.y < 150) && (lander->velocity.y > 0)) {
			lander->velocity.x = 0;
			lander->velocity.y = 0;

			lander->landed = 1;
		}

		// If lander crashed
		else {
			explodeLander(lander);
		}
	}
}

void updateLander(lander_t *lander, uint32_t deltaTime, uint8_t joystickState) {
	updateInput(lander, deltaTime, joystickState);
	updatePhysics(lander, deltaTime);
	checkCollisions(lander, deltaTime);

	// Update exploding status
	if (lander->exploding == 1) {
		lander->exploding = updateExplosion(&(lander->explosion), deltaTime);
	}

	// Scale lander position to fit onto display
	vector_t displayPosition = {(lander->position.x >> 10) + lander->offsets[lander->direction >> 4].x, (lander->position.y >> 10) + lander->offsets[lander->direction >> 4].y};

	addToBuffer(lander->image[lander->direction >> 4], displayPosition, lander->size);
}
