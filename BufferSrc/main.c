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
	uint32_t seed = 533; //1232 no landing pad
	uint16_t level = 2;
	initBackground(&level, &seed);

	lander_t lander;
	initLander(&lander, 1000, 3, 0);

	comet_t comet;
	vector_t cometPosition = {200000, 25000};
	vector_t cometVelocity = {0, 0};
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


			//printf("X: %d \t Y: %d", lander.position.x, lander.position.y);
			//TODO: For loop with level to update all comets
			updateComet(&comet, deltaTime);

			updateLander(&lander, deltaTime, joystickState);


			// If the lander has landed, move to the next level
			/*if (lander.landed == 1) {
				level++;
				seed = runtime;
				initBackground(&level, &seed);
				initLander(&lander, lander.fuel, lander.lives, lander.score);
				continue;
			}

			// Check collisions
			if ((lander.position.x + (4 << 10) > comet.position.x) && (lander.position.x < comet.position.x + (11 << 10)) && (lander.position.y + (4 << 10) > comet.position.y) && (lander.position.y < comet.position.y + (7 << 10))) {
				explodeLander(&lander);
				breakComet(&comet);
			}*/
			//printf("Runtime: %0000ld \t X Position: %0000ld \t Velocity Y: %0000ld", runtime, lander.position.x, lander.velocity.y);

			//TODO: Add everything to buffer
			//TODO: Render buffer
			pushBuffer();


			// Update information
			//updateFuelGauge(&runtime, lander.fuel);
		}
	}

	return 0;
}
