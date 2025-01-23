/*
 * lives.c
 *
 *  Created on: 14 Jan 2025
 *      Author: magnu
 */

#include "stm32f30x.h"
#include "30010_io.h"
#include <stdio.h>

const uint8_t emptyHeart[5] = {
    14,  // Column 1: 00000000
    17, // Column 2: 10001000
    34,  // Column 3: 01000100
    17, // Column 4: 10001000
    14   // Column 5: 01100000
};
const uint8_t filledHeart[5] = {
	    14,  // Column 1: 00000000
	    31, // Column 2: 10001000
	    62,  // Column 3: 01000100
	    31, // Column 4: 10001000
	    14   // Column 5: 01100000
};
void displayHearts(uint8_t *buffer, int lives) {
    // Write "Lives: " to the buffer
	 lcd_write_string(buffer, "Lives: ", 0, 0);
    // Display hearts based on the number of lives
    for (int i = 0; i < 3; i++) {
        if (i < lives) {
            // Display filled heart
            for (uint8_t col = 0; col < 5; col++) {
                buffer[0 * 128 + 36 + (i * 6) + col] = filledHeart[col];
            }
        } else {
            // Display unfilled heart
            for (uint8_t col = 0; col < 5; col++) {
                buffer[0 * 128 + 36 + (i * 6) + col] = emptyHeart[col];
            }
        }
    }

    // Push the buffer to the LCD
    lcd_push_buffer(buffer);
}
void handleCollisionLives(uint8_t *buffer, int *lives) {
    static uint8_t prevState = 0; // Keep track of PB5 state

    // Read the current state of PB5
    uint8_t currentState = (GPIOB->IDR & (1 << 5)) ? 1 : 0;

    // Detect rising edge (current state is high and previous state is low)
    if (currentState && !prevState) {
        if (*lives > 1) {
            (*lives)--;

            // Print to Putty
            printf("Collision detected! Lives remaining: %d\n", *lives);

            // Update the LCD display
            displayHearts(buffer, *lives);
        } else {
            // Game Over Logic: Clear LCD and print "Game Over"
            memset(buffer, 0x00, 512);           // Clear the buffer
            lcd_write_string(buffer, "GAME", 54, 1); // Write "Game Over" on line 2
            lcd_write_string(buffer, "OVER", 54, 2);
            lcd_push_buffer(buffer);             // Push to LCD
            printf("Game Over!\n");              // Print to Putty
        }
    }

    // Update the previous state
    prevState = currentState;
}
