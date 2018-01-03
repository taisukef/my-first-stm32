#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

WEAK void IntDefaultHandler(void);

void Reset_Handler(void);
void NMI_Handler(void) ALIAS(IntDefaultHandler);
void HardFault_Handler(void) ALIAS(IntDefaultHandler);
void SVC_Handler(void) ALIAS(IntDefaultHandler);
void PendSV_Handler(void) ALIAS(IntDefaultHandler);
void SysTick_Handler(void) ALIAS(IntDefaultHandler);

extern void _vStackTop(void);

extern void (*const __Vectors[])(void);

__attribute__ ((section(".isr_vector")))
void (*const __Vectors[])(void) = {
	// common STM32 & LPC
    &_vStackTop,		    				// The initial stack pointer
	Reset_Handler,                          // The reset handler
    NMI_Handler,                            // The NMI handler
    HardFault_Handler,                      // The hard fault handler
	0,
	0,
	0,
	0,
	0,
	0,
	0,
    SVC_Handler,                      	    // SVCall handler
	0,
	0,
    PendSV_Handler,                      	// The PendSV handler
    SysTick_Handler,                      	// The SysTick handler
	
	// other interrupts
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void) {
	for (;;);
}

