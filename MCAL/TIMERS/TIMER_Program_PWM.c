#include "TIMER_Interface.h"

void TIMER_generatePWM(TIMER_RegDef_t *Timer, uint8_t channel, uint32_t periode, uint32_t duty_cycle)
{
    uint8_t offset = (channel - 1) * 4;

    // Configure CCMRx registers
    switch (channel)
    {
        case 1:
            Timer->CCMR1 &= ~(0b111 << 4);
            Timer->CCMR1 |= (PWM_MODE << 4);
            Timer->CCMR1 |= (1 << 3);  // OC1PE
            Timer->CCR1 = duty_cycle;
            break;

        case 2:
            Timer->CCMR1 &= ~(0b111 << 12);
            Timer->CCMR1 |= (PWM_MODE << 12);
            Timer->CCMR1 |= (1 << 11); // OC2PE
            Timer->CCR2 = duty_cycle;
            break;

        case 3:
            Timer->CCMR2 &= ~(0b111 << 4);
            Timer->CCMR2 |= (PWM_MODE << 4);
            Timer->CCMR2 |= (1 << 3);  // OC3PE
            Timer->CCR3 = duty_cycle;
            break;

        case 4:
            Timer->CCMR2 &= ~(0b111 << 12);
            Timer->CCMR2 |= (PWM_MODE << 12);
            Timer->CCMR2 |= (1 << 11); // OC4PE
            Timer->CCR4 = duty_cycle;
            break;

        default:
            return; // Invalid channel
    }

    // Clear both CCxE and CCxP before setting
    Timer->CCER &= ~(0b11 << offset);                 // Clear enable and polarity
    Timer->CCER |= (1 << offset);                     // Enable output
    Timer->CCER |= (POLARITY << (offset + 1));        // Set polarity

    Timer->ARR = periode - 1;
    Timer->EGR |= (1 << 0);   // UG: update registers immediately
    Timer->CR1 |= (1 << 0);   // CEN = 1: start the timer
}
