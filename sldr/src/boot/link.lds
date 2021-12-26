/***************************************************
*  Celestial Semiconductior Inc 2008.
*        
*  Description:  
*   (1) Secu Boot, Link Scripts
*                        
*  Author:  Jiasheng Chen (jiasheng.chen@celestialsemi.com) 
*  Rev 0.1 @ 2008.02.18 First Edition
*  Details:
*   
*   
*        
****************************************************/

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x0;/*BootCode Start Address*/
	text_start = .;
	.text :
	{
	  *(.text)
	  *(.rodata)
	  *(.rodata.*)
	  *(.glue_7)
	  *(.glue_7t)
	  *(.got)
	}
	text_end = .;
	. = ALIGN(4);
	data_img_start = .;
	data_start = .;
	.data :
	{
	  *(.data)
	}
	. = ALIGN(4);
	data_end = .;
	
	. = ALIGN(4);
	bss_start = .;
  .bss :
  {
    *(.bss)
  }
  . = ALIGN(4);
	bss_end = .;
}
