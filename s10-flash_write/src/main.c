#include "stddef.h"
#include "vectors.h"
#include "stm32f0.h"
#include "core_cm0.h"
#include "c_init.h"

void led_init() {
	// test PA7
	GPIOA->MODER = (GPIOA->MODER & ~(3 << (2 * 7))) | (1 << (2 * 7)); // PA7 as output
}
void led_blink() {
	int wait = 100000;
	GPIOA->ODR = 1 << 7; // PB1 on
	for (volatile int i = 0; i < wait; i++);
	GPIOA->ODR = 0 << 7;
	for (volatile int i = 0; i < wait; i++);
}
void led_err(int cntblink) {
	for (int i = 0; i < cntblink; i++) {
		led_blink();
	}
	for (;;);
}

int flash_write(char* flashaddr, char* data1k) { // flashaddr(0x08000000 + 1k * n) -> 0: ok, 1:erase_err, 2:write_err
	// unlock
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
	
	int ok = 0;
	
	// erase
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = (uint32)flashaddr;
	FLASH->CR |= FLASH_CR_STRT;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_PER;
	ok = FLASH->SR & FLASH_SR_EOP;
	FLASH->SR &= ~FLASH_SR_EOP;
	
	if (!ok) {
		return 1;
	}
	
	// write
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PG;
	uint16* dst = (uint16*)flashaddr; // need 16bit each
	uint16* src = (uint16*)data1k;
	for (int i = 0; i < 1024 / 2; i++) {
		dst[i] = src[i];
		while (FLASH->SR & FLASH_SR_BSY);
	}
	FLASH->CR &= ~FLASH_CR_PG;
	ok = FLASH->SR & FLASH_SR_EOP;
	FLASH->SR &= ~FLASH_SR_EOP;
	
	if (!ok) {
		return 2;
	}
	
	// lock
	FLASH->CR |= FLASH_CR_LOCK;
	
	return 0;
}

void Reset_Handler(void) {
	c_init();
	
	// power
	RCC->AHBENR |= AHBENR_IOPB | AHBENR_IOPA;
	RCC->APB2ENR |= APB2ENR_SYSCFGCOMP;
	
	// for debug
	SYSCFG->CFGR1 &= ~0b11; // MEM_MODEをFlashに切り替え // リリース時には不要
	
	led_init();
	
	char* addr = (char*)FLASH_LOC + FLASH_LEN - 1024; // 32KB last 1page
	int len = 1024;
	char buf[len];
	for (int i = 0; i < len; i++) {
		buf[i] = i ^ 0x55;
	}
	
	int err = 0;
	err = flash_write(addr, buf);
	if (err) {
		led_err(err);
	}
	
	// check
	for (int i = 0; i < len; i++) {
		if (buf[i] != addr[i]) {
			led_err(5);
		}
	}
	
	led_err(1);
	
	for (;;);
}
