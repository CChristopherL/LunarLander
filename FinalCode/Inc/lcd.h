#ifndef LCD_H_
#define LCD_H_

#include <string.h>

void lcd_write_string(uint8_t* buffer, char* string, int16_t slice, uint8_t line);
void displayHealth(uint8_t* buffer, uint8_t health);
void displayGameTime(uint8_t *buffer, uint32_t runtime);
void displayScore(uint8_t *buffer, uint32_t score);
void displayFuel(uint8_t *buffer, uint16_t Fuel);

#endif /* LCD_H_ */
