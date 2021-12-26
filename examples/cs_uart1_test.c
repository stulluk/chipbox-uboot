#include <exports.h>

#if 0

int cs_uart1_test1(int argc, char *argv[])
{

    unsigned char readed_buffer[5];
    unsigned int readedlen=0,i;
    rx_arg readpara;
    unsigned char init_buffer[3];
    unsigned int readpointer;
    unsigned int writepointer;

    int time_out = 0;
    int ret = 0;
    readpara.rx_buffer = readed_buffer;
    readpara.bufferlen =  5;
    readpara.read_pointer= &readpointer;
    readpara.write_pointer = &writepointer;

    init_buffer[0]=0x02;
    init_buffer[1]=0x32;
    init_buffer[2]=0x30;

    app_startup(argv);

    cs_uart1_init(&readpara);
    //ret = cs_uart1_setattr(115200, 2);
    //if (ret == 1) return 1;
    //orion_irq_disable(13);

    cs_uart1_write(init_buffer, 3);
    //orion_irq_enable(13);
    orion_irq_enable(13);

    while(time_out < 70000000){
        if (readpointer != writepointer){
            
            printf("readed data[%d] = 0x%x\n",i,readed_buffer[readpointer]);
            readpointer ++;
            if (readpointer >= readpara.bufferlen)
                readpointer = 0;
        }
    
        time_out ++;
    // printf("timed out =%d\n",time_out);
    }

    return 0;
}

#endif 

int cs_uart1_test2(int argc, char *argv[])
{

    unsigned char readed_buffer[5];
    unsigned int readedlen=0,i;
    rx_arg readpara;
    unsigned char init_buffer[3];
    unsigned int readpointer =0;
  

    int time_out = 0;

    readpara.rx_buffer = NULL;
    readpara.bufferlen =  0;
    readpara.read_pointer= &readpointer;
    readpara.write_pointer = NULL;

    init_buffer[0]=0x02;
    init_buffer[1]=0x32;
    init_buffer[2]=0x30;

    app_startup(argv);
    printf("before init uart1\n");

    cs_uart1_init(&readpara);
    cs_uart1_write(init_buffer, 3);

    printf("after init uart1\n");

    while(time_out < 70000000){
        if (readpointer != 0){
            readedlen = cs_uart1_read(readed_buffer, 4);
            if (readedlen >4) 
                readedlen = 4;
            for (i=0; i< readedlen; i++)
                printf("readed data[%d] = 0x%x\n",i,readed_buffer[i]);
            readpointer =0;
         
        }
        
        time_out ++;

    }
    cs_uart1_disable();
    orion_irq_disable(UART1_IRQ);
    return 0;
}
