#include "stddef.h"
#include "vectors.h"
#include "stm32f1.h"
#include "c_init.h"

static int WAIT_CNT = 100000;

void Reset_Handler(void) {
	c_init();
	
	RCC->APB2ENR |= APB2ENR_IOPC;
	
	// for debug
//	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP; // ない？？
//	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashに切り替え // なくても大丈夫
	
	// DISCORVERY
	// LD3 左LED - PC9
	// LD4 右LED - PC8
			
	GPIOC->CRH = (GPIOC->CRH & ~((0xf << (4 * (8 - 8))) | (0xf << (4 * (9 - 8))))) | (2 << (4 * (8 - 8))) | (2 << (4 * (9 - 8))); // PC8, PC9 as OUTPUT
	
	for (;;) {
		GPIOC->ODR |= 1 << 8; // PC8 on
		GPIOC->ODR &= ~(1 << 9); // PC9 off
		
		for (volatile int i = 0; i < WAIT_CNT; i++);
		
		GPIOC->ODR |= 1 << 9; // PC9 on
		GPIOC->ODR &= ~(1 << 8); // PC8 off
		
		for (volatile int i = 0; i < WAIT_CNT; i++);
	}
}
