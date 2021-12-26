
/*
 * Basic GPIO functions
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

static volatile unsigned short *gpio_base  = 
		(volatile unsigned short *)(ORION_GPIO_BASE);

int
gpio_hw_set_direct(int gpio_id, int dir)
{
	unsigned short flags = (1 << gpio_id);

	switch (dir) {
		case GPIO_READ:
			gpio_base[REG_GPIO_SWPORTA_DDR >> 1] &= ~flags;
			break;
		case GPIO_WRITE:
			gpio_base[REG_GPIO_SWPORTA_DDR >> 1] |= flags;
			break;
		default:
			return -1;
	}

	return 0;
}

unsigned short
gpio_hw_read(int gpio_id)
{
	return (gpio_base[REG_GPIO_EXT_PORTA >> 1] >> gpio_id) & 0x1;
}

int
gpio_hw_write(int gpio_id, unsigned short data)
{
	unsigned short flags = (1 << gpio_id);

	if (!data) {
		gpio_base[REG_GPIO_SWPORTA_DR >> 1] &= ~flags;
	} else {
		gpio_base[REG_GPIO_SWPORTA_DR >> 1] |= flags;
	}

	return 0;
}

#endif /* CONFIG_ORION && CONFIG_HARD_GPIO */
