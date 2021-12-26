#ifndef __ORION_XPORT_H__
#define __ORION_XPORT_H__


#define XPORT_CFG1_OUT_CHL0_LINE_SYNC   0x00000001
#define XPORT_CFG1_OUT_CHL1_LINE_SYNC   0x00000002
#define XPORT_CFG1_OUT_CHL2_LINE_SYNC   0x00000004
#define XPORT_CFG1_OUT_CHL3_LINE_SYNC   0x00000008
/* Firmware Instruction and data address*/
#define  MIPS_FW_WRITE_DATA  		(0x41400000+(0x4000<<2))
#define  MIPS_FW_WRITE_INST  		(0x41400000+(0x2000<<2))
#define  MIPS_FW_EN   			(0x41400000+(0x6000<<2))

/* memory map*/
#define XPORT_MEM_BASE					0xA00000	// xport memory base address at 16M
#define XPORT_CHL0_BASE_ADDR_DEF    	XPORT_MEM_BASE
#define XPORT_CHL0_UNIT_SIZE_DEF    	400	// 400 bytes
#define XPORT_CHL0_UNIT_NUM_DEF     	320
#define XPORT_CHL0_CFG_DEF          	((XPORT_CHL0_UNIT_NUM_DEF<<8) | (XPORT_CHL0_UNIT_SIZE_DEF>>3))
#define XPORT_CHL0_RP_DEF           	0
#define XPORT_CHL0_WP_DEF           	0

#define XPORT_CHL1_BASE_ADDR_DEF    	(XPORT_CHL0_BASE_ADDR_DEF+0x20000)
#define XPORT_CHL1_UNIT_SIZE_DEF    	400	// 400 bytes
#define XPORT_CHL1_UNIT_NUM_DEF     	320
#define XPORT_CHL1_CFG_DEF          	((XPORT_CHL1_UNIT_NUM_DEF<<8) | (XPORT_CHL1_UNIT_SIZE_DEF>>3))
#define XPORT_CHL1_RP_DEF           	0
#define XPORT_CHL1_WP_DEF           	0

#define FILTER0_BUF_BASE_ADDR		(XPORT_CHL1_BASE_ADDR_DEF+0x20000)
#define FILTER0_BUF_SIZE          	0x20000 // 128k
#define FILTER1_BUF_BASE_ADDR     	(FILTER0_BUF_BASE_ADDR + FILTER0_BUF_SIZE)
#define FILTER1_BUF_SIZE          	0x20000	// 128k
#define FILTER2_BUF_BASE_ADDR     	(FILTER1_BUF_BASE_ADDR + FILTER1_BUF_SIZE)
#define FILTER2_BUF_SIZE          	0x20000	// 128k
#define FILTER3_BUF_BASE_ADDR     	(FILTER2_BUF_BASE_ADDR + FILTER2_BUF_SIZE)
#define FILTER3_BUF_SIZE          	0x20000	// 128k
#define FILTER4_BUF_BASE_ADDR     	(FILTER3_BUF_BASE_ADDR + FILTER3_BUF_SIZE)
#define FILTER4_BUF_SIZE          	0x20000	// 128k
#define FILTER5_BUF_BASE_ADDR     	(FILTER4_BUF_BASE_ADDR + FILTER4_BUF_SIZE)
#define FILTER5_BUF_SIZE          	0x20000	// 128k
#define FILTER6_BUF_BASE_ADDR     	(FILTER5_BUF_BASE_ADDR + FILTER5_BUF_SIZE)
#define FILTER6_BUF_SIZE          	0x20000	// 128k
#define FILTER7_BUF_BASE_ADDR     	(FILTER6_BUF_BASE_ADDR + FILTER6_BUF_SIZE)
#define FILTER7_BUF_SIZE          	0x20000	// 128k
#define FILTER8_BUF_BASE_ADDR     	(FILTER7_BUF_BASE_ADDR + FILTER7_BUF_SIZE)
#define FILTER8_BUF_SIZE          	0x20000	// 128k
#define FILTER9_BUF_BASE_ADDR     	(FILTER8_BUF_BASE_ADDR + FILTER8_BUF_SIZE)
#define FILTER9_BUF_SIZE          	0x20000	// 128k
#define FILTER10_BUF_BASE_ADDR    	(FILTER9_BUF_BASE_ADDR + FILTER9_BUF_SIZE)
#define FILTER10_BUF_SIZE         	0x10000	// 64k
#define FILTER11_BUF_BASE_ADDR    	(FILTER10_BUF_BASE_ADDR+FILTER10_BUF_SIZE)
#define FILTER11_BUF_SIZE         	0x10000	// 64k
#define FILTER12_BUF_BASE_ADDR    	(FILTER11_BUF_BASE_ADDR+FILTER11_BUF_SIZE)
#define FILTER12_BUF_SIZE         	0x10000	// 64k
#define FILTER13_BUF_BASE_ADDR    	(FILTER12_BUF_BASE_ADDR+FILTER12_BUF_SIZE)
#define FILTER13_BUF_SIZE         	0x10000	// 64k
#define FILTER14_BUF_BASE_ADDR    	(FILTER13_BUF_BASE_ADDR+FILTER13_BUF_SIZE)
#define FILTER14_BUF_SIZE         	0x10000	// 64k
#define FILTER15_BUF_BASE_ADDR    	(FILTER14_BUF_BASE_ADDR+FILTER14_BUF_SIZE)
#define FILTER15_BUF_SIZE         	0x10000 // 64k
#define FILTER16_BUF_BASE_ADDR    	(FILTER15_BUF_BASE_ADDR+FILTER15_BUF_SIZE)
#define FILTER16_BUF_SIZE         	0x10000	// 64k
#define FILTER17_BUF_BASE_ADDR    	(FILTER16_BUF_BASE_ADDR+FILTER16_BUF_SIZE)
#define FILTER17_BUF_SIZE         	0x10000	// 64k
#define FILTER18_BUF_BASE_ADDR    	(FILTER17_BUF_BASE_ADDR+FILTER17_BUF_SIZE)
#define FILTER18_BUF_SIZE         	0x10000	// 64k
#define FILTER19_BUF_BASE_ADDR    	(FILTER18_BUF_BASE_ADDR+FILTER18_BUF_SIZE)
#define FILTER19_BUF_SIZE         	0x10000	// 64k
#define FILTER20_BUF_BASE_ADDR    	(FILTER19_BUF_BASE_ADDR+FILTER19_BUF_SIZE)
#define FILTER20_BUF_SIZE         	0x10000	// 64k
#define FILTER21_BUF_BASE_ADDR    	(FILTER20_BUF_BASE_ADDR+FILTER20_BUF_SIZE)
#define FILTER21_BUF_SIZE         	0x10000	// 64k
#define FILTER22_BUF_BASE_ADDR    	(FILTER21_BUF_BASE_ADDR+FILTER21_BUF_SIZE)
#define FILTER22_BUF_SIZE         	0x10000	// 64k
#define FILTER23_BUF_BASE_ADDR    	(FILTER22_BUF_BASE_ADDR+FILTER22_BUF_SIZE)
#define FILTER23_BUF_SIZE         	0x10000	// 64k
#define FILTER24_BUF_BASE_ADDR    	(FILTER23_BUF_BASE_ADDR+FILTER23_BUF_SIZE)
#define FILTER24_BUF_SIZE         	0x10000	// 64k
#define FILTER25_BUF_BASE_ADDR    	(FILTER24_BUF_BASE_ADDR+FILTER24_BUF_SIZE)
#define FILTER25_BUF_SIZE         	0x10000	// 64k
#define FILTER26_BUF_BASE_ADDR    	(FILTER25_BUF_BASE_ADDR+FILTER25_BUF_SIZE)
#define FILTER26_BUF_SIZE         	0x10000	// 64k
#define FILTER27_BUF_BASE_ADDR    	(FILTER26_BUF_BASE_ADDR+FILTER26_BUF_SIZE)
#define FILTER27_BUF_SIZE         	0x10000	// 64k
#define FILTER28_BUF_BASE_ADDR    	(FILTER27_BUF_BASE_ADDR+FILTER27_BUF_SIZE)
#define FILTER28_BUF_SIZE         	0x10000	// 64k
#define FILTER29_BUF_BASE_ADDR    	(FILTER28_BUF_BASE_ADDR+FILTER28_BUF_SIZE)
#define FILTER29_BUF_SIZE         	0x10000	// 64k
#define FILTER30_BUF_BASE_ADDR    	(FILTER29_BUF_BASE_ADDR+FILTER29_BUF_SIZE)
#define FILTER30_BUF_SIZE         	0x10000	// 64k
#define FILTER31_BUF_BASE_ADDR    	(FILTER30_BUF_BASE_ADDR+FILTER30_BUF_SIZE)
#define FILTER31_BUF_SIZE         	0x10000 // 64k

/* Register Map */
// xport_reg_pack block  addr: 0x00-0x1f
#define MAIL_BOX0_ADDR              (CFG_XPORT_BASE+(0x0000<<2))
#define MAIL_BOX1_ADDR              (CFG_XPORT_BASE+(0x0001<<2))
#define MAIL_BOX2_ADDR              (CFG_XPORT_BASE+(0x0002<<2))
#define MAIL_BOX3_ADDR              (CFG_XPORT_BASE+(0x0003<<2))
#define MAIL_BOX4_ADDR              (CFG_XPORT_BASE+(0x0004<<2))
#define MAIL_BOX5_ADDR              (CFG_XPORT_BASE+(0x0005<<2))
#define MAIL_BOX6_ADDR              (CFG_XPORT_BASE+(0x0006<<2))
#define MAIL_BOX7_ADDR              (CFG_XPORT_BASE+(0x0007<<2))

// bit0:   tuner0_mode,         0: Parallel mode    1: Serial
// bit1:   tuner1_mode,         0: Parallel mode    1: Serial
// bit2:   dma_in0_mode,        0: Normal DMA       1: DMA To Tuner
// bit4_3: dma_in0_speed_mode
// bit5:   dma_in1_mode,        0: Normal DMA       1: DMA To Tuner
// bit7_6: dma_in1_speed_mode
#define XPORT_CFG_ADDR0             (CFG_XPORT_BASE+(0x0008<<2))

// bit3_0:     av_rp sync_mode,    0: host sync    1: line sync
#define XPORT_CFG_ADDR1             (CFG_XPORT_BASE+(0x0009<<2))

// bit0:  dma0 end interrupt
// bit1:  dma1 end interrupt                                                         
#define XPORT_INT_REG_ADDR0         (CFG_XPORT_BASE+(0x0010<<2))
#define XPORT_INT_ENB_ADDR0         (CFG_XPORT_BASE+(0x0011<<2))
#define XPORT_INT_SET_ADDR0         (CFG_XPORT_BASE+(0x0012<<2))
#define XPORT_INT_CLS_ADDR0         (CFG_XPORT_BASE+(0x0013<<2))

#define XPORT_INT_REG_ADDR1         (CFG_XPORT_BASE+(0x0014<<2))
#define XPORT_INT_ENB_ADDR1         (CFG_XPORT_BASE+(0x0015<<2))
#define XPORT_INT_SET_ADDR1         (CFG_XPORT_BASE+(0x0016<<2))
#define XPORT_INT_CLS_ADDR1         (CFG_XPORT_BASE+(0x0017<<2))

#define  XPORT_TUNER_EN             (CFG_XPORT_BASE+(0x0018<<2))

#define XPORT_CHL0_BASE_ADDR            (CFG_XPORT_BASE+(0x0040<<2))
#define XPORT_CHL0_CFG_ADDR             (CFG_XPORT_BASE+(0x0041<<2))
#define XPORT_CHL0_RP_ADDR              (CFG_XPORT_BASE+(0x0042<<2))
#define XPORT_CHL0_WP_ADDR              (CFG_XPORT_BASE+(0x0043<<2))
#define XPORT_CHL0_SEQ_ADDR             (CFG_XPORT_BASE+(0x0044<<2))
#define XPORT_CHL0_STATUS_ADDR          (CFG_XPORT_BASE+(0x0045<<2))

#define XPORT_CHL1_BASE_ADDR            (CFG_XPORT_BASE+(0x0046<<2))
#define XPORT_CHL1_CFG_ADDR             (CFG_XPORT_BASE+(0x0047<<2))
#define XPORT_CHL1_RP_ADDR              (CFG_XPORT_BASE+(0x0048<<2))
#define XPORT_CHL1_WP_ADDR              (CFG_XPORT_BASE+(0x0049<<2))
#define XPORT_CHL1_SEQ_ADDR             (CFG_XPORT_BASE+(0x004a<<2))
#define XPORT_CHL1_STATUS_ADDR          (CFG_XPORT_BASE+(0x004b<<2))

#define XPORT_CHL2_BASE_ADDR            (CFG_XPORT_BASE+(0x004c<<2))
#define XPORT_CHL2_CFG_ADDR             (CFG_XPORT_BASE+(0x004d<<2))
#define XPORT_CHL2_RP_ADDR              (CFG_XPORT_BASE+(0x004e<<2))
#define XPORT_CHL2_WP_ADDR              (CFG_XPORT_BASE+(0x004f<<2))
#define XPORT_CHL2_SEQ_ADDR             (CFG_XPORT_BASE+(0x0050<<2))
#define XPORT_CHL2_STATUS_ADDR          (CFG_XPORT_BASE+(0x0051<<2))

#define XPORT_CHL3_BASE_ADDR            (CFG_XPORT_BASE+(0x0052<<2))
#define XPORT_CHL3_CFG_ADDR             (CFG_XPORT_BASE+(0x0053<<2))
#define XPORT_CHL3_RP_ADDR              (CFG_XPORT_BASE+(0x0054<<2))
#define XPORT_CHL3_WP_ADDR              (CFG_XPORT_BASE+(0x0055<<2))
#define XPORT_CHL3_SEQ_ADDR             (CFG_XPORT_BASE+(0x0056<<2))
#define XPORT_CHL3_STATUS_ADDR          (CFG_XPORT_BASE+(0x0057<<2))

/*xunli: direct dma mode */
#define XPORT_CHL_DMA0_WP_ADDR          (MAIL_BOX2_ADDR)
#define XPORT_CHL_DMA1_WP_ADDR          (MAIL_BOX3_ADDR)

// PID filter reg  addr:  0x60-0x9f
#define __PID_FILTER__(i)             (CFG_XPORT_BASE+((0x0060 + i)<<2))

#define PID_FILTER0                   (CFG_XPORT_BASE+(0x0060<<2))
#define PID_FILTER1                   (CFG_XPORT_BASE+(0x0061<<2))
#define PID_FILTER2                   (CFG_XPORT_BASE+(0x0062<<2))
#define PID_FILTER3                   (CFG_XPORT_BASE+(0x0063<<2))
#define PID_FILTER4                   (CFG_XPORT_BASE+(0x0064<<2))
#define PID_FILTER5                   (CFG_XPORT_BASE+(0x0065<<2))
#define PID_FILTER6                   (CFG_XPORT_BASE+(0x0066<<2))
#define PID_FILTER7                   (CFG_XPORT_BASE+(0x0067<<2))
#define PID_FILTER8                   (CFG_XPORT_BASE+(0x0068<<2))
#define PID_FILTER9                   (CFG_XPORT_BASE+(0x0069<<2))
#define PID_FILTER10                  (CFG_XPORT_BASE+(0x006a<<2))
#define PID_FILTER11                  (CFG_XPORT_BASE+(0x006b<<2))
#define PID_FILTER12                  (CFG_XPORT_BASE+(0x006c<<2))
#define PID_FILTER13                  (CFG_XPORT_BASE+(0x006d<<2))
#define PID_FILTER14                  (CFG_XPORT_BASE+(0x006e<<2))
#define PID_FILTER15                  (CFG_XPORT_BASE+(0x006f<<2))
#define PID_FILTER16                  (CFG_XPORT_BASE+(0x0070<<2))
#define PID_FILTER17                  (CFG_XPORT_BASE+(0x0071<<2))
#define PID_FILTER18                  (CFG_XPORT_BASE+(0x0072<<2))
#define PID_FILTER19                  (CFG_XPORT_BASE+(0x0073<<2))
#define PID_FILTER20                  (CFG_XPORT_BASE+(0x0074<<2))
#define PID_FILTER21                  (CFG_XPORT_BASE+(0x0075<<2))
#define PID_FILTER22                  (CFG_XPORT_BASE+(0x0076<<2))
#define PID_FILTER23                  (CFG_XPORT_BASE+(0x0077<<2))
#define PID_FILTER24                  (CFG_XPORT_BASE+(0x0078<<2))
#define PID_FILTER25                  (CFG_XPORT_BASE+(0x0079<<2))
#define PID_FILTER26                  (CFG_XPORT_BASE+(0x007a<<2))
#define PID_FILTER27                  (CFG_XPORT_BASE+(0x007b<<2))
#define PID_FILTER28                  (CFG_XPORT_BASE+(0x007c<<2))
#define PID_FILTER29                  (CFG_XPORT_BASE+(0x007d<<2))
#define PID_FILTER30                  (CFG_XPORT_BASE+(0x007e<<2))
#define PID_FILTER31                  (CFG_XPORT_BASE+(0x007f<<2))
#define PID_FILTER32                  (CFG_XPORT_BASE+(0x0080<<2))
#define PID_FILTER33                  (CFG_XPORT_BASE+(0x0081<<2))
#define PID_FILTER34                  (CFG_XPORT_BASE+(0x0082<<2))
#define PID_FILTER35                  (CFG_XPORT_BASE+(0x0083<<2))
#define PID_FILTER36                  (CFG_XPORT_BASE+(0x0084<<2))
#define PID_FILTER37                  (CFG_XPORT_BASE+(0x0085<<2))
#define PID_FILTER38                  (CFG_XPORT_BASE+(0x0086<<2))
#define PID_FILTER39                  (CFG_XPORT_BASE+(0x0087<<2))
#define PID_FILTER40                  (CFG_XPORT_BASE+(0x0088<<2))
#define PID_FILTER41                  (CFG_XPORT_BASE+(0x0089<<2))
#define PID_FILTER42                  (CFG_XPORT_BASE+(0x008a<<2))
#define PID_FILTER43                  (CFG_XPORT_BASE+(0x008b<<2))
#define PID_FILTER44                  (CFG_XPORT_BASE+(0x008c<<2))
#define PID_FILTER45                  (CFG_XPORT_BASE+(0x008d<<2))
#define PID_FILTER46                  (CFG_XPORT_BASE+(0x008e<<2))
#define PID_FILTER47                  (CFG_XPORT_BASE+(0x008f<<2))
#define PID_FILTER48                  (CFG_XPORT_BASE+(0x0090<<2))
#define PID_FILTER49                  (CFG_XPORT_BASE+(0x0091<<2))
#define PID_FILTER50                  (CFG_XPORT_BASE+(0x0092<<2))
#define PID_FILTER51                  (CFG_XPORT_BASE+(0x0093<<2))
#define PID_FILTER52                  (CFG_XPORT_BASE+(0x0094<<2))
#define PID_FILTER53                  (CFG_XPORT_BASE+(0x0095<<2))
#define PID_FILTER54                  (CFG_XPORT_BASE+(0x0096<<2))
#define PID_FILTER55                  (CFG_XPORT_BASE+(0x0097<<2))
#define PID_FILTER56                  (CFG_XPORT_BASE+(0x0098<<2))
#define PID_FILTER57                  (CFG_XPORT_BASE+(0x0099<<2))
#define PID_FILTER58                  (CFG_XPORT_BASE+(0x009a<<2))
#define PID_FILTER59                  (CFG_XPORT_BASE+(0x009b<<2))
#define PID_FILTER60                  (CFG_XPORT_BASE+(0x009c<<2))
#define PID_FILTER61                  (CFG_XPORT_BASE+(0x009d<<2))
#define PID_FILTER62                  (CFG_XPORT_BASE+(0x009e<<2))
#define PID_FILTER63                  (CFG_XPORT_BASE+(0x009f<<2))

/**/
#define MIPS_CMD_REQ 	MAIL_BOX0_ADDR
#define MIPS_CMD_DATA	MAIL_BOX1_ADDR

#define  FILTER_EN_ADDR(x)                   (0x0400+((x)<<8))
#define  FILTER_FILTER0_ADDR(x)              (0x0401+((x)<<8))
#define  FILTER_FILTER1_ADDR(x)              (0x0402+((x)<<8))
#define  FILTER_FILTER2_ADDR(x)              (0x0403+((x)<<8))
#define  FILTER_MASK0_ADDR(x)                (0x0404+((x)<<8))
#define  FILTER_MASK1_ADDR(x)                (0x0405+((x)<<8))
#define  FILTER_MASK2_ADDR(x)		     (0x0406+((x)<<8))
#define  FILTER_BUF_LOW_ADDR(x)              (0x0407+((x)<<8))
#define  FILTER_BUF_UP_ADDR(x)               (0x0408+((x)<<8))
#define  FILTER_BUF_WP_ADDR(x)               (0x41400000+(0x0110<<2)+((x)<<3))
#define  FILTER_BUF_RP_ADDR(x)               (0x41400000+(0x0111<<2)+((x)<<3))

#define  FILTER_PID_ADDR(x)                 (0x0416+((x)<<8))

#define  FILTER_DISABLE_ADDR(x)		     (0x0436+((x)<<8))

/*MIPS*/
/******************MIPS software Register define start************************/
#define MIPS_OUTCHL_EN                   0x0000
#define MIPS_OUTCHL_FILTER0              0x0001
#define MIPS_OUTCHL_FILTER1              0x0002
#define MIPS_OUTCHL_FILTER2              0x0003
#define MIPS_OUTCHL_MASK0                0x0004
#define MIPS_OUTCHL_MASK1                0x0005
#define MIPS_OUTCHL_MASK2                0x0006
#define MIPS_OUTCHL_DIR_LOW_ADRR         0x0007
#define MIPS_OUTCHL_DIR_UP_ADRR          0x0008
#define MIPS_OUTCHL_DIR_WP               0x0009
#define MIPS_OUTCHL_BUF_TC0_ERR_CNT      0x000a
#define MIPS_OUTCHL_BUF_TC1_ERR_CNT      0x000b
#define MIPS_OUTCHL_BUF_TC2_ERR_CNT      0x000c
#define MIPS_OUTCHL_BUF_TC3_ERR_CNT      0x000d
#define MIPS_OUTCHL_BUF_ERR_PACKET_CNT   0x000e
#define MIPS_OUTCHL_BUF_OUT_PACKET_CNT   0x000f
#define MIPS_OUTCHL_BUF_LOW_ADRR         0x0010
#define MIPS_OUTCHL_BUF_UP_ADRR          0x0011
#define MIPS_PCR_PID_ADDR                0x0012
#define MIPS_PCR_GET_ADDR                0x0013
#define MIPS_OUTCHL_CRC_EN               0x0014
#define MIPS_OUTCHL_CRC_NOTIFY_EN        0x0015

#define MIPS_OUTCHL_PID_BASE             0x0016

#define MIPS_OUTCHL_PID0                 (MIPS_OUTCHL_PID_BASE + 0x0)
#define MIPS_OUTCHL_PID1                 (MIPS_OUTCHL_PID_BASE + 0x1)
#define MIPS_OUTCHL_PID2                 (MIPS_OUTCHL_PID_BASE + 0x2)
#define MIPS_OUTCHL_PID3                 (MIPS_OUTCHL_PID_BASE + 0x3)
#define MIPS_OUTCHL_PID4                 (MIPS_OUTCHL_PID_BASE + 0x4)
#define MIPS_OUTCHL_PID5                 (MIPS_OUTCHL_PID_BASE + 0x5)
#define MIPS_OUTCHL_PID6                 (MIPS_OUTCHL_PID_BASE + 0x6)
#define MIPS_OUTCHL_PID7                 (MIPS_OUTCHL_PID_BASE + 0x7)
#define MIPS_OUTCHL_PID8                 (MIPS_OUTCHL_PID_BASE + 0x8)
#define MIPS_OUTCHL_PID9                 (MIPS_OUTCHL_PID_BASE + 0x9)
#define MIPS_OUTCHL_PID10                (MIPS_OUTCHL_PID_BASE + 0xa)
#define MIPS_OUTCHL_PID11                (MIPS_OUTCHL_PID_BASE + 0xb)

#define MIPS_OUTCHL_DISABLE		 (MIPS_OUTCHL_PID_BASE + 0x20)
/******************MIPS software Register define end***********************/

#define MIPS_CHL_EN(x)                  (MIPS_OUTCHL_EN+((x)<<8))
#define MIPS_CHL_PID0(x)                (MIPS_OUTCHL_PID0+((x)<<8))
#define MIPS_CHL_PID1(x)                (MIPS_OUTCHL_PID1+((x)<<8))
#define MIPS_CHL_PID2(x)                (MIPS_OUTCHL_PID2+((x)<<8))
#define MIPS_CHL_PID3(x)                (MIPS_OUTCHL_PID3+((x)<<8))
#define MIPS_CHL_PID4(x)                (MIPS_OUTCHL_PID4+((x)<<8))
#define MIPS_CHL_PID5(x)                (MIPS_OUTCHL_PID5+((x)<<8))
#define MIPS_CHL_PID6(x)                (MIPS_OUTCHL_PID6+((x)<<8))
#define MIPS_CHL_PID7(x)                (MIPS_OUTCHL_PID7+((x)<<8))
#define MIPS_CHL_PID8(x)                (MIPS_OUTCHL_PID8+((x)<<8))
#define MIPS_CHL_PID9(x)                (MIPS_OUTCHL_PID9+((x)<<8))
#define MIPS_CHL_PID10(x)               (MIPS_OUTCHL_PID10+((x)<<8))
#define MIPS_CHL_PID11(x)               (MIPS_OUTCHL_PID11+((x)<<8))
#define MIPS_CHL_FILTER0(x)             (MIPS_OUTCHL_FILTER0+((x)<<8))
#define MIPS_CHL_FILTER1(x)             (MIPS_OUTCHL_FILTER1+((x)<<8))
#define MIPS_CHL_FILTER2(x)             (MIPS_OUTCHL_FILTER2+((x)<<8))
#define MIPS_CHL_MASK0(x)               (MIPS_OUTCHL_MASK0+((x)<<8))
#define MIPS_CHL_MASK1(x)               (MIPS_OUTCHL_MASK1+((x)<<8))
#define MIPS_CHL_MASK2(x)               (MIPS_OUTCHL_MASK2+((x)<<8))
#define MIPS_CHL_DIR_LOW_ADDR(x)        (MIPS_OUTCHL_DIR_LOW_ADRR+((x)<<8))
#define MIPS_CHL_DIR_UP_ADDR(x)         (MIPS_OUTCHL_DIR_UP_ADRR+((x)<<8))
#define MIPS_CHL_WP(x)                  (MIPS_OUTCHL_DIR_WP+((x)<<8))
#define MIPS_CHL_TC0_ERR_CNT(x)         (MIPS_OUTCHL_BUF_TC0_ERR_CNT+((x)<<8))
#define MIPS_CHL_TC1_ERR_CNT(x)         (MIPS_OUTCHL_BUF_TC1_ERR_CNT+((x)<<8))
#define MIPS_CHL_TC2_ERR_CNT(x)         (MIPS_OUTCHL_BUF_TC2_ERR_CNT+((x)<<8))
#define MIPS_CHL_TC3_ERR_CNT(x)         (MIPS_OUTCHL_BUF_TC3_ERR_CNT+((x)<<8))
#define MIPS_CHL_ERR_PKT_CNT(x)         (MIPS_OUTCHL_BUF_ERR_PACKET_CNT+((x)<<8))
#define MIPS_CHL_OUT_PKT_CNT(x)         (MIPS_OUTCHL_BUF_OUT_PACKET_CNT+((x)<<8))
#define MIPS_CHL_BUF_LOW_ADDR(x)        (MIPS_OUTCHL_BUF_LOW_ADRR+((x)<<8))
#define MIPS_CHL_BUF_UP_ADDR(x)         (MIPS_OUTCHL_BUF_UP_ADRR+((x)<<8))
#define MIPS_PCR_PID(x)                 (MIPS_PCR_PID_ADDR+((x)<<8))
#define MIPS_PCR_GET(x)                 (MIPS_PCR_GET_ADDR+((x)<<8))
#define MIPS_CHL_CRC_EN(x)              (MIPS_OUTCHL_CRC_EN+((x)<<8))
#define MIPS_CHL_CRC_NOTIFY_EN(x)       (MIPS_OUTCHL_CRC_NOTIFY_EN+((x)<<8))
#define MIPS_CHL_DISABLE(x)		(MIPS_OUTCHL_DISABLE + ((x)<<8))

#endif


