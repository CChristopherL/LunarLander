#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


void initTimer15(); //timer 15 initialized for "music"
void initTimer2(); //timer 2 initialized for "buzzer"


#endif /* TIMER_H_ */
