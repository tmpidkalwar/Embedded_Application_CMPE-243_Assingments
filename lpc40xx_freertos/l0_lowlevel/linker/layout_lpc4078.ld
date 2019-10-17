MEMORY
{
  MFlash512 (rx)    : ORIGIN = 0x00000000, LENGTH = 512K
  RamLoc64 (rwx)    : ORIGIN = 0x10000000, LENGTH = 64K
  RamPeriph32 (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

/*************************************************
                                                  *
     64K RAM +-----Stack Top   +-----+ 32K RAM    *
             |    ||           |     |            *
             |    \/           |     |            *
             |     |           |     |            *
             |     |           |     |            *
             |    /\ heap      |    /\            *
             |    || overflow  |    ||            *
             .-----.           +-----Heap Start   *
             |bbbbb|                              *
             |bbbbb| *bss                         *
             .-----.                              *
             |ddddd|                              *
             |ddddd| *data                        *
             .-----.                              *
**************************************************
*/

/* Define a symbol for the top of each memory region */
__base_Flash     = 0x00000000;
__top_Flash      = __base_Flash + 512k;

__base_RamLoc64  = 0x10000000;
__top_RamLoc64   = __base_RamLoc64 + 64k;

__base_RamPeriph32 = 0x20000000;
__top_RamPeriph32  = __base_RamPeriph32 + 32k;

ENTRY(entry_point)

SECTIONS
{
    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.interrupt_vector_table))

        /* Global Section Table */
        . = ALIGN(4) ;
        section_table_start = .;
        data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        data_section_table_end = .;
        bss_section_table = .;

        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        bss_section_table_end = .;
        section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)

        /* C++ constructors etc */
        . = ALIGN(4);
        KEEP(*(.init))

        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        KEEP(*(.fini));

        . = ALIGN(4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))
        /* End C++ */
    } >MFlash512

    .text : ALIGN(4)
    {
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > MFlash512

    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > MFlash512
    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > MFlash512
    __exidx_end = .;

    _etext = .;

    /* DATA section for RamPeriph32 */
    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$RamPeriph32)
        *(.data.$RAM2*)
        *(.data.$RamPeriph32*)
        . = ALIGN(4) ;
        PROVIDE(_edata_peripheral = .) ;
     } > RamPeriph32 AT>MFlash512

    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > RamLoc64

    /* Main DATA section (RamLoc64) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       *(vtable)
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       PROVIDE(_edata = .);
    } > RamLoc64 AT>MFlash512

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        PROVIDE(_ebss = .);
        PROVIDE(end = .);
    } > RamLoc64

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
    } > RamLoc64

    /* At this point, we have un-used RAM after the BSS and DATA section all the way to the top */
    PROVIDE(__start_of_unused_ram64 = .);
    PROVIDE(__end_of_unused_ram64 = __top_RamLoc64);

    /* NOINIT section for RamPeriph32 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
        KEEP(*(.persistent_ram))
        persistent_ram = .;

        KEEP(*(._noinit_ram2))
        _noinit_ram2 = .;
        *(.noinit_ram2*)
       . = ALIGN(4) ;
    } > RamPeriph32

    /* BSS section for RamPeriph32 */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(_bbss_ram2 = .) ;
       *(.bss.$RAM2*)
       *(.bss.$RamPeriph32*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(_ebss_ram2 = .) ;
    } > RamPeriph32

    /* Heap starts at the beginning of RAM2 after any entities that occupy the beginning of RAM2 such as persistent_ram */
    PROVIDE(_bheap = .);
    PROVIDE(_eheap = __top_RamPeriph32);

    /* Stack is at the top of RAM64 unless explicitly defined by the user */
    PROVIDE(_stack_top = DEFINED(__user_stack_top) ? __user_stack_top : __top_RamLoc64 - 0);
}

PROVIDE(_estack = _stack_top);

PROVIDE(_bdata_lma = LOADADDR(.data));
PROVIDE(_bdata_vma = ADDR(.data));

PROVIDE(_bdata_lma_peripheral = LOADADDR(.data_RAM2));
PROVIDE(_bdata_vma_peripheral = ADDR(.data_RAM2));