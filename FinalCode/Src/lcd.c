#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "charset.h"

void lcd_write_string(uint8_t* buffer, char* string, int16_t slice, uint8_t line) {
	for (uint16_t i = 0; i < strlen(string); i++) {
		for (uint16_t j = 0; j < 7; j++) {
			if (line * 128 + slice + i * 8 + j > (128 * (line + 1)) - 1) {
				slice -= 128;
			}

			if (line * 128 + slice + i * 8 + j < (128 * line)) {
				slice += 128;
			}
			buffer[line * 128 + slice + i * 8 + j] = getCharacterData(string[i] - 0x20)[j];
		}
	}
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
	lcd_write_string(buffer,timeString, 10, 1);
}

void displayScore(uint8_t *buffer, uint32_t score) {
	char displayScore[13];
	sprintf(displayScore, "Score: %06lu", (score % 1000000));
	lcd_write_string(buffer,displayScore, 10, 2);
}

void displayFuel(uint8_t *buffer,uint16_t fuel) {
	char displayFuel[11];
	sprintf(displayFuel, "Fuel: %04d", fuel);
	lcd_write_string(buffer,displayFuel, 0, 0);
}
