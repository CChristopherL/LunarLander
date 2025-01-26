#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "math.h"

typedef struct {
	uint16_t (*titleImage)[48];
	vector_t titleImageSize;
	vector_t titleImagePosition;

	uint16_t (*playButtonImage)[156];
	vector_t playButtonImageSize;
	vector_t playButtonImagePosition;

	uint16_t (*helpButtonImage)[156];
	vector_t helpButtonImageSize;
	vector_t helpButtonImagePosition;

	uint8_t selectedButton;
	uint8_t previousGameControllerState;
	uint8_t helpMenu;
	uint8_t gameStarted;
	uint32_t highScore;
} mainMenu_t;

void initMainMenu(mainMenu_t *mainMenu, uint32_t highScore);
void updateMainMenu(mainMenu_t *mainMenu, uint8_t gameControllerState);

#endif /* MAINMENU_H_ */
