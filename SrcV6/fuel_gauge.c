#include "fuel_gauge.h"
#include "led.h"

void updateFuelGauge(uint32_t *runtime, int16_t fuel) {
	static uint8_t color = NONE;

	// High fuel = green
	if (fuel > 750) {
		if (color != GREEN) {
			resetLEDs();
			setGreen();
			color = GREEN;
		}
	}

	// Mid fuel = yellow
	else if (fuel > 250) {
		if (color != YELLOW) {
			resetLEDs();
			setRed();
			setGreen();
			color = YELLOW;
		}
	}

	// Low fuel = red
	else if (fuel > 0) {
		if (color != RED) {
			resetLEDs();
			setRed();
			color = RED;
		}
	}

	// Very low fuel = blinking red
	else if (((*runtime >> 6) & 1)) {
		if (color != NONE) {
			resetLEDs();
			color = NONE;
		}
	} else if (color != RED) {
		resetLEDs();
		setRed();
		color = RED;
	}
}
