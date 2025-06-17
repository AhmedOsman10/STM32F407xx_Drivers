/*
 * SYSCFG_Registers.h
 *
 *  Created on: Jun 3, 2025
 *      Author: Ahmed
 */

#ifndef SYSCFG_SYSCFG_REGISTERS_H_
#define SYSCFG_SYSCFG_REGISTERS_H_

#include <stdint.h>

/* Base address of the SYSCFG peripheral */
#define SYSCFG_BASEADDR  0x40013800U

/* @brief Structure representing the SYSCFG peripheral registers. */
typedef struct
{
	uint32_t MEMRMP;
	uint32_t PMC;
	/**
	 * @brief EXTI configuration registers (EXTICR1 to EXTICR4).
	 * EXTICR[0] = EXTICR1: Configures EXTI lines 0–3
	 * EXTICR[1] = EXTICR2: Configures EXTI lines 4–7
	 * EXTICR[2] = EXTICR3: Configures EXTI lines 8–11
	 * EXTICR[3] = EXTICR4: Configures EXTI lines 12–15
	 */
	uint32_t EXTICR[4];  // EXTI configuration registers 1 to 4. This array provides convenient access to all 4 EXTICR registers in the above comment.
	uint32_t CMPCR;
}SYSCFG_RegDef_t;

/* Macro to access the SYSCFG peripheral via a pointer to its base address */
#define SYSCFG  ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#endif /* SYSCFG_SYSCFG_REGISTERS_H_ */
