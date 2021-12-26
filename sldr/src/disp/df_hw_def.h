#ifndef _DF_HW_DEF_H_
#define _DF_HW_DEF_H_
#include "../common/type_def.h"

//#define ORION_1_3_0_VERSION
#define ORION_1_4_VERSION
#ifdef ORION_1_4_VERSION
#define DF_HW_FIXED_SCALOR_COEFF
#define DF_SCALOR_CHROMA_422_TO_444
#define DF_2VIDEO_LAYERS
//#define DF_H264_VIDEO_SRC
#endif

#define BIT_MASK( BitWidth ) ((1 << (BitWidth)) - 1)

enum _YUV420_SCALOR_CFG_
{
#ifdef ORION_1_4_VERSION
	VIDEO_SCALOR_Y_FIR_TAP_NUM            = 2,
	VIDEO_SCALOR_C_FIR_TAP_NUM            = 2,
	
	VIDEO_SCALOR_PAHSE_BIT_WIDTH          = 8,
	VIDEO_SCALOR_PAHSE_NUM                = (1 << VIDEO_SCALOR_PAHSE_BIT_WIDTH),
	VIDEO_SCALOR_STEP_FRACTION_BIT_WIDTH  = 10,
	
	VIDEO_SCALOR_COEFF_FRACTION_BIT_WIDTH = 10,//VIDEO_SCALOR_PAHSE_BIT_WIDTH * 3,//
	VIDEO_SCALOR_COEFF_INTREGER_BIT_WIDTH = 1,
	VIDEO_SCALOR_COEFF_POLARITY_BIT       = 15,	
#else
	VIDEO_SCALOR_Y_FIR_TAP_NUM            = 4,
	VIDEO_SCALOR_C_FIR_TAP_NUM            = 2,
	
	VIDEO_SCALOR_PAHSE_BIT_WIDTH          = 3,
	VIDEO_SCALOR_PAHSE_NUM                = (1 << VIDEO_SCALOR_PAHSE_BIT_WIDTH),
	VIDEO_SCALOR_STEP_FRACTION_BIT_WIDTH  = 13,

	VIDEO_SCALOR_COEFF_FRACTION_BIT_WIDTH = 10,//VIDEO_SCALOR_PAHSE_BIT_WIDTH * 3,//
	VIDEO_SCALOR_COEFF_INTREGER_BIT_WIDTH = 1,
	VIDEO_SCALOR_COEFF_POLARITY_BIT       = 15,		
#endif
};

//Define Register Map
/*display processor register*/
//-----------------------------------------------------------------
//                  Register Usage Note
//-----------------------------------------------------------------
//1. How to Caculate Scalor Source Video Width and Height
//
enum _ADDRESS_SPACE_
{
	HW_REG_ADDR_MASK = ~(0xFFFFF),
};

enum  _DF_REGISTER_BANK_
{
   DISP_REG_BASE              = 0x41800000, 
   //-----------------------------------
   //        DISP_BYTE_ENDIAN
   //-----------------------------------
   //  Bit[0]: Byte_endian, 32bit Bytes Endian,  Reset Value: 0
   //  Bit[1]: word_endian, 128bit 32bit Endian, Reset Value: 1
   //  Bit[2]: 16bit Bytes Endian,  Reset Value: 1
   //  For All Endian Bit, if 0 change bit Order, 1 unchanged
   //   
   //
   DISP_BYTE_ENDIAN           = ( DISP_REG_BASE ) + ( 0 << 2),      //  byte_endian 
   DISP_NIBBLE_ENDIAN         = ( DISP_REG_BASE ) + ( 1 << 2),      //  nibble_endian <= i_host_wr_data[0];
                                     
   DISP_GFX1_BUF_START        = ( DISP_REG_BASE ) + ( 2 << 2),      //  gfx1_buf_start   <= i_host_wr_data[19:0];
   DISP_GFX1_LINE_PITCH       = ( DISP_REG_BASE ) + ( 3 << 2),      //  gfx1_line_pitch  <= i_host_wr_data[19:0];
   DISP_GFX1_BLANK_PXL        = ( DISP_REG_BASE ) + ( 4 << 2),      //  gfx1_blank_pixel <= i_host_wr_data[4:0];
                                     
   DISP_GFX2_BUF_START        = ( DISP_REG_BASE ) + ( 5 << 2),      //  gfx2_buf_start   <= i_host_wr_data[19:0];
   DISP_GFX2_LINE_PITCH       = ( DISP_REG_BASE ) + ( 6 << 2),      //  gfx2_line_pitch  <= i_host_wr_data[19:0];
   DISP_GFX2_BLANK_PXL        = ( DISP_REG_BASE ) + ( 7 << 2),      //  gfx2_blank_pixel <= i_host_wr_data[4:0];
                                     
   DISP_GFX1_X_START          = ( DISP_REG_BASE ) + ( 8 << 2),      //  gfx1_x_start  <= i_host_wr_data[10:0];
   DISP_GFX1_X_END            = ( DISP_REG_BASE ) + ( 9 << 2),      //  gfx1_x_end    <= i_host_wr_data[10:0];
   DISP_GFX1_Y_START          = ( DISP_REG_BASE ) + (10 << 2),      //  gfx1_y_start  <= i_host_wr_data[10:0];
   DISP_GFX1_Y_END            = ( DISP_REG_BASE ) + (11 << 2),      //  gfx1_y_end    <= i_host_wr_data[10:0];
                                     
   DISP_GFX2_X_START          = ( DISP_REG_BASE ) + (12 << 2),      //  gfx2_x_start  <= i_host_wr_data[10:0];
   DISP_GFX2_X_END            = ( DISP_REG_BASE ) + (13 << 2),      //  gfx2_x_end    <= i_host_wr_data[10:0];
   DISP_GFX2_Y_START          = ( DISP_REG_BASE ) + (14 << 2),      //  gfx2_y_start  <= i_host_wr_data[10:0];
   DISP_GFX2_Y_END            = ( DISP_REG_BASE ) + (15 << 2),      //  gfx2_y_end    <= i_host_wr_data[10:0];
                                     
   DISP_VIDEO_X_START         = ( DISP_REG_BASE ) + (16 << 2),      //  video_x_start  <= i_host_wr_data[10:0];
   DISP_VIDEO_X_END           = ( DISP_REG_BASE ) + (17 << 2),      //  video_x_end    <= i_host_wr_data[10:0];
   DISP_VIDEO_Y_START         = ( DISP_REG_BASE ) + (18 << 2),      //  video_y_start  <= i_host_wr_data[10:0];
   DISP_VIDEO_Y_END           = ( DISP_REG_BASE ) + (19 << 2),      //  video_y_end    <= i_host_wr_data[10:0];
                                     
   DISP_DISP_ENABLE           = ( DISP_REG_BASE ) + (20 << 2),      //  disp_en   <= i_host_wr_data[0];
   DISP_SYNC_MODE             = ( DISP_REG_BASE ) + (21 << 2),      //  Not Used In Current Version, sync_mode <= i_host_wr_data[0];
                                     
   DISP_VIDEO_FORMAT          = ( DISP_REG_BASE ) + (22 << 2),      //  video_format <= i_host_wr_data[2:0];
                                     
   DISP_GFX1_EN               = ( DISP_REG_BASE ) + (23 << 2),      //  gfx1_en <= 1'b0;
   DISP_GFX1_FORMAT           = ( DISP_REG_BASE ) + (24 << 2),      //  gfx1_format <= i_host_wr_data[2:0];
	//-----------------------------------
	// Add ARGB1555 Format Support 
	//     gfx1_format:	 DF_GFX_ARGB1555  = 5,
	//     DISP_GFX1_FORMAT Bit Format
	// ---------------------------------
	// Bit[15: 8] ARGB1555 0 Alpha Value
	// Bit[23:16] ARGB1555 1 Alpha Value
	//----------------------------------
   DISP_GFX1_DEFAULT_ALPHA    = ( DISP_REG_BASE ) + (25 << 2),      //  gfx1_default_alpha <= i_host_wr_data[7:0];
   DISP_GFX1_Z_ORDER          = ( DISP_REG_BASE ) + (26 << 2),      //  gfx1_z_order <= i_host_wr_data[1:0];
   DISP_GFX1_KEYING_EN        = ( DISP_REG_BASE ) + (27 << 2),      //  gfx1_keying_en <= i_host_wr_data[0];
   DISP_GFX1_R_MIN            = ( DISP_REG_BASE ) + (28 << 2),      //  gfx1_r_min <= i_host_wr_data[4:0];
   DISP_GFX1_R_MAX            = ( DISP_REG_BASE ) + (29 << 2),      //  gfx1_r_max <= i_host_wr_data[4:0]; 
   DISP_GFX1_G_MIN            = ( DISP_REG_BASE ) + (30 << 2),      //  gfx1_g_min <= i_host_wr_data[5:0];
   DISP_GFX1_G_MAX            = ( DISP_REG_BASE ) + (31 << 2),      //  gfx1_g_max <= i_host_wr_data[5:0];
   DISP_GFX1_B_MIN            = ( DISP_REG_BASE ) + (32 << 2),      //  gfx1_b_min <= i_host_wr_data[4:0];
   DISP_GFX1_B_MAX            = ( DISP_REG_BASE ) + (33 << 2),      //  gfx1_b_max <= i_host_wr_data[4:0];
                                     
   DISP_GFX2_EN               = ( DISP_REG_BASE ) + (34 << 2),      //  gfx2_en <= 1'b0;
   DISP_GFX2_FORMAT           = ( DISP_REG_BASE ) + (35 << 2),      //  gfx2_format <= i_host_wr_data[2:0];
	//-----------------------------------
	// Add ARGB1555 Format Support 
	//     DISP_GFX1_FORMAT Bit Format
	//     gfx2_format:	 DF_GFX_ARGB1555  = 5,
	// ---------------------------------
	// Bit[15: 8] ARGB1555 0 Alpha Value, the Reset Value is 0;
	// Bit[23:16] ARGB1555 1 Alpha Value, the Reset Value is 1;
	//----------------------------------

   DISP_GFX2_DEFAULT_ALPHA    = ( DISP_REG_BASE ) + (36 << 2),      //  gfx2_default_alpha <= i_host_wr_data[7:0];
   DISP_GFX2_Z_ORDER          = ( DISP_REG_BASE ) + (37 << 2),      //  gfx2_z_order <= i_host_wr_data[1:0];
   DISP_GFX2_KEYING_EN        = ( DISP_REG_BASE ) + (38 << 2),      //  gfx2_keying_en <= i_host_wr_data[0];
   DISP_GFX2_R_MIN            = ( DISP_REG_BASE ) + (39 << 2),      //  gfx2_r_min <= i_host_wr_data[4:0];
   DISP_GFX2_R_MAX            = ( DISP_REG_BASE ) + (40 << 2),      //  gfx2_r_max <= i_host_wr_data[4:0]; 
   DISP_GFX2_G_MIN            = ( DISP_REG_BASE ) + (41 << 2),      //  gfx2_g_min <= i_host_wr_data[5:0];
   DISP_GFX2_G_MAX            = ( DISP_REG_BASE ) + (42 << 2),      //  gfx2_g_max <= i_host_wr_data[5:0];
   DISP_GFX2_B_MIN            = ( DISP_REG_BASE ) + (43 << 2),      //  gfx2_b_min <= i_host_wr_data[4:0];
   DISP_GFX2_B_MAX            = ( DISP_REG_BASE ) + (44 << 2),      //  gfx2_b_max <= i_host_wr_data[4:0];
                                     
   DISP_VIDEO_ENABLE          = ( DISP_REG_BASE ) + (45 << 2),      //  video_en <= i_host_wr_data[0];
   DISP_VIDEO_DEFAULT_ALPHA   = ( DISP_REG_BASE ) + (46 << 2),      //  video_default_alpha <= i_host_wr_data[7:0];
   DISP_VIDEO_Z_ORDER         = ( DISP_REG_BASE ) + (47 << 2),      //  video_z_order <= i_host_wr_data[1:0];
                                     
   DISP_BG_Y                  = ( DISP_REG_BASE ) + (48 << 2),      //  bg_y <= i_host_wr_data[7:0];
   DISP_BG_U                  = ( DISP_REG_BASE ) + (49 << 2),      //  bg_u <= i_host_wr_data[7:0];
   DISP_BG_V                  = ( DISP_REG_BASE ) + (50 << 2),      //  bg_v <= i_host_wr_data[7:0];
                                     
   DISP_UPDATE_REG            = ( DISP_REG_BASE ) + (51 << 2),      //  reg_update_in_proc <= i_host_wr_data[0];                 

   DISP_CLUT1_ADDR            = ( DISP_REG_BASE ) + (52 << 2),      //  clu1_table_addr <= i_host_wr_data[7:0];
   DISP_CLUT1_WE              = ( DISP_REG_BASE ) + (53 << 2),      //  (W) clu1_table_we <= 1; (R) clu1_table_we <= 0;

   DISP_CLUT2_ADDR            = ( DISP_REG_BASE ) + (54 << 2),      //  clu2_table_addr <= i_host_wr_data[7:0];
   DISP_CLUT2_WE              = ( DISP_REG_BASE ) + (55 << 2),      //  (W) clu2_table_we <= 1; (R) clu2_table_we <= 0;

   DISP_GFX1_DRAM_MAPPING     = ( DISP_REG_BASE ) + (56 << 2),      //  GFX1 dram mpping;
   DISP_GFX2_DRAM_MAPPING     = ( DISP_REG_BASE ) + (57 << 2),      //  GFX2 dram mpping;

   DISP_CLIPPING_EN           = ( DISP_REG_BASE ) + (58 << 2),      //  display output clipping enable ;
#ifdef USER_SET_RGB_YUV_CONVERT_TABLE
   DISP_CONVERT_C00           = ( DISP_REG_BASE ) + (59 << 2),      // -255 ... 255
   DISP_CONVERT_C01           = ( DISP_REG_BASE ) + (60 << 2), 
   DISP_CONVERT_C02           = ( DISP_REG_BASE ) + (61 << 2), 
   DISP_CONVERT_C03           = ( DISP_REG_BASE ) + (62 << 2), 
   
   DISP_CONVERT_C10           = ( DISP_REG_BASE ) + (63 << 2), 
   DISP_CONVERT_C11           = ( DISP_REG_BASE ) + (64 << 2), 
   DISP_CONVERT_C12           = ( DISP_REG_BASE ) + (65 << 2), 
   DISP_CONVERT_C13           = ( DISP_REG_BASE ) + (66 << 2), 
   
   DISP_CONVERT_C20           = ( DISP_REG_BASE ) + (67 << 2), 
   DISP_CONVERT_C21           = ( DISP_REG_BASE ) + (68 << 2), 
   DISP_CONVERT_C22           = ( DISP_REG_BASE ) + (69 << 2), 
   DISP_CONVERT_C23           = ( DISP_REG_BASE ) + (70 << 2), 
#endif
//-------------------------------------------------------------
//             User defined Display Mode Register
//-------------------------------------------------------------
//Adapted From Orion 1.4 Should Port the Coressponding RTL Code
//-------------------------------------------------------------
#ifdef ORION_1_4_VERSION
   DISP_USR_CTRL          = ( DISP_REG_BASE ) + (71 << 2),     
#endif
   DISP_USR_X_TOTAL       = ( DISP_REG_BASE ) + (72 << 2),     
   DISP_USR_Y_TOTAL       = ( DISP_REG_BASE ) + (73 << 2),     
   DISP_USR_ACTIVE_X      = ( DISP_REG_BASE ) + (74 << 2),     
   DISP_USR_ACTIVE_Y      = ( DISP_REG_BASE ) + (75 << 2),     
   DISP_USR_SAV_START     = ( DISP_REG_BASE ) + (76 << 2),     
   DISP_USR_V0_START_TOP  = ( DISP_REG_BASE ) + (77 << 2),     
   DISP_USR_V0_END_TOP    = ( DISP_REG_BASE ) + (78 << 2),     
   DISP_USR_V0_START_BOT  = ( DISP_REG_BASE ) + (79 << 2),     
   DISP_USR_V0_END_BOT    = ( DISP_REG_BASE ) + (80 << 2),     
   DISP_USR_F0_START      = ( DISP_REG_BASE ) + (81 << 2),     
   DISP_USR_F0_END        = ( DISP_REG_BASE ) + (82 << 2),     
   DISP_USR_HS_START      = ( DISP_REG_BASE ) + (83 << 2),     
   DISP_USR_HS_END        = ( DISP_REG_BASE ) + (84 << 2),     
   DISP_USR_VS_END_TOP    = ( DISP_REG_BASE ) + (85 << 2),     
   DISP_USR_VS_END_BOT    = ( DISP_REG_BASE ) + (86 << 2),     
   DISP_USR_IS_HD         = ( DISP_REG_BASE ) + (87 << 2),     
   DISP_USR_IMG_WIDTH     = ( DISP_REG_BASE ) + (88 << 2),     
   DISP_USR_IMG_HEIGHT    = ( DISP_REG_BASE ) + (89 << 2),     
#ifdef DF_2VIDEO_LAYERS
//Used For Orion 1.4 of two Video2
//Reseved in Orion 1.3.1
   DISP_VIDEO2_X_START        = ( DISP_REG_BASE ) + (90 << 2),     //  video_x_start  <= i_host_wr_data[10:0];
   DISP_VIDEO2_X_END          = ( DISP_REG_BASE ) + (91 << 2),     //  video_x_end    <= i_host_wr_data[10:0];
   DISP_VIDEO2_Y_START        = ( DISP_REG_BASE ) + (92 << 2),     //  video_y_start  <= i_host_wr_data[10:0];
   DISP_VIDEO2_Y_END          = ( DISP_REG_BASE ) + (93 << 2),     //  video_y_end    <= i_host_wr_data[10:0];
//Whya a Video 2 Format?
   DISP_VIDEO2_FORMAT         = ( DISP_REG_BASE ) + (94 << 2),     //  video_format <= i_host_wr_data[2:0];
   DISP_VIDEO2_ENABLE         = ( DISP_REG_BASE ) + (95 << 2),     //  video_en <= i_host_wr_data[0];
   DISP_VIDEO2_DEFAULT_ALPHA  = ( DISP_REG_BASE ) + (96 << 2),     //  video_default_alpha <= i_host_wr_data[7:0];
   DISP_VIDEO2_Z_ORDER        = ( DISP_REG_BASE ) + (97 << 2),     //  video_z_order <= i_host_wr_data[1:0];
#endif

//-------------------------------------------------------------
//            Output IF Y Value Clip Register
//-------------------------------------------------------------
//Adapted From Orion 1.4 Should Port the Coressponding RTL Code
//-------------------------------------------------------------

   DISP_CLIP_Y_LOW            = ( DISP_REG_BASE ) + ( 98 << 2),     //for composite use
   DISP_CLIP_Y_RANGE          = ( DISP_REG_BASE ) + ( 99 << 2),     //
   DISP_CLIP_C_RANGE          = ( DISP_REG_BASE ) + (100 << 2),     //128+range
#ifdef 	ORION_1_4_VERSION
   DISP_VERTICAL_SEL          = ( DISP_REG_BASE)  + (101 << 2),
   DISP_USR_CTRL_V2           = ( DISP_REG_BASE)  + (102 << 2),
#endif
//Video Source Regain Clip Control 
//  -----------------------------------------
//  DISP_VIDEO_SRC_X_CLIP Format:
//  Bit[10 : 0] X/Y Offset
//  Bit[26 :16] Crop Windows Width/Height
//  -----------------------------------------
   DISP_VIDEO_SRC_X_CROP     = ( DISP_REG_BASE ) + (103 << 2),     
   DISP_VIDEO_SRC_Y_CROP     = ( DISP_REG_BASE ) + (104 << 2),     

#ifdef DF_2VIDEO_LAYERS
   DISP_VIDEO2_SRC_X_CROP    = ( DISP_REG_BASE ) + (105 << 2),     
   DISP_VIDEO2_SRC_Y_CROP    = ( DISP_REG_BASE ) + (106 << 2),     
#endif

#ifndef DF_HW_FIXED_SCALOR_COEFF
//Scalor FIR Coefficients Define
//4-TAP Luma FIR Coefficients
//------------------------------------------------------------------------------
//   FIR Coefficient Formate
//   Bit[31   ] :  Tap 0 Sign bit
//   Bit[26:16] :  Tap 0 Coefficient Value (1 bit Integer bit, 10 bit fraction)
//
//   Bit[15   ] :  Tap 1 Sign bit
//   Bit[10: 0] :  Tap 1 Coefficient Value (1 bit Integer bit, 10 bit fraction)
//-----------------------------------------------------------------------------

//Display Luma 4tap FIR Phase 0 Tap0 and Tap 1 Coefficent

 DISP_LUMA_SCAL_COEFF_P0_T0_T1    = ( DISP_REG_BASE ) + (107 << 2),
 DISP_LUMA_SCAL_COEFF_P0_T2_T3    = ( DISP_REG_BASE ) + (108 << 2),
                                                              
 DISP_LUMA_SCAL_COEFF_P1_T0_T1	  = ( DISP_REG_BASE ) + (109 << 2),
 DISP_LUMA_SCAL_COEFF_P1_T2_T3    = ( DISP_REG_BASE ) + (110 << 2),
                                                              
 DISP_LUMA_SCAL_COEFF_P2_T0_T1	  = ( DISP_REG_BASE ) + (111 << 2),
 DISP_LUMA_SCAL_COEFF_P2_T2_T3    = ( DISP_REG_BASE ) + (112 << 2),
                                                              
 DISP_LUMA_SCAL_COEFF_P3_T0_T1	  = ( DISP_REG_BASE ) + (113 << 2), 
 DISP_LUMA_SCAL_COEFF_P3_T2_T3    = ( DISP_REG_BASE ) + (114 << 2),
                                                              
 DISP_LUMA_SCAL_COEFF_P4_T0_T1	  = ( DISP_REG_BASE ) + (115 << 2),
 DISP_LUMA_SCAL_COEFF_P4_T2_T3    = ( DISP_REG_BASE ) + (116 << 2),
                                                               
 DISP_LUMA_SCAL_COEFF_P5_T0_T1	  = ( DISP_REG_BASE ) + (117  << 2), 
 DISP_LUMA_SCAL_COEFF_P5_T2_T3    = ( DISP_REG_BASE ) + (118  << 2),
                                                               
 DISP_LUMA_SCAL_COEFF_P6_T0_T1	  = ( DISP_REG_BASE ) + (119  << 2), 
 DISP_LUMA_SCAL_COEFF_P6_T2_T3    = ( DISP_REG_BASE ) + (120  << 2),
                                                               
 DISP_LUMA_SCAL_COEFF_P7_T0_T1	  = ( DISP_REG_BASE ) + (121  << 2), 			 
 DISP_LUMA_SCAL_COEFF_P7_T2_T3    = ( DISP_REG_BASE ) + (122  << 2), 
#endif

//---------------------------------------
//           INIT_PHASE Format
//---------------------------------------
//   [2  : 0]  Vetical Initial Phase
//   [10 : 8]  Horizontal Initial Phase
//--------------------------------------- 
//Remove to CMD Interface
#ifdef DF_SET_INIT_PHASE_IN_REG_BANK
 DISP_SCAL_INIT_PHASE             = ( DISP_REG_BASE ) + (123  << 2),
#endif
//---------------------------------------
//           LUMA_SCAL_VFIR_TAP_NUM
//---------------------------------------
//   [0] : 0,  Vertical FIR Using 4 Tap, Default Value
//         1,  Vertical FIR Using 2 Tap        
//---------------------------------------  
#ifndef ORION_1_4_VERSION
 DISP_LUMA_SCAL_VFIR_TAP_NUM_SEL  = ( DISP_REG_BASE ) + (124  << 2),
#endif 
//2-TAP Chroma FIR Coefficients
//Not Needed Currently  Fixed By Hardware

//OutIF Display Clock Output
 DISP_DISP_CLK_OUT_ENABLE        =  ( DISP_REG_BASE ) + (125  << 2), //Default is Enable, Set 1 to Enable, Set 0 Disable
//---------------------------------------
//           DISP_ERR_CLEAR
//---------------------------------------
//   Any Write Operation will Clear
//   the DF Error.
//   the Read back Value is meaningless
//---------------------------------------  
 DISP_ERR_CLEAR                  =  ( DISP_REG_BASE ) + (126  << 2), 
 
};

enum _DF_REG_BYTE_EDNIAN_FORMAT_
{
	DF_REG_BYTE_ENDIAN_128BIT_BIT = 1,
	DF_REG_BYTE_ENDIAN_16BIT_BIT  = 2,
	DF_REG_BYTE_ENDIAN_BYTE_BIT   = 0,
};
enum _DF_REG_GFX_FORMAT_FORMAT_
{
	DF_REG_GFX_FORMAT_FORMAT_LSB   = 0,
	DF_REG_GFX_FORMAT_FORMAT_WIDTH = 3,

	DF_REG_GFX_FORMAT_ALPHA0_LSB   = 8,
	DF_REG_GFX_FORMAT_ALPHA0_WIDTH = 8,

	DF_REG_GFX_FORMAT_ALPHA1_LSB   = 16,
	DF_REG_GFX_FORMAT_ALPHA1_WIDTH = 8,
	
};

enum _DF_REG_VIDEO_CROP_FORMAT_
{
	DF_REG_VIDEO_CROP_OFF_LSB   = 0,
	DF_REG_VIDEO_CROP_OFF_WIDTH = 11,
	DF_REG_VIDEO_CROP_WH_LSB    = 16,
	DF_REG_VIDEO_CROP_WH_WIDTH  = 11,	
};

enum DF_REG_VIDEO_SCAL_COEFF_FORMAT_
{
	DF_REG_VIDEO_SCAL_COEFF0_LSB    = 16,
	DF_REG_VIDEO_SCAL_COEFF0_WIDTH  = 16,	

	DF_REG_VIDEO_SCAL_COEFF1_LSB    =  0,
	DF_REG_VIDEO_SCAL_COEFF1_WIDTH  = 16,

};
enum _DF_REG_VIDEO_SCAL_INIT_PHASE_FORMAT_
{
	DF_REG_SCAL_INI_PHASE_V_LSB   =  0,
	DF_REG_SCAL_INI_PHASE_V_WIDTH =  3,
	DF_REG_SCAL_INI_PHASE_H_LSB   =  8,
	DF_REG_SCAL_INI_PHASE_H_WIDTH =  3,
};

typedef enum _DF_GFX_COLOR_FORMAT_
{
	DF_GFX_CLUT4      = 0,
	DF_GFX_CLUT8      = 1,
	DF_GFX_RGB565    = 2,
	DF_GFX_ARGB4444  = 3,
	DF_GFX_A0        = 4,
	DF_GFX_ARGB1555  = 5,
	
} DF_GFX_COLOR_FORMAT;

//Define Command Interface Register

enum _DFVC_STA_FORMAT_
{
#ifdef ORION_1_4_VERSION
	DFV2C_STA_IDEL_BIT     = 8,
	DFV2C_STA_CMD_FULL_BIT = 7,
	DFV2C_STA_DFERR_BIT    = 6,
#else
	DF_VIDEO_CORP_STA_SHIFT = 0,
#endif
	DFVC_STA_IDEL_BIT     = 5,
	DFVC_STA_CMD_FULL_BIT = 4,
	DFVC_STA_DFERR_BIT    = 3,
};
enum _DFVC_CMD_FORMAT_
{
	DFVC_CMD_LEVEL_LSB    = 30,
	DFVC_CMD_LEVEL_WIDTH  =  2,
	DFVC_CMD_IDX_LSB      = 28,
	DFVC_CMD_IDX_WIDTH    =  2,
};

enum _DFVC_ADDR_FORMAT_
{
	DFVC_ADDR_LSB   = 0,
	DFVC_ADDR_WIDTH = 24,	
};


//Command Interface API
enum _DF_VIDEO_CORP_REG_
{
	DF_VIDEO_CORP_REG_BASE  = 0x41600000,
#ifdef ORION_1_4_VERSION
	REG_DF_VIDEO_CORP_CMD       = (DF_VIDEO_CORP_REG_BASE + (0x16 << 2)),
	REG_DF_VIDEO_CORP_STA       = (DF_VIDEO_CORP_REG_BASE + (0x01 << 2)),
	REG_DF_VIDEO_CORP_ADDR      = (DF_VIDEO_CORP_REG_BASE + (0x17 << 2)),
	REG_DF_VIDEO_CORP_STA0      = (DF_VIDEO_CORP_REG_BASE + (0x18 << 2)),
	REG_DF_VIDEO_CORP_STA1      = (DF_VIDEO_CORP_REG_BASE + (0x19 << 2)),
	REG_DF_VIDEO_CORP_STA2      = (DF_VIDEO_CORP_REG_BASE + (0x1a << 2)),

	REG_DF_VIDEO2_CORP_CMD      = (DF_VIDEO_CORP_REG_BASE + (0x1b << 2)),
	REG_DF_VIDEO2_CORP_STA      = (DF_VIDEO_CORP_REG_BASE + (0x01 << 2)),
	REG_DF_VIDEO2_CORP_ADDR     = (DF_VIDEO_CORP_REG_BASE + (0x1c << 2)),
	REG_DF_VIDEO2_CORP_STA0     = (DF_VIDEO_CORP_REG_BASE + (0x1d << 2)),
	REG_DF_VIDEO2_CORP_STA1     = (DF_VIDEO_CORP_REG_BASE + (0x1e << 2)),
	REG_DF_VIDEO2_CORP_STA2     = (DF_VIDEO_CORP_REG_BASE + (0x1f << 2)),	
#else	
	DF_VIDEO_CORP_CMD       = (DF_VIDEO_CORP_REG_BASE + 0x78),
//Read Only Staturs Register
	DF_VIDEO_CORP_STA       = (DF_VIDEO_CORP_REG_BASE + 0x2C),
	DF_VIDEO_CORP_ADDR      = (DF_VIDEO_CORP_REG_BASE + 0xB0),
	DF_VIDEO_CORP_STA0      = (DF_VIDEO_CORP_REG_BASE + 0xB4),
	DF_VIDEO_CORP_STA1      = (DF_VIDEO_CORP_REG_BASE + 0xB8),
	DF_VIDEO_CORP_STA2      = (DF_VIDEO_CORP_REG_BASE + 0xBC)		
#endif
};

#ifdef ORION_1_4_VERSION
extern U32	DF_VIDEO_CORP_CMD       ;
//Read Only Staturs Register
extern U32	DF_VIDEO_CORP_STA       ;
extern U32	DF_VIDEO_CORP_ADDR      ;
extern U32	DF_VIDEO_CORP_STA0      ;
extern U32	DF_VIDEO_CORP_STA1      ;
extern U32	DF_VIDEO_CORP_STA2      ;
#endif
/****************************************************************
//                DF_VIDEO_CORP_STA0  FORMAT
//---------------------------------------------------------------
// 0th     bit: display_enable: host read/write, firmware read
// 1th     bit: video_display_enable: host read/write firmware read/
// 2th     bit: Current Display Address is Valid
// 3th     bit: Reserved
// 4th-6th bit: video_format: 
//			 DISP_IS_PAL 	     3 
//			 DISP_IS_NTSC		 0 
//			 DISP_IS_1080I		 2 
//			 DISP_IS_1080I_25FPS 4 
//			 DISP_IS_720P	 	 1 
//			 DISP_IS_720P_50FPS	 5 
//			 DISP_IS_USR_DEF     7 
// 7th-8th bit: Pixel_FEEDER_ERROR Bit, 
//             error_wre[0]: Pixel Feeder FIFO o_wcnt_y == 0, 
//             error_wre[1]: Pixel Feeder FIFO o_wcnt_c == 0,
//******************************************************************/

enum _DFVC_STA0_FORMAT_
{
	DFVC_STA0_DISP_ENABLE_BIT       = 0, // Display Feeder Enable Or Not
	DFVC_STA0_VIDEO_DISP_ENABLE_BIT = 1, // Video Layer Enalbe or Not
	DFVC_STA0_DISP_ADDR_VALID_BIT   = 2,
	DFVC_STA0_DISP_VIDEO_FMT_LSB    = 4,
	DFVC_STA0_DISP_VIDEO_FMT_WIDTH  = 3, // 
	DFVC_STA0_PFEEDER_ERR_LSB         = 7,
	DFVC_STA0_PFEEDER_ERR_WIDTH       = 2, // 
};

/****************************************************************
//                DF_VIDEO_CORP_STA1  FORMAT
//---------------------------------------------------------------
// 0th       bit: vsync: Output IF Sync Info
// 2th       bit: Top/bottom id, 0-top, 1-bottom, current display field type. 
                  Not cared for frame(720P)
//  4th-15th bit: display_line_count: firmware read
// 16th-18th bit: Current CMD fifo depth: firmware read (for PTS control)
// 20th-22th bit: display_frame_buffer_ID: firmware read
// 24th-28th bit: repeat_count: firmware read, once the new command arrives, clear 0.
****************************************************************************/

enum _DFVC_STA1_FORMAT_
{
	DFVC_STA1_VSYNC_BIT               =  0, //Usage ??
	DFVC_STA1_TOP_BOT_ID_BIT          =  2, //0: Top, 1: Bottom, Not Cared for Frame
	DFVC_STA1_DISP_LINE_CNT_LSB       =  4, //Display Line Or Source Scan Line ??
	DFVC_STA1_DISP_LINE_CNT_WIDTH     = 12,
	DFVC_STA1_DISP_CMD_NUM_LSB        = 16,
	DFVC_STA1_DISP_CMD_NUM_WIDTH      =  5,
	DFVC_STA1_DISP_FRAME_BUF_ID_LSB   = 21,
	DFVC_STA1_DISP_FRAME_BUF_ID_WIDTH =  2,
	DFVC_STA1_DISP_REPEAT_CNT_LSB     = 23,  //Clear 0 when New Display Command
	DFVC_STA1_DISP_REPEAT_CNT_WIDTH   =  5,  
};

enum _DFVC_STA2_FORMAT_
{
	DFVC_STA1_DISP_FRAME_NUM_LSB      =  0,  //Clear 0 when New Display Command
	DFVC_STA1_DISP_FRAME_NUM_WIDTH    = 32,  		
};

enum
{
	SD_DDRWIDTH	= (1024),
	HD_DDRWIDTH	= (2048)
};

typedef enum 
{
	FIELD_DISP_MODE = 0,
	FRAME_DISP_MODE = 1,
}DISPLAY_MODE;

typedef enum {

	CMD_LEVEL_INITIAL   = 0,
	CMD_LEVEL_SEQUENCE  = 1,
	CMD_LEVEL_PICTURE   = 2,
	CMD_LEVEL_SLICE     = 3
	
} PIPELINE_CMD_LEVEL;

enum
{
	DF_ADDR_WORD_UNIT_LOG2   = (4) , //128bit Word Addr
	DRAM_WORD_SIZE           = (1 << DF_ADDR_WORD_UNIT_LOG2), //(16 bytes, 128 bit)
	DF_ADDR_MASK  =  0xFFFFFF0,    //28 bit Address, 128 bit Aligned
};
enum _DF_M2VD_MEM_IF_CFG_
{
	LINE_SEGMENT_SIZE  =  8, //(8 Words, 8x16 = 128 bytes)
	LINE_SEGMENT_BYTES = LINE_SEGMENT_SIZE * DRAM_WORD_SIZE, // 128
};

typedef enum _DF_VIDEO_FORMAT_
{
	NOT_STANDARD_VIDEO_MODE = -1,
	DISP_IS_PAL 	        =  3, 
	DISP_IS_NTSC		    =  0,
	DISP_IS_1080I		    =  2, 
	
	DISP_IS_1080I_25FPS     =  4, 
	DISP_IS_720P	 	    =  1, 
	DISP_IS_720P_50FPS	    =  5, 
	DISP_IS_USR_DEF         =  7,
    DISP_IS_480P            =  ((1<< 4) | 7), 
    DISP_IS_576P            =  ((2<< 4) | 7), 
}DF_VIDEO_FORMAT;

typedef struct _DF_STATUS_
{	
	I32 IsDFIdel;
	I32 IsDFCmdFifoFull;
	I32 IsDFDecodeErr;
	I32 IsDFEnable;
	I32 IsDFVideoEnable;
	I32 IsDFAddrValid;
	DF_VIDEO_FORMAT DFVideoFormat;
	U32 DFPixelFeederErr;
	I32 VSync;
	I32 IsDFTopBotField;
	I32 DispLineCnt;
	I32 DFCurCmdFifoDepth;
	I32 DFDispFrameBufID;
	I32 DFRepeatCnt;
	I32 DFAllDispFrameNum;
	U32 DFCurDispAddr;
	
}DF_STATUS;

enum _DF_OUT_SCREEN_FMT_CONST_
{
// NTSC 
	 X_TOTAL_NTSC		= (858*2) ,
	 Y_TOTAL_NTSC		= 525 ,
	 ACTIVE_X_NTSC		= (720*2), 
	 ACTIVE_Y_NTSC		= 480, 
	 SAV_START_NTSC		= (X_TOTAL_NTSC-ACTIVE_X_NTSC-4),
	 V0_START_TOP_NTSC	= 22, 
	 V0_END_TOP_NTSC    = 262 ,
	 V0_START_BOT_NTSC	= 285 ,
	 V0_END_BOT_NTSC    = 525 ,
	 F0_START_NTSC		= 3 ,
	 F0_END_NTSC		= 265, 
	 HS_START_NTSC		= (19*2),
	 HS_END_NTSC		= ((19+62) *2),
	 VS_END_TOP_NTSC    = 6,
	 VS_END_BOT_NTSC    = 268,
	 
	// PAL 
	 X_TOTAL_PAL		= (864*2) ,
	 Y_TOTAL_PAL		= 625 ,
	 ACTIVE_X_PAL		= (720*2), 
	 ACTIVE_Y_PAL		= 576 ,
	 SAV_START_PAL		= (X_TOTAL_PAL-ACTIVE_X_PAL-4) ,
	 V0_START_TOP_PAL	= 22 ,
	 V0_END_TOP_PAL		= 310 ,
	 V0_START_BOT_PAL	= 335 ,
	 V0_END_BOT_PAL		= 623 ,
	 F0_START_PAL		= 0 ,
	 F0_END_PAL		    = 312, 
	 HS_START_PAL		= (12*2),
	 HS_END_PAL		    = ((12+63)*2),
	 VS_END_TOP_PAL		= 3	,
	 VS_END_BOT_PAL		= 315,

	// 1080i at 30FPS 
	 X_TOTAL_1080I		= 2200, 
	 Y_TOTAL_1080I		= 1125 ,
	 ACTIVE_X_1080I		= 1920 ,
	 ACTIVE_Y_1080I		= 1080 ,
	 SAV_START_1080I    = (X_TOTAL_1080I-ACTIVE_X_1080I-4) ,
	 V0_START_TOP_1080I	= 20 ,
	 V0_END_TOP_1080I	= 560 ,
	 V0_START_BOT_1080I	= 583 ,
	 V0_END_BOT_1080I	= 1123 ,
	 F0_START_1080I		= 0 ,
	 F0_END_1080I		= 562, 
	 HS_START_1080I		= 88,
	 HS_END_1080I		= (88+44),
	 VS_END_TOP_1080I	= 5,
	 VS_END_BOT_1080I	= 567,

	// 1080i at 25FPS DISP_IS_1080I_25FPS
	 X_TOTAL_1080I_25FPS		= 2640 ,
	 Y_TOTAL_1080I_25FPS		= 1125 ,
	 ACTIVE_X_1080I_25FPS		= 1920 ,
	 ACTIVE_Y_1080I_25FPS		= 1080 ,
	 SAV_START_1080I_25FPS		= (X_TOTAL_1080I_25FPS-ACTIVE_X_1080I_25FPS-4) ,
	 V0_START_TOP_1080I_25FPS	= 20 ,
	 V0_END_TOP_1080I_25FPS		= 560 ,
	 V0_START_BOT_1080I_25FPS	= 583 ,
	 V0_END_BOT_1080I_25FPS		= 1123 ,
	 F0_START_1080I_25FPS		= 0 ,
	 F0_END_1080I_25FPS		    = 562, 
	 HS_START_1080I_25FPS		= 88,
	 HS_END_1080I_25FPS		    = (88+44),
	 VS_END_TOP_1080I_25FPS		= 5,
	 VS_END_BOT_1080I_25FPS		= 567,
	 
	// 720p at 60FPS 
	 X_TOTAL_720P		= 1650 ,
	 Y_TOTAL_720P		= 750 ,
	 ACTIVE_X_720P		= 1280 ,
	 ACTIVE_Y_720P		= 720 ,
	 SAV_START_720P		= (X_TOTAL_720P-ACTIVE_X_720P-4) ,
	 V0_START_720P		= 25 ,
	 V0_END_720P		= 745 ,
	 F0_START_720P		= 0 ,
	 F0_END_720P		= 750, 
	 HS_START_720P		= 110,
	 HS_END_720P		= (110+40),
	 VS_END_720P		= 5,

	// 720p at 50FPS 
	 X_TOTAL_720P_50FPS		= 1980 ,
	 Y_TOTAL_720P_50FPS		= 750 ,
	 ACTIVE_X_720P_50FPS	= 1280 ,
	 ACTIVE_Y_720P_50FPS	= 720 ,
	 SAV_START_720P_50FPS	= (X_TOTAL_720P_50FPS-ACTIVE_X_720P_50FPS-4) ,
	 V0_START_720P_50FPS	= 25 ,
	 V0_END_720P_50FPS		= 745 ,
	 F0_START_720P_50FPS	= 0 ,
	 F0_END_720P_50FPS		= 750, 
	 HS_START_720P_50FPS	= 110,
	 HS_END_720P_50FPS		= (110+40),
	 VS_END_720P_50FPS		= 5,
	 X_TOTAL_MAX            = 3000,
//480p
	 X_TOTAL_480P		= 858 ,
	 Y_TOTAL_480P		= 525 ,
	 ACTIVE_X_480P		= 720 ,
	 ACTIVE_Y_480P		= 483 ,
	 SAV_START_480P		= (X_TOTAL_480P-ACTIVE_X_480P-4) ,
	 V0_START_480P		= 25 ,
	 V0_END_480P		= V0_START_480P +  ACTIVE_Y_480P,
	 F0_START_480P		= 0 ,
	 F0_END_480P		= Y_TOTAL_480P, 
	 HS_START_480P		= 110,
	 HS_END_480P		= (HS_START_480P+40),
	 VS_END_480P		= 5,
//576p
	 X_TOTAL_576P		= 864 ,
	 Y_TOTAL_576P		= 625 ,
	 ACTIVE_X_576P		= 720 ,
	 ACTIVE_Y_576P		= 576 ,
	 SAV_START_576P		= (X_TOTAL_576P-ACTIVE_X_576P-4) ,
	 V0_START_576P		= 25 ,
	 V0_END_576P		= V0_START_576P +  ACTIVE_Y_576P,
	 F0_START_576P		= 0 ,
	 F0_END_576P		= Y_TOTAL_576P, 
	 HS_START_576P		= 110,
	 HS_END_576P		= (HS_START_576P+40),
	 VS_END_576P		= 5,	
};

typedef struct _DF_OUT_SCREEN_PARA_
{
	U32 XTotal;
	U32 YTotal;
	U32 ActiveX;
	U32 ActiveY;
	U32 SavStart;
	U32 V0StartTop;
	U32 V0EndTop;	
	U32 V0StartBot;
	U32 V0EndBot;
	U32 F0Start;
	U32 F0End;
	
}DF_OUT_SCREEN_PARA;


#endif

