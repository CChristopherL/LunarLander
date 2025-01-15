//to run fuelgauge you need: "LED"


#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "timer.h"
#include "LED.h"
#ifndef LED_H_
#define LED_H_

void fuelgauge(uint16_t fuel);

#endif /* LED_H_ */
