/*
 * NVIC_Registers.h
 *
 *  Created on: May 28, 2025
 *      Author: Ahmed
 */

#ifndef NVIC_NVIC_REGISTERS_H_
#define NVIC_NVIC_REGISTERS_H_

#include <stdint.h>
#define NVIC_BASEADDR			0xE000E100U

typedef struct{
	 uint32_t ISER[8];			/* Interrupt set-enable register x.						 Address offset:  0x100 + 0x04 * x, (x = 0 to 7) */
	 uint32_t RESERVED0[24];	/* Reserved space.						 				 Address offset:  0x120 - 0x17C */
	 uint32_t ICER[8];			/* Interrupt clear-enable register x.					 Address offset:  0x180 + 0x04 * x, (x = 0 to 7) */
	 uint32_t RESERVED1[24];	/* Reserved space.						 				 Address offset:  0x1A0 - 0x1FC */
	 uint32_t ISPR[8];			/* Interrupt set-pending register x.					 Address offset:  0x200 + 0x04 * x, (x = 0 to 7) */
	 uint32_t RESERVED2[24];	/* Reserved space.										 Address offset:  0x220 - 0x27C */
	 uint32_t ICPR[8];			/* Interrupt clear-pending register x.					 Address offset:  0x280 + 0x04 * x, (x = 0 to 7) */
	 uint32_t RESERVED3[24];	/* Reserved space.										 Address offset:  0x2A0 - 0x2FC */
	 uint32_t IABR[8];			/* Interrupt active bit register x.					 	 Address offset:  0x300 + 0x04 * x, (x = 0 to 7) */
	 uint32_t RESERVED4[56];	/* Reserved space.										 Address offset:  0x320 - 0x3FC */
	 uint8_t IPR[240]; 			/* The NVIC has 60 IPR (Interrupt Priority Register) entries. Each 32-bit IPR register holds priority for 4 interrupts, since each interrupt takes 8 bits.
								   That covers 60 * 4 = 240 interrupts in total (maximum supported by NVIC = 240 interrupts). To simplify access to each interrupt's priority individually,
								   this can alternatively be declared as:
								   uint8_t IPR[240]; This way, each element (elements of indexing) directly corresponds to one interrupt's priority byte.
	 	 	 	 	 	 	 	 */

}NVIC_RegDef_t;

#define NVIC					((NVIC_RegDef_t*)(NVIC_BASEADDR))


#endif /* NVIC_NVIC_REGISTERS_H_ */
