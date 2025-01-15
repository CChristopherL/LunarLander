#include "timer.h"

//timer 15 initialized for "music"
void initTimer15() {

	RCC->APB2ENR |= RCC_APB2Periph_TIM15;

	TIM15->CR1 = 0x0000; // Configure timer 15
	TIM15->ARR = 0x2180; // Set reload value
	TIM15->PSC = 349; // Set prescale value
	TIM15->CR1 |= 0x0001;
	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts

	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}

void initTimer2() {
	RCC->APB1ENR |= RCC_APB1Periph_TIM2;
	TIM2->CR1 = 0x0000;
	TIM2->ARR = 0xF9FF;
	TIM2->PSC = 9;
	TIM2->CCER &= ~TIM_CCER_CC3P; // Clear CCER register
	TIM2->CCER |= 0x00000001 << 8; // Enable OC3 output

	TIM2->CCMR2 &= ~TIM_CCMR2_OC3M; // Clear CCMR2 register
	TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;
	TIM2->CCMR2 |= TIM_OCMode_PWM1; // Set output mode to PWM1
	TIM2->CCMR2 &= ~TIM_CCMR2_OC3PE;
	TIM2->CCMR2 |=  TIM_OCPreload_Enable;
	TIM2->CCR3 = 0xF9FF / 2; // Set duty cycle to 50 %
	TIM2->CR1 |= 0x0001;
}
