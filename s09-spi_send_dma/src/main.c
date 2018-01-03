#include "stddef.h"
#include "vectors.h"
#include "stm32f0.h"
#include "core_cm0.h"
#include "c_init.h"

void Reset_Handler(void) {
	c_init();
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA | AHBENR_DMA;
	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP | APB2ENR_SPI1;
	
	// for debug
	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashに切り替え // リリース時には不要
	
	// PA7 - AF0(MOSI)
	GPIOA->MODER |= 2 << (2 * 7); // PA7 as alternate
	GPIOA->AFRL |= (0 << (4 * 7)); // PB7 - AF0 (MOSI)
	
	char buf[256];
	int len = 256;
	for (int i = 0; i < len; i++) {
		buf[i] = i;
	}
	
	// DMA_CH3 - MOSI
	DMA->CCR3 |= (0b00 << 8) | (1 << 7) | (1 << 4); // peripheral8bit, MINC(mem increment), DIR:1(from mem)
	DMA->CPAR3 = (uint32)&SPI1->DR;
	DMA->CMAR3 = (uint32)buf;
	
	// SPI1
	SPI1->CR1 = (1 << 2) | (0b111 << 3); // master, baud rate 0b111(f/256) (defalut:MSB)
//	SPI1->CR1 = (1 << 2) | (0b000 << 3); // master, baud rate 0b111(f/2) (defalut:MSB)
	SPI1->CR2 = ((8 - 1) << 8) | (1 << 2) | (1 << 1); // 8bit(4-16bit) (default:SPI motorola), SSOE, TXDMAEN
	
	SPI1->CR1 |= 1 << 6; // SPI1 enable
	
	for (int i = 0; i < 100; i++) { // 100回送信
		DMA->CCR3 &= ~(1 << 0); // disable
		DMA->CNDTR3 = len; // 最大65535
		DMA->CCR3 |= (1 << 0); // enable
		while (!(DMA->ISR & (1 << 9))); // TCIF3ch=1:sent
		DMA->IFCR |= 1 << 9;
	}
	
	// test PA7
	GPIOA->MODER = (GPIOA->MODER & ~(3 << (2 * 7))) | (1 << (2 * 7)); // PA7 as output
	GPIOA->ODR = 1 << 7;
	
	for (;;);
}
