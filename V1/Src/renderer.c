#include "renderer.h"
#include "background.h"

void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
	// Initialize the ascii string
	uint16_t maxASCIIStringSize = size.x * size.y << 5;
	char *asciiString = malloc(maxASCIIStringSize * sizeof(char));
	memset(asciiString, '\0', maxASCIIStringSize * sizeof(char));

	// Set initial colors
	uint8_t currentFaint = asciiImage[0] >> 15;
	uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
	uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
	uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;
	sprintf(asciiString + strlen(asciiString), "%c[%d;%d;%d;%dm", ESC, (currentFaint) ? 2 : 22, (currentBlink) ? 5 : 25, currentForegroundColor + 30, currentBackgroundColor + 40);

	for (uint8_t r = 0; r < size.y; r++) {
		// Skip character if it is out of bounds in y-direction
		int16_t y = position.y + r;
		if (y < 0 || y > 95) {
			continue;
		}

		// Set initial cursor position
		sprintf(asciiString + strlen(asciiString), "%c[%d;%dH", ESC, y + 1, position.x + 1);

		for (uint8_t c = 0; c < size.x; c++) {
			// Move up a line if character is to the right edge of the screen
			uint16_t x = position.x + c;
			if (x == 256 && y < 95) {
				sprintf(asciiString + strlen(asciiString), "\n%c[%dA", ESC, 1);
			} else if (x == 256) {
				sprintf(asciiString + strlen(asciiString), "%c[%dA", ESC, 1);
			}

			// Change attributes upon attribute change (faint, blink, foreground color, background color)
			uint16_t asciiImageIndex = r * size.x + c;
			uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
			uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
			uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
			uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

			if (currentFaint != newFaint || currentBlink != newBlink || currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
				currentFaint = newFaint;
				currentBlink = newBlink;
				currentForegroundColor = newForegroundColor;
				currentBackgroundColor = newBackgroundColor;
				sprintf(asciiString + strlen(asciiString), "%c[%d;%d;%d;%dm", ESC, (currentFaint) ? 2 : 22, (currentBlink) ? 5 : 25, currentForegroundColor + 30, currentBackgroundColor + 40);
			}

			// Append the most recent character
			sprintf(asciiString + strlen(asciiString), "%c", (asciiImage[r * size.x + c] & 0x00FF));
		}
	}

	// Re-allocate the string
	asciiString = realloc(asciiString, strlen(asciiString) * sizeof(char));

	// Print the ascii string
	printf("%s%c[1;1H", asciiString, ESC);

	// De-allocate the ascii string
	free(asciiString);
}

// initBackground() from "background.h" must be called to enable background image
void drawASCIIImageWithBackground(uint16_t *asciiImage, vector_t position, vector_t size) {
	// Get the background behind the ascii image plus a padding size of 2 characters
	uint8_t const padding = 2;
	vector_t backgroundPosition = {position.x - padding, position.y - padding};
	vector_t backgroundSize = {size.x + (padding << 1), size.y + (padding << 1)};
	uint16_t *backgroundASCIIImage = getBackgroundASCIIImage(backgroundPosition, backgroundSize, NULL, NULL, asciiImage, padding);

	backgroundPosition.x = (position.x - padding < 0) ? ((position.x - padding) & 255) - 1 : (position.x - padding) & 255;

	// Merge the background ascii image and the ascii image (ignore ' ' space characters)
	for (uint8_t c = 0; c < size.x; c++) {
		for (uint8_t r = 0; r < size.y; r++) {
			if ((asciiImage[r * size.x + c] & 0x00FF) != ' ') {
				backgroundASCIIImage[(r + padding) * (size.x + (padding << 1)) + (c + padding)] = asciiImage[r * size.x + c];
			}
		}
	}

	// Generate and print the final ascii image
	drawASCIIImage(backgroundASCIIImage, backgroundPosition, backgroundSize);

	// De-allocate the backgroundASCIIImage
	destroyBackgroundASCIIImage(backgroundASCIIImage);
}
