#include "lander.h"
#include "background.h"
#include "renderer.h"

void initLander(lander_t *lander, int16_t fuel, int8_t lives, int16_t score, uint8_t dockingSize) {
	static uint16_t landerImage[3][8][24] = {
		{{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x38DB,0x0020,0x0020,0x0020,
		0x0020,0x2FDF,0x0020,0x2FDF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x38DC,0x0020,
		0x0020,0x38DC,0x28DF,0x2FDC,0x0020,0x0020,
		0x0020,0x0020,0x38DF,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x38DC,0x28DC,0x0020,0x0020,0x0020,
		0x0020,0x38DC,0x28DC,0x38DF,0x38DF,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x38DC,0x0020,0x0020,0x0020,
		0x0020,0x38DF,0x28DC,0x2FDF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x38DF,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x2FDC,0x0020,0x2FDC,0x0020,
		0x0020,0x0020,0x0020,0x38DB,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x38DC,0x0020,0x0020,
		0x0020,0x0020,0x2FDF,0x28DC,0x38DF,0x0020,
		0x0020,0x38DF,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x28DC,0x38DC,0x0020,
		0x0020,0x38DF,0x38DF,0x28DC,0x38DC,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x38DC,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x2FDC,0x28DF,0x38DC,0x0020,
		0x0020,0x0020,0x0020,0x38DF,0x0020,0x0020}},


		{{0x0020,0x0020,0x0020,0x38DC,0x0020,0x0020,
		0x0020,0x0020,0x28DC,0x38DF,0x28DC,0x0020,
		0x0020,0x0020,0x2FDC,0x0020,0x2FDC,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x38DC,
		0x0020,0x0020,0x38DC,0x28DF,0x2FDC,0x0020,
		0x0020,0x28DF,0x28DC,0x38DF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x28DF,0x38DF,0x28DF,0x38DC,0x38DC,
		0x0020,0x28DF,0x38DF,0x28DF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x28DC,0x0020,0x0020,0x0020,
		0x0020,0x28DF,0x38DC,0x38DF,0x28DC,0x0020,
		0x0020,0x0020,0x0020,0x28DF,0x38DF,0x38DC,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x28DC,0x0020,0x28DC,0x0020,
		0x0020,0x0020,0x2FDC,0x0020,0x2FDC,0x0020,
		0x0020,0x0020,0x0020,0x38DB,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x28DC,0x0020,
		0x0020,0x0020,0x28DC,0x38DF,0x38DC,0x28DF,
		0x0020,0x38DC,0x38DF,0x28DF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x38DC,0x38DC,0x28DF,0x38DF,0x28DF,
		0x0020,0x0020,0x0020,0x28DF,0x38DF,0x28DF,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x38DC,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x2FDC,0x28DF,0x38DC,0x0020,
		0x0020,0x0020,0x0020,0x38DF,0x28DC,0x28DF,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020}},


		{{0x0020,0x0020,0x0020,0x38DB,0x0020,0x0020,
		0x0020,0x0020,0x2FDF,0x0020,0x2FDF,0x0020,
		0x0020,0x0020,0x2FDF,0x0020,0x2FDF,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x28DC,0x38DC,0x38DF,
		0x0020,0x28DC,0x38DF,0x38DC,0x28DF,0x0020,
		0x38DF,0x38DC,0x28DF,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x38DF,0x28DF,0x38DF,0x28DF,0x38DC,0x38DC,
		0x38DF,0x28DF,0x38DF,0x28DF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x38DC,0x38DF,0x28DC,0x0020,0x0020,0x0020,
		0x0020,0x28DF,0x38DC,0x38DF,0x28DC,0x0020,
		0x0020,0x0020,0x0020,0x28DF,0x38DF,0x38DC,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x2FDC,0x0020,0x2FDC,0x0020,0x0020,
		0x0020,0x2FDC,0x0020,0x2FDC,0x0020,0x0020,
		0x0020,0x0020,0x38DB,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x28DC,0x38DF,0x38DC,
		0x0020,0x28DC,0x38DF,0x38DC,0x28DF,0x0020,
		0x38DC,0x38DF,0x28DF,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x38DC,0x38DC,0x28DF,0x38DF,0x28DF,0x38DF,
		0x0020,0x0020,0x28DF,0x38DF,0x28DF,0x38DF,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

		{0x38DF,0x38DC,0x28DC,0x0020,0x0020,0x0020,
		0x0020,0x28DF,0x38DC,0x38DF,0x28DC,0x0020,
		0x0020,0x0020,0x0020,0x28DF,0x38DC,0x38DF,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020}}
	};
	static vector_t landerImageOffsets[3][8] = {
		{(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0}},

		{(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0}},

		{(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0},
		(vector_t){0, 0}}
	};
	static vector_t landerImageSize = {6, 4};

	lander->image = landerImage;
	lander->imageOffsets = landerImageOffsets;
	lander->imageSize = landerImageSize;
	lander->dockingSize = dockingSize;

	vector_t position = {10000, 10000};
	vector_t velocity = {500, 10};
	lander->position = position;
	lander->velocity = velocity;
	lander->direction = 0;
	lander->fuel = fuel;
	lander->lives = lives;
	lander->score = score;
	lander->landed = 0;

	for (uint8_t i = 0; i < 2; i++) {
		initExplosion(&(lander->explosions[i]), (vector_t){0, 0});
	}

	static vector_t boostParticleOffsets[3][8] = {
		{(vector_t){2, 2},
		(vector_t){1, 3},
		(vector_t){0, 2},
		(vector_t){1, 2},
		(vector_t){3, 2},
		(vector_t){4, 1},
		(vector_t){5, 2},
		(vector_t){4, 3}},

		{(vector_t){3, 2},
		(vector_t){1, 2},
		(vector_t){1, 1},
		(vector_t){1, 0},
		(vector_t){3, 0},
		(vector_t){5, 1},
		(vector_t){6, 1},
		(vector_t){5, 2}},

		{(vector_t){3, 2},
		(vector_t){0, 3},
		(vector_t){0, 1},
		(vector_t){0, 0},
		(vector_t){2, 0},
		(vector_t){5, 0},
		(vector_t){6, 1},
		(vector_t){5, 2}}
	};
	lander->boostParticleOffsets = boostParticleOffsets;
	lander->boostParticleDebounce = 0;
	lander->boostParticleCount = 6;
	for (uint8_t i = 0; i < lander->boostParticleCount; i++) {
		initParticle(&(lander->boostParticles[i]), (vector_t){0, 0}, (vector_t){0, 0});
	}

	static vector_t bulletOffsets[3][8] = {
		{(vector_t){2, 2},
		(vector_t){1, 3},
		(vector_t){0, 2},
		(vector_t){1, 2},
		(vector_t){3, 2},
		(vector_t){4, 1},
		(vector_t){5, 2},
		(vector_t){4, 3}},

		{(vector_t){3, 2},
		(vector_t){1, 2},
		(vector_t){1, 1},
		(vector_t){1, 0},
		(vector_t){3, 0},
		(vector_t){5, 1},
		(vector_t){6, 1},
		(vector_t){5, 2}},

		{(vector_t){3, 2},
		(vector_t){0, 3},
		(vector_t){0, 1},
		(vector_t){0, 0},
		(vector_t){2, 0},
		(vector_t){5, 0},
		(vector_t){6, 1},
		(vector_t){5, 2}}
	};
	lander->bulletOffsets = bulletOffsets;
	lander->bulletDebounce = 32;
	lander->bulletCount = 6;
	lander->bulletType = 2;
	for (uint8_t i = 0; i < lander->bulletCount; i++) {
		initBullet(&(lander->bullets[i]), (vector_t){0, 0}, (vector_t){0, 0});
	}
}

void explodeLander(lander_t *lander) {
	lander->velocity.x = 0;
	lander->velocity.y = 0;

	for (uint8_t i = 0; i < 2; i++) {
		if ((lander->explosions[i].frame >> 4) > 3) {
			initExplosion(&(lander->explosions[i]), lander->position);
			lander->explosions[i].frame = 0;
			break;
		}
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
		// Rotate lander counter-clockwise
		lander->direction -= (deltaTime << 1);
		if (lander->direction < 0) { // Wrap direction value
			lander->direction = 127;
		}
	}
	if (joystickState & JOYSTICK_RIGHT) {
		// Rotate lander clockwise
		lander->direction += (deltaTime << 1);
		if (lander->direction > 127) { // Wrap direction value
			lander->direction = 0;
		}
	}

	lander->bulletDebounce += deltaTime;
	if (joystickState & BUTTON_1) {
		// Spawn a bullet based on the current bullet type
		if (lander->bulletType == 0 && lander->bulletDebounce > 64) { // Single-bullet type
			for (uint8_t i = 0; i < lander->bulletCount; i++) {
				if ((lander->bullets[i].frame >> 7) > 0) {
					int16_t thetaX = (lander->direction >> 4) << 6;
					int16_t thetaY = (lander->direction >> 4) << 6;
					vector_t bulletDirection = {(sinLUT(thetaX) >> 10), -(cosLUT(thetaY) >> 10)};
					initBullet(&(lander->bullets[i]), (vector_t){lander->position.x + (lander->bulletOffsets[lander->dockingSize][lander->direction >> 4].x << 10), lander->position.y + (lander->bulletOffsets[lander->dockingSize][lander->direction >> 4].y << 10)}, (vector_t){bulletDirection.x * 128, bulletDirection.y * 64});
					lander->bullets[i].frame = 0;
					lander->bulletDebounce = 0;
					break;
				}
			}
		} else if (lander->bulletType == 1 && lander->bulletDebounce > 64) { // Spray-bullet type
			uint8_t burstBulletCount = 0;
			for (uint8_t i = 0; i < lander->bulletCount; i++) {
				if ((lander->bullets[i].frame >> 7) > 0) {
					int16_t thetaX = ((lander->direction >> 4) << 6) - 16 + (16 * burstBulletCount);
					int16_t thetaY = ((lander->direction >> 4) << 6) - 16 + (16 * burstBulletCount);
					vector_t bulletDirection = {(sinLUT(thetaX) >> 10), -(cosLUT(thetaY) >> 10)};
					initBullet(&(lander->bullets[i]), (vector_t){lander->position.x + (lander->bulletOffsets[lander->dockingSize][lander->direction >> 4].x << 10), lander->position.y + (lander->bulletOffsets[lander->dockingSize][lander->direction >> 4].y << 10)}, (vector_t){bulletDirection.x * 128, bulletDirection.y * 64});
					lander->bullets[i].frame = 0;
					lander->bulletDebounce = 0;
					burstBulletCount++;
					if (burstBulletCount > 2) {
						break;
					}
					continue;
				}
			}
		} else if (lander->bulletType == 2 && lander->bulletDebounce > 128) { // Radial-bullet type
			uint8_t burstBulletCount = 0;
			for (uint8_t i = 0; i < lander->bulletCount; i++) {
				if ((lander->bullets[i].frame >> 7) > 0) {
					int16_t thetaX = ((lander->direction >> 4) << 6) + (85 * burstBulletCount);
					int16_t thetaY = ((lander->direction >> 4) << 6) + (85 * burstBulletCount);
					vector_t bulletDirection = {(sinLUT(thetaX) >> 10), -(cosLUT(thetaY) >> 10)};
					initBullet(&(lander->bullets[i]), (vector_t){lander->position.x + (lander->bulletOffsets[lander->dockingSize][lander->direction >> 4].x << 10), lander->position.y + (lander->bulletOffsets[lander->dockingSize][lander->direction >> 4].y << 10)}, (vector_t){bulletDirection.x * 128, bulletDirection.y * 64});
					lander->bullets[i].frame = 0;
					lander->bulletDebounce = 0;
					burstBulletCount++;
					if (burstBulletCount > 5) {
						break;
					}
					continue;
				}
			}
		}
	}
	if (joystickState & BUTTON_2) {
		// Boost if lander has fuel
		if (lander->fuel > 0) {
			lander->fuel -= deltaTime;

			// Update velocity components based on lander orientation
			vector_t acceleration = {(sinLUT((lander->direction >> 4) << 6) >> 11), -(cosLUT((lander->direction >> 4) << 6) >> 11)};
			lander->velocity.x += acceleration.x * deltaTime;
			lander->velocity.y += acceleration.y * deltaTime;

			// Spawn a boost particle every 640ms
			lander->boostParticleDebounce += deltaTime;
			if (lander->boostParticleDebounce > 1) {
				for (uint8_t i = 0; i < lander->boostParticleCount; i++) {
					if ((lander->boostParticles[i].frame >> 4) > 3) {
						int16_t thetaX = ((lander->direction >> 4) << 6) + (generateWhiteNoise((vector_t){lander->fuel, lander->fuel}, 64, lander->fuel) - 32);
						int16_t thetaY = ((lander->direction >> 4) << 6) + (generateWhiteNoise((vector_t){lander->fuel, lander->fuel}, 32, lander->fuel + 812) - 16);
						vector_t boostDirection = {-(sinLUT(thetaX) >> 11), (cosLUT(thetaY) >> 11)};
						initParticle(&(lander->boostParticles[i]), (vector_t){lander->position.x + (lander->boostParticleOffsets[lander->dockingSize][lander->direction >> 4].x << 10), lander->position.y + (lander->boostParticleOffsets[lander->dockingSize][lander->direction >> 4].y << 10)}, (vector_t){boostDirection.x * 48, boostDirection.y * 48});
						lander->boostParticles[i].frame = 0;
						lander->boostParticleDebounce = 0;
						break;
					}
				}
			}
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
	vector_t landerCollisionPosition = {(lander->position.x >> 10) + lander->imageOffsets[lander->dockingSize][lander->direction >> 4].x + (lander->imageSize.x >> 1), (lander->position.y >> 10) + lander->imageOffsets[lander->dockingSize][lander->direction >> 4].y + lander->imageSize.y - 1};

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

static void updateBoostParticles(lander_t *lander, uint32_t deltaTime) {
	for (uint8_t i = 0; i < lander->boostParticleCount; i++) {
		updateParticle(&(lander->boostParticles[i]), deltaTime);
	}
}

static void updateExplosions(lander_t *lander, uint32_t deltaTime) {
	for (uint8_t i = 0; i < 2; i++) {
		updateExplosion(&(lander->explosions[i]), deltaTime);
	}
}

static void updateBullets(lander_t *lander, uint32_t deltaTime) {
	for (uint8_t i = 0; i < lander->bulletCount; i++) {
		updateBullet(&(lander->bullets[i]), deltaTime);
	}
}

void updateLander(lander_t *lander, uint32_t deltaTime, uint8_t joystickState) {
	// Update lander
	updateInput(lander, deltaTime, joystickState);
	updatePhysics(lander, deltaTime);
	checkCollisions(lander, deltaTime);

	// Update boost particles
	updateBoostParticles(lander, deltaTime);

	// Update bullets
	updateBullets(lander, deltaTime);

	// Update explosions
	updateExplosions(lander, deltaTime);

	// Scale lander position to fit onto display
	vector_t displayPosition = {(lander->position.x >> 10) + lander->imageOffsets[lander->dockingSize][lander->direction >> 4].x, (lander->position.y >> 10) + lander->imageOffsets[lander->dockingSize][lander->direction >> 4].y};
	addToBuffer(lander->image[lander->dockingSize][lander->direction >> 4], displayPosition, lander->imageSize);
}
