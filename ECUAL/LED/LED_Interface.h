/*
 * LED_Interface.h
 *
 *  Created on: May 21, 2025
 *      Author: Ahmed
 */

#ifndef LED_LED_INTERFACE_H_
#define LED_LED_INTERFACE_H_

#include "../MCAL/GPIO/GPIO_Interface.h"
#include "../MCAL/SYSTICK/SYSTICK_Interface.h"

/* Struct representing a LED configuration. */
typedef struct{
	GPIO_RegDef_t *port;
	uint8_t pinNumber;
}LED_t;

void LED_init(LED_t *led);
void LED_turnOn(LED_t *led);
void LED_turnOff(LED_t *led);
/* To use the LED_blink() function, you must first initialize the SYSTICK peripheral by calling the following function: SYSTICK_init(); */
void LED_blink(LED_t *led, uint32_t milliseconds);


#endif /* LED_LED_INTERFACE_H_ */
