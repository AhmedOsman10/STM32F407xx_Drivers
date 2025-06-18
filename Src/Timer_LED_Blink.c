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
        TIMER_voidDelayMilliSec(TIM2, 5000); // 1 second delay
        LED_turnOff(&red);
        TIMER_voidDelayMilliSec(TIM2, 5000); // 1 second delay
    }
}

/*
  * Enable peripheral clocks before using them.
  * You should enable the TIM2 clock via RCC_Peri_Enable(TIM2EN) before calling TIMER_init(TIM2), otherwise the peripheral will not be accessible.
*/
