#include "30010_io.h"
#include "stm32f30x_conf.h"

#include "runtime.h";

uint32_t runtime = 0;

void initRuntime() {
	RCC->APB2ENR |= RCC_APB2Periph_TIM15;
	TIM15->CR1 = 0x0000; // Configure timer 15
	TIM15->ARR = 0x2180; // Set reload value
	TIM15->PSC = 99; // Set prescale value
	TIM15->CR1 |= 0x0001;

	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts

	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}

void TIM1_BRK_TIM15_IRQHandler() {
	runtime++;

	TIM15->SR &= ~0x0001; // Clear interrupt bit
}
