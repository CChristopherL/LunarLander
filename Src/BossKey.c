/*
 * BossKey.c
 *
 *  Created on: 13 Jan 2025
 *      Author: magnu
 */
#include "stm32f30x.h"
#include "30010_io.h" // For UART communication and console handling
#include <stdio.h>
#include <stdlib.h>

void BossKeyInit(){

	RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port A
	GPIOB->MODER &= ~(0x00000003 << (5 * 2)); // Clear mode register
	GPIOB->MODER |=  (0x00000000 << (5 * 2)); // Set mode register (0x00 –Input, 0x01 -Output, 0x02 -Alternate Function, 0x03 -Analog in/out)
	GPIOB->PUPDR &= ~(0x00000003 << (5 * 2)); // Clear push/pull register
	GPIOB->PUPDR |=  (0x00000002 << (5 * 2)); // Set push/pull register to pull-down (0x00 -No pull, 0x01 -Pull-up, 0x02 -Pull-down)
}

uint8_t CheckBossKey(){
	uint8_t val = GPIOB->IDR & (0x0001 << 5); //Read from pin PB5 (center)
	return val;
}
void system_clear(void) {
    // Send ANSI escape code to clear the screen
    printf("\033[2J\033[H");
}

void BossKey(void) {
    system_clear(); // Clear the console using 30010_io.h
    printf("Boss Key Activated! Game hidden or minimized.\n");

    // Halt execution to simulate game minimization
    while (1) {
        // Wait for the Boss Key to be released
        if (!CheckBossKey()) {
            break;
        }
    }
    printf("Returning to game...\n");
}
