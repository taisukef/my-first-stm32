#define SRAM_LOC 0x20000000
#define SRAM_LEN 0x1000

#define __I  volatile const // read only
#define __O  volatile       // write only
#define __IO volatile       // read / write

typedef struct {
	uint32 RESERVED_00;
	uint32 RESERVED_04;
	uint32 RESERVED_08;
	uint32 RESERVED_0C;
	uint32 RESERVED_10;
	__IO uint32 AHBENR;
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
