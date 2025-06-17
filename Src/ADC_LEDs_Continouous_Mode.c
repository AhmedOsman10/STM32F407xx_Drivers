/*
    This C code is designed to read analog signals using ADC1 on an STM32 microcontroller and control LEDs based on threshold values.

    🧠 Core Idea:
    - Initializes the system clock and enables GPIO and ADC1 peripherals.
    - Sets up ADC1 in **continuous conversion mode** to monitor two analog input channels.
    - Configures 3 GPIO pins (PA5, PB1, PC3) as analog inputs.
    - Enables two specific ADC channels:
        - Channel 5 (PA5)
        - Channel 13 (PC3)
    - Defines 3 LEDs connected to PA1 (Red), PB2 (Blue), and PC2 (Yellow), though only Red and Yellow are used in logic.
    - Continuously reads the two ADC values in a loop:
        - If **channel 5 > 1000**, turns ON the **Red LED**, otherwise turns it OFF.
        - If **channel 13 > 1000**, turns ON the **Yellow LED**, otherwise turns it OFF.
    - Blue LED is initialized but not used—can be reserved for future logic.
*/

/* Include necessary hardware abstraction layer (HAL) headers */
#include "../MCAL/RCC/RCC_Interface.h"         // Provides functions to initialize and enable system clocks
#include "../MCAL/ADC/ADC_Interface.h"         // Provides functions to configure and read from the ADC
#include "../MCAL/GPIO/GPIO_Interface.h"       // Provides functions for GPIO pin configuration
#include "../MCAL/SYSTICK/SYSTICK_Interface.h" // (Not used here) Provides delay/periodic interrupt using SysTick
#include "../ECUAL/LED/LED_Interface.h"        // Provides LED control abstraction (initialization, ON/OFF)

/* Main application entry point */
int main(void)
{
    /* Initialize the system clock */
    RCC_init();

    /* Enable clocks for GPIO ports A, B, C and the ADC1 peripheral */
    RCC_Peri_Enable(GPIOA_EN);
    RCC_Peri_Enable(GPIOB_EN);
    RCC_Peri_Enable(GPIOC_EN);
    RCC_Peri_Enable(ADC1EN);

    /*
     * Initialize ADC1 in continuous conversion mode with 2 channels in the sequence.
     * Continuous mode allows the ADC to convert continuously without restarting.
     */
    ADC_init(ADC1, ADC_MODE_CONTINUOUS_CONV, 2);

    /* Define LED objects with their corresponding GPIO ports and pins */
    LED_t red    = {GPIOA, 1};  // Red LED on pin PA1
    LED_t blue   = {GPIOB, 2};  // Blue LED on pin PB2 (not used in this logic)
    LED_t yellow = {GPIOC, 2};  // Yellow LED on pin PC2

    /* Initialize the LEDs (configure the pins as output, etc.) */
    LED_init(&red);
    LED_init(&blue);
    LED_init(&yellow);

    /* Configure ADC input pins as analog mode */
    GPIO_Mode(GPIOA, GPIO_PIN_5, GPIO_ANALOG);  // PA5 = ADC Channel 5
    GPIO_Mode(GPIOB, GPIO_PIN_1, GPIO_ANALOG);  // PB1 = ADC Channel 9 (unused)
    GPIO_Mode(GPIOC, GPIO_PIN_3, GPIO_ANALOG);  // PC3 = ADC Channel 13

    /* Disable internal pull-up/pull-down resistors for analog pins */
    GPIO_Pull_Up_Pull_Down(GPIOA, GPIO_PIN_5, GPIO_NO_PULL_UP_NO_PULL_DOWN);
    GPIO_Pull_Up_Pull_Down(GPIOB, GPIO_PIN_1, GPIO_NO_PULL_UP_NO_PULL_DOWN);
    GPIO_Pull_Up_Pull_Down(GPIOC, GPIO_PIN_3, GPIO_NO_PULL_UP_NO_PULL_DOWN);

    /*
     * Configure ADC channels to be part of the conversion sequence:
     * - Channel 5 (PA5) as the first channel
     * - Channel 13 (PC3) as the second channel
     * Channel 9 (PB1) is not enabled or used in this example.
     */
    ADC_enableChannel(ADC1, 5, 1);   // First in sequence
    ADC_enableChannel(ADC1, 13, 2);  // Second in sequence

    /* Start ADC conversions */
    ADC_startConversion(ADC1);

    /* Buffer to store ADC conversion results */
    uint16_t adc_values[2];

    /* Main loop: monitor ADC readings and control LEDs */
    while (1)
    {
        /*
         * Read 2 ADC conversion results from the sequence:
         * - adc_values[0] corresponds to channel 5
         * - adc_values[1] corresponds to channel 13
         */
        ADC_readSequence(ADC1, adc_values, 2);
        /* Control red LED based on channel 5 value */
        (adc_values[0] > 1000) ? LED_turnOn(&red) : LED_turnOff(&red);
        /* Control yellow LED based on channel 13 value */
        (adc_values[1] > 1000) ? LED_turnOn(&yellow) : LED_turnOff(&yellow);
    }
}
