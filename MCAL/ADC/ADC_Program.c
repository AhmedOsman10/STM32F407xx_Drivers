/*
 * ADC_Program.c
 *
 *  Created on: Jun 6, 2025
 *      Author: Ahmed
 */

/* Explanation of the code is provided at the end of this file */

#include "ADC_Interface.h"

void ADC_init(ADC_RegDef_t *ADC, uint8_t mode, uint8_t conv_number)
{
    /* Enable ADC by setting ADON bit (CR2[0]) */
    ADC->CR2 |= (1 << 0);

    /* Set resolution: bits CR1[25:24] */
    ADC->CR1 &= ~(0x3 << 24);              // Clear previous resolution
    ADC->CR1 |= (RESOLUTION << 24);        // Apply selected resolution

    /* Configure conversion mode: CR2[1] */
    if (mode == ADC_MODE_SINGLE_CONV)
        ADC->CR2 &= ~(1 << 1);             // Single conversion
    else
    {
    	/* Enable scan mode */
    	ADC->CR1 |= (1 << 8); 				// SCAN bit
    	ADC->CR2 |= (1 << 1);    			// Continuous conversion
    }

    /* Set number of conversions in the regular sequence (SQR1[23:20]) */
    ADC->SQR1 &= ~(0b1111 << 20);
    ADC->SQR1 |= (conv_number << 20);
}

void ADC_enableChannel(ADC_RegDef_t *ADC, uint8_t channel, uint8_t priority)
{
	/*
		* Set ADC channel to be converted (configured in SQR3 for low-numbered channels).
		*
		* IMPORTANT:
		* - ADC channels are **fixed** to specific pins — unlike EXTI, which allows flexible mapping across GPIO ports.
		* - You must determine the correct ADC channel number from the **microcontroller datasheet**, not from the reference manual
		*   specifically from the **"Additional Functions"** column in the pinout table.
		* - For example:
		*     • `PA1` maps to `ADC_IN1`, so to sample `PA1`, you must select **Channel 1**.
		*     • `PB1` might map to a different ADC channel — you cannot reassign channels like you can with EXTI.
		* - Unlike EXTI (e.g., EXTI0 ↔ PA0 or PB0), **each ADC channel corresponds to one and only one specific pin**.
		* - Therefore, you select **one channel number**, not a combination of port and pin like EXTI.
		* Summary:
		* ✅ “The channel number is only one, not like the EXTI which you configure the channel you want from the ports like GPIOA, GPIOB, GPIOC…”
		Unlike EXTI, where you can select any GPIO pin from any port to connect to a specific EXTI line (e.g., EXTI0 can come from PA0, PB0, etc.), the ADC channels are fixed to specific pins.
		   Example:
		   ADC_IN1 is always mapped to PA1 on STM32F407. You cannot map ADC_IN1 to another pin (e.g., PB1).
	*/
	    if (priority < 1 || priority > 16)
	        return;

	    else if(priority < 7)
	    {
			ADC->SQR3 &= ~(0b11111 << (5 * (priority - 1)));        // Clear first conversion channel
			ADC->SQR3 |= (channel << (5 * (priority - 1)));         // Set desired channel
	    }

	    else if(priority < 13)
	    {
			ADC->SQR2 &= ~(0b11111 << (5 * (priority - 7)));        // Clear first conversion channel
			ADC->SQR2 |= (channel<< (5 * (priority - 7)));         // Set desired channel
	    }

	    else if (priority < 17)
	    {
	        ADC->SQR1 &= ~(0b11111 << (5 * (priority - 13)));
	        ADC->SQR1 |= (channel << (5 * (priority - 13)));
	    }
}

void ADC_startConversion(ADC_RegDef_t *ADC)
{
    /*
     * Start regular conversion by setting the SWSTART bit (CR2[30]).
     *
     * IMPORTANT:
     * - This command is required for **both** single and continuous conversion modes.
     * - In **single conversion mode**, you need to call this function each time you want a new conversion.
     *   So it should be called inside a loop like while(1) to continuously get updated readings.
     * - In **continuous conversion mode**, only **one call** to this function is needed.
     *   After that, the ADC will keep converting automatically without needing to call this again.
     */
    ADC->CR2 |= (1 << 30);  // SWSTART is bit 30
}

uint16_t ADC_readData(ADC_RegDef_t *ADC)
{
    /* Wait until EOC (end of conversion) flag is set: SR[1] */
    while (!((ADC->SR >> 1) & 0x1));

    /* Read converted data */
    return (uint16_t)(ADC->DR);
    // Reading ADC->DR automatically clears the EOC flag in STM32F4
}

void ADC_readSequence(ADC_RegDef_t *ADC, uint16_t* buffer, uint8_t count)
{
    /*
     * @brief  Reads a full sequence of ADC conversions.
     *         This function is useful when the ADC is set up to convert multiple channels in sequence.
     *
     * @param  ADC     : Pointer to ADC register definition structure.
     * @param  buffer  : Pointer to the array where conversion results will be stored.
     * @param  count   : Number of conversions to read (must match configured sequence length).
     *
     * Process:
     *   - Waits for End of Conversion (EOC) flag for each channel.
     *   - Reads the ADC data register (DR) into the buffer.
     *   - Reading the DR clears the EOC flag automatically.
     */
    for (uint8_t i = 0; i < count; i++)
    {
        while (!((ADC->SR >> 1) & 0x1));  // Wait for EOC (end of each channel)
        buffer[i] = ADC->DR;              // Read channel result in sequence
        // Reading ADC->DR automatically clears the EOC flag in STM32F4
    }
}

void ADC_disable(ADC_RegDef_t *ADC)
{
    /* Disable ADC by clearing ADON bit (CR2[0]) */
    ADC->CR2 &= ~(1 << 0);
}


/*
 * -----------------------------------------------------------------------------
 * Concept and Functionality:
 *
 * This code initializes, starts, reads from, and disables the ADC (Analog to Digital Converter) peripheral of an STM32 microcontroller.
 * The ADC converts an analog voltage input (e.g., 0 to 3.3V) into a digital value, which can be processed by the microcontroller. The digital output depends on the
 * configured resolution (e.g., 12-bit resolution means values from 0 to 4095).
 *
 * -----------------------------------------------------------------------------
 * How the ADC peripheral works in STM32:
 *
 * 1. Enable the ADC by setting the ADON bit.
 * 2. Configure resolution, conversion mode (single or continuous), and input channel.
 * 3. Start the conversion (regular or injected).
 * 4. Wait until conversion completes (polling the EOC flag).
 * 5. Read the converted digital value from the data register.
 * 6. Disable the ADC by clearing the ADON bit when ADC is no longer needed to save power.
 *
 * -----------------------------------------------------------------------------
 * Function Details:
 *
 * ADC_init():
 *   - Enables the ADC.
 *   - Sets resolution via CR1 register.
 *   - Configures single or continuous conversion mode in CR2.
 *   - Sets the number of conversions in the regular sequence (usually 1).
 *   - Selects the input channel on SQR3 register.
 *
 * ADC_startConversion():
 *   - Starts regular conversion by setting the SWSTART bit (CR2[30]).
 *   - This must be called to trigger the ADC whether using single or continuous mode.
 *   - In **single conversion mode**, this must be called **each time** a new value is needed, so it should typically be placed inside a while(1) loop.
 *   - In **continuous conversion mode**, you only need to call it **once**; the ADC will continue conversions automatically.
 *
 * ADC_readData():
 *   - Waits until the EOC (End Of Conversion) flag is set (SR bit 1).
 *   - Clears the EOC flag.
 *   - Returns the digital conversion result from the data register (DR).
 *
 *   - Used when the ADC is configured to perform multiple conversions in a regular sequence (multi-channel scan mode).
 *   - Takes a pointer to a buffer and fills it with the ADC results, one for each channel in the sequence.
 *   - It waits for the End Of Conversion (EOC) flag before reading each value from the data register (DR).
 *   - The EOC flag is automatically cleared after each read, so the loop proceeds to the next result.
 *   - The number of readings (`count`) should match the number configured in ADC_init (via conv_number).
 *   - Example usage: Reading temperature, light, and voltage sensors in one loop using a single ADC.
 *
 *
 * ADC_disable():
 *   - Disables the ADC by clearing the ADON bit in CR2 register to save power when ADC is not used.
 *
 * -----------------------------------------------------------------------------
 * Usage Example:
 *
 * For a sensor output voltage between 0V and 3.3V:
 *   - At 12-bit resolution, ADC value ranges from 0 to 4095.
 *   - You can convert ADC reading back to voltage by:
 *       voltage = (adc_value / 4095.0) * 3.3;
 *
 * -----------------------------------------------------------------------------
 */
