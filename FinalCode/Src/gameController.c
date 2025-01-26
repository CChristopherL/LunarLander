#include "gameController.h"
#include "30010_io.h"
#include "stm32f30x_conf.h"

void initGameController() {
    // Enable GPIOC clock
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

    // Set PC2 and PC3 to analog mode
    GPIOC->MODER |= (3 << (2 * 2)) | (3 << (3 * 2)); // Analog mode
    GPIOC->PUPDR &= ~((2 << (2 * 2)) | (3 << (3 * 2))); // No pull-up/pull-down

    // Enable ADC12 clock
    RCC->CFGR2 &= ~RCC_CFGR2_ADCPRE12; // Clear ADC12 prescaler bits
    RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6; // Set ADC12 prescaler to divide by 6
    RCC->AHBENR |= RCC_AHBPeriph_ADC12;

    // Reset ADC1
    ADC1->CR = 0x00000000; // Reset control register
    ADC1->CFGR &= 0xFDFFC007; // Clear ADC1 configuration register
    ADC1->SQR1 &= ~ADC_SQR1_L; // Clear regular sequence register

    ADC1->CR |= 0x10000000; // Enable internal voltage regulator
    for(int i = 0 ; i < 1000 ; i++) {} // Wait for about 16 microsecond

    // Perform ADC calibration
    ADC1->CR |= 0x80000000; // ADCAL
    while (ADC1->CR & 0x80000000) {} // Wait for calibration
    for (int i = 0 ; i < 100 ; i++) {} // Wait for a little while

    // Enable ADC1
    ADC1->CR |= 0x00000001; // ADEN
    while (!(ADC1->ISR & 0x00000001)) {} // Wait for ADC to be ready

    // Enable GPIOB clock for buttons
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;

    // Configure PB1 and PB2 as input with pull-down resistors
    GPIOB->MODER &= ~((3 << (1 * 2)) | (3 << (2 * 2))); // Input mode for PB1, PB2
    GPIOB->PUPDR |= (2 << (1 * 2)) | (2 << (2 * 2));    // Pull-down for PB1, PB2

    // Configure PB5 as input with pull-down resistors for boss key
    GPIOB->MODER &= ~(0x00000003 << (5 * 2)); // Clear mode register
    GPIOB->MODER |=  (0x00000000 << (5 * 2)); // Set mode register (0x00 –Input, 0x01 -Output, 0x02 -Alternate Function, 0x03 -Analog in/out)
    GPIOB->PUPDR &= ~(0x00000003 << (5 * 2)); // Clear push/pull register
    GPIOB->PUPDR |=  (0x00000002 << (5 * 2)); // Set push/pull register to pull-down (0x00 -No pull, 0x01 -Pull-up, 0x02 -Pull-down)
}

uint8_t readGameController() {
    // Joystick Y
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_181Cycles5);
    ADC_StartConversion(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
    uint16_t joystickY = ADC_GetConversionValue(ADC1);

    // Joystick X
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_181Cycles5);
    ADC_StartConversion(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
    uint16_t joystickX = ADC_GetConversionValue(ADC1);

    // Button 1
    uint8_t button1 = (GPIOB->IDR & (1 << 1)) ? 1 : 0; // PB0

    // Button 2
    uint8_t button2 = (GPIOB->IDR & (2 << 1)) ? 1 : 0; // PB1

    uint16_t gameControllerInput = 0;
    uint16_t deadzone = 256;

    if (joystickY > 2048 + deadzone) {
    	gameControllerInput |= (1 << 0); // Joystick Up
    }

    if (joystickY < 2048 - deadzone) {
    	gameControllerInput |= (1 << 1); // Joystick Down
    }

    if (joystickX < 2048 - deadzone) {
    	gameControllerInput |= (1 << 2); // Joystick Left
    }

    if (joystickX > 2048 + deadzone) {
    	gameControllerInput |= (1 << 3); // Joystick Right
    }

    gameControllerInput |= (button1 << 4); // Button 1
    gameControllerInput |= (button2 << 5); // Button 2

    // Digital joystick center (for bosskey)
    uint8_t button3 = (GPIOB->IDR & (1 << 5)) ? 1 : 0;;
    gameControllerInput |= (button3 << 6); // Button 3

    return gameControllerInput;
}
