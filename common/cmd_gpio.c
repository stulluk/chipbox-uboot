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
#include <command.h>

#if defined(CONFIG_CELESTIAL) && defined(CONFIG_HW_GPIO)

/* For GPIO2 by KB Kim 20101231 */
int Orion_gpio_hw_set_direct(int gpio_id, int dir)
{
	if (gpio_id < 16)
	{
		return gpio_hw_set_direct(gpio_id, dir);
	}
	else if (gpio_id < 55)
	{
		return gpio2_hw_set_direct(gpio_id, dir);
	}
}

unsigned short Orion_gpio_hw_read(int gpio_id)
{
	if (gpio_id < 16)
	{
		return gpio_hw_read(gpio_id);
	}
	else if (gpio_id < 55)
	{
		return gpio2_hw_read(gpio_id);
	}	
}

int Orion_gpio_hw_write(int gpio_id, unsigned short data)
{
	if (gpio_id < 16)
	{
		return gpio_hw_write(gpio_id, data);
	}
	else if (gpio_id < 55)
	{
		return gpio2_hw_write(gpio_id, data);
	}	
}

int do_gpio(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int gpio_id;
	unsigned short data;

	if (argc < 3 || argc > 4) {
		printf("Usage: %s\n", cmdtp->help);
		return -1;
	}

	gpio_id = simple_strtoul(argv[2], NULL, 0);

	/* For GPIO2 by KB Kim 20101231 */
	if (gpio_id < 0 || gpio_id >54) {
		printf("%s: invalid parameters! at line: %d\n", 
			__FUNCTION__, __LINE__);
		return -1;
	}

	if (strcmp(argv[1], "read") == 0) {
		if (argc > 3) {
			printf("Usage: %s\n", cmdtp->help);
			return -1;
		}

		Orion_gpio_hw_set_direct(gpio_id, 0);
		data = Orion_gpio_hw_read(gpio_id);
		printf("gpio[%d] = %u\n", gpio_id, data);
	} else if (strcmp(argv[1], "write") == 0) {
		if (argc < 4) {
			printf("Usage: %s\n", cmdtp->help);
			return -1;
		}

		data = simple_strtoul(argv[3], NULL, 0);
		Orion_gpio_hw_set_direct(gpio_id, 1);
		Orion_gpio_hw_write(gpio_id, data);
		printf("Write ok!\n");
	} else {
		printf("Usage: %s\n", cmdtp->help);
		return -1;
	}

	return 0;
}

U_BOOT_CMD(
	gpio,	CFG_MAXARGS/*5*/,	1,	do_gpio,
	"gpio    - gpio read/write bit\n",
	"\ngpio <read>  <gpio_id>\n"
	"gpio <write> <gpio_id> <data>\n"
	);

#endif /* CONFIG_ORION && CONFIG_HARD_GPIO */
