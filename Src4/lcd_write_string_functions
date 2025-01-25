#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "charset.h"
#include <string.h>


uint8_t empty_heart[7] = {0x0E, 0x11, 0x21, 0x42, 0x21, 0x11, 0x0E};
uint8_t full_heart[7] = {0x0E, 0x1F, 0x3F, 0x7E, 0x3F, 0x1F, 0x0E};

void lcd_write_string(uint8_t* buffer, char* string, int16_t slice, uint8_t line) {



	for (uint16_t i = 0; i < strlen(string); i++) {
		for (uint16_t j = 0; j < 7; j++) {
			if (line * 128 + slice + i * 8 + j > (128 * (line + 1)) - 1) {
				slice -= 128;
			}

			if (line * 128 + slice + i * 8 + j < (128 * line)) {
				slice += 128;
			}
			buffer[line * 128 + slice + i * 8 + j] = character_data[string[i] - 0x20][j];
		}
	}
	lcd_push_buffer(buffer);
}







void displayHealth(uint8_t *buffer, uint8_t health) {



	if (health == 3) {
		lcd_write_string(buffer, "~~~", 104, 0);
	}
	else if (health == 2) {
			lcd_write_string(buffer, "~~}", 104, 0);
		}

	else if (health == 1) {
			lcd_write_string(buffer, "~}}", 104, 0);
		}

	else {

	lcd_write_string(buffer, "}}}", 104, 0);
	}



}

void displayGameTime(uint8_t *buffer, uint32_t runtime) {

	uint8_t minutes = (runtime / 4500);
	uint8_t seconds = (runtime / 75) % 60;
	uint16_t centiseconds = runtime % 75;
	char timeString[22];
	sprintf(timeString,"Time: %02d:%02d.%02d", minutes, seconds, centiseconds);

	lcd_write_string(buffer,timeString, 10, 2);
}


void displayScore(uint8_t *buffer, uint32_t score) {

	char displayScore[13];
	sprintf(displayScore,"Score: %06lu", (score % 1000000));
	lcd_write_string(buffer,displayScore, 10, 3);
}

void displayFuel(uint8_t *buffer,uint16_t fuel) {
	char displayFuel[11];
	sprintf(displayFuel,"Fuel: %04d", fuel);
	lcd_write_string(buffer,displayFuel, 0, 0);
}



void points4landing(int32_t *score, int32_t levelTime) {
	int32_t points = 10000;

	if ((points - levelTime) < 1000) {
		points = 1000;
	}
	else {
		points -= levelTime;
	}

	*score += points;

	printf("points: %d\t score: %d\n hello", points, *score);
}





