#include "stddef.h"
#include "vectors.h"
#include "stm32f0.h"
#include "core_cm0.h"

extern unsigned int __data_section_table;

static inline void c_init() {
	// clear RAM
	memclear((uint8*)SRAM_LOC, SRAM_LEN);
	// copy init value to RAM
	uint* ad = &__data_section_table;
	memcopy((uint8*)SRAM_LOC, (uint8*)ad[0], ad[1]);
}

// LED

void led_init() {
	// GPIO
	GPIOB->MODER |= 1 << (2 * 1); // PB1 as OUTPUT
}
void led_blink() {
	int wait = 100000;
	GPIOB->ODR = 1 << 1; // PB1 on
	for (volatile int i = 0; i < wait; i++);
	GPIOB->ODR = 0 << 1;
	for (volatile int i = 0; i < wait; i++);
}
void led_invert() {
	GPIOB->ODR ^= 1 << 1; // PB1 xor
}

//

void TIM3_IRQHandler(void) {
	led_invert();
	
	TIM3->SR &= ~1;
}

void Reset_Handler(void) {
	c_init();
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA;
	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP;
	RCC->APB1ENR |= APB1ENR_TIM3;
	
	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashに切り替え // リリース時には不要
	
	led_init();
	
	// PA0 入力
	GPIOA->PUPDR |= 2 << 0; // PA0 pull down
	
	// TIM3
	TIM3->PSC = 8000 - 1; // 8MHz / 8000 = 1kHz
	TIM3->DIER = 1; // 更新割り込み enable
	TIM3->ARR = 1000; // 1000msec = 1sec
	TIM3->CR1 = 1; // enable count
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 2);
	
//	led_blink();
	
	for (;;) {
		__WFI();
	}
}
