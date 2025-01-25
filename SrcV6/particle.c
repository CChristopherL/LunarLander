#include "particle.h"

// Initializes the particle
void initParticle(particle_t *particle, vector_t position, vector_t velocity) {
	static uint16_t particleImage[5][1] = {
		{0x19DB},
		{0x19B1},
		{0x19B0},
		{0x1920},
		{0x0020}
	};
	static vector_t particleSize = {1, 1};

	particle->image = particleImage;
	particle->size = particleSize;

	particle->position = position;
	particle->velocity = velocity;
	particle->frame = (4 << 4);
}

// Updates the particle's physics
static void updatePhysics(particle_t *particle, uint32_t deltaTime) {
	// Apply gravity
	//particle->velocity.y += (deltaTime << 2);

	// Apply velocity
	particle->position.x += particle->velocity.x * deltaTime;
	particle->position.y += particle->velocity.y * deltaTime;
}

// Called continuously updating particle
void updateParticle(particle_t *particle, uint32_t deltaTime) {
	// If the particle is visible (last frame is the blank invisible frame)
	if ((particle->frame >> 4) > 3) {
		return;
	}

	// Update physics
	updatePhysics(particle, deltaTime);

	// Update frame
	particle->frame += (deltaTime << 2);

	// Add the particle to the buffer to be drawn
	vector_t displayPosition = convertToDisplayPosition(particle->position);
	addToBuffer(particle->image[(particle->frame) >> 4], displayPosition, particle->size);
}
