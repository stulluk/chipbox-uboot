/*
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>

 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#if defined(CFG_ENV_IS_IN_SEEPROM) /* Environment is in EEPROM */

#include <command.h>
#include <environment.h>
#include <linux/stddef.h>

DECLARE_GLOBAL_DATA_PTR;

env_t *env_ptr = NULL;

char * env_name_spec = "SFlash";

extern uchar (*env_get_char)(int);
extern uchar env_get_char_memory (int index);


uchar env_get_char_spec (int index)
{
	uchar c;

	seeprom_read (CFG_ENV_OFFSET+index+offsetof(env_t,data), &c, 1);

	return (c);
}

void env_relocate_spec (void)
{
	seeprom_read (CFG_ENV_OFFSET, (uchar*)env_ptr, CFG_ENV_SIZE);
}

int saveenv(void)
{
	seeprom_erase(1, CFG_ENV_OFFSET, CFG_ENV_SIZE);
	return seeprom_write (CFG_ENV_OFFSET, (uchar *)env_ptr, CFG_ENV_SIZE);
}

/************************************************************************
 * Initialize Environment use
 *
 * We are still running from ROM, so data use is limited
 * Use a (moderately small) buffer on the stack
 */
int env_init(void)
{
	ulong crc, len, new;
	unsigned off;
	uchar buf[64];

	seeprom_read (CFG_ENV_OFFSET,
		     (uchar *)env_ptr, CFG_ENV_SIZE);

	crc = env_ptr->crc;

	new = 0;
	off = 0;
	len = ENV_SIZE;
	while (len > 0) {
		int n = (len > 64) ? 64 : len;

		new = crc32 (new, env_ptr->data+off, n);
		len -= n;
		off += n;
	}

	if (crc == new) {
		gd->env_addr  = offsetof(env_t,data);
		gd->env_valid = 1;
	} else {
		gd->env_addr  = 0;
		gd->env_valid = 0;
	}

	return (0);
}

#endif /* CFG_ENV_IS_IN_EEPROM */
