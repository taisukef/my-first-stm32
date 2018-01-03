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

void changeClockTo48MHzIRC() {
	// 48MHz = 8MHz * 6(PLLMUL)
	FLASH->ACR |= 0b10001; // 1<<4:enable prefetch, 1 wait state
	RCC->CFGR = (RCC->CFGR & ~(0b1111111 << 15)) | ((1 << 15) | ((6 - 2) << 18)); // PLL=HSI/PREDIV, PLLMUL6
	RCC->CR |= 1 << 24; // PLLON
	while (RCC->CR & (1 << 25));
	RCC->CFGR |= 2; // SW = PLL
	while ((RCC->CFGR & 12) != 8);
}

void changeClockTo48MHzEx() {
	// 48MHz = external 12MHz x 4
	RCC->CR |= 1 << 16; // HSEON
	while (!(RCC->CR & (1 << 17))); // wait HSERDY
	
	RCC->CFGR = (RCC->CFGR & ~(0b1111111 << 15)) | ((2 << 15) | ((4 - 2) << 18)); // PLL=HSE/PREDIV, PLLMUL4
	RCC->CR |= 1 << 24; // PLLON
	while (RCC->CR & (1 << 25));
	RCC->CFGR |= 2; // SW = PLL
	while ((RCC->CFGR & 12) != 8);
}

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

static int WAIT_CNT = 100000;

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
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA;
	
	// GPIO
	GPIOB->MODER |= 1 << (2 * 1); // PB1 as OUTPUT
	
	// USART1
//	RCC->CFGR3 |= 1; // USART1SW 0:PCLK 1:sysclk
	RCC->APB2ENR |= APB2ENR_USART1; // TX:PA9, RX:PA10
//	RCC->APB2RSTR |= APB2RSTR_USART1RST; // reset USART
	GPIOA->AFRH |= (1 << (4 * (9 - 8))) | (1 << (4 * (10 - 8))); // PA9 and PA10 as AF1 == USART
	GPIOA->OSPEEDR |= 3 << (2 * 9); // PA9 as 50Mhz
	GPIOA->MODER |= (2 << (2 * 9)) | (2 << (2 * 10)); // PA9, PA10 as alternatefunction
	USART1->BRR = 48000000 / 115200; // 48MHz, 115200bps
//	USART1->BRR = 48000000 / 9600; // 48MHz, 9600bps
	
	int autoboad = 1;
	if (autoboad) {
		USART1->CR2 |= USART_CR2_ABREN; // auto boad mode ... ok!
		// 外部クロックの場合 416と理論値通り
		// 内蔵クロックの場合 392になる理論値416, 5.8%ずれ
	}
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
	
	int c = '0';
	
//	while (!(USART1->CR2 & USART_CR2_ABREN)); // ok
	
	if (autoboad) {
		while (!(USART1->ISR & USART_ISR_ABRF)); // ok // auto boad rate
	}
//	putn(12345);
//	putn(USART1->BRR);
	
	/*
	// echo test
	for (;;) {
		if (USART1->ISR & USART_ISR_RXNE) {
			GPIOB->ODR = 1 << 1; // PB1 on
			c = (uint8)(USART1->RDR);
			
			while (!(USART1->ISR & USART_ISR_TXE)); // wait for sent
			USART1->TDR = c + 1; // 書き込みはできてそう、TXEがセットされる
			GPIOB->ODR = 0 << 1;
		}
	}
	*/
	int wait = WAIT_CNT;
	for (;;) {
		GPIOB->ODR = 1 << 1; // PB1 on
		
		// single buffer では、読み込む前に次のデータが来ると止まってしまう
		if (USART1->ISR & USART_ISR_RXNE) {
			c = (uint8)(USART1->RDR);
			if (c >= '0' && c <= '9')
				wait = WAIT_CNT * (c - '0' + 1);
			putc('\n');
			putn(c);
			putn(USART1->BRR);
			putc('\n');
		}
		
		// loop
		while (!(USART1->ISR & USART_ISR_TXE)); // wait for sent
		USART1->TDR = c;
		
//		while (!(USART2->ISR & USART_ISR_TXE)); // wait for sent
//		USART2->TDR = c;
		
		// last
//		while (!(USART1->ISR & USART_ISR_TC)); // wait for sent
		
		if (c >= '9' || c < '0') {
			c = '0';
		} else {
			c++;
		}
		
		for (volatile int i = 0; i < wait; i++);
		
		GPIOB->ODR = 0 << 1;
		
		for (volatile int i = 0; i < wait; i++);
	}
}
