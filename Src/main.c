#include "30010_io.h"
#include "stm32f30x_conf.h"

#include "runtime.h"
#include "music.h"
#include "fuel_gauge.h"
#include "led.h"
#include "background.h"

int main(void)
{
	// Initialize serial monitor
	uart_init(921600);

	// Clear screen
	printf("%c[2J", ESC);
	printf("%c[39;49m", ESC);

	// Initialize runtime
	uint32_t runtime = 0;
	initRuntime(&runtime);

	// Initialize music
	initMusic();

	// Initialize LEDs
	initLEDs();

	// Initialize display
	initDisplay(&runtime);

	// Draw entire background
	refreshBackground();

	// Set runtime to zero upon game loop start
	resetRuntime(&runtime);

	while (1) {
		updateMusic(&runtime);
		updateFuelGauge(&runtime, 50);
	}

	return 0;
}
