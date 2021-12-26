/*
 * Basic GPIO2 functions
 *
 * Copyright (c) 2008 Celestial Semiconductor
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author: Tu BaoZhao bz.tu@celestialsemi.com, Celestial Semiconductor
 *
 * Copyright (c) 2007 Celestial Semiconductor
 *
 */

#include <common.h>

#if defined(CONFIG_CELESTIAL) && defined(CONFIG_HW_GPIO)

#define GPIO_READ 	0
#define GPIO_WRITE	1

static volatile unsigned char *gpio_base  = 
		(volatile unsigned int *)(ORION_GPIO2_BASE);

int
gpio2_hw_set_direct(int gpio_id, int dir)
{	
	unsigned int flags;
	if((gpio_id < 32)&&(gpio_id >= 0))
	{
		flags = (1 << gpio_id);
		*(unsigned int *)(gpio_base+REG_GPIO2_PINMUXA) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_PINMUXA))|(flags);
		switch (dir) {
			case GPIO_READ:
				*(unsigned int *)(gpio_base+REG_GPIO2_SWPORTA_DDR) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_SWPORTA_DDR))|(flags);
				break;
			case GPIO_WRITE:
				*(unsigned int *)(gpio_base+REG_GPIO2_SWPORTA_DDR) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_SWPORTA_DDR))&(~flags);
				break;
			default:
				return -1;
		}
	}
	else if((gpio_id >= 32)&&(gpio_id < 55))
	{
		gpio_id = gpio_id - 32;
		flags = (1 << gpio_id);
		*(unsigned int *)(gpio_base+REG_GPIO2_PINMUXB) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_PINMUXB))|(flags);
		switch (dir) {
			case GPIO_READ:
				*(unsigned int *)(gpio_base+REG_GPIO2_SWPORTB_DDR) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_SWPORTB_DDR))|(flags);
				break;
			case GPIO_WRITE:
				*(unsigned int *)(gpio_base+REG_GPIO2_SWPORTB_DDR) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_SWPORTB_DDR))&(~flags);
				break;
			default:
				return -1;
		}
	}

	return 0;
}

unsigned short
gpio2_hw_read(int gpio_id)
{
	if((gpio_id < 32)&&(gpio_id >= 0))
		return((*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_EXT_PORTA))>>gpio_id)& 0x1;
	else if((gpio_id >= 32)&&(gpio_id < 55))
	{
		gpio_id = gpio_id - 32;
		return((*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_EXT_PORTB))>>gpio_id)& 0x1;
	}
	
	return 0;
}

int
gpio2_hw_write(int gpio_id, unsigned short data)
{
	unsigned int flags ;
	if((gpio_id < 32)&&(gpio_id >= 0))
	{
		flags = (1<<gpio_id);
		if (!data) {
			*(unsigned int *)(gpio_base+REG_GPIO2_SWPORTA_DR) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_SWPORTA_DR))&(~flags);
		} else {
			*(unsigned int *)(gpio_base+REG_GPIO2_SWPORTA_DR) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_SWPORTA_DR))|(flags);
		}
	}
	else if((gpio_id >= 32)&&(gpio_id < 55))
	{
		gpio_id = gpio_id - 32;
		flags = (1<<gpio_id);
		if (!data) {
		*(unsigned int *)(gpio_base+REG_GPIO2_SWPORTB_DR) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_SWPORTB_DR))&(~flags);
			//gpio_base[REG_GPIO2_SWPORTB_DR ] &= ~flags;
		} else {
			//printf("\n befs  *(unsigned int *)(gpio_base+REG_GPIO_SWPORTB_DR) = %0x\n",*(unsigned int *)(gpio_base+REG_GPIO_SWPORTB_DR));
			*(unsigned int *)(gpio_base+REG_GPIO2_SWPORTB_DR) =(*(unsigned int *)(ORION_GPIO2_BASE+REG_GPIO2_SWPORTB_DR))|(flags);
		}
	}
	return 0;
}

#endif /* CONFIG_ORION && CONFIG_HARD_GPIO2 */