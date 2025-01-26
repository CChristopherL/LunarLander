#include "30010_io.h"
#include "stm32f30x_conf.h"

#include "runtime.h"

void initRuntime(uint32_t *runtime) {
	RCC->APB2ENR |= RCC_APB2Periph_TIM15;
	TIM15->CR1 = 0x0000;
	TIM15->ARR = 0x2180;
	TIM15->PSC = 99;
	TIM15->CR1 |= 0x0001;

	TIM15->DIER |= 0x0001;

	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0);
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);

	incrementRuntime(runtime);
}

void incrementRuntime(uint32_t *runtime) {
	static uint32_t *runtimePtr = NULL;

	if (runtime != NULL) {
		runtimePtr = runtime;
	}

	if (runtimePtr != NULL) {
		(*runtimePtr)++;
	}
}

void resetRuntime(uint32_t *runtime) {
	*runtime = 0;
}

void TIM1_BRK_TIM15_IRQHandler() {
	incrementRuntime(NULL);

	// Clear interrupt bit
	TIM15->SR &= ~0x0001;
}
