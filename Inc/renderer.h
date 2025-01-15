#ifndef RENDERER_H_
#define RENDERER_H_

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define ESC 0x1B

void initDisplay(uint32_t *runtime);
void** getBatch(uint8_t batch);
void freeBatch(void** batchData);
void refreshBatch(uint8_t batch);
void refreshBackground();

#endif /* RENDERER_H_ */
