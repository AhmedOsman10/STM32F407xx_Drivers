/*
 * LED_Program.c
 *
 *  Created on: May 21, 2025
 *      Author: Ahmed
 */

#include "LED_Interface.h"

void LED_init(LED_t *led)
{
	GPIO_Mode(led->port, led->pinNumber, GPIO_GENERAL_OUTPUT_PURPOSE);
	GPIO_Output_Type(led->port, led->pinNumber, GPIO_PUSH_PULL);
	GPIO_Speed(led->port, led->pinNumber, GPIO_LOW_SPEED);
	GPIO_Pull_Up_Pull_Down(led->port, led->pinNumber, GPIO_NO_PULL_UP_NO_PULL_DOWN);
}
void LED_turnOn(LED_t *led)
{
	GPIO_Output_Data(led->port, led->pinNumber, GPIO_SET);
}
void LED_turnOff(LED_t *led)
{
	GPIO_Output_Data(led->port, led->pinNumber, GPIO_RESET);
}
void LED_blink(LED_t *led, uint32_t milliseconds)
{
	GPIO_Output_Data(led->port, led->pinNumber, GPIO_SET);
	SYSTICK_voidDelayMilliSec(milliseconds);
	GPIO_Output_Data(led->port, led->pinNumber, GPIO_RESET);
	SYSTICK_voidDelayMilliSec(milliseconds);
}
