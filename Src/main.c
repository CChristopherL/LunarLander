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
	initBackground((uint32_t*)635);

	uint16_t platform[] = {
	0x58DC,0x58DC,0x58DC,0x58DC,0x58DC,0x58DC,0x58DC,0x58DC,
	0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF,0x38DF
	};

	vector_t pos1 = {50, 85};
	vector_t size1 = {8, 2};

	uint16_t gasCan[] = {0xF8DC,0x3820,0x3820,0x3820,0x3820,0x50DB,0x50DB,0x078BB};

	vector_t pos2 = {165, 15};
	vector_t size2 = {4, 2};

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
			updateLander(&lander, (currentRuntime - previousRuntime), joystickState);

			drawASCIIImage(platform, pos1, size1);

			drawASCIIImage(gasCan, pos2, size2);

			// Update the fuel gauge led indicator
			updateFuelGauge(&runtime, lander.fuel);

			// Save previous calculation time
			previousRuntime = currentRuntime;
		}

		//updateMusic(&runtime);
	}

	return 0;
}
