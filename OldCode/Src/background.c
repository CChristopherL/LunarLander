#include "30010_io.h"
#include "stm32f30x_conf.h"

#include "background.h"
#include "math.h"

void initDisplay(uint32_t *runtime) {
	// Flash memory address
	uint32_t baseAddress = 0x08004000;

	// Set seed based on runtime
	uint32_t seed = *runtime;
	srand(seed);

	// Set sin shift amount
	uint16_t terrainShift = rand() & 511; //TODO: Move this to terrain library

	// Flash memory has 32 pages (2048 bytes each), but we will only need the first 24.
	for (uint8_t batch = 0; batch < 24; batch++) {
		// Unlock the memory to allow for our page writing operation
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

		// Flash memory page address
		uint32_t pageAddress = baseAddress + (23 - batch) * 0x800;

		// Erase the page
		FLASH_ErasePage(pageAddress);

		// Loop through every address in the page
		for (uint16_t offset = 0; offset < 1024; offset++) {
			// Find the x and y positions on our display that the data should represent
			uint16_t x = ((batch & 7) << 5) + (offset & 31); // Equivalent to and faster than x = 32 * (batch % 8) + offset % 32
			uint16_t y = ((batch >> 3) << 5) + (offset >> 5); // Equivalent to and faster than y = 32 * (batch / 8) + offset / 32

			// Write background/foreground/character data based on our x and y display position
			uint16_t data;// = 0b0111011101000000;//0b0000000001000000;
			if (y > (87 - ((sinLUT((x << 3) + terrainShift) - cosLUT(x << 2)) >> 13))) { //TODO: Move this to terrain library
				switch ((rand() & 3)) {
					case 0: // Make terrain the '#' character
						data = 0b0000011100100011; // background(4bits) foreground(4bits) character(8bits)
						break;
					case 1: // Make terrain the '@' character
						data = 0b0000011101000000; // background(4bits) foreground(4bits) character(8bits)
						break;
					case 2: // Make terrain the 'Ä' character
						data = 0b0000011110001110; // background(4bits) foreground(4bits) character(8bits)
						break;
					case 3: // Make terrain the 'Æ' character
						data = 0b0000011110010010; // background(4bits) foreground(4bits) character(8bits)
						break;
					default: // Make terrain the 'W' character
						data = 0b0000011101010111; // background(4bits) foreground(4bits) character(8bits)

				}
			} else {
				if ((rand() & 511) == 1) {
					data = 0b0000011100101010; // bg(4bits) fg(4bits) char(8bits)
				} else {
					data = 0b0000000001000000; // bg(4bits) fg(4bits) char(8bits)
				}
			}
			FLASH_ProgramHalfWord(pageAddress + (offset << 1), data);
		}

		// Lock the memory and finish our page writing operation
		FLASH_Lock();
	}

}

void** getBatch(uint8_t batch) {
	// Flash memory page address
	uint32_t pageAddress = 0x08004000 + (23 - batch) * 0x00000800;

	// Allocate memory to store flash memory page data
	void** batchData = malloc(3 * sizeof(void*));
	batchData[0] = malloc(1024 * sizeof(char));		// Character
	batchData[1] = malloc(1024 * sizeof(uint8_t));	// Foreground color
	batchData[2] = malloc(1024 * sizeof(uint8_t));	// Background color

	// Read every address in the page
	uint16_t offsetData;
	for (uint16_t offset = 0; offset < 1024; offset++) {
		offsetData = *(uint16_t *)(pageAddress + (offset << 1));
		((char*)batchData[0])[offset] = (offsetData & 0x00FF); // Our character data
		((uint8_t*)batchData[1])[offset] = (offsetData & 0x0F00) >> 8; // Our foreground color data
		((uint8_t*)batchData[2])[offset] = (offsetData & 0xF000) >> 12; // Our background color data
	}

	return batchData;
}

void freeBatch(void** batchData) {
	// Free the allocated memory to store flash memory page data
	free((char*)batchData[0]);
	free((uint8_t*)batchData[1]);
	free((uint8_t*)batchData[2]);
	free(batchData);
}

void refreshBatch(uint8_t batch) {
	// Get background/foreground/character data from corresponding page in the flash memory
	void** batchData = getBatch(batch);
	uint8_t currentForegroundColor = ((uint8_t*)batchData[1])[0];
	uint8_t currentBackgroundColor = ((uint8_t*)batchData[1])[0];

	// Set foreground and background color
	printf("%c[%d;%dm", ESC, currentForegroundColor + 30, currentBackgroundColor + 40);

	// Loop through the batch rows
	for (uint8_t y = 0; y < 32; y++) {
		// Set the cursor position to the start of the row
		//printf("Moving cursor to X: %d, Y: %d\n", (batch & 7) * 32 + 1, (batch >> 3) * 32 + y + 1);
		printf("%c[%u;%uH", ESC, ((batch >> 3) << 5) + y + 1, ((batch & 7) << 5) + 1);

		// Stores all consecutive characters with identical foreground and background color
		char* chunk = malloc(33 * sizeof(char));
		memset(chunk, '\0', 33 * sizeof(char));

		// Loop through the batch columns
		uint8_t shiftX = 0;
		for (uint8_t x = 0; x < 32; x++) {
			// Current character's foreground or background color differ
			if (((uint8_t*)batchData[1])[(y << 5) + x] != currentForegroundColor || ((uint8_t*)batchData[2])[(y << 5) + x] != currentBackgroundColor) {
				// Print merged characters
				printf("%s", chunk);

				// Reset chunked characters array
				free(chunk);
				chunk = malloc((33 - x) * sizeof(char));
				memset(chunk, '\0', (33 - x) * sizeof(char));

				// Update to new foreground and background colors
				currentForegroundColor = ((uint8_t*)batchData[1])[(y << 5) + x];
				currentBackgroundColor = ((uint8_t*)batchData[2])[(y << 5) + x];
				printf("%c[%d;%dm", ESC, currentForegroundColor + 30, currentBackgroundColor + 40);

				// Save distance into row where new chunk was created
				shiftX = x;
			}

			// Add character to our merged characters
			chunk[x - shiftX] = ((char*)batchData[0])[(y << 5) + x];
		}

		// End of row reached, so print and free our chunk
		printf("%s", chunk);
		free(chunk);
	}

	// Batch data is no longer needed, so de-allocate it from memory
	freeBatch(batchData);
}

void refreshBackground() {
	for (uint8_t batch = 0; batch < 24; batch++) {
		refreshBatch(batch);
	}
}
