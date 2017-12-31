#include "stddef.h"
#include "vectors.h"
#include "stm32f0.h"

extern unsigned int __data_section_table;

static inline void c_init() {
	// clear RAM
	memclear((uint8*)SRAM_LOC, SRAM_LEN);
	
	// copy init value to RAM
	uint* ad = &__data_section_table;
	memcopy((uint8*)SRAM_LOC, (uint8*)ad[0], ad[1]);
}

static int WAIT_CNT = 100000;

void Reset_Handler(void) {
	c_init();
	
	RCC->AHBENR |= 1 << 18; // GPIO on
	
	GPIOB->MODER |= 1 << (2 * 1); // PB1 as OUTPUT
	
	for (;;) {
		GPIOB->ODR = 1 << 1; // PB1 on
		
		for (volatile int i = 0; i < WAIT_CNT; i++);
		
		GPIOB->ODR = 0 << 1;
		
		for (volatile int i = 0; i < WAIT_CNT; i++);
	}
}
