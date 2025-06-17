/*
 * ADC_Interface.h
 *
 *  Created on: Jun 6, 2025
 *      Author: Ahmed
 *
 *  Description:
 *  This header provides the interface (APIs) for initializing, starting,
 *  reading from, and disabling the ADC peripheral in STM32 microcontrollers.
 */

#ifndef ADC_ADC_INTERFACE_H_
#define ADC_ADC_INTERFACE_H_

#include "ADC_Config.h"
#include "ADC_Registers.h"

/* ADC regular sequence register 1 (ADC_SQR1) */
/* ADC regular sequence length (L[3:0] in ADC_SQR1) */
/* These define the number of conversions in the regular group MINUS ONE */

#define ADC_REG_CONV_1				0b0000  // 1 conversion
#define ADC_REG_CONV_2				0b0001  // 2 conversions
#define ADC_REG_CONV_3				0b0010  // 3 conversions
#define ADC_REG_CONV_4				0b0011  // 4 conversions
#define ADC_REG_CONV_5				0b0100  // 5 conversions
#define ADC_REG_CONV_6				0b0101  // 6 conversions
#define ADC_REG_CONV_7				0b0110  // 7 conversions
#define ADC_REG_CONV_8				0b0111  // 8 conversions
#define ADC_REG_CONV_9				0b1000  // 9 conversions
#define ADC_REG_CONV_10				0b1001  // 10 conversions
#define ADC_REG_CONV_11				0b1010  // 11 conversions
#define ADC_REG_CONV_12				0b1011  // 12 conversions
#define ADC_REG_CONV_13				0b1100  // 13 conversions
#define ADC_REG_CONV_14				0b1101  // 14 conversions
#define ADC_REG_CONV_15				0b1110  // 15 conversions
#define ADC_REG_CONV_16				0b1111  // 16 conversions

/* ----------------------------------------------------------------------------
 * ADC Conversion Modes
 *
 * ADC_MODE_SINGLE_CONV:
 *    - Performs a single conversion and stops.
 *    - Requires software to manually start each conversion (typically in a loop).
 *
 * ADC_MODE_CONTINOUS_CONV:
 *    - ADC continues converting automatically after each conversion.
 *    - Only one start trigger is needed.
 * -------------------------------------------------------------------------- */
#define ADC_MODE_SINGLE_CONV        0
#define ADC_MODE_CONTINUOUS_CONV    1

/* ----------------------------------------------------------------------------
 * Function Prototypes
 * -------------------------------------------------------------------------- */

/**
 * @brief  Initializes the ADC peripheral with desired mode and number of conversions.
 * @param  ADC         : Pointer to ADC register definition structure. i.e. ADC1, or ADC2, or ADC3.
 * @param  mode        : Conversion mode (single or continuous).
 * @param  conv_number : Number of regular conversions to configure (1–16).
 */
void ADC_init(ADC_RegDef_t *ADC, uint8_t mode, uint8_t conv_number);

/**
 * @brief  Enables the selected ADC channel and assigns its priority in the sequence.
 * @param  ADC      : Pointer to ADC register definition structure.
 * @param  channel  : Channel number to enable (0–15 depending on MCU).
 * @param  priority : Position in conversion sequence (0 = first).
 */
void ADC_enableChannel(ADC_RegDef_t *ADC, uint8_t channel, uint8_t priority);

/**
 * @brief  Starts ADC regular conversion (for both single and continuous modes).
 *         - In single mode: must be called before every read.
 *         - In continuous mode: only called once.
 */
void ADC_startConversion(ADC_RegDef_t *ADC);

/**
 * @brief  Reads ADC conversion result from data register.
 * @note   Waits for End Of Conversion (EOC) flag before returning the result.
 * @retval 12-bit (or configured resolution) digital ADC result.
 */
uint16_t ADC_readData(ADC_RegDef_t *ADC);

/**
 * @brief  Reads a sequence of ADC conversions into a buffer.
 * @param  ADC     : Pointer to ADC register definition structure.
 * @param  buffer  : Pointer to memory to store conversion results.
 * @param  count   : Number of conversions to read (must match sequence length).
 */
void ADC_readSequence(ADC_RegDef_t *ADC, uint16_t* buffer, uint8_t count);

/**
 * @brief  Disables the ADC to save power.
 * @param  ADC : Pointer to ADC register definition structure.
 */
void ADC_disable(ADC_RegDef_t *ADC);

#endif /* ADC_ADC_INTERFACE_H_ */
