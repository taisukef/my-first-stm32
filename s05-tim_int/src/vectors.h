#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

WEAK void IntDefaultHandler(void);

void Reset_Handler(void);
void NMI_Handler(void) ALIAS(IntDefaultHandler);
void HardFault_Handler(void) ALIAS(IntDefaultHandler);
void SVC_Handler(void) ALIAS(IntDefaultHandler);
void PendSV_Handler(void) ALIAS(IntDefaultHandler);
void SysTick_Handler(void) ALIAS(IntDefaultHandler);

void WWDG_IRQHandler(void) ALIAS(IntDefaultHandler);                // Window Watchdog
void RTC_IRQHandler(void) ALIAS(IntDefaultHandler);                 // RTC through EXTI Line
void FLASH_IRQHandler(void) ALIAS(IntDefaultHandler);               // FLASH
void RCC_IRQHandler(void) ALIAS(IntDefaultHandler);                 // RCC
void EXTI0_1_IRQHandler(void) ALIAS(IntDefaultHandler);             // EXTI Line 0 and 1
void EXTI2_3_IRQHandler(void) ALIAS(IntDefaultHandler);             // EXTI Line 2 and 3
void EXTI4_15_IRQHandler(void) ALIAS(IntDefaultHandler);            // EXTI Line 4 to 15
void DMA1_Channel1_IRQHandler(void) ALIAS(IntDefaultHandler);       // DMA1 Channel 1
void DMA1_Channel2_3_IRQHandler(void) ALIAS(IntDefaultHandler);     // DMA1 Channel 2 and Channel 3
void DMA1_Channel4_5_IRQHandler(void) ALIAS(IntDefaultHandler);     // DMA1 Channel 4 and Channel 5
void ADC1_IRQHandler(void) ALIAS(IntDefaultHandler);                // ADC1 
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) ALIAS(IntDefaultHandler); // TIM1 Break, Update, Trigger and Commutation
void TIM1_CC_IRQHandler(void) ALIAS(IntDefaultHandler);             // TIM1 Capture Compare
void TIM3_IRQHandler(void); // ALIAS(IntDefaultHandler);                // TIM3
void TIM14_IRQHandler(void) ALIAS(IntDefaultHandler);               // TIM14
void TIM16_IRQHandler(void) ALIAS(IntDefaultHandler);               // TIM16
void TIM17_IRQHandler(void) ALIAS(IntDefaultHandler);               // TIM17
void I2C1_IRQHandler(void) ALIAS(IntDefaultHandler);                // I2C1
void SPI1_IRQHandler(void) ALIAS(IntDefaultHandler);                // SPI1
void USART1_IRQHandler(void) ALIAS(IntDefaultHandler);              // USART1

extern void _vStackTop(void);

extern void (*const __Vectors[])(void);

__attribute__ ((section(".isr_vector")))
void (*const __Vectors[])(void) = {
	// common STM32 & LPC
    &_vStackTop,                    // The initial stack pointer
	Reset_Handler,                  // The reset handler
    NMI_Handler,                    // The NMI handler
    HardFault_Handler,              // The hard fault handler
	0,
	0,
	0,
	0,
	0,
	0,
	0,
    SVC_Handler,                    // SVCall handler
	0,
	0,
    PendSV_Handler,                 // The PendSV handler
    SysTick_Handler,                // The SysTick handler
	
	// other interrupts
	WWDG_IRQHandler,                // Window Watchdog
	0,                              // Reserved
	RTC_IRQHandler,                 // RTC through EXTI Line
	FLASH_IRQHandler,               // FLASH
	RCC_IRQHandler,                 // RCC
	EXTI0_1_IRQHandler,             // EXTI Line 0 and 1
	EXTI2_3_IRQHandler,             // EXTI Line 2 and 3
	EXTI4_15_IRQHandler,            // EXTI Line 4 to 15
	0,                              // Reserved
	DMA1_Channel1_IRQHandler,       // DMA1 Channel 1
	DMA1_Channel2_3_IRQHandler,     // DMA1 Channel 2 and Channel 3
	DMA1_Channel4_5_IRQHandler,     // DMA1 Channel 4 and Channel 5
	ADC1_IRQHandler,                // ADC1 
	TIM1_BRK_UP_TRG_COM_IRQHandler, // TIM1 Break, Update, Trigger and Commutation
	TIM1_CC_IRQHandler,             // TIM1 Capture Compare
	0,                              // Reserved
	TIM3_IRQHandler,                // TIM3
	0,                              // Reserved
	0,                              // Reserved
	TIM14_IRQHandler,               // TIM14
	0,                              // Reserved
	TIM16_IRQHandler,               // TIM16
	TIM17_IRQHandler,               // TIM17
	I2C1_IRQHandler,                // I2C1
	0,                              // Reserved
	SPI1_IRQHandler,                // SPI1
	0,                              // Reserved
	USART1_IRQHandler,              // USART1
};

__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void) {
	for (;;);
}

typedef enum {
/******  Cortex-M0 Processor Exceptions Numbers **************************************************************/
	NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                        */
	HardFault_IRQn              = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                                */
	SVC_IRQn                    = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                                  */
	PendSV_IRQn                 = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                                  */
	SysTick_IRQn                = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                              */
	
	/******  STM32F0 specific Interrupt Numbers ******************************************************************/
	WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                               */
	RTC_IRQn                    = 2,      /*!< RTC Interrupt through EXTI Lines 17, 19 and 20                  */
	FLASH_IRQn                  = 3,      /*!< FLASH global Interrupt                                          */
	RCC_IRQn                    = 4,      /*!< RCC global Interrupt                                            */
	EXTI0_1_IRQn                = 5,      /*!< EXTI Line 0 and 1 Interrupt                                     */
	EXTI2_3_IRQn                = 6,      /*!< EXTI Line 2 and 3 Interrupt                                     */
	EXTI4_15_IRQn               = 7,      /*!< EXTI Line 4 to 15 Interrupt                                     */
	DMA1_Channel1_IRQn          = 9,      /*!< DMA1 Channel 1 Interrupt                                        */
	DMA1_Channel2_3_IRQn        = 10,     /*!< DMA1 Channel 2 and Channel 3 Interrupt                          */
	DMA1_Channel4_5_IRQn        = 11,     /*!< DMA1 Channel 4 and Channel 5 Interrupt                          */
	ADC1_IRQn                   = 12,     /*!< ADC1 Interrupt                                                  */
	TIM1_BRK_UP_TRG_COM_IRQn    = 13,     /*!< TIM1 Break, Update, Trigger and Commutation Interrupt           */
	TIM1_CC_IRQn                = 14,     /*!< TIM1 Capture Compare Interrupt                                  */
	TIM3_IRQn                   = 16,     /*!< TIM3 global Interrupt                                           */
	TIM14_IRQn                  = 19,     /*!< TIM14 global Interrupt                                          */
	TIM16_IRQn                  = 21,     /*!< TIM16 global Interrupt                                          */
	TIM17_IRQn                  = 22,     /*!< TIM17 global Interrupt                                          */
	I2C1_IRQn                   = 23,     /*!< I2C1 Event Interrupt                                            */
	SPI1_IRQn                   = 25,     /*!< SPI1 global Interrupt                                           */
	USART1_IRQn                 = 27      /*!< USART1 global Interrupt                                         */
} IRQn_Type;
