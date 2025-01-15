#ifndef RUNTIME_H_
#define RUNTIME_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern uint32_t runtime;

void initRuntime();
void TIM1_BRK_TIM15_IRQHandler();

#endif /* RUNTIME_H_ */
