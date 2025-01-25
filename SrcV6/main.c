// Libraries
#include "comet1.h"
#include "comet2.h"
#include "gameController.h"
#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "runtime.h"
#include "music.h"
#include "fuel_gauge.h"
#include "led.h"
#include "background.h"
#include "renderer.h"
#include "lander.h"
#include "mainMenu.h"
#include "helpMenu.h"

#define MAIN_MENU 0
#define HELP_MENU 1
#define IN_GAME 2

int main() {
	// Screen setup
	uart_init(2000000);
	printf("%c[1;1H%c[2J%c[37;40m", ESC, ESC, ESC);

	// Initialize music
	music_t music;
	initMusic(&music);

	initLEDs();

	// Initialize game controller
	uint8_t gameControllerState = 0;
	initGameController();

	uint8_t gameState = MAIN_MENU;

	// Initialize runtime
	uint32_t runtime = 0;
	initRuntime(&runtime);
	uint32_t currentRuntime = runtime;
	uint32_t deltaTime = 0;

	// Initialize menus
	mainMenu_t mainMenu;
	initMainMenu(&mainMenu);
	helpMenu_t helpMenu;
	//initHelpMenu(&helpMenu);

	// Initialize game values
	uint32_t seed;
	uint16_t level;
	lander_t lander;
	comet1_t comet1a;
	comet2_t comet2a;

	while (1) {
		// Calculate time passed between frames
		deltaTime = runtime - currentRuntime;
		currentRuntime = runtime;

		// Read game controller input
		gameControllerState = readGameController();

		// Update melody
		//updateMusic(&music, deltaTime, level);

		// MAIN_MENU
		if (gameState == MAIN_MENU) {
			updateMainMenu(&mainMenu, gameControllerState);

			// Help menu open event
			if (mainMenu.helpMenu == 1) {
				initHelpMenu(&helpMenu);

				gameState = HELP_MENU;
				continue;
			}

			// Game start event
			if (mainMenu.gameStarted == 1) {
				// Clear the screen
				printf("%c[1;1H%c[2J%c[37;40m", ESC, ESC, ESC);

				// Initialize game start
				seed = runtime;
				level = 0;
				resetRuntime(&runtime);

				initBackground(&level, &seed);

				initLander(&lander, 1000, 3, 0, 0);

				// TODO: Comets based on level
				vector_t comet1aPosition = {180000, 35000};
				vector_t comet1aVelocity = {-200, 5};
				initComet1(&comet1a, comet1aPosition, comet1aVelocity);

				vector_t comet2aPosition = {220000, 50000};
				vector_t comet2aVelocity = {-200, 5};
				initComet2(&comet2a, comet2aPosition, comet2aVelocity);

				gameState = IN_GAME;
				continue;
			}
		}

		// HELP_MENU
		else if (gameState == HELP_MENU) {
			updateHelpMenu(&helpMenu, gameControllerState);

			// Help menu open event
			if (helpMenu.mainMenu == 1) {
				initMainMenu(&mainMenu);

				gameState = MAIN_MENU;
				continue;
			}
		}

		// IN_GAME
		else if (gameState == IN_GAME) {
			//printf("Game tick!");
			// Update objects
			updateComet1(&comet1a, deltaTime);
			//updateComet2(&comet2a, deltaTime);

			updateLander(&lander, deltaTime, gameControllerState);

			// Check collisions
			vector_t landerDisplayPosition = convertToDisplayPosition(lander.position);
			vector_t comet1aDisplayPosition = convertToDisplayPosition(comet1a.position);
			vector_t comet2aDisplayPosition = convertToDisplayPosition(comet2a.position);

			if ((comet1a.broken == 0) && (landerDisplayPosition.x + lander.imageSize.x > comet1aDisplayPosition.x) && (landerDisplayPosition.x < comet1aDisplayPosition.x + comet1a.imageSize.x) && (landerDisplayPosition.y + lander.imageSize.y > comet1aDisplayPosition.y) && (landerDisplayPosition.y < comet1aDisplayPosition.y + comet1a.imageSize.y)) {
				explodeLander(&lander);
			}

			if ((comet2a.broken == 0) && (landerDisplayPosition.x + lander.imageSize.x > comet2aDisplayPosition.x) && (landerDisplayPosition.x < comet2aDisplayPosition.x + comet2a.imageSize.x) && (landerDisplayPosition.y + lander.imageSize.y > comet2aDisplayPosition.y) && (landerDisplayPosition.y < comet2aDisplayPosition.y + comet2a.imageSize.y)) {
				explodeLander(&lander);
			}

			for (uint8_t i = 0; i < lander.bulletCount; i++) {
				// If bullet is currently active (playing the 2 frame animation)
				if ((lander.bullets[i].frame >> 7) < 1) {
					// Check collisions with comets
					vector_t bulletDisplayPosition = convertToDisplayPosition(lander.bullets[i].position);

					if ((comet1a.broken == 0) && (bulletDisplayPosition.x + lander.bullets[i].imageSize.x > comet1aDisplayPosition.x) && (bulletDisplayPosition.x < comet1aDisplayPosition.x + comet1a.imageSize.x) && (bulletDisplayPosition.y + lander.bullets[i].imageSize.y > comet1aDisplayPosition.y) && (bulletDisplayPosition.y < comet1aDisplayPosition.y + comet1a.imageSize.y)) {
						lander.bullets[i].frame = 1 << 7;
						breakComet1(&comet1a);
					}

					if ((comet2a.broken == 0) && (bulletDisplayPosition.x + lander.bullets[i].imageSize.x > comet2aDisplayPosition.x) && (bulletDisplayPosition.x < comet2aDisplayPosition.x + comet2a.imageSize.x) && (bulletDisplayPosition.y + lander.bullets[i].imageSize.y > comet2aDisplayPosition.y) && (bulletDisplayPosition.y < comet2aDisplayPosition.y + comet2a.imageSize.y)) {
						lander.bullets[i].frame = 1 << 7;
						breakComet2(&comet2a);
					}
				}
			}

			//TODO: Error happens in here
			// Render all objects in the buffer to the screen
			pushBuffer();

			// Game events
			if (lander.landed == 1) {
				// Redraw background
				level++;
				seed = runtime;
				lander.dockingSize = (lander.dockingSize < 2) ? lander.dockingSize + 1 : lander.dockingSize;
				initMusic(&music);
				initBackground(NULL, NULL);
				lander.fuel = (lander.fuel < 100) ? 100 : lander.fuel;
				initLander(&lander, lander.fuel, lander.lives, lander.score, lander.dockingSize);
				vector_t comet1aPosition = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 262144, runtime) + 131072, generateWhiteNoise((vector_t){runtime, deltaTime}, 65536, runtime * 13)};
				vector_t comet1aVelocity = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 512, (runtime + 1) * 13) - 256, generateWhiteNoise((vector_t){runtime, deltaTime}, 64, (runtime + 2) * 13) - 32};
				initComet1(&comet1a, comet1aPosition, comet1aVelocity);

				vector_t comet2aPosition = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 262144, (runtime + 3) * 13) + 131072, generateWhiteNoise((vector_t){runtime, deltaTime}, 65536, (runtime + 4) * 13)};
				vector_t comet2aVelocity = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 512, (runtime + 5) * 13) - 256, generateWhiteNoise((vector_t){runtime, deltaTime}, 64, (runtime + 6) * 13) - 32};
				initComet2(&comet2a, comet2aPosition, comet2aVelocity);
				currentRuntime = runtime;
			}

			// Update information
			updateFuelGauge(&runtime, lander.fuel);
		}
	}

	return 0;
}
