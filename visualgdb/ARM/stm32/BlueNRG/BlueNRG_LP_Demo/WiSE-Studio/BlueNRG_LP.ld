/*******************************************************************************
* BlueNRG-LP generic linker file for GCC
* Main linker variables to control it are:
*
* MEMORY_FLASH_APP_SIZE: define the size of the application in case not all the flash is needed.
* Default value is: 252KB when no OTA firmware upgrade is supported
*
* MEMORY_FLASH_APP_OFFSET: define the offset of the application.
* Default value is: 0 offset
*
* MEMORY_RAM_APP_OFFSET: define the offset in RAM from which variables can be
* allocated.
* 
* CONFIG_OTA_HIGHER: When defined application is built for OTA support
* in the higher part of memory
*
* CONFIG_OTA_LOWER: When defined application is built for OTA support
* in the lower part of memory
*
* CONFIG_OTA_USE_SERVICE_MANAGER: When defined application is built for
* OTA firmware upgrade support with separated application for firmware upgrade
*
*******************************************************************************/

/*******************************************************************************
* Memory Definitions
*******************************************************************************/
/*
BlueNRG-LP memory map
+-----------------------+ 0x2000FFFF
|  RAM (64K)            |
+-----------------------+ 0x20000000
|                       |
|                       |
+-----------------------+ 0x1007FFFF
|                       |
|  FLASH (256K)         |
+-----------------------+ 0x10040000
|                       |
+-----------------------| 0x100017FF
|   ROM (6K)            |
+-----------------------+ 0x10000000
*/


_MEMORY_RAM_BEGIN_   = 0x20000000;

_MEMORY_RAM_SIZE_ = DEFINED(CONFIG_DEVICE_BLUENRG_345) ? (0x8000)     : (0x10000) ;     /* 32KB  -  64KB  */
_MEMORY_RAM_END_  = DEFINED(CONFIG_DEVICE_BLUENRG_345) ? (0x20007FFF) : (0x2000FFFF) ;  /* 32KB  -  64KB  */

_MEMORY_FLASH_BEGIN_ = 0x10040000;  
_MEMORY_FLASH_SIZE_  = 0x40000;          /* 256KB */ 
_MEMORY_FLASH_END_   = 0x1007FFFF;  

_MEMORY_ROM_BEGIN_   = 0x10000000;    
_MEMORY_ROM_SIZE_    = 0x01800;          /* 6KB */ 
_MEMORY_ROM_END_     = 0x10001FFF;  


/* Reserved for BTLE stack non volatile memory */
FLASH_NVM_DATASIZE   = (4*1024);


  /* This configuration is intended for application not supporting OTA firmware upgrade */
  /*
  BlueNRG-LP standard application memory map
  +-----------------------+ 0x2000FFFF
  |  RAM (64K)            |
  +-----------------------+ 0x20000000
  |                       |
  |                       |
  +-----------------------+ 0x10080000
  |                       |
  |  NVM(4K)              |
  +-----------------------+ 0x1007F000
  |                       |
  |  User app (252K)      |
  +-----------------------+ 0x10040000
  |                       |
  +-----------------------| 0x100017FF
  |   ROM (6K)            |
  +-----------------------+ 0x10000000
*/

MEMORY_FLASH_APP_OFFSET = DEFINED(MEMORY_FLASH_APP_OFFSET) ? (MEMORY_FLASH_APP_OFFSET) : (0) ;
MEMORY_FLASH_APP_SIZE = DEFINED(MEMORY_FLASH_APP_SIZE) ? (MEMORY_FLASH_APP_SIZE) : (_MEMORY_FLASH_SIZE_ - FLASH_NVM_DATASIZE - MEMORY_FLASH_APP_OFFSET);
RESET_MANAGER_SIZE = DEFINED(RESET_MANAGER_SIZE) ? (RESET_MANAGER_SIZE) : (0x800) ;


/* 
   *****************************
   * 		CONFIG_OTA_HIGHER     *
   *****************************
*/
   /* This configuration is intended for application supporting OTA firmware upgrade with 2-app scheme (app in the upper part of memory map) */
  /*
     BlueNRG-LP OTA firmware upgrade support for higher application memory map
     +-----------------------+ 0x2000FFFF
     |  RAM (64K)            |
     +-----------------------+ 0x20000000
     |                       |
     |                       |
     +-----------------------+ 0x10080000
     |                       |
     |  NVM(4K)              |
     +-----------------------+ 0x1007E800
     |                       |
     |  Higher app (124K)    |
     +-----------------------+ 0x1005F800
     |                       |
     |  Lower app (124K)     |
     +-----------------------| 0x10040800
     |  Reset Manager (2K)   |
     +-----------------------+ 0x10040000
     |                       |
     +-----------------------| 0x100017FF
     |   ROM (6K)            |
     +-----------------------+ 0x10000000
  */

MEMORY_FLASH_APP_SIZE = DEFINED(CONFIG_OTA_HIGHER) ? ((((_MEMORY_FLASH_SIZE_ - RESET_MANAGER_SIZE - FLASH_NVM_DATASIZE) / 2) / 2048) * 2048) : (MEMORY_FLASH_APP_SIZE) ;
MEMORY_FLASH_APP_OFFSET = DEFINED(CONFIG_OTA_HIGHER) ? (RESET_MANAGER_SIZE + MEMORY_FLASH_APP_SIZE) : (MEMORY_FLASH_APP_OFFSET) ;


/* 
   *****************************
   *     CONFIG_OTA_LOWER      *
   *****************************
*/
  /* This configuration is intended for application supporting OTA firmware upgrade with 2-app scheme (app in the lower part of memory map) */
  /*
     BlueNRG-LP OTA firmware upgrade support for lower application memory map
     +-----------------------+ 0x2000FFFF
     |  RAM (64K)            |
     +-----------------------+ 0x20000000
     |                       |
     |                       |
     +-----------------------+ 0x10080000
     |                       |
     |  NVM(4K)              |
     +-----------------------+ 0x1007E800
     |                       |
     |  Higher app (124K)    |
     +-----------------------+ 0x1005F800
     |                       |
     |  Lower app (124K)     |
     +-----------------------| 0x10040800
     |  Reset Manager (2K)   |
     +-----------------------+ 0x10040000
     |                       |
     +-----------------------| 0x100017FF
     |   ROM (6K)            |
     +-----------------------+ 0x10000000
  */

MEMORY_FLASH_APP_SIZE = DEFINED(CONFIG_OTA_LOWER) ? ((((_MEMORY_FLASH_SIZE_ - RESET_MANAGER_SIZE - FLASH_NVM_DATASIZE) / 2) / 2048) * 2048) : (MEMORY_FLASH_APP_SIZE) ;
MEMORY_FLASH_APP_OFFSET = DEFINED(CONFIG_OTA_LOWER) ? (RESET_MANAGER_SIZE) : (MEMORY_FLASH_APP_OFFSET) ;


/* 
   ******************************************
   * 		CONFIG_OTA_USE_SERVICE_MANAGER     *
   ******************************************
*/
  /* This configuration is intended for application supporting OTA firmware upgrade with independent OTA firmware upgrade service manager
  (app in the upper part of memory map) */
  /*
     BlueNRG-LP OTA firmware upgrade support for service manager application memory map
     +-----------------------+ 0x2000FFFF
     |  RAM (64K)            |
     +-----------------------+ 0x20000000
     |                       |
     |                       |
     +-----------------------+ 0x10080000
     |                       |
     |  NVM(4K)              |
     +-----------------------+ 0x1007F000
     |                       |
     |  User app (158K)      |
     +-----------------------+ 0x10057800
     |  OTA Service          |
     |  Manager (94K)        |
     +-----------------------+ 0x10040000
     |                       |
     +-----------------------| 0x100017FF
     |   ROM (6K)            |
     +-----------------------+ 0x10000000
  */

SERVICE_MANAGER_SIZE = 0x17800;
MEMORY_FLASH_APP_SIZE = DEFINED(CONFIG_OTA_USE_SERVICE_MANAGER) ? (_MEMORY_FLASH_SIZE_ - SERVICE_MANAGER_SIZE - FLASH_NVM_DATASIZE) : (MEMORY_FLASH_APP_SIZE) ;
MEMORY_FLASH_APP_OFFSET = DEFINED(CONFIG_OTA_USE_SERVICE_MANAGER) ? (SERVICE_MANAGER_SIZE) : (MEMORY_FLASH_APP_OFFSET) ;

/* Entry Point */
ENTRY(RESET_HANDLER)


/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size  = 0x200; /* required amount of heap  */
_Min_Stack_Size = 0xC00; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
  REGION_RAM (xrw)              : ORIGIN = _MEMORY_RAM_BEGIN_, LENGTH = _MEMORY_RAM_SIZE_
  REGION_FLASH_BOOTLOADER (rx)  : ORIGIN = _MEMORY_FLASH_BEGIN_, LENGTH = MEMORY_FLASH_APP_OFFSET
  REGION_FLASH (rx)             : ORIGIN = _MEMORY_FLASH_BEGIN_ + MEMORY_FLASH_APP_OFFSET, LENGTH = MEMORY_FLASH_APP_SIZE
  REGION_NVM (rx)               : ORIGIN = _MEMORY_FLASH_END_ + 1 - FLASH_NVM_DATASIZE, LENGTH = FLASH_NVM_DATASIZE
  REGION_ROM (rx)               : ORIGIN = _MEMORY_ROM_BEGIN_, LENGTH = _MEMORY_ROM_SIZE_
}

/* Define output sections */
SECTIONS
{  
  /* The startup code goes first into FLASH */
  .bootloader (ORIGIN(REGION_FLASH_BOOTLOADER)) :
  {
    . = ALIGN(4);
    
    KEEP(*(.bootloader)) /* Startup code */
    
    . = ALIGN(4);
  } >REGION_FLASH_BOOTLOADER

  /* The startup code goes first into FLASH */
  .intvec (ORIGIN(REGION_FLASH)) :
  {
    . = ALIGN(4);
    
    KEEP(*(.intvec)) /* Startup code */
    
    . = ALIGN(4);
  } >REGION_FLASH


  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);

    KEEP(*(.cmd_call_table))
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(i.*)             /* i.* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.constdata)
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */

    . = ALIGN(4);
     _etext = .;
  } >REGION_FLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* RAM preamble, uninitialized */
  .ram_preamble 0x20000004 (NOLOAD) :
  {
    KEEP(*(.ram_vr))
  }  >REGION_RAM AT> REGION_FLASH

  /* RAM preamble, unininitialized */
  .ram_preamble_2 0x20000034 (NOLOAD) :
  {
    KEEP(*(.crash_info_ram_vr))
  }  >REGION_RAM
  /* Uninitialized data section */
   
  .bss.blueRAM 0x200000C0 :
  {
    . = ALIGN(4);
    _sbssblue = .;         /* define a global symbol at .bss.blueRAM start */
    KEEP(*(.bss.__blue_RAM))
    . = ALIGN(4);
    _ebssblue = .;         /* define a global symbol at .bss.blueRAM end */   
  } >REGION_RAM  
  
  .bss DEFINED(MEMORY_RAM_APP_OFFSET) ? (ORIGIN(REGION_RAM) + MEMORY_RAM_APP_OFFSET) : . :
  {
    . = ALIGN(4);
    _sbss = .;         /* define a global symbol at bss start */
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
  } >REGION_RAM

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >REGION_RAM AT> REGION_FLASH
  
  /* Data section that will not be initialized to any value. */
  .noinit (NOLOAD):
  {
    . = ALIGN(4);
    *(.noinit)
    . = ALIGN(4);
  } >REGION_RAM

  .heap (NOLOAD):
  {
    . = ALIGN(4);
    _sheap = .;
    . = . + _Min_Heap_Size;
    . = ALIGN(4);
    _eheap = .;
  } >REGION_RAM

  /* This is to emulate place at end of IAR linker */
  CSTACK (ORIGIN(REGION_RAM) + LENGTH(REGION_RAM) - _Min_Stack_Size) (NOLOAD) :
  {
    . = ALIGN(4);
    . = . + _Min_Stack_Size;
    . = ALIGN(4);
    _estack = .;         /* define a global symbol at bss end */
    . = ALIGN(4);
  } > REGION_RAM

  
  .rom_info (NOLOAD) :
  {
    . = ALIGN(4);
    KEEP(*(.rom_info)) 
    . = ALIGN(4);
  } >REGION_ROM
  
  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
