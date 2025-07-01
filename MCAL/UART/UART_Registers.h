/*
 * UART_Registers.h
 *
 *  Created on: Jun 30, 2025
 *      Author: Ahmed
 */

#ifndef UART_UART_REGISTERS_H_
#define UART_UART_REGISTERS_H_

#include <stdint.h>

/**
 * @brief USART register definition structure
 * Covers both USARTx and UARTx peripherals (asynchronous and synchronous modes).
 */
typedef struct
{
    volatile uint32_t SR;     // Status Register: TXE, RXNE, TC, ORE, NE, FE, PE
    volatile uint32_t DR;     // Data Register: read = received data, write = data to transmit
    volatile uint32_t BRR;    // Baud Rate Register: contains USARTDIV (mantissa + fraction)
    volatile uint32_t CR1;    // Control Register 1: UE, TE, RE, RXNEIE, TXEIE, M, PCE, etc.
    volatile uint32_t CR2;    // Control Register 2: STOP bits, LIN mode, CLKEN, CPOL, CPHA, etc.
    volatile uint32_t CR3;    // Control Register 3: CTS/RTS enable, DMAT, DMAR, error interrupts
    volatile uint32_t GTPR;   // Guard Time and Prescaler Register: used in Smartcard/IrDA mode
}UART_RegDef_t;

/*
 * @brief Base addresses for USART/UART peripherals (STM32F4 family)
 */
#define UART1  	((UART_RegDef_t*)(0x40011000U))
#define UART2  	((UART_RegDef_t*)(0x40004400U))
#define UART3  	((UART_RegDef_t*)(0x40004800U))
#define UART4   ((UART_RegDef_t*)(0x40004C00U))
#define UART5   ((UART_RegDef_t*)(0x40005000U))
#define UART6  	((UART_RegDef_t*)(0x40011400U))

#endif /* UART_UART_REGISTERS_H_ */
