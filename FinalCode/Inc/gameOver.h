#ifndef GAMEOVER_H_
#define GAMEOVER_H_

#include <stdint.h>
#include "math.h"

typedef struct {
	uint16_t (*image)[168];
	vector_t imageSize;
	vector_t imagePosition;
	uint8_t mainMenu;
	uint16_t displayTime;
} gameOver_t;

void initGameOver(gameOver_t *gameOver);
void updateGameOver(gameOver_t *gameOver, uint32_t deltaTime);

#endif /* GAMEOVER_H_ */
