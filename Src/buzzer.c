#include <buzzer.h>

void initBuzzer() {
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
	GPIOB->MODER &= ~(0x00000003 << (10 * 2));
	GPIOB->MODER |= (0x00000002 << (10 * 2));
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);

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

void setFreq(uint16_t freq) {
	uint32_t reload = 64e6 / freq / (9 + 1) - 1;
	TIM2->ARR = reload;
	TIM2->CCR3 = reload / 2;

	TIM2->EGR |= 0x01;
}
