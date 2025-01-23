// Libraries
#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "runtime.h"
#include "music.h"
#include "fuel_gauge.h"
#include "led.h"
#include "background.h"
#include "renderer.h"
#include "joystick.h"
#include "comet.h"
#include "lander.h"

int main() {
	// Screen setup
	uart_init(2000000);
	printf("%c[1;1H", ESC);
	printf("%c[2J", ESC);
	printf("%c[37;40m", ESC);


	// Initialization
	initMusic();
	initLEDs();

	uint8_t joystickState = 0;
	initializeJoystick();

	uint8_t gameState = 1;

	// Initialize Background (TODO: CHANGE THIS TO WHEN MAIN MENU PLAY BUTTON IS PRESSED)
	uint32_t seed = 535; //1232 no landing pad
	uint16_t level = 1;
	initBackground(&level, &seed);

	lander_t lander;
	initLander(&lander, 1000, 3, 0);

	comet_t comet;
	vector_t cometPosition = {200000, 25000};
	vector_t cometVelocity = {-200, 25};
	initComet(&comet, cometPosition, cometVelocity);

	uint32_t runtime = 0;
	initRuntime(&runtime);
	//resetRuntime(&runtime);
	uint32_t currentRuntime = runtime;
	uint32_t deltaTime = 0;


	while (1) {
		// Calculate time passed between frames
		deltaTime = runtime - currentRuntime;
		currentRuntime = runtime;


		// Input
		joystickState = readJoystick();


		// Output
		//updateMusic(&runtime);


		// Menu
		if (gameState == 0) {

		}


		// In-game
		else if (gameState == 1) {
			// Update objects
			//TODO: For loop with level to update all comets
			updateComet(&comet, deltaTime);

			updateLander(&lander, deltaTime, joystickState);

			// Check collisions
			vector_t landerDisplayPosition = convertToDisplayPosition(lander.position);
			vector_t cometDisplayPosition = convertToDisplayPosition(comet.position);
			if ((landerDisplayPosition.x + lander.size.x > cometDisplayPosition.x) && (landerDisplayPosition.x < cometDisplayPosition.x + comet.size.x) && (landerDisplayPosition.y + lander.size.y > cometDisplayPosition.y) && (landerDisplayPosition.y < cometDisplayPosition.y + comet.size.y)) {
				explodeLander(&lander);
				breakComet(&comet);
			}

			// Render all objects in the buffer to the screen
			pushBuffer();

			// Game events
			if (lander.landed == 1) {
				// Redraw background
				level++;
				seed++;
				initBackground(NULL, NULL);
				initLander(&lander, lander.fuel, lander.lives, lander.score);
				currentRuntime = runtime;

				// Reset/add comets

			}


			// Update information
			updateFuelGauge(&runtime, lander.fuel);
		}
	}

	return 0;
}
