/*
 * TIMERS_Program.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Ahmed
 */

#include "TIMER_Interface.h"

void TIMER_init(TIMER_RegDef_t *Timer)
{
    // ARPE = 1: Auto-reload preload enable. This causes ARR writes to go to shadow register.
    Timer->CR1 |= (1 << 7);

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
    Timer->PSC = TIMER_CLOCK_PRESCALER;
}

void TIMER_voidDelayMilliSec(TIMER_RegDef_t *Timer, uint32_t ms)
{
    // 1. Set Auto-Reload Register (ARR):
    //    - If ARPE = 0 → written directly to ARR.
    //    - If ARPE = 1 → written to shadow register.
    Timer->ARR = ms - 1;

    // 2. Force Update Event (UEV) by setting UG = 1:
    //    ➤ WHY?
    //    - When ARPE = 1, ARR and PSC are written to shadow registers.
    //    - Without a UEV, those values won't be loaded into the active logic until the next counter overflow.
    //    - UG = 1 forces an update event **immediately**, so:
    //        → PSC and ARR values from shadow registers are loaded into the actual counter logic.
    //        → Ensures accurate delay without waiting for overflow.
    Timer->EGR |= (1 << 0);  // UG = 1

    // 3. Clear UIF (Update Interrupt Flag) to prevent old overflow from affecting this delay.
    //    - UIF may still be set from a previous timer session.
    //    - Also, forcing a UEV manually by setting UG can set UIF.
    //    - If UIF is not cleared here, the polling loop might exit immediately, skipping the delay.
    Timer->SR &= ~(1 << 0);  // Clear UIF

    // 4. Start the Timer Counter (CNT starts counting from 0).
    Timer->CR1 |= (1 << 0);  // CEN = 1: Enable counter

    // 5. Poll for Update Event:
    //    - CNT counts up at the rate defined by PSC.
    //    - When CNT == ARR → hardware triggers UEV internally.
    //    - But UEV is *not directly readable by software*.
    //    - Instead, UIF (Update Interrupt Flag) is set when UEV occurs. So we check UIF to detect that overflow (update) has occurred.
    //
    // 🔴 Why not check for UEV directly?
    //     → UEV is an internal event. It's not stored in any readable flag.
    //     → UIF is the only flag software can check that reflects a UEV.
    while (!(Timer->SR & (1 << 0)));  // Wait until UIF is set (i.e., UEV occurred)

    // 6. Clear UIF again to reset status.
    //    ⚠️ This is important! If you don’t clear UIF here:
    //       - Next time you call this function, the previous UIF may still be set.
    //       - That would make the next `while()` check exit immediately without waiting → incorrect delay.
    Timer->SR &= ~(1 << 0);  // Clear UIF again

    Timer->CR1 &= ~(1 << 0); // Stop the counter. Though I believe it wouldn't cause issues if left running (Not stopping the counter).
}

/***************************************************************************
 *                     Timer Interrupt-Based Delay Mode                    *
 **************************************************************************/

// Function pointers for timer callbacks: index 2 = TIM2, 3 = TIM3, etc.
void(*Timer_Call_Function[6])(void);

/**
 * @brief Enable update interrupt from timer (UIE = 1).
 */
void TIMER_enableInterrupt(TIMER_RegDef_t *Timer)
{
    Timer->DIER |= (1 << 0);  // UIE = 1
}

/**
 * @brief Disable interrupt and stop counter (UIE = 0, CEN = 0).
 */
void TIMER_disableInterrupt_and_Counter(TIMER_RegDef_t *Timer)
{
    Timer->DIER &= ~(1 << 0);  // Disable update interrupt
    Timer->CR1  &= ~(1 << 0);  // Stop timer
}

/**
 * @brief Configure and start a millisecond delay with interrupt-based callback.
 *
 * - ARR gets loaded
 * - UG triggers update event to apply buffered values
 * - UIF is cleared so interrupt doesn't fire prematurely
 * - Timer is started
 */
void TIMER_voidStartCountMilliSeconds(TIMER_RegDef_t *Timer, uint32_t ms, void(*ptr)(void), uint8_t Timer_Number)
{
    Timer_Call_Function[Timer_Number] = ptr;
    Timer->ARR = ms - 1;  // Target delay value
    Timer->EGR |= (1 << 0);  // Trigger update event (UEV) → ARR/PSC shadow values applied
    Timer->SR &= ~(1 << 0);  // Clear UIF before starting
    Timer->CR1 |= (1 << 0);  // Start timer
}

/**
 * TIM2 IRQ Handler — fires when UIF = 1
 * UIF is set after CNT reaches ARR (i.e. update event occurred)
 */
void TIM2_IRQHandler(void)
{
    if ((TIM2->SR >> 0) & 1)
        TIM2->SR &= ~(1 << 0);  // Clear UIF
    Timer_Call_Function[2]();
}

void TIM3_IRQHandler(void)
{
    if ((TIM3->SR >> 0) & 1)
        TIM3->SR &= ~(1 << 0);
    Timer_Call_Function[3]();
}

void TIM4_IRQHandler(void)
{
    if ((TIM4->SR >> 0) & 1)
        TIM4->SR &= ~(1 << 0);
    Timer_Call_Function[4]();
}

void TIM5_IRQHandler(void)
{
    if ((TIM5->SR >> 0) & 1)
        TIM5->SR &= ~(1 << 0);
    Timer_Call_Function[5]();
}

/*
 * 🧠 How TIMERS Work Together (Prescaler, CNT, ARR, ARPE, UG, UIF, UEV):
 *
 * ┌────────────────┐         ┌────────────────┐         ┌────────────────┐         ┌─────────────────┐
 * │ TIM_CLK        │ ────>   │ Prescaler      │ ────>   │ Counter        │ ────>   │ Auto-Reload     │
 * └────────────────┘         └────────────────┘         └────────────────┘         └──────────────── ┘
 *   (e.g. 16 MHz)         ÷ (PSC + 1)              CNT ticks              Resets CNT when ARR reached
 *
 * ➞ **PSC**: slows down counter speed by dividing TIM_CLK
 * ➞ **CNT**: increments based on PSC result. When CNT == ARR → Update Event (UEV)
 * ➞ **ARR**: sets the overflow value. When CNT reaches ARR → CNT resets, and:
 *      - UEV is triggered
 *      - UIF (bit 0 in SR) is set
 *      - If UIE is enabled → IRQ is fired
 *
 * ➞ **ARPE (Auto-Reload Preload Enable)**:
 *      - 0 → ARR is updated immediately
 *      - 1 → ARR is written into shadow register, takes effect only on UEV
 *
 * ➞ **Shadow Register**:
 *      - Ensures timing consistency
 *      - Useful when updating ARR while timer is running
 *
 * ➞ **UG (Update Generation)**:
 *      - Manual way to generate UEV
 *      - Needed when ARPE = 1, to apply new ARR/PSC values immediately
 *
 * ➞ **UIF (Update Interrupt Flag)**:
 *      - Set when UEV happens (CNT = ARR)
 *      - Must be cleared manually
 *      - Polling logic and IRQs depend on it
 *
 * 🔀 Typical Sequence:
 * 1. Set PSC
 * 2. Set ARR
 * 3. If ARPE = 1, trigger UG to transfer ARR to logic
 * 4. Clear UIF
 * 5. Start timer (CEN)
 * 6. Wait for UIF or handle in interrupt
 */

/*
╔═════════════════════════════════════════════════════════════════════════════╗
║ 🔄 FULL EXAMPLE FLOW: How Timer Components Work Together                    ║
╚═════════════════════════════════════════════════════════════════════════════╝

🧠 GOAL:
Generate a delay using the timer with accurate timing and optional interrupts.

🔧 SETUP EXAMPLE:
- TIM_CLK       = 16 MHz
- Prescaler     = 15999 ➜ Tick every 1 ms
- ARR (Auto-Reload) = 999 ➜ Overflow (UIF) every 1000 ticks (1 second)

1️⃣ Prescaler (PSC)
   • PSC = 15999 → Counter ticks once every 1 ms.
   • Set directly to PSC register.

2️⃣ ARR & ARPE (Auto-Reload Preload Enable)
   • If ARPE = 1:
        - ARR value is written into a **shadow register**.
        - It doesn't take effect immediately.
        - It takes effect only when a **UEV (Update Event)** happens.
   • If ARPE = 0:
        - ARR is applied immediately.

3️⃣ UG (Update Generation)
   • UG = 1 (set in EGR):
        - Forces a UEV manually.
        - This causes shadow register values (ARR, PSC) to be transferred into the real logic.
        - Ensures updated values are used right away.

4️⃣ Counter (CNT)
   • Starts from 0 and increments on each tick (based on PSC).
   • Counts up to ARR.
   • If CNT == ARR → triggers **UEV** (automatically).
   • Counter is then reloaded with 0.

5️⃣ UIF (Update Interrupt Flag)
   • Set in SR register when UEV occurs.
   • This flag tells us that the update event happened.
   • Can be:
        - **Polled manually**: via while loop
        - **Handled automatically**: via interrupt (if UIE = 1)

6️⃣ UIE (Update Interrupt Enable)
   • Set in DIER register.
   • When UIE = 1 and UIF is set → NVIC triggers the ISR (e.g., TIM2_IRQHandler).

7️⃣ Inside ISR:
   • Check UIF bit → Clear it manually.
   • Call user callback function if needed.

🧩 KEY INTEGRATION POINT:
   ▪ ARR written (buffered if ARPE = 1)
   ▪ UG triggers UEV → Transfers ARR from shadow to actual
   ▪ CNT starts counting
   ▪ CNT reaches ARR → UEV triggered again → UIF is set
   ▪ If UIE = 1 → ISR is executed

📌 Example Timeline:
Time  | CNT | ARR = 999 | Event
------|-----|-----------|-------------------------------
0 ms  | 0   | 999       | CNT starts
...   | ... |           | CNT increments every 1 ms
999ms | 999 | 999       | CNT == ARR → UEV occurs
      |     |           | UIF = 1 → interrupt or polling
      | 0   |           | CNT resets to 0

*/

