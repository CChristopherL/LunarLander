//to run "buzzer" you need: "timer"

#ifndef BUZZER_H_
#define BUZZER_H_

#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
void initBuzzer();
void setFreq(uint16_t freq);

#endif /* BUZZER_H_ */

