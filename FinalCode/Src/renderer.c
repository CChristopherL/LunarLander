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
	uint32_t maxASCIIStringSize = 1; // Begins at 1 to accomodate for \0 terminator
	char buffer[500];

    // Initial color settings
    uint8_t currentFaint = asciiImage[0] >> 15;
    uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
    uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
    uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

    maxASCIIStringSize += sprintf(buffer, "%c[%d;%d;%d;%dm", ESC, currentFaint ? 2 : 22, currentBlink ? 5 : 25, currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
    	int16_t y = position.y + r;
    	if (y < 0 || y > 95) continue;

    	maxASCIIStringSize += sprintf(buffer, "%c[%d;%dH", ESC, y + 1, position.x + 1);

    	for (uint8_t c = 0; c < size.x; c++) {
    		// Move up a line if character is to the right edge of the screen
			uint16_t x = position.x + c;
			if (x == 256 && y < 95) {
				maxASCIIStringSize += sprintf(buffer + strlen(buffer), "\n%c[%dA", ESC, 1);
			} else if (x == 256) {
				maxASCIIStringSize += sprintf(buffer + strlen(buffer), "%c[%dA", ESC, 1);
			}

    		uint16_t asciiImageCharacterIndex = r * size.x + c;

            // Extract new attributes
            uint8_t newFaint = asciiImage[asciiImageCharacterIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageCharacterIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageCharacterIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageCharacterIndex] & 0b0000011100000000) >> 8;

            // If there is an attribute change (faint, blink, foreground color, background color)
            if (currentFaint != newFaint || currentBlink != newBlink || currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
            	maxASCIIStringSize += sprintf(buffer, "%c[", ESC);
            	if (currentFaint != newFaint) {
            		currentFaint = newFaint;
            		maxASCIIStringSize += sprintf(buffer, "%d;", (currentFaint ? 2 : 22));
            	}

            	if (currentBlink != newBlink) {
            		currentBlink = newBlink;
            		maxASCIIStringSize += sprintf(buffer, "%d;", (currentBlink ? 5 : 25));
            	}

            	if (currentForegroundColor != newForegroundColor) {
            		currentForegroundColor = newForegroundColor;
            		maxASCIIStringSize += sprintf(buffer, "%d;", (currentForegroundColor + 30));
            	}

            	if (currentBackgroundColor != newBackgroundColor) {
            		currentBackgroundColor = newBackgroundColor;
            		maxASCIIStringSize += sprintf(buffer, "%d;", (currentBackgroundColor + 40));
            	}

            	maxASCIIStringSize -= 1; // Remove the extra ; suffix when changing multiple colors
            	maxASCIIStringSize += sprintf(buffer, "%c", 'm');
            }

            maxASCIIStringSize += sprintf(buffer, "%c", (asciiImage[asciiImageCharacterIndex] & 0x00FF));
    	}
    }

    char *asciiString = malloc(maxASCIIStringSize * sizeof(char));
    memset(asciiString, '\0', maxASCIIStringSize * sizeof(char));

    currentFaint = asciiImage[0] >> 15;
    currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
    currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
    currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

    sprintf(asciiString + strlen(asciiString), "%c[%d;%d;%d;%dm", ESC, currentFaint ? 2 : 22, currentBlink ? 5 : 25, currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
    	int16_t y = position.y + r;
    	if (y < 0 || y > 95) continue;

    	// Setting cursor position
    	sprintf(asciiString + strlen(asciiString), "%c[%d;%dH", ESC, y + 1, position.x + 1);

    	for (uint8_t c = 0; c < size.x; c++) {
    		// Move up a line if character is to the right edge of the screen
			uint16_t x = position.x + c;
			if (x == 256 && y < 95) {
				sprintf(asciiString + strlen(asciiString), "\n%c[%dA", ESC, 1);
			} else if (x == 256) {
				sprintf(asciiString + strlen(asciiString), "%c[%dA", ESC, 1);
			}

    		uint16_t asciiImageCharacterIndex = r * size.x + c;

            // Extract new attributes
            uint8_t newFaint = asciiImage[asciiImageCharacterIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageCharacterIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageCharacterIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageCharacterIndex] & 0b0000011100000000) >> 8;

            // If there is an attribute change (faint, blink, foreground color, background color)
            if (currentFaint != newFaint || currentBlink != newBlink || currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
            	sprintf(asciiString + strlen(asciiString), "%c[", ESC);
            	if (currentFaint != newFaint) {
            		currentFaint = newFaint;
            		sprintf(asciiString + strlen(asciiString), "%d;", (currentFaint ? 2 : 22));
            	}

            	if (currentBlink != newBlink) {
            		currentBlink = newBlink;
            		sprintf(asciiString + strlen(asciiString), "%d;", (currentBlink ? 5 : 25));
            	}

            	if (currentForegroundColor != newForegroundColor) {
            		currentForegroundColor = newForegroundColor;
            		sprintf(asciiString + strlen(asciiString), "%d;", (currentForegroundColor + 30));
            	}

            	if (currentBackgroundColor != newBackgroundColor) {
            		currentBackgroundColor = newBackgroundColor;
            		sprintf(asciiString + strlen(asciiString), "%d;", (currentBackgroundColor + 40));
            	}

            	asciiString[strlen(asciiString) - 1] = 'm';
            }

            sprintf(asciiString + strlen(asciiString), "%c", (asciiImage[asciiImageCharacterIndex] & 0x00FF));
    	}
    }

    // Print the ascii image string
    printf("%s%c[1;1H", asciiString, ESC);
    free(asciiString);
}
