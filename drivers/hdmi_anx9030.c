/*  driver for Analogix 9030 HDMI transmitter driver*/
#include <common.h>
#include <asm/io.h>
#include <command.h>
#include "anx9030.h"
#include <exports.h>

#if defined(CONFIG_ANALOGIX_9030) && defined(CONFIG_HARD_I2C)

unsigned char anx9030_video_timing_parameter[18]=
	{0x60/*H_RES_LOW*/,0x03 /*H_RES_HIGH*/,0xd0 /*ACT_PIX_LOW*/, 0x02/*ACT_PIX_HIGH*/, 
    0x3f/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0x45/*H_BP_LOW*/,0x00 /*H_BP_HIGH*/, 
    0x40/*ACT_LINE_LOW*/,0x02 /*ACT_LINE_HIGH*/, 0x03/*VSYNC_WIDTH*/, 0x13/*V_BP_LINE*/, 
    0x02/*V_FP_LINE*/, 0x0c/*H_FP_LOW*/, 0x00/*H_FP_HIGH*/, 0x08, 0x20, 0x40};
    
typedef struct
{
    unsigned char type;
    unsigned char version; 
    unsigned char length;
    unsigned char pb_byte[28];    
}infoframe_struct;

typedef struct
{
    infoframe_struct avi_info;   
} config_packets;


typedef enum
{
    ANX9030_avi_infoframe,    
}packet_type;

//audio_config_struct s_anx9030_audio_config;
config_packets s_anx9030_packet_config;

static int ANX9030_i2c_write_p0_reg(unsigned int registeraddr, unsigned char value)
{
    int rev;
    rev = i2c_write (ANX9030_ADDR1, registeraddr, 1, &value, 1);
    if (rev > 0){
        printf("HDMI Error: I2C wirte %d Error!\n",ANX9030_ADDR1<<1);
    }
    udelay(1000);
    return rev;
}

static int ANX9030_i2c_read_p0_reg(unsigned int registeraddr, unsigned char * value)
{
    int rev;
    rev = i2c_read (ANX9030_ADDR1, registeraddr, 1, value, 1);
    if (rev > 0){
        printf("HDMI Error: I2C read %d Error!\n",ANX9030_ADDR1<<1);
    }
    udelay(100);
    return rev;

}

static int ANX9030_i2c_write_p1_reg(unsigned int registeraddr, unsigned char value)
{
    int rev;
    rev = i2c_write (ANX9030_ADDR2, registeraddr, 1, &value, 1);
    if (rev > 0){
        printf("HDMI Error: I2C wirte %d Error!\n",ANX9030_ADDR2<<1);
    }
    udelay(1000);
    return rev;
    

}

static int ANX9030_i2c_read_p1_reg(unsigned int registeraddr, unsigned char* value)
{
    int rev;
    rev = i2c_read (ANX9030_ADDR2, registeraddr, 1, value, 1);
    if (rev > 0){
        printf("HDMI Error: I2C read %d Error!\n",ANX9030_ADDR2<<1);
    }
    udelay(100);
    return rev;
    

}
unsigned char ANX9030_Checksum(infoframe_struct *p)
{
    unsigned char checksum = 0x00;
    unsigned char i;

    checksum = p->type + p->length + p->version;
    for(i=1; i <= p->length; i++)
    {
        checksum += p->pb_byte[i];
    }
    checksum = ~checksum;
    checksum += 0x01;

    return checksum;
}

unsigned char ANX9030_Load_Infoframe(packet_type member,
                            infoframe_struct *p)
{
    unsigned char exe_result = 0x00;
    unsigned char address[8] = {0x00,0x20,0x40,0x60,0x80,0x80,0xa0,0xa0};
    unsigned char i;
    unsigned char c;

    p->pb_byte[0] = ANX9030_Checksum(p);        

    // write infoframe to according regs
    ANX9030_i2c_write_p1_reg(address[member], p->type);
    ANX9030_i2c_write_p1_reg(address[member]+1, p->version);
    ANX9030_i2c_write_p1_reg(address[member]+2, p->length);
    
    for(i=0; i <= p->length; i++)
    {
        ANX9030_i2c_write_p1_reg(address[member]+3+i, p->pb_byte[i]);
        ANX9030_i2c_read_p1_reg(address[member]+3+i, &c);
    }
    return exe_result;
}



void ANX9030_Hardware_Initial(void) 
{
    unsigned char c,i;
    unsigned char video_type,hsync_pol,vsync_pol,v_fp,v_bp,vsync_width;	
    unsigned char hsync_width_low,hsync_width_high,v_active_low,v_active_high;
    unsigned char h_active_low,h_active_high,h_res_low,h_res_high,h_bp_low,h_bp_high,h_fp_low,h_fp_high;
    unsigned int  hsync_width,h_active,h_res,h_bp,h_fp;
	
    gpio_hw_write(15, 0);
    udelay(20000);
    gpio_hw_write(15, 1);
    udelay(20000);
    //Power on I2C
    ANX9030_i2c_read_p0_reg(ANX9030_SYS_CTRL3_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_SYS_CTRL3_REG, (c | ANX9030_SYS_CTRL3_I2C_PWON));
	
    ANX9030_i2c_write_p0_reg(ANX9030_SYS_CTRL2_REG, 0x00);
    ANX9030_i2c_write_p0_reg(ANX9030_SRST_REG, 0x00);
    //clear HDCP_HPD_RST
    ANX9030_i2c_read_p0_reg(ANX9030_SYS_CTRL1_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_SYS_CTRL1_REG, c & 0xbf);
    //Power on Audio capture and Video capture module clock
    ANX9030_i2c_read_p0_reg(ANX9030_SYS_PD_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_SYS_PD_REG, (c | 0x06));
				
	
	
    //Enable auto set clock range for video PLL
    ANX9030_i2c_read_p0_reg(ANX9030_CHIP_CTRL_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_CHIP_CTRL_REG, (c & 0xfe));
	
	
    //Set registers of PLL loop bit and swing when using R_BIAS(680 Ohm)   
    ANX9030_i2c_read_p0_reg(ANX9030_TMDS_CH0_CONFIG_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_TMDS_CH0_CONFIG_REG, (c | 0x1c));
	
    ANX9030_i2c_read_p0_reg(ANX9030_TMDS_CH1_CONFIG_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_TMDS_CH1_CONFIG_REG, (c | 0x1c));
	
    ANX9030_i2c_read_p0_reg(ANX9030_TMDS_CH2_CONFIG_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_TMDS_CH2_CONFIG_REG, (c | 0x1c));
    
    ANX9030_i2c_read_p0_reg(ANX9030_TMDS_CLKCH_CONFIG_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_TMDS_CLKCH_CONFIG_REG, (c | 0x1c));
    //ANX9030_i2c_write_p0_reg(ANX9030_TMDS_CLKCH_CONFIG_REG, 0x81);//(c | 0x1c));//7150
	
    ANX9030_i2c_read_p0_reg(ANX9030_PLL_CTRL0_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_PLL_CTRL0_REG, (c & 0xf3)); 
				
    ANX9030_i2c_read_p0_reg(ANX9030_SYS_CTRL1_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_SYS_CTRL1_REG, (c | 0x03)); 
	
    //debug_puts("ANX9030_register initial OK!");
	
	
    //video config				
    //
    video_type = anx9030_video_timing_parameter[15];
    hsync_pol = anx9030_video_timing_parameter[16];
    vsync_pol = anx9030_video_timing_parameter[17];
    v_fp = anx9030_video_timing_parameter[12];
    v_bp = anx9030_video_timing_parameter[11];
    vsync_width = anx9030_video_timing_parameter[10];
    hsync_width = anx9030_video_timing_parameter[5];
    hsync_width = (hsync_width << 8) + anx9030_video_timing_parameter[4];
    v_active_high = anx9030_video_timing_parameter[9];
    v_active_low = anx9030_video_timing_parameter[8];
    h_active = anx9030_video_timing_parameter[3];
    h_active = (h_active << 8) + anx9030_video_timing_parameter[2];
    h_res = anx9030_video_timing_parameter[1];
    h_res = (h_res << 8) + anx9030_video_timing_parameter[0];
    h_bp = anx9030_video_timing_parameter[7];
    h_bp = (h_bp << 8) + anx9030_video_timing_parameter[6];
				
    hsync_width = 2* hsync_width;
    h_active = 2 * h_active;
    h_res = 2 * h_res;
    h_bp = 2 * h_bp;
	
    hsync_width_low = hsync_width & 0xff;
    hsync_width_high = (hsync_width >> 8) & 0xff;
    h_active_low = h_active & 0xff;
    h_active_high = (h_active >> 8) & 0xff;
    h_res_low = h_res & 0xff;
    h_res_high = (h_res >> 8) & 0xff;
    h_bp_low = h_bp & 0xff;
    h_bp_high = (h_bp >> 8) & 0xff;
	
    ANX9030_i2c_read_p0_reg(ANX9030_VID_CAPCTRL1_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_CAPCTRL1_REG, c & 0xf7 | video_type);//set video type
    ANX9030_i2c_read_p0_reg(ANX9030_VID_CAPCTRL1_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_CAPCTRL1_REG, c  & 0xdf |hsync_pol);//set HSYNC polarity
    ANX9030_i2c_read_p0_reg(ANX9030_VID_CAPCTRL1_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_CAPCTRL1_REG, c & 0xbf |vsync_pol);//set VSYNC polarity
    ANX9030_i2c_write_p0_reg(ANX9030_ACT_LINEL_REG, v_active_low);
    ANX9030_i2c_write_p0_reg(ANX9030_ACT_LINEH_REG, v_active_high);
    ANX9030_i2c_write_p0_reg(ANX9030_VSYNC_WID_REG, vsync_width);
    ANX9030_i2c_write_p0_reg(ANX9030_VSYNC_TAIL2VIDLINE_REG, v_bp);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_PIXL_REG, h_active_low);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_PIXH_REG, h_active_high);
    ANX9030_i2c_write_p0_reg(ANX9030_H_RESL_REG, h_res_low);
    ANX9030_i2c_write_p0_reg(ANX9030_H_RESH_REG, h_res_high);
    ANX9030_i2c_write_p0_reg(ANX9030_HSYNC_ACT_WIDTHL_REG, hsync_width_low);
    ANX9030_i2c_write_p0_reg(ANX9030_HSYNC_ACT_WIDTHH_REG, hsync_width_high);
    ANX9030_i2c_write_p0_reg(ANX9030_H_BACKPORCHL_REG, h_bp_low);
    ANX9030_i2c_write_p0_reg(ANX9030_H_BACKPORCHH_REG, h_bp_high);
    ANX9030_i2c_read_p0_reg(ANX9030_VID_CAPCTRL0_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_CAPCTRL0_REG, c | ANX9030_VID_CAPCTRL0_DEGEN_EN);
    //
    //				
    h_fp = anx9030_video_timing_parameter[14];
    h_fp = (h_fp << 8) + anx9030_video_timing_parameter[13];				
    h_fp = 2 * h_fp;
				
    h_fp_low = h_fp & 0xff;
    h_fp_high = (h_fp >> 8) & 0xff;
								
    ANX9030_i2c_read_p0_reg(ANX9030_VID_CAPCTRL0_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_CAPCTRL0_REG, c | ANX9030_VID_CAPCTRL0_EMSYNC_EN);
    ANX9030_i2c_write_p0_reg(ANX9030_ACT_LINE2VSYNC_REG, v_fp);
    ANX9030_i2c_write_p0_reg(ANX9030_H_FRONTPORCHL_REG, h_fp_low);
    ANX9030_i2c_write_p0_reg(ANX9030_H_FRONTPORCHH_REG, h_fp_high);
    //
				
    ANX9030_i2c_read_p0_reg(ANX9030_VID_CAPCTRL0_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_CAPCTRL0_REG, c | ANX9030_VID_CAPCTRL0_DEMUX_EN);				
    ANX9030_i2c_read_p0_reg(ANX9030_VID_MODE_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_MODE_REG, c | ANX9030_VID_MODE_CSPACE_Y2R);
    ANX9030_i2c_read_p0_reg(ANX9030_VID_MODE_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_MODE_REG, c & (~ANX9030_VID_MODE_Y2R_SEL));
    ANX9030_i2c_read_p0_reg(ANX9030_VID_MODE_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_MODE_REG, c | ANX9030_VID_MODE_UPSAMPLE);
    ANX9030_i2c_read_p0_reg(ANX9030_VID_MODE_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_MODE_REG, c & (~ANX9030_VID_MODE_RANGE_Y2R));
    ANX9030_i2c_read_p0_reg(ANX9030_VID_CTRL_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_CTRL_REG, ((c & 0xfc) |0x01));
    ANX9030_i2c_read_p0_reg(ANX9030_VID_CTRL_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_VID_CTRL_REG, c | ANX9030_VID_CTRL_IN_EN);
    ANX9030_i2c_read_p0_reg(ANX9030_SYS_CTRL1_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_SYS_CTRL1_REG, c | 0x02);
    ANX9030_i2c_read_p0_reg(ANX9030_SRST_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_SRST_REG, (c | ANX9030_TX_RST));
    ANX9030_i2c_write_p0_reg(ANX9030_SRST_REG, (c & (~ANX9030_TX_RST)));
    //Enable TMDS clock output // just enable bit7, and let the other bits along to avoid overwriting.
    ANX9030_i2c_read_p0_reg(ANX9030_TMDS_CLKCH_CONFIG_REG, &c);
    ANX9030_i2c_write_p0_reg(ANX9030_TMDS_CLKCH_CONFIG_REG, (c | ANX9030_TMDS_CLKCH_MUTE));
    //video config
    //packet config
    s_anx9030_packet_config.avi_info.type = 0x82;
    s_anx9030_packet_config.avi_info.version = 0x02;
    s_anx9030_packet_config.avi_info.length = 0x0d;
    s_anx9030_packet_config.avi_info.pb_byte[1] = 0x01;//rgb
    s_anx9030_packet_config.avi_info.pb_byte[2] = 0x08; 
    s_anx9030_packet_config.avi_info.pb_byte[3] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[4] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[5] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[6] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[7] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[8] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[9] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[10] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[11] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[12] = 0x00; 
    s_anx9030_packet_config.avi_info.pb_byte[13] = 0x00;
				
    c = s_anx9030_packet_config.avi_info.pb_byte[1];  //color space
    c &= 0x9f;
    c |= (0x00 << 5);			//RGB
    s_anx9030_packet_config.avi_info.pb_byte[1] = c | 0x10;
    c = s_anx9030_packet_config.avi_info.pb_byte[4];// vid ID
    c = c & 0x80;
    s_anx9030_packet_config.avi_info.pb_byte[4] = c | 0x15;//576i@4:3
    c = s_anx9030_packet_config.avi_info.pb_byte[5]; //repeat times
    c = c & 0xf0;
    c |= 0x01;					//repeat 1 time
    s_anx9030_packet_config.avi_info.pb_byte[5] = c;
    //printf("config avi infoframe packet."); 
    // Disable repeater
    ANX9030_i2c_read_p1_reg(ANX9030_INFO_PKTCTRL1_REG, &c);				
    c &= ~ANX9030_INFO_PKTCTRL1_AVI_RPT;
    ANX9030_i2c_write_p1_reg(ANX9030_INFO_PKTCTRL1_REG, c);				
						
    // load packet data to regs
    ANX9030_Load_Infoframe( ANX9030_avi_infoframe,
                            &(s_anx9030_packet_config.avi_info));
				
				
    // Enable and repeater
    ANX9030_i2c_read_p1_reg(ANX9030_INFO_PKTCTRL1_REG, &c);
    c |= 0x30;
    ANX9030_i2c_write_p1_reg(ANX9030_INFO_PKTCTRL1_REG, c);
    //packet config
}

int do_hdmi_init(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (argc > 1){
        printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
    }
    else
        ANX9030_Hardware_Initial();
    return 0;
}

U_BOOT_CMD(
	anx9030_init,	1,	1,	do_hdmi_init,
	"anx9030_init     - initial hdmi transmitter of Analogix 9030\n",
    "anx9030_init        -set analogix 9030 to PAL"

);



#endif /* CONFIG_ANALOGIX_9030 */
