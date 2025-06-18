/*
 * TIMERS_Program.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Ahmed
 */

#include "TIMER_Interface.h"

void TIMER_init(TIMER_RegDef_t *Timer)
{
    Timer->CR1 |= (1 << 7); // ARPE = 1: Auto-reload preload enable

    /*
     * Timer Clock Prescaler (PSC):
     * The prescaler divides the timer input clock (TIM_CLK) to slow down the counter frequency.
     *
     * Formula:
     *     F_timer = TIM_CLK / (PSC + 1)
     *
     * Where:
     *     F_timer = Frequency of the timer counter (ticks per second)
     *     TIM_CLK = Timer input clock (in Hz), typically 16 MHz (internal clock)
     *     PSC = Value loaded into the Prescaler register (TIMx_PSC)
     *
     * Example:
     *     TIM_CLK = 16,000,000 Hz
     *     PSC = 15999
     *     F_timer = 16,000,000 / (15999 + 1) = 1000 Hz → 1 ms per tick
     *
     * This means the counter increments once every 1 millisecond, so setting ARR = (ms - 1)
     * will generate accurate millisecond delays.
     */
    Timer->PSC = TIMER_CLOCK_PRESCALER; // Set prescaler
}

void TIMER_voidDelayMilliSec(TIMER_RegDef_t *Timer, uint32_t ms)
{

    Timer->ARR = ms - 1; 		   // Assign value to auto-reload register. Load auto-reload register for desired delay
    Timer->EGR |= (1 << 0);        // UG = 1: Update generation (load ARR and PSC)
    Timer->SR &= ~(1 << 0);        // Clear UIF
    Timer->CR1 |= (1 << 0);        // Enable counter

    while (! ((Timer->SR >> 0) & 1) );  // Wait for UIF (update event flag)
    Timer->CR1 &= ~(1 << 0);       // Disable counter
}

/***************************************************************************
 *                     Timer Interrupt-Based Delay Mode                    *
 **************************************************************************/

/* Global function pointer for timer callback */
void(*Timer_Call_Function[6])(void); // Index 0–5

/**
 * @brief Enable timer update interrupt.
 */
void TIMER_enableInterrupt(TIMER_RegDef_t *Timer)
{
    Timer->DIER |= (1 << 0);  // UIE = 1: Enable update interrupt
}

/**
 * @brief Disable timer interrupt and stop counter.
 */
void TIMER_disableInterrupt_and_Counter(TIMER_RegDef_t *Timer)
{
    Timer->DIER &= ~(1 << 0);  // UIE = 0: Disable update interrupt
    Timer->CR1  &= ~(1 << 0);  // CEN = 0: Disable counter
}

/**
 * @brief Start timer in interrupt mode and assign callback.
 *
 * @param Timer Pointer to timer (e.g., TIM2)
 * @param ms Delay in milliseconds
 * @param ptr Callback function to be executed on interrupt
 */
void TIMER_voidStartCountMilliSeconds(TIMER_RegDef_t *Timer, uint32_t ms, void(*ptr)(void), uint8_t Timer_Number)
{
	Timer_Call_Function[Timer_Number] = ptr;
    Timer->ARR = ms - 1;             // Load ARR with delay value
    Timer->EGR |= (1 << 0);          // Force update event
    Timer->SR &= ~(1 << 0);          // Clear UIF
    Timer->CR1 |= (1 << 0);          // Start counter
}

/**
 * @brief Timer interrupt handler for TIM2.
 *
 * Calls the registered user callback when UIF is set.
 */
void TIM2_IRQHandler(void)
{
    if((TIM2->SR >> 0) & 1)  // Check if UIF is set
        TIM2->SR &= ~(1 << 0);     // Clear UIF flag
    Timer_Call_Function[2]();
}

void TIM3_IRQHandler(void)
{
    if((TIM3->SR >> 0) & 1)  // Check if UIF is set
        TIM3->SR &= ~(1 << 0);     // Clear UIF flag
    Timer_Call_Function[3]();
}

void TIM4_IRQHandler(void)
{
    if((TIM4->SR >> 0) & 1)  // Check if UIF is set
        TIM4->SR &= ~(1 << 0);     // Clear UIF flag
    Timer_Call_Function[4]();
}

void TIM5_IRQHandler(void)
{
    if((TIM5->SR >> 0) & 1)  // Check if UIF is set
        TIM5->SR &= ~(1 << 0);     // Clear UIF flag
    Timer_Call_Function[5]();
}

/*
 * Timer Clock System (Prescaler, Counter, Auto-Reload):
 *
 * ┌────────────┐         ┌────────────┐         ┌────────────┐         ┌─────────────┐
 * │ TIM_CLK    │ ─────>  │ Prescaler  │ ─────>  │ Counter    │ ─────>  │ Auto-Reload │
 * └────────────┘         └────────────┘         └────────────┘         └─────────────┘
 *   (e.g. 16 MHz)         ÷ (PSC + 1)              CNT ticks              Resets CNT when ARR reached
 *
 * ➤ TIM_CLK: Timer input clock source (typically 16 MHz from internal clock).
 * ➤ PSC (Prescaler): Divides the clock to slow down the tick frequency.
 *
 *      Formula:
 *          F_timer = TIM_CLK / (PSC + 1)
 *
 *      → This is the frequency at which the counter increments.
 *
 * ➤ CNT (Counter): Increments at F_timer rate. It counts up (or down) from 0.
 *
 * ➤ ARR (Auto-Reload Register):
 *     - When CNT reaches ARR, an **update event (UEV)** is triggered.
 *     - CNT is then **reloaded** (reset) to 0 (or ARR in downcounting mode) and starts over.
 *     - This creates a repeating timer cycle.
 *
 * ➤ Reload Behavior:
 *     - ARR is a **buffered register** if ARPE (Auto-Reload Preload Enable) is set.
 *     - When you write to ARR while ARPE = 1, the new value is stored in a **shadow register**.
 *     - The update takes effect only after an **update event (UEV)**.
 *     - To force a reload of ARR and PSC values immediately, set the **UG bit** in the EGR register.
 *
 * ➤ Example Setup for 1 ms Delay:
 *     TIM_CLK = 16,000,000 Hz
 *     PSC = 15999
 *     → F_timer = 16,000,000 / (15999 + 1) = 1000 Hz → 1 ms per tick
 *
 *     To generate a delay of 'ms' milliseconds:
 *     → ARR = ms - 1
 *     → Timer generates an update event when CNT reaches ARR
 */
