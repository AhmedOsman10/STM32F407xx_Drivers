/*
 * SYSTICK_Interface.h
 *
 *  Created on: Mar 3, 2025
 *      Author: Ahmed
 */

#ifndef SYSTICK_SYSTICK_INTERFACE_H_
#define SYSTICK_SYSTICK_INTERFACE_H_

#include "../LIB/STD_TYPES.h"
#include "SYSTICK_Registers.h"
#include "SYSTICK_Config.h"

/**
 * @brief Initializes the SysTick timer by selecting the clock source.
 *
 * Clock source is determined by the configuration macro (AHB or AHB/8).
 */
void SYSTICK_init(void);

/**
 * @brief Creates a blocking delay in milliseconds.
 *
 * @param ms: Number of milliseconds to delay.
 *
 * Uses polling (busy-wait) to block code execution.
 */
void SYSTICK_voidDelayMilliSec(u32 ms);

/**
 * @brief Creates a blocking delay in microseconds.
 *
 * @param us: Number of microseconds to delay (max 1,000,000).
 *
 * Note: SysTick has a 24-bit counter, so at 16 MHz max = 1 second.
 * Each microsecond = 16 clock cycles (if using 16 MHz system clock).
 */
void SYSTICK_voidDelayMicroSec(u32 us);

/**
 * @brief Returns the number of elapsed ticks since the timer started.
 *
 * @return Number of ticks elapsed.
 */
u32 SYSTICK_u32GetElapsedTime(void);

/**
 * @brief Returns the remaining ticks before the timer reaches zero.
 *
 * @return Number of ticks remaining.
 */
u32 SYSTICK_u32GetRemainingTime(void);

/********** Interrupt-Based Functionality **********/

/**
 * @brief Enables the SysTick interrupt.
 *
 * When enabled, the SysTick_Handler will be triggered on underflow.
 */
void SYSTICK_EnableInterrupt(void);

/**
 * @brief Disables the SysTick interrupt and stops the counter.
 */
void SYSTICK_DisableInterrupt_and_Counter(void);

/**
 * @brief Starts SysTick with a given delay (in ms) and a callback function.
 *
 * @param ms: Time in milliseconds.
 * @param ptr: Function to call when the timer expires.
 */
void SYSTICK_voidStartCountMilliSeconds(u32 ms, void(*ptr)(void));

/**
 * @brief Starts SysTick with a given delay (in µs) and a callback function.
 *
 * @param us: Time in microseconds (up to 1,000,000 µs).
 * @param ptr: Function to call when the timer expires.
 */
void SYSTICK_voidStartCountMicroSeconds(u32 us, void(*ptr)(void));

#endif /* SYSTICK_SYSTICK_INTERFACE_H_ */
