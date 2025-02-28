#include "debris.h"
#include "background.h"

void initDebris(debris_t *debris, vector_t position, vector_t velocity, uint8_t type) {
	debris->position = position;
	debris->velocity = velocity;
	debris->frame = 0;
	debris->type = type;
}

uint8_t updateDebris(debris_t *debris, uint32_t deltaTime) {
	static const uint16_t debrisImage[3][5][24] = {
			{
				{0x0020,0x0020,0x0020,0xB8DC,0xB8DC,0xB8DC,
				0x0020,0xB8DC,0xB8DB,0x38B0,0x38B2,0x38B2,
				0xB8DB,0x38B2,0x38B2,0x38B0,0x38B0,0x38B2,
				0x0020,0x0020,0x0020,0x0020,0x38B0,0x38B0},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0xB8DB,0x38B2,0x38B2,0x38B2,0x38B0,0x38B2,
				0x0020,0xB8DF,0xB8DB,0x38B0,0x38B0,0x0020,
				0x0020,0x0020,0x0020,0xB8DF,0xB8DF,0x0020},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x38B0,0x38B0,0x38B2,0xB8DB,0x0020,
				0x0020,0x38B2,0xB8DB,0xB8DF,0x0020,0x0020,
				0x0020,0xB8DF,0x0020,0x0020,0x0020,0x0020},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0xB8DC,0xB8DC,0x0020,0x0020,
				0x0020,0x0020,0x38B2,0x38B2,0xB8DB,0x0020},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020}
			},

			{
				{0xB8DB,0x38B2,0x38B2,0x38B2,0x38B2,0x0020,
				0xB8DB,0x38B2,0x38B2,0x38B0,0x38B0,0x38B2,
				0x0020,0xB8DF,0xB8DB,0x38B2,0x38B0,0x38B0,
				0x0020,0x0020,0x0020,0xB8DB,0xB8DB,0xB8DB},

				{0x0020,0x38B2,0x38B2,0x38B2,0x38B0,0xB8DB,
				0x0020,0x38B2,0x38B2,0x38B0,0x38B0,0xB8DB,
				0x0020,0x38B2,0x38B2,0xB8DB,0xB8DF,0x0020,
				0xB8DF,0xB8DF,0x0020,0x0020,0x0020,0x0020},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0xB8DC,0xB8DC,0xB8DC,0x0020,0x0020,
				0x0020,0x38B2,0x38B0,0x38B0,0xB8DB,0x0020,
				0x0020,0x38B2,0x38B2,0x38B0,0xB8DB,0x0020},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0xB8DC,0xB8DC,0x0020,0x0020,
				0x0020,0x0020,0x38B2,0x38B2,0xB8DB,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020}
			},

			{
				{0x0020,0x38B2,0x38B2,0x38B0,0x38B2,0xB8DB,
				0x0020,0x38B2,0x38B2,0x38B2,0x38B2,0xB8DB,
				0x38B2,0x38B2,0x38B2,0xB8DB,0xB8DF,0x0020,
				0xB8DF,0xB8DF,0xB8DF,0x0020,0x0020,0x0020},

				{0x0020,0xB8DC,0xB8DC,0x0020,0x0020,0x0020,
				0x0020,0x38B0,0x38B2,0xB8DB,0xB8DC,0x0020,
				0x0020,0x38B2,0x38B2,0x38B2,0x38B2,0xB8DB,
				0x0020,0x0020,0x0020,0x38B2,0x38B2,0xB8DB},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0xB8DC,0xB8DC,0x0020,
				0x0020,0x0020,0xB8DB,0x38B2,0x38B2,0x0020,
				0x0020,0xB8DB,0x38B0,0x38B2,0x0020,0x0020},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0xB8DB,0x38B2,0x38B2,0x0020,0x0020,
				0x0020,0x0020,0xB8DF,0xB8DF,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020},

				{0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
				0x0020,0x0020,0x0020,0x0020,0x0020,0x0020}
			}

			};

	static const vector_t debrisImageSize = {6, 4};

	// Apply gravity
	debris->velocity.y += (deltaTime << 2);

	// Apply velocity
	debris->position.x += debris->velocity.x * deltaTime;
	debris->position.y += debris->velocity.y * deltaTime;

	debris->frame += deltaTime;

	if (((debris->frame) >> 4) > 4) {
		return 0;
	}

	vector_t displayPosition = {(debris->position.x >> 10), (debris->position.y >> 10)};
	drawASCIIImageWithBackground(debrisImage[debris->type][(debris->frame) >> 4], displayPosition, debrisImageSize);
	return 1;
}
