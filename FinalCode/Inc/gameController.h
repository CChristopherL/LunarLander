#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>
#include <stdio.h>

#define JOYSTICK_LEFT 0b00000100
#define JOYSTICK_RIGHT 0b00001000
#define JOYSTICK_UP 0b00000001
#define JOYSTICK_DOWN 0b00000010
#define BUTTON_1 0b00010000
#define BUTTON_2 0b00100000
#define BUTTON_3 0b01000000

void initGameController();
uint8_t readGameController();

#endif // JOYSTICK_H
