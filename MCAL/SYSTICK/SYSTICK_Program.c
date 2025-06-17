/*
 * SYSTICK_Program.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Ahmed
 */

#include "SYSTICK_Interface.h"

/**
 * @brief Initialize the SysTick timer clock source.
 *
 * Selects the clock source for the SysTick timer based on compile-time config.
 * Options:
 *   - AHB (full speed system clock)
 *   - AHB/8 (slower clock)
 */
void SYSTICK_init(void)
{
	#if (SYSTICK_CLOCK_SOURCE == SYSTICK_AHB_DIV1)
		SYSTICK->CTRL |= (1 << 2);   // Use processor clock (AHB)
	#else
		SYSTICK->CTRL &= ~(1 << 2);  // Use AHB/8
	#endif
}

/**
 * @brief Create a blocking delay in milliseconds.
 *
 * @param ms: Number of milliseconds to wait.
 *
 * Uses polling mode with the COUNTFLAG to wait for each millisecond interval.
 * Can handle delays longer than 1 second by breaking into 1000 ms chunks.
 */
void SYSTICK_voidDelayMilliSec(u32 ms)
{
	while(ms > 0)
	{
		if(ms > 1000)
		{
			SYSTICK->LOAD = (CLOCK_FREQUENCY * 1000000) - 1;  // 1 second
			ms -= 1000;
		}
		else
		{
			SYSTICK->LOAD = (CLOCK_FREQUENCY * ms * 1000) - 1;
			ms = 0;
		}

		SYSTICK->CTRL |= (1 << 0);  // Start counter

		while(!((SYSTICK->CTRL >> 16) & 1));  // Wait for COUNTFLAG

		SYSTICK->CTRL &= ~(1 << 0);  // Stop counter
	}
}

/**
 * @brief Create a blocking delay in microseconds.
 *
 * @param us: Number of microseconds to wait.
 *
 * Suitable for precise timing operations (e.g., sensor communication).
 * Breaks long delays (>1 second) into smaller chunks.
 */
void SYSTICK_voidDelayMicroSec(u32 us)
{
	while(us > 0)
	{
		if(us > 1000000)
		{
			SYSTICK->LOAD = (CLOCK_FREQUENCY * 1000000) - 1;
			us -= 1000000;
		}
		else
		{
			SYSTICK->LOAD = (CLOCK_FREQUENCY * us) - 1;
			us = 0;
		}

		SYSTICK->CTRL |= (1 << 0);  // Start counter

		while(!((SYSTICK->CTRL >> 16) & 1));  // Wait for COUNTFLAG

		SYSTICK->CTRL &= ~(1 << 0);  // Stop counter
	}
}

/**
 * @brief Get the elapsed time (in ticks) since the counter started.
 *
 * @return Number of ticks since the timer started.
 */
u32 SYSTICK_u32GetElapsedTime(void)
{
	return (SYSTICK->LOAD - SYSTICK->VAL);
}

/**
 * @brief Get the remaining time (in ticks) before timer underflows.
 *
 * @return Current value of the counter.
 */
u32 SYSTICK_u32GetRemainingTime(void)
{
	return SYSTICK->VAL;
}

/*********************** Interrupt-Based Mode **************************/

/* Global function pointer to store callback for SysTick interrupt */
void (*Call_Function)(void);

/**
 * @brief Enable SysTick interrupt.
 *
 * Allows the SysTick timer to generate an interrupt when it counts down to 0.
 */
void SYSTICK_EnableInterrupt(void)
{
	SYSTICK->CTRL |= (1 << 1);  // Enable interrupt (TICKINT)
}

/**
 * @brief Disable both SysTick interrupt and the counter.
 */
void SYSTICK_DisableInterrupt_and_Counter(void)
{
	SYSTICK->CTRL &= ~(1 << 1);  // Disable interrupt
	SYSTICK->CTRL &= ~(1 << 0);  // Stop counter
}

/**
 * @brief Start SysTick with a user-defined callback in milliseconds.
 *
 * @param ms: Time in milliseconds before interrupt fires.
 * @param ptr: Pointer to callback function to execute on interrupt.
 */
void SYSTICK_voidStartCountMilliSeconds(u32 ms, void(*ptr)(void))
{
	Call_Function = ptr;
	SYSTICK->LOAD =  (CLOCK_FREQUENCY * 1000 * ms) - 1;
	SYSTICK->CTRL |= (1 << 0);  // Start timer
}

/**
 * @brief Start SysTick with a user-defined callback in microseconds.
 *
 * @param us: Time in microseconds before interrupt fires.
 * @param ptr: Pointer to callback function to execute on interrupt.
 */
void SYSTICK_voidStartCountMicroSeconds(u32 us, void(*ptr)(void))
{
	Call_Function = ptr;
	SYSTICK->LOAD =  (CLOCK_FREQUENCY * us) - 1;
	SYSTICK->CTRL |= (1 << 0);  // Start timer
}

/**
 * @brief SysTick interrupt handler.
 *
 * Calls the user-defined callback function when timer reaches 0.
 * In periodic mode, the counter is disabled after one shot.
 */
void SysTick_Handler(void)
{
	Call_Function();

	#if SYSTICK_INTERPUT == SYSTICK_PERIODIC
		SYSTICK->CTRL &= ~(1 << 0);  // Stop after one interval if configured
	#endif
}

/*
 * -----------------------------------------------------------------------------
 * Concept and Functionality:
 *
 * The SysTick timer is a 24-bit down-counter integrated into the ARM Cortex-M core.
 * It’s mainly used for timekeeping (delays, scheduling, interrupts).
 *
 * -----------------------------------------------------------------------------
 * Function Summary:
 *
 * SYSTICK_init():
 *   - Chooses the clock source (AHB or AHB/8) for SysTick timer.
 *
 * SYSTICK_voidDelayMilliSec(), SYSTICK_voidDelayMicroSec():
 *   - Blocking delay functions using polling.
 *   - Suitable for short and accurate timing (e.g., LED blink, sensor hold).
 *   - Breaks long durations into smaller chunks.
 *
 * SYSTICK_u32GetElapsedTime(), SYSTICK_u32GetRemainingTime():
 *   - Help measure time passed or time left based on current timer values.
 *
 * SYSTICK_EnableInterrupt(), SYSTICK_DisableInterrupt_and_Counter():
 *   - Enable or disable SysTick interrupt and counter operation.
 *
 * SYSTICK_voidStartCountMilliSeconds(), SYSTICK_voidStartCountMicroSeconds():
 *   - Start timer for a user-defined duration with a callback function.
 *   - Useful for periodic tasks (e.g., polling sensors, triggering events).
 *
 * SysTick_Handler():
 *   - Executes the user-defined callback.
 *   - Can optionally stop the timer if in one-shot mode.
 *
 * -----------------------------------------------------------------------------
 * Example Use Case:
 *
 * // Blocking delay
 * SYSTICK_voidDelayMilliSec(100);  // Wait for 100 ms
 *
 * // Non-blocking delay with interrupt
 * SYSTICK_voidStartCountMilliSeconds(10, MyCallback);
 * SYSTICK_EnableInterrupt();
 *
 * -----------------------------------------------------------------------------
 */
