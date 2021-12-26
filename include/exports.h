#ifndef __EXPORTS_H__
#define __EXPORTS_H__

#ifndef __ASSEMBLY__

#include <common.h>
#include <orion_csdemux.h>
#include <orion_tuner.h>
#include <orion_fp.h>
#include "../common/sketch.h"
#include <csm120x_uart1_drv.h>

/* These are declarations of exported functions available in C code */
unsigned long get_version(void);
int  getc(void);
int  tstc(void);
void putc(const char);
void puts(const char*);
void printf(const char* fmt, ...);
void install_hdlr(int, interrupt_handler_t*, void*);
void free_hdlr(int);
void *malloc(size_t);
void free(void*);
void udelay(unsigned long);
unsigned long get_timer(unsigned long);
void vprintf(const char *, va_list);
void do_reset (void);
unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
char *getenv (char *name);
void setenv (char *varname, char *varvalue);

#if (CONFIG_COMMANDS & CFG_CMD_I2C)
int i2c_write (uchar, uint, int , uchar* , int);
int i2c_read (uchar, uint, int , uchar* , int);
#endif	/* CFG_CMD_I2C */


#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
int eeprom_read (unsigned dev_addr, unsigned offset, uchar *buffer, unsigned cnt);
int eeprom_write (unsigned dev_addr, unsigned offset, uchar *buffer, unsigned cnt);
#endif /* EEPROM*/


#if defined(CONFIG_CELESTIAL)

#ifdef CONFIG_USE_IRQ 
typedef int (* irq_handle)(unsigned int, void *);
void orion_irq_enable(unsigned int irq);
void orion_irq_disable(unsigned int irq);
void orion_irq_ack(unsigned int irq);
void orion_register_irq(int irq, irq_handle irq_r, void * irq_data);
void orion_unregister_irq(int irq);
#endif /* CONFIG_USE_IRQ */

/*
*  Old API. New version is in orion_csdemux.h.
*/
int _xport_load_firmware(void);
int xport_hw_init(void);

int xport_init(void);
int xport_set_channel_status(int chl_num,int status);
int xport_set_channel_inputmode(int chl_num,int input_mode);
int xport_pidft_bind_chl(int chl_num,int pidft_index);
int xport_pidft_set_pid(int pidft_index,short int pid);
int xport_pidft_set_status(int pidft_index,int status);
int xport_filter_set_filter(int filter_index, unsigned char *filter, unsigned char *mask);
int xport_filter_set_pid(unsigned int filter_index, unsigned int pid);
int xport_filter_set_type(int filter_index, int filter_type);
int xport_filter_set_status(int filter_index, int status);
int xport_filter_wait_data(int filter_index, int ms_timeout);
int xport_fliter_read_data(int filter_index, unsigned char *buf, unsigned int size);
int xport_filter_clear_buffer(unsigned int filter_index);
int xport_filter_check_section_number(unsigned int filter_index);
int xport_filter_check_section_count(unsigned int filter_index);
#endif /* CONFIG_CELESTIAL */

void app_startup(char **);

#endif    /* ifndef __ASSEMBLY__ */

enum {
#define EXPORT_FUNC(x) XF_ ## x ,
#include <_exports.h>
#undef EXPORT_FUNC

	XF_MAX
};

#define XF_VERSION	3

#if defined(CONFIG_I386)
extern gd_t *global_data;
#endif

#endif	/* __EXPORTS_H__ */
