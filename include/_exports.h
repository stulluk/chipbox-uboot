EXPORT_FUNC(get_version)
EXPORT_FUNC(getc)
EXPORT_FUNC(tstc)
EXPORT_FUNC(putc)
EXPORT_FUNC(puts)
EXPORT_FUNC(printf)
EXPORT_FUNC(install_hdlr)
EXPORT_FUNC(free_hdlr)
EXPORT_FUNC(malloc)
EXPORT_FUNC(free)
EXPORT_FUNC(udelay)
EXPORT_FUNC(get_timer)
EXPORT_FUNC(vprintf)
EXPORT_FUNC(do_reset)
EXPORT_FUNC(getenv)
EXPORT_FUNC(setenv)
EXPORT_FUNC(simple_strtoul)
#if (CONFIG_COMMANDS & CFG_CMD_I2C)
EXPORT_FUNC(i2c_write)
EXPORT_FUNC(i2c_read)
#endif	/* CFG_CMD_I2C */

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
EXPORT_FUNC(eeprom_write)
EXPORT_FUNC(eeprom_read)
#endif 

#if defined(CONFIG_CELESTIAL)

#ifdef CONFIG_USE_IRQ 
EXPORT_FUNC(orion_irq_enable)
EXPORT_FUNC(orion_irq_disable)
EXPORT_FUNC(orion_irq_ack)
EXPORT_FUNC(orion_register_irq)
EXPORT_FUNC(orion_unregister_irq)
#endif /* CONFIG_USE_IRQ */


/* Old API */
EXPORT_FUNC(xport_init)
EXPORT_FUNC(xport_set_channel_status)
EXPORT_FUNC(xport_set_channel_inputmode)
EXPORT_FUNC(xport_pidft_bind_chl)
EXPORT_FUNC(xport_pidft_set_pid)
EXPORT_FUNC(xport_pidft_set_status)
EXPORT_FUNC(xport_filter_set_filter)
EXPORT_FUNC(xport_filter_set_pid)
EXPORT_FUNC(xport_filter_set_type)
EXPORT_FUNC(xport_filter_set_status)
EXPORT_FUNC(xport_filter_wait_data)
EXPORT_FUNC(xport_fliter_read_data)

/* New API */
EXPORT_FUNC(CSDEMUX_Init)
EXPORT_FUNC(CSDEMUX_CHL_Open)
EXPORT_FUNC(CSDEMUX_CHL_Close)
EXPORT_FUNC(CSDEMUX_CHL_SetInputMode)
EXPORT_FUNC(CSDEMUX_CHL_Enable)
EXPORT_FUNC(CSDEMUX_CHL_Disable)
EXPORT_FUNC(CSDEMUX_PIDFT_Open)
EXPORT_FUNC(CSDEMUX_PIDFT_Close)
EXPORT_FUNC(CSDEMUX_PIDFT_SetChannel)
EXPORT_FUNC(CSDEMUX_PIDFT_SetPID)
EXPORT_FUNC(CSDEMUX_PIDFT_Enable)
EXPORT_FUNC(CSDEMUX_PIDFT_Disable)
EXPORT_FUNC(CSDEMUX_Filter_Open)
EXPORT_FUNC(CSDEMUX_Filter_Close)
EXPORT_FUNC(CSDEMUX_Filter_SetFilterType)
EXPORT_FUNC(CSDEMUX_Filter_SetFilter)
EXPORT_FUNC(CSDEMUX_Filter_AddPID)
EXPORT_FUNC(CSDEMUX_Filter_ResetPID)
EXPORT_FUNC(CSDEMUX_Filter_ReadWait)
EXPORT_FUNC(CSDEMUX_Filter_CheckDataSize)
EXPORT_FUNC(CSDEMUX_Filter_GetSectionCount)
EXPORT_FUNC(CSDEMUX_Filter_ReadSectionData)
EXPORT_FUNC(CSDEMUX_Filter_Enable)
EXPORT_FUNC(CSDEMUX_Filter_Disable)

EXPORT_FUNC(CSTUNER_Init)
EXPORT_FUNC(CSTUNER_Open)
EXPORT_FUNC(CSTUNER_SetFrequency)
EXPORT_FUNC(CSTUNER_GetTunerInfo)
EXPORT_FUNC(CSTUNER_ReadStatus)

#ifndef MERIH_GOLD_BOARD   // by kb :20100416
EXPORT_FUNC(fp_set_led_display_pos)
EXPORT_FUNC(fp_set_led_dispaly_mode)
EXPORT_FUNC(fp_set_led_dispaly_char)
EXPORT_FUNC(fp_set_led_display_raw)
EXPORT_FUNC(fp_enable_keyscan)
EXPORT_FUNC(fp_disable_keyscan)
EXPORT_FUNC(fp_set_keyscan_map)
EXPORT_FUNC(fp_get_keyscan_input)
#endif //#ifndef MERIH_GOLD_BOARD
// EXPORT_FUNC(fp_enable_serial_in)
// EXPORT_FUNC(fp_disable_serial_in)
// EXPORT_FUNC(fp_reset_serial_in)
// EXPORT_FUNC(fp_set_serial_in_attr)
// EXPORT_FUNC(fp_write_serial_in)

 EXPORT_FUNC(cs_uart1_init)
 EXPORT_FUNC(cs_uart1_setattr)
 EXPORT_FUNC(cs_uart1_write)
 EXPORT_FUNC(cs_uart1_read)
 EXPORT_FUNC(cs_uart1_disable)
 EXPORT_FUNC(cs_uart1_enable)


/* For gpio read/write */
EXPORT_FUNC(gpio_hw_set_direct)
EXPORT_FUNC(gpio_hw_read)
EXPORT_FUNC(gpio_hw_write)

/* For uart update */
EXPORT_FUNC(uart_read)
EXPORT_FUNC(uart_write)
EXPORT_FUNC(uart_tstc)

/* draw interfaces */
EXPORT_FUNC(DrawPixel)
EXPORT_FUNC(DrawHLine)
EXPORT_FUNC(DrawVLine)
EXPORT_FUNC(DrawLine)
EXPORT_FUNC(DrawRectangle)
EXPORT_FUNC(FillRectangle)
EXPORT_FUNC(DrawText)

#endif /* (CONFIG_CELESTIAL) */
