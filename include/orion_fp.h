#ifndef __ORION_FP_H__
#define __ORION_FP_H__

#if defined(CONFIG_CELESTIAL)
/* Register Map */

#if defined(MERIH_GOLD_BOARD)   // by kb :20100416
#include <csm120x_uart1_drv.h>
#include <type_def.h>

#define MAX_FB_COMM_LENGTH    30
#define MIN_FB_COMMAND_LENGT  2    /* Command + Length*/
#define MAX_FB_RECV_LENGTH    10


/* Define Front COMMAND */
#define FB_START_CODE                 0xF0

#define FB_BOOT_MODE                  0x00
#define FB_REQUEST_BOOT_MODE          0x01
#define FB_CLEAR_BOOT_END             0x02

#define FB_RCU_KEY                    0x10
#define FB_RCU_REPEAT                 0x11
#define FB_FRONT_KEY                  0x12
#define FB_FRONT_REPEAT               0x13
#define FB_SET_MID                    0x1F   /* Set RCU Manutacture ID */

#define FB_DISPLAY_FND_SINGLE         0x20
#define FB_DISPLAY_FND_ALL            0x21
#define FB_DISPLAY_VFD_SINGLE         0x22
#define FB_DISPLAY_VFD_ALL            0x23
#define FB_DISPLAY_VFD_TEST           0x24

#define FB_SET_CURRENT_TIME           0x40
#define FB_SET_TIMER                  0x41
#define FB_REQUEST_TIME               0x42
#define FB_RETURN_TIME                0x43

#define FB_SET_AUDIO_MUTE_ON          0x54
#define FB_SET_AUDIO_MUTE_OFF         0x55

#define FB_WATCHDOG_START             0x60
#define FB_WATCHDOG_STOP              0x61
#define FB_WATCHDOG_KICK              0x62

#define FB_SET_RCU_MODE               0xA0
#define FB_SET_FRONT_LEVEL            0xA1
#define FB_SET_FRONT_CLOCK            0xA2

#define FB_REQUEST_MCU_VERSION        0xE0
#define FB_RETURN_MCU_VERSION         0xE1

#define FB_MCU_ACK                    0xFF

#define FB_SET_STATUS                 0x70

#define FB_STAND_BY                   0x00
#define FB_SHUT_DOWN                  0x01
#define FB_STAND_BY_SIGNAL            0x02

#define FB_BOOT_MODE_NORMAL           0x10
#define FB_BOOT_MODE_TIMER            0x11
#define FB_BOOT_MODE_AUX              0x12
#define FB_BOOT_MODE_DIAG             0x13
#define FB_BOOT_MODE_WATCH            0x14
#define FB_BOOT_MODE_AC_ON            0x15

#define FB_KEY_POWER                  0x10
#define FB_KEY_HOLD                   0x0F /* UP + DOWN */
#define FB_KEY_AUTO_UPDATE_ALL        0x3C /* MENU + OK */
#define FB_KEY_AUTO_UPDATE_WO_BOOT    0x7E /* MENU + EXIT */
#define FB_KEY_AUTO_UPDATE_UBOOT      0x36 /* MENU + RIGHT */
#define FB_KEY_AUTO_UPDATE_CHDB       0x3A /* MENU + LEFT */
#define FB_KEY_AUTO_UPDATE_ROOTFS     0x2E /* MENU + UP*/
#define FB_KEY_AUTO_UPDATE_WORK0      0x1E /* MENU + DOWN*/

#define FB_END_CODE                   0x0F

int FbInt(U32 baudrate, U8 parity);
int FbSendCommand(U8 command, U8 *data, U8 len);
int FbSendFndDisplay(char *data);
int FbSendFndDisplayNum(U32 num);
int FbRquestBootMode(void);
int FbSendBootEnd(void);
int FbStartWatchdog(U8 time);
int FbStopWatchdog(void);
int FbSendKick(void);
int FbFlushFrontData(void);
int FbReceiveFrontData(U8 *buffer, U32 timeout);
int FbGetAck(void);

#else  /* MERIH_GOLD_BOARD */
/* Register Map */
#define FP_INTS						(0x00  >> 1) /* Interrupt Status Register */
#define FP_INTM						(0x04  >> 1) /* Interrupt Mask Register */
#define FP_RC_CTL_L				(0x08  >> 1) /* Remote Control Register */
#define FP_RC_CMD_L				(0x100 >> 1) /* RC5 Command Register */
#define FP_NEC_CMD_L			(0x104 >> 1) /* NEC Command Register */
#define FP_LED_DATA_L		(0x200 >> 1) /* Led Display Data Register */
#define FP_LED_CNTL_L			(0x204 >> 1) /* Led Display Control Register */
#define FP_KSCAN_DATA_L	(0x300 >> 1) /* Key Scan Data Register */
#define FP_KSCAN_CNTL_L	(0x304 >> 1) /* Key Scan Control Register */
#define FP_ARBCNTL_L			(0x308 >> 1) /* Led & Key Scan Arbiter Control Register */
#define FP_RC_CTL_H				(0x0a  >> 1) /* Remote Control Register */
#define FP_RC_CMD_H			(0x102 >> 1) /* RC5 Command Register */
#define FP_NEC_CMD_H			(0x106 >> 1) /* NEC Command Register */
#define FP_LED_DATA_H		(0x202 >> 1) /* Led Display Data Register */
#define FP_LED_CNTL_H			(0x206 >> 1) /* Led Display Control Register */
#define FP_KSCAN_DATA_H	(0x302 >> 1) /* Key Scan Data Register */
#define FP_KSCAN_CNTL_H	(0x306 >> 1) /* Key Scan Control Register */
#define FP_ARBCNTL_H			(0x30a >> 1) /* Led & Key Scan Arbiter Control Register */

// #define		UART1_RBR 	(0x00 >> 1)		/*Receiver Buffer Register */
// #define		UART1_THR 	(0x00 >> 1)		/*Transmit Holding Register */
// #define		UART1_DLL 	(0x00 >> 1)		/*Divisor Latch Low */
// #define		UART1_DLH 	(0x04 >> 1)		/*Divisor Latch High */
// #define		UART1_IER 	(0x04 >> 1)		/*Interrupt Enable Register */
// #define		UART1_IIR 	(0x08 >> 1)		/*Interrup Identity Register */
// #define		UART1_FCR 	(0x08 >> 1)		/*FIFO Control Register */
// #define		UART1_LCR 	(0x0C >> 1)		/*Line Control Register */
// #define		UART1_MCR 	(0x10 >> 1)		/*Modem Control Register */
// #define		UART1_LSR 	(0x14 >> 1)		/*Line Status Register */
// #define		UART1_MSR 	(0x18 >> 1)		/*Modem Status Register */
// #define		UART1_FAR 	(0x70 >> 1)		/*FIFO Access Register */
// #define		UART1_TFR 	(0x74 >> 1)		/*Transmit FIFO Read */
// #define		UART1_RFW 	(0x78 >> 1)		/*Receive FIFO Write */
// #define		UART1_USR 	(0x7C >> 1)		/*UART Status Register */
// #define		UART1_HTX 	(0xA4)					/*Halt Transmission */

/******************************************************************************************************/

typedef enum 
{
	FP_LEDDISP_OFF = 0,
	FP_LEDDISP_BLINK = 1,
	FP_LEDDISP_ON = 2 
}CSFP_LEDDISPMODE;

/******************************************************************************************************/

int fp_set_led_display_pos(unsigned char column);
int fp_set_led_dispaly_mode(CSFP_LEDDISPMODE mode);
int fp_set_led_dispaly_char(unsigned char *char_value);
int fp_set_led_display_raw(unsigned char *raw_value, int size);
int fp_enable_keyscan (void);
int fp_disable_keyscan (void);
int fp_set_keyscan_map(unsigned char raw_input, unsigned char sys_code);
int fp_get_keyscan_input (unsigned char *key, unsigned char *valid);
// int fp_enable_serial_in (void);
// int fp_disable_serial_in (void);
// int fp_reset_serial_in(void);
// int fp_set_serial_in_attr(unsigned int baudrate, unsigned char parity);
// int fp_write_serial_in(unsigned char *write_char);

#endif /* !MERIH_GOLD_BOARD */

#endif /* CONFIG_CELESTIAL */
#endif /*  __ORION_FP_H__ */
