/*
 * UART_Program.c
 *
 *  Created on: Jun 30, 2025
 *      Author: Ahmed
 */

#include "UART_Interface.h"

/**
 * @brief  Initializes the UART peripheral with a given baud rate.
 *         Supports both oversampling by 16 (default) and 8.
 * @param  UART: Pointer to the UART peripheral structure.
 * @param  baud_rate: Desired baud rate (e.g., 9600, 115200).
 */
void UART_init(UART_RegDef_t *UART, uint16_t baud_rate)
{
    // Enable USART (CR1 bit 13: UE = 1)
    UART->CR1 |= (1 << 13);

    // Set word length (M = 0 for 8-bit, M = 1 for 9-bit) — CR1 bit 12
    UART->CR1 |= (UART_WORD_LENGTH << 12);

    // Set stop bits (STOP[13:12] in CR2)
    UART->CR2 &= ~(0b11 << 12);          // Clear existing STOP bits
    UART->CR2 |= (UART_STOP << 12);      // Apply configured STOP bits

    /*
        * ----------------------------------------------------------
        * ⚙️ Oversampling Mode (OVER8 Bit – CR1[15])
        * ----------------------------------------------------------
        * STM32 USART supports two oversampling modes:
        *
        * 1. Oversampling by 16 (OVER8 = 0) → Default
        *    - Higher precision in baud rate generation
        *    - Formula: USARTDIV = fCK / (16 × baud_rate)
        *    - BRR layout:
        *        Bits [15:4] = mantissa
        *        Bits [3:0]  = fraction (range 0–15)
        *
        * 2. Oversampling by 8 (OVER8 = 1)
        *    - Doubles the baud sampling rate
        *    - Formula: USARTDIV = fCK / (8 × baud_rate)
        *    - BRR layout:
        *        Bits [15:4] = mantissa
        *        Bits [3:1]  = fraction (range 0–7, shifted left)
        *        Bit [0]     = always 0
        *
        * In both modes:
        * - The mantissa is the integer part of USARTDIV
        * - The fraction is the scaled decimal part
        */

    float usartdiv;       // Holds the USART divider value (baud rate calculation)
    uint32_t mantissa;    // Integer part of the USART divider
    uint32_t fraction;    // Fractional part of the USART divider

	#if UART_OVERSAMPLING == UART_OVERSAMPLING_8

		// Set OVER8 = 1 → oversampling by 8
		UART->CR1 |= (1 << 15);

		// USARTDIV = fCK / (8 × baud_rate)
		usartdiv = 16000000.0f / (8 * baud_rate);
		mantissa = (uint32_t)usartdiv;
		fraction = (uint32_t)((usartdiv - mantissa) * 8);

		/*
		 * BRR packing in oversampling by 8:
		 * - mantissa is stored in bits [15:4]
		 * - fraction is stored in bits [3:1] → shift left by 1
		 * - BRR[0] must always be 0
		 */
		UART->BRR = (mantissa << 4) | ((fraction & 0x7) << 1);

	#else

		// Set OVER8 = 0 → oversampling by 16
		UART->CR1 &= ~(1 << 15);

		// USARTDIV = fCK / (16 × baud_rate)
		usartdiv = 16000000.0f / (16 * baud_rate);
		mantissa = (uint32_t)usartdiv;
		fraction = (uint32_t)((usartdiv - mantissa) * 16);

		/*
		 * BRR packing in oversampling by 16:
		 * - mantissa in bits [15:4]
		 * - fraction in bits [3:0]
		 */
		UART->BRR = (mantissa << 4) | (fraction & 0xF);

	#endif
}

/**
 * @brief  Sends a single byte of data through the UART.
 * @param  UART: Pointer to the UART peripheral structure.
 * @param  data: 8-bit data to transmit.
 */
void UART_sendData(UART_RegDef_t *UART, uint8_t data)
{
    // Enable transmitter (TE bit = 1 in CR1, bit 3)
    UART->CR1 |= (1 << 3);

    // Wait until TXE (Transmit Data Register Empty) = 1
    // TXE is bit 7 in USART_SR
    while (!((UART->SR >> 7) & 1));

    // Write the data byte to the data register (maps to internal TDR)
    UART->DR = data;

    // Wait until TC (Transmission Complete) = 1
    // TC is bit 6 in USART_SR; ensures stop bit has also been transmitted
    while (!((UART->SR >> 6) & 1));
}

/**
 * @brief  Receives a single byte of data through the UART (polling mode).
 * @param  UART: Pointer to the UART peripheral structure.
 * @return 8-bit received data.
 */
uint8_t UART_receiveData(UART_RegDef_t *UART)
{
    // Enable receiver (RE bit = 1 in CR1, bit 2)
    UART->CR1 |= (1 << 2);

    // Wait until RXNE (Receive Data Register Not Empty) = 1
    // RXNE is bit 5 in USART_SR
    while (!((UART->SR >> 5) & 1));

    // Read and return the received data byte from the data register (clears RXNE)
    // Mask with 0xFF to keep only the lowest 8 bits of the DR register
    return (uint8_t)(UART->DR & 0xFF);
}

/**
 * @brief Sends a null-terminated string via UART.
 * @param UART: Pointer to UART peripheral.
 * @param str: Pointer to null-terminated string.
 */
void UART_sendString(UART_RegDef_t *UART, char *str)
{
    while (*str)
    {
        UART_sendData(UART, (uint8_t)(*str));
        str++;
    }
}

/*
 * ============================================
 * 📘 STM32 USART Transmitter – Full Explanation
 * Based on Section 30.6.2 from RM0090
 * ============================================
 *
 * ✅ Core Idea:
 * The USART transmitter sends serial data over the TX pin by:
 * - Receiving bytes from software (via USART_DR → TDR)
 * - Shifting them out bit-by-bit using a shift register
 * - Following UART framing: Start → Data → (Optional Parity) → Stop bits
 *
 * ⚙️ Key Registers:
 * - USART_DR: Write-only register to send data (feeds the TDR)
 * - USART_SR: Status register with TXE, TC, etc.
 * - USART_CR1: Control register (includes TE = Transmitter Enable)
 */

/*
 * ---------------------------------------------------
 * 🧱 Block Diagram – USART Transmitter Flow Overview:
 * ---------------------------------------------------
 *
 *          +--------------------+
 *          |      CPU / DMA     |
 *          +--------------------+
 *                    |
 *                    v
 *          +--------------------+
 *          | USART_DR / TDR     |  ← Your software writes here
 *          +--------------------+
 *                    |
 *             TXE = 1 → Ready
 *             TXE = 0 → Busy
 *                    |
 *                    v
 *          +--------------------+
 *          |  Shift Register    |  ← Serializes data
 *          +--------------------+
 *                    |
 *                    v
 *         +-------------------------+
 *         | TX Pin (bit-by-bit out) |
 *         +-------------------------+
 *
 *     ^              |
 *     |              v
 *  Baud Rate     Framing Logic
 *  Generator     (Start, Parity, Stop)
 */

/*
 * ---------------------------------------
 * 🧱 Transmitter Block Architecture:
 * ---------------------------------------
 *
 * [CPU or DMA]
 *     ↓
 * USART_DR (mapped to internal TDR)
 *     ↓       TXE = 0 → Busy (TDR Full)
 *             TXE = 1 → Ready for new byte
 * Shift Register
 *     ↓       Bit-by-bit serial output
 * TX Pin (physical line)
 */

/*
 * -----------------------------------------------------
 * 📦 TDR vs. Shift Register (What’s the difference?)
 * -----------------------------------------------------
 * - TDR = Transmit Data Register
 *   → Receives full byte from your code (via USART_DR)
 *
 * - Shift Register = Serial engine
 *   → Takes data from TDR and shifts out bits (LSB first by default)
 *
 * ✅ Think of TDR as a mailbox, and the shift register as the mailman.
 * The TXE flag says whether the mailbox is empty or not.
 */

/*
 * -----------------------------------------------------
 * 🧠 TXE (Transmit Data Register Empty)
 * -----------------------------------------------------
 *
 * Bit 7 of USART_SR (Status Register)
 *
 * Meaning:
 * - TXE = 1 → TDR is empty → You can write a new byte (to USART_DR)
 * - TXE = 0 → TDR is still holding a byte → Wait before writing again
 *
 * ✅ TXE does not "send" the data. It simply tells you:
 *    "The last byte you gave me has been moved to the shift register.
 *     I’m ready for the next one."
 *
 * ⚠️ Writing to USART_DR when TXE = 0 could result in data corruption.
 */

/*
 * -----------------------------------------------------
 * ⏱️ TC (Transmission Complete)
 * -----------------------------------------------------
 *
 * Bit 6 of USART_SR (Status Register)
 *
 * - TC = 1 → Entire frame (including stop bit) has been shifted out
 * - Useful when you want to wait until the *last* bit has left the TX pin
 */

/*
 * -----------------------------------------------------
 * 🔧 TE (Transmitter Enable)
 * -----------------------------------------------------
 *
 * Bit 3 of USART_CR1
 *
 * - TE = 1 → Enables the TX functionality
 * - TE = 0 → Disables the transmitter
 *
 * 🧠 When TE is set (0 → 1) *during transmission*, it inserts an IDLE (preamble) frame
 *   after the current byte completes — except in Smartcard mode.
 *
 * 🔁 Re-setting TE triggers an idle line frame, which can be used to signal a new frame start.
 */

/*
 * -----------------------------------------------------
 * 🧭 IDLE LINE / PREAMBLE
 * -----------------------------------------------------
 *
 * If TE is toggled 0 → 1 while a transmission is in progress:
 * → After the current byte is done, USART sends an idle line (logic high) for 1+ bit-time.
 * → This idle line acts as a "preamble" between transmissions.
 * → Useful for framing or synchronization.
 *
 * ❗This idle line is *not* sent in Smartcard mode (different protocol).
 */

/*
 * -----------------------------------------------------
 * 🧩 Example Code Snippet:
 * -----------------------------------------------------
 *
 * // Transmit one byte when USART is ready
 * while (!(USARTx->SR & (1 << TXE)));  // Wait for TXE = 1
 * USARTx->DR = 'A';                    // Send ASCII 'A'
 *
 * // Optional: Wait until transmission completes
 * while (!(USARTx->SR & (1 << TC)));   // Wait for TC = 1
 */

/*
 * -----------------------------------------------------
 * 🔄 Summary Table:
 * -----------------------------------------------------
 * | Component      | Role                                                |
 * |----------------|-----------------------------------------------------|
 * | TDR (via DR)   | Holds data to be sent next                          |
 * | Shift Register | Serializes byte from TDR and sends it via TX pin   |
 * | TXE (SR bit 7) | TDR is empty → Ready for next byte                  |
 * | TC (SR bit 6)  | Shift register is done → Last bit has been sent    |
 * | TE (CR1 bit 3) | Turns on/off the transmitter engine                 |
 * | Idle Line      | Sent after TE 0→1 during transmission              |
 */

/*
 * -----------------------------------------------------
 * 🧪 Analogy Recap:
 * -----------------------------------------------------
 * - USART_DR = Mail slot you drop the letter into
 * - TDR      = Mailbox holding letter to be sent
 * - ShiftReg = Mailman delivering bits
 * - TX Pin   = Delivery van
 * - TXE      = "Mailbox is empty" signal
 * - TC       = "Mail delivery is fully completed"
 */

/*
 * ============================================
 * ✅ You’re now fully equipped to understand:
 * - USART transmission timing
 * - TXE/TC status logic
 * - How TDR and shift register work together
 * - What idle line insertion (preamble) does
 * ============================================
 */

/*
 * ------------------------------
 * 📘 TXE (Transmit Data Register Empty)
 * ------------------------------
 *
 * TXE is bit 7 in the USART Status Register (USART_SR).
 * It tells you whether the USART is ready to accept new data for transmission.
 *
 * -------------------------------------------------------
 * TXE = 1  →  TDR (Transmit Data Register) is EMPTY.
 *            ✔️ You are allowed to write a new byte to USART_DR.
 *
 * TXE = 0  →  TDR is still FULL (hasn't been moved to shift register yet).
 *            ❌ You must WAIT before writing another byte.
 * -------------------------------------------------------
 */

/*
 * ------------------------------
 * 🧠 How it works under the hood:
 * ------------------------------
 *
 * [USART_DR] → internal TDR (Transmit Data Register)
 *      |
 *      ↓  [when shift register is ready]
 *  Shift Register ← loads data from TDR
 *      |
 *      ↓  [bit-by-bit transmission]
 *      TX Pin → sends serial bits
 *
 * When TDR is empty (data has moved to the shift register), TXE is set to 1.
 * When you write to USART_DR, TXE is cleared to 0.
 *
 * The TDR acts as a buffer for the next byte to be sent,
 * while the Shift Register is the component that actually transmits data out.
 */

/*
 * ---------------------------------------
 * 📦 Analogy: TDR vs Shift Register
 * ---------------------------------------
 * - TDR      = Mailbox → Where you drop your data (USART_DR)
 * - ShiftReg = Mailman → Delivers the bits out over TX pin
 * - TXE      = "Mailbox is empty" flag
 */

/*
 * ---------------------------------------
 * ✅ Correct Usage Pattern:
 * ---------------------------------------
 *
 * // Wait until USART is ready to accept new data
 * while (!(USARTx->SR & (1 << TXE)));  // Wait for TXE = 1
 * USARTx->DR = myData;                 // Write data to transmit
 *
 * Always check TXE before writing to avoid overwriting data.
 */

/*
 * ---------------------------------------
 * ⚠️ Common Misconception:
 * ---------------------------------------
 *
 * Some developers think that TXE sends the data.
 * ❌ That's not true.
 *
 * ✔️ TXE is just a signal — it tells you:
 *    "Hey, the previous byte has moved from TDR to the shift register.
 *     You can now load the next byte."
 */

/*
 * ------------------------------
 * 🧠 Summary:
 * ------------------------------
 * - TXE = 1 → USART ready for new data (TDR is empty)
 * - TXE = 0 → USART still busy; TDR not yet consumed
 * - You must not write to USART_DR if TXE = 0
 *
 * - TDR holds the next byte to be sent
 * - Shift Register sends bits serially (start → data → parity → stop)
 */

