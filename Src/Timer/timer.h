#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f30x_conf.h"
#include "30010_io.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ESC 0x1B

typedef struct {uint16_t hours, minutes, seconds, milliseconds, enabled;} timeCustom_t;

timeCustom_t time;
timeCustom_t split1;
timeCustom_t split2;

void updateTimer(timeCustom_t *time);

void startStop();
void splitTime1();
void splitTime2();
void resetTime();

void displayTimer();

void initializeTimer();
void TIM1_BRK_TIM15_IRQHandler();

#endif /* TIMER_H_ */
