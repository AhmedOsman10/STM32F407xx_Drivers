/*
    This C code is designed to generate a PWM signal using TIM2 on an STM32 microcontroller and output it on GPIO pin PA1.

    🧠 Core Idea:
    - Initializes the system clock and enables clocks for GPIOA and TIM2 peripherals.
    - Configures **PA1** as an alternate function output pin to be used for PWM:
        - PA1 is mapped to **TIM2 Channel 2** via **Alternate Function 1 (AF1)**.
    - Initializes TIM2 with a default prescaler to start generating time-based events.
    - Uses the **TIMER_generatePWM()** function to configure:
        - PWM mode (PWM Mode 1)
        - Frequency = 1 kHz → Period = 1000 µs
        - Duty cycle = 50% → High for 500 µs, Low for 500 µs
    - PWM runs continuously on hardware without CPU intervention.

    🧩 Peripheral Pin Mapping:
    - TIM2_CH2 → PA1 (AF1)

    🔁 In the infinite loop:
    - The program does nothing in software; PWM generation continues via hardware timers.
    - You can connect an oscilloscope or an LED (with a current-limiting resistor) to PA1 to observe the PWM output.
*/

#include "../MCAL/RCC/RCC_Interface.h"
#include "../MCAL/TIMERS/TIMER_Interface.h"
#include "../MCAL/GPIO/GPIO_Interface.h"
#include "../ECUAL/LED/LED_Interface.h"

int main(void)
{
    /* ----------------------------- */
    /* Step 1: Enable peripheral clocks */
    /* ----------------------------- */
    RCC_init();                       // Initialize system clock
    RCC_Peri_Enable(GPIOA_EN);       // Enable clock for GPIOA (needed for PA1)
    RCC_Peri_Enable(TIM2EN);         // Enable clock for TIM2 (used for PWM)

    /* ---------------------------------------- */
    /* Step 2: Configure GPIOA Pin 1 for PWM use */
    /* ---------------------------------------- */
    // TIM2_CH2 is mapped to PA1 → alternate function AF1
    GPIO_configurePWM(GPIOA, 1, GPIO_AF1);  // Set PA1 to alternate function mode for PWM output

    /* ---------------------------------- */
    /* Step 3: Initialize TIM2 and set PWM */
    /* ---------------------------------- */
    TIMER_init(TIM2);                          // Initialize TIM2 with default prescaler settings
    TIMER_generatePWM(TIM2, 2, 1000, 500);     // Generate PWM on channel 2 (PA1) with 50% duty cycle and 1 kHz frequency

    /* ----------------- */
    /* Step 4: Main loop */
    /* ----------------- */
    while (1)
    {
        // Infinite loop – PWM runs in hardware
    }
}
