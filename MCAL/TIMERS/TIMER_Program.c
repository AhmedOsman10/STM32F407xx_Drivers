/*
 * TIMERS_Program.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Ahmed
 */

#include "TIMER_Interface.h"

void TIMER_init(TIMER_RegDef_t *Timer)
{
	Timer->CR1 = ( 1 << 0);
	Timer->CR1 = ( 1 << 7);
}
