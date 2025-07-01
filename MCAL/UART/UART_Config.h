/*
 * UART_Config.h
 *
 *  Created on: Jun 30, 2025
 *      Author: Ahmed
 */

#ifndef UART_UART_CONFIG_H_
#define UART_UART_CONFIG_H_

// Word length: 0 = 8 bits, 1 = 9 bits
#define DATA_BITS_8         0
#define DATA_BITS_9         1
#define UART_WORD_LENGTH    DATA_BITS_8

// Stop bits (used in CR2 STOP[13:12])
#define STOP_1              0b00  // 1 stop bit
#define STOP_0_5            0b01  // 0.5 stop bit
#define STOP_2              0b10  // 2 stop bits
#define STOP_1_5            0b11  // 1.5 stop bit
#define UART_STOP           STOP_1

// Oversampling: 0 = by 16, 1 = by 8 (CR1 OVER8)
#define UART_OVERSAMPLING_16    0
#define UART_OVERSAMPLING_8     1
#define UART_OVERSAMPLING       UART_OVERSAMPLING_8

#endif /* UART_UART_CONFIG_H_ */
