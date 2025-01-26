// Libraries
#include "comet.h"
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
#include "lcd.h"
#include "powerup.h"
#include "gameOver.h"

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

	// Initialize LED fuel indicator
	initLEDs();

	// Initialize LCD information display
	lcd_init();
	uint8_t buffer[512];
	memset(buffer, 0, 512);
	lcd_push_buffer(buffer);

	// Initialize game controller
	uint8_t gameControllerState = 0;
	initGameController();
	uint8_t previousGameControllerState = 0;

	uint8_t gameState = MAIN_MENU;

	// Initialize runtime
	uint32_t runtime = 0;
	initRuntime(&runtime);
	uint32_t currentRuntime = runtime;
	uint32_t deltaTime = 0;

	uint8_t bossKeyOpen = 0;

	// Initialize game values
	uint32_t seed = 0;
	uint16_t level = 1;
	getSeed(&seed);
	getLevel(&level);
	lander_t lander;
	comet_t comet1;
	comet_t comet2;
	powerup_t powerup;
	gameOver_t gameOver;

	// Initialize menus
	mainMenu_t mainMenu;
	initMainMenu(&mainMenu, 0);
	helpMenu_t helpMenu;

	while (1) {
		// Read game controller input
		gameControllerState = readGameController();

		// Boss key check
		if ((gameControllerState & BUTTON_3) != (previousGameControllerState & BUTTON_3)) {
			if (gameControllerState & BUTTON_3) {
				previousGameControllerState |= BUTTON_3;
				setFreq(0);
				bossKeyOpen = 1;
				printf("%c[1;1H%c[2J%c[37;40m", ESC, ESC, ESC);

			} else {
				previousGameControllerState &= ~(BUTTON_3);
				bossKeyOpen = 0;

				if (gameState == MAIN_MENU) {
					initMainMenu(&mainMenu, mainMenu.highScore);
				} else if (gameState == HELP_MENU) {
					initHelpMenu(&helpMenu);
				} else if (gameState == IN_GAME) {
					initBackground(&level, &seed);
					currentRuntime = runtime;
				}
			}
		}
		if (bossKeyOpen == 1) {
			currentRuntime = runtime;
			printf("BOSS KEY  ");
			continue;
		}


		// Calculate time passed between frames
		deltaTime = runtime - currentRuntime;
		currentRuntime = runtime;

		// Update melody
		updateMusic(&music, deltaTime, level);

		// MAIN_MENU
		if (gameState == MAIN_MENU) {
			seed = runtime;
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
				setFreq(0);
				initBackground(&level, &seed);
				resetRuntime(&runtime);
				currentRuntime = 0;

				initLander(&lander, 1000, 3, 0, 0);

				vector_t comet1Position = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 262144, runtime) + 131072, generateWhiteNoise((vector_t){runtime, deltaTime}, 65536, (runtime + 1) * 13)};
				vector_t comet1Velocity = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 512, (runtime + 1) * 13) - 256, generateWhiteNoise((vector_t){runtime, deltaTime}, 64, (runtime + 2) * 13) - 32};
				initComet(&comet1, comet1Position, comet1Velocity);

				vector_t comet2Position = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 262144, (runtime + 3) * 13) + 131072, generateWhiteNoise((vector_t){runtime, deltaTime}, 65536, (runtime + 4) * 13)};
				vector_t comet2Velocity = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 512, (runtime + 5) * 13) - 256, generateWhiteNoise((vector_t){runtime, deltaTime}, 64, (runtime + 6) * 13) - 32};
				initComet(&comet2, comet2Position, comet2Velocity);

				vector_t powerupPosition = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 262144, (runtime + 7) * 13) + 131072, generateWhiteNoise((vector_t){runtime, deltaTime}, 65536, (runtime + 8) * 13)};
				uint8_t powerupType = generateWhiteNoise((vector_t){runtime, deltaTime}, 4, (runtime + 9) * 13);
				initPowerup(&powerup, powerupPosition, powerupType);

				gameState = IN_GAME;
				continue;
			}
		}

		// HELP_MENU
		else if (gameState == HELP_MENU) {
			updateHelpMenu(&helpMenu, gameControllerState);

			// Help menu open event
			if (helpMenu.mainMenu == 1) {
				initMainMenu(&mainMenu, mainMenu.highScore);

				gameState = MAIN_MENU;
				continue;
			}
		}

		// IN_GAME
		else if (gameState == IN_GAME) {
			// If game is over, update the game over text
			if (lander.lives < 0) {
				if (lander.lives == -1) {
					initGameOver(&gameOver);
					lander.lives -= 1;
				}
				updateGameOver(&gameOver, deltaTime);

				if (gameOver.mainMenu == 1) {
					initMainMenu(&mainMenu, (lander.score > mainMenu.highScore) ? lander.score : mainMenu.highScore);
					gameState = MAIN_MENU;
				}
				continue;
			}

			// Update objects
			updateComet(&comet1, deltaTime);
			updateComet(&comet2, deltaTime);
			updatePowerup(&powerup);

			updateLander(&lander, deltaTime, gameControllerState);

			// Check collisions
			vector_t landerDisplayPosition = convertToDisplayPosition(lander.position);
			vector_t comet1DisplayPosition = convertToDisplayPosition(comet1.position);
			vector_t comet2DisplayPosition = convertToDisplayPosition(comet2.position);
			vector_t powerupDisplayPosition = convertToDisplayPosition(powerup.position);

			if (((comet1.broken == 0) && (landerDisplayPosition.x + lander.imageSize.x > comet1DisplayPosition.x) && (landerDisplayPosition.x < comet1DisplayPosition.x + comet1.imageSize.x) && (landerDisplayPosition.y + lander.imageSize.y > comet1DisplayPosition.y) && (landerDisplayPosition.y < comet1DisplayPosition.y + comet1.imageSize.y)) || ((comet2.broken == 0) && (landerDisplayPosition.x + lander.imageSize.x > comet2DisplayPosition.x) && (landerDisplayPosition.x < comet2DisplayPosition.x + comet2.imageSize.x) && (landerDisplayPosition.y + lander.imageSize.y > comet2DisplayPosition.y) && (landerDisplayPosition.y < comet2DisplayPosition.y + comet2.imageSize.y))) {
				explodeLander(&lander);
			}

			if ((powerup.type != 4) && (landerDisplayPosition.x + lander.imageSize.x > powerupDisplayPosition.x) && (landerDisplayPosition.x < powerupDisplayPosition.x + powerup.imageSize.x) && (landerDisplayPosition.y + lander.imageSize.y > powerupDisplayPosition.y) && (landerDisplayPosition.y < powerupDisplayPosition.y + powerup.imageSize.y)) {
				if (powerup.type == 0) {
					lander.bulletType = 1;
				} else if (powerup.type == 1) {
					lander.bulletType = 2;
				} else if (powerup.type == 2) {
					lander.lives += 1;
					if (lander.lives > 3) {
						lander.lives = 3;
					}
				} else if (powerup.type == 3) {
					lander.fuel += 500;
					if (lander.fuel > 1000 + (lander.dockingSize * 250)) {
						lander.fuel = 1000 + (lander.dockingSize * 250);
					}
				}
				powerup.type = 4;
			}

			for (uint8_t i = 0; i < lander.bulletCount; i++) {
				// If bullet is currently active (playing the 2 frame animation)
				if ((lander.bullets[i].frame >> 7) < 1) {
					// Check collisions with comets
					vector_t bulletDisplayPosition = convertToDisplayPosition(lander.bullets[i].position);

					if ((comet1.broken == 0) && (bulletDisplayPosition.x + lander.bullets[i].imageSize.x > comet1DisplayPosition.x) && (bulletDisplayPosition.x < comet1DisplayPosition.x + comet1.imageSize.x) && (bulletDisplayPosition.y + lander.bullets[i].imageSize.y > comet1DisplayPosition.y) && (bulletDisplayPosition.y < comet1DisplayPosition.y + comet1.imageSize.y)) {
						lander.bullets[i].frame = 1 << 7;
						lander.score += 1000;
						breakComet(&comet1);
					}

					if ((comet2.broken == 0) && (bulletDisplayPosition.x + lander.bullets[i].imageSize.x > comet2DisplayPosition.x) && (bulletDisplayPosition.x < comet2DisplayPosition.x + comet2.imageSize.x) && (bulletDisplayPosition.y + lander.bullets[i].imageSize.y > comet2DisplayPosition.y) && (bulletDisplayPosition.y < comet2DisplayPosition.y + comet2.imageSize.y)) {
						lander.bullets[i].frame = 1 << 7;
						lander.score += 1000;
						breakComet(&comet2);
					}
				}
			}

			// Render all objects in the buffer to the screen
			pushBuffer();

			// Game events
			if (lander.landed == 1) {
				lander.score += 5000;
				setFreq(0);
				// Redraw background
				level++;
				seed = runtime;
				lander.dockingSize = (lander.dockingSize < 2) ? lander.dockingSize + 1 : lander.dockingSize;
				initMusic(&music);
				initBackground(NULL, NULL);
				lander.fuel = (lander.fuel < 200) ? 200 : lander.fuel;
				initLander(&lander, lander.fuel, lander.lives, lander.score, lander.dockingSize);
				vector_t comet1Position = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 262144, runtime) + 131072, generateWhiteNoise((vector_t){runtime, deltaTime}, 65536, runtime * 13)};
				vector_t comet1Velocity = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 512, (runtime + 1) * 13) - 256, generateWhiteNoise((vector_t){runtime, deltaTime}, 64, (runtime + 2) * 13) - 32};
				initComet(&comet1, comet1Position, comet1Velocity);

				vector_t comet2Position = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 262144, (runtime + 3) * 13) + 131072, generateWhiteNoise((vector_t){runtime, deltaTime}, 65536, (runtime + 4) * 13)};
				vector_t comet2Velocity = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 512, (runtime + 5) * 13) - 256, generateWhiteNoise((vector_t){runtime, deltaTime}, 64, (runtime + 6) * 13) - 32};
				initComet(&comet2, comet2Position, comet2Velocity);

				vector_t powerupPosition = (vector_t){generateWhiteNoise((vector_t){runtime, deltaTime}, 262144, (runtime + 7) * 13) + 131072, generateWhiteNoise((vector_t){runtime, deltaTime}, 65536, (runtime + 8) * 13)};
				uint8_t powerupType = generateWhiteNoise((vector_t){runtime, deltaTime}, 4, (runtime + 9) * 13);
				initPowerup(&powerup, powerupPosition, powerupType);

				currentRuntime = runtime;
			}

			// Update information
			updateFuelGauge(&runtime, lander.fuel);

			memset(buffer, 0, 512);
			displayHealth(&buffer, lander.lives);
			displayGameTime(&buffer, runtime);
			displayScore(&buffer, lander.score);
			displayFuel(&buffer, lander.fuel);
			lcd_push_buffer(buffer);
			uint8_t minutes = (runtime / 4500);
			uint8_t seconds = (runtime / 75) % 60;
			uint16_t centiseconds = runtime % 75;
			printf("Lives: %01d\nFuel: %04d\nTime: %02d:%02d.%02d\nScore: %d\n", lander.lives, lander.fuel, minutes, seconds, centiseconds, lander.score);
		}
	}

	return 0;
}
