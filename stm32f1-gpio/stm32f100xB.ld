MEMORY {
	FlashLoc(rx) : ORIGIN = 0x08000000, LENGTH = 128k
	
	/* RAM */
	RamLoc(rwx) : ORIGIN = 0x10000000, LENGTH = 8k
}
/* Define a symbol for the top of each memory region */
__top_FlashLoc = 0x08000000 + 128k;
__top_RamLoc   = 0x20000000 + 8k;

/*ENTRY(ResetISR)*/

SECTIONS {
    /* MAIN TEXT SECTION */    
    .text : ALIGN(4) {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.);
        KEEP(*(.isr_vector))

        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
/*        LONG(    ADDR(.data)) ; */
        LONG(  SIZEOF(.data));
		/*
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        __bss_section_table_end = .;
        __section_table_end = . ;
		*/
        /* End of Global Section Table */

		*(.after_vectors*)
    } >FlashLoc
    
    .text : ALIGN(4) {
         *(.text*)
        *(.rodata .rodata.*)
        . = ALIGN(4);
    } > FlashLoc

    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(4)
    {
    	*(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FlashLoc
    __exidx_start = .;
    
    .ARM.exidx : ALIGN(4)
    {
    	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FlashLoc
    __exidx_end = .;
    
    _etext = .;
    	
    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > RamLoc

	
	/* Main DATA section (RamLoc) */
	.data : ALIGN(4)
	{
	   FILL(0xff)
	   _data = .;
	   *(vtable)
	   *(.data*)
       *(.ichigojam_ram)
	   . = ALIGN(4) ;
	   _edata = . ;
	} > RamLoc AT>FlashLoc

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > RamLoc
	
    /* DEFAULT NOINIT SECTION */
	
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > RamLoc
	
    PROVIDE(_pvHeapStart = .);
	PROVIDE(_vStackTop = __top_RamLoc);
/*	PROVIDE(_vStackTop = __top_RamLoc - 32); */ /* offset 32 for iap command set */
}
