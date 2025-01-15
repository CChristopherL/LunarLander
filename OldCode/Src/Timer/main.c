#include "joystick.h"
#include "led.h"
#include "timer.h"

int main(void)
{
	uart_init(921600);

	initializeJoystick();

	uint8_t joystickState = 0;

	resetTime();
	initializeTimer();
	displayTime();

	while(1) {
		if (joystickState != readJoystick()) {
			joystickState = readJoystick();
			if (joystickState & 0b00000010) { // DOWN
				resetTime();
			}

			if (joystickState & 0b00000100) { // LEFT
				splitTime1();
			}

			if (joystickState & 0b00001000) { // RIGHT
				splitTime2();
			}

			if (joystickState & 0b00010000) { // CENTER
				startStop();
			}
		}

		displayTime();
		// Small delay //
		for (int i = 0; i < 100000; i++) {
			printf("");
		}
	}
}
