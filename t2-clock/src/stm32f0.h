#define SRAM_LOC 0x20000000
#define SRAM_LEN 0x1000

#define __I  volatile const // read only
#define __O  volatile       // write only
#define __IO volatile       // read / write

typedef struct {
	__IO uint32 ACR; // 0
	__IO uint32 KEYR; // 4
	__IO uint32 OPTKEYR; // 8
	__IO uint32 SR; // C
	__IO uint32 CR; // 10
	__IO uint32 AR; // 14
	uint32 RESERVED_00; // 18
	__IO uint32 OBR; // 1C
	__IO uint32 WRPR; // 20
} FLASH_TypeDef;
#define FLASH ((FLASH_TypeDef*)0x40022000)

typedef struct {
	__IO uint32 CR; // 0
	__IO uint32 CFGR; // 4
	__IO uint32 CIR; // 8
	__IO uint32 APB2RSTR; // 0x0c
	__IO uint32 APB1RSTR; // 0x10
	__IO uint32 AHBENR;  // 0x14
	__IO uint32 APB2ENR; // 0x18
	__IO uint32 APB1ENR; // 0x1c
	__IO uint32 BDCR; // 0x20 // RTC
	__IO uint32 CSR; // 0x24
	__IO uint32 AHBRSTR; // 0x28
	__IO uint32 CFGR2; // 0x2c
	__IO uint32 CFGR3; // 0x30 // USART_SW
	__IO uint32 CR2; // 0x34
} RCC_TypeDef;
#define RCC ((RCC_TypeDef*)0x40021000)

typedef struct {
	__IO uint32 MODER;
	uint32 RESERVED_04;
	uint32 RESERVED_08;
	uint32 RESERVED_0C;
	uint32 RESERVED_10;
	__O uint32 ODR;
} GPIOB_TypeDef;
#define GPIOB ((GPIOB_TypeDef*)0x48000400)
