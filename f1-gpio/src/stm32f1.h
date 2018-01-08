#define SRAM_LOC 0x20000000
#define SRAM_LEN 0x1000
#define FLASH_LOC 0x08000000
#define FLASH_LEN 0x8000

#define __I  volatile const // read only
#define __O  volatile       // write only
#define __IO volatile       // read / write

typedef struct {
	__IO uint32 ACR; // 0
	__O  uint32 KEYR; // 4
	__O  uint32 OPTKEYR; // 8
	__IO uint32 SR; // C
	__IO uint32 CR; // 10
	__IO uint32 AR; // 14
	uint32 RESERVED_00; // 18
	__IO uint32 OBR; // 1C
	__IO uint32 WRPR; // 20
} FLASH_TypeDef;
#define FLASH ((FLASH_TypeDef*)0x40022000)

#define FLASH_SR_BSY		(1 << 0)
#define FLASH_SR_PGERR		(1 << 2)
#define FLASH_SR_WRPRTERR	(1 << 4)
#define FLASH_SR_EOP		(1 << 5)

#define FLASH_CR_PG			(1 << 0)
#define FLASH_CR_PER		(1 << 1)
#define FLASH_CR_MER		(1 << 2)
#define FLASH_CR_STRT		(1 << 6)
#define FLASH_CR_LOCK		(1 << 7)

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

#define APB2RSTR_USART1RST	(1 << 14)

#define AHBENR_DMA		(1 << 0)
#define AHBENR_DMA2		(1 << 1)
#define AHBENR_SRAM		(1 << 2)
#define AHBENR_FLITF	(1 << 4)
#define AHBENR_CRC		(1 << 6)
#define AHBENR_TSC		(1 << 24)

#define APB2ENR_AFIO		(1 << 0)
#define APB2ENR_IOPA		(1 << 2)
#define APB2ENR_IOPB		(1 << 3)
#define APB2ENR_IOPC		(1 << 4)
#define APB2ENR_IOPD		(1 << 5)
#define APB2ENR_IOPE		(1 << 6)

#define APB2ENR_SYSCFGCOMP	(1 << 0)
#define APB2ENR_ADC			(1 << 9)
#define APB2ENR_TIM1		(1 << 11)
#define APB2ENR_SPI1		(1 << 12)
#define APB2ENR_USART1		(1 << 14)
#define APB2ENR_TIM16		(1 << 17)
#define APB2ENR_TIM17		(1 << 18)
#define APB2ENR_DBG			(1 << 22)

#define APB1ENR_TIM2		(1 << 0)
#define APB1ENR_TIM3		(1 << 1)
#define APB1ENR_TIM14		(1 << 8)
#define APB1ENR_WWDG		(1 << 11)
#define APB1ENR_SPI2		(1 << 14)
#define APB1ENR_USART2		(1 << 17)
#define APB1ENR_I2C1		(1 << 21)
#define APB1ENR_PWR			(1 << 28)

#define RCC_CR2_HSI14ON		(1 << 0)
#define RCC_CR2_HSI14RDY	(1 << 1)

typedef struct {
	__IO uint32 CRL;
	__IO uint32 CRH;
	__I  uint32 IDR;
	__O  uint32 ODR;
	__O  uint32 BSRR; // 0x18 bit0-15:bit set, bit16-31:bet reset
	__O  uint32 BRR; // 0x28 bit0-15:bet reset
	__IO uint32 LCKR; // 0x1c
} GPIO_TypeDef;
#define GPIOA ((GPIO_TypeDef*)0x40010800)
#define GPIOB ((GPIO_TypeDef*)0x40010C00)
#define GPIOC ((GPIO_TypeDef*)0x40011000)
#define GPIOD ((GPIO_TypeDef*)0x40011400)
#define GPIOE ((GPIO_TypeDef*)0x40011800)

#define GPIO_MODER_IN	0
#define GPIO_MODER_OUT	1
#define GPIO_MODER_AF	2
#define GPIO_MODER_ANA	3

#define GPIO_PUPDR_OPEN		0
#define GPIO_PUPDR_PUP		1
#define GPIO_PUPDR_PDOWN	2

typedef struct {
	__IO uint32 CR1; // 0x00
	__IO uint32 CR2; // 0x04
	__IO uint32 CR3; // 0x08
	__IO uint32 BRR; // 0x0c
	__IO uint32 GTPR; // 0x10
	__IO uint32 RTOR; // 0x14
	__IO uint32 RQR; // 0x18
	__I  uint32 ISR; // 0x1c
	__O  uint32 ICR; // 0x20
	__I  uint32 RDR; // 0x24
	__IO uint32 TDR; // 0x28
} USART_TypeDef;
#define USART1 ((USART_TypeDef*)0x40013800)
#define USART2 ((USART_TypeDef*)0x40004400)

#define USART_CR1_UE		(1 << 0)
#define USART_CR1_RE		(1 << 2)
#define USART_CR1_TE		(1 << 3)
#define USART_CR1_IDLEIE	(1 << 4)
#define USART_CR1_RXNEIE	(1 << 5)
#define USART_CR1_TCIE		(1 << 6)
#define USART_CR1_TXEIE		(1 << 7)
#define USART_CR1_OVER8		(1 << 15) // 0:x16(default) 1:x8

//#define USART_CR2_ABRMOD	(1 << 21)
#define USART_CR2_ABREN		(1 << 20) // auto boad rate

#define USART_CR3_DAMR		(1 << 6)
#define USART_CR3_DAMT		(1 << 7)

#define USART_ISR_PE 		(1 << 0)
#define USART_ISR_FE 		(1 << 1)
#define USART_ISR_NF 		(1 << 2)
#define USART_ISR_ORE 		(1 << 3)
#define USART_ISR_IDLE 		(1 << 4)
#define USART_ISR_RXNE 		(1 << 5) // recevie not empty
#define USART_ISR_TC 		(1 << 6) // transfer complete
#define USART_ISR_TXE 		(1 << 7)
#define USART_ISR_LBDF 		(1 << 8)
#define USART_ISR_CTSIF		(1 << 9)
#define USART_ISR_CTS 		(1 << 10)
#define USART_ISR_RTOF 		(1 << 11)
#define USART_ISR_EOBF 		(1 << 12)
#define USART_ISR_ABRE		(1 << 14) // auto boad error
#define USART_ISR_ABRF		(1 << 15) // auto boad status

#define USART_RQR_RXFRQ		(1 << 3)

typedef struct {
	__IO uint32 IMR;
	__IO uint32 EMR;
	__IO uint32 RTSR;
	__IO uint32 FTSR;
	__IO uint32 SWIER;
	__IO uint32 PR;
} EXTI_TypeDef;
#define EXTI ((EXTI_TypeDef*)0x40010400)

typedef struct {
	__IO uint32 CFGR1;
	uint32 RESERVERD;
	__IO uint32 EXTICR1;
	__IO uint32 EXTICR2;
	__IO uint32 EXTICR3;
	__IO uint32 EXTICR4;
	__IO uint32 CFGR2;
} SYSCFG_TypeDef;
#define SYSCFG ((SYSCFG_TypeDef*)0x40010000)

typedef struct {
	__IO uint32 CR1;
	__IO uint32 CR2;
	__IO uint32 SMCR;
	__IO uint32 DIER; // for DMA and Interupt
	__IO uint32 SR; //
	__IO uint32 EGR; // for Event
	__IO uint32 CCMR1;
	__IO uint32 CCMR2;
	__IO uint32 CCER;
	__IO uint32 CNT;
	__IO uint32 PSC;
	__IO uint32 ARR;
	__IO uint32 RCR; // only TIM1 - rpeat counter
	__IO uint32 CCR1;
	__IO uint32 CCR2;
	__IO uint32 CCR3;
	__IO uint32 CCR4;
	__IO uint32 BDTR; // only TIM1
	__IO uint32 DCR;
	__IO uint32 DMAR;
	__IO uint32 OR; // 0x50 only TIM14
} TIM_TypeDef;
#define TIM1 ((TIM_TypeDef*)0x40012C00)
#define TIM3 ((TIM_TypeDef*)0x40000400)
#define TIM14 ((TIM_TypeDef*)0x40002000)
#define TIM16 ((TIM_TypeDef*)0x40014400)
#define TIM17 ((TIM_TypeDef*)0x40014800)

typedef struct {
	__IO uint32 CR1; // 0
	__IO uint32 CR2; // 4
	__IO uint32 SR; // 8
	__IO uint32 DR; // C
	__IO uint32 CRCPR; // 10
	__IO uint32 RXCRCR; // 14
	__IO uint32 TXCRCR; // 18
	__IO uint32 I2SCFGR; // 1C // only I2S
	__IO uint32 I2SPR; // 20 // only I2S
} SPI_TypeDef;
#define SPI1 ((SPI_TypeDef*)0x40013000)

typedef struct {
	__I  uint32 ISR; // 0
	__O  uint32 IFCR; // 4
	__IO uint32 CCR1; // 8
	__IO uint32 CNDTR1; // C
	__IO uint32 CPAR1; // 10
	__IO uint32 CMAR1; // 14
	uint32 RESERVED1; // 18
	__IO uint32 CCR2; // 1c
	__IO uint32 CNDTR2; // 20
	__IO uint32 CPAR2;
	__IO uint32 CMAR2;
	uint32 RESERVED2;
	__IO uint32 CCR3;
	__IO uint32 CNDTR3;
	__IO uint32 CPAR3;
	__IO uint32 CMAR3;
	uint32 RESERVED3;
	__IO uint32 CCR4;
	__IO uint32 CNDTR4;
	__IO uint32 CPAR4;
	__IO uint32 CMAR4;
	uint32 RESERVED4;
	__IO uint32 CCR5;
	__IO uint32 CNDTR5;
	__IO uint32 CPAR5;
	__IO uint32 CMAR5;
	uint32 RESERVED5;
} DMA_TypeDef;
#define DMA ((DMA_TypeDef*)0x40020000)

typedef struct {
	__IO uint32 CR;
	__IO uint32 CSR;
} PWR_TypeDef;
#define PWR ((PWR_TypeDef*)0x40007000)

#define PWR_CR_LPDS		(1 << 0)
#define PWR_CR_PDDS		(1 << 1)
#define PWR_CR_CWUF		(1 << 2)	// set: clear wake up flag
#define PWR_CR_CSBF		(1 << 3)	// set: clear standby flag

#define PWR_CSR_WUF		(1 << 0)
#define PWR_CSR_SBF		(1 << 1)
#define PWR_CSR_EWUP1	(1 << 8)

typedef struct {
	__IO uint32 ISR;
	__IO uint32 IER;
	__IO uint32 CR;
	__IO uint32 CFGR1;
	__IO uint32 CFGR2;
	__IO uint32 SMPR;
	__IO uint32 RESERVED18;
	__IO uint32 RESERVED1C;
	__IO uint32 TR;
	__IO uint32 RESERVED24;
	__IO uint32 CHSELR;
	__IO uint32 RESERVED2C_3C[5];
	__IO uint32 DR; // 0x40
	__IO uint32 RESERVED44_304[177];
	__IO uint32 CCR; // 0x308
} ADC_TypeDef;
#define ADC ((ADC_TypeDef*)0x40012400)

#define ADC_ISR_ADRDY	(1 << 0)
#define ADC_ISR_EOSMP	(1 << 1)
#define ADC_ISR_EOC		(1 << 2)
#define ADC_ISR_EOSEQ	(1 << 3)
#define ADC_ISR_OVR		(1 << 4)
#define ADC_ISR_AWD		(1 << 7)

#define ADC_CR_ADEN		(1 << 0)
#define ADC_CR_ADDIS	(1 << 1)
#define ADC_CR_ADSTART	(1 << 2)
#define ADC_CR_ADSTP	(1 << 4)
#define ADC_CR_ADCAL	(1 << 31)

#define ADC_CCR_VBATEN	(1 << 24)	// VBAT for RTC?
#define ADC_CCR_TSEN	(1 << 23)	// 温度センサー
#define ADC_CCR_VREFEN	(1 << 22)	// ADC
