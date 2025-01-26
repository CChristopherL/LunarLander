#include "mainMenu.h"
#include "gameController.h"
#include "renderer.h"

void initMainMenu(mainMenu_t *mainMenu, uint32_t highScore) {
	// Define our ASCII image for the title (it is split up into 33 8x8 chunks)
	static uint16_t titleImage[14][48] = {
		{0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x38DC,0x38DC,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820},

		{0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,0x38DC,0x38DC,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820},

		{0x3820,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,0x38DC,
		0x3820,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,0x38DC,
		0x3820,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,0x38DC,
		0x3820,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,0x3820,
		0x38DC,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820},

		{0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,0x3820,0x38DC,
		0x38DC,0x3820,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,
		0x38DC,0x38DC,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,
		0x38DC,0x38DC,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,
		0x3820,0x38DC,0x38DC,0x38DC,0x3820,0x3820,0x38DC,0x38DC,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820},

		{0x38DC,0x38DC,0x38DC,0x38DC,0x3820,0x3820,0x3820,0x38DC,
		0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,0x38DC,
		0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x3820,0x3820,0x38DC,
		0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,0x38DC,
		0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,0x38DC,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820},

		{0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x3820,0x3820,0x3820,
		0x38DC,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x3820,0x3820,0x3820,
		0x38DC,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,0x3820,
		0x38DC,0x3820,0x3820,0x3820,0x38DC,0x38DC,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820},

		{0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,
		0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820},

		{0x30DC,0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x30DC,0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x30DC,0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x30DC,0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x3020,0x3020,
		0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020},

		{0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x3020,0x3020,
		0x30DC,0x30DC,0x3020,0x3020,0x3020,0x30DC,0x30DC,0x3020,
		0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x3020,
		0x30DC,0x30DC,0x3020,0x3020,0x3020,0x30DC,0x30DC,0x3020,
		0x30DC,0x30DC,0x3020,0x3020,0x3020,0x30DC,0x30DC,0x3020,
		0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020},

		{0x3020,0x30DC,0x30DC,0x30DC,0x3020,0x3020,0x30DC,0x30DC,
		0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x3020,0x30DC,0x30DC,
		0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,
		0x3020,0x30DC,0x30DC,0x3020,0x30DC,0x30DC,0x30DC,0x30DC,
		0x3020,0x30DC,0x30DC,0x3020,0x3020,0x30DC,0x30DC,0x30DC,
		0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020},

		{0x3020,0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x3020,
		0x3020,0x3020,0x30DC,0x30DC,0x3020,0x3020,0x30DC,0x30DC,
		0x3020,0x3020,0x30DC,0x30DC,0x3020,0x3020,0x3020,0x30DC,
		0x3020,0x3020,0x30DC,0x30DC,0x3020,0x3020,0x30DC,0x30DC,
		0x3020,0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,0x3020,
		0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020},

		{0x3020,0x3020,0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,
		0x3020,0x3020,0x3020,0x30DC,0x30DC,0x3020,0x3020,0x3020,
		0x30DC,0x3020,0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,
		0x3020,0x3020,0x3020,0x30DC,0x30DC,0x3020,0x3020,0x3020,
		0x3020,0x3020,0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,
		0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020},

		{0x30DC,0x3020,0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,
		0x3020,0x3020,0x3020,0x30DC,0x30DC,0x3020,0x3020,0x3020,
		0x3020,0x3020,0x3020,0x30DC,0x30DC,0x30DC,0x30DC,0x30DC,
		0x3020,0x3020,0x3020,0x30DC,0x30DC,0x3020,0x3020,0x30DC,
		0x30DC,0x3020,0x3020,0x30DC,0x30DC,0x3020,0x3020,0x3020,
		0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020},

		{0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x30DC,0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x30DC,0x30DC,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,
		0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020,0x3020}
	};

	static vector_t titleImageSize = {8, 6};
	static vector_t titleImagePosition = {100, 10};
	mainMenu->titleImage = titleImage;
	mainMenu->titleImageSize = titleImageSize;
	mainMenu->titleImagePosition = titleImagePosition;

	// Define our ASCII image for the play button
	static uint16_t playButtonImage[2][156] = {
		{0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,
		0x38DB,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x38DB,
		0x38DB,0x3820,0x08DC,0x08DF,0x08DF,0x08DF,0x3820,0x08DF,0x08DB,0x08DF,0x3820,0x08DC,0x08DF,0x08DF,0x08DC,0x3820,0x08DB,0x08DF,0x08DF,0x08DC,0x3820,0x08DF,0x08DB,0x08DF,0x3820,0x38DB,
		0x38DB,0x3820,0x3820,0x08DF,0x08DF,0x08DC,0x3820,0x3820,0x08DB,0x3820,0x3820,0x08DB,0x08DF,0x08DF,0x08DB,0x3820,0x08DB,0x08DF,0x08DF,0x08DC,0x3820,0x3820,0x08DB,0x3820,0x3820,0x38DB,
		0x38DB,0x3820,0x08DF,0x08DF,0x08DF,0x3820,0x3820,0x3820,0x08DF,0x3820,0x3820,0x08DF,0x3820,0x3820,0x08DF,0x3820,0x08DF,0xB820,0x3820,0x08DF,0x3820,0x3820,0x08DF,0x3820,0x3820,0x38DB,
		0x38DB,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DC,0x38DB},

		{0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,
		0xB8DB,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB8DB,
		0xB8DB,0xB820,0xB8DC,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB8DF,0xB8DB,0xB8DF,0xB820,0xB8DC,0xB8DF,0xB8DF,0xB8DC,0xB820,0xB8DB,0xB8DF,0xB8DF,0xB8DC,0xB820,0xB8DF,0xB8DB,0xB8DF,0xB820,0xB8DB,
		0xB8DB,0xB820,0xB820,0xB8DF,0xB8DF,0xB8DC,0xB820,0xB820,0xB8DB,0xB820,0xB820,0xB8DB,0xB8DF,0xB8DF,0xB8DB,0xB820,0xB8DB,0xB8DF,0xB8DF,0xB8DC,0xB820,0xB820,0xB8DB,0xB820,0xB820,0xB8DB,
		0xB8DB,0xB820,0xB8DF,0xB8DF,0xB8DF,0xB820,0xB820,0xB820,0xB8DF,0xB820,0xB820,0xB8DF,0xB820,0xB820,0xB8DF,0xB820,0xB8DF,0xB820,0xB820,0xB8DF,0xB820,0xB820,0xB8DF,0xB820,0xB820,0xB8DB,
		0xB8DB,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB8DB}
	};
	static vector_t playButtonImageSize = {26, 6};
	static vector_t playButtonImagePosition = {115, 44};
	mainMenu->playButtonImage = playButtonImage;
	mainMenu->playButtonImageSize = playButtonImageSize;
	mainMenu->playButtonImagePosition = playButtonImagePosition;

	// Define our ASCII image for the help button
	static uint16_t helpButtonImage[2][156] = {
		{0x38DB,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DB,
		0x38DB,0x3820,0x3820,0x3820,0x08DC,0x3820,0x3820,0x08DC,0x3820,0x08DC,0x08DC,0x08DC,0x08DC,0x3820,0x08DC,0x3820,0x3820,0x3820,0x08DC,0x08DC,0x08DC,0x3820,0x3820,0x3820,0x3820,0x38DB,
		0x38DB,0x3820,0x3820,0x3820,0x08DB,0x08DC,0x08DC,0x08DB,0x3820,0x08DB,0x08DC,0x08DC,0x3820,0x3820,0x08DB,0x3820,0x3820,0x3820,0x08DB,0x08DC,0x08DC,0x08DF,0x3820,0x3820,0x3820,0x38DB,
		0x38DB,0x3820,0x3820,0x3820,0x08DB,0x3820,0x3820,0x08DB,0x3820,0x08DB,0x08DC,0x08DC,0x08DC,0x3820,0x08DB,0x08DC,0x08DC,0x3820,0x08DB,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x38DB,
		0x38DB,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x3820,0x38DB,
		0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF},

		{0xB8DB,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DB,
		0xB8DB,0xB820,0xB820,0xB820,0xB8DC,0xB820,0xB820,0xB8DC,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB8DC,0xB820,0xB820,0xB820,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB820,0xB820,0xB820,0xB8DB,
		0xB8DB,0xB820,0xB820,0xB820,0xB8DB,0xB8DC,0xB8DC,0xB8DB,0xB820,0xB8DB,0xB8DC,0xB8DC,0xB820,0xB820,0xB8DB,0xB820,0xB820,0xB820,0xB8DB,0xB8DC,0xB8DC,0xB8DF,0xB820,0xB820,0xB820,0xB8DB,
		0xB8DB,0xB820,0xB820,0xB820,0xB8DB,0xB820,0xB820,0xB8DB,0xB820,0xB8DB,0xB8DC,0xB8DC,0xB8DC,0xB820,0xB8DB,0xB8DC,0xB8DC,0xB820,0xB8DB,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB8DB,
		0xB8DB,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB820,0xB8DB,
		0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF,0xB8DF}
	};
	static vector_t helpButtonImageSize = {26, 6};
	static vector_t helpButtonImagePosition = {115, 54};
	mainMenu->helpButtonImage = helpButtonImage;
	mainMenu->helpButtonImageSize = helpButtonImageSize;
	mainMenu->helpButtonImagePosition = helpButtonImagePosition;

	// Set our values
	mainMenu->selectedButton = 0;
	mainMenu->previousGameControllerState = 0xFF;
	mainMenu->helpMenu = 0;
	mainMenu->gameStarted = 0;
	mainMenu->highScore = highScore;

	// Clear the screen and display the main menu
	printf("%c[1;1H%c[2J%c[37;40m", ESC, ESC, ESC);

	initBackground(NULL, NULL);

	for (uint8_t r = 0; r < 2; r++) {
		for (uint8_t c = 0; c < 7; c++) {
			vector_t chunkPosition;
			chunkPosition.x = mainMenu->titleImagePosition.x + mainMenu->titleImageSize.x * c;
			chunkPosition.y = mainMenu->titleImagePosition.y + mainMenu->titleImageSize.y * r;
			drawASCIIImage(mainMenu->titleImage[r * 7 + c], chunkPosition, mainMenu->titleImageSize);
		}
	}

	printf("%c[24;118HHigh Score: %07ld", ESC, mainMenu->highScore);
}

void updateMainMenu(mainMenu_t *mainMenu, uint8_t gameControllerState) {
	// If button1 input changed
	if ((gameControllerState & BUTTON_1) != (mainMenu->previousGameControllerState & BUTTON_1)) {
		if (gameControllerState & BUTTON_1) {
			mainMenu->previousGameControllerState |= BUTTON_1;

			// Play button pressed
			if (mainMenu->selectedButton == 0) {
				mainMenu->gameStarted = 1;
				return;
			}

			// Help button pressed
			else if (mainMenu->selectedButton == 1) {
				mainMenu->helpMenu = 1;
				return;
			}
		} else {
			mainMenu->previousGameControllerState &= ~(BUTTON_1);
		}
	}

	// If button2 input changed
	if ((gameControllerState & BUTTON_2) != (mainMenu->previousGameControllerState & BUTTON_2)) {
		if (gameControllerState & BUTTON_2) {
			mainMenu->previousGameControllerState |= BUTTON_2;

			// Play button pressed
			if (mainMenu->selectedButton == 0) {
				mainMenu->gameStarted = 1;
				return;
			}

			// Help button pressed
			else if (mainMenu->selectedButton == 1) {
				mainMenu->helpMenu = 1;
				return;
			}
		} else {
			mainMenu->previousGameControllerState &= ~(BUTTON_2);
		}
	}

	// If joystick up input changed
	if ((gameControllerState & JOYSTICK_UP) != (mainMenu->previousGameControllerState & JOYSTICK_UP)) {
		if (gameControllerState & JOYSTICK_UP) {
			mainMenu->previousGameControllerState |= JOYSTICK_UP;
			mainMenu->selectedButton = (mainMenu->selectedButton == 0) ? 0 : mainMenu->selectedButton - 1;
		} else {
			mainMenu->previousGameControllerState &= ~(JOYSTICK_UP);
		}

		// Draw both menu button images with and without selection
		if (mainMenu->selectedButton == 0) {
			drawASCIIImage(mainMenu->playButtonImage[0], mainMenu->playButtonImagePosition, mainMenu->playButtonImageSize);
			drawASCIIImage(mainMenu->helpButtonImage[1], mainMenu->helpButtonImagePosition, mainMenu->helpButtonImageSize);
		} else if (mainMenu->selectedButton == 1) {
			drawASCIIImage(mainMenu->playButtonImage[1], mainMenu->playButtonImagePosition, mainMenu->playButtonImageSize);
			drawASCIIImage(mainMenu->helpButtonImage[0], mainMenu->helpButtonImagePosition, mainMenu->helpButtonImageSize);
		}
		return;
	}

	// If joystick down input changed
	if ((gameControllerState & JOYSTICK_DOWN) != (mainMenu->previousGameControllerState & JOYSTICK_DOWN)) {
		if (gameControllerState & JOYSTICK_DOWN) {
			mainMenu->previousGameControllerState |= JOYSTICK_DOWN;
			mainMenu->selectedButton = (mainMenu->selectedButton == 1) ? 1 : mainMenu->selectedButton + 1;
		} else {
			mainMenu->previousGameControllerState &= ~(JOYSTICK_DOWN);
		}

		// Draw both menu button images with and without selection
		if (mainMenu->selectedButton == 0) {
			drawASCIIImage(mainMenu->playButtonImage[0], mainMenu->playButtonImagePosition, mainMenu->playButtonImageSize);
			drawASCIIImage(mainMenu->helpButtonImage[1], mainMenu->helpButtonImagePosition, mainMenu->helpButtonImageSize);
		} else if (mainMenu->selectedButton == 1) {
			drawASCIIImage(mainMenu->playButtonImage[1], mainMenu->playButtonImagePosition, mainMenu->playButtonImageSize);
			drawASCIIImage(mainMenu->helpButtonImage[0], mainMenu->helpButtonImagePosition, mainMenu->helpButtonImageSize);
		}
		return;
	}

	// If button3 input changed (for bosskey)
	if ((gameControllerState & BUTTON_3) != (mainMenu->previousGameControllerState & BUTTON_3)) {
		if (gameControllerState & BUTTON_3) {
			mainMenu->previousGameControllerState |= BUTTON_3;

		} else {
			mainMenu->previousGameControllerState &= ~(BUTTON_1);
		}
	}
}
