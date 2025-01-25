#ifndef RUNTIME_H_
#define RUNTIME_H_

#include <stdint.h>

void initRuntime(uint32_t *runtime);
void incrementRuntime(uint32_t *runtime);
void resetRuntime(uint32_t *runtime);
void TIM1_BRK_TIM15_IRQHandler();

#endif /* RUNTIME_H_ */
