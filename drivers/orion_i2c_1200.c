/*
 * Basic I2C functions
 *
 * Copyright (c) 2007 Celestial Semiconductor
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author: 
 *     Tu BaoZhao bz.tu@celestialsemi.com, Celestial Semiconductor
 *     Fan Xiaodong xiaodong@celestialsemi.com Celestial Semiconductor
 * Copyright (c) 2007 Celestial Semiconductor
 *
 */

#include <common.h>
#include <asm/io.h>
#include "orion_i2c_1200.h"

#if defined(CONFIG_ARCH_CSM1200) && defined(CONFIG_HARD_I2C)

#define i2c_writeb(v,a)    writeb(v, (CFG_I2C_BASE + (a)))
#define i2c_writew(v,a)    writew(v, (CFG_I2C_BASE + (a)))
#define i2c_writel(v,a)    writel(v, (CFG_I2C_BASE + (a)))

#define i2c_readb(a)       readb((CFG_I2C_BASE + (a)))
#define i2c_readw(a)       readw((CFG_I2C_BASE + (a)))
#define i2c_readl(a)       readl((CFG_I2C_BASE + (a)))

static uchar previous_chip = 0x7f;

void i2c_init (int speed, int slaveadd)
{
	unsigned int ss_scl_cnt;

	if(i2c_readw(IC_COMP_TYPE) != 0x0140 && i2c_readw(IC_COMP_TYPE+2) != 0x4457) {
		printf("Warning: I2C controller not found.\n");
	}

	ss_scl_cnt = (CFG_I2C_CLK / 2) / speed;

	i2c_writew(0x0, IC_ENABLE);
	i2c_writew(slaveadd, IC_SAR);
	i2c_writew(0x63, IC_CON);  		/* master only, support re-start, standard speed */
	i2c_writew(ss_scl_cnt, IC_SS_SCL_HCNT);	
	i2c_writew(ss_scl_cnt, IC_SS_SCL_LCNT);
	i2c_writew(0x1, IC_ENABLE);
}

int i2c_probe (uchar chip)
{
	int i;

	if(chip != previous_chip) {
		i2c_writew(0x0, IC_ENABLE);
		i2c_writew(chip, IC_TAR);
		i2c_writew(0x1, IC_ENABLE);
		previous_chip = chip;
	}

	i2c_writew(0x100, IC_DATA_CMD);

	while(!(i2c_readw(IC_STATUS) & 0x8)) {
		if(i2c_readw(IC_RAW_INTR_STAT) & 0x40) {
			i = i2c_readw(IC_CLR_TX_ABRT);
			return 1;
		}
	}

	i = i2c_readw(IC_DATA_CMD);
	/* Wait STOP condition */
	while(i2c_readw(IC_STATUS) & 0x1);

	return 0;			
}

int i2c_read (uchar chip, uint addr, int alen, uchar * buffer, int len)
{
	int i;
	int cmd_cnt;
	int data_cnt;
    unsigned int time_cnt;
	unsigned char *byte_addr = (unsigned char*)&addr;
	unsigned char *p = buffer;

	if(chip != previous_chip) {
		i2c_writew(0x0, IC_ENABLE);
		i2c_writew(chip, IC_TAR);
		i2c_writew(0x1, IC_ENABLE);
		previous_chip = chip;
	}

	for(i = alen-1; i >= 0; i--)
		i2c_writew(byte_addr[i], IC_DATA_CMD);

	data_cnt = 0;
	cmd_cnt = 0;
    time_cnt =0;
	while(data_cnt < len) {
		while((cmd_cnt < len) && (i2c_readw(IC_STATUS) & 0x2)) {
			i2c_writew(0x100, IC_DATA_CMD);
			cmd_cnt++;
		}

		if(i2c_readw(IC_STATUS) & 0x8) {
			*p = i2c_readw(IC_DATA_CMD);
			p++;
			data_cnt++;
		}       
        else{
            time_cnt ++;
            if (time_cnt >=100000)
                return 1;
        }
        

		if (i2c_readw(IC_RAW_INTR_STAT) & 0x40) {
			i = i2c_readw(IC_TX_ABRT_SOURCE);
			printf("Chipaddress=0x%x, subaddr=0x%x, IC_TX_ABRT_SOURCE=0x%x\n",
                   chip, addr, i);
			i = i2c_readw(IC_CLR_TX_ABRT);
			previous_chip = 0;
			return 1;
        }
	}		

	/* Wait STOP condition */
    time_cnt = 0;
	while (i2c_readw(IC_STATUS) & 0x1){
		if ((i2c_readw(IC_RAW_INTR_STAT) & 0x40) || time_cnt >=100000) {
			i = i2c_readw(IC_CLR_TX_ABRT);
			previous_chip = 0;
			return 1;
		}
        time_cnt++;
    }


    return 0;
}

int i2c_write (uchar chip, uint addr, int alen, uchar * buffer, int len)
{
	int i;
	int data_cnt;
    unsigned int time_cnt;

	unsigned char *byte_addr = (unsigned char*)&addr;
	unsigned char *p = buffer;

	if(chip != previous_chip) {
		i2c_writew(0x0, IC_ENABLE);
		i2c_writew(chip, IC_TAR);
		i2c_writew(0x1, IC_ENABLE);
		previous_chip = chip;
	}

	for(i = alen-1; i >= 0; i--)
		i2c_writew(byte_addr[i], IC_DATA_CMD);

	data_cnt = 0;
    time_cnt = 0;

	while(data_cnt < len) {
		if(i2c_readw(IC_STATUS) & 0x2) {
			i2c_writew(*p, IC_DATA_CMD);
			p++;
			data_cnt++;
		}
        else{
            time_cnt ++;
            if (time_cnt >=100000)
                return 1;
        }

		if(i2c_readw(IC_RAW_INTR_STAT) & 0x40) {
            i = i2c_readw(IC_TX_ABRT_SOURCE);
			printf("Chipaddress=0x%x, subaddr=0x%x, IC_TX_ABRT_SOURCE=0x%x\n",
			     chip, addr, i);
			i = i2c_readw(IC_CLR_TX_ABRT);
			previous_chip = 0;

			return 1;
		}
	}



	/* Wait STOP condition */
    time_cnt=0;
	while (i2c_readw(IC_STATUS) & 0x1) {
		if ((i2c_readw(IC_RAW_INTR_STAT) & 0x40) || time_cnt >= 100000) {
			i = i2c_readw(IC_CLR_TX_ABRT);
			previous_chip = 0;
			return 1;
		}
        time_cnt ++;
    }

	return 0;
}

#endif /* CONFIG_ORION && CONFIG_HARD_I2C */
