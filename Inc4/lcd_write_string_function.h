/*
 * lcd_write_string_function.h
 *
 *  Created on: 23. jan. 2025
 *      Author: root
 */

#ifndef LCD_WRITE_STRING_FUNCTION_H_
#define LCD_WRITE_STRING_FUNCTION_H_



void lcd_write_string(uint8_t* buffer, char* string, int16_t slice, uint8_t line);
void displayHealth(uint8_t* buffer, uint8_t health);
void displayGameTime(uint8_t *buffer, uint32_t runtime);
void displayScore(uint8_t *buffer, uint32_t score);
void displayFuel(uint8_t *buffer, uint16_t Fuel);
void points4landing(int32_t *score, int32_t levelTime);

#endif /* LCD_WRITE_STRING_FUNCTION_H_ */
