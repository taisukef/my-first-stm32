#include "stddef.h"
#include "vectors.h"
#include "stm32f0.h"
#include "core_cm0.h"
#include "c_init.h"

void Reset_Handler(void) {
	c_init();
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA;
	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP;
	RCC->APB1ENR |= APB1ENR_TIM3;
	
	// for debug
	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashに切り替え // リリース時には不要
	
	// PB1 setting for TIM3_CH4
	GPIOB->MODER |= 2 << (2 * 1); // PB1 as alternate
	GPIOB->AFRL |= (1 << (4 * 1)); // PB1 - AF1 (TIM3_CH4)
	
	// TIM3 PWM CH4(PB1)
	TIM3->CCER = 1 << 12; // ch4 enable
	TIM3->CCMR2 = 0b110 << 12; // ch4, OCxM ビットに“110”(PWM モード 1)や“111”(PWM モード 2)
	TIM3->PSC = 8000 - 1; // 8MHz / 8000 = 1kHz
	TIM3->ARR = 1000; // 1000msec = 1sec
	TIM3->CCR4 = 500; // duty 50%, 500msec
	TIM3->CR1 = 1; // enable count
	
	for (;;) {
		__WFI();
	}
}
