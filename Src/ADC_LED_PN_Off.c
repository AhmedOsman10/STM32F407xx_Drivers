#include "../MCAL/RCC/RCC_Interface.h"         /* Interface for RCC (Reset and Clock Control) functions */
#include "../MCAL/ADC/ADC_Interface.h"         /* Interface for ADC (Analog-to-Digital Converter) functions */
#include "../MCAL/GPIO/GPIO_Interface.h"       /* Interface for GPIO configuration */
#include "../MCAL/SYSTICK/SYSTICK_Interface.h" /* Interface for SYSTICK timer (not used in this code) */
#include "../ECUAL/LED/LED_Interface.h"        /* Interface for external LED control */

int main(void)
{
    /* Initialize system clocks */
    RCC_init();

    /* Enable peripheral clocks for GPIOA and ADC1 */
    RCC_Peri_Enable(GPIOA_EN);
    RCC_Peri_Enable(ADC1EN);

    /* Initialize LED connected to GPIOA PIN5 */
    LED_t red = {GPIOA, 5};
    LED_init(&red);

    /* Configure GPIOA PIN1 as analog input for ADC */
    GPIO_Mode(GPIOA, GPIO_PIN_1, GPIO_ANALOG);
    GPIO_Pull_Up_Pull_Down(GPIOA, GPIO_PIN_1, GPIO_NO_PULL_UP_NO_PULL_DOWN);

    /* Initialize ADC1 in single conversion mode on channel 1 (PA1) */
    ADC_init(ADC1, ADC_MODE_SINGLE_CONV, 1);

    /* Variable to store ADC conversion result */
    uint16_t var = 0;

    while (1)
    {
        /* Start ADC conversion on channel 1 */
        ADC_startConversion(ADC1, 0, 1);

        /* Read ADC conversion result */
        var = ADC_readData(ADC1);

        /* If ADC result exceeds threshold (1000), turn on LED; otherwise, turn it off */
        if(var > 1000)
            LED_turnOn(&red);
        else
            LED_turnOff(&red);
    }
}
