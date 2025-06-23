/*
  	This C code is designed to blink an LED connected to pin PA0 of the microcontroller using Timer 2 and interrupts.
  	Here's a concise explanation of the core idea and how it works:

	🧠 Core Idea:
	- The program initializes GPIO pin PA0 as an output for the LED.
	- It configures Timer 2 to generate an interrupt every 1000 milliseconds (1 second).
	- When the timer interrupt triggers, it executes the `func()` callback.
	- The `func()` function turns the LED ON during the first interrupt, OFF during the second,
	  and disables the timer and its interrupt during the third.
	- This approach shows how general-purpose timers (like TIM2) can be used with interrupts
	  to control peripherals in a non-blocking and precise timing manner.
*/

#include "../MCAL/RCC/RCC_Interface.h"        // RCC interface: used to initialize and enable clocks for peripherals
#include "../MCAL/TIMERS/TIMER_Interface.h"   // Timer interface: provides timer configuration, start, stop, interrupt management
#include "../MCAL/NVIC/NVIC_Interface.h"      // NVIC interface: enables interrupts at the CPU level
#include "../ECUAL/LED/LED_Interface.h"       // LED abstraction layer: provides functions to control LEDs

// Create a red LED object connected to GPIOA pin 0
LED_t red = {GPIOA, 0};

// Counter variable to track LED state changes
int counter = 0;

// Callback function executed when timer interrupt fires
void func(void)
{
	/* First timer interrupt: turn ON the LED */
	if(counter == 0)
	{
		LED_turnOn(&red);
		counter++;
	}
	/* Second timer interrupt: turn OFF the LED */
	else if(counter == 1)
	{
		LED_turnOff(&red);
		counter++;
	}
	/* Third timer interrupt: disable the timer and its interrupt */
	else if(counter == 2)
	{
		TIMER_disableInterrupt_and_Counter(TIM2); // Stops timer and disables further interrupts
	}
}

int main(void)
{
	RCC_init();					  // Initialize the system clock and enable peripheral clocks
	RCC_Peri_Enable(GPIOA_EN);    // Enable clock for GPIOA (for LED)
	RCC_Peri_Enable(TIM2EN);      // Enable clock for Timer 2

	// Initialize Timer 2 and the LED
	TIMER_init(TIM2);             // Configure Timer 2
	LED_init(&red);               // Configure GPIO pin as output for LED

	// Enable Timer 2 interrupt in the NVIC
	NVIC_EnableIRQn(NVIC_IRQ_TIM2);
	// Enable the timer interrupt at the peripheral level
	TIMER_enableInterrupt(TIM2);

	/*
		Start Timer 2 in interrupt mode with:
		- 1000 ms interval (1 second)
		- Callback function 'func' to be called on each interrupt 2 as the number of repetitions (internal use for tracking, if applicable)
	*/
	TIMER_voidStartCountMilliSeconds(TIM2, 1000, func, 2);

	// Infinite loop to keep the main function alive
	while(1);
}
