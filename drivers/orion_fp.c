/*
 * Basic FRONTPANEL functions
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
 * Author: SunBin bin.sun@celestialsemi.com, Celestial Semiconductor
 *
 * Copyright (c) 2008 Celestial Semiconductor
 *
 */

#include <common.h>
#include "orion_fp.h"

// #define ORION_FP_DEBUG_ON   // by kb :20100416

#if defined(CONFIG_CELESTIAL) && defined(CFG_FP)

#if defined(MERIH_GOLD_BOARD)   // by kb :20100416
int FbInt(U32 baudrate, U8 parity)
{
	rx_arg  readpara;
	int     ret;
	U32     readpointer =0;
	
	readpointer = 0;

	 /* Init Parameter */
    readpara.rx_buffer = NULL; /* Not use IRQ Receive */
	readpara.bufferlen =  0;
	readpara.read_pointer= &readpointer;
	readpara.write_pointer = NULL;

	/* Init UART1 */
	ret = cs_uart1_init(&readpara);
	if (ret != 0)
	{
#ifdef ORION_FP_DEBUG_ON
		printf("FbInt Error : cs_uart1_init Error [%d]\n", ret);
#endif
		return ret;
	}

	/* Before set Baudrate and Parity, Disable UART1 */
	cs_uart1_disable();
	/* Set UART1 Baudrate and Parity */
	ret = cs_uart1_setattr(baudrate, parity);
#ifdef ORION_FP_DEBUG_ON
	printf("FbInt Info : cs_uart1_setattr Return [%d]\n", ret);
#endif
	/* Enable UART1 */
	cs_uart1_enable();

	return ret;
}

int FbSendCommand(U8 command, U8 *data, U8 len)
{
	U8  sendData[MAX_FB_COMM_LENGTH];
	U32 totalLength;

	if ((len + 4) > MAX_FB_COMM_LENGTH)
	{
#ifdef ORION_FP_DEBUG_ON
		printf("FbSendCommand Error : Data length is too long [%d]\n", len);
#endif
		return -1;
	}
	
	sendData[0] = FB_START_CODE;
	sendData[1] = command;
	sendData[2] = len;
	totalLength = 3;
	if (len > 0)
	{
		memcpy(sendData + totalLength, data, len);
		totalLength += (U32)len;
	}
	sendData[totalLength] = FB_END_CODE;
	totalLength++;

#ifdef ORION_FP_DEBUG_ON
	// printf("FbSendCommand Info : Total Length [%d]\n", totalLength);
#endif
	cs_uart1_write(sendData, totalLength);

	return 0;
}

int FbSendFndDisplay(char *data)
{
	return FbSendCommand(FB_DISPLAY_FND_ALL, data, 4);
}

int FbSendFndDisplayNum(U32 num)
{
	U32 value;
	U8  data[5];

	if (num > 9999)
	{
		value = num % 10000;
	}
	else
	{
		value = num;
	}

#if 0
	data[0] = (U8)((value /1000) + 0x30);  /* Convert First digit to Ascii Code */
	value = value % 1000;
	data[1] = (U8)((value /100) + 0x30);  /* Convert Second digit to Ascii Code */
	value = value % 100;
	data[2] = (U8)((value /10) + 0x30);  /* Convert Third digit to Ascii Code */
	value = value % 10;
	data[3] = (U8)(value  + 0x30);  /* Convert Fourth digit to Ascii Code */
#else
	sprintf(data, "%04d", value);
#endif
	
	return FbSendCommand(FB_DISPLAY_FND_ALL, data, 4);
}

int FbRquestBootMode(void)
{
	return FbSendCommand(FB_REQUEST_BOOT_MODE, NULL, 0);
}

int FbRequestStandBy(void)
{
	U8 data;
	
	data = FB_STAND_BY;
	return FbSendCommand(FB_REQUEST_BOOT_MODE, &data, 1);
}

/* Real Power Off */
int FbRequestShoutDowm(void)
{
	U8 data;
	
	data = FB_SHUT_DOWN;
	return FbSendCommand(FB_REQUEST_BOOT_MODE, &data, 1);
}

int FbSetRcuPowerKey(U8 num, U8 Mid[2], U8 powerKey)
{
	U8 data[4];
	
	data[0] = num;
	data[1] = Mid[0];
	data[2] = Mid[1];
	data[3] = powerKey;
	return FbSendCommand(FB_SET_RCU_MODE, (char *)data, 4);
}

int FbSetFrontDisplay(U8 level)
{
	return FbSendCommand(FB_SET_FRONT_LEVEL, &level, 1);
}

int FbSetFrontClock(U8 on)
{
	return FbSendCommand(FB_SET_FRONT_CLOCK, &on, 1);
}

int FbSendBootEnd(void)
{
	return FbSendCommand(FB_CLEAR_BOOT_END, NULL, 0);
}

int FbStartWatchdog(U8 time)
{
	U8 data;

	data = time;

	return FbSendCommand(FB_WATCHDOG_START, &data, 1);
}

int FbStopWatchdog(void)
{
	return FbSendCommand(FB_WATCHDOG_STOP, NULL, 0);
}

int FbSendKick(void)
{
	return FbSendCommand(FB_WATCHDOG_KICK, NULL, 0);
}

int FbFlushFrontData(void)
{
	U8 data;
	U8 count = 0;

	while (cs_uart1_read(&data, 1) == 1)
	{
		printf("FbFlushFrontData : Data[0x%x], count[%d]\n", data, count++);
	}
	return;
}

int FbReceiveFrontData(U8 *buffer, U32 timeout)
{
	U32 timeCount;
	int count;
	U8  data;
	U8  waitHeader;
	U8  dataLen;

	if (buffer == NULL)
	{
#ifdef ORION_FP_DEBUG_ON
		printf("FbReceiveFrontData Error : Data Wrong !\n");
#endif
		return 0;
	}
	
	timeCount  = 0;
	waitHeader = 1;
	while (waitHeader && (timeCount < timeout))
	{
		if (cs_uart1_read(&data, 1) == 1)
		{
#ifdef ORION_FP_DEBUG_ON
			printf("FbReceiveFrontData Info : Read Data [0x%02X]\n", data);
#endif
			if (data == FB_START_CODE)
			{
				waitHeader = 0;
			}
		}
		else
		{
			udelay (50);
			timeCount++;
		}
	}
#ifdef ORION_FP_DEBUG_ON
	printf("FbReceiveFrontData Info : Count [%d] [%d]\n", timeCount, waitHeader);
#endif

	if ((timeCount >= timeout) && waitHeader)
	{
		/* Receive Timeout */
#ifdef ORION_FP_DEBUG_ON
		printf("FbReceiveFrontData Error : FB_START_CODE Timeout\n");
#endif
		return 0;
	}
	
	count = 0;
	while((count < MIN_FB_COMMAND_LENGT) && (timeCount < timeout))
	{
		if (cs_uart1_read(&data, 1) == 1)
		{
			*(buffer + count) = data;
			count++;
		}
		else
		{
			udelay (50);
			timeCount++;
		}
	}

	if ((count < MIN_FB_COMMAND_LENGT) && (timeCount >= timeout))
	{
		/* Receive Timeout */
#ifdef ORION_FP_DEBUG_ON
		printf("FbReceiveFrontData Error : Data Header Timeout\n");
#endif
		return 0;
	}

	dataLen = *(buffer + 1) + 2;

	while((count < dataLen) && (timeCount < timeout))
	{
		if (cs_uart1_read(&data, 1) == 1)
		{
			*(buffer + count) = data;
			count++;
		}
		else
		{
			udelay (50);
			timeCount++;
		}
	}
	if ((count < dataLen) && (timeCount >= timeout))
	{
		/* Receive Timeout */
#ifdef ORION_FP_DEBUG_ON
		printf("FbReceiveFrontData Error : Data Timeout\n");
#endif
		return 0;
	}

	waitHeader = 1;
	while (waitHeader && (timeCount < timeout))
	{
		if (cs_uart1_read(&data, 1) == 1)
		{
			if (data == FB_END_CODE)
			{
				waitHeader = 0;
			}
		}
		else
		{
			udelay (50);
			timeCount++;
		}
	}
	
	if ((timeCount >= timeout) && waitHeader)
	{
		/* Receive Timeout */
#ifdef ORION_FP_DEBUG_ON
		printf("FbReceiveFrontData Error : FB_END_CODE Timeout\n");
#endif
		return 0;
	}
	

#ifdef ORION_FP_DEBUG_ON
	printf("FbReceiveFrontData Info : Receive %d datas in %d mSec\n", count, timeCount);
#endif

	return count;
}

int FbGetAck(void)
{
	U8  data[MAX_FB_RECV_LENGTH];
	int len;

	len = FbReceiveFrontData(data, 10000);
	if (len < 0)
	{
#ifdef ORION_FP_DEBUG_ON
		printf("FbGetAck Error : Can not get ACK : %d \n", len);
#endif
		return 1;
	}
	
	if (data[0] == FB_MCU_ACK)
	{
#ifdef ORION_FP_DEBUG_ON
		printf("FbGetAck Info : ACK!! %d \n", len);
#endif
		return 0;
	}

#ifdef ORION_FP_DEBUG_ON
	printf("FbGetAck Error : Can not Get ACK\n");
#endif
	return 1;
}

#else  /* MERIH_GOLD_BOARD */

#define fp_writew(v,a)         do { *((volatile unsigned short *)CFG_FP_BASE + (a)) = v; }while(0)
#define fp_readw(a)            *((volatile unsigned short  *)CFG_FP_BASE + (a))

#define serial_writew(v,a)		do{*((volatile unsigned short *)UART1_BASE + (a)) = v; } while(0)
#define serial_readw(a)			*((volatile unsigned short *)UART1_BASE + (a))

static unsigned int led_val = 0xffffffff;

static unsigned char num_tab[] = { 
	0x98, 0xf7, 0xfb, 0x7f, 0xbf, 0xdf, 0xef, 0xff, /* 0xff */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x10 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x20 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 
	0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, /* 0x30 */
	0x01, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0x11, 0xc1, 0x63, 0x85, 0x61, 0x71, 0xff, /* 0x40 */
	0x91, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 
	0x31, 0xff, 0xf5, 0xff, 0xe1, 0x83, 0xff, 0xff, /* 0x50 */
	0xff, 0x89, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x60 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x70 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x80 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x90 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xa0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xb0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xc0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xd0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xe0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xf0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff 
};

static unsigned char Kscan_Map[16] = {
				0x00,0x01,0x02,0x03,
				0x04,0x05,0x06,0x07,
				0x08,0x09,0x0a,0x0b,
				0x0c,0x0d,0x0e,0x0f
				     };

static unsigned disp_column = 0;
static unsigned disp_mode = 2;
	

int fp_set_led_display_pos(unsigned char column)
{
	if (column <= 3 ){
		disp_column = column;
	}

	return 0;	
}

int fp_set_led_dispaly_mode(CSFP_LEDDISPMODE mode)
{
	disp_mode = mode;

	return 0;	
}

int fp_set_led_dispaly_char(unsigned char *char_value)
{
	unsigned char disp_char[2];
	int val=0,count = 60,enb,temp;

	if (char_value == NULL){
		return -1;
	}
	
	disp_char[0] = *char_value;
	disp_char[1] = '\0';

	val = num_tab[(int)disp_char[0]]<<(disp_column * 8);

	if (disp_column == 0) {
		led_val &= 0xffffff00;
		led_val |= val;
	}
	if (disp_column == 1) {
		led_val &= 0xffff00ff;
		led_val |= val;
	}
	if (disp_column == 2) {
		led_val &= 0xff00ffff;
		led_val |= val;
	}
	if (disp_column == 3) {
		led_val &= 0x00ffffff;
		led_val |= val;
	}

	enb =1;
	temp = 0xffffffff;
	
	switch(disp_mode)
	{
			case FP_LEDDISP_ON :

				fp_writew((led_val >> 16) & 0xffff, FP_LED_DATA_H);
				fp_writew(led_val & 0xffff, FP_LED_DATA_L);

				fp_writew(0x8000, FP_LED_CNTL_H);
				fp_writew(0 != enb ? 0x1000 : 0x0000, FP_LED_CNTL_L);
				
				break;
				
			case FP_LEDDISP_BLINK:
				
				while(count--)
					{
						fp_writew((led_val >> 16) & 0xffff, FP_LED_DATA_H);
						fp_writew(led_val & 0xffff, FP_LED_DATA_L);
						
						fp_writew(0x8000, FP_LED_CNTL_H);
						fp_writew(0 != enb ? 0x1000 : 0x0000, FP_LED_CNTL_L);
				
						udelay(200000);
						
						fp_writew((temp >> 16) & 0xffff, FP_LED_DATA_H);
						fp_writew(temp & 0xffff, FP_LED_DATA_L);
						
						fp_writew(0x8000, FP_LED_CNTL_H);
						fp_writew(0 != enb ? 0x1000 : 0x0000, FP_LED_CNTL_L);
				
						udelay(200000);;
						
						if(FP_LEDDISP_BLINK != disp_mode)
							break;
						
					}
				break;
				
			case FP_LEDDISP_OFF :
				fp_writew((temp >> 16) & 0xffff, FP_LED_DATA_H);
				fp_writew(temp & 0xffff, FP_LED_DATA_L);
						
				fp_writew(0x8000, FP_LED_CNTL_H);
				fp_writew(0 != enb ? 0x1000 : 0x0000, FP_LED_CNTL_L);
				
				break;
				
			default :
				break;		
	}
	
	return 0;
}

void fp_enable_led(void)
{
	fp_writew(0x8000, FP_LED_CNTL_H);
	fp_writew(1 ? 0x1000 : 0x0000, FP_LED_CNTL_L);
}

void fp_disable_led(void)
{
	fp_writew(0x8000, FP_LED_CNTL_H);
	fp_writew(0 ? 0x1000 : 0x0000, FP_LED_CNTL_L);
}

int fp_set_led_display_raw(unsigned char *raw_value, int size)
{
	unsigned char disp_char[5];
	int i=0,val=0,count = 60,enb,temp, column, delay, disnum = 0;

	if (raw_value == NULL){
		return -1;
	}
		
	disp_char[0] = '\0';
	disp_char[1] = '\0';
	disp_char[2] = '\0';
	disp_char[3] = '\0';
	disp_char[4] = '\0';
	memcpy(disp_char,raw_value,size);
	disp_char[4] = '\0';
	
	disnum = size;

	column = disp_column;
	
	if( (disnum == 0) || ((column + disnum) > 4) ){
		return -1;
	}
		
	if (column == 1)
		val |= 0x000000ff;
	if (column == 2)
		val |= 0x0000ffff;
	if (column == 3)
		val |=0x00ffffff;
	
	for( i = 0; i < disnum; i++,column++ )
	{
		temp = disp_char[i] << (column * 8);
		val |= temp;	
	}

	if (column == 1)
		val |= 0xffffff00;
	if (column == 2)
		val |= 0xffff0000;
	if (column == 3)
		val |=0xff000000;
	
	enb =1;
	temp = 0xffffffff;

	switch(disp_mode)
	{
			case FP_LEDDISP_ON :

				fp_writew((val >> 16) & 0xffff, FP_LED_DATA_H);
				fp_writew(val & 0xffff, FP_LED_DATA_L);

				fp_writew(0x8000, FP_LED_CNTL_H);
				fp_writew(0 != enb ? 0x1000 : 0x0000, FP_LED_CNTL_L);
				
				break;
				
			case FP_LEDDISP_BLINK:
				
				while(count--)
					{
						fp_writew((val >> 16) & 0xffff, FP_LED_DATA_H);
						fp_writew(val & 0xffff, FP_LED_DATA_L);
						
						fp_writew(0x8000, FP_LED_CNTL_H);
						fp_writew(0 != enb ? 0x1000 : 0x0000, FP_LED_CNTL_L);
				
						for( delay=0xffffff; delay>0; delay--);
						
						fp_writew((temp >> 16) & 0xffff, FP_LED_DATA_H);
						fp_writew(temp & 0xffff, FP_LED_DATA_L);
						
						fp_writew(0x8000, FP_LED_CNTL_H);
						fp_writew(0 != enb ? 0x1000 : 0x0000, FP_LED_CNTL_L);
				
						for( delay=0xffffff; delay>0; delay--);
						
						if(FP_LEDDISP_BLINK != disp_mode)
							break;
						
					}
				break;
				
			case FP_LEDDISP_OFF :
				fp_writew((temp >> 16) & 0xffff, FP_LED_DATA_H);
				fp_writew(temp & 0xffff, FP_LED_DATA_L);
						
				fp_writew(0x8000, FP_LED_CNTL_H);
				fp_writew(0 != enb ? 0x1000 : 0x0000, FP_LED_CNTL_L);
				
				break;
				
		default :
			break;			
	}
	return 0;
}

int fp_enable_keyscan (void)
{	
	int enable;

	enable = 1;
	fp_writew((enable >> 16)& 0xffff,FP_KSCAN_CNTL_H);
	fp_writew(enable & 0xffff,FP_KSCAN_CNTL_L);
	
	return 0;	
}

int fp_disable_keyscan (void)
{	
	int enable;

	enable = 0;
	fp_writew((enable >> 16)& 0xffff,FP_KSCAN_CNTL_H);
	fp_writew(enable & 0xffff,FP_KSCAN_CNTL_L);
	
	return 0;	
}

int fp_set_keyscan_map(unsigned char raw_input, unsigned char sys_code)
{
	if(raw_input > 15)
		return		-1;

	Kscan_Map[raw_input] = sys_code;	
	return 0;
}

int fp_get_keyscan_input (unsigned char *key, unsigned char *valid)
{
	unsigned	char		keynum;
	unsigned	short	data;

	if( (key==NULL) || (valid==NULL) )
		return		-1;

	data = fp_readw(FP_KSCAN_DATA_L);
	
	if( data & 0x0080 )
		*valid = 1;
	else
		*valid = 0;

	keynum = data & 0xf;
	*key = Kscan_Map[keynum];

	return		0;
}


// int fp_enable_serial_in(void)
// {
// 	/* Enable FIFO's and Interrupts */
// 	serial_writew(0x0000, UART1_FCR);
// 	serial_writew(0x0000, UART1_MCR);
// 	serial_writew(0x0001, UART1_IER);

// 	return 0;

// }

// int fp_disable_serial_in(void)
// {
// 	/* Disable FIFO's & Interrupts */
// 	serial_writew(0x0000, UART1_FCR); 
// 	serial_writew(0x0000, UART1_IER);

// 	return 0;
// }

// int fp_reset_serial_in(void)
// {
// 	/* Flush FIFO's */
// 	serial_writew(0x0000, UART1_FCR); 
// 	serial_writew(0x0001, UART1_FCR);

// 	return 0;
// }

// int fp_set_serial_in_attr(unsigned int baudrate, unsigned char parity)
// {
	
// 	unsigned long SERIAL_CLK = 55000000; /* 55 Mhz Clock */
// 	unsigned long arg,temp;
// 	int baud_value;
// 	int parity_value;

// 	switch(baudrate){
		
// 		case 4800:
//  		case 9600:
//  		case 19200:
//  		case 38400:
//  		case 57600:			
//  		case 115200:
			
// 			baud_value = SERIAL_CLK / (16 * baudrate );
//  			break;
			
// 		default:											/* default set to 9600 */
// 			baud_value = SERIAL_CLK / (16 * 9600 );
//  			break;
// 	}

// 	switch (parity){
		
// 		case 0:	/* Even Parity */
// 			parity_value = 0x18;
// 			break;
			
// 		case 1:  /* Odd Parity */
// 			parity_value = 0x08;
// 			break;
			
// 		case 2:
// 		default: /* No parity */
// 			parity_value = 0x00;
// 			break;
// 	}
// 	arg = (0x83 | parity_value) & 0xFFFF; /* 8 data bits and 1 stop bit implemented */
// 	temp = serial_readw(UART1_USR);	
// 	if (( temp & 0x0001 ) == 0x0000 ){	
// 		serial_writew(arg, UART1_LCR);
// 	}
// 	else	{
// 		printf("<CS-RC> UART Modem Busy, USR arg = 0x%lx\n", temp);
// 	}

// 	arg = baud_value;
// 	temp = serial_readw(UART1_USR);
// 	if (( temp & 0x0001 ) == 0x0000 ){
// 		serial_writew((arg & 0x00FF), UART1_DLL);
// 		serial_writew(((arg & 0xFF00) >> 8), UART1_DLH);
// 	}
// 	else	{
// 		printf("<CS-RC> UART Modem Busy, USR arg = 0x%lx\n", temp);
// 	}
	
// 	arg = 0x03 | parity_value; /* 8 data bits and 1 stop bit implemented */
// 	temp = serial_readw(UART1_USR);	
// 	if (( temp & 0x0001 ) == 0x0000 ){	
// 		serial_writew(arg, UART1_LCR);
// 	}
// 	else	{
// 		printf("<CS-RC> UART Modem Busy, USR arg = 0x%lx\n", temp);
// 	}

// 	return 0;
// }

// int fp_write_serial_in(unsigned char *write_char)
// {
// 	unsigned long arg;
// 	arg = (unsigned long)*write_char;
	
// 	printf("<CS-RC> writing 0x%lx to TX register \n", arg);
// 	serial_writew(arg, UART1_THR);
			
// 	return 0;
// }

#endif /* !MERIH_GOLD_BOARD */

#endif /* CONFIG_ORION && CFG_FP */

