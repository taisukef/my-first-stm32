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

void changeClockTo48MHz() {
	// 48MHz = 8MHz * 6(PLLMUL)
	FLASH->ACR |= 0b10001; // 1<<4:enable prefetch, 1 wait state
	RCC->CFGR = (RCC->CFGR & ~(0b1111 << 18)) | (4 << 18); // PLLMUL6
	RCC->CR |= 1 << 24; // PLLON
	while (RCC->CR & (1 << 25));
	RCC->CFGR |= 2; // PLL
	while ((RCC->CFGR & 12) != 8);
}

static int WAIT_CNT = 100000;

void Reset_Handler(void) {
	changeClockTo48MHz();
	
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
