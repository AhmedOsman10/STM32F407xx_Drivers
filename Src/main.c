
#include "../MCAL/RCC/RCC_Interface.h"
#include "../MCAL/TIMERS/TIMER_Interface.h"
#include "../MCAL/GPIO/GPIO_Interface.h"
#include "../ECUAL/LED/LED_Interface.h"

int main(void)
{
    /* Enable clocks */
    RCC_init();
    RCC_Peri_Enable(GPIOA_EN);
    RCC_Peri_Enable(TIM2EN);

    GPIO_Mode(GPIOA, 1, GPIO_ALTERNATE_FUNCTION);
    GPIO_Speed(GPIOA, 1, GPIO_HIGH_SPEED);
    GPIO_Output_Type(GPIOA, 1, GPIO_PUSH_PULL);
    GPIO_Pull_Up_Pull_Down(GPIOA, 1, GPIO_NO_PULL_UP_NO_PULL_DOWN);
    GPIO_Alternate_Function(GPIOA, 1, GPIO_AF1);

    TIMER_init(TIM2);
    TIMER_generatePWM(TIM2, 2, 1000, 700);
    while (1)
    {
    }
}

