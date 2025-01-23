#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "renderer.h"
#include "background.h"

static uint16_t*** getBufferImages() {
	static uint16_t **bufferImages = NULL;
	return &bufferImages;
}

static vector_t** getBufferImagePositions() {
	static vector_t *bufferImagePositions = NULL;
	return &bufferImagePositions;
}

static vector_t** getBufferImageSizes() {
	static vector_t *bufferImageSizes = NULL;
	return &bufferImageSizes;
}

static uint8_t* getBufferSize() {
	static uint8_t bufferSize = 0;
	return &bufferSize;
}

void addToBuffer(uint16_t *asciiImage, vector_t position, vector_t size) {
	static const uint8_t MAX_BUFFER_SIZE = 25;
	uint16_t ***bufferImages = getBufferImages();
	vector_t **bufferImagePositions = getBufferImagePositions();
	vector_t **bufferImageSizes = getBufferImageSizes();
	uint8_t *bufferSize = getBufferSize();

	if (*bufferImages == NULL || *bufferImagePositions == NULL || *bufferImageSizes == NULL) {
		*bufferImages = malloc(MAX_BUFFER_SIZE * sizeof(uint16_t*));
		*bufferImagePositions = malloc(MAX_BUFFER_SIZE * sizeof(vector_t));
		*bufferImageSizes = malloc(MAX_BUFFER_SIZE * sizeof(vector_t));
		*bufferSize = 0;
	}

	(*bufferImages)[*bufferSize] = asciiImage;
	(*bufferImagePositions)[*bufferSize] = position;
	(*bufferImageSizes)[*bufferSize] = size;
	*bufferSize = *bufferSize + 1;
}
/*
void pushBuffer() {
	// Merge and print the buffers
	static const uint8_t padding = 2;
	uint16_t **bufferImages = *getBufferImages();
	vector_t *bufferImagePositions = *getBufferImagePositions();
	vector_t *bufferImageSizes = *getBufferImageSizes();
	uint8_t bufferSize = *getBufferSize();

	for (uint8_t i = 0; i < bufferSize; i++) {
		// Create a new asciiImage with 1 character padding
		vector_t asciiImageSize = {bufferImageSizes[i].x + (padding << 1), bufferImageSizes[i].y + (padding << 1)};
		vector_t asciiImagePosition = {bufferImagePositions[i].x - padding, bufferImagePositions[i].y - padding};
		uint16_t *asciiImage = malloc(asciiImageSize.x * asciiImageSize.y * sizeof(uint16_t));
		memset(asciiImage, 0x0020, asciiImageSize.x * asciiImageSize.y * sizeof(uint16_t));

		// Copy the buffer image to new asciiImage
		for (uint8_t r = 0; r < bufferImageSizes[i].y; r++) {
			for (uint8_t c = 0; c < bufferImageSizes[i].x; c++) {
				asciiImage[(r + padding) * asciiImageSize.x + c + padding] = bufferImages[i][r * bufferImageSizes[i].x + c];
			}
		}

		// Merge images from buffer that are overlapping
		for (uint8_t j = 0; j < bufferSize; j++) {
			if (i == j) continue;

			// If images are overlapping copy characters from the image added to buffer most recently
			if (asciiImagePosition.x < bufferImagePositions[j].x + bufferImageSizes[j].x && bufferImagePositions[j].x < asciiImagePosition.x + asciiImageSize.x) {
				if (asciiImagePosition.y < bufferImagePositions[j].y + bufferImageSizes[j].y && bufferImagePositions[j].y < asciiImagePosition.y + asciiImageSize.y) {
					// Find the indexes inside ascii image where there is overlap
					uint16_t asciiImageStartColumn = 0;
					uint16_t asciiImageEndColumn = 0;
					uint16_t asciiImageStartRow = 0;
					uint16_t asciiImageEndRow = 0;

					if (bufferImagePositions[j].x > asciiImagePosition.x) {
						asciiImageStartColumn = bufferImagePositions[j].x - asciiImagePosition.x;
						asciiImageEndColumn = (bufferImagePositions[j].x + bufferImageSizes[j].x > asciiImagePosition.x + asciiImageSize.x) ? asciiImageSize.x : asciiImageStartColumn + bufferImageSizes[j].x;
					} else {
						asciiImageStartColumn = 0;
						asciiImageEndColumn = (bufferImagePositions[j].x + bufferImageSizes[j].x > asciiImagePosition.x + asciiImageSize.x) ? asciiImageSize.x : bufferImagePositions[j].x + bufferImageSizes[j].x - asciiImagePosition.x;
					}

					if (bufferImagePositions[j].y > asciiImagePosition.y) {
						asciiImageStartRow = bufferImagePositions[j].y - asciiImagePosition.y;
						asciiImageEndRow = (bufferImagePositions[j].y + bufferImageSizes[j].y > asciiImagePosition.y + asciiImageSize.y) ? asciiImageSize.y : asciiImageStartRow + bufferImageSizes[j].y;
					} else {
						asciiImageStartRow = 0;
						asciiImageEndRow = (bufferImagePositions[j].y + bufferImageSizes[j].y > asciiImagePosition.y + asciiImageSize.y) ? asciiImageSize.y : bufferImagePositions[j].y + bufferImageSizes[j].y - asciiImagePosition.y;
					}

					uint16_t bufferImageStartColumn = 0;
					uint16_t bufferImageEndColumn = 0;
					uint16_t bufferImageStartRow = 0;
					uint16_t bufferImageEndRow = 0;

					if (asciiImagePosition.x > bufferImagePositions[j].x) {
						bufferImageStartColumn = asciiImagePosition.x - bufferImagePositions[j].x;
						bufferImageEndColumn = (asciiImagePosition.x + asciiImageSize.x > bufferImagePositions[j].x + bufferImageSizes[j].x) ? bufferImageSizes[j].x : bufferImageStartColumn + asciiImageSize.x;
					} else {
						bufferImageStartColumn = 0;
						bufferImageEndColumn = (asciiImagePosition.x + asciiImageSize.x > bufferImagePositions[j].x + bufferImageSizes[j].x) ? bufferImageSizes[j].x : asciiImagePosition.x + asciiImageSize.x - bufferImagePositions[j].x;
					}

					if (asciiImagePosition.y > bufferImagePositions[j].y) {
						bufferImageStartColumn = asciiImagePosition.y - bufferImagePositions[j].y;
						bufferImageEndColumn = (asciiImagePosition.y + asciiImageSize.y > bufferImagePositions[j].y + bufferImageSizes[j].y) ? bufferImageSizes[j].y : bufferImageStartColumn + asciiImageSize.y;
					} else {
						bufferImageStartRow = 0;
						bufferImageEndRow = (asciiImagePosition.y + asciiImageSize.y > bufferImagePositions[j].y + bufferImageSizes[j].y) ? bufferImageSizes[j].y : asciiImagePosition.y + asciiImageSize.y - bufferImagePositions[j].y;
					}


					for (uint16_t r = asciiImageStartRow; r < asciiImageEndRow; r++) {
					    for (uint16_t c = asciiImageStartColumn; c < asciiImageEndColumn; c++) {
					        uint16_t bufferRow = r - asciiImageStartRow + bufferImageStartRow;
					        uint16_t bufferColumn = c - asciiImageStartColumn + bufferImageStartColumn;

					        if (j > i || (asciiImage[r * asciiImageSize.x + c] & 0x00FF) == 0x0020) {
					            asciiImage[r * asciiImageSize.x + c] = bufferImages[j][bufferRow * bufferImageSizes[j].x + bufferColumn];
					        }
					    }
					}

					// Loop over our asciiRows/asciiColumns and our bufferRows/bufferColumns
					/*for (uint16_t asciiRow = asciiImageStartRow - 1, bufferRow = bufferImageStartRow - 10; asciiRow < asciiImageEndRow - 1; asciiRow++, bufferRow++) {
						for (uint16_t asciiColumn = asciiImageStartColumn, bufferColumn = bufferImageStartColumn; asciiColumn < asciiImageEndColumn; asciiColumn++, bufferColumn++) {
							// If overlapping image has priority
							if (j > i && (bufferImages[j][bufferRow * bufferImageSizes[j].x + bufferColumn] & 0x00FF) != 0x0020) {
								// Replace the character entirely
								asciiImage[asciiRow * asciiImageSize.x + asciiColumn] = bufferImages[j][bufferRow * bufferImageSizes[j].x + bufferColumn];
							}

							// If overlapping image does not have priority
							//else if ((asciiImage[asciiRow * asciiImageSize.x + asciiColumn] & 0x00FF) == (0x0020)) {
							//	asciiImage[asciiRow * asciiImageSize.x + asciiColumn] = bufferImages[j][bufferRow * bufferImageSizes[j].x + bufferColumn];
							//}
						}
					}*/



					/*// Loop over every overlapping character
					for (uint16_t r = 0; r < asciiImageEndRow - asciiImageStartRow; r++) {
						for (uint16_t c = 0; c < asciiImageEndColumn - asciiImageStartColumn; c++) {
							// If overlapping image has priority
							if (j > i && i == 0) {
								// Replace the character entirely
								asciiImage[(r + asciiImageStartRow) * asciiImageSize.x + (c + asciiImageStartColumn)] = bufferImages[j][(r + bufferImageStartRow) * bufferImageSizes[j].x + (c + bufferImageStartColumn)];
							}
							// If overlapping image does not have priority
							else if ((asciiImage[(r + asciiImageStartRow) * asciiImageSize.x + (c + asciiImageStartColumn)] & 0x00FF) == (0x0020) && i == 0) {
								// Only replace if existing character is a space
								asciiImage[(r + asciiImageStartRow) * asciiImageSize.x + (c + asciiImageStartColumn)] = bufferImages[j][(r + bufferImageStartRow) * bufferImageSizes[j].x + (c + bufferImageStartColumn)];
							}

						}
					}
				}
			}
		}

		// Print the ascii image, where all the remaining space characters are used to generate the background
		displayASCIIImageWithBackground(asciiImage, asciiImagePosition, asciiImageSize);

		// Free the ascii image after it is printed
		free(asciiImage);
	}

	uint16_t ***bufferImagesPtr = getBufferImages();
	vector_t **bufferImagePositionsPtr = getBufferImagePositions();
	vector_t **bufferImageSizesPtr = getBufferImageSizes();
	uint8_t *bufferSizePtr = getBufferSize();

	// Clear the buffer
	free(*bufferImagesPtr);
	*bufferImagesPtr = NULL;
	free(*bufferImagePositionsPtr);
	*bufferImagePositionsPtr = NULL;
	free(*bufferImageSizesPtr);
	*bufferImageSizesPtr = NULL;
	*bufferSizePtr = 0;
}*/

// Helper function to clamp values within a range
uint16_t clamp(uint16_t value, uint16_t min, uint16_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Merge and print the buffers
void pushBuffer() {
    static const uint8_t padding = 2;
    uint16_t **bufferImages = *getBufferImages();
    vector_t *bufferImagePositions = *getBufferImagePositions();
    vector_t *bufferImageSizes = *getBufferImageSizes();
    uint8_t bufferSize = *getBufferSize();

    for (uint8_t i = 0; i < bufferSize; i++) {
		// Create a new asciiImage with 2 character padding to contain the merged ascii images from buffer
        vector_t asciiImageSize = {bufferImageSizes[i].x + (padding << 1), bufferImageSizes[i].y + (padding << 1)};
        vector_t asciiImagePosition = {bufferImagePositions[i].x - padding, bufferImagePositions[i].y - padding};
        asciiImagePosition.x = (asciiImagePosition.x < 0) ? (asciiImagePosition.x & 255) - 1 : asciiImagePosition.x & 255;
        uint16_t *asciiImage = malloc(asciiImageSize.x * asciiImageSize.y * sizeof(uint16_t));
        memset(asciiImage, 0x0020, asciiImageSize.x * asciiImageSize.y * sizeof(uint16_t));

        // Copy the current buffer image to new asciiImage
        for (uint8_t r = 0; r < bufferImageSizes[i].y; r++) {
            for (uint8_t c = 0; c < bufferImageSizes[i].x; c++) {
                asciiImage[(r + padding) * asciiImageSize.x + c + padding] = bufferImages[i][r * bufferImageSizes[i].x + c];
            }
        }

        // Merge overlapping images into asciiImage
        for (uint8_t j = 0; j < bufferSize; j++) {
            if (i == j) continue;

            // Calculate overlapping character indices
            int16_t overlapStartColumn = clamp(bufferImagePositions[j].x, asciiImagePosition.x, asciiImagePosition.x + asciiImageSize.x);
            int16_t overlapEndColumn = clamp(bufferImagePositions[j].x + bufferImageSizes[j].x, asciiImagePosition.x, asciiImagePosition.x + asciiImageSize.x);
            int16_t overlapStartRow = clamp(bufferImagePositions[j].y, asciiImagePosition.y, asciiImagePosition.y + asciiImageSize.y);
            int16_t overlapEndRow = clamp(bufferImagePositions[j].y + bufferImageSizes[j].y, asciiImagePosition.y, asciiImagePosition.y + asciiImageSize.y);

            // Translate calculated overlap character indices to local indices inside each image array
            int16_t asciiStartColumn = overlapStartColumn - asciiImagePosition.x;
            int16_t asciiStartRow = overlapStartRow - asciiImagePosition.y;
            int16_t bufferStartColumn = overlapStartColumn - bufferImagePositions[j].x;
            int16_t bufferStartRow = overlapStartRow - bufferImagePositions[j].y;
            int16_t overlapColumns = overlapEndColumn - overlapStartColumn;
            int16_t overlapRows = overlapEndRow - overlapStartRow;

            // Merge overlapping characters
            for (uint16_t r = 0; r < overlapRows; r++) {
                for (uint16_t c = 0; c < overlapColumns; c++) {
                    uint16_t bufferASCIICharacter = bufferImages[j][(bufferStartRow + r) * bufferImageSizes[j].x + (bufferStartColumn + c)];
                    uint16_t *asciiCharacter = &asciiImage[(asciiStartRow + r) * asciiImageSize.x + (asciiStartColumn + c)];

                    // Merge non-space characters (images most recently added to buffer are prioritized)
                    if (j > i && (bufferASCIICharacter & 0x00FF) != 0x0020) { // High-priority (on-top)
                        *asciiCharacter = bufferASCIICharacter;
                    } else if (((*asciiCharacter) & 0x00FF) == 0x0020) { // Low-priority (only replaces if their is a space character to be filled)
                    	*asciiCharacter = bufferASCIICharacter;
                    }
                }
            }
        }

        // Render the merged asciiImage
        displayASCIIImageWithBackground(asciiImage, asciiImagePosition, asciiImageSize);

        // Free memory
        free(asciiImage);
    }

    // Clear the buffer
    uint16_t ***bufferImagesPtr = getBufferImages();
    vector_t **bufferImagePositionsPtr = getBufferImagePositions();
    vector_t **bufferImageSizesPtr = getBufferImageSizes();
    uint8_t *bufferSizePtr = getBufferSize();

    free(*bufferImagesPtr);
    *bufferImagesPtr = NULL;
    free(*bufferImagePositionsPtr);
    *bufferImagePositionsPtr = NULL;
    free(*bufferImageSizesPtr);
    *bufferImageSizesPtr = NULL;
    *bufferSizePtr = 0;
}

void displayASCIIImageWithBackground(uint16_t *asciiImage, vector_t position, vector_t size) {
	// Merge the background ascii image and the ascii image (ignore ' ' space characters)
	for (uint8_t c = 0; c < size.x; c++) {
		for (uint8_t r = 0; r < size.y; r++) {
			if ((asciiImage[r * size.x + c] & 0x00FF) == ' ') {
				asciiImage[r * size.x + c] = getBackgroundASCIICharacter((vector_t){position.x + c, position.y + r});
			}
		}
	}

	drawASCIIImage(asciiImage, position, size);
}


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
				sprintf(asciiString + strlen(asciiString), "%\n%c[%dA", ESC, 1);
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
/*void drawASCIIImageWithBackground(uint16_t *asciiImage, vector_t position, vector_t size) {
	// Get the background behind the ascii image plus a padding size of 2 characters
	uint8_t const padding = 2;
	vector_t backgroundPosition = {position.x - padding, position.y - padding};
	vector_t backgroundSize = {size.x + (padding << 1), size.y + (padding << 1)};
	uint16_t *backgroundASCIIImage = getBackgroundASCIIImage(backgroundPosition, backgroundSize);

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
}*/
