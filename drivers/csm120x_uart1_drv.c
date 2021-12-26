/*Copyright (C) Celestial Semiconductor
  Author: xiaodong.fan@celestialsemi.cn
*/

#include <exports.h>
#include <csm120x_uart1_drv.h>


static int uart1_irq(unsigned int irq, void * irq_args)
{
    
    unsigned long arg;
    unsigned int * writepointer;
    unsigned int * readpointer;
	unsigned char  tmpData;   // by kb :20100416
    printf("Ok, in serialport interrupt!");
    

    writepointer = ((rx_arg *)irq_args)->write_pointer;
    readpointer = ((rx_arg *)irq_args)->read_pointer;

	arg = serial_readb(UART1_IIR); /*read this register to identify the interrupt */

	if((arg & 0xC0) == 0xC0){
		// printf("cs-uart1:  FIFO's are enabled\n");
	}
	else{
		// printf("cs-uart1:  FIFO's are disabled\n");
	}

	if((arg & 0x0F) == 0x00){
		printf("cs-uart1:  CTS / DSR / RI / Carrier Detect : Reading Modem Status Register\n");
	} 
	else if ((arg & 0x0F) == 0x01){
		printf("cs-uart1:  No Pending Interrupts \n");
	} 
	else if ((arg & 0x0F) == 0x02){
		printf("cs-uart1:  THR Empty: Reading Interrupt Identity Register\n");
	} 
	else if ((arg & 0x0F) == 0x04){
		// printf("cs-uart1:  Receiver Data Availabler\n");
        if (irq_args != NULL){
            if (((rx_arg *)irq_args)->rx_buffer != NULL && ((rx_arg *)irq_args)->bufferlen >1 ){
               //  printf("Receive data: Saving data into usr-defined buffer !\n");
                while(serial_readb(UART1_RFL)){
                    (((rx_arg *)irq_args)->rx_buffer)[*writepointer] = (unsigned char) serial_readb(UART1_RBR);
                    (*writepointer) ++;
                    if (*writepointer >= ((rx_arg *)irq_args)->bufferlen) 
                        *writepointer = 0;
                    if ((*writepointer) == (*readpointer)){ 
                        // printf("cs-uart1: Read Buffer is full, following data will be lost!\n");
                        *readpointer ++;
                        if (*readpointer >= ((rx_arg *)irq_args)->bufferlen) 
                            *readpointer = 0;
                    }
                }
            }
            else 
            {
                printf("There is no usr-defined buffer, pls read data yourself!\n");
                *readpointer = 1;
				tmpData = (unsigned char) serial_readb(UART1_RBR); // by kb :20100416
				// printf("cs-uart1: Read Data [0x%02X]\n", tmpData);
                // serial_writeb(0x00, UART1_IER);
            }
        }
        else
        {
			// printf("cs-uart1: Error\n");
        }
	} 
	else if ((arg & 0x0F) == 0x06){
		// printf("cs-uart1:  Reading Receiver Line Status : (overrun / parity / framing / break interrupt) \n");
		//buffer = serial_readb(UART1_LSR);
    } 
	else if ((arg & 0x0F) == 0x07){
		printf("cs-uart1:  Busy Detected: USR[0] is set to 1\n");
        while (((serial_readb(UART1_USR)) & 0x01)  == 0x01); /* uart is busy */
        serial_writeb(LCRVAL, UART1_LCR);
    } 
	else if ((arg & 0x0F) == 0x000C){
		printf("cs-uart1:  Character Timeout: No Character in or out of Receiver FIFO\n");
        *readpointer = 1;
        serial_writeb(0x00, UART1_IER);;
        serial_writeb(FCRVAL, UART1_FCR);
	}
    
    return 0;

}

   // by kb :20100416
int cs_uart1_init(rx_arg *rxargs)
{
 

   if (rxargs == NULL || rxargs->read_pointer == NULL){
       printf("cs-uart1: RX paramaters is error!\n");
       return 1;
   }
   *(rxargs->read_pointer) = 0;
   if (rxargs->write_pointer != NULL) 
       *(rxargs->write_pointer) = 0;
   serial_writeb(0x00, UART1_IER);
   serial_writeb(0x00, UART1_FCR);
   serial_writeb(LCR_BKSE | LCR_8N1, UART1_LCR);
   serial_writeb(0x1d, UART1_DLL); // set to 115200 baudrate
   serial_writeb(0x00, UART1_DLH);
   serial_writeb(LCRVAL, UART1_LCR);
   serial_writeb(MCRVAL, UART1_MCR);
   serial_writeb(FCRVAL, UART1_FCR);
   
	// by kb :20100416
   // orion_irq_disable(UART1_IRQ);
   // orion_register_irq(UART1_IRQ, (irq_handle )uart1_irq, (void *)rxargs);
   // orion_irq_enable(UART1_IRQ);
   // serial_writeb(0x01, UART1_IER); /*interrupt is only enabled for "Enable Received Data Availableinterrupt" */
   
   return 0;
}

int cs_uart1_disable(void)
{
    serial_writeb(0x00, UART1_FCR); 
    serial_writeb(0x00, UART1_IER);
    return 0;
}

   // by kb :20100416
int cs_uart1_enable(void)
{
    serial_writeb(FCRVAL, UART1_FCR);
    serial_writeb(MCRVAL, UART1_MCR);
    // serial_writeb(0x00, UART1_MCR);
    // serial_writeb(0x01, UART1_IER);
    return 0;
}            
    
   // by kb :20100416
int cs_uart1_setattr(unsigned int baudrate, unsigned char parity)
{
    unsigned long arg;
   int baud_value;
   int parity_value;
   unsigned char data;


    if (baudrate != 4800 && baudrate != 9600 && baudrate != 19200 && baudrate != 38400 && baudrate != 57600 && baudrate != 115200 ){
        printf("Baudrate is not standard baudrate!\n");
        baudrate = 115200;
    }

    // printf("Now setting the baudrate is %d\n",baudrate);
    baud_value = SERIAL_CLK / (16 * baudrate );

    switch (parity){
    case 0:/* Even Parity */
        parity_value = 0x18;
        break;
    case 1:  /* Odd Parity */
        parity_value = 0x08;
        break;
    case 2:
    default: /* No parity */
        parity_value = 0x00;
        break;
    }
   
    cs_uart1_disable();

	// by kb :20100416
	data = 0;
	do
    {
        data = (unsigned char)serial_readb(UART1_USR);
        // printf("UART1_USR = 0x%x\n",temp);
    }while ((data & 0x01)  == 0x01);
    serial_writeb((0x83 | parity_value) & 0xFFFF, UART1_LCR); /* 8 data bits and 1 stop bit implemented */
    serial_writeb((baud_value & 0x00FF), UART1_DLL);
    serial_writeb(((baud_value & 0xFF00) >> 8), UART1_DLH);
    serial_writeb(0x03 | parity_value, UART1_LCR);

    cs_uart1_enable();
    // printf("DLL =0x%x\n",(baud_value & 0x00FF));
    // printf("DLH =0x%x\n", ((baud_value & 0xFF00) >> 8));
    return 0;
}

   // by kb :20100416
int cs_uart1_write(unsigned char *write_buffer, unsigned int len)
{
    int count =0;

    while (write_buffer != NULL && len != 0){
		// udelay (600);
		if (serial_readb(UART1_LSR) & LSR_THRE)
		{
			// printf("Send Data : 0x%02X\n", write_buffer[count]);
			serial_writeb(write_buffer[count]&0xff, UART1_THR);
			len --;
			count ++;
		}
    }
    
    return 0;
}

#if 0    // by kb :20100416
int cs_uart1_read(unsigned char *read_buffer, unsigned int len)
{
    int count = 0;
    if (read_buffer == NULL  || len == 0){
       printf("cs-uart1: read function paramaters is wrong!\n");
       return 0;
    }
    
    while(serial_readb(UART1_RFL) || len == 0){
        *(read_buffer + count) = (unsigned char) serial_readb(UART1_RBR);
        count ++;
        len --;
    }
    count --;
    serial_writeb(0x01, UART1_IER);
    return count;
}
#else
int cs_uart1_read(unsigned char *read_buffer, unsigned int len)
{
    unsigned int count;
	
    if (read_buffer == NULL  || len == 0){
       printf("cs-uart1: read function paramaters is wrong!\n");
       return 0;
    }

	count = 0;
	
	while (serial_readb(UART1_RFL) && (count < len))
	// while ((serial_readb(UART1_LSR) & LSR_DR) && (count < len))
	{
		*(read_buffer + count) = (unsigned char) serial_readb(UART1_RBR);
		count++;
	}

    // serial_writeb(0x01, UART1_IER);
	
    return (int)count;
}
#endif

