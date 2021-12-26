/*
 *  armboot - Startup Code for ARM926EJS CPU-core
 *
 *  Copyright (c) 2003  Texas Instruments
 *
 *  ----- Adapted for OMAP1610 OMAP730 from ARM925t code ------
 *
 *  Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 *  Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 *  Copyright (c) 2002	Gary Jennejohn <gj@denx.de>
 *  Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 *  Copyright (c) 2003	Kshitij <kshitij@ti.com>
 *
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

	.align	5
.globl reset_cpu
reset_cpu:                     /* Use WatchDog to Reset System */
    ldr  r1, WDT_BASE
    mov  r2, #0
    strh r2, [r1, #0x4]        /* WDT_TORR, minimum value */
    strh r2, [r1, #0x6]
    strh r2, [r1, #0x8]        /* WDT_CCVR, minimum value */
    strh r2, [r1, #0xa]  
    mov  r2, #1
    strh r2, [r1, #0x0]        /* WDT_CR, enable WDT and generated System Reset */
_loop_forever:
	b	_loop_forever

WDT_BASE:
    .word   0x101e1000
