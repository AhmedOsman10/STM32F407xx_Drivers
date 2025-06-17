/*
 * Description:
 * This program initializes a button connected to pin PE3 and an LED connected to pin PA1.
 * When the button is pressed (triggering EXTI line 3), an interrupt is generated, and the LED blinks for 700 milliseconds.
 */

#include "../MCAL/RCC/RCC_Interface.h"        /* RCC peripheral interface */
#include "../MCAL/EXTI/EXTI_Interface.h"      /* External interrupt interface */
#include "../MCAL/SYSCFG/SYSCFG_Interface.h"  /* SYSCFG for EXTI line mapping */
#include "../MCAL/NVIC/NVIC_Interface.h"      /* NVIC interrupt controller */
#include "../ECUAL/LED/LED_Interface.h"       /* LED control interface */
#include "../ECUAL/Button/Button_Interface.h" /* Button control interface */

/* Interrupt callback function declaration */
void Function(void);
/* Define LED connected to GPIOA pin 1 */
LED_t LED = {GPIOA, 1};

int main(void)
{
    /* Initialize system clock */
    RCC_init();
    /* Initialize SysTick timer for delay functions */
    SYSTICK_init();

    /* Enable required peripheral clocks */
    RCC_Peri_Enable(SYSCFGEN);  /* Enable SYSCFG for EXTI configuration */
    RCC_Peri_Enable(GPIOA_EN);  /* Enable GPIOA (used for LED) */
    RCC_Peri_Enable(GPIOE_EN);  /* Enable GPIOE (used for Button) */
    NVIC_EnableIRQn(NVIC_IRQ_EXTI3);   /* Enable NVIC interrupt for EXTI line 3 */

    /* Configure button on PE3 with internal pull-up resistor */
    Button_t btn = {GPIOE, 3, GPIO_PULL_UP};
    Button_init(&btn);
    /* Initialize the LED on PA1 */
    LED_init(&LED);

    /* Map EXTI line 3 to GPIOE pin 3 */
    SYSCFG_enableEXTILineX(SYSCFG_GPIOE, 3);

    /* Enable EXTI line 3 interrupt:
     * - Rising edge: disabled
     * - Falling edge: enabled
     * - Callback function: Function()
     */
    EXTI_enableInterruptHardware(3, DISABLE, ENABLE, Function);

    /* Infinite loop; waits for interrupt */
    while (1);
}

/* Function:
 * Callback executed when EXTI line 3 is triggered (button press).Blinks the LED on PA1 for 700 milliseconds.
 */
void Function(void)
{
    LED_blink(&LED, 700);
}
