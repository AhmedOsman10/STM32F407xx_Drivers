#include "../MCAL/RCC/RCC_Interface.h"
#include "../MCAL/TIMERS/TIMER_Interface.h"
#include "../MCAL/NVIC/NVIC_Interface.h"
#include "../ECUAL/LED/LED_Interface.h"

LED_t red = {GPIOA, 0};
int counter = 0;

void func(void)
{
	/* Turn on the LED */
	if(counter == 0)
	{
		LED_turnOn(&red);
		counter++;
	}
	/* Turn off the LED */
	else if(counter == 1)
	{
		LED_turnOff(&red);
		counter++;
	}

	else if(counter == 2)
	{
		TIMER_disableInterrupt_and_Counter(TIM2);
	}
}

int main(void)
{
	RCC_init();
	RCC_Peri_Enable(GPIOA_EN);
	RCC_Peri_Enable(TIM2EN);
	TIMER_init(TIM2);
	LED_init(&red);
	NVIC_EnableIRQn(NVIC_IRQ_TIM2);
	TIMER_enableInterrupt(TIM2);
	TIMER_voidStartCountMilliSeconds(TIM2, 1000, func, 2);

	while(1);
}
