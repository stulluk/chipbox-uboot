/*// ---------------------------------------------------------------------------
// FILE NAME        : clock.c
// MODULE NAME      : clock model
// AUTHOR           : Jiasheng Chen
// AUTHOR'S EMAIL   : jiasheng.chen@celestialsemi.com
// ---------------------------------------------------------------------------
// [RELEASE HISTORY]                           Last Modified : 07-05-23
// VERSION  DATE       AUTHOR                  DESCRIPTION
// 0.1      2004-12-16 hzjia                   Create
// 0.2      07-05-23   jiasheng Chen           Modified
// ---------------------------------------------------------------------------
// [DESCRIPTION]
// 
// ---------------------------------------------------------------------------
// $Id: 
///////////////////////////////////////////////////////////////////////*/

#ifdef WIN32_PCI_TEST  
#include <stdio.h>
#include <assert.h>
//#define ORION_130
#include <windows.h>
#else 
#include "../stdlib/c_stdlib.h" 
#endif
#include "hw_if.h"
#include "clock.h"

enum 
{
	REG_CLK_BASE = 0x10171000,
	REG_CLK_IDS_ID_LO   = REG_CLK_BASE + 0x000,
	REG_CLK_IDS_ID_HI   = REG_CLK_BASE + 0x004,
	REG_CLK_PLL_DDR_DIV = REG_CLK_BASE + 0x100,
	REG_CLK_PLL_BP      = REG_CLK_BASE + 0x104,
	REG_CLK_FREQ_CTRL   = REG_CLK_BASE + 0x108,
	REG_CLK_AUD_CLK_GEN_HPD    = REG_CLK_BASE + 0x10c,
	REG_CLK_AUD_CLK_GEN_FREQ   = REG_CLK_BASE + 0x118,
	REG_CLK_AUD_CLK_GEN_JITTER = REG_CLK_BASE + 0x11c,
	REG_CLK_SOFT_RESET  = REG_CLK_BASE + 0x200,
	REG_CLK_OPROC_CTRL  = REG_CLK_BASE + 0x400,

// ----------Test set registers ----------
    IDS_MBIST_DONE      = REG_CLK_BASE +  0x304,
    IDS_MBIST_RESULT0   = REG_CLK_BASE +  0x308,
    IDS_MBIST_RESULT1   = REG_CLK_BASE +  0x30c,
    IDS_TEST_CTRL       = REG_CLK_BASE +  0x310,

    // ----------System control register ----------
    IDS_OPROC_LOCTRL    = REG_CLK_BASE +  0x400,
    IDS_OPROC_HICTRL    = REG_CLK_BASE +  0x404,
    IDS_PCTL_HI         = REG_CLK_BASE +  0x408,
    IDS_PCTL_LO         = REG_CLK_BASE +  0x40c,

    //AHB error capturer
    IDS_HER_IND         = REG_CLK_BASE +  0x410,
    IDS_HER0_STAT0      = REG_CLK_BASE +  0x414,
    IDS_HER0_STAT1      = REG_CLK_BASE +  0x418,
    IDS_HER1_STAT0      = REG_CLK_BASE +  0x41c,
    IDS_HER1_STAT1      = REG_CLK_BASE +  0x420,
    IDS_HER2_STAT0      = REG_CLK_BASE +  0x424,
    IDS_HER2_STAT1      = REG_CLK_BASE +  0x428,
};

#ifdef WIN32_PCI_TEST
int InitClkReg()
{
	//Do Nothing
	U32 RegValue = 0;
	RegValue = RegRead(REG_CLK_IDS_ID_LO);
	printf("%-20s : %08x\n","REG_CLK_IDS_ID_LO", RegValue);
	RegValue = RegRead(REG_CLK_IDS_ID_HI);
	printf("%-20s : %08x\n","REG_CLK_IDS_ID_HI", RegValue);
	RegValue = RegRead(REG_CLK_PLL_DDR_DIV);
	printf("%-20s : %08x\n","REG_CLK_PLL_DDR_DIV", RegValue);
	RegValue = RegRead(REG_CLK_PLL_BP);
	printf("%-20s : %08x\n","REG_CLK_PLL_BP", RegValue);
	RegValue = RegRead(REG_CLK_FREQ_CTRL);
	printf("%-20s : %08x\n","REG_CLK_FREQ_CTRL", RegValue);

	return 1;
}
#endif


int SetTVEncoderClk(int IsHD, int IsProgressive)
{
	U16 FreqCtlValue = 0;
	U16 TVEPLLValue  = 0;
	HWReadShort(REG_CLK_FREQ_CTRL, &FreqCtlValue);
	FreqCtlValue  &= (~(1 << 3)); //Disable the TVE Clock
	HWWriteShort(REG_CLK_FREQ_CTRL, FreqCtlValue);
	//Delay for A while
	HWDelay(1024);
	//Set TVE clk pll value
	if (IsHD)
	{
		TVEPLLValue = 0x4621;
	}
	else
	{
		TVEPLLValue = 0x8618;
	}
	HWWriteShort(IDS_PCTL_LO, TVEPLLValue);
	//Delay for A while
	HWDelay(1024);
	//Enable the TVE Clk
	FreqCtlValue |= (1 << 3);
	//Set TVE_CLK6 switch:
	FreqCtlValue &= (~( 0x3 << 4));
	if (!IsHD && IsProgressive)
	{
		FreqCtlValue |= (0x2 << 4);
	}
	else
	{
		FreqCtlValue |= (0x1 << 4);
	}
	HWWriteShort(REG_CLK_FREQ_CTRL, FreqCtlValue);
	//Read Back to Check
	{
		U16 ReadValue = 0;
		HWReadShort( REG_CLK_FREQ_CTRL,&ReadValue);
		assert(FreqCtlValue == ReadValue);
	}

	//For Test
	//Enable CCIR 656 Clk

	return 1;
}

int SetVideoDACMux(int IsYPbPr)
{
	U16 RegValue = 0;
	HWReadShort(REG_CLK_OPROC_CTRL, &RegValue);
	if (IsYPbPr)         //Bit0	Dac0	Dac1	Dac2
	{
		RegValue |= 0x1; //1	Y	    Pb	    Pr
	}
	else
	{
		RegValue |= 0x0; //0	CVBS	S-Y     S-C
	}
	HWWriteShort(REG_CLK_OPROC_CTRL, RegValue);
	//Check Register Write
	HWReadShort(REG_CLK_OPROC_CTRL, &RegValue);
	if (IsYPbPr)         //Bit0	Dac0	Dac1	Dac2
	{
		if ((RegValue & 0x1) != 1) //1	Y	    Pb	    Pr
		{
			assert(((RegValue & 0x1) == 1));
			return 0;
		}		
	}
	else
	{
		if ((RegValue & 0x1) != 0) //1	Y	    Pb	    Pr
		{
			assert(((RegValue & 0x1) == 0));
			return 0;
		}
	}
	return 1;	
}

#ifdef WIN32_PCI_TEST
#define   FIN    27.0
#define   DELTA  2
#include <conio.h>
static int CaculatePLLClkReg( float FOUT, unsigned int *PLLRegValue)
{
	int m, n, no;
	int od;
	float  fvco, fin, fout;
	int pll_ddr_conf;
	int rt = 0;
	fin = FIN;

	//printf("\n=== OD    M     N   DIV  FOUT  FVCO  FREF===");
	
	assert(PLLRegValue != NULL);
	
	for(m=2; m<256; m++)
	{
		for(n=2; n<32; n++)
		{
            if((FIN/n < 1) || (FIN/n > 25) )
            {
                //printf("FIN/N should be between 1MHz-- 25MHz");
                //exit(1);
                continue;
            }
    
			for(no = 1; no <9; no*=2)
			{
				fout = (float)FIN * m / (n * no);
				if( (fout > (FOUT - DELTA)) &&
					fout < (FOUT + DELTA)) 
				{
					// fref = fin/nr/2;
					fvco = fin*m/n;
					if(((fvco>200)&&fvco<1000) )
					{
						if( no == 1) od = 0;	
						if( no == 2) od = 1;
						if( no == 4) od = 2;
                        if( no == 8) od = 3;
						pll_ddr_conf = (n<<9) | m | (od<<14);
						*PLLRegValue = pll_ddr_conf;
						rt = 1;
						printf("\n%2x    %2x    %3x    %5.2f %3.2f 0x%x", od, m, n,  fout, fvco, pll_ddr_conf);
						return rt;
					}
				}
			}	
		}
	}
	return rt;
}

int SetDDRPLLClk(float Clk) 
{
	unsigned int PLLRegValue = 0;
	int rt = 0;
	unsigned int ReadValue = 0;
	rt = CaculatePLLClkReg(Clk, &PLLRegValue);
	assert(rt);
	if (!rt) return rt;
	HWWriteShort(REG_CLK_PLL_DDR_DIV, PLLRegValue);
	HWReadShort(REG_CLK_PLL_DDR_DIV, &ReadValue);
	Sleep(10);
	assert(ReadValue == PLLRegValue);
	return (ReadValue == PLLRegValue);
}
#endif
