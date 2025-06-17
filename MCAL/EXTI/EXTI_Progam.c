/*
 * EXTI_Program.c
 *
 *  Created on: May 30, 2025
 *      Author: Ahmed
 */

/* --------------------------------------------------------------------------
 * IMPORTANT: “Enabling” an interrupt is **not** the same as “triggering” it.
 *
 *   • Enabling (setting the bit in IMR/EMR) simply arms the EXTI line so the
 *     CPU is allowed to react **if** a trigger event occurs.
 *
 *   • Triggering is the event itself ─ either a hardware edge detected by
 *     RTSR/FTSR, or a manual software request via SWIER ─ that sets the
 *     pending flag and invokes the ISR.
 *
 *   In short: enabling = “un-mute the doorbell”; triggering = “press the
 *   doorbell button”.
 * -------------------------------------------------------------------------- */

#include "EXTI_Interface.h"

// Array of function pointers to hold callback functions for each EXTI line
void(*EXTI_Call_Function[16])(void);

/*
 * Enables hardware interrupt for the specified EXTI line
 * Parameters:
 *   EXTI_line - the external interrupt line (0 to 15)
 *   RTSR - 1 to enable rising edge trigger, 0 otherwise
 *   FTSR - 1 to enable falling edge trigger, 0 otherwise
 *   ptr - pointer to the callback function
 */
void EXTI_enableInterruptHardware(uint8_t EXTI_line, uint8_t RTSR, uint8_t FTSR, void(*ptr)(void))
{
	EXTI->IMR |= (1 << EXTI_line);                    // Enable interrupt request from EXTI line
	EXTI->RTSR &= ~(1 << EXTI_line);                  // Clear RTSR bit first
	EXTI->RTSR |= (RTSR << EXTI_line);                // Set RTSR bit if needed
	EXTI->FTSR &= ~(1 << EXTI_line);                  // Clear FTSR bit first
	EXTI->FTSR |= (FTSR << EXTI_line);                // Set FTSR bit if needed
	EXTI_Call_Function[EXTI_line] = ptr;              // Save user callback function
}

/*
 * Enables software-controlled interrupt for the EXTI line
 * Parameters:
 *   EXTI_line - the external interrupt line (0 to 15)
 *   ptr - pointer to the callback function
 */
void EXTI_enableInterruptSoftware(uint8_t EXTI_line, void(*ptr)(void))
{
	EXTI->IMR |= (1 << EXTI_line);                    // Enable interrupt mask
	EXTI_Call_Function[EXTI_line] = ptr;              // Save user callback function
}

/*
 * Triggers the interrupt manually via software
 * Parameters:
 *   EXTI_line - the external interrupt line (0 to 15)
 */
void EXTI_triggerInterruptSoftware(uint8_t EXTI_line)
{
	EXTI->SWIER |= (1 << EXTI_line);                  // Set software interrupt event register
}

/*
 * Enables hardware event (not interrupt) for the EXTI line
 * Parameters:
 *   EXTI_line - the external interrupt line (0 to 15)
 *   RTSR - 1 to enable rising edge trigger, 0 otherwise
 *   FTSR - 1 to enable falling edge trigger, 0 otherwise
 */
void EXTI_enableEventHardware(uint8_t EXTI_line, uint8_t RTSR, uint8_t FTSR)
{
	EXTI->EMR |= (1 << EXTI_line);                    // Enable event mask
	EXTI->RTSR &= ~(1 << EXTI_line);
	EXTI->RTSR |= (RTSR << EXTI_line);
	EXTI->FTSR &= ~(1 << EXTI_line);
	EXTI->FTSR |= (FTSR << EXTI_line);
}

/*
 * Enables software event (not interrupt) for the EXTI line
 * Parameters:
 *   EXTI_line - the external interrupt line (0 to 15)
 */
void EXTI_enableEventSoftware(uint8_t EXTI_line)
{
	EXTI->EMR |= (1 << EXTI_line);                    // Enable event mask
}

/*
 * ISR for EXTI line 0
 */
void EXTI0_IRQHandler(void)
{
	EXTI_Call_Function[0]();                          // Call user-defined function
	EXTI->PR |= (1 << 0);                             // Clear pending flag
}

/*
 * ISR for EXTI line 1
 */
void EXTI1_IRQHandler(void)
{
	EXTI_Call_Function[1]();
	EXTI->PR |= (1 << 1);
}

/*
 * ISR for EXTI line 2
 */
void EXTI2_IRQHandler(void)
{
	EXTI_Call_Function[2]();
	EXTI->PR |= (1 << 2);
}

/*
 * ISR for EXTI line 3
 */
void EXTI3_IRQHandler(void)
{
	EXTI_Call_Function[3]();
	EXTI->PR |= (1 << 3);
}

/*
 * ISR for EXTI line 4
 */
void EXTI4_IRQHandler(void)
{
	EXTI_Call_Function[4]();
	EXTI->PR |= (1 << 4);
}

/*
 * ISR for EXTI lines 5 to 9 (shared vector)
 */
void EXTI9_5_IRQHandler(void)
{
	for(int i = 5; i < 10; i++)
	{
		if(((EXTI->PR >> i) & 1) == 1)                 // Check pending bit
		{
			EXTI_Call_Function[i]();                  // Call user-defined function
			EXTI->PR |= (1 << i);                     // Clear pending flag
		}
	}
}

/*
 * ISR for EXTI lines 10 to 15 (shared vector)
 */
void EXTI15_10_IRQHandler(void)
{
	for(int i = 10; i < 16; i++)
	{
		if(((EXTI->PR >> i) & 1) == 1)
		{
			EXTI_Call_Function[i]();
			EXTI->PR |= (1 << i);
		}
	}
}

/*
 * ------------------------------------------------------------------------------
 * HOW EXTI WORKS:
 * ------------------------------------------------------------------------------
 *
 * The EXTI (External Interrupt/Event Controller) in STM32 allows peripherals or
 * GPIO pins to generate **interrupts** or **events** based on signal edges.
 * This driver handles both software and hardware-triggered EXTI lines.
 *
 * 1. INTERRUPT VS EVENT:
 *    • Interrupt (IMR): When triggered, causes the CPU to jump to an ISR.
 *    • Event (EMR): When triggered, causes an internal signal but does NOT
 *      generate an interrupt (no ISR).
 *
 * 2. TRIGGER CONFIGURATION:
 *    • RTSR: Rising trigger selection register.
 *    • FTSR: Falling trigger selection register.
 *    • You can enable either or both based on what edge should trigger the EXTI.
 *
 * 3. ENABLING AN INTERRUPT:
 *    EXTI_enableInterruptHardware():
 *      - Enables interrupt via IMR.
 *      - Configures rising/falling edges.
 *      - Stores user-defined callback.
 *
 * 4. SOFTWARE TRIGGER:
 *    EXTI_triggerInterruptSoftware():
 *      - Manually triggers the interrupt by setting the SWIER bit.
 *      - Useful for testing or internal signaling.
 *
 * 5. EVENTS:
 *    EXTI_enableEventHardware() and EXTI_enableEventSoftware():
 *      - Enable event-only generation without CPU interrupt.
 *
 * 6. CALLBACK MECHANISM:
 *    - Each EXTI line (0–15) can have a separate user-defined callback function.
 *    - These are stored in the EXTI_Call_Function[] array.
 *    - In ISRs (e.g., EXTI0_IRQHandler), the corresponding callback is called.
 *    - Pending bit is cleared by writing 1 to EXTI->PR.
 *
 * 7. SHARED INTERRUPTS:
 *    - EXTI lines 5–9 and 10–15 share IRQ handlers (EXTI9_5 and EXTI15_10).
 *    - The code checks each line’s pending bit and calls the respective callback.
 *
 * SUMMARY FLOW:
 * --------------
 * [1] Configure GPIO pin to be EXTI source (done in SYSCFG).
 * [2] Use `EXTI_enableInterruptHardware()` to enable the EXTI line.
 * [3] On trigger (rising/falling edge or software), EXTI sets pending bit.
 * [4] NVIC calls the corresponding IRQHandler().
 * [5] IRQHandler calls the stored callback function.
 * [6] IRQHandler clears the pending bit.
 */
