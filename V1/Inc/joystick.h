#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "stm32f30x_conf.h"
#include "30010_io.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void initializeJoystick();
uint8_t readJoystick();

#endif /* JOYSTICK_H_ */
