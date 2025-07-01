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

/* PWM Modes */
#define PWM_MODE_1       0b110  // Output active while counter < CCR
#define PWM_MODE_2       0b111  // Output inactive while counter < CCR

#define PWM_MODE         PWM_MODE_1


/* PWM Polarity */
#define ACTIVE_HIGH      0  // Output high during active time
#define ACTIVE_LOW       1  // Output low during active time

#define POLARITY         ACTIVE_HIGH


/* Timer Prescalers */
/*
 * This driver runs from the internal 16 MHz clock.
 * TIMER_CLOCK_PRESCALER_DELAY divides the clock by 16000 (15999 + 1), resulting in a 1 kHz timer tick (1 ms per tick), ideal for millisecond delays.
 *
 * TIMER_CLOCK_PRESCALER_PWM divides the clock by 16 (15 + 1), producing a 1 MHz timer tick (1 µs per tick) for precise PWM timing.
 *
 * TIMER_CLOCK_PRESCALER is set to TIMER_CLOCK_PRESCALER_PWM by default, to enable high-resolution PWM control.
 */
#define TIMER_CLOCK_PRESCALER_DELAY     15999  // 1 ms tick (16 MHz clock)
#define TIMER_CLOCK_PRESCALER_PWM       15     // 1 µs tick (16 MHz clock)

#define  TIMER_CLOCK_PRESCALER			TIMER_CLOCK_PRESCALER_PWM

/* ============================= */
/*        FUNCTION PROTOTYPES   */
/* ============================= */

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

/**
 * @brief Generates a PWM signal on a specified timer channel.
 *
 * - Configures the appropriate CCMRx, CCER, and ARR/CCR registers.
 * - Sets the PWM mode (default: PWM mode 1).
 * - Configures polarity (default: active high).
 * - Enables output compare and counter.
 *
 * @param Timer Pointer to the timer peripheral (e.g., TIM2, TIM3).
 * @param channel Timer channel number (1 to 4).
 * @param periode Total period in microseconds (ARR value).
 * @param duty_cycle Active time in microseconds (CCR value).
 */
void TIMER_generatePWM(TIMER_RegDef_t *Timer, uint8_t channel, uint32_t periode, uint32_t duty_cycle);


#endif /* TIMERS_TIMERS_INTERFACE_H_ */
