/*
 * UART_Interface.h
 *
 *  Created on: Jun 30, 2025
 *      Author: Ahmed
 *
 *  Description:
 *  Header file for UART driver interface.
 *  Declares the main functions for UART initialization,
 *  data transmission, reception, and sending strings.
 */

#ifndef UART_UART_INTERFACE_H_
#define UART_UART_INTERFACE_H_

// Include the UART register definitions (hardware registers struct)
#include "UART_Registers.h"

// Include the UART configuration header (macros like baud rate, word length, etc.)
#include "UART_Config.h"

/**
 * @brief Initializes the UART peripheral with the specified baud rate.
 * @param UART: Pointer to the UART peripheral base address (e.g., USART1).
 * @param baud_rate: The desired baud rate (e.g., 9600, 115200).
 */
void UART_init(UART_RegDef_t *UART, uint16_t baud_rate);

/**
 * @brief Sends a single byte of data via UART.
 * @param UART: Pointer to the UART peripheral base address.
 * @param data: 8-bit data byte to transmit.
 */
void UART_sendData(UART_RegDef_t *UART, uint8_t data);

/**
 * @brief Receives a single byte of data via UART (polling mode).
 * @param UART: Pointer to the UART peripheral base address.
 * @return The received 8-bit data byte.
 */
uint8_t UART_receiveData(UART_RegDef_t *UART);

/**
 * @brief Sends a null-terminated string via UART.
 * @param UART: Pointer to the UART peripheral base address.
 * @param str: Pointer to a null-terminated string to transmit.
 */
void UART_sendString(UART_RegDef_t *UART, char *str);

#endif /* UART_UART_INTERFACE_H_ */
