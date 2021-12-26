/*  driver for CAT6611 HDMI transmitter driver*/
#include <common.h>
#include <asm/io.h>
#include <command.h>
#include "cat6611.h"
#include <exports.h>

uchar OutputColorMode = F_MODE_RGB444 ;
unsigned VideoPixelClock ; 
uchar VIC ; // 480p60
uchar PixelRep ; // no pixelrepeating
HDMI_Colorimetry Colorimetry ;
HDMI_Aspec SetAspecType = HDMI_16x9;
uchar Authenticated = 0;

#ifndef SUPPORT_SYNCEMB
uchar InputColorMode = F_MODE_RGB444;
uchar InputSignalType = 0 ;
#else
uchar InputColorMode = F_MODE_YUV422 ;
uchar InputSignalType = T_MODE_SYNCEMB | T_MODE_CCIR656; // for 16 bit sync embedded
#endif // SUPPORT_SYNCEMB
uchar  EDID_Buf[128];
uchar bHDMIMode, bAudioEnable;

RX_CAP    RxCapability;
TXDEVINFO TXDev_Info[TXDEVCOUNT];
AVI_InfoFrame AviInfo;

uchar bCSCOffset_16_235[] =
{
    0x00, 0x80, 0x00
};

uchar bCSCOffset_0_255[] =
{
    0x10, 0x80, 0x10
};

#ifdef SUPPORT_INPUTYUV

uchar bCSCMtx_YUV2RGB_ITU601_16_235[] =
{
    0x00,0x08,0x6A,0x3A,0x4F,0x3D,
    0x00,0x08,0xF7,0x0A,0x00,0x00,
    0x00,0x08,0x00,0x00,0xDB,0x0D
} ;

uchar bCSCMtx_YUV2RGB_ITU601_0_255[] =
{
    0x4F,0x09,0x81,0x39,0xDF,0x3C,
    0x4F,0x09,0xC2,0x0C,0x00,0x00,
    0x4F,0x09,0x00,0x00,0x1E,0x10
} ;

uchar bCSCMtx_YUV2RGB_ITU709_16_235[] =
{
    0x00,0x08,0x53,0x3C,0x89,0x3E,
    0x00,0x08,0x51,0x0C,0x00,0x00,
    0x00,0x08,0x00,0x00,0x87,0x0E
} ;

uchar bCSCMtx_YUV2RGB_ITU709_0_255[] =
{
    0x4F,0x09,0xBA,0x3B,0x4B,0x3E,
    0x4F,0x09,0x56,0x0E,0x00,0x00,
    0x4F,0x09,0x00,0x00,0xE7,0x10
} ;
#endif

void DelayMS(word ms)
{
    udelay(ms * 1000);
}

void Ca6611_reset(void)
{
	gpio_hw_set_direct(15, 1);
	gpio_hw_write(15, 0);
	DelayMS(20);
	gpio_hw_write(15, 1);
}

uchar HDMITX_ReadI2C_Byte(uchar regAddr)
{
    int rev;
	uchar value = 0;
	
    rev = i2c_read (CAT6611_ADDR, regAddr, 1, &value, 1);
    if (rev > 0){
        printf("HDMI Error: I2C read %d Error!\n", CAT6611_ADDR<<1);
    }
    // udelay(100);
    return value;
}

int HDMITX_WriteI2C_Byte(uchar regAddr, uchar value)
{
    int rev;
	
    rev = i2c_write (CAT6611_ADDR, regAddr, 1, &value, 1);
    if (rev > 0){
        printf("HDMI Error: I2C wirte %d Error!\n", CAT6611_ADDR<<1);
    }
    // udelay(1000);
    return rev;
}

int HDMITX_ReadI2C_ByteN(uchar regAddr, uchar *pData, int len)
{
	int count;

	for (count = 0; count < len; count++)
	{
		pData[count] = HDMITX_ReadI2C_Byte(regAddr + count);
	}

	return 0;
}

int HDMITX_WriteI2C_ByteN(word regAddr, uchar *pData, int len)
{
	int count;
	int ret;

	for (count = 0; count < len; count++)
	{
		ret = HDMITX_WriteI2C_Byte(regAddr + count, pData[count]);

		if (ret > 0)
		{
			/* Error during writing */
			return ret;
		}
	}

	return 0;
}

#ifdef SUPPORT_SYNCEMB

// sync embedded table setting, defined as comment.
struct SyncEmbeddedSetting {
    uchar fmt ;
    uchar RegHVPol ; // Reg90
	uchar RegHfPixel ; // Reg91
    uchar RegHSSL ; // Reg95
    uchar RegHSEL ; // Reg96
    uchar RegHSH ; // Reg97
    uchar RegVSS1 ; // RegA0
    uchar RegVSE1 ; // RegA1
    uchar RegVSS2 ; // RegA2
    uchar RegVSE2 ; // RegA3
	uchar REGHVPol656; // Reg90 when CCIR656
	uchar REGHfPixel656; //Reg91 when CCIR656
	uchar RegHSSL656 ; // Reg95 when CCIR656
    uchar RegHSEL656 ; // Reg96 when CCIR656
    uchar RegHSH656 ; // Reg97 when CCIR656
    unsigned long PCLK ;
    uchar VFreq ;
} ;

static struct SyncEmbeddedSetting SyncEmbTable[] = {

	
 // {FMT,0x90, 0x91,
 //   VIC ,0x90, 0x91, 0x95, 0x96,0x97, 0xA0, 0xA1, 0xA2, 0xA3, 0x90_CCIR8,0x91_CCIR8    ,0x95_CCIR8,0x960_CCIR8   ,0x97_CCIR8  ,  PCLK    ,VFREQ},
    {   1 ,0xF0, 0x31, 0x0E, 0x6E,0x00, 0x0A, 0xC0, 0xFF, 0xFF, 0xF0      ,0x31          ,0x1D      ,0xDD          ,0X00            , 25175000,60},
    {   2 ,0xE0, 0x00, 0x0E, 0x4c,0x00, 0x09, 0xF0, 0x09, 0xF0, 0xF0      ,0x31          ,0x1D      ,0x99          ,0x00            , 27000000,60},
    {   3 ,0xE0, 0x00, 0x0E, 0x4c,0x00, 0x09, 0xF0, 0x09, 0xF0, 0xF0      ,0x31          ,0x1D      ,0x99          ,0x00            , 27000000,60},
    {   4 ,0xc6, 0x06, 0x6c, 0x94,0x00, 0x05, 0xA0, 0x05, 0xA0, 0x76      ,0x33          ,0xDA      ,0x2A          ,0x10            , 74175000,60},
    {   5 ,0x26, 0x4A, 0x56, 0x82,0x00, 0x02, 0x70, 0x34, 0x92, 0x46      ,0x94          ,0x57      ,0x83          ,0x00            , 74175000,60},
    {   6 ,0xE0, 0x1B, 0x11, 0x4F,0x00, 0x04, 0x70, 0x0A, 0xD1, 0xE0      ,0x37          ,0x23      ,0x9F          ,0x00            , 27000000,60},
    {   7 ,0xE0, 0x1B, 0x11, 0x4F,0x00, 0x04, 0x70, 0x0A, 0xD1, 0xE0      ,0x37          ,0x23      ,0x9F          ,0x00            , 27000000,60},
    {   8 ,0x00, 0xff, 0x11, 0x4F,0x00, 0x04, 0x70, 0xFF, 0xFF, 0x00      ,0xFF          ,0x23      ,0x9F          ,0x00            , 27000000,60},
    {   9 ,0x00, 0xff, 0x11, 0x4F,0x00, 0x04, 0x70, 0xFF, 0xFF, 0x00      ,0xFF          ,0x23      ,0x9F          ,0x00            , 27000000,60},
    {  10 ,0xe0, 0x1b, 0x11, 0x4F,0x00, 0x04, 0x70, 0x0A, 0xD1, 0xE0      ,0x37          ,0x23      ,0x9F          ,0x00            , 54000000,60},
    {  11 ,0xe0, 0x1b, 0x11, 0x4F,0x00, 0x04, 0x70, 0x0A, 0xD1, 0xE0      ,0x37          ,0x23      ,0x9F          ,0x00            , 54000000,60},
    {  12 ,0x00, 0xff, 0x11, 0x4F,0x00, 0x04, 0x70, 0xFF, 0xFF, 0x00      ,0xff          ,0x23      ,0x9F          ,0x00            , 54000000,60},
    {  13 ,0x00, 0xff, 0x11, 0x4F,0x00, 0x04, 0x70, 0xFF, 0xFF, 0x00      ,0xff          ,0x23      ,0x9F          ,0x00            , 54000000,60},
    {  14 ,0x00, 0xff, 0x1e, 0x9A,0x00, 0x09, 0xF0, 0xFF, 0xFF, 0x00      ,0xff          ,0x3D      ,0x35          ,0x10            , 54000000,60},
    {  15 ,0x00, 0xff, 0x1e, 0x9A,0x00, 0x09, 0xF0, 0xFF, 0xFF, 0x00      ,0xff          ,0x3D      ,0x35          ,0x10            , 54000000,60},
    {  16 ,0x06, 0xff, 0x56, 0x82,0x00, 0x04, 0x90, 0xFF, 0xFF, 0x00      ,0xff          ,0xFF      ,0xFF          ,0xFF            ,148350000,60},
    {  17 ,0xA0, 0x00, 0x0a, 0x4A,0x00, 0x05, 0xA0, 0x05, 0xA0, 0x50      ,0x01          ,0x15      ,0x95          ,0x00            , 27000000,50},
    {  18 ,0xA0, 0x00, 0x0a, 0x4A,0x00, 0x05, 0xA0, 0x05, 0xA0, 0x50      ,0x01          ,0x15      ,0x95          ,0x00            , 27000000,50},
    {  19 ,0x66, 0x1B, 0xB6, 0xDE,0x11, 0x05, 0xA0, 0x05, 0xA0, 0xD6      ,0x36          ,0x6D      ,0xBD          ,0x33            , 74250000,50},
    
    {  20 ,0x66, 0x73, 0x0e, 0x3A,0x22, 0x02, 0x70, 0x34, 0x92, 0xC6      ,0xE6          ,0x1D      ,0x75          ,0x44            , 74250000,50},
    {  21 ,0xA0, 0x1B, 0x0A, 0x49,0x00, 0x02, 0x50, 0x3A, 0xD1, 0x50      ,0x37          ,0x15      ,0x93          ,0x00            , 27000000,50},
    {  22 ,0xA0, 0x1B, 0x0a, 0x49,0x00, 0x02, 0x50, 0x3A, 0xD1, 0x50      ,0x37          ,0x15      ,0x93          ,0x00            , 27000000,50},
    {  23 ,0x00, 0xff, 0x0a, 0x49,0x00, 0x02, 0x50, 0xFF, 0xFF, 0x00      ,0xFF          ,0x15      ,0x93          ,0x00	        , 27000000,50},
    {  24 ,0x00, 0xff, 0x0a, 0x49,0x00, 0x02, 0x50, 0xFF, 0xFF, 0x00      ,0xFF          ,0x15      ,0x93          ,0x00	        , 27000000,50},
    {  25 ,0xA0, 0x1B, 0x0a, 0x49,0x00, 0x02, 0x50, 0x3A, 0xD1, 0x50      ,0x37          ,0x15      ,0x93          ,0x00	        , 54000000,50},
    {  26 ,0xA0, 0x1B, 0x0a, 0x49,0x00, 0x02, 0x50, 0x3A, 0xD1, 0x50      ,0x37          ,0x15      ,0x93          ,0x00	        , 54000000,50},
    {  27 ,0x00, 0xff, 0x0a, 0x49,0x00, 0x02, 0x50, 0xFF, 0xFF, 0x00      ,0xFF          ,0x15      ,0x93          ,0x00	        , 54000000,50},
    {  28 ,0x00, 0xff, 0x0a, 0x49,0x00, 0x02, 0x50, 0xFF, 0xFF, 0x00      ,0xFF          ,0x15      ,0x93          ,0x00	        , 54000000,50},
    {  29 ,0x04, 0xff, 0x16, 0x96,0x00, 0x05, 0xA0, 0xFF, 0xFF, 0x00      ,0xFF          ,0x2D      ,0x2D          ,0x10	        , 54000000,50},
    {  30 ,0x04, 0xff, 0x16, 0x96,0x00, 0x05, 0xA0, 0xFF, 0xFF, 0x00      ,0xFF          ,0x2D      ,0x2D          ,0x10	        , 54000000,50},
    {  31 ,0x06, 0xff, 0x0e, 0x3a,0x22, 0x04, 0x90, 0xFF, 0xFF, 0x00      ,0xFF          ,0xFF      ,0xFF          ,0xFF	        ,148500000,50},
    { 0xFF,0xFF, 0xff, 0xFF, 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF      ,0xFF          ,0xFF      ,0xFF          ,0xFF            , 0       , 0}
} ;

static uchar DEGen480i[] = {0x89,0x35,0xee,0x8e,0x60,0xB3,0x7B,0x06,0x0C,0x02,0x11,0x01,0x10,0x18,0x08,0x21,0x0C,0x22,0x06,0x91} ;
static uchar DEGen576i[] = {0xD9,0x35,0x06,0xA6,0x61,0xBD,0x7B,0x06,0x0C,0x02,0x16,0x36,0x10,0x4F,0x6F,0x21,0x00,0x30,0x39,0xC1} ;
static uchar DEGen480p[] = {0x39,0xFF,0xEE,0x8E,0x60,0xAE,0x75,0x06,0x0C,0x02,0x24,0x04,0x20,0x23,0x36,0x00,0x00,0x60,0x35,0xff} ;
static uchar DEGen576p[] = {0x39,0xFF,0x06,0xA6,0x61,0xBE,0x7E,0x06,0x0C,0x02,0x2C,0x6C,0x20,0x23,0x36,0x00,0x00,0x50,0x35,0xff} ;

uchar ProgramSyncEmbeddedVideoMode(uchar vIC, uchar bInputSignalType)
{
    int i ;
    // if Embedded Video, need to generate timing with pattern register

	if(bInputSignalType & T_MODE_SYNCEMB )
    {
      
        for( i = 0 ; SyncEmbTable[i].fmt != 0xFF ; i++ ) 
        {
            if( vIC == SyncEmbTable[i].fmt )
            {
                break ;
            }
        }
    
        if( SyncEmbTable[i].fmt == 0xFF )
        {
            return 0 ;
        }
    
    //    HDMITX_SetReg_Byte(REG_HVPol,~0x6,(SyncEmbTable[i].RegHVPol)&6) ; // Reg90
        Switch_HDMITX_Bank(0) ;
    	
        if(bInputSignalType & T_MODE_CCIR656) {
            HDMITX_WriteI2C_Byte(REG_HVPol,SyncEmbTable[i].REGHVPol656) ; // Reg90
            HDMITX_WriteI2C_Byte(REG_HfPixel,SyncEmbTable[i].REGHfPixel656) ; // Reg91
    		HDMITX_WriteI2C_Byte(REG_HSSL,SyncEmbTable[i].RegHSSL656) ; // Reg95
    		HDMITX_WriteI2C_Byte(REG_HSEL,SyncEmbTable[i].RegHSEL656) ; // Reg96
            HDMITX_WriteI2C_Byte(REG_HSH,SyncEmbTable[i].RegHSH656) ;   // Reg97
    	}
    	else 
    	{
            HDMITX_WriteI2C_Byte(REG_HVPol,SyncEmbTable[i].RegHVPol) ; // Reg90
            HDMITX_WriteI2C_Byte(REG_HfPixel,SyncEmbTable[i].RegHfPixel) ; // Reg91
    		HDMITX_WriteI2C_Byte(REG_HSSL,SyncEmbTable[i].RegHSSL) ; // Reg95
    		HDMITX_WriteI2C_Byte(REG_HSEL,SyncEmbTable[i].RegHSEL) ; // Reg96
    		HDMITX_WriteI2C_Byte(REG_HSH,SyncEmbTable[i].RegHSH) ; // Reg97
    	}
        HDMITX_WriteI2C_Byte(REG_VSS1,SyncEmbTable[i].RegVSS1) ; // RegA0
        HDMITX_WriteI2C_Byte(REG_VSE1,SyncEmbTable[i].RegVSE1) ; // RegA1 
        HDMITX_WriteI2C_Byte(REG_VSS2,SyncEmbTable[i].RegVSS2) ; // RegA2
        HDMITX_WriteI2C_Byte(REG_VSE2,SyncEmbTable[i].RegVSE2) ; // RegA3
    }
    else if( bInputSignalType & T_MODE_CCIR656 )
    {
        uchar *pTimingGen ;

        switch(vIC)
        {
        case 2:
        case 3:
            // 480p
            pTimingGen = DEGen480p ;
            break ;
        case 6:
        case 7:
            // 480i
            pTimingGen = DEGen480i ;
            break ;
        case 17:
        case 18:
            // 576p
            pTimingGen = DEGen576p ;
            break ;
        case 21:
        case 22:
            // 576i
            pTimingGen = DEGen576i ;
            break ;
        default:
            return 0 ;                    
        }
        
        Switch_HDMITX_Bank(0) ;
        for( i = 0x90 ; i < 0xA4 ; i++ )
        {
            HDMITX_WriteI2C_Byte(i,pTimingGen[i-0x90]) ; // Reg90 ~ RegA3
        }
    }

    return 1 ;
}
#endif

void ClearDDCFIFO(void)
{
    HDMITX_WriteI2C_Byte(REG_DDC_MASTER_CTRL, (B_MASTERDDC|B_MASTERHOST)) ;
    HDMITX_WriteI2C_Byte(REG_DDC_CMD, CMD_FIFO_CLR ) ;
}

void AbortDDC(void)
{
    uchar cPDesire, sWReset, dDCMaster ;
    uchar count, uc ;
    
    // save the SW reset, DDC master, and CP Desire setting.
    sWReset = HDMITX_ReadI2C_Byte(REG_SW_RST) ;
    cPDesire = HDMITX_ReadI2C_Byte(REG_HDCP_DESIRE) ;
    dDCMaster = HDMITX_ReadI2C_Byte(REG_DDC_MASTER_CTRL) ;
    
	
    HDMITX_WriteI2C_Byte(REG_HDCP_DESIRE, cPDesire&(~B_CPDESIRE)) ; // @emily change order
    HDMITX_WriteI2C_Byte(REG_SW_RST, sWReset|B_HDCP_RST) ;		 // @emily change order
    HDMITX_WriteI2C_Byte(REG_DDC_MASTER_CTRL, B_MASTERDDC|B_MASTERHOST) ;
    HDMITX_WriteI2C_Byte(REG_DDC_CMD, CMD_DDC_ABORT ) ;
    
    for( count = 200 ; count > 0 ; count-- )
    {
        uc = HDMITX_ReadI2C_Byte(REG_DDC_STATUS) ;
        if( uc & B_DDC_DONE )
        {
            break ;
        }
        if( uc & B_DDC_ERROR )
        {
            // error when abort.
            break ;
        }
        DelayMS(1) ; // delay 1 ms to stable.
    }
    
    // restore the SW reset, DDC master, and CP Desire setting.
    HDMITX_WriteI2C_Byte(REG_SW_RST, sWReset) ;
    HDMITX_WriteI2C_Byte(REG_HDCP_DESIRE,cPDesire) ;
    HDMITX_WriteI2C_Byte(REG_DDC_MASTER_CTRL,dDCMaster) ;
}

void FireAFE(void)
{
    Switch_HDMITX_Bank(0) ;
    // 2008/09/19 modified by jj_tseng@chipadvanced.com
    // HDMITX_WriteI2C_Byte(REG_AFE_DRV_CTRL,0x01<<O_AFE_DRV_SR /* 0x04 */) ;
    HDMITX_WriteI2C_Byte(REG_AFE_DRV_CTRL, 0x00) ; // disable pre-emphisis
    //~jj_tseng@chipadvanced.com
}

void HDMICheckTx(uchar *hPd)
{
	uchar hPdStatus;
	uchar sysStat;
	uchar intdata1;
	uchar intdata3;
	uchar intclr3;

	sysStat = HDMITX_ReadI2C_Byte(REG_SYS_STATUS);
	// printf("HDMICheckTx : sysStat[0x%02X] \n", sysStat);
	if ((sysStat & (B_HPDETECT|B_RXSENDETECT)) == (B_HPDETECT|B_RXSENDETECT))
	{
		hPdStatus = 1;
	}
	else
	{
		hPdStatus = 0;
		Authenticated = 0;
	}
	
    if(sysStat & B_INT_ACTIVE)
    {
        intdata1 = HDMITX_ReadI2C_Byte(REG_INT_STAT1);        
		if( intdata1 & B_INT_DDCFIFO_ERR )
		{
		    ClearDDCFIFO();
		}


		if( intdata1 & B_INT_DDC_BUS_HANG )
		{
			AbortDDC();
#if 0
			if( bAuthenticated )
			{
				// when DDC hang, and aborted DDC, the HDCP authentication need to restart.
				HDCP_ResumeAuthentication() ;
			}
#endif
		}


		if( intdata1 & (B_INT_HPD_PLUG|B_INT_RX_SENSE ))
		{
			if(!hPdStatus)
			{
				HDMITX_WriteI2C_Byte(REG_SW_RST, (B_AREF_RST|B_VID_RST|B_AUD_RST|B_HDCP_RST)) ;
				DelayMS(1) ;
				HDMITX_WriteI2C_Byte(REG_AFE_DRV_CTRL, (B_AFE_DRV_RST|B_AFE_DRV_PWD)) ;
			}
		}

#if 0
        intdata2 = HDMITX_ReadI2C_Byte(REG_INT_STAT2) ;

#ifdef SUPPORT_HDCP
		if( intdata2 & B_INT_AUTH_DONE )
		{
#ifdef CAT6611_DRV_DEBUG_ON
	            ErrorF("interrupt Authenticate Done.\n") ;                
#endif // #ifdef CAT6611_DRV_DEBUG_ON
	            HDMITX_OrReg_Byte(REG_INT_MASK2, B_AUTH_DONE_MASK) ;
	            bAuthenticated = 1 ;
	            SetAVMute(0) ;
		}


	if( intdata2 & B_INT_AUTH_FAIL )
		{
#ifdef CAT6611_DRV_DEBUG_ON
			ErrorF("interrupt Authenticate Fail.\n") ;       
#endif // #ifdef CAT6611_DRV_DEBUG_ON
			AbortDDC();   // @emily add 
			HDCP_ResumeAuthentication() ;
        }
#endif // SUPPORT_HDCP
#endif // #if 0

        intdata3 = HDMITX_ReadI2C_Byte(REG_INT_STAT3) ;
        if( intdata3 & B_INT_VIDSTABLE )
        {
            if(sysStat & B_TXVIDSTABLE)
            {
                FireAFE() ;
            }
        }

        HDMITX_WriteI2C_Byte(REG_INT_CLR0, 0xFF) ;
        HDMITX_WriteI2C_Byte(REG_INT_CLR1, 0xFF) ;
        intclr3 = (HDMITX_ReadI2C_Byte(REG_SYS_STATUS))|B_CLR_AUD_CTS | B_INTACTDONE ;
        HDMITX_WriteI2C_Byte(REG_SYS_STATUS, intclr3) ; // clear interrupt.
        intclr3 &= ~(B_INTACTDONE|B_CLR_AUD_CTS) ;
        HDMITX_WriteI2C_Byte(REG_SYS_STATUS, intclr3) ; // INTACTDONE reset to zero.
    }
    else
    {
		#if 0
        if(pHPDChange) 
        {
            *pHPDChange = (HPD != PrevHPD)?1:0 ;
            
            if( *pHPDChange &&(!HPD))
            {
                HDMITX_WriteI2C_Byte(REG_SW_RST,B_AREF_RST|B_VID_RST|B_AUD_RST|B_HDCP_RST) ;
                DelayMS(1) ;
                HDMITX_WriteI2C_Byte(REG_AFE_DRV_CTRL,B_AFE_DRV_RST|B_AFE_DRV_PWD) ;
            }
        }
		#else
		if(!hPdStatus)
		{
			HDMITX_WriteI2C_Byte(REG_SW_RST,B_AREF_RST|B_VID_RST|B_AUD_RST|B_HDCP_RST);
			DelayMS(1);
			HDMITX_WriteI2C_Byte(REG_AFE_DRV_CTRL,B_AFE_DRV_RST|B_AFE_DRV_PWD) ;
		}
		#endif
    }
    
    // SetAudioChannel() ;
    
    *hPd = hPdStatus;
	// printf("HDMICheckTx : hPdStatus[%d] \n", hPdStatus);
}

void HDMISetDisplayOption(void)
{
	#if 0
	/* Set Display mode to 720p50*/
	VIC = 19;
	VideoPixelClock = 74250000 ;
	PixelRep = 0 ;
	SetAspecType = HDMI_16x9 ;
	Colorimetry = HDMI_ITU709 ;
	OutputColorMode = F_MODE_RGB444;
	InputSignalType = T_MODE_SYNCEMB;
	InputColorMode |= F_VIDMODE_ITU709;
	// InputColorMode &= ~F_VIDMODE_ITU709 ;
	InputColorMode |= F_VIDMODE_16_235;
	#else
	/* Set Display mode to 576i50*/
	VIC = 21;
	VideoPixelClock = 13500000 ;
	PixelRep = 1 ;
	SetAspecType = HDMI_4x3 ;
	Colorimetry = HDMI_ITU601 ;
	OutputColorMode = F_MODE_RGB444;
	InputSignalType = T_MODE_SYNCEMB | T_MODE_CCIR656;
	InputColorMode &= ~F_VIDMODE_ITU709 ;
	InputColorMode |= F_VIDMODE_16_235;
	/*
	VIC = 17;
	VideoPixelClock = 27000000 ;
	PixelRep = 0;
	SetAspecType = HDMI_16x9;
	Colorimetry = HDMI_ITU601;
	OutputColorMode = F_MODE_RGB444;
	InputSignalType = T_MODE_SYNCEMB;
	InputColorMode &= ~F_VIDMODE_ITU709 ;
	InputColorMode |= F_VIDMODE_16_235;
	*/
	#endif
}

uchar ReadEDID(uchar *pData, uchar bSegment, uchar offset, word count)
{
    word  remainedCount, reqCount ;
    uchar bCurrOffset ;
    word  timeOut ;
    uchar *pBuff = pData ;
    uchar ucdata ;

    if( !pData )
    {
        return 0 ;
    }

    if(HDMITX_ReadI2C_Byte(REG_INT_STAT1) & B_INT_DDC_BUS_HANG)
    {
        AbortDDC() ;
    }

    ClearDDCFIFO() ;

    remainedCount = count ;
    bCurrOffset = offset ;

    Switch_HDMITX_Bank(0) ;

    while(remainedCount > 0 )
    {
		reqCount = (remainedCount > DDC_FIFO_MAXREQ)? DDC_FIFO_MAXREQ: remainedCount;

        ClearDDCFIFO() ;

        HDMITX_WriteI2C_Byte(REG_DDC_MASTER_CTRL, B_MASTERDDC|B_MASTERHOST) ;
        HDMITX_WriteI2C_Byte(REG_DDC_HEADER, DDC_EDID_ADDRESS ) ; // for EDID ucdata get
        HDMITX_WriteI2C_Byte(REG_DDC_REQOFF, bCurrOffset ) ;
        HDMITX_WriteI2C_Byte(REG_DDC_REQCOUNT, (uchar)reqCount ) ;
        HDMITX_WriteI2C_Byte(REG_DDC_EDIDSEG, bSegment) ;
        HDMITX_WriteI2C_Byte(REG_DDC_CMD, CMD_EDID_READ) ;

        bCurrOffset += reqCount ;
        remainedCount -= reqCount ;

        for(timeOut = 250 ; timeOut > 0 ; timeOut -- )
        {
            DelayMS(1) ;
            ucdata = HDMITX_ReadI2C_Byte(REG_DDC_STATUS) ;
            if( ucdata & B_DDC_DONE )
            {
                break ;
            }

            if( ucdata & B_DDC_ERROR)
            {
                return 0 ;
            }
        }

        if( timeOut == 0 )
        {
            return 0 ;
        }

        do
        {
            *(pBuff++) = HDMITX_ReadI2C_Byte(REG_DDC_READFIFO) ;
            reqCount -- ;
        }while(reqCount > 0) ;

    }

    return 1;
}

uchar GetEDIDData(int eDIDBlockID, uchar *pEDIDData)
{
    return ReadEDID(pEDIDData, eDIDBlockID/2, (eDIDBlockID%2)*128, 128);
}

uchar ParseCEAEDID(uchar *pCEAEDID)
{
    uchar offset, endPoint;
    uchar count ;
    uchar tag ;
    word  parCount;

    if( pCEAEDID[0] != 0x02 || pCEAEDID[1] != 0x03 )
    {
		return 0; // not a CEA BLOCK.
    }
	
    endPoint = pCEAEDID[2]  ; // CEA description.
    RxCapability.VideoMode = pCEAEDID[3] ;

	RxCapability.VDOModeCount = 0 ;
    RxCapability.idxNativeVDOMode = 0 ;
    parCount=0;
    for( offset = 4 ; offset < endPoint ; )
    {
        parCount++;
        tag = pCEAEDID[offset] >> 5 ;
        count = pCEAEDID[offset] & 0x1f ;
		
        switch( tag )
        {
        case 0x01: // Audio Data Block ;
            offset += count+1 ;
            break ;

        case 0x02: // Video Data Block ;
            offset += count+1 ;
            break ;

        case 0x03: // Vendor Specific Data Block ;
            offset ++ ;
            RxCapability.IEEEOUI = (unsigned long)pCEAEDID[offset+2] ;
            RxCapability.IEEEOUI <<= 8 ;
            RxCapability.IEEEOUI += (unsigned long)pCEAEDID[offset+1] ;
            RxCapability.IEEEOUI <<= 8 ;
            RxCapability.IEEEOUI += (unsigned long)pCEAEDID[offset] ;
            offset += count ; // ignore the remaind.

            break ;

        case 0x04: // Speaker Data Block ;
            offset ++ ;
            RxCapability.SpeakerAllocBlk.uc[0] = pCEAEDID[offset] ;
            RxCapability.SpeakerAllocBlk.uc[1] = pCEAEDID[offset+1] ;
            RxCapability.SpeakerAllocBlk.uc[2] = pCEAEDID[offset+2] ;
            offset += 3 ;
            break ;
			
        case 0x05: // VESA Data Block ;
            offset += count+1 ;
            break ;
			
        case 0x07: // Extended Data Block ;
            offset += count+1 ; //ignore
            break ;
			
        default:
            offset += count+1 ; // ignore
        }
		
        if(parCount>128)
            break;
    }
	
    RxCapability.ValidCEA = 1 ;
    return 1 ;
}

uchar ParseEDID(void)
{
    // collect the EDID ucdata of segment 0
    uchar checkSum ;
    uchar blockCount;
    uchar result = 1;
    uchar bValidCEA = 0;
    int i ;

    RxCapability.ValidCEA  = 0;
    RxCapability.ValidHDMI = 0;    /* Add By River 12.03.2008 */
	
    GetEDIDData(0, EDID_Buf);


    for( i = 0, checkSum = 0 ; i < 128 ; i++ )
    {
        checkSum += EDID_Buf[i];
		checkSum &= 0xFF ;
    }
	
	if( checkSum != 0 )
	{
		return 0;
	}
	
	if( EDID_Buf[0] != 0x00 ||
	    EDID_Buf[1] != 0xFF ||
	    EDID_Buf[2] != 0xFF ||
	    EDID_Buf[3] != 0xFF ||
	    EDID_Buf[4] != 0xFF ||
	    EDID_Buf[5] != 0xFF ||
	    EDID_Buf[6] != 0xFF ||
	    EDID_Buf[7] != 0x00)
    {
        return 0 ;
    }


    blockCount = EDID_Buf[0x7E] ;

    if( blockCount == 0 )
    {
        return 1; // do nothing.
    }
    else if ( blockCount > 4 )
    {
        blockCount = 4 ;
    }
        	
    for( i = 1 ; i <= blockCount ; i++ )
    {
        result = GetEDIDData(i, EDID_Buf) ;

        if( result )
        {  
           if( (!bValidCEA) && (EDID_Buf[0] == 0x2) && (EDID_Buf[1] == 0x3) )
            {
                result = ParseCEAEDID(EDID_Buf) ;
                if(result)
                {
 
				    if(RxCapability.IEEEOUI==0x0c03)
				    {
				    	RxCapability.ValidHDMI = 1 ;
				    	bValidCEA = 1 ;
					}
				    else
				    {
				    	RxCapability.ValidHDMI = 0 ;
				    }
				                   
                }
            }
        }
    }

    return result ;

}

void SetAVMute(uchar bEnable) 
{
    Switch_HDMITX_Bank(0) ;
	if (bEnable)
	{
		HDMITX_WriteI2C_Byte(REG_AV_MUTE, B_SET_AVMUTE) ;
	}
	else
	{
		HDMITX_WriteI2C_Byte(REG_AV_MUTE, B_CLR_AVMUTE) ;
	}
    HDMITX_WriteI2C_Byte(REG_PKT_GENERAL_CTRL, B_ENABLE_PKT|B_REPEAT_PKT) ;
}

void SetInputMode(uchar inputColorMode, uchar inputSignalType)
{
    uchar ucData ;
    
    ucData = HDMITX_ReadI2C_Byte(REG_INPUT_MODE) ;

    ucData &= ~(M_INCOLMOD|B_2X656CLK|B_SYNCEMB|B_INDDR) ;

    switch( inputColorMode & F_MODE_CLRMOD_MASK )
    {
    case F_MODE_YUV422:
        ucData |= B_IN_YUV422 ;
        break ;
    case F_MODE_YUV444:
        ucData |= B_IN_YUV444 ;
        break ;
    case F_MODE_RGB444:
    default:
        ucData |= B_IN_RGB ;
        break ;
    }

    if( inputSignalType & T_MODE_CCIR656 )
    {
        ucData |= B_2X656CLK ;
    }

    if( inputSignalType & T_MODE_SYNCEMB )
    {
        ucData |= B_SYNCEMB ;
    }

    if( inputSignalType & T_MODE_INDDR )
    {
        ucData |= B_INDDR ;
    }

    HDMITX_WriteI2C_Byte(REG_INPUT_MODE, ucData) ;
}

void SetCSCScale(uchar bInputMode,uchar bOutputMode)
{
    uchar ucData; 
    uchar csc = B_CSC_YUV2RGB;
    uchar filter = 0 ; // filter is for Video CTRL DN_FREE_GO, EN_DITHER, and ENUDFILT


	// (1) YUV422 in, RGB/YUV444 output ( Output is 8-bit, input is 12-bit)
	// (2) YUV444/422  in, RGB output ( CSC enable, and output is not YUV422 )
	// (3) RGB in, YUV444 output   ( CSC enable, and output is not YUV422 )
    // 
    // YUV444/RGB444 <-> YUV422 need set up/down filter.
    
    switch(bInputMode & F_MODE_CLRMOD_MASK)
    {
    #ifdef SUPPORT_INPUTYUV422
    case F_MODE_YUV422:
    default:                    
        switch(bOutputMode & F_MODE_CLRMOD_MASK)
        {
        case F_MODE_YUV444:
            csc = B_CSC_BYPASS ;
            if( bInputMode & F_VIDMODE_EN_UDFILT) // YUV422 to YUV444 need up filter
            {
                filter |= B_EN_UDFILTER ;
            }

            if( bInputMode & F_VIDMODE_EN_DITHER) // YUV422 to YUV444 need dither
            {
                filter |= B_EN_DITHER | B_DNFREE_GO ;
            }

            break ;
        case F_MODE_YUV422:
            csc = B_CSC_BYPASS ;

            break ;

        case F_MODE_RGB444:
        default:       
            csc = B_CSC_YUV2RGB ;
            if( bInputMode & F_VIDMODE_EN_UDFILT) // YUV422 to RGB444 need up/dn filter.
            {
                filter |= B_EN_UDFILTER ;
            }
            
            if( bInputMode & F_VIDMODE_EN_DITHER) // YUV422 to RGB444 need dither
            {
                filter |= B_EN_DITHER | B_DNFREE_GO ;
            }
            break ;
        }
        break ;
    #endif        
    
    }
    #ifdef SUPPORT_INPUTYUV
    if(csc == B_CSC_BYPASS)
    {

    }
    else// ( csc == B_CSC_YUV2RGB )
    {
        switch(bInputMode & (F_VIDMODE_ITU709|F_VIDMODE_16_235))
        {
        case F_VIDMODE_ITU709|F_VIDMODE_16_235://11
            HDMITX_WriteI2C_ByteN(REG_CSC_YOFF, bCSCOffset_16_235, SIZEOF_CSCOFFSET) ;
            HDMITX_WriteI2C_ByteN(REG_CSC_MTX11_L,bCSCMtx_YUV2RGB_ITU709_16_235,SIZEOF_CSCMTX) ;
            break ;
        case F_VIDMODE_ITU709|F_VIDMODE_0_255://01
            HDMITX_WriteI2C_ByteN(REG_CSC_YOFF, bCSCOffset_0_255, SIZEOF_CSCOFFSET) ;
            HDMITX_WriteI2C_ByteN(REG_CSC_MTX11_L,bCSCMtx_YUV2RGB_ITU709_0_255,SIZEOF_CSCMTX) ;
            break ;
        case F_VIDMODE_ITU601|F_VIDMODE_16_235://10
            HDMITX_WriteI2C_ByteN(REG_CSC_YOFF, bCSCOffset_16_235, SIZEOF_CSCOFFSET) ;
            HDMITX_WriteI2C_ByteN(REG_CSC_MTX11_L,bCSCMtx_YUV2RGB_ITU601_16_235,SIZEOF_CSCMTX) ;
            break ;
        case F_VIDMODE_ITU601|F_VIDMODE_0_255://00
        default:            
            HDMITX_WriteI2C_ByteN(REG_CSC_YOFF, bCSCOffset_0_255, SIZEOF_CSCOFFSET) ;
            HDMITX_WriteI2C_ByteN(REG_CSC_MTX11_L,bCSCMtx_YUV2RGB_ITU601_0_255,SIZEOF_CSCMTX) ;
            break ;
        }
    }
    #endif
    ucData = HDMITX_ReadI2C_Byte(REG_CSC_CTRL) & ~(M_CSC_SEL|B_DNFREE_GO|B_EN_DITHER|B_EN_UDFILTER) ;
    ucData |= filter|csc ;

    HDMITX_WriteI2C_Byte(REG_CSC_CTRL,ucData) ;

    // set output Up/Down Filter, Dither control

}

void SetHDMIMode(uchar bHDMIMode)
{
    Switch_HDMITX_Bank(0) ;
    if(bHDMIMode)
    {
        HDMITX_WriteI2C_Byte(REG_HDMI_MODE, B_HDMI_MODE) ;
    }
    else
    {
        HDMITX_WriteI2C_Byte(REG_HDMI_MODE, B_DVI_MODE) ;

        DISABLE_NULL_PKT();
        DISABLE_ACP_PKT();
        DISABLE_ISRC1_PKT();
        DISABLE_ISRC2_PKT();
        DISABLE_AVI_INFOFRM_PKT();
        DISABLE_AUD_INFOFRM_PKT();
        DISABLE_SPD_INFOFRM_PKT();
        DISABLE_MPG_INFOFRM_PKT();
        Switch_HDMITX_Bank(1) ;
        HDMITX_WriteI2C_Byte(REG_AVIINFO_DB1, 0) ;
    }
    Switch_HDMITX_Bank(0) ;
}

void SetupAFE(uchar highFreqMode)
{
    uchar uc ;
    
	// @emily turn off reg61 before SetupAFE parameters.
    Switch_HDMITX_Bank(0) ;
    uc = (HDMITX_ReadI2C_Byte(REG_INT_CTRL)&B_IDENT_6612)? B_AFE_XP_BYPASS: 0 ;
    // for identify the CAT6612 operation.
    
    HDMITX_WriteI2C_Byte(REG_AFE_DRV_CTRL,B_AFE_DRV_RST);/* 0x00 */
    if( highFreqMode )
    {
        HDMITX_WriteI2C_Byte(REG_AFE_XP_CTRL1,B_AFE_XP_GAINBIT|B_AFE_XP_RESETB/* 0x88 */|uc) ;
        HDMITX_WriteI2C_Byte(REG_AFE_XP_CTRL2,B_XP_CLKSEL_1_PCLKHV/*0x01*/) ;
        HDMITX_WriteI2C_Byte(REG_AFE_IP_CTRL, B_AFE_IP_GAINBIT|B_AFE_IP_SEDB|B_AFE_IP_RESETB|B_AFE_IP_PDIV1 /*0x56*/) ;
        HDMITX_WriteI2C_Byte(REG_AFE_RING,0x00) ;
    }
    else
    {
        HDMITX_WriteI2C_Byte(REG_AFE_XP_CTRL1,B_AFE_XP_ER0|B_AFE_XP_RESETB/* 0x18 */|uc) ;
        HDMITX_WriteI2C_Byte(REG_AFE_XP_CTRL2,B_XP_CLKSEL_1_PCLKHV/*0x01*/) ;
        HDMITX_WriteI2C_Byte(REG_AFE_IP_CTRL, B_AFE_IP_SEDB|B_AFE_IP_ER0|B_AFE_IP_RESETB|B_AFE_IP_PDIV1 /*0x1E*/) ;
        HDMITX_WriteI2C_Byte(REG_AFE_RING,0x00) ;
    }
    HDMITX_AndReg_Byte(REG_SW_RST,~(B_REF_RST|B_VID_RST/*|B_AREF_RST*/|B_HDMI_RST)) ;

    // REG_AFE_DRV_CTRL have to be set at the last step of setup .
}

uchar EnableVideoOutput(uchar HiFreq, uchar inputColorMode, uchar inputSignalType, uchar outputColorMode, uchar bHDMI)
{
    // bInputVideoMode, bOutputVideoMode, bVideoInputType, bAudioInputType, should be configured by upper F/W or loaded from EEPROM.
    // should be configured by initsys.c
    int i ;

    HDMITX_WriteI2C_Byte(REG_SW_RST, (B_VID_RST|B_AUD_RST|B_AREF_RST|B_HDCP_RST)) ;
    
    TXDev_Info[0].bHDMIMode = bHDMI ;
    
    if( bHDMI )
    {
        SetAVMute(1) ;
    }

    SetInputMode(inputColorMode, inputSignalType) ;

    SetCSCScale(inputColorMode, outputColorMode) ;

    SetHDMIMode(bHDMI) ;

    SetupAFE(HiFreq) ; // pass if High Freq request    
    
    for(i = 0 ; i < 100 ; i++)
    {
        if( HDMITX_ReadI2C_Byte(REG_SYS_STATUS) & B_TXVIDSTABLE )
        {
            break ;
        }
        DelayMS(1) ;
    }
    FireAFE() ;
	
	return 1;
}

void HDCP_ClearAuthInterrupt(void)
{
    uchar uc ;
    uc = HDMITX_ReadI2C_Byte(REG_INT_MASK2) & (~(B_KSVLISTCHK_MASK|B_AUTH_DONE_MASK|B_AUTH_FAIL_MASK));
    HDMITX_WriteI2C_Byte(REG_INT_CLR0, B_CLR_AUTH_FAIL|B_CLR_AUTH_DONE|B_CLR_KSVLISTCHK) ;
    HDMITX_WriteI2C_Byte(REG_INT_CLR1, 0) ;
    HDMITX_WriteI2C_Byte(REG_SYS_STATUS, B_INTACTDONE) ;
}

void HDCP_Reset(void)
{
    uchar uc ;
    HDMITX_WriteI2C_Byte(REG_LISTCTRL, 0) ;
    HDMITX_WriteI2C_Byte(REG_HDCP_DESIRE, 0) ;
    uc = HDMITX_ReadI2C_Byte(REG_SW_RST) ;
    uc |= B_HDCP_RST ;
    HDMITX_WriteI2C_Byte(REG_SW_RST, uc) ;
    HDMITX_WriteI2C_Byte(REG_DDC_MASTER_CTRL, B_MASTERDDC|B_MASTERHOST) ;
    HDCP_ClearAuthInterrupt() ;
    AbortDDC() ;
}

uchar SetAVIInfoFrame(AVI_InfoFrame *pAVIInfoFrame)
{
    int i ;
    uchar ucData ;

    if(!pAVIInfoFrame)
    {
        return 0;
    }

    Switch_HDMITX_Bank(1) ;
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB1, pAVIInfoFrame->pktbyte.AVI_DB[0]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB2, pAVIInfoFrame->pktbyte.AVI_DB[1]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB3, pAVIInfoFrame->pktbyte.AVI_DB[2]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB4, pAVIInfoFrame->pktbyte.AVI_DB[3]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB5, pAVIInfoFrame->pktbyte.AVI_DB[4]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB6, pAVIInfoFrame->pktbyte.AVI_DB[5]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB7, pAVIInfoFrame->pktbyte.AVI_DB[6]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB8, pAVIInfoFrame->pktbyte.AVI_DB[7]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB9, pAVIInfoFrame->pktbyte.AVI_DB[8]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB10, pAVIInfoFrame->pktbyte.AVI_DB[9]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB11, pAVIInfoFrame->pktbyte.AVI_DB[10]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB12, pAVIInfoFrame->pktbyte.AVI_DB[11]);
	HDMITX_WriteI2C_Byte(REG_AVIINFO_DB13, pAVIInfoFrame->pktbyte.AVI_DB[12]);
	ucData = 0;
	
    for( i = 0, ucData = 0; i < 13 ; i++ )
    {
        ucData -= pAVIInfoFrame->pktbyte.AVI_DB[i] ;
    }

    ucData -= 0x80 + AVI_INFOFRAME_VER + AVI_INFOFRAME_TYPE + AVI_INFOFRAME_LEN ;
	HDMITX_WriteI2C_Byte(REG_AVIINFO_SUM, ucData);


    Switch_HDMITX_Bank(0) ;
    ENABLE_AVI_INFOFRM_PKT();
	
    return 1;
}

void ConfigAVIInfoFrame(uchar vIC, uchar pixelrep)
{
    AviInfo.pktbyte.AVI_HB[0] = AVI_INFOFRAME_TYPE|0x80 ; 
    AviInfo.pktbyte.AVI_HB[1] = AVI_INFOFRAME_VER ; 
    AviInfo.pktbyte.AVI_HB[2] = AVI_INFOFRAME_LEN ; 

    switch(OutputColorMode)
    {
    case F_MODE_YUV444:
        // AviInfo.info.ColorMode = 2 ;
        AviInfo.pktbyte.AVI_DB[0] = (2<<5)|(1<<4) ;
        break ;
    case F_MODE_YUV422:
        // AviInfo.info.ColorMode = 1 ;
        AviInfo.pktbyte.AVI_DB[0] = (1<<5)|(1<<4) ;
        break ;
    case F_MODE_RGB444:
    default:
        // AviInfo.info.ColorMode = 0 ;
        AviInfo.pktbyte.AVI_DB[0] = (0<<5)|(1<<4) ;
        break ;
    }
// 
//     AviInfo.info.ActiveFormatAspectRatio = 8 ; // same as picture aspect ratio
    AviInfo.pktbyte.AVI_DB[1] = 8 ;
//     AviInfo.info.PictureAspectRatio = (aspec != HDMI_16x9)?1:2 ; // 4x3
    AviInfo.pktbyte.AVI_DB[1] |= (SetAspecType != HDMI_16x9)?(1<<4):(2<<4) ; // 4:3 or 16:9
//     AviInfo.info.Colorimetry = (Colorimetry != HDMI_ITU709) ? 1:2 ; // ITU601
    AviInfo.pktbyte.AVI_DB[1] |= (Colorimetry != HDMI_ITU709)?(1<<6):(2<<6) ; // 4:3 or 16:9
//     AviInfo.info.Scaling = 0 ;
    AviInfo.pktbyte.AVI_DB[2] = 0 ;
//     AviInfo.info.VIC = VIC ;
    AviInfo.pktbyte.AVI_DB[3] = vIC ;
//     AviInfo.info.PixelRepetition = pixelrep;
    AviInfo.pktbyte.AVI_DB[4] =  pixelrep & 3 ;
    AviInfo.pktbyte.AVI_DB[5] = 0 ;
    AviInfo.pktbyte.AVI_DB[6] = 0 ;
    AviInfo.pktbyte.AVI_DB[7] = 0 ;
    AviInfo.pktbyte.AVI_DB[8] = 0 ;
    AviInfo.pktbyte.AVI_DB[9] = 0 ;
    AviInfo.pktbyte.AVI_DB[10] = 0 ;
    AviInfo.pktbyte.AVI_DB[11] = 0 ;
    AviInfo.pktbyte.AVI_DB[12] = 0 ;

    SetAVIInfoFrame(&AviInfo) ;
}

void DisableHDCP(void)
{
	HDMITX_OrReg_Byte(REG_SW_RST, B_HDCP_RST) ;
	HDMITX_WriteI2C_Byte(REG_HDCP_DESIRE, 0) ;
}

void DisableVideoOutput(void)
{
    HDMITX_WriteI2C_Byte(REG_SW_RST, B_VID_RST|B_AUD_RST|B_AREF_RST|B_HDCP_RST) ;
    HDMITX_WriteI2C_Byte(REG_AFE_DRV_CTRL, B_AFE_DRV_PWD|B_AFE_DRV_ENBIST) ;
}

void DisableAudioOutput(void)
{
    uchar uc ;
    
    Switch_HDMITX_Bank(0) ;
    
    uc = HDMITX_ReadI2C_Byte(REG_AUDIO_CTRL0) ;
    uc = 0xC0 ;
    HDMITX_WriteI2C_Byte(REG_AUDIO_CTRL0, uc) ;
    uc = HDMITX_ReadI2C_Byte(REG_SW_RST) ;
    HDMITX_WriteI2C_Byte(REG_SW_RST, uc|B_AUD_RST|B_AREF_RST) ;
}

void HDMITX_SetOutput(void)
{
	uchar videoClockHi;

	if (VideoPixelClock > _80MHz)
	{
		videoClockHi = 1;
	}
	else
	{
		videoClockHi = 0;
	}
    #ifdef SUPPORT_SYNCEMB
    ProgramSyncEmbeddedVideoMode(VIC, InputSignalType) ; // inf CCIR656 input
    #endif
    EnableVideoOutput(videoClockHi, InputColorMode, InputSignalType, OutputColorMode, bHDMIMode) ;
    HDCP_Reset();//mingchih add
    if(bHDMIMode)
    {
        ConfigAVIInfoFrame(VIC, PixelRep) ;

        DisableHDCP();
		/*
		if( bAudioEnable )
		{
		    SetNonPCMAudio(0) ; // For LPCM audio. If AC3 or other compressed audio, please set the parameter as 1
            EnableAudioOutput(VideoPixelClock*(PixelRep+1),48000, 2, FALSE);
            ConfigAudioInfoFrm() ;
		}
		*/
    }
    SetAVMute(0) ;
}

void HDMISetOutput(void)
{
	uchar hPdValue;
	uchar count = 0;

	HDMICheckTx(&hPdValue);
	while((hPdValue == 0) && (count < 10))
	{
		DelayMS(100);
		HDMICheckTx(&hPdValue);
		count++;
	}

	if (hPdValue)
	{
		ParseEDID() ;

		// 2008/09/19 modified by jj_tseng@chipadvanced.com
		OutputColorMode = F_MODE_RGB444 ;
		OutputColorMode &= ~F_MODE_CLRMOD_MASK ; // color mode need to reset at first before DVI/HDMI/YUV check
		if( RxCapability.ValidHDMI )
		{
			bHDMIMode = 1 ;

			if(RxCapability.VideoMode & (1<<6))
			{
				bAudioEnable = 1 ;
			}

			if (RxCapability.VideoMode & (1<<4))
			{
				OutputColorMode = F_MODE_YUV422 ;
			}

			if( RxCapability.VideoMode & (1<<5))
			{
				OutputColorMode = F_MODE_YUV444;
			}
		}
		else
		{
			bHDMIMode = 0 ;
			bAudioEnable = 0 ;
			OutputColorMode &= ~F_MODE_CLRMOD_MASK ; // color mode need to reset at first before DVI/HDMI/YUV check
			OutputColorMode = F_MODE_RGB444 ;
		}
		HDMITX_SetOutput();
	}
	else
	{
		DisableAudioOutput();
		DisableVideoOutput();
	}
}

void SetCAT6611Display(void)
{
	HDMISetDisplayOption();
	HDMISetOutput();
}

void InitCAT6611(void)
{
	Ca6611_reset();
	HDMISetDisplayOption();
    Switch_HDMITX_Bank(0) ;
    HDMITX_WriteI2C_Byte(REG_INT_CTRL, (B_INTPOL_ACTL | B_INTPOL_ACTH)) ; // depends on the interrupt type
    // bIntPOL = (bIntType&B_INTPOL_ACTH)?1:0 ;

    HDMITX_WriteI2C_Byte(REG_SW_RST, (B_REF_RST|B_VID_RST|B_AUD_RST|B_AREF_RST|B_HDCP_RST)) ;
    DelayMS(10) ;
    HDMITX_WriteI2C_Byte(REG_SW_RST, (B_VID_RST|B_AUD_RST|B_AREF_RST|B_HDCP_RST)) ;

    // Avoid power loading in un play status.
    HDMITX_WriteI2C_Byte(REG_AFE_DRV_CTRL, (B_AFE_DRV_RST/*|B_AFE_DRV_PWD*/)) ;

    // set interrupt mask, mask value 0 is interrupt available.
    HDMITX_WriteI2C_Byte(REG_INT_MASK1, 0xF8) ;
    HDMITX_WriteI2C_Byte(REG_INT_MASK2, 0xF8) ;
    HDMITX_WriteI2C_Byte(REG_INT_MASK3, 0xF7) ;

    
    DISABLE_NULL_PKT();
    DISABLE_ACP_PKT();
    DISABLE_ISRC1_PKT();
    DISABLE_ISRC2_PKT();
    DISABLE_AVI_INFOFRM_PKT();
    DISABLE_AUD_INFOFRM_PKT();
    DISABLE_SPD_INFOFRM_PKT();
    DISABLE_MPG_INFOFRM_PKT();

	HDMISetOutput();
}

