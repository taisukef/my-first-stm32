#include "stddef.h"
#include "vectors.h"
#include "stm32f0.h"
#include "core_cm0.h"
#include "c_init.h"

void Reset_Handler(void) {
	c_init();
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA;
	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP | APB2ENR_SPI1;
	
	// for debug
	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashに切り替え // リリース時には不要
	
	// test PA7
//	GPIOA->MODER |= 1 << (2 * 7); // PA7 as output
//	GPIOA->ODR = 1 << 7;
//	for (;;);
	
	// PA7(AF0) setting for MOSI
	GPIOA->MODER |= 2 << (2 * 7); // PA7 as alternate
	GPIOA->AFRL |= (0 << (4 * 7)); // PB7 - AF0 (MOSI)
	
	// SPI1
	SPI1->CR1 = (1 << 2) | (0b111 << 3); // BIDIOE, master, baud rate 0b111(f/256) (defalut:MSB)
//	SPI1->CR1 = (1 << 2) | (0b000 << 3); // BIDIOE, master, baud rate 0b111(f/2) (defalut:MSB)
	SPI1->CR2 = (1 << 2) | ((8 - 1) << 8); // SPI_CR2_SSOE, 8bit(4-16bit) (default:SPI motorola)
	SPI1->CR1 |= 1 << 6; // SPI1 enable
	
	for (;;) {
		while (!(SPI1->SR & (1 << 1))); // TXE=1:empty
		SPI1->DR = 0x1;
		
		for (int i = 0; i < 100; i++) {
			while (!(SPI1->SR & (1 << 1))); // TXE=1:empty
			SPI1->DR = 0x00;
		}
	}
}
