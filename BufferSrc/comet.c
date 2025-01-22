#include "comet.h"
#include "renderer.h"
#include "debris.h"
#include "noise.h"

void initComet(comet_t *comet, vector_t position, vector_t velocity) {
	static const uint16_t cometImage[8][77] = {
			{0xB820,0xB820,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB820,0xB820,
			0xB820,0xB8DC,0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8DB,0xB8DC,0xB820,
			0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8DB,
			0xB8DB,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8DB,
			0xB8DB,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8DB,
			0xB820,0xB8DF,0xB8DB,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8DB,0xB8DF,0xB820,
			0xB820,0xB820,0xB820,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB820,0xB820},

			{0xB820,0xB820,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB820,0xB820,
			0xB820,0xB8DC,0xB8DB,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8DB,0xB8DC,0xB820,
			0xB8DB,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8DB,
			0xB820,0xB8DF,0xB8DB,0xB8B1,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8DB,0xB8DF,0xB820,
			0xB820,0xB820,0xB820,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB820,0xB820},

			{0xB820,0xB820,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB820,0xB820,
			0xB820,0xB8DC,0xB8DB,0xB8B0,0xB8B1,0xB8B1,0xB8B1,0xB8B0,0xB8DB,0xB8DC,0xB820,
			0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8DB,
			0xB820,0xB8DF,0xB8DB,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8DB,0xB8DF,0xB820,
			0xB820,0xB820,0xB820,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB820,0xB820},


			{0xB820,0xB820,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB820,0xB820,
			0xB820,0xB8DC,0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8DB,0xB8DC,0xB820,
			0xB8DB,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B1,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8DB,
			0xB820,0xB8DF,0xB8DB,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8DB,0xB8DF,0xB820,
			0xB820,0xB820,0xB820,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB820,0xB820},

			 {0xB820,0xB820,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB820,0xB820,
			0xB820,0xB8DC,0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8DB,0xB8DC,0xB820,
			0xB8DB,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8DB,
			0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8DB,
			0xB820,0xB8DF,0xB8DB,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8DB,0xB8DF,0xB820,
			0xB820,0xB820,0xB820,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB820,0xB820},


			{0xB820,0xB820,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB820,0xB820,
			0xB820,0xB8DC,0xB8DB,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B1,0xB8DB,0xB8DC,0xB820,
			0xB8DB,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8DB,
			0xB820,0xB8DF,0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8DB,0xB8DF,0xB820,
			0xB820,0xB820,0xB820,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB820,0xB820},

			{0xB820,0xB820,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB820,0xB820,
			0xB820,0xB8DC,0xB8DB,0xB8B0,0xB8B1,0xB8B1,0xB8B1,0xB8B0,0xB8DB,0xB8DC,0xB820,
			0xB8DB,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8DB,
			0xB8DB,0xB8B0,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8DB,
			0xB8DB,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B0,0xB8B1,0xB8B1,0xB8DB,
			0xB820,0xB8DF,0xB8DB,0xB8B1,0xB8B1,0xB8B1,0xB8B0,0xB8B0,0xB8DB,0xB8DF,0xB820,
			0xB820,0xB820,0xB820,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB820,0xB820},


			{0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
			0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
			0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
			0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
			0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
			0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
			0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820}
	};
	static const vector_t cometSize = {11, 7};

	comet->image = cometImage;
	comet->size = cometSize;

	//vector_t position = {10000, 10000};
	//vector_t velocity = {500, 10};
	comet->position = position;
	comet->velocity = velocity;
	comet->frame = 0;
	comet->broken = 0;
}

void breakComet(comet_t *comet) {
	comet->broken = 1;

	// Initialize the debris
	vector_t debris1Position = {comet->position.x, comet->position.y};
	vector_t debris1Velocity = {300, -300};
	initDebris(&(comet->debris1), debris1Position, debris1Velocity, 0);

	vector_t debris2Position = {comet->position.x, comet->position.y + 3};
	vector_t debris2Velocity = {200, 100};
	initDebris(&(comet->debris2), debris2Position, debris2Velocity, 1);

	vector_t debris3Position = {comet->position.x + 6, comet->position.y + 3};
	vector_t debris3Velocity = {-300, -200};
	initDebris(&(comet->debris3), debris3Position, debris3Velocity, 2);
}

void updatePhysics(comet_t *comet, uint32_t deltaTime) {
	// Apply gravity
	//comet->velocity.y += deltaTime;

	// Apply velocity
	//comet->position.x += comet->velocity.x * deltaTime;
	//comet->position.y += comet->velocity.y * deltaTime;

	// Wrap x position
	//comet->position.x = (comet->position.x < 0) ? (comet->position.x & 262143) - 1 : (comet->position.x & 262143);
}

void checkCollisions(comet_t *comet, uint32_t deltaTime) {
	vector_t collisionPosition = {(comet->position.x >> 10), (comet->position.y >> 10)};
	collisionPosition.y = comet->size.y - 1;
	collisionPosition.x = (comet->size.x >> 1);
	if ((comet->broken == 0) && (collisionPosition.y > getTerrainHeight(collisionPosition))) {
		breakComet(comet);
	}
}

void updateComet(comet_t *comet, uint32_t deltaTime) {
	updatePhysics(comet, deltaTime);

	// Apply animation
	if (comet->broken == 0) {
		//comet->frame += (deltaTime << 1);
		if ((comet->frame >> 4) > 6) {
			comet->frame = 0;
		}
	}

	checkCollisions(comet, deltaTime);

	vector_t displayPosition = {(comet->position.x >> 10), (comet->position.y >> 10)};

	// Draw the debris
	if (comet->broken == 1) {

		// Erase the comet
		if ((comet->frame >> 4) < 7) {
			comet->frame = (7 << 4);
			addToBuffer(comet->image[7], displayPosition, comet->size);
			//drawASCIIImageWithBackground(cometImage[7], displayPosition, cometImageSize);
		}

		updateDebris(&(comet->debris1), deltaTime);
		updateDebris(&(comet->debris2), deltaTime);
		updateDebris(&(comet->debris3), deltaTime);
	} else {
		// Draw the comet
		// TODO: Send to buffer
		addToBuffer(comet->image[(comet->frame) >> 4], displayPosition, comet->size);
		//drawASCIIImageWithBackground(cometImage[(comet->frame) >> 4], displayPosition, cometImageSize);
	}


}
