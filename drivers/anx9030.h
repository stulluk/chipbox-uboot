/* Analogix 9030 defines */

#define ANX9030_ADDR1	0x39  //ANX9030, 0x72
#define ANX9030_ADDR2	0x3D  //ANX9030, 0x7A

/*DDC status bit*/
#define DDC_Error_bit   0x07
#define DDC_Occup_bit  0x06
#define DDC_Fifo_Full_bit  0x05
#define DDC_Fifo_Empt_bit  0x04
#define DDC_No_Ack_bit 0x03
#define DDC_Fifo_Rd_bit    0x02
#define DDC_Fifo_Wr_bit    0x01
#define DDC_Progress_bit   0x00

#define YCbCr422 0x20
#define null 0
#define source_ratio 0x08

/*DDC Command*/
#define Abort_Current_Operation 0x00
#define Sequential_Byte_Read 0x01
#define Sequential_Byte_Write 0x02
#define Implicit_Offset_Address_Read 0x3
#define Enhanced_DDC_Sequenital_Read 0x04
#define Clear_DDC_Fifo 0x05
#define I2c_reset 0x06

/*DDC result*/
#define DDC_NO_Err 0x00
#define DDC_Status_Err 0x01
#define DDC_Data_Addr_Err 0x02
#define DDC_Length_Err  0x03

/*checksum result*/
#define Edid_Checksum_No_Err     0x00
#define Edid_Checksum_Err   0x01

/*HDCP device base address*/
#define HDCP_Dev_Addr   0x74

/*HDCP Bksv offset*/
#define HDCP_Bksv_Offset 0x00

/*HDCP Bcaps offset*/
#define HDCP_Bcaps_Offset   0x40

/*HDCP Bstatus offset*/
#define HDCP_Bstatus_offset     0x41

/*HDCP KSV Fifo offset */
#define HDCP_Ksv_Fifo_Offset    0x43

/*HDCP bksv data nums*/
#define Bksv_Data_Nums  5

/*HDCP ksvs data number by defult*/
#define ksvs_data_nums 50

/*DDC Max bytes*/
#define DDC_Max_Length 1024

/*DDC fifo depth*/
#define DDC_Fifo_Depth  16

/*DDC read delay ms*/
#define DDC_Read_Delay 5

/*DDC Write delay ms*/
#define DDC_Write_Delay 40
/*end*/


// extern BIT anx9030_parse_edid_done;
// extern BIT anx9030_system_config_done, anx9030_system_config_retry;
// extern BYTE anx9030_video_format_config,anx9030_video_timing_id;
// extern BIT anx9030_new_csc,anx9030_new_vid_id,anx9030_new_HW_interface;
// extern BIT anx9030_ddr_edge;
// extern BYTE anx9030_in_pix_rpt_bkp,anx9030_tx_pix_rpt_bkp;
// extern BYTE anx9030_in_pix_rpt,anx9030_tx_pix_rpt;
// extern BIT anx9030_pix_rpt_set_by_sys;
// extern BYTE anx9030_RGBorYCbCr;
// extern audio_config_struct s_anx9030_audio_config;
// extern config_packets s_anx9030_packet_config;
// extern BIT anx9030_app_hold_system_config;
// extern BIT anx9030_shutdown;

// extern BYTE timer_slot,misc_reset_needed;
// extern BYTE bist_switch_value_pc,switch_value;
// extern BYTE switch_value_sw_backup,switch_value_pc_backup;
// extern BYTE ANX9030_system_state;
// extern BIT anx9030_srm_checked;
// extern BIT anx9030_HDCP_enable;

// extern BYTE FREQ_MCLK;

//********************** BIST Enable***********************************
//#define BIST_MODE_USED 1





//***********************Video Config***********************************
#define anx9030_RGB_YCrCb444_SepSync 0
#define anx9030_YCrCb422_SepSync 1
#define anx9030_YCrCb422_EmbSync 2
#define anx9030_YCMux422_SepSync_Mode1 3
#define anx9030_YCMux422_SepSync_Mode2 4
#define anx9030_YCMux422_EmbSync_Mode1 5
#define anx9030_YCMux422_EmbSync_Mode2 6
#define anx9030_RGB_YCrCb444_DDR_SepSync 7
#define anx9030_RGB_YCrCb444_DDR_EmbSync 8

#define anx9030_RGB_YCrCb444_SepSync_No_DE 9
#define anx9030_YCrCb422_SepSync_No_DE 10

#define anx9030_Progressive 0
#define anx9030_Interlace 0x08
#define anx9030_Neg_Hsync_pol 0x20
#define anx9030_Pos_Hsync_pol 0
#define anx9030_Neg_Vsync_pol 0x40
#define anx9030_Pos_Vsync_pol 0


#define anx9030_V640x480p_60Hz 1
#define anx9030_V720x480p_60Hz_4x3 2
#define anx9030_V720x480p_60Hz_16x9 3
#define anx9030_V1280x720p_60Hz 4
#define anx9030_V1280x720p_50Hz 19
#define anx9030_V1920x1080i_60Hz 5
#define anx9030_V1920x1080p_60Hz 16
#define anx9030_V1920x1080i_50Hz 20
#define anx9030_V1920x1080p_50Hz 31
#define anx9030_V720x480i_60Hz_4x3 6
#define anx9030_V720x480i_60Hz_16x9 7
#define anx9030_V720x576i_50Hz_4x3 21
#define anx9030_V720x576i_50Hz_16x9 22
#define anx9030_V720x576p_50Hz_4x3 17
#define anx9030_V720x576p_50Hz_16x9 18
#define anx9030_V1920x1080p_24Hz 32
#define anx9030_V1920x1080p_25Hz 33
#define anx9030_V1920x1080p_30Hz 34


#define ANX9030_RGB 0x00
#define ANX9030_YCbCr422 0x01
#define ANX9030_YCbCr444 0x02   //modified by zy 060814
#define ANX9030_CSC_BT709 1
#define ANX9030_CSC_BT601 0

#define ANX9030_EMBEDED_BLUE_SCREEN_ENABLE 1
#define ANX9030_HDCP_FAIL_THRESHOLD 10

#define ANX9030_avi_sel 0x01
#define ANX9030_audio_sel 0x02
#define ANX9030_spd_sel 0x04
#define ANX9030_mpeg_sel 0x08
#define ANX9030_acp_sel 0x10
#define ANX9030_isrc1_sel 0x20
#define ANX9030_isrc2_sel 0x40
#define ANX9030_vendor_sel 0x80

#define sim
// audio type
#define ANX9030_i2s_input 0x01
#define ANX9030_spdif_input 0x02
#define ANX9030_super_audio_input 0x04
// freq_mclk
#define ANX9030_mclk_128_Fs 0x00
#define ANX9030_mclk_256_Fs 0x01
#define ANX9030_mclk_384_Fs 0x02
#define ANX9030_mclk_512_Fs 0x03
// thresholds
#define ANX9030_spdif_stable_th 0x03
// fs -> N(ACR)
#define ANX9030_N_32k 0x1000
#define ANX9030_N_44k 0x1880
#define ANX9030_N_88k 0x3100
#define ANX9030_N_176k 0x6200
#define ANX9030_N_48k 0x1800
#define ANX9030_N_96k 0x3000
#define ANX9030_N_192k 0x6000

#define ANX9030_INITIAL 0x01
#define ANX9030_WAIT_HOTPLUG 0x02
#define ANX9030_READ_PARSE_EDID 0x03
#define ANX9030_CONFIG_VIDEO 0x04
#define ANX9030_CONFIG_AUDIO 0x05
#define ANX9030_CONFIG_PACKETS 0x06
#define ANX9030_HDCP_AUTHENTICATION 0x07
#define ANX9030_PLAY_BACK 0x08
#define ANX9030_RESET_LINK 0x09
#define ANX9030_WAIT_RX_SENSE 0x0a
#define ANX9030_UNKNOWN 0x0b

#define spdif_error_th 0x0a

#define Hresolution_1920 1920
#define Vresolution_540 540
#define Vresolution_1080 1080
#define Hresolution_1280 1280
#define Vresolution_720 720
#define Hresolution_640 640
#define Vresolution_480 480
#define Hresolution_720 720
#define Vresolution_240 240
#define Vresolution_576 576
#define Vresolution_288 288
#define Hz_50 50
#define Hz_60 60
#define Interlace_EDID 0
#define Progressive_EDID 1
#define ratio_16_9 1.777778
#define ratio_4_3 1.333333

#define ANX9030_EDID_BadHeader 0x01;
#define ANX9030_EDID_861B_not_supported 0x02;
#define ANX9030_EDID_CheckSum_ERR 0x03;
#define ANX9030_EDID_No_ExtBlock 0x04;
#define ANX9030_EDID_ExtBlock_NotFor_861B 0x05;

#define ANX9030_VND_IDL_REG 0x00
#define ANX9030_VND_IDH_REG 0x01
#define ANX9030_DEV_IDL_REG 0x02
#define ANX9030_DEV_IDH_REG 0x03
#define ANX9030_DEV_REV_REG 0x04

#define ANX9030_SRST_REG 0x05
#define ANX9030_TX_RST 0x40
#define ANX9030_SRST_VIDCAP_RST	        0x20	// bit position
#define ANX9030_SRST_AFIFO_RST	       	 0x10	// bit position
#define ANX9030_SRST_HDCP_RST		        0x08	// bit position
#define ANX9030_SRST_VID_FIFO_RST		 0x04	// bit position
#define ANX9030_SRST_AUD_RST		 0x02	// bit position
#define ANX9030_SRST_SW_RST			 0x01	// bit position

#define ANX9030_SYS_STATE_REG 0x06
#define ANX9030_SYS_STATE_AUD_CLK_DET	        0x20	// bit position
#define ANX9030_SYS_STATE_AVMUTE	       	 0x10	// bit position
#define ANX9030_SYS_STATE_HP		       	 0x08	// bit position
#define ANX9030_SYS_STATE_VSYNC		 		 0x04	// bit position
#define ANX9030_SYS_STATE_CLK_DET		 	 0x02	// bit position
#define ANX9030_SYS_STATE_RSV_DET			 0x01	// bit position

#define ANX9030_SYS_CTRL1_REG 0x07
#define ANX9030_SYS_CTRL1_LINKMUTE_EN	        0x80	// bit position
#define ANX9030_SYS_CTRL1_HDCPHPD_RST		 0x40	// bit position
#define ANX9030_SYS_CTRL1_PDINT_SEL		 0x20	// bit position
#define ANX9030_SYS_CTRL1_DDC_FAST	        	 0x10	// bit position
#define ANX9030_SYS_CTRL1_DDC_SWCTRL	        0x08	// bit position
#define ANX9030_SYS_CTRL1_HDCPMODE		 0x04	// bit position
#define ANX9030_SYS_CTRL1_HDMI				 0x02	// bit position
#define ANX9030_SYS_CTRL1_PWDN_CTRL	        0x01	// bit position

#define ANX9030_SYS_CTRL2_REG 0x08
#define ANX9030_SYS_CTRL2_DDC_RST	      		  0x08	// bit position
#define ANX9030_SYS_CTRL2_TMDSBIST_RST	  0x04	// bit position
#define ANX9030_SYS_CTRL2_MISC_RST		 	  0x02	// bit position
#define ANX9030_SYS_CTRL2_HW_RST	     		  0x01	// bit position

#define ANX9030_SYS_CTRL3_REG 0x09
#define ANX9030_SYS_CTRL3_I2C_PWON 0x02
#define ANX9030_SYS_CTRL3_PWON_ALL 0x01

#define ANX9030_VID_STATUS_REG 0x10
#define ANX9030_VID_STATUS_VID_STABLE		 0x20	// bit position
#define ANX9030_VID_STATUS_EMSYNC_ERR	        0x10	// bit position
#define ANX9030_VID_STATUS_FLD_POL	    		 0x08	// bit position
#define ANX9030_VID_STATUS_TYPE		 	 0x04	// bit position
#define ANX9030_VID_STATUS_VSYNC_POL		 0x02	// bit position
#define ANX9030_VID_STATUS_HSYNC_POL	        0x01	// bit position

#define ANX9030_VID_MODE_REG 0x11
#define ANX9030_VID_MODE_CHKSHARED_EN	 0x80	// bit position
#define ANX9030_VID_MODE_LINKVID_EN		 0x40	// bit position
#define ANX9030_VID_MODE_RANGE_Y2R		 0x20	// bit position
#define ANX9030_VID_MODE_CSPACE_Y2R	        0x10	// bit position
#define ANX9030_VID_MODE_Y2R_SEL	        	 0x08	// bit position
#define ANX9030_VID_MODE_UPSAMPLE			 0x04	// bit position

#define ANX9030_VID_CTRL_REG  0x12
#define ANX9030_VID_CTRL_IN_EN	    		 0x10	// bit position
#define ANX9030_VID_CTRL_YCBIT_SEL        	 0x08	// bit position
#define ANX9030_VID_CTRL_BITCTRL_EN	 		0x04	// bit position

#define ANX9030_VID_CAPCTRL0_REG  0x13
#define ANX9030_VID_CAPCTRL0_DEGEN_EN	 	 0x80	// bit position
#define ANX9030_VID_CAPCTRL0_EMSYNC_EN	 0x40	// bit position
#define ANX9030_VID_CAPCTRL0_DEMUX_EN		 0x20	// bit position
#define ANX9030_VID_CAPCTRL0_INV_IDCK	        0x10	// bit position
#define ANX9030_VID_CAPCTRL0_DV_BUSMODE	 0x08	// bit position
#define ANX9030_VID_CAPCTRL0_DDR_EDGE		 0x04	// bit position
#define ANX9030_VID_CAPCTRL0_VIDBIT_SWAP	 0x02	// bit position
#define ANX9030_VID_CAPCTRL0_VIDBIST_EN	 0x01	// bit position
	
#define ANX9030_VID_CAPCTRL1_REG 0x14
#define ANX9030_VID_CAPCTRL1_FORMAT_SEL	 	 0x80	// bit position
#define ANX9030_VID_CAPCTRL1_VSYNC_POL	   	 0x40	// bit position
#define ANX9030_VID_CAPCTRL1_HSYNC_POL		 0x20	// bit position
#define ANX9030_VID_CAPCTRL1_INV_FLDPOL	        0x10	// bit position
#define ANX9030_VID_CAPCTRL1_VID_TYPE	 		0x08	// bit position

#define ANX9030_H_RESL_REG 0x15
#define ANX9030_H_RESH_REG 0x16
#define ANX9030_VID_PIXL_REG 0x17
#define ANX9030_VID_PIXH_REG 0x18
#define ANX9030_H_FRONTPORCHL_REG 0x19
#define ANX9030_H_FRONTPORCHH_REG 0x1A
#define ANX9030_HSYNC_ACT_WIDTHL_REG 0x1B
#define ANX9030_HSYNC_ACT_WIDTHH_REG 0x1C
#define ANX9030_H_BACKPORCHL_REG 0x1D
#define ANX9030_H_BACKPORCHH_REG 0x1E
#define ANX9030_V_RESL_REG 0x1F
#define ANX9030_V_RESH_REG 0x20
#define ANX9030_ACT_LINEL_REG 0x21
#define ANX9030_ACT_LINEH_REG 0x22
#define ANX9030_ACT_LINE2VSYNC_REG 0x23
#define ANX9030_VSYNC_WID_REG 0x24
#define ANX9030_VSYNC_TAIL2VIDLINE_REG 0x25
#define ANX9030_VIDF_HRESL_REG 0x26
#define ANX9030_VIDF_HRESH_REG 0x27
#define ANX9030_VIDF_PIXL_REG 0x28
#define ANX9030_VIDF_PIXH_REG 0x29
#define ANX9030_VIDF_HFORNTPORCHL_REG 0x2A
#define ANX9030_VIDF_HFORNTPORCHH_REG 0x2B
#define ANX9030_VIDF_HSYNCWIDL_REG 0x2C
#define ANX9030_VIDF_HSYNCWIDH_REG 0x2D
#define ANX9030_VIDF_HBACKPORCHL_REG 0x2E
#define ANX9030_VIDF_HBACKPORCHH_REG 0x2F
#define ANX9030_VIDF_VRESL_REG 0x30
#define ANX9030_VIDF_VRESH_REG 0x31
#define ANX9030_VIDF_ACTVIDLINEL_REG 0x32
#define ANX9030_VIDF_ACTVIDLINEH_REG 0x33
#define ANX9030_VIDF_ACTLINE2VSYNC_REG 0x34
#define ANX9030_VIDF_VSYNCWIDLINE_REG 0x35
#define ANX9030_VIDF_VSYNCTAIL2VIDLINE_REG 0x36

//Video input data bit control registers    

#define VID_BIT_CTRL0 0x37      //added
#define VID_BIT_CTRL1 0x38 
#define VID_BIT_CTRL2 0x39
#define VID_BIT_CTRL3 0x3A
#define VID_BIT_CTRL4 0x3B
#define VID_BIT_CTRL5 0x3C
#define VID_BIT_CTRL6 0x3D
#define VID_BIT_CTRL7 0x3E
#define VID_BIT_CTRL8 0x3F
#define VID_BIT_CTRL9 0x48
#define VID_BIT_CTRL10 0x49
#define VID_BIT_CTRL11 0x4A
#define VID_BIT_CTRL12 0x4B
#define VID_BIT_CTRL13 0x4C
#define VID_BIT_CTRL14 0x4D
#define VID_BIT_CTRL15 0x4E
#define VID_BIT_CTRL16 0x4F
#define VID_BIT_CTRL17 0x89
#define VID_BIT_CTRL18 0x8A
#define VID_BIT_CTRL19 0x8B
#define VID_BIT_CTRL20 0x8C


#define ANX9030_INTR_STATE_REG 0x40

#define ANX9030_INTR_CTRL_REG 0x41

#define ANX9030_INTR_CTRL_SOFT_INTR	 0x04	// bit position
#define ANX9030_INTR_CTRL_TYPE			 0x02	// bit position
#define ANX9030_INTR_CTRL_POL	 		 0x01	// bit position

#define ANX9030_INTR1_STATUS_REG 0x42
#define ANX9030_INTR1_STATUS_CTS_CHG 	 	 0x80	// bit position
#define ANX9030_INTR1_STATUS_AFIFO_UNDER	 0x40	// bit position
#define ANX9030_INTR1_STATUS_AFIFO_OVER	 0x20	// bit position
#define ANX9030_INTR1_STATUS_SPDIF_ERR	        0x10	// bit position
#define ANX9030_INTR1_STATUS_SW_INT	 	0x08	// bit position
#define ANX9030_INTR1_STATUS_HP_CHG		 0x04	// bit position
#define ANX9030_INTR1_STATUS_CTS_OVRWR	 	0x02	// bit position
#define ANX9030_INTR1_STATUS_CLK_CHG		 0x01	// bit position

#define ANX9030_INTR2_STATUS_REG 0x43
#define ANX9030_INTR2_STATUS_ENCEN_CHG 	 	0x80	// bit position
#define ANX9030_INTR2_STATUS_HDCPLINK_CHK	 	0x40	// bit position
#define ANX9030_INTR2_STATUS_HDCPENHC_CHK 	0x20	// bit position
#define ANX9030_INTR2_STATUS_BKSV_RDY		        0x10	// bit position
#define ANX9030_INTR2_STATUS_PLLLOCK_CHG	 	0x08	// bit position
#define ANX9030_INTR2_STATUS_SHA_DONE			 0x04	// bit position
#define ANX9030_INTR2_STATUS_AUTH_CHG	 		0x02	// bit position
#define ANX9030_INTR2_STATUS_AUTH_DONE		 0x01	// bit position

#define ANX9030_INTR3_STATUS_REG 0x44
#define ANX9030_INTR3_STATUS_SPDIFBI_ERR 	 	0x80	// bit position
#define ANX9030_INTR3_STATUS_VIDF_CHG	 		0x40	// bit position
#define ANX9030_INTR3_STATUS_AUDCLK_CHG 		0x20	// bit position
#define ANX9030_INTR3_STATUS_DDCACC_ERR	        0x10	// bit position
#define ANX9030_INTR3_STATUS_DDC_NOACK	 	0x08	// bit position
#define ANX9030_INTR3_STATUS_VSYNC_DET		 0x04	// bit position
#define ANX9030_INTR3_STATUS_RXSEN_CHG		0x02	// bit position
#define ANX9030_INTR3_STATUS_SPDIF_UNSTBL		 0x01	// bit position

#define ANX9030_INTR1_MASK_REG 0x45
#define ANX9030_INTR2_MASK_REG 0x46
#define ANX9030_INTR3_MASK_REG 0x47

#define ANX9030_HDMI_AUDCTRL0_REG 0x50
#define ANX9030_HDMI_AUDCTRL0_LAYOUT 	 	0x80	// bit position
#define ANX9030_HDMI_AUDCTRL0_DOWN_SMPL 	0x60	// bit position
#define ANX9030_HDMI_AUDCTRL0_CTSGEN_SC 	 	0x10	// bit position
#define ANX9030_HDMI_AUDCTRL0_INV_AUDCLK 	 	0x08	// bit position

#define ANX9030_HDMI_AUDCTRL1_REG 0x51
#define ANX9030_HDMI_AUDCTRL1_IN_EN 	 		0x80	// bit position
#define ANX9030_HDMI_AUDCTRL1_SPDIFIN_EN	 	0x40	// bit position
#define ANX9030_HDMI_AUDCTRL1_SD3IN_EN		0x20	// bit position
#define ANX9030_HDMI_AUDCTRL1_SD2IN_EN	        0x10	// bit position
#define ANX9030_HDMI_AUDCTRL1_SD1IN_EN	 	0x08	// bit position
#define ANX9030_HDMI_AUDCTRL1_SD0IN_EN		 0x04	// bit position
#define ANX9030_HDMI_AUDCTRL1_SPDIFFS_OVRWR	0x02	// bit position
#define ANX9030_HDMI_AUDCTRL1_CLK_SEL		 0x01	// bit position

#define ANX9030_I2S_CTRL_REG 0x52
#define ANX9030_I2S_CTRL_VUCP 	 		0x80	// bit position
#define ANX9030_I2S_CTRL_SHIFT_CTRL	 	0x08	// bit position
#define ANX9030_I2S_CTRL_DIR_CTRL		 0x04	// bit position
#define ANX9030_I2S_CTRL_WS_POL		0x02	// bit position
#define ANX9030_I2S_CTRL_JUST_CTRL		 0x01	// bit position

#define ANX9030_I2SCH_CTRL_REG 0x53
#define ANX9030_I2SCH_FIFO3_SEL	 	0xC0	// bit position
#define ANX9030_I2SCH_FIFO2_SEL	 0x30	// bit position
#define ANX9030_I2SCH_FIFO1_SEL	 0x0C	// bit position
#define ANX9030_I2SCH_FIFO0_SEL	 0x03	// bit position

#define ANX9030_I2SCH_SWCTRL_REG 0x54

#define ANX9030_I2SCH_SWCTRL_SW3 	 		0x80	// bit position
#define ANX9030_I2SCH_SWCTRL_SW2	 	0x40	// bit position
#define ANX9030_I2SCH_SWCTRL_SW1		0x20	// bit position
#define ANX9030_I2SCH_SWCTRL_SW0	        0x10	// bit position
#define ANX9030_I2SCH_SWCTRL_INWD_LEN		0xE0	// bit position
#define ANX9030_I2SCH_SWCTRL_INWD_MAX		 0x01	// bit position

#define ANX9030_SPDIFCH_STATUS_REG 0x55
#define ANX9030_SPDIFCH_STATUS_FS_FREG	0xF0	// bit position
#define ANX9030_SPDIFCH_STATUS_WD_LEN 0x0E	// bit position
#define ANX9030_SPDIFCH_STATUS_WD_MX 0x01	// bit position

#define ANX9030_I2SCH_STATUS1_REG 0x56
#define ANX9030_I2SCH_STATUS1_MODE	 0xC0	// bit position
#define ANX9030_I2SCH_STATUS1_PCM_MODE	 0x38	// bit position
#define ANX9030_I2SCH_STATUS1_SW_CPRGT	 0x04	// bit position
#define ANX9030_I2SCH_STATUS1_NON_PCM	0x02	// bit position
#define ANX9030_I2SCH_STATUS1_PROF_APP	 0x01	// bit position

#define ANX9030_I2SCH_STATUS2_REG 0x57

#define ANX9030_I2SCH_STATUS3_REG 0x58
#define ANX9030_I2SCH_STATUS3_CH_NUM	0xF0	// bit position
#define ANX9030_I2SCH_STATUS3_SRC_NUM	0x0F	// bit position



#define ANX9030_I2SCH_STATUS4_REG 0x59

#define ANX9030_I2SCH_STATUS5_REG 0x5A

#define ANX9030_I2SCH_STATUS5_WORD_MAX 0x01	// bit position

#define ANX9030_HDMI_AUDSTATUS_REG 0x5B

#define ANX9030_HDMI_AUDSTATUS_SPDIF_DET 0x01	// bit position

#define ANX9030_HDMI_AUDBIST_CTRL_REG 0x5C

#define ANX9030_HDMI_AUDBIST_EN3	 	0x08	// bit position
#define ANX9030_HDMI_AUDBIST_EN2		 0x04	// bit position
#define ANX9030_HDMI_AUDBIST_EN1		0x02	// bit position
#define ANX9030_HDMI_AUDBIST_EN0		 0x01	// bit position

#define ANX9030_AUD_INCLK_CNT_REG 0x5D
#define ANX9030_AUD_DEBUG_STATUS_REG 0x5E

#define ANX9030_ONEBIT_AUD_CTRL_REG 0x60

#define ANX9030_ONEBIT_AUD_CTRL_SEN7 	 	0x80	// bit position
#define ANX9030_ONEBIT_AUD_CTRL_SEN6	 	0x40	// bit position
#define ANX9030_ONEBIT_AUD_CTRL_SEN5		0x20	// bit position
#define ANX9030_ONEBIT_AUD_CTRL_SEN4	    0x10	// bit position
#define ANX9030_ONEBIT_AUD_CTRL_SEN3	 	0x08	// bit position
#define ANX9030_ONEBIT_AUD_CTRL_SEN2		0x04	// bit position
#define ANX9030_ONEBIT_AUD_CTRL_SEN1		0x02	// bit position
#define ANX9030_ONEBIT_AUD_CTRL_SEN0		0x01	// bit position

#define ANX9030_ONEBIT_AUD0_CTRL_REG 0x61
#define ANX9030_ONEBIT_AUD1_CTRL_REG 0x62
#define ANX9030_ONEBIT_AUD2_CTRL_REG 0x63
#define ANX9030_ONEBIT_AUD3_CTRL_REG 0x64

#define ANX9030_ONEBIT_AUDCLK_CTRL_REG 0x65

#define ANX9030_ONEBIT_AUDCLK_DET 	0x08	// bit position

#define ANX9030_SPDIF_ERR_THRSHLD_REG 0x66
#define ANX9030_SPDIF_ERR_CNT_REG 0x67

#define ANX9030_HDMI_LINK_CTRL_REG 0x70

#define ANX9030_HDMI_LINK_DATA_MUTEEN1 	 	0x80	// bit position
#define ANX9030_HDMI_LINK_DATA_MUTEEN0	 	0x40	// bit position
#define ANX9030_HDMI_LINK_CLK_MUTEEN2		0x20	// bit position
#define ANX9030_HDMI_LINK_CLK_MUTEEN1	    0x10	// bit position
#define ANX9030_HDMI_LINK_CLK_MUTEEN0	 	0x08	// bit position
#define ANX9030_HDMI_LINK_DEC_DE			0x04	// bit position
#define ANX9030_HDMI_LINK_PRMB_INC			0x02	// bit position
#define ANX9030_HDMI_LINK_AUTO_PROG			0x01	// bit position

#define ANX9030_VID_CAPCTRL2_REG  0x71

#define ANX9030_VID_CAPCTRL2_CHK_UPDATEEN    0x10	// bit position

#define ANX9030_LINK_MUTEEE_REG 0x72

#define ANX9030_LINK_MUTEEE_AVMUTE_EN2		0x20	// bit position
#define ANX9030_LINK_MUTEEE_AVMUTE_EN1	    0x10	// bit position
#define ANX9030_LINK_MUTEEE_AVMUTE_EN0	 	0x08	// bit position
#define ANX9030_LINK_MUTEEE_AUDMUTE_EN2		0x04	// bit position
#define ANX9030_LINK_MUTEEE_AUDMUTE_EN1		0x02	// bit position
#define ANX9030_LINK_MUTEEE_AUDMUTE_EN0		0x01	// bit position

#define ANX9030_SERDES_TEST0_REG 0x73
#define ANX9030_SERDES_TEST1_REG 0x74
#define ANX9030_SERDES_TEST2_REG 0x75
#define ANX9030_DDC_SLV_ADDR_REG 0x80
#define ANX9030_DDC_SLV_SEGADDR_REG 0x81
#define ANX9030_DDC_SLV_OFFADDR_REG 0x82
#define ANX9030_DDC_ACC_CMD_REG 0x83
#define ANX9030_DDC_ACCNUM0_REG 0x84
#define ANX9030_DDC_ACCNUM1_REG 0x85

#define ANX9030_DDC_CHSTATUS_REG 0x86

#define ANX9030_DDC_CHSTATUS_DDCERR	 	0x80	// bit position
#define ANX9030_DDC_CHSTATUS_DDC_OCCUPY	 	0x40	// bit position
#define ANX9030_DDC_CHSTATUS_FIFO_FULL		0x20	// bit position
#define ANX9030_DDC_CHSTATUS_FIFO_EMPT	    0x10	// bit position
#define ANX9030_DDC_CHSTATUS_NOACK	 	0x08	// bit position
#define ANX9030_DDC_CHSTATUS_FIFO_RD			0x04	// bit position
#define ANX9030_DDC_CHSTATUS_FIFO_WR			0x02	// bit position
#define ANX9030_DDC_CHSTATUS_INPRO			0x01	// bit position

#define ANX9030_DDC_FIFO_ACC_REG 0x87
#define ANX9030_DDC_FIFOCNT_REG 0x88

#define ANX9030_SYS_PD_REG 0x90
#define ANX9030_SYS_PD_PLL 	 	0x80	// bit position
#define ANX9030_SYS_PD_TMDS	 	0x40	// bit position
#define ANX9030_SYS_PD_TMDS_CLK		0x20	// bit position
#define ANX9030_SYS_PD_MISC	    0x10	// bit position
#define ANX9030_SYS_PD_LINK	 	0x08	// bit position
#define ANX9030_SYS_PD_IDCK			0x04	// bit position
#define ANX9030_SYS_PD_AUD			0x02	// bit position
#define ANX9030_SYS_PD_MACRO_ALL	0x01	// bit position

#define ANX9030_LINKFSM_DEBUG0_REG 0x91
#define ANX9030_LINKFSM_DEBUG1_REG 0x92

#define ANX9030_PLL_CTRL0_REG 0x93
#define ANX9030_PLL_CTRL0_CPREG_BLEED			0x02	// bit position
#define ANX9030_PLL_CTRL0_TEST_EN	0x01	// bit position

#define ANX9030_PLL_CTRL1_REG 0x94
#define ANX9030_PLL_CTRL1_TESTEN 	 	0x80	// bit position

#define ANX9030_OSC_CTRL_REG 0x95
#define ANX9030_OSC_CTRL_TESTEN	 	0x80	// bit position
#define ANX9030_OSC_CTRL_SEL_BG	 	0x40	// bit position

#define ANX9030_TMDS_CH0_CONFIG_REG 0x96
#define ANX9030_TMDS_CH0_TESTEN		0x20	// bit position
#define ANX9030_TMDS_CH0_AMP		0x1C	// bit position
#define ANX9030_TMDS_CHO_EMP		0x03	// bit position 

#define ANX9030_TMDS_CH1_CONFIG_REG 0x97
#define ANX9030_TMDS_CH1_TESTEN		0x20	// bit position
#define ANX9030_TMDS_CH1_AMP		0x1C	// bit position
#define ANX9030_TMDS_CH1_EMP		0x03	// bit position 

#define ANX9030_TMDS_CH2_CONFIG_REG 0x98
#define ANX9030_TMDS_CH2_TESTEN		0x20	// bit position
#define ANX9030_TMDS_CH2_AMP		0x1C	// bit position
#define ANX9030_TMDS_CH2_EMP		0x03	// bit position 

#define ANX9030_TMDS_CLKCH_CONFIG_REG 0x99
#define ANX9030_TMDS_CLKCH_MUTE	 	0x80	// bit position
#define ANX9030_TMDS_CLKCH_TESTEN	0x08	// bit position
#define ANX9030_TMDS_CLKCH_AMP		0x07	// bit position 

#define ANX9030_CHIP_CTRL_REG 0x9A
#define ANX9030_CHIP_CTRL_PRBS_GENEN 	 	0x80	// bit position
#define ANX9030_CHIP_CTRL_LINK_DBGSEL	 	0x70	// bit position
#define ANX9030_CHIP_CTRL_VIDCHK_EN		 	0x08	// bit position
#define ANX9030_CHIP_CTRL_MISC_TIMER		0x04	// bit position
#define ANX9030_CHIP_CTRL_PLL_RNG		0x02	// bit position
#define ANX9030_CHIP_CTRL_PLL_MAN		0x01	// bit position

#define ANX9030_CHIP_STATUS_REG 0x9B
#define ANX9030_CHIP_STATUS_GPIO	 	0x80	// bit position
#define ANX9030_CHIP_STATUS_SDA	 		0x40	// bit position
#define ANX9030_CHIP_STATUS_SCL			0x20	// bit position
#define ANX9030_CHIP_STATUS_PLL_HSPO	0x04	// bit position
#define ANX9030_CHIP_STATUS_PLL_LOCK	0x02	// bit position
#define ANX9030_CHIP_STATUS_MISC_LOCK	0x01	// bit position

#define ANX9030_DBG_PINGPIO_CTRL_REG  0x9C
#define ANX9030_DBG_PINGPIO_VDLOW_SHAREDEN		0x04	// bit position
#define ANX9030_DBG_PINGPIO_GPIO_ADDREN			0x02	// bit position
#define ANX9030_DBG_PINGPIO_GPIO_OUT			0x01	// bit position

#define ANX9030_CHIP_DEBUG0_CTRL_REG  0x9D
#define ANX9030_CHIP_DEBUG0_PRBS_ERR 0xE0		// bit position
#define ANX9030_CHIP_DEBUG0_CAPST  	 0x1F		// bit position

#define ANX9030_CHIP_DEBUG1_CTRL_REG  0x9E
#define ANX9030_CHIP_DEBUG1_SDA_SW 	 	0x80	// bit position
#define ANX9030_CHIP_DEBUG1_SCL_SW	 	0x40	// bit position
#define ANX9030_CHIP_DEBUG1_SERDES_TESTEN		0x20	// bit position
#define ANX9030_CHIP_DEBUG1_CLK_BYPASS	    0x10	// bit position
#define ANX9030_CHIP_DEBUG1_FORCE_PLLLOCK	 	0x08	// bit position
#define ANX9030_CHIP_DEBUG1_PLLLOCK_BYPASS			0x04	// bit position
#define ANX9030_CHIP_DEBUG1_FORCE_HP			0x02	// bit position
#define ANX9030_CHIP_DEBUG1_HP_DEGLITCH			0x01	// bit position

#define ANX9030_CHIP_DEBUG2_CTRL_REG  0x9F
#define ANX9030_CHIP_DEBUG2_EXEMB_SYNCEN		0x04	// bit position
#define ANX9030_CHIP_DEBUG2_VIDBIST			0x02	// bit position

#define ANX9030_VID_INCLK_REG  0x5F

#define ANX9030_HDCP_STATUS_REG  0xA0
#define ANX9030_HDCP_STATUS_ADV_CIPHER 	 	0x80	// bit position
#define ANX9030_HDCP_STATUS_R0_READY	    0x10	// bit position
#define ANX9030_HDCP_STATUS_AKSV_ACT	 	0x08	// bit position
#define ANX9030_HDCP_STATUS_ENCRYPT			0x04	// bit position
#define ANX9030_HDCP_STATUS_AUTH_PASS			0x02	// bit position
#define ANX9030_HDCP_STATUS_KEY_DONE			0x01	// bit position

#define ANX9030_HDCP_CTRL0_REG  0xA1
#define ANX9030_HDCP_CTRL0_STORE_AN 	 	0x80	// bit position
#define ANX9030_HDCP_CTRL0_RX_REP	 	0x40	// bit position
#define ANX9030_HDCP_CTRL0_RE_AUTH		0x20	// bit position
#define ANX9030_HDCP_CTRL0_SW_AUTHOK	    0x10	// bit position
#define ANX9030_HDCP_CTRL0_HW_AUTHEN	 	0x08	// bit position
#define ANX9030_HDCP_CTRL0_ENC_EN			0x04	// bit position
#define ANX9030_HDCP_CTRL0_BKSV_SRM			0x02	// bit position
#define ANX9030_HDCP_CTRL0_KSV_VLD			0x01	// bit position

#define ANX9030_HDCP_CTRL1_REG  0xA2
#define ANX9030_LINK_CHK_12_EN  0x40
#define ANX9030_HDCP_CTRL1_DDC_NOSTOP		0x20	// bit position
#define ANX9030_HDCP_CTRL1_DDC_NOACK	    0x10	// bit position
#define ANX9030_HDCP_CTRL1_EDDC_NOACK	 	0x08	// bit position
#define ANX9030_HDCP_CTRL1_BLUE_SCREEN_EN			0x04	// bit position
#define ANX9030_HDCP_CTRL1_RCV11_EN			0x02	// bit position
#define ANX9030_HDCP_CTRL1_HDCP11_EN			0x01	// bit position

#define ANX9030_HDCP_Link_Check_FRAME_NUM_REG  0xA3
#define ANX9030_HDCP_AKSV1_REG  0xA5
#define ANX9030_HDCP_AKSV2_REG  0xA6
#define ANX9030_HDCP_AKSV3_REG  0xA7
#define ANX9030_HDCP_AKSV4_REG  0xA8
#define ANX9030_HDCP_AKSV5_REG  0xA9

#define ANX9030_HDCP_AN1_REG  0xAA
#define ANX9030_HDCP_AN2_REG  0xAB
#define ANX9030_HDCP_AN3_REG  0xAC
#define ANX9030_HDCP_AN4_REG  0xAD
#define ANX9030_HDCP_AN5_REG  0xAE
#define ANX9030_HDCP_AN6_REG  0xAF
#define ANX9030_HDCP_AN7_REG  0xB0
#define ANX9030_HDCP_AN8_REG  0xB1

#define ANX9030_HDCP_BKSV1_REG  0xB2
#define ANX9030_HDCP_BKSV2_REG  0xB3
#define ANX9030_HDCP_BKSV3_REG  0xB4
#define ANX9030_HDCP_BKSV4_REG  0xB5
#define ANX9030_HDCP_BKSV5_REG  0xB6

#define ANX9030_HDCP_RI1_REG  0xB7
#define ANX9030_HDCP_RI2_REG  0xB8

#define ANX9030_HDCP_PJ_REG  0xB9
#define ANX9030_HDCP_RX_CAPS_REG  0xBA
#define ANX9030_HDCP_BSTATUS0_REG  0xBB
#define ANX9030_HDCP_BSTATUS1_REG  0xBC

#define ANX9030_HDCP_AMO0_REG  0xD0
#define ANX9030_HDCP_AMO1_REG  0xD1
#define ANX9030_HDCP_AMO2_REG  0xD2
#define ANX9030_HDCP_AMO3_REG  0xD3
#define ANX9030_HDCP_AMO4_REG  0xD4
#define ANX9030_HDCP_AMO5_REG  0xD5
#define ANX9030_HDCP_AMO6_REG  0xD6
#define ANX9030_HDCP_AMO7_REG  0xD7

#define ANX9030_HDCP_DBG_CTRL_REG  0xBD

#define ANX9030_HDCP_DBG_ENC_INC 	0x08	// bit position
#define ANX9030_HDCP_DBG_DDC_SPEED	0x06	// bit position
#define ANX9030_HDCP_DBG_SKIP_RPT	0x01	// bit position

#define ANX9030_HDCP_KEY_STATUS_REG  0xBE
#define ANX9030_HDCP_KEY_BIST_EN	0x04	// bit position
#define ANX9030_HDCP_KEY_BIST_ERR	0x02	// bit position
#define ANX9030_HDCP_KEY_CMD_DONE	0x01	// bit position

#define ANX9030_KEY_CMD_REGISTER 0xBF   //added

#define ANX9030_HDCP_AUTHDBG_STATUS_REG  0xC7 
#define ANX9030_HDCP_ENCRYPTDBG_STATUS_REG  0xC8
#define ANX9030_HDCP_FRAME_NUM_REG  0xC9

#define ANX9030_DDC_MSTR_INTER_REG  0xCA
#define ANX9030_DDC_MSTR_LINK_REG  0xCB

#define ANX9030_HDCP_BLUESCREEN0_REG  0xCC
#define ANX9030_HDCP_BLUESCREEN1_REG  0xCD
#define ANX9030_HDCP_BLUESCREEN2_REG  0xCE
//	DEV_ADDR = 0x7A or 0x7E
#define ANX9030_INFO_PKTCTRL1_REG  0xC0
#define ANX9030_INFO_PKTCTRL1_SPD_RPT 	 	0x80	// bit position
#define ANX9030_INFO_PKTCTRL1_SPD_EN	 	0x40	// bit position
#define ANX9030_INFO_PKTCTRL1_AVI_RPT		0x20	// bit position
#define ANX9030_INFO_PKTCTRL1_AVI_EN	    0x10	// bit position
#define ANX9030_INFO_PKTCTRL1_GCP_RPT	 	0x08	// bit position
#define ANX9030_INFO_PKTCTRL1_GCP_EN		0x04	// bit position
#define ANX9030_INFO_PKTCTRL1_ACR_NEW		0x02	// bit position
#define ANX9030_INFO_PKTCTRL1_ACR_EN		0x01	// bit position

#define ANX9030_INFO_PKTCTRL2_REG  0xC1
#define ANX9030_INFO_PKTCTRL2_UD1_RPT 	 	0x80	// bit position
#define ANX9030_INFO_PKTCTRL2_UD1_EN	 	0x40	// bit position
#define ANX9030_INFO_PKTCTRL2_UD0_RPT		0x20	// bit position
#define ANX9030_INFO_PKTCTRL2_UD0_EN	    0x10	// bit position
#define ANX9030_INFO_PKTCTRL2_MPEG_RPT	 	0x08	// bit position
#define ANX9030_INFO_PKTCTRL2_MPEG_EN		0x04	// bit position
#define ANX9030_INFO_PKTCTRL2_AIF_RPT		0x02	// bit position
#define ANX9030_INFO_PKTCTRL2_AIF_EN		0x01	// bit position

#define ANX9030_ACR_N1_SW_REG  0xC2
#define ANX9030_ACR_N2_SW_REG  0xC3
#define ANX9030_ACR_N3_SW_REG  0xC4

#define ANX9030_ACR_CTS1_SW_REG  0xC5
#define ANX9030_ACR_CTS2_SW_REG  0xC6
#define ANX9030_ACR_CTS3_SW_REG  0xC7

#define ANX9030_ACR_CTS1_HW_REG  0xC8
#define ANX9030_ACR_CTS2_HW_REG  0xC9
#define ANX9030_ACR_CTS3_HW_REG  0xCA

#define ANX9030_ACR_CTS_CTRL_REG  0xCB

#define ANX9030_GNRL_CTRL_PKT_REG  0xCC
#define ANX9030_GNRL_CTRL_CLR_AVMUTE		0x02	// bit position
#define ANX9030_GNRL_CTRL_SET_AVMUTE		0x01	// bit position

#define ANX9030_AUD_PKT_FLATCTRL_REG  0xCD
#define ANX9030_AUD_PKT_AUTOFLAT_EN 		0x80	// bit position
#define ANX9030_AUD_PKT_FLAT 	 			0x07	// bit position

#define YCbCr422 0x20
#define null 0
#define source_ratio 0x08

