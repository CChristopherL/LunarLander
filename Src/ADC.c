/*
 * ADC.c
 *
 *  Created on: 13 Jan 2025
 *      Author: magnu
 */
#include "stm32f30x_conf.h"
#include "stm32f30x.h"
#include "30010_io.h"
#include <string.h>

extern uint8_t graphicsBuffer[];

void initPins(void) {
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    GPIOA->MODER |= (3 << (0 * 2)) | (3 << (1 * 2)); // Analog mode for PA0 and PA1
    GPIOA->PUPDR &= ~((3 << (0 * 2)) | (3 << (1 * 2))); // No pull-up, no pull-down
}

void initADC(){
	RCC->CFGR2 &= ~RCC_CFGR2_ADCPRE12;
	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6;
	RCC->AHBENR |= RCC_AHBPeriph_ADC12;

	ADC1->CR = 0x00000000;
	ADC1->CFGR &= 0xFDFFC007;
	ADC1->SQR1 &= ~ADC_SQR1_L;

	ADC1->CR |= 0x10000000;
	for (int i =0 ; i<1000 ; i++) {}

	ADC1->CR |= 0x80000000;
	while (!(ADC1->CR & 0x80000000));
	for (int i=0 ; i<100; i++){}

	ADC1->CR |= 0x00000001;
	while (!(ADC1->ISR & 0x00000001));
}
void analogRead_PA0(uint16_t *pa0_value, uint16_t *joystick_angle) {

	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_1Cycles5);
	ADC_StartConversion(ADC1);

	while (ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0);

	*pa0_value = ADC_GetConversionValue(ADC1);
	printf("Conversion value at PA0 and angle: %d\n", *pa0_value);

}
uint16_t ADC_Measure(uint8_t channel) {
    // Configure the ADC to read from the specified channel
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_1Cycles5);

    // Start the conversion
    ADC_StartConversion(ADC1);

    // Wait for the conversion to complete
    while (!(ADC1->ISR & ADC_ISR_EOC)); // End of conversion flag

    // Return the result
    return ADC_GetConversionValue(ADC1);
}

void lcd_update(void) {
    uint16_t adc_pa0, adc_pa1;  // ADC values for PA0 and PA1

    // Fetch ADC measurements
    adc_pa0 = ADC_Measure(1); // Channel 1 (PA0)
    adc_pa1 = ADC_Measure(2); // Channel 2 (PA1)

    // Clear the buffer
    memset(graphicsBuffer, 0x00, 512);

    // Create a string with the ADC values
    char message[64];
    sprintf(message, "PA0: %4d PA1: %4d", adc_pa0, adc_pa1);

    // Write the string to the graphics buffer
    lcd_write_strings(message, 0, 0); // Top row

    // Push the buffer to the LCD
    lcd_push_buffer(graphicsBuffer);
}


