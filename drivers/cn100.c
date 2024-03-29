#include <common.h>
#include <command.h>
#include <net.h>
#include <asm/io.h>

#ifdef CONFIG_DRIVER_CN100

#include "cn100.h"

// #define CONFIG_CN100_DEBUG

#ifdef CONFIG_CN100_DEBUG
#define CN100_DBG(fmt,args...) printf(fmt ,##args)
#else
#define CN100_DBG(fmt,args...)
#endif
#define CARDNAME 				"CN100"
#define PRINTK3(args...)  	printf(CARDNAME ": " args)


/* Structure/enum declaration ------------------------------- */
#define SWAP(x) ( ( ( x >> 24 ) & 0xFF ) | ( ( x >> 8 ) & 0xFF00 ) | ( ( x << 8 ) & 0xFF0000 ) | ( ( x << 24 ) & 0xFF000000 ) )

#define CONFIG_CN100_DATA_BASE	0x07C00000

#define RX_BUFFER_ITEM_SIZE 		1600
#define RX_BUFFER_ITEM_NUM 		128
#define TX_BUFFER_NOD_LEN			( 16 )
#define TX_BUFFER_DATA_LEN			( 2048 )
#define TX_BUFFER_LEN				( TX_BUFFER_NOD_LEN +  TX_BUFFER_DATA_LEN )
#define RX_BUFFER_LEN				( RX_BUFFER_ITEM_SIZE * RX_BUFFER_ITEM_NUM )

/* For default MAC address By KB Kim : 07 Jan 2011 */
#define DEFAULT_MAC_ADDR_0   0x02
#define DEFAULT_MAC_ADDR_1   0x03
#define DEFAULT_MAC_ADDR_2   0x04

typedef struct buffer_t
{
	u32 	size;
	u32 	phy_address;
	u32 	vir_address;
} buffer_f;

typedef struct tx_buffer_t
{
	u32 buffer_addr;
	u32 buffer_len;
	u32 next_link_item;
	u32 next_item_valid_flag;
} tx_buffer_f;

typedef struct rx_buffer_t
{
	u32 rx_buffer_addr[RX_BUFFER_ITEM_NUM];
	u32 buffer_num;
} rx_buffer_f;

struct buffer_t 				tx_buffer_t;
struct tx_buffer_t 		*	tx_buffer = NULL;
struct rx_buffer_t 			rx_buffer;	

/* function declaration ------------------------------------- */
int 			eth_init			(bd_t * bd);
int 			eth_send			(volatile void *, int);
int 			eth_rx			(void);
void 			eth_halt			(void);

int cn100_eth_readmac(bd_t *bd);
static int 	init_eth_hw 	( void );
static int 	release_eth_hw ( void );
static int 	init_phy_hw 	( void );
static int 	release_phy_hw ( void );
static void set_tx_int 		( int enable );
static void set_rx_enable 	( int enable );
static void set_rx_int 		( int enable );
static int 	set_tx_buffer 	( void );
static int 	set_rx_buffer 	( void );
static void set_hw_clock 	( void );
int 	set_mac_addr 	( u8 mac_addr[6] );   // by kb :20100416
static void cn100_reset		( void );


/* CN100 network board routine ---------------------------- */
// #define CN100_outb(d,r) 	( *(volatile u8 *)r = d )
// #define CN100_outw(d,r) 	( *(volatile u16 *)r = d )
//#define CN100_outl(d,r) 	( *(volatile u32 *)r = d )
#define CN100_outl(r,d) 	( *(volatile u32 *)r = d )
// #define CN100_inb(r) 		(*(volatile u8 *)r)
// #define CN100_inw(r) 		(*(volatile u16 *)r)
#define CN100_inl(r) 		(*(volatile u32 *)r)

#ifdef CONFIG_CN100_DEBUG
static void
dump_regs(void)
{
	CN100_DBG("\n");
	CN100_DBG ( "ETH_CFG_ADDR: 		0x%08x\n", 			CN100_inl (( CN100_BASEADDRESS + 0x0800 ) ) );
	CN100_DBG ( "ETH_INT_REG_ADDR: 	0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0840 ) ) );
	CN100_DBG ( "ETH_INT_ENB_ADDR: 	0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0844 ) ) );
	CN100_DBG ( "ETH_TX_DMA_ADDR: 	0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x09c0 ) ) );	
	CN100_DBG ( "ETH_TX_STATES_ADDR: 0x%08x\n",      	CN100_inl (( CN100_BASEADDRESS + 0x09c4 ) ) );	
	CN100_DBG ( "ETH_RX_BASE_ADDR: 	0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x09c8 ) ) );	
	CN100_DBG ( "ETH_RX_CFG_ADDR: 	0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x09cc ) ) );	
	CN100_DBG ( "ETH_RX_RD_ADDR: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x09d0 ) ) );	
	CN100_DBG ( "ETH_RX_WR_ADDR: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x09d4 ) ) );	
	CN100_DBG ( "ETH_RX_STATES_ADDR: 0x%08x\n",      	CN100_inl (( CN100_BASEADDRESS + 0x09dc ) ) );	
	CN100_DBG ( "ETH_MAC_SA_MASK: 	0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a98 ) ) );	
	CN100_DBG ( "ETH_MAC_HASH_TAB0: 	0x%08x\n",       	CN100_inl (( CN100_BASEADDRESS + 0x0a9c ) ) );	
	CN100_DBG ( "ETH_MAC_HASH_TAB1: 	0x%08x\n",       	CN100_inl (( CN100_BASEADDRESS + 0x0aa0 ) ) );
	                                                            
	CN100_DBG ( "ETH_MAC_CFG0: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a40 ) ) );	
	CN100_DBG ( "ETH_MAC_CFG1: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a44 ) ) );	
	CN100_DBG ( "ETH_MAC_IPGT: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a48 ) ) );	
	CN100_DBG ( "ETH_MAC_IPGR: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a4c ) ) );	
	CN100_DBG ( "ETH_MAC_CLRT: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a50 ) ) );	
	CN100_DBG ( "ETH_MAC_MAXF: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a54 ) ) );	
	CN100_DBG ( "ETH_MAC_SUPP: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a58 ) ) );	
	CN100_DBG ( "ETH_MAC_TEST: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a5c ) ) );	
	CN100_DBG ( "ETH_MAC_MCFG: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a60 ) ) );	
	CN100_DBG ( "ETH_MAC_MCMD: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a64 ) ) );	
	CN100_DBG ( "ETH_MAC_MADR: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a68 ) ) );	
	CN100_DBG ( "ETH_MAC_MWTD: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a6c ) ) );	
	CN100_DBG ( "ETH_MAC_MRDD: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a70 ) ) );	
	CN100_DBG ( "ETH_MAC_MIND: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a74 ) ) );	
	CN100_DBG ( "ETH_MAC_SA0: 			0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a80 ) ) );	
	CN100_DBG ( "ETH_MAC_SA1: 			0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a84 ) ) );	
	CN100_DBG ( "ETH_MAC_SA2: 			0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a88 ) ) );	
	CN100_DBG ( "ETH_MAC_TSV0: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a8c ) ) );	
	CN100_DBG ( "ETH_MAC_TSV1: 		0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a90 ) ) );	
	CN100_DBG ( "ETH_MAC_RSV: 			0x%08x\n",        CN100_inl (( CN100_BASEADDRESS + 0x0a94 ) ) );	
	CN100_DBG("\n");
}
#endif
 
static int 
alloc_eth_mem ( void )
{
	s32 i = 0;
	u32 value = 0;
//	PRINTK3 ( "alloc_eth_mem ...\n" );

	/* init tx data buffer struct */
	tx_buffer = (struct tx_buffer_t *)CONFIG_CN100_DATA_BASE;
	tx_buffer_t.size = TX_BUFFER_DATA_LEN;
	tx_buffer_t.vir_address = (u32)(CONFIG_CN100_DATA_BASE + TX_BUFFER_NOD_LEN );
	tx_buffer_t.phy_address = CONFIG_CN100_DATA_BASE + TX_BUFFER_NOD_LEN;
	/* init tx node buffer struct */
	value = tx_buffer_t.phy_address;
	tx_buffer->buffer_addr = SWAP ( value );
	tx_buffer->buffer_len = 0;
	tx_buffer->next_link_item = 0;
	tx_buffer->next_item_valid_flag = 0;
	
	/* init rx buffer struct */
	rx_buffer.buffer_num = RX_BUFFER_ITEM_NUM;
	for ( i = 0; i < RX_BUFFER_ITEM_NUM; i ++ )
	{
		rx_buffer.rx_buffer_addr[i] = (u32)(CONFIG_CN100_DATA_BASE + TX_BUFFER_LEN + RX_BUFFER_ITEM_SIZE * i );
	}
	return 0;
}

static int 
release_eth_mem ( void )
{
	s32 i = 0;
	PRINTK3 ( "release_eth_mem ...\n" );

	if ( NULL != tx_buffer )
	{
		tx_buffer = NULL;
	}
	tx_buffer_t.size = 0;
	tx_buffer_t.phy_address = 0;
	tx_buffer_t.vir_address = 0;
	
	/* RX */
	for ( i = 0; i < rx_buffer.buffer_num; i ++ )
	{
		rx_buffer.rx_buffer_addr[i] = 0;
	}
	rx_buffer.buffer_num = 0;
	
	return 0;
}


static void 
set_tx_int ( int enable )
{
	u32 value = 0;
	value = CN100_inl ( ETH_INT_ENB_ADDR );
	if ( enable )
	{
		value |= 0x01;
	}
	else
	{
		value &= 0xFFFFFFFE;
	}
	CN100_outl ( ETH_INT_ENB_ADDR, value );
}

static int 
init_eth_hw ( void )
{
	u32 value = 0;
	
//	PRINTK3("entering %s\n",__FUNCTION__);
	cn100_reset 	( );
	set_rx_int	  	( 0 );
	set_tx_int	  	( 0 );	
	set_rx_enable 	( 0 );
	set_rx_buffer 	( );

	CN100_outl 		(ETH_CFG_ADDR, 0x00005500 );
	CN100_outl 		(ETH_MAC_SUPP, 0x00000100 );
	CN100_outl 		(ETH_MAC_IPGT, 0x00000015 );
	CN100_outl 		(ETH_MAC_IPGR, 0x00000c12 );
	CN100_outl 		(ETH_MAC_SA_MASK, 0x3 );
	CN100_outl 		(ETH_MAC_HASH_TAB0, 0x0 );
	CN100_outl 		(ETH_MAC_HASH_TAB1, 0x0 );
	value = CN100_inl (ETH_MAC_CFG1 );
	value |= 0x1;
	CN100_outl 		(ETH_MAC_CFG1, value );
	set_rx_enable 	( 1 );
	set_rx_int	  	( 1 );
	set_tx_int    	( 1 );
	set_hw_clock  	(  );
	return 0;
}

static int 
release_eth_hw ( void )
{
	/* free interrupt */
	cn100_reset ( );
	return 0;
}

static int 
init_phy_hw ( void )
{
	//PRINTK3 ( "entering init_phy_hw\n");
	return 0;
}

static int 
release_phy_hw ( void )
{
	//PRINTK3 ( "entering release_phy_hw\n");
	return 0;
}

static void 
set_rx_enable ( int enable )
{
	u32 value = 0;
	value = CN100_inl (ETH_MAC_CFG0 );
	if ( enable )
	{
		value |= 0x01;
	}
	else
	{
		value &= 0xFFFFFFFE;
	}
	CN100_outl (ETH_MAC_CFG0, value );
	value = CN100_inl (ETH_RX_CFG_ADDR );
	if ( enable )
	{
		value |= 0x80000000;
	}
	else
	{
		value &= 0x7FFFFFFF;
	}
	CN100_outl (ETH_RX_CFG_ADDR, value );
}

static void 
set_rx_int ( int enable )
{
	u32 value = 0;
	value = CN100_inl (ETH_INT_ENB_ADDR );
	if ( enable )
	{
		value |= 0x02;
	}
	else
	{
		value &= 0xFFFFFFFD;
	}
	CN100_outl (ETH_INT_ENB_ADDR, value );
}

static void 
set_hw_clock ( )
{
	CN100_outl (ETH_MAC_MCFG, 0x1c );
}

int 
set_mac_addr ( u8 mac_addr[6] )   // by kb :20100416
{
	u32 value = 0;	
	/* Set Node Address */
	value = (u32)mac_addr[1];
	value = (value<<8) | mac_addr[0];
	CN100_outl (ETH_MAC_SA2, value );
	value = (u32)mac_addr[3];
	value = (value<<8) | mac_addr[2];
	CN100_outl (ETH_MAC_SA1, value );
	value = (u32)mac_addr[5];
	value = (value<<8) | mac_addr[4];
	CN100_outl (ETH_MAC_SA0, value );
	
	return 0;
}

static int 
set_rx_buffer ( void )
{
	u32 value = 0;
	
	if ( 0 == rx_buffer.buffer_num )
	{
		return -1;
	}
	/* get virtual map physcial address */
	value = CONFIG_CN100_DATA_BASE + TX_BUFFER_LEN;
	CN100_outl (ETH_RX_BASE_ADDR, ( value >> 3 ) );
	value = rx_buffer.buffer_num;
	value = ( ( value << 8 ) & 0xFF00 ) | ( ( RX_BUFFER_ITEM_SIZE >> 3 ) & 0xFF ) ;
	CN100_outl (ETH_RX_CFG_ADDR, value );	
	/* set output ptr equal input ptr */
	value = CN100_inl (ETH_RX_WR_ADDR );
	CN100_outl (ETH_RX_RD_ADDR, value );
	
	return 0;
}

static int 
set_tx_buffer 	( void )
{
	u32 value = 0;
	if ( NULL == tx_buffer )
	{
		return -1;
	}
	value = CONFIG_CN100_DATA_BASE;
	CN100_outl (ETH_TX_DMA_ADDR, value );
	return 0;
}

/* General Purpose cn100 reset routine */
static void
cn100_reset(void)
{
	CN100_DBG ( "resetting\n" );
	u32 value = 0x000000b7;
	/* disable rx and tx */
	set_rx_int	  ( 0 );
	set_tx_int	  ( 0 );	
	set_rx_enable ( 0 );
	
	CN100_outl ( ETH_MAC_CFG0, 0x00008000 );
	udelay ( 200 );
	udelay ( 200 );
	udelay ( 200 );
	udelay ( 200 );
	CN100_outl ( ETH_MAC_CFG0, 0x00000001 );
	CN100_outl ( ETH_INT_SET_ADDR, 0 );
	CN100_outl ( ETH_MAC_CFG1, value );
	udelay(1000);		/* delay 1ms */
}

// by kb :20100416
/* For default MAC address By KB Kim : 07 Jan 2011 */
u8 CheckValidMac(u8 *macAddress)
{
	if ((macAddress[0] == DEFAULT_MAC_ADDR_0) &&
		(macAddress[1] == DEFAULT_MAC_ADDR_1) &&
		(macAddress[2] == DEFAULT_MAC_ADDR_2))
	{
		return 0;
	}

	return 1;
}

int cn100_eth_readmac(bd_t * bd)
{
	unsigned int macsum=0;
	unsigned int envMacsum=0;
	int i;
	int len;
	ulong reg;
	char *s, *e;
	char tmp[64];
	u8   envENetAddr[6];

#if 0
    for (i=0; i<6; i++)
    {
        macsum +=((u8 *) bd->bi_enetaddr)[i];
    }
	
    if (macsum == 0)
#endif
     {
#if (CONFIG_COMMANDS & CFG_CMD_EEPROM) && defined(CFG_ORION_ETHADDR_FROM_EEPROM)
        eeprom_read(CFG_I2C_EEPROM_ADDR, CFG_ORION_ETH_EEPROM_OFFSET, ((u8 *) bd->bi_enetaddr), 6);

	 	macsum =0;
        for (i=0; i<6; i++)
        {
            macsum += (unsigned int)bd->bi_enetaddr[i];
        }
            
#elif !defined(CFG_ORION_NAND_BOOT) && defined(CFG_ORION_ETHADDR_FROM_NOR_FLASH)
        u8 *MacAddr = (u8 *)PA_ETHMAC;

	 	macsum =0;
        for (i = 0; i < 6; i++)
        {
            ((u8 *) bd->bi_enetaddr)[i] = MacAddr[i];
            macsum +=((u8 *) bd->bi_enetaddr)[i];
        }
#endif
		len = getenv_r ("ethaddr", tmp, sizeof (tmp));
		if (len > 0)
		{
			s = tmp;
			printf("KB : cn100_eth_readmac ethaddr %s\n", tmp);
		}
		else
		{
			s = NULL;
		}

		/* For default MAC address By KB Kim : 07 Jan 2011 */
        // if (macsum == 0 || macsum == 255*6)
        if (CheckValidMac((u8 *)bd->bi_enetaddr))
        {
			printf("No MAC Address in EEPROM : Get from Env\n");
			for (reg = 0; reg < 6; ++reg)
			{
				bd->bi_enetaddr[reg] = s ? simple_strtoul (s, &e, 16) : 0;
				if (s)
				{
					s = (*e) ? e + 1 : e;
				}
			}

			/* For default MAC address By KB Kim : 07 Jan 2011 */
			/*
			macsum =0;
	        for (i=0; i<6; i++)
	            macsum += (unsigned int)bd->bi_enetaddr[i];
	            */

			/* For default MAC address By KB Kim : 07 Jan 2011 */
			// if (macsum == 0 || macsum == 255*6)
			if (CheckValidMac((u8 *)bd->bi_enetaddr))
			{
				printf("No MAC Address in EEPROM and Env : Set Default Value");
				
				/* For default MAC address By KB Kim : 07 Jan 2011 */
				((u8 *) bd->bi_enetaddr)[0] = DEFAULT_MAC_ADDR_0;
				((u8 *) bd->bi_enetaddr)[1] = DEFAULT_MAC_ADDR_1;
				((u8 *) bd->bi_enetaddr)[2] = DEFAULT_MAC_ADDR_2;
				((u8 *) bd->bi_enetaddr)[3] = 0x00;
				((u8 *) bd->bi_enetaddr)[4] = 0x00;
				((u8 *) bd->bi_enetaddr)[5] = 0x00;
			}

			eeprom_write (CFG_I2C_EEPROM_ADDR, CFG_ORION_ETH_EEPROM_OFFSET, ((u8 *) bd->bi_enetaddr), 6);
        }
		else
		{
			/* We get MacAddress from EEPRom */
			/* Just Compare with Env Value and update it */
			for (reg = 0; reg < 6; ++reg)
			{
				envENetAddr[reg] = s ? simple_strtoul (s, &e, 16) : 0;
				if (s)
				{
					s = (*e) ? e + 1 : e;
				}
			}
			envMacsum =0;
	        for (i=0; i<6; i++)
	        {
	            envMacsum += (unsigned int)envENetAddr[i];
	        }

			if (macsum != envMacsum)
			{
				/* Env Mac Address is not same */
				memset(tmp, 0x00, 64);
				sprintf(tmp, "%02X:%02X:%02X:%02X:%02X:%02X",
					         bd->bi_enetaddr[0], bd->bi_enetaddr[1], bd->bi_enetaddr[2],
					         bd->bi_enetaddr[3], bd->bi_enetaddr[4], bd->bi_enetaddr[5]);
				printf("Mac Address are not Matched with %s --> Change it\n", tmp);
				setenv ("ethaddr", tmp);
				saveenv();
			}
		}
    }

	printf("MAC1: %02x:%02x:%02x:%02x:%02x:%02x\n", bd->bi_enetaddr[0],
	       bd->bi_enetaddr[1], bd->bi_enetaddr[2], bd->bi_enetaddr[3],
	       bd->bi_enetaddr[4], bd->bi_enetaddr[5]);
    set_mac_addr ( ((u8 *) bd->bi_enetaddr) );

	return 0;

}


/* Initilize cn100 board
*/
int 
eth_init ( bd_t * bd )
{
	
	s32 ret = 0;
	
	CN100_DBG("eth_init()\n");
   

	if ( 0 != ( ret = alloc_eth_mem ( ) ) )
	{
		printf ( "alloc eth memory resource fail\n" );
		goto ERROR_ALLOC_ETH_MEM;
	}
	/* init eth hard register */
	if ( 0 != ( ret = init_eth_hw ( ) ) )
	{
		printf ( "init eth hw resource fail\n");
		goto ERROR_INIT_ETH_HW;
	}

	/* init phy hard register */
	if ( 0 != ( ret = init_phy_hw ( ) ) )
	{
		printf ( "init phy resource fail\n");
		goto ERROR_INIT_PHY_HW;
	}
	return 0;

ERROR_INIT_PHY_HW:
	release_eth_hw ( );
ERROR_INIT_ETH_HW:
	release_eth_mem ( );
ERROR_ALLOC_ETH_MEM:
	return -2;	
}

/*
  Hardware start transmission.
  Send a packet to media from the upper layer.
*/
int
eth_send(volatile void *packet, int length)
{
	u32 i, value;

	CN100_DBG("eth_send: length: %d\n", length);
	
	for (i = 0; i < length; i++) 
	{
		if (i % 8 == 0)
		{
			CN100_DBG("\nSend: 02x: ", i);
		}
		CN100_DBG("%02x ", ((unsigned char *) packet)[i]);
	} 
	CN100_DBG("\n");
	memcpy (  (void *)(u32 *)(tx_buffer_t.vir_address),
				 (char *) packet,
				 length );
	tx_buffer->buffer_len = SWAP ( length );
	set_tx_buffer ( );
	value = 0;
	while ( !( value & 0x1 ) ) 
	{
		value = CN100_inl ( ETH_INT_REG_ADDR );
//		printf ( "current interrupt: 0x%08x\n", value );
	}
	/* clr send interrupt */
	CN100_outl ( ETH_INT_CLS_ADDR, 0x01  );
	CN100_DBG("transmit done\n\n");
	return 0;
}

/*
  Stop the interface.
  The interface is stopped when it is brought.
*/
void
eth_halt(void)
{
	CN100_DBG ( "eth_halt\n" );
	cn100_reset ( );
	CN100_DBG ( "eth_halt finish\n" );
}

/*
  Received a packet and pass to upper layer
*/
int
eth_rx(void)
{
	u8 *rdptr = (u8 *) NetRxPackets[0];
	u32 RxLen = 0;
	u32 isr;
	u32 write_ptr;
	u32 read_ptr;
	
	CN100_DBG ( "eth_rx be call\n");
	/* Check packet ready or not */
	isr = CN100_inl( ETH_INT_REG_ADDR );
	if(!( isr & 0x2 ) )
   {
   	return 0;
	}
	/* clr rx interrupt */
	CN100_outl ( ETH_INT_CLS_ADDR, 0x02 );
	if ( isr & 0x3 )
	{
		CN100_outl ( ETH_INT_CLS_ADDR, 0x03 );
	}
	
	CN100_DBG ( "receiving packet\n");
	write_ptr = CN100_inl ( ETH_RX_WR_ADDR );
	read_ptr = CN100_inl ( ETH_RX_RD_ADDR );	
//	printf ( "w: 0x%08x, r: 0x%08x\n", write_ptr, read_ptr );
	RxLen = *(volatile u32 *)( rx_buffer.rx_buffer_addr[read_ptr & 0x3FFFFFFF] + 4 );
	RxLen = SWAP ( RxLen );
	CN100_DBG("rx len: %d\n", RxLen);

	/* Move data from CN100 */
	memcpy ( (void *)rdptr, 
				(void *)(u32 *)(rx_buffer.rx_buffer_addr[read_ptr & 0x3FFFFFFF] + 8),
				RxLen );
				
	if( ( read_ptr & 0x3FFFFFFF ) < ( rx_buffer.buffer_num - 1 ) )
	{
		read_ptr ++;
	}
	else
	{
		read_ptr = ( (~read_ptr) & 0x80000000 );
	}
	
	CN100_outl ( ETH_RX_RD_ADDR, read_ptr );
	
	if ( RxLen > RX_BUFFER_ITEM_SIZE - 16 ) 
	{
		printf("rx length too big\n");
		cn100_reset();
	}
	else 
	{
		/* Pass to upper layer */
		CN100_DBG("passing packet to upper layer\n");
		NetReceive(NetRxPackets[0], RxLen);
		return RxLen;
	}
	return 0;
}

#endif /* CONFIG_DRIVER_CN100 */
