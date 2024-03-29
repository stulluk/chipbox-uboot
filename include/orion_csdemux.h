#ifndef __ORION_CSDEMUX_H__
#define __ORION_CSDEMUX_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_CELESTIAL)

#define CSDEMUX_INVALID_HANDLE     -1

typedef void *CSDEMUX_HANDLE;

typedef enum
{
	DEMUX_SUCCEED = 0,
	DEMUX_FAILED = -1
} CSDEMUX_RESULT;

typedef enum
{
	DEMUX_DISABLE = 0,
	DEMUX_ENABLE = 1,
} CSDEMUX_STATUS;

typedef enum 
{
	DEMUX_CHL_UNDEF = -1,
	DEMUX_CHL_ID0 = 0,
	DEMUX_CHL_ID1,
	DEMUX_CHL_NUM
} CSDEMUX_CHL_ID;

typedef enum 
{
	DEMUX_PIDFT_UNDEF = -1,
	DEMUX_PIDFT_ID0 = 0,
	DEMUX_PIDFT_ID1,
	DEMUX_PIDFT_ID2,
	DEMUX_PIDFT_ID3,
	DEMUX_PIDFT_ID4,
	DEMUX_PIDFT_ID5,
	DEMUX_PIDFT_ID6,
	DEMUX_PIDFT_ID7,
	DEMUX_PIDFT_ID8,
	DEMUX_PIDFT_ID9,
	DEMUX_PIDFT_ID10,
	DEMUX_PIDFT_ID11,
	DEMUX_PIDFT_ID12,
	DEMUX_PIDFT_ID13,
	DEMUX_PIDFT_ID14,
	DEMUX_PIDFT_ID15,
	DEMUX_PIDFT_ID16,
	DEMUX_PIDFT_ID17,
	DEMUX_PIDFT_ID18,
	DEMUX_PIDFT_ID19,
	DEMUX_PIDFT_ID20,
	DEMUX_PIDFT_ID21,
	DEMUX_PIDFT_ID22,
	DEMUX_PIDFT_ID23,
	DEMUX_PIDFT_ID24,
	DEMUX_PIDFT_ID25,
	DEMUX_PIDFT_ID26,
	DEMUX_PIDFT_ID27,
	DEMUX_PIDFT_ID28,
	DEMUX_PIDFT_ID29,
	DEMUX_PIDFT_ID30,
	DEMUX_PIDFT_ID31,
	DEMUX_PIDFT_ID32,
	DEMUX_PIDFT_ID33,
	DEMUX_PIDFT_ID34,
	DEMUX_PIDFT_ID35,
	DEMUX_PIDFT_ID36,
	DEMUX_PIDFT_ID37,
	DEMUX_PIDFT_ID38,
	DEMUX_PIDFT_ID39,
	DEMUX_PIDFT_ID40,
	DEMUX_PIDFT_ID41,
	DEMUX_PIDFT_ID42,
	DEMUX_PIDFT_ID43,
	DEMUX_PIDFT_ID44,
	DEMUX_PIDFT_ID45,
	DEMUX_PIDFT_ID46,
	DEMUX_PIDFT_ID47,
	DEMUX_PIDFT_ID48,
	DEMUX_PIDFT_ID49,
	DEMUX_PIDFT_ID50,
	DEMUX_PIDFT_ID51,
	DEMUX_PIDFT_ID52,
	DEMUX_PIDFT_ID53,
	DEMUX_PIDFT_ID54,
	DEMUX_PIDFT_ID55,
	DEMUX_PIDFT_ID56,
	DEMUX_PIDFT_ID57,
	DEMUX_PIDFT_ID58,
	DEMUX_PIDFT_ID59,
	DEMUX_PIDFT_ID60,
	DEMUX_PIDFT_ID61,
	DEMUX_PIDFT_ID62,
	DEMUX_PIDFT_ID63,
	DEMUX_PIDFT_NUM
} CSDEMUX_PIDFT_ID;

typedef enum 
{
	DEMUX_FILTER_UNDEF = -1,
	DEMUX_FILTER_ID0 = 0,
	DEMUX_FILTER_ID1,
	DEMUX_FILTER_ID2,
	DEMUX_FILTER_ID3,
	DEMUX_FILTER_ID4,
	DEMUX_FILTER_ID5,
	DEMUX_FILTER_ID6,
	DEMUX_FILTER_ID7,
	DEMUX_FILTER_ID8,
	DEMUX_FILTER_ID9,
	DEMUX_FILTER_ID10,
	DEMUX_FILTER_ID11,
	DEMUX_FILTER_ID12,
	DEMUX_FILTER_ID13,
	DEMUX_FILTER_ID14,
	DEMUX_FILTER_ID15,
	DEMUX_FILTER_ID16,
	DEMUX_FILTER_ID17,
	DEMUX_FILTER_ID18,
	DEMUX_FILTER_ID19,
	DEMUX_FILTER_ID20,
	DEMUX_FILTER_ID21,
	DEMUX_FILTER_ID22,
	DEMUX_FILTER_ID23,
	DEMUX_FILTER_ID24,
	DEMUX_FILTER_ID25,
	DEMUX_FILTER_ID26,
	DEMUX_FILTER_ID27,
	DEMUX_FILTER_ID28,
	DEMUX_FILTER_ID29,
	DEMUX_FILTER_ID30,
	DEMUX_FILTER_ID31,
	DEMUX_FILTER_NUM
} CSDEMUX_FILTER_ID;

typedef enum 
{
	DEMUX_FILTER_TYPE_UNDEF = -1,
	DEMUX_FILTER_TYPE_SEC = 0,
	DEMUX_FILTER_TYPE_TS,
	DEMUX_FILTER_TYPE_PES,
	DEMUX_FILTER_TYPE_ES
} CSDEMUX_FILTER_TYPE;

typedef enum 
{
	DEMUX_INPUT_MOD_TUNER = 0,
	DEMUX_INPUT_MOD_DMA,
	DEMUX_INPUT_MOD_DIRECT
} CSDEMUX_INPUT_MOD;



/*XPORT API*/
/* common interface*/
CSDEMUX_RESULT CSDEMUX_Init(void);

/* channel interface */
CSDEMUX_HANDLE CSDEMUX_CHL_Open(CSDEMUX_CHL_ID chl_id);
CSDEMUX_RESULT CSDEMUX_CHL_Close(CSDEMUX_HANDLE handle);
CSDEMUX_RESULT CSDEMUX_CHL_SetInputMode(CSDEMUX_HANDLE handle, CSDEMUX_INPUT_MOD input_mod);
CSDEMUX_RESULT CSDEMUX_CHL_Enable(CSDEMUX_HANDLE handle);
CSDEMUX_RESULT CSDEMUX_CHL_Disable(CSDEMUX_HANDLE handle);

/*pid filter interface*/
CSDEMUX_HANDLE CSDEMUX_PIDFT_Open(CSDEMUX_PIDFT_ID pidfilter_id);
CSDEMUX_RESULT CSDEMUX_PIDFT_Close(CSDEMUX_HANDLE handle);
CSDEMUX_RESULT CSDEMUX_PIDFT_SetChannel(CSDEMUX_HANDLE handle, CSDEMUX_CHL_ID chl_id);
CSDEMUX_RESULT CSDEMUX_PIDFT_SetPID(CSDEMUX_HANDLE handle, unsigned short pid);
CSDEMUX_RESULT CSDEMUX_PIDFT_Enable(CSDEMUX_HANDLE handle);
CSDEMUX_RESULT CSDEMUX_PIDFT_Disable(CSDEMUX_HANDLE handle);

/* Filter interface */
CSDEMUX_HANDLE CSDEMUX_Filter_Open(CSDEMUX_FILTER_ID filter_id);
CSDEMUX_RESULT CSDEMUX_Filter_Close(CSDEMUX_HANDLE handle);
CSDEMUX_RESULT CSDEMUX_Filter_SetFilterType(CSDEMUX_HANDLE handle, CSDEMUX_FILTER_TYPE type);
CSDEMUX_RESULT CSDEMUX_Filter_SetFilter(CSDEMUX_HANDLE handle, const unsigned char *const filter_ptr, const unsigned char *const mask_ptr);
CSDEMUX_RESULT CSDEMUX_Filter_AddPID(CSDEMUX_HANDLE handle, unsigned short pid);
CSDEMUX_RESULT CSDEMUX_Filter_ResetPID(CSDEMUX_HANDLE handle);
CSDEMUX_RESULT CSDEMUX_Filter_ReadWait(CSDEMUX_HANDLE handle, unsigned int ms_timeout);
CSDEMUX_RESULT CSDEMUX_Filter_CheckDataSize(CSDEMUX_HANDLE handle, unsigned int *size);
CSDEMUX_RESULT CSDEMUX_Filter_GetSectionCount(CSDEMUX_HANDLE handle, unsigned int *count);
CSDEMUX_RESULT CSDEMUX_Filter_ReadSectionData(CSDEMUX_HANDLE handle, unsigned char *buf, unsigned int *size);
CSDEMUX_RESULT CSDEMUX_Filter_Enable(CSDEMUX_HANDLE handle);
CSDEMUX_RESULT CSDEMUX_Filter_Disable(CSDEMUX_HANDLE handle);

#endif /*CONFIG_CELESTIAL*/


#ifdef __cplusplus
}
#endif


#endif
