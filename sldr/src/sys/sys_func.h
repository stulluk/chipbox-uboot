#ifndef _SYS_FUNC_H_
#define _SYS_FUNC_H_

//#define SPI_NAND_BOOT

/* SPI_BOOT NAND_BOOT NOR_BOOT */
//#define MANUAL_BOOT_MODE SPI_BOOT

#define	APP_SIG_START 		 0
#define	APP_SIG_LEN 		(2048/8)
#define	APP_IMG_SIZE_START      (APP_SIG_START + APP_SIG_LEN)
#define	APP_IMG_SIZE_LEN         4 /*32Bit, BigEndian */

#ifdef SPI_NAND_BOOT
  #define APP_IMG_START    	 0
#else
  #define APP_IMG_START         (1024+1024*4) /* U-Boot Start From */
#endif

#define	APP_IMG_COPY_SIZE       (1024*300)
#define	APP_IMG_COPY_DDR_ADDR    0x02000000

int SysImgCopyToDDR(void);

#endif
