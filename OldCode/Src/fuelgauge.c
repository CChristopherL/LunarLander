#include "fuelgauge.h"
#include "runtime.h"



void fuelgauge(uint16_t fuel) {



	RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
	RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

	//Green - high fuel
	if (fuel >= 75) {
		resetLED();
		setGreen();
	}

	//Yellow - mid fuel
	else if (fuel<75 && fuel>25) {
		resetLED();
		setGreen();
		setRed();
	}

	//red - low fuel
	else if (fuel <= 25 && fuel > 10) {
		resetLED();
		setRed();
	}



	//blinking red - very low fuel
	else if (fuel <=10) {

		if ((runtime >> 6) & 1) {
			resetLED();
		}

		else {
			setRed();
		}
	}
}
