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

/*void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
    // Step 1: Calculate the exact size needed
    size_t exactSize = 1; // Null terminator
    char temp[20]; // Temporary buffer for size calculation

    // Initial color settings
    uint8_t currentFaint = asciiImage[0] >> 15;
    uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
    uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
    uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

    // Add size for initial color escape sequence
    exactSize += snprintf(temp, sizeof(temp), "%c[%d;%d;%d;%dm", ESC,
                          currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                          currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add size for cursor position escape sequence
        exactSize += snprintf(temp, sizeof(temp), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add size for attribute change escape sequence, if necessary
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                exactSize += snprintf(temp, sizeof(temp), "%c[%d;%d;%d;%dm", ESC,
                                      newFaint ? 2 : 22, newBlink ? 5 : 25,
                                      newForegroundColor + 30, newBackgroundColor + 40);

                currentFaint = newFaint;
                currentBlink = newBlink;
                currentForegroundColor = newForegroundColor;
                currentBackgroundColor = newBackgroundColor;
            }

            // Add size for the character itself
            exactSize += 1;
        }
    }

    // Step 2: Allocate the exact buffer size
    char *asciiString = malloc(exactSize);
    if (!asciiString) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Step 3: Build the string
    char *current = asciiString;

    // Add initial color escape sequence
    current += snprintf(current, exactSize, "%c[%d;%d;%d;%dm", ESC,
                        currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                        currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add cursor position escape sequence
        current += snprintf(current, exactSize - (current - asciiString), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add attribute change escape sequence if necessary
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                current += snprintf(current, exactSize - (current - asciiString), "%c[%d;%d;%d;%dm", ESC,
                                    newFaint ? 2 : 22, newBlink ? 5 : 25,
                                    newForegroundColor + 30, newBackgroundColor + 40);

                currentFaint = newFaint;
                currentBlink = newBlink;
                currentForegroundColor = newForegroundColor;
                currentBackgroundColor = newBackgroundColor;
            }

            // Add the character
            *current++ = (asciiImage[asciiImageIndex] & 0x00FF);
        }
    }

    // Null-terminate the string
    *current = '\0';

    // Step 4: Print the string
    printf("%s%c[1;1H", asciiString, ESC);

    // Step 5: Free memory
    free(asciiString);
}*/

/*
void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
    // Step 1: Calculate the exact buffer size
    size_t exactSize = 1; // Null terminator
    char temp[20];        // Temporary buffer for size calculation

    // Initial color settings
    uint8_t currentFaint = asciiImage[0] >> 15;
    uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
    uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
    uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

    // Add space for initial color escape sequence
    exactSize += snprintf(temp, sizeof(temp), "%c[22;25;%d;%dm", ESC, currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add space for cursor position escape sequence
        exactSize += snprintf(temp, sizeof(temp), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add escape sequences only for changed attributes
            if (currentFaint != newFaint) {
                exactSize += snprintf(temp, sizeof(temp), "%d;", newFaint ? 2 : 22);
                currentFaint = newFaint;
            }
            if (currentBlink != newBlink) {
                exactSize += snprintf(temp, sizeof(temp), "%d;", newBlink ? 5 : 25);
                currentBlink = newBlink;
            }
            if (currentForegroundColor != newForegroundColor) {
                exactSize += snprintf(temp, sizeof(temp), "%d;", newForegroundColor + 30);
                currentForegroundColor = newForegroundColor;
            }
            if (currentBackgroundColor != newBackgroundColor) {
                exactSize += snprintf(temp, sizeof(temp), "%d;", newBackgroundColor + 40);
                currentBackgroundColor = newBackgroundColor;
            }

            // Add space for the character itself
            exactSize += 1;
        }
    }

    // Step 2: Allocate the exact buffer size
    char *asciiString = malloc(exactSize);
    if (!asciiString) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    char *current = asciiString;

    // Step 3: Build the string
    // Add initial color escape sequence
    current += snprintf(current, exactSize, "%c[22;25;%d;%dm", ESC, currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add cursor position escape sequence
        current += snprintf(current, exactSize - (current - asciiString), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add escape sequences only for changed attributes
            if (currentFaint != newFaint) {
                current += snprintf(current, exactSize - (current - asciiString), "%c[%d;", ESC, newFaint ? 2 : 22);
                currentFaint = newFaint;
            }
            if (currentBlink != newBlink) {
                current += snprintf(current, exactSize - (current - asciiString), "%d;", newBlink ? 5 : 25);
                currentBlink = newBlink;
            }
            if (currentForegroundColor != newForegroundColor) {
                current += snprintf(current, exactSize - (current - asciiString), "%d;", newForegroundColor + 30);
                currentForegroundColor = newForegroundColor;
            }
            if (currentBackgroundColor != newBackgroundColor) {
                current += snprintf(current, exactSize - (current - asciiString), "%d;", newBackgroundColor + 40);
                currentBackgroundColor = newBackgroundColor;
            }

            // Terminate the attribute change
            if (current[-1] == ';') {
                *current++ = 'm';
            }

            // Add the character
            *current++ = (asciiImage[asciiImageIndex] & 0x00FF);
        }
    }

    // Null-terminate the string
    *current = '\0';

    // Step 4: Print the string
    printf("%s%c[1;1H", asciiString, ESC);

    // Step 5: Free memory
    free(asciiString);
}*/

/*
void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
    // Step 1: Calculate the exact size needed
    size_t exactSize = 1; // Null terminator
    char temp[20]; // Temporary buffer for size calculation

    // Initial color settings
    uint8_t currentFaint = asciiImage[0] >> 15;
    uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
    uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
    uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

    // Add size for initial color escape sequence
    exactSize += snprintf(temp, sizeof(temp), "%c[%d;%d;%d;%dm", ESC,
                          currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                          currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add size for cursor position escape sequence
        exactSize += snprintf(temp, sizeof(temp), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add size for attribute change escape sequence, if necessary
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                exactSize += snprintf(temp, sizeof(temp), "%c[%d;%d;%d;%dm", ESC,
                                      newFaint ? 2 : 22, newBlink ? 5 : 25,
                                      newForegroundColor + 30, newBackgroundColor + 40);

                currentFaint = newFaint;
                currentBlink = newBlink;
                currentForegroundColor = newForegroundColor;
                currentBackgroundColor = newBackgroundColor;
            }

            // Add size for the character itself
            exactSize += 1;
        }
    }

    // Step 2: Allocate the exact buffer size
    char *asciiString = malloc(exactSize);
    if (!asciiString) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Step 3: Build the string
    char *current = asciiString;

    // Add initial color escape sequence
    current += snprintf(current, exactSize, "%c[%d;%d;%d;%dm", ESC,
                        currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                        currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add cursor position escape sequence
        current += snprintf(current, exactSize - (current - asciiString), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add attribute change escape sequence if necessary
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                current += snprintf(current, exactSize - (current - asciiString), "%c[%d;%d;%d;%dm", ESC,
                                    newFaint ? 2 : 22, newBlink ? 5 : 25,
                                    newForegroundColor + 30, newBackgroundColor + 40);

                currentFaint = newFaint;
                currentBlink = newBlink;
                currentForegroundColor = newForegroundColor;
                currentBackgroundColor = newBackgroundColor;
            }

            // Add the character
            *current++ = (asciiImage[asciiImageIndex] & 0x00FF);
        }
    }

    // Null-terminate the string
    *current = '\0';

    // Step 4: Print the string
    printf("%s%c[1;1H", asciiString, ESC);

    // Step 5: Free memory
    free(asciiString);
}*/


/*void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
    // Pre-calculate the required buffer size
    size_t maxASCIIStringSize = 1; // Null terminator
    char temp[20];

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add space for cursor position escape code
        maxASCIIStringSize += snprintf(temp, sizeof(temp), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Add space for attribute changes (if any)
            maxASCIIStringSize += 14; // Worst-case escape sequence length

            // Add space for the character itself
            maxASCIIStringSize += 1;
        }
    }

    // Allocate buffer
    char *asciiString = malloc(maxASCIIStringSize);
    if (!asciiString) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    char *current = asciiString;
    size_t remainingSize = maxASCIIStringSize;

    // Initial colors
    uint8_t currentFaint = asciiImage[0] >> 15;
    uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
    uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
    uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

    // Add initial color escape sequence
    current += snprintf(current, remainingSize, "%c[%d;%d;%d;%dm", ESC,
                        currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                        currentForegroundColor + 30, currentBackgroundColor + 40);
    remainingSize = maxASCIIStringSize - (current - asciiString);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add cursor position
        current += snprintf(current, remainingSize, "%c[%d;%dH", ESC, y + 1, position.x + 1);
        remainingSize = maxASCIIStringSize - (current - asciiString);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Change attributes if necessary
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                current += snprintf(current, remainingSize, "%c[%d;%d;%d;%dm", ESC,
                                    newFaint ? 2 : 22, newBlink ? 5 : 25,
                                    newForegroundColor + 30, newBackgroundColor + 40);
                remainingSize = maxASCIIStringSize - (current - asciiString);

                currentFaint = newFaint;
                currentBlink = newBlink;
                currentForegroundColor = newForegroundColor;
                currentBackgroundColor = newBackgroundColor;
            }

            // Append character
            *current++ = (asciiImage[asciiImageIndex] & 0x00FF);
            remainingSize--;
        }
    }

    // Null-terminate and print
    *current = '\0';
    printf("%s%c[1;1H", asciiString, ESC);

    // Free memory
    free(asciiString);
}*/

/*void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
    // Step 1: Calculate the exact buffer size
    size_t maxASCIIStringSize = 1; // Null terminator
    char temp[50]; // Temporary buffer for size calculations

    uint8_t currentFaint = asciiImage[0] >> 15;
    uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
    uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
    uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

    // Add size for initial color escape sequence
    maxASCIIStringSize += snprintf(temp, sizeof(temp), "%c[%d;%d;%d;%dm", ESC,
                                   currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                                   currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add size for cursor position escape sequence
        maxASCIIStringSize += snprintf(temp, sizeof(temp), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add size for attribute changes, if necessary
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                maxASCIIStringSize += snprintf(temp, sizeof(temp), "%c[", ESC);
                if (currentFaint != newFaint) maxASCIIStringSize += snprintf(temp, sizeof(temp), "%d;", newFaint ? 2 : 22);
                if (currentBlink != newBlink) maxASCIIStringSize += snprintf(temp, sizeof(temp), "%d;", newBlink ? 5 : 25);
                if (currentForegroundColor != newForegroundColor)
                    maxASCIIStringSize += snprintf(temp, sizeof(temp), "%d;", newForegroundColor + 30);
                if (currentBackgroundColor != newBackgroundColor)
                    maxASCIIStringSize += snprintf(temp, sizeof(temp), "%d;", newBackgroundColor + 40);
                maxASCIIStringSize += 1; // For 'm'
            }

            // Add size for the character itself
            maxASCIIStringSize += 1;
        }
    }

    printf("NOW ALLOCATING: %zu BYTES\n", maxASCIIStringSize);

    // Step 2: Allocate the buffer
    char *asciiString = malloc(maxASCIIStringSize);
    if (!asciiString) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Step 3: Build the string
    char *current = asciiString;

    // Add initial color escape sequence
    current += snprintf(current, maxASCIIStringSize, "%c[%d;%d;%d;%dm", ESC,
                        currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                        currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add cursor position escape sequence
        current += snprintf(current, maxASCIIStringSize - (current - asciiString), "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add attribute changes, if necessary
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                current += snprintf(current, maxASCIIStringSize - (current - asciiString), "%c[", ESC);
                if (currentFaint != newFaint)
                    current += snprintf(current, maxASCIIStringSize - (current - asciiString), "%d;", newFaint ? 2 : 22);
                if (currentBlink != newBlink)
                    current += snprintf(current, maxASCIIStringSize - (current - asciiString), "%d;", newBlink ? 5 : 25);
                if (currentForegroundColor != newForegroundColor)
                    current += snprintf(current, maxASCIIStringSize - (current - asciiString), "%d;", newForegroundColor + 30);
                if (currentBackgroundColor != newBackgroundColor)
                    current += snprintf(current, maxASCIIStringSize - (current - asciiString), "%d;", newBackgroundColor + 40);
                *current++ = 'm';

                // Update attributes
                currentFaint = newFaint;
                currentBlink = newBlink;
                currentForegroundColor = newForegroundColor;
                currentBackgroundColor = newBackgroundColor;
            }

            // Add the character
            *current++ = (asciiImage[asciiImageIndex] & 0x00FF);
        }
    }

    // Null-terminate the string
    *current = '\0';

    // Step 4: Print the string
    printf("%s%c[1;1H", asciiString, ESC);

    // Step 5: Free memory
    free(asciiString);
}*/






/*void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
	// Calculate the size of the string needed to print ascii escape codes + characters in our image
	uint16_t maxASCIIStringSize = 1;

	// Set initial colors
	uint8_t currentFaint = asciiImage[0] >> 15;
	uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
	uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
	uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

	maxASCIIStringSize += (currentFaint) ? 1 : 2; // Faint setting color number of characters
	maxASCIIStringSize += (currentBlink) ? 1 : 2; // Blink setting color number of character
	maxASCIIStringSize += 4; // Foreground and background color number of characters

	//TODO: if memory is still a problem, set initial cursor position once, and use skip row and colums for next rows because it uses less memory

	for (uint8_t r = 0; r < size.y; r++) {
		// Skip character if it is out of bounds in y-direction
		int16_t y = position.y + r;
		if (y < 0 || y > 95) {
			continue;
		}

		// Add amount of characters in escape code for setting the initial cursor position
		maxASCIIStringSize += 9;

		for (uint8_t c = 0; c < size.x; c++) {
			// Move up a line if character is to the right edge of the screen
			uint16_t x = position.x + c;

			// Add the amount of escape ascii characters this operation would add to the string
			if (x == 256 && y < 95) {
				maxASCIIStringSize += 5;
			} else if (x == 256) {
				maxASCIIStringSize += 4;
			}

			// Change attributes upon attribute change (faint, blink, foreground color, background color)
			uint16_t asciiImageIndex = r * size.x + c;
			uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
			uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
			uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
			uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

			// Add the amount of characters in the escape code for setting the attributes for this character (faint, blink, foreground color, background color)
			if ((currentFaint != newFaint || currentBlink != newBlink || currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor)) {
				//maxASCIIStringSize += 14;
				maxASCIIStringSize += 3;
				maxASCIIStringSize += (currentFaint != newFaint) ? 3 : 0;
				maxASCIIStringSize += (currentBlink != newBlink) ? 3 : 0;
				maxASCIIStringSize += (currentForegroundColor != newForegroundColor) ? 3 : 0;
				maxASCIIStringSize += (currentBackgroundColor != newBackgroundColor) ? 3 : 0;
			}

			// Add a character to our string size for this character
			maxASCIIStringSize += 1;
		}
	}

	printf("NOW ALLOCATING: %d BYTES\n", maxASCIIStringSize);

	// Initialize the ascii string
	char *asciiString = malloc(maxASCIIStringSize * sizeof(char));
	memset(asciiString, '\0', maxASCIIStringSize * sizeof(char));
	printf("ALLOCATED\n");

	// Set initial colors
	currentFaint = asciiImage[0] >> 15;
	currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
	currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
	currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;
	//asciiString = realloc(asciiString, (strlen(asciiString) + 15) * sizeof(char));
	sprintf(asciiString + strlen(asciiString), "%c[%d;%d;%d;%dm", ESC, (currentFaint) ? 2 : 22, (currentBlink) ? 5 : 25, currentForegroundColor + 30, currentBackgroundColor + 40);

	for (uint8_t r = 0; r < size.y; r++) {
		// Skip character if it is out of bounds in y-direction
		int16_t y = position.y + r;
		if (y < 0 || y > 95) {
			continue;
		}

		printf("SETTING CURSOR POSITION\n");

		// Set initial cursor position
		//asciiString = realloc(asciiString, (strlen(asciiString) + 10) * sizeof(char));
		sprintf(asciiString + strlen(asciiString), "%c[%d;%dH", ESC, y + 1, position.x + 1);

		for (uint8_t c = 0; c < size.x; c++) {
			// Move up a line if character is to the right edge of the screen
			uint16_t x = position.x + c;
			if (x == 256 && y < 95) {
				//asciiString = realloc(asciiString, (strlen(asciiString) + 6) * sizeof(char));
				sprintf(asciiString + strlen(asciiString), "\n%c[%dA", ESC, 1);
			} else if (x == 256) {
				//asciiString = realloc(asciiString, (strlen(asciiString) + 5) * sizeof(char));
				sprintf(asciiString + strlen(asciiString), "%c[%dA", ESC, 1);
			}


			// Change attributes upon attribute change (faint, blink, foreground color, background color)
			uint16_t asciiImageIndex = r * size.x + c;
			uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
			uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
			uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
			uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

			if ((currentFaint != newFaint || currentBlink != newBlink || currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor)) {
				printf("ATTRIBUTE CHANGED!\n");
				sprintf(asciiString + strlen(asciiString), "%c[", ESC);
				if (currentFaint != newFaint) {
					sprintf(asciiString + strlen(asciiString), "%d;", (newFaint) ? 2 : 22);
				}
				if (currentBlink != newBlink) {
					sprintf(asciiString + strlen(asciiString), "%d;", (newBlink) ? 5 : 25);
				}
				if (currentForegroundColor != newForegroundColor) {
					sprintf(asciiString + strlen(asciiString), "%d;", newForegroundColor + 30);
				}
				if (currentBackgroundColor != newBackgroundColor) {
					sprintf(asciiString + strlen(asciiString), "%d;", newBackgroundColor + 40);
				}

				currentFaint = newFaint;
				currentBlink = newBlink;
				currentForegroundColor = newForegroundColor;
				currentBackgroundColor = newBackgroundColor;
				sprintf(asciiString + strlen(asciiString), "%c", 0x6D);
				//asciiString = realloc(asciiString, (strlen(asciiString) + 15) * sizeof(char));
				//sprintf(asciiString + strlen(asciiString), "%c[%d;%d;%d;%dm", ESC, (currentFaint) ? 2 : 22, (currentBlink) ? 5 : 25, currentForegroundColor + 30, currentBackgroundColor + 40);
			}

			// Append the most recent character
			//asciiString = realloc(asciiString, (strlen(asciiString) + 2) * sizeof(char));
			sprintf(asciiString + strlen(asciiString), "%c", (asciiImage[r * size.x + c] & 0x00FF));
		}
	}
	printf("PRINTING\n");

	// Print the ascii string
	printf("%s%c[1;1H", asciiString, ESC);

	// De-allocate the ascii string
	free(asciiString);
}*/

/*void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {

}*/

/*void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
    // Step 1: Calculate the size of the final ASCII string
    uint16_t maxASCIIStringSize = 1; // Null terminator
    char tempBuffer[100]; // Temporary buffer for calculating lengths

    // Initial color settings
    uint8_t currentFaint = asciiImage[0] >> 15;
    uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
    uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
    uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

    printf("Inside draw and counting");

    // Add size for the initial color escape code
    maxASCIIStringSize += sprintf(tempBuffer, "%c[%d;%d;%d;%dm", ESC,
                                   currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                                   currentForegroundColor + 30, currentBackgroundColor + 40);

    // Traverse the image to calculate size
    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add size for cursor position
        maxASCIIStringSize += sprintf(tempBuffer, "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract new attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add size for attribute change escape codes
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                maxASCIIStringSize += sprintf(tempBuffer, "%c[%d;%d;%d;%dm", ESC,
                                               newFaint ? 2 : 22, newBlink ? 5 : 25,
                                               newForegroundColor + 30, newBackgroundColor + 40);
                currentFaint = newFaint;
                currentBlink = newBlink;
                currentForegroundColor = newForegroundColor;
                currentBackgroundColor = newBackgroundColor;
            }

            // Add size for the character
            maxASCIIStringSize += 1;
        }
    }

    printf("Allocating buffer size: %d bytes\n", maxASCIIStringSize);

    // Step 2: Allocate the buffer
    char *asciiString = malloc(maxASCIIStringSize);
    if (!asciiString) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    memset(asciiString, '\0', maxASCIIStringSize);

    // Step 3: Build the ASCII string
    char *current = asciiString;

    // Add initial color escape sequence
    current += sprintf(current, "%c[%d;%d;%d;%dm", ESC,
                        currentFaint ? 2 : 22, currentBlink ? 5 : 25,
                        currentForegroundColor + 30, currentBackgroundColor + 40);

    for (uint8_t r = 0; r < size.y; r++) {
        int16_t y = position.y + r;
        if (y < 0 || y > 95) continue;

        // Add cursor position escape sequence
        current += sprintf(current, "%c[%d;%dH", ESC, y + 1, position.x + 1);

        for (uint8_t c = 0; c < size.x; c++) {
            uint16_t asciiImageIndex = r * size.x + c;

            // Extract new attributes
            uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
            uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
            uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
            uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

            // Add attribute change escape sequence if necessary
            if (currentFaint != newFaint || currentBlink != newBlink ||
                currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor) {
                current += sprintf(current, "%c[%d;%d;%d;%dm", ESC,
                                    newFaint ? 2 : 22, newBlink ? 5 : 25,
                                    newForegroundColor + 30, newBackgroundColor + 40);
                currentFaint = newFaint;
                currentBlink = newBlink;
                currentForegroundColor = newForegroundColor;
                currentBackgroundColor = newBackgroundColor;
            }

            // Add the character
            *current++ = (asciiImage[asciiImageIndex] & 0x00FF);
        }
    }

    // Null-terminate the string
    *current = '\0';

    // Step 4: Print the ASCII string
    printf("%s%c[1;1H", asciiString, ESC);

    // Step 5: Free the buffer
    free(asciiString);
}*/


void drawASCIIImage(uint16_t *asciiImage, vector_t position, vector_t size) {
	// Initialize the ascii string
	uint16_t maxASCIIStringSize = size.x * size.y + 1;
	char *asciiString = malloc(maxASCIIStringSize * sizeof(char));
	memset(asciiString, '\0', maxASCIIStringSize * sizeof(char));

	// Set initial colors
	uint8_t currentFaint = asciiImage[0] >> 15;
	uint8_t currentBlink = (asciiImage[0] & 0b0100000000000000) >> 14;
	uint8_t currentForegroundColor = (asciiImage[0] & 0b0011100000000000) >> 11;
	uint8_t currentBackgroundColor = (asciiImage[0] & 0b0000011100000000) >> 8;

	char *escapeCodeForCharacter = malloc(100 * sizeof(char));
	memset(escapeCodeForCharacter, '\0', 100 * sizeof(char));

	asciiString = realloc(asciiString, (strlen(asciiString) + 15) * sizeof(char));
	sprintf(escapeCodeForCharacter + strlen(escapeCodeForCharacter), "%c[%d;%d;%d;%dm", ESC, (currentFaint) ? 2 : 22, (currentBlink) ? 5 : 25, currentForegroundColor + 30, currentBackgroundColor + 40);

	for (uint8_t r = 0; r < size.y; r++) {
		// Skip character if it is out of bounds in y-direction
		int16_t y = position.y + r;
		if (y < 0 || y > 95) {
			continue;
		}

		// Set initial cursor position
		asciiString = realloc(asciiString, (strlen(asciiString) + 10) * sizeof(char));
		sprintf(escapeCodeForCharacter + strlen(escapeCodeForCharacter), "%c[%d;%dH", ESC, y + 1, position.x + 1);

		for (uint8_t c = 0; c < size.x; c++) {
			// Move up a line if character is to the right edge of the screen
			uint16_t x = position.x + c;
			if (x == 256 && y < 95) {
				asciiString = realloc(asciiString, (strlen(asciiString) + 6) * sizeof(char));
				sprintf(escapeCodeForCharacter + strlen(escapeCodeForCharacter), "\n%c[%dA", ESC, 1);
			} else if (x == 256) {
				asciiString = realloc(asciiString, (strlen(asciiString) + 5) * sizeof(char));
				sprintf(escapeCodeForCharacter + strlen(escapeCodeForCharacter), "%c[%dA", ESC, 1);
			}


			// Change attributes upon attribute change (faint, blink, foreground color, background color)
			uint16_t asciiImageIndex = r * size.x + c;
			uint8_t newFaint = asciiImage[asciiImageIndex] >> 15;
			uint8_t newBlink = (asciiImage[asciiImageIndex] & 0b0100000000000000) >> 14;
			uint8_t newForegroundColor = (asciiImage[asciiImageIndex] & 0b0011100000000000) >> 11;
			uint8_t newBackgroundColor = (asciiImage[asciiImageIndex] & 0b0000011100000000) >> 8;

			if ((currentFaint != newFaint || currentBlink != newBlink || currentForegroundColor != newForegroundColor || currentBackgroundColor != newBackgroundColor)) {
				currentFaint = newFaint;
				currentBlink = newBlink;
				currentForegroundColor = newForegroundColor;
				currentBackgroundColor = newBackgroundColor;
				asciiString = realloc(asciiString, (strlen(asciiString) + 15) * sizeof(char));
				sprintf(escapeCodeForCharacter + strlen(escapeCodeForCharacter), "%c[%d;%d;%d;%dm", ESC, (currentFaint) ? 2 : 22, (currentBlink) ? 5 : 25, currentForegroundColor + 30, currentBackgroundColor + 40);
			}

			// Append the most recent character
			escapeCodeForCharacter = realloc(escapeCodeForCharacter, (strlen(escapeCodeForCharacter) + 100) * sizeof(char));
			asciiString = realloc(asciiString, (strlen(asciiString) + strlen(escapeCodeForCharacter) + 2) * sizeof(char));
			sprintf(asciiString + strlen(asciiString), "%s%c", escapeCodeForCharacter, (asciiImage[r * size.x + c] & 0x00FF));
			memset(escapeCodeForCharacter, '\0', 100 * sizeof(char));
		}
	}

	// Print the ascii string
	printf("%s%c[1;1H", asciiString, ESC);

	// De-allocate the ascii string
	free(asciiString);
	free(escapeCodeForCharacter);
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
