#include "TIMER_Interface.h"

/**
 * @brief Generate a PWM signal on a specific timer channel.
 *
 * @param Timer       Pointer to the timer peripheral registers (e.g., TIM2, TIM3).
 * @param channel     PWM output channel (1 to 4).
 * @param periode     Auto-reload value (ARR): determines PWM frequency.
 * @param duty_cycle  Capture/Compare value (CCR): determines PWM duty cycle.
 *
 * ➤ This function configures PWM mode, enables preload, and sets ARR, CCRx, and CCER.
 */
void TIMER_generatePWM(TIMER_RegDef_t *Timer, uint8_t channel, uint32_t periode, uint32_t duty_cycle)
{
    // Each channel's configuration bits (CCxE, CCxP) are spaced by 4 bits in CCER
    uint8_t offset = (channel - 1) * 4;

    /***********************************************************
     * ⚙️ Configure CCMRx: Set PWM mode and enable preload
     *
     * ➤ Each channel has an Output Compare Mode:
     *    - OCxM bits select PWM mode (PWM_MODE = 0b110 or 0b111)
     *    - OCxPE (Preload Enable): buffers CCRx to ensure smooth PWM
     *
     * ➤ CCRx = duty_cycle (pulse width)
     ***********************************************************/
    switch (channel)
    {
        case 1:
            Timer->CCMR1 &= ~(0b111 << 4);              // Clear OC1M bits
            Timer->CCMR1 |= (PWM_MODE << 4);            // Set PWM mode
            Timer->CCMR1 |= (1 << 3);                   // OC1PE = 1: Preload enable
            Timer->CCR1 = duty_cycle;                   // Set duty cycle (pulse width)
            break;

        case 2:
            Timer->CCMR1 &= ~(0b111 << 12);             // Clear OC2M bits
            Timer->CCMR1 |= (PWM_MODE << 12);           // Set PWM mode
            Timer->CCMR1 |= (1 << 11);                  // OC2PE = 1: Preload enable
            Timer->CCR2 = duty_cycle;
            break;

        case 3:
            Timer->CCMR2 &= ~(0b111 << 4);              // Clear OC3M bits
            Timer->CCMR2 |= (PWM_MODE << 4);            // Set PWM mode
            Timer->CCMR2 |= (1 << 3);                   // OC3PE = 1: Preload enable
            Timer->CCR3 = duty_cycle;
            break;

        case 4:
            Timer->CCMR2 &= ~(0b111 << 12);             // Clear OC4M bits
            Timer->CCMR2 |= (PWM_MODE << 12);           // Set PWM mode
            Timer->CCMR2 |= (1 << 11);                  // OC4PE = 1: Preload enable
            Timer->CCR4 = duty_cycle;
            break;

        default:
            return;  // ❌ Invalid channel: exit without configuring
    }

    /***********************************************************
     * ⚡ Configure CCER: Output enable and polarity
     *
     * ➤ CCxE = 1: Enable output on this channel
     * ➤ CCxP = 0 or 1: Output polarity (POLARITY macro defines it)
     ***********************************************************/
    Timer->CCER &= ~(0b11 << offset);                  // Clear CCxE and CCxP bits
    Timer->CCER |= (1 << offset);                      // CCxE = 1: Output enable
    Timer->CCER |= (POLARITY << (offset + 1));         // Set output polarity (0 = active high, 1 = active low)

    /***********************************************************
     * 📏 Set PWM Frequency and Start Timer
     *
     * ➤ ARR defines PWM period: total duration of one cycle
     * ➤ UG = 1 (EGR): Forces update event → transfer buffered ARR/CCR
     * ➤ CEN = 1: Start the timer (enable counting)
     ***********************************************************/
    Timer->ARR = periode - 1;                          // Set auto-reload for full PWM period
    Timer->EGR |= (1 << 0);                            // UG = 1: Trigger update event
    Timer->CR1 |= (1 << 0);                            // CEN = 1: Start counter
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ 🧠 How TIMER_generatePWM Works: Full PWM Output Setup Explained              ║
╚══════════════════════════════════════════════════════════════════════════════╝

🔧 GOAL:
Generate a PWM signal on a given channel with a specified frequency and duty cycle.

╭───────────────────────────────╮
│ 📦 Registers & What They Do   │
╰───────────────────────────────╯

1️⃣ CCRx (Capture/Compare Register):
    - Stores the pulse width (ON time).
    - When CNT == CCRx → Output goes LOW (in PWM mode 1).
    - Duty % = (CCRx / (ARR + 1)) * 100

2️⃣ ARR (Auto-Reload Register):
    - Sets the top value the timer counts to.
    - Controls the total period (T_PWM).
    - T_PWM = ((PSC + 1) * (ARR + 1)) / f_CLK

3️⃣ CCMRx (Capture/Compare Mode Registers):
    - Configures PWM mode (OCxM bits).
    - Enables preload buffer for CCRx (OCxPE = 1).

4️⃣ CCER (Capture/Compare Enable Register):
    - Enables output on the channel (CCxE).
    - Sets output polarity (CCxP).

5️⃣ EGR (Event Generation Register):
    - UG = 1 triggers update event → transfers buffered values (ARR/CCR) to active logic.

6️⃣ CR1 (Control Register 1):
    - CEN = 1 starts the timer.

╭───────────────────────────────╮
│ ⏱️ Timing Behavior Summary     │
╰───────────────────────────────╯

- Timer CNT counts from 0 → ARR repeatedly.
- Output is:
    - HIGH when CNT < CCRx
    - LOW when CNT ≥ CCRx (in PWM mode 1)
- PWM frequency depends on ARR and PSC:
    f_PWM = f_CLK / ((PSC + 1) * (ARR + 1))

╭───────────────────────────────╮
│ ✅ What This Function Does     │
╰───────────────────────────────╯

✔ Sets the correct PWM mode on the selected channel
✔ Enables output and polarity
✔ Loads CCRx = duty_cycle and ARR = periode - 1
✔ Triggers update event if ARPE is enabled
✔ Starts the timer

╭───────────────────────────────╮
│ 📝 Example:                   │
╰───────────────────────────────╯
- f_CLK = 16 MHz
- PSC = 15 → f_tick = 1 MHz
- ARR = 999 → T_PWM = 1 ms → f_PWM = 1 kHz
- CCR = 250 → 25% Duty cycle (250/1000)

Output = HIGH for 250 µs, LOW for 750 µs



══════════════════════════════════════════════════════════════════════════════
*/

/*
╔═══════════════════════════════════════════════════════════════════════╗
║ 📘 Timer and PWM Frequency Explained                                  ║
╚═══════════════════════════════════════════════════════════════════════╝

1️⃣ Timer Tick Frequency (f_tick)
──────────────────────────────────────
f_tick = f_CLK / (PSC + 1)

Example:
f_CLK  = 16,000,000 Hz (16 MHz internal clock)
PSC    = 15
=> f_tick = 16,000,000 / (15 + 1)
          = 1,000,000 Hz (1 MHz)

👉 This means the timer counter (CNT) increments every **1 microsecond (µs)**.

2️⃣ PWM Frequency (f_PWM)
──────────────────────────────────────
f_PWM = f_tick / (ARR + 1)

Example:
ARR    = 999
=> f_PWM = 1,000,000 / (999 + 1)
         = 1,000 Hz

👉 One full PWM cycle occurs every **1 millisecond (ms)**.

3️⃣ PWM Period (T_PWM)
──────────────────────────────────────
T_PWM = 1 / f_PWM

Example:
T_PWM = 1 / 1000 = 0.001 seconds = **1 ms**

👉 This is the time for one full PWM cycle:
    - LED is ON for CCR ticks
    - LED is OFF for the rest until ARR is reached

═══════════════════════════════════════════════════════════════════════
*/


/*
 * =============================== PWM Dimming Notes ===============================
 *
 * 💡 Goal: Smooth LED dimming without visible blinking
 *
 * To achieve smooth and flicker-free dimming with PWM:
 *
 * 1. ✅ Use a **small prescaler (PSC)**:
 *      - Increases timer tick speed → higher PWM frequency
 *      - Example: PSC = 15 → f_tick = 1 MHz (for 16 MHz system clock)
 *
 * 2. ✅ Use a **reasonably large ARR**:
 *      - More ARR = more PWM steps → smoother brightness control
 *      - Example: ARR = 999 → 1000 steps (CCR = 0 to 999)
 *
 * 3. ✅ Make sure **PWM frequency is at least 500–1000 Hz**:
 *      - Avoids visible flickering to the human eye
 *
 * ----------------------------------------------
 * ⚠️ Avoid setting both PSC and ARR too small:
 *     - PSC = 0, ARR = 9 → f_PWM = 1.6 MHz (good), but only 10 brightness steps (bad)
 *     - Result: LED brightness will change in large jumps (choppy dimming)
 *
 * 🧠 Key Formulas:
 *
 *     Timer Tick Frequency:
 *         f_tick = f_CLK / (PSC + 1)
 *
 *     PWM Frequency:
 *         f_PWM = f_tick / (ARR + 1)
 *               = f_CLK / ((PSC + 1) * (ARR + 1))
 *
 *     PWM Period:
 *         T_PWM = 1 / f_PWM
 *
 *     Brightness (Duty Cycle %):
 *         Duty % = (CCR / (ARR + 1)) * 100
 *
 * ✅ Recommended Setup:
 *     - f_CLK = 16 MHz
 *     - PSC = 15       → f_tick = 1 MHz
 *     - ARR = 999      → f_PWM = 1 kHz (no flicker)
 *     - CCR = 0–999    → 1000 brightness levels (fine control)
 *
 * =================================================================================
 */

