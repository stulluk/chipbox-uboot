/*
   image layout in .img file:

   ------------------------------------
   ( 1) bootargs size
   ( 1) bootcmd size
   ( x) bootargs string
   ( x) bootcmd string
   (48) address & sz on flash
   (64) image header
   ( x) u-boot
   ( x) kernel
   ( x) ramdisk  
   ------------------------------------
   */

#include <stdio.h>
#include <string.h>

#define do_encrypt	data ^= 0xe3

char firmware_st[]  = "34000000";
char firmware_len[] = "+002b120";
char kernel_st[]    = "34060000";
char kernel_len[]   = "+019aaa4";
char ramdisk_st[]   = "34200000";
char ramdisk_len[]  = "+0330000";
char bootargs[]     = "root=/dev/mtdblock/2 rw rootfstype=jffs2 console=ttyS0,115200 mem=64M mtdparts=phys_mapped_flash:384k(u-boot)ro,1664k(kernel),-(fs)";
char bootcmd[]      = "bootm 0x34060000";

char *img_ptr[]     = {firmware_st, firmware_len, kernel_st, kernel_len, ramdisk_st, ramdisk_len};

int main()
{
	FILE *fp_bin;
	FILE *fp_img;
	unsigned char data, i, j, bootargs_len, bootcmd_len, pad = 0;
	long count;
	char *ptr;

	if ((fp_bin = fopen("misc.temp", "rb")) == NULL)
	{
		printf("can not open file misc.temp!\n");
		return 0;
	}
	if ((fp_img = fopen("misc.img", "wb")) == NULL)
	{
		printf("can not open file misc.img!\n");
		return 0;
	}

	bootargs_len = strlen(bootargs);
	bootcmd_len = strlen(bootcmd);
	if (((bootargs_len + bootcmd_len + 2) % 4) != 0)
	{
		pad = 4 - ((bootargs_len + bootcmd_len + 2) % 4);
		bootcmd_len += pad;
	}

	data = bootargs_len;
	do_encrypt;
	fputc(data, fp_img);

	data = bootcmd_len;
	do_encrypt;
	fputc(data, fp_img);

	ptr = bootargs;
	while (*ptr != '\0')
	{
		data = *ptr++;
		do_encrypt;
		fputc(data, fp_img);
	}

	ptr = bootcmd;
	while (*ptr != '\0')
	{
		data = *ptr++;
		do_encrypt;
		fputc(data, fp_img);
	}

	while (pad != 0)
	{
		pad--;
		data = ' ';
		do_encrypt;
		fputc(data, fp_img);
	}

	for (i = 0; i < 6; i++)
	{
		ptr = img_ptr[i];
		for (j = 0; j < 8; j++)
		{
			data = *ptr++;
			do_encrypt;
			fputc(data, fp_img);
		}
	}

	count = 0;
	while (!feof(fp_bin))
	{
		count++;
		data = fgetc(fp_bin);
		do_encrypt;
		fputc(data, fp_img);
		if ((count % 409600) == 0)
			printf(".");
	}
	printf("\n");

	fclose(fp_bin);
	fclose(fp_img);

	return 0;
}
