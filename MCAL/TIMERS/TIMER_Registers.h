/*
 * TIMERS_Registers.h
 *
 *  Created on: Jun 12, 2025
 *      Author: Ahmed
 */

#ifndef TIMERS_TIMERS_REGISTERS_H_
#define TIMERS_TIMERS_REGISTERS_H_

#include <stdint.h>
#define TIMER_BASEADDR   0x40000000U  // Base address for TIM2

typedef struct
{
    uint32_t CR1;      // Control register 1
    uint32_t CR2;      // Control register 2
    uint32_t SMCR;     // Slave mode control register
    uint32_t DIER;     // DMA/Interrupt enable register
    uint32_t SR;       // Status register
    uint32_t EGR;      // Event generation register
    uint32_t CCMR1;    // Capture/Compare mode register 1
    uint32_t CCMR2;    // Capture/Compare mode register 2
    uint32_t CCER;     // Capture/Compare enable register
    uint32_t CNT;      // Counter
    uint32_t PSC;      // Prescaler
    uint32_t ARR;      // Auto-reload register
    uint32_t CCR1;     // Capture/Compare register 1
    uint32_t CCR2;     // Capture/Compare register 2
    uint32_t CCR3;     // Capture/Compare register 3
    uint32_t CCR4;     // Capture/Compare register 4
    uint32_t DCR;      // DMA control register
    uint32_t DMAR;     // DMA address for full transfer
}TIMER_RegDef_t;

#define TIM2    		((TIMER_RegDef_t*)(TIMER_BASEADDR + 0x0000))
#define TIM3    		((TIMER_RegDef_t*)(TIMER_BASEADDR + 0x0400))
#define TIM4    		((TIMER_RegDef_t*)(TIMER_BASEADDR + 0x0800))
#define TIM5    		((TIMER_RegDef_t*)(TIMER_BASEADDR + 0x0C00))

#endif /* TIMERS_TIMERS_REGISTERS_H_ */
