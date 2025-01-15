#ifndef LED_H_
#define LED_H_

#include "timer.h"
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course


void resetLED();
void setRed();
void setGreen();
void setBlue();


#endif /* LED_H_ */
