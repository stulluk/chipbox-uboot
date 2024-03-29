/* cn100 net driver */

#ifdef CONFIG_DRIVER_CN100

#define CN100_BASEADDRESS			0x41400000

#define	ETH_CFG_ADDR				( CN100_BASEADDRESS + 0x0800 )
#define	ETH_INT_REG_ADDR        ( CN100_BASEADDRESS + 0x0840 )
#define	ETH_INT_ENB_ADDR        ( CN100_BASEADDRESS + 0x0844 )
#define	ETH_INT_SET_ADDR        ( CN100_BASEADDRESS + 0x0848 )
#define	ETH_INT_CLS_ADDR        ( CN100_BASEADDRESS + 0x084c )	
#define	ETH_TX_DMA_ADDR        	( CN100_BASEADDRESS + 0x09c0 )	
#define	ETH_TX_STATES_ADDR     	( CN100_BASEADDRESS + 0x09c4 )	
#define	ETH_RX_BASE_ADDR       	( CN100_BASEADDRESS + 0x09c8 )	
#define	ETH_RX_CFG_ADDR        	( CN100_BASEADDRESS + 0x09cc )	
#define	ETH_RX_RD_ADDR         	( CN100_BASEADDRESS + 0x09d0 )	
#define	ETH_RX_WR_ADDR         	( CN100_BASEADDRESS + 0x09d4 )	
#define	ETH_RX_STATES_ADDR     	( CN100_BASEADDRESS + 0x09dc )	
#define	ETH_MAC_SA_MASK        	( CN100_BASEADDRESS + 0x0a98 )	
#define	ETH_MAC_HASH_TAB0      	( CN100_BASEADDRESS + 0x0a9c )	
#define	ETH_MAC_HASH_TAB1      	( CN100_BASEADDRESS + 0x0aa0 )
                                                             
#define	ETH_MAC_CFG0          	( CN100_BASEADDRESS + 0x0a40 )	
#define	ETH_MAC_CFG1          	( CN100_BASEADDRESS + 0x0a44 )	
#define	ETH_MAC_IPGT          	( CN100_BASEADDRESS + 0x0a48 )	
#define	ETH_MAC_IPGR          	( CN100_BASEADDRESS + 0x0a4c )	
#define	ETH_MAC_CLRT          	( CN100_BASEADDRESS + 0x0a50 )	
#define	ETH_MAC_MAXF          	( CN100_BASEADDRESS + 0x0a54 )	
#define	ETH_MAC_SUPP          	( CN100_BASEADDRESS + 0x0a58 )	
#define	ETH_MAC_TEST          	( CN100_BASEADDRESS + 0x0a5c )	
#define	ETH_MAC_MCFG          	( CN100_BASEADDRESS + 0x0a60 )	
#define	ETH_MAC_MCMD          	( CN100_BASEADDRESS + 0x0a64 )	
#define	ETH_MAC_MADR          	( CN100_BASEADDRESS + 0x0a68 )	
#define	ETH_MAC_MWTD          	( CN100_BASEADDRESS + 0x0a6c )	
#define	ETH_MAC_MRDD          	( CN100_BASEADDRESS + 0x0a70 )	
#define	ETH_MAC_MIND          	( CN100_BASEADDRESS + 0x0a74 )	
#define	ETH_MAC_SA0           	( CN100_BASEADDRESS + 0x0a80 )	
#define	ETH_MAC_SA1           	( CN100_BASEADDRESS + 0x0a84 )	
#define	ETH_MAC_SA2           	( CN100_BASEADDRESS + 0x0a88 )	
#define	ETH_MAC_TSV0           	( CN100_BASEADDRESS + 0x0a8c)	
#define	ETH_MAC_TSV1           	( CN100_BASEADDRESS + 0x0a90)	
#define	ETH_MAC_RSV            	( CN100_BASEADDRESS + 0x0a94)	

#endif /* CONFIG_DRIVER_CN100 */
