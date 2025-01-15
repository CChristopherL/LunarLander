#include "timer.h"

void initializeTimer() {
	RCC->APB2ENR |= RCC_APB2Periph_TIM15;
	TIM15->CR1 = 0x0000; // Configure timer 15
	TIM15->ARR = 0xF9FF; // Set reload value
	TIM15->PSC = 9; // Set prescale value
	TIM15->CR1 |= 0x0001;

	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts

	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}

void updateTime(timeCustom_t *time, uint8_t milliseconds) {
	time->milliseconds += milliseconds;
	if (time->milliseconds > 1000) {
		time->milliseconds -= 1000;
		time->seconds++;
	}

	if (time->seconds > 60) {
		time->seconds -= 60;
		time->minutes++;
	}

	if (time->minutes > 60) {
		time->minutes -= 60;
		time->hours++;
	}
}

void TIM1_BRK_TIM15_IRQHandler() {
	if (time.enabled) {
		updateTime(&time, 10);
	}

	if (time.enabled && split1.enabled) {
		updateTime(&split1, 10);
	}

	if (time.enabled && split2.enabled) {
		updateTime(&split2, 10);
	}

	TIM15->SR &= ~0x0001; // Clear interrupt bit
}

void startStop() {
	time.enabled = ~(time.enabled);
}

void splitTime1() {
	split1.milliseconds = 0;
	split1.seconds = 0;
	split1.minutes = 0;
	split1.hours = 0;
	split1.enabled = 0;

	split1.enabled = 1;
}

void splitTime2() {
	split2.milliseconds = 0;
	split2.seconds = 0;
	split2.minutes = 0;
	split2.hours = 0;
	split2.enabled = 0;

	split2.enabled = 1;
}

void resetTime() {
	time.milliseconds = 0;
	time.seconds = 0;
	time.minutes = 0;
	time.hours = 0;
	time.enabled = 0;

	split1.milliseconds = 0;
	split1.seconds = 0;
	split1.minutes = 0;
	split1.hours = 0;
	split1.enabled = 0;

	split2.milliseconds = 0;
	split2.seconds = 0;
	split2.minutes = 0;
	split2.hours = 0;
	split2.enabled = 0;
}

void displayTime() {
	printf("%c[2J", ESC);
	printf("%c[0;0H", ESC);
	printf("Timer: %02d:%02d:%02d.%03d\n", time.hours, time.minutes, time.seconds, time.milliseconds);
	printf("Split 1: %02d:%02d:%02d.%03d\n", split1.hours, split1.minutes, split1.seconds, split1.milliseconds);
	printf("Split 2: %02d:%02d:%02d.%03d\n", split2.hours, split2.minutes, split2.seconds, split2.milliseconds);
}
