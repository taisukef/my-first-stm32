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

void EXTI0_1_IRQHandler(void) {
//	if ((EXTI->IMR & (1 << 0)) && (EXTI->PR & (1 << 0))) {
	if (EXTI->PR & (1 << 0)) {
		
		led_blink();
		
		EXTI->PR |= 1 << 0; // 割り込みペンディングリセット
	}
}


void Reset_Handler(void) {
	c_init();
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA;
	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP;
	
	// フラッシュ書き込みから起動のとき割り込みが効かない問題
	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashにすればok! // リリース時には不要
	
	led_init();
	/*
	int n = SYSCFG->CFGR1 & 0b11; // flash書き込みからの時は1、resetからは0
	for (int i = 0; i <= n; i++)
		led_blink();
	*/
	
	// PA0 入力
	GPIOA->PUPDR |= 2 << 0; // PA0 pull down
	
	// PB0 割り込み
	EXTI->IMR = 1;
	EXTI->RTSR |= 1; // 立ち上がり
	EXTI->FTSR |= 1; // 立ち下がり
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI0_1_IRQn, 2);
	
//	led_blink();
	EXTI->SWIER |= 1; // 割り込みかけてみる
	
	for (;;) {
		__WFI();
	}
}
