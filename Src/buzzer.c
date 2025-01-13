#include <buzzer.h>


void initBuzzer() {
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
	GPIOB->MODER &= ~(0x00000003 << (10 * 2));
	GPIOB->MODER |= (0x00000002 << (10 * 2));
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);

	initTimer2();
}

void setFreq(uint16_t freq) {
	uint32_t reload = 64e6 / freq / (9 + 1) - 1;
	TIM2->ARR = reload;
	TIM2->CCR3 = reload / 2;

	TIM2->EGR |= 0x01;
}
