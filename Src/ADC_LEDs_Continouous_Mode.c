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

#include "../MCAL/RCC/RCC_Interface.h"         // Clock configuration
#include "../MCAL/ADC/ADC_Interface.h"         // ADC setup and usage
#include "../MCAL/GPIO/GPIO_Interface.h"       // GPIO control
#include "../MCAL/SYSTICK/SYSTICK_Interface.h" // (Unused)
#include "../ECUAL/LED/LED_Interface.h"        // LED abstraction

int main(void)
{
    /* Initialize the system clock */
    RCC_init();

    /* Enable clocks for GPIO ports A, B, C and ADC1 */
    RCC_Peri_Enable(GPIOA_EN);
    RCC_Peri_Enable(GPIOB_EN);
    RCC_Peri_Enable(GPIOC_EN);
    RCC_Peri_Enable(ADC1EN);

    /* Initialize ADC1 in continuous conversion mode with 2 channels */
    ADC_init(ADC1, ADC_MODE_CONTINUOUS_CONV, 2);

    /* Define LEDs */
    LED_t red    = {GPIOA, 1};  // PA1
    LED_t blue   = {GPIOB, 2};  // PB2
    LED_t yellow = {GPIOC, 2};  // PC2

    /* Initialize LED pins */
    LED_init(&red);
    LED_init(&blue);
    LED_init(&yellow);

    /* ✅ Configure analog input pins using the new GPIO function */
    GPIO_configureAnalogInput(GPIOA, GPIO_PIN_5);  // PA5 → ADC Channel 5
    GPIO_configureAnalogInput(GPIOB, GPIO_PIN_1);  // PB1 → ADC Channel 9 (unused)
    GPIO_configureAnalogInput(GPIOC, GPIO_PIN_3);  // PC3 → ADC Channel 13

    /* Configure ADC channels in conversion sequence */
    ADC_enableChannel(ADC1, 5, 1);   // Channel 5 first
    ADC_enableChannel(ADC1, 13, 2);  // Channel 13 second

    /* Start ADC conversion */
    ADC_startConversion(ADC1);

    uint16_t adc_values[2];  // Buffer for ADC values

    /* Main loop */
    while (1)
    {
        ADC_readSequence(ADC1, adc_values, 2);

        (adc_values[0] > 1000) ? LED_turnOn(&red) : LED_turnOff(&red);
        (adc_values[1] > 1000) ? LED_turnOn(&yellow) : LED_turnOff(&yellow);
    }
}
