
// to run "music" you need: "buzzer", "timer"
// to run "music" call "initMusic()" in main

#ifndef MUSIC_H_
#define MUSIC_H_

#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "buzzer.h"
#include "timer.h"

#define AS2 233
#define B2	245
#define F3	349
#define FS3	370
#define A3	440
#define AS3	466
#define C4	523
#define CS4	554
#define D4	587
#define DS4	622
#define E4	659
#define F4	698


extern uint16_t note;
extern uint16_t beats;
extern const uint16_t melodies[64];

void initMusic();
void setMelody();
void TIM1_BRK_TIM15_IRQHandler();

#endif /* MUSIC_H_ */
