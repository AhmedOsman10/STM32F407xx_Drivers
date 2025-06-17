/*
 * Registers_EXTI.h
 *
 *  Created on: May 30, 2025
 *      Author: Ahmed
 */

#ifndef EXTI_EXTI_REGISTERS_H_
#define EXTI_EXTI_REGISTERS_H_

#include <stdint.h>
#define EXTI_BASEADDR      0x40013C00U

// Structure definition representing the EXTI registers
typedef struct
{
    uint32_t IMR;     // Interrupt mask register
    uint32_t EMR;     // Event mask register
    uint32_t RTSR;    // Rising trigger selection register
    uint32_t FTSR;    // Falling trigger selection register
    uint32_t SWIER;   // Software interrupt event register
    uint32_t PR;      // Pending register
} EXTI_RegDef_t;

#define EXTI              ((EXTI_RegDef_t*)(EXTI_BASEADDR))

#endif /* EXTI_EXTI_REGISTERS_H_ */
