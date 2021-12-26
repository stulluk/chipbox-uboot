/*Copyright (C) Celestial Semiconductor
  Author: xiaodong.fan@celestialsemi.cn
*/


#ifndef __CSM120X_UART1_DRV_
#define __CSM120X_UART1_DRV_

#if defined(CONFIG_CELESTIAL) 

#define 	MASK		0x0F			/*only (LSB) 8 bits of 32 is used in the registers below */
#define 	MASK_MCR	0x1F			/*mask for Modem Control Register */

#define		UART1_BASE_ADDR 	0x101F2000		/*Orion Base for UART1 */
#define		UART1_BASE_SIZE 0x100		/*Total Size of Register starting from Base Required */
#define		UART1_RBR 	(0x00)		/*Receiver Buffer Register */
#define		UART1_THR 	(0x00)		/*Transmit Holding Register */
#define		UART1_DLL 	(0x00)		/*Divisor Latch Low */
#define		UART1_DLH 	(0x04)		/*Divisor Latch High */
#define		UART1_IER 	(0x04)		/*Interrupt Enable Register */
#define		UART1_IIR 	(0x08)		/*Interrup Identity Register */
#define		UART1_FCR 	(0x08)		/*FIFO Control Register */
#define		UART1_LCR 	(0x0C)		/*Line Control Register */
#define		UART1_MCR 	(0x10)		/*Modem Control Register */
#define		UART1_LSR 	(0x14)		/*Line Status Register */
#define		UART1_MSR 	(0x18)		/*Modem Status Register */

//#define		UART1_SCR 	(0x1C)
//#define		UART1_LPDLL 	(0x20)
//#define		UART1_LPDLH 	(0x24)
//#define		UART1_SRBR 	(0x30)
//#define		UART1_STHR 	(0x30)

#define		UART1_FAR 	(0x70)		/*FIFO Access Register */
#define		UART1_TFR 	(0x74)		/*Transmit FIFO Read */
#define		UART1_RFW 	(0x78)		/*Receive FIFO Write */
#define		UART1_USR 	(0x7C)		/*UART Status Register */

//#define		UART1_TFL 	(0x80)
#define		UART1_RFL 	(0x84)
//#define		UART1_SRR 	(0x88)
//#define		UART1_SRTS 	(0x8C)
//#define		UART1_SBCR 	(0x90)
//#define		UART1_SDMAM 	(0x94)
//#define		UART1_SFE 	(0x98)
//#define		UART1_SRT 	(0x9C)
//#define		UART1_STET 	(0xA0)

#define		UART1_HTX 	(0xA4)			/*Halt Transmission */

//#define		UART1_DMAS 	(0xA8)
//#define		UART1_CPR 	(0xF4)
//#define		UART1_UCV 	(0xF8)
//#define		UART1_CTR 	(0xFC)

#define LCR_8N1		0x03
#define FCR_FIFO_EN     0x01		/* Fifo enable */
#define FCR_RXSR        0x02		/* Receiver soft reset */
#define FCR_TXSR        0x04		/* Transmitter soft reset */

#define MCR_DTR         0x01
#define MCR_RTS         0x02
#define MCR_DMA_EN      0x04
#define MCR_TX_DFR      0x08

#define LCR_WLS_MSK	0x03		/* character length select mask */
#define LCR_WLS_5	0x00		/* 5 bit character length */
#define LCR_WLS_6	0x01		/* 6 bit character length */
#define LCR_WLS_7	0x02		/* 7 bit character length */
#define LCR_WLS_8	0x03		/* 8 bit character length */
#define LCR_STB		0x04		/* Number of stop Bits, off = 1, on = 1.5 or 2) */
#define LCR_PEN		0x08		/* Parity eneble */
#define LCR_EPS		0x10		/* Even Parity Select */
#define LCR_STKP	0x20		/* Stick Parity */
#define LCR_SBRK	0x40		/* Set Break */
#define LCR_BKSE	0x80		/* Bank select enable */

#define LSR_DR		0x01		/* Data ready */
#define LSR_OE		0x02		/* Overrun */
#define LSR_PE		0x04		/* Parity error */
#define LSR_FE		0x08		/* Framing error */
#define LSR_BI		0x10		/* Break */
#define LSR_THRE	0x20		/* Xmit holding register empty */
#define LSR_TEMT	0x40		/* Xmitter empty */
#define LSR_ERR		0x80		/* Error */


#define LCRVAL LCR_8N1					/* 8 data, 1 stop, no parity */
#define MCRVAL (MCR_DTR | MCR_RTS)			/* RTS/DTR */
#define FCRVAL (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR)	/* Clear & enable FIFOs */

//#define serial_writew(v,a)	do{*((unsigned int *)UART1_BASE_ADDR + (a)) = v; } while(0)
//#define serial_readw(a)		*((unsigned int* )UART1_BASE_ADDR + (a))

#define serial_writeb(v,a) (*((volatile unsigned char *)((UART1_BASE_ADDR)+(a))) = (v))
#define serial_readb(a)  (*(((volatile unsigned char *)(UART1_BASE_ADDR + (a)))))


//#define serial_writeb(v,a) do{ printf("write[0x%x]=0x%x\n",a,(v)); writeb(v,a);} while(0)
//#define serial_readb(a)  readb(a)

#define UART1_IRQ  13					/* IRQ of Serial Port */
#define SERIAL_CLK 55000000             /* 55 Mhz Clock */


typedef struct _rx_struct {    
    unsigned char * rx_buffer;
    int bufferlen;
    unsigned int* read_pointer;
    unsigned int* write_pointer;
}rx_arg;

int cs_uart1_init(rx_arg *rxargs);
int cs_uart1_setattr(unsigned int baudrate, unsigned char parity);
int cs_uart1_write(unsigned char *write_buffer, unsigned int len);
int cs_uart1_read(unsigned char *read_buffer, unsigned int len);
int cs_uart1_disable(void);
int cs_uart1_enable(void);

#endif /* CELESTIAL */
#endif 
