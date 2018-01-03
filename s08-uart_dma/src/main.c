#include "stddef.h"
#include "vectors.h"
#include "stm32f0.h"
#include "core_cm0.h"
#include "c_init.h"

void changeClockTo48MHz() { // auto
	// 48MHz = external 12MHz x 4
	FLASH->ACR |= 0b10001; // 1<<4:enable prefetch, 1 wait state
	
	RCC->CR |= 1 << 16; // HSEON
	int timeout = 10000;
	while (!(RCC->CR & (1 << 17))) { // wait HSERDY
		timeout--;
		if (!timeout)
			break;
	}
	if (!timeout) { // IRC
		RCC->CFGR = (RCC->CFGR & ~(0b1111111 << 15)) | ((1 << 15) | ((6 - 2) << 18)); // PLL=HSI/PREDIV, PLLMUL6
	} else {
		RCC->CFGR = (RCC->CFGR & ~(0b1111111 << 15)) | ((2 << 15) | ((4 - 2) << 18)); // PLL=HSE/PREDIV, PLLMUL4
	}
	RCC->CR |= 1 << 24; // PLLON
	while (RCC->CR & (1 << 25));
	RCC->CFGR |= 2; // SW = PLL
	while ((RCC->CFGR & 12) != 8);
}

void putc(int c) {
	while (!(USART1->ISR & USART_ISR_TXE)); // wait for sent
	USART1->TDR = c; // 書き込みはできてそう、TXEがセットされる
}
void putn(int n) {
	int a = 1000000;
	for (int i = 0; i < 7; i++) {
		int m = n / a;
		putc(m + '0');
		n %= a;
		a /= 10;
	}
	putc('\n');
}

void Reset_Handler(void) {
	changeClockTo48MHz();
	c_init();
	
	// for debug
	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashに切り替え // リリース時には不要
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA | AHBENR_DMA;
	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP | APB2ENR_USART1;
	
	// GPIO for test
//	GPIOB->MODER |= 1 << (2 * 1); // PB1 as OUTPUT
	GPIOA->MODER = (GPIOA->MODER & ~(3 << (2 * 7))) | (1 << (2 * 7)); // PA7 as output
	
	// DMA buffer
	char buf[256];
	int len = 100;
	
	char mes[] = { "hello world!\n" };
	
	// DMA CH2 - USART1_TX
	DMA->CCR2 = (0b00 << 8) | (1 << 7) | (1 << 4) | (3 << 12); // peripheral8bit, MINC(mem increment), DIR:1(from mem), priority:3(veryhigh)
	DMA->CNDTR2 = sizeof(mes) - 1; // 最大65535
	DMA->CPAR2 = (uint32)&USART1->TDR;
	DMA->CMAR2 = (uint32)mes;
	
	// DMA CH3 - USART1_RX
	DMA->CCR3 = (0b00 << 8) | (1 << 7) | (0 << 4) | (3 << 12); // peripheral8bit, MINC(mem increment), DIR:0(from pheri), priority:3(veryhigh)
	DMA->CNDTR3 = len; // 最大65535
	DMA->CPAR3 = (uint32)&USART1->RDR;
	DMA->CMAR3 = (uint32)buf;
	
	// USART1 TX:PA9, RX:PA10
	GPIOA->AFRH |= (1 << (4 * (9 - 8))) | (1 << (4 * (10 - 8))); // PA9 and PA10 as AF1 == USART
	GPIOA->OSPEEDR |= 3 << (2 * 9); // PA9 as 50Mhz
	GPIOA->MODER |= (2 << (2 * 9)) | (2 << (2 * 10)); // PA9, PA10 as alternate
	USART1->BRR = 48000000 / 115200; // 48MHz, 115200bps
//	USART1->BRR = 48000000 / 9600; // 48MHz, 9600bps
	int autoboad = 0;
	if (autoboad) {
		USART1->CR2 |= USART_CR2_ABREN; // auto boad mode ... ok!
		// 外部クロックの場合 416と理論値通り
		// 内蔵クロックの場合 392になる理論値416, 5.8%ずれ
	}
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
	
	USART1->CR3 |= USART_CR3_DAMR | USART_CR3_DAMT;
	USART1->CR1 |= USART_CR1_UE;
	
	if (autoboad) {
		while (!(USART1->ISR & USART_ISR_ABRF)); // ok // auto boad rate
	}
	
	// 4byte read
	DMA->CNDTR3 = 4; // 最大65535
	DMA->CCR3 |= (1 << 0); // enable
	// wait recevie
	while (!(DMA->ISR & (1 << 9))); // TCIF3ch=1:recv
	DMA->IFCR |= 1 << 9;
	DMA->CCR3 &= ~(1 << 0); // disable
	
	DMA->CNDTR3 = len; // 最大65535
	DMA->CCR3 |= (1 << 0); // enable
	
//	while (!(USART1->ISR & USART_ISR_TXE)); // wait for empty to send
//	USART1->TDR = 'S'; // ok
	
	/*
	for (int i = 0; i < len; i++) {
		while (!(USART1->ISR & USART_ISR_TXE)); // wait for empty to send
		char c = buf[i];
		if (c >= 'a' && c <= 'z') {
			c += 'A' - 'a';
		}
		USART1->TDR = c;
	}
	while (!(USART1->ISR & USART_ISR_TXE)); // wait for empty to send
	*/
//	while (!(USART1->ISR & USART_ISR_TXE)); // wait for empty to send
//	while (!(USART1->ISR & USART_ISR_TC)); // wait for empty to send
	
	for (volatile int i = 0; i < 100000; i++); // なくすと文字化けする??
	for (;;) {
		GPIOA->ODR = 1 << 7; // PB7 on
		
		DMA->CCR2 &= ~(1 << 0); // disable
		DMA->CNDTR2 = sizeof(mes) - 1;
		DMA->CCR2 |= (1 << 0); // enable
		while (!(DMA->ISR & (1 << 5))); // TCIF2ch=1:sent
		DMA->IFCR |= 1 << 5;
		
		// next
		
//		putn(1);
//		USART1->TDR = '0';
		
		for (volatile int i = 0; i < 100000; i++); // なくすと文字化けする??
		putn(DMA->CNDTR3); // 入力されるごとに減っていく
		GPIOA->ODR &= ~(1 << 7); // PB7 off
		
	}
}
