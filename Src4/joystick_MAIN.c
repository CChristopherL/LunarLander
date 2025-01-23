#include "stm32f30x.h"
#include "30010_io.h"
#include <stdio.h>
#include "joystick.h"
#include <stdbool.h>

#define FILTER_SIZE 5

int main() {
    // Initialize UART communication at 9600 baud rate
    uart_init(9600);

    // Initialize ADC and configure joystick pins
    initADC();
    initPins();

    // Initialize buttons for user input
    initButtons();

    // Variables to store raw and adjusted joystick values
    uint16_t pa0 = 0, pa1 = 0;
    int16_t pa0_adjusted = 0, pa1_adjusted = 0;

    // Buffers to store filtered joystick values
    int16_t x_buffer[FILTER_SIZE] = {0};
    int16_t y_buffer[FILTER_SIZE] = {0};

    // Structure to hold joystick calibration data
    JoystickCalibration calibration;

    // Variable to store digital joystick state
    uint8_t joystickState;

    // Calibrate the joystick to find center and range
    calibrateJoystick(&calibration);

    while (1) {
        // Read raw analog values from joystick
        analogReadJoystick(&pa0, &pa1);

        // Update buffers with the latest joystick values
        updateBuffer(x_buffer, pa0, FILTER_SIZE);
        updateBuffer(y_buffer, pa1, FILTER_SIZE);

        // Calculate the average value from the buffers
        int16_t filtered_pa0 = calculateAverage(x_buffer, FILTER_SIZE);
        int16_t filtered_pa1 = calculateAverage(y_buffer, FILTER_SIZE);

        // Adjust the filtered values based on calibration data
        pa0_adjusted = joystickAdjust(filtered_pa0, PA0, &calibration);
        pa1_adjusted = joystickAdjust(filtered_pa1, PA1, &calibration);

        // Determine digital joystick state with hysteresis
        joystickState = readJoystickAnalog(pa0_adjusted, pa1_adjusted, 20);

        // Print the adjusted joystick values and digital state
        printf("\rAnalog Joystick: X = %4d, Y = %4d   ", pa0_adjusted, pa1_adjusted);
        printf("Digital Joystick State: 0x%02X\n", joystickState);

        // Check if boost button (PB0) is pressed
        if (readButton1()) {
            printf("Boost activated!\n");
        }

        // Check if shoot button (PB1) is pressed
        if (readButton2()) {
            printf("Shoot fired!\n");
        }

        // Simple delay loop for debouncing and readability
        for (volatile uint32_t i = 0; i < 100000; i++) __NOP();
    }
}
