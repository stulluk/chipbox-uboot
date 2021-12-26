/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Add to readline cmdline-editing by
 * (C) Copyright 2005
 * JinHua Luo, GuangDong Linux Center, <luo.jinhua@gd-linux.com>
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

/* #define	DEBUG	*/

#include <common.h>
#include <watchdog.h>
#include <command.h>
#ifdef CONFIG_MODEM_SUPPORT
#include <malloc.h>		/* for free() prototype */
#endif

#ifdef CFG_HUSH_PARSER
#include <hush.h>
#endif

#include <post.h>

#if defined(MERIH_GOLD_BOARD)    // by kb :20100416
#include <orion_fp.h>
#include <net.h>

/* For USB Download by KB Kim 2010.11.11 */
#include <usb.h>
#include "fat.h"

/* For USB Download by KB Kim 2010.11.11 */
#define ALL_FLASH_FILE_NAME     "All"
#define FLASH_FILE_NAME         "Flash"
#define UBOOT_FILE_NAME         "Boot"
#define ROOTFS_FILE_NAME        "Root"
#define CH_DB_FILE_NAME         "ChDb"
#define WORK0_FILE_NAME         "Wk0"


#define FILE_8M                  "08M.bin"
#define FILE_16M                 "16M.bin"
#define FILE_32M                 "32M.bin"

#define DEFULT_TFTP_SERVER       "85.159.65.90"
#define DEFULT_SUB_NET_MASK      "255.255.255.0"

#define FLASH_BASE_ADDRESS     CFG_FLASH_BASE
#define FLASH_KERNEL_ADDRESS   (CFG_FLASH_BASE + 0x80000)
#define FLASH_UBOOT_ADDRESS    (CFG_FLASH_BASE + 0x400)

/* For USB Download by KB Kim 2010.11.11 */
#define UPDATE_NONE            0xFF
#define UPDATE_UBOOT           0
#define UPDATE_ALL             1
#define UPDATE_WO_BOOT         2
#define UPDATE_ROOTFS          3
#define UPDATE_CH_DB           4
#define UPDATE_WORK0           5
#define UBOOT_FILE_SIZE        0x60000
#define ALL08M_FILESIZE        0x800000
#define ALL16M_FILESIZE        0x1000000
#define ALL32M_FILESIZE        0x2000000
#define ROOTFS08M_FILESIZE     0x5A0000
#define ROOTFS16M_FILESIZE     0x5C0000
#define ROOTFS32M_FILESIZE     0x5C0000
#define CH_DB_FILESIZE         0x1E0000
#define WORK_FILESIZE          0x7A0000
#define ROOTFS08M_OFFSET       0x260000
#define ROOTFS16M_OFFSET       0x860000
#define ROOTFS32M_OFFSET       0x860000
#define CH_DB_OFFSET           0xE20000
#define DOWN_ADDRESS           0x5000000

/* For USB Download by KB Kim 2010.11.11 */
ulong FlashSize         = 32;
ulong FlashWriteAddress = 0;
ulong FlashFileSize     = 0;

extern char		UpdateFile[128];

DECLARE_GLOBAL_DATA_PTR;
#endif

#ifdef CONFIG_SILENT_CONSOLE
DECLARE_GLOBAL_DATA_PTR;
#endif

/* For USB Download by KB Kim 2010.11.11 */
extern block_dev_desc_t *get_dev (char*, int);

// #if defined(CONFIG_BOOT_RETRY_TIME) && defined(CONFIG_RESET_TO_RETRY)     // by kb :20100416
extern int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);		/* for do_reset() prototype */
// #endif

extern int do_bootd (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
extern int do_protect (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);   // by kb :20100416

#define MAX_DELAY_STOP_STR 32

static int parse_line (char *, char *[]);
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
static int abortboot(int);
#endif

#undef DEBUG_PARSER

char        console_buffer[CFG_CBSIZE];		/* console I/O buffer	*/

#ifndef CONFIG_CMDLINE_EDITING
static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen);
static char erase_seq[] = "\b \b";		/* erase sequence	*/
static char   tab_seq[] = "        ";		/* used to expand TABs	*/
#endif /* CONFIG_CMDLINE_EDITING */

#ifdef CONFIG_BOOT_RETRY_TIME
static uint64_t endtime = 0;  /* must be set, default is instant timeout */
static int      retry_time = -1; /* -1 so can call readline before main_loop */
#endif

#define	endtick(seconds) (get_ticks() + (uint64_t)(seconds) * get_tbclk())

#ifndef CONFIG_BOOT_RETRY_MIN
#define CONFIG_BOOT_RETRY_MIN CONFIG_BOOT_RETRY_TIME
#endif

#ifdef CONFIG_MODEM_SUPPORT
int do_mdm_init = 0;
extern void mdm_init(void); /* defined in board.c */
#endif

#if defined(MERIH_GOLD_BOARD)   // by kb :20100416

#define ASCII_CODE_0   0x30
#define ASCII_CODE_9   0x39
#define ASCII_CODE_A   0x41
#define ASCII_CODE_F   0x46
#define ASCII_CODE_X   0x58
#define ASCII_CODE_a   0x61
#define ASCII_CODE_f   0x66
#define ASCII_CODE_x   0x78

extern void InitCAT6611(void);
extern void SetCAT6611Display(void);

extern int DrawPicture(unsigned char *pic_buf, int pic_w, int pic_h, int dst_x, int dst_y);

int IsNotHexValue(unsigned char data)
{
	if (((data >= ASCII_CODE_0) && (data <= ASCII_CODE_9)) ||
		((data >= ASCII_CODE_A) && (data <= ASCII_CODE_F)) ||
		((data >= ASCII_CODE_a) && (data <= ASCII_CODE_f)))
	{
		return 0;
	}

	return 1;
}

int GetHexFromText(unsigned char *buf, unsigned *value, int len)
{
	int count;
	int point;
	unsigned val;
	unsigned nibble;
	unsigned data;

	count = 0;
	point = 0;
	/* Skip space */
	while ((point < len) && (IsNotHexValue(buf[point])))
	{
		point++;
	}

	if ((buf[point] == ASCII_CODE_0) && ((buf[point + 1] == ASCII_CODE_X) || (buf[point + 1] == ASCII_CODE_x)))
	{
		/* Skip 0x */
		point += 2;
	}

	val = 0;
	while ((count < 8) && (point < len))
	{
		data = (unsigned)buf[point];
		if ((data >= ASCII_CODE_0) && (data <= ASCII_CODE_9))
		{
			nibble = data - ASCII_CODE_0;
		}
		else if ((data >= ASCII_CODE_A) && (data <= ASCII_CODE_F))
		{
			nibble = data - ASCII_CODE_A + 0x10;
		}
		else if ((data >= ASCII_CODE_a) && (data <= ASCII_CODE_f))
		{
			nibble = data - ASCII_CODE_a + 0x10;
		}
		else
		{
			if (val > 0)
			{
				*value = val;
			}

			// printf("GetHexFromText0 0x%08X - %d\n", val, point);
			return point;
		}

		val = (val << 4) | nibble;
		point++;
		count++;
	}

	if (val > 0)
	{
		*value = val;
	}

	// printf("GetHexFromText1 0x%08X - %d\n", val, point);
	return point;
}

int GetBCDFromText(unsigned char *buf, unsigned *value, int len)
{
	int count;
	int point;
	unsigned val;
	unsigned nibble;
	unsigned data;

	count = 0;
	point = 0;
	/* Skip space */
	while ((point < len) && ((buf[point] < ASCII_CODE_0) || (buf[point] > ASCII_CODE_9)))
	{
		point++;
	}

	val = 0;
	while ((count < 8) && (point < len))
	{
		data = (unsigned)buf[point];
		if ((data >= ASCII_CODE_0) && (data <= ASCII_CODE_9))
		{
			nibble = data - ASCII_CODE_0;
		}
		else
		{
			if (val > 0)
			{
				*value = val;
			}

			return point;
		}

		val = (val * 10) + nibble;
		point++;
		count++;
	}
	
	if (val > 0)
	{
		*value = val;
	}
	return point;
}

void GetLogoInformation (unsigned *picAddr, int *pic_w, int *pic_h, int *dst_x, int *dst_y)
{
	unsigned char tmp[256];
	int           len;
	int           point;
	int           dataLen;
	int           count;
	int           i;
	int           data[5];

	/* Set Default Value */
	data[0] = 0x38080000;
	data[1] = 435;
	data[2] = 65;
	data[3] = 130;
	data[4] = 250;

	len = getenv_r ("logo", tmp, sizeof (tmp));

	// printf("Logo Env = %s\n", tmp);
	count = 0;
	if (len > 0)
	{
		point = 0;
		while (point < len)
		{
			if (count)
			{
				dataLen = GetBCDFromText(tmp + point, &data[count], (len - point));
				// printf("BCD data[%d] = %d - %d\n", count, data[count], dataLen);
			}
			else
			{
				/* First data is HEX */
				dataLen = GetHexFromText(tmp + point, &data[count], (len - point));
				// printf("Hex data[%d] = 0x%08X - %d\n", count, data[count], dataLen);
			}
			point += dataLen;
			count++;

			if (count >= 5)
			{
				/* Get All Data */
				point = len;

				break;
			}
		}
	}
	*picAddr = (unsigned)data[0];
	*pic_w   = data[1];
	*pic_h   = data[2];
	*dst_x   = data[3];
	*dst_y   = data[4];

	// printf("Logo Data : 0x%08X - W[%d] H[%d] X[%d] Y[%d]\n", data[0], data[1], data[2], data[3], data[4]);
}

void ResetByKey(void)
{
	int len;
	unsigned char data[MAX_FB_RECV_LENGTH];
	
	while(1)
	{
		len = FbReceiveFrontData(data, 1000);
		if (len > 2)
		{
			if ((data[0] == FB_FRONT_KEY) || (data[0] == FB_RCU_KEY))
			{
				/* Reinit board to run initialization code again */
				do_reset (NULL, 0, 0, NULL);
				return;
			}
		}
	}
}

int SaveToFlash(ulong flashAddr, char *buffer, ulong size)
{
	ulong flashend;
	int rc;
	int argc = 3;
	char *argv[] = {"protect", "off", "all"};
	char *pOn = "on";

	flashend = flashAddr + size - 1;

	// flash_sect_protect (0, flashAddr, flashend);
	do_protect((cmd_tbl_t *) NULL, 0, argc, argv);
	FbSendFndDisplay("Eras");
	if (flash_sect_erase (flashAddr, flashend) == 1)
	{
		/* Error to get DHCP IP Address */
		FbSendFndDisplay("Eerr"); /* Flash Erase Error */
		return (-1);
	}

	FbSendFndDisplay("Save");
	rc = flash_write (buffer, flashAddr, size);
	if (rc != ERR_OK)
	{
		FbSendFndDisplay("Werr");
		return (-1);
	}
	argv[1] = pOn;
	do_protect((cmd_tbl_t *) NULL, 0, argc, argv);

	return 0;
}

int CheckNetConfigulation(void)
{
	unsigned char tmp[22];
	bd_t *bd = gd->bd;
	
	NetCopyIP(&NetOurIP, &bd->bi_ip_addr);
	NetOurGatewayIP  = getenv_IPaddr ("gatewayip");
	if ((NetOurIP == 0) || (NetOurGatewayIP == 0))
	{
		FbSendFndDisplay("dhcp");
		if (NetLoop(DHCP) < 0)
		{
			/* Error to get DHCP IP Address */
			FbSendFndDisplay("Nerr");
			printf ("CheckBootMode Tftp AutoUpdate Error : Cannot get IP Address from DHCP\n");
			ResetByKey();

			return (-1);
		}
		else
		{
			ip_to_string (NetOurIP, tmp);
			setenv ("ipaddr", tmp);
			ip_to_string (NetOurGatewayIP, tmp);
			setenv ("gatewayip", tmp);
			ip_to_string (NetOurSubnetMask, tmp);
			setenv ("netmask", tmp);
			ip_to_string (NetOurDNSIP, tmp);
			setenv ("dnsip", tmp);
		}
	}
	else
	{
		NetOurSubnetMask = getenv_IPaddr ("netmask");
		if (NetOurSubnetMask == 0)
		{
			NetOurSubnetMask = string_to_ip(DEFULT_SUB_NET_MASK);
			setenv ("netmask", DEFULT_SUB_NET_MASK);							
		}
	}

	return 0;
}

/* For USB Download by KB Kim 2010.11.11 */

int CopyFileName (char *dst, char *src, int size)
{
	int count;

	count = 0;
	while ((count < size) && (*src != '\0') && (*src != 0))
	{
		*dst = *src;
		dst++;
		src++;
		count++;
	}
	
	*dst = '\0';

	return count;
}

void GetFileInfo(U8 mode)
{
	int   len;
	int   count;
	char  tmp[128];

	memset(UpdateFile, 0x00, 128);
	memset(tmp, 0x00, 128);
	count = 0;
	switch (mode)
	{
		case UPDATE_UBOOT  :
			len = getenv_r ("bootfile", tmp, sizeof (tmp));
			if (len > 0)
			{
				CopyFileName (UpdateFile, tmp, 128);
			}
			else
			{
				count = CopyFileName (UpdateFile, UBOOT_FILE_NAME, 128);
			}
			break;
		case UPDATE_ROOTFS :
			len = getenv_r ("roofsfile", tmp, sizeof (tmp));
			if (len > 0)
			{
				CopyFileName (UpdateFile, tmp, 128);
			}
			else
			{
				count = CopyFileName (UpdateFile, ROOTFS_FILE_NAME, 128);
			}
			break;
		case UPDATE_CH_DB :
			len = getenv_r ("chdbfile", tmp, sizeof (tmp));
			if (len > 0)
			{
				CopyFileName (UpdateFile, tmp, 128);
			}
			else
			{
				count = CopyFileName (UpdateFile, CH_DB_FILE_NAME, 128);
			}
			break;
		case UPDATE_WORK0 :
			len = getenv_r ("work0file", tmp, sizeof (tmp));
			if (len > 0)
			{
				CopyFileName (UpdateFile, tmp, 128);
			}
			else
			{
				count = CopyFileName (UpdateFile, WORK0_FILE_NAME, 128);
			}
			break;
		case UPDATE_ALL     :
		case UPDATE_WO_BOOT :
		default             :
			len = getenv_r ("allfile", tmp, sizeof (tmp));
			if (len > 0)
			{
				CopyFileName (UpdateFile, tmp, 128);
			}
			else
			{
				count = CopyFileName (UpdateFile, ALL_FLASH_FILE_NAME, 128);
			}
			break;
	}

	printf("GetFileName default file name : %s , count :%d\n", UpdateFile, count);

	/* Set default file name by Flash size */
	switch (FlashSize)
	{
		case   8:
			switch(mode)
			{
				case UPDATE_UBOOT  :
					FlashWriteAddress = FLASH_BASE_ADDRESS;
					FlashFileSize     = UBOOT_FILE_SIZE;
					break;
				case UPDATE_ROOTFS :
					FlashWriteAddress = FLASH_BASE_ADDRESS + ROOTFS08M_OFFSET;
					FlashFileSize    = ROOTFS08M_FILESIZE;
					break;
				case UPDATE_ALL     :
				case UPDATE_WO_BOOT :
				default             :
					FlashWriteAddress = FLASH_BASE_ADDRESS;
					FlashFileSize     = ALL08M_FILESIZE;
					break;
			}
			
			if (count > 0)
			{
				CopyFileName (UpdateFile + count, FILE_8M, (128 - count));
			}
			break;
		case  16:
			switch(mode)
			{
				case UPDATE_UBOOT  :
					FlashWriteAddress = FLASH_BASE_ADDRESS;
					FlashFileSize     = UBOOT_FILE_SIZE;
					break;
				case UPDATE_ROOTFS :
					FlashWriteAddress = FLASH_BASE_ADDRESS + ROOTFS16M_OFFSET;
					FlashFileSize    = ROOTFS16M_FILESIZE;
					break;
				case UPDATE_CH_DB :
					FlashWriteAddress = FLASH_BASE_ADDRESS + CH_DB_OFFSET;
					FlashFileSize    = CH_DB_FILESIZE;
					break;
				case UPDATE_ALL     :
				case UPDATE_WO_BOOT :
				default             :
					FlashWriteAddress = FLASH_BASE_ADDRESS;
					FlashFileSize     = ALL16M_FILESIZE;
					break;
			}
			
			if (count > 0)
			{
				CopyFileName (UpdateFile + count, FILE_16M, (128 - count));
			}
			
			break;
		case  32:
			switch(mode)
			{
				case UPDATE_UBOOT  :
					FlashWriteAddress = FLASH_BASE_ADDRESS;
					FlashFileSize     = UBOOT_FILE_SIZE;
					break;
				case UPDATE_ROOTFS :
					FlashWriteAddress = FLASH_BASE_ADDRESS + ROOTFS32M_OFFSET;
					FlashFileSize     = ROOTFS32M_FILESIZE;
					break;
				case UPDATE_CH_DB :
					FlashWriteAddress = FLASH_BASE_ADDRESS + CH_DB_OFFSET;
					FlashFileSize     = CH_DB_FILESIZE;
					break;
				case UPDATE_WORK0 :
					FlashWriteAddress = FLASH_BASE_ADDRESS + ALL16M_FILESIZE + UBOOT_FILE_SIZE;
					FlashFileSize     = WORK_FILESIZE;
					break;
				case UPDATE_ALL     :
				case UPDATE_WO_BOOT :
				default             :
					FlashWriteAddress = FLASH_BASE_ADDRESS;
					FlashFileSize     = ALL32M_FILESIZE;
					break;
			}
			
			if (count > 0)
			{
				CopyFileName (UpdateFile + count, FILE_32M, (128 - count));
			}
			break;
	}

}

long UsbAutoUpdate(void)
{
	block_dev_desc_t *storDevice = NULL;
	int   currentUsbDevice;
	long  fileSize;
	int   sleepCount;

	printf ("UsbAutoUpdate : Start\n");

	FbSendFndDisplay("USB");
	if (usb_stop() < 0)
	{
		printf ("UsbAutoUpdate : USB Stop error\n");
		FbSendFndDisplay("err1");
		return (-1);
	}
	if (usb_init() < 0)
	{
		printf ("UsbAutoUpdate : USB Init error\n");
		FbSendFndDisplay("err2");
		return (-1);
	}

	/*
	 * check whether a storage device is attached (assume that it's
	 * a USB memory stick, since nothing else should be attached).
	 */
	currentUsbDevice = usb_stor_scan(1);
	if (currentUsbDevice == (-1))
	{
		currentUsbDevice = usb_stor_scan(1);
	}
	if (currentUsbDevice == (-1))
	{
		printf ("UsbAutoUpdate : No USB Device found\n");
		FbSendFndDisplay("nUSB");
		return (-1);
	}

	printf ("UsbAutoUpdate : Current USB Device %d\n", currentUsbDevice);
	
	/* check whether it has a partition table */
	storDevice = get_dev("usb", 0);
	if (storDevice == NULL)
	{
		printf ("UsbAutoUpdate : Un known device\n");
		FbSendFndDisplay("err3");
		return (-1);
	}

	/* Register USB device as FAT Device */
	if (fat_register_device(storDevice, 1) != 0)
	{
		printf ("UsbAutoUpdate : Unable to use USB %d:%d for fatls\n", currentUsbDevice, 1);
		FbSendFndDisplay("err3");
		return (-1);
	}

	/* Detect FAT File system*/
	if (file_fat_detectfs() != 0)
	{
		printf ("UsbAutoUpdate : Can not find Fat\n");
		FbSendFndDisplay("err4");
		return (-1);
	}

	fileSize = file_fat_read(UpdateFile, (char *)load_addr, FlashFileSize);
	if (fileSize <= 0)
	{
		printf ("UsbAutoUpdate : Can not Read File\n");
		FbSendFndDisplay("err5");
		return (-1);
	}

	return fileSize;
}

long TftpAutoUpdate(void)
{
	long  fileSize;
	char  tmp[128];
	/* Tftp Auto-Update Function */

	if (CheckNetConfigulation() < 0)
	{
		return (-1);
	}

	FbSendFndDisplay("tftp");
	NetServerIP = getenv_IPaddr ("serverip");
	if (NetServerIP == 0)
	{
		NetServerIP = string_to_ip(DEFULT_TFTP_SERVER);
		setenv ("serverip", DEFULT_TFTP_SERVER);
		printf ("CheckBootMode Info : Set ServerIP Default\n");
	}
	ip_to_string(NetServerIP, tmp);
	printf ("CheckBootMode Info : Set ServerIP %s\n", tmp);
	printf("netboot_common : Load %s file to Addr 0x%X\n", UpdateFile, load_addr);
	fileSize = (long)NetLoop(TFTP);
	if (fileSize < 0)
	{
		FbSendFndDisplay("derr");
		return (-1);
	}
	
	return fileSize;	
}

int UpdateBootLoader(void)
{
	ulong flashAddress;

	flashAddress = FLASH_BASE_ADDRESS;
	FbSendFndDisplay("B--0");
	if (SaveToFlash(flashAddress, (char *)load_addr, UBOOT_FILE_SIZE) != 0)
	{
		ResetByKey();

		return (-1);
	}
	
	if (FlashSize > 8)
	{
		FbSendFndDisplay("B--1");
		flashAddress += ALL08M_FILESIZE;
		if (SaveToFlash(flashAddress, (char *)load_addr, UBOOT_FILE_SIZE) != 0)
		{
			ResetByKey();

			return (-1);
		}
	}
	
	if (FlashSize > 16)
	{
		FbSendFndDisplay("B--2");
		flashAddress += ALL08M_FILESIZE;
		if (SaveToFlash(flashAddress, (char *)load_addr, UBOOT_FILE_SIZE) != 0)
		{
			ResetByKey();

			return (-1);
		}
		FbSendFndDisplay("B--3");
		flashAddress += ALL08M_FILESIZE;
		if (SaveToFlash(flashAddress, (char *)load_addr, UBOOT_FILE_SIZE) != 0)
		{
			ResetByKey();

			return (-1);
		}
	}

	return 0;
}

int UpdateAllWoLoader(void)
{
	ulong flashAddress;
	ulong dataAddress;

	flashAddress = FLASH_BASE_ADDRESS + UBOOT_FILE_SIZE;
	dataAddress = load_addr + UBOOT_FILE_SIZE;
	
	FbSendFndDisplay("Syst");
	if (SaveToFlash(flashAddress, (char *)dataAddress, WORK_FILESIZE) != 0)
	{
		ResetByKey();

		return (-1);
	}
	
	if (FlashSize > 8)
	{
		FbSendFndDisplay("Root");
		flashAddress += ALL08M_FILESIZE;
		dataAddress  += ALL08M_FILESIZE;
		if (SaveToFlash(flashAddress, (char *)dataAddress, WORK_FILESIZE) != 0)
		{
			ResetByKey();

			return (-1);
		}
	}
	
	if (FlashSize > 16)
	{
		FbSendFndDisplay("WK-0");
		flashAddress += ALL08M_FILESIZE;
		dataAddress  += ALL08M_FILESIZE;
		if (SaveToFlash(flashAddress, (char *)dataAddress, WORK_FILESIZE) != 0)
		{
			ResetByKey();

			return (-1);
		}
		FbSendFndDisplay("WK-1");
		flashAddress += ALL08M_FILESIZE;
		dataAddress  += ALL08M_FILESIZE;
		if (SaveToFlash(flashAddress, (char *)dataAddress, WORK_FILESIZE) != 0)
		{
			ResetByKey();

			return (-1);
		}
	}

	return 0;
}

void CheckBootMode(void)
{
	int           sleepCount;
	int           len;
	long          fileSize;
	unsigned char count;
	unsigned char data[MAX_FB_RECV_LENGTH];
	unsigned char bootMode;
	unsigned char holdMode;
	U8            mode = UPDATE_NONE;

	holdMode = 0;
	FbRquestBootMode();
	// FbGetAck();
	len = FbReceiveFrontData(data, 1000);

	if ((data[0] == FB_BOOT_MODE) && (data[1] == 1))
	{
		bootMode = data[2];
		// printf ("CheckBootMode Info : BootMode[0x%02X]\n", bootMode);
		FbSendBootEnd();
		if (bootMode != FB_BOOT_MODE_WATCH)
		{
			FbSendFndDisplay("boot");
		}
		
		if (bootMode == FB_BOOT_MODE_AC_ON)
		{
			len = FbReceiveFrontData(data, 1000);
			if ((len > 2) && (data[0] == FB_FRONT_KEY))
			{
				switch(data[2])
				{
					case FB_KEY_HOLD :
						FbSendFndDisplay("hold");
						holdMode = 1;
						while(holdMode)
						{
							len = FbReceiveFrontData(data, 1000);
							if (len > 2)
							{
								printf ("CheckBootMode Info Hold : Header[0x%02X] Length[%d]", data[0], data[1]);
								for (count = 2; count < len; count++)
								{
									printf (" Data%d[0x%02X]", (count-2), data[count]);
								}
								printf ("\n");
								if ((data[0] == FB_FRONT_KEY) && (data[2] == FB_KEY_POWER))
								{
									holdMode = 0;
									FbSendFndDisplay("boot");
								}
							}
						}
						break;
					case FB_KEY_AUTO_UPDATE_ALL     :
						mode = UPDATE_ALL;
						break;
					case FB_KEY_AUTO_UPDATE_WO_BOOT :
						mode = UPDATE_WO_BOOT;
						break;
					case FB_KEY_AUTO_UPDATE_ROOTFS  :
						mode = UPDATE_ROOTFS;
						break;
					case FB_KEY_AUTO_UPDATE_UBOOT   :
						mode = UPDATE_UBOOT;
						break;
					case FB_KEY_AUTO_UPDATE_CHDB    :
						mode = UPDATE_CH_DB;
						break;
					case FB_KEY_AUTO_UPDATE_WORK0   :
						mode = UPDATE_WORK0;
						break;
					default :
						break;
				}

				if (mode != UPDATE_NONE)
				{
					FbSendFndDisplay("auto");
					load_addr = DOWN_ADDRESS;
					GetFileInfo(mode);
					printf("CheckBootMode File name : %s\n", UpdateFile);
					FbFlushFrontData();
					fileSize = UsbAutoUpdate();
					if (fileSize <= 0)
					{
						fileSize = TftpAutoUpdate();
					}

					if (fileSize > 0)
					{
						if (mode > UPDATE_WO_BOOT)
						{
							if (SaveToFlash(FlashWriteAddress, (char *)load_addr, (ulong)fileSize) != 0)
							{
								ResetByKey();
							}
						}
						else
						{
							if ((mode == UPDATE_UBOOT) || (mode == UPDATE_ALL))
							{
								if (UpdateBootLoader() != 0)
								{
									ResetByKey();
								}
							}

							if ((mode == UPDATE_WO_BOOT) || (mode == UPDATE_ALL))
							{
								if (UpdateAllWoLoader() != 0)
								{
									ResetByKey();
								}
							}
						}

						FbSendFndDisplay("End-");
					}
					else
					{
						FbSendFndDisplay("derr");
						ResetByKey();
					}

					printf("CheckBootMode : Auto Update Success 0x%X\n", fileSize);

					ResetByKey();
					do_reset (NULL, 0, 0, NULL);
				}
			}
		}
	}
}
#endif

/***************************************************************************
 * Watch for 'delay' seconds for autoboot stop or autoboot delay string.
 * returns: 0 -  no key string, allow autoboot
 *          1 - got key string, abort
 */
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
# if defined(CONFIG_AUTOBOOT_KEYED)
static __inline__ int abortboot(int bootdelay)
{
	int abort = 0;
	uint64_t etime = endtick(bootdelay);
	struct {
		char* str;
		u_int len;
		int retry;
	}
	delaykey [] = {
		{ str: getenv ("bootdelaykey"),  retry: 1 },
		{ str: getenv ("bootdelaykey2"), retry: 1 },
		{ str: getenv ("bootstopkey"),   retry: 0 },
		{ str: getenv ("bootstopkey2"),  retry: 0 },
	};

	char presskey [MAX_DELAY_STOP_STR];
	u_int presskey_len = 0;
	u_int presskey_max = 0;
	u_int i;

#ifdef CONFIG_SILENT_CONSOLE
	if (gd->flags & GD_FLG_SILENT) {
		/* Restore serial console */
		console_assign (stdout, "serial");
		console_assign (stderr, "serial");
	}
#endif

#  ifdef CONFIG_AUTOBOOT_PROMPT
	printf (CONFIG_AUTOBOOT_PROMPT, bootdelay);
#  endif

#  ifdef CONFIG_AUTOBOOT_DELAY_STR
	if (delaykey[0].str == NULL)
		delaykey[0].str = CONFIG_AUTOBOOT_DELAY_STR;
#  endif
#  ifdef CONFIG_AUTOBOOT_DELAY_STR2
	if (delaykey[1].str == NULL)
		delaykey[1].str = CONFIG_AUTOBOOT_DELAY_STR2;
#  endif
#  ifdef CONFIG_AUTOBOOT_STOP_STR
	if (delaykey[2].str == NULL)
		delaykey[2].str = CONFIG_AUTOBOOT_STOP_STR;
#  endif
#  ifdef CONFIG_AUTOBOOT_STOP_STR2
	if (delaykey[3].str == NULL)
		delaykey[3].str = CONFIG_AUTOBOOT_STOP_STR2;
#  endif

	for (i = 0; i < sizeof(delaykey) / sizeof(delaykey[0]); i ++) {
		delaykey[i].len = delaykey[i].str == NULL ?
				    0 : strlen (delaykey[i].str);
		delaykey[i].len = delaykey[i].len > MAX_DELAY_STOP_STR ?
				    MAX_DELAY_STOP_STR : delaykey[i].len;

		presskey_max = presskey_max > delaykey[i].len ?
				    presskey_max : delaykey[i].len;

#  if DEBUG_BOOTKEYS
		printf("%s key:<%s>\n",
		       delaykey[i].retry ? "delay" : "stop",
		       delaykey[i].str ? delaykey[i].str : "NULL");
#  endif
	}

	/* In order to keep up with incoming data, check timeout only
	 * when catch up.
	 */
	while (!abort && get_ticks() <= etime) {
		for (i = 0; i < sizeof(delaykey) / sizeof(delaykey[0]); i ++) {
			if (delaykey[i].len > 0 &&
			    presskey_len >= delaykey[i].len &&
			    memcmp (presskey + presskey_len - delaykey[i].len,
				    delaykey[i].str,
				    delaykey[i].len) == 0) {
#  if DEBUG_BOOTKEYS
				printf("got %skey\n",
				       delaykey[i].retry ? "delay" : "stop");
#  endif

#  ifdef CONFIG_BOOT_RETRY_TIME
				/* don't retry auto boot */
				if (! delaykey[i].retry)
					retry_time = -1;
#  endif
				abort = 1;
			}
		}

		if (tstc()) {
			if (presskey_len < presskey_max) {
				presskey [presskey_len ++] = getc();
			}
			else {
				for (i = 0; i < presskey_max - 1; i ++)
					presskey [i] = presskey [i + 1];

				presskey [i] = getc();
			}
		}
	}
#  if DEBUG_BOOTKEYS
	if (!abort)
		puts ("key timeout\n");
#  endif

#ifdef CONFIG_SILENT_CONSOLE
	if (abort) {
		/* permanently enable normal console output */
		gd->flags &= ~(GD_FLG_SILENT);
	} else if (gd->flags & GD_FLG_SILENT) {
		/* Restore silent console */
		console_assign (stdout, "nulldev");
		console_assign (stderr, "nulldev");
	}
#endif

	return abort;
}

# else	/* !defined(CONFIG_AUTOBOOT_KEYED) */

#ifdef CONFIG_MENUKEY
static int menukey = 0;
#endif

static __inline__ int abortboot(int bootdelay)
{
	int abort = 0;

#ifdef CONFIG_SILENT_CONSOLE
	if (gd->flags & GD_FLG_SILENT) {
		/* Restore serial console */
		console_assign (stdout, "serial");
		console_assign (stderr, "serial");
	}
#endif

#ifdef CONFIG_MENUPROMPT
	printf(CONFIG_MENUPROMPT, bootdelay);
#else
	printf("Hit any key to stop autoboot: %2d ", bootdelay);
#endif

#if defined CONFIG_ZERO_BOOTDELAY_CHECK
	/*
	 * Check if key already pressed
	 * Don't check if bootdelay < 0
	 */
	if (bootdelay >= 0) {
		if (tstc()) {	/* we got a key press	*/
			(void) getc();  /* consume input	*/
			puts ("\b\b\b 0");
			abort = 1; 	/* don't auto boot	*/
		}
	}
#endif

	while ((bootdelay > 0) && (!abort)) {
		int i;

		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<100; ++i) {
			if (tstc()) {	/* we got a key press	*/
				abort  = 1;	/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/
# ifdef CONFIG_MENUKEY
				menukey = getc();
# else
				(void) getc();  /* consume input	*/
# endif
				break;
			}
			udelay (10000);
		}

		printf ("\b\b\b%2d ", bootdelay);
	}

	putc ('\n');

#ifdef CONFIG_SILENT_CONSOLE
	if (abort) {
		/* permanently enable normal console output */
		gd->flags &= ~(GD_FLG_SILENT);
	} else if (gd->flags & GD_FLG_SILENT) {
		/* Restore silent console */
		console_assign (stdout, "nulldev");
		console_assign (stderr, "nulldev");
	}
#endif

	return abort;
}
# endif	/* CONFIG_AUTOBOOT_KEYED */
#endif	/* CONFIG_BOOTDELAY >= 0  */

/****************************************************************************/

void main_loop (void)
{
#if defined(MERIH_GOLD_BOARD) /* For Boot Logo By KB Kim : 20101231 */
	unsigned logoAddr;
	int      logoW;
	int      logoH;
	int      logoX;
	int      logoY;
#endif // #if defined(MERIH_GOLD_BOARD)

#ifndef CFG_HUSH_PARSER
	static char lastcommand[CFG_CBSIZE] = { 0, };
	int len;
	int rc = 1;
	int flag;
#endif

#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
	char *s;
	int bootdelay;
#endif
#ifdef CONFIG_PREBOOT
	char *p;
#endif
#ifdef CONFIG_BOOTCOUNT_LIMIT
	unsigned long bootcount = 0;
	unsigned long bootlimit = 0;
	char *bcs;
	char bcs_set[16];
#endif /* CONFIG_BOOTCOUNT_LIMIT */

#if defined(CONFIG_VFD) && defined(VFD_TEST_LOGO)
	ulong bmp = 0;		/* default bitmap */
	extern int trab_vfd (ulong bitmap);

#ifdef CONFIG_MODEM_SUPPORT
	if (do_mdm_init)
		bmp = 1;	/* alternate bitmap */
#endif
	trab_vfd (bmp);
#endif	/* CONFIG_VFD && VFD_TEST_LOGO */

#ifdef CONFIG_BOOTCOUNT_LIMIT
	bootcount = bootcount_load();
	bootcount++;
	bootcount_store (bootcount);
	sprintf (bcs_set, "%lu", bootcount);
	setenv ("bootcount", bcs_set);
	bcs = getenv ("bootlimit");
	bootlimit = bcs ? simple_strtoul (bcs, NULL, 10) : 0;
#endif /* CONFIG_BOOTCOUNT_LIMIT */

#ifdef CONFIG_MODEM_SUPPORT
	debug ("DEBUG: main_loop:   do_mdm_init=%d\n", do_mdm_init);
	if (do_mdm_init) {
		char *str = strdup(getenv("mdm_cmd"));
		setenv ("preboot", str);  /* set or delete definition */
		if (str != NULL)
			free (str);
		mdm_init(); /* wait for modem connection */
	}
#endif  /* CONFIG_MODEM_SUPPORT */

#ifdef CONFIG_VERSION_VARIABLE
	{
		extern char version_string[];

		setenv ("ver", version_string);  /* set version variable */
	}
#endif /* CONFIG_VERSION_VARIABLE */

#ifdef CFG_HUSH_PARSER
	u_boot_hush_start ();
#endif

#ifdef CONFIG_AUTO_COMPLETE
	install_auto_complete();
#endif

#ifdef CONFIG_PREBOOT
	if ((p = getenv ("preboot")) != NULL) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

# ifndef CFG_HUSH_PARSER
		run_command (p, 0);
# else
		parse_string_outer(p, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
# endif

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}
#endif /* CONFIG_PREBOOT */

#if defined(MERIH_GOLD_BOARD)   // by kb :20100416
	FbInt(19200, 2); /* Baudrate = 19200, Parity NONE */
	/* For Boot Logo By KB Kim : 20101231 */
	InitCAT6611();
	GetLogoInformation (&logoAddr, &logoW, &logoH, &logoX, &logoY);
	DrawPicture((unsigned char *)logoAddr, logoW, logoH, logoX, logoY);
	SetCAT6611Display();
#endif

#ifdef CFG_CMD_CSLOGO
	//run_command("logo 0x34030000 344 190 188 193", 0);
#endif
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
	s = getenv ("bootdelay");
	bootdelay = s ? (int)simple_strtol(s, NULL, 10) : CONFIG_BOOTDELAY;

	debug ("### main_loop entered: bootdelay=%d\n\n", bootdelay);

# ifdef CONFIG_BOOT_RETRY_TIME
	init_cmd_timeout ();
# endif	/* CONFIG_BOOT_RETRY_TIME */

#ifdef CONFIG_BOOTCOUNT_LIMIT
	if (bootlimit && (bootcount > bootlimit)) {
		printf ("Warning: Bootlimit (%u) exceeded. Using altbootcmd.\n",
		        (unsigned)bootlimit);
		s = getenv ("altbootcmd");
	}
	else
#endif /* CONFIG_BOOTCOUNT_LIMIT */
		s = getenv ("bootcmd");

	debug ("### main_loop: bootcmd=\"%s\"\n", s ? s : "<UNDEFINED>");

	if (bootdelay >= 0 && s && !abortboot (bootdelay)) {
		// printf("KB : !abortboot\n");
#if defined(MERIH_GOLD_BOARD)    // by kb :20100416
	CheckBootMode();
#endif // #if defined(MERIH_GOLD_BOARD)

# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

# ifndef CFG_HUSH_PARSER
		run_command (s, 0);
# else
		parse_string_outer(s, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
# endif

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}

# ifdef CONFIG_MENUKEY
	if (menukey == CONFIG_MENUKEY) {
	    s = getenv("menucmd");
	    if (s) {
# ifndef CFG_HUSH_PARSER
		run_command (s, 0);
# else
		parse_string_outer(s, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
# endif
	    }
	}
#endif /* CONFIG_MENUKEY */
#endif	/* CONFIG_BOOTDELAY */

#ifdef CONFIG_AMIGAONEG3SE
	{
	    extern void video_banner(void);
	    video_banner();
	}
#endif

	/*
	 * Main Loop for Monitor Command Processing
	 */
#ifdef CFG_HUSH_PARSER
	parse_file_outer();
	/* This point is never reached */
	for (;;);
#else
	for (;;) {
#ifdef CONFIG_BOOT_RETRY_TIME
		if (rc >= 0) {
			/* Saw enough of a valid command to
			 * restart the timeout.
			 */
			reset_cmd_timeout();
		}
#endif
		len = readline (CFG_PROMPT);

		flag = 0;	/* assume no special flags for now */
		if (len > 0)
			strcpy (lastcommand, console_buffer);
		else if (len == 0)
			flag |= CMD_FLAG_REPEAT;
#ifdef CONFIG_BOOT_RETRY_TIME
		else if (len == -2) {
			/* -2 means timed out, retry autoboot
			 */
			puts ("\nTimed out waiting for command\n");
# ifdef CONFIG_RESET_TO_RETRY
			/* Reinit board to run initialization code again */
			do_reset (NULL, 0, 0, NULL);
# else
			return;		/* retry autoboot */
# endif
		}
#endif

		if (len == -1)
			puts ("<INTERRUPT>\n");
		else
			rc = run_command (lastcommand, flag);

		if (rc <= 0) {
			/* invalid command or not repeatable, forget it */
			lastcommand[0] = 0;
		}
	}
#endif /*CFG_HUSH_PARSER*/
}

#ifdef CONFIG_BOOT_RETRY_TIME
/***************************************************************************
 * initialize command line timeout
 */
void init_cmd_timeout(void)
{
	char *s = getenv ("bootretry");

	if (s != NULL)
		retry_time = (int)simple_strtol(s, NULL, 10);
	else
		retry_time =  CONFIG_BOOT_RETRY_TIME;

	if (retry_time >= 0 && retry_time < CONFIG_BOOT_RETRY_MIN)
		retry_time = CONFIG_BOOT_RETRY_MIN;
}

/***************************************************************************
 * reset command line timeout to retry_time seconds
 */
void reset_cmd_timeout(void)
{
	endtime = endtick(retry_time);
}
#endif

#ifdef CONFIG_CMDLINE_EDITING

/*
 * cmdline-editing related codes from vivi.
 * Author: Janghoon Lyu <nandy@mizi.com>
 */

#define putnstr(str,n)	do {			\
		printf ("%.*s", n, str);	\
	} while (0)

#define CTL_CH(c)		((c) - 'a' + 1)

#define MAX_CMDBUF_SIZE		256

#define CTL_BACKSPACE		('\b')
#define DEL			((char)255)
#define DEL7			((char)127)
#define CREAD_HIST_CHAR		('!')

#define getcmd_putch(ch)	putc(ch)
#define getcmd_getch()		getc()
#define getcmd_cbeep()		getcmd_putch('\a')

#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

static int hist_max = 0;
static int hist_add_idx = 0;
static int hist_cur = -1;
unsigned hist_num = 0;

char* hist_list[HIST_MAX];
char hist_lines[HIST_MAX][HIST_SIZE];

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)

static void hist_init(void)
{
	int i;

	hist_max = 0;
	hist_add_idx = 0;
	hist_cur = -1;
	hist_num = 0;

	for (i = 0; i < HIST_MAX; i++) {
		hist_list[i] = hist_lines[i];
		hist_list[i][0] = '\0';
	}
}

static void cread_add_to_hist(char *line)
{
	strcpy(hist_list[hist_add_idx], line);

	if (++hist_add_idx >= HIST_MAX)
		hist_add_idx = 0;

	if (hist_add_idx > hist_max)
		hist_max = hist_add_idx;

	hist_num++;
}

static char* hist_prev(void)
{
	char *ret;
	int old_cur;

	if (hist_cur < 0)
		return NULL;

	old_cur = hist_cur;
	if (--hist_cur < 0)
		hist_cur = hist_max;

	if (hist_cur == hist_add_idx) {
		hist_cur = old_cur;
		ret = NULL;
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

static char* hist_next(void)
{
	char *ret;

	if (hist_cur < 0)
		return NULL;

	if (hist_cur == hist_add_idx)
		return NULL;

	if (++hist_cur > hist_max)
		hist_cur = 0;

	if (hist_cur == hist_add_idx) {
		ret = "";
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

#ifndef CONFIG_CMDLINE_EDITING
static void cread_print_hist_list(void)
{
	int i;
	unsigned long n;

	n = hist_num - hist_max;

	i = hist_add_idx + 1;
	while (1) {
		if (i > hist_max)
			i = 0;
		if (i == hist_add_idx)
			break;
		printf("%s\n", hist_list[i]);
		n++;
		i++;
	}
}
#endif /* CONFIG_CMDLINE_EDITING */

#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		int tmp;				\
		for (tmp = num; tmp < eol_num; tmp++)	\
			getcmd_putch(' ');		\
		while (tmp-- > num)			\
			getcmd_putch(CTL_BACKSPACE);	\
		eol_num = num;				\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(buf + num, wlen);	\
		num = eol_num;			\
	}					\
}

static void cread_add_char(char ichar, int insert, unsigned long *num,
	       unsigned long *eol_num, char *buf, unsigned long len)
{
	unsigned long wlen;

	/* room ??? */
	if (insert || *num == *eol_num) {
		if (*eol_num > len - 1) {
			getcmd_cbeep();
			return;
		}
		(*eol_num)++;
	}

	if (insert) {
		wlen = *eol_num - *num;
		if (wlen > 1) {
			memmove(&buf[*num+1], &buf[*num], wlen-1);
		}

		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
		while (--wlen) {
			getcmd_putch(CTL_BACKSPACE);
		}
	} else {
		/* echo the character */
		wlen = 1;
		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
	}
}

static void cread_add_str(char *str, int strsize, int insert, unsigned long *num,
	      unsigned long *eol_num, char *buf, unsigned long len)
{
	while (strsize--) {
		cread_add_char(*str, insert, num, eol_num, buf, len);
		str++;
	}
}

static int cread_line(char *buf, unsigned int *len)
{
	unsigned long num = 0;
	unsigned long eol_num = 0;
	unsigned long rlen;
	unsigned long wlen;
	char ichar;
	int insert = 1;
	int esc_len = 0;
	int rc = 0;
	char esc_save[8];

	while (1) {
		rlen = 1;
		ichar = getcmd_getch();

		if ((ichar == '\n') || (ichar == '\r')) {
			putc('\n');
			break;
		}

		/*
		 * handle standard linux xterm esc sequences for arrow key, etc.
		 */
		if (esc_len != 0) {
			if (esc_len == 1) {
				if (ichar == '[') {
					esc_save[esc_len] = ichar;
					esc_len = 2;
				} else {
					cread_add_str(esc_save, esc_len, insert,
						      &num, &eol_num, buf, *len);
					esc_len = 0;
				}
				continue;
			}

			switch (ichar) {

			case 'D':	/* <- key */
				ichar = CTL_CH('b');
				esc_len = 0;
				break;
			case 'C':	/* -> key */
				ichar = CTL_CH('f');
				esc_len = 0;
				break;	/* pass off to ^F handler */
			case 'H':	/* Home key */
				ichar = CTL_CH('a');
				esc_len = 0;
				break;	/* pass off to ^A handler */
			case 'A':	/* up arrow */
				ichar = CTL_CH('p');
				esc_len = 0;
				break;	/* pass off to ^P handler */
			case 'B':	/* down arrow */
				ichar = CTL_CH('n');
				esc_len = 0;
				break;	/* pass off to ^N handler */
			default:
				esc_save[esc_len++] = ichar;
				cread_add_str(esc_save, esc_len, insert,
					      &num, &eol_num, buf, *len);
				esc_len = 0;
				continue;
			}
		}

		switch (ichar) {
		case 0x1b:
			if (esc_len == 0) {
				esc_save[esc_len] = ichar;
				esc_len = 1;
			} else {
				puts("impossible condition #876\n");
				esc_len = 0;
			}
			break;

		case CTL_CH('a'):
			BEGINNING_OF_LINE();
			break;
		case CTL_CH('c'):	/* ^C - break */
			*buf = '\0';	/* discard input */
			return (-1);
		case CTL_CH('f'):
			if (num < eol_num) {
				getcmd_putch(buf[num]);
				num++;
			}
			break;
		case CTL_CH('b'):
			if (num) {
				getcmd_putch(CTL_BACKSPACE);
				num--;
			}
			break;
		case CTL_CH('d'):
			if (num < eol_num) {
				wlen = eol_num - num - 1;
				if (wlen) {
					memmove(&buf[num], &buf[num+1], wlen);
					putnstr(buf + num, wlen);
				}

				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('k'):
			ERASE_TO_EOL();
			break;
		case CTL_CH('e'):
			REFRESH_TO_EOL();
			break;
		case CTL_CH('o'):
			insert = !insert;
			break;
		case CTL_CH('x'):
			BEGINNING_OF_LINE();
			ERASE_TO_EOL();
			break;
		case DEL:
		case DEL7:
		case 8:
			if (num) {
				wlen = eol_num - num;
				num--;
				memmove(&buf[num], &buf[num+1], wlen);
				getcmd_putch(CTL_BACKSPACE);
				putnstr(buf + num, wlen);
				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('p'):
		case CTL_CH('n'):
		{
			char * hline;

			esc_len = 0;

			if (ichar == CTL_CH('p'))
				hline = hist_prev();
			else
				hline = hist_next();

			if (!hline) {
				getcmd_cbeep();
				continue;
			}

			/* nuke the current line */
			/* first, go home */
			BEGINNING_OF_LINE();

			/* erase to end of line */
			ERASE_TO_EOL();

			/* copy new line into place and display */
			strcpy(buf, hline);
			eol_num = strlen(buf);
			REFRESH_TO_EOL();
			continue;
		}
		default:
			cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
			break;
		}
	}
	*len = eol_num;
	buf[eol_num] = '\0';	/* lose the newline */

	if (buf[0] && buf[0] != CREAD_HIST_CHAR)
		cread_add_to_hist(buf);
	hist_cur = hist_add_idx;

	return (rc);
}

#endif /* CONFIG_CMDLINE_EDITING */

/****************************************************************************/

/*
 * Prompt for input and read a line.
 * If  CONFIG_BOOT_RETRY_TIME is defined and retry_time >= 0,
 * time out when time goes past endtime (timebase time in ticks).
 * Return:	number of read characters
 *		-1 if break
 *		-2 if timed out
 */
int readline (const char *const prompt)
{
#ifdef CONFIG_CMDLINE_EDITING
	char *p = console_buffer;
	unsigned int len=MAX_CMDBUF_SIZE;
	int rc;
	static int initted = 0;

	if (!initted) {
		hist_init();
		initted = 1;
	}

	puts (prompt);

	rc = cread_line(p, &len);
	return rc < 0 ? rc : len;
#else
	char   *p = console_buffer;
	int	n = 0;				/* buffer index		*/
	int	plen = 0;			/* prompt length	*/
	int	col;				/* output column cnt	*/
	char	c;

	/* print prompt */
	if (prompt) {
		plen = strlen (prompt);
		puts (prompt);
	}
	col = plen;

	for (;;) {
#ifdef CONFIG_BOOT_RETRY_TIME
		while (!tstc()) {	/* while no incoming data */
			if (retry_time >= 0 && get_ticks() > endtime)
				return (-2);	/* timed out */
		}
#endif
		WATCHDOG_RESET();		/* Trigger watchdog, if needed */

#ifdef CONFIG_SHOW_ACTIVITY
		while (!tstc()) {
			extern void show_activity(int arg);
			show_activity(0);
		}
#endif
		c = getc();

		/*
		 * Special character handling
		 */
		switch (c) {
		case '\r':				/* Enter		*/
		case '\n':
			*p = '\0';
			puts ("\r\n");
			return (p - console_buffer);

		case '\0':				/* nul			*/
			continue;

		case 0x03:				/* ^C - break		*/
			console_buffer[0] = '\0';	/* discard input */
			return (-1);

		case 0x15:				/* ^U - erase line	*/
			while (col > plen) {
				puts (erase_seq);
				--col;
			}
			p = console_buffer;
			n = 0;
			continue;

		case 0x17:				/* ^W - erase word 	*/
			p=delete_char(console_buffer, p, &col, &n, plen);
			while ((n > 0) && (*p != ' ')) {
				p=delete_char(console_buffer, p, &col, &n, plen);
			}
			continue;

		case 0x08:				/* ^H  - backspace	*/
		case 0x7F:				/* DEL - backspace	*/
			p=delete_char(console_buffer, p, &col, &n, plen);
			continue;

		default:
			/*
			 * Must be a normal character then
			 */
			if (n < CFG_CBSIZE-2) {
				if (c == '\t') {	/* expand TABs		*/
#ifdef CONFIG_AUTO_COMPLETE
					/* if auto completion triggered just continue */
					*p = '\0';
					if (cmd_auto_complete(prompt, console_buffer, &n, &col)) {
						p = console_buffer + n;	/* reset */
						continue;
					}
#endif
					puts (tab_seq+(col&07));
					col += 8 - (col&07);
				} else {
					++col;		/* echo input		*/
					putc (c);
				}
				*p++ = c;
				++n;
			} else {			/* Buffer full		*/
				putc ('\a');
			}
		}
	}
#endif /* CONFIG_CMDLINE_EDITING */
}

/****************************************************************************/

#ifndef CONFIG_CMDLINE_EDITING
static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
	char *s;

	if (*np == 0) {
		return (p);
	}

	if (*(--p) == '\t') {			/* will retype the whole line	*/
		while (*colp > plen) {
			puts (erase_seq);
			(*colp)--;
		}
		for (s=buffer; s<p; ++s) {
			if (*s == '\t') {
				puts (tab_seq+((*colp) & 07));
				*colp += 8 - ((*colp) & 07);
			} else {
				++(*colp);
				putc (*s);
			}
		}
	} else {
		puts (erase_seq);
		(*colp)--;
	}
	(*np)--;
	return (p);
}
#endif /* CONFIG_CMDLINE_EDITING */

/****************************************************************************/

int parse_line (char *line, char *argv[])
{
	int nargs = 0;

#ifdef DEBUG_PARSER
	printf ("parse_line: \"%s\"\n", line);
#endif
	while (nargs < CFG_MAXARGS) {

		/* skip any white space */
		while ((*line == ' ') || (*line == '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	printf ("** Too many args (max. %d) **\n", CFG_MAXARGS);

#ifdef DEBUG_PARSER
	printf ("parse_line: nargs=%d\n", nargs);
#endif
	return (nargs);
}

/****************************************************************************/

static void process_macros (const char *input, char *output)
{
	char c, prev;
	const char *varname_start = NULL;
	int inputcnt = strlen (input);
	int outputcnt = CFG_CBSIZE;
	int state = 0;		/* 0 = waiting for '$'  */

	/* 1 = waiting for '(' or '{' */
	/* 2 = waiting for ')' or '}' */
	/* 3 = waiting for '''  */
#ifdef DEBUG_PARSER
	char *output_start = output;

	printf ("[PROCESS_MACROS] INPUT len %d: \"%s\"\n", strlen (input),
		input);
#endif

	prev = '\0';		/* previous character   */

	while (inputcnt && outputcnt) {
		c = *input++;
		inputcnt--;

		if (state != 3) {
			/* remove one level of escape characters */
			if ((c == '\\') && (prev != '\\')) {
				if (inputcnt-- == 0)
					break;
				prev = c;
				c = *input++;
			}
		}

		switch (state) {
		case 0:	/* Waiting for (unescaped) $    */
			if ((c == '\'') && (prev != '\\')) {
				state = 3;
				break;
			}
			if ((c == '$') && (prev != '\\')) {
				state++;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		case 1:	/* Waiting for (        */
			if (c == '(' || c == '{') {
				state++;
				varname_start = input;
			} else {
				state = 0;
				*(output++) = '$';
				outputcnt--;

				if (outputcnt) {
					*(output++) = c;
					outputcnt--;
				}
			}
			break;
		case 2:	/* Waiting for )        */
			if (c == ')' || c == '}') {
				int i;
				char envname[CFG_CBSIZE], *envval;
				int envcnt = input - varname_start - 1;	/* Varname # of chars */

				/* Get the varname */
				for (i = 0; i < envcnt; i++) {
					envname[i] = varname_start[i];
				}
				envname[i] = 0;

				/* Get its value */
				envval = getenv (envname);

				/* Copy into the line if it exists */
				if (envval != NULL)
					while ((*envval) && outputcnt) {
						*(output++) = *(envval++);
						outputcnt--;
					}
				/* Look for another '$' */
				state = 0;
			}
			break;
		case 3:	/* Waiting for '        */
			if ((c == '\'') && (prev != '\\')) {
				state = 0;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		}
		prev = c;
	}

	if (outputcnt)
		*output = 0;

#ifdef DEBUG_PARSER
	printf ("[PROCESS_MACROS] OUTPUT len %d: \"%s\"\n",
		strlen (output_start), output_start);
#endif
}

/****************************************************************************
 * returns:
 *	1  - command executed, repeatable
 *	0  - command executed but not repeatable, interrupted commands are
 *	     always considered not repeatable
 *	-1 - not executed (unrecognized, bootd recursion or too many args)
 *           (If cmd is NULL or "" or longer than CFG_CBSIZE-1 it is
 *           considered unrecognized)
 *
 * WARNING:
 *
 * We must create a temporary copy of the command since the command we get
 * may be the result from getenv(), which returns a pointer directly to
 * the environment data, which may change magicly when the command we run
 * creates or modifies environment variables (like "bootp" does).
 */

int run_command (const char *cmd, int flag)
{
	cmd_tbl_t *cmdtp;
	char cmdbuf[CFG_CBSIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char finaltoken[CFG_CBSIZE];
	char *str = cmdbuf;
	char *argv[CFG_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int repeatable = 1;
	int rc = 0;

#ifdef DEBUG_PARSER
	printf ("[RUN_COMMAND] cmd[%p]=\"", cmd);
	puts (cmd ? cmd : "NULL");	/* use puts - string may be loooong */
	puts ("\"\n");
#endif

	clear_ctrlc();		/* forget any previous Control C */

	if (!cmd || !*cmd) {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CFG_CBSIZE) {
		puts ("## Command too long!\n");
		return -1;
	}

	strcpy (cmdbuf, cmd);

	/* Process separators and check for invalid
	 * repeatable commands
	 */

#ifdef DEBUG_PARSER
	printf ("[PROCESS_SEPARATORS] %s\n", cmd);
#endif
	while (*str) {

		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep=='\'') &&
			    (*(sep-1) != '\\'))
				inquotes=!inquotes;

			if (!inquotes &&
			    (*sep == ';') &&	/* separator		*/
			    ( sep != str) &&	/* past string start	*/
			    (*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
			str = sep;	/* no more commands for next pass */
#ifdef DEBUG_PARSER
		printf ("token: \"%s\"\n", token);
#endif

		/* find macros in this token and replace them */
		process_macros (token, finaltoken);

		/* Extract arguments */
		if ((argc = parse_line (finaltoken, argv)) == 0) {
			rc = -1;	/* no command at all */
			continue;
		}

		/* Look up command in command table */
		if ((cmdtp = find_cmd(argv[0])) == NULL) {
			printf ("Unknown command '%s' - try 'help'\n", argv[0]);
			rc = -1;	/* give up after bad command */
			continue;
		}

		/* found - check max args */
		if (argc > cmdtp->maxargs) {
			printf ("Usage:\n%s\n", cmdtp->usage);
			rc = -1;
			continue;
		}

#if (CONFIG_COMMANDS & CFG_CMD_BOOTD)
		/* avoid "bootd" recursion */
		if (cmdtp->cmd == do_bootd) {
#ifdef DEBUG_PARSER
			printf ("[%s]\n", finaltoken);
#endif
			if (flag & CMD_FLAG_BOOTD) {
				puts ("'bootd' recursion detected\n");
				rc = -1;
				continue;
			} else {
				flag |= CMD_FLAG_BOOTD;
			}
		}
#endif	/* CFG_CMD_BOOTD */

		/* OK - call function to do the command */
		if ((cmdtp->cmd) (cmdtp, flag, argc, argv) != 0) {
			rc = -1;
		}

		repeatable &= cmdtp->repeatable;

		/* Did the user stop this? */
		if (had_ctrlc ())
			return 0;	/* if stopped then not repeatable */
	}

	return rc ? rc : repeatable;
}

/****************************************************************************/

#if (CONFIG_COMMANDS & CFG_CMD_RUN)
int do_run (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int i;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	for (i=1; i<argc; ++i) {
		char *arg;

		if ((arg = getenv (argv[i])) == NULL) {
			printf ("## Error: \"%s\" not defined\n", argv[i]);
			return 1;
		}
#ifndef CFG_HUSH_PARSER
		if (run_command (arg, flag) == -1)
			return 1;
#else
		if (parse_string_outer(arg,
		    FLAG_PARSE_SEMICOLON | FLAG_EXIT_FROM_LOOP) != 0)
			return 1;
#endif
	}
	return 0;
}
#endif	/* CFG_CMD_RUN */
