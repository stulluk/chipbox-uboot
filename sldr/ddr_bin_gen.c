#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "ddr_config.h"
#define PARAS_NUM 6
int main()
{
    int ret;
    FILE *binfd;
    unsigned int paras[PARAS_NUM];
    binfd = fopen("./sldr/ddr_config.bin","w+");
    // sprintf(paras, "%d,%d,%d,%d", DDR_PLL,DDR_TIMING,DDR_MOD,DDR_DELAY_0,DDR_DELAY_1, DDR_MISC);
    paras[0] = DDR_PLL;
    paras[1] = DDR_TIMING;
    paras[2] = DDR_MOD;
    paras[3] = DDR_DELAY_0;
    paras[4] = DDR_DELAY_1;
    paras[5] = DDR_MISC;

    fwrite((void *)paras, sizeof(unsigned int), PARAS_NUM, binfd);
    fclose(binfd);


}
