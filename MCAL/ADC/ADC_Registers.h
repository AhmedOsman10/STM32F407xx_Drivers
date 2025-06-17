/*
 * ADC.Registers.h
 *
 *  Created on: Jun 6, 2025
 *      Author: Ahmed
 */

#ifndef ADC_ADC_REGISTERS_H_
#define ADC_ADC_REGISTERS_H_

#include <stdint.h>

#define ADC_BASEADDR  0x40012000U  // Base address for ADC1

typedef struct
{
    uint32_t SR;         // Status register: indicates end of conversion, analog watchdog flag, etc.
    uint32_t CR1;        // Control register 1: configures resolution, analog watchdog, etc.
    uint32_t CR2;        // Control register 2: starts conversions, DMA, triggers, etc.
    uint32_t SMPR1;      // Sample time register 1: sampling times for channels 10 to 18
    uint32_t SMPR2;      // Sample time register 2: sampling times for channels 0 to 9
    uint32_t JOFR[4];    // Injected channel data offset registers (JOFR1–JOFR4)
    uint32_t HTR;        // Higher threshold register: for analog watchdog
    uint32_t LTR;        // Lower threshold register: for analog watchdog
    uint32_t SQR1;       // Regular sequence register 1: length and channels 13-16
    uint32_t SQR2;       // Regular sequence register 2: channels 7-12
    uint32_t SQR3;       // Regular sequence register 3: channels 1-6
    uint32_t SQR;        // (Optional/Invalid — remove if using STM32F1/F4) Possibly a mistake, not a valid ADC register
    uint32_t JDR[4];     // Injected data registers (JDR1–JDR4): holds results of injected conversions
    uint32_t DR;         // Data register: holds result of regular conversion
    uint32_t CSR;        // Common status register: used in dual/triple ADC mode (ADC1 only)
    uint32_t CDR;        // Common data register: combined data from ADC1 and ADC2 (dual mode)
}ADC_RegDef_t;

// ADC instance base pointers
#define ADC1  ((ADC_RegDef_t*)(ADC_BASEADDR + 0x000))  // ADC1 base
#define ADC2  ((ADC_RegDef_t*)(ADC_BASEADDR + 0x100))  // ADC2 base (offset 0x100)
#define ADC3  ((ADC_RegDef_t*)(ADC_BASEADDR + 0x200))  // ADC3 base (offset 0x200)

#endif /* ADC_ADC_REGISTERS_H_ */
