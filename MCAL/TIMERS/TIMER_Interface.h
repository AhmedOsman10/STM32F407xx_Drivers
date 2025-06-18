/*
 * TIMERS_Interface.h
 *
 *  Created on: Jun 12, 2025
 *      Author: Ahmed
 */

#ifndef TIMERS_TIMERS_INTERFACE_H_
#define TIMERS_TIMERS_INTERFACE_H_

#include "TIMER_Registers.h"
#include "TIMER_Config.h"

/**
 * @brief Initialize the timer with the configured prescaler.
 * @param Timer Pointer to the timer peripheral registers.
 */
void TIMER_init(TIMER_RegDef_t *Timer);

/**
 * @brief Blocking delay function in milliseconds.
 *
 * @note The maximum delay depends on the timer's bit width and prescaler setting.
 *       With a 16 MHz clock and prescaler = 15999, the timer tick period is 1 ms.
 *
 *       Timer bit widths:
 *         - TIM2: 32-bit timer
 *         - TIM3: 16-bit timer
 *         - TIM4: 16-bit timer
 *         - TIM5: 32-bit timer
 *
 *       Maximum delay based on timer size:
 *         - 32-bit timer: ≈ 49.7 days max delay
 *         - 16-bit timer: ≈ 65.5 seconds max delay
 *
 * @param Timer Pointer to the timer peripheral registers.
 * @param ms Delay time in milliseconds.
 */
void TIMER_voidDelayMilliSec(TIMER_RegDef_t *Timer, uint32_t ms);

void TIMER_enableInterrupt(TIMER_RegDef_t *Timer);
void TIMER_disableInterrupt_and_Counter(TIMER_RegDef_t *Timer);
void TIMER_voidStartCountMilliSeconds(TIMER_RegDef_t *Timer, uint32_t ms, void(*ptr)(void), uint8_t Timer_Number);

#endif /* TIMERS_TIMERS_INTERFACE_H_ */
