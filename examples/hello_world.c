/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <exports.h>
#include <orion_tuner.h>

#if 1
typedef struct CS_Service_t_
{
    unsigned int servce_id;
    unsigned int servce_pmtpid;
    unsigned int servce_vidpid;
    unsigned int servce_audpid;
    unsigned int pmt_complete;
    CSDEMUX_HANDLE filter_handle;
    CSDEMUX_HANDLE pidfilter_handle;
    unsigned int vid_type;
    unsigned int aud_type;
    char providername[20];
    char servicename[20];
} CS_Service_t;

typedef struct CS_Test_Section_t_
{
    unsigned int servce_id;
    unsigned int servce_pmtpid;
    unsigned int servce_vidpid;
    unsigned int servce_audpid;
    unsigned int pmt_complete;
    CSDEMUX_HANDLE filter_handle;
    CSDEMUX_HANDLE pidfilter_handle;
    int testfilter_index;
} CS_Test_Section_t;

#define test_pid unsigned short  

#define PAT_PID 0x00
#define PAT_TAB_ID 0x00
#define PMT_TAB_ID 0x02
#define SDT_PID 0x11
#define SDT_TAB_ID 0x42
#define SECTION_LENGTN 1024
#define MAX_SECTION_LENGTN 1024
#define CURRENT_SERVICE_NUM 16
#define  MAX_SERVICE_NUM 32

#define MAX_SECTION_FILTER_CHAR    4
#define MAX_SECTION_SIZE           4284
#define TS_SIZE                    188

unsigned char section_buf[SECTION_LENGTN];
unsigned int service_num=0;
unsigned int cur_pmt_parser_index=0;
unsigned int sdtcomplete=0;
unsigned char current_section_num=0,last_section_num=0; 
CS_Service_t service[MAX_SERVICE_NUM];
CS_Test_Section_t testfilter[MAX_SERVICE_NUM];
static unsigned char   mFilter[MAX_SECTION_FILTER_CHAR];
static unsigned char   mMask[MAX_SECTION_FILTER_CHAR];
static unsigned char   mpBuff[MAX_SECTION_SIZE];
static unsigned int    mPID;
static unsigned char   mTABID;
static unsigned int    mPos;
static unsigned int    mIsChecked;
static unsigned char   mNextCounter;

static void Pat_table_parse(unsigned char* ptr,unsigned int len)
{
	int serverid;
	int pmtpid;
	int sectionlen;

	if(len<12||(ptr[1]&0x80)==0){
                printf("exit 1\n");
                return;
        }
	if(!(ptr[5]&0x1)){ 
                printf("exit 2\n");
                return;
       }
	sectionlen=((ptr[1]&0xf)<<8|ptr[2]);

	ptr+=8;
	int i=sectionlen-9;
	while(i>=4)
	{
		serverid=(ptr[0]<<8)|ptr[1];
		pmtpid=((ptr[2]&0x1f)<<8)|ptr[3];
		printf("server id = 0x%x, pmt pid = 0x%x\n",serverid,pmtpid);
                if(service_num<MAX_SERVICE_NUM&&serverid!=0)
                {
                   service[service_num].servce_id = serverid;
                   service[service_num].servce_pmtpid = pmtpid;
                   service[service_num].servce_vidpid = 0x1fff;
                   service[service_num].servce_audpid = 0x1fff;
                   service[service_num].pmt_complete = 0;
                   service_num++;
                }
		ptr+=4;
		i-=4;
	}
}

static void  Pmt_table_parse(unsigned char* ptr,unsigned int len)
{
    int l;
    int tmp;
    int vpid = 0x1fff;
    int apid = 0x1fff;
    int audionum=0;

    if(len<12||(ptr[1]&0x80)==0)  return;
    if(!(ptr[5]&0x1)) return;

    tmp=(((ptr[10]&0xf)<<8)|ptr[11])+12;
    l=len-tmp-4;
    ptr+=tmp;

    while(l>=5)
    {
  
        if(ptr[0]==0x01||ptr[0]==0x02||ptr[0]==0x1b){
            vpid=((ptr[1]&0x1f)<<8)|ptr[2];
            service[cur_pmt_parser_index].servce_vidpid = vpid;
            if(ptr[0] == 0x1b) service[cur_pmt_parser_index].vid_type = 0;
            else service[cur_pmt_parser_index].vid_type = 1;
        }
        else if((ptr[0]==0x03||ptr[0]==0x04 ||ptr[0]==0x81 ||ptr[0]==0x6a ||ptr[0]==0x06)&&(audionum == 0)){
            apid=((ptr[1]&0x1f)<<8)|ptr[2];
            service[cur_pmt_parser_index].servce_audpid = apid;
            if(ptr[0] == 0x6a || ptr[0]==0x81 || ptr[0]==0x06) service[cur_pmt_parser_index].aud_type = 0;
            else service[cur_pmt_parser_index].aud_type = 1;
            audionum++;
        }
        else{  
            printf("=====>invalid type:%d\n",ptr[0]);
        }
        tmp=(((ptr[3]&0xf)<<8)|ptr[4])+5;
        l-=tmp;
        ptr+=tmp;
    }
    printf("video pid = 0x%08x, audio pid = 0x%08x\n",vpid,apid);
}

static void Sdt_table_parse(unsigned char* ptr,unsigned int len)
{
    int l,tmp,i,descriptorlooplen,descriptorlen,namelen,templen;
    unsigned short serviceid;
    char* aaa=NULL;
    unsigned int aaaaaa = 0;
	
    printf("start parse sdt!\n");
    if(len<12||(ptr[1]&0x80)==0)  return;
    if(!(ptr[5]&0x1)) return;

    tmp=((ptr[1]<<8)&0xf00)|ptr[2];
   // printf("tmp=%d\n",tmp);

    current_section_num=ptr[6];
    last_section_num=ptr[7];
    if(current_section_num == last_section_num) sdtcomplete=1;
    else sdtcomplete=0;

    printf("current_section_num %d, last_section_num %d\n",current_section_num,last_section_num);
    l=tmp-4-8;
    ptr+=11;
    
    while(l)
    {
        //getchar();
        serviceid=ptr[0]<<8 |ptr[1];
        templen=descriptorlooplen=((ptr[3]<<8)&0xf00)|ptr[4];
        printf("serviceid %d,descriptorlooplen %d\n",serviceid,descriptorlooplen);
        aaa=ptr;
        for(i=0;i<service_num;i++){
            if(service[i].servce_id == serviceid){
                aaa+=5;
                descriptorlen=aaa[1];
                if(aaa[0] == 0x48){
                    aaa+=3;
                    namelen=aaa[0];
                    printf("provider namelen =%d \n",namelen);
                    aaa++;
                    if(namelen !=0){
                           // memset((char*)service[i].providername,0,20);
                           for(aaaaaa= 0; aaaaaa< 20; aaaaaa++){
					service[i].providername[aaaaaa] = 0;
				}
				for(aaaaaa= 0; aaaaaa< namelen; aaaaaa++){
					service[i].providername[aaaaaa] = *(aaa+aaaaaa);
				}
                            //memcpy((char*)service[i].providername,(char*)aaa,namelen);
                            printf("provider name %s \n",(char*)service[i].providername);
                            aaa+=namelen;
                    }
                    namelen=aaa[0];
                    printf("service namelen =%d \n",namelen);
                    aaa++;
                    if(namelen !=0){
                            //memset((char*)service[i].servicename,0,20);
				for(aaaaaa= 0; aaaaaa< 20; aaaaaa++){
					service[i].servicename[aaaaaa] = 0;
				}
				for(aaaaaa= 0; aaaaaa< namelen; aaaaaa++){
					service[i].servicename[aaaaaa] = *(aaa+aaaaaa);
				}
				//memcpy((char*)service[i].servicename,(char*)aaa,namelen);
                            printf("service name %s \n",(char*)service[i].servicename);
                            aaa+=namelen;
                    }
                    //templen=templen-descriptorlen-2;
                }
                else{
                    printf("invalid descriptor 0x%x\n",aaa[0]);
                    aaa=aaa+descriptorlen+2;
                    //descriptorlooplen=descriptorlooplen-descriptorlen-2;
                }
            }
        }
        l=l-templen-5;
        ptr=ptr+templen+5;
    }
}

static int Search_Channel(void)
{
	CSDEMUX_HANDLE hchl; 
    CSDEMUX_HANDLE hfilter;   
    CSDEMUX_HANDLE hpidfilter;   
    unsigned char filter[12];
    unsigned char mask[12];
    unsigned int  data_lengtn = 0;
    int pmt_parsed_num = 0;
	int gogogo= 0;
    unsigned int servce_id = 0;
    unsigned int servce_pmtpid = 0;
    unsigned int servce_vidpid = 0;
    unsigned int servce_audpid = 0;

    service_num = 0;
    cur_pmt_parser_index = 0;

   for(gogogo = 0; gogogo < 12; gogogo++){
	filter[gogogo] = 0;
	mask[gogogo] = 0;
   }

    filter[0] = 0x00;
    mask[0]   = 0xff;

	if(CSDEMUX_Init()){//download firmware and init xport
		printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
		return (-1);
	}

	hchl = CSDEMUX_CHL_Open(DEMUX_CHL_ID0);
	
	if(CSDEMUX_CHL_Disable(hchl)){// disable channel0
		printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
		return (-1);
	}
	if(CSDEMUX_CHL_SetInputMode(hchl, DEMUX_INPUT_MOD_TUNER)){// channel0 sets inputmode  with tuner mode
		printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
		return (-1);
	}
	if(CSDEMUX_CHL_Enable(hchl)){// enable channel0
		printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
		return (-1);
	}

    hfilter = CSDEMUX_Filter_Open(DEMUX_FILTER_ID0);

    hpidfilter = CSDEMUX_PIDFT_Open(DEMUX_PIDFT_ID0);

    if(CSDEMUX_PIDFT_SetChannel(hpidfilter,DEMUX_CHL_ID0)){
	printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
			return DEMUX_FAILED;
	}
    if(CSDEMUX_PIDFT_SetPID(hpidfilter,PAT_PID)){
	printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
			return DEMUX_FAILED;
	}  
    if(CSDEMUX_PIDFT_Enable(hpidfilter)){
	printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
			return DEMUX_FAILED;
	}
    
    if(CSDEMUX_Filter_SetFilter(hfilter,filter,mask)){
	printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
			return DEMUX_FAILED;
	}
    if(CSDEMUX_Filter_AddPID(hfilter,PAT_PID)){
	printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
			return DEMUX_FAILED;
	}
    if(CSDEMUX_Filter_SetFilterType(hfilter,DEMUX_FILTER_TYPE_SEC)){
	printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
			return DEMUX_FAILED;
	}
    if(CSDEMUX_Filter_Enable(hfilter)){
	printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
			return DEMUX_FAILED;
	}
    
    while(1)
    {
       data_lengtn = 1024;
       if(CSDEMUX_Filter_ReadWait(hfilter,500) == DEMUX_SUCCEED)
       {
           if(CSDEMUX_Filter_ReadSectionData(hfilter, section_buf, &data_lengtn)== DEMUX_SUCCEED)
           {
              printf("data size  = %d\n", data_lengtn);
		Pat_table_parse(section_buf,data_lengtn);

		CSDEMUX_Filter_CheckDataSize(hfilter, &data_lengtn);
		printf("data_length = %d\n",data_lengtn);
              CSDEMUX_Filter_GetSectionCount(hfilter, &data_lengtn);
		printf("section count = %d\n",data_lengtn);

		udelay(1000*1000);

		CSDEMUX_Filter_CheckDataSize(hfilter, &data_lengtn);
		printf("data_length = %d\n",data_lengtn);
              CSDEMUX_Filter_GetSectionCount(hfilter, &data_lengtn);
		printf("section count = %d\n",data_lengtn);

		break;
           }
       }
       else printf("PAT---------ReadWait Timeout\n");
    }
    CSDEMUX_PIDFT_Disable(hpidfilter);
    CSDEMUX_PIDFT_Close(hpidfilter);
    CSDEMUX_Filter_Disable(hfilter);
    CSDEMUX_Filter_Close(hfilter);

    printf("PAT complete! Service_num = %d \n",service_num);

    for( cur_pmt_parser_index = 0; cur_pmt_parser_index < service_num; cur_pmt_parser_index++)
    {
        if(service[cur_pmt_parser_index].servce_id==0) continue;

        hfilter = CSDEMUX_Filter_Open(cur_pmt_parser_index);
        hpidfilter = CSDEMUX_PIDFT_Open(cur_pmt_parser_index);
        service[cur_pmt_parser_index].filter_handle = hfilter;
        service[cur_pmt_parser_index].pidfilter_handle = hpidfilter;
        filter[0] = 0x02;
        mask[0]   = 0xff;
        
        CSDEMUX_PIDFT_SetChannel(hpidfilter,DEMUX_CHL_ID0);
        CSDEMUX_PIDFT_SetPID(hpidfilter,service[cur_pmt_parser_index].servce_pmtpid);  
        CSDEMUX_PIDFT_Enable(hpidfilter);

        CSDEMUX_Filter_SetFilter(hfilter,filter,mask);
        CSDEMUX_Filter_AddPID(hfilter,service[cur_pmt_parser_index].servce_pmtpid);
        CSDEMUX_Filter_SetFilterType(hfilter,DEMUX_FILTER_TYPE_SEC);
        CSDEMUX_Filter_Enable(hfilter);
    }
    
    pmt_parsed_num = service_num;
    printf("pmt_parsed_num = %d\n",pmt_parsed_num);
    while(pmt_parsed_num)
    {
        for( cur_pmt_parser_index = 0; cur_pmt_parser_index < service_num; cur_pmt_parser_index++)
        {
            if(service[cur_pmt_parser_index].pmt_complete) continue;
            data_lengtn = 1024;
            hfilter = service[cur_pmt_parser_index].filter_handle;
            if(CSDEMUX_Filter_ReadWait(hfilter,1000) == DEMUX_SUCCEED)
            {
                if(CSDEMUX_Filter_ReadSectionData(hfilter, section_buf, &data_lengtn)==DEMUX_SUCCEED)
                {
                    printf("data size  = %d\n", data_lengtn);
                    Pmt_table_parse(section_buf,data_lengtn);
                    service[cur_pmt_parser_index].pmt_complete = 1;
                    printf("PMT 0x%x parse complete !\n ",service[cur_pmt_parser_index].servce_pmtpid);
                    pmt_parsed_num--;
                    break;
                }
            }
		else printf("PMT---------ReadWait Timeout\n");
        }
    }

    for( cur_pmt_parser_index = 0; cur_pmt_parser_index < service_num; cur_pmt_parser_index++)
    {
        servce_id = service[cur_pmt_parser_index].servce_id;
        servce_pmtpid= service[cur_pmt_parser_index].servce_pmtpid;
        servce_vidpid = service[cur_pmt_parser_index].servce_vidpid;
        servce_audpid = service[cur_pmt_parser_index].servce_audpid;
        CSDEMUX_PIDFT_Disable(service[cur_pmt_parser_index].pidfilter_handle);
        CSDEMUX_PIDFT_Close(service[cur_pmt_parser_index].pidfilter_handle);
        CSDEMUX_Filter_Disable(service[cur_pmt_parser_index].filter_handle);
        CSDEMUX_Filter_Close(service[cur_pmt_parser_index].filter_handle);
        printf("servce_id = 0x%x,  servce_pmtpid = 0x%x,  servce_vidpid = 0x%x, servce_audpid = 0x%x\n",servce_id,servce_pmtpid,servce_vidpid,servce_audpid);
    }
#if 1//request and parse sdt 
    sdtcomplete = 0;
    while(1)
    {
        int timeout_flag=1;
        
        if(sdtcomplete){ 
           printf("SDT complete! Service_num = %d \n",service_num);
           break;
        }

	for(gogogo = 0; gogogo < 12; gogogo++){
		filter[gogogo] = 0;
		mask[gogogo] = 0;
   	}
        filter[0] = 0x42;
        mask[0] = 0xff;
        if(current_section_num){
            filter[6] = current_section_num+1;
            mask[6] = 0xff;
        }
        else{
            filter[6] = 0x00;
            mask[6] = 0xff;
        }
        hfilter = CSDEMUX_Filter_Open(DEMUX_FILTER_ID13);
        hpidfilter = CSDEMUX_PIDFT_Open(DEMUX_PIDFT_ID13);
        CSDEMUX_PIDFT_SetChannel(hpidfilter,DEMUX_CHL_ID0);
        CSDEMUX_PIDFT_SetPID(hpidfilter,SDT_PID);  
        CSDEMUX_PIDFT_Enable(hpidfilter);
        CSDEMUX_Filter_SetFilter(hfilter,filter,mask);
        CSDEMUX_Filter_AddPID(hfilter,SDT_PID);
        CSDEMUX_Filter_SetFilterType(hfilter,DEMUX_FILTER_TYPE_SEC);
        CSDEMUX_Filter_Enable(hfilter);
        data_lengtn = 1024;
        timeout_flag=1;
        while(CSDEMUX_Filter_ReadWait(hfilter,5000) == DEMUX_SUCCEED)
        {
           timeout_flag=0;
           if(CSDEMUX_Filter_ReadSectionData(hfilter, section_buf, &data_lengtn)== DEMUX_SUCCEED)
           {
              printf("data size  = %d\n", data_lengtn);
              Sdt_table_parse(section_buf,data_lengtn);
              break;
           }
        }
        printf("SDT#########################Disable PIDFilter and Filter\n");
        CSDEMUX_PIDFT_Disable(hpidfilter);
        CSDEMUX_PIDFT_Close(hpidfilter);
        CSDEMUX_Filter_Disable(hfilter);
        CSDEMUX_Filter_Close(hfilter);
        if(timeout_flag)
        {
            printf("SDT---------ReadWait Timeout\n");
            //break;
        }
    }
#endif    

	if(CSDEMUX_CHL_Disable(hchl)){
		printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
		return -1;
	}
	if(CSDEMUX_CHL_Close(hchl)){
		printf("error: %s, %d. \n",__FUNCTION__,__LINE__);
		return -1;
	}
}
#endif

int hello_world (int argc, char *argv[])
{
	int i;
	int err_code = 0;
	TUNER_PARAMS_S tuner_param;
	TUNER_STATUS_E tuner_status = 0;
	int icount = 0;

	unsigned char filter[12];
	unsigned char mask[12];
	unsigned int return_len = 0;
	unsigned char buf[4096];

	for(i = 0;i < 12;i ++){
		filter[i] = 0;
		mask[i] = 0;
	}
	filter[0] = 0x00;
	mask[0]   = 0xff;
	
	/* Print the ABI version */
	//app_startup(argv);
	//printf ("Example expects ABI version %d\n", XF_VERSION);
	//printf ("Actual U-Boot ABI version %d\n", (int)get_version());

	printf ("Hello World\n");

/*****************tuner start************************/
#if 1
	err_code = CSTUNER_Open(TUNER_DVBC);
	if(err_code < 0) {
		printf("Error: CSTUNER_Open.\n");
		return err_code;
	}	
	printf("1 success CSTUNER_Open. \n");

	err_code = CSTUNER_Init();
	if(err_code < 0) {
		printf("Error: CSTUNER_Init.\n");
		return err_code;
	}
	printf("1 success CSTUNER_Init. \n");

	tuner_param.frequency = 698;
	tuner_param.symbol_rate = 6875;
	tuner_param.modulation = QAM_64;
	tuner_param.inversion = INVERSION_AUTO;
	err_code = CSTUNER_SetFrequency(&tuner_param);
	if(err_code < 0) {
		printf("Error: CSTUNER_SetFrequency.\n");
		return err_code;
	}

	for(icount = 0; icount < 10000; icount++) {
		tuner_status = 0;
		err_code = CSTUNER_ReadStatus(&tuner_status);
		if(err_code < 0) {
			printf("Error: CSTUNER_ReadStatus.\n");
			return err_code;
		}
		
		if(tuner_status == TUNER_STATUS_FE_LOCKED) {
			printf("tuner_status = %d\n", tuner_status);
			break;
		}
		udelay(2000000);
	}
#endif
/*******************tuner end**********************/

/*******************xport start***********************/
#if 1
	Search_Channel();
#else
	if(xport_init())//download firmware and init xport
		return (-1);
	
	if(xport_set_channel_status(0, 0))// disable channel0
		return (-1);
	
	if(xport_set_channel_inputmode(0, 0))// channel0 sets inputmode  with tuner mode
		return (-1);

	if(xport_set_channel_status(0, 1))// enable channel0
		return (-1);

	xport_pidft_bind_chl(0, 0);// pidft1 binds to channel0
	xport_pidft_set_pid(0, 0x0);// pidft1 filters 0x0"PAT"
	xport_pidft_set_status(0, 1);// enable pidtf
	
	xport_filter_set_filter(0, filter, mask);// filter0 set filter condition
	xport_filter_set_pid(0, 0x0);// filter0 filters 0x0"PAT"
	xport_filter_set_type(0, 0);// filter0 output section data
	xport_filter_set_status(0, 1);// enable filter0

	while(1){
		if(xport_filter_wait_data(0, 5*1000) == 0){// filter0 max timeout value is 5s 
			return_len = xport_fliter_read_data(0, buf, 4096);// read section data from filter0
			printf("read length %d\n",return_len);
			printf("filter0:section  = 0x%02x, 0x%02x, 0x%02x,  0x%02x, 0x%02x, 0x%02x,0x%02x, 0x%02x, 0x%02x\n", buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8]);
		}
		else{
			printf("read wait\n");// timeout wait
		}
	}
#endif
	return (0);
/*******************xport end***********************/
	printf("can't go to there!\n");

	printf ("argc = %d\n", argc);

	for (i=0; i<=argc; ++i) {
		printf ("argv[%d] = \"%s\"\n",
			i,
			argv[i] ? argv[i] : "<NULL>");
	}

	printf ("Hit any key to exit ... ");
	while (!tstc())
		;
	/* consume input */
	(void) getc();

	printf ("\n\n");
	return (0);
}
