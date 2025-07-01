/*
 * GPIO_Interface.h
 *
 *  Created on: May 6, 2025
 *      Author: Ahmed
 */

#ifndef GPIO_GPIO_INTERFACE_H_
#define GPIO_GPIO_INTERFACE_H_

#include "GPIO_Registers.h"

/* ============================== */
/*     ENUMS FOR CONFIGURATION    */
/* ============================== */

/* GPIO Pin Numbers */
typedef enum {
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15
} GPIO_Pin_Number_t;

/* Pin Modes */
typedef enum {
	GPIO_INPUT,
	GPIO_GENERAL_OUTPUT_PURPOSE,
	GPIO_ALTERNATE_FUNCTION,
	GPIO_ANALOG
} GPIO_Mode_t;

/* Output Types */
typedef enum {
	GPIO_PUSH_PULL,
	GPIO_OPEN_DRAIN
} GPIO_Output_Type_t;

/* Output Speeds */
typedef enum {
	GPIO_LOW_SPEED,
	GPIO_MEDUIM_SPEED,
	GPIO_HIGH_SPEED,
	GPIO_VERY_HIGH_SPEED
} GPIO_Speed_t;

/* Pull-up/Pull-down Config */
typedef enum {
	GPIO_NO_PULL_UP_NO_PULL_DOWN,
	GPIO_PULL_UP,
	GPIO_PULL_DOWN
} GPIO_Pull_Up_Pull_Down_t;

/* Output Write Logic Level */
typedef enum {
	GPIO_RESET,
	GPIO_SET
} GPIO_Output_Data_t;

/* Alternate Function Mapping (AF0 - AF15) */
typedef enum {
	GPIO_AF0,
	GPIO_AF1,
	GPIO_AF2,
	GPIO_AF3,
	GPIO_AF4,
	GPIO_AF5,
	GPIO_AF6,
	GPIO_AF7,
	GPIO_AF8,
	GPIO_AF9,
	GPIO_AF10,
	GPIO_AF11,
	GPIO_AF12,
	GPIO_AF13,
	GPIO_AF14,
	GPIO_AF15
} GPIO_Alternate_Function_t;

/* ============================== */
/*     GPIO CONTROL FUNCTIONS     */
/* ============================== */

/**
 * @brief Set the mode (Input, Output, AF, Analog) for a specific pin.
 */
void GPIO_Mode(GPIO_RegDef_t *GPIO, GPIO_Pin_Number_t N, GPIO_Mode_t M);

/**
 * @brief Configure the output type (Push-Pull or Open-Drain).
 */
void GPIO_Output_Type(GPIO_RegDef_t *GPIO, GPIO_Pin_Number_t N, GPIO_Output_Type_t O);

/**
 * @brief Set the output speed of a GPIO pin.
 */
void GPIO_Speed(GPIO_RegDef_t *GPIO, GPIO_Pin_Number_t N, GPIO_Speed_t S);

/**
 * @brief Configure internal pull-up or pull-down resistors.
 */
void GPIO_Pull_Up_Pull_Down(GPIO_RegDef_t *GPIO, GPIO_Pin_Number_t N, GPIO_Pull_Up_Pull_Down_t P);

/**
 * @brief Read the input value from a GPIO pin.
 */
uint32_t GPIO_Input(GPIO_RegDef_t *GPIO, GPIO_Pin_Number_t N);

/**
 * @brief Set or reset the output value of a GPIO pin.
 */
void GPIO_Output_Data(GPIO_RegDef_t *GPIO, GPIO_Pin_Number_t N, GPIO_Output_Data_t O);

/**
 * @brief Assign alternate function number to a GPIO pin.
 */
void GPIO_Alternate_Function(GPIO_RegDef_t *GPIO, GPIO_Pin_Number_t N, GPIO_Alternate_Function_t F);

/* ============================== */
/*     HIGH-LEVEL UTILITIES       */
/* ============================== */

/**
 * @brief Configures a GPIO pin for PWM output (Alternate Function mode).
 *
 * @param port GPIO port (e.g., GPIOA)
 * @param pin Pin number (0–15)
 * @param af Alternate function number (AF0–AF15)
 */
void GPIO_configurePWM(GPIO_RegDef_t *port, uint8_t pin, uint8_t af);

/**
 * @brief Configures a GPIO pin for analog input mode.
 *
 * @param port GPIO port (e.g., GPIOA)
 * @param pin Pin number (0–15)
 */
void GPIO_configureAnalogInput(GPIO_RegDef_t *port, uint8_t pin);

#endif /* GPIO_GPIO_INTERFACE_H_ */
