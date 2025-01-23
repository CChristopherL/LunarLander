#include "stm32f30x_conf.h"
#include "30010_io.h"

#include "led.h"

void initLEDs() {
	// Enable clock for GPIOA, GPIOB, and GPIOC
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
	RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

	resetLEDs();
}

void resetLEDs() {
	// Reset red
	GPIOB->OSPEEDR &= ~(0x00000003 << (4 * 2));
	GPIOB->OTYPER &= ~(0x0001 << 4);
	GPIOB->MODER &= ~(0x00000003 << (4 * 2));
	GPIOB->ODR &= ~(0x00000003 << (4 * 2));

	// Reset green
	GPIOC->OSPEEDR &= ~(0x00000003 << (7 * 2));
	GPIOC->OTYPER &= ~(0x0001 << 7);
	GPIOC->MODER &= ~(0x00000003 << (7 * 2));
	GPIOC->ODR &= ~(0x00000003 << (7 * 2));

	// Reset blue
	GPIOA->OSPEEDR &= ~(0x00000003 << (9 * 2));
	GPIOA->OTYPER &= ~(0x0001 << 9);
	GPIOA->MODER &= ~(0x00000003 << (9 * 2));
	GPIOA->ODR &= ~(0x00000003 << (9 * 2));
}


void setRed() {
	GPIOB->OSPEEDR |= (0x00000002 << (4 * 2));
	GPIOB->OTYPER |= (0x0000 << 4);
	GPIOB->MODER |= (0x00000001 << (4 * 2));
	GPIOB->ODR |= (0x00000001 << (4 * 2));
}

void setGreen() {
	GPIOC->OSPEEDR |= (0x00000002 << (7 * 2));
	GPIOC->OTYPER |= (0x0000 << 7);
	GPIOC->MODER |= (0x00000001 << (7 * 2));
	GPIOC->ODR |= (0x00000001 << (7 * 2));
}

void setBlue() {
	GPIOA->OSPEEDR |= (0x00000002 << (9 * 2));
	GPIOA->OTYPER |= (0x0000 << 9);
	GPIOA->MODER |= (0x00000001 << (9 * 2));
	GPIOA->ODR |= (0x00000001 << (9 * 2));
}
