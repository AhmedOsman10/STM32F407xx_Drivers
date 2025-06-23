/*
 * TIMERS_Config.h
 *
 *  Created on: Jun 12, 2025
 *      Author: Ahmed
 */

#ifndef TIMERS_TIMERS_CONFIG_H_
#define TIMERS_TIMERS_CONFIG_H_

/*
 * This driver uses the internal clock source (16 MHz).
 * The timer prescaler is set to 15999, reducing the timer tick frequency to 1 kHz (1 ms per tick). This enables simple and accurate millisecond-based delays.
 */
#define TIMER_CLOCK_PRESCALER     15  // Prescaler for 1 ms tick with 16 MHz clock

#endif /* TIMERS_TIMERS_CONFIG_H_ */
