/*
  	This C code is designed to blink an LED connected to pin PA0 of the microcontroller using Timer 2 in polling mode (with blocking delays).
  	Here's a concise explanation of the core idea and how it works:

	🧠 Core Idea:
	- The program configures GPIO pin PA0 as an output to control an LED.
	- Timer 2 is initialized and used to create blocking delays in milliseconds.
	- Inside the main loop, the LED is turned ON, a 1000 ms (1 second) delay is applied, then the LED
*/

#include "../MCAL/RCC/RCC_Interface.h"
#include "../MCAL/TIMERS/TIMER_Interface.h"
#include "../ECUAL/LED/LED_Interface.h"

int main(void)
{
    /* Enable clocks */
    RCC_init();
    RCC_Peri_Enable(GPIOA_EN);   // Enable GPIOA for LED
    RCC_Peri_Enable(TIM2EN);     // Enable TIM2 before using it

    /* Initialize peripherals */
    TIMER_init(TIM2);            // Initialize TIM2 with configured prescaler
    LED_t red = {GPIOA, 0};      // LED connected to PA0
    LED_init(&red);              // Initialize LED pin

    while (1)
    {
        LED_turnOn(&red);
        TIMER_voidDelayMilliSec(TIM2, 1000); // 1 second delay
        LED_turnOff(&red);
        TIMER_voidDelayMilliSec(TIM2, 1000); // 1 second delay
    }
}

/*
  * Enable peripheral clocks before using them.
  * You should enable the TIM2 clock via RCC_Peri_Enable(TIM2EN) before calling TIMER_init(TIM2), otherwise the peripheral will not be accessible.
*/
