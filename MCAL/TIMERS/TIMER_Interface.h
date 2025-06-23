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

#define PWM_MODE_1		0b110
#define PWM_MODE_2		0b111

#define PWM_MODE		PWM_MODE_1


#define ACTIVE_HIGH		0
#define ACTIVE_LOW		1

#define POLARITY			ACTIVE_HIGH

/**
 * @brief Initialize the timer with the configured prescaler.
 *
 * - Enables ARPE (Auto-Reload Preload Enable) to buffer ARR.
 * - Sets the PSC (Prescaler) to divide the timer clock.
 * - Prepares timer for delay or periodic tasks.
 *
 * @param Timer Pointer to the timer peripheral registers (e.g., TIM2, TIM3).
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

/**
 * @brief Enable update interrupt for timer.
 *
 * - Sets UIE bit in DIER register.
 * - Timer will trigger interrupt on overflow (CNT == ARR).
 *
 * @param Timer Pointer to the timer peripheral.
 */
void TIMER_enableInterrupt(TIMER_RegDef_t *Timer);

/**
 * @brief Disable timer update interrupt and stop counter.
 *
 * - Clears UIE (disable update interrupt).
 * - Clears CEN (disable counting).
 *
 * @param Timer Pointer to the timer peripheral.
 */
void TIMER_disableInterrupt_and_Counter(TIMER_RegDef_t *Timer);

/**
 * @brief Start timer with interrupt after given delay and assign ISR callback.
 *
 * @param Timer Pointer to timer (e.g., TIM2).
 * @param ms Delay in milliseconds.
 * @param ptr Pointer to callback function to be called on interrupt.
 * @param Timer_Number Timer index to store in function pointer array (e.g., TIM2 → 2).
 */
void TIMER_voidStartCountMilliSeconds(TIMER_RegDef_t *Timer, uint32_t ms, void(*ptr)(void), uint8_t Timer_Number);

void TIMER_generatePWM(TIMER_RegDef_t *Timer, uint8_t channel, uint32_t periode, uint32_t duty_cycle);

#endif /* TIMERS_TIMERS_INTERFACE_H_ */
