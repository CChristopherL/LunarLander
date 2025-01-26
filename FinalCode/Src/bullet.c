#include "bullet.h"
#include "renderer.h"

// Initializes the bullet
void initBullet(bullet_t *bullet, vector_t position, vector_t velocity) {
	static uint16_t bulletImage[2][1] = {
		{0x10FE},
		{0x0020}
	};
	static vector_t bulletSize = {1, 1};

	bullet->image = bulletImage;
	bullet->imageSize = bulletSize;

	bullet->position = position;
	bullet->velocity = velocity;
	bullet->frame = (1 << 7);

	bullet->shadowPosition = (vector_t){0, 0};
}

// Updates the particle's physics
static void updatePhysics(bullet_t *bullet, uint32_t deltaTime) {
	// Apply gravity
	//particle->velocity.y += (deltaTime << 2);

	// Apply velocity
	bullet->shadowPosition.x = bullet->position.x;
	bullet->shadowPosition.y = bullet->position.y;

	bullet->position.x += bullet->velocity.x * deltaTime;
	bullet->position.y += bullet->velocity.y * deltaTime;
}

// Check collision with terrain
static void checkCollisions(bullet_t *bullet, uint32_t deltaTime) {
	vector_t bulletCollisionPosition = convertToDisplayPosition(bullet->position);

	// Terrain collision
	if (bulletCollisionPosition.y > getTerrainHeight(bulletCollisionPosition)) {
		bullet->frame = 1 << 7;
	}
}

// Called continuously updating particle
void updateBullet(bullet_t *bullet, uint32_t deltaTime) {
	// If the particle is visible (last frame is the blank invisible frame)
	if ((bullet->frame >> 7) > 0) {
		return;
	}

	// Update physics
	updatePhysics(bullet, deltaTime);

	// Check terrain collisions
	checkCollisions(bullet, deltaTime);

	// Update frame
	bullet->frame += deltaTime;

	// Add the bullet to the buffer to be drawn
	vector_t displayPosition = convertToDisplayPosition(bullet->position);
	addToBuffer(bullet->image[(bullet->frame) >> 7], displayPosition, bullet->imageSize);

	// Add the bullet's shadow to the buffer to erase previous bullets if the bullet is currently active
	vector_t shadowDisplayPosition = convertToDisplayPosition(bullet->shadowPosition);
	addToBuffer(bullet->image[1], shadowDisplayPosition, bullet->imageSize);
}
