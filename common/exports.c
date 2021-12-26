#include <common.h>
#include <exports.h>

DECLARE_GLOBAL_DATA_PTR;

static void dummy(void)
{
}

unsigned long get_version(void)
{
	return XF_VERSION;
}

void jumptable_init (void)
{
	int i;

	gd->jt = (void **) malloc (XF_MAX * sizeof (void *));
	for (i = 0; i < XF_MAX; i++)
		gd->jt[i] = (void *) dummy;

	gd->jt[XF_get_version] = (void *) get_version;
	gd->jt[XF_malloc] = (void *) malloc;
	gd->jt[XF_free] = (void *) free;
	gd->jt[XF_getenv] = (void *) getenv;
	gd->jt[XF_setenv] = (void *) setenv;
	gd->jt[XF_get_timer] = (void *) get_timer;
	gd->jt[XF_simple_strtoul] = (void *) simple_strtoul;
	gd->jt[XF_udelay] = (void *) udelay;

#if defined(CONFIG_I386) || defined(CONFIG_PPC)
	gd->jt[XF_install_hdlr] = (void *) irq_install_handler;
	gd->jt[XF_free_hdlr] = (void *) irq_free_handler;
#endif	/* I386 || PPC */
    
#if (CONFIG_COMMANDS & CFG_CMD_I2C)
	gd->jt[XF_i2c_write] = (void *) i2c_write;
	gd->jt[XF_i2c_read] = (void *) i2c_read;
#endif /*CFG_CMD_I2C */

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
    gd->jt[XF_eeprom_read] = (void *) eeprom_read;
    gd->jt[XF_eeprom_write] = (void *) eeprom_write;
#endif 

#if defined(CONFIG_CELESTIAL)

#ifdef CONFIG_USE_IRQ 
    gd->jt[XF_orion_register_irq] = (void *) orion_register_irq;
    gd->jt[XF_orion_unregister_irq] = (void *) orion_unregister_irq;
    gd->jt[XF_orion_unregister_irq] = (void *) orion_unregister_irq;
    gd->jt[XF_orion_irq_ack] = (void *) orion_irq_ack;
    gd->jt[XF_orion_irq_enable] = (void *) orion_irq_enable;
    gd->jt[XF_orion_irq_disable] = (void *) orion_irq_disable;
#endif /* IRQ*/

/* Old API */
	gd->jt[XF_xport_init] = (void *) xport_init;
	gd->jt[XF_xport_set_channel_status] = (void *) xport_set_channel_status;
	gd->jt[XF_xport_set_channel_inputmode] = (void *) xport_set_channel_inputmode;
	gd->jt[XF_xport_pidft_bind_chl] = (void *) xport_pidft_bind_chl;
	gd->jt[XF_xport_pidft_set_pid] = (void *) xport_pidft_set_pid;
	gd->jt[XF_xport_pidft_set_status] = (void *) xport_pidft_set_status;
	gd->jt[XF_xport_filter_set_filter] = (void *) xport_filter_set_filter;
	gd->jt[XF_xport_filter_set_pid] = (void *) xport_filter_set_pid;
	gd->jt[XF_xport_filter_set_type] = (void *) xport_filter_set_type;
	gd->jt[XF_xport_filter_set_status] = (void *) xport_filter_set_status;
	gd->jt[XF_xport_filter_wait_data] = (void *) xport_filter_wait_data;
	gd->jt[XF_xport_fliter_read_data] = (void *) xport_fliter_read_data;
/* New API */
	gd->jt[XF_CSDEMUX_Init] = (void *) CSDEMUX_Init;
	gd->jt[XF_CSDEMUX_CHL_Open] = (void *) CSDEMUX_CHL_Open;
	gd->jt[XF_CSDEMUX_CHL_Close] = (void *) CSDEMUX_CHL_Close;
	gd->jt[XF_CSDEMUX_CHL_SetInputMode] = (void *) CSDEMUX_CHL_SetInputMode;
	gd->jt[XF_CSDEMUX_CHL_Enable] = (void *) CSDEMUX_CHL_Enable;
	gd->jt[XF_CSDEMUX_CHL_Disable] = (void *) CSDEMUX_CHL_Disable;
	gd->jt[XF_CSDEMUX_PIDFT_Open] = (void *) CSDEMUX_PIDFT_Open;
	gd->jt[XF_CSDEMUX_PIDFT_Close] = (void *) CSDEMUX_PIDFT_Close;
	gd->jt[XF_CSDEMUX_PIDFT_SetChannel] = (void *) CSDEMUX_PIDFT_SetChannel;
	gd->jt[XF_CSDEMUX_PIDFT_SetPID] = (void *) CSDEMUX_PIDFT_SetPID;
	gd->jt[XF_CSDEMUX_PIDFT_Enable] = (void *) CSDEMUX_PIDFT_Enable;
	gd->jt[XF_CSDEMUX_PIDFT_Disable] = (void *) CSDEMUX_PIDFT_Disable;
	gd->jt[XF_CSDEMUX_Filter_Open] = (void *) CSDEMUX_Filter_Open;
	gd->jt[XF_CSDEMUX_Filter_Close] = (void *) CSDEMUX_Filter_Close;
	gd->jt[XF_CSDEMUX_Filter_SetFilterType] = (void *) CSDEMUX_Filter_SetFilterType;
	gd->jt[XF_CSDEMUX_Filter_SetFilter] = (void *) CSDEMUX_Filter_SetFilter;
	gd->jt[XF_CSDEMUX_Filter_AddPID] = (void *) CSDEMUX_Filter_AddPID;
	gd->jt[XF_CSDEMUX_Filter_ResetPID] = (void *) CSDEMUX_Filter_ResetPID;
	gd->jt[XF_CSDEMUX_Filter_ReadWait] = (void *) CSDEMUX_Filter_ReadWait;
	gd->jt[XF_CSDEMUX_Filter_CheckDataSize] = (void *) CSDEMUX_Filter_CheckDataSize;
	gd->jt[XF_CSDEMUX_Filter_GetSectionCount] = (void *) CSDEMUX_Filter_GetSectionCount;
	gd->jt[XF_CSDEMUX_Filter_ReadSectionData] = (void *) CSDEMUX_Filter_ReadSectionData;
	gd->jt[XF_CSDEMUX_Filter_Enable] = (void *) CSDEMUX_Filter_Enable;
	gd->jt[XF_CSDEMUX_Filter_Disable] = (void *) CSDEMUX_Filter_Disable;

	gd->jt[XF_CSTUNER_Init] = (void *) CSTUNER_Init;
	gd->jt[XF_CSTUNER_Open] = (void *) CSTUNER_Open;
	gd->jt[XF_CSTUNER_SetFrequency] = (void *) CSTUNER_SetFrequency;
	gd->jt[XF_CSTUNER_GetTunerInfo] = (void *) CSTUNER_GetTunerInfo;
	gd->jt[XF_CSTUNER_ReadStatus] = (void *) CSTUNER_ReadStatus;

#ifndef MERIH_GOLD_BOARD  // by kb :20100416
	gd->jt[XF_fp_set_led_display_pos] = (void *) fp_set_led_display_pos;
	gd->jt[XF_fp_set_led_dispaly_mode] = (void *) fp_set_led_dispaly_mode;
	gd->jt[XF_fp_set_led_dispaly_char] = (void *) fp_set_led_dispaly_char;
	gd->jt[XF_fp_set_led_display_raw] = (void *) fp_set_led_display_raw;
	gd->jt[XF_fp_enable_keyscan] = (void *) fp_enable_keyscan;
	gd->jt[XF_fp_disable_keyscan] = (void *) fp_disable_keyscan;
	gd->jt[XF_fp_set_keyscan_map] = (void *) fp_set_keyscan_map;
	gd->jt[XF_fp_get_keyscan_input] = (void *) fp_get_keyscan_input;
#endif // #ifndef MERIH_GOLD_BOARD
// 	gd->jt[XF_fp_enable_serial_in] = (void *) fp_enable_serial_in;
// 	gd->jt[XF_fp_disable_serial_in] = (void *) fp_disable_serial_in;
// 	gd->jt[XF_fp_reset_serial_in] = (void *) fp_reset_serial_in;
// 	gd->jt[XF_fp_set_serial_in_attr] = (void *) fp_set_serial_in_attr;
// 	gd->jt[XF_fp_write_serial_in] = (void *) fp_write_serial_in;

    gd->jt[XF_cs_uart1_init] = (void *) cs_uart1_init;
    gd->jt[XF_cs_uart1_setattr] = (void *) cs_uart1_setattr;
    gd->jt[XF_cs_uart1_write] = (void *) cs_uart1_write;
    gd->jt[XF_cs_uart1_read] = (void *) cs_uart1_read;
    gd->jt[XF_cs_uart1_disable] = (void *) cs_uart1_disable;
    gd->jt[XF_cs_uart1_enable] = (void *) cs_uart1_enable;
 
	gd->jt[XF_gpio_hw_set_direct] = (void *) gpio_hw_set_direct;
	gd->jt[XF_gpio_hw_read] = (void *) gpio_hw_read;
	gd->jt[XF_gpio_hw_write] = (void *) gpio_hw_write;

	gd->jt[XF_uart_read] = (void *) uart_read;
	gd->jt[XF_uart_write] = (void *) uart_write;
	gd->jt[XF_uart_tstc] = (void *) uart_tstc;

	gd->jt[XF_DrawPixel] = (void *) DrawPixel;
	gd->jt[XF_DrawHLine] = (void *) DrawHLine;
	gd->jt[XF_DrawVLine] = (void *) DrawVLine;
	gd->jt[XF_DrawLine] = (void *) DrawLine;
	gd->jt[XF_DrawRectangle] = (void *) DrawRectangle;
	gd->jt[XF_FillRectangle] = (void *) FillRectangle;
	gd->jt[XF_DrawText] = (void *) DrawText;

#endif	/* celestial */
}
