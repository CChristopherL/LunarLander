#include "30010_io.h"
#include "stm32f30x_conf.h"

#include "runtime.h"
#include "music.h"
#include "fuel_gauge.h"
#include "led.h"
#include "background.h"
#include "renderer.h"
#include "joystick.h"

#include "lander.h"

int main(void)
{
	// Initialize serial monitor
	uart_init(2000000);

	// Clear screen
	printf("%c[1;1H", ESC);
	printf("%c[2J", ESC);
	printf("%c[37;40m", ESC);

	// Initialize runtime
	uint32_t runtime = 0;
	initRuntime(&runtime);

	// Initialize music
	//initMusic();

	// Initialize LEDs
	initLEDs();

	// Initialize Joystick
	uint8_t joystickState = 0;
	initializeJoystick();

	// Initialize Lander
	lander_t lander;
	initLander(&lander);

	// Initialize menu
	uint8_t gameState = 1;

	// Initialize Background (TODO: CHANGE THIS TO WHEN MAIN MENU PLAY BUTTON IS PRESSED)
	uint32_t seed = 133;
	initBackground(&seed);

	// Set runtime to zero upon game loop start
	resetRuntime(&runtime);
	uint32_t currentRuntime = runtime;
	uint32_t previousRuntime = runtime;

	while (1) {
		// Set the time for this frame
		currentRuntime = runtime;

		// Read the player input
		joystickState = readJoystick();

		// If the game is in the menu
		if (gameState == 0) {

		} else if (gameState == 1) { // If the game is being played

			// Update the lander
			updateLander(&lander, (currentRuntime - previousRuntime), joystickState, seed);

			// Update the fuel gauge led indicator
			updateFuelGauge(&runtime, lander.fuel);

			// Save previous calculation time
			previousRuntime = currentRuntime;
		}

		//updateMusic(&runtime);
	}

	return 0;
}
