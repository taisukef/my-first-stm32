#include "stddef.h"
#include "vectors.h"
#include "stm32f0.h"
#include "c_init.h"
#include "core_cm0.h"

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

#define UART_BUF_SIZE 4
uint8 uart_buf[UART_BUF_SIZE];
int16 uart_write = 0;
int16 uart_read = 0;

void uart_put(int c);

void USART1_IRQHandler(void) {
	// if (USART1->ISR & USART_ISR_RXNE)  // omit, only recevie
	int next = uart_write + 1;
	if (next == UART_BUF_SIZE) {
		next = 0;
	}
	uint8 c = (uint8)(USART1->RDR);
	if (next != uart_read) {
		uart_buf[uart_write] = c;
		uart_write = next;
	} else {
		uart_put('*'); // else buffer over flow
	}
	
//	USART1->RQR |= USART_RQR_RXFRQ; // clear RXNE flag, RDR読めばクリアされるから不要
}

int uart_get() { // -1 not ready
	if (uart_write == uart_read) {
		return -1;
	}
	int res = uart_buf[uart_read];
	uart_read++;
	if (uart_read == UART_BUF_SIZE) {
		uart_read = 0;
	}
	return res;
}

void uart_put(int c) {
	while (!(USART1->ISR & USART_ISR_TXE)); // wait for sent
	USART1->TDR = c;
}

//

void putc(int c) {
	uart_put(c);
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

int readline(char* buf, int maxlen) {
	int cnt = 0;
	for (;;) {
		int n = uart_get();
		if (n < 0) {
			__WFI();
			continue;
		}
		if (n == 13) // CR r
			continue;
		if (n == 10) // LF n
			break;
		buf[cnt++] = (uint8)n;
		if (cnt + 1 == maxlen)
			break;
	}
	buf[cnt] = (uint8)0;
	return cnt;
}

void Reset_Handler(void) {
	changeClockTo48MHz();
	
	c_init();
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA;
	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP | APB2ENR_USART1;
	
	// フラッシュ書き込みから起動には必要（割り込みを有効にするため）  // リリース時には不要
	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashにする
	
	// USART1
	GPIOA->AFRH |= (1 << (4 * (9 - 8))) | (1 << (4 * (10 - 8))); // PA9 and PA10 as AF1 == USART
//	GPIOA->OSPEEDR |= 3 << (2 * 9); // PA9 as 50Mhz
	GPIOA->MODER |= (2 << (2 * 9)) | (2 << (2 * 10)); // PA9, PA10 as alternatefunction(AF2)
	USART1->BRR = 48000000 / 115200; // 48MHz, 115200bps
//	USART1->BRR = 48000000 / 9600; // 48MHz, 9600bps
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 2);
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;
	
	/*
	// echo test
	for (;;) {
		int c = uart_get();
		if (c >= 0) {
			if (c >= 'a' && c <= 'z')
				c++;
			uart_put(c);
		}
	}
	*/
	
	// command test
	int bufsize = 32;
	char buf[bufsize];
	for (;;) {
		int len = readline(buf, bufsize);
		putc('[');
		for (int i = 0; i < len; i++) {
			putc(buf[i]);
		}
		putc(']');
		putn(len);
	}
}
